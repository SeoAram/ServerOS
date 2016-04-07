#pragma once
#include "stdafx.h"

class GameServer
{
private:
	NetworkEngine* m_pNetEngine;
	GameNetwork* m_pGameNet;
public:
	GameServer();
	~GameServer();

	//게임 서버 초기화
	void initServer(boost::asio::io_service& io_service);
	//서버 시작
	void startServer(boost::asio::io_service& io_service);
	//서버 로직 루프 실행
	void loopServer(boost::asio::io_service* io_service);
	//서버 종료
	void endServer();
};

