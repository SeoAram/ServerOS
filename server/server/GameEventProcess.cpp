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
	m_EventQueue.push(GameEvent{ objID, time, type });
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
	
}

void GameEventProcess::eventThread(){
	
}

void GameEventProcess::characterMove(unsigned int objID){
	
}