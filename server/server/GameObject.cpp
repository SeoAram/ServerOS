#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject() :m_iObjId(MAX_CONNECT_CLIENT)
{
}

GameObject::GameObject(unsigned int i) : 
m_iObjId(i)
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
	GameMap* pGameMap = GameMap::getInstance();

	*m_pPosition = *m_pPosition + *m_pDirect;
	int bx = ((int)m_pPosition->x + 2048) / BLOCK_COUNT;
	int bz = ((int)m_pPosition->z + 2048) / BLOCK_COUNT;

	if (m_wBlockX != bx || m_wBlockZ != bz){
		
		//기존 블록에서 objID제거 -> 다른 블록에 objID입력
		pGameMap->deleteObjId(bx, bz, m_iObjId);
		pGameMap->insertObjId(bx, bz, m_iObjId);
	}
}