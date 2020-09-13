//-----------------------------------------------------------------------------
//!\file	client_db.h
//!\brief	客户端数据库管理类
//!
//!\date	2009-04-20
//! last	2009-04-20
//!\author	zhangrong
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// 服务器数据库管理类
//-----------------------------------------------------------------------------
class ServerDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ServerDB()	{}
	~ServerDB() {}

	//-------------------------------------------------------------------------
	// 初始化及销毁
	//-------------------------------------------------------------------------
	BOOL	Init(LPCTSTR strHost, LPCTSTR strUser, LPCTSTR strPassword, LPCTSTR strDatabase, INT nPort);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// 数据库相关处理
	//-------------------------------------------------------------------------
	BOOL	DBResumeRole(DWORD dwRoleID, LPCTSTR tszNewName);
	BOOL	DBRoleModify(LPCTSTR tszName, INT nLel, INT nSkill, INT nYuanbao, INT64 n64Exp, INT64 n64Money, INT nAttPoint);
	BOOL	DBCreateItem(DWORD dwAccountID, DWORD dwRoleID, INT64 n64Serial, DWORD dwItemTypeID, INT nItemNum, BYTE byQuality);
	DWORD	DBQueryItemExist(LPCSTR szTableName, INT64 n64SerialNum);
	DWORD	DBDeleteItem(DWORD dwRoleID, INT64 n64ItemID);
	DWORD	DBResumeItem(INT64 n64SerialNum, DWORD dwTargetID, DWORD dwAccountID);
	BOOL	DBChangeAccountLoginState(DWORD dwAccountID);

	BOOL	DBQueryNameByRoleID(IN DWORD dwRoleID, OUT LPTSTR szName);
	BOOL	DBQueryIsReMovedByRoleID(IN DWORD dwRoleID);//查询角色是否被删除
	BOOL	DBQueryRoleIDByName(IN LPTSTR szName, OUT DWORD &dwRoleID);
	BOOL	DBQueryAccountIDByName(IN LPTSTR, OUT DWORD&);
	BOOL	DBQueryRoleIDByAccountID(IN DWORD dwAccountID, OUT DWORD &dwRoleID);
	BOOL	DBQueryAccountIDByRoleID(IN DWORD dwRoleID, OUT DWORD &dwAccountID);
	BOOL	DBRoleNameExist(LPCTSTR szName);

	BOOL	DBIssueTitle(DWORD dwRoleID, DWORD dwTitleID);
	BOOL	DBDelTitle(DWORD dwRoleID, DWORD dwTitleID);

	BOOL	DBFindValidOuterSerial(INT64 &n64SerialNum);
	BOOL	DBMinItemSerialDec();

	DWORD	DBQueryGuildExist(IN LPTSTR szGuildName, OUT DWORD& dwGuildID);

	BOOL	DBClearAccountSafeCode(DWORD dwAccountID);
	BOOL	DBQueryAccountCommonIDByName(IN LPTSTR szAccountName, OUT DWORD& dwAccountID);
	BOOL	DBCloseGuard(DWORD dwAccountID);
private:
	BOOL	DBWaitCreateItemInBaiBao(DWORD dwAccountID, DWORD dwRoleID, INT64 n64Serial, DWORD dwItemTypeID, INT nItemNum, Connection* pCon);
	BOOL	DBWaitCreateEquipSpecInBaiBao(INT64 n64Serial, DWORD dwItemTypeID, BYTE byQuality, Connection* pCon);
	BOOL	DBInsertBaiBaoLog(DWORD dwAccountID, DWORD dwRoleID, INT16 n16Type, DWORD dwTypeID, DWORD dwTime );
	BOOL	DBQueryTypeIDBySerial(IN INT64 n64ItemID, OUT DWORD &dwTypeID, IN LPCSTR pTable);
	BOOL	DBDeleteBySerial(INT64 n64ItemID, LPCSTR szTable, BOOL bRememberDel);
private:
	DataBase		m_DB;			// Mysql数据库
	TObjRef<Util>	m_pUtil;		// Util
};
