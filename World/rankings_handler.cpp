//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: rankings_handler.cpp
// author: zr
// actor:
// data: 2009-12-3
// last:
// brief: 客户端和服务器端间消息处理 -- 实力系统相关
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "player_session.h"
#include "Role.h"
#include "pet_pocket.h"
#include "pet_soul.h"
#include "role_mgr.h"
#include "WorldRankings.h"
#include "../WorldDefine/role_att.h"
#include "../WorldDefine/msg_rankings.h"
#include "../ServerDefine/rankings_define.h"

DWORD PlayerSession::HandleGetRankings(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetRankingInfo);

	DWORD dwRet = E_Success;
	if ( pRecv->eType == ERT_Strength )
	{
		dwRet = HandleGetStrengthRankings(pRecv->nPage);
	}
	if ( pRecv->eType == ERT_Fame )
	{
		dwRet = HandleGetFameRankings(pRecv->nPage);
	}
	else if ( pRecv->eType == ERT_Level )
	{
		dwRet = HandleGetLevelRankings(pRecv->nPage);
	}
	else if ( pRecv->eType == ERT_Pet )
	{
		dwRet = HandleGetPetRankings(pRecv->nPage);
	}
	else if ( pRecv->eType == ERT_Equip )
	{
		dwRet = HandleGetEquipRankings(pRecv->nPage);
	}
	else if ( pRecv->eType == ERT_WuXun )
	{
		dwRet = HandleGetWuXunRankings(pRecv->nPage);
	}
	else if( pRecv->eType == ERT_Flower )
	{
		dwRet = HandleGetFlowerRankings(pRecv->nPage);
	}
	else if( pRecv->eType == ERT_Egg )
	{
		dwRet = HandleGetEggRankings(pRecv->nPage);
	}

	return dwRet;
}

DWORD PlayerSession::HandleGetStrengthRankings( INT nPage )
{
	INT nStartNum = (nPage-1) * RANKINGS_PAGE_LEN + 1;

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	tagNS_GetRankingInfo send;
	send.nPage = nPage;
	send.eType = ERT_Strength;
//	send.nLocalPlayerRank = pRole->GetPm(ERT_Strength);
	send.nLocalPlayerRank = pRole->GetPm(ERT_Strength);

	TMap<INT, tagRankingsNode*> StrengthRankings = g_WorldRankingMgr.GetStrengthRanking();
	tagRankingsNode* pStrengthRoleNode = NULL;

	INT nIndex = 0;
	for (INT i = nStartNum; i>=nStartNum && i<=nStartNum+RANKINGS_PAGE_LEN-1; ++i, ++nIndex)
	{
		pStrengthRoleNode = StrengthRankings.Peek(i);
		if (!P_VALID(pStrengthRoleNode))
		{
			if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
			{
				ASSERT(FALSE);
				return GT_INVALID;
			}

			send.sData[nIndex].nCurRanking = 0;
			send.sData[nIndex].nRankingChange = 0;
			send.sData[nIndex].sIDName.dwID = 0;
			ZeroMemory(send.sData[nIndex].sIDName.szName, X_SHORT_NAME);

			break;
		}

		if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
		{
			ASSERT(FALSE);
			return GT_INVALID;
		}

		pStrengthRoleNode->szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.sData[nIndex].sIDName.szName, X_SHORT_NAME, pStrengthRoleNode->szRoleName);
		send.sData[nIndex].sIDName.dwID = pStrengthRoleNode->dwRoleID;
		send.sData[nIndex].nCurRanking = pStrengthRoleNode->nNum;
		send.sData[nIndex].n64Value = pStrengthRoleNode->n64Value;
		if ( pStrengthRoleNode->nPreNum != 0 )
		{
			send.sData[nIndex].nRankingChange = pStrengthRoleNode->nPreNum - pStrengthRoleNode->nNum;
		}
		else
		{
			send.sData[nIndex].nRankingChange = 500 + ( 500 - pStrengthRoleNode->nNum );
		}
	}

	SendMessage(&send, send.dwSize);

	return E_Success;
}

DWORD PlayerSession::HandleGetFameRankings(INT nPage)
{
	INT nStartNum = (nPage-1) * RANKINGS_PAGE_LEN + 1;

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	tagNS_GetRankingInfo send;
	send.nPage = nPage;
	send.eType = ERT_Fame;
	send.nLocalPlayerRank = pRole->GetPm(ERT_Fame);

	TMap<INT, tagRankingsNode*> FameRankings = g_WorldRankingMgr.GetFameRanking();
	tagRankingsNode* pFameRoleNode = NULL;

	INT nIndex = 0;
	for (INT i = nStartNum; i>=nStartNum && i<=nStartNum+RANKINGS_PAGE_LEN-1; ++i, ++nIndex)
	{
		pFameRoleNode = FameRankings.Peek(i);
		if (!P_VALID(pFameRoleNode))
		{
			if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
			{
				ASSERT(FALSE);
				return GT_INVALID;
			}

			send.sData[nIndex].nCurRanking = 0;
			send.sData[nIndex].nRankingChange = 0;
			send.sData[nIndex].sIDName.dwID = 0;
			ZeroMemory(send.sData[nIndex].sIDName.szName, X_SHORT_NAME);

			break;
		}

		if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
		{
			ASSERT(FALSE);
			return GT_INVALID;
		}
		
		pFameRoleNode->szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.sData[nIndex].sIDName.szName, X_SHORT_NAME, pFameRoleNode->szRoleName);
		send.sData[nIndex].sIDName.dwID = pFameRoleNode->dwRoleID;
		send.sData[nIndex].nCurRanking = pFameRoleNode->nNum;
		send.sData[nIndex].n64Value = pFameRoleNode->n64Value;
		if ( pFameRoleNode->nPreNum != 0 )
		{
			send.sData[nIndex].nRankingChange = pFameRoleNode->nPreNum - pFameRoleNode->nNum;
		}
		else
		{
			send.sData[nIndex].nRankingChange = 500 + ( 500 - pFameRoleNode->nNum );
		}
	}

	SendMessage(&send, send.dwSize);

	return E_Success;
}


DWORD PlayerSession::HandleGetLevelRankings(INT nPage)
{
	INT nStartNum = (nPage-1) * RANKINGS_PAGE_LEN + 1;

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	tagNS_GetRankingInfo send;
	send.nPage = nPage;
	send.eType = ERT_Level;
	send.nLocalPlayerRank = pRole->GetPm(ERT_Level);

	TMap<INT, tagRankingsNode*> LevelRankings = g_WorldRankingMgr.GetLevelRanking();
	tagRankingsNode* pLevelRoleNode = NULL;

	INT nIndex = 0;
	for (INT i = nStartNum; i>=nStartNum && i<=nStartNum+RANKINGS_PAGE_LEN-1; ++i, nIndex++)
	{
		//tagRankingInfo TmpNode;
		//tagRoleIDName RoleIDName;
		pLevelRoleNode = LevelRankings.Peek(i);
		if (!P_VALID(pLevelRoleNode))
		{
			// 没有该名次的玩家		
			if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
			{
				ASSERT(FALSE);
				return GT_INVALID;
			}

			send.sData[nIndex].nCurRanking = 0;
			send.sData[nIndex].nRankingChange = 0;
			send.sData[nIndex].sIDName.dwID = 0;
			ZeroMemory(send.sData[nIndex].sIDName.szName, X_SHORT_NAME);

			break;
		}

		if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
		{
			ASSERT(FALSE);
			return GT_INVALID;
		}

		pLevelRoleNode->szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.sData[nIndex].sIDName.szName, X_SHORT_NAME, pLevelRoleNode->szRoleName);
		send.sData[nIndex].sIDName.dwID = pLevelRoleNode->dwRoleID;
		send.sData[nIndex].nCurRanking = pLevelRoleNode->nNum;
		//send.sData[nIndex].nRankingChange = pLevelRoleNode->nPreNum - pLevelRoleNode->nNum;
		send.sData[nIndex].n64Value = pLevelRoleNode->n64Value;
		if ( pLevelRoleNode->nPreNum != 0 )
		{
			send.sData[nIndex].nRankingChange = pLevelRoleNode->nPreNum - pLevelRoleNode->nNum;
		}
		else
		{
			send.sData[nIndex].nRankingChange = 500 + ( 500 - pLevelRoleNode->nNum );
		}
	}

	SendMessage(&send, send.dwSize);

	return E_Success;
}





DWORD PlayerSession::HandleGetWuXunRankings(INT nPage)
{
	INT nStartNum = (nPage-1) * RANKINGS_PAGE_LEN + 1;

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	tagNS_GetRankingInfo send;
	send.nPage = nPage;
	send.eType = ERT_WuXun;
	send.nLocalPlayerRank = pRole->GetPm(ERT_WuXun);

	TMap<INT, tagRankingsNode*> WuXunRankings = g_WorldRankingMgr.GetWuXunRanking();
	tagRankingsNode* pWuXunRoleNode = NULL;

	INT nIndex = 0;
	for (INT i = nStartNum; i>=nStartNum && i<=nStartNum+RANKINGS_PAGE_LEN-1; ++i, nIndex++)
	{
		//tagRankingInfo TmpNode;
		//tagRoleIDName RoleIDName;
		pWuXunRoleNode = WuXunRankings.Peek(i); 
		if (!P_VALID(pWuXunRoleNode))
		{
			// 没有该名次的玩家		
			if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
			{
				ASSERT(FALSE);
				return GT_INVALID;
			}

			send.sData[nIndex].nCurRanking = 0;
			send.sData[nIndex].nRankingChange = 0;
			send.sData[nIndex].sIDName.dwID = 0;
			ZeroMemory(send.sData[nIndex].sIDName.szName, X_SHORT_NAME);

			break;
		}

		if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
		{
			ASSERT(FALSE);
			return GT_INVALID;
		}

		pWuXunRoleNode->szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.sData[nIndex].sIDName.szName, X_SHORT_NAME, pWuXunRoleNode->szRoleName);
		send.sData[nIndex].sIDName.dwID = pWuXunRoleNode->dwRoleID;
		send.sData[nIndex].nCurRanking = pWuXunRoleNode->nNum;
		//send.sData[nIndex].nRankingChange = pLevelRoleNode->nPreNum - pLevelRoleNode->nNum;
		send.sData[nIndex].n64Value = pWuXunRoleNode->n64Value;
		if ( pWuXunRoleNode->nPreNum != 0 )
		{
			send.sData[nIndex].nRankingChange = pWuXunRoleNode->nPreNum - pWuXunRoleNode->nNum;
		}
		else
		{
			send.sData[nIndex].nRankingChange = 500 + ( 500 - pWuXunRoleNode->nNum );
		}
	}

	SendMessage(&send, send.dwSize);

	return E_Success;
}






DWORD PlayerSession::HandleGetPetRankings(INT nPage)
{
	INT nStartNum = (nPage-1) * RANKINGS_PAGE_LEN + 1;

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	tagNS_GetPetRankingInfo send;
	send.nPage = nPage;
	//send.eType = ERT_Pet;
	send.nLocalPlayerRank = pRole->GetPm(ERT_Pet);

	//------------------------------------------------------\
	// 获得本角色的所有宠物中等级最高的那个宠物的等级

	/*DWORD pAllPetID[10] ={0};
	INT nPetNum = 0;
	pRole->GetPetPocket()->GetAllPetID( pAllPetID, nPetNum );
	
	if ( P_VALID(pAllPetID) )
	{
		INT nBestPm = 501;
		DWORD dwBestPetID = 0;
		for( INT i = 0; i < nPetNum; i++ )
		{
			DWORD dwPetID = pAllPetID[i];
			PetSoul* pPet = pRole->GetPetPocket()->GetPetSoul(dwPetID);
			if (!P_VALID(pPet))
			{
				continue;
			}

			INT nPm = pPet->GetPetAtt().GetPetPm();
			if ( nPm < nBestPm && nPm!=0 )
			{
				nBestPm = nPm;
				dwBestPetID = dwPetID;
			}
		}

		if (nBestPm>500)
		{
			send.nLocalPlayerRank = 0;
		}
		else
		{
			send.nLocalPlayerRank = nBestPm;
		}
		
	}*/
	
	//------------------------------------------------------/


	TMap<INT, tagPetRankingsNode*> PetRankings = g_WorldRankingMgr.GetPetRanking();
	tagPetRankingsNode* pPetRoleNode = NULL;

	INT nIndex = 0;
	for (INT i = nStartNum; i >= nStartNum && i <= nStartNum + RANKINGS_PAGE_LEN - 1; ++i, nIndex++)
	{
		//tagRankingInfo TmpNode;
		//tagRoleIDName RoleIDName;
		pPetRoleNode = PetRankings.Peek(i);
		if (!P_VALID(pPetRoleNode))
		{
			// 没有该名次的玩家
			if( nIndex >= RANKINGS_PAGE_LEN || nIndex < 0)
			{
				ASSERT(FALSE);
				return GT_INVALID;
			}

			send.sData[nIndex].nCurRanking = 0;
			send.sData[nIndex].nRankingChange = 0;
			//send.sData[nIndex].n64Value = 0;
			ZeroMemory(send.sData[nIndex].szPetName, X_SHORT_NAME);
			send.sData[nIndex].sIDName.dwID = 0;
			ZeroMemory(send.sData[nIndex].sIDName.szName, X_SHORT_NAME);

			break;
		}

		if( nIndex >= RANKINGS_PAGE_LEN || nIndex < 0)
		{
			ASSERT(FALSE);
			return GT_INVALID;
		}

		//_tcscpy_s( send.sData[nIndex].sIDName.szName, X_SHORT_NAME, pPetRoleNode->szRoleName);
		g_roleMgr.GetRoleNameByID( pPetRoleNode->dwRoleID, send.sData[nIndex].sIDName.szName );
		send.sData[nIndex].sIDName.dwID = pPetRoleNode->dwRoleID;
		send.sData[nIndex].nCurRanking = pPetRoleNode->nNum;
		if ( pPetRoleNode->nPreNum != 0 )
		{
			send.sData[nIndex].nRankingChange = pPetRoleNode->nPreNum - pPetRoleNode->nNum;
		}
		else
		{
			send.sData[nIndex].nRankingChange = 500 + ( 500 - pPetRoleNode->nNum );
		}

		pPetRoleNode->szPetName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.sData[nIndex].szPetName, X_SHORT_NAME, pPetRoleNode->szPetName ); 
	}

	SendMessage(&send, send.dwSize);

	return E_Success;
}


DWORD PlayerSession::HandleGetEquipRankings(INT nPage)
{
	INT nStartNum = (nPage-1) * RANKINGS_PAGE_LEN + 1;

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	tagNS_GetRankingInfo send;
	send.nPage = nPage;
	send.eType = ERT_Equip;
	send.nLocalPlayerRank = pRole->GetPm(ERT_Equip);

	TMap<INT, tagRankingsNode*> EquipRankings = g_WorldRankingMgr.GetEquipRanking();
	tagRankingsNode* pEquipRoleNode = NULL;

	INT nIndex = 0;
	for (INT i = nStartNum; i >= nStartNum && i <= nStartNum + RANKINGS_PAGE_LEN - 1; ++i, nIndex++)
	{
		//tagRankingInfo TmpNode;
		//tagRoleIDName RoleIDName;
		pEquipRoleNode = EquipRankings.Peek(i);
		if (!P_VALID(pEquipRoleNode))
		{
			// 没有该名次的玩家
			if( nIndex >= RANKINGS_PAGE_LEN || nIndex < 0)
			{
				ASSERT(FALSE);
				return GT_INVALID;
			}

			send.sData[nIndex].nCurRanking = 0;
			send.sData[nIndex].nRankingChange = 0;
			send.sData[nIndex].n64Value = 0;
			send.sData[nIndex].sIDName.dwID = 0;
			ZeroMemory(send.sData[nIndex].sIDName.szName, X_SHORT_NAME);

			break;
		}

		if( nIndex >= RANKINGS_PAGE_LEN || nIndex < 0)
		{
			ASSERT(FALSE);
			return GT_INVALID;
		}

		pEquipRoleNode->szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.sData[nIndex].sIDName.szName, X_SHORT_NAME, pEquipRoleNode->szRoleName);
		send.sData[nIndex].sIDName.dwID = pEquipRoleNode->dwRoleID;
		send.sData[nIndex].nCurRanking = pEquipRoleNode->nNum;
		send.sData[nIndex].n64Value = pEquipRoleNode->n64Value;
		if ( pEquipRoleNode->nPreNum != 0 )
		{
			send.sData[nIndex].nRankingChange = pEquipRoleNode->nPreNum - pEquipRoleNode->nNum;
		}
		else
		{
			send.sData[nIndex].nRankingChange = 500 + ( 500 - pEquipRoleNode->nNum );
		}
	}

	SendMessage(&send, send.dwSize);

	return E_Success;
}

DWORD PlayerSession::HandleSynTotalStrength(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_SynTotalStrength send;
	send.nTotalStrength = pRole->GetTotalStrength();
	SendMessage(&send, send.dwSize);

	return E_Success;
}

DWORD PlayerSession::HandleGetFlowerRankings(INT nPage)
{
	INT nStartNum = (nPage-1) * RANKINGS_PAGE_LEN + 1;

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	tagNS_GetRankingInfo send;
	send.nPage = nPage;
	send.eType = ERT_Flower;
	send.nLocalPlayerRank = pRole->GetPm(ERT_Flower);

	TMap<INT, tagRankingsNode*> LevelRankings = g_WorldRankingMgr.GetFlowerRanking();
	tagRankingsNode* pLevelRoleNode = NULL;

	INT nIndex = 0;
	for (INT i = nStartNum; i>=nStartNum && i<=nStartNum+RANKINGS_PAGE_LEN-1; ++i, nIndex++)
	{
		//tagRankingInfo TmpNode;
		//tagRoleIDName RoleIDName;
		pLevelRoleNode = LevelRankings.Peek(i);
		if (!P_VALID(pLevelRoleNode))
		{
			// 没有该名次的玩家		
			if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
			{
				ASSERT(FALSE);
				return GT_INVALID;
			}

			send.sData[nIndex].nCurRanking = 0;
			send.sData[nIndex].nRankingChange = 0;
			send.sData[nIndex].sIDName.dwID = 0;
			ZeroMemory(send.sData[nIndex].sIDName.szName, X_SHORT_NAME);

			break;
		}

		if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
		{
			ASSERT(FALSE);
			return GT_INVALID;
		}

		pLevelRoleNode->szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.sData[nIndex].sIDName.szName, X_SHORT_NAME, pLevelRoleNode->szRoleName);
		send.sData[nIndex].sIDName.dwID = pLevelRoleNode->dwRoleID;
		send.sData[nIndex].nCurRanking = pLevelRoleNode->nNum;
		//send.sData[nIndex].nRankingChange = pLevelRoleNode->nPreNum - pLevelRoleNode->nNum;
		send.sData[nIndex].n64Value = pLevelRoleNode->n64Value;
		if ( pLevelRoleNode->nPreNum != 0 )
		{
			send.sData[nIndex].nRankingChange = pLevelRoleNode->nPreNum - pLevelRoleNode->nNum;
		}
		else
		{
			send.sData[nIndex].nRankingChange = 500 + ( 500 - pLevelRoleNode->nNum );
		}
	}

	SendMessage(&send, send.dwSize);

	return E_Success;
}

DWORD PlayerSession::HandleGetEggRankings(INT nPage)
{
	INT nStartNum = (nPage-1) * RANKINGS_PAGE_LEN + 1;

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	tagNS_GetRankingInfo send;
	send.nPage = nPage;
	send.eType = ERT_Egg;
	send.nLocalPlayerRank = pRole->GetPm(ERT_Egg);

	TMap<INT, tagRankingsNode*> LevelRankings = g_WorldRankingMgr.GetEggRanking();
	tagRankingsNode* pLevelRoleNode = NULL;

	INT nIndex = 0;
	for (INT i = nStartNum; i>=nStartNum && i<=nStartNum+RANKINGS_PAGE_LEN-1; ++i, nIndex++)
	{
		//tagRankingInfo TmpNode;
		//tagRoleIDName RoleIDName;
		pLevelRoleNode = LevelRankings.Peek(i);
		if (!P_VALID(pLevelRoleNode))
		{
			// 没有该名次的玩家		
			if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
			{
				ASSERT(FALSE);
				return GT_INVALID;
			}

			send.sData[nIndex].nCurRanking = 0;
			send.sData[nIndex].nRankingChange = 0;
			send.sData[nIndex].sIDName.dwID = 0;
			ZeroMemory(send.sData[nIndex].sIDName.szName, X_SHORT_NAME);

			break;
		}

		if( nIndex > RANKINGS_PAGE_LEN || nIndex < 0)
		{
			ASSERT(FALSE);
			return GT_INVALID;
		}

		pLevelRoleNode->szRoleName[X_SHORT_NAME-1] = '\0';
		_tcscpy_s( send.sData[nIndex].sIDName.szName, X_SHORT_NAME, pLevelRoleNode->szRoleName);
		send.sData[nIndex].sIDName.dwID = pLevelRoleNode->dwRoleID;
		send.sData[nIndex].nCurRanking = pLevelRoleNode->nNum;
		//send.sData[nIndex].nRankingChange = pLevelRoleNode->nPreNum - pLevelRoleNode->nNum;
		send.sData[nIndex].n64Value = pLevelRoleNode->n64Value;
		if ( pLevelRoleNode->nPreNum != 0 )
		{
			send.sData[nIndex].nRankingChange = pLevelRoleNode->nPreNum - pLevelRoleNode->nNum;
		}
		else
		{
			send.sData[nIndex].nRankingChange = 500 + ( 500 - pLevelRoleNode->nNum );
		}
	}

	SendMessage(&send, send.dwSize);

	return E_Success;
}