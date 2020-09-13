//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: HolySoulHandle.h
// author: zxzhang
// actor:
// data: 2010-06-13
// last:
// brief:  元神系统消息处理
//-----------------------------------------------------------------------------

#include "stdAfx.h"

#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/msg_yuanshen.h"
#include "../WorldDefine/msg_compose.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "role.h"
#include "player_session.h"
#include "HolySoul.h"


//------------------------------------------------------------------------
// 穿元神装备
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleRoleSoulEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SoulEquip);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//地图和坐标判断待定
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	DWORD dwRet = pRole->GetHolySoul().EquipHolySoul(pRecv->n64Serial,(DWORD)pRecv->ePosDst);

	tagNS_SoulEquip Msg;
	Msg.dwErrorCode = dwRet;
	Msg.n64Serial   = pRecv->n64Serial;
	return E_Success;
}

//------------------------------------------------------------------------
// 脱元神装备
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleRoleUnSoulEquip(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UnSoulEquip);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//地图和坐标判断待定
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	DWORD dwRet = pRole->GetHolySoul().UnEquipHolySoul(pRecv->n64Serial,pRecv->n16PosDst);

	tagNS_UnSoulEquip Msg;
	Msg.dwErrorCode = dwRet;
	Msg.n64Serial   = pRecv->n64Serial;

	return E_Success;
}

//------------------------------------------------------------------------
// 灌注元神经验
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleRoleHolyAffuse(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetHolyAffuse);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	//地图和坐标判断待定
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	DWORD dwRet = pRole->GetHolySoul().PourExp(pRecv->n64AffuseExp);

	tagNS_GetHolyAffuse Msg;
	Msg.dwErrCode = dwRet;
	SendMessage(&Msg,Msg.dwSize);

    return E_Success;
}

//------------------------------------------------------------------------
// 灌注转生
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleRoleHolyReborn(tagNetCmd* pCmd)
{
// 	MGET_MSG(pRecv, pCmd, NC_GetHolyReborn);
// 	if (!P_VALID(pRecv))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	Role* pRole = GetRole();
// 	if (!P_VALID(pRole))
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	tagNS_GetHolyReborn msg;
// 	HolySoul& holySoul = pRole->GetHolySoul();
// 	tagHolySoulInfo& holySoulInfo = holySoul.GetHolySoulInfo();
// 	if (EHSS_Awake == holySoul.GetState())
// 	{
// 		msg.dwErrCode = E_HolySoulReborn_AwakeCannot;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolySoulReborn_AwakeCannot;
// 	}
// 
// 	INT nHolySoulLevel = holySoulInfo.SoulAtt.nSoulAtt[ESAT_SoulLevel];	
// 	if (nHolySoulLevel < HOLYSOUL_MAX_LEVEL)
// 	{
// 		msg.dwErrCode = E_HolySoulReborn_LevelNotEnough;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolySoulReborn_LevelNotEnough;
// 	}
// 
// 	ItemMgr& itemMgr = pRole->GetItemMgr();
// 	tagItem* pHyItem = itemMgr.GetBagItem(pRecv->n64ItemID);
// 	tagItem* pLjItem = itemMgr.GetBagItem(pRecv->n64StuffID);
// 	if (!P_VALID(pHyItem) || !P_VALID(pLjItem))
// 	{
// 		msg.dwErrCode = E_HolySoulReborn_ItemNotExist;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolySoulReborn_ItemNotExist;
// 	}
// 
// 	//判断是否幻元玉魄
// 	if (holySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv] == 10000)
// 	{
// 		if (pHyItem->dwTypeID != REBORN_ITEM_TYPEID_LV1)
// 		{
// 			msg.dwErrCode = E_HolySoulReborn_ItemNotExist;
// 			SendMessage(&msg,msg.dwSize);
// 			return E_HolySoulReborn_ItemNotExist;
// 		}
// 	}
// 	else if (holySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv] == 20000)
// 	{
// 		if (pHyItem->dwTypeID != REBORN_ITEM_TYPEID_LV2)
// 		{
// 			msg.dwErrCode = E_HolySoulReborn_ItemNotExist;
// 			SendMessage(&msg,msg.dwSize);
// 			return E_HolySoulReborn_ItemNotExist;
// 		}
// 	}
// 
// 	//判断是否老君仙露
// 	if (pLjItem->dwTypeID != REBORN_STUFF_BINDING && pLjItem->dwTypeID != REBORN_STUFF_UNBINDING)
// 	{
// 		msg.dwErrCode = E_HolySoulReborn_ItemNotExist;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolySoulReborn_ItemNotExist;
// 	}
// 
// 	if (holySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv] >= REBORN_MAX_LEVEL * 10000)
// 	{
// 		msg.dwErrCode = E_HolySoulReborn_LevelMax;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolySoulReborn_LevelMax;
// 	}
// 
// 	
// 	if (pRecv->nNumStuff <= 0)
// 	{
// 		return GT_INVALID;
// 	}
// 
// 	//判断背包中老君仙露个数是否够用
// 	INT32 nljItemNum = itemMgr.GetBagSameItemCount(pLjItem->dwTypeID);
// 	if(pLjItem->dwTypeID == REBORN_STUFF_BINDING)
// 	{
// 		nljItemNum += itemMgr.GetBagSameItemCount(REBORN_STUFF_UNBINDING); 
// 	}
// 	else if(pLjItem->dwTypeID == REBORN_STUFF_UNBINDING)
// 	{
// 		nljItemNum += itemMgr.GetBagSameItemCount(REBORN_STUFF_BINDING); 
// 	}
// 	if ((pRecv->nNumStuff) > (INT)nljItemNum)
// 	{
// 		msg.dwErrCode = E_HolySoulReborn_ItemNotEnough;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolySoulReborn_ItemNotEnough;
// 	}
// 	INT nItemCostNum = pRecv->nNumStuff;
// 	//判断绑定和非绑定的老君仙露个数
// 	TList<tagItem*> itemListBind;  //消耗绑定老君仙露list
// 	TList<tagItem*> itemListUnBind;//消耗非绑定老君仙露list
// 	itemListBind.Clear();
// 	itemListUnBind.Clear();
// 	INT nItemBindNum = 0;
// 	INT nItemUnBindNum = 0;
// 	if (P_VALID(pLjItem))
// 	{
// 		pRole->CalculateItemCostNum(pLjItem->dwTypeID,itemListBind,itemListUnBind,nItemCostNum,nItemBindNum,nItemUnBindNum);
// 	}
// 
// 	
// 	const tagHolySoulReborn* pReborn = g_attRes.GetSoulRebornProto(pRecv->nNumStuff);
// 	if (!P_VALID(pReborn))
// 	{
// 		msg.dwErrCode = E_HolySoulReborn_ProtoNotExist;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolySoulReborn_ProtoNotExist;
// 	}
// 
// 	//获得当前转生等级对应的成功率
// 	INT nRate = 0;
// 	INT nRebornLevel = (INT)holySoulInfo.SoulAtt.nSoulAtt[ESAT_RebornLv] / 10000;
// 	switch (nRebornLevel)
// 	{
// 	case 1:
// 		nRate = pReborn->nRate1;
// 		break;
// 	case 2:
// 		nRate = pReborn->nRate2;
// 		break;
// 	}
// 	 
// 	BOOL bRet = IUTIL->RandomInRange(0,10000) < nRate;
// 	DWORD dwErrorCode = GT_INVALID;
// 	//扣除绑定和非绑定的老君仙露
// 	if(nItemBindNum)
// 		dwErrorCode = pRole->GetItemMgr().DelBagSameItem(itemListBind,nItemBindNum,ELCLD_HolySoulReborn);
// 	if(nItemUnBindNum)
// 		dwErrorCode = pRole->GetItemMgr().DelBagSameItem(itemListUnBind,nItemUnBindNum,ELCLD_HolySoulReborn);
// 
// 	if (bRet)
// 	{
// 		//先从背包中扣除幻元玉魄和老君仙露
// 		dwErrorCode = itemMgr.DelFromBag(pHyItem->n64Serial,ELCLD_HolySoulReborn,1,TRUE);
// 		if (E_Success != dwErrorCode)
// 		{
// 			return dwErrorCode;
// 		}
// 		//dwErrorCode = itemMgr.DelFromBag(pLjItem->n64Serial,ELCLD_HolySoulReborn,pRecv->nNumStuff,TRUE);//扣除所有放入的老君仙露
// 		
// 		if (E_Success != dwErrorCode)
// 		{
// 			return dwErrorCode;
// 		}
// 
// 		dwErrorCode = holySoul.HolySoulReborn();
// 		if (E_Success != dwErrorCode)
// 		{
// 			msg.dwErrCode = dwErrorCode;
// 			SendMessage(&msg,msg.dwSize);
// 			return dwErrorCode;
// 		}
// 
// 		msg.dwErrCode = E_Success;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_Success;
// 
// 	}
// 	else
// 	{
// 		//转生失败，扣除放入的所有老君仙露
// 		//dwErrorCode = itemMgr.DelFromBag(pLjItem->n64Serial,ELCLD_HolySoulReborn,pRecv->nNumStuff,TRUE);//老君仙露扣除个数待定
// 		if (E_Success != dwErrorCode)
// 		{
// 			return dwErrorCode;
// 		}
// 
// 		//元神等级降低10级
// 		dwErrorCode = holySoul.LevelDownSoul(10);
// 		if (E_Success != dwErrorCode)
// 		{
// 			return dwErrorCode;
// 		}
// 
// 		msg.dwErrCode = E_HolySoulReborn_Fail;
// 		SendMessage(&msg,msg.dwSize);
// 		return E_HolySoulReborn_Fail;
// 	}
	return E_Success;
}
