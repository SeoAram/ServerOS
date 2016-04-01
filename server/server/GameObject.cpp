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
	//Map ������ �´��� Ȯ�� �� ���� ���Ϳ� ���� �̵�

	if (m_wState == IniData::getInstance()->getData("GAME_OBJECT_MOVE")){
		GameMap* pGameMap = GameMap::getInstance();
		//*m_pPosition = *m_pPosition + (*m_pDirect * m_wSpeed);

		int bx = ((int)m_pPosition->x + IniData::getInstance()->getData("GAME_MAP_WIDTH")) / BLOCK_COUNT;
		int bz = ((int)m_pPosition->z + IniData::getInstance()->getData("GAME_MAP_HEIGHT")) / BLOCK_COUNT;

		if (m_wBlockX != bx || m_wBlockZ != bz){

			//���� ��Ͽ��� objID���� -> �ٸ� ��Ͽ� objID�Է�
			//�ֺ� 8�� ��Ͽ����� �������־�� ��
			//���� �̵��� �ֺ� 8�� ��Ͽ� ���� �˷�����
			if (pGameMap->deleteObjId(bx, bz, m_iObjId)) // �������� ����
				pGameMap->insertObjId(bx, bz, m_iObjId);
		}
	}
}