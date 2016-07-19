#include "stdafx.h"
#include "GameEventProcess.h"

GameEventProcess::GameEventProcess(boost::asio::io_service& io_service) :
m_io_service(io_service)
{
	m_pLock = new boost::mutex();
	m_pEventThread = new boost::thread(mem_fun(&GameEventProcess::eventThread), this);
	cout << "create Event Thread" << endl;
}

GameEventProcess::~GameEventProcess()
{
}

void GameEventProcess::addGameEvent(const unsigned int objID, const boost::posix_time::ptime& time, const EventType& type){
	lock();
	m_EventQueue.push(GameEvent{ objID, time, type });
	unlock();
}

void GameEventProcess::funcRegisterd(){
	m_mapEventRoutine[EventType::CHARACTER_MOVE] = bind(&GameEventProcess::characterMove, this, placeholders::_1);
}

void GameEventProcess::eventProcess(/*const DWORD& objID*/){
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
	GameEvent gEvent;

	while (true){
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		lock();
		if (m_EventQueue.size() == 0)
			continue;
		gEvent = m_EventQueue.top();
		if (boost::posix_time::microsec_clock::local_time() < gEvent.wakeTime){
			unlock();
			continue;
		}
		m_mapEventRoutine[gEvent.eType](gEvent.objID);
		m_EventQueue.pop();
		unlock();
	}
}

void GameEventProcess::eventThread(){
	
}

void GameEventProcess::characterMove(unsigned int objID){
	ClientInfo* cInfo = ClientInfoManager::getInstance()->getClient(objID);
	cInfo->getObject()->moveObject();
}