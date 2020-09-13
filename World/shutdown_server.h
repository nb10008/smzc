//-----------------------------------------------------------------------------
//!\file shutdown_server.h
//!\author Jason
//!
//!\date 2010-10-8
//! last 
//!
//!\brief 友好关服管理器
//!
//!	Copyright (c) 2004 DACHENG Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#ifndef shutdown_server_h__
#define shutdown_server_h__

class Role ;
/************************************************************************/
/* 每分钟广播给客户端，关服倒计时tick，最后一分钟每5s广播一次           */
/* 最后一分钟，广播时需要弹窗口                                         */
/************************************************************************/
class FriendlyShutdownServer
{
	volatile LONG		m_nShutdownTick;			// 关服倒计时tick
	LONG		m_nLastBroadcastTick;		// 上次广播tick
public:
	FriendlyShutdownServer();
	~FriendlyShutdownServer();

	VOID ShutDownTick(LONG nTicks){ ::InterlockedExchange( (LPLONG)&m_nShutdownTick , nTicks );m_nLastBroadcastTick=GT_INVALID; }
	LONG	ShutDownTick(VOID)	const { return m_nShutdownTick; }

	// 每tick在world::threadupdate里面做
	VOID	Update(VOID);
	BOOL	CanEnterGame() const;

	VOID	NotifyNewJoinGamePly(Role * ply);

private:
	VOID	TryBroadcastShutdownTimes(VOID);
	VOID	BroadcastShutdownTimes(DWORD dwBroadcastID,INT nSeconds);
};

extern FriendlyShutdownServer g_FriendlyShutdownServer;

#endif // shutdown_server_h__


