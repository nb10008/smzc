//-----------------------------------------------------------------------------
//!\file login_server.h
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief LoginServer程序入口
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "Mutex.h"

//-----------------------------------------------------------------------------
// LoginServer程序入口
//-----------------------------------------------------------------------------
class LoginServer
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	LoginServer();
	~LoginServer();

	//-------------------------------------------------------------------------
	// 初始化，主循环和销毁
	//-------------------------------------------------------------------------
	BOOL			Init(HINSTANCE hInst);
	VOID			Mainloop();
	VOID			Destroy();

	//-------------------------------------------------------------------------
	// 线程函数
	//-------------------------------------------------------------------------
	VOID			ThreadUpdate();

	//-------------------------------------------------------------------------
	// 锁函数
	//-------------------------------------------------------------------------
	VOID			LockUpdate()		{ m_UpdateMutex.Acquire(); }
	VOID			UnlockUpdate()		{ m_UpdateMutex.Release(); }
	VOID            LockGetTokenID()    { m_Lock.Acquire();}
	VOID            UnlockGetTokenID()  { m_Lock.Release();}

	//-------------------------------------------------------------------------
	// 关闭服务器
	//-------------------------------------------------------------------------
	VOID			Shutdown()			{ InterlockedExchange((long*)(&m_bTerminateUpdate), TRUE); }

	//-------------------------------------------------------------------------
	// 得到ID
	//-------------------------------------------------------------------------
	DWORD			GetSectionID()		{ return m_dwID; }

	//-------------------------------------------------------------------------
	// 辅助函数
	//-------------------------------------------------------------------------
	TObjRef<Log>&	GetLog()	{ return m_pLog; }
	TObjRef<Util>&	GetUtil()	{ return m_pUtil; }
	BOOL			IsSimFatigueServer()	{	return m_bSimFatigueServer;		}
	BOOL			IsEnableFatigueGuard()	{	return m_bEnableFatigueGuard;	}
	
	//-------------------------------------------------------------------------
	// 防沉迷系统开关
	//-------------------------------------------------------------------------
	DWORD OpenFatigueNetCmd(DWORD);
	DWORD CloseFatigueNetCmd(DWORD);

    DWORD Broadcast(LPCTSTR);

	TCHAR*  GetstrBroadcastMsg(){return strBroadcastMsg;}
	

private:
	//-------------------------------------------------------------------------
	// 辅助函数
	//-------------------------------------------------------------------------
	VOID			UpdateMemoryUsage();

private:
	TSFPTrunk<LoginServer>	m_Trunk;
	TObjRef<GDIRender>		m_pRender;
	TObjRef<WindowWrap>		m_pWindow;
	TObjRef<GUISystem>		m_pGUI;
	IGUIRenderNative*		m_pGUIRender;
	TObjRef<Thread>			m_pThread;
	TObjRef<Console>		m_pConsole;
	TObjRef<Log>			m_pLog;
	TObjRef<Util>			m_pUtil;

	//--------------------------------------------------------------------------
	// 大区ID
	//--------------------------------------------------------------------------
	DWORD					m_dwID;

	//--------------------------------------------------------------------------
	// 线程相关
	//--------------------------------------------------------------------------
	VOLATILE BOOL			m_bTerminateUpdate;		// 线程停止标志
	Mutex					m_UpdateMutex;			// 更新锁
	Mutex                   m_Lock;                 // 指定YY获得TOKENID锁
	//--------------------------------------------------------------------------
	// 系统信息
	//--------------------------------------------------------------------------
	INT						m_nCpuNum;				// CPU 数量
	DWORD					m_dwTotalPhys;			// 总共物理内存
	DWORD					m_dwAvailPhys;			// 可用物理内存
	DWORD					m_dwTotalRunMinute;		// 服务器运行的时间

	BOOL					m_bSimFatigueServer;	// 模拟防沉迷服务器
	BOOL					m_bEnableFatigueGuard;	// 开启防沉迷

	//--------------------------------------------------------------------------
	// 统计信息
	//--------------------------------------------------------------------------
	volatile DWORD			m_dwIdleTime;			// 空闲时间
	volatile DWORD			m_dwTick;				// 服务器心跳


	TCHAR   strBroadcastMsg[X_LONG_NAME];  
};

extern LoginServer g_login;

#define ILOG	(g_login.GetLog())
#define IUTIL	(g_login.GetUtil())


#ifdef KOREA_LOG

class  SPE_LogException
{	
	TCHAR					    m_szDefaultName[1024];
	tagDWORDTime				m_InitTime;
	TObjRef<Log>				m_pLog;
	void	CreateDir(const TCHAR* Path);
	LPTSTR	CreateLogFileName();

	SPE_LogException();
	SPE_LogException(const SPE_LogException&);
	SPE_LogException& operator=(const SPE_LogException&);
public:
	static SPE_LogException& getInstance();
	BOOL   Init();
	VOID   Update();
	TObjRef<Log>&	GetLog()	{ return m_pLog; }
};

#define SPE_LOG	(SPE_LogException::getInstance().GetLog())
#endif