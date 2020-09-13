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

#include "../WorldDefine/base_define.h"
#include "../ServerDefine/gm_tool_define.h"

#define PRIVILEGE_TABLE_NAME	""
const INT nPrivilegeCount = 3;

#define IsActionValid(x) ((x)>EAT_Null && (x)<EAT_End)

struct tagLogRecord 
{
	DWORD		dwClientID;
	DWORD		dwTime;
	EActionType	eActionType;
	TCHAR		szName[X_SHORT_NAME];
	TCHAR		szIP[X_SHORT_NAME];
	TCHAR		szDetails[X_LONG_STRING];
};

//-----------------------------------------------------------------------------
// 客户端数据库管理类
//-----------------------------------------------------------------------------
class ClientDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ClientDB();
	~ClientDB() {}

	//-------------------------------------------------------------------------
	// 初始化及销毁
	//-------------------------------------------------------------------------
	BOOL	Init(LPCTSTR strHost, LPCTSTR strUser, LPCTSTR strPassword, LPCTSTR strDatabase, INT nPort);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// 数据库相关处理
	//-------------------------------------------------------------------------
	BOOL	DBQueryClientData(LPCTSTR szName, DWORD dwPsdCrc, DWORD& dwClientID, INT& nPrivilege);
	BOOL	DBCheckIPBind(LPCTSTR szName, DWORD dwClientIP);
	BOOL	DBCreateGameMaster(LPCTSTR szName, DWORD dwPsdCrc, INT nPrivilege);
	BOOL	DBDeleteGameMaster(LPCTSTR szName);
	BOOL	DBChangeGMPrivilege(LPCTSTR szName, INT nNewPrivilege);
	BOOL	DBQueryPrivilegeInfo(TMap<DWORD, DWORD>& mapPrivilege);
	BOOL	DBQueryAccountPrivilege(LPCTSTR szName, INT& nPrivilege);
	BOOL	DBGMExist(LPCTSTR szName);

	BOOL	DBChangeBindIp(LPCTSTR szName, DWORD dwIP);
	BOOL	DBQueryBindIp(LPCTSTR szName, DWORD& dwIP);
	BOOL	DBReplaceBindIp(LPCTSTR szName, DWORD dwIP);

	BOOL	DBLogGMAction(DWORD dwAccountID, LPCTSTR szName, LPCTSTR szIP, EActionType eActionType, LPCTSTR szDetails);

	BOOL	LoadLog(LPVOID& pMsg, EActionType eActionType1, EActionType eActionType2);

private:
	DataBase		m_DB;			// Mysql数据库
	
};