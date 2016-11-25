/*
 * network.h
 *
 *  Created on: 2016/09/05
 *      Author: c501506069
 */

#ifndef SUIKOMIKIBUN_CLIENT_H_
#define SUIKOMIKIBUN_CLIENT_H_

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <time.h>

#include "../util/vector3.h"
#include "../util/uGL.h"
#include "../gv.h"

namespace asio = boost::asio;
using asio::ip::tcp;
using asio::ip::udp;

class Client {
protected:
	enum State {
		kConnectWait, kRun, kCom
	};

	//通信用
	asio::io_service io_service_;
	asio::streambuf receive_buff_;
	tcp::socket *socket_;
	int port_; //ポート番号
	ToServerContainer send_data_;		//送信データ
	ToClientContainer receive_data_;	//受信データ
	//接続タイムアウト
	asio::deadline_timer connect_timer_;
	asio::deadline_timer send_timer_;
	asio::deadline_timer receive_timer_;
	//スレッド
	boost::thread conect_thread_;
	boost::thread run_thread_;
	//定数
	const std::string kIpAdress; //サーバーのIPアドレス
	//状態
	bool has_conected_;
	State state_;

	//接続
	void OnConnect(const boost::system::error_code& error);
	void OnConnectTimeOut(const boost::system::error_code& error);
	//送信
	void Send();
	void OnSend(const boost::system::error_code& error, size_t bytes_transferred);
	void OnSendTimeOut(const boost::system::error_code& error);
	//受信
	void StartReceive();
	void OnReceive(const boost::system::error_code& error, size_t bytes_transferred);
	void OnReceiveTimeOut(const boost::system::error_code& error);
	//io_serviceを実行する(別スレッドで呼び出し用)
	void ThRun();
public:
	Client(std::string ip_adress, int start_port);
	void Connect();	//接続開始
	~Client();
	void Update();
	void Draw();

	void set_send_data(const ToServerContainer &send_data);
	ToClientContainer get_receive_data() const;
};

class ClientUdp: public Client {
private:
	//ソケット(override)
	udp::socket *socket_;

	//送信
	void Send();
	//受信
	void StartReceive();

public:
	ClientUdp(std::string ip_adress, int start_port);
	void Connect();	//接続開始
	virtual ~ClientUdp();
};

#endif
