#include "StdAfx.h"
#include "ItemTimerMgr.h"

ItemTimerMgr ItemTimerMgr::m_SItemTimeMgr;

ItemTimerMgr::ItemTimerMgr(void)
{
}

ItemTimerMgr::~ItemTimerMgr(void)
{
}

ItemTimerMgr* ItemTimerMgr::Inst()
{
	return &m_SItemTimeMgr;
}

VOID ItemTimerMgr::Init()
{
	m_mapItemTimeLimit.clear();
}
VOID ItemTimerMgr::Destroy()
{
	m_mapItemTimeLimit.clear();
}
VOID ItemTimerMgr::Update()
{
	if (m_mapItemTimeLimit.empty())	return;
	
	DWORD itemPassed = Kernel::Inst()->GetDeltaTimeDW();
	for (map<INT64,DWORD>::iterator it = m_mapItemTimeLimit.begin();
		it != m_mapItemTimeLimit.end(); ++it)
	{
		if (0 == it->second)
			continue;

		it->second -= itemPassed;

		if(it->second < 0)
			it->second = 0;
	}
}
DWORD ItemTimerMgr::GetItemRemainTime(INT64 n64ID)
{
	map<INT64,DWORD>::iterator it = m_mapItemTimeLimit.find(n64ID);
	if (it != m_mapItemTimeLimit.end())
	{
		return it->second/1000;		// 返回秒数
	}
	return 0;
}

VOID	ItemTimerMgr::SetItemRemainTime(INT64 N64ID,DWORD dwTick)
{
	map<INT64,DWORD>::iterator it = m_mapItemTimeLimit.find(N64ID);
	if (it != m_mapItemTimeLimit.end())
	{
		it->second = dwTick/TICK_PER_SECOND*1000;
	}
	else	// 没有的话添加新的元素
	{
		m_mapItemTimeLimit.insert(make_pair(N64ID,dwTick/TICK_PER_SECOND*1000));
	}
}