//------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loongdb_family.cpp
// actor:
// last:
//------------------------------------------------------------------------------
#include "StdAfx.h"

#include "LoongDB.h"
#include "../WorldDefine/family_define.h"
#include "../ServerDefine/msg_family.h"

DWORD CLoongDB::CreateFamily(tagFamilyLoad& sFamilyLoad)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetInsert("family");
	pStream->FillString("FamilyID=")<<sFamilyLoad.sFamilyAtt.dwFamilyID;
	pStream->FillString(",FamilyName='");
	pStream->FillString(sFamilyLoad.strFamilyName).FillString("'");
	pStream->FillString(",LeaderID=")<<sFamilyLoad.sFamilyAtt.dwLeaderID;
	pStream->FillString(",FounderID=")<<sFamilyLoad.sFamilyAtt.dwFounderID;
	pStream->FillString(",Active=")<<sFamilyLoad.sFamilyAtt.nActive;
	pStream->FillString(",CreateTime=now()");

	BOOL bRet = m_pDBLoong->Execute(pStream);
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::SaveAllFamily(INT nFamilyNum, tagFamilyLoad* pFamilyLoad)
{
	if (nFamilyNum == 0 || !P_VALID(pFamilyLoad))
		return GT_INVALID;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	for (INT nCnt = 0; nCnt < nFamilyNum; nCnt++)
	{
		pStream->Clear();
		pStream->SetUpdate("family");
		pStream->FillString("LeaderID=")<<pFamilyLoad[nCnt].sFamilyAtt.dwLeaderID;
		pStream->FillString(",FounderID=")<<pFamilyLoad[nCnt].sFamilyAtt.dwFounderID;
		pStream->FillString(",Active=")<<pFamilyLoad[nCnt].sFamilyAtt.nActive;
		pStream->FillString(",FamilyName='");
		pStream->FillString(pFamilyLoad[nCnt].strFamilyName);
		pStream->FillString("'");
		char szDate[X_DATATIME_LEN + 1];
		ZeroMemory(szDate, sizeof(szDate));
		DwordTime2DataTime(szDate, X_DATATIME_LEN + 1, pFamilyLoad[nCnt].sFamilyAtt.dwCreateTime);
		pStream->FillString(",CreateTime='").FillString(szDate);
		pStream->FillString("'");
		pStream->SetWhere();
		pStream->FillString("FamilyID=")<<pFamilyLoad[nCnt].sFamilyAtt.dwFamilyID;
		m_pDBLoong->Execute(pStream);
	}
	m_pDBLoong->ReturnStream(pStream);

	return E_Success;
}

DWORD CLoongDB::DismissFamily(DWORD dwFamilyID)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetDelete("family");
	pStream->SetWhere();
	pStream->FillString("FamilyID=")<<dwFamilyID;

	BOOL bRet = m_pDBLoong->Execute(pStream);
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::LoadAllFamily(LPVOID pBuff, INT32& nFamilyNum)
{
	return Load(pBuff, &nFamilyNum, GT_INVALID, GT_INVALID, 
		&CLoongDB::FormatLoadAllFamily, &CLoongDB::ProcResLoadAllFamily);
}
DWORD CLoongDB::LoadFamilyMember(LPVOID pBuff, INT32& nFamilyMemNum)
{
	return Load(pBuff, &nFamilyMemNum, GT_INVALID, GT_INVALID, 
		&CLoongDB::FormatLoadAllFamilyMember, &CLoongDB::ProcResLoadAllFamilyMember);
}
VOID CLoongDB::FormatLoadAllFamilyMember(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("family_member", 
		"RoleID,"	"FamilyID,"		"JoinTime,"		"IsLeader,"		"BeRewarded");
}
VOID CLoongDB::FormatLoadAllFamily(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("family", 
		"FamilyID,"			"FamilyName,"			"LeaderID,"	"FounderID,"		"Active,"
		"CreateTime");
	pStream->SetWhere();
	pStream->FillString("FamilyID in (select distinct FamilyID from family_member)");
}
VOID CLoongDB::ProcResLoadAllFamilyMember(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, tagFamilyMember);

	*pNum = pRes->GetRowCount();
	INT a = 0;

	for(INT32 i=0; i<*pNum; ++i)
	{
		p->dwRoleID			= (*pRes)[0].GetDword();
		p->dwFamilyID		= (*pRes)[1].GetDword();
		if( p->dwFamilyID == GT_INVALID )
		{
			++a;
			pRes->NextRow();
			continue;
		}
		DataTime2DwordTime(*(tagDWORDTime*)&(p->dwJoinTime), (*pRes)[2].GetString(), (*pRes)[2].GetLen());
		p->bLeader	=(*pRes)[3].GetBool();
		p->bRewarded = (*pRes)[4].GetBool();

		pRes->NextRow();
		++p;
	}
	if( a > 0 )
		*pNum -= a;
}
VOID CLoongDB::ProcResLoadAllFamily(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, tagFamilyLoad);

	*pNum = pRes->GetRowCount();
	INT a = 0;

	for(INT32 i=0; i<*pNum; ++i)
	{
		p->sFamilyAtt.dwFamilyID				= (*pRes)[0].GetDword();
		if( p->sFamilyAtt.dwFamilyID == GT_INVALID )
		{
			++a;
			pRes->NextRow();
			continue;
		}
		p->sFamilyAtt.dwLeaderID	= (*pRes)[2].GetDword();
		p->sFamilyAtt.dwFounderID = (*pRes)[3].GetDword();
		p->sFamilyAtt.nActive			= (*pRes)[4].GetInt();
		DataTime2DwordTime(*(tagDWORDTime*)&(p->sFamilyAtt.dwCreateTime), (*pRes)[5].GetString(), (*pRes)[5].GetLen());

		(*pRes)[1].GetTCHAR(p->strFamilyName, MAX_FAMILY_NAME);

		pRes->NextRow();
		++p;
	}
	if( a > 0 )
		*pNum -= a;
}

DWORD CLoongDB::SaveFamilyAtt(tagFamilyAtt& sFamilyAtt)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetUpdate("family");
	pStream->FillString("LeaderID=")<<sFamilyAtt.dwLeaderID;
	pStream->FillString(",FounderID=")<<sFamilyAtt.dwFounderID;
	pStream->FillString(",Active=")<<sFamilyAtt.nActive;
	char szDate[X_DATATIME_LEN + 1];
	ZeroMemory(szDate, sizeof(szDate));
	DwordTime2DataTime(szDate, X_DATATIME_LEN + 1, sFamilyAtt.dwCreateTime);
	pStream->FillString(",CreateTime='").FillString(szDate);
	pStream->FillString("'");
	pStream->SetWhere();
	pStream->FillString("FamilyID=")<<sFamilyAtt.dwFamilyID;

	BOOL bRet = m_pDBLoong->Execute(pStream);
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::SaveFamilyActive(DWORD dwFamilyID, INT nActive)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetUpdate("family");
	pStream->FillString("Active=")<<nActive;
	pStream->SetWhere();
	pStream->FillString("FamilyID=")<<dwFamilyID;

	BOOL bRet = m_pDBLoong->Execute(pStream);
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::JoinFamily(tagFamilyMember& sFamilyMember)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetInsert("family_member");
	pStream->FillString("FamilyID=")<<sFamilyMember.dwFamilyID;
	pStream->FillString(",RoleID=")<<sFamilyMember.dwRoleID;
	pStream->FillString(",BeRewarded=")<<sFamilyMember.bRewarded;
	pStream->FillString(",JoinTime=now()");

	BOOL bRet = m_pDBLoong->Execute(pStream);
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::LeaveFamily(DWORD dwRoleID)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetDelete("family_member");
	pStream->SetWhere();
	pStream->FillString("RoleID=")<<dwRoleID;

	BOOL bRet = m_pDBLoong->Execute(pStream);

	if (bRet)
	{
		pStream->Clear();
		pStream->SetUpdate("roledata");
		pStream->FillString("FamilyID=4294967295");
		pStream->SetWhere();
		pStream->FillString("RoleID=")<<dwRoleID;
		bRet = m_pDBLoong->Execute(pStream);
	}

	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::SetLeader(DWORD dwRoleID, BOOL bLeader)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetUpdate("family_member");
	pStream->FillString("IsLeader=")<<bLeader;
	pStream->SetWhere();
	pStream->FillString("RoleID=")<<dwRoleID;

	BOOL bRet = m_pDBLoong->Execute(pStream);
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::SaveRoleFamilyQuestRewarded(DWORD dwFamilyID, DWORD dwRoleID)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) 
		return FALSE;

	pStream->Clear();
	pStream->SetUpdate("family_member");
	pStream->FillString("BeRewarded=1");
	pStream->SetWhere();
	pStream->FillString("RoleID=")<<dwRoleID;
	pStream->FillString(" and FamilyID=")<<dwFamilyID;

	BOOL bRet = m_pDBLoong->Execute(pStream);
	m_pDBLoong->ReturnStream(pStream);

	return bRet;	
}

VOID CLoongDB::FormatLoadAllFamilyQuest(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("family_quest","*");
}

VOID CLoongDB::FormatLoadFamilyQuest(Beton::MyStream *pStream, DWORD dwDummy, DWORD dwFamilyID)
{
	pStream->SetSelect("family_quest","*");
	pStream->SetWhere();
	pStream->FillString("FamilyID=") <<dwFamilyID;
}

VOID CLoongDB::ProcResLoadAllFamilyQuest(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, tagFamilyQuestLoadData);

	*pNum = pRes->GetRowCount();
	INT a = 0;

	for(INT32 i=0; i<*pNum; ++i)
	{
		if (!P_VALID(p))
		{
			return;
		}

		p->dwFamilyID = (*pRes)[0].GetDword();
		p->nProcess = (*pRes)[1].GetInt();
		p->bIssueState = (1==(*pRes)[2].GetByte()) ? true : false;
	
		for (int i=0, j=3; i<MAX_FAMILY_QUESTS_NUM; ++i, ++j)
		{
			p->QuestData[i].dwQuestID = (*pRes)[j+i].GetDword();
			p->QuestData[i].nTargetCount = (*pRes)[j+i+1].GetInt();
		}

		pRes->NextRow();
		++p;
	}
}
