//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role.h
// author: Aslan
// actor:
// data: 2008-7-11
// last:
// brief: 人物数据结构
//-----------------------------------------------------------------------------
#pragma once

#include "stdafx.h"
#include "../WorldDefine/base_define.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/move_define.h"
#include "../WorldDefine/talent_define.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/pk_define.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/skill_define.h"
#include "../WorldDefine/compose_define.h"
#include "../WorldDefine/ScriptMsgInfo.h"
#include "../WorldDefine/msg_mall.h"

#include "../WorldDefine/msg_quickbar.h"
#include "../WorldDefine/msg_compose.h"
#include "../WorldDefine/msg_pk.h"
#include "../WorldDefine/msg_talent.h"
#include "../WorldDefine/action.h"
#include "../WorldDefine/msg_combat.h"
#include "../WorldDefine/msg_gm_cmd.h"
#include "../WorldDefine/msg_train.h"

#include "../WorldDefine/msg_guild.h"
#include "../WorldDefine/TreasureChest_define.h"
#include "../WorldDefine/penalty_define.h"
#include "../WorldDefine/msg_GroupChat.h"

#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/msg_skill.h"
#include "../WorldDefine/quest.h"

#include "../WorldDefine/msg_AutoFight.h"
#include "../WorldDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_eggbroker.h"
#include "../ServerDefine/data_collect.h"
#include "../ServerDefine/msg_account_reactive.h"
#include "egg_broker.h"

#include "../WorldDefine/msg_yuanshen.h"
#include "../ServerDefine/msg_holySoul.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "unit.h"
#include "item_mgr.h"
#include "currency.h"
#include "db_session.h"
#include "player_session.h"
#include "exchange.h"
#include "pet_exchange.h"
#include "suit.h"
#include "quest.h"
#include "stall.h"
#include "group_mgr.h"
#include "team.h"
#include "vcard.h"
#include "clandata.h"
#include "state_mgr.h"
#include "mutex.h"
#include "longhun.h"
#include "vip_netbar.h"
#include "role_mgr.h"
#include "quest_board.h"
#include <set>
#include "godarea_mgr.h"
#include "bloodbrother.h"
#include "TigerMgr.h"
#include "war_role.h"
#include "HolySoul.h"

class PlayerSession;
class Map;
class Quest;
class Skill;
class Creature;
class Stall;
class Team;
class RoleScript;
class TitleMgr;
class PetPocket;
class BoardQuestMgr;
class group_chat;

struct tagRoleDataLoad;
struct tagQuestDoneSave;
struct tagRoleTalent;
struct tagNDBC_SaveRole;
struct tagIMEffect;
struct tagQuickData;
struct tagQuickBarState;
struct tagAthleticsProperty;

// Jason 2010-1-15 v1.3.2 离线挂机
class OfflineReward;
class IpgCashSession;

enum EAutoFightState;

// 召唤圣灵时消耗圣灵值的时间间隔
const INT COST_HOLY_INTER_TICK		= 5 * TICK_PER_SECOND;

// 人物身上圣灵的默契值的更新时间间隔
const INT HOLY_COVALUE_INTER_TICK		= 1 * TICK_PER_SECOND * 400;

//--------------------------------------------------------------------------------
// 被动技能和装备的触发器类型
//--------------------------------------------------------------------------------
enum EPassiveSkillAndEquipTrigger
{
	EPSAET_Null				=	-1,
	EPSAET_BeAttacked		=	0,	// 被攻击
	EPSAET_Hit				=	1,	// 命中
	EPSAET_Hited			=	2,	// 被命中
	EPSAET_Dodged			=	3,	// 被闪避
	EPSAET_Dodge			=	4,	// 闪避
	EPSAET_Blocked			=	5,	// 被格挡
	EPSAET_Block			=	6,	// 格挡
	EPSAET_Crit				=	7,	// 暴击
	EPSAET_Crited			=	8,	// 被暴击
	EPSAET_Die				=	9,	// 死亡
	EPSAET_Random			=	10,	// 随机
	EPSAET_Attack			=	11,	// 攻击
	EPSAET_Kill				=	12,	// 击杀
	EPSAET_End				=	13
};

// 师门人数发生变化类型
enum EMasterApprenticeNumChg
{
	EMANC_NoChg = 0,	// 没有变化
	EMANC_BaiShi,		// 拜师
	EMANC_ShouTu,		// 收徒
	EMANC_TuoLiShiMen,	// 徒弟脱离师门
	EMANC_ZhuChuShiMen,	// 师傅将徒弟逐出师门
};

// 帮派人数发生变化类型


typedef VOID (Role::*pFun_RegTriggerEquipSet)(DWORD, DWORD, INT16);
//------------------------------------------------------------------------------
// 角色类
//------------------------------------------------------------------------------
class Role : public Unit, public ScriptData<ESD_Role>
{
	friend class OfflineReward;
public:
	friend class PlayerSession;
	friend class GMCommandMgr;
	friend class RoleMgr;

public:
	typedef TMap<UINT16, tagQuestDoneSave*>			QuestDoneMap;
	typedef TMap<UINT16, Quest*>					QuestMap;

	typedef State<DWORD, ERoleState>				RoleState;
	typedef State<DWORD, ERoleStateEx>				RoleStateEx;

	typedef TMap<UINT16, tagTrainState*>			TrainStateMap;

protected:
	//---------------------------------------------------------------------------
	// Constructor&Destructor
	//---------------------------------------------------------------------------
	Role(DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession);
	virtual ~Role();
public:
	static Role* Create(DWORD dwRoleID, const tagRoleDataLoad* pData, PlayerSession* pSession);
	static VOID	Delete(Role* &pRole);

	//---------------------------------------------------------------------------
	// 初始化
	//---------------------------------------------------------------------------
	VOID Init(const tagRoleDataLoad* pData);

	//---------------------------------------------------------------------------
	// 上线
	//---------------------------------------------------------------------------
	VOID Online(BOOL bFirst);

	//----------------------------------------------------------------------------
	// 第一次上线
	//----------------------------------------------------------------------------
	VOID VirginOnline();

	//----------------------------------------------------------------------------
	// 各种Update函数
	//----------------------------------------------------------------------------
	virtual VOID Update();

	//-----------------------------------------------------------------------------
	// 保存数据库
	//-----------------------------------------------------------------------------
	VOID SaveToDB(BOOL bExit = FALSE);

	//-----------------------------------------------------------------------------
	// 加入到游戏世界中
	//-----------------------------------------------------------------------------
	BOOL AddToWorld(BOOL bFirst);

	//-----------------------------------------------------------------------------
	// 传送到到某个地图中
	//-----------------------------------------------------------------------------
	BOOL GotoNewMap(DWORD dwDestMapID, FLOAT fX, FLOAT fY, FLOAT fZ, DWORD dwMisc=0, BOOL bSameInstance=TRUE, DWORD dwDestMapInstID = 0);

	//------------------------------------------------------------------------------
	// 回城
	//------------------------------------------------------------------------------
	VOID ReturnCity();

	//------------------------------------------------------------------------------
	// 改名
	//------------------------------------------------------------------------------
	VOID ReName(LPCTSTR szNewName);

	//------------------------------------------------------------------------------
	// 物品是否需要冷却
	//------------------------------------------------------------------------------
	BOOL ObjectCoolOff()
	{
		return m_bObjectCoolOff;
	}

	VOID SetObjectCoolMode(BOOL bMode)
	{
		m_bObjectCoolOff = bMode;
	}

	//------------------------------------------------------------------------------
	// 禁言
	//------------------------------------------------------------------------------
	BOOL SpeakOff() { return m_bSpeakOff; }
	VOID SetSpeakOff(BOOL bFlag) { m_bSpeakOff = bFlag; }

	//-----------------------------------------------------------------------
	// 检查是否在对应职能NPC附近
	//-----------------------------------------------------------------------
	DWORD CheckFuncNPC(DWORD dwNPCID, EFunctionNPCType eNPCType, OUT Creature **ppNPC = NULL, OUT Map **ppMap = NULL);

	//------------------------------------------------------------------------------
	// 当前状态 -- 各状态间的转换，需要手动完成。即SetState()不会自动清除其他状态位。
	//------------------------------------------------------------------------------
	DWORD GetRoleState() const { return m_RoleState.GetState(); }

	BOOL IsInRoleState(ERoleState eState) const
	{
		return m_RoleState.IsInState(eState);
	}

	BOOL IsInRoleStateAll(DWORD dwState) const
	{
		return m_RoleState.IsInStateAll(dwState);
	}

	BOOL IsInRoleStateAny(DWORD dwState) const
	{
		return m_RoleState.IsInStateAny(dwState);
	}

	VOID SetRoleState(ERoleState eState, BOOL bSendMsg=TRUE)
	{
		m_RoleState.SetState(eState);

		if( bSendMsg )
		{
			tagNS_SetRoleState send;
			send.dwRoleID = GetID();
			send.eState = eState;
			if( P_VALID(GetMap()) )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}

	VOID UnsetRoleState(ERoleState eState, BOOL bSendMsg=TRUE)
	{
		if( FALSE == IsInRoleState(eState) ) return;

		m_RoleState.UnsetState(eState);

		if( bSendMsg )
		{
			tagNS_UnSetRoleState send;
			send.dwRoleID = GetID();
			send.eState = eState;
			if( P_VALID(GetMap()) )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}

	//------------------------------------------------------------------------------
	// 扩展状态 -- 该状态只需玩家本人知道就可以
	//------------------------------------------------------------------------------
	const RoleStateEx& GetRoleStateEx() const
	{
		return m_RoleStateEx;
	}
	
	VOID SetRoleStateEx(ERoleStateEx eState, BOOL bSendMsg=TRUE)
	{
		if( TRUE == m_RoleStateEx.IsInState(eState) ) return;

		m_RoleStateEx.SetState(eState);

		if( bSendMsg )
		{
			tagNS_SetRoleStateEx send;
			send.eState = eState;
			SendMessage(&send, send.dwSize);
		}
	}

	VOID UnsetRoleStateEx(ERoleStateEx eState, BOOL bSendMsg=TRUE)
	{
		if( FALSE == m_RoleStateEx.IsInState(eState) ) return;

		m_RoleStateEx.UnsetState(eState);

		if( bSendMsg )
		{
			tagNS_UnSetRoleStateEx send;
			send.eState = eState;
			SendMessage(&send, send.dwSize);
		}
	}

	//------------------------------------------------------------------------------
	// 是否处在不能移动的状态
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantMove()
	{
		return Unit::IsInStateCantMove() || IsInRoleState(ERS_Stall) ;
	}

	//------------------------------------------------------------------------------
	// 是否处在不能使用技能的状态
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantCastSkill()
	{
		return Unit::IsInStateCantCastSkill() || IsInRoleState(ERS_Stall) || IsInRoleState(ERS_Wedding);
	}

	//------------------------------------------------------------------------------
	// 是否处在不可被攻击状态
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantBeSkill()
	{
		return Unit::IsInStateCantBeSkill() || IsInRoleState(ERS_Stall) || IsInRoleState(ERS_Wedding);
	}
	//------------------------------------------------------------------------------
	//是否处在不可以被邀请组队的状态
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateCantBeInviteJoinTeam()
	{ 
		return Unit::IsInStateCantBeInviteJoinTeam() || IsInRoleState(ERS_Wedding);
	} 
	//------------------------------------------------------------------------------
	// 是否处在不可见的状态
	//------------------------------------------------------------------------------
	virtual BOOL IsInStateInvisible()
	{
		return Unit::IsInStateInvisible();
	}

#ifdef ON_INTERNET_CAFE
	VOID EnableInternetCafe();		// 开启网吧奖励效果 [5/25/2012 zhangzhihua]
	VOID CancelInternetCafe();		// 关闭网吧奖励效果 [5/25/2012 zhangzhihua]
	VOID UpdateInternetCafe();
	VOID SetInternetCafePingLostTime(BYTE byTime);
	BYTE GetInternetCafePingLostTime();
	VOID EndInternetCafe() { m_bTimeout = TRUE; }
	BOOL IsEnd() { return m_bTimeout; }
#endif

	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// 检测所在地图区域
	//------------------------------------------------------------------------------
	VOID CheckMapArea();
	
	//------------------------------------------------------------------------------
	// 金钱和物品
	//------------------------------------------------------------------------------
	CurrencyMgr&	GetCurMgr()		{ return m_CurMgr; }
	ItemMgr&		GetItemMgr()	{ return m_ItemMgr; }
	HolySoul&		GetHolySoul()	{ return m_HolySoul;}

	//-------------------------------------------------------------------------------
	// 人物数据获取
	//-------------------------------------------------------------------------------
	INT						GetLevelPm()	const { return m_nLevelPm; }
	EClassType				GetClass()		const { return m_eClass; }
	EClassTypeEx			GetClassEx()	const { return m_eClassEx; }
	INT						GetCredit()		const { return m_nCredit; }
	INT						GetIdentity()	const { return m_nIdentity; }
	//INT						GetVIPPoint()	const { return GetCurMgr().GetVIPPoint(); }
	PlayerSession*			GetSession()	const { return m_pSession; }
	const tagAvatarAtt*		GetAvatar()		const { return &m_Avatar; }
	const tagDisplaySet&	GetDisplaySet()	const { return m_DisplaySet; }
	ERolePKState			GetPKState()	const { return m_ePKState; }
	DWORD					GetRebornMapID()const { return m_dwRebornMapID; }
	DWORD					GetNameID()		const { return GetID(); }			//?? 该接口需进一步处理
	DWORD					GetTeamID()		const { return m_dwTeamID; }
	DWORD					GetGroupID()	const { return m_dwGroupID; }
	DWORD					GetTeamInvite()	const { return m_dwTeamInvite; }
	DWORD					GetEarnRate()	const { return GetSession()->GetFatigueGuarder().GetEarnRate();}
	FLOAT					GetSpiRate()	const { return m_fSpiRate; }
	virtual BYTE			GetSex()		const { return m_Avatar.bySex; }
	INT						GetOnlineTime() const { return m_nOnlineTime; }
	tagDWORDTime			GetLogoutTime() const { return m_LogoutTime; }
	tagDWORDTime			GetRoleCreateTime() const {return m_CreatTime; }

	tagDWORDTime			GetLoginTime() const {return m_LoginTime;}

	const tagRemoteOpenSet& GetRemoteOpenSet() const { return m_sRemoteOpenSet; }

	LPCTSTR					GetVNBName() const { return GetSession()->GetVNBName(); }

	const tagAvatarEquip&	GetAvatarEquip()
	{
		if (GetDisplaySet().bFashionDisplay)
		{
			if (GetWardrobe().IsPlayActing() || GetWardrobe().IsBePlayActing())
				return m_AvatarEquipWardrobe;
			else
				return m_AvatarEquipFashion;
		}
		else
		{
			return m_AvatarEquipEquip;
		}
	}

	const tagChestItem&		GetChestItem()	const	{ return m_TreasureState.ChestItem; }
	INT64					GetChestSerial() const	{ return m_TreasureState.nChestSerial; }
	INT64					GetKeySerial() const	{ return m_TreasureState.nKeySerial; }
	DWORD					GetChestTypeID() const	{ return m_TreasureState.dwChestTypeID; }
	DWORD					GetKeyTypeID() const	{ return m_TreasureState.dwKeyTypeID; }
	DWORD					GetMasterID() const		{ return m_dwMasterID; }
	TMap<DWORD, DWORD>&		GetApprenticeData() 	{ return m_mapApprentice; }
	INT						GetJingWuPoint() const	{ return m_nJingWuPoint; }
	INT						GetQinWuPoint() const	{ return m_nQinWuPoint; }
	INT						GetLevelUpBonus() const	{ return m_nLevelUpBonus; }
	INT						GetRewardNum()	const	{ return m_nRewardNum; }
	tagClassMateData&		GetClassMate()			{ return m_ClassMateData; }

	const RoleScript*		GetScript()				{ return m_pScript; } 

	//-------------------------------------------------------------------------------
	// 人物数据设置
	//-------------------------------------------------------------------------------

	// 通过易容师改变avatar容貌,返回TRUE,表示有变化，FALSE表示没有变化
	BOOL SetAvatarHairAndFace(	WORD	wHairMdlID,			// 发型
								WORD	wHairTexID,			// 发色
								WORD	wFaceMdlID,			// 面部五官
								WORD	wFaceDetailTexID	// 面部细节
							)
	{
		if (m_Avatar.wHairMdlID != wHairMdlID ||
			m_Avatar.wHairTexID != wHairTexID ||
			m_Avatar.wFaceMdlID != wFaceMdlID ||
			m_Avatar.wFaceDetailTexID != wFaceDetailTexID)
		{
			m_Avatar.wHairMdlID = wHairMdlID;
			m_Avatar.wHairTexID = wHairTexID;
			m_Avatar.wFaceMdlID = wFaceMdlID;
			m_Avatar.wFaceDetailTexID = wFaceDetailTexID;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	VOID SetClass(EClassType eVocation)		
	{
		BOOL bFiltrateSoulSkill = FALSE;
		if ( m_eClass != eVocation )
		{
			bFiltrateSoulSkill = TRUE;
		}
		m_eClass = eVocation;
		g_roleMgr.SetRoleClass(GetID(), static_cast<BYTE>(eVocation));

		if( bFiltrateSoulSkill )
			FiltrateSoulSkill();
	}
	
	VOID SetClassEx(EClassTypeEx eHVocation)	
	{
		m_eClassEx = eHVocation;
	}

	VOID SetBaseStrength(INT nStrength)
	{
		m_Strength.nBaseStrength = nStrength;
	}

	VOID SetStrengthPm(INT nPm)
	{
		m_nStrengthNum = nPm;
	}

	VOID SetChestTypeID(DWORD dwTypeID)
	{
		m_TreasureState.dwChestTypeID = dwTypeID;
	}

	VOID SetKeyTypeID(DWORD dwTypeID)
	{
		m_TreasureState.dwKeyTypeID = dwTypeID;
	}

	VOID SetChestSerial(INT64 nSerial)
	{
		m_TreasureState.nChestSerial = nSerial;
	}

	VOID SetKeySerial(INT64 nSerial)
	{
		m_TreasureState.nKeySerial = nSerial;
	}

	VOID SetChestItem(tagChestItem item)
	{
		m_TreasureState.ChestItem = item;
	}

	VOID SetFashionMode(bool bDisplay)
	{
		m_DisplaySet.bFashionDisplay = bDisplay;
	}

	VOID SetDisplaySet(bool bHead, bool bFace, bool bBack)
	{
		m_DisplaySet.Set(bHead, bFace, bBack);
	}

	VOID SetEquipDisplay(BOOL bFashion, INT nDisplayPos, DWORD dwTypeID, BYTE byFlareVal, INT8 n8ColorID)
	{
		if(bFashion)	// 时装
		{
			m_AvatarEquipFashion.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);
		}
		else	// 武器或防具
		{
			m_AvatarEquipEquip.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);

			// 如果是武器，则做特殊处理
			if(MIsWeaponByDisplayPos(nDisplayPos))
			{
				m_AvatarEquipFashion.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);
				m_AvatarEquipWardrobe.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);
			}
		}
	}

	VOID ResetWardrobeDisplay()
	{
		ZeroMemory(&m_AvatarEquipWardrobe, SIZE_AVATAR_EQUIP);
	}
	VOID SetWardrobeDisplay(INT nDisplayPos, DWORD dwTypeID, BYTE byFlareVal, INT8 n8ColorID)
	{
		m_AvatarEquipWardrobe.Set(nDisplayPos, dwTypeID, byFlareVal, n8ColorID);
	}

	// 龙魂能力特效需要单独设置
	VOID SetEquipEffect(INT nDisplayPos, BYTE byEquipEffect)
	{
		m_AvatarEquipEquip.Set(nDisplayPos, byEquipEffect);
		m_AvatarEquipFashion.Set(nDisplayPos, byEquipEffect);
		m_AvatarEquipWardrobe.Set(nDisplayPos, byEquipEffect);
	}

	// 取得可激发龙魂ID
	LongHun& GetLongHun()				{ return m_LongHun; }

	// 设置记录式传送符的坐标
	DWORD SetItemTransportPoint(tagNetCmd* pCmd);

	// 使用记录式传送符
	DWORD UseNoteTakingItemTransport(tagNetCmd* pCmd);

	// 设置师徒关系
	VOID ModifyMasterApprenticeCount(INT nCount)
	{
		m_nMARelation += nCount;
		if(m_nMARelation < 0) m_nMARelation = 0; 
		g_roleMgr.ModifyMasterApprenticeCount(m_dwID, nCount);
	}

	// 设置师傅ID
	VOID SetMasterID(DWORD dwMasterID)
	{
		m_dwMasterID = dwMasterID;
	}

	// 增加弟子
	VOID AddApprentice(DWORD dwApprenticeID)
	{
		m_mutexMaster.Acquire();
		if (m_mapApprentice.Size() >= MAX_APPRENTICE_NUM)
		{
			m_mutexMaster.Release();
			return;
		}

		m_mapApprentice.Add(dwApprenticeID, dwApprenticeID);
		tagApprenticeData appData;

		m_mapApprentice.ResetIterator();
		DWORD dwData = GT_INVALID;
		int i = 0;
		while (m_mapApprentice.PeekNext(dwData))
		{
			appData.dwData[i++] = dwData;
			if (i >= MAX_APPRENTICE_NUM)
			{
				break;
			}
		}

		m_ApprenticeData = appData;
		m_mutexMaster.Release();
	}

	// 删除弟子
	VOID DeleteApprentice(DWORD dwApprenticeID)
	{
		m_mutexMaster.Acquire();

		m_mapApprentice.Erase(dwApprenticeID);
		tagApprenticeData appData;

		m_mapApprentice.ResetIterator();
		DWORD dwData = GT_INVALID;
		int i = 0;
		while (m_mapApprentice.PeekNext(dwData))
		{
			appData.dwData[i++] = dwData;
		}

		m_ApprenticeData = appData;
		m_mutexMaster.Release();
	}

	// 增加同门
	VOID AddClassMate(DWORD dwMateID)
	{
		m_mutexMate.Acquire();

		for (INT i=0; i!= MAX_APPRENTICE_NUM; ++i)
		{
			// 避免同门重复
			if (m_ClassMateData.dwData[i] == dwMateID)
			{
				break;
			}
			if (m_ClassMateData.dwData[i] == 0)
			{
				m_ClassMateData.dwData[i] = dwMateID;
				break;
			}
		}

		m_mutexMate.Release();
	}

	// 删除同门
	VOID DeleteClassMate(DWORD dwMateID)
	{
		m_mutexMate.Acquire();

		for (INT i=0; i!= MAX_APPRENTICE_NUM; ++i)
		{
			if (m_ClassMateData.dwData[i] == dwMateID)
			{
				m_ClassMateData.dwData[i] = 0;
				break;
			}
		}

		m_mutexMate.Release();
	}

	// 删除所有同门
	VOID DeleteAllClassMate()
	{
		m_mutexMate.Acquire();

		for (INT i=0; i!= MAX_APPRENTICE_NUM; ++i)
		{
			m_ClassMateData.dwData[i] = 0;
		}

		m_mutexMate.Release();
	}

	// 修改精武点数
	VOID SetJingWuPoint(INT nPoint)
	{
		m_nJingWuPoint = nPoint;
	}

	// 修改勤武点数
	VOID SetQinWuPoint(INT nPoint)
	{
		m_nQinWuPoint = nPoint;
	}

	// 修改徒弟升级积分
	VOID SetLevelUpBonus(INT nPoint)
	{
		m_nLevelUpBonus = nPoint;
	}

	// 修改出师奖励次数
	VOID SetRewardNum(INT nNum)
	{
		m_nRewardNum = nNum;
	}

	//-------------------------------------------------------------------------------
	// 练习仙界技能
	//-------------------------------------------------------------------------------
	DWORD PracticeMiracleSkill(DWORD dwSkillID);

	DWORD UseAddMiraclePrcNumItem(INT64 n64ItemID);

	INT16 GetMiraclePrcNum() { return m_n16MiraclePracticeNum; }

	//-------------------------------------------------------------------------------
	// 帮派相关
	//-------------------------------------------------------------------------------
	BOOL	IsInGuild()	const			{ return m_dwGuildID != GT_INVALID; }
	DWORD	GetGuildID() const			{ return m_dwGuildID; }
	VOID	SetGuildID(DWORD dwGuildID);
	DWORD	GetFamilyID() const			{ return m_dwFamilyID; }
	VOID SetFamilyID(DWORD dwFamilyID) { m_dwFamilyID = dwFamilyID; }

	//-------------------------------------------------------------------------------
	// 修炼场相关
	//-------------------------------------------------------------------------------
	VOID   InitMapBuffLeftToClient();            //刚
    VOID   UpdateSendMapBuffLeftTimeToClient();  //发送进地图buff的剩余时间给客户端      
    //设置当前玩家地图buffID
	VOID   SetRoleMapBuffID(DWORD dwBuffId )  {   m_dwMapBuffID = dwBuffId; }
	DWORD  GetRoleMapBuffID()const      {    return m_dwMapBuffID; }
	
	private:
    DWORD  m_dwMapBuffID ;
	INT   m_nIsSended;
	
	public:
	//-------------------------------------------------------------------------------
	// 是否需要保存到数据库
	//-------------------------------------------------------------------------------
	BOOL IsNeedSave2DB() const { return m_nNeedSave2DBCountDown <= 0; }

	//-------------------------------------------------------------------------------
	// 发送消息
	//-------------------------------------------------------------------------------
	VOID SendMessage(LPVOID pMsg, DWORD dwSize, EMessageRange eRange = EMR_Self);

	//---------------------------------------------------------------------------------
	// 投属性点
	//---------------------------------------------------------------------------------
	INT BidAttPoint(const INT nAttPointAdd[X_ERA_ATTA_NUM]);

	//---------------------------------------------------------------------------------
	// 洗属性点
	//---------------------------------------------------------------------------------
	INT ClearAttPoint(INT64 n64ItemID, BOOL bCheckItem=TRUE);

	//---------------------------------------------------------------------------------
	// 是否可以学技能
	//---------------------------------------------------------------------------------
	INT	CanLearnSkill(DWORD dwSkillID, const tagSkillProto* pProto, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID);

	//---------------------------------------------------------------------------------
	// 学技能
	//---------------------------------------------------------------------------------
	VOID LearnSkill(DWORD dwSkillID, const tagSkillProto* pProto, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID, BOOL bDelTalentPoint = TRUE);

	//---------------------------------------------------------------------------------
	// 学技能
	//---------------------------------------------------------------------------------
	INT	LearnSkill(DWORD dwSkillID, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID);

	//---------------------------------------------------------------------------------
	// 职业判断
	//---------------------------------------------------------------------------------
	DWORD VocationCheck(DWORD dwVocationLimit);

	//---------------------------------------------------------------------------------
	// 是否可以升级技能
	//---------------------------------------------------------------------------------
	INT CanLevelUpSkill(Skill* pSkill, const tagSkillProto* pProto, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID);

	//---------------------------------------------------------------------------------
	// 升级技能
	//---------------------------------------------------------------------------------
	VOID LevelUpSkill(Skill* pSkill, const tagSkillProto* pProto, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID, BOOL bDelTalentPoint = TRUE);

	//---------------------------------------------------------------------------------
	// 检查前置技能
	//---------------------------------------------------------------------------------
	INT CheckPreSkill(DWORD dwPreLevelSkillID);

	//---------------------------------------------------------------------------------
	// 升级技能
	//---------------------------------------------------------------------------------
	INT LevelUpSkill(DWORD dwSkillID, DWORD dwNPCID=GT_INVALID, INT64 n64ItemID=GT_INVALID);

	//---------------------------------------------------------------------------------
	// 遗忘技能
	//---------------------------------------------------------------------------------
	INT ForgetSkill(DWORD dwSkillID, DWORD dwNPCID);

	//---------------------------------------------------------------------------------
	// 洗点
	//---------------------------------------------------------------------------------
	INT	ClearTalent(INT64 n64ItemID, BOOL bCheckItem=TRUE);

	//---------------------------------------------------------------------------------
	// 开启/关闭PK保护
	//---------------------------------------------------------------------------------
	INT OpenSafeGuard(INT64 n64Item);
	INT CloseSafeGuard();

	//---------------------------------------------------------------------------------
	// 开启/更新行凶状态
	//---------------------------------------------------------------------------------
	INT SetPK(BOOL bOnlyPK);
	VOID ResetPK();
	VOID UpdatePK();

	//精力系统
	VOID UpdateSpirit();
	float CalSpiritRate(INT nSpirit);
	INT64  ModFixSpiriteCDTime(INT64 nValue) 
	{
		m_dwFixSpiriteCDTime += nValue;
		if(m_dwFixSpiriteCDTime < 0 ) m_dwFixSpiriteCDTime = 0;
		return m_dwFixSpiriteCDTime;
	}

	//---------------------------------------------------------------------------------
	// 开启/更新PVP状态
	//---------------------------------------------------------------------------------
	VOID SetPVP();
	VOID UpdatePVP();

	//---------------------------------------------------------------------------------
	// 计算PK状态
	//---------------------------------------------------------------------------------
	VOID CalPKState(BOOL bSendMsg=TRUE);

	//---------------------------------------------------------------------------------
	// 设置戾气为0时回城复活开启pk保护
	//---------------------------------------------------------------------------------
	VOID SetHostilitySafeGuard();
	VOID UpdateHostilitySafeGuard();

	//---------------------------------------------------------------------------------
	// 复活
	//---------------------------------------------------------------------------------
	VOID BeRevived(INT nHP, INT nMP, Unit* pSrc);

	//---------------------------------------------------------------------------------
	// 同步单向好友是否在线
	//---------------------------------------------------------------------------------
	DWORD UpdateFriOnline();

	//---------------------------------------------------------------------------------
	// 目标是否完全可见//??
	//---------------------------------------------------------------------------------
	BOOL CanSeeTargetEntirely(Unit *pUnit)
	{
		return (FriendEnemy(pUnit) & ETFE_Friendly) == ETFE_Friendly;
	}

	//---------------------------------------------------------------------------------
	// 任务设置复活点
	//---------------------------------------------------------------------------------
	DWORD QuestSetRebornMap(UINT16 u16QuestID, DWORD &dwBornMapID, Vector3 &vBornPos);

	//---------------------------------------------------------------------------------
	// 添加，删除和改变技能技能
	//---------------------------------------------------------------------------------
	virtual VOID AddSkill(Skill* pSkill, BOOL bSendMsg=TRUE)
	{
		Unit::AddSkill(pSkill);

		AddSpecSkill(pSkill);

		Map *pMap = GetMap();
		if(P_VALID(pMap))
			UpdateSkillByMap(pSkill->GetID(),pMap->GetMapType(),pMap->GetMapID());

		if( bSendMsg )
		{
			// 发送给自己添加一个技能
			tagNS_AddSkill send;
			pSkill->GenerateMsgInfo(&send.Skill);
			SendMessage(&send, send.dwSize);

			// 保存到数据库
			if(!pSkill->IsExtraSkill())
			{
				tagNDBC_AddSkill send1;
				send1.dwRoleID = GetID();
				send1.Skill.dwID = pSkill->GetID();
				send1.Skill.nSelfLevel = pSkill->GetSelfLevel();
				send1.Skill.nLearnLevel = pSkill->GetLearnLevel();
				send1.Skill.nProficiency = pSkill->GetProficiency();
				send1.Skill.nCoolDown = pSkill->GetCoolDownCountDown();
				g_dbSession.Send(&send1, send1.dwSize);
			}
		}
	};

	virtual VOID RemoveSkill(DWORD dwSkillID)
	{
		BOOL bExtra = FALSE;
		Skill* pSkill = GetSkill(dwSkillID);
		if(P_VALID(pSkill)) bExtra = pSkill->IsExtraSkill();

		if(P_VALID(pSkill))
			Unit::RemoveSkill(dwSkillID,pSkill->GetCanUseByMap());
		else
			Unit::RemoveSkill(dwSkillID);

		RemoveSpecSkill(dwSkillID);

		// 发送给自己删除一个技能
		tagNS_RemoveSkill send;
		send.dwSkillID = dwSkillID;
		SendMessage(&send, send.dwSize);

		// 发送到数据库删除
		if(!bExtra)
		{
			tagNDBC_RemoveSkill send1;
			send1.dwRoleID = GetID();
			send1.dwSkillID = dwSkillID;
			g_dbSession.Send(&send1, send1.dwSize);
		}
	}

	virtual VOID ChangeSkillLevel(Skill* pSkill, ESkillLevelChange eType, INT nChange=1)
	{
		INT nLstLevel = pSkill->GetLevel();
		Unit::ChangeSkillLevel(pSkill, eType, nChange);

		// 发送给自己更新一个技能
		tagNS_UpdateSkill send;
		pSkill->GenerateMsgInfo(&send.Skill);
		SendMessage(&send, send.dwSize);

		if( pSkill->GetLevel() != nLstLevel && !pSkill->IsExtraSkill() )
		{
			tagNDBC_UpdateSkill send1;
			send1.dwRoleID = GetID();
			send1.Skill.dwID = pSkill->GetID();
			send1.Skill.nSelfLevel = pSkill->GetSelfLevel();
			send1.Skill.nLearnLevel = pSkill->GetLearnLevel();
			send1.Skill.nProficiency = pSkill->GetProficiency();
			send1.Skill.nCoolDown = pSkill->GetCoolDownCountDown();
			g_dbSession.Send(&send1, send1.dwSize);
		}
	}

	virtual VOID ChangeSkillExp(Skill *pSkill, INT nValue)
	{
		Unit::ChangeSkillExp(pSkill, nValue);
		
		// 发送给自己更新一个技能
		tagNS_UpdateSkill send;
		pSkill->GenerateMsgInfo(&send.Skill);
		SendMessage(&send, send.dwSize);
	}

	virtual VOID StartSkillCoolDown(Skill* pSkill);

	virtual VOID ClearSkillCoolDown(DWORD dwSkillID);

	//--------------------------------------------------------------------------------
	// 属性改变引起的其它内容改变
	//--------------------------------------------------------------------------------
	virtual VOID OnAttChange(INT nIndex);
	virtual VOID OnAttChange(bool bRecalFlag[ERA_End]);

	//--------------------------------------------------------------------------------
	// 获得或减少经验
	//--------------------------------------------------------------------------------
	VOID ExpChange(INT64 nValue, BOOL bKill=FALSE, BOOL bForce=FALSE,BOOL bShare = FALSE);

	//--------------------------------------------------------------------------------
	// 改变等级
	//--------------------------------------------------------------------------------
	VOID LevelChange(INT nValue, BOOL bKill=FALSE, BOOL bForce=FALSE);

	//--------------------------------------------------------------------------------
	// 设置套装特效
	//--------------------------------------------------------------------------------
	VOID SetSuitEffect(DWORD dwSuitEffect)
	{ 
		m_AvatarEquipEquip.dwSuitEffectID = dwSuitEffect;
		m_AvatarEquipFashion.dwSuitEffectID = dwSuitEffect;
		m_AvatarEquipWardrobe.dwSuitEffectID = dwSuitEffect;
	}

	//--------------------------------------------------------------------------------
	// 换装相关
	//--------------------------------------------------------------------------------
	VOID ProcEquipEffect(tagEquip* pNewEquip, tagEquip* pOldEquip, INT16 n16IndexOld, BOOL bDiscard, BOOL bSend, BOOL byFamily = FALSE  );
	INT32 GetEquipDisplayPos(INT16 n16EquipPos);
	DWORD Equip(INT64 n64Serial, EEquipPos ePosDst);
	VOID  ResetWeaponDmg(tagEquip& Equip);				// 崭新度变化时调用
    
	//--------------------------------------------------------------------------------
	// 获得装备后自动穿，并切换显示模式
	//--------------------------------------------------------------------------------
	DWORD  EquipAutoDress(tagItem* pNewItem);
	BOOL  RewardEquip(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT32 nNum, EItemQuality eQlty, DWORD dwCmdID);

	//--------------------------------------------------------------------------------
	// 角色采集技能加成
	//--------------------------------------------------------------------------------
	INT CalGatherRate( Creature* pCreature );

	//--------------------------------------------------------------------------------
	// 角色实际打怪获得经验计算
	//--------------------------------------------------------------------------------
	INT64 CalRealGainExp( INT64 nSrcExp );

	//----------------------------------------------------------------------------------
	// 根据索引得到好友列表的相应位置
	//----------------------------------------------------------------------------------
	tagFriend GetFriend(INT nIndex)
	{
		ASSERT(nIndex < MAX_FILENAME && nIndex >= 0);
		return m_Friend[nIndex];
	}

	VOID SetFriend(INT nIndex, DWORD dwFriendID, DWORD dwFriVal = 0, BYTE byGroup = 1)
	{
		ASSERT(nIndex < MAX_FRIENDNUM && nIndex >= 0);
		ASSERT(byGroup < 5 && byGroup > 0);

		if(m_Friend[nIndex].dwFriendID == GT_INVALID && dwFriendID != GT_INVALID)
		{
			m_mapFriend.Add(dwFriendID, &m_Friend[nIndex]);
		}
		else if(m_Friend[nIndex].dwFriendID != GT_INVALID && dwFriendID == GT_INVALID)
		{
			m_mapFriend.Erase(m_Friend[nIndex].dwFriendID);
		}

		m_Friend[nIndex].dwFriendID = dwFriendID; 
		if(dwFriendID == GT_INVALID)
			m_Friend[nIndex].dwFriVal = 0;
		else
			m_Friend[nIndex].dwFriVal += dwFriVal;	
		m_Friend[nIndex].byGroup = byGroup;

		m_Friend[nIndex].dwFriVal = (m_Friend[nIndex].dwFriVal > MAX_FRIENDVAL) ? MAX_FRIENDVAL : m_Friend[nIndex].dwFriVal;
	}

	tagFriend* GetFriendPtr(DWORD dwFriendID) { return m_mapFriend.Peek(dwFriendID); }

	//----------------------------------------------------------------------------------
	// 根据索引得到黑名单的相应位置
	//----------------------------------------------------------------------------------
	DWORD GetBlackList(INT nIndex)
	{
		ASSERT(nIndex < MAX_BLACKLIST && nIndex >= 0);
		return m_dwBlackList[nIndex];
	}

	VOID SetBlackList(INT nIndex, DWORD dwRoleID)
	{
		ASSERT(nIndex < MAX_BLACKLIST && nIndex >= 0);
		m_dwBlackList[nIndex] = dwRoleID;
	}

	//----------------------------------------------------------------------------------
	// 根据索引得到仇敌名单的相应位置
	//----------------------------------------------------------------------------------
	DWORD GetEnemyList(INT nIndex)
	{
		ASSERT(nIndex < MAX_ENEMYNUM && nIndex >= 0);
		return m_dwEnemyList[nIndex];
	}

	VOID SetEnemyList(INT nIndex, DWORD dwRoleID)
	{
		ASSERT(nIndex < MAX_ENEMYNUM && nIndex >= 0);
		m_dwEnemyList[nIndex] = dwRoleID;
	}

	//----------------------------------------------------------------------------------
	// 设置小队和团队ID
	//----------------------------------------------------------------------------------
	VOID SetTeamID(DWORD dwTeamID)		{ m_dwTeamID = dwTeamID; }
	VOID SetGroupID(DWORD dwGroupID)	{ m_dwGroupID = dwGroupID; }
	VOID SetTeamInvite(DWORD dwRoleID)	{ m_dwTeamInvite = dwRoleID; }

	BOOL IsTeamMate(Role* pRole)		{ return GetTeamID() != GT_INVALID && GetTeamID() == pRole->GetTeamID(); }
	BOOL IsGuildMate(Role* pRole)		{ return GetGuildID() != GT_INVALID && GetGuildID() == pRole->GetGuildID(); }
	BOOL IsLover(Role* pRole)			{ return GetLoverID() != GT_INVALID && GetLoverID() == pRole->GetID(); }
	BOOL IsMasterApprentice(Role* pRole)	{ return ( ( (GetMasterID() != GT_INVALID) && (GetMasterID() == pRole->GetID()) ) || 
															((pRole->GetMasterID() != GT_INVALID) && (pRole->GetMasterID() == GetID())) ); }
	BOOL IsMasterMate(Role* pRole)		{ return GetMasterID() != GT_INVALID && GetMasterID() == pRole->GetMasterID(); }
	BOOL IsFamilyMate(Role* pRole)		{ return GetFamilyID() != GT_INVALID && GetFamilyID() == pRole->GetFamilyID(); }

	//----------------------------------------------------------------------------------
	// 与某个单位是不是友好的社会关系（队友，结拜，师徒，夫妻，同门等）
	//----------------------------------------------------------------------------------
	BOOL IsFriendlySocial(Role* pRole)	{ return IsTeamMate(pRole) || IsGuildMate(pRole) || IsLover(pRole) || IsMasterApprentice(pRole) || IsMasterMate(pRole) || IsFamilyMate(pRole); }

	//----------------------------------------------------------------------------------
	// 副本
	//----------------------------------------------------------------------------------
	DWORD		GetOwnInstanceID()	const;
	DWORD		GetOwnInstanceMapID() const;
	DWORD		GetMyOwnInstanceID() const			{ return m_dwOwnInstanceID; }
	DWORD		GetMyOwnInstanceMapID() const		{ return m_dwOwnInstanceMapID; }
	DWORD		GetExportMapID() const				{ return m_dwExportMapID; }
	Vector3&	GetExportPoint()					{ return m_vExport; }
	VOID		SetMyOwnInstanceID(DWORD dwID)		{ m_dwOwnInstanceID = dwID; }
	VOID		SetMyOwnInstanceMapID(DWORD dwID)	{ m_dwOwnInstanceMapID = dwID; }
	VOID		SetExportMapID(DWORD dwMapID)		{ m_dwExportMapID = dwMapID; }
	VOID		SetExportPoint(Vector3 &vExport)	{ m_vExport = vExport; }

	//----------------------------------------------------------------------------------
	// 商城相关
	//----------------------------------------------------------------------------------
	tagDWORDTime GetLastMallFreeTime() const { return m_LastGetMallFreeTime; }

	VOID SetLastGetMallFreeTime(DWORD dwNewLastGetTime) { m_LastGetMallFreeTime = dwNewLastGetTime; }
	
	//----------------------------------------------------------------------------------
	// 主动技能状态触发
	//----------------------------------------------------------------------------------
	BOOL OnActiveItemBuffTrigger(tagItem* pItem, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);

	//----------------------------------------------------------------------------------
	// 被动技能状态触发
	//----------------------------------------------------------------------------------
	BOOL OnPassiveSkillBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);

	//----------------------------------------------------------------------------------
	// 装备被动触发
	//----------------------------------------------------------------------------------
	BOOL OnEquipmentBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, DWORD dwEventMisc1=GT_INVALID, DWORD dwEventMisc2=GT_INVALID);

	//----------------------------------------------------------------------------------
	// 注册触发器技能关联表
	//----------------------------------------------------------------------------------
	VOID RegisterTriggerSkillSet(ETriggerEventType eEvent, DWORD dwSkillID);

	//----------------------------------------------------------------------------------
	// 反注册触发器技能关联表
	//----------------------------------------------------------------------------------
	VOID UnRegisterTriggerSkillSet(ETriggerEventType eType, DWORD dwSkillID);

	//----------------------------------------------------------------------------
	// 注册触发器装备关联表
	//----------------------------------------------------------------------------
	VOID RegisterTriggerEquipSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, true);
		if( EPSAET_Null == eTriggerType ) return;

		if(EEP_Null == n16EquipPos)
			return;

		m_bitsetEquipTrigger[eTriggerType][n16EquipPos] = true;
	}

	//----------------------------------------------------------------------------
	// 反注册触发器装备关联表
	//----------------------------------------------------------------------------
	VOID UnRegisterTriggerEquipSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, false);
		if( EPSAET_Null == eTriggerType ) return;

		if(EEP_Null == n16EquipPos)
			return;

		m_bitsetEquipTrigger[eTriggerType][n16EquipPos] = false;
	}

	//----------------------------------------------------------------------------
	// 注册触发器龙魂关联表
	//----------------------------------------------------------------------------
	VOID RegisterTriggerLongHunSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, true);
		if( EPSAET_Null == eTriggerType ) return;

		if(EEP_Null == n16EquipPos)
			return;

		m_bitsetLongHunTrigger[eTriggerType][n16EquipPos] = true;
	}

	//----------------------------------------------------------------------------
	// 反注册触发器龙魂关联表
	//----------------------------------------------------------------------------
	VOID UnRegisterTriggerLongHunSet(DWORD dwTriggerID, DWORD dwBuffID, INT16 n16EquipPos)
	{
		ASSERT(MIsInEquipPos(n16EquipPos));

		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, false);
		if( EPSAET_Null == eTriggerType ) return;

		if(EEP_Null == n16EquipPos)
			return;

		m_bitsetLongHunTrigger[eTriggerType][n16EquipPos] = false;
	}

	//----------------------------------------------------------------------------------
	// 注册触发器套装关联表
	//----------------------------------------------------------------------------------
	VOID RegisterTriggerSuitSet(DWORD dwTriggerID, DWORD dwBuffID, DWORD dwSuitID)
	{
		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, true);
		if( EPSAET_Null == eTriggerType ) return;

		m_setSuitTrigger[eTriggerType].insert(dwSuitID);
	}

	//----------------------------------------------------------------------------------
	// 反注册触发器套装关联表
	//----------------------------------------------------------------------------------
	VOID UnRegisterTriggerSuitSet(DWORD dwTriggerID, DWORD dwBuffID, DWORD dwSuitID)
	{
		if( GT_INVALID == dwTriggerID || GT_INVALID == dwBuffID )
		{
			return;
		}

		EPassiveSkillAndEquipTrigger eTriggerType = PreRegisterTriggerEquipSet(dwTriggerID, dwBuffID, false);
		if( EPSAET_Null == eTriggerType ) return;

		m_setSuitTrigger[eTriggerType].erase(dwSuitID);
	}
	
	//----------------------------------------------------------------------------------
	// 夫妻相关
	//----------------------------------------------------------------------------------
	VOID	UpdateLoverID(DWORD dwLoverID)
	{ 
		m_dwLoverID = dwLoverID; 
		if(P_VALID(g_roleMgr.GetRoleInfo(GetID())))
			g_roleMgr.GetRoleInfo(GetID())->dwLoverID = dwLoverID;
	}
	DWORD	GetLoverID() { return m_dwLoverID; }

	BOOL IsRoleMarried() { return (m_dwLoverID != GT_INVALID) ? TRUE:FALSE ; } //是否已婚
	BOOL IsRoleWedded() { return m_bHaveWedding; } //是否举行过婚礼
	

private:
	//----------------------------------------------------------------------------------
	// 装备相关buff预处理，并获取tigger类型
	//----------------------------------------------------------------------------------
	EPassiveSkillAndEquipTrigger PreRegisterTriggerEquipSet(DWORD dwTriggerID, DWORD dwBuffID, BOOL bEquip);

	//----------------------------------------------------------------------------------
	// 装备相关被动trigger触发处理
	//----------------------------------------------------------------------------------
	VOID OnEquipmentBuffTriggerCommon(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType);
	VOID OnEquipmentBuffTriggerLongHun(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType);
	VOID OnEquipmentBuffTriggerSuit(ETriggerEventType eEvent, EPassiveSkillAndEquipTrigger eTriggerType);

private:
	//------------------------------------------------------------------------------
	// 发送客户端初始属性
	//------------------------------------------------------------------------------
	VOID SendInitPersonalSet();
	VOID SendInitStateAtt();
	VOID SendInitStateSkill();
	VOID SendInitStateItem();	// 物品&装备
	VOID SendInitStateSuit();	// 套装
	VOID SendInitStateLongHun();
	VOID SendInitStateCompleteQuest();
	VOID SendInitStateDailyCompleteQuest();
	VOID SendInitStateIncompleteQuest();
	VOID SendInitStateMoney();
	VOID SendInitStateReputation();
	VOID SendFriendAndEnemy();
	VOID SendInitStateGuild();				// 帮派
	VOID SendFatigueGuardInfo(BYTE byCode);	// 防沉迷
	VOID SendInitStateCity();
	VOID SendInitHolySoulInfo();

	//------------------------------------------------------------------------------
	// 初始化从数据库中读取的列表数据
	//------------------------------------------------------------------------------
	VOID InitAtt(const tagRoleDataSave* pData);
	VOID InitSkill(const BYTE* &pData, INT32 nNum);
	VOID InitBuff(const BYTE* &pData, INT32 nNum);
	VOID InitItem(const BYTE* &pData, INT32 nNum, INT32 nOffMinute);
	VOID InitItemCDTime(const BYTE* &pData, INT32 nNum);
	VOID InitFriend(const BYTE* &pData, INT32 nNum);
	VOID InitEnemy(const BYTE* &pData, INT32 nNum);	// 仇敌列表相关消息
	VOID InitFriendValue(const BYTE* &pData, INT32 nNum);
	VOID InitBlackList(const BYTE* &pData, INT32 nNum);
	VOID InitMaster(const BYTE* &pData);
	VOID InitClassMates(const BYTE* &pData);
	VOID InitStrength();
	VOID InitHolySoulInfo(const BYTE* &pData,INT nOffHour/*离线小时*/);

	DWORD Put2Container(tagItem* pItem);

	VOID CalInitAtt();

	VOID CalInitState();

	//------------------------------------------------------------------------------
	// 初始化需保存数据库的数据
	//------------------------------------------------------------------------------
	VOID SaveSkill2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID SaveBuff2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID SaveQuest2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID SaveMaster2DB(IN LPVOID pData, OUT LPVOID &pOutPointer);
	VOID SaveBoardQuest2DB(IN LPVOID pData, OUT LPVOID &pOutPointer);	
	VOID SaveHolySoulInfo(IN LPVOID pData, OUT LPVOID &pOutPointer);

	//------------------------------------------------------------------------------
	// 重置保存到数据库时间倒计时
	//------------------------------------------------------------------------------
	VOID ResetNeedSave2DBCD() { m_nNeedSave2DBCountDown = MIN_ROLE_SAVE2DB_INTERVAL; }

	//------------------------------------------------------------------------------
	// 复活
	//------------------------------------------------------------------------------
	DWORD SetRebornMap(DWORD dwNPCID, DWORD &dwBornMapID, Vector3 &vBornPos);
	DWORD Revive(ERoleReviveType eType, INT64 n64ItemID, BOOL bNeedItem=TRUE);

	DWORD CityRevive();
	DWORD LocusRevive(INT64 n64ItemID, BOOL bNeedItem=TRUE);
	DWORD AcceptRevive();
	DWORD PrisonRevive();
	DWORD PerfectRevive();
	DWORD IsWarRevive();	//战场复活
	//------------------------------------------------------------------------------
	// 换装
	//------------------------------------------------------------------------------
	DWORD	CanEquip(tagEquip* pEquip, EEquipPos ePosDst);
	DWORD	Unequip(INT64 n64Serial, INT16 n16IndexDst);
	DWORD	SwapWeapon();
	DWORD	MoveRing(INT64 n64SerialSrc, INT16 n16PosDst);
public:
	VOID	ProcEquipEffectPos(tagEquip* pNewEquip, tagEquip* pOldEquip, EItemConType eConTypeNewDst, EItemConType eConTypeNewSrc);
	//VOID	RecalAttEx(BOOL bSend);
	VOID	ActiveHolySoul();
	VOID	ProcEquipEffectAtt(tagEquip* pEquip, bool bEquip, const INT16 n16Index, bool bIgnoreWar = false, bool bRecal = false);
private:
	VOID	ProcEquipEffectAvatar(tagEquip* pNewEquip, INT16 n16IndexOld);

public:
	VOID	RecalEquipEffectAttForWar(bool bEnterWarMap);
	INT32	ResetOneEquipDisplay(tagEquip* pEquip, INT16 n16Index);
	//或得战场物品，重新计算装备属性
	VOID	GetWarItemRecalEffect(INT16 nIndex, bool bGet);

public:
	//计算绑定和非绑定老君仙露需要消耗个数
	//VOID	CalculateItemCostNum(DWORD dwTypeID, TList<tagItem*>& itemListBind,TList<tagItem*>& itemListUnBind,INT& nLjxlNum ,INT& nItemBindNum, INT& nItemUnBindNum);

private:
	VOID	ChangeRoleAtt(const tagRoleAttEffect Effect[], INT32 nArraySz, INT32 nFactor);
	VOID	ChangeRoleAtt(const INT32 nValue[], INT32 nArraySz, INT32 nAttStart, INT32 nFactor, bool bPctIgnore=false);
	VOID	ChangeRoleAtt(const DWORD dwValue[], INT32 nArraySz, INT32 nFactor, BYTE nDiamondAddLevel=0);	//添加宝石增幅相关，加一个参数宝石增幅强度
	VOID	ChangeRoleAtt(const INT32 nValue[], INT32 nArraySz, INT32 nArrayStartIdx, INT32 nAttStart, INT32 nFactor);
	VOID	ProcEquipBuffTrigger(DWORD dwBuffID, BOOL bEquip);
	VOID	ProcSoulCrystalAttEffect(tagEquip* pEquip, bool bEquip);
	//------------------------------------------------------------------------------
	// 武器伤害相关计算
	//------------------------------------------------------------------------------
	/* 计算磨损对武器伤害影响.返回值为对武器伤害影响的百分比值.*/
	FLOAT	CalAbrasionEffect(const tagEquip& Equip);
	VOID	ResetWeaponDmg(const tagEquip& Equip, BOOL bEquip);	// 换装时调用

	//------------------------------------------------------------------------------
	// 玩家间交易相关
	//------------------------------------------------------------------------------
	ExchangeMgr& GetExchMgr() { return m_ExchMgr; }
	
	BOOL	IsExchanging()					{ return IsInRoleState(ERS_Exchange); }
	BOOL	CanExchange()
	{
		return !IsInRoleStateAny(ERS_Exchange | ERS_Shop | ERS_Stall | ERS_Wedding) 
				&& (GetExchMgr().GetTgtRoleID() == GT_INVALID);
	}
	
	VOID	BeginExchange(DWORD dwTgtRoleID)
	{
		SetRoleState(ERS_Exchange);
		GetExchMgr().CreateData();
		GetExchMgr().SetTgtRoleID(dwTgtRoleID);
	}

	VOID	EndExchange()
	{
		UnsetRoleState(ERS_Exchange);
		GetExchMgr().DeleteData();
		GetExchMgr().SetTgtRoleID(GT_INVALID);
	}

	DWORD	ProcExchangeReq(OUT Role* &pTarget, DWORD dwTgtRoleID);
	DWORD	ProcExchangeReqRes(OUT Role* &pApplicant, DWORD dwTgtRoleID, DWORD dwErrorCode);
	DWORD	ProcExchangeAdd(OUT Role* &pTarget, OUT tagItem* &pItem, INT32 &nInsIndex, INT64 n64Serial);
	DWORD	ProcExchangeDec(OUT Role* &pTarget, INT64 n64Serial);
	DWORD	ProcExchangeMoney(OUT Role* &pTarget, INT64 n64Silver);
	DWORD	ProcExchangeLock(OUT Role* &pTarget);
	DWORD	ProcExchangeCancel(OUT Role* &pTarget);
	DWORD	ProcExchangeVerify(OUT Role* &pTarget, OUT DWORD &dwFailedRoleID);

	DWORD	VerifyExchangeData(OUT tagItem* pItem[]);
	DWORD	ProcExchange();

	DWORD	ProcPrepareExchange(OUT Role* &pTarget);
	public:
	VOID    ClenupExchState();	
	private:


	//---------------------------------------------------------------------------------
	// 商店相关
	//---------------------------------------------------------------------------------
	DWORD	GetShopItems(DWORD dwNPCID, BYTE byShelf, DWORD dwNPCTypeID);
	DWORD	GetShopEquips(DWORD dwNPCID, BYTE byShelf, DWORD dwNPCTypeID);
	DWORD	BuyShopItem(DWORD dwNPCID, BYTE byShelf, DWORD dwTypeID, INT16 n16ItemNum, DWORD dwNPCTypeID);
	DWORD	BuyShopEquip(DWORD dwNPCID, BYTE byShelf, DWORD dwTypeID, INT64 n64Serial, DWORD dwNPCTypeID);
	DWORD	SellToShop(DWORD dwNPCID, INT64 n64Serial, DWORD dwNPCTypeID);

	VOID	SendShopFeedbackMsg(DWORD dwErrorCode, DWORD dwNPCID);

	//-----------------------------------------------------------------------------
	// 商城相关
	//-----------------------------------------------------------------------------
	DWORD	GetMallAll(OUT DWORD &dwMallTime);
	DWORD	UpdateMallAll(OUT DWORD &dwNewMallTime, IN DWORD dwOldMallTime);
	DWORD	BuyMallItem(DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex,EShoppingDistrict eShoppingDistrict, INT16 n16OfferItemLvl, ELogCmdID eCmdID = ELCID_Mall_BuyItem);
	DWORD	BuyMallPack(DWORD dwID, INT nUnitPrice, BYTE byIndex,EShoppingDistrict eShopingDistrict);
	DWORD	BuyMallItem(DWORD dwTgtRoleID, LPCTSTR szLeaveWord, 
							DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex,EShoppingDistrict eShoppingDistrict);
	DWORD	BuyMallPack(DWORD dwTgtRoleID, LPCTSTR szLeaveWord,
							DWORD dwID, INT nUnitPrice, BYTE byIndex,EShoppingDistrict eShoppingDistrict);
	DWORD	GetMallFreeItem(DWORD dwID);
	DWORD	BuyMallCart(DWORD* dwID, INT* nIndex, INT16* n16BuyNum);

	//-----------------------------------------------------------------------------
	// 婚姻相关
	//-----------------------------------------------------------------------------
	//结婚
	DWORD ProcGetMarriageReq(OUT Role* &pTarget, DWORD dwTgtRoleID, DWORD dwNpcID);
	DWORD ProcGetMarriageReqRes(OUT Role* &pApplicant, DWORD dwApplicantRoleID, DWORD dwErrorCode);

	//离婚
	DWORD ProcBreakMarriageReq(OUT Role* &pTarget, DWORD dwTgtRoleID);
	DWORD ProcForceBreakMarriageReq(DWORD dwTgtRoleID);
	DWORD ProcBreakMarriageReqRes(OUT Role* &pApplicant, DWORD dwApplicantRoleID, DWORD dwErrorCode);

	//婚礼
	DWORD ProcCelebrateWeddingReq(OUT Role* &pTarget,OUT Role* &pOfficiatorRole,DWORD dwOfficiatorNameCrc,DWORD dwNpcID);
	DWORD ProcCelebrateWeddingReqRes(OUT Role* &pApplicant, DWORD dwErrorCode);
public:
	DWORD	MallItemExchange(DWORD dwMallID, INT nPrice, INT16 n16BuyNum, INT nIndex);
	DWORD	MallPackExchange(DWORD dwMallID, INT nPrice, INT nIndex);

public:
	DWORD	RoleSetVocation(DWORD dwNPCID, EClassType eVocation);
	DWORD	RoleSetHeroVocation(DWORD dwNPCID, EClassTypeEx eHVocation);
	DWORD	ChangeVocation(DWORD dwNPCID, EClassType eVocation);

private:

	//-----------------------------------------------------------------------------
	// 商城元宝交易相关
	//-----------------------------------------------------------------------------
	DWORD	SaveYB2Account(DWORD dwID, INT nNum);
	DWORD 	SaveSilver2Account(DWORD dwID, INT64 nNum);
	DWORD 	DepositYBAccout(DWORD dwID, INT nNum);
	DWORD 	DepositSilverAccount(DWORD dwID, INT64 nNum);
	DWORD 	GetYBTradeInfo();
	DWORD 	SubmitSellOrder(DWORD dwRoleID, INT nNum, INT nPrice);
	DWORD 	SubmitBuyOrder(DWORD dwRole, INT nNum, INT nPrice);
	DWORD 	DeleteOrder(DWORD dwRoleID, DWORD dwOrderID, EYBOTYPE eYBOType);
	DWORD 	GetYBOrder(DWORD dwRoleID);

public:
	//------------------------------------------------------------------------
	// 摆摊相关
	//------------------------------------------------------------------------
	DWORD	StartStall()
	{
		DWORD dwRtv = m_pStall->Init(0);
		if (E_Success == dwRtv)
		{
			StopMount();
		}
		return dwRtv;//?? 城镇税率
	}

	DWORD	SetStallGoods(INT64 n64Serial, INT64 n64UnitPrice, BYTE byIndex)
	{
		return m_pStall->SetGoods(n64Serial, n64UnitPrice, byIndex);
	}

	DWORD	UnsetStallGoods(const BYTE byIndex)
	{
		return m_pStall->UnSetGoods(byIndex);
	}

	DWORD	SetStallTitle(LPCTSTR strTitle)
	{
		return m_pStall->SetTitle(strTitle);
	}

	DWORD	SetStallAdText(LPCTSTR strAdText)
	{
		return m_pStall->SetAdText(strAdText);
	}

	DWORD	SetStallAdFlag(bool bAdFlag)
	{
		return m_pStall->SetAdFlag(bAdFlag);
	}

	DWORD	SetStallFinish()
	{
		return m_pStall->SetFinish();
	}

	DWORD	CloseStall()
	{
		return m_pStall->Destroy();
	}

	DWORD	GetStallTitle(OUT LPTSTR pSzTitle)
	{
		return m_pStall->GetStallTitle(pSzTitle);
	}

	DWORD	GetStallSpecGoods(BYTE byIndex, OUT LPVOID pData, OUT INT &nGoodsSz)
	{
		return m_pStall->GetSpecGoods(byIndex, pData, nGoodsSz);
	}
	
	DWORD	GetStallGoods(OUT LPVOID pData, OUT BYTE &byGoodsNum, OUT INT &nGoodsSz)
	{
		return m_pStall->GetGoods(pData, byGoodsNum, nGoodsSz);
	}

	DWORD	BuyStallGoods(Role *pBuyer, INT64 n64Serial, INT64 n64UnitPrice, 
							INT16 n16Num, BYTE byIndex, OUT INT16 &n16RemainNum)
	{
		return m_pStall->Sell(pBuyer, n64UnitPrice, n64Serial, byIndex, n16Num, n16RemainNum);
	}

	BYTE	GetStallModeLevel() const
	{
		return m_pStall->GetModeLevel();
	}

	//INT8	GetStallGoodsNum() const
	//{
	//	return m_pStall->GetGoodsNum();
	//}

	BOOL	IsNoGoodsInStall() const
	{
		return m_pStall->IsEmpty();
	}

	BOOL	IsSetGoodsFinish() const
	{
		return IsInRoleState(ERS_Stall);
	}

	INT32	CalStallGoodsMemUsed() const	// 所有摊位上商品存入结构tagMsgStallGoods时的大小
	{
		return m_pStall->CalMemSzGoodsUsed();
	}

	DWORD	SendCloseStall();

	DWORD	GainStallExp(INT32 nExp)
	{
		return m_pStall->GainExp(nExp);
	}

	// 重置疲劳值
	VOID ResetWearyValue()
	{
		SetAttValue(ERA_Weary, 0);
	}

public:
	VOID	CalEquipFlare(tagEquip* pEquip);

private:
	//---------------------------------------------------------------------------------
	// 部分职能NPC功能
	//---------------------------------------------------------------------------------
	DWORD	ProcDak(DWORD dwNPCID, INT32 nIndex, DWORD dwMapID);

	//---------------------------------------------------------------------------------
	// 使用磨石
	//---------------------------------------------------------------------------------
	DWORD	AbraseWeapon(INT64 n64AbraserSerial, INT64 n64WeaponSerial, OUT INT32 &nAttackTimes);

	//---------------------------------------------------------------------------------
	// 地图触发器
	//---------------------------------------------------------------------------------
	BOOL	MapTrigger(DWORD dwTriggerID, DWORD dwMisc);

	//---------------------------------------------------------------------------------
	// 通知队友进入副本
	//---------------------------------------------------------------------------------
	DWORD	InstanceNotify(BOOL bNotify);

	//---------------------------------------------------------------------------------
	// 玩家是否同意其它玩家进入副本的邀请
	//---------------------------------------------------------------------------------
	DWORD	InstanceAgree(BOOL bAgree);

	//---------------------------------------------------------------------------------
	// 玩家请求离开副本
	//---------------------------------------------------------------------------------
	DWORD	LeaveInstance();

	//---------------------------------------------------------------------------------
	// 强化装备
	//---------------------------------------------------------------------------------
	// 铭纹
	DWORD	PosyEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);	
	DWORD	GMPosyEquip(DWORD dwFormulaID, INT16 n16ItemIndex);	
	// 镌刻
	DWORD	EngraveEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	DWORD	GMEngraveEquip(DWORD dwFormulaID, INT16 n16ItemIndex);
	// 镶嵌
	DWORD	InlayEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID);
	// 烙印
	DWORD	BrandEquip(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD dwCmdID);
	// 龙附
	DWORD	LoongSoulEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID);
	DWORD	GMLoongSoulEquip(INT64 n64SrcItemID, INT16 n16ItemIndex);
	// 淬火
	DWORD	QuenchEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	// 开凿
	DWORD	ChiselEquip(INT64 n64SrcItemID, INT64 n64SuffID, DWORD dwCmdID);
	// 时装染色
	DWORD	DyeFashion(INT64 n64DyeSN, INT64 n64EquipSerial, DWORD dwCmdID);
	// 精纺时装
	DWORD	SpinFashion(ESuitAttType eSpinType, INT64 n64EquipSerial, DWORD dwStuffTypeID, DWORD dwStuffItemNum, DWORD dwCmdID);
	// 萃取
	DWORD	Extraction(DWORD dwNPCID, INT64 n64Item);

	//得到精纺阶段
	INT GetSpinStep(BYTE byMinUseLevel);

	//得到精纺装备的品级
	EItemQuality GetSpinItemQuality(ESpinStep eStep);

	// 计算B类属性对强化成功率的影响
	FLOAT	CalSpecAttEffectSuc(EEquipSpecAtt eSpecAtt, FLOAT fProp, EEquipSpecAtt eSpecType);
	// 计算B类属性对强化完美率的影响
	FLOAT	CalSpecAttEffectPef(EEquipSpecAtt eSpecAtt, FLOAT fPerfect);
	// 打开随身仓库
	DWORD	OpenSideWare(INT64 n64ItemSerial);
	
	//---------------------------------------------------------------------------------
	// 生产合成物品
	//---------------------------------------------------------------------------------
	DWORD	ProduceItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	DWORD	ProduceEquip(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID);
	DWORD	CheckProduceLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, const tagProduceProtoSer* &pProduceProto, INT64 n64StuffID[], INT32 nArraySz);
	DWORD	DeComposeItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, INT64 n64IMID, INT64 n64Item, DWORD dwCmdID);
	DWORD	CheckDeComposeLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, const tagDeComposeProtoSer* &pDeComposeProto, INT64 n64Item, tagEquip *pEquip);
	EProduceType Skill2ProduceType(ESkillTypeEx2 eSkillType);	
	tagItem*	 CreateItem(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num, DWORD dwCreator);
	VOID	CalIMEffect(EConsolidateTypeSer eConType, tagIMEffect &IMEffect, INT64 n64IMID, const LPVOID pProto);
	// Jason 2009-12-6 提升装备潜力值道具处理
	DWORD RaiseEquipPotVal(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD & dwPotValAdded);

	// Jason v1.3.1 date:2009-12-21
	DWORD GetGemRemovalInfo(DWORD dwNPCID,INT64 equipID,INT8 & num,DWORD dwGemIDs[MAX_EQUIPHOLE_NUM]);
	DWORD RemoveGemFromEquip(DWORD dwNPCID,INT64 equipID,INT8 num,INT64 SignIDs[MAX_EQUIPHOLE_NUM]);

	//---------------------------------------------------------------------------------
	// 查询某个天资所在的位置（如果没有，返回GT_INVALD）
	//---------------------------------------------------------------------------------
	INT FindTalentIndex(ETalentType eType)
	{
		INT nBegin = X_MAIN_TALENT_START, nEnd = X_FRESHHAND_TALENT;	// 设置起始点和终点

		// 如果是新手系天资，则取后半部分
		if( ETT_FreshHand == eType)
		{
			nBegin	=	X_FRESHHAND_TALENT;
			nEnd	=	X_MAX_TALENT_PER_ROLE;
		}

		// 如果该天资是主天资，则
		for(INT n = nBegin; n < nEnd; n++)
		{
			if( m_Talent[n].eType == eType )
				return n;
		}

		return GT_INVALID;
	}

	//---------------------------------------------------------------------------------
	// 找到某个可插入的天资位置（如果没有，返回GT_INVALID）
	//---------------------------------------------------------------------------------
	INT FindBlankTalentIndex(ETalentType eType)
	{
		INT nBegin = X_MAIN_TALENT_START, nEnd = X_FRESHHAND_TALENT;	// 设置起始点和终点

		// 如果是新手系天资，则取后半部分
		if( ETT_FreshHand == eType )
		{
			nBegin	=	X_FRESHHAND_TALENT;
			nEnd	=	X_MAX_TALENT_PER_ROLE;
		}

		for(INT n = nBegin; n < nEnd; n++)
		{
			if( ETT_Null == m_Talent[n].eType )
				return n;
		}

		return GT_INVALID;
	}

	//---------------------------------------------------------------------------------
	// 增加一个天资
	//---------------------------------------------------------------------------------
	VOID AddTalent(INT nIndex, ETalentType eType, INT nPoint=1)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null == m_Talent[nIndex].eType );
		ASSERT( nPoint > 0 );

		m_Talent[nIndex].eType = eType;
		m_Talent[nIndex].nPoint = nPoint;

		// 同步
		tagNS_AddTalent send;
		send.eType = eType;
		send.nPoint = nPoint;
		SendMessage(&send, send.dwSize);
	}

	//---------------------------------------------------------------------------------
	// 删除一个天资
	//---------------------------------------------------------------------------------
	VOID RemoveTalent(INT nIndex)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null != m_Talent[nIndex].eType );
		
		ETalentType eType = m_Talent[nIndex].eType;

		m_Talent[nIndex].eType = ETT_Null;
		m_Talent[nIndex].nPoint = 0;

		// 同步
		tagNS_RemoveTalent send;
		send.eType = eType;
		SendMessage(&send, send.dwSize);
	}


	//---------------------------------------------------------------------------------
	// 得到某个索引所对应的天资当前投点数
	//---------------------------------------------------------------------------------
	INT GetTalentPoint(INT nIndex)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null != m_Talent[nIndex].eType );

		return m_Talent[nIndex].nPoint;
	}
    
	//---------------------------------------------------------------------------------
	// 得到获取当前已经投了总天资数
	//---------------------------------------------------------------------------------
	INT GetTotalTalentPoint()
	{
		INT nTotalTalentPoint = 0;
		for (INT nIndex = 0 ; nIndex < X_MAX_TALENT_PER_ROLE; nIndex++)
		{
			if ( m_Talent[nIndex].eType != ETT_Null)
			{
				nTotalTalentPoint += m_Talent[nIndex].nPoint ;
			}
			
		}
        return nTotalTalentPoint;
	}
	//---------------------------------------------------------------------------------
	// 给天资加点
	//---------------------------------------------------------------------------------
	VOID AddTalentPoint(INT nIndex, INT nPoint=1)
	{
		ASSERT( nIndex >= 0 && nIndex < X_MAX_TALENT_PER_ROLE );
		ASSERT( ETT_Null != m_Talent[nIndex].eType );
		ASSERT( nPoint > 0 );

		m_Talent[nIndex].nPoint += nPoint;

		// 同步
		tagNS_UpdateTalent send;
		send.eType = m_Talent[nIndex].eType;
		send.nPoint = m_Talent[nIndex].nPoint;
		SendMessage(&send, send.dwSize);
	}

	//---------------------------------------------------------------------------------
	// 被攻击
	//---------------------------------------------------------------------------------
	virtual VOID OnBeAttacked(Unit* pSrc, Skill* pSkill, BOOL bHited, BOOL bBlock, BOOL bCrited);

	//---------------------------------------------------------------------------------
	// 死亡
	//---------------------------------------------------------------------------------
	virtual VOID OnDead(Unit* pSrc, Skill* pSkill=NULL, const tagBuffProto* pBuff=NULL, DWORD dwSerial=GT_INVALID, DWORD dwMisc=0);

	//---------------------------------------------------------------------------------
	// 击杀
	//---------------------------------------------------------------------------------
	virtual VOID OnKill(Unit* pSrc);

	//---------------------------------------------------------------------------------
	// 是否进行死亡惩罚
	//---------------------------------------------------------------------------------
	BOOL IsDeadPenalty(EDeadPenaltyMode eMode);

	//---------------------------------------------------------------------------------
	// 死亡惩罚
	//---------------------------------------------------------------------------------
	VOID DeadPenalty(Unit* pSrc, EDeadPenaltyMode eMode);

	//---------------------------------------------------------------------------------
	// 是否能免除死亡惩罚
	//---------------------------------------------------------------------------------
	VOID CanCancelPenalty(Unit* pSrc, BOOL& bEquip, BOOL& bExp, BOOL& bInjury, BOOL& bHostility, BOOL& bCommercePenalty);

	//---------------------------------------------------------------------------------
	// 目标的类型标志
	//---------------------------------------------------------------------------------
	virtual DWORD TargetTypeFlag(Unit* pTarget);

	//---------------------------------------------------------------------------------
	// 状态标志
	//---------------------------------------------------------------------------------
	virtual DWORD GetStateFlag()
	{
		DWORD dwStateFlag = Unit::GetStateFlag();

		dwStateFlag |= ( IsInRoleState(ERS_Mount)		?	ESF_Mount	:	ESF_NoMount );
		dwStateFlag |= ( IsInRoleState(ERS_PrisonArea)	?	ESF_Prison	:	ESF_NoPrison );
		dwStateFlag |= ( IsInRoleState(ERS_Commerce)	?	ESF_Commerce:	ESF_NoCommerce );
		//dwStateFlag |= ( IsInRoleState(ERS_Transform)	?	ESF_Transform:	ESF_NoTransform	);

		return dwStateFlag;
	}

	//---------------------------------------------------------------------------------
	// 与目标的敌友中立判断
	//---------------------------------------------------------------------------------
	virtual DWORD FriendEnemy(Unit* pTarget);

	//----------------------------------------------------------------------------------
	// 触发器类型对应被动技能和装备触发器类型
	//----------------------------------------------------------------------------------
	EPassiveSkillAndEquipTrigger TriggerTypeToPassiveSkillAndEquipTriggerType(ETriggerEventType eType)
	{   
		switch(eType)
		{
		case ETEE_BeAttacked:
			return EPSAET_BeAttacked;
			break;

		case ETEE_Hit:
			return EPSAET_Hit;
			break;

		case ETEE_Hited:
			return EPSAET_Hited;
			break;

		case ETEE_Dodged:
			return EPSAET_Dodged;
			break;

		case ETEE_Dodge:
			return EPSAET_Dodge;
			break;

		case ETEE_Blocked:
			return EPSAET_Blocked;
			break;

		case ETEE_Block:
			return EPSAET_Block;

		case ETEE_Crit:
			return EPSAET_Crit;
			break;

		case ETEE_Crited:
			return EPSAET_Crited;
			break;

		case ETEE_Die:
			return EPSAET_Die;
			break;

		case ETEE_Random:
			return EPSAET_Random;
			break;

		case ETEE_Attack:
			return EPSAET_Attack;
			break;

		case ETEE_Kill:
			return EPSAET_Kill;

		default:
			return EPSAET_Null;
			break;
		}
	}
	
private:
	DWORD TargetTypeFlag(Role* pTarget);
	DWORD TargetTypeFlag(Creature* pTarget);
	DWORD FriendEnemy(Role* pTarget);
	DWORD FriendEnemy(Creature* pCreature);

private:
	//---------------------------------------------------------------------------------
	// 将角色数据保存到数据库相关
	//---------------------------------------------------------------------------------
	class SaveRole
	{
	public:
		SaveRole();
		~SaveRole();

		VOID Init();
		operator tagNDBC_SaveRole*() { return m_pSaveRole; }

	private:
		tagNDBC_SaveRole*	m_pSaveRole;
	};

	static SaveRole	m_SaveRole;
	static Mutex	m_SaveRoleLock;
	
	//---------------------------------------------------------------------------------
	// 复活相关
	//---------------------------------------------------------------------------------
	struct tagRevive
	{
		INT		nHP;
		INT		nMP;

		DWORD	dwMapID;
		FLOAT	fX;
		FLOAT	fY;
		FLOAT	fZ;

		tagRevive() { ZeroMemory(this, sizeof(*this)); }
	};

	tagRevive m_Revive;

private:
	//---------------------------------------------------------------------------------
	// 对应的session
	//---------------------------------------------------------------------------------
	PlayerSession*				m_pSession;

protected:
	//-------------------------------------------------------------------------------------
	// 时间相关
	//-------------------------------------------------------------------------------------
	tagDWORDTime		m_CreatTime;						// 创建时间
	tagDWORDTime		m_LoginTime;						// 登陆时间
	tagDWORDTime		m_LogoutTime;						// 登出时间
	INT					m_nOnlineTime;						// 累计在线时间（秒）
	INT					m_nCurOnlineTime;					// 本次在线时间（秒）

	tagDWORDTime		m_dwLastUpgrade;					// 最近一次升级时间

	tagDWORDTime		m_CloseSafeGuardTime;				// 上一次关闭PK保护的时间
	INT					m_nUnSetPKTickCountDown;			// 关闭玩家行凶状态的Tick倒计时
	INT					m_nUnSetPVPTickCountDown;			// 关闭玩家PVP状态的Tick倒计时
	INT					m_nNeedSave2DBCountDown;			// 可以保存数据库倒计时
	INT					m_nDeadUnSetSafeGuardCountDown;		// 关闭玩家戾气为0时回城复活所加上的pk保护倒计tick

	DWORD				m_dwItemTransportMapID;				//当前记录传送符绑定地图ID
	FLOAT				m_fItemTransportX;					//当前记录传送符绑定的x坐标
	FLOAT				m_fItemTransportZ;					//当前记录传送符绑定的z坐标
	FLOAT				m_fItemTransportY;					//当前记录传送符绑定的y坐标

	tagDWORDTime		m_LastGetMallFreeTime;				// 上一次从商城获取免费物品时间

	//-------------------------------------------------------------------------------------
	// 玩家当前状态
	//-------------------------------------------------------------------------------------
	RoleState			m_RoleState;						// 玩家状态	-- 变化后，需通知周围玩家
	RoleStateEx			m_RoleStateEx;						// 玩家状态 -- 变化后，只需要通知自己
	ERolePKState		m_ePKState;							// 当前PK状态

	//-------------------------------------------------------------------------------------
	// 人物属性
	//-------------------------------------------------------------------------------------
	tagAvatarAtt		m_Avatar;							// 外观属性
	tagAvatarEquip		m_AvatarEquipEquip;					// 装备外观
	tagAvatarEquip		m_AvatarEquipFashion;				// 时装外观
	tagAvatarEquip		m_AvatarEquipWardrobe;			// 衣橱外观
	tagDisplaySet		m_DisplaySet;						// 装备显示设置
	EClassType			m_eClass;							// 职业
	EClassTypeEx		m_eClassEx;							// 扩展职业
	INT					m_nCredit;							// 信用度
	INT					m_nIdentity;						// 身份
	INT					m_nVIPPoint;						// 会员积分
	DWORD				m_dwGuildID;						// 所属帮派ID
	DWORD				m_dwFamilyID;						// 所属家族ID
	DWORD				m_dwLoverID;						// 爱人对应的roleid 没结婚就是GT_INVALID
	BOOL				m_bHaveWedding;						// 是否举行过婚礼
	BOOL				m_bNeedPrisonRevive;				// 是否需要牢狱复活

	INT16				m_n16MiraclePracticeNum;			// 仙界修行次数
	tagDWORDTime		m_dwMiracleResetTime;				// 仙界修行点数重置时间

	DWORD				m_dwMasterID;						// 师傅ID
	tagApprenticeData	m_ApprenticeData;					// 徒弟数据
	INT					m_nJingWuPoint;						// 精武点数
	INT					m_nQinWuPoint;						// 勤武点数
	INT					m_nLevelUpBonus;					// 徒弟升级积分
	INT					m_nRewardNum;						// 出师奖励次数
	TMap<DWORD, DWORD>	m_mapApprentice;					// 徒弟ID容器
	tagClassMateData	m_ClassMateData;					// 同门数据
	Mutex				m_mutexMaster;						// 师傅锁
	Mutex				m_mutexMate;						// 同门锁
	INT					m_nMARelation;						// 师徒总数量
	BYTE				m_byTeammatesNum;					// 队友数
	BYTE				m_byTeamFriendNum;					// 队友中的好友数

	//-------------------------------------------------------------------------------------
	// 物品冷却
	//-------------------------------------------------------------------------------------
	BOOL				m_bObjectCoolOff;					// 行囊内的物品是否冷却

	//-------------------------------------------------------------------------------------
	// 禁言
	//-------------------------------------------------------------------------------------
	BOOL				m_bSpeakOff;						// 玩家是否被禁言

	//-------------------------------------------------------------------------------------
	// 属性点和技能点
	//-------------------------------------------------------------------------------------
	INT					m_nAttPointAdd[X_ERA_ATTA_NUM];		// 投入各种一级属性的属性点
	tagRoleTalent		m_Talent[X_MAX_TALENT_PER_ROLE];	// 天资及各自天资点

	//-------------------------------------------------------------------------------------
	// 复活相关
	//-------------------------------------------------------------------------------------
	DWORD				m_dwRebornMapID;					// 复活地图ID

	//-------------------------------------------------------------------------------------
	// 被动技能的触发器索引表
	//-------------------------------------------------------------------------------------
	std::set<DWORD>		m_setPassiveSkillTrigger[EPSAET_End];

	//-------------------------------------------------------------------------------------
	// 装备的触发器索引表
	//-------------------------------------------------------------------------------------
	typedef std::bitset<X_EQUIP_BAR_SIZE>	BitSetEquipPos;		/*n16EquipPos*/
	typedef std::set<DWORD>					SetSuitTrigger;		/*dwSuitID*/

	BitSetEquipPos		m_bitsetEquipTrigger[EPSAET_End];
	BitSetEquipPos		m_bitsetLongHunTrigger[EPSAET_End];
	SetSuitTrigger		m_setSuitTrigger[EPSAET_End];
	// Jason 2010-5-18 v2.1.0-fix
	VOID CalFabaoMatchTriggerBuff(const tagFabao * pFabao,bool bEquip,INT nPos);
	VOID TriggerFabaoMatchBuff(const tagFabao * pFabao);

	//-------------------------------------------------------------------------------------
	// 金钱
	//-------------------------------------------------------------------------------------
	CurrencyMgr			m_CurMgr;

	//-------------------------------------------------------------------------------------
	// 物品管理器 -- 管理行囊、任务页、装备栏、角色仓库和百宝袋等
	//-------------------------------------------------------------------------------------
	ItemMgr				m_ItemMgr;							// 物品管理器

	//-------------------------------------------------------------------------------------
	// 元神
	//-------------------------------------------------------------------------------------
	HolySoul            m_HolySoul;

	//-------------------------------------------------------------------------------------
	// 套装管理
	//-------------------------------------------------------------------------------------
	Suit				m_Suit;

	//-------------------------------------------------------------------------------------
	// 龙魂能力相关
	//-------------------------------------------------------------------------------------
	LongHun				m_LongHun;

	//-------------------------------------------------------------------------------------
	// 玩家间交易相关
	//-------------------------------------------------------------------------------------
	ExchangeMgr			m_ExchMgr;

	//-------------------------------------------------------------------------------------
	// 妖精融合
	//-------------------------------------------------------------------------------------
	bool				m_bIsInFairyFusion;
	INT64				m_n64FusionFairySerial;

	//-------------------------------------------------------------------------------------
	// 摆摊
	//-------------------------------------------------------------------------------------
	Stall				*m_pStall;

	//-------------------------------------------------------------------------------------
	// 好友相关
	//-------------------------------------------------------------------------------------
	tagFriend						m_Friend[MAX_FRIENDNUM];				// 好友列表	
	TMap<DWORD, tagFriend*>			m_mapFriend;
	DWORD							m_dwBlackList[MAX_BLACKLIST];			// 黑名单
	DWORD							m_dwEnemyList[MAX_ENEMYNUM];			// 增加仇敌名单

	//-------------------------------------------------------------------------------------
	// 团队相关
	//-------------------------------------------------------------------------------------
	DWORD				m_dwTeamID;											// 小队ID
	DWORD				m_dwGroupID;										// 团队ID
	DWORD				m_dwTeamInvite;										// 邀请人ID
	BOOL				m_bTeamSyn;											// 小队玩家状态同步标志

	//-------------------------------------------------------------------------------------
	// 副本相关
	//-------------------------------------------------------------------------------------
	DWORD				m_dwOwnInstanceMapID;								// 玩家所创建的副本的地图ID
	DWORD				m_dwOwnInstanceID;									// 玩家创建的副本ID
	DWORD				m_dwExportMapID;									// 传出副本时的地图ID
	Vector3				m_vExport;											// 传出副本时地图的坐标		

	//-------------------------------------------------------------------------------------
	// 对远端玩家公开信息设置
	//-------------------------------------------------------------------------------------
	tagRemoteOpenSet	m_sRemoteOpenSet;

	//-------------------------------------------------------------------------------------
	// 玩家脚本
	//-------------------------------------------------------------------------------------
	const RoleScript*	m_pScript;											// 玩家脚本

	//-------------------------------------------------------------------------------------
	// 角色开启宝箱计数
	//-------------------------------------------------------------------------------------
	INT					m_nTreasureSum;										// 玩家已开启的宝箱数
	
	//-------------------------------------------------------------------------------------
	// 开宝箱状态
	//-------------------------------------------------------------------------------------
	tagChestState		m_TreasureState;

	//-------------------------------------------------------------------------------------
	// 是否升30级已关闭pk保护
	//-------------------------------------------------------------------------------------
// 	BOOL				m_bLevelUpClosedSafeGuard;

	//-------------------------------------------------------------------------------------
	// 实力系统及相关
	//-------------------------------------------------------------------------------------
	tagStrength			m_Strength;											// 实力值
	INT					m_nStrengthNum;										// 实力排名，实力排行按照固有实力排
	INT					m_nEquipPm;											// 装备实力排名
	DWORD				m_dwLatestOne;										// 记录该玩家最后一次击杀的玩家的ID
	INT					m_nKillScore;										// 记录该玩家一日之内击杀非白名玩家获得的进阶实力值
	tagDWORDTime		m_dwLastClearKillScore;								// 记录上次将killscore清零时间

public:
	INT					GetPm(ERankingType);
	VOID				SetEquipPm(INT nPm)					{ m_nEquipPm = nPm;			}
	INT					GetEquipPm()						{ return m_nEquipPm;		}
	INT					GetStrengthPm()						{ return m_nStrengthNum;	}
	tagStrength			GetStrength()						{ return m_Strength;		}
	INT					GetTotalStrength()					{ return m_Strength.nAdvanceStrength + m_Strength.nBaseStrength; }
	
	INT					GetActivityStrength()				{ return GetScriptData(99);	}
	INT					GetQuestStrength()					{ return GetScriptData(100);}
	INT					GetKillStrength()					{ return GetScriptData(101);}
	INT					GetInstanceStrength()				{ return GetScriptData(95);	}

	VOID				SetAdvanceStrength(INT nValue)		{ m_Strength.nAdvanceStrength = nValue;		}
	VOID				SetConsumptiveStrength(INT nValue)	{ m_Strength.nConsumptiveStrength = nValue;	}
	VOID				ClearKillScore()					{ m_nKillScore = 0; m_dwLastClearKillScore = GetCurrentDWORDTime(); }
	BOOL				IsInBattleField(Role* pSrcRole, Role* pTargetRole);
	BOOL				IsEquipTimeEquipment();

protected:
	//-------------------------------------------------------------------------------------
	// 精力系统
	//-------------------------------------------------------------------------------------
	INT64				m_dwFixSpiriteCDTime;								// 精力值不变的剩余时间
	DWORD				m_dwSpiUpdateTickCount;								// 用于计时，每300个tick更新一次
	FLOAT				m_fSpiRate;											// 当前收益

protected:	
	//-------------------------------------------------------------------------------------
	// 神职竞选相关
	//-------------------------------------------------------------------------------------
	DWORD				m_dwClergyMarsPoint;			// 战神系分值
	DWORD				m_dwClergyApolloPoint;			// 太阳神系分值
	DWORD				m_dwClergyRabbiPoint;			// 法系神系分值
	DWORD				m_dwClergyPeacePoint;			// 和平之神系分值
	ECampaignGodType	m_eClergyCandidate;				// 神职竞选报名状态
	ECandidateFlag		m_eClergy4SeniorOrJunior;		// 参与神职竞选资格
	DWORD				m_dwPrevClergy;					// 上一期的神职
	DWORD				m_dwClergy;						// 当前的神职

public:
	VOID				CalClergyPoint()				// 计算神系分值
	{
		//战神方向：
		//分值=体力×2+真气+物理攻击×3+法术攻击+物理防御×2+法术防御+精准+灵巧+灵力	
		//太阳神方向
		//分值=体力+真气+物理攻击×3+法术攻击+物理防御×2+法术防御+精准×3+灵巧+灵力	
		//法师之神方向
		//分值=体力+真气×2+物理攻击+法术攻击×3+物理防御+法术防御×2+精准+灵巧+灵力	
		//和平之神方向
		//分值=体力×2+真气+物理攻击+法术攻击×2+物理防御+法术防御+精准+灵巧+灵力×5
		m_dwClergyMarsPoint = m_nAtt[ERA_MaxHP]*2+m_nAtt[ERA_MaxMP]+m_nAtt[ERA_ExAttack]*3+m_nAtt[ERA_InAttack]+m_nAtt[ERA_ExDefense]*2+m_nAtt[ERA_InDefense]+m_nAtt[ERA_AttackTec]+m_nAtt[ERA_DefenseTec]+m_nAtt[ERA_Regain_Addtion];
		m_dwClergyApolloPoint = m_nAtt[ERA_MaxHP]+m_nAtt[ERA_MaxMP]+m_nAtt[ERA_ExAttack]*3+m_nAtt[ERA_InAttack]+m_nAtt[ERA_ExDefense]*2+m_nAtt[ERA_InDefense]+m_nAtt[ERA_AttackTec]*3+m_nAtt[ERA_DefenseTec]+m_nAtt[ERA_Regain_Addtion];
		m_dwClergyRabbiPoint = m_nAtt[ERA_MaxHP]+m_nAtt[ERA_MaxMP]*2+m_nAtt[ERA_ExAttack]+m_nAtt[ERA_InAttack]*3+m_nAtt[ERA_ExDefense]+m_nAtt[ERA_InDefense]*2+m_nAtt[ERA_AttackTec]+m_nAtt[ERA_DefenseTec]+m_nAtt[ERA_Regain_Addtion];
		m_dwClergyPeacePoint = m_nAtt[ERA_MaxHP]*2+m_nAtt[ERA_MaxMP]+m_nAtt[ERA_ExAttack]+m_nAtt[ERA_InAttack]*2+m_nAtt[ERA_ExDefense]+m_nAtt[ERA_InDefense]+m_nAtt[ERA_AttackTec]+m_nAtt[ERA_DefenseTec]+m_nAtt[ERA_Regain_Addtion]*5;
	
		if (!ECGT_INVALID(m_eClergyCandidate))
		{
			if(E_Success == CanGetClergy(m_eClergyCandidate, true))
				m_eClergy4SeniorOrJunior = ECF_4BossClergy;
			else if(E_Success == CanGetClergy(m_eClergyCandidate, false))
				m_eClergy4SeniorOrJunior = ECF_4BaseClergy;
			else
				m_eClergy4SeniorOrJunior = ECF_Null;
		}
			
	}
	
	ECampaignGodType	GetClergyCandidateType()
	{
		return m_eClergyCandidate;
	}

	DWORD				CanGetClergy(ECampaignGodType eType, bool bSeniorOrJunior = false/* true 大神 false 小神*/);
	VOID				SetClergyCandidateType(ECampaignGodType eType)
	{
		m_eClergyCandidate = eType;
	}

	VOID				ClergyBack()
	{
		const tagClergyProto *pPreClergyProto = g_attRes.GetClergyProto(m_dwPrevClergy);
	
		if (P_VALID(pPreClergyProto) &&
			(pPreClergyProto->eType == EGT_WarBoss || pPreClergyProto->eType == EGT_SunBoss ||
			pPreClergyProto->eType == EGT_ManaBoss || pPreClergyProto->eType == EGT_PeaceBoss || 
			pPreClergyProto->eType == EGT_War || pPreClergyProto->eType == EGT_Sun ||
			pPreClergyProto->eType == EGT_Mana || pPreClergyProto->eType == EGT_Peace))
		{
			m_dwPrevClergy = 0;
		}

		if( E_Success != PromotionClergy(m_dwPrevClergy, false))
		{
			m_dwClergy = 0;
		}
	
		//广播给周围的角色
		tagNS_ChangeClergy send1;
		send1.dwClergy = m_dwClergy;
		send1.dwRoleID = GetID();
		if( P_VALID(GetMap())) GetMap()->SendBigVisTileMsg(this, &send1, send1.dwSize);
	}

	VOID				LoginClergyVoteResultCheck();	// 角色上线的时候检查其时候竞选神职成功

	DWORD				GetClergy(){return m_dwClergy;}

public:
	//-------------------------------------------------------------------------------------
	// 初始化当前任务
	//-------------------------------------------------------------------------------------
	VOID InitCurrentQuest(const BYTE* &pData, INT32 nNum);

	//-------------------------------------------------------------------------------------
	// 初始化所有已完成任务
	//-------------------------------------------------------------------------------------
	VOID InitCompleteQuest(const BYTE* &pData, INT32 nNum);
    
	//-------------------------------------------------------------------------------------
	// 初始化所有已完成每日任务
	//-------------------------------------------------------------------------------------
	VOID InitDailyCompleteQuest(const BYTE* &pData, INT32 nNum);
	//-------------------------------------------------------------------------------------
	// 通过NPC处接取任务
	//-------------------------------------------------------------------------------------
	INT AcceptQuestFromNPC(UINT16 u16QuestID, DWORD dwNPCID);

	//-------------------------------------------------------------------------------------
	// 通过触发器接取任务
	//-------------------------------------------------------------------------------------
	INT AcceptQuestFromMapTrigger(UINT16 u16QuestID, DWORD dwMapTriggerID);

	//-------------------------------------------------------------------------------------
	// 是否可以接取任务，返回参数为如果可参加，对应的索引
	//-------------------------------------------------------------------------------------
	INT CanAcceptQuest(UINT16 u16QuestID, INT& nIndex, Creature* pNPC=NULL);
	INT	CanAcceptBoardQuest(UINT16 u16QuestID, INT& nIndex, EQuestBoardType eQuestBoardType);
	//-------------------------------------------------------------------------------------
	// 增加任务
	//-------------------------------------------------------------------------------------
	BOOL AddQuest(const tagQuestProto* pProto, INT nIndex);
	BOOL AddBoardQuest(const tagBoardQuestProto* pProto, INT nIndex, UINT16 u16ID);

	//-------------------------------------------------------------------------------------
	// 完成任务
	//-------------------------------------------------------------------------------------
	INT CompleteQuest(UINT16 u16QuestID, DWORD dwNPCID, INT nChoiceItemIndex, EQuestComplteType eComplteType, UINT16& u16NexitQuestID );

	//-------------------------------------------------------------------------------------
	// 是否可以交任务
	//-------------------------------------------------------------------------------------
	INT CanCompleteQuest(Quest* pQuest, DWORD dwNPCID, INT nChoiceItemIndex);

	//-------------------------------------------------------------------------------------
	// 任务奖励
	//-------------------------------------------------------------------------------------
	VOID RewardQuest(Quest* pQuest, INT32 nChoiceItemIndex);
	VOID RewardBoardQuest(UINT16 u16QuestID, EQuestBoardType eQuestBoardType);
	//-------------------------------------------------------------------------------------
	// 删除任务
	//-------------------------------------------------------------------------------------
	INT DeleteQuest(UINT16 u16QuestID);

	//-------------------------------------------------------------------------------------
	// 删除一个任务
	//-------------------------------------------------------------------------------------
	VOID RemoveQuest(UINT16 u16QuestID, BOOL bComplete);

	//-------------------------------------------------------------------------------------
	// 更新NPC对话任务状态
	//-------------------------------------------------------------------------------------
	VOID UpdateQuestNPCTalk(UINT16 u16QuestID, DWORD dwNPCID);

	//-------------------------------------------------------------------------------------
	// 更新Trigger任务状态
	//-------------------------------------------------------------------------------------
	VOID UpdateQuestTrigger(UINT16 u16QuestID, DWORD dwTriggerID);

	//-------------------------------------------------------------------------------------
	// 触发任务事件
	//-------------------------------------------------------------------------------------
	VOID OnQuestEvent(EQuestEvent eQuestType, DWORD dwEventMisc1=0, DWORD dwEventMisc2=0, DWORD dwEventMisc3=0);

	//-------------------------------------------------------------------------------------
	// 触发家族杀怪任务事件
	//-------------------------------------------------------------------------------------
	VOID OnFamilyKillMonsterQuestEvent(DWORD dwMonsterID, INT nMonsterLevel, DWORD dwMapCrc);

	//------------------------------------------------------------------------------------
	// 得到完成任务的个数
	//------------------------------------------------------------------------------------
	INT GetCompleteQuestCount() { return m_mapCompleteQuests.Size(); }

	//------------------------------------------------------------------------------------
	// 清空所有已完成任务
	//------------------------------------------------------------------------------------
	VOID ClearCompleteQuest()
	{
		tagQuestDoneSave* pDoneQuest = NULL;
		QuestDoneMap::TMapIterator it = m_mapCompleteQuests.Begin();
		while( m_mapCompleteQuests.PeekNext(it, pDoneQuest) )
		{
			// 向客户端发送消息
			tagNS_GMQuestState send;
			send.u16QuestID = pDoneQuest->u16QuestID;
			send.bDone = FALSE;
			SendMessage(&send, send.dwSize);

			SAFE_DEL(pDoneQuest);
		}
		m_mapCompleteQuests.Clear();
	}
	//------------------------------------------------------------------------------------
	// 清空所有已完成每日任务
	//------------------------------------------------------------------------------------
	VOID ClearDailyCompleteQuest();

	//------------------------------------------------------------------------------------
	// 添加或删除一个已完成任务
	//------------------------------------------------------------------------------------
	VOID AddDelCompleteQuest(UINT16 u16QuestID, BOOL bAddOrDel)
	{
		// 将指定任务加入已完成任务
		if( bAddOrDel )
		{
			if( m_mapCompleteQuests.IsExist(u16QuestID) )
				return;

			tagQuestDoneSave* pQuestDone = new tagQuestDoneSave;
			pQuestDone->u16QuestID = u16QuestID;
			pQuestDone->nTimes = 1;
			pQuestDone->dwStartTime = g_world.GetWorldTime();

			m_mapCompleteQuests.Add(u16QuestID, pQuestDone);
			if( !m_mapDailyCompleteQuests.IsExist(u16QuestID) )
			{
				tagQuestDoneSave* pQuestDoneTemp = new tagQuestDoneSave;
				pQuestDoneTemp->u16QuestID = u16QuestID;
				pQuestDoneTemp->nTimes = 1;
				pQuestDoneTemp->dwStartTime = g_world.GetWorldTime();
               m_mapDailyCompleteQuests.Add(u16QuestID, pQuestDoneTemp); 
			}
			// 向客户端发送消息
			tagNS_GMQuestState send;
			send.u16QuestID = u16QuestID;
			send.bDone = TRUE;
			SendMessage(&send, send.dwSize);
		}
		else // 令指定任务为未完成任务
		{		
			tagQuestDoneSave* pQuestDone = m_mapCompleteQuests.Peek(u16QuestID);
			if( P_VALID(pQuestDone) )
			{
				m_mapCompleteQuests.Erase(u16QuestID);
				SAFE_DEL(pQuestDone);
			}

			tagQuestDoneSave* pQuestDoneTemp = m_mapDailyCompleteQuests.Peek(u16QuestID);
			if( P_VALID(pQuestDoneTemp) )
			{
				m_mapDailyCompleteQuests.Erase(u16QuestID);
				SAFE_DEL(pQuestDoneTemp);
			}

			// 向客户端发送消息
			tagNS_GMQuestState send;
			send.u16QuestID = u16QuestID;
			send.bDone = FALSE;
			SendMessage(&send, send.dwSize);
		}
	}

	//------------------------------------------------------------------------------------
	// 得到完成任务的个数
	//------------------------------------------------------------------------------------
	INT GetCurrentQuestCount() { return m_mapCurrentQuests.Size(); }

	//-------------------------------------------------------------------------------------
	// 得到指定任务完成的次数
	//-------------------------------------------------------------------------------------
	INT GetQuestCompleteTimes(UINT16 u16QuestID)
	{
		tagQuestDoneSave* pDoneQuest = m_mapCompleteQuests.Peek(u16QuestID);
		if( !P_VALID(pDoneQuest) ) return GT_INVALID;

		return pDoneQuest->nTimes;
	}

	//-------------------------------------------------------------------------------------
	// 得到指定任务完成的接取时间
	//-------------------------------------------------------------------------------------
	DWORD GetQuestCompleteAcceptTime(UINT16 u16QuestID)
	{
		tagQuestDoneSave* pDoneQuest = m_mapCompleteQuests.Peek(u16QuestID);
		if( !P_VALID(pDoneQuest) ) return GT_INVALID;

		return pDoneQuest->dwStartTime;
	}

	//-------------------------------------------------------------------------------------
	// 得到指定任务
	//-------------------------------------------------------------------------------------
	Quest* GetQuest(UINT16 u16QuestID) { return m_mapCurrentQuests.Peek(u16QuestID); }

	//-------------------------------------------------------------------------------------
	// 得到指定任务是否存在
	//-------------------------------------------------------------------------------------
	BOOL IsHaveQuest(UINT16 u16QuestID) { return m_mapCurrentQuests.IsExist(u16QuestID); }

	//-------------------------------------------------------------------------------------
	// 检测是否做过某任务
	//-------------------------------------------------------------------------------------
	BOOL IsHaveDoneQuest(UINT16 u16QuestID) { return m_mapCompleteQuests.IsExist(u16QuestID); }

	//-------------------------------------------------------------------------------------
	// 得到好友个数
	//-------------------------------------------------------------------------------------
	INT  GetFriendCount() { return m_mapFriend.Size(); }

	//-------------------------------------------------------------------------------------
	// 服务器可控对话框缺省事件
	//-------------------------------------------------------------------------------------
	VOID OnDlgDefaultEvent(EMsgUnitType	eDlgTarget, DWORD dwTargetID, EDlgOption eDlgOption);

	//-------------------------------------------------------------------------------------
	// 触发该角色的队友身上的传道任务
	//-------------------------------------------------------------------------------------
	VOID OnChuanDaoQuest(EChuanDaoType eType);

	//-------------------------------------------------------------------------------------
	// 身上是否有传道任务
	//-------------------------------------------------------------------------------------
	UINT16 IsHaveChuanDaoQuest(INT nLevel)
	{
		Quest* pQuest = NULL;
		QuestMap::TMapIterator it = m_mapCurrentQuests.Begin();
		while (m_mapCurrentQuests.PeekNext(it, pQuest))
		{
			if (!P_VALID(pQuest))
			{
				continue;
			}

			if (!P_VALID (pQuest->GetProto()->chuandao_count))
			{
				continue;
			}

			if(nLevel<pQuest->GetProto()->chuandao_junior_min_level || nLevel>pQuest->GetProto()->chuandao_junior_max_level)
			{
				continue;
			}
			
			if(pQuest->GetChuanDaoCount() >= pQuest->GetProto()->chuandao_count)
			{
				continue;
			}

			return pQuest->GetID();
		}

		return 0;
	}

public:

	// 保存试炼状态
	VOID SaveTrainState(DWORD dwTrainID, ETrainState eTrainState);

	// 取试炼状态
	ETrainState GetTrainState(DWORD dwTrainID);

	// 获取一个试炼的完成度
	DWORD GetTrialCompleteNum(DWORD dwTrainID);

	// 设置一个成就的完成度
	VOID SetTrainCompleteness(DWORD dwTrainID, DWORD dwCompleteness);

	// 将一个成就设置为已完成状态
	VOID CompleteTrain(DWORD dwTrainID);
	
	// 给与试炼奖励
	DWORD GiveTrainReward(DWORD dwTrainID);

	// 取完成试炼个数
	INT GetPassTrainCnt();

	// 给客户端发送试炼状态表
	ETrainErrCode SendTrainStateList2Client();

private:

	// 创建奖励物品
	DWORD CreateRewardItem(TList<tagItem *>& pListRewardItem, tagItemReward* pItemReward, INT16* nLapNum);

	// 初始化试炼状态表
	VOID InitTrainState(const BYTE* &pData, INT32 nNum);
	VOID DelTrainStateMap()
	{
		tagTrainState* pTrainState = NULL;
		TrainStateMap::TMapIterator it = m_mapTrainStates.Begin();
		while( m_mapTrainStates.PeekNext(it, pTrainState) )
		{
			SAFE_DEL(pTrainState);
		}
		m_mapTrainStates.Clear();
	}

private:
	Quest			m_Quests[QUEST_MAX_COUNT];		// 玩家当前的任务
public:
	BOOL			QuestValid(INT nIndex){return m_Quests[nIndex].IsValid();}
private:
	QuestMap		m_mapCurrentQuests;				// 玩家当前的任务
	QuestDoneMap	m_mapCompleteQuests;			// 已经完成的任务集合
	QuestDoneMap    m_mapDailyCompleteQuests;       // 每天完成的任务次数
    BOOL          	bIsDailyQuestCleared;           // 每日是否已经被清空过
	TrainStateMap	m_mapTrainStates;				// 玩家所有试炼状态表
	BYTE			m_byTrainStateTransfered;

#ifdef ON_INTERNET_CAFE
	DWORD			m_dwInternetCafePingTime;
	BYTE			m_byInternetCafePingLostTime;
	BOOL				m_bTimeout;					// 网吧奖励结束

#endif
// 任务板相关
private:
	BoardQuestMgr*	m_pBoardQuestMgr;			// 悬赏任务板
	BoardQuestMgr*	m_pYellowListMgr;				// 天之行会任务榜
	UINT16				m_n16YellowListAcceptTimes;		// 接取天之行会任务榜任务次数

public:
// 任务板相关函数
	BoardQuestMgr*	GetBoardQuestMgr(EQuestBoardType eQuestBoardType)
	{
		if (eQuestBoardType == EQBT_Reward)
			return m_pBoardQuestMgr;
		else if (eQuestBoardType == EQBT_YellowList)
			return m_pYellowListMgr;
		else
			return NULL;
	}
	EQuestBoardType GetQuestBoardType(UINT16 u16QuestID)
	{
		if (IsRewardBoardQuest(u16QuestID))
			return EQBT_Reward;
		else if (IsYellowListQuest(u16QuestID))
			return EQBT_YellowList;
		else
			return EQBT_Null;
	}
	UINT16 GetYellowListAcceptTimes(){ return m_n16YellowListAcceptTimes; }
	VOID IncrYellowListAcceptTimes()	{ SetScriptData(TodayYellowListQuestTimesIdx, ++m_n16YellowListAcceptTimes); }
	VOID ResetYellowListAcceptTimes() { m_n16YellowListAcceptTimes = 0; SetScriptData(TodayYellowListQuestTimesIdx, 0); }

// 称号相关
public:
	TitleMgr*		GetTitleMgr() { return m_pTitleMgr; }
private:
	TitleMgr*		m_pTitleMgr;
//角色名贴相关
public:
	tagRoleVCard&	GetVCard() { return m_VCard; }
private:
	tagRoleVCard	m_VCard;
//氏族信息
public:
	ClanData& GetClanData()	{ return m_ClanData;}
private:
	ClanData		m_ClanData;
// 宠物带
public:
	PetPocket*	GetPetPocket() { return m_pPetPocket; }
private:
	PetPocket*	m_pPetPocket;
public:
	VOID OnLeaveMap(BOOL bChgMap = TRUE);

	void BreakMount();
	VOID OnEnterMap();
public:
	// 世界喊话，并重置时间
	BOOL TalkToWorld();
private:
	INT m_nWorldTalkCounter;
	VOID UpdateTalkToWorld();

public:
	// 场景喊话
	BOOL TalkToScene();
private:
	INT m_nSceneTalkCounter;
	VOID UpDateTalkToScene();
	//群聊 
public:
	VOID   AddGroupChat(DWORD dwGroupChatId,group_chat* pGroupChat) { m_mapGroupChat.Add(dwGroupChatId,pGroupChat); }
	VOID   DeleteGroupChat(DWORD dwGroupChatId)                     { m_mapGroupChat.Erase(dwGroupChatId); }
	VOID   ClearGroupChat();
private:
	TSafeMap<DWORD,group_chat*>      m_mapGroupChat;
public:
	DWORD CanPickUpItem(INT64 n64GroundID, BOOL bRole = TRUE);
	// 拾取物品
	DWORD PickUpItem(INT64 n64GroundID, DWORD& dwTypeID, BOOL bRole = TRUE);
	// 放下物品
	DWORD PutDownItem(INT64 n64Serial, INT64 &n64GroupID, BOOL bAdd2Ground = FALSE);

	// 放下战场相关物品
	DWORD PutDownWarItem(INT64 n64Serial, INT64 &n64GroupID, BOOL bAdd2Ground = FALSE);

	DWORD LootItem(INT64 n64Serial, INT64 &n64GroupID);
public:
	// 设置骑乘状态
	VOID SetMount(BOOL bSet, INT nMountSpeed, const tagPetProto* pPetProto);

	// 设置骑乘速度
	VOID ModMountSpeed(INT nMod);

	// 邀请骑乘
	VOID SetPassenger(Role* pHost);

	typedef MoveData::EMoveRet (MoveData::*PFMoveAction2P)(const Vector3& v1, const Vector3& v2);
	typedef MoveData::EMoveRet (MoveData::*PFMoveAction1P)(const Vector3& v);

	MoveData::EMoveRet	MoveAction(PFMoveAction2P pAction, Vector3& v1, Vector3& v2);
	MoveData::EMoveRet	MoveAction(PFMoveAction1P pAction, Vector3& v);

	//------------------------------------------------------------------------------
	// 玩家间宠物交易相关
	//------------------------------------------------------------------------------
	PetExchangeMgr& GetPetExchMgr() { return m_PetExchMgr; }

	BOOL	IsPetExchanging()					{ return IsInRoleState(ERS_PetExchange); }
	BOOL	CanPetExchange()
	{
		return !IsInRoleStateAny(ERS_Exchange | ERS_Shop | ERS_Stall | ERS_PetExchange) 
			&& (GetPetExchMgr().GetTgtRoleID() == GT_INVALID);
	}

	VOID	BeginPetExchange(DWORD dwTgtRoleID)
	{
		SetRoleState(ERS_PetExchange);
		GetPetExchMgr().CreateData();
		GetPetExchMgr().SetTgtRoleID(dwTgtRoleID);
	}

	VOID	EndPetExchange()
	{
		UnsetRoleState(ERS_PetExchange);
		GetPetExchMgr().DeleteData();
		GetPetExchMgr().SetTgtRoleID(GT_INVALID);
	}

	DWORD	ProcPetExchangeReq(OUT Role* &pTarget, DWORD dwTgtRoleID);
	DWORD	ProcPetExchangeReqRes(OUT Role* &pApplicant, DWORD dwTgtRoleID, DWORD dwErrorCode);
	DWORD ProcPetExchangeAdd(OUT Role* &pTarget, DWORD dwPetID);
	DWORD ProcPetExchangeDec(OUT Role* &pTarget, DWORD dwPetID);
	DWORD	ProcPetExchangeMoney(OUT Role* &pTarget, INT64 n64Silver);
	DWORD	ProcPetExchangeLock(OUT Role* &pTarget);
	DWORD	ProcPetExchangeCancel(OUT Role* &pTarget);
	DWORD	ProcPetExchangeVerify(OUT Role* &pTarget, OUT DWORD &dwFailedRoleID);

	DWORD VerifyPetExchangeData();
	DWORD	ProcPetExchange();

	DWORD	ProcPreparePetExchange(OUT Role* &pTarget);
	public:
	VOID     ClenupPetExchState();
	private:

private:
	PetExchangeMgr m_PetExchMgr;
	//	角色开启宝箱计数相关
public:

	//	获取角色开启宝箱计数
	INT GetTreasureSum()		{ return m_nTreasureSum; }
	//	增加宝箱计数
	VOID IncTreasureSum();
	//	初始化宝箱相关
	VOID InitChestState();
	//	设置宝箱开启数
	VOID SetTreasureSum(INT nSum);

	VOID StopMount();
	VOID ContinueMount();

public:
	// 金牌网吧经验加成
	FLOAT	GetVNBExpRate();
	// 金牌网吧掉率加成
	FLOAT	GetVNBLootRate();
	
public:
	// GM命令调用的几个函数
	DWORD AddSuit(DWORD dwSuitID, INT nQlty);
	DWORD AddEquip(INT nType, INT nLevel, INT nQlty);
public:
	//重置是否有留言消息
	VOID	ResetHasLeftMsg()
	{
		m_bHasLeftMsg = FALSE;
	}
private:
	BOOL	m_bHasLeftMsg;
public:	
	// 是否可以采集
	INT CanGather(Creature* pRes);

public:
	//是否可以播放个性动作
	DWORD	CanCastMotion(Unit* pDest, DWORD dwActionID);
	//是否可以播放单人个性化动作
	DWORD	CanCastMotion(DWORD dwActionID);
	// 播放个性动作
	DWORD	CastMotion(Unit* pDest, DWORD dwActionID);
public:	
	// 获得重生信息	
	tagRevive& GetRevive() 	{ return m_Revive; }
	// 判断dwRoleID有没有权利进行交互
	BOOL	GetMotionInviteStatus(DWORD dwRoleID);
	// 玩家是否可以接受或者邀请标记
	VOID	SetMotionInviteStatus(BOOL bSet, DWORD dwRoleID);
	// 更新状态
	VOID	UpdateMotionInviteState();
private:
	INT		m_nMotionCounter;
	DWORD	m_dwPartnerID;
public:
	//保存经验到db
	VOID	SaveExp2DB();
	//保存属性点到db
	VOID SaveAttPoint2DB();
	//保存天资点到db
	VOID	SaveTalentPoint2DB(INT nIndex);
	//保存等级到db
	VOID	SaveLevel2DB();
	//保存名字到db
	VOID	SaveNewName2DB();

private:
	//台湾金牌网吧
	OrangeVipNetBarMgr m_clsOrangeVNBMgr;				// 台湾金牌网吧管理

	// Jason 2009-12-7 神龙赐福
	VOID	UpdateBlessingOfLoong(VOID);

	// Jason 2009-12-7 神龙赐福
	DWORD	m_timeLastLessing;							// 上次神龙赐福时间，注意，该值与总在线时间单位一致，单位秒
	INT				m_nLessingLevel;					// 赐福级别，最高ROLE_MAX_LESSLEVEL_OF_LOONG级
	INT				m_nLessingLevelEx;					// 神龙赐福扩展级别。也即过8次后的神龙赐福。
	tagDWORDTime			m_dateLastLessingLoong;		// 这个作为开启在线奖励的的标志吧

	//==============================================================================================================================
	// Jason 2010-1-7 v1.3.2
	DWORD					m_dwCurrentDayOnlineTime;	// 当日累计在线时间
	tagDWORDTime		m_timeOneDayFirstLogined;		// 上次登录的日期(首次登陆)，注意，这个字段主要用于判断是否跨天，因此，对于时间中的秒数等不做其他要求，只要能标示某天即可
	tagDWORDTime		m_timeOnlineRewardPerDayRec;	// 当日领取在线奖励的时间
	tagDWORDTime		m_timeLastCalOnlineTime;		// 上次计算时间
	//--------------------------------------------------------------------------------------------------------------------------------
	// 下面的不要保存到数据库
	INT		m_n32UpdateOnlineReward;		// 在线更新时间，不要保存到数据库
	BOOL	m_bOnlineRewardPromptFlag;
	INT		m_nSelfShape;					// 自身大小，双人骑乘时，被邀请方需保存原有大小
	INT		m_nMountShape;					// 骑乘时，变化后的大小
	INT		m_nLessingExCounter;
	//==============================================================================================================================

	BOOL IsValidOfLessingLoong(VOID);

public:
	VOID NotifyClientToReceiveLessingOfLoong(VOID); // 通知玩家领下一次取神龙赐福
	VOID NotifyClientIniLessingOfLoong(VOID);		// 通知玩家神龙赐福，开启客户端相关功能界面

	DWORD ReceiveLessingOfLoong(VOID);				// 领取神龙赐福奖励。

	// 这个函数返回的是累计的秒数，
	DWORD GetCurrentOnlineTime(VOID);
	DWORD ProcessGoldStoneMsg( INT64 GoldStoneItemID, INT64 destItemID);

	//==========================================================================================================
	// 竞技相关
	//==========================================================================================================
	static const INT      MAX_VALIANT_MERIT =  10;
	INT16                 m_nValiantMeritLevel;  //武勋等级
    //根据武勋值获玩家的武勋等级
	INT16 GetValiantMeritLevel(INT16 nValiantMeritVal);


	//==========================================================================================================
	// 快捷栏保存数据库
	//==========================================================================================================
private:
	static const int			BAR_COUNT = 10;					//快捷栏分组数
	static const int			BAR_BUTTON_COUNT = 8;			//每组快捷键个数
	tagQuickData                m_ShortCutBarData[BAR_COUNT][BAR_BUTTON_COUNT]; //快捷栏数据
    tagQuickBarState            m_QuickBarStateFlags[BAR_COUNT][BAR_BUTTON_COUNT]; //标记需要保存的快捷栏数据 
	INT16                       nIsSavedQuickBarData;      //针对在数据库未存过的玩家进行通知
	BOOL                        m_bClientReqShortCutBarData;
	BOOL                        m_bLoadFromDBFlag;
	BOOL                        m_bIsSended;
public:
    VOID SetShortCutBarData(INT nLnCount,INT nCol,const tagQuickData& QuickBarData);
	VOID SetShortCutBarDataFlag(INT nLnCount,INT nCol,const tagQuickData& QuickBarData);
    tagQuickData& GetShortCutBarData(INT nLnCount,INT nCol);
	VOID SendShortCutBarDataToClient();
	BOOL IsNeedSave();
	//保存快捷栏到db
	VOID	SaveQuickBarData2DB();  
    VOID    LoadQuickBarData2DB();
	VOID    SetIsSavedQuickBarData(INT16 nIsSaved);
	INT16    GetIsSavedQuickBarData()const;
	VOID    SendHaveNotSavedQuickBarMsg(); 
	VOID    SetShortCutBarReqFlag(BOOL bReqShortCutBarData)  { m_bClientReqShortCutBarData = bReqShortCutBarData ;}
	BOOL    GetShortCutBarReqFlag()  {   return m_bClientReqShortCutBarData;}
	VOID    SetLoadFromDBFlag(BOOL bLoadFromDBFlag)  { m_bLoadFromDBFlag = bLoadFromDBFlag;}
	BOOL    GetLoadFromDBFlag()  {  return m_bLoadFromDBFlag ; }

	//==========================================================================================================
	// Jason 2010-1-15 v1.3.2 是否开启了脱机挂，改状态应该保存到数据库
	BOOL IsOfflineHangOpen(VOID);
	//
	BOOL GetOfflineRewardParam(DWORD & hour,DWORD & minute);
	// Jason 2010-1-30 v.1.3.2 扩充宠物带容量，每次只加1个
	BOOL IncFollowPetPocketValve();
	BOOL IncRidingPetPocketValve();
    BOOL IncPetPocketValve(); //总的个数
    inline UINT16 FollowPetPocketValue()const
    {
        return  m_u16FollowPetPocketValve;
	}
	inline UINT16 RidingPetPocketValue()const
	{
        return  m_u16RidingPetPocketValve;
	}
	inline UINT16 PetPocketValve() const
	{
		return  m_u16PetPocketValve;
	}
	DWORD GetOfflineTime();
private:
	// Jason 2010-1-15 v1.3.2 离线挂机开关
	BOOL	m_bIsOpenOfflineReward;
	tagDWORDTime		m_iniLogoutTime;						// 登出时间

	// Jason 2010-1-30 v.1.3.2 当前宠物栏最大容量，一定要小于配置文件中的阀值，如果大于则只能取出阀值
	UINT16		m_u16PetPocketValve; //宠物和坐骑总量
	UINT16      m_u16FollowPetPocketValve;  //跟随宠物总量
	UINT16      m_u16RidingPetPocketValve;  //坐骑总量
    UINT16      m_u16FollowPetCount;        //跟随宠物个数　　　　
    UINT16      m_u16RidingPetCount;        //坐骑个数

private:
	// 实力系统相关
	VOID	UpdateEquipValue();				// 计算装备的实力值
	VOID	UpdateBaseStrength();			// 计算固有实力值
	//VOID	UpdateConsumptiveStrength();	// 更新可消耗实力值
	VOID	UpdateAdvanceStrength();		// 更新进阶实力值
	VOID	UpdateStrength();				// 更新总实力值
	VOID	CheckRoleAdvanceStrength();		// 角色每次上线时检查一下是否需要将进阶实力值清零
	VOID	UpdateYaoJingValue();				// 计算妖精的实力值

	// Jason 1.3.2 2010-1-7 计算当天在线时长
	DWORD CalTodayOnlineTime(BOOL notify = FALSE);
	DWORD CalTodayCurrentTime();
	tagDWORDTime m_timeCurrentDayTime;
	//INT					m_nCurrentDayOnlineTime;						// 累计当天在线时间（秒）

private:
	//仙界技能
	VOID UpdateMiracleSkill();

	//检查修炼限制
	DWORD CheckPracticeMiracleSkillLimit(DWORD dwSkillTypeID);

	//仙界技能消耗
	DWORD DoPracticeMiracleSkillCost(DWORD dwSkillTypeID);

private:
	tagDWORDTime	m_dwLastClearAdvenceStrength;
	int				m_nGodMiraclePoints;
public:
	VOID	ClearAdvanceStrength();

public:
	BOOL	CanGetTeamProfit();
	VOID	GetTeamProfit();
	VOID	CancelTeamProfit(bool bLauncheredByDeleteMember = false);
	VOID	UpdateTeamProfit();

	VOID	GodMiraclePoints(INT points);
	INT		GetGodMiraclePoints(VOID);

	// Jason 2010-4-15 v2.0.0 法宝修炼
	DWORD	PracticeFabao(INT64 srcItemID,INT num,INT64 fabaoID,tagFabaoSpec & fabaoSpec);
	DWORD	InlayFabao(INT64 n64FabaoID,INT num,INT64 * pBeadIDs,tagFabaoSpec & fabaoSpec);
	DWORD	ReverseFabao(INT64 dstFabaoID,INT64 srcFabaoID,tagFabaoSpec & fabaoSpec);
	DWORD	MergeFabao(INT64 dstFabaoID,INT64 srcFabaoID,tagFabaoSpec & fabaoSpec);
	
public:
	// 给客户端发送城战个人战绩信息
	DWORD SendCityStruggleRecord();

	// 跟客户端同步好友在线人数、同帮派在线人数、同门在线人数
	VOID SyncOnlineNum2Role(ESyncOnlineNumType eSyncOnlineNumType, UINT16 nOnlineNum);

	// 同步客户端玩家单个属性 [4/27/2012 zhangzhihua]
	VOID SyncRoleAttr(ERoleAttrSync eAttr, EMessageRange eRange = EMR_Self);

	// 取在线好友人数
	UINT16 GetOnlineFriendCnt();

	// 取同门在线人数
	UINT16 GetOnlineClassmatesCnt();

	// 跟徒弟同步同门人数变化
	VOID SyncOnlineNum2Apprentices(BOOL bLogout = FALSE);

	// 跟同门同步同门人数变化
	VOID SyncOnlineNum2Classmates(BOOL bLogout = FALSE);

	// 跟师门同门同步同门人数变化
	VOID SyncOnlineNum2MasterApprenticesClassmates(BOOL bLogout = FALSE);

public:
	VOID  SetCurScriptArea(DWORD dwCurScriptArea)	{ m_dwCurScriptArea = dwCurScriptArea; }
	DWORD GetCurScriptArea()						{ return m_dwCurScriptArea;				}
	BOOL  IsInFixArea(DWORD dwAreaID);

private:
	DWORD		m_dwCurScriptArea;		// 当前玩家所在的脚本区域
	INT			m_nSoaringSkillLearnTimes;	// 当天飞升学习次数
	//tagDWORDTime m_timeLearnSoaringSkill;	// 学习飞升技能的时间
	static CONST INT	m_nCLearnSoaringSkillTotalTimes = 100;

	struct tagAllSpeed
	{
		BOOL	bSetted;
		FLOAT						m_fXZSpeed;							// 当前XZ方向速度
		FLOAT						m_fYSpeed;							// 当前Y方向速度
		FLOAT						m_fSwimXZSpeed;						// 游泳的XZ方向速度
		FLOAT						m_fJumpXZSpeed;						// 跳跃的XZ方向速度，在跳跃开始时付值，跳跃过程中不改变
		FLOAT						m_fJumpYSpeed;						// 跳跃的Y方向速度，在跳跃开始时付值，在跳跃过程中不改变
		FLOAT						m_fDropXZSpeed;						// 掉落时的XZ方向速度
		FLOAT						m_fSlideSpeed;						// 滑落速度
		FLOAT						m_fMountXZSpeed;					// 当前骑乘XZ方向速度
	}	m_ownerSpeeds;

	// 这个结构主要用于在装备栏上直接强化的功能，防止影响最大值的装备，在处理效果时，拉下当前值
	// 其他地方，禁止随便使用
	struct tagAffectedByThresholdValue
	{
		INT nHP;
		INT nMP;
		INT nEndurance;
		INT nVitality;
	} m_stuValueBeforeStrengthen;
	VOID SaveTheValueBeforeStrengthen(VOID);
	VOID RestoreTheValueAfterStrengthen(VOID);

	// Jason 2010-5-24 v2.1.2
	typedef DWORD (Role::*TSpecSkillUsedFunc)(DWORD dwSkillID,DWORD dwNPCID,INT64 n64ItemID);
	typedef struct 
	{
		DWORD dwSkillID;
		DWORD dwNPCID;
		INT64 n64ItemID;
		TSpecSkillUsedFunc checkfunc;
		TSpecSkillUsedFunc learnfunc;
		TSpecSkillUsedFunc calcostfunc;
	} tagSpecSkillUsedParam,*PSpecSkillUsedParam;
	DWORD LearnSpecSkillLimit(PSpecSkillUsedParam param);

	DWORD CheckSoaringSkill(DWORD dwSkillID,DWORD dwNPCID,INT64 n64ItemID);
	DWORD CalSoaringSkillCost(DWORD dwSkillID,DWORD dwNPCID,INT64 n64ItemID);
	DWORD LearnSoaringSkillSpec(DWORD dwSkillID,DWORD dwNPCID,INT64 n64ItemID);
public:
	VOID	SynetRiderSpeed(Role*pRider);
	VOID	RestorOwnerSpeed(VOID);

	DWORD LearnSoaringSkill(DWORD dwSkillID);
	INT GetPracticeSoaringSkillTimes() const {return m_nSoaringSkillLearnTimes;}

	VOID OnReadyToSoaring(VOID);

	VOID OnCompletePracticeEquipment(DWORD dwEquipTypeID);
	VOID OnCompleteCastingEquipment(DWORD dwEquipTypeID,INT nCastingLevel);

	VOID SynRemoteBodyFabaoInfo(tagRemoteRoleFabaoInfo & info);

	VOID SetSoaringAtt(INT v);

// 角色战斗挂机系统状态初始化
public:
	VOID SetAutoFightState(EAutoFightState eAutoFightState)
	{
		if (eAutoFightState == EAFS_Close ||	// 没有“战斗符”道具状态
			eAutoFightState == EAFS_Start ||	// 开始挂机
			eAutoFightState == EAFS_End)		// 未开始
		{
			m_eAutoFightState = eAutoFightState;
			if (eAutoFightState == EAFS_Start)
				TryAddBuffToMyself(AUTO_FIGHT_BUFF_ID);
			else
				RemoveBuff(Buff::GetIDFromTypeID(AUTO_FIGHT_BUFF_ID), TRUE);
		}
	}
	EAutoFightState GetAutoFightState()
	{
		return m_eAutoFightState;
	}

	boolean CanAutoFight()
	{
		if (m_eAutoFightState == EAFS_Start &&
			GetItemMgr().GetBag().GetZhanDouFuCnt() > 0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	INT64 GetCurZhanDouFu64ID() { return m_n64CurZhanDouFu; }
	VOID SetCurZhanDouFu64ID(INT64 n64CurZhanDouFu) { m_n64CurZhanDouFu = n64CurZhanDouFu; }
private:
	EAutoFightState m_eAutoFightState;		// 角色的战斗挂机系统状态
	INT64 m_n64CurZhanDouFu;					// 角色当前使用的战斗符64位ID

	// Jason 2010-6-12 v1.0.0装备强化次数
	INT		m_nConsolidateTimes;

	set<DWORD> m_AttackSet;		// 攻击列表，pk预警使用，Jason 2010-7-17 v1.0.0
	set<DWORD> m_DefenseSet;		// 防御列表，pk预警使用，Jason 2010-7-17 v1.0.0
public:
	INT		GetConsolidateTimes() const;
	VOID	IncConsolidateTimes(INT add = 1);

	//--------------------------------------------------------------------------------------------
	// 从角色装备栏删除装备
	//--------------------------------------------------------------------------------------------
	DWORD DeleteEquipFromEquipBar(DWORD dwTypeID, INT nCount, DWORD dwCmdID);

	// Jason 2010-7-6 v1.0.0
	static VOID OnScriptNoticeOfFabaoAttChange(Role*pRole,tagFabao * pFabao,INT nEventType);
	enum {EFABAO_ChangePractice_Success = 0,EFABAO_ChangePractice_Failse,EFABAO_ChangePractice_AlreadyFull,EFABAO_ChangePractice_RoleLevelLower};
	static INT ChangeFabaoPracticeValue( Role * pRole,tagFabao * pFabao,INT addvalue,INT & nChanged );

	// 搜背包的东西，尽量少调用，Jason 2010-7-6 v1.0.0
	BOOL ModWoreFabao(INT nAddValue,INT & nChanged );

	BOOL InSamePKState(Role * pRole);
	BOOL InSamePKStateEx(Role * pRole);

	enum {
		EAD_NULL		= 0x00,
		EAD_Attack		= 0x01,
		EAD_Defense	= 0x02
	};
	// Jason 2010-7-17 v1.0.0
	// 测试某个角色是被自己攻击的，还是防御的
	DWORD TestAttackDefenseRelationship(DWORD dwRoleID);
	// 清空攻击防御信息，在死亡一方或者脱离战斗一方调用；
	VOID ClearAttackDefenseRelationship(VOID);
	// 把这个人从自己的攻击防御集合中删除，当对方脱离战斗或者死亡时调用
	VOID RemoveAttackDefenseRelationship(DWORD dwRoleID);
	VOID RemoveAttackDefenseRelationshipWithMe();
	// 增加攻击方
	VOID AddAttackRelationship(DWORD dwRoleID);
	// 增加防御方
	VOID AddDefenseRelationship(DWORD dwRoleID);

	// 判断是否在Team拾取范围内
	BOOL IsInTeamPickupRank(DWORD dwMapID, const Vector3& vPos);

	// 带有妖精经验分成
	VOID ExpChangeWithFabaoExp(INT64 nValue, BOOL bKill=FALSE, BOOL bForce=FALSE, INT nFabaoChanged = 0);

public:
	VOID TryAddBuffToMyself(DWORD dwBuffID);

private:
	VOID KillCountInc(ERoleKillType eRoleKillType);

	DWORD m_dwMonsterKillCount;	// 击杀怪物计数
	DWORD m_dwRoleKillCount;	// 击杀角色计数

private:

	// 角色当前月在线时长(在线时长单位为分钟)
	tagMonthOnlineTime m_CurMonthOnlineTime;

	VOID InitCurMonthOnlineTime(const BYTE* &pData, INT nNum);
	VOID UpdateCurMonthOnlineTime(BOOL bForceSave = FALSE);
public:
	// 保存身上的妖精
	VOID SaveRoleFabaoChange(VOID);
public:
	INT GetWearyUpperLimit()
	{
		INT nWearyUpperLimit = 100;
		EVipLevel eVipLvl = (EVipLevel)GetAttValue(ERA_VipLevel);
		switch (eVipLvl)
		{
		case EVL_Silver: // 银钻级vip
			nWearyUpperLimit = 120;
			break;
		case EVL_Gold: // 金钻级vip
			nWearyUpperLimit = 150;
			break;
		case EVL_Platinum: // 白金级vip
			nWearyUpperLimit = 200;
			break;
		case EVL_None: // 不是vip
		default:
			break;
		}
		return nWearyUpperLimit;
	}
	UINT16 GetMaxFreeWorldTalkTimes()
	{
		UINT16 n16MaxFreeWorldTalkTimes = 0;
		EVipLevel eVipLvl = (EVipLevel)GetAttValue(ERA_VipLevel);
		switch (eVipLvl)
		{
		case EVL_Silver: // 银钻级vip
			n16MaxFreeWorldTalkTimes = 50;
			break;
		case EVL_Gold: // 金钻级vip
			n16MaxFreeWorldTalkTimes = 80;
			break;
		case EVL_Platinum: // 白金级vip
			n16MaxFreeWorldTalkTimes = 100;
			break;
		case EVL_None: // 不是vip
		default:
			break;
		}
		return n16MaxFreeWorldTalkTimes;
	}
	INT GetInjuryRateDerate()
	{
		EVipLevel eVipLvl = (EVipLevel)GetAttValue(ERA_VipLevel);
		switch (eVipLvl)
		{
		case EVL_Gold: // 金钻级vip
			return 25;
		case EVL_Platinum: // 白金级vip
			return 50;
		case EVL_Silver: // 银钻级vip
		case EVL_None: // 不是vip
		default:
			return 0;
		}
	}

	INT GetPlayActFriendLimit()
	{
		EVipLevel eVipLvl = (EVipLevel)GetAttValue(ERA_VipLevel);
		switch (eVipLvl)
		{
		case EVL_Gold: // 金钻级vip
			return 6;
		case EVL_Platinum: // 白金级vip
			return 8;
		case EVL_Silver: // 银钻级vip
			return 4;
		case EVL_None: // 不是vip
		default:
			return 2;
		}
	}

	UINT16 GetFreeWorldTalkTimes()
	{
		return m_n16FreeWorldTalkTimes;
	}
	VOID SetFreeWorldTalkTimes(UINT16 n16FreeWorldTalkTimes)
	{
		m_n16FreeWorldTalkTimes = n16FreeWorldTalkTimes;
	}

	VOID UpdateVipLevel();
	VOID UseVipCard(INT64 n64ItemID);

	VOID CheckVipStateWhenLogin();
	VOID ResetVipState();
	VOID AddVipBuff(EVipLevel eVipLevel);

	// 妖精心情相关更新
	VOID UpdateFabaoMood(VOID);
	// 妖精心情降低多少
	INT	 ModFabaoMood(INT v);

	// 圣灵属性相关更新，及召唤圣灵时角色圣灵值更新
	VOID UpdateHolyData(VOID);

private:
	DWORD m_dwVipMaxDays;						// Vip道具时限(单位为天)
	tagDWORDTime m_dwVipStartTime;	// Vip道具使用时间，没有使用过或者已经过期为0
	UINT16 m_n16FreeWorldTalkTimes;	// Vip角色已经使用的免费世界喊话的次数
	// Jason 2010-9-20优化经验
	BOOL			m_bSaveExpDirect;		// 是否可以直接保存经验，不受计数器影响
	static INT		m_nNeedSaveExpCounter;	// 需要保存当前经验的计数器，按比之前，优化5-10倍的方式
	INT		m_nSaveExpCounter;			// 每次调用SaveExp2DB，都会增加一次

public:
	void UpdateChaseHolderOnWater();
	INT		m_nFabaoMoodUpdateTicks;		// 更新妖精心情

	INT		m_nHolyTicks;		// 消耗圣灵值的Tick
	INT		m_nCoValueTicks;	// 人物身上圣灵的默契值更新的Tick
public:
	BYTE GetBuy50LvlItemFlag()		{ return m_byBuy50LvlItemFlag; }
	BYTE GetBuy60LvlItemFlag()		{ return m_byBuy60LvlItemFlag; }
	BYTE GetBuy70LvlItemFlag()		{ return m_byBuy70LvlItemFlag; }
	BYTE GetBuy80LvlItemFlag()		{ return m_byBuy80LvlItemFlag; }
	VOID SetBuyLvlItemFlag(INT nLevel, BYTE byBuyLvlItemFlag)
	{
		switch (nLevel)
		{
		case 50:
			m_byBuy50LvlItemFlag = byBuyLvlItemFlag;
			break;
		case 60:
			m_byBuy60LvlItemFlag = byBuyLvlItemFlag;
			break;
		case 70:
			m_byBuy70LvlItemFlag = byBuyLvlItemFlag;
			break;
		case 80:
			m_byBuy80LvlItemFlag = byBuyLvlItemFlag;
			break;
		default:
			break;
		}
	}
	BYTE GetBuyLvlItemFlag(INT nLevel)
	{
		BYTE byHasBuy50To80LvlItem = 1;
		switch (nLevel)
		{
		case 50:
			byHasBuy50To80LvlItem = GetBuy50LvlItemFlag();
			break;
		case 60:
			byHasBuy50To80LvlItem = GetBuy60LvlItemFlag();
			break;
		case 70:
			byHasBuy50To80LvlItem = GetBuy70LvlItemFlag();
			break;
		case 80:
			byHasBuy50To80LvlItem = GetBuy80LvlItemFlag();
			break;
		default:
			byHasBuy50To80LvlItem = 1;
			break;
		}
		return byHasBuy50To80LvlItem;
	}

	// 发送一元计划原型表给客户端
	VOID SyncMallOfferProto();

	// 激活身上的妖精的特技buff，穿脱妖精时调用，其他时间不要调用
	BOOL ActiveFabaoStuntBuff(bool bEquip,tagFabao * pFabao,int nEquipPos);

	// 神铸成功的脚本事件
	VOID OnGodStrengthenSuccess(tagEquip*pEquip);


	BOOL HasTakenGuildDailyReward() const;
	VOID ResetGuildDailyRewardTime();

private:
	BYTE m_byBuy50LvlItemFlag;
	BYTE m_byBuy60LvlItemFlag;
	BYTE m_byBuy70LvlItemFlag;
	BYTE m_byBuy80LvlItemFlag;

	DWORD			m_dwPickupModeSetting;	// 拾取模式设置信息，位；影响宠物拾取，以及'~'拾取

	tagDWORDTime	m_timeTakeGuildReward;	// 领取公会每日奖励时间

	DWORD			m_dwVipNetBarInstBuffID;	// 台湾圣魔金牌网吧新需求，副本特殊buffid

	// 计算额外伤害
	virtual FLOAT CalAdditionalDmg(Skill * pSkill);
	// 附加减免，包括元素抗性
	virtual FLOAT CalAdditionalResistance(Skill * pSkill);
private:
	VOID  NoticeClient();

public:
	DWORD GetExpPilularUseTimes() { return m_dwExpPilularUseTimes; }
	VOID IncExpPilularUseTimes() { m_dwExpPilularUseTimes++; }
	VOID ResetExpPilularUseTimes() { m_dwExpPilularUseTimes = 0; }
	VOID ResetHolyToDayEatNum();

	// 台湾新金牌网吧需求 获取副本buffid
	DWORD	GetVipNetbarInstBuffID(VOID) const {	return m_dwVipNetBarInstBuffID; }
	// 台湾新金牌网吧需求 设置副本buffid
	VOID	SetVipNetbarInstBuffID(DWORD dwBuffId) { m_dwVipNetBarInstBuffID = dwBuffId; }

	INT		GetBuffWrapTimes(DWORD dwBuffTypeID,DWORD& dwSrcRoleID);

	void OnCreateGuild(DWORD dwGuildID);

	// 同步自身目标变化给九宫格内的其他的玩家(该玩家的目标必须是这个unit才能收到消息)
	VOID	SynTargetChangeToRolesOfVisTile();

	VOID	SynPickupModeSetting2Client();

	// 拾取模式属性
	VOID	PickupMode(DWORD dwMode) {m_dwPickupModeSetting = dwMode;}
	DWORD	PickupMode(VOID) const {return m_dwPickupModeSetting;}

	VOID	SynRemoteFabaoInfo(BOOL bEquip);

private:
	DWORD m_dwExpPilularUseTimes;
	BOOL		m_bPlayerBack;
	tagDWORDTime m_timeRealLogout; // 真正的上次登出时间
	UINT16		m_byPlayerBackDay;

public:
	EggBroker& GetEggBroker() { return m_EggBroker; }

	enum {EPlyBack_Recv = 1,EPlyBack_NotRecv = 2, EPlyBack_NotTimeout=0};
	BOOL IsPlayerBack() const {return m_bPlayerBack == EPlyBack_Recv;}
	BOOL IsPlayerBackNotRecv() const {return m_bPlayerBack == EPlyBack_NotRecv;}
	VOID PlayerBack(BOOL b) { m_bPlayerBack = b;}
	UINT16	GetPlayerBackDays() const {return m_byPlayerBackDay;}
	VOID SetPlayerBackDays(UINT16 y) {m_byPlayerBackDay = y;}

private:
	EggBroker m_EggBroker;
	// 是否使用配置文件里面的最小等级
	BOOL		m_bUsingPkMinLevel;

	INT		m_nCondenseCoolDown; // 凝聚倒计时，5s
public:
	bool IsOldSerNewPlayer();
	tagDWORDTime GetRealLogoutTime() const  {return m_timeRealLogout;}

	//---------------------------------------------------------------------------------
	// 获得一个临时技能，变形
	//---------------------------------------------------------------------------------
	INT	LearnExtraSkill(DWORD dwSkillID, INT nLevel);

	//---------------------------------------------------------------------------------
	// 删除一个临时技能，变形
	//---------------------------------------------------------------------------------
	INT	RemoveExtraSkill(DWORD dwSkillID);

	BOOL StartCondense(); // 开始凝聚
	VOID StopCondense( INT nType, DWORD dwErr = 0,BOOL bSendMsg = FALSE);	// 结束或打断凝聚

	//晋升神职（非竞选）
	DWORD DirectPromotionClergy(DWORD dwClergyID);
	
	// 打断凝聚神格
	VOID InterruptCondense();
private:
	VOID UpdateCondense();	// 更新凝聚
	DWORD CondenseGodHead();	// 凝聚神格

public:	
	//晋升神职操作，不论直接晋升还是竞选晋升都可以走这个接口
	DWORD PromotionClergy(DWORD dwClergyID, bool bNeedSavePreClergy = true);

	// 判断是否可以晋升神职
	DWORD CanPromotionClergy(DWORD dwClergyID);
	

	BOOL IsUsingPKConstraintsSafeGuradMinLevel() const {return m_bUsingPkMinLevel;}

	// 玩家是否同意到20级关闭pk保护模式，注意该函数只在升级到20，提示玩家后调用
	VOID ClosePKSafeGuard(BOOL bPlyCloseOrNot);

public:
	Wardrobe& GetWardrobe() { return m_Wardrobe; }
	EFashionState OnFashionChange(DWORD dwIdx, DWORD& dwLayerID);
	VOID GetFashionTemperamentSum(INT& nSum, INT& nSuit1Num, INT& nSuit2Num, INT& nSuit3Num, INT& nSuitSum) 
		{ m_Wardrobe.GetFashionTemperamentSum(nSum, nSuit1Num, nSuit2Num, nSuit3Num, nSuitSum); }

private:
	Wardrobe m_Wardrobe;
	INT			m_nTransformSkinNdx;

	BOOL		m_bCondensing;		//正在凝聚
public:
	VOID		SetTransSkinNdx(INT ndx,BOOL bSendBigVisMsg = FALSE, BOOL bSendAvatarEquip = TRUE);

	void			SyncRoleSkin2RemoteRole();
	void			SendAvatarEquip2Client();
	INT			GetTransSkinNdx() const { return m_nTransformSkinNdx; }
	VOID		OnCompleteFashionSuitNumChg() 
	{
		if (P_VALID(m_pScript))
			m_pScript->OnCompleteFashionSuitNumChange(this);
	}
	VOID		OnWardLevelUp()
	{
		if(P_VALID(m_pScript))
			m_pScript->OnRoleWardLevelup(this);
	}

//神系统领域相关
private:
	AreaMgr		m_GodAreaMgr;

public:
	AreaMgr&		GetGodAreaMgr()	{ return m_GodAreaMgr; }

	//激活神职领域能力
	DWORD			ChangeAreaAbility(DWORD dwAreaID, BOOL bActive, BOOL bByCD = TRUE);

	//增加神职领域
	DWORD			ChangeArea( DWORD dwAreaID,BOOL bAdd, tagGlobalLimitedItem *pGlobalLimitedItem = NULL, BOOL bDelGodFaith=TRUE );

	// 初始化领域
	VOID			InitArea(const BYTE* &pData, INT32 nNum);

	// 保存神系统领域
	VOID			SaveArea2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);

	// 发送人物初始领域给客户端
	VOID			SendInitStateArea();

	//某种领域已激活的数量
	INT				GetAbilityAreaNum(EGodAreaType eType, BOOL bAll = TRUE);

	//随机减少一个稀有领域能力
	DWORD			RandCancleAreaAbility();

	//随机删除一个稀有领域
	DWORD			RandDelArea();

	//根据地图类型和ID更新角色的技能
	VOID			UpdateSkillByMap( EMapType eType, DWORD dwMapID );

	//根据地图类型和ID更新角色的技能
	VOID			UpdateSkillByMap( DWORD dwSkillID, EMapType eType, DWORD dwMapID );

	//重新激活技能
	VOID			ResumeUseSkillByMap( Skill *pSkill );

	//技能禁止使用
	VOID			ForbidUseSkillByMap( Skill *pSkill );

	//通过领域直接加技能
	VOID			AddSkillByArea(DWORD dwSkillID);

	VOID			ReCalRoleAtt();

	VOID			UpdateArea();

	//获得领域CD时间
	DWORD			GetAreaCD(DWORD dwAreaID);

	//去掉稀有领域特效buff
	VOID			RemoveAreaBuff();
	DWORD			GetRandRareArea();

	BOOL			IsHaveArea(DWORD dwAreaID);

	BOOL			DirectDelOneRareArea(DWORD dwAreaID);

	//在该地图技能是否可用
	BOOL			CanUseSkillByMap(DWORD dwSkillID, EMapType eType, DWORD dwMapID);

public:
	VOID RoleRecalAtt()
	{
		// 重新计算属性
		if( NeedRecalAtt() )
			RecalAtt();
	}

//家族妖精相关
public:
	//修炼家族妖精
	DWORD	TrainFamilySprite( DWORD dwTrainID, INT &nRet, INT64 n8ItemID, BOOL bAgain = FALSE );

	//登记玩家妖精
	DWORD	FamilySpriteRegister( INT64 n8ID );

	//取回玩家妖精
	DWORD	FamilySpriteGetBack( );

	//直接返回给角色登记的妖精
	VOID	FamilyRoleSpriteReturn();

	//把从家族发回的登记的妖精放回背包、仓库、百宝袋内
	VOID	AddRoleSpriteFromFamily( tagFabao *pFabao );

	//直接设置玩家登记妖精
	VOID	SetFamilyFabao(tagItem *pItem);

	//角色登陆时更新登记妖精带来的属性加成
	VOID	UpdateRoleFamilySprite();

private:
	//是否可以修炼家族妖精
	DWORD	CanTrainFamilySprite( DWORD dwTrainID, INT64 n8ItemID, BOOL bAgain = FALSE );

	//是否可以登记玩家妖精
	DWORD	CanFamilySpriteRegister( INT64 n8ID );

	//是否可以取回玩家妖精
	DWORD	CanFamilySpriteGetBack( INT64 n8ID );

	//初始化玩家的家族妖精增加的能力属性
	VOID	InitAbilityFromFamilySprite();


private:
	DWORD m_dwKeyCodeRewarded;	// 是否领取过key码奖励,未领取过的这个值为0，领取过的这个值是他使用的key码
public:
	DWORD GetKeyCodeRewardedState() { return m_dwKeyCodeRewarded; }
	VOID SetKeyCodeRewardedState(DWORD dwKey) { m_dwKeyCodeRewarded = dwKey; }

private:
	// 是否今天首次登陆
	BOOL m_bTodayFirstLogin;
public:
	BOOL IsTodayFirstLogin()
	{
		return m_bTodayFirstLogin;
	}
	VOID SyncFamilyActive(INT nActive);


	VOID SendInitStateClientConfig();

// 商城物品限量购买相关
private:
	TMap<DWORD, INT> m_mapBuyMallItemCount;	// 角色购买商城物品计数 <物品id，购买个数>
	VOID SaveBuyMallItemCount2DB(OUT LPVOID pData, OUT LPVOID &pOutPointer, OUT INT32 &nNum);
	VOID InitBuyMallItemCount(const BYTE* &pData, INT32 nNum);
	VOID ClearBuyMallItemCount();

public:
	INT BuyThisMallItemCount(DWORD dwTypeID);
	VOID IncreaseRoleBuyMallItemCount(DWORD dwTypeID, INT nNum);

private:
	BOOL m_bSafeguardFlag;

public:
	VOID SetRoleSafeguardFlag(BOOL bSafeguardFlag){m_bSafeguardFlag = bSafeguardFlag;}
	BOOL GetRoleSafeguardFlag( ){ return m_bSafeguardFlag;}

private:
	DWORD	m_dwTrainDate;
	INT		m_nTrainNum;
	INT		m_nAwardPoint;
	INT		m_nAwardFlag;

public:
	VOID	UpdateFamilyTrainNum();

	VOID	AddRoleAwardPoint(INT nData);

	BOOL	CanAddGiftByAwardPoint(DWORD &dwID, DWORD &dwGiftID);
	DWORD	AddGiftByAwardPoint();

	INT		GetAwardPoint(){return m_nAwardPoint;}
	INT		GetAwardFlag(){return m_nAwardFlag;}
private:
	BloodbrotherMgr m_BrotherMgr;
public:
	BloodbrotherMgr& GetBrotherMgr(){return m_BrotherMgr;}
	// 是否是结拜兄弟
	bool IsBrotherOf(DWORD dwID);
	VOID MakeBorther(DWORD dwID,bool IsTeacher = false);
	VOID MakeBrotherSuccess(DWORD dwRoleID);
	VOID	CancelBrotherProfit();
	VOID	SyncBrotherEndTime();
	VOID	SyncClient2EndBloodBrother();

private:
	DWORD m_dwRoleLogData[DataCollection_Max_RoleData_Num];
public:
	VOID InitLogData(const DWORD* p) { memcpy(m_dwRoleLogData, p, sizeof(DWORD)*DataCollection_Max_RoleData_Num ); }
	DWORD GetData(BYTE byIndex) { if(DataCollection_Role_Index_Valid(byIndex)) return m_dwRoleLogData[byIndex]; }
	VOID SetData(BYTE byIndex, DWORD dwdata) { if(DataCollection_Role_Index_Valid(byIndex)) m_dwRoleLogData[byIndex] = dwdata; }

public:
	DWORD ActivePulse(DWORD dwID);
	VOID ResetPulseCanLearnNum();
	INT GetPulseLearnTime(){return m_nPulseLearnTimes; }
private:
	DWORD CheckPulseSkill(DWORD dwID,DWORD dwNPCID,INT64 n64ItemID);
	DWORD CalPulseSkillCost(DWORD dwID,DWORD dwNPCID,INT64 n64ItemID);
	DWORD LearnPulseSkillSpec(DWORD dwID,DWORD dwNPCID,INT64 n64ItemID);
	INT			m_nPulseLearnTimes;	// 当天打通经脉的次数

	// 装备成长相关函数
public:
	DWORD OnRoleEquipGetGrowID(DWORD dwTypeID, BYTE byQlty);
	BYTE OnRoleEquipGrow(DWORD dwTypeID, BYTE byQlty, INT nGrowTimes, INT nIMUseTimes, BYTE byGrowLevel, 
		BYTE byGrayNum, BYTE byGreenNum, BYTE byBlueNum, BYTE byPurpleNum, DWORD dwIM);
	DWORD OnRoleUseDiamondEquipGrowIM(DWORD dwTypeID, INT64 n64EquipID);
	DWORD OnRefreshBaoXiang();
	DWORD OnGetBaoXiang(BYTE byIndex, DWORD dwItemID);

public:
	void	SetGetCash(BOOL bGetCash){m_bGetCash = bGetCash;}
	BOOL	GetGetCash(){return m_bGetCash;}

	VOID	SetRoleBagYuanBao(INT32 nYuanbao){m_CurMgr.SetCurBagYuanBao(nYuanbao);}
	VOID	SetRoleBaibaoYuanBao(INT32 nYuanbao){m_CurMgr.SetCurBaiBaoYuanBao(nYuanbao);}

	VOID	SendBagYuanBao(){m_CurMgr.SendBagYuanBao();}
	VOID	SendBaibaoYuanBao(){m_CurMgr.SendBaibaoYuanBao();}

private:
	BOOL	m_bGetCash;		//是否已经成功获得圣币

private:
	TigerMgr	m_TigerMgr;
public:
	TigerMgr& GetTigerMgr(){return m_TigerMgr;}

private:
	BOOL	m_bInitPkState;

public:
	INT GetTigerTime(){return m_nTigerTimes; }
	VOID AddTigerTime(){m_nTigerTimes++;}
private:
	INT			m_nTigerTimes;	// 玩老虎机的次数

// 魂晶相关
public:
	VOID RenewSoulCrystalAtt(DWORD dwTypeID, BYTE byQlty, INT* nAtt, OUT DWORD& dwErrorCode, OUT INT* nRetAtt);
	DWORD GetSkillIDFromSoulCrystal()
	{
		tagEquip* pEquip = GetItemMgr().GetEquipBar().GetItem((INT16)EEP_HunJing);
		if(!P_VALID(pEquip)) return 0;
		
		tagSoulCrystal* pSoul = (tagSoulCrystal*)pEquip;
		return (pSoul->SoulSpec.dwStaticSkillID);
	}

// 玩家回归
public:
	tagAccountReactiveInfo m_AccountReactiveInfo;
	VOID InitAccountReactiveInfo(tagAccountReactiveInfo* pData)
	{
		IFASTCODE->MemCpy(&m_AccountReactiveInfo, pData, sizeof(tagAccountReactiveInfo));
	}
	VOID AccountSuccessReactive();
	//DWORD RewardReactive(BYTE byRewardLevel);
	EAccountReactiveIdentity IsAccountReactiver() {return m_AccountReactiveInfo.eIdentity;} // 该角色是否是玩家回归介绍人
	DWORD GetAccountReactiveKey() 
	{
		if(IsAccountReactiver())
			return m_AccountReactiveInfo.dwKey;
		else
			return 0;
	}
	INT GetAccountReactiveScore()
	{
		if(IsAccountReactiver())
			return m_AccountReactiveInfo.nScore;
		else
			return 0;
	}
	VOID AddAccountReactiveScore(INT nScore)
	{
		m_AccountReactiveInfo.nScore += nScore;
	}
	
	VOID SetRewardReactiveLevel(BYTE byLevel) 
	{ 
		if(byLevel<=10) 
			m_AccountReactiveInfo.byRewardLevel = byLevel;

		tagNDBC_UpdateRewardReactiveLevel send;
		send.dwAccountID = m_pSession->GetSessionID();
		send.byLevel = byLevel;
		g_dbSession.Send(&send, send.dwSize);
	}

	BYTE GetRewardReactiveLevel() { return m_AccountReactiveInfo.byRewardLevel; }

//------------------------神魂魔魂相关---------------------------
private:
	INT m_nGod;					//神魂
	INT m_nMonster;				//魔魂
	INT m_nGodPoint;			//神魂声望
	INT m_nMonsterPoint;		//魔魂声望
	TList<DWORD>				m_listSkillActiveTime;		// 计时技能列表

public:
	INT		GetGod(){return m_nGod;}
	VOID	SetGod( INT nGod ){m_nGod = nGod;}
	VOID ChgGod(INT nChgGod){m_nGod += nChgGod; if(m_nGod<0) m_nGod=0;}

	INT		GetMonster(){return m_nMonster;}
	VOID	SetMonster( INT nMonster ){m_nMonster = nMonster;}

	INT		GetGodPoint(){return m_nGodPoint;}
	VOID	SetGodPoint( INT nGodPoint ){m_nGodPoint = nGodPoint;}

	INT		GetMonsterPoint(){return m_nMonsterPoint;}
	VOID	SetMonsterPoint( INT nMonsterPoint ){m_nMonsterPoint = nMonsterPoint;}
	
	DWORD	ActiveSoulSkill(DWORD dwSkillID,DWORD &dwTime);
	DWORD	ContinueSoulSkill(DWORD dwSkillID, INT64 n64ItemID, DWORD &dwTime);

	//改变神魔魂
	VOID	ChangeSoul( INT nGod, INT nMonster );
	//改变神魔声望
	VOID	ChangeSoulPoint( INT nGodPoint, INT nMonsterPoint );

	BYTE	GetGodSoulLevel();
	BYTE	GetMonsterSoulLevel();

	VOID	ContinueSoulSkillByBook( DWORD dwSkillID, tagDWORDTime time );

private:
	DWORD	CanActiveSoulSkill( DWORD dwSkillID );
	DWORD	CanContinueSoulSkill( DWORD dwSkillID, INT64 n64ItemID );
	//通过神魔直接加技能
	DWORD	AddSkillBySoul(DWORD dwSkillID,BOOL bForerver, DWORD &dwTime);
	//过滤神魔魂技能
	VOID	FiltrateSoulSkill();
//------------------------神魂魔魂相关end---------------------------

//------------------------韩国billing相关---------------------------
public:
	VOID	UpdateGetCash();
	BOOL	DecIPGCash(int nYuanbao, int nNum=0, DWORD dwItemTypeID=0 );

	int		GetIPGCash();

//------------------------韩国billing相关end---------------------------
public:
	VOID	CheckAllEquip(DWORD dwMapID = GT_INVALID);
//------------------------新Kye码--------------------------------------
public:
	DWORD	AddNewKeyCodeGift(DWORD dwKeyCodeCrc);

	BOOL	IsInWar();
	void	SetWarRole(WarRole* pWarRole);
	WarRole* GetWarRole();
	BOOL    IsUseSkill();
private:
	WarRole* m_pWarRole;
};
