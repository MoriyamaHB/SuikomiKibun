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

namespace asio = boost::asio;
using asio::ip::tcp;

class Server {
private:
	ComClient client0_;
	ComClient client1_;
	ComClient client2_;
	asio::io_service io_service_;

	void ThRun(ComClient *com);
public:
	Server();
	void StartAccept();			//全体つながるまで返ってこない
	void Run();
	void Update();
};

#endif /* SUIKOMIKIBUN_NET_NET_H_ */
