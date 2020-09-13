//-----------------------------------------------------------------------------
//!\file shutdown_server.cpp
//!\author Jason
//!
//!\date 2010-10-8
//! last 
//!
//!\brief 友好关服管理器
//!
//!	Copyright (c) 2004 DACHENG Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "shutdown_server.h"
#include "../worlddefine/scriptmsginfo.h"
#include "world.h"
#include "role.h"
//#define BROADCAST_TO_CLIENT

#if defined( BROADCAST_TO_CLIENT )
#	include "const_define.h"
#	include "MsgInfoMgr.h"
#else
#	include "../WorldDefine/msg_shutdown.h"
#	include "role_mgr.h"
#endif

FriendlyShutdownServer g_FriendlyShutdownServer;

FriendlyShutdownServer::FriendlyShutdownServer()
:m_nShutdownTick(GT_INVALID),m_nLastBroadcastTick(GT_INVALID)
{
}
FriendlyShutdownServer::~FriendlyShutdownServer()
{
}

VOID	FriendlyShutdownServer::BroadcastShutdownTimes(DWORD dwBroadcastID,INT nSeconds)
{
#if defined( BROADCAST_TO_CLIENT )
	DWORD	dwMsgInfoID		= (DWORD)g_MsgInfoMgr.BeginMsgEvent();
	DWORD   dwTmpNum			= dwBroadcastID;
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwTmpNum);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_Integer, &nSeconds);
	g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);
#else
	tagNS_WillShutdownWorld msg;
	msg.nSecondsDelay	= nSeconds;
	g_roleMgr.SendWorldMsg(&msg,msg.dwSize);
#endif
}

VOID	FriendlyShutdownServer::TryBroadcastShutdownTimes(VOID)
{
#if defined( BROADCAST_TO_CLIENT )
	const static int nMinuTicks = TICK_PER_SECOND*60;
	const static int n5sTicks = TICK_PER_SECOND * 5;
	// 准备广播
	if( m_nShutdownTick > 0 &&  m_nShutdownTick >= nMinuTicks ) // 大于一分钟时，每分钟广播一次
	{
		if( m_nShutdownTick % nMinuTicks == 0 )
		{
			INT nTime = m_nShutdownTick * TICK_TIME / 1000;
			if( m_nShutdownTick == nMinuTicks )
			{
				BroadcastShutdownTimes(BroadcastShutdownDelayTicksPopupDialog,nTime);
			}
			else
			{
				BroadcastShutdownTimes(BroadcastShutdownDelayTicks,nTime);
			}
		}
	}
	else if( m_nShutdownTick > 0 )	// 小于一分钟，每5s广播一次
	{
		if( m_nShutdownTick % n5sTicks == 0 )
		{
			INT nTime = m_nShutdownTick * TICK_TIME / 1000;
			BroadcastShutdownTimes(BroadcastShutdownDelayTicks,nTime);
		}
	}
#else
	if( m_nLastBroadcastTick == GT_INVALID && m_nShutdownTick > 0 )
	{
		m_nLastBroadcastTick = m_nShutdownTick;
		BroadcastShutdownTimes(0,m_nShutdownTick * TICK_TIME / 1000);
	}
#endif
}

BOOL	FriendlyShutdownServer::CanEnterGame() const
{
	const static int nMinuTicks = TICK_PER_SECOND*60;
	if( m_nShutdownTick >= 0 && m_nShutdownTick <= nMinuTicks )
		return FALSE;
	return TRUE;
}

VOID	FriendlyShutdownServer::Update(VOID)
{
	if( m_nShutdownTick < 0 )
	{
		return ;
	}
	CHECK_TIME()

	TryBroadcastShutdownTimes();

	if( --m_nShutdownTick <= 0 )
	{
		m_nShutdownTick = GT_INVALID;
		g_world.ShutDown();
	}
}

VOID	FriendlyShutdownServer::NotifyNewJoinGamePly(Role * ply)
{
	LONG lDelay = m_nShutdownTick;
	if( P_VALID(lDelay) && P_VALID(ply) )
	{
		tagNS_WillShutdownWorld msg;
		msg.nSecondsDelay	= lDelay * TICK_TIME / 1000;
		ply->SendMessage(&msg,msg.dwSize);
	}
}


