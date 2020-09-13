//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_GuildBattle.h
// author: chdeng
// actor:
// data: 2009--01--25
// last:
// brief: 客户端和服务器端间消息 -- 帮战相关
//-----------------------------------------------------------------------------
#pragma once
#include "guild_define.h"

#pragma pack(push, 1)

enum EGuildErrorCode
{
	EGEC_Success = 0,
	EGEC_NPC_NotExist	= 1,	// 战场相关npc不存在
	EGEC_NPC_DisFar	= 2,	// 与NPC距离太远
	EGEC_NPC_TypeMismatch = 3,	// npc类型不匹配
	EGEC_DailyReward_LevelLimit	= 4,	// 领取每日奖励等级不足
	EGEC_DailyReward_GuildPosLimit	= 5,	// 领取每日奖励，职位数量限制
	EGEC_DailyReward_AlreadyTaken		= 6,	// 已经领取过每日奖励
	EGEC_DailyReward_CannotFindProto = 7,	// 没有找到奖励原型
	EGEC_DailyReward_BagFull = 8,	// 背包已满
};

// 同步三界阵旗所属
CMD_START(NS_SyncFlagAttack)
	EGodMiracle eGodID;					// 神迹类型
	EFlagType eFlagType;				// 指定旗子
	EGuildCapType eGuildCapType;		// 旗子所属
	INT nFlagPosX;
	INT nFlagPosY;
	INT nFlagPosZ;
CMD_END

CMD_START(NC_ApplyCapGodMiracle)		// 申请争夺神迹
	BOOL				bAddBid;		// 是否是追加竞价
	EGodMiracle			dwGodID;		// 要争夺的神迹ID
	DWORD				dwMoney;		// 玩家出据的金钱
CMD_END

CMD_START(NS_ApplyCapGodMiracle)		// 申请争夺神迹返回
	DWORD				dwErrorCode;
	EGodMiracle			dwGodID;		// 要争夺的神迹ID
	DWORD				dwMoney;		// 玩家出据的金钱
CMD_END

CMD_START(NC_GetAllGodInfo)				// 获取神迹的所有信息 
CMD_END


struct tagGuildInfo
{
	DWORD dwGuildID;	// 帮派ID
	int   nLevel;		// 帮派等级
	INT   nNum;			// 成员数量 
	tagGuildInfo() : dwGuildID(GT_INVALID), nLevel(0), nNum(0){}
	tagGuildInfo& operator = (tagGuildInfo& sGuildInfo)
	{
		dwGuildID = sGuildInfo.dwGuildID;
		nLevel = sGuildInfo.nLevel;
		nNum = sGuildInfo.nNum;
		return *this;
	}
};

struct tagGodInfo
{
	EGodMiracle			eGodType;				// 神迹类型
	EGodMiracleState	eGodState;				// 神迹的状态
	//DWORD				dwDefendGuild;			// 防御的帮派ID(无防御帮派及为0，无防御帮派那么就不存在争夺帮派)
	tagGuildInfo        sDefenceGuild;			// 防御的帮派(无防御帮派dwGuildID为0，无防御帮派那么就不存在争夺帮派)
	DWORD				dwStarttime;			// 防御帮派夺取神迹时的时间
	tagGuildInfo        sCapGuild;				// 进攻帮派
	DWORD				dwApplyMoney;			// 发起争夺的竞价
	DWORD				dwBidFund;				// 客户端公会当前出价（不在前三位就是0）
	DWORD				dwAttAlliance[MAX_ATTACK_ALLIANCE_GUILD];
	DWORD				dwDefAlliance[MAX_DEFANCE_ALLIANCE_GUILD];

	tagGodInfo():dwStarttime(0),dwApplyMoney(0)
	{}
};


CMD_START(NS_GetAllGodInfo)				// 返回所有神迹的所有信息 
	BYTE		byGodNum;				// 神迹的数量
	tagGodInfo	sTagInfo[1];			// 神迹的信息
CMD_END


// 取同盟列表
CMD_START(NC_GetAllianceList)
	EGodMiracle			eGodID;		// 战场ID
CMD_END

CMD_START(NS_GetAllianceList)
	EGodMiracle			eGodID;		// 战场ID
	EGuildCapType		eGuildCapType;
	DWORD				dwGuildCnt;			// 如果为0，其他可以忽略
	tagGuildInfo		sGuildInfo[1];
CMD_END

// 战场同盟公会报名
CMD_START(NC_ApplyForAlliance)
	EGodMiracle			dwGodID;		// 战场ID
	EGuildCapType		eGuildCapType;
CMD_END

CMD_START(NS_ApplyForAlliance)
	DWORD				dwErrorCode;
	EGodMiracle			dwGodID;		// 战场ID
CMD_END

// 确认同盟
CMD_START(NC_ConfirmAllianceGuild)
	EGodMiracle			dwGodID;		// 战场ID
	DWORD				dwGuildID;
CMD_END

CMD_START(NS_ConfirmAllianceGuild)
	DWORD				dwErrorCode;
	DWORD				dwGuildID;
	EGodMiracle			dwGodID;		// 战场ID
	EGuildCapType		eGuildCapType;
CMD_END

struct tagWinOrLoseRoleInfo
{
	DWORD dwRoleID;
	int   nKillTime;
	tagWinOrLoseRoleInfo():dwRoleID(0),nKillTime(0)
	{}
};
// 神迹争夺结束后奖励, 服务器主动发给客户端
CMD_START(NS_GodReward)
	bool bWin;							// 是否我方获胜

	int  nKillTime;						// 击杀次数
	DWORD dwLiveTime;					// 活跃时间
	int  nExploit;						// 帮派功勋
	int  nContribute;					// 帮派贡献
	
	tagWinOrLoseRoleInfo	sRoleIndo[5];// 排名前五位的玩家
CMD_END

// 进入神迹区域
CMD_START(NS_EnterGodArea)
	DWORD dwAttackGuildID;		// 进攻帮派ID
	DWORD dwDefenceGuildID;		// 防御帮派ID
	INT   nAttackGuildMember;	// 进攻帮派成员数目
	INT   nDefenceGuildMember;	// 防御帮派成员数目
CMD_END

// 离开神迹区域
CMD_START(NS_LeaveGodArea)
CMD_END

// 在神迹区域内的状况刷新
CMD_START(NS_GodPersonalState)
	INT   nKillTime;			// 个人击杀次数
	DWORD dwLiveTime;			// 个人活跃时间
	INT   nExp;					// 所得经验
	INT   nMoney;				// 所得金钱
	INT   nGuildExploit;		// 帮派功勋
	INT   nGuildContribute;		// 帮派贡献
CMD_END

// 向被顶掉的帮派帮主发送被顶掉信息
CMD_START(NS_InsteadInfo)
	EReturnMoneyReason	eReturnMoneyReason;	// 是否是进攻权确认导致竞价失败
	EGodMiracle			dwGodID;			// 要争夺的神迹ID
CMD_END

//////////////////////////////////////////////////////////////////////////
// 公会战争奖励
//////////////////////////////////////////////////////////////////////////
// 请求当前占领的战场数量
CMD_START(NC_GuildOccupyedNum)
CMD_END

CMD_START(NS_GuildOccupyedNum)
	INT		nNum;				// 当前公会占领战场的数量
	bool		bAlreadyTake;	// 今天的奖励是否已经领取过
CMD_END

// 领取奖励
CMD_START(NC_TakeGuildWarReword)
	DWORD dwNPCID;	// NPCid
CMD_END

CMD_START(NS_TakeGuildWarReword)
	DWORD dwErrorCode;
CMD_END

// 战场占领分数和活跃时间奖励的提示
CMD_START(NS_GetBattleReward)
	EBattleFieldRewardType eRewardType;
	DWORD dwPointOrTime;
	DWORD dwExp;
	DWORD dwSilver;
CMD_END

#pragma pack(pop)