//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_define.h
// author: twan
// actor:
// data: 2010-01-28
// last:
// brief: 城战系统定义
//-----------------------------------------------------------------------------
#pragma once
#include "msg_common_errorcode.h"
#include "city_define.h"

#pragma pack(push, 1)

//----------------------------------------------------------------
// 判断是否为城战地图（还需后续添加）
//----------------------------------------------------------------
#define		IsCityStruggleMap(dwMapID)	( (dwMapID) == TObjRef<Util>()->Crc32(_T("c03")) )

const INT32 LONGZHU_SEIZED_COUNT_DOWN		= 20;				// 龙珠被封印后城战结束的倒计时（分钟）
const INT32 CITY_DOOR_REPAIR_COUNT_DOWN		= 20;				// 城门击破后修复倒计时（分钟）
const INT32 MAX_YINYANGZHU_NUM				= 5;				// 城战阴阳柱数量
const INT32 MAX_CITY_DOOR_NUM				= 3;				// 城门数量
const INT32 NOT_REPEAT_LIMIT_ROLE_NUM		= 3;				// 增加击杀玩家数积分时要求是此前不重复的n个玩家
const INT32 MAX_CS_RANK_NUM					= 20;				// 城战战绩最大排行榜玩家数
const INT32 CITY_STRUGGLE_ACT_ID			= 70;				// 城战对应活动ID

const INT32 ADD_SCORE_LIMIT[7]				= { 60, 100, 255, 255, 30, 10, 3 };		// 增加城战积分的各种上限

// 城战时状态改变的类型
enum ECityStruggleChangeType
{
	ECSCT_PlayerSumChange,					// 城战一方人数改变
	ECSCT_LongZhuBeSealing,					// 龙珠被封印中
	ECSCT_LongZhuCancelSeal,				// 龙珠取消封印
	ECSCT_LongZhuBeSeized,					// 龙珠被另一方占领
	ECSCT_YinYangZhuSeized,					// 阴阳柱被另一方占领
	ECSCT_WuZiKu,							// 防御联盟物资库
	ECSCT_DoorDestroied,					// 城门被击破
	ECSCT_DoorRepaired,						// 城门修复
};

// 龙珠状态结构
struct tagLongZhuState
{
	ECityForceType		eForce;				// 当前占领龙珠的联盟
	BOOL				bSealing;			// 是否正在被封印
	INT32				nCountDown;			// 离城战结束倒计时（龙珠下方的倒计时）
};

// 阴阳柱状态结构
struct tagYinYangZhuState
{
	ECityForceType		eForce;				// 当前占领阴阳柱的联盟
};

// 城门状态结构
struct tagCityDoorState
{
	BOOL					bDestroied;		// 是否已被攻破
	INT32					nCountDown;		// 城门复原倒计时（分钟）
	INT32					nDoorType;		// 城门类型：1-正门，2-西门，3-东门		（此处没法用枚举，不知是系统还是vs的bug）
};

// 城战玩家同步客户端数据结构
struct tagCSRoleData2Client
{
	INT					nScore;				// 城战积分
	BYTE				byActiveTime;		// 活跃时间
	BYTE				byKills;			// 击杀次数
	BYTE				byDestroyDoor;		// 攻陷城门次数
	BYTE				byDestroyMaterial;	// 攻陷物资库次数
	BYTE				byUseRepairStone;	// 使用修复石料次数
	BYTE				byOpenYinYang;		// 开启阴阳柱次数
	BYTE				byOpenLongZhu;		// 开启龙珠次数

	tagCSRoleData2Client()
	{
		ZeroMemory(this, sizeof(*this));
	}

	tagCSRoleData2Client& operator= (tagCSRoleData2Client& other)
	{
		if (this == &other)
			return *this;

		memcpy(this, &other, sizeof(other));
		return *this;
	}
};

// 城战战绩排行信息
struct tagCSRoleRankData
{
	DWORD						dwRoleID;			// 玩家ID
	ECityForceType				eForce;				// 所属联盟
	INT32						nScore;				// 积分
	BYTE						byKills;			// 击杀次数
};

// 城战玩家数据结构
struct tagCSRoleData
{
	tagCSRoleData2Client		RoleData;			// 同步给客户端的数据
	TList<DWORD>				listKill;			// 玩家此前杀死的n个不重复玩家RoleID

	INT							nExploit;			// 帮派功勋
	INT							nContribute;		// 帮派贡献
	INT							nMoney;				// 金钱奖励

	tagCSRoleData():RoleData(), listKill(), nExploit(0), nContribute(0), nMoney(0)	{}
	
	tagCSRoleData& operator= (tagCSRoleData& other)
	{
		if (this == &other)
			return *this;

		this->RoleData = other.RoleData;
		this->listKill = other.listKill;
		this->nExploit = other.nExploit;			// 帮派功勋
		this->nContribute = other.nContribute;		// 帮派贡献
		this->nMoney = other.nMoney;				// 金钱奖励

		return *this;
	}
};

// 城战玩家数据结构枚举
enum ECityStruggleRoleData
{
	ECSRD_ActiveTime,
	ECSRD_Kills,
	ECSRD_DestroyDoor,
	ECSRD_DestroyMaterial,
	ECSRD_UseRepairStone,
	ECSRD_OpenYinYang,
	ECSRD_OpenLongZhu,
	ECSRD_GuildExploit,
	ECSRD_GuildContribute,
	ECSRD_GuildMoney,
};

// 城战结果
enum ECityStruggleResult
{
	ECSRT_None,
	ECSRT_Atk,
	ECSRT_Def,
};

#pragma pack(pop)