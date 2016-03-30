#pragma once
#include "stdafx.h"
class CGameObject
{
public:
	PointVector3D* m_pvPos;
	PointVector3D* m_pvDir;

	float m_fSin, m_fCos;
	unsigned int m_wSpeed;

	void turnLeft();
	void turnRight();
	void turnUp();
	void turnDown();

	void move();

	CGameObject();
	~CGameObject();
};

