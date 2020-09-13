//-----------------------------------------------------------------------------
// brief: 神系统属性
//-----------------------------------------------------------------------------
#pragma once

#include "RoleDefine.h"

#pragma pack(push, 1)

const INT	MAX_FieldNum = 8;					// 神职领域最大数量
const INT	MAX_GodHead  = 20;					// 神格最大数量
const INT	MAX_Belief   = 99999;				// 信仰最大值 
const INT	MAX_CampaignInfoNumPerType = 10;	// 竞选结果显示人数（单个神）
const INT   MAX_GOD_NUM = 7;					// 每一系高级神个数
const INT	MAX_GOD_Relation_NUM = 26;			// 神之系谱上的神职数量

enum ECompetitiveClergy
{
	ECC_WarWinner	= 15,	//战神
	ECC_SunWinner	= 16,	//太阳神
	ECC_ManaWinner	= 17,	//法师之神
	ECC_PeaceWinner	= 18,	//和平之神

	ECC_WarMen		= 19,	//暴君
	ECC_WarWomen	= 20,	//死亡女神
	ECC_SunMen		= 21,	//暗夜魔神
	ECC_SunWomen	= 22, 	//暗影之女
	ECC_ManaMen		= 23, 	//元素法尊
	ECC_ManaWomen	= 24,	//魔法女王
	ECC_PeaceMen    = 25,	//痛苦之王
	ECC_PeaceWomen  = 26,	//厄运
};

enum ECandidateFlag
{
	ECF_Null		= 0,
	ECF_4BaseClergy	= 1,
	ECF_4BossClergy	= 2,
};

enum EPromotionType
{
	EPTT_NULL     = 0, //无
	EPTT_Directly = 1, //直接晋升
	EPTT_Campaign = 2, //排名晋升
};

enum ECampaignGodType
{
	ECGT_Null =-1,

	ECGT_War =0,	// 战神
	ECGT_Sun,		// 太阳神
	ECGT_Mana,		// 法师之神
	ECGT_Peace,		// 和平之神

	ECGT_Num,
};

enum EGodType
{
	EGT_Null = 0,

	EGT_War,
	EGT_WarBoss,
	EGT_Sun,
	EGT_SunBoss,
	EGT_Mana,
	EGT_ManaBoss,
	EGT_Peace,
	EGT_PeaceBoss,
};

#define ECGT_INVALID(n) (n<=ECGT_Null || n>=ECGT_Num)

// 神系内的职位
enum EClergyType
{
	ECT_Null = -1,

	ECT_WomenChamp	= 0,
	ECT_MenChamp	= 1,
	ECT_Winner		= 2,
	ECT_Loser		= 3,
};

// 神职方向
// enum EClergyType
// {
// 	ECT_Start	= -1,
// 	ECT_Mars	= 0,	// 战神
// 	ECT_Apollo	= 1,	// 太阳神
// 	ECT_Rabbi	= 2,	// 法师之神
// 	ECT_Peace	= 3,	// 和平之神
// 	ECT_End		= 4,
// };

struct tagClergyProto
{
	DWORD			dwID;						//神职ID
	INT8			n8ClergyLevel;				//神职数值等级
	INT				nNum;						//名额限制
	EGodType		eType;						//神职类型
	EPromotionType	ePromotionType;				//晋升模式
	DWORD			dwClassLimit;				//职业限制
	INT				nGodheadLimit;				//神格限制
	INT				nLevelLimit;				//等级限制
	ESex			eSexLimit;					//性别限制
	ERoleAttribute	eAttType;					//属性类型
	DWORD			dwAttValue;					//属性数值
	INT8			n8FieldNum;					//领域数量
	DWORD			dwFieldID[MAX_FieldNum];	//领域ID
};

struct tagClergyName : tagClergyProto
{
	DWORD			dwID;										//神职ID
	TCHAR			szName[X_LONG_NAME];						//神职名称
	TCHAR			szIcon[X_LONG_NAME];						//神职图标
	TCHAR			szEffect[X_LONG_NAME];						//特效
	TCHAR			szText[X_LONG_NAME];						//文本描述
	TCHAR			szClergyText[X_LONG_NAME];					//神职文本(上位神等)

};

struct tagCondensedProto
{
	DWORD			dwID;						//ID
	INT8			n8LowerLevel;				//最低等级
	DWORD			dwAgglomerate_val;			//凝聚度消耗
	INT				nCanActNum;					// 能够激活的领域数量
};

struct tagPersonInfo
{
	TCHAR			szName[X_SHORT_NAME];	// 人名
	DWORD			dwStrength;				// 实力值
	EClergyType		eClergy;				// 职位

	tagPersonInfo& operator =(const tagPersonInfo&rv)
	{
		if(this==&rv)
			return *this;

		memcpy(this->szName, rv.szName, X_SHORT_NAME);
		this->dwStrength = rv.dwStrength;
		this->eClergy = rv.eClergy;
		return *this;
	}
};

struct tagGodCampaignInfo
{
	tagPersonInfo	sPlayerInfo[MAX_CampaignInfoNumPerType];

	tagGodCampaignInfo& operator =(const tagGodCampaignInfo& rv)
	{
		if(this==&rv)
			return *this;

		for (int i =0; i < MAX_CampaignInfoNumPerType ;i++)
		{
			this->sPlayerInfo[i] = rv.sPlayerInfo[i];
		}

		return *this;
	}
};

struct tagCampaignResultInfo
{
	tagGodCampaignInfo sCampaignResult[ECGT_Num];

	tagCampaignResultInfo& operator = (const tagCampaignResultInfo& rv)
	{
		if(this==&rv)
			return *this;

		for (int i=0; i <ECGT_Num ;i++)
		{
			this->sCampaignResult[i] = rv.sCampaignResult[i];
		}

		return *this;
	}
};

#define GetClassEnumBinary(eClassType, val) do{\
		val = 1<<(eClassType-1);	\
		break;	\
	}while(0)


//神职领域相关
//能力类型
enum EGodAreaType
{
	EGAT_NULL     = 0,	//无
	EGAT_GOD,			//神职
	EGAT_OPTIONAL, 		//自选
	EGAT_RARE,	  		//稀有

	ECGT_NUM,
};

//表现形式
enum EGodAreaNatureType
{
	EGANT_NULL     = 0,	//无
	EGANT_SKILL,		//技能
	EGANT_BUFF, 		//状态
	EGANT_ALL, 			//技能+状态

	EGANT_NUM,
};

//主被动
enum EGodAreaUseType
{
	EGANUT_NULL     = 0,	//无
	EGANUT_INITIATIVE,		//主动
	EGANUT_PASSIVE, 		//被动

	EGANUT_NUM,
};

//特殊功能
enum EGodAreaFuncType
{
	EGAFT_NULL     = -1,	//无
};

struct tagGodAreaProto
{
	DWORD				dwID;						//领域ID
	EGodAreaType		eType;						//能力类型
	EGodAreaNatureType	eNatureType;				//表现形式
	EGodAreaUseType		eUseType;					//主被动
	INT					nLevel;						//能力等级
	INT					nLevelLimit;				//激活等级要求
	INT					nGodheadLimit;				//激活神格要求
	INT					nMaxNum;					//存在数量
	DWORD				dwPreID;					//学习时领域前提
	INT					nFaithValue;				//学习时消耗信仰
	DWORD				dwSkillID;					//关联技能
	DWORD				dwBuffID;					//关联状态
	EGodAreaFuncType	eFunctionType;				//特殊功能	
	INT					nFunctionValue;				//特殊功能参数
	INT					nTime;						//稀有领域时间
	BOOL				bContest;					//是否可以争夺
};

struct tagGodAreaName : tagGodAreaProto
{
	DWORD				dwID;						//领域ID
	TCHAR				szName[X_LONG_NAME];		//名称
	TCHAR				szIcon[X_LONG_NAME];		//图标
	TCHAR				szDesc[X_LONG_NAME];		//说明
	TCHAR				szDescType[X_LONG_NAME];	//领域类型描述
	TCHAR				szGetDesc[X_LONG_NAME];	//获得途径
};

//--------------------------------------------------------------------------------
// 领域传送客户端的消息结构
struct tagAreaMsgInfo
{
	DWORD	dwAreaID;								//领域ID
	BOOL	bActivation;							//是否激活
	DWORD	dwTime;									//稀有领域剩余时间
	DWORD	dwCD;									//领域CD

 	tagAreaMsgInfo& operator =(const tagAreaMsgInfo&rv)
 	{
 		if(this==&rv)
 			return *this;
 
 		this->dwAreaID = rv.dwAreaID;
 		this->bActivation = rv.bActivation;
		this->dwTime = rv.dwTime;
 		return *this;
 	}
};

struct tagAreaEffectProto
{
	DWORD dwID;
	DWORD dwBuffID;
};

#pragma pack(pop)