//-----------------------------------------------------------------------------
//!\file msg_shortcut_quickbar.h
//!
//!\date 2010-02-21
//!
//!\brief 服务器与数据库  消息
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push)
#pragma pack(1)
#include "../WorldDefine/msg_quickbar.h"

const int QUICK_BAR_TOTALCOUNT = 10*8 ;

//向数据库存数据
CMD_START(NDBC_SaveShortCut_QuickBar)
	DWORD			dwRoleID;
	tagQuickData	QuickBarData[QUICK_BAR_TOTALCOUNT];
CMD_END;

//向数据库要数据
CMD_START(NDBC_LoadShortCut_QuickBar)
	DWORD						dwRoleID;
CMD_END

//数据库向服务器发数据
CMD_START(NDBS_LoadShortCut_QuickBar) 
	DWORD						dwRoleID;
	tagQuickData	QuickBarData[QUICK_BAR_TOTALCOUNT];
CMD_END;

//如果数据库没有数据,就发此消息通知服务器
CMD_START(NDBS_IsSavedShortCut_QuickBar) 
	DWORD						dwRoleID;
	INT16                       nIsSaved;
CMD_END;
#pragma pack(pop)