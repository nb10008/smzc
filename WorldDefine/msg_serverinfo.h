//-----------------------------------------------------------------------------
//!\file msg_spirit.h
//!
//!\date 2010-01-15
//!
//!\brief 服务器信息
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

#include <bitset>
using std::bitset;

CMD_START(NS_ServerInfo)				// 服务器信息
	TCHAR	szServer[256];
	DWORD	dwLoginMode;
	BOOL	bEnableExpressPay;
	TCHAR	szExpressPayUrl[256];	
	BOOL	bEnableSpiritSys;
	TCHAR	szTodayHotActivityUrl[256];
	BOOL	bEnableCitySys;
	DWORD	dwAutoFightTimeInterval;	// 战斗挂机战斗施放技能和使用物品时间间隔
	BOOL bGuildWarOpen; // 公会战开关
	DWORD	dwServerFunctionSwitch;		//各种功能开关
CMD_END

enum EServerFunctionSwitch
{
	ESFS_Null = -1,
	ESFS_MallCart,

	ESFS_End,
};
typedef bitset<ESFS_End>	BitSetServerFunctionSwitch;

#pragma pack(pop)