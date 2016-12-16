#ifndef SUIKOMIKIBUN_NET_SERVER_H_
#define SUIKOMIKIBUN_NET_SERVER_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

#include "../util/uGL.h"
#include "../gv.h"
#include "../time/time.h"

class Server;
#include "com_client.h"

namespace asio = boost::asio;
using asio::ip::tcp;

class Server {
private:
	//状態を表す列挙型
	enum State {
		kAcceptWait, kRun, kCom
	};

	//通信
	std::vector<asio::io_service*> io_service_;
	std::vector<ComClientUdp*> client_;
	//スレッド
	std::vector<boost::thread*> thread_;
	//状態
	State state_;
	int com_accept_num_; //接続数
	LimitedTime time; //制限時間
	//定数
	const int kStartPort;	//待機開始ポート
	const int kClientNum;	//クライアント接続数

	//io_serviceを実行する(別スレッドで呼び出し用)
	void ThRun(asio::io_service *io);
public:
	Server(int start_port, int client_num);
	~Server();
	void Update();
	void Draw() const;

	void SetSendData(const ToClientContainer &send_data, int n);
	ToServerContainer GetReceiveData(int n) const;

	//テスト
	bool changed_player_data_;
};

#endif /* SUIKOMIKIBUN_NET_NET_H_ */
