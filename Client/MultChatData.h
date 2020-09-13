#pragma once

#include "..\WorldDefine\msg_GroupChat.h"

#pragma pack(push, 1)
#define      MAX_GROUPCHAT_PAGE_SHOW    512
#define      PLAYER_MAX_CREATE_GROUP_NUM  5
#define      GROUP_MAX_MEMBER_NUM		   20
//本地群聊结构
struct tagLocalChatGroup
{
	DWORD							dwGroupChatId;							//群的id
	DWORD							dwGroupChatMasterId;                  //群主id   
	TCHAR							szGroupChatName[MAX_GROUPCHAT_LEN];		//群的名字
	DWORD							dwMemberNum;                         //群成员个数 
	std::list<tagGroupChatMember*>	GroupChatMember;						//群成员的名字
	TCHAR							szChatContent[MAX_GROUPCHAT_PAGE_SHOW];  //最近聊天内容
	tagLocalChatGroup()
	{
		dwGroupChatId = 0;
		dwGroupChatMasterId = 0;
		dwMemberNum = 0;
		ZeroMemory(szGroupChatName,sizeof(szGroupChatName));
		ZeroMemory(szChatContent,sizeof(szChatContent));
	}
};

struct tagGroupChatButtonGlin: public tagGameEvent
{
	tagGroupChatButtonGlin():tagGameEvent(_T("tagGroupChatButtonGlin"),NULL){}
	tagGroupChatButtonGlin(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

#pragma pack(pop)