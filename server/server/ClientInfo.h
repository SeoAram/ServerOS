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
	//���� ��Ŷ�� �޾ƺ��ô�
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);
	boost::asio::ip::tcp::socket m_Socket;

	std::string m_WriteMessage;		// sendpacket������, ���� �ʿ���
	GameObject* m_pObject;	//objectŬ����(��ü �̵� ����)
	GameNetwork* m_pGameNet; //Accept
	
	std::array<char, MAX_RECEIVE_BUFFER_LEN> m_ReceiveBuffer; // �޴���Ŷ ����

	int m_nPacketBufferMark;	//���� ��Ŷ�� ��ġ
	char m_PacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2];	//...??

	std::deque< Data* > m_SendDataQueue;
};

