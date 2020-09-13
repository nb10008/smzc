//-----------------------------------------------------------------------------
//!\file	client_db.cpp
//!\brief	客户端数据库管理类
//!
//!\date	2009-04-20
//! last	2009-04-20
//!\author	zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "client_db.h"

//-----------------------------------------------------------------------------
// 初始化
//-----------------------------------------------------------------------------
BOOL ClientDB::Init(LPCTSTR strHost, LPCTSTR strUser, LPCTSTR strPassword, LPCTSTR strDatabase, INT nPort)
{
	m_pUtil = "Util";

	BOOL bRet = m_DB.Init(strHost, strUser, strPassword, strDatabase, nPort);

	return bRet;
}

//------------------------------------------------------------------------------
// 销毁
//------------------------------------------------------------------------------
VOID ClientDB::Destroy()
{

}

DWORD ClientDB::GetRoleOldID(tagRole* pRole)
{
	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return GT_INVALID;

	// 获取一条连接
	Connection* pCon = m_DB.GetFreeConnection();

	QueryResult* pResult = NULL;

	pStream->SetSelect("roledata", "RoleID");
	pStream->SetWhere();
	pStream->FillString("RoleName='").FillString(pRole->szRoleName).FillString("'");

	pResult = m_DB.Query(pStream);

	DWORD dwErr = GetLastError();

	if( P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		DWORD dwRoleID = (*pResult)[0].GetDword();
		return dwRoleID;
	}
	else
	{
		// to do 报错
		return GT_INVALID;
	}
	

	// 释放资源
	m_DB.FreeQueryResult(pResult);
	m_DB.ReturnConnection(pCon);
	m_DB.ReturnStream(pStream);
}