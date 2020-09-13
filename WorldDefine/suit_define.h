//--------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: suit_define.h
// author: 
// actor:
// data: 2008-12-1
// last:
// brief: 套装相关定义
//--------------------------------------------------------------------------------------------
#pragma once

#include "ItemDefine.h"

//--------------------------------------------------------------------------------------------
// 常量
//--------------------------------------------------------------------------------------------
const INT MAX_SUIT_ATT_NUM			= 5;	// 套装(隐藏)属性个数
const INT MAX_SUIT_EQUIP_NUM		= 12;	// 套装最多部件个数
const INT MIN_SUIT_EQUIP_NUM		= 2;	// 套装最少部件个数

//套装属性类型
enum ESuitAttType
{
	ESSAT_Null			= 0,	//空
	ESSAT_Brave			= 1,	//勇猛之心
	ESSAT_Manliness		= 2,	//刚毅之躯
	ESSAT_Resplendence	= 3,	//辉煌之气
	ESSAT_PureSpirit	= 4,	//纯洁之灵

	ESSAT_End
};


//--------------------------------------------------------------------------------------------
// 静态结构
//--------------------------------------------------------------------------------------------
struct tagSuitProto
{
	DWORD				dwID;								// 套装编号
	DWORD				dwTriggerID[MAX_SUIT_ATT_NUM];		// Trigger ID
	DWORD				dwBuffID[MAX_SUIT_ATT_NUM];			// Buff ID
	INT8				n8ActiveNum[MAX_SUIT_ATT_NUM];		// 隐藏属性激活件数
	INT8				n8SpecEffectNum;					// 特效激活件数

	tagRoleAttEffect	BaseEffect[MAX_ROLEATT_SPIN_EFFECT];// 精纺时属性加成 只有精纺装备才有效

	ESuitAttType		eAttType; //套装属性类型

	INT8				n8Dummy[2];
};

//--------------------------------------------------------------------------------------------
// 发送到客户端消息内的数据结构
//--------------------------------------------------------------------------------------------
struct tagSuitInit
{
	DWORD	dwSuitID;
	INT		nEquipNum;
};


//--------------------------------------------------------------------------------------------
// 客户端静态结构
//--------------------------------------------------------------------------------------------
struct tagSuitProtoClient : public tagSuitProto
{
	TCHAR		szSuitName[X_SHORT_NAME];
	TCHAR		szSpecEffectMod[X_SHORT_NAME];
};