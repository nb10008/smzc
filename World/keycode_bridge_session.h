//-----------------------------------------------------------------------------
// Copyright (c) 2010 KONGZHONG Entertainment All rights reserved.
// filename: keycode_bridge_session.h
// data: 2010-09-06
// last:
// brief: 游戏服务器与KeyCodeBridgeServer的对话层
//-----------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"

#define GAME_ID 1300000	// 圣魔之血在会员中心里面的游戏ID

struct tagKeyCodeInfo
{
	DWORD	dwGameID;
	DWORD	dwServerID;	
	string		strAccount;
	DWORD	dwRoleID;
	string		strKeyCode;
	DWORD  dwClientIP;
	tagKeyCodeInfo(DWORD dwSerID, LPCSTR szAccount, DWORD dwRID, LPCSTR pKeyCode, DWORD dwIP)
	{
		dwGameID = GAME_ID;
		dwServerID = dwSerID;
		strAccount = szAccount;
		dwRoleID = dwRID;
		strKeyCode = pKeyCode;
		dwClientIP = dwIP;
	}
};

struct tagRecvResult 
{
	string strKeyCode;
	INT nResult;
};

//-----------------------------------------------------------------------------
class KeyCodeBridgeSession
{
public:
	KeyCodeBridgeSession();
	~KeyCodeBridgeSession();

public:
	BOOL Init();
	VOID Destroy();

	VOID Update();
	BOOL IsWell() { return (m_bInitOK && m_pTran->IsConnected()); }

	VOID Proof(LPCSTR szAccount, DWORD dwRoleID, LPCSTR szKeyCode, DWORD dwClientIP);

private:
	// 读取文件, 初始化成员
	BOOL InitConfig();

	// 连接数据库服务器
	DWORD ThreadConnectKeyCodeBridge();

	// 更新验证列表
	VOID UpdateProofList();

	VOID HandleRecvResult(LPBYTE pRecv);

	BOOL ParseProofData(const string &strFrameString, vector<tagRecvResult> &vecOrangeData);
	BOOL ParseSingleData(const string &strFrameString, string &strKeyCode, INT &nResult);

	BOOL CheckString(string& str);

private:
	TSFPTrunk<KeyCodeBridgeSession>			m_Trunk;
	TObjRef<Util>						m_pUtil;
	TObjRef<StreamTransport>	m_pTran;
	TObjRef<Thread>					m_pThread;

	// 连接参数
	CHAR										m_szIP[X_IP_LEN];	// 服务器ip
	DWORD									m_dwPort;				// 服务器port
	DWORD									m_dwOn;					// key码充值开关

	DWORD									m_dwWorldID;				// 服务器ID

	volatile BOOL				m_bTermConnect;			// 记录当前连接状态
	volatile BOOL				m_bInitOK;						// 是否初始化完成

	TList<tagKeyCodeInfo*>		m_listKeyCodeInfo;			// 验证用户数据
	TList<tagKeyCodeInfo*>		m_listRetKeyCodeInfo;	// 处理完数据下层处理

	CRITICAL_SECTION	m_ListLock;
	CRITICAL_SECTION	m_RetListLock;
};

extern KeyCodeBridgeSession g_keyCodeBridgeSession;		// KeyCodeBridge session全局对象
