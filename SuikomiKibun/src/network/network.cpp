/*
 * network.cpp
 *
 *  Created on: 2016/09/05
 *      Author: c501506069
 */

#include "network.h"

namespace asio = boost::asio;
using asio::ip::tcp;

Client::Client(asio::io_service& io_service, std::string ip_adress, ClientData *send_data) :
		io_service_(io_service), socket_(io_service), kIpAdress(ip_adress), send_data_(send_data) {
}

void Client::Start() {
	Connect();
}

void Client::Connect() {
	socket_.async_connect(tcp::endpoint(asio::ip::address::from_string(kIpAdress), 31400),
			boost::bind(&Client::OnConnect, this, asio::placeholders::error));
}

void Client::OnConnect(const boost::system::error_code& error) {
	if (error) {
		std::cout << "Connect failed :" << error.message() << std::endl;
		return;
	}
	Send();
	StartRecive();
}

//クライアント情報送信
void Client::Send() {
	asio::async_write(socket_, asio::buffer(send_data_, sizeof(ClientData)),
			boost::bind(&Client::OnSend, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

//送信完了
//error: エラー情報
// bytes_tranferred: 送信したバイト数
void Client::OnSend(const boost::system::error_code &error, size_t bytes_transferred) {
	if (error) {
		std::cout << "send failed:" << error.message() << std::endl;
	} else {
		std::cout << "send correct!" << std::endl;
	}
}

//サーバー情報受信
void Client::StartRecive() {
	boost::asio::async_read(socket_, receive_buff_,asio::transfer_exactly(sizeof(ServerData)) ,
			boost::bind(&Client::OnRecive, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

// 受信完了
// error : エラー情報
// bytes_transferred : 受信したバイト数

void Client::OnRecive(const boost::system::error_code& error, size_t bytes_transferred) {
	if (error && error != boost::asio::error::eof) {
		std::cout << "receive failed:" << error.message() << std::endl;

	}
	else{
		const ServerData* recive_data = asio::buffer_cast<const ServerData*>(receive_buff_.data());
		std::cout << recive_data->pos.x << std::endl;
		receive_buff_.consume(receive_buff_.size());
	}
}
