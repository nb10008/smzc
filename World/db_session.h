//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: db_session.h
// author: Sxg
// actor:
// data: 2008-5-19
// last:
// brief: 游戏服务器与数据库服务器的对话层
//-----------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"

#include "..\WorldDefine\base_define.h"
#include "..\ServerDefine\role_data.h"

// Jason [v1.3.1] 外部链接
//#include "external_links.h"
#include "external_links_define.h"

class ExternalLinks;


//初始化步骤
enum EDBSessionInitStep
{
	EDBS_ItemInfo = 0,
	EDBS_ItemNeedLog,
	EDBS_LoadAllRoleInfo,
	EDBS_LoadAllYBAccount,
	EDBS_LoadAllOrder,
	EDBS_LoadAllGuild,
	EDBS_LoadAllGuildMember,

	EDBS_LoadGuildUpgradeInfo,	//设施
	EDBS_LoadGuildSkillIInfo,	//技能
	EDBS_LoadGuildCommerceInfo,	//跑商
	EDBS_LoadGuildWareItems,	//仓库

	EDBS_InitCity,
	EDBS_LoadAllGPInfo,
	EDBS_LoadAllVIPStallInfo,
	EDBS_LoadActivityData,
	EDBS_LoadRanking,
	EDBS_LoadGetVNBData,
	EDBS_ExternalLinksList,
	EDBS_LoadAllLimitNumItemInfo,
	EDBS_InitOk,
};


//-----------------------------------------------------------------------------
class DBSession
{
public:
	DBSession();
	~DBSession();

public:
	BOOL Init();
	VOID Destroy();

	VOID Update();
	BOOL IsWell() { return (m_bInitOK && m_lTransCanUse && P_VALID(m_pTran) && m_pTran->IsConnected()); }

	VOID Send(LPVOID pMsg, DWORD dwMsgSize);	

	LPBYTE Recv(DWORD& dwMsgSize) { if(m_lTransCanUse && P_VALID(m_pTran)) return m_pTran->Recv( dwMsgSize ); return NULL; }

	// 清除收到的网络消息
	VOID FreeRecved(LPVOID pMsg) { if( m_lTransCanUse && P_VALID(m_pTran) ) m_pTran->FreeRecved((LPBYTE)pMsg); }

	INT	GetUnsendPackageNum() { if(m_lTransCanUse && P_VALID(m_pTran)) return m_pTran->GetUnsendPackageNum(); return -1;}
	INT	GetReceivedPackageNum() { if(m_lTransCanUse && P_VALID(m_pTran)) return m_pTran->GetReceivedPackageNum(); return -1; }
	INT	GetToDBConnected() { if(m_lTransCanUse && P_VALID(m_pTran)) return (int)m_pTran->IsConnected();return 0; }

private:
	// 读取文件, 初始化成员
	BOOL InitConfig();

	// 注册所有的网络命令
	VOID RegisterAllDBCommand();

private:
	// 连接数据库服务器
	DWORD ThreadConnectDB();

private:
	// 认证及心跳
	DWORD HandleCertification(tagNetCmd* pMsg, DWORD);
	DWORD HandleHeartBeat(tagNetCmd* pMsg, DWORD);

	// 初始化消息
	DWORD HandleLoongWorldInitOK(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadAllRoleInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleItemInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleItemNeedLog(tagNetCmd* pMsg, DWORD);

	// 人物创建、删除和读取
	DWORD HandleRoleEnum(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleCreate(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleDelete(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleResume(tagNetCmd* pMsg, DWORD) { return 0; }
	DWORD HandleRoleLoad(tagNetCmd* pMsg, DWORD);

	// 向百宝袋放入新的物品
	DWORD HandleBaiBaoLoad(tagNetCmd* pMsg, DWORD);
	// 更新百宝袋中的元宝
	DWORD HandleBaiBaoYuanBaoLoad(tagNetCmd* pMsg, DWORD);

	// 元宝交易相关
	DWORD HandleLoadAllYBAccount(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadAllYBOrder(tagNetCmd* pMsg, DWORD);
	DWORD HandleRoleGetYBOrder(tagNetCmd* pMsg, DWORD);

	// 百宝袋历史记录相关
	DWORD HandleBaiBaoLoadLog(tagNetCmd* pMsg, DWORD);

	// 名人堂
	DWORD HandleRepRankLoad(tagNetCmd* pMsg, DWORD);
	DWORD HandleGetActTreasureList(tagNetCmd* pMsg, DWORD);
	DWORD HandleRepRstTimeStamp(tagNetCmd* pMsg, DWORD);
	DWORD HandleGetFameHallEnterSnap(tagNetCmd* pMsg, DWORD);

	// 帮派相关
	DWORD HandleLoadAllEmptyGuild(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuild(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildMember(tagNetCmd* pMsg, DWORD);
	DWORD HandleGuildMemberInitOk(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildWareItems(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildUpgradeInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildSkillInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadGuildCommerceInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadCommerceRankInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleGuildInitOK(tagNetCmd* pMsg, DWORD);
	DWORD HandleCreateGuild(tagNetCmd* pMsg, DWORD);
public:
	DWORD HandleInsertGuildName(tagNetCmd* pMsg, DWORD);
private:

	// 宠物
	DWORD HandleCreatePetSoul(tagNetCmd* pMsg, DWORD);
	
	// 商城相关
	DWORD HandleLoadAllGPInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadLimitItemInfo(tagNetCmd* pMsg, DWORD);
	

	// 角色名贴
	DWORD HandleLoadRoleVCard(tagNetCmd* pMsg, DWORD);

	// VIP摊位
	DWORD HandleLoadVIPStallInfo(tagNetCmd* pMsg, DWORD);

	// 固定活动
	DWORD HandleLoadActivityData(tagNetCmd* pMsg, DWORD);

	// VIP网吧
	DWORD HandleLoadVNBData(tagNetCmd* pMsg, DWORD);

	// 实力排行榜相关
	DWORD HandleInitRankings(tagNetCmd* pMsg, DWORD);
	DWORD HandleReloadRankings(tagNetCmd* pMsg, DWORD);
	DWORD HandleInitPetRankings(tagNetCmd* pMsg, DWORD);
	DWORD HandleInitFlowerRankings(tagNetCmd* pMsg, DWORD);
	DWORD HandleRetUpdateRankings(tagNetCmd* pMsg, DWORD);
	DWORD HandleRenewAdvanceStrength(tagNetCmd* pMsg, DWORD);
	DWORD HandleSyncCanRankFlag(tagNetCmd* pMsg, DWORD);

	// 神职竞选相关
	//DWORD HandleAsk4NewClergy(tagNetCmd* pMsg, DWORD);
	DWORD HandleInitClergyRank(tagNetCmd* pMsg, DWORD);
	DWORD HandleRenewClergyCandidateType(tagNetCmd* pMsg, DWORD);
	DWORD HandleGetClergyCandidateCount(tagNetCmd* pMsg, DWORD);
	DWORD HandleClearAllCompetitiveClergy(tagNetCmd* pMsg, DWORD);

	// 读取留言
	DWORD HandleLoadLeftMsg(tagNetCmd* pMsg, DWORD);
	
	// 城市系统
	DWORD HandleLoadCityInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleGetGuildClanValue(tagNetCmd* pMsg, DWORD);

	// 师徒
	DWORD HandleLoadMasterApprenticeData(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadForceBreakOutApprenticeList(tagNetCmd* pMsg, DWORD);

	// 选秀
    DWORD HandleLoadShowPlayerData(tagNetCmd* pMsg, DWORD);
    DWORD HandleLoadVotePlayerData(tagNetCmd* pMsg, DWORD);

	// 活动排行
    DWORD HandleLoadActiveRankData(tagNetCmd* pMsg, DWORD);
	// 注意该函数是需要修改g_roleMgr中的数据的，正好该协议处理器是在threadupdate中做，不是在地图线程，因此可以安全的用
	DWORD HandleLoadOneRoleInfo(tagNetCmd* pMsg, DWORD);

	// 稀有物品
	DWORD HandleLoadGlobalLimitItem(tagNetCmd* pMsg, DWORD);

	//DWORD HandleLoadRareArea(tagNetCmd* pMsg, DWORD);
	// 家族妖精
	DWORD HandleLoadFamilySprite(tagNetCmd* pMsg, DWORD);
	// 家族任务
	DWORD HandleLoadFamilyQuest(tagNetCmd* pMsg, DWORD);
	
	DWORD HandleLoadAllFamily(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadFamilyMember(tagNetCmd* pMsg, DWORD);
	DWORD HandleFamilyMemberInitOK(tagNetCmd* pMsg, DWORD);
	DWORD HandleFamilyInitOK(tagNetCmd* pMsg, DWORD);

	DWORD HandleLoadWorldData(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadRoleLogData(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadRoleTigerProfit(tagNetCmd* pMsg, DWORD);

	DWORD HandleAccountReactiveOrNot(tagNetCmd* pMsg, DWORD);
	DWORD HandleInitAccountReactiveInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandelKeyCodeAddGift(tagNetCmd* pMsg, DWORD);
	DWORD HandleNDBS_ReceiveYuanBao(tagNetCmd* pMsg, DWORD);
private:
	TSFPTrunk<DBSession>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<StreamTransport>	m_pTran;
	TObjRef<Thread>				m_pThread;
	TObjRef<NetCmdMgr>			m_pMsgCmdMgr;

	// 连接参数
	CHAR						m_szDBIP[X_IP_LEN];		// 数据库服务器ip
	DWORD						m_dwDBPort;				// 数据库服务器port

	// 初次连接后，需发送的数据
	DWORD						m_dwGoldenCode;					// 数据库服务器金色代码
	//TCHAR						m_szServerName[X_LONG_STRING];	// 运行游戏世界的机器名称

	volatile BOOL				m_bTermConnect;					// 记录当前连接状态
	volatile BOOL				m_bInitOK;						// 是否初始化完成

	DWORD						m_dwInitTime;					//从db初始化所有数据所耗时间 毫秒

	// Jason v[1.3.1] 外部链接
	TSafeList<tagSExternalLink>	m_lstExterLinks;
	ExternalLinks		*	m_elExternalLinks;
	tagDWORDTime			m_timLastPullExtLink;

	VOID RefreshExternalLinksList(tagNetCmd * pCmd);
	DWORD HandleExternalLinksList(tagNetCmd* pMsg, DWORD);
 
	VOID UpdateExternalLinksList(VOID);

	VOID InitialExternalLinksList(VOID);

	//打印信息
	VOID OutPutInitStepInfo(EDBSessionInitStep stp, BOOL bSuccess, DWORD dwPara1 = GT_INVALID, DWORD dwPara2 = GT_INVALID);

	// Jason 2010-1-27 v1.3.2 玩家回归/离线奖励相关
	DWORD HandleGetAccountInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleLoadShortCut_QuickBar(tagNetCmd* pMsg, DWORD);
	DWORD HandleIsSavedShortCut_QuickBar(tagNetCmd* pMsg, DWORD);

	DWORD HandleGetSerialVal(tagNetCmd* pMsg, DWORD);
	
	BOOL HandleCheckName(tagNetCmd* pMsg, DWORD);

	// 仇敌上线，通知玩家
	BOOL HandleGetRoleListOfEnemy(tagNetCmd* pMsg, DWORD);
	
	// 载入数据库中的公会宣战关系
	BOOL HandleLoadGuildWarDeclare(tagNetCmd* pMsg, DWORD);

	// 给角色添加天生技能
	VOID AddInitialSkill2Role(DWORD dwRoleID);

	// 战场
	BOOL HandleLoadBattleFieldAttackSignUpInfo(tagNetCmd* pMsg, DWORD);
	BOOL HandleLoadAllicanceGuildInfo(tagNetCmd* pMsg, DWORD);

	// Jason
	DWORD	m_dwWaitConnectionTick;
	DWORD	m_dwTransDBReconnectTicks;
	BOOL ReconnectDBServer(VOID);

	// 得到妖精重生数据
	DWORD HandleGetFabaoNirVanaData(tagNetCmd* pMsg, DWORD);

	// 神之竞标
	DWORD HandleLoadGodBidInfo(tagNetCmd* pMsg, DWORD);
	DWORD HandleClearGodBidInfo(tagNetCmd* pMsg, DWORD);
	
	volatile LONG m_lTransCanUse;

	INT32		m_nItemNLItemNum;
	DWORD		m_dwItemNLdwTypeID[MAX_ITEM_NEEDLOG_NUM];
public:
	VOID SendMsgToPlayerForExtLinks(Role * ply);

	VOID ReInitItemNeedLog(VOID);

	//删除角色领域
	VOID			SendDeleteArea2DB( DWORD dwRoleID,DWORD dwAreaID );

	//增加角色领域
	VOID			SendCreateArea2DB( DWORD dwRoleID, DWORD dwAreaID, BOOL bActivation );

	//删除角色技能
	VOID			SendRemoveSkill( DWORD dwRoleID, DWORD dwSkillID );

	//删除角色一个buff
	VOID			SendRemoveOneBuff( DWORD dwRoleID, DWORD dwBuffID );

	//给非在线角色取消领域能力
	VOID			RemoveAreaAbilityOffline( DWORD dwRoleID, DWORD dwAreaID );
};

extern DBSession g_dbSession;		// DB session全局对象
