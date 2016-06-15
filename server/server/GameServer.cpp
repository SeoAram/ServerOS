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
	

	pClientManager->initClientInfoManager(io_service);
	pMemory->createMemoryPool();

	m_pGameNet = GameNetwork::getInstance(io_service);
}

void GameServer::startServer(boost::asio::io_service& io_service){
	std::cout << "���� ���� ������ ������ :: " << boost::thread::hardware_concurrency() << std::endl;
	//��Ʈ��ũ ���� ��ü �ʱ�ȭ / ����
}

void GameServer::loopServer(boost::asio::io_service* io_service){
	//���� ����->���� �ֱ⸶�� �̺�Ʈ�� ó����

	m_pGameNet->Start();

	boost::shared_ptr<boost::asio::io_service::work> work;
	work.reset(new boost::asio::io_service::work(*io_service));
	io_service->run();
	//EventThread�� ���� �װ����� ������
	//while (1);
	std::cout << "��Ʈ�� ���� ����" << std::endl;
}

void GameServer::endServer(){
	//���� ���� ����
	exit(1);
}