#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject() :m_iObjId(MAX_CONNECT_CLIENT), 
m_wSpeed(3), m_lastChangeTime(boost::posix_time::microsec_clock::local_time())
{
}

GameObject::GameObject(unsigned int i) : 
m_iObjId(i),
m_wSpeed(IniData::getInstance()->getData("OBJECT_SPEED")),
m_wState(IniData::getInstance()->getData("GAME_OBJECT_STAT")),
m_lastChangeTime(boost::posix_time::microsec_clock::local_time())
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

	std::srand(std::time(0));

	m_wSpeed = IniData::getInstance()->getData("OBJECT_SPEED");
	m_wState = IniData::getInstance()->getData("GAME_OBJECT_STAT");
	m_pPosition->x = Random::getInstance()->getRandomX();//std::rand() % IniData::getInstance()->getData("MAP_WIDTH");
	m_pPosition->y = 0;
	m_pPosition->z = Random::getInstance()->getRandomY();//std::rand() % IniData::getInstance()->getData("MAP_HEIGHT");
	m_pDirect->x = m_pDirect->z = 1;
	m_pDirect->y = 0;
	m_pDirect->vectorNormalization();

	m_iAxis = 90;

	std::cout << m_iObjId << " - (" << m_pPosition->x << ", 0, " << m_pPosition->z << ")" << std::endl;

	m_wBlockX = (int)m_pPosition->x / GameMap::getInstance()->getBlockW();
	m_wBlockZ = (int)m_pPosition->z / GameMap::getInstance()->getBlockH();
	m_lastChangeTime = boost::posix_time::microsec_clock::local_time();
}

void GameObject::moveObject(){
	//Map ������ �´��� Ȯ�� �� ���� ���Ϳ� ���� �̵�

	if (m_wState != IniData::getInstance()->getData("GAME_OBJECT_LOGOUT")){
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
	m_lastChangeTime = boost::posix_time::microsec_clock::local_time();
}

void GameObject::moveObject(const PacketMove& mPack){
	//Map ������ �´��� Ȯ�� �� ���� ���Ϳ� ���� �̵�

	if (m_wState != IniData::getInstance()->getData("GAME_OBJECT_LOGOUT")){
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
	m_lastChangeTime = boost::posix_time::microsec_clock::local_time();
}


void GameObject::moveObject(const float second){
	//Map ������ �´��� Ȯ�� �� ���� ���Ϳ� ���� �̵�

	if (m_wState != IniData::getInstance()->getData("GAME_OBJECT_LOGOUT")){
		GameMap* pGameMap = GameMap::getInstance();

		*m_pPosition = (*m_pPosition + &(*m_pDirect * (m_wSpeed * (1.0 / second))));
		if (m_pPosition->x < 0)
			m_pPosition->x = 0;
		else if (IniData::getInstance()->getData("MAP_WIDTH") <= m_pPosition->x)
			m_pPosition->x = IniData::getInstance()->getData("MAP_WIDTH");

		if (m_pPosition->z < 0)
			m_pPosition->z = 0;
		else if (IniData::getInstance()->getData("MAP_HEIGHT") <= m_pPosition->z)
			m_pPosition->z = IniData::getInstance()->getData("MAP_HEIGHT");

		m_wBlockX = ((int)m_pPosition->x / pGameMap->getBlockW());
		m_wBlockZ = ((int)m_pPosition->z / pGameMap->getBlockH());
		m_lastChangeTime = boost::posix_time::microsec_clock::local_time();
	}
}

void GameObject::setData(const PacketMove* const pData){
	m_pPosition->x = pData->pos_x;
	m_pPosition->y = pData->pos_y;
	m_pPosition->z = pData->pos_z;

	m_pDirect->x = pData->dir_x;
	m_pDirect->y = pData->dir_y;
	m_pDirect->z = pData->dir_z;

	m_iAxis = pData->wAxis;

	m_wState = IniData::getInstance()->getData("GAME_OBJECT_ALIVE");
	m_lastChangeTime = boost::posix_time::microsec_clock::local_time();
}