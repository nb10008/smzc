#pragma once


#pragma pack(push, 1)

//------------------------------------------------------------------------------
// 物品特殊功能类型定义
//------------------------------------------------------------------------------

/** \物品特殊功能类型(该处用<val1, val2>表示有2个参数,<val>表示1个,没有注明表示没有)
*/
enum EItemSpecFunc	
{
	EISF_Null										= 0,	// 无

	EISF_AcceptQuest								= 1,	// 接取任务
	// nSpecFuncVal1: 任务编号
	// nSpecFuncVal2: 保留
	EISF_IdetifyEquip								= 2,	// 鉴定装备
	// nSpecFuncVal1: 鉴定级别
	// nSpecFuncVal2: 保留
	EISF_HoleGem									= 3,    // 镶嵌
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_Brand										= 4,	// 烙印
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_LongHunShi									= 5,    // 龙附
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_RemoveAttPt								= 6,	// 属性洗点
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_RemoveTalentPt								= 7,	// 天资洗点
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_LearnSkill									= 8,	// 学习技能
	// nSpecFuncVal1: 技能ID
	// nSpecFuncVal2: 保留
	EISF_Grind										= 9,	// 磨石
	// nSpecFuncVal1: 恢复暂新度值
	// nSpecFuncVal2: 保留
	EISF_ComposeAdvance								= 10,	// 合成几率提升符
	// nSpecFuncVal1: 可提升类型	
	// nSpecFuncVal2: 提升百分比
	EISF_OpenSafeGuard								= 11,	// 强开PK保护
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_RevieveItem								= 12,	// 复活道具
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_FriendGift									= 13,	// 好友礼物
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_Chisel										= 15,	// 开凿
	// nSpecFuncVal1: 基础成功率
	// nSpecFuncVal2: 开凿等级
	EISF_ProtectSign								= 16,	// 保底符
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_Chest										= 17,	// 宝箱
	// nSpecFuncVal1: 宝箱开出的物品(宝箱表)
	// nSpecFuncVal2: 钥匙的TypeID
	EISF_ChestKey									= 18,	// 宝箱钥匙
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留

  //EISF_Chest*                                     = 19,   // 宝箱保留
	// nSpecFuncVal1: 保留
    // nSpecFuncVal2: 保留
	
	EISF_PetEgg										= 20,	// 宠物蛋
	// nSpecFuncVal1: 原型ID
	// nSpecFuncVal2: 保留
	EISF_PetFood									= 21,	// 宠物食物
	// nSpecFuncVal1: 1,携带等级 2,增加寿命
	// nSpecFuncVal2: 1，精气值	 2，增加量
	EISF_PetSkillBook								= 22,	// 宠物技能书
	// nSpecFuncVal1: 技能原型ID
	// nSpecFuncVal2: 保留
	EISF_PetLvlupStep								= 23,	// 宠物升阶
	// nSpecFuncVal1: 灵阶后将要到达的灵阶值
	// nSpecFuncVal2: 保留
    EISF_PetEquipment                               = 24,	// 宠物装备
    // nSpecFuncVal1: 装备原型ID
    // nSpecFuncVal2: 保留
    EISF_ColorProbability                           = 25,   // 指定颜色阶段几率提升
    // nSpecFuncVal1: 数字1-4，由低到高分别表示四个颜色稀有程度阶段：低，中，高，极高
    // nSpecFuncVal2: 1-10000，表示对指定阶段的几率的提升程度（10000表示提升100%）
    EISF_Dye                                        = 26,   // 染料
    // nSpecFuncVal1: 1-12，对应颜色贴图编号
    // nSpecFuncVal2: 保留
	EISF_GuildSkillBook								= 27,	// 帮派技能典籍
	// nSpecFuncVal1: 上缴时提升进度增加量
	// nSpecFuncVal2: 帮派贡献增加量
	EISF_PetExpPour									= 28,	// 宠物经验灌注
	// nSpecFuncVal1: 灌注经验值
	// nSpecFuncVal2: 保留
	EISF_PetEnhance									= 29,	// 宠物资质提升
	// nSpecFuncVal1: 灵兽的品级
	// nSpecFuncVal2: 最大资质值
	EISF_ItemLock									= 30,	// 物品锁定

	EISF_ItemUnLock									= 31,	// 物品解锁
	
	EISF_PetLock									= 32,	// 宠物锁定
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_PetUnLock									= 33,	// 宠物解锁
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	
	EISF_PortableWare								= 34,   // 随身仓库

	EISF_TransportTool                              = 35,   // 随身传送符

	EISF_Fishing									= 36,	// 钓鱼，挂机系统

	EISF_ChangeVocation								= 37,	// 转职

	EISF_EquipAddPotValue							= 38,	// 提升装备潜力道具
	// nSpecFuncVal1: 成功率
	// nSpecFuncVal2: 潜力值的增加量
	
	EISF_BagExtend									= 39,	// 背包扩展
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	
	EISF_RoleWareExtend								= 40,	// 仓库扩展
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	
	EISF_BeGoldStone								= 41,	// 点金石
	// nSpecFuncVal1: 1： 点金石	 2： 次级点金石
	// nSpecFuncVal2: 保留
	
	EISF_RenameItem									= 42,	// 易名贴
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	
	EISF_GemRemovalSign								= 43,	// 宝石拆除符

	EISF_PlayerTrack								= 44,	// 道具追踪
	// nSpecFuncVal1: 0	表示不能进行传送  1 表示能进行传送
	// nSpecFuncVal2: 保留

	EISF_EquipSignature								= 45,	// 装备署名
	// nSpecFuncVal1: 0	表示署名后不绑定  1 表示署名后绑定

	EISF_PetPocketExtItem							= 46,	// 扩充宠物带道具
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留

	EISF_AddMiraclePrcNumItem						= 47,	// 增加仙界修行点儿的道具　仙界修行符
	// nSpecFuncVal1: 增加的点数
	// nSpecFuncVal2: 保留

	EISF_SpinFashionDressItem						= 48,	// 金丝线
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留

	EISF_LoadScript									= 49,	// 调用脚本的特殊物品
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留

	// Jason 2010-4-12 v2.0.0
	EISF_LingZhu									= 50,	// 灵珠，镶嵌法宝用（即，融合灵珠）
	// nSpecFuncVal1: 需求法宝等阶（1~50）
	// nSpecFuncVal2: 需求法宝类型（金木水火土对应1~5）
	EISF_YuLingZhu									= 51,	// 御灵珠，提升法宝等阶之用
	// nSpecFuncVal1: 提升等阶的量
	// nSpecFuncVal2: 保留
	EISF_GuildChangeName							= 52,	// 帮派更名道具
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_EarthTemple								= 53,	// 土地庙
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_UpgradeEquipMaxPotValue					= 54,	// 提升装备潜力上限道具
	// nSpecFuncVal1: 装备位置，默认值为-1（代表没有限制）
	// nSpecFuncVal2: 使用该具后装备的潜力值上限可以提升到的数值，字段默认值为0
	EISF_ZhanDouFu									= 55,   // 战斗挂机道具战斗符
	
	EISF_RefreshQuestBoard							= 70,	// 刷新任务板道具

	EISF_LuckyFu									= 71,	// 幸运符
	// nSpecFuncVal1: 类型
	// nSpecFuncVal2: 提升百分比
	EISF_BaoDiFu									= 72,	// 保底符
	// nSpecFuncVal1: 0：保底符； 1：守护符
	// nSpecFuncVal2: 保留
	EISF_XiHun										= 73,	// 吸魂
	// nSpecFuncVal1: 吸魂等级
	// nSpecFuncVal2: 保留
	EISF_FuHun										= 74,	// 附魂
	// nSpecFuncVal1: 附魂的强化等级
	// nSpecFuncVal2: 保留
	EISF_QiangHuaStone								= 75,   // 强化石
	// nSpecFuncVal1: 强化等级
	// nSpecFuncVal2: 保留
	EISF_GuiYuan									= 76,   // 归原
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_FairyPowder								= 77,   // 妖精之粉
	// nSpecFuncVal1: 资质下限
	// nSpecFuncVal2: 资质上限

	EISF_TransferStone								= 78,   // 传送灵石

	EISF_GuildSkillActive							= 79,	// 公会技能激活书
	// specfuncval1: 可以激活的技能ID
	// nSpecFuncVal2: 保留

	EISF_TreasureSeizer								= 80,	// 夺宝奇兵

	EISF_GodStrengthen								= 81,   // 打开神铸界面的物品
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留

	EISF_VipCard									= 82,   // VIP卡
	// nSpecFuncVal1: VIP卡等级，1银，2金，3白金
	// nSpecFuncVal2: 保留

	EISF_FairyCode									= 83,	// 妖精法典
	// nSpecFuncVal1: 1火，2水，3沙，4风，5光明，6暗黑
	
	EISF_VerityOfStone								= 84,	// 真理之石
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留

	EISF_FairyFruit									= 85,	// 妖精果实
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留

	EISF_FairyStuntBook								= 86,	// 妖精特技书
	// nSpecFuncVal1: 特技id 
	// nSpecFuncVal2: 保留

	EISF_FairyNirVana								= 87,	// 妖精涅槃
	// nSpecFuncVal1: 0: 会退到60级 1：回退到最近的10级
	// nSpecFuncVal2: 保留

	EISF_ExpBall											= 88, // 经验球（水晶球）
	// nSpecFuncVal1: 0:未约定 1：约定
	// nSpecFuncVal2: 如果nSpecFuncVal1=0，为水晶球[约定]的typeID

	EISF_ExpPilular										= 89, // 经验丹（水晶之心）
	// nSpecFuncVal1: 经验丹里面存储的经验
	// nSpecFuncVal2: 保留

	EISF_RockGod											= 90,	// 神石
	// nSpecFuncVal1: 装备的等级限制最小等级，是装备的等级限制，不是使用等级
	// nSpecFuncVal2: 装备的等级限制最高等级

	EISF_FairyDecPregnancy								= 91,	// 缩短孕期
	EISF_FairyIncBreedingTimes							= 92,	// 增加配对次数
	EISF_FairyDecRestTime								= 93,	// 缩短休憩时间

	EISF_EggHammer											= 94,	// 砸金蛋的锤子
	EISF_WardrobeLayerLvlUp							= 95,	// 升级衣橱栏位

	EISF_GlobalLimitedItem								= 96,	// 全局限量的物品

	EISF_FamilySpriteItem									= 97,	// 家族妖精修炼物品

	EISF_FairyRaiseIntelligenceMax					= 98,	// 提高妖精的资质上限药剂
	// nSpecFuncVal1: 对应开启资质上限的标识 0-开启100到120
	// nSpecFuncVal2: 成功的概率

	EISF_FairyDoRaiseIntelligence						= 99,	//	稳定提高妖精资质药剂
	// nSpecFuncVal1: 妖精资质下限
	// nSpecFuncVal2: 妖精资质上限

	EISF_BloodBrother										= 100,	//	结拜使用道具
	// nSpecFuncVal1: 天数

	EISF_EquipGrowClips									= 101,	// 器魂碎片
	// nSpecFuncVal1: 提升的装备成长度

	EISF_EquipGrowTransIM								= 102,	// 装备成长继承符
	// nSpecFuncVal1: 降星最小数
	// nSpecFuncVal2: 降星最大数
	EISF_EquipGrowLuckSign								= 103,	//成长幸运符
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_EquipReduceIM									= 104,	//退星道具
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留

	EISF_TigerCoins								= 105,	//老虎机币

	EISF_HunShiJingHua								= 106,	//魂石精华

	EISF_Soul_Skill								= 107,	//神魔技能续约

	EISF_EquipGrowTransPIM				= 108,	// 完美装备成长继承符

	EISF_DiamondEquipGrowIM				= 109,	// 装备成长洗属性以及宝石增幅道具
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_FairyFusion					= 110,	//妖精熔炼IM道具
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	//EISF_SoulInlay										= 111,	// 天影神石--废弃
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	//EISF_SoulChisel									= 112,	// 穿云箭
	// nSpecFuncVal1: 保留
	// nSpecFuncVal2: 保留
	EISF_ShengWenChaiChu				= 111,	//圣纹拆除器
	//nSpecFuncVal1: 保留
	//nSpecFuncVal2: 保留
	EISF_PerfectGrow					= 112,	//完美成长道具
	//nSpecFuncVal1: 附几颗星
	//nSpecFuncVal2: 保留
	EISF_XiStarIM						= 113,	//吸星大法
	//nSpecFuncVal1: 吸几颗星
	//nSpecFuncVal2: 对应生成的道具id
	EISF_ReplaceFabao					= 115,	//代妖精道具
	//nSpecFuncVal1:字段填写具体值（50-140），对应妖精评分分数
	//nSpecFuncVal2:保留
	EISF_HolyStrength					= 116,	//提升圣灵吞噬妖精次数的道具
	EISF_HolyEquipmentNumber			= 117,	//提升圣灵可装备圣纹数量
	EISF_NewChisel						= 118,	// 新开凿
	//nSpecFuncVal1: 保留
	//nSpecFuncVal2: 保留
};

/** \合成几率提升符 EISF_ComposeAdvance 可提升几率类别
*/
enum ESpecFuncComposeAdvance			
{
	ESFCA_AllProduce		= 0,	// 所有生产都可提升
	ESFCA_Artisan,					// 巧匠
	ESFCA_DanTraining,				// 练丹
	ESFCA_Smith,					// 锻造
	ESFCA_Casting,					// 铸甲
	ESFCA_Dressmaker,				// 裁缝
	ESFCA_Aechnics,					// 工艺
	ESFCA_Smilt,					// 冶练

	EISFC_AllConsolidate	= 10,	// 所有装备强化都可提升
	EISFC_Posy,						// 铭纹
	EISFC_Engrave,					// 镌刻
	EISFC_Quench,					// 淬火
	

	EISFC_All				= 20,	// 所有合成都可提升 
};


#pragma pack(pop)