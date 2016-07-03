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
	MemoryPool* pMemory = MemoryPool::getInstance();
	

	pClientManager->initClientInfoManager(io_service);
	pMemory->createMemoryPool();

	m_pGameNet = GameNetwork::getInstance(io_service);
}

void GameServer::startServer(boost::asio::io_service& io_service){
	std::cout << "동시 실행 가능한 스레드 :: " << boost::thread::hardware_concurrency() << std::endl;
	//네트워크 엔진 객체 초기화 / 실행
}

void GameServer::loopServer(boost::asio::io_service* io_service){
	//메인 루프->일정 주기마다 이벤트를 처리함

	m_pGameNet->Start();

	boost::shared_ptr<boost::asio::io_service::work> work;
	work.reset(new boost::asio::io_service::work(*io_service));
	io_service->run();
	//EventThread를 만들어서 그곳으로 빼내라
	//while (1);
	std::cout << "네트웍 접속 종료" << std::endl;
}

void GameServer::endServer(){
	//게임 서버 종료
	exit(1);
}