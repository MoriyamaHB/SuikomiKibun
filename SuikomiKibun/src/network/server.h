#ifndef SUIKOMIKIBUN_NET_SERVER_H_
#define SUIKOMIKIBUN_NET_SERVER_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <time.h>

#include "../util/uGL.h"
#include "../gv.h"
#include "com_client.h"

namespace asio = boost::asio;
using asio::ip::tcp;

class Server {
private:
	enum State {
		kAcceptWait, kRun, kCom
	};

	//通信
	asio::io_service io_service0_;
	asio::io_service io_service1_;
	asio::io_service io_service2_;
	ComClient *client0_;
	ComClient *client1_;
	ComClient *client2_;
	//状態
	State state_;
	int com_accept_num_; //接続数
	//スレッド
	boost::thread thread0_;
	boost::thread thread1_;
	boost::thread thread2_;

	//io_serviceを実行する(別スレッドで呼び出し用)
	void ThRun(asio::io_service *io);
public:
	Server();
	~Server();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_NET_NET_H_ */
