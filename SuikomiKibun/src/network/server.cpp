#include "../network/server.h"

Server::Server() {
	state_ = kAcceptWait;
	//クライアント作成
	client0_ = new ComClient(io_service0_, 31470);
	client1_ = new ComClient(io_service1_, 31471);
	client2_ = new ComClient(io_service2_, 31472);
	//接続待機開始
	client0_->StartAccept();
	client1_->StartAccept();
	client2_->StartAccept();
	boost::thread thd0(&Server::ThRun, this, &io_service0_);
	thread0_.swap(thd0);
	boost::thread thd1(&Server::ThRun, this, &io_service1_);
	thread1_.swap(thd1);
	boost::thread thd2(&Server::ThRun, this, &io_service2_);
	thread2_.swap(thd2);
	//その他
	com_accept_num_ = 0;
}

Server::~Server() {
	//io_service終了処理
	io_service0_.stop();
	io_service1_.stop();
	io_service2_.stop();
	//クライアント削除
	delete client0_;
	delete client1_;
	delete client2_;
	//スレッド終了まで待機
	if (thread0_.joinable())
		thread0_.join();
	if (thread1_.joinable())
		thread1_.join();
	if (thread2_.joinable())
		thread2_.join();
}

void Server::ThRun(asio::io_service *io) {
	io->run();
	io->reset();
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
		boost::thread thd0(&Server::ThRun, this, &io_service0_);
		thread0_.swap(thd0);
		boost::thread thd1(&Server::ThRun, this, &io_service1_);
		thread1_.swap(thd1);
		boost::thread thd2(&Server::ThRun, this, &io_service2_);
		thread2_.swap(thd2);
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
