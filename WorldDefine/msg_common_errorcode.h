#pragma once

//------------------------------------------------------------------------
// 所有网络消息共同的错误码
// 
// 除了E_Success，所有错误码均为负数，各个消息独有的错误码可从1开始
//------------------------------------------------------------------------
enum
{
	E_Success						= 0,
	E_SystemError					= -1,

	E_Item_NotFound					= -2,		// 没有找到物品
	E_Item_NotEquipment				= -3,		// 不是装备

	E_Item_CanNotAdd				= -4,		// 不能添加到指定的容器中
	E_Item_CanNotRemove				= -5,		// 不能从容器删除物品
	E_Item_CanNotMove_Other			= -6,		// 不能移动到其他容器中
	E_Item_AddFailed				= -7,		// 向容器中插入时失败

	E_Item_Place_NotFree			= -8,		// 目标位置不为空

	E_Item_LevelLimit				= -9,		// 等级不满足要求
	E_Item_SexLimit					= -10,		// 性别不满足要求
	E_Item_ClassLimit				= -11,		// 职业不满足要求
	E_Item_TalentLimit				= -12,		// 天资不满足要求
	E_Item_ClanRepLimit				= -13,		// 氏族声望不满足要求
	E_Item_OtherClanRepLimit		= -14,		// 其他声望不满足要求
	E_Item_RoleStatusLimit			= -15,		// 玩家状态(战斗，非战斗)不满足要求
	E_Item_AttA_Limit				= -16,		// 一级属性不满足

	E_Item_NotEnough				= -20,		// 物品个数不够

	E_Item_MaxHold					= -25,		// 物品达到可拥有上限

	E_Item_CountLimit				= -26,		// 稀有物品由于全局数量的限制而不能拾取

	E_Holy_CountLimit				= -27,		// 人物身上圣灵数量限制，不能再获得

	E_Item_Used_NotValid			= -30,		// 使用的物品非法

	E_Cannot_FindMap				= -31,		// 无法找到地图
	E_Area_Limitted					= -32,		// 限制区域，不可进入之类
	E_Role_Status_Limitted			= -33,		// 角色状态限制
	E_Role_Buff_Limit					= -34,		// 角色身上buff限制

	E_Pet_RecallWithoutItem			= -35,		// 没有找到回收宠物需要的道具
	E_Pet_RecallDeleteErr			= -36,		// 删除宠物失败

	E_Bag_NotEnoughSpace			= -100,		// 背包没有足够的空间
	E_Con_NotEnoughSpace			= -101,		// 没有足够的空间
	E_Con_PswNotPass				= -102,		// 未输入过行囊密码
	E_Ware_MoveLimit				= -103,		// 该物品不能存入仓库
	E_GuildWare_MoveLimit			= -104,		// 该物品不能放入帮派仓库

	E_BagYuanBao_NotEnough			= -200,		// 背包中元宝不够
	E_BagSilver_NotEnough			= -201,		// 背包中金钱不够
	E_ExVolume_NotEnough			= -202,		// 角色赠点不够
	E_VipPoint_NotEnough            = -203,     // 角色积分不够　

	E_Role_Not_ExistIn_World		= -300,		// 游戏世界中不存在指定角色
	E_Role_Not_Online				= -301,		// 角色不在线
	E_Role_CantMove					= -302,		// 角色处于不可以移动状态

	E_NPC_NotFound					= -310,		// 没有找到指定NPC
	E_NPC_NotValid					= -311,		// 职能类型不对
	E_NPC_TooFar					= -312,		// 距离NPC太远

	E_Filter_Name_InFilterFile		= -351,		// 在过滤词表中
	E_Filter_Name_TooLong			= -352,		// 名字太长
	E_Filter_Name_Null				= -353,		// 名字为空
	E_Filter_Name_Illegal			= -354,		// 名字包括特殊字符
	E_Filter_Chat_NoContent			= -355,		// 聊天内容为空
	E_Filter_Text_TooLong			= -356,		// 内容太长
	E_Filter_Name_TooShort			= -357,		// 名字太短

	E_Opt_Timeout					=	-400,	// 操作超时
	E_Opt_Fail						= -401,		// 操作失败

	E_Promotion_Clergy_Type			= -410,		// 神职晋升方式错误
	E_Promotion_Clergy				= -411,		// 神职晋升资格未满足
	E_Enroll_Clergy_Limited			= -412,		// 神职竞选报名，已经报名
	E_God_Area_None					= -413,		// 您还未拥有该领域能力，无法激活	
	E_God_Area_Max					= -414,		// 您所激活的自选领域与稀有领域之合已经到达数量上限，无法激活新的领域能力
	E_God_Area_Condition			= -415,		// 您未满足该领域的激活条件，无法激活
	E_Enroll_Clergy_LevelLimited	= -416,		// 神职竞选报名，等级不够
	E_Enroll_Clergy_SexLimited		= -417,		// 神职竞选报名，性别不符
	E_Enroll_Clergy_ClassLimited	= -418,		// 神职竞选报名，职业不符
	E_Enroll_Clergy_GodheadLimited	= -419,		// 神职竞选报名，神格不够
	E_Enroll_Clergy_AttValueLimited	= -420,		// 神职竞选报名，相关属性值不够
	E_God_Area_CD					= -421,		// CD时间未到
	E_God_Area_FaithValue			= -422,		// 信仰不足
	E_KeyCode_CodeInvalid		= -423,		// 无效的key码
	E_KeyCode_NPCInvalid		= -424,		// NPC无效
	E_KeyCode_OnlyOnce			= -425,		// 只能领取一次
	E_God_Area_Career		= -426,			//职业不符，不可激活
	E_Offline_Role_Sprite		= -427,			//离线后的角色家族妖精

	E_NOT_GET_GIFT				= -428,			//不符合领取礼包条件

	E_TimeEquipLimit			= -429,		//穿戴限时装备不可晋升

	E_Item_EquipBar_Error = -500, // 装备栏物品错位

	E_Call_Holy_NotTakeoff = -501, // 召唤圣灵不能被脱下

	E_Return_NotNeedFeed			= -10000,	// 不用向客户端反馈 -- 该值为服务器反馈客户端消息用
};