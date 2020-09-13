//-----------------------------------------------------------------------------
//!\file	
//!\brief	
//!
//!\date	
//! last	
//!\author	
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "client_db.h"
#include "tool_define.h"
#include "RoleInfoReader.h"
#include "section.h"

#define TSectionMap		TMap<DWORD,	Section*>

enum Eorder
{
	EO_XMLSelected,		//xml文件已选中
	EO_Merge,			//开始合服
};

class CTool
{
public:
	BOOL	Init();
	BOOL	Control( INT nOrderID, INT nParaNum, ...);				//主逻辑
	BOOL	DoMerge();
	
	VOID	LoadRoleFromXml(LPCTSTR);

	BOOL	SaveAccount();
	BOOL	SaveAccountDetail(const tagRole*, DWORD);
	BOOL	SaveRoleData();
	BOOL	SaveRoleDataDetail(const tagRole*, DWORD, DWORD);
	BOOL	SaveEquip(const tagRole*);
	BOOL	SaveSkill(const tagRole*);
	BOOL	SaveTitle(const tagRole*);
	BOOL	SaveVisitingCard(const tagRole*);
	//BOOL	SaveSkill(tagRole*) {return TRUE;}
	BOOL	SaveAccountCommon();
	//BOOL	SaveEquipDetail(tagRole*, BYTE, tagTWrap_Loong_Item*);

	BOOL	CheckRoleName(tagRole*&, DWORD, DWORD&);
	DWORD	GetRoleAccountID(const tagRole*);

public:
	ClientDB*						m_pdb1;					// 目的数据库（即新生成的数据库）的LoginDB
	ClientDB*						m_pdb2;					// 目的数据库的LoongDB

private:
	TSFPTrunk<CTool>				m_Trunk;
	TObjRef<VarContainer>			m_pVar;
	TObjRef<Log>					m_pLog;
	TObjRef<Util>					m_pUtil;
	CRoleInfoReader					m_RoleInfoReader;
	volatile INT64					m_n64ItemIDGen;			// 物品ID生成器，使用前加1

	TSectionMap						m_mapSection;
	INT								m_nSectionNum;
};

extern CTool g_Tool;