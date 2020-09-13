//--------------------------------------------------------------------------------------------
// Copyright (c) 2010 DACHENG Entertainment All rights reserved.
// filename: TrainProtoData.h
// author: Su.Zhang
// actor:
// data: 2010-3-10
// last:
// brief: 龙之试炼数据结构定义
//--------------------------------------------------------------------------------------------

#pragma once

#include "..\WorldDefine\train_define.h"

// 龙之试炼静态属性
struct tagTrainProtoData
{
	DWORD					dwTrainID;							// 试炼ID
	tstring					Name;								// 试炼名称

	mutable ETrainState		eType;								// 试炼状态

	INT						nCapability;						// 奖励实力值
	DWORD					dwItem1ID;							// 物品ID
	INT						nItem1Num;							// 物品数量
	BYTE					byItem1Quality;						// 物品品级
	DWORD					dwItem2ID;							// 物品ID
	INT						nItem2Num;							// 物品数量
	BYTE					byItem2Quality;						// 物品品级

	INT						nLevelOneType;						// 一级类型
	INT						nLevelTwoType;						// 二级类型
	DWORD					dwMoney;							// 金钱
	DWORD					dwExp;								// 经验
	INT						nMaxNum;							// 任务追踪中的最大个数
	INT						bVisible;							// 是否可见
	mutable INT				nCurrentNum;						// 当前任务的数量

	tstring					GoalDescription;					// 目标说明
	tstring					FinishDescription;					// 完成说明
	tstring					StartDescription;					// 开启说明

	tstring					DescLevelOne;						// 一级说明文字
	tstring					DescLevelTwo;						// 二级说明文字

};


class TrainProtoData
{
public:
	typedef std::map<DWORD, tagTrainProtoData> TrainProtoMap;
public:

	BOOL LoadFromFile();
	const TrainProtoMap& GetData() const;
	const tagTrainProtoData *GetTrainProto(DWORD id) const;
private:
	TrainProtoMap m_mapTrainProtoData;	// 活动ID所对应的属性表
};
