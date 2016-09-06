#include "../network/com_client.h"

ComClient::ComClient(asio::io_service &io_service, int port) :
		io_service_(io_service), acceptor_(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)), socket_(
				io_service), kPort(port) {
	has_accepted_ = false;
}

//接続
void ComClient::StartAccept() {
	acceptor_.async_accept(socket_, bind(&ComClient::OnAccept, this, asio::placeholders::error));
}

void ComClient::OnAccept(const boost::system::error_code& error) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "接続受信失敗:" + error.message());
		return;
	} else
		has_accepted_ = true;
}

//送受信スタート
void ComClient::Start() {
	if (!has_accepted_) {
		uErrorOut(__FILE__, __func__, __LINE__, "まだ接続されていません");
		return;
	}
	Send();
	Receive();
}

//送信
void ComClient::Send() {
	asio::async_write(socket_, asio::buffer(&send_data_, sizeof(ServerData)),
			bind(&ComClient::OnSend, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

void ComClient::OnSend(const boost::system::error_code& error, size_t bytes_transferred) {
	if (error) {
		uErrorOut(__FILE__, __func__, __LINE__, "送信失敗:" + error.message());
		return;
	}
	Send();
}

//受信
void ComClient::Receive() {
	asio::async_read(socket_, receive_buff_, asio::transfer_exactly(sizeof(ClientData)),
			bind(&ComClient::OnReceive, this, asio::placeholders::error,
					asio::placeholders::bytes_transferred));
}

void ComClient::OnReceive(const boost::system::error_code& error, size_t bytes_transferred) {
	if (error && error != asio::error::eof) {
		uErrorOut(__FILE__, __func__, __LINE__, "受信:" + error.message());
	} else {
		const ClientData* data = asio::buffer_cast<const ClientData*>(receive_buff_.data());
		receive_data_ = *data;
		printf("server_receive(%d):%f\n", kPort, receive_data_.pos.x);
		receive_buff_.consume(receive_buff_.size());
		Receive();
	}
}

//getter,setter
void ComClient::set_send_data(ServerData send_data) {
	send_data_ = send_data;
}

ClientData ComClient::get_receive_data() const {
	return receive_data_;
}

bool ComClient::get_has_accepted() const {
	return has_accepted_;
}

