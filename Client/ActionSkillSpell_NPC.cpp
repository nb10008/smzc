#include "StdAfx.h"
#include "ActionSkillSpell_NPC.h"
#include "NPC.h"
#include "RoleMgr.h"
#include "..\WorldDefine\msg_combat.h"
#include "SkillProtoData.h"
#include "CombatEvent.h"
#include "EffectMgr.h"
#include "CreatureData.h"
#include "NPCSkillEffectData.h"
ActionSkillSpell_NPC::ActionSkillSpell_NPC(void)
: m_bClosed(true)
{
}

ActionSkillSpell_NPC::~ActionSkillSpell_NPC(void)
{}

void ActionSkillSpell_NPC::Active( NPC* pRole,Role* pTarget,DWORD skillID,DWORD spellTime )
{	
	m_bClosed=false;

	//--从技能属性中查找动作名
	const tagSkillProtoClient* pSkillProto=SkillProtoData::Inst()->FindSkillProto(skillID);
	ASSERT(pSkillProto!=NULL);
	
	//--播放吟唱动作
	pRole->PlayTrack(pSkillProto->szPrepareMod,true,true);

	//--自动朝向目标
	if(pTarget)
	{
		Vector3 dir=pTarget->GetPos()-pRole->GetPos();
		pRole->SetYaw(CalcYaw(dir));
	}

	//--记录技能ID
	m_skillID=skillID;
	m_spellTime=spellTime;
	m_startSpellTime=Kernel::Inst()->GetAccumTimeDW();

	//--播放吟唱特效
	StopEffect();
	if (!P_VALID(pRole->GetProto()))
		return;

    const tagEffectSet& sfxset = NPCSkillEffectData::Inst()->GetNPCEffectSet(pRole->GetProto()->szModelName, pRole->GetTypeID(), skillID, true);
	m_vecPreSfxID.resize(sfxset.model_node.size(),GT_INVALID);

	for( size_t nEff = 0; nEff < sfxset.model_node.size(); nEff++ )
	{
		//tstring strBoneName = CreatureData::Inst()->FindNpcBoneName( pRole->GetTypeID(), sfxset.model_node[nEff].first.c_str() );
        m_vecPreSfxID[nEff]=EffectMgr::Inst()->PlayRoleEffect(pRole->GetID(),sfxset.model_node[nEff].second.c_str(), sfxset.model_node[nEff].first.c_str(), EffectMgr::EEST_RoleScale, NULL);
	}


	//--发送角色开始吟唱事件
	tagRoleSpellStartEvent evt;
	evt.dwRoleID=pRole->GetID();
	evt.bSkill=true;
	evt.dwTypeID=skillID;
	evt.dwCurTime=0;
	evt.dwTimeCount=spellTime;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);
}

void ActionSkillSpell_NPC::Update(NPC* pRole,Role* pTarget)
{
	if(m_bClosed)
		return;

	if(Kernel::Inst()->GetAccumTimeDW()-m_startSpellTime>=m_spellTime)
	{
		m_bClosed=true;

		//--发送角色结束吟唱事件
		tagRoleSpellStopEvent evt;
		evt.dwRoleID=pRole->GetID();
		evt.bInterrupt=false;
		TObjRef<GameFrameMgr>()->SendEvent(&evt);

		StopEffect();
	}
}

void ActionSkillSpell_NPC::Stop(NPC* pRole)
{
	//--发送角色结束吟唱事件
	tagRoleSpellStopEvent evt;
	evt.dwRoleID=pRole->GetID();
	evt.bInterrupt=true;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);

	StopEffect();
}

void ActionSkillSpell_NPC::StopEffect()
{
	//--停止吟唱特效
	for( size_t nEff = 0; nEff < m_vecPreSfxID.size(); nEff++ )
	{
		if( GT_INVALID != m_vecPreSfxID[nEff] )
		{
			EffectMgr::Inst()->StopEffect(m_vecPreSfxID[nEff]);
			m_vecPreSfxID[nEff] = GT_INVALID;
		}
	}
}

void ActionSkillSpell_NPC::OnTargetSelectEvent(tagTargetSelectEvent* pEvent,Role* pRole)
{
	if(m_bClosed)
		return;

	//--发送角色开始吟唱事件
	tagRoleSpellStartEvent evt;
	evt.dwRoleID=pRole->GetID();
	evt.bSkill=true;
	evt.dwTypeID=m_skillID;
	evt.dwCurTime=Kernel::Inst()->GetAccumTimeDW()-m_startSpellTime;
	evt.dwTimeCount=m_spellTime;
	TObjRef<GameFrameMgr>()->SendEvent(&evt);
}