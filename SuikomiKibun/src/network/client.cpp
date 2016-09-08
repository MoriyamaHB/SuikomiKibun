#include "../network/client.h"

Client::Client(std::string ip_adress, int port) :
		socket_(io_service_), kIpAdress(ip_adress), kPort(port) {
	//メンバー変数初期化
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
	socket_.async_connect(tcp::endpoint(asio::ip::address::from_string(kIpAdress), kPort),
			boost::bind(&Client::OnConnect, this, asio::placeholders::error));
}

//接続完了
void Client::OnConnect(const boost::system::error_code& error) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "接続受信失敗:" + error.message());
		return;
	}
	has_conected_ = true;
}

//クライアント情報送信
void Client::Send() {
	asio::async_write(socket_, asio::buffer(&send_data_, sizeof(ClientData)),
			boost::bind(&Client::OnSend, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

//送信完了
//error: エラー情報
// bytes_tranferred: 送信したバイト数
void Client::OnSend(const boost::system::error_code &error, size_t bytes_transferred) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "送信失敗:" + error.message());
		return;
	}

	Send();
}

//サーバー情報受信
void Client::StartReceive() {
	boost::asio::async_read(socket_, receive_buff_, asio::transfer_exactly(sizeof(ServerData)),
			boost::bind(&Client::OnReceive, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

// 受信完了
// error : エラー情報
// bytes_transferred : 受信したバイト数
void Client::OnReceive(const boost::system::error_code& error, size_t bytes_transferred) {
	if (error && error != boost::asio::error::eof) {
		uErrorOut(__FILE__, __func__, __LINE__, "受信失敗:" + error.message());
		return;
	}
	const ServerData* recive_data = asio::buffer_cast<const ServerData*>(receive_buff_.data());
	receive_data_ = *recive_data;
	printf("client_receive(%d):%f\n", kPort, receive_data_.pos.x);
	receive_buff_.consume(receive_buff_.size());
	StartReceive();
}

void Client::set_send_data(const ClientData& send_data) {
	send_data_ = send_data;
}

ServerData Client::get_receive_data() const {
	return receive_data_;
}
