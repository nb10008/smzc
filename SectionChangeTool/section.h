#pragma once

#include "../ServerDefine/login_define.h"
#include "tool_define.h"
#include "client_db.h"
#include "world.h"

#define TWorldMap		TMap<DWORD, World*>

class Section
{
public:

	Section(DWORD, LPCTSTR);
	~Section()	{}
	
	DWORD		GetID()	{ return m_dwSectionID; }
	BOOL		Init();
	
	VOID		GetAccountDetail(LPCTSTR, CHAR*, BYTE&, EPlayerLoginStatus&, DWORD&, DWORD&, BOOL&, CHAR*);

	// ¸÷ÖÖGetº¯Êý
	TWorldMap	GetWorldMap()	{ return m_mapWorld; }

public:
	ClientDB*	m_pLoginDB;

private:

	DWORD		m_dwSectionID;
	TCHAR		m_szSectionName[X_SHORT_STRING];

	TCHAR		m_szDBHost[X_LONG_STRING];
	TCHAR		m_szDBName[X_LONG_STRING];
	TCHAR		m_szDBUser[X_LONG_STRING];
	TCHAR		m_szDBPsd[X_LONG_STRING];
	INT			m_nDBPort;
	
	TWorldMap	m_mapWorld;
	INT			m_nWorldNum;
	TObjRef<Util>					m_pUtil;
};
