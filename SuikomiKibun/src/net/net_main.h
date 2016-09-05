/*
 * net.h
 *
 *  Created on: 2016/09/05
 *      Author: c501506068
 */

#ifndef SUIKOMIKIBUN_NET_NETMAIN_H_
#define SUIKOMIKIBUN_NET_NETMAIN_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string.h>

#include "../util/uGL.h"
#include "server.h"

namespace asio = boost::asio;
using asio::ip::tcp;

typedef struct {
	Vector3 pos;
} ClientData;

typedef struct {

} ServerData;

#endif /* SUIKOMIKIBUN_NET_NET_H_ */
