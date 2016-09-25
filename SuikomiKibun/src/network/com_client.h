#ifndef SUIKOMIKIBUN_NET_COMCLIENT_H_
#define SUIKOMIKIBUN_NET_COMCLIENT_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string.h>

#include "../util/uGL.h"
#include "../gv.h"

namespace asio = boost::asio;
using asio::ip::tcp;

class ComClient {
private:
	//通信用
	asio::io_service &io_service_;
	tcp::acceptor acceptor_;
	tcp::socket socket_;
	asio::streambuf receive_buff_;	//受信バッファ
	ClientData receive_data_; 	//受信データ
	ServerData send_data_;		//送信データ
	//接続タイムアウト
	asio::deadline_timer accept_timer_;
	asio::deadline_timer send_timer_;
	asio::deadline_timer receive_timer_;
	//定数
	const int kPort;			//ポート番号
	//状態変数
	bool has_accepted_;

	//接続待機
	void StartAccept();
	void OnAccept(const boost::system::error_code& error);
	void OnAcceptTimeOut(const boost::system::error_code& error);
	//送信
	void Send();
	void OnSend(const boost::system::error_code& error, size_t bytes_transferred);
	void OnSendTimeOut(const boost::system::error_code& error);
	//受信
	void Receive();
	void OnReceive(const boost::system::error_code& error, size_t bytes_transferred);
	void OnReceiveTimeOut(const boost::system::error_code& error);
public:
	ComClient(asio::io_service &io_service, int port);
	~ComClient();
	void Start();	//送受信スタート

	void set_send_data(const ServerData &send_data);
	ClientData get_receive_data() const;
	bool get_has_accepted() const;
};

#endif