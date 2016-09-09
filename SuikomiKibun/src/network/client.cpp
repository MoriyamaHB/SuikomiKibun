#include "../network/client.h"

Client::Client(std::string ip_adress, int start_port) :
		socket_(io_service_), connect_timer_(io_service_), send_timer_(io_service_), receive_timer_(io_service_), kIpAdress(
				ip_adress) {
	//メンバー変数初期化
	port_ = start_port;
	has_conected_ = false;
	state_ = kConnectWait;
	memset(&send_data_, 0, sizeof(send_data_));
	memset(&receive_data_, 0, sizeof(receive_data_));
	//接続を登録
	Connect();
	//接続を別スレッドで実行
	boost::thread thd(&Client::ThRun, this);
	conect_thread_.swap(thd);
}

Client::~Client() {
	// タイムアウトのタイマーを切る
	connect_timer_.cancel();
	send_timer_.cancel();
	receive_timer_.cancel();
	//接続を切る
	socket_.close();
	//io_serviceを止める
	io_service_.stop();
	//スレッド終了まで待機
	if (conect_thread_.joinable())
		conect_thread_.join();
	if (run_thread_.joinable())
		run_thread_.join();
}

void Client::Update() {
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
		boost::thread thd(&Client::ThRun, this);
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

void Client::Draw() {
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

//io_serviceを実行する(別スレッドで呼び出し用)
void Client::ThRun() {
	io_service_.run();
	io_service_.reset();
}

//接続
void Client::Connect() {
	socket_.async_connect(tcp::endpoint(asio::ip::address::from_string(kIpAdress), port_),
			boost::bind(&Client::OnConnect, this, asio::placeholders::error));
	//5秒でタイムアウト
	connect_timer_.expires_from_now(boost::posix_time::seconds(5));
	connect_timer_.async_wait(boost::bind(&Client::OnConnectTimeOut, this, _1));
}

//接続完了
void Client::OnConnect(const boost::system::error_code& error) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "接続受信失敗:" + error.message());
		return;
	}
	printf("client(%d):接続に成功\n", port_);
	connect_timer_.cancel(); // タイムアウトのタイマーを切る
	has_conected_ = true;
}

//接続タイムアウト
void Client::OnConnectTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(port_) + "):接続にタイムアウト\n");
		return;
	}
}

//クライアント情報送信
void Client::Send() {
	asio::async_write(socket_, asio::buffer(&send_data_, sizeof(ClientData)),
			boost::bind(&Client::OnSend, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
	//60秒でタイムアウト
	send_timer_.expires_from_now(boost::posix_time::seconds(60));
	send_timer_.async_wait(boost::bind(&Client::OnSendTimeOut, this, _1));
}

//送信完了
//error: エラー情報
// bytes_tranferred: 送信したバイト数
void Client::OnSend(const boost::system::error_code &error, size_t bytes_transferred) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "送信失敗:" + error.message());
		return;
	}
	send_timer_.cancel(); // タイムアウトのタイマーを切る
	Send();
}

//送信タイムアウト
void Client::OnSendTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(port_) + "):送信タイムアウト\n");
		return;
	}
}

//サーバー情報受信
void Client::StartReceive() {
	boost::asio::async_read(socket_, receive_buff_, asio::transfer_exactly(sizeof(ServerData)),
			boost::bind(&Client::OnReceive, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
	//60秒でタイムアウト
	receive_timer_.expires_from_now(boost::posix_time::seconds(60));
	receive_timer_.async_wait(boost::bind(&Client::OnReceiveTimeOut, this, _1));
}

// 受信完了
// error : エラー情報
// bytes_transferred : 受信したバイト数
void Client::OnReceive(const boost::system::error_code& error, size_t bytes_transferred) {
	if (error && error != boost::asio::error::eof) {
		uErrorOut(__FILE__, __func__, __LINE__, "受信失敗:" + error.message());
		return;
	}
	receive_timer_.cancel(); // タイムアウトのタイマーを切る
	const ServerData* recive_data = asio::buffer_cast<const ServerData*>(receive_buff_.data());
	receive_data_ = *recive_data;
	printf("client_receive(%d):%f\n", port_, receive_data_.pos[0].x);
	receive_buff_.consume(receive_buff_.size());
	StartReceive();
}

//受信タイムアウト
void Client::OnReceiveTimeOut(const boost::system::error_code& error) {
	if (!error) {
		uErrorOut(__FILE__, __func__, __LINE__, "server(" + uToStr(port_) + "):受信タイムアウト\n");
		return;
	}
}

void Client::set_send_data(const ClientData& send_data) {
	send_data_ = send_data;
}

ServerData Client::get_receive_data() const {
	return receive_data_;
}
