#pragma once

/** \设置地图标志事件
*/
struct tagSetFlagEvent : public tagGameEvent
{
	float x;
	float z;

	tagSetFlagEvent():tagGameEvent(_T("tagSetFlagEvent"),NULL){}
	tagSetFlagEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

/** \打开/关闭地图窗口事件
*/
struct tagOpenCloseMapWndEvent : public tagGameEvent
{
	bool bWorldMap;

	tagOpenCloseMapWndEvent():tagGameEvent(_T("tagOpenCloseMapWndEvent"),NULL), bWorldMap( false ){}
	tagOpenCloseMapWndEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

/** \打开地图窗口事件
*/
struct tagOpenMapWndEvent : public tagGameEvent
{
	bool bWorldMap;

	tagOpenMapWndEvent():tagGameEvent(_T("tagOpenMapWndEvent"),NULL), bWorldMap( false ){}
	tagOpenMapWndEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

/** \右键打开角色菜单
*/
struct tagPlayerPopupMenu : public tagGameEvent
{
	DWORD		dwRoleID;
	tagPoint	pt;
	tagPlayerPopupMenu():tagGameEvent( _T("tagPlayerPopupMenu"),	NULL),dwRoleID(GT_INVALID){}
	tagPlayerPopupMenu(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent( szEventName, pSenderFrame),dwRoleID(GT_INVALID){}
};

/** \PvP房间右键打开角色菜单
*/
struct tagPvPRoomMenuEvent : public tagGameEvent
{
	DWORD				dwRoleID;
	TCHAR				szName[X_SHORT_NAME];
	tagPoint			pt;
	tagPvPRoomMenuEvent():tagGameEvent(_T("tagPvPRoomMenuEvent"),NULL)
	{}
};
