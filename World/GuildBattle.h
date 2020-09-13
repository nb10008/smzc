//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: GuildBattle.h
// author: chdeng
// actor:
// data: 2009-01-26
// last:
// brief: 帮战
//-------------------------------------------------------------------------------
#pragma once
#include "activity_mgr.h"
#include "role_mgr.h"
#include "guild_mgr.h"
#include "guild.h"
#include <hash_map>
#include <list>
#include "world.h"

#include "../WorldDefine/time.h"
#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/guild_define.h"

#define GUILBATTLE_ACTIVITY_ID 25

enum EBroadcastType
{
	EBT_Start = 0,

	EBT_Attack = 1,							// 进攻方
	EBT_AttackAlliance = 2,				// 进攻同盟
	EBT_DefendAlliance = 3,			// 防守同盟

	EBT_End,
};

//  帮派中优秀的玩家积分
struct tagGuildMaxScoresPlayer
{
	DWORD dwGuildID;
	DWORD dwRoleID;
	DWORD dwScores;
	tagGuildMaxScoresPlayer():dwGuildID(0),dwRoleID(0),dwScores(0)
	{}
};

// 复活角色信息
struct tagRevieRoleInfo
{
	DWORD dwRoleID;
	DWORD dwCurTick;
	tagRevieRoleInfo():dwRoleID(0),dwCurTick(0)
	{}
};


//这里写法不够高效，可重复利用
// 被杀玩家
struct tagBeKillerInfo
{
	DWORD dwBeKilledRoleID;
	DWORD dwTime;
	tagBeKillerInfo():dwBeKilledRoleID(0),dwTime(0) { Init(); }

	BOOL Init()
	{
		dwBeKilledRoleID = 0;
		dwTime			 = 0;
		return TRUE; 
	}
public:
	void  SetRoleID(DWORD inRoleID) { dwBeKilledRoleID = inRoleID;}
	DWORD GetRoleID()				{ return dwBeKilledRoleID;}

	void  SetTime(DWORD inTime)		{dwTime = inTime;}
	DWORD GetTime()					{ return dwTime;}
};

// 战场占领分数奖励
const INT32 CAP_POINT_REWARDS[] = {
// 分数     经验奖励倍数 金钱奖励倍数
	30,      3,                 1,
	60,   	 6,                 2,
	120,     12,                4,
	180,     20,                6,
	240,     30,                8,
	300,     40,                10
};

// 活跃时间奖励
const INT32 ACTIVE_TIME_REWARDS[] = {
// 活跃时间    经验奖励倍数   金钱奖励倍数
	15,              10,       2,
	30,              10,       2,
	45,              10,       2
};

// 最终积分上限
const INT MAX_MIRACLES_POINTS = 5000;

#define CAL_FINIALPOINTS(active_time, kill_times) ((active_time)*20+(kill_times)*20)

// 玩家信息
struct tagGodRoleInfo
{
public:
	DWORD	dwRoleID;				// 角色ID
	DWORD	dwKillTimes;			// 击杀次数
	DWORD	dwActiveTime;			// 活跃时间
	BOOL	bIsInGodMiracleArea;	// 是否在神迹区域
	
	BOOL    bIsBeCheck;

	DWORD	dwGodID;				// 所在的神迹区域

	BOOL    bChange;				// 击杀和活跃时间是否改变

	BOOL	nKillStopFalg;		// 60斩停止标志

	INT   nExp;					// 所得经验
	INT   nMoney;				// 所得金钱

	tagBeKillerInfo	sBeKilledRole[3];

	tagGodRoleInfo():dwRoleID(0),dwKillTimes(0),dwActiveTime(0),bIsInGodMiracleArea(FALSE),
					 bIsBeCheck(0),dwGodID(-1),bChange(FALSE),nKillStopFalg(FALSE),nExp(0),nMoney(0)
	{}
	BOOL Init()
	{
		dwRoleID	= 0;
		dwKillTimes = 0;
		dwActiveTime= 0;
		bIsInGodMiracleArea =FALSE;
		bIsBeCheck  = 0;
		dwGodID		= -1;
		bChange     = 0;
		nKillStopFalg	= FALSE;
		nExp = 0;
		nMoney = 0;
	}

public:
	void   SetGodID(DWORD inGodID)	{ dwGodID = inGodID;}
	DWORD  GetGodID()					{ return dwGodID;}	

	void  SetRoleID(DWORD inRoleID)			{ dwRoleID = inRoleID; }
	DWORD GetRoleID()						{ return dwRoleID; }

	void  ModKillTimes(DWORD inKillTimes)	{ dwKillTimes += inKillTimes;}
	DWORD GetKillTimes()					{ return dwKillTimes;}

	void  SetActiveTime(DWORD inActiveTime)	{ dwActiveTime = inActiveTime;}
	DWORD GetActiveTime()					{ return dwActiveTime;}
	BOOL  GetAreaFlag()						{ return bIsInGodMiracleArea ;}

	void  SetAreaFlag(BOOL inFlag)			{ bIsInGodMiracleArea = inFlag;}
	BOOL  SetBeKilledRole(DWORD inBeKilledRoleID, DWORD dwGuildID, BOOL bIsTeamFlag = FALSE);
	
	void  SetBeCheck(BOOL bType)			{ bIsBeCheck = bType;}
	BOOL  GetBeCheck()						{ return bIsBeCheck; }

	VOID IncExp(INT nAddExp)	{ nExp += nAddExp; }
	VOID IncMoney(INT nAddMoney) 	{ nMoney += nAddMoney; }

public:
	// 发送活跃时间和击杀次数改边的适时更新消息
	void SendChangeActiveTimeAndKillTime();

	VOID  UpdateMin();
	VOID Update();
	VOID GiveActiveTimeReward(DWORD dwActiveTime, DWORD dwLastTime);
	static VOID GiveReward(DWORD	dwGodID, EBattleFieldRewardType eRewardType, DWORD dwRoleID, DWORD dwExpRewardParam, DWORD dwSilverRewardParam, DWORD dwTimeOrPoint );
	VOID UnsetState(EState eState);
};

class CGodMiracleMgr;

// 神迹
class CGodMiracle
{
public:
	CGodMiracle();
	~CGodMiracle();

	BOOL Init();
	void Uninit();
	void Refresh();

public:
	void  SetCurState(EGodMiracleState eCurState);
	EGodMiracleState GetCurState()						{ return m_eCurState;}
	void  SetGodID(DWORD dwGodID)
	{
		m_dwGodID = dwGodID;
		switch (m_dwGodID)
		{
		case EGM_NH:// 天神冢战场
			m_bGodMiracleOpen = g_world.IsTianShenZhongOpen();
			break;

		case EGM_XY:// 七花之海战场
			m_bGodMiracleOpen = g_world.IsQiHuaZhiHaiOpen();
			break;

		case EGM_SM:// 奔雷裂谷战场
			m_bGodMiracleOpen = g_world.IsBenLeiLieGuOpen();
			break;
		}
	}
	DWORD GetGodID()						{ return m_dwGodID;}
	DWORD GetCapGodID()						{ return m_dwCapGodMiracle;}
	DWORD GetCapStartTime()					{ return m_dwCapStartTime ;}
	void  SetApplyID(DWORD dwApplyID)		{ m_dwAttackGuild = dwApplyID;}
	DWORD GetApplyID()						{ return m_dwAttackGuild;}  
	void  SetApplyMoney(DWORD dwApplyMoney) { m_dwAttackBidFund = dwApplyMoney;}
	DWORD GetApplyMoney()					{ return m_dwAttackBidFund;}
	void  ModPoint(DWORD dwDAndCFlag, DWORD dwPoint);

	DWORD GetPoint(DWORD dwDAndCFlag)
	{
		// 守方
		if ( 1 == dwDAndCFlag) return m_dwDefendPoint;
		// 攻方
	    if ( 2 == dwDAndCFlag)  return m_dwCapturePoint;

		return 0;
	
	}

	void  SetCapGodID(DWORD dwCapID)		{ m_dwCapGodMiracle = dwCapID;}
	void  SetCapStartTime(DWORD dwStartToime){ m_dwCapStartTime = dwStartToime;}


	tagGodRoleInfo* GetGodRoleInfo(DWORD dwDefendOrCaptureFlag, DWORD dwRoleID);
	tagGodRoleInfo* GetGodRoleInfo(DWORD dwRoleID);

	BOOL	PushGodRoleInfo(DWORD dwDefendOrCaptureFlag, DWORD dwRoleID);

	BOOL	RemoveGodRoleInfo(DWORD dwDefendOrCaptureFlag, DWORD dwRoleID);

	// 得到一个击杀次数最多的玩家
	tagGodRoleInfo*	GetNextMaxKillTimesPlayer(DWORD dwDefendOrCaptureFlag);

	// 当人数变化时，向所有玩家发送当前的对战情况
	VOID SendCurDAndCInfoToAllGodplayer();

public:
	// 玩家申请争夺神迹
	DWORD HandleApplyCapGodMiracle(DWORD dwRoleID, DWORD& dwMoney, BOOL bAddBid);
	
	// 得到神迹的状态(依据时间+帮派占领情况)
	EGodMiracleState GetGodMiracleState();
	
	// 每分钟更新	
	VOID UpdateMin();
	
	// 适时更新
	VOID Update();

	// 计算帮派所有玩家的贡献和功勋(1 = 守方赢 2--守方输)
	VOID CalAllRoleContributionAndExploit(DWORD dwWinOrLoseType); 

	// 将帮派占领信息保存到数据库
	VOID SaveCapTureInfoToActive();

	// 得到最大的积分玩家
	tagGodRoleInfo* GetMaxScoresPlayer(EGuildCapType eGuidType);

	// 得到积分超过某一数值的玩家
	BOOL GetSomeplayerOverFixedScores(std::vector<tagGuildMaxScoresPlayer> &lstMaxScoresPlayer);
	
	// 从全局缓冲表中,拿到将要缓冲的玩家
	BOOL GetAllCachePlayer();

	// 判断所有的帮派成员是否在神迹区域并触发一次事件
	VOID CheckAllGuildMemberIsInGodArea();

	// 向争夺帮派所有成员发送更新神迹状态信息
	VOID BroadcastAllGoodPlayer();

	static BOOL IsNeedReward(EBattleFieldRewardType eBattleFieldRewardType, DWORD dwPointOrTime, DWORD dwLastPointOrTime, DWORD& dwExpReward, DWORD& dwSilverReward);
	VOID GivePointReward(DWORD dwDAndCFlag, DWORD dwPoint, DWORD dwLastPoint);
	VOID GiveGuildMemberReward(DWORD dwGuildID, DWORD dwExpRewardParam, DWORD dwSilverRewardParam, DWORD dwTimeOrPoint);

	typedef stdext::hash_map<DWORD,tagGodRoleInfo>	MAPROLEINFO;
	typedef TMap<DWORD, tagAllianceGuildInfo*>	AllicanceGuildMap;
protected:
	// 向胜负方广播每位参战者信息
	VOID BroadcastGodCaptureResultToAllPlayer(DWORD dwWinOrLoseType);
	void AddWinnerBuff( Role* pRole );
	VOID LogBattleField();
#if 0
	BOOL CalDefendContributionAndExploit(DWORD dwWinOrLoseType);
	BOOL CalCaptureContributionAndExploit(DWORD dwWinOrLoseType);
#endif

private:
	BOOL IsBidEnough(DWORD dwMoney);
	VOID ResetSignUpList(DWORD dwGuildID, DWORD dwMoney);
	VOID GiveMoneyBack2Guild(DWORD dwGuildID, DWORD dwMoney, EReturnMoneyReason eReturnMoneyReason);
	VOID ConfirmSignUp();

public:
	VOID GiveMoneyBack2FailedGuild();
	INT32 GetFirstMiracleIdxInSignUpList(DWORD dwGuildID, DWORD& dwBidFund);
	DWORD CanGuildDismiss(DWORD dwGuildID);
	DWORD Add2AllianceGuildMap(	DWORD			dwGuildID,				// 帮派ID
								EGuildCapType	eAllianceForceType,		// 防守或进攻
								BOOL			bConfirmed,				// 是否被确认了
							    BOOL			bSave2DB = TRUE);
	DWORD ConfirmAllianceGuild(DWORD dwGuildID);

	VOID ClearAllianceGuildMap();

	VOID SendAllianceList2Client(EGodMiracle eGodID, Role* pRole);

	DWORD Add2SignUpGuildInfo(DWORD dwGuildID, DWORD dwBidFund);

	AllicanceGuildMap& GetAllianceGuildMap()
	{
		return m_mapAllianceGuild;
	}

	EGuildCapType GetAllianceCapType(DWORD dwGuildID);
	EGuildCapType GetCapType(DWORD dwGuildID, BOOL &bAlliance );

	VOID SendAllianceGuildMsg(DWORD	dwMsgInfoID, DWORD	dwAllianceType);

	VOID UnsetStateOfAllRoleInBattleField(EGuildCapType eGuildCapType, EState eState);
	BOOL IsInAllianceGuild(DWORD dwGuildID);
	VOID SendJoinBattleMsg(Guild* pGuild, EBroadcastType eBroadcastType = EBT_Attack);

	BOOL GetAllianceConfirmed(DWORD dwGuildID);
private:
	DWORD m_dwGodID;				// 神迹ID
	DWORD m_dwCapGodMiracle;		// 占领神迹的帮派ID
	DWORD m_dwCapStartTime;			// 占领帮派的开始时间 
	DWORD m_dwAttackGuild;			// 获得攻击权的帮派ID
	DWORD m_dwAttackBidFund;		// 获得攻击权的帮派出的钱
	
	DWORD m_dwDefendPoint;			// 防守方积分
	DWORD m_dwCapturePoint;			// 进攻防积分
	BOOL  m_PointChangeFlag;		// 积分改变标志  

	EGodMiracleState m_eCurState;				// 当前状态

	tagSignUpGuildInfo		m_SignUpGuildInfo[MAX_COMPETE_BID];	// 公会竞价报名信息
	AllicanceGuildMap		m_mapAllianceGuild;					// 同盟公会报名信息

	// 攻防和守方的角色列表
	MAPROLEINFO						m_mapDefendGodRoleInfo;
	MAPROLEINFO						m_mapCaptureGodRoleInfo;

	BOOL m_bGodMiracleOpen;
};


// 神迹管理器
class CGodMiracleMgr
{
friend class CGodMiracle;

public:
	CGodMiracleMgr();
	~CGodMiracleMgr();
	
	BOOL Init();
	void Uninit() {};
	
	static CGodMiracleMgr* GetInst();

	// 从指定活动加载所有神迹信息
	BOOL LoadGodMiracleInfoFormActivity(CActivity* pActivity);

	// 复活更新
	VOID UpdateRevie();

	// 每分钟更新
	VOID UpdateMin();

	// 适时更新
	VOID Update();

public:
	// 查看某帮派占有几个神迹
	DWORD GetGuildDefendGodMiracle(DWORD dwGuildID);

	// 查看某公会取得了几个战场的进攻权
	DWORD GetGuildAttackGodMiracle(DWORD dwGuildID);

	// 得到神迹
	CGodMiracle* GetGodMiracle(EGodMiracle eGodType);

	// 得到玩家的击杀次数
	 DWORD GetOnePleyerKillTimes(DWORD dwRoleID);

	// 得到玩家的活跃时
	 DWORD GetonePlyerActiveTime(DWORD dwRoleID);
	
	 // 得到参与各神迹的帮派的最优秀玩家(积分最高的)
	 tagGodRoleInfo* GetCaptureGuildMaxScoresPlayer(DWORD dwGodID, EGuildCapType eGuidType);

	 BOOL GetGuildMaxScoresPlayer(std::vector<tagGuildMaxScoresPlayer> &lstMaxScoresPlayer);

	// 增加复活角色
	BOOL AddRevieRole(DWORD dwRoleID);

	// 复活
	VOID ReviveRole(DWORD dwRoleID);
	
	// 判断俩玩家是否在不同的帮派并争夺同一神迹
	BOOL IsCaptureGodAndDifferentGuild(Role* pRole, Role* pAnotherRole);
	
	// 判断玩家是否在争夺神迹帮派内
	BOOL IsInCaptureGodMiracleGuild(Role* pRole);

	// 查看当前角色的帮派是否在争夺神迹的帮派的范围内
	CGodMiracle* IsInCaptureGodRange(DWORD dwGuildID);

	// 广播所有争夺中优秀的玩家
	VOID BroadcastAllGoodPlayer();

	/// 设置神迹状态
	VOID SetGodDefendAndCaptureState(int nGodIndex);

	/// 角色击杀对于组队的处理
	VOID ModteamInfoForkillRole(DWORD dwGodID, DWORD dwSrcID, DWORD dwTargetID);

// Dealwith Msg
	// 返回所有神迹的信息
	DWORD HandleSendAllGodInfoToPlayer(DWORD dwRoleID);

	// 向争夺帮派所有成员发送更新神迹状态信息
	DWORD UpdateAllGodInfoToAllPlayer();

	BOOL IsGodMiracleTime();
	BOOL IsAllianceSignUpTime();
	DWORD CanGuildDismiss(DWORD dwGuildID);

	EGodMiracle GetEGodMiracle(DWORD dwMapID);

	DWORD Add2AllianceGuildMap(	EGodMiracle		eGodID,					// 战场ID
								DWORD			dwGuildID,				// 帮派ID
								EGuildCapType	eAllianceForceType,		// 防守或进攻
								BOOL			bConfirmed);			// 是否被确认了

	DWORD ConfirmAllianceGuild(	EGodMiracle		eGodID,					// 战场ID
								DWORD			dwGuildID);				// 帮派ID

	VOID ClearAllianceGuildMap(EGodMiracle eGodID);

	VOID SendAllianceList2Client(EGodMiracle eGodID, Role* pRole);

	// 载入竞价报名和同盟公会报名
	VOID LoadSignUpInfo();

	// 报名确认
	VOID ConfirmBidFundSignUp();

	// 判断两个公会是不是敌对关系
	BOOL IsGuildInSameAlliance(DWORD dwMapID, DWORD dwGuildID1, DWORD dwGuildID2);

	// 占领战场公会的公会资金每日22:00额外产出
	VOID AddCapGuildFundEveryday();

	VOID ResetMergedData()
	{
		m_bHaveMergedData = FALSE;
		m_listMergedCapGuildID.Clear();
	}

public:
	typedef TMap<DWORD, tagRevieRoleInfo*>			MAPREVIEROLEINFO;
private:
	CGodMiracle	m_VecGodmiracle[EGM_MAX];
	DWORD m_dwBattleFieldMapID[EGM_MAX];
	BOOL m_bHaveMergedData;		// 是否有合服工具合并的活动数据
	TList<DWORD> m_listMergedCapGuildID;
	Mutex					m_mutexRevive;	
	MAPREVIEROLEINFO		m_mapRevieRole;

	BOOL	m_bBidFundSignUpConfirm;	// 竞价报名确认进攻公会是不是已经确认过了
	BOOL	m_bAddCapGuildFund;	// 占领战场公会的公会资金每日22:00额外产出
};