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

	m_wBlockX = (int)m_pPosition->x / GameMap::getInstance()->getBlockW();
	m_wBlockZ = (int)m_pPosition->z / GameMap::getInstance()->getBlockH();
}

GameObject::~GameObject()
{
}

void GameObject::resetObject(){

	m_wSpeed = IniData::getInstance()->getData("OBJECT_SPEED");
	m_wState = IniData::getInstance()->getData("GAME_OBJECT_STAT");
	m_pPosition->x = m_pPosition->y = m_pPosition->z = 0;
	m_pDirect->x = m_pDirect->z = 1;
	m_pDirect->y = 0;
	m_pDirect->vectorNormalization();

	m_wBlockX = (int)m_pPosition->x / GameMap::getInstance()->getBlockW();
	m_wBlockZ = (int)m_pPosition->z / GameMap::getInstance()->getBlockH();
}

void GameObject::moveObject(){
	//Map ������ �´��� Ȯ�� �� ���� ���Ϳ� ���� �̵�

	if (m_wState == IniData::getInstance()->getData("GAME_OBJECT_MOVE")){
		GameMap* pGameMap = GameMap::getInstance();

		*m_pPosition = (*m_pPosition + &(*m_pDirect * (m_wSpeed*(1.0 / IniData::getInstance()->getData("FRAME_RATE")))));

		int bx = ((int)m_pPosition->x / pGameMap->getBlockW());
		int bz = ((int)m_pPosition->z / pGameMap->getBlockH());

		PacketMove movePack;
		movePack.Init();
		movePack.id = m_iObjId;

		if (m_wBlockX != bx || m_wBlockZ != bz){

			//���� ��Ͽ��� objID���� -> �ٸ� ��Ͽ� objID�Է�
			//�ֺ� 8�� ��Ͽ����� �������־�� ��
			//���� �̵��� �ֺ� 8�� ��Ͽ� ���� �˷�����
			if (pGameMap->deleteObjId(m_wBlockX, m_wBlockZ, m_iObjId)){ // �������� ����
				pGameMap->insertObjId(bx, bz, m_iObjId);
			}
		}
	}
}

void GameObject::moveObject(const PacketMove& mPack){
	//Map ������ �´��� Ȯ�� �� ���� ���Ϳ� ���� �̵�

	if (true || m_wState == IniData::getInstance()->getData("GAME_OBJECT_MOVE")){
		GameMap* pGameMap = GameMap::getInstance();

		m_pPosition->x = mPack.pos_x;
		m_pPosition->y = mPack.pos_y;
		m_pPosition->z = mPack.pos_z;
		m_pDirect->x = mPack.dir_x;
		m_pDirect->y = mPack.dir_y;
		m_pDirect->z = mPack.dir_z;

		int bx = ((int)m_pPosition->x / pGameMap->getBlockW());
		int bz = ((int)m_pPosition->z / pGameMap->getBlockH());

		if (m_wBlockX != bx || m_wBlockZ != bz){
			//���� ��Ͽ��� objID���� -> �ٸ� ��Ͽ� objID�Է�
			//�ֺ� 8�� ��Ͽ����� �������־�� ��
			//���� �̵��� �ֺ� 8�� ��Ͽ� ���� �˷�����
			if (pGameMap->deleteObjId(m_wBlockX, m_wBlockZ, m_iObjId)){ // �������� ����
				pGameMap->insertObjId(bx, bz, m_iObjId);
				m_wBlockX = bx;
				m_wBlockZ = bz;
			}
		}
	}
}