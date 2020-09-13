//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: mall.cpp
// author: Sxg
// actor:
// data: 2009-2-1
// last:
// brief: 商城类 -- 可动态更新商场物品
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/mall_define.h"
#include "../WorldDefine/msg_mall.h"
#include "../ServerDefine/msg_rt_errorcode.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/msg_log.h"

#include "mall.h"
#include "att_res.h"
#include "world.h"
#include "role.h"
#include "role_mgr.h"
#include "item_creator.h"
#include "guild.h"
#include "guild_mgr.h"
#include "guild_purchase.h"

Mall g_mall;
//-----------------------------------------------------------------------------
// 构造&析构
//-----------------------------------------------------------------------------
Mall::Mall()
{
	m_bInitOK		= FALSE;
	m_dwLoadTime	= GT_INVALID;
	m_dwTimeKeeper	= GT_INVALID;
	m_byMinuteTime	= GT_INVALID;

	m_nItemNum		= 0;
	m_nPackNum		= 0;
	m_pMallItem		= NULL;
	m_pMallPack		= NULL;
	m_pMallFreeItem = new tagMallGoods;
	m_mapGuildPurchase.Clear();
	m_listLimitItemInfo.Clear();
}

Mall::~Mall()
{
	Destroy();

	SAFE_DEL(m_pMallFreeItem);
}

//-----------------------------------------------------------------------------
// 初始化数据
//-----------------------------------------------------------------------------
BOOL Mall::Init()
{
	if(IsInitOK())
	{
		return TRUE;
	}

	m_dwLoadTime	= g_world.GetWorldTime();
	m_dwTimeKeeper	= g_world.GetWorldTime();
	m_byMinuteTime	= 0;

	m_nItemNum		= g_attRes.GetMallItemNum();
	m_nPackNum		= g_attRes.GetMallPackNum();

	if(m_nItemNum <= 0 && m_nPackNum <= 0)
	{
		ASSERT(0);
		IMSG(_T("\nCaution: \n\tMall Res Load Failed! Please Check!\n\n"));
		return FALSE;
	}

	if(m_nItemNum > 0)
	{
		m_pMallItem	= new tagMallGoods[m_nItemNum];
		if(!P_VALID(m_pMallItem))
		{
			return FALSE;
		}

		InitItem();
	}

	if(m_nPackNum > 0)
	{
		m_pMallPack	= new tagMallGoods[m_nPackNum];
		if(!P_VALID(m_pMallPack))
		{
			Destroy();
			return FALSE;
		}

		if(!CheckPack())
		{
			IMSG(_T("\n\nCaution:\n\tMall pack proto have problem, please check and reopen mall!\n\n\n"));
			Destroy();
			return FALSE;
		}

		InitPack();
	}

	// 初始化免费领取物品
	m_pMallFreeItem->pMallFreeItem = g_attRes.GetMallFreeProto();

	m_bInitOK		= TRUE;

	return m_bInitOK;
}

//-----------------------------------------------------------------------------
// 销毁数据
//-----------------------------------------------------------------------------
VOID Mall::Destroy()
{
	m_bInitOK		= FALSE;
	m_dwLoadTime	= GT_INVALID;
	m_dwTimeKeeper	= GT_INVALID;
	m_byMinuteTime	= GT_INVALID;

	m_nItemNum		= 0;
	m_nPackNum		= 0;

	m_listLimitItemInfo.Clear();

	SAFE_DEL_ARRAY(m_pMallItem);
	SAFE_DEL_ARRAY(m_pMallPack);
}

//-----------------------------------------------------------------------------
// 实时更新商城
//-----------------------------------------------------------------------------
DWORD Mall::ReInit()
{
	// 关闭商城
	if(IsInitOK())
	{
		Destroy();
	}

	// 重新读取资源文件
	if(!g_attRes.ReloadMallProto())
	{
		MAttResCaution(_T("mall proto"), _T("NULL"), GT_INVALID);
		return E_Mall_ProtoLoad_Failed;
	}

	// 初始化商城
	if(!Init())
	{
		return E_Mall_Init_Failed;
	}

	return E_RT_Success;
}

//-----------------------------------------------------------------------------
// 初始化普通商品
//-----------------------------------------------------------------------------
VOID Mall::InitItem()
{
	INT i = 0;
	tagMallItemProto *pProto = NULL;
	TMap<DWORD, tagMallItemProto*> mapMallItem = g_attRes.GetMallItem();
	TMap<DWORD, tagMallItemProto*>::TMapIterator it = mapMallItem.Begin();
	while(mapMallItem.PeekNext(it, pProto))
	{
		m_pMallItem[i].pMallItem	= pProto;
		m_pMallItem[i].nCurNum		= pProto->nNum;
		//如果是限量商品
		if ( GT_INVALID != pProto->nNum )
		{
			m_listLimitItemInfo.PushBack(&m_pMallItem[i]);
		}
		++i;
	}

	ASSERT(m_nItemNum == i);
}

//-----------------------------------------------------------------------------
// 初始化礼品包
//-----------------------------------------------------------------------------
VOID Mall::InitPack()
{
	INT i = 0;
	tagMallPackProto *pProto = NULL;
	TMap<DWORD, tagMallPackProto*> mapMallPack = g_attRes.GetMallPack();
	TMap<DWORD, tagMallPackProto*>::TMapIterator it = mapMallPack.Begin();
	while(mapMallPack.PeekNext(it, pProto))
	{
		m_pMallPack[i].pMallPack	= pProto;
		m_pMallPack[i].nCurNum		= pProto->nNum;

		++i;
	}

	ASSERT(m_nPackNum == i);
}

//-----------------------------------------------------------------------------
// 检查礼品包静态属性是否正确
//-----------------------------------------------------------------------------
BOOL Mall::CheckPack()
{
	tagMallPackProto *pProto = NULL;
	TMap<DWORD, tagMallPackProto*> mapMallPack = g_attRes.GetMallPack();
	TMap<DWORD, tagMallPackProto*>::TMapIterator it = mapMallPack.Begin();
	while(mapMallPack.PeekNext(it, pProto))
	{
		INT nTotalPrice = 0;
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			nTotalPrice += pProto->nItemPrice[i];
		}

		if(nTotalPrice != pProto->nPrice)
		{
			return FALSE;
		}
	}

	return TRUE;
}

//活动是否打开
BOOL Mall::IsActivityOpen()
{
	return g_attRes.GetMallCfg(EMCA_BuyActivityOpen);
}

//是否在活动中
BOOL Mall::IsInActivity()
{
	if (TRUE == IsActivityOpen())
	{
		//在活动时间内
		//DWORD dwBeginTime		= g_attRes.GetMallCfg(EMCA_BuyActivityBeginTime);
		//DWORD dwEndTime			= g_attRes.GetMallCfg(EMCA_BuyActivityEndTime);
		//tagDWORDTime curTime	= GetCurrentDWORDTime();
		//tagDWORDTime BeginTime	= ConvertCfgTimeToDWORDTime(dwBeginTime);
		//tagDWORDTime EndTime	= ConvertCfgTimeToDWORDTime(dwEndTime);
		//
		//if ((DWORD)curTime >= (DWORD)BeginTime &&
		//	(DWORD)curTime <= (DWORD)EndTime)
		{
			return TRUE;
		}
	}

	return FALSE;
}

tagDWORDTime Mall::ConvertCfgTimeToDWORDTime(DWORD dwTime)
{
	DWORD dwTemp = dwTime;
	tagDWORDTime DWORDTime;
	DWORDTime.sec	= 0;
	DWORDTime.min	= 0;
	DWORDTime.hour	= dwTime % 100;
	dwTemp /= 100;
	DWORDTime.day	= dwTemp % 100;
	dwTemp /= 100;
	DWORDTime.month	= dwTime % 100;
	dwTemp /= 100;
	DWORDTime.year	= dwTime % 10000;

	return DWORDTime;
}

//-----------------------------------------------------------------------------
// 获取所有物品
//-----------------------------------------------------------------------------
DWORD Mall::GetAllItems(OUT LPVOID pData)
{
	ASSERT(IsInitOK());

	MTRANS_POINTER(p, pData, tagMallItemProto);

	for(INT i=0; i<GetItemNum(); ++i)
	{
		memcpy(&p[i], m_pMallItem[i].pMallItem, sizeof(tagMallItemProto));

		// 将商品在服务器的下标及当前个数放到proto中
		p[i].nIndexInServer = i;
		p[i].nNum = m_pMallItem[i].nCurNum;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 获取所有礼品包
//-----------------------------------------------------------------------------
DWORD Mall::GetAllPacks(OUT LPVOID pData)
{
	ASSERT(IsInitOK());

	MTRANS_POINTER(p, pData, tagMallPackProto);

	for(INT i=0; i<GetPackNum(); ++i)
	{
		memcpy(&p[i], m_pMallPack[i].pMallPack, sizeof(tagMallPackProto));

		// 将商品在服务器的下标及当前个数放到proto中
		p[i].nIndexInServer = i;
		p[i].nNum = m_pMallPack[i].nCurNum;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 获取免费物品
//-----------------------------------------------------------------------------
DWORD Mall::GetFreeItem(OUT LPVOID pData)
{
	ASSERT(IsInitOK());
	ASSERT(m_pMallFreeItem != NULL);
	ASSERT(m_pMallFreeItem->pMallFreeItem != NULL);

	MTRANS_POINTER(p, pData, tagMallFreeItem);

	p->byNum		= m_pMallFreeItem->pMallFreeItem->byNum;
	p->dwTypeID		= m_pMallFreeItem->pMallFreeItem->dwTypeID;
	p->nUnitPrice	= m_pMallFreeItem->pMallFreeItem->nUnitPrice;

	return 0;
}

//-----------------------------------------------------------------------------
// 更新所有物品
//-----------------------------------------------------------------------------
DWORD Mall::UpdateAllItems(OUT LPVOID pData, OUT INT &nRefreshNum)
{
	ASSERT(IsInitOK());

	MTRANS_POINTER(p, pData, tagMallUpdate);

	nRefreshNum = 0;
	for(INT i=0; i<GetItemNum(); ++i)
	{
		if(!P_VALID(m_pMallItem[i].pMallItem))
		{
			// 不应该执行到该处
			ASSERT(0);
			return GT_INVALID;
		}

		if((BYTE)GT_INVALID == m_pMallItem[i].pMallItem->nNum
			|| m_pMallItem[i].nCurNum == m_pMallItem[i].pMallItem->nNum)
		{
			continue;
		}

		p[nRefreshNum].dwID = m_pMallItem[i].pMallItem->dwID;
		p[nRefreshNum].byRemainNum = m_pMallItem[i].nCurNum;
		++nRefreshNum;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 更新所有礼品包
//-----------------------------------------------------------------------------
DWORD Mall::UpdateAllPacks(OUT LPVOID pData, OUT INT &nRefreshNum)
{
	ASSERT(IsInitOK());

	MTRANS_POINTER(p, pData, tagMallUpdate);

	nRefreshNum = 0;
	for(INT i=0; i<GetPackNum(); ++i)
	{
		if(!P_VALID(m_pMallPack[i].pMallPack))
		{
			ASSERT(0);
			return GT_INVALID;
		}

		if((BYTE)GT_INVALID == m_pMallPack[i].pMallPack->nNum
			|| m_pMallPack[i].nCurNum == m_pMallPack[i].pMallPack->nNum)
		{
			continue;
		}

		p[nRefreshNum].dwID = m_pMallPack[i].pMallPack->dwID;
		p[nRefreshNum].byRemainNum = m_pMallPack[i].nCurNum;
		++nRefreshNum;
	}

	return 0;
}

DWORD Mall::CanSellCart(Role *pRole, BYTE byCartNum, DWORD* dwIDArray, INT* nIndexArray, INT16* n16BuyNumArray, DWORD &dwPrice)
{
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (!P_VALID(dwIDArray) || !P_VALID(nIndexArray) || !P_VALID(n16BuyNumArray))
	{
		return GT_INVALID;
	}

	if( g_world.IsUseIPGSystem() && (!pRole->GetGetCash() ))
		return GT_INVALID;

	// 判断商城是否开放
	if(!IsInitOK())
	{
		return E_Mall_Close;
	}

	// 行囊是否解锁
	if(!pRole->GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// 预检查背包中是否有空位
	if(pRole->GetItemMgr().GetBagFreeSize() < byCartNum)
	{
		return E_Bag_NotEnoughSpace;
	}

	dwPrice = 0;

	// 消息合法性验证
	for (INT i = 0; i < byCartNum; i++)
	{
		if (byCartNum >= MAX_MALL_CART_NUM)
		{
			return GT_INVALID;
		}

		if(n16BuyNumArray[i] <= 0 || nIndexArray[i] < 0)
		{
			return GT_INVALID;
		}

		// 检查索引合法性
		if(nIndexArray[i] >= GetItemNum())
		{
			return GT_INVALID;
		}

		const tagMallItemProto *pProto = m_pMallItem[nIndexArray[i]].pMallItem;
		// id
		if(pProto->dwID != dwIDArray[i])
		{
			return E_Mall_ID_Error;
		}

		// 检查目前玩家拥有的该物品数量是否已达到最大
		if( !MIsEquipment( pProto->dwTypeID ) )
		{
			tagItemProto* pTmpProto = NULL;
			pTmpProto = g_attRes.GetItemProto(pProto->dwTypeID);
			if (!P_VALID(pTmpProto))
			{
				return E_Mall_ID_Error;
			}
			else if( pTmpProto->n16MaxHoldNum != GT_INVALID )// GT_INVALID 表示物品个数没有限制
			{
				if ( pRole->GetItemMgr().GetBagSameItemCount(pProto->dwTypeID) >= pTmpProto->n16MaxHoldNum )
				{
					return E_Trade_ItemNum_MaxHoldLimit;
				}
			}
		}
		else 
		{
			tagEquipProto * pEqipProto = g_attRes.GetEquipProto  (pProto->dwTypeID);
			if( !P_VALID(pEqipProto) )
			{
				return E_Mall_ID_Error;
			}
		}

		// 购买次数限制
		if (pProto->nNum >= MALL_ITEM_PERSONAL_NUM_LIMIT_LINE)
		{
			if(pRole->BuyThisMallItemCount(pProto->dwTypeID) > pProto->nNum - MALL_ITEM_PERSONAL_NUM_LIMIT_LINE)
				return E_Mall_PersonalBuyLimit;
		}

		// 个数
		if(m_pMallItem[nIndexArray[i]].nCurNum != GT_INVALID && m_pMallItem[nIndexArray[i]].nCurNum < n16BuyNumArray[i])
		{
			return E_Mall_Item_NotEnough;
		}

		// 单价
		if(pProto->dwTimeSaleEnd != GT_INVALID && g_world.GetWorldTime() < pProto->dwTimeSaleEnd)
		{
			// 促销
			dwPrice += (pProto->nSalePrice * n16BuyNumArray[i]);
		}
		else
		{
			// 正常
			dwPrice += (pProto->nPrice * n16BuyNumArray[i]);
		}

		if ( pProto->n8Kind == 7 || pProto->n8Kind == 8 || pProto->n8Kind == 10)
		{
			return GT_INVALID;
		}

	}

	if( g_world.IsUseIPGSystem() )
	{
		int nCash = pRole->GetIPGCash();
		if(dwPrice > nCash || dwPrice <= 0)
		{
			return E_BagYuanBao_NotEnough;
		}
	}
	else
	{
		// 检查玩家元宝是否足够
		if(dwPrice > pRole->GetCurMgr().GetBagYuanBao() || dwPrice <= 0)
		{
			return E_BagYuanBao_NotEnough;
		}
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// 出售物品
//-----------------------------------------------------------------------------
DWORD Mall::SellItem(Role *pRole, DWORD dwToRoleID, DWORD dwCmdID, 
					DWORD dwID, INT nIndex, INT nUnitPrice, INT16 n16BuyNum, 
					OUT tagMallItemSell &itemSell,EShoppingDistrict eShopingDistrict, INT16 n16OfferItemLvl)
{
	ASSERT(P_VALID(pRole));
	ASSERT(IsInitOK());
	// 无用判断， 注释掉,如无问题，删之 [6/8/2012 zhangzhihua]ASSERT(nUnitPrice > 0);
	ASSERT(n16BuyNum > 0);

	if( g_world.IsUseIPGSystem() && (!pRole->GetGetCash() ))
		return GT_INVALID;

	// 检查索引合法性
	if(nIndex >= GetItemNum())
	{
		return GT_INVALID;
	}

	const tagMallItemProto *pProto = m_pMallItem[nIndex].pMallItem;

	// id
	if(pProto->dwID != dwID)
	{
		return E_Mall_ID_Error;
	}

	// 检查目前玩家拥有的该物品数量是否已达到最大
	if( !MIsEquipment( pProto->dwTypeID ) )
	{
		tagItemProto* pTmpProto = NULL;
		pTmpProto = g_attRes.GetItemProto(pProto->dwTypeID);
		if (!P_VALID(pTmpProto))
		{
			return E_Mall_ID_Error;
		}
		else if( pTmpProto->n16MaxHoldNum != GT_INVALID )// GT_INVALID 表示物品个数没有限制
		{
			if ( pRole->GetItemMgr().GetBagSameItemCount(pProto->dwTypeID) >= pTmpProto->n16MaxHoldNum )
			{
				return E_Trade_ItemNum_MaxHoldLimit;
			}
		}
	}
	else 
	{
		tagEquipProto * pEqipProto = g_attRes.GetEquipProto  (pProto->dwTypeID);
		if( !P_VALID(pEqipProto) )
		{
			return E_Mall_ID_Error;
		}
	}

	// 购买次数限制
	if (pProto->nNum >= MALL_ITEM_PERSONAL_NUM_LIMIT_LINE)
	{
		if(pRole->BuyThisMallItemCount(pProto->dwTypeID) > pProto->nNum - MALL_ITEM_PERSONAL_NUM_LIMIT_LINE)
			return E_Mall_PersonalBuyLimit;
	}

	// 个数
	if(m_pMallItem[nIndex].nCurNum != GT_INVALID && m_pMallItem[nIndex].nCurNum < n16BuyNum)
	{
		itemSell.byRemainNum = m_pMallItem[nIndex].nCurNum;
		return E_Mall_Item_NotEnough;
	}

	INT nPrice = 0;

	// 单价
	if(pProto->dwTimeSaleEnd != GT_INVALID && g_world.GetWorldTime() < pProto->dwTimeSaleEnd)
	{
		// 促销
		nPrice = pProto->nSalePrice;
	}
	else
	{
		// 正常
		nPrice = pProto->nPrice;
	}

	// 无用判断， 注释掉,如无问题，删之 [6/8/2012 zhangzhihua]
	//if(nPrice != nUnitPrice)
	//{
	//	return E_Mall_YuanBao_Error;
	//}

	nPrice *= n16BuyNum;
    if ( pProto->n8Kind != 7 && pProto->n8Kind != 8)
	{
		if( g_world.IsUseIPGSystem() )
		{
			//int nCash = pRole->GetIPGCash();
			//if(nPrice > nCash || nPrice <= 0)
			//{
			//	return E_BagYuanBao_NotEnough;
			//}

			if(pRole->GetGetCash())
			{
				TCHAR tszTemp[64] = {0};
				wsprintf(tszTemp,_T("DecIPGCash3. Dec cash:%d. \r\n"),nPrice);
				g_world.WriteKroeaLog(tszTemp);

				if( !pRole->DecIPGCash(nPrice,n16BuyNum,pProto->dwTypeID) )
				{
					return GT_INVALID;
				}
			}
		}
		else
		{
			// 检查玩家元宝是否足够
			if( (nPrice > pRole->GetCurMgr().GetBagYuanBao() && nPrice > pRole->GetItemMgr().GetBagSameItemCount(YuanBaoDaiBi)) || nPrice <= 0 )
			{
				return E_BagYuanBao_NotEnough;
			}
		}
	}

    if(  pProto->n8Kind == 7)
	{
		// 检查玩家赠券是否足够
		if(nPrice > pRole->GetCurMgr().GetExchangeVolume() || nPrice <= 0)
		{
			return E_ExVolume_NotEnough;
		}
        //如果是赠券，赠送就返回无效
		if ( ELCID_Mall_PresentItem == dwCmdID )
		{
			return GT_INVALID;
		}

	}

    if ( pProto->n8Kind == 8)
    {
		if(nPrice > pRole->GetCurMgr().GetVIPPoint() || nPrice <= 0)
		{
			return E_VipPoint_NotEnough;
		} 
		//如果是积分，赠送就返回无效
		if ( ELCID_Mall_PresentItem == dwCmdID )
		{
			return GT_INVALID;
		}
    }
	
	// 一元计划
	if ( pProto->n8Kind == 10 )
	{
		tagMallOfferProto* pMallOfferProto = g_attRes.GetMallOfferProto(dwID);
		if (!P_VALID(pMallOfferProto))
		{
			IMSG(_T("Can't Find Mall Offer item,mall id is%d!!!"), dwID);
			return GT_INVALID;
		}
		if (pMallOfferProto->MallOfferProto2Client.nLevel != n16OfferItemLvl)
		{
			IMSG(_T("OfferItemLvl Invalid!!!"));
			return GT_INVALID;
		}
		if (pRole->GetLevel() < pMallOfferProto->MallOfferProto2Client.nLevel)
		{
			return E_Mall_Offer_Level_Limit;
		}

		if (P_VALID(pMallOfferProto) && P_VALID(pRole))
		{
			if (pRole->GetBuyLvlItemFlag(n16OfferItemLvl) == 1)
				return E_Mall_Offer_AlreadyBuy;
			else
				pRole->SetBuyLvlItemFlag(n16OfferItemLvl, 1);
		}
		else
		{
			return GT_INVALID;
		}
	}

	// 创建物品
	tagItem *pItemNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, pProto->dwTypeID, n16BuyNum, EIQ_White);
	if(!P_VALID(pItemNew))
	{
		ASSERT(P_VALID(pItemNew));
		return E_Mall_CreateItem_Failed;
	}

	if (P_VALID(pItemNew->pProtoType) && pItemNew->pProtoType->eSpecFunc != EISF_ZhanDouFu)
		pItemNew->dw1stGainTime	 = g_world.GetWorldTime();

	// 如果有赠品，则生成赠品
	tagItem *pPresentNew = NULL;
	if(pProto->dwPresentID != 0)
	{
		pPresentNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, 
			pProto->dwPresentID, pProto->nPresentNum * n16BuyNum, EIQ_White);
		if(!P_VALID(pPresentNew))
		{
			::Destroy(pItemNew);
			ASSERT(P_VALID(pPresentNew));
			return E_Mall_CreatePres_Failed;
		}

		if (P_VALID(pPresentNew->pProtoType) && pPresentNew->pProtoType->eSpecFunc != EISF_ZhanDouFu)
			pPresentNew->dw1stGainTime	= g_world.GetWorldTime();
	}

	// 回馈玩家赠点
	if (pProto->nExAssign >= 0)
	{
		itemSell.nVipPoint = pProto->nExAssign * n16BuyNum;
	}

	// 更新商店中物品个数
	if(m_pMallItem[nIndex].nCurNum != GT_INVALID && m_pMallItem[nIndex].nCurNum < MALL_ITEM_PERSONAL_NUM_LIMIT_LINE)
	{
		m_pMallItem[nIndex].nCurNum -= n16BuyNum;
		if ( P_VALID(m_pMallItem[nIndex].pMallItem) )
		{
			SaveToDB(m_pMallItem[nIndex].pMallItem->dwID,m_pMallItem[nIndex].pMallItem->dwTypeID,m_pMallItem[nIndex].nCurNum);
		}
		
	}
    
	if ( pProto->n8Kind != 7 && pProto->n8Kind != 8)
	{
		//if( g_world.IsUseIPGSystem() )
		//{
		//	if(pRole->GetGetCash())
		//	{
		//		TCHAR tszTemp[64] = {0};
		//		wsprintf(tszTemp,_T("DecIPGCash3. Dec cash:%d. \r\n"),nPrice);
		//		g_world.WriteKroeaLog(tszTemp);

		//		pRole->DecIPGCash(nPrice,pItemNew->dwTypeID);
		//	}
		//}
		//else
		//{
		//	// 从玩家背包中扣除元宝
		//	pRole->GetCurMgr().DecBagYuanBao(nPrice, dwCmdID);
		//}

		BOOL bIsUseYuanBaoDaiBi = CanUseYuanBaoDaiBi(pRole, nPrice);
		if( !g_world.IsUseIPGSystem() && !bIsUseYuanBaoDaiBi )//优先扣除元宝代币
		{
			// 从玩家背包中扣除元宝
		    pRole->GetCurMgr().DecBagYuanBao(nPrice, dwCmdID);
		}
		if(bIsUseYuanBaoDaiBi)
		{
			pItemNew->eCreateMode = EICM_YuanBaoDaiBi;
		}
		// log
		LogMallSell(pRole->GetID(), dwToRoleID, *pItemNew, 
			pItemNew->n64Serial, n16BuyNum, pItemNew->dw1stGainTime, nPrice, 0,0, dwCmdID);
	}
	if(  pProto->n8Kind == 7)
	{ // 从玩家背包中扣除赠券
		pRole->GetCurMgr().DecExchangeVolume(nPrice, dwCmdID);
		// log
		LogMallSell(pRole->GetID(), dwToRoleID, *pItemNew, 
			pItemNew->n64Serial, n16BuyNum, pItemNew->dw1stGainTime, 0, nPrice,0, dwCmdID);
	}
	if ( pProto->n8Kind == 8)
	{
		// 从玩家背包中扣除积分
		pRole->GetCurMgr().DecVIPPoint(nPrice, dwCmdID);
		// log
		LogMallSell(pRole->GetID(), dwToRoleID, *pItemNew, 
			pItemNew->n64Serial, n16BuyNum, pItemNew->dw1stGainTime, 0, 0,nPrice, dwCmdID);
	}
	
	// 改变角色购买此物品的计数
	if (pProto->nNum >= MALL_ITEM_PERSONAL_NUM_LIMIT_LINE)
	{
		if (P_VALID(pProto) && P_VALID(pRole))
			pRole->IncreaseRoleBuyMallItemCount(pProto->dwTypeID, n16BuyNum);
	}

	// 设置传出参数
	itemSell.pItem			= pItemNew;
	itemSell.pPresent		= pPresentNew;
	itemSell.nYuanBaoNeed	= nPrice;
	itemSell.byRemainNum	= m_pMallItem[nIndex].nCurNum;

	return E_Success;
}

//-----------------------------------------------------------------------------
// 出售礼品包
//-----------------------------------------------------------------------------
DWORD Mall::SellPack(Role *pRole, DWORD dwToRoleID, DWORD dwCmdID, DWORD dwID, 
					 BYTE byIndex, INT nUnitPrice,  OUT tagMallPackSell &packSell, 
					 BOOL bNeedCheckBagSpace,EShoppingDistrict eShopingDistrict)
{
	ASSERT(P_VALID(pRole));
	ASSERT(IsInitOK());
	ASSERT(nUnitPrice > 0);

	if( g_world.IsUseIPGSystem() && (!pRole->GetGetCash() ))
		return GT_INVALID;


	// 检查索引合法性
	if(byIndex >= GetPackNum())
	{
		return GT_INVALID;
	}

	const tagMallPackProto *pProto = m_pMallPack[byIndex].pMallPack;

	// id
	if(pProto->dwID != dwID)
	{
		return E_Mall_ID_Error;
	}

	// 个数
	if(m_pMallPack[byIndex].nCurNum != GT_INVALID && m_pMallPack[byIndex].nCurNum < 1)
	{
		return E_Mall_Pack_NotEnough;
	}

	INT nPrice = 0;

	// 单价
	if(pProto->dwTimeSaleEnd != GT_INVALID && g_world.GetWorldTime() < pProto->dwTimeSaleEnd)
	{
		// 促销
		nPrice = pProto->nSalePrice;
	}
	else
	{
		// 正常
		nPrice = pProto->nPrice;
	}

	// 金钱
	if(nPrice != nUnitPrice)
	{
		return E_Mall_YuanBao_Error;
	}

	if( g_world.IsUseIPGSystem() )
	{
		//int nCash = pRole->GetIPGCash();
		//if(nPrice > nCash)
		//{
		//	return E_BagYuanBao_NotEnough;
		//}

		if(pRole->GetGetCash())
		{
			TCHAR tszTemp[64] = {0};
			wsprintf(tszTemp,_T("DecIPGCash4. Dec cash:%d. \r\n"),nPrice);
			g_world.WriteKroeaLog(tszTemp);

			if( !pRole->DecIPGCash(nPrice,1,pProto->dwPresentID) )
			{
				return GT_INVALID;
			}
		}
	}
	else
	{
		//检查玩家元宝是否足够
		if(nPrice > pRole->GetCurMgr().GetBagYuanBao() && nPrice > pRole->GetItemMgr().GetBagSameItemCount(YuanBaoDaiBi))
		{
			return E_BagYuanBao_NotEnough;
		}
	}

	
		
 	

	// 检查玩家背包空间
	if(bNeedCheckBagSpace && pProto->n8ItemKind > pRole->GetItemMgr().GetBagFreeSize())
	{
		return E_Bag_NotEnoughSpace;
	}

	// 如果有赠品，则先生成赠品 -- 方便生成物品失败时，内存释放处理
	tagItem *pPresentNew = NULL;
	if(pProto->dwPresentID != 0)
	{
		pPresentNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, 
			pProto->dwPresentID, pProto->nPresentNum, EIQ_White);
		if(!P_VALID(pPresentNew))
		{
			ASSERT(P_VALID(pPresentNew));
			IMSG(_T("\n\nCaution:\n"));
			IMSG(_T("\tThere is critical error in proto of mall pack or item&equip!!!!!!!!!\n\n"));
			return E_Mall_CreatePres_Failed;
		}

		if (P_VALID(pPresentNew->pProtoType) && pPresentNew->pProtoType->eSpecFunc != EISF_ZhanDouFu)
			pPresentNew->dw1stGainTime = g_world.GetWorldTime();
	}

	//if( g_world.IsUseIPGSystem() )
	//{
	//	if(pRole->GetGetCash())
	//	{
	//		TCHAR tszTemp[64] = {0};
	//		wsprintf(tszTemp,_T("DecIPGCash4. Dec cash:%d. \r\n"),nPrice);
	//		g_world.WriteKroeaLog(tszTemp);

	//		pRole->DecIPGCash(nPrice,pPresentNew->dwTypeID);
	//	}
	//}

	// 创建物品
	INT i = 0;
	for(; i<pProto->n8ItemKind; ++i)
	{
		tagItem *pItemNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, pProto->dwTypeID[i], pProto->byItemNum[i], EIQ_White);
		if(!P_VALID(pItemNew))
		{
			ASSERT(P_VALID(pItemNew));
			IMSG(_T("\n\nCaution:\n"));
			IMSG(_T("\tThere is critical error in proto of mall pack or item&equip!!!!!!!!!\n\n"));
			break;
		}
		else
		{
			if (P_VALID(pItemNew->pProtoType) && pItemNew->pProtoType->eSpecFunc != EISF_ZhanDouFu)
				pItemNew->dw1stGainTime = g_world.GetWorldTime();
			packSell.pItem[i] = pItemNew;
		}
	}

	// 检查物品是否均创建成功
	if(i != pProto->n8ItemKind)
	{
		// 是否已创建成功物品内存
		for(INT j=0; j<i; ++j)
		{
			::Destroy(packSell.pItem[j]);
		}

		// 赠品
		if(P_VALID(pPresentNew))
		{
			::Destroy(pPresentNew);
		}

		return E_Mall_CreateItem_Failed;
	}

	// 回馈玩家赠点
	if (pProto->nExAssign >= 0)
	{
		packSell.nVipPoint = pProto->nExAssign;
	}

	// 更新商店中物品个数
	if(m_pMallPack[byIndex].nCurNum != GT_INVALID)
	{
		--m_pMallPack[byIndex].nCurNum;
	}

	// 从玩家背包中扣除元宝
	//pRole->GetCurMgr().DecBagYuanBao(nPrice, dwCmdID);
	//if( g_world.IsUseIPGSystem() )
	//{
	//	if(pRole->GetGetCash())
	//	{
	//		TCHAR tszTemp[64] = {0};
	//		wsprintf(tszTemp,_T("DecIPGCash4. Dec cash:%d. \r\n"),nPrice);
	//		g_world.WriteKroeaLog(tszTemp);

	//		pRole->DecIPGCash(nPrice,pPresentNew->dwTypeID);
	//	}
	//}
	//else
	//{
	//	// 从玩家背包中扣除元宝
	//	pRole->GetCurMgr().DecBagYuanBao( nPrice, dwCmdID);
	//}
	BOOL bIsUseYuanBaoDaiBi = CanUseYuanBaoDaiBi(pRole, nPrice);
	if( !g_world.IsUseIPGSystem() && !bIsUseYuanBaoDaiBi)
	{
		// 从玩家背包中扣除元宝
		pRole->GetCurMgr().DecBagYuanBao( nPrice, dwCmdID);
	}

	//修改物品产出类型
	if(bIsUseYuanBaoDaiBi)
	{
		for(i=0; i<pProto->n8ItemKind; ++i)
		{
				packSell.pItem[i]->eCreateMode = EICM_YuanBaoDaiBi;
		}
	}

	// 礼品包内物品log
	for(i=0; i<pProto->n8ItemKind; ++i)
	{
		LogMallSell(pRole->GetID(), dwToRoleID, *packSell.pItem[i], 
			packSell.pItem[i]->n64Serial, packSell.pItem[i]->GetNum(), 
			packSell.pItem[i]->dw1stGainTime, pProto->nItemPrice[i], 0,0, dwCmdID);
	}

	// 礼品包log
	LogMallSellPack(dwID, pRole->GetID(), dwToRoleID, nPrice);

	

	// 设置传出参数
	packSell.pPresent		= pPresentNew;
	packSell.nYuanBaoNeed	= nPrice;
	packSell.byRemainNum	= m_pMallPack[byIndex].nCurNum;

	return E_Success;
}

//-----------------------------------------------------------------------------
// 免费发放
//-----------------------------------------------------------------------------
DWORD Mall::GrantFreeItem(Role *pRole, DWORD dwID, OUT tagMallItemSell &itemSell)
{
	// 是否有免费物品
	if(GetFreeItemNum() <= 0)
	{
		return E_Mall_Free_NotExist;
	}

	// 检查玩家当天是否已经领取过了
	tagDWORDTime dwCurDay = g_world.GetWorldTime();
	dwCurDay.hour	= 0;
	dwCurDay.min	= 0;
	dwCurDay.sec	= 0;

	if(pRole->GetLastMallFreeTime() >= dwCurDay)
	{
		return E_Mall_Free_AlreadyGet;
	}

	// 生成免费物品
	tagItem *pItemNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, 
		m_pMallFreeItem->pMallFreeItem->dwTypeID, 
		m_pMallFreeItem->pMallFreeItem->byNum, EIQ_White);
	if(!P_VALID(pItemNew))
	{
		ASSERT(P_VALID(pItemNew));
		IMSG(_T("\n\nCaution:\n"));
		IMSG(_T("\tThere is critical error in proto of mall free item!!!!!!!!!\n\n"));
		return E_Mall_CreateItem_Failed;
	}

	// 设置领取时间
	pRole->SetLastGetMallFreeTime(dwCurDay);

	// 设置传出参数
	itemSell.pItem = pItemNew;

	return E_Success;
}


//-----------------------------------------------------------------------------
//限量物品个数
//-----------------------------------------------------------------------------
DWORD Mall::InitLimitNumItemInfo(INT nItemInfoNum, LPVOID pData)
{
     MTRANS_POINTER(pItemInfo, pData, tagMallItemLimitNum);
     tagMallGoods	*pProto = NULL;
     for (INT i = 0; i < nItemInfoNum; i++)
     {
           m_listLimitItemInfo.ResetIterator();
		   while(m_listLimitItemInfo.PeekNext(pProto))
		   {
			   if( P_VALID(pProto))
			   {
				   if (pProto->pMallItem)
				   {

					   if ( (pProto->pMallItem->dwID == pItemInfo[i].dwID) && ( pProto->pMallItem->dwTypeID == pItemInfo[i].dwTypeID) && 
						    (pProto->pMallItem->nNum != GT_INVALID) )
					   {
						   pProto->nCurNum  = pItemInfo[i].n16RemainNum;
						   break;
					   }
				   }
			   }
		   }
		 
     }

     return E_Success;
}

//-----------------------------------------------------------------------------
//限量物品存到数据库
//-----------------------------------------------------------------------------
VOID  Mall::SaveToDB(DWORD dwID,DWORD dwTypeID,INT nNum)
{
     tagNDBC_UpdateLimitItemInfo Msg;
	 Msg.LimitItemInfo.dwID         = dwID;
	 Msg.LimitItemInfo.dwTypeID     = dwTypeID;
	 Msg.LimitItemInfo.n16RemainNum = nNum;
	 if ( nNum <= 0 )
	 {
		 Msg.LimitItemInfo.bIsEnd = TRUE;
	 }
	 else
	 {
		 Msg.LimitItemInfo.bIsEnd = FALSE;
	 }
	 g_dbSession.Send(&Msg,Msg.dwSize);
}

//-----------------------------------------------------------------------------
// 载入所有团购信息
//-----------------------------------------------------------------------------
DWORD Mall::LoadAllGPInfo( INT nGPInfoNum, LPVOID pData )
{
	MTRANS_POINTER(pGPInfo, pData, tagGPInfo);

	for (int i=0; i<nGPInfoNum; i++)
	{
		GuildPurchase* pMapGPInfo = m_mapGuildPurchase.Peek(pGPInfo->dwGuildID);
		if (!P_VALID(pMapGPInfo))
		{
			pMapGPInfo = new GuildPurchase;
			if (!P_VALID(pMapGPInfo) || !pMapGPInfo->Init(pGPInfo->dwGuildID))
			{
				ASSERT(P_VALID(pMapGPInfo));
				IMSG(_T("\npMapGPInfo Create Faild in LoadAllGPInfo.\n"));
				SAFE_DEL(pMapGPInfo);
				goto Next_GPInfo;
			}
			else
			{
				m_mapGuildPurchase.Add(pGPInfo->dwGuildID, pMapGPInfo);
			}
		}

		// 建立新的团购信息
		tagGroupPurchase* pTmpGPInfo	= new tagGroupPurchase;
		pTmpGPInfo->dwGuildID			= pGPInfo->dwGuildID;
		pTmpGPInfo->dwRoleID			= pGPInfo->dwRoleID;
		pTmpGPInfo->dwMallID			= pGPInfo->dwMallID;
		pTmpGPInfo->nPrice				= pGPInfo->nPrice;
		pTmpGPInfo->nParticipatorNum	= pGPInfo->nParticipatorNum;
		pTmpGPInfo->nRequireNum			= pGPInfo->nRequireNum;
		pTmpGPInfo->dwRemainTime		= pGPInfo->nRemainTime;

		pTmpGPInfo->listParticipators	= new TList<DWORD>;
		if (!P_VALID(pTmpGPInfo->listParticipators))
		{
			ASSERT(P_VALID(pTmpGPInfo->listParticipators));
			IMSG(_T("\nlistParticipators Create Faild in LoadAllGPInfo.\n"));
			SAFE_DEL(pTmpGPInfo);
			goto Next_GPInfo;
		}

		for (int j=0; j<pGPInfo->nParticipatorNum; j++)
		{
			pTmpGPInfo->listParticipators->PushBack(pGPInfo->dwParticipators[j]);
		}

		// 加入信息到管理表
		if (!pMapGPInfo->Add(pTmpGPInfo, FALSE))
		{
			SAFE_DEL(pTmpGPInfo->listParticipators);
			SAFE_DEL(pTmpGPInfo);
			SAFE_DEL(pMapGPInfo);
			goto Next_GPInfo;
		}

Next_GPInfo:
		// 将pGPInfo进行位移
		pGPInfo = (tagGPInfo*)((BYTE*)pGPInfo + sizeof(tagGPInfo) + (pGPInfo->nParticipatorNum-1)*sizeof(DWORD));
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// 发起团购
//-----------------------------------------------------------------------------
DWORD Mall::LaunchGroupPurchase(Role *pRole, DWORD dwID, BYTE byScope,
								BYTE byIndex, INT nUnitPrice)
{
	if( !P_VALID(pRole) )
		return GT_INVALID;

	if( g_world.IsUseIPGSystem() && (!pRole->GetGetCash() ))
		return GT_INVALID;

	// 判断商城是否开放
	if(!IsInitOK())
	{
		return E_Mall_Close;
	}

	// 行囊是否解锁
	if (!pRole->GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	tagGroupPurchase* pGPInfo = NULL;
	DWORD dwItemTypeID = GT_INVALID;

	// 取得帮派团购信息
	DWORD dwGuildID = pRole->GetGuildID();
	if (!GT_VALID(dwGuildID))
	{
		return E_GroupPurchase_NotInGuild;
	}

	GuildPurchase* pMapGPInfo = m_mapGuildPurchase.Peek(dwGuildID);
	if (!P_VALID(pMapGPInfo))
	{
		// 该帮派还没有团购信息
		pMapGPInfo = new GuildPurchase;
		if (!P_VALID(pMapGPInfo) || !pMapGPInfo->Init(dwGuildID))
		{
			SAFE_DEL(pMapGPInfo);
			return E_GroupPurchase_NotMember;
		}

		// 加入到管理表中
		m_mapGuildPurchase.Add(dwGuildID, pMapGPInfo);
	}

	// 检查数据合法性并管理团购信息
	DWORD dwErrorCode = pMapGPInfo->LaunchGroupPurchase(pRole, dwID, byScope, byIndex, nUnitPrice, pGPInfo, dwItemTypeID);

	// 处理结果
	if(E_Success == dwErrorCode && pGPInfo->nPrice > 0)
	{
		//没有团购，不改了IPG
		// 从玩家背包中扣除元宝
		pRole->GetCurMgr().DecBagYuanBao(pGPInfo->nPrice, ELCLD_GroupPurchase_BuyItem );
		//if( g_world.IsUseIPGSystem() && (pRole->GetGetCash() ))
		//	pRole->DecIPGCash();

		// 帮派内广播团购发起成功消息
		Guild* tmpGuild = g_guildMgr.GetGuild(pGPInfo->dwGuildID);
		if (!P_VALID(tmpGuild)) //以防万一，再判断一下
		{
			ASSERT(tmpGuild);

			// 有错误，就不发了
			return dwErrorCode;
		}

		tagNS_RespondBroadCast send;
		send.eType = ERespondBroadCast_Launch;
		send.dwRoleID = pGPInfo->dwRoleID;
		send.dwTypeID = dwItemTypeID;

		tmpGuild->SendGuildMsg(&send, send.dwSize);
	}

	// 团购所需人数应该不是一个人，所以在此处不做达成条件判断

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 响应团购
//-----------------------------------------------------------------------------
DWORD Mall::RespondGroupPurchase(Role *pRole, DWORD dwGuildID, DWORD dwID, DWORD dwRoleID, INT nPrice)
{
	//没有团购
	return GT_INVALID;

	if( !P_VALID(pRole) )
		return GT_INVALID;

	if( g_world.IsUseIPGSystem() && (!pRole->GetGetCash() ))
		return GT_INVALID;

	// 判断商城是否开放
	if(!IsInitOK())
	{
		return E_Mall_Close;
	}

	// 行囊是否解锁
	if(!pRole->GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	tagGroupPurchase* pGPInfo = NULL;

	// 检查数据合法性并管理团购信息
	GuildPurchase* pMapGPInfo = m_mapGuildPurchase.Peek(dwGuildID);
	if (!P_VALID(pMapGPInfo))
	{
		return E_GroupPurchase_NoInfo;
	}

	DWORD dwErrorCode = pMapGPInfo->RespondGroupPurchase(pRole, dwID, dwRoleID, nPrice, pGPInfo);

	BOOL bCompelete = FALSE;

	// 处理结果
	if(E_Success == dwErrorCode && pGPInfo->nPrice > 0)
	{
		// 从玩家背包中扣除元宝
		pRole->GetCurMgr().DecBagYuanBao(pGPInfo->nPrice, ELCLD_GroupPurchase_BuyItem );


		// 更新响应者列表
		pGPInfo->listParticipators->PushBack(pRole->GetID());
		pGPInfo->nParticipatorNum++;

		if (pGPInfo->nParticipatorNum == pGPInfo->nRequireNum)
		{
			bCompelete = TRUE;
		}
		else
		{
			pMapGPInfo->UpdateGPInfo2DB(pGPInfo);
		}
	}

	// 团购成功
	if (bCompelete)
	{
		pMapGPInfo->DoSuccessStuff(pGPInfo);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 获取指定帮派的团购简易信息
//-----------------------------------------------------------------------------
DWORD Mall::GetAllSimGPInfo(Role* pRole)
{
	ASSERT(P_VALID(pRole));
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (!IsInitOK())
	{
		return E_Mall_Close;
	}

	DWORD dwErrorCode = E_Success;
	DWORD dwGuildID = pRole->GetGuildID();

	// 检验帮派的合法性
	if (!GT_VALID(dwGuildID))
	{
		dwErrorCode = E_GroupPurchase_NotInGuild;
	}

	Guild* tmpGuild = g_guildMgr.GetGuild(dwGuildID);
	if (!P_VALID(tmpGuild))
	{
		dwErrorCode = E_GroupPurchase_NotInGuild;
	}

	GuildPurchase* pMapGPInfo = m_mapGuildPurchase.Peek(dwGuildID);
	if (!P_VALID(pMapGPInfo))
	{
		dwErrorCode = E_GroupPurchase_NoInfo;
	}

	if (dwErrorCode == E_Success)
	{
		// 读取该帮派的团购信息
		INT nGPInfoNum = pMapGPInfo->GetGroupPurchaseInfoNum();
		INT nMsgSize = sizeof(tagNS_GetSimGPInfo)+(nGPInfoNum-1)*sizeof(tagSimGPInfo);
		MCREATE_MSG(pSend, nMsgSize, NS_GetSimGPInfo);

		pSend->nGroupPurchase	= tmpGuild->GetGuildAtt().nGroupPurchase;
		pSend->nGPInfoNum		= nGPInfoNum;		

		if (nGPInfoNum > 0)
		{
			pSend->dwErrorCode = dwErrorCode = pMapGPInfo->GetAllSimGPInfo(nGPInfoNum, pSend->simData);

			if (pSend->nGPInfoNum != nGPInfoNum)
			{
				// 重新计算大小
				pSend->dwSize		= sizeof(tagNS_GetSimGPInfo) + (nGPInfoNum - 1) * sizeof(tagSimGPInfo);
				pSend->nGPInfoNum	= nGPInfoNum;
			}
		}

		pRole->SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}
	else
	{
		tagNS_GetSimGPInfo send;
		if ((dwErrorCode == E_GroupPurchase_NoInfo) && P_VALID(tmpGuild))
		{
			send.dwErrorCode	= E_Success;
			send.nGroupPurchase	= tmpGuild->GetGuildAtt().nGroupPurchase;
			send.nGPInfoNum		= 0;
		}
		else
		{
			send.dwErrorCode = dwErrorCode;
		}
		pRole->SendMessage(&send, send.dwSize);
	}


	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 获取指定团购信息的响应者列表
//-----------------------------------------------------------------------------
DWORD Mall::GetParticipators(Role* pRole, DWORD dwGuildID, DWORD dwID, DWORD dwRoleID)
{
	ASSERT(P_VALID(pRole));
	if (!P_VALID(pRole) || pRole->GetGuildID() != dwGuildID)
	{
		return GT_INVALID;
	}

	if (!IsInitOK())
	{
		return E_Mall_Close;
	}

	DWORD dwErrorCode = E_Success;

	if (!GT_VALID(dwGuildID) || !GT_VALID(dwRoleID))
	{
		dwErrorCode = E_GroupPurchase_NoInfo;
	}

	// 读取响应者
	if (dwErrorCode == E_Success)
	{
		// 确定帮派
		GuildPurchase* pMapGPInfo = m_mapGuildPurchase.Peek(dwGuildID);
		INT nParticipatorNum = 0;
		if (P_VALID(pMapGPInfo))
		{
			nParticipatorNum = pMapGPInfo->GetParticipatorNum(dwID, dwRoleID);
		}

		MCREATE_MSG(pSend, sizeof(tagNS_Getparticipators)+(nParticipatorNum-1)*sizeof(DWORD), NS_Getparticipators);

		pSend->nGroupPurchaseKey = dwRoleID;
		pSend->nGroupPurchaseKey = (pSend->nGroupPurchaseKey << 32) | dwID;
		pSend->nParticipatorNum = nParticipatorNum;

		if (nParticipatorNum > 0)
		{
			// 此时也保证了pMapGPInfo的合法性
			pSend->dwErrorCode = dwErrorCode = pMapGPInfo->GetParticipators(dwID, dwRoleID, pSend->dwParticipators);
		}
		else
		{
			pSend->dwErrorCode = dwErrorCode = E_GroupPurchase_NoInfo;
		}

		pRole->SendMessage(pSend, pSend->dwSize);
	}
	else
	{
		tagNS_Getparticipators send;
		send.dwErrorCode = dwErrorCode;
		send.nParticipatorNum = 0;
		send.nGroupPurchaseKey = dwRoleID;
		send.nGroupPurchaseKey = (send.nGroupPurchaseKey << 32) | dwID;
		pRole->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 更新团购信息
//-----------------------------------------------------------------------------
VOID Mall::Update()
{
	CHECK_TIME();

	// 商城关闭时停止计时
	if(!IsInitOK())
	{
		return;
	}

	// 计时更新(每秒更新)
	if (m_dwTimeKeeper == g_world.GetWorldTime())
		return;

	DWORD dwSecTime = g_world.GetWorldTime() - m_dwTimeKeeper;
	m_dwTimeKeeper = g_world.GetWorldTime();

	DWORD dwGuildID = GT_INVALID;
	GuildPurchase* pMapGPInfo = NULL;

	INT64 nMapKey = GT_INVALID;
	tagGroupPurchase *pGPInfo = NULL;

	TMap<DWORD, GuildPurchase*>::TMapIterator iter = m_mapGuildPurchase.Begin();
	while (m_mapGuildPurchase.PeekNext(iter, dwGuildID, pMapGPInfo))
	{
		if (!P_VALID(pMapGPInfo))
		{
			m_mapGuildPurchase.Erase(dwGuildID);
			continue;
		}

		// 每个帮派更新
		pMapGPInfo->Update(dwSecTime);

		// 帮派团购信息无效
		if (pMapGPInfo->IsEmpty())
		{
			m_mapGuildPurchase.Erase(dwGuildID);
			SAFE_DEL(pMapGPInfo);
			continue;
		}
	}

	// 通知数据库更新所有数据的剩余时间(每分钟更新)
	// World和DB之间有最大一分钟的误差，即数据库中可能存在剩余时间小于0的信息
	// 此类信息如果读入World将在下一个Tick删除
	if (++m_byMinuteTime > 60)
	{
		m_byMinuteTime = 0;

		tagNDBC_UpdateGPTime send;
		g_dbSession.Send(&send, send.dwSize);
	}
}

//-----------------------------------------------------------------------------
// 兑换商城物品
//-----------------------------------------------------------------------------
DWORD Mall::ExchangeItem( Role *pRole, DWORD dwCmdID, DWORD dwID, INT nIndex, 
						 INT nPrice, INT16 n16BuyNum, OUT tagMallItemSell &itemSell )
{
	ASSERT(P_VALID(pRole));
	ASSERT(IsInitOK());
	ASSERT(nPrice > 0 && n16BuyNum > 0);

	// 检查索引合法性
	if(nIndex >= GetItemNum())
	{
		return GT_INVALID;
	}

	const tagMallItemProto *pProto = m_pMallItem[nIndex].pMallItem;

	// id
	if(pProto->dwID != dwID)
	{
		return E_Mall_ID_Error;
	}

	// 个数
	if(m_pMallItem[nIndex].nCurNum != GT_INVALID && m_pMallItem[nIndex].nCurNum < n16BuyNum)
	{
		// 返回当前剩余数量
		itemSell.byRemainNum = m_pMallItem[nIndex].nCurNum;
		return E_Mall_Item_NotEnough;
	}

	INT nExVolume = (INT)pProto->nExNum;

	if (nExVolume == GT_INVALID)
	{
		// 该商品不可兑换
		return E_Mall_Exchange_NotAllowable;
	}

	// 赠点
	if(nPrice != nExVolume)
	{
		return E_Mall_ExVolume_Error;
	}

	// 计算总价
	nExVolume *= n16BuyNum;

	// 检查玩家赠点是否足够
	if(nExVolume > pRole->GetCurMgr().GetExchangeVolume() || nExVolume <= 0)
	{
		return E_ExVolume_NotEnough;
	}

	// 创建物品
	tagItem *pItemNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, pProto->dwTypeID, n16BuyNum, EIQ_White);
	if(!P_VALID(pItemNew))
	{
		ASSERT(P_VALID(pItemNew));
		return E_Mall_CreateItem_Failed;
	}

	if (P_VALID(pItemNew->pProtoType) && pItemNew->pProtoType->eSpecFunc != EISF_ZhanDouFu)
		pItemNew->dw1stGainTime = g_world.GetWorldTime();

	// 如果有赠品，则生成赠品
	tagItem *pPresentNew = NULL;
	// Jason 2010-3-15 v1.5.0屏蔽玩家获得的赠品和增点
	//if(pProto->dwPresentID != GT_INVALID)
	//{
	//	pPresentNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, 
	//		pProto->dwPresentID, (INT16)pProto->byPresentNum*n16BuyNum, EIQ_White);
	//	if(!P_VALID(pPresentNew))
	//	{
	//		::Destroy(pItemNew);
	//		ASSERT(P_VALID(pPresentNew));
	//		return E_Mall_CreatePres_Failed;
	//	}

	//	pPresentNew->dw1stGainTime = g_world.GetWorldTime();
	//}

	// 回馈玩家赠点
	//if (pProto->byExAssign >= 0)
	//{
		itemSell.nExVolumeAssign = 0;//pProto->byExAssign * n16BuyNum;
	//}

	// 更新商店中物品个数
	if(m_pMallItem[nIndex].nCurNum != GT_INVALID)
	{
		m_pMallItem[nIndex].nCurNum -= n16BuyNum;
	}

	// 扣除玩家的赠点
	pRole->GetCurMgr().DecExchangeVolume(nExVolume, /*dwCmdID*/ELCLD_MALL_GiftPointCosted);

	// log
	LogMallSell(pRole->GetID(), pRole->GetID(), *pItemNew, 
		pItemNew->n64Serial, n16BuyNum, pItemNew->dw1stGainTime, 0, nExVolume,0, dwCmdID);

	// 设置传出参数
	itemSell.pItem			= pItemNew;
	itemSell.pPresent		= pPresentNew;
	itemSell.nYuanBaoNeed	= nExVolume;
	itemSell.byRemainNum	= m_pMallItem[nIndex].nCurNum;

	return E_Success;
}

//-----------------------------------------------------------------------------
// 兑换商城打包物品
//-----------------------------------------------------------------------------
DWORD Mall::ExchangePack( Role *pRole, DWORD dwCmdID, DWORD dwID, INT nIndex, 
						 INT nPrice, OUT tagMallPackSell &packSell )
{
	ASSERT(P_VALID(pRole));
	ASSERT(IsInitOK());
	ASSERT(nPrice > 0);

	// 检查索引合法性
	if(nIndex >= GetPackNum())
	{
		return GT_INVALID;
	}

	const tagMallPackProto *pProto = m_pMallPack[nIndex].pMallPack;

	// id
	if(pProto->dwID != dwID)
	{
		return E_Mall_ID_Error;
	}

	// 个数
	if(m_pMallPack[nIndex].nCurNum != GT_INVALID && m_pMallPack[nIndex].nCurNum < 1)
	{
		return E_Mall_Pack_NotEnough;
	}

	INT nExVolume = (INT)pProto->nExNum;
	if (nExVolume == GT_INVALID)
	{
		return E_Mall_Exchange_NotAllowable;
	}

	// 赠点
	if(nPrice != nExVolume)
	{
		return E_Mall_ExVolume_Error;
	}

	// 检查玩家赠点是否足够
	if(nExVolume > pRole->GetCurMgr().GetExchangeVolume() || nExVolume <= 0)
	{
		return E_ExVolume_NotEnough;
	}

	// 检查玩家背包空间
	if(pProto->n8ItemKind > pRole->GetItemMgr().GetBagFreeSize())
	{
		return E_Bag_NotEnoughSpace;
	}

	// 如果有赠品，则先生成赠品 -- 方便生成物品失败时，内存释放处理
	tagItem *pPresentNew = NULL;
	// Jason 2010-3-15 v1.5.0屏蔽玩家获得的赠品和增点
	//if(pProto->dwPresentID != GT_INVALID)
	//{
	//	pPresentNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, 
	//		pProto->dwPresentID, pProto->byPresentNum, EIQ_White);
	//	if(!P_VALID(pPresentNew))
	//	{
	//		ASSERT(P_VALID(pPresentNew));
	//		IMSG(_T("\n\nCaution:\n"));
	//		IMSG(_T("\tThere is critical error in proto of mall pack or item&equip!!!!!!!!!\n\n"));
	//		return E_Mall_CreatePres_Failed;
	//	}
	//}

	// 创建物品
	INT i = 0;
	for(; i<pProto->n8ItemKind; ++i)
	{
		tagItem *pItemNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, pProto->dwTypeID[i], pProto->byItemNum[i], EIQ_White);
		if(!P_VALID(pItemNew))
		{
			ASSERT(P_VALID(pItemNew));
			IMSG(_T("\n\nCaution:\n"));
			IMSG(_T("\tThere is critical error in proto of mall pack or item&equip!!!!!!!!!\n\n"));
			break;
		}
		else
		{
			packSell.pItem[i] = pItemNew;
		}
	}

	// 检查物品是否均创建成功
	if(i != pProto->n8ItemKind)
	{
		// 是否已创建成功物品内存
		for(INT j=0; j<i; ++j)
		{
			::Destroy(packSell.pItem[j]);
		}

		// 赠品
		if(P_VALID(pPresentNew))
		{
			::Destroy(pPresentNew);
		}

		return E_Mall_CreateItem_Failed;
	}

	// 回馈玩家赠点
	//if (pProto->byExAssign >= 0)
	//{
		packSell.nExVolumeAssign = 0;//pProto->byExAssign;
	//}

	// 更新商店中物品个数
	if(m_pMallPack[nIndex].nCurNum != GT_INVALID)
	{
		--m_pMallPack[nIndex].nCurNum;
	}

	// 扣除玩家赠点
	pRole->GetCurMgr().DecExchangeVolume(nExVolume, /*dwCmdID*/ELCLD_MALL_GiftPointCosted);

	// 记录到第一个物品上
	LogMallSell(pRole->GetID(), pRole->GetID(), *packSell.pItem[0], 
		packSell.pItem[0]->n64Serial, packSell.pItem[0]->GetNum(), 
		packSell.pItem[0]->dw1stGainTime, 0, nExVolume,0 ,dwCmdID);

	// 设置传出参数
	packSell.pPresent		= pPresentNew;
	packSell.nYuanBaoNeed	= nExVolume;
	packSell.byRemainNum	= m_pMallPack[nIndex].nCurNum;

	return E_Success;
}

//-----------------------------------------------------------------------------
// 获取商城物品信息
//-----------------------------------------------------------------------------
const tagMallGoods* Mall::GetMallItem( BYTE byIndex, EMallItemType eType /*= EMIT_Item*/ )
{
	tagMallGoods* pMallGoods = NULL;

	switch (eType)
	{
	case EMIT_Item:
		if (byIndex < GetItemNum())
		{
			pMallGoods = &m_pMallItem[byIndex];
		}
		break;

	case EMIT_Pack:
		if (byIndex < GetPackNum())
		{
			pMallGoods = &m_pMallPack[byIndex];
		}
		break;

	case EMIT_FreeItem:
		if (GetFreeItemNum() != 0)
		{
			pMallGoods = m_pMallFreeItem;
		}
		break;
	}

	return pMallGoods;
}

//-----------------------------------------------------------------------------
// 删除帮派团购信息(帮派解散)
//-----------------------------------------------------------------------------
VOID Mall::RemoveGuildPurchaseInfo( DWORD dwGuildID )
{
	if (!GT_VALID(dwGuildID))
	{
		return;
	}

	GuildPurchase* pGPInfo = m_mapGuildPurchase.Peek(dwGuildID);
	if (!P_VALID(pGPInfo))
	{
		return;
	}

	pGPInfo->RemoveGroupPurchaseInfo();
}

//-----------------------------------------------------------------------------
// 向db发消息，要求记录log
//-----------------------------------------------------------------------------
VOID Mall::LogMallSell(DWORD dwBuyRoleID, DWORD dwToRoleID, const tagItem& item, 
					   INT64 n64Serial, INT16 n16Num, DWORD dwFstGainTime, 
					   INT nCostYuanBao, INT nCostExVolume,INT nCostVIPPoint, DWORD dwCmdID)
{
	tagNDBC_LogMallSell send;

	send.sLogMallSell.dwTypeID		= item.dwTypeID;
	send.sLogMallSell.dwExistTime	= item.pProtoType->dwTimeLimit;
	send.sLogMallSell.nMaxUseTimes	= item.pProtoType->nMaxUseTimes;

	send.sLogMallSell.dwRoleID		= dwBuyRoleID;
	send.sLogMallSell.dwToRoleID	= dwToRoleID;
	send.sLogMallSell.n64Serial		= n64Serial;
	send.sLogMallSell.dwFstGainTime	= dwFstGainTime;
	send.sLogMallSell.nCostYuanBao	= nCostYuanBao;
	send.sLogMallSell.nCostExVolume	= nCostExVolume;
	send.sLogMallSell.nCostVIPPoint = nCostVIPPoint;
	send.sLogMallSell.dwCmdID		= dwCmdID;
	send.sLogMallSell.n16NumSell	= n16Num;

	tagRoleInfo* pBuyerInfo = g_roleMgr.GetRoleInfo(dwBuyRoleID);
	if (P_VALID(pBuyerInfo))
	{
		send.sLogMallSell.dwAccountID = pBuyerInfo->dwAccountID;
	}

	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// 向db发消息，要求记录礼品包log
//-----------------------------------------------------------------------------
VOID Mall::LogMallSellPack(DWORD dwPackID, DWORD dwBuyRoleID, DWORD dwToRoleID, INT nCostYuanBao)
{
	tagNDBC_LogMallSellPack send;
	send.sLogMallSellPack.dwPackID		= dwPackID;
	send.sLogMallSellPack.dwBuyRoleID	= dwBuyRoleID;
	send.sLogMallSellPack.dwToRoleID	= dwToRoleID;
	send.sLogMallSellPack.nCostYuanBao	= nCostYuanBao;

	tagRoleInfo* pBuyerInfo = g_roleMgr.GetRoleInfo(dwBuyRoleID);
	if (P_VALID(pBuyerInfo))
	{
		send.sLogMallSellPack.dwAccountID = pBuyerInfo->dwAccountID;
	}

	g_dbSession.Send(&send, send.dwSize);
}

BOOL Mall::CanUseYuanBaoDaiBi(Role* pRole, INT nPrice)
{
	//扣道具
	TList<tagItem*> tmpList;
	INT nCount = pRole->GetItemMgr().GetBagSameItemList(tmpList, YuanBaoDaiBi, nPrice);
	if(nCount < nPrice)
		return FALSE;

	pRole->GetItemMgr().DelBagSameItem(tmpList, nPrice, ELCID_MallDaiBi);
	return TRUE;
}