//-----------------------------------------------------------------------------
// Copyright (c) 2010 KONGZHONG Entertainment All rights reserved.
// filename: keycode_bridge_session.cpp
// data: 2010-09-06
// last:
// brief: 游戏服务器与KeyCodeBridgeServer的对话层
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "keycode_bridge_session.h"
#include "..\ServerDefine\base_define.h"
#include "world.h"
#include "TimeChecker.h"
#include "role_mgr.h"
#include "role.h"
#include "..\WorldDefine\msg_func_npc.h"

#define X_SINGLE_DIVIDE_SIGN ';' //接收消息的分隔符
#define X_RECEIVE_DIVIDE_SIGN "\r\n" //接收消息的分隔符

//-----------------------------------------------------------------------------
// 构造函数
//-----------------------------------------------------------------------------
KeyCodeBridgeSession g_keyCodeBridgeSession;

KeyCodeBridgeSession::KeyCodeBridgeSession() : m_Trunk(this), m_dwWorldID(0)
{
	m_dwPort	= GT_INVALID;
	m_bTermConnect	= FALSE;

	m_bInitOK = FALSE;
	m_dwOn = 0;

	InitializeCriticalSection(&m_ListLock); 
	InitializeCriticalSection(&m_RetListLock); 

	ZeroMemory(m_szIP, sizeof(m_szIP));
}

//-----------------------------------------------------------------------------
// 析构函数
//-----------------------------------------------------------------------------
KeyCodeBridgeSession::~KeyCodeBridgeSession()
{
}

//-----------------------------------------------------------------------------
// 初始化函数
//-----------------------------------------------------------------------------
BOOL KeyCodeBridgeSession::Init()
{
	// 该部分成员重新赋值是因为该类包含在一个全局变量中
	m_pUtil = "Util";
	m_pThread = "Thread";

	// 创建连接对象，并初始化
	CreateObj("ToKeyCodeBridge", "StreamTransport");
	m_pTran = "ToKeyCodeBridge";
	if(!P_VALID(m_pTran))
	{
		ERR(_T("Create KeyCodeBridge(StreamTransport) obj failed!"));
		return FALSE;
	}
	//m_pTran->Init(TRUE,ESTM_Stream);

	if( !m_pTran->Init(TRUE,ESTM_Stream) )
	{
		ERR(_T("KeyCodeBridge Init failed!"));
		return FALSE;
	}

	// 读取文件, 初始化成员
	if(!InitConfig())
	{
		ERR(_T("Init File Read Failed! Please Check......"));
		return FALSE;
	}

	// 启动连接线程
	if(!m_pThread->CreateThread(_T("ConnectKeyCodeBridge"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&KeyCodeBridgeSession::ThreadConnectKeyCodeBridge), NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ConnectKeyCodeBridge")))
	{
		continue;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// 读取文件, 初始化成员
//-----------------------------------------------------------------------------
BOOL KeyCodeBridgeSession::InitConfig()
{
	//CreateObj("LoongDBVar", "VarContainer");
	TObjRef<VarContainer> pVar = "VarContainer";
	if(!P_VALID(pVar))
	{
		ERR(_T("Create KeyCodeBridgeVar(VarContainer) obj failed!"));
		return FALSE;
	}

	// 获取数据库服务器的端口号
	m_dwPort	= pVar->GetDword(_T("port keycode_recharge_bridge_server"));
	m_dwOn = pVar->GetDword(_T("on keycode_recharge_bridge_server"));

	// 获取数据服务器IP地址
	TCHAR szIP[X_IP_LEN];
	_tcsncpy(szIP, pVar->GetString(_T("ip keycode_recharge_bridge_server")), MCalTCharArrayNum(szIP) - 1);
	IFASTCODE->MemCpy(m_szIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szIP) - 1);

	// 获取ServerID
	m_dwWorldID = pVar->GetDword(_T("id world"));

	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID KeyCodeBridgeSession::Destroy()
{
	// 等待所有线程结束
	InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
	m_pThread->WaitForThreadDestroy(_T("ConnectKeyCodeBridge"), INFINITE);

	m_pTran->Destroy();

	KillObj("ToKeyCodeBridge");

	EnterCriticalSection(&m_ListLock);

	// 删除所有list中的数据
	tagKeyCodeInfo* pData = m_listKeyCodeInfo.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listKeyCodeInfo.PopFront();
	}
	LeaveCriticalSection(&m_ListLock);

	EnterCriticalSection(&m_RetListLock);
	pData = m_listRetKeyCodeInfo.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listRetKeyCodeInfo.PopFront();
	}
	LeaveCriticalSection(&m_RetListLock);

	DeleteCriticalSection(&m_ListLock); 
	DeleteCriticalSection(&m_RetListLock); 
}

//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID KeyCodeBridgeSession::Update()
{
	if (m_dwOn == 0)
		return;

	CHECK_TIME();
	if( NULL == m_pTran )
		return;

	if(!m_pTran->IsConnected() && !m_pThread->IsThreadActive(_T("ConnectKeyCodeBridge")))
	{
		InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
		m_pTran->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ConnectKeyCodeBridge"), INFINITE);

		// 重新启动数据库服务器连接线程
		InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
		m_pThread->CreateThread(_T("ConnectKeyCodeBridge"), 
			(vEngine::THREADPROC)m_Trunk.sfp0(&KeyCodeBridgeSession::ThreadConnectKeyCodeBridge),
			NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ConnectKeyCodeBridge")))
		{
			continue;
		}

		return;
	}

	// 更新验证列表
	UpdateProofList();

	// 处理来自KeyCodeBridge服务器的消息
	DWORD	dwSize = 0;
	LPBYTE	pRecv = m_pTran->Recv(dwSize);
	while( P_VALID(pRecv) && dwSize > 0 && m_pTran->IsConnected())
	{	
  		HandleRecvResult(pRecv);

		//越南验证的返回信息
		m_pTran->FreeRecved(pRecv);

		pRecv = m_pTran->Recv(dwSize);
	}
}

//-----------------------------------------------------------------------------
// 连接线程(连接数据库服务器)
//-----------------------------------------------------------------------------
DWORD KeyCodeBridgeSession::ThreadConnectKeyCodeBridge()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	if (m_dwOn == 0)
		return 0;

	while(FALSE == m_bTermConnect)
	{
		if( !m_pTran->IsConnected() )
		{
			if( !m_pTran->IsTryingToConnect() )
			{
				m_pTran->TryToConnect(m_szIP, m_dwPort);
			}

			Sleep(100);
			continue;	// 重新检测连接
		}

		IMSG(_T("Contected to KeyCodeBridge Server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szIP), m_dwPort);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	return 0;
}

VOID KeyCodeBridgeSession::Proof(LPCSTR szAccount, DWORD dwRoleID, LPCSTR szKeyCode, DWORD dwClientIP)
{
	if (m_dwOn == 0)
		return;

	if(!P_VALID(szAccount) || !P_VALID(dwRoleID) ||  !P_VALID(szKeyCode) || !P_VALID(dwClientIP)) return;

	// 生成一个新的验证信息
	tagKeyCodeInfo* pData = new tagKeyCodeInfo(m_dwWorldID, szAccount, dwRoleID, szKeyCode, dwClientIP);

	EnterCriticalSection(&m_ListLock);
	m_listKeyCodeInfo.PushBack(pData);
	LeaveCriticalSection(&m_ListLock);
}

//-------------------------------------------------------------------------
// 更新验证列表
//-------------------------------------------------------------------------
VOID KeyCodeBridgeSession::UpdateProofList()
{
	if(!m_pTran->IsConnected())
		return;

#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	EnterCriticalSection(&m_ListLock);
	// 从列表中取出验证数据
	tagKeyCodeInfo* pData = m_listKeyCodeInfo.PopFront();
	LeaveCriticalSection(&m_ListLock);
	int iNum = 0;

	//一次处理50个
	while( P_VALID(pData) && iNum<50)
	{
		++iNum;

		//////////////////////////////////////////////////////////////////////////
		//以下数据都走ansi
		//拼验证数据
		stringstream strStream;
		strStream<<pData->dwGameID;
		strStream<<X_SINGLE_DIVIDE_SIGN;
		strStream<<pData->dwServerID; 
		strStream<<X_SINGLE_DIVIDE_SIGN;
		CHAR szIP[X_IP_LEN] = {0};
		strncpy(szIP, inet_ntoa((*(in_addr*)&pData->dwClientIP)), X_IP_LEN);
		strStream<<szIP;
		strStream<<X_SINGLE_DIVIDE_SIGN;
		strStream<<pData->strAccount;
		strStream<<X_SINGLE_DIVIDE_SIGN;
		strStream<<pData->strKeyCode;
		strStream<<"\r\n";
		//发送数据
		m_pTran->Send((void *)strStream.str().c_str(),strStream.str().length());

		EnterCriticalSection(&m_RetListLock);
		m_listRetKeyCodeInfo.PushBack(pData);
		LeaveCriticalSection(&m_RetListLock);

		EnterCriticalSection(&m_ListLock);
		pData = m_listKeyCodeInfo.PopFront();		// 取出下一个
		LeaveCriticalSection(&m_ListLock);
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
}

/* result的定义 
* 0-卡密错误  
* 1-卡已充值过   
* 2-卡有效期已过
* 3-卡类型不是NPC卡
* 4-道具礼包错误
* 5-区服输入有误
* 6-操作失败
* 7-非《圣魔之血》
* 8-非《圣魔之血》
* 9-礼包内无道具
* 10-充值成功
* 11-过程失败
* 12-卡类型已经使用过
* 13-帐号非会员中心
*/
VOID KeyCodeBridgeSession::HandleRecvResult(LPBYTE pRecv)
{
	// 直接转
	string strRecv = (CHAR*)pRecv;
	vector <tagRecvResult> vecOrangeData;

	// 解析数据
	if (ParseProofData(strRecv, vecOrangeData) == FALSE)
		return;

	INT nVectSize = vecOrangeData.size();
	if (nVectSize <= 0)
		return;

	// 给客户端返回结果
 	EnterCriticalSection(&m_RetListLock);
	for (INT nCnt = 0; nCnt < nVectSize; nCnt++)
	{
		TList<tagKeyCodeInfo*>::TListIterator iterator = m_listRetKeyCodeInfo.Begin();
		tagKeyCodeInfo* pData = NULL;

		while( m_listRetKeyCodeInfo.PeekNext(iterator, pData) )
		{
			if (!P_VALID(pData))
				continue;
			if (!pData->strKeyCode.compare((vecOrangeData[nCnt]).strKeyCode))
			{
				Role* pRole = g_roleMgr.GetRolePtrByID(pData->dwRoleID);
				if (P_VALID(pRole))
				{
					tagNS_KeyCode send;
					send.nErrorCode = (vecOrangeData[nCnt]).nResult;
					pRole->SendMessage(&send, send.dwSize);
				}
				m_listRetKeyCodeInfo.Erase(pData);
				SAFE_DEL(pData);
				break;
			}
		}
	}

 	LeaveCriticalSection(&m_RetListLock);
}

BOOL KeyCodeBridgeSession::ParseProofData(const string &strFrameString, vector<tagRecvResult> &vecOrangeData)
{
	int iprepos = 0;
	int icurpos = 0;
	for(;;)
	{
		string strTemp;

		icurpos = strFrameString.find(X_RECEIVE_DIVIDE_SIGN,iprepos);
		//最后一个
		if(icurpos == -1)
		{
			strTemp = strFrameString.substr(iprepos,strFrameString.length() - iprepos);

			//检查最后一个字符串
			CheckString(strTemp);

			tagRecvResult recvResult;
			ParseSingleData(strTemp, recvResult.strKeyCode, recvResult.nResult);

			vecOrangeData.push_back(recvResult);
			break;
		}
		strTemp = strFrameString.substr(iprepos,icurpos-iprepos);

		tagRecvResult recvResult;
		ParseSingleData(strTemp, recvResult.strKeyCode, recvResult.nResult);

		vecOrangeData.push_back(recvResult);
		iprepos = icurpos + 1;
	}
	return TRUE;
}

BOOL KeyCodeBridgeSession::ParseSingleData(const string &strFrameString, string &strKeyCode, INT &nResult)
{
	int iprepos = 0;
	int icurpos = 0;
	for(;;)
	{
		string strTemp;

		icurpos = strFrameString.find(X_SINGLE_DIVIDE_SIGN,iprepos);
		//最后一个
		if(icurpos == -1)
		{
			strTemp = strFrameString.substr(iprepos,strFrameString.length() - iprepos);

			//检查最后一个字符串
			CheckString(strTemp);
			nResult = atoi(strTemp.c_str());
			break;
		}
		strTemp = strFrameString.substr(iprepos,icurpos-iprepos);
		strKeyCode = strTemp;
		iprepos = icurpos + 1;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------------------------
// 去掉回车换行
//-----------------------------------------------------------------------------------------------
BOOL KeyCodeBridgeSession::CheckString(string& str)
{
	// 以回车换行结束
	INT nFind = 0;
	while( (nFind = str.find('\r', nFind)) != str.npos )
		str = str.substr(0,nFind);

	// 
	nFind = 0;
	while( (nFind = str.find('\n', nFind)) != str.npos )
		str = str.substr(0,nFind);

	// 检查用户名是否为空
	return str.empty();
}