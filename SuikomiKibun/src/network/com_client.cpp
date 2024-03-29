#include "com_client.h"

ComClientTcp::ComClientTcp(asio::io_service &io_service, int port, Server* se) :
		io_service_(io_service), server_(se), accept_timer_(io_service_), send_timer_(io_service_), receive_timer_(
				io_service_), kPort(port) {
	//メンバー変数初期化
	memset(&send_data_, 0, sizeof(send_data_));
	memset(&receive_data_, 0, sizeof(receive_data_));
	has_accepted_ = false;
	//StartAcceptで作成
	socket_ = NULL;
	acceptor_ = NULL;
	//状態
	is_tcp_ = true;
}

ComClientUdp::ComClientUdp(asio::io_service &io_service, int port, Server* se) :
		ComClientTcp(io_service, port, se) {
	//StartAcceptで作成
	send_socket_ = NULL;
	receive_socket_ = NULL;
	//使用しない
	acceptor_ = NULL;
	//状態
	is_tcp_ = false;
}

ComClientTcp::~ComClientTcp() {
	// タイムアウトのタイマーを切る
	accept_timer_.cancel();
	send_timer_.cancel();
	receive_timer_.cancel();
	//接続を切る
	if (socket_ != NULL)
		socket_->close();
	//アクセプターを切る
	if (acceptor_ != NULL)
		acceptor_->close();
	//開放
	delete acceptor_;
	delete socket_;
}

ComClientUdp::~ComClientUdp() {
	// タイムアウトのタイマーを切る
	accept_timer_.cancel();
	send_timer_.cancel();
	receive_timer_.cancel();
	//接続を切る
	if (socket_ != NULL)
		send_socket_->close();
	if (acceptor_ != NULL)
		receive_socket_->close();
	//開放
	delete send_socket_;
	delete receive_socket_;
}

//接続
void ComClientTcp::StartAccept() {
	//ソケット、アクセプタ作成
	socket_ = new tcp::socket(io_service_);
	acceptor_ = new tcp::acceptor(io_service_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), kPort));
	//接続待機登録
	acceptor_->async_accept(*socket_, bind(&ComClientTcp::OnAccept, this, asio::placeholders::error));
	//180秒でタイムアウト
	accept_timer_.expires_from_now(boost::posix_time::seconds(180));
	accept_timer_.async_wait(boost::bind(&ComClientTcp::OnAcceptTimeOut, this, _1));
}

void ComClientUdp::StartAccept() {
	//endpoint設定
	udp::endpoint endpoint(boost::asio::ip::udp::v4(), kPort /*ポート番号*/);
	//ソケット作成
	receive_socket_ = new udp::socket(io_service_, endpoint);
	//クライアントからの送信待ち
	IniReceive();
}

void ComClientUdp::IniReceive() {
	receive_socket_->async_receive_from(asio::buffer(&receive_data_, sizeof(ToServerContainer)),
			remote_endpoint_,
			boost::bind(&ComClientUdp::OnIniReceive, this, asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	//180秒でタイムアウト
	receive_timer_.expires_from_now(boost::posix_time::seconds(180));
	receive_timer_.async_wait(boost::bind(&ComClientUdp::OnReceiveTimeOut, this, _1));
}

void ComClientUdp::OnIniReceive(const boost::system::error_code& error, size_t bytes_transferred) {
	//エラー時
	if (error && error != asio::error::eof) {
		uErrorOut(__FILE__, __func__, __LINE__, "受信失敗:" + error.message());
		return;
	}
	//再度受信準備
	receive_timer_.cancel(); // タイムアウトのタイマーを切る
	receive_buff_.consume(receive_buff_.size());
	//登録完了
	has_accepted_ = true;
	//情報表示
	printf("%s\n", uToStr(remote_endpoint_.address()).c_str());
	//送信先登録
	send_endpoint_ = udp::endpoint(asio::ip::address::from_string(uToStr(remote_endpoint_.address())),
			kPort + 10);
	send_socket_ = new udp::socket(io_service_);
	send_socket_->open(udp::v4());
}

void ComClientTcp::OnAccept(const boost::system::error_code& error) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "接続受信失敗:" + error.message());
		return;
	}
	has_accepted_ = true;
	accept_timer_.cancel(); // タイムアウトのタイマーを切る
}
void ComClientTcp::OnAcceptTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(kPort) + "):接続にタイムアウト\n");
		return;
	}
}

//送受信スタート
void ComClientTcp::Start() {
	if (!has_accepted_) {
		uErrorOut(__FILE__, __func__, __LINE__, "まだ接続されていません");
		return;
	}
	Send();
	Receive();
}

//送信
void ComClientTcp::Send() {
	asio::async_write(*socket_, asio::buffer(&send_data_, sizeof(ToClientContainer)),
			bind(&ComClientTcp::OnSend, this, asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	//5秒でタイムアウト
	send_timer_.expires_from_now(boost::posix_time::seconds(5));
	send_timer_.async_wait(boost::bind(&ComClientTcp::OnSendTimeOut, this, _1));
}

void ComClientUdp::Send() {
	send_socket_->async_send_to(asio::buffer(&send_data_, sizeof(ToClientContainer)), send_endpoint_,
			boost::bind(&ComClientUdp::OnSend, this, asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	//5秒でタイムアウト
	send_timer_.expires_from_now(boost::posix_time::seconds(5));
	send_timer_.async_wait(boost::bind(&ComClientUdp::OnSendTimeOut, this, _1));
}

void ComClientTcp::OnSend(const boost::system::error_code& error, size_t bytes_transferred) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "送信失敗:" + error.message());
		return;
	}
	send_timer_.cancel(); // タイムアウトのタイマーを切る
	Send();
}

void ComClientTcp::OnSendTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(kPort) + "):送信タイムアウト\n");
		return;
	}
}

//受信
void ComClientTcp::Receive() {
	asio::async_read(*socket_, receive_buff_, asio::transfer_exactly(sizeof(ToServerContainer)),
			bind(&ComClientTcp::OnReceive, this, asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	//5秒でタイムアウト
	receive_timer_.expires_from_now(boost::posix_time::seconds(5));
	receive_timer_.async_wait(boost::bind(&ComClientTcp::OnReceiveTimeOut, this, _1));
}

void ComClientUdp::Receive() {
	receive_socket_->async_receive(asio::buffer(&receive_data_, sizeof(ToServerContainer)),
			boost::bind(&ComClientUdp::OnReceive, this, asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	//5秒でタイムアウト
	receive_timer_.expires_from_now(boost::posix_time::seconds(5));
	receive_timer_.async_wait(boost::bind(&ComClientUdp::OnReceiveTimeOut, this, _1));
}

void ComClientTcp::OnReceive(const boost::system::error_code& error, size_t bytes_transferred) {
	//エラー時
	if (error && error != asio::error::eof) {
		uErrorOut(__FILE__, __func__, __LINE__, "受信失敗:" + error.message());
		return;
	}
	//再度受信準備
	receive_timer_.cancel(); // タイムアウトのタイマーを切る
	const ToServerContainer* data = asio::buffer_cast<const ToServerContainer*>(receive_buff_.data());
	receive_buff_.consume(receive_buff_.size());
	//データ処理
	receive_data_ = *data;
	printf("server_receive(%d):%d\n", kPort, receive_data_.player_data.level);
	server_->changed_player_data_ = true;
	//再度受信
	Receive();
}

void ComClientUdp::OnReceive(const boost::system::error_code& error, size_t bytes_transferred) {
	//エラー時
	if (error && error != boost::asio::error::message_size) {
		uErrorOut(__FILE__, __func__, __LINE__, "受信失敗:" + error.message());
		return;
	}
	//再度受信準備
	receive_timer_.cancel(); // タイムアウトのタイマーを切る
	//printf("server_receive(%d):%d\n", kPort, receive_data_.player_data.color);
	//再度受信
	Receive();
}

void ComClientTcp::OnReceiveTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(kPort) + "):受信タイムアウト\n");
		return;
	}
}

//getter,setter
void ComClientTcp::set_send_data(const ToClientContainer& send_data) {
	send_data_ = send_data;
}

ToServerContainer ComClientTcp::get_receive_data() const {
	return receive_data_;
}

bool ComClientTcp::get_has_accepted() const {
	return has_accepted_;
}

