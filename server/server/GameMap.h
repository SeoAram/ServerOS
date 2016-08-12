#pragma once
#include "stdafx.h"

#define SIGHT_BLOCK 3

class ClientInfoManager;

struct __Block{
	int x, y;
};

class GameMap
{
private:
	struct __Block block[SIGHT_BLOCK][SIGHT_BLOCK];
	boost::shared_mutex m_sharedMutex[BLOCK_COUNT][BLOCK_COUNT];	//맵 접근에 대한 권한 관리
	std::vector<int> m_vObjIdBlock[BLOCK_COUNT][BLOCK_COUNT];
	ClientInfoManager* m_pClientManager;
	const unsigned short m_uBlockSizeW;
	const unsigned short m_uBlockSizeH;
	GameMap();
public:
	static GameMap* getInstance(){
		static GameMap instance;
		return &instance;
	}
	~GameMap();

	unsigned short getBlockW(){ return m_uBlockSizeW; }
	unsigned short getBlockH(){ return m_uBlockSizeH; }

	void insertObjId(short x, short z, unsigned int objId);
	bool deleteObjId(short x, short z, unsigned int objId);
	std::vector<int>& getObjIdList(short x, short z);
	void sendObjId(short x, short z, const bool memoryCheck, unsigned int objId, char* pData, short _x = 0, short _z = 0);
};

