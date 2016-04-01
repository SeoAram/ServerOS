#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject() :m_iObjId(MAX_CONNECT_CLIENT), 
m_wSpeed(3)
{
}

GameObject::GameObject(unsigned int i) : 
m_iObjId(i),
m_wSpeed(IniData::getInstance()->getData("OBJECT_SPEED")),
m_wState(IniData::getInstance()->getData("GAME_OBJECT_STAT"))
{
	m_pPosition = new PointVector3D();
	m_pDirect = new PointVector3D(1, 0, 1);
	m_pDirect->vectorNormalization();

	m_wBlockX = (int)m_pPosition->x / BLOCK_COUNT;
	m_wBlockZ = (int)m_pPosition->z / BLOCK_COUNT;
}

GameObject::~GameObject()
{
}

void GameObject::moveObject(){
	//Map 범위에 맞는지 확인 후 방향 벡터에 따라 이동

	if (m_wState == IniData::getInstance()->getData("GAME_OBJECT_MOVE")){
		GameMap* pGameMap = GameMap::getInstance();
		//*m_pPosition = *m_pPosition + (*m_pDirect * m_wSpeed);

		int bx = ((int)m_pPosition->x + IniData::getInstance()->getData("GAME_MAP_WIDTH")) / BLOCK_COUNT;
		int bz = ((int)m_pPosition->z + IniData::getInstance()->getData("GAME_MAP_HEIGHT")) / BLOCK_COUNT;

		if (m_wBlockX != bx || m_wBlockZ != bz){

			//기존 블록에서 objID제거 -> 다른 블록에 objID입력
			//주변 8개 블록에서도 제거해주어야 함
			//새로 이동한 주변 8개 블록에 등장 알려야함
			if (pGameMap->deleteObjId(bx, bz, m_iObjId)) // 성공했을 때만
				pGameMap->insertObjId(bx, bz, m_iObjId);
		}
	}
}