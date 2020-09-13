//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: Container.cpp
// author: Sxg
// actor:
// data: 2008-06-16
// last:
// brief: 物品容器类
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "Container.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/item_define.h"
#include "../WorldDefine/msg_item.h"
#include "att_res.h"
#include "container_restrict.h"
#include "item_creator.h"
#include "world_session.h"

//****************************************** ItemContainer ***********************************************

//-------------------------------------------------------------------------------------------------------
// 构造函数
//-------------------------------------------------------------------------------------------------------
ItemContainer::ItemContainer(EItemConType eConType,  INT16 n16CurSize, INT16 n16MaxSize, 
							 DWORD dwOwnerID, DWORD dwAccountID, ContainerRestrict *pRestrict/*=NULL*/)
							 : Container(eConType, n16CurSize, n16MaxSize), m_TimeLimitMgr(ITEM_UPDATE_TIME)
{
	m_n16MinFreeIndex = 0;
	m_nZhanDouFuItemCnt = 0;

	m_dwOwnerID = dwOwnerID;
	m_dwAccountID = dwAccountID;

	m_pRestrict = (pRestrict != NULL ? pRestrict : new ContainerRestrict);
	m_pRestrict->init(this);
}

//-------------------------------------------------------------------------------------------------------
// 析构函数
//-------------------------------------------------------------------------------------------------------
ItemContainer::~ItemContainer()
{
	m_n16MinFreeIndex = GT_INVALID;

	if(P_VALID(m_pRestrict))
		SAFE_DEL(m_pRestrict);
}

//-------------------------------------------------------------------------------------------------------
// 向容器中添加新物品,但不指定添加位置,返回错误码
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Add(tagItem* pItem, OUT INT16 &n16Index, OUT tagItemMove &ItemMove, 
						 BOOL bCheckAdd/* = TRUE*/, BOOL bChangeOwner/* = TRUE*/)
{
	ASSERT(pItem != NULL);

	if(pItem->GetNum() <= 0)
	{
		return GT_INVALID;
	}

	if(bCheckAdd && !m_pRestrict->CanAdd(pItem))
	{
		return E_Item_CanNotAdd;
	}

	// 初始化传出参数
	n16Index = GT_INVALID;
	ItemMove.pItem2	= NULL;
	ItemMove.n16NumRes1	= 0;
	ItemMove.n16NumRes2	= 0;
	ItemMove.bCreateItem = FALSE;
	ItemMove.bOverlap = FALSE;
	ItemMove.bChangePos = TRUE;

	INT16 n16Add = 0;
	
	if(pItem->pProtoType->n16MaxLapNum > 1)	// 可堆叠物品
	{
		TList<tagItem*> itemList;
		GetSameItemList(itemList, pItem->dwTypeID);

		tagItem *pItemInCon;
		INT16 n16CanLap;

		// 和同类物品放在一起
		itemList.ResetIterator();
		while(itemList.PeekNext(pItemInCon))
		{
			// 已有的同类物品是被锁定的，则不可堆叠存放
			if( (pItem->bLock && !pItemInCon->bLock)  ||  (pItemInCon->bLock && ( !pItem->bLock || pItem->dwOwnerID != pItemInCon->dwOwnerID ) ))
			{
				continue;
			}

			n16CanLap = pItemInCon->pProtoType->n16MaxLapNum - pItemInCon->GetNum();
			if(n16CanLap >= pItem->GetNum())
			{
				if(pItem->dwOwnerID != pItemInCon->dwOwnerID
					&& pItem->IsBind())
				{
					continue;
				}
				
				if (pItem->IsTimeLimit() || pItemInCon->IsTimeLimit())
				{
					continue;
				}

				n16Add = pItem->GetNum();
				pItemInCon->AddNum(n16Add);

				// 设置传出参数
				ItemMove.pItem2	= pItemInCon;
				ItemMove.n16NumRes1	= 0;
				ItemMove.n16NumRes2	= pItemInCon->GetNum();
				ItemMove.bOverlap	= TRUE;

				n16Index = pItemInCon->n16Index;

				// 设置更新数据信息位
				pItemInCon->SetUpdate(EUDBS_Update);

				break;
			}
		}
	}
	
	// 确定成功之后再检查
	if (n16Add > 0 &&
		P_VALID(pItem->pProtoType) &&
		pItem->pProtoType->eSpecFunc == EISF_ZhanDouFu)
	{
		m_nZhanDouFuItemCnt = GetSameFuncSpecItemCount(EISF_ZhanDouFu);
	}

	// 同类物品中放不下,则放入空位
	if(0 == n16Add)
	{		
		// 设置传出参数
		ItemMove.n16NumRes1	= pItem->GetNum();
		n16Index = GetOneFreeSpace();

		if(GT_INVALID == n16Index)
		{
			IMSG(_T("Container is full!<roleid:%u, eConType:%d>"), m_dwOwnerID, GetConType());
			return E_Con_NotEnoughSpace;
		}
		
		return this->Add(pItem, n16Index, bChangeOwner, FALSE);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 向容器中指定位置添加新物品,返回添加成功物品个数(指定位置必须为空)
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Add(tagItem* pItem, INT16 n16NewIndex, BOOL bChangeOwner/* = TRUE*/, BOOL bCheckAdd/* = TRUE*/)
{
	// 新位置合法性判断
	//if(n16NewIndex < 0 || n16NewIndex >= GetCurSpaceSize())
	//{
	//	ASSERT(n16NewIndex >=0 && n16NewIndex < GetCurSpaceSize());
	//	return GT_INVALID;
	//}
	
	// 是否可放入容器
	if(bCheckAdd && !m_pRestrict->CanAdd(pItem))
	{
		return E_Item_CanNotAdd;
	}
	
	// 待放入位置是否合法
	if(!IsOnePlaceFree(n16NewIndex))
	{
		return E_Item_Place_NotFree;
	}

	// 判断物品个数合法性
	if(pItem->GetNum() <= 0)
	{
		return GT_INVALID;
	}

	INT16 n16Add = Container::Add(pItem, n16NewIndex);
	if(0 == n16Add)
	{
		return E_Item_AddFailed;
	}
	
	ASSERT(n16Add == pItem->GetNum());
	if(bChangeOwner && !pItem->IsBind())
	{
		pItem->SetOwner(m_dwOwnerID, m_dwAccountID);
	}

	// 是否为时限物品
	if( pItem->IsTimeLimit() &&	GetConType() != EICT_Baibao)
	{
		m_TimeLimitMgr.Add2TimeLeftList(pItem->n64Serial, pItem->pProtoType->dwTimeLimit, pItem->dw1stGainTime);
	}

	//// 设置更新数据信息位
	//pItem->SetUpdate(EUDBS_Update);

	// 确定成功之后再检查
	if (P_VALID(pItem->pProtoType) &&
		pItem->pProtoType->eSpecFunc == EISF_ZhanDouFu)
	{
		m_nZhanDouFuItemCnt = GetSameFuncSpecItemCount(EISF_ZhanDouFu);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 从容器中删除指定物品,返回错误码(该处与基类的定义返回值不同)
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Remove(INT64 n64Serial, BOOL bChangeOwner/* = FALSE*/, BOOL bCheckRemove/* = TRUE*/)
{
	tagItem *pItem = GetItem(n64Serial);
	if(NULL == pItem)
	{
		return E_Item_NotFound;
	}

	if(bCheckRemove && !m_pRestrict->CanRemove(pItem))
	{
		return E_Item_CanNotRemove;
	}

	BOOL bZhanDouFu = FALSE;
	if (P_VALID(pItem->pProtoType) &&
		pItem->pProtoType->eSpecFunc == EISF_ZhanDouFu)
	{
		bZhanDouFu = TRUE;
	}

	INT16 n16OldIndex = pItem->n16Index;
	Container::Remove(n64Serial);

	if(bChangeOwner)
	{
		pItem->SetOwner(GT_INVALID, GT_INVALID);
	}

	// 重置m_n16MinFreeIndex
	if(n16OldIndex < m_n16MinFreeIndex)
	{
		m_n16MinFreeIndex = n16OldIndex;
	}

	// 是否为时限物品
	if(pItem->IsTimeLimit())
	{
		m_TimeLimitMgr.RemoveFromTimeLeftList(pItem->n64Serial);
	}

	if (bZhanDouFu)
	{
		m_nZhanDouFuItemCnt = GetSameFuncSpecItemCount(EISF_ZhanDouFu);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 从容器中删除指定个数的指定物品
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::Remove(INT64 n64Serial, INT16 n16Num, BOOL bCheckRemove/* = TRUE*/)
{
	if(n16Num <= 0)
	{
		ASSERT(n16Num > 0);
		return GT_INVALID;
	}

	tagItem *pItem = GetItem(n64Serial);
	if(NULL == pItem)
	{
		return E_Item_NotFound;
	}

	if(bCheckRemove && !m_pRestrict->CanRemove(pItem))
	{
		return E_Item_CanNotRemove;
	}

	BOOL bZhanDouFu = FALSE;
	if (P_VALID(pItem->pProtoType) &&
		pItem->pProtoType->eSpecFunc == EISF_ZhanDouFu)
	{
		bZhanDouFu = TRUE;
	}

	// 不够
	if(pItem->GetNum() < n16Num)
	{
		// 删除失败,发送监控信息 //??

		ASSERT(pItem->GetNum() >= n16Num);
		return GT_INVALID;
	}

	// 刚好
	if(pItem->GetNum() == n16Num)
	{
		// 执行到该处，会有内存泄漏 -- 外层应调用别的接口
		ASSERT(0);
		return this->Remove(n64Serial, TRUE, FALSE);
	}

	// 有富余
	pItem->AddNum(-n16Num);

	// 保存
	pItem->SetUpdate(EUDBS_Update);

	// 确定成功之后再检查
	if (bZhanDouFu)
	{
		m_nZhanDouFuItemCnt = GetSameFuncSpecItemCount(EISF_ZhanDouFu);
	}

	return E_Success;
}

////-------------------------------------------------------------------------------------------------------
//// 从容器中删除某类物品，返回成功个数
////-------------------------------------------------------------------------------------------------------
//INT32 ItemContainer::Remove(DWORD dwTypeID, BOOL bCheckRemove/* = TRUE*/)
//{
//	INT32 nNumDel = 0;
//
//	TList<tagItem*> itemList;
//	GetSameItemList(itemList, dwTypeID);
//
//	tagItem *pItem;
//	itemList.ResetIterator();
//	while(itemList.PeekNext(pItem))
//	{		
//		nNumDel += this->Remove(pItem->n64Serial, TRUE, bCheckRemove);
//	}
//
//	return nNumDel;
//}
//
////-------------------------------------------------------------------------------------------------------
//// 从容器中删除指定个数的某类物品，返回成功个数
////-------------------------------------------------------------------------------------------------------
//INT32 ItemContainer::Remove(DWORD dwTypeID, INT32 nNum, BOOL bCheckRemove/* = TRUE*/)
//{
//	if(nNum <= 0)
//	{
//		ASSERT(nNum > 0);
//		return 0;
//	}
//	
//	TList<tagItem*> itemList;
//	INT32 nNumTotal = GetSameItemList(itemList, dwTypeID);
//
//	// 不够
//	if(nNumTotal < nNum)
//	{
//		// 删除失败,发送监控信息
//
//		ASSERT(nNumTotal >= nNum);
//		return 0;
//	}
//
//	// 刚好, 或富余
//	INT16 n16NumDel = 0;
//	INT32 nNumNeedDel = nNum;
//	tagItem *pItem = NULL;
//	itemList.ResetIterator();
//	while(itemList.PeekNext(pItem) && nNumNeedDel != 0)
//	{
//		if(bCheckRemove && !m_pRestrict->CanRemove(pItem))
//		{
//			ASSERT(0 == n16NumDel);	
//			continue;
//		}
//
//		if(pItem->n16Num <= nNumNeedDel)
//		{
//			n16NumDel = this->Remove(pItem->n64Serial, TRUE, FALSE);
//			ASSERT(n16NumDel == pItem->n16Num);
//			nNumNeedDel -= n16NumDel;
//		}
//		else
//		{
//			pItem->n16Num -= nNumNeedDel;
//			nNumNeedDel = 0;
//
//			// 数据库保存状态
//		}
//	}
//
//	return nNum - nNumNeedDel;
//}

//-------------------------------------------------------------------------------------------------------
// 将物品移动到指定位置
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64Serial1, INT16 n16Index2, OUT tagItemMove &ItemMove)
{
	// 0a.目标位置合法性检查
	if(n16Index2 < 0 || n16Index2 >= GetCurSpaceSize())
	{
		return GT_INVALID;
	}

	tagItem *pItem1, *pItem2;

	// 0b.检查移动目标
	pItem1 = GetItem(n64Serial1);
	if(NULL == pItem1 || pItem1->n16Index == n16Index2)
	{
		ASSERT(pItem1 != NULL);
		ASSERT(pItem1->n16Index != n16Index2);
		return GT_INVALID;
	}

	// 初始化传出参数
	ItemMove.pItem2			= NULL;
	ItemMove.n16NumRes1		= pItem1->GetNum();
	ItemMove.n16NumRes2		= 0;
	ItemMove.bCreateItem	= FALSE;
	ItemMove.bOverlap		= FALSE;
	ItemMove.bChangePos		= TRUE;

	// 1。目标位置为空
	if(IsOnePlaceFree(n16Index2))
	{
		DWORD dwErrorCode = this->Remove(n64Serial1, FALSE,FALSE);
		if(dwErrorCode != E_Success)
		{
			return dwErrorCode;
		}

		return this->Add(pItem1, n16Index2, FALSE,FALSE);
	}

	// 2。目标位置不为空
	pItem2 = GetItem(n16Index2);
	if(!P_VALID(pItem2))
	{
		ASSERT(P_VALID(pItem2));
		return GT_INVALID;
	}
	
	// 得到目标位置物品信息
	ItemMove.pItem2 = pItem2;
	ItemMove.n16NumRes2 = pItem2->GetNum();

	// 如果目标位置物品的绑定状态和要放在该位置上的物品的绑定状态不同，则不能放入
	if ((pItem2->bLock^pItem1->bLock) || 
		(pItem1->bLock && pItem2->bLock && (pItem1->dwUnlockTime!=pItem2->dwUnlockTime)) )
	{
		return GT_INVALID;
	}

	// 2a.物品类性(TypeID || 绑定物品OwnerID)不同,或某一堆物品个数达到了堆叠上限
	if(pItem1->dwTypeID != pItem2->dwTypeID 
		|| (pItem1->dwOwnerID != pItem2->dwOwnerID && (pItem1->IsBind() || pItem2->IsBind()))
		|| pItem1->GetNum() >= pItem1->pProtoType->n16MaxLapNum
		|| pItem2->GetNum() >= pItem2->pProtoType->n16MaxLapNum)
	{
		Swap(pItem1->n16Index, pItem2->n16Index);

		return E_Success;
	}

	// 2b.同种类物品，且两堆均没有达到堆叠上限
	INT32 nNum = pItem1->pProtoType->n16MaxLapNum - pItem2->GetNum();
	nNum = min(nNum, pItem1->GetNum());
	pItem1->AddNum(-nNum);
	pItem2->AddNum(nNum);

	// 重新设置物品信息
	ItemMove.bOverlap	= TRUE;
	ItemMove.bChangePos	= FALSE;
	ItemMove.n16NumRes1 = pItem1->GetNum();
	ItemMove.n16NumRes2 = pItem2->GetNum();

	// 设置数据库保存状态
	pItem1->SetUpdate(EUDBS_Update);
	pItem2->SetUpdate(EUDBS_Update);

	// 物品1全部移动到物品2里 -- 需外部检查，并回收内存
	if(0 == pItem1->GetNum())
	{
		this->Remove(pItem1->n64Serial, TRUE, FALSE);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 将指定个数物品移动到指定位置,返回移动成功的个数
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64Serial1, INT16 n16NumMove, INT16 n16Index2, OUT tagItemMove &ItemMove)
{	
	if(n16NumMove <= 0)
	{
		ASSERT(n16NumMove > 0);
		return GT_INVALID;
	}
	
	tagItem *pItem1 = GetItem(n64Serial1);
	if(NULL == pItem1)
	{
		ASSERT(pItem1 != NULL);
		return E_Item_NotFound;
	}

	if(pItem1->GetNum() <= n16NumMove)
	{
		// 全部移动
		return this->MoveTo(n64Serial1, n16Index2, ItemMove);
	}

	// 初始化传出参数
	ItemMove.pItem2			= NULL;
	ItemMove.n16NumRes1		= pItem1->GetNum();
	ItemMove.n16NumRes2		= 0;
	ItemMove.bCreateItem	= FALSE;
	ItemMove.bOverlap		= FALSE;
	ItemMove.bChangePos		= FALSE;

	// 0.目标位置合法性检查
	if(n16Index2 < 0 || n16Index2 >= GetCurSpaceSize())
	{
		return GT_INVALID;
	}

	// 1。目标位置为空
	if(IsOnePlaceFree(n16Index2))
	{
		if (!P_VALID(pItem1->pProtoType))
			return E_Item_NotFound;
		if(n16NumMove > pItem1->pProtoType->n16MaxLapNum)
			return E_Item_CanNotAdd;

		pItem1->AddNum(-n16NumMove);

		// 设置数据库保存状态
		pItem1->SetUpdate(EUDBS_Update);

		// 重新设置物品信息
		ItemMove.n16NumRes1 = pItem1->GetNum();

		// 生成新的堆叠物品
		tagItem *pNewItem = ItemCreator::Create(*pItem1, n16NumMove);
		if (!P_VALID(pNewItem))
		{
			pItem1->AddNum(n16NumMove);
			pItem1->SetUpdate(EUDBS_Null);
			ItemMove.n16NumRes1	= pItem1->GetNum();
			return E_Item_CanNotAdd;
		}
		// 得到目标位置物品信息
		ItemMove.pItem2 = pNewItem;
		ItemMove.n16NumRes2 = pNewItem->GetNum();
		ItemMove.bCreateItem = TRUE;

		return this->Add(pNewItem, n16Index2, FALSE, FALSE);
	}

	// 2。目标位置不为空
	if(pItem1->n16Index == n16Index2)
	{
		ASSERT(pItem1->n16Index != n16Index2);
		return GT_INVALID;
	}

	// 该物品必存在，否则，不会执行到此处
	tagItem *pItem2 = GetItem(n16Index2);

	// 得到目标位置物品信息
	ItemMove.pItem2 = pItem2;
	ItemMove.n16NumRes2 = pItem2->GetNum();
	
	// 如果目标位置物品的绑定状态和要放在该位置上的物品的绑定状态不同，则不能放入
	if ((pItem2->bLock^pItem1->bLock) || 
		(pItem1->bLock && pItem2->bLock && (pItem1->dwUnlockTime!=pItem2->dwUnlockTime)) )
	{
		return GT_INVALID;
	}

	// 2a.物品类性不同,或移动后超过堆叠上限
	if(pItem1->dwTypeID != pItem2->dwTypeID
		|| (pItem1->dwOwnerID != pItem2->dwOwnerID && (pItem1->IsBind() || pItem2->IsBind()))
		|| pItem2->GetNum() + n16NumMove > pItem2->pProtoType->n16MaxLapNum
		|| pItem1->IsTimeLimit() || pItem2->IsTimeLimit())
	{
		return GT_INVALID;
	}

	// 2b.同种类物品，且移动后目标物品也不会达到堆叠上限
	pItem1->AddNum(-n16NumMove);
	pItem2->AddNum(n16NumMove);

	// 重新设置物品信息
	ItemMove.n16NumRes1 = pItem1->GetNum();
	ItemMove.n16NumRes2 = pItem2->GetNum();
	ItemMove.bOverlap	= TRUE;

	// 设置数据库保存状态
	pItem1->SetUpdate(EUDBS_Update);
	pItem2->SetUpdate(EUDBS_Update);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 将物品移动到其他容器中(仓库和背包间)，不指定目标容器中具体位置
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64SerialSrc, ItemContainer &conDst, 
							OUT tagItemMove &ItemMove, OUT INT16 &n16IndexDst)
{
	// 检查目标容器中是否有空间
	if(conDst.GetFreeSpaceSize() < 1)
	{
		return GT_INVALID;
	}
	
	// 从原容器中取出待移动物品
	tagItem *pItem = GetItem(n64SerialSrc);
	if(NULL == pItem)
	{
		// 网络卡时,客户端发重复的消息,可能执行到此
		ASSERT(pItem != NULL);
		return GT_INVALID;
	}

	if(!m_pRestrict->CanMoveToOther(pItem, &conDst))
	{
		return E_Item_CanNotMove_Other;
	}

	this->Remove(n64SerialSrc, FALSE, FALSE);
	
	// 放入目标容器中
	return conDst.Add(pItem, n16IndexDst, ItemMove, FALSE, TRUE);
}

//-------------------------------------------------------------------------------------------------------
// 将物品移动到其他容器中(仓库和背包间)，指定目标背包中具体位置
//-------------------------------------------------------------------------------------------------------
DWORD ItemContainer::MoveTo(INT64 n64SerialSrc, ItemContainer &conDst, INT16 n16IndexDst, OUT tagItemMove &ItemMove)
{
	// 0.目标位置合法性检查
	if(n16IndexDst < 0 || n16IndexDst >= conDst.GetCurSpaceSize())
	{
		return GT_INVALID;
	}

	tagItem *pItem1, *pItem2;

	// 获得待移动物品指针
	pItem1 = GetItem(n64SerialSrc);
	if(NULL == pItem1)
	{
		// 网络卡时,客户端发重复的消息,可能执行到此		
		ASSERT(pItem1 != NULL);
		return GT_INVALID;
	}

	// 初始化传出参数
	ItemMove.pItem2 = NULL;
	ItemMove.n16NumRes2 = 0;
	ItemMove.n16NumRes1 = pItem1->GetNum();
	ItemMove.bCreateItem = FALSE;
	ItemMove.bOverlap = FALSE;
	ItemMove.bChangePos = TRUE;

	if(!m_pRestrict->CanMoveToOther(pItem1, &conDst))
	{
		return E_Item_CanNotMove_Other;
	}

	// 1。目标位置为空
	if(conDst.IsOnePlaceFree(n16IndexDst))
	{		
		this->Remove(n64SerialSrc);
		return conDst.Add(pItem1, n16IndexDst, TRUE);
	}

	// 2。目标位置不为空
	pItem2 = conDst.GetItem(n16IndexDst);

	if( !P_VALID(pItem2) )
		return GT_INVALID;

	if( (P_VALID(pItem2->dwOwnerID) && P_VALID(pItem1->dwOwnerID)) && (pItem2->dwOwnerID != pItem1->dwOwnerID) )
		return E_Item_CanNotMove_Other;
	if( !(pItem1->IsBind() && pItem2->IsBind() ) )
		return E_Item_CanNotMove_Other;
	//if( P_VALID(pItem2->dwOwnerID) && P_VALID(pItem1->dwOwnerID) && pItem2->dwOwnerID != pItem1->dwOwnerID && ( pItem1->IsBind() || pItem2->IsBind() ) )
	//	return E_Item_CanNotMove_Other;

	ItemMove.pItem2 = pItem2;
	ItemMove.n16NumRes2 = pItem2->GetNum();

	// 2a.物品类性不同,或某一堆物品个数达到了堆叠上限
	if(pItem1->dwTypeID != pItem2->dwTypeID
		|| (pItem1->dwOwnerID != pItem2->dwOwnerID && (pItem1->IsBind() || pItem2->IsBind()))
		|| pItem1->GetNum() >= pItem1->pProtoType->n16MaxLapNum
		|| pItem2->GetNum() >= pItem2->pProtoType->n16MaxLapNum)
	{
		if(!/*conDst.*/GetRestrict()->CanMoveToOther(pItem2, /*this*/&conDst))
		{
			return E_Item_CanNotMove_Other;
		}
		
		// 记录原栏位号
		INT16 n16IndexSrc = pItem1->n16Index;

		// 从容器中清除物品
		Container::Remove(n16IndexSrc);
		((Container *)&conDst)->Remove(n16IndexDst);

		// 放回
		this->Add(pItem2, n16IndexSrc, TRUE);
		conDst.Add(pItem1, n16IndexDst, TRUE);

		return E_Success;
	}

	// 2b.同种类物品，且两堆均没有达到堆叠上限
	INT32 nNum = pItem2->pProtoType->n16MaxLapNum - pItem2->GetNum();
	nNum = min(nNum, pItem1->GetNum());
	pItem1->AddNum(-nNum);
	pItem2->AddNum(nNum);

	ItemMove.n16NumRes1 = pItem1->GetNum();
	ItemMove.n16NumRes2 = pItem2->GetNum();
	ItemMove.bOverlap	= TRUE;
	ItemMove.bChangePos = FALSE;

	// 设置数据库保存状态
	pItem1->SetUpdate(EUDBS_Update);
	pItem2->SetUpdate(EUDBS_Update);

	// 物品1全部移动到物品2里
	if(0 == pItem1->GetNum())
	{
		this->Remove(pItem1->n64Serial, TRUE);
	}

	return E_Success;
}

//****************************************** EquipContainer ***********************************************

//-------------------------------------------------------------------------------------------------------
// 构造函数
//-------------------------------------------------------------------------------------------------------
EquipContainer::EquipContainer(EItemConType eConType,  INT16 n16CurSize, INT16 n16MaxSize)
								: Container(eConType, n16CurSize, n16MaxSize), m_TimeLimitMgr(ITEM_UPDATE_TIME)
{
}

//-------------------------------------------------------------------------------------------------------
// 析构函数
//-------------------------------------------------------------------------------------------------------
EquipContainer::~EquipContainer()
{
}

//-------------------------------------------------------------------------------------------------------
// 放入容器中
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Add(tagEquip *pEquip, EEquipPos ePos)
{
	// 检查空位
	if(!IsOnePlaceFree(ePos))
	{
		return E_Item_Place_NotFree;
	}
	
	// 为了避免可能出现的错位，对时装的位置进行检查
	if (ePos >= EEP_Equip_End)
	{
		if(P_VALID(pEquip->pEquipProto) && pEquip->pEquipProto->eEquipPos != ePos)
		{
			return E_Item_EquipBar_Error;
		}
	}

	// 放入容器中
	if(Container::Add(pEquip, ePos) < 1)
	{
		return E_Equip_OnFailed;
	}

	//// 设置数据库保存状态
	//pEquip->SetUpdate(EUDBS_Update);

	// 是否为时限物品
	if(pEquip->IsTimeLimit())
	{
		m_TimeLimitMgr.Add2TimeLeftList(pEquip->n64Serial, pEquip->pProtoType->dwTimeLimit, pEquip->dw1stGainTime);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 移动(仅限两个戒指位)
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::MoveTo(INT64 n64SerialSrc, EEquipPos ePosDst)
{
	// 检查目标位置是否为戒指位
	if(ePosDst != EEP_Finger1 && ePosDst != EEP_Finger2)
	{
		return E_Equip_NotRingPos;
	}

	// 获取待移动装备
	tagEquip *pEquip = GetItem(n64SerialSrc);
	if(NULL == pEquip)
	{
		ASSERT(pEquip != NULL);
		return GT_INVALID;
	}

	// 待移动装备是否为ring
	if(!MIsRing(pEquip))
	{
		return E_Equip_NotRing;
	}

	// 是否已在目标位置
	if(pEquip->n16Index == ePosDst)
	{
		return E_Equip_SamePos;
	}

	// 移动
	if(IsOnePlaceFree(ePosDst))	// 目标位置为空
	{
		Container::Remove(n64SerialSrc);
		return this->Add(pEquip, ePosDst);
	}
	else // 目标位置有装备(ring)
	{
		Container::Swap(pEquip->n16Index, ePosDst);
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 穿上装备
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Equip(ItemContainer &bagSrc, INT64 n64SerialSrc, EEquipPos ePosDst)
{
	// 取得待物品指针
	tagItem *pItemNew = bagSrc.GetItem(n64SerialSrc);
	if(NULL == pItemNew)
	{
		// 网络卡时,客户端发重复的消息,可能执行到此
		ASSERT(pItemNew != NULL);
		return E_Item_NotFound;
	}

	// 判断是否为装备
	if(!MIsEquipment(pItemNew->dwTypeID))
	{
		return E_Item_NotEquipment;
	}

	tagEquip *pEquipNew = (tagEquip *)pItemNew;

	// 得到可装备位置
	EEquipPos ePos = (pEquipNew->pEquipProto)->eEquipPos;

	// 检查是否可以装备到目标位置(ring要做特殊判断)
	// if(!(ePos == ePosDst || (ePos + 1 == ePosDst && MIsRing(pEquipNew))))
	if(ePos != ePosDst && (ePos + 1 != ePosDst || (!MIsRing(pEquipNew) && !MIsWeapon(pEquipNew))))
	{
		return E_Equip_InvalidPos;
	}


	INT16 n16IndexBag = pEquipNew->n16Index;

	// 目标位不为空
	if(!IsOnePlaceFree(ePosDst))
	{
		// 从背包中取出装备
		((Container *)&bagSrc)->Remove(n16IndexBag);
		// 从装备栏中取下装备
		tagEquip *pEquipOld = Container::Remove((INT16)ePosDst);
		// 将原装备栏中的装备放入背包中
		bagSrc.Add(pEquipOld, n16IndexBag, FALSE);
	}
	else
	{
		// 从背包中取出装备
		bagSrc.Remove(n64SerialSrc);
	}

	// 将原背包中物品装备上
	return this->Add(pEquipNew, ePosDst);
}

//-------------------------------------------------------------------------------------------------------
// 脱下装备
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Unequip(INT64 n64SerialSrc, ItemContainer &bagDst)
{
	//// 检查背包中是否有空间
	//if(bagDst.GetFreeSpaceSize() < 1)
	//{
	//	return E_Bag_NotEnoughSpace;
	//}

	//// 脱下装备
	//tagEquip *pEquipSrc = Container::Remove(n64SerialSrc);
	//if(NULL == pEquipSrc)
	//{
	//	// 网络卡时,客户端发重复的消息,可能执行到此
	//	ASSERT(pEquipSrc != NULL);
	//	return E_Item_NotFound;
	//}

	//// 放入背包中
	//bagDst.Add(pEquipSrc, bagDst.GetOneFreeSpace(), FALSE);

	//return E_Success;

	return Unequip(n64SerialSrc, bagDst, bagDst.GetOneFreeSpace());
}

//-------------------------------------------------------------------------------------------------------
// 脱下装备(指定背包中位置)
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::Unequip(INT64 n64SerialSrc, ItemContainer &bagDst, INT16 n16IndexDst)
{
	// 检查目标位置是否合法
	if(n16IndexDst < 0 || n16IndexDst > bagDst.GetCurSpaceSize() - 1)
	{
		return GT_INVALID;
	}
	
	// 检查背包中指定位置是否有物品，有则走穿上装备流程
	if(!bagDst.IsOnePlaceFree(n16IndexDst))
	{
		tagEquip *pEquip = GetItem(n64SerialSrc);
		if(NULL == pEquip)
		{
			// 网络卡时,客户端发重复的消息,可能执行到此
			ASSERT(pEquip != NULL);
			return E_Item_NotFound;
		}
		
		return Equip(bagDst, bagDst.GetItem(n16IndexDst)->n64Serial, (EEquipPos)pEquip->n16Index);
	}

	// 脱下装备
	tagEquip *pEquipSrc = Container::Remove(n64SerialSrc);
	if(NULL == pEquipSrc)
	{
		// 网络卡时,客户端发重复的消息,可能执行到此
		ASSERT(pEquipSrc != NULL);
		return E_Item_NotFound;
	}

	// 是否为时限物品
	if(pEquipSrc->IsTimeLimit())
	{
		m_TimeLimitMgr.RemoveFromTimeLeftList(pEquipSrc->n64Serial);
	}

	// 放入背包中
	bagDst.Add(pEquipSrc, n16IndexDst, FALSE);

	return E_Success;
}

tagEquip* EquipContainer::GetEquipBarEquip(DWORD dwTypeID)
{
	tagEquip* pEquip = NULL;
	TMap<INT64, INT16>::TMapIterator iterator = Begin();
	while (GetNextItem(iterator, pEquip))
	{
		if (P_VALID(pEquip) && (pEquip->dwTypeID == dwTypeID))
		{
			return pEquip;
		}
	}
	return NULL;
}

//-------------------------------------------------------------------------------------------------------
// 主副手交换
//-------------------------------------------------------------------------------------------------------
DWORD EquipContainer::SwapWeapon()
{
	tagEquip *pEquipRight	= GetItem((INT16)EEP_RightHand);
	tagEquip *pEquipLeft	= GetItem((INT16)EEP_LeftHand);

	if(P_VALID(pEquipRight))
	{
		Container::Remove((INT16)EEP_RightHand);
	}

	if(P_VALID(pEquipLeft))
	{
		Container::Remove((INT16)EEP_LeftHand);
	}

	if(P_VALID(pEquipRight))
	{
		Add(pEquipRight, EEP_LeftHand);
	}

	if(P_VALID(pEquipLeft))
	{
		Add(pEquipLeft, EEP_RightHand);
	}

	return E_Success;
}

VOID ItemContainer::Add2TimeLeftList(INT64 n64Serial, DWORD dwTimeLimit, tagDWORDTime dwSrcTime)
{
	m_TimeLimitMgr.Add2TimeLeftList(n64Serial, dwTimeLimit, dwSrcTime);
}

// 是否可以移动时装(同一容器内)
DWORD WardrobeContainer::CanMove(INT64 n64Serial, INT16 n16TargetIndex)
{
	if (!g_worldSession.IsWardrobeOn())
		return GT_INVALID;

	// 检查移动目标
	tagItem *pItem = GetItem(n64Serial);

	return CanAddNormalCheck(pItem, n16TargetIndex);
}

// 是否可以向容器中添加道具
DWORD WardrobeContainer::CanAdd2Wardrobe(INT64 n64Serial, ItemContainer& RoleBag, INT16 &n16DstIndex)
{
 	if (!g_worldSession.IsWardrobeOn())
 		return GT_INVALID;

	// 从原容器中取出待移动物品
	tagItem *pItem = RoleBag.GetItem(n64Serial);

	return CanAdd2Wardrobe(pItem, n16DstIndex);
}
DWORD WardrobeContainer::CanAdd2Wardrobe(tagItem *pItem, INT16 &n16DstIndex)
{
 	if (!g_worldSession.IsWardrobeOn())
 		return GT_INVALID;

	// 检查限制条件，是否非时装，是否非时限制
	if (!CanAddItem(pItem))
		return GT_INVALID;
	
	// 检查是否是右键自动放入
 	if (n16DstIndex == GT_INVALID)
 		n16DstIndex = GetOneFreeSpace(pItem->dwTypeID);

	if (!IsOnePlaceFree(n16DstIndex))
		return GT_INVALID;

	return CanAddNormalCheck(pItem, n16DstIndex);
}

// 获得一个空位，失败返回GT_INVALID，调用前请确保该物品是时装并且是非时限制的
INT16 WardrobeContainer::GetOneFreeSpace(DWORD dwTypeID)
{
	TMap<DWORD, tagWardrobeProto*>& mapWardrobeProto = g_attRes.GetWardrobeProtoMap();
	TMap<DWORD, tagWardrobeProto*>::TMapIterator iterator = mapWardrobeProto.Begin();
	tagWardrobeProto* pWardrobeProto = NULL;
	while (mapWardrobeProto.PeekNext(iterator, pWardrobeProto))
	{
		if (!P_VALID(pWardrobeProto))
			continue;
		for (int nCnt = 0; nCnt < MAX_FASHION_PERLAYER; nCnt++)
		{
			if (pWardrobeProto->dwFashionID[nCnt] == dwTypeID)
				return (MAX_FASHION_PERLAYER*(pWardrobeProto->dwLayerID-1)+nCnt);
		}
	}
	return GT_INVALID;
}

BOOL WardrobeContainer::CanAddItem(tagItem* pItem)
{
	// 藏衣阁只能放时装
	if (P_VALID(pItem) && P_VALID(pItem->pProtoType)
		&& pItem->pProtoType->eType == EIT_Fashion
		&& pItem->IsTimeLimit() == FALSE)
		return TRUE;
	else
		return FALSE;
}

DWORD WardrobeContainer::CanAddNormalCheck( tagItem * pItem, INT16 n16TargetIndex )
{
	if(!P_VALID(pItem))
		return GT_INVALID;

	// 目标位置合法性检查
	if(	n16TargetIndex < 0 ||	n16TargetIndex >= GetCurSpaceSize())
		return GT_INVALID;

	// 检查是不是装备
	if (!MIsEquipment(pItem->dwTypeID))
		return GT_INVALID;

	// 检查放入的时装跟位置是否对应
	tagEquip* pEquip = (tagEquip*)pItem;
	if (!P_VALID(pEquip))
		return GT_INVALID;
	if (!P_VALID(pEquip->pEquipProto))
		return GT_INVALID;

	// 检查是否是装备到同一位置的时装
	DWORD dwLayer = CalLayerID(n16TargetIndex);
	DWORD dwPos = (DWORD)(n16TargetIndex%MAX_FASHION_PERLAYER);
	tagWardrobeProto* pWardrobeProto = g_attRes.GetWardrobeProto(dwLayer);
	if (!P_VALID(pWardrobeProto) || dwPos > MAX_FASHION_PERLAYER)
		return GT_INVALID;
	if (pWardrobeProto->eLayerType == EWLT_Diy)
	{
		EWardrobeFashionPos eWardrobeFashionPos = EquipPos2WardrobeFashionPos(pEquip->pEquipProto->eEquipPos);
		if (eWardrobeFashionPos == EWFP_Null || dwPos != eWardrobeFashionPos)
			return GT_INVALID;
	}
	else if (pWardrobeProto->eLayerType == EWLT_Normal)
	{
		if (pWardrobeProto->dwFashionID[dwPos] != pItem->dwTypeID)
			return GT_INVALID;
	}
	else
	{
		return GT_INVALID;
	}
	return E_Success;
}

//---------------------------------------------------------------
//脱下装备，不放入任何容器中
DWORD EquipContainer::Unequip( INT64 n64SerialSrc )
{
	// 检查背包中指定位置是否有物品，有则走穿上装备流程
	//if(!bagDst.IsOnePlaceFree(n16IndexDst))
	//{
	//	tagEquip *pEquip = GetItem(n64SerialSrc);
	//	if(NULL == pEquip)
	//	{
	//		// 网络卡时,客户端发重复的消息,可能执行到此
	//		ASSERT(pEquip != NULL);
	//		return E_Item_NotFound;
	//	}
	//	
	//	return Equip(bagDst, bagDst.GetItem(n16IndexDst)->n64Serial, (EEquipPos)pEquip->n16Index);
	//}

	// 脱下装备
	tagEquip *pEquipSrc = Container::Remove(n64SerialSrc);
	if(NULL == pEquipSrc)
	{
		// 网络卡时,客户端发重复的消息,可能执行到此
		ASSERT(pEquipSrc != NULL);
		return GT_INVALID;
	}

	// 是否为时限物品
	if(pEquipSrc->IsTimeLimit())
	{
		m_TimeLimitMgr.RemoveFromTimeLeftList(pEquipSrc->n64Serial);
	}

	// 放入背包中
	//bagDst.Add(pEquipSrc, n16IndexDst, FALSE);

	return E_Success;
}

//---------------------------------------------------------------
//穿上装备从家族注册的法宝中
DWORD EquipContainer::EquipByFamily(tagItem *pItemNew, EEquipPos ePosDst)
{
	// 取得待物品指针
	
	if (!P_VALID(pItemNew))
	{
		// 网络卡时,客户端发重复的消息,可能执行到此
		ASSERT(pItemNew != NULL);
		return E_Item_NotFound;
	}

	// 判断是否为装备
	if(!MIsEquipment(pItemNew->dwTypeID))
	{
		return E_Item_NotEquipment;
	}

	tagEquip *pEquipNew = (tagEquip *)pItemNew;

	// 得到可装备位置
	EEquipPos ePos = (pEquipNew->pEquipProto)->eEquipPos;
	
	// 检查是否可以装备到目标位置(ring要做特殊判断)
	// if(!(ePos == ePosDst || (ePos + 1 == ePosDst && MIsRing(pEquipNew))))
	if(ePos != ePosDst && (ePos + 1 != ePosDst || (!MIsRing(pEquipNew) && !MIsWeapon(pEquipNew))))
	{
		return E_Equip_InvalidPos;
	}

	//INT16 n16IndexBag = pEquipNew->n16Index;

	//// 目标位不为空
	//if(!IsOnePlaceFree(ePosDst))
	//{
	//	// 从背包中取出装备
	//	((Container *)&bagSrc)->Remove(n16IndexBag);
	//	// 从装备栏中取下装备
	//	tagEquip *pEquipOld = Container::Remove((INT16)ePosDst);
	//	// 将原装备栏中的装备放入背包中
	//	bagSrc.Add(pEquipOld, n16IndexBag, FALSE);
	//}
	//else
	//{
	//	// 从背包中取出装备
	//	bagSrc.Remove(n64SerialSrc);
	//}

	// 将原背包中物品装备上
	return this->Add(pEquipNew, ePosDst);
}

BOOL ItemContainer::OverLappedItemHandler(tagItem* pItem)
{
	INT16 nSpace = GetOneFreeSpace();
	if (GT_INVALID != nSpace )
	{
		Add(pItem, nSpace, FALSE, FALSE);
		return TRUE;
	}

	return FALSE;
}




