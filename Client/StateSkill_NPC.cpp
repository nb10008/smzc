#include "StdAfx.h"
#include "StateSkill_NPC.h"
#include "RoleMgr.h"
#include "FSM_NPC.h"
#include "NPC.h"
#include "..\WorldDefine\msg_combat.h"
#include "SkillProtoData.h"
#include "LocalPlayer.h"

StateSkill_NPC::StateSkill_NPC(void)
{}

StateSkill_NPC::~StateSkill_NPC(void)
{}

void StateSkill_NPC::Active( tagNetCmd* pNetCmd,NavMap* pNav,NPC* pRole )
{
	ASSERT(pNetCmd->dwID==pNetCmd->Crc32("NS_Skill"));
	tagNS_Skill* pSkillCmd=(tagNS_Skill*)pNetCmd;

	m_targetID=pSkillCmd->dwTarRoleID;
	m_skillID=pSkillCmd->dwSkillID;
	m_dwSerial=pSkillCmd->dwSerial;

	//--检查目标是否存在
	Role* pTarget=NULL;
	if(m_targetID!=GT_INVALID)
	{
		pTarget=RoleMgr::Inst()->FindRole(m_targetID);
		if(pTarget==NULL)
		{
			m_pFSM->Change2IdleState();
			return;
		}
	}

	//--
	if(pSkillCmd->nSpellTime>0)
	{
		m_actSpell.Active(pRole,pTarget,m_skillID,pSkillCmd->nSpellTime);
		m_step=EStep_Spell;
	}
	else
	{
		m_actAttack.Active(pRole,pTarget,pSkillCmd->dwSkillID,pSkillCmd->dwSerial);
		m_step=EStep_Attack;
	}
	
	if (P_VALID(pRole))
	{
		if(m_targetID == pRole->GetSkillToID())		return;	// 说明目标没有变
		
		pRole->SetSkillToID(m_targetID);	// 记录使用技能的目标，即该NPC的目标

		//--目标的目标（针对选中目标后，目标的目标变化的情况） 拗口吧，晕死你没商量
// 		LocalPlayer *plp = RoleMgr::Inst()->GetLocalPlayer();
// 		if (P_VALID(plp))
// 		{
// 			tagNC_TargetOfTarget cmd;
// 			cmd.dwPlayerID = plp->GetID();
// 			cmd.dwTargetID = pRole->GetID();
// 			TObjRef<NetSession>()->Send(&cmd);
// 		}
	}
	
}

void StateSkill_NPC::Update( NavMap* pNav,NPC* pRole )
{
	//--检查目标是否存在
	Role* pTarget=NULL;
	if(m_targetID!=GT_INVALID)
	{
		pTarget=RoleMgr::Inst()->FindRole(m_targetID);
		if(pTarget==NULL)
		{
			m_pFSM->Change2IdleState();
			return;
		}
	}

	if(m_step==EStep_Spell)
	{
		m_actSpell.Update(pRole,pTarget);
		if(m_actSpell.IsClosed())
		{
			m_actAttack.Active(pRole,pTarget,m_skillID,m_dwSerial);
			m_step=EStep_Attack;
		}
	}
	else if(m_step==EStep_Attack)
	{
		m_actAttack.Update(pRole,pTarget);
		if(m_actAttack.IsClosed())
		{
			m_pFSM->Change2IdleState();
		}
	}
}

void StateSkill_NPC::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_SkillInterrupt"))
	{
		tagNS_SkillInterrupt* pCmd = (tagNS_SkillInterrupt*)pNetCmd;
		if( pCmd->dwSkillID == m_skillID)
			m_pFSM->Change2IdleState();
	}
	else
	{
		m_pFSM->Change2State(pNetCmd);
	}
}

void StateSkill_NPC::OnGameEvent(tagGameEvent* pEvent)
{
	if(m_step==EStep_Spell && pEvent->strEventName==_T("tagTargetSelectEvent"))
		m_actSpell.OnTargetSelectEvent((tagTargetSelectEvent*)pEvent,m_pFSM->GetNPC());

	if(pEvent->strEventName==_T("tagBeAttackEvent"))
	{
		const tagSkillProtoClient* pProto = SkillProtoData::Inst()->FindSkillProto(m_skillID);
		const tagCreatureProto* pMonster = m_pFSM->GetNPC()->GetProto();
		if (P_VALID(pProto) && P_VALID(pMonster))
		{
			// 普通攻击被打断
			if (pProto->nType2 == ESSTE_Monster && 0 != rand()%5 && pMonster->eLite < ER_Boss)
			{
				m_pFSM->Change2State(pEvent);
			}
		}
	}
}

void StateSkill_NPC::OnExit()
{
	if(m_step==EStep_Spell)
		m_actSpell.Stop(m_pFSM->GetNPC());
	else
		m_actAttack.Stop(m_pFSM->GetNPC());
}