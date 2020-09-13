//-----------------------------------------------------------------------------
//!\file world_session.cpp
//!\author Aslan
//!
//!\date 2008-06-10
//! last 2008-06-10
//!
//!\brief 区域服务器客户端主框架类
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "world_session.h"
#include "login_session.h"
#include "player_session.h"
#include "world.h"
#include "game_guarder.h"
#include "TimeChecker.h"
#include "..\WorldDefine\selectrole.h"
#include "../WorldDefine/TreasureChest_define.h"
#include "../WorldDefine/msg_serverinfo.h"
#include "../ServerDefine/msg_common.h"
#include "../ServerDefine/msg_log.h"
#include "att_res.h"
// Jason 2010-1-27 v1.3.2 玩家回归/离线奖励相关
#include "player_account.h"
#include "activity_mgr.h"
#include "Role.h"
#include <fstream>


const INT MAX_RECV_WAIT_UNIT_NUM = 64;		// 客户端等待处理的包的最大数量，超过数量将被踢掉
const INT MAX_SEND_CAST_UNIT_NUM = 1000;	// 发送给客户端但还没有收到响应的包数量，超过数量将被踢掉
const INT MAX_SEND_CAST_SIZE = 2*1024*1024;	// 发送给客户端但还没有收到相应的字节数量，超过即被踢掉

//-------------------------------------------------------------------------------
// constructor
//-------------------------------------------------------------------------------
WorldSession::WorldSession(): m_Trunk(this), m_lfPlayerNumFactor(1.0) , m_bIsStartPrint(FALSE), m_bIsPrintInfo(FALSE)
{
	m_dwLastUpdateTime = GetCurrentDWORDTime();
	m_mapAccountLoginCount.Clear();
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
WorldSession::~WorldSession()
{

}

//-------------------------------------------------------------------------------
// 得到一个session（要注意指针得到后的安全性）
//-------------------------------------------------------------------------------
PlayerSession* WorldSession::FindSession(DWORD dwID)
{
	m_AllSessionMutex.Acquire();
	PlayerSession* pSession = m_mapAllSessions.Peek(dwID);
	m_AllSessionMutex.Release();

	return pSession;
}

//--------------------------------------------------------------------------------
// 得到一个选人界面的session（只能在主线程中调用）
//--------------------------------------------------------------------------------
PlayerSession* WorldSession::FindGlobalSession(DWORD dwID)
{
	m_GlobalSessionMutex.Acquire();
	PlayerSession* pSession = m_mapGlobalSessions.Peek(dwID);
	m_GlobalSessionMutex.Release();

	return pSession;
}

//--------------------------------------------------------------------------------
// 检测一个sessionID是否存在
//--------------------------------------------------------------------------------
BOOL WorldSession::IsSessionExist(DWORD dwID)
{
	m_AllSessionMutex.Acquire();
	BOOL bExist = m_mapAllSessions.IsExist(dwID);
	m_AllSessionMutex.Release();

	return bExist;
}

//--------------------------------------------------------------------------------
// 加入一个新的session
//-------------------------------------------------------------------------------
VOID WorldSession::AddSession(PlayerSession* pSession)
{ 
	if( P_VALID(pSession) )
	{
		m_AllSessionMutex.Acquire();
		m_mapAllSessions.Add(pSession->GetSessionID(), pSession);
		m_AllSessionMutex.Release();
	}
}
//-------------------------------------------------------------------------------
// 删除一个session
//-------------------------------------------------------------------------------
VOID WorldSession::RemoveSession(UINT32 dwID)
{
	m_AllSessionMutex.Acquire();
	m_mapAllSessions.Erase(dwID);
	m_AllSessionMutex.Release();
}

//-------------------------------------------------------------------------------
// 踢掉一个连接
//-------------------------------------------------------------------------------
VOID WorldSession::Kick(DWORD dwInternalIndex)
{
	m_pNetSession->Kick(dwInternalIndex);
}
//-------------------------------------------------------------------------------
// 向选人界面session中加入一个session
//-------------------------------------------------------------------------------
VOID WorldSession::AddGlobalSession(PlayerSession* pSession)
{
	if( P_VALID(pSession) )
	{
		if( m_GlobalSessionMutex.TryAcquire() )
		{
			m_mapGlobalSessions.Add(pSession->GetSessionID(), pSession);
			m_GlobalSessionMutex.Release();
		}
		else
		{
			m_InsertPoolMutex.Acquire();
			m_listInsertPool.PushBack(pSession);
			m_InsertPoolMutex.Release();
		}
	}
}

//-------------------------------------------------------------------------------
// 从选人界面session列表中删除一个session
//-------------------------------------------------------------------------------
VOID WorldSession::RemoveGlobalSession(DWORD dwSessionID)
{
	m_GlobalSessionMutex.Acquire();
	m_mapGlobalSessions.Erase(dwSessionID);
	m_GlobalSessionMutex.Release();
}

//-------------------------------------------------------------------------------
// 初始化
//-------------------------------------------------------------------------------
BOOL WorldSession::Init()
{
	// vEngine对象
	m_pUtil = "Util";
	m_pNetSession = "XServer";

	// 加载配置文件
	TObjRef<VarContainer> pVar = "VarContainer";

	m_nPlayerNumLimit = (INT)pVar->GetDword(_T("player_num_limit"), _T("world"));
	m_nPort = (INT)pVar->GetDword(_T("port"), _T("server"));

	m_nMaxRecvWaitUnitNum	= (INT)pVar->GetDword(_T("max_recv_wait_unit_num"), _T("house_keeping"), 64);		// 客户端等待处理的包的最大数量，超过数量将被踢掉
	m_nMaxSendCastUnitNum	= (INT)pVar->GetDword(_T("max_send_cast_unit_num"), _T("house_keeping"), 1000);		// 发送给客户端但还没有收到响应的包数量，超过数量将被踢掉
	m_nMaxSendCastSize		= (INT)pVar->GetDword(_T("max_send_cast_size"), _T("house_keeping"), 2097152);		// 发送给客户端但还没有收到相应的字节数量，超过即被踢掉


	m_bEnableExpressPay = (BOOL)pVar->GetDword(_T("enableexpresspay worldinfo"));
	if(m_bEnableExpressPay)
		_tcscpy(m_szExpressPayUrl, pVar->GetString(_T("expresspayurl worldinfo")));
	
	// 今日热门活动URL
	_tcscpy(m_szTodayHotActivityUrl, pVar->GetString(_T("today_hot_activity_url worldinfo")));
	// 战斗挂机战斗施放技能和使用物品时间间隔
	m_dwAutoFightTimeInterval = pVar->GetDword(_T("time_interval auto_fight"));
	m_bGuildWarOpen = pVar->GetDword(_T("open guild_war"));

	m_dwLoginMode = pVar->GetDword(_T("loginmode worldinfo"));

	m_dwMediaOnlineRecordInterval = pVar->GetDword(_T("record_interval media_online_num"));
	m_bDBQBOpenState[0] = pVar->GetDword(_T("Sunday duo_bao_qi_bing"));
	m_bDBQBOpenState[1] = pVar->GetDword(_T("Monday duo_bao_qi_bing"));
	m_bDBQBOpenState[2] = pVar->GetDword(_T("Tuesday duo_bao_qi_bing"));
	m_bDBQBOpenState[3] = pVar->GetDword(_T("Wednesday duo_bao_qi_bing"));
	m_bDBQBOpenState[4] = pVar->GetDword(_T("Thursday duo_bao_qi_bing"));
	m_bDBQBOpenState[5] = pVar->GetDword(_T("Friday duo_bao_qi_bing"));
	m_bDBQBOpenState[6] = pVar->GetDword(_T("Saturday duo_bao_qi_bing"));
	m_dwWardrobeOn = pVar->GetDword(_T("on wardrobe"));
	// Kick player Log开启标志
	m_dwKickPlayerLogOn = pVar->GetDword(_T("on KickPlayerLog"));
	// Kick role记Log库开启标志，1开启，0关闭
	m_dwKillRoleLogOn = pVar->GetDword(_T("on KillRoleLog"));

	// 设定网络底层
	tagXServerConfig InitParam;
	InitParam.fnLogIn		=	(XLOGINCALLBACK)m_Trunk.sfp2(&WorldSession::LoginCallBack);
	InitParam.fnLogOut		=	(XLOGOUTCALLBACK)m_Trunk.sfp1(&WorldSession::LogoutCallBack);
	InitParam.bReusePort	=	true;
	InitParam.nPort			=	m_nPort;

	m_pNetSession->Init(InitParam);
	m_nPort = m_pNetSession->GetConfig()->nPort;
	m_nTreasureSum = 0;

#ifdef ON_GAMEGUARD
	m_dwLastUpdateCStime = timeGetTime();
#endif

	// 注册所有客户端命令
	PlayerSession::RegisterAllPlayerCmd();
	// 注册所有服务器端发送的命令
	PlayerSession::RegisterALLSendCmd();

	return TRUE;
}

//-------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------
VOID WorldSession::Update()
{
	CHECK_TIME()
	InterlockedExchange((LPLONG)&m_nMsgSendThisTick, 0);
	InterlockedExchange((LPLONG)&m_nMsgProceedThisTick, 0);
	InterlockedExchange((LPLONG)&m_nMsgRecvWait, 0);

	// 从InsertPool中拿出要加入的session
	m_InsertPoolMutex.Acquire();
	PlayerSession* pSession = m_listInsertPool.PopFront();
	while( P_VALID(pSession) )
	{
		AddGlobalSession(pSession);
		pSession = m_listInsertPool.PopFront();
	}
	m_InsertPoolMutex.Release();

	// 更新选人界面的session
	UpdateSession();

	// 更新各个媒体的在线人数统计结果
	UpdateMediaOnlineNum();

	if(g_world.IsPrintRoleInfoOpen())
	{
		// 打印当前在线人角色id和账号
		tagDWORDTime now = GetCurrentDWORDTime();

		if ( !m_bIsStartPrint && now.hour == 21 && now.min == 0  && now.sec == 0)
		{
			m_bIsStartPrint = FALSE;	//9点时解锁
		}
		// 每天晚上9：20打印
		if ( !m_bIsStartPrint && now.hour == 21 && now.min == 20  && now.sec == 0)
		{
			m_bIsPrintInfo = TRUE;
			m_mapAllSessions.ResetIterator();
			m_bIsStartPrint = TRUE;
		}

		if(m_bIsPrintInfo)
		{
			PrintRoleInfo();
		}

	}

	// 如果是韩国版本，定期生成认证值并发送给客户端
#ifdef ON_GAMEGUARD
	// 每隔3分钟生成CS认证值并发送给客户端
	if( timeGetTime() - m_dwLastUpdateCStime >= 3*60*1000 )
	{ 
		m_dwLastUpdateCStime = timeGetTime();
		CreateSendCS();
	}	
#endif
}

//-------------------------------------------------------------------------------
// 更新各个客户端session
//-------------------------------------------------------------------------------
VOID WorldSession::UpdateSession()
{
	// 更新所有选人界面的session
	m_GlobalSessionMutex.Acquire();

	PlayerSession* pSession = NULL;
	m_mapGlobalSessions.ResetIterator();

	while( m_mapGlobalSessions.PeekNext(pSession) )
	{
		if( CON_LOST == pSession->Update())
		{
			// IMSG(_T("player logout in worldsession, sessionid=%u, internalid=%u\r\n"), pSession->GetSessionID(), pSession->GetInternalIndex());

			RemoveSession(pSession->GetSessionID());
			m_mapGlobalSessions.Erase(pSession->GetSessionID());
			g_loginSession.SendPlayerLogout(pSession->GetSessionID());

			PlayerLogout();

			g_gameGuarder.Logout(pSession->GetSessionID(), pSession->GetAccount());
			SAFE_DEL(pSession);
		}
	}

	m_GlobalSessionMutex.Release();
}

#ifdef ON_GAMEGUARD
//-------------------------------------------------------------------------------
// 韩国版本对于每个客户端，生成CS认证值并发送Client
//-------------------------------------------------------------------------------
VOID WorldSession::CreateSendCS()
{
	g_world.LockNetWork();

	// 更新所有选人界面的session
	m_AllSessionMutex.Acquire();

	PlayerSession* pSession = NULL;
	UINT32 uRetPackSize = 0;
	m_mapAllSessions.ResetIterator();

	while( m_mapAllSessions.PeekNext(pSession) )
	{
		UINT32 uRet = pSession->m_CS.Get( &uRetPackSize );

		if( uRet >= 3000 )	
		{		
			pSession->m_CS.Close();

			// 断开玩家连接
			ILOG->Write(_T("产生CS认证包失败,错误码:%u,Kick Player: %u\r\n"), uRet, pSession->GetSessionID());
			Kick(pSession->GetInternalIndex());
			pSession->SetKicked();
			continue;
		}

		// 发送CS认证值
		DWORD dwMsgSize = sizeof(tagNS_KoreaCSApprove) + uRetPackSize;
		BYTE* p = new BYTE[dwMsgSize];
		memset(p,0,dwMsgSize);
		tagNS_KoreaCSApprove* pSend = (tagNS_KoreaCSApprove*)p;
		pSend->dwID        = pSend->Crc32("NS_KoreaCSApprove");
		pSend->dwSize      = dwMsgSize;

		memcpy(pSend->byData, pSession->m_CS.packet, uRetPackSize);
		pSession->SendMessage(pSend, pSend->dwSize);
		SAFE_DEL_ARRAY(p);
		// test
		//ILOG->Write(_T("发送CS认证包 Player: %u\r\n"), pSession->GetSessionID());
	}

	m_AllSessionMutex.Release();

	g_world.UnlockNetWork();
}
#endif

//-------------------------------------------------------------------------------
// 做一些网络层的清理工作
//-------------------------------------------------------------------------------
VOID WorldSession::DoHouseKeeping()
{
	CHECK_TIME();

	// 启动底层网络层开始发送
	m_pNetSession->ActiveSend();

	g_world.LockNetWork();

	// 轮询所有session
	m_AllSessionMutex.Acquire();

	PlayerSession* pSession = NULL;
	TMap<DWORD, PlayerSession*>::TMapIterator it = m_mapAllSessions.Begin();

	INT nAllSendCast = 0;
	INT nMaxChokeSize = 0;
	DWORD dwMaxChokeSessionID = GT_INVALID;	
	
	
	while( m_mapAllSessions.PeekNext(it, pSession) )
	{
		// 检查客户端接受队列的消息数量
		if( pSession->GetMsgWaitNum() >= GetMaxRecvWaitUnitNum() && !pSession->IsKicked() )
		{
			IMSG(_T("Kick Too Fast Player[%u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetMsgWaitNum(),g_world.GetCurTick());
			ITRACE(_T("Kick Too Fast Player[%u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetMsgWaitNum(),g_world.GetCurTick());
			ILOG->Write(_T("Kick Too Fast Player[%u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetMsgWaitNum(),g_world.GetCurTick());
			
			// 查踢外网玩家，加Log
			if (m_dwKickPlayerLogOn == 1)
			{
				DWORD dwSize = 0;
				int nMsgNum = 0;	
				static TCHAR tempLog[2*1024] = {0};

				bool isLog = false;
				memset(tempLog, 0, sizeof(tempLog));
				LPBYTE pMsg = m_pNetSession->Recv(pSession->GetInternalIndex(), dwSize, nMsgNum);
				while (pMsg != NULL)
				{
					tagNetCmd* pCmd = (tagNetCmd*)pMsg;				
					_stprintf(tempLog, _T("%s  %u"), tempLog, pCmd->dwID);
					isLog =true;
					pMsg = m_pNetSession->Recv(pSession->GetInternalIndex(), dwSize, nMsgNum);
					if (!pMsg)
					{
						break;
					}
				}

				// 查踢外网玩家，加Log	
				if (isLog)
				{
					ILOG->Write(_T("kick client log:%s\r\n"),tempLog);
				}	
			}			

			Kick(pSession->GetInternalIndex());
			pSession->SetKicked();
			continue;
		}

		// 检查该客户端未收到确认的包的数量
		INT nCastSize = GetSendCastSize(pSession->GetInternalIndex());
		nAllSendCast += nCastSize;

		if( nCastSize >= GetMaxSendCastSize() && !pSession->IsKicked() )
		{
			IMSG(_T("Kick Choke Player[%u, %u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetInternalIndex(), nCastSize,g_world.GetCurTick());
			ITRACE(_T("Kick Choke Player[%u, %u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetInternalIndex(), nCastSize,g_world.GetCurTick());
			ILOG->Write(_T("Kick Choke Player[%u, %d] cur_tick%u\r\n"), pSession->GetSessionID(), pSession->GetMsgWaitNum(),g_world.GetCurTick());
			Kick(pSession->GetInternalIndex());
			pSession->SetKicked();
		}
		else if( nCastSize > nMaxChokeSize )
		{
			nMaxChokeSize = nCastSize;
			dwMaxChokeSessionID = pSession->GetSessionID();
		}
	}

	// todo: 如果出现内存不足等情况，则踢掉最卡的玩家

	// 设置本Tick的SendCast量
	SetMsgSendCast(nAllSendCast);

	m_AllSessionMutex.Release();

	g_world.UnlockNetWork();
}

//-------------------------------------------------------------------------------
// 销毁
//-------------------------------------------------------------------------------
VOID WorldSession::Destroy()
{
	// 删除所有的session（因为所有线程已经停止，所以这里不需要锁定）
	PlayerSession* pSession = m_listInsertPool.PopFront();
	while( P_VALID(pSession) )
	{
		g_gameGuarder.Logout(pSession->GetSessionID(), pSession->GetAccount());
		SAFE_DEL(pSession);
		pSession = m_listInsertPool.PopFront();
	}

	m_mapGlobalSessions.Clear();
	
	m_mapAllSessions.ResetIterator();
	while( m_mapAllSessions.PeekNext(pSession) )
	{
		g_gameGuarder.Logout(pSession->GetSessionID(), pSession->GetAccount());
		SAFE_DEL(pSession);
	}
	m_mapAllSessions.Clear();

	m_pNetSession->Destroy();

	// 撤销所有注册的网络命令和GM命令
	PlayerSession::UnRegisterALL();
}

//-------------------------------------------------------------------------------
// 登陆回调函数
//-------------------------------------------------------------------------------
DWORD WorldSession::LoginCallBack(LPVOID p1, LPVOID p2)
{
	static DWORD dwJoinGameCmdCrc = m_pUtil->Crc32("NC_JoinGame");

	if( g_world.IsShutingdown() )
	{
		ILOG->Write(_T("Kick,world is shuting down!!!\r\n"));
		return GT_INVALID;
	}

	// 查看第一条消息
	tagUnit* pUnit = (tagUnit*)p1;
	tagXLoginParam* pParam = (tagXLoginParam*)p2;
	
	tagNC_JoinGame* pCmd = (tagNC_JoinGame*)pUnit->pBuffer;

	if( pCmd->dwID != dwJoinGameCmdCrc )
	{
		ILOG->Write(_T("Kick,pCmd->dwID != dwJoinGameCmdCrc Error\r\n"));
		return GT_INVALID;
	}

	if( g_world.IsWillShutdown() )
	{
		tagNS_JoinGame msg_back;
		msg_back.dwErrorCode = E_JoinGame_WorldWillShutdown;
		SendMsg(pParam->dwHandle, (LPBYTE)&msg_back, msg_back.dwSize);
		m_pNetSession->DisconnectFriendly(pParam->dwHandle);
		return 0;
	}

	if( !P_VALID(pParam->dwHandle) )
	{
		ILOG->Write(_T("Kick,vEngine Client Hanlder Error\r\n"));
		return GT_INVALID;
	}

	// 锁住网络
	g_world.LockNetWork();

	// 登陆服务器判断帐号是否合法
	BYTE byPrivilege = 0;
	BOOL bGuard = true;
	DWORD dwAccOLSec = 0;
	char szAccount[X_SHORT_NAME] = {0};
	char szKrUserID[X_SHORT_NAME] = {0};

	tagDWORDTime dwPreLoginTime;
	DWORD	dwPreLoginIP = 0;
	DWORD   dwChannelID = 0;
	if( FALSE == g_loginSession.IsAccountValid(pCmd->dwAccountID, pCmd->dwVerifyCode, byPrivilege, bGuard, 
												dwAccOLSec, szAccount,szKrUserID,dwPreLoginTime,dwPreLoginIP,dwChannelID) )
	{
		ILOG->Write(_T("Kick, Account is inValid,accountid=%d\r\n"),pCmd->dwAccountID);
		g_world.UnlockNetWork();
		return GT_INVALID;
	}

	if( IsSessionExist(pCmd->dwAccountID) )
	{
		ILOG->Write(_T("Kick, Session Already Exist!!!,accountid=%d\r\n"),pCmd->dwAccountID);
		g_world.UnlockNetWork();
		return GT_INVALID;
	}
    AddAccountLoginCount(pCmd->dwAccountID,dwPreLoginTime);
	// 加入一个新的session
	PlayerSession* pSession = new PlayerSession(pCmd->dwAccountID, pParam->dwHandle, pParam->dwAddress, byPrivilege,
												bGuard, dwAccOLSec, szAccount,szKrUserID,dwPreLoginTime,dwPreLoginIP,dwChannelID, pCmd->szMac);
#ifdef ON_GAMEGUARD
	// 如果是韩国版本，初始化每个Player的CCSAuth3成员对象
	pSession->m_CS.Init(true);
#endif
    
	// 分别加入到总session和globlesession中
	AddSession(pSession);
	AddGlobalSession(pSession);

	// 设置登陆
	PlayerLogin();

	// 通知登陆服务器帐号登陆成功
	g_loginSession.SendPlayerLogin(pCmd->dwAccountID, pParam->dwAddress, E_Success, pCmd->szMac);

	// 反外挂记录
	g_gameGuarder.Login(pCmd->dwAccountID, szAccount, pParam->dwAddress);

	// 解锁
	g_world.UnlockNetWork();

	// 发送服务器信息
	tagNS_ServerInfo send;
	_tcscpy(send.szServer, g_world.GetWorldName());
	send.dwLoginMode = m_dwLoginMode;
	send.bEnableExpressPay = m_bEnableExpressPay;
	send.bEnableSpiritSys = g_attRes.IsSpiritSysOn();
	send.bEnableCitySys = g_activityMgr.GetCityStruggleState() == 1;
	if(m_bEnableExpressPay)
		_tcscpy(send.szExpressPayUrl, m_szExpressPayUrl);

	// 今日热门活动URL
	_tcscpy(send.szTodayHotActivityUrl, m_szTodayHotActivityUrl);
	// 战斗挂机战斗施放技能和使用物品时间间隔
	send.dwAutoFightTimeInterval = m_dwAutoFightTimeInterval;
	
	// 公会战开关
	send.bGuildWarOpen = m_bGuildWarOpen;

	send.dwServerFunctionSwitch = g_attRes.GetServerFunctionSwitch();

	pSession->SendMessage((LPBYTE)&send, send.dwSize);

	// Jason 2010-1-27 v1.3.2 玩家回归/离线奖励相关
	g_PlayerAccountMgr.RoleLogin(pCmd->dwAccountID);

	return pSession->GetSessionID();
}

//--------------------------------------------------------------------------------------
// 登出回调函数
//--------------------------------------------------------------------------------------
DWORD WorldSession::LogoutCallBack(LPVOID p)
{
	DWORD dwSessionID = (DWORD)p;

	ITRACE(_T("Log out callback, sessionid=%u\r\n"), dwSessionID);

	if( g_world.IsShutingdown() )
		return GT_INVALID;

	// 锁住网络
	g_world.LockNetWork();

	PlayerSession* pSession = FindSession(dwSessionID);

	if( P_VALID(pSession) )
	{ 
        SetAccountLoginCount(pSession->GetSessionID(),pSession->GetCreatedRoleNumLimit());    
		pSession->SessionLogout();
		// Jason 2010-1-27 v1.3.2 玩家回归/离线奖励相关
		g_PlayerAccountMgr.RoleLogout  (pSession->GetSessionID  ());
		Role *pRole = pSession->GetRole();
		if(P_VALID(pRole))
		{
			ILOG->Write(_T("Logout client, roleid is:%u\r\n"),pRole->GetID());
		}
	}

	// 解锁网络
	g_world.UnlockNetWork();

	return 0;
}

//--------------------------------------------------------------------------------------
// 服务器宝箱开启计数加一
//--------------------------------------------------------------------------------------
VOID WorldSession::IncTreasureSum()
{
	InterlockedExchangeAdd((LPLONG)&m_nTreasureSum, 1);
	if (m_nTreasureSum > SERVER_CHEST_RATE)
		InterlockedExchange((LPLONG)&m_nTreasureSum, 1);
}

//--------------------------------------------------------------------------------------
// 设置服务器宝箱开启计数
//--------------------------------------------------------------------------------------
VOID WorldSession::SetTreasureSum(INT nSum)
{
	if (nSum > SERVER_CHEST_RATE)
		nSum = 1;
	InterlockedExchange((LPLONG)&m_nTreasureSum, nSum);
}

//--------------------------------------------------------------------------------------
// 获取所有在线玩家AccountId
//--------------------------------------------------------------------------------------
INT WorldSession::GetAllOLAccountID( DWORD* pAccountIDs )
{
	std::list<PlayerSession*> listPlayerSessions;
	m_AllSessionMutex.Acquire();

	m_mapAllSessions.ExportAllValue(listPlayerSessions);

	INT i=0;
	std::list<PlayerSession*>::iterator itr = listPlayerSessions.begin();
	while (itr != listPlayerSessions.end())
	{
		if (P_VALID(*itr) && (*itr)->GetFatigueGuarder().IsGuard())
		{
			pAccountIDs[i++] = (*itr)->GetSessionID();
		}
		++itr;
	}

	m_AllSessionMutex.Release();

	return i;
}

VOID WorldSession::AddAccountLoginCount(DWORD dwAccountId,const tagDWORDTime& dwPreLoginTime)
{    

	  
	  INT result = m_mapAccountLoginCount.Peek(dwAccountId);
	  if ( GT_VALID(result))
	  {
		  tagDWORDTime dwCurrentDWORDTime = GetCurrentDWORDTime();
		 // DWORD dwTimeRange  =   CalcTimeDiff(dwCurrentDWORDTime,dwPreLoginTime);

		  if( dwCurrentDWORDTime.day != dwPreLoginTime.day) //如果超过一天
		  {
			 
			  TObjRef<VarContainer> pVar = "VarContainer";
			  int  nLoginCount = pVar->GetDword(_T("max_role_num create_role"));
              SetAccountLoginCount(dwAccountId,nLoginCount); 
			 
		  }
		  return;
	  }

	   TObjRef<VarContainer> pVar = "VarContainer";
	   int  nLoginCount = pVar->GetDword(_T("max_role_num create_role"));
	   m_mapAccountLoginCount.Add(dwAccountId,nLoginCount);  

	  
}

INT  WorldSession::GetAccountLoginCount(DWORD dwAccountId)
{
	
      return m_mapAccountLoginCount.Peek(dwAccountId);  
	 
}

VOID   WorldSession::SetAccountLoginCount(DWORD dwAccountId,INT  nLoginCount)
{
	  
      m_mapAccountLoginCount.ChangeValue(dwAccountId,nLoginCount);
	  
}

VOID WorldSession::UpdateMediaOnlineNum()
{
	tagDWORDTime dwCurTime = GetCurrentDWORDTime();
	if (CalcTimeDiff(dwCurTime, m_dwLastUpdateTime) > GetMediaOnlineRecordInterval()*60)
	{
		m_dwLastUpdateTime = dwCurTime;
		SaveOnlineNum2DB();
	}
}

VOID WorldSession::SaveOnlineNum2DB()
{
	m_mapMediaOnlineNumMutex.Acquire();

	DWORD dwDataNum = m_mapMediaOnlineNum.Size();
	if (dwDataNum == 0)
	{
		m_mapMediaOnlineNumMutex.Release();
		return;
	}

	DWORD dwMsgSize = sizeof(tagNDBC_SaveMediaOnlineNum)+(dwDataNum-1)*sizeof(tagMediaOnlineNum);
	MCREATE_MSG(pSend, dwMsgSize, NDBC_SaveMediaOnlineNum);
	if (!P_VALID(pSend))
	{
		m_mapMediaOnlineNumMutex.Release();
		return;
	}

	pSend->dwNum = dwDataNum;

	TMap<INT, DWORD>::TMapIterator iterator = m_mapMediaOnlineNum.Begin();
	DWORD dwOnlineNum = 0;
	INT nDistributionID = iterator->first;
	DWORD dwIdx = 0;
	while(m_mapMediaOnlineNum.PeekNext(iterator, dwOnlineNum))
	{
		pSend->pData[dwIdx].dwOnlineNum = dwOnlineNum;
		pSend->pData[dwIdx].nDistributionID = nDistributionID;
		if (iterator != m_mapMediaOnlineNum.End())
			nDistributionID = iterator->first;
		dwIdx++;
	}
	g_dbSession.Send(pSend, pSend->dwSize);
	MDEL_MSG(pSend);
	m_mapMediaOnlineNumMutex.Release();
}

VOID WorldSession::OnRoleLogin(INT nDistributionID)
{
	m_mapMediaOnlineNumMutex.Acquire();
	DWORD dwOnlineNum = m_mapMediaOnlineNum.Peek(nDistributionID);
	if (dwOnlineNum != GT_INVALID)
		m_mapMediaOnlineNum.ChangeValue(nDistributionID, ++dwOnlineNum);
	else
		m_mapMediaOnlineNum.Add(nDistributionID, 1);
	m_mapMediaOnlineNumMutex.Release();
}

VOID WorldSession::OnRoleLogout(INT nDistributionID)
{
	m_mapMediaOnlineNumMutex.Acquire();
	DWORD dwOnlineNum = m_mapMediaOnlineNum.Peek(nDistributionID);
	if (dwOnlineNum != GT_INVALID && dwOnlineNum > 0)
		m_mapMediaOnlineNum.ChangeValue(nDistributionID, --dwOnlineNum);
	m_mapMediaOnlineNumMutex.Release();
}

BOOL WorldSession::GetDBQBOpenState(DWORD dwWeekDay)
{
	if (dwWeekDay >= 0 && dwWeekDay < DAYS_ONE_WEEK)
	{
		return m_bDBQBOpenState[dwWeekDay];
	}
	return FALSE;
}
VOID WorldSession::SendMsg(DWORD dwInternalIndex, LPBYTE pMsg, DWORD dwSize)
{
	InterlockedExchangeAdd((LPLONG)&m_nMsgSendThisTick, 1);
	m_pNetSession->Send(dwInternalIndex, pMsg, dwSize);
	g_world.CountNetMsg( pMsg, dwSize );
}
VOID WorldSession::PrintRoleInfo()
{
	PlayerSession* temp;
	ofstream wFileStream;
	CHAR sFileName[128] = {0};
	tagDWORDTime now = GetCurrentDWORDTime();
	_snprintf(sFileName, sizeof(sFileName)/sizeof(CHAR),  "log\\RoleInfo_20%d-%d-%d-%d-%d-%d.txt", now.year, now.month, now.day, now.hour, now.min, now.sec);
	wFileStream.open(sFileName,ios::out|ios::app/*|ios::trunc*/);
	for(int i = 0; i < 100; i++)
	{
		if(m_mapAllSessions.PeekNext(temp))
		{
			if (wFileStream)
			{
				Role* pRole = temp->GetRole();
				if(P_VALID(pRole))
					wFileStream << "Account:" <<temp->GetAccount() << "  RoleId:" << pRole->GetID()<< "  Level:" << pRole->GetLevel() << "  Class:" << pRole->GetClass() << "  MapCrc:"<< pRole->GetMapID() <<endl;
			}
		}
		else
		{
			m_bIsPrintInfo = FALSE;
			break;
		}
	}
	wFileStream.close();
}
WorldSession g_worldSession;