//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_gm_cmd.h
// author: 
// actor:
// data: 
// last:
// brief: GM命令相关消息
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)
//----------------------------------------------------------------------------
// 错误码枚举
//----------------------------------------------------------------------------
enum
{
	E_GMCmd_Low_Privilege,	// 权限不够

};

enum EDDPType{
	EDDP_Exp		= 0,				//双倍经验
	EDDP_Item		= 1,				//双倍物品
};

#define MAX_GM_COMMAND_LEN 128 
//----------------------------------------------------------------------------
// 消息结构
//----------------------------------------------------------------------------
CMD_START(NC_GMCommand)		// 通用GM命令
	TCHAR	szCommand[MAX_GM_COMMAND_LEN];
CMD_END;

//----------------------------------------------------------------------------
// 游戏世界发给客户端，控制某个任务为已完成，或者未完成
//----------------------------------------------------------------------------
CMD_START(NS_GMQuestState)		// 通用GM命令
	UINT16		u16QuestID;		// 任务ID
	BOOL		bDone;			// 任务状态 TRUE：已完成，FALSE：未完成
CMD_END;


CMD_START( NS_DoubleDropProbabilityChange )	// 双倍掉率变化通知
	EDDPType	eType;			// 双倍类型
	float			nProbability;	// 几倍率
	BOOL		bOpen;			// 是否开启
	DWORD		dwTimeLeft;		// 剩余时间
CMD_END
//----------------------------------------------------------------------------
CMD_START(NC_GetServiceInfo)		// 获取服务器信息指令
CMD_END;

CMD_START(NS_GetServiceInfo)		// 获取服务器信息指令
	CHAR szIP[64];
	CHAR szMac[64];
	CHAR szMd5[48];
	CHAR szSign[32];
CMD_END;


#pragma pack(pop)