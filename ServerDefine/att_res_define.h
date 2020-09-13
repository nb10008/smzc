//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: att_res_define.h
// author: Sxg
// actor:
// data: 2009-02-19
// last:
// brief: 资源管理器数据结构定义
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/longhun_define.h"
#include "../WorldDefine/godbid_define.h"
#include "../WorldDefine/compose_define.h"
#include "..\WorldDefine\tiger_define.h"

//-----------------------------------------------------------------------------
// 品质影响的装备属性相关参数
//-----------------------------------------------------------------------------
struct tagEquipQltyEffect
{
	// 基础属性影响系数
	FLOAT fWeaponFactor;
	FLOAT fArmorFactor;

	// 一级属性
	INT32 nAttAFactor;
	FLOAT fAttAFactor;
	INT32 nAttANumEffect;

	// 潜力值
	FLOAT fPotFactor;

	// 镶嵌孔数量 -- 记录出现的几率
	INT32 nHoleNumPct[MAX_EQUIPHOLE_NUM + 1];

	// 龙魂能力 -- 表里
	INT32 nLonghunPct[X_LONGHUN_TYPE_NUM];

	// 特殊属性
	INT32 nSpecAttPct;

	// 强化等级限制
	INT32	n8StrengLevelLimit;
};

//-----------------------------------------------------------------------------
// 技能主从之间的影响结构
//-----------------------------------------------------------------------------
struct tagSkillModify
{
	TList<DWORD>	listModify;		// 直接影响该主技能的从技能
};

//-----------------------------------------------------------------------------
// 时装生成规则相关定义
//-----------------------------------------------------------------------------
struct tagFashionGen
{
	FLOAT		fAppearanceFactor;	// 仪容属性(AppearancePct)
	INT16		n16ReinPct;			// 统御属性加成(ReinPct)
	INT16		n16SavvyPct;		// 悟性属性加成(SavvyPct)
	INT16		n16FortunePct;		// 福缘属性加成(FortunePct)
	INT8		n8ReinVal;			// 值=物品等级÷ReinVal[取整](ReinVal)
	INT8		n8SavvyVal;			// 值=物品等级÷SavvyVal[取整](SavvyVal)
	INT8		n8FortuneVal1;		// 值=FortuneVal1（上下浮动20%）+装备品级/FortuneVal2
	INT8		n8FortuneVal2;		// (FortuneVal1, FortuneVal2)
	INT8		n8Dummy[2];
};

struct tagFashionColorPct	// 时装生成时颜色概率
{
	INT16	n16ColorPct[X_COLOR_NUM];	// 颜色概率
};

//-----------------------------------------------------------------------------
// 任务完成可使用特殊道具定义
//-----------------------------------------------------------------------------
struct tagQuestUseSpeciItem
{
	INT16	n16QuestID;		// 任务ID
	DWORD	dwItemID;		// 道具ID
	INT16	n16ItemNum;		// 道具数量
};

//-----------------------------------------------------------------------------
// 击杀玩家获取的经验数
//-----------------------------------------------------------------------------
struct tagKillerExp
{
	INT16	n16Level;
	DWORD	dwExp;
	DWORD	dwBaseRwdExp; // 经验奖励基础值
	DWORD	dwBaseRwdSilver; // 金钱奖励基础值
};
// Jason 2010-5-24 飞升技能原型
struct tagLearnSoaringSkillProto
{
	//soar_skill_proto.xml
	DWORD	dwID;
	INT		nLearnSilverCosted;
	INT		nExpCosted;
	DWORD	dwItemNeeded;
	INT		nItemCount;
};
struct tagCastingProto
{
	DWORD	dwID;
	BYTE	byType;				// 0	武器；1	防具；2	饰品
	UINT16	u16Level;			// casting_proto.xml中的等级，对于要比装备熔铸次数大于一。
	UINT16	u16PotvalConsume;	// 潜力值消耗
	INT		nProbability;		// 成功几率，10000是百分之壹百
	ERoleAttribute		eCastingRoleAtt[5];
	INT		nRoleAttValue[5];
	DWORD	dwStuffTypeID[5];
	INT		nStuffNum[5];
	tagCastingProto()
	{
		ZeroMemory(this,sizeof(*this));
	}
};

struct tagMsgDeliver
{
	DWORD	dwID;
	DWORD	dwIDMsgID;
	DWORD	dwMapID;
	INT	X;
	INT	Y;
	INT	Z;
};

// 妖精食物阶段定义
struct tagFabaoFoodParam
{
	INT	nMiniLevel;		// 等级区间，最低等级
	INT nMaxLevel;			// 等级区间，最高等级
	INT nRecoveryValue;	// 心情恢复量
	tagFabaoFoodParam()
		: nMaxLevel(0),nMiniLevel(0),nRecoveryValue(0)	{}
};
// 妖精食物原型定义
struct tagFabaoFoodProto
{
	DWORD	dwItemTypeID;	// 道具typeid
	INT		nLevel;			// 要就妖精等级
	INT		nExp;			// 妖精食物额外经验
	DWORD	dwBuffID;		// 特效buffid
	DWORD	dwTriggerID;	// 特效triggerid
	tagFabaoFoodParam	param[3];
	tagFabaoFoodProto()
		: dwItemTypeID(GT_INVALID),nExp(0),nLevel(0),dwBuffID(GT_INVALID),dwTriggerID(GT_INVALID)
	{
	}
};
// 妖精果实原型
struct tagFabaoFruitProto
{
	DWORD	dwTypeID;		// 果实typeid
	INT		nLower;			// 最低提升
	INT		nHight;			// 最高提升
	INT		nExtType;		// 附加属性，当该索引与元素伤害或者抗性值相同时，会有附加提升量
	INT		nExtAdding;	// 附加提升
};

// 妖精特技原型
struct tagFabaoStuntProto
{
	DWORD	dwID;			// 妖精特技id
	INT		nStuntType;	// 特技类型，对应于某个元素伤害类型
	DWORD	dwTriggerID;	// 触发器
	DWORD	dwBuffID;		// buff
	BOOL	bCanRealized;	// 是否可以领悟
	INT		nIntelligenceLimit;	// 资质限制
	INT		nStageLimit;		// 等级限制
	BOOL	bBinding;			// 是否绑定，绑定的，不会被覆盖在学习技能书的时候
};

// 妖精点数提升原型 [3/13/2012 zhangzhihua]
const	INT FABAO_RESIST_UPGRADE_BASE = 10000;
struct tagFabaoAddPromoteTimesProto
{
	DWORD	dwID;			// Times提升次数，抵抗从10000开始计数 [3/13/2012 zhangzhihua]
	DWORD	dwMoney;		// 消耗金钱 [3/13/2012 zhangzhihua]
};

// 帮派每日奖励原型
struct tagGuildDailyRewardProto
{
	DWORD	dwID;
	INT		nGuildPos;		// 帮派职位
	INT		nWinNum;		// 占领战场的次数
	DWORD	dwItemTypeID;	// 礼包的typeid
	DWORD	dwBuffID;		// 状态id
};

// 竞标属性表原型
struct tagGodBidProto
{
	DWORD 			dwProtoID;					// 竞标原型ID
	DWORD 			dwItemID;					// 竞标物品ID
	DWORD 			dwItemCnt;					// 竞标物品个数
	EGodBidType	eGodBidType;				// 竞标类型枚举
	DWORD			dwBasePrice;				// 物品
	DWORD			dwBidLowerLimit;		// 出价底限
};



// 妖精繁殖表和繁殖类型细表共享的数据结构
struct tagFabaoBirthItemType
{
	DWORD	dwType;	// 妖精类型，对应fabao_birth.xml表中的item_typex字段(xml属性)；或者对应fabao_birth_type.xml中的itemx
	INT		nPct;			// 几率
};


template<typename T,INT num>
struct tagFabaoBirthFunc : public T 
{
	tagFabaoBirthItemType Items[num];
	DWORD GetItemByPct(INT nPct)
	{
		INT pct_total = 0;
		for( int i = 0; i < num; ++i )
		{
			pct_total += Items[i].nPct;
			if( nPct <= pct_total )
			{
				return Items[i].dwType;
			}
		}
		return GT_INVALID;
	}
	VOID PushBack(DWORD	dwType,INT nPct)
	{
		for( int i = 0; i < num; ++i )
		{
			if( !P_VALID(Items[i].dwType) )
			{
				Items[i].dwType = dwType;
				Items[i].nPct = nPct;
				break;
			}
		}
	}
private:
	void __copy(const tagFabaoBirthFunc<T,num> * p)
	{
		memcpy(Items,p->Items,sizeof(Items));
		// 父类必须实现的函数Copy
		Copy(p);
	}
public:
	tagFabaoBirthFunc(){}
	tagFabaoBirthFunc(const tagFabaoBirthFunc<T,num> & d)
	{
		__copy(&d);
	}
	tagFabaoBirthFunc<T,num> & operator= (const tagFabaoBirthFunc<T,num> & d)
	{
		__copy(&d);
		return *this;
	}
};


#define MFabaoBirthItemsCount 10
// 妖精繁殖表原型
struct tagFabaoBirthProto
{
	DWORD	dwID;
	INT		nLowLimit;
	INT		nHigherLimit;
	VOID Copy(const tagFabaoBirthProto * p)
	{
		dwID = p->dwID;
		nLowLimit = p->nLowLimit;
		nHigherLimit = p->nHigherLimit;
	}
};

typedef tagFabaoBirthFunc<tagFabaoBirthProto,MFabaoBirthItemsCount>		TFabaoBirthProto;

#define MFabaoBirthItemTypes	6

struct tagFabaoBirthItem
{
	DWORD	dwMainFabaoTypeID;
	VOID Copy(const tagFabaoBirthItem * p)
	{
		dwMainFabaoTypeID = p->dwMainFabaoTypeID;
	}
};
typedef tagFabaoBirthFunc<tagFabaoBirthItem,MFabaoBirthItemTypes>		TFabaoBirthItem;
// 妖精繁殖类型细表
struct tagFabaoBirthItemTypeProto
{
	DWORD	dwID;
	INT		nItemType;		// 注意用这个作为map的索引
	map<DWORD,TFabaoBirthItem> Items;
};

const INT32 GOLD_EGG_MAX_CNT = 20;
struct tagGoldEggPctProto
{
	DWORD dwLayer;			// 层数
	INT n32EmptyPct;		// 空蛋几率
	INT n32ItemPct[GOLD_EGG_MAX_CNT];  // 蛋可以砸出的物品的几率信息
};

struct tagGoldEggProto
{
	DWORD dwLayer;			// 层数
	INT nGenConsum;			// 普通消耗
	INT nSpecConsum;			// 彩蛋消耗
	DWORD dwItemID[GOLD_EGG_MAX_CNT];	// 蛋可以砸出的物品
};

struct tagAdvCompMaterial
{
	DWORD dwTypeID;
	DWORD dwCount;
};
struct tagFashionComp
{
	DWORD dwTypeID;
	DWORD dwPct;
};

const INT32 MAX_ADVANCED_COMPOSE_MATERIAL = 2; // 高级合成需要材料数
const INT32 MAX_FASHION_CAN_COMPOSE = 20; // 高级合成需要材料数
struct tagAdvancedComposeProto
{
	DWORD dwComposeID;									// 合成ID
	EAdvanceComposeType eAdvCompType;		// 高级合成类型
	DWORD dwDesignID;										//  设计图ID
	tagAdvCompMaterial Material[MAX_ADVANCED_COMPOSE_MATERIAL];
	INT64 n64SilverCost;										// 金钱消耗
	INT nVitalityCost;									// 活力消耗
	tagFashionComp dwFashionComp[MAX_FASHION_CAN_COMPOSE];
};

struct tagGodHeadProto
{
	DWORD	dwID;
	INT		nMinLevel;			// 等级下限
	INT		nMaxLevel;			// 等级上限
	INT		nCondenseValue;		// 当前级别需要消耗，多少凝聚度
	INT		nCanActNum;			// 能够激活的领域数量
};


struct tagTigerProto
{
	DWORD dwId;										// 有4个 1 ，5 ，10， 50
	DWORD dwRate[MAX_TIGER_NUM];		// 奖励的概率
	DWORD dwNum[MAX_TIGER_NUM];		// 奖励的数量
};

struct tagExpOfflineProto
{
	DWORD dwID;										
	DWORD dwExpbase;		
	DWORD dwExpAdditional;	
	DWORD dwGold;
};

struct tagForceSkillProto
{
	DWORD	dwID;										
	BYTE	bySide;		
	DWORD	dwJob;
	DWORD	dwLevel;
	DWORD	dwNeedSkill;
	BOOL	bForerver;
	DWORD	dwOpenSoul;
	DWORD	dwOpenMoney;
	DWORD	dwContinueSoul;
	DWORD	dwContinueMoney;
	BOOL	bDelete;
};

struct tagForceLevelProto
{
	DWORD dwID;										
	DWORD dwPoint;
};

