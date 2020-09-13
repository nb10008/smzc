#include "stdafx.h"
#include "world.h"
#include "db_session.h"
#include "role_mgr.h"
#include "role.h"
#include "pet_pocket.h"
#include "pet_soul.h"
#include "WorldRankings.h"

WorldRankingMgr g_WorldRankingMgr;

WorldRankingMgr::WorldRankingMgr()
{
	for (INT i=0; i<ERT_All; i++)
	{
		m_UpdateMark[i] = FALSE;
	}

	m_dwLastUpdate = 0;
}

BOOL WorldRankingMgr::Init()
{
	tagNDBC_InitRankings send;
	g_dbSession.Send( &send, send.dwSize );

	m_dwLastUpdate = GetCurrentDWORDTime();
	return TRUE;
}

//VOID WorldRankingMgr::Update()
//{
//	tagDWORDTime now =GetCurrentDWORDTime();
//
//	if (( now.hour == 0 && now.min == 0 && now.sec < 1 ) && ( m_dwLastUpdate.year != now.year || m_dwLastUpdate.month != now.month || m_dwLastUpdate.day != now.day ))
//	{
//		m_dwLastUpdate = GetCurrentDWORDTime();
//
//		// 零点更新排行榜
//		tagNDBC_UpdateRankings send;
//		g_dbSession.Send(&send, send.dwSize);
//	}
//
//	// 更新标记全为真后（代表所有排行的表已经更新完毕）重新载入各排行
//	for (INT i=0; i<ERT_All; i++)
//	{
//		if ( !m_UpdateMark[i] )
//		{
//			return;
//		}
//	}
//
//	// 更新标记全为真才能执行到这里
//	Init();
//
//	for (INT i=0; i<ERT_All; i++)
//	{
//		SetUpdateMark((ERankingType)i, FALSE);
//	}
//}

VOID WorldRankingMgr::DoInit(tagNDBS_InitRankings* pData)
{
	if (!P_VALID(pData))
	{
		return;
	}

	if ( pData->eType == ERT_Strength )
	{
		DoInitStrengthRankings(pData);
	}
	else if ( pData->eType == ERT_Level )
	{
		DoInitLevelRankings(pData);
	}
	else if ( pData->eType == ERT_Fame )
	{
		DoInitFameRankings(pData);
	}
	/*else if ( pData->eType == ERT_Pet )
	{
		DoInitPetRankings(pData);
	}*/
	else if ( pData->eType == ERT_Equip )
	{
		DoInitEquipRankings(pData);
	}
	else if ( pData->eType == ERT_WuXun )
	{
		DoInitWuXunRankings(pData);
	}
}

VOID WorldRankingMgr::SyncCanRankFlag(DWORD dwRoleID, BYTE byCanRankFlag)
{
	tagNDBC_SyncCanRankFlag send;
	send.dwRoleID = dwRoleID;
	send.byCanRankFlag = 0;// 角色是否能进入排行,0不能，1能
	g_dbSession.Send(&send, send.dwSize);
}

VOID WorldRankingMgr::DoInitStrengthRankings(tagNDBS_InitRankings* pRecv)
{	
	if (!P_VALID(pRecv))
	{
		return;
	}

	INT nCount = pRecv->nSum;
	ASSERT(nCount <= MAX_RANKINGS_LEN);

	m_Lock.Acquire();

	// 清空现有的排行榜
	tagRankingsNode* pTemp = NULL;
	//m_mapRankings.ResetIterator();
	TMap<INT, tagRankingsNode*>::TMapIterator it = m_mapStrengthRanking.Begin();

	while (m_mapStrengthRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapStrengthRanking.Clear();

	const INT32 nSecInOneWeek = 604800; //7*24*60*60
	INT nCurRank = 0;

	// 生成新的排行榜
	for(INT i=0; i<nCount; ++i)
	{
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRecv->Rankings[i].dwRoleID);
		if (!P_VALID(pRoleInfo))
			continue;
		if (pRoleInfo->bOnline == FALSE &&
			(pRoleInfo->dwTimeLastLogout == 0 ||
			CalcTimeDiff(GetCurrentDWORDTime(), pRoleInfo->dwTimeLastLogout) > nSecInOneWeek))
		{
			SyncCanRankFlag(pRoleInfo->dwRoleID, 0);
			continue;
		}
		else
		{
			nCurRank++;
		}

		tagRankingsNode* pRankingsNode = new tagRankingsNode;

		pRankingsNode->eType = ERT_Strength;
		pRankingsNode->nNum = nCurRank/*pRecv->Rankings[i].nNum*/;
		pRankingsNode->nPreNum = pRecv->Rankings[i].nPreNum;
		
		{
			// 一旦排行榜数据有误，保证服务器不会崩
			pRecv->Rankings[i].szRoleName[X_SHORT_NAME-1] = '\0';
			_tcscpy_s(pRankingsNode->szRoleName, X_SHORT_NAME, pRecv->Rankings[i].szRoleName);
		}
		
		pRankingsNode->dwRoleID = pRecv->Rankings[i].dwRoleID;//g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(pRecv->Rankings[i].szRoleName)); // 根据所获得的角色名字获得角色ID
		pRankingsNode->n64Value = pRecv->Rankings[i].n64Value;

		m_mapStrengthRanking.Add(pRankingsNode->nNum, pRankingsNode);

		Role* pRole = g_roleMgr.GetRolePtrByID(pRankingsNode->dwRoleID);
		if (P_VALID(pRole))
		{
			pRole->SetStrengthPm(pRankingsNode->nNum);
		}
	}

	m_Lock.Release();
	
}

VOID WorldRankingMgr::DoInitLevelRankings(tagNDBS_InitRankings* pRecv)
{	
	if (!P_VALID(pRecv))
	{
		return;
	}
	
	INT nCount = pRecv->nSum;
	ASSERT(nCount <= MAX_RANKINGS_LEN);

	m_Lock.Acquire();

	// 清空现有的排行榜
	tagRankingsNode* pTemp = NULL;
	//m_mapRankings.ResetIterator();
	TMap<INT, tagRankingsNode*>::TMapIterator it = m_mapLevelRanking.Begin();

	while (m_mapLevelRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapLevelRanking.Clear();

	const INT32 nSecInOneWeek = 604800; //7*24*60*60
	INT nCurRank = 0;

	// 生成新的排行榜
	for(INT i=0; i<nCount; ++i)
	{
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRecv->Rankings[i].dwRoleID);
		if (!P_VALID(pRoleInfo))
			continue;
		if (pRoleInfo->bOnline == FALSE &&
			(pRoleInfo->dwTimeLastLogout == 0 ||
			CalcTimeDiff(GetCurrentDWORDTime(), pRoleInfo->dwTimeLastLogout) > nSecInOneWeek))
		{
			SyncCanRankFlag(pRoleInfo->dwRoleID, 0);
			continue;
		}
		else
		{
			nCurRank++;
		}

		tagRankingsNode* pRankingsNode = new tagRankingsNode;
		pRankingsNode->eType = ERT_Level;
		pRankingsNode->nNum = nCurRank/*pRecv->Rankings[i].nNum*/;
		pRankingsNode->nPreNum = pRecv->Rankings[i].nPreNum;

		{
			// 一旦排行榜数据有误，保证服务器不会崩
			pRecv->Rankings[i].szRoleName[X_SHORT_NAME-1] = '\0';
			_tcscpy_s(pRankingsNode->szRoleName, X_SHORT_NAME, pRecv->Rankings[i].szRoleName);
		}

		pRankingsNode->dwRoleID = pRecv->Rankings[i].dwRoleID;//g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(pRecv->Rankings[i].szRoleName)); // 根据所获得的角色名字获得角色ID
		pRankingsNode->n64Value = pRecv->Rankings[i].n64Value;

		m_mapLevelRanking.Add(pRankingsNode->nNum, pRankingsNode);

		Role* pRole = g_roleMgr.GetRolePtrByID(pRankingsNode->dwRoleID);
		if (P_VALID(pRole))
		{
			pRole->SetLevelPm(pRankingsNode->nNum);
		}
	}

	m_Lock.Release();

}

VOID WorldRankingMgr::DoInitFameRankings(tagNDBS_InitRankings* pRecv)
{	
	if (!P_VALID(pRecv))
	{
		return;
	}

	INT nCount = pRecv->nSum;
	ASSERT(nCount <= MAX_RANKINGS_LEN);

	m_Lock.Acquire();

	// 清空现有的排行榜
	tagRankingsNode* pTemp = NULL;
	//m_mapRankings.ResetIterator();
	TMap<INT, tagRankingsNode*>::TMapIterator it = m_mapFameRanking.Begin();

	while (m_mapFameRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapFameRanking.Clear();

	// 生成新的排行榜
	for(INT i=0 ; i<nCount; ++i)
	{
		tagRankingsNode* pRankingsNode = new tagRankingsNode;
		pRankingsNode->eType = ERT_Fame;
		pRankingsNode->nNum = pRecv->Rankings[i].nNum;
		pRankingsNode->nPreNum = pRecv->Rankings[i].nPreNum;
		pRankingsNode->n64Value = pRecv->Rankings[i].n64Value;
		pRankingsNode->dwRoleID = pRecv->Rankings[i].dwRoleID;
		g_roleMgr.GetRoleNameByID(pRecv->Rankings[i].dwRoleID, pRankingsNode->szRoleName);
		
		m_mapFameRanking.Add(pRecv->Rankings[i].nNum, pRankingsNode);

		Role* pRole = g_roleMgr.GetRolePtrByID(pRankingsNode->dwRoleID);
		if (P_VALID(pRole))
		{
			pRole->GetClanData().SetFamePm(pRankingsNode->nNum);
		}
	}

	m_Lock.Release();

}

VOID WorldRankingMgr::DoInitEquipRankings( tagNDBS_InitRankings* pRecv )
{
	if (!P_VALID(pRecv))
	{
		return;
	}

	INT nCount = pRecv->nSum;
	ASSERT(nCount <= MAX_RANKINGS_LEN);

	m_Lock.Acquire();

	// 清空现有的排行榜
	tagRankingsNode* pTemp = NULL;
	//m_mapRankings.ResetIterator();
	TMap<INT, tagRankingsNode*>::TMapIterator it = m_mapEquipRanking.Begin();

	while (m_mapEquipRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}

	m_mapEquipRanking.Clear();

	const INT32 nSecInOneWeek = 604800; //7*24*60*60
	INT nCurRank = 0;

	// 生成新的排行榜
	for(INT i=0 ; i<nCount; ++i)
	{
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRecv->Rankings[i].dwRoleID);
		if (!P_VALID(pRoleInfo))
			continue;
		if (pRoleInfo->bOnline == FALSE &&
			(pRoleInfo->dwTimeLastLogout == 0 ||
			CalcTimeDiff(GetCurrentDWORDTime(), pRoleInfo->dwTimeLastLogout) > nSecInOneWeek))
		{
			SyncCanRankFlag(pRoleInfo->dwRoleID, 0);
			continue;
		}
		else
		{
			nCurRank++;
		}

		tagRankingsNode* pRankingsNode = new tagRankingsNode;
		pRankingsNode->eType = ERT_Equip;
		pRankingsNode->nNum = nCurRank/*pRecv->Rankings[i].nNum*/;
		pRankingsNode->nPreNum = pRecv->Rankings[i].nPreNum;
		pRankingsNode->dwRoleID = pRecv->Rankings[i].dwRoleID; 
		g_roleMgr.GetRoleNameByID(pRecv->Rankings[i].dwRoleID, pRankingsNode->szRoleName);
		pRankingsNode->n64Value = pRecv->Rankings[i].n64Value;

		m_mapEquipRanking.Add(pRankingsNode->nNum, pRankingsNode);

		Role* pRole = g_roleMgr.GetRolePtrByID(pRankingsNode->dwRoleID);
		if (P_VALID(pRole))
		{
			pRole->SetEquipPm(pRankingsNode->nNum);
		}
	}

	m_Lock.Release();
}

VOID WorldRankingMgr::DoInitPetRankings( tagNDBS_InitPetRankings* pRecv )
{
	if (!P_VALID(pRecv))
	{
		return;
	}

	INT nCount = pRecv->nSum;
	ASSERT(nCount <= MAX_RANKINGS_LEN);

	m_Lock.Acquire();

	// 清空现有的排行榜
	tagPetRankingsNode* pTemp = NULL;
	//m_mapRankings.ResetIterator();
	TMap<INT, tagPetRankingsNode*>::TMapIterator it = m_mapPetRanking.Begin();

	while (m_mapPetRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapPetRanking.Clear();

	// 生成新的排行榜
	for(INT i=0 ; i<nCount; ++i)
	{
		tagPetRankingsNode* pRankingsNode = new tagPetRankingsNode;
		pRankingsNode->eType = ERT_Pet;
		pRankingsNode->nNum = pRecv->Rankings[i].nNum;
		pRankingsNode->nPreNum = pRecv->Rankings[i].nPreNum;
		pRankingsNode->dwRoleID = pRecv->Rankings[i].dwRoleID;
		pRankingsNode->n64Value = pRecv->Rankings[i].n64Value;	// 宠物实力值
		pRankingsNode->dwPetID = pRecv->Rankings[i].dwPetID;	// 宠物ID
		
		{
			// 一旦排行榜数据有误，保证服务器不会崩
			pRecv->Rankings[i].szPetName[X_SHORT_NAME-1] = '\0';
			_tcscpy_s( pRankingsNode->szPetName, X_SHORT_NAME, pRecv->Rankings[i].szPetName );
		}

		m_mapPetRanking.Add(pRecv->Rankings[i].nNum, pRankingsNode);

		Role* pRole = g_roleMgr.GetRolePtrByID(pRankingsNode->dwRoleID);
		if (P_VALID(pRole))
		{
			DWORD dwPetIDs[CONST_MAX_PETSOUL_NUM] = {0};
			INT nCount = 0;
			pRole->GetPetPocket()->GetAllPetID(dwPetIDs, nCount);
			for(int i = 0; i<nCount; i++)
			{
				PetSoul* pPet = pRole->GetPetPocket()->GetPetSoul(dwPetIDs[i]);
				if (P_VALID(pPet))
				{
					pPet->GetPetAtt().SetPetPm(pRankingsNode->nNum);
				}
			}
		}
	}

	m_Lock.Release();
}




VOID WorldRankingMgr::DoInitWuXunRankings(tagNDBS_InitRankings* pRecv)
{	
	if (!P_VALID(pRecv))
	{
		return;
	}

	INT nCount = pRecv->nSum;
	ASSERT(nCount <= MAX_RANKINGS_LEN);

	m_Lock.Acquire();

	// 清空现有的排行榜
	tagRankingsNode* pTemp = NULL;
	//m_mapRankings.ResetIterator();
	TMap<INT, tagRankingsNode*>::TMapIterator it = m_mapWuXunRanking.Begin();

	while (m_mapWuXunRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapWuXunRanking.Clear();

	const INT32 nSecInOneWeek = 604800; //7*24*60*60
	INT nCurRank = 0;

	// 生成新的排行榜
	for(INT i=0; i<nCount; ++i)
	{
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRecv->Rankings[i].dwRoleID);
		if (!P_VALID(pRoleInfo))
			continue;
		if (pRoleInfo->bOnline == FALSE &&
			(pRoleInfo->dwTimeLastLogout == 0 ||
			CalcTimeDiff(GetCurrentDWORDTime(), pRoleInfo->dwTimeLastLogout) > nSecInOneWeek))
		{
			SyncCanRankFlag(pRoleInfo->dwRoleID, 0);
			continue;
		}
		else
		{
			nCurRank++;
		}

		tagRankingsNode* pRankingsNode = new tagRankingsNode;
		pRankingsNode->eType = ERT_WuXun; 
		pRankingsNode->nNum = nCurRank/*pRecv->Rankings[i].nNum*/;
		pRankingsNode->nPreNum = pRecv->Rankings[i].nPreNum;

		{
			// 一旦排行榜数据有误，保证服务器不会崩
			pRecv->Rankings[i].szRoleName[X_SHORT_NAME-1] = '\0';
			_tcscpy_s(pRankingsNode->szRoleName, X_SHORT_NAME, pRecv->Rankings[i].szRoleName);
		}

		pRankingsNode->dwRoleID = pRecv->Rankings[i].dwRoleID;//g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(pRecv->Rankings[i].szRoleName)); // 根据所获得的角色名字获得角色ID
		pRankingsNode->n64Value = pRecv->Rankings[i].n64Value;

		m_mapWuXunRanking.Add(pRankingsNode->nNum, pRankingsNode);
 
		Role* pRole = g_roleMgr.GetRolePtrByID(pRankingsNode->dwRoleID);
		if (P_VALID(pRole))
		{
			//pRole->SetLevelPm(pRankingsNode->nNum);
		}
	}

	m_Lock.Release();

}

VOID WorldRankingMgr::DoInitFlowerRankings(tagNDBS_InitFlowerRankings* pRecv)
{	
	if (!P_VALID(pRecv))
	{
		return;
	}

	INT nCount = pRecv->nSum;
	ASSERT(nCount <= MAX_FlowerEggRankings_LEN);

	m_Lock.Acquire();

	// 清空现有的排行榜
	tagRankingsNode* pTemp = NULL;
	//m_mapRankings.ResetIterator();
	TMap<INT, tagRankingsNode*>::TMapIterator it = m_mapLikeRanking.Begin();

	while (m_mapLikeRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapLikeRanking.Clear();

	const INT32 nSecInOneWeek = 604800; //7*24*60*60
	INT nCurRank = 0;

	// 生成新的排行榜
	for(INT i=0; i<nCount; ++i)
	{
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRecv->Rankings[i].dwRoleID);
		if (!P_VALID(pRoleInfo))
			continue;
		if (pRoleInfo->bOnline == FALSE &&
			(pRoleInfo->dwTimeLastLogout == 0 ||
			CalcTimeDiff(GetCurrentDWORDTime(), pRoleInfo->dwTimeLastLogout) > nSecInOneWeek))
		{
			SyncCanRankFlag(pRoleInfo->dwRoleID, 0);
			continue;
		}
		else
		{
			nCurRank++;
		}

		tagRankingsNode* pRankingsNode = new tagRankingsNode;
		pRankingsNode->eType = ERT_Flower; 
		pRankingsNode->nNum = nCurRank/*pRecv->Rankings[i].nNum*/;
		pRankingsNode->nPreNum = pRecv->Rankings[i].nPreNum;

		{
			// 一旦排行榜数据有误，保证服务器不会崩
			pRecv->Rankings[i].szRoleName[X_SHORT_NAME-1] = '\0';
			_tcscpy_s(pRankingsNode->szRoleName, X_SHORT_NAME, pRecv->Rankings[i].szRoleName);
		}

		pRankingsNode->dwRoleID = pRecv->Rankings[i].dwRoleID;//g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(pRecv->Rankings[i].szRoleName)); // 根据所获得的角色名字获得角色ID
		pRankingsNode->n64Value = pRecv->Rankings[i].n64Value;

		m_mapLikeRanking.Add(pRankingsNode->nNum, pRankingsNode);

		Role* pRole = g_roleMgr.GetRolePtrByID(pRankingsNode->dwRoleID);
		if (P_VALID(pRole))
		{
			//pRole->SetLevelPm(pRankingsNode->nNum);
		}
	}

	m_Lock.Release();

}

VOID WorldRankingMgr::DoInitEggRankings(tagNDBS_InitFlowerRankings* pRecv)
{	
	if (!P_VALID(pRecv))
	{
		return;
	}

	INT nCount = pRecv->nSum;
	ASSERT(nCount <= MAX_FlowerEggRankings_LEN);

	m_Lock.Acquire();

	// 清空现有的排行榜
	tagRankingsNode* pTemp = NULL;
	//m_mapRankings.ResetIterator();
	TMap<INT, tagRankingsNode*>::TMapIterator it = m_mapHateRanking.Begin();

	while (m_mapHateRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapHateRanking.Clear();

	const INT32 nSecInOneWeek = 604800; //7*24*60*60
	INT nCurRank = 0;

	// 生成新的排行榜
	for(INT i=0; i<nCount; ++i)
	{
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRecv->Rankings[i].dwRoleID);
		if (!P_VALID(pRoleInfo))
			continue;
		if (pRoleInfo->bOnline == FALSE &&
			(pRoleInfo->dwTimeLastLogout == 0 ||
			CalcTimeDiff(GetCurrentDWORDTime(), pRoleInfo->dwTimeLastLogout) > nSecInOneWeek))
		{
			SyncCanRankFlag(pRoleInfo->dwRoleID, 0);
			continue;
		}
		else
		{
			nCurRank++;
		}

		tagRankingsNode* pRankingsNode = new tagRankingsNode;
		pRankingsNode->eType = ERT_Egg; 
		pRankingsNode->nNum = nCurRank/*pRecv->Rankings[i].nNum*/;
		pRankingsNode->nPreNum = pRecv->Rankings[i].nPreNum;

		{
			// 一旦排行榜数据有误，保证服务器不会崩
			pRecv->Rankings[i].szRoleName[X_SHORT_NAME-1] = '\0';
			_tcscpy_s(pRankingsNode->szRoleName, X_SHORT_NAME, pRecv->Rankings[i].szRoleName);
		}

		pRankingsNode->dwRoleID = pRecv->Rankings[i].dwRoleID;//g_roleMgr.GetRoleIDByNameCrc(IUTIL->Crc32(pRecv->Rankings[i].szRoleName)); // 根据所获得的角色名字获得角色ID
		pRankingsNode->n64Value = pRecv->Rankings[i].n64Value;

		m_mapHateRanking.Add(pRankingsNode->nNum, pRankingsNode);

		Role* pRole = g_roleMgr.GetRolePtrByID(pRankingsNode->dwRoleID);
		if (P_VALID(pRole))
		{
			//pRole->SetLevelPm(pRankingsNode->nNum);
		}
	}

	m_Lock.Release();

}


void WorldRankingMgr::Destroy()
{
	tagRankingsNode* pTemp = NULL;
	//m_mapRankings.ResetIterator();
	TMap<INT, tagRankingsNode*>::TMapIterator it = m_mapStrengthRanking.Begin();

	while (m_mapStrengthRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapStrengthRanking.Clear();

	pTemp = NULL;
	it = m_mapLevelRanking.Begin();

	while (m_mapLevelRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapLevelRanking.Clear();

	pTemp = NULL;
	it = m_mapFameRanking.Begin();

	while (m_mapFameRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapFameRanking.Clear();

	pTemp = NULL;
	it = m_mapEquipRanking.Begin();

	while (m_mapEquipRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}

	m_mapEquipRanking.Clear();




	pTemp = NULL;
	it = m_mapWuXunRanking.Begin();

	while (m_mapWuXunRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}

	m_mapWuXunRanking.Clear();

	pTemp = NULL;
	it = m_mapLikeRanking.Begin();

	while(m_mapLikeRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapLikeRanking.Clear();

	pTemp = NULL;
	it = m_mapHateRanking.Begin();

	while(m_mapHateRanking.PeekNext(it, pTemp))
	{
		SAFE_DEL(pTemp);
	}
	m_mapHateRanking.Clear();



	tagPetRankingsNode* pTempPet = NULL;
	TMap<INT, tagPetRankingsNode*>::TMapIterator it2 = m_mapPetRanking.Begin();

	while (m_mapPetRanking.PeekNext(it2, pTempPet))
	{
		SAFE_DEL(pTempPet);
	}
	m_mapPetRanking.Clear();
}
