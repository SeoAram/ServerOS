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

	pClientManager->initClientInfoManager(io_service);
}

void GameServer::startServer(boost::asio::io_service& io_service){
	//��Ʈ��ũ ���� ��ü �ʱ�ȭ / ����

	m_pNetEngine = NetworkEngine::getInstance(io_service);
	m_pNetEngine->initNetworkEngine();
}

void GameServer::loopServer(boost::asio::io_service* io_service){
	//���� ����->���� �ֱ⸶�� �̺�Ʈ�� ó����

	//m_pNetEngine->acceptThread();

	io_service->run();


	while (true){
		Sleep(1);
	}

	std::cout << "��Ʈ�� ���� ����" << std::endl;

	/*GameEvent myEvent;
	while (true){
		Sleep(1);
		pEvent->enterCriticalSection();
		while (!pEvent->m_EventQueue.empty()){
			myEvent = pEvent->m_EventQueue.top();
			if (GetTickCount() < myEvent.wakeTime){
				pEvent->leaveCriticalSection();
				break;
			}
			pEvent->m_EventQueue.pop();
			pEvent->eventToWorkerthread(myEvent);
			if (pEvent->m_EventQueue.empty()){
				pEvent->leaveCriticalSection();
				break;
			}
		}
		pEvent->leaveCriticalSection();
	}*/
}

void GameServer::endServer(){
	//���� ���� ����
	exit(1);
}