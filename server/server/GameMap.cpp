#include "stdafx.h"
#include "GameMap.h"

GameMap::GameMap():
m_uBlockSizeH(IniData::getInstance()->getData("MAP_HEIGHT") / BLOCK_COUNT),
m_uBlockSizeW(IniData::getInstance()->getData("MAP_WIDTH") / BLOCK_COUNT), 
m_pClientManager(ClientInfoManager::getInstance())
{
	for (int i = 0; i < SIGHT_BLOCK; ++i){
		for (int j = 0; j < SIGHT_BLOCK; ++j){
			block[i][j].x = j - 1;
			block[i][j].y = i - 1;
		}
	}
}

GameMap::~GameMap()
{
}

void GameMap::insertObjId(short x, short z, unsigned int objId){
	m_sharedMutex[z][x].lock();
	m_vObjIdBlock[z][x].push_back(objId);
	std::cout << "Insert " << objId << std::endl;
	m_sharedMutex[z][x].unlock();
}

bool GameMap::deleteObjId(short x, short z, unsigned int objId){// false가 반환되는 경우 삭제 실패
	m_sharedMutex[z][x].lock();
	vector<int>::iterator iter = find_if(m_vObjIdBlock[z][x].begin(), m_vObjIdBlock[z][x].end(), [&](int x){return x == objId; });
	bool result = false;
	if (iter != m_vObjIdBlock[z][x].end()){
		m_vObjIdBlock[z][x].erase(iter);
		result = true;
		std::cout << "Delete " << objId << std::endl;
	}
	m_sharedMutex[z][x].unlock();
	return result;

}

std::vector<int>& GameMap::getObjIdList(short x, short z){
	m_sharedMutex[z][x].lock();
	std::vector<int>& result = m_vObjIdBlock[z][x];
	m_sharedMutex[z][x].unlock();
	return result;
}

void GameMap::sendObjId(short x, short z, const bool memoryCheck, unsigned int objId, char* pData, short _x, short _z){
	int size = m_vObjIdBlock[z][x].size();

	short tmpX, tmpY;

	if ((0 <= x && x < BLOCK_COUNT) && (0 <= z && z < BLOCK_COUNT)){
		for (int i : m_vObjIdBlock[z][x]){
			if (objId != i)
				m_pClientManager->getClient(i)->PostSend(memoryCheck, ((PacketHeader*)pData)->packetSize, pData);
			
		}
	}

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