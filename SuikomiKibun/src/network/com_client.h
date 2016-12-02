#ifndef SUIKOMIKIBUN_NET_COMCLIENT_H_
#define SUIKOMIKIBUN_NET_COMCLIENT_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string.h>
#include <time.h>

#include "../util/uGL.h"
#include "../gv.h"

class ComClient;
class ComClientUdp;
#include "server.h"

namespace asio = boost::asio;
using asio::ip::tcp;
using asio::ip::udp;

class ComClient {
protected:
	//通信用
	asio::io_service &io_service_;
	tcp::acceptor *acceptor_;
	tcp::socket *socket_;
	asio::streambuf receive_buff_;	//受信バッファ
	ToServerContainer receive_data_; 	//受信データ
	ToClientContainer send_data_;		//送信データ
	Server *server_;		//サーバー
	//接続タイムアウト
	asio::deadline_timer accept_timer_;
	asio::deadline_timer send_timer_;
	asio::deadline_timer receive_timer_;
	//定数
	const int kPort;			//ポート番号
	//状態変数
	bool has_accepted_;	//接続済みか
	bool is_tcp_;	//tcpであるか

	//接続
	void OnAccept(const boost::system::error_code& error);
	void OnAcceptTimeOut(const boost::system::error_code& error);
	//送信
	virtual void Send();
	void OnSend(const boost::system::error_code& error, size_t bytes_transferred);
	void OnSendTimeOut(const boost::system::error_code& error);
	//受信
	virtual void Receive();
	virtual void OnReceive(const boost::system::error_code& error, size_t bytes_transferred);
	void OnReceiveTimeOut(const boost::system::error_code& error);
public:
	ComClient(asio::io_service &io_service, int port, Server* se);
	virtual void StartAccept();	//接続待機
	virtual ~ComClient();
	void Start();	//送受信スタート

	void set_send_data(const ToClientContainer &send_data);
	ToServerContainer get_receive_data() const;
	bool get_has_accepted() const;
};

class ComClientUdp: public ComClient {
private:
	udp::socket *send_socket_;
	udp::socket *receive_socket_;
	udp::endpoint remote_endpoint_;
	udp::endpoint send_endpoint_;

	//送信
	void Send();
	//受信
	void Receive();

	//初期時の処理
	void IniReceive();
	void OnIniReceive(const boost::system::error_code& error, size_t bytes_transferred);
public:
	ComClientUdp(asio::io_service &io_service, int port, Server* se);
	void StartAccept();	//接続
	~ComClientUdp();
};

#endif
