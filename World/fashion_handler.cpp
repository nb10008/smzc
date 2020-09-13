//-----------------------------------------------------------------------------
// brief: 时装系统丰富
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/msg_fashion.h"
#include "../WorldDefine/container_define.h"
#include "wardrobe.h"
#include "att_res.h"
#include "role.h"
#include "player_session.h"

DWORD	PlayerSession::HandleRehanding(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_Rehanding);
	if ( !P_VALID(pRecv) )
		return GT_INVALID;

	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
		return GT_INVALID;

	tagWardrobeLayer* pWardrobeLayer = pRole->GetWardrobe().GetWardrobeLayer(pRecv->dwWardrobeLayerID);
	tagWardrobeProto* pWardrobeProto = g_attRes.GetWardrobeProto(pRecv->dwWardrobeLayerID);
	if (!P_VALID(pWardrobeLayer) || !P_VALID(pWardrobeProto))
		return GT_INVALID;
	
	DWORD dwStartIdx = (pRecv->dwWardrobeLayerID>0)?((pRecv->dwWardrobeLayerID-1)*MAX_FASHION_PERLAYER):0;
	BOOL bCanReHanding = FALSE;
	for (INT nCnt = 0; nCnt < MAX_FASHION_PERLAYER; nCnt++)
	{
		tagItem* pItem = pRole->GetItemMgr().GetWardrobeContainer().GetItem((INT16)(dwStartIdx+nCnt));
		if (P_VALID(pItem))
		{
			bCanReHanding = TRUE;
			break;
		}
	}
	if (!bCanReHanding)
		return GT_INVALID;

	DWORD dwErrorCode = GT_INVALID;
	if (pRecv->bTakeOffFashion)
	{
		// 将原来穿着的时装脱下来
		dwErrorCode = pRole->GetWardrobe().PutOffCurFashion();
		if ((E_Success == dwErrorCode) && P_VALID(pRole->GetMap()) && pRole->GetDisplaySet().bFashionDisplay)
		{
			tagNS_AvatarEquip send;
			send.dwRoleID		= pRole->GetID();
			send.bFashion		= TRUE;
			send.sAvatarEquip	= pRole->GetAvatarEquip();
			pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
		}
	}
	else
	{
			// 检查性别
		if (pWardrobeProto->eLayerType == EWLT_Normal)
		{
			EWardrobeLayerSex eSex = (pRole->GetSex() == 0)?EWLS_Women:EWLS_Men;
			if (pWardrobeProto->eLayerSex != eSex)
			{
				tagNS_Rehanding send;
				send.dwErrorCode = EFEC_Sex_NotMatch;
				pRole->SendMessage(&send, send.dwSize);
				return EFEC_Sex_NotMatch;
			}
		}
		// 将原来穿着的时装脱下来
		pRole->GetWardrobe().PutOffCurFashion();

		// 原来好友的装扮状态删除
		pRole->GetWardrobe().CancelBePlayActing(pRole);

		dwErrorCode = pRole->GetWardrobe().PlayActMyself(pRecv->dwWardrobeLayerID);
		if (E_Success == dwErrorCode)
			dwErrorCode = pRole->GetWardrobe().PutOnFashion(pRole, pRecv->dwWardrobeLayerID, pWardrobeLayer->byLevel);
	}

	if (dwErrorCode != GT_INVALID)
	{
		tagNS_Rehanding send;
		send.dwErrorCode = dwErrorCode;
		pRole->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

DWORD	PlayerSession::HandleWardrobeLevelUp(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_WardrobeLevelUp);
	if ( !P_VALID(pRecv) )
		return GT_INVALID;

	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
		return GT_INVALID;

	return pRole->GetWardrobe().LevelUpWardrobeLayer(pRole, pRecv->dwWardrobeLayerID);
}
DWORD	PlayerSession::HandleDressUpAnotherRole(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_DressUpAnotherRole);
	if ( !P_VALID(pRecv) )
		return GT_INVALID;

	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
		return GT_INVALID;

	// 检查角色
	DWORD dwErrorCode = GT_INVALID;
	tagNS_IsDressUpAgree send;
	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwTargetRoleID);
	if (!P_VALID(pDestRole))
	{
		dwErrorCode = EFEC_FriendNotOnLine;
		goto Err;
	}

	// 检查角色是否已经穿着了自己衣橱里面的时装
	if (pDestRole->GetWardrobe().IsPlayActing())
	{
		dwErrorCode = EFEC_IsPlayActing;
		goto Err;
	}

	dwErrorCode = pRole->GetWardrobe().StartPlayActFriend(pRecv->dwWardrobeLayerID,
		pRecv->dwTargetRoleID, pRole->GetPlayActFriendLimit());
	if (E_Success != dwErrorCode)
		goto Err;

	send.dwSrcRoleID = pRole->GetID();
	send.dwWardrobeLayerID = pRecv->dwWardrobeLayerID;
	pDestRole->SendMessage(&send, send.dwSize);
	return dwErrorCode;

Err:
	if (dwErrorCode != GT_INVALID)
	{
		tagNS_DressUpAnotherRole send2AnotherRole;
		send2AnotherRole.dwErrorCode = dwErrorCode;
		send2AnotherRole.dwSrcRoleID = pRole->GetID();
		send2AnotherRole.bAgree = true;
		pRole->SendMessage(&send2AnotherRole, send2AnotherRole.dwSize);
	}
	return dwErrorCode;
}
DWORD	PlayerSession::HandleIsDressUpAgree(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_IsDressUpAgree);
	if ( !P_VALID(pRecv) )
		return GT_INVALID;

	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
		return GT_INVALID;

	// 检查角色
	tagNS_DressUpAnotherRole send;
	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwTargetRoleID);
	if (!P_VALID(pDestRole))
	{
		send.dwErrorCode = EFEC_FriendNotOnLine;
		pRole->SendMessage(&send, send.dwSize);
		return send.dwErrorCode;
	}

	BYTE byLevel = GT_INVALID;
	// 加入角色是否已经穿着自己的时装的判断(这种情况是不应该出现的， 出现了，说明客户端有问题，不理它)
	if (pRole->GetWardrobe().IsPlayActing())
	{
		pDestRole->GetWardrobe().EndPlayActFriend(pRole->GetID(), byLevel, send.dwWardrobeLayerID, FALSE);
		send.dwErrorCode = EFEC_IsPlayActing;
		send.dwSrcRoleID = pDestRole->GetID();
		send.bAgree = true;
		pDestRole->SendMessage(&send, send.dwSize);
		return EFEC_IsPlayActing;
	}

	send.dwErrorCode = pDestRole->GetWardrobe().EndPlayActFriend(pRole->GetID(), byLevel, send.dwWardrobeLayerID, pRecv->bAgree);
	if (GT_INVALID == send.dwErrorCode)
	{
		return GT_INVALID;
	}
	else if (send.dwErrorCode != E_Success)
	{
		pDestRole->SendMessage(&send, send.dwSize);
		return send.dwErrorCode;
	}

	send.dwSrcRoleID = pRole->GetID();
	send.bAgree = pRecv->bAgree;
	if (pRecv->bAgree == true && byLevel != GT_INVALID && byLevel <= MAX_LEVEL_PERLAYER)
		send.dwErrorCode = pRole->GetWardrobe().PutOnFashion(pRole, send.dwWardrobeLayerID, byLevel, FALSE);
	pDestRole->SendMessage(&send, send.dwSize);
	return E_Success;
}
