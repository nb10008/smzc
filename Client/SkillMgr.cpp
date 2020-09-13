#include "stdafx.h"
#include "SkillMgr.h"
#include "SkillProtoData.h"
#include "SkillEvent.h"
#include "CombatEvent.h"
#include "ItemMgr.h"
#include "Item.h"
#include "NPC.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "EffectMgr.h"
#include "CombatSysUtil.h"
#include "ShortcutBarFrame.h"
#include "LevelUpData.h"
#include "FairyTrainFrame.h"
#include "HolyManMgr.h"
#include "Container.h"

SkillMgr::SkillMgr():m_Trunk(this)
{
	m_dwLastUseSkillTime = 0;
}

SkillMgr::~SkillMgr()
{
}

SkillMgr g_skillmgr;
SkillMgr* SkillMgr::Inst()
{
	return &g_skillmgr;
}

void SkillMgr::Init()
{
	m_pCmdMgr = TObjRef<NetCmdMgr>();
	m_pSession = TObjRef<NetSession>();
	m_pUtil	= TObjRef<Util>();
	m_pFrameMgr = TObjRef<GameFrameMgr>();
	m_dwLastUpdateTime = timeGetTime();


	//--注册网络消息处理函数
	m_pCmdMgr->Register("NS_GetRoleInitState_Skill",	(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetInitLocalPlayerSkill), _T("NS_GetRoleInitState_Skill"));
	m_pCmdMgr->Register("NS_AddSkill",					(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetAddSkill),			  _T("NS_AddSkill"));
	m_pCmdMgr->Register("NS_UpdateSkill",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetUpdateSkill),		  _T("NS_UpdateSkill"));
	m_pCmdMgr->Register("NS_RemoveSkill",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetRemoveSkill),		  _T("NS_RemoveSkill"));
	m_pCmdMgr->Register("NS_LearnSkill",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetLearnSkill),			  _T("NS_LearnSkill"));
	m_pCmdMgr->Register("NS_LevelUpSkill",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetLevelUpSkill),		  _T("NS_LevelUpSkill"));
	m_pCmdMgr->Register("NS_ForgetSkill",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetForgetSkill),		  _T("NS_ForgetSkill"));
	m_pCmdMgr->Register("NS_ClearTalent",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetClearTalent),		  _T("NS_ClearTalent"));
	m_pCmdMgr->Register("NS_AddTalent",					(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetAddTalent),			  _T("NS_AddTalent"));
	m_pCmdMgr->Register("NS_RemoveTalent",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetRemoveTalent),		  _T("NS_RemoveTalent"));
	m_pCmdMgr->Register("NS_UpdateTalent",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetUpdateTalent),		  _T("NS_UpdateTalent"));
	m_pCmdMgr->Register("NS_UpdateSkillCoolDown",		(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetUpdateSkillCD),		  _T("NS_UpdateSkillCoolDown"));
	m_pCmdMgr->Register("NS_UpdateSkillCoolDownEx",		(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetUpdateSkillCDEx),	  _T("NS_UpdateSkillCoolDownEx"));
	m_pCmdMgr->Register("NS_RoleGetTalentPoint",		(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::OnNS_RoleGetTalentPoint), _T("NS_RoleGetTalentPoint") );
	m_pCmdMgr->Register("NS_Skill",						(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::OnNS_Skill),			  _T("NS_Skill"));

	//--注册游戏事件
}

void SkillMgr::Destroy()
{
	FreeMap();

	m_pCmdMgr->UnRegister("NS_GetRoleInitState_Skill",	(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetInitLocalPlayerSkill));
	m_pCmdMgr->UnRegister("NS_AddSkill",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetAddSkill));
	m_pCmdMgr->UnRegister("NS_UpdateSkill",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetUpdateSkill));
	m_pCmdMgr->UnRegister("NS_RemoveSkill",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetRemoveSkill));
	m_pCmdMgr->UnRegister("NS_LearnSkill",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetLearnSkill));
	m_pCmdMgr->UnRegister("NS_LevelUpSkill",			(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetLevelUpSkill));
	m_pCmdMgr->UnRegister("NS_ForgetSkill",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetForgetSkill));
	m_pCmdMgr->UnRegister("NS_ClearTalent",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetClearTalent));
	m_pCmdMgr->UnRegister("NS_AddTalent",				(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetAddTalent));
	m_pCmdMgr->UnRegister("NS_RemoveTalent",			(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetRemoveTalent));
	m_pCmdMgr->UnRegister("NS_UpdateTalent",			(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetUpdateTalent));
	m_pCmdMgr->UnRegister("NS_UpdateSkillCoolDown",		(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetUpdateSkillCD));
	m_pCmdMgr->UnRegister("NS_UpdateSkillCoolDownEx",	(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::NetUpdateSkillCDEx));
	m_pCmdMgr->UnRegister("NS_RoleGetTalentPoint",		(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::OnNS_RoleGetTalentPoint));
	m_pCmdMgr->UnRegister("NS_Skill",					(NETMSGPROC)m_Trunk.sfp2(&SkillMgr::OnNS_Skill));
}


DWORD SkillMgr::NetInitLocalPlayerSkill(tagNS_GetRoleInitState_Skill* pNetCmd, DWORD dwParam)
{
	//清除技能数据
	FreeMap();

	for(INT j=0; j<X_MAX_TALENT_PER_ROLE; ++j)
	{
		SetTalentPoint(pNetCmd->Talent[j]);
	}

	//添加本地玩家技能信息
	for(INT i=0; i<pNetCmd->nNum; ++i)
	{
		AddSkill(pNetCmd->Skill[i]);
	}
	
	tagNC_GetRoleInitState m;
	m.eType = ERIT_Item;
	m_pSession->Send(&m);

	return 0;
}

void SkillMgr::AddSkill(tagSkillMsgInfo data)
{
	tagSkillData* pSkill = new tagSkillData;
	pSkill->dwSkillID = data.dwID;
	pSkill->nCurCD = data.nCoolDown;
	pSkill->nLevel = data.nLevel;
	pSkill->nLearnLevel = data.nLearnLevel;
	pSkill->nExp = data.nProficiency;
	pSkill->dwLimitTime = data.unActiveTime;
	pSkill->dwTypeID = MTransSkillTypeID(data.dwID, data.nLevel);
	tagSkillProtoClient* pProto = const_cast<tagSkillProtoClient*>(SkillProtoData::Inst()->FindSkillProto(pSkill->dwTypeID));
	if(P_VALID(pProto))
	{
		pSkill->pProto = pProto;
		m_skillMap.Add(pSkill->dwSkillID, pSkill);
		
		// 合成其他类型
		pSkill->nCurCD = data.nCoolDown;
		pSkill->nLearnLevel = data.nLearnLevel;
		pSkill->nCooldown = data.nMaxCoolDown;
		pSkill->nPrepareTime = data.nPrepareTime;
		pSkill->fOPDist = data.fOpDist;
		pSkill->nSkillDmg = 0;
		for (INT i=0; i<MAX_CHANNEL_TIMES; ++i)
		{
			pSkill->nSkillDmg += data.nChannelDmg[i];
		}
		for (INT i=0; i<ESCT_End; ++i)
		{
			pSkill->nCost[i] = data.nCost[i];
		}
		// 如果冷却倒计时不等于零则更新冷缺计算开始时间
		if(0 != data.nCoolDown)
		{
			INT nIncMilliSecond = data.nCoolDown - data.nMaxCoolDown;
			pSkill->dwStartTime = Kernel::Inst()->GetAccumTimeDW() + nIncMilliSecond;
		}

		// 发送游戏事件
		tagUpdateLPSkillEvent event(_T("tagUpdateLPSkillEvent"),NULL);
		event.bAdd	= TRUE;
		event.dwID	= pSkill->dwSkillID;
		event.eType = pSkill->pProto->eUseType;
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}
	else
	{
		SAFE_DEL(pSkill);
	}
}

void SkillMgr::RemoveSkill(DWORD dwID)
{
	tagSkillData* pSkill = m_skillMap.Peek(dwID);
	if(P_VALID(pSkill))
	{
		BOOL bRet = m_skillMap.Erase(dwID);
		if(bRet)
		{
			// 发送游戏事件
			tagUpdateLPSkillEvent event(_T("tagUpdateLPSkillEvent"),NULL);
			event.bAdd  = FALSE;
			event.dwID  = dwID;
			event.eType = pSkill->pProto->eUseType;
			TObjRef<GameFrameMgr>()->SendEvent(&event);
			
			//从列表中删除后，删除该指针
			SAFE_DEL(pSkill);
		}
	}
}

void SkillMgr::UpdateSkill( tagSkillMsgInfo data )
{
	tagSkillData* pSkill = m_skillMap.Peek(data.dwID);
	if(P_VALID(pSkill))
	{
		// 等级不同，则模板也要发生变化
		if(pSkill->nLevel != data.nLevel)
		{
			pSkill->nLevel = data.nLevel;
			pSkill->dwTypeID = MTransSkillTypeID(data.dwID, data.nLevel);
			tagSkillProtoClient* pProto = const_cast<tagSkillProtoClient*>(SkillProtoData::Inst()->FindSkillProto(pSkill->dwTypeID));
			if(P_VALID(pProto))
			{
				pSkill->pProto = pProto;
			}
			else
			{
				pSkill->pProto = NULL;
			}
		}
		
		// 合成其他类型
		pSkill->nCurCD = data.nCoolDown;
		pSkill->nLearnLevel = data.nLearnLevel;
		pSkill->nCooldown = data.nMaxCoolDown;
		pSkill->nPrepareTime = data.nPrepareTime;
		pSkill->nSkillDmg = 0;
		pSkill->nExp = data.nProficiency;
		pSkill->fOPDist = data.fOpDist;
		for (INT i=0; i<MAX_CHANNEL_TIMES; ++i)
		{
			pSkill->nSkillDmg += data.nChannelDmg[i];
		}
		for (INT i=0; i<ESCT_End; ++i)
		{
			pSkill->nCost[i] = data.nCost[i];
		}
		// 如果冷却倒计时不等于零则更新冷缺计算开始时间
		if(0 != data.nCoolDown)
		{
			INT nIncMilliSecond = (INT)(data.nCoolDown - data.nMaxCoolDown);
			pSkill->dwStartTime = Kernel::Inst()->GetAccumTimeDW() + nIncMilliSecond;
		}
		
		// 发送游戏事件
		tagUpdateLPSkillEvent event(_T("tagUpdateLPSkillEvent"),NULL);
		event.bAdd  = FALSE;
		event.dwID  = pSkill->dwSkillID;
		event.eType = pSkill->pProto->eUseType;
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}
}

DWORD SkillMgr::GetNormalAttackID()
{
	Item* pItem = ItemMgr::Inst()->GetEquipment()->GetValue((INT16)EEP_RightHand);
	//判断是否为幻彩武器
	
	if(P_VALID(pItem))
	{
		switch(pItem->GetItemTypeID())
		{
		case 8019006://幻彩长刃
			return BT_NORMAL_ATTACK_SKILL_ID[0];
		case 8029006://幻彩弓箭
			return BT_NORMAL_ATTACK_SKILL_ID[1];
		case 8049006://幻彩法器
			return BT_NORMAL_ATTACK_SKILL_ID[2];
		case 8039006://幻彩魔琴
			return BT_NORMAL_ATTACK_SKILL_ID[3];
		}
	}

	EItemTypeEx eType = ItemMgr::Inst()->GetHandWeaponType();
	switch (eType)
	{
	case EITE_Wand:
		return NORMAL_ATTACK_SKILL_ID[1];
	case EITE_Bow:
		return NORMAL_ATTACK_SKILL_ID[2];
	case EITE_Faqi:
		return NORMAL_ATTACK_SKILL_ID[3];
	case EITE_Qin:
		return NORMAL_ATTACK_SKILL_ID[4];
	}
	return NORMAL_ATTACK_SKILL_ID[0];
}

void SkillMgr::GetGuildSkillList( TList<DWORD>& list )
{
	list.Clear();

	tagSkillData* pData;
	m_skillMap.ResetIterator();
	while( m_skillMap.PeekNext( pData ) )
	{
		//公会技能
		if( ESSTE_Guild == pData->pProto->nType2 && ESSTE2_Hidden != pData->pProto->nType3 )
		{
			list.PushBack( pData->dwSkillID );
		}
	}
}

void SkillMgr::GetTransformSkillList( TList<DWORD>& list )
{
	list.Clear();

	tagSkillData* pData;
	m_skillMap.ResetIterator();
	while( m_skillMap.PeekNext( pData ) )
	{
		if( ESSTE_Transform == pData->pProto->nType2 && ESSTE2_Hidden != pData->pProto->nType3 )
		{
			list.PushBack( pData->dwSkillID );
		}
	}
}

void SkillMgr::GetGodSkillList( TList<DWORD>& list )
{
	list.Clear();

	tagSkillData* pData;
	m_skillMap.ResetIterator();
	while( m_skillMap.PeekNext( pData ) )
	{
		if( (ESSTE_GodArea == pData->pProto->nType2  ||ESSTE_Transform == pData->pProto->nType2  || ESSTE_SingleTarget == pData->pProto->nType2 || ESSTE_SingleTargetC == pData->pProto->nType2)&& ESSTE2_Hidden != pData->pProto->nType3 )
		{
			list.PushBack( pData->dwSkillID );
		}
	}
}

void SkillMgr::GetSkillList( TList<DWORD>& List )
{
	//--加入徒手攻击的技能，普通攻击只显示一个，统一处理
	List.Clear();
	
	tagSkillData* pData;
	BOOL bRead = FALSE;
	m_skillMap.ResetIterator();
	EItemTypeEx weaponType = ItemMgr::Inst()->GetHandWeaponType();
	if( weaponType == EITE_Null) weaponType = ItemMgr::Inst()->GetHandWeaponType(EEP_LeftHand);
	while( m_skillMap.PeekNext( pData ) )
	{
		//只显示一个普通攻击
		if( IsNormalAttackSkill( pData->dwSkillID ) )
		{
			if(bRead)
				continue;
			else 
				bRead = TRUE;
		}

		ESkillTypeEx skillType = (ESkillTypeEx)pData->pProto->nType2;
		if(skillType == ESSTE_FreshHandNomal || skillType == ESSTE_FreshHandRanged || skillType == ESSTE_FreshHandEnhanced)
		{
			if(pData->pProto->nWeaponLimit!=weaponType) continue;
		}

		//只显示不隐藏的主动技能
		if( ESUT_Active == pData->pProto->eUseType && ESSTE2_Hidden != pData->pProto->nType3
			&& ESSTE_Produce != pData->pProto->nType2 && pData->pProto->nType2 != ESSTE_Guild
			&& ESSTE_Transform != pData->pProto->nType2 && ESSTE_GodArea != pData->pProto->nType2
			&& ESSTE_SingleTarget != pData->pProto->nType2 && ESSTE_SingleTargetC != pData->pProto->nType2
			&& ESSTE_SoulSubSkill != pData->pProto->nType2)
		{	
			List.PushBack( pData->dwSkillID );
		}
	}
}

DWORD SkillMgr::AdaptFreshHandSkillID(DWORD dwSkillID)
{
	tagSkillData* pData = m_skillMap.Peek(MTransSkillID(dwSkillID));
	if(P_VALID(pData))
	{
		ESkillTypeEx skillType = (ESkillTypeEx)pData->pProto->nType2;
		if(skillType == ESSTE_FreshHandNomal || skillType == ESSTE_FreshHandRanged || skillType == ESSTE_FreshHandEnhanced)
		{
			EItemTypeEx weaponType = ItemMgr::Inst()->GetHandWeaponType();
			if( weaponType == EITE_Null) weaponType = ItemMgr::Inst()->GetHandWeaponType(EEP_LeftHand);
			if(pData->pProto->nWeaponLimit!=weaponType )
			{
				m_skillMap.ResetIterator();
				while( m_skillMap.PeekNext( pData ) )
				{
					if(skillType == pData->pProto->nType2 && pData->pProto->nWeaponLimit==weaponType)
					{
						return MTransSkillTypeID(pData->dwSkillID, MTransSkillLevel(dwSkillID)); // 保留等级
					}
				}
				return GT_INVALID;
			}
		}
	}
	return dwSkillID;
}

void SkillMgr::GetPassiveSkillList( TList<DWORD>& List, TList<DWORD>& lstOther )
{
	//--加入徒手攻击的技能，普通攻击只显示一个，统一处理
	List.Clear();

	tagSkillData* pData;
	m_skillMap.ResetIterator();
	while( m_skillMap.PeekNext( pData ) )
	{
		//--只显示不隐藏的被动技能
		if( ESUT_Passive == pData->pProto->eUseType && ESSTE2_Hidden != pData->pProto->nType3  
			&& ESSTE_Transform != pData->pProto->nType2 && ESSTE_GodArea != pData->pProto->nType2 
			&& ESSTE_SingleTarget != pData->pProto->nType2 && ESSTE_SingleTargetC != pData->pProto->nType2
			&& ESSTE_SoulSubSkill != pData->pProto->nType2)
		{	
			if( pData->pProto->nType2 == ESSTE_Guild || pData->pProto->nType2 == ESSTE_LowerImmortal || 
				pData->pProto->nType2 == ESSTE_NormalImmortal || pData->pProto->nType2 == ESSTE_HighImmortal )
			{
				if(!lstOther.IsExist(pData->dwSkillID))
					lstOther.PushBack( pData->dwSkillID );
			}
			else
			{
				List.PushBack( pData->dwSkillID );
			}
		}
	}
}

void SkillMgr::GetProduceSkillList( TList<DWORD>& list )
{
	list.Clear();

	tagSkillData* pData;
	m_skillMap.ResetIterator();
	while( m_skillMap.PeekNext( pData ) )
	{
		//生产技能
		if( ESUT_Active == pData->pProto->eUseType && ESSTE_Produce == pData->pProto->nType2 && ESSTE2_Hidden != pData->pProto->nType3 )
		{
			list.PushBack( pData->dwSkillID );
		}
	}
}


const tagSkillData* SkillMgr::GetSkillData( DWORD dwID )
{
	if(IsNormalAttackSkill(dwID))
	{
		dwID = GetNormalAttackID();
	}
	return m_skillMap.Peek(dwID);
}


const tagSkillData* SkillMgr::GetSkillDataByTypeID( DWORD dwTypeID )
{
	return GetSkillData(MTransSkillID(dwTypeID));
}

void SkillMgr::FreeMap()
{
	tagSkillData *pSkill = NULL;

	m_skillMap.ResetIterator();
	while(m_skillMap.PeekNext(pSkill))
	{
		SAFE_DEL(pSkill);
	}
	m_skillMap.Clear();

	for(INT i=0; i<X_MAX_TALENT_PER_ROLE; ++i)
	{
		m_talentPoint[i].eType = ETT_Null;
		m_talentPoint[i].nPoint = NULL;
	}
}

DWORD SkillMgr::NetAddSkill( tagNS_AddSkill* pNetCmd, DWORD dwParam )
{
	AddSkill(pNetCmd->Skill);

    ShortcutBarFrame* pFrame = (ShortcutBarFrame*)m_pFrameMgr->GetFrame(_T("ShortcutBar"));
    if (P_VALID(pFrame))
    {
        const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillData(pNetCmd->Skill.dwID);
        if ( P_VALID(pSkill) && pSkill->pProto->eUseType == ESUT_Active
            && pSkill->pProto->nType3 != ESSTE2_Hidden
			&& pSkill->pProto->nType3 != ESSTE2_Mining
			&& pSkill->pProto->nType3 != ESSTE2_Harvestry )
            pFrame->AddNewSkill(MTransSkillTypeID(pNetCmd->Skill.dwID, pNetCmd->Skill.nLevel));
    }

	// 播放特效
	EffectMgr::Inst()->PlayRoleEffect( RoleMgr::Inst()->GetLocalPlayerID(), _T("Txxy06") );

	return 0;
}

DWORD SkillMgr::NetUpdateSkill( tagNS_UpdateSkill* pNetCmd, DWORD dwParam )
{
	UpdateSkill(pNetCmd->Skill);

	return 0;
}

DWORD SkillMgr::NetRemoveSkill( tagNS_RemoveSkill* pNetCmd, DWORD dwParam )
{
	RemoveSkill(pNetCmd->dwSkillID);

	return 0;
}

DWORD SkillMgr::NetLearnSkill( tagNS_LearnSkill* pNetCmd, DWORD dwParam )
{
	switch (pNetCmd->dwErrorCode)
	{
	case E_Success:
		break;
	case E_LearnSkill_ClassLimit:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_LearnSkill_ClassLimit")]);
		}
		break;
	case E_LearnSkill_ProtoNotFound:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("LearnSkill_ProtoNotFound")]);
		}
		break;
	case E_LearnSkill_Existed:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("LearnSkill_Existed")]);
		}
		break;
	case E_LearnSkill_NoPreSkill:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_LearnSkill_NoPreSkill")]);
		}
		break;
	case E_LearnSkill_NeedMoreTalentPoint:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_LearnSkill_NeedMoreTalentPoint")]);
		}
		break;
	default:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("LearnSkill_CanNotLearn")]);
		}
		break;
	}
	return 0;
}

DWORD SkillMgr::NetLevelUpSkill( tagNS_LevelUpSkill* pNetCmd, DWORD dwParam )
{
	switch( pNetCmd->dwErrorCode )
	{
	case E_Success:
		break;
	case E_LevelUpSkill_NeedMoreLevel:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_LevelUpSkill_NeedMoreLevel")]);
		}
		break;
	default:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("LearnSkill_CanNotLearn")]);
		}
		break;
	}
	return 0;
}

DWORD SkillMgr::NetForgetSkill( tagNS_ForgetSkill* pNetCmd, DWORD dwParam )
{
	
	return 0;
}

DWORD SkillMgr::NetClearTalent( tagNS_ClearTalent* pNetCmd, DWORD dwParam )
{
	if (E_Success == pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("NS_RoleClearAttPoint_Success")]);
		tagGameEvent event(_T("SkillEvent_ClearTalent"), NULL);
		m_pFrameMgr->SendEvent(&event);
	}
	return 0;
}

DWORD SkillMgr::NetAddTalent( tagNS_AddTalent* pNetCmd, DWORD dwParam )
{
	tagRoleTalent talent;
	talent.eType = pNetCmd->eType;
	talent.nPoint = pNetCmd->nPoint;
	SetTalentPoint(talent);

	return 0;
}

DWORD SkillMgr::NetRemoveTalent( tagNS_RemoveTalent* pNetCmd, DWORD dwParam )
{
	tagRoleTalent talent;
	talent.eType = pNetCmd->eType;
	talent.nPoint = 0;
	SetTalentPoint(talent);

	return 0;
}

DWORD SkillMgr::NetUpdateTalent( tagNS_UpdateTalent* pNetCmd, DWORD dwParam )
{
	tagRoleTalent talent;
	talent.eType = pNetCmd->eType;
	talent.nPoint = pNetCmd->nPoint;
	SetTalentPoint(talent);

	return 0;
}


DWORD SkillMgr::NetUpdateSkillCD( tagNS_UpdateSkillCoolDown* pNetCmd, DWORD dwParam )
{
	tagSkillData* pSkill  = NULL;
	const tagSkillProtoClient *pProto = SkillProtoData::Inst()->FindSkillProto(MTransSkillTypeID(pNetCmd->dwSkillID,1));
	if(P_VALID(pProto) && ESSTE_SoulSubSkill == pProto->nType2)
		pSkill = m_skillMap.Peek(25052);
	else
		pSkill = m_skillMap.Peek(pNetCmd->dwSkillID);
	if(P_VALID(pSkill))
	{
		pSkill->nCurCD = pNetCmd->nCoolDown;
		INT nIncMilliSecond = pSkill->nCurCD - pSkill->nCooldown;
		pSkill->dwStartTime = Kernel::Inst()->GetAccumTimeDW() +  nIncMilliSecond;
		
		// 发送游戏事件
		tagUpdateLPSkillEvent event(_T("tagUpdateLPSkillEvent"),NULL);
		event.bAdd  = FALSE;
		event.dwID  = pSkill->dwSkillID;
		event.eType = pSkill->pProto->eUseType;
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}

	return 0;
}

DWORD SkillMgr::NetUpdateSkillCDEx( tagNS_UpdateSkillCoolDownEx* pNetCmd, DWORD dwParam )
{
	for(int i=0; i< pNetCmd->nCount; i++)
	{
		tagSkillData* pSkill = m_skillMap.Peek(pNetCmd->dwSkillID[i]);
		if(P_VALID(pSkill))
		{
			pSkill->nCurCD = pNetCmd->nCoolDown;
			INT nIncMilliSecond = pSkill->nCurCD - pSkill->nCooldown;
			pSkill->dwStartTime = Kernel::Inst()->GetAccumTimeDW() +  nIncMilliSecond;

			// 发送游戏事件
			tagUpdateLPSkillEvent event(_T("tagUpdateLPSkillEvent"),NULL);
			event.bAdd  = FALSE;
			event.dwID  = pSkill->dwSkillID;
			event.eType = pSkill->pProto->eUseType;
			TObjRef<GameFrameMgr>()->SendEvent(&event);
		}
	}

	return 0;
}

DWORD SkillMgr::OnNS_RoleGetTalentPoint(tagNS_RoleGetTalentPoint* pNetCmd, DWORD dwParam)
{
	switch( pNetCmd->dwErrorCode )
	{
	case ERTEC_OverMaxTalentAvail_Limit:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("TalentArriveToLimit")] );
			break;
		}
		break;
	default:
		break;
	}
	
	return 0;
}


void SkillMgr::ShowNetError( INT nErrorCode )
{
	if(GT_INVALID != nErrorCode && E_Success != nErrorCode)
	{

	}
}

BOOL SkillMgr::IsInsideOfMaxTalent( ETalentType eTalent )
{
    // 新手技能不限制
    if (ETT_FreshHand == eTalent)
        return TRUE;
    
    // 已获得的天资，不受限制
    if (IsTalentAquired(eTalent))
        return TRUE;

    // 如果拥有MAX_BATTLE_TALENT_COUNT个战斗系技能，则关掉其它战斗系天资
    if (GetBattleTalentCount() < MAX_BATTLE_TALENT_COUNT)
        return TRUE;

	return FALSE;
}

INT SkillMgr::GetTalentPoint( ETalentType eTalent )
{
	for(int i=0; i<X_MAX_TALENT_PER_ROLE; ++i)
	{
		if(eTalent == m_talentPoint[i].eType)
			return m_talentPoint[i].nPoint;
	}
	return 0;
}

void SkillMgr::SetTalentPoint( tagRoleTalent Talent )
{
	INT nMaxTalent = 0;
	BOOL bSave = FALSE;
	for (INT i=0; i<X_MAX_TALENT_PER_ROLE; ++i)
	{
		if(!bSave)
		{
			// 如果未存，则找相同的或者空的存
			if(m_talentPoint[i].eType == Talent.eType || 
				m_talentPoint[i].eType == ETT_Null)
			{
				if(0 != Talent.nPoint)
				{
					m_talentPoint[i].eType = Talent.eType;
					m_talentPoint[i].nPoint = Talent.nPoint;
				}
				else// 如果为零则为删除
				{
					m_talentPoint[i].eType = ETT_Null;
					m_talentPoint[i].nPoint = 0;

				}
				
				bSave = TRUE;
			}
		}
		else
		{
			// 如果已存还发现有相同的，则清除重复的内容
			if(m_talentPoint[i].eType == Talent.eType)
			{
				m_talentPoint[i].eType = ETT_Null;
				m_talentPoint[i].nPoint = 0;
			}
		}
		
		// 有不为空的数组则计数
		if(ETT_Null != m_talentPoint[i].eType)
		{
			++nMaxTalent;
		}
	}
	
	if(ETT_Null != Talent.eType)
	{
		// 发送游戏事件
		tagUpdateLPTalentPoint event(_T("tagUpdateLPTalentPoint"),NULL);
		event.eType = Talent.eType;
		event.nPoint = Talent.nPoint;
		event.nMaxTalent = nMaxTalent;
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}
}

VOID SkillMgr::OnUseSkill( DWORD dwTypeID )
{
	//如果是普通攻击，则要重新取一下TypeID
	if(IsNormalAttackSkill(MTransSkillID(dwTypeID)))
	{
		dwTypeID = GetNormalAttackTypeID();
	}

	tagUseSkillEvent event(_T("tagUseSkillEvent"),NULL);
	event.dwSkillID = dwTypeID;
	m_pFrameMgr->SendEvent(&event);
}

DWORD SkillMgr::GetNormalAttackTypeID()
{
	DWORD dwSkillID = GetNormalAttackID();
	tagSkillData* pSkill = m_skillMap.Peek(dwSkillID);
	if(P_VALID(pSkill))
	{
		return pSkill->dwTypeID;
	}
	return GT_INVALID;
}

BOOL SkillMgr::IsHaveSkillByTypeID( DWORD dwTypeID )
{
	tagSkillData* pSkill = m_skillMap.Peek(MTransSkillID(dwTypeID));
	//如果找到技能且模板ID相同
	if(P_VALID(pSkill) && MTransSkillLevel(dwTypeID) <= pSkill->nLevel)
		return TRUE;
	
	return FALSE;
}

BOOL SkillMgr::IsHaveSkillBySkillID(DWORD dwSkillID)
{
	tagSkillData* pSkill = m_skillMap.Peek(dwSkillID);
	//如果找到技能且模板ID相同
	if(P_VALID(pSkill))
		return TRUE;

	return FALSE;
}

BOOL SkillMgr::IsHaveProduceSkillByTypeID(DWORD dwTypeID)
{
	tagSkillData* pSkill = m_skillMap.Peek(MTransSkillID(dwTypeID));
	if( P_VALID(pSkill) )
		return TRUE;

	return FALSE;
}

BOOL SkillMgr::IsSkillCDByTypeID( DWORD dwTypeID )
{
	FLOAT fCDCount = 0.0f;
	FLOAT fRestoreRatio = 0.0f;
	GetSkillCDTime(MTransSkillID(dwTypeID), fCDCount, fRestoreRatio);
	
	return fCDCount <=0;
}

VOID SkillMgr::GetSkillCDTime( DWORD dwID, FLOAT& fCDCount, FLOAT& fRestoreRatio )
{
	const tagSkillData* pSkill = GetSkillData(dwID);
	if(P_VALID(pSkill) && P_VALID(pSkill->dwStartTime))
	{
		DWORD dwStartTime = pSkill->dwStartTime;
		FLOAT fCDTime = (FLOAT)pSkill->nCooldown;
		//离上一次该类物品使用的时间
		INT nPassMilliSecond = Kernel::Inst()->GetAccumTimeDW() - pSkill->dwStartTime;
		//倒计时时间
		fCDCount = fCDTime - nPassMilliSecond;	
		//倒计时旋转律
		fRestoreRatio = (fCDTime - fCDCount) / fCDTime + 0.000001f;
	}
}

tstring SkillMgr::GetSkillIconPath( DWORD dwTypeID )
{
	//如果是普通攻击需要查看
	if(IsNormalAttackSkill(MTransSkillID(dwTypeID)))
	{
		//主手武器存在
		Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(EEP_RightHand);
		if(P_VALID(pEquip))
		{
			return ItemMgr::Inst()->GetItemIconPath(pEquip->GetItemTypeID());
		}
	}
	TCHAR szPath[X_LONG_NAME] = {0};
	const tagSkillProtoClient* pProto = SkillProtoData::Inst()->FindSkillProto(dwTypeID);
	if (P_VALID(pProto))
	{
		_sntprintf(szPath, sizeof(szPath)/sizeof(TCHAR), _T("data\\ui\\Icon\\%s.tga"), pProto->szIcon);
	}

	return szPath;
}


DWORD SkillMgr::GetGatherSkillTypeID( INT nType )
{
	switch (nType)
	{
	case EGT_Mine:
	case EGT_Herb:
		{
			tagSkillData* pSkill = m_skillMap.Peek(GATHER_SKILL_MINING);
			if (P_VALID(pSkill) && P_VALID(pSkill->pProto))
			{
				return pSkill->dwTypeID;
			}
		}
		break;
	case EGT_Food:
	case EGT_Cloth:
		{
			tagSkillData* pSkill = m_skillMap.Peek(GATHER_SKILL_HARVEST);
			if (P_VALID(pSkill) && P_VALID(pSkill->pProto))
			{
				return pSkill->dwTypeID;
			}
		}
		break;
	}
	return GT_INVALID;
}

DWORD SkillMgr::IsSkillCanLearn( DWORD dwTypeID )
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return GT_INVALID;

	// 判断技能是否存在
	if(IsHaveSkillByTypeID(dwTypeID))
	{
		return E_LearnSkill_Existed;
	}
	// 获取技能原型
	const tagSkillProtoClient* pProto = SkillProtoData::Inst()->FindSkillProto(dwTypeID);
	if(!P_VALID(pProto))
	{
		return E_LearnSkill_ProtoNotFound;
	}

	// 职业限制是否达成
	

	// 等级限制是否达成
	if(pProto->nNeedRoleLevel > 1 && pProto->nNeedRoleLevel > pLp->GetRoleLevel())
	{
		return E_LearnSkill_NeedMoreLevel;
	}

	// 天资限制是否达成
	if(pProto->nNeedTalentPoint > SkillMgr::Inst()->GetTalentPoint(pProto->eTalentType))
	{
		return E_LearnSkill_NeedMoreTalentPoint;
	}

	// 技能前置是否达成
	if(GT_INVALID != pProto->dwPreLevelSkillID)
	{
		if(pProto->dwPreLevelSkillID < 10000) // 前置盒子
		{
			const tagSkillBoxProto* pSkillBox = SkillProtoData::Inst()->FindSkillBoxProto(pProto->dwPreLevelSkillID);
			if(!P_VALID(pSkillBox)) return E_LearnSkill_NoPreSkill;
			for(int i=0; i< tagSkillBoxProto::E_SBP_Size; i++)
			{
				if(P_VALID(pSkillBox->dwSkillIDs[i]) && !IsHaveSkillByTypeID(pSkillBox->dwSkillIDs[i]))
				{
					return E_LearnSkill_NoPreSkill;
				}
			}
		}
		else if(!IsHaveSkillByTypeID(pProto->dwPreLevelSkillID))
		{
			return E_LearnSkill_NoPreSkill;
		}
	}

	return E_Success;
}

int SkillMgr::GetBattleTalentCount()
{
    int nCount = 0;

    for (int i = 0; i < X_MAX_TALENT_PER_ROLE; ++i)
    {
        if (m_talentPoint[i].eType > ETT_Null
            && m_talentPoint[i].eType < ETT_End
            && m_talentPoint[i].eType != ETT_FreshHand)
            ++nCount;
    }

    return nCount;
}

BOOL SkillMgr::IsTalentAquired( ETalentType eType )
{
    for (int i = 0; i < X_MAX_TALENT_PER_ROLE; ++i)
    {
        if (eType == m_talentPoint[i].eType)
            return TRUE;
    }

    return FALSE;
}

INT SkillMgr::GetTotalBidden()
{
	INT nTalentCount = 0;
	for( int i = 0; i < ETT_End; i++ )
	{
		nTalentCount += GetTalentPoint( (ETalentType)i );
	}

	return nTalentCount;
}

ETalentType SkillMgr::GetMainTalentType()
{
	EClassType classType = RoleMgr::Inst()->GetLocalPlayer()->GetPlayerClass();
	switch(classType)
	{
	case EV_Blade:		// 刀客
	case EV_Fuhu:
	case EV_XuanJia:
		return ETT_Blade; // 神兵
		break;
	case EV_Bow:			// 弓羽
	case EV_HuanShi:
	case EV_ZhanXin:
		return ETT_Bow; // 玄翔
		break;
	case EV_Strength:	// 玄师
	case EV_LeiMang:
	case EV_GuaXiang:
		return ETT_Valor; // 乾坤
		break;
	case EV_Musician:	// 乐师 
	case EV_ShenShi:
	case EV_TianYi:
		return ETT_Swing; // 仙音
		break;
	}
	return ETT_Null;
}

BOOL SkillMgr::IsSkillLearned( DWORD dwSkillTypeID )
{
	tagSkillData* pSkill = m_skillMap.Peek( dwSkillTypeID );
	return P_VALID(pSkill);
}

INT SkillMgr::GetExtraTalentPointNum()
{
	INT nAvailPoint = RoleMgr::Inst()->GetLocalPlayer()->GetAttribute( ERA_TalentPoint );
	INT nBiddenPoint = SkillMgr::Inst()->GetTotalBidden();
	INT nCanBuy =  GetTalentLimit(RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel()) - (nAvailPoint + nBiddenPoint);

	if( nCanBuy < 0 )
		nCanBuy = 0;

	return nCanBuy;
}

INT SkillMgr::GetTalentLimit( DWORD dwLevel )
{
	const tagExperience *pExp = LevelUpData::Inst()->FindExpData( dwLevel );

	if( P_VALID(pExp) )
		return pExp->nTalentAvail_limit;
	else 
		return 0;
}

DWORD SkillMgr::OnNS_Skill( tagNS_Skill* pMsg, DWORD dwParam )
{
	if( pMsg->dwSkillID != FairyTrain_SKILL && 
		MTransSkillID(pMsg->dwSkillID) != FairyTrain_SKILL &&
		pMsg->dwSkillID != Call_Holy_Skill)
		return 0;

	if( pMsg->dwSkillID == Call_Holy_Skill )
	{
		if(RoleMgr::Inst()->GetLocalPlayer()->GetID() != pMsg->dwSrcRoleID)
			return 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( (P_VALID(pLocalPlayer) || (pLocalPlayer->GetRoleState() & ERS_CALLHOLY)) && !pLocalPlayer->IsInState(ES_Dead) )
		{
			tagNC_CallShengLing cmd;
			m_pSession->Send(&cmd);
		}
		else if(pLocalPlayer->IsInState(ES_Dead))
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CallShengLing_Dead")]);
		}
		else
		{
		 	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CallShengLing_HolyNotEnough")]);
		}

		return 0;
	}

	Role* pRole = RoleMgr::Inst()->FindRole(pMsg->dwTarRoleID, false, true);
	if( !P_VALID(pRole) )
		return 0;
	NPC* pNpc = (NPC*)pRole;
	if( pNpc->IsFairySoul() )
	{
		if(RoleMgr::Inst()->GetLocalPlayer()->GetID() != pMsg->dwSrcRoleID)
			return 0;
		if( pMsg->dwErrorCode != E_Success )
		{
			return 0;
		}

		FairyTrainFrame *pFrame = (FairyTrainFrame*)m_pFrameMgr->GetFrame(_T("FairyTrain"));
		if(!P_VALID(pFrame))
		{
			pFrame = (FairyTrainFrame*)m_pFrameMgr->CreateFrame( _T("World"), _T("FairyTrain"), _T("FairyTrainFrame"), 0 );
			if( P_VALID(pFrame) )
			{
				pFrame->SetNPCID(pMsg->dwTarRoleID);
				tagNC_OpenFairyTrain m;
				m.dwFairyID = pMsg->dwTarRoleID;
				m_pSession->Send(&m);
			}
		}
	}
	return 0;
}

DWORD SkillMgr::GetCondensedSkillID()
{
	tagSkillData* pData;
	m_skillMap.ResetIterator();
	while(m_skillMap.PeekNext(pData))
	{
		//凝聚技能
		if(ESSTE2_GodCondense == pData->pProto->nType3)
		{
			return pData->dwTypeID;
		}
	}
	return GT_INVALID;
}

void SkillMgr::Update()
{
	// 10秒更新一次技能列表中的时限技能
	if (timeGetTime() - m_dwLastUpdateTime > 10000)
	{
		m_dwLastUpdateTime = timeGetTime();
		tagSkillData* pData;
		m_skillMap.ResetIterator();
		while(m_skillMap.PeekNext(pData))
		{
			if (pData->dwLimitTime > 0)
			{
				pData->dwLimitTime = pData->dwLimitTime > 10 ? (pData->dwLimitTime - 10) : 0;
			}
		}
	}

}

VOID SkillMgr::SetSkillRenewTime(DWORD dwSkillID,DWORD dwTime)
{
	tagSkillData* pData =  m_skillMap.Peek(MTransSkillID(dwSkillID));
	if (P_VALID(pData))
		pData->dwLimitTime = dwTime;
}

VOID SkillMgr::SetLastUseSkillTime(DWORD dwTime)
{
	m_dwLastUseSkillTime = dwTime;
}
DWORD SkillMgr::GetLastUseSkillTime()
{
	return m_dwLastUseSkillTime;
}