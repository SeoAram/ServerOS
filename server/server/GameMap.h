#pragma once
#include "stdafx.h"

class GameMap
{
private:
	boost::shared_mutex m_sharedMutex;	//�� ���ٿ� ���� ���� ����
	vector<int> m_vObjIdBlock[BLOCK_COUNT][BLOCK_COUNT];
	GameMap();
public:
	static GameMap* getInstance(){
		static GameMap instance;
		return &instance;
	}
	GameMap(char m_cIsLoadMap);
	~GameMap();
};

