#include "server.h"

Server::Server() :
		client0_(io_service0_, 31400), client1_(io_service1_, 31401), client2_(io_service2_, 31402) {
	state_ = kAcceptWait;
	//接続待機開始
	client0_.StartAccept();
	client1_.StartAccept();
	client2_.StartAccept();
	boost::thread thd0(&Server::ThRun, this, &io_service0_);
	thread0_.swap(thd0);
	boost::thread thd1(&Server::ThRun, this, &io_service1_);
	thread1_.swap(thd1);
	boost::thread thd2(&Server::ThRun, this, &io_service2_);
	thread2_.swap(thd2);
}

Server::~Server() {
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
}

void Server::Update() {
	switch (state_) {
	case kAcceptWait: 	//接続待機中
		//すべて接続されたら次に進む
		if (client0_.get_has_accepted() && client1_.get_has_accepted() && client2_.get_has_accepted())
			state_ = kRun;
		break;
	case kRun:		//送受信開始
		client0_.Start();
		client1_.Start();
		client2_.Start();
		{	//クラス生成するため{}が必要
			boost::thread thd0(&Server::ThRun, this, &io_service0_);
			thread0_.swap(thd0);
			boost::thread thd1(&Server::ThRun, this, &io_service1_);
			thread1_.swap(thd1);
			boost::thread thd2(&Server::ThRun, this, &io_service2_);
			thread2_.swap(thd2);
		}
		state_ = kCom;
		break;
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
	case kAcceptWait: //接続待機中
		output_display0.Regist("sever:接続待機中です", uColor4fv_green);
		break;
	case kRun:
		output_display0.Regist("sever:接続を確認.送受信を開始しました.", uColor4fv_green, 180);
		break;
	case kCom:
		output_display0.Regist("sever:送受信中です", uColor4fv_green);
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なcaseです");
		break;
	}
}
