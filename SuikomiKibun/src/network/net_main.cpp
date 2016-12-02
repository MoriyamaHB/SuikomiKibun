#include "net_main.h"

NetMain::NetMain() {
	//ネットワーク初期化
	char is_server;
	std::string server_ip;
	int port;
	std::cout << "s/c?";
	std::cin >> is_server;
	if (is_server == 's') {
//		std::cout << "client_num:";
//		std::cin >> client_num_;
		server_ = new Server(31600, 3);
		is_server_ = true;
		server_ip = "127.0.0.1";
		port = 31600;
	} else {
		server_ = NULL;
		is_server_ = false;
		std::cout << "server_ip:";
		std::cin >> server_ip;
		std::cout << "port:";
		std::cin >> port;
	}
	client_ = new ClientUdp(server_ip, port);
	client_->Connect();
}

NetMain::~NetMain() {
	//ネットワーク
	if (is_server_)
		delete server_;
	delete client_;
}

void NetMain::Update() {
	//サーバー更新
	if (is_server_) {
		server_->Update();
		server_data_ = client_->get_receive_data();
	}
	//クライアント更新
	client_->Update();
	client_->set_send_data(client_data_);
}

void NetMain::Draw() const {
	//ネットワーク
	if (is_server_)
		server_->Draw();
	client_->Draw();
}

//getter
int NetMain::get_enemy_num() const {
	return client_num_ - 1;
}

btVector3 NetMain::GetEnemyPos(int num) const {
	btVector3 pos;
	Vector3 pos_v = client_->get_receive_data().player_data[num].pos;
	pos[0] = pos_v.x;
	pos[1] = pos_v.y;
	pos[2] = pos_v.z;
	return pos;
}

//setter
void NetMain::SetMePos(btVector3 pos) {
	client_data_.player_data.pos = pos;
}
