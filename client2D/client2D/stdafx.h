// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#pragma comment(lib, "ws2_32")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <winsock2.h>

#include <cmath>

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

#define RADIAN 3.14159265358979323846 / 180.0
#define TURN 5

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "../../common/iniRead.h"
#include "../../common/protocol.h"
#include "../../common/PointVector3D.h"


#include "GameObject.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif