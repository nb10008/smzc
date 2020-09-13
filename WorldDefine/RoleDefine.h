#pragma once
/********************************************************************
	created:	2010/06/09
	created:	9:6:2010   16:23
	filename: 	e:\F-Project\outofcontrol\trunk\WorldDefine\RoleDefine.h
	file path:	e:\F-Project\outofcontrol\trunk\WorldDefine
	file base:	RoleDefine
	file ext:	h
	author:		
	
	purpose:	F-Project
	history:
			2010-6-9 修改相关角色属性名称，增加相关属性
*********************************************************************/
#include "base_define.h"
#include "AvatarAtt.h"
#include "QuestDef.h"

#pragma pack(push, 1)

const INT	MAX_ROLENUM_ONEACCOUNT		= 5;				// 帐号的最大角色数量
const INT	MAX_ROLE_LEVEL				= 150;				// 玩家可升到的最高级别
const DWORD MAX_SAFECODE_RESET_TIME		= (72 * 60 * 60);	// 安全码最大重置时间(72小时)

const INT	ROLE_LEVEL_LIMIT			= 100;				// 等级上限

const DWORD MAX_KILL_MONSTER			= 99999;			// 击杀怪物计数上限
const DWORD MAX_KILL_ROLE				= 9999;				// 击杀角色计数上限

//---------------------------------------------------------------------------
// 数据变化状态,数据库保存相关
//---------------------------------------------------------------------------
enum EUpdateDBStatus
{
	EUDBS_Null			= 0, // 不保存(默认状态)
	EUDBS_Insert		= 1, // 需插入记录
	EUDBS_Remove		= 2, // 删除
	EUDBS_Update		= 3, // 数据更新
};

//---------------------------------------------------------------------------
// 性别
//---------------------------------------------------------------------------
enum ESex
{
	ES_Null			= -1,
	ES_Woman		= 0,	// 女限制，即女可用
	ES_Man			= 1,	// 男限制，即男可用
};

// 消息发送范围 [4/28/2012 zhangzhihua]
enum EMessageRange
{
	EMR_Self		= 0,	//
	EMR_Around,				//
	EMR_Map,				//
	EMR_World,				//
};

//----------------------------------------------------------------------------
// 职业类型
//----------------------------------------------------------------------------
//enum EClassType
//{
//	ECT_Start		= -1,
//
//	ECT_Null		= -1,
//	ECT_Hobo		= 0,	// 浪人
//	ECT_Sword		= 1,	// 剑客
//	ECT_Blade		= 2,	// 刀客
//	ECT_Wand		= 3,	// 杖客
//	ECT_Bow			= 4,	// 灵羽
//	ECT_Valor		= 5,	// 气师
//	ECT_Supple		= 6,	// 玄师
//	ECT_Apothecary	= 7,	// 药师
//	ECT_Musician	= 8,	// 乐师
//	ECT_Stabber		= 9,	// 刺客
//
//	ECT_End			= 10,
//};
//
//const INT32 X_ClASS_TYPE_NUM = ECT_End - ECT_Start - 1;

//---------------------------------------------------------------------------
// 职业类型扩展
//---------------------------------------------------------------------------
//enum EClassTypeEx
//{
//	ECTE_Base		= 0x1,	// 基础职业
//
//	ECTE_JianXian	= 0x2,	// 剑仙
//	ECTE_XieWu		= 0x4,	// 邪舞
//
//	ECTE_BaDao		= 0x2,	// 霸刀
//	ECTE_GangRen	= 0x4,	// 罡刃
//
//	ECTE_ZongZhang	= 0x2,	// 宗杖
//	ECTE_DiMo		= 0x4,	// 地墨
//
//	ECTE_TianJian	= 0x2,	// 天箭
//	ECTE_MoLing		= 0x4,	// 魔翎
//
//	ECTE_XuXing		= 0x2,	// 虚行
//	ECTE_GuiQi		= 0x4,	// 鬼气
//
//	ECTE_XuangWang	= 0x2,	// 玄王
//	ECTE_MiZong		= 0x4,	// 密宗
//
//	ECTE_XianYi		= 0x2,	// 仙医
//	ECTE_DuYin		= 0x4,	// 毒隐
//n 
//	ECTE_YuSheng	= 0x2,	// 乐圣
//	ECTE_HuanYin	= 0x4,	// 幻音
//
//	ECTE_YingWu		= 0x2,	// 影武
//	ECTE_GuiWu		= 0x4,	// 鬼武
//};

//---------------------------------------------------------------
// 专精职业
//---------------------------------------------------------------
enum EClassType				 
{
	EV_Begin = -1,
    EV_Null  = 0,
	
	EV_Base	= 1,	// 基础职业：新手

	EV_Blade,		// 神兵
	EV_Bow,			// 玄翎
	EV_Strength,	// 乾坤
	EV_Musician,	// 仙音
	EV_Fuhu,		// 王者
	EV_XuanJia,		// 神甲
	EV_HuanShi,		// 幻矢
	EV_ZhanXin,		// 占星
	EV_LeiMang,		// 雷神
	EV_GuaXiang,	// 破魔
	EV_ShenShi,		// 神侍
	EV_TianYi,		// 天仪

	EV_End,
};

const INT32 X_ClASS_TYPE_NUM = EV_End - EV_Begin - 2;
//--------------------------------------------------------------
// 英雄职业
//--------------------------------------------------------------
enum EClassTypeEx
{
	EHV_Begin = -1,

	EHV_Base = 0,		// 基础职业，未选择英雄职业

	EHV_JianXian,		// 仙剑
	EHV_XieWu,			// 邪舞

	EHV_BaDao,			// 霸刀
	EHV_YingWu,			// 影舞
	
	EHV_JinGang,		// 金刚
	EHV_KuangZhan,		// 狂战
	
	EHV_TianJian,		// 天箭
	EHV_MoLing,			// 魔翎
	
	EHV_XuanHuang,		// 玄皇
	EHV_MiaoFa,			// 妙法
	
	EHV_MingWang,		// 冥王
	EHV_XuKong,			// 虚空
	
	EHV_XianYi,			// 仙医
	EHV_DuYin,			// 毒隐
	
	EHV_YueSheng,		// 乐圣
	EHV_HuanYin,		// 幻音

	EHV_End,
};

//--------------------------------------------------------------------------
// 天资类型
//--------------------------------------------------------------------------
enum ETalentType
{
	ETT_Start		= -1,
	ETT_Null		= -1,

	ETT_Blade		= 0, // 神兵
	ETT_Bow			= 1, // 玄翔
	ETT_Valor		= 2, // 乾坤
	ETT_Swing		= 3, // 仙音
	ETT_FreshHand	= 4, // 新手技能

	ETT_End			= 5,
};

const INT32 X_TALENT_TYPE_NUM = ETT_End - ETT_Start - 1;

//--------------------------------------------------------------------------
// 声望类型（可能会有两种类型的声望）
//--------------------------------------------------------------------------
// enum EReputationType
// {
// 	ERT_Start		= -1,
// 	ERT_Null		= -1,
// 
// 	ERT_End			= 0,
// };

//--------------------------------------------------------------------------
// 角色与生物公有状态标示
//--------------------------------------------------------------------------
enum EState
{
	ES_Dead			=	0,	// 死亡
	ES_Dizzy		=	1,	// 眩晕
	ES_Tie			=	2,	// 定身
	ES_Spor			=	3,	// 昏睡
	ES_Invincible	=	4,	// 无敌
	ES_Lurk			=	5,	// 隐身
	ES_DisArm		=	6,	// 缴械
	ES_NoSkill		=	7,	// 封技

	ES_CSAttack		=	8,	// 城战进攻阵营
	ES_CSDefence	=	9,	// 城战防御阵营

	ES_PvPAttack	=	10,	// 竞技场红营
	ES_PvPDefence	=	11,	// 竞技场绿营
	ES_NoMovement	=	12, // 禁止移动
	ES_NoJump		=	13, // 无法跳跃

	ES_End			=	14,
};

//--------------------------------------------------------------------------
// 角色状态标识 -- 该状态改变，需要向周围玩家广播
//--------------------------------------------------------------------------
enum ERoleState
{
	ERS_Safeguard	= 0x0001,	// 被保护
	ERS_Combat		= 0x0002,	// 战斗姿态
	ERS_PK			= 0x0004,	// 杀戮模式
	ERS_Arena		= 0x0008,	// 擂台(用于PVP活动或战场，该状态由脚本设置）

	ERS_SafeArea	= 0x0010,	// 安全区
	ERS_PVPArea		= 0x0020,	// PVP区
	ERS_StallArea	= 0x0040,	// 摆摊区
	ERS_PVP			= 0x0080,	// PVP状态

	ERS_Shop		= 0x0100,	// 商店交易
	ERS_Exchange	= 0x0200,	// 玩家间直接交易
	ERS_Stall		= 0x0400,	// 摆摊交易
	ERS_StallSet	= 0x0800,	// 设置摆摊物品

	ERS_PrisonArea	= 0x1000,	// 牢狱状态

	ERS_Mount		= 0x2000,	// 骑乘
	ERS_WaterWalk   = 0x4000,   // 水上行走
	ERS_Mount2		= 0x8000,	// 骑乘2

	ERS_Commerce	= 0x10000,	// 跑商中	
	ERS_PetExchange	= 0x20000,	// 宠物交易

	ERS_Hostility	= 0x40000,	// 戾气状态

	ERS_Wedding		= 0x80000,	// 婚礼状态

	ERS_PKEX		= 0x100000,	// 对抗模式

	ERS_TaxArea		= 0x200000,	// 收税区
	ERS_ScriptArea	= 0x400000,	// 脚本区
	ERS_PeaceModel	= 0x800000,	// 练级模式

	ERS_HoldChest	= 0x1000000,// 夺宝奇兵活动携宝模式
	ERS_WardrobeWare= 0x2000000,// 衣橱时装穿戴模式
	ERS_Transform	= 0x4000000,// 变身状态

	ERS_CALLHOLY    = 0x8000000,// 召唤圣灵状态

	ERS_End			= 0x40000000,
};

//--------------------------------------------------------------------------
// 角色状态标识 -- 该状态改变，只需要通知玩家本人
//--------------------------------------------------------------------------
enum ERoleStateEx
{
	ERSE_BagPsdPass		= 0x1,	// 行囊密码验证已通过
	ERSE_BagPsdExist	= 0x2,	// 行囊密码存在
};

//--------------------------------------------------------------------------
// 角色PK状态
//--------------------------------------------------------------------------
enum ERolePKState
{
	ERPKS_Peace = 0,			// 和平
	ERPKS_Warnning,				// 警告
	ERPKS_Wicked,				// 邪恶
	ERPKS_Wanted,				// 通缉
	ERPKS_End,
};

//-------------------------------------------------------------------------------
// 阵营设定
//-------------------------------------------------------------------------------
enum ECampType
{
	ECamp_Null = 0,			// 无阵营 [4/5/2012 zhangzhihua]
	ECamp_Justice,				// 正义 [4/5/2012 zhangzhihua]
	ECamp_Evil,					// 邪恶 [4/5/2012 zhangzhihua]
	ECamp_Neutrality,			// 中立 [4/5/2012 zhangzhihua]

	ECamp_End,
};

//-------------------------------------------------------------------------------
// 人物基本属性枚举(该处枚举顺序、个数调整,可能会影响到数据库装备表)
//-------------------------------------------------------------------------------
enum ERoleAttribute
{
	ERA_Null					=	-1,		//
	
	// 一级属性
	ERA_AttA_Start				=	0,
	ERA_Physique				=	0,		// 当前体质
	ERA_Strength				=	1,		// 当前武力	
	ERA_Pneuma					=	2,		// 当前真元
	ERA_InnerForce				=	3,		// 当前法力	
	ERA_Technique				=	4,		// 当前技力【不再使用】	
	ERA_Agility					=	5,		// 当前身法【不再使用】
	ERA_AttA_End				=	5,

	// 玩家的点数
	ERA_AttPoint				=	6,		// 当前属性点数
	ERA_TalentPoint				=	7,		// 当前天资点数

	// 二级属性
	ERA_AttB_Start				=	8,
	ERA_MaxHP					=	8,		// 最大体力
	ERA_HP						=	9,		// 当前体力
	ERA_MaxMP					=	10,		// 最大真气
	ERA_MP						=	11,		// 当前真气
	ERA_ExAttack				=	12,		// 物理攻击
	ERA_ExDefense				=	13,		// 物理防御
	ERA_InAttack				=	14,		// 法术攻击
	ERA_InDefense				=	15,		// 法术防御
	ERA_AttackTec				=	16,		// 精准
	ERA_DefenseTec				=	17,		// 灵巧
	ERA_HitRate					=	18,		// 命中率【不再使用】
	ERA_Dodge					=	19,		// 闪避率【不在使用】
	ERA_Endurance				=	20,		// 当前耐力，神力
	ERA_MaxEndurance			=	21,		// 最大耐力，神力上限
	ERA_AttB_End				=	22,

	// 固有属性
	ERA_HPRegainRate			=	22,		// 体力恢复速度
	ERA_MPRegainRate			=	23,		// 真气恢复速度
	ERA_Rage					=	24,		// 怒气
	ERA_Vitality				=	25,		// 活力
	ERA_MaxVitality				=	26,		// 最大活力
	ERA_VitalityRegainRate		=	27,		// 活力恢复速度
	ERA_Speed_XZ				=	28,		// XZ平面速度速度
	ERA_Speed_Y					=	29,		// Y方向速度
	ERA_Speed_Swim				=	30,		// 游泳的XZ速度
	ERA_Speed_Mount				=	31,		// 骑乘的XZ速度

	ERA_EngraveAtt_Start		=	32,		// 镌刻影响属性
	ERA_WeaponDmgMin			=	32,		// 最小武器伤害
	ERA_WeaponDmgMax			=	33,		// 最大武器伤害
	ERA_WeaponSoul				=	34,		// 法器伤害
	ERA_Armor					=	35,		// 防具护甲
	ERA_EngraveAtt_End			=	35,		// 镌刻影响属性

	ERA_Derate_Start			=	36,
	ERA_Derate_Ordinary			=	36,		// 普通（徒手）伤害减免【不再使用】
	ERA_Derate_Bleeding			=	37,		// 出血伤害减免【不再使用】
	ERA_Derate_Brunt			=	38,		// 玄翎伤害减免
	ERA_Derate_Bang				=	39,		// 神兵伤害减免
	ERA_Derate_Poison			=	40,		// 毒性伤害减免【不再使用】
	ERA_Derate_Thinker			=	41,		// 仙音伤害减免
	ERA_Derate_Injury			=	42,		// 乾坤伤害减免
	ERA_Derate_Stunt			=	43,		// 绝技伤害减免【不再使用】
	ERA_Derate_ExAttack			=	44,		// 物理伤害减免
	ERA_Derate_InAttack			=	45,		// 法术伤害减免
	ERA_Derate_ALL				=	46,		// 所有伤害减免
	ERA_Derate_End				=	46,

	ERA_ExDamage				=	47,		// 额外伤害
	ERA_ExDamage_Absorb			=	48,		// 额外伤害吸收

	ERA_Resist_Bleeding			=	49,		// 玄伤抗性——命中几率
	ERA_Resist_Weak				=	50,		// 衰老抗性——命中几率
	ERA_Resist_Choas			=	51,		// 心乱抗性——命中几率
	ERA_Resist_Special			=	52,		// 封速抗性——命中几率
	ERA_Regain_Addtion			=	53,		// 灵力

	ERA_Attack_MissRate			=	54,		// 攻击失误率
	ERA_CloseAttack_DodgeRate	=	55,		// 近战躲避率加成
	ERA_RemoteAttack_DodgeRate	=	56,		// 远程躲避率加成

	ERA_Crit_Rate				=	57,		// 暴击加成
	ERA_Crit_Amount				=	58,		// 暴击量加成
	ERA_Block_Rate				=	59,		// 格挡率加成

	ERA_Inspiration				=	60,		// 灵感
	ERA_Lurk					=	61,		// 隐匿
	ERA_Savvy					=	62,		// 悟性
	ERA_Morale					=	63,		// 士气【不再使用】
	ERA_Injury					=	64,		// 内伤
	ERA_Enmity_Degree			=	65,		// 仇恨度
	ERA_Shape					=	66,		// 体型
	ERA_Exp_Add_Rate			=	67,		// 经验获得加成		原经验/金钱/掉率 * (1+加成/10000)
	ERA_Money_Add_Rate			=	68,		// 金钱获得加成
	ERA_Loot_Add_Rate			=	69,		// 物品掉落几率加成

	// 社会属性
	ERA_Fortune					=	70,		// 福缘
	ERA_Appearance				=	71,		// 仪容【不再使用】
	ERA_Rein					=	72,		// 统御【不再使用】
	ERA_Knowledge				=	73,		// 阅历
	ERA_Morality				=	74,		// 道德
	ERA_Culture					=	75,		// 修为【不再使用】
	ERA_Hostility				=	76,		// 戾气	【不再使用】

	ERA_Spirit					=	77,		// 精力【不再使用】
	ERA_Shili					=	78,		// 实力

	ERA_WuXun					=	79,		// 武勋
	ERA_WuJi					=	80,		// 武技

	ERA_Weary					=	81,     // 疲劳值
	
	ERA_Soar					=   82,		// 飞升状态

	ERA_Toughness				=	83,		// 韧性【不再使用】

	ERA_transform_Start			=	84,
	ERA_transform_Bang			=	84,		// 神兵伤害加深
	ERA_transform_Brunt			=	85,		// 玄翎伤害加深
	ERA_transform_Injury		=	86,		// 乾坤伤害加深
	ERA_transform_Thinker		=	87,		// 仙音伤害加深
	ERA_transform_ExAttack		=	88,		// 物理伤害加深
	ERA_transform_InAttack		=	89,		// 法术伤害加深
	ERA_transform_ALL			=	90,		// 全部伤害加深
	ERA_transform_End			=	90,

	ERA_VipLevel				=   91,		// 角色的Vip等级

	// 佩戴特殊妖精才具有的伤害属性，怪除外，怪可以有这个伤害
	ERA_EEI_START	= 92,
	ERA_EEI_Fire	= 92,	// 火元素
	ERA_EEI_Water	= 93,	// 水元素
	ERA_EEI_Earth	= 94,	// 土元素
	ERA_EEI_Wind	= 95,	// 风元素
	ERA_EEI_Bright	= 96,	// 光明元素
	ERA_EEI_Dark	= 97,	// 黑暗元素
	ERA_EEI_End		= 97,
	// 佩戴妖精才有的抗性，当然有些怪除外，怪可以有抗性
	ERA_EER_START	= 98,
	ERA_EER_Fire	= 98,	// 火元素抗性
	ERA_EER_Water	= 99,	// 水元素抗性
	ERA_EER_Earth	= 100,	// 土元素抗性
	ERA_EER_Wind	= 101,	// 风元素抗性
	ERA_EER_Bright	= 102,	// 光明元素抗性
	ERA_EER_Dark	= 103,	// 黑暗元素抗性
	ERA_EER_End		= 103,

	ERA_InstanceWearyEffect		=	104,	// 进入副本疲劳影响

	ERA_Target					= 105,			// 目标，用于“目标的目标”

	// 神系统添加属性start
	ERA_God_Godhead   = 106, // 神格
	ERA_God_Faith     = 107, // 信仰
	ERA_God_Condenced = 108, // 凝聚度
	ERA_Mana_combustion_resistance                =			109,//法力燃烧抵抗
	ERA_Toughness_god                             =			110,//韧性
	ERA_Toughness_strength                        =			111,//韧性强度
	ERA_Control_effect_resistance                 =			112,//控制效果抵抗//废弃，不可使用！！！2011.9.16
	ERA_Contrel_effect_deepen                     =			113,//控制效果加深
	ERA_Fixed_damage_aggrandizement               =			114,//固定伤害强化
	ERA_Aging_effect_strengthening                =			115,//衰老效果强化
	ERA_Slowing_effect_strengthening              =			116,//减速效果强化
	ERA_Physical_damage_rebound_num               =			117,//物理反弹数值伤害
	ERA_Physical_damage_rebound_ratio             =			118,//物理反弹比率伤害 
	ERA_Magic_damage_rebound_num                  =			119,//法术反弹数值伤害
	ERA_Magic_damage_rebound_ratio                =			120,//法术反弹比率伤害
	ERA_Physical_damage_rebound_immune            =			121,//物理反弹伤害免疫
	ERA_Magic_damage_rebound_immune               =			122,//法术反弹伤害免疫
	ERA_Mana_combustion                           =			123,//法力燃烧
	
	ERA_God_Day_Faith = 124,	//每日信仰

	//没事折腾加的
	ERA_Nosee_attack = 125,		//无视攻击
	ERA_Nosee_defend = 126,		//无视防御

	// 神系统添加属性end
	ERA_Soul		= 127, //神魂
	//鲜花，鸡蛋，蛋疼的属性
	ERA_FlowerNum	= 128,
	ERA_EggsNum		= 129,

	// 妖精穿透 [3/12/2012 zhangzhihua]
	ERA_EE_ATTR = 130,			// 妖精伤害穿透元素属性 [3/12/2012 zhangzhihua]
	ERA_EEP_VALUE = 131,			// 妖精穿透值 [3/12/2012 zhangzhihua]
	ERA_EER_ALL = 132,			// 妖精元素抗性通用值 [3/20/2012 zhangzhihua]

	//圣灵值
	ERA_Holy = 133,
	//圣魔 命中
	ERA_HitRateAdd = 134,
	//元素伤害
	ERA_EEI_ALL = 135,

	ERA_End,
	// 玩家的姓名，声望，投放属性点，已经投放的天资点没有放到基本属性中
};

//-----------------------------------------------------------------------------
// 元神基础属性枚举类型
//-----------------------------------------------------------------------------

enum EHolyAttType	//圣灵升级后强化的属性，这个值的计算是Proto表加上升级提升
{
	ESAT_NULL				= -1,	// 无

	ESAT_Demage				= 0,	//圣灵伤害
	ESAT_HolyDef			= 1,	//圣灵防御
	ESAT_Crit				= 2,	//致命
	ESAT_CritRate			= 3,	//致命量
	ESAT_ExDmg				= 4,	//伤害加深
	ESAT_AttackTec			= 5,	//精准0
	ESAT_NeglectToughness	= 6,	//灵巧
	ESAT_LingNeng			= 7,	//灵能

	ESAT_End				= 8,	//---type 数目
};

const INT32	X_ERA_ATTA_NUM		= ERA_AttA_End - ERA_AttA_Start + 1;
const INT32	X_ESAT_ATTA_NUM		= ESAT_LingNeng - ESAT_Demage;

#define MTransERAAttA2Index(ERAAttA)		((ERAAttA) - ERA_AttA_Start)
#define MTransIndex2ERAATTA(Index)			((Index) + ERA_AttA_Start)

const INT32 X_ERA_AttB_NUM		= ERA_AttB_End - ERA_AttB_Start;
#define MTransERAAttB2Index(ERAAttB)		((ERAAttB) - ERA_AttB_Start)
#define MTransIndex2ERAATTB(Index)			((Index) + ERA_AttB_Start)

const INT32	X_ERA_DERATE_NUM	= ERA_Derate_End - ERA_Derate_Start + 1;
#define MTransERADerate2Index(ERADerate)	((ERADerate) - ERA_Derate_Start)
#define MTransIndex2ERADerate(Index)		((Index) + ERA_Derate_Start)

const INT32 X_ERA_TRANSFORM_NUM	= ERA_transform_End - ERA_transform_Start + 1;
#define MTransERATransform2Index(ERATransform)	(X_ERA_DERATE_NUM + (ERATransform) - ERA_transform_Start)
#define MTransIndex2ERATransform(Index)			((Index) - X_ERA_DERATE_NUM + ERA_transform_Start)

const INT32 X_ERA_BRAND_NUM = X_ERA_TRANSFORM_NUM + X_ERA_DERATE_NUM;

//--------------------------------------------------------------------------------------
// 远程玩家的属性枚举
//--------------------------------------------------------------------------------------
enum ERemoteRoleAtt
{
	ERRA_Null		=	-1,		//

	ERRA_MaxHP		=	0,		// 最大体力
	ERRA_HP			=	1,		// 当前体力
	ERRA_MaxMP		=	2,		// 最大真气
	ERRA_MP			=	3,		// 当前真气
	ERRA_Rage		=	4,		// 怒气
	ERRA_Speed_XZ	=	5,		// XZ平面速度
	ERRA_Speed_Y	=	6,		// Y平面速度
	ERRA_Speed_Swim	=	7,		// 游泳速度
	ERRA_Speed_Mount=	8,		// 骑乘速度
	ERRA_Shape		=	9,		// 体型
	ERRA_Hostility	=	10,		// 戾气状态
	// Jason 2010-5-27 v2.2.0
	ERRA_Soaring	=	11,		// 飞升标志
	ERRA_VipLevel	=	12,		// VIP等级

	ERRA_End		=	13,
};

//-----------------------------------------------------------------------------------------
// 目标类型标示位
//-----------------------------------------------------------------------------------------
enum ETargetFlag
{
	ETF_Self				=	0x00000001,	// 自身
	ETF_Teammate			=	0x00000002,	// 队友
	ETF_Guildmate			=	0x00000004,	// 同门
	ETF_Lover				=	0x00000008,	// 情侣
	ETF_Couple				=	0x00000010,	// 配偶
	ETF_Brother				=	0x00000020,	// 结拜
	ETF_Teacher				=	0x00000040,	// 师徒
	ETF_Player				=	0x00000080,	// 玩家

	ETF_Pet					=	0x00000100,	// 宠物
	ETF_NPC					=	0x00000200,	// NPC
	ETF_NormalMonster		=	0x00000400,	// 普通怪物
	ETF_Boss				=	0x00000800,	// Boss
	ETF_Nest				=	0x00001000,	// 巢穴
	ETF_NatuRes				=	0x00002000,	// 自然资源
	ETF_ManRes				=	0x00004000,	// 人造资源
	ETF_Door				=	0x00008000,	// 城门
	ETF_Building			=	0x00010000,	// 建筑
	ETF_NormalGameObject	=	0x00020000,	// 普通可交互地物
	ETF_InvesGameObject		=	0x00040000,	// 可调查地物
	ETF_QuestTalkGameObject	=	0x00080000,	// 任务对话地物
	ETF_Remnant				=   0x00100000,	// 神迹
	ETF_FairySoul			=   0x00200000,	// 妖精魂魄
	ETF_FairyHeritage		=	0x00400000,	// 妖精遗产，宝箱
};

//-----------------------------------------------------------------------------------------
// 状态标示位
//-----------------------------------------------------------------------------------------
enum EStateFlag
{
	ESF_Dead			=	0x00000001,	// 死亡
	ESF_NoDead			=	0x00000002,	// 非死亡
	ESF_Dizzy			=	0x00000004,	// 眩晕
	ESF_NoDizzy			=	0x00000008,	// 非眩晕
	ESF_Spor			=	0x00000010,	// 昏睡
	ESF_NoSpor			=	0x00000020,	// 非昏睡
	ESF_Tie				=	0x00000040,	// 定身
	ESF_NoTie			=	0x00000080,	// 非定身
	ESF_Invincible		=	0x00000100,	// 无敌
	ESF_NoInvincible	=	0x00000200,	// 非无敌
	ESF_NoSkill			=	0x00000400,	// 封技
	ESF_NoNoSkill		=	0x00000800, // 非封技
	ESF_DisArm			=	0x00001000,	// 缴械
	ESF_NoDisArm		=	0x00002000,	// 非缴械
	ESF_Lurk			=	0x00004000,	// 隐身
	ESF_NoLurk			=	0x00008000,	// 非隐身
	ESF_Mount			=	0x00010000,	// 骑乘
	ESF_NoMount			=	0x00020000, // 非骑乘
	ESF_Prison			=	0x00040000,	// 牢狱
	ESF_NoPrison		=	0x00080000,	// 非牢狱
	ESF_Mount2			=	0x00100000,	// 骑乘2
	ESF_NoMount2		=	0x00200000, // 非骑乘2
	ESF_Commerce		=	0x00400000,	// 跑商中
	ESF_NoCommerce		=	0x00800000,	// 非跑商中

	ESF_Transform		=	0x01000000, // 变形
	ESF_NoTransform	=	0x02000000,	// 非变形
};

//-----------------------------------------------------------------------------------------
// 人物生物基本属性的默认值及最大最小值
//-----------------------------------------------------------------------------------------
struct tagAttDefMinMax
{
	INT		nDefRole;			// 人物初始化默认值
	INT		nDefCreature;		// 生物初始化默认值
	INT		nMin;				// 属性最小值
	INT		nMax;				// 属性最大值
};

////---------------------------------------------------------------------------------------
//// 客户端本地玩家同步数据消息结构（未完成）
////---------------------------------------------------------------------------------------
//struct tagRoleData
//{
//	DWORD			dwID;						// 玩家ID
//	DWORD			dwRoleNameCrc;				// 名称CRC
//	TCHAR			szRoleName[X_SHORT_NAME];	// 名称
//
//	DWORD			dwMapID;					// 所在的地图的ID
//	FLOAT			fPos[3];					// 位置
//	FLOAT			fFaceTo[3];					// 朝向
//
//	tagAvatarAtt	Avatar;						// 外观
//	tagAvatarEquip	AvatarEquip;				// 装备外观
//	tagDisplaySet	DisplaySet;					// 外观显示设置
//	INT				nAtt[ERA_End];				// 当前人物属性
//
//	INT				nAttPointAdd[X_ERA_ATTA_NUM];// 玩家投放到各个一级属性中的值
//
//	EClassType		eClass;						// 职业
//	EClassTypeEx	eClassEx;					// 扩展职业
//
//	INT16			n16MaxBagSize;				// 背包大小
//	INT16			n16MaxWareSize;				// 账号仓库大小
//
//	INT				nSkillNum;					// 技能个数
//	INT				nBuffNum;					// 状态个数
//	INT				nTitleNum;					// 称号个数
//	INT				nItemNum;					// 物品个数
//	INT				nEquipNum;					// 装备个数
//	INT				nFriendNum;					// 好友个数
//	INT				nEnemyNum;					// 仇敌个数
//	INT				nQuestNum;					// 任务列表个数
//	INT				nQuestDoneNum;				// 已完成的任务个数
//
//	/*********************************************************************************************************
//	 *技能列表，状态列表，称号列表，物品列表，装备列表，好友列表，仇敌列表，任务列表，已完成任务列表
//	 *必须按顺序拷贝
//	 *********************************************************************************************************/
//	//任务列表
//	DWORD			dwIncompleteQuestID[QUEST_MAX_COUNT];			// 在当前玩家身上未完成的任务 30上限
//	DWORD			dwIncompleteQuestTakeTime[QUEST_MAX_COUNT];		// 在当前玩家身上当前任务的接取时间 30上限
//	//任务列表
//	
//
//	BYTE			byData[1];
//};

//------------------------------------------------------------------------------------------
// 在选人界面要看到的人物信息
//------------------------------------------------------------------------------------------
struct tagSimRole								// size = 64
{
	tagAvatarAtt	Avatar;						// size = 16
	tagAvatarEquip	AvatarEquip;				// size = (2 * 9 + 1) * 4
	TCHAR			szRoleName[X_SHORT_NAME];

	DWORD			dwRoleID;
	DWORD			dwMapID;
	BYTE			byLevel;
	tagDisplaySet	sDisplaySet;
	BOOL			bOldPlayerBack;
	DWORD			timeLogout; // 最近一次登出时间
	UINT16			dwBackDays;
	BYTE			byReserved[2];
};

//------------------------------------------------------------------------------------------
// 是否对远端玩家公开相关信息 -- 注：数据库表记录用32位
//------------------------------------------------------------------------------------------
struct tagRemoteOpenSet
{
	DWORD	bTitle		: 1,	// 称号
			bVCard		: 1,	// 名帖
			bRoleAtt	: 1,	// 角色属性
			bDummy		: 29;	// 占位用

	tagRemoteOpenSet() { ZeroMemory(this, sizeof(*this)); }
	
	tagRemoteOpenSet& operator = (DWORD dw)
	{
		return *this = *(tagRemoteOpenSet*)&dw;
	}

	operator DWORD()
	{
		return *(DWORD*)this;
	}
};


//------------------------------------------------------------------------------------------
// 玩家信息的公开类型
//------------------------------------------------------------------------------------------
enum EPlayerPersonlInfoOpenType
{
	EPIS_Private,  // 完全保密
	EPIS_Friend,   // 仅对互为好友的玩家公开
	EPIS_Public,   // 完全公开
	EPIS_End,
};

//------------------------------------------------------------------------------------------
// 玩家的个人设置 
// 注意 
// 每次新增字段要向1个字节对齐 如果不足1字节请加填充位
//------------------------------------------------------------------------------------------
struct tagDWPlayerPersonalSet
{
	BYTE	bAcceptTeamInvite		: 1,	// 是否接受组队邀请
			bAcceptFactionInvite	: 1,	// 是否接受帮派邀请
			bAllowFriendAdd			: 1,	// 是否允许其他人添加自己为好友
			bAcceptTradeRequest		: 1,	// 是否接受交易请求
			eInfoOpenType           : 4;    // 信息的公开类型

	tagDWPlayerPersonalSet() 
	{ 
		bAcceptTeamInvite		=	true;
		bAcceptFactionInvite	=	true;
		bAllowFriendAdd			=	true;
		bAcceptTradeRequest		=	true;
		eInfoOpenType			=	EPIS_Public;
	}	

	tagDWPlayerPersonalSet& operator = (const tagDWPlayerPersonalSet &set)
	{
		bAcceptTeamInvite		= set.bAcceptTeamInvite;
		bAcceptFactionInvite	= set.bAcceptFactionInvite;
		bAllowFriendAdd			= set.bAllowFriendAdd;
		bAcceptTradeRequest		= set.bAcceptTradeRequest;
		eInfoOpenType			= set.eInfoOpenType;

		return *this;
	}

	tagDWPlayerPersonalSet(const tagDWPlayerPersonalSet& set)
	{
		*this = set;
	}

};

//-------------------------------------------------------------------------------------
// 实力值
//-------------------------------------------------------------------------------------
struct tagStrength
{
	INT nStrength;					// 总实力值，参与实力排名

	// 总实力值相关三项
	INT nBaseStrength;				// 固有实力值
	INT	nAdvanceStrength;			// 进阶实力值
	INT nConsumptiveStrength;		// 可消耗实力值

	// 固有实力值相关各项
	INT	nLevel;						// 等级
	INT nEquipValue;				// 装备实力
	INT	nPet;						// 宠物实力
	INT nFriendship;				// 好友：玩家好友列表中友好度超过300的好友数量*5
	INT	nQuest;						// 任务：所有已完成的非重复非周期性任务数量/5
	INT nProduce;					// 生产技能：玩家已学所有生产技能等级之和*50
	INT nFame;						// 声望
	INT nTitle;						// 称号
	INT nFairy;                     // 妖精
	// 等级

	// 进阶实力值相关各项
	INT nActivity;					// 活动相关
	INT nRanQuest;					// 每日、随机任务
	INT nInstance;					// 副本
	INT nKill;						// 击杀非白名玩家

	tagStrength()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//------------------------------------------------------------------------------------------
// 枚举玩家竞技属性
//------------------------------------------------------------------------------------------
enum EAthleticsProperty
{
	EAP_ValiantMerit = 0,  //武勋
	EAP_ValiantSkill = 1,  //武技
	EAP_End,
};

//------------------------------------------------------------------------------------------
// 枚举玩家每一武勋等级对应的武勋值
//------------------------------------------------------------------------------------------
enum EMAXVALIANT_PERLEVEL
{
	EMVPL_Level1_Max_Valiant =      199,
	EMVPL_Level2_Max_Valiant =      399,
	EMVPL_Level3_Max_Valiant =      599,
	EMVPL_Level4_Max_Valiant =      799,
	EMVPL_Level5_Max_Valiant =      999,
	EMVPL_Level6_Max_Valiant =      1199,
	EMVPL_Level7_Max_Valiant =      1399,
    EMVPL_Level8_Max_Valiant =      1599,
    EMVPL_Level9_Max_Valiant =      1799,
	EMVPL_Level10_Max_Valiant=      9999,
	EMVPL_End,
};

//-------------------------------------------------------------------------------------
// 竞技获得相关角色熟悉
//-------------------------------------------------------------------------------------

const INT16 MAX_ATHLETICS_PROPERTY = 9999;

//-------------------------------------------------------------------------------------
// 角色的VIP状态属性枚举
//-------------------------------------------------------------------------------------
enum EVipLevel
{
	EVL_None		= 0, // 不是vip
	EVL_Silver		= 1, // 银钻级vip
	EVL_Gold			= 2, // 金钻级vip
	EVL_Platinum	= 3, // 白金级vip

	EVL_Max,
};

#define DEFAULT_MAX_VIP_DAYS 30
#define MAX_DAYS_SECOND(MaxDays) ((MaxDays)*86400) // MaxDays天有多少秒24*60*60
#define SILVER_VIP_BUFF			9003401 // 银钻级VIP
#define GOLDEN_VIP_BUFF		9003402 // 金钻级VIP
#define PLATINUM_VIP_BUFF	9003403 // 白金级VIP

#define MaxExpPillularBaseTimes 20
#define MaxExpPillularUseTimes(VipLvl) ((VipLvl+1)*MaxExpPillularBaseTimes)

#define GetGodHeadLimit( level,maxvalue )	do{	\
		if(level < 20)	\
		{	\
			maxvalue = 0;	\
			break;			\
		}	\
		else if(level < 30)	\
		{	\
			maxvalue = 1;	\
			break;			\
		}	\
		else if(level < 40)	\
		{	\
			maxvalue = 2;	\
			break;			\
		}	\
		else if(level < 50)	\
		{	\
			maxvalue = 3;	\
			break;			\
		}	\
		else if(level < 60)	\
		{	\
			maxvalue = 5;	\
			break;			\
		}	\
		else if(level < 70)	\
		{	\
			maxvalue = 7;	\
			break;			\
		}	\
		else if(level < 80)	\
		{	\
			maxvalue = 9;	\
			break;			\
		}	\
		else if(level < 90)	\
		{	\
			maxvalue = 15;	\
			break;			\
		}	\
		else if(level <= 100)	\
		{	\
			maxvalue = 20;	\
			break;			\
		}	\
	}while(0)

enum EAccountSpec
{
	EAS_Null = -1,
	EAS_Normal = 0,	// 普通账号
	EAS_FromEM = 1, // 来自恶魔法则
	EAS_FromEMRewared = 2, // 来自恶魔，已领取过奖励
};

enum ERoleAttrSync
{
	ERoleAttrWarGold = 0,					// 战场金币
	ERoleAttrMonsterKillCount,				// 击杀怪物计数
	ERoleAttrBossKillCount,					// 击杀boss计数(首领)
	ERoleAttrRoleKillCount,					// 阵亡次数
	ERoleAttrRareBossKillCount,				// 击杀boss计数(稀有)
	//ERoleAttrRoleDeadCount,				// 击杀敌方怪物数量
	//ERoleAttrNeutralMonsterKillCount,		// 中立怪物击杀数量

	ERoleAttrEnd,
};

enum EWarRoleResult
{
	EWRR_NULL		= 0,
	EWRR_Lose,
	EWRR_Win,

	EWRR_End,
};

const DWORD REFRESH_BAOXIANG_ITEMID = 3110037;

#pragma pack(pop)
