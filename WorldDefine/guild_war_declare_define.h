//-----------------------------------------------------------------------------
// date:  2010-7-1
// brief: 公会战相关定义
//-----------------------------------------------------------------------------
#pragma once

// 公会每天宣战次数上限
const INT32 MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY = 5;

// 每个公会对同一个公会每天宣战次数上限
const INT32 MAX_SAME_GUILD_BE_DECLARE_WAR_TIMES	= 2;

// 公会战状态buff的ID
const INT32 GUILD_WAR_BUFF_ID = 9000001;
const INT32 GUILD_WAR_BUFF_TYPE_ID = GUILD_WAR_BUFF_ID/100;

// 公会战宣战道具(暂定宣战令牌)
const INT32 DECLEAR_WAR_ITEM_TYPE_ID = 4500003;

// 公会战宣战扣除金钱数(1w金)
const INT32 DECLEAR_WAR_SILVER_COST = 10000;

// 错误码
enum
{
	E_GuildDeclareWar_Success				= 1,		// 宣战成功

	E_GuildDeclareWar_Gold_Not_Enough		= 2,		// 背包金钱不足
	E_GuildDeclareWar_No_Spec_Item			= 3,		// 角色身上没有宣战道具
	E_GuildDeclareWar_Declare_Times_Full	= 4,		// 宣战次数已达上限
	E_GuildDeclareWar_Not_Guild_Leader		= 5,        // 不是公会会长
	E_GuildDeclareWar_Same_Guild_Full		= 6,		// 对同一个工会每天宣战次数达到上限
	E_GuildDeclareWar_Guild_Not_Exist		= 7,		// 被宣战公会不存在了
	E_GuildDeclareWar_Declare_Exist			= 8,		// 已经宣战过了
};
