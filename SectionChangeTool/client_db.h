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

struct tagRole
{
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szAccoutName[X_SHORT_STRING];
	TCHAR	szRoleName[X_SHORT_STRING];
	DWORD	dwOldID;							// 在原服上的RoleID
	DWORD	dwNewID;							// 在新服上的RoleID

	tagRole()
	{
		ZeroMemory(this, sizeof(*this));
	}
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
	ClientDB(){}
	~ClientDB(){}

	//-------------------------------------------------------------------------
	// 初始化及销毁
	//-------------------------------------------------------------------------
	BOOL		Init(LPCTSTR strHost, LPCTSTR strUser, LPCTSTR strPassword, LPCTSTR strDatabase, INT nPort);
	VOID		Destroy();

	//DataBase	GetDB() { return m_DB; }
	DWORD		GetRoleOldID(tagRole*);

public:
	DataBase		m_DB;		// Mysql数据库
	TObjRef<Util>	m_pUtil;	// Util
};