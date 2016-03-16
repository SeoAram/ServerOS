#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject() :m_iObjId(MAX_CONNECT_CLIENT)
{
}

GameObject::GameObject(unsigned int i) : 
m_iObjId(i),
m_pPosition(),
m_pDirect()
{
}

GameObject::~GameObject()
{
}
