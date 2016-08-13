#pragma once
#include "stdafx.h"

class Random{
private:
	std::default_random_engine m_dre;
	std::uniform_real_distribution<float>* m_pDx;
	std::uniform_real_distribution<float>* m_pDy;
	Random(){ 
		m_pDx = new std::uniform_real_distribution<float>(0, IniData::getInstance()->getData("MAP_WIDTH"));
		m_pDy = new std::uniform_real_distribution<float>(0, IniData::getInstance()->getData("MAP_HEIGHT"));
	}
public:
	static Random* getInstance(){
		static Random instance;
		return &instance;
	}

	float getRandomX(){ return (*m_pDx)(m_dre); }
	float getRandomY(){ return (*m_pDy)(m_dre); }
};


class GameObject
{
private:
	unsigned int m_iObjId;		//��ü Id//�Ժη� ���� �Ұ�
	unsigned short m_wSpeed;
	boost::posix_time::ptime m_lastChangeTime;

public:
	GameObject();
	GameObject(unsigned int i);
	~GameObject();
	PointVector3D* m_pPosition;	//x, y, z��ǥ
	PointVector3D* m_pDirect; //���⺤��
	unsigned short m_wBlockX, m_wBlockZ;	//�ʿ��� ���� ��ġ
	unsigned short m_wState;	//��ü�� ����
	int m_iAxis;				//ȸ����

	void resetObject();
	unsigned int getObjId(){ return m_iObjId; }
	void moveObject();
	void moveObject(const PacketMove& mPack);
	void moveObject(float second);

	void setData(const PacketMove* const pData);

	boost::posix_time::ptime getLastChangeTime(){ return m_lastChangeTime; }
};