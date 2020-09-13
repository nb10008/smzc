#pragma once

#include "mutex.h"
#define RANDOM_TIGER_NUM_X1_1 5
#define RANDOM_TIGER_NUM_X1_2 10

#define RANDOM_TIGER_NUM_X5_1 3
#define RANDOM_TIGER_NUM_X5_2 6

#define RANDOM_TIGER_NUM_X10_1 1
#define RANDOM_TIGER_NUM_X10_2 2

#define RANDOM_TIGER_NUM_X50_1 1
#define RANDOM_TIGER_NUM_X50_2 2

class Role;
class TigerMgr
{
public:
	TigerMgr(void);
	~TigerMgr(void);
	
	int		CreateResult(Role* pRole,INT nType);
	int		CreateResultX1(Role* pRole);
	int		CreateResultX5(Role* pRole);
	int		CreateResultX10(Role* pRole);
	int		CreateResultX50(Role* pRole);
	void		CreateItem(Role* pRole,DWORD dwNum);

	DWORD		CheckRoleTigerCoins(Role* pRole,INT nType);
	void		DeleteTigerCoins(Role* pRole,INT nNum);
	static INT64	GetProfitNum(){return m_dwProfit;}
	static VOID	SetProfitNum(INT64 dwNum)	{ m_dwProfit = dwNum;}
	static VOID	HideProfitNum(){ if( m_dwProfit > 100000)	m_dwProfit = m_dwProfit*95/100;}
	VOID	AddProfit(INT64 lValue);
	VOID	DeleteProfit(INT64 lValue);
	int        GetCurrAddRate(Role* pRole,int nLayer);

	static VOID	ClearTimeMark();

	VOID SendBroadcast(INT nX, DWORD dwRoleID,INT nType);

private:
	static INT64 m_dwProfit;
	Mutex	m_Mutex;
public:
	// 1倍率 1等将
	static bool m_bRanMark_X1_1[RANDOM_TIGER_NUM_X1_1];
	static int m_nRanTime_X1_1[RANDOM_TIGER_NUM_X1_1];
	static int m_nRanTimeMin_X1_1[RANDOM_TIGER_NUM_X1_1];

	// 1倍率 2等将
	static bool m_bRanMark_X1_2[RANDOM_TIGER_NUM_X1_2];
	static int m_nRanTime_X1_2[RANDOM_TIGER_NUM_X1_2];
	static int m_nRanTimeMin_X1_2[RANDOM_TIGER_NUM_X1_2];
	// 5倍率 1等将
	static bool m_bRanMark_X5_1[RANDOM_TIGER_NUM_X5_1];
	static int m_nRanTime_X5_1[RANDOM_TIGER_NUM_X5_1];
	static int m_nRanTimeMin_X5_1[RANDOM_TIGER_NUM_X5_1];
	// 5倍率 2等将
	static bool m_bRanMark_X5_2[RANDOM_TIGER_NUM_X5_2];
	static int m_nRanTime_X5_2[RANDOM_TIGER_NUM_X5_2];
	static int m_nRanTimeMin_X5_2[RANDOM_TIGER_NUM_X5_2];
	// 10倍率 1等将
	static bool m_bRanMark_X10_1[RANDOM_TIGER_NUM_X10_1];
	static int m_nRanTime_X10_1[RANDOM_TIGER_NUM_X10_1];
	static int m_nRanTimeMin_X10_1[RANDOM_TIGER_NUM_X10_1];
	// 10倍率 2等将
	static bool m_bRanMark_X10_2[RANDOM_TIGER_NUM_X10_2];
	static int m_nRanTime_X10_2[RANDOM_TIGER_NUM_X10_2];
	static int m_nRanTimeMin_X10_2[RANDOM_TIGER_NUM_X10_2];
	// 50倍率 1等将
	static bool m_bRanMark_X50_1[RANDOM_TIGER_NUM_X50_1];
	static int m_nRanTime_X50_1[RANDOM_TIGER_NUM_X50_1];
	static int m_nRanTimeMin_X50_1[RANDOM_TIGER_NUM_X50_1];
	// 50倍率 2等将
	static bool m_bRanMark_X50_2[RANDOM_TIGER_NUM_X50_2];
	static int m_nRanTime_X50_2[RANDOM_TIGER_NUM_X50_2];
	static int m_nRanTimeMin_X50_2[RANDOM_TIGER_NUM_X50_2];
};
