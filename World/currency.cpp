//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: currency.cpp
// author: Sxg
// actor:
// data: 2008-08-14
// last:
// brief: 货币类 -- 管理背包仓库中的金钱元宝
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "currency.h"
#include "role.h"
#include "role_mgr.h"
#include "../WorldDefine/currency_define.h"
#include "../WorldDefine/msg_currency.h"
#include "../ServerDefine/msg_log.h"
#include "../ServerDefine/msg_common.h"
#include "guild.h"
#include "guild_mgr.h"
#include "war_role.h"
#include "war_role_mgr.h"
//#include "ipg_session_cash.h"

//-----------------------------------------------------------------------------
// 构造
//-----------------------------------------------------------------------------
CurrencyMgr::CurrencyMgr(Role *pRole, INT32 nBagGold, INT32 nBagSilver, INT32 nBagYuanBao, 
						INT64 nWareSilver, INT32 nBaiBaoYuanBao, INT32 nExVolume,INT32 nVIPPoint)
						: m_BagSilver(MGold2Silver(nBagGold) + (INT64)nBagSilver, MAX_BAG_SILVER_NUM),
						  m_BagYuanBao(nBagYuanBao, MAX_BAG_YUANBAO_NUM),
						  m_WareSilver(nWareSilver, MAX_WARE_SILVER_NUM),
						  m_BaiBaoYuanBao(nBaiBaoYuanBao, MAX_BAIBAO_YUANBAO_NUM),
						  m_pRoleClanData( &( pRole->GetClanData() ) ),
						  m_ExchangeVolume(nExVolume, MAX_EXCHANGE_VOLUME_NUM),
						  m_VIPPoint(nVIPPoint,MAX_EXCHANGE_VOLUME_NUM)
{
	m_pRole = pRole;
	m_nTimeStatCountDown = ROLE_TIME_STAT_INTERVAL;
	//ILOG->Write(_T("RoleID is:%d, Cur Money is:%.2X"),m_pRole->GetID(),m_BagSilver.GetCur());
}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
VOID CurrencyMgr::Update()
{
	if(--m_nTimeStatCountDown <= 0)
	{
		m_nTimeStatCountDown = ROLE_TIME_STAT_INTERVAL;
		LogTimeStat();
	}
}

//-----------------------------------------------------------------------------
// 向客户端发送消息
//-----------------------------------------------------------------------------
VOID CurrencyMgr::SendMessage(LPVOID pMsg, DWORD dwSize)
{
	ASSERT(P_VALID(m_pRole));
	ASSERT(P_VALID(m_pRole->GetSession()));

	m_pRole->GetSession()->SendMessage(pMsg, dwSize);
}

//-----------------------------------------------------------------------------
// 向LoongDB发消息 -- 仓库金钱
//-----------------------------------------------------------------------------
VOID CurrencyMgr::SendWareSilver2DB(DWORD dwAccountID, INT64 n64WareSilver)
{
	tagNDBC_WareMoneyUpdate send;
	send.dwAccountID		= dwAccountID;
	send.n64WareSilver		= n64WareSilver;
	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// 向LoongDB发消息 -- 百宝袋元宝
//-----------------------------------------------------------------------------
VOID CurrencyMgr::SendBaiBaoYB2DB(DWORD dwAccountID, INT nBaiBaoYuanBao)
{
	if(!g_world.IsUseIPGSystem())
	{
		tagNDBC_BaiBaoYBUpdate send;
		send.dwAccountID	= dwAccountID;
		send.nBaiBaoYuanBao	= nBaiBaoYuanBao;
		g_dbSession.Send(&send, send.dwSize);
	}
}

VOID CurrencyMgr::SendBagYB2DB(DWORD dwRoleID, INT nBagYuanBao, BOOL bIncrement/* = FALSE*/)
{
	tagNDBC_BagYBUpdate send;
	send.dwRoleID = dwRoleID;
	send.nBagYuanBao = nBagYuanBao;
	send.bIncrement = bIncrement;
	g_dbSession.Send(&send, send.dwSize);
}
VOID CurrencyMgr::SendBagGold2DB(DWORD dwRoleID, INT nBagGod, BOOL bIncrement/* = FALSE*/)
{
	tagNDBC_BagGoldUpdate send;
	send.dwRoleID = dwRoleID;
	send.nBagGod = nBagGod;
	send.bIncrement = bIncrement;
	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// 记录金钱log，向LoongDB发消息
//-----------------------------------------------------------------------------
VOID CurrencyMgr::LogSilver(const ELogConType eLogConType, const INT64 n64Num, 
							const INT64 n64TotalNum, const DWORD dwCmdID, const DWORD dwRoleIDRel)
{
	//IMSG(_T("LogSilver1. \r\n"));

	tagNDBC_LogSilver send;
	send.sLogSilver.dwRoleID		= m_pRole->GetID();
	send.sLogSilver.dwAccountID		= m_pRole->GetSession()->GetSessionID();
	send.sLogSilver.dwCmdID			= dwCmdID;
	send.sLogSilver.dwRoleIDRel		= dwRoleIDRel;
	send.sLogSilver.n8LogConType	= (INT8)eLogConType;
	send.sLogSilver.n64Silver		= n64Num;
	send.sLogSilver.n64TotalSilver	= n64TotalNum;

	g_dbSession.Send(&send, send.dwSize);
}

VOID CurrencyMgr::LogWareSilver( const DWORD dwAccountID, const INT64 n64Num, const DWORD dwCmdID )
{
	//IMSG(_T("LogSilver2. \r\n"));

	tagNDBC_LogSilver send;
	send.sLogSilver.dwRoleID		= GT_INVALID;
	send.sLogSilver.dwAccountID		= dwAccountID;
	send.sLogSilver.dwCmdID			= dwCmdID;
	send.sLogSilver.n8LogConType	= (INT8)ELCT_RoleWare;
	send.sLogSilver.n64Silver		= n64Num;
	send.sLogSilver.n64TotalSilver	= GT_INVALID;

	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// 记录元宝log，向LoongDB发消息
//-----------------------------------------------------------------------------
VOID CurrencyMgr::LogYuanBao(const ELogConType eLogConType, const INT nNum, 
							 const INT nTotalNum, const DWORD dwCmdID)
{
	tagNDBC_LogYuanBao send;
	send.sLogYuanBao.dwRoleID		= m_pRole->GetID();
	send.sLogYuanBao.dwAccountID	= m_pRole->GetSession()->GetSessionID();
	send.sLogYuanBao.dwCmdID		= dwCmdID;
	send.sLogYuanBao.n8LogConType	= (INT8)eLogConType;
	send.sLogYuanBao.nYuanBao		= nNum;
	send.sLogYuanBao.nTotalYuanBao	= nTotalNum;

	g_dbSession.Send(&send, send.dwSize);
}

VOID CurrencyMgr::LogBaiBaoYuanBao( const DWORD dwAccountID, const INT nNum, const DWORD dwCmdID )
{
	tagNDBC_LogYuanBao send;
	send.sLogYuanBao.dwRoleID		= GT_INVALID;
	send.sLogYuanBao.dwAccountID	= dwAccountID;
	send.sLogYuanBao.dwCmdID		= dwCmdID;
	send.sLogYuanBao.n8LogConType	= (INT8)ELCT_BaiBao;
	send.sLogYuanBao.nYuanBao		= nNum;
	send.sLogYuanBao.nTotalYuanBao	= GT_INVALID;

	g_dbSession.Send(&send, send.dwSize);
}
VOID CurrencyMgr::LogBagYuanBao(const DWORD dwRoleID, const INT nNum, const DWORD dwCmdID)
{
	tagNDBC_LogYuanBao send;
	send.sLogYuanBao.dwRoleID		= dwRoleID;
	send.sLogYuanBao.dwAccountID	= GT_INVALID;
	send.sLogYuanBao.dwCmdID		= dwCmdID;
	send.sLogYuanBao.n8LogConType	= (INT8)ELCT_Bag;
	send.sLogYuanBao.nYuanBao		= nNum;
	send.sLogYuanBao.nTotalYuanBao	= GT_INVALID;

	g_dbSession.Send(&send, send.dwSize);
}
VOID CurrencyMgr::LogBagGold(const DWORD dwRoleID, const INT nNum, const DWORD dwCmdID)
{
	//IMSG(_T("LogSilver3. \r\n"));

	tagNDBC_LogSilver send;
	send.sLogSilver.dwRoleID		= dwRoleID;
	send.sLogSilver.dwAccountID	= GT_INVALID;
	send.sLogSilver.dwCmdID		= dwCmdID;
	send.sLogSilver.n8LogConType	= (INT8)ELCT_Bag;
	send.sLogSilver.dwRoleIDRel		= GT_INVALID;
	send.sLogSilver.n64Silver		= nNum;
	send.sLogSilver.n64TotalSilver	= GT_INVALID;

	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// 记录赠点log，向LoongDB发消息
//-----------------------------------------------------------------------------
VOID CurrencyMgr::LogExVolume(const INT nNum, const INT nTotalNum, const DWORD dwCmdID)
{
	tagNDBC_LogExVolume send;
	send.sLogExVolume.dwAccountID		= m_pRole->GetSession()->GetSessionID();
	send.sLogExVolume.dwRoleID			= m_pRole->GetID();
	send.sLogExVolume.dwCmdID			= dwCmdID;
	send.sLogExVolume.nExVolume			= nNum;
	send.sLogExVolume.nTotalExVolume	= nTotalNum;

	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// 记录赠点log，向LoongDB发消息
//-----------------------------------------------------------------------------
VOID CurrencyMgr::LogVIPPoint(const INT nNum, const INT nTotalNum, const DWORD dwCmdID)
{ 
	 tagNDBC_LogVIPPoint send;
    
	 send.sLogVIPPoint.dwAccountID		= m_pRole->GetSession()->GetSessionID();
	 send.sLogVIPPoint.dwRoleID			= m_pRole->GetID();
	 send.sLogVIPPoint.dwCmdID			= dwCmdID;
	 send.sLogVIPPoint.nVIPPoint	    = nNum;
	 send.sLogVIPPoint.nTotalVIPPoint	= nTotalNum;

	 g_dbSession.Send(&send, send.dwSize);

}

//-----------------------------------------------------------------------------
// 定时统计玩家货币
//-----------------------------------------------------------------------------
VOID CurrencyMgr::LogTimeStat()
{
	tagNDBC_LogTimeStat send;
	send.sLogTimeStat.dwRoleID			= m_pRole->GetID();
	send.sLogTimeStat.dwAccountID		= m_pRole->GetSession()->GetSessionID();
	send.sLogTimeStat.n64BagSilver		= GetBagSilver();
	send.sLogTimeStat.n64WareSilver		= GetWareSilver();
	send.sLogTimeStat.nBagYuanBao		= GetBagYuanBao();
	send.sLogTimeStat.nBaiBaoYuanBao	= GetBaiBaoYuanBao();
	send.sLogTimeStat.nExVolume			= GetExchangeVolume();
    send.sLogTimeStat.nVIPPoint         = GetVIPPoint();

	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// 获得背包金钱
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::IncBagSilver(INT64 n64Silver, DWORD dwCmdID, DWORD dwRoleIDRel/* = GT_INVALID*/)
{ 
	ASSERT(P_VALID(m_pRole));

	if(n64Silver <= 0)
	{
		return FALSE;
	}

	INT64 n64Inc = m_BagSilver.Gain(n64Silver);

	if(n64Inc > 100000000)
	{
		ILOG->Write(_T("RoleID is:%d, IncMoney is:%.2X"),m_pRole->GetID(),n64Inc);
	}
	// 记录log
	LogSilver(ELCT_Bag, n64Inc, m_BagSilver.GetCur(), dwCmdID, dwRoleIDRel);

	// 向客户端发消息
	tagNS_BagSilver	send;
	send.n64CurSilver		= GetBagSilver();
	send.n64ChangeSilver	= n64Inc;
	SendMessage(&send, send.dwSize);

	// 玩家游戏币增加的事件接口
	g_ScriptMgr.GetRoleScript()->OnSilverChange(m_pRole, n64Silver);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 获得背包元宝
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::IncBagYuanBao(INT32 nYuanBao, DWORD dwCmdID)
{ 
	ASSERT(P_VALID(m_pRole));

	if(!P_VALID(m_pRole)) return FALSE;

	if( g_world.IsUseIPGSystem() && (!m_pRole->GetGetCash() ))
		return FALSE;

	if(nYuanBao <= 0)
	{
		return FALSE;
	}

	INT32 nInc = m_BagYuanBao.Gain(nYuanBao);

	TCHAR szName[32] ={0};
	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(m_pRole->GetID());
	if (!P_VALID(pRoleInfo))
	{
		_tcscpy( szName, pRoleInfo->szRoleName );
	}

	//发送到IPG
	//DWORD dwIP = m_pRole->GetSession()->GetGetIP();
	//in_addr* pAddr = (in_addr*)(&dwIP);
	//char *pszIP = inet_ntoa( *pAddr );
	//g_IpgCashSession.SendIncYuanbaoNum(m_pRole->GetSession()->GetSessionID(),m_pRole->GetID(),szName,pszIP,0,nInc );

	if(!g_world.IsUseIPGSystem())
	{
		// 数据库更新//??
		SendBagYB2DB(m_pRole->GetID(), GetBagYuanBao());
	}
	
	// 记录log
	LogYuanBao(ELCT_Bag, nYuanBao, m_BagYuanBao.GetCur(), dwCmdID);

	// 向客户端发消息
	tagNS_BagYuanBao send;
	send.nCurYuanBao	= GetBagYuanBao();
	send.nChangeYuanBao	= nInc;
	SendMessage(&send, send.dwSize);

	

	return TRUE;
}

//-----------------------------------------------------------------------------
// 获得仓库金钱
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::IncWareSilver(INT64 n64Silver, DWORD dwCmdID)
{ 
	ASSERT(P_VALID(m_pRole));

	if(n64Silver <= 0)
	{
		return FALSE;
	}

	INT64 n64Inc = m_WareSilver.Gain(n64Silver);

	// 保存数据
	SendWareSilver2DB(m_pRole->GetSession()->GetSessionID(), n64Inc);
	
	// 记录log
	LogSilver(ELCT_RoleWare, n64Inc, m_WareSilver.GetCur(), dwCmdID);

	// 向客户端发消息
	tagNS_WareSilver send;
	send.n64CurSilver		= GetWareSilver();
	send.n64ChangeSilver	= n64Inc;
	SendMessage(&send, send.dwSize);

	// 玩家游戏币增加的事件接口
	g_ScriptMgr.GetRoleScript()->OnSilverChange(m_pRole, n64Silver);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 获得百宝元宝
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::IncBaiBaoYuanBao(INT32 nYuanBao, DWORD dwCmdID, BOOL bSaveDB /*= TRUE*/)
{ 
	//使用IPG后，没有了百宝袋元宝的概念
	if(g_world.IsUseIPGSystem())
		return FALSE;

	ASSERT(P_VALID(m_pRole));

	if(nYuanBao <= 0)
	{
		return FALSE;
	}

	INT32 nInc = m_BaiBaoYuanBao.Gain(nYuanBao);

	// 保存数据
	if (bSaveDB)
	{
		SendBaiBaoYB2DB(m_pRole->GetSession()->GetSessionID(), nInc);
	}
	
	// 记录log
	LogYuanBao(ELCT_BaiBao, nInc, m_BaiBaoYuanBao.GetCur(), dwCmdID);

	// 向客户端发消息
	tagNS_BaiBaoYuanBao send;
	send.nCurYuanBao	= GetBaiBaoYuanBao();
	send.nChangeYuanBao	= nInc;
	SendMessage(&send, send.dwSize);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 百宝元宝变更(针对不在线玩家)
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::ModifyBaiBaoYuanBao(DWORD dwRoleID, INT32 nYuanBao, DWORD dwCmdID)
{
	if(g_world.IsUseIPGSystem())
		return FALSE;

	if(nYuanBao == 0)
	{
		// 变更0，没有意义
		return FALSE;
	}

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);

	// 验证该玩家是否存在
	if (!P_VALID(pRoleInfo))
	{
		return FALSE;
	}

	// 保存数据
	SendBaiBaoYB2DB(pRoleInfo->dwAccountID, nYuanBao);

	// 记录log
	LogBaiBaoYuanBao(pRoleInfo->dwAccountID, nYuanBao, dwCmdID);

	// 是否在选人界面
	PlayerSession* pSession = g_worldSession.FindGlobalSession(pRoleInfo->dwAccountID);
	if (P_VALID(pSession))
	{
		INT nBaiBaoYB = pSession->GetBaiBaoYB() + nYuanBao;
		if (nBaiBaoYB < 0)
		{
			nBaiBaoYB = 0;
		}
		pSession->SetBaiBaoYB(nBaiBaoYB);
	}

	return TRUE;
}
BOOL CurrencyMgr::ModifyBagYuanBao(DWORD dwRoleID, INT32 nYuanBao, DWORD dwCmdID)
{
	if(nYuanBao == 0)
	{
		// 变更0，没有意义
		return FALSE;
	}

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);

	// 验证该玩家是否存在
	if (!P_VALID(pRoleInfo))
	{
		return FALSE;
	}

	//发送到IPG
	//g_IpgCashSession.SendDecYuanbaoNum(0,dwRoleID,_T(""),"",0, nYuanBao );

	if(!g_world.IsUseIPGSystem())
	{
		// 保存数据
		SendBagYB2DB(dwRoleID, nYuanBao);
	}

	// 记录log
	LogBagYuanBao(dwRoleID, nYuanBao, dwCmdID);

	return TRUE;
}
BOOL CurrencyMgr::ModifyBagGold(DWORD dwRoleID, INT64 n64Silver, DWORD dwCmdID)
{
	DWORD dwGold = static_cast<INT>(n64Silver/GOLD2SILVER);
	if(dwGold == 0)
	{
		// 变更0，没有意义
		return FALSE;
	}

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);

	// 验证该玩家是否存在
	if (!P_VALID(pRoleInfo))
	{
		return FALSE;
	}

	// 保存数据
	SendBagGold2DB(dwRoleID, dwGold);

	// 记录log
	LogBagGold(dwRoleID, static_cast<INT>(n64Silver), dwCmdID);

	return TRUE;
}

BOOL CurrencyMgr::IncBagYuanBao(DWORD dwRoleID, INT32 nYuanBao, DWORD dwCmdID)
{
	if(nYuanBao == 0)
	{
		// 变更0，没有意义
		return FALSE;
	}

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);

	// 验证该玩家是否存在
	if (!P_VALID(pRoleInfo))
	{
		return FALSE;
	}

	//发送到IPG
	//g_IpgCashSession.SendIncYuanbaoNum(0,dwRoleID,_T(""),"",0, nYuanBao );

	if(!g_world.IsUseIPGSystem())
	{
		// 保存数据
		SendBagYB2DB(dwRoleID, nYuanBao, TRUE);
	}

	// 记录log
	LogBagYuanBao(dwRoleID, nYuanBao, dwCmdID);

	return TRUE;
}

BOOL CurrencyMgr::IncBagGold(DWORD dwRoleID, INT64 n64Silver, DWORD dwCmdID)
{
	DWORD dwGold = static_cast<INT>(n64Silver/GOLD2SILVER);
	if(dwGold == 0)
	{
		// 变更0，没有意义
		return FALSE;
	}

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);

	// 验证该玩家是否存在
	if (!P_VALID(pRoleInfo))
	{
		return FALSE;
	}

	// 保存数据
	SendBagGold2DB(dwRoleID, dwGold, TRUE);

	// 记录log
	LogBagGold(dwRoleID, static_cast<INT>(n64Silver), dwCmdID);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 仓库金钱变更(针对不在线玩家)
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::ModifyWareSilver( DWORD dwRoleID, INT64 n64Silver, DWORD dwCmdID )
{
	if(n64Silver == 0)
	{
		// 变更0，没有意义
		return FALSE;
	}

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);

	// 验证该玩家是否存在
	if (!P_VALID(pRoleInfo))
	{
		return FALSE;
	}

	// 保存数据
	SendWareSilver2DB(pRoleInfo->dwAccountID, n64Silver);

	// 记录log
	LogWareSilver(pRoleInfo->dwAccountID, n64Silver, dwCmdID);

	// 是否在选人界面
	PlayerSession* pSession = g_worldSession.FindGlobalSession(pRoleInfo->dwAccountID);
	if (P_VALID(pSession))
	{
		INT64 n64WareSilver = pSession->GetWareSilver() + n64Silver;
		if (n64WareSilver < 0)
		{
			n64WareSilver = 0;
		}
		pSession->SetWareSilver(n64WareSilver);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// 获得赠点
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::IncExchangeVolume(INT32 nExVolume, DWORD dwCmdID)
{
	ASSERT(P_VALID(m_pRole));

	if(nExVolume <= 0)
	{
		return FALSE;
	}

	INT32 nInc = m_ExchangeVolume.Gain(nExVolume);

	// 数据库更新//??

	// 记录log
	LogExVolume(nInc, m_ExchangeVolume.GetCur(), dwCmdID);

	// 向客户端发消息
	tagNS_ExchangeVolume send;
	send.nCurExVolume		= GetExchangeVolume();
	send.nChangeExVolume	= nInc;
	SendMessage(&send, send.dwSize);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 获得积分
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::IncVIPPoint(INT32 nVIPPoint, DWORD dwCmdID)
{
	ASSERT(P_VALID(m_pRole));

	if(nVIPPoint <= 0)
	{
		return FALSE;
	}  
	INT32  nInc  = m_VIPPoint.Gain(nVIPPoint);

	LogVIPPoint(nInc,m_VIPPoint.GetCur(),dwCmdID);
    
	tagNS_Mark send;
	send.nCurMark		= GetVIPPoint();
	send.nChangeMark	= nInc;
	SendMessage(&send, send.dwSize);

	return TRUE;
}

BOOL CurrencyMgr::IncClanCon(INT32 nIncr, DWORD dwCmdID, ECLanType eClanType)
{
	if (nIncr <= 0)
	{
		return FALSE;
	}
	

	tagNS_ClanCon send;
	send.byClanType		= eClanType;
	send.nChangeClanCon = m_pRoleClanData->ClanConInc(nIncr, eClanType);
	send.nCurClanCon	= GetClanCon(eClanType);	
	SendMessage(&send, send.dwSize);
	return TRUE;
}

//-----------------------------------------------------------------------------
// 失去背包金钱
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::DecBagSilver(INT64 n64Silver, DWORD dwCmdID, DWORD dwRoleIDRel/* = GT_INVALID*/)
{ 
	ASSERT(P_VALID(m_pRole));

	if(n64Silver <= 0)
	{
		return FALSE;
	}

	INT64 n64CurMoney = m_BagSilver.GetCur();
	if(n64Silver > n64CurMoney)
	{
		ILOG->Write(_T("RoleID is:%d, ExgMoney is:%.2X,Cur Money is:%.2X"),m_pRole->GetID(),n64Silver,n64CurMoney);
		return FALSE;
	}

	if(n64Silver > 100000000)
	{
		//ILOG->Write(_T("RoleID is:%d, ExgMoney1 is:%.2X,Cur Money is:%.2X"),m_pRole->GetID(),n64Silver,n64CurMoney);
		//return FALSE;
	}

	INT64 n64Dec = m_BagSilver.Spend(n64Silver);

	// 记录log
	LogSilver(ELCT_Bag, -n64Dec, m_BagSilver.GetCur(), dwCmdID, dwRoleIDRel);

	// 向客户端发消息
	tagNS_BagSilver	send;
	send.n64CurSilver		= GetBagSilver();
	send.n64ChangeSilver	= -n64Dec;
	SendMessage(&send, send.dwSize);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 失去背包元宝
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::DecBagYuanBao(INT32 nYuanBao, DWORD dwCmdID )
{ 
	ASSERT(P_VALID(m_pRole));

	if(!P_VALID(m_pRole)) return FALSE;

	if( g_world.IsUseIPGSystem() && (!m_pRole->GetGetCash() ))
		return FALSE;

	if(nYuanBao <= 0)
	{
		return FALSE;
	}

	INT32 nDec = m_BagYuanBao.Spend(nYuanBao);

	//TCHAR szName[32] ={0};
	//tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(m_pRole->GetID());
	//if (!P_VALID(pRoleInfo))
	//{
	//	_tcscpy( szName, pRoleInfo->szRoleName );
	//}
	//发送到IPG

	//DWORD dwIP = m_pRole->GetSession()->GetGetIP();
	//in_addr* pAddr = (in_addr*)(&dwIP);
	//char *pszIP = inet_ntoa( *pAddr );
	//g_IpgCashSession.SendDecYuanbaoNum(m_pRole->GetSession()->GetSessionID(),m_pRole->GetID(),szName,pszIP,dwItemID, nDec );

	if(!g_world.IsUseIPGSystem())
	{
		// 数据库更新//??
		SendBagYB2DB(m_pRole->GetID(), GetBagYuanBao());
	}
	
	// 记录log
	LogYuanBao(ELCT_Bag, -nYuanBao, m_BagYuanBao.GetCur(), dwCmdID);

	// 向客户端发消息
	tagNS_BagYuanBao send;
	send.nCurYuanBao	= GetBagYuanBao();
	send.nChangeYuanBao	= -nDec;
	SendMessage(&send, send.dwSize);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 失去仓库金钱
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::DecWareSilver(INT64 n64Silver, DWORD dwCmdID)
{ 
	ASSERT(P_VALID(m_pRole));

	if(n64Silver <= 0)
	{
		return FALSE;
	}

	INT64 n64Dec = m_WareSilver.Spend(n64Silver);

	// 保存数据库
	SendWareSilver2DB(m_pRole->GetSession()->GetSessionID(), -n64Dec);

	// 记录log
	LogSilver(ELCT_RoleWare, -n64Silver, m_WareSilver.GetCur(), dwCmdID);

	// 向客户端发消息
	tagNS_WareSilver	send;
	send.n64CurSilver		= GetWareSilver();
	send.n64ChangeSilver	= -n64Dec;
	SendMessage(&send, send.dwSize);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 失去百宝元宝
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::DecBaiBaoYuanBao(INT32 nYuanBao, DWORD dwCmdID)
{ 
	if(g_world.IsUseIPGSystem())
		return FALSE;
	
	ASSERT(P_VALID(m_pRole));

	if(nYuanBao <= 0)
	{
		return FALSE;
	}

	INT32 nDec = m_BaiBaoYuanBao.Spend(nYuanBao);

	// 保存数据库
	SendBaiBaoYB2DB(m_pRole->GetSession()->GetSessionID(), -nDec);
	
	// 数据库更新//??

	// 记录log
	LogYuanBao(ELCT_BaiBao, -nDec, m_BaiBaoYuanBao.GetCur(), dwCmdID);

	// 向客户端发消息
	tagNS_BaiBaoYuanBao send;
	send.nCurYuanBao	= GetBaiBaoYuanBao();
	send.nChangeYuanBao	= -nDec;
	SendMessage(&send, send.dwSize);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 失去赠点
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::DecExchangeVolume(INT32 nExVolume, DWORD dwCmdID)
{
	ASSERT(P_VALID(m_pRole));

	if(nExVolume <= 0)
	{
		return FALSE;
	}

	INT32 nDec = m_ExchangeVolume.Spend(nExVolume);

	// 记录log
	LogExVolume(-nDec, m_ExchangeVolume.GetCur(), dwCmdID);

	// 向客户端发消息
	tagNS_ExchangeVolume	send;
	send.nCurExVolume		= GetExchangeVolume();
	send.nChangeExVolume	= -nDec;

	SendMessage(&send, send.dwSize);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 失去积分
//-----------------------------------------------------------------------------
BOOL CurrencyMgr::DecVIPPoint(INT32 nVIPPoint, DWORD dwCmdID)
{
	ASSERT(P_VALID(m_pRole));

	if(nVIPPoint <= 0)
	{
		return FALSE;
	}

	INT32 nDec = m_VIPPoint.Spend(nVIPPoint);

	LogVIPPoint(-nDec, m_VIPPoint.GetCur(), dwCmdID);

	tagNS_Mark send;
	send.nCurMark		= GetVIPPoint();
	send.nChangeMark	= -nDec;
	SendMessage(&send, send.dwSize);

	return TRUE;
       
}

BOOL CurrencyMgr::DecClanCon(INT32 nClanCon, DWORD dwCmdID, ECLanType eClanType)
{
	if (nClanCon <= 0)
	{
		return FALSE;
	}

	tagNS_ClanCon send;
	send.byClanType		= eClanType;
	send.nChangeClanCon = -m_pRoleClanData->ClanConDec(nClanCon, eClanType);
	send.nCurClanCon	= GetClanCon(eClanType);	
	SendMessage(&send, send.dwSize);
	return TRUE;
}

//------------------------------------------------------------------------------
// 检查身上是否有足够的金钱、元宝、贡献等
//------------------------------------------------------------------------------
BOOL CurrencyMgr::IsEnough(ECurCostType eCurType, INT32 nNeed)
{
	ASSERT(nNeed > 0);
	
	switch(eCurType)
	{
	case ECCT_BagSilver:
		return (GetBagSilver() >= nNeed);
		break;
	case ECCT_BagYuanBao:
		return (GetBagYuanBao() >= nNeed);
		break;
	case ECCT_WareSilver:
		return (GetWareSilver() >= nNeed);
		break;
	case ECCT_BaiBaoYuanBao:
		return (GetBaiBaoYuanBao() >= nNeed);
		break;
	case ECCT_ExchangeVolume:
		return (GetExchangeVolume() >= nNeed);
		break;

	case ECCT_WarMoney:
		return (GetWarMoney() >= nNeed);
		break;
	case ECCT_ClanConXuanYuan:
	case ECCT_ClanConShenNong:
	case ECCT_ClanConFuXi:
	case ECCT_ClanConSanMiao:
	case ECCT_ClanConJiuLi:
	case ECCT_ClanConYueZhi:
	case ECCT_ClanConNvWa:
	case ECCT_ClanConGongGong:
		ECLanType eClanType = MTRANS_ECCT2ECLT(eCurType);
		return ( GetClanCon(eClanType) >= nNeed );
		break;

	}

	return FALSE;
}

//------------------------------------------------------------------------------
// 扣除指定货币
//------------------------------------------------------------------------------
BOOL CurrencyMgr::DecCurrency(ECurCostType eCurType, INT32 nNeed, DWORD dwCmdID)
{
	ASSERT(nNeed > 0);

	switch(eCurType)
	{
	case ECCT_BagSilver:
		if(GetBagSilver() >= nNeed)
		{
			return DecBagSilver(nNeed, dwCmdID);
		}
		break;
	case ECCT_BagYuanBao:
		if(GetBagYuanBao() >= nNeed)
		{
			if(!P_VALID(m_pRole))
				return FALSE;

			if( g_world.IsUseIPGSystem() && (!m_pRole->GetGetCash()) )
				return FALSE;

			if( g_world.IsUseIPGSystem() )
			{
				if(m_pRole->GetGetCash())
				{
					TCHAR tszTemp[64] = {0};
					wsprintf(tszTemp,_T("DecIPGCash2. Dec cash:%d. \r\n"),nNeed);
					g_world.WriteKroeaLog(tszTemp);

					return m_pRole->DecIPGCash(nNeed);
				}
				else
				{
					return FALSE;
				}
			}
			else
			{
				return DecBagYuanBao(nNeed, dwCmdID);
			}
		}
		break;
	case ECCT_WareSilver:
		if(GetWareSilver() >= nNeed)
		{
			return DecWareSilver(nNeed, dwCmdID);
		}
		break;
	case ECCT_BaiBaoYuanBao:
		{
			if( g_world.IsUseIPGSystem() )
				return FALSE;

			if(GetBaiBaoYuanBao() >= nNeed)
			{
				return DecBaiBaoYuanBao(nNeed, dwCmdID);
			}
		}
		break;

	case ECCT_ExchangeVolume:
		if (GetExchangeVolume() >= nNeed)
		{
			return DecExchangeVolume(nNeed, dwCmdID);
		}
		break;

	case ECCT_ClanConXuanYuan:
	case ECCT_ClanConShenNong:
	case ECCT_ClanConFuXi:
	case ECCT_ClanConSanMiao:
	case ECCT_ClanConJiuLi:
	case ECCT_ClanConYueZhi:
	case ECCT_ClanConNvWa:
	case ECCT_ClanConGongGong:
		{
			ECLanType eClanType = MTRANS_ECCT2ECLT(eCurType);
			if (m_pRoleClanData->ClanConGetVal(eClanType) >= nNeed)
			{
				return DecClanCon(nNeed, dwCmdID, eClanType);
			}
			break;
		}
	case ECCT_WuJi:
		if(m_pRole->GetAttValue(ERA_WuJi) >= nNeed)
		{
			m_pRole->SetAttValue(ERA_WuJi, m_pRole->GetAttValue(ERA_WuJi)-nNeed, TRUE);
			return TRUE;
		}
		break;
	case ECCT_GuildContribe:
	case ECCT_GuildExploit:
		if (DecContribeOrExploit(eCurType, nNeed))
			return TRUE;
		break;
	case ECCT_WarMoney:
		AddWarMoney(-nNeed);
		break;
	}

	return FALSE;
}
INT32 CurrencyMgr::GetClanCon(ECLanType eClanType) const 
{ 
	return m_pRoleClanData->ClanConGetVal(eClanType); 
}

INT32 CurrencyMgr::GetCanIncClanCon(ECLanType eClanType)	const 
{ 
	return GetMaxClanCon(eClanType) - GetClanCon(eClanType); 
}
INT32 CurrencyMgr::GetMaxClanCon(ECLanType eClanType)		const 
{
	return m_pRoleClanData->ClanConGetMaxVal(eClanType);
}

BOOL CurrencyMgr::DecContribeOrExploit( ECurCostType eCurType, INT32 nNeed )
{
	if (eCurType != ECCT_GuildContribe && eCurType != ECCT_GuildExploit)
		return FALSE;

	Guild* pGuild = g_guildMgr.GetGuild(m_pRole->GetGuildID());
	if (P_VALID(pGuild))
	{
		DWORD dwRoleID = m_pRole->GetID();
		tagGuildMember* pGuildMember = pGuild->GetMemberMgr().GetMember(dwRoleID);
		if (P_VALID(pGuildMember))
		{
			if (eCurType == ECCT_GuildContribe)
			{
				if (pGuildMember->nContribution >= nNeed)
				{
					pGuild->GetMemberMgr().DecMemberContribution(dwRoleID, nNeed);
					return TRUE;
				}
			}
			else if (eCurType == ECCT_GuildExploit)
			{
				if (pGuildMember->nExploit >= nNeed)
				{
					INT32 n32ExploitDec = 0-nNeed;
					pGuild->GetMemberMgr().SetMemberExploit(dwRoleID, n32ExploitDec);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

VOID CurrencyMgr::SetCurBagYuanBao(INT32 nBagYuanBao)
{
	m_BagYuanBao.SetMoney(nBagYuanBao);
}
	
VOID CurrencyMgr::SetCurBaiBaoYuanBao(INT32 nBaiBaoYuanBao)
{
	m_BaiBaoYuanBao.SetMoney(nBaiBaoYuanBao);
}

BOOL CurrencyMgr::SendBagYuanBao()
{ 
	ASSERT(P_VALID(m_pRole));

	if(!P_VALID(m_pRole)) 
		return FALSE;

	// 向客户端发消息
	tagNS_BagYuanBao send;
	send.nCurYuanBao	= GetBagYuanBao();
	send.nChangeYuanBao	= 0;
	SendMessage(&send, send.dwSize);

	return TRUE;
}

BOOL CurrencyMgr::SendBaibaoYuanBao()
{ 
	ASSERT(P_VALID(m_pRole));

	if(!P_VALID(m_pRole)) 
		return FALSE;

	// 向客户端发消息
	tagNS_BaiBaoYuanBao send;
	send.nCurYuanBao	= GetBaiBaoYuanBao();
	send.nChangeYuanBao	= 0;
	SendMessage(&send, send.dwSize);

	return TRUE;
}

DWORD CurrencyMgr::GetWarMoney()
{
	if(!P_VALID(m_pRole))
		return 0;

	WarRoleMgr* pWarRoleMgr = m_pRole->GetMap()->GetWarRoleMgr();
	if(!P_VALID(pWarRoleMgr))
		return 0;

	WarRole* pWarRole = pWarRoleMgr->GetWarRole(m_pRole->GetID());
	if(!P_VALID(pWarRole))
		return 0;

	return pWarRole->GetGold();
}

VOID CurrencyMgr::AddWarMoney(DWORD dwWarMoney)
{
	if(!P_VALID(m_pRole))
		return;

	WarRoleMgr* pWarRoleMgr = m_pRole->GetMap()->GetWarRoleMgr();
	if(!P_VALID(pWarRoleMgr))
		return;

	WarRole* pWarRole = pWarRoleMgr->GetWarRole(m_pRole->GetID());
	if(!P_VALID(pWarRole))
		return;

	pWarRole->AddGlod(dwWarMoney);
}