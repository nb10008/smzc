//-----------------------------------------------------------------------------
//!\file to_rt_session.cpp
//!\author Lyp
//!
//!\date 2005-04-05
//! last 2005-04-05
//!
//!\brief 量子区域服务器与服务器控制服务的对话层
//!
//!	Copyright (c) 2005 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../ServerDefine/msg_rt_s.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/role_data.h"
#include "rt_session.h"
#include "world.h"
#include "world_session.h"
#include "gm_policy.h"
#include "role_mgr.h"
#include "../WorldDefine/mall_define.h"
#include "mall.h"
#include "../WorldDefine/chat.h"
#include "channel_mgr.h"
#include "../WorldDefine/chat_define.h"
#include "godbid_mgr.h"
#include "guild.h"
#include "GuildBattle.h"
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
RTSession::RTSession() : m_Trunk(this)
{
	m_bInitOK					=	FALSE;
	m_bTermConnect				=	FALSE;
	m_nSendInfoTickCountDown	=	SEND_INFO_INTERVAL;
}

//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
RTSession::~RTSession()
{
}

//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL RTSession::Init()
{
	m_pThread		=	"Thread";
	m_pUtil			=	"Util";

	// 初始化成员属性
	TObjRef<VarContainer> pVar = "VarContainer";
	m_strIP			=	pVar->GetString(_T("ip gm_server"));
	m_dwPort		=	pVar->GetDword(_T("port gm_server"));
	m_dwSectionID	=	pVar->GetDword(_T("section_id world"));
	m_dwWorldID		=	pVar->GetDword(_T("id world"));

	// 创建消息管理
	CreateObj("ToRtNetCmdMgr", "NetCmdMgr");
	m_pMsgCmdMgr = "ToRtNetCmdMgr";

	// 创建连接对象，并初始化
	CreateObj("ToRT", "StreamTransport");
	m_pTran = "ToRT";
	if( !P_VALID(m_pTran) )
	{
		ERR(_T("Create ToRT(StreamTransport) obj failed!\r\n"));
		return FALSE;
	}
	m_pTran->Init();

	// 注册所有网络命令
	RegisterAllRTCommand();

	// 启动连接线程
	if(!m_pThread->CreateThread(_T("ConnectRT"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&RTSession::ThreadConnectRT), NULL))
	{
		return FALSE;
	}

	while( !m_pThread->IsThreadActive(_T("ConnectRT")) )
	{
		continue;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID RTSession::Destroy()
{
	// 等待所有线程结束
	InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
	m_pThread->WaitForThreadDestroy(_T("ConnectRT"), INFINITE);

	m_pTran->Destroy();
	KillObj("ToRT");

	// 注销消息管理
	m_pMsgCmdMgr->Destroy();
	KillObj("ToRtNetCmdMgr");
}



//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID RTSession::Update()
{
	CHECK_TIME()
	UpdateSession();
	SendServerInfo();
}

//-----------------------------------------------------------------------------
// 接收消息
//-----------------------------------------------------------------------------
VOID RTSession::UpdateSession()
{
	if( NULL == m_pTran )
		return;

	if( !m_pTran->IsConnected() && !m_pThread->IsThreadActive(_T("ConnectRT")) )
	{
		InterlockedExchange((LONG*)&m_bTermConnect, TRUE);
		m_pTran->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ConnectRT"), INFINITE);

		// 重新启动登陆服务器连接线程
		InterlockedExchange((LONG*)&m_bTermConnect, FALSE);
		m_pThread->CreateThread(_T("ConnectRT"), (vEngine::THREADPROC)m_Trunk.sfp0(&RTSession::ThreadConnectRT), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ConnectRT")))
		{
			continue;
		}

		return;
	}

	while(m_pTran->IsConnected())
	{
		DWORD dwSize = 0;
		LPBYTE pRecv = m_pTran->Recv(dwSize);
		if( !P_VALID(pRecv) )
			break;

		// 处理消息
		m_pMsgCmdMgr->HandleCmd((tagNetCmd*)pRecv, dwSize, GT_INVALID);

		// 回收资源
		m_pTran->FreeRecved(pRecv);

		
	}	
}

//-----------------------------------------------------------------------------
// 发送服务器情况
//-----------------------------------------------------------------------------
VOID RTSession::SendServerInfo()
{
	if( !m_pTran->IsConnected() || !m_bInitOK ) return;

	if( --m_nSendInfoTickCountDown > 0 )
		return;

	m_nSendInfoTickCountDown = SEND_INFO_INTERVAL;

	tagNSC_WorldInfo send;
	
	if( g_world.IsWell() )
	{
		send.eStatus = EWS_Well;
	}
	else
	{
		send.eStatus = EWS_SystemError;
	}
	send.nMaxOnlineNum = g_worldSession.GetPlayerNumLimit();
	send.nOnlineNum = g_worldSession.GetPlayerNumCurrent();
	send.nOnlineNumDisplay = g_worldSession.GetPlayerNumDisplay();
	m_pTran->Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// 连接线程(连接监控服务器)
//-----------------------------------------------------------------------------
DWORD RTSession::ThreadConnectRT()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while( FALSE == m_bTermConnect )
	{
		if( !m_pTran->IsConnected() )
		{
			if( !m_pTran->IsTryingToConnect() )
			{
				m_pTran->TryToConnect(m_pUtil->UnicodeToUnicode8(m_strIP.c_str()), m_dwPort);
			}

			Sleep(100);
			continue;	// 重新检测连接
		}

		IMSG(_T("Contected to RT Server at %s: %d\r\n"), m_strIP.c_str(), m_dwPort);

		
		tagNSC_LoginService send;
		send.dwSectionID	=	m_dwSectionID;
		send.dwWorldID		=	m_dwWorldID;
		send.eType			=	EST_World;
		m_pTran->Send(&send, send.dwSize);

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
VOID RTSession::RegisterAllRTCommand()
{
	m_pMsgCmdMgr->Register("NSS_LoginService",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleServerLogin),	_T("RT Server Certification Reply"));
	m_pMsgCmdMgr->Register("NSS_Close",				(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleCloseServer),	_T("Close Server"));
	m_pMsgCmdMgr->Register("NSS_Double",			(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleDouble),			_T("Double Rate"));
	m_pMsgCmdMgr->Register("NSS_AutoNotice",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleAutoNotice),		_T("AutoNotice"));
	m_pMsgCmdMgr->Register("NSS_RightNotice",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleRightNotice),	_T("RightNotice"));
	m_pMsgCmdMgr->Register("NSS_MaxNum",			(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleSetMaxNum),		_T("Set Max User Online"));
	m_pMsgCmdMgr->Register("NSS_UpdateMall",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleUpdateMall),		_T("Update Mall"));
	m_pMsgCmdMgr->Register("NSS_AutoChatNotice",	(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleAutoChatNotice),	_T("Chat Notice"));
	m_pMsgCmdMgr->Register("NSS_CancelDouble",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleCancelDouble),	_T("Cancel Double"));
	m_pMsgCmdMgr->Register("NSS_ClearBaibao",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleClearBaibao),	_T("Clear Baibao"));
	m_pMsgCmdMgr->Register("NSS_RoleSpeak",			(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleRoleSpeak),		_T("Control Role Speaking Rights"));
	m_pMsgCmdMgr->Register("NSS_SetFackNumer",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleSetFackNum),		_T("Set online number display"));
	m_pMsgCmdMgr->Register("NSS_UpdateGodBid",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleUpdateGodBid),	_T("update GodBid"));
	m_pMsgCmdMgr->Register("NSS_UpdateGoldEggPct",	(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleUpdateGoldEggPct),_T("update GoldEggPct"));
	m_pMsgCmdMgr->Register("NSS_ReloadItemNeedLogPct",(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleReloadItemNeedLogPct),_T("reload item_needlog"));
	m_pMsgCmdMgr->Register("NSS_ItemTypeID",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleCheckItemTypeID),_T("Check Item TypeID"));
	m_pMsgCmdMgr->Register("NSS_ClearAttPoint",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleClearAttPoint),	_T("Clear Role Att Point"));
	m_pMsgCmdMgr->Register("NSS_GuildChgMemLvl",	(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleGuildChgMemLvl),	_T("Change Guild Member's Level"));
	m_pMsgCmdMgr->Register("NSS_GuildChgFund",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleGuildChgFund),	_T("Change Guild Fund"));
	m_pMsgCmdMgr->Register("NSS_GuildChgState",		(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleGuildChgState),	_T("Change Guild State"));
	m_pMsgCmdMgr->Register("NSS_GuildGodMiracle",	(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleGuildGodMiracle),	_T("NSS_GuildGodMiracle"));
	m_pMsgCmdMgr->Register("NSS_AddNeedPKLogRole",	(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleAddNeedPKLogRole),	_T("Add Need PK Log Role"));
	m_pMsgCmdMgr->Register("NSS_AddFilterWords",	(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleAddFilterWords),	_T("Add Filter Words"));
	m_pMsgCmdMgr->Register("NSS_DelFilterWords",	(NETMSGPROC)m_Trunk.sfp2(&RTSession::HandleDelFilterWords),	_T("Delete Filter Words"));
	
}

//----------------------------------------------------------------------------------------------
// 服务器认证网络消息
//----------------------------------------------------------------------------------------------
DWORD RTSession::HandleServerLogin(tagNetCmd* pMsg, DWORD)
{
	tagNSS_LoginService* pRecv = (tagNSS_LoginService*)pMsg;

	InterlockedExchange((LPLONG)&m_bInitOK, TRUE);
	return 0;
}

//-----------------------------------------------------------------------------------------------
// 服务器关闭网络消息
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleCloseServer(tagNetCmd* pMsg, DWORD)
{
	g_world.ShutDown();
	return 0;
}

//-----------------------------------------------------------------------------------------------
// 设置双倍
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleDouble( tagNetCmd* pMsg, DWORD )
{
	MGET_MSG(pRecv, pMsg, NSS_Double);

	g_GMPolicy.SetRate(pRecv->eDType, pRecv->dwRatio, pRecv->dwOpenTime, pRecv->dwLastTime);

	tagNSC_Double send;
	send.dwErrorCode	= 0;
	send.dwClientID		= pRecv->dwClientID;
	m_pTran->Send(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// 设置自动循环公告
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleAutoNotice( tagNetCmd* pMsg, DWORD )
{
	MGET_MSG(pSend, pMsg, NSS_AutoNotice);

	DWORD dwLen = sizeof(tagNS_AutoNotice) - sizeof(TCHAR) + 2*(wcslen(pSend->szContent) + 1);

	MCREATE_MSG(pSendMsg, dwLen, NS_AutoNotice);

	pSendMsg->nCirInterval = pSend->nCirInterval;
	pSendMsg->nType = pSend->eType;
	_tcscpy(pSendMsg->szContent, pSend->szContent);
	
	g_roleMgr.SendWorldMsg(pSendMsg, dwLen);

	MDEL_MSG(pSendMsg);

	tagNSC_AutoNotice msg;
	msg.dwErrorCode	= 0;
	msg.dwClientID		= pSend->dwClientID;
	m_pTran->Send(&msg, msg.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// 设置右下角公告
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleRightNotice(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pSend, pMsg, NSS_RightNotice);

	DWORD dwLen = sizeof(tagNS_RightNotice) - sizeof(TCHAR) + 2*( wcslen(pSend->szTitle) + wcslen(pSend->szContent) + wcslen(pSend->szLink) +1);
	
	MCREATE_MSG(pSendMsg, dwLen, NS_RightNotice);

	pSendMsg->nTitleLen = wcslen(pSend->szTitle);
	pSendMsg->nContentLen = wcslen(pSend->szContent);
	pSendMsg->nLinkLen = wcslen(pSend->szLink);

	_tcscpy(pSendMsg->szContent, pSend->szTitle);
	_tcscat(pSendMsg->szContent, pSend->szLink);
	_tcscat(pSendMsg->szContent, pSend->szContent);

	g_roleMgr.SendWorldMsg(pSendMsg, dwLen);

	MDEL_MSG(pSendMsg);

	tagNSC_RightNotice msg;
	msg.dwErrorCode	= 0;
	msg.dwClientID		= pSend->dwClientID;
	m_pTran->Send(&msg, msg.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// 设置游戏最大在线人数
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleSetMaxNum(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pSend, pMsg, NSS_MaxNum);

	g_worldSession.SetPlayerNumLimit(pSend->nMaxNum);

	tagNSC_MaxNum send;
	send.dwClientID = pSend->dwClientID;
	send.dwErrorCode = 0;
	m_pTran->Send(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// 更新商城
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleUpdateMall(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pSend, pMsg, NSS_UpdateMall);
	
	BOOL bSuccess = FALSE;
	bSuccess = g_mall.ReInit();

	tagNSC_UpdateMall send;
	send.dwClientID = pSend->dwClientID;
	send.dwErrorCode = bSuccess;
	m_pTran->Send(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// 更新神之竞标
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleUpdateGodBid(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pSend, pMsg, NSS_UpdateGodBid);

	BOOL bSuccess = FALSE;
	bSuccess = g_GodBidMgr.ReLoadGodBidProto();

	tagNSC_UpdateGodBid send;
	send.dwClientID = pSend->dwClientID;
	send.dwErrorCode = bSuccess;
	m_pTran->Send(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// 更新砸金蛋概率
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleUpdateGoldEggPct(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pSend, pMsg, NSS_UpdateGoldEggPct);

	BOOL bSuccess = FALSE;
	bSuccess = g_attRes.ReLoadGoldEggPctProto();

	tagNSC_UpdateGoldEggPct send;
	send.dwClientID = pSend->dwClientID;
	send.dwErrorCode = bSuccess;
	m_pTran->Send(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------------------------
// 更新item_needlog
//-----------------------------------------------------------------------------------------------
DWORD RTSession::HandleReloadItemNeedLogPct(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pSend, pMsg, NSS_ReloadItemNeedLogPct);
	if (!P_VALID(pSend))
	{
		return GT_INVALID;
	}

	// 发送获取物品是否保存信息
	tagNDBC_LoadItemNeedLog itemNeedLog;
	g_dbSession.Send(&itemNeedLog, itemNeedLog.dwSize);	

	tagNSC_ReloadItemNeedLogPct send;
	send.dwClientID = pSend->dwClientID;
	send.dwErrorCode = E_Success;
	m_pTran->Send(&send, send.dwSize);

	return 0;
}

DWORD RTSession::HandleAutoChatNotice(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NSS_AutoChatNotice);

	DWORD dwSize = sizeof(tagNS_RoleChat) + X_HUGE_STRING;
	MCREATE_MSG(pSend, dwSize, NS_RoleChat);
	pSend->byChannel		= (BYTE)ESCC_Affiche;
	pSend->dwID				= TObjRef<Util>()->Crc32("NS_RoleChat");
	pSend->dwDestRoleID		= GT_INVALID;
	pSend->dwSrcRoleID		= GT_INVALID;
	pSend->dwErrorCode		= 0;
	// Jason 2010-1-5 v1.3.1 adding
	pSend->n16Sex			= ES_Null;
	pSend->dwSize			= dwSize;
	IFASTCODE->MemCpy(pSend->szMsg, pRecv->szNotice, X_HUGE_STRING);

	g_roleMgr.ForEachRoleInWorld(SendAction(ESCC_Affiche, pSend));

	MDEL_MSG(pSend);
	return 0;
}

DWORD RTSession::HandleCancelDouble(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NSS_CancelDouble);

	g_GMPolicy.DoubleSwitch(EDoubleType_Exp, FALSE);
	g_GMPolicy.DoubleSwitch(EDoubleType_Item, FALSE);

	tagNSC_CancelDouble send;
	send.dwErrorCode	= 0;
	send.dwClientID		= pRecv->dwClientID;
	m_pTran->Send(&send, send.dwSize);

	return 0;
}

DWORD RTSession::HandleClearBaibao(tagNetCmd* pMsg, DWORD)
{
	//MGET_MSG(pRecv, pMsg, NSS_ClearBaibao);

	//DWORD dwNameCrc = m_pUtil->Crc32(pRecv->szOwnerName);
	//Role* pRole = g_roleMgr.GetRoleIDByNameCrc(dwNameCrc);
	//if(!P_VALID(pRole)) return FALSE;

	//pRole->GetItemMgr().GetBaiBaoBag()

	return E_Success;
}

DWORD RTSession::HandleSetFackNum(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pSend, pMsg, NSS_SetFackNumer);

	g_worldSession.SetPlayNumberFactor(pSend->fFackRate);

	return E_Success;
}

DWORD RTSession::HandleRoleSpeak(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NSS_RoleSpeak);
	DWORD dwRoleID = g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(pRecv->szRoleName));
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		// 角色在线的话需要进行下面的操作
		// 不在线角色直接操作数据库
		if (pRecv->bAction) 
		{
			// 解禁
			pRole->SetSpeakOff(FALSE);
		}
		else 
		{
			// 禁言
			pRole->SetSpeakOff(TRUE);
		}
	}
	
	// 通知数据库做相应修改
	tagNDBC_RoleSpeak send;
	send.dwRoleID = dwRoleID;
	send.bAction = pRecv->bAction;
	g_dbSession.Send(&send, send.dwSize);

	tagNSC_RoleSpeak ret;
	ret.dwClientID = pRecv->dwClientID;
	ret.dwErrorCode = E_Success;
	m_pTran->Send(&ret, ret.dwSize);

	return E_Success;
}

DWORD RTSession::HandleCheckItemTypeID(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NSS_ItemTypeID);

	tagNSC_ItemTypeID send;
	//tagItemProto* pProto = g_attRes.GetItemProto(pRecv->dwTypeID);
	if (!P_VALID(g_attRes.GetItemProto(pRecv->dwTypeID)) && !P_VALID(g_attRes.GetEquipProto(pRecv->dwTypeID)))
	{
		send.dwErrorCode = E_GMTool_CreateItem_ItemTypeIDErr;
	}
	else
	{
		send.dwErrorCode = E_GMTool_Success;
	}
	send.dwSectionID = pRecv->dwSectionID;
	send.dwWorldID = pRecv->dwWorldID;
	send.dwAccountID = pRecv->dwAccountID;
	send.dwRoleID = pRecv->dwRoleID;
	send.dwTypeID = pRecv->dwTypeID;
	send.nItemNum = pRecv->nItemNum;
	send.byQlty = pRecv->byQlty;
	send.dwClientID = pRecv->dwClientID;
	m_pTran->Send(&send, send.dwSize);

	return E_Success;
}

DWORD RTSession::HandleClearAttPoint(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NSS_ClearAttPoint);

	//DWORD dwRoleID = g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(pRecv->szRoleName));
	Role* pRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pRole))
	{
		tagNSC_ClearAttPoint send;
		send.dwClientID = pRecv->dwClientID;
		send.dwErrorCode = E_GMTool_RoleNotOnline;
		m_pTran->Send(&send, send.dwSize);
		return E_Success;
	}

	tagNSC_ClearAttPoint msg;
	msg.dwClientID = pRecv->dwClientID;
	msg.dwErrorCode = pRole->ClearAttPoint(NULL, 0);
	m_pTran->Send(&msg, msg.dwSize);

	return E_Success;
}

DWORD RTSession::HandleGuildChgMemLvl(tagNetCmd* pMsg, DWORD)
{
	return E_Success;
}

DWORD RTSession::HandleGuildChgFund(tagNetCmd* pMsg, DWORD)
{
	return E_Success;
}

DWORD RTSession::HandleGuildChgState(tagNetCmd* pMsg, DWORD)
{
	return E_Success;
}

DWORD RTSession::HandleGuildGodMiracle(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NSS_GuildGodMiracle);
	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	if (pRecv->dwMiracleType < EGM_NH || pRecv->dwMiracleType >= EGM_MAX)
	{
		return GT_INVALID;
	}

	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->GetGodMiracle((EGodMiracle)pRecv->dwMiracleType);
	if (!P_VALID(pGodMiracle))
	{
		return GT_INVALID;
	}

	pGodMiracle->SetCapGodID(pRecv->dwGuildID);
	pGodMiracle->SetCapStartTime(GetCurrentDWORDTime());
	pGodMiracle->SetCurState(EGMS_Rest);
	pGodMiracle->SaveCapTureInfoToActive();

	tagNSC_GuildGodMiracle msg;
	msg.dwClientID = pRecv->dwClientID;
	msg.dwErrorCode = E_GMTool_Success;
	m_pTran->Send(&msg,msg.dwSize);

	return E_Success;
}

DWORD RTSession::HandleAddNeedPKLogRole(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NSS_AddNeedPKLogRole);

	DWORD dwRoleID = 0;
	if (0!=pRecv->dwRoleID)
	{
		dwRoleID = pRecv->dwRoleID;	
	}
	else
	{
		dwRoleID = g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(pRecv->szRoleName));
	}

	if (!P_VALID(dwRoleID))
	{
		tagNSC_AddNeedPKLogRole send;
		send.dwClientID = pRecv->dwClientID;
		send.dwErrorCode = E_GMTool_AddPKLog_RoleInvalid;
		m_pTran->Send(&send, send.dwSize);
		return GT_INVALID;
	}

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		tagNSC_AddNeedPKLogRole send;
		send.dwClientID = pRecv->dwClientID;
		send.dwErrorCode = E_GMTool_AddPKLog_RoleOffline;
		m_pTran->Send(&send, send.dwSize);
		return GT_INVALID;
	}

	pRole->GetCombatHandler().SetNeedPKLogOn();

	tagNSC_AddNeedPKLogRole send;
	send.dwClientID = pRecv->dwClientID;
	send.dwErrorCode = E_Success;
	m_pTran->Send(&send, send.dwSize);
	return E_Success;
}

DWORD RTSession::HandleAddFilterWords(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NSS_AddFilterWords);
	
	std::vector<tstring>* vecFilterWords = g_attRes.GetChatFilterWordsVec();
	std::vector<tstring>::iterator it = find(vecFilterWords->begin(), vecFilterWords->end(), pRecv->szFilterWords);
	
	if (it != vecFilterWords->end())
	{
		tagNSC_AddFilterWords send;
		send.dwClientID = pRecv->dwClientID;
		send.dwErrorCode = E_GMTool_FilterWords_Existed;
		m_pTran->Send(&send, send.dwSize);
		return E_GMTool_FilterWords_Existed;

	}

	else
	{
		//过滤词不存在，则插入，并写入xml文件
		vecFilterWords->push_back(pRecv->szFilterWords);	
		ChangeXmlElement(pRecv->szFilterWords,TRUE,vecFilterWords);
	}



	tagNSC_AddFilterWords msg;
	msg.dwClientID = pRecv->dwClientID;
	msg.dwErrorCode = E_GMTool_Success;
	m_pTran->Send(&msg, msg.dwSize);

	return E_GMTool_Success;
}
DWORD RTSession::HandleDelFilterWords(tagNetCmd* pMsg, DWORD)
{
	MGET_MSG(pRecv, pMsg, NSS_DelFilterWords);

	std::vector<tstring>* vecFilterWords = g_attRes.GetChatFilterWordsVec();
	std::vector<tstring>::iterator it = find(vecFilterWords->begin(), vecFilterWords->end(), pRecv->szFilterWords);

	if (it == vecFilterWords->end())
	{
		tagNSC_DelFilterWords send;
		send.dwClientID = pRecv->dwClientID;
		send.dwErrorCode = E_GMTool_FilterWords_NotExisted;
		m_pTran->Send(&send, send.dwSize);
		return E_GMTool_FilterWords_NotExisted;

	}

	else
	{
		//过滤词存在，则删除
		vecFilterWords->erase(it);	
		ChangeXmlElement(pRecv->szFilterWords,FALSE,vecFilterWords);
	}



	tagNSC_DelFilterWords msg;
	msg.dwClientID = pRecv->dwClientID;
	msg.dwErrorCode = E_GMTool_Success;
	m_pTran->Send(&msg, msg.dwSize);

	return E_GMTool_Success;
}

DWORD RTSession::ChangeXmlElement(LPWSTR szFilterWord, BOOL bInsert, std::vector<tstring>* vecStr)
{
	tstring strFileName = _T("data/system/attdata/filter_words.xml");
	if( strFileName.find(_T(".xml")) != tstring::npos )
	{
		XmlDocument doc;
		if( !doc.LoadFile(NULL, strFileName) )
			return FALSE;

		XmlHandle docHandle( &doc );
		XmlHandle eleRootHandle = docHandle.FirstChildElement("root");
		XmlElement* pRootElement = eleRootHandle.Element();
		if( !pRootElement )	// 是否符合存盘格式
			return FALSE;
	
		CHAR* chFilterName = NULL;
		

		//插入记录
		if (bInsert)
		{
			GBKToUTF8(chFilterName,szFilterWord);
			XmlElement ele("chat_filter_words");
			ele.SetAttribute("id", vecStr->size());
			ele.SetAttribute("item", chFilterName);
			pRootElement->InsertEndChild(ele);
		}
		//否则删除记录,重新写XMl文件
		else
		{			
			XmlElement* pXmlElement = docHandle.FirstChildElement("root").FirstChildElement().Element();
			while( pXmlElement )
			{
				XmlElement* pTmpXmlElement = pXmlElement->NextSiblingElement();
				pRootElement->RemoveChild(pXmlElement );				
			
				pXmlElement = pTmpXmlElement;
			}

			pXmlElement = docHandle.FirstChildElement("root").FirstChildElement().Element();
			INT nId = 1;
			for (std::vector<tstring>::iterator it = vecStr->begin(); it != vecStr->end(); it++)
			{
				GBKToUTF8(chFilterName,(*it).c_str());
				XmlElement ele("chat_filter_words");
				ele.SetAttribute("id", nId++);
				ele.SetAttribute("item", chFilterName);
				pRootElement->InsertEndChild(ele);
			}
		}
		doc.SaveXmlFile(strFileName);

		delete[] chFilterName;

	}
	
	

	return TRUE;
}

 void RTSession::GBKToUTF8(char* &szOut,LPCWSTR wszIn)
 { 
 	int len = WideCharToMultiByte(CP_UTF8,0,(LPCWSTR)wszIn,-1,NULL,0,NULL,NULL);
 	szOut = new char[len + 1];
 	memset(szOut,0,len + 1);
 	WideCharToMultiByte(CP_UTF8,0,(LPCWSTR)wszIn,-1,szOut,len,NULL,NULL); 
 	
 }

RTSession g_rtSession;

