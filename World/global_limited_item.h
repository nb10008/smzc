//----------------------------------------------------------------------
// class:管理游戏中稀有物品的类
// time: 2011-2
// author: Zhangr
//----------------------------------------------------------------------
#pragma once

#include "../WorldDefine/ItemDefine.h"

#include "att_res.h"
#include "../ServerDefine/item_define.h"
#include "../ServerDefine/msg_item.h"
#include "World.h"

class Role;

struct tagDelLinitedItem
{
	DWORD	dwItemID;
	INT64	n64ID;
};

class GlobalLimitedItemMgr
{
public:
	VOID Destroy();

	VOID Init(tagGlobalLimitedItem* pData, INT32 nNum);
	VOID Update();

	BOOL IsLimitedItemFull(DWORD dwTypeID);

	VOID AddNew2LimitedItemMap(tagGlobalLimitedItem* pNew);
	VOID DelFromLimitedItemMap(DWORD, INT64);

	//回收稀有领域
	VOID RecycleRareArea( DWORD dwRoleID, DWORD dwAreaID );

	//删除数据
	//VOID SendDeleteRareArea2DB(INT64 n64ID, DWORD dwAreaID);

	//更新数据
	VOID SendUpdateRareArea2DB(tagGlobalLimitedItem *pGlobalLimitedItem, INT64 n64ID);

	//新建数据
	//VOID SendCreateRareArea2DB(tagGlobalLimitedItem *pGlobalLimitedItem);

	//获得一个数据
	tagGlobalLimitedItem *GetLimitedItemFromMap( DWORD dwTypeID, INT64 n64Serial );

	tagGlobalLimitedItem *GetLimitedItemFromMapByAreaID( DWORD dwAreaID, INT64 n64Serial );

	//存储所有数据到DB
	VOID	SaveAllData2DB();

private:
	TMap<DWORD, std::vector<tagGlobalLimitedItem*>*>	m_mapLimitedItem;

	DWORD	m_dwCurTime;
	DWORD	m_dwLastTime;
		
};

BOOL inline GlobalLimitedItemMgr::IsLimitedItemFull(DWORD dwTypeID)
{
	std::vector<tagGlobalLimitedItem*>* pTmp = m_mapLimitedItem.Peek(dwTypeID);
	if (P_VALID(pTmp))
	{
		tagItemProto* pProto = g_attRes.GetItemProto(dwTypeID);
		if (P_VALID(pProto) && EISF_GlobalLimitedItem == pProto->eSpecFunc)
		{
			if (pProto->nSpecFuncVal1 <= 0)
				return TRUE;

			if (pTmp->size() >= (DWORD)pProto->nSpecFuncVal1)
				return TRUE;
		}
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

VOID inline GlobalLimitedItemMgr::AddNew2LimitedItemMap(tagGlobalLimitedItem* pNew)
{
	if (!P_VALID(pNew))
	{
		return;
	}

	std::vector<tagGlobalLimitedItem*>* pTmp = m_mapLimitedItem.Peek(pNew->dwID);
	if (P_VALID(pTmp))
	{
		pTmp->push_back(pNew);
	}
	else
	{
		tagItemProto* pProto = g_attRes.GetItemProto(pNew->dwID);
		if(!P_VALID(pProto)) return;

		if(pProto->nSpecFuncVal1<=0)
			return;

		std::vector<tagGlobalLimitedItem*>* pVec = new std::vector<tagGlobalLimitedItem*>;
		if (!P_VALID(pVec)) return;

		pVec->push_back(pNew);
		m_mapLimitedItem.Add(pNew->dwID, pVec);
	}

	tagNDBC_SaveGlobalLimitedItem send;
	memcpy(&(send.data), pNew, sizeof(tagGlobalLimitedItem));
	send.data.dwRecordTime = g_world.GetWorldTime();
	g_dbSession.Send(&send, send.dwSize);
}

//VOID inline GlobalLimitedItemMgr::DelFromLimitedItemMap(DWORD dwTypeID, INT64 n64Serial)
//{
//	std::vector<tagGlobalLimitedItem*>* pTmp = m_mapLimitedItem.Peek(dwTypeID);
//	if (P_VALID(pTmp) && !pTmp->empty())
//	{
//		std::vector<tagGlobalLimitedItem*>::iterator it = pTmp->begin();
//		tagGlobalLimitedItem* pItem = NULL;
//		while(it != pTmp->end())
//		{
//			tagGlobalLimitedItem* p = *it;
//			if(!P_VALID(p))
//			{
//				it++;
//				continue;
//			}
//
//			if(dwTypeID == p->dwID && n64Serial == p->n64ID)
//			{
//				pTmp->erase(it);
//				SAFE_DEL(p);
//
//				//tagNDBC_DelGlobalLimitedItem send;
//				//send.dwID = dwTypeID;
//				//send.n64Serial = n64Serial;
//				//g_dbSession.Send(&send, send.dwSize);
//
//				//tagNDBC_SaveGlobalLimitedItem send1;
//				//g_dbSession.Send(&send1, send1.dwSize);
//
//				tagNDBC_DEL send2;
//				g_dbSession.Send(&send2, send2.dwSize);
//
//
//				break;
//			}
//			else
//			{
//				it++;
//			}
//		}
//	}
//}

extern GlobalLimitedItemMgr g_GlobalLimitedItemMgr;