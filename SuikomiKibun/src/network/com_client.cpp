#include "../network/com_client.h"

ComClient::ComClient(asio::io_service &io_service, int port, Server* se) :
		io_service_(io_service), acceptor_(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), socket_(
				io_service), accept_timer_(io_service_), send_timer_(io_service_), receive_timer_(io_service_), kPort(
				port), server_(se) {
	//メンバー変数初期化
	memset(&send_data_, 0, sizeof(send_data_));
	memset(&receive_data_, 0, sizeof(receive_data_));
	has_accepted_ = false;
	StartAccept(); //接続待機開始
}

ComClient::~ComClient() {
	// タイムアウトのタイマーを切る
	accept_timer_.cancel();
	send_timer_.cancel();
	receive_timer_.cancel();
	//接続を切る
	socket_.close();
	//アクセプターを切る
	acceptor_.close();
}

//接続
void ComClient::StartAccept() {
	acceptor_.async_accept(socket_, bind(&ComClient::OnAccept, this, asio::placeholders::error));
	//180秒でタイムアウト
	accept_timer_.expires_from_now(boost::posix_time::seconds(180));
	accept_timer_.async_wait(boost::bind(&ComClient::OnAcceptTimeOut, this, _1));
}

void ComClient::OnAccept(const boost::system::error_code& error) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "接続受信失敗:" + error.message());
		return;
	}
	has_accepted_ = true;
	accept_timer_.cancel(); // タイムアウトのタイマーを切る
}
void ComClient::OnAcceptTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(kPort) + "):接続にタイムアウト\n");
		return;
	}
}

//送受信スタート
void ComClient::Start() {
	if (!has_accepted_) {
		uErrorOut(__FILE__, __func__, __LINE__, "まだ接続されていません");
		return;
	}
	Send();
	Receive();
}

//送信
void ComClient::Send() {
	timespec time;
	//0.1秒を設定
	time.tv_sec = 0;
	time.tv_nsec = 100000000;
	//データが更新されるまで待機
	while (server_->changed_player_data_ == false)
		nanosleep(&time, NULL);
	asio::async_write(socket_, asio::buffer(&send_data_, sizeof(ToClientContainer)),
			bind(&ComClient::OnSend, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
	//5秒でタイムアウト
	send_timer_.expires_from_now(boost::posix_time::seconds(5));
	send_timer_.async_wait(boost::bind(&ComClient::OnSendTimeOut, this, _1));
}

void ComClient::OnSend(const boost::system::error_code& error, size_t bytes_transferred) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "送信失敗:" + error.message());
		return;
	}
	send_timer_.cancel(); // タイムアウトのタイマーを切る
	Send();
}

void ComClient::OnSendTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(kPort) + "):送信タイムアウト\n");
		return;
	}
}

//受信
void ComClient::Receive() {
	asio::async_read(socket_, receive_buff_, asio::transfer_exactly(sizeof(ToServerContainer)),
			bind(&ComClient::OnReceive, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
	//5秒でタイムアウト
	receive_timer_.expires_from_now(boost::posix_time::seconds(5));
	receive_timer_.async_wait(boost::bind(&ComClient::OnReceiveTimeOut, this, _1));
}

void ComClient::OnReceive(const boost::system::error_code& error, size_t bytes_transferred) {
	if (error && error != asio::error::eof) {
		uErrorOut(__FILE__, __func__, __LINE__, "受信失敗:" + error.message());
		return;
	}
	receive_timer_.cancel(); // タイムアウトのタイマーを切る
	const ToServerContainer* data = asio::buffer_cast<const ToServerContainer*>(receive_buff_.data());
	receive_data_ = *data;
	printf("server_receive(%d):%f\n", kPort, receive_data_.player_data.pos.x);
	receive_buff_.consume(receive_buff_.size());
	server_->changed_player_data_ = true;
	Receive();
}

void ComClient::OnReceiveTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(kPort) + "):受信タイムアウト\n");
		return;
	}
}

//getter,setter
void ComClient::set_send_data(const ToClientContainer& send_data) {
	send_data_ = send_data;
}

ToServerContainer ComClient::get_receive_data() const {
	return receive_data_;
}

bool ComClient::get_has_accepted() const {
	return has_accepted_;
}

