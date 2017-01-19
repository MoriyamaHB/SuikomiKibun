#include "net_main.h"

NetMain::NetMain() {
	client_num_ = 0;
	is_client_ = true;
	//ネットワーク初期化
	char is_server;
	std::string server_ip;
	int port;
	std::cout << "サーバーなら's',クライアントなら'c'を入力してください:";
	std::cin >> is_server;
	if (is_server == 's') {
		server_ = new Server(31600, 3);
		is_server_ = true;
		server_ip = "localhost";
		port = 31600;
	} else {
		server_ = NULL;
		is_server_ = false;
		std::cout << "サーバーのPC名:";
		std::cin >> server_ip;
		std::cout << "ポート番号(1人目:31601,2人目:31602):";
		std::cin >> port;
	}
	std::cout << "あなたの名前:";
	std::cin >> client_name_;
	//クライアント作成
	client_udp_ = new ClientUdp(server_ip, port);
	//接続
	client_udp_->Connect();
	//名前登録
	strncpy(client_data_.player_data.name, client_name_.c_str(), PlayerData::kNameLength);
	client_data_.player_data.name[PlayerData::kNameLength - 1] = '\0'; //null文字追加
}

NetMain::NetMain(InputIniInfoData i_data) {
	client_num_ = 0;
	is_client_ = true;

	//ネットワーク初期化
	if (i_data.s_or_c == 's') {
		server_ = new Server(31600, 3);
		is_server_ = true;
	} else {
		server_ = NULL;
		is_server_ = false;
	}
	//クライアント作成
	client_udp_ = new ClientUdp(i_data.server_ip, i_data.port);
	//接続
	client_udp_->Connect();
	//名前登録
	client_name_ = i_data.client_name;
	strncpy(client_data_.player_data.name, client_name_.c_str(), PlayerData::kNameLength);
	client_data_.player_data.name[PlayerData::kNameLength - 1] = '\0'; //null文字追加
}

NetMain::NetMain(bool enable_server_only) {
	if (!enable_server_only) {
		uErrorOut(__FILE__, __func__, __LINE__, "クライアントも起動する場合は引数なしでこのクラスを作成してください");
		is_server_ = false;
		is_client_ = false;
		uExit();
		return;
	}
	server_ = new Server(31600, 3);
	is_server_ = true;
	is_client_ = false;
	client_num_ = 0;
	client_udp_ = NULL;
}

NetMain::~NetMain() {
	//ネットワーク
	if (is_server_)
		delete server_;
	if (is_client_)
		delete client_udp_;
}

void NetMain::Update() {
	//サーバー更新
	if (is_server_) {
		server_->Update();
	}
	//クライアント更新
	if (is_client_) {
		server_data_ = client_udp_->get_receive_data();
		client_udp_->Update();
		client_udp_->set_send_data(client_data_);
	}
}

void NetMain::Draw() const {
	//ネットワーク
	if (is_server_)
		server_->Draw();
	if (is_client_) {
		client_udp_->Draw();
	}
}

//getter
int NetMain::get_enemy_num() const {
	if (!is_client_) {
		uErrorOut(__FILE__, __func__, __LINE__, "サーバーのみのためこの関数は利用できません");
		uExit();
	}
	return client_num_ - 1;
}

btVector3 NetMain::GetEnemyPos(int num) const {
	if (!is_client_) {
		uErrorOut(__FILE__, __func__, __LINE__, "サーバーのみのためこの関数は利用できません");
		uExit();
	}
	btVector3 pos;
	Vector3 pos_v = client_udp_->get_receive_data().player_data[num].pos;
	pos[0] = pos_v.x;
	pos[1] = pos_v.y;
	pos[2] = pos_v.z;
	return pos;
}

int NetMain::GetEnemyLevel(int num) const {
	if (!is_client_) {
		uErrorOut(__FILE__, __func__, __LINE__, "サーバーのみのためこの関数は利用できません");
		uExit();
	}
	return client_udp_->get_receive_data().player_data[num].level;
}

int NetMain::GetColor(int num) const {
	if (!is_client_) {
		uErrorOut(__FILE__, __func__, __LINE__, "サーバーのみのためこの関数は利用できません");
		uExit();
	}
	return client_udp_->get_receive_data().player_data[num].color;
}

GameState NetMain::GetGameState() const {
	if (!is_client_) {
		uErrorOut(__FILE__, __func__, __LINE__, "サーバーのみのためこの関数は利用できません");
		uExit();
	}
	return client_udp_->get_receive_data().game_data.state;
}

time_t NetMain::GetLimitedTime() const {
	if (!is_client_) {
		uErrorOut(__FILE__, __func__, __LINE__, "サーバーのみのためこの関数は利用できません");
		uExit();
	}
	return client_udp_->get_receive_data().game_data.limited_time;
}

std::string NetMain::GetEnemyName(int num) const {
	if (!is_client_) {
		uErrorOut(__FILE__, __func__, __LINE__, "サーバーのみのためこの関数は利用できません");
		uExit();
	}
	std::string s(client_udp_->get_receive_data().player_data[num].name);
	return s;
}

std::string NetMain::GetMyName() const {
	if (!is_client_) {
		uErrorOut(__FILE__, __func__, __LINE__, "サーバーのみのためこの関数は利用できません");
		uExit();
	}
	return client_name_;
}

//setter
void NetMain::SetMyPos(btVector3 pos) {
	if (!is_client_) {
		uErrorOut(__FILE__, __func__, __LINE__, "サーバーのみのためこの関数は利用できません");
		uExit();
	}
	client_data_.player_data.pos = pos;
}

void NetMain::SetMyLevel(int level) {
	if (!is_client_) {
		uErrorOut(__FILE__, __func__, __LINE__, "サーバーのみのためこの関数は利用できません");
		uExit();
	}
	client_data_.player_data.level = level;
}

void NetMain::SetMyColor(int color) {
	if (!is_client_) {
		uErrorOut(__FILE__, __func__, __LINE__, "サーバーのみのためこの関数は利用できません");
		uExit();
	}
	client_data_.player_data.color = color;
}
