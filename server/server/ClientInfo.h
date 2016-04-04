#pragma once
#include "stdafx.h"

class ClientInfo
{
public:
	ClientInfo(boost::asio::io_service& io_service);
	ClientInfo(unsigned int i, boost::asio::io_service& io_service);

	boost::asio::ip::tcp::socket& Socket(){
		return m_Socket;
	}

	void PostReceive();
	void PostSend(const bool bImmediately, const int nSize, char* pData);
	GameObject* getObject(){ return m_pObject; }

	void Init(){ m_nPacketBufferMark = 0; };

private:
	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/);
	//접속 패킷을 받아봅시다
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);
	boost::asio::ip::tcp::socket m_Socket;
	std::string m_WriteMessage;
	GameObject* m_pObject;	//object클래스(객체 이동 관리)


	std::array<char, MAX_RECEIVE_BUFFER_LEN> m_ReceiveBuffer;

	int m_nPacketBufferMark;
	char m_PacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2];

	std::deque< char* > m_SendDataQueue;


};

