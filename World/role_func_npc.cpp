//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_func_npc.cpp
// author: Sxg
// actor:
// data: 2008-11-10
// last:
// brief: 部分职能NPC功能处理
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "role.h"
#include "creature.h"
#include "map_creator.h"
#include "../WorldDefine/msg_func_npc.h"
#include "../WorldDefine/func_npc_define.h"
#include "../WorldDefine/msg_map.h"
#include "../ServerDefine/log_cmdid_define.h"

//---------------------------------------------------------------------------------
// 部分职能NPC功能
//---------------------------------------------------------------------------------
DWORD Role::ProcDak(DWORD dwNPCID, INT32 nIndex, DWORD dwMapID)
{
	// 检查玩家是否处于不可移动状态
	if(IsInStateCantMove() || IsInRoleState(ERS_Commerce))
	{
		return E_Role_CantMove;
	}
	
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// 找到NPC，并检查合法性
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC))
	{
		return E_Dak_NPCNotFound;
	}
	
	if(!pNPC->IsFunctionNPC(EFNPCT_Dak))
	{
		return E_Dak_NPCNotValid;
	}

	if(!pNPC->CheckNPCTalkDistance(this))
	{
		return E_Dak_TooFar;
	}

	// 找到驿站
	const tagDakProto *pDak = g_attRes.GetDakProto(pNPC->GetDakID());
	if(!P_VALID(pDak))
	{
		ASSERT(P_VALID(pDak));
		return E_Dak_NotExist;
	}

	// 检查目的地
	if(pDak->dakSite[nIndex].dwMapID != dwMapID)
	{
		return E_Dak_TargetMap_Invalid;
	}

	// 检查消耗品 2010-07-21 增加一种消耗类型传送灵石  ECCT_TransferStone= 20 ,消耗特殊道具 spec_func= EISF_TransferStone 78
	if (pDak->dakSite[nIndex].eCostType==ECCT_TransferStone){

		tagItem* pItem = GetItemMgr().GetBag().GetOneSpecItemBySpecFunEnum(EISF_TransferStone);
		if( !P_VALID(pItem) || ( pItem->pProtoType->eSpecFunc != EISF_TransferStone ) ){
			return E_Dak_NoStone;
		}
			 
	} else{
		if(!GetCurMgr().IsEnough(pDak->dakSite[nIndex].eCostType, pDak->dakSite[nIndex].nCostVal))
		{
			return E_Dak_NotEnough_Money;
		}
	}


	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript))
	{
		if (!pRoleScript->CanUseDakNPC(this, dwMapID, pDak->dakSite[nIndex].fX, pDak->dakSite[nIndex].fY, pDak->dakSite[nIndex].fZ))
			return GT_INVALID;
	}
	
	// 传送
	if(!GotoNewMap(dwMapID, pDak->dakSite[nIndex].fX, pDak->dakSite[nIndex].fY, pDak->dakSite[nIndex].fZ))
	{
		return E_Dak_ChangeMap_Failed;
	}

	// 扣除消耗品 2010-07-21 增加一种消耗类型传送灵石  ECCT_TransferStone= 20 ,消耗特殊道具 spec_func=78

	if (pDak->dakSite[nIndex].eCostType==ECCT_TransferStone){

		tagItem* pItem = GetItemMgr().GetBag().GetOneSpecItemBySpecFunEnum(EISF_TransferStone);
		if( P_VALID(pItem) && ( pItem->pProtoType->eSpecFunc == EISF_TransferStone ) ){
			GetItemMgr().ItemUsedFromBag(pItem->n64Serial, (INT16)1, (DWORD)ELCLD_Item_Use);
		} 


	} else{
		GetCurMgr().DecCurrency(pDak->dakSite[nIndex].eCostType, pDak->dakSite[nIndex].nCostVal, ELCLD_Dak);
	}
	return E_Success;   
}

//------------------------------------------------------------------------------
// 设置复活城市
//------------------------------------------------------------------------------
DWORD Role::SetRebornMap( DWORD dwNPCID, DWORD &dwBornMapID, Vector3 &vBornPos )
{
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	if(pMap->GetMapInfo()->eType != EMT_Normal)
	{
		ASSERT(pMap->GetMapInfo()->eType == EMT_Normal);
		return E_BindRebornMap_MapInvalid;
	}

	// 找到NPC，并检查合法性
	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if(!P_VALID(pNPC))
	{
		return E_BindRebornMap_NPCNotFound;
	}

	if(!pNPC->IsFunctionNPC(EFNPCT_Revive))
	{
		return E_BindRebornMap_NPCInvalid;
	}

	if(!pNPC->CheckNPCTalkDistance(this))
	{
		return E_BindRebornMap_TooFar;
	}

	if(GetRebornMapID() == pMap->GetMapID())
	{
		return E_BindRebornMap_Already;
	}

	// 设置
	m_dwRebornMapID = pMap->GetMapID();

	dwBornMapID = m_dwRebornMapID;
	vBornPos = g_mapCreator.GetReBornPoint(m_dwRebornMapID);

	return E_Success;
}

//---------------------------------------------------------------------------------
// 使用磨石
//---------------------------------------------------------------------------------
DWORD Role::AbraseWeapon(INT64 n64AbraserSerial, INT64 n64WeaponSerial, OUT INT32 &nAttackTimes)
{
	nAttackTimes = 0;

	tagItem *pItem = GetItemMgr().GetBagItem(n64AbraserSerial);
	if(!P_VALID(pItem))
	{
		return GT_INVALID;
	}

	if(pItem->pProtoType->eSpecFunc != EISF_Grind)
	{
		return GT_INVALID;
	}

	BOOL bEquiped = FALSE;

	tagItem *pEquip = GetItemMgr().GetBagItem(n64WeaponSerial);
	if(P_VALID(pEquip))
	{
		bEquiped = FALSE;
	}
	else
	{
		pEquip = GetItemMgr().GetEquipBarEquip(n64WeaponSerial);
		if(P_VALID(pEquip))
		{
			bEquiped = TRUE;
		}
		else
		{
			return GT_INVALID;
		}
	}

	if(!MIsEquipment(pEquip->dwTypeID))
	{
		return GT_INVALID;
	}

	MTRANS_POINTER(p, pEquip, tagEquip);
	if(0 == p->nUseTimes)
	{
		return GT_INVALID;
	}

	INT nNewnessChange = pItem->pProtoType->nSpecFuncVal1;
	
	// 消耗磨石
	DWORD dwErrorCode = GetItemMgr().ItemUsedFromBag(n64AbraserSerial, 1, ELCLD_Item_Use);
	if(dwErrorCode != E_Success)
	{
		return dwErrorCode;
	}

	// 更新崭新度
	p->DecAttackTimes(nNewnessChange * ABRASION2USETIMES);
	GetItemMgr().UpdateEquipSpec(*p);
	nAttackTimes = p->nUseTimes;

	return E_Success;
}