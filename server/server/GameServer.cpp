#include "stdafx.h"
#include "GameServer.h"


GameServer::GameServer()
{
}


GameServer::~GameServer()
{
}

void GameServer::initServer(boost::asio::io_service& io_service){
	//서버 실행을 할 때 필요한 초기화 작업들을 해줌
	//싱글톤 객체들 생성, 필요한 메모리들 미리 할당 받음
	IniData* pIniData = IniData::getInstance();
	GameMap* pGMap = GameMap::getInstance();
	ClientInfoManager* pClientManager = ClientInfoManager::getInstance();

	pClientManager->initClientInfoManager(io_service);
}

void GameServer::startServer(boost::asio::io_service& io_service){
	//네트워크 엔진 객체 초기화 / 실행
}

void GameServer::loopServer(boost::asio::io_service* io_service){
	//메인 루프->일정 주기마다 이벤트를 처리함


	m_pGameNet = new GameNetwork(*io_service);

	m_pGameNet->Start();
	io_service->run();

	//EventThread를 만들어서 그곳으로 빼내라
	std::cout << "네트웍 접속 종료" << std::endl;
	 
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
	//게임 서버 종료
	exit(1);
}