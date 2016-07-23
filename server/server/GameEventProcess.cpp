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

void GameEventProcess::addGameEvent(const unsigned int objID, float delayTime_ms, const EventType& type){
	lock();
	m_EventQueue.push(GameEvent{ objID, 
		boost::posix_time::microsec_clock::local_time() + boost::posix_time::seconds(delayTime_ms), 
		delayTime_ms,
		type });
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
		m_mapEventRoutine[gEvent.eType](gEvent.objID, gEvent.delayTime_ms);
		m_EventQueue.pop();
		unlock();
	}
}

void GameEventProcess::eventThread(){
	
}

void GameEventProcess::characterMove(unsigned int objID, float delayTime){
	static ClientInfoManager* pManage = ClientInfoManager::getInstance();
	ClientInfo* cInfo = pManage->getClient(objID);
	GameObject* gObj = cInfo->getObject();
	GameMap* pGameMap = GameMap::getInstance();
	unsigned short a_bx = gObj->m_wBlockX;
	unsigned short a_bz = gObj->m_wBlockZ;
	cInfo->getObject()->moveObject(delayTime * 0.001);

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
				pManage->getClient(a)->setSendQueue(false, lPack.packetSize, (char*)&lPack);
			v = pGameMap->getObjIdList(gObj->m_wBlockX, gObj->m_wBlockZ);
			for (auto& a : v)
				pManage->getClient(a)->setSendQueue(false, lPack.packetSize, (char*)&lPack);

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