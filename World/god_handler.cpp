//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: god_handler.cpp
// author: 
// actor:
// data: 2011-01-12
// last:
// brief: 神系统
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_god.h"
#include "player_session.h"
#include "role.h"
#include "ClergyRank.h"
//-----------------------------------------------------------------------------
// 获得属性值
DWORD PlayerSession::HandleRoleGetAttribute(tagNetCmd* pCmd)
{
	tagNC_GetAttribute* pRecv = (tagNC_GetAttribute*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) 
		return GT_INVALID;

	// 返回给客户端
	tagNS_GetAttribute send;
	send.dwValue = pRole->GetAttValue(pRecv->eType);
	SendMessage(&send, send.dwSize);

	return E_Success;
}

//----------------------------------------------------------------------------
// 神职晋升
DWORD PlayerSession::HandleRolePromotionClergy(tagNetCmd* pCmd)
{
	tagNC_PromotionClergy* pRecv = (tagNC_PromotionClergy*)pCmd;

	tagNS_PromotionClergy send;
	send.dwErrorCode = E_Success;
	send.dwClergy = pRecv->dwClergy;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) 
	{
		send.dwErrorCode = E_SystemError;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	send.dwErrorCode = pRole->DirectPromotionClergy(pRecv->dwClergy);
	SendMessage(&send, send.dwSize);

	//广播给周围的角色
	if(E_Success == send.dwErrorCode)
	{
		tagNS_ChangeClergy send1;
		send1.dwClergy = send.dwClergy;
		send1.dwRoleID = pRole->GetID();
		if( P_VALID(pRole->GetMap()) )
			pRole->GetMap()->SendBigVisTileMsg(pRole, &send1, send1.dwSize);
	}

	return E_Success;
}
//----------------------------------------------------------------------------
//
DWORD PlayerSession::HandleGetCompetitiveClergyResult(tagNetCmd *pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_CampaignResult);

	if (ECGT_INVALID(pRecv->eType))
	{
		return GT_INVALID;
	}

	TMap<DWORD, tagClergyNode*>& mapTmp = g_ClergyRankingMgr.GetClergyRank(pRecv->eType);
	
	tagNS_CampaignResult send;
	send.eType = pRecv->eType;
	send.nNum = mapTmp.Size();

	TMap<DWORD, tagClergyNode*>::TMapIterator it = mapTmp.Begin();
	tagClergyNode* pNode = NULL;
	int i=0;
	while(mapTmp.PeekNext(it, pNode) && i<MAX_CampaignInfoNumPerType)
	{
		if(!P_VALID(pNode)) continue;

		send.sResult.sPlayerInfo[i].eClergy = pNode->eClergy;
		send.sResult.sPlayerInfo[i].dwStrength = pNode->dwPoint;
		g_roleMgr.GetRoleNameByID(pNode->dwRoleID, send.sResult.sPlayerInfo[i].szName);

		i++;
	}
	send.nNum = i;
	SendMessage(&send, send.dwSize);

	return E_Success;
}

DWORD PlayerSession::HandleGetClergyRalation(tagNetCmd *pCmd)
{
	for (int j=ECGT_War; j<ECGT_Num; ++j)
	{
		TMap<DWORD, tagClergyNode*>& mapTmp = g_ClergyRankingMgr.GetClergyRank((ECampaignGodType)j);

		tagNS_GetGodRelation send;
		send.eType = (ECampaignGodType)j;
		
		TMap<DWORD, tagClergyNode*>::TMapIterator it = mapTmp.Begin();
		tagClergyNode* pNode = NULL;
		int i=0;
		while(mapTmp.PeekNext(it, pNode) && i<MAX_CLERGY_WINNER)
		{
			if(!P_VALID(pNode)) continue;

			if(pNode->eClergy == ECT_Loser) continue;

			send.sResult.sPlayerInfo[i].eClergy = pNode->eClergy;
			send.sResult.sPlayerInfo[i].dwStrength = pNode->dwPoint;
			g_roleMgr.GetRoleNameByID(pNode->dwRoleID, send.sResult.sPlayerInfo[i].szName);

			i++;
		}
		send.nNum = i;

		SendMessage(&send, send.dwSize);
	}

	return E_Success;
}

//----------------------------------------------------------------------------
//
DWORD PlayerSession::HandleGetClergyEnrollInfo(tagNetCmd *pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_CampaignInfo);

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	tagNS_CampaignInfo send;
	send.eType = pRole->GetClergyCandidateType();
	g_ClergyRankingMgr.GetCandidateCount(send.dwNum);
	SendMessage(&send, send.dwSize);

	return E_Success;
}

//----------------------------------------------------------------------------
//
DWORD PlayerSession::HandleRoleEnrollClergy(tagNetCmd *pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_Campaign);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_Campaign send;
	if ( pRole->GetClergyCandidateType() != ECGT_Null )
	{
		send.dwErrorCode = E_Enroll_Clergy_Limited;
	}
	else
	{
		DWORD dwErrorCode = pRole->CanGetClergy(pRecv->eType);
		if (dwErrorCode != E_Success)
		{
			send.dwErrorCode = dwErrorCode;
		}
		else
		{
			pRole->SetClergyCandidateType(pRecv->eType);
			send.dwErrorCode = E_Success;
			g_ClergyRankingMgr.SetCandidateCountChg();
		}
	}

	SendMessage(&send, send.dwSize);

	return E_Success;
}

//----------------------------------------------------------------------------
DWORD PlayerSession::HandleActivationArea(tagNetCmd *pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ActivationArea);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_ActivationArea send;

	send.dwAreaID = pRecv->dwAreaID;
	send.dwErrorCode = pRole->ChangeAreaAbility(pRecv->dwAreaID,pRecv->bActive);
	send.bActive = pRecv->bActive;
	send.dwCD = 0;

	if(E_Success != send.dwErrorCode)
	{
		send.bActive = !pRecv->bActive;
		if(E_God_Area_CD == send.dwErrorCode)
		{
			send.dwCD = pRole->GetAreaCD(pRecv->dwAreaID);
		}
	}

	SendMessage(&send, send.dwSize);

	return E_Success;
}

//----------------------------------------------------------------------------
DWORD PlayerSession::HandleAddArea(tagNetCmd *pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_AddArea);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_AddArea send;
	send.dwAreaID = pRecv->dwAreaID;
	send.dwErrorCode = pRole->ChangeArea(pRecv->dwAreaID,pRecv->bAdd);
	send.dwTime = 0;
	
	if(E_Success == send.dwErrorCode)
	{
		SendMessage(&send, send.dwSize);

		if(pRecv->bAdd)
			g_dbSession.SendCreateArea2DB(pRole->GetID(),pRecv->dwAreaID,FALSE);
		else
			g_dbSession.SendDeleteArea2DB(pRole->GetID(),pRecv->dwAreaID);
	}

	return E_Success;
}
//----------------------------------------------------------------------------

