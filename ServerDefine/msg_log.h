//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_log.h
// author: 
// actor:
// data: 2009-03-23
// last:
// brief: log相关
//-----------------------------------------------------------------------------
#pragma once
#include "../World/data_collect.h"

#pragma pack(push, 1)

#include "log_cmdid_define.h"
//-----------------------------------------------------------------------------

CMD_START(NDBC_LogSilver)		// 金钱变化
	tagLogSilver	sLogSilver;
CMD_END

CMD_START(NDBC_LogYuanBao)		// 元宝变化
	tagLogYuanBao	sLogYuanBao;
CMD_END

CMD_START(NDBC_LogExVolume)		// 赠点变化
	tagLogExVolume	sLogExVolume;
CMD_END

CMD_START(NDBC_LogVIPPoint)		// 积分变化
    tagLogVIPPoint	sLogVIPPoint;
CMD_END

CMD_START(NDBC_LogTimeStat)		// 定时统计
	tagLogTimeStat	sLogTimeStat;
CMD_END

CMD_START(NDBC_LogItem)			// 物品log
	tagLogItem		sLogItem;
CMD_END

CMD_START(NDBC_LogItemTimes)	// 物品log
	tagLogItemTims	sLogItemTimes;
CMD_END

CMD_START(NDBC_LogMallSell)		// 商城出售物品log
	tagLogMallSell	sLogMallSell;
CMD_END

CMD_START(NDBC_LogMallSellPack)		// 商城出售礼品包log
	tagLogMallSellPack	sLogMallSellPack;
CMD_END

CMD_START(NDBC_LogGodBid)
	tagLogGodBid	 sLogGodBid;
CMD_END

CMD_START(NDBC_LogBattlefield)
	tagLogBattlefield	 sLogBattlefield;
CMD_END

//-----------------------------------------------------------------------------
// 帮派LOG记录
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogFund)
tagLogFund			sLogFund;
CMD_END

CMD_START(NDBC_LogMaterial)
tagLogMaterial		sLogMaterial;
CMD_END

CMD_START(NDBC_LogReputation)
tagLogReputation	sLogReputation;
CMD_END

//-----------------------------------------------------------------------------
// 帮派LOG记录
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogGMCmd)
	tagLogGMCmd		sLogGMCmd;
CMD_END

//-----------------------------------------------------------------------------
// 宠物LOG记录
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogPet)
	tagLogPet	sLogPet;
CMD_END

//-----------------------------------------------------------------------------
// 妖精生育LOG记录
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogFairyBirth)
	tagLogFairyBirth	sLogFairyBirth;
CMD_END

//-----------------------------------------------------------------------------
// 角色登陆LOG记录
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogRoleOnlineInfo)
	tagLogRoleOnline	sLogRoleOnline;
CMD_END

//-----------------------------------------------------------------------------
// 服务器数据LOG记录
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogWorldData)
	DWORD dwData[DataCollection_Max_Data_Num];
CMD_END

CMD_START(NDBC_RequestLogWorldData)
CMD_END

CMD_START(NDBS_RequestLogWorldData)
	DWORD dwData[DataCollection_Max_Data_Num];
CMD_END

//-----------------------------------------------------------------------------
// 角色数据LOG记录
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogRoleData)
	DWORD dwRoleID;
	DWORD dwData[DataCollection_Max_RoleData_Num];
CMD_END

CMD_START(NDBC_RequestLogRoleData)
	DWORD dwRoleID;
CMD_END

CMD_START(NDBS_RequestLogRoleData)
	DWORD dwRoleID;
	DWORD dwData[DataCollection_Max_RoleData_Num];
CMD_END

//-----------------------------------------------------------------------------
// 账号在线时长LOG记录
//-----------------------------------------------------------------------------
CMD_START(NDBC_LogAccountOnlineTime)
	DWORD dwAccoutID;
	DWORD dwOnlineSecond;
CMD_END

//元神装备强化log
CMD_START(NDBC_LogHolyEquipCon)
tagLogHolyEquipConsolidate sLogHolyEquip;
CMD_END

// 角色PK杀人记Log
CMD_START(NDBC_LogKillrole)
	DWORD dwSrcRoleID;		// 杀人角色ID
	DWORD dwTargetRoleID;	// 被杀角色ID
CMD_END
//-----------------------------------------------------------------------------
#pragma pack(pop)