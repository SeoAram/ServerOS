#include "stdafx.h"
#include "GameEventProcess.h"

GameEventProcess::GameEventProcess():
MAX_EVENT_THREAD(IniData::getInstance()->getData("MAX_THREAD"))
{
	m_pLock = new boost::mutex();
	m_pMemoryLock = new boost::mutex();
	m_EventQueue.empty();
	for (int i = 0; i < 3001; ++i)
		m_pEventMemory.push(new GameEvent);
	//m_pEventThread = new boost::thread(mem_fun(&GameEventProcess::eventThread), this);
	for (int i = 0; i < MAX_EVENT_THREAD; ++i)
		m_vThread.push_back(new boost::thread(mem_fun(&GameEventProcess::eventToWorkerthread), this, i));
	cout << "create Event Thread" << endl;
}

GameEventProcess::~GameEventProcess()
{
	//-m_pEventThread->join();
}

void GameEventProcess::addGameEvent(const unsigned int objID, float delayTime_ms, const EventType& type){
	lock();
	while (m_pEventMemory.size() == 0)
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	memoryLock();
	GameEvent* gMemory = m_pEventMemory.front();
	m_pEventMemory.pop();
	memoryUnlock();

	gMemory->objID = objID;
	gMemory->wakeTime = boost::posix_time::microsec_clock::local_time() + boost::posix_time::milliseconds(delayTime_ms);
	gMemory->delayTime_ms = delayTime_ms;
	gMemory->eType = type;
	m_EventQueue.push(gMemory);

	std::cout << "add Event :: " << objID << " / Event Queue Size :: " << m_EventQueue.size() << " / Memory Size :: " << m_pEventMemory.size() << std::endl;
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

void GameEventProcess::eventToWorkerthread(const int threadNum){
	boost::this_thread::sleep(boost::posix_time::seconds(1));
	static ClientInfoManager* pManage = ClientInfoManager::getInstance();
	ClientInfo* cInfo = nullptr;
	GameObject* gObj = nullptr;
	unsigned short a_bx;
	unsigned short a_bz;
	GameMap* pGameMap = GameMap::getInstance();
	boost::posix_time::time_duration diff;
	while (true){
		for (int i = 0; i < MAX_CONNECT_CLIENT; ++i){
			if ((i % MAX_EVENT_THREAD) == threadNum){
				cInfo = pManage->getClient(i);
				if (cInfo != nullptr){
					gObj = cInfo->getObject();
					if (gObj->m_wState == IniData::getInstance()->getData("GAME_OBJECT_MOVE")){
						a_bx = gObj->m_wBlockX;
						a_bz = gObj->m_wBlockZ;
						diff = boost::posix_time::microsec_clock::local_time() - gObj->getLastChangeTime(); //시간 차 확인
						if (diff.total_milliseconds() < 0)
							continue;
						cInfo->getObject()->moveObject(diff.total_milliseconds() * 0.001);

						if (gObj->m_wBlockX != a_bx || gObj->m_wBlockZ != a_bz){
							//기존 블록에서 objID제거 -> 다른 블록에 objID입력
							//주변 8개 블록에서도 제거해주어야 함
							//새로 이동한 주변 8개 블록에 등장 알려야함
							if (pGameMap->deleteObjId(a_bx, a_bz, gObj->getObjId())){ // 성공했을 때만
								pGameMap->insertObjId(gObj->m_wBlockX, gObj->m_wBlockZ, gObj->getObjId());
								PacketLogout lPack;
								lPack.Init();
								lPack.id = gObj->getObjId();

								short difX = gObj->m_wBlockX - a_bx;
								short difZ = gObj->m_wBlockZ - a_bz;

								pGameMap->sendObjId(a_bx, a_bz, false, gObj->getObjId(), (char*)&lPack, PacketType::LOGOUT_PACKET_LIST);
								PacketInit iPack;
								iPack.Init();
								iPack.pos_x = gObj->m_pPosition->x;
								iPack.pos_y = gObj->m_pPosition->y;
								iPack.pos_z = gObj->m_pPosition->z;
								iPack.dir_x = gObj->m_pDirect->x;
								iPack.dir_y = gObj->m_pDirect->y;
								iPack.dir_z = gObj->m_pDirect->z;
								iPack.iAxis = gObj->m_iAxis;
								iPack.id = gObj->getObjId();
								//아래 함수 하면 에러남
								pGameMap->sendObjId(gObj->m_wBlockX, gObj->m_wBlockZ, false, i, (char*)&iPack, PacketType::LOGIN_PACKET_LIST);
							}
						}
					}
				}
			}
		}
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
}

void GameEventProcess::eventThread(){
	GameEvent* gEvent;

	std::cout << "Event Thread run " << std::endl;

	while (true){
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		lock();
		if (m_EventQueue.size() == 0){
			unlock();
			continue;
		}
		gEvent = m_EventQueue.top(); 
		if (boost::posix_time::microsec_clock::local_time() < gEvent->wakeTime){
			unlock();
			continue;
		}
		m_EventQueue.pop();
		unlock();
		m_mapEventRoutine[gEvent->eType](gEvent->objID, gEvent);
	}
}

void GameEventProcess::characterMove(unsigned int objID, GameEvent* gEvent) {
	static ClientInfoManager* pManage = ClientInfoManager::getInstance();
	ClientInfo* cInfo = pManage->getClient(objID);
	GameObject* gObj = cInfo->getObject();
	GameMap* pGameMap = GameMap::getInstance();
	unsigned short a_bx = gObj->m_wBlockX;
	unsigned short a_bz = gObj->m_wBlockZ;

	boost::posix_time::time_duration diff = gEvent->wakeTime - gObj->getLastChangeTime(); //시간 차 확인
	cInfo->getObject()->moveObject(diff.total_milliseconds() * 0.001);

	/*if (diff.total_milliseconds() < gEvent.delayTime_ms){
		cInfo->getObject()->moveObject(diff.total_milliseconds() * 0.001);
		}
		else{
		cInfo->getObject()->moveObject(gEvent.delayTime_ms * 0.001);
		}*/
	memoryLock();
	m_pEventMemory.push(gEvent);
	memoryUnlock();

	if (gObj->m_wState == IniData::getInstance()->getData("GAME_OBJECT_MOVE"))
		addGameEvent(objID, 1000, EventType::CHARACTER_MOVE);

	if (gObj->m_wBlockX != a_bx || gObj->m_wBlockZ != a_bz){

		//기존 블록에서 objID제거 -> 다른 블록에 objID입력
		//주변 8개 블록에서도 제거해주어야 함
		//새로 이동한 주변 8개 블록에 등장 알려야함
		if (pGameMap->deleteObjId(a_bx, a_bz, objID)){ // 성공했을 때만
			pGameMap->insertObjId(gObj->m_wBlockX, gObj->m_wBlockZ, objID);
			PacketLogout lPack;
			lPack.Init();
			lPack.id = objID;

			short difX = gObj->m_wBlockX - a_bx;
			short difZ = gObj->m_wBlockZ - a_bz;

			/*int i = 0;
			PacketLogoutList lListPack;
			lListPack.Init();
			lListPack.id = objID;

			std::vector<int>& v = pGameMap->getObjIdList(a_bx, a_bz);
			for (auto& a : v){
			pManage->getClient(a)->PostSend(false, lPack.packetSize, (char*)&lPack);
			lListPack.idList[i++] = a;
			if (i == 10){
			i = 0;
			cInfo->PostSend(false, lListPack.packetSize, (char*)&lListPack);
			}
			}
			if (i != 0){
			cInfo->PostSend(false, lListPack.packetSize, (char*)&lListPack);
			}

			PacketInit iPack;
			iPack.Init();
			ClientInfo* tmp;
			v = pGameMap->getObjIdList(gObj->m_wBlockX, gObj->m_wBlockZ);
			for (auto& a : v){
			tmp = pManage->getClient(a);
			tmp->PostSend(false, lPack.packetSize, (char*)&lPack);
			iPack.pos_x	= tmp->getObject()->m_pPosition->x;
			iPack.pos_y	= tmp->getObject()->m_pPosition->y;
			iPack.pos_z	= tmp->getObject()->m_pPosition->z;
			iPack.dir_x	= tmp->getObject()->m_pDirect->x;
			iPack.dir_y	= tmp->getObject()->m_pDirect->y;
			iPack.dir_z	= tmp->getObject()->m_pDirect->z;
			iPack.iAxis	= tmp->getObject()->m_iAxis;
			iPack.id	= tmp->getObject()->getObjId();
			cInfo->PostSend(false, iPack.packetSize, (char*)&iPack);
			}

			}*/
		}
	}
}