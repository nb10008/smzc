//-----------------------------------------------------------------------------
//!\file msg_compose.h
//!
//!\date 2008-09-25
//!
//!\brief 客户端与服务器之间关于合成，分解，强化的消息
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "compose_define.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// 错误码枚举
//----------------------------------------------------------------------------
enum 
{
	//共有错误码
	E_Compose_NPC_Not_Exist = 1, 				//NPC不存在
	E_Compose_Formula_Not_Exist = 2, 			//配方不存在
	E_Compose_Stuff_Not_Enough = 3, 			//材料数量不够
	E_Compose_Stuff_Formula_Not_Match = 4, 		//材料和配方不匹配
	E_Compose_NPC_Distance = 5,					//NPC距离太远
	
	//合成强化返回的三种结果,是根据合成公式算的
	E_Compose_Consolidate_Success = 11, 		//成功
	E_Compose_Consolidate_Lose = 12, 			//失败
	E_Compose_Consolidate_Perfect = 13, 		//完美

	
	//合成错误码
	E_Compose_Stuff_Not_Exist = 21, 			//合成材料不存在
	E_Compose_Type_Not_Exist = 22,				//合成类型不存在
	E_Compose_Skill_Not_Exist = 23,				//玩家没有合成技能
	E_Compose_FormulaNotMatch = 24,				//合成配方不匹配
	E_Compose_NPCCanNotCompose = 25,			//不是合成NPC
	E_Compose_Vitality_Inadequate = 26,			//玩家活力值不足
	E_Compose_NotEnough_Money = 27,				//玩家金钱不够
	E_Compose_Bag_Full = 28,					//背包已满
	E_Compose_EquipUnidentify = 29,				// 装备未鉴定
	
	//分解错误码
	E_Decomposition_Item_Not_Exist = 31, 		//分解物品不存在

	//强化错误码
	E_Consolidate_Equip_Not_Exist = 41, 		//强化装备不存在
	E_Consolidate_Type_Not_Exist = 42,			//强化类型不存在
	E_Consolidate_NPCCanNotPosy	= 43,			//不是铭纹NPC
	E_Consolidate_NotEquipment = 44,			//强化的物品不是装备
	E_Consolidate_BeyondPosyTime,
	E_Consolidate_FormulaNotMatch,				//强化配方不匹配
	E_Consolidate_NotEnough_Money,				//玩家金钱不够
	E_Consolidate_NotEnough_Stuff,				//玩家材料不够
	E_Consolidate_EquipCanNotPosy,				//该属性不能被铭纹
	E_Consolidate_ValConsume_Inadequate,		//装备潜力值不足

	E_Compose_NPCCanNotEngrave,					//不是镌刻NPC
	E_Consolidate_EquipCanNotEngrave,			//该属性不能被镌刻
	E_Consolidate_BeyondEngraveTime,			//超过镌刻次数

	E_Consolidate_Gem_Not_Exit,					//宝石不存在
	E_Consolidate_Gem_Locked,					//宝石被锁定
	E_Consolidate_Gem_Full,						//装备已镶满宝石
	E_Consolidate_EquipCanNotInlay,				//该宝石不能被镶嵌
	E_Consolidate_Gem_Not_Hole,					//装备无孔

	E_Consolidate_Brand_Not_Exit,				//印记不存在
	E_Consolidate_Brand_TopLevel,				//印记等级达到上限
	E_Consolidate_EquipCanNotBrand,				//该装备不能被印记
	E_Consolidate_GuiYuanStone_Not_Exist,		//归元石不存在
	E_Consolidate_GuiYuanEquip_Not_Exist,		//要执行归元操作的装备不存在

	E_Consolidate_LoongSoul_Not_Exit,			//龙魂石不存在
	E_Consolidate_LoongSoul_TopLevel,			//龙魂等级达到上限
	E_Consolidate_EquipCanNotLoongSoul,			//该装备不能被龙附

	E_Consolidate_NPCCanNotQuench,				//不是淬火NPC
	E_Consolidate_WXValueInadequate,			//原属性值不足
	E_Consolidate_False,						//淬火失误

	E_Compose_Quality_Not_Match,				//点化只限于黄色及以上装备
	E_Compose_Not_Fashion,						//面具及时装不可点化或分解
	E_Compose_Equip_Lock,						//已锁定的装备不可点化或分解
	E_Compose_Equip_Time_Limit,					//有时间限制的装备不可点化或分解
	E_Compose_Equip_Not_identify,				//未鉴定的装备不可点化或分解
	E_Compose_Equip_Level_Inadequate,			//装备等级不够
	E_Compose_Equip_Quality_Inadequate,			//装备品质不够
	E_Compose_Equip_Type_Inadequate,			//装备类型不满足配方要求

	E_Consolidate_Chisel_Not_Exit,				//凿石不存在
	E_Consolidate_Equip_CanNot_Chisel,			//装备不能被开凿
	E_Consolidate_Equip_Hole_Full,				//镶嵌孔数量已满，不可进行开凿
	E_Consolidate_Chisel_Not_Match,				//凿石等级小于所开凿的装备等级，不可进行开凿
	E_Consolidate_NotIdentified,				//未鉴定装备不可开凿

	E_Raise_PotVal_Not_Needing,					// 已经满了，不需要在提升潜力值
	E_Raise_PotVal_Times_Zero,					// 提升次数为零

	// Jason v1.3.1 2009-12-21 宝石拆除errorid
	E_Gem_NPCCanNotRemoveGem,					//不是宝石拆除NPC

	E_Consolidate_CanNotSpin,					//非精纺物品

	E_Consolidate_Spin_TopLevel,				//精纺达到最高等级

	E_Extraction_EquipInvalid,					//萃取的装备不存在
	E_Extraction_NotEquip,						//要萃取的不是装备
	E_Extraction_NoFormula,						//没有对应的萃取配方
	E_Extraction_QualityLimit,					//品级限制
	E_Extraction_VitalityLimit,					//活力限制
	E_Extraction_MoneyLimit,					//金钱限制
	E_Extraction_BagFull,							//背包空间不足

	// Jason 2010-4-15 v2.0.0 法宝相关
	E_Fabao_Practice_NoYuLingshi,				// 御灵石不存在
	E_Fabao_Practice_NotYuLingshi,				// 原料不是御灵石
	E_Fabao_FabaoNotExist,				// 法宝不存在
	E_Fabao_Practice_NotFabao,					// 目标装备不是法宝
	E_Fabao_Practice_LingshiNumCrossBorder,		// 御灵石数量超过最大可堆叠数量限制
	E_Fabao_Practice_FabaoStageFull,			// 法宝已经到了最大阶
	E_Fabao_Practice_NoResource,				// 没有找到相关的资源，比如等阶表xml
	E_Fabao_Level_Limitted,						// 修炼法宝，受到个人等级限制
	E_Fabao_LingzhuWanted,						// 需要融合灵珠后，才能进一步修炼法宝

	// 2010-4-16 v2.0.0
	E_Fabao_Inlay_NotLingzhu,					// 不是灵珠
	E_Fabao_Inlay_SlotLTLingzhu,				// 灵槽小于灵珠个数
	E_Fabao_Inlay_Mismatch,						// 灵珠与法宝不匹配（包括类型，阶数等）
	E_Fabao_UnkownError,						// 其他未知错误

	E_Fabao_Not_Exist,							// 法宝不存在
	E_Fabao_TypeMismatch,						// 非法宝
	E_Fabao_LevelLimmit,						// 法宝等级不足
	E_Fabao_NotPermitted,						// 不允许重置
	E_Fabao_CannotBeAider,						// 不能作为辅助法宝

	E_UpgradeMaxPot_NoNeed,						// 该装备不需要使用此道具
	E_UpgradeMaxPot_EquipPotTooLow,				// 该装备的最高潜力值太低，无法使用此道具

	E_SoaringEquipment_NPC_Invalid,				// npc非法

	E_EquipPotval_ItemTypeInvalid,				// 提升潜力值上限的道具类型不对
	E_EquipPotval_EquipPosInvalid,				// 物品不能提升此类装备的潜力值
	E_SoaringStone_NotExist,					// 炼石不存在
	E_SoaringEquipment_NotEquip,			// 非装备不可炼器
	E_SoaringEquipment_StuffMustBeEquip,	// 辅助材料必须是装备
	E_SoaringEquipment_StuffQltyLower,		// 辅料装备品级不够
	E_SoaringEquipment_NotAccessories,		// 非炼器辅料
	E_SoaringEquipment_LevelLower,			// 待炼器装备，等级不足

	E_CastingEquipment_NPC_Invalid,			// 非熔铸npc
	E_CastingEquipment_NotEquip,			// 非装备，不可参与炼器
	E_CastingEquipment_MustBeSoaringEquip,	// 必须是飞升装备
	E_CastingEquipment_CastingTimesNotMatch,	// 熔铸材料id没有匹配装备的熔铸次数
	E_CastingEquipment_StuffTypeDismatch,	// 材料与装备类型不一致
	E_CastingEquipment_StuffTypeDismatchProto,	// 材料与熔铸原型类型不一致
	E_CastingEquipment_StuffNotEnough,		// 材料总类型数量不对
	E_CastingEquipment_StuffNotExist,		// 材料不存在


	E_Compost_Equip_PotvalNotEnough,		//装备潜力值不足
	E_SoaringEquipment_NotIdentify,			// 未鉴定装备不可炼器或熔铸
	E_SoaringEquipment_FabaoHasBeenProhibited,	// 法宝不能炼器或熔铸
	E_SoaringEquipment_Fail,					// 炼器失败，正常概率失败
	E_CastingEquipment_Fail,					// 熔铸失败，正常概率失败
	E_SoaringEquipment_FashionHasBeenProhibited,	// 时装不能被用来炼器或熔铸
	E_SoaringEquipment_TimelimittedEquipHasBeenProhibited,	// 时限装备不能炼器或熔铸

	// Jason 2010-6-19 v1.0.0 强化特殊错误码
	E_Consolidate_Stone_Not_Exist ,				// 没有强化石
	E_Consolidate_Stone_LevelLow,				// 强化石等级低
	E_Consolidate_FormulaNotExist,				// 强化配方不存在
	E_Consolidate_LevelLimitExceeded,			// 超过强化最高等级
	E_Consolodate_F_Fail,							// 强化失败
	E_ADSoul_Equip_ConsolidateLevelZero,		// 装备强化等级为0，不可吸魂
	E_ADSoul_Equip_XihunStoneNotExist,			// 吸魂石不存在
	E_ADSoul_Equip_NotXihunStone,				// 非吸魂石
	E_ADSoul_Equip_XihunLevelLow,				// 吸魂石等级低
	E_ADSoul_Equip_SpecEquipCannotBeDetachSoul,	// 特殊装备，如时装，法宝等，不可以被吸魂
	E_ADSoul_Equip_CannotFindFuhunStone	,			// 无法找到附魂石
	E_ADSoul_Equip_FuhunStoneCreatorFail,			// 附魂石产生失败

	E_ADSoul_Equip_FuhunStoneNotExist,				// 附魂石不存在
	E_ADSoul_Equip_NotFuhunStone,					// 非附魂石
	E_ADSoul_Equip_CannotFindFuhunInfo,				// 找不到附魂石相关的强化等级信息
	E_ADSoul_Equip_FuhunLevelLow,					// 附魂石等级没有装备强化等级高

	E_Fabao_RaiseIntelligence_Success,				// 妖精秘药，提升资质成功
	E_Fabao_RaiseIntelligence_Failse,				// 提示失败，并且资质减少
	E_Fabao_RaiseIntelligence_NoChange,				// 资质没有改变
	E_Fabao_RaiseIntelligence_AlreadyFull,			// 资质已满，不能再提升了。
	E_Fabao_RaiseIntelligence_FairyPowderNotExist,			// 妖精之粉不存在
	E_Fabao_RaiseIntelligence_FairyPowderNotFit,
	E_Fabao_Spec98ItemNotExist,	// 妖精潜能秘药不存在
	E_Fabao_Spec98Item_Success, // 成功提升了妖精的资质上限
	E_Fabao_Spec98Item_AlreadyUse,	// 您的妖精已经激发过潜能且不消耗道具
	E_Fabao_Spec98Item_UseFailed, // 提升妖精资质上限失败
	E_Fabao_Spec99ItemNotExist, // 稳固妖精资质药剂不存在
	E_Fabao_ItemNotFit2FairyIntellgence,	// 妖精当前的资质无法使用该药剂
	E_Fabao_ItemUseFailed,	// 物品使用失败
	E_Fabao_RaiseIntelligence_NotInBag,				// 妖精没有在背包，这个需要在背包中做，而不是在装备栏
	E_Fabao_SecondeFabaoLevelLower,					// 辅助妖精（原法宝）等阶不足20级
	E_ConsolidateInlay_EquipLevelLower,				// 装备等级不够

	E_ADSoul_Equip_FuhunLevelToHight,				// 附魂石等级太高，装备未飞升（20），飞升（30）可能会报这个错误
	E_Consolidate_Stone_NotMatchStuff,				// 装备强化石类型不匹配下一等级强化材料类型
	E_ADSoul_Equip_GodStrength_NotEnough,			// 要附魂的装备神铸等级不足，不可以附魂

	// 神铸
	E_GodStrengthen_EquipQltyLower,					// 此装备品级不足
	E_GodStrengthen_EquipCannotStrengAgain,			// 该装备不需要再神铸了，满10级了
	E_GodStrengthen_AssitEquip_QltyLower,			// B栏装备品级不足
	E_GodStrengthen_AssitEquip_LevelLower,			// B栏装备等级不足
	E_GodStrengthen_AssitEquip_TimelimForbided,	// 时限制装备不能作为辅助装备
	E_GodStrengthen_EquipPosNotMatch,				// 装备位置不匹配
	E_GodStrengthen_EquipCannotLevelupInOneDay,	// 装备不能在一天内，连续神铸升级

	// 强化新增errorcode
	E_Consolidate_LuckySign_NeedGodStrengSign,				// 装备强化需要神铸幸运符
	E_Consolidate_LuckySign_LevelLimit,		// 灵魂链，等级限制
	E_Consolidate_LuckySign_BaodiLimit,	// 灵魂链，不需要保底符

	E_Fabao_EleInjuryChange_NPCTypeErr		,		// 妖精元素伤害类型转换npc类型不对
	E_Fabao_EleInjuryChange_FairyEquipErr	,		// 请放入妖精，或者妖精不存在
	E_Fabao_EleInjuryChange_StuffErr,				// 材料类型不对
	E_Fabao_EleInjuryChange_LevelLimit,				// 妖精等级太低，低于60级
	E_Fabao_EleInjuryChange_NotEnoughMoney,			// 钱不够

	E_Fabao_StuntBook_SameID,						// 不能学习相同的妖精特技
	E_Fabao_StuntBook_LevelLimit,						// 妖精资质或等级不足，不能学习

	E_Fabao_StuntConfirm_SameID,					// 不能学习相同的妖精特技

	E_RockStone_NotRockStone,						// 不是神石
	E_RockStone_NotEquipment,						// 不能洗非装备道具
	E_RockStone_LevelLimit,							// 装备等级限制
	E_RockStone_TypeMissMatch,						// 不能洗此类装备
	E_RockStone_WhiteQualityLimit,				// 不能洗白色品级装备

	E_GodStrengthen_BaodiFuLevelLimit,				// 保底符或者守护符，等级太低

	E_OnlyCanStrengthenByScript,				// 只能靠脚本对该装备进行强化	

	E_SoulCrystal_Grow_ItemInvalid,	// 魂晶非法
	E_SoulCrystal_Grow_IMError, // 精华非法
	E_SoulCrystal_Grow_MaxLvl, // 魂晶已达最大等级
	E_SoulCrystal_Grow_SoulLimit, // 神魂不足
	E_SoulCrystal_Grow_MoneyLimit, // 金钱不足

	E_SoulCrystal_Renew_ItemInvalid, // 放入的魂晶非法
	E_SoulCrystal_Renew_IMLimit, // 缺少魂晶改造符
	E_SoulCrystal_Renew_Limit, // 重置所需条件不足
	E_FairyAddPromoteTimes_ToMax,// 已达到最大提升次数
	E_FairyAddPromoteTimes_LackMoney,//金钱不足

	E_FairyFusion_LowQuality,// 妖精品质不足（要求橙色以上） [3/22/2012 zhangzhihua]
	E_FairyFusion_LowNativeIntelligence,// 妖精资质不足60 [3/22/2012 zhangzhihua]
	E_FairyFusion_LowLevel,// 妖精等级不足40 [3/22/2012 zhangzhihua]
	E_FairyFusion_StatusError,// 妖精不能在孕育，生育，休憩阶段 [3/22/2012 zhangzhihua]
	E_FairyFusion_FairyError,// 特殊妖精，无法融合 [3/22/2012 zhangzhihua]
	E_FairyFusion_CannotUseItem,// 不能使用道具 [3/23/2012 zhangzhihua]
	E_FairyFusion_Faild,

	E_SLEatFairy_EquipInValid,		// 圣灵吞噬妖精，客户端提供的装备类型不符合
	E_SLEatFairy_GetProtoFailed,    // 圣灵吞噬妖精，通过妖精得分没有找到相应的属性提升区间
	E_SLEatFairy_CountNotEnough,	// 圣灵吞噬妖精，当天剩余次数不足
	E_SLEatFairy_CountExceed,		// 圣灵吞噬妖精，已达到总次数限制
	E_SLEatFairy_lowLevel,			// 圣灵吞噬妖精，不能吞噬小于40级的妖精
	
	E_CallShengLing_HolyNotEnough,	// 人物身上的圣灵值不足召唤圣灵
	E_CallShengLing_EquipInValid,	// 圣灵不符合召唤条件
	E_CallShengLing_ForMountState,	// 骑乘状态下不能召唤圣灵
	E_CallShengLing_UseSkillState,	// 技能读条时不让召唤圣灵

	E_EquipHoly_EquipInValid,		// 给圣灵穿装备，客户端提供的装备类型不符合
	E_EquipHoly_NotEmpty,			// 给圣灵穿装备，圣灵上装备已满
	E_EquipHoly_HolyLowLevel,		// 给圣灵穿装备，放入的圣纹吞噬等级要求过高
	E_EquipHoly_NotEnough,			// 给圣灵穿装备，圣灵剩余灵能不足
	E_EquipHoly_ExceedLapCount,		// 给圣灵穿装备，超过了该类型圣纹可在圣灵上重复装备的数量

	E_EquipHoly_UnEquipInValid,		// 给圣灵卸装备，客户端提供的装备类型不符合
	E_EquipHoly_QualityNotEqual,	// 给圣灵卸装备，洗印符与圣纹 品级不相等
	E_EquipHoly_SilverNotEnough,	// 给圣灵卸装备，所需金币不足
	E_EquipHoly_SpaceNotEnough,		// 给圣灵卸装备，背包空间不足

	E_EnhanceHolyEquip_SilverNotEnough,	// 给圣纹充能，所需金币不足
	E_EnhanceHolyEquip_CountExceed,		// 给圣纹充能，充能次数已经达到最大
	E_EnhanceHolyEquip_ProtoNotFound,	// 给圣纹充能，找不到强化道具对应的属性
	E_EnhanceHolyEquip_MinHolyExceed,	// 给圣纹充能，消耗的灵能已经达到最小灵能值
	E_EnhanceHolyEquip_ItemNotFit,		// 给圣纹充能，充能道具不符合

	E_EnhanceHolyEquip_MaxHolyExceed,		// 给圣纹基础属性充能，圣灵的灵能消耗不可再增加10
	E_EnhanceHolyEquip_ProtoNotExist,		// 给圣纹基础属性充能，圣纹本身不具备要偏向的属性

	E_StarEnhanceCondensate_MaxLevel,				//元神装备冲星已达到最大等级
	E_StarEnhanceCondensate_NoExist,				//冲星强化不存在
	E_StarEnhanceCondensate_EquipCanNotOpt,			//非元神装备不能进行冲星强化
	E_StarEnhanceCondensate_NoEnoughItem,			//没有足够道具进行下一级冲星强化
	E_StarEnhanceCondensate_NoEnoughMoney,			//没有足够金钱进行下一句冲星强化
	E_StarEnhanceCondensate_Failed,					//冲星失败
	E_StarEnhanceCondensate_NotOpen,				//冲星强化暂时关闭

	E_HolySoulReborn_LevelNotEnough,				//元神必须达到100级，才可以进行转生操作
	E_HolySoulReborn_ItemNotExist,					//元神转生所需物品不足
	E_HolySoulReborn_LevelMax,						//元神转生等级已达到最大
	E_HolySoulReborn_ProtoNotExist,					//元神转生不存在	
	E_HolySoulReborn_AwakeCannot,					//元神觉醒状态下不能转生
	E_HolySoulReborn_ItemNotEnough,					//老君仙露个数不足
	E_HolySoulReborn_Fail,							//元神转生失败

	E_TransEquipSoul_HasNotFuHuned,					//未附魂装备不能进行魂核转移
	E_TransEquipSoul_NoEquipSoulTransfer,			//没有相应的魂核可转移
	E_TransEquipSoul_NoExsitItem,					//放入相应的道具有误
	E_TransEquipSoul_Failed,						//魂核转移操作失败
	E_TransEquipSoul_PosNotExist,					//待转移魂核的装备部位不匹配
	E_TransEquipSoul_UnIdentified,					//未鉴定装备不可进行魂核转移操作
	E_TransEquipSoul_EquipTimeLimited,				//时限制装备不可进行魂核转移操作
	E_TransEquipSoul_NotEnoughNumToCostItem,		//没有足够的消耗道具

	E_HolyEquipHole_EquipNotExist,					//请放入待凿孔装备
	E_HolyEquipHole_ItemNotExist,					//请放入凿孔道具
	E_HolyEquipHole_NotHolyEquip,					//放入的不是元神装备
	E_HolyEquipHole_NotHoleItem,					//放入的不是穿云箭
	E_HolyEquipHole_HoleNumMax,						//该装备开凿孔数已达到最大，无法继续开凿
	E_HolyEquipHole_ProtoNotExist,					//凿孔属性不存在	
	E_HolyEquipHole_MoneyNotEnough,					//背包中金钱不足
	E_HolyEquipHole_Fail,							//凿孔失败

	E_HolyEquipInlay_EquipNotExist,					//请放入元神装备
	E_HolyEquipInlay_NotInlayItem,					//请放入天影神石
	E_HolyEquipInlay_HoleIDUnusual,					//选择镶嵌的孔位异常
	E_HolyEquipInlay_HoleNotChisel,					//选择镶嵌的孔位尚未开凿
	E_HolyEquipInlay_HoleAlreadyInlay,				//选择镶嵌的孔位已经镶嵌宝石
	E_HolyEquipInlay_ProtoNotExist,					//镶嵌属性不存在	
	E_HolyEquipInlay_FormulaNotExist,				//配方不存在
	E_HolyEquipInlay_MoneyNotEnough,				//背包中金钱不足
	E_HolyEquipInlay_EquipCannotInlay,				//天影神石与装备镶嵌部位不匹配
	E_HolyEquipInlay_ItemNotEnough,					//背包中老君仙露个数不足
	E_HolyEquipInlay_Fail,							//镶嵌失败

	E_HolyEquipRemove_EquipNotExist,				//请放入元神装备
	E_HolyEquipReomve_HoleIDUnusual,				//选择的孔位异常
	E_HolyEquipReomve_HoleNotChisel,				//选择镶嵌的孔位尚未开凿
	E_HolyEquipReomve_HoleNotInlay,					//装备该孔位尚未镶嵌
	E_HolyEquipReomve_BagNotEnough,					//背包空间不足
};

//-----------------------------------------------------------------------------
//	合成消息
//-----------------------------------------------------------------------------
CMD_START(NC_Produce)
	DWORD 				dwNPCID; 		//NPCID
	DWORD				dwSkillID;		//技能ID
	INT64				n64ItemID;		//物品ID
	DWORD 				dwFormulaID;  	//配方ID
	INT64				n64IMID;		//IMid
	INT64				n64StuffID[1]; 	//材料 数量变长
CMD_END

CMD_START(NS_Produce)
	DWORD				dwErrorCode;	//返回码
CMD_END

// 高级生产
CMD_START(NC_AdvancedCompose)
	DWORD dwNPCID; 		//NPCID
	INT64 n64DesignID;
	INT64 n64Material1;
	INT64 n64Material2;
CMD_END

CMD_START(NS_AdvancedCompose)
	DWORD dwErrorCode;
	DWORD dwCompItemID;
CMD_END

//-----------------------------------------------------------------------------
//	分解消息 属于点化技能中的一个分支
//-----------------------------------------------------------------------------
CMD_START(NC_Decomposition)
	DWORD				dwNPCID; 		//NPCID	
	DWORD				dwSkillID;		//技能ID
	INT64				n64ItemID;		//物品ID
	DWORD				dwFormulaID;	//配方ID
	INT64				n64IMID;		//IMid
	INT64				n64Item; 		//被分解物品64位ID
CMD_END

CMD_START(NS_Decomposition)
	DWORD				dwErrorCode;	//返回码
CMD_END

//-----------------------------------------------------------------------------
//	萃取消息
//-----------------------------------------------------------------------------
CMD_START(NC_Extraction)
	DWORD				dwNPCID; 		//NPCID	
	INT64				n64Item; 		//被萃取物品64位ID
CMD_END

CMD_START(NS_Extraction)
	DWORD				dwErrorCode;	//返回码
CMD_END

//-----------------------------------------------------------------------------
//	强化消息
//-----------------------------------------------------------------------------
CMD_START(NC_ConsolidatePosy)
	DWORD 				dwNPCID; 		//NPCID
	DWORD 				dwFormulaID;  	//配方ID
	INT64 				n64ItemID; 		//要被强化物品的64位ID
	INT64				n64IMID;		//IMid
	INT64				n64StuffID[1]; 	//材料 数量变长	
CMD_END

CMD_START(NS_ConsolidatePosy)
	DWORD				dwErrorCode;	//返回码
CMD_END

CMD_START(NC_ConsolidateEngrave)
	DWORD				dwNPCID;		//NPCID
	DWORD				dwFormulaID;	//配方ID
	INT64				n64ItemID;		//要被强化物品的64位ID
	INT64				n64IMID;		//IMID
	INT64				n64StuffID[1];	//材料 
CMD_END

CMD_START(NS_ConsolidateEngrave)
	DWORD				dwErrorCode;	//返回码
CMD_END

CMD_START(NC_ConsolidateQuench)
	DWORD				dwNPCID;		//NPCID
	DWORD				dwFormulaID;	//配方ID
	INT64				n64ItemID;		//要被强化物品的64位ID
	INT64				n64IMID;		//IMID
	INT64				n64StuffID[1];	//材料 
CMD_END

CMD_START(NS_ConsolidateQuench)
	DWORD				dwErrorCode;	//返回码
CMD_END

//-----------------------------------------------------------------------------
//	时装精纺
//-----------------------------------------------------------------------------
CMD_START(NC_ConsolidateSpin)
	DWORD 				dwNPCID; 		//NPCID
	ESuitAttType 		eSpinType;  	//精纺属性类型
	INT64 				n64ItemID; 		//要被强化物品的64位ID
	DWORD				dwStuffTypeID;	//精纺材料typeid
	DWORD				dwStuffItemNum;	//精纺材料
CMD_END

CMD_START(NS_ConsolidateSpin)
	DWORD				dwErrorCode;	//返回码
CMD_END

//-----------------------------------------------------------------------------
//	镶嵌、烙印、龙附
//-----------------------------------------------------------------------------
CMD_START(NC_Inlay)
	INT64				n64SrcItemID;	//宝石的64ID
	INT64				n64DstItemID;	//装备的64ID
CMD_END

CMD_START(NS_Inlay)
	DWORD				dwErrorCode;	//返回码
CMD_END

CMD_START(NC_Brand)
	INT64				n64SrcItemID;	//印记的64ID
	INT64				n64DstItemID;	//装备的64ID
	INT64				n64IMID;		//保底符64ID 不使用:GT_INVALID
CMD_END

CMD_START(NS_Brand)
	DWORD				dwErrorCode;	//返回码
CMD_END

CMD_START(NC_LoongSoul)
	INT64				n64SrcItemID;	//龙魂石64ID
	INT64				n64DstItemID;	//装备的64ID
CMD_END

CMD_START(NS_LoongSoul)
	DWORD				dwErrorCode;	//返回码
CMD_END

CMD_START(NC_Chisel)	
	INT64				n64SrcItemID;	//装备的64ID
	INT64				n64StuffID;		//凿石的64ID
CMD_END

CMD_START(NS_Chisel)
	DWORD				dwErrorCode;	//返回码
CMD_END

//-----------------------------------------------------------------------------
//	提升装备潜力值
//-----------------------------------------------------------------------------

CMD_START(NC_AddPot)
	INT64				n64SrcItemID;	//道具的64ID
	INT64				n64DstItemID;	//装备的64ID
	INT64				n64IMID;		// 保底符64ID 不使用:GT_INVALID
CMD_END

CMD_START(NS_AddPot)
	DWORD				dwErrorCode;	//返回码 0：成功； 1：完美； 2：失败 
	DWORD				dwPotValue;		// 增加的潜力值
CMD_END

// Jason v1.3.1 2009-12-21 宝石拆解相关协议
CMD_START( NC_GetGemRemovalInfo )
	DWORD 				dwNPCID; 		// NPCID
	INT64				n64EquipID;		// 装备序列id
CMD_END

CMD_START( NS_GetGemRemovalInfo )
	DWORD				dwErrCode;		// 
	INT16				n16GemNum;		// 宝石个数，最多5颗宝石
	DWORD				dwGemIDs[MAX_EQUIPHOLE_NUM];	// 宝石typeids，0/GT_INVALID表示没有宝石
CMD_END

CMD_START( NC_GemRemoval )
	DWORD 				dwNPCID; 		// NPCID
	INT64				n64EquipID;		// 装备序列id
	INT16				n16GemNum;
	INT64				n64GemRomovalSign[MAX_EQUIPHOLE_NUM];	// 宝石拆除符id
CMD_END

CMD_START( NS_GemRemoval )
	DWORD				dwErrorCode;
CMD_END

// Jason 2010-4-15 v2.0.0 法宝修炼
CMD_START( NC_FabaoPractice )
	INT64				n64SrcItemID;	// 御灵石id
	INT					nSrcItemNum;	// 消耗灵石个数
	INT64				n64FabaoID;		// 法宝id
CMD_END
CMD_START( NS_FabaoPractice )
	DWORD				dwErrorCode;
	//tagFabaoSpec		fabaoSpec;
CMD_END
// 灵珠镶嵌
CMD_START( NC_FabaoInlay )
	INT64				n64FabaoID;
	INT64				n64Bead;	//灵珠serialid
CMD_END
CMD_START( NS_FabaoInlay )
	DWORD				dwErrorCode;
	//tagFabaoSpec		fabaoSpec;
CMD_END

// 五行逆转
CMD_START( NC_FabaoWuxingReversal )
	INT64				n64MainFabaoID;
	INT64				n64SecondaryFabaoID;
CMD_END
CMD_START( NS_FabaoWuxingReversal )
	DWORD				dwErrorCode;
	//tagFabaoSpec		fabaoSpec;
CMD_END
// 五行转生
CMD_START( NC_FabaoWuxingMerging )
	INT64				n64MainFabaoID;
	INT64				n64SecondaryFabaoID;
CMD_END
CMD_START( NS_FabaoWuxingMerging )
	DWORD				dwErrorCode;
	//tagFabaoSpec		fabaoSpec;
CMD_END

// 提升潜力值，不需要受潜力值提升次数限制
CMD_START( NC_RaiseEquipPotval )
	INT64		n64EquipID;
	INT64		n64ItemID;
CMD_END
CMD_START( NS_RaiseEquipPotval )
	DWORD dwErrorCode;
CMD_END

// Jason 2010-5-25 v2.1.2 装备飞升以及炼器
CMD_START( NC_BuildEuipment )
	DWORD		dwNPCID;
	INT64		n64ToBuildEquipID;		// 待修炼装备64位id
	INT64		n64StoneID		;		// 炼石ID
	INT64		n64StuffID		;		// 辅料id，也是装备
CMD_END
CMD_START( NS_BuildEuipment )
	DWORD	dwErrorCode;
CMD_END
//-----------------------------------------------------------------------------
//	装备熔铸
//-----------------------------------------------------------------------------
CMD_START(NC_SoarEquipCasting)
	DWORD 				dwNPCID; 		//NPCID
	DWORD 				dwFormulaID;  	//配方ID
	INT64 				n64ItemID; 		//要被强化物品的64位ID
	INT64 				n64ProtectSignID;	//保底符64位ID
	INT64				n64StuffID[1]; 		//材料 数量变长	
CMD_END

CMD_START(NS_SoarEquipCasting)
	DWORD				dwErrorCode;	//返回码
CMD_END


//struct tagCastingStuff
//{
//	INT64		n64ItemID;
//	UINT16		n16ItemNum;
//};
//
//CMD_START( NC_CastingEquipment )
//	DWORD	dwNPCID;
//	INT64	n64EquipID;
//	DWORD	dwCastringProtoID;
//	INT64	n64ItemID;
//	BYTE	byItemNum;
//	BYTE	byData[1];	// tagCastingStuff
//CMD_END

//CMD_START( NS_CastingEquipment )
//	DWORD dwErrorCode;
//CMD_END
// 装备强化
CMD_START( NC_StrengthenEquip )
	INT64	n64ToBeStrengthenedEquip;	// 待强化装备
	INT64	n64StrengthenStone;			// 强化石
	INT64	n64LuckySign;					// 幸运符物品id，非必须品，填0或者-1，表示没有
	INT64	n64ProtectedSign;				// 保底符，非必须品，填0或者-1，表示没有
CMD_END
CMD_START( NS_StrengthenEquip )
	DWORD dwErrorCode;
CMD_END
// 吸魂附魂
CMD_START( NC_DetachOrAttackSoul_Equip )
	BOOL	bDetachOrAttack;				// TRUE，吸魂；FALSE，附魂
	INT64	n64Equip;						// 装备
	INT64	n64Stone;						// 吸魂石或者附魂石
CMD_END
CMD_START( NS_DetachOrAttackSoul_Equip )
	DWORD dwErrorCode;
	BOOL bDetachOrAttack;	// TRUE，吸魂；FALSE，附魂
	INT64	n64ID;				// 64位ID
CMD_END

// 归元
CMD_START( NC_GuiYuan )
	INT64	n64Equip;						// 装备
	INT64	n64Stone;						// 归元石
CMD_END
CMD_START( NS_GuiYuan )
	DWORD dwErrorCode;
CMD_END

// 妖精之粉
CMD_START( NC_FairyPowder )
	INT64	n64Fairy;						// 妖精
	INT64	n64FairyPowder;					// 妖精之粉
CMD_END
CMD_START( NS_FairyPowder )
	DWORD dwErrorCode;
CMD_END

CMD_START( NC_FairyRaiseIntelligenceMax )
	INT64	n64Fairy;						// 妖精
	INT64	n64FairyPowder;					// 妖精之粉
CMD_END
CMD_START( NS_FairyRaiseIntelligenceMax )
	DWORD dwErrorCode;
CMD_END

CMD_START( NC_FairyDoRaiseIntelligence )
	INT64	n64Fairy;						// 妖精
	INT64	n64FairyPowder;					// 妖精之粉
CMD_END
CMD_START( NS_FairyDoRaiseIntelligence )
	DWORD dwErrorCode;
CMD_END

CMD_START( NC_GodStrengthen )
	INT64		dwEquipID;			// 神铸装备ID
	INT64		dwAssitEquipID;		// 辅助装备ID
CMD_END

CMD_START( NS_GodStrengthen )
	DWORD	dwErrorCode;
CMD_END

// 妖精元素伤害转化
CMD_START( NC_FairyEleInjuryChange )
	DWORD 				dwNPCID; 		//NPCID
	INT64				n64FairyID;	// 妖精序号，背包中的妖精
	EElementInjury		eEleInjuryType;		// 要转换为什么类型的元素伤害
	INT64				n64ItemID;				// 辅助道具的ID
CMD_END
CMD_START( NS_FairyEleInjuryChange )
	DWORD	dwErrorCode;			// 成功0，否则就是失败，查错误码
	INT64				n64FairyID;	// 妖精序号，背包中的妖精
	EElementInjury		eEleType;	// 成功后，元素伤害类型，至于伤害值不变
CMD_END

CMD_START( NC_FairyIncEleInjury )
	BYTE			byType;			// 待提升的类型
	BYTE			bAttType;		// 元素类型 0伤害 1穿透 2抵抗
	INT64			n64Fruit1;		// 妖精果实1
	INT64			n64Fruit2;		// 妖精果实2
	INT64			n64Fruit3;		// 妖精果实3
	INT64			n64VerityOfStone;	// 真理之石
CMD_END
CMD_START( NS_FairyIncEleInjury )
	DWORD			dwErrorCode;
	INT				nAdded;
CMD_END
// 增加妖精可提升次数 [3/12/2012 zhangzhihua]
CMD_START(NC_FairyAddPromoteTimes)
	BYTE			byType;			// 1:伤害 2:抵抗 [3/12/2012 zhangzhihua]
CMD_END
CMD_START(NS_FairyAddPromoteTimes)
	DWORD			dwErrorCode;
CMD_END
// 妖精特技确认
CMD_START( NC_FairyStuntConfirm )
	BOOL		bConfirmOrNot;		// 是否确认
CMD_END
CMD_START( NS_FairyStuntConfirm )
	BOOL		bConfirmOrNot;		// 是否确认，传回这个参数
	DWORD		dwErrorCode;
CMD_END

// 妖精特技书学习
CMD_START( NC_FairyStuntBook )
	INT64		n64StuntBook;	// 技能书
CMD_END
CMD_START( NS_FairyStuntBook )
	DWORD		dwErrorCode;
CMD_END

// 妖精重生
CMD_START( NC_FairyNirvana )
	INT64		n64NirvanID;	// 重生物品
CMD_END
CMD_START( NS_FairyNirvana )
	DWORD		dwErrorCode;
CMD_END

// 神石
CMD_START( NC_RockStone )
	INT64		n64RockID;
	INT64		n64EquipID;
CMD_END
CMD_START( NS_RockStone )
	DWORD dwErrorCode;
CMD_END

// 装备成长
CMD_START( NC_EquipGrow )
	INT64		n64Equip;
	DWORD		dwGrowValue;	//成长度
	DWORD		dwIM; // 幸运符
CMD_END
CMD_START( NS_EquipGrow )
	DWORD dwErrorCode;
CMD_END

// 装备成长属性转移
CMD_START(NC_EquipGrowTrans)
	INT64 n64OldEquip;
	INT64 n64NewEquip;
	INT64 n64IM;
CMD_END
CMD_START(NS_EquipGrowTrans)
	DWORD dwErrorCode;
CMD_END

// 装备成长退星
CMD_START(NC_EquipGrowDec)
	INT64 n64Equip;
	INT64 n64IM;
CMD_END
CMD_START(NS_EquipGrowDec)
	DWORD dwErrorCode;
CMD_END

CMD_START(NC_SoulCrystalGrow)
	INT64	n64Serial;
	INT64	n64IMSerial;
CMD_END

CMD_START(NS_SoulCrystalGrow)
	DWORD dwErrorCode;
CMD_END

//装备完美成长 一下长满10颗紫星~
CMD_START(NC_EquipPerfectGrow)
	INT64 n64IMSerialID;
	INT64 n64EquipID;
CMD_END

CMD_START(NS_EquipPerfectGrow)
	DWORD dwErrorCode;
CMD_END

//吸完美成长符 
CMD_START(NC_XiPurpleStar)
	INT64 n64IMID;
	INT64 n64EquipID;
CMD_END

CMD_START(NS_XiPurpleStar)
	DWORD dwErrorCode;
CMD_END

// 魂晶改造
#define SoulCrystal_Renew_Item_1 4820083
#define SoulCrystal_Renew_Item_2 4820084
CMD_START(NC_SoulCrystalRenew)
	INT64	n64Serial;
CMD_END

CMD_START(NS_SoulCrystalRenew)
	DWORD dwErrorCode;
CMD_END

CMD_START(NC_DiamondEquipGrowIM)
	INT64 n64ItemID;	//道具id
	INT64 n64EquipID;	//装备id
CMD_END

CMD_START(NS_DiamondEquipGrowIM)
	DWORD dwErrorCode;
CMD_END

CMD_START(NC_HolyStrength)
	INT64 n64ItemID;
CMD_END

CMD_START(NS_HolyStrength)
	DWORD dwErrorCode;
CMD_END

CMD_START(NC_HolyEquipmentNumber)
INT64 n64ItemID;
CMD_END

CMD_START(NS_HolyEquipmentNumber)
DWORD dwErrorCode;
CMD_END

#pragma pack(pop)
		




