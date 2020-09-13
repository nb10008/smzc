//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: log_cmdid_define.h
// author: 
// actor:
// data: 2008-11-11
// last:
// brief: 服务器记录log用的网络命令代码枚举 -- 方便查找
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)
//-----------------------------------------------------------------------------
// log用网络消息枚举 -- 一旦定义了最好不要修改(1.数据库已有相应记录；2.脚本可能已写死)
//-----------------------------------------------------------------------------
enum ELogCmdID
{
	ELCID_Null					= 0,	// 无

	// 商店相关
	ELCID_Shop_BuyItem			= 1,	// 买物品(非装备)
	ELCID_Shop_BuyEquip			= 2,	// 买装备
	ELCID_Shop_Sell				= 3,	// 卖物品&装备

	// PK保护相关
	ELCID_Open_SafeGuard		= 11,	// 开启PK保护

	// 死亡惩罚
	ELCID_Dead_Penalty			= 12,	// PK惩罚

	// 百宝袋相关
	ELCID_BaiBao_LoadFromDB		= 20,	// 从item_baibao表中读取的物品
	ELCID_BaiBao_Bill_YuanBao	= 21,	// 元宝直冲

	// 行囊相关
	ELCID_Bag_PosOverlap		= 30,	// 初始化角色背包时，发现物品位置重叠

	// 洗点
	ELCID_Clear_Att				= 40,	// 洗属性点
	ELCID_Clear_Talent			= 41,	// 洗天资点
    ELCID_Buy_Talent            = 42,   // 购买天资点

	// 角色仓库相关
	ELCID_RoleWare_PosOverlap	= 50,	// 初始化角色仓库时，发现物品位置重叠
	ELCID_RoleWare_SaveSilver	= 51,	// 存钱
	//ELCID_RoleWare_SaveYuanBao	= 52,	// 存元宝
	ELCID_RoleWare_GetSilver	= 53,	// 取钱
	ELCID_RoleWare_GetYuanBao	= 54,	// 取元宝

	
	// 商城
	ELCID_Mall_CartBuy			= 59,	// 使用购物车购买物品
	ELCID_Mall_BuyItem			= 60,	// 购买商城物品
	ELCID_Mall_BuyPack			= 61,	// 购买商城礼品包
	ELCID_Mall_FreeItem			= 62,	// 领取商城免费物品
	ELCID_Mall_BuyItem_Add		= 63,	// 购买商城物品时附带
	ELCID_Mall_BuyPack_Add		= 64,	// 购买商城礼品包时附带
	ELCID_Mall_PresentItem		= 65,	// 赠送商城物品
	ELCID_Mall_PresentPack		= 66,	// 赠送商城礼品包
	ELCID_Mall_PresentItem_Add	= 67,	// 赠送商城物品时附带
	ELCID_Mall_PresentPack_Add	= 68,	// 赠送商城礼品包时附带
	ELCID_MALL_Activity_Add		= 69,	// 商城活动赠送

	// 元宝交易
	ELCID_Trade_SaveYuanBao		= 70,	// 向交易账户存元宝
	ELCID_Trade_SaveSilver		= 71,	// 向交易账户存金钱
	ELCID_Trade_DepositYuanBao  = 72,	// 从交易账户取出元宝
	ELCID_Trade_DepositSilver	= 73,	// 从交易账户取出金钱
	ELCID_Trade_Tax				= 74,	// 提交订单时扣除手续费

	// 商城 - 续
	ELCLD_Mall_ExchangeItem		= 80,	// 兑换商城物品
	ELCLD_Mall_ExchangePack		= 81,	// 兑换商城礼品包
	ELCLD_Mall_ExchangeItem_Add	= 82,	// 兑换商城物品时附带
	ELCLD_Mall_ExchangePack_Add	= 83,	// 兑换商城礼品包时附带
	// Jason 2010-3-16 v1.5.0 脚本增加赠点接口
	ELCLD_MALL_LUA_AddGiftPoint	= 84,	// 脚本接口，用于每周领取赠点活动。
	ELCLD_MALL_GiftPointCosted	= 85,	// 赠点消耗

	// 团购
	ELCID_GroupPurchase_Faild		= 90,	// 团购失败返还所得
	ELCLD_GroupPurchase_BuyItem		= 91,	// 团购购买商城物品
	ELCLD_GroupPurchase_BuyItem_Add	= 92,	// 团购商城物品附带

	// 任务相关
	ELCID_Quest_Loot			= 100,	// 任务掉落
	ELCID_Quest_Complete		= 101,	// 完成任务
	ELCID_Quest_Accept			= 102,	// 接取任务
	ELCID_Quest_Discard			= 103,	// 放弃任务
	ELCID_Quest_rewards			= 104,  // 任务奖励
	ELCID_FamilyQuest_rewards	= 105,	// 家族任务奖励
	ELCID_FamilyQuest_GiveInItem = 106,// 家族任务上缴物品

	// 装备强化相关
	ELCLD_Compose_Spin			= 149,	// 精纺
	ELCID_Compose_Posy			= 150,	// 铭文
	ELCID_Compose_Engrave		= 151,	// 镌刻
	ELCID_Compose_Enchase		= 152,	// 镶嵌
	ELCID_Compose_Brand			= 153,	// 烙印
	ELCID_Compose_LongFu		= 154,	// 龙附
	ELCID_Compose_Produce		= 155,	// 合成
	ELCID_Compose_Quench		= 156,	// 淬火
	ELCID_Compose_Decompose		= 157,	// 点化,装备分解
	ELCID_Compose_Chisel		= 158,	// 开凿
	ELCID_Compose_Dye			= 159,	// 染色

	// 玩家间交易
	ELCID_Exchange_Verify		= 160,	// 交易确认
	// Jason 没有位置了，暂时放这里，2009-12-6
	ELCID_Raise_PotVal			= 165,	// 没有空隙了，看看怎么处理啊，郁闷
	ELCID_LessingLoong			= 166,	// 神龙赐福
	ELCID_PhilosophersStone		= 167,	// 点金石
	// 特殊潜力提升，不需要消耗潜力提升次数
	ELCID_Spec_Raise_Potval		= 168,
	// 神石洗装备
	ELCID_RockStone				= 169,

	// 战场参战奖励
	ELCID_CapPointRewards = 170, // 战场占领分数奖励
	ELCID_ActiveTimeRewards = 171, // 活跃时间奖励

	// 原地复活,完美复活
	ELCLD_Revive_Locus			= 180,
	ELCLD_Revive_Perfect		= 181,

	// 摆摊
	ELCLD_Stall_BeSold			= 190,	// 摆摊卖出
	ELCLD_Stall_Buy				= 191,	// 从摊位上购买

	// 换场景
	ELCLD_Dak					= 200,

	// 物品
	ELCLD_Item_Use				= 300,	// 物品使用
	ELCLD_Item_Move				= 301,	// 同一容器内移动
	ELCLD_Item_Move2Other		= 302,	// 不同容器间移动
	// Jason 2010-5-14 v2.1.0 土地庙消耗物品
	ELCLD_Item_CostedByEarthTemple	= 303,	// 土地庙消耗道具
	ELCLD_Item_UseVipCard	= 304,	// 使用VIP身份道具

	ELCLD_Item_AtTerm			= 310,	// 有时限物品，到期删除
	ELCLD_Item_EquipHoly		= 311,	// 圣灵穿装备
	ELCLD_Item_UnEquipHoly		= 312,	// 圣灵脱装备

	// 装备
	ELCLD_Equip_Identify		= 350,	// 装备鉴定
	ELCLD_Equip_ReGet			= 351,	// 装备恢复
	
	// 新手
	ELCD_1stOnline_Item			= 380,	// 新手发放

	// 玩家吃从背包扔掉物品
	ELCLD_Role_Discard_Item		= 401,

	// 玩家拾取物品
	ELCLD_PickUp_Item			= 402,
	ELCLD_PickUp_Money			= 403,

	// 给好友送礼物
	ELCLD_Send_Gift				= 410,

	// 礼品包
	ELCLD_Gift_Bag				= 420,

	// 职能NPC相关
	ELCLD_Bag_Extend			= 450,	// 扩充背包
	ELCLD_RoleWare_Extend		= 451,	// 角色仓库扩充
	ELCLD_CureInjure			= 452,	// npc治疗内伤
	
	// 普通掉落
	ELCID_Loot					= 500,	// 普通掉落

	// 帮派相关
	ELCLD_Guild_Create			= 600,	// 创建帮派
	ELCLD_Guild_Upgrade			= 601,	// 帮派设施升级
	ELCLD_Guild_SpreadAffair	= 602,	// 发布帮务
	ELCLD_Guild_DailyCost		= 603,	// 日常消耗
	ELCLD_Guild_SkillResearch	= 604,	// 帮派技能升级
	ELCLD_Guild_SkillLearn		= 605,	// 学习帮派技能
	ELCLD_Guild_Commerce		= 606,	// 跑商
	ELCLD_Guild_Reputation		= 607,	// 帮派声望变更
	ELCID_Guild_ChangeName      = 608,  // 帮派改名
	ELCID_Guild_DeclareWar      = 609,  // 宣战
	ELCID_Guild_BattleField_SignUp  = 610,  // 战场报名消耗
	ELCLD_Guild_Skill_Active	= 611,	// 公会技能激活
	ELCLD_Guild_BattleField_Hold = 612, //领战场公会的公会资金每日22:00额外产出

	// 聊天相关
	ELCLD_CHAT_WORLD_CHANNEL		= 700,	//在世界频道发言
	ELCLD_SHOW_EQUIP_WORLD_CHANNEL	= 701,	//在世界频道展示装备
	ELCLD_SHOW_ITEM_WORLD_CHANNEL	= 702,	//在世界频道展示物品
	ELCLD_CAST_TALK_IM				= 703,	//传音使用IM道具(绑定)
    ELCLD_WORLD_TALK_IM             = 704,  //世界使用IM道具
    ELCLD_WORLD_TALK_IM_UNBIND      = 705,  //世界使用IM道具(非绑定)
	ELCLD_CAST_TALK_IM_UNBIND		= 706,	//传音使用IM道具(非绑定)
	ELCLD_WORLD_TALK_IM_NETBAR		= 707,	//世界聊天使用网吧喇叭

	// 宠物相关
	ELCLD_PET_ITEM_CREATE_PET		= 800,	// 生成宠物
	ELCLD_PET_ITEM_FEED_PET			= 801,	// 宠物喂食
	ELCLD_PET_ITEM_PET_LEARNSKILL	= 802,	// 宠物学习技能
	ELCLD_PET_ITEM_PET_REVIVE		= 803,	// 宠物复活
	ELCLD_PET_ITEM_PET_EQUIP		= 804,	// 宠物装备
	ELCLD_RolePourExpPet			= 805,	// 灌注经验
	ELCLD_PET_ITEM_PET_Enhance		= 806,	// 宠物装备
	ELCLD_PET_ITEM_PET_Lock			= 807,	// 宠物锁定
	ELCLD_PET_ITEM_PET_UnLock		= 808,	// 宠物解锁
	ELCLD_PET_ITEM_PET_Food			= 809,	// 宠物食物
	ELCLD_PET_SKILL_SALE			= 810,	// 宠物小贩技能
	
	ELCLD_PET_CREATE_PET			= 811,	// 创建宠物
	ELCLD_PET_GAIN_PET				= 812,	// 获得宠物
	ELCLD_PET_DEL_PET_CREATE		= 813,	// 删除宠物（创建失败）
	ELCLD_PET_DEL_PET_INIT			= 814,	// 删除宠物（初始化失败）
	ELCLD_PET_DEL_PET_CLIENT		= 815,	// 删除宠物（客户端命令）
	ELCLD_PET_LOSE_PET				= 816,	// 失去宠物
	// Jason v1.3.2 2010-1-31 扩充宠物背包消耗
	ELCLD_PET_PocketExtend_Costed	= 817,	// 
	ELCLD_PET_RecallPet				= 818,	//回收宠物
	// 妖精元素伤害类型转换
	ELCLD_Fabao_EleTypeChange_Costed = 850,	// 妖精转换元素伤害类型消耗
	ELCLD_Fabao_IncEleAbility_Costed		= 851,  // 提升妖精能力
	ELCLD_Fabao_StuntBook					= 852,	// 妖精特技书消耗
	// 妖精重生消耗物品
	ELCLD_Fabao_Nirvana					= 853,	// 妖精重生消耗
	ELCLD_Fabao_Birth						= 854,	// 妖精繁殖
	ELCLD_Fabao_Fusion				= 855,	// 妖精融合消耗 [3/22/2012 zhangzhihua]
	ELCLD_FairyAddPromoteTimes		= 856,	//妖精提升属性

	// 氏族相关
	ELCLD_ACT_TREASURE				= 900,	// 激活氏族珍宝获得

	//固定活动相关
	ELCLD_WU_SE_SHI					= 1000, // 固定活动删除五色石
	ELCLD_PVP						= 1001, // pvp固定活动
	ELCLD_LEAVEWAR_DEL				= 1002, // 退出战场的背包清理

	// VIP摊位相关
	ELCLD_VIP_Stall_Rent			= 1100,	// VIP摊位租赁

	// 金牌网吧
	ELCLD_VIP_NetBar				= 1200,	// 金牌网吧

	// 技能
	ELCLD_Skill_Use					= 1300, // 技能

	// 修炼仙界技能
	ELCLD_PracticeM_Skill			= 1350, // 练习仙界技能
	ELCID_PracticeSoaringSkill		= 1351,	// 联系飞升技能消耗

	// 职业相关
	ELCLD_Class_Change				= 1400, // 转职
	ELCLD_Class_Get					= 1401, // 就职

	// 城市系统
	ELCLD_City_Produce				= 1301,	// 城市产出
	ELCLD_City_Prolificacy			= 1302,	// 提升城市生产力
	ELCLD_City_TransformTaxation	= 1303,	// 提取税金
	ELCLD_City_SignUp				= 1304,	// 城战报名
	ELCLD_City_Examine				= 1305,	// 查看城市信息

	// 精力系统
	ELCLD_Spirit_Reward				= 1400, // 精力系统奖励道具
	ELCLD_Strength_Reward			= 1401, // 精力系统奖励道具-实力

	// 其他相关在此处之上加
	ELCID_GemRemoval				= 1500,	// 宝石拆除

	// Jason v1.3.2 2010-1-20 玩家离线经验领取
	ELCID_PlayerOfflineReward		= 1501,
	// 飞升炼器
	ELCID_Soaring_BuildEquip		= 1502,	// 飞升炼器标志
	ELCID_Casting_SoaringEquip		= 1503, // 熔铸已经飞升装备

	ELCID_GuildDailyReward		= 1504,	// 公会战，每日在线奖励
	
	// Jason 2010-4-16 v2.0.0 法宝相关
	ELCID_Fabao_RemoveYuLingzhu		= 1550,	// 法宝成长消耗御灵珠
	ELCID_Fabao_RemoveLingzhu		= 1551,	// 法宝灵槽契合，消耗灵珠
	ELCID_Fabao_ReversalCosted		= 1552,	// 法宝五行逆转消耗辅助法宝
	ELCID_Fabao_MergingCosted		= 1553, // 法宝五行相生重置消耗
	ELCID_Fabao_FairyPowderCosted	= 1554, // 妖精之粉消耗
	ELCID_GodStrengthen_Costed		= 1555, // 神铸消耗
	ELCID_Fabao_Feeding				= 1556,	// 妖精喂食消耗
	ELCID_Fabao_StoreExp_Costed		= 1557,	// 妖精经验封印消耗
	ELCID_Fabao_StoreExp_Created		= 1558,	// 妖精经验封印生成

	// 师徒
	ELCLD_Apprentice_UseBaoJian		= 1600,	// 徒弟使用宝鉴升级
    
	// 选秀
	ELCLD_VoteShow_UseNecklace      = 1700, // 选秀消耗珍珠项链

	// 易容
	ELCLD_Appearance_Transform		= 1800, // 易容消耗道具

	ELCID_FCompose_Consolidate		= 1850,	// F计划，强化消耗
	ELCID_FCompose_Xinhun				= 1851,	// F计划，吸魂
	ELCID_FCompose_Fuhun				= 1852,	// F计划，附魂

	// 妖精契约型消耗
	ELCID_Fairy_Costed						= 1900,	// 圣魔，妖精契约消耗

	// 神之竞标
	ELCID_Godbid_NormalBid_Cost = 1910,			// 普通竞标消耗
	ELCID_Godbid_NormalBid_Added = 1911,		// 普通竞标
	ELCID_Godbid_BlackBid_Cost = 1912,				// 黑市竞标消耗
	ELCID_Godbid_BlackBid_Added = 1913,			// 黑市竞标

	// 砸金蛋
	ELCID_Egg_Broker_Cost = 1914,
	ELCID_Egg_Broker_Added = 1915,
	
	// 高级合成
	ELCID_Advanced_Composed_Cost = 1916,
	ELCID_Advanced_Composed_Add = 1917,

	// 衣橱
	ELCID_Wardrobe_Layer_LevelUp_Cost = 1918,

	// 家族任务
	ELCID_FamilyQuest_GetReward = 1919,

	ELCLD_Family_Sprite_Train		= 1920,		// 家族妖精修炼
	ELCID_Family_Sprite_Train_Item	= 1921,		// 妖精修炼赠品

	// Key码领奖励
	ELCID_KeyCode_Reward = 1922,

	ELCID_Compose_Extract	= 1923,	// 萃取

	ELCLD_Family_Role_Sprite = 1924,		//家族妖精登记返还

	ELCID_Family_Quest_GiveInItem = 1925, // 上缴家族任务物品

	ELCID_Marrige = 1926, // 结婚

	ELCID_Zhanghao_Gift		= 1927,	// 账号回购礼包

	// 结拜
	ELCID_BloodBrother = 1928,

	ELCID_PulseSkill		= 1929, // 经脉

	// 装备成长
	ELCID_EquipGrow = 1940,
	ELCID_EquipGrowTrans = 1941, // 装备成长继承

	// 老虎机
	ELCID_Tiger = 1942,

	// 激活神魔技能
	ELCID_SOUL_SKILL = 1943,

	// 神魔技能续约
	ELCID_SOUL_CONTINUE_SKILL = 1944,

	// 神魔技能续约物品消耗
	ELCID_SOUL_CONTINUE_ITEM = 1945, 

	// 魂晶
	ELCID_SoulCrystal_Grow = 1950, // 魂晶升级
	ELCID_SoulCrystal_Renew = 1951, // 魂晶重置

	// 刷成长属性或者宝石增幅
	ELCID_EquipGrowIDOrDiamondAdd = 1960,	

	// 新key码
	ELCID_NewKeyCode	= 1970,

	// 保存手机号
	ELCID_SaveTelNum	= 1971,

	//元神强化
	ELCLD_StarEnhanceConsolidate		=   2000,//元神装备冲星强化
	ELCLD_HolySoulReborn				=	2100,//元神转生

	ELCLD_HolySoulEquipHole				=	2110,//元神装备凿孔
	ELCLD_HolySoulEquipInlay			=	2111,//元神装备镶嵌
	ELCLD_HolySoulEquipRemove			=	2112,//元神装备摘除
	ELCLD_HolyEatFairy					=	2113,//圣灵吞噬妖精

	ELCLD_UnEquipHoly					=	2120,//圣灵洗印消耗金币
	ELCID_ShengWenChaiChu				=	2121,// 圣灵洗印扣除洗印符

	ELCLD_EnhanceHolyEquip				=	2122,//充能圣纹消耗金币
	ELCID_DelEnhanceItem				=	2123,//充能圣纹扣除充能道具
	ELCID_DelPianXiangItem				=	2123,//充能圣纹扣除偏向道具

	ELCID_EquipPerfectGrow				=	2130,//完美成长消耗
	ELCID_EquipXiStar					=	2131,//吸星大法
	ELCID_WarReward						=	2132,//战场赠品
	ELCID_WarCost						=	2133,//战场消耗

	ELCID_HolyStrength					=	2140,//提升吞噬妖精次数
	ELCID_HolyEquipmentNumber			=	2141,//提升圣灵可装备圣纹数量

	ELCID_MallDaiBi						=	2150,//商城代币购买东西消耗
	ELCID_YuanBaoDaiBi					=   2151,//NPC领取代币

	// GM命令
	ELCID_GM_ClearBag				= 1000000,	// 清理背包
	ELCID_GM_CreateItem				= 1000001,	// 创建物品/装备/时装
	ELCID_GM_GetMoney				= 1000002,	// 获得金钱
	ELCID_GM_GetYuanBao				= 1000003,	// 获得元宝
	ELCID_GM_GetExVolume			= 1000004,	// 获得赠点
	ELCID_GM_GetFund				= 1000005,	// 获得帮派资金
	ELCID_GM_GetMaterial			= 1000006,	// 获得帮派资材
	ELCID_GM_GetContribute			= 1000007,	// 获得帮派个人贡献
    ELCID_GM_GetVIPPoint            = 1000008,  // 获得积分 
};

// 元神装备强化类型
enum EHolyEquipConsolidateType
{
	EHECT_Null			= 0,	//
	EHECT_StarEnhance	= 1,	// 冲星强化
	EHECT_EquipHole		= 2,	// 凿孔强化
	EHECT_EquipInlay	= 3,	// 镶嵌强化

};

//-----------------------------------------------------------------------------
// log用数据结构
//-----------------------------------------------------------------------------
struct tagLogSilver				// 金钱变化
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwCmdID;
	DWORD	dwRoleIDRel;
	INT64	n64Silver;			// +：获得；-：失去
	INT64	n64TotalSilver;		// 对应容器中总量
	INT8	n8LogConType;		// 背包，仓库
};

struct tagLogYuanBao			// 元宝变化
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwCmdID;
	INT		nYuanBao;			// +：获得；-：失去
	INT		nTotalYuanBao;		// 对应容器中总量
	INT8	n8LogConType;		// 背包，百宝袋
};

struct tagLogExVolume			// 赠点变化
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwCmdID;
	INT		nExVolume;			// +：获得；-：失去
	INT		nTotalExVolume;		// 对应容器中总量
};

struct tagLogVIPPoint			// 积分变化
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwCmdID;
	INT		nVIPPoint;			// +：获得；-：失去
	INT		nTotalVIPPoint;		// 对应容器中总量
};

struct tagLogGodBid			// 神之竞标
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	DWORD	dwItemID;
	BYTE		byMoneyType;
	DWORD	dwMoney;
};

struct tagLogBattlefield			// 战场占领记录
{
	DWORD	dwGuildID;															// '公会ID'
	TCHAR		szGuildName[24];			// 帮派名称
	BYTE		byBattlefield;														// '战场枚举，0--天神冢 1--七花之海 2--奔雷裂谷',
};

struct tagLogTimeStat			// 在线玩家定时统计
{
	DWORD	dwAccountID;
	DWORD	dwRoleID;
	INT64	n64BagSilver;
	INT64	n64WareSilver;
	INT		nBagYuanBao;
	INT		nBaiBaoYuanBao;
	INT		nExVolume;
	INT     nVIPPoint;
};

struct tagLogItem				// 物品log结构
{
	DWORD		dwRoleID;
	DWORD		dwTypeID;

	INT64		n64Serial1;
	INT64		n64Serial2;
	INT8		n8ConType1;
	INT8		n8ConType2;
	INT16		n16ResNum1;
	INT16		n16ResNum2;

	INT16		n16OptNum;		// 变化个数(+：表示获得；-：表示失去)
	DWORD		dwCmdID;
	DWORD		dwRoleIDRel;
};

struct tagLogMallSell			// 商城出售物品log结构
{
	INT64		n64Serial;
	DWORD		dwTypeID;
	DWORD		dwAccountID;	// 购买者账号ID
	DWORD		dwRoleID;		// 购买者
	DWORD		dwToRoleID;		// 获得物品者
	DWORD		dwFstGainTime;
	DWORD		dwExistTime;
	INT			nMaxUseTimes;
	INT			nCostYuanBao;
	INT			nCostExVolume;
	INT         nCostVIPPoint;
	DWORD		dwCmdID;
	INT16		n16NumSell;
};

struct tagLogMallSellPack		// 商城出售礼品包log结构
{
	DWORD		dwPackID;
	DWORD		dwAccountID;	// 购买者账号ID
	DWORD		dwBuyRoleID;
	DWORD		dwToRoleID;
	INT			nCostYuanBao;
};

struct tagLogItemTims			// 物品使用次数log结构
{
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT64		n64Serial;
	DWORD		dwTypeID;
	INT			nUsedTimes;
	INT			nMaxUseTimes;
	INT8		n8ConType;
};

struct tagLogFund
{
	DWORD		dwGuildID;
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT			nFund;
	INT			nTotalFund;
};

struct tagLogMaterial
{
	DWORD		dwGuildID;
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT			nMaterial;
	INT			nTotalMaterial;
};

struct tagLogReputation
{
	DWORD		dwGuildID;
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	DWORD		dwCmdID;

	INT			nReputation;
	INT			nTotalReputation;
};

struct tagLogGMCmd
{
	DWORD		dwRoleID;
	TCHAR		szGMCmd[X_LONG_STRING];
	DWORD		dwErrorCode;
};

struct tagLogPet
{
	DWORD		dwMasterID;
	DWORD		dwPetID;
	DWORD		dwCmdID;
};

struct tagLogRoleOnline
{
	DWORD		dwRoleID;
	DWORD		dwOnlineLastTime;
	CHAR			szMac[X_SHORT_STRING];
};

struct tagLogFairyBirth
{
	DWORD		dwAccountID;
	DWORD		dwRoleID;
	INT64			nFaBao64Serial;
	BOOL			bNormalChange;
	INT		nBeforeChangeBirthState;
	INT		nAfterChangeBirthState;
};
//元神装备强化log
struct tagLogHolyEquipConsolidate
{
	EHolyEquipConsolidateType	eType;	//元神装备强化类型
	INT64	n64HolyEquipID;				//元神装备ID
	bool	bCostMoney;					//扣除金钱是否成功
	INT		nCostMoney;					//扣除钱数
	bool	bCostItem;					//扣除物品是否成功
	INT		nCostItem;					//扣除物品数
	bool	bCostItemOther;				//扣除老君仙露是否成功
	INT		nCostItemOther;				//扣除老君仙露数
	bool	bConsolidateRes;			//强化是否成功
	BYTE	byCurrentLevel;				//元神装备当前等级
	DWORD	dwRoleID;					//角色ID	
};

//-----------------------------------------------------------------------------
#pragma pack(pop)