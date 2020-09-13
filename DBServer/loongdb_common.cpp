//------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loongdb_common.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: 数据库操作应用层实现 -- 保存数据相关
//------------------------------------------------------------------------------
#include "StdAfx.h"

#include "LoongDB.h"
#include "../WorldDefine/currency_define.h"
#include "../ServerDefine/role_data_define.h"

/************************* 读取 *****************************/

//------------------------------------------------------------------------------
// 读取账号公用信息
//------------------------------------------------------------------------------
BOOL CLoongDB::LoadAccountCommon(LPVOID pData, DWORD dwAccountID)
{
	return Load(pData, NULL, dwAccountID, GT_INVALID, 
				&CLoongDB::FormatLoadAccountCommon, 
				&CLoongDB::ProcResLoadAccountCommon);
}

VOID CLoongDB::FormatLoadAccountCommon(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("account_common", 
		"SafeCodeCrc,ResetTime,BagPsdCrc,BaiBaoYuanBao,WareSize,WareSilver,PersonalSet,LastUseRoleID,EMacc");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

VOID CLoongDB::ProcResLoadAccountCommon(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, tagAccountCommon);

	p->sSafeCode.dwSafeCodeCrc	= (*pRes)[0].GetDword();
	p->sSafeCode.dwTimeReset	= (*pRes)[1].GetDword();
	p->dwBagPsdCrc				= (*pRes)[2].GetDword();
	p->nBaiBaoYuanBao			= (*pRes)[3].GetInt();
	p->n16WareSize				= (*pRes)[4].GetInt();
	p->n64WareSilver			= (*pRes)[5].GetLong();
	(*pRes)[6].GetBlob(&p->stPersonalSet,min((*pRes)[6].GetLen(),sizeof(tagDWPlayerPersonalSet)));

	p->dwLastUseRoleID			=	(*pRes)[7].GetDword();
	p->eSpec = (EAccountSpec)(*pRes)[8].GetInt();
}

//------------------------------------------------------------------------------
// 查询一下运营商ID
//------------------------------------------------------------------------------
BOOL CLoongDB::LoadAccountChannelIDCommon(DWORD dwAccountID,LPVOID pData)
{
	return Load(pData, NULL, dwAccountID, GT_INVALID, 
		&CLoongDB::FormatLoadAccountChannelIDCommon, 
		&CLoongDB::ProcResLoadAccountChannelIDCommon);
}


VOID CLoongDB::FormatLoadAccountChannelIDCommon(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("account_common", 
		"ChannelID");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

VOID CLoongDB::ProcResLoadAccountChannelIDCommon(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, DWORD);

	*p = (*pRes)[0].GetDword();
}

//------------------------------------------------------------------------------
// 更新最新的账号信息
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateNewAccountData(LPVOID pData, DWORD dwAccountID)
{
      return Update(dwAccountID,pData, 1, 0, (&CLoongDB::FormatUpdateNewAccountData));
}


VOID CLoongDB::FormatUpdateNewAccountData(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNewAccountCommon);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetUpdate("account_common");
	pStream->FillString("ChannelID=") << p->_dwChannelID;
	pStream->FillString(",AccountName='").FillString(p->szNewAccountName, pCon).FillString("'");

	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;   
	m_pDBLoong->ReturnConnection(pCon);
}

//------------------------------------------------------------------------------
// 锁住account_common
//------------------------------------------------------------------------------
BOOL CLoongDB::LockAccountCommon()
{
	return LockTables("account_common");
}

/************************* 更新 *****************************/

//------------------------------------------------------------------------------
// 插入帐号记录
//------------------------------------------------------------------------------
BOOL CLoongDB::InsertAccountCommon(DWORD dwAccountID, LPVOID pData)
{
	return Update(dwAccountID, pData, 1, 0, (&CLoongDB::FormatInsertAccountCommon));
}

VOID CLoongDB::FormatInsertAccountCommon(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	
    MTRANS_POINTER(p, pData, tagNewAccountCommon);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetInsert("account_common");
	pStream->FillString("AccountID=") << dwAccountID;
	pStream->FillString(",ChannelID=") << p->_dwChannelID;
	pStream->FillString(",AccountName='").FillString(p->szNewAccountName, pCon).FillString("'");

	m_pDBLoong->ReturnConnection(pCon);
}

//------------------------------------------------------------------------------
// 更新安全码
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateSafeCode(DWORD dwAccountID, DWORD dwSafeCodeCrc)
{
	return Update(dwAccountID, &dwSafeCodeCrc, 1, sizeof(DWORD), (&CLoongDB::FormatUpdateSafeCode));
}

VOID CLoongDB::FormatUpdateSafeCode(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("SafeCodeCrc=") << *(DWORD*)pData;
	pStream->FillString(",ResetTime=") << (DWORD)GT_INVALID;

	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//------------------------------------------------------------------------------
// 更新安全码重置时间
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateSafeCodeResetTime(DWORD dwAccountID, DWORD dwTimeReset)
{
	return Update(dwAccountID, &dwTimeReset, 1, sizeof(DWORD), (&CLoongDB::FormatUpdateSafeCodeResetTime));
}

VOID CLoongDB::FormatUpdateSafeCodeResetTime(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("ResetTime=") << *(DWORD*)pData;

	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//------------------------------------------------------------------------------
// 更新帐号下背包密码
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateBagPsd(DWORD dwAccountID, DWORD dwNewBagPsd)
{
	return Update(dwAccountID, &dwNewBagPsd, 1, 0, (&CLoongDB::FormatUpdateBagPsd));
}

VOID CLoongDB::FormatUpdateBagPsd(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("BagPsdCrc=") << *(DWORD*)pData;

	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//------------------------------------------------------------------------------
// 更新角色仓库大小
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateWareSize(DWORD dwAccountID, LPVOID pData)
{
	return Update(dwAccountID, pData, 1, 0, (&CLoongDB::FormatUpdateWareSize));
}

VOID CLoongDB::FormatUpdateWareSize(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("WareSize=") << *(INT16*)pData;

	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//------------------------------------------------------------------------------
// 更新角色仓库金钱
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateWareMoney(DWORD dwAccountID, INT64 n64WareSilver)
{
	return Update(dwAccountID, (LPVOID)&n64WareSilver, 1, 0, (&CLoongDB::FormatUpdateWareMoney));
}

VOID CLoongDB::FormatUpdateWareMoney(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("WareSilver=WareSilver+") << *(INT64*)pData;
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//------------------------------------------------------------------------------
// 更新百宝袋元宝
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateBaiBaoYuanBao(DWORD dwAccountID, LPVOID pData)
{
	return Update(dwAccountID, pData, 1, 0, (&CLoongDB::FormatUpdateBaiBaoYuanBao));
}

VOID CLoongDB::FormatUpdateBaiBaoYuanBao(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("BaiBaoYuanBao=BaiBaoYuanBao+") << *(INT*)pData;

	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//------------------------------------------------------------------------------
// 更新背包元宝
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateBagYuanBao(DWORD dwRole, LPVOID pData)
{
	return Update(dwRole, pData, 1, 0, (&CLoongDB::FormatUpdateBagYuanBao));
}

VOID CLoongDB::FormatUpdateBagYuanBao(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("roledata");
	pStream->FillString("BagYuanBao=") << *(INT*)pData;

	pStream->SetWhere();
	pStream->FillString("roleID=") << dwRoleID;
}

BOOL CLoongDB::UpdateBagGold(DWORD dwRoleID, LPVOID pData)
{
	return Update(dwRoleID, pData, 1, 0, (&CLoongDB::FormatUpdateBagGold));
}

VOID CLoongDB::FormatUpdateBagGold(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("roledata");
	pStream->FillString("BagGold=") << *(INT*)pData;

	pStream->SetWhere();
	pStream->FillString("roleID=") << dwRoleID;
}

BOOL CLoongDB::IncBagYuanBao(DWORD dwRoleID, LPVOID pData)
{
	BOOL bRet = TRUE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->Clear();
	pStream->SetSelect("roledata", "BagYuanBao");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bRet = FALSE;
		goto Exit;
	}
	// 检验并保存查询结果
	INT nResCnt = pResult->GetRowCount();
	if (nResCnt == 1)
	{
		INT nYuanBao = (*pResult)[0].GetInt();
		nYuanBao += *(INT*)pData;
		if (nYuanBao > MAX_BAG_YUANBAO_NUM)
			nYuanBao = MAX_BAG_YUANBAO_NUM;
		UpdateBagYuanBao(dwRoleID, (LPVOID)&nYuanBao);
	}
	else
	{
		bRet = FALSE;
	}

Exit:
	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return bRet;
}
BOOL CLoongDB::IncBagGold(DWORD dwRoleID, LPVOID pData)
{
	BOOL bRet = TRUE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->Clear();
	pStream->SetSelect("roledata", "BagGold");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bRet = FALSE;
		goto Exit;
	}
	// 检验并保存查询结果
	INT nResCnt = pResult->GetRowCount();
	if (nResCnt == 1)
	{
		INT nGold = (*pResult)[0].GetInt();
		nGold += *(INT*)pData;
		if (nGold > MAX_BAG_GOLD_NUM)
			nGold = MAX_BAG_GOLD_NUM;
		UpdateBagGold(dwRoleID, (LPVOID)&nGold);
	}
	else
	{
		bRet = FALSE;
	}

Exit:
	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return bRet;
}

//------------------------------------------------------------------------------
// 更新用户设置
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdatePersonalSet(DWORD dwAccountID, LPVOID pData)
{
	return Update(dwAccountID, pData, 1, 0, (&CLoongDB::FormatUpdatePersonalSet));
}

VOID CLoongDB::FormatUpdatePersonalSet(Beton::MyStream *pStream, DWORD dwAccountID, LPVOID pData)
{

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetUpdate("account_common");
	pStream->FillString("PersonalSet='").FillBlob(pData,sizeof(tagDWPlayerPersonalSet),pCon).FillString("'");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;

	m_pDBLoong->ReturnConnection(pCon);
}

//------------------------------------------------------------------------------
// 上次选择的人物
//------------------------------------------------------------------------------
BOOL CLoongDB::UpdateLastUseRoleID(DWORD dwRoleID, LPVOID pData)
{
	return Update(dwRoleID, pData, 1, 0, (&CLoongDB::FormatUpdateLastUseRoleID));
}

VOID CLoongDB::FormatUpdateLastUseRoleID(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetUpdate("account_common");
	pStream->FillString("LastUseRoleID=")<<dwRoleID;
	pStream->SetWhere();
	pStream->FillString("AccountID=") << *(DWORD *)pData;
}