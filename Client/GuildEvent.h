#pragma once
#include "..\WorldDefine\guild_define.h"
#include "..\WorldDefine\msg_activity.h"
#include "..\WorldDefine\msg_GuildBattle.h"
#include "GameClientDefine.h"
#include "GuildDefine.h"

class Item;

// 创建帮派最低要求
const INT CREATE_NEED_LEVEL		= 20;					// 创建帮派角色最低等级
const INT CREATE_NEED_GOLD		= 10;					// 创建帮派所需资金(单位：金)

struct tagGuildPosAppointEvent : public tagGameEvent
{
	DWORD			dwRoleID;
	EGuildMemberPos ePos;

	tagGuildPosAppointEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{dwRoleID = GT_INVALID;ePos = EGMP_End;}
};

struct tagGuildInfoReflashEvent : public tagGameEvent
{
	DWORD dwRoleID;

	tagGuildInfoReflashEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{dwRoleID = GT_INVALID;}
};

struct tagGuildNameGetEvent : public tagGameEvent
{
	DWORD	dwGuildID;
	tstring strName;

	tagGuildNameGetEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName, pSenderFrame)
	{dwGuildID = GT_INVALID; strName = _T("");}
};

struct tagInviteJoinGuildEvent : public tagGameEvent
{
	DWORD			dwRoleID;
	tagInviteJoinGuildEvent() : tagGameEvent( _T("GuildEvent_Invite"), NULL ), dwRoleID( GT_INVALID ){}
	tagInviteJoinGuildEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame), dwRoleID( GT_INVALID ){}
};

struct tagUseGuildSkillBookEvent : public tagGameEvent
{
	Item *pItem;
	tagUseGuildSkillBookEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		pItem = NULL;
	}
};

struct tagRefreshPickUpSilverEvent : public tagGameEvent
{
	const tagCommerceInfo *pCommerceInfo;
	tagRefreshPickUpSilverEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		pCommerceInfo = NULL;
	}
};

struct tagRefreshContributeSilverEvent : public tagGameEvent
{
	INT nLevel;
	const tagCommerceInfo *pCommerceInfo;
	tagRefreshContributeSilverEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		pCommerceInfo = NULL;
	}
};

struct tagRefreshGuildBusinessRankEvent : public tagGameEvent
{
	BOOL bCommend;
	std::vector<tagCommerceRank> vecRankInfo;
	tagRefreshGuildBusinessRankEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		bCommend = FALSE;
	}
};

struct tagRefreshBusinessRankCommendStateEvent : public tagGameEvent
{
	BOOL bCommend;
	tagRefreshBusinessRankCommendStateEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{
		bCommend = FALSE;
	}
};

struct tagGuildWarePriChange : public tagGameEvent
{
    DWORD       dwRoleID;
    BOOL        bCanUse;

    tagGuildWarePriChange(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame){}
};
struct tagGetSomeGuildNameEvent : public tagGameEvent
{
	int nFlag;
	tagGetSomeGuildNameEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagRefreshRemnantStateEvent : public tagGameEvent
{
	std::vector<DWORD> vecRemnantID;
	tagRefreshRemnantStateEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};



struct tagRefreshGodRewardEvent : public tagGameEvent
{
	tagNS_GodReward cmd;
	tagRefreshGodRewardEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagRefreshGodStateEvent : public tagGameEvent
{
	DWORD dwAttackGuildID;		// 进攻帮派ID
	DWORD dwDefenceGuildID;		// 防御帮派ID
	INT   nAttackGuildMember;	// 进攻帮派成员数目
	INT   nDefenceGuildMember;	// 防御帮派成员数目
	tagRefreshGodStateEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagRefreshPersonalGodStateEvent : public tagGameEvent
{
	INT   nKillTime;			// 个人击杀次数
	DWORD dwLiveTime;			// 个人活跃时间
	INT   nExp;					// 所得经验
	INT   nMoney;				// 所得金钱
	INT   nGuildExploit;		// 帮派功勋
	INT   nGuildContribute;		// 帮派贡献
	tagRefreshPersonalGodStateEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagRefreshFlagStateEvent : public tagGameEvent
{
	EGodMiracle	eGodID;					// 神迹类型
	EFlagType	eFlagType;				// 指定旗子
	EGuildCapType	eGuildCapType;		// 旗子所属
	tagRefreshFlagStateEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};
