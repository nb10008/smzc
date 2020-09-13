//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_miracle.h
// author: songg
// actor:
// data: 2010-01-05
// last:
// brief: 仙界修行
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/miracle_define.h"
#include "../WorldDefine/msg_common_errorcode.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// 获取玩家当前修行状态
//----------------------------------------------------------------------------

//练习仙界技能
CMD_START(NC_PracticeMiracleSkill)
	DWORD dwSkillID;			// 大ID
CMD_END

//返回错误码
CMD_START(NS_PracticeMiracleSkill)
	DWORD	dwErrCode;			// 错误码
	INT16	n16CurPrcNum;		// 当前人物修炼点数
CMD_END



//0点 重置在线玩家修炼点数
CMD_START(NS_ResetMiraclePrcNum)

CMD_END



//使用仙界修行符增加修炼点儿
CMD_START(NC_UseAddMiraclePrcNumItem)
	INT64		n64ItemID;			//物品64位ID
CMD_END

//返回
CMD_START(NS_UseAddMiraclePrcNumItem)
	DWORD	dwErrCode;			// 错误码	
	INT16	n16CurPrcNum;		// 当前人物修炼点数
CMD_END

////飞升技能
//CMD_START(NC_PracticeSoarSkill)
//	DWORD dwSkillID;			// 大ID
//CMD_END
//
////返回错误码
//CMD_START(NS_PracticeSoarSkill)
//	DWORD	dwErrCode;			// 错误码
//	INT16	n16CurPrcNum;		// 当前人物修炼点数
//CMD_END

//0点 重置在线玩家飞升技能可修炼次数
CMD_START(NS_ResetSoarSkillLearnTimes)
	INT16    nCurrLeftNum;		// 当前还可以修炼的次数
CMD_END

#pragma pack(pop)
