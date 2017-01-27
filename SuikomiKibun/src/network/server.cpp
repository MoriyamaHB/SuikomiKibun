#include "server.h"

Server::Server(int start_port, int client_num) :
		kStartPort(start_port), kClientNum(client_num) {
	//状態初期化
	com_accept_num_ = 0;
	state_ = kAcceptWait;
	changed_player_data_ = true;
	memset(&win_lose, 0, sizeof(WinLoseData) * 3);
	for (int i = 0; i < kClientNum; i++) {
		//io_service作成
		io_service_.push_back(new asio::io_service());
		//クライアント作成
		client_.push_back(new ComClientUdp(*io_service_[i], start_port + i, this));
		client_[i]->StartAccept();
		//スレッド作成,実行
		thread_.push_back(new boost::thread());
		boost::thread thd(&Server::ThRun, this, io_service_[i]);
		thread_[i]->swap(thd);
	}
}

Server::~Server() {
	for (int i = kClientNum - 1; i >= 0; i--) {
		//io_service終了処理
		io_service_[i]->stop();
		//スレッド終了まで待機
		if (thread_[i]->joinable())
			thread_[i]->join();
		//vectorの中身削除
		delete thread_[i];
		thread_.pop_back();
		delete client_[i];
		client_.pop_back();
		delete io_service_[i];
		io_service_.pop_back();
	}
}

//io_serviceを実行する(別スレッドで呼び出し用)
void Server::ThRun(asio::io_service *io) {
	io->run();
	io->reset();
}

void Server::Update() {
	switch (state_) {
	case kAcceptWait: 	//接続待機中
		//残り接続数をカウント
		com_accept_num_ = 0;
		for (int i = 0; i < kClientNum; i++)
			if (client_[i]->get_has_accepted())
				com_accept_num_++;
		//すべて接続されたら次に進む
		if (com_accept_num_ == kClientNum)
			state_ = kRun;
		break;
	case kRun: {	//送受信開始
		for (int i = 0; i < kClientNum; i++) {
			//送受信開始を登録
			client_[i]->Start();
			//別スレッドで送受信を実行
			boost::thread thd(&Server::ThRun, this, io_service_[i]);
			thread_[i]->swap(thd);
		}
		//送受信中状態へ移行
		state_ = kCom;
		time.Init(kLimitedTime);	//制限時間を設定
		break;
	}
	case kCom: {	//送受信中
		//制限時間更新
		time.Update();
		//プレイヤーどうしの当たり判定
		PlayerCollisionDetection(0, 1);
		PlayerCollisionDetection(1, 2);
		PlayerCollisionDetection(2, 0);
		//全クライアントに送信データ登録
		for (int i = 0; i < kClientNum; i++) {
			ToClientContainer server_data;
			//プレイヤーデータ
			for (int j = 0, cnt = 0; j < kClientNum; j++) {
				if (i != j) {
					server_data.player_data[cnt] = client_[j]->get_receive_data().player_data;
					cnt++;
				}
			}
			//勝敗データ
			server_data.game_data.win_lose = win_lose[i];
			//ゲームの状態
			server_data.game_data.state = kPlay;
			//残り時間
			server_data.game_data.limited_time = time.get_time_remaining();

			//送信データ登録
			client_[i]->set_send_data(server_data);
		}
		break;
	}
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なcaseです");
		break;
	}
}

void Server::Draw() const {
	switch (state_) {
	case kAcceptWait: { //接続待機中
		output_display0.Regist("sever:接続を待機中(現在" + uToStr(com_accept_num_) + "/" + uToStr(kClientNum) + "台接続されました)",
				uColor4fv_green);
		break;
	}
	case kRun: //送受信開始
		output_display0.Regist("sever:接続を確認.送受信を開始しました.", uColor4fv_green, 60);
		break;
	case kCom: //送受信中
		output_display0.Regist("sever:送受信中です", uColor4fv_green);
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なcaseです");
		break;
	}
}

void Server::SetSendData(const ToClientContainer &send_data, int n) {
	if (n < 0 || n >= kClientNum)
		uErrorOut(__FILE__, __func__, __LINE__, "範囲外");
	client_[n]->set_send_data(send_data);
}

ToServerContainer Server::GetReceiveData(int n) const {
	if (n < 0 || n >= kClientNum)
		uErrorOut(__FILE__, __func__, __LINE__, "範囲外");
	return client_[n]->get_receive_data();
}

//プレイヤーどうしの当たり判定
void Server::PlayerCollisionDetection(int client_num1, int client_num2) {
	if (uIsCollisionBallAndBall(client_[client_num1]->get_receive_data().player_data.pos,
			client_[client_num1]->get_receive_data().player_data.level / 3.0,
			client_[client_num2]->get_receive_data().player_data.pos,
			client_[client_num2]->get_receive_data().player_data.level / 3.0, NULL)) {
		if (client_[client_num1]->get_receive_data().player_data.level
				== client_[client_num2]->get_receive_data().player_data.level
				&& client_[client_num1]->get_receive_data().player_data.color
						== client_[client_num2]->get_receive_data().player_data.color) {
			//引き分け
			win_lose[client_num1].draw_cnt++;
			win_lose[client_num2].draw_cnt++;
		} else if (IsWin(client_[client_num1]->get_receive_data().player_data.level,
				client_[client_num1]->get_receive_data().player_data.color,
				client_[client_num2]->get_receive_data().player_data.level,
				client_[client_num2]->get_receive_data().player_data.color)) {
			//勝った時
			win_lose[client_num1].win_cnt++;
			win_lose[client_num1].win_level = client_[client_num2]->get_receive_data().player_data.level;
			win_lose[client_num2].lose_cnt++;
		} else {
			//負けた時
			win_lose[client_num1].lose_cnt++;
			win_lose[client_num2].win_cnt++;
			win_lose[client_num2].win_level = client_[client_num1]->get_receive_data().player_data.level;
		}
	}
}

//勝敗判定
bool Server::IsWin(int my_level, int my_color, int enemy_level, int enemy_color) {
	if (my_color == enemy_color) {
		if (enemy_level > my_level) {
			return false;
		} else if (enemy_level < my_level) {
			return true;
		}
	} else if (my_color == 2 && enemy_color == 1) {
		return true;
	} else if (my_color == 3 && enemy_color == 2) {
		return true;
	} else if (my_color == 1 && enemy_color == 3) {
		return true;
	} else if (my_color == 1 && enemy_color == 2) {
		return false;
	} else if (my_color == 2 && enemy_color == 3) {
		return false;
	} else if (my_color == 3 && enemy_color == 1) {
		return false;
	}
	return false;
}
