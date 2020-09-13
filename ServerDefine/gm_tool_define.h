//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_rt_c.h
// author: 
// actor:
// data: 2009-04-27
// last:
// brief: GM服务端与GM工具之间的数据定义
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

#define Notice_Title 30
#define Notice_Content 256
#define Notice_Link 256
#define System_Cmd_Length 1024


// 列举与定时有关的消息
enum ETimeLimitedMsg
{
	ETLM_Start,

	ETLM_Double = 1,
	ETLM_RightNotice,
	ETLM_AutoNotice,
	ETLM_AutoChatNotice,

	ETLM_End
};

enum 
{
	E_GMTool_Err							= -1,
	E_GMTool_Success						= 0,

	E_GMTool_GM_AlreadyExist				= 1,		// GM已经存在
	E_GMTool_GM_NotExist					= 2,		// GM不存在
	E_GMTool_GM_CreateFail					= 3,		// GM创建失败

	E_GMTool_ResumeRole_NameAlreadyExist	= 10,		// 角色名已经存在（重新起名）
	E_GMTool_ResumeRole_RoleNotExist		= 11,		// 恢复的角色不存在
	E_GMTool_ResumeRole_RoleNotReMove		= 12,		// 恢复的角色未被删除

	E_GMTool_ModifyRole_RoleNotExist		= 20,		// 角色不存在

	E_GMTool_NullContent					= 30,		// 内容不能为空

	E_GMTool_RoleNotExist					= 40,		// 角色不存在
	E_GMTool_AccountNotExist				= 41,		// 账号不存在
	E_GMTool_RoleNotOnline					= 42,		// 角色不在线	

	E_GMTool_DeleteItem_ItemNotExist		= 50,		// 物品不存在
	E_GMTool_CreateItem_EquipNumErr			= 51,		// 装备数量错误
	E_GMTool_CreateItem_ItemQualityErr		= 52,		// 物品品级错误
	E_GMTool_CreateItem_ItemTypeIDErr		= 53,		// 输入的物品ID不正确

	E_GMTool_GuildNotExist					= 60,		// 帮派不存在

	E_GMTool_Time_Err						= 100,		// 时间不对	

	E_RTClient_IpBind_Err					= 110,		// ip绑定错误
	E_RTClient_Ip_Err						= 111,		// ip绑定错误

	E_GMTool_TimeSetError					= 120,		// 时间设置错误，设置的开始时间已过

	R_RTClient_NotEnoughPri					= 130,		// 权限不够，不能查询

	E_GMTool_ResumeItemNotExit				= 140,		// 要恢复的装备不存在
	E_GMTool_ResumeItemNotEquip				= 141,		// 要恢复的物品不是装备

	E_GMTool_MoveRole_MapNameErr			= 150,		// 地图名错误
	E_GMTool_MoveRole_CoordinateErr			= 151,		// 坐标不合法
    E_GMTool_MoveRole_RoleOffLine           = 152,      // 玩家离线 

	E_GMTool_AddPKLog_RoleOffline			= 155,		// 角色不在线
	E_GMTool_AddPKLog_RoleInvalid			= 156,		// 角色指定的不对

	E_GMTool_FilterWords_Existed			= 160,		// 过滤词已经存在
	E_GMTool_FilterWords_NotExisted			= 161,		// 过滤词不存在

	E_GMTool_LoginDBLost	= 162,	// login数据库连接失败
};

enum EDoubleType
{
	EDoubleType_NULL		= -1,
	EDoubleType_Exp			= 0,				//双倍经验
	EDoubleType_Item		= 1,				//双倍物品
	EDoubleType_End			= 2,
};

const INT NUM_EDT			= EDoubleType_End - EDoubleType_Exp;
#define EDT_VALID(edt)	((edt) > EDoubleType_NULL && (edt) < EDoubleType_End)	


//马跑灯/自动循环公告颜色设置
enum EColorType
{
	EColorType_NULL			= -1,
	EColorType_Red			= 0,
	EColorType_Yellow		= 1,
	EColorType_Green		= 2,	
	EColorType_Blue			= 3,
	EColorType_Orange		= 4,
	EColorType_End			= 5,
};

//Log结构
enum EActionType
{
	EAT_Null,

	EAT_CreateGM = 1,			// 线下GM创建
	EAT_ModifyGM,				// 线下GM修改
	EAT_DeleteGM,				// 线下GM删除

	EAT_ResumeRole,				// 角色恢复
	EAT_ProModify,				// 属性修改

	EAT_ItemCreate,				// 道具生成
	EAT_ItemDelete,				// 道具删除
	EAT_ItemResume,				// 道具恢复

	EAT_DoubleStart,			// 开放双倍
	EAT_DoubleEnd,				// 双倍删除

	EAT_AutoNoticeStart,		// 自动循环公告
	EAT_AutoNoticeEnd,			// 自动循环公告删除

	EAT_NoticeStart,			// 右下角公告
	EAT_NoticeEnd,				// 右下角公告删除

	EAT_AutoChatStart,			// 聊天频道公告
	EAT_AutoChatEnd,			// 聊天频道公告删除

	EAT_StartServer,			// 开启服务器
	EAT_CloseServer,			// 关闭服务器
	EAT_KillServer,				// 强制关闭服务器

	EAT_IpBind,					// 绑定IP

	EAT_SendSystemCmd,			// 发送系统消息

	EAT_AccountBanned,			// 账号封停
	EAT_UndoAccountBanned,		// 账号解封

	EAT_End
};
#pragma pack(pop)