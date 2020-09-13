//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_mall.cpp
// author: Sxg
// actor:
// data: 2009-2-1
// last:
// brief: 商城处理
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/msg_mall.h"
#include "../WorldDefine/mall_define.h"
#include "../ServerDefine/msg_yuanbao.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "role.h"
#include "mall.h"
#include "role_mgr.h"
#include "TradeYuanBao.h"
#include "guild.h"
#include "guild_mgr.h"
#include "world.h"
#include "item_creator.h"

//-----------------------------------------------------------------------------
// 获取商城中所有商品
//-----------------------------------------------------------------------------
DWORD Role::GetMallAll(OUT DWORD &dwMallTime)
{
	// 判断商城是否开放
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// 获取服务器商城加载时间
	dwMallTime = g_mall.GetMallTime();

	INT nGoodsNum, nSzMsg;

	// 普通商品
	nGoodsNum = g_mall.GetItemNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallItem) - 1 + sizeof(tagMallItemProto) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallItem);
		pSend->nItemNum = nGoodsNum;
		g_mall.GetAllItems(pSend->byData);

		SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}

	// 礼品包
	nGoodsNum = g_mall.GetPackNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallPack) - 1 + sizeof(tagMallPackProto) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallPack);
		pSend->nPackNum = nGoodsNum;
		g_mall.GetAllPacks(pSend->byData);

		SendMessage(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}
	
	// 免费领取商品(只有1个)
	nGoodsNum = g_mall.GetFreeItemNum();
	if(nGoodsNum > 0)
	{
		tagNS_MallFreeItem send;
		g_mall.GetFreeItem(&send.freeItem);
		SendMessage(&send, send.dwSize);
	}

	//　推荐商品
    nGoodsNum = g_attRes.GetMallRecommendingItemNum();
	if ( nGoodsNum > 0)
	{
		 nSzMsg = sizeof(tagNS_MallRecommendingItem) - 1 + sizeof(tagRecommendingItemInfo)*nGoodsNum;
		 MCREATE_MSG(pSend,nSzMsg,NS_MallRecommendingItem);
		 pSend->nItemNum = nGoodsNum;

		 TMap<DWORD, tagRecommendingItemInfo*> mapRecommendingItemInfo = g_attRes.GetMallRecommendingItem();
         tagRecommendingItemInfo* pRecommendingItemInfo = NULL;
		 INT nNum = 0 ;
		 MTRANS_POINTER(p, pSend->byData, tagRecommendingItemInfo);
		 TMap<DWORD, tagRecommendingItemInfo*>::TMapIterator it = mapRecommendingItemInfo.Begin();
		 while (mapRecommendingItemInfo.PeekNext(it,pRecommendingItemInfo))
		 {

			 if( P_VALID(pRecommendingItemInfo) )
			 {
                 memcpy(&p[nNum],pRecommendingItemInfo,sizeof(tagRecommendingItemInfo));
				 nNum++;
			 }
             if ( nNum >= nGoodsNum)
             {
				 break;
             }
		 }
		 SendMessage(pSend,pSend->dwSize);
		 MDEL_MSG(pSend);
		

	}

	tagNS_MallUpdateCfg sendMallCfg;
	for (INT i = 0; i < EMCA_End; i++)
	{
		sendMallCfg.dwMallCfg[i] = g_attRes.GetMallCfg((EMallCfgAttr)i);
	}

	SendMessage(&sendMallCfg, sendMallCfg.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 更新商城中有出售个数限制的所有商品个数
//-----------------------------------------------------------------------------
DWORD Role::UpdateMallAll(OUT DWORD &dwNewMallTime, IN DWORD dwOldMallTime)
{
	// 判断商城是否开放
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// 获取服务器商城加载时间
	dwNewMallTime = g_mall.GetMallTime();

	//-- 检查客户端的商城静态属性是否与服务器相同

	// 1.不同：重新发送商店原型信息
	if(dwNewMallTime != dwOldMallTime)
	{
		return GetMallAll(dwNewMallTime);
	}

	// 2.相同：只刷新有个数限制的商品个数信息
	INT nGoodsNum, nSzMsg;

	// 普通商品
	nGoodsNum = g_mall.GetItemNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);
		g_mall.UpdateAllItems(pSend->byData, pSend->nItemNum);

		// 重新计算消息大小
		if(pSend->nItemNum > 0)
		{
			pSend->dwSize = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate) * pSend->nItemNum;
			SendMessage(pSend, pSend->dwSize);
		}

		MDEL_MSG(pSend);
	}

	// 礼品包
	nGoodsNum = g_mall.GetPackNum();
	if(nGoodsNum > 0)
	{
		nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate) * nGoodsNum;
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);
		g_mall.UpdateAllPacks(pSend->byData, pSend->nItemNum);

		// 重新计算消息大小
		if(pSend->nItemNum > 0)
		{
			pSend->dwSize = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate) * pSend->nItemNum;
			SendMessage(pSend, pSend->dwSize);
		}

		MDEL_MSG(pSend);
	}

	tagNS_MallUpdateCfg sendMallCfg;
	for (INT i = 0; i < EMCA_End; i++)
	{
		sendMallCfg.dwMallCfg[i] = g_attRes.GetMallCfg((EMallCfgAttr)i);
	}

	SendMessage(&sendMallCfg, sendMallCfg.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 购买商城物品
//-----------------------------------------------------------------------------
DWORD Role::BuyMallItem(DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex,EShoppingDistrict eShoppingDistrict, INT16 n16OfferItemLvl, ELogCmdID eCmdID)
{
	// 判断商城是否开放
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// 行囊是否解锁
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// 预检查背包中是否有空位
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallItemSell sItemSell;

	// 商城中商品相关检查
	DWORD dwErrorCode = g_mall.SellItem(this, GetID(), eCmdID, 
										dwID, nIndex, nUnitPrice, n16BuyNum, sItemSell,eShoppingDistrict, n16OfferItemLvl);

	// 处理结果
	if(E_Success == dwErrorCode	&& sItemSell.nYuanBaoNeed > 0 && P_VALID(sItemSell.pItem))
	{
		// 元宝已在商城中扣除
		
		// 将物品放到玩家背包中
		GetItemMgr().Add2Bag(sItemSell.pItem, ELCID_Mall_BuyItem, TRUE);
		
		// 如果有赠品，则放到百宝贷中
		if(P_VALID(sItemSell.pPresent))
		{
			// 百宝袋历史记录
			GetItemMgr().ProcBaiBaoRecord(sItemSell.pPresent->dwTypeID, 
							GetNameID(), GT_INVALID, EBBRT_Mall, sItemSell.pPresent->dw1stGainTime);

			GetItemMgr().Add2BaiBao(sItemSell.pPresent, ELCID_Mall_BuyItem_Add);

		}

		// 回馈玩家赠点
		if (sItemSell.nVipPoint > 0)
		{
			GetCurMgr().IncVIPPoint(sItemSell.nVipPoint, ELCID_Mall_BuyItem);
		}
	}

	// 玩家获得一件道具的事件接口
	if (P_VALID(sItemSell.pItem))
	{
		g_ScriptMgr.GetRoleScript()->OnObtainItem(this, sItemSell.pItem->dwTypeID, sItemSell.pItem->eCreateMode);
	}

	// 发送更新后商店物品 -- 只有刷新物品要更新物品个数
	if((E_Success == dwErrorCode || E_Mall_Item_NotEnough == dwErrorCode) 
		&& sItemSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);
		
		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sItemSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 购买商城礼品包
//-----------------------------------------------------------------------------
DWORD Role::BuyMallPack(DWORD dwID, INT nUnitPrice, BYTE byIndex,EShoppingDistrict eShoppingDistrict)
{
	// 判断商城是否开放
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// 行囊是否解锁
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// 预检查背包中是否有空位
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallPackSell sPackSell;

	// 商城中商品相关检查
	DWORD dwErrorCode = g_mall.SellPack(this, GetID(), ELCID_Mall_BuyPack, 
										dwID, byIndex, nUnitPrice, sPackSell, TRUE,eShoppingDistrict);

	// 处理结果
	if(E_Success == dwErrorCode	&& sPackSell.nYuanBaoNeed > 0 && P_VALID(sPackSell.pItem[0]))
	{
		// 将物品放到玩家背包中
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			if(!P_VALID(sPackSell.pItem[i]))
			{
				break;
			}

			GetItemMgr().Add2Bag(sPackSell.pItem[i], ELCID_Mall_BuyPack, TRUE);
		}

		// 如果有赠品，则放到百宝贷中
		if(P_VALID(sPackSell.pPresent))
		{
			// 百宝袋历史记录
			GetItemMgr().ProcBaiBaoRecord(sPackSell.pPresent->dwTypeID, 
								GetNameID(), GT_INVALID, EBBRT_Mall, sPackSell.pPresent->dw1stGainTime);

			GetItemMgr().Add2BaiBao(sPackSell.pPresent, ELCID_Mall_BuyPack_Add);

		}

		// 回馈玩家赠点
		if (sPackSell.nVipPoint > 0)
		{
			GetCurMgr().IncVIPPoint(sPackSell.nVipPoint, ELCID_Mall_BuyPack);
		}
	}

	// 发送更新后商店物品 -- 只有刷新物品要更新物品个数
	if((E_Success == dwErrorCode || E_Mall_Pack_NotEnough == dwErrorCode) 
		&& sPackSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sPackSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 赠送商城物品给好友
//-----------------------------------------------------------------------------
DWORD Role::BuyMallItem(DWORD dwTgtRoleID, LPCTSTR szLeaveWord, 
						DWORD dwID, INT nUnitPrice, INT16 n16BuyNum, INT nIndex,EShoppingDistrict eShoppingDistrict)
{
	// 判断商城是否开放
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// 行囊是否解锁
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// 检查好友ID在游戏世界中是否存在
	if(!g_roleMgr.IsRoleBelongToWorld(dwTgtRoleID))
	{
		return E_Role_Not_ExistIn_World;
	}

	tagMallItemSell sItemSell;

	// 商城中商品相关检查
	DWORD dwErrorCode = g_mall.SellItem(this, dwTgtRoleID, ELCID_Mall_PresentItem, 
										dwID, nIndex, nUnitPrice, n16BuyNum, sItemSell,eShoppingDistrict, 0);

	// 处理结果
	if(E_Success == dwErrorCode	&& sItemSell.nYuanBaoNeed > 0 && P_VALID(sItemSell.pItem))
	{
		// 百宝袋历史记录
		GetItemMgr().ProcBaiBaoRecord(sItemSell.pItem->dwTypeID, dwTgtRoleID, 
							GetNameID(), EBBRT_Friend, sItemSell.pItem->dw1stGainTime, szLeaveWord);

		// 将物品放到好友百宝袋中
		Role *pFriend = g_roleMgr.GetRolePtrByID(dwTgtRoleID);
		if(P_VALID(pFriend))
		{
			pFriend->GetItemMgr().Add2BaiBao(sItemSell.pItem, ELCID_Mall_PresentItem, FALSE, GetID());
		}
		else
		{
			// 存储到item_baibao表中
			ItemMgr::InsertBaiBao2DB(sItemSell.pItem, dwTgtRoleID, ELCID_Mall_PresentItem);
			
			// 删除物品
			::Destroy(sItemSell.pItem);
		}

		// 如果有赠品，则放到好友百宝袋中
		if(P_VALID(sItemSell.pPresent))
		{
			// 百宝袋历史记录
			GetItemMgr().ProcBaiBaoRecord(sItemSell.pPresent->dwTypeID, dwTgtRoleID, 
										GetNameID(), EBBRT_Mall, sItemSell.pPresent->dw1stGainTime);

			if(P_VALID(pFriend))
			{
				pFriend->GetItemMgr().Add2BaiBao(sItemSell.pPresent, ELCID_Mall_PresentItem_Add);
			}
			else
			{
				// 存储到item_baibao表中
				ItemMgr::InsertBaiBao2DB(sItemSell.pPresent, dwTgtRoleID, ELCID_Mall_PresentItem_Add);

				// 删除物品
				::Destroy(sItemSell.pPresent);
			}
		}
		
		// 向买家回馈赠点
		if (sItemSell.nVipPoint > 0)
		{
			GetCurMgr().IncVIPPoint(sItemSell.nVipPoint, ELCID_Mall_PresentItem);
		}
	}

	// 发送更新后商店物品 -- 只有刷新物品要更新物品个数
	if((E_Success == dwErrorCode || E_Mall_Item_NotEnough == dwErrorCode) 
		&& sItemSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sItemSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 赠送商品礼品包给好友
//-----------------------------------------------------------------------------
DWORD Role::BuyMallPack(DWORD dwTgtRoleID, LPCTSTR szLeaveWord, 
						DWORD dwID, INT nUnitPrice, BYTE byIndex,EShoppingDistrict eShoppingDistrict)
{
	// 判断商城是否开放
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// 行囊是否解锁
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// 检查好友ID在游戏世界中是否存在
	if(!g_roleMgr.IsRoleBelongToWorld(dwTgtRoleID))
	{
		return E_Role_Not_ExistIn_World;
	}

	tagMallPackSell sPackSell;

	// 商城中商品相关检查
	DWORD dwErrorCode = g_mall.SellPack(this, dwTgtRoleID, ELCID_Mall_PresentPack, 
										dwID, byIndex, nUnitPrice, sPackSell, FALSE,eShoppingDistrict);

	// 处理结果
	if(E_Success == dwErrorCode	&& sPackSell.nYuanBaoNeed > 0 && P_VALID(sPackSell.pItem[0]))
	{		
		Role *pFriend = g_roleMgr.GetRolePtrByID(dwTgtRoleID);

		// 将物品放到好友百宝袋中 -- item_baibao表
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			if(!P_VALID(sPackSell.pItem[i]))
			{
				break;
			}

			// 记录到第一个物品上
			if(0 == i)
			{
				// 百宝袋历史记录
				GetItemMgr().ProcBaiBaoRecord(sPackSell.pItem[i]->dwTypeID, 
					dwTgtRoleID, GetNameID(), EBBRT_Friend, sPackSell.pItem[i]->dw1stGainTime, szLeaveWord);
			}
			else
			{
				// 百宝袋历史记录(无留言)
				GetItemMgr().ProcBaiBaoRecord(sPackSell.pItem[i]->dwTypeID, 
					dwTgtRoleID, GetNameID(), EBBRT_Friend, sPackSell.pItem[i]->dw1stGainTime);
			}

			if(P_VALID(pFriend))
			{
				pFriend->GetItemMgr().Add2BaiBao(sPackSell.pItem[i], ELCID_Mall_PresentPack, FALSE, GetID());
			}
			else
			{
				// 存储到item_baibao表中
				ItemMgr::InsertBaiBao2DB(sPackSell.pItem[i], dwTgtRoleID, ELCID_Mall_PresentPack);

				// 删除物品
				::Destroy(sPackSell.pItem[i]);
			}
		}

		// 如果有赠品，则放到好友百宝贷中
		if(P_VALID(sPackSell.pPresent))
		{
			// 百宝袋历史记录
			GetItemMgr().ProcBaiBaoRecord(sPackSell.pPresent->dwTypeID, 
							dwTgtRoleID, GetNameID(), EBBRT_Mall, sPackSell.pPresent->dw1stGainTime);

			if(P_VALID(pFriend))
			{
				pFriend->GetItemMgr().Add2BaiBao(sPackSell.pPresent, ELCID_Mall_PresentPack_Add, FALSE, GetID());
			}
			else
			{
				// 存储到item_baibao表中
				ItemMgr::InsertBaiBao2DB(sPackSell.pPresent, dwTgtRoleID, ELCID_Mall_PresentPack_Add);

				// 删除物品
				::Destroy(sPackSell.pPresent);
			}
		}

		// 向买家回馈赠点
		if (sPackSell.nVipPoint > 0)
		{
			GetCurMgr().IncVIPPoint(sPackSell.nVipPoint, ELCID_Mall_PresentPack);
		}
	}

	// 发送更新后商店物品 -- 只有刷新物品要更新物品个数
	if((E_Success == dwErrorCode || E_Mall_Pack_NotEnough == dwErrorCode) 
		&& sPackSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sPackSell.byRemainNum;
		p->dwID			= dwID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 索取商城免费物品
//-----------------------------------------------------------------------------
DWORD Role::GetMallFreeItem(DWORD dwID)
{
	// 判断商城是否开放
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// 行囊是否解锁
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}
	
	// 预检查背包中是否有空位
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallItemSell sItemSell;

	// 商城中商品相关检查
	DWORD dwErrorCode = g_mall.GrantFreeItem(this, dwID, sItemSell);

	// 处理结果

	if(E_Success == dwErrorCode && P_VALID(sItemSell.pItem))
	{
		// 将物品放到背包中
		GetItemMgr().Add2Bag(sItemSell.pItem, ELCID_Mall_FreeItem, TRUE);
	}
	
	return dwErrorCode;
}

DWORD Role::BuyMallCart(DWORD* dwIDArray, INT* nIndexArray, INT16* n16BuyNumArray)
{
	if (!P_VALID(dwIDArray) || !P_VALID(nIndexArray) || !P_VALID(n16BuyNumArray))
	{
		return GT_INVALID;
	}

	DWORD dwPrice = 0;
	BYTE byCartNum = 0;	// 购物车物品数量

	for (INT i = 0; i < MAX_MALL_CART_NUM; i++)
	{
		if (!P_VALID(dwIDArray[i]))
		{
			break;
		}

		byCartNum++;
	}
	DWORD dwRet = g_mall.CanSellCart(this, byCartNum, dwIDArray, nIndexArray, n16BuyNumArray, dwPrice);

	if (E_Success != dwRet)
	{
		return dwRet;
	}

	for (INT i = 0; i < byCartNum; i++)
	{
		if (i >= MAX_MALL_CART_NUM)
		{
			return GT_INVALID;
		}

		dwRet = BuyMallItem(dwIDArray[i], 0, n16BuyNumArray[i], nIndexArray[i], Shopping_Item, GT_INVALID, ELCID_Mall_CartBuy);
		if (E_Success != dwRet)
		{
			return dwRet;
		}
	}

	if (TRUE == g_mall.IsInActivity() && E_Success == dwRet)
	{
		// 商城活动奖励 [6/14/2012 zhangzhihua]
		INT nTimes = dwPrice / g_attRes.GetMallCfg(EMCA_BuyActivityRewardLine);

		for (INT i = 0; i <= (EMCA_BuyActivityRewardItem3 - EMCA_BuyActivityRewardItem0); i++)
		{
			DWORD dwItemTypeID = g_attRes.GetMallCfg((EMallCfgAttr)(i + EMCA_BuyActivityRewardItem0));
			INT nItemNum = g_attRes.GetMallCfg((EMallCfgAttr)(i + EMCA_BuyActivityRewardItemNum0));
			if (P_VALID(dwItemTypeID) && nItemNum > 0)
			{
				// 创建物品
				INT nTotalNum = nTimes * nItemNum;
				while (nTotalNum > 0)
				{
					tagItem *pItemNew = ItemCreator::CreateEx(EICM_Mall, GT_INVALID, dwItemTypeID, nTotalNum, EIQ_White);
					if(!P_VALID(pItemNew))
					{
						ASSERT(P_VALID(pItemNew));
						return E_Mall_CreateItem_Failed;
					}

					nTotalNum -= pItemNew->pProtoType->GetMaxLapNum();

					if (P_VALID(pItemNew->pProtoType) && pItemNew->pProtoType->eSpecFunc != EISF_ZhanDouFu)
						pItemNew->dw1stGainTime	 = g_world.GetWorldTime();
					// 百宝袋历史记录
					GetItemMgr().ProcBaiBaoRecord(pItemNew->dwTypeID, 
						GetNameID(), GT_INVALID, EBBRT_Mall, pItemNew->dw1stGainTime);

					GetItemMgr().Add2BaiBao(pItemNew, ELCID_MALL_Activity_Add);
				}
			}
		}
	}

	return dwRet;
}

//-----------------------------------------------------------------------------
// 兑换商城物品
//-----------------------------------------------------------------------------
DWORD Role::MallItemExchange(DWORD dwMallID, INT nPrice, INT16 n16BuyNum, INT nIndex)
{
	// 判断商城是否开放
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// 行囊是否解锁
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// 预检查背包中是否有空位
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallItemSell sItemSell;

	// 商城中商品相关检查
	DWORD dwErrorCode = g_mall.ExchangeItem(this, ELCLD_Mall_ExchangeItem, dwMallID, nIndex, nPrice, n16BuyNum, sItemSell);

	// 处理结果
	if(E_Success == dwErrorCode	&& sItemSell.nYuanBaoNeed > 0 && P_VALID(sItemSell.pItem))
	{
		// 将物品放到玩家背包中
		GetItemMgr().Add2Bag(sItemSell.pItem, ELCLD_Mall_ExchangeItem, TRUE);

		// 如果有赠品，则放到百宝贷中
		if(P_VALID(sItemSell.pPresent))
		{
			// 百宝袋历史记录
			GetItemMgr().ProcBaiBaoRecord(sItemSell.pPresent->dwTypeID, 
				GetNameID(), GT_INVALID, EBBRT_Mall, sItemSell.pPresent->dw1stGainTime);

			GetItemMgr().Add2BaiBao(sItemSell.pPresent, ELCLD_Mall_ExchangeItem_Add);
		}

		// 回馈玩家赠点
		if (sItemSell.nExVolumeAssign > 0)
		{
			GetCurMgr().IncExchangeVolume(sItemSell.nExVolumeAssign, ELCLD_Mall_ExchangeItem);
		}
	}

	// 发送更新后商店物品 -- 只有刷新物品要更新物品个数
	if((E_Success == dwErrorCode || E_Mall_Item_NotEnough == dwErrorCode) 
		&& sItemSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdateItem) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdateItem);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sItemSell.byRemainNum;
		p->dwID			= dwMallID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;

}

//-----------------------------------------------------------------------------
// 兑换商城打包物品
//-----------------------------------------------------------------------------
DWORD Role::MallPackExchange(DWORD dwMallID, INT nPrice, INT nIndex)
{
	// 判断商城是否开放
	if(!g_mall.IsInitOK())
	{
		return E_Mall_Close;
	}

	// 行囊是否解锁
	if(!GetRoleStateEx().IsInState(ERSE_BagPsdPass))
	{
		return E_Con_PswNotPass;
	}

	// 预检查背包中是否有空位
	if(GetItemMgr().GetBagFreeSize() < 1)
	{
		return E_Bag_NotEnoughSpace;
	}

	tagMallPackSell sPackSell;

	// 商城中商品相关检查
	DWORD dwErrorCode = g_mall.ExchangePack(this, ELCLD_Mall_ExchangePack, dwMallID, nIndex, nPrice, sPackSell);

	// 处理结果
	if(E_Success == dwErrorCode	&& sPackSell.nYuanBaoNeed > 0 && P_VALID(sPackSell.pItem[0]))
	{
		INT64 n64Serial = sPackSell.pItem[0]->n64Serial;
		INT16 n16BuyNum = sPackSell.pItem[0]->GetNum();
		DWORD dwFstGainTime = g_world.GetWorldTime();

		// 将物品放到玩家背包中
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			if(!P_VALID(sPackSell.pItem[i]))
			{
				break;
			}

			GetItemMgr().Add2Bag(sPackSell.pItem[i], ELCLD_Mall_ExchangePack, TRUE);
		}

		// 如果有赠品，则放到百宝贷中
		if(P_VALID(sPackSell.pPresent))
		{
			// 百宝袋历史记录
			GetItemMgr().ProcBaiBaoRecord(sPackSell.pPresent->dwTypeID, 
				GetNameID(), GT_INVALID, EBBRT_Mall, dwFstGainTime);

			GetItemMgr().Add2BaiBao(sPackSell.pPresent, ELCLD_Mall_ExchangePack_Add);
		}

		// 回馈玩家赠点
		if (sPackSell.nExVolumeAssign > 0)
		{
			GetCurMgr().IncExchangeVolume(sPackSell.nExVolumeAssign, ELCLD_Mall_ExchangePack);
		}
	}

	// 发送更新后商店物品 -- 只有刷新物品要更新物品个数
	if((E_Success == dwErrorCode || E_Mall_Pack_NotEnough == dwErrorCode) 
		&& sPackSell.byRemainNum != (BYTE)GT_INVALID)
	{
		INT nSzMsg = sizeof(tagNS_MallUpdatePack) - 1 + sizeof(tagMallUpdate);
		MCREATE_MSG(pSend, nSzMsg, NS_MallUpdatePack);

		pSend->nItemNum = 1;
		MTRANS_POINTER(p, pSend->byData, tagMallUpdate);
		p->byRemainNum	= sPackSell.byRemainNum;
		p->dwID			= dwMallID;

		SendMessage(pSend, pSend->dwSize);

		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 玩家向元宝交易账户存元宝
//-----------------------------------------------------------------------------
DWORD Role::SaveYB2Account(DWORD dwID, INT nNum)
{
	if( g_world.IsUseIPGSystem() && (!GetGetCash() ))
		return GT_INVALID;

	if( g_world.IsUseIPGSystem() )
	{
		int nCash = GetIPGCash();
		if( nCash < nNum )
			return E_Trade_BagYB_NotEnough;
	}
	else
	{
		// 检查玩家背包元宝数量
		if(GetCurMgr().GetBagYuanBao() < nNum)
			return E_Trade_BagYB_NotEnough;
	}

	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
	{
		pYBAccount = g_tradeYB.CreateTradeAccount(dwID);
		if(!P_VALID(pYBAccount))
			return GT_INVALID;
	}

	// Jason 2010-3-25 防止溢出问题
	if( pYBAccount->YuanBao.GetCur() + nNum > pYBAccount->YuanBao.GetMax() )
		return E_Trade_AccountYB_Full;

	pYBAccount->IncAccountYB(nNum, (DWORD)ELCID_Trade_SaveYuanBao, TRUE);
	

	if( g_world.IsUseIPGSystem())
	{
		if(GetGetCash())
		{
			TCHAR tszTemp[64] = {0};
			wsprintf(tszTemp,_T("DecIPGCash6. Dec cash:%d. \r\n"),nNum);
			g_world.WriteKroeaLog(tszTemp);

			if( !DecIPGCash(nNum))
				return GT_INVALID;
		}
	}
	else
	{
		GetCurMgr().DecBagYuanBao(nNum, (DWORD)ELCID_Trade_SaveYuanBao);
	}
		
	return E_Success;
}

//-----------------------------------------------------------------------------
// 玩家向元宝交易账户存金钱
//-----------------------------------------------------------------------------
DWORD Role::SaveSilver2Account(DWORD dwID, INT64 nNum)
{
	// 检查玩家背包金钱数量
	if(GetCurMgr().GetBagSilver() < nNum)
		return E_Trade_BagSilver_NotEnough;

	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
	{
		pYBAccount = g_tradeYB.CreateTradeAccount(dwID);
		if(!P_VALID(pYBAccount))
			return GT_INVALID;
	}

	// Jason 2010-3-25 防止溢出问题
	if( pYBAccount->Silver.GetCur() + nNum > pYBAccount->Silver.GetMax() )
		return E_Trade_AccountSilver_Full;

	pYBAccount->IncAccountSilver(nNum, (DWORD)ELCID_Trade_SaveSilver, TRUE);
	GetCurMgr().DecBagSilver(nNum, (DWORD)ELCID_Trade_SaveSilver);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 玩家向元宝交易账户取元宝
//-----------------------------------------------------------------------------
DWORD Role::DepositYBAccout(DWORD dwID, INT nNum)
{
	if( g_world.IsUseIPGSystem() && (!GetGetCash() ))
		return GT_INVALID;

	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	// 检查账户元宝数量
	if(pYBAccount->GetAccountYB() < nNum)
		return E_Trade_AccountYB_NotEnough;

	// 检查玩家是否提交过出售订单
	tagYuanBaoOrder *pYBOrder = g_tradeYB.GetYBSellOrder(dwID);
	if(P_VALID(pYBOrder))
		return GT_INVALID;

	// Jason 2010-2-2 防止溢出问题
	if( GetCurMgr  ().GetBagYuanBao  () + nNum > GetCurMgr  ().GetMaxBagYuanBao  () )
		return E_Trade_BagYB_WillBeFull;

	pYBAccount->DecAccountYuanBao(nNum, (DWORD)ELCID_Trade_DepositYuanBao, TRUE);
	GetCurMgr().IncBagYuanBao(nNum, (DWORD)ELCID_Trade_DepositYuanBao);
	
	return E_Success;
}

//-----------------------------------------------------------------------------
// 玩家向元宝交易账户取金钱
//-----------------------------------------------------------------------------
DWORD Role::DepositSilverAccount(DWORD dwID, INT64 nNum)
{
	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	// 检查账户金钱数量
	if(pYBAccount->GetAccountSilver() < nNum)
		return E_Trade_AccountSilver_NotEnough;

	// 检查玩家是否提交过收购订单
	tagYuanBaoOrder *pYBOrder = g_tradeYB.GetYBBuyOrder(dwID);
	if(P_VALID(pYBOrder))
		return GT_INVALID;

	// Jason 2010-2-2 防止溢出问题
	if( GetCurMgr  ().GetBagSilver  () + nNum > GetCurMgr  ().GetMaxBagSilver  () )
		return E_Trade_BagSilver_WillBeFull;


	pYBAccount->DecAccountSilver(nNum, (DWORD)ELCID_Trade_DepositSilver, TRUE);
	GetCurMgr().IncBagSilver(nNum, (DWORD)ELCID_Trade_DepositSilver);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 同步元宝交易初始化信息
//-----------------------------------------------------------------------------
DWORD Role::GetYBTradeInfo()
{
	g_tradeYB.SynBuyPriceList(this);
	g_tradeYB.SynSellPriceList(this);
	g_tradeYB.SynYBAccount(this);
	return E_Success;
}

//-----------------------------------------------------------------------------
// 玩家提交元宝出售订单
//-----------------------------------------------------------------------------
DWORD Role::SubmitSellOrder(DWORD dwRoleID, INT nNum, INT nPrice)
{
	tagYuanBaoAccount* pYBAccount = g_tradeYB.GetYBAccount(dwRoleID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	if(nNum <= 0 || nPrice <= 0)
		return GT_INVALID;

	INT64 n64Total = (INT64)nNum * nPrice;
	INT64 n64Tem = ~0x7FFFFFFF;

	if( n64Total & n64Tem )
		return E_Trade_AmountOfTheTradeTooBig;
		
	// 是否已经提交过出售订单
	tagYuanBaoOrder* pSellOrder = (tagYuanBaoOrder*)GT_INVALID;
	pSellOrder = g_tradeYB.GetYBSellOrder(dwRoleID);
	if(P_VALID(pSellOrder))
		return E_Trade_SellOrder_Exit;

	// 交易账户元宝是否足够
	if(pYBAccount->GetAccountYB() < nNum)
		return E_Trade_AccountYB_NotEnough;

	// Jason 2010-3-16 v1.5.0 补充交易金钱满的判断
	//CurrencyMgr & refCurrency = GetCurMgr();
	//if( refCurrency.GetBagSilver() + nPrice * nNum > refCurrency.GetMaxBagSilver() )
	//	return E_Trade_AccountSilver_WillBeFull;

	// Jason 2010-3-16 v1.5.0提交订单时并不扣除手续费，而是在交易后扣除。
	// 交易手续费为总价的2%
	//INT nTax = nPrice * nNum * 2 / 100;
	//if(nTax < 1)    nTax = 1;

	// 玩家手续费是否足够
	//if (GetCurMgr().GetBagSilver() < nTax)
	//	return E_Trade_Tax_NotEnough;
	if( pYBAccount->GetCanIncAccountSilver() < /*nNum*nPrice*/0 )
		return E_Trade_AccountSilver_Full;


	tagYuanBaoOrder * pYBOrder = g_tradeYB.CreateYBOrder(dwRoleID, EYBOT_SELL, nPrice, nNum);
	if(!P_VALID(pYBOrder))
		return GT_INVALID;

	// 设置账户中订单的提交状态
	pYBAccount->SetSellOrder(TRUE);

	// 扣除手续费 
	//GetCurMgr().DecBagSilver(nTax, ELCID_Trade_Tax);

	// 出售元宝
	g_tradeYB.DealYBSell(pYBOrder);
	
	return E_Success;
}

//-----------------------------------------------------------------------------
// 玩家提交元宝收购订单
//-----------------------------------------------------------------------------
DWORD Role::SubmitBuyOrder(DWORD dwRoleID, INT nNum, INT nPrice)
{
	tagYuanBaoAccount *pYBAccount = g_tradeYB.GetYBAccount(dwRoleID);
	if(!P_VALID(pYBAccount))
		return GT_INVALID;

	if(nNum <= 0 || nPrice <= 0)
		return GT_INVALID;

	INT64 n64Total = (INT64)nNum * nPrice;
	INT64 n64Tem = ~0x7FFFFFFF;

	if( n64Total & n64Tem )
		return E_Trade_AmountOfTheTradeTooBig;

	// 是否已近提交过订单
	tagYuanBaoOrder* pBuyOrder = (tagYuanBaoOrder*)GT_INVALID;
	pBuyOrder = g_tradeYB.GetYBBuyOrder(dwRoleID);
	if(P_VALID(pBuyOrder))
		return E_Trade_BuyOrder_Exit;

	// 交易账户金钱是否足够
	if(pYBAccount->GetAccountSilver() < n64Total)
		return E_Trade_AccountSilver_NotEnough;

	// Jason 2010-3-16 v1.5.0 收购元宝时并不扣除手续费。
	// 交易手续费为总价的2%
	//INT nTax = (FLOAT)(nPrice * nNum) * 0.02f;
	//if(nTax < 1)    nTax = 1;

	// 玩家手续费是否足够
	//if (GetCurMgr().GetBagSilver() < nTax)
	//	return E_Trade_Tax_NotEnough;
	if( pYBAccount->GetCanIncAccountYB() < 0 )
		return E_Trade_AccountYB_Full;

	tagYuanBaoOrder *pYBOrder = g_tradeYB.CreateYBOrder(dwRoleID, EYBOT_BUY, nPrice, nNum);
	if(!P_VALID(pYBOrder))
		return GT_INVALID;

	// 设置账户中订单的提交状态
	pYBAccount->SetBuyOrder(TRUE);

	// 扣除手续费 。
	//GetCurMgr().DecBagSilver(nTax, ELCID_Trade_Tax);

	// 购买元宝
	g_tradeYB.DealYBBuy(pYBOrder);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 删除订单
//-----------------------------------------------------------------------------
DWORD Role::DeleteOrder(DWORD dwRoleID, DWORD dwOrderID, EYBOTYPE eYBOType)
{
	if(eYBOType != EYBOT_BUY && eYBOType != EYBOT_SELL)
		return GT_INVALID;

	tagYuanBaoOrder *pYBOrder = (tagYuanBaoOrder*)GT_INVALID;
	if(eYBOType == EYBOT_BUY)
		pYBOrder = g_tradeYB.GetYBBuyOrder(dwRoleID);
	else
		pYBOrder = g_tradeYB.GetYBSellOrder(dwRoleID);

	if(!P_VALID(pYBOrder))
		return GT_INVALID;

	if(pYBOrder->dwID != dwOrderID)
		return GT_INVALID;

	g_tradeYB.DeleteYBOrder(pYBOrder, EYBOM_Cancel);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 查询一周内该玩家的元宝交易订单
//-----------------------------------------------------------------------------
DWORD Role::GetYBOrder(DWORD dwRoleID)
{
	tagYuanBaoAccount *pYBAccount = g_tradeYB.GetYBAccount(dwRoleID);
	if(!P_VALID(pYBAccount))
		return  GT_INVALID;

	DWORD dwCurTick = g_world.GetWorldTick();
	if(dwCurTick - pYBAccount->GetQuestTick() > 25)
		pYBAccount->SetQuestTick(dwCurTick);
	else
		return GT_INVALID;

	// 向数据库发送查询消息
	tagNDBC_GetRoleYBOrder	sendDB;
	sendDB.dwRoleID = dwRoleID;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return E_Success;
}