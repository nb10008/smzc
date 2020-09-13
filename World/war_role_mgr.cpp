/********************************************************************
	created:	2012/04/26
	created:	26:4:2012   16:25
	filename: 	e:\code\World\war_role_mgr.cpp
	file path:	e:\code\World
	file base:	war_role_mgr
	file ext:	cpp
	author:		zhangzhihua
	
	purpose:	战场角色管理
*********************************************************************/
#include "StdAfx.h"
#include "war_role_mgr.h"
#include "role.h"
#include "map.h"
#include "creature.h"
#include "map_creator.h"

//-------------------------------------------------------------------------------------------------------
//  构造、析构
//-------------------------------------------------------------------------------------------------------
WarRoleMgr::WarRoleMgr(Map* pMap)
				:m_pMap(pMap)
{
	m_mapWarRole.Clear();
	m_eWarSuccess = EWRR_NULL;
}

WarRoleMgr::~WarRoleMgr()
{
	WarRole* pWarRole = NULL;
	TMap<DWORD, WarRole*>::TMapIterator	itDel = m_mapWarRole.Begin();
	while ( m_mapWarRole.PeekNext(itDel, pWarRole))
	{
		SAFE_DEL(pWarRole);
	}

	m_mapWarRole.Clear();
}

//-------------------------------------------------------------------------------------------------------
//  初始化
//-------------------------------------------------------------------------------------------------------
BOOL WarRoleMgr::Init()
{
	m_mapWarRole.Clear();
	m_eWarSuccess = EWRR_NULL;
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
//  更新
//-------------------------------------------------------------------------------------------------------
VOID WarRoleMgr::Update()
{
	TMap<DWORD, WarRole*>::TMapIterator it = m_mapWarRole.Begin();
	WarRole* pWarRole = NULL;
	while (m_mapWarRole.PeekNext(it, pWarRole))
	{
		pWarRole->Update();
	}
}

//-------------------------------------------------------------------------------------------------------
//  清空
//-------------------------------------------------------------------------------------------------------
VOID WarRoleMgr::Clear()
{
	TMap<DWORD, WarRole*>::TMapIterator it = m_mapWarRole.Begin();
	WarRole* pWarRole = NULL;
	while ( m_mapWarRole.PeekNext(it, pWarRole))
	{
		SAFE_DEL(pWarRole);
	}
	m_mapWarRole.Clear();
}

//-------------------------------------------------------------------------------------------------------
// 增加战斗角色
//-------------------------------------------------------------------------------------------------------
BOOL WarRoleMgr::AddWarRole(WarRole* pWarRole)
{
	if ( !P_VALID(pWarRole))
		return FALSE;

	if ( !P_VALID(pWarRole->GetRoleID()))
		return FALSE;

	m_mapWarRole.Add(pWarRole->GetRoleID(), pWarRole);

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// 删除战斗角色
//-------------------------------------------------------------------------------------------------------
VOID WarRoleMgr::DelWarRole(DWORD dwRoleID)
{
	if ( !P_VALID(dwRoleID))
		return ;

	WarRole* pWarRole = GetWarRole(dwRoleID);
	if ( P_VALID(pWarRole))
	{
		SAFE_DEL(pWarRole);
		m_mapWarRole.Erase(dwRoleID);
	}
}

//-------------------------------------------------------------------------------------------------------
// 得到战斗角色
//-------------------------------------------------------------------------------------------------------
WarRole* WarRoleMgr::GetWarRole(DWORD dwRoleID)
{
	if ( !P_VALID(dwRoleID))
		return NULL;

	return m_mapWarRole.Peek(dwRoleID);
}

//-------------------------------------------------------------------------------------------------------
// 得到在战场中的所有角色
//-------------------------------------------------------------------------------------------------------
TMap<DWORD, WarRole*> WarRoleMgr::GetWarRoles()
{
	return m_mapWarRole;
}

EWarRoleResult WarRoleMgr::GetWarResult()
{
	return m_eWarSuccess;
}

VOID WarRoleMgr::SetWarResult(EWarRoleResult eResult)
{
	if (eResult < 0 || eResult >= EWRR_End)
	{
		eResult = EWRR_NULL;
	}
	m_eWarSuccess = eResult;
}

//-------------------------------------------------------------------------------------------------------
// 角色进入战场
//-------------------------------------------------------------------------------------------------------
VOID WarRoleMgr::OnRoleEnterWar(Role* pRole)
{
	if ( !P_VALID(pRole) || !P_VALID(pRole->GetSession()) || !P_VALID(pRole->GetMap()))
		return;

	WarRole* pWarRole = NULL;
	pWarRole = GetWarRole(pRole->GetID());
	if ( !P_VALID(pWarRole))
	{
		pWarRole = WarRole::Create(pRole->GetSession()->GetSessionID(), pRole->GetID());
		if (!P_VALID(pWarRole))
		{
			return;
		}
		pWarRole->Init();

		AddWarRole(pWarRole);
	}

	pWarRole->OnEnterWar(pRole);
	pRole->SetWarRole(pWarRole);
}

//-------------------------------------------------------------------------------------------------------
// 角色离开战场
//-------------------------------------------------------------------------------------------------------
VOID WarRoleMgr::OnRoleLeaveWar(Role* pRole)
{
	if ( !P_VALID(pRole) )
		return;

	WarRole* pWarRole = GetWarRole(pRole->GetID());
	if ( P_VALID(pWarRole))
	{
		// 清除战场角色的属性
		pWarRole->OnLeaveWar(pRole);
		DelWarRole(pRole->GetID());
		pRole->SetWarRole(NULL);
	}

}

//-------------------------------------------------------------------------------------------------------
// 发送消息相关
//-------------------------------------------------------------------------------------------------------
VOID	WarRoleMgr::SendMsgToAll(LPVOID pMsg, DWORD dwSize)
{
	m_mapWarRole.ResetIterator();
	DWORD dwRoleID = 0;
	WarRole* pWarRole = NULL;
	while( m_mapWarRole.PeekNext(dwRoleID, pWarRole) )
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if ( P_VALID(pRole))
		{
			pRole->SendMessage(pMsg, dwSize);
		}
	}
}


