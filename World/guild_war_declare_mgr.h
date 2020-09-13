//-----------------------------------------------------------------------------
// date:  2010-7-1
// brief: 公会战宣战管理器
//-----------------------------------------------------------------------------

#pragma once

#include "../ServerDefine/guild_war_declare_define.h"
#include "guild_mgr.h"
#include "guild.h"

// 公会战宣战类
class CGuildWarDeclare
{
public:
	CGuildWarDeclare( DWORD 		dwAttackGuildID,			// 宣战工会ID
					  DWORD 		dwDefenceGuildID,			// 被宣战工会ID
					  BOOL			bWarEnded,					// 宣战是不是已经结束过了
					  tagDWORDTime	dwWarDeclareTime);			// 宣战时间
					
	~CGuildWarDeclare() {}

	// 给两个公会所有在线角色增加公会战buff
	VOID AddBuffToAllOnlineRole();

	// 删除两个公会所有在线角色增加公会战buff
	VOID DeleteBuffOnAllOnlineRole( INT nDelType = 0 );

	// 将宣战关系保存到数据库
	VOID SaveGuildWarDeclare2DB();

	DWORD GetAttackGuildID() { return m_dwAttackGuildID; }
	DWORD GetDefenceGuildID() { return m_dwDefenceGuildID; }
	tagDWORDTime GetWarDeclareTime() { return m_dwWarDeclareTime; }

	// 检查结束战争时间是否到了
	BOOL IsGuildWarEnd();

	// 公会战争结束了
	VOID GuildWarEnd();

	// 检查公会战是不是已经结束过了
	BOOL IsGuildWarEnded()	{ return m_bWarEnded; }

private:

	DWORD			m_dwAttackGuildID;			// 宣战工会ID
	DWORD			m_dwDefenceGuildID;			// 被宣战工会ID
	tagDWORDTime	m_dwWarDeclareTime;			// 宣战时间

	BOOL			m_bWarEnded;				// 是不是已经结束过了
};

// 公会战宣战管理器类
class CGuildWarDeclareMgr
{
private:
	typedef multimap<DWORD, CGuildWarDeclare*>::iterator MultiMapIterator;

public:
	CGuildWarDeclareMgr();
	~CGuildWarDeclareMgr();

	VOID Init();

	VOID Update();

	// 将宣战关系添加到map里面
	VOID AddGuildWarDeclare2Map(DWORD dwAttackGuildID,			// 宣战工会ID
		DWORD dwDefenceGuildID);		// 被宣战工会ID

	// 初始化公会宣战关系map
	VOID InitGuildWarDeclareMap(DWORD dwGuildWarDeclareCnt, tagGuildWarDeclare* pGuildWarDeclare);

	// 清除数据库中过期的公会宣战关系
	VOID ClearExpireGuildDeclareInDB();

	// 同步与角色所属公会处于战争状态的帮派
	VOID SyncWarStateGuild(Role* pRole);

	// 取指定公会当天对外宣战次数
	BYTE GetTodayDeclareWarTimes(DWORD dwGuildID);

	// 取指定公会对同一个指定公会当天的宣战次数
	BYTE GetTodaySameGuildBeWarTimes( DWORD dwAttackGuildID,			// 宣战帮派ID
									  DWORD dwDefenceGuildID);			// 被宣战帮派ID

	// 当有公会解散时触发
	VOID OnGuildDismiss(DWORD dwGuildID);

	// 当有角色退出公会时候触发
	VOID OnRoleLeaveGuild(Role* pRole);

	// 当有角色加入公会时候触发
	VOID OnRoleJoinGuild(Role* pRole);

	// 两个公会是否有宣战关系
	BOOL IsInGuildWarState(Role* pRole1, Role* pRole2);
	BOOL IsInGuildWarState(DWORD dwGuildID1, DWORD dwGuildID2);

	// 当角色下线时触发
	VOID OnRoleLogout(Role* pRole);

	//获取该公会在公会战中的数量，可以是攻击方也可是防御方
	INT GetGuildWarNum( DWORD dwGuildID );

private:

	// 载入数据库中的公会宣战关系
	VOID LoadAllGuildWarDeclareFromDB();

	// 释放公会宣战关系map
	VOID DestoryGuildWarDeclareMap();

	// 将宣战关系添加到map里面
	VOID AddGuildWarDeclare2Map(DWORD dwAttackGuildID,			// 宣战工会ID
		DWORD			dwDefenceGuildID,						// 被宣战工会ID
		tagDWORDTime	dwWarDeclareTime,						// 宣战时间
		BOOL			bWarEnded = FALSE,						// 宣战是不是已经结束过了
		BOOL			bNeedSave2DB = TRUE);					// 是否要存数据库

	template<typename DeclareOperation>
	VOID ForEachDeclareInMap(DeclareOperation oper);

private:

	// 保存所有宣战
	std::multimap<DWORD, CGuildWarDeclare*> m_mapGuildWarDeclareList;

};

template<typename DeclareOperation>
VOID CGuildWarDeclareMgr::ForEachDeclareInMap(DeclareOperation oper)
{
	TMap<DWORD, CGuildWarDeclare*>::TMapIterator iterator = m_mapGuildWarDeclareList.Begin();
	CGuildWarDeclare* pGuildWarDeclare = NULL;
	while (m_mapGuildWarDeclareList.PeekNext(iterator,pGuildWarDeclare))
	{
		if (P_VALID(pGuildWarDeclare))
		{
			oper(pGuildWarDeclare);
		}
	}
}

extern CGuildWarDeclareMgr g_guildWarDeclareMgr;