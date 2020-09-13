#include "StdAfx.h"
#include "GameEventHandler_NPC.h"
#include "CombatEvent.h"
#include "FSM_NPC.h"
#include "NPC.h"
#include "CombatEvent.h"
#include "EffectMgr.h"
#include "SkillProtoData.h"
#include "ItemProtoData.h"
#include "..\WorldDefine\remote_role.h"

//--class NetCmdHander_NPC-------------------------------------------
GameEventHandler_NPC::GameEventHandler_NPC(void):m_pFSM(NULL)
{}

GameEventHandler_NPC::~GameEventHandler_NPC(void)
{}





//--class HitTargetEventHandler_NPC-------------------------------------------
HitTargetEventHandler_NPC::HitTargetEventHandler_NPC( void )
{}

HitTargetEventHandler_NPC::~HitTargetEventHandler_NPC( void )
{}

void HitTargetEventHandler_NPC::OnGameEvent( tagGameEvent* pEvent )
{
	if(pEvent->strEventName==_T("tagHitTargetEvent"))
	{
		tagHitTargetEvent* pHitEvent=(tagHitTargetEvent*)pEvent;

		//IMSG(_T("[%d]HitTargetEventHandler_NPC::OnGameEvent recv hittarget,skill=%d,srcrole=%d,serial=%d,channel=%d\n"),
		//	timeGetTime(),pHitEvent->dwMisc,pHitEvent->dwSrcRoleID,pHitEvent->dwSerial,pHitEvent->dwMisc2);

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

void HitTargetEventHandler_NPC::OnNetCmd( tagNetCmd* pNetCmd )
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

tagHitTargetEvent* HitTargetEventHandler_NPC::GetLastHitTargetEvent( DWORD roleID )
{
	map<DWORD,tagHitTargetEvent>::iterator iter=m_eventMap.find(roleID);
	if(iter==m_eventMap.end())
		return NULL;

	return &iter->second;
}

//--class EffectHPChangeEventHandler_NPC-------------------------------------------
EffectHPChangeEventHandler_NPC::EffectHPChangeEventHandler_NPC( void )
{}

EffectHPChangeEventHandler_NPC::~EffectHPChangeEventHandler_NPC( void )
{}

void EffectHPChangeEventHandler_NPC::OnGameEvent( tagGameEvent* pEvent )
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

void EffectHPChangeEventHandler_NPC::OnNetCmd( tagNetCmd* pNetCmd )
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

tagEffectHPChangeEvent* EffectHPChangeEventHandler_NPC::GetLastEffectHPChangeEvent( DWORD roleID )
{
	map<DWORD,tagEffectHPChangeEvent>::iterator iter=m_eventMap.find(roleID);
	if(iter==m_eventMap.end())
		return NULL;

	return &iter->second;
}

////////////////////////////////////add by hpy

SkipFrameEventHandler_NPC::SkipFrameEventHandler_NPC( void )
{
}

SkipFrameEventHandler_NPC::~SkipFrameEventHandler_NPC( void )
{
	m_cache.clear();
}

void SkipFrameEventHandler_NPC::OnNetCmd( tagNetCmd *pNetCmd )
{
	if( pNetCmd->dwID == m_pUtil->Crc32("NS_HitTarget") )
	{
		tagNS_HitTarget* pCmd = (tagNS_HitTarget*)pNetCmd;
		if( pCmd->dwRoleID == m_pFSM->GetNPC()->GetID() )
		{
			tagHitTargetEvent* pLastEvent = m_pFSM->GetLastHitTargetEvent( pCmd->dwSrcRoleID );
			if( pLastEvent == NULL )
			{
				CacheCmd( pCmd );
			}
			else if( pCmd->dwSerial > pLastEvent->dwSerial )
			{
				CacheCmd( pCmd );
			}
		}
	}
}

void SkipFrameEventHandler_NPC::OnGameEvent( tagGameEvent* pEvent )
{
	if( pEvent->strEventName == _T("tagHitTargetEvent") )
	{
		tagHitTargetEvent* pHitEvent = (tagHitTargetEvent*)pEvent;

		// 只处理群攻技能 和 有弹道的技能
		BOOL bShake = FALSE;
		NPC* pNpc = m_pFSM->GetNPC();
		if( !P_VALID(pNpc) )
			return;
		const tagCreatureProto* pProto = pNpc->GetProto();
		if( !P_VALID(pProto) )
			return;
		if( pProto->eLite >= ER_Boss )
		{
			for( list<tagData>::iterator iter = m_cache.begin(); iter != m_cache.end(); )
			{
				if( IfNeedHandleCmd( &iter->cmd, pHitEvent ) )
				{
					//Role* pRole = RoleMgr::Inst()->FindRole( pHitEvent->dwSrcRoleID );
					//if( P_VALID(pRole) )
					//	pRole->SkipFrame( pHitEvent->nSrcSkipFrameNum );
					pNpc->Shake( pProto->dwTarShakeTimeLen, pProto->dwShakeD );


					iter = m_cache.erase( iter );
					continue;
				}

				++iter;
			}
		}
	}
	//else if( pEvent->strEventName == _T("tagSkipFrameEvent") )
	//{
	//	tagSkipFrameEvent* pSkipFrameEvent = (tagSkipFrameEvent*)pEvent;
	//	NPC* pNpc = m_pFSM->GetNPC();
	//	if( P_VALID(pNpc) )
	//	{
	//		pNpc->SkipFrame( pSkipFrameEvent->nTarSkipFrameNum );
	//		pNpc->Shake( pSkipFrameEvent->dwTarShakeTimeLen, pSkipFrameEvent->nTarShakeD );
	//	}
	//}
}

void SkipFrameEventHandler_NPC::Update()
{
	for( list<tagData>::iterator iter = m_cache.begin(); iter != m_cache.end(); )
	{
		if( Kernel::Inst()->GetAccumTimeDW() - iter->recvTime >= 5000 )
		{
			iter = m_cache.erase( iter );
		}
		else
			iter++;
	}
}

void SkipFrameEventHandler_NPC::CacheCmd( tagNS_HitTarget* pCmd )
{
	tagData data;
	data.recvTime = Kernel::Inst()->GetAccumTimeDW();
	data.cmd = *pCmd;
	m_cache.push_back( data );
}

bool SkipFrameEventHandler_NPC::IfNeedClearCmd( tagNS_HitTarget* pCacheCmd,tagHitTargetEvent* pEvent )
{
	if( pCacheCmd->dwSrcRoleID == pEvent->dwSrcRoleID )
	{
		if( pEvent->dwSerial >= pCacheCmd->dwSerial )
			return true;
	}

	return false;
}

bool SkipFrameEventHandler_NPC::IfNeedHandleCmd( tagNS_HitTarget* pCacheCmd, tagHitTargetEvent* pEvent )
{
	if( pCacheCmd->dwSrcRoleID == pEvent->dwSrcRoleID )
	{
		if( pEvent->dwSerial >= pCacheCmd->dwSerial )
			return true;
	}

	return false;
}