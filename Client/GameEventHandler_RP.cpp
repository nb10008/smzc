#include "StdAfx.h"
#include "GameEventHandler_RP.h"
#include "CombatEvent.h"
#include "FSM_RP.h"
#include "Player.h"
#include "CombatEvent.h"
#include "EffectMgr.h"
#include "SkillProtoData.h"
#include "ItemProtoData.h"
#include "..\WorldDefine\remote_role.h"

//--class NetCmdHander_RP-------------------------------------------
GameEventHandler_RP::GameEventHandler_RP(void):m_pFSM(NULL)
{}

GameEventHandler_RP::~GameEventHandler_RP(void)
{}







//--class HitTargetEventHandler_RP-------------------------------------------
HitTargetEventHandler_RP::HitTargetEventHandler_RP( void )
{}

HitTargetEventHandler_RP::~HitTargetEventHandler_RP( void )
{}

void HitTargetEventHandler_RP::OnGameEvent( tagGameEvent* pEvent )
{
	if(pEvent->strEventName==_T("tagHitTargetEvent"))
	{
		tagHitTargetEvent* pHitEvent=(tagHitTargetEvent*)pEvent;

		map<DWORD,tagHitTargetEvent>::iterator iter=m_eventMap.find(pHitEvent->dwSrcRoleID);
		if(iter==m_eventMap.end())
		{
			m_eventMap[pHitEvent->dwSrcRoleID]=*pHitEvent;
		}
		else
		{
			if(pHitEvent->dwSerial<iter->second.dwSerial)
				return;
			if(pHitEvent->dwSerial==iter->second.dwSerial
				&&pHitEvent->dwMisc2<iter->second.dwMisc2)
				return;
			iter->second=*pHitEvent;
		}
	}
}

void HitTargetEventHandler_RP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_RemoveRemote"))
	{
		tagNS_RemoveRemote* pCmd=(tagNS_RemoveRemote*)pNetCmd;
		int num = ( pCmd->dwSize - sizeof(tagNS_RemoveRemote) ) / sizeof(DWORD) + 1;
		for(int i=0;i<num;i++)
		{
			m_eventMap.erase(pCmd->dwRoleID[i]);
		}
	}
}

tagHitTargetEvent* HitTargetEventHandler_RP::GetLastHitTargetEvent( DWORD roleID )
{
	map<DWORD,tagHitTargetEvent>::iterator iter=m_eventMap.find(roleID);
	if(iter==m_eventMap.end())
		return NULL;

	return &iter->second;
}

//--class EffectHPChangeEventHandler_RP-------------------------------------------
EffectHPChangeEventHandler_RP::EffectHPChangeEventHandler_RP( void )
{}

EffectHPChangeEventHandler_RP::~EffectHPChangeEventHandler_RP( void )
{}

void EffectHPChangeEventHandler_RP::OnGameEvent( tagGameEvent* pEvent )
{
	if(pEvent->strEventName==_T("tagEffectHPChangeEvent"))
	{
		tagEffectHPChangeEvent* pHitEvent=(tagEffectHPChangeEvent*)pEvent;

		map<DWORD,tagEffectHPChangeEvent>::iterator iter=m_eventMap.find(pHitEvent->dwSrcRoleID);
		if(iter==m_eventMap.end())
		{
			m_eventMap[pHitEvent->dwSrcRoleID]=*pHitEvent;
		}
		else
		{
			if(pHitEvent->dwSerial<iter->second.dwSerial)
				return;
			if(pHitEvent->dwSerial==iter->second.dwSerial
				&&pHitEvent->dwMisc2<iter->second.dwMisc2)
				return;
			iter->second=*pHitEvent;
		}
	}
}

void EffectHPChangeEventHandler_RP::OnNetCmd( tagNetCmd* pNetCmd )
{
	if(pNetCmd->dwID==pNetCmd->Crc32("NS_RemoveRemote"))
	{
		tagNS_RemoveRemote* pCmd=(tagNS_RemoveRemote*)pNetCmd;
		int num = ( pCmd->dwSize - sizeof(tagNS_RemoveRemote) ) / sizeof(DWORD) + 1;
		for(int i=0;i<num;i++)
		{
			m_eventMap.erase(pCmd->dwRoleID[i]);
		}
	}
}

tagEffectHPChangeEvent* EffectHPChangeEventHandler_RP::GetLastEffectHPChangeEvent( DWORD roleID )
{
	map<DWORD,tagEffectHPChangeEvent>::iterator iter=m_eventMap.find(roleID);
	if(iter==m_eventMap.end())
		return NULL;

	return &iter->second;
}