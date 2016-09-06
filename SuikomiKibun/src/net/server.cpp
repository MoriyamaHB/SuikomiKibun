#include "server.h"

Server::Server() :
		client0_(io_service_, 31410), client1_(io_service_, 31411), client2_(io_service_, 31412) {
	state_ = kAcceptWait;
	//接続待機開始
	client0_.StartAccept();
	client1_.StartAccept();
	client2_.StartAccept();
	boost::thread thd(&Server::ThRun, this);
	accept_thread_.swap(thd);
}

Server::~Server() {
	//スレッド終了まで待機
	if (accept_thread_.joinable())
		accept_thread_.join();
	if (com_thread_.joinable())
		accept_thread_.join();
}

void Server::ThRun() {
	io_service_.run();
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
			boost::thread thd(&Server::ThRun, this);
			com_thread_.swap(thd);
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
