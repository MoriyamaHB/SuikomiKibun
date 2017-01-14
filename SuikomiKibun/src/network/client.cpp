#include "client.h"

ClientTcp::ClientTcp(std::string ip_adress, int port, int client_num) :
		receive_data_(client_num - 1), connect_timer_(io_service_), send_timer_(io_service_), receive_timer_(
				io_service_), kIpAdress(ip_adress) {
	//メンバー変数初期化
	port_ = port;
	has_conected_ = false;
	state_ = kConnectWait;
	memset(&send_data_, 0, sizeof(send_data_));
	memset(&receive_data_, 0, sizeof(receive_data_));
	socket_ = NULL; //Connectで作成
	//状態
	is_tcp_ = true;
}

ClientUdp::ClientUdp(std::string ip_adress, int start_port, int client_num) :
		ClientTcp(ip_adress, start_port, client_num) {
	send_socket_ = NULL;
	receive_socket_ = NULL;
	//状態
	is_tcp_ = false;
}

ClientTcp::~ClientTcp() {
	// タイムアウトのタイマーを切る
	connect_timer_.cancel();
	send_timer_.cancel();
	receive_timer_.cancel();
	//接続を切る
	if (socket_ != NULL)
		socket_->close();
	//io_serviceを止める
	io_service_.stop();
	//スレッド終了まで待機
	if (conect_thread_.joinable())
		conect_thread_.join();
	if (run_thread_.joinable())
		run_thread_.join();
	//開放
	delete socket_;
}

ClientUdp::~ClientUdp() {
	// タイムアウトのタイマーを切る
	connect_timer_.cancel();
	send_timer_.cancel();
	receive_timer_.cancel();
	//接続を切る
	send_socket_->close();
	receive_socket_->close();
	//io_serviceを止める
	io_service_.stop();
	//スレッド終了まで待機
	if (conect_thread_.joinable())
		conect_thread_.join();
	if (run_thread_.joinable())
		run_thread_.join();
	//開放
	delete send_socket_;
	delete receive_socket_;
}

void ClientTcp::Update() {
	switch (state_) {
	case kConnectWait: //未接続
		//接続済みなら送受信開始
		if (has_conected_)
			state_ = kRun;
		break;
	case kRun: { //送受信開始
		//送受信登録
		Send();
		StartReceive();
		//別スレッドで実行
		boost::thread thd(&ClientTcp::ThRun, this);
		run_thread_.swap(thd);
		//送受信中状態に移行
		state_ = kCom;
		break;
	}
	case kCom: //送受信中
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なcaseです");
		break;
	}
}

void ClientTcp::Draw() {
	switch (state_) {
	case kConnectWait: { //未接続
		output_display0.Regist("client:サーバーと接続中です", uColor4fv_maroon);
		break;
	}
	case kRun: //送受信開始
		output_display0.Regist("client:接続を確認.送受信を開始.", uColor4fv_maroon, 60);
		break;
	case kCom: //送受信中
		output_display0.Regist("client:送受信中です", uColor4fv_maroon);
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なcaseです");
		break;
	}
}

void ClientUdp::Draw() {
	switch (state_) {
	case kConnectWait: { //未接続
		output_display0.Regist("client:サーバーと接続中です", uColor4fv_maroon);
		break;
	}
	case kRun: //送受信開始
		output_display0.Regist("client:接続を確認.送受信を開始.", uColor4fv_maroon, 60);
		break;
	case kCom: //送受信中
		if (receive_data_.game_data.state == kConnect)
			output_display0.Regist("client:他プレイヤーの接続を待機しています", uColor4fv_maroon);
		if (receive_data_.game_data.state == kPlay)
			output_display0.Regist("client:送受信中です", uColor4fv_maroon);
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なcaseです");
		break;
	}
}

//io_serviceを実行する(別スレッドで呼び出し用)
void ClientTcp::ThRun() {
	io_service_.run();
	io_service_.reset();
}

//接続
void ClientTcp::Connect() {
	//ソケット作成
	socket_ = new tcp::socket(io_service_);
	//接続登録
	socket_->async_connect(tcp::endpoint(asio::ip::address::from_string(kIpAdress), port_),
			boost::bind(&ClientTcp::OnConnect, this, asio::placeholders::error));
	//5秒でタイムアウト
	connect_timer_.expires_from_now(boost::posix_time::seconds(5));
	connect_timer_.async_wait(boost::bind(&ClientTcp::OnConnectTimeOut, this, _1));
	//接続を別スレッドで実行
	boost::thread thd(&ClientTcp::ThRun, this);
	conect_thread_.swap(thd);
}

void ClientUdp::Connect() {
	//endpoint設定
	udp::endpoint endpoint(boost::asio::ip::udp::v4(), port_ + 10 /*ポート番号*/);
	udp::resolver resolver(io_service_);
	udp::resolver::query query(udp::v4(), kIpAdress, uToStr(port_));
	send_endpoint_ = *resolver.resolve(query);
	//ソケット作成
	receive_socket_ = new udp::socket(io_service_, endpoint);
	send_socket_ = new udp::socket(io_service_);
	send_socket_->open(udp::v4());
	//登録完了
	printf("client(%d):登録完了\n", port_);
	has_conected_ = true;
}

//接続完了
void ClientTcp::OnConnect(const boost::system::error_code& error) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "接続受信失敗:" + error.message());
		return;
	}
	printf("client(%d):接続に成功\n", port_);
	connect_timer_.cancel(); // タイムアウトのタイマーを切る
	has_conected_ = true;
}

//接続タイムアウト
void ClientTcp::OnConnectTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(port_) + "):接続にタイムアウト\n");
		return;
	}
}

//クライアント情報送信
void ClientTcp::Send() {
	asio::async_write(*socket_, asio::buffer(&send_data_, sizeof(ToServerContainer)),
			boost::bind(&ClientTcp::OnSend, this, asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	//60秒でタイムアウト
	send_timer_.expires_from_now(boost::posix_time::seconds(60));
	send_timer_.async_wait(boost::bind(&ClientTcp::OnSendTimeOut, this, _1));
}

void ClientUdp::Send() {
	send_socket_->async_send_to(asio::buffer(&send_data_, sizeof(ToServerContainer)), send_endpoint_,
			boost::bind(&ClientUdp::OnSend, this, asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	//60秒でタイムアウト
	send_timer_.expires_from_now(boost::posix_time::seconds(60));
	send_timer_.async_wait(boost::bind(&ClientUdp::OnSendTimeOut, this, _1));
}

//送信完了
//error: エラー情報
// bytes_tranferred: 送信したバイト数
void ClientTcp::OnSend(const boost::system::error_code &error, size_t bytes_transferred) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "送信失敗:" + error.message());
		return;
	}
	send_timer_.cancel(); // タイムアウトのタイマーを切る
	Send();
}

//送信タイムアウト
void ClientTcp::OnSendTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(port_) + "):送信タイムアウト\n");
		return;
	}
}

//サーバー情報受信
void ClientTcp::StartReceive() {
	boost::asio::async_read(*socket_, receive_buff_, asio::transfer_exactly(sizeof(receive_data_)),
			boost::bind(&ClientTcp::OnReceive, this, asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	//60秒でタイムアウト
	receive_timer_.expires_from_now(boost::posix_time::seconds(60));
	receive_timer_.async_wait(boost::bind(&ClientTcp::OnReceiveTimeOut, this, _1));
}

void ClientUdp::StartReceive() {
	receive_socket_->async_receive_from(asio::buffer(&receive_data_, sizeof(receive_data_)),
			remote_endpoint_,
			boost::bind(&ClientUdp::OnReceive, this, asio::placeholders::error,
					asio::placeholders::bytes_transferred));
	//60秒でタイムアウト
	receive_timer_.expires_from_now(boost::posix_time::seconds(60));
	receive_timer_.async_wait(boost::bind(&ClientUdp::OnReceiveTimeOut, this, _1));
}

// 受信完了
// error : エラー情報
// bytes_transferred : 受信したバイト数
void ClientTcp::OnReceive(const boost::system::error_code& error, size_t bytes_transferred) {
	//エラー時
	if (error && error != boost::asio::error::eof) {
		uErrorOut(__FILE__, __func__, __LINE__, "受信失敗:" + error.message());
		return;
	}
	//再度受信準備
	receive_timer_.cancel(); // タイムアウトのタイマーを切る
	const ToClientContainer* recive_data = asio::buffer_cast<const ToClientContainer*>(receive_buff_.data());
	receive_buff_.consume(receive_buff_.size());
	//データ処理
	receive_data_ = *recive_data;
	printf("client_receive(%d):%d\n", port_, receive_data_.player_data[0].level);
	//再度受信
	StartReceive();
}

void ClientUdp::OnReceive(const boost::system::error_code& error, size_t bytes_transferred) {
	//エラー時
	if (error && error != asio::error::message_size) {
		uErrorOut(__FILE__, __func__, __LINE__, "受信失敗:" + error.message());
		return;
	}
	//再度受信準備
	receive_timer_.cancel(); // タイムアウトのタイマーを切る
	//printf("client_receive(%d):%f\n", port_, receive_data_.player_data[0].radius);
	//再度受信
	StartReceive();
}

//受信タイムアウト
void ClientTcp::OnReceiveTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(port_) + "):受信タイムアウト\n");
		return;
	}
}

void ClientTcp::set_send_data(const ToServerContainer& send_data) {
	send_data_ = send_data;
}

ToClientContainer ClientTcp::get_receive_data() const {
	return receive_data_;
}

