#include "StdAfx.h"
#include "TigerMgr.h"

#include "..\WorldDefine\tiger_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../ServerDefine/att_res_define.h"
#include "..\WorldDefine\msg_tiger.h"

#include "item_creator.h"
#include "world.h"
#include "att_res.h"
#include "role.h"
#include "MsgInfoMgr.h"

INT64 TigerMgr::m_dwProfit = 0;
bool TigerMgr::m_bRanMark_X1_1[RANDOM_TIGER_NUM_X1_1]	= {0};
int TigerMgr::m_nRanTime_X1_1[RANDOM_TIGER_NUM_X1_1] = {0};
int TigerMgr::m_nRanTimeMin_X1_1[RANDOM_TIGER_NUM_X1_1] = {0};

bool TigerMgr::m_bRanMark_X1_2[RANDOM_TIGER_NUM_X1_2]	= {0};
int TigerMgr::m_nRanTime_X1_2[RANDOM_TIGER_NUM_X1_2] = {0};
int TigerMgr::m_nRanTimeMin_X1_2[RANDOM_TIGER_NUM_X1_2] = {0};

bool TigerMgr::m_bRanMark_X5_1[RANDOM_TIGER_NUM_X5_1]	= {0};
int TigerMgr::m_nRanTime_X5_1[RANDOM_TIGER_NUM_X5_1] = {0};
int TigerMgr::m_nRanTimeMin_X5_1[RANDOM_TIGER_NUM_X5_1] = {0};

bool TigerMgr::m_bRanMark_X5_2[RANDOM_TIGER_NUM_X5_2]	= {0};
int TigerMgr::m_nRanTime_X5_2[RANDOM_TIGER_NUM_X5_2] = {0};
int TigerMgr::m_nRanTimeMin_X5_2[RANDOM_TIGER_NUM_X5_2] = {0};

bool TigerMgr::m_bRanMark_X10_1[RANDOM_TIGER_NUM_X10_1]	= {0};
int TigerMgr::m_nRanTime_X10_1[RANDOM_TIGER_NUM_X10_1] = {0};
int TigerMgr::m_nRanTimeMin_X10_1[RANDOM_TIGER_NUM_X10_1] = {0};

bool TigerMgr::m_bRanMark_X10_2[RANDOM_TIGER_NUM_X10_2]	= {0};
int TigerMgr::m_nRanTime_X10_2[RANDOM_TIGER_NUM_X10_2] = {0};
int TigerMgr::m_nRanTimeMin_X10_2[RANDOM_TIGER_NUM_X10_2] = {0};

bool TigerMgr::m_bRanMark_X50_1[RANDOM_TIGER_NUM_X50_1]	= {0};
int TigerMgr::m_nRanTime_X50_1[RANDOM_TIGER_NUM_X50_1] = {0};
int TigerMgr::m_nRanTimeMin_X50_1[RANDOM_TIGER_NUM_X50_1] = {0};

bool TigerMgr::m_bRanMark_X50_2[RANDOM_TIGER_NUM_X50_2]	= {0};
int TigerMgr::m_nRanTime_X50_2[RANDOM_TIGER_NUM_X50_2] = {0};
int TigerMgr::m_nRanTimeMin_X50_2[RANDOM_TIGER_NUM_X50_2] = {0};

TigerMgr::TigerMgr(void)
{

}

TigerMgr::~TigerMgr(void)
{

}

void		TigerMgr::CreateItem(Role* pRole,DWORD dwNum)
{
	DWORD dwNum1 = dwNum;
	while(dwNum1 > 0)
	{
		// 生成物品
		tagItem* pNewItem = ItemCreator::Create(EICM_Tiger, NULL, TIGER_COIN_TYPEID,dwNum1);
		if (!P_VALID(pNewItem))
		{
			IMSG(_T("Tiger Create Item Failed,itemid=%d\r\n"), TIGER_COIN_TYPEID);
			return;
		}

		dwNum1 -= pNewItem->GetNum();
		// 判断背包是否已满，如果已满，放百宝袋，否则放背包
		if (pRole->GetItemMgr().GetBagFreeSize() < 1)
		{
			pRole->GetItemMgr().Add2BaiBao(pNewItem, ELCID_Tiger);
		}
		else
		{
			pRole->GetItemMgr().Add2Bag(pNewItem, ELCID_Tiger, TRUE);
		}
	}
	

	DeleteProfit(dwNum);
}

int TigerMgr::CreateResult(Role* pRole,INT nType)
{
	if (!P_VALID(pRole))
		return 0;

	DeleteTigerCoins(pRole,nType);
	pRole->AddTigerTime();

	if ( m_dwProfit <  LOWEST_PROFIT)
		return 0;

	if (1 == nType)
		return CreateResultX1(pRole);
	else if(5 == nType)
		return CreateResultX5(pRole);
	else if(10 == nType)
		return CreateResultX10(pRole);
	else if(50 == nType)
		return CreateResultX50(pRole);

	return 0;
}	

int		TigerMgr::CreateResultX1(Role* pRole)
{
	bool bAddRare = false;
	if (m_dwProfit > X1_ADD_BASE_NUM)
		bAddRare = true;

	tagTigerProto* pProto = g_attRes.GetTigerProto(1);
	if(!P_VALID(pProto))	return 0;

	INT nSuccessNum = IUTIL->RandomInRange(0,10000);

	tagDWORDTime time = GetCurrentDWORDTime();
	if (m_dwProfit > 200 )
	{
		for (int i = 0;i <RANDOM_TIGER_NUM_X1_2;i++)
		{
			if (m_bRanMark_X1_2[i] && m_nRanTime_X1_2[i] == time.hour  && (m_nRanTimeMin_X1_2[i] <= (time.min +15)  &&   m_nRanTimeMin_X1_2[i] >= time.min  ))
			{
				m_bRanMark_X1_2[i] = false;
				CreateItem(pRole,pProto->dwNum[1]);
				return 2;
			}
		}

		for (int i = 0;i <RANDOM_TIGER_NUM_X1_1;i++)
		{
			if (m_bRanMark_X1_1[i] && m_nRanTime_X1_1[i] == time.hour  && (m_nRanTimeMin_X1_1[i] <= (time.min +15)  &&   m_nRanTimeMin_X1_1[i] >= time.min  ))
			{
				m_bRanMark_X1_1[i] = false;
				CreateItem(pRole,pProto->dwNum[0]);
				return 1;
			}
		}
	}

	int curNum =0;
	for (int i = 0; i< MAX_TIGER_NUM ; i++)
	{
		curNum += pProto->dwRate[i] + GetCurrAddRate(pRole,i);
		if(nSuccessNum < curNum)	
		{
			CreateItem(pRole,pProto->dwNum[i]);
			return i+1;	// 返回奖励的类型 1-7 
		}
	}

	return 0;
}
int		TigerMgr::CreateResultX5(Role* pRole)
{
	bool bAddRare = false;
	if (m_dwProfit > X5_ADD_BASE_NUM)
		bAddRare = true;

	tagTigerProto* pProto = g_attRes.GetTigerProto(5);
	if(!P_VALID(pProto))	return 0;

	INT nSuccessNum = IUTIL->RandomInRange(0,10000);

	tagDWORDTime time = GetCurrentDWORDTime();
	if (m_dwProfit > 2000 )
	{
		for (int i = 0;i <RANDOM_TIGER_NUM_X5_2;i++)
		{
			if (m_bRanMark_X5_2[i] && m_nRanTime_X5_2[i] == time.hour  && (m_nRanTimeMin_X5_2[i] <= (time.min +15)  &&   m_nRanTimeMin_X5_2[i] >= time.min  ))
			{
				m_bRanMark_X5_2[i] = false;
				CreateItem(pRole,pProto->dwNum[1]);
				return 2;
			}
		}

		for (int i = 0;i <RANDOM_TIGER_NUM_X5_1;i++)
		{
			if (m_bRanMark_X5_1[i] && m_nRanTime_X5_1[i] == time.hour  && (m_nRanTimeMin_X5_1[i] <= (time.min +15)  &&   m_nRanTimeMin_X5_1[i] >= time.min  ))
			{
				m_bRanMark_X5_1[i] = false;
				CreateItem(pRole,pProto->dwNum[0]);
				return 1;
			}
		}
	}

	int curNum =0;
	for (int i = 0; i< MAX_TIGER_NUM ; i++)
	{
		curNum += pProto->dwRate[i]+ GetCurrAddRate(pRole,i);
		if(nSuccessNum < curNum)	
		{
			CreateItem(pRole,pProto->dwNum[i]);
			return i+1;	// 返回奖励的类型 1-7 
		}
	}
	return 0;
}
int		TigerMgr::CreateResultX10(Role* pRole)
{
	bool bAddRare = false;
	if (m_dwProfit > X10_ADD_BASE_NUM)
		bAddRare = true;

	tagTigerProto* pProto = g_attRes.GetTigerProto(10);
	if(!P_VALID(pProto))	return 0;

	INT nSuccessNum = IUTIL->RandomInRange(0,10000);

	tagDWORDTime time = GetCurrentDWORDTime();
	if (m_dwProfit > 6000 )
	{
		for (int i = 0;i <RANDOM_TIGER_NUM_X10_2;i++)
		{
			if (m_bRanMark_X10_2[i] && m_nRanTime_X10_2[i] == time.hour  && (m_nRanTimeMin_X10_2[i] <= (time.min +15)  &&   m_nRanTimeMin_X10_2[i] >= time.min  ))
			{
				m_bRanMark_X10_2[i] = false;
				CreateItem(pRole,pProto->dwNum[1]);
				return 2;
			}
		}

		for (int i = 0;i <RANDOM_TIGER_NUM_X10_1;i++)
		{
			if (m_bRanMark_X10_1[i] && m_nRanTime_X10_1[i] == time.hour  && (m_nRanTimeMin_X10_1[i] <= (time.min +15)  &&   m_nRanTimeMin_X10_1[i] >= time.min  ))
			{
				m_bRanMark_X10_1[i] = false;
				CreateItem(pRole,pProto->dwNum[0]);
				return 1;
			}
		}
	}

	int curNum =0;
	for (int i = 0; i< MAX_TIGER_NUM ; i++)
	{
		curNum += pProto->dwRate[i] + GetCurrAddRate(pRole,i);
		if(nSuccessNum < curNum)	
		{
			CreateItem(pRole,pProto->dwNum[i]);
			return i+1;	// 返回奖励的类型 1-7 
		}
	}
	return 0;
}
int		TigerMgr::CreateResultX50(Role* pRole)
{
	bool bAddRare = false;
	if (m_dwProfit > X50_ADD_BASE_NUM)
		bAddRare = true;

	tagTigerProto* pProto = g_attRes.GetTigerProto(50);
	if(!P_VALID(pProto))	return 0;

	INT nSuccessNum = IUTIL->RandomInRange(0,10000);

	tagDWORDTime time = GetCurrentDWORDTime();
	if (m_dwProfit > 100000 )
	{
		for (int i = 0;i <RANDOM_TIGER_NUM_X50_2;i++)
		{
			if (m_bRanMark_X50_2[i] && m_nRanTime_X50_2[i] == time.hour  && (m_nRanTimeMin_X50_2[i] <= (time.min +15)  &&   m_nRanTimeMin_X50_2[i] >= time.min  ))
			{
				m_bRanMark_X50_2[i] = false;
				CreateItem(pRole,pProto->dwNum[1]);
				return 2;
			}
		}

		for (int i = 0;i <RANDOM_TIGER_NUM_X50_1;i++)
		{
			if (m_bRanMark_X50_1[i] && m_nRanTime_X50_1[i] == time.hour  && (m_nRanTimeMin_X50_1[i] <= (time.min +15)  &&   m_nRanTimeMin_X50_1[i] >= time.min  ))
			{
				m_bRanMark_X50_1[i] = false;
				CreateItem(pRole,pProto->dwNum[0]);
				return 1;
			}
		}
	}

	int curNum =0;
	for (int i = 0; i< MAX_TIGER_NUM ; i++)
	{
		curNum += pProto->dwRate[i]+ GetCurrAddRate(pRole,i);
		if(nSuccessNum < curNum)
		{
			CreateItem(pRole,pProto->dwNum[i]);
			return i+1;	// 返回奖励的类型 1-7 
		}
	}
	return 0;
}

DWORD		TigerMgr::CheckRoleTigerCoins(Role* pRole,INT nType)
{
	TList<tagItem*> ItemList;
	if (SearchPack(pRole->GetItemMgr().GetBag(), ItemList, EISF_TigerCoins, tagFindItemSpecFunc()) < nType)
	{
		return E_Tiger_NotEnoughItem;
	}
	return 0;
}

void		TigerMgr::DeleteTigerCoins(Role* pRole,INT nNum)
{
	AddProfit(nNum);

	TList<tagItem*> ItemList;

	INT nHammerCnt = nNum;
	CHECK_SPEC_BEGIN("Tiger::SearchPack",200);
	if (SearchPack(pRole->GetItemMgr().GetBag(), ItemList, EISF_TigerCoins, tagFindItemSpecFunc()) < nHammerCnt)
	{
		return;
	}
	CHECK_SPEC_END;

	CHECK_SPEC_BEGIN("Tiger::RemoveFromContainer",200);
	
	RemoveFromContainer(pRole->GetItemMgr(), pRole->GetItemMgr().GetBag(), ItemList, nHammerCnt, ELCID_Tiger);
	CHECK_SPEC_END;
}

VOID	TigerMgr::AddProfit(INT64 lValue)
{
	m_Mutex.Acquire();
	m_dwProfit += lValue;
	m_Mutex.Release();
}

VOID	TigerMgr::DeleteProfit(INT64 lValue)
{
	m_Mutex.Acquire();
	m_dwProfit -= lValue;
	m_Mutex.Release();
}

VOID	TigerMgr::ClearTimeMark()
{
	for (int i = 0; i< RANDOM_TIGER_NUM_X1_1; i++)
	{
		m_bRanMark_X1_1[i] = true;
		m_nRanTime_X1_1[i] = IUTIL->RandomInRange(0,23);
		m_nRanTimeMin_X1_1[i] = IUTIL->RandomInRange(0,44);
	}

	for (int i = 0; i< RANDOM_TIGER_NUM_X1_2; i++)
	{
		m_bRanMark_X1_2[i] = true;
		m_nRanTime_X1_2[i] = IUTIL->RandomInRange(0,23);
		m_nRanTimeMin_X1_2[i] = IUTIL->RandomInRange(0,44);
	}

	for (int i = 0; i< RANDOM_TIGER_NUM_X5_1; i++)
	{
		m_bRanMark_X5_1[i] = true;
		m_nRanTime_X5_1[i] = IUTIL->RandomInRange(0,23);
		m_nRanTimeMin_X5_1[i] = IUTIL->RandomInRange(0,44);
	}

	for (int i = 0; i< RANDOM_TIGER_NUM_X5_2; i++)
	{
		m_bRanMark_X5_2[i] = true;
		m_nRanTime_X5_2[i] = IUTIL->RandomInRange(0,23);
		m_nRanTimeMin_X5_2[i] = IUTIL->RandomInRange(0,44);
	}

	for (int i = 0; i< RANDOM_TIGER_NUM_X10_1; i++)
	{
		m_bRanMark_X10_1[i] = true;
		m_nRanTime_X10_1[i] = IUTIL->RandomInRange(0,23);
		m_nRanTimeMin_X10_1[i] = IUTIL->RandomInRange(0,44);
	}

	for (int i = 0; i< RANDOM_TIGER_NUM_X10_2; i++)
	{
		m_bRanMark_X10_2[i] = true;
		m_nRanTime_X10_2[i] = IUTIL->RandomInRange(0,23);
		m_nRanTimeMin_X10_2[i] = IUTIL->RandomInRange(0,44);
	}

	for (int i = 0; i< RANDOM_TIGER_NUM_X50_1; i++)
	{
		m_bRanMark_X50_1[i] = true;
		m_nRanTime_X50_1[i] = IUTIL->RandomInRange(0,23);
		m_nRanTimeMin_X50_1[i] = IUTIL->RandomInRange(0,44);
	}

	for (int i = 0; i< RANDOM_TIGER_NUM_X50_2; i++)
	{
		m_bRanMark_X50_2[i] = true;
		m_nRanTime_X50_2[i] = IUTIL->RandomInRange(0,23);
		m_nRanTimeMin_X50_2[i] = IUTIL->RandomInRange(0,44);
	}
}

VOID TigerMgr::SendBroadcast(INT nX, DWORD dwRoleID,INT nType)
{
	DWORD dwBroadcastID = 0;
	if(nX == 1)
	{
		if (nType == 1)
			dwBroadcastID = 300057;
		else if (nType == 5)
			dwBroadcastID = 300059;
		else if (nType == 10)
			dwBroadcastID = 300061;
		else if (nType == 50)
			dwBroadcastID = 300063;
	}
	else if(nX == 2)
	{
		if (nType == 1)
			dwBroadcastID = 300058;
		else if (nType == 5)
			dwBroadcastID = 300060;
		else if (nType == 10)
			dwBroadcastID = 300062;
		else if (nType == 50)
			dwBroadcastID = 300064;
	}
	else if(nX == 3)
	{
		if (nType == 5)
			dwBroadcastID = 300065;
		else if (nType == 10)
			dwBroadcastID = 300066;
		else if (nType == 50)
			dwBroadcastID = 300067;
	}
	else if(nX == 4)
	{
		if (nType == 10)
			dwBroadcastID = 300068;
		else if (nType == 50)
			dwBroadcastID = 300069;
	}
	else if(nX == 5)
	{
		if (nType == 50)
			dwBroadcastID = 300070;
	}
	if (dwBroadcastID == 0 || dwBroadcastID == GT_INVALID)
		return;
	DWORD dwMsgInfoID =	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwBroadcastID);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_RoleID, &dwRoleID);
	g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);
}

int        TigerMgr::GetCurrAddRate(Role* pRole,int nLayer)
{
	if (!P_VALID(pRole))
		return 0;

	int nPlayerTimes = pRole->GetTigerTime();
	if (nPlayerTimes > 65)
	{
		if(nPlayerTimes%5 == 0)
		{
			if (nLayer == 4)
			{
				return 1200;
			}
			else if (nLayer == 5)
			{
				return -1000;
			}
			else if (nLayer == 6)
			{
				return -4000;
			}
		}
		return 0;
	}

	if (nLayer == 4)
	{
		if (nPlayerTimes <= 20)
		{
			return 500;
		}
		else if (nPlayerTimes <= 30)
		{
			return 300;
		}
		else if (nPlayerTimes <= 40)
		{
			return 0;
		}
		else if (nPlayerTimes <= 50)
		{
			return 300;
		}
		else if (nPlayerTimes <= 65)
		{
			return 0;
		}
	}
	else if (nLayer == 5)
	{
		if (nPlayerTimes <= 20)
		{
			return 200;
		}
		else if (nPlayerTimes <= 30)
		{
			return 0;
		}
		else if (nPlayerTimes <= 40)
		{
			return 0;
		}
		else if (nPlayerTimes <= 50)
		{
			return 200;
		}
		else if (nPlayerTimes <= 65)
		{
			return 0;
		}
	}
	else if (nLayer == 6)
	{
		if (nPlayerTimes <= 20)
		{
			return 200;
		}
		else if (nPlayerTimes <= 30)
		{
			return 200;
		}
		else if (nPlayerTimes <= 40)
		{
			return 0;
		}
		else if (nPlayerTimes <= 50)
		{
			return 200;
		}
		else if (nPlayerTimes <= 65)
		{
			return 0;
		}
	}

	return 0;
}