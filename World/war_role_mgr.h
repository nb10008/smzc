/********************************************************************
	created:	2012/04/26
	created:	26:4:2012   16:22
	filename: 	e:\code\World\war_role_mgr.h
	file path:	e:\code\World
	file base:	war_role_mgr
	file ext:	h
	author:		zhangzhihua
	
	purpose:	战场角色管理
	        1.进入战场的角色都会生成WarRole对象,不到战场结束不进行删除
				方便战场结算
*********************************************************************/

#pragma once
#include "war_role.h"

class Role;
class Unit;

//---------------------------------------------------------------------------------
// 战场角色管理
//---------------------------------------------------------------------------------
class WarRoleMgr
{
public:
	WarRoleMgr(Map* pMap);
	~WarRoleMgr();

	//---------------------------------------------------------------------------------
	// 初始化，更新和销毁
	//---------------------------------------------------------------------------------
	BOOL	Init();
	VOID	Update();
	VOID	Clear();

	//---------------------------------------------------------------------------------
	// Get/Set
	//---------------------------------------------------------------------------------
	WarRole* GetWarRole(DWORD dwRoleID);
	TMap<DWORD, WarRole*> GetWarRoles();

	EWarRoleResult	GetWarResult();
	VOID	SetWarResult(EWarRoleResult eResult);


	//---------------------------------------------------------------------------------
	// 角色进入战场
	// 1.若是角色的话，只针对角色离开战场和进入战场但死亡除外
	//---------------------------------------------------------------------------------
	VOID	OnRoleEnterWar(Role* pRole);
	VOID	OnRoleLeaveWar(Role* pRole);

	//---------------------------------------------------------------------------------
	// 发送聊天消息到战场内的全部玩家
	//---------------------------------------------------------------------------------
	template<typename UnitOperation>
	VOID ForEachRoleInWar( UnitOperation oper );

	//---------------------------------------------------------------------------------
	// 发送聊天消息到战场内的全部友方
	//---------------------------------------------------------------------------------
	template<typename UnitOperation>
	VOID ForEachRoleInSameCamp( UnitOperation oper, ECampType eCampType );

	//---------------------------------------------------------------------------------
	// 发送消息相关
	//---------------------------------------------------------------------------------
	VOID	SendMsgToAll(LPVOID pMsg, DWORD dwSize);

private:
	//---------------------------------------------------------------------------------
	// 战场角色操作
	//---------------------------------------------------------------------------------
	BOOL	AddWarRole(WarRole* pWarRole);
	VOID	DelWarRole(DWORD dwRoleID);

private:
	TMap<DWORD, WarRole*>	m_mapWarRole;				// key:RoleID  凡走过必留下痕迹的检查
	Map*					m_pMap;

	EWarRoleResult			m_eWarSuccess;
};


//---------------------------------------------------------------------------------
// 发送聊天消息到战场内的全部玩家
//---------------------------------------------------------------------------------
template<typename UnitOperation>
VOID WarRoleMgr::ForEachRoleInWar( UnitOperation oper )
{
	m_mapWarRole.ResetIterator();
	WarRole* pWarRole = NULL;
	while( m_mapWarRole.PeekNext(pWarRole) )
	{
		if( P_VALID(pWarRole))
		{
			Role* pRole = g_roleMgr.GetRolePtrByID(pWarRole->GetRoleID());
			if ( P_VALID(pRole))
			{
				oper(pRole);
			}
		}
	}
}

//---------------------------------------------------------------------------------
// 发送聊天消息到战场内的全部友方
//---------------------------------------------------------------------------------
template<typename UnitOperation>
VOID WarRoleMgr::ForEachRoleInSameCamp( UnitOperation oper, ECampType eCampType )
{
	m_mapWarRole.ResetIterator();
	WarRole* pWarRole = NULL;
	while( m_mapWarRole.PeekNext(pWarRole) )
	{
		if( P_VALID(pWarRole))
		{
			Role* pRole = g_roleMgr.GetRolePtrByID(pWarRole->GetRoleID());
			if ( P_VALID(pRole))
			{
				if(pRole->GetCampType() == eCampType)
				{
					oper(pRole);
				}
			}
		}
	}
}







