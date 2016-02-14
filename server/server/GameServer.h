#pragma once
class GameServer
{
public:
	GameServer();
	~GameServer();

	//게임 서버 초기화
	void initServer();
	//서버 시작
	void startServer();
	//서버 로직 루프 실행
	void loopServer();
	//서버 종료
	void endServer();
};

