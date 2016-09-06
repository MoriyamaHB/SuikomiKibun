/*
 * network.cpp
 *
 *  Created on: 2016/09/05
 *      Author: c501506069
 */

#include "network.h"

namespace asio = boost::asio;
using asio::ip::tcp;

Client::Client(std::string ip_adress, int port) :
		socket_(io_service_), kIpAdress(ip_adress), kPort(port) {
	Connect();
	boost::thread thd(&Client::ThRun, this);
	conect_thread_.swap(thd);
	has_conected_ = false;
}

Client::~Client() {
	io_service_.stop();
	//スレッド終了まで待機
	if (conect_thread_.joinable())
		conect_thread_.join();
	if (run_thread_.joinable())
		run_thread_.join();
}

void Client::Update() {
	switch (state_) {
	case kConectWait:
		if (has_conected_)
			state_ = kRun;
		break;
	case kRun: {
		Send();
		StartReceive();
		state_ = kCom;
		boost::thread thd(&Client::ThRun, this);
		run_thread_.swap(thd);
		break;
	}
	case kCom: {
		break;
	}
	}
}
void Client::Draw() {

}
void Client::Connect() {
	socket_.async_connect(tcp::endpoint(asio::ip::address::from_string(kIpAdress), kPort),
			boost::bind(&Client::OnConnect, this, asio::placeholders::error));
}

void Client::OnConnect(const boost::system::error_code& error) {
	if (error) {
		std::cout << "Connect failed :" << error.message() << std::endl;
		return;
	}
	has_conected_ = true;
}

void Client::ThRun() {
	io_service_.run();
	io_service_.reset();
}

//クライアント情報送信
void Client::Send() {
	asio::async_write(socket_, asio::buffer(&send_data_, sizeof(ClientData)),
			boost::bind(&Client::OnSend, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

//送信完了
//error: エラー情報
// bytes_tranferred: 送信したバイト数
void Client::OnSend(const boost::system::error_code &error, size_t bytes_transferred) {
	if (error) {
		std::cout << "send failed:" << error.message() << std::endl;
		return;
	}

	Send();
}

//サーバー情報受信
void Client::StartReceive() {
	boost::asio::async_read(socket_, receive_buff_, asio::transfer_exactly(sizeof(ServerData)),
			boost::bind(&Client::OnRecive, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

// 受信完了
// error : エラー情報
// bytes_transferred : 受信したバイト数

void Client::OnRecive(const boost::system::error_code& error, size_t bytes_transferred) {
	if (error && error != boost::asio::error::eof) {
		std::cout << "receive failed:" << error.message() << std::endl;
		return;
	}
	const ServerData* recive_data = asio::buffer_cast<const ServerData*>(receive_buff_.data());
	receive_data_ = *recive_data;
	std::cout << "client_receive:" << kPort << ":" << recive_data->pos.x << std::endl;
	receive_buff_.consume(receive_buff_.size());
	StartReceive();
}
