#pragma once
#include "stdafx.h"

class GameServer
{
private:
	GameNetwork* m_pGameNet;
public:
	GameServer();
	~GameServer();

	//���� ���� �ʱ�ȭ
	void initServer(boost::asio::io_service& io_service);
	//���� ����
	void startServer(boost::asio::io_service& io_service);
	//���� ���� ���� ����
	void loopServer(boost::asio::io_service* io_service);
	//���� ����
	void endServer();
};

