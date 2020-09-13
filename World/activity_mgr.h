//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: activity.h
// author: wjiang
// actor:
// data: 2009-03-27
// last:
// brief: 活动管理器
//-----------------------------------------------------------------------------
#pragma once

#include "../ServerDefine/activity_define.h"
#include "db_session.h"
#include "unit.h"

//----------------------------------------------------------------------------------------------------
// 固定活动类
//----------------------------------------------------------------------------------------------------

class ActScript;

struct tagMergedScriptData
{
	BOOL bHaveData;
	ScriptData<ESD_Activity> m_MergedScriptData;		// 合服工具合并过来的scriptdata

	tagMergedScriptData():bHaveData(FALSE){}
	tagMergedScriptData& operator=(tagMergedScriptData& MergedScriptData)
	{
		bHaveData = MergedScriptData.bHaveData;
		m_MergedScriptData = MergedScriptData.m_MergedScriptData;
		return *this;
	}
};

class CActivity : public ScriptData<ESD_Activity>
{
public:
	CActivity();
	~CActivity();

	VOID	Init(const tagActInfo *pActInfo);
	VOID	InitScriptData(tagActivityData* pActivityData);
	BOOL	CanStart(tagDWORDTime cur_time);
	BOOL	IsStartBroad(tagDWORDTime cur_time);
	BOOL	IsEndBroad(tagDWORDTime cur_time);
	BOOL	IsEnd(tagDWORDTime cur_time);
	BOOL	IsStart() { return m_bIsStart; }
	VOID	Start();
	VOID	End();

	DWORD		GetActID() { return m_pActInfo->dwID; }
	EActMode	GetMode()	{return m_pActInfo->eActMode; }

	tagDWORDTime	GetStartTime() { return m_pActInfo->ActTime.startTime; }
	tagDWORDTime	GetEndTime() { return m_pActInfo->ActTime.endTime; }
	DWORD			GetWeekDay() { return m_pActInfo->ActTime.eWeekday; }

	const ActScript* GetScript() { return m_pScript; }
	
	VOID	Update();
	VOID	OnMinute(tagDWORDTime cur_time);
	
	/// 发送神迹的所有信息给玩家
	DWORD	SendAllGodMiracleToPlayer(Unit* pPlayer);
	/// 得到某帮派是否占有神迹
	DWORD	GetGuildCpatureGodMiracleNum(DWORD dwGuild);

	VOID OpenActivity(DWORD dwActivityID);
	VOID CloseActivity(DWORD dwActivityID);

public:
	VOID	AddEventTime(DWORD dwTime);
	VOID	SaveActivityData2DB();

	BOOL IsHaveMergedData() { return m_bHaveMergedData; }
	VOID ResetIsHaveMergedData()
	{
		m_bHaveMergedData = FALSE;
		for (INT nCnt = 0; nCnt < MERGED_SCRIPTDATA_MAX; nCnt++)
		{
			m_MergedData[nCnt].bHaveData = FALSE;
			for (INT nDataIdx = 0; nDataIdx < ESD_Activity; nDataIdx++)
				m_MergedData[nCnt].m_MergedScriptData.SetScriptData(nDataIdx, 0);
		}
	}
	BOOL GetMergedScriptData(INT nIdx, tagMergedScriptData& MergedScriptData);
private:
	VOID    BroadActivityState(EActBroadMode eState);

private:
	const tagActInfo*				m_pActInfo;
	BOOL							m_bIsStart;				// 活动是否开始
	std::vector<tagDWORDTime>		m_vecStartBroadTime;	// 活动开始前的广播时间
	std::vector<tagDWORDTime>		m_vecEndBroadTime;		// 活动结束前的广播时间
	std::set<DWORD>					m_setOnTimeTick;		// 活动中时间触发事件的触发时间列表
	DWORD							m_dwTick;				// 活动进行的时间

	const ActScript*				m_pScript;

	BOOL m_bHaveMergedData;		// 是否有合服工具合并的活动数据
	tagMergedScriptData m_MergedData[MERGED_SCRIPTDATA_MAX];
};


//----------------------------------------------------------------------------------------------------
// 固定活动管理类
//----------------------------------------------------------------------------------------------------
class CActivityMgr
{
public:
	CActivityMgr():m_dwCityStruggleState(1)
	{

	}
	~CActivityMgr();

	BOOL   Init();
	VOID   Update();
	VOID   OnMinute(DWORD cur_time);
	VOID   Destroy();

	CActivity*	GetActivity(DWORD dwID)	{ return m_mapActivity.Peek(dwID); }
	BOOL		HaveActivitStart();
	VOID		SaveAllActivityData2DB();

	DWORD		GetCityStruggleState(){ return m_dwCityStruggleState;}

private:
	BOOL   LoadActivityData();

	VOID OpenActivity(LPCTSTR szActivityID);
	VOID CloseActivity(LPCTSTR szActivityID);

private:
	TSFPTrunk<CActivityMgr> m_Trunk;

	TObjRef<VarContainer>		m_pVar;

	TMap<DWORD,	tagActInfo*>	m_mapActInfo;
	TMap<DWORD, CActivity*>		m_mapActivity;
	DWORD						m_dwCityStruggleState;//1为开启城战，0为关闭城战
};

extern CActivityMgr	g_activityMgr;

















