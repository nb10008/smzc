#pragma once

#include "..\WorldDefine\city_define.h"
struct tagCityAtt2Client;



struct tagCityAttEvent : public tagGameEvent
{
	tagCityAtt2Client *pCityAtt;
	tagCityAttEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) 
		: tagGameEvent(szEventName, pSenderFrame), pCityAtt(NULL) {}
};

struct tagCityWarApplyInfoRefreshEvent : public tagGameEvent
{
	bool bAllNameCanFind;				// 通知事件接受者所有结构里面的帮派名称是否都能找到
	int nGetGuildNameFlag;				// 获取所有帮派名称的标记
	DWORD dwCityID;						// 城市ID
	DWORD dwOwnGuild;					// 城市所属帮派ID
	DWORD dwAvgAckMoney;				// 城战报名平均费用
	tagWarTime warTime;					//城战开始时间
	std::vector<DWORD> vecAttackIDs;	// 攻击方帮派ID队列
	std::vector<DWORD> vecDefenseIDs;	// 防守方帮派ID队列
	std::vector<DWORD> vecConfirmIDs;	// 已经确认
	tagCityWarApplyInfoRefreshEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame) { }
};

struct tagCityWarAssistConfirmRefreshEvent : public tagGameEvent
{
	std::vector<DWORD> vecDefenseIDs;	// 防守方报名ID
	tagCityWarAssistConfirmRefreshEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagCityWarCityRefreshEvent : public tagGameEvent
{
	std::vector<DWORD> vecCitys;
	tagCityWarCityRefreshEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagOpenCityEvent : public tagGameEvent
{
	DWORD dwCityID;
	tagOpenCityEvent(LPCTSTR szEventName, GameFrame *pSenderFrame) : tagGameEvent(szEventName, pSenderFrame){}
};

struct tagOpenCityWarResult: public tagGameEvent
{
	tagOpenCityWarResult(GameFrame *pSenderFrame) : tagGameEvent(_T("tagOpenCityWarResult"), pSenderFrame){}
};

struct tagLeaveCityWarEvent: public tagGameEvent
{
	tagLeaveCityWarEvent(GameFrame *pSenderFrame) : tagGameEvent(_T("tagLeaveCityWarEvent"), pSenderFrame){}
};