// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#pragma comment(lib, "ws2_32")

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#define SERVERIP   "127.0.0.1"
#define SERVERPORT 13400
#define BUFSIZE    128

using namespace std;