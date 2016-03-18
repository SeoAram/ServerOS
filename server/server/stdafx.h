// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <unordered_map>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace std;


#define WORKED_THREAD 1
const unsigned int MAX_CONNECT_CLIENT = 3000;
const unsigned short PORT_NUMBER = 31400;
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "protocol.h"
#include "PointVector3D.h"

#include "GameObject.h"
#include "ClientInfo.h"
#include "GameMap.h"

#include "ClientInfoManager.h"
#include "GameMapManager.h"

#include "MemoryPool.h"
#include "PacketProcess.h"
#include "NetworkEngine.h"

#include "GameServer.h"