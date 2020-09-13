#include "StdAfx.h"
#include "player_regression.h"
#include "../worlddefine/msg_player_preventlost.h"
#include <math.h>
#include "world.h"
#include "role.h"
#include "map.h"
#include "creature.h"
#include "../vEngine/globe/globe_typedefine.h"
// Jason 2010-1-27 v1.3.2 玩家回归/离线奖励相关
#include "player_account.h"
#include "item_creator.h"

//#define REGRESSION_DEBUG

#ifdef REGRESSION_DEBUG
#define TIMEOUT_HOUR	1
#else
#define TIMEOUT_HOUR	(60 * 60)
#endif

VOID PlayerRegression::ClearRules()
{
	m_mapRules.clear  ();
}

VOID PlayerRegression::AddRule(VarContainer * pVar,LPCTSTR field)
{
	if( P_VALID(pVar) && field )
	{
		tagPlayRegressionRule t;
		t._time_low			= pVar->GetInt  (_T("time_low_limit"),field);
		t._time_hight			= pVar->GetInt  (_T("time_high_limit"),field);
		t._level_low			= pVar->GetInt  (_T("level_low_limit"),field);
		t._level_hight			= pVar->GetInt  (_T("level_high_limit"),field);
		t._item_type			= pVar->GetInt  (_T("item_id"),field);

		//MutexGuard<CSCreatedLock> guard(m_cs);
		DWORD dwID = pVar->GetDword(_T("id"),field);
		m_mapRules[dwID] = t;
	}
}

BOOL PlayerRegression::Initial()
{
#if 0
	tagDWORDTime now = g_world.GetWorldTime  ();
	MutexGuard<CSCreatedLock> guard(m_cs);
	size_t size = m_mapRules.Size  ();
	tagPlayRegressionRule rule;
	DWORD key = 0;
	m_mapRules.ResetIterator();
	for( size_t i = 0; i < size ; ++i )
	{
		m_mapRules.PeekNext  (key,rule);
		if( rule.StartTime <= now && rule.EndTime > now )
		{
			//::InterlockedExchange((LONG*)&m_lOpen,1);
			//m_curRule = rule;
			//m_dwMapID = key;
			//return TRUE;
			rule.Opened = TRUE;
		}
	}
	return FALSE;
#endif
	return TRUE;
}

BOOL PlayerRegression::IniRole(Role * pRole)
{
	if( P_VALID(pRole) )
		return IniPlayer(pRole->GetSession  ());
	return FALSE;
}

BOOL PlayerRegression::IniPlayer(PlayerSession * pRole)
{
	//if( /*IsOpen() && */P_VALID(pRole) )
	if( g_world.IsPlayerBack())
	{
		PlayerSession * pSession = pRole;
		if( !P_VALID(pSession) )
			return FALSE;

		Role * pMy = pSession->GetRole();
		if(!P_VALID(pMy))
			return FALSE;
		if( pMy->IsPlayerBack() )
			return FALSE;

		tagDWORDTime timeLogout = pMy->GetRealLogoutTime();
		tagDWORDTime timeLogin = pMy->GetLoginTime();
		DWORD dwOfflineTime = CalcTimeDiff(timeLogin,timeLogout);
		DWORD dwDay = dwOfflineTime / (24 * 60 * 60);
		if(pMy->IsPlayerBackNotRecv())
			dwDay = pMy->GetPlayerBackDays();
		else
			pMy->SetPlayerBackDays((BYTE)dwDay);


		//MutexGuard<CSCreatedLock> guard(m_cs);

		for( TRegMap::iterator i = m_mapRules.begin  (); i != m_mapRules.end  () ; ++i )
		{
			//if ( i->second.Opened )
			{
				if(  dwDay >= i->second._time_low && dwDay <= i->second._time_hight &&
					pMy->GetLevel() >= i->second._level_low &&
					pMy->GetLevel() <= i->second._level_hight ) 
					// 其实这个recLastTime > rule.EndTime是为了调试等情况准备的，但只这个recLastTime在区间内，则肯定不能领取了
				{
					tagNS_PlayerRegression msg;
					msg.dwItemTypeID = i->second._item_type;
					msg.nItemNum = 1;
					tagItemProto * pItemProto = g_attRes.GetItemProto( i->second._item_type );
					if( P_VALID(pItemProto) )
						msg.byQuality = pItemProto->byQuality;
					else
						msg.byQuality = EIQ_White;
					pSession->SendMessage  (&msg,msg.dwSize);
					pMy->PlayerBack(Role::EPlyBack_NotRecv);
					return TRUE;
				}
			}
		}


		return FALSE;
	}
	return FALSE;
}

DWORD PlayerRegression::RewardPlayerBack(Role * pRole)
{
	if( !g_world.IsPlayerBack())
		return FALSE;
	if( !P_VALID(pRole) )
		return FALSE;

	if(pRole->IsPlayerBack())
		return FALSE;

	tagDWORDTime timeLogout = pRole->GetRealLogoutTime();
	tagDWORDTime timeLogin = pRole->GetLoginTime();
	DWORD dwOfflineTime = CalcTimeDiff(timeLogin,timeLogout);
	DWORD dwDay = dwOfflineTime / (24 * 60 * 60);
	if(pRole->IsPlayerBackNotRecv())
		dwDay = pRole->GetPlayerBackDays();

	if( pRole->GetItemMgr().GetBagFreeSize() < 1 )
	{
		tagNS_PlayerRegressionOk msg;
		msg.dwErrorCode = E_Bag_NotEnoughSpace;
		pRole->SendMessage(&msg,msg.dwSize);
		return FALSE;
	}


	//MutexGuard<CSCreatedLock> guard(m_cs);

	for( TRegMap::iterator i = m_mapRules.begin  (); i != m_mapRules.end  () ; ++i )
	{
		//if ( i->second.Opened )
		{
			if(  dwDay >= i->second._time_low && dwDay <= i->second._time_hight &&
				pRole->GetLevel() >= i->second._level_low &&
				pRole->GetLevel() <= i->second._level_hight ) 
				// 其实这个recLastTime > rule.EndTime是为了调试等情况准备的，但只这个recLastTime在区间内，则肯定不能领取了
			{
				tagItem * pItem = ItemCreator::Create(EICM_PlayerBack,pRole->GetID(),i->second._item_type,1);
				if( P_VALID(pItem) )
				{
					pRole->GetItemMgr().Add2Bag(pItem,EICM_PlayerBack,TRUE);
					pRole->PlayerBack(TRUE);
					tagNS_PlayerRegressionOk msg;
					msg.dwErrorCode = E_Success;
					pRole->SendMessage(&msg,msg.dwSize);
					pRole->PlayerBack(Role::EPlyBack_Recv);
					pRole->SetPlayerBackDays(0);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}
BOOL PlayerRegression::CanRecRegression(tagDWORDTime timeLogout,BYTE byBackFlag,UINT16 days,BYTE byLevel)
{
	if( !g_world.IsPlayerBack())
		return FALSE;
	if(byBackFlag == Role::EPlyBack_Recv)
		return FALSE;
	tagDWORDTime now = g_world.GetWorldTime();
	DWORD dwOfflineTime = CalcTimeDiff(now,timeLogout);
	DWORD dwDay = dwOfflineTime / (24 * 60 * 60);
	if(Role::EPlyBack_NotRecv == byBackFlag)
		dwDay = days;
	for( TRegMap::iterator i = m_mapRules.begin  (); i != m_mapRules.end  () ; ++i )
	{
		if(  dwDay >= i->second._time_low && dwDay <= i->second._time_hight &&
			byLevel >= i->second._level_low &&
			byLevel <= i->second._level_hight ) 
		{
			return TRUE;
		}
	}
	return FALSE;
}
BOOL PlayerRegression::CanRecRegression(Role*pRole)
{
	if( !g_world.IsPlayerBack())
		return FALSE;
	if( /*IsOpen() && */P_VALID(pRole) )
	{
		PlayerSession * pSession = pRole->GetSession  ();
		if( !P_VALID(pSession) )
			return FALSE;

		if(pRole->IsPlayerBack())
			return FALSE;

		tagDWORDTime timeLogout = pRole->GetRealLogoutTime();
		tagDWORDTime timeLogin = pRole->GetLoginTime();
		DWORD dwOfflineTime = CalcTimeDiff(timeLogin,timeLogout);
		DWORD dwDay = dwOfflineTime / (24 * 60 * 60);
		if(pRole->IsPlayerBackNotRecv())
			dwDay = pRole->GetPlayerBackDays();


		//MutexGuard<CSCreatedLock> guard(m_cs);

		for( TRegMap::iterator i = m_mapRules.begin  (); i != m_mapRules.end  () ; ++i )
		{
			//if ( i->second.Opened )
			{
				if(  dwDay >= i->second._time_low && dwDay <= i->second._time_hight &&
					pRole->GetLevel() >= i->second._level_low &&
					pRole->GetLevel() <= i->second._level_hight ) 
				{
					return TRUE;
				}
			}
		}

	}
	return FALSE;
}


BOOL PlayerRegression::IniMap(Map * pMap)
{
	return TRUE;
}

VOID PlayerRegression::UpdateMap(Map * pMap)
{
	return;
	
}
PlayerRegression::PlayerRegression()
:m_lCounter(0)
{
	::InterlockedExchange((LONG*)&m_lOpen,0);
}

PlayerRegression::~PlayerRegression()
{
}
DWORD  PlayerRegression::RoleGotoNpc(Role * pRole)
{
	return GT_INVALID;
	
}

#ifdef TIMEOUT_HOUR
#undef TIMEOUT_HOUR
#endif
