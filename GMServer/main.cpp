//-----------------------------------------------------------------------------
//!\file	main.cpp
//!\brief	游戏主循环
//!
//!\date	2009-04-20
//! last	2009-04-20
//!\author	zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "server.h"

CLog g_Log;

//----------------------------------------------------------------------------------
// 主循环
//----------------------------------------------------------------------------------
INT main(VOID)
{	
	g_Log.Write(_T("Start to Init Network ...\n"));

	vEngine::InitNetwork();

	g_Log.Write(_T("Init Network Finished ...\n"));
	g_Log.Write(_T("Start to Init Engine ...\n"));

	vEngine::InitEngine();

	g_Log.Write(_T("Init Engine Finished ...\n"));

	if( g_Server.Init() )
	{
		g_Server.MainLoop();
	}
	g_Server.Destroy();

	vEngine::DestroyEngine();
	vEngine::DestroyNetwork();

	return 0;
}

