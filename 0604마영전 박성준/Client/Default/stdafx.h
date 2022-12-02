// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <string>
#include <atlstr.h>

#include <algorithm>
#include <functional>
#include <stdio.h>
#include <ctime>

#include <process.h> // _beginthreadex �Լ��� ȣ���ϱ� ���ؼ� �ݵ�� �����ؾ� ��

#define	DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

#include "Transform.h"
#include "Renderer.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Texture.h"
#include "RcTex.h"
#include "atlstr.h"

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

#include <crtdbg.h>
using namespace std;
#include <iostream>

//#include<vld.h>

//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif