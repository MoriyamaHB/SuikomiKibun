#ifndef SUIKOMIKIBUN_NET_COMCLIENT_H_
#define SUIKOMIKIBUN_NET_COMCLIENT_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string.h>

#include "../util/uGL.h"

namespace asio = boost::asio;
using asio::ip::tcp;

class ComClient {
private:
	asio::io_service io_service_;
	tcp::acceptor acceptor_;
	tcp::socket socket_;
	asio::streambuf receive_buff_;	//受信バッファ
	ClientData receive_data_; 	//受信データ
	ServerData send_data_;		//送信データ
	const int kPort;			//ポート番号

	//状態変数
	bool has_accepted_;

	void OnAccept(const boost::system::error_code& error);
	void Send();
	void OnSend(const boost::system::error_code& error, size_t bytes_transferred);
	void Receive();
	void OnReceive(const boost::system::error_code& error, size_t bytes_transferred);
public:
	ComClient(asio::io_service &io_service, int port);
	void StartAccept();
	void Start();			//返ってこないので別スレッドで呼び出し
	void set_send_data(ServerData send_data);
	ClientData get_receive_data();
	bool get_has_accepted();
};

#endif
