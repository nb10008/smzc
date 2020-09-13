//-----------------------------------------------------------------------------
//!\file World.h
//!\author Aslan
//!
//!\date 2008-06-10
//! last 2008-06-10
//!
//!\brief 区域服务器主框架类
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "mutex.h"

//初始化步骤
enum EWorldInitStep
{
	EWI_ScriptMgr = 0,
	EWI_AttRes,
	EWI_GuildMgr,
	EWI_SocialMgr,
	EWI_Mall,
	EWI_TradeYB,
	EWI_GroupMgr,
	EWI_FameHall,
	EWI_NPCTeamMgr,
	EWI_VIPStall,
	EWI_MapCreator,
	EWI_WorldNetCmdMgr,
	EWI_RoleMgr,
	EWI_QuestMgr,
	EWI_MsgInfoMgr,
	EWI_VipNetBarMgr,
	EWI_LootMgr,
	EWI_CityMgr,
	EWI_WorldSession,
	EWI_DBSession,
	EWI_LoginSession,
	EWI_BillSession,
	EWI_RtSession,
	EWI_PvPAthleticsSystem,
	EWI_KEYCODEBRIDGESESSION,
	EWI_IPGSession,
	EWI_IPGCashSession,
	EWI_InternetCafeSession,
};
//-----------------------------------------------------------------------------
// 区域服务器主框架类
//-----------------------------------------------------------------------------
class World
{

public:
	World();
	~World();

	BOOL Init(HINSTANCE hInst);
	VOID Mainloop();
	VOID Destroy();

	BOOL	IsOverload() const			{ return m_bOverloadAlert; }
	DWORD	GetMaxTickTime() const		{ return m_dwMaxTickTime; }
	DWORD	GetIdleTime() const			{ return m_dwIdleTime; }
	DWORD	GetTotalRunMinute() const	{ return m_dwTotalRunMinute; }
	BYTE	GetSaveOneTicks() const		{ return m_bySaveOneNeedTicks; }
	DWORD	GetGoldenCode() const		{ return m_dwGoldenCode; }
	LPCTSTR	GetWorldName() const		{ return m_strWorldName.c_str(); }
	INT		GetCpuNum()	const			{ return m_nCpuNum; }
	DWORD	GetTotalMemory() const		{ return m_dwTotalPhys; }
	DWORD	GetAvailMemory() const		{ return m_dwAvailPhys; }
	INT		GetQuotaNonPagedPoolUsage()	{ return m_nQuotaNonPagedPoolUsage; }

	VOID	ShutDown()					{ InterlockedExchange((LONG*)&m_bShutingdown, TRUE); }
	BOOL	IsShutingdown() const		{ return m_bShutingdown; }

	VOID	SwitchGameGuarder(BOOL bOn);
	BOOL	IsWell() const;
	VOID	UpdateMemoryUsage();	

	DWORD	GetWorldTick()				{ return m_dwWorldTick; }
	VOID	SetWorldTickInc()			{ InterlockedExchangeAdd((LPLONG)&m_dwWorldTick, 1); }

	DWORD	GetMaxMsgSize()				{ return m_dwMaxMsgSize; }
	VOID	SetMaxMsgSize(DWORD dwMsgSize)	{ InterlockedExchange((LONG*)&m_dwMaxMsgSize, dwMsgSize); }

	tagDWORDTime GetWorldTime()			{ return m_CurrentTime; }
	VOID	UpdateWorldTime()			{ m_CurrentTime = GetCurrentDWORDTime(); }

	VOID	LockNetWork()				{ m_MutexNetwork.Acquire(); }
	VOID	UnlockNetWork()				{ m_MutexNetwork.Release(); }


	BOOL FirstInit(HINSTANCE hInst);

	// Jason 2010-4-19 v2.0.0 法宝开关
	BOOL	IsFabaoOpen(VOID);

	// Jason 2010-4-26 v2.1.0 pk逃逸延迟时间
	INT		GetPKRunawayTickCount(VOID);

	// Jason 2010-5-18 v2.1.2飞升标志
	BOOL	IsSoaringOpen(VOID);

	INT		GetLoginTimeOutTicks(VOID);

	BOOL	IsPrintRoleInfoOpen(VOID);

	BOOL	IsTianShenZhongOpen();
	BOOL	IsQiHuaZhiHaiOpen();
	BOOL	IsBenLeiLieGuOpen();
	BOOL	IsKillCounterOpen();

	INT		GetMaxRoleLevel(VOID);
	//元神是否开放
	bool  IsHolySoulOpen(void);
	LPTSTR CreateLogAnalyzeFileName();
public:
	FLOAT	GetLaxCheatDistanceSQ()		{ return m_fLaxCheatDistanceSQ; }
	FLOAT	GetStrictCheatDistanceSQ()	{ return m_fStrictCheatDistanceSQ; }

	TObjRef<Util>&		GetUtil()			{ return m_pUtil; }
	TObjRef<FastCode>&	GetFastCode()		{ return m_pFastCode; }
	TObjRef<Log>&		GetLog()			{ return m_pLog; }
	TObjRef<Log>&		GetLogNetAnalyze()	{ return m_pLogNetAnalyze; }

public:
	// crc32方法封装 -- 先转换为小写再计算
	DWORD	LowerCrc32(LPCTSTR str);

private:
	// 保存所有游戏世界其他数据
	VOID	SaveWorldInfoToDB();
	VOID	WaitDBTerminate();
private:
	TSFPTrunk<World>		m_Trunk;

	TObjRef<Thread>			m_pThread;
	TObjRef<GDIRender>		m_pRender;
	TObjRef<WindowWrap>		m_pWindowWrap;
	TObjRef<GUISystem>		m_pGUI;
	TObjRef<Util>			m_pUtil;
	TObjRef<FastCode>		m_pFastCode;
	TObjRef<Console>		m_pConsole;
	TObjRef<Log>			m_pLog;
	TObjRef<Log>			m_pLogNetAnalyze;

	IGUIRenderNative*		m_pGUIRender;

	volatile BOOL			m_bShutingdown;						// 服务器是否正在关闭
	volatile BOOL			m_bTerminateUpdate;					// 服务器线程停止位
	volatile BOOL			m_bOverloadAlert;					// 服务器超载警报
	volatile DWORD			m_dwIdleTime;						// 上次服务器主循环空闲时间
	volatile DWORD			m_dwMaxTickTime;					// 最大的Tick时间
	volatile BYTE			m_bySaveOneNeedTicks;				// 多少Tick保存一个玩家

	volatile DWORD			m_dwMaxMsgSize;

	DWORD					m_dwViewWidth;
	DWORD					m_dwViewHeight;

	INT						m_nCpuNum;
	DWORD					m_dwTotalPhys;
	DWORD					m_dwAvailPhys;
	INT						m_nQuotaNonPagedPoolUsage;

	tstring					m_strWorldName;						// 游戏世界名称
	DWORD					m_dwGoldenCode;						// 区域服务器的golden_code

	DWORD					m_dwLastMinute;
	DWORD					m_dwKoreaLastTime;					//韩国测试计时
	DWORD					m_dwLastHour;
	DWORD					m_dwTotalRunMinute;
	volatile DWORD			m_dwWorldTick;						// 游戏世界心跳
	tagDWORDTime			m_CurrentTime;						// 当前系统时间
	DWORD					m_dwLastSaveTick;					// 上一次保存玩家记录心跳

	FLOAT					m_fLaxCheatDistanceSQ;				// 弱检测情况下的距离平方
	FLOAT					m_fStrictCheatDistanceSQ;			// 强检测情况下的距离平方

	Mutex					m_MutexNetwork;						// 网络层锁，用于与底层的logincallback和logoutcallback进行互斥

private:
	// Update线程
	DWORD ThreadUpdate(LPVOID);
	// 记录服务器主线调用堆栈信息
//	DWORD RecordStack(LPCSTR);
	// 修改多少个tick保存一个玩家数据
	DWORD ChangeSaveTicks(LPCSTR szTicks);
	// 每分钟执行一次
	VOID OnMinute();
	// 每小时执行一次
	VOID OnHour();
	// 每整点执行一次
	VOID OnClock(BYTE byHour);
	// 保存玩家信息
	VOID SaveRolePerTick();
	//打印信息
	VOID OutPutInitStepInfo(EWorldInitStep stp, BOOL bSuccess, DWORD dwPara1 = GT_INVALID, DWORD dwPara2 = GT_INVALID);

	// Jason 2010-4-19 v2.0.0 法宝开关
	VOLATILE LONG m_lFabaoOpen;
	volatile LONG m_lWillShutdown;

	volatile LONG m_lWGCheck_On;

	CHAR		m_strConfigWanIP[X_IP_LEN];		// 配置的网管ip
	UINT16		m_n16ConfigWanPort;	// 配置的外网映射端口

	VOLATILE LONG m_lPrintRoleInfo;

public:
	VOID WatchKeyValue( LPCTSTR szFind, LPCTSTR key, DWORD val );
	BOOL	IsFairyContractOpen(VOID) const;

	BOOL    IsWGCheck_OnOpen(VOID)const;

	// Jason 2010-9-7 神铸开工以及参数
	BOOL	IsGodStrengthenOpen(VOID) const;
	// 神铸装备最低品级
	INT		GetGodStrengthenEquipMinQlty(VOID) const;
	// 神铸冷却时间
	DWORD	GetGodStrengthenCoolDownTime(VOID) CONST;
	// 神铸辅助装备最低品级
	INT		GetGodStrengthenAssitEquipMinQlty(VOID) CONST;
	// 神铸辅助装备最低等级
	INT		GetGodStengthenAssitEquipMinLevel(VOID) CONST;

	BOOL	IsWillShutdown() CONST;
	VOID	SetWillShutdownFlat(VOID);

	INT		GetFabaoLevelLimit(VOID) CONST;

	BOOL	GetDBReconnectFlag(VOID) CONST;

	BOOL	GetFabaoFoodQualifFlag(VOID) CONST;
	BOOL	GetFabaoElemStuntFlag(VOID) CONST;
	tagDWORDTime GetOldSerNewPlayerDate() const;
	DWORD GetOldSerNewPlayerMaxLevel(VOID) const;

	BOOL	IsUsingConfigWanIP() const;
	LPCSTR		GetConfigWanIP() const{ return m_strConfigWanIP; }
	UINT16		GetConfigWanPort () const { return m_n16ConfigWanPort; }

	BOOL	IsFairyMatingOpen() const ;

	DWORD	GetCurTick() const;

	BOOL	IsOverseasVersionShieldingOpen() const;
	BOOL	IsEggBrokerOpen() const;
	BOOL 	IsPlayerBack() const;

	VOID	InputConsoleCmd(LPCTSTR strCmd);
	VOID	CountNetMsg(LPVOID pMsg,DWORD dwSize);
	BOOL	IsPkConstraintsOpen() const;
	INT		GetPKMiniLevel() const;
	// 为湾湾做的pk限制约束等
	

	BOOL	IsGodSystemOpen(VOID) CONST;

	INT		GetDeductMorality() const;

	INT		GetMoralityTime() const;

	INT		GetReturnMorality() const;

	INT		GetDistance() const;

	void __fastcall GetMAC(string& strMAC);

	CHAR*	GetIP(VOID) const;

	CHAR*	GetMAC(VOID) const;

	CHAR*	GetMD5(VOID) const;

	CHAR*	GetSign(VOID) const;

	BOOL	IsOneSign(CHAR *pszSign);

	BOOL	IsUseIPGSystem() const;

	DWORD	GetGetCashPort() const;

	DWORD	GetIOCashPort() const;

	CHAR*	GetIPGIP() const;

private:
	volatile LONG m_LPuperStars[10];//存1颗紫星-10颗紫星的装备数量
public:
	void SetPuperStarsNum(BYTE n);	//参数表示几颗紫星
	void OutputStarsLog();

public:
	HANDLE	m_hKoreaLogFile;

	VOID	WriteKroeaLog(TCHAR *ptszLog);

	//元神强化系统
	//冲星强化
	bool	IsStarEnhanceOpent()const;

};

extern World g_world;

#define	IUTIL		(g_world.GetUtil())
#define IFASTCODE	(g_world.GetFastCode())
#define ILOG		(g_world.GetLog())
#define ILOGNETANALYZE		(g_world.GetLogNetAnalyze())
