//-----------------------------------------------------------------------------
// brief: 战斗挂机系统消息处理
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "player_session.h"
#include "../WorldDefine/msg_AutoFight.h"
#include "../WorldDefine/ItemDefine.h"
#include "role.h"

// 使用战斗符
DWORD PlayerSession::HandleUseZhanDouFu(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UseZhanDouFu);
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

	tagNS_UseZhanDouFu send;

	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
	if (!P_VALID(pItem))
	{
		send.dwErrorCode = E_AutoFight_No_ZhanDouFu;
	}
	else if (P_VALID(pItem->pProtoType) && pItem->pProtoType->eSpecFunc != EISF_ZhanDouFu)
	{
		send.dwErrorCode = E_AutoFight_No_ZhanDouFu;
	}
	else
	{
		// 如果是台湾版本的战斗符，用dwSpecVal1记录剩余秒数，初始值为dwTimeLimit
		if (pItem->pProtoType->nSpecFuncVal1 == EAFSFV2_OMG && pItem->dwSpecVal1 == 0)
		{
			pItem->dw1stGainTime = 0;
			pItem->dwSpecVal1 = pItem->pProtoType->dwTimeLimit*TICK_PER_SECOND;
			pItem->BindOMGAutoFightItem();
			pItem->SetUpdate(EUDBS_Update);
		}
		else if (pItem->pProtoType->nSpecFuncVal1 == EAFSFV2_Normal)
		{
			if (!pRole->GetItemMgr().GetBag().IsInTimeLimitMgr(pRecv->n64ItemID))
			{
					pItem->dw1stGainTime = GetCurrentDWORDTime();
					pItem->SetUpdate(EUDBS_Update);
					pRole->GetItemMgr().GetBag().Add2TimeLeftList(pRecv->n64ItemID, pItem->pProtoType->dwTimeLimit, pItem->dw1stGainTime);
			}
		}
		send.dw1stGainTime = pItem->dw1stGainTime;
		send.n64ItemID = pRecv->n64ItemID;
		send.dwLeftTicks = pItem->dwSpecVal1;
		send.dwErrorCode = E_AutoFight_UseZhanDouFu_Success;
	}

	pRole->SendMessage(&send, send.dwSize);

	return E_Success;
}

// 同步内挂状态
DWORD PlayerSession::HandleSyncAutoFightState(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SyncAutoFightState);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}

	// 地图和坐标判断待定
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	if (pRecv->eAutoFightState == EAFS_End ||
		pRecv->eAutoFightState == EAFS_Start)
	{
		tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
		if (!P_VALID(pItem))
		{
			// 没有战斗符道具，先关掉
			pRole->SetAutoFightState(EAFS_Close);
			pRole->SetCurZhanDouFu64ID(0);
		}
		else
		{
			// 有战斗符道具，设置角色战斗挂机状态
			INT64 n64CurZhanDouFu = (pRecv->eAutoFightState == EAFS_Start)?pRecv->n64ItemID:0;
			pRole->SetAutoFightState(pRecv->eAutoFightState);
			pRole->SetCurZhanDouFu64ID(n64CurZhanDouFu);
		}
	}
	else
	{
		pRole->SetAutoFightState(EAFS_Close);
		pRole->SetCurZhanDouFu64ID(0);
	}
	
	// 然后通知客户端
	tagNS_SyncAutoFightState send;
	send.n64ItemID = pRecv->n64ItemID;
	send.eAutoFightState = pRole->GetAutoFightState();
	pRole->SendMessage(&send, send.dwSize);

	return E_Success;
}
