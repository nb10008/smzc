/********************************************************************
purpose:	师徒系统游戏事件定义
*********************************************************************/
#pragma once

#pragma pack(push, 1)

// 拜师请求
struct tagGetAquireMasterEvent : public tagGameEvent
{
    DWORD roleID;		// 玩家ID

    tagGetAquireMasterEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame)
        , roleID(GT_INVALID)
    { } 
};

// 收徒请求
struct tagAquireApprenticeEvent : public tagGameEvent
{
    DWORD roleID;		// 玩家ID

    tagAquireApprenticeEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
        : tagGameEvent(szEventName, pSenderFrame)
        , roleID(GT_INVALID)
    { } 
};


// 收徒成功后
struct tagAddStudentEvent : public tagGameEvent
{
	DWORD roleID;		// 玩家ID

	tagAddStudentEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
		, roleID(GT_INVALID)
	{ } 
};

// 拜师成功后
struct tagGetMasterEvent : public tagGameEvent
{
	DWORD roleID;		// 玩家ID

	tagGetMasterEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
		, roleID(GT_INVALID)
	{ } 
};

// 逐出师门
struct tagBanStudentEvent : public tagGameEvent
{
	DWORD roleID;		// 玩家ID

	tagBanStudentEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
		, roleID(GT_INVALID)
	{ } 
};

// 脱离师门
struct tagBaEvent : public tagGameEvent
{
	DWORD roleID;		// 玩家ID

	tagBaEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName, pSenderFrame)
		, roleID(GT_INVALID)
	{ } 
};


#pragma pack(pop)