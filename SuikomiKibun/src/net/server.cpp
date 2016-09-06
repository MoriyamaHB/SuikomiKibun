#include "server.h"

Server::Server() {
	state_ = kAcceptWait;
	//クライアント作成
	client0_ = new ComClient(io_service_, 31470);
	client1_ = new ComClient(io_service_, 31471);
	client2_ = new ComClient(io_service_, 31472);
	//接続待機開始
	client0_->StartAccept();
	client1_->StartAccept();
	client2_->StartAccept();
	boost::thread thd(&Server::ThRun, this);
	accept_thread_.swap(thd);
	//その他
	com_accept_num_ = 0;
}

Server::~Server() {
	//クライアント削除
	delete client0_;
	delete client1_;
	delete client2_;
	//io_service終了処理
	io_service_.stop();
	//スレッド終了まで待機
	if (accept_thread_.joinable())
		accept_thread_.join();
	if (com_thread_.joinable())
		com_thread_.join();
}

void Server::ThRun() {
	io_service_.run();
}

void Server::Update() {
	switch (state_) {
	case kAcceptWait: 	//接続待機中
		//残り接続数をカウント
		com_accept_num_ = 0;
		if (client0_->get_has_accepted()) {
			com_accept_num_++;
		}
		if (client1_->get_has_accepted()) {
			com_accept_num_++;
		}
		if (client2_->get_has_accepted()) {
			com_accept_num_++;
		}
		if (com_accept_num_ == 3)
			state_ = kRun;		//すべて接続されたら次に進む
		break;
	case kRun: {	//送受信開始
		client0_->Start();
		client1_->Start();
		client2_->Start();
		boost::thread thd(&Server::ThRun, this);
		com_thread_.swap(thd);
		state_ = kCom;
		break;
	}
	case kCom:
		//ここで送受信データのやり取りを行う
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なcaseです");
		break;
	}
}

void Server::Draw() const {
	switch (state_) {
	case kAcceptWait: { //接続待機中
		char string[256];
		sprintf(string, "sever:接続を確認.送受信を開始しました.(現在%d台接続されました)", com_accept_num_);
		output_display0.Regist(string, uColor4fv_green);
		break;
	}
	case kRun:
		output_display0.Regist("sever:接続開始", uColor4fv_green, 60);
		break;
	case kCom:
		output_display0.Regist("sever:送受信中です", uColor4fv_green);
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なcaseです");
		break;
	}
}
