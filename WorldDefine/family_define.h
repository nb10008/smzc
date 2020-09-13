//////////////////////////////////////////////////////////////////////////
// 家族系统结构体定义
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "RoleDefine.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
// common
//////////////////////////////////////////////////////////////////////////
#define MAX_ItemOrMonsterID				80			// 任务所需的物品或怪物个数
#define MAX_FQ_RewardNum				5			// 奖励物品个数
#define MAX_FAMILY_MEMBER				12			// 家族额定的成员数量
#define MAX_FAMILY_NAME_LEN				7			// 家族名称最大长度
#define MIN_CREATE_FAMILY_TEAM_MEMBER	2			// 创建家族组队最少人数
#define MAX_FAMILY_QUESTS_NUM			4			// 家族任务最大个数
#define CAN_GET_REWARD_PROCESS			3000		// 昨日完成度达到30%的家族可领取奖励
#define CAN_GET_ITEM_REWARD_PROCESS		8000		// 昨日完成度达到80%的家族可领取物品
#define SAVE_FAMILY_QUEST_INTERNAL_MIN	5			// 对于一个家族，每隔5分钟保存一次他的任务计数
#define JOIN_FAMILY_REQ_TIMEOUT			60		// 加入家族申请的最长时间
#define MAX_FAMILY_REWARD_ITEM_NUM		5			// 奖励物品数量
#define FAMILY_ACTIVE_INCREASE_ONCE		40			// 家族成员登陆游戏增加家族活跃度
#define MAX_FAMILY_ACTIVE						(FAMILY_ACTIVE_INCREASE_ONCE*MAX_FAMILY_MEMBER)
#define MAX_FAMILY_SPIRITE_ADDED_EXP	1000 // 家族妖精的成长度增加的最大值
// 家族属性
struct tagFamilyAtt 
{
	DWORD 	dwFamilyID;				// 家族ID（家族名称的CRC）
	DWORD 	dwLeaderID;				// 家族族长ID
	DWORD 	dwFounderID;			// 创始人ID
	INT		nActive;				// 家族活跃度
	DWORD 	dwCreateTime;			// 家族创建时间

	tagFamilyAtt()
	{
		ZeroMemory(this, sizeof(*this));
		dwFamilyID = GT_INVALID;
		dwLeaderID = GT_INVALID;
		dwFounderID = GT_INVALID;
	}
	VOID Init(DWORD 	dwFamilyID,			// 家族ID（家族名称的CRC）
					DWORD 	dwLeaderID,		// 家族族长ID
					DWORD 	dwFounderID,		// 创始人ID
					INT			nActive,				// 家族活跃度
					DWORD 	dwCreateTime)	// 家族创建时间
					{
						this->dwFamilyID = dwFamilyID;
						this->dwLeaderID = dwLeaderID;
						this->dwFounderID = dwFounderID;
						this->nActive = nActive;
						this->dwCreateTime = dwCreateTime;
					}
	tagFamilyAtt& operator=(tagFamilyAtt& other)
	{
		dwFamilyID = other.dwFamilyID;
		dwLeaderID = other.dwLeaderID;
		dwFounderID = other.dwFounderID;
		nActive = other.nActive;
		dwCreateTime = other.dwCreateTime;
		return *this;
	}
	VOID SetFamilyActive(INT nFamilyActive)
	{
		if (nFamilyActive > MAX_FAMILY_ACTIVE)
			nActive = MAX_FAMILY_ACTIVE;
		else if (nFamilyActive < 0)
			nActive = 0;
		else
			nActive = nFamilyActive;
	}
};

// 家族成员
struct tagFamilyMember
{
	DWORD	dwRoleID;
	DWORD	dwFamilyID;		// 所属家族ID（家族名称的CRC）
	DWORD	dwJoinTime;		// 加入时间
	BOOL	bLeader;		// 是否是族长
	BOOL	bRewarded;	// 是否领取了上次家族任务的奖励
	// 权限？？？

	tagFamilyMember()   { ZeroMemory(this, sizeof(*this)); }
	tagFamilyMember(DWORD dwRoleID, DWORD dwFamilyID, DWORD dwJoinTime, BOOL bLeader, BOOL bRewarded)
	{
		this->dwRoleID = dwRoleID;
		this->dwFamilyID = dwFamilyID;
		this->dwJoinTime = dwJoinTime;
		this->bLeader = bLeader;
		this->bRewarded = bRewarded;
	}
	tagFamilyMember& operator=(tagFamilyMember& other)
	{
		this->dwRoleID = other.dwRoleID;
		this->dwFamilyID = other.dwFamilyID;
		this->dwJoinTime = other.dwJoinTime;
		this->bLeader = other.bLeader;
		this->bRewarded = other.bRewarded;
		return *this;
	}
};

// 玩家家族信息
struct tagRoleFamilyInfoBase
{
	DWORD	dwFamilyID;		// 所属家族ID（家族名称的CRC）
	BOOL	bLeader;				// 是否是族长
	tagRoleFamilyInfoBase()
		:dwFamilyID(GT_INVALID), bLeader(FALSE)
	{}
};

// 玩家家族信息
struct tagRoleFamilyInfo: public tagRoleFamilyInfoBase
{
	DWORD				dwRoleID;				// 玩家ID
	INT						nLevel;						// 等级
	BYTE					bySex;						// 0:girl，1:boy(便于以后扩展)
	EClassType          eClassType;             // 专精职业
	INT						nHpMax;				//最大血量
	INT						nHp;				//当前血量
	INT						nMpMax;
	INT						nMp;
	DWORD				dwMapID;				// 所在场景ID
	DWORD				dwLoverID;				// 配偶ID
	tagRoleFamilyInfo()
		:tagRoleFamilyInfoBase(), dwRoleID(GT_INVALID)
		,nLevel(0),bySex(GT_INVALID),eClassType(EV_Null)
		,nHp(0),nHpMax(0),nMp(0),nMpMax(0),dwMapID(GT_INVALID)
		,dwLoverID(GT_INVALID)
	{
	}
};

// 加入家族申请
struct tagJoinFamilyReq
{
	DWORD dwJoinerID;
	DWORD dwMemberID;
	DWORD dwAddReqTime;
	BOOL bInviteJoin;
	tagJoinFamilyReq()
		:dwJoinerID(GT_INVALID), dwMemberID(GT_INVALID), dwAddReqTime(0), bInviteJoin(TRUE)
	{}
	tagJoinFamilyReq& operator=(tagJoinFamilyReq& other)
	{
		this->dwJoinerID = other.dwJoinerID;
		this->dwMemberID = other.dwMemberID;
		this->dwAddReqTime = other.dwAddReqTime;
		this->bInviteJoin = other.bInviteJoin;
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////////
// 家族妖精相关
//////////////////////////////////////////////////////////////////////////
enum EFamilySpriteError
{
	EFSE_LACK_ITEM = 1,				//角色身上是否拥有足够数量的对应物品
	EFSE_NOT_FAMILY_MEMBER = 2,		//不是家族成员
	EFSE_HAS_COMPLETE = 3,			//该修炼需求已被家族成员完成
	EFSE_BAG_FULL = 4,				//您的背包已满，将无法获得物品奖励，请整理背包后再提交
	EFSE_SPRITE_LEVEL_FULL = 5,		//守护妖精的该能力已经达到当前上限，您仍可进行修炼，单只会获得经验和物品奖励，您是否确认？——确认/取消
	EFSE_FABAO_WHITE = 6,			//所登记的妖精品质必须高于白色品质
	EFSE_FABAO_LEVEL = 7,			//您无法将高于您自身等级的妖精进行登记
	EFSE_FABAO_STATU = 8,			//妖精正处于孕育、生育或是休憩阶段时，是无法进行登记的
	EFSE_FABAO_HAVE = 9,			//已经有登记的妖精了
	EFSE_FABAO_NOT_HAVE = 10,		//还没有登记妖精
	EFSE_FABAO_BAG = 11,			//背包已满
	EFSE_FABAO_LEVEL_DOWN = 12,		//角色等级太低
	EFSE_USE_ITEM_MAX = 13,			//使用物品数量查过上限
};

//	 守护妖精能力
enum EFamilySpriteAtt
{
	EFSA_Begin = 0,
	EFSA_HP = 0,
	EFSA_EXAttack,
	EFSA_InAttack,
	EFSA_EXDefense,
	EFSA_InDefense,
	EFSA_EXAttackDeeper,
	EFSA_InAttackDeeper,
	EFSA_EXAttackResistance,
	EFSA_InAttackResistance,
	EFSA_Toughness,
	EFSA_CritDes,
	EFSA_ControleffectDeepen,	
	EFSA_ControleffectResistance,
	EFSA_SlowingeffectDeepen,
	EFSA_SlowingeffectResistance,
	EFSA_FixedeffectDeepen,		
	EFSA_FixedeffectResistance,
	EFSA_AgingeffectDeepen,		
	EFSA_AgingeffectResistance,

	EFSA_Num,
};


struct tagFamilySpriteProto
{
	INT							nID;									// 等级作为ID
	DWORD						dwExp;									// 成长度
	DWORD						dwAbilityUpperLimit[EFSA_Num]; 
	DWORD						dwPickTypeNum[2];						// 抽取类型数量
	DWORD						dwShedingzhi;

	//INT						nLevel;									// 等级
	//DWORD						dwTrainNum;								// 修炼需求发布个数
	
	//DWORD						dwHP;									// 体力上限
	//DWORD						dwExAttack;								// 物理攻击能力上限
	//DWORD						dwInAttack;								// 法术攻击能力上限
	//DWORD						dwExDefense;							// 物理防御能力上限
	//DWORD						dwInDefense;							// 法术防御能力上限
	//DWORD						dwExAttackAdd;							// 物理伤害加深上限
	//DWORD						dwInAttackAdd;							// 法术伤害加深上限
	//DWORD						dwExDefenseDes;							// 物理减免加深上限
	//DWORD						dwInDefenseDes;							// 法术减免加深上限
	//DWORD						dwToughness;							// 韧性上限
	//DWORD						dwCritDes;								// 暴击抵消上限
	//DWORD						dwControleffectDeepen;					// 控制效果加深上限
	//DWORD						dwControleffectResistance;				// 控制抵抗加深上限
	//DWORD						dwSlowingeffectDeepen;					// 减速效果加深上限
	//DWORD						dwSlowingeffectResistance;				// 减速抵抗加深上限
	//DWORD						dwFixedeffectDeepen;					// 固定效果加深上限
	//DWORD						dwFixedeffectResistance;				// 固定抵抗加深上限
	//DWORD						dwAgingeffectDeepen;					// 衰弱效果加深上限
	//DWORD						dwAgingeffectResistance;				// 衰弱抵抗加深上限
	
};



struct tagSpriteTrainProto
{
	INT	nID;										
	INT	nPageType;								// 分页类型
	INT	nLevelLimit;								// 等级限制
	DWORD	dwAbilityType;			// 能力类型
	DWORD	dwPowerUpLimit;			// 能力提升上限
	DWORD	dwPowerLowLimit;			// 能力提升下线
	DWORD	dwItemID;						// 需求物品
	DWORD	dwItemNum;					// 需求物品数量
	DWORD	dwItemQuality;				// 需求物品品级(是装备的时候才读)
	DWORD	dwShowUpLimit;				// 出现等级上线
	DWORD	dwShowLowLimit;			// 出现等级下线
	DWORD	dwPickType;					// 抽取类型
	DWORD	dwSpriteExpReword;		// 妖精成长度奖励
	DWORD	dwExpReword;					// 经验奖励
	DWORD	dwItemReword;				// 物品奖励
	DWORD	dwItemRewordNum;		// 物品奖励数量
	DWORD	dwItemRewordQuality;	// 物品奖励品级(是装备的时候才读)
};

struct tagSpriteTrainData
{
	INT		nLevel;
	std::vector<DWORD>	m_vecTrainID1;
	std::vector<DWORD>	m_vecTrainID2;
};

struct tagSpriteTrainClientProto : tagSpriteTrainProto
{
	DWORD	dwRealResult;							// 实际结果
	TCHAR		szName[X_LONG_NAME];		// 完成人
	tagSpriteTrainClientProto():dwRealResult(GT_INVALID)
	{
		ZeroMemory(szName,X_LONG_NAME*sizeof(TCHAR));
	}
};

enum EFamilySpritePickType
{
	EFSPT_1 = 1,
	EFSPT_2 = 2,
};


//家族妖精存储
struct tagFamilySpriteSave
{
	DWORD		dwFamilyID;								// 家族ID
	UINT16		un2Level;								// 等级
	INT64		n8Exp;									// 成长度
	TCHAR		tszName[X_SHORT_NAME];					// 名字
	INT			nAbility[EFSA_Num];						// 守护能力

	tagFamilySpriteSave()
	{
		Clear();
	}

	~tagFamilySpriteSave()
	{
		Clear();
	}

	tagFamilySpriteSave& operator=(tagFamilySpriteSave& other)
	{
		if( this == &other)
			return *this;

		this->dwFamilyID = other.dwFamilyID;
		this->un2Level = other.un2Level;
		this->n8Exp = other.n8Exp;
		for(int i = 0; i < EFSA_Num; ++i)
		{
			this->nAbility[i] = other.nAbility[i];
		}
		_stprintf(this->tszName,other.tszName);

		return *this;
	}

	VOID Clear()
	{
		for(int i = 0; i < EFSA_Num; ++i)				//守护能力
		{
			nAbility[i] = -1;
		}
		memset( tszName, 0, sizeof(tszName) );			//守护妖精名称	
		un2Level = 0;									//守护妖精等级
		n8Exp = 0;										//守护妖精成长度	
		dwFamilyID = 0;
	}

};

//家族妖精修炼需求存储
struct tagFamilySpriteRequirementSave
{
	DWORD		dwRequirementID;						//需求ID
	BOOL		bComplete;								//完成状态
	TCHAR		tszRoleName[X_SHORT_NAME];				//完成人
	INT			nRet;									//完成实际结果

	tagFamilySpriteRequirementSave()
	{
		Clear();
	}

	~tagFamilySpriteRequirementSave()
	{
		Clear();
	}

	tagFamilySpriteRequirementSave& operator=(const tagFamilySpriteRequirementSave& other)
	{
		if( this == &other)
			return *this;

		this->dwRequirementID = other.dwRequirementID;
		this->bComplete = other.bComplete;
		this->nRet = other.nRet;
		_tcscpy_s( this->tszRoleName, X_SHORT_NAME, other.tszRoleName );

		return *this;
	}

	VOID Clear()
	{
		dwRequirementID = 0;
		bComplete = FALSE;
		nRet = 0;
		memset( tszRoleName, 0, sizeof(tszRoleName) );
	}
};

//玩家登记妖精信息
struct tagRoleSpriteDataSave
{
	TCHAR	tszRoleName[X_SHORT_NAME];				//登记人
	FLOAT	fShili;									//实力值
	FLOAT	fGongming;								//共鸣值
	UINT16	un2Level;								//玩家登记妖精等级
	DWORD	dwSpriteType;							//玩家登记妖精TypeID
	DWORD	dwRoleID;								//角色ID
	INT	bQuality;

	tagRoleSpriteDataSave()
	{
		Clear();
	}

	~tagRoleSpriteDataSave()
	{
		Clear();
	}

	tagRoleSpriteDataSave& operator=(tagRoleSpriteDataSave& other)
	{
		if( this == &other)
			return *this;

		this->fShili = other.fShili;
		this->fGongming = other.fGongming;
		this->un2Level = other.un2Level;
		this->dwSpriteType = other.dwSpriteType;
		this->dwRoleID = other.dwRoleID;
		this->bQuality = other.bQuality;
		_tcscpy_s( this->tszRoleName, X_SHORT_NAME, other.tszRoleName );

		return *this;
	}

	VOID Clear()
	{
		fShili = 0;
		fGongming = 0;
		un2Level = 0;
		dwSpriteType = 0;
		dwRoleID = 0;
		bQuality = 0;
		memset( tszRoleName, 0, sizeof(tszRoleName) );
	}
};


//////////////////////////////////////////////////////////////////////////
// 家族任务相关
//////////////////////////////////////////////////////////////////////////
enum EFamilyQuestType
{
	EQT_Null = 0,
	EQT_Collect = 1,
	EQT_Kill	= 2
};

//家族任务结构体
struct tagFamilyQuestProto
{
	DWORD					dwID;			// 任务ID	
	EFamilyQuestType		eType;
	DWORD					dwConditionID;	// 任务条件ID
	int						nLowLevel;
	int						nHighLevel;
	INT						nQuestNum;		
};

struct tagFamilyQuestName : tagFamilyQuestProto
{
	TCHAR szDesc[X_LONG_NAME];
};

enum EFamilyQuestFinishType
{
	EQFT_NULL = 0,
	EQFT_Item = 1,		//物品
	EQFT_Monster = 2,	//怪物
};

enum EJudgeType
{	
	EJT_Null = 0,
	EJT_ID = 1,				//判断ID
	EJT_Condition = 2,	//判断条件
};

//集合表
struct tagFamilyQuestAttProto
{
	DWORD dwID;
	EFamilyQuestFinishType eFinishType;
	EJudgeType eJudgeType;
	int nLowLevel;
	int nHighLevel;
	DWORD dwMapcrc;
	INT nMonsterRank;
	DWORD dwItemOrMonsterID[MAX_ItemOrMonsterID];
};

//家族任务奖励表
struct tagFamilyQuestRewardProto
{
	DWORD dwID;		//此值表示角色等级，根据这个值取奖励表
	DWORD dwExp;
	DWORD dwMoney;
	DWORD dwItemID[MAX_FQ_RewardNum];
	DWORD dwItemNum[MAX_FQ_RewardNum];
	DWORD dwItemIDEx[MAX_FQ_RewardNum];
	DWORD dwItemNumEx[MAX_FQ_RewardNum];
};

//  记录家族任务状态的结构
struct tagFamilyQuest
{
	DWORD	dwQuestID;			// 任务ID
	INT		nTargetCount;		// 计数
	//INT		nProcess;			// 完成度
};

//家族操作类型枚举
enum EFamilyEventType
{
	EFET_NoEvent		= 0,

	EFET_ChangeLeader	= 1,	//移交族长
	EFET_MemberDismis	= 2,	//开出成员
	EFET_LeaveFamily	= 3,	//离开家族

	EFET_EventNum,
};

#pragma pack(pop)