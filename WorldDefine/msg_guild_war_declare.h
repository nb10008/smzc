//-----------------------------------------------------------------------------
// date:  2010-7-1
// brief: 客户端和服务器端间消息 -- 公会战相关
//-----------------------------------------------------------------------------
#pragma once

#include "guild_war_declare_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// 宣战
//-----------------------------------------------------------------------------
CMD_START(NC_DeclareGuildWar)
	DWORD	dwAttackGuildID;			// 宣战公会ID
	DWORD	dwDefenceGuildID;			// 被宣战公会ID
CMD_END

CMD_START(NS_DeclareGuildWar)
	DWORD	dwErrorCode;
	BYTE	byTodayDeclareWarTimes;		// 宣战公会当天宣战次数
	DWORD	dwAttackGuildID;			// 宣战公会ID
	DWORD	dwDefenceGuildID;			// 被宣战公会ID
CMD_END

//-----------------------------------------------------------------------------
// 战争结束
//-----------------------------------------------------------------------------
CMD_START(NS_GuildWarEnd)
	DWORD	dwAttackGuildID;			// 宣战公会ID
	DWORD	dwDefenceGuildID;			// 被宣战公会ID
CMD_END

//-----------------------------------------------------------------------------
// 角色上线时同步与角色所属帮派处于战争状态的帮派
//-----------------------------------------------------------------------------
CMD_START(NS_SyncWarStateGuild)
	INT		nRemainWarDeclareTimes;		// 剩余宣战次数
	INT		nGuildCount;				// 与角色所属公会处于战争状态的公会个数
	DWORD	dwGuildID[1];				// 与角色所属公会处于战争状态的公会ID数组
CMD_END

//-----------------------------------------------------------------------------
// 每天零点恢复剩余宣战次数
//-----------------------------------------------------------------------------
CMD_START(NS_ResetRemainDeclareTimes)
CMD_END

#pragma pack(pop)
