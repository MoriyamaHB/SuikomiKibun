/*
 * net.h
 *
 *  Created on: 2016/09/05
 *      Author: c501506068
 */

#ifndef SUIKOMIKIBUN_NET_NETMAIN_H_
#define SUIKOMIKIBUN_NET_NETMAIN_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>

#include "server.h"
#include "../util/uGL.h"
#include "client.h"
#include "../gv.h"

namespace asio = boost::asio;
using asio::ip::tcp;

class NetMain {
private:
	//状態変数
	int client_num_;
	//クライアント
	ClientUdp *client_udp_;
	ToServerContainer client_data_;
	std::string client_name_;
	bool is_client_;
	//サーバー
	Server *server_;
	bool is_server_;
	ToClientContainer server_data_;
public:
	NetMain();
	NetMain(bool enable_server_only);
	~NetMain();
	void Update();
	void Draw() const;

	//getter
	int get_enemy_num() const;
	btVector3 GetEnemyPos(int num) const;
	int GetEnemyLevel(int num) const;
	int GetColor(int num) const;
	GameState GetGameState() const;
	time_t GetLimitedTime() const;
	std::string GetEnemyName(int num) const;
	std::string GetMyName() const;

	//setter
	void SetMyPos(btVector3 pos);
	void SetMyLevel(int level);
	void SetMyColor(int color);
};

#endif /* SUIKOMIKIBUN_NET_NETMAIN_H_ */
