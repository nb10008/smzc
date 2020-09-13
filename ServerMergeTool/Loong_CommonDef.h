#pragma once
#include "stdafx.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/login.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/vcard_define.h"
#include "../WorldDefine/mall_define.h"
#include "../WorldDefine/msg_external_links.h"
#include "../WorldDefine/quest_board.h"
#include "../WorldDefine/msg_eggbroker.h"
#include "../WorldDefine/msg_family.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/login_define.h"
#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/msg_famehall.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/guild_define.h"
#include "../ServerDefine/pet_define.h"
#include "../ServerDefine/msg_vipstall.h"
#include "../ServerDefine/activity_define.h"
#include "../ServerDefine/msg_city.h"
#include "../ServerDefine/master_apprentice.h"
#include "../ServerDefine/msg_shortcut_quickbar.h"
#include "../ServerDefine/msg_draft_show.h"
#include "../ServerDefine/guild_war_declare_define.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/msg_godbid.h"
#include "../ServerDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_fashion.h"
#include "../ServerDefine/msg_family.h"
#include "../ServerDefine/msg_family_sprite.h"
#include "../ServerDefine/msg_account_reactive.h"


#pragma pack(push,1)
//------------------------------------------------------------------------------
// 宏
//------------------------------------------------------------------------------
#define ALING_TO_EIGHT(v) ( (v) = (v) + (8-(v)%8) )			// 向上8字节对齐

#define CONFIG_INI			"ServerMerge.ini"
#define COMPENSATION_INI	"ServerMerge_C.ini"		// 玩家补偿
#define LOG_FOLDER			"Log"
#define MIBAO_LEN			240						// 密保卡程度
#define DB_PAGING			750000					// 分页数据量75W
#define THREAD_POOL_NUM		3						// 线程池线程数
#define ROLE_MAX_LIMIT		3						// 单账号人数限制
#define ACTIVITY_DATA_NUM	5						// 脚本数据个数

#define DB_PAGINGSPEC		350000					// 对于单行数据量较大的表，分页合并且每页查询记录数较少

#define ITEM_DEL_EQUIP_DEL_SURVIVE(nDay)	(3600*24*(nDay))		// item_del和equip_del幸存时间 一个月

//------------------------------------------------------------------------------
// 枚举
//------------------------------------------------------------------------------
enum ERoleCompensationType
{
	ERC_START				= 0,	//开始

	ERC_CHANGE_ROLE_NAME	= 0,	//换名的补偿

	ERC_END					= 1,
};

enum EThreadPoolState
{
	ETPS_NULL				= -1,

	ETPS_STILL_HAVE_ACTIVE	= 0, //仍然有线程在工作
	ETPS_SUCCESS			= 1, //成功
	ETPS_FAILURE			= 2, //失败
};

//分页查询状态
enum EPagingState
{		
	EPS_DB_NULL				= -1,

	EPS_DB_SUCCESS			= 0,		//成功
	EPS_DB_ERROR			= 1,		//失败
	EPS_DB_RESULT_NULL		= 2,		//结果集空
};

//db类型
enum EDBType 
{
	EM_DB_NULL				= -1,

	EM_DB_SRC_ONE			= 0,		//源一 DB
	EM_DB_SRC_TWO			= 1,		//源二 DB
	EM_DB_TAR				= 2,		//目标 DB

	EM_DB_TYPE_END = 3,
};

//合并类型
enum EDBMergeType 
{
	EM_MERGE_NULL			= -1,

	EM_MERGE_INSIDE_SECTION = 0,		//大区内
	EM_MERGE_CROSS_SECTION	= 1,		//跨大区

	EM_MERGE_TYPE_END		= 2,
};

//item  分类
enum ETBItemType 
{
	EM_TB_NULL			= -1,

	EM_TB_ITEM			= 0,		//item
	EM_TB_ITEM_BAIBAO	= 1,		//item baibao
	EM_TB_ITEM_DEL		= 2,		//item del

	EM_TB_END			= 3,
};

//equip  分类
enum ETBEquipType
{
	EM_TB_EQUIP_NULL	= -1,

	EM_TB_EQUIP			= 0,			//equip
	EM_TB_EQUIP_BAIBAO	= 1,			//equip baibao
	EM_TB_EQUIP_DEL		= 2,			//equip del

	EM_TB_EQUIP_END		= 3,
};

//角色删除类型
enum ETBRoleRemoveType
{
	EM_R_NULL				=	-1,

	EM_R_ONESELF			=	1,		//自删除
	EM_R_MERGE				=	2,		//合并时删除
	EM_R_ROLEDATA_CLEAN		=	3,		//清理roledata表时

	EM_R_END				=	4,
};

//db查询扩展选项
enum ETBQueryExtraOption
{
	EM_TBQ_EOPTION_NULL			= -1,

	EM_TBQ_EOPTION_GUILDWARE	= 0,	//帮派仓库物品

	EM_TBQ_EOPTION_END			= 1,

};

//------------------------------------------------------------------------------
// 结构体
//------------------------------------------------------------------------------
//用户配置信息
struct tagConfigDetail{
	tstring strIp;		//ip
	tstring strUser;	//用户名
	tstring strPsd;		//密码
	tstring strDBName;	//数据库名
	DWORD	dwPort;		//数据库端口
	tagConfigDetail()
	{
		memset(this,0,sizeof(tagConfigDetail));
	}
};


//------------------------------------------------------------------------------
// 结构体 login数据库表格
//------------------------------------------------------------------------------
struct tagTWrap_Longin_Account
{
	CHAR	szName[X_SHORT_NAME];			// 用户名
	CHAR	szPsd[MAX_MD5_ARRAY];			// 密码
	CHAR	szMibao[MIBAO_LEN];				// 密保
	tagProofResult stOthers;

	tagTWrap_Longin_Account()
	{
		memset(szName,0,X_SHORT_NAME);

		memset(szPsd,0,MAX_MD5_ARRAY);

		memset(szMibao,0,MIBAO_LEN);
	}
};
struct tagTWrap_Longin_WorldForbid
{
	DWORD dwAccountID;
	DWORD dwWorldNameCrc;

	tagTWrap_Longin_WorldForbid()
	{
		dwAccountID = 0;
		dwWorldNameCrc = 0;
	}
};
//黑名单
struct tagTWrap_Longin_BlackList
{
	CHAR  szIP[X_IP_LEN];				//ip

	tagTWrap_Longin_BlackList()
	{
		memset(this,0,sizeof(tagTWrap_Longin_BlackList));
	}
};
//防沉迷
struct tagTWrap_Longin_FatigueTime
{
	DWORD dwAccountNameCrc;
	DWORD dwOnlineTime;
	DWORD dwOfflineTime;

	tagTWrap_Longin_FatigueTime()
	{
		memset(this,0,sizeof(tagTWrap_Longin_FatigueTime));
	}
};

// 全局角色名字
struct tagTWrap_Longin_RoleName
{
	TCHAR	szName[X_SHORT_NAME];			// 用户名

	tagTWrap_Longin_RoleName()
	{
		memset(this,0,sizeof(tagTWrap_Longin_RoleName));
	}
};

// 全局帮派名字
struct tagTWrap_Longin_GuildName
{
	TCHAR	szName[X_SHORT_NAME];			// 帮派名

	tagTWrap_Longin_GuildName()
	{
		memset(this,0,sizeof(tagTWrap_Longin_GuildName));
	}
};

// 全局家族名字
struct tagTWrap_Longin_FamilyName
{
	TCHAR	szName[X_SHORT_NAME];			// 家族名

	tagTWrap_Longin_FamilyName()
	{
		memset(this,0,sizeof(tagTWrap_Longin_FamilyName));
	}
};

//------------------------------------------------------------------------------
// 结构体 loong数据库表格
//------------------------------------------------------------------------------
struct tagTWrap_Loong_AccountCommon
{
	DWORD dwAccountID;
	CHAR  szAccountName[X_SHORT_NAME];// 用户名

	tagDWORDTime timeLastLogin;//该账号上次登入时间
	tagDWORDTime timeLastLogout;//该账号上次登出时间
	tagDWORDTime timeLastReceiveDailyOfflineReward;//上次领取离线奖励时间
	tagDWORDTime timeLastReceiveRegression;//上次领取回归奖励时间

	tagAccountCommon stOthers;

	tagTWrap_Loong_AccountCommon()
	{
		dwAccountID = GT_INVALID;
		memset(szAccountName,0,X_SHORT_NAME);
	}
};

//BlackList
struct tagTWrap_Loong_BlackList
{
	DWORD dwRoleID;
	DWORD dwBlackID;

	tagTWrap_Loong_BlackList()
	{
		dwRoleID = GT_INVALID;
		dwBlackID = GT_INVALID;
	}
};
//buff
struct tagTWrap_Loong_Buff
{
	DWORD dwRoleID;
	CHAR *pcBlob;

	tagBuffSave stOthers;

	tagTWrap_Loong_Buff()
	{
		dwRoleID = GT_INVALID;
		pcBlob = NULL;
	}
	~tagTWrap_Loong_Buff()
	{
		if(pcBlob != NULL)
		{
			delete []pcBlob;
		}
	}
};

//氏族
struct tagTWrap_Loong_ClanData
{
	DWORD dwRoleID;
	bool  bRemoveFlag;
	tagDBReputeData stOthers;

	tagTWrap_Loong_ClanData()
	{
		dwRoleID = GT_INVALID;
		bRemoveFlag = false;
	}
};
//Enemy
struct tagTWrap_Loong_Enemy
{
	DWORD dwRoleID;
	DWORD dwEnemyID;

	tagTWrap_Loong_Enemy()
	{
		dwRoleID = GT_INVALID;
		dwEnemyID = GT_INVALID;
	}
};
//equip
struct tagTWrap_Loong_Equip
{
	INT64 n64SerialNum;
	tagEquip stOthers;

	tagTWrap_Loong_Equip()
	{
		n64SerialNum = GT_INVALID;
	}
};
struct tagTWrap_Loong_EquipDel
{
	INT64 n64SerialNum;
	tagEquip stOthers;

	tagTWrap_Loong_EquipDel()
	{
		n64SerialNum = GT_INVALID;
	}
};
struct tagTWrap_Loong_EquipBaiBao
{
	INT64 n64SerialNum;
	tagEquip stOthers;

	tagTWrap_Loong_EquipBaiBao()
	{
		n64SerialNum = GT_INVALID;
	}
};

//氏族珍宝激活表
typedef tagNDBC_InsertActClanTreasure tagTWrap_Loong_ActClanTreasure;

//氏族进入快照
typedef tagNDBC_InsertFameHallSnap tagTWrap_Loong_FameHallEnterSnap;

//氏族声望重置时间快照
typedef tagNDBC_UpDateRepRstTime tagTWrap_Loong_RepRstTime;

//好友
struct tagTWrap_Loong_Friend
{
	DWORD dwRoleID;
	tagFriendSave stOthers;

	tagTWrap_Loong_Friend()
	{
		dwRoleID = GT_INVALID;
	}
};

//友好度
struct tagTWrap_Loong_FriendshipSave
{
	DWORD dwRoleID;
	tagFriendshipSave stOthers;

	tagTWrap_Loong_FriendshipSave()
	{
		dwRoleID = GT_INVALID;
	}
};

//团购
struct tagTWrap_Loong_GroupPurchase
{
	CHAR *pcBlob;
	DWORD dwBlobSize;

	tagGPInfo stOthers;

	tagTWrap_Loong_GroupPurchase()
	{
		pcBlob = NULL;
		dwBlobSize = 0;
	}
	~tagTWrap_Loong_GroupPurchase()
	{
		if(pcBlob != NULL)
		{
			delete []pcBlob;
		}
	}
};
//帮派
struct tagTWrap_Loong_Guild
{
	char szDate[X_DATATIME_LEN + 1];
	tagGuildLoad stOthers;

	tagTWrap_Loong_Guild()
	{
		memset(szDate,0,X_DATATIME_LEN+1);
	}
};
//帮派成员
struct tagTWrap_Loong_GuildMem
{
	char szDate[X_DATATIME_LEN + 1];
	tagGuildMemLoad stOthers;

	tagTWrap_Loong_GuildMem()
	{
		memset(szDate,0,X_DATATIME_LEN+1);
	}
};
//item

//item
struct tagTWrap_Loong_Item
{
	tagItem stOthers;
};
struct tagTWrap_Loong_ItemBaiBao
{
	tagItem stOthers;
};
struct tagTWrap_Loong_ItemDel
{
	tagItem stOthers;
};

//item cd time
struct tagTWrap_Loong_ItemCDTime
{
	DWORD dwRoleID;
	std::vector <tagCDTime> vecCDTime;

	tagTWrap_Loong_ItemCDTime()
	{
		dwRoleID = GT_INVALID;
	}

};

//item need log
struct tagTWrap_Loong_ItemNeedLog
{
	DWORD dwTypeID; //'物品的TypeID(唯一)',
	BOOL  bNeedLog;	//'是否记录log(0:不记录;1:记录)',
	SHORT sMinQlty;	//'需记录log的物品的最低品级',

	tagTWrap_Loong_ItemNeedLog()
	{
		dwTypeID = GT_INVALID;
		bNeedLog = TRUE;
		sMinQlty = 0;
	}
};

//log baibao
struct tagTWrap_Loong_LogBaiBao
{
	DWORD dwAccountID;
	TCHAR  *ptcWords;
	tagBaiBaoRecord stOthers;

	tagTWrap_Loong_LogBaiBao()
	{
		dwAccountID = GT_INVALID;
		ptcWords     = NULL;
	}
};


//角色删除
struct tagTWrap_Loong_RoleDel
{
	DWORD dwAccountID;
	DWORD dwRoleID;
	TCHAR  szRoleName[X_SHORT_NAME];				// 名
	TCHAR  szIP[X_IP_LEN];							// ip
	TCHAR  szDeleteTime[X_DATATIME_LEN + 1];		// del time

	tagTWrap_Loong_RoleDel()
	{
		memset(this,0,sizeof(tagTWrap_Loong_RoleDel));

		dwAccountID = GT_INVALID;
		dwRoleID = GT_INVALID;
	}

};
//角色数据
struct tagTWrap_Loong_RoleData
{
	DWORD dwAccountID;
	DWORD dwRoleID;
	BOOL  bRemoveFlag;
	INT64 n64WorkedLevel; //加成过的等级值，用于等级排名
	CHAR  szRemoveTime[X_DATATIME_LEN + 1];		// del time

	//1.5.0新增
	DWORD dwTStateTblSize;

	tagRoleDataLoad	stOthers;

	//试炼
	tagTrainStateTable stTrainStateTable;

	tagTWrap_Loong_RoleData()
	{
		bRemoveFlag		= FALSE;
		dwRoleID		= GT_INVALID;
		dwAccountID		= GT_INVALID;
		dwTStateTblSize = 0;
		memset(szRemoveTime ,0 ,X_DATATIME_LEN + 1);
	}

};
struct tagTWrap_Loong_RoleDataDel
{
	DWORD dwAccountID;
	DWORD dwRoleID;
	BOOL  bRemoveFlag;
	CHAR  szRemoveTime[X_DATATIME_LEN + 1];		// del time
	tagRoleDataLoad	stOthers;

	tagTWrap_Loong_RoleDataDel()
	{
		bRemoveFlag = FALSE;
		dwRoleID = GT_INVALID;
		dwAccountID = GT_INVALID;
		memset(szRemoveTime ,0 ,X_DATATIME_LEN + 1);
	}

};
//技能
struct tagTWrap_Loong_Skill
{

	DWORD dwRoleID;
	tagSkillSave stOthers;

	tagTWrap_Loong_Skill()
	{
		dwRoleID = GT_INVALID;
	}
};

//任务
struct tagTWrap_Loong_Task
{

	DWORD dwRoleID;
	tagQuestSave stOthers;

	tagTWrap_Loong_Task()
	{
		dwRoleID = GT_INVALID;
	}
};

//完成任务
struct tagTWrap_Loong_TaskDone
{
	
	DWORD dwRoleID;
	tagQuestDoneSave stOthers;

	tagTWrap_Loong_TaskDone()
	{
		dwRoleID = GT_INVALID;
	}
};

//称号
struct tagTWrap_Loong_Title
{

	DWORD dwRoleID;
	tagTitleSave stOthers;

	tagTWrap_Loong_Title()
	{
		dwRoleID = GT_INVALID;
	}
};

//名帖
struct tagTWrap_Loong_VisitingCard
{
	TCHAR tcHeadUrl[LEN_MAX_URL];
	TCHAR tcSignature[LEN_MAX_SIGNATURE];

	TCHAR tcNameHistory[500];

	tagVCardData stOthers;

	tagTWrap_Loong_VisitingCard()
	{
		memset(tcHeadUrl,0,LEN_MAX_URL);
		memset(tcSignature,0,LEN_MAX_SIGNATURE);
		memset(tcNameHistory,0,500);
	}
};



//元宝
typedef tagYBAccount tagTWrap_Loong_YBAccount;

//元宝交易订单
typedef tagYuanBaoOrder tagTWrap_Loong_YuanBaoOrder;

//bill_item
struct tagTWrap_Loong_BillItem
{
	CHAR  szTokenID[50];	// 
	DWORD dwAccountID;
	DWORD dwItemTypeID;
	INT nNum;
	tagTWrap_Loong_BillItem()
	{
		memset(this,0,sizeof(tagTWrap_Loong_BillItem));
	}
};
//bill_yuanbao
struct tagTWrap_Loong_BillYuanBao
{
	CHAR  szTokenID[50];	// 
	DWORD dwAccountID;
	INT	nNum;

	tagTWrap_Loong_BillYuanBao()
	{
		memset(this,0,sizeof(tagTWrap_Loong_BillYuanBao));
	}
};
//bill_yuanbao_log
struct tagTWrap_Loong_BillYuanBaoLog
{
	CHAR  szTokenID[50];	// 
	DWORD dwAccountID;
	INT	  nNum;
	CHAR  szTime[X_DATATIME_LEN + 1];//time

	tagTWrap_Loong_BillYuanBaoLog()
	{
		memset(this,0,sizeof(tagTWrap_Loong_BillYuanBaoLog));
	}
};
//pet_data
struct tagTWrap_Loong_PetData
{
	bool bRemoveFlag;
	tagDBPetAtt stOthers;

	tagTWrap_Loong_PetData()
	{
		bRemoveFlag = false;
	}
};


//pet_skill
struct tagTWrap_Loong_PetSkill
{
	DWORD dwPetID;
	tagDBPetSkill stOthers;
	tagTWrap_Loong_PetSkill()
	{

	}
};

//vip_stall
typedef tagVipStall2DB tagTWrap_Loong_VipStall;

//guild_commodity
struct tagTWrap_Loong_GuildCommodity
{
	DWORD dwGuildID;
	DWORD dwGoodInfoSize;
	tagGuildCommerceInfo stOthers;

	tagTWrap_Loong_GuildCommodity()
	{
		dwGuildID = 0;
		dwGoodInfoSize = 0;
	}
};
//guild_skill
struct tagTWrap_Loong_GuildSkill
{
	DWORD dwGuildID;
	tagGuildSkillInfo stOthers;

	tagTWrap_Loong_GuildSkill()
	{
		dwGuildID = 0;
	}
};

//金牌网吧
struct tagTWrap_Loong_VipNetBar
{	
	DWORD dwAccountID;
	DWORD dwLoginTime;

	tagTWrap_Loong_VipNetBar()
	{
		dwAccountID = 0;
		dwLoginTime = 0;
	}
};

//帮派设施
typedef tagFacilitiesLoad tagTWrap_Loong_GuildFacilities;

//跑商排名
struct tagTWrap_Loong_CommerceRank
{
	DWORD dwGuildID;
	tagCommerceRank stOthers;

	tagTWrap_Loong_CommerceRank()
	{
		dwGuildID = 0;
	}
};

//活动脚本
struct tagTWrap_Loong_ActivityData
{
	DWORD			dwID;		// 活动ID
	tagActivityData stAData[ACTIVITY_DATA_NUM];
	BOOL			bHaveData[ACTIVITY_DATA_NUM];

	tagTWrap_Loong_ActivityData()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//留言
struct tagTWrap_Loong_LeftMsg
{
	DWORD dwMsgID;

	DWORD dwRoleID;

	CHAR *pcBlob;
	DWORD dwBlobSize;

	tagTWrap_Loong_LeftMsg()
	{
		dwMsgID		= GT_INVALID;
		dwRoleID	= GT_INVALID;
		pcBlob		= NULL;
		dwBlobSize	= 0;
	}
	~tagTWrap_Loong_LeftMsg()
	{
		if(pcBlob != NULL)
		{
			delete []pcBlob;
		}
	}

};

//外部链接表
typedef tagExternalLink tagTWrap_Loong_ExternalLink;

//记录改名记录的表  
struct tagTWrap_Loong_NameHistory
{
	DWORD dwAccountID;
	DWORD dwRoleID;
	TCHAR  szRoleName[X_SHORT_NAME*10];				// 名

	tagTWrap_Loong_NameHistory()
	{
		memset(this,0,sizeof(tagTWrap_Loong_NameHistory));
	}

};

//city
struct tagTWrap_Loong_City
{

	tagNDBS_LoadCityInfo stOthers;

	CHAR *pcSigupList;

	tagTWrap_Loong_City()
	{
		memset(this,0,sizeof(tagTWrap_Loong_City));
	}
	~tagTWrap_Loong_City()
	{
		SAFE_DEL_ARRAY(pcSigupList);
	}
};

//master
typedef tagMasterApprentice2DB tagTWrap_Loong_Master;


//快捷栏
typedef tagNDBS_LoadShortCut_QuickBar tagTWrap_Loong_ShortCutBar;


//龙之试炼
struct tagTWrap_Loong_TrainState
{
	DWORD dwRoleID;

	tagTrainState stOthers;

	tagTWrap_Loong_TrainState()
	{
		memset(this,0,sizeof(tagTWrap_Loong_TrainState));
	}
};

//世博活动
typedef tagActiveRankData tagTWrap_Loong_ActiveRankData;

//fabao
struct tagTWrap_Loong_Fabao
{
	INT64 n64Serial;
	tagFabao stOthers;

	tagTWrap_Loong_Fabao()
	{
		memset(this,0,sizeof(tagTWrap_Loong_Fabao));
	}
};

// holyman
struct tagTWrap_Loong_Holy
{
	INT64 n64Serial;
	tagHolyMan stOthers;

	tagTWrap_Loong_Holy()
	{
		memset(this,0,sizeof(tagTWrap_Loong_Holy));
	}
};

// holyequip
struct tagTWrap_Loong_HolyEquip
{
	INT64 n64Serial;
	tagHolyEquip stOthers;

	tagTWrap_Loong_HolyEquip()
	{
		memset(this,0,sizeof(tagTWrap_Loong_HolyEquip));
	}
};

// 强制脱离帮派
typedef tagForceBreakOutApprentice tagTWrap_Loong_ForceBreakOutApprentice;

//------------------------------------------------------------------------------
//FPlan新增
//------------------------------------------------------------------------------

// 任务版
typedef tagBoardQuestSaveData tagTWrap_Sheng_BoardQuestSaveData;

// 工会宣战
typedef tagGuildWarDeclare tagTWrap_Sheng_GuildWarDeclare;

// 玩家在线时间统计
struct tagTWrap_Sheng_CurMonthOnlineTime
{
	DWORD dwAccountID;
	DWORD dwRoleID;
	BYTE  byYear;

	//{"January", "February","March","April","May","June","July","August","September","October","November","December"};
	UINT16 nJanuary;
	UINT16 nFebruary;
	UINT16 nMarch;
	UINT16 nApril;
	UINT16 nMay;
	UINT16 nJune;
	UINT16 nJuly;
	UINT16 nAugust;
	UINT16 nSeptember;
	UINT16 nOctober;
	UINT16 nNovember;
	UINT16 nDecember;

	tagDWORDTime dwLastUpdateTm;

	tagTWrap_Sheng_CurMonthOnlineTime()
	{
		ZeroMemory(this, sizeof(*this));
	}
		
};

// 妖精重生
typedef tagNDBS_GetFabaoNirVanaData tagTWrap_Sheng_FabaoNirVanaData;

// 神之竞标
typedef tagGodBidInfo2DB tagTWrap_Sheng_GodBid;

//砸蛋
struct  tagTWrap_Sheng_EggInfo
{
	DWORD dwRoleID;
	tagEggInfoInDB stOthers;

	tagTWrap_Sheng_EggInfo()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

// 衣橱
typedef tagWardrobeLayer2DB tagTWrap_Sheng_Wardrobe;

// 领域
typedef tagAreaSave tagTWrap_Sheng_Area;

// 稀有领域
typedef tagGlobalLimitedItem tagTWrap_Sheng_RareArea;

//------------------------------------------------------------------------------
// 结构体 用于转换对照表的结构
//------------------------------------------------------------------------------

//玩家补偿
struct tagRoleCompensation
{
	ERoleCompensationType	Type;	// 补偿类型
	DWORD	dwAccountID;			// 账号id
	DWORD	dwRoleID;				// 玩家id

	tagRoleCompensation()
	{
		Type			= ERC_START;
		dwAccountID		= GT_INVALID;
		dwRoleID		= GT_INVALID;
	}

	tagRoleCompensation(ERoleCompensationType _Type, DWORD _AccountID, DWORD _RoleID)
	{
		Type			= _Type;
		dwAccountID		= _AccountID;
		dwRoleID		= _RoleID;
	}
	~tagRoleCompensation()
	{

	}
};

//补偿
struct tagCompensation
{
	tagCompensation()
	{
	
	}
	virtual ~tagCompensation()
	{

	}
};
//物品补偿
struct tagItemCompensation : public tagCompensation
{
	DWORD	dwItemTypeID;

	tagItemCompensation()
	{
		dwItemTypeID	= GT_INVALID;
	}
	~tagItemCompensation()
	{

	}
	tagItemCompensation operator = (const tagItemCompensation& rhs)
	{
		dwItemTypeID = rhs.dwItemTypeID;

		return *this;
	}
	bool operator < (const tagItemCompensation& rhs) const
	{
		return dwItemTypeID < rhs.dwItemTypeID;
	}
};
//////////////////////////////////////////////////////////////////////////
struct tagRoleIDKey
{
	DWORD dwRoleID;

	EDBType emDBType;

	tagRoleIDKey(DWORD dwRoleID,EDBType emDBType)
	{
		this->dwRoleID = dwRoleID;
		this->emDBType = emDBType;
	}
	bool operator<(const tagRoleIDKey& ver) const
	{
		if( dwRoleID<ver.dwRoleID)
			return true;
		else if(dwRoleID>ver.dwRoleID)
			return false;

		if( emDBType<ver.emDBType)
			return true;
		else if(emDBType>ver.emDBType)
			return false;

		return false;
	}

};

struct tagFamilyIDKey
{
	DWORD dwFamilyID;		// 家族id

	EDBType emDBType;

	tagFamilyIDKey(DWORD dwFamilyID,EDBType emDBType)
	{
		this->dwFamilyID = dwFamilyID;
		this->emDBType = emDBType;
	}
	bool operator<(const tagFamilyIDKey& ver) const
	{
		if( this->dwFamilyID<ver.dwFamilyID)
			return true;
		else if(this->dwFamilyID>ver.dwFamilyID)
			return false;

		if( emDBType<ver.emDBType)
			return true;
		else if(emDBType>ver.emDBType)
			return false;

		return false;
	}
};

struct tagGuildIDKey
{
	DWORD dwGuildID;		// 帮派id

	EDBType emDBType;

	tagGuildIDKey(DWORD dwGuildID,EDBType emDBType)
	{
		this->dwGuildID = dwGuildID;
		this->emDBType = emDBType;
	}
	bool operator<(const tagGuildIDKey& ver) const
	{
		if( this->dwGuildID<ver.dwGuildID)
			return true;
		else if(this->dwGuildID>ver.dwGuildID)
			return false;

		if( emDBType<ver.emDBType)
			return true;
		else if(emDBType>ver.emDBType)
			return false;

		return false;
	}
};
struct tagAccountIDKey
{
	DWORD dwAccountID;

	EDBType emDBType;

	tagAccountIDKey(DWORD dwAccountID,EDBType emDBType)
	{
		this->dwAccountID = dwAccountID;
		this->emDBType = emDBType;
	}
	bool operator<(const tagAccountIDKey& ver) const
	{
		if(dwAccountID<ver.dwAccountID)
			return true;
		else if(dwAccountID>ver.dwAccountID)
			return false;

		if( emDBType<ver.emDBType)
			return true;
		else if(emDBType>ver.emDBType)
			return false;

		return false;
	}
};

struct tagActClanTreasureKey
{
	BYTE	byClanType;				// 氏族类型
	UINT16	u16TreasureID;			// 珍宝关联的物品id

	tagActClanTreasureKey(BYTE	byClanType,UINT16	u16TreasureID)
	{
		this->byClanType = byClanType;
		this->u16TreasureID = u16TreasureID;
	}
	bool operator<(const tagActClanTreasureKey& ver) const
	{
		if(byClanType<ver.byClanType)
			return true;
		else if(byClanType>ver.byClanType)
			return false;

		if( u16TreasureID<ver.u16TreasureID)
			return true;
		else if(u16TreasureID>ver.u16TreasureID)
			return false;

		return false;
	}
};
/*
struct tagItemNumKey
{
	DWORD dwItemSerialNum;

	EDBType emDBType;

	tagItemNumKey(DWORD dwItemSerialNum,EDBType emDBType)
	{
		this->dwItemSerialNum = dwItemSerialNum;
		this->emDBType = emDBType;
	}
	bool operator<(const tagItemNumKey& ver) const
	{
		if(dwItemSerialNum<ver.dwItemSerialNum)
			return true;
		else if(dwItemSerialNum>ver.dwItemSerialNum)
			return false;

		if( emDBType<ver.emDBType)
			return true;
		else if(emDBType>ver.emDBType)
			return false;

		return false;
	}
};
*/

struct tagTWrap_Loong_Family
{
	tagFamilyLoad stFamilyAtt;

	tagTWrap_Loong_Family()
	{
		memset( stFamilyAtt.strFamilyName, 0, sizeof(stFamilyAtt.strFamilyName) );
	}
};

struct tagTWrap_Loong_FamilyMem
{
	tagFamilyMember stFamilyMem;
};

struct tagTWrap_Loong_FamilyQuset
{
	tagFamilyQuestLoadData stFamilyQuest;
};

struct tagTWrap_Loong_FamilyRoleSprite
{
	DWORD dwFamilyID;
	tagRoleSpriteDataSave stFamilyRoleSprite;
};

struct tagTWrap_Loong_FamilySprite
{
	tagFamilySpriteSave stFamilySprite;
};

struct tagTWrap_Loong_SpriteRequirement
{
	DWORD	dwFamilyID;
	tagFamilySpriteRequirementSave stSpriteRequirement;
};

// F-Plan 1.5.0
struct tagTWrap_Loong_SoulCrystal
{
	INT64 n64SerialNum;
	tagSoulCrystal stOthers;

	tagTWrap_Loong_SoulCrystal()
	{
		n64SerialNum = GT_INVALID;
	}
};


// F-Plan 1.5.0 account_reactive
struct tagTWrap_Loong_AccountReactive
{
	DWORD dwAccountID;
	tagAccountReactiveInfo stOthers;

	tagTWrap_Loong_AccountReactive()
	{
		dwAccountID = GT_INVALID;
	}
};

// F-Plan 1.5.0 account_reactiver
struct tagTWrap_Loong_AccountReactiver
{
	DWORD dwAccountID;
	tagAccountReactiverInfo stOthers;

	tagTWrap_Loong_AccountReactiver()
	{
		dwAccountID = GT_INVALID;
	}
};

// F-Plan 1.5.12 account_reactiver
struct tagTWrap_Loong_UseKeyCode
{
	DWORD dwRoleID;
	
	tagTWrap_Loong_UseKeyCode()
	{
		dwRoleID = GT_INVALID;
	}
};

struct tagFamilyNeedChangeName
{
	tagFamilyNeedChangeName()
	{
		Clear();
	};

	~tagFamilyNeedChangeName()
	{
		Clear();
	}

	VOID Clear()
	{
		memset( tszOldName, 0, MAX_FAMILY_NAME );
		memset( tszNewName2, 0, MAX_FAMILY_NAME );
		dwNewCRCID2 = 0;
		dwOldCRCID = 0;
	}

	TCHAR tszOldName[MAX_FAMILY_NAME];
	TCHAR tszNewName2[MAX_FAMILY_NAME];
	DWORD dwNewCRCID2;	
	DWORD dwOldCRCID;	
};


struct tagTWrap_Loong_GmServerData
{
	INT64 n64Value;
};


#pragma pack(pop)