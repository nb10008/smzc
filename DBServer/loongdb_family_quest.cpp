#include "StdAfx.h"

#include "../WorldDefine/family_define.h"
#include "LoongDB.h"

BOOL CLoongDB::RenewFamilyQuest(DWORD dwFamilyID, INT nProcess, BYTE byNum, tagFamilyQuest* pData)
{
	if( !P_VALID(pData) )
		return FALSE;

	if (!P_VALID(dwFamilyID))
		return FALSE;

	if (byNum > MAX_FAMILY_QUESTS_NUM)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->SetReplace("family_quest");
		pStream->FillString("LastQuestProcess = ") << nProcess;
		pStream->FillString(",IssueFlag = 1");
		
		for (int i=0; i<byNum; ++i)
		{
			if (!P_VALID(pData[i].dwQuestID) || pData[i].nTargetCount < 0)
			{
				return FALSE;
			}

			TCHAR szTmp[X_SHORT_STRING] = {0};
			_sntprintf(szTmp, X_SHORT_STRING, _T(",QuestID%d="), i+1);
			pStream->FillString(szTmp) << pData[i].dwQuestID;

			ZeroMemory(szTmp, X_SHORT_STRING);
			_sntprintf(szTmp, X_SHORT_STRING, _T(",QuestNum%d="), i+1);
			pStream->FillString(szTmp) << pData[i].nTargetCount;
		}
		pStream->FillString(",FamilyID=") << dwFamilyID;

		if( m_pDBLoong->Execute  ( pStream ) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return TRUE;
		}

		m_pDBLoong->ReturnStream(pStream);
	}

	return FALSE;	
}

BOOL CLoongDB::ClearFamilyMemRewarded(DWORD dwFamilyID)
{
	if(!P_VALID(dwFamilyID))
	{
		return FALSE;
	}

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->SetUpdate("family_member");
		pStream->FillString("BeRewarded=0");
		pStream->SetWhere();
		pStream->FillString("FamilyID=") << dwFamilyID;

		if( m_pDBLoong->Execute  ( pStream ) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return TRUE;
		}

		m_pDBLoong->ReturnStream(pStream);
	}

	return FALSE;	
}

BOOL CLoongDB::SaveFamilyQuest(DWORD dwFamilyID, LPVOID pQuestData)
{
	if (!P_VALID(dwFamilyID))
		return FALSE;

	if( !P_VALID(pQuestData) )
		return FALSE;

	tagFamilyQuest* pData = (tagFamilyQuest*) pQuestData;
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->SetUpdate("family_quest");

		TCHAR szTmp1[X_SHORT_STRING] = {0};
		TCHAR szTmp2[X_SHORT_STRING] = {0};
		for (int i=0; i<MAX_FAMILY_QUESTS_NUM; ++i, ++pData)
		{
			if (0!=i)
			{
				pStream->FillString(",");
			}

			if (!P_VALID(pData) || !P_VALID(pData->dwQuestID))
			{	
				_sntprintf(szTmp1, X_SHORT_STRING, _T("QuestID%d="), i+1);
				pStream->FillString(szTmp1) << 0;
				_sntprintf(szTmp2, X_SHORT_STRING, _T(",QuestNum%d="), i+1);
				pStream->FillString(szTmp2) << 0;
				continue;
			}

			_sntprintf(szTmp1, X_SHORT_STRING, _T("QuestID%d="), i+1);
			pStream->FillString(szTmp1) << pData->dwQuestID;

			_sntprintf(szTmp2, X_SHORT_STRING, _T(",QuestNum%d="), i+1);
			pStream->FillString(szTmp2) << pData->nTargetCount;
		}

		pStream->SetWhere();
		pStream->FillString("FamilyID=") << dwFamilyID;

		if( m_pDBLoong->Execute  ( pStream ) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return TRUE;
		}

		m_pDBLoong->ReturnStream(pStream);
	}

	return FALSE;	
}

BOOL CLoongDB::ChangeFamilyQuestIssueState(DWORD dwFamilyID, bool bFlag)
{
	if (!P_VALID(dwFamilyID))
	{
		return FALSE;
	}

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->SetUpdate("family_quest");
		pStream->FillString("IssueFlag=") << (BYTE)((false == bFlag) ? 0 : 1);
		pStream->SetWhere();
		pStream->FillString("FamilyID=") << dwFamilyID;

		if( m_pDBLoong->Execute  ( pStream ) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return TRUE;
		}

		m_pDBLoong->ReturnStream(pStream);
	}

	return FALSE;	
}

DWORD CLoongDB::LoadFamilyQuest(INT& nNum, LPVOID pData, DWORD dwFamilyID)
{
	BOOL bRet = FALSE;
	if(!P_VALID(dwFamilyID))
		bRet = Load(pData, &nNum, 0, 0, 
			&CLoongDB::FormatLoadAllFamilyQuest, &CLoongDB::ProcResLoadAllFamilyQuest);
	else
		bRet = Load(pData, &nNum, 0, dwFamilyID, 
			&CLoongDB::FormatLoadFamilyQuest, &CLoongDB::ProcResLoadAllFamilyQuest);

	if (0==nNum)
	{
		return TRUE;
	}

	return bRet;
}

BOOL CLoongDB::DeleteFamilyQuest(DWORD dwFamilyID)
{
	if (!P_VALID(dwFamilyID))
	{
		return FALSE;
	}

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->SetDelete("family_quest");
		pStream->SetWhere();
		pStream->FillString("FamilyID=") << dwFamilyID;

		if( m_pDBLoong->Execute  ( pStream ) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return TRUE;
		}

		m_pDBLoong->ReturnStream(pStream);
	}

	return FALSE;	
}