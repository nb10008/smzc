#pragma once
#include "..\WorldDefine\family_define.h"
#pragma pack(push, 1)

//申请加入家族
struct tagFamilyInviteEvent : public tagGameEvent
{
	DWORD	dwRoleID;
	tstring	strRoleName;
	tagFamilyInviteEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), dwRoleID( GT_INVALID )
	{}
};

//邀请加入家族
struct tagFamilyApplyEvent : public tagGameEvent
{
	DWORD	dwRoleID;
	tstring	strRoleName;
	tagFamilyApplyEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), dwRoleID( GT_INVALID )
	{}
};

//家族操作的消息
struct tagFamilyEvent : public tagGameEvent
{
	EFamilyEventType eEventType;
	DWORD			 dwRoleIndex; //操作成员的索引
	tagFamilyEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame),dwRoleIndex(GT_INVALID),eEventType(EFET_NoEvent)
	{}
};

//家族操作的消息
struct tagFamilyFrameEvent : public tagGameEvent
{
	BOOL	bShowFrame;
	tagFamilyFrameEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame),bShowFrame(FALSE)
	{}
};

//家族操作的消息
struct tagFamilyNameEvent : public tagGameEvent
{
	tstring	strFamilyName;
	tagFamilyNameEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame)
	{}
};

#pragma pack(pop)