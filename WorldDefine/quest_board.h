//--------------------------------------------------------------------------------
//!\file quest_board.h
//!\author zhangrong
//!
//!\date 2010-06
//! last 2010-06
//!
//!\brief 任务板头文件
//!
//--------------------------------------------------------------------------------
#pragma once

#define QUEST_BOARD_MIN_CAPACITY	2				// 任务板可显示的最小任务数
#define QUEST_BOARD_MAX_CAPACITY	6				// 任务板可显示的最大任务数
#define BOARD_QUEST_LIMIT_LEVEL		10				// 接取任务板任务的最低等级
#define GUILD_QUEST_LIMIT_LEVEL		40				// 接取公会任务板任务的最低等级
#define YELLOW_QUEST_LIMIT_LEVEL	50				// 接取天之行会任务板任务的最低等级
const UINT16 QUEST_PANEL_START_TASK_ID	= 10001;	// 任务板起始任务ID
const UINT16 QUEST_PANEL_END_TASK_ID	= 10012;	// 任务板截止任务ID
const UINT16 YELLOW_LIST_START_TASK_ID = 10007;	// 天之行会任务板起始任务ID
const DWORD REFRESH_QUEST_PANEL_ITEM_ID1 = 4500002;	// 悬赏令
const DWORD REFRESH_QUEST_PANEL_ITEM_ID2 = 3070056;	// 绑定悬赏令
const DWORD REFRESH_QUEST_PANEL_FIRST_ID = 3070056;

#define BOARD_QUEST_INVEST_NUM		4
#define BOARD_QUEST_MONSTER_NUM		4
#define BOARD_QUEST_NPC_NUM			2
#define BOARD_QUEST_GATHER_NUM		3
#define BOARD_QUEST_REWARD_NUM		5

#define YELLOWLIST_ACCEPT_TIMES_LIMIT 10		// 天之行会任务榜每天只能接取10次
#define RewardBoardQuestLvlIdx					30   //--记录玩家身上悬赏榜的等级
#define GuildBoardQuestLvlIdx						31   //--记录玩家身上公会任务板的等级                             
#define YellowListBoardQuestLvlIdx				32   //--记录玩家身上天之行会任务板的等级                      
#define TodayYellowListQuestTimesIdx		33   //--记录玩家当天接取天之行会任务板任务的次数   

// 任务板任务品级枚举
enum EBoardQuestQuality
{
	EBQQ_White		= 0,	// 白
	EBQQ_Yellow		= 1,	// 黄
	EBQQ_Green		= 2,	// 绿
	EBQQ_Blue		= 3,	// 蓝
	EBQQ_Orange		= 4,	// 橙
	EBQQ_Purple		= 5,	// 紫
};

#pragma pack(push)
#pragma pack(1)
// 任务状态
enum EBoardQuestState
{
	EQS_Null		= -1,

	EQS_NotAccept	= 0,	// 未接取
	EQS_HaveAccept	= 1,	// 已接取
	
	EQS_End			= 2,
};

// 数据库存储结构
struct tagBoardQuestSaveData
{
	DWORD		dwRoleID;
	UINT16		u16QuestID;
	UINT16		u16BoardID;
	BYTE			byType;
	EBoardQuestState	eState;
};
#pragma pack(pop)

struct tagBoardQuestTargetMonster
{
	DWORD		dwTargetMonsterID_A;
	DWORD		dwTargetMonsterID_B;
	INT			byTargetMonsterNum;
};

struct tagBoardQuestObject
{
	DWORD		dwID;
	INT			byNum;
};

struct tagBoardQuestReward
{
	DWORD		dwID;
	BYTE		byNum;
	BYTE		byQuality;
};

// 任务板中任务的属性结构
struct tagBoardQuestProto
{
	UINT16						u16ID;			// 任务ID
	BYTE						byType;			// 任务类型
	EQuestTargetType			eTarget;		// 任务目标类型
	BYTE						byLevel;		// 任务等级
	EBoardQuestQuality			eQuality;		// 任务品级

	// 任务需调查地物
	DWORD						dwInvestItem[BOARD_QUEST_INVEST_NUM];

	// 任务需击杀怪物
	tagBoardQuestTargetMonster	TargetMonster[BOARD_QUEST_MONSTER_NUM];

	// 任务需对话NPC
	DWORD						dwTalkToNPC[BOARD_QUEST_NPC_NUM];

	// 任务需采集资源
	tagBoardQuestObject			GatherItem[BOARD_QUEST_GATHER_NUM];
	
	// 任务奖励物品
	tagBoardQuestReward			RewardItem[BOARD_QUEST_REWARD_NUM];

	// 任务奖励经验
	INT							nRewardExp;

	// 任务奖励金钱
	INT							nRewardMoney;
};