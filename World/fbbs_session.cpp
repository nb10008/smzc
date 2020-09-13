#include "stdafx.h"
#include "fbbs_session.h"
#include "world.h"
#include "TimeChecker.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/mini_tool.h"
#include "role_mgr.h"
#include "../ServerDefine/role_data_define.h"
#include "guild_mgr.h"
#include "guild.h"
#include "role.h"

FBBSSession::FBBSSession()
:m_Trunk(this)
{
}
FBBSSession::~FBBSSession()
{
}
DWORD FBBSSession::ThreadConnectFBBS()
{
	INT nCounter = 100;
	while(FALSE == m_bTermConnect)
	{
		if( !m_pTran->IsConnected() )
		{
			if( !m_pTran->IsTryingToConnect() )
			{
				m_pTran->TryToConnect(m_szIP, m_dwPort);
			}

			if( --nCounter < 1 )
				break;

			Sleep(110);
			continue;	// 重新检测连接
		}

		IMSG(_T("Connected to fbbs at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szIP), m_dwPort);

		break;
	}
	if( nCounter < 1 )
		IMSG(_T("Connection has been lost to fbbs.\r\n"));
	return 0;
}

BOOL FBBSSession::Init()
{
	InterlockedExchange((LPLONG)&m_bInitOK ,FALSE);
	m_pUtil = "Util";
	m_pThread = "Thread";

	TObjRef<VarContainer> pVar = "VarContainer";
	if(!P_VALID(pVar))
	{
		ERR(_T("read fbbs info failed!"));
		return FALSE;
	}

	// 获取数据库服务器的端口号
	m_dwPort	= pVar->GetDword(_T("port facebook_bs"));
	m_dwOn		= pVar->GetDword(_T("open facebook_bs"));

	if( !m_dwOn )
	{
		return TRUE;
	}

	// 创建连接对象，并初始化
	CreateObj("FBBSObj", "StreamTransport");
	m_pTran = "FBBSObj";
	if(!P_VALID(m_pTran))
	{
		ERR(_T("Create FBBS(StreamTransport) obj failed!"));
		return FALSE;
	}
	m_pTran->Init(TRUE,ESTM_Stream);

	// 获取数据服务器IP地址
	TCHAR szIP[X_IP_LEN] = {0};
	_tcsncpy(szIP, pVar->GetString(_T("ip facebook_bs")), MCalTCharArrayNum(szIP) - 1);
	IFASTCODE->MemCpy(m_szIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szIP) - 1);

	// 获取ServerID
	m_dwWorldID = pVar->GetDword(_T("id world"));

	// 启动连接线程
	if(!m_pThread->CreateThread(_T("ConnectFBBS"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&FBBSSession::ThreadConnectFBBS), NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ConnectFBBS")))
	{
		continue;
	}

	InterlockedExchange((LPLONG)&m_bInitOK ,TRUE);
	return TRUE;
}

VOID FBBSSession::Destroy()
{
	if( m_dwOn )
	{
		// 等待所有线程结束
		InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
		m_pThread->WaitForThreadDestroy(_T("ConnectFBBS"), INFINITE);

		m_pTran->Destroy();

		KillObj("FBBSObj");
	}
}

VOID FBBSSession::Update()
{
	if (m_dwOn == 0)
		return;

	CHECK_TIME();
	if( NULL == m_pTran )
		return;

	if(!m_pTran->IsConnected() && !m_pThread->IsThreadActive(_T("ConnectFBBS")))
	{
		InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
		m_pTran->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ConnectFBBS"), INFINITE);

		// 重新启动FBBS服务器连接线程
		InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
		m_pThread->CreateThread(_T("ConnectFBBS"), 
			(vEngine::THREADPROC)m_Trunk.sfp0(&FBBSSession::ThreadConnectFBBS),
			NULL);

		int nWCounter = 0;
		while(FALSE == m_pThread->IsThreadActive(_T("ConnectFBBS")))
		{
			if( ++nWCounter > 10 )
				break;
			::Sleep(10);
		}

		return;
	}

	// 丢弃来自FBBS服务器的消息
	DWORD	dwSize = 0;
	LPBYTE	pRecv = NULL;
	while( m_pTran->IsConnected() )
	{	
		pRecv = m_pTran->Recv(dwSize);
		if( P_VALID(pRecv) )
			m_pTran->FreeRecved(pRecv);
		else
			break;
	}
}

VOID FBBSSession::Send(LPVOID pMsg,INT size)
{
	if (m_dwOn == 0)
		return;
	if( P_VALID(m_pTran) && m_pTran->IsConnected() )
	{
		m_pTran->Send(pMsg, size);
		return;
	}
	ILOG->Write(_T("fbbs send fail."));
}

VOID FBBSSession::NotifyFBBS(DWORD dwRoleID,INT nCmdID,INT nParam)
{
	if (m_dwOn == 0)
		return;
	char buf[4] = {0};
	sprintf(buf,"%03d",nCmdID);

	char strBuff[X_SHORT_NAME*3] = {0};
	TObjRef<Util> pUtil = "Util";
	if( !pUtil )
		return ;

	stringstream streamsend;
	streamsend << buf;
	streamsend << ";";

	LPCSTR strtem = NULL;
	INT num = 0;

	Role * pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if( P_VALID(pRole) )
	{
		PlayerSession * pSession = pRole->GetSession();
		if( P_VALID(pSession) )
		{
			strtem = pSession->GetAccount();
			num = min(_countof(strBuff)-1,strlen(strtem));
			strncpy_s(strBuff,strtem,num);
			strBuff[num] = 0;
			streamsend << strBuff;
			streamsend << ";";
		}
		else
			streamsend << "unknown;";
	}
	else
		streamsend << "unknown;";

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
	if( !P_VALID(pRoleInfo) )
		return;

	strtem = pUtil->UnicodeToUnicode8(pRoleInfo->szRoleName);
	num = min(_countof(strBuff)-1,strlen(strtem));
	strncpy_s(strBuff,strtem,num);
	strBuff[num] = 0;
	if( nCmdID > 999 )
		nCmdID /= 1000;

	streamsend << strBuff;
	streamsend << ";";
	switch( nCmdID )
	{
		// 忘记登入游戏世界
	case 1:
		{
			char strParam[20] = {0};
			tagDWORDTime now = g_world.GetWorldTime();
			sprintf(strParam,"%02d%02d%02d%02d%02d%02d",now.year,now.month,now.day,now.hour,now.min,now.sec);
			streamsend << strParam;
		}
		break;
		// 建立工会
	case 5:
		{
			Guild * pGuild = g_guildMgr.GetGuild(nParam);
			if( P_VALID(pGuild) )
			{
				char strParam[100] = {0};
				strtem = pUtil->UnicodeToUnicode8(pGuild->GetGuildAtt().strName.c_str());
				num = min(_countof(strParam)-1,strlen(strtem));
				strncpy_s(strParam,strtem,num);
				strParam[num] = 0;
				streamsend << strParam;
			}
		}
		break;
	default:
		streamsend << nParam;
	}
	streamsend << "\r\n";

	Send((LPVOID)streamsend.str().c_str(),streamsend.str().length());
}

FBBSSession g_FBBSSession;
