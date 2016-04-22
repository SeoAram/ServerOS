#pragma once
#include "stdafx.h"

class GameObject;
class GameNetwork;

class ClientInfo
{
public:
	ClientInfo(boost::asio::io_service& io_service);
	ClientInfo(boost::asio::io_service& io_service, GameNetwork* pGameNet = nullptr);
	ClientInfo(unsigned int i, boost::asio::io_service& io_service, GameNetwork* pGameNet = nullptr);
	~ClientInfo();

	boost::asio::ip::tcp::socket& Socket(){
		return m_Socket;
	}

	void PostReceive();
	void PostSend(const bool bImmediately, const int nSize, char* pData);
	GameObject* getObject(){ return m_pObject; }

	void Init();

private:
	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/);
	//접속 패킷을 받아봅시다
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);
	boost::asio::ip::tcp::socket m_Socket;

	std::string m_WriteMessage;		// sendpacket같은데, 변경 필요함
	GameObject* m_pObject;	//object클래스(객체 이동 관리)
	GameNetwork* m_pGameNet; //Accept
	
	std::array<char, MAX_RECEIVE_BUFFER_LEN> m_ReceiveBuffer; // 받는패킷 버퍼

	int m_nPacketBufferMark;	//수신 패킷의 위치
	char m_PacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2];	//...??

	std::deque< Data* > m_SendDataQueue;
};

