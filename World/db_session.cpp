//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: db_session.cpp
// author: Sxg
// actor:
// data: 2008-5-19
// last:
// brief: 游戏服务器与数据库服务器的对话层
// todo: 消息处理函数在这里消息不安全，
//-----------------------------------------------------------------------------
#include "Stdafx.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_yuanbao.h"
#include "../ServerDefine/msg_famehall.h"
#include "../ServerDefine/msg_guild.h"
#include "../WorldDefine/selectrole.h"
#include "../WorldDefine/msg_mall.h"
#include "../ServerDefine/msg_pet.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/msg_vcard.h"
#include "../ServerDefine/msg_vipstall.h"
#include "../ServerDefine/msg_vip_netbar.h"
#include "../ServerDefine/msg_rankings.h"
#include "../ServerDefine/msg_city.h"
#include "../ServerDefine/msg_shortcut_quickbar.h"
#include "../ServerDefine/msg_miracle.h"
#include "../ServerDefine/athletics_data_define.h"
#include "../ServerDefine/msg_draft_show.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "../WorldDefine/time.h"
#include "../ServerDefine/msg_godbid.h"
#include "../WorldDefine/godbid_define.h"
#include "../ServerDefine/msg_log.h"
#include "godbid_mgr.h"

// 包含头文件
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/msg_social.h"

#include "../WorldDefine/msg_pet.h"
#include "../WorldDefine/msg_role_vcard.h"
#include "../ServerDefine/activity_define.h"
#include "WorldRankings.h"
#include "db_session.h"
#include "world.h"
#include "world_session.h"
#include "role.h"
#include "role_mgr.h"
#include "att_res.h"
#include "item_creator.h"
#include "TradeYuanBao.h"
#include "famehall.h"
#include "guild.h"
#include "guild_mgr.h"
#include "pet_pocket.h"
#include "pet_soul.h"
#include "mall.h"
#include "vip_stall.h"
#include "activity_mgr.h"
#include "vip_netbar.h"
#include "chat_mgr.h"
#include "city_mgr.h"
#include "city.h"
#include "GuildBattle.h"
#include "athletics_PvP_define.h"
#include "athletics_room.h"
#include "athletics_pvp.h"
#include "athletics_room_mgr.h"
#include "athletics_pvp_mgr.h"
#include "athletics_system.h"
#include "draft_show.h"
#include "ClergyRank.h"
#include "global_limited_item.h"

// Jason v1.3.1 external links
#include "../ServerDefine/msg_sexternal_links.h"

#include "../ServerDefine/master_apprentice.h"
#include "force_breakout_apprentice_mgr.h"
#include "external_links.h"
// Jason 2010-1-27 v1.3.2 玩家回归/离线奖励相关
#include "player_account.h"
#include "player_regression.h"
// Jason 2010-2-20 v1.3.2申请加入帮派相关
#include "guild_apply_to_join.h"
#include "pet_define.h"

#include "guild_war_declare_mgr.h"
#include "../ServerDefine/msg_guild_war_declare.h"
#include "social_mgr.h"
#include "../WorldDefine/msg_god.h"
#include "../ServerDefine/msg_god.h"
#include "family_sprite_mgr.h"
#include "../ServerDefine/msg_family_sprite.h"
#include "../ServerDefine/msg_family.h"
#include "family.h"
#include "family_join.h"
#include "family_mgr.h"
#include "../WorldDefine/msg_func_npc.h"
#include "../ServerDefine/msg_account_reactive.h"
#include "TigerMgr.h"

//-----------------------------------------------------------------------------
// 构造函数
//-----------------------------------------------------------------------------
DBSession g_dbSession;

DBSession::DBSession() : m_Trunk(this),m_dwInitTime(0),
m_timLastPullExtLink(0),m_dwWaitConnectionTick(0),m_dwTransDBReconnectTicks(0),m_lTransCanUse(0)
{
	m_dwDBPort	= GT_INVALID;
	m_dwGoldenCode	= GT_INVALID;
	m_bTermConnect	= FALSE;

	m_bInitOK = FALSE;

	ZeroMemory(m_szDBIP, sizeof(m_szDBIP));
	//ZeroMemory(m_szServerName, sizeof(m_szServerName));
}

//-----------------------------------------------------------------------------
// 析构函数
//-----------------------------------------------------------------------------
DBSession::~DBSession()
{
}

//-----------------------------------------------------------------------------
// 初始化函数
//-----------------------------------------------------------------------------
BOOL DBSession::Init()
{
	// 该部分成员重新赋值是因为该类包含在一个全局变量中
	m_pUtil = "Util";
	m_pThread = "Thread";

	// 创建NetCmdMgr
	CreateObj("LoongDBNetCmdMgr", "NetCmdMgr");
	m_pMsgCmdMgr = "LoongDBNetCmdMgr";

	// 创建连接对象，并初始化
	CreateObj("ToLoongDB", "StreamTransport");
	m_pTran = "ToLoongDB";
	if(!P_VALID(m_pTran))
	{
		ERR(_T("Create ToLoonDB(StreamTransport) obj failed!"));
		return FALSE;
	}
	m_pTran->Init();
	InterlockedExchange((LONG*)&m_lTransCanUse,1);

	// 读取文件, 初始化成员
	if(!InitConfig())
	{
		ERR(_T("Init File Read Failed! Please Check......"));
		return FALSE;
	}

	// 注册所有网络命令
	RegisterAllDBCommand();

	// 启动连接线程
	if(!m_pThread->CreateThread(_T("ConnectLoongDB"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&DBSession::ThreadConnectDB), 

		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ConnectLoongDB")))
	{
		continue;
	}

	//void * pExternallink = g_MemPoolSafe.Alloc  (sizeof(ExternalLinks));
	m_elExternalLinks = new ExternalLinks;//::new (pExternallink)ExternalLinks;

	m_dwWaitConnectionTick = 0;
	m_dwTransDBReconnectTicks = 0;

	m_nItemNLItemNum = 0;
	memset(m_dwItemNLdwTypeID,0,sizeof(m_dwItemNLdwTypeID));

	return TRUE;
}

//-----------------------------------------------------------------------------
// 读取文件, 初始化成员
//-----------------------------------------------------------------------------
BOOL DBSession::InitConfig()
{
	//CreateObj("LoongDBVar", "VarContainer");
	TObjRef<VarContainer> pVar = "VarContainer";
	if(!P_VALID(pVar))
	{
		ERR(_T("Create LoongDBVar(VarContainer) obj failed!"));
		return FALSE;
	}

	// 获取数据库服务器的端口号和金色代码
	m_dwDBPort	= pVar->GetDword(_T("port db_server"));
	m_dwGoldenCode	= pVar->GetDword(_T("golden_code db_server"));

	// 获取运行游戏世界的机器名
	/*CHAR szName[X_LONG_STRING];
	strncpy(szName, m_pTran->GetHostName(), X_LONG_STRING);*/
	//_tcsncpy(m_szServerName, m_pUtil->Unicode8ToUnicode(szName), MCalTCharArrayNum(m_szServerName) - 1);

	// 获取数据服务器IP地址
	TCHAR szIP[X_IP_LEN];
	_tcsncpy(szIP, pVar->GetString(_T("ip db_server")), MCalTCharArrayNum(szIP) - 1);
	IFASTCODE->MemCpy(m_szDBIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szDBIP) - 1);

	//KillObj("LoongDBVar");


	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID DBSession::Destroy()
{
	// 等待所有线程结束
	InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
	m_pThread->WaitForThreadDestroy(_T("ConnectLoongDB"), INFINITE);

	if( P_VALID(m_pTran) )
	m_pTran->Destroy();

	KillObj("ToLoongDB");

	// 清空注册的消息
	m_pMsgCmdMgr->Destroy();
	KillObj("LoongDBNetCmdMgr");

	delete m_elExternalLinks;

	g_WorldRankingMgr.Destroy();
	//m_elExternalLinks->~ExternalLinks();
	//g_MemPoolSafe.Free  (m_elExternalLinks);
}

BOOL DBSession::ReconnectDBServer(VOID)
{
	if( !P_VALID(m_pTran) )
	{
		InterlockedExchange((LONG*)&m_lTransCanUse,0);
		IMSG(_T("transtodb null,creating "));
		// 创建连接对象，并初始化
		CreateObj("ToLoongDB", "StreamTransport");
		m_pTran = "ToLoongDB";
		if(!P_VALID(m_pTran))
		{
			IMSG(_T("false\r\n"));
			return FALSE;
		}
		if(m_pTran->Init() == FALSE)
		{
			IMSG(_T("ok init false\r\n"));
			return FALSE;
		}
		IMSG(_T("ok init ok\r\n"));
		InterlockedExchange((LONG*)&m_lTransCanUse,1);
	}

	if(!m_pTran->IsConnected() /*&& !m_pThread->IsThreadActive(_T("ConnectLoongDB"))*/)
	{
		if( m_pThread->IsThreadActive(_T("ConnectLoongDB")) )
		{
			++m_dwWaitConnectionTick;

			if( m_dwWaitConnectionTick < TICK_PER_SECOND * 2 ) // 不够2秒继续等待；否则等待了2秒，还没有成功，杀掉，重新来
				return FALSE;
		}

		m_dwWaitConnectionTick = 0;

		//if( m_dwTransDBReconnectTicks % TICK_PER_SECOND == 2 ) // 2秒重链接一次
		//{
		IMSG(_T("trans disconnect "));
			InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
			m_pTran->Disconnect();

			//if( m_pThread->IsThreadActive(_T("ConnectLoongDB")) )
				m_pThread->WaitForThreadDestroy(_T("ConnectLoongDB"), INFINITE);
		//}
			IMSG(_T("ok\r\n"));

		if( m_dwTransDBReconnectTicks++ > 3 )
		{
			IMSG(_T("destroy trans "));
			m_dwTransDBReconnectTicks= 0;
			InterlockedExchange((LONG*)&m_lTransCanUse,0);
			m_pTran->Destroy();
			KillObj("ToLoongDB");
			m_pTran = "ToLoongDB";
			IMSG(_T("ok\r\n"));

			TObjRef<StreamTransport> pTrans;

			// 创建连接对象，并初始化
			IMSG(_T("new trans "));
			CreateObj("ToLoongDB", "StreamTransport");
			pTrans = "ToLoongDB";
			if(!P_VALID(pTrans))
			{
				IMSG(_T("false\r\n"));
				return FALSE;
			}
			if(pTrans->Init() == FALSE)
			{
				IMSG(_T("ok init false\r\n"));
				return FALSE;
			}
			m_pTran = "ToLoongDB";
			IMSG(_T("ok init ok\r\n"));
			InterlockedExchange((LONG*)&m_lTransCanUse,1);
		}

		IMSG(_T("connecting "));
		// 重新启动数据库服务器连接线程
		InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
		m_pThread->CreateThread(_T("ConnectLoongDB"), (vEngine::THREADPROC)m_Trunk.sfp0(&DBSession::ThreadConnectDB), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ConnectLoongDB")))
		{
			continue;
		}
		IMSG(_T("end\r\n"));

	}

	m_dwWaitConnectionTick = 0;
	if( m_pTran->IsConnected() )
	{
		m_dwTransDBReconnectTicks = 0;
		IMSG(_T("connect false\r\n"));
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
static INT snCon2DBCounter = 0;
static BOOL bCanTechEmptyCon = TRUE;
VOID DBSession::Update()
{
	CHECK_TIME()
	if( NULL == m_pTran )
		return;

	if(!m_pTran->IsConnected() && !m_pThread->IsThreadActive(_T("ConnectLoongDB")))
	{

		if( !g_world.GetDBReconnectFlag() )
		{
			InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
			m_pTran->Disconnect();

	// 		if( m_pThread->IsThreadActive(_T("ConnectLoongDB")) )	// 应该是一处笔误，前面已经判断过，如果走到这里ConnectLoongDB线程肯定已经是非激活状态了，下面的代码永远都执行不到了。
				m_pThread->WaitForThreadDestroy(_T("ConnectLoongDB"), INFINITE);

			// 重新启动数据库服务器连接线程
			InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
			m_pThread->CreateThread(_T("ConnectLoongDB"), (vEngine::THREADPROC)m_Trunk.sfp0(&DBSession::ThreadConnectDB), NULL);

			while(FALSE == m_pThread->IsThreadActive(_T("ConnectLoongDB")))
			{
				continue;
			}
		}
		else
		{
			if( ReconnectDBServer() == FALSE )
				return;
		}
	}

	// 处理来自数据库服务器的消息
	while(m_pTran->IsConnected())
	{
		DWORD dwSize = 0;
		LPBYTE pRecv = m_pTran->Recv(dwSize);

		if( !P_VALID(pRecv) )
		{
			// 去掉每一小时更新
			//UpdateExternalLinksList();
			//if( bCanTechEmptyCon && ++snCon2DBCounter >= TICK_PER_SECOND * 10 )
			//{
			//	if( !m_pThread->IsThreadActive(_T("ConnectLoongDB")) )
			//	{
			//		m_pTran->Disconnect();
			//		bCanTechEmptyCon = FALSE;
			//	}
			//}
			return;
		}
		snCon2DBCounter = 0;

		// 处理消息
		m_pMsgCmdMgr->HandleCmd((tagNetCmd*)pRecv, dwSize, GT_INVALID);

		// 回收资源
		m_pTran->FreeRecved(pRecv);
	}

	// Jason v1.3.1 external links,两分钟更新一次
	//UpdateExternalLinksList();
}

//-----------------------------------------------------------------------------
// 连接线程(连接数据库服务器)
//-----------------------------------------------------------------------------
DWORD DBSession::ThreadConnectDB()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif
	int nConnectCounter = 100;
	while(FALSE == m_bTermConnect)
	{
		if( !m_pTran->IsConnected() )
		{
			if( !m_pTran->IsTryingToConnect() )
			{
				IMSG(_T("trying to connect dbserver.\r\n"));
				m_pTran->TryToConnect(m_szDBIP, m_dwDBPort);
			}
			--nConnectCounter;

			Sleep(100);
			if( nConnectCounter < 1 )
				return 0;
			continue;	// 重新检测连接
		}
		nConnectCounter = 100;

		IMSG(_T("Contected to DB Server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szDBIP), m_dwDBPort);

		// 发送登陆验证信息及游戏世界的名字和运新游戏世界的机器名
		tagN_Certification	sendCheck;
		sendCheck.dwGoldenCode = g_world.GetGoldenCode();
		_tcsncpy(sendCheck.szWorldName, g_world.GetWorldName(),X_SHORT_NAME - 1);
		//_tcsncpy(sendCheck.szServerName, m_szServerName, X_SHORT_NAME - 1);

		m_pTran->Send(&sendCheck, sendCheck.dwSize);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	return 0;
}


//----------------------------------------------------------------------------------------------
// 注册所有的网络命令
//----------------------------------------------------------------------------------------------
VOID DBSession::RegisterAllDBCommand()
{
	m_pMsgCmdMgr->Register("Heartbeat",				(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleHeartBeat),			_T("Heart Beat"));

	m_pMsgCmdMgr->Register("N_Certification",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleCertification),		_T("DB Server Certification Reply"));
	m_pMsgCmdMgr->Register("NDBS_LoongWorldInitOK", (NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoongWorldInitOK),   _T("LoongWorld Init OK Reply"));
	m_pMsgCmdMgr->Register("NDBS_GetItemInfo",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleItemInfo),			_T("Load Item Max&Min Serial"));
	m_pMsgCmdMgr->Register("NDBS_LoadItemNeedLog",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleItemNeedLog),		_T("Load Item TypeID Which Need Log"));
	m_pMsgCmdMgr->Register("NDBS_LoadAllRoleInfo",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadAllRoleInfo),	_T("Load All Role Info"));

	m_pMsgCmdMgr->Register("NDBS_LoadSimRole",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleEnum),			_T("Load simple Role"));
	m_pMsgCmdMgr->Register("NDBS_CreateRole",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleCreate),			_T("Create Role"));
	m_pMsgCmdMgr->Register("NDBS_DelRole",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleDelete),			_T("Delete Role"));
	m_pMsgCmdMgr->Register("NDBS_ResumeRole",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleResume),			_T("Resume Role"));
	m_pMsgCmdMgr->Register("NDBS_LoadRole",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleLoad),			_T("Load Role all data"));

	m_pMsgCmdMgr->Register("NDBS_LoadBaiBao",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleBaiBaoLoad),			_T("Load New Item Neet Put Into BaiBao Bag"));
	m_pMsgCmdMgr->Register("NDBS_LoadBaiBaoYuanBao",(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleBaiBaoYuanBaoLoad),	_T("Reload Yuanbao Put Into BaiBao Bag"));
	m_pMsgCmdMgr->Register("NDBS_LoadBaiBaoLog",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleBaiBaoLoadLog),		_T("Load BaiBao History Record"));

	m_pMsgCmdMgr->Register("NDBS_LoadAllYBAccount", (NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadAllYBAccount),	_T("Load All Yuan Bao Trade Account"));
	m_pMsgCmdMgr->Register("NDBS_LoadAllOrder",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadAllYBOrder),		_T("Load All Yuan Bao Order"));
	m_pMsgCmdMgr->Register("NDBS_GetRoleYBOrder",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRoleGetYBOrder),		_T("Load Role Yuan Bao Order"));

	m_pMsgCmdMgr->Register("NDBS_GetActTreasureList",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGetActTreasureList),		_T("Load Clan Treasure"));
	m_pMsgCmdMgr->Register("NDBS_GetFameHallEnterSnap",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGetFameHallEnterSnap),	_T("Load FameHall EnterSnap"));
	m_pMsgCmdMgr->Register("NDBS_GetRepResetTimestamp",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRepRstTimeStamp),		_T("Load Reputation RstTimeSamp"));
	m_pMsgCmdMgr->Register("NDBS_GetRepRankList",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRepRankLoad),		_T("Load Reputation Rank"));

	// 帮派相关
	m_pMsgCmdMgr->Register("NDBS_LoadAllEmptyGuild",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadAllEmptyGuild),			_T("load guild skill info"));
	m_pMsgCmdMgr->Register("NDBS_LoadAllGuild",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuild),				_T("load guild"));
	m_pMsgCmdMgr->Register("NDBS_LoadAllGuildMember",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuildMember),		_T("load guild member"));
	m_pMsgCmdMgr->Register("NDBS_GuildMemberInitOK",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGuildMemberInitOk),		_T("load guild member ok"));
	m_pMsgCmdMgr->Register("NDBS_LoadGuildWareItems",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuildWareItems),		_T("load guild warehouse items"));
	m_pMsgCmdMgr->Register("NDBS_LoadFacilitiesInfo",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuildUpgradeInfo),	_T("load guild upgrade info"));
	m_pMsgCmdMgr->Register("NDBS_LoadGuildSkillInfo",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuildSkillInfo),		_T("load guild skill info"));
	m_pMsgCmdMgr->Register("NDBS_LoadCommodity",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuildCommerceInfo),	_T("load guild commerce info"));
	m_pMsgCmdMgr->Register("NDBS_LoadCommerceRank",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadCommerceRankInfo),	_T("load commerce rank info"));
	m_pMsgCmdMgr->Register("NDBS_GuildInitOK",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGuildInitOK),			_T("load guild skill info"));
	m_pMsgCmdMgr->Register("NDBS_CreateGuild",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleCreateGuild),			_T("CreateGuild info"));
	m_pMsgCmdMgr->Register("NDBS_InsertGuildName",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleInsertGuildName),			_T("InsertGuildName info"));
	
	// 宠物相关
	m_pMsgCmdMgr->Register("NDBS_CreatePetSoul",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleCreatePetSoul),			_T("create pet soul"));

	// 商城相关
	m_pMsgCmdMgr->Register("NDBS_GetAllGPInfo",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadAllGPInfo),			_T("load all group purchase info"));
	m_pMsgCmdMgr->Register("NDBS_LoadLimitItemInfo",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadLimitItemInfo),			_T("load all Limit Item Num Info"));

	// 名帖相关
	m_pMsgCmdMgr->Register("NDBS_GetOffLineVCard",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadRoleVCard),			_T("load db vcard"));

	// VIP摊位
	m_pMsgCmdMgr->Register("NDBS_GetAllVIPStallInfo",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadVIPStallInfo),		_T("load all vip stall info"));

	// 固定活动
	m_pMsgCmdMgr->Register("NDBS_LoadActivityData",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadActivityData),		_T("load activity data"));

	// 金牌网吧
	m_pMsgCmdMgr->Register("NDBS_GetVNBData",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadVNBData),			_T("load vip netbar data"));

	// 实力排行榜
	m_pMsgCmdMgr->Register("NDBS_UpdateRankings",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRetUpdateRankings),		_T("update rankings"));
	m_pMsgCmdMgr->Register("NDBS_InitRankings",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleInitRankings),			_T("init rankings"));
	m_pMsgCmdMgr->Register("NDBS_InitPetRankings",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleInitPetRankings),		_T("init pet rankings"));
	m_pMsgCmdMgr->Register("NDBS_InitFlowerRankings",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleInitFlowerRankings),		_T("init flower rankings"));
	m_pMsgCmdMgr->Register("NDBC_UpdateRankingsDone",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleReloadRankings),			_T("reload rankings"));
	//m_pMsgCmdMgr->Register("NDBS_ClergyVoteDone",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleAsk4NewClergy),			_T("Ask 4 NewClergy"));
	m_pMsgCmdMgr->Register("NDBS_ClearAllCompetitiveClergy",(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleClearAllCompetitiveClergy),		_T("Ask 4 NewClergy"));
	m_pMsgCmdMgr->Register("NDBC_RenewAdvanceStrength",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRenewAdvanceStrength),	_T("renew AdvanceStrength"));
	
	// 神职竞选
	m_pMsgCmdMgr->Register("NDBS_InitClergyRank",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleInitClergyRank),					_T(""));
	m_pMsgCmdMgr->Register("NDBS_RenewClergyCandidateType",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleRenewClergyCandidateType),	_T(""));
	m_pMsgCmdMgr->Register("NDBS_Ask4CandidateCount",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGetClergyCandidateCount),		_T(""));

	// 读取留言
	m_pMsgCmdMgr->Register("NDBS_LoadLeftMsg",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadLeftMsg),			_T("load left msg"));

	// 城市相关
	m_pMsgCmdMgr->Register("NDBS_LoadCityInfo",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadCityInfo),			_T("load city info"));
	m_pMsgCmdMgr->Register("NDBS_GetGuildClanValue",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGetGuildClanValue),		_T("get guild clan value"));

	// 师徒
	m_pMsgCmdMgr->Register("NDBS_LoadMasterApprenticeData",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadMasterApprenticeData),	_T("load master data"));
	m_pMsgCmdMgr->Register("NDBS_LoadForceBreakOutApprenticeList",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadForceBreakOutApprenticeList),	_T("load Force Break Out Apprentice List"));

	//m_pMsgCmdMgr->Register("",				(NETMSGPROC)m_Trunk.sfp2(&DBSession::),			_T(""));

	// Jason v1.3.1 外部链接
	m_pMsgCmdMgr->Register("NDBS_GetExternalLinks",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleExternalLinksList),			_T("external links msg"));

	// Jason 2010-1-27 v1.3.2 玩家回归/离线奖励相关
	m_pMsgCmdMgr->Register("NDBS_LoadAccountInfo",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGetAccountInfo),			_T("load account info"));
	m_pMsgCmdMgr->Register("NDBS_LoadShortCut_QuickBar",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadShortCut_QuickBar), _T("load quickbar info"));
	m_pMsgCmdMgr->Register("NDBS_IsSavedShortCut_QuickBar",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleIsSavedShortCut_QuickBar), _T("have been saved quickbar info"));
   // 竞技场
    m_pMsgCmdMgr->Register("NDBS_GetSerialVal",			   (NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGetSerialVal),			_T("Get Serial Value message"));
	
	m_pMsgCmdMgr->Register("NDBS_LoadShowPlayerData",	   (NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadShowPlayerData),				_T("LoadShowPlayerData"));
	m_pMsgCmdMgr->Register("NDBS_LoadVotePlayerData",	   (NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadVotePlayerData),				_T("LoadVotePlayerData"));

	m_pMsgCmdMgr->Register("NDBS_LoadActiveRankData",	   (NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadActiveRankData),				_T("LoadVotePlayerData"));
 
 

	m_pMsgCmdMgr->Register("NDBS_CheckName",			   (NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleCheckName),				_T("check name"));

	// 仇敌上线，通知玩家
	m_pMsgCmdMgr->Register("NDBS_GetRoleListOfEnemy",	   (NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGetRoleListOfEnemy),				_T("Get Role List Of Enemy"));

	// 载入数据库中的公会宣战关系
	m_pMsgCmdMgr->Register("NDBS_LoadGuildWarDeclare",	   (NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGuildWarDeclare),				_T("Load Guild War Declare from db"));

	// 公会战场
	m_pMsgCmdMgr->Register("NDBS_LoadBattleFieldAttackSignUpInfo",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadBattleFieldAttackSignUpInfo),	_T("Load BattleField SignUp Info"));
	m_pMsgCmdMgr->Register("NDBS_LoadAllicanceGuildInfo",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadAllicanceGuildInfo),				_T("Load Allicance GuildInfo"));

	// 妖精重生
	m_pMsgCmdMgr->Register("NDBS_GetFabaoNirVanaData",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleGetFabaoNirVanaData),				_T("NDBS_GetFabaoNirVanaData"));
	// 神之竞标
	m_pMsgCmdMgr->Register("NDBS_LoadGodBidInfo",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGodBidInfo),				_T("NDBS_LoadGodBidInfo"));
	m_pMsgCmdMgr->Register("NDBS_ClearGodBidInfo",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleClearGodBidInfo),				_T("NDBS_ClearGodBidInfo"));

	// 可以重新load某个角色的全局simple信息
	m_pMsgCmdMgr->Register("NDBS_LoadOneRoleInfo",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadOneRoleInfo),				_T("NDBC_LoadOneRoleInfo"));

	// 稀有物品
	m_pMsgCmdMgr->Register("NDBS_LoadGlobalLimitedItem",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadGlobalLimitItem),			_T(""));

	// 家族妖精
	m_pMsgCmdMgr->Register("NDBS_LoadFamilySprite",				(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadFamilySprite),			_T(""));
	m_pMsgCmdMgr->Register("NDBS_LoadFamilyQuest",				(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadFamilyQuest),			_T(""));
	
	//m_pMsgCmdMgr->Register("NDBS_LoadFamilySpriteRequirement",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadFamilySpriteRequirement),			_T(""));

	m_pMsgCmdMgr->Register("NDBS_LoadAllFamily",						(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadAllFamily),								_T("NDBS_LoadAllFamily"));
	m_pMsgCmdMgr->Register("NDBS_LoadFamilyMember",				(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadFamilyMember),					_T("NDBS_LoadFamilyMember"));
	m_pMsgCmdMgr->Register("NDBS_FamilyMemberInitOK",			(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleFamilyMemberInitOK),					_T("NDBS_FamilyMemberInitOK"));
	m_pMsgCmdMgr->Register("NDBS_FamilyInitOK",							(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleFamilyInitOK),								_T("NDBS_FamilyInitOK"));
	
	m_pMsgCmdMgr->Register("NDBS_RequestLogWorldData",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadWorldData),		_T("NDBS_RequestLogWorldData"));
	m_pMsgCmdMgr->Register("NDBS_RequestLogRoleData",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadRoleLogData),		_T("NDBS_RequestLogRoleData"));

	m_pMsgCmdMgr->Register("NDBS_AccountReactiveCode",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleAccountReactiveOrNot),		_T("NDBS_AccountReactiveCode"));
	m_pMsgCmdMgr->Register("NDBS_InitAccountReactiveInfo",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleInitAccountReactiveInfo),		_T("NDBS_InitAccountReactiveInfo"));

	m_pMsgCmdMgr->Register("NDBS_TigerProfit",		(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleLoadRoleTigerProfit),		_T("NDBS_TigerProfit"));

	m_pMsgCmdMgr->Register("NDBS_KeyCodeAddGift",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandelKeyCodeAddGift), _T("NDBS_KeyCodeAddGift"));
	m_pMsgCmdMgr->Register("NDBS_ReceiveYuanBao",	(NETMSGPROC)m_Trunk.sfp2(&DBSession::HandleNDBS_ReceiveYuanBao), _T("NDBS_ReceiveYuanBao"));
}

//-------------------------------------------------------------------------------------------------
// 认证信息
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleCertification(tagNetCmd* pMsg, DWORD)
{
	tagN_Certification* pRecv = (tagN_Certification*)pMsg;

	/*if( pRecv->dwGoldenCode == m_dwGoldenCode
		&& 0 == _tcsncmp(pRecv->szServerName, m_szServerName, MCalTCharArrayNum(pRecv->szServerName))
		&& 0 == _tcsncmp(pRecv->szWorldName, g_world.GetWorldName(), MCalTCharArrayNum(pRecv->szWorldName))
	)
	{*/

	m_dwInitTime =  timeGetTime();

	// 初始化名人堂信息
	g_fameHall.SendLoadDBData();

	// 获取物品信息
	tagNDBC_GetItemInfo itemInfo;
	Send(&itemInfo, itemInfo.dwSize);
	
	// 发送获取物品是否保存信息
	tagNDBC_LoadItemNeedLog itemNeedLog;
	Send(&itemNeedLog, itemNeedLog.dwSize);	
	
	// 发送获取所有角色信息的消息
	tagNDBC_LoadAllRoleInfo send;
	Send(&send, send.dwSize);

	tagNDBC_TigerProfit profit;
	Send(&profit, profit.dwSize);

	// 发送获得所有玩家元宝交易账户消息
	tagNDBC_LoadAllYBAccount YBAccount;
	Send(&YBAccount, YBAccount.dwSize);

	tagNDBC_LoadAllOrder YBOrder;
	Send(&YBOrder, YBOrder.dwSize);

	// 获取帮派及帮派成员信息
	tagNDBC_LoadAllGuild loadGuild;
	Send(&loadGuild, loadGuild.dwSize);

	tagNDBC_LoadAllGuildMember loadGuildMember;
	Send(&loadGuildMember, loadGuildMember.dwSize);

	// 获取家族信息
	tagNDBC_LoadAllFamily LoadFamily;
	Send(&LoadFamily, LoadFamily.dwSize);

	tagNDBC_LoadFamilyQuest LoadFamilyQuest;
	Send(&LoadFamilyQuest, LoadFamilyQuest.dwSize);

	tagNDBC_LoadFamilyMember LoadFamilyMember;
	Send(&LoadFamilyMember, LoadFamilyMember.dwSize);

	// 申请城市信息(必须在申请帮派信息之后)
	g_cityMgr.RequestCityInfoFromDB();

	// 向DB申请初始化团购信息
	tagNDBC_GetAllGPInfo GPInfo;
	g_dbSession.Send(&GPInfo, GPInfo.dwSize);

	// 向DB申请商城限量物品信息
	
	tagNDBC_LoadLimitItemInfo LimitItemInfo;
    g_dbSession.Send(&LimitItemInfo, LimitItemInfo.dwSize);

	// 获取VIP摊位信息
	tagNDBC_GetAllVIPStallInfo VIPStallInfo;
	g_dbSession.Send(&VIPStallInfo, VIPStallInfo.dwSize);

	// 读取活动脚本数据
	tagNDBC_LoadActivityData ActivityData;		 
	g_dbSession.Send(&ActivityData, ActivityData.dwSize);

	// 获取金牌网吧数据
	tagDWORDTime dwToday = GetCurrentDWORDTime();
	dwToday.sec = 0;
	dwToday.min = 0;
	dwToday.hour = 0;
    
	// 获取竞技场数据
	tagNDBC_GetSerialVal SerialVal;
	SerialVal.dwSerialId = 1 ;
	g_dbSession.Send(&SerialVal,SerialVal.dwSize);

	// 获取稀有物品信息
	tagNDBC_LoadGlobalLimitedItem GlobalItemInfo;
	g_dbSession.Send(&GlobalItemInfo, GlobalItemInfo.dwSize);

	// 获取家族妖精信息
	tagNDBC_LoadFamilySprite FamilySprite;
	g_dbSession.Send(&FamilySprite, FamilySprite.dwSize);

	//// 获取稀有领域信息
	//tagNDBC_LoadAllRareArea RareArea;
	//Send(&RareArea, RareArea.dwSize);
	
	// 加载各种排行榜
	g_WorldRankingMgr.Init();

	// 加载神职竞选信息
	g_ClergyRankingMgr.Init();
    
	// 载入数据库中存储的强制脱离师门申请列表
	g_forceBreakOutApprenticeMgr.Init();

	// 载入数据库中的公会宣战关系
	g_guildWarDeclareMgr.Init();

	// 载入world data
	tagNDBC_RequestLogWorldData worlddata;
	g_dbSession.Send(&worlddata, worlddata.dwSize);
	
	// 载入战场报名数据
	CGodMiracleMgr::GetInst()->LoadSignUpInfo();

	//获得选秀的数据
	g_DraftShow.LoadDataFromDB();

	/*tagNDBC_GetVNBData VIPNetBar;
	VIPNetBar.dwDate = dwToday;
	g_dbSession.Send(&VIPNetBar, VIPNetBar.dwSize);*/

	// Jason 2009-12-29 初始化外部链接
	InitialExternalLinksList();

	g_GodBidMgr.LoadGodBidInfoFromDB();

	// loongworld向loongDB发送的初始化完成的确认消息（保证最后一个发送）
	tagNDBC_LoongWorldInitOK	sendInitOk;	
	Send(&sendInitOk, sendInitOk.dwSize);
	/*}
	else
	{
		ASSERT(0);	
	}*/

	IMSG(_T("=========================================\r\n"));
	IMSG(_T("==                                     ==\r\n"));
	IMSG(_T("==         World is working now.       ==\r\n"));
	IMSG(_T("==                                     ==\r\n"));
	IMSG(_T("=========================================\r\n"));
	IMSG(_T("\r\n"));
	IMSG(_T("Receiving message from db server, please wait.\r\n"));
	IMSG(_T("\r\n"));

	return 0;
}

//-------------------------------------------------------------------------------------------------
// 心跳
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleHeartBeat(tagNetCmd* pMsg, DWORD)
{
	return 0;
}

//------------------------------------------------------------------------------------------------
// LoongWorld初始化完成后LoongDB返回的确认消息
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoongWorldInitOK(tagNetCmd* pMsg, DWORD)
{
	// 是否需要先检查是否所有需要数据都初始化成功//?? 比如加个初始化状态标志位 -- 实现方式待考虑
	
	// 初始化完成
	InterlockedExchange((LPLONG)&m_bInitOK, TRUE);

	m_dwInitTime = timeGetTime() - m_dwInitTime;
	this->OutPutInitStepInfo(EDBS_InitOk,TRUE,m_dwInitTime);
    

	//帮派改名需要在所有东西初始化后，再向数据库要这些空帮派的数据，防止服务器宕机
	tagNDBC_LoadAllEmptyGuild msg;
	g_dbSession.Send(&msg,msg.dwSize);

	// 帮派初始化完成后，帮派排序
	ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
	if( P_VALID(pToolkit) )
	{
		pToolkit->GuildRanking();
	}

	return 0;
}

//-------------------------------------------------------------------------------------------------
// 载入数据库中所有人物简易信息
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadAllRoleInfo(tagNetCmd* pMsg, DWORD)
{
	tagNDBS_LoadAllRoleInfo* pRecv = (tagNDBS_LoadAllRoleInfo*)pMsg;

	if( E_Success != pRecv->dwErrorCode )
	{
		this->OutPutInitStepInfo(EDBS_LoadAllRoleInfo,FALSE);
		return GT_INVALID;
	}
	
	// 删除所有静态角色
	//g_roleMgr.EraseAllRoleInfo();

	// 设置当前最大角色ID
//	g_roleMgr.SetMaxRoleID(pRecv->dwMaxRoleID);

	// 开始加载角色
	tagRoleInfo* pInfo = &pRecv->RoleInfo[0];
	for(INT n = 0; n < pRecv->nNum; n++)
	{
		g_roleMgr.CreateRoleInfo(pInfo);
		pInfo++;
	}

	this->OutPutInitStepInfo(EDBS_LoadAllRoleInfo,TRUE);

	return 0;
}

DWORD DBSession::HandleLoadOneRoleInfo(tagNetCmd* pMsg, DWORD)
{
	tagNDBS_LoadOneRoleInfo* pRecv = (tagNDBS_LoadOneRoleInfo*)pMsg;

	if( E_Success != pRecv->dwErrorCode )
	{
		return GT_INVALID;
	}
	
	tagRoleInfo* pInfo = &pRecv->RoleInfo;
	tagRoleInfo* pExistInfo = g_roleMgr.GetRoleInfo(pInfo->dwRoleID);
	if( P_VALID(pExistInfo) )
	{
		g_roleMgr.SetRoleInfo(pInfo);
		Role * pRole = g_roleMgr.GetRolePtrByID(pInfo->dwRoleID);
		if( P_VALID(pRole) )
		{
			tagNS_RoleGetSomeName msg;
			msg.nUserData = 1;
			msg.nNum = 1;
			msg.IdName[0].dwID = pInfo->dwRoleID;
			memcpy(msg.IdName[0].szName,pRecv->RoleInfo.szRoleName,sizeof(msg.IdName[0].szName));
			msg.IdName[0].szName[X_SHORT_NAME-1] = 0;
			pRole->SendMessage(&msg,msg.dwSize);
		}
	}
	else
	{
		g_roleMgr.CreateRoleInfo(pInfo);
	}

	return 0;
}


//-------------------------------------------------------------------------------------------------
// 载入角色客户端简易信息
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleItemInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_GetItemInfo);

	// 交给物品管理器处理
	ItemCreator::SetItemSerial(pRecv->n64MaxSerial, pRecv->n64MinSerial);

	this->OutPutInitStepInfo(EDBS_ItemInfo,TRUE);

	return 0;
}

//-------------------------------------------------------------------------------------------------
// 载入角色客户端简易信息
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleItemNeedLog(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadItemNeedLog);

	if( E_Success != pRecv->dwErrorCode )
	{	
		this->OutPutInitStepInfo(EDBS_ItemNeedLog,FALSE);
		return GT_INVALID;
	}

	// 交给资源管理器处理
	if( !m_bInitOK )
		g_attRes.ResetItemLog(pRecv->dwTypeID, pRecv->nItemNum);
	else
	{
		memset(m_dwItemNLdwTypeID,0,sizeof(m_dwItemNLdwTypeID));
		m_nItemNLItemNum = pRecv->nItemNum;
		m_nItemNLItemNum = min(pRecv->nItemNum , MAX_ITEM_NEEDLOG_NUM);
		if( m_nItemNLItemNum > 0 )
			memcpy(m_dwItemNLdwTypeID,pRecv->dwTypeID,m_nItemNLItemNum * sizeof(DWORD));
	}

	this->OutPutInitStepInfo(EDBS_ItemNeedLog,TRUE);

	return 0;
}


//-------------------------------------------------------------------------------------------------
// 载入角色客户端简易信息
//-------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRoleEnum(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadSimRole);
	
	// 首先根据AccountID找到session
	PlayerSession* pSession = g_worldSession.FindGlobalSession(pRecv->dwAccountID);

	// 预想的是dbsession处理的时候不处理客户端的消息，这样就不存在线程互锁问题了
	if( !P_VALID(pSession) ) return GT_INVALID;
	if( !pSession->IsRoleEnuming() ) return GT_INVALID;

	// 初始化
	for(INT i = 0; i < MAX_ROLENUM_ONEACCOUNT; ++i)
	{
		pSession->m_dwRoleID[i] = GT_INVALID;
	}

	pSession->m_n8RoleNum = 0;
	
	if( E_Success == pRecv->dwErrorCode )
	{
		for(INT n = 0; n < pRecv->nNum; n++)
		{
			pSession->m_dwRoleID[n] = pRecv->SimRole[n].dwRoleID;
		}

		// 账号通用信息
		pSession->m_sAccountCommon = pRecv->sAccountCommon;

		// 角色个数
		pSession->m_n8RoleNum = pRecv->nNum;
	}

	pSession->m_bRoleEnuming = false;
	pSession->m_bRoleEnumDone = true;
	pSession->m_bRoleEnumSuccess = (E_Success == pRecv->dwErrorCode ? true : false);

	// 返回给客户端
	if( E_Success != pRecv->dwErrorCode || pRecv->nNum <= 0 )
	{
		tagNS_EnumRole send;
		send.dwErrorCode = pRecv->dwErrorCode;
		
		send.dwSafeCodeCrc = pRecv->sAccountCommon.sSafeCode.dwSafeCodeCrc;
		send.dwTimeReset = pRecv->sAccountCommon.sSafeCode.dwTimeReset;
		send.nNum = 0;

		send.dwTimeLastLogin = pSession->GetPreLoginTime();
		send.dwIPLast = pSession->GetPreLoginIP();
		send.dwIP = pSession->GetCurLoginIP();
		send.dwRoleID = pSession->GetLastUseRoleID();

		pSession->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}
	else
	{
		DWORD dwSize = sizeof(tagNS_EnumRole) - 1 + sizeof(tagSimRole) *  pRecv->nNum;
		PBYTE pNew = new BYTE[dwSize];
		if( !P_VALID(pNew) ) return GT_INVALID;

		PlayerRegression * pPlayerRegression = TPlayerRegression::Instance  ();

		tagNS_EnumRole* pSend = (tagNS_EnumRole*)pNew;

		pSend->dwID = m_pUtil->Crc32("NS_EnumRole");
		pSend->dwSize = dwSize;
		pSend->dwErrorCode = pRecv->dwErrorCode;
		pSend->dwSafeCodeCrc = pRecv->sAccountCommon.sSafeCode.dwSafeCodeCrc;
		pSend->dwTimeReset = pRecv->sAccountCommon.sSafeCode.dwTimeReset;
		pSend->nNum = pRecv->nNum;

		pSend->dwTimeLastLogin = pSession->GetPreLoginTime();
		pSend->dwIPLast = pSession->GetPreLoginIP();
		pSend->dwIP = pSession->GetCurLoginIP();
		pSend->dwRoleID = pSession->GetLastUseRoleID();

		IFASTCODE->MemCpy(pSend->bySimpleInfo, pRecv->SimRole, sizeof(tagSimRole) *  pRecv->nNum);
		tagSimRole * pSimRole = (tagSimRole *)pSend->bySimpleInfo;
		for(int i = 0; i < pRecv->nNum; ++i)
		{
			if(g_world.IsPlayerBack())
			{
				if(pSimRole[i].bOldPlayerBack == Role::EPlyBack_Recv)
				{
					pSimRole[i].bOldPlayerBack = FALSE;
				}
				else if(pSimRole[i].bOldPlayerBack == Role::EPlyBack_NotRecv)
				{
					pSimRole[i].bOldPlayerBack = TRUE;
				}
				else
				if(P_VALID(pPlayerRegression) && 
					pPlayerRegression->CanRecRegression(pRecv->SimRole[i].timeLogout,
														pRecv->SimRole[i].bOldPlayerBack,
														pRecv->SimRole[i].dwBackDays,
														pRecv->SimRole[i].byLevel))
				{
					pSimRole[i].bOldPlayerBack = TRUE;
				}
			}
		}

		pSession->SendMessage(pSend, pSend->dwSize);

		SAFE_DEL_ARRAY(pNew);

		// Jason 2010-2-2
		// Jason 2010-1-19 v1.3.2 玩家回归
		//PlayerRegression * pPlayerRegression = TPlayerRegression::Instance  ();
		//if( P_VALID(pPlayerRegression) )
		//	pPlayerRegression->IniPlayer  (pSession);

		return 0;
	}
}

//--------------------------------------------------------------------------------------------------------
// 创建角色
//--------------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRoleCreate(tagNetCmd* pMsg, DWORD)
{
	
	tagNDBS_CreateRole* pRecv = (tagNDBS_CreateRole*)pMsg;

	PlayerSession* pSession = g_worldSession.FindGlobalSession(pRecv->dwAccountID);
	if( !P_VALID(pSession) ) return GT_INVALID;
	if( !pSession->IsRoleCreating() ) return GT_INVALID;

	// 如果创建成功
	if( E_Success == pRecv->dwErrorCode )
	{
		pSession->DeductCreateRoleNumLimit();
		if ( pSession->GetCreatedRoleNumLimit() <= 0 )
		{
			pSession->SetCreatedRoleNumLimit(0);
		}
		ASSERT( P_VALID(pRecv->SimRole.dwRoleID) );

		// 加入到session的角色列表中
		pSession->AddRole(pRecv->SimRole.dwRoleID);

		// 增加一个新的离线玩家
		g_roleMgr.CreateRoleInfo(&pRecv->RoleInfo);

		// 给角色添加天生技能
		AddInitialSkill2Role(pRecv->SimRole.dwRoleID);
	}
	pSession->m_bRoleCreating = false;

	// 返回给客户端
	if( E_Success != pRecv->dwErrorCode )
	{
		tagNS_CreateRole send;
		send.dwErrorCode = E_CreateRole_NameExist;

		pSession->SendMessage(&send, send.dwSize);

		return GT_INVALID;	
	}

	else
	{
		DWORD dwSize = sizeof(tagNS_CreateRole) - 1 + sizeof(tagSimRole);
		PBYTE pNew = new BYTE[dwSize];
		if( !P_VALID(pNew) ) return GT_INVALID;

		tagNS_CreateRole* pSend = (tagNS_CreateRole*)pNew;

		pSend->dwID = m_pUtil->Crc32("NS_CreateRole");
		pSend->dwSize = dwSize;
		pSend->dwErrorCode = pRecv->dwErrorCode;
		IFASTCODE->MemCpy(pSend->bySimRoleInfo, &pRecv->SimRole, sizeof(tagSimRole));

		pSession->SendMessage(pSend, pSend->dwSize);

		SAFE_DEL_ARRAY(pNew);

		return 0;
	}
}

//---------------------------------------------------------------------------------------------
// 删除角色
//---------------------------------------------------------------------------------------------
DWORD DBSession::HandleRoleDelete(tagNetCmd* pMsg, DWORD)
{
	tagNDBS_DelRole* pRecv = (tagNDBS_DelRole*)pMsg;

	PlayerSession* pSession = g_worldSession.FindGlobalSession(pRecv->dwAccountID);
	if( !P_VALID(pSession) ) return GT_INVALID;
	if( !pSession->IsRoleDeleting() ) return GT_INVALID;
	
	if( E_Success == pRecv->dwErrorCode )
	{
		// 从玩家的角色列表中删除
		pSession->RemoveRole(pRecv->dwRoleID);

		// 删除一个离线玩家
		g_roleMgr.DeleteRoleInfo(pRecv->dwRoleID);
	}
	pSession->m_bRoleDeleting = false;

	// 发送给客户端
	tagNS_DeleteRole send;
	send.dwErrorCode = pRecv->dwErrorCode;
	send.dwRoleID = pRecv->dwRoleID;

	pSession->SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------------------------------
// 选择角色
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRoleLoad(tagNetCmd* pMsg, DWORD)
{
	tagNDBS_LoadRole* pRecv = (tagNDBS_LoadRole*)pMsg;

	PlayerSession* pSession = g_worldSession.FindGlobalSession(pRecv->dwAccountID);
	if( !P_VALID(pSession) ) return GT_INVALID;
	if( !pSession->IsRoleLoading() ) return GT_INVALID;

	// 给客户端发送返回消息
	tagNS_SelectRole send;
	send.dwErrorCode = pRecv->dwErrorCode;
	send.dwRoleID = pRecv->dwRoleID;
	pSession->SendMessage(&send, send.dwSize);

	if( E_Success == pRecv->dwErrorCode )
	{
		ASSERT( NULL == pSession->GetRole() );
		
		// todo: 检查一下roledata的大小及合法性
		
		// 生成一个新的role
		BOOL bFirst = FALSE;
		Role* pRole = g_roleMgr.CreateRole(pRecv->dwRoleID, &pRecv->RoleData, pSession, bFirst);
		if( NULL == pRole ) return GT_INVALID;
		//g_worldSession.Kick(pSession->GetInternalIndex());pSession->SetKicked(); return GT_INVALID;
#if 1
		// 客户端在这时可能还没有初始化ok，改在别的地方发送这条消息。
		// Jason 2010-2-22 v1.3.3 帮派功勋通知消息
		DWORD dwGuildID = pRole->GetGuildID();
		if( P_VALID(dwGuildID) )
		{
			Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
			if( P_VALID(pGuild) )
			{
				tagNS_GuildExploit msg_guild_exploit;
				msg_guild_exploit.dwRoleID = pRecv->dwRoleID;
				msg_guild_exploit.nExploit = 0;

				tagGuildMember* pMember = pGuild->GetMember(pRecv->dwRoleID);
				if( P_VALID(pMember) )
				{
					msg_guild_exploit.nExploit = pMember->nExploit;
				}

				pRole->SendMessage(&msg_guild_exploit,msg_guild_exploit.dwSize);
			}
		}
#endif
		// Todo: 角色生成后的修正步骤也写一个函数

		if( !pSession->FullLogin(pRole, bFirst) )
		{
			g_roleMgr.DeleteRole(pRole->GetID());
		}
		else
		{
			// 如果百宝袋中有空位，则申请检查是否还有未放入的物品
			if(pRole->GetItemMgr().GetBaiBaoFreeSize() > 0)
			{
				tagNDBC_LoadBaiBao send;
				send.dwAccountID = pRecv->dwAccountID;
				send.n64Serial = GT_INVALID;
				send.nFreeSpaceSize = pRole->GetItemMgr().GetBaiBaoFreeSize();

				g_dbSession.Send(&send, send.dwSize);
			}

// 			if (P_VALID(dwGuildID))
// 			{
// 				tagDWORDTime dwTime = GetCurrentDWORDTime();
// 				if ( dwTime.hour == 20 && (dwTime.min >= 0 && dwTime.min <= 30))
// 				{
// 					CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->IsInCaptureGodRange(dwGuildID);
// 					if ( P_VALID(pGodMiracle))
// 					{
// 						pGodMiracle->CheckAllGuildMemberIsInGodArea();
// 					}
// 				}
// 			}
		}
	}
	else
		pSession->SetRoleLoading(FALSE);

	return 0;
}

//------------------------------------------------------------------------------------------------
// 向百宝袋放入新的物品
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleBaiBaoLoad(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadBaiBao);

	if(pRecv->dwErrorCode != E_Success || 0 == pRecv->nRetNum)
	{
		return pRecv->dwErrorCode;
	}
	
	PlayerSession* pSession = g_worldSession.FindSession(pRecv->dwAccountID);
	if( !P_VALID(pSession) ) return GT_INVALID;
	
	Role *pRole = pSession->GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	INT32 nItemSize		= sizeof(tagItem);
	INT32 nEquipSize	= sizeof(tagEquip);

	tagItem	*pTmpItem	= NULL;
	tagItem	*pNewItem	= NULL;

	tagNDBC_DelBaiBaoItem	sendItem;
	tagNDBC_DelBaiBaoEquip	sendEquip;

	BYTE * p = (BYTE*)pRecv->byData;
	tagEquip * pEquip = NULL;
	//pTmpItem = (tagItem*)pRecv->byData;
	for(INT32 i=0; i<pRecv->nRetNum; ++i)
	{
		INT32 * pLen = (INT32*)p;
		p += sizeof(INT32);
		pTmpItem = (tagItem*)p;

		if(!MIsEquipment(pTmpItem->dwTypeID))
		{
			pNewItem = ItemCreator::CreateItemByData(pTmpItem);

			//pTmpItem = (tagItem*)((BYTE*)pTmpItem + nItemSize);

			// 先发消息,通知DBServer从item_baibao表中删除物品，以免出现因宕机等产生的复制现象
			if(P_VALID(pNewItem->pProtoType))
			{
				sendItem.n64Serial = pNewItem->n64Serial;
				g_dbSession.Send(&sendItem, sendItem.dwSize);

				// 记录log //??
			}
		}
		else
		{
			pTmpItem->pEquipProto = g_attRes.GetEquipProto(pTmpItem->dwTypeID);
			if( !P_VALID(pTmpItem->pEquipProto) )
			{
				p += *pLen;
				continue;
			}
			if( MIsFaBao(pTmpItem) && *pLen < SIZE_FABAO )
			{
				const_cast<tagItemProto*>(pTmpItem->pProtoType)->eType = EIT_Armor;
				pEquip = ItemCreator::CreateEquipByData(pTmpItem);
				pEquip->equipSpec.byQuality = GT_INVALID;
				const_cast<tagItemProto*>(pTmpItem->pProtoType)->eType = EIT_FaBao;
				tagFabao * pFabao = new tagFabao;
				memcpy((tagEquip*)pFabao,pEquip,SIZE_EQUIP);
				pFabao->pProtoType = g_attRes.GetFabaoProto(pTmpItem->dwTypeID);
				delete pEquip;
				pEquip = pFabao;
			}
			else if (MIsSoulCrystal(pTmpItem) && *pLen < SIZE_HUNJING)
			{				
				pEquip = ItemCreator::CreateEquipByData(pTmpItem);
				tagSoulCrystal * pHunjing = new tagSoulCrystal;
				memcpy((tagEquip*)pHunjing,pEquip,SIZE_EQUIP);
				delete pEquip;
				pEquip = pHunjing;
			}
			else if (MIsHoly(pTmpItem) && *pLen < SIZE_HOLY)
			{
				const_cast<tagItemProto*>(pTmpItem->pProtoType)->eType = EIT_Armor;
				pEquip = ItemCreator::CreateEquipByData(pTmpItem);
				const_cast<tagItemProto*>(pTmpItem->pProtoType)->eType = EIT_Holy;
				tagHolyMan * pHolyMan = new tagHolyMan;
				memcpy((tagEquip*)pHolyMan,pEquip,SIZE_EQUIP);
				pHolyMan->pProtoType = g_attRes.GetHolyProto(pTmpItem->dwTypeID);
				delete pEquip;
				pEquip = pHolyMan;
			}
			else if (MIsHolyEquip(pTmpItem) && *pLen < SIZE_HOLYEQUIP)
			{
				const_cast<tagItemProto*>(pTmpItem->pProtoType)->eType = EIT_Armor;
				pEquip = ItemCreator::CreateEquipByData(pTmpItem);
				const_cast<tagItemProto*>(pTmpItem->pProtoType)->eType = EIT_HolyEquip;
				tagHolyEquip * pHolyEquip = new tagHolyEquip;
				memcpy((tagEquip*)pHolyEquip,pEquip,SIZE_EQUIP);
				pHolyEquip->pProtoType = g_attRes.GetHolyEquipProto(pTmpItem->dwTypeID);
				delete pEquip;
				pEquip = pHolyEquip;
			}
			else
			{
				pEquip = ItemCreator::CreateEquipByData(pTmpItem);
			}

			if( !MIsIdentified(pEquip) )
			{
				ItemCreator::IdentifyEquip(pEquip);
			}

			pNewItem = pEquip;

			//pTmpItem = pEquip;
			// Jason 2010-4-22 法宝偏址
			//tagEquipProto * pEquipProto = g_attRes.GetEquipProto(pTmpItem->dwTypeID);
			//INT size_of = nEquipSize;
			//if( P_VALID(pEquipProto) )
			//{
			//	if( MIsFaBaoType(pEquipProto->eType) )
			//		size_of = SIZE_FABAO;
			//}

			//pTmpItem = (tagEquip*)((BYTE*)pTmpItem + size_of);

			// 先发消息,通知DBServer从item_baibao表中删除物品，以免出现因宕机等产生的复制现象
			if(P_VALID(pNewItem) && P_VALID(pNewItem->pProtoType))
			{
				sendEquip.n64Serial = pNewItem->n64Serial;
				g_dbSession.Send(&sendEquip, sendEquip.dwSize);

				// 记录log //??
			}
		}

		p += *pLen;

		if(!P_VALID(pNewItem)  || !P_VALID(pNewItem->pProtoType))
		{
			ASSERT(P_VALID(pNewItem->pProtoType));
			MAttResCaution(_T("item/equip"), _T("typeid"), pTmpItem->dwTypeID);
			IMSG(_T("The item(SerialNum: %lld) hasn't found proto type!"), pTmpItem->n64Serial);			
			
			::Destroy(pNewItem);
			continue;
		}

		pRole->GetItemMgr().Add2BaiBao(pNewItem, ELCID_BaiBao_LoadFromDB, TRUE);
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 更新百宝袋元宝数量
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleBaiBaoYuanBaoLoad(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadBaiBaoYuanBao);

	// 找到指定账号的在线玩家
	PlayerSession* pPlayerSession = g_worldSession.FindSession(pRecv->dwAccountID);

	// 账号不在线
	if (!P_VALID(pPlayerSession))
		return GT_INVALID;

	// 检查玩家是否游戏中
	Role* pRole = pPlayerSession->GetRole();
	if (P_VALID(pRole))
	{
		// 重新设置玩家百宝元宝的数量(这里只会增加玩家元宝数量)
		INT32 nCurYuanBao = pRole->GetCurMgr().GetBaiBaoYuanBao();
		ASSERT(pRecv->nBaiBaoYuanBao >= nCurYuanBao);
		pRole->GetCurMgr().IncBaiBaoYuanBao(pRecv->nBaiBaoYuanBao - nCurYuanBao, ELCID_BaiBao_Bill_YuanBao, FALSE);
	}
	// 是否在选人界面
	else if (P_VALID(g_worldSession.FindGlobalSession(pRecv->dwAccountID)))
	{
		ASSERT(pRecv->nBaiBaoYuanBao >= 0);
		pPlayerSession->SetBaiBaoYB(pRecv->nBaiBaoYuanBao);
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 名人堂声望排名更新
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRepRankLoad(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_GetRepRankList);

	if(pRecv->dwErrorCode != E_Success || 0 == pRecv->nRecNum)
	{
		return pRecv->dwErrorCode;
	}
	g_fameHall.HandleUpdateRepRank(pMsg);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 读取所有玩家的元宝交易账户
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadAllYBAccount(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadAllYBAccount);

	// 开始加载交易账户
	tagYBAccount* pYBAccount = &pRecv->Account[0];
	for(INT n = 0; n < pRecv->nNum; n++)
	{
		g_tradeYB.CreateTradeAccount(pYBAccount);
		pYBAccount++;
	}

	this->OutPutInitStepInfo(EDBS_LoadAllYBAccount,TRUE);

	return 0;
}

//------------------------------------------------------------------------------------------------
// 读取所有玩家的元宝交易订单
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadAllYBOrder(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadAllOrder);

	// 开始加载交易账户
	g_tradeYB.SetMaxOrderIndex(pRecv->dwMaxOrderID + 1);
	tagYuanBaoOrder* pYBOrder = &pRecv->YBOrder[0];
	for(INT n = 0; n < pRecv->nNum; n++)
	{
		g_tradeYB.LoadYOOrder(pYBOrder);
		pYBOrder++;
	}

	this->OutPutInitStepInfo(EDBS_LoadAllOrder,TRUE);


	return 0;	
}

//------------------------------------------------------------------------------------------------
// 读取一个玩家的元宝交易订单
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRoleGetYBOrder(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_GetRoleYBOrder);

	INT			nNum = pRecv->nNum;
	DWORD		dwSize = sizeof(tagNS_GetYuanbaoOrder);
	tagYuanBaoOrder* pNDBSOrder = &pRecv->YBOrder[0];
		
	Role *pRole = g_roleMgr.GetRolePtrByID(pNDBSOrder->dwRoleID);
	if(!P_VALID(pRole))
		return GT_INVALID;

	if( nNum > 0 )
	{
		dwSize += (nNum-1) * sizeof(tagYuanBaoOrder);
	}

	MCREATE_MSG(pSend, dwSize, NS_GetYuanbaoOrder);

	// 开始加载交易账户
	tagYuanBaoOrder* pNCOrder = &pSend->Orders[0];
	for(INT n = 0; n < pRecv->nNum; n++)
	{
		*pNCOrder = *pNDBSOrder;
		pNDBSOrder++;
		pNCOrder++;
	}
	pSend->nNum = nNum;
	pRole->SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return 0;
}

//------------------------------------------------------------------------------------------------
// 读取指定账户百宝袋历史记录
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleBaiBaoLoadLog(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadBaiBaoLog);

	if (pRecv->dwErrorCode != E_Success)
		return GT_INVALID;

	PlayerSession* pSession = g_worldSession.FindSession(pRecv->dwAccountID);
	if( !P_VALID(pSession) ) return GT_INVALID;

	// 给客户端发送返回消息
	INT nSize = pRecv->dwSize - sizeof(tagNDBS_LoadBaiBaoLog) + sizeof(tagNS_InitBaiBaoRecord);
	MCREATE_MSG(pSend, nSize, NS_InitBaiBaoRecord);

	pSend->n16Num = pRecv->nLogNum;
	IFASTCODE->MemCpy(pSend->byData, pRecv->byData, pRecv->dwSize - sizeof(tagNDBS_LoadBaiBaoLog) + sizeof(pRecv->byData));

	pSession->SendMessage(pSend, pSend->dwSize);
	MDEL_MSG(pSend);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 声望重置时间获取
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleRepRstTimeStamp(tagNetCmd* pMsg, DWORD)
{
	g_fameHall.HandleInitRepRstTimeStamp(pMsg);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 获取名人堂进入快照
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleGetFameHallEnterSnap(tagNetCmd* pMsg, DWORD)
{
	g_fameHall.HandleInitFameHallTop50(pMsg);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 获取已激活氏族珍宝
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleGetActTreasureList(tagNetCmd* pMsg, DWORD)
{
	g_fameHall.HandleInitActTreasureList(pMsg);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 处理帮派属性反馈
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuild(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(p, pMsg, NDBS_LoadAllGuild);

	g_guildMgr.InitDBGuild(&p->sGuildLoad);
	// Jason 2010-2-20 v1.3.2帮派排名
	ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
	if( P_VALID(pToolkit) )
	{
		if (p->sGuildLoad.dwLeaderRoleID != 0)
		{
			pToolkit->AddGuild(p->sGuildLoad.dwID);
		}
		
	}

	return E_Success;
}

DWORD DBSession::HandleCreateGuild(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(p, pMsg, NDBS_CreateGuild);

	return E_Success;

}

DWORD DBSession::HandleInsertGuildName(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_InsertGuildName);

	DWORD dwErrorCode = pRecv->dwError;
	Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleId);
	if ( !P_VALID(pRole) )
	{
		return  dwErrorCode;
	}
	if(E_Success == dwErrorCode)
	{

		INT32 nNameTCHAR = (pRecv->dwSize - FIELD_OFFSET(tagNDBS_InsertGuildName, szGuildName)) / sizeof(TCHAR);
		dwErrorCode = g_guildMgr.CreateGuild(pRole, pRecv->szGuildName, nNameTCHAR);
		if( dwErrorCode == E_Guild_Create_Success )
		{
			ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
			if( P_VALID(pToolkit) && P_VALID(pRole) )
			{
				tstring szTmpGuildName(pRecv->szGuildName, nNameTCHAR);
				// 是否与已建帮派重名
				DWORD dwNewGuildID = g_world.LowerCrc32(szTmpGuildName.c_str());
				pToolkit->AddGuild(dwNewGuildID);
			}
		}
	}

	// 向客户端反馈
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(pRole->GetSession(), dwErrorCode);
	}
	return dwErrorCode;
}


DWORD DBSession::HandleLoadAllEmptyGuild(tagNetCmd* pMsg, DWORD)
{
    MGET_MSG(p, pMsg, NDBS_LoadAllEmptyGuild);
	if ( P_VALID(p))
	{
		for ( INT i = 0 ; i < p->nEmptyGuildNum ; i++)
		{
			Guild* pGuild = new Guild;
			if ( P_VALID( pGuild ) )
			{
			    pGuild->SetGuildAtt(&p->sGuildLoad[i]);
				g_guildMgr.GetMapGuild().Add(p->sGuildLoad[i].dwID,pGuild);
			}
		}
	}
	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 处理帮派成员属性反馈
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuildMember(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(p, pMsg, NDBS_LoadAllGuildMember);

	if(p->nAllGuildNum <= 0)
	{
		if(g_guildMgr.GetGuildNum() > 0)
		{
			ASSERT(0);
			this->OutPutInitStepInfo(EDBS_LoadAllGuildMember,FALSE);
			g_world.ShutDown();
			return GT_INVALID;
		}

		return E_Success;
	}
	else
	{
		if(g_guildMgr.GetGuildNum() <= 0)
		{
			ASSERT(0);
			this->OutPutInitStepInfo(EDBS_LoadAllGuild,FALSE);
			g_world.ShutDown();
			return GT_INVALID;
		}
	}

	g_guildMgr.InitDBGuildMember(p->sGuildMemberLoad, p->nAllGuildNum);

	this->OutPutInitStepInfo(EDBS_LoadAllGuildMember,TRUE);
	// 发送帮派初始化申请完成消息
// 	tagNDBC_GuildInitOK send;
// 	Send(&send, send.dwSize);	
	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 处理帮派仓库物品反馈
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuildWareItems(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadGuildWareItems);

	// 帮派ID验证
	if (!GT_VALID(pRecv->dwGuildID))
		return GT_INVALID;

	// 物品信息错误
	if (pRecv->nItemNum < 0)
		return GT_INVALID;

	// 取得帮派
	Guild* pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if (!P_VALID(pGuild))
	{
		// 帮派不存在
		return GT_INVALID;
	}

	pGuild->GetGuildWarehouse().LoadWarehouseItems(pRecv->byData, pRecv->nItemNum);

	if (!pGuild->GetGuildWarehouse().IsInitOK())
	{
		this->OutPutInitStepInfo(EDBS_LoadGuildWareItems,FALSE, pRecv->dwGuildID, pRecv->nItemNum);
	}


	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 载入帮派的设施升级信息
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuildUpgradeInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadFacilitiesInfo);

	if (!GT_VALID(pRecv->dwGuildID) || pRecv->nInfoNum < 0)
	{
		return GT_INVALID;
	}

	// 取得帮派
	Guild* pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	if (!pGuild->GetGuildFacilities().LoadFacilitiesInfo(pRecv->sFacilitiesInfo, pRecv->nInfoNum))
	{
		return GT_INVALID;
	}

	if (!pGuild->GetGuildFacilities().IsInitOK())
	{
		this->OutPutInitStepInfo(EDBS_LoadGuildUpgradeInfo,FALSE,pRecv->dwGuildID,pRecv->nInfoNum);
	}


	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 载入帮派技能信息
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuildSkillInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadGuildSkillInfo);

	if (!GT_VALID(pRecv->dwGuildID) || pRecv->nInfoNum <= 0)
	{
		return GT_INVALID;
	}

	// 取得帮派
	Guild* pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	if (!pGuild->GetGuildSkill().LoadGuildSkillIInfo(pRecv->sGuildSkillInfo, pRecv->nInfoNum))
	{
		return GT_INVALID;
	}

	if (!pGuild->GetGuildSkill().IsInitOK())
	{
		this->OutPutInitStepInfo(EDBS_LoadGuildSkillIInfo,FALSE, pRecv->dwGuildID, pRecv->nInfoNum);
	}


	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 载入帮派跑商信息
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadGuildCommerceInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadCommodity);

	if (!GT_VALID(pRecv->dwGuildID) || pRecv->nCommodityNum < 0)
	{
		return GT_INVALID;
	}

	// 取得帮派
	Guild* pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagGuildCommerceInfo* pInfo = NULL;
	if (pRecv->nCommodityNum > 0)
	{
		pInfo = pRecv->sCommerceInfo;
	}
	if (!pGuild->GetGuildCommerce().LoadCommerceInfo(pInfo, pRecv->nCommodityNum))
	{
		return GT_INVALID;
	}

	if (!pGuild->GetGuildCommerce().IsInitOK())
	{
		this->OutPutInitStepInfo(EDBS_LoadGuildCommerceInfo,FALSE, pRecv->dwGuildID, pRecv->nCommodityNum);
	}


	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 载入帮派跑商排名信息
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadCommerceRankInfo(vEngine::tagNetCmd *pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadCommerceRank);

	if (!GT_VALID(pRecv->dwGuildID) || pRecv->nRankNum < 0)
	{
		return GT_INVALID;
	}

	// 取得帮派
	Guild* pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagCommerceRank* pInfo = NULL;
	if (pRecv->nRankNum > 0)
	{
		pInfo = pRecv->sRankInfo;
	}
	if (!pGuild->GetGuildCommerce().LoadCommerceRankInfo(pInfo, pRecv->nRankNum))
	{
		return GT_INVALID;
	}

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 检验帮派初始化结果
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleGuildInitOK(tagNetCmd* pMsg, DWORD)
{
	if (!g_guildMgr.IsGuildInitOK())
	{
		// 关闭服务器
		ASSERT(0);
		ILOG->Write(_T("Guild init failed! Please check db!\n"));
		g_world.ShutDown();
		return GT_INVALID;
	}


	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 创建宠物
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleCreatePetSoul( tagNetCmd* pMsg, DWORD )
{
	MGET_MSG(pRecv, pMsg, NDBS_CreatePetSoul);

	MTRANS_ELSE_RET(pRole, g_roleMgr.GetRolePtrByID(pRecv->petDataLoad.petAtt.dwMasterID), Role, GT_INVALID);

	DWORD dwRtv = E_Success;

	if (E_Success != pRecv->dwErrCode)
	{
		dwRtv = E_Pets_Soul_CreateDBErr;
		goto EXIT;
	}

	// 初始化
	const BYTE* pData = (BYTE*)(&pRecv->petDataLoad);
	PetSoul* pSoulLoad = PetSoul::CreateSoulByDBData(pData, TRUE);
	if (!GT_VALID(pSoulLoad))
	{
		dwRtv = E_Pets_Soul_CreateInitErr;
		goto EXIT;
	}

	// 能否放入宠物带
	dwRtv = pRole->GetPetPocket()->CanPutIn(pSoulLoad);
	if (E_Success != dwRtv)
		goto EXIT;

	DWORD dwPetTypeID = 0;
	tagItem * pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Serial);
	if( !P_VALID(pItem) )
	{
		dwRtv = E_Pets_Soul_PetEggErr;
		goto EXIT;
	}
	dwPetTypeID = pItem->pProtoType->nSpecFuncVal1;

	// 消耗宠物蛋
	if (E_Success != pRole->GetItemMgr().ItemUsedFromBag(pRecv->n64Serial, 1, ELCLD_PET_ITEM_CREATE_PET))
	{
		ILOG->Write(_T("Delete pet egg err:typeid %d\r\n"), pItem->pProtoType->dwTypeID);
		dwRtv = E_Pets_Soul_PetEggErr;
		goto EXIT;
	}

	// Jason 2010-3-4 v1.4.0 可骑乘pet自动进入预备驾驭状态
	PetPocket * pPetPocket = pRole->GetPetPocket();

	const tagPetProto * pPetProto = g_attRes.GetPetProto(dwPetTypeID);
	if( !P_VALID(pPetProto) )
	{
		ILOG->Write(_T("Delete pet egg err:typeid %d dwPetTypeID:%d\r\n"), pItem->pProtoType->dwTypeID, dwPetTypeID);
		dwRtv = E_Pets_Soul_PetEggErr;
		goto EXIT;
	}

	// 放入宠物带
	dwRtv = pRole->GetPetPocket()->PutIn(pSoulLoad, FALSE, TRUE);
	if (E_Success != dwRtv)
		goto EXIT;

	if( pPetProto->nMountSpeed == 0 )
	{
		// 召唤
		pRole->GetPetPocket()->CallPet(pSoulLoad->GetID());
	}
	else
	{
		// Jason 2010-3-4 v1.4.0 可骑乘pet自动进入预备驾驭状态
		if( P_VALID(pPetPocket) )
		{
			//pPetPocket->CancleLastPetPreparingStatus();
			//DWORD dwRet = pPetPocket->CanRidePet();
			//if( E_UseSkill_Mount_NoPreparingPet == dwRet ||
			//	E_UseSkill_SelfStateLimit == dwRet ||
			//	E_Pets_Success == dwRet )
			{
				pPetPocket->PreparePet(pSoulLoad->GetID());
			}
		}
	}

	// 打log
	PetSoul::LogPet(pRole->GetID(), pSoulLoad->GetID(), ELCLD_PET_CREATE_PET);

EXIT:
	// 若出错 则从数据库删除
	if (E_Pets_Success != dwRtv)
	{
		ILOG->Write(_T("Can not create pet soul when dbpet created petid:%u, masterid:%u, errcode:%u\n"), pRecv->petDataLoad.petAtt.dwPetID, pRole->GetID(), dwRtv);
		PetSoul::LogPet(pRole->GetID(), pRecv->petDataLoad.petAtt.dwPetID, ELCLD_PET_DEL_PET_CREATE);

		if(P_VALID(pSoulLoad))
			PetSoul::DeleteSoul(pSoulLoad, TRUE);
	}
		
	// 给客户端回消息
	tagNS_UsePetEgg send;
	send.dwErrCode = dwRtv;
	send.n64ItemID = pRecv->n64Serial;
	pRole->SendMessage(&send, send.dwSize);

	return E_Success;
}

DWORD DBSession::HandleLoadAllGPInfo( tagNetCmd* pMsg, DWORD )
{
	MGET_MSG(pRecv, pMsg, NDBS_GetAllGPInfo);

	if (pRecv->dwErrorCode != E_Success)
	{
		this->OutPutInitStepInfo(EDBS_LoadAllGPInfo,FALSE);

		return pRecv->dwErrorCode;
	}

	DWORD dwErrorCode = g_mall.LoadAllGPInfo(pRecv->nGPInfoNum, (LPVOID)(pRecv->GPInfoData));
	if(dwErrorCode != E_Success)
	{
		this->OutPutInitStepInfo(EDBS_LoadAllGPInfo,FALSE);
		return dwErrorCode;
	}

	this->OutPutInitStepInfo(EDBS_LoadAllGPInfo,TRUE);

	return dwErrorCode;
}

DWORD DBSession::HandleLoadLimitItemInfo(tagNetCmd* pMsg, DWORD)
{
     MGET_MSG(pRecv, pMsg, NDBS_LoadLimitItemInfo);
	 
     DWORD dwErrorCode = g_mall.InitLimitNumItemInfo(pRecv->nNum, (LPVOID)(pRecv->LimitItemInfo));
	 if(dwErrorCode != E_Success)
	 {
		 this->OutPutInitStepInfo(EDBS_LoadAllLimitNumItemInfo,FALSE);
		 return dwErrorCode;
	 }
	 this->OutPutInitStepInfo(EDBS_LoadAllLimitNumItemInfo,TRUE);

     return E_Success;
}

DWORD DBSession::HandleLoadRoleVCard( tagNetCmd* pMsg, DWORD )
{
	MGET_MSG(pRecv, pMsg, NDBS_GetOffLineVCard);

	if (E_Success != pRecv->dwErrCode)
	{
		return E_Success;
	}

	Role* pQuery = g_roleMgr.GetRolePtrByID(pRecv->dwQueryID);
	if (!P_VALID(pQuery))
	{
		return E_Success;
	}

	tagRoleVCard tmpVCard;

	const BYTE* pByte1 = pRecv->byData;
	tmpVCard.Init(pByte1, NULL);

	tmpVCard.SendVCard2Client(pRecv->dwQueryID);

	return E_Success;
}

DWORD DBSession::HandleLoadVIPStallInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_GetAllVIPStallInfo);

	if (pRecv->dwErrorCode != E_Success)
	{
		this->OutPutInitStepInfo(EDBS_LoadAllVIPStallInfo,FALSE);
		return pRecv->dwErrorCode;
	}

	DWORD dwErrorCode = g_VIPStall.LoadAllVIPStallInfo(pRecv->VipStallInfo);
	if(dwErrorCode != E_Success)
	{
		this->OutPutInitStepInfo(EDBS_LoadAllVIPStallInfo,FALSE);
		return dwErrorCode;
	}

	this->OutPutInitStepInfo(EDBS_LoadAllVIPStallInfo,TRUE);

	return dwErrorCode;
}

//------------------------------------------------------------------------------------------------
// 读取固定活动数据
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadActivityData(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadActivityData);

	if(pRecv->nNum == 0) return 0;

	// 开始固定活动脚本数据
	tagActivityData* pActivityData = &pRecv->Activity[0];
	for(INT n = 0; n < pRecv->nNum; n++)
	{
		CActivity* pActivity = g_activityMgr.GetActivity(pActivityData->dwID);
		if(P_VALID(pActivity))
			pActivity->InitScriptData(pActivityData);

		pActivityData++;
	}

	this->OutPutInitStepInfo(EDBS_LoadActivityData,TRUE);

	return 0;
}

DWORD DBSession::HandleLoadVNBData( tagNetCmd* pMsg, DWORD )
{
	MTRANS_POINTER(pData, pMsg, tagNDBS_GetVNBData);
	if(EVMT_FirstOne == pData->eType)
		g_VipNetBarMgr.InitData(&pData->players, TRUE);	
	else
		g_VipNetBarMgr.InitData(&pData->players);

	if (EVMT_LastOne == pData->eType)
	{
		this->OutPutInitStepInfo(EDBS_LoadGetVNBData,TRUE);
	}

	return E_Success;
}

DWORD DBSession::HandleInitRankings(tagNetCmd* pMsg, DWORD)
{
	MTRANS_POINTER(pData, pMsg, tagNDBS_InitRankings);

	g_WorldRankingMgr.DoInit(pData);

	this->OutPutInitStepInfo(EDBS_LoadRanking,TRUE);

	return E_Success;
}

DWORD DBSession::HandleReloadRankings(tagNetCmd* pMsg, DWORD)
{
	g_WorldRankingMgr.Init();

	return E_Success;
}

DWORD DBSession::HandleInitPetRankings(tagNetCmd* pMsg, DWORD)
{
	MTRANS_POINTER(pData, pMsg, tagNDBS_InitPetRankings);
	g_WorldRankingMgr.DoInitPetRankings( pData );

	this->OutPutInitStepInfo(EDBS_LoadRanking,TRUE);

	return E_Success;
}

DWORD DBSession::HandleInitFlowerRankings(tagNetCmd* pMsg, DWORD)
{
	MTRANS_POINTER(pData, pMsg, tagNDBS_InitFlowerRankings);
	if(pData->eType == ERT_Flower)
		g_WorldRankingMgr.DoInitFlowerRankings( pData );
	else if(pData->eType == ERT_Egg)
		g_WorldRankingMgr.DoInitEggRankings( pData );


	return E_Success;
}

DWORD DBSession::HandleRetUpdateRankings(tagNetCmd* pMsg, DWORD)
{
	MTRANS_POINTER(pData, pMsg, tagNDBS_UpdateRankings);

	g_WorldRankingMgr.SetUpdateMark(pData->eType, TRUE);

	return E_Success;
}

DWORD DBSession::HandleRenewAdvanceStrength(tagNetCmd* pMsg, DWORD)
{
	g_roleMgr.RenewAdvanceStrength();

	return E_Success;
}

DWORD DBSession::HandleLoadLeftMsg( tagNetCmd* pMsg, DWORD )
{
	g_msgMgr.HandleLoadLeftMsg(pMsg);

	return E_Success;
}

DWORD DBSession::HandleGuildMemberInitOk( tagNetCmd* pMsg, DWORD )
{
	tagNDBC_GuildInitOK send;
	g_dbSession.Send(&send, send.dwSize);

	return E_Success;
}

DWORD DBSession::HandleExternalLinksList(tagNetCmd* pMsg, DWORD)
{
	tagNDBS_GetExternalLinks * p = (tagNDBS_GetExternalLinks*)pMsg;
	if( P_VALID(p) )
	{
		RefreshExternalLinksList(p);
	}

	this->OutPutInitStepInfo(EDBS_ExternalLinksList,TRUE);

	return E_Success;
}


VOID DBSession::SendMsgToPlayerForExtLinks(Role * ply)
{
	m_elExternalLinks->SendMessageToPlayer  (ply);
}

VOID DBSession::RefreshExternalLinksList(tagNetCmd * p1)
{
	tagNDBS_GetExternalLinks * pCmd = (tagNDBS_GetExternalLinks *)p1;
	if( P_VALID(pCmd) )
	{
		m_lstExterLinks.Clear  ();
		tagSExternalLink t;
		tagExternalLink * p = pCmd->links;
		for( int i = 0 ; i < pCmd->dwElementNum ; ++i )
		{
			t.LinkName	= p[i].linkName;
			t.LinkURL	= p[i].linkValue;
			m_lstExterLinks.PushBack  (t);
		}
		if( pCmd->dwElementNum > 0 )
			m_elExternalLinks->FillExternalLinks(m_lstExterLinks);
	}
}

VOID DBSession::UpdateExternalLinksList(VOID)
{
	//if( 0 == (DWORD)m_timLastPullExtLink )
	//{
	//	m_timLastPullExtLink = g_world.GetWorldTime  ();
	//	tagNDBC_GetExternalLinks msg;
	//	Send(&msg,msg.dwSize);
	//}
	//else
	if( 0 < m_timLastPullExtLink && m_timLastPullExtLink != GT_INVALID )
	{
		tagDWORDTime t = g_world.GetWorldTime  ();
		if( CalcTimeDiff(t,m_timLastPullExtLink) / 60 >= 60 )
		{
			m_timLastPullExtLink = t;
			tagNDBC_GetExternalLinks msg;
			Send(&msg,msg.dwSize);
		}
	}
}

VOID DBSession::InitialExternalLinksList(VOID)
{
	m_timLastPullExtLink = g_world.GetWorldTime  ();
	tagNDBC_GetExternalLinks msg;
	Send(&msg,msg.dwSize);
}

//------------------------------------------------------------------------------------------------
// 读取城市信息
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadCityInfo(tagNetCmd *pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadCityInfo);

	City* pCity = g_cityMgr.GetCity(pRecv->sCityAtt.dwCityID);
	if (!P_VALID(pCity))
	{
		this->OutPutInitStepInfo(EDBS_InitCity,FALSE);
		return GT_INVALID;
	}

	if (!pCity->LoadCityInfo(&(pRecv->sCityAtt), pRecv->nSignUpNum, pRecv->sSignUp))
	{
		this->OutPutInitStepInfo(EDBS_InitCity,FALSE);
		return GT_INVALID;
	}
	this->OutPutInitStepInfo(EDBS_InitCity,TRUE);	

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 读取申请管理城市帮派的声望
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleGetGuildClanValue(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_GetGuildClanValue);

	City* pCity = g_cityMgr.GetCity(pRecv->eClanType);
	if (!P_VALID(pCity))
		return GT_INVALID;

	pCity->ConfirmAttackForce(pRecv->nGuildNum, pRecv->sClanValue);

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 读取师徒信息
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadMasterApprenticeData(tagNetCmd* pMsg, DWORD)
{

	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 读取数据库中存储的强制脱离师门申请列表
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadForceBreakOutApprenticeList(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadForceBreakOutApprenticeList);

	if (pRecv->dwForceBreakOutApprenceCnt <= 0)
	{
		return GT_INVALID;
	}

	g_forceBreakOutApprenticeMgr.InitApprenticeList(pRecv->dwForceBreakOutApprenceCnt, static_cast<tagForceBreakOutApprentice*>(pRecv->sForceBreakOutApprentice));
	
	return E_Success;
}


//------------------------------------------------------------------------------------------------
// 读小龙女查询表信息
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadShowPlayerData(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadShowPlayerData);
	if( !P_VALID(pRecv) )
	{
		return  GT_INVALID;
	}
    if( GT_VALID( pRecv->dwCount ) && 0 == pRecv->dwCount)
	{
		return GT_INVALID;
	}
    for (DWORD  dwI = 0 ; dwI < pRecv->dwCount ; dwI++)
    {
		
		if ( !GT_VALID(pRecv->data[dwI].dwAthletePoint) )
		{
			pRecv->data[dwI].dwAthletePoint = 0;   
		}
		tagShowPlayer* pNewData = new tagShowPlayer;
		if ( !P_VALID(pNewData))
		{
			IMSG(_T("LoadShowPlayer Allocate Memory failed, id=%u\r\n"), pRecv->dwCount);
			return GT_INVALID;
		}
		tagShowPlayer* pDataTemp = g_DraftShow.GetShowPlayer(pRecv->data[dwI].dwAthleteSignId);
		if ( P_VALID(pDataTemp))
		{
			 SAFE_DEL(pNewData);
             continue;
		}
		else
		{
			pNewData->dwAthleteSignId = pRecv->data[dwI].dwAthleteSignId;
			pNewData->dwAthleteRoleId = pRecv->data[dwI].dwAthleteRoleId;
			if (pRecv->data[dwI].dwAthleteRoleId != 0 && GT_VALID(pRecv->data[dwI].dwAthleteRoleId))
			{
               g_roleMgr.GetRoleNameByNameID(pRecv->data[dwI].dwAthleteRoleId,pNewData->tcAthleteRoleName);  
			}
			else
			{
                _tcscpy(pNewData->tcAthleteRoleName,pRecv->data[dwI].tcAthleteRoleName);
			}	
			pNewData->dwAthletePoint  = pRecv->data[dwI].dwAthletePoint;
			pNewData->dwAthleteSerialId = pRecv->data[dwI].dwAthleteSerialId;
		}
		
		g_DraftShow.AddShowPlayer(pNewData);

    }
	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 龙卫士女查询表信息
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadVotePlayerData(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadVotePlayerData);
	if( !P_VALID(pRecv) )
	{
       return  GT_INVALID;
	}
	if( GT_VALID( pRecv->dwCount ) && 0 == pRecv->dwCount)
	{
		return GT_INVALID;
	}

	for (DWORD  dwI = 0 ; dwI < pRecv->dwCount ; dwI++)
	{

		
		if ( !GT_VALID(pRecv->data[dwI].dwVotePoint) )
		{
             pRecv->data[dwI].dwVotePoint = 0;   
		}
		tagVotePlayer* pNewData = new tagVotePlayer;
		if ( !P_VALID(pNewData))
		{
			IMSG(_T("LoadVotePlayer Allocate Memory failed, id=%u\r\n"), pRecv->dwCount);
			return GT_INVALID;
		}
		tagVotePlayer* pTempData = g_DraftShow.GetVotePlayer(pRecv->data[dwI].dwVoteRoleId);
		if ( P_VALID(pTempData) )
		{
			SAFE_DEL(pNewData);
			continue;

		}
		else
		{
			pNewData->dwVoteRoleId = pRecv->data[dwI].dwVoteRoleId;
			if (pRecv->data[dwI].dwVoteRoleId != 0 && GT_VALID(pRecv->data[dwI].dwVoteRoleId))
			{
				g_roleMgr.GetRoleNameByNameID(pRecv->data[dwI].dwVoteRoleId,pNewData->tcVoteRoleName);  
			}
			else
			{
				_tcscpy(pNewData->tcVoteRoleName,pRecv->data[dwI].tcVoteRoleName);
			}
			pNewData->dwVotePoint  = pRecv->data[dwI].dwVotePoint;
			pNewData->dwSerialId   = pRecv->data[dwI].dwSerialId;
			g_DraftShow.AddVotePlayer(pNewData);


		}
		
	}

    return E_Success;
}

//------------------------------------------------------------------------------------------------
// 活动排行表
//------------------------------------------------------------------------------------------------
DWORD DBSession::HandleLoadActiveRankData(tagNetCmd* pMsg, DWORD)
{
	
	MGET_MSG(pRecv, pMsg, NDBS_LoadActiveRankData);
	if( !P_VALID(pRecv) )
	{
		return  GT_INVALID;
	}
	if( GT_VALID( pRecv->dwCount ) && 0 == pRecv->dwCount)
	{
		return GT_INVALID;
	}

	for (DWORD  dwI = 0 ; dwI < pRecv->dwCount ; dwI++)
	{

		tagActiveRankData* pNewData = new tagActiveRankData;
		if ( !P_VALID(pNewData))
		{
			IMSG(_T("ActiveRankData Allocate Memory failed, id=%u\r\n"), pRecv->dwCount);
			return GT_INVALID;
		}
		tagActiveRankData* pTempData = g_WorldExpo.GetActiveRankData(pRecv->data[dwI].dwRoleId);
		if ( P_VALID(pTempData) )
		{
			SAFE_DEL(pNewData);
			continue;
			
		}
		else
		{
			memcpy(pNewData,&pRecv->data[dwI],sizeof(tagActiveRankData));
			g_WorldExpo.AddActiveRankData(pNewData);


		}

	}
	return E_Success;
}

//------------------------------------------------------------------------------------------------
// 打印初始化信息
//------------------------------------------------------------------------------------------------
VOID DBSession::OutPutInitStepInfo(EDBSessionInitStep stp, BOOL bSuccess,DWORD dwPara1, DWORD dwPara2)
{
	tstringstream strInfo;
	if(bSuccess)
	{
		strInfo << _T("Success!	DBSession ");
	}
	else
	{
		strInfo << _T("Failed!	 DBSession ");
	}


	switch(stp)
	{
	case (EDBS_LoadAllLimitNumItemInfo) :
		{
			strInfo << _T("Update Mall Limit Num Item Info!\n");	
    	}
		break;
	case (EDBS_ItemInfo):
		{
			strInfo << _T("Load Item Info!\n");	
		}
		break;
	case (EDBS_ItemNeedLog):
		{
			//  EDBS_ItemNeedLog，虽然bSuccess返回是FALSE，但其实是数据库没数据而已
			strInfo << _T("Load Item Need Log!\n");
		}
		break;
	case (EDBS_LoadAllRoleInfo):
		{
			strInfo << _T("Load All Role Info!\n");	
		}
		break;
	case (EDBS_LoadAllYBAccount):
		{
			strInfo << _T("Load All YBAccount!\n");
		}
		break;
	case (EDBS_LoadAllOrder):
		{
			strInfo << _T("Load All Order!\n");	
		}
		break;
	case (EDBS_LoadAllGuild):
		{
			strInfo << _T("Load All Guild!\n");	
		}
		break;
	case (EDBS_LoadAllGuildMember):
		{
			strInfo << _T("Load All Guild Member!\n");
		}
		break;
	case (EDBS_LoadGuildUpgradeInfo):
		{
			if(bSuccess != TRUE)
			{
				strInfo << _T("Guild Upgradeinfo Load!<guildid:%u>, <infonum:%u> Please check db!\n");
			}
			else
			{
				strInfo << _T("Guild Upgradeinfo Load!\n");
			}
		}
		break;
	case (EDBS_LoadGuildSkillIInfo):
		{
			if(bSuccess != TRUE)
			{
				strInfo << _T("Guild Skills Load!<guildid:%u>, <skillnum:%u> Please check db!\n");
			}
			else
			{
				strInfo << _T("Guild Skills Load!\n");
			}
		}
		break;
	case (EDBS_LoadGuildCommerceInfo):
		{
			if(bSuccess != TRUE)
			{
				strInfo << _T("Guild Commerceinfo Load!<guildid:%u>, <commoditynum:%u> Please check db!\n");
			}
			else
			{
				strInfo << _T("Guild Commerceinfo Load!\n");
			}
		}
		break;
	case (EDBS_LoadGuildWareItems):
		{
			if(bSuccess != TRUE)
			{
				strInfo << _T("Guild Warehouses Load!<guildid:%u>, <itemnum:%u> Please check db!\n");
			}
			else
			{
				strInfo << _T("Guild Warehouses Load!\n");
			}
		}
		break;
	case (EDBS_InitCity):
		{
			strInfo << _T("Init City!\n");
		}
		break;
	case (EDBS_LoadAllGPInfo):
		{
			strInfo << _T("Load All GP Info!\n");
		}
		break;
	case (EDBS_LoadAllVIPStallInfo):
		{
			strInfo << _T("Load All VIP Stall Info!\n");
		}
		break;
	case (EDBS_LoadActivityData):
		{
			strInfo << _T("Load All Activity Data!\n");
		}
		break;
	case (EDBS_LoadRanking):
		{
			strInfo << _T("Load Ranking!\n");
		}
		break;
	case (EDBS_LoadGetVNBData):
		{
			strInfo << _T("Load Get VNB Data!\n");
		}
		break;
	case (EDBS_ExternalLinksList):
		{
			strInfo << _T("Load External Links List!\n");
		}
		break;
	case (EDBS_InitOk):
		{	
			strInfo << _T("LoongWorld Spend <%d> ms For Wait DB Init!\n");
		}
		break;
	default:
		goto ERROR_END;
		break;
	}

	if(dwPara1 != GT_INVALID &&  dwPara2 == GT_INVALID)
	{
		IMSG(strInfo.str().c_str(),dwPara1,dwPara2);
		ILOG->Write(strInfo.str().c_str(),dwPara1,dwPara2);
	}
	else if(dwPara1 != GT_INVALID &&  dwPara2 != GT_INVALID)
	{
		IMSG(strInfo.str().c_str(),dwPara1,dwPara2);
		ILOG->Write(strInfo.str().c_str(),dwPara1,dwPara2);
	}
	else if(dwPara1 == GT_INVALID &&  dwPara2 == GT_INVALID)
	{
		IMSG(strInfo.str().c_str());
		ILOG->Write(strInfo.str().c_str());
	}

//错误结束
ERROR_END:

	return;
}

DWORD DBSession::HandleGetAccountInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadAccountInfo);
	if( P_VALID(pRecv) )
	{
		if( 0 == pRecv->dwErrorCode )
		{
			PlayerAccount account;
			account.m_dwAccountID			= pRecv->dwAccountID;
			account.m_timeLastLoginTime		= pRecv->timeLastLogin;
			account.m_timeLastLogoutTime	= pRecv->timeLastLogout;
			account.m_timeLastReceiveDailyOfflineReward	= pRecv->timeLastReceiveDailyOfflineReward;
			account.m_timeLastReceiveRegression			= pRecv->timeLastReceiveRegression;
			g_PlayerAccountMgr.OnRecAccountInfo  (account);
			return E_Success;
		}
	}
	return GT_INVALID;
}

//从数据库加载快捷栏数据
DWORD DBSession::HandleLoadShortCut_QuickBar(tagNetCmd* pMsg, DWORD)
{
    const INT  BAR_COUNT        = 10 ;
	const INT  BAR_BUTTON_COUNT = 8 ;

	MGET_MSG(pRecv, pMsg, NDBS_LoadShortCut_QuickBar);

	if( P_VALID(pRecv) )
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if (!P_VALID(pRole))
		{
			return GT_INVALID;
		}

		for(int i = 0; i < BAR_COUNT ; i++ )
		{
			for(int j = 0; j < BAR_BUTTON_COUNT; j++)
		   {
			 const tagQuickData& quickbar = pRecv->QuickBarData[i*BAR_BUTTON_COUNT+j];
			 pRole->SetShortCutBarDataFlag(i,j,quickbar);
			 pRole->SetShortCutBarData(i,j,quickbar);
		   }
		}
		if ( pRole->GetShortCutBarReqFlag())
		{
			pRole->SendShortCutBarDataToClient();
		}
		pRole->SetLoadFromDBFlag(TRUE);
		return E_Success;
	}
	return GT_INVALID;
}

//校验是否在服务器存过数据
DWORD DBSession::HandleIsSavedShortCut_QuickBar(tagNetCmd* pMsg, DWORD)
{
    MGET_MSG(pRecv, pMsg, NDBS_IsSavedShortCut_QuickBar);

	if( P_VALID(pRecv) )
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if (!P_VALID(pRole))
		{
			return GT_INVALID;
		}

		
		pRole->SetIsSavedQuickBarData(pRecv->nIsSaved);
	}
    return 0;
}



DWORD DBSession::HandleGetSerialVal(tagNetCmd* pMsg, DWORD)
{

	MGET_MSG(pRecv, pMsg, NDBS_GetSerialVal);
	if( P_VALID(pRecv) )
	{
         g_AthleticsSystem.SetSystemInfoSerialVal(pRecv->dwSerialVal); 
	}
	return 0;

}

BOOL DBSession::HandleCheckName(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_CheckName);

	if (!P_VALID(pRecv)) return FALSE;
	
	Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pRole))
	{
		return FALSE;
	}

	pRecv->szNewName[X_SHORT_NAME-1] = 0;
	pRecv->szOldName[X_SHORT_NAME-1] = 0;
	if(pRecv->dwErrorCode != E_Success)
	{
		// 向客户端发消息
		tagNS_LPRename send;
		send.dwRoleID = pRecv->dwRoleID;
		send.dwErrorCode = pRecv->dwErrorCode;
		_tcscpy_s(send.szNewName, X_SHORT_NAME, pRecv->szNewName);
		_tcscpy_s(send.szOldName, X_SHORT_NAME, pRecv->szOldName);

		pRole->SendMessage(&send, send.dwSize);

		return FALSE;
	}

	// 改名
	pRole->ReName(pRecv->szNewName);

	//消耗物品
	if ( pRecv->dwErrorCode == E_Success )
	{
		tagItem *pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemSerial);
		if(!P_VALID(pItem))
		{
			return FALSE;
		}
		pRole->GetItemMgr().ItemUsedFromBag(pItem->n64Serial, 1, ELCLD_Item_Use);
	}

	// 向客户端发消息
	tagNS_LPRename send;
	send.dwRoleID = pRole->GetID();
	send.dwErrorCode = pRecv->dwErrorCode;
	_tcscpy_s(send.szNewName, X_SHORT_NAME, pRecv->szNewName);
	_tcscpy_s(send.szOldName, X_SHORT_NAME, pRecv->szOldName);
	pRole->SendMessage(&send, send.dwSize);

	// 向所有在线玩家发送消息
	tagNS_RoleRename broa;
	broa.dwRoleID = pRole->GetID();
	_tcscpy_s(broa.szNewName, X_SHORT_NAME, pRecv->szNewName);
	g_roleMgr.SendWorldMsg(&broa, broa.dwSize);

	return TRUE;
}

// 仇敌上线，通知玩家
BOOL DBSession::HandleGetRoleListOfEnemy(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_GetRoleListOfEnemy);

	if (!P_VALID(pRecv)) return FALSE;

	if (pRecv->dwRoleCount == 0)
	{
		return FALSE;
	}

	for (DWORD nCnt = 0; nCnt < pRecv->dwRoleCount; nCnt++)
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->pRoleIDData[nCnt]);
		if (!P_VALID(pRole))
		{
			continue;
		}
		if (pRecv->bEnemyLogin)
		{
			tagNS_EnemyLogin send;
			send.dwRoleID = pRecv->dwEnemyID;
			pRole->SendMessage(&send, send.dwSize);
		}
		else
		{
			g_socialMgr.DeleteEnemy(pRecv->dwEnemyID, pRecv->pRoleIDData[nCnt]);
		}
	}
	return TRUE;
}

// 载入数据库中的公会宣战关系
BOOL DBSession::HandleLoadGuildWarDeclare(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadGuildWarDeclare);

	if (!P_VALID(pRecv)) return FALSE;

	if (pRecv->dwGuildWarDeclareCnt <= 0)
		return FALSE;

	g_guildWarDeclareMgr.InitGuildWarDeclareMap(pRecv->dwGuildWarDeclareCnt, static_cast<tagGuildWarDeclare*>(pRecv->pGuildWarDeclare));

	return TRUE;
}

// 给角色添加天生技能
VOID DBSession::AddInitialSkill2Role(DWORD dwRoleID)
{
	DWORD dwInitialSkillCount = g_attRes.GetInitialSkillCount();
	if (dwInitialSkillCount <= 0)
	{
		return;
	}

	TMap<DWORD, tagInitialSkill*>& mapInitialSkill = g_attRes.GetInitialSkillMap();
	TMap<DWORD, tagInitialSkill*>::TMapIterator iterator = mapInitialSkill.Begin();
	tagInitialSkill* pInitialSkill = NULL;
	while (mapInitialSkill.PeekNext(iterator, pInitialSkill))
	{
		if (!P_VALID(pInitialSkill))
		{
			continue;
		}

		// 保存技能到数据库
		tagNDBC_AddSkill send;
		send.dwRoleID = dwRoleID;
		send.Skill.dwID = pInitialSkill->dwSkillID/100;
		send.Skill.nCoolDown = 0;
		send.Skill.nSelfLevel = 0;
		send.Skill.nLearnLevel = pInitialSkill->dwSkillID%100;
		send.Skill.nProficiency = 0;
		g_dbSession.Send(&send, send.dwSize);
	}
}

// 战场
BOOL DBSession::HandleLoadBattleFieldAttackSignUpInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadBattleFieldAttackSignUpInfo);

	if (!P_VALID(pRecv)) return FALSE;

	if (pRecv->dwInfoCnt <= 0)
		return FALSE;

	if (!P_VALID(CGodMiracleMgr::GetInst()))
		return FALSE;

	for (DWORD dwCnt = 0; dwCnt < pRecv->dwInfoCnt; dwCnt++)
	{
		CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle(pRecv->sSignUpGuild[dwCnt].eGodID);
		if (!P_VALID(pGodMiracle))
			continue;
		pGodMiracle->Add2SignUpGuildInfo(pRecv->sSignUpGuild[dwCnt].sSignUpGuildInfo.dwGuildID, pRecv->sSignUpGuild[dwCnt].sSignUpGuildInfo.dwBidFund);
	}
	return TRUE;
}

BOOL DBSession::HandleLoadAllicanceGuildInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadAllicanceGuildInfo);

	if (!P_VALID(pRecv)) return FALSE;

	if (pRecv->dwInfoCnt <= 0)
		return FALSE;

	if (!P_VALID(CGodMiracleMgr::GetInst()))
		return FALSE;

	for (DWORD dwCnt = 0; dwCnt < pRecv->dwInfoCnt; dwCnt++)
	{
		CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle(pRecv->sAllianceGuild[dwCnt].eGodID);
		if (!P_VALID(pGodMiracle))
			continue;
		pGodMiracle->Add2AllianceGuildMap(pRecv->sAllianceGuild[dwCnt].sAllianceGuildInfo.dwGuildID,
			pRecv->sAllianceGuild[dwCnt].sAllianceGuildInfo.eAllianceForceType,
			pRecv->sAllianceGuild[dwCnt].sAllianceGuildInfo.bConfirmed,
			FALSE);
	}
	return TRUE;
}
DWORD DBSession::HandleGetFabaoNirVanaData(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_GetFabaoNirVanaData);

	if (!P_VALID(pRecv)) return FALSE;

	Role * pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if( !P_VALID(pRole) )
		return GT_INVALID;

	tagEquip * pEquip = pRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Face);
	tagFabao * pFabao = NULL;
	if( P_VALID(pEquip) && P_VALID(pEquip->pEquipProto) && MIsFaBao(pEquip) )
	{
		pFabao = (tagFabao*)pEquip;
	}
	tagNS_FairyNirvana msg;
	msg.dwErrorCode = GT_INVALID;
	if( !P_VALID(pFabao) || pFabao->n16Stage < 60 )
	{
		pRole->SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}
	if( pRecv->dwErrorCode != E_Success )
	{
		pRole->SendMessage(&msg,msg.dwSize);
		return GT_INVALID;
	}

	pRole->ProcEquipEffect(NULL,pFabao,pFabao->pEquipProto->eEquipPos,TRUE,FALSE);

	pFabao->n16Stage = pRecv->data.nStage;
	const tagFabaoLevelProto * pLevelProto = g_attRes.GetFabaoLevelProto(pFabao->n16Stage);
	if( P_VALID(pLevelProto) )
	{
		pFabao->nPracticeValue = pLevelProto->nMinValue;
		pFabao->nUseLevel		= pLevelProto->nMinUseLevel;
	}
	ItemCreator::RecalFabaoAttWithoutCalStage(pFabao);
	pFabao->eEleInjury = (EElementInjury)pRecv->data.nEleInjuryType;
	pFabao->nEleInjury = pRecv->data.nEleInjury;
	pFabao->nElePierce = pRecv->data.nElePierce;
	memcpy(pFabao->nEleResistance,pRecv->data.nEleResistance,sizeof(pRecv->data.nEleResistance));
	pFabao->wDamageUpgradeTimes = pRecv->data.wDamageUpgradeTimes + pFabao->wEleInjPromoteTimes - pRecv->data.wEleInjPromoteTimes;
	pFabao->wResistanceUpgradeTimes = pRecv->data.wResistanceUpgradeTimes + pFabao->wEleResPromoteTimes - pRecv->data.wEleResPromoteTimes;
	//memcpy(pFabao->dwFairySkill,pRecv->data.dwFairySkill,sizeof(pRecv->data.dwFairySkill));
	//pFabao->dwUnsureSkill = pRecv->data.dwUnsureSkill;

	pRole->ProcEquipEffect(pFabao,NULL,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);

	// 资质发生变化，重新计算
	ItemCreator::RecalFaBaoCurStoredExp(pFabao);

	pRole->GetItemMgr().UpdateFabaoSpec(*pFabao);
	msg.dwErrorCode = E_Success;
	pRole->SendMessage(&msg,msg.dwSize);
	pRole->GetItemMgr().DelFromBag(pRecv->n64NirvanID,ELCLD_Fabao_Nirvana,1);
	return E_Success;
}

// 神之竞标
DWORD DBSession::HandleLoadGodBidInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadGodBidInfo);

	if (!P_VALID(pRecv)) return FALSE;

	if (pRecv->dwBidCount <= 0)
		return FALSE;

	g_GodBidMgr.InitGodBidInfoFromDB(pRecv->dwBidCount, static_cast<tagGodBidInfo2DB*>(pRecv->GodBidInfo2DB));

	return E_Success;
}

DWORD DBSession::HandleClearGodBidInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_ClearGodBidInfo);

	if (!P_VALID(pRecv)) return FALSE;

	g_GodBidMgr.BidEnd();

	return E_Success;
}

VOID DBSession::ReInitItemNeedLog(VOID)
{
	CHECK_TIME()
	if( m_nItemNLItemNum > 0 )
	{
		g_attRes.ResetItemLog(m_dwItemNLdwTypeID, m_nItemNLItemNum);
	}
	m_nItemNLItemNum = 0;
}

// 神职竞选相关
// DWORD DBSession::HandleAsk4NewClergy(tagNetCmd* pMsg, DWORD)
// {
// 	//g_ClergyRankingMgr.ReInit();
// 
// // 	tagNDBC_RequestInitClergyRank send;
// // 	m_pTran->Send(&send, send.dwSize);
// 
// 	return E_Success;
// }

DWORD DBSession::HandleInitClergyRank(tagNetCmd* pMsg, DWORD)
{
	MTRANS_POINTER(pData, pMsg, tagNDBS_InitClergyRank);

	g_ClergyRankingMgr.DoInit(pData);

	return E_Success;
}

DWORD DBSession::HandleRenewClergyCandidateType(tagNetCmd* pMsg, DWORD)
{
	g_roleMgr.RenewClergyCandidateType();

	return E_Success;
}

DWORD DBSession::HandleGetClergyCandidateCount(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_Ask4CandidateCount);

	g_ClergyRankingMgr.UpdateCandidateCount(pRecv->dwCount);
	return E_Success;
}

DWORD DBSession::HandleClearAllCompetitiveClergy(tagNetCmd* pMsg, DWORD)
{
	g_ClergyRankingMgr.ClearPreClergy(ECGT_War);
	g_ClergyRankingMgr.ClearPreClergy(ECGT_Sun);
	g_ClergyRankingMgr.ClearPreClergy(ECGT_Mana);
	g_ClergyRankingMgr.ClearPreClergy(ECGT_Peace);

	tagNDBC_RequestInitClergyRank send;
	g_dbSession.Send(&send, send.dwSize);

	return E_Success;
}

VOID DBSession::Send(LPVOID pMsg, DWORD dwMsgSize)	
{ 
	if(m_lTransCanUse && P_VALID(m_pTran)) 
	{
		m_pTran->Send(pMsg, dwMsgSize); 
		g_world.CountNetMsg(pMsg, dwMsgSize);
	}
}

DWORD DBSession::HandleLoadGlobalLimitItem(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadGlobalLimitedItem);

	g_GlobalLimitedItemMgr.Init(pRecv->data, pRecv->nNum);

	return E_Success;
}
//
//DWORD DBSession::HandleLoadRareArea(tagNetCmd* pMsg, DWORD)
//{
//	MGET_MSG(pLoadAllRareArea, pMsg, NDBS_LoadAllRareArea);
//
//	if( pLoadAllRareArea->nAllRareArea > 0 )
//	{
//		g_RareAreaMgr.InitDBRareArea(&pLoadAllRareArea->sRareAreaLoad);
//	}
//
//	return E_Success; 
//}

//---------------------------------------------------------------------------------
//删除角色领域
VOID DBSession::SendDeleteArea2DB( DWORD dwRoleID, DWORD dwAreaID)
{
	MCREATE_MSG(pSend, sizeof(tagNDBC_DeleteRoleArea), NDBC_DeleteRoleArea);

	if (!P_VALID(pSend))
		return;

	pSend->dwRoleID = dwRoleID;
	pSend->dwAreaID = dwAreaID;

	g_dbSession.Send(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}
//---------------------------------------------------------------------------------
//增加角色领域
VOID DBSession::SendCreateArea2DB(DWORD dwRoleID, DWORD dwAreaID, BOOL bActivation)
{
	MCREATE_MSG(pSend, sizeof(tagNDBC_CreateRoleArea), NDBC_CreateRoleArea);

	if (!P_VALID(pSend))
		return;

	pSend->area.dwRoleID = dwRoleID;
	pSend->area.dwAreaID = dwAreaID;
	pSend->area.bActivation = bActivation;

	g_dbSession.Send(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}
//---------------------------------------------------------------------------------
//删除角色技能
VOID DBSession::SendRemoveSkill( DWORD dwRoleID, DWORD dwSkillID )
{
	tagNDBC_RemoveSkill send;
	send.dwRoleID = dwRoleID;
	send.dwSkillID = dwSkillID;
	g_dbSession.Send(&send, send.dwSize);
}
//---------------------------------------------------------------------------------
//删除角色技能
VOID DBSession::SendRemoveOneBuff( DWORD dwRoleID, DWORD dwBuffID )
{
	tagNDBC_DelRoleOneBuff send;
	send.dwRoleID = dwRoleID;
	send.dwBuffID = dwBuffID;
	g_dbSession.Send(&send, send.dwSize);
}
//-------------------------------------------------------------------------------
//给非在线角色取消领域能力
VOID DBSession::RemoveAreaAbilityOffline( DWORD dwRoleID, DWORD dwAreaID )
{
	const tagGodAreaProto *pGodAreaProto = g_attRes.GetGodAreaProto(dwAreaID);
	if( !P_VALID(pGodAreaProto) )
	{
		return;
	}

	switch(pGodAreaProto->eNatureType)
	{
	case EGANT_SKILL:
		{//减少技能
			SendRemoveSkill(dwRoleID,Skill::GetIDFromTypeID(pGodAreaProto->dwSkillID));
		}
		break;
	case EGANT_BUFF:
		{//减少BUFF
			SendRemoveOneBuff(dwRoleID,Buff::GetIDFromTypeID(pGodAreaProto->dwBuffID));
		}
		break;
	case EGANT_ALL:
		{
			SendRemoveSkill(dwRoleID,Skill::GetIDFromTypeID(pGodAreaProto->dwSkillID));
			SendRemoveOneBuff(dwRoleID,Buff::GetIDFromTypeID(pGodAreaProto->dwBuffID));
		}
		break;
	}
}
//---------------------------------------------------------------------------------
DWORD DBSession::HandleLoadFamilySprite(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadFamilySprite);

	tagRoleSpriteDataSave *p = NULL;
	if( pRecv->nNumRequirement > 0 )
		p = (tagRoleSpriteDataSave *)((char*)pRecv + sizeof(tagNDBS_LoadFamilySprite) - sizeof(tagRoleSpriteDataSave) + sizeof(tagFamilySpriteRequirementSave) * ( pRecv->nNumRequirement - 1));
	else
		p = pRecv->rsds;

	g_FamilySpriteMgr.InitFromDB( &pRecv->fs, pRecv->fsr, pRecv->nNumRequirement, p, pRecv->nNumSprite);

	return E_Success;
}
//---------------------------------------------------------------------------------
//DWORD DBSession::HandleLoadFamilySpriteRequirement(tagNetCmd* pMsg, DWORD)
//{
//	MGET_MSG(pRecv, pMsg, NDBS_LoadFamilySpriteRequirement);
//
//	g_GlobalLimitedItemMgr.Init(&pRecv->fsr);
//
//	return E_Success;
//}
//---------------------------------------------------------------------------------

DWORD DBSession::HandleLoadAllFamily(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadAllFamily);
	if (!P_VALID(pRecv))
		return GT_INVALID;
	g_FamilyMgr.LoadAllFamily(pRecv->sFamilyLoad);
	return E_Success;
}

DWORD DBSession::HandleLoadFamilyMember(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_LoadFamilyMember);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	g_FamilyMgr.LoadFamilyMember(pRecv->nMemberNum, (tagFamilyMember*)pRecv->sFamilyMember);

	return E_Success;
}

DWORD DBSession::HandleFamilyMemberInitOK(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_FamilyMemberInitOK);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	tagNDBC_FamilyInitOK send;
	Send(&send, send.dwSize);

	return E_Success;
}

DWORD DBSession::HandleFamilyInitOK(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_FamilyInitOK);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	if (!g_FamilyMgr.IsFamilyInitOK())
	{
		// 关闭服务器
		ILOG->Write(_T("family init failed! Please check db!\n"));
		g_world.ShutDown();
		return GT_INVALID;
	}

	return E_Success;
}

DWORD DBSession::HandleLoadFamilyQuest(tagNetCmd* pMsg, DWORD)
{
	if (!P_VALID(pMsg))
	{
		return GT_INVALID;
	}

	MGET_MSG(pRecv, pMsg, NDBS_LoadFamilyQuest);

	if(E_Success != pRecv->dwErrorCode)
	{
		return GT_INVALID;
	}
	
	INT nNum = pRecv->nNum;
	if (nNum<=0)
	{
		g_FamilyMgr.SetQuestInitOK();
		return 0;
	}

	tagFamilyQuestLoadData* p = pRecv->QuestData;
	for (int i=0; i<nNum; ++i)
	{
		if (!P_VALID(p))
		{
			break;
		}

		DWORD dwFamilyID = p->dwFamilyID;
		if (!P_VALID(dwFamilyID))
		{
			continue;
		}

		Family* pFmly = g_FamilyMgr.GetFamily(dwFamilyID);
		if(!P_VALID(pFmly))
		{
			continue;
		}

		pFmly->GetFamilyQuest().Init(p);

		++p;
	}

	return 0;
}

DWORD DBSession::HandleLoadWorldData(tagNetCmd* pMsg, DWORD)
{
	if (!P_VALID(pMsg))
	{
		return GT_INVALID;
	}

	MGET_MSG(pRecv, pMsg, NDBS_RequestLogWorldData);

	g_DataCollection.Init(pRecv->dwData);

	return 0;
}

DWORD DBSession::HandleLoadRoleLogData(tagNetCmd* pMsg, DWORD)
{
	if (!P_VALID(pMsg))
	{
		return GT_INVALID;
	}

	MGET_MSG(pRecv, pMsg, NDBS_RequestLogRoleData);

	Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	pRole->InitLogData(pRecv->dwData);

	return 0;
}

DWORD DBSession::HandleAccountReactiveOrNot(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_AccountReactiveCode);

	PlayerSession* pSession = g_worldSession.FindSession(pRecv->dwAccountID);
	if (!P_VALID(pSession))
	{
		return GT_INVALID;
	}

	Role* pRole = pSession->GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (pRecv->dwErrorCode == E_Success && P_VALID(pRecv->dwCAccountID))
	{
		pRole->AccountSuccessReactive();

		PlayerSession* pIntroSession = g_worldSession.FindSession(pRecv->dwCAccountID);
		if(P_VALID(pIntroSession))
		{
			Role* pIntro = pIntroSession->GetRole();
			if(P_VALID(pIntro))
			{
				pIntro->AddAccountReactiveScore(pRecv->nScore);
			}
		}
	}

	tagNS_ReactiveKeyCode send;
	send.dwErrorCode = pRecv->dwErrorCode;
	pSession->SendMessage(&send, send.dwSize);

	return 0;
}

DWORD DBSession::HandleInitAccountReactiveInfo(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NDBS_InitAccountReactiveInfo);

	Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	pRole->InitAccountReactiveInfo(&(pRecv->info));
	return 0;
}

DWORD DBSession::HandleLoadRoleTigerProfit(tagNetCmd* pMsg, DWORD)
{
	if (!P_VALID(pMsg))
	{
		return GT_INVALID;
	}

	MGET_MSG(pRecv, pMsg, NDBS_TigerProfit);
	TigerMgr::SetProfitNum(pRecv->n64Profit);

	return 0;
}

DWORD DBSession::HandelKeyCodeAddGift(tagNetCmd* pMsg, DWORD)
{
	if(!P_VALID(pMsg))
	{
		return GT_INVALID;
	}

	MGET_MSG(pRecv, pMsg, NDBS_KeyCodeAddGift);
	Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if(!P_VALID(pRole))
		return GT_INVALID;

	if(pRecv->bIsSuccessful)
	{
		//没领过，开始发奖
		tagItem* pNewItem = ItemCreator::Create(EICM_NewKeyCode, NULL, NewKeyCodeGiftid);
		pRole->GetItemMgr().Add2BaiBao(pNewItem, ELCID_NewKeyCode, TRUE);
	}
	else
	{
		//领过了，告诉他一下~
		tagNS_GetKeyCodeGift send;
		send.dwErrorCode = 2;	//已经领过了
		pRole->SendMessage(&send, send.dwSize);
	}
}

DWORD DBSession::HandleNDBS_ReceiveYuanBao(tagNetCmd* pMsg, DWORD)
{
	if(!P_VALID(pMsg))
	{
		return GT_INVALID;
	}
	MGET_MSG(pRecv, pMsg, NDBS_ReceiveYuanBao);
	Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if(!P_VALID(pRole))
		return GT_INVALID;

	if(pRecv->nNum == 0)
	{
		return GT_INVALID;	//不告诉他了
	}
	else
	{
		//开始发东西
		tagItem* pNewItem = ItemCreator::Create(EICM_ReceiveYuanBaoDaiBi, NULL, YuanBaoDaiBi, pRecv->nNum);
		pRole->GetItemMgr().Add2BaiBao(pNewItem, ELCID_YuanBaoDaiBi, TRUE);
	}
}