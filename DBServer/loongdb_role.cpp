//------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loongdb_role.cpp
// author: Sxg
// actor:
// data: 2009-05-04
// last:
// brief: 数据库操作应用层实现 -- 角色表(roledata)操作相关
//------------------------------------------------------------------------------
#include "StdAfx.h"

#include "LoongDB.h"
//-------------------------------------------------------------------------------------------------------
// 修改角色名称
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ChangeRoleName(DWORD dwAccountID, DWORD dwRoleID, LPCTSTR szNewRoleName)
{
	BOOL bRet = TRUE;

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	// 格式化数据
	pStream->SetUpdate("roledata");
	pStream->FillString("RoleName='").FillString(szNewRoleName, pCon);
	pStream->FillString("',RoleNameCrc=") << m_pUtil->Crc32(szNewRoleName);
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and AccountID=") << dwAccountID;

	// 释放流和连接
	m_pDBLoong->ReturnConnection(pCon);

	// 执行
	bRet = m_pDBLoong->Execute(pStream);
	
	m_pDBLoong->ReturnStream(pStream);

	++m_dwWriteTimes;

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// 角色使用过的名字历史记录
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveNameHistory(DWORD dwAccountID, DWORD dwRoleID, LPCTSTR szNewRoleName)
{
	BOOL bRet = TRUE;

	// 获取流
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	
	pStream->SetSelect("name_history", "*");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and AccountID=") << dwAccountID;

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		INT i = 9;
		while ( i > 0 )
		{
			char* strName = NULL;
			char szTmpColume1[X_SHORT_STRING] = {0};
			sprintf( szTmpColume1, "name_%d", i );
			
			pStream->Clear();
			pStream->SetSelect("name_history", szTmpColume1);
			pStream->SetWhere();
			pStream->FillString("RoleID=") << dwRoleID;
			pStream->FillString(" and AccountID=") << dwAccountID;

			// 查询数据库
			Beton::QueryResult* pRes = m_pDBLoong->Query(pStream);
			if(P_VALID(pRes) && pRes->GetRowCount() > 0)
			{
				strName = (char *)(*pRes)[0].GetString();
				if( *strName == '0' )
				{
					i--;
					m_pDBLoong->FreeQueryResult(pRes);
					continue;
				}
			}
			else // 不会执行到这里
			{
				if(P_VALID(pRes)) m_pDBLoong->FreeQueryResult(pRes);
				m_pDBLoong->ReturnConnection(pCon);
				m_pDBLoong->ReturnStream(pStream);
				return FALSE;
			}

			// 错位写
			char szTmpColume2[X_SHORT_STRING] = {0};
			sprintf( szTmpColume2, "name_%d", i+1 );
			pStream->Clear();
			pStream->SetUpdate("name_history");
			pStream->FillString(szTmpColume2).FillString("='").FillString(strName,pCon).FillString("'");
			pStream->SetWhere();
			pStream->FillString("RoleID=") << dwRoleID;
			pStream->FillString(" and AccountID=") << dwAccountID;

			m_pDBLoong->FreeQueryResult(pRes);
			// 执行
			m_pDBLoong->Execute(pStream);

			i--;
		}

		// 写入最新使用的名字
		pStream->Clear();
		pStream->SetUpdate("name_history");
		pStream->FillString("name_1='").FillString(szNewRoleName, pCon).FillString("'");
		pStream->SetWhere();
		pStream->FillString("RoleID=") << dwRoleID;
		pStream->FillString(" and AccountID=") << dwAccountID;

		// 执行
		bRet = m_pDBLoong->Execute(pStream);
	}
	else
	{
		// 写入最新使用的名字
		pStream->Clear();
		pStream->SetInsert("name_history");
		pStream->FillString("name_1='").FillString(szNewRoleName, pCon).FillString("'");
		pStream->FillString(",RoleID=") << dwRoleID;
		pStream->FillString(",AccountID=") << dwAccountID;

		// 执行
		bRet = m_pDBLoong->Execute(pStream);
	}

	m_pDBLoong->ReturnConnection(pCon);
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// 修改角色帮派id
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ChangeRoleGuild(DWORD dwRoleID, DWORD dwGuildID)
{
	return Update(dwRoleID, &dwGuildID, 1, 0, &CLoongDB::FormatUpdateChangeRoleGuild);
}

VOID CLoongDB::FormatUpdateChangeRoleGuild(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("roledata");
	pStream->FillString("GuildID=") << *(DWORD*)pData;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 修改角色dressMdID
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ChangeRoleDressMdID(DWORD dwRoleID, WORD wValue)
{
	return Update(dwRoleID, &wValue, 1, 0, &CLoongDB::FormatUpdateChangeRoleDressMdID);
}

VOID CLoongDB::FormatUpdateChangeRoleDressMdID(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("roledata");
	pStream->FillString("DressModelID=") << *(WORD*)pData;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 设置角色删除标志位
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SetRoleRemoveFlag(DWORD dwRoleID, DWORD dwTime)
{
	return Update(dwRoleID, &dwTime, 1, 0, &CLoongDB::FormatUpdateSetRoleRemoveFlag);
}

BOOL CLoongDB::SetRoleClanRemoveFlag(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, &CLoongDB::FormatUpdateSetRoleClanRemoveFlag);
}

VOID CLoongDB::FormatUpdateSetRoleRemoveFlag(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTime)
{
	pStream->SetUpdate("roledata");
	pStream->FillString("RemoveFlag=") << 1;
	pStream->FillString(",RemoveTime=") << *(DWORD*)pTime;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

VOID CLoongDB::FormatUpdateSetRoleClanRemoveFlag(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("clan_data");
	pStream->FillString("RemoveFlag=") << 1;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}