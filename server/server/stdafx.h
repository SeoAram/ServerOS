// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
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

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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