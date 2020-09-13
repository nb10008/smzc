//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: team.cpp
// author: wjiang
// actor:
// data: 2008-12-10
// last:
// brief: 队伍类
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_group.h"

#include "team.h"
#include "role.h"
#include "role_mgr.h"
#include "creature.h"
#include "map_creator.h"
#include "map.h"
#include "map_instance.h"

// Jason 2009-11-29
#include "../WorldDefine/msg_show_item.h"
#include "../WorldDefine/chat_define.h"
#include "convenient_team.h"
#include "../WorldDefine/family_define.h"
#include "../WorldDefine/msg_family.h"
#include "..\WorldDefine\brother_define.h"

//-------------------------------------------------------------------------------------------------------
// 构造函数
//-------------------------------------------------------------------------------------------------------
Team::Team(DWORD dwTeamID, Role* pLeaderPtr, Role* pFirstMemPtr)
: m_dwTeamID(dwTeamID), m_dwGroupID(GT_INVALID), m_dwOwnInstanceID(GT_INVALID), m_dwOwnMapID(GT_INVALID),
  m_nMemNum(2), m_ePickMode(EPUM_Order), m_fExpFactor(0.0f), m_bNeedDelete(FALSE)
{
	m_dwTeamSynTick	=	g_world.GetWorldTick();

	// 初始化队员数组
	memset(m_dwMemID, 0xFF, sizeof(m_dwMemID));
	memset(m_pMemPtr, 0xFF, sizeof(m_pMemPtr));

	// 设置队员
	m_dwMemID[0]	= pLeaderPtr->GetID();
	m_pMemPtr[0]	= pLeaderPtr;
	m_dwMemID[1]	= pFirstMemPtr->GetID();
	m_pMemPtr[1]	= pFirstMemPtr;

	// 设置两个队员的队伍ID
	pLeaderPtr->SetTeamID(dwTeamID);
	pFirstMemPtr->SetTeamID(dwTeamID);

	// 初始化拾取列表
	//m_nPickList.push_back(0);
	//m_nPickList.push_back(1);
	
	// 队伍最高等级和最低等级
	m_nMaxLevel = max(pLeaderPtr->GetLevel(), pFirstMemPtr->GetLevel());
	m_nMinLevel = min(pLeaderPtr->GetLevel(), pFirstMemPtr->GetLevel());

	// 计算队伍经验参数
	CalExpFactor();
}

//-------------------------------------------------------------------------------------------------------
// 析构函数
//-------------------------------------------------------------------------------------------------------
Team::~Team()
{
	for(INT i = 0; i < MAX_TEAM_NUM; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) )
			m_pMemPtr[i]->SetTeamID(GT_INVALID);
		else
			break;
	}

	m_nPickList.clear();
}

//-------------------------------------------------------------------------------------------------------
// 发送队伍消息
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamMessage(LPVOID pMsg, DWORD dwSize)
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( !P_VALID(m_pMemPtr[i]) )
			break;
		m_pMemPtr[i]->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// 将所有的队员ID导出
//-------------------------------------------------------------------------------------------------------
VOID Team::ExportAllMemID(DWORD dwMemID[MAX_TEAM_NUM]) const
{
	IFASTCODE->MemCpy(dwMemID, m_dwMemID, sizeof(DWORD)*MAX_TEAM_NUM);
}

//-------------------------------------------------------------------------------------------------------
// 对所有队员触发任务事件
//-------------------------------------------------------------------------------------------------------
VOID Team::AllMemQuestEvent(BOOL bAddMem)
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( !P_VALID(m_pMemPtr[i]) )
			break;

		m_pMemPtr[i]->OnQuestEvent(EQE_Teammates);
	}
}

//-------------------------------------------------------------------------------------------------------
// 发送消息给队友
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamateMessage(DWORD dwRoleID, LPVOID pMsg, DWORD dwSize)
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) || m_dwMemID[i] != dwRoleID )
			m_pMemPtr[i]->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// 发送消息给同地图的队友
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamMesInSameMap(DWORD dwMapID, LPVOID pMsg, DWORD dwSize)
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) && dwMapID == m_pMemPtr[i]->GetMapID())
			m_pMemPtr[i]->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// 给可视链表外的小队成员发消息(不会给自己发)
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamMesOutBigVis(Role* pRole, LPVOID pMsg, DWORD dwSize)
{
	if( !P_VALID(pRole) ) return;

	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) && pRole != m_pMemPtr[i] 
			&& (pRole->GetMapID() != m_pMemPtr[i]->GetMapID() || !pRole->GetMap()->InSameBigVisTile(pRole, m_pMemPtr[i])) )
			m_pMemPtr[i]->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// 通知队友进入副本
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamInstanceNotigy(Role* pRole, LPVOID pMsg, DWORD dwSize)
{
	if( !P_VALID(pRole) ) return;

	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) && pRole != m_pMemPtr[i] 
		&& (pRole->GetMapID() != m_pMemPtr[i]->GetMapID() || !pRole->GetMap()->InSameBigVisTile(pRole, m_pMemPtr[i])) 
		&& !m_pMemPtr[i]->IsInRoleState(ERS_PrisonArea) 
		// Jason 2009-11-16 死亡状态的队友，不通知进入副本
		/*&& !m_pMemPtr[i]->IsInState(ES_Dead)*/ )
			m_pMemPtr[i]->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// 得到小队的平均等级
//-------------------------------------------------------------------------------------------------------
INT Team::GetTeamAvgLevel() const
{
	INT nTotalLevel = 0;
	for(INT i = 1; i < m_nMemNum; ++i)
	{
		if( !P_VALID(m_pMemPtr[i]) )
			break;
		nTotalLevel += m_pMemPtr[i]->GetLevel();
	}

	return nTotalLevel / m_nMemNum;
}

//-------------------------------------------------------------------------------------------------------
// 增加队友
//-------------------------------------------------------------------------------------------------------
INT Team::AddMember(Role* pInviter, Role* pReplyer)
{
	INT nRet = CanAddMember(pInviter, pReplyer);

	if( E_Success != nRet ) return nRet;

	pReplyer->SetTeamID(m_dwTeamID);	// 设置队伍ID

	CheckBrothersInTeam(pReplyer);
	// 设置队伍成员
	m_dwMemID[m_nMemNum] = pReplyer->GetID();
	m_pMemPtr[m_nMemNum] = pReplyer;

	// 重置队伍最高等级和最低等级
	m_nMaxLevel = max(m_nMaxLevel, pReplyer->GetLevel());
	m_nMinLevel = min(m_nMinLevel, pReplyer->GetLevel());

	// 重置队伍经验参数
	CalExpFactor();

	// 累加队伍人数
	++m_nMemNum;

	
	// 设置拾取
	//m_nPickList.push_back(m_nMemNum - 1);

	// 加入队伍事件
	OnAddMember(pReplyer);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 竞技场-服务器提供LUA调用接口
// LUA接口专用，在不通知玩家的情况下，将玩家加入已经存在的小队中。
//-------------------------------------------------------------------------------------------------------
INT Team::AddMemberWithoutNotify(DWORD dwRoleID)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return E_Team_Target_Not_Exit;
	}

	// 被邀请者已经有队
	if( GT_INVALID != pRole->GetTeamID() )
	{
		return E_Team_Target_Have_Team;
	}

	// 人数已满
	if( m_nMemNum >= MAX_TEAM_NUM )
	{
		return E_Team_Member_Full;
	}

	pRole->SetTeamID(m_dwTeamID);	// 设置队伍ID

	// 设置队伍成员
	m_dwMemID[m_nMemNum] = pRole->GetID();
	m_pMemPtr[m_nMemNum] = pRole;

	// 重置队伍最高等级和最低等级
	m_nMaxLevel = max(m_nMaxLevel, pRole->GetLevel());
	m_nMinLevel = min(m_nMinLevel, pRole->GetLevel());

	// 重置队伍经验参数
	CalExpFactor();

	// 累加队伍人数
	++m_nMemNum;

	// 设置拾取
	//m_nPickList.push_back(m_nMemNum - 1);

	tagNS_InviteReply send;
	send.bAgree = TRUE;
	send.dwErrorCode = E_Success;
	ExportAllMemID(send.dwTeamMemID);

	SendTeamMessage(&send, send.dwSize);
	SendRoleInitStateToTeam(pRole);
	SendTeamState(pRole);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 是否能加入小队
//-------------------------------------------------------------------------------------------------------
INT Team::CanAddMember(Role* pInviter, Role* pReplyer)
{
	// 邀请者不是队长
	if( !IsLeader(pInviter->GetID()) )
	{
		return E_Team_Not_Leader;
	}

	// 被邀请者已经有队
	if( GT_INVALID != pReplyer->GetTeamID() )
	{
		return E_Team_Target_Have_Team;
	}

	// 人数已满
	if( m_nMemNum >= MAX_TEAM_NUM )
	{
		return E_Team_Member_Full;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 踢掉队友
//-------------------------------------------------------------------------------------------------------
INT Team::KickMember(Role* pSrc, Role* pDest)
{
	if( !P_VALID(pSrc) || !P_VALID(pDest) ) return GT_INVALID;

	INT nIndex = GT_INVALID;

	INT nRet = CanKickMember(pSrc, pDest, nIndex);

	if( E_Success != nRet ) return nRet;
	
	Map* pMap = pDest->GetMap();
	if ( P_VALID(pMap) )
	{
		nRet = pMap->CanKickMember();
		if( E_Success != nRet ) return nRet;
	}
	
	// 可以踢掉队友，则踢掉
	DeleteMem(nIndex);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 是否可以踢人，如果可以踢，返回参数中存储可以踢的索引
//-------------------------------------------------------------------------------------------------------
INT Team::CanKickMember(Role* pSrc, Role* pDest, INT& nIndex)
{
	// 不是队长不能踢人
	if( !IsLeader(pSrc->GetID()) )
	{
		return E_Team_Not_Leader;
	}

	// 是不是自己
	if( pSrc->GetID() == pDest->GetID() )
	{
		return E_Team_Target_Not_Exit;
	}

	// 检查被踢者是不是在队伍里
	nIndex = IsRoleInTeam(pDest->GetID());
	if( GT_INVALID == nIndex || 0 == nIndex )	// 不能踢掉队长
	{
		return E_Team_Role_Not_In_Team;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 离开小队
//-------------------------------------------------------------------------------------------------------
INT Team::LeaveTeam(Role* pRole)
{
	INT nIndex = GT_INVALID;

	INT nRet = CanLeaveTeam(pRole, nIndex);

	if( E_Success != nRet ) return nRet;

	// 判断在某地图是否允许玩家离开
	Map* pMap = pRole->GetMap();
	if ( P_VALID(pMap) )
	{
		nRet = pMap->CanLeaveTeam();
		if( E_Success != nRet ) return nRet;
	}
	
	// 可以删除
	DeleteMem(nIndex);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 是否可以离开小队
//-------------------------------------------------------------------------------------------------------
INT Team::CanLeaveTeam(Role* pRole, INT& nIndex)
{
	// 检查玩家是不是在队伍里
	nIndex = IsRoleInTeam(pRole->GetID());
	if( GT_INVALID == nIndex )
	{
		return E_Team_Role_Not_In_Team;
	}

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 得到所有小队成员
//-------------------------------------------------------------------------------------------------------
DWORD Team::GetTemAllMemberID(DWORD aryRoleID[6])
{
	DWORD dwCount(0);
	for ( int i = 0; i < MAX_TEAM_NUM; i++)
	{
		if ( P_VALID(m_dwMemID[i]))
		{
			aryRoleID[i] = m_dwMemID[i];
			dwCount++;
		}
	}
	return dwCount;
}


//-------------------------------------------------------------------------------------------------------
// 删除某个索引的小队成员
//-------------------------------------------------------------------------------------------------------
VOID Team::DeleteMem(const INT nIndex)
{
	if( nIndex < 0 || nIndex >= m_nMemNum ) return;

	// 得到该小队成员
	Role* pMem = m_pMemPtr[nIndex];
	if( !P_VALID(pMem) ) return;

	// 去掉组队增益
	pMem->CancelTeamProfit(true);

	// 设置该成员的小队ID
	pMem->SetTeamID(GT_INVALID);

	// 重置该小队成员的结构
	for(INT n = nIndex; n < m_nMemNum - 1; ++n)
	{
		m_dwMemID[n] = m_dwMemID[n+1];
		m_pMemPtr[n] = m_pMemPtr[n+1];
	}
	m_dwMemID[m_nMemNum - 1] = GT_INVALID;
	m_pMemPtr[m_nMemNum - 1] = (Role*)GT_INVALID;
	--m_nMemNum;

	// 重置拾取顺序
	m_Lock.Acquire();
	m_nPickList.remove(nIndex);
	list<INT>::iterator it = m_nPickList.begin();
	while( it != m_nPickList.end() )
	{
		if(*it > nIndex)
			(*it)--;
		++it;
	}
	m_Lock.Release();

	// 重新计算小队最高,最低等级
	ReCalTeamLevel();

	// 重算队伍经验参数
	CalExpFactor();

	// 离开队伍事件
	OnDelMember(pMem);

	// 如果队伍人数小于等于1，则设置队伍删除
	if( m_nMemNum <= 1 )
	{
		m_bNeedDelete = TRUE;
	}
}

//-------------------------------------------------------------------------------------------------------
// 设置小队的拾取模式
//-------------------------------------------------------------------------------------------------------
INT Team::SetPickMode(Role* pRole, EPickMode ePickMode)
{
	// 不是队长不能设置
	if( !IsLeader(pRole->GetID()) )
	{
		return E_Team_Not_Leader;
	}

	// 判断拾取模式是否非法
	if( ePickMode != EPUM_Free && ePickMode != EPUM_Order )
	{
		return E_Team_Pick_Model_Not_Exit;
	}

	// 判断，成功，设置拾取模式
	m_ePickMode = ePickMode;

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 改变队长
//-------------------------------------------------------------------------------------------------------
INT Team::ChangeLeader(Role* pSrc, Role* pDest)
{
	// 不是队长，不能改变队长
	if( !IsLeader(pSrc->GetID()) )
	{
		return E_Team_Not_Leader;
	}

	// 查看是不是自己
	if( pSrc->GetID() == pDest->GetID() )
	{
		return E_Team_Target_Not_Exit;
	}

	// 查看玩家是不是在队伍中
	INT nIndex = IsRoleInTeam(pDest->GetID());
	if( GT_INVALID == nIndex || 0 == nIndex )
	{
		return E_Team_Role_Not_In_Team;
	}

	// 判断在某地图是否允许移交队长
	Map* pMap = pSrc->GetMap();
	if ( P_VALID(pMap) )
	{
		INT nRet = E_Success;
		nRet = pMap->CanChangeLeader();
		if( E_Success != nRet ) return nRet;
	}

	// 判断成功，交换两个人的位置
	swap(m_dwMemID[0], m_dwMemID[nIndex]);
	swap(m_pMemPtr[0], m_pMemPtr[nIndex]);

	// 交换拾取顺序
	m_Lock.Acquire();
	list<INT>::iterator it = m_nPickList.begin();
	while (it != m_nPickList.end())
	{
		if(*it == nIndex)
		{
			*it = 0;
		}
		else if(*it == 0)	
		{
			*it = nIndex;
		}

		++it;
	}
	m_Lock.Release();

	// 重算整个队伍的经验参数
	CalExpFactor();

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	EasyTeam.OnTeamChanged(ConvenientTeam::ECT_Creature,ConvenientTeam::ETE_ChangeLeader,this);

	return E_Success;
}

//-------------------------------------------------------------------------------------------------------
// 队伍成员的等级改变
//-------------------------------------------------------------------------------------------------------
VOID Team::OnMemLevelChange(Role* pRole)
{
	INT nIndex = IsRoleInTeam(pRole->GetID());

	if( GT_INVALID == nIndex ) return;

	m_nMaxLevel = max(m_nMaxLevel, pRole->GetLevel());
	m_nMinLevel = min(m_nMinLevel, pRole->GetLevel());
}

//-------------------------------------------------------------------------------------------------------
// 队长统御值改变
//-------------------------------------------------------------------------------------------------------
VOID Team::OnLeaderReinChange(Role* pRole)
{
	if( !IsLeader(pRole->GetID()) ) return;

	// 重算统御值
	CalExpFactor();
}

//-------------------------------------------------------------------------------------------------------
// 将新加入队伍的玩家同步队伍信息
//-------------------------------------------------------------------------------------------------------
VOID Team::SendRoleInitStateToTeam(Role* pNewMem)
{
	if( !P_VALID(pNewMem) ) return;

	for(INT n = 0; n < m_nMemNum; ++n)
	{
		if( !P_VALID(m_pMemPtr[n]) ) continue;

		// 如果是自己就跳过
		if( m_pMemPtr[n] == pNewMem ) continue;

		// 将新队员的消息发送给老队员
		tagNS_RoleStateToTeam sendToOld;

		sendToOld.dwRoleID	=	pNewMem->GetID();
		sendToOld.dwMapID	=	pNewMem->GetMapID();
		sendToOld.eClassEx	=	pNewMem->GetClassEx();
		sendToOld.bySex		=	pNewMem-> GetSex();
		sendToOld.nLevel	=	pNewMem->GetLevel();
		sendToOld.nMaxHP	=	pNewMem->GetAttValue(ERA_MaxHP);
		sendToOld.nHP		=	pNewMem->GetAttValue(ERA_HP);
		sendToOld.nMaxMP	=	pNewMem->GetAttValue(ERA_MaxMP);
		sendToOld.nMP		=	pNewMem->GetAttValue(ERA_MP);
		sendToOld.fX		=	pNewMem->GetCurPos().x;
		sendToOld.fZ		=	pNewMem->GetCurPos().z;
		for(INT i = 0; i < TEAM_DISPLAY_NUM; ++i)
		{
			sendToOld.dwEquipTypeID[i] = pNewMem->GetAvatarEquip().AvatarEquip[i].dwTypeID;
		}
		memcpy(&sendToOld.AvatarAtt, pNewMem->GetAvatar(), sizeof(tagAvatarAtt));

		m_pMemPtr[n]->SendMessage(&sendToOld, sizeof(sendToOld));

		// 将老队员的消息发送给新队员
		tagNS_RoleStateToTeam sendToNew;

		sendToNew.dwRoleID	=	m_pMemPtr[n]->GetID();
		sendToNew.dwMapID	=	m_pMemPtr[n]->GetMapID();
		sendToNew.eClassEx	=	m_pMemPtr[n]->GetClassEx();
		sendToNew.bySex		=	m_pMemPtr[n]-> GetSex();
		sendToNew.nLevel	=	m_pMemPtr[n]->GetLevel();
		sendToNew.nMaxHP	=	m_pMemPtr[n]->GetAttValue(ERA_MaxHP);
		sendToNew.nHP		=	m_pMemPtr[n]->GetAttValue(ERA_HP);
		sendToNew.nMaxMP	=	m_pMemPtr[n]->GetAttValue(ERA_MaxMP);
		sendToNew.nMP		=	m_pMemPtr[n]->GetAttValue(ERA_MP);
		sendToNew.fX		=	m_pMemPtr[n]->GetCurPos().x;
		sendToNew.fZ		=	m_pMemPtr[n]->GetCurPos().z;
		for(INT i = 0; i < TEAM_DISPLAY_NUM; ++i)
		{
			sendToNew.dwEquipTypeID[i] = m_pMemPtr[n]->GetAvatarEquip().AvatarEquip[i].dwTypeID;
		}
		memcpy(&sendToNew.AvatarAtt,m_pMemPtr[n]->GetAvatar(), sizeof(tagAvatarAtt));

		pNewMem->SendMessage(&sendToNew, sizeof(sendToNew));


	}
}

//-------------------------------------------------------------------------------------------------------
// 发送队伍信息给玩家
//-------------------------------------------------------------------------------------------------------
VOID Team::SendTeamState(Role* pNewMem)
{
	if( !P_VALID(pNewMem) ) return;

	tagNS_GetTeamID		send; 
	send.dwTeamID = GetTeamID();
	send.ePickMode = GetPickMode();

	pNewMem->SendMessage(&send, send.dwSize);
}

//-------------------------------------------------------------------------------------------------------
// 同步队友位置
//-------------------------------------------------------------------------------------------------------
VOID Team::UpdateTeamatePos()
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if(P_VALID(m_pMemPtr[i]))
		{
			tagNS_RolePosToTeam		send;
			send.dwRoleID = m_pMemPtr[i]->GetID();
			send.dwMapID = m_pMemPtr[i]->GetMapID();
			send.fX = m_pMemPtr[i]->GetCurPos().x;
			send.fZ = m_pMemPtr[i]->GetCurPos().z;
			SendTeamMesOutBigVis(m_pMemPtr[i], &send, send.dwSize);
		}	
	}
}

//-------------------------------------------------------------------------------------------------------
// 有人离队时,重新计算小队最高,最低等级
//-------------------------------------------------------------------------------------------------------
VOID Team::ReCalTeamLevel()
{
	if(P_VALID(m_pMemPtr[0]))
	{
		m_nMaxLevel = m_pMemPtr[0]->GetLevel();
		m_nMinLevel = m_pMemPtr[0]->GetLevel();
	}

	for(INT i = 1; i < m_nMemNum; ++i)
	{
		if(!P_VALID(m_pMemPtr[i]))
			continue;

		if(m_pMemPtr[i]->GetLevel() > m_nMaxLevel)
			m_nMaxLevel = m_pMemPtr[i]->GetLevel();

		if(m_pMemPtr[i]->GetLevel() < m_nMinLevel)
			m_nMinLevel = m_pMemPtr[i]->GetLevel();
	}
}

//-------------------------------------------------------------------------------------------------------
// 设置小队的杀怪经验因数
//-------------------------------------------------------------------------------------------------------
VOID Team::CalExpFactor()
{
	if(m_nMemNum <=0 || m_bNeedDelete) return;
	FLOAT fLeaderRein = (FLOAT)m_pMemPtr[0]->GetAttValue(ERA_Rein);

	// De2= ((150-（1-队长统御力/999）*（F-A）)/150)^4, 小队经验玩家等级修正值
	FLOAT fDe2 = pow((150.0f - (1.0f - fLeaderRein / 999.0f) * ((FLOAT)m_nMaxLevel - (FLOAT)m_nMinLevel)) / 150.0f, 4.0f);

	m_fExpFactor = fDe2;
}

//-------------------------------------------------------------------------------------------------------
// 获得小队掉落拾取玩家
//-------------------------------------------------------------------------------------------------------
Role* Team::GetTeamPickRole(Creature* pCreature) const
{
	if(!P_VALID(pCreature)) return NULL;

	// 轮流拾取
	if( EPUM_Order == m_ePickMode )
	{
		m_Lock.Acquire();
		for(int nTry =0 ; nTry<2; nTry++)
		{
			if(m_nPickList.empty())
			{
				// 创建随机列表
				INT temp[MAX_TEAM_NUM];
				for(int i=0; i< MAX_TEAM_NUM; i++) temp[i] = i;
				INT nCount = m_nMemNum;
				while(nCount> 0)
				{
					int index = nCount==1?0: IUTIL->Rand() %nCount;
					m_nPickList.push_back(temp[index]);
					temp[index] = temp[nCount-1];
					nCount--;
				}
			}
			list<INT>::iterator it = m_nPickList.begin();
			list<INT>::iterator it2 = it;
			while( it != m_nPickList.end() )
			{
				INT nIndex = *it;
				if(P_VALID(m_pMemPtr[nIndex]) && pCreature->IsLootShareDistance(m_pMemPtr[nIndex]))
				{					
					m_nPickList.erase(it);
					//m_nPickList.push_back(nIndex);
					m_Lock.Release();
					return m_pMemPtr[nIndex];
				}
				it2 = it;
				it++;
				m_nPickList.erase(it2);
			}
		}
		m_Lock.Release();

		// 如果没有，就是全体
		return NULL;
	}
	// 自由拾取
	else if( EPUM_Free == m_ePickMode )
	{
		return NULL;
	}
	// 其它
	else
	{
		return NULL;
	}
}

//-----------------------------------------------------------------------------------------------------------
// 队伍创建时
//-----------------------------------------------------------------------------------------------------------
VOID Team::OnCreate()
{
	// 将队长的副本ID设置为队伍的副本ID，同时清空队长的副本ID
	SetOwnInstanceMapID(m_pMemPtr[0]->GetMyOwnInstanceMapID());
	SetOwnInstanceID(m_pMemPtr[0]->GetMyOwnInstanceID());
	m_pMemPtr[0]->SetMyOwnInstanceMapID(GT_INVALID);
	m_pMemPtr[0]->SetMyOwnInstanceID(GT_INVALID);

	// 如果副本ID不为空，则设置原来单人副本为队伍副本
	if( GT_INVALID != GetOwnInstanceID() && GT_INVALID != GetOwnInstanceMapID() )
	{
		Map* pMap = g_mapCreator.GetMap(GetOwnInstanceMapID(), GetOwnInstanceID());

		// 确定是普通副本
		if( P_VALID(pMap) && EMT_Instance == pMap->GetMapType() )
		{
			MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
			if( P_VALID(pInstance) )
			{
				pInstance->OnTeamCreate(this);
			}
		}
	}

	// 检查队员是否创建过副本
	DWORD dwMemInstanceMapID = m_pMemPtr[1]->GetMyOwnInstanceMapID();
	DWORD dwMemInstanceID = m_pMemPtr[1]->GetMyOwnInstanceID();

	if( GT_INVALID != dwMemInstanceMapID && GT_INVALID != dwMemInstanceID )
	{
		m_pMemPtr[1]->SetMyOwnInstanceMapID(GT_INVALID);
		m_pMemPtr[1]->SetMyOwnInstanceID(GT_INVALID);

		g_mapCreator.SetInstanceDelete(dwMemInstanceMapID, dwMemInstanceID);
	}

	CheckBrothersInTeam(m_pMemPtr[1]);

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	EasyTeam.OnTeamChanged(ConvenientTeam::ECT_Creature,ConvenientTeam::ETE_New,this);
}

//-----------------------------------------------------------------------------------------------------------
// 队伍删除时
//-----------------------------------------------------------------------------------------------------------
VOID Team::OnDelete()
{
	// 如果该队伍有副本，则设置副本删除
	if( GT_INVALID != GetOwnInstanceID() && GT_INVALID != GetOwnInstanceMapID() )
	{
		Map* pMap = g_mapCreator.GetMap(GetOwnInstanceMapID(), GetOwnInstanceID());

		// 确定是普通副本
		if( P_VALID(pMap) && EMT_Instance == pMap->GetMapType() )
		{
			MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
			if( P_VALID(pInstance) )
			{
				pInstance->OnTeamDelete(this);
			}
		}
	}

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	EasyTeam.OnTeamChanged(ConvenientTeam::ECT_Creature,ConvenientTeam::ETE_Del,this);
}

//------------------------------------------------------------------------------------------------------------
// 当加入一个玩家
//------------------------------------------------------------------------------------------------------------
VOID Team::OnAddMember(Role* pRole)
{
	if( !P_VALID(pRole) ) return;

	// 检查队伍现在是否创建了副本
	if( GT_INVALID != GetOwnInstanceID() && GT_INVALID != GetOwnInstanceMapID() )
	{
		Map* pMap = g_mapCreator.GetMap(GetOwnInstanceMapID(), GetOwnInstanceID());

		// 确定是普通副本
		if( P_VALID(pMap) && EMT_Instance == pMap->GetMapType() )
		{
			MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
			if( P_VALID(pInstance) )
			{
				pInstance->OnRoleEnterTeam(pRole->GetID(), this);
			}
		}
	}

	// 检查队员是否创建过副本
	DWORD dwMemInstanceMapID = pRole->GetMyOwnInstanceMapID();
	DWORD dwMemInstanceID = pRole->GetMyOwnInstanceID();

	if( GT_INVALID != dwMemInstanceMapID && GT_INVALID != dwMemInstanceID )
	{
		pRole->SetMyOwnInstanceMapID(GT_INVALID);
		pRole->SetMyOwnInstanceID(GT_INVALID);

		g_mapCreator.SetInstanceDelete(dwMemInstanceMapID, dwMemInstanceID);
	}

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	EasyTeam.OnTeamChanged(ConvenientTeam::ECT_Creature,ConvenientTeam::ETE_MemAdd,this);
}

//-------------------------------------------------------------------------------------------------------------
// 当删除一个玩家
//-------------------------------------------------------------------------------------------------------------
VOID Team::OnDelMember(Role* pRole)
{
	if( !P_VALID(pRole) ) return;

	// 检查队伍现在是否创建了副本
	if( GT_INVALID != GetOwnInstanceID() && GT_INVALID != GetOwnInstanceMapID() )
	{
		Map* pMap = g_mapCreator.GetMap(GetOwnInstanceMapID(), GetOwnInstanceID());

		// 确定是普通副本
		if( P_VALID(pMap) && EMT_Instance == pMap->GetMapType() )
		{
			MapInstanceNormal* pInstance = static_cast<MapInstanceNormal*>(pMap);
			if( P_VALID(pInstance) )
			{
				pInstance->OnRoleLeaveTeam(pRole->GetID(), this);
			}
		}
	}

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	EasyTeam.OnTeamChanged(ConvenientTeam::ECT_Creature,ConvenientTeam::ETE_MemRemove,this);
}

VOID Team::BroadcastLootItemMsgInTeam(Role * pOwner,tagItem * pItem)
{
	if( !P_VALID(pOwner) ) return;
	if( !P_VALID(pItem) ) return;

	DWORD dwTeamID = pOwner->GetTeamID();
	
	if(GT_INVALID != dwTeamID)
	{
		Team * pTeam = const_cast<Team*>( g_groupMgr.GetTeamPtr  (dwTeamID) );
		if( P_VALID(pTeam) )
		{
			BOOL canbroadcast = FALSE;
			if( MIsEquipment(pItem->dwTypeID) )
			{
				MTRANS_POINTER(pEquip,pItem,tagEquip)
				if( pEquip->equipSpec.byQuality >= EIQ_Blue )
					canbroadcast = TRUE;
			}
			else if( pItem->pProtoType->byQuality >= EIQ_Blue )
			{
				canbroadcast = TRUE;
			}
			if(canbroadcast)
			{
				tagNS_RoleShowDroptItemInTeam msg;
				msg.byFabao = 0;
				msg.byChannel	= ESCC_Combat;
				msg.dwSrcRoleID = pOwner->GetID();
				msg.dwTypeID	= pItem->dwTypeID;
				if(MIsEquipment(pItem->dwTypeID))
				{
					msg.byQlty = ((tagEquip*)pItem)->equipSpec.byQuality;
					INT _size = sizeof( tagNS_RoleShowDroptItemInTeam ) - sizeof(TCHAR) + 4;
					INT _toCopyLen = 0;					

					switch (pItem->pProtoType->eType)
					{
					case EIT_FaBao:
						{
							_size += SIZE_FABAO;
							_toCopyLen = SIZE_FABAO;
							msg.byFabao = 1;
							MCREATE_MSG(pSend,_size,NS_RoleShowDroptItemInTeam);
							memcpy(pSend,&msg,msg.dwSize);
							pSend->dwSize = _size;
							memcpy(pSend->byEquipData,pItem,_toCopyLen);
							g_roleMgr.GetRoleNameByID  (pOwner->GetID  (),pSend->szPickerName);
							pTeam->SendTeamMessage(pSend,pSend->dwSize);
							MDEL_MSG(pSend);
							break;
						}
					case EIT_SoulCrystal:
						{
							_size += SIZE_HUNJING;
							_toCopyLen = SIZE_HUNJING;
							msg.byFabao = 2;	
							MCREATE_MSG(pSend,_size,NS_RoleShowDroptItemInTeam);
							memcpy(pSend,&msg,msg.dwSize);
							pSend->dwSize = _size;
							memcpy(pSend->byEquipData,pItem,_toCopyLen);
							g_roleMgr.GetRoleNameByID  (pOwner->GetID  (),pSend->szPickerName);
							pTeam->SendTeamMessage(pSend,pSend->dwSize);
							MDEL_MSG(pSend);
							break;
						}
					case EIT_Holy:
						{
							// 如果是交易圣灵，若装备圣纹，则用其TypeId和充能次数替换 数组中存放的圣纹64位Id
							ItemMgr &itemMgr = pOwner->GetItemMgr();
							tagHolyMan *pHoly = (tagHolyMan *)pItem;

							tagHolyMan tmpHoly;
							IFASTCODE->MemCpy(&tmpHoly, pHoly, sizeof(tagHolyMan));

							for (int i=0; i< MaxHolyEquipNum; i++)
							{
								if (tmpHoly.n64EquipSerialIDs[i] != 0)
								{
									tagHolyEquip * pHolyEquip = (tagHolyEquip*)itemMgr.GetHolyManBarItem(tmpHoly.n64EquipSerialIDs[i]);
									if (P_VALID(pHolyEquip))
									{
										INT64 nTmpType = (INT64)pHolyEquip->dwTypeID;
										tmpHoly.n64EquipSerialIDs[i] = nTmpType * 1000 + pHolyEquip->n16EnhanceCount;										
									}
								}
							}						

							_size += SIZE_HOLY;
							_toCopyLen = SIZE_HOLY;
							msg.byFabao = 3;
							MCREATE_MSG(pSend,_size,NS_RoleShowDroptItemInTeam);
							memcpy(pSend,&msg,msg.dwSize);
							pSend->dwSize = _size;
							memcpy(pSend->byEquipData,&tmpHoly,_toCopyLen);
							g_roleMgr.GetRoleNameByID  (pOwner->GetID  (),pSend->szPickerName);
							pTeam->SendTeamMessage(pSend,pSend->dwSize);
							MDEL_MSG(pSend);
							break;			
						}
					case EIT_HolyEquip:
						{
							_size += SIZE_HOLYEQUIP;
							_toCopyLen = SIZE_HOLYEQUIP;
							msg.byFabao = 4;
							MCREATE_MSG(pSend,_size,NS_RoleShowDroptItemInTeam);
							memcpy(pSend,&msg,msg.dwSize);
							pSend->dwSize = _size;
							memcpy(pSend->byEquipData,pItem,_toCopyLen);
							g_roleMgr.GetRoleNameByID  (pOwner->GetID  (),pSend->szPickerName);
							pTeam->SendTeamMessage(pSend,pSend->dwSize);
							MDEL_MSG(pSend);
							break;			
						}
					default:
						_size += SIZE_EQUIP;
						_toCopyLen = SIZE_EQUIP;
						MCREATE_MSG(pSend,_size,NS_RoleShowDroptItemInTeam);
						memcpy(pSend,&msg,msg.dwSize);
						pSend->dwSize = _size;
						memcpy(pSend->byEquipData,pItem,_toCopyLen);
						g_roleMgr.GetRoleNameByID  (pOwner->GetID  (),pSend->szPickerName);
						pTeam->SendTeamMessage(pSend,pSend->dwSize);
						MDEL_MSG(pSend);
						break;
					}
					
					return ;
				}
				else 
					msg.byQlty = 0;
				g_roleMgr.GetRoleNameByID  (pOwner->GetID  (),msg.szPickerName);
				pTeam->SendTeamMessage(&msg,msg.dwSize);
			}
		}
	}
}

VOID	Team::BroadcastLootItemMsgInTeamExceptPicker(Role * pPicker,tagItem * pItem)
{
	if( !P_VALID(pPicker) ) return;
	if( !P_VALID(pItem) ) return;

	DWORD dwTeamID = pPicker->GetTeamID();

	if(GT_INVALID != dwTeamID)
	{
		Team * pTeam = const_cast<Team*>( g_groupMgr.GetTeamPtr  (dwTeamID) );
		if( P_VALID(pTeam) )
		{
			BOOL canbroadcast = FALSE;
			if( MIsEquipment(pItem->dwTypeID) )
			{
				MTRANS_POINTER(pEquip,pItem,tagEquip)
					if( pEquip->equipSpec.byQuality >= EIQ_Blue )
						canbroadcast = TRUE;
			}
			else if( pItem->pProtoType->byQuality >= EIQ_Blue )
			{
				canbroadcast = TRUE;
			}
			if(canbroadcast)
			{
				tagNS_RoleShowDroptItemInTeam msg;
				msg.byFabao = 0;
				msg.byChannel	= ESCC_Current;
				msg.dwSrcRoleID = pPicker->GetID();
				msg.dwTypeID	= pItem->dwTypeID;
				if(MIsEquipment(pItem->dwTypeID))
				{
					msg.byQlty = ((tagEquip*)pItem)->equipSpec.byQuality;
					INT _size = sizeof( tagNS_RoleShowDroptItemInTeam ) - sizeof(TCHAR) + 4;
					INT _toCopyLen = 0;					

					switch (pItem->pProtoType->eType)
					{
					case EIT_FaBao:
						{
							_size += SIZE_FABAO;
							_toCopyLen = SIZE_FABAO;
							msg.byFabao = 1;
							MCREATE_MSG(pSend,_size,NS_RoleShowDroptItemInTeam);
							memcpy(pSend,&msg,msg.dwSize);
							pSend->dwSize = _size;
							memcpy(pSend->byEquipData,pItem,_toCopyLen);
							g_roleMgr.GetRoleNameByID  (pPicker->GetID  (),pSend->szPickerName);
							pTeam->SendTeamMessage(pSend,pSend->dwSize);
							MDEL_MSG(pSend);
							break;
						}
					case EIT_SoulCrystal:
						{
							_size += SIZE_HUNJING;
							_toCopyLen = SIZE_HUNJING;
							msg.byFabao = 2;
							MCREATE_MSG(pSend,_size,NS_RoleShowDroptItemInTeam);
							memcpy(pSend,&msg,msg.dwSize);
							pSend->dwSize = _size;
							memcpy(pSend->byEquipData,pItem,_toCopyLen);
							g_roleMgr.GetRoleNameByID  (pPicker->GetID  (),pSend->szPickerName);
							pTeam->SendTeamMessage(pSend,pSend->dwSize);
							MDEL_MSG(pSend);
							break;
						}
					case EIT_Holy:
						{
							// 如果是交易圣灵，若装备圣纹，则用其TypeId和充能次数替换 数组中存放的圣纹64位Id
							ItemMgr &itemMgr = pPicker->GetItemMgr();
							tagHolyMan *pHoly = (tagHolyMan *)pItem;

							tagHolyMan tmpHoly;
							IFASTCODE->MemCpy(&tmpHoly, pHoly, sizeof(tagHolyMan));

							for (int i=0; i< MaxHolyEquipNum; i++)
							{
								if (tmpHoly.n64EquipSerialIDs[i] != 0)
								{
									tagHolyEquip * pHolyEquip = (tagHolyEquip*)itemMgr.GetHolyManBarItem(tmpHoly.n64EquipSerialIDs[i]);
									if (P_VALID(pHolyEquip))
									{
										tmpHoly.n64EquipSerialIDs[i] = pHolyEquip->dwTypeID * 1000 + pHolyEquip->n16EnhanceCount;
									}
								}
							}		

							_size += SIZE_HOLY;
							_toCopyLen = SIZE_HOLY;
							msg.byFabao = 3;
							MCREATE_MSG(pSend,_size,NS_RoleShowDroptItemInTeam);
							memcpy(pSend,&msg,msg.dwSize);
							pSend->dwSize = _size;
							memcpy(pSend->byEquipData,pItem,_toCopyLen);
							g_roleMgr.GetRoleNameByID  (pPicker->GetID  (),pSend->szPickerName);
							pTeam->SendTeamMessage(pSend,pSend->dwSize);
							MDEL_MSG(pSend);
							break;			
						}
					case EIT_HolyEquip:
						{
							_size += SIZE_HOLYEQUIP;
							_toCopyLen = SIZE_HOLYEQUIP;
							msg.byFabao = 4;
							MCREATE_MSG(pSend,_size,NS_RoleShowDroptItemInTeam);
							memcpy(pSend,&msg,msg.dwSize);
							pSend->dwSize = _size;
							memcpy(pSend->byEquipData,pItem,_toCopyLen);
							g_roleMgr.GetRoleNameByID  (pPicker->GetID  (),pSend->szPickerName);
							pTeam->SendTeamMessage(pSend,pSend->dwSize);
							MDEL_MSG(pSend);
							break;			
						}
					default:
						_size += SIZE_EQUIP;
						_toCopyLen = SIZE_EQUIP;
						MCREATE_MSG(pSend,_size,NS_RoleShowDroptItemInTeam);
						memcpy(pSend,&msg,msg.dwSize);
						pSend->dwSize = _size;
						memcpy(pSend->byEquipData,pItem,_toCopyLen);
						g_roleMgr.GetRoleNameByID  (pPicker->GetID  (),pSend->szPickerName);
						pTeam->SendTeamMessage(pSend,pSend->dwSize);
						MDEL_MSG(pSend);
						break;
					}		
					
					return ;
				}
				else
					msg.byQlty = 0;
				g_roleMgr.GetRoleNameByID  (pPicker->GetID  (),msg.szPickerName);
				pTeam->SendTeamMessageExceptSomeone(&msg,msg.dwSize,pPicker);
			}
		}
	}
}
VOID			Team::SendTeamMessageExceptSomeone(LPVOID pMsg, DWORD dwSize,Role* pExpRole)
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if( P_VALID(m_pMemPtr[i]) && pExpRole != m_pMemPtr[i] )
		{
			m_pMemPtr[i]->SendMessage(pMsg, dwSize);
		}
	}
}

//-------------------------------------------------------------------------------------------------------
// 更新各队员的组队增益
//-------------------------------------------------------------------------------------------------------
VOID Team::UpdateTeammatesTeamProfit()
{
	for(INT i = 0; i < m_nMemNum; ++i)
	{
		if(P_VALID(m_pMemPtr[i]))
		{
			if( m_pMemPtr[i]->CanGetTeamProfit() )
			{
				m_pMemPtr[i]->GetTeamProfit();
			}
			else
			{
				m_pMemPtr[i]->CancelTeamProfit();
			}
		}	
	}
}

VOID	Team::DeleteTeamBloodBrotherBuff(Role* pRole) const
{
	if(!P_VALID(pRole)) return;

	pRole->CancelBrotherProfit();

	if(pRole->GetBrotherMgr().CanMakeBrother())		// 说明不是小弟，判断有没有小弟在
	{
		DWORD dwRoleID = pRole->GetID();

		for(INT i = 0; i < m_nMemNum; ++i)
		{
			if(P_VALID(m_pMemPtr[i]))
			{
				if( m_pMemPtr[i]->GetBrotherMgr().GetTeatherID()==  dwRoleID)
				{
					m_pMemPtr[i]->CancelBrotherProfit();
				}
			}	
		}
	}
	else		//是小弟，判断师傅在不在
	{
		DWORD dwRoleID = pRole->GetBrotherMgr().GetTeatherID();

		for(INT i = 0; i < m_nMemNum; ++i)
		{
			if(P_VALID(m_pMemPtr[i]))
			{
				if( m_pMemPtr[i]->GetID()==  dwRoleID)
				{
					m_pMemPtr[i]->CancelBrotherProfit();
				}
			}	
		}
	}
	
}

DWORD Team::CanCreateFamily()
{
	// 	判断队伍成员数量是否≥4人，若不符合，则提示：“您的队伍人数必须≥4人，否则无法创建家族”，若符合，则进入下一步
	if (GetMemNum() < MIN_CREATE_FAMILY_TEAM_MEMBER)
		return E_CreateFamily_TeamMemNotEnough;

	for (INT nCnt = 0; nCnt < MAX_TEAM_NUM; nCnt++)
	{
		if (P_VALID(m_pMemPtr[nCnt]))
		{
			if (P_VALID(m_pMemPtr[nCnt]->GetFamilyID()))
			{
				// 	判断全部队伍成员是否加入了其他家族，若有成员加入了其他家族，则提示：“您的队伍成员有人已经加入了其他家族，无法创建”，若均为加入其它家族，则进入下一步
				return E_CreateFamily_MemberHaveFamily;
			}
			else if(m_pMemPtr[nCnt]->IsHaveBuff(Buff::GetIDFromTypeID(LEAVE_FAMILY_BUFF_ID)))
			{
				// 	判断全部队伍成员是否拥有“脱离家族”状态，若有成员拥有，则提示：“您的队伍成员里有人刚脱离家族，无法创建”，若未拥有，则进入下一步
				return E_CreateFamily_MemberJustLeaveFamily;
			}
		}
	}
	return E_FAMILY_SUCCESS;
}

VOID Team::CheckBrothersInTeam(Role * pRole)
{
	for (INT nCnt = 0; nCnt < MAX_TEAM_NUM; nCnt++)
	{
		if (P_VALID(m_pMemPtr[nCnt]))
		{
			// 如果新加进来的角色有师傅在队伍里面
			if ( pRole->IsBrotherOf(m_pMemPtr[nCnt]->GetID()))
			{
				pRole->TryAddBuffToMyself(BloodBrotherBuffBrother);
				m_pMemPtr[nCnt]->TryAddBuffToMyself(BloodBrotherBuffTeather);
			}
			// 如果新加进来的角色有小弟在队伍里面
			if ( m_pMemPtr[nCnt]->IsBrotherOf(pRole->GetID()))
			{
				pRole->TryAddBuffToMyself(BloodBrotherBuffTeather);
				m_pMemPtr[nCnt]->TryAddBuffToMyself(BloodBrotherBuffBrother);
			}
		}
	}
}