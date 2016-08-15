#include "stdafx.h"
#include "GameMap.h"

GameMap::GameMap():
m_uBlockSizeH(IniData::getInstance()->getData("BLOCK_SIZE")),
m_uBlockSizeW(IniData::getInstance()->getData("BLOCK_SIZE")), 
BLOCK_COUNT(IniData::getInstance()->getData("MAP_WIDTH") / IniData::getInstance()->getData("BLOCK_SIZE")),
m_pClientManager(ClientInfoManager::getInstance())
{
	for (int i = 0; i < SIGHT_BLOCK; ++i){
		for (int j = 0; j < SIGHT_BLOCK; ++j){
			block[i][j].x = j - 1;
			block[i][j].y = i - 1;
		}
	}

	for (int i = 0; i < BLOCK_COUNT; ++i){
		for (int j = 0; j < BLOCK_COUNT; ++j)
			m_sharedMutex[i][j] = new boost::mutex();
	}
}

GameMap::~GameMap()
{
}

void GameMap::insertObjId(short x, short z, unsigned int objId){
	if (BLOCK_COUNT <= x)
		x = BLOCK_COUNT - 1;
	else if (x < 0)
		x = 0;
	if (BLOCK_COUNT <= z)
		z = BLOCK_COUNT - 1;
	else if (z < 0)
		z = 0;
	m_sharedMutex[z][x]->lock();
	m_vObjIdBlock[z][x].push_back(objId);
	std::cout << "Insert (" << x << ", " << z << "): " << objId << " size :: " << m_vObjIdBlock[z][x].size() << std::endl; 
	m_sharedMutex[z][x]->unlock();
}

bool GameMap::deleteObjId(short x, short z, unsigned int objId){// false가 반환되는 경우 삭제 실패
	if (BLOCK_COUNT <= x)
		x = BLOCK_COUNT - 1;
	else if (x < 0)
		x = 0;
	if (BLOCK_COUNT <= z)
		z = BLOCK_COUNT - 1;
	else if (z < 0)
		z = 0;
	m_sharedMutex[z][x]->lock();
	vector<int>::iterator iter = find_if(m_vObjIdBlock[z][x].begin(), m_vObjIdBlock[z][x].end(), [&](int x){return x == objId; });
	bool result = false;
	if (iter != m_vObjIdBlock[z][x].end()){
		m_vObjIdBlock[z][x].erase(iter);
		result = true;
		std::cout << "Delete " << objId << std::endl;
	}
	m_sharedMutex[z][x]->unlock();
	return result;

}

//std::vector<int>& GameMap::getObjIdList(short x, short z){
//	if (BLOCK_COUNT <= x)
//		x = BLOCK_COUNT - 1;
//	else if (x < 0)
//		x = 0;
//	if (BLOCK_COUNT <= z)
//		z = BLOCK_COUNT - 1;
//	else if (z < 0)
//		z = 0;
//	m_sharedMutex[z][x]->lock();
//	std::vector<int>& result = m_vObjIdBlock[z][x];
//	std::cout << "(" << x << ", " << z << ") :: " << result.size() << std::endl;
//	m_sharedMutex[z][x]->unlock();
//	return result;
//}

void GameMap::sendObjId(short x, short z, const bool memoryCheck, unsigned int objId, char* pData, PacketType pType){
	ClientInfoManager* pManage = ClientInfoManager::getInstance();
	ClientInfo* cInfo;
	ClientInfo* sendClient;
	if (BLOCK_COUNT <= x)
		x = BLOCK_COUNT - 1;
	else if (x < 0)
		x = 0;
	if (BLOCK_COUNT <= z)
		z = BLOCK_COUNT - 1;
	else if (z < 0)
		z = 0;
	std::cout << "sendObjId :: " << objId << " (" << x << ", " << z << ")" << std::endl;
	m_sharedMutex[z][x]->lock();
	int i = 0;
	PacketIdList lListPack;
	lListPack.Init();
	lListPack.protocol = pType;
	lListPack.id = objId;

	sendClient = pManage->getClient(objId);
	for (auto a : m_vObjIdBlock[z][x]){
		if (a == objId)
			continue;
		cInfo = pManage->getClient(a);
		if (cInfo->Socket().is_open() 
			&& cInfo->getObject()->m_wState != IniData::getInstance()->getData("GAME_OBJECT_LOGOUT")){

			cInfo->PostSend(memoryCheck, ((PacketHeader*)pData)->packetSize, pData);

			if (pType == PacketType::LOGOUT_PACKET_LIST)
				lListPack.idList[i++] = a;
			else if (pType == PacketType::LOGIN_PACKET_LIST){
				PacketInit iPack;
				iPack.Init();
				iPack.id = a;
				iPack.pos_x = cInfo->getObject()->m_pPosition->x;
				iPack.pos_y = cInfo->getObject()->m_pPosition->y;
				iPack.pos_z = cInfo->getObject()->m_pPosition->z;
				iPack.dir_x = cInfo->getObject()->m_pDirect->x;
				iPack.dir_y = cInfo->getObject()->m_pDirect->y;
				iPack.dir_z = cInfo->getObject()->m_pDirect->z;
				iPack.iAxis = cInfo->getObject()->m_iAxis;
				sendClient->PostSend(false, iPack.packetSize, (char*)&iPack);
			}
		}

		if (i == 10){
			lListPack.idSize = i;
			sendClient->PostSend(false, lListPack.packetSize, (char*)&lListPack);
			i = 0;
		}
	}
	m_sharedMutex[z][x]->unlock();
	if (i != 0){
		lListPack.idSize = i;
		sendClient->PostSend(false, lListPack.packetSize, (char*)&lListPack);
	}
}

void GameMap::sendObjId(short x, short z, const bool memoryCheck, unsigned int objId, char* pData, short _x, short _z){



	/*int size = m_vObjIdBlock[z][x].size();

	short tmpX, tmpY;

	if ((0 <= x && x < BLOCK_COUNT) && (0 <= z && z < BLOCK_COUNT)){
		for (int i : m_vObjIdBlock[z][x]){
			if (objId != i)
				m_pClientManager->getClient(i)->PostSend(memoryCheck, ((PacketHeader*)pData)->packetSize, pData);
			
		}
	}*/

	/*if (_x == 0 && _z == 0){
		for (int i = 0; i < SIGHT_BLOCK; ++i){
			for (int j = 0; j < SIGHT_BLOCK; ++j){
				tmpX = x + block[i][j].x;
				tmpY = z + block[i][j].y;

				if ((0 <= (tmpX) && (tmpX) < BLOCK_COUNT) && (0 <= (tmpY) && (tmpY) < BLOCK_COUNT)){
					for (int k = 0; k < m_vObjIdBlock[tmpY][tmpX].size(); ++k){
						if (objId != m_vObjIdBlock[tmpY][tmpX][k]){
							m_pClientManager->getClient(m_vObjIdBlock[tmpY][tmpX][k])->PostSend(memoryCheck, ((PacketHeader*)pData)->packetSize, pData);
						}
					}
				}
			}
		}
	}
	else{
		for (int i = 0; i < SIGHT_BLOCK; ++i){
			for (int j = 0; j < SIGHT_BLOCK; ++j){
				tmpX = x + block[i][j].x;
				tmpY = z + block[i][j].y;
				if ((block[i][j].x == _x || block[i][j].y == _z) &&
					(0 <= (tmpX) && (tmpX) < BLOCK_COUNT) && (0 <= (tmpY) && (tmpY) < BLOCK_COUNT)){
					for (int k = 0; k < m_vObjIdBlock[tmpY][tmpX].size(); ++k){
						m_pClientManager->getClient(m_vObjIdBlock[tmpY][tmpX][k])->PostSend(memoryCheck, ((PacketHeader*)pData)->packetSize, pData);
					}
				}
			}
		}
	}*/
}