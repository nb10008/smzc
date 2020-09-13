//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loongdb_log.cpp
// author: Sxg
// actor:
// data: 2009-03-24
// last:
// brief: log相关处理 -- db层
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "LoongDB.h"
#include "loongRank.h"
#include "../ServerDefine/guild_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/data_collect.h"
#include "../ServerDefine/msg_log.h"
//-----------------------------------------------------------------------------
// log表名称定义
//-----------------------------------------------------------------------------
LPCSTR TableName_LogMoney				= "log_money";
LPCSTR TableName_LogYuanBao				= "log_yuanbao";
LPCSTR TableName_LogExVolume			= "log_exvolume";
LPCSTR TableName_LogVIPPoint			= "log_vippoint";
LPCSTR TableName_LogGodBid			= "log_godbid";
LPCSTR TableName_LogBattlefield		= "log_battlefield";
LPCSTR TableName_LogTimeStat			= "log_time_stat";
LPCSTR TableName_LogItem				= "log_item";
LPCSTR TableName_LogMallSell			= "log_mallsell";
LPCSTR TableName_LogMallSellPack		= "log_mallsell_pack";
LPCSTR TableName_LogItemTimes			= "log_itemtimes";
LPCSTR TableName_LogFund				= "log_fund";
LPCSTR TableName_LogMaterial			= "log_material";
LPCSTR TableName_LogReputation			= "log_reputation";
LPCSTR TableName_LogGMCmd				= "log_gmcmd";
LPCSTR TableName_LogPet					= "log_pet";
LPCSTR TableName_LogFairyBirth			= "log_fairybirth";
LPCSTR TableName_LogRoleOnline		= "log_role_online";
LPCSTR TableName_LogRoleYuanbaoCost		= "log_role_yuanbaocost";
LPCSTR TableName_LogAccountOnline = "log_account_online";
LPCSTR TableName_LogHolyEquipConsolidate = "log_holyequipconsolidate"; 
LPCSTR TableName_LogKillRole = "log_killrole"; 

// 以下base库里的
LPCSTR TableName_LogWorldData		= "log_world_data";
LPCSTR TableName_LogRoleData		= "log_role_data";

//-----------------------------------------------------------------------------
// 创建log表通用方法
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogTable(LPCSTR strTable, LPCSTR strPostfix)
{
	// 获取流
	Beton::MyStream* pStream = m_pDBLog->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();
	pStream->FillString("CREATE TABLE IF NOT EXISTS ");
	pStream->FillString(strTable);
	pStream->FillString(strPostfix);
	pStream->FillString(" LIKE ");
	pStream->FillString(strTable);

	if(!m_pDBLog->Execute(pStream))
	{
		// 向监控发消息

		IMSG(_T("\n\n\nLog File<%s"), m_pUtil->Unicode8ToUnicode(strTable));
		IMSG(_T("%s> create failed!\n\n\n"), m_pUtil->Unicode8ToUnicode(strPostfix));

		// 释放流
		m_pDBLog->ReturnStream(pStream);

		return FALSE;
	}

	IMSG(_T("Log File<%s"), m_pUtil->Unicode8ToUnicode(strTable));
	IMSG(_T("%s> create successful!\n"), m_pUtil->Unicode8ToUnicode(strPostfix));

	// 释放流
	m_pDBLog->ReturnStream(pStream);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 金钱记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogMoneyTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogMoney, strPostfix);
}

VOID CLoongDB::InsertMoneyLog(LPCSTR szTablePostfix, LPVOID pData)
{
	//IMSG(_T("InsertMoneyLog. \r\n"));
#ifdef WRITE_KOREALOG
	TCHAR tszTemp[64] = {0};
	wsprintf( tszTemp, _T("InsertMoneyLog, %s.\r\n"), szTablePostfix);
	WriteKroeaLog(tszTemp);
#endif

	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertMoneyLog));
}

VOID CLoongDB::FormatInsertMoneyLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogSilver);

	pStream->SetInsert(TableName_LogMoney, szTablePostfix);

	pStream->FillString("AccountID=") << p->dwAccountID;
	pStream->FillString(",RoleID=") << p->dwRoleID;
	pStream->FillString(",CmdID=") << p->dwCmdID;
	pStream->FillString(",ConType=") << p->n8LogConType;
	pStream->FillString(",Num=") << p->n64Silver;
	pStream->FillString(",TotalNum=") << p->n64TotalSilver;
	pStream->FillString(",RoleIDRel=") << p->dwRoleIDRel;
	pStream->FillString(",Time=now()");
}

//-----------------------------------------------------------------------------
// 元宝记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogYuanBaoTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogYuanBao, strPostfix);
}

VOID CLoongDB::InsertYuanBaoLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertYuanBaoLog));

	MTRANS_POINTER(p, pData, tagLogYuanBao);
	if (!P_VALID(p))
	{
		return;
	}

	if (p->dwCmdID == ELCID_Godbid_NormalBid_Cost
		|| p->dwCmdID == ELCID_Godbid_NormalBid_Added
		|| p->dwCmdID == ELCID_Godbid_BlackBid_Cost
		|| p->dwCmdID == ELCID_Godbid_BlackBid_Added)
	{
		InsertRoleCostLog(szTablePostfix, p->dwAccountID);
		UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertRoleGodbidCostLog));
	}
}

VOID CLoongDB::FormatInsertYuanBaoLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogYuanBao);

	pStream->SetInsert(TableName_LogYuanBao, szTablePostfix);

	pStream->FillString("AccountID=") << p->dwAccountID;
	pStream->FillString(",RoleID=") << p->dwRoleID;
	pStream->FillString(",CmdID=") << p->dwCmdID;
	pStream->FillString(",ConType=") << p->n8LogConType;
	pStream->FillString(",Num=") << p->nYuanBao;
	pStream->FillString(",TotalNum=") << p->nTotalYuanBao;
	pStream->FillString(",Time=now()");
}

//-----------------------------------------------------------------------------
// 赠点记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogExVolumeTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogExVolume, strPostfix);
}

//-----------------------------------------------------------------------------
// 积分记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogVIPPointTable(LPCSTR strPostfix)
{
    return CreateLogTable(TableName_LogVIPPoint, strPostfix);
}

BOOL CLoongDB::CreateLogGodBidTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogGodBid, strPostfix);
}

BOOL CLoongDB::CreateLogBattlefieldTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogBattlefield, strPostfix);
}
BOOL CLoongDB::CreateLogFairyBirthTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogFairyBirth, strPostfix);
}

VOID CLoongDB::InsertVIPPointLog(LPCSTR szTablePostfix, LPVOID pData)
{
     UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertVIPPointLog));
}

VOID CLoongDB::InsertGodBidLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertGodBidLog));
}

VOID CLoongDB::InsertBattlefieldLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertBattlefieldLog));
}
VOID CLoongDB::InsertFairyBirthLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertFairyBirthLog));
}

VOID CLoongDB::InsertExVolumeLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertExVolumeLog));
}

VOID CLoongDB::InsertRoleOnlineLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertRoleOnlineLog));
}

VOID  CLoongDB::InsertAccountOnlineTimeLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertAccountOnlineLog));
}
//-----------------------------------------------------------------------------
// 积分记录
//-----------------------------------------------------------------------------
VOID CLoongDB::FormatInsertExVolumeLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
    
	MTRANS_POINTER(p, pData, tagLogExVolume);

	pStream->SetInsert(TableName_LogExVolume, szTablePostfix);

	pStream->FillString("AccountID=") << p->dwAccountID;
	pStream->FillString(",RoleID=") << p->dwRoleID;
	pStream->FillString(",CmdID=") << p->dwCmdID;
	pStream->FillString(",Num=") << p->nExVolume;
	pStream->FillString(",TotalNum=") << p->nTotalExVolume;
	pStream->FillString(",Time=now()");
}

VOID CLoongDB::FormatInsertVIPPointLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogVIPPoint);

	pStream->SetInsert(TableName_LogVIPPoint, szTablePostfix);
	pStream->FillString("AccountID=") << p->dwAccountID;
	pStream->FillString(",RoleID=") << p->dwRoleID;
	pStream->FillString(",CmdID=") << p->dwCmdID;
	pStream->FillString(",Num=") << p->nVIPPoint;
	pStream->FillString(",TotalNum=") << p->nTotalVIPPoint;
	pStream->FillString(",Time=now()");
}

VOID CLoongDB::FormatInsertGodBidLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogGodBid);

	pStream->SetInsert(TableName_LogGodBid, szTablePostfix);
	pStream->FillString("AccountID=") << p->dwAccountID;
	pStream->FillString(",RoleID=") << p->dwRoleID;
	pStream->FillString(",ItemID=") << p->dwItemID;
	pStream->FillString(",MoneyType=") << p->byMoneyType;
	pStream->FillString(",Money=") << p->dwMoney;
	pStream->FillString(",Time=now()");
}

VOID CLoongDB::FormatInsertBattlefieldLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogBattlefield);

	pStream->SetInsert(TableName_LogBattlefield, szTablePostfix);
	pStream->FillString("GuildID=") << p->dwGuildID;
	pStream->FillString(",GuildName='").FillString(p->szGuildName).FillString("'");
	pStream->FillString(",Battlefield=") << p->byBattlefield;
	pStream->FillString(",Time=now()");
}

VOID CLoongDB::FormatInsertFairyBirthLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogFairyBirth);

	pStream->SetInsert(TableName_LogFairyBirth, szTablePostfix);
	pStream->FillString("AccountID=") << p->dwAccountID;
	pStream->FillString(",RoleID=") << p->dwRoleID;
	pStream->FillString(",YaoJingSerial=") << p->nFaBao64Serial;
	pStream->FillString(",IsNormalChange=")<< p->bNormalChange;
	pStream->FillString(",BeforeChangeBirthState=") << p->nBeforeChangeBirthState;
	pStream->FillString(",AfterChangeBirthState=") << p->nAfterChangeBirthState;
	pStream->FillString(",Time=now()");
}

VOID CLoongDB::FormatInsertRoleOnlineLog(Beton::MyStream * pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogRoleOnline);

// 	tagDWORDTime date = GetCurrentDWORDTime();
// 	date.hour = 0;
// 	date.min = 0;
// 	date.sec = 0;

	pStream->SetInsert(TableName_LogRoleOnline, szTablePostfix);
	pStream->FillString("RoleID=") << p->dwRoleID;
	pStream->FillString(",`Date`=CURDATE()");
	pStream->FillString(",OnlineTime=") << p->dwOnlineLastTime;
	pStream->FillString(",Mac='").FillString(p->szMac).FillString("'");
}

VOID CLoongDB::FormatInsertAccountOnlineLog(Beton::MyStream * pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	struct tmp : tagNetCmd
	{
		DWORD dwAccountID;
		DWORD dwOnline;
	};
	
	MTRANS_POINTER(p, pData, tmp);

	pStream->SetInsert(TableName_LogAccountOnline, szTablePostfix);
	pStream->FillString("AccountID=") << p->dwAccountID;
	pStream->FillString(",OnlineTime=") << p->dwOnline;	
}
//-----------------------------------------------------------------------------
// 定时统计记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogTimeStatTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogTimeStat, strPostfix);
}

VOID CLoongDB::InsertTimeStatLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertTimeStatLog));
}

VOID CLoongDB::FormatInsertTimeStatLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogTimeStat);

	pStream->SetInsert(TableName_LogTimeStat, szTablePostfix);

	pStream->FillString("AccountID=") << p->dwAccountID;
	pStream->FillString(",RoleID=") << p->dwRoleID;
	pStream->FillString(",BagSilver=") << p->n64BagSilver;
	pStream->FillString(",WareSilver=") << p->n64WareSilver;
	pStream->FillString(",BagYB=") << p->nBagYuanBao;
	pStream->FillString(",BaiBaoYB=") << p->nBaiBaoYuanBao;
	pStream->FillString(",ExVolume=") << p->nExVolume;
	pStream->FillString(",VIPPoint=") << p->nVIPPoint;

	pStream->FillString(",Time=now()");
}

//-----------------------------------------------------------------------------
// 物品变化记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogItemTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogItem, strPostfix);
}

VOID CLoongDB::InsertItemLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertItemLog));
}

VOID CLoongDB::FormatInsertItemLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogItem);

	pStream->SetInsert(TableName_LogItem, szTablePostfix);

	pStream->FillString("RoleID=") << p->dwRoleID;
	pStream->FillString(",CmdID=") << p->dwCmdID;
	pStream->FillString(",TypeID=") << p->dwTypeID;
	pStream->FillString(",OptNum=") << p->n16OptNum;

	pStream->FillString(",Serial1=") << p->n64Serial1;
	pStream->FillString(",ConType1=") << p->n8ConType1;
	pStream->FillString(",ResNum1=") << p->n16ResNum1;

	pStream->FillString(",Serial2=") << p->n64Serial2;
	pStream->FillString(",ConType2=") << p->n8ConType2;
	pStream->FillString(",ResNum2=") << p->n16ResNum2;

	pStream->FillString(",RoleIDRel=") << p->dwRoleIDRel;

	pStream->FillString(",Time=now()");
}

//-----------------------------------------------------------------------------
// 物品使用次数变化记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogItemTimesTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogItemTimes, strPostfix);
}

VOID CLoongDB::InsertItemTimesLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertItemTimesLog));
}

VOID CLoongDB::FormatInsertItemTimesLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogItemTims);

	pStream->SetInsert(TableName_LogItemTimes, szTablePostfix);

	pStream->FillString("RoleID=") << p->dwRoleID;
	pStream->FillString(",CmdID=") << p->dwCmdID;
	pStream->FillString(",Serial=") << p->n64Serial;
	pStream->FillString(",TypeID=") << p->dwTypeID;
	pStream->FillString(",ConType=") << p->n8ConType;
	pStream->FillString(",UsedTimes=") << p->nUsedTimes;
	pStream->FillString(",MaxUseTimes=") << p->nMaxUseTimes;

	pStream->FillString(",Time=now()");
}

//-----------------------------------------------------------------------------
// 商城出售商品记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogMallSellTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogMallSell, strPostfix);
}

VOID CLoongDB::InsertMallSellLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertMallSellLog));
	
	MTRANS_POINTER(p, pData, tagLogMallSell);
	if(P_VALID(p))
		InsertRoleCostLog(szTablePostfix, p->dwAccountID);
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertRoleItemCostLog));
}

VOID CLoongDB::FormatInsertRoleItemCostLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogMallSell);

	pStream->FillString("update ").FillString(TableName_LogRoleYuanbaoCost).FillString(szTablePostfix).FillString(" set ");
	pStream->FillString("yuanbaocost=yuanbaocost+") << p->nCostYuanBao;
	pStream->FillString(",exvolumecost=exvolumecost+") << p->nCostExVolume;
	pStream->FillString(",vippointcost=vippointcost+") << p->nCostVIPPoint;
	pStream->SetWhere();
	pStream->FillString("AccountID=") << p->dwAccountID;
}

VOID CLoongDB::FormatInsertRoleGodbidCostLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogYuanBao);

	pStream->FillString("update ").FillString(TableName_LogRoleYuanbaoCost).FillString(szTablePostfix).FillString(" set ");
	pStream->FillString("yuanbaocost=yuanbaocost+") << -(p->nYuanBao);
	pStream->SetWhere();
	pStream->FillString("AccountID=") << p->dwAccountID;
}

VOID CLoongDB::FormatInsertMallSellLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogMallSell);

	pStream->SetInsert(TableName_LogMallSell, szTablePostfix);

	pStream->FillString("CmdID=") << p->dwCmdID;
	pStream->FillString(",Serial=") << p->n64Serial;
	pStream->FillString(",TypeID=") << p->dwTypeID;
	pStream->FillString(",RoleID=") << p->dwRoleID;
	pStream->FillString(",Num=") << p->n16NumSell;
	pStream->FillString(",ExistTime=") << p->dwExistTime;
	pStream->FillString(",MaxUseTimes=") << p->nMaxUseTimes;
	pStream->FillString(",CostYB=") << p->nCostYuanBao;
	pStream->FillString(",CostExVolume=") << p->nCostExVolume;
	pStream->FillString(",CostVIPPoint=") << p->nCostVIPPoint;


	DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwFstGainTime);
	pStream->FillString(",FstGainTime='").FillString(m_szTime);
	pStream->FillString("'");

	pStream->FillString(",Time=now()");
}

//-----------------------------------------------------------------------------
// 商城出售礼品包记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogMallSellPackTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogMallSellPack, strPostfix);
}

VOID CLoongDB::InsertMallSellPackLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertMallSellPackLog));
// 	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertRoleCostLog));
// 	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertRolePackCostLog));
}

VOID CLoongDB::InsertRoleCostLog(LPCSTR szTablePostfix, DWORD dwAccountID)
{
	if (!P_VALID(dwAccountID))
	{
		return;
	}

	Beton::MyStream* pStream = m_pDBLog->GetStream();
	if (!P_VALID(pStream))
	{
		return;
	}

	pStream->Clear();
	pStream->FillString("select * from ").FillString(TableName_LogRoleYuanbaoCost).FillString(szTablePostfix);
	pStream->SetWhere();
	pStream->FillString("AccountID=")<<dwAccountID;

	//Beton::Connection* con = m_pDBLog->GetFreeConnection();
	Beton::QueryResult* pRes = m_pDBLog->Query(pStream);
	if (!P_VALID(pRes) || pRes->GetRowCount() > 0)
	{
		return;
	}

	pStream->Clear();
	pStream->SetInsert(TableName_LogRoleYuanbaoCost, szTablePostfix);
	pStream->FillString("AccountID=") << dwAccountID;
	m_pDBLog->Execute(pStream);

	m_pDBLog->ReturnStream(pStream);
	m_pDBLog->FreeQueryResult(pRes);
}

// VOID CLoongDB::FormatInsertRolePackCostLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
// {
// 	MTRANS_POINTER(p, pData, tagLogMallSellPack);
// 
// 	pStream->FillString("update ").FillString(TableName_LogRoleYuanbaoCost).FillString(szTablePostfix).FillString(" set ");
// 	pStream->FillString("yuanbaocost=yuanbaocost+") << p->nCostYuanBao;
// 	pStream->SetWhere();
// 	pStream->FillString("AccountID=") << p->dwAccountID;
// }

VOID CLoongDB::FormatInsertMallSellPackLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogMallSellPack);

	pStream->SetInsert(TableName_LogMallSellPack, szTablePostfix);

	pStream->FillString("PackID=") << p->dwPackID;
	pStream->FillString(",BuyID=") << p->dwBuyRoleID;
	pStream->FillString(",ToID=") << p->dwToRoleID;
	pStream->FillString(",CostYB=") << p->nCostYuanBao;

	pStream->FillString(",Time=now()");
}

//-----------------------------------------------------------------------------
// 帮派资金记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogFundTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogFund, strPostfix);
}

VOID CLoongDB::InsertFundLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertFundLog));
}

VOID CLoongDB::FormatInsertFundLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogFund);

	pStream->SetInsert(TableName_LogFund, szTablePostfix);

	pStream->FillString("guild_id=") << p->dwGuildID;
	pStream->FillString(",account_id=") << p->dwAccountID;
	pStream->FillString(",role_id=") << p->dwRoleID;
	pStream->FillString(",cmd_id=") << p->dwCmdID;
	pStream->FillString(",fund=") << p->nFund;
	pStream->FillString(",total_fund=") << p->nTotalFund;
	pStream->FillString(",time=now()");
}

//-----------------------------------------------------------------------------
// 帮派资材记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogMaterialTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogMaterial, strPostfix);
}

VOID CLoongDB::InsertMaterialLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertMaterialLog));
}

VOID CLoongDB::FormatInsertMaterialLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogMaterial);

	pStream->SetInsert(TableName_LogMaterial, szTablePostfix);

	pStream->FillString("guild_id=") << p->dwGuildID;
	pStream->FillString(",account_id=") << p->dwAccountID;
	pStream->FillString(",role_id=") << p->dwRoleID;
	pStream->FillString(",cmd_id=") << p->dwCmdID;
	pStream->FillString(",material=") << p->nMaterial;
	pStream->FillString(",total_material=") << p->nTotalMaterial;
	pStream->FillString(",time=now()");
}

//-----------------------------------------------------------------------------
// 帮派声望记录
//-----------------------------------------------------------------------------
BOOL CLoongDB::CreateLogReputationTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogReputation, strPostfix);
}

VOID CLoongDB::InsertReputationLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertReputationLog));
}

VOID CLoongDB::FormatInsertReputationLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogReputation);

	pStream->SetInsert(TableName_LogReputation, szTablePostfix);

	pStream->FillString("guild_id=") << p->dwGuildID;
	pStream->FillString(",account_id=") << p->dwAccountID;
	pStream->FillString(",role_id=") << p->dwRoleID;
	pStream->FillString(",cmd_id=") << p->dwCmdID;
	pStream->FillString(",reputation=") << p->nReputation;
	pStream->FillString(",total_reputation=") << p->nTotalReputation;
	pStream->FillString(",time=now()");
}

// GM命令log相关
BOOL CLoongDB::CreateLogGMCmd(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogGMCmd, strPostfix);
}

VOID CLoongDB::InsertGMCmdLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertGMCmdLog));
}

VOID CLoongDB::FormatInsertGMCmdLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogGMCmd);

	pStream->SetInsert(TableName_LogGMCmd, szTablePostfix);

	pStream->FillString("role_id=") << p->dwRoleID;
	pStream->FillString(",gm_cmd='").FillString(p->szGMCmd).FillString("'");
	pStream->FillString(",error_code=") << p->dwErrorCode;
	pStream->FillString(",time=now()");
}

// 宠物log相关
BOOL CLoongDB::CreateLogPet(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogPet, strPostfix);
}

VOID CLoongDB::InsertPetLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertPetLog));
}

VOID CLoongDB::FormatInsertPetLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogPet);

	pStream->SetInsert(TableName_LogPet, szTablePostfix);

	pStream->FillString("master_id=") << p->dwMasterID;
	pStream->FillString(",pet_id=") << p->dwPetID;
	pStream->FillString(",cmd_id=") << p->dwCmdID;
}

BOOL CLoongDB::CreateLogRoleOnline(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogRoleOnline, strPostfix);
}

BOOL CLoongDB::CreateLogAccountOnline(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogAccountOnline, strPostfix);
}

BOOL CLoongDB::CreateLogRoleYuanbaoCost(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogRoleYuanbaoCost, strPostfix);
}

VOID CLoongDB::FormatUpdateWorldDataLog(Beton::MyStream * pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, DWORD);

	pStream->SetUpdate(TableName_LogWorldData);

	for (int i = 0 ; i<DataCollection_Max_Data_Num; ++i)
	{
		TCHAR szTmp[X_SHORT_STRING] = {0};
		_sntprintf(szTmp, X_SHORT_STRING, _T("data%d="), i+1);
		pStream->FillString(szTmp) << p[i];
		if(i<DataCollection_Max_Data_Num-1)
			pStream->FillString(",");
	}

	pStream->SetWhere();

	tagDWORDTime date = GetCurrentDWORDTime();
	DWORD dwDate = date.year*10000 + date.month*100 + date.day;
	pStream->FillString("`Date`=") << dwDate;
}

VOID CLoongDB::FormatLoadWorldData(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
	pStream->SetSelect(TableName_LogWorldData, "*");
	pStream->SetWhere();

	tagDWORDTime date = GetCurrentDWORDTime();
	DWORD dwDate = date.year*10000 + date.month*100 + date.day;
	pStream->FillString("`Date`=") << dwDate;
}

VOID CLoongDB::FormatInsertRoleDataLog(Beton::MyStream* pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, DWORD);

	pStream->SetUpdate(TableName_LogRoleData);

	for (int i = 0 ; i<DataCollection_Max_RoleData_Num; ++i)
	{
		TCHAR szTmp[X_SHORT_STRING] = {0};
		_sntprintf(szTmp, X_SHORT_STRING, _T("data%d="), i+1);
		pStream->FillString(szTmp) << p[i];
		if(i<DataCollection_Max_RoleData_Num-1)
			pStream->FillString(",");
	}

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

VOID CLoongDB::FormatLoadRoleLogData(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect(TableName_LogRoleData, "*");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

VOID CLoongDB::FormatInsertNewRoleLogData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->FillString("insert into ");
	pStream->FillString(TableName_LogRoleData);
	pStream->FillString(" (`RoleID`) values(") << dwRoleID;
	pStream->FillString(")");
}

VOID CLoongDB::FormatInsertWorldDataLog(Beton::MyStream *pStream, DWORD dwRoleID, DWORD dwDummy)
{
	pStream->Clear();
	pStream->SetSelect(TableName_LogWorldData, "*");
	pStream->SetWhere();
	pStream->FillString("`Date`=(select max(`Date`) from ");
	pStream->FillString(TableName_LogWorldData);
	pStream->FillString(")");
}

VOID CLoongDB::ProcResInsertWorldDataLog(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	*pNum = pRes->GetRowCount();

	if (1 != *pNum)
	{
		return;
	}

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if (!P_VALID(pStream))
	{
		return;
	}

	pStream->Clear();
	pStream->SetInsert(TableName_LogWorldData);
	
	for (int i = 0 ; i<DataCollection_Max_Data_Num; ++i)
	{
		TCHAR szTmp[X_SHORT_STRING] = {0};
		_sntprintf(szTmp, X_SHORT_STRING, _T("data%d="), i+1);
		pStream->FillString(szTmp) << (*pRes)[i+1].GetDword();
		pStream->FillString(",");
	}

	tagDWORDTime date = GetCurrentDWORDTime();
	DWORD dwdate = date.year*10000 + date.month*100 + date.day;
	pStream->FillString("`Date`=")<<dwdate;
	m_pDBLoong->Execute(pStream);
	m_pDBLoong->ReturnStream(pStream);
}

VOID CLoongRank::RefreshWorldLogData()
{
	return;
	Beton::MyStream* pStream = m_pDBLoongRank->GetStream();
	if (!P_VALID(pStream))
	{
		return;
	}

	pStream->Clear();
	pStream->SetSelect(TableName_LogWorldData, "*");
	pStream->SetWhere();
	pStream->FillString("`Date`=(select max(`Date`) from ");
	pStream->FillString(TableName_LogWorldData);
	pStream->FillString(")");

	Beton::QueryResult* pResult = m_pDBLoongRank->Query(pStream);
	if(!P_VALID(pResult))
	{
		m_pDBLoongRank->ReturnStream(pStream);
		return;
	}

	// 检验并保存查询结果
	if (1==pResult->GetRowCount())
	{
		pStream->Clear();
		pStream->SetInsert(TableName_LogWorldData);

		for (int i = 0 ; i<DataCollection_Max_Data_Num; ++i)
		{
			TCHAR szTmp[X_SHORT_STRING] = {0};
			_sntprintf(szTmp, X_SHORT_STRING, _T("data%d="), i+1);
			pStream->FillString(szTmp) << (*pResult)[i+1].GetDword();
			pStream->FillString(",");
		}

		tagDWORDTime date = GetCurrentDWORDTime();
		DWORD dwdate = date.year*10000 + date.month*100 + date.day;
		pStream->FillString("`Date`=")<<dwdate;
		m_pDBLoongRank->Execute(pStream);
	}

	m_pDBLoongRank->ReturnStream(pStream);
	m_pDBLoongRank->FreeQueryResult(pResult);
}

DWORD CLoongDB::SaveTigerProfit(INT64 dwProfit)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetUpdate("gm_server_data");
	pStream->FillString("data_value=")<<dwProfit;
	pStream->SetWhere();
	pStream->FillString("data_name='TigerProfit'");

	BOOL bRet = m_pDBLoong->Execute(pStream);
	m_pDBLoong->ReturnStream(pStream);

	return bRet;	
}

BOOL CLoongDB::CreateLogHolyEquipConsolidateTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogHolyEquipConsolidate, strPostfix);
}

BOOL CLoongDB::CreateLogKillRoleTable(LPCSTR strPostfix)
{
	return CreateLogTable(TableName_LogKillRole, strPostfix);
}


VOID CLoongDB::InsertHolyEquipConLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertHolyEquipConLog));
}

VOID CLoongDB::InsertKillRoleLog(LPCSTR szTablePostfix, LPVOID pData)
{
	UpdateLog(szTablePostfix, pData, (&CLoongDB::FormatInsertKillRoleLog));
}

VOID CLoongDB::FormatInsertHolyEquipConLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagLogHolyEquipConsolidate);

	pStream->SetInsert(TableName_LogHolyEquipConsolidate,szTablePostfix);
	pStream->FillString("ConsolidateType = ")<<p->eType;
	pStream->FillString(",HolyEquipID = ")<<p->n64HolyEquipID;
	pStream->FillString(",bCostMoney = ")<<p->bCostMoney;
	pStream->FillString(",CostMoney = ")<<p->nCostMoney;
	pStream->FillString(",bCostItem = ")<<p->bCostItem;
	pStream->FillString(",CostItem = ")<<p->nCostItem;
	pStream->FillString(",bCostItemOther = ")<<p->bCostItemOther;
	pStream->FillString(",CostItemOther = ")<<p->nCostItemOther;
	pStream->FillString(",bConsolidateRes = ")<<p->bConsolidateRes;
	pStream->FillString(",CurrentLevel = ")<<p->byCurrentLevel;
	pStream->FillString(",RoleID = ")<<p->dwRoleID;
	pStream->FillString(",Time = now()");
}

VOID CLoongDB::FormatInsertKillRoleLog(Beton::MyStream *pStream, LPCSTR szTablePostfix, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_LogKillrole);

	pStream->SetInsert(TableName_LogKillRole,szTablePostfix);
	pStream->FillString("SrcRoleID = ")<<p->dwSrcRoleID;
	pStream->FillString(",TargetRoleID = ")<<p->dwTargetRoleID;
	pStream->FillString(",Time = now()");
}