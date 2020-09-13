//-----------------------------------------------------------------------------
// date:  2010-7-1
// brief: 公会战宣战关系world和db相关消息定义
//-----------------------------------------------------------------------------
#pragma once

#include "guild_war_declare_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// 清除数据库中过期的宣战关系
//-----------------------------------------------------------------------------
CMD_START(NDBC_ClearExpireGuildWarDeclare)

CMD_END

//-----------------------------------------------------------------------------
// 将新增宣战关系保存到数据库
//-----------------------------------------------------------------------------
CMD_START(NDBC_InsertGuildWarDeclare)
	DWORD			dwAttackGuildID;			// 宣战公会ID
	DWORD			dwDefenceGuildID;			// 被宣战公会ID
	tagDWORDTime	dwWarDeclareTime;			// 宣战时间
	BOOL			bWarEnded;					// 是不是已经结束过了
CMD_END

//-----------------------------------------------------------------------------
// 修改数据库中的宣战关系的状态为宣战过
//-----------------------------------------------------------------------------
CMD_START(NDBC_SetGuildWarDeclareEnded)
	DWORD			dwAttackGuildID;			// 宣战公会ID
	DWORD			dwDefenceGuildID;			// 被宣战公会ID
	tagDWORDTime	dwWarDeclareTime;			// 宣战时间
CMD_END

//-----------------------------------------------------------------------------
// 载入数据库中的公会宣战关系
//-----------------------------------------------------------------------------
CMD_START(NDBC_LoadGuildWarDeclare)

CMD_END

CMD_START(NDBS_LoadGuildWarDeclare)
	DWORD					dwGuildWarDeclareCnt;		// 公会战争宣战关系个数
	tagGuildWarDeclare		pGuildWarDeclare[1];		// 所有存在数据库中的公会宣战关系
CMD_END

#pragma pack(pop)
