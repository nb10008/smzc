#include "StdAfx.h"
#include "UpperStateSkill_LP.h"
#include "RoleMgr.h"
#include "FSM_LP.h"
#include "LocalPlayer.h"
#include "CombatEvent.h"
#include "..\WorldDefine\msg_combat.h"
#include "SkillMgr.h"
#include "SkillProtoData.h"
#include "CombatSysUtil.h"
#include "CombatActionChecker.h"
#include "SkillProEvent.h"
#include "PlayerActionData.h"
#include "RoleEvent.h"
#include "EffectMgr.h"

UpperStateSkill_LP::UpperStateSkill_LP(void)
{}

UpperStateSkill_LP::~UpperStateSkill_LP(void)
{}

void UpperStateSkill_LP::Active( tagGameEvent* pEvent,NavMap* pNav,LocalPlayer* pRole )
{
	ASSERT(pEvent->strEventName==_T("tagUseSkillEvent"));
	tagUseSkillEvent* pAttackEvent=(tagUseSkillEvent*)pEvent;

	m_startSpellTime = 0;
	m_dwChangeSkillDelay= 0;
	m_fCurAniPos = 0.0f;
	m_spellRealTime = GT_INVALID;
	m_step=EStep_None;
	m_eventCache.eventType=tagEventCache::EET_None;
	m_bWaitForWeaponSwap = false;
	m_bWaitForUnmount = false;
	m_bSkillInterruptable = false;

	m_targetID=pAttackEvent->dwTargetRoleID;
	m_skillID=pAttackEvent->dwSkillID;

	//--保存技能相关属性
	const tagSkillData* pSkillData=SkillMgr::Inst()->GetSkillDataByTypeID(m_skillID);
	ASSERT(P_VALID(pSkillData));
	if(!P_VALID(pSkillData)) 
	{
		m_pFSM->Change2UpperState_Idle();
		return;
	}


	// 骑乘状态 先下马
	if(pRole->GetMount() != NULL) 
	{
		TList<tagRoleBuff*>& buffList = RoleMgr::Inst()->GetLocalPlayer()->GetBuffList();
		tagRoleBuff* pRoleBuff = NULL;
		buffList.ResetIterator();
		while(buffList.PeekNext(pRoleBuff))
		{
			if (P_VALID(pRoleBuff)) 
			{
				if(pRoleBuff->dwBuffTypeID == MOUNT_BUFF_ID || pRoleBuff->dwBuffTypeID == MOUNT2_BUFF_ID)
				{
					tagNC_CancelBuff cmd;
					cmd.dwBuffTypeID = pRoleBuff->dwBuffTypeID;
					TObjRef<NetSession>()->Send( &cmd );

					m_eventCache.eventType=tagEventCache::EET_UseSkill;
					m_eventCache.useSkill=*pAttackEvent;
					m_bWaitForUnmount=true;
					return;
				}
			}
		}
		IMSG(_T("Unexcepted error.Unmount failed before using skill.\r\n "));
		ASSERT(0);
		// 服务器端下马， 可能导致动作播放不出来
		/*m_pFSM->Change2UpperState_Idle();
		return;*/
	}	

	const tagSkillProtoClient* pSkillProto=pSkillData->pProto;
	THROW_FALSE(P_VALID(pSkillProto),_T("cannot find skill proto"));
	m_bAttackSkill		= pSkillProto->bHostile==TRUE;
	m_bSpellSkill		= pSkillData->nPrepareTime>0;
	m_bMoveable			= pSkillProto->bMoveable==TRUE;
	m_bAniBlend			= pSkillProto->bActionBlend==TRUE;
	m_bMoveSpell		= pSkillProto->bInterruptMove==FALSE;
	m_bAutoNormalAttack = pSkillProto->bAutoNormalAttack==TRUE;
	m_dwSkillStartTime = 0;
	m_dwAttackEndTime = 0;
	m_dwAttackAdvanceTime = 0;
	m_dwAttackEndDelayTime = 0;
	if( m_bSpellSkill )
	{
		const tagPlayerActionData* pActData = PlayerActionData::Inst()->FindData( pSkillProto->szActionMod );
		if( NULL != pActData )
			m_dwAttackAdvanceTime = pActData->dwAttackTime;

		m_pFSM->GetLocalPlayer()->SetRePathFinding(false);
	}
	if( m_bAutoNormalAttack )
	{
		m_dwAttackEndDelayTime = m_bSpellSkill ? 500 : 100;
	}

	// 副手武器的技能，首先切换武器
	if( pSkillProto->nWeaponLimit != pRole->GetRWeaponType() && pSkillProto->nWeaponLimit == pRole->GetLWeaponType() && EITE_Null != pSkillProto->nWeaponLimit )
	{
		tagNC_SwapWeapon cmd;
		TObjRef<NetSession>()->Send( &cmd );
		m_eventCache.eventType=tagEventCache::EET_UseSkill;
		m_eventCache.useSkill=*pAttackEvent;
		m_bWaitForWeaponSwap=true;
		return;
	}

	m_dwSerial=CombatSysUtil::MakeSkillMsgSerial();

	//--发送进入战斗状态事件
	if( pSkillProto->bInterCombat )
	{
		tagEnterCombatEvent event(_T("tagEnterCombatEvent"),NULL);
		m_pFSM->OnGameEvent(&event);
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}

	//--以自已为中心的爆炸技能
	if(m_targetID==GT_INVALID)
	{
		if(m_bSpellSkill)
		{
			//停止移动
			if(!m_bMoveSpell)
				m_pFSM->Change2LowerState_Stand();

			m_actSpell.Active(pRole,NULL,m_skillID,m_dwSerial);
			m_step=EStep_Spell;
		}
		else
		{
			//停止移动
			if(!m_bMoveable)
				m_pFSM->Change2LowerState_Stand();

			m_actAttack.Active(pRole,NULL,m_skillID,m_dwSerial,true);
			m_step=EStep_Attack;

			m_dwSkillStartTime = Kernel::Inst()->GetAccumTimeDW();
		}
	}
	//--对自已使用
	else if(m_targetID==RoleMgr::Inst()->GetLocalPlayerID())
	{
		if(m_bSpellSkill)
		{
			//停止移动
			if(!m_bMoveSpell)
				m_pFSM->Change2LowerState_Stand();

			m_actSpell.Active(pRole,pRole,m_skillID,m_dwSerial);
			m_step=EStep_Spell;
		}
		else
		{
			//停止移动
			if(!m_bMoveable)
				m_pFSM->Change2LowerState_Stand();

			m_actAttack.Active(pRole,pRole,m_skillID,m_dwSerial,true);
			m_step=EStep_Attack;
		}
	}
	//--对别人使用
	else
	{
		//--检查目标是否存在
		Role* pTarget=RoleMgr::Inst()->FindRole(m_targetID);
		if(pTarget==NULL)
		{
			m_pFSM->Change2UpperState_Idle();
			return;
		}

		//--计算攻击距离
		LocalPlayer* pSelf=RoleMgr::Inst()->GetLocalPlayer();

		//--fOPDist是动态的，所以要从SkillMgr中取
		m_attackDist=pSkillData->fOPDist+(pSelf->GetRoleSize().z+pTarget->GetRoleSize().z)*0.5f;
		
		//--检查目标是否在攻击范围内
		if(IsInAttackRange(pSelf,pTarget))
		{
			if(m_bSpellSkill)
			{
				//停止移动
				if(!m_bMoveSpell)
					m_pFSM->Change2LowerState_Stand();

				m_actSpell.Active(pRole,pTarget,m_skillID,m_dwSerial);
				m_step=EStep_Spell;
			}
			else
			{
				//停止移动
				if(!m_bMoveable)
					m_pFSM->Change2LowerState_Stand();

				m_actAttack.Active(pRole,pTarget,m_skillID,m_dwSerial,true);
				m_step=EStep_Attack;
			}
		}
		else
		{
			m_step=EStep_Pursue;

			tagPursueEvent event;
			event.start=pSelf->GetPos();
			event.targetType=tagPursueEvent::ETargetType_Role;
			event.targetRoleID=m_targetID;
			event.validDist=m_attackDist;
			event.bNeedSendStopCmd=true;

			if(!m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_Pursue,&event))
			{
				CombatSysUtil::Inst()->ShowActionCheckMsg(CombatActionChecker::EEC_TooFar);//提示距离太远
				m_pFSM->Change2UpperState_Idle();
				return;
			}
		}
	}
}

void UpperStateSkill_LP::Update( NavMap* pNav,LocalPlayer* pRole )
{
	//--跟踪目标------------------------------------------------------
	if(m_step==EStep_Pursue)
	{
		//--检查目标是否存在
		Role* pTarget=RoleMgr::Inst()->FindRole(m_targetID);
		if(pTarget==NULL)
		{
			m_pFSM->Change2UpperState_Idle();
			SendStopPursueEvent();
			return;
		}
	}
	//--起手-----------------------------------------------------------
	else if(m_step==EStep_Spell)
	{
		//--检查目标是否存在
		Role* pTarget=NULL;
		if(m_targetID!=GT_INVALID)
		{
			pTarget=RoleMgr::Inst()->FindRole(m_targetID,false,true);
			if(pTarget==NULL)
			{
				m_pFSM->Change2UpperState_Idle();
				return;
			}
		}

		//--
		m_actSpell.Update(pRole,pTarget);
		if(m_actSpell.IsClosed())//吟唱时间结束
		{
			m_actAttack.Active(pRole,pTarget,m_skillID,m_dwSerial,false);
			m_step=EStep_Attack;
			return;
		}		
	}
	//--攻击-----------------------------------------------------------
	else if(m_step==EStep_Attack)
	{
		//--检查目标是否存在
		Role* pTarget=NULL;
		if(m_targetID!=GT_INVALID)
		{
			pTarget=RoleMgr::Inst()->FindRole(m_targetID,false,true);
			if(pTarget==NULL)
			{
				m_pFSM->Change2UpperState_Idle();
				return;
			}
		}

		//--
		m_actAttack.Update(pRole,pTarget);
		if(m_actAttack.IsClosed())
		{
			if( 0 == m_dwAttackEndTime )
			{
				m_dwAttackEndTime=Kernel::Inst()->GetAccumTimeDW();
				if( m_dwAttackEndDelayTime > 0 )
				{
					const TCHAR* szTrackName=CombatSysUtil::Inst()->GetReadyTrackName(pRole->GetRWeaponType());
					pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Ready,AvatarAniMixer::EAP_All,szTrackName,true);
					RoleMgr::Inst()->SynVRCombatAction(pRole->GetID(), szTrackName );
				}
			}
			else if( Kernel::Inst()->GetAccumTimeDW() - m_dwAttackEndTime > m_dwAttackEndDelayTime )
			{
				//--检查是否有缓存的游戏事件
				if(m_eventCache.eventType!=tagEventCache::EET_None)
				{
					if(m_eventCache.eventType==tagEventCache::EET_UseSkill)
					{
						if(!m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,&m_eventCache.useSkill))
							m_pFSM->Change2UpperState_Idle();
					}
					else if(m_eventCache.eventType==tagEventCache::EET_UseItem)
					{
						if(!m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_UseItem,&m_eventCache.useItem))
							m_pFSM->Change2UpperState_Idle();
					}
					else if(m_eventCache.eventType==tagEventCache::EET_MouseMove)
					{
						m_pFSM->Change2UpperState_Idle();
						m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_MouseMove,&m_eventCache.mouseMove);
					}
					else if(m_eventCache.eventType==tagEventCache::EET_KeyboardMove)
					{
						m_pFSM->Change2UpperState_Idle();
						m_pFSM->TryChange2LowerState(FSM_LP::ELowerState_KeyboardMove,&m_eventCache.keyboardMove);
					}
					else
						m_pFSM->Change2UpperState_Idle();
				}
				//--自动接普通攻击
				else if( m_bAutoNormalAttack 
					&& pTarget!=NULL 
					&& !pTarget->IsInState(ES_Dead) 
					&& !(m_pFSM->IsMoveState() && !CombatSysUtil::Inst()->NormalAttackCanMove()) )
				{
					tagUseSkillEvent event(_T("tagUseSkillEvent"),NULL);
					event.dwSkillID=SkillMgr::Inst()->GetNormalAttackTypeID();
					event.dwTargetRoleID=m_targetID;
					if(!m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,&event))
						m_pFSM->Change2UpperState_Idle();
				}
				//--切换到空闲状态
				else
					m_pFSM->Change2UpperState_Idle();
			}
		}
	}
	//--切招-----------------------------------------------------------
	else if(m_step==EStep_Change)
	{
		if (timeGetTime() - m_dwChangeSkillDelay > 100)
		{
			if (!m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,&m_eventCache.useSkill))
			{
				m_pFSM->Change2UpperState_Idle();
			}

		}
		else
		{
			const tagSkillProtoClient* pSkillProto=SkillProtoData::Inst()->FindSkillProto(m_skillID);
			if (P_VALID(pSkillProto) || m_fCurAniPos > 0.0f)
			{
				pRole->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Upper,pSkillProto->szActionMod,false,true,m_fCurAniPos);
			}
		}
	}
	if( m_step==EStep_Attack || m_step==EStep_Spell)
	{
		if( GT_INVALID!=m_spellRealTime &&
			Kernel::Inst()->GetAccumTimeDW()-m_startSpellTime>=m_spellRealTime)
		{
			//--发送游戏事件，隐藏吟唱进度条
			tagSkillProgressEvent event(_T("tagSkillProgressEvent"),NULL);
			event.bShow=false;
			event.bSkill=true;
			event.dwTypeID=m_skillID;
			event.dwTime=0;
			TObjRef<GameFrameMgr>()->SendEvent(&event);

			m_startSpellTime = 0;
			m_spellRealTime = GT_INVALID;
		}
	}
}

void UpperStateSkill_LP::OnGameEvent( tagGameEvent* pEvent )
{
	//--等待切换武器
	if(m_bWaitForWeaponSwap)
	{
		if(pEvent->strEventName==_T("tagWeaponChangeEvent"))
		{
			tagWeaponChangeEvent* pWeaponChangeEvent = (tagWeaponChangeEvent*)pEvent;
			if( pWeaponChangeEvent->bRightWeapon )
			{
				if( !m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,&m_eventCache.useSkill) )
					m_pFSM->Change2UpperState_Idle();
			}
		}
	}
	// 下马
	else if(m_bWaitForUnmount)
	{
		if(pEvent->strEventName==_T("tagLPRoleStateChangeEvent"))
		{
			if( !m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,&m_eventCache.useSkill) )
				m_pFSM->Change2UpperState_Idle();
		}
	}
	//--跟踪目标------------------------------------------------------
	else if(m_step==EStep_Pursue)
	{
		if(pEvent->strEventName==_T("tagUseSkillEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,pEvent);
		}
		else if(pEvent->strEventName==_T("tagUseItemEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_UseItem,pEvent);
		}
		else if(pEvent->strEventName==_T("tagFollowRoleEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Follow,pEvent);
		}
		else if(pEvent->strEventName==_T("tagTalkNPCEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_TalkNPC,pEvent);
		}
		else if(pEvent->strEventName==_T("tagPickupItemEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_PickupItem,pEvent);
		}
		else if(pEvent->strEventName==_T("tagPlayPersonalityActEvent"))
		{
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_StyleAction,pEvent);
		}
		else if(pEvent->strEventName==_T("tagPursueFinishEvent"))
		{
			tagPursueFinishEvent* pFinishEvent=(tagPursueFinishEvent*)pEvent;
			if(pFinishEvent->bSuccessed)
			{
				//--检查目标是否存在
				Role* pTarget=RoleMgr::Inst()->FindRole(m_targetID);
				if(pTarget==NULL)
				{
					m_pFSM->Change2UpperState_Idle();
					return;
				}

				if(m_bSpellSkill)
				{
					m_actSpell.Active(m_pFSM->GetLocalPlayer(),pTarget,m_skillID,m_dwSerial);
					m_step=EStep_Spell;
				}
				else
				{
					m_actAttack.Active(m_pFSM->GetLocalPlayer(),pTarget,m_skillID,m_dwSerial,true);
					m_step=EStep_Attack;
				}
			}
			else
			{
				m_pFSM->Change2UpperState_Idle();
			}
		}
	}
	//--起手-----------------------------------------------------------
	else if(m_step==EStep_Spell)
	{
/*		if(pEvent->strEventName==_T("tagUseSkillEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,pEvent);
		}
		else if(pEvent->strEventName==_T("tagUseItemEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_UseItem,pEvent);
		}
		else */if(pEvent->strEventName==_T("tagFollowRoleEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Follow,pEvent);
		}
		else if(pEvent->strEventName==_T("tagTalkNPCEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_TalkNPC,pEvent);
		}
		else if(pEvent->strEventName==_T("tagPickupItemEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_PickupItem,pEvent);
		}
		else if(pEvent->strEventName==_T("tagPlayPersonalityActEvent"))
		{
			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_StyleAction,pEvent);
		}
		else if(pEvent->strEventName==_T("tagESCCancelEvent"))
		{
			tagESCCancelEvent* pCancelEvent=(tagESCCancelEvent*)pEvent;

			m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
			m_pFSM->Change2UpperState_Idle();

			pCancelEvent->bCanceled=TRUE;
		}
	}
	//--攻击-----------------------------------------------------------
	else if(m_step==EStep_Attack)
	{
		if(pEvent->strEventName==_T("tagUseSkillEvent"))
		{
			tagUseSkillEvent* pSkillEvent=(tagUseSkillEvent*)pEvent;

			if(m_skillID==SkillMgr::Inst()->GetNormalAttackTypeID())//当前使用普通攻击
			{
				if(pSkillEvent->dwSkillID==SkillMgr::Inst()->GetNormalAttackTypeID())//普通攻击
				{
					if(pSkillEvent->dwTargetRoleID==m_targetID)//同一目标
					{
						//不做任何处理
					}
					else//不同目标
					{
						//直接切
						m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,pEvent);
					}
				}
				else//技能
				{
					//直接切
					m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,pEvent);
				}
			}
			else//当前使用技能
			{
				//连续按两次打断
				if(m_eventCache.eventType == tagEventCache::EET_UseSkill && m_eventCache.useSkill.dwSkillID == pSkillEvent->dwSkillID && m_eventCache.useSkill.dwTargetRoleID == pSkillEvent->dwTargetRoleID && m_bSkillInterruptable)
				{
					if (m_skillID != pSkillEvent->dwSkillID)// 不同技能才能打断
					{
						if (SkillMgr::Inst()->IsSkillCDByTypeID(pSkillEvent->dwSkillID))// 冷却的才能去打断
						{
							m_actAttack.Stop(m_pFSM->GetLocalPlayer(),TRUE);

							m_dwChangeSkillDelay = timeGetTime();
							m_step = EStep_Change;
							LocalPlayer* pLp = m_pFSM->GetLocalPlayer();
							if (P_VALID(pLp))
							{
								const tagSkillProtoClient* pSkillProto=SkillProtoData::Inst()->FindSkillProto(m_skillID);
								THROW_FALSE(P_VALID(pSkillProto),_T("cannot find skillproto"));
								m_fCurAniPos = pLp->GetAniCtrl()->GetTrackPosition(pSkillProto->szActionMod);
								EffectMgr::Inst()->PlayRoleEffect(pLp->GetID(),_T("txfzt28"),_T("Bip01 Spine2"));
							}
						}
					}
				}
				else
				{
					//缓存起来
					m_eventCache.eventType=tagEventCache::EET_UseSkill;
					m_eventCache.useSkill=*pSkillEvent;
				}

			}
		}
		else if(pEvent->strEventName==_T("tagUseItemEvent"))
		{
			tagUseItemEvent* pItemEvent=(tagUseItemEvent*)pEvent;

			if(m_skillID==SkillMgr::Inst()->GetNormalAttackTypeID())//当前使用普通攻击
			{
				//直接切
				m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_UseItem,pEvent);
			}
			else
			{
				//缓存起来
				m_eventCache.eventType=tagEventCache::EET_UseItem;
				m_eventCache.useItem=*pItemEvent;
			}
		}
		else if(pEvent->strEventName==_T("tagMouseMoveEvent"))
		{
			tagMouseMoveEvent* pMoveEvent=(tagMouseMoveEvent*)pEvent;

			if(!m_bMoveable
				&&m_skillID!=SkillMgr::Inst()->GetNormalAttackTypeID())
			{
				//缓存起来
				m_eventCache.eventType=tagEventCache::EET_MouseMove;
				m_eventCache.mouseMove=*pMoveEvent;
			}
		}
		else if(pEvent->strEventName==_T("tagKeyboardMoveEvent"))
		{
			tagKeyboardMoveEvent* pMoveEvent=(tagKeyboardMoveEvent*)pEvent;

			if(!m_bMoveable
				&&m_skillID!=SkillMgr::Inst()->GetNormalAttackTypeID())
			{
				//缓存起来
				m_eventCache.eventType=tagEventCache::EET_KeyboardMove;
				m_eventCache.keyboardMove=*pMoveEvent;
			}
		}
		else if(pEvent->strEventName==_T("tagStopKeyboardMoveEvent"))
		{
			if(m_eventCache.eventType==tagEventCache::EET_KeyboardMove)
				m_eventCache.eventType=tagEventCache::EET_None;
		}
		else if(pEvent->strEventName==_T("tagESCCancelEvent"))
		{
			tagESCCancelEvent* pCancelEvent=(tagESCCancelEvent*)pEvent;

			if(m_skillID==SkillMgr::Inst()->GetNormalAttackTypeID())//当前使用普通攻击
			{
				m_actAttack.Stop(m_pFSM->GetLocalPlayer(),true);
				m_pFSM->Change2UpperState_Idle();

				pCancelEvent->bCanceled=TRUE;
			}
		}
		else if(pEvent->strEventName==_T("tagSkillChannelEvent"))
		{
			tagSkillChannelEvent* pChannel = (tagSkillChannelEvent*) pEvent;
			const tagSkillProtoClient* pProto = SkillProtoData::Inst()->FindSkillProto(m_skillID);
			if (P_VALID(pProto))
			{
				if (pChannel->dwMisc == m_skillID && pChannel->dwMisc2 >= pProto->dwCutChannel && m_dwSerial == pChannel->dwSerial )
				{
					// 所有条件满足开启打断模式
					m_bSkillInterruptable = true;
				}
			}
		}
	}
}

void UpperStateSkill_LP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==TObjRef<Util>()->Crc32("NS_Skill"))
	{
		tagNS_Skill* pSkillCmd=(tagNS_Skill*)pNetCmd;
		if(pSkillCmd->dwSerial==m_dwSerial)
		{
			//IMSG(_T("[%d]recv ns_skill,skill=%d,errcode=%d\n"),timeGetTime(),pSkillCmd->dwSkillID,pSkillCmd->dwErrorCode);
			if(pSkillCmd->dwErrorCode==E_Success)
			{
				if (	pSkillCmd->dwSkillID == 1000101 || pSkillCmd->dwSkillID == 1000201 
					||	pSkillCmd->dwSkillID == 1000301 || pSkillCmd->dwSkillID == 1000401 
					||	pSkillCmd->dwSkillID == 1000501)
				{
					// 普通攻击不进行外挂检测
				}
				else
				{
					tagGameEvent event(_T("tagIllegalSkill"), NULL);
					TObjRef<GameFrameMgr>()->SendEvent(&event);
				}
				
				if(m_step==EStep_Spell)
				{
					// 缩短吟唱动作播放时间，预播攻击动作
					DWORD dwSpellActionTime = 0;
					if( pSkillCmd->nSpellTime > 0 && (DWORD)pSkillCmd->nSpellTime > m_dwAttackAdvanceTime )
					    dwSpellActionTime = pSkillCmd->nSpellTime - m_dwAttackAdvanceTime;
					m_actSpell.OnRecvSpellTime(dwSpellActionTime);

					if( pSkillCmd->nSpellTime > 0 )
					{
						m_spellRealTime= pSkillCmd->nSpellTime;
						m_startSpellTime=Kernel::Inst()->GetAccumTimeDW();

						//--发送游戏事件，显示吟唱进度条
						tagSkillProgressEvent event(_T("tagSkillProgressEvent"),NULL);
						event.bShow=true;
						event.bSkill=true;
						event.dwTypeID=m_skillID;
						event.dwTime=m_spellRealTime;
						TObjRef<GameFrameMgr>()->SendEvent(&event);
					}
				}
			}
			else//使用技能失败
			{
				IMSG(_T("使用技能时服务器返回错误代码：%d"),pSkillCmd->dwErrorCode);
				m_pFSM->Change2UpperState_Idle();
			}
		}
	}
	else if(pNetCmd->dwID==pNetCmd->Crc32("NS_SkillInterrupt"))
	{
		tagNS_SkillInterrupt* pCmd = (tagNS_SkillInterrupt*)pNetCmd;
		if( pCmd->dwSkillID == m_skillID && (pCmd->dwSerial == 0 || pCmd->dwSerial == GT_INVALID|| pCmd->dwSerial >= m_dwSerial))
		{
			m_pFSM->Change2UpperState_Idle();
			EffectMgr::Inst()->StopBeAttackEffect(GT_INVALID, m_pFSM->GetLocalPlayer()->GetID(), EHTC_Skill, m_skillID, m_dwSerial);
		}
	}
}

bool UpperStateSkill_LP::IsInAttackRange(LocalPlayer* pRole,Role* pTarget)
{
	return Vec3DistSq(pRole->GetPos(),pTarget->GetPos()) < m_attackDist*m_attackDist;
}

void UpperStateSkill_LP::SendStopPursueEvent()
{
	tagStopPursueEvent event;
	m_pFSM->OnGameEvent(&event);
}

void UpperStateSkill_LP::OnExit()
{
	if(m_step==EStep_Attack)
		m_actAttack.Stop(m_pFSM->GetLocalPlayer(),false);
	else if(m_step==EStep_Spell)
		m_actSpell.Stop(m_pFSM->GetLocalPlayer(),false);

	if( GT_INVALID!=m_spellRealTime )
	{
		//--发送游戏事件，隐藏吟唱进度条
		tagSkillProgressEvent event(_T("tagSkillProgressEvent"),NULL);
		event.bShow=false;
		event.bSkill=true;
		event.dwTypeID=m_skillID;
		event.dwTime=0;
		TObjRef<GameFrameMgr>()->SendEvent(&event);

		m_startSpellTime = 0;
		m_spellRealTime = GT_INVALID;
	}
}

bool UpperStateSkill_LP::IfCanChange2UpperState(FSM_LP::EUpperState state,tagGameEvent* pEvent)
{
	return true;
}

bool UpperStateSkill_LP::IfCanChange2LowerState(FSM_LP::ELowerState state,tagGameEvent* pEvent)
{
	//--追踪---------------------------------------
	if(m_step==EStep_Pursue||m_bWaitForWeaponSwap)
	{
		return true;
	}
	//--起手---------------------------------------
	else if(m_step==EStep_Spell)
	{
		return true;
	}
	//--攻击---------------------------------------
	else if(m_step==EStep_Attack)
	{
		if(m_bMoveable)//可以移动攻击
		{
			switch(state)
			{
			case FSM_LP::ELowerState_KeyboardMove:
			case FSM_LP::ELowerState_MouseMove:
			case FSM_LP::ELowerState_Stand:
				return true;
			}
		}
		if(m_skillID==SkillMgr::Inst()->GetNormalAttackTypeID())//普通攻击可以被移动打断
			return true;
	}

	return false;
}

void UpperStateSkill_LP::OnChangeLowerState(FSM_LP::ELowerState newState,tagGameEvent* pEvent)
{
	//--追踪---------------------------------------
	if(m_step==EStep_Pursue||m_bWaitForWeaponSwap)
	{
		switch(newState)
		{
		case FSM_LP::ELowerState_Pursue:
		case FSM_LP::ELowerState_Jump:
		case FSM_LP::ELowerState_Slide:
		case FSM_LP::ELowerState_VDrop:
		case FSM_LP::ELowerState_Drop:
		case FSM_LP::ELowerState_Stand:
			break;
		case FSM_LP::ELowerState_KeyboardMove:
			{
				if( !m_bWaitForWeaponSwap )
					m_pFSM->Change2UpperState_Idle();
			}
			break;
		default:
			{
				m_pFSM->Change2UpperState_Idle();
			}
			break;
		}
	}
	//--起手---------------------------------------
	else if(m_step==EStep_Spell)
	{
		if(m_bMoveSpell)//可以移动起手
		{
			switch(newState)
			{
			case FSM_LP::ELowerState_KeyboardMove:
			case FSM_LP::ELowerState_MouseMove:
			case FSM_LP::ELowerState_Stand:
				break;
			default:
				{
					m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
					m_pFSM->Change2UpperState_Idle();
				}
				break;
			}
		}
		else//不可以移动起手
		{
			switch(newState)
			{
			case FSM_LP::ELowerState_Stand:
				break;
			default:
				{
					m_actSpell.Stop(m_pFSM->GetLocalPlayer(),true);
					m_pFSM->Change2UpperState_Idle();
				}
				break;
			}
		}
	}
	//--攻击---------------------------------------
	else if(m_step==EStep_Attack)
	{
		if(m_bMoveable)//可以移动攻击
		{
			switch(newState)
			{
			case FSM_LP::ELowerState_KeyboardMove:
			case FSM_LP::ELowerState_MouseMove:
			case FSM_LP::ELowerState_Stand:
				break;
			default:
				{
					m_actAttack.Stop(m_pFSM->GetLocalPlayer(),false);
					m_pFSM->Change2UpperState_Idle();
				}
				break;
			}
		}
		else//不可以移动攻击
		{
			//普通攻击可能被鼠标移动和键盘移动打断
			switch(newState)
			{
			case FSM_LP::ELowerState_KeyboardMove:
			case FSM_LP::ELowerState_MouseMove:
				{
					m_actAttack.Stop(m_pFSM->GetLocalPlayer(),false);
					m_pFSM->Change2UpperState_Idle();
				}
				break;
			}
		}
	}

}

bool UpperStateSkill_LP::IfCanPlayLowerAni()
{
	if(m_step==EStep_None)
	{
		return true;
	}
	//--追踪---------------------------------------
	else if(m_step==EStep_Pursue)
	{
		return true;
	}
	//--起手---------------------------------------
	else if(m_step==EStep_Spell)
	{
		return true;
	}
	//--攻击---------------------------------------
	else if(m_step==EStep_Attack)
	{
		if(m_bMoveable)//可以移动攻击
		{
			if(m_bAniBlend)
				return true;
		}
		else//不可以移动攻击
		{}
	}

	return false;
}

bool UpperStateSkill_LP::IfLowerCanSetYaw()
{
	if(m_step==EStep_None)
	{
		return true;
	}
	//--追踪---------------------------------------
	else if(m_step==EStep_Pursue)
	{
		return true;
	}
	//--起手---------------------------------------
	else if(m_step==EStep_Spell)
	{
		return true;
	}
	//--攻击---------------------------------------
	else if(m_step==EStep_Attack)
	{
		if(m_bMoveable)//可以移动攻击
		{
			if(m_bAniBlend)
				return true;
		}
		else//不可以移动攻击
		{}
	}

	return false;
}

bool UpperStateSkill_LP::IsNormalSkill()
{
	if(m_skillID==SkillMgr::Inst()->GetNormalAttackTypeID())//普通攻击可以被移动打断
		return true;

	return false;
}