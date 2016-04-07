#pragma once
#include "stdafx.h"
class GameObject
{
private:
	unsigned int m_iObjId;		//객체 Id//함부로 변경 불가
	unsigned short m_wSpeed;
public:
	GameObject();
	GameObject(unsigned int i);
	~GameObject();
	PointVector3D* m_pPosition;	//x, y, z좌표
	PointVector3D* m_pDirect; //방향벡터
	unsigned short m_wBlockX, m_wBlockZ;	//맵에서 현재 위치
	unsigned short m_wState;	//객체의 상태

	void resetObject();
	unsigned int getObjId(){ return m_iObjId; }
	void moveObject();
};