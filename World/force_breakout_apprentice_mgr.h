//-----------------------------------------------------------------------------
// filename: force_breakout_apprentice_mgr.h
// author: 
// data: 
// last:
// brief: 强制脱离帮派的徒弟管理器
//-----------------------------------------------------------------------------
#pragma once

#include "..\WorldDefine\master_apprentice.h"
#include "..\ServerDefine\master_apprentice.h"
#include "..\WorldDefine\msg_master_apprentice.h"

#define DAY_SECOND_COUNT 86400  // 24x60x60

class CForceBreakOutApprentice
{
public:
	CForceBreakOutApprentice(DWORD dwApprenticeID, DWORD dwMasterID, tagDWORDTime dwordBreakoutTime)
		:m_dwApprenticeID(dwApprenticeID),
		m_dwMasterID(dwMasterID),
		m_BreakoutInsertTime(dwordBreakoutTime)
	{
	}

	~CForceBreakOutApprentice(){}

	// 返回值 TRUE：到时间了  FALSE：还没到时间呢
	BOOL	OnMinute();

	// 执行角色退出师门操作
	VOID	ApprenticeBreakOut();

	// 将数据存入数据库
	VOID	InsertApprenticeData2DB();

	// 删除数据库中的数据
	VOID	RemoveApprenticeFromDB();

	DWORD	GetApprenticeID() { return m_dwApprenticeID; }

	DWORD	GetRemainTime()
	{
		DWORD dwRemainTime = CalcTimeDiff(GetCurrentDWORDTime(), m_BreakoutInsertTime);
		if (dwRemainTime > DAY_SECOND_COUNT)
		{
			dwRemainTime = DAY_SECOND_COUNT;
		}
		// 时间要转换为小时
		dwRemainTime = 24 - ((dwRemainTime)/3600);
		return dwRemainTime;
	}

private:
	DWORD			m_dwApprenticeID;			// 徒弟ID
	DWORD			m_dwMasterID;				// 师傅ID
	tagDWORDTime	m_BreakoutInsertTime;		// 徒弟申请强制脱离师门时候的时间
};

class CForceBreakOutApprenticeMgr
{
public:
	CForceBreakOutApprenticeMgr(){}
	~CForceBreakOutApprenticeMgr();

	VOID	OnMinute();

	// 添加强制脱离师门的徒弟的申请
	VOID	InsertApprentice2List(DWORD dwApprenticeID, DWORD dwMasterID);

	VOID	Init();

	VOID	InitApprenticeList(DWORD dwForceBreakOutApprenceCnt, tagForceBreakOutApprentice *pForceBreakOutApprentice);
	
	BOOL	FindApprentice(DWORD dwApprenticeID, INT& dwRemainTime);

private:
	
	// 载入数据库中存储的强制脱离师门申请列表
	VOID	LoadApprenticeListFromDB();

	// 释放强制脱离师门申请列表
	VOID	DestoryApprenticeList();

	VOID	InsertApprentice2List(DWORD dwApprenticeID, DWORD dwMasterID, tagDWORDTime dwordBreakoutTime, BOOL bNeedSave2DB = TRUE);

private:

	TList<CForceBreakOutApprentice*> m_listForceBreakOutApprentice; // 已经申请了强制脱离师门的徒弟的ID列表
};

extern CForceBreakOutApprenticeMgr g_forceBreakOutApprenticeMgr;