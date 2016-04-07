#pragma once
#include "stdafx.h"
class GameObject
{
private:
	unsigned int m_iObjId;		//��ü Id//�Ժη� ���� �Ұ�
	unsigned short m_wSpeed;
public:
	GameObject();
	GameObject(unsigned int i);
	~GameObject();
	PointVector3D* m_pPosition;	//x, y, z��ǥ
	PointVector3D* m_pDirect; //���⺤��
	unsigned short m_wBlockX, m_wBlockZ;	//�ʿ��� ���� ��ġ
	unsigned short m_wState;	//��ü�� ����

	void resetObject();
	unsigned int getObjId(){ return m_iObjId; }
	void moveObject();
};