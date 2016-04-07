#pragma once
#include "stdafx.h"
class GameObject
{
private:
	int m_iAxis;
public:
	PointVector3D* m_pvPos;
	PointVector3D* m_pvDir;

	unsigned int m_wSpeed;

	void turnLeft();
	void turnRight();
	void turnUp();
	void turnDown();

	void move();

	GameObject();
	~GameObject();
};

