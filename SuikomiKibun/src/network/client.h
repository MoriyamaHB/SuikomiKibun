/*
 * network.h
 *
 *  Created on: 2016/09/05
 *      Author: c501506069
 */

#ifndef SUIKOMIKIBUN_NETWORK_H_
#define SUIKOMIKIBUN_NETWORK_H_

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "../util/vector3.h"
#include "../gv.h"

namespace asio = boost::asio;
using asio::ip::tcp;

class Client {
private:
	asio::io_service io_service_;
	asio::streambuf receive_buff_;
	tcp::socket socket_;
	const std::string kIpAdress; //サーバーのIPアドレス
	const int kPort; //ポート番号
	ClientData send_data_;
	ServerData receive_data_;
	boost::thread conect_thread_;
	boost::thread run_thread_;
	enum State {
		kConectWait, kRun, kCom
	};
	bool has_conected_;
	State state_;

public:
	Client(std::string ip_adress, int port);
	~Client();
	void Update();
	void Draw();

private:
	//接続
	void Connect();
	void OnConnect(const boost::system::error_code& error);
	void Send();
	void OnSend(const boost::system::error_code& error, size_t bytes_transferred);
	void StartReceive();
	void OnReceive(const boost::system::error_code& error, size_t bytes_transferred);
	void ThRun();
};

#endif
