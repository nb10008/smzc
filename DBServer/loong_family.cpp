//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: LoongProcMsg.cpp
// author: Sxg
// actor:
// data: 2008-06-04
// last:
// brief: 处理从游戏服务器接收到的消息(检查消息内容的合法性、请求DB处理及反馈)
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "LoongDB.h"
#include "Loong.h"
#include "../WorldDefine/msg_family.h"
#include "../ServerDefine/msg_family.h"

#define MFamilyMemMsgSz(nNum)	((nNum - 1) * sizeof(tagFamilyMember) + sizeof(tagNDBS_LoadFamilyMember))

DWORD CLoong::CreateFamily(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_CreateFamily);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	return m_pDB->CreateFamily(pRecv->sFamilyLoad);
}
DWORD CLoong::DismissFamily(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DismissFamily);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	return m_pDB->DismissFamily(pRecv->dwFamilyID);
}
DWORD CLoong::LoadAllFamily(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadAllFamily);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 预先分配一个足够大的空间
	CHAR *pBuffLoad = new CHAR[MAX_FAMILY_BUFF_SIZE];
	ZeroMemory(pBuffLoad, sizeof(CHAR) * MAX_FAMILY_BUFF_SIZE);

	INT32 nFamilyNum = 0;
	if(m_pDB->LoadAllFamily(pBuffLoad, nFamilyNum))
	{
		MTRANS_POINTER(pFamilyLoad, pBuffLoad, tagFamilyLoad);
		tagNDBS_LoadAllFamily send;
		for(INT32 i=0; i<nFamilyNum; ++i)
		{
			memcpy(&send.sFamilyLoad, pFamilyLoad + i, sizeof(tagFamilyLoad));

			SendMsg( &send, send.dwSize);
		}
	}

	SAFE_DEL_ARRAY(pBuffLoad);

	return E_Success;
}
DWORD CLoong::LoadFamilyMember(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LoadFamilyMember);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 预先分配一个足够大的空间
	CHAR *pBuffLoad = new CHAR[MAX_FAMILY_BUFF_SIZE];
	ZeroMemory(pBuffLoad, sizeof(CHAR) * MAX_FAMILY_BUFF_SIZE);

	INT32 nFamilyMemNum = 0;
	if(m_pDB->LoadFamilyMember(pBuffLoad, nFamilyMemNum))
	{
		CHAR *pBuffSend = new CHAR[MFamilyMemMsgSz(MAX_FAMILYMEM_NUM_PER_MSG)];

		tagNDBS_LoadFamilyMember *pSend = (tagNDBS_LoadFamilyMember*)(pBuffSend);
		ZeroMemory(pBuffSend, MFamilyMemMsgSz(MAX_FAMILYMEM_NUM_PER_MSG));
		MMSGINIT(pSend, tagNDBS_LoadFamilyMember);

		MTRANS_POINTER(pCur, pBuffLoad, tagFamilyMember);
		INT32 nSendLeft = nFamilyMemNum;
		INT32 nSendNum	= 0;
		while(nSendLeft > 0)
		{
			pCur = &pCur[nSendNum];

			if(nSendLeft > MAX_FAMILYMEM_NUM_PER_MSG)
			{
				nSendNum = MAX_FAMILYMEM_NUM_PER_MSG;
			}
			else
			{
				nSendNum = nSendLeft;
			}

			memcpy(pSend->sFamilyMember, pCur, nSendNum * sizeof(tagFamilyMember));
			pSend->nMemberNum	= nSendNum;
			pSend->dwSize		= MFamilyMemMsgSz(nSendNum);

			SendMsg( pSend, pSend->dwSize);

			nSendLeft -= nSendNum;
		}

		SAFE_DEL_ARRAY(pBuffSend);
	}
	// 帮派成员初始化结束
	tagNDBS_FamilyMemberInitOK send;
	SendMsg(&send, send.dwSize);

	SAFE_DEL_ARRAY(pBuffLoad);

	return E_Success;
}
DWORD CLoong::FamilyInitOK(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_FamilyInitOK);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	tagNDBS_FamilyInitOK send;
	SendMsg(&send, send.dwSize);

	return E_Success;
}

DWORD CLoong::SaveAllFamily(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveAllFamily);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	return m_pDB->SaveAllFamily(pRecv->nFamilyNum, pRecv->pFamilyLoad);
}
DWORD CLoong::SaveFamilyAtt(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveFamilyAtt);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	return m_pDB->SaveFamilyAtt(pRecv->sFamilyAtt);
}
DWORD CLoong::SaveFamilyActive(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveFamilyActive);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	return m_pDB->SaveFamilyActive(pRecv->dwFamilyID, pRecv->nActive);
}
DWORD CLoong::JoinFamily(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_JoinFamily);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	return m_pDB->JoinFamily(pRecv->sFamilyMemInfo);
}
DWORD CLoong::LeaveFamily(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_LeaveFamily);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	return m_pDB->LeaveFamily(pRecv->dwRoleID);
}
DWORD CLoong::SetLeader(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SetLeader);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	return m_pDB->SetLeader(pRecv->dwRoleID, pRecv->bLeader);
}

DWORD CLoong::SaveRoleFamilyQuestRewarded(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_SaveRoleFamilyQuestRewarded);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	return m_pDB->SaveRoleFamilyQuestRewarded(pRecv->dwFamilyID, pRecv->dwRoleID);
}

DWORD CLoong::RequestFamilyQuest(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_RequestFamilyQuest);
	if (!P_VALID(pRecv) || !P_VALID(pRecv->dwFamilyID))
		return GT_INVALID;

	BYTE* pData = new BYTE[1024];
	if (!P_VALID(pData))
	{
		return GT_INVALID;
	}

	ZeroMemory(pData, sizeof(BYTE)*1024);

	INT nNum = 0;
	if(TRUE != m_pDB->LoadFamilyQuest(nNum, pData, pRecv->dwFamilyID))
	{
		SAFE_DEL_ARRAY(pData);
		return GT_INVALID;
	}

	if (nNum <= 0)
	{
		tagNDBS_LoadFamilyQuest send;
		send.dwErrorCode = E_Success;
		send.nNum = 0;
		SendMsg(&send, send.dwSize);
		SAFE_DEL_ARRAY(pData);
		return 0;
	}

	tagNDBS_LoadFamilyQuest msg;
	msg.dwErrorCode = E_Success;
	msg.nNum = 1;
	memcpy(&(msg.QuestData), pData, sizeof(tagFamilyQuestLoadData));
	SendMsg(&msg, msg.dwSize);

	SAFE_DEL_ARRAY(pData);
	return 0;
}

DWORD CLoong::DeleteFamilyQuest(DWORD pDBMsg, DWORD dwDummy)
{
	MGET_MSG(pRecv, pDBMsg, NDBC_DeleteFamilyQuest);
	if (!P_VALID(pRecv) || !P_VALID(pRecv->dwFamilyID))
		return GT_INVALID;

	return m_pDB->DeleteFamilyQuest(pRecv->dwFamilyID);
}
