#include "StdAfx.h"
#include "StateSkill_PET.h"
#include "FSM_PET.h"
#include "Pet.h"
#include "CombatEvent.h"
#include "EffectMgr.h"

StateSkill_PET::StateSkill_PET(void)
{
}

StateSkill_PET::~StateSkill_PET(void)
{
}

void StateSkill_PET::Active( tagNetCmd* pNetCmd, NavMap* pNav, Pet* pRole )
{
    // 播放施法动作
    pRole->PlayTrack(_T("satk"),false);

    // 播放施法特效
    EffectMgr::Inst()->PlayRoleEffect(pRole->GetID(), _T("Txpt01"));
}

void StateSkill_PET::Update( NavMap* pNav, Pet* pRole )
{
    if( !pRole->IsAttributeInited() || !pRole->IsPet() )
        return;

    if(pRole->IsTrackEnd(_T("satk")))
    {
        m_pFSM->Change2IdleState();
    }
}

void StateSkill_PET::OnNetCmd( tagNetCmd* pNetCmd )
{
    //m_pFSM->Change2State(pNetCmd);
}

void StateSkill_PET::OnGameEvent( tagGameEvent* pEvent )
{
    m_pFSM->Change2State(pEvent);
}