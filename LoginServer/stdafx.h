
#pragma once

#pragma warning(disable:4311)
#pragma warning(disable:4267)
#pragma warning(disable:4312)

//-----------------------------------------------------------------------------
// 引用底层引擎
//-----------------------------------------------------------------------------

#define _WIN32_WINNT 0x0403
#define _USE_D3D7_

#define _SECURE_SCL 1
#define _SECURE_SCL_THROWS 1

//包含vEngine头文件
#include "..\vEngine\vEngine.h"
using namespace vEngine;

#include "..\Beton\Beton.h"
using namespace Beton;

#ifdef _DEBUG
#define X_STRING_RUN_TIME "Debug"
#pragma comment(lib,"..\\vsout\\vEngine\\debug\\vEngine.lib")
#else
#define X_STRING_RUN_TIME "Release"
#pragma comment(lib,"..\\vsout\\vEngine\\release\\vEngine.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib,"..\\vsout\\Beton\\debug\\Beton.lib")
#else
#pragma comment(lib,"..\\vsout\\Beton\\release\\Beton.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib,"..\\vsout\\ServerDefine\\debug\\ServerDefine.lib")
#else
#pragma comment(lib,"..\\vsout\\ServerDefine\\release\\ServerDefine.lib")
#endif

#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"winmm.lib")

#include "..\WorldDefine\base_define.h"

#include "..\WorldDefine\login.h"
#include "..\WorldDefine\time.h"

//引用STL
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

#define DEBUG

//定义的消息结构
#include "LoginServer_define.h"

#pragma pack(push, 1)

#define X_STRING_LEN	50			// 字符串长度
#define X_USERNAME_LENEX  128

//#define DEBUG

//-----------------------------------------------------------------------------
// 网络消息基本结构
//-----------------------------------------------------------------------------
struct tagNetCmd2
{
	DWORD	dwID;
	DWORD	dwSize;

	DWORD Crc32(LPCSTR lpData)
	{
		UINT unResult;
		LPBYTE pData = (LPBYTE)lpData;
		if( 0 == pData[0] ) 
			return (DWORD)GT_INVALID;
		unResult = *pData++ << 24;
		if( *pData )
		{
			unResult |= *pData++ << 16;
			if( *pData )
			{
				unResult |= *pData++ << 8;
				if( *pData ) unResult |= *pData++;
			}
		}
		unResult = ~ unResult;
		while( *pData )
		{
			unResult = (unResult << 8 | *pData) ^ globe_Crc32Table[unResult >> 24];
			pData++;
		}
		return ~unResult;
	}
};


//-----------------------------------------------------------------------------
// 方便网络消息的定义
//-----------------------------------------------------------------------------
#ifndef CMD_START2
#define CMD_START2(name)	\
struct tag##name : public tagNetCmd2{	\
	tag##name(){ ZeroMemory(this,sizeof(*this));  \
	dwID = Crc32(#name); dwSize = sizeof(*this); }
#define CMD_END2		};
#endif

#ifndef YYLogin_LOG 
//#define YYLogin_LOG
#endif
#ifndef KOREA_LOG 
//#define KOREA_LOG
	#ifdef  KOREA_LOG
	#define TS_LOG
	#define TEST_MIX_SERVER
	#endif
#endif

#pragma pack(pop)


//类前置声明
//class LoongWorld;
//class Player;
//class LoongWorldMgr;