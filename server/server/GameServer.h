#pragma once
class GameServer
{
public:
	GameServer();
	~GameServer();

	//���� ���� �ʱ�ȭ
	void initServer();
	//���� ����
	void startServer();
	//���� ���� ���� ����
	void loopServer();
	//���� ����
	void endServer();
};

