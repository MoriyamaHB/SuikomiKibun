#include "net_main.h"

NetMain::NetMain() {
	client_num_ = 0;
	//ネットワーク初期化
	char is_server;
	std::string server_ip;
	int port;
	std::cout << "s/c?";
	std::cin >> is_server;
	if (is_server == 's') {
//		std::cout << "client_num:";
//		std::cin >> client_num_;
		server_udp_ = new ServerUdp(31600, 3);
		server_tcp_ = new ServerTcp(31600 + 100, 3);
		is_server_ = true;
		server_ip = "127.0.0.1";
		port = 31600;
	} else {
		server_udp_ = NULL;
		server_tcp_ = NULL;
		is_server_ = false;
		std::cout << "server_ip:";
		std::cin >> server_ip;
		std::cout << "port:";
		std::cin >> port;
	}
	client_udp_ = new ClientUdp(server_ip, port);
	client_tcp_ = new ClientTcp(server_ip, port + 100);
	client_udp_->Connect();
	client_tcp_->Connect();
}

NetMain::~NetMain() {
	//ネットワーク
	if (is_server_) {
		delete server_udp_;
		delete server_tcp_;
	}
	delete client_udp_;
	delete client_tcp_;
}

void NetMain::Update() {
	//サーバー更新
	if (is_server_) {
		server_udp_->Update();
		server_tcp_->Update();
		server_data_ = client_udp_->get_receive_data();
	}
	//クライアント更新
	client_udp_->Update();
	client_tcp_->Update();
	client_udp_->set_send_data(client_data_);
}

void NetMain::Draw() const {
	//ネットワーク
	if (is_server_) {
		server_udp_->Draw();
		server_tcp_->Draw();
	}
	client_udp_->Draw();
	client_tcp_->Draw();
}

//getter
int NetMain::get_enemy_num() const {
	return client_num_ - 1;
}

btVector3 NetMain::GetEnemyPos(int num) const {
	btVector3 pos;
	Vector3 pos_v = client_udp_->get_receive_data().player_data[num].pos;
	pos[0] = pos_v.x;
	pos[1] = pos_v.y;
	pos[2] = pos_v.z;
	return pos;
}

btScalar NetMain::GetEnemyRadius(int num) const {
	return client_udp_->get_receive_data().player_data[num].radius;
}

//setter
void NetMain::SetMePos(btVector3 pos) {
	client_data_.player_data.pos = pos;
}

void NetMain::SetMeRadius(btScalar radius) {
	client_data_.player_data.radius = radius;
}
