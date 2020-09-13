#pragma once

class ItemTimerMgr
{
	static ItemTimerMgr m_SItemTimeMgr;
	ItemTimerMgr(void);
	~ItemTimerMgr(void);
	ItemTimerMgr(const ItemTimerMgr&){}
	const ItemTimerMgr& operator =(const ItemTimerMgr&){}
public:
	static ItemTimerMgr* Inst();
	VOID Init();
	VOID Destroy();
	VOID Update();
	
	DWORD GetItemRemainTime(INT64 n64ID);		// 返回剩余的秒数
	VOID	SetItemRemainTime(INT64 N64ID,DWORD dwTick);	// 时间为剩余的tick数量
private:
	std::map<INT64,DWORD>	m_mapItemTimeLimit;	// 时间限制物品（目前主要是战斗符文）
};
