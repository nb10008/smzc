//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_vip_netbar.h
// author: xlguo
// actor:
// data: 2009-8-3
// last:
// brief: 金牌网吧服务器消息
//-------------------------------------------------------------------------------
#pragma once
#include "vip_netbar_define.h"
#pragma pack(push,1)

#define VNB_Load_Msg_Max_Data_Once 100
enum EVNBMsgType
{
	EVMT_FirstOne = 0,
	EVMT_Middle = 1,
	EVMT_LastOne = 2,
};

//-------------------------------------------------------------------------
// 获取账号历史数据
//-------------------------------------------------------------------------
CMD_START(NDBC_GetVNBData)
	DWORD dwDate;
CMD_END

CMD_START(NDBS_GetVNBData)
	tagDBVNBPlayers players;
	EVNBMsgType eType;
CMD_END

//-------------------------------------------------------------------------
// 更新金牌网吧玩家
//-------------------------------------------------------------------------
CMD_START(NDBC_UpdateVNBPlayer)
	DWORD	dwAccountID;
	DWORD	dwLoginTime;
CMD_END


//-------------------------------------------------------------------------
// 台湾金牌网吧 加红利点
//-------------------------------------------------------------------------
CMD_START(NDBC_Orange_AddBonusPoint)
	TCHAR szRoleName[X_SHORT_NAME];
	CHAR szAccountName[X_SHORT_NAME];
	DWORD dwIP;
	INT   nBonusPoint;
CMD_END


#pragma pack(pop)