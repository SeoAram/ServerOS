#pragma once
#include "stdafx.h"

class ClientInfo
{
public:
	//ClientInfo();
	ClientInfo(boost::asio::io_service& io_service);
	ClientInfo(unsigned int i, boost::asio::io_service& io_service);
	//ClientInfo(unsigned int i);

	boost::asio::ip::tcp::socket& Socket(){
		return m_Socket;
	}

	void PostReceive();

private:
	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/);
	//접속 패킷을 받아봅시다
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);
	boost::asio::ip::tcp::socket m_Socket;
	std::string m_WriteMessage;
	std::array<char, 128> m_ReceiveBuffer;
	GameObject* m_pObject;	//object클래스(객체 이동 관리)
};

