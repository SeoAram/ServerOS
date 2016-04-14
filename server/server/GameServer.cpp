#include "stdafx.h"
#include "GameServer.h"


GameServer::GameServer()
{
}


GameServer::~GameServer()
{
}

void GameServer::initServer(boost::asio::io_service& io_service){
	//���� ������ �� �� �ʿ��� �ʱ�ȭ �۾����� ����
	//�̱��� ��ü�� ����, �ʿ��� �޸𸮵� �̸� �Ҵ� ����
	IniData* pIniData = IniData::getInstance();
	GameMap* pGMap = GameMap::getInstance();
	ClientInfoManager* pClientManager = ClientInfoManager::getInstance();
	MemoryPool* pMemory = MemoryPool::getInstance();
	
	m_pGameNet = GameNetwork::getInstance(io_service);

	pClientManager->initClientInfoManager(io_service);
	pMemory->createMemoryPool();
}

void GameServer::startServer(boost::asio::io_service& io_service){
	//��Ʈ��ũ ���� ��ü �ʱ�ȭ / ����
}

void GameServer::loopServer(boost::asio::io_service* io_service){
	//���� ����->���� �ֱ⸶�� �̺�Ʈ�� ó����

	m_pGameNet->Start();
	io_service->run();

	//EventThread�� ���� �װ����� ������
	std::cout << "��Ʈ�� ���� ����" << std::endl;
}

void GameServer::endServer(){
	//���� ���� ����
	exit(1);
}