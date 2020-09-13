#pragma once

#include "RoleDefine.h"
#include "container_define.h"
#include "../WorldDefine/skill_define.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/reputation.h"
#include "specfunc_define.h"
#include "../WorldDefine/container_define.h"


#pragma pack(push, 1)

class ItemScript;

//-----------------------------------------------------------------------------
// 常量定义
//-----------------------------------------------------------------------------
const INT MAX_ITEM_TYPE_NUM				= 100000;	// 静态表中，物品装备等最多条数

const INT MAX_ROLEATT_BASE_EFFECT		= 6;	// 装备原型中可填写的影响属性个数

const INT MAX_EQUIPATT_BASE_EFFECT		= 7;	// 圣纹原型中可填写的影响圣灵属性个数

const INT MAX_POSY_TIMES				= 20;	// 一件装备可以最多的铭文次数
const INT MAX_ROLEATT_POSY_EFFECT		= 4;	// 一件装备中铭文最多影响的属性个数

const INT MAX_ENGRAVE_TIMES				= 20;	// 一件装备可以最多的镌刻次数
const INT MAX_ROLEATT_ENGRAVE_EFFECT	= 4;	// 一件装备中镌刻最多影响的属性个数

const INT MAX_ROLEATT_ENHANCE_EFFECT	= 3;	// 强化装备的材料影响的属性个数
const INT MAX_EQUIPHOLE_NUM				= 7;	// 一件装备可以最多的镶嵌孔数数

const INT MAX_BRAND_LEVEL				= 10;	// 装备的烙印等级上限

const INT MAX_LONGHUN_LEVEL				= 10;	// 龙魂等级上限


const INT ABRASION2USETIMES				= 25;	// 武器使用次数与崭新度进率.1点崭新度 = 使用50次

const INT MAX_ITEM_DCTIME				= 7 * 24 * 60 * 60;	// 物品最长冷却时间(单位：秒)

const INT MAX_EQUIP_ATTACH_NUM          = 20;    // 装备挂载变形模型最大数量

const INT MAX_PEREQUIP_SUIT_NUM			= 3;	// 一件装备最多可属于3套套装

const INT MAX_EQUIP_BUFF_NUM			= 5;	// 装备上带有buff最大个数

const INT MAX_ROLEATT_SPIN_EFFECT		= 3;	// 精纺对人物属性的影响

const INT MAX_CASTING_PARAM_NUM			= 5;	// 熔铸参数最大个数，材料和角色属性个数

const INT MAX_FCONSOLIDATE_NUM		= 5;	// F计划装备最多强化属性影响

const INT MIN_STORE_EXP_FABAO_LEVEL = 40;  // 可以激活妖精经验封印的最小级别

const INT MIN_Sprite_Born_LEVEL = 40;  // 可以激活妖精生育的最小级别

//const INT EQUIP_GROW_STAR_MAX_NUM = 7; // 装备成长各阶段对应星星的品级

const INT EQUIP_GROW_ADD_ATT_MAX_NUM = 5; // 装备成长影响的属性的最大个数

const INT EQUIP_GROW_MAX_LEVEL = 11; // 装备成长的最高等级

#define MFaBaoStoredMaxExp(Level, Intelligence) (200000.0f*((Level/20.0f)*(Level/20.0f))*((Intelligence/50.0f)*(Intelligence/50.0f)))

#define MAX_LIMITED_ITEM_PROTO	100	// 全服稀有物品的最大数量

const INT SOUL_CRYSTAL_ATT_MAX_NUM = 8; // 魂晶成长能力最大个数 

const INT SOUL_CRYSTAL_MAX_LVL = 10; // 魂晶的最大等级
//元神冲星等级上限
const INT8 MAX_STARENHANCE_LEVEL	= 15;	//最大冲星等级
//元神镶嵌加入老君仙露后提升数值
const INT8 IMPROVE_SOULINLAY_RATE	= 10;

const INT		MAX_ROLE_HOLY_VALUE					= 600;	//角色圣灵值上限
const INT		HOLY_MIN_VALUE						= 100;	// 角色可以召唤圣灵所需的最小圣灵值

const INT		MAX_HOLY_NUM   = 10;		// 角色身上的最大圣灵数量

const INT		MAX_HOLYEAT_COUNT   = 5;		// 圣灵每天吞噬妖精的最大次数


const INT		UNEQUIP_HOLY_COST_SILVER   = 20*10000;			// 圣灵洗印消耗金币数
const INT		ENHANCE_HOLY_EQUIP_COST_SILVER   = 20*10000;		// 圣纹充能消耗金币数
const INT		COST_HOLY_ADD   = 10;						// 圣纹基础属性充能，增加的灵能消耗值

const INT		MIN_FABAO_LEVEL   = 40;		// 吞噬妖精的最小等级
//-----------------------------------------------------------------------------
// 枚举定义
//-----------------------------------------------------------------------------
enum EItemType
{
	EIT_Null		= 0,
	EIT_Expend		= 1,	// 消耗品
	EIT_Unexpend	= 2,	// 非消耗品
	EIT_Spec		= 3,	// 特殊物品
	EIT_Pet			= 4,	// 宠物
	EIT_Bag			= 5,	// 物品包
	EIT_Mall		= 6,	// 商城道具

	EIT_Weapon		= 7,	// 武器
	EIT_ClothArmor	= 8,	// 布甲
	EIT_Decoration	= 9,	// 饰品
	EIT_Fashion		= 10,	// 时装
	EIT_Armor		= 11,	// 铠甲
	// Jason 2010-4-12 v2.0.0
	EIT_FaBao		= 12,	// 法宝
	EIT_NoviceEquip = 13,	// 新手装
	EIT_SoulCrystal = 14,	// 魂晶
	EIT_Holy        = 15,	//圣灵，圣魔里的元神是一种装备，这种装备上能再放装备~
	EIT_HolyEquip	= 16, //元神装备
	//EIT_TransItem  = 16, //变身道具
};

enum EItemTypeEx
{
	EITE_Null		= 0,
	EITE_Resume		= 1,	// 恢复类
	EITE_Advance	= 2,	// 强化类
	EITE_Other		= 3,	// 其他
	EITE_Material	= 4,	// 原材料
	EITE_Capture	= 5,	// 战利品
	EITE_Quest		= 6,	// 任务物品
	EITE_Book		= 7,	// 秘籍
	EITE_Pet		= 8,	// 宠物
	EITE_Ride		= 9,	// 坐骑
	EITE_Expend		= 10,	// 消耗
	EITE_Time		= 11,	// 时限

	EITE_Sword		= 12,	// 剑
	EITE_Blade		= 13,	// 刀
	EITE_Wand		= 14,	// 长兵
	EITE_Bow		= 15,	// 弓 
	EITE_Faqi		= 16,	// 法器
	EITE_Qin		= 17,	// 琴
	// Jason 2010-6-18 补充类型
	EITE_Neck		= 25,	// 项链
	EITE_Finger	= 26,	// 戒指
	EITE_Waist		= 27,	// 腰饰
	EITE_Head		= 28,	// 头部
	EITE_Body		= 29,	// 上身
	EITE_Legs		= 30,	// 下身
	EITE_Hand		= 31,	// 手部
	EITE_Feet		= 32,	// 足部
	EITE_Back		= 33,	// 背部
	EITE_Face		= 34,	// 面部
	EITE_Weapon	= 35,	// 武器
	//
	EITE_CastingAccessories	= 36,		// 熔铸辅料
	EITE_MixingEquipAccessories = 37, // 炼器辅料
	EITE_AdvancedCastingAccessories	= 38,		// 熔铸辅料
	EITE_Cloak		= 39,	// 披风
	EITE_Armor		= 40,	// 防具
	EITE_SoulCrystalGod = 41, // 神魂晶
	EITE_SoulCrystalEvil = 42, // 魔魂晶
	EITE_War			= 43,	// 专属道具 [4/26/2012 zhangzhihua]

// 	EITE_TianHun			= 43,			//天魂		//龙元神装备删除信息
// 	EITE_DiHun				= 44,			//地魂
// 	EITE_MingHun			= 45,			//命魂
// 	EITE_TianChong			= 46,			//天冲
// 	EITE_LingHui			= 47,			//灵慧
// 	EITE_QiPo				= 48,			//气魄
// 	EITE_LiPo				= 49,			//力魄
// 	EITE_ZhongShu			= 50,			//中枢
// 	EITE_JingPo				= 51,			//精魄
// 	EITE_YingPo				= 52,			//英魄

};

enum EItemTypeReserved
{
	EITR_Null		= 0,
	EITR_Yaopin		= 1,	// 药品
	EITR_Shipin		= 2,	// 食品
	EITR_Xiandan	= 3,	// 仙丹
	EITR_Longhun	= 4,	// 龙魂
	EITR_Yinji		= 5,	// 印记
	EITR_Baoshi		= 6,	// 宝石
	EITR_Qizhen		= 7,	// 阵旗
	EITR_Ranliao	= 8,	// 染料
	EITR_Siliao		= 9,	// 饲料
	EITR_Fuzhou		= 10,	// 符咒
	EITR_Kuangshi	= 11,	// 矿石
	EITR_Caoyao		= 12,	// 草药
	EITR_ShiCai		= 13,	// 食材
	EITR_Buliao		= 14,	// 布料
	EITR_Mucai		= 15,	// 木材
	EITR_Longyuan	= 16,	// 龙魂原料
	EITR_Yinyuan	= 17,	// 印记原料
	EITR_BigFaqi	= 18,	// 大型法器
	EITR_SmallFaqi	= 19,	// 小型法器
	EITR_VocationSuit = 20, // 职业套装
	EITR_Spin		= 21,	// 可精纺时装
};

enum EItemQuality
{
	EIQ_Start		= -1,

	EIQ_Null		= -1,	// 未鉴定
	EIQ_White		= 0,
	EIQ_Yellow		= 1,
	EIQ_Green		= 2,
	EIQ_Blue		= 3,
	EIQ_Orange		= 4,
	// Jason 2009-11-30 增加紫色品级
	EIQ_Purple		= 5,
	EIQ_Pink		= 6,

	EIQ_End			= 7,
};

const INT32 X_EQUIP_QUALITY_NUM = EIQ_End - EIQ_Start - 1;

enum EBindMode	// 绑定类型
{
	EBM_Null		= 0,	// 无

	EBM_Gain		= 1,	// 获得绑定
	EBM_BindAfterUse	= 2,	// 使用后绑定
	EBM_AccountBind	= 3,		// 账号绑定
};

enum EBindStatus	// 绑定状态
{
	EBS_Unknown		= 0,		// 未知的，如商店中的商品
	EBS_Unbind		= 1,		// 未绑定
	EBS_Bind		= 2,		// 绑定
	EBS_BindAfterUse = 3,		// 使用后绑定
	EBS_AccountBind	= 4,		// 账号绑定
	EBS_AttachSoulBind	= 5,	// 附魂绑定 （装备被绑定的附魂石作用后，不管原来是绑定还是非绑定都变成附魂绑定）
};

//enum ETimeMode		// 计时方式
//{
//	ETM_Null		= 0,
//	TEM_Create		= 1, // 生成计时
//	TEM_Online		= 2, // 线上计时
//	TEM_Battle		= 3, // 战斗计时
//	TEM_Quest		= 4, // 任务触发计时
//	TEM_Used		= 5, // 使用后计时
//	TEM_Get			= 6, // 获得计时
//};

enum ESexLimit		// 物品可使用的角色性别,要与AvatarAtt中性别对应
{
	ESL_Null		= -1,	// 不限制，即男女均可用
	ESL_Woman		= 0,	// 女限制，即女可用
	ESL_Man			= 1,	// 男限制，即男可用
};

enum EClassLimit
{
	ECL_Sword		= 0x1,	// 剑客
	ECL_JianXian	= 0x2,	// 剑仙
	ECL_XieWu		= 0x4,	// 邪舞
					
	ECL_Blade		= 0x1,	// 刀客
	ECL_BaDao		= 0x2,	// 霸刀
	ECL_GangRen		= 0x4,	// 罡刃
					
	ECL_Wand		= 0x1,	// 杖客
	ECL_ZongZhang	= 0x2,	// 宗杖
	ECL_DiMo		= 0x4,	// 地墨
					
	ECL_Bow			= 0x1,	// 灵羽
	ECL_TianJian	= 0x2,	// 天箭
	ECL_MoLing		= 0x4,	// 魔翎
					
	ECL_Valor		= 0x1,	// 气师
	ECL_XuXing		= 0x2,	// 虚行
	ECL_GuiQi		= 0x4,	// 鬼气
					
	ECL_Supple		= 0x1,	// 玄师
	ECL_XuangWang	= 0x2,	// 玄王
	ECL_MiZong		= 0x4,	// 密宗

	ECL_Apothecary	= 0x1,	// 药师
	ECL_XianYi		= 0x2,	// 仙医
	ECL_DuYin		= 0x4,	// 毒隐

	ECL_Musician	= 0x1,	// 乐师
	ECL_YueSheng	= 0x2,	// 乐圣
	ECL_HuanYin		= 0x4,	// 幻音

	ECL_Stabber		= 0x1,	// 刺客
	ECL_YingWu		= 0x2,	// 影武
	ECL_GuiWu		= 0x4,	// 鬼武
};

enum EWuXing	// 五行属性
{
	EWX_Start		= -1,
	EWX_Null		= -1,

	EWX_Metal		= 0,
	EWX_Wood		= 1,
	EWX_Water		= 2,
	EWX_Fire		= 3,
	EWX_Soil		= 4,

	EWX_End			= 5,
};

// Jason 2010-10-24 元素伤害类型
enum EElementInjury
{
	EEI_Null	= -1,
	EEI_START	= 0,
	EEI_Fire	= 0,	// 火元素
	EEI_Water	= 1,	// 水元素
	EEI_Earth	= 2,	// 土元素
	EEI_Wind	= 3,	// 风元素
	EEI_Bright	= 4,	// 光明元素
	EEI_Dark	= 5,	// 黑暗元素
	EEI_End		= 6
};
// 元素抗性
enum EElementResistance
{
	EER_Null	= -1,
	EER_START	= 0,
	EER_Fire	= 0,	// 火元素抗性
	EER_Water	= 1,	// 水元素抗性
	EER_Earth	= 2,	// 土元素抗性
	EER_Wind	= 3,	// 风元素抗性
	EER_Bright	= 4,	// 光明元素抗性
	EER_Dark	= 5,	// 黑暗元素抗性
	EER_End		= 6
};

const INT32 X_WUXING_TYPE_NUM = EWX_End - EWX_Start - 1;


enum EStuffType		// 材料类型
{
	EST_Null		= 0,	//无
	EST_Stone		= 1,	//石
	EST_Jade		= 2,	//玉
	EST_Iron		= 3,	//铁
	EST_Silver		= 4,	//银
	EST_Gold		= 5,	//金
	EST_ChunXinCao	= 6,	//春辛草
	EST_XiaKuCao	= 7,	//夏枯草
	EST_QiuBaiCao	= 8,	//秋白草
	EST_DongKuiCao	= 9,	//冬葵草
	EST_DongYangCao = 10,	//东阳草
	EST_Flax		= 11,	//麻布
	EST_Leather		= 12,	//皮革
	EST_Cotton		= 13,	//棉布
	EST_SilkOrgandy	= 14,	//素纱
	EST_Silk		= 15,	//丝绸
	EST_Chop		= 16,	//肉块
	EST_Dew			= 17,	//露水
	EST_Grain		= 18,	//谷物
	EST_Bone		= 19,	//骨头
	EST_Potherb		= 20,	//野菜
	EST_IcePith		= 21,	// 冰精
	EST_FirePith	= 22,	// 火精
	EST_WoodPith	= 23,	// 木精
	EST_Consolidate = 24,	// 强化
	EST_Consolidate2 = 25,	// 强化2超凡水晶
	EST_Consolidate3 = 26,	// 强化3神铸强化水晶
	EST_Consolidate4 = 27,	// 强化4圣魔之血
	EST_ChongNeng	= 28,	// 普通充能道具
	EST_PianXiang	= 29,	// 圣灵偏向道具
	EST_LingNeng	= 30,	// 灵能强化道具
};

enum EItemCreateMode	// 物品&装备生产方式
{
	EICM_Null			= 0,	// 没有记录
	EICM_GM				= 1,	// GM命令生成
	EICM_Loot			= 2,	// 怪物掉落
	EICM_Quest			= 3,	// 任务奖励
	EICM_Product		= 4,	// 玩家生产
	EICM_ShopNPC		= 5,	// 商店NPC
	EICM_AccQuest		= 6,	// 接任务
	EICM_Mall			= 7,	// 商城
	EICM_Gift			= 8,	// 礼品包
	EICM_ActTreasure	= 9,	// 激活氏族珍宝
	EICM_GMTool			= 10,	// GM工具生成
	EICM_PetSkill		= 11,	// 宠物技能
	EICM_VipNetBar		= 12,	// 金牌网吧
	EICM_ReGet			= 13,	// 道具恢复
	EICM_Activity		= 14,	// 活动奖励
	EICM_LessingLoong	= 15,	// 神龙赐福
	EICM_GoldStone		= 16,	// 点金石
	EICM_GemRemoval		= 17,	// 宝石拆除
	EICM_ServerMerge	= 18,	// 合服生产
	EICM_CityProduce	= 19,	// 城市产出
	EICM_SpiritReward	= 20,	// 精力系统奖励
	EICM_StrengthReward	= 21,	// 精力系统奖励-实力
	EICM_Spin			= 22,	// 精纺
	EICM_Xihun			= 23,	// 吸魂，产生附魂石
	EICM_GuildDailyReward	= 24,	// 公会每日在线奖励领取
	EICM_ExpPilularBind	= 25,	// 妖精经验封印-水晶球约定
	EICM_GodBid	= 26,	// 神之竞标
	EICM_FairyBirth	= 27,	// 妖精繁殖
	EICM_EggBroker	= 28,	// 砸金蛋
	EICM_PlayerBack = 29,	// 玩家回归奖励
	EICM_AdvancedCompose = 30,	// 高级合成
	EICM_FamilyQuest	= 31,	//家族任务
	EICM_KeyCodeReward	= 32, // 输入Key码领取奖励
	EICM_Zhanghao	= 33, // 账号回购
	EICM_Tiger	= 34, // 老虎机
	EICM_NewGiftBox = 35, //新宝箱
	EICM_FairyFusion = 36,	// 妖精融合 [3/22/2012 zhangzhihua]
	EICM_NewKeyCode = 37,	//新Key码
	EICM_HolySoulEquipInlay=38,//元神装备镶嵌，失败后删除并生成低一级的天影神石
	EICM_HolySoulEquipRemove=39,//元神装备摘除
	EICM_EquipXiStar = 40,		//吸星大法
	EICM_SaveTelNum = 41,		//玩家保存手机号
	EICM_WarReward		= 42,	//守护地图奖励
	EICM_YuanBaoDaiBi	= 43,	//元宝代币购买
	EICM_RecallPet		=44,
	EICM_ReceiveYuanBaoDaiBi = 45,	//通过NPC领取的

	EICM_End,
};

enum EEnhanceEquipPos	// 可强化的装备类型
{
	EEEP_Hand		= 0x00000001,	// 武器
	EEEP_Head		= 0x00000002,	// 头部
	EEEP_Face			= 0x00000004,	// 面部
	EEEP_Body		= 0x00000008,	// 上身
	EEEP_Legs			= 0x00000010,	// 下身
	EEEP_Back			= 0x00000020,	// 背部
	EEEP_Wrist		= 0x00000040,	// 腕部
	EEEP_Feet			= 0x00000080,	// 足部
	EEEP_Finger		= 0x00000100,	// 戒指
	EEEP_Waist		= 0x00000200,	// 腰饰
	EEEP_Neck		= 0x00000400,	// 项链
	EEEP_HunJing	= 0x00000800,	// 魂晶
};

enum EEquipMdlType     // 装备的模型类型
{
	EEMT_NULL	= 0,
	EEMT_FSKIN  = 1,   // 蒙皮
	EEMT_FAK    = 2,   // 关键帧
	EEMT_FSM    = 3,   // 静态模型
};

enum EEquipAttachPos    // 装备可变形部分的挂载位置
{
	EEAP_NULL   = 0,
    EEAP_haa	= 1,	// 头
	EEAP_chm	= 2,	// 前胸
	EEAP_baa	= 3,	// 后背
	EEAP_bab	= 4,	// 腰
	EEAP_shb	= 5,	// 左肩
	EEAP_sha	= 6,	// 右肩
	EEAP_shl	= 7,	// 左上臂1
	EEAP_shx	= 8,	// 左上臂2
	EEAP_shr	= 9,	// 右上臂1
	EEAP_shu	= 10,	// 右上臂2
	EEAP_chg	= 11,	// 左摆1
	EEAP_chh	= 12,	// 左摆2
	EEAP_chi	= 13,	// 左摆3
	EEAP_chd	= 14,	// 右摆1
	EEAP_che	= 15,	// 右摆2
	EEAP_chf	= 16,	// 右摆3
	EEAP_chj	= 17,	// 后甲1
	EEAP_chk	= 18,	// 后甲2
	EEAP_chl	= 19,	// 后甲3
	EEAP_chp	= 20,	// 臀
	EEAP_lla	= 21,	// 左小腿
	EEAP_lra	= 22,	// 右小腿
	EEAP_fla	= 23,	// 左脚
	EEAP_fra	= 24,	// 右脚
	EEAP_gla	= 25,	// 左小臂
	EEAP_gra	= 26,	// 右小臂
	EEAP_bal	= 27,	// 轮
	/// hdf [20100919] add two 翅膀
	EEAP_bax	= 28,
	EEAP_bay	= 29,
	EEAP_Num	= 30	///28
};

enum EEquipSpecAtt		// 装备B类属性
{
	EESA_Null				= -1,
	EESA_Start				= 0,

	EESA_LevelLim_Simple	= 0,	// 简易		该装备等级限制-5，最低可减少至0
	EESA_LevelLim_Fine		= 1,	// 精简		该装备等级限制-10，最低可减少至0
	EESA_LevelLim_None		= 2,	// 无级别	该装备无等级限制

	EESA_AttALim_Simple		= 3,	// 轻便		该装备属性限制减少10%，取整
	EESA_AttALim_Comfort	= 4,	// 舒适		该装备属性限制减少25%，取整
	EESA_AttALim_Light		= 5,	// 轻盈		该装备属性限制减少50%，取整

	EESA_Potential_YinFeng	= 6,	// 隐凤		该装备的初始潜力值+200
	EESA_Potential_YinHuang	= 7,	// 隐凰		该装备的初始潜力值+400
	EESA_Potential_FeiFeng	= 8,	// 飞凤		该装备的初始潜力值+800
	EESA_Potential_MingHuang= 9,	// 鸣凰		该装备的初始潜力值+1200
	EESA_Potential_WoLong	= 10,	// 卧龙		装备的初始潜力值提高5%
	EESA_Potential_CangLong	= 11,	// 藏龙		装备的初始潜力值提高10%
	EESA_Potential_FuLong	= 12,	// 伏龙		装备的初始潜力值提高20%
	EESA_Potential_ShengLong= 13,	// 升龙		装备的初始潜力值提高30%

	EESA_EquipSpec_RelEnd	= 13,	// 

	EESA_Guard_Posy			= 14,	// 铭纹守护	装备在进行铭纹强化时不会失败
	EESA_Guard_Brand		= 15,	// 烙印守护	装备在进行烙印强化时不会失败
	EESA_Guard_Engrave		= 16,	// 镌刻守护	装备在进行镌刻强化时不会失败
	EESA_Guard_LongFu		= 17,	// 龙附守护	装备在进行龙附强化时不会失败
	EESA_Guard_Quench		= 18,	// 淬火守护	装备在进行淬火强化时不会失败
	EESA_Guard_Cut			= 19,	// 开凿守护	装备在进行开凿强化时不会失败
	EESA_Guard_AllProduce	= 20,	// 绝世		装备在进入任何强化时不会失败

	EESA_Guard_Talisman		= 21,	// 避邪		进行装备强化时成功率增加[增加5%]
	EESA_Guard_Fortune		= 22,	// 幸运		进行装备强化时完美率增加[增加5%]

	EESA_Guard_DeadPunish	= 23,	// 死亡守护	该装备不会在死亡时掉落

	EESA_End				= 23,
};

const INT X_EQUIP_SPECATT_NUM	= EESA_End - EESA_Start + 1;

enum ECorlor		// 颜色贴图枚举
{
	EC_Null		= -1,

	EC_Start	= 0,
	EC_Val0		= 0,
	EC_Val1		= 1,
	EC_Val2		= 2,
	EC_Val3		= 3,
	EC_Val4		= 4,
	EC_Val5		= 5,
	EC_Val6		= 6,
	EC_Val7		= 7,
	EC_Val8		= 8,
	EC_Val9		= 9,
	EC_Val10	= 10,
	EC_Val11	= 11,
	EC_End		= 11,
};

enum EPickupType
{
	EPT_Money = 0,			// 钱
	EPT_Equip = 1,			// 装备
	EPT_Medicine = 2,		// 药水
	EPT_StrengthenStuff = 3,	// 强化材料
	EPT_Gem = 4,				// 宝石圣印
	EPT_Quest = 5,			// 任务活动
	EPT_Sprite = 6,			// 妖精相关
	EPT_Others = 7,			// 其他物品
};

// 丢弃类型
enum EDiscardEvent
{
	EDE_Null = -1,
	EDE_DiscardGround	= 0, // 掉地上
	EDE_DiscardDestory	= 1, // 丢弃销毁
	EDE_DiscardNot		= 2, // 不能丢弃
};

const INT X_COLOR_NUM	= EC_End - EC_Start + 1;

// Jason 2010-4-13 v2.0.0
// 法宝类型
enum ENUM_FabaoType 
{
	EFB_NULL	= -1,
	EFB_Attack	= 0, // 攻击型法宝
	EFB_Defense	= 1, // 防御型法宝
	EFB_Assit	= 2, // 辅助型法宝
	EFB_END
};

// 战斗符
enum EAutoFightSpecFuncVal2
{
	EAFSFV2_Normal = 0,	// 普通版本
	EAFSFV2_OMG = 1,	// 台湾OMG版本
};

// 装备成长星
enum EEquipGrowStar
{
	EEGS_Null		= 0,
	EEGS_Gray	= 1,
	EEGS_Green	= 2,
	EEGS_Blue	= 3,
	EEGS_Purple	= 4,

	EEGS_End = 5,
};
#define IsGrowStarValid(star) (star > EEGS_Null && star < EEGS_End)

struct tagFabao;

//-----------------------------------------------------------------------------
// 结构定义
//-----------------------------------------------------------------------------
struct tagRoleAttEffect
{
	ERoleAttribute	eRoleAtt;
	INT32			nValue;

	tagRoleAttEffect() { ZeroMemory(this, sizeof(*this)); eRoleAtt = ERA_Null; }
};

struct tagHolyAttEffect
{
	EHolyAttType	eHolyAtt;
	INT32			nValue;

	tagHolyAttEffect() { ZeroMemory(this, sizeof(*this)); eHolyAtt = ESAT_NULL; }
};

struct tagEquipQltyPct
{
	DWORD				dwTypeID;
	INT32				nEquipQltyPct[X_EQUIP_QUALITY_NUM];	// 品级生成几率
};

struct tagCDTime
{
	DWORD		dwTypeID;
	DWORD		dwTime;		// 冷却时间(单位：毫秒)
};

/** struct	tagItemDisplayInfo
brief	物品显示结构
remarks	主要用于存储物品的名称、类型、描述、模型等等	
*/
struct tagItemDisplayInfo
{
	TCHAR				szType[X_SHORT_NAME];				// 显示类型
	TCHAR				szName[X_SHORT_NAME];				// 物品名称
	TCHAR				szDesc[X_HUGE_NAME];				// 物品描述
	TCHAR				szDescEx[X_HUGE_NAME];				// 物品特性描述
	TCHAR				szMaleIcon[X_SHORT_NAME];			// 物品图标男版
	TCHAR				szFemaleIcon[X_SHORT_NAME];			// 物品图标女版
	TCHAR				szActionMod[X_SHORT_NAME];			// 物品使用动作编号
	TCHAR				szEffectMod[X_SHORT_NAME];			// 物品使用特效编号
	TCHAR				szSourceEffect[X_SHORT_NAME];		// 物品使用发起者特效编号
	TCHAR				szDestEffect[X_SHORT_NAME];			// 物品使用目标特效编号
	TCHAR				szPrepareMod[X_SHORT_NAME];			// 物品使用起手动作编号
	TCHAR				szMaleRightMod[X_SHORT_NAME];		// 男模型编号1
	TCHAR				szFemaleRightMod[X_SHORT_NAME];		// 女模型编号1
	TCHAR				szMaleLeftMod[X_SHORT_NAME];		// 男模型编号2
	TCHAR				szFemaleLeftMod[X_SHORT_NAME];		// 女模型编号2
	TCHAR				szDropMod[X_SHORT_NAME];			// 掉落模型编号
	TCHAR				szPertainMod[X_SHORT_NAME];			// 附属模型编号
	TCHAR				szBladeFlareSFX[X_SHORT_NAME];		// 刀光特效编号
	TCHAR				szBladeGlowSFX[X_SHORT_NAME];		// 光晕特效编号
	TCHAR				szAttachSFX[X_SHORT_NAME];			// 龙魂挂载特效编号
	TCHAR				szBladeBornSFX[X_SHORT_NAME];		// 武器自带特效编号
	EWeaponPos			ePertainModPos;						// 附属模型挂载位置
	EEquipMdlType       eModelType;							// 模型类型
	EEquipAttachPos     eAttachModelPos[MAX_EQUIP_ATTACH_NUM];  // 挂载变形模型的位置
	BOOL				bActionBlend;						// 是否上下身分开
	
	tagItemDisplayInfo(){ZeroMemory(this, sizeof(*this));}
};

struct tagItemProto
{
	DWORD				dwTypeID;		// 基本物品种类编号(同种物品相同)

	EItemType			eType;			// 基本类性
	EItemTypeEx			eTypeEx;		// 扩展类型
	EItemTypeReserved	eTypeReserved;	// 保留类型

	EStuffType			eStuffType;		// 材料类型，合成用

 	DWORD				dwQuestID;		// 任务编号
	INT32				nBasePrice;		// 基础价格

	INT32				nMaxUseTimes;	// 最多使用次数
	INT16				n16MaxLapNum;	// 最大叠放数量
	INT16				n16MaxHoldNum;	// 最大拥有数量

	DWORD				dwTimeLimit;	// 有时限物品(秒)

	INT16				n16Enmity;		// 仇恨

	BYTE				byMinUseLevel;	// 等级限制
	BYTE				byMaxUseLevel;	// 等级上限
	ESexLimit			eSexLimit;		// 性别限制
	DWORD				dwVocationLimit;// 职业限制
	// Jason 2010-5-18 v2.1.2
	BOOL				bSoarLimit;		// 飞升限制

	EReputationType		eClanRepute;	// 氏族声望类型值
	INT32				nClanReputeVal;	// 氏族声望值
	EReputationType		eOtherClanRepute;	// 其他声望类型值
	INT32				nOtherClanReputeVal;// 其他声望值

	EItemSpecFunc		eSpecFunc;		// 物品特殊功用类型
	INT32				nSpecFuncVal1;
	INT32				nSpecFuncVal2;

	ESkillOPType		eOPType;							// 作用类型
	FLOAT				fOPDist;							// 作用距离（世界单位）
	FLOAT				fOPRadius;							// 作用范围（世界单位）
	INT32				nPrepareTime;						// 起手时间（毫秒）
	DWORD				dwCooldownTime;						// 技能冷却时间(单位：ms)
	INT32				nInterruptSkillOrdRate;				// 普通技能打断几率[0,10000]
	DWORD				dwSpecBuffLimitID;					// 自身BUFF限制ID
	DWORD				dwStateLimit;						// 自身状态标志位限制
	DWORD				dwTargetLimit;						// 对象限制
	BOOL				bMoveable;							// 是否可以移动使用			
	bool				bInterruptMove;						// 是否移动打断

	BYTE				byLevel;			// 物品等级
	BYTE				byQuality;			// 物品品级

	BYTE				byBindType;			// 绑定类型

	DWORD				dwTriggerID0;		// 触发条件
	DWORD				dwBuffID0;			// Buff
	
	DWORD				dwTriggerID1;		// 触发器ID1
	DWORD				dwBuffID1;			// 状态ID1

	DWORD				dwTransTriggerID;	// 变形触发类型ID
	DWORD				dwTransID;			// 变形后ID

	bool				bNeedBroadcast;		// 是否广播(0，不广播；1，广播)
	bool				bCanSell;			// 是否不可出售(0，可出售；1，不可出售)

	bool				bNeedLog;			// 是否需要做log(该属性有运营通过数据表item_needlog编辑)
	BYTE				byLogMinQlty;		// 记录log物品的最低品级

	bool				bDeadLoot;			// 死亡掉落
	EDiscardEvent		DiscardEvent;		// "DiscardEvent"	0\1\2
	EPickupType			ePickType;			// 拾取类型（自动拾取筛选使用）

	bool				bCanMove2Ware;		// 是否可存入仓库 0-可以存，1-不可以存

	tagItemDisplayInfo*	pDisplayInfo;		// 物品显示结构，客户端用
	
	tagItemProto(){ZeroMemory(this,sizeof(*this));}
	tagItemProto(DWORD dwID){ZeroMemory(this,sizeof(*this)); dwTypeID = dwID;}
	INT16			GetMaxLapNum() const
	{
		if (n16MaxLapNum <= 0)
		{
			return 1;
		}

		return n16MaxLapNum;
	}
};

struct tagEquipProto: public tagItemProto
{
	DWORD				dwSuitID[MAX_PEREQUIP_SUIT_NUM];		// 套装编号
	BYTE				bySuitMinQlty[MAX_PEREQUIP_SUIT_NUM];	// 最低品级要求
	bool				bCanDye;								// 时装是否可染色
	bool				bCanGrow;	// 装备是否具有成长属性

	INT16				n16AttALimit[X_ERA_ATTA_NUM];			// 属性限制
	
	DWORD				dwVocationLimitWear;					// 穿戴职业限制
	DWORD				dwCondenseLimit;						// 神格限制

	EEquipPos			eEquipPos;		// 装备位置
	EWeaponPos			eWeaponPos;		// 武器位置

	INT16				n16WuHun;		// 内功伤害计算用
	INT16				n16MinDmg;		// 武器最小伤害
	INT16				n16MaxDmg;		// 武器最大伤害
	INT16				n16Armor;		// 防具护甲

	INT32				nMaxPotVal;		// 装备最大潜力值
	INT32				nPotVal;		// 装备潜力值
	// Jason 2009-12-1 装备潜力值提升次数
	INT32				nPotIncTimes;	// 潜力值提升剩余次数，注意这个值是递减的。

	tagRoleAttEffect	BaseEffect[MAX_ROLEATT_BASE_EFFECT];// 加工前，影响的属性(和品级无关)
	BYTE				byLongfuAtt[X_WUXING_TYPE_NUM];		// 龙魂属性,即五行

	//BYTE				byClassLimit[X_ClASS_TYPE_NUM];		// 职业限制(指定职业可用)//??

	DWORD				dwTriggerID2;		// 触发器ID1
	DWORD				dwBuffID2;			// 状态ID1
	
	DWORD				dwColor;			// 颜色

	INT16				n16Newness;			// 崭新度 -- 武器用

	bool				bIdentifyProduct;	// 生产鉴定状态
	bool				bIdentifyLoot;		// 掉落鉴定状态

	INT				nInitConsolidateLevel;		// 初始装备强化等级
	INT				nMaxConsolidateLevel;	// 装备最高强化等级限制

	// 神铸初始等级，默认-1
	INT				nBornLevelOfGodStrengthen;
	bool			bSuckLimit;				// 装备是否可以被吸魂，1表示不可吸魂，0表示可以吸魂
	bool			bOnlyScriptStrengthen;		// 只有脚本可以强化
	bool			bMapLimit;				//是否地图限制

	tagEquipProto(){ZeroMemory(this,sizeof(*this));}
	tagEquipProto(DWORD dwID){ZeroMemory(this, sizeof(*this)); dwTypeID = dwID;}
};

struct tagSoulCrystalLvlLine
{
	BYTE byLvl;
	DWORD dwAttSoul;
	INT64 n64Money;
};

struct tagSoulCrystalAttData
{
	INT nID;
	BYTE byLevel;
	ERoleAttribute eAttType;
	DWORD dwAttValue;
};

struct tagEquipGrowProto
{
	DWORD dwID;
	DWORD dwExp[EQUIP_GROW_MAX_LEVEL];

	ERoleAttribute eAtt[EQUIP_GROW_ADD_ATT_MAX_NUM]; // 装备的成长属性
	BYTE byAttOpenLevel[EQUIP_GROW_ADD_ATT_MAX_NUM];	// 成长属性开启的条件(装备成长到几级时候开启)
	DWORD dwAttValue[EQUIP_GROW_ADD_ATT_MAX_NUM][EQUIP_GROW_MAX_LEVEL]; // 成长属性在各级时的最大值
};

struct tagEquipGrowName : tagEquipGrowProto
{
	BYTE byStrength;	//属性强度
};

#define MEquipMaxUseTimes(pEquipProto)	((pEquipProto)->n16Newness * ABRASION2USETIMES)

struct tagGemProto		// 强化物品原型
{
	DWORD				dwTypeID;		// 物品原型编号

	INT16				n16PotValNeed;	// 需要消耗的装备潜力值
	INT16				n16SuccessPct;	// 成功率

	EWuXing				eWuXing;		// 龙魂，即五行属性(-1表示该物品不是龙魂)
	tagRoleAttEffect	BaseEffect[MAX_ROLEATT_ENHANCE_EFFECT];	// 装备强化影响

	INT32				nEnhanceFlg;	// 按位记录 -- EEnhanceEquipPos

	tagGemProto() { ZeroMemory(this, sizeof(*this)); }
};

struct tagItem
{
	INT64			n64Serial;		// 物品唯一序列号(不同堆叠物品不同)
	DWORD			dwTypeID;		// 基本物品种类编号(同种物品相同)

	INT16			n16Num;			// 物品个数
	bool			bLock	: 1;	// 锁定标志
	bool			bUsing	: 1;	// 是否在对该物品进行其他操作
	BYTE			byBind;			// 绑定状态
	INT32			nUseTimes;		// 物品上技能使用次数

	DWORD			dwAccountID;	// 玩家账号ID, 百宝袋和角色仓库用(同一帐号下角色共享)
	DWORD			dwOwnerID;		// 拥有者, GT_INVALID表示没有拥有者

	EItemConType	eConType;		// 物品所在容器类型
	INT16			n16Index;		// 物品在容器中的位置
	INT16			n16Dummy;		// 内存对齐用

	EItemCreateMode	eCreateMode;	// 物品&装备生成方式
	DWORD			dwCreateID;		// 外部生成该物品的ID，如：GMID，QuestID等, 界面上不显示
	
	DWORD			dwCreatorID;	// 物品制作者，GT_INVALID表示系统生产, 界面上显示
	tagDWORDTime	dwCreateTime;	// 制作物品的时间

	DWORD			dwNameID;		// 珍宝名字ID

	mutable tagDWORDTime	dw1stGainTime;	// 第一个玩家获得时间(因为战斗符，所以这里改为mutable)

	EUpdateDBStatus	eStatus;		// 记录物品是否需要进行数据库操作
	
	DWORD			dwUnlockTime;	// 真正的解锁时间

	DWORD			dwSpecVal1;	// 物品特殊功能字段值1，动态属性，受特定条件限制
	DWORD			dwSpecVal2;	// 物品特殊功能字段值2

	union
	{
		const tagItemProto	*pProtoType;	// 指向物品基本属性
		const tagEquipProto	*pEquipProto;	// 执行装备基本属性
	};

	const ItemScript		*pScript;		// 脚本指针
	
	tagItem() { ZeroMemory(this, sizeof(*this)); }
	VOID SetOwner(DWORD dwNewOwnerID, DWORD dwNewAccountID) { dwOwnerID = dwNewOwnerID; dwAccountID = dwNewAccountID; }
	VOID SetPos(EItemConType eNewCon, INT16 n16NewIndex) { if(TRUE == SetConType(eNewCon)) n16Index = n16NewIndex; }
	VOID SetIndex(INT16 n16NewIndex) { n16Index = n16NewIndex; }
	VOID SetUsing(bool bUsingState) { bUsing = bUsingState; }
	VOID SetUpdate(EUpdateDBStatus eNewUpdateStatus) { eStatus = eNewUpdateStatus; }
	INT64 GetKey() const { return n64Serial; }
	VOID SetNum(INT16 n16Val)
	{
		if (!P_VALID(pProtoType))
		{
			return;
		}
		if (n16Val > pProtoType->n16MaxLapNum)
		{
			n16Val = pProtoType->n16MaxLapNum;
		}
		if (n16Val < 0)
		{
			n16Val = 0;
		}
		n16Num = n16Val;
	}
	VOID AddNum(INT16 n16Val)
	{
		n16Num += n16Val;
		if (!P_VALID(pProtoType))
		{
			return;
		}
		if (n16Num > pProtoType->n16MaxLapNum)
		{
			n16Num = pProtoType->n16MaxLapNum;
		}
		if (n16Num < 0)
		{
			n16Num = 0;
		}
	}
	INT16 GetNum() const
	{
		if (n16Num < 0 || !P_VALID(pProtoType))
		{
			return 0;
		}
		return (n16Num > pProtoType->n16MaxLapNum) ? pProtoType->n16MaxLapNum : n16Num;
	}
	BOOL CanOverlap() const { return (P_VALID(pProtoType) ? pProtoType->n16MaxLapNum > 1 : FALSE); }
	VOID IncUseTimes(INT32 nUse) { nUseTimes += nUse; SetUpdate(EUDBS_Update); }
	BOOL SetConType(EItemConType eNewCon)
	{
		// 不能将非装备物品装备到身上 [4/13/2012 zhangzhihua]
		if (EICT_Equip == eNewCon && !MIsEquipment(dwTypeID))
		{
			return FALSE;
		}

		eConType = eNewCon;
		return TRUE;
	}

	// 得到某个索引对应的BuffID
	DWORD GetBuffID(INT nIndex)
	{
		switch(nIndex)
		{
		case 0:
			return pProtoType->dwBuffID0;
		case 1:
			return pProtoType->dwBuffID1;
		case 2:
			return pProtoType->dwTransID;
		default:
			return GT_INVALID;
		}
	}

	// 得到某个索引对应的TriggerID
	DWORD GetTriggerID(INT nIndex)
	{
		switch(nIndex)
		{
		case 0:
			return pProtoType->dwTriggerID0;
		case 1:
			return pProtoType->dwTriggerID1;
		case 2:
			return pProtoType->dwTransTriggerID;
		default:
			return GT_INVALID;
		}
	}

	VOID Bind()
	{
		ASSERT(P_VALID(pProtoType));
		switch(pProtoType->byBindType)
		{
		case EBM_Null:
			byBind = EBS_Unbind;
			break;
		case EBM_Gain:
			byBind = EBS_Bind;
			break;
		case EBM_AccountBind:
			byBind = EBS_AccountBind;
			break;
		//case EBM_BindAfterUse:
		//	byBind = EBS_BindAfterUse;
		//	break;
		}
	}
	// 注：OMG战斗符专用
	VOID BindOMGAutoFightItem()
	{
		byBind = EBS_Bind;
	}
	// 使用后绑定
	VOID BindAfterUse()
	{
		if(EBM_BindAfterUse == pProtoType->byBindType)
		{
			byBind = EBS_Bind;
			eStatus = EUDBS_Update;
		}
	}

	BOOL IsBind() const { return EBS_Bind == byBind || EBS_AttachSoulBind == byBind; }

	BOOL IsAccountBind() const { return EBS_AccountBind == byBind; }

	BOOL IsAttachSoulBind() const { return EBS_AttachSoulBind == byBind; }

	BOOL IsLevelLimit(BYTE byRoleLevel) const
	{
		return pProtoType->byMinUseLevel > byRoleLevel || pProtoType->byMaxUseLevel < byRoleLevel;
	}

	BOOL IsTimeLimit() const { return pProtoType->dwTimeLimit != GT_INVALID; }

	EBindMode GetBindMode() const 
	{
		if(P_VALID(pProtoType))
		{
			return (EBindMode)pProtoType->byBindType;
		}
		return EBM_Null;
	}

};

struct tagEquipSpec
{
	// 神铸前后，强化等级上限
	enum{ ECL_BefConMaxLevel = 20,ECL_AftConMaxLevel = 30};
	/******装备鉴定时确定******/
	INT16				n16WuHun;		// 内功伤害计算用
	INT16				n16MinDmg;		// 武器最小伤害
	INT16				n16MaxDmg;		// 武器最大伤害
	INT16				n16Armor;		// 防具护甲

	INT32				nPotVal;		// 装备潜力值(原潜力值+道具提升值)
	INT32				nPotValUsed;	// 已经消耗的潜力值
	// Jason 2009-12-1 装备潜力值提升次数
	INT32				nPotIncTimes;	// 潜力值提升剩余次数，注意这个值是递减的。
	// Jason 2009-12-2 增加紫色装备鉴定几率
	DWORD				dwPurpleQltyIdfPct;

	// 一级属性
	INT32				nRoleAttEffect[X_ERA_ATTA_NUM];
	// 二级属性，进阶属性（从原型表填的属性里面随机）
	//tagRoleAttEffect	BaseEffect[MAX_ROLEATT_BASE_EFFECT];//
	// 二级属性，跟策划协商后，按顺序影响角色属性，这里只需要一个二级属性的个数就可以了，其他的如属性类型以及值，从原型表里面读取。
	INT32				nRoleL2AttCount;
	// 强化等级，有退级原则
	BYTE				byConsolidateLevel;

	// 表,里龙魂能力(GT_INVALID表示没有)
	DWORD				dwLongHunInnerID;
	DWORD				dwLongHunOuterID;

	// 特殊属性
	BYTE				bySpecAtt;

	// 使用等级限制(便于外部因素作用)
	BYTE				byMinUseLevel;
	BYTE				byMaxUseLevel;

	// 品级
	BYTE				byQuality;

	/******时装生成时确定******/
	INT16				n16Appearance;	// 仪容
	BYTE				byRein;			// 统御
	BYTE				bySavvy;		// 悟性
	BYTE				byFortune;		// 福缘
	INT8				n8ColorID;		// 颜色
	INT8				n8Dummy[2];


	/******元神装备冲星等级******/
	BYTE						byStarEnhanceLv;
	//元神装备当前孔数
	BYTE				byHolyHoleNum;

	/******装备强化时修改******/
	bool				bCanCut;		// 是否可开凿
	BYTE				byFlareVal;		// 光晕

	// 原始一级属性(升级自动加点和手动加点部分)限制
	INT16				n16AttALimMod;
	INT16				n16AttALimModPct;	/*10000为基数1*/

	// 龙附，铭文，镌刻，镶嵌，烙印(伤害减免相关属性)
	BYTE				byLongfuLevel;
	BYTE				byPosyTimes;
	BYTE				byEngraveTimes;
	BYTE				byHoleNum;
	BYTE				byBrandLevel;

	BYTE				byLongfuAtt[X_WUXING_TYPE_NUM];
	tagRoleAttEffect	PosyEffect[MAX_ROLEATT_POSY_EFFECT];
	INT32				nEngraveAtt[MAX_ROLEATT_ENGRAVE_EFFECT];
	DWORD				dwHoleGemID[MAX_EQUIPHOLE_NUM];
	INT32				nBrandAtt[X_ERA_BRAND_NUM];

	/******生产装备 -- 品级鉴定修正率******/
	INT16				n16QltyModPct;		// 一级修正率(-10000,10000)
	INT16				n16QltyModPctEx;	// 二级修正率[0,10000)

	/******生产装备 -- 潜力值影响系数******/
	INT16				n16PotValModPct;

	BYTE				bySigned;				// 是否被署名,1为被署名 2为不著名
	DWORD				dwSignatruePlayerID;	// 署名者的ID
	TCHAR				szSignature[11];		// 署名内容
	// Jason 2010-5-18 v2.1.2
	BOOL				bSoaringFlag;
	// 飞升，熔铸属性
	INT					nCastingTimes;							// 熔铸次数
	//ERoleAttribute		eCastingRoleAtt[MAX_CASTING_PARAM_NUM]; // 熔铸成功后，穿装影响属性
	//INT					nCastingAttValue[MAX_CASTING_PARAM_NUM];// 属性值

	// Jason 2010-6-18 v1.0.0强化属性影响，强化表id
	//INT					nConsolidateEffectID;

	BYTE				 byLastGodStrengthenLevel;		// 上次神铸等级
	BYTE				 byGodStrengthenLevel;			// 神铸当前等级
	DWORD				 dwCurGodStrengthenExp;			// 当前等级已获得的神铸经验
	tagDWORDTime		 dwGodStrengthenTime;			// 上次神铸时间	 
	// 计算的值，
	tagRoleAttEffect	ConsolidateEffect[MAX_FCONSOLIDATE_NUM];

	// 装备成长相关属性
	DWORD dwGrowTypeID;	// 成长属性ID
	DWORD dwGrowValue; // 当前成长值
	BYTE byGrowLevel; // 当前成长等级，0-11
	INT nTotalGrowTimes; // 总共成长过的次数
	INT nTotalIMUseTimes; // 总共使用过IM道具的次数
	EEquipGrowStar	eGrowStar[EQUIP_GROW_MAX_LEVEL-1]; // 成长星星的颜色
	INT nGrowAttValue[EQUIP_GROW_ADD_ATT_MAX_NUM]; // 成长属性的值

	// 宝石增幅
	BYTE byDiamondLevel;	//宝石增幅0-10
	
	/******方法******/
	tagEquipSpec() { ZeroMemory(this, sizeof(*this)); }
};

#define MGetEquipGrowRatio(star, Output)	do{ \
	switch(star) \
	{ \
		case EEGS_Gray: \
			Output = 0.5; \
			break; \
		case EEGS_Green: \
			Output = 0.8; \
			break; \
		case EEGS_Blue: \
			Output = 1.0; \
			break; \
		case EEGS_Purple: \
			Output = 1.5; \
			break; \
		default: \
			Output = 0; \
	} \
}while(0)

enum EEquipSignleSpecType
{
	EESSType_Null			= 0, //空
	EESSType_StarEnhance	= 1, //冲星属性
	EESSType_HolyEquipHole	= 2, //凿孔属性
	EESSType_HolyEquipInlay = 3, //镶嵌属性
};

struct tagEquipSingleSpec
{
	INT64		  		 n64Serial;
	EEquipSignleSpecType eEquipSingleSpecType;
	DWORD				 dwVal;
};

struct tagEquip: public tagItem
{
	tagEquipSpec	equipSpec;

	VOID SetFlareVal(BYTE byFlareVal)
	{
		equipSpec.byFlareVal = byFlareVal;
	}

	VOID ChangeFlareVal(BYTE byFlareVal)
	{
		equipSpec.byFlareVal += byFlareVal;
		equipSpec.byFlareVal = (equipSpec.byFlareVal > 100) ? 100 : equipSpec.byFlareVal;
		equipSpec.byFlareVal = (equipSpec.byFlareVal < 0 ) ? 0 : equipSpec.byFlareVal;
	}

	bool ChangeStarEnhanceLvl(BYTE byLvl)
	{
		BYTE byOldVal = equipSpec.byStarEnhanceLv;
		equipSpec.byStarEnhanceLv = byLvl;
		if ( equipSpec.byStarEnhanceLv < 0){ equipSpec.byStarEnhanceLv = 0; }
		if ( equipSpec.byStarEnhanceLv > MAX_STARENHANCE_LEVEL	){ equipSpec.byStarEnhanceLv = MAX_STARENHANCE_LEVEL;}
		if ( byOldVal == equipSpec.byStarEnhanceLv )
		{
			return false;
		}
		return true;
	}

	VOID ChangePotVal(INT nValue)
	{
		INT nOrgPotVal = equipSpec.nPotVal;
		equipSpec.nPotVal += nValue;

		if(equipSpec.nPotVal < 0)		equipSpec.nPotVal = 0;

		if(equipSpec.nPotVal > pEquipProto->nMaxPotVal)
			equipSpec.nPotVal = pEquipProto->nMaxPotVal;

		if(nValue < 0)
			equipSpec.nPotValUsed += (nOrgPotVal - equipSpec.nPotVal);
	}

	VOID IncAttackTimes()
	{		
		++nUseTimes;

		if(eStatus != EUDBS_Update)
		{
			SetUpdate(EUDBS_Update);
		}
	}

	VOID DecAttackTimes(INT nTimes)
	{
		nUseTimes = min(MEquipMaxUseTimes(pEquipProto), nUseTimes);
		nUseTimes = max(0, nUseTimes - nTimes);

		if(eStatus != EUDBS_Update)
		{
			SetUpdate(EUDBS_Update);
		}
	}

	BOOL IsNewessChange(INT &nCurAttackTimes)
	{
		nCurAttackTimes = MEquipMaxUseTimes(pEquipProto);
		if(nUseTimes > nCurAttackTimes)
		{
			nUseTimes = nCurAttackTimes;
			return FALSE;
		}

		nCurAttackTimes = nUseTimes;
		if(nUseTimes % ABRASION2USETIMES == 0)
		{
			return TRUE;
		}

		return FALSE;
	}

	VOID GetTriggerIDBuffID(OUT DWORD &dwTriggerID, OUT DWORD &dwBuffID, IN INT nIndex)
	{
		dwTriggerID = dwBuffID = GT_INVALID;

		if(!P_VALID(pEquipProto))
		{
			ASSERT(0);
			return;
		}

		switch(nIndex)
		{
		case 0:
			dwTriggerID = pEquipProto->dwTriggerID0;
			dwBuffID	= pEquipProto->dwBuffID0;
			break;
		case 1:
			dwTriggerID = pEquipProto->dwTriggerID1;
			dwBuffID	= pEquipProto->dwBuffID1;
			break;
		case 2:
			dwTriggerID = pEquipProto->dwTriggerID2;
			dwBuffID	= pEquipProto->dwBuffID2;
			break;
		default:
			break;
		}
	}

	VOID GetLongHunID(DWORD &dwLongHunID, BOOL &bOuterLongHun) const
	{
		if(equipSpec.dwLongHunOuterID != GT_INVALID)
		{
			bOuterLongHun	= TRUE;
			dwLongHunID		= equipSpec.dwLongHunOuterID;
			return;
		}

		if(equipSpec.dwLongHunInnerID != GT_INVALID)
		{
			bOuterLongHun	= FALSE;
			dwLongHunID		= equipSpec.dwLongHunInnerID;
			return;
		}
	}

	BYTE GetLongfuAtt(INT nWuXingType) const
	{
		return equipSpec.byLongfuAtt[nWuXingType] + pEquipProto->byLongfuAtt[nWuXingType];
	}

	VOID Signature(TCHAR* szContent)
	{
		_tcscpy_s(equipSpec.szSignature, 11, szContent);
	}

	BYTE GetWpnPosyTimes()
	{
		return equipSpec.byPosyTimes;
	}
	BYTE GetWpnEngraveTimes()
	{
		return equipSpec.byEngraveTimes;
	}
	BYTE GetWpnEquipHoleCnt()
	{
		return equipSpec.byHoleNum;
	}

	DWORD GetLastHoleGemID()
	{
		if (equipSpec.byHoleNum >= 1 && equipSpec.byHoleNum <= MAX_EQUIPHOLE_NUM) // 因为是这个，byHoleNum-1，所以才用等号
			return equipSpec.dwHoleGemID[equipSpec.byHoleNum-1];
		else
			return 0;
	}

	BYTE GetBrandLevel()
	{
		return equipSpec.byBrandLevel;
	}
	VOID ReSetBrand()
	{
		equipSpec.byBrandLevel = 0;
		memset(equipSpec.nBrandAtt, 0, sizeof(equipSpec.nBrandAtt));
	}

	// Jason 2010-4-12 v2.0.0
	BOOL IsFaBao(VOID)
	{
		return pEquipProto->eType == EIT_FaBao;
	}
};


//-----------------------------------------------------------------------------
// 常量定义 -- 和结构相关
//-----------------------------------------------------------------------------
const INT SIZE_ITEM						= sizeof(tagItem);
const INT SIZE_EQUIP					= sizeof(tagEquip);
const INT SIZE_EQUIPSPEC				= sizeof(tagEquipSpec);

// Jason 2010-4-13 v2.0.0
// 法宝系统
const INT MaxWuxingRewardNUM = 3;	// 五行契合奖励个数
const INT MaxFabaoRoleAttNUM = 3;	// 五行契合奖励个数
const INT MaxWuxingSlotNum	= 5;	// 五行凹槽个数
const INT MaxFairySkillNum	= 5;	// 妖精最大特技个数
const INT MaxFairyBirthLevel		= 8; // 妖精40，60，80，100，120提升配对次数

struct tagFabaoProto : public tagEquipProto
{
	ENUM_FabaoType	eFarbaoType;								// 法宝类型
	ERoleAttribute	eAttType[MaxFabaoRoleAttNUM];			// 影响角色属性类型
	INT16			nAttGrowing[MaxFabaoRoleAttNUM];		// 属性影响值,也即成长基数
	ERoleAttribute	eSlotAttType[MaxWuxingSlotNum];				// 凹槽契合奖励类型
	INT				nSlotAttValue[MaxWuxingSlotNum];			// 凹槽契合奖励值，超过100000的值为百分比。比如110000=100%，101000=10%。
	BOOL			bCanReset;									// 是否允许五行转生，即重置属性
	BOOL			bCanUse;									// 是否允许作为辅助法宝
	BOOL			bHasDefaultValue;							// 是否有默认属性
	INT16			n16DefaultFabaoQulity;						// 法宝资质，在允许默认情况的下，才取原型表中的值
	INT16			n16DefaultWuxing;							// 缺省五行属性
	//INT				nWuxingMatchReward[MaxWuxingRewardNUM];	// 缺省五行契合触发器id
	EWuXing			eDefaultSlotType[MaxWuxingSlotNum];			// 默认凹槽
	INT				nIdfQltyPct[EIQ_End];						// 品质鉴定几率

	// Jason 2010-10-24 妖精元素伤害以及抗性等
	EElementInjury eEleInjury;		// 元素伤害类型
	INT			nEleInjury;		// 元素伤害值
	INT			nElePierce;		// 元素穿透 [2/16/2012 zhangzhihua]

	INT			nEleResistance[EER_End];	// 元素抗性，以元素抗性枚举做索引访问
	DWORD		dwDef60StuntID;				// 妖精升到60级后，某人学习到的特技，可以不填
	BOOL		bCanBirth;						// 是否可以配对
	INT16		n16CanFusion;				// 是否可以融合 [3/22/2012 zhangzhihua]

	bool		CanFusion() const {return GT_INVALID != n16CanFusion;}
};

const INT MaxHolyEquipNum	= 6;		// 圣灵上可放圣纹的个数
// 圣灵附加属性
struct tagHolyProto : public tagEquipProto
{
	INT nDevourNum;						// 圣灵可吸收妖精的总次数
	INT16 n16EquipmentNumber;			// 圣灵初始可装备道具数量
	INT	  nHolyMaxvalue;				// 灵能的上限值
	INT16 n16CallConsumption;			// 在圣灵召唤状态下每秒消耗的人物圣灵值
	INT   nHolyDmg;						// 圣灵伤害值
	INT	  nHolyDef;						// 圣灵防御
	INT   nHolyValue;					// 初始灵能值
	INT	  nCrit;						// 致命,影响致命率的属性
	INT16 n16HolyCritRate;				// 致命量
	INT   nExDamage;					// 伤害加深
	INT   nAttackTec;					// 精准
	INT   nNeglectToughness;			// 灵巧	
};

struct tagHolyEquipProto : public tagEquipProto
{
	INT   nCostHolyValue;									// 装备此圣纹要消耗的灵能值
	INT   nCostMinHolyValue;								// 装备此圣纹要消耗的最小灵能值
	INT16 n16MaxLapNum;										// 最大充能次数
	INT16 n16WearLapNum;									// 可在圣灵上重复装备的数量
	INT   nHolyLevel;										// 圣灵吞噬等级限制

	tagHolyAttEffect BaseEffect[MAX_EQUIPATT_BASE_EFFECT];	// 充能前，装备圣灵 影响的属性(和品级无关)
};



const INT MAX_FAIRY_SHAPE = 11;
struct tagFairyEffect
{
	DWORD dwTypeID;										// 妖精typeID
	TCHAR szFairyShape[MAX_FAIRY_SHAPE][X_SHORT_NAME];	// 妖精形态
	TCHAR szFirstPic[X_LONG_NAME];		// 第一形态
	TCHAR szSecondPic[X_LONG_NAME];		// 第二形态
	TCHAR szThirdPic[X_LONG_NAME];		// 第三形态
	tagFairyEffect()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

// 法宝等阶原型
struct tagFabaoLevelProto
{
	INT		nID;
	INT		nMinUseLevel;			// 使用法宝最小等级要求，不需要最大限制的哟
	INT		nLevelUpWhated;			// 升到下一级需要多少修炼值
	INT		nMinValue;				// 达到该级别，需要至少多少修炼值
	FLOAT	fCoefficientOfGrowing;	// 成长系数
};
// 法宝五行相生奖励原型
struct tagFabaoWuxingProto
{
	INT		nID;
	DWORD	dwTriggerID;				// 相生奖励触发器
	DWORD	dwBuffID;					// 相生，buffid，大ID
	INT		nDifficulty;				// 法宝出现难度20 - 200
	INT16	n16NativeIntelligenceLim;	// 法宝资质限制
	ENUM_FabaoType	eFabaoTypeLim;		// 法宝类型限制
	INT		nXSRewardTimes;		// 法宝相生奖励激发类型（三槽，四槽，五槽），名字不改了
	INT		nRare;						// 珍视度
};

struct tagTrumpWuXingAward
{
	DWORD dwTypeID;
	TCHAR szName[X_LONG_NAME];
	TCHAR szDesc[X_HUGE_NAME];
	tagTrumpWuXingAward(){ZeroMemory(this, sizeof(*this));}
};

// Jason 2010-12-6 v1.3.0妖精繁殖相关
enum EFabaoStatus
{
	EEFS_Normal	=	0,	// 正常状态
	EEFS_Waiting	=	1,	// 等待生育确认
	EEFS_Pregnancy	=	2,	// 孕育
	EEFS_GiveBirth	=	3,	// 生育
	EEFS_Rest		=	4,	// 休息
};
struct tagFabaoSpec
{
	INT16		n16NativeIntelligence;	// 资质:白色的法宝资质为1~10，黄色的法宝资质为11~20，绿色品质的法宝资质为21~40，蓝色品质的法宝品质为41~60，橙色的法宝资质为61~80，紫色的法宝资质为81~100
	INT16		n16Stage;				// 等阶
	INT			nPracticeValue;			// 修炼值
	BOOL		bStoredExpOpen;		// 经验封印是否开启
	INT			nCurStoredExp;			// 封印的经验值
	ERoleAttribute	eAttType[MaxFabaoRoleAttNUM];			// 影响角色属性类型
	INT16			nAttGrowing0[MaxFabaoRoleAttNUM];		// 属性影响值,也即成长基数
	INT			nAttGrowing[MaxFabaoRoleAttNUM];		// 属性影响值,也即成长基数
	EWuXing		eWuxing;				// 五行属性 
	EWuXing		eWuxingHole[MaxWuxingSlotNum];					// 五行灵槽
	DWORD		dwWuxingBeadID[MaxWuxingSlotNum];				// 五行灵珠id，用的是typeid，考虑。。。
	ERoleAttribute	eSlotAttType[MaxWuxingSlotNum];				// 凹槽契合奖励类型
	INT			nSlotAttValue[MaxWuxingSlotNum];		// 灵珠契合度奖励
	//INT			nWuxingMatchReward[MaxWuxingRewardNUM];		// 五行相生奖励
	INT			nUseLevel;										// 等级限制
	INT			nSlotNum;										// 当前灵槽数
	// 2010-4-16增加
	BYTE			bySlotLingzhuMatch;							// 当前灵珠契合数
	BYTE		bySlotMacthNdx[MaxWuxingSlotNum];				// 当前灵槽是否契合
	//ERoleAttribute	eAttLingzhu[MaxWuxingSlotNum][MaxWuxingRewardNUM];			// 镶嵌灵珠带来的影响属性
	//INT16			nAttLingzhuValue[MaxWuxingSlotNum][MaxWuxingRewardNUM];		// 属性影响值,也即成长基数

	INT			nMood;									// 心情，波动
	INT			nMoodTicks;							// 心情计时，服务器用
	DWORD		dwMoodBuffID;		// 亢奋状态buff

	EElementInjury eEleInjury;		// 元素伤害类型
	INT			nEleInjury;		// 元素伤害值
	INT			nElePierce;			// 元素穿透
	INT			nEleResistance[EER_End];	// 元素抗性，以元素抗性枚举做索引访问
	WORD		wEleInjPromoteTimes;		// 伤害提升次数 [3/12/2012 zhangzhihua]	
	WORD		wEleResPromoteTimes;		// 抗性提升次数 [3/12/2012 zhangzhihua]

	WORD		wDamageUpgradeTimes;				// 伤害可提升次数
	WORD		wResistanceUpgradeTimes;			// 抗性可提升次数
	DWORD		dwFairySkill[MaxFairySkillNum];		// 已有特技，默认GT_INVALID	
	DWORD		dwUnsureSkill;						// 待确认特技，默认GT_INVALID

	EFabaoStatus	eFabaoStatus;					// 妖精状态
	DWORD			dwStatusChangeTime;		// 状态更新时间，单位秒
	INT				nBirthValue;				// 生育值，在配对时计算，生育时更具该值随机生成物品
	INT				nBirthTimes;				// 配对次数

	BYTE			byBirthLevel[MaxFairyBirthLevel]; // 妖精升级，获得可配对次数时，记录妖精等级
	BYTE			byIsGivenBirthTimes;					// 名字很怪，hh，是否生育过，主要是为了给已经升级到40以上的玩家补偿配对次数判断用

	BYTE			byMaxIntelligence;	// 资质上限，未经上限提升的话该值为0
	// 元素伤害类型，伤害值
	// 抗性值，按枚举值定义的数组

	tagFabaoSpec()
		:n16NativeIntelligence(0),	n16Stage(0),				nPracticeValue(0),				bStoredExpOpen(FALSE),		nCurStoredExp(0),
		eWuxing(EWX_Null),			nUseLevel(0),				nSlotNum(0),					bySlotLingzhuMatch(0),		nMood(0),
		nMoodTicks(0),				dwMoodBuffID(GT_INVALID),	wDamageUpgradeTimes(0),		wResistanceUpgradeTimes(0),	dwUnsureSkill(-1),
		nEleInjury(0),				eEleInjury(EEI_Null),		eFabaoStatus(EEFS_Normal),	dwStatusChangeTime(0),		nBirthValue(0),
		nBirthTimes(0),				byIsGivenBirthTimes(0),		byMaxIntelligence(0),			nElePierce(0),				wEleInjPromoteTimes(0),
		wEleResPromoteTimes(0)
	{
		memset(eWuxingHole,EWX_Null,sizeof(eWuxingHole));
		memset(dwWuxingBeadID,0,sizeof(dwWuxingBeadID));
		memset(eSlotAttType,ERA_Null,sizeof(eSlotAttType));
		memset(nSlotAttValue,0,sizeof(nSlotAttValue));
		//memset(nWuxingMatchReward,0,sizeof(nWuxingMatchReward));
		memset(bySlotMacthNdx,0,sizeof(bySlotMacthNdx));
		//memset(eAttLingzhu,ERA_Null,sizeof(eAttLingzhu));
		//memset(nAttLingzhuValue,0,sizeof(nAttLingzhuValue));
		memset(nEleResistance,0,sizeof(nEleResistance));
		memset(dwFairySkill,GT_INVALID,sizeof(dwFairySkill));
		memset(byBirthLevel,0,sizeof(byBirthLevel));
	}
};

struct tagHolySpec
{
	INT		nDevourNum;							// 吸收妖精的当前次数
	INT16	n16EquipmentNumber;					// 圣灵可装备道具数量
	INT		nCoValue;							// 默契值
	INT		nToDayEatNum;						// 今天吃了多少只了~
	INT64	n64EquipSerialIDs[MaxHolyEquipNum];	//  圣灵上圣纹64ID
	INT		nCostHoly;							// 装备圣纹所消耗的灵能
	INT		nMaxDevourNum;						// 最大可吃妖精次数，通过道具提升，未提升时读proto表中的nDevourNum字段

	DWORD dwLevelUpAtt[8];			//圣灵强化后的属性数组，第一个值表示等级第二个数表示属性类型  第二个数应该对应ESAT_End


	//INT   nHolyDmg;						// 圣灵伤害值
	//INT	  nHolyDef;						// 圣灵防御
	//INT	  nCrit;						// 致命,影响致命率的属性
	//INT16 n16HolyCritRate;				// 致命量
	//INT   nExDamage;					// 伤害加深
	//INT   nAttackTec;					// 精准
	//INT   nNeglectToughness;			// 灵巧
	//INT   nCoValue;						// 默契值

	tagHolySpec()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct tagHolyEquipSpec
{
	INT   nCostHoly;							// 装备该圣纹要消耗的灵能
	UINT16 n16EnhanceCount;						// 圣纹的充能次数
	INT	  nHolyAttEffect[X_ESAT_ATTA_NUM];		// 对圣灵的属性影响值	

	tagHolyEquipSpec()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

// 妖精繁殖最小等级
#define FABAO_MATING_MIN_STAGE	40
// 妖精繁殖最小资质
#define FABAO_MATING_MIN_INTELLIGENCE	70
// 妖精繁育cd停止秒数
#define FABAO_MATING_CD_STOP	(60*60)

struct tagFabao : public tagEquip,public tagFabaoSpec
{
	//const tagFabaoProto * pFabaoProto;	// 法宝原型
	tagFabao(){}
	//原型指针

	BOOL CanMating() // 是否可以配对
	{
		if( equipSpec.byQuality < EIQ_Orange )
			return FALSE;
		if( n16Stage < FABAO_MATING_MIN_STAGE )
			return FALSE;
		if( n16NativeIntelligence < FABAO_MATING_MIN_INTELLIGENCE )
			return FALSE;
		if( P_VALID(pEquipProto) && (((tagFabaoProto*)pEquipProto)->bCanBirth == GT_INVALID))
			return FALSE;
		return TRUE;
	}
	INT GetIncTimes(INT nLastStage)
	{
		static INT nTimes[] = {0,1,2,3,4};
		INT last = 0;
		INT cur = 0;
		if(nLastStage >= FABAO_MATING_MIN_STAGE)
			last = (nLastStage - FABAO_MATING_MIN_STAGE/2) / 20;
		if(n16Stage >= FABAO_MATING_MIN_STAGE)
			cur = (n16Stage - FABAO_MATING_MIN_STAGE/2) / 20;
		return nTimes[cur] - nTimes[last];
	}
	BOOL CanIncBirthTimes(int stage)
	{
		BOOL bFull = TRUE;
		if( P_VALID(pProtoType) )
		{
			tagFabaoProto * pFabaoProto = (tagFabaoProto*)pProtoType;
			if( pFabaoProto->bCanBirth == GT_INVALID )
				return FALSE;
		}
		for( int i = 0; i < MaxFairyBirthLevel; ++i )
		{
			if( !P_VALID(byBirthLevel[i]) )
				bFull = FALSE;
			if(byBirthLevel[i] == stage)
				return FALSE;
		}
		return !bFull;
	}
	BOOL IncBirthTimes(int nLastLevel)
	{
		if( P_VALID(pProtoType) )
		{
			tagFabaoProto * pFabaoProto = (tagFabaoProto*)pProtoType;
			if( pFabaoProto->bCanBirth == GT_INVALID )
				return FALSE;
		}
		if( n16Stage >= FABAO_MATING_MIN_STAGE )
		{
			BOOL bFinded = FALSE;
			if( nLastLevel < FABAO_MATING_MIN_STAGE )
			{
				bFinded = TRUE;
				// 付初始值
				tagFabaoProto * pFabaoProto = (tagFabaoProto*)pProtoType;
				if(P_VALID(pFabaoProto) && pFabaoProto->bCanBirth != GT_INVALID)
					nBirthTimes = pFabaoProto->bCanBirth;
			}
			else if( (nLastLevel - FABAO_MATING_MIN_STAGE) / 20 != (n16Stage  - FABAO_MATING_MIN_STAGE) / 20 )
				bFinded = TRUE;
			if( !bFinded )
				return FALSE;
			
			bFinded = FALSE;
			int nCurStage = (n16Stage / 10) * 10;
			int nNdx = -1;
			for( int i = MaxFairyBirthLevel - 1; i >= 0 ; --i )
			{
				if( byBirthLevel[i] == nCurStage)
				{
					bFinded = TRUE;
					break;
				}
				if( byBirthLevel[i] == 0 )
				{
					nNdx = i;
				}
			}
			if( !bFinded && nNdx >= 0 && nNdx < MaxFairyBirthLevel )
			{
				nBirthTimes += GetIncTimes(nLastLevel);
				byBirthLevel[nNdx] = nCurStage;
				return TRUE;
			}
		}
		return FALSE;
	}
	BOOL CanExchange() const
	{
		switch( eFabaoStatus )
		{
		case EEFS_Pregnancy:
		case EEFS_Rest:
			return FALSE;
		}
		return TRUE;
	}
	BOOL CanBirth() const
	{
		if( P_VALID(pEquipProto) )
		{
			const tagFabaoProto * pPro = (const tagFabaoProto*)pEquipProto;
			if( pPro->bCanBirth != GT_INVALID)
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	BOOL LogFairyBirth(INT nBeforeChangeBirthState, INT nAfterChangeBirthState);
	BOOL IsBirthChangeValid(INT nBeforeChangeBirthState, INT nAfterChangeBirthState)
	{
		if (nBeforeChangeBirthState == EEFS_Rest && nAfterChangeBirthState == EEFS_Normal)
			return TRUE;
		if (nBeforeChangeBirthState > nAfterChangeBirthState)
			return FALSE;
		if (nBeforeChangeBirthState == nAfterChangeBirthState)
			return TRUE;
		if (nAfterChangeBirthState - nBeforeChangeBirthState == 1)
			return TRUE;
	}
};/*法宝结构定义*/

// 圣灵结构定义
struct tagHolyMan :public tagEquip, public tagHolySpec
{
	tagHolyMan(){};		
};

// 圣纹结构定义
struct tagHolyEquip :public tagEquip, public tagHolyEquipSpec
{
	tagHolyEquip(){};		
};



static CONST INT FabaoQltyNativeIntelligence[][3] = 
{
	{ 1,50,50 },	/*白色品质*/
	{ 1,50,50 },	/*黄色品质*/
	{ 1,50,50 },	/*绿色品质*/
	{ 50,80,70 },	/*蓝色品质*/
	{ 50,90,80 },	/*橙色品质*/
	{ 60,100,80 },	/*紫色品质*/
	{ 60,100,90 }	/*粉色品质*/
};
#define MGetFabaoQltyIntel(qlty,minIntel,maxIntel)	do{ \
	if( qlty > EIQ_Start && qlty < EIQ_End ) \
	{\
		minIntel = FabaoQltyNativeIntelligence[qlty][0];\
		maxIntel = FabaoQltyNativeIntelligence[qlty][1];\
	}\
}while(0)

#define MGetFabaoQltyIntelWhenCreate(qlty,minIntel,maxIntel)	do{ \
	if( qlty > EIQ_Start && qlty < EIQ_End ) \
{\
	minIntel = FabaoQltyNativeIntelligence[qlty][0];\
	maxIntel = FabaoQltyNativeIntelligence[qlty][2];\
}\
}while(0)

CONST INT SIZE_FABAO	= sizeof(tagFabao);
CONST INT SIZE_HOLY	= sizeof(tagHolyMan);
CONST INT SIZE_HOLYEQUIP	= sizeof(tagHolyEquip);

//-----------------------------------------------------------------------------
// 魂晶结构定义
//-----------------------------------------------------------------------------
struct tagSoulCrystalSpec
{
	BYTE bySoulCrystalQlty;		// 资质
	BYTE bySoulCrystalLvl;			// 等级 0-10
	DWORD dwStaticSkillID;		// 自带能力
	INT nSoulAttID[SOUL_CRYSTAL_ATT_MAX_NUM];	// 成长能力			
	DWORD dwSoulStrength;	// 能力强度

	tagSoulCrystalSpec()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

struct tagSoulCrystal : public tagEquip/*, public tagSoulCrystalSpec*/
{
	tagSoulCrystalSpec SoulSpec;
};

CONST INT SIZE_HUNJING	= sizeof(tagSoulCrystal);

// 销毁物品&装备(避免在tagItem中使用虚析构函数)
static VOID Destroy(tagItem *&pItem)
{
	if(!P_VALID(pItem))
	{
		return;
	}

	if( P_VALID(pItem->pProtoType) && EIT_FaBao  == pItem->pProtoType->eType )
		delete ((tagFabao*)pItem);
	else if( P_VALID(pItem->pProtoType) && EIT_SoulCrystal == pItem->pProtoType->eType )
		delete ((tagSoulCrystal*)pItem);
	else if( P_VALID(pItem->pProtoType) && EIT_Holy == pItem->pProtoType->eType )
		delete ((tagHolyMan*)pItem);
	else if( P_VALID(pItem->pProtoType) && EIT_HolyEquip == pItem->pProtoType->eType )
		delete ((tagHolyEquip*)pItem);
	else if(MIsEquipment(pItem->dwTypeID))
	{
		delete ((tagEquip*)pItem);
	}
	else
	{
		delete pItem;
	}

	pItem = NULL;
}

// 销毁物品&装备(避免在tagItem中使用虚析构函数)
static VOID Destroy(tagEquip *&pEquip)
{
	if( P_VALID(pEquip) && P_VALID(pEquip->pProtoType) && EIT_FaBao  == pEquip->pProtoType->eType )
	{
		delete ((tagFabao*)pEquip);
		pEquip = NULL;
		return;
	}
	else if (P_VALID(pEquip) && P_VALID(pEquip->pProtoType) && EIT_SoulCrystal  == pEquip->pProtoType->eType)
	{
		delete ((tagSoulCrystal*)pEquip);
		pEquip = NULL;
		return;
	}
	else if (P_VALID(pEquip) && P_VALID(pEquip->pProtoType) && EIT_Holy  == pEquip->pProtoType->eType)
	{
		delete ((tagHolyMan*)pEquip);
		pEquip = NULL;
		return;
	}
	else if (P_VALID(pEquip) && P_VALID(pEquip->pProtoType) && EIT_HolyEquip  == pEquip->pProtoType->eType)
	{
		delete ((tagHolyEquip*)pEquip);
		pEquip = NULL;
		return;
	}
	if(!P_VALID(pEquip))
	{
		return;
	}

	SAFE_DEL(pEquip);
}

// Jason 2010-8-26 v1.0.0 妖精契约消耗的相关itemtypeids
static const DWORD ItemsOfFairyContract[]=
{
	4080080,	// 彼岸之花，好像王菲的彼岸花
	4080081,	// 风铃之石
	4080082,	// 遗忘之水
	4080083,	// 幻化之羽
	4080084	// 契约之书
};

/** struct	tagItemDisplayInfo
brief 妖精特技
*/
struct tagFairySkill
{
	DWORD dwTypeID;							// 物品ID
	INT nType;
	TCHAR szName[X_SHORT_NAME];				// 物品名称
	TCHAR szDesc[X_HUGE_NAME];				// 物品描述
	tagFairySkill(){ZeroMemory(this, sizeof(*this));}
};

static INT GetFairyMatingValue(const tagFabao * pFabao)
{
	if( !P_VALID(pFabao) )
		return 0;
	switch(pFabao->equipSpec.byQuality)
	{
	case EIQ_Orange:
		return 15;
	case EIQ_Purple:
		return 30;
	case EIQ_Pink:
		return 50;
	}
	return 0;
}

#define MAX_GIFT_ITEM_NUM 5
#define MAX_GIFT_ITEM_SHOW_NUM 6	//显示商品最大数...
struct tagIDBuyBack
{
	DWORD dwID;
	DWORD dwExp;
	DWORD dwGiftID;
	DWORD dwItemID[MAX_GIFT_ITEM_NUM];
};
struct tagIDBuyBackEx
{
	DWORD dwID;
	DWORD dwItemID[MAX_GIFT_ITEM_SHOW_NUM];
	tstring strDesc[MAX_GIFT_ITEM_SHOW_NUM];
};

#define NewKeyCodeString _T("JDSHJKSDHF")
#define NewKeyCodeGiftid 4700294

#define SaveTelNumGiftid 4830067
#define YuanBaoDaiBi 4810054

//妖精融合
struct tagFairyFusionRetProto
{
	DWORD dwID;			//id
	INT byLowScore;	//最低分数
	INT byHighScore;	//最高分数
	DWORD dwPinkRet;	//粉色几率
	DWORD dwPurpleRet;	//紫色几率
	DWORD dwOrangeRet;	//橙色几率
	DWORD dwFailRet;	//失败几率
};

//妖精融合 服务器
enum
{
	EFairyFusion_Orange = 0,
	EFairyFusion_Purple,
	EFairyFusion_Pink,

	EFairyFusion_End,
};
struct tagFairyFusionQualityProto
{
	DWORD dwID;			//id
	INT16 n16LowScore;	//最低分数
	INT16 n16HighScore;	//最高分数
	INT16 n16QualityRate[EFairyFusion_End];	//粉色几率
};

const INT MAX_FAIRY_FUSION_TYPE = 29;
struct tagFairyFusionTypeProto
{
	DWORD dwID;			//id
	DWORD dwTypeID[MAX_FAIRY_FUSION_TYPE];	//随即妖精的ID
	INT16 n16Rate[MAX_FAIRY_FUSION_TYPE];		//出现的概率
};

struct tagWarRewardPointProto
{
	DWORD dwID;
	DWORD dwExp;
	DWORD dwMoney;
};

//-----------------------元神-------------------------------------------------
// 元神觉醒
const INT		MAX_HOLY_VALUE						= 99999;
const INT		MAX_PRIVITE_VALUE					= 999;
const INT64     MAX_HOLY_EXP						= 999999999;
const INT       MIN_HOLY_SOUL_LEVEL					= 15;			 // 元神最低等级
//const DWORD		HOLYSOUL_BUFFID						= 99905;		 // 元神buff类型ID
const INT		DEC_SOULVAL_PER_SECOND				= 1;			 // 每秒消耗元神值
const INT		ADD_SOULVAL_PER_MINUTE				= 1;			 //	每秒恢复元神值
const INT		CALLSOUL_MIN_SOULVAL				= 25;			 // 召唤元神的元神值下限
const INT		HOLYSOUL_MAX_LEVEL					= 100;			 // 每一转元神最大等级
const INT		HOLYSOUL_INITID						= 10001;		 // 元神初始化ID
const INT		COVALUE_OFFTIME_PER_HOUR			= 5;			 // 离线每小时扣除默契值
const INT		COVALUE_ONTIME_PER_SECOND			= 1;			 // 在线元神觉醒状态恢复默契值
//const INT		HOLYSOUL_SKILLID					= 6020104;		 // 召唤元神的技能ID
const INT		ADD_SOULVAL_TIME					= 5;			 // 每5秒增加一点元神值

const INT		REBORN_MAX_LEVEL					= 3;		 // 元神转生最高等级
const DWORD REBORN_ITEM_TYPEID_LV1 = 6020101;		//幻元玉魄 一转
const DWORD REBORN_ITEM_TYPEID_LV2 = 6020102;		//幻元玉魄 二转
const DWORD REBORN_ITEM_TYPEID_LV3 = 6020103;		//幻元玉魄 三转

const DWORD	REBORN_STUFF_BINDING		= 2005301;	// 老君仙露
const DWORD	REBORN_STUFF_UNBINDING	= 2005303;	// 老君仙露(非绑定)

//-----------------------------------------------------------------------------
// 元神状态类型
//-----------------------------------------------------------------------------
enum EHolySoulState
{
	EHSS_Asleep         = 0,       //元神休眠
	EHSS_Awake          = 1,       //元神觉醒
};

//-----------------------------------------------------------------------------
// 元神基础属性
//-----------------------------------------------------------------------------
struct tagSoulAttProto
{
// 	DWORD	    dwID;                //元神ID
// 	INT64		n64UpLvExp;		     //元神升级所需经验
// 	INT16		n8RebornLv;		     //元神转生等级 
// 	INT16		n8SoulLevel;		 //元神等级 150
// 	INT			nDamage;			 //元神绝技类伤害
// 	INT			nHit;				 //攻击命中
// 	INT			nCrit;				 //攻击致命
// 	INT			nCritAmount;	     //攻击致命量
// 	INT			nExattack;			 //外功攻击
// 	INT			nInattack;			 //内功攻击		
// 	INT			nAccrate;			 //精准
// 	INT			nExdamage;		     //额外伤害
// 	INT			nToughness;		     //忽视韧性
// 	INT16        n8Morale;	       	 //士气
// 	INT16		n8Covalue;		     //默契值当前最大值	
// 	DWORD       dwSoulValue;         //元神当前最大值

	DWORD		dwID;	//id
	BYTE		byEquipNum;				//可装备数量
	DWORD		dwHolyDmg;				//圣灵伤害
	DWORD		dwHolyValue;			//灵能值(初始值)
	DWORD		dwHolyValueMax;			//灵能上限(灵能扩充最大上限)
	DWORD		dwCallConsumption;		//召唤时灵能消耗值
	DWORD		dwCritValue;			//致命
	DWORD		dwCritRate;				//致命量
	DWORD		dwExDmg;				//伤害加深
	DWORD		dwAttackTec;			//精准
	DWORD		dwCoValue;				//默契值
	DWORD		dwEatNum;				//吞噬妖精次数
	tagSoulAttProto()
	{
		memset(this,0,sizeof(*this));
	}
};

struct tagSoulAttName : tagHolyProto
{
	TCHAR szMode_Low[128];		//低阶模型编号
	TCHAR szMode_High[128];		//高阶模型编号
	TCHAR szIcon_Low[128];		//低阶图标
	TCHAR szIcon_High[128];		//高阶图标
	TCHAR szEffect_Low[128];	//低阶特效编号
	TCHAR szEffect_High[128];	//高阶特效编号
	DWORD	dwHighEatNum;		//吃多少个妖精以后变成高阶~
	DWORD	dwScal;				//缩放
};


//-----------------------------------------------------------------------------
// 元神简单属性
//-----------------------------------------------------------------------------
struct tagSoulAtt
{
	INT			nSoulAtt[ESAT_End];
};

//-----------------------------------------------------------------------------
// 元神当前属性
//-----------------------------------------------------------------------------
struct tagSoulAttEx
{
	DWORD	    dwSoulID;
	INT64		n64LevelExp;	//元神当前经验
	INT64		n64MaxExp;		//元神升级所需经验
	INT			nCurSoulValue;
	INT			nMaxSoulValue;
	INT			nCurCovalue;	//当前默契值	
	INT			nMaxCovalue;	//当前默契值	
	tagSoulAttEx()
	{
		memset(this,0,sizeof(*this));
	}
};

//-----------------------------------------------------------------------------
// 元神基本属性
//-----------------------------------------------------------------------------
struct tagHolySoulInfo
{
	tagSoulAtt    SoulAtt;
	tagSoulAttEx  SoulAttEx;
	tagHolySoulInfo()
	{
		memset(this,0,sizeof(*this));
	}
};


//元神转生属性
struct tagHolySoulReborn
{
	BYTE	byCostNum;	//放入老君仙露的数量
	INT		nRate1;		//一转成功率
	INT		nRate2;		//二转成功率
	tagHolySoulReborn()
	{
		memset(this,0,sizeof(*this));
	}
};


//元神凿孔属性
struct tagHolySoulEquipHole
{
	BYTE	byID;			//元神装备的孔位
	INT		nRate;			//开凿孔位对应的成功几率
	INT		nCostMoney;		//装备凿孔消耗的金钱数
	tagHolySoulEquipHole()
	{
		memset(this,0,sizeof(*this));
	}
};

//元神镶嵌属性
struct tagHolySoulEquipInlay
{
	BYTE	byID;			//元神装备的孔位
	INT		nRate;			//镶嵌对应的成功几率
	INT		nCostMoney;		//镶嵌消耗的金钱数
	tagHolySoulEquipInlay()
	{
		memset(this,0,sizeof(*this));
	}
};
//--------------------------------------------------------------------------------
//圣灵升级属性提升proto
struct tagHolyLevelUpProto
{
	DWORD dwID;				//id表示区间
	DWORD dwDmgMin;			//伤害最小值
	DWORD dwDmgMax;			//伤害最大值
	DWORD dwDefMin;			//防御最小值
	DWORD dwDefMax;			//防御最大值
	DWORD dwLingNengMin;	//灵能最小值
	DWORD dwLingNengMax;	//灵能最大值
	DWORD dwCritMin;		//致命最小值
	DWORD dwCritMax;		//致命最大值
	DWORD dwCritValueMin;	//致命量最小值
	DWORD dwCritValueMax;	//致命量最大值
	DWORD dwDmgAddMin;		//伤害加深最小值
	DWORD dwDmgAddMax;		//伤害加深最大值
	DWORD dwAttackTecMin;	//精准最小值
	DWORD dwAttackTecMax;	//进准最大值
	DWORD dwNeglectMin;		//灵巧最小值
	DWORD dwNeglectMax;		//灵巧最大值
};

//--------------------------------------------------------------------------------
//圣纹强化属性表
struct tagHolyEquipEnhance
{
	DWORD dwID;							//id
	DWORD dwPowMin[X_ESAT_ATTA_NUM];	//基础属性最小值
	DWORD dwPowMax[X_ESAT_ATTA_NUM];	//基础属性最大值

	DWORD dwLingNengPow;		//减少的消耗灵能

	tagHolyEquipEnhance()
	{
		memset(this,0,sizeof(*this));
	}
};

//战场奖励结构
struct tagWarReward
{
	DWORD dwID;
	DWORD dwExpReward;	//每分给多少经验
	DWORD dwMoneyReward;//每分给多少金钱
};

// static CONST FLOAT FairyQltyScore[4] = 
// {
// 		1.25,	//蓝色品质
// 		1.75,	//橙色品质
// 		2.75,	//紫色品质
// 		4		//粉色品质
// };
// //根据妖精品质 等级  资质计算分值,最低用蓝色妖精
// //（等级分数*（品级影响*1.5）+资质分数*（品级影响*1.8））*10
// //等级分数：1.25*(1+(妖精等级/100)^3)
// //资质分数：1.75*(1+(妖精资质/100)^5)
// #define CallEatFairyScore(nQuility, nLevel, nIntelligence, nOutPut)	do{ \
// 	if( nQuility >= EIQ_Blue && nQuility <= EIQ_Pink ) \
// {\
// 	nOutPut = (INT)((1.25f * (1 + pow((nLevel/100.0f), 3)) * FairyQltyScore[nQuility-3]*1.5f + 1.75f * (1+pow((nIntelligence/100.0f), 5)) * FairyQltyScore[nQuility-3]*1.8f)*10.0f);\
// }\
// }while(0)

static CONST FLOAT FairyQltyScore[3] = 
{
		0,	//蓝色品质
		20,	//橙色品质
		40,	//紫色品质
};
#define CallEatFairyScore(nQuility, nLevel, nOutPut)	do{ \
	if( nQuility >= EIQ_Blue && nQuility <= EIQ_Purple ) \
{\
	nOutPut = FairyQltyScore[nQuility - EIQ_Blue] + nLevel;\
}\
}while(0)


static CONST DWORD GetColorValue[] =
{
	0xFFFFF7E0,	//白色
	0xfffff717, //黄色
	0xff05ff00, //绿色
	0xff00a2ff, //蓝色
	0xffff7800, //橙色
	0xff9932cd, //紫色
	0xffff80c0, //粉色
};
#pragma pack(pop)
