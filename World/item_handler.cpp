//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_handler.cpp
// author: Sxg
// actor:
// data: 2008-07-21
// last:
// brief: 客户端和服务器端间消息处理 -- 物品/装备相关
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_item.h"
#include "../WorldDefine/msg_loot.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/msg_common.h"
#include "../ServerDefine/role_data.h"

#include "player_session.h"
#include "world_session.h"
#include "role.h"
#include "world.h"
#include "guild_mgr.h"
#include "role_mgr.h"
#include "item_creator.h"

//-----------------------------------------------------------------------------
// 穿上装备
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_Equip);

	// 消息合法性验证
	if(pRecv->ePosDst <= EEP_Start || pRecv->ePosDst >= EEP_End)
	{
		IMSG(_T("The equip position is invalid!"));
		return GT_INVALID;
	}

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 换装
	DWORD dwErrorCode = pRole->Equip(pRecv->n64Serial, pRecv->ePosDst);

	tagNS_Equip	send;
	send.n64Serial = pRecv->n64Serial;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 脱下装备
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUnequip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_Unequip);
	
	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 换装
	DWORD dwErrorCode = pRole->Unequip(pRecv->n64Serial, pRecv->n16PosDst);
// 	if(E_Success == dwErrorCode)
// 	{
// 		return E_Success;
// 	}

	tagNS_Unequip send;
	send.n64Serial = pRecv->n64Serial;
	send.dwErrorCode = dwErrorCode;
	send.eType = pRecv->eType;

	// 反馈给客户端换装结果
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// 主副手对换
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSwapWeapon(tagNetCmd* pCmd)
{
	//MGET_MSG(pRecv, pCmd, NC_SwapWeapon);

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 换装
	DWORD dwErrorCode = pRole->SwapWeapon();
	if(E_Success == dwErrorCode)
	{
		return E_Success;
	}

	tagNS_SwapWeapon send;
	send.dwErrorCode = dwErrorCode;

	// 反馈给客户端换装结果
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// 鉴定装备
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleIdentifyEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_IdentifyEquip);

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pRole->GetItemMgr().IdentifyEquip(pRecv->n64SerialReel, 
												pRecv->n64SerialEquip, ELCLD_Equip_Identify);
	if(E_Success == dwErrorCode)
	{
		return E_Success;
	}

	tagNS_IdentifyEquip send;
	send.n64Serial = pRecv->n64SerialEquip;
	send.dwErrorCode = dwErrorCode;

	// 反馈给客户端换装结果
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//-----------------------------------------------------------------------------
// 同一容器内移动物品
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleChangeItemPos(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemPosChange);

	if(pRecv->n16Num < 0)
	{
		return GT_INVALID;
	}
	
	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode;
	switch(pRecv->eConType)
	{
	case EICT_Equip:
		return pRole->MoveRing(pRecv->n64Serial, pRecv->n16PosDst);
		break;
	case EICT_RoleWare:
		{
			if(pRecv->dwNPCID == GT_INVALID)
			{
				tagItem* pItem = NULL;
				TMap<INT64, INT16>::TMapIterator iter = pRole->GetItemMgr().GetBag().Begin();

				while(pRole->GetItemMgr().GetBag().GetNextItem(iter, pItem))
				{
					if (!P_VALID(pItem))
					{
						continue;
					}

					if (pItem->pProtoType->eSpecFunc == EISF_PortableWare)
					{
						dwErrorCode = E_Success;
						break;
					}

					dwErrorCode = E_UseItem_ItemNotExist;
				}				
			}
			else
			{
				// 如果NPCID为-1，则是使用随身仓库,不必验证npc
				dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
				
			}
			if(dwErrorCode != E_Success)
			{
				return dwErrorCode;
			}
		}
		break;

	case EICT_GuildWare:
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
		if(dwErrorCode == E_Success)
		{
			g_guildMgr.AddGuildWareEvent(pRole->GetID(), EVT_GuildWareMoveTo, pCmd->dwSize, pRecv);
		}
		return dwErrorCode;
		break;
	case EICT_Wardrobe:
		if (pRole->GetItemMgr().GetWardrobeContainer().CanMove(pRecv->n64Serial, pRecv->n16PosDst) != E_Success)
			return GT_INVALID;
		if (pRole->GetWardrobe().CanAddFashion(pRecv->n16PosDst, pRole->GetItemMgr().GetWardrobeContainer().GetItem(pRecv->n64Serial)))
			return GT_INVALID;
		break;
	}

	return pRole->GetItemMgr().Move(pRecv->eConType, pRecv->n64Serial, 
								pRecv->n16Num, pRecv->n16PosDst, ELCLD_Item_Move);
}

//-----------------------------------------------------------------------------
// 容器间移动物品
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleChangeItemPosEx(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemPosChangeEx);

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 如果和角色仓库有关，则需判断NPC
	if(EICT_RoleWare == pRecv->eConTypeSrc || EICT_RoleWare == pRecv->eConTypeDst)
	{
		DWORD dwErrorCode = E_Success;
		
		if(!GT_VALID(pRecv->dwNPCID)) // 如果NPCID为-1，则是使用随身仓库,不必验证npc 
		{
			// 判断随身仓库的相关物品是否存在

			// 遍历背包
			tagItem* pItem = NULL;
			TMap<INT64, INT16>::TMapIterator iter = pRole->GetItemMgr().GetBag().Begin();

			while(pRole->GetItemMgr().GetBag().GetNextItem(iter, pItem))
			{
				if (!P_VALID(pItem))
				{
					continue;
				}
				
				if (pItem->pProtoType->eSpecFunc == EISF_PortableWare)
				{
					dwErrorCode = E_Success;
					break;
				}

				dwErrorCode = E_UseItem_ItemNotExist;
			}
		}	
		else
		{
			dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
		}

		if(dwErrorCode != E_Success)
		{
			return dwErrorCode;
		}
		
// 		DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
// 		if(dwErrorCode != E_Success)
// 		{
// 			return dwErrorCode;
// 		}
	}

	// 如果和帮派仓库有关，则需判断NPC
	if(EICT_GuildWare == pRecv->eConTypeSrc || EICT_GuildWare == pRecv->eConTypeDst)
	{
		DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
		if(dwErrorCode == E_Success)
		{
			if( pRecv->eConTypeDst == EICT_GuildWare )
			{
				tagItem * pItem = pRole->GetItemMgr().GetItemFromContainer(pRecv->n64Serial1,pRecv->eConTypeSrc);
				if( MIsFaBao(pItem) )
				{
					tagFabao * pFabao = (tagFabao*)pItem;
					if( pFabao->eFabaoStatus != EEFS_Normal )
						return E_UseItem_PosLimit;
				}

				if (MIsHoly(pItem))
				{
					return E_UseItem_PosLimit;
				}
			}
			g_guildMgr.AddGuildWareEvent(pRole->GetID(), EVT_GuildWareMove2Other, pCmd->dwSize, pRecv);
		}
		return dwErrorCode;
	}

	if (pRecv->eConTypeSrc == EICT_RoleWare && pRecv->eConTypeDst == EICT_Bag)
	{
		ItemContainer *pConSrc = pRole->GetItemMgr().GetContainer(pRecv->eConTypeSrc);
		if(P_VALID(pConSrc))
		{
			tagItem *pItem = pConSrc->GetItem(pRecv->n64Serial1);

			// 玩家获得一件道具的事件接口
			if (P_VALID(pItem))
			{
				g_ScriptMgr.GetRoleScript()->OnObtainItem(pRole, pItem->dwTypeID, pItem->eCreateMode);
			}
		}
	}

	if (pRecv->eConTypeDst == EICT_Wardrobe)
	{
		DWORD dwErrorCode = pRole->GetItemMgr().GetWardrobeContainer().CanAdd2Wardrobe(pRecv->n64Serial1, pRole->GetItemMgr().GetBag(), pRecv->n16PosDst);
		if (dwErrorCode != E_Success)
		{
			if (pRecv->eConTypeSrc == EICT_Bag)
				return dwErrorCode;
			else
				return GT_INVALID;
		}
		ItemContainer *pConSrc = pRole->GetItemMgr().GetContainer(pRecv->eConTypeSrc);
		if(P_VALID(pConSrc))
		{
			tagItem *pItem = pConSrc->GetItem(pRecv->n64Serial1);
			if (!pRole->GetWardrobe().CanAddFashion(pRecv->n16PosDst, pItem))
				return GT_INVALID;
		}
	}
	else if (pRecv->eConTypeSrc == EICT_Wardrobe)
	{
		ItemContainer *pConSrc = pRole->GetItemMgr().GetContainer(pRecv->eConTypeSrc);
		if(P_VALID(pConSrc))
		{
			tagItem *pItem = pConSrc->GetItem(pRecv->n64Serial1);
			if (P_VALID(pItem))
			{
				if (CalLayerID(pItem->n16Index) == pRole->GetWardrobe().GetCurWareLayer())
					return GT_INVALID;
			}
		}
	}

	return pRole->GetItemMgr().Move2Other(pRecv->eConTypeSrc, pRecv->n64Serial1, 
									pRecv->eConTypeDst, pRecv->n16PosDst, ELCLD_Item_Move2Other);
}

//------------------------------------------------------------------------
// 按指定顺序排列行囊中物品
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleReorderItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemReorder);

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 消息合法性弱检测
	if(pRecv->n16ItemNum <= 0 
		|| (pRecv->n16ItemNum > SPACE_ALL_BAG && pRecv->n16ItemNum > SPACE_ALL_WARE))
	{
		return GT_INVALID;
	}

	// 创建消息
	INT nSzMsg = sizeof(tagNS_ItemReorder) + (pRecv->n16ItemNum - 1) * sizeof(INT16);
	MCREATE_MSG(pSend, nSzMsg, NS_ItemReorder);

	// 处理
	DWORD dwErrorCode = E_Success;
	if(GT_INVALID == pRecv->dwNPCID)	
	{
		if(pRecv->eContainerType == EICT_Bag)	// 背包
		{
			pSend->eConType = EICT_Bag;
			dwErrorCode = pRole->GetItemMgr().ReorderBag(pRecv->n16Index, pSend->n16OldIndex, pRecv->n16ItemNum);
		}
		else if (pRecv->eContainerType == EICT_RoleWare)
		{
			pSend->eConType = EICT_RoleWare;
			tagItem* pItem = NULL;
			TMap<INT64, INT16>::TMapIterator iter = pRole->GetItemMgr().GetBag().Begin();

			while(pRole->GetItemMgr().GetBag().GetNextItem(iter, pItem))
			{
				if (!P_VALID(pItem))
				{
					continue;
				}

				if (pItem->pProtoType->eSpecFunc == EISF_PortableWare)
				{
					dwErrorCode = E_Success;
					break;
				}

				dwErrorCode = E_UseItem_ItemNotExist;
			}
			if(E_Success == dwErrorCode)
			{
				dwErrorCode = pRole->GetItemMgr().ReorderRoleWare(pRecv->n16Index, pSend->n16OldIndex, pRecv->n16ItemNum);
			}
		}
	}
	else	// 仓库
	{
		pSend->eConType = EICT_RoleWare;
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
		if(E_Success == dwErrorCode)
		{
			dwErrorCode = pRole->GetItemMgr().ReorderRoleWare(pRecv->n16Index, pSend->n16OldIndex, pRecv->n16ItemNum);
		}
	}

	// 设置消息
	pSend->n16ItemNum = pRecv->n16ItemNum;
	pSend->dwErrorCode = dwErrorCode;

	if(dwErrorCode != E_Success)
	{
		pSend->dwSize = sizeof(tagNS_ItemReorder);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return dwErrorCode;
}
//------------------------------------------------------------------------
// 使用记录式传送符设置传送坐标
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetTransportMap(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetTransportMap);

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	tagNS_SetTransportMap	sendclient;
	//设置点
	sendclient.dwErrorCode = (E_SET_TRANSPORT_ERROR_CODE)pRole->SetItemTransportPoint(pRecv);
	//回复客户端
	sendclient.dwTransportMapID = pRole->GetMapID();
	sendclient.vTransportPoint = pRole->GetCurPos();
	SendMessage(&sendclient, sendclient.dwSize);

	//数据库
	if(sendclient.dwErrorCode == EST_SuccessSet)
	{
		tagNDBC_UpdateRoleItemTransport senddb;
		senddb.dwRoleID	= pRole->GetID();
		senddb.dwMapID	= pRole->GetMapID();
		senddb.fX		= sendclient.vTransportPoint.x;
		senddb.fZ		= sendclient.vTransportPoint.z;
		senddb.fY		= sendclient.vTransportPoint.y;
		g_dbSession.Send(&senddb, senddb.dwSize);
	}
	return E_Success;
}
//------------------------------------------------------------------------
// 使用记录式传送符传送
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUseTransportMap(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UseTransportMap);

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	tagNS_UseItem send;
	//使用记录式传送符
	send.dwErrorCode = pRole->UseNoteTakingItemTransport(pRecv);

	//返回
	send.dwSrcRoleID	= pRole->GetID();
	send.dwTarRoleID	= pRole->GetID();
	send.n64ItemID		= pRecv->n64ItemID;
	send.dwTypeID		= GT_INVALID;
	send.dwSerial		= 0;
	send.bInmmediate	= true;

	SendMessage(&send, send.dwSize);

	return E_Success;
}


DWORD PlayerSession::HandleRoleReorderItemEx(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ItemReorderEx);

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 消息合法性弱检测
	if(pRecv->n16ItemNum <= 0 
		|| (pRecv->n16ItemNum > SPACE_ALL_BAG && pRecv->n16ItemNum > SPACE_ALL_WARE))
	{
		return GT_INVALID;
	}

	// 创建消息
	INT nSzMsg = sizeof(tagNS_ItemReorderEx) + pRecv->n16ItemNum * sizeof(tagItemOrder) - sizeof(BYTE);
	MCREATE_MSG(pSend, nSzMsg, NS_ItemReorderEx);

	// 处理
	DWORD dwErrorCode = E_Success;
	if( GT_INVALID == pRecv->dwNPCID)	// 背包
	{
		if( pRecv->eContainerType == EICT_Bag )
		{
			pSend->eConType = EICT_Bag;
			dwErrorCode = pRole->GetItemMgr().ReorderBagEx(pRecv->byData, pSend->byData, pSend->n16ItemNum, pRecv->n16ItemNum);
		}
		else if( pRecv->eContainerType == EICT_RoleWare )		//随身仓库
		{
			pSend->eConType = EICT_RoleWare;
			pSend->eConType = EICT_RoleWare;
			tagItem* pItem = NULL;
			TMap<INT64, INT16>::TMapIterator iter = pRole->GetItemMgr().GetBag().Begin();

			while(pRole->GetItemMgr().GetBag().GetNextItem(iter, pItem))
			{
				if (!P_VALID(pItem))
				{
					continue;
				}

				if (pItem->pProtoType->eSpecFunc == EISF_PortableWare)
				{
					dwErrorCode = E_Success;
					break;
				}

				dwErrorCode = E_UseItem_ItemNotExist;
			}
			if(E_Success == dwErrorCode)
			{
				dwErrorCode = pRole->GetItemMgr().ReorderRoleWareEx(pRecv->byData, pSend->byData, pSend->n16ItemNum, pRecv->n16ItemNum);
			}
		}
	}
	else	// 仓库
	{
		pSend->eConType = EICT_RoleWare;
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Ware);
		if(E_Success == dwErrorCode)
		{
			dwErrorCode = pRole->GetItemMgr().ReorderRoleWareEx(pRecv->byData, 
							pSend->byData, pSend->n16ItemNum, pRecv->n16ItemNum);
		}
	}

	// 设置消息
	pSend->dwErrorCode = dwErrorCode;

	if(pSend->n16ItemNum != pRecv->n16ItemNum)
	{
		IMSG(_T("\n\nCaution:\n\tRole<id: %u> maybe modified bag order msg!\n"), pRole->GetID());
		ASSERT(pSend->n16ItemNum == pRecv->n16ItemNum);

		pSend->dwSize = sizeof(tagNS_ItemReorderEx) + pSend->n16ItemNum * sizeof(tagItemOrder) - sizeof(BYTE);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return dwErrorCode;
}

//------------------------------------------------------------------------
// 拾取物品
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePickUpItem(tagNetCmd* pCmd)
{
	tagNC_RolePickUpItem* pRecv = (tagNC_RolePickUpItem*)pCmd;

	if( !P_VALID(GetRole()) )
		return GT_INVALID;

	DWORD dwTypeID = 0;
	DWORD dwRtv =GetRole()->CanPickUpItem(pRecv->n64Serial);
	if (E_Success == dwRtv)
	{
		dwRtv = GetRole()->PickUpItem(pRecv->n64Serial, dwTypeID);
	}
	tagNS_RolePickUpItem send;
	send.dwRoleID		= GetRole()->GetID();
	send.n64Serial		= pRecv->n64Serial;
	send.dwErrorCode	= dwRtv;
	SendMessage(&send, send.dwSize);

	if (E_Success == dwRtv)
	{
		const ItemScript* pItemScript = g_ScriptMgr.GetItemScript(dwTypeID);
		if (P_VALID(pItemScript))
		{
			pItemScript->OnPickUpItem( dwTypeID, GetRole()->GetID());
		}
	}

	return 0;
}

//------------------------------------------------------------------------
// 扔掉物品
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRolePutDownItem(tagNetCmd* pCmd)
{
	tagNC_RolePutDownItem* pRecv = (tagNC_RolePutDownItem*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) )
		return GT_INVALID;

	BOOL bWarBag = FALSE;
	tagItem* pItem = pRole->GetItemMgr().GetBag().GetItem(pRecv->n64Serial);
	if( !P_VALID(pItem) )
	{
		pItem = pRole->GetItemMgr().GetWarBag().GetItem(pRecv->n64Serial);
		if( !P_VALID(pItem) )
			return GT_INVALID;

		bWarBag = TRUE;
	}

	DWORD dwErrorCode = E_Success;

	// 脚本判断物品是否可以被删除
	const ItemScript* pItemScript = g_ScriptMgr.GetItemScript(pItem->dwTypeID);
	if (P_VALID(pItemScript))
	{
		if (pItemScript->ItemCanPutDown(pItem->dwTypeID, pItem->n64Serial) != E_Success)
			dwErrorCode = E_SystemError;
	}

	tagNS_RolePutDownItem send;
	if (dwErrorCode != E_Success || pItem->pProtoType->DiscardEvent == EDE_DiscardNot)
	{
		send.dwErrorCode = dwErrorCode;
	}
	else
	{
		INT64 n64GroupID = 0;
		BOOL bAdd2Ground = pItem->pProtoType->DiscardEvent == EDE_DiscardGround;
		if(pItem->IsBind()) // 若该道具已绑定，DeadLoot = 0，丢弃后立刻摧毁，效果同DeadLoot = 1
			bAdd2Ground = FALSE;

		if( bWarBag )
		{
			send.dwErrorCode = pRole->PutDownWarItem(pRecv->n64Serial, n64GroupID, bAdd2Ground);
		}
		else
		{
			send.dwErrorCode = pRole->PutDownItem(pRecv->n64Serial, n64GroupID, bAdd2Ground);
		}
	}

	SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------
// 行囊加密
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_SetBagPsd);
	
	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_BagPsd send;
	
	if(IsHaveBagPsd())	// 已设置
	{
		send.dwErrorCode = E_BagPsd_Exist;
	}
	else	// 未设置，设置
	{
		SetBagPsd(p->dwBagPsdCrc);
		send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
		send.dwErrorCode	= E_Success;
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// 取消行囊加密
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUnsetBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_UnsetBagPsd);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;

	if(!IsHaveBagPsd())
	{
		send.dwErrorCode = E_BagPsd_NoExist;
	}
	else
	{
		if(GetSafeCode() != p->dwSafeCodeCrc)
		{
			send.dwErrorCode = E_BagPsd_SafeCode_Error;
		}
		else
		{
			SetBagPsd(GT_INVALID);
			send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
			send.dwErrorCode = E_Success;
		}
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// 验证行囊密码
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleCheckBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_OldBagPsd);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;
	send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
	send.dwErrorCode	= E_BagPsd_OK;

	if(GetBagPsd() != p->dwOldBagPsdCrc)
	{
		send.dwErrorCode = E_BagPsd_Error;
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// 修改行囊密码
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleResetBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_ResetBagPsd);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;

	if(!IsHaveBagPsd())
	{
		send.dwErrorCode = E_BagPsd_NoExist;
	}
	else
	{
		if(GetBagPsd() != p->dwOldBagPsdCrc)
		{
			send.dwErrorCode = E_BagPsd_Error;
		}
		else if(p->dwOldBagPsdCrc == p->dwNewBagPsdCrc)
		{
			send.dwErrorCode = GT_INVALID;
		}
		else
		{
			SetBagPsd(p->dwNewBagPsdCrc);
			send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
			send.dwErrorCode = E_Success;
		}
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// 打开背包等需要发送密码
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleOpenBagPsd(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_OpenBagPsd);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BagPsd send;

	if(IsHaveBagPsd() && GetBagPsd() != p->dwBagPsdCrc)
	{
		send.dwErrorCode = E_BagPsd_Error;
	}
	else
	{
		pRole->SetRoleStateEx(ERSE_BagPsdPass, TRUE);
		send.dwRoleStateEx	= pRole->m_RoleStateEx.GetState();
		send.dwErrorCode	= E_Success;
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// 设置行囊密码及发送相关消息
//------------------------------------------------------------------------
VOID PlayerSession::SetBagPsd(DWORD dwNewPsdCrc)
{
	m_sAccountCommon.dwBagPsdCrc = dwNewPsdCrc;

	// 向dbserver发消息
	tagNDBC_ChangeBagPsd send;
	send.dwAccountID = GetSessionID();
	send.dwNewBagPsd = dwNewPsdCrc;
	g_dbSession.Send(&send, send.dwSize);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return;
	}
	
	// 根据新密码设置角色状态
	pRole->m_RoleStateEx.SetState(ERSE_BagPsdPass);
	pRole->m_RoleStateEx.SetState(ERSE_BagPsdExist);

	if(GT_INVALID == dwNewPsdCrc)
	{
		pRole->m_RoleStateEx.UnsetState(ERSE_BagPsdExist);
	}
}

//------------------------------------------------------------------------
// 获得其他玩家的装备信息
//------------------------------------------------------------------------
DWORD PlayerSession::HandleGetSomeoneEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetRemoteRoleEquipInfo);

	// 获取角色
	Role *pMyRole = GetRole();
	if(!P_VALID(pMyRole))
		return GT_INVALID;

	DWORD dwRoleID = pRecv->dwRoleID;
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole)) 
	{
		tagNS_GetOfflineEquipInfo send;
		SendMessage(&send, send.dwSize);

		return GT_INVALID; 
	}

	DWORD dwErrorCode = E_Success;
//#if 0	// 策划的新需求，查看玩家装备不受玩家隐私设置影响
	//仅好友可见

	if(pRole->GetSession()->GetPersonalSet().eInfoOpenType == EPIS_Friend)
	{
		tagFriend* pFriRole = pRole->GetFriendPtr(pMyRole->GetID());
		if( !P_VALID(pFriRole) )
		{
			dwErrorCode = ECRI_E_FriendOnly;
		}
	}
	//隐私
	else if(pRole->GetSession()->GetPersonalSet().eInfoOpenType == EPIS_Private)
	{
		dwErrorCode = ECRI_E_Private;
	}
//#endif

// 	if (dwErrorCode != E_Success)
// 	{
// 		tagNS_GetRemoteRoleEquipInfo send;
// 		send.dwErrorCode = dwErrorCode;
// 		SendMessage(&send, send.dwSize);
// 
// 		return GT_INVALID;
// 	}
	INT nIndex = 0;
	INT nMaxIndex = EEP_MaxEquip;
	BYTE* pTemp = NULL;

	INT nMaxSize = (sizeof(tagFabao)+1)*nMaxIndex + sizeof(tagNS_GetRemoteRoleEquipInfo);
	MCREATE_MSG(pSend, nMaxSize,NS_GetRemoteRoleEquipInfo);

	pSend->dwRoleID = pRecv->dwRoleID;
	pSend->nEquipNum = 0;
	pTemp = pSend->byEquip;

	pSend->dwErrorCode = dwErrorCode;

	for(; nIndex < nMaxIndex; ++nIndex)
	{	
		tagEquip* pEquipInfo = pRole->GetItemMgr().GetEquipBarEquip((INT16)nIndex);
		if (!P_VALID(pEquipInfo)) continue;

		(*pTemp++) = (BYTE) pEquipInfo->pEquipProto->eType;
		if(pEquipInfo->IsFaBao())
		{
			memcpy(pTemp, (tagFabao*)pEquipInfo, sizeof(tagFabao));
			pTemp += sizeof(tagFabao);
		}
		else if(MIsSoulCrystal(pEquipInfo))
		{
			memcpy(pTemp, (tagSoulCrystal*)pEquipInfo, sizeof(tagSoulCrystal));
			pTemp += sizeof(tagSoulCrystal);
		}
		else if(MIsHoly(pEquipInfo))
		{
			// 如果是交易圣灵，若装备圣纹，则用其TypeId和充能次数替换 数组中存放的圣纹64位Id
			tagHolyMan *pHoly = (tagHolyMan *)pEquipInfo;

			tagHolyMan tmpHoly;
			IFASTCODE->MemCpy(&tmpHoly, pHoly, sizeof(tagHolyMan));

			for (int i=0; i< MaxHolyEquipNum; i++)
			{
				if (tmpHoly.n64EquipSerialIDs[i] != 0)
				{
					tagHolyEquip * pHolyEquip = (tagHolyEquip*)pRole->GetItemMgr().GetHolyManBarItem(tmpHoly.n64EquipSerialIDs[i]);
					if (P_VALID(pHolyEquip))
					{
						INT64 nTmpType = (INT64)pHolyEquip->dwTypeID;
						tmpHoly.n64EquipSerialIDs[i] = nTmpType * 1000 + pHolyEquip->n16EnhanceCount;										
					}
				}
			}	
			memcpy(pTemp, &tmpHoly, sizeof(tagHolyMan));
			pTemp += sizeof(tagHolyMan);
		}
		else if(MIsHolyEquip(pEquipInfo))
		{
			memcpy(pTemp, (tagHolyEquip*)pEquipInfo, sizeof(tagHolyEquip));
			pTemp += sizeof(tagHolyEquip);
		}
		else
		{
			memcpy(pTemp, pEquipInfo, sizeof(tagEquip));
			pTemp += sizeof(tagEquip);			
		}

		(pSend->nEquipNum)++;
		pEquipInfo = NULL;
	}

	(*pTemp) = _T('\0');
	pSend->dwSize = sizeof(tagNS_GetRemoteRoleEquipInfo) + (pTemp-pSend->byEquip);

	SendMessage(pSend, pSend->dwSize);

	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
	if (P_VALID(pRoleInfo) && P_VALID(pRole))
	{
		tagRoleInfo* pThisRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());
		if (!P_VALID(pThisRoleInfo))
		{
			return GT_INVALID;
		}

		tagRoleInfo* pMyInfo = g_roleMgr.GetRoleInfo(pMyRole->GetID());
		if (!P_VALID(pMyInfo))
		{
			return GT_INVALID;
		}

		tagNS_SomeoneViewYourEquip send;
		memcpy_s(send.szRoleName, sizeof(send.szRoleName), pMyInfo->szRoleName, sizeof(pMyInfo->szRoleName));
		pRole->SendMessage(&send, send.dwSize);
	}

	MDEL_MSG(pSend);
	return E_Success;
}

//-----------------------------------------------------------------------------
// 打开随身仓库
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSideWareOpen(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_WareOpen);

	DWORD dwRet = E_Success;

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_WareOpen	send;
	send.dwErrorCode = pRole->OpenSideWare(pRecv->n64ItemID);
	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

DWORD PlayerSession::HandleSyncFaBaoStoreExpInfo(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SyncFaBaoStoreExpInfo);
	
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	tagFabao* pFaBao = (tagFabao*)pRole->GetItemMgr().GetEquipBar().GetItem(static_cast<INT16>(EEP_Face));
	if (!P_VALID(pFaBao))
		return E_FabaoStoreExp_NoFabao;

	// 检查妖精级别
	if (pFaBao->n16Stage < MIN_STORE_EXP_FABAO_LEVEL)
		return E_FabaoStoreExp_LvlLimit;

	pFaBao->bStoredExpOpen = pRecv->bStoreExpOpen ? TRUE:FALSE;
	
	tagNS_SyncFaBaoStoreExpInfo send;
	send.bStoreExpOpen = pFaBao->bStoredExpOpen;
	send.nCurStoreExp = pFaBao->nCurStoredExp;
	pRole->SendMessage(&send, send.dwSize);

	return E_Success;
}

DWORD PlayerSession::HandleExpBall2ExpPilular(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ExpBall2ExpPilular);

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	DWORD dwRet = E_Success;

	tagFabao* pFaBao = (tagFabao*)pRole->GetItemMgr().GetEquipBar().GetItem(static_cast<INT16>(EEP_Face));
	if (!P_VALID(pFaBao))
	{
		dwRet = E_FabaoStoreExp_NoFabao;
		goto error;
	}

	// 检查妖精级别
	if (pFaBao->n16Stage < MIN_STORE_EXP_FABAO_LEVEL)
	{
		dwRet = E_FabaoStoreExp_LvlLimit;
		goto error;
	}

	// 检查是否存在经验球
 	tagItem* pExpBall = pRole->GetItemMgr().GetBag().GetItem(pRecv->n64ExpBallSerialID);
	if (!P_VALID(pExpBall))
	{
		dwRet = E_FabaoStoreExp_NoExpBall;
		goto error;
	}
	else if (!P_VALID(pExpBall->pProtoType))
	{
		IMSG(_T("Invalid ExpBall Proto!!typeid=%d\r\n"), pExpBall->dwTypeID);
		return GT_INVALID;
	}
	if (pExpBall->pProtoType->eSpecFunc != EISF_ExpBall)
	{
		IMSG(_T("Invalid ExpBall!!typeid=%d\r\n"), pExpBall->dwTypeID);
		return GT_INVALID;
	}
	BOOL bExpBallBinded = (pExpBall->pProtoType->nSpecFuncVal1 == 1)?TRUE:FALSE;
	DWORD dwBindedRoleID = pExpBall->dwSpecVal1;

	// 检查封印的经验是否足够生成水晶之心
	tagItemProto* pItemProto = g_attRes.GetItemProto(pRecv->dwExpPilularID);
	if (!P_VALID(pItemProto))
	{
		IMSG(_T("ExpPilular not exist!!typeid=%d\r\n"), pRecv->dwExpPilularID);
		return GT_INVALID;
	}
	if (pItemProto->eSpecFunc != EISF_ExpPilular)
	{
		IMSG(_T("Invalid ExpPilular!!typeid=%d\r\n"), pRecv->dwExpPilularID);
		return GT_INVALID;
	}
	if (pItemProto->nSpecFuncVal1 > pFaBao->nCurStoredExp)
	{
		dwRet = E_FabaoStoreExp_NoEnoughExp;
		goto error;
	}

	// 检查背包空间
	if (pRole->GetItemMgr().GetBagFreeSize() <= 0)
	{
		dwRet = E_FabaoStoreExp_BagFull;
		goto error;
	}

	// 扣除水晶球
	pRole->GetItemMgr().DelFromBag(pRecv->n64ExpBallSerialID, ELCID_Fabao_StoreExp_Costed, 1);

	// 扣除封印经验
	pFaBao->nCurStoredExp -= pItemProto->nSpecFuncVal1;

	// 生成水晶之心
	tagItem* pExpPilular = ItemCreator::Create(EICM_ExpPilularBind, pRole->GetID(), pRecv->dwExpPilularID);
	if (!P_VALID(pExpPilular))
	{
		IMSG(_T("Create ExpPilularItem Failed!!!typeid=%d,roleid=%d\r\n"), pRecv->dwExpPilularID, pRole->GetID());
		return GT_INVALID;
	}
	
	// 如果水晶球是绑定的，生成的水晶之心也是约定的
	if (bExpBallBinded == TRUE)
		pExpPilular->dwSpecVal1 = dwBindedRoleID;

	pRole->GetItemMgr().UpdateFabaoSpec(*pFaBao);

	pRole->GetItemMgr().Add2Bag(pExpPilular, ELCID_Fabao_StoreExp_Created, TRUE);
error:
	tagNS_ExpBall2ExpPilular send;
	send.dwErrorCode = dwRet;
	pRole->SendMessage(&send, send.dwSize);

	return dwRet;
}

DWORD PlayerSession::HandleExpBallBind2Role(tagNetCmd * pCmd)	
{
	MGET_MSG(pRecv, pCmd, NC_ExpBallBind2Role);

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	DWORD dwRet = E_Success;

	tagFabao* pFaBao = (tagFabao*)pRole->GetItemMgr().GetEquipBar().GetItem(static_cast<INT16>(EEP_Face));
	if (!P_VALID(pFaBao))
	{
		dwRet = E_FabaoStoreExp_NoFabao;
		goto error;
	}

	// 先检查是不是未约定的水晶球
	tagItem* pExpBall = pRole->GetItemMgr().GetBag().GetItem(pRecv->n64ExpBallSerialID);
	if (!P_VALID(pExpBall))
	{
		dwRet = E_FabaoStoreExp_NoExpBall;
		goto error;
	}
	else if (!P_VALID(pExpBall->pProtoType))
	{
		IMSG(_T("Invalid ExpBall Proto!!typeid=%d\r\n"), pExpBall->dwTypeID);
		return GT_INVALID;
	}
	if (pExpBall->pProtoType->eSpecFunc != EISF_ExpBall)
	{
		IMSG(_T("Invalid ExpBall!!typeid=%d\r\n"), pExpBall->dwTypeID);
		return GT_INVALID;
	}
	if (pExpBall->dwSpecVal1 != 0 && pExpBall->dwSpecVal1 != GT_INVALID)
	{
		IMSG(_T("ExpBall already binded!!typeid=%d"), pExpBall->dwTypeID);
		dwRet = E_FabaoStoreExp_BallBinded;
		goto error;
	}

	// 检查指定的水晶球[约定]是否存在
	tagItemProto* pItemProto = g_attRes.GetItemProto(pExpBall->pProtoType->nSpecFuncVal2);
	if (!P_VALID(pItemProto))
	{
		IMSG(_T("binded ExpBall Proto not exist!!typeid=%d\r\n"), pExpBall->dwTypeID);
		return GT_INVALID;
	}
	
	// 判断背包是否足够
	if (pRole->GetItemMgr().GetBagFreeSize() <= 0)
	{
		dwRet = E_FabaoStoreExp_BagFull;
		goto error;
	}

	DWORD dwExpBallTypeID = pExpBall->pProtoType->nSpecFuncVal2;
	// 删除原有的水晶球
	pRole->GetItemMgr().DelFromBag(pRecv->n64ExpBallSerialID, ELCID_Fabao_StoreExp_Costed, 1);

	// 生成新的水晶球[约定]，并执行约定操作
	tagItem* pNewExpBall = ItemCreator::Create(EICM_ExpPilularBind, pRole->GetID(), dwExpBallTypeID);
	if (!P_VALID(pNewExpBall))
	{
		IMSG(_T("Create ExpBallItem Failed!!!typeid=%d,roleid=%d\r\n"), pRecv->n64ExpBallSerialID, pRole->GetID());
		return GT_INVALID;
	}
	pNewExpBall->dwSpecVal1 = pRole->GetID();
	pRole->GetItemMgr().Add2Bag(pNewExpBall, ELCID_Fabao_StoreExp_Created, TRUE);

error:
	tagNS_ExpBallBind2Role send;
	send.dwErrorCode = dwRet;
	pRole->SendMessage(&send, send.dwSize);

	return dwRet;
}

DWORD	PlayerSession::HandleSetPickupMode(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_PickItemStrategy);

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;
	pRole->PickupMode(pRecv->dwStrategy);
	return E_Success;
}

