//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: miracle_define.h
// author: songg
// actor:
// data: 2010-01-05
// last:
// brief: 仙界修行
//-----------------------------------------------------------------------------
#pragma once
#include "msg_common_errorcode.h"
#include "msg_talent.h"

#pragma pack(push, 1)


//-----------------------------------------------------------------------------
// 常量
//-----------------------------------------------------------------------------
const INT32 MIRACLE_PRC_RESET_NUM	= 100; //每日0点刷

const INT32 MIRACLE_BASIC_ITEM_COST		= 3300102;	//初级技能消耗物品	仙界渡劫令
const INT32 MIRACLE_MIDDLE_ITEM_COST	= 3300103; //中级技能消耗物品	仙界天劫令
const INT32 MIRACLE_SENIER_ITEM_COST	= 3300104; //高级技能消耗物品	仙界飞升令

const INT32 MIRACLE_ADD_PRC_NUM_ITEM	= 3300105; //仙界修行符

//-----------------------------------------------------------------------------
// 错误码
//-----------------------------------------------------------------------------
enum EMiracleSkillErrorCode
{
	E_Miracle_Success			= E_Skill_End,	//成功
	E_Miracle_LackItem ,						//缺少必备物品
	E_Miracle_LackPower	,						//缺少实力值
	E_Miracle_LackMoney	,						//少银子
	E_Miracle_LackPracticeNum ,					//修练点数不足

	E_Miracle_ItemNotExist ,					//物品不存在
	E_Miracle_SpecFunError ,					//特殊使用类型错误

};


//-----------------------------------------------------------------------------
// 枚举
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// 结构体
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//仙界技能结构 -- 仙界技能升级信息
//-----------------------------------------------------------------------------

struct tagMiracleSkill
{
	DWORD					dwSkillTypeID;		// 技能typeid

	DWORD					dwSilverCost;		// 花费
	INT16					n16PowerCost;		// 实力值
	DWORD					dwItemTypeID;		// 消耗物品的typeid
	INT16					n16ItemNumCost;		// 物品消耗数


	tagMiracleSkill()
	{
		ZeroMemory(this, sizeof(*this));
	}
	tagMiracleSkill(const tagMiracleSkill &rhs)
	{
		memcpy(this, &rhs, sizeof(*this));
	}
};


#pragma pack(pop)
