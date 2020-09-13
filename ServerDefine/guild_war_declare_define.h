//-----------------------------------------------------------------------------
// date:  2010-7-1
// brief: 公会战宣战关系相关定义
//-----------------------------------------------------------------------------
#pragma once

struct tagGuildWarDeclare
{
	DWORD			dwAttackGuildID;			// 宣战工会ID
	DWORD			dwDefenceGuildID;			// 被宣战工会ID
	tagDWORDTime	dwWarDeclareTime;			// 宣战时间
	BOOL			bWarEnded;					// 是不是已经结束过了
};