//-----------------------------------------------------------------------------
// filename: train_define.h
// author: lfmeng
// actor:
// data: 2010-3-10
// last:
// brief: 龙之试炼系统定义
//-----------------------------------------------------------------------------

#pragma once

#pragma pack(push, 1)


//-----------------------------------------------------------------------------
// 常量定义
//-----------------------------------------------------------------------------
#define	REWARD_ITEM_MAX_CNT		2			// 奖励物品最大个数
#define TRAIN_MAX_CNT			100		// 试炼最大个数

//-----------------------------------------------------------------------------
// 试炼状态
//-----------------------------------------------------------------------------
enum ETrainState
{
	ETS_Close	= 0,	// 未开启
	ETS_Open	= 1,	// 进行中
	ETS_Finish	= 2,	// 已完成(没有领取奖励)
	ETS_Pass		= 3,	// 已通过(已经领取完奖励)
};

//-----------------------------------------------------------------------------
// 物品奖励
//-----------------------------------------------------------------------------
struct tagItemReward
{
	DWORD			nItemTypeID;			// 获得的物品奖励
	INT				nItemCount;				// 物品奖励的个数
	INT				nItemQuality;			// 物品品质
};

//-----------------------------------------------------------------------------
// 奖励内容
//-----------------------------------------------------------------------------
struct tagTrainReward
{
	INT				nStrengthReward;					// 实力值奖励
	DWORD		dwMoneyReward;					// 金钱奖励
	DWORD		dwExpReward;							// 经验奖励
	tagItemReward	pItemReward[REWARD_ITEM_MAX_CNT];	// 物品奖励
};

//-----------------------------------------------------------------------------
// 试炼状态表(存数据库结构)
//-----------------------------------------------------------------------------
struct tagTrainState
{
	DWORD			dwTrainID;			// 试炼ID
	ETrainState		eTrainState;			// 试炼状态
	INT					nCompleteness;	// 试炼完成度
};
struct tagTrainStateTable
{
	BOOL			bFirstInit;
	INT				nTrainCnt;
	tagTrainState	pTrainState[TRAIN_MAX_CNT];

	tagTrainStateTable()
	{
		bFirstInit = TRUE;
		nTrainCnt = 0;
		ZeroMemory(pTrainState, sizeof(tagTrainState)*TRAIN_MAX_CNT);
	}

	tagTrainStateTable& operator = (tagTrainStateTable& other)
	{
		bFirstInit	= other.bFirstInit;
		nTrainCnt	= other.nTrainCnt;
		memcpy(pTrainState, other.pTrainState, sizeof(tagTrainState)*TRAIN_MAX_CNT);
		return *this;
	}
};

//-----------------------------------------------------------------------------
// 试炼属性表
//-----------------------------------------------------------------------------
struct tagTrainAttProto
{
	DWORD					dwTrainID;			// 试炼ID
	DWORD					dwType1;				// 试炼类型1（大的类别）
	DWORD					dwType2;				// 试炼类型2（小的类别）
	DWORD					dwMaxNum;		// 完成需要最大数
	tagTrainReward		TrainReward;		// 试炼通过的奖励
};

#pragma pack(pop)