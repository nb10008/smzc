#pragma once
#include "AutoFight_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------
// 错误码
//-----------------------------------------------------------------------
enum EAutoFightError
{
	E_AutoFight_UseZhanDouFu_Success = 0,	// 使用成功
	E_AutoFight_No_ZhanDouFu,				// 没有战斗符道具
	E_AutoFight_ZhanDouFu_TimeOut,			// 战斗符到时间了
};

//-----------------------------------------------------------------------
// 使用战斗符
//-----------------------------------------------------------------------
CMD_START(NC_UseZhanDouFu)
	INT64   n64ItemID;						// 物品的64位ID
CMD_END

CMD_START(NS_UseZhanDouFu)
	DWORD				dwErrorCode;
	INT64					n64ItemID;						// 物品的64位ID
	tagDWORDTime	dw1stGainTime;				// 第一个玩家获得时间
	DWORD				dwLeftTicks;					// 此战斗符剩余tick数，为0表示不要开始计时
CMD_END

//-----------------------------------------------------------------------
// 内挂状态
//-----------------------------------------------------------------------
CMD_START(NC_SyncAutoFightState)
	INT64			n64ItemID;						// 物品的64位ID
	EAutoFightState eAutoFightState;
CMD_END

CMD_START(NS_SyncAutoFightState)
	EAutoFightState eAutoFightState;
	INT64			n64ItemID;
CMD_END

//-----------------------------------------------------------------------
// 服务器通知客户端战斗符道具时限到了
//-----------------------------------------------------------------------
CMD_START(NS_ZhanDouFuTimeOut)
	INT64			n64ItemID;
CMD_END

#pragma pack(pop)