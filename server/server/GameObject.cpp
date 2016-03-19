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
	//Map ������ �´��� Ȯ�� �� ���� ���Ϳ� ���� �̵�
	GameMap* pGameMap = GameMap::getInstance();

	*m_pPosition = *m_pPosition + *m_pDirect;
	int bx = ((int)m_pPosition->x + 2048) / BLOCK_COUNT;
	int bz = ((int)m_pPosition->z + 2048) / BLOCK_COUNT;

	if (m_wBlockX != bx || m_wBlockZ != bz){
		
		//���� ��Ͽ��� objID���� -> �ٸ� ��Ͽ� objID�Է�
		pGameMap->deleteObjId(bx, bz, m_iObjId);
		pGameMap->insertObjId(bx, bz, m_iObjId);
	}
}