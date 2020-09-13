//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: skill_define.h
// author: Aslan
// actor:
// data: 2008-09-08
// last:
// brief: 技能静态属性
//-----------------------------------------------------------------------------
#pragma once

#include "RoleDefine.h"
#include "buff_define.h"

#pragma pack(push, 1)

const INT MAX_CHANNEL_TIMES = 5;		// 管道攻击的最多次数
const INT MAX_BUFF_PER_SKILL = 3;		// 每个技能所带的最多buff数量
const INT MAX_BUFF_PER_ITEM = 3;		// 每个物品所带的最多buff数量

const INT MAX_ROLE_PRODUCE_NUM = 3;		// 每个玩家身上所学的最大生产技能的数量
const INT MAX_SKILL_MAP = 12;	// 有地图限制的技能最多可以在多少个地图使用
//------------------------------------------------------------------------------
// 技能类型
//------------------------------------------------------------------------------
enum ESkillType
{
	ESST_Null		=	0,
	ESST_Role		=	1,		// 角色技能
	ESST_Creature	=	2,		// 怪物技能
	ESST_Item		=	3,		// 物品技能
	ESST_Social		=	4,		// 社会技能
	ESST_Pet		=	5,		// 宠物技能
};

enum ESkillTypeEx
{
	ESSTE_Null		=	0,
	ESSTE_Default	=	1,		// 普通攻击
	ESSTE_Sword		=	2,		// 剑
	ESSTE_Blade		=	3,		// 刀
	ESSTE_Wand		=	4,		// 长兵
	ESSTE_Bow		=	5,		// 弓
	ESSTE_Valor		=	6,		// 阳刚
	ESSTE_Supple	=	7,		// 阴柔
	ESSTE_Poison	=	8,		// 医毒
	ESSTE_Swing		=	9,		// 音律
	ESSTE_Action	=	10,		// 战技
	ESSTE_Jugglery	=	11,		// 奇术
	ESSTE_Produce	=	12,		// 生产技能
	ESSTE_Guild		=	13,		// 帮派技能
	ESSTE_Valentine	=	14,		// 情侣技能
	ESSTE_Consort	=	15,		// 夫妻技能
	ESSTE_Brother	=	16,		// 兄弟技能
	ESSTE_Teacher	=	17,		// 师徒技能
	ESSTE_Monster	=	18,		// 怪物普通技能
	ESSTE_MonsterAI	=	19,		// 怪物AI技能
	ESSTE_ItemUse	=	20,		// 物品技能
	ESSTE_Equipment	=	21,		// 装备技能
	ESSTE_Longhun	=	22,		// 龙魂技能
	ESSTE_Society	=	23,		// 社会技能
	ESSTE_PetSkill	=	24,		// 宠物技能
	ESSTE_SealSkill	=	25,		// 封印技能
	ESSTE_LowerImmortal	 = 26,	// 下级仙技
	ESSTE_NormalImmortal = 27,	// 中级仙技
	ESSTE_HighImmortal	 = 28,	// 高级仙技
	ESSTE_FreshHandNomal = 29,	// 新手强攻
	ESSTE_FreshHandRanged= 30,	// 新手群攻
	ESSTE_FreshHandEnhanced = 31,// 新手暴气
	// 妖精蛋调查技能
	ESSTE_FairySoul				= 32,	// 妖精蛋调查
	ESSTE_FairyHeitage			= 33,	// 妖精遗产，妖精宝箱
	
	ESSTE_Transform				= 34,	// 变身技能
	ESSTE_GodArea				= 35,    // 领域技能
	ESSTE_SingleTarget			= 36,	// 只有一个目标的技能
	ESSTE_SingleTargetC			= 37,

	ESSTE_SoulSkill = 39, // 魂晶技能
	ESSTE_SoulSubSkill = 40, // 真实魂晶技能
};

// nType3字来判断生产技能类型
enum ESkillTypeEx2
{
	ESSTE2_NULL		= 0,		//
	ESSTE2_Mining	= 1,		//开采
	ESSTE2_Harvestry,			//收获
	ESSTE2_Smilt,				//冶炼
	ESSTE2_Artisan,				//巧匠
	ESSTE2_DanTraining,			//炼丹
	ESSTE2_Smith,				//锻造
	ESSTE2_Casting,				//铸甲
	ESSTE2_Dressmaker,			//裁缝
	ESSTE2_Aechnics,			//工艺
	ESSTE2_PointUp,				//点化
	ESSTE2_Hidden,				//不显示的技能

	ESSTE2_GodCondense = 12,	// 神，凝聚神格

    ESSTE_End
};

//------------------------------------------------------------------------------
// 技能目标类型
//------------------------------------------------------------------------------
enum ESkillTargetType
{
	ESTT_Null		=	0,
	ESTT_Buff		=	1,		// 状态
	ESTT_Skill		=	2,		// 技能
	ESTT_Trigger	=	3,		// 触发器
	ESTT_Combat		=	4,		// 战斗目标
	ESTT_NoCombat	=	5,		// 非战斗目标
};

//-------------------------------------------------------------------------------
// 技能距离类型
//-------------------------------------------------------------------------------
enum ESkillDistType
{
	ESDT_Null		=	0,
	ESDT_Melee		=	1,		// 近身
	ESDT_Ranged		=	2,		// 远程
	ESDT_Inner		=	3,		// 内功
};

//-------------------------------------------------------------------------------
// 技能施展类型
//-------------------------------------------------------------------------------
enum ESkillUseType
{
	ESUT_Null		=	0,
	ESUT_Active		=	1,		// 主动
	ESUT_Passive	=	2,		// 被动
};

//-------------------------------------------------------------------------------
// 技能作用类型
//-------------------------------------------------------------------------------
enum ESkillOPType
{
	ESOPT_NUll		=	0,
	ESOPT_Rect		=	1,		// 矩形
	ESOPT_Explode	=	2,		// 爆炸
};

//--------------------------------------------------------------------------------
// 技能伤害类型
//--------------------------------------------------------------------------------
enum ESkillDmgType
{
	ESDGT_Null		=	0,
	ESDGT_Ordinary	=	1,		// 普通伤害类型
	ESDGT_Bleeding	=	2,		// 出血伤害类型
	ESDGT_Brunt		=	3,		// 冲击伤害类型
	ESDGT_Bang		=	4,		// 重击伤害类型
	ESDGT_Poison	=	5,		// 毒性伤害类型
	ESDGT_Thinker	=	6,		// 心智伤害类型
	ESDGT_Injury	=	7,		// 内损伤害类型
	ESDGT_Stunt		=	8,		// 绝技伤害类型
};

//---------------------------------------------------------------------------------
// 技能消耗类型
//---------------------------------------------------------------------------------
enum ESkillCostType
{
	ESCT_HP			=	0,		// 体力
	ESCT_MP			=	1,		// 真气
	ESCT_Rage		=	2,		// 怒气
	ESCT_Endurance	=	3,		// 持久
	ESCT_Valicity	=	4,		// 活力
	ESCT_End		=	5,
};

//---------------------------------------------------------------------------------
// 技能升级类型
//---------------------------------------------------------------------------------
enum ESkillLevelUpType
{
	ESLUT_Fixed		=	0,		// 固定
	ESLUT_Exp		=	1,		// 熟练度
	ESLUT_Book		=	2,		// 技能书
};

//---------------------------------------------------------------------------------
// 技能性别使用限制
//---------------------------------------------------------------------------------
enum ESkillSexLimit
{
	ESSL_Null		=	0,
	ESSL_Man		=	1,
	ESSL_Woman		=	2,
};

//---------------------------------------------------------------------------------
// 技能职业使用限制
//---------------------------------------------------------------------------------
//enum ESkillClassLimit
//{
//
//};

//---------------------------------------------------------------------------------
// 技能武器限制
//---------------------------------------------------------------------------------
enum ESkillWeaponLimit
{

};

//---------------------------------------------------------------------------------
// 技能位置限制
//---------------------------------------------------------------------------------
enum ESkillPosType
{
	ESPT_NUll		=	0,		// 无限制
	ESPT_Front		=	1,		// 在身前
	ESPT_Back		=	2,		// 在身后
};


//技能扩展功能
enum ESkillSpecFunc
{
	ESSF_NULL			=	0,
	ESSF_Strengthen		=	1,		// 强化成功率相关，低于等于一个等级的都可以强化
	ESSF_Valicity		=	2,		// 活力消耗相关
	ESSF_FabaoExp		=	3,		// 妖精经验相关
	ESSF_Gem			=	4,		// 宝石升级相关
	ESSF_Brand			=	5,		// 圣印升级相关
	ESSF_SkillMap		=	6,		// 技能地图限制相关	
	ESSF_Strengthen1	=	7,		// 有一个强化，高于一个等级的都可以强化
	ESSF_FreeOpenFace	=	8,		// 随机开界面

	ESSF_END			=	9,
};

//技能扩展功能,活力消耗相关
enum ESkillSpecVitality
{
	ESSV_NULL			=	0,
	ESSV_ALL			=	1,		//全部
    ESSV_COMPOSE		=	2,		//点化
	ESSV_CONSOLODATE	=	3,		//合成

	ESSV_END			=	4,
};

//技能扩展功能,随身打开界面相关
enum ESkillSpecOpenFace
{
	ESSOF_NULL			=	0,
	ESSOF_FLY			=	1,		//飞翔石界面
    ESSOF_AWARD			=	2,		//悬赏榜
	ESSOF_SHOP			=	3,		//商店

	ESSOF_END			=	4,
};



//---------------------------------------------------------------------------------
// 技能静态属性
//----------------------------------------------------------------------------------
struct tagSkillProto 
{
	DWORD				dwID;								// 技能ID
	ESkillType			eType;								// 技能类型
	INT					nType2;								// 技能子类型
	INT					nType3;								// 技能子类型
	DWORD				dwMainSkillID;						// 主技能ID（如果本身为主技能，则该项目不填写）
	ETalentType			eTalentType;						// 天资类型

	// 目标类型
	ESkillTargetType	eTargetType;						// 目标对象类型
	DWORD				dwTargetSkillID;					// 技能目标ID（只是针对于目标对象类型为技能）
	DWORD				dwTargetBuffID;						// 状态目标ID（只是针对与目标对象类型为状态）
	DWORD				dwTargetTriggerID;					// 触发器目标ID（只是针对与目标对象类型为技能的触发器）

	// 技能敌我判断标志
	BOOL				bFriendly;							// 对友方使用
	BOOL				bHostile;							// 对敌方使用
	BOOL				bIndependent;						// 对中立方使用
	
	// 攻击判定
	ESkillDistType		eDistType;							// 攻击距离类型（近身，远程）
	ESkillUseType		eUseType;							// 施展类型（主动，被动）
	ESkillOPType		eOPType;							// 作用类型
	FLOAT				fOPDist;							// 作用距离（世界单位）
	FLOAT				fOPRadius;							// 作用范围（世界单位）
	BOOL				bInterruptMove;						// 是否移动打断起手
	INT					nInterruptSkillOrdRate;				// 普通技能打断几率
	INT					nInterruptSkillSpecRate;			// 特殊技能打断几率
	INT					nPrepareTime;						// 起手时间（毫秒）
	INT					nCoolDown;							// 冷却时间（毫秒）
	ESkillDmgType		eDmgType;							// 伤害类型
	INT					nEnmity;							// 技能仇恨
	INT					nHit;								// 技能命中率
	INT					nCrit;								// 技能致命率
	BOOL				bMoveable;							// 是否可以移动施放
	FLOAT				fActionTime;						// 动作数值时间

	// 管道
	INT					nChannelDmg[MAX_CHANNEL_TIMES];		// 管道伤害每一次的伤害量
	INT					nChannelTime[MAX_CHANNEL_TIMES];	// 管道伤害每一次的时间
	INT					nDmgTimes;							// 管道伤害总次数

	// 状态
	DWORD				dwBuffID[MAX_BUFF_PER_SKILL];		// 技能所带buff
	DWORD				dwTriggerID[MAX_CHANNEL_TIMES];		// 技能所带的buff触发器

	// 消耗
	DWORD				dwCostItemID;						// 消耗物品ID
	INT					nCostItemNum;						// 消耗物品数量
	INT					nSkillCost[ESCT_End];				// 技能消耗

	// 升级
	INT					nLevel;								// 技能等级
	ESkillLevelUpType	eLevelUpType;						// 升级类型（固定，熟练度）
	INT					nLevelUpExp;						// 需要熟练度（针对于以熟练度升级的技能）
	DWORD				dwPreLevelSkillID;					// 前置技能属性ID
	INT					nMaxLevel;							// 等级上限
	INT					nMaxLearnLevel;						// 技能可学上限（小于等于等级上限）
	INT					nNeedRoleLevel;						// 技能升级的玩家等级限制
	INT					nNeedTalentPoint;					// 技能升级需要的玩家当前天资投点数
	//EClassType			eNeedClassType;						// 技能升级需要的职业类型

	// 学习技能
	DWORD				dwLearnVocationLimit;				// 学习技能的职业限制

	// 使用限制
	INT					nUseHPPctLimit;						// 体力百分比限制
	INT					nUseMPPctLimit;						// 真气百分比限制
	ESkillSexLimit		eSexLimit;							// 性别限制
	EClassType			eClassLimit;						// 职业限制
	INT					nWeaponLimit;						// 武器限制
	ESkillPosType		ePosType;							// 位置限制
	DWORD				dwTargetLimit;						// 目标对象限制
	DWORD				dwSelfStateLimit;					// 自身状态限制
	DWORD				dwTargetStateLimit;					// 目标状态限制
	BOOL				bInterCombat;						// 使用后是否进入备战
	DWORD				dwBuffLimitID;						// 自身特殊状态限制ID
	DWORD				dwTargetBuffLimitID;				// 目标特殊状态限制ID
	DWORD				dwVocationLimit;					// 职业限制
	// Jason 2010-5-18 v2.1.2 飞升技能限制，只有开启飞升才能学习
	BOOL				bSoarLimitLearn;
	
	// 触发器加成
	INT					nTriggerEventMisc1Add;				// 事件触发值1加成
	INT					nTriggerEventMisc2Add;				// 事件触发值2加成
	INT					nTriggerStateMisc1Add;				// 状态触发值1加成
	INT					nTriggerStateMisc2Add;				// 状态触发值2加成
	INT					nTriggerPropAdd;					// 触发几率加成

	// 状态加成
	INT					nBuffPersistTimeAdd;				// 持续时间加成（毫秒）
	INT					nBuffWarpTimesAdd;					// 叠加次数上限加成
	INT					nBuffInterruptResistAdd;			// 攻击消除抗性加成
	EBuffEffectMode		eModBuffMode;						// 影响Buff的哪个特殊效果阶段
	INT					nBuffMisc1Add;						// 特殊效果值加成
	INT					nBuffMisc2Add;						// 特殊效果值加成
	INT					nBuffEffectAttMod[EBEA_End];		// 影响的某个阶段的人物当前属性

	// 人物属性影响
	mutable TMap<ERoleAttribute, INT>	mapRoleAttMod;		// 人物属性影响
	mutable TMap<ERoleAttribute, INT>	mapRoleAttModPct;	// 人物属性百分比影响		
	// 所有的字符串属性没有添加在里面，这些属性有：
	// 名称，描述，动画编号，特效编号，图标编号，起手动作编号

	INT					iMaxAttackNum;						// 最大攻击人数/被动技能修改盒里里面的所有技能的最大攻击人数
	INT					iModBufAttackNum;					// 被动技能修改状态的最大攻击人数
	INT					iTopHit;							// 最大伤害/被动技能修改盒子里面所有技能的最大伤害
	DWORD				dwCutChannel;						// 技能可打断的管道点
	FLOAT					fDamageAddFactor;				// 伤害增值系数

	ESkillSpecFunc		eSpecFunc;
	INT					nSpecFuncVal1;
	INT					nSpecFuncVal2;
};

//--------------------------------------------------------------------------------
// 技能传送客户端的消息结构
//--------------------------------------------------------------------------------
struct tagSkillMsgInfo
{
	DWORD	dwID;							// 技能ID
	INT		nLevel;							// 当前等级
	INT		nLearnLevel;					// 当前投点等级
	INT		nCoolDown;						// 冷却
	INT		nMaxCoolDown;					// 技能的最大冷却时间
	INT		nProficiency;					// 熟练度
	FLOAT	fOpDist;						// 作用距离
	INT		nPrepareTime;					// 起手时间
	INT		nChannelDmg[MAX_CHANNEL_TIMES];	// 伤害
	INT		nCost[ESCT_End];				// 消耗
	BOOL	bExtraSkill;					// 是否是临时技能
	UINT	unActiveTime;							// 神魔激活技能的剩余时间（秒）
};

//--------------------------------------------------------------------------------
// 技能模板结构客户端版
//--------------------------------------------------------------------------------*/
struct tagSkillProtoClient : public tagSkillProto
{
	TCHAR					szName[X_SHORT_NAME];			// 技能名称
	TCHAR					szDesc[X_HUGE_NAME];			// 技能描述
	TCHAR					szIcon[X_SHORT_NAME];			// 技能图标
	TCHAR					szActionMod[X_SHORT_NAME];		// 技能使用动作模型
	TCHAR					szPrepareMod[X_SHORT_NAME];		// 技能使用起手动作模型
	TCHAR					szNextDesc[X_HUGE_NAME];		// 下一级技能描述
	TCHAR					szSound[X_LONG_NAME];			// 技能攻击音效
	TCHAR					szPreSfx[X_SHORT_NAME];			// 起手特效
	std::vector<tstring>	vecTagPreSfx;					// 起手特效挂载点或骨骼名称
	TCHAR					szAtkSfx[X_SHORT_NAME];			// 攻击特效
	std::vector<tstring>	vecTagAtkSfx;					// 攻击特效挂载点或骨骼名称
	TCHAR					szLchSfx[X_SHORT_NAME];			// 发射特效
	TCHAR					szTagLchSfx[X_SHORT_NAME];		// 发射特效挂载点或骨骼名称
	TCHAR					szBatkSfx[X_SHORT_NAME];		// 被攻击特效基本编号
	INT						nNumBatkSfx;					// 被攻击特效数量
	BOOL					bActionBlend;					// 技能动作是否上下身分开
	BOOL					bAutoNormalAttack;				// 技能是否自动接普通攻击
	
	DWORD					dwBatkAction[5];				// 被攻击时的动作
};

struct tagSkillBoxProto
{
	// 技能盒子中最多可以有10个技能
	enum {E_SBP_Size=10};
	DWORD	dwID;
	BOOL	bCanBeLearned;
	DWORD	dwSkillIDs[E_SBP_Size];
	tagSkillBoxProto()
		:dwID(0){memset(dwSkillIDs,0,sizeof(dwSkillIDs));}
	tagSkillBoxProto(const tagSkillBoxProto &t)
	{
		_copy(*this,t);
	}
	tagSkillBoxProto & operator = (const tagSkillBoxProto &t)
	{
		_copy(*this,t);
		return *this;
	}
	BOOL Find(DWORD dwSkillID) const
	{
		for(INT i = 0 ; i < E_SBP_Size; ++i)
		{
			if( dwSkillIDs[i] == dwSkillID )
			{
				return TRUE;
			}
		}
		return FALSE;
	}
private:
	static void _copy(tagSkillBoxProto &des,const tagSkillBoxProto &t)
	{
		des.dwID = t.dwID;
		des.bCanBeLearned = t.bCanBeLearned;
		memcpy(des.dwSkillIDs,t.dwSkillIDs,sizeof(t.dwSkillIDs));
	}
};

struct tagInitialSkill 
{
	DWORD		dwID;
	DWORD		dwSkillID;
};

//----------------------------------------------------------------------------------
// 普通攻击
//----------------------------------------------------------------------------------
const INT X_NORMAL_ATTACK_SKILL_NUM = 5;

const DWORD NORMAL_ATTACK_SKILL_ID[X_NORMAL_ATTACK_SKILL_NUM] = // 普通攻击技能ID（大ID）
{
	10001,					// 徒手普通攻击
	10002,					// 神兵通攻击
	10003,					// 月弓普通攻击
	10004,					// 法器普通攻击
	10005,					// 瑶琴普通攻击
};

const INT X_BT_NORMAL_ATTACK_SKILL_NUM = 4;
const DWORD BT_NORMAL_ATTACK_SKILL_ID[X_BT_NORMAL_ATTACK_SKILL_NUM] = 
{
	10006,					//长刃
	10007,					//弓箭
	10008,					//宝器
	10009,					//魔琴
};

//-----------------------------------------------------------------------------------
// 某个技能的TypeID是不是普通攻击技能
//-----------------------------------------------------------------------------------
inline BOOL IsNormalAttackSkill(DWORD dwSkillID)
{
	for(INT n = 0; n < X_NORMAL_ATTACK_SKILL_NUM; n++)
	{
		if( NORMAL_ATTACK_SKILL_ID[n] == dwSkillID )
			return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------------
// 采集类技能
//-----------------------------------------------------------------------------------
const DWORD GATHER_SKILL_MINING		= 90000;	// 开采，采集矿石及草药
const DWORD GATHER_SKILL_HARVEST	= 90001;	// 收获，采集食材及布料

//-----------------------------------------------------------------------------------
// 调查地物技能tbc:inves
//-----------------------------------------------------------------------------------
const DWORD INVES_SKILL				= 90010;

//-----------------------------------------------------------------------------------
// 妖精捕捉技能
//-----------------------------------------------------------------------------------
const DWORD FairyTrain_SKILL		= 90003;

//-----------------------------------------------------------------------------------
// 骑乘技能
//-----------------------------------------------------------------------------------
const DWORD MOUNT_SKILL	= 90001;

//-----------------------------------------------------------------------------------
// 开妖精宝箱技能ID
//-----------------------------------------------------------------------------------
const DWORD FAIRY_CHEST_SKILL	= 90004;

//-----------------------------------------------------------------------------------
// 召唤圣灵技能
//-----------------------------------------------------------------------------------
const DWORD Call_Holy_Skill = 5944101;

// 变身技能个数
#define TRANSFORM_SKILL_NUM	6

// 技能地图限制
struct tagSkillableMaps 
{
	DWORD dwID;
	DWORD dwMapID[MAX_SKILL_MAP];
};


#pragma pack(pop)
