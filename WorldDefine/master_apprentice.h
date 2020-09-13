//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: master_apprentice.h
// author: twan
// actor:
// data: 2010-01-18
// last: 
// brief: 师徒相关
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "time.h"

#pragma pack(push, 1)

const	INT		MAX_APPRENTICE_NUM				=		5;		// 最大徒弟数量
const	INT		GOOD_STU_CAN_LEAVE_LEVEL		=		50;		// 徒弟出师等级
const	INT		MASTER_QUALIFICATION_LEVEL		=		50;		// 为人师表资格等级
const	UINT16	MASTER_QUALIFICATION_QUEST_ID	=		8001;	// 师傅资格任务
const	INT		MASTER_BREAKOUT_DAY_LIMIT		=		1;		// 解除师徒关系需要的一方未上线时间（天数）
const	DWORD	APPRENTICE_LEVEL_UP_ITEM		=		1370001;	// 师徒宝鉴TypeID

//-----------------------------------------------------------------------------
// 存数据库结构
//-----------------------------------------------------------------------------
struct tagApprenticeData
{
	DWORD	dwData[MAX_APPRENTICE_NUM];
	tagApprenticeData()
	{
		ZeroMemory(dwData, sizeof(DWORD)*MAX_APPRENTICE_NUM);
	}

	tagApprenticeData& operator = (tagApprenticeData& other)
	{
		memcpy(this->dwData, other.dwData, sizeof(other.dwData));
		return *this;
	}
};

//-----------------------------------------------------------------------------
// 师徒授业点数属性表
// 存数据库同门结构
//-----------------------------------------------------------------------------
struct tagClassMateData : public tagApprenticeData
{
	INT32	nJingWuPoint;
	INT32	nQinWuPoint;
	INT32	nLevelUpBonus;
	tagClassMateData():tagApprenticeData(), nJingWuPoint(0), nQinWuPoint(0), nLevelUpBonus(0)
	{
		
	}

	tagClassMateData& operator = (tagClassMateData& other)
	{
		memcpy(this->dwData, other.dwData, sizeof(other.dwData));
		this->nJingWuPoint = other.nJingWuPoint;
		this->nQinWuPoint = other.nQinWuPoint;
		this->nLevelUpBonus = other.nLevelUpBonus;
		return *this;
	}
};

//-----------------------------------------------------------------------------
// 师徒精武点数属性表
//-----------------------------------------------------------------------------
struct tagMasterApprenticeProto
{
	DWORD		dwLevelID;				// 等级
	INT32		nLevelUpAddPoint;		// 升级增加授业点数
	INT32		nLevelUpNeedPoint;		// 升级需要授业点数
	INT32		nLevelUpNeedYuanBao;	// 升级需要消耗宝鉴数量
	INT32		nLevelUpSelfPoint;		// 升级给自身增加的勤武点数
	INT32		nLevelUpBonus;			// 升级给师傅增加的积分
};

//-----------------------------------------------------------------------------
// 同门和徒弟的同步客户端结构
//-----------------------------------------------------------------------------
struct tagMatesAndApprenticeInfo
{
	DWORD			dwRoleID;				// 玩家ID
	INT32			nLevel;					// 玩家等级
	tagDWORDTime	lastQuestTime;			// 上次师徒任务时间（天），大于7天统一发8天
	tagDWORDTime	lastOnLineTime;			// 上次上线（天），大于3天统一发4天
	bool			bIsOnLine;				// 是否在线
	bool			bReserved[3];			// 保留
};

//-----------------------------------------------------------------------------
// 师尊的同步客户端结构
//-----------------------------------------------------------------------------
struct tagMasterInfo
{
	DWORD		dwRoleID;				// 玩家ID
	INT32		nLevel;					// 等级
	DWORD		dwJingWuPoint;			// 授业点数
	DWORD		dwQinWuPoint;			// 师德点数
};


// 错误码
enum
{
	E_MA_Success							=	E_Success,		// 成功
	E_MA_Already_Has_Master					=	1,				// 您已经有师傅
	E_MA_Level_Limit						=	2,				// 等级超过50级，无需拜师
	E_MA_DestRole_Have_No_Qualification		=	3,				// 对方尚无收徒资格
	E_MA_DestRole_Apprentice_Full			=	4,				// 对方徒弟数量已满
	E_MA_Friend_Full						=	5,				// 您的好友栏位已满
	E_MA_Dest_Friend_Full					=	6,				// 对方好友栏位已满
	E_MA_Dest_Already_Has_Master			=	7,				// 对方已经有师傅
	E_MA_Dest_Level_Limit					=	8,				// 对方等级超过50级，无法收为徒弟
	E_MA_Have_No_Qualification				=	9,				// 您尚无收徒资格
	E_MA_Apprentice_Full					=	10,				// 您徒弟数量已满
	E_MA_Not_Enough_JingWuPoint				=	11,				// 授业点数不够
	E_MA_Not_Enough_BaoJian					=	12,				// 宝鉴不够
	E_MA_Not_OnLine							=	13,				// 对方不在线
	E_MA_OnLine_Day_Limit					=	14,				// 需要对方连续3日未上线
	E_MA_You_Refuse_BaiShi					=	15,				// 您拒绝了XXX的拜师请求
	E_MA_Dest_Refuse_BaiShi					=	16,				// XXX拒绝了您的拜师请求
	E_MA_You_Refuse_ShouTu					=	17,				// 您拒绝了XXX的收徒请求
	E_MA_Dest_Refuse_ShouTu					=	18,				// XXX拒绝了您的收徒请求
	E_MA_Already_Force_BreakOut				=	19,				// 您已经选择过强制脱离师门，剩余脱离等待时间约X小时，请耐心等待。

    E_MA_ErrorCode_End
};

#pragma pack(pop)
