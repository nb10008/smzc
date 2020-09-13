#pragma once

#pragma pack(push, 1)

const INT MAX_FRIENDNUM					= 100;		// 好友最大个数
const INT MAX_BLACKLIST					= 20;		// 黑名单最倒个数
const INT MAX_FRIENDVAL					= 9999;		// 好友度最大值
const INT MAX_ENEMYNUM					= 20;		// 仇敌最大个数
const INT GIFT_STORE_TIME				= 60 * TICK_PER_SECOND;	// 礼物在管道里保存时间

struct tagFriend
{
	DWORD	dwFriendID;								// 好友ID
	DWORD	dwFriVal;								// 好友度
	BYTE	byGroup;								// 分组
	tagFriend():dwFriendID(GT_INVALID),dwFriVal(0),byGroup(1){}
};

struct tagFriendInfo
{
	DWORD	dwFriendID;								// 好友ID
	DWORD	dwFriVal;								// 好友度
	BYTE	byGroup;								// 分组
	BOOL    bOnline;								// 好友是否在线
	INT		nLevel;									// 角色等级
	INT		nVIPLevel;								// vip等级，0是无等级，1银，2金，3白金
};

struct tagEnemyInfo
{
	DWORD	dwEnemyID;								// 仇敌ID
	INT		nLevel;									// 仇敌级别
	tagEnemyInfo():dwEnemyID(GT_INVALID),nLevel(0){}
};

struct tagSendGift				
{
	DWORD	dwSrcRoleID;							// 发送者
	DWORD	dwDestRoleID;							// 接收者
	DWORD	dwTypeID;								// 礼物TypeID
	INT 	nStoreTick;								// 发送时间
	tagSendGift(DWORD dwSrcRoleID, DWORD dwDestRoleID, DWORD dwTypeID):
		dwSrcRoleID(dwSrcRoleID),dwDestRoleID(dwDestRoleID),dwTypeID(dwTypeID)
	{
		nStoreTick = GIFT_STORE_TIME;
	}
};

struct tagFriUpdate
{
	DWORD   dwRoleID;								// 好友ID
	BOOL	bOnline;								// 是否在线
	INT		nLevel;									// 等级
	INT		nVIPLevel;								// vip等级，0是无等级，1银，2金，3白金
};

// 组队增益的相关buff
//2024401	任务	任务		队伍协作	队伍人数：2，获得提升原始体力最大值2%、原始真气最大值2%的奖励。
//2024402	任务	任务		队伍协作	队伍人数：3，获得提升原始体力最大值4%、原始真气最大值4%的奖励。
//2024403	任务	任务		队伍协作	队伍人数：4，获得提升原始体力最大值6%、原始真气最大值6%的奖励。
//2024404	任务	任务		队伍协作	队伍人数：5，获得提升原始体力最大值8%、原始真气最大值8%的奖励。
//2024405	任务	任务		队伍协作	队伍人数：6，获得提升原始体力最大值10%、原始真气最大值10%的奖励。
//2024501	任务	任务		齐心协力	队伍中好友人数：2，获得2%经验加成
//2024502	任务	任务		齐心协力	队伍中好友人数：3，获得4%经验加成
//2024503	任务	任务		齐心协力	队伍中好友人数：4，获得6%经验加成
//2024504	任务	任务		齐心协力	队伍中好友人数：5，获得8%经验加成
//2024505	任务	任务		齐心协力	队伍中好友人数：6，获得10%经验加成
//2024601	任务	任务		亲密无间	队伍中好友人数：2，获得提升原始体力最大值4%、原始真气最大值4%的奖励。
//2024602	任务	任务		亲密无间	队伍中好友人数：3，获得提升原始体力最大值8%、原始真气最大值8%的奖励。
//2024603	任务	任务		亲密无间	队伍中好友人数：4，获得提升原始体力最大值12%、原始真气最大值12%的奖励。
//2024604	任务	任务		亲密无间	队伍中好友人数：5，获得提升原始体力最大值16%、原始真气最大值16%的奖励。
//2024605	任务	任务		亲密无间	队伍中好友人数：6，获得提升原始体力最大值20%、原始真气最大值20%的奖励。

#define BuffTeamProfit		90001
#define BuffTeamProfit2		9000101
#define BuffTeamProfit3		9000102
#define BuffTeamProfit4		9000103
#define BuffTeamProfit5		9000104
#define BuffTeamProfit6		9000105

#define BuffFriExpProfit	90002
#define BuffFriExpProfit2	9000201
#define BuffFriExpProfit3	9000202
#define BuffFriExpProfit4	9000203
#define BuffFriExpProfit5	9000204
#define BuffFriExpProfit6	9000205

#define BuffFriBaseProfit	90003
#define BuffFriBaseProfit2	9000301
#define BuffFriBaseProfit3	9000302
#define BuffFriBaseProfit4	9000303
#define BuffFriBaseProfit5	9000304
#define BuffFriBaseProfit6	9000305

#pragma pack(pop)