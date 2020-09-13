#pragma once
#include "..\WorldDefine\chat_define.h"
#pragma pack(push, 1)

struct tagPrivateChatEvent : public tagGameEvent
{
	DWORD	dwRoleID;
	tstring strRoleName;
	tagPrivateChatEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : 
	tagGameEvent(szEventName,pSenderFrame), dwRoleID( GT_INVALID )
	{}
};

struct tagSocialInfoEvent : public tagGameEvent
{
	tstring strInfo;
	tagSocialInfoEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) :
	tagGameEvent(szEventName,pSenderFrame)
	{}
};

struct tagDungeonEvent : public tagGameEvent
{
	tstring strInfo;
	tagDungeonEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) :
	tagGameEvent(szEventName,pSenderFrame)
	{}
};

struct tagChannelSetEvent : public tagGameEvent
{
	INT		nChannel;
	DWORD	nParam;
	tagChannelSetEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) :
	tagGameEvent(szEventName,pSenderFrame)
	{}
};


struct tagOpenMiniChatEvent : public tagGameEvent           //定义双击好友时弹出密语界面的消息结构
{
	tstring name;
	tagOpenMiniChatEvent(LPCTSTR szEventName, GameFrame*pSendFrame):
	tagGameEvent(szEventName,pSendFrame)
	{}
};

struct tagPushSpecInfoEvent : public tagGameEvent
{
	tstring strDesc;
	ESendChatChannel eChannelType;
	tagPushSpecInfoEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
	{}
};
#pragma pack(pop)