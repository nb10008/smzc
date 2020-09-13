//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_define.h
// author: 
// actor:
// data: 2008-10-17
// last:
// brief: 服务器用物品相关结构
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)
#include <vector>
using namespace std;

#include "../WorldDefine/itemdefine.h"
//-----------------------------------------------------------------------------
// 容器update时间(单位：tick)
//-----------------------------------------------------------------------------
const INT ITEM_UPDATE_TIME		= 60 * TICK_PER_SECOND;		// 60s

//-----------------------------------------------------------------------------
// 装备B类属性生成概率[0，10000]
//-----------------------------------------------------------------------------
const INT g_nEquipSpecAttPct[X_EQUIP_SPECATT_NUM] = 
{
	800,	400,	200,	800,	400,
	200,	800,	400,	200,	100,
	800,	400,	200,	100,	500,
	500,	500,	500,	500,	500,
	100,	500,	500,	100
};

//-----------------------------------------------------------------------------
// 同一容器中物品移动需由方法传出的参数
//-----------------------------------------------------------------------------
struct tagItemMove
{
	tagItem	*pItem2;		// 目标位置上的物品
	INT16	n16NumRes1;		// 待移动物品的最终个数
	INT16	n16NumRes2;		// 目标位置上物品的最终个数
	bool	bCreateItem;	// 目标位置上的物品是否为新创建的(拆分时为true)
	bool	bOverlap;		// 是否堆叠
	bool	bChangePos;		// 位置是否移动

	tagItemMove()
	{
		Init();
	}
	
	VOID Init()
	{
		pItem2		= NULL;
		n16NumRes1	= 0;
		n16NumRes2	= 0;
		bCreateItem = FALSE;
		bOverlap	= FALSE;
		bChangePos	= TRUE;
	}
};

//-----------------------------------------------------------------------------
// 保存角色数据时需更新的物品数据
//-----------------------------------------------------------------------------
struct tagItemUpdate
{
	INT64		n64Serial;
	DWORD		dwOwnerID;
	DWORD		dwAccountID;
	INT32		nUseTimes;		// 物品上技能使用次数
	INT16		n16Num;			// 物品个数
	INT16		n16Index;		// 容器中位置
	BYTE		byConType;		// 容器类型
	BYTE		byBindType;		// 绑定类型
	DWORD       dwUnlockTime;
	tagDWORDTime	dw1stGainTime;
	DWORD			dwSpecVal1;	// 物品特殊功能字段值1，动态属性，受特定条件限制
	DWORD			dwSpecVal2;	// 物品特殊功能字段值2
};

struct tagEquipSpecUpdate
{
	INT64			n64Serial;
	tagEquipSpec	equipSpec;
};

//-----------------------------------------------------------------------------
// IM 物品影响属性枚举
//-----------------------------------------------------------------------------
enum EIMEffect
{
	EIME_Null			= 0,

	EIME_Color			= 1,	// 颜色
	EIME_ComAdvance		= 2,	// 提高成功几率
	EIME_ProtectSign	= 3,	// 保底符（强化失败时不消耗装备潜力值）
};

//-----------------------------------------------------------------------------
// IM 物品影响
//-----------------------------------------------------------------------------
struct tagIMEffect
{
	EIMEffect	eEffect;	// 影响类别
	DWORD		dwParam1;	// 影响值
	DWORD		dwParam2;
	tagIMEffect()
	{
		eEffect = EIME_Null;
		dwParam1 = 0;
		dwParam2 = 0;
	}
};

//-----------------------------------------------------------------------------
// 物品冷却信息
//-----------------------------------------------------------------------------
struct tagCDInfo
{
	DWORD		dwElapseTime;
	DWORD		dwRemainTime;
};


// Jason 2009-12-8 点金石配置结构
struct tagGoldStone
{
	DWORD	dwTypeID;
	INT		nPct;
};
struct tagPhilosophersStoneConfig
{
	union unionKey
	{
		struct tagPair
		{
			INT nStoneType;
			DWORD dwSrcItemTypeID;
		} m_Pair;
		INT64 m_n64Key;
	} uniKey;
	//DWORD dwDestItemTypeID;
	vector<tagGoldStone> vecGoldStone;
	DWORD GetStoneTypeIDByPct(INT nPct)
	{
		INT pct = 0;
		for( vector<tagGoldStone>::iterator i = vecGoldStone.begin(); i != vecGoldStone.end(); ++i )
		{
			pct += i->nPct;
			if( nPct <= pct )
				return i->dwTypeID;
		}
		return GT_INVALID;
	}
	tagPhilosophersStoneConfig()
	{
		uniKey.m_n64Key = 0;
	}
	tagPhilosophersStoneConfig(const tagPhilosophersStoneConfig & t)
	{
		_copy(t,*this);
	}
	tagPhilosophersStoneConfig & operator= (const tagPhilosophersStoneConfig &t)
	{
		_copy(t,*this);
		return *this;
	}
private:
	static void _copy(const tagPhilosophersStoneConfig & t, tagPhilosophersStoneConfig & t1)
	{
		t1.uniKey.m_n64Key = t.uniKey.m_n64Key;
		t1.vecGoldStone = t.vecGoldStone;
	}
};

//-----------------------------------------------------------------------------
// 获取物品数据的结构
//-----------------------------------------------------------------------------
struct tagItemData
{
	INT64	n64Serial;
	DWORD	dwTypeID;
};

// 稀有物品的存在形式
enum EGlobalLimitedItemExistType
{
	EGLIET_Item		= 0,	// 物品
	EGLIET_Skill	= 1,	// 技能
};

// 稀有物品的存在记录
struct tagGlobalLimitedItem
{
	DWORD						dwID;
	EGlobalLimitedItemExistType eType;
	INT64						n64ID;			// 以最长的类型为准,如果是物品，则是物品64位ID，如果是技能，则为技能所有者ID
	DWORD						dwAreaID;		//稀有领域ID
	INT							nTime;			//剩余时间
	DWORD						dwRecordTime;	//记录时间

	tagGlobalLimitedItem& operator =(const tagGlobalLimitedItem &gli)
 	{
 		if( this == &gli )
 			return *this;
 
 		this->dwID			= gli.dwID;
 		this->eType			= gli.eType;
		this->n64ID			= gli.n64ID;
		this->dwAreaID		= gli.dwAreaID;
 		this->nTime			= gli.nTime;
		this->dwRecordTime	= gli.dwRecordTime;
 		return *this;
 	}
};

#pragma pack(pop)
