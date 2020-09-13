#include "StdAfx.h"

#include "player_account.h"
#include "../serverdefine/role_data.h"
#include "world.h"
#include "player_session.h"
#include "db_session.h"
#include "world_session.h"



PlayerAccount::PlayerAccount()
:m_dwAccountID(0),m_timeLastLoginTime(0),m_timeLastLogoutTime(0),m_timeLastReceiveDailyOfflineReward(0),m_timeLastReceiveRegression(0),m_timeTemLogin(0)
{
}
PlayerAccount::~PlayerAccount()
{
}

PlayerAccount::PlayerAccount(const PlayerAccount & account)
:m_dwAccountID(account.m_dwAccountID),
m_timeLastLoginTime(account.m_timeLastLoginTime),
m_timeLastLogoutTime(account.m_timeLastLogoutTime),
m_timeLastReceiveDailyOfflineReward(account.m_timeLastReceiveDailyOfflineReward),
m_timeLastReceiveRegression(account.m_timeLastReceiveRegression),
m_timeTemLogin(m_timeLastLoginTime)
{
}

PlayerAccount & PlayerAccount::operator= (const PlayerAccount & account)
{
	m_dwAccountID							= account.m_dwAccountID ;
	m_timeLastLoginTime						= account.m_timeLastLoginTime ;
	m_timeLastLogoutTime					= account.m_timeLastLogoutTime ;
	m_timeLastReceiveDailyOfflineReward		= account.m_timeLastReceiveDailyOfflineReward ;
	m_timeLastReceiveRegression				= account.m_timeLastReceiveRegression ;
	m_timeTemLogin = account.m_timeTemLogin;
	return *this;
}

VOID	PlayerAccount::Assign( DWORD accountid,tagDWORDTime logintime,tagDWORDTime logouttime,tagDWORDTime dailyoffline,tagDWORDTime regression,tagDWORDTime timeTemLogin )
{
	m_dwAccountID							= accountid ;
	m_timeLastLoginTime						= logintime ;
	m_timeLastLogoutTime					= logouttime ;
	m_timeLastReceiveDailyOfflineReward		= dailyoffline ;
	m_timeLastReceiveRegression				= regression ;
	m_timeTemLogin = timeTemLogin;
}

VOID	PlayerAccount::Login( PlayerSession * pPly )
{
	if( P_VALID(pPly) )
	{
		tagDWORDTime now = g_world.GetWorldTime  ();
		m_timeLastLoginTime = now;
		//m_timeTemLogin = ;
	}
}

VOID	PlayerAccount::Logout( PlayerSession * pPly )
{
	if( P_VALID(pPly) )
	{
		tagDWORDTime now = g_world.GetWorldTime  ();
		m_timeLastLogoutTime = now;
	}
}

DWORD	PlayerAccount::GetOfflineTime()
{
	DWORD delta = 0;
	tagDWORDTime now = g_world.GetWorldTime  ();
	if( (DWORD)m_timeLastLogoutTime == 0 )
	{
		m_timeLastLogoutTime = now;
	}
	if( (DWORD)m_timeTemLogin == 0 )
	{
		m_timeTemLogin = now;
	}
	delta = CalcTimeDiff  (m_timeTemLogin,m_timeLastLogoutTime);
	return delta;
}

DWORD	PlayerAccount::GetDailyOfflineTime()
{
	DWORD delta = 0;
	tagDWORDTime now = g_world.GetWorldTime  ();
	tagDWORDTime startTime ,endTime;
	startTime = m_timeTemLogin;
	endTime   = m_timeLastLogoutTime;
	if( (DWORD)m_timeLastLogoutTime == 0 )
	{
		m_timeLastLogoutTime = now;
		endTime = now;
	}
	else 
	{
		tagDWORDTime tem ,tem1;
		tem.year	= now.year;
		tem.month	= now.month;
		tem.day		= now.day;
		tem.hour	= 0;
		tem.min		= 0;
		tem.sec		= 0;
		tem1.year	= now.year;
		tem1.month	= now.month;
		tem1.day		= now.day;
		tem1.hour	= 23;
		tem1.min		= 59;
		tem1.sec		= 59;
		if( /*endTime < tem ||*/ endTime > tem1 ) //防挂
			endTime = tem;
	}

	if( (DWORD)m_timeTemLogin == 0 )
	{
		m_timeTemLogin = now;
		startTime = now;
	}
	else
	if(  m_timeTemLogin.year != now.year || m_timeTemLogin.month != now.month || m_timeTemLogin.day != now.day  )
	{
		//startTime = now;
		if( startTime > now ) //挂
			startTime = endTime;
		//else //可能是没有保存到，所以弄到与now一样
		//	startTime = now;
	}

	delta = CalcTimeDiff  (startTime,endTime);
	return delta;
}

BOOL	PlayerAccount::CanReceiveDailyOfflineReward()
{
	//不管领不领，过了一天，同一天就不能领取
	//tagDWORDTime now = g_world.GetWorldTime  ();
	//if( m_timeLastReceiveDailyOfflineReward.year	!= now.year  ||
	//	m_timeLastReceiveDailyOfflineReward.month	!= now.month ||
	//	m_timeLastReceiveDailyOfflineReward.day		!= now.day)
	if( CalcTimeDiff  (m_timeLastLoginTime,m_timeLastLogoutTime) >= 60 * 60 && m_timeLastReceiveDailyOfflineReward < m_timeLastLogoutTime )
		return TRUE;
	return FALSE;
}

BOOL	PlayerAccount::CanReceiveRegressionReward(DWORD delta)
{
	if( CalcTimeDiff  (m_timeTemLogin,m_timeLastLogoutTime) >= delta && 
		( m_timeLastReceiveRegression.day != m_timeTemLogin.day || 
		  m_timeLastReceiveRegression.month != m_timeTemLogin.month ||
		  m_timeLastReceiveRegression.year != m_timeTemLogin.year ) )
		return TRUE;
	return FALSE;
}



PlayerAccountMgr::PlayerAccountMgr()
{
}
PlayerAccountMgr::~PlayerAccountMgr()
{
}

VOID PlayerAccountMgr::OnRecAccountInfo(const PlayerAccount & t)
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	m_mapAccount[t.m_dwAccountID] = t;
	m_mapAccount[t.m_dwAccountID].m_timeLastLoginTime = g_world.GetWorldTime  ();
	if( m_mapAccount[t.m_dwAccountID].m_timeTemLogin == 0 )
		m_mapAccount[t.m_dwAccountID].m_timeTemLogin = m_mapAccount[t.m_dwAccountID].m_timeLastLoginTime;
	SaveAccountTimeInfoOneTick(t.m_dwAccountID);
}
VOID PlayerAccountMgr::RoleLogin(DWORD accountid)
{
	PlayerSession * p = g_worldSession.FindSession(accountid);

	jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	TAccountMap::iterator i = m_mapAccount.find(accountid);
	if( i != m_mapAccount.end  () && P_VALID(p) )
	{
		i->second.Login  (p);
		SaveAccountTimeInfoOneTick(accountid);
	}
	else
	{
		tagNDBC_LoadAccountInfo msg;
		msg.dwAccountID	= accountid;
		g_dbSession.Send  (&msg,msg.dwSize);
	}
}
VOID PlayerAccountMgr::RoleLogout(DWORD accountid)
{
	PlayerSession * p = g_worldSession.FindSession(accountid);

	jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	TAccountMap::iterator i = m_mapAccount.find(accountid);
	if( i != m_mapAccount.end  () && P_VALID(p) )
	{
		i->second.Logout  (p);
		// send message to db server
		tagNDBC_SaveAccountInfo msg;
		msg.dwAccountID		= i->second.m_dwAccountID;
		msg.timeLastLogin	= i->second.m_timeLastLoginTime;
		msg.timeLastLogout	= i->second.m_timeLastLogoutTime;
		msg.timeLastReceiveDailyOfflineReward	= i->second.m_timeLastReceiveDailyOfflineReward;
		msg.timeLastReceiveRegression			= i->second.m_timeLastReceiveRegression;
		g_dbSession.Send  (&msg,msg.dwSize);
		m_mapAccount.erase  (i);
	}
}

DWORD	PlayerAccountMgr::GetOfflineTime(DWORD accountid)
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	TAccountMap::iterator i = m_mapAccount.find(accountid);
	if( i != m_mapAccount.end  () )
		return i->second.GetOfflineTime();
	return 0;
}
DWORD	PlayerAccountMgr::GetDailyOfflineTime(DWORD accountid)
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	TAccountMap::iterator i = m_mapAccount.find(accountid);
	if( i != m_mapAccount.end  () )
		return i->second.GetDailyOfflineTime();
	return 0;
}
BOOL	PlayerAccountMgr::CanReceiveDailyOfflineReward(DWORD accountid)
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	TAccountMap::iterator i = m_mapAccount.find(accountid);
	if( i != m_mapAccount.end  () )
		return i->second.CanReceiveDailyOfflineReward();
	return FALSE;
}
BOOL	PlayerAccountMgr::CanReceiveRegressionReward(DWORD accountid,DWORD delta)
{
	//jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	//TAccountMap::iterator i = m_mapAccount.find(accountid);
	//if( i != m_mapAccount.end  () )
	//	return i->second.CanReceiveRegressionReward(delta);
	return FALSE;
}
VOID	PlayerAccountMgr::ReceiveDailyOfflineRewardTime( DWORD accountid, tagDWORDTime  rec_time )
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	TAccountMap::iterator i = m_mapAccount.find(accountid);
	if( i != m_mapAccount.end  () )
	{
		bool b = false;
		if( (DWORD)i->second.m_timeLastReceiveDailyOfflineReward != (DWORD)rec_time )
			b = true;
		i->second.m_timeLastReceiveDailyOfflineReward = rec_time;
		if(b)
		{
			//i->second.Logout  (NULL);
			//i->second.m_timeTemLogin = i->second.m_timeLastLoginTime;
			tagNDBC_SaveAccountInfo msg;
			msg.dwAccountID		= i->second.m_dwAccountID;
			msg.timeLastLogin	= i->second.m_timeLastLoginTime;
			msg.timeLastLogout	= g_world.GetWorldTime();//i->second.m_timeLastLogoutTime;
			msg.timeLastReceiveDailyOfflineReward	= i->second.m_timeLastReceiveDailyOfflineReward;
			msg.timeLastReceiveRegression			= i->second.m_timeLastReceiveRegression;
			g_dbSession.Send  (&msg,msg.dwSize);
		}
	}
}
tagDWORDTime	PlayerAccountMgr::ReceiveDailyOfflineRewardTime(DWORD accountid)
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	TAccountMap::iterator i = m_mapAccount.find(accountid);
	if( i != m_mapAccount.end  () )
		return i->second.m_timeLastReceiveDailyOfflineReward;
	return tagDWORDTime(0);
}

VOID	PlayerAccountMgr::ReceiveRegressionRewardTime( DWORD accountid, tagDWORDTime  rec_time)
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	TAccountMap::iterator i = m_mapAccount.find(accountid);
	if( i != m_mapAccount.end  () )
	{
		bool b = false;
		if( (DWORD)i->second.m_timeLastReceiveRegression != (DWORD)rec_time )
			b = true;
		i->second.m_timeLastReceiveRegression = rec_time;
		if(b)
		{
			i->second.Logout  (NULL);
			if( rec_time > 0 )
				i->second.m_timeTemLogin = i->second.m_timeLastLoginTime;
		}
	}
}
tagDWORDTime PlayerAccountMgr::ReceiveRegressionRewardTime(DWORD accountid)
{
	jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	TAccountMap::iterator i = m_mapAccount.find(accountid);
	if( i != m_mapAccount.end  () )
		return i->second.m_timeLastReceiveRegression;
	return tagDWORDTime(0);
}

VOID	PlayerAccountMgr::SaveAccountTimeInfoOneTick( DWORD accountid )
{
	//PlayerSession * p = g_worldSession.FindSession(accountid);

	jm_tool::MutexGuard<jm_tool::CSCreatedLock> guard(m_cs);
	TAccountMap::iterator i = m_mapAccount.find(accountid);
	if( i != m_mapAccount.end  () /*&& P_VALID(p)*/ )
	{
		// send message to db server
		tagNDBC_SaveAccountInfo msg;
		msg.dwAccountID		= i->second.m_dwAccountID;
		msg.timeLastLogin	= i->second.m_timeLastLoginTime;
		msg.timeLastLogout	= g_world.GetWorldTime();//i->second.m_timeLastLogoutTime;
		msg.timeLastReceiveDailyOfflineReward	= i->second.m_timeLastReceiveDailyOfflineReward;
		msg.timeLastReceiveRegression			= i->second.m_timeLastReceiveRegression;
		g_dbSession.Send  (&msg,msg.dwSize);
		//m_mapAccount.erase  (i);
	}
}


PlayerAccountMgr g_PlayerAccountMgr;

