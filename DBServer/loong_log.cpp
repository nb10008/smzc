//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loong_log.cpp
// author: Sxg
// actor:
// data: 2009-03-24
// last:
// brief: log相关处理 -- 消息处理
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "Loong.h"
#include "LoongDB.h"
#include "../ServerDefine/guild_define.h"
#include "../ServerDefine/msg_log.h"

#include "log_command.h"

//-----------------------------------------------------------------------------
// 创建新表
//-----------------------------------------------------------------------------
VOID CLoong::CreateCurDataLogTable()
{	
	IMSG(_T("\n\n"));

	CHAR szTime[8] = {'\0'};
	tagDWORDTime dwTime = GetCurrentDWORDTime();
	sprintf_s(szTime, sizeof(szTime), "_%02d%02d", dwTime.month, dwTime.day);

	// 创建新表
	if(m_pDB->CreateLogMoneyTable(szTime)
		&& m_pDB->CreateLogYuanBaoTable(szTime)
		&& m_pDB->CreateLogExVolumeTable(szTime)
		&& m_pDB->CreateLogVIPPointTable(szTime)
		&& m_pDB->CreateLogGodBidTable(szTime)
		&& m_pDB->CreateLogBattlefieldTable(szTime)
		&& m_pDB->CreateLogTimeStatTable(szTime)
		&& m_pDB->CreateLogItemTable(szTime)
		&& m_pDB->CreateLogItemTimesTable(szTime)
		&& m_pDB->CreateLogMallSellTable(szTime)
		&& m_pDB->CreateLogMallSellPackTable(szTime)
		&& m_pDB->CreateLogFundTable(szTime)
		&& m_pDB->CreateLogMaterialTable(szTime)
		&& m_pDB->CreateLogReputationTable(szTime)
		&& m_pDB->CreateLogGMCmd(szTime)
		&& m_pDB->CreateLogFairyBirthTable(szTime)
		&& m_pDB->CreateLogPet(szTime)
		&& m_pDB->CreateLogRoleOnline(szTime)
		&& m_pDB->CreateLogRoleYuanbaoCost(szTime)
		&& m_pDB->CreateLogAccountOnline(szTime)
		&&m_pDB->CreateLogHolyEquipConsolidateTable(szTime)
		&&m_pDB->CreateLogKillRoleTable(szTime))
	{
		m_dwLogTime = dwTime;
		sprintf_s(m_szLogFilePostfix, sizeof(m_szLogFilePostfix), szTime);
	}

	IMSG(_T("\n\n"));
}

//-----------------------------------------------------------------------------
// 金钱
//-----------------------------------------------------------------------------
DWORD CLoong::LogSilver(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogSilver);

	//IMSG(_T("LogSilver \r\n"));

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertMoneyLog(m_szLogFilePostfix, &p->sLogSilver);
#else
	DBCmdCall(_LogSilver,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 元宝
//-----------------------------------------------------------------------------
DWORD CLoong::LogYuanBao(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogYuanBao);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertYuanBaoLog(m_szLogFilePostfix, &p->sLogYuanBao);
#else
	DBCmdCall(_LogYuanBao,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 赠点
//-----------------------------------------------------------------------------
DWORD CLoong::LogExVolume(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogExVolume);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertExVolumeLog(m_szLogFilePostfix, &p->sLogExVolume);
#else
	DBCmdCall(_LogExVolume,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 积分
//-----------------------------------------------------------------------------
DWORD CLoong::LogVIPPoint(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogVIPPoint);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertVIPPointLog(m_szLogFilePostfix,&p->sLogVIPPoint);
#else
	DBCmdCall(_LogVIPPoint,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 神之竞标
//-----------------------------------------------------------------------------
DWORD CLoong::LogGodBid(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogGodBid);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertGodBidLog(m_szLogFilePostfix,&p->sLogGodBid);
#else
	DBCmdCall(_LogGodBid,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 战场占领记录
//-----------------------------------------------------------------------------
DWORD CLoong::LogBattlefield(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogBattlefield);
	
#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertBattlefieldLog(m_szLogFilePostfix,&p->sLogBattlefield);
#else
	DBCmdCall(_LogBattlefield,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 定时统计
//-----------------------------------------------------------------------------
DWORD CLoong::LogTimeStat(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogTimeStat);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertTimeStatLog(m_szLogFilePostfix, &p->sLogTimeStat);
#else
	DBCmdCall(_LogTimeStat,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 物品log
//-----------------------------------------------------------------------------
DWORD CLoong::LogItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogItem);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertItemLog(m_szLogFilePostfix, &p->sLogItem);
#else
	DBCmdCall(_LogItem,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 商城出售log
//-----------------------------------------------------------------------------
DWORD CLoong::LogMallSell(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogMallSell);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertMallSellLog(m_szLogFilePostfix, &p->sLogMallSell);
#else
	DBCmdCall(_LogMallSell,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 商城出售礼品包log
//-----------------------------------------------------------------------------
DWORD CLoong::LogMallSellPack(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogMallSellPack);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertMallSellPackLog(m_szLogFilePostfix, &p->sLogMallSellPack);
#else
	DBCmdCall(_LogMallSellPack,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 商城出售log
//-----------------------------------------------------------------------------
DWORD CLoong::LogItemTimes(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogItemTimes);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertItemTimesLog(m_szLogFilePostfix, &p->sLogItemTimes);
#else
	DBCmdCall(_LogItemTimes,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 帮派资金log
//-----------------------------------------------------------------------------
DWORD CLoong::LogFund(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogFund);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertFundLog(m_szLogFilePostfix, &p->sLogFund);
#else
	DBCmdCall(_LogFund,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 帮派资材log
//-----------------------------------------------------------------------------
DWORD CLoong::LogMaterial(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogMaterial);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertMaterialLog(m_szLogFilePostfix, &p->sLogMaterial);
#else
	DBCmdCall(_LogMaterial,pDBMsg);
#endif

	return 0;
}

//-----------------------------------------------------------------------------
// 帮派声望log
//-----------------------------------------------------------------------------
DWORD CLoong::LogReputation(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogReputation);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertReputationLog(m_szLogFilePostfix, &p->sLogReputation);
#else
	DBCmdCall(_LogReputation,pDBMsg);
#endif

	return 0;
}

DWORD CLoong::LogGMCmd(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogGMCmd);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertGMCmdLog(m_szLogFilePostfix, &p->sLogGMCmd);
#else
	DBCmdCall(_LogGMCmd,pDBMsg);
#endif

	return 0;
}

DWORD CLoong::LogPet(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogPet);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertPetLog(m_szLogFilePostfix, &p->sLogPet);
#else
	DBCmdCall(_LogPet,pDBMsg);
#endif

	return 0;
}

DWORD CLoong::LogFairyBirth(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogFairyBirth);

#ifndef _USE_OTHER_THREAD_
	m_pDB->InsertFairyBirthLog(m_szLogFilePostfix, &p->sLogFairyBirth);
#else
	DBCmdCall(_LogFairyBirth,pDBMsg);
#endif

	return 0;
}

DWORD CLoong::LogRoleOnlineInfo(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogRoleOnlineInfo);
	m_pDB->InsertRoleOnlineLog(m_szLogFilePostfix, &p->sLogRoleOnline);
	return 0;
}
// 实际处理log的代码

DWORD CLoong::LogWorldData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogWorldData);
	m_pDB->UpdateWorldDataLog(m_szLogFilePostfix, &p->dwData);
	return 0;
}

DWORD CLoong::LogRolelogData(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogRoleData);
	m_pDB->InsertRoleDataLog(m_szLogFilePostfix, p->dwRoleID, p->dwData);
	return 0;
}

//-----------------------------------------------------------------------------
// 金钱
//-----------------------------------------------------------------------------
DWORD CLoong::_LogSilver(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogSilver);

	m_pDB->InsertMoneyLog(m_szLogFilePostfix, &p->sLogSilver);

	return 0;
}

//-----------------------------------------------------------------------------
// 元宝
//-----------------------------------------------------------------------------
DWORD CLoong::_LogYuanBao(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogYuanBao);

	m_pDB->InsertYuanBaoLog(m_szLogFilePostfix, &p->sLogYuanBao);

	return 0;
}

//-----------------------------------------------------------------------------
// 赠点
//-----------------------------------------------------------------------------
DWORD CLoong::_LogExVolume(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogExVolume);

	m_pDB->InsertExVolumeLog(m_szLogFilePostfix, &p->sLogExVolume);

	return 0;
}

//-----------------------------------------------------------------------------
// 积分
//-----------------------------------------------------------------------------
DWORD CLoong::_LogVIPPoint(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogVIPPoint);

	m_pDB->InsertVIPPointLog(m_szLogFilePostfix,&p->sLogVIPPoint);

	return 0;
}

//-----------------------------------------------------------------------------
// 神之竞标
//-----------------------------------------------------------------------------
DWORD CLoong::_LogGodBid(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogGodBid);
	
	m_pDB->InsertGodBidLog(m_szLogFilePostfix,&p->sLogGodBid);

	return 0;
}

//-----------------------------------------------------------------------------
// 战场占领记录
//-----------------------------------------------------------------------------
DWORD CLoong::_LogBattlefield(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogBattlefield);
	
	m_pDB->InsertBattlefieldLog(m_szLogFilePostfix,&p->sLogBattlefield);

	return 0;
}

//-----------------------------------------------------------------------------
// 定时统计
//-----------------------------------------------------------------------------
DWORD CLoong::_LogTimeStat(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogTimeStat);

	m_pDB->InsertTimeStatLog(m_szLogFilePostfix, &p->sLogTimeStat);

	return 0;
}

//-----------------------------------------------------------------------------
// 物品log
//-----------------------------------------------------------------------------
DWORD CLoong::_LogItem(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogItem);

	m_pDB->InsertItemLog(m_szLogFilePostfix, &p->sLogItem);

	return 0;
}

//-----------------------------------------------------------------------------
// 商城出售log
//-----------------------------------------------------------------------------
DWORD CLoong::_LogMallSell(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogMallSell);

	m_pDB->InsertMallSellLog(m_szLogFilePostfix, &p->sLogMallSell);

	return 0;
}

//-----------------------------------------------------------------------------
// 商城出售礼品包log
//-----------------------------------------------------------------------------
DWORD CLoong::_LogMallSellPack(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogMallSellPack);

	m_pDB->InsertMallSellPackLog(m_szLogFilePostfix, &p->sLogMallSellPack);

	return 0;
}

//-----------------------------------------------------------------------------
// 商城出售log
//-----------------------------------------------------------------------------
DWORD CLoong::_LogItemTimes(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogItemTimes);

	m_pDB->InsertItemTimesLog(m_szLogFilePostfix, &p->sLogItemTimes);

	return 0;
}

//-----------------------------------------------------------------------------
// 帮派资金log
//-----------------------------------------------------------------------------
DWORD CLoong::_LogFund(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogFund);

	m_pDB->InsertFundLog(m_szLogFilePostfix, &p->sLogFund);

	return 0;
}

//-----------------------------------------------------------------------------
// 帮派资材log
//-----------------------------------------------------------------------------
DWORD CLoong::_LogMaterial(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogMaterial);

	m_pDB->InsertMaterialLog(m_szLogFilePostfix, &p->sLogMaterial);

	return 0;
}

//-----------------------------------------------------------------------------
// 帮派声望log
//-----------------------------------------------------------------------------
DWORD CLoong::_LogReputation(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogReputation);

	m_pDB->InsertReputationLog(m_szLogFilePostfix, &p->sLogReputation);

	return 0;
}

DWORD CLoong::_LogGMCmd(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogGMCmd);

	m_pDB->InsertGMCmdLog(m_szLogFilePostfix, &p->sLogGMCmd);

	return 0;
}

DWORD CLoong::_LogPet(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogPet);

	m_pDB->InsertPetLog(m_szLogFilePostfix, &p->sLogPet);

	return 0;
}

DWORD CLoong::_LogFairyBirth(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogFairyBirth);

	m_pDB->InsertFairyBirthLog(m_szLogFilePostfix, &p->sLogFairyBirth);

	return 0;
}

DWORD CLoong::LogAccountOnlineTime(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogAccountOnlineTime);

	m_pDB->InsertAccountOnlineTimeLog(m_szLogFilePostfix, p);

	return 0;
}

DWORD CLoong::LogHolyEquipCon(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogHolyEquipCon);
	m_pDB->InsertHolyEquipConLog(m_szLogFilePostfix,&p->sLogHolyEquip);
	return 0;
}

DWORD CLoong::LogKillrole(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(p, pDBMsg, NDBC_LogKillrole);
	m_pDB->InsertKillRoleLog(m_szLogFilePostfix, p);
	return 0;
}
