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
	GameObject* getObject(){ return m_pObject; }

private:
	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/);
	//���� ��Ŷ�� �޾ƺ��ô�
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);
	boost::asio::ip::tcp::socket m_Socket;
	std::string m_WriteMessage;
	GameObject* m_pObject;	//objectŬ����(��ü �̵� ����)


	std::array<char, MAX_RECEIVE_BUFFER_LEN> m_ReceiveBuffer;

	int m_nPacketBufferMark;
	char m_PacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2];

	std::deque< char* > m_SendDataQueue;

};

