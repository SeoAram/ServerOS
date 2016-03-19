#include "stdafx.h"
#include "GameMap.h"

GameMap::GameMap(){

}

GameMap::~GameMap()
{
}

void GameMap::insertObjId(short x, short z, unsigned int objId){
	m_vObjIdBlock[z][x].push_back(objId);
}

bool GameMap::deleteObjId(short x, short z, unsigned int objId){// false가 반환되는 경우 삭제 실패
	vector<int>::iterator iter = find_if(m_vObjIdBlock[z][x].begin(), m_vObjIdBlock[z][x].end(), [&](int x){return x == objId; });
	bool result = false;
	if (iter != m_vObjIdBlock[z][x].end()){
		m_vObjIdBlock[z][x].erase(iter);
		result = true;
	}
	return result;
}
