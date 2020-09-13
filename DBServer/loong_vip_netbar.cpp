#include "StdAfx.h"
#include "Loong.h"
#include "LoongDB.h"
#include "../ServerDefine/msg_vip_netbar.h"
DWORD CLoong::UpdateVNBPlayer( DWORD pDBMsg, DWORD dwDummy )
{
	MTRANS_POINTER(pRecv, pDBMsg, tagNDBC_UpdateVNBPlayer);
	m_pDB->UpdateVNBPlayer(pRecv->dwAccountID, pRecv->dwLoginTime);

	return E_Success;
}

DWORD CLoong::LoadVNBData( DWORD pDBMsg, DWORD dwDummy )
{
	MTRANS_POINTER(pRecv, pDBMsg, tagNDBC_GetVNBData);

	BYTE* pTmpMem = new BYTE[1024*1024*X_SHORT_STRING];
	if (!P_VALID(pTmpMem))
	{
		return GT_INVALID;
	}
	ZeroMemory(pTmpMem, 1024*1024*X_SHORT_STRING*sizeof(BYTE));
	tagNDBS_GetVNBData send;
	send.eType = EVMT_FirstOne;
	send.players.nHisPlayers = 0;
	send.players.nTodaysPlayers = 0;
	SendMsg(&send, send.dwSize);

	tagDBVNBPlayers* pData = (tagDBVNBPlayers*)pTmpMem;
	m_pDB->LoadVNBData(pData, pRecv->dwDate);

	if (pData->nHisPlayers <= 0)
	{
		SAFE_DEL_ARRAY(pTmpMem);
		return 0;
	}

	// 先发所有
	INT nSendCount = (pData->nHisPlayers)/VNB_Load_Msg_Max_Data_Once + 1;
	INT nLastDataNum = (pData->nHisPlayers)%VNB_Load_Msg_Max_Data_Once;
	while (nSendCount>0)
	{
		INT nSize = 0;
		INT nAccountNum = 0;
		if (nSendCount>1)
		{
			nSize = sizeof(tagNDBS_GetVNBData)+(VNB_Load_Msg_Max_Data_Once-1)*sizeof(DWORD);
			nAccountNum = VNB_Load_Msg_Max_Data_Once;
		}
		else
		{
			// 最后一个
			if (nLastDataNum<=0)
			{
				break;
			}
			nSize = sizeof(tagNDBS_GetVNBData)+(nLastDataNum-1)*sizeof(DWORD);
			nAccountNum = nLastDataNum;
		}

		MCREATE_MSG(pSend, nSize, NDBS_GetVNBData);
		pSend->dwSize = nSize;
		pSend->eType = EVMT_Middle;
		pSend->players.nTodaysPlayers = 0;
		pSend->players.nHisPlayers = nAccountNum;
		memcpy(pSend->players.dwAccountIDs, 
			&(pData->dwAccountIDs[(nSendCount-1)*VNB_Load_Msg_Max_Data_Once]), 
			sizeof(DWORD)*nAccountNum); 
		SendMsg( pSend, pSend->dwSize);

		nSendCount--;
	}
	
	// 再发指定时间段的
	if (pData->nTodaysPlayers <= 0)
	{
		SAFE_DEL_ARRAY(pTmpMem);
		return 0;
	}

	nSendCount = (pData->nTodaysPlayers)/VNB_Load_Msg_Max_Data_Once + 1;
	nLastDataNum = (pData->nTodaysPlayers)%VNB_Load_Msg_Max_Data_Once;
	while (nSendCount>0)
	{
		INT nSize = 0;
		INT nAccountNum = 0;
		if (nSendCount>1)
		{
			nSize = sizeof(tagNDBS_GetVNBData)+(VNB_Load_Msg_Max_Data_Once-1)*sizeof(DWORD);
			nAccountNum = VNB_Load_Msg_Max_Data_Once;
		}
		else
		{
			// 最后一个
			if (nLastDataNum<=0)
			{
				tagNDBS_GetVNBData send;
				send.eType = EVMT_LastOne;
				send.players.nHisPlayers = 0;
				send.players.nTodaysPlayers = 0;
				SendMsg(&send, send.dwSize);
				break;
			}
			nSize = sizeof(tagNDBS_GetVNBData)+(nLastDataNum-1)*sizeof(DWORD);
			nAccountNum = nLastDataNum;
		}

		MCREATE_MSG(pSend, nSize, NDBS_GetVNBData);
		pSend->dwSize = nSize;
		pSend->eType = EVMT_Middle;
		pSend->players.nHisPlayers = 0;
		pSend->players.nTodaysPlayers = nAccountNum;
		memcpy(pSend->players.dwAccountIDs, 
			&(pData->dwAccountIDs[pData->nHisPlayers + (nSendCount-1)*VNB_Load_Msg_Max_Data_Once]), 
			sizeof(DWORD)*nAccountNum); 
		if (nAccountNum!=VNB_Load_Msg_Max_Data_Once)
		{
			pSend->eType = EVMT_LastOne;
		}
		SendMsg( pSend, pSend->dwSize);

		nSendCount--;
	}

	SAFE_DEL_ARRAY(pTmpMem);
	return E_Success;
}

BOOL CLoongDB::UpdateVNBPlayer(DWORD dwAccountID, DWORD dwLoginTime)
{
	return Update(dwAccountID, &dwLoginTime, 1, sizeof(DWORD), &CLoongDB::FormatUpdateVNBPlayer);
}

VOID CLoongDB::FormatUpdateVNBPlayer(Beton::MyStream *pStream, DWORD dwAccountID, PVOID pData)
{
	pStream->SetReplace("vip_netbar");
	pStream->FillString("accountid=") << dwAccountID;
	pStream->FillString(",login_time=") << *((DWORD*)pData);
}

BOOL CLoongDB::LoadVNBData(OUT PVOID pData, DWORD dwDate)
{
	MTRANS_POINTER(pVNBData, pData, tagDBVNBPlayers);
	PVOID pTmp = pVNBData->dwAccountIDs;

	Load(pTmp, &pVNBData->nHisPlayers, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadHistroyVNBPlayers, &CLoongDB::ProcResLoadVNBPlayers);
	Load(pTmp, &pVNBData->nTodaysPlayers, dwDate, GT_INVALID, &CLoongDB::FormatLoadTodayVNBPlayers, &CLoongDB::ProcResLoadVNBPlayers);

	return TRUE;
}

VOID CLoongDB::FormatLoadHistroyVNBPlayers(Beton::MyStream *pStream, DWORD dwDate, DWORD dwData)
{
	pStream->SetSelect("vip_netbar", "accountid, login_time");
}

VOID CLoongDB::FormatLoadTodayVNBPlayers(Beton::MyStream *pStream, DWORD dwDate, DWORD dwData)
{
	pStream->SetSelect("vip_netbar", "accountid, login_time");
	pStream->SetWhere();
	pStream->FillString("login_time=")<<dwDate;
}

VOID CLoongDB::ProcResLoadVNBPlayers(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pAccountIDs, pData, DWORD);
	*pNum = pRes->GetRowCount();

	for(DWORD i=0; i<*pNum; ++i)
	{
		pAccountIDs[i]		= (*pRes)[0].GetDword();
		pRes->NextRow();
	}

	pData = &pAccountIDs[*pNum];
}



//-------------------------------------------------------------------------
// 台湾金牌网吧 加红利点
//-------------------------------------------------------------------------
DWORD CLoong::AddBonusPoint( DWORD pDBMsg, DWORD dwDummy )
{
	MTRANS_POINTER(pRecv, pDBMsg, tagNDBC_Orange_AddBonusPoint);
	m_pDB->InsertBonusPoint(pRecv->dwIP, pRecv);

	return E_Success;
}

BOOL CLoongDB::InsertBonusPoint( DWORD dwIP,PVOID pData)
{
	return Update(dwIP, pData, 1, sizeof(tagNDBC_Orange_AddBonusPoint), &CLoongDB::FormatInsertBonusPoint);
}

VOID CLoongDB::FormatInsertBonusPoint(Beton::MyStream *pStream,DWORD dwIP,PVOID pData)
{
	MTRANS_POINTER(pRecv, pData, tagNDBC_Orange_AddBonusPoint);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetInsert("bonus");
	pStream->FillString("datetime = now()");
	pStream->FillString(",accountname = '").FillString(pRecv->szAccountName,pCon).FillString("'");
	pStream->FillString(",charname = '").FillString(pRecv->szRoleName,pCon).FillString("'");

	TObjRef<StreamTransport> pTrans;
	pTrans = "StreamTransport";

	char szIP[X_IP_LEN] = {_T('\0')};
	strcpy(szIP,pTrans->IP2String(dwIP));
	pStream->FillString(",ip = '").FillString(szIP).FillString("'");

	pStream->FillString(",bonuspoint = ") << pRecv->nBonusPoint;

	m_pDBLoong->ReturnConnection(pCon);


}