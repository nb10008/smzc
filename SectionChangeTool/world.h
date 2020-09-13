#pragma once
#include "tool_define.h"
#include "client_db.h"

#define TRoleMap TMap<DWORD, tagRole*>

class World
{

public:
	DWORD		m_dwSectionID;
	TCHAR		m_szWorldName[X_SHORT_STRING];
	DWORD		m_dwWorldID;
	
private:
	TCHAR		m_szDBHost[X_LONG_STRING];
	TCHAR		m_szDBName[X_LONG_STRING];
	TCHAR		m_szDBUser[X_LONG_STRING];
	TCHAR		m_szDBPsd[X_LONG_STRING];
	INT			m_nDBPort;

	TRoleMap	m_mapRole;

	TObjRef<Util>					m_pUtil;
public:
	World()		{ m_pUtil = "Util";}
	~World()	{};

	BOOL		Init(DWORD, int);
	//BOOL		InitDB();
	//VOID		CloseDB();

	VOID		AddRoleInfo(DWORD, tagRole*);
	VOID		GetRoleData(DWORD, tagTWrap_Loong_RoleData*);
	VOID		GetRoleEquipBarItemDetail(const tagRole*, BYTE&, tagTWrap_Loong_Item*&);
	VOID		GetRoleEquipBarEquipDetail(INT64, tagTWrap_Loong_Equip*&);
	VOID		GetVisitingCard(const tagRole*, tagTWrap_Loong_VisitingCard*&);
	VOID		GetRoleOldID();

	// ¸÷ÖÖget
	DWORD		GetID()			{ return m_dwWorldID; }
	TRoleMap	GetRoleMap()	{ return m_mapRole; }

public:
	ClientDB*	m_pWorldDB;
};

inline VOID World::AddRoleInfo(DWORD dwIndex, tagRole* pRole)
{
	m_mapRole.Add(dwIndex, pRole);
}
