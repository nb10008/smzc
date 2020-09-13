#pragma once

#pragma pack(push, 1)

/** 进入某区域
*/
struct tagEnterMapAreaEvent : public tagGameEvent
{
	EMapArea	eAreaType;
	DWORD		mapAreaID;

	tagEnterMapAreaEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		, eAreaType(EMA_Null),mapAreaID(GT_INVALID)
	{ } 
};

/** 离开某区域
*/
struct tagLeaveMapAreaEvent : public tagGameEvent
{
	EMapArea	eAreaType;
	DWORD		mapAreaID;

	tagLeaveMapAreaEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		, eAreaType(EMA_Null),mapAreaID(GT_INVALID)
	{ } 
};

/**	进入某触发器
*/
struct tagEnterTriggerEvent : public tagGameEvent
{
	DWORD	triggerID;

	tagEnterTriggerEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
		, triggerID(GT_INVALID)
	{ }
};

/**	进入某个场景提示区域
*/
struct tagEnterPromptEvent : public tagGameEvent
{
	tstring strPromptDesc;
	tagEnterPromptEvent(LPCTSTR szEventName, GameFrame* pSenderFrame)
		: tagGameEvent(szEventName,pSenderFrame)
	{}
};

#pragma pack(pop)