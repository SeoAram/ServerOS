#include "stdafx.h"
#include "GameEventProcess.h"


GameEventProcess::GameEventProcess(boost::asio::io_service& io_service) :
m_io_service(io_service)
{
	m_pEventThread = new boost::thread(mem_fun(&GameEventProcess::eventThread), this);
	cout << "create Event Thread" << endl;
}


GameEventProcess::~GameEventProcess()
{
}

void GameEventProcess::addGameEvent(const unsigned int objID, const unsigned int time, const EventType& type){
	//EnterCriticalSection(&m_csLock);
	m_EventQueue.push(GameEvent{ objID, time, type });
	//LeaveCriticalSection(&m_csLock);
}

void GameEventProcess::funcRegisterd(){
	//m_mapEventRoutine[EventType::MONSTER_MOVE] = bind(&GameEventProcess::monsterMove, this, placeholders::_1);
}

void GameEventProcess::eventProcess(/*const DWORD& objID, OVER_EX* overlapped*/){
	//SendOver* packet = reinterpret_cast<SendOver*>(overlapped);
	//auto iter = m_mapEventRoutine.find(packet->m_eGameEventType);
	//if (iter != m_mapEventRoutine.end()){
	//	m_mapEventRoutine[packet->m_eGameEventType](objID);
	//}
	//else{
	//	return;
	//}
}

void GameEventProcess::eventToWorkerthread(const GameEvent& myEvent){
	
}

void GameEventProcess::eventThread(){
	NetworkEngine* pNetEngine = NetworkEngine::getInstance(m_io_service);
	while (true){
		//EventThread를 만들어서 그곳으로 빼내라
		Sleep(1);
		pNetEngine->post(boost::bind(&NetworkEngine::workerThread, NetworkEngine::getInstance(m_io_service)));
	}
}

void GameEventProcess::monsterMove(unsigned int objID){
	
}