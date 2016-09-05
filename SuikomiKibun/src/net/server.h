#ifndef SUIKOMIKIBUN_NET_SERVER_H_
#define SUIKOMIKIBUN_NET_SERVER_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string.h>
#include <time.h>

#include "../util/uGL.h"
#include "com_client.h"
#include "../gv.h"

namespace asio = boost::asio;
using asio::ip::tcp;

class Server {
private:
	enum State {
		kAcceptWait, kRun, kCom
	};

	ComClient client0_;
	ComClient client1_;
	ComClient client2_;
	asio::io_service io_service0_;
	asio::io_service io_service1_;
	asio::io_service io_service2_;
	State state_;
	boost::thread thread0_;
	boost::thread thread1_;
	boost::thread thread2_;

public:
	void ThRun(asio::io_service *io);
	Server();
	~Server();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_NET_NET_H_ */
