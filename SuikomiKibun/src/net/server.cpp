#include "server.h"

Server::Server() :
		client0_(&io_service_, 31400), client1_(&io_service_, 31401), client2_(&io_service_, 31402) {
}

void Server::StartAccept() {
	client0_.StartAccept();
	client1_.StartAccept();
	client2_.StartAccept();
	io_service_.run();
	while (1) { //すべて接続されるまでループ
		if (client0_.get_has_accepted() && client1_.get_has_accepted() && client2_.get_has_accepted())
			break;
		usleep(10);
	}
}

void Server::ThRun(ComClient *com) {
	com->Start();
}

void Server::Run() {
	boost::thread th0(boost::bind(&Server::ThRun, this, &client0_));
	boost::thread th0(boost::bind(&Server::ThRun, this, &client1_));
	boost::thread th0(boost::bind(&Server::ThRun, this, &client2_));
}

void Server::Update() {

}
