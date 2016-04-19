#pragma once

#include <vector>

#include "../common/iniRead.h"
#include "../common/protocol.h"
#include "../common/PointVector3D.h"

#define RADIAN 3.14159265358979323846 / 180.0
#define TURN 5
class GameObject
{
private:
	int m_iAxis;
	unsigned int m_uObjId;
public:
	PointVector3D* m_pvPos;
	PointVector3D* m_pvDir;

	unsigned int m_wSpeed;

	unsigned int getObjId(){ return m_uObjId; }
	int getAxis(){ return m_iAxis; }
	void setObjId(unsigned int i){ m_uObjId = i; }

	void turnLeft()
	{
		if (m_iAxis == 0){
			m_iAxis = 180;
		}
		else if (0 < m_iAxis && m_iAxis <= 180){
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
	void turnRight()
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
	void turnUp()
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
	void turnDown()
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

	void move()
	{
		m_pvDir->x = cosf(m_iAxis * RADIAN) * 1;
		m_pvDir->z = sinf(m_iAxis * RADIAN) * 1;
		m_pvDir->vectorNormalization();
		*m_pvPos = (*m_pvPos + &(*m_pvDir * (m_wSpeed)));
	}

	void initData(PacketInit& pData){
		m_uObjId = pData.id;
		m_pvPos->setXYZ(pData.pos_x, pData.pos_y, pData.pos_z);
		m_pvDir->setXYZ(pData.dir_x, pData.dir_y, pData.dir_z);
		m_pvDir->vectorNormalization();
		m_iAxis = 90;
	}

	GameObject() :m_uObjId(-1), m_iAxis(90), m_wSpeed(IniData::getInstance()->getData("OBJECT_SPEED"))
	{
		float z = IniData::getInstance()->getData("MAP_HEIGHT");
		float x = IniData::getInstance()->getData("MAP_WIDTH");

		m_pvDir = new PointVector3D(1, 0, 1);
		m_pvPos = new PointVector3D(x, 0, z);
		m_pvDir->vectorNormalization();
	}

	GameObject(unsigned int id) :m_uObjId(id), m_iAxis(90), m_wSpeed(IniData::getInstance()->getData("OBJECT_SPEED"))
	{
		float z = IniData::getInstance()->getData("MAP_HEIGHT");
		float x = IniData::getInstance()->getData("MAP_WIDTH");

		m_pvDir = new PointVector3D(1, 0, 1);
		m_pvPos = new PointVector3D(x, 0, z);
		m_pvDir->vectorNormalization();
	}

	~GameObject(){}
};


class GameObjectManager{
private:
	std::vector<GameObject*> m_vObject;
	GameObjectManager(){};
public:
	static GameObjectManager* getInstance(){
		static GameObjectManager instance;
		return &instance;
	}

	void setObject(const unsigned int objId, PacketInit& pData){
		//lock필요
		int size = m_vObject.size();
		bool checkObj = false;
		for (int i = 0; i < size; ++i){
			if (m_vObject[i]->getObjId() == objId)
				checkObj = true;
		}

		if (!checkObj){
			m_vObject.push_back(new GameObject(objId));
			m_vObject[size]->initData(pData);
		}
	}

	GameObject* getObject(const unsigned int objId){
		// lock필요
		int size = m_vObject.size();
		for (int i = 0; i < size; ++i){
			if (m_vObject[i]->getObjId() == objId)
				return m_vObject[i];
		}
		return nullptr;
	}

	void deleteObject(const unsigned int objId){
		//lock 필요
		int i = -1;
		auto iter = find_if(m_vObject.begin(), m_vObject.end(), [objId, &i](GameObject* obj){++i; return obj->getObjId() == objId; });
		if (iter != m_vObject.end()){
			GameObject* obj = m_vObject[i];
			m_vObject.erase(iter);
			delete obj;
		}
	}

	std::vector<GameObject*>* getObjectList(){
		return &m_vObject;
	}
};