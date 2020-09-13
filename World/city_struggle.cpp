//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: city_struggle.cpp
// author: twan
// actor:
// data: 2009-9-10
// last:
// brief: 城战
//-------------------------------------------------------------------------------

#include "StdAfx.h"
#include "city_struggle.h"
#include "Role.h"
#include "role_mgr.h"
#include "map.h"
#include "map_creator.h"
#include "city.h"
#include "city_mgr.h"
#include "guild_mgr.h"
#include "../WorldDefine/guild_define.h"

CityStruggle::CityStruggle()
{
}

CityStruggle::~CityStruggle()
{
	Destroy();
}

//--------------------------------------------------------------------
// 初始化
//--------------------------------------------------------------------
VOID CityStruggle::Init(City* pCity)
{
	m_eState				=	ECSGS_UnStruggle;
	m_nUpdateTick			=	0;
	m_nAtkReviveTick		=	25 * TICK_PER_SECOND;
	m_nDefReviveTick		=	25 * TICK_PER_SECOND;
	m_nEudemonTick			=	0;
	m_pCity					=	pCity;
	m_bBeginSummonCountDown	=	FALSE;
	m_eResult				=	ECSRT_None;
}

//--------------------------------------------------------------------
// 设置城战状态
//--------------------------------------------------------------------
VOID CityStruggle::SetCityStruggleState(ECityStruggleState eState)
{
	// 判断合法性
	if (eState < ECSGS_Start || eState > ECSGS_End)
	{
		return;
	}

	m_eState = eState;
}

//--------------------------------------------------------------------
// 销毁
//--------------------------------------------------------------------
VOID CityStruggle::Destroy()
{
	tagStruggleRevive* pRevive = NULL;
	m_mapRevive.ResetIterator();
	while (m_mapRevive.PeekNext(pRevive))
	{
		SAFE_DEL(pRevive);
	}
	m_mapRevive.Clear();
}

//--------------------------------------------------------------------
// 城战更新
//--------------------------------------------------------------------
VOID CityStruggle::Update()
{
	if (m_eState != ECSGS_Struggle)		return;

	++m_nUpdateTick;

	UpdateRoleRevive();
	UpdateBall();
	UpdateColumns();
	UpdateEudemon();
}

//--------------------------------------------------------------------
// 更新城战玩家复活
//--------------------------------------------------------------------
VOID CityStruggle::UpdateRoleRevive()
{
	// 复活阵亡的玩家
	DWORD dwRoleID = 0;
	vector<DWORD> vecNeedDel;
	tagStruggleRevive* pRevive = NULL;
	TMap<DWORD, tagStruggleRevive*>::TMapIterator it = m_mapRevive.Begin();
	while(m_mapRevive.PeekNext(it,dwRoleID, pRevive))
	{
		if (pRevive->eForceType == ECFT_Attack)
		{
			// 进攻联盟
			if (m_nUpdateTick - pRevive->nDeadTick >= m_nAtkReviveTick)
			{
				// 复活玩家
				ReviveRole(dwRoleID);
				vecNeedDel.push_back(dwRoleID);
			}
		}
		else
		{
			// 防御联盟
			if (m_nUpdateTick - pRevive->nDeadTick >= m_nDefReviveTick)
			{
				// 复活玩家
				ReviveRole(dwRoleID);
				vecNeedDel.push_back(dwRoleID);
			}
		}
	}  

	// 从待复活队列中删除已复活玩家
	for (vector<DWORD>::iterator it = vecNeedDel.begin(); it != vecNeedDel.end(); ++it)
	{
		tagStruggleRevive* pDel = m_mapRevive.Peek(*it);
		SAFE_DEL(pDel);
		m_mapRevive.Erase(*it);
	}
}

//--------------------------------------------------------------------
// 更新龙珠
//--------------------------------------------------------------------
VOID CityStruggle::UpdateBall()
{

}

//--------------------------------------------------------------------
// 更新阴阳柱
//--------------------------------------------------------------------
VOID CityStruggle::UpdateColumns()
{

}

//--------------------------------------------------------------------
// 更新守护神
//--------------------------------------------------------------------
VOID CityStruggle::UpdateEudemon()
{
	// 没有开始召唤
	if (!m_bBeginSummonCountDown)
		return;

	if (--m_nEudemonTick < 0)
	{
		// 成功召唤，调用脚本
		const ActScript* pScript = g_ScriptMgr.GetActScript(CITY_STRUGGLE_ACT_ID);
		if (P_VALID(pScript))
		{
			pScript->FinishEudemonSummon();
		}
		m_bBeginSummonCountDown = FALSE;
	}
}

//--------------------------------------------------------------------
// 处理玩家加入复活队列
//--------------------------------------------------------------------
VOID CityStruggle::AddToReviveMap(Role* pRole)
{
	if (m_eState != ECSGS_Struggle)		
		return;

	ECityForceType eForce;
	// 从进攻联盟中查找玩家
	if (TRUE == IsRoleInForce(pRole,ECFT_Attack))
	{
		eForce = ECFT_Attack;
	}

	// 从防御联盟中查找玩家
	else if (TRUE == IsRoleInForce(pRole,ECFT_Defence))
	{
		eForce = ECFT_Defence;
	}

	else
	{
		// 没找到
		return;
	}

	tagStruggleRevive* pRevive = new tagStruggleRevive;
	pRevive->eForceType = eForce;
	pRevive->nDeadTick = m_nUpdateTick;

	tagStruggleRevive* pRet = m_mapRevive.Peek(pRole->GetID());
	if (!P_VALID(pRet))
	{
		// 查找的角色没在复活队列中才添加
		m_mutexRevive.Acquire();
		m_mapRevive.Add(pRole->GetID(), pRevive);
		m_mutexRevive.Release();

		// 向客户端发送复活所需时间
		tagNS_CityStruggleRevive send;
		if (eForce == ECFT_Attack)
		{
			send.nSec = m_nAtkReviveTick / TICK_PER_SECOND;
		}
		else
		{
			send.nSec = m_nDefReviveTick / TICK_PER_SECOND;
		}
		pRole->SendMessage(&send, send.dwSize);
	}
	else
		delete pRevive;
}

//--------------------------------------------------------------------
// 玩家是否在阵营中
//--------------------------------------------------------------------
BOOL CityStruggle::IsRoleInForce(Role* pRole, ECityForceType eForce)
{
	ASSERT(P_VALID(m_pCity));

	// 获取参与帮派
	INT nGuildNum = MAX_CITY_ATTACK_FORCE;
	DWORD dwGuildID[MAX_CITY_ATTACK_FORCE];
	memset(dwGuildID, 0xFF, sizeof(dwGuildID));

	if (!m_pCity->GetConfirmGuild(eForce, nGuildNum, dwGuildID))
		return FALSE;

	if (nGuildNum <= 0)
		return FALSE;

	// 从帮派里找
	for (INT i = 0; i != nGuildNum; ++i)
	{
		Guild* pGuild = g_guildMgr.GetGuild(dwGuildID[i]);
		if (!P_VALID(pGuild))
			return FALSE;

		tagGuildMember* pGuildMem = pGuild->GetMember(pRole->GetID());
		if (P_VALID(pGuildMem))		// 找到
		{
			return TRUE;
		}

	}
	return FALSE;
}

//--------------------------------------------------------------------
// 获得玩家阵营
//--------------------------------------------------------------------
BOOL CityStruggle::GetForceType(Role* pRole, ECityForceType& eForce)
{
	if (TRUE == IsRoleInForce(pRole, ECFT_Attack))
	{
		eForce = ECFT_Attack;
		return TRUE;
	}

	if (TRUE == IsRoleInForce(pRole, ECFT_Defence))
	{
		eForce = ECFT_Defence;
		return TRUE;
	}

	return FALSE;
}

//--------------------------------------------------------------------
// 复活玩家
//--------------------------------------------------------------------
VOID CityStruggle::ReviveRole(DWORD dwRoleID)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))		
		return;

	tagNS_RoleRevive send;
	send.dwRoleID		= dwRoleID;
	//send.dwErrorCode	= pRole->Revive(ERRT_CityStruggle, GT_INVALID);
	if( !pRole->IsInState(ES_Dead) )
		send.dwErrorCode = E_Revive_NotDead;

	const tagMapInfo *pMapInfo = g_mapCreator.GetMapInfo(pRole->GetRebornMapID());
	if(!P_VALID(pMapInfo))
	{
		ASSERT(P_VALID(pMapInfo));
		send.dwErrorCode = E_Revive_MapNotFound;
	}

	
	// 设置复活点及血量和真气量
	pRole->GetRevive().nHP		=	pRole->GetAttValue(ERA_MaxHP);
	pRole->GetRevive().nMP		=	pRole->GetAttValue(ERA_MaxMP);
	pRole->GetRevive().fX		=	pMapInfo->vRebornPos.x;
	pRole->GetRevive().fY		=	pMapInfo->vRebornPos.y;
	pRole->GetRevive().fZ		=	pMapInfo->vRebornPos.z;
	pRole->GetRevive().dwMapID	=	pRole->GetRebornMapID();

	pRole->GetMap()->OnRevive(pRole, ERRT_CityStruggle, pRole->GetRevive().nHP, pRole->GetRevive().nMP, 
					pRole->GetRevive().fX, pRole->GetRevive().fY, pRole->GetRevive().fZ, pRole->GetRevive().dwMapID);

	pRole->SetAttValue(ERA_HP, pRole->GetRevive().nHP);
	pRole->SetAttValue(ERA_MP, pRole->GetRevive().nMP);
	pRole->UnsetState(ES_Dead);

	// 清空复活相关记录信息
	pRole->GetRevive().nHP = 0; 
	pRole->GetRevive().nMP = 0;
	send.dwErrorCode = E_Success;

	if( P_VALID(pRole->GetMap()) )
	{
		pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
	}

	if(E_Success == send.dwErrorCode)
	{
		// 复活到指定地点
		pRole->GotoNewMap(pRole->GetRevive().dwMapID, 
			pRole->GetRevive().fX, pRole->GetRevive().fY, pRole->GetRevive().fZ);
	}

}

//--------------------------------------------------------------------
// 设置联盟复活时间
//--------------------------------------------------------------------
VOID CityStruggle::SetReviveTime(ECityForceType eForce, INT nSec)
{
	if (eForce < ECFT_Attack || eForce > ECFT_Defence)
		return;

	if (eForce == ECFT_Attack)
		m_nAtkReviveTick = nSec * TICK_PER_SECOND;
	else
		m_nDefReviveTick = nSec * TICK_PER_SECOND;
}

//--------------------------------------------------------------------
// 设置守护神召唤时间
//--------------------------------------------------------------------
VOID CityStruggle::SetSummonTime(INT nSec)
{
	m_bBeginSummonCountDown = TRUE;
	m_nEudemonTick = nSec * TICK_PER_SECOND;
}

VOID CityStruggle::StopSummon()
{
	m_bBeginSummonCountDown = FALSE;
}

//--------------------------------------------------------------------
// 设置城战结果
//--------------------------------------------------------------------
VOID CityStruggle::SetResult(ECityStruggleResult eResult)
{
	m_eResult = eResult;
}

//--------------------------------------------------------------------
// 初始化城战玩家数据
//--------------------------------------------------------------------
VOID CityStruggle::InitCSRoleData()
{
	m_mapRoleData.ResetIterator();
	tagCSRoleData* pData = NULL;
	while (m_mapRoleData.PeekNext(pData))
	{
		SAFE_DEL(pData);
	}
	m_mapRoleData.Clear();
}

//--------------------------------------------------------------------
// 增加一条城战玩家数据
//--------------------------------------------------------------------
VOID CityStruggle::AddRoleDataInfo(DWORD dwRoleID)
{
	tagCSRoleData* pData = m_mapRoleData.Peek(dwRoleID);
	if (P_VALID(pData))
		return;

	pData = new tagCSRoleData;
	m_mapRoleData.Add(dwRoleID, pData);
}

//--------------------------------------------------------------------
// 城战击杀玩家增加击杀积分
//--------------------------------------------------------------------
VOID CityStruggle::AddKillRoleNum(DWORD dwKillerID, DWORD dwTargetID)
{
	tagCSRoleData* pData = m_mapRoleData.Peek(dwKillerID);
	if (!P_VALID(pData))
		return;

	if (pData->listKill.Size() < NOT_REPEAT_LIMIT_ROLE_NUM && !pData->listKill.IsExist(dwTargetID))
	{
		pData->listKill.PushBack(dwTargetID);
		AddRoleData(dwKillerID, ECSRD_Kills, 1);
	}
	else if (!pData->listKill.IsExist(dwTargetID))
	{
		pData->listKill.PopFront();
		pData->listKill.PushBack(dwTargetID);
		AddRoleData(dwKillerID, ECSRD_Kills, 1);
	}
	else
	{
		return;
	}

}

//--------------------------------------------------------------------
// 改变玩家城战数据
//--------------------------------------------------------------------
VOID CityStruggle::AddRoleData(DWORD dwRoleID, ECityStruggleRoleData eType, DWORD dwData)
{
	tagCSRoleData* pData = m_mapRoleData.Peek(dwRoleID);
	if (!P_VALID(pData))
		return;

	switch (eType)
	{
	case ECSRD_ActiveTime:
		if (pData->RoleData.byActiveTime < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byActiveTime += (BYTE)dwData;
		break;

	case ECSRD_Kills:
		if (pData->RoleData.byKills < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byKills += (BYTE)dwData;
		break;

	case ECSRD_DestroyDoor:
		if (pData->RoleData.byDestroyDoor < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byDestroyDoor += (BYTE)dwData;
		break;

	case ECSRD_DestroyMaterial:
		if (pData->RoleData.byDestroyMaterial < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byDestroyMaterial += (BYTE)dwData;
		break;

	case ECSRD_UseRepairStone:
		if (pData->RoleData.byUseRepairStone < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byUseRepairStone += (BYTE)dwData;
		break;

	case ECSRD_OpenYinYang:
		if (pData->RoleData.byOpenYinYang < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byOpenYinYang += (BYTE)dwData;
		break;

	case ECSRD_OpenLongZhu:
		if (pData->RoleData.byOpenLongZhu < ADD_SCORE_LIMIT[(INT)eType])
			pData->RoleData.byOpenLongZhu += (BYTE)dwData;
		break;
	case ECSRD_GuildExploit:			// 帮派功勋
		pData->nExploit += (BYTE)dwData;
		break;
	case ECSRD_GuildContribute:			// 帮派贡献
		pData->nContribute += (BYTE)dwData;
		break;
	case ECSRD_GuildMoney:				// 金钱奖励
		pData->nMoney += (BYTE)dwData;
		break;

	default:

		break;
	}

	pData->RoleData.nScore = pData->RoleData.byActiveTime * 2 + pData->RoleData.byKills + pData->RoleData.byDestroyDoor * 10
								+ pData->RoleData.byDestroyMaterial * 3 + pData->RoleData.byUseRepairStone + pData->RoleData.byOpenYinYang * 5
								+ pData->RoleData.byOpenLongZhu * 20;
}

//--------------------------------------------------------------------
// 城战玩家积分排序
//--------------------------------------------------------------------
VOID CityStruggle::SortByScore()
{
	// 清空上次排序
	m_vecScore.clear();

	// 开始排序
	m_mapRoleData.ResetIterator();
	DWORD dwRoleID = GT_INVALID;
	tagCSRoleData* pData = NULL;
	while (m_mapRoleData.PeekNext(dwRoleID, pData))
	{
		if (P_VALID(pData))
		{
			// 放到排序容器中
			m_vecScore.push_back(make_pair(dwRoleID, pData->RoleData.nScore));
		}
	}

	sort(m_vecScore.begin(), m_vecScore.end(), CSScoreRank());
}