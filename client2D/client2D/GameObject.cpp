#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject() : m_fCos(-90), m_fSin(90)
{
	float z = IniData::getInstance()->getData("MAP_HEIGHT");
	float x = IniData::getInstance()->getData("MAP_WIDTH");

	m_pvDir = new PointVector3D(1, 0, 1);
	m_pvPos = new PointVector3D(x, 0, z);
}

CGameObject::~CGameObject()
{
}

void CGameObject::turnLeft()
{
	m_fCos -= TURN;
	
	if (m_fCos < -180)
		m_fCos = -180;

	m_pvDir->x *= cosf(m_fCos * RADIAN);
	m_pvDir->vectorNormalization();
}
void CGameObject::turnRight()
{
	m_fCos += TURN;
	if (0 < m_fCos)
		m_fCos = 0;

	m_pvDir->x *= cosf(m_fCos * RADIAN);
	m_pvDir->vectorNormalization();
}
void CGameObject::turnUp()
{
	m_fSin += TURN;
	if (90 < m_fSin)
		m_fSin = 90;

	m_pvDir->z *= sinf(m_fSin * RADIAN);
	m_pvDir->vectorNormalization();
}
void CGameObject::turnDown()
{
	m_fSin -= TURN;
	if (m_fSin < -90)
		m_fSin = -90;

	m_pvDir->z *= sinf(m_fSin * RADIAN);
	m_pvDir->vectorNormalization();
}