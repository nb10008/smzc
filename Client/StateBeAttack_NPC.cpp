#include "StdAfx.h"
#include "StateBeAttack_NPC.h"
#include "FSM_NPC.h"
#include "NPC.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_combat.h"
#include "..\WorldDefine\action.h"
#include "SkillProtoData.h"
#include <errno.h>
#include "CombatEvent.h"


StateBeAttack_NPC::StateBeAttack_NPC(void)
{}

StateBeAttack_NPC::~StateBeAttack_NPC(void)
{}

void StateBeAttack_NPC::Active( tagNetCmd* pNetCmd,NavMap* pNav,NPC* pRole )
{
	tagNS_NPCBeAttacked* pCmd = (tagNS_NPCBeAttacked*)pNetCmd;
	const tagCreatureProto* pProto = pRole->GetProto();
	m_trackName = _T("batk");
	if(P_VALID(pProto))
	{
		if (pProto->eLite >= ER_Boss)
		{
			m_trackName = _T("batk");
		}
		else
		{
			switch(pCmd->dwBatkAction)
			{
			case 0:
				m_trackName = _T("batk");
				break;
			case 1:
				m_trackName = _T("bak2");
				break;
			case 2:
				m_trackName = _T("batk");
				break;
			default:
				m_trackName = _T("batk");
				break;
			}
		}
	}

	if (m_trackName == _T("bak2"))
	{
		tstringstream ss;
		ss << _T("data\\animation\\npc\\") << pProto->szModelName << _T("\\bak2.FSKEL");
		TObjRef<VirtualFileSys> pVFS;
		//--读取文件
		DWORD dwSize = pVFS->GetSize(ss.str().c_str());
		if(dwSize == GT_INVALID)
			m_trackName = _T("batk");
	}


	pRole->PlayTrack(m_trackName.c_str(),false,true);

	// 播放被攻击喊叫声(%5几率)
	if( 0 == rand() % 20 )
		CombatSysUtil::Inst()->PlayMonsterHowlSound(pRole);
}

void StateBeAttack_NPC::Update(NavMap* pNav,NPC* pRole)
{
	if(m_trackName.size()==0 || pRole->IsTrackEnd(m_trackName.c_str()))
		m_pFSM->Change2IdleState();
}

void StateBeAttack_NPC::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==m_pUtil->Crc32("NS_Skill"))
	{
		tagNS_Skill* pSkillCmd=(tagNS_Skill*)pNetCmd;
		const tagSkillProtoClient* pSkillProto = SkillProtoData::Inst()->FindSkillProto(pSkillCmd->dwSkillID);
		if (P_VALID(pSkillProto))
		{
			//是否是普通攻击,直接返回，继续被攻击
			if(pSkillProto->nType2 == ESSTE_Monster)
				return;
		}
	}
	m_pFSM->Change2State(pNetCmd);
}

void StateBeAttack_NPC::OnGameEvent( tagGameEvent* pEvent )
{
	// 被攻击不能被被攻击打断
	if(pEvent->strEventName==_T("tagBeAttackEvent")) 
	{
		if(m_trackName.size()==0 || m_pFSM->GetNPC()->IsTrackEnd(m_trackName.c_str()))
		{
			tagBeAttackEvent* pBeAatkEvent = (tagBeAttackEvent*)pEvent;
			tagNS_NPCBeAttacked msg;
			msg.dwBatkAction = pBeAatkEvent->dwActionType;
			Active(&msg, NULL, m_pFSM->GetNPC());
		}
		return;		
	}
	m_pFSM->Change2State(pEvent);
	m_pFSM->Change2State(pEvent);
}