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
#include "../util/vector3.h"

namespace asio = boost::asio;
using asio::ip::tcp;

typedef struct {
	Vector3 pos;
} ClientData;

typedef struct {
	Vector3 pos;
} ServerData;

class Client {
	asio::io_service& io_service_;
	asio::streambuf receive_buff_;
	tcp::socket socket_;
	const std::string kIpAdress; //サーバーのIPアドレス
	ClientData *send_data_;

public:
	Client(asio::io_service& io_service, std::string ip_adress, ClientData *send_data);
	void Start();

private:
	//接続
	void Connect();
	void OnConnect(const boost::system::error_code& error);
	void Send();
	void OnSend(const boost::system::error_code& error, size_t bytes_transferred);
	void StartRecive();
	void OnRecive(const boost::system::error_code& error, size_t bytes_transferred);
};

#endif
