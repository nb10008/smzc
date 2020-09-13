//------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: loongdb_guild.cpp
// author: Sxg
// actor:
// data: 2008-04-14
// last:
// brief: 数据库操作应用层实现 -- 保存稀有领域数据相关
//------------------------------------------------------------------------------
#include "StdAfx.h"

#include "LoongDB.h"
#include "../WorldDefine/container_define.h"
#include "../ServerDefine/item_define.h"

//------------------------------------------------------------------------------
// 读取稀有领域
//------------------------------------------------------------------------------
BOOL CLoongDB::LoadAllRareArea(LPVOID pBuff, INT32& nGuildNum)
{
	return Load(pBuff, &nGuildNum, GT_INVALID, GT_INVALID, 
				&CLoongDB::FormatLoadAllRareArea, &CLoongDB::ProcResLoadAllRareArea);
}
//------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadAllRareArea(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("rarearea", 
		"AreaID,"	"ItemID,"	"DataID,"	"State,"	"LeaveTime,"
		"RecordTime");
}
//------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadAllRareArea(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, tagGlobalLimitedItem);

	*pNum = pRes->GetRowCount();

	for( INT32 i = 0; i < *pNum; ++i )
	{
		p->dwAreaID				= (*pRes)[0].GetDword();
		p->dwID					= (*pRes)[1].GetDword();
		p->n64ID				= (*pRes)[2].GetLong();
		p->eType				= (EGlobalLimitedItemExistType)(*pRes)[3].GetInt();
		p->nTime				= (*pRes)[4].GetInt();
		p->dwRecordTime			= (*pRes)[5].GetDword();

		pRes->NextRow();
		++p;
	}
}

//------------------------------------------------------------------------------

DWORD CLoongDB::SaveRareItem(LPVOID pData)
{
	tagGlobalLimitedItem* pItem = (tagGlobalLimitedItem*)pData;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->Clear();
	pStream->SetInsert("rarearea");
	pStream->FillString("ItemID=") << pItem->dwID;
	pStream->FillString(",AreaID=") << pItem->dwAreaID;
	pStream->FillString(",DataID=") << pItem->n64ID;
	pStream->FillString(",State=") << (INT)pItem->eType;
	pStream->FillString(",LeaveTime=") << pItem->nTime;
	pStream->FillString(",RecordTime=") << pItem->dwRecordTime;

	BOOL bRes = m_pDBLoong->Execute(pStream);
	
	m_pDBLoong->ReturnStream(pStream);
	return bRes;
}

//------------------------------------------------------------------------------
DWORD CLoongDB::UnSaveRareItem(DWORD dwID, INT64 n64Serial)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->Clear();
	pStream->SetDelete("rarearea");
	pStream->SetWhere();
	pStream->FillString("ItemID=") << dwID;
	pStream->FillString(" and DataID=") << n64Serial;

	BOOL bRes = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);
	return bRes;
}

//------------------------------------------------------------------------------
// 创建稀有领域
//BOOL CLoongDB::InsertRareArea(tagGlobalLimitedItem* pGlobalLimitedItem)
//{
//	return Update(GT_INVALID, pGlobalLimitedItem, 1, 0, &CLoongDB::FormatInsertRareArea);
//}
////------------------------------------------------------------------------------
//VOID CLoongDB::FormatInsertRareArea(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	MTRANS_POINTER(p, pData, tagGlobalLimitedItem);
//
//	if (!P_VALID(p))
//	{
//		m_bUpdateFormatError = TRUE;
//		return ;
//	}
//
//	// 获取连接
//	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
//
//	pStream->SetInsert("rarearea");
//	pStream->FillString("AreaID=") << p->dwAreaID;
//	pStream->FillString(",ItemID=") << p->dwAreaID;
//	pStream->FillString(",DataID=") << p->n64ID;
//	pStream->FillString(",State=") << p->eType;
//	pStream->FillString(",LeaveTime=") << p->nTime;
//	pStream->FillString(",RecordTime=") << p->dwRecordTime;
//
//	// 释放连接
//	m_pDBLoong->ReturnConnection(pCon);
//}
//------------------------------------------------------------------------------
// 更新稀有领域
BOOL CLoongDB::UpdateRareArea(tagGlobalLimitedItem* pGlobalLimitedItem, INT64 nOriginal64ID)
{
	//return Update(GT_INVALID, pGlobalLimitedItem, 1, 0, &CLoongDB::FormatUpdateRareArea);

	if(!P_VALID(pGlobalLimitedItem))
		return FALSE;

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();
	pStream->SetUpdate("rarearea");
	pStream->FillString(" ItemID=") << pGlobalLimitedItem->dwID;
	pStream->FillString(",DataID=") << pGlobalLimitedItem->n64ID;
	pStream->FillString(",State=") << pGlobalLimitedItem->eType;
	pStream->FillString(",LeaveTime=") << pGlobalLimitedItem->nTime;
	pStream->FillString(",RecordTime=") << pGlobalLimitedItem->dwRecordTime;
	pStream->SetWhere();
	pStream->FillString("AreaID=") << pGlobalLimitedItem->dwAreaID;
	pStream->FillString(" and DataID=") << nOriginal64ID;


	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

VOID CLoongDB::FormatUpdateRareArea(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	//MTRANS_POINTER(p, pData, tagGlobalLimitedItem);

	//if (!P_VALID(p))
	//{
	//	m_bUpdateFormatError = TRUE;
	//	return ;
	//}

	//// 获取连接
	//Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	//pStream->SetUpdate("rarearea");
	//pStream->FillString(",ItemID=") << p->dwAreaID;
	//pStream->FillString(",DataID=") << p->n64ID;
	//pStream->FillString(",State=") << p->eType;
	//pStream->FillString(",LeaveTime=") << p->nTime;
	//pStream->FillString(",RecordTime=now()");
	//pStream->SetWhere();
	//pStream->FillString("AreaID=") << p->dwAreaID;
	//pStream->FillString("and DataID=") << p->nOriginal64ID;

	//// 释放连接
	//m_pDBLoong->ReturnConnection(pCon);
}
//------------------------------------------------------------------------------
// 删除稀有领域
//BOOL CLoongDB::DeleteRareArea(tagGlobalLimitedItem* pGlobalLimitedItem)
//{
//	return Update(GT_INVALID, pGlobalLimitedItem, 1, 0, &CLoongDB::FormatDeleteRareArea);
//}
//
//VOID CLoongDB::FormatDeleteRareArea(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	MTRANS_POINTER(p, pData, tagGlobalLimitedItem);
//
//	if (!P_VALID(p))
//	{
//		m_bUpdateFormatError = TRUE;
//		return ;
//	}
//
//	pStream->SetDelete("rarearea");
//	pStream->SetWhere();
//	pStream->FillString("AreaID=") << p->dwAreaID;
//	pStream->FillString(" and DataID=") << p->n64ID;
//}
//------------------------------------------------------------------------------
 //创建稀有领域
//BOOL CLoongDB::SaveRareItem(tagGlobalLimitedItem* pGlobalLimitedItem)
//{
//	return Update(GT_INVALID, pGlobalLimitedItem, 1, sizeof(tagGlobalLimitedItem), (&CLoongDB::FormatInsertRareArea));
//}
////------------------------------------------------------------------------------
//VOID CLoongDB::FormatInsertRareArea(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	MTRANS_POINTER(p, pData, tagGlobalLimitedItem);
//
//	if (!P_VALID(p))
//	{
//		m_bUpdateFormatError = TRUE;
//		return ;
//	}
//
//	pStream->SetInsert("rarearea");
//	pStream->FillString("AreaID=") << p->dwAreaID;
//	pStream->FillString(",ItemID=") << p->dwID;
//	pStream->FillString(",DataID=") << p->n64ID;
//	pStream->FillString(",State=") << (INT)p->eType;
//	pStream->FillString(",LeaveTime=") << p->nTime;
//	pStream->FillString(",RecordTime=now()");
//}

