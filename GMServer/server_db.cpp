#include "StdAfx.h"
#include "server_db.h"

#include "../WorldDefine/container_define.h"
#include "../WorldDefine/ItemDefine.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/gm_tool_define.h"

BOOL ServerDB::Init( LPCTSTR strHost, LPCTSTR strUser, LPCTSTR strPassword, LPCTSTR strDatabase, INT nPort )
{
	m_pUtil = "Util";

	BOOL bRet = m_DB.Init(strHost, strUser, strPassword, strDatabase, nPort);

	return bRet;
}

VOID ServerDB::Destroy()
{

}

BOOL ServerDB::DBResumeRole( DWORD dwRoleID, LPCTSTR tszNewName )
{
	BOOL bRtv = FALSE;

	if( !P_VALID(tszNewName) ) return FALSE;

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 获取一条连接
	Connection* pCon = m_DB.GetFreeConnection();


	// 设置sql语句,将roledata的removeflag位修改
	pStream->SetUpdate("roledata");
	pStream->FillString("RemoveFlag=") << 0;
	pStream->FillString(",RemoveTime=") << DWORD(-1);

	if (_tcslen(tszNewName) > 0)
	{
		pStream->FillString(",RoleName='").FillString(tszNewName, pCon).FillString("'");
		DWORD dwNameCrc = m_pUtil->Crc32(tszNewName);
		pStream->FillString(",RoleNameCrc=") << dwNameCrc;
	}
	
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// 进行操作
	bRtv = m_DB.Execute(pStream);

	if (bRtv)
	{
		// 设置sql语句，将该角色对应的pet_data中的宠物记录的removeflag修改
		pStream->SetUpdate("pet_data");
		pStream->FillString("RemoveFlag=") << 0;
		pStream->SetWhere();
		pStream->FillString("master_id=") << dwRoleID;

		// 进行操作
		bRtv = m_DB.Execute(pStream);
	}

	// 归还连接
	m_DB.ReturnConnection(pCon);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBRoleModify(LPCTSTR tszName, INT nLel, INT nSkill, INT nYuanbao, INT64 n64Exp, INT64 n64Money, INT nAttPoint)
{
	BOOL bRtv = FALSE;

	if( !P_VALID(tszName) ) return FALSE;

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 获取一条连接
	Connection* pCon = m_DB.GetFreeConnection();

	// 设置sql语句
 	pStream->SetUpdate("roledata");
	
	BOOL bFirst = TRUE;
	if ((nLel > 0)&&(n64Exp >= 0))
	{
		pStream->FillString("Level=") << nLel;
		pStream->FillString(",ExpCurLevel=") << n64Exp;
		bFirst = FALSE;
	}
	if(nSkill>=0)
	{
		if(!bFirst)
			pStream->FillString(",");
		pStream->FillString("TalentPtAvail=") << nSkill;			
		bFirst = FALSE;
	}
	if(nYuanbao>=0)
	{
		if(!bFirst)
			pStream->FillString(",");
		pStream->FillString("BagYuanBao=") << nYuanbao;
		bFirst = FALSE;
	}
	if(n64Money>=0)
	{
		if(!bFirst)
			pStream->FillString(",");
		pStream->FillString("BagSilver=")	<< n64Money % 10000;
		pStream->FillString(",BagGold=")	<< n64Money / 10000;
		bFirst = FALSE;
	}
	if (nAttPoint>=0)
	{
		if (!bFirst)
			pStream->FillString(",");
		pStream->FillString("AttPtAvail=") << nAttPoint;
	}
	
 	pStream->SetWhere();
	pStream->FillString("RoleName='").FillString(tszName, pCon).FillString("'");

	// 进行操作
	bRtv = m_DB.Execute(pStream);

	// 归还连接
	m_DB.ReturnConnection(pCon);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}

DWORD ServerDB::DBQueryItemExist(LPCSTR szTableName, INT64 n64SerialNum)
{
	// 获取流
	Beton::MyStream* pStream = m_DB.GetStream();

	// 格式化
	pStream->SetSelect(szTableName, "*");
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64SerialNum;

	// 查询数据库
	Beton::QueryResult* pResult = m_DB.Query(pStream);
	if(!(P_VALID(pResult))) return E_GMTool_Err;

	if( pResult->GetRowCount() > 0 )
		return TRUE;
	else
		return FALSE;

	// 释放流和结果
	m_DB.ReturnStream(pStream);
	m_DB.FreeQueryResult(pResult);
}

DWORD ServerDB::DBDeleteItem( DWORD dwRoleID, INT64 n64ItemID )
{
	DWORD dwTypeID = GT_INVALID;
	if (!DBQueryTypeIDBySerial(n64ItemID, dwTypeID, "item") &&	!DBQueryTypeIDBySerial(n64ItemID, dwTypeID, "item_baibao") )
		return E_GMTool_DeleteItem_ItemNotExist;

	BOOL bRememberDel = FALSE;
	if (MIsEquipment(dwTypeID))
	{
		bRememberDel = TRUE;
		if ( !(DBDeleteBySerial(n64ItemID, "equip", TRUE) && DBDeleteBySerial(n64ItemID, "equip_baibao", TRUE)) )
			return E_GMTool_Err;
	}

	if ( !(DBDeleteBySerial(n64ItemID, "item", bRememberDel) && DBDeleteBySerial(n64ItemID, "item_baibao", bRememberDel)) )
		return E_GMTool_Err;

	return E_GMTool_Success;
}

DWORD ServerDB::DBResumeItem(INT64 n64SerialNum, DWORD dwTargetID, DWORD dwAccountID)
{
	DWORD dwTypeID = GT_INVALID;
	if ( !(DBQueryTypeIDBySerial(n64SerialNum, dwTypeID, "item_del")) )
	{
		return E_GMTool_ResumeItemNotExit;
	}

	if (!MIsEquipment(dwTypeID))
	{
		return E_GMTool_ResumeItemNotEquip;
	}

	//记录百宝袋是否插入成功
	BOOL bItemOK = FALSE;
	BOOL bEquipOK = FALSE;

	// 将del表中的数据分别恢复到item_baibao和equip_baibao中
	DWORD dwRtv = DBQueryItemExist("item", n64SerialNum);
	if( FALSE == DBQueryItemExist("item", n64SerialNum) 
		&& FALSE == DBQueryItemExist("item_baibao", n64SerialNum)
		&& FALSE == DBQueryItemExist("equip_baibao", n64SerialNum) 
		&& TRUE == DBQueryItemExist("item_del", n64SerialNum)
		&& TRUE == DBQueryItemExist("equip_del", n64SerialNum))
	{
		MyStream* pStream = m_DB.GetStream();
		MyStream* pStreamS = m_DB.GetStream();

		pStream->SetCopyItem("item_baibao", "*", "item_del");
		pStream->SetWhere();
		pStream->FillString("SerialNum=")<<n64SerialNum;

		pStreamS->SetCopyItem("equip_baibao", "*", "equip_del");
		pStreamS->SetWhere();
		pStreamS->FillString("SerialNum=")<<n64SerialNum;

		bItemOK = m_DB.Execute(pStream);
		bEquipOK = m_DB.Execute(pStreamS);

		m_DB.ReturnStream(pStream);
		m_DB.ReturnStream(pStreamS);
	}
	else
	{
		return E_GMTool_ResumeItemNotExit;
	}

	// 删除del表中的相应数据
	MyStream* pStreamT = m_DB.GetStream();
	MyStream* pStreamF = m_DB.GetStream();

	pStreamT->SetDelete("item_del");
	pStreamT->SetWhere();
	pStreamT->FillString("SerialNum=")<<n64SerialNum;

	pStreamF->SetDelete("equip_del");
	pStreamF->SetWhere();
	pStreamF->FillString("SerialNum=")<<n64SerialNum;

	//插入百宝袋成功，才删除del表数据
	if(bItemOK)
	{
		m_DB.Execute(pStreamT);
	}
	if(bEquipOK)
	{
		m_DB.Execute(pStreamF);
	}

	m_DB.ReturnStream(pStreamT);
	m_DB.ReturnStream(pStreamF);

	// 修改装备所有者
	if( dwTargetID != 0 && dwAccountID != 0 )
	{
		MyStream* pStreamR = m_DB.GetStream();

		pStreamR->SetUpdate("item_baibao");
		pStreamR->FillString("OwnerID=")<<dwTargetID;
		pStreamR->FillString(",AccountID=")<<dwAccountID;
		pStreamR->SetWhere();
		pStreamR->FillString("SerialNum=")<<n64SerialNum;

		m_DB.Execute(pStreamR);

		m_DB.ReturnStream(pStreamR);
	}
	
	// 设置在百宝表中的suffix位置
	MyStream* pStreamQ = m_DB.GetStream();

	pStreamQ->SetUpdate("item_baibao");
	pStreamQ->FillString("Suffix=0");
	pStreamQ->SetWhere();
	pStreamQ->FillString("SerialNum=")<<n64SerialNum;

	m_DB.Execute(pStreamQ);
	
	m_DB.ReturnStream(pStreamQ);
	
	return E_GMTool_Success;
}

BOOL ServerDB::DBQueryNameByRoleID( IN DWORD dwRoleID, OUT LPTSTR szName )
{
	BOOL bRtv = FALSE;

	if( !P_VALID(szName) ) return FALSE;

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 设置sql语句
	pStream->SetSelect("roledata", "RoleName");
	pStream->SetWhere();
	pStream->FillString("RoleID=")	<< dwRoleID;

	// 查询
	Beton::QueryResult* pResult = m_DB.Query(pStream);
	
	// 处理结果
	if (!P_VALID(pResult))
	{	
		szName[0] = 0;
		bRtv = FALSE;
	}
	else if (1 == pResult->GetRowCount())
	{
		(*pResult)[0].GetTCHAR(szName, X_SHORT_NAME);
		bRtv = TRUE;
	}
	else
	{
		szName[0] = 0;
		bRtv = FALSE;
	}


	// 回收结果
	m_DB.FreeQueryResult(pResult);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBQueryRoleIDByName( IN LPTSTR szName, OUT DWORD &dwRoleID )
{
	BOOL bRtv = FALSE;

	if( !P_VALID(szName) ) return FALSE;

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 获取一条连接
	Connection* pCon = m_DB.GetFreeConnection();

	// 设置sql语句
	pStream->SetSelect("roledata", "RoleID");
	pStream->SetWhere();
	pStream->FillString("RoleName='").FillString(szName, pCon).FillString("'");

	// 进行操作
	Beton::QueryResult* pResult = m_DB.Query(pStream);

	// 处理结果
	
	if (!P_VALID(pResult))
	{
		dwRoleID = GT_INVALID;
		bRtv = FALSE;
	}
	else if (1 == pResult->GetRowCount())
	{
		INT nLen = 0;
		dwRoleID = (*pResult)[0].GetDword();
		bRtv = TRUE;
	}
	else
	{
		dwRoleID = GT_INVALID;
		bRtv = FALSE;
	}

	// 回收结果
	m_DB.FreeQueryResult(pResult);

	// 归还连接
	m_DB.ReturnConnection(pCon);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBQueryAccountIDByName(IN LPTSTR szAccountName, OUT DWORD& dwAccountID)
{
	BOOL bRtv = FALSE;

	if( !P_VALID(szAccountName) ) return FALSE;

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 获取一条连接
	Connection* pCon = m_DB.GetFreeConnection();

	// 设置sql语句
	pStream->SetSelect("account", "id");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// 进行操作
	Beton::QueryResult* pResult = m_DB.Query(pStream);

	// 处理结果
	if (!P_VALID(pResult))
	{
		dwAccountID = GT_INVALID;
		bRtv = FALSE;
	}
	else if (1 == pResult->GetRowCount())
	{
		INT nLen = 0;
		dwAccountID = (*pResult)[0].GetDword();
		bRtv = TRUE;
	}
	else
	{
		dwAccountID = GT_INVALID;
		bRtv = FALSE;
	}

	// 回收结果
	m_DB.FreeQueryResult(pResult);

	// 归还连接
	m_DB.ReturnConnection(pCon);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBQueryAccountCommonIDByName(IN LPTSTR szAccountName, OUT DWORD& dwAccountID)
{
	BOOL bRtv = FALSE;

	if( !P_VALID(szAccountName) ) return FALSE;

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 获取一条连接
	Connection* pCon = m_DB.GetFreeConnection();

	// 设置sql语句
	pStream->SetSelect("account_common", "AccountID");
	pStream->SetWhere();
	pStream->FillString("AccountName='").FillString(szAccountName, pCon).FillString("'");

	// 进行操作
	Beton::QueryResult* pResult = m_DB.Query(pStream);

	// 处理结果
	if (!P_VALID(pResult))
	{
		dwAccountID = GT_INVALID;
		bRtv = FALSE;
	}
	else if (1 == pResult->GetRowCount())
	{
		INT nLen = 0;
		dwAccountID = (*pResult)[0].GetDword();
		bRtv = TRUE;
	}
	else
	{
		dwAccountID = GT_INVALID;
		bRtv = FALSE;
	}

	// 回收结果
	m_DB.FreeQueryResult(pResult);

	// 归还连接
	m_DB.ReturnConnection(pCon);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBQueryRoleIDByAccountID( IN DWORD dwAccountID, OUT DWORD &dwRoleID )
{
	ASSERT(0);
	return FALSE;
}

BOOL ServerDB::DBQueryAccountIDByRoleID( IN DWORD dwRoleID, OUT DWORD &dwAccountID )
{
	BOOL bRtv = FALSE;

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 获取一条连接
	Connection* pCon = m_DB.GetFreeConnection();

	// 设置sql语句
	pStream->SetSelect("roledata", "AccountID");
	pStream->SetWhere();
	pStream->FillString("RoleID=")	<< dwRoleID;

	// 进行操作
	Beton::QueryResult* pResult = m_DB.Query(pStream);

	// 处理结果
	if (!P_VALID(pResult))
	{
		dwRoleID = GT_INVALID;
		bRtv = FALSE;
	}
	else if (1 == pResult->GetRowCount())
	{
		INT nLen = 0;
		dwAccountID = (*pResult)[0].GetDword();
		bRtv = TRUE;
	}
	else
	{	
		dwAccountID = GT_INVALID;
		bRtv = FALSE;
	}

	// 回收结果
	m_DB.FreeQueryResult(pResult);

	// 归还连接
	m_DB.ReturnConnection(pCon);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBCreateItem(DWORD dwAccountID, DWORD dwRoleID, INT64 n64Serial, DWORD dwItemTypeID, INT nItemNum, BYTE byQuality)
{
	BOOL bRtv = TRUE;

	// 放入百宝袋item_baibao
	Connection* pCon = m_DB.GetFreeConnection();

	m_DB.BeginTransaction(pCon);

	if (!DBWaitCreateItemInBaiBao(dwAccountID, dwRoleID, n64Serial, dwItemTypeID, nItemNum, pCon))
	{
		m_DB.RollBack(pCon);
		m_DB.ReturnConnection(pCon);
		return FALSE;
	}
	// 放入百宝袋equip_baibao
	else if (MIsEquipment(dwItemTypeID) && !DBWaitCreateEquipSpecInBaiBao(n64Serial, dwItemTypeID, byQuality, pCon))
	{
		m_DB.RollBack(pCon);
		m_DB.ReturnConnection(pCon);
		return FALSE;
	}

	m_DB.EndTransaction(pCon);
	m_DB.ReturnConnection(pCon);

	DBInsertBaiBaoLog(dwAccountID, dwRoleID, EBBRT_System, dwItemTypeID, GetCurrentDWORDTime());
	
	return TRUE;
}

// BOOL ServerDB::DBCreateItem(DWORD dwAccountID, DWORD dwRoleID, DWORD dwItemTypeID, INT nItemNum, BYTE byQuality)
// {
// 	// 找到最小的物品序列号
// 	INT64 n64Serial = GT_INVALID;
// 
// 	BOOL bRtv = TRUE;
// 
// 	if (!DBFindValidOuterSerial(n64Serial))
// 	{
// 		bRtv = FALSE;
// 	}
// 	// 放入百宝袋item_baibao
// 	else if (!DBWaitCreateItemInBaiBao(dwAccountID, dwRoleID, n64Serial, dwItemTypeID, nItemNum))
// 	{
// 		bRtv = FALSE;
// 	}
// 	// 放入百宝袋equip_baibao
// 	else if (MIsEquipment(dwItemTypeID) && !DBWaitCreateEquipSpecInBaiBao(n64Serial, dwItemTypeID, byQuality))
// 	{
// 		DBDeleteBySerial(n64Serial, "item_baibao");
// 		bRtv = FALSE;
// 	}
// 
// 	if (bRtv)
// 	{
// 		DBInsertBaiBaoLog(dwAccountID, dwRoleID, EBBRT_System, dwItemTypeID, GetCurrentDWORDTime());
// 	}
// 
// 	return bRtv;
// }

BOOL ServerDB::DBWaitCreateItemInBaiBao( DWORD dwAccountID, DWORD dwRoleID, INT64 n64Serial, DWORD dwItemTypeID, INT nItemNum, Connection* pCon )
{
	BOOL bRtv = FALSE;

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 获取一条连接
// 	Connection* pCon = m_DB.GetFreeConnection();

	// 设置sql语句
	pStream->SetInsert("item_baibao");
	pStream->FillString("SerialNum=")	<< n64Serial;
	pStream->FillString(",TypeID=")		<< dwItemTypeID;
	pStream->FillString(",Num=")		<< nItemNum;
	pStream->FillString(",AccountID=")  << dwAccountID;
	pStream->FillString(",OwnerID=")	<< dwRoleID;

	CHAR szTime[X_DATATIME_LEN+1]= {0};
	DwordTime2DataTime(szTime, X_DATATIME_LEN + 1, GetCurrentDWORDTime());

	pStream->FillString(",CreateTime='").FillString(szTime).FillString("'");

	pStream->FillString(",CreateID=")		<<(DWORD)GT_INVALID;
	pStream->FillString(",CreatorID=")		<<(DWORD)GT_INVALID;

	pStream->FillString(",Bind=")			<<EBS_Unknown;
	pStream->FillString(",CreateMode=")		<<EICM_GMTool;
	pStream->FillString(",ContainerTypeID=")<<EICT_Baibao;
	pStream->FillString(",Suffix=")         <<(BYTE)GT_INVALID;

	// 进行操作
	bRtv = m_DB.WaitExecute(pStream, pCon);

	// 归还连接
//	m_DB.ReturnConnection(pCon);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBFindValidOuterSerial( INT64 &n64SerialNum)
{
	n64SerialNum = GT_INVALID;

	/*select min(ss) from 
	( 
		select min(SerialNum)as ss from  item where  SerialNum>=300000000001 and SerialNum<=400000000000
	union all
		select  min(SerialNum)as ss from item_del where  SerialNum>=300000000001 and SerialNum<=400000000000
	union all
		select min(SerialNum)as ss  from item_baibao where  SerialNum>=300000000001 and SerialNum<=400000000000
	
	) as TT where ss is not null*/

	MyStream* pStream = m_DB.GetStream();
	if (!P_VALID(pStream))
	{
		return FALSE;
	}
	
// 	pStream->FillString("select min(ss) from "
// 						"( select min(SerialNum)as ss from item");
// 	
// 	pStream->FillString(" where SerialNum>=")<<MIN_ITEM_SERIAL_OUTTER;
// 	pStream->FillString(" and SerialNum<=")<<MAX_ITEM_SERIAL_OUTTER;
// 	pStream->FillString(" union all");
// 		
// 	pStream->FillString(" select min(SerialNum)as ss from item_del");
// 
// 	pStream->FillString(" where SerialNum>=")<<MIN_ITEM_SERIAL_OUTTER;
// 	pStream->FillString(" and SerialNum<=")<<MAX_ITEM_SERIAL_OUTTER;
// 	pStream->FillString(" union all");
// 
// 	pStream->FillString(" select min(SerialNum)as ss from item_baibao");
// 
// 	pStream->FillString(" where SerialNum>=")<<MIN_ITEM_SERIAL_OUTTER;
// 	pStream->FillString(" and SerialNum<=")<<MAX_ITEM_SERIAL_OUTTER;
// 
// 	pStream->FillString(" ) as TT where ss is not null");

	pStream->SetSelect("gm_server_data", "data_value");
	pStream->SetWhere();
	pStream->FillString("data_name='MinItemSerial'");

	Beton::QueryResult* pResult = m_DB.Query(pStream);
	m_DB.ReturnStream(pStream);

	// 记录数据库中记录的最小id
	INT64 n64New = -1;
	if (P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		n64New = (*pResult)[0].GetLong();
		m_DB.FreeQueryResult(pResult);
	}
	if (-1 == n64New || 0 == n64New)
	{
		n64New = MAX_ITEM_SERIAL_OUTTER;
	}

	// 从数据库中查找实际使用到的最小id
	INT64 n64RealNew = -1;
	MyStream* pStreamS = m_DB.GetStream();
	if (!P_VALID(pStreamS))
	{
		return FALSE;
	}

	pStreamS->FillString("select min(ss) from "
		"( select min(SerialNum)as ss from item");

	pStreamS->FillString(" where SerialNum>=")<<MIN_ITEM_SERIAL_OUTTER;
	pStreamS->FillString(" and SerialNum<=")<<MAX_ITEM_SERIAL_OUTTER;
	pStreamS->FillString(" union all");

	pStreamS->FillString(" select min(SerialNum)as ss from item_del");

	pStreamS->FillString(" where SerialNum>=")<<MIN_ITEM_SERIAL_OUTTER;
	pStreamS->FillString(" and SerialNum<=")<<MAX_ITEM_SERIAL_OUTTER;
	pStreamS->FillString(" union all");

	pStreamS->FillString(" select min(SerialNum)as ss from item_baibao");

	pStreamS->FillString(" where SerialNum>=")<<MIN_ITEM_SERIAL_OUTTER;
	pStreamS->FillString(" and SerialNum<=")<<MAX_ITEM_SERIAL_OUTTER;

	pStreamS->FillString(" ) as TT where ss is not null");

	Beton::QueryResult* pResultS = m_DB.Query(pStreamS);
	m_DB.ReturnStream(pStreamS);
	if (P_VALID(pResultS) && pResultS->GetRowCount() > 0)
	{
		n64RealNew = (*pResultS)[0].GetLong() - 1;
		m_DB.FreeQueryResult(pResultS);
	}
	if (-1 == n64RealNew)
	{
		n64RealNew = MAX_ITEM_SERIAL_OUTTER;
	}

	n64SerialNum = min(n64New, n64RealNew) <= MAX_ITEM_SERIAL_OUTTER ? min(n64New, n64RealNew) : MAX_ITEM_SERIAL_OUTTER;
	
	MyStream* pStreamT = m_DB.GetStream();
	if (!P_VALID(pStreamT))
	{
		return FALSE;
	}

	pStreamT->SetUpdate("gm_server_data");
	pStreamT->FillString("data_value=") << n64SerialNum;
	pStreamT->SetWhere();
	pStreamT->FillString("data_name='MinItemSerial'");

	if(!m_DB.Execute(pStreamT))
	{
		m_DB.ReturnStream(pStreamT);
		return FALSE;
	}
	m_DB.ReturnStream(pStreamT);
			
	return TRUE;
}

BOOL ServerDB::DBInsertBaiBaoLog(DWORD dwAccountID, DWORD dwRoleID, INT16 n16Type, DWORD dwTypeID, DWORD dwTime )
{
	MyStream* pStream = m_DB.GetStream();

	// 获取连接
	Beton::Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetInsert("log_baibao");
	pStream->FillString("AccountID=") << dwAccountID;
	pStream->FillString(",RoleID=") << dwRoleID;

	// LoongDB设置时间
	CHAR szTime[20];
	if(DwordTime2DataTime(szTime, sizeof(szTime), dwTime))
	{
		pStream->FillString(",LogTime='").FillString(szTime);
		pStream->FillString("'");
	}
	else
	{
		ASSERT(0);
	}

	pStream->FillString(",n16Type=") << n16Type;
	pStream->FillString(",TypeID=") << dwTypeID;
	pStream->FillString(",LeaveWords='Create Item by GMTool'");
	
	// 释放连接
	m_DB.ReturnConnection(pCon);

	m_DB.ReturnStream(pStream);

	return FALSE;
}

BOOL ServerDB::DBWaitCreateEquipSpecInBaiBao( INT64 n64Serial, DWORD dwItemTypeID, BYTE byQuality, Connection* pCon )
{
	BOOL bRtv = FALSE;

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 获取一条连接
//	Connection* pCon = m_DB.GetFreeConnection();

	// 设置sql语句
	pStream->SetInsert("equip_baibao")	;

	pStream->FillString(" SerialNum=")<<n64Serial;
	pStream->FillString(",Quality=")<<(BYTE)byQuality;

	pStream->FillString(",WuHun=")	<<(BYTE)GT_INVALID;
	pStream->FillString(",MinDmg=")	<<(BYTE)GT_INVALID;
	pStream->FillString(",MaxDmg=")	<<(BYTE)GT_INVALID;
	pStream->FillString(",Armor=")	<<(BYTE)GT_INVALID;
	pStream->FillString(",PotVal=")	<<(BYTE)GT_INVALID;

	// 进行操作
	bRtv = m_DB.WaitExecute(pStream, pCon);

	// 归还连接
	//m_DB.ReturnConnection(pCon);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBDeleteBySerial( INT64 n64ItemID, LPCSTR szTable, BOOL bRememberDel)
{
	MyStream* pStream = m_DB.GetStream();
	BOOL bRtv = TRUE;

	// 若删除的是装备，则需要向item_del和equip_del表里添加记录
	if(bRememberDel)
	{
		if( strcmp(szTable, "item") == 0 || strcmp(szTable, "item_baibao") == 0 )
		{
			pStream->SetCopyItem("item_del", "*", szTable);
			pStream->SetWhere();
			pStream->FillString("SerialNum=") << n64ItemID;
		}
		else if( strcmp(szTable, "equip") == 0 || strcmp(szTable, "equip_baibao") == 0 )
		{
			pStream->SetCopyItem("equip_del", "*", szTable);
			pStream->SetWhere();
			pStream->FillString("SerialNum=") << n64ItemID;
		}

		bRtv = m_DB.Execute(pStream);

		// 记录删除时间
		CHAR szTime[X_DATATIME_LEN+1] = {0};
		DwordTime2DataTime(szTime, sizeof(szTime), GetCurrentDWORDTime());

		pStream->Clear();
		pStream->SetUpdate("item_del");
		pStream->FillString("CreateTime='").FillString(szTime).FillString("'");
		pStream->SetWhere();
		pStream->FillString("SerialNum=") << n64ItemID;

		bRtv = bRtv && m_DB.Execute(pStream);
	}

	if(!bRtv) return FALSE;

	pStream->SetDelete(szTable);
	pStream->SetWhere();
	pStream->FillString("SerialNum=")<<n64ItemID;
	bRtv = m_DB.Execute(pStream);

	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBQueryTypeIDBySerial(IN INT64 n64ItemID, OUT DWORD &dwTypeID, IN LPCSTR pTable)
{
	MyStream* pStream = m_DB.GetStream();

	pStream->SetSelect(pTable, "TypeID");

	pStream->SetWhere();
	pStream->FillString("SerialNum=")<<n64ItemID;

	Beton::QueryResult* pResult = m_DB.Query(pStream);

	BOOL bRtv = TRUE;
	if( !P_VALID(pResult) || pResult->GetRowCount() <= 0)
	{
		bRtv = FALSE;
	}
	else
	{
		dwTypeID = (*pResult)[0].GetDword();
	}

	m_DB.FreeQueryResult(pResult);
	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBChangeAccountLoginState(DWORD dwAccountID)
{
	MyStream* pStream = m_DB.GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->SetUpdate("account");
	pStream->FillString("login_status=1");
	pStream->SetWhere();
	pStream->FillString("id=")<< dwAccountID;

	BOOL bRtv = m_DB.Execute(pStream);

	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBIssueTitle(DWORD dwRoleID, DWORD dwTitleID)
{
	MyStream* pStream = m_DB.GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->SetInsert("title");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",ID=") << dwTitleID;
	pStream->FillString(",Count=0");

	BOOL bRtv = m_DB.Execute(pStream);

	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBDelTitle(DWORD dwRoleID, DWORD dwTitleID)
{
	MyStream* pStream = m_DB.GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->SetDelete("title");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and ID=") << dwTitleID;

	BOOL bRtv = m_DB.Execute(pStream);

	m_DB.ReturnStream(pStream);

	return bRtv;
}

//查询角色是否被删除
BOOL ServerDB::DBQueryIsReMovedByRoleID(IN DWORD dwRoleID)
{
	BOOL bRtv = FALSE;
	

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 设置sql语句
	pStream->SetSelect("roledata", "RemoveFlag");
	pStream->SetWhere();
	pStream->FillString("RoleID=")	<< dwRoleID;
	pStream->FillString(" and RemoveFlag=")<<1;

	// 查询
	Beton::QueryResult* pResult = m_DB.Query(pStream);

	// 处理结果
	if (!P_VALID(pResult))
	{			
		bRtv = FALSE;
	}
	else if (1 == pResult->GetRowCount())
	{	
		bRtv = TRUE;
	}
	else
	{		
		bRtv = FALSE;
	}


	// 回收结果
	m_DB.FreeQueryResult(pResult);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}

BOOL ServerDB::DBMinItemSerialDec()
{
	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	pStream->SetUpdate("gm_server_data");
	pStream->FillString("data_value=data_value-1");
	pStream->SetWhere();
	pStream->FillString("data_name='MinItemSerial'");

	BOOL bRet = m_DB.Execute(pStream);
	
	m_DB.ReturnStream(pStream);
	return bRet;
}

DWORD ServerDB::DBQueryGuildExist(IN LPTSTR szGuildName, OUT DWORD& dwGuildID)
{
	dwGuildID = GT_INVALID;
	BOOL bRes = FALSE;
	// 获取流
	Beton::MyStream* pStream = m_DB.GetStream();
	// 获取一条连接
	Connection* pCon = m_DB.GetFreeConnection();

	// 格式化
	pStream->SetSelect("guild", "*");
	pStream->SetWhere();
	pStream->FillString("Name='").FillBlob(szGuildName,(_tcsclen(szGuildName) + 1) * sizeof(TCHAR), pCon);
	pStream->FillString("'");

	// 查询数据库
	Beton::QueryResult* pResult = m_DB.Query(pStream);
	
	 if (P_VALID(pResult) && (1 == pResult->GetRowCount()))
	{
		dwGuildID = (*pResult)[0].GetDword();
		bRes = TRUE;
	}
	
	// 释放流和结果
	m_DB.ReturnStream(pStream);
	m_DB.FreeQueryResult(pResult);
	// 归还连接
	m_DB.ReturnConnection(pCon);
	return bRes;
}


//-------------------------------------------------------------------------------------------------------
// 清空帐号安全码
//-------------------------------------------------------------------------------------------------------
BOOL ServerDB::DBClearAccountSafeCode(DWORD dwAccountID)
{
	BOOL bRtv = FALSE;

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 获取一条连接
	Connection* pCon = m_DB.GetFreeConnection();

	// 设置sql语句
	pStream->SetUpdate("account_common");


	pStream->FillString("SafeCodeCrc=") << 4294967295;
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;

	// 进行操作
	bRtv = m_DB.Execute(pStream);

	// 归还连接
	m_DB.ReturnConnection(pCon);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}

//-------------------------------------------------------------------------------------------------------
// 关闭防沉迷
//-------------------------------------------------------------------------------------------------------
BOOL ServerDB::DBCloseGuard(DWORD dwAccountID)
{
	BOOL bRtv = FALSE;

	// 获取一个Stream
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 设置sql语句
	pStream->SetUpdate("account");
	pStream->FillString("guard=") << 0;
	pStream->SetWhere();
	pStream->FillString("id=") << dwAccountID;

	// 进行操作
	bRtv = m_DB.Execute(pStream);

	// 释放Stream
	m_DB.ReturnStream(pStream);

	return bRtv;
}