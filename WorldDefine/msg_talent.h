//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_talent.h
// author: 
// actor:
// data: 2008-10-20
// last:
// brief: 天资技能相关消息
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "RoleDefine.h"
#include "skill_define.h"

#pragma pack(push, 1)

enum
{
	E_LearnSkill_Existed				=		1,		// 该技能已经存在
	E_LearnSkill_ProtoNotFound			=		2,		// 没有找到该技能对应的静态属性
	E_LearnSkill_NPCNotEixst			=		3,		// NPC不存在
	E_LearnSkill_NPCTooFar				=		4,		// NPC距离太远
	E_LearnSkill_NPCCanNotTeach			=		5,		// NPC不能教授该技能
	E_LearnSkill_ItemNotFound			=		6,		// 物品不存在
	E_LearnSkill_ItemInvalid			=		7,		// 物品不是技能书
	E_LearnSkill_NoPreSkill				=		8,		// 没有前置技能
	E_LearnSkill_NeedMoreLevel			=		9,		// 人物等级不足
	E_LearnSkill_NeedMoreTalentPoint	=		10,		// 该技能需要的天资点投放量不足
	E_LearnSkill_ClassLimit				=		11,		// 职业限制
	E_LearnSkill_NoTalentType			=		12,		// 该技能无天资类型
	E_LearnSkill_NoTalentPoint			=		13,		// 无天资点可投
	E_LearnSkill_ExceedTalentMaxNum		=		14,		// 超过人物可投的天资数量上限
	E_LearnSkill_ProduceSkillFull		=		15,		// 目前玩家身上的生产技能已满
	E_LearnSkill_NotSoaring				=		16,		// 没有飞升，不可以学习飞升技能

	E_LevelUpSkill_NotExist				=		21,		// 技能不存在
	E_LevelUpSkill_NotFixed				=		22,		// 技能不是固定升级方式
	E_LevelUpSkill_NoPreSkill			=		23,		// 没有前置技能
	E_LevelUpSkill_ExceedMaxLevel		=		24,		// 超过技能等级上限
	E_LevelUpSkill_ExceedMaxLearnLevel	=		25,		// 超过技能可学上限
	E_LevelUpSkill_NeedMoreLevel		=		26,		// 人物等级不足
	E_LevelUpSkill_NeedMoreTalentPoint	=		27,		// 该技能需要的天资点投放量不足
	E_LevelUpSkill_ClassLimit			=		28,		// 职业限制
	E_LevelUpSkill_NoTalentType			=		29,		// 该技能无天资类型
	E_LevelUpSkill_NoTalentPoint		=		30,		// 无天资点可投
	E_LevelUpSkill_ProtoNotFound		=		31,		// 没有找到该技能对应的静态属性
	E_LevelUpSkill_NPCNotEixst			=		32,		// NPC不存在
	E_LevelUpSkill_NPCTooFar			=		33,		// NPC距离太远
	E_LevelUpSkill_NPCCanNotTeach		=		34,		// NPC不能教授该技能

	E_ForgetSkill_NotExist				=		51,		// 技能不存在
	E_ForgetSkill_CanNotBeForgotten		=		52,		// 该种技能不能被遗忘
	E_ForgetSkill_NPCNotValid			=		53,		// NPC不合法

	E_ClearTalent_NoTalent				=		71,		// 人物没有该天资
	E_ClearTalent_NoItem				=		72,		// 无洗点物品

	E_Skill_End,
};

//玩家修炼天资点的情况
enum ERolePracticeTalentErrorCode 
{
	ERPT_Success           =  E_Success,

	ERPT_NoEnoughMonney    =  1,            //  游戏币不足
	ERPT_OverStepMaxTalent =  2,            //  超出最大天资点数

	ERPT_End,
};

//玩家修炼天资点的情况
enum ERoleGetTalentErrorCode 
{
	ERTEC_Success                  =   E_Success,
    ERTEC_OverMaxTalentAvail_Limit =   1,    //   玩家做任务或者升级获得天资超过上限
	ERTEC_End,
};
// 定义飞升等特殊技能的errorcode，将来扩展可以重用
enum ERoleSpecSkillErrorCode
{
	ERSSEC_Success	=	E_Success,
	ERSSEC_TimesLimitted = E_Skill_End + 1,				// 学习技能次数限制
	ERSSEC_SilverLimitted	= E_Skill_End + 2,			// 背包金钱不足
	ERSSEC_ExpLimitted		= E_Skill_End + 3,			// 经验不足于学习该技能
	ERSSEC_ItemLimitted		= E_Skill_End + 4,			// 道具数量不足
	ERSSEC_SkillPracticeTimesLimitted	= E_Skill_End + 5,	// 技能修炼次数限制，超了
};
//------------------------------------------------------------------------------
// 学习一个新技能（可能为技能书，NPC处或技能书）
//------------------------------------------------------------------------------
CMD_START(NC_LearnSkill)
	DWORD	dwSkillID;				// 技能ID（大ID）
	DWORD	dwNPCID;				// NPC的全局ID（如果不是通过NPC则为GT_INVALID）
	INT64	n64ItemID;				// 技能书物品的64位ID（如果不是通过技能书，则为GT_INVALID）
CMD_END

CMD_START(NS_LearnSkill)
	DWORD	dwErrorCode;			// 错误码
	DWORD	dwSkillID;				// 技能ID（大ID）
CMD_END

CMD_START(NS_AddSkill)
	tagSkillMsgInfo	Skill;			// 添加一个技能
CMD_END

//-------------------------------------------------------------------------------
// 升级一个技能
//-------------------------------------------------------------------------------
CMD_START(NC_LevelUpSkill)
	DWORD	dwSkillID;				// 技能ID（大ID）
	DWORD	dwNPCID;				// NPC的全局ID（如果不是通过NPC则为GT_INVALID）
	INT64	n64ItemID;				// 技能书物品的64位ID（如果不是通过技能书，则为GT_INVALID）
CMD_END

CMD_START(NS_LevelUpSkill)
	DWORD	dwErrorCode;			// 错误码
	DWORD	dwSkillID;				// 技能ID（大ID）
CMD_END

CMD_START(NS_UpdateSkill)
	tagSkillMsgInfo	Skill;			// 技能
CMD_END

CMD_START(NS_UpdateSkillCoolDown)
	DWORD			dwSkillID;		// 技能ID
	INT				nCoolDown;		// 新的冷却时间
CMD_END

CMD_START(NS_UpdateSkillCoolDownEx)
	INT				nCoolDown;		// 新的冷却时间
	INT				nCount;			//技能数量
	DWORD			dwSkillID[1];		// 技能ID列表
CMD_END

//-------------------------------------------------------------------------------
// 遗忘一个技能
//-------------------------------------------------------------------------------
CMD_START(NC_ForgetSkill)
	DWORD	dwSkillID;		// 技能ID（大ID）
	DWORD	dwNPCID;		// NPCID
CMD_END

CMD_START(NS_ForgetSkill)
	DWORD	dwErrorCode;	// 错误码
	DWORD	dwSkillID;		// 技能ID（大ID）
CMD_END

//-------------------------------------------------------------------------------
// 洗点
//-------------------------------------------------------------------------------
CMD_START(NC_ClearTalent)
	ETalentType	eType;		// 天资类型
	INT64		n64ItemID;	// 使用的道具
CMD_END

CMD_START(NS_ClearTalent)
	DWORD		dwErrorCode;// 错误码
	ETalentType	eType;		// 天资类型
CMD_END

CMD_START(NS_RemoveSkill)
	DWORD	dwSkillID;
CMD_END

//-------------------------------------------------------------------------------
// 增加一个天资
//------------------------------------------------------------------------------
CMD_START(NS_AddTalent)
	ETalentType	eType;
	INT			nPoint;
CMD_END

//------------------------------------------------------------------------------
// 删除一个天资
//------------------------------------------------------------------------------
CMD_START(NS_RemoveTalent)
	ETalentType	eType;
CMD_END

//------------------------------------------------------------------------------
// 更新一个天资
//-----------------------------------------------------------------------------
CMD_START(NS_UpdateTalent)
	ETalentType	eType;
	INT			nPoint;
CMD_END

//--------------------------------------------------------------------------------
//  玩家购买天资点 
//--------------------------------------------------------------------------------
CMD_START(NC_RolePracticeTalentPoint)
     INT	    nTalentPointAdd;       //  修炼天资点  
CMD_END

CMD_START(NS_RolePracticeTalentPoint)
    ERolePracticeTalentErrorCode  dwErrorCode ;   // 错误码
CMD_END


//--------------------------------------------------------------------------------
//  玩家做任务或升级获得天资 
//--------------------------------------------------------------------------------
CMD_START(NS_RoleGetTalentPoint)
     ERoleGetTalentErrorCode  dwErrorCode ;   // 错误码
CMD_END

//练习飞升技能
CMD_START(NC_PracticeSoaringSkill)
	DWORD dwSkillID;			// 大ID
CMD_END

//返回错误码
CMD_START(NS_PracticeSoaringSkill)
	DWORD	dwErrCode;			// 错误码
	INT16	n16CurPrcNum;		// 当前人物修炼点数
CMD_END


#pragma pack(pop)