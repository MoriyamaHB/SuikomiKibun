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

	asio::io_service io_service_;
	ComClient client0_;
	ComClient client1_;
	ComClient client2_;
	State state_;
	boost::thread accept_thread_;
	boost::thread com_thread_;

public:
	void ThRun();
	Server();
	~Server();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_NET_NET_H_ */
