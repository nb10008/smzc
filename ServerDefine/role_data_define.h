#pragma once

#include "../WorldDefine/base_define.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/talent_define.h"
#include "../WorldDefine/currency_define.h"
#include "../WorldDefine/master_apprentice.h"
#include "../ServerDefine/clandata_define.h"
#include "../ServerDefine/rankings_define.h"
#include "../WorldDefine/train_define.h"
#include "../WorldDefine/clergy_define.h"
#pragma pack(push, 1)

struct tagDWORDTime;

//-------------------------------------------------------------------------------------
// 常量
//-------------------------------------------------------------------------------------
const INT MIN_ROLE_SAVE2DB_INTERVAL			= 60 * TICK_PER_SECOND;
const INT ROLE_TIME_STAT_INTERVAL			= 30 * 60 * TICK_PER_SECOND;

// Jason 2009-12-7 神龙赐福最大级别
// 不做限定,从属性表去最大数，bug:去掉 static有局部属性，
extern INT ROLE_MAX_LESSLEVEL_OF_LOONG		 ;

//-------------------------------------------------------------------------------------
// 人物创建结构
//-------------------------------------------------------------------------------------
struct tagCreateRoleInfo
{
	tagAvatarAtt	Avatar;						// size = 16
	TCHAR			szRoleName[X_SHORT_NAME];

	tagDWORDTime	CreateTime;					// 创建时间
	DWORD			dwMapID;
	FLOAT			fX;
	FLOAT			fY;
	FLOAT			fZ;
	FLOAT			fFaceX;
	FLOAT			fFaceY;
	FLOAT			fFaceZ;
	BYTE			byLevel;
	BYTE			byReserved[3];

};

//---------------------------------------------------------------------------------------
// 实时保存数据库枚举
//---------------------------------------------------------------------------------------
enum ERealTimeSaveAtt
{
	ERTSA_Null			= -1,
	ERTSA_Exp			= 0,
	ERTSA_Level			= 1,
};

//---------------------------------------------------------------------------------------
// 离线玩家数据结构
//---------------------------------------------------------------------------------------
struct tagRoleInfo
{
	DWORD			dwRoleID;					// ID
	DWORD			dwRoleNameCrc;				// 名称CRC
	DWORD			dwAccountID;				// 账户ID
	TCHAR			szRoleName[X_SHORT_NAME];	// 名字
	DWORD			dwMapID;					// 所在地图
	tagDWORDTime	dwTimeLastLogout;			// 最后上线时间
	BYTE			byLevel;					// 等级
	BYTE			bySex;						// 性别
	bool			bOnline;					// 玩家是否在线
	BYTE			byClass;					// 职业
	DWORD			dwLoverID;					// 爱人
	// Jason 2010-3-2 v1.3.2 增加帮派id
	DWORD			dwGuildID;					// 玩家所属帮派
	INT				nMARelation;				// 师徒数量
	INT		nVIPLevel;								// vip等级，0是无等级，1银，2金，3白金
	DWORD dwFamilyID;					// 家族ID
};

//---------------------------------------------------------------------------------------
// 安全码
//---------------------------------------------------------------------------------------
struct tagSafeCode
{
	DWORD	dwSafeCodeCrc;		// 安全码Crc32，GT_INVALID表示没有设置过
	DWORD	dwTimeReset;		// 重置时间，GT_INVALID表示没有重置
};


// Jason 神龙赐福配置文件数据结构
struct tagConfigLessingOfLoong
{
	INT nID;			// 级别0-7，也即8次
	INT nExpireTime;	// 到期时间
	INT64 nExperience;	// 经验值
	DWORD	dwRewardItemTypeID;	// 奖励物品typeid
	INT	nQlty;
	INT	nItemCount;		// 奖励物品数量
	tagConfigLessingOfLoong()
		:nID(0),nExpireTime(0),nExperience(0),dwRewardItemTypeID(0),nItemCount(0),nQlty(0)
	{
	}
	tagConfigLessingOfLoong(const tagConfigLessingOfLoong & t)
		:nID(t.nID),nExpireTime(t.nExpireTime),nExperience(t.nExperience),dwRewardItemTypeID(t.dwRewardItemTypeID),nItemCount(t.nItemCount),
		nQlty(t.nQlty)
	{
	}
	tagConfigLessingOfLoong & operator = (const tagConfigLessingOfLoong & t)
	{
		memcpy(this,&t,sizeof(t));
	}
};


//---------------------------------------------------------------------------------------
// accountrel表内相关属性
//---------------------------------------------------------------------------------------
struct tagAccountCommon
{
	tagSafeCode				sSafeCode;
	DWORD					dwBagPsdCrc;
	INT						nBaiBaoYuanBao;
	INT64					n64WareSilver;
	INT16					n16WareSize;
	tagDWPlayerPersonalSet	stPersonalSet;
	DWORD					dwLastUseRoleID;
	EAccountSpec		eSpec; // 特殊权限，用于特定活动

	VOID Init()
	{
		sSafeCode.dwSafeCodeCrc = GT_INVALID;
		sSafeCode.dwTimeReset	= GT_INVALID;

		dwBagPsdCrc				= GT_INVALID;
		n64WareSilver			= 0;
		nBaiBaoYuanBao			= 0;
		n16WareSize				= 0;

		stPersonalSet.bAcceptFactionInvite	=	true;
		stPersonalSet.bAcceptTeamInvite		=	true;
		stPersonalSet.bAcceptTradeRequest	=	true;
		stPersonalSet.bAllowFriendAdd		=	true;
		stPersonalSet.eInfoOpenType			=	EPIS_Public;

		dwLastUseRoleID			=	GT_INVALID;
	}
};

//--------------------------------------------------------------------------------------------
//混服账号新信息
//--------------------------------------------------------------------------------------------
struct tagNewAccountCommon
{
	DWORD  _dwChannelID;                    // 混服的运营商ID
	CHAR   szNewAccountName[X_SHORT_NAME];	// 名称
	tagNewAccountCommon()
	{
		memset(this, 0,sizeof(*this));
	}
};

//---------------------------------------------------------------------------------------
// 游戏服务器与数据库服务器存取的玩家数据结构
//---------------------------------------------------------------------------------------
struct tagRoleDataConst
{
	DWORD			dwRoleNameCrc;				// 名称CRC
	TCHAR			szRoleName[X_SHORT_NAME];	// 名称
	tagAvatarAtt	Avatar;						// 外观
	tagDWORDTime	CreateTime;					// 创建时间
};

struct tagRoleDataSave
{
	tagDWORDTime	LoginTime;					// 登入时间
	tagDWORDTime	LogoutTime;					// 登出时间
	INT				nOnlineTime;				// 总在线时常
	INT				nCurOnlineTime;				// 当次在线时常

	// Jason 2009-12-7 神龙赐福
	DWORD			timeLastLessing;			// 上次神龙赐福时间，注意，该值与总在线时间单位一致
	INT				nLessingLevel;				// 赐福级别，最高ROLE_MAX_LESSLEVEL_OF_LOONG级
	INT				nLessingLevelEx;			// Jason，2010-3-12 v1.5.0

	// Jason 2010-1-8 每日在线奖励相关字段
	DWORD			dwCurrentDayOnlineTime;		// 当日累计在线时长
	tagDWORDTime	timeOneDayFirstLogined;		// 当日首次登陆时间
	tagDWORDTime	dateLastLessingLoong;		// 最后一次神龙赐福的时间,神龙赐福有效时，没有对它判断
	tagDWORDTime	timeOnlineRewardPerDayRec;	// 当日领取在线奖励的时间
	// Jason 2010-1-15 离线奖励
	BOOL			bIsOpenOfflineReward;		// 是否开启离线奖励
	// Jason 2010-1-30 v1.3.2宠物栏阀
	UINT16			u16FollowPetPocketValve;  // 跟随宠物上限
    UINT16          u16RidingPetPocketValve;  // 坐骑上限
	// Jason 2010-3-29 v1.5.1 神迹积分
	INT				nGodMiraclePoints;

	BYTE			byTrainStatesTransfered;

	INT16			n16MiraclePracticeNum;		// 神迹技能修练次数
	tagDWORDTime	dwMiracleResetTime;			// 重置时间

	tagAvatarEquip	AvatarEquip;				// 装备外观
	tagDisplaySet	DisplaySet;					// 外观显示设置

	DWORD			dwMapID;					// 所在的地图的ID
	DWORD			dwRebornMapID;				// 复活地图ID
	FLOAT			fCoordinate[3];				// 位置（x,y,z）
	FLOAT			fFace[3];					// 朝向（Vectro3）

	EClassType		eClass;						// 职业
	EClassTypeEx	eClassEx;					// 扩展职业

	INT				nLevel;						// 玩家等级
	DWORD			dwLastUpgrade;				// 最近一次的升级时间
	INT				nLevelPm;					// 等级排名
	INT64			nCurExp;					// 玩家当前级别的升级经验

	INT				nHP;						// 体力
	INT				nMP;						// 真气
	INT				nRage;						// 怒气
	INT				nEndurance;					// 持久力
	INT				nVitality;					// 活力
	INT				nInjury;					// 内伤
	INT				nKnowledge;					// 阅历
	INT				nMorale;					// 士气
	INT				nMorality;					// 道德
	INT				nCulture;					// 武学修为
	INT				nHostility;					// 戾气
	INT				nCredit;					// 信用度
	INT				nIdentity;					// 身份
	INT				nSpirit;					// 精力
	INT				nWunXun;					// 武勋
	INT				nWunJi;						// 武技
	INT				nVIPPoint;					// 会员积分

	INT				nWeary;						// 疲劳值
	INT				nVipLevel;					// VIP等级
	tagDWORDTime dwVipStartTime;					// Vip道具使用时间，没有使用过或者已经过期为0'
	UINT16				n16FreeWorldTalkTimes;	// Vip角色已经使用的免费世界喊话的次数
	DWORD		dwVipMaxDays;	// Vip道具时限(单位为天)
	INT				nAttPoint;					// 玩家可用的属性点
	INT				nTalentPoint;				// 玩家可用的天资点

	// 实力相关
	INT				nStrength;					// 总实力值
	INT				nAdvanceStrength;			// 进阶实力值
	INT				nConsumptiveStrength;		// 可消耗实力值
	INT				nStrengthNum;				// 实力排名
	INT				nEquipValue;				// 装备实力值
	INT				nYaoJingValue;				// 妖精实力值
	INT				nEquipPm;					// 装备实力排名
	INT				nKillScore;					// 记录该玩家一日之内击杀非白名玩家获得的进阶实力值

	INT				nAttPointAdd[X_ERA_ATTA_NUM];// 玩家投放到各个一级属性中的值
	tagRoleTalent	talent[X_MAX_TALENT_PER_ROLE];// 玩家的天资

	BOOL			bSafeGuard;					// 是否被PK保护
	tagDWORDTime	CloseSafeGuardTime;			// 上次关闭PK保护的时间
	INT				nDeadUnSetSafeGuardCountDown;	//关闭玩家戾气为0时回城复活所加上的pk保护倒计tick
	DWORD           dwItemTransportMapID;           //当前记录传送符绑定地图ID
	FLOAT			fItemTransportX;				//当前记录传送符绑定的x坐标
	FLOAT			fItemTransportZ;				//当前记录传送符绑定的z坐标
	FLOAT			fItemTransportY;				//当前记录传送符绑定的y坐标

	INT16			n16BagSize;					// 背包大小
	INT				nBagGold;					// 背包中金子数
	INT				nBagSilver;					// 背包中银子数
	INT				nBagYuanBao;				// 背包中元宝数
	INT				nExVolume;					// 角色赠点数

	//INT16			n16WareSize;				// 账号仓库大小
	//INT				nWareGold;					// 账号仓库中金子数
	//INT				nWareSilver;				// 账号仓库中蝇子数
	//INT				nWareYuanBao;				// 账号仓库中元宝数

	DWORD			dwGuildID;					// 帮派id
	DWORD			dwFamilyID;					// 家族id
	
	INT				nTotalTax;					// 缴税总额

	tagRemoteOpenSet sRemoteOpenSet;			// 对远端玩家公开信息设置
	UINT16			u16ActiveTitleID;			// 当前称号

	DWORD			dwTimeGetMallFree;			// 上次从商城领取免费物品时间

	INT	 			nSkillNum;					// 技能个数			-- tagSkillSave
	INT				nBuffNum;					// 状态个数			-- tagBuffSave

	INT				nTitleNum;					// 称号个数			-- tagTitleSave

	INT				nItemNum;					// 物品装备个数		-- tagItem,tagEquip
	INT				nFriendNum;					// 好友个数			-- 读: tagFriendSave, 存: 实时,即单独发消息
	INT				nEnemyNum;					// 仇敌个数			-- 读: DWORD dwEnemyID, 存: 实时,即单独发消息
	INT				nQuestNum;					// 任务列表个数		-- tagQuestSave
	INT				nQuestDoneNum;				// 已完成的任务个数 -- 读：tagQuestDoneSave，存：实时
	INT             nDailyQuestDoneNum;         // 已完成的每日任务个数 读 tagQuestDoneSave，存：实时
	INT				nItemCDTimeNum;				// 物品冷却时间		-- tagCDTime
	INT				nFriendshipNum;				// 友好度			-- tagFriendshipSave
	INT				nBlackNum;					// 黑名单			-- DWORD
	BOOL			bClanDataChg;				// 氏族数据是否改变
	INT				nPetsNum;					// 宠物数目
	tagScriptData<ESD_Role>	Data;				// 玩家脚本数据
	INT				nTreasureSum;				// 玩家开启宝箱数

	DWORD			dwLastStallTime;			// 摊位最近更新时间
	INT32			nStallDailyExp;				// 摊位当日经验
	INT32			nStallCurExp;				// 摊位当前等级经验
	BYTE			byStallLevel;				// 摊位等级
	DWORD			dwLoverID;					// 爱人的roleid 没结婚就是4294967295
	BOOL			bHaveWedding;				// 是否举行过婚礼
	BOOL			bHasLeftMsg;				// 有留言
	
	BOOL			bSpeakOff;					// 禁言状态
	BOOL			bNeedPrisonRevive;			// 需要在牢狱复活
	DWORD			dwFixSpiritCD;				// 精力冷却时间
	DWORD			dwMasterID;					// 师傅ID
	tagApprenticeData		ApprenticeData;		// 徒弟ID
	INT				nJingWuPoint;				// 精武点数
	INT				nQinWuPoint;				// 勤武点数
	INT				nMARelation;				// 师徒数量
	INT				nOpenTrainCnt;				// 已开启试炼个数
	// Jason 2010-5-18 v2.1.2
	UINT8			nSoaringValue;				// 飞升标志
	INT				nSoaringSkillLearnTimes;	// 飞升技能学习次数限制
	//tagDWORDTime	timeLearnSoaringSkill;		// 飞升技能修炼时间
	// Jason 2010-5-27 v2.2.0
	INT				nToughness;					// 韧性属性
	// Jason 2010-6-13 v1.0.0
	INT				nConsolidateTimes;				// 强化次数

	// 任务板相关
	INT				nCompleteRefreshTime;
	INT				nIMRefreshTime;
	INT				nBoardQuestNum;
	INT				nYellowListCompleteRefreshTime;
	INT				nYellowListIMRefreshTime;
	// Jason 2010-7-14 1.0.0
	DWORD		dwRoleState;

	// 角色是否已经购买了一元计划活动中的装备
	BYTE 			byBuy50LvlItemFlag;
	BYTE 			byBuy60LvlItemFlag;
	BYTE 			byBuy70LvlItemFlag;
	BYTE 			byBuy80LvlItemFlag;

	BYTE			byCanRankFlag;// 角色是否能进入排行,0不能，1能

	DWORD		dwExpPilularUseTimes;

	INT32			nMonthOnlineTimeCnt;
	INT32			nEggInfoCnt;
	INT32			nWardrobeLayerCnt;
	// 处理被好友装扮
	DWORD		dwBePlayActLayerID;
	BYTE			byBePlayActLevel;

	// 领取公会每日奖励的时间，注意每天只能领取一次，所以没有记录次数
	tagDWORDTime	timeTakeGuildReward;	// 领取战场每日奖励的时间

	DWORD			dwPickupModeSetting;	// 拾取模式设置
	BYTE			byPlayerBack;			// 是否领取过玩家回归奖励
	UINT16			byPlayerBackDays;		// 累计回归天数
	BYTE			byUsingConstraintsPKSafeGuardLevel; // 是否使用默认的pk保护等级

	INT				nGodhead;		// 神格
	INT				nGodFaith;		// 信仰
	INT				nGodCondense;	// 凝聚度

	// 竞选神职相关
	DWORD			dwClergyMarsPoint;			// 战神系分值
	DWORD			dwClergyApolloPoint;		// 太阳神系分值
	DWORD			dwClergyRabbiPoint;			// 法系神系分值
	DWORD			dwClergyPeacePoint;			// 和平之神系分值
	ECampaignGodType		eClergyCandidate;	// 神职竞选报名状态
	ECandidateFlag	eSeniorOrJunior;			// 是否可以当选大神
	DWORD			dwClergy;					// 当前神职
	DWORD			dwPreClergy;				// 上次的神职
	INT					nAreaNum;					// 神职数量，该数量包括了自选领域和稀有领域的，不包含神职领域的
	
	// 商城限量购买物品购买记录
	INT					nBuyMallItemLogCount;	

	// key码领取奖励相关
	DWORD			dwKeyCodeRewarded;

	DWORD 			dwTrainDate;				// 家族妖精修炼物日期
	INT				nTrainNum;					// 家族妖精修炼当日修炼物品使用次数
	INT				nAwardPoint;				// 角色奖励点 
	INT				nAwardFlag;					// 礼包领取标志位 
	DWORD		dwBrotherTeatherID;		// 结拜兄弟中大哥的ID
	tagDWORDTime	timeBloodBrother;	// 结拜兄弟的结束时间
	INT				nPulseLearnRemainTime;	// 剩余的经脉可以打通的次数
	INT				nTigerTime;	// 老虎机玩的次数

	INT 			nGod;				//神魂
	INT 			nMonster;			//魔魂
	INT 			nGodPoint;			//神魂声望
	INT 			nMonsterPoint;		//魔魂声望
	INT				nFlower;			//鲜花
	INT				nEgg;				//鸡蛋
	INT				nHolyValue;			//圣灵值
	INT				nHidAdd;			//命中
	INT				nEEI_All;			//元素全伤

	//bool			bIsHolySoulActivated;      //元神是否被激活
	/*********************************************************************************************************
	*技能列表，状态列表，称号列表，称号条件列表，物品列表，装备列表，好友列表，仇敌列表，任务列表，已完成任务列表，
	*物品冷却时间表, 友好度表，角色名帖(自定义)，师徒列表，已开启试炼状态表，神系统领域表
	*必须按顺序拷贝
	*********************************************************************************************************/

	BYTE			byData[1];
};

struct tagRoleDataLoad
{
	DWORD				dwSize;					// 玩家信息大小
	tagRoleDataConst	RoleDataConst;
	tagRoleDataSave		RoleDataSave;
};

struct tagRecordDel // 删除的相关信息(两次保存间)
{
	INT	 	nSkillNum;		// 技能个数(dwSkillID):  所有原因产生的技能遗失,如果先遗忘后再学,此处也会记录
	INT		nBuffNum;		// 状态个数(dwBuffID):    从玩家身上消失过的所有状态
//	INT		nTalentNum;		// 天资列表(ETalentType): 洗点产生的
//	INT		nItemNum;		// 物品个数(n64Serial):   需摧毁的，即需从数据库中删除的 -- 实时
//	INT		nEquipNum;		// 装备个数(n64Serial):   需摧毁的，即需从数据库中删除的 -- 实时
	INT		nQuestNum;		// 任务个数(dwQuestID):   包括删除的和完成的

	/*********************************************************************************************************
	*技能列表，状态列表，物品列表，装备列表，任务列表
	*必须按顺序拷贝
	*********************************************************************************************************/
	BYTE	byData[1];		// 记录类型
};

//---------------------------------------------------------------------------------------------
// 人物技能
//---------------------------------------------------------------------------------------------
struct tagSkillSave
{
	DWORD	dwID;				// 技能ID
	INT		nSelfLevel;			// 自身等级
	INT		nLearnLevel;		// 投点等级
	INT		nCoolDown;		// 技能还需要的冷却时间
	INT     nProficiency;		// 技能熟练度
	DWORD	dwActiveTime;		//激活时间
};

//---------------------------------------------------------------------------------------------
// 人物buff的基本信息
//---------------------------------------------------------------------------------------------
struct tagBuffSave
{
	DWORD		dwBuffID;					// ID
	INT32		nPersistTick;				// 当前持续时间

	INT8		n8Level;					// 等级
	INT8		n8CurLapTimes;				// 当前叠加次数
	INT8		n8ModifierNum;				// 影响它的技能个数
	INT8		n8Dummy[1];

	DWORD		dwSrcUnitID;				// 源Unit的ID
	DWORD		dwSrcSkillID;				// 由哪个技能产生（大ID）
	INT64		n64Serial;					// 由哪个物品或装备产生
	DWORD		dwItemTypeID;				// 物品或装备的属性ID

	BYTE		byData[1];
};

//---------------------------------------------------------------------------------------
// 好友
//---------------------------------------------------------------------------------------
struct tagFriendSave
{
	DWORD		dwFriendID;		// 好友id
	INT32		nGroupID;		// 分组
};

struct tagFriendshipSave
{
	DWORD		dwFriendID;		// 好友id
	INT32		nFriVal;		// 友好度
};

//-----------------------------------------------------------------------------
// 保存到数据库的条件数据
//-----------------------------------------------------------------------------
struct tagTitleSave
{
	UINT16			u16TitleID;			// 称号ID
	DWORD			dwStateMark;		// 若为0，则表示已获取，否则表示para2
	DWORD			dwTimeRemain;		// 若为时限制称号，则表示剩余时间
};

//---------------------------------------------------------------------------------------
// 玩家各个级别所对应的基本属性及升到该级得到天资和升到下一级所需的经验增量 -- 从xml中读入
//---------------------------------------------------------------------------------------
struct tagLevelUpEffect
{
	INT64	nExpLevelUp;		// 升到下一级所需的经验增量
	INT16	n16RoleAtt[6];		// 筋骨、劲力、元气、内力、技力、身法
	INT16	n16RoleAttAvail;	// 得到的可投属性点
	INT16	n16TalentAvail;		// 得到的可投天资点
    INT16   n16TalentAvail_limit;// 得到当前级别天资上限
	INT16	n16HP;				// 当前级别血量
	INT16	n16MP;				// 当前级别真气量
	INT16	n16Vitality;		// 当前级别活力
	INT16	n16Durance;		// 耐力

	tagLevelUpEffect() { ZeroMemory(this, sizeof(*this)); nExpLevelUp = GT_INVALID; }
};

//---------------------------------------------------------------------------------------
// 玩家各个武勋等级所对应的武勋和武技 -- 从xml中读入
//---------------------------------------------------------------------------------------
struct tagWuXunProto
{
	INT16     n16WuXunLevel ;    //武勋等级
	UINT      unWuXunVal;        //当前武勋等级的武勋值
	UINT      unMaxWuJiVal;      //武技值   
	tagWuXunProto() { ZeroMemory(this,sizeof(*this));}
};

//---------------------------------------------------------------------------------------
// 玩家购买各个级别天资的相应花费 -- 从xml中读入
//---------------------------------------------------------------------------------------
struct tagTalentAvailCost
{
	
    INT     nTalentSequenceNum ; // 购买的天资序列数
	DWORD   dwCostSilver ;       // 花费的游戏币
	tagTalentAvailCost() { ZeroMemory(this, sizeof(*this)); }
};

//---------------------------------------------------------------------------------------
// 玩家脚本数据类型定义
//---------------------------------------------------------------------------------------
enum ERoleScriptData
{
	ERSD_Injury			=	0,				// 0用作内伤治疗状态
	ERSD_END			=	100,
};

// 月份枚举
enum EMonth
{
	EM_January = 0,
	EM_February,
	EM_March,
	EM_April,
	EM_May,
	EM_June,
	EM_July,
	EM_August,
	EM_September,
	EM_October,
	EM_November,
	EM_December,

	EM_End
};
//-------------------------------------------------------------------------------------
// 角色每月在线时长统计(在线时长单位为分钟)
//-------------------------------------------------------------------------------------
struct tagMonthOnlineTime 
{
	tagDWORDTime	dwCurTime;								// 当前时间
	UINT16				n16OnlineTime[EM_End];		// 角色当年每月在线时间
	tagMonthOnlineTime() { ZeroMemory(this, sizeof(*this));  }
};

//-------------------------------------------------------------------------------------------
struct tagAreaSave
{
	DWORD	dwAreaID;				//领域ID
	DWORD	dwRoleID;				//角色ID
	BOOL	bActivation;			//激活状态 
	DWORD	dwCD;					//CD时间
};
//-------------------------------------------------------------------------------------------

#pragma pack(pop)