#include "StdAfx.h"
#include "UpperStateNoMovement_LP.h"
#include "FSM_LP.h"
#include "LocalPlayer.h"
#include "CombatEvent.h"
#include "..\WorldDefine\role_att.h"

UpperStateNoMovement_LP::UpperStateNoMovement_LP(void)
{}

UpperStateNoMovement_LP::~UpperStateNoMovement_LP(void)
{}

void UpperStateNoMovement_LP::Active( tagNetCmd* pNetCmd,NavMap* pNav,LocalPlayer* pRole )
{
}

void UpperStateNoMovement_LP::Update(NavMap* pNav,LocalPlayer* pRole)
{}

void UpperStateNoMovement_LP::OnGameEvent( tagGameEvent* pEvent )
{
	if(pEvent->strEventName==_T("tagUseSkillEvent"))
	{
		m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_Skill,pEvent);
	}
	else if(pEvent->strEventName==_T("tagUseItemEvent"))
	{
		m_pFSM->TryChange2UpperState(FSM_LP::EUpperState_UseItem,pEvent);
	}
}

void UpperStateNoMovement_LP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_UnSetState"))
	{
		tagNS_UnSetState* pCmd=(tagNS_UnSetState*)pNetCmd;
		if(pCmd->eState==ES_NoMovement)
		{
			//Role* pRole = m_pFSM->GetLocalPlayer();
			//if( pRole->IsInState(ES_NoMovement) ) 
				m_pFSM->Change2UpperState_Idle();
		}
	}
}

bool UpperStateNoMovement_LP::IfCanChange2UpperState( FSM_LP::EUpperState state,tagGameEvent* pEvent )
{
	return false;
}

bool UpperStateNoMovement_LP::IfCanChange2LowerState( FSM_LP::ELowerState state,tagGameEvent* pEvent )
{
	return false;
}

bool UpperStateNoMovement_LP::IfCanPlayLowerAni()
{
	return false;
}

bool UpperStateNoMovement_LP::IfLowerCanSetYaw()
{
	return false;
}	

void UpperStateNoMovement_LP::OnExit()
{
	m_pFSM->ReplayLowerAni();
}