//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_guild.cpp
// author: Sunnee
// actor:
// data: 2009-07-03
// last:
// brief: 帮派技能
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "guild.h"
#include "guild_skill.h"
#include "att_res.h"
#include "role.h"

#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_guild.h"
#include "MsgInfoMgr.h"

//-----------------------------------------------------------------------------
// 构造与析构
//-----------------------------------------------------------------------------
GuildSkill::GuildSkill()
{
	m_bInit			= FALSE;
	m_pGuild		= NULL;
	m_dwCurSkill	= GT_INVALID;
	m_mapGuildSkill.Clear();
}

GuildSkill::~GuildSkill()
{
	Destroy();
}

//-----------------------------------------------------------------------------
// 初始化和销毁
//-----------------------------------------------------------------------------
VOID GuildSkill::Init( Guild* pGuild, BOOL bRequest /*= FALSE*/ )
{
	ASSERT(P_VALID(pGuild));
	if (!P_VALID(pGuild))
	{
		return;
	}

	m_bInit			= FALSE;
	m_pGuild		= pGuild;
	m_dwCurSkill	= GT_INVALID;

	m_mapGuildSkill.Clear();

	// 发送数据库请求
	if (bRequest)
	{
		tagNDBC_LoadGuildSkillInfo send;
		send.dwGuildID	= m_pGuild->GetGuildAtt().dwID;
		g_dbSession.Send(&send, send.dwSize);
	}
}

VOID GuildSkill::Destroy()
{
	m_bInit			= FALSE;
	m_pGuild		= NULL;
	m_dwCurSkill	= GT_INVALID;
	
	tagGuildSkill* pSkillInfo = NULL;
	MapGuildSkill::TMapIterator iter = m_mapGuildSkill.Begin();
	while (m_mapGuildSkill.PeekNext(iter, pSkillInfo))
	{
		SAFE_DEL(pSkillInfo);
	}
	m_mapGuildSkill.Clear();
}

//-----------------------------------------------------------------------------
// 载入帮派技能信息
//-----------------------------------------------------------------------------
BOOL GuildSkill::LoadGuildSkillIInfo( tagGuildSkillInfo* pSkillInfo, INT nSkillNum )
{
	if (!P_VALID(pSkillInfo) || nSkillNum <= 0)
	{
		return FALSE;
	}

	if (!g_attRes.LoadGuildSkillInfo(m_mapGuildSkill))
		return FALSE;

	// 上层保证pSkillInfo的大小
	for (int n=0; n<nSkillNum; n++)
	{
		// 读取帮派技能静态属性
		tagGuildSkill* pGuildSkill	= m_mapGuildSkill.Peek(pSkillInfo[n].dwSkillID);
		if (!P_VALID(pGuildSkill))
			continue;

		if (!g_attRes.GetGuildSkillInfo(pSkillInfo[n].dwSkillID, pSkillInfo[n].nLevel, *pGuildSkill))
		{
			// 满级判断
			if (pSkillInfo[n].nLevel > g_attRes.GetGuildSkillMaxLvl(pSkillInfo[n].dwSkillID))
				pGuildSkill->bActive = pSkillInfo[n].bActive;
			continue;
		}
		pGuildSkill->n16Progress	= pSkillInfo[n].n16Progress;
		pGuildSkill->bActive		= pSkillInfo[n].bActive;

		// 设置当前研究技能
		if (pSkillInfo[n].bResearching)
		{
			if (!GT_VALID(m_dwCurSkill))
			{
				m_dwCurSkill = pSkillInfo[n].dwSkillID;
			}
			else if (pSkillInfo[n].dwSkillID != m_dwCurSkill)
			{
				// 数据库中有错误,纠正之
				ChangeResearchSkill(pSkillInfo[n].dwSkillID);
				m_dwCurSkill = pSkillInfo[n].dwSkillID;
			}
		}
	}

	// 初始化完成
	m_bInit = TRUE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// 获取所有帮派技能信息
//-----------------------------------------------------------------------------
DWORD GuildSkill::GetGuildSkillInfo( DWORD& dwSkillID, INT16& n16Progress, INT& nSkillNum, tagGuildSkills* pLevelInfo )
{
	if (!m_bInit)
	{
		return GT_INVALID;
	}

	// 上层保证pLevelInfo大小
	if (!P_VALID(pLevelInfo))
	{
		return GT_INVALID;
	}
	tagGuildSkill* pGuildSkill	= NULL;
	nSkillNum					= 0;
	dwSkillID					= GT_INVALID;
	n16Progress					= GT_INVALID;

	// 取得当前研究技能
	pGuildSkill = m_mapGuildSkill.Peek(m_dwCurSkill);
	if (P_VALID(pGuildSkill))
	{
		dwSkillID		= m_dwCurSkill * 100 + pGuildSkill->nLevel;
		n16Progress		= pGuildSkill->n16Progress;
	}
	
	MapGuildSkill::TMapIterator iter = m_mapGuildSkill.Begin();
	while (m_mapGuildSkill.PeekNext(iter, pGuildSkill))
	{
		if (!P_VALID(pGuildSkill))
		{
			continue;
		}

		pLevelInfo[nSkillNum].dwSkillID = pGuildSkill->dwSkillID * 100 + pGuildSkill->nLevel;
		pLevelInfo[nSkillNum].bActive = pGuildSkill->bActive;
		nSkillNum++;
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// 整点升级
//-----------------------------------------------------------------------------
DWORD GuildSkill::SkillUpgradeOnClock()
{
	if (!m_bInit)
	{
		return GT_INVALID;
	}

	// 取得当先研究技能
	if (!GT_VALID(m_dwCurSkill))
	{
		return E_GuildSkill_NotResearch;
	}

	tagGuildSkill* pGuildSkill = m_mapGuildSkill.Peek(m_dwCurSkill);
	if (!P_VALID(pGuildSkill))
	{
		return E_GuildSkill_NotExist;
	}

	// 技能达到等级上限
	if (pGuildSkill->nLevel > g_attRes.GetGuildSkillMaxLvl(pGuildSkill->dwSkillID))
	{
		return E_GuildSkill_Level_Limit;
	}

	// 帮派状态判定
	if (m_pGuild->IsInGuildStateAny(EGDSS_Shortage | EGDSS_Distress | EGDSS_Chaos))
	{
		return E_Guild_State_Limit;
	}

	FLOAT fModify = 1.0f;
	if (m_pGuild->IsInGuildState(EGDSS_Refulgence))
	{
		fModify = 1.5f;
	}
	else if (m_pGuild->IsInGuildState(EGDSS_Primacy))
	{
		fModify = 2.0f;
	}

	// 帮派资金资材限制
	const tagGuild& sAtt = m_pGuild->GetGuildAtt();
	if (sAtt.nFund < pGuildSkill->nResearchFund)
	{
		return E_Guild_Fund_NotEnough;
	}
	if (sAtt.nMaterial < pGuildSkill->nResearchMaterial)
	{
		return E_Guild_Material_NotEnough;
	}

	// 扣除帮派资源
	m_pGuild->DecGuildFund(GT_INVALID, pGuildSkill->nResearchFund, ELCLD_Guild_SkillResearch);
	m_pGuild->DecGuildMaterial(GT_INVALID, pGuildSkill->nResearchMaterial, ELCLD_Guild_SkillResearch);

	// 计算提升量(研究进度提升量=帮派等级×10×[1+（当前安定度-5000）/10000])
	INT16 n16Upgrade = INT16(sAtt.byLevel * 10.0f * (1.0f + (sAtt.n16Peace - 5000.0f) / 10000.0f) * fModify);

	pGuildSkill->n16Progress += n16Upgrade;
	if (pGuildSkill->n16Progress >= pGuildSkill->n16Fulfill)
	{
		// 取得等级上限
		INT nMaxLevel = g_attRes.GetGuildSkillMaxLvl(pGuildSkill->dwSkillID);

		pGuildSkill->nLevel++;
		if (pGuildSkill->nLevel > nMaxLevel)
		{
			// 达到技能上限
			m_dwCurSkill				= GT_INVALID;
			pGuildSkill->n16Progress	= 0;
			ChangeResearchSkill(GT_INVALID);

			// 通知客户端
			tagNS_SetResearchSkill send;
			send.dwSkillID		= GT_INVALID;
			send.n16Progress	= 0;
			m_pGuild->SendGuildMsg(&send, send.dwSize);
		}
		else
		{
			INT16 n16Fulfill = pGuildSkill->n16Fulfill;

			// 读取下一级帮派技能静态属性
			g_attRes.GetGuildSkillInfo(pGuildSkill->dwSkillID, pGuildSkill->nLevel, *pGuildSkill);

			pGuildSkill->n16Progress -= n16Fulfill;
		}

		// 帮派广播
		tagNS_GuildSkillLevelUp send_broad;
		send_broad.dwSkillID = pGuildSkill->dwSkillID * 100 + pGuildSkill->nLevel;
		m_pGuild->SendGuildMsg(&send_broad, send_broad.dwSize);
	}

	// 保存到数据库
	SaveSkillInfo2DB(pGuildSkill->dwSkillID);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 上缴物品升级
//-----------------------------------------------------------------------------
DWORD GuildSkill::SkillUpgradeByHandIn( Role* pRole, INT64 n64Serial, DWORD& dwSkillID, INT16& n16Progress )
{
	if (!m_bInit)
	{
		return GT_INVALID;
	}

	// 玩家权限判断
	tagGuildMember* pMember = m_pGuild->GetMember(pRole->GetID());
	if (!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}
// 	if (!m_pGuild->GetGuildPower(pMember->eGuildPos).bAdvSkill)
// 	{
// 		return E_Guild_Power_NotEnough;
// 	}

	// 取得当先研究技能
	if (!GT_VALID(m_dwCurSkill))
	{
		return E_GuildSkill_NotResearch;
	}

	tagGuildSkill* pGuildSkill = m_mapGuildSkill.Peek(m_dwCurSkill);
	if (!P_VALID(pGuildSkill))
	{
		return E_GuildSkill_NotExist;
	}

	// 技能达到等级上限(不大可能)
	if (pGuildSkill->nLevel > g_attRes.GetGuildSkillMaxLvl(pGuildSkill->dwSkillID))
	{
		return E_GuildSkill_Level_Limit;
	}

	// 上缴物品判定
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(n64Serial);
	if (!P_VALID(pItem) || pItem->pProtoType->eSpecFunc != EISF_GuildSkillBook)
	{
		return E_GuildSkill_Wrong_Item;
	}

	// 帮派状态判定
	if (m_pGuild->IsInGuildStateAny(EGDSS_Shortage | EGDSS_Distress | EGDSS_Chaos))
	{
		return E_Guild_State_Limit;
	}

	FLOAT fModify = 1.0f;
	if (m_pGuild->IsInGuildState(EGDSS_Refulgence))
	{
		fModify = 1.5f;
	}
	else if (m_pGuild->IsInGuildState(EGDSS_Primacy))
	{
		fModify = 2.0f;
	}

	// 帮派资金资材限制
	const tagGuild& sAtt = m_pGuild->GetGuildAtt();
	if (sAtt.nFund < pGuildSkill->nResearchFund)
	{
		return E_Guild_Fund_NotEnough;
	}
	if (sAtt.nMaterial < pGuildSkill->nResearchMaterial)
	{
		return E_Guild_Material_NotEnough;
	}

	// 扣除帮派资源
	m_pGuild->DecGuildFund(pRole->GetID(), pGuildSkill->nResearchFund, ELCLD_Guild_SkillResearch);
	m_pGuild->DecGuildMaterial(pRole->GetID(), pGuildSkill->nResearchMaterial, ELCLD_Guild_SkillResearch);

	// 计算提升量
	INT16 n16Upgrade = INT16(pItem->pProtoType->nSpecFuncVal1 * fModify);

	pGuildSkill->n16Progress += n16Upgrade;
	if (pGuildSkill->n16Progress >= pGuildSkill->n16Fulfill)
	{
		// 取得等级上限
		INT nMaxLevel = g_attRes.GetGuildSkillMaxLvl(pGuildSkill->dwSkillID);

		pGuildSkill->nLevel++;
		if (pGuildSkill->nLevel > nMaxLevel)
		{
			// 达到技能上限
			m_dwCurSkill				= GT_INVALID;
			pGuildSkill->n16Progress	= 0;
			ChangeResearchSkill(GT_INVALID);

			// 通知客户端
			tagNS_SetResearchSkill send;
			send.dwSkillID		= GT_INVALID;
			send.n16Progress	= 0;
			m_pGuild->SendGuildMsg(&send, send.dwSize);
		}
		else
		{
			INT16 n16Fulfill = pGuildSkill->n16Fulfill;

			// 读取下一级帮派技能静态属性
			g_attRes.GetGuildSkillInfo(pGuildSkill->dwSkillID, pGuildSkill->nLevel, *pGuildSkill);

			pGuildSkill->n16Progress -= n16Fulfill;
		}

		// 帮派广播
		tagNS_GuildSkillLevelUp send_broad;
		send_broad.dwSkillID = pGuildSkill->dwSkillID * 100 + pGuildSkill->nLevel;
		m_pGuild->SendGuildMsg(&send_broad, send_broad.dwSize);
	}

	// 广播通知所有公会成员
	DWORD dwMsgInfoID	= (DWORD)g_MsgInfoMgr.BeginMsgEvent();
	DWORD dwTmpNum		= 100053; // 玩家“<p1>”上缴了【<p2>】，为研究中的公会技能增砖添瓦！
	DWORD dwRoleID		= pRole->GetID();
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwTmpNum);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_RoleID, &dwRoleID);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_ItemTypeID, &(pItem->dwTypeID));
	g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, m_pGuild);

	// 保存到数据库
	SaveSkillInfo2DB(pGuildSkill->dwSkillID);

	// 增加玩家贡献值
	m_pGuild->ChangeMemberContribution(pRole->GetID(), pItem->pProtoType->nSpecFuncVal2, TRUE);

	// 扣除玩家物品
	pRole->GetItemMgr().RemoveFromRole(pItem->dwTypeID, 1, ELCLD_Guild_SkillResearch);

	// 设置传出参数
	dwSkillID	= pGuildSkill->dwSkillID * 100 + pGuildSkill->nLevel;
	n16Progress	= pGuildSkill->n16Progress;

	return E_Success;
}

//-----------------------------------------------------------------------------
// 学习技能
//-----------------------------------------------------------------------------
DWORD GuildSkill::LearnGuildSkill( Role* pRole, DWORD dwSkillID, INT& nLevel, BOOL& bLearnError )
{
	if (!m_bInit)
	{
		return GT_INVALID;
	}

	// 判断玩家
	if (!P_VALID(pRole) || !GT_VALID(dwSkillID))
	{
		return GT_INVALID;
	}

	bLearnError	= FALSE;

	tagGuildMember* pMember = m_pGuild->GetMember(pRole->GetID());
	if (!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}

	// 判断指定技能
	tagGuildSkill* pGuildSkill = m_mapGuildSkill.Peek(dwSkillID);
	if (!P_VALID(pGuildSkill))
	{
		return E_GuildSkill_NotExist;
	}

	// 判断已学技能
	BOOL bLearn = TRUE;
	Skill* pSkill = pRole->GetSkill(dwSkillID);
	if (P_VALID(pSkill))
	{
		bLearn = FALSE;		// 技能升级
		if (pGuildSkill->nLevel <= pSkill->GetLevel()+1)
		{
			return E_GuildSkill_Level_Limit;
		}
	}

	// 判断帮派状态
	if (m_pGuild->IsInGuildStateAny(EGDSS_Shortage | EGDSS_Distress | EGDSS_Warfare))
	{
		return E_Guild_State_Limit;
	}

	// 取得帮派技能属性
	const tagGuildSkill* pSkillProto = NULL;
	if (P_VALID(pSkill))
	{
		pSkillProto = g_attRes.GetGuildSkillProto(pSkill->GetTypeID() + 1);
	}
	else
	{
		pSkillProto = g_attRes.GetGuildSkillProto(Skill::CreateTypeID(dwSkillID, 1));
	}
	
	if (!P_VALID(pSkillProto))
	{
		return E_GuildSkill_Level_Limit;
	}

	// 检查学习公会技能等级限制
	if (pRole->GetLevel() < pSkillProto->n16LearnRestrictLvl)
	{
		return E_GuildSkill_Learn_Lvl_Limit;
	}

	// 检查消耗内容
	if (pRole->GetCurMgr().GetBagSilver() < pSkillProto->nLearnSilver)
	{
		return E_BagSilver_NotEnough;
	}
	if (pMember->nContribution < pSkillProto->nLearnContribution)
	{
		return E_GuildMember_Contribution_Limit;
	}
	if(m_pGuild->GetGuildAtt().nFund < pSkillProto->nLearnFund)
	{
		return E_Guild_Fund_NotEnough;
	}
	if (m_pGuild->GetGuildAtt().nMaterial < pSkillProto->nLearnMaterial)
	{
		return E_Guild_Material_NotEnough;
	}

	// 学习或者升级技能
	bLearnError			= TRUE;
	DWORD dwErrorCode	= GT_INVALID;
	if (bLearn)
	{
		dwErrorCode = pRole->LearnSkill(dwSkillID);
	}
	else
	{
		dwErrorCode = pRole->LevelUpSkill(dwSkillID);
	}

	if (E_Success == dwErrorCode)
	{
		// 扣除消耗
		pRole->GetCurMgr().DecBagSilver(pSkillProto->nLearnSilver, ELCLD_Guild_SkillLearn);
		m_pGuild->ChangeMemberContribution(pRole->GetID(), pSkillProto->nLearnContribution, FALSE);
// 		m_pGuild->DecGuildFund(pRole->GetID(), pSkillProto->nLearnFund, ELCLD_Guild_SkillLearn);
// 		m_pGuild->DecGuildMaterial(pRole->GetID(), pSkillProto->nLearnMaterial, ELCLD_Guild_SkillLearn);
	}

	// 设置传出参数
	pSkill = pRole->GetSkill(dwSkillID);
	if (P_VALID(pSkill))
	{
		nLevel	= pSkill->GetLevel();
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 创建帮派技能
//-----------------------------------------------------------------------------
VOID GuildSkill::CreateGuildSkill()
{
	// 读取初始帮派技能信息
	g_attRes.LoadGuildSkillInfo(m_mapGuildSkill);

	// 设置当前无研究技能
	m_dwCurSkill = GT_INVALID;

	tagGuildSkill* pSkillInfo = NULL;
	MapGuildSkill::TMapIterator iter = m_mapGuildSkill.Begin();
	while (m_mapGuildSkill.PeekNext(iter, pSkillInfo))
	{
		if (!P_VALID(pSkillInfo))
		{
			continue;
		}
		
		// 向数据库发送1级技能
		tagNDBC_CreateGuildSkill send;
		send.dwGuildID					= m_pGuild->GetGuildAtt().dwID;
		send.sSkillInfo.nLevel			= pSkillInfo->nLevel;
		send.sSkillInfo.dwSkillID		= pSkillInfo->dwSkillID;
		send.sSkillInfo.n16Progress		= pSkillInfo->n16Progress;
		send.sSkillInfo.bResearching	= FALSE;
		send.sSkillInfo.bActive		= pSkillInfo->bActive;

		g_dbSession.Send(&send, send.dwSize);
	}

	// 初始化完成
	m_bInit = TRUE;
}

//-----------------------------------------------------------------------------
// 设置当前升级技能
//-----------------------------------------------------------------------------
DWORD GuildSkill::SetCurUpgradeGuildSkill( DWORD dwRoleID, DWORD dwSkillID, INT& nLevel, INT16& n16Progress )
{
	if (!m_bInit)
	{
		return GT_INVALID;
	}

	if (!GT_VALID(dwRoleID))
	{
		return GT_INVALID;
	}

	// 同样的技能不做处理
	if (dwSkillID == m_dwCurSkill)
	{
		return GT_INVALID;
	}

	// 判断玩家权限
	tagGuildMember* pMember = m_pGuild->GetMember(dwRoleID);
	if (!P_VALID(pMember))
	{
		return E_Guild_MemberNotIn;
	}
	if (!m_pGuild->GetGuildPower(pMember->eGuildPos).bSetSkill)
	{
		return E_Guild_Power_NotEnough;
	}

	// 判断技能
	if (GT_VALID(dwSkillID))
	{
		tagGuildSkill* pGuildSkill = NULL;
		MapGuildSkill::TMapIterator iterator = m_mapGuildSkill.Begin();
		BOOL bFindSkill = FALSE;

		while(m_mapGuildSkill.PeekNext(iterator, pGuildSkill))
		{
			if (P_VALID(pGuildSkill) && pGuildSkill->dwSkillID == dwSkillID)
			{
				bFindSkill = TRUE; 
				break;
			}
		}
		if (!P_VALID(pGuildSkill) || !bFindSkill)
		{
			return E_GuildSkill_NotExist;
		}
		pGuildSkill = m_mapGuildSkill.Peek(dwSkillID);
		if (P_VALID(pGuildSkill) && pGuildSkill->n16ResearchRestrictLvl > m_pGuild->GetGuildFacilities().GetAcademyLevel())
		{
			return E_GuildSkill_Academy_Lvl_Limit;
		}
		if (pGuildSkill->bActive == FALSE)
		{
			return E_SystemError;
		}
		if (IsMaxSkillLevel(pGuildSkill->dwSkillID, pGuildSkill->nLevel))
		{
			return E_GuildSkill_Level_Limit;
		}
		n16Progress	= pGuildSkill->n16Progress;
		nLevel		= pGuildSkill->nLevel;

		// 读取技能静态属性
		g_attRes.GetGuildSkillInfo(pGuildSkill->dwSkillID, pGuildSkill->nLevel, *pGuildSkill);
	}

	// 设置当前升级技能
	ChangeResearchSkill(dwSkillID);
	m_dwCurSkill	= dwSkillID;

	return E_Success;
}

BOOL GuildSkill::IsMaxSkillLevel(DWORD dwSkillID, INT nLevel)
{
	INT nMaxLvl = g_attRes.GetGuildSkillMaxLvl(dwSkillID);
	if (nMaxLvl <= GT_INVALID)
	{
		return GT_INVALID;
	}
	if (nMaxLvl < nLevel)
	{
		return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// 与数据库通讯
//-----------------------------------------------------------------------------
VOID GuildSkill::SaveSkillInfo2DB(DWORD dwSkillID)
{
	if (!GT_VALID(dwSkillID))
	{
		return;
	}

	tagGuildSkill* pGuildSkill = m_mapGuildSkill.Peek(dwSkillID);
	if (!P_VALID(pGuildSkill))
	{
		return;
	}

	tagNDBC_SaveGuildSkill send;
	send.dwGuildID				= m_pGuild->GetGuildAtt().dwID;
	send.sSkillInfo.nLevel		= pGuildSkill->nLevel;
	send.sSkillInfo.dwSkillID	= pGuildSkill->dwSkillID;
	send.sSkillInfo.n16Progress	= pGuildSkill->n16Progress;
	send.sSkillInfo.bActive		= pGuildSkill->bActive;

	g_dbSession.Send(&send, send.dwSize);
}

VOID GuildSkill::ChangeResearchSkill( DWORD dwNewSkillID )
{
	tagNDBC_ChangeResearchSkill send;
	send.dwGuildID		= m_pGuild->GetGuildAtt().dwID;
	send.dwNewSkillID	= dwNewSkillID;

	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// 清除所有帮派技能信息
//-----------------------------------------------------------------------------
VOID GuildSkill::RemoveGuildSkillInfo()
{
	if (!m_bInit)
	{
		return;
	}

	tagNDBC_RemoveGuildSkill send;
	send.dwGuildID = m_pGuild->GetGuildAtt().dwID;

	g_dbSession.Send(&send, send.dwSize);
}

// 激活公会技能
DWORD GuildSkill::ActiveGuildSkill(DWORD dwRoleID, INT64 n64ItemID, DWORD dwGuildID, DWORD dwSkillID)
{
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (!m_bInit)
	{
		return GT_INVALID;
	}

	tagGuildSkill* pGuildSkill = m_mapGuildSkill.Peek(dwSkillID);
	if (!P_VALID(pGuildSkill))
	{
		return GT_INVALID;
	}

	// 先判断有没有公会技能激活道具
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(n64ItemID);
	if (P_VALID(pItem) && P_VALID(pItem->pProtoType) && pItem->pProtoType->eSpecFunc == EISF_GuildSkillActive)
	{
		if (P_VALID(pItem->pProtoType) && pItem->pProtoType->nSpecFuncVal1 == dwSkillID)
			return GT_INVALID;

		if (pGuildSkill->bActive)
			return E_GuildSkill_Already_Active;

		// 扣除有效的公会技能书，将对应技能“是否已激活”字段标记为可研究状态
		pGuildSkill->bActive = TRUE;
		SaveSkillInfo2DB(pGuildSkill->dwSkillID);
		pRole->GetItemMgr().DelFromBag(n64ItemID, ELCLD_Guild_Skill_Active, 1);
		return E_Success;
	}
	else
	{
		return E_GuildSkill_ActiveItem_Not_Exist;
	}
}
