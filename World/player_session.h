//-----------------------------------------------------------------------------
//!\file player_session.h
//!\author Aslan
//!
//!\date 2008-06-10
//! last 2008-06-10
//!
//!\brief 玩家连接代理类
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "player_net_cmd_mgr.h"
#include "../ServerDefine/base_define.h"
#include "../ServerDefine/role_data_define.h"
#include "../WorldDefine/msg_quickbar.h"
#include "Mutex.h"
#include "gm_net_cmd_mgr.h"
#include "fatigue_guarder.h"
#include "xtrap_guarder.h"
#include "db_session.h"
#include "../ServerDefine/role_data.h"

#ifdef ON_GAMEGUARD
#include "../GameGuard/ggsrv30.h"
#endif

class Creature;
class MotionScript;
struct tagQuickData;
enum EQuestBoardType;

const INT CON_LOST	=	-1000000;		// 连接断开标志
const INT RET_TRANS	=	-2000000;		// 需上层处理

// Jason 2010-1-15 v1.3.2 离线奖励
class OfflineReward;
class IPGDragonfly;

class PlayerSession
{
public:
	friend class DBSession;
	friend class WorldNetCmdMgr;

	PlayerSession(DWORD dwSessionID, DWORD dwInternalIndex, DWORD dwIP, BYTE byPrivilege,
				BOOL bGuard, DWORD dwAccOLSec, LPCSTR tszAccount,char *szKrUserID,tagDWORDTime dwPreLoginTime,
				DWORD dwPreLoginIP,DWORD dwChannelID, CHAR* szMac);
	~PlayerSession();

	//-----------------------------------------------------------------------------
	// 各种Get
	//-----------------------------------------------------------------------------

	tagDWORDTime GetPreLoginTime()	const	{ return m_dwPreLoginTime;}
	DWORD		 GetPreLoginIP()	const	{ return m_dwPreLoginIP;}
	DWORD		 GetCurLoginIP()	const	{ return m_dwIP;}

	LPCSTR		GetAccount()		const	{ return m_szAccount;	}
	LPCSTR		GetKrUserID()       const   { return m_szKrUserID;}
	DWORD		GetGetIP()			const	{ return m_dwIP; }
	DWORD		GetSessionID()		const	{ return m_dwAccountID; }
	INT			GetMsgWaitNum()		const	{ return m_nMsgNum; }
	DWORD		GetInternalIndex()	const	{ return m_dwInternalIndex; }
	Role*		GetRole()			const	{ return m_pRole; }
	Role*		GetOtherInMap( DWORD dwRoleID ) const ;
	const FatigueGuarder&	GetFatigueGuarder()	const { return m_FatigueGarder; }
	INT			GetVNBExpRate()		const;
	INT			GetVNBLootRate()		const;
	LPCTSTR		GetVNBName()		const;
	BOOL		IsKicked() const			{ return m_bKicked; }
	INT			GetDistributionID()		const	{ return m_nDistributionID; }

	// 帐号下角色通用属性相关操作
	const INT	GetBaiBaoYB()		const	{ return m_sAccountCommon.nBaiBaoYuanBao; }
	const INT64	GetWareSilver()		const	{ return m_sAccountCommon.n64WareSilver; }
	const INT16	GetWareSize()		const	{ return m_sAccountCommon.n16WareSize; }
	const tagDWPlayerPersonalSet	GetPersonalSet()		const	{ return m_sAccountCommon.stPersonalSet; }
	const DWORD GetLastUseRoleID()	const	{ return m_sAccountCommon.dwLastUseRoleID; }
	DWORD GetAccountOnlineTime() 
	{ 
		tagDWORDTime now = GetCurrentDWORDTime();
		return CalcTimeDiff(now, m_dwLoginTime); 
	}

	EAccountSpec GetAccountSpec() { return m_sAccountCommon.eSpec; }

	DWORD		GetLastRecvTime(DWORD dwID)	{ return m_mapLastRecvTime.Peek(dwID); }

	VOID GetMac(CHAR* szMac) { if(P_VALID(szMac)) memcpy(szMac, m_szMac, X_SHORT_STRING); };
	//----------------------------------------------------------------------------
	// 各种Set
	//----------------------------------------------------------------------------
	VOID		SetRole(Role* pRole)		{ m_pRole = pRole; }
	VOID		SetConnectionLost()			{ InterlockedExchange((LPLONG)(&m_bConnectionLost), TRUE); }
	VOID		SetKicked()					{ InterlockedExchange((LPLONG)(&m_bKicked), TRUE); }

	VOID		SetBaiBaoYB(INT nYuanBao)		{ m_sAccountCommon.nBaiBaoYuanBao	= nYuanBao; }
	VOID		SetWareSilver(INT64 n64Silver)	{ m_sAccountCommon.n64WareSilver	= n64Silver; }
	VOID		SetAccOLMin(DWORD dwState, DWORD dwAccOLMin)	{ m_FatigueGarder.SetAccOLTimeMin(dwState, dwAccOLMin);				}
	VOID		SetRoleLoading(BOOL bLoading=TRUE)				{ m_bRoleLoading = TRUE == bLoading;	}
	VOID		SessionLogout();
	VOID		SetAccountSpec(EAccountSpec espec) 
	{ 
		m_sAccountCommon.eSpec = espec;
		tagNDBC_SaveAccountSpec send;
		send.dwAccountID = m_dwAccountID;
		send.eSpec = espec;
		g_dbSession.Send(&send, send.dwSize);
	}
	VOID		SetLastRecvTime(DWORD dwID)	{ m_mapLastRecvTime.SetValue(dwID, timeGetTime()); }
	//----------------------------------------------------------------------------
	// 消息处理相关
	//----------------------------------------------------------------------------
	INT			HandleMessage();
	VOID		SendMessage(LPVOID pMsg, DWORD dwSize);
	VOID		BroadcastCurrChannel(LPCTSTR szMsg);

	//-----------------------------------------------------------------------------
	// 选人界面的判断
	//-----------------------------------------------------------------------------
	bool		IsRoleLoading()		const	{ return m_bRoleLoading; }
	bool		IsRoleEnuming()		const	{ return m_bRoleEnuming; }
	bool		IsRoleCreating()	const	{ return m_bRoleCreating; }
	bool		IsRoleDeleting()	const	{ return m_bRoleDeleting; }
	bool		IsInWorld()			const	{ return m_bRoleInWorld; }


	//-----------------------------------------------------------------------------
	// 角色通用属性相关
	//-----------------------------------------------------------------------------
	bool		IsHaveBagPsd()		const	{ return GetBagPsd() != GT_INVALID; }

	//-----------------------------------------------------------------------------
	// 角色相关
	//-----------------------------------------------------------------------------
	BOOL		FullLogin(Role* pRole, BOOL bFirst);
	VOID		LogoutPlayer();
	VOID		Refresh();

	//-----------------------------------------------------------------------------
	// 网络命令相关和GM命令
	//----------------------------------------------------------------------------
	static VOID RegisterAllPlayerCmd();
	static VOID RegisterALLSendCmd();
	static VOID UnRegisterALL();

	static VOID LogNetAnalyze();

	//-----------------------------------------------------------------------------
	// GM命令相关
	//-----------------------------------------------------------------------------
	BOOL		IsPrivilegeEnough(BYTE byPrivilege) const { return byPrivilege <= m_byPrivilege; }
	BYTE GetPriviliege()	{ return m_byPrivilege; }

	//-----------------------------------------------------------------------
	// 回收消息Msg(有用则再利用，无用则释放)
	//-----------------------------------------------------------------------
	VOID		RecycleMsg(LPBYTE pMsg);

	//-----------------------------------------------------------------------------
	// 更新
	//----------------------------------------------------------------------------
	INT			Update();

	VOID			SyncOnlineNum(Role* pRole, BOOL bLogout = FALSE);

	DWORD	       HandleRoleCreateFromLogin(tagNetCmd* pCmd);

private:
	//-----------------------------------------------------------------------
	// 需要在所有地图线程上层处理的消息注册
	//-----------------------------------------------------------------------
	static VOID	RegisterWorldMsg(LPCSTR szCmd, NETMSGHANDLER fp, LPCTSTR szDesc, DWORD dwSize, DWORD dwTimeInterval = 0);

	//-----------------------------------------------------------------------
	// 消息处理相关
	//-----------------------------------------------------------------------
	VOID			SendSmallMessage(LPVOID pMsg, DWORD dwSize);
	VOID			SendLargeMessage(LPVOID pMsg, DWORD dwSize);

	//-----------------------------------------------------------------------
	// 底层包相关
	//-----------------------------------------------------------------------
	LPBYTE			RecvMsg(DWORD& dwSize);
	VOID			ReturnMsg(LPBYTE pMsg);
	VOID			SendMsg(LPBYTE pMsg, DWORD dwSize);

	//-----------------------------------------------------------------------
	// 选人界面相关
	//-----------------------------------------------------------------------
	BOOL			IsRoleExist(const DWORD dwRoleID) const;
	BOOL			AddRole(const DWORD dwRoleID);
	BOOL			RemoveRole(const DWORD dwRoleID);
	BOOL			CanSetSafeCode();
	BOOL			CanResetSafeCode() const;
	BOOL			CanCancelSafeCodeReset() const;

	//-----------------------------------------------------------------------
	// 帐号下角色通用属性相关操作
	//-----------------------------------------------------------------------
	const DWORD		GetBagPsd()		const { return m_sAccountCommon.dwBagPsdCrc; }
	const DWORD		GetSafeCode()	const { return m_sAccountCommon.sSafeCode.dwSafeCodeCrc; }

	VOID			SetBagPsd(DWORD dwNewPsdCrc);

	// 给所有帮派成员发送在线人数变化消息
	VOID			SyncOnlineNum2AllGuildMumber(DWORD dwGuildID, BOOL bLogout = FALSE);

	EQuestBoardType GetQuestBoardType(DWORD dwNPCID);
	EQuestBoardType GetQuestBoardTypeByTypeID(DWORD dwNPCTypeID);

	/************************************************************************
	** Handlers -- map thread
	*************************************************************************/

	//-----------------------------------------------------------------------
	// 进入游戏
	//-----------------------------------------------------------------------
	DWORD	HandleJoinGame(tagNetCmd* pCmd);

	//-----------------------------------------------------------------------
	// 角色创建、删除、获取及选择
	//-----------------------------------------------------------------------
	DWORD	HandleRoleCreate(tagNetCmd* pCmd);
	DWORD	HandleRoleEnum(tagNetCmd* pCmd);
	DWORD	HandleRoleDelete(tagNetCmd* pCmd);
	DWORD	HandleRoleSelect(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 安全码处理
	//------------------------------------------------------------------------
	DWORD	HandleRoleSetSafeCode(tagNetCmd* pCmd);
	DWORD	HandleRoleResetSafeCode(tagNetCmd* pCmd);
	DWORD	HandleRoleCancelSafeCodeReset(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 人物属性获取
	//------------------------------------------------------------------------
	DWORD	HandleGetRoleInitAtt(tagNetCmd* pCmd);
	DWORD	HandleGetRemoteUnitAtt(tagNetCmd* pCmd);
	DWORD	HandleChangeRoleName(tagNetCmd* pCmd);
	//DWORD	HandleGetSelfStrength(tagNetCmd* pCmd);
	DWORD	HandleSetPersonalSet(tagNetCmd* pCmd);
	DWORD	HandleCheckRemoteRoleInfoOpenType(tagNetCmd* pCmd);
	DWORD	HandleGetStrengthDetail(tagNetCmd* pCmd);
	DWORD	HandleGetConsumptiveStrength(tagNetCmd* pCmd);
	DWORD	HandleGetPvPStatics(tagNetCmd* pCmd);
	DWORD	HandleRefreshBaoXiang(tagNetCmd* pCmd);
	DWORD	HandleGetWarBaoXiang(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 行走
	//------------------------------------------------------------------------
	DWORD	HandleRoleWalk(tagNetCmd* pCmd);
	DWORD	HandleRoleStopWalk(tagNetCmd* pCmd);
	DWORD	HandleRoleJump(tagNetCmd* pCmd);
	DWORD	HandleRoleDrop(tagNetCmd* pCmd);
	DWORD	HandleRoleVDrop(tagNetCmd* pCmd);
	DWORD	HandleRoleSlide(tagNetCmd* pCmd);
	DWORD	HandleRoleCanGoToPos(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 装备相关
	//------------------------------------------------------------------------
	DWORD	HandleRoleEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleUnequip(tagNetCmd* pCmd);
	DWORD	HandleRoleSwapWeapon(tagNetCmd* pCmd);
	DWORD	HandleRoleIdentifyEquip(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 角色外观显示设置
	//------------------------------------------------------------------------
	DWORD	HandleRoleSetFashion(tagNetCmd* pCmd);
	DWORD	HandleRoleSetDisplay(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 物品操作相关
	//------------------------------------------------------------------------
	DWORD	HandleRoleChangeItemPos(tagNetCmd* pCmd);
	DWORD	HandleRoleChangeItemPosEx(tagNetCmd* pCmd);
	DWORD	HandleRoleReorderItem(tagNetCmd* pCmd);
	DWORD	HandleRoleReorderItemEx(tagNetCmd* pCmd);
	DWORD	HandleRoleSetTransportMap(tagNetCmd* pCmd); //使用记录式传送符
	DWORD	HandleRoleUseTransportMap(tagNetCmd* pCmd); //使用记录式传送符
	//------------------------------------------------------------------------
	// 玩家间交易相关
	//------------------------------------------------------------------------
	DWORD	HandleRoleExchangeReq(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeReqRes(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeAdd(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeDec(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeMoney(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeLock(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeCancel(tagNetCmd* pCmd);
	DWORD	HandleRoleExchangeVerify(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 商店相关
	//------------------------------------------------------------------------
	DWORD	HandleRoleGetShopItems(tagNetCmd* pCmd);
	DWORD	HandleRoleGetShopEquips(tagNetCmd* pCmd);
	DWORD	HandleRoleBuyShopItem(tagNetCmd* pCmd);
	DWORD	HandleRoleBuyShopEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleSellToShop(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 摆摊相关
	//------------------------------------------------------------------------
	DWORD	HandleRoleStallStart(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetGoods(tagNetCmd* pCmd);
	DWORD	HandleRoleStallUnsetGoods(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetTitle(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetAdText(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetAdFlag(tagNetCmd* pCmd);
	DWORD	HandleRoleStallSetFinish(tagNetCmd* pCmd);
	DWORD	HandleRoleStallClose(tagNetCmd* pCmd);
	DWORD	HandleRoleStallGet(tagNetCmd* pCmd);
	DWORD	HandleRoleStallGetTitle(tagNetCmd* pCmd);
	DWORD	HandleRoleStallBuy(tagNetCmd* pCmd);
	DWORD	HandleRoleStallGetSpec(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 驿站&乾坤石
	//------------------------------------------------------------------------
	DWORD	HandleRoleDak(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 使用磨石
	//------------------------------------------------------------------------
	DWORD	HandleRoleAbrase(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 角色仓库
	//------------------------------------------------------------------------
	DWORD	HandleRoleSideWareOpen(tagNetCmd* pCmd);
	DWORD	HandleRoleWareExtend(tagNetCmd* pCmd);
	DWORD	HandleRoleBagExtand(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 角色仓库中存取金钱&元宝
	//------------------------------------------------------------------------
	DWORD	HandleRoleSaveSilver(tagNetCmd* pCmd);
	DWORD	HandleRoleGetSilver(tagNetCmd* pCmd);
	//DWORD	HandleRoleSaveYuanBao(tagNetCmd* pCmd);
	DWORD	HandleRoleGetYuanBao(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 行囊加密相关
	//------------------------------------------------------------------------
	DWORD	HandleRoleSetBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleUnsetBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleCheckBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleResetBagPsd(tagNetCmd* pCmd);
	DWORD	HandleRoleOpenBagPsd(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 测试
	//------------------------------------------------------------------------
	DWORD	HandleRoleEnterWorld(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 聊天
	//------------------------------------------------------------------------
	DWORD   HandleRoleChat(tagNetCmd* pCmd);
	DWORD   HandleRoleGetID(tagNetCmd* pCmd);
	DWORD	HandleRoleGetNameByNameID(tagNetCmd* pCmd);
	DWORD	HandleRoleGetSomeName(tagNetCmd* pCmd);

	//-------------------------------------------------------------------------
	// 群聊
	//------------------------------------------------------------------------
	DWORD	HandleCreateGroupChat(tagNetCmd* pCmd);
	DWORD	HandleDeleteGroupChat(tagNetCmd* pCmd);

	DWORD	HandleLoginOutGroupChat(tagNetCmd* pCmd);
	DWORD	HandleAddRoleToGroupChat(tagNetCmd* pCmd);
	DWORD	HandleSendGroupChatMsg(tagNetCmd* pCmd);
	DWORD	HandleInviteRoleToGroupChat(tagNetCmd* pCmd);
	DWORD	HandleKickRoleFromGroupChat(tagNetCmd* pCmd);


	//------------------------------------------------------------------------
	// 装备展示
	//------------------------------------------------------------------------
	DWORD   HandleRoleShowEquip(tagNetCmd* pCmd);
	//------------------------------------------------------------------------
	// 物品展示
	//------------------------------------------------------------------------
	DWORD   HandleRoleShowItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 读取留言
	//------------------------------------------------------------------------
	DWORD   HandleRoleLoadLeftMsg(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 地图事件
	//------------------------------------------------------------------------
	DWORD	HandleRoleMapTrigger(tagNetCmd* pCmd);
	DWORD	HandleRoleInstanceNotify(tagNetCmd* pCmd);
	DWORD	HandleRoleInstanceAgree(tagNetCmd* pCmd);
	DWORD	HandleRoleLeaveInstance(tagNetCmd* pCmd);

	//-------------------------------------------------------------------------
	// 属性点相关
	//-------------------------------------------------------------------------
	DWORD	HandleRoleBidAttPoint(tagNetCmd* pCmd);
	DWORD	HandleRoleClearAttPoint(tagNetCmd* pCmd);

	//-------------------------------------------------------------------------
	// 天资技能相关
	//------------------------------------------------------------------------
	DWORD	HandleRoleLearnSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleLevelUpSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleForgetSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleClearTalent(tagNetCmd* pCmd);
    DWORD   HandlePracticeTalent(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 战斗系统
	//------------------------------------------------------------------------
	DWORD	HandleRoleEnterCombat(tagNetCmd* pCmd);
	DWORD	HandleRoleLeaveCombat(tagNetCmd* pCmd);
	DWORD	HandleRoleSkill(tagNetCmd* pCmd);
	DWORD	HandleRoleInterruptSkill(tagNetCmd* pCmd);

	DWORD	HandleRoleCancelBuff(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// PK系统
	//------------------------------------------------------------------------
	DWORD	HandleRoleSafeGuard(tagNetCmd* pCmd);
	DWORD	HandleRoleSetPK(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 复活
	//------------------------------------------------------------------------
	DWORD	HandleRoleBindRebornMap(tagNetCmd* pCmd);
	DWORD	HandleRoleRevive(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 通用函数 -- 需在地图线程上层处理的消息
	//------------------------------------------------------------------------
	DWORD   HandleRoleMsg2World(tagNetCmd* pCmd) { return RET_TRANS; }

	//------------------------------------------------------------------------
	// 通用命令 -- GM
	//------------------------------------------------------------------------
	DWORD   HandleGMCommand(tagNetCmd* pCmd);
	
	//------------------------------------------------------------------------
	// 任务相关
	//------------------------------------------------------------------------
	DWORD	HandleRoleNPCAcceptQuest(tagNetCmd* pCmd);
	DWORD	HandleRoleTriggerAcceptQuest(tagNetCmd* pCmd);
	DWORD	HandleRoleCompleteQuest(tagNetCmd* pCmd);
	DWORD   HandleRoleDeleteQuest(tagNetCmd* pCmd);
	DWORD	HandleRoleUpdateQuestNPCTalk(tagNetCmd* pCmd);
	DWORD	HandleGetSingleQuestDoneInfo(tagNetCmd* pCmd);
	
	        
            
	//------------------------------------------------------------------------
	// 物品强化
	//------------------------------------------------------------------------
	DWORD	HandleRolePosyEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleEngraveEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleInlayEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleBrandEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleLoongSoul(tagNetCmd* pCmd);
	DWORD	HandleRoleQuench(tagNetCmd* pCmd);
	DWORD	HandleRoleChisel(tagNetCmd* pCmd);
	DWORD	HandleRoleDyeFashion(tagNetCmd* pCmd);
	DWORD	HandleEquipGrow(tagNetCmd* pCmd);
	DWORD   HandleEquipPerfectGrow(tagNetCmd* pCmd);
	DWORD	HandleXiPurpleStar(tagNetCmd* pCmd);
	DWORD	HandleEquipGrowTrans(tagNetCmd* pCmd);
	DWORD	HandleEquipGrowDec(tagNetCmd* pCmd);
	DWORD	HandleSoulCrystalGrow(tagNetCmd* pCmd);
	DWORD	HandleSoulCrystalRenew(tagNetCmd* pCmd);
	DWORD	HandleRoleHolyStrength(tagNetCmd* pCmd);
	DWORD	HandleRoleHolyEquipmentNumber(tagNetCmd* pCmd);	

	//------------------------------------------------------------------------
	// 元神
	//------------------------------------------------------------------------
	DWORD	HandleRoleSoulEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleUnSoulEquip(tagNetCmd* pCmd);
	DWORD	HandleRoleHolyAffuse(tagNetCmd* pCmd);
	DWORD	HandleRoleHolyReborn(tagNetCmd* pCmd);
	//------------------------------------------------------------------------
	//元神装备冲星
	//------------------------------------------------------------------------
	//DWORD	HandleStarEnhance(tagNetCmd* pCmd);
	//------------------------------------------------------------------------
	//元神装备凿孔
	//------------------------------------------------------------------------
	//DWORD HandleHolyEquipHole(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	//元神装备镶嵌
	//------------------------------------------------------------------------
	//DWORD HandleHolyEquipInlay(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	//元神装备摘除
	//------------------------------------------------------------------------
	//DWORD HandleHolyEquipRemove(tagNetCmd* pCmd);

	//精纺时装
	DWORD	HandleRoleSpinFashionDress(tagNetCmd* pCmd);
	//------------------------------------------------------------------------
	// 生产合成物品
	//------------------------------------------------------------------------
	DWORD	HandleRoleProduceItem(tagNetCmd* pCmd);
	DWORD	HandleRoleAdvancedCompose(tagNetCmd* pCmd);

	// Jason 2009-12-6
	DWORD	HandleRoleAddEquipPotVal(tagNetCmd* pCmd);
	// Jason 2009-12-7
	DWORD	HandleLessingLoong(tagNetCmd * pCmd);

	// Jason 2009-12-8 点金石
	DWORD HandleGoldStone(tagNetCmd * pCmd);

	// Jason v1.3.1 2009-12-21
	DWORD HandleGemRemoval(tagNetCmd * pCmd);
	DWORD HandleGetGemRemovalInfo(tagNetCmd * pCmd);
	// Jason v1.3.2 玩家回归，传送至npc请求
	DWORD HandlePlayerRegression(tagNetCmd * pCmd);
	DWORD HandleGetOfflineExp(tagNetCmd * pCmd);

	//------------------------------------------------------------------------
	// 点化, 装备分解
	//------------------------------------------------------------------------
	DWORD	HandleRoleDeCompose(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 萃取
	//------------------------------------------------------------------------
	DWORD	HandleRoleExtract(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 使用物品
	//------------------------------------------------------------------------
	DWORD	HandleRoleUseItem(tagNetCmd* pCmd);
	DWORD	HandleRoleInterruptUseItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 拾取物品
	//------------------------------------------------------------------------
	DWORD	HandleRolePickUpItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 扔掉物品
	//------------------------------------------------------------------------
	DWORD	HandleRolePutDownItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 好友相关
	//------------------------------------------------------------------------
	DWORD   HandleRoleMakeFriend(tagNetCmd* pCmd);
	DWORD	HandleRoleCancelFriend(tagNetCmd* pCmd);
	DWORD	HandleUpdateFriendGroup(tagNetCmd* pCmd);
	DWORD	HandleMoveBlackList(tagNetCmd* pCmd);
	DWORD	HandleDeleteBlackList(tagNetCmd* pCmd);
	DWORD	HandleRoleSendGift(tagNetCmd* pCmd);
	DWORD	HandleRoleSendGiftReply(tagNetCmd* pCmd);
	DWORD   HandleUpdateFriOnline(tagNetCmd* pCmd);
	//Jason 2009-11-25 入队申请相关协议
	DWORD	HandleApplyJoinTeam(tagNetCmd *pCmd);
	DWORD	HandleReplyForApplingToJoinTeam(tagNetCmd * pCmd);

	//------------------------------------------------------------------------
	// 小队相关
	//------------------------------------------------------------------------
	DWORD   HandleRoleJoinTeam(tagNetCmd* pCmd);
	DWORD	HandleRoleJoinTeamReply(tagNetCmd* pCmd);
	DWORD	HandleRoleKickMember(tagNetCmd* pCmd);
	DWORD	HandleRoleLeaveTeam(tagNetCmd* pCmd);
	DWORD	HandleRoleSetPickMol(tagNetCmd* pCmd);
	DWORD	HandleRoleChangeLeader(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 称号相关
	//------------------------------------------------------------------------
	DWORD HandleRoleActiveTitle(tagNetCmd* pCmd);
	DWORD HandleRoleGetTitles(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 名帖相关
	//------------------------------------------------------------------------
	DWORD HandleRoleGetVCard(tagNetCmd* pCmd);
	DWORD HandleRoleSetVCard(tagNetCmd* pCmd);
	DWORD HandleGetHeadPicUrl(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 防沉迷相关
	//------------------------------------------------------------------------
	DWORD HandleGetFatigueInfo(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 名人堂相关
	//------------------------------------------------------------------------
	DWORD HandleGetFameHallRoles(tagNetCmd* pCmd);
	DWORD HandleGetReputeTop(tagNetCmd* pCmd);
	DWORD HandleGetActClanTreasure(tagNetCmd* pCmd);
	DWORD HandleActiveTreasure(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 声望相关
	//------------------------------------------------------------------------
	DWORD HandleGetRoleClanData(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 反外挂相关
	//------------------------------------------------------------------------
	DWORD HandleGameGuarderMsg(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 宠物相关
	//------------------------------------------------------------------------
	DWORD HandleGetPetAttr(tagNetCmd* pCmd);
	DWORD HandlePetSkill(tagNetCmd* pCmd);
	DWORD HandlePetSkillStopWorking(tagNetCmd* pCmd);
	DWORD HandleUsePetEgg(tagNetCmd* pCmd);
	DWORD HandleDeletePet(tagNetCmd* pCmd);
	DWORD HandleNC_RecallPet(tagNetCmd* pCmd);
	DWORD HandleGetPetDispInfo(tagNetCmd* pCmd);
	DWORD HandleSetPetState(tagNetCmd* pCmd);

	DWORD HandlePetEquip(tagNetCmd* pCmd);
	DWORD HandlePetUnEquip(tagNetCmd* pCmd);
	DWORD HandlePetSwapEquipPos(tagNetCmd* pCmd);
	DWORD HandleGetPetPourExpMoneyNeed(tagNetCmd* pCmd);
	DWORD HandlePetPourExp(tagNetCmd* pCmd);
	DWORD HandlePetUpStep(tagNetCmd* pCmd);
	DWORD HandlePetEnhance(tagNetCmd* pCmd);
	DWORD HandlePetLearnSkill(tagNetCmd* pCmd);
	DWORD HandlePetInvite(tagNetCmd* pCmd);
	DWORD HandlePetOnInvite(tagNetCmd* pCmd);
	DWORD HandlePetFood(tagNetCmd* pCmd);
	DWORD HandlePetSetLock(tagNetCmd* pCmd);
	DWORD HandlePetRename(tagNetCmd* pCmd);	

	DWORD	HandleRolePetExchangeReq(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeReqRes(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeAdd(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeDec(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeMoney(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeLock(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeCancel(tagNetCmd* pCmd);
	DWORD	HandleRolePetExchangeVerify(tagNetCmd* pCmd);

	// Jason 2010-1-30 v1.3.2 宠物带扩容
	DWORD	HandlePetPocketExtendRequest(tagNetCmd* pCmd);
	// 快捷栏数据保存数据库
	DWORD	HandleQuickBarDataSaveToServer(tagNetCmd* pCmd);
	DWORD	HandleQuickBarDataLoadFromServer(tagNetCmd* pCmd);
	DWORD	HandleInformSaveQuickBarData(tagNetCmd* pCmd);
	DWORD	HandleIsSavedQuickBarData(tagNetCmd* pCmd);
	                 
	        
	DWORD	HandlePetGetPocketMaxCount(tagNetCmd * pCmd);


	//------------------------------------------------------------------------
	// 百宝袋相关
	//------------------------------------------------------------------------
	DWORD HandleInitBaiBaoRecord(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// VIP摊位相关
	//------------------------------------------------------------------------
	DWORD HandleGetAllVIPStallInfo(tagNetCmd* pCmd);
	DWORD HandleUpdateVIPStallInfo(tagNetCmd* pCmd);
	DWORD HandleApplyVIPStall(tagNetCmd* pCmd);
	DWORD HandleSpecVIPStallGet(tagNetCmd* pCmd);
	DWORD HandleBuyVIPStallGoods(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// NPC相关
	//------------------------------------------------------------------------
	DWORD HandleTalkToNPC(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 金牌网吧相关
	//------------------------------------------------------------------------
	DWORD HandleGetVipNetBarName(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 仙界修行
	//------------------------------------------------------------------------
	DWORD HandlePracticeMiracleSkill(tagNetCmd* pCmd);
	DWORD HandleUseAddMiraclePrcNumItem(tagNetCmd* pCmd);	

	//------------------------------------------------------------------------
	// 夫妻系统
	//------------------------------------------------------------------------
	//结婚
	DWORD HandleGetMarriageReq(tagNetCmd* pCmd);
	DWORD HandleGetMarriageReqRes(tagNetCmd* pCmd);
	//离婚
	DWORD HandleBreakMarriageReq(tagNetCmd* pCmd);
	DWORD HandleForceBreakMarriageReq(tagNetCmd* pCmd);
	DWORD HandleBreakMarriageReqRes(tagNetCmd* pCmd);
	//婚礼
	DWORD HandleCelebrateWeddingReq(tagNetCmd* pCmd);
	DWORD HandleCelebrateWeddingReqRes(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 个性动作
	//------------------------------------------------------------------------
	DWORD	HandleRoleStyleAction(tagNetCmd* pCmd);
	DWORD	HandleRoleDuetMotionInvite(tagNetCmd* pCmd);
	DWORD	HandleRoleDuetMotionOnInvite(tagNetCmd* pCmd);
	DWORD	HandleRoleDuetMotionStart(tagNetCmd* pCmd);


	/************************************************************************
	** Handlers -- upper all map thread
	*************************************************************************/

	//------------------------------------------------------------------------
	// 商城相关
	//------------------------------------------------------------------------
	DWORD HandleRoleMallGet(tagNetCmd* pCmd);
	DWORD HandleRoleMallUpdate(tagNetCmd* pCmd);
	DWORD HandleRoleMallBuyItem(tagNetCmd* pCmd);
	DWORD HandleRoleMallShoppingCart(tagNetCmd* pCmd);
	DWORD HandleRoleMallBuyPack(tagNetCmd* pCmd);
	DWORD HandleRoleMallPresentItem(tagNetCmd* pCmd);
	DWORD HandleRoleMallPresentPack(tagNetCmd* pCmd);
	DWORD HandleRoleMallFreeGetItem(tagNetCmd* pCmd);
	DWORD HandleRoleMallLaunchGroupPurchase(tagNetCmd* pCmd);
	DWORD HandleRoleMallRespondGroupPurchase(tagNetCmd* pCmd);
	DWORD HandleRoleMallGetGroupPurchaseInfo(tagNetCmd* pCmd);
	DWORD HandleRoleMallGetParticipators(tagNetCmd* pCmd);
	DWORD HandleRoleMallItemExchange(tagNetCmd* pCmd);
	DWORD HandleRoleMallPackExchange(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 元宝交易相关
	//------------------------------------------------------------------------
	DWORD HandleRoleSaveYB2Account(tagNetCmd* pCmd);
	DWORD HandleRoleSaveSilver2Account(tagNetCmd* pCmd);
	DWORD HandleRoleDepositYBAccount(tagNetCmd* pCmd);
	DWORD HandleRoleDepositSilver(tagNetCmd* pCmd);
	DWORD HandleRoleGetYBTradeInfo(tagNetCmd* pCmd);
	DWORD HandleRoleSubmitSellOrder(tagNetCmd* pCmd);
	DWORD HandleRoleSubmitBuyOrder(tagNetCmd* pCmd);
	DWORD HandleRoleDeleteOrder(tagNetCmd* pCmd);
	DWORD HandleRoleGetYBOrder(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 帮派相关
	//------------------------------------------------------------------------
	DWORD HandleCreateGuild(tagNetCmd* pCmd);
	DWORD HandleDismissGuild(tagNetCmd* pCmd);
	DWORD HandleJoinGuildReq(tagNetCmd* pCmd);
	DWORD HandleJoinGuildReqRes(tagNetCmd* pCmd);
	DWORD HandleLeaveGuild(tagNetCmd* pCmd);
	DWORD HandleKickFromGuild(tagNetCmd* pCmd);
	DWORD HandleTurnoverGuild(tagNetCmd* pCmd);
	DWORD HandleDemissFromGuild(tagNetCmd* pCmd);
	DWORD HandleAppointForGuild(tagNetCmd* pCmd);
	DWORD HandleChangeGuildTenet(tagNetCmd* pCmd);
	DWORD HandleGuildChangeName(tagNetCmd* pCmd);
	

	DWORD HandleSyncGuildInfo(tagNetCmd* pCmd);

	DWORD HandleGetGuildMembers(tagNetCmd* pCmd);
	DWORD HandleGetGuildMemberEx(tagNetCmd* pCmd);
	DWORD HandleRefreshGuildMember(tagNetCmd* pCmd);
	DWORD HandleGetGuildName(tagNetCmd* pCmd);
	DWORD HandleGetGuildTenet(tagNetCmd* pCmd);
	DWORD HandleGetSomeGuildName(tagNetCmd* pCmd);

	DWORD HandleGetGuildWareItems(tagNetCmd* pCmd);
	DWORD HandleGetGuildWarePriList(tagNetCmd* pCmd);
	DWORD HandleGuildWarePrivilege(tagNetCmd* pCmd);

	DWORD HandleGetGuildFacilitiesInfo(tagNetCmd* pCmd);
	DWORD HandleHandInItems(tagNetCmd* pCmd);

	DWORD HandleSpreadGuildAffair(tagNetCmd* pCmd);

	DWORD HandleGetGuildSkillInfo(tagNetCmd* pCmd);
	DWORD HandleUpgradeGuildSkill(tagNetCmd* pCmd);
	DWORD HandleLearnGuildSkill(tagNetCmd* pCmd);
	DWORD HandleSetResearchSkill(tagNetCmd* pCmd);
	DWORD HandleActiveGuildSkill(tagNetCmd* pCmd);

	DWORD HandleGetCofCInfo(tagNetCmd* pCmd);
	DWORD HandleCloseCofC(tagNetCmd* pCmd);
	DWORD HandleBuyCofCGoods(tagNetCmd* pCmd);
	DWORD HandleSellCofCGoods(tagNetCmd* pCmd);
	DWORD HandleGetCommodityInfo(tagNetCmd* pCmd);
	DWORD HandleGetTaelInfo(tagNetCmd* pCmd);
	DWORD HandleGetCommerceRank(tagNetCmd* pCmd);

	DWORD HandleAcceptCommerce(tagNetCmd* pCmd);
	DWORD HandleCompleteCommerce(tagNetCmd* pCmd);
	DWORD HandleAbandonCommerce(tagNetCmd* pCmd);
	DWORD HandleSwitchCommendation(tagNetCmd* pCmd);

	// Jason 2010-2-20 v1.3.2 申请入帮派相关
	// 得到帮派宗旨
	DWORD HandleGetGuildPurpose(tagNetCmd * pCmd);
	// 得到前一百名帮派简短信息
	DWORD HandleGetGuildSmallInfo(tagNetCmd * pCmd);
	// 申请加入帮派
	DWORD HandleApplyToJoinGuild(tagNetCmd * pCmd);
	// 联系帮主
	DWORD HandleContactGuildLeader(tagNetCmd * pCmd);
	// 获取帮派申请者清单
	DWORD HandleGetGuildApplierBill(tagNetCmd * pCmd);
	// 传递帮主批准入帮派申请消息
	DWORD HandleApproveToJoinGuild(tagNetCmd * pCmd);

	DWORD HandleSearchGuild(tagNetCmd * pCmd);

	//------------------------------------------------------------------------
	// 家族相关
	//------------------------------------------------------------------------
	DWORD HandleCreateFamily(tagNetCmd * pCmd);
	DWORD HandleDismissFamily(tagNetCmd * pCmd);
	DWORD HandleJoinFamilyReq(tagNetCmd * pCmd);
	DWORD HandleJoinFamilyReqRes(tagNetCmd * pCmd);
	DWORD HandleLeaveFamily(tagNetCmd * pCmd);
	DWORD HandleKickFromFamily(tagNetCmd * pCmd);
	DWORD HandleHandOverFamily(tagNetCmd * pCmd);
	DWORD HandleFamilyApply(tagNetCmd * pCmd);
	DWORD HandleFamilyApplyRes(tagNetCmd * pCmd);
	DWORD HandleGetFamilyName(tagNetCmd * pCmd);
	DWORD HandleGetFamilyInfo(tagNetCmd * pCmd);

	//------------------------------------------------------------------------
	// 开启宝箱相关
	//------------------------------------------------------------------------
	
	DWORD HandleStartTreasureChest(tagNetCmd* pCmd);
	DWORD HandleOpenTreasureChest(tagNetCmd* pCmd);
	DWORD HandleStopTreasureChest(tagNetCmd* pCmd);
	DWORD HandleAgainTreasureChest(tagNetCmd* pCmd);
	DWORD HandleChestGetItem(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 返回角色选择
	//------------------------------------------------------------------------
	DWORD HandleReturnRoleSelect(tagNetCmd* pCmd);

	// 客户端对话框发给服务的缺省消息
	DWORD HandleDlgDefaultMsg(tagNetCmd* pCmd);
	// 客户端触发服务器脚本的缺省消息
	DWORD HandleDefaultRequest(tagNetCmd* pCmd);

	// 获得其他玩家的装备信息
	DWORD HandleGetSomeoneEquip(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 职业相关
	//------------------------------------------------------------------------
	DWORD HandleSetClass(tagNetCmd* pCmd);
	DWORD HandleChangeClass(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 实力系统相关
	//------------------------------------------------------------------------
	DWORD HandleGetRankings(tagNetCmd* pCmd);
	DWORD HandleSynTotalStrength(tagNetCmd* pCmd);
	
	DWORD HandleGetStrengthRankings(INT nPage);
	DWORD HandleGetFameRankings(INT nPage);
	DWORD HandleGetLevelRankings(INT nPage);
	DWORD HandleGetPetRankings(INT nPage);
	DWORD HandleGetEquipRankings(INT nPage);
    DWORD HandleGetWuXunRankings(INT nPage);
	DWORD HandleGetFlowerRankings(INT nPage);
	DWORD HandleGetEggRankings(INT nPage);

	//------------------------------------------------------------------------
	// 离线提示
	//------------------------------------------------------------------------
	DWORD HandleRoleLeaveNotify(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 城市系统
	//------------------------------------------------------------------------
	DWORD HandleGetCityAttInfo(tagNetCmd* pCmd);
	DWORD HandleGetCityAttInfoByCityID(tagNetCmd* pCmd);
	DWORD HandleGetAppointedCityAtt(tagNetCmd* pCmd);
	DWORD HandleSetTaxRate(tagNetCmd* pCmd);
	DWORD HandleTransformTaxation(tagNetCmd* pCmd);
	DWORD HandleSignupForCitywar(tagNetCmd* pCmd);
	DWORD HandleGetSigupInfo(tagNetCmd* pCmd);
	DWORD HandleConfirmDefenceForce(tagNetCmd* pCmd);
	DWORD HandleGetCityWarInfo(tagNetCmd* pCmd);
	DWORD HandleGetCityWarCity(tagNetCmd* pCmd);
	DWORD HandleUpgradeProlificacy(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 城战系统
	//------------------------------------------------------------------------
	DWORD HandleWaitSummon(tagNetCmd* pCmd);
	DWORD HandleCancelSummon(tagNetCmd* pCmd);
	DWORD HandleCSRoleRecord(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 物品系统
	//------------------------------------------------------------------------
	DWORD HandleBindItem(tagNetCmd* pCmd);
	DWORD HandleTracePlayer(tagNetCmd* pCmd);
	DWORD HandleMove2Role(tagNetCmd* pCmd);
	DWORD HandleGetPlayerPos(tagNetCmd* pCmd);
	DWORD HandleEquipSignature(tagNetCmd* pCmd);
	DWORD HandleSpecScriptItem(tagNetCmd* pCmd);
	DWORD HandleAddPetLiveTime(tagNetCmd* pCmd);

	//------------------------------------------------------------------------
	// 活动帮战相关
	//------------------------------------------------------------------------
	DWORD HandleGetAllGodMiracle(tagNetCmd* pCmd);
	DWORD HandleApplyCapGodMiracle(tagNetCmd* pCmd);
	DWORD HandleGetAllGodInfo(tagNetCmd* pCmd);

	// 精力系统
	DWORD HandleSpiritReward(tagNetCmd* pCmd);
	DWORD HandleStrength2Spirit(tagNetCmd* pCmd);

	// 组队增益相关
	DWORD HandleTeamProfit(tagNetCmd* pCmd);

    //竞技系统相关 
    DWORD HandlePvPEnterHall(tagNetCmd* pCmd); 
	DWORD HandlePvPLeaveHall(tagNetCmd* pCmd);
	
	DWORD HandlePvPGetRoomList(tagNetCmd* pCmd);
	DWORD HandlePvPGetRoomInfo(tagNetCmd* pCmd);
	DWORD HandlePvPGetSimRoomInfo(tagNetCmd* pCmd);

	DWORD HandlePvPInviteJoin(tagNetCmd* pCmd);
	DWORD HandlePvPInvite(tagNetCmd* pCmd);
	DWORD HandlePvPAutoJoinRoom(tagNetCmd* pCmd);
    DWORD HandlePvPJoinRoom(tagNetCmd* pCmd);
	DWORD HandlePvPCreateRoom(tagNetCmd* pCmd);
	DWORD HandlePvPLeaveRoom(tagNetCmd* pCmd);
	
	DWORD HandlePvPChangePos(tagNetCmd* pCmd);
	DWORD HandlePvPPrepare(tagNetCmd* pCmd);
	DWORD HandlePvPUnPrepare(tagNetCmd* pCmd);
    DWORD HandlePvPKickPlayer(tagNetCmd* pCmd); 
	DWORD HandlePvPStart(tagNetCmd* pCmd);

	//选秀活动相关
    DWORD HandleQueryDragonGirlRankings(tagNetCmd* pCmd);
	DWORD HandleQueryDragonDefenderRankings(tagNetCmd* pCmd);
	DWORD HandleVoteForDrangonGirl(tagNetCmd* pCmd);
  
    //------------------------------------------------------------------------
	// 龙之试炼系统
	//------------------------------------------------------------------------
	DWORD HandleGetTrainList(tagNetCmd* pCmd);
	DWORD HandleFinishTrain(tagNetCmd* pCmd);

    //------------------------------------------------------------------------
	// 战斗挂机系统
	//------------------------------------------------------------------------
	DWORD HandleUseZhanDouFu(tagNetCmd* pCmd);
	DWORD HandleSyncAutoFightState(tagNetCmd* pCmd);

    //------------------------------------------------------------------------
	// 仇敌列表相关消息
    //------------------------------------------------------------------------
	DWORD HandleDeleteEnemy(tagNetCmd* pCmd);

    //------------------------------------------------------------------------
	// 归元
    //------------------------------------------------------------------------
	DWORD HandleGuiYuan(tagNetCmd* pCmd);

    //------------------------------------------------------------------------
	// 公会战争
    //------------------------------------------------------------------------
	DWORD HandleDeclareGuildWar(tagNetCmd* pCmd);

    //------------------------------------------------------------------------
	// KeyCodeBridge
    //------------------------------------------------------------------------
	DWORD HandleKeyCode(tagNetCmd* pCmd);
	
	//------------------------------------------------------------------------
	// 师徒系统
	//------------------------------------------------------------------------
	DWORD HandleApprenticeApplyMaster(tagNetCmd* pCmd);
	DWORD HandleMasterReply(tagNetCmd* pCmd);
	DWORD HandleMasterAcceptApprentice(tagNetCmd* pCmd);
	DWORD HandleApprenticeReply(tagNetCmd* pCmd);
	DWORD HandleMatesInfo(tagNetCmd* pCmd);
	DWORD HandleApprenticeInfo(tagNetCmd* pCmd);
	DWORD HandleGiveJingWuPoint(tagNetCmd* pCmd);
	DWORD HandleJingWu2Client(tagNetCmd* pCmd);
	DWORD HandleUseJingWuPoint(tagNetCmd* pCmd);
	DWORD HandleBreakOutMaster(tagNetCmd* pCmd);
	DWORD HandleForceBreakOutMaster(tagNetCmd* pCmd);
	DWORD HandleBreakOutMasterConfirm(tagNetCmd* pCmd);
	DWORD HandleBreakOutApprentice(tagNetCmd* pCmd);
	DWORD HandleBreakOutApprenticeConfirm(tagNetCmd* pCmd);

	// Jason 2010-4-15 v2.0.0 法宝相关
	DWORD HandleFabaoPractice(tagNetCmd * pCmd);
	DWORD HandleFabaoInlay(tagNetCmd * pCmd);
	// 2010-4-16
	DWORD HandleFabaoWuxingReversal(tagNetCmd * pCmd);
	DWORD HandleFabaoWuxingMerging(tagNetCmd * pCmd);
	// 2010-7-5 v1.0.0 妖精秘药
	DWORD HandleFairyPowder(tagNetCmd * pCmd);
	DWORD HandleFairyRaiseIntelligenceMax(tagNetCmd * pCmd);
	DWORD HandleFairyDoRaiseIntelligence(tagNetCmd * pCmd);

	DWORD HandleEarthTemple(tagNetCmd * pCmd);
	DWORD HandleRaiseEquipPotval(tagNetCmd *);

	// 易容师
	DWORD HandleAppearanceTransform(tagNetCmd * pCmd);

	DWORD HandlePracticeSoaringSkill(tagNetCmd*pCmd);

	DWORD HandleBuildEuipment(tagNetCmd * pCmd);
	DWORD HandleCastingEquipment(tagNetCmd * pCmd);
	// Jason 2010-6-2 v2.2.0 服务器脚本拉人，客户端反馈消息
	DWORD HandlePullRoleMsgCallback(tagNetCmd*pCmd);
	// Jason 2010-6-18 v1.0.0
	DWORD HandleStrengthenEquip(tagNetCmd * pCmd);
	DWORD HandleDetachOrAttackSoulEquip(tagNetCmd * pCmd);

	DWORD HandleGetAllianceList(tagNetCmd * pCmd);
	DWORD HandleApplyForAlliance(tagNetCmd * pCmd);
	DWORD HandleConfirmAllianceGuild(tagNetCmd * pCmd);

	// 妖精灵魂相关，妖精蛋
	// 开启妖精蛋
	DWORD HandleOpenFairyTrain(tagNetCmd * pCmd);
	// 捕捉妖精
	DWORD HandleArrestingFairy(tagNetCmd * pCmd);
	// 关闭妖精捕捉游戏
	DWORD HandleCloseFairyTrain(tagNetCmd * pCmd);

	// 神铸
	DWORD	HandleGodStrengthen(tagNetCmd * pCmd);
	
	// 使用VIP卡片道具
	DWORD	HandleUseVipCard(tagNetCmd * pCmd);

	// 神石，使用神石，洗装备一级属性以及二级属性个数等
	DWORD	HandleRockStone(tagNetCmd * pCmd);

	// 神之竞标
	DWORD	HandleGodBiderShopOpen(tagNetCmd * pCmd);
	DWORD	HandleConformPrice(tagNetCmd * pCmd);
	DWORD	HandleBlackMarketShopOpen(tagNetCmd * pCmd);
	DWORD	HandleBlackConPrice(tagNetCmd * pCmd);

	// Jason 2010-11-29 目标的目标
	DWORD	HandleTargetOfTarget(tagNetCmd * pCmd);

	// Jason 2010-11-30 处理客户端设置拾取模式
	DWORD	HandleSetPickupMode(tagNetCmd * pCmd);

	// 通过道具增加领域
	DWORD	HandleRoleUseLimitedItem(tagNetCmd* pCmd);

public:
	// Jason 2010-5-18 v2.1.2 飞升相关
	DWORD HandleQuerySoaringSwitch(tagNetCmd * pCmd);
	// 便捷组队
	DWORD HandleGetEasyTeamInfo(tagNetCmd * pCmd);
	// 获得散人
	DWORD HandleGetFreePlayers(tagNetCmd* pCmd);
	// 获取求组申请清单
	DWORD HandleGetPersonalInfo(tagNetCmd* pCmd);
	// 提交组队或者求组申请
	DWORD HandleNCPutOutInfo(tagNetCmd*pCmd);
	// 删除个人的求组或者组队申请
	DWORD HandleNCDeleteAppTeamInfo(tagNetCmd* pCmd);
	DWORD HandleNCDeleteMyTeamInfo(tagNetCmd* pCmd);

public:
	DWORD HandleOpenQuestBoard(tagNetCmd* pCmd);
	DWORD HandleRefreshQuestBoard(tagNetCmd* pCmd);
	// 妖精喂食
	DWORD HandleFabaoFeeding(tagNetCmd * pCmd);
	// 妖精元素伤害转化
	DWORD HandleFabaoEleInjuryType(tagNetCmd* pCmd);
	// 妖精能力提升
	DWORD HandleFabaoIncEleInjury(tagNetCmd * pCmd);
	// 增加妖精可提升次数
	DWORD HandleFabaoAddPromoteTimes(tagNetCmd * pCmd);
	// 妖精特技确认
	DWORD HandleFabaoStuntConfirm(tagNetCmd * pCmd);
	// 妖精特技书
	DWORD HandleFabaoStuntBook(tagNetCmd * pCmd);
	// 妖精重生
	DWORD HandleFabaoNirvana(tagNetCmd * pCmd);

	// 战场奖励
	// 获取帮派占领战场次数
	DWORD HandleGuildWarOccupyedNum(tagNetCmd * pCmd);
	// 领取每日奖励
	DWORD HandleGuildWarTakeWarReword(tagNetCmd * pCmd);

	DWORD HandleSyncFaBaoStoreExpInfo(tagNetCmd * pCmd);
	DWORD HandleExpBall2ExpPilular(tagNetCmd * pCmd);
	DWORD HandleExpBallBind2Role(tagNetCmd * pCmd);
	DWORD HandleUseExpPilular(tagNetCmd * pCmd);

	// 妖精繁殖相关
	// 查看妖精生育状态
	DWORD	HandleFairyInquireSpirteBornState(tagNetCmd * pCmd);
	// 邀请 妖精配对
	DWORD	HandleApplyFairyBirth(tagNetCmd * pCmd);
	// 应答妖精配对
	DWORD	HandleReplyFairyBirth(tagNetCmd * pCmd);
	// 放入妖精
	DWORD	HandlePutFairyInto(tagNetCmd * pCmd);
	// 妖精配对
	DWORD	HandleFairyMating(tagNetCmd * pCmd);
	// 取消妖精配对邀请
	DWORD	HandleCancelInvitingFairyMating(tagNetCmd * pCmd);
	// 妖精生育
	DWORD	HandleFairyBorn(tagNetCmd * pCmd);

	// 砸金蛋
	DWORD	HandleEggBegin(tagNetCmd * pCmd);
	DWORD	HandleBreakEgg(tagNetCmd * pCmd);
	DWORD	HandleTakeEggTreasure(tagNetCmd * pCmd);
	DWORD	HandleFinishEggGame(tagNetCmd * pCmd);

	// 关闭pk保护模式提示消息
	DWORD	HandleClosePKSafeGuard(tagNetCmd * pCmd);

	// 衣橱
	DWORD	HandleRehanding(tagNetCmd * pCmd);
	DWORD	HandleWardrobeLevelUp(tagNetCmd * pCmd);
	DWORD	HandleDressUpAnotherRole(tagNetCmd * pCmd);
	DWORD	HandleIsDressUpAgree(tagNetCmd * pCmd);

	// 停止凝聚神格
	DWORD	HandleStopGodCondense(tagNetCmd * pCmd);

	// 神职
	//获得属性值
	DWORD HandleRoleGetAttribute(tagNetCmd *pCmd);
	// 神职晋升
	DWORD HandleRolePromotionClergy(tagNetCmd *pCmd);
	// 神职竞选
	DWORD HandleGetCompetitiveClergyResult(tagNetCmd *pCmd);
	DWORD HandleGetClergyEnrollInfo(tagNetCmd *pCmd);
	DWORD HandleRoleEnrollClergy(tagNetCmd *pCmd);
	DWORD HandleGetClergyRalation(tagNetCmd* pCmd);

	// 开始凝聚神格
	DWORD HandleStartGodCondense(tagNetCmd * pCmd);

	//神职领域
	DWORD HandleActivationArea(tagNetCmd *pCmd);
	DWORD HandleAddArea(tagNetCmd *pCmd);

	// XTRAP反外挂
	DWORD	HandleXTrap(tagNetCmd * pCmd);

	DWORD	HandleRoleInitDone(tagNetCmd * pCmd);

	//家族妖精

	//获取妖精修炼内容
	DWORD	HandleGetFamilySpriteTrain(tagNetCmd* pCmd);

	//妖精修炼
	DWORD	HandleFamilySpriteTrain(tagNetCmd* pCmd);

	//妖精登记
	DWORD	HandleFamilySpriteRegister(tagNetCmd* pCmd);

	//取回妖精
	DWORD	HandleFamilySpriteGetBack(tagNetCmd* pCmd);

	//获取家族妖精信息
	DWORD	HandleGetFamilyRoleSprite(tagNetCmd* pCmd);

	//获取妖精登记信息
	DWORD	HandleGetFamilySpriteAtt(tagNetCmd* pCmd);

	//获取家族妖精信息
	DWORD	HandleGetFamilySpriteInfo(tagNetCmd* pCmd);
	
	// 领取奖励keycode
	DWORD	HandleRewardByKeyCode(tagNetCmd* pCmd);
	//DWORD HandleRewardReactiveAccount(tagNetCmd* pCmd);
	DWORD	HandleAccountReactive(tagNetCmd* pCmd);

	DWORD	HandleGetServiceInfo(tagNetCmd* pCmd);

	// 家族任务相关
	DWORD	HandleRoleOpenFamilyQuestPage(tagNetCmd* pCmd);
	DWORD	HandleRoleGetFamilyQuestYesterdayInfo(tagNetCmd* pCmd);
	DWORD	HandleRoleGiveInFamilyQuestItem(tagNetCmd* pCmd);
	DWORD	HandleRoleGetFamilyQuestReward(tagNetCmd* pCmd);

	DWORD	HandleGetLiLianExp(tagNetCmd* pCmd);
	DWORD	HandleReceiveGift(tagNetCmd* pCmd);

	DWORD	HandleRequestAttackCode(tagNetCmd* pCmd);
	DWORD	HandleServerAttack(tagNetCmd* pCmd);

	// 结拜
	DWORD	HandleTryMakeBrother(tagNetCmd* pCmd);

	// 经脉
	DWORD	HandleActivePulse(tagNetCmd* pCmd);

	// 老虎机
	DWORD	HandleTiger(tagNetCmd* pCmd);

	//神魔魂
	DWORD	HandleActiveSoulSkill(tagNetCmd* pCmd);
	DWORD	HandleContinueSoulSkill(tagNetCmd* pCmd);

	//刷新成长属性以及宝石增幅道具
	DWORD   HandleUseDiamondEquipGrowIM(tagNetCmd* pCmd);

	//新Key码
	DWORD	HandleNC_GetKeyCodeGift(tagNetCmd* pCmd);

	// 妖精融合 [3/22/2012 zhangzhihua]
	DWORD	GetFairyQualityValue(BYTE byQuality);
	INT		GetFairyQuality(INT nValue, INT nNum);
	DWORD	HandleNC_FairyFusion(tagNetCmd* pCmd);
	DWORD	HandleNC_GetFairyFusion(tagNetCmd* pCmd);

	// 圣灵吸收妖精
	DWORD	HandleNC_SLEatFairy(tagNetCmd* pCmd);

	// 召唤圣灵
	DWORD	HandleNC_CallShengLing(tagNetCmd* pCmd);	

	// 给圣灵穿装备
	DWORD	HandleNC_EquipHoly(tagNetCmd* pCmd);	

	// 给圣灵卸装备
	DWORD	HandleNC_UnEquipHoly(tagNetCmd* pCmd);	

	// 给圣纹充灵能
	DWORD	HandleNC_LingNeng(tagNetCmd* pCmd);

	// 圣纹基础属性充能
	DWORD	HandleNC_ChongNeng(tagNetCmd* pCmd);	

	// 圣纹充能确认
	DWORD	HandleNC_ChongNengResult(tagNetCmd* pCmd);

	// 保存玩家手机号
	DWORD  HandleNC_SaveTelNum(tagNetCmd* pCmd);

	// 玩家不存储手机号
	DWORD  HandleNC_NotSaveTelNum(tagNetCmd* pCmd);
	
	// 领取圣币卷
	DWORD HandleNC_ReceiveYuanBao(tagNetCmd* pCmd);

#ifdef ON_GAMEGUARD
	// (韩国)从客户端接收CS认证包
	DWORD   HandleAnswerKoreaCSApprove(tagNetCmd* pCmd);
	// (韩国)从客户端接收GameGuard报告
	DWORD   HandleGameGuardReport(tagNetCmd* pCmd);
#endif

private:
	static PlayerNetCmdMgr	m_PlayerNetMgr;						// 对应的客户端消息管理器
	static GMCommandMgr		m_GMCommandMgr;						// GM命令管理器

	TMap<DWORD, DWORD>		m_mapLastRecvTime;					// 上次接收消息的时间

	DWORD					m_dwAccountID;						// session id，对应的是帐号ID
	DWORD					m_dwInternalIndex;					// 底层的网络ID
	INT						m_nMsgNum;							// 网络底层未处理的消息数量

	bool					m_bRoleLoading;						// 选择人物时正在等待数据库返回
	bool					m_bRoleEnuming;						// 进入游戏时等待服务器选人将角色信息读取出来
	bool					m_bRoleEnumDone;					// 读取角色信息完毕
	bool					m_bRoleEnumSuccess;					// 读取角色信息是否成功
	bool					m_bRoleCreating;					// 等待创建角色
	bool					m_bRoleDeleting;					// 删除人物时等待数据库返回
	BYTE					m_byPrivilege;						// gm权限
	INT8					m_n8RoleNum;						// 已经创建的角色个数

	bool					m_bRoleInWorld;						// 在游戏世界中
	INT					m_nDistributionID;						// 客户端的运营商ID

	DWORD					m_dwRoleID[MAX_ROLENUM_ONEACCOUNT];	// 帐号下所有角色ID
	
	char					m_szAccount[X_SHORT_NAME];			// 玩家账号

	volatile BOOL			m_bConnectionLost;					// 连接是否已经断开
	volatile BOOL			m_bKicked;							// 是否已经被踢掉

	tagAccountCommon		m_sAccountCommon;					// 账号通用信息

	Role*					m_pRole;							// 对应的角色对象

	DWORD					m_dwIP;								// 客户端IP

	FatigueGuarder			m_FatigueGarder;					// 防沉迷


	tagDWORDTime			m_dwPreLoginTime;					// 上次登录时间
	DWORD					m_dwPreLoginIP;						// 上次登录ip
	tagDWORDTime		m_dwLoginTime;						// 本次登录时间

	FastMutex				m_PlayerNetLock;

	// Jason 2010-1-15 v1.3.2 
	OfflineReward	*		m_pOfflineExperienceReward;			// 离线奖励
    volatile INT            m_nCreatedRoleNumLimit ;            // 限制创建角色的次数
	
	INT16                   m_n16QuickSaveCount;                //快捷栏数据的排数

	// 战斗挂机系统消息的几个crc值
	DWORD					m_dwSyncAfs;
	DWORD					m_dwUseZDF;
	BYTE						m_dwExtID;

	tagDWORDTime     m_dwPreXtrapUpdateTime;
	char							m_XtrapBuffSession[XTRAP_CS4_BUFSIZE_SESSION];

	DWORD                   m_dwChannelID;

	CHAR						m_szMac[X_SHORT_STRING];

	CHAR					m_szKrUserID[X_SHORT_STRING];

#ifdef ON_GAMEGUARD
public:
	CCSAuth3					m_CS;                    // 韩国版本每个玩家对应一个CCSAuth3对象
#endif

public:
	VOID RegisterUserOffline(Role * player);
	INT  GetCreatedRoleNumLimit() { return  m_nCreatedRoleNumLimit;}
	VOID DeductCreateRoleNumLimit(){ m_nCreatedRoleNumLimit--; }
	VOID SetCreatedRoleNumLimit(INT nCreateRoleNum) {  m_nCreatedRoleNumLimit = nCreateRoleNum;}
	// Jason 2010-4-26 v2.1.0
	INT						m_nPKRunawayTick;
	VOLATILE BOOL					m_bPKRunaway;

	//DWORD				m_dwInitTime;			// 为圣灵召唤状态下消耗人物圣灵值计时
	//DWORD				m_dwCoValueTime;		// 为在线恢复圣灵默契值计时

	// 圣纹充能需确认修改的属性及其值
	bool m_bNeedConfirm;	//是否需要充能确认
	BYTE m_byIndex;			//需要确认充能的圣纹索引
	BYTE m_byAttType;		//属性类型
	DWORD m_dwAttValue;		//属性值
	DWORD m_dwLingNeng;		//灵能减少值
public:
	BOOL IsInPKRunawayPunishmentState(VOID);
};


