// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <climits>

#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <unordered_map>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace std;


#define WORKED_THREAD 1
#define BLOCK_COUNT 32


enum class MsgType : unsigned char{
	RECVmsg = 1,
	SENDmsg,
	EVENTmsg
};

const unsigned short MAX_CONNECT_CLIENT = 3000;
const unsigned short PORT_NUMBER = 31400;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "../../Common/protocol.h"
#include "../../Common/iniRead.h"
#include "../../Common/PointVector3D.h"

#include "MemoryPool.h"

#include "GameObject.h"
#include "ClientInfo.h"
#include "GameMap.h"

#include "ClientInfoManager.h"
#include "GameMapManager.h"

#include "GameNetwork.h"

#include "MemoryPool.h"
#include "PacketProcess.h"
#include "GameEventProcess.h"

#include "GameServer.h"