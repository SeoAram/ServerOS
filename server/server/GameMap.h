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
	const unsigned short BLOCK_COUNT;
	struct __Block block[SIGHT_BLOCK][SIGHT_BLOCK];
	boost::mutex* m_sharedMutex[DEF_BLOCK_COUNT][DEF_BLOCK_COUNT];	//맵 접근에 대한 권한 관리
	std::vector<int> m_vObjIdBlock[DEF_BLOCK_COUNT][DEF_BLOCK_COUNT];
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
	unsigned short getBlockCount(){ return BLOCK_COUNT; }

	void insertObjId(short x, short z, unsigned int objId);
	bool deleteObjId(short x, short z, unsigned int objId);
	//std::vector<int>& getObjIdList(short x, short z);
	void sendObjId(short x, short z, const bool memoryCheck, unsigned int objId, char* pData, short _x = 0, short _z = 0);
	void sendObjId(short x, short z, const bool memoryCheck, unsigned int objId, char* pData, PacketType pType);
};

