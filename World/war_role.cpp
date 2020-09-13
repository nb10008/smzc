/********************************************************************
	created:	2012/04/26
	created:	26:4:2012   16:31
	filename: 	e:\code\World\war_role.cpp
	file path:	e:\code\World
	file base:	war_role
	file ext:	cpp
	author:		zhangzhihua
	
	purpose:	战场角色
*********************************************************************/
#include "StdAfx.h"
#include "war_role.h"
#include "war_role_mgr.h"
#include "role_mgr.h"
#include "role.h"
#include "unit.h"
#include "creature.h"
#include "pet_pocket.h"

WarRole::WarRole(DWORD dwAccountID, DWORD dwRoleID)
			:m_dwAccountID(dwAccountID)
			,m_dwRoleID(dwRoleID)
			,m_dwNeutralMonsterKillCount(0)
			,m_dwMonsterKillCount(0)
			,m_dwBossKillCount(0)
			,m_dwRoleKillCount(0)
			,m_dwRoleDeadCount(0)
			,m_nGolds(0)
			,m_dwRareBossKillCount(0)
{

}

WarRole::~WarRole()
{

}

WarRole* WarRole::Create(DWORD dwAccountID, DWORD dwRoleID)
{
	return new WarRole(dwAccountID, dwRoleID);
}

BOOL WarRole::Init()
{
	m_dwNeutralMonsterKillCount = 0;	// 击杀中立怪数量
	m_dwMonsterKillCount = 0;			// 击杀怪物计数
	m_dwBossKillCount = 0;			// 击杀boss计数
	m_dwRoleKillCount = 0;			// 击杀角色计数
	m_dwRoleDeadCount = 0;			// 角色死亡计数
	m_nGolds = 0;
	m_dwRareBossKillCount = 0;
	return TRUE;
}

VOID WarRole::Update()
{

}

VOID WarRole::Destroy()
{

}

DWORD WarRole::GetAccountID()
{
	return m_dwAccountID;
}

DWORD WarRole::GetRoleID()
{
	return m_dwRoleID;
}

DWORD WarRole::GetRewardItemID(BYTE byIndex)
{
	if (byIndex >= 0 && byIndex < WAR_REWARD_ITEM_NUM)
	{
		return m_dwRewardItemID[byIndex];
	}

	return GT_INVALID;
}

VOID WarRole::SetRewardItemID(DWORD dwItemID1, DWORD dwItemID2, DWORD dwItemID3)
{
	DWORD dwIdx = 0;
	m_dwRewardItemID[dwIdx++] = dwItemID1;
	m_dwRewardItemID[dwIdx++] = dwItemID2;
	m_dwRewardItemID[dwIdx++] = dwItemID3;
}

VOID WarRole::OnDead(Unit* pSelf, Unit* pSrc)
{
	if (!P_VALID(pSelf) || !P_VALID(pSrc))
	{
		return;
	}

	// 死亡数
	if (pSelf->IsRole())
	{
		// 自己是玩家
		Role* pRole = (Role*)pSelf;
		++m_dwRoleDeadCount;
		pRole->SyncRoleAttr(ERoleAttrRoleKillCount, EMR_Map);
	}
}

VOID WarRole::OnKill(Unit* pSelf, Unit* pSrc)
{
	if (!P_VALID(pSelf) || !P_VALID(pSrc))
	{
		return;
	}

	if (pSelf->IsRole())
	{
		// 自己是玩家
		Role* pRole = (Role*)pSelf;

		if (pSrc->IsCreature())
		{
			// 杀的是怪物
			Creature* pCreature = (Creature*)pSrc;
			if (pCreature->IsShouLingBoss())
			{
				// 杀boss数
				++m_dwBossKillCount;
				pRole->SyncRoleAttr(ERoleAttrBossKillCount, EMR_Map);

			}
			else if(pCreature->IsBoss())
			{
				//杀稀有boss数
				++m_dwRareBossKillCount;
				pRole->SyncRoleAttr(ERoleAttrRareBossKillCount, EMR_Map);
			}
			else
			{
				// 杀怪数
				++m_dwMonsterKillCount;
				pRole->SyncRoleAttr(ERoleAttrMonsterKillCount, EMR_Map);
			}

			if (ECamp_Neutrality == pCreature->GetCampType())
			{
				// 击杀中立怪数量
				++m_dwNeutralMonsterKillCount;
				//pRole->SyncRoleAttr(ERoleAttrNeutralMonsterKillCount, EMR_Map);
			}
		}
		else if (pSrc->IsRole())
		{
			// 杀的是玩家
			// 击杀角色计数
			++m_dwRoleKillCount;
			pRole->SyncRoleAttr(ERoleAttrRoleKillCount, EMR_Map);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
//  金币点
//-------------------------------------------------------------------------------------------------------
VOID WarRole::AddGlod(INT nGlod, BOOL bSendMsg/* = TRUE*/)
{
	if ( 0 == nGlod)
		return ;

	m_nGolds += nGlod;
	if ( m_nGolds < 0)	m_nGolds = 0;

	Role* pRole = g_roleMgr.GetRolePtrByID(GetRoleID());
	if (!P_VALID(pRole))
	{
		return;
	}

	pRole->SyncRoleAttr(ERoleAttrWarGold, EMR_Map);
}

DWORD WarRole::GetNeutralMonsterKillCount()
{
	return m_dwNeutralMonsterKillCount;
}

DWORD WarRole::GetRareBossKillCount()
{
	return m_dwRareBossKillCount;
}

DWORD WarRole::GetMonsterKillCount()
{
	return m_dwMonsterKillCount;
}

DWORD WarRole::GetBossKillCount()
{
	return m_dwBossKillCount;
}

DWORD WarRole::GetRoleKillCount()
{
	return m_dwRoleKillCount;
}

DWORD WarRole::GetRoleDeadCount()
{
	return m_dwRoleDeadCount;
}

//-------------------------------------------------------------------------------------------------------
// 进入战场
//-------------------------------------------------------------------------------------------------------
VOID WarRole::OnEnterWar(Role* pRole)
{
	if ( !P_VALID(pRole))
		return;

	//把宠物收回
	PetPocket* pPocket = pRole->GetPetPocket();
	if(P_VALID(pPocket))
		pPocket->RestAPet();

	// 清理背包
	ItemMgr			&itemMgr	= pRole->GetItemMgr();

	tagItem *pItem = NULL;
	INT16 n16SzWarBag = itemMgr.GetWarBag().GetCurSpaceSize();
	for(INT16 i=0; i<n16SzWarBag; ++i)
	{
		if(itemMgr.GetWarBagFreeSize() == n16SzWarBag)
		{
			break;
		}

		pItem = itemMgr.GetWarBag().GetItem(i);
		if(!P_VALID(pItem))
		{
			continue;
		}

		itemMgr.DelFromWarBag(pItem->n64Serial, ELCLD_LEAVEWAR_DEL);
	}
	pRole->RecalEquipEffectAttForWar(true);

}

//---------------------------------------------------------------------------------
// 清理所有的角色战场信息
//---------------------------------------------------------------------------------
VOID WarRole::OnLeaveWar(Role* pRole)
{
	if ( !P_VALID(pRole))
		return;

	// 离开战场时清理背包
	ItemMgr			&itemMgr	= pRole->GetItemMgr();

	tagItem *pItem = NULL;
	INT16 n16SzWarBag = itemMgr.GetWarBag().GetCurSpaceSize();
	for(INT16 i=0; i<n16SzWarBag; ++i)
	{
		if(itemMgr.GetWarBagFreeSize() == n16SzWarBag)
		{
			break;
		}

		pItem = itemMgr.GetWarBag().GetItem(i);
		if(!P_VALID(pItem))
		{
			continue;
		}

		itemMgr.DelFromWarBag(pItem->n64Serial, ELCLD_LEAVEWAR_DEL);
	}

	pRole->RecalEquipEffectAttForWar(false);
}




