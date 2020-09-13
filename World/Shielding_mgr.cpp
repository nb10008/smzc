//-------------------------------------------------------------------------------
// Copyright (c) 2010 Dacheng Entertainment All rights reserved.
// filename: shielding_mgr.h
// author: Jason
// actor:
// data: 2010-12-15
// last:
// brief: 屏蔽npc、物品掉落、任务、配方等
//-------------------------------------------------------------------------------

#include "stdafx.h"

#include "shielding_mgr.h"
#include "../ServerDefine/mini_tool.h"
#include "unit.h"
#include "creature.h"
#include "../WorldDefine/itemdefine.h"

static jm_tool::CSCreatedLock __cs;
typedef jm_tool::MutexGuard<jm_tool::CSCreatedLock> TCSGuard;
static ShieldingMgr * __pThis = NULL;

ShieldingMgr * ShieldingMgr::_create()
{
	if( __pThis )
		return __pThis;
	TCSGuard g(__cs);
	if( __pThis )
		return __pThis;
	__pThis = new ShieldingMgr;
	return __pThis;
}
void ShieldingMgr::_destroy(ShieldingMgr *& p)
{
	if( !p )
		return ;
	TCSGuard g(__cs);
	if( p )
		delete p;
	p = NULL;
}
ShieldingMgr::ShieldingMgr()
{
}
ShieldingMgr::~ShieldingMgr()
{
}
VOID ShieldingMgr::Init(VarContainer * pVar,LPCTSTR field)
{
	if( P_VALID(pVar) && field )
	{
		INT nType 			= pVar->GetInt(_T("type"),field);
		DWORD dwTypeID 	= pVar->GetInt(_T("typeid"),field);
		_push_shieldingitem(nType,dwTypeID);
	}
}
ShieldingMgr * ShieldingMgr::Create()
{
	return _create();
}
VOID ShieldingMgr::FinalRelease()
{
	_destroy(__pThis);
}
BOOL ShieldingMgr::IsBlocked(INT nType,DWORD dwQueryID) const
{
	if(!g_world.IsOverseasVersionShieldingOpen())
		return FALSE;
	const TMaskSet & shieldingset = _get_shieldingset(nType);
	if( shieldingset.size() > 0)
	{
		TMaskSet::const_iterator i = shieldingset.find( dwQueryID );
		return i != shieldingset.end();
	}
	return FALSE;
}
const ShieldingMgr::TMaskSet & ShieldingMgr::_get_shieldingset(INT nType) const
{
	static TMaskSet _empty;
	switch( nType )
	{
	case SMBlock_NPC:
		return _NPCShieldingSet;
	case SMBlock_Item:
		return _ItemShieldingSet;
	case SMBlock_Formula:
		return _TaskShieldingSet;
	case SMBlock_Task:
		return _FormulaShieldingSet;
	}
	return _empty;
}
void ShieldingMgr::_push_shieldingitem(INT nType,DWORD dwParam)
{
	switch( nType )
	{
	case SMBlock_NPC:
		_NPCShieldingSet.insert(dwParam);
		break;
	case SMBlock_Item:
		_ItemShieldingSet.insert(dwParam);
		break;
	case SMBlock_Formula:
		_TaskShieldingSet.insert(dwParam);
		break;
	case SMBlock_Task:
		_FormulaShieldingSet.insert(dwParam);
		break;
	}
}

BOOL IsShieldingNPC( Unit * pUnit )
{
	BOOL bNotVisbale = FALSE;
	if( P_VALID(pUnit) )
	{
		if( pUnit->IsCreature() )
		{
			Creature * pCreature = (Creature*)pUnit;
			if( P_VALID(pCreature->GetProto()) )
			{
				ShieldMgrCallWithRetValue(IsBlocked,(ShieldingMgr::SMBlock_NPC,pCreature->GetProto()->dwTypeID),bNotVisbale);
			}
		}
	}
	return bNotVisbale;
}
BOOL IsShieldingItem( DWORD dwTypeID )
{
	BOOL bNotVisbale = FALSE;
	if( P_VALID(dwTypeID) )
	{
		ShieldMgrCallWithRetValue(IsBlocked,(ShieldingMgr::SMBlock_Item,dwTypeID),bNotVisbale);
	}
	return bNotVisbale;
}
BOOL IsShieldingTask(DWORD dwTaskID)
{
	BOOL bNotVisbale = FALSE;
	if( P_VALID(dwTaskID) )
	{
		ShieldMgrCallWithRetValue(IsBlocked,(ShieldingMgr::SMBlock_Task,dwTaskID),bNotVisbale);
	}
	return bNotVisbale;
}
BOOL IsShieldingFormula(DWORD dwFormulaID)
{
	BOOL bNotVisbale = FALSE;
	if( P_VALID(dwFormulaID) )
	{
		ShieldMgrCallWithRetValue(IsBlocked,(ShieldingMgr::SMBlock_Formula,dwFormulaID),bNotVisbale);
	}
	return bNotVisbale;
}


