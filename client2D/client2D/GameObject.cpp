#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject() :m_iAxis(90), m_wSpeed(IniData::getInstance()->getData("OBJECT_SPEED"))
{
	float z = IniData::getInstance()->getData("MAP_HEIGHT");
	float x = IniData::getInstance()->getData("MAP_WIDTH");

	m_pvDir = new PointVector3D(1, 0, 1);
	m_pvPos = new PointVector3D(x, 0, z);
	m_pvDir->vectorNormalization();
}

CGameObject::~CGameObject()
{
}

void CGameObject::turnLeft()
{
	if (m_iAxis == 0){
		m_iAxis = 180;
	}
	else if(0 < m_iAxis && m_iAxis <= 180){
		m_iAxis += TURN;
		if (180 < m_iAxis)
			m_iAxis = 180;
	}
	else if (180 < m_iAxis){
		m_iAxis -= TURN;
		if (m_iAxis < 180)
			m_iAxis = 180;
	}

	m_iAxis %= 360;

}
void CGameObject::turnRight()
{
	if (m_iAxis == 180){
		m_iAxis = 0;
	}
	else if (0 < m_iAxis && m_iAxis <= 180){
		m_iAxis -= TURN;
		if (m_iAxis < 0)
			m_iAxis = 0;
	}
	else if (180 < m_iAxis){
		m_iAxis += TURN;
		if (360 < m_iAxis)
			m_iAxis = 0;
	}

	m_iAxis %= 360;
}
void CGameObject::turnDown()
{
	if (m_iAxis == 270){
		m_iAxis = 90;
	}
	else if (90 < m_iAxis && m_iAxis <= 270){
		m_iAxis -= TURN;
		if (m_iAxis < 90)
			m_iAxis = 90;
	}
	else if (m_iAxis < 90 || 270 < m_iAxis){
		m_iAxis += TURN;
		if (80 < m_iAxis && m_iAxis < 100)
			m_iAxis = 90;
	}

	m_iAxis %= 360;

}
void CGameObject::turnUp()
{
	if (m_iAxis == 90){
		m_iAxis = 270;
	}
	else if (90 <= m_iAxis && m_iAxis < 270){
		m_iAxis += TURN;
		if (270 < m_iAxis)
			m_iAxis = 270;
	}
	else if ((0 <= m_iAxis && m_iAxis < 90) || (270 < m_iAxis)){
		m_iAxis -= TURN;
		if (m_iAxis <= 0)
			m_iAxis += 360;
		if (260 < m_iAxis && m_iAxis < 280)
			m_iAxis = 270;
	}

	m_iAxis %= 360;

}

void CGameObject::move()
{
	m_pvDir->x = cos(m_iAxis * RADIAN) * 1;
	m_pvDir->z = sin(m_iAxis * RADIAN) * 1;
	m_pvDir->vectorNormalization();
	*m_pvPos = (*m_pvPos + &(*m_pvDir * 2));

	/*cout << "Position : ";
	m_pvPos->operator<<(cout);
	cout << " Direct : ";
	m_pvDir->operator<<(cout) << endl;*/

}