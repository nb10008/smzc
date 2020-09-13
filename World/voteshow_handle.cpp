//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: voteshow_handle.cpp
// author: Lamoair
// actor:
// data: 2010-4-1
// last:
// brief: 客户端和服务器端间消息处理 -- 选秀相关
//-----------------------------------------------------------------------------


#include "stdafx.h"


#include "role.h"

#include "role_mgr.h"

#include "../WorldDefine/msg_VoteShow_define.h"
#include "../WorldDefine/role_att.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_draft_show.h"

#include "draft_show.h"

#include "player_session.h"


//-----------------------------------------------------------------------------
// 小龙女排行查询
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleQueryDragonGirlRankings(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_QueryDragonGirlRankings);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}
	Role* pRole = GetRole();
	if ( !P_VALID( pRole) )
	{
          return GT_INVALID;
	}
    
	if ( g_DraftShow.GetTalentShow() == 0 )
	{
		tagNS_QueryDragonGirlRankings  msg;
		msg.n16Count = 0 ;
		pRole->SendMessage(&msg,msg.dwSize);
		return 0;
	}

     INT dwCount= g_DraftShow.GetShowPlayerRankingsSize();
     if ( 0 == dwCount )
     {
          tagNS_QueryDragonGirlRankings Msg;
		  Msg.n16Count = 0 ;
		  pRole->SendMessage(&Msg,Msg.dwSize);
		  return 0 ;
     }

	 DWORD dwSize = sizeof(tagNS_QueryDragonGirlRankings) + (dwCount-1)*sizeof(tagVoteShowRankingsData);
	 MCREATE_MSG(pSend,dwSize, NS_QueryDragonGirlRankings);
     pSend->n16Count = (INT16)dwCount;
	
	 for( INT i = 0 ; i < dwCount ; i++)
	 {
        const tagShowPlayer* pShowPlayer = g_DraftShow.GetMaxPointShowPlayer(i+1);
		if ( P_VALID( pShowPlayer ) )
		{
			pSend->data[i].dwPoint = pShowPlayer->dwAthletePoint;
			_tcscpy(pSend->data[i].tcRoleName,pShowPlayer->tcAthleteRoleName);
		}
		else
		{
		     pSend->data[i].dwPoint = 0;
			 _tcscpy(pSend->data[i].tcRoleName,_T("/0"));
		}
	 }
     

	 pRole->SendMessage(pSend,pSend->dwSize);
	 MDEL_MSG(pSend);
    return 0;
}


//-----------------------------------------------------------------------------
// 龙卫士排行查询
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleQueryDragonDefenderRankings(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_QueryDragonDefenderRankings);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}
	Role* pRole = GetRole();
	if ( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}
	if ( g_DraftShow.GetTalentShow() == 0 )
	{
		tagNS_QueryDragonDefenderRankings  msg;
		msg.n16Count = 0 ;
		pRole->SendMessage(&msg,msg.dwSize);
		return 0;
	}

	INT dwCount= g_DraftShow.GetVotePlayerRankingsSize();
	if ( 0 == dwCount )
	{
		tagNS_QueryDragonDefenderRankings Msg;
		Msg.n16Count = 0 ;
		pRole->SendMessage(&Msg,Msg.dwSize);
		return 0 ;
	}

	DWORD dwSize = sizeof(tagNS_QueryDragonDefenderRankings) + (dwCount-1)*sizeof(tagVoteShowRankingsData);
	MCREATE_MSG(pSend,dwSize, NS_QueryDragonDefenderRankings);
	pSend->n16Count = (INT16)dwCount;

	for( INT i = 0 ; i < dwCount ; i++)
	{
		const tagVotePlayer* pVotePlayer = g_DraftShow.GetMaxPointVotePlayer(i+1);
		if ( P_VALID( pVotePlayer ) )
		{
			pSend->data[i].dwPoint = pVotePlayer->dwVotePoint;
			_tcscpy(pSend->data[i].tcRoleName,pVotePlayer->tcVoteRoleName);
		}
		else
		{
			pSend->data[i].dwPoint = 0;
			_tcscpy(pSend->data[i].tcRoleName,_T("/0"));
		}
	}


	pRole->SendMessage(pSend,pSend->dwSize);
	MDEL_MSG(pSend);
	return 0;
}


//-----------------------------------------------------------------------------
// 为小龙女投票
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleVoteForDrangonGirl(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_VoteForDrangonGirl);
	if ( !P_VALID(pRecv) )
	{
		return GT_INVALID;
	}
	Role* pRole = GetRole();
	if ( !P_VALID( pRole) )
	{
		return GT_INVALID;
	}
    if ( pRecv->nDrangonGirlID == 0 || !GT_VALID(pRecv->nDrangonGirlID))
    {
		tagNS_VoteForDrangonGirl  msg;
		msg.dwErrorCode = E_VoteShow_SignIdInvaild;
		pRole->SendMessage(&msg,msg.dwSize);
		return 0;
    }
	if ( g_DraftShow.GetTalentShow() == 0 )
	{
		tagNS_VoteForDrangonGirl  msg;
		msg.dwErrorCode = E_VoteShow_UnOpen;
		pRole->SendMessage(&msg,msg.dwSize);
		return 0;
	}
    tagItem* pItem = pRole->GetItemMgr().GetItemByTypeID(g_DraftShow.GetNeckLaceTypeId());
    if ( !P_VALID(pItem))
    {
		tagNS_VoteForDrangonGirl  msg;
		msg.dwErrorCode = E_VoteShow_NoNeckLace;
		pRole->SendMessage(&msg,msg.dwSize);
		return 0;
    }

	DWORD dwResult =  g_DraftShow.Vote(pRecv->nDrangonGirlID,pRole->GetID());
    if ( dwResult != E_VoteShow_Success)
    {
		tagNS_VoteForDrangonGirl  msg;
		msg.dwErrorCode = E_VoteShow_SignIdInvaild;
		pRole->SendMessage(&msg,msg.dwSize);
		return 0;
    }
	dwResult = pRole->GetItemMgr().DelFromBag(pItem->n64Serial,(DWORD)ELCLD_VoteShow_UseNecklace,1);
    
	tagNS_VoteForDrangonGirl  msg;
	msg.dwErrorCode = dwResult;
	pRole->SendMessage(&msg,msg.dwSize);
   


	return 0;
}