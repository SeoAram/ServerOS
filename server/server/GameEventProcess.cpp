#include "stdafx.h"
#include "GameEventProcess.h"

GameEventProcess::GameEventProcess()
{
	m_pLock = new boost::mutex();
	m_EventQueue.empty();
	m_pEventThread = new boost::thread(mem_fun(&GameEventProcess::eventThread), this);
	cout << "create Event Thread" << endl;
}

GameEventProcess::~GameEventProcess()
{
	m_pEventThread->join();
}

void GameEventProcess::addGameEvent(const unsigned int objID, float delayTime_ms, const EventType& type){
	lock();
	m_EventQueue.push(GameEvent{ objID, 
		boost::posix_time::microsec_clock::local_time() + boost::posix_time::milliseconds(delayTime_ms), 
		delayTime_ms,
		type });

	std::cout << "add Event :: " << objID << std::endl;
	unlock();
}

void GameEventProcess::funcRegisterd(){
	m_mapEventRoutine[EventType::CHARACTER_MOVE] = bind(&GameEventProcess::characterMove, 
		this, 
		placeholders::_1, 
		placeholders::_2);
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

	/*while (true){
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		lock();
		if (m_EventQueue.size() == 0){
			unlock();
			continue;
		}
		gEvent = m_EventQueue.top();
		boost::posix_time::time_duration diff = (boost::posix_time::microsec_clock::local_time() - gEvent.wakeTime);
		std::cout << diff.total_milliseconds() << std::endl;
		if (boost::posix_time::microsec_clock::local_time() < gEvent.wakeTime){
			unlock();
			continue;
		}
		m_mapEventRoutine[gEvent.eType](gEvent.objID, gEvent);
		m_EventQueue.pop();
		unlock();
	}*/
}

void GameEventProcess::eventThread(){
	GameEvent gEvent;

	std::cout << "Event Thread run " << std::endl;

	while (true){
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		lock();
		if (m_EventQueue.size() == 0){
			unlock();
			continue;
		}
		gEvent = m_EventQueue.top();
		if (boost::posix_time::microsec_clock::local_time() < gEvent.wakeTime){
			unlock();
			continue;
		}
		m_EventQueue.pop();
		unlock();
		m_mapEventRoutine[gEvent.eType](gEvent.objID, gEvent);
	}
}

void GameEventProcess::characterMove(unsigned int objID, GameEvent& gEvent){
	static ClientInfoManager* pManage = ClientInfoManager::getInstance();
	ClientInfo* cInfo = pManage->getClient(objID);
	GameObject* gObj = cInfo->getObject();
	GameMap* pGameMap = GameMap::getInstance();
	unsigned short a_bx = gObj->m_wBlockX;
	unsigned short a_bz = gObj->m_wBlockZ;

	boost::posix_time::time_duration diff = gEvent.wakeTime - gObj->getLastChangeTime(); //시간 차 확인
	cInfo->getObject()->moveObject(diff.total_milliseconds() * 0.001);

	/*if (diff.total_milliseconds() < gEvent.delayTime_ms){
		cInfo->getObject()->moveObject(diff.total_milliseconds() * 0.001);
	}
	else{
		cInfo->getObject()->moveObject(gEvent.delayTime_ms * 0.001);
	}*/


	addGameEvent(objID, 1000, EventType::CHARACTER_MOVE);
	
	if (gObj->m_wBlockX != a_bx || gObj->m_wBlockZ != a_bz){

		//기존 블록에서 objID제거 -> 다른 블록에 objID입력
		//주변 8개 블록에서도 제거해주어야 함
		//새로 이동한 주변 8개 블록에 등장 알려야함
		if (pGameMap->deleteObjId(a_bx, a_bz, objID)){ // 성공했을 때만
			PacketLogout lPack;
			lPack.Init();
			lPack.id = objID;

			short difX = gObj->m_wBlockX - a_bx;
			short difZ = gObj->m_wBlockZ - a_bz;

			std::vector<int>& v = pGameMap->getObjIdList(a_bx, a_bz);
			for (auto& a : v)
				pManage->getClient(a)->PostSend(false, lPack.packetSize, (char*)&lPack);
			v = pGameMap->getObjIdList(gObj->m_wBlockX, gObj->m_wBlockZ);
			for (auto& a : v)
				pManage->getClient(a)->PostSend(false, lPack.packetSize, (char*)&lPack);
			cInfo->PostSend(false, lPack.packetSize, (char*)&lPack);

			/*std::vector<int>& v = pGameMap->getObjIdList(a_bx - difX, a_bz);
			for (auto& a : v)
				pManage->getClient(a)->setSendQueue(false, lPack.packetSize, (char*)&lPack);
			v = pGameMap->getObjIdList(a_bx, a_bz - difZ);
			for (auto& a : v)
				pManage->getClient(a)->setSendQueue(false, lPack.packetSize, (char*)&lPack);
			v = pGameMap->getObjIdList(a_bx - difX, a_bz - difZ);
			for (auto& a : v)
				pManage->getClient(a)->setSendQueue(false, lPack.packetSize, (char*)&lPack);


			pGameMap->insertObjId(gObj->m_wBlockX, gObj->m_wBlockZ, objID);

			lPack.protocol = PacketType::LOGIN_PACKET;
			v = pGameMap->getObjIdList(gObj->m_wBlockX + difX, gObj->m_wBlockZ);
			for (auto& a : v)
				pManage->getClient(a)->setSendQueue(false, lPack.packetSize, (char*)&lPack);
			v = pGameMap->getObjIdList(gObj->m_wBlockX, gObj->m_wBlockZ + difZ);
			for (auto& a : v)
				pManage->getClient(a)->setSendQueue(false, lPack.packetSize, (char*)&lPack);
			v = pGameMap->getObjIdList(gObj->m_wBlockX + difX, gObj->m_wBlockZ + difZ);
			for (auto& a : v)
				pManage->getClient(a)->setSendQueue(false, lPack.packetSize, (char*)&lPack);*/
		}
	}
}