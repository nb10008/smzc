//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: godbid_handler.cpp
// actor:
// data: 2010-11-27
// last:
// brief: 客户端和服务器端间消息处理 -- 神之竞标系统
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "../WorldDefine/msg_GodBidDefine.h"
#include "role.h"
// #include "role_mgr.h"
// #include "map.h"

#include "player_session.h"
#include "godbid_mgr.h"

DWORD	PlayerSession::HandleGodBiderShopOpen(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GodBiderShopOpen);
	if ( !P_VALID(pRecv) )
		return GT_INVALID;

	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
		return GT_INVALID;

	// 判断npc职能
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GodBidNPC);
	if (dwErrorCode != E_Success)
	{
		tagNS_GodBiderShopOpen send;
		send.dwErrorCode = dwErrorCode;
		pRole->SendMessage(&send, send.dwSize);
		return dwErrorCode;
	}

	g_GodBidMgr.SendNormalBidInfo2Client(pRole);

	return E_Success;
}

DWORD	PlayerSession::HandleConformPrice(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ConformPrice);
	if ( !P_VALID(pRecv) )
		return GT_INVALID;

	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
		return GT_INVALID;
	
	g_GodBidMgr.ProcessNormalBidConformPrice(pRecv->dwFormulaID, pRole, pRecv->n64Price);

	return E_Success;
}

DWORD	PlayerSession::HandleBlackMarketShopOpen(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BlackMarketShopOpen);
	if ( !P_VALID(pRecv) )
		return GT_INVALID;

	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
		return GT_INVALID;

	// 判断npc职能
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_BlackMarketNPC);
	if (dwErrorCode != E_Success)
	{
		tagNS_BlackMarketShopOpen send;
		send.dwErrorCode = dwErrorCode;
		pRole->SendMessage(&send, send.dwSize);
		return dwErrorCode;
	}

	g_GodBidMgr.SendBlackMarketBidInfo2Client(pRole);

	return E_Success;
}

DWORD	PlayerSession::HandleBlackConPrice(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BlackConPrice);
	if ( !P_VALID(pRecv) )
		return GT_INVALID;

	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
		return GT_INVALID;

	g_GodBidMgr.ProcessBlackBidConformPrice(pRecv->dwFormulaID, pRole, pRecv->n64Price);

	return E_Success;
}
