//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_member.cpp
// author: Sxg
// actor:
// data: 2009-4-7
// last:
// brief: 帮派成员
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "guild_member.h"
#include "role.h"
#include "guild.h"
#include "guild_mgr.h"
#include "role_mgr.h"
#include "role_mgr.h"
#include "db_session.h"
#include "att_res.h"
#include "../WorldDefine/guild_define.h"
#include "../WorldDefine/buff_define.h"
#include "../ServerDefine/msg_guild.h"
#include "title_mgr.h"
//-------------------------------------------------------------------------------
// 构造&析构
//-------------------------------------------------------------------------------
GuildMember::GuildMember():m_MemberChangeFlag(FALSE)
{
}

GuildMember::~GuildMember()
{
	tagGuildMember *pMember;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		SAFE_DEL(pMember);
	}

	m_mapMember.Clear();
}

//-------------------------------------------------------------------------------
// 添加成员
//-------------------------------------------------------------------------------
VOID GuildMember::AddMember(DWORD dwInviteeID, EGuildMemberPos ePos, BOOL bSave2DB/* = TRUE*/)
{
	// 检查是否已加入 -- LoongDB瞬断
	ASSERT(!m_mapMember.IsExist(dwInviteeID));
	
	tagGuildMember *pNewMember = new tagGuildMember(dwInviteeID, ePos);
	if(!P_VALID(pNewMember))
	{
		ASSERT(0);
		return;
	}

	m_mapMember.Add(dwInviteeID, pNewMember);

	if(bSave2DB)
	{
		tagNDBC_JoinGuild send;
		send.sGuildMemInfo.dwRoleID		= dwInviteeID;
		send.sGuildMemInfo.dwGuildID	= m_dwGuildID;
		send.sGuildMemInfo.n8GuildPos	= ePos;

		g_dbSession.Send(&send, send.dwSize);
	}

	// 给所有帮派成员发送在线人数变化消息
	SyncOnlineNum2AllGuildMumber();

	m_MemberChangeFlag = TRUE;
}

//-------------------------------------------------------------------------------
// 载入成员
//-------------------------------------------------------------------------------
VOID GuildMember::LoadMember(const tagGuildMember& sGuildMember)
{
	if (m_mapMember.IsExist(sGuildMember.dwRoleID))
	{
		return;
	}

	tagGuildMember *pNewMember = new tagGuildMember(sGuildMember);
	if(!P_VALID(pNewMember))
	{
		ASSERT(0);
		return;
	}

	m_mapMember.Add(sGuildMember.dwRoleID, pNewMember);
}

//-------------------------------------------------------------------------------
// 删除成员
//-------------------------------------------------------------------------------
VOID GuildMember::RemoveMember(DWORD dwRoleID, BOOL bSave2DB/* = TRUE*/)
{
	tagGuildMember *pMember = m_mapMember.Peek(dwRoleID);
	if(!P_VALID(pMember))
	{
		return;
	}

	m_mapMember.Erase(dwRoleID);

	if(bSave2DB)
	{
		tagNDBC_LeaveGuild send;
		send.dwRoleID	= dwRoleID;

		g_dbSession.Send(&send, send.dwSize);
	}

	SAFE_DEL(pMember);

	// 给被删除的帮派成员同步在线帮派人数变化信息
	Role* pRemovedRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRemovedRole))
	{
		pRemovedRole->SyncOnlineNum2Role(ESOR_Online_GuildMember_Num, 0);
	}

	// 给所有帮派成员发送在线人数变化消息
	SyncOnlineNum2AllGuildMumber();

	m_MemberChangeFlag = TRUE;
}

//-------------------------------------------------------------------------------
// 设置成员的帮派职位
//-------------------------------------------------------------------------------
VOID GuildMember::SetGuildPos(tagGuildMember *pMember, EGuildMemberPos ePos, BOOL bSave2DB/* = TRUE*/)
{
	ASSERT(P_VALID(pMember));
	ASSERT(::IsGuildPosValid(ePos));

	pMember->eGuildPos = ePos;

	if(bSave2DB)
	{
		tagNDBC_ChangeGuildPos send;
		send.dwRoleID	= pMember->dwRoleID;
		send.n8NewPos	= ePos;

		g_dbSession.Send(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------
// 帮派内广播
//-------------------------------------------------------------------------------
VOID GuildMember::SendGuildMsg(LPVOID pMsg, DWORD dwSize)
{
	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		Role *pRole = g_roleMgr.GetRolePtrByID(pMember->dwRoleID);
		if(!P_VALID(pRole))
		{
			continue;
		}

		pRole->SendMessage(pMsg, dwSize);
	}
}

//-------------------------------------------------------------------------------
// 帮派内及其9空格广播
//-------------------------------------------------------------------------------
VOID   GuildMember::SendChangeGuildNameMsg(LPVOID pMsg, DWORD dwSize)
{
	
    Guild* pGuild = g_guildMgr.GetGuild(m_dwGuildID);
	if( !P_VALID(pGuild) )
	{
		return ;
	}
	INT32 nNumTCHAR = pGuild->GetGuildAtt().strName.size();
	ASSERT(nNumTCHAR > 0);

	INT32 nMsgSz = sizeof(tagNS_GetGuildName) + nNumTCHAR * sizeof(TCHAR);

	MCREATE_MSG(pSend, nMsgSz, NS_GetGuildName);
	pSend->dwGuildID	= pGuild->GetGuildAtt().dwID;
	_tcscpy(pSend->szGuildName,pGuild->GetGuildAtt().strName.c_str());
	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		Role *pRole = g_roleMgr.GetRolePtrByID(pMember->dwRoleID);
		if(!P_VALID(pRole))
		{
			continue;
		}
		else
		{
			Map* pMap = pRole->GetMap();
			if ( P_VALID(pMap) )
			{
				g_roleMgr.SendWorldMsg(pSend,pSend->dwSize);
			}
		}
	}
	MDEL_MSG(pSend);
}

//-------------------------------------------------------------------------------
// 发送所有帮派成员信息
//-------------------------------------------------------------------------------
VOID GuildMember::SendAllMembers2Client(Role *p2Role)
{
	INT16 n16Num = m_mapMember.Size() - 1;	// 减去自己
	ASSERT(n16Num > 0);

	// 申请内存
	INT32 nMsgSz = sizeof(tagNS_GuildGetAllMembers) - sizeof(BYTE) + n16Num * sizeof(tagGuildMemInfo);
	MCREATE_MSG(pSend, nMsgSz, NS_GuildGetAllMembers);

	pSend->n16Num = 0;
	MTRANS_POINTER(pMemberInfo, pSend->byData, tagGuildMemInfo);
	
	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		if(pMember->dwRoleID == p2Role->GetID())
		{
			continue;
		}

		pMemberInfo[pSend->n16Num].dwRoleID		= pMember->dwRoleID;
		pMemberInfo[pSend->n16Num].n8GuildPos	= pMember->eGuildPos;
		pMemberInfo[pSend->n16Num].nCurContrib	= pMember->nContribution;
		
		tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(pMember->dwRoleID);
		if(P_VALID(pRoleInfo))
		{
			pMemberInfo[pSend->n16Num].byLevel			= pRoleInfo->byLevel;
			pMemberInfo[pSend->n16Num].bySex			= pRoleInfo->bySex;
			pMemberInfo[pSend->n16Num].byClass			= pRoleInfo->byClass;		//
			pMemberInfo[pSend->n16Num].dwTimeLastLogout	= pRoleInfo->dwTimeLastLogout;
			pMemberInfo[pSend->n16Num].bOnline			= pRoleInfo->bOnline;
		}
		
		// 帮派逻辑成员管理逻辑有问题
		ASSERT(P_VALID(pRoleInfo));

		++pSend->n16Num;
	}

	if(n16Num != pSend->n16Num)
	{
		ASSERT(n16Num == pSend->n16Num);
		IMSG(_T("\n\n\n\tCaution:\n\tGuild<id:%u> members maybe have problem, please check db!\n\n"), p2Role->GetGuildID());
	}

	p2Role->SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);
}

//-------------------------------------------------------------------------------------------------------
// 设置帮派仓库操作权限
//-------------------------------------------------------------------------------------------------------
VOID GuildMember::SetGuildWarePrivilege( DWORD dwRoleID, BOOL bCanUse, BOOL bSave2DB /*= TRUE*/ )
{
	if (!GT_VALID(dwRoleID))
		return;

	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;

	if (bSave2DB && (pMember->bUseGuildWare != bCanUse))
	{
		pMember->bUseGuildWare = bCanUse;

		// 通知数据库
		tagNDBC_GuildWarePri send;
		send.dwRoleID	= dwRoleID;
		send.bEnable	= bCanUse;

		g_dbSession.Send(&send, send.dwSize);
	}
}

//-------------------------------------------------------------------------------------------------------
// 帮派贡献
//-------------------------------------------------------------------------------------------------------
VOID GuildMember::IncMemberContribution( DWORD dwRoleID, INT32 nContribution, BOOL bSave2DB /*= TRUE*/ )
{
	if (!GT_VALID(dwRoleID))
		return;

	if (nContribution <= 0)
		return;

	// 取得成员属性
	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;

	// 属性变化
	pMember->nContribution += nContribution;
	if (pMember->nContribution > MAX_GUILDMEM_CURCONTRIB)
	{
		pMember->nContribution = MAX_GUILDMEM_CURCONTRIB;
	}
	pMember->nTotalContribution += nContribution;
	if (pMember->nTotalContribution > MAX_GUILDMEM_TOTALCONTRIB)
	{
		pMember->nTotalContribution = MAX_GUILDMEM_TOTALCONTRIB;
	}
	
	// 保存到数据库
	if (bSave2DB)
	{
		tagNDBC_ChangeContrib send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;

		g_dbSession.Send(&send, send.dwSize);
	}

	// 同步到客户端
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		tagNS_GuildContribution send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;
		pRole->SendMessage(&send, send.dwSize);
	}
}

VOID GuildMember::DecMemberContribution( DWORD dwRoleID, INT32 nContribution, BOOL bSave2DB /*= TRUE*/ )
{
	if (!GT_VALID(dwRoleID))
		return;

	if (nContribution <= 0)
		return;

	// 取得成员属性
	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;

	// 属性变化
	pMember->nContribution -= nContribution;
	if (pMember->nContribution < 0)
	{
		pMember->nContribution = 0;
	}

	// 保存到数据库
	if (bSave2DB)
	{
		tagNDBC_ChangeContrib send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;

		g_dbSession.Send(&send, send.dwSize);
	}

	// 同步到客户端
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		tagNS_GuildContribution send;
		send.dwRoleID			= dwRoleID;
		send.nContribution		= pMember->nContribution;
		send.nTotalContribution	= pMember->nTotalContribution;
		pRole->SendMessage(&send, send.dwSize);
	}
}

VOID GuildMember::SetMemberExploit(DWORD dwRoleID, INT32 nExploit, BOOL bSave2DB /* = TRUE */)
{
	if (!GT_VALID(dwRoleID))
		return;

	tagGuildMember* pMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pMember))
		return;
	
	pMember->nExploit += nExploit;
		
	if (pMember->nExploit > MAX_GUILDMEM_EXPLOIT)
		pMember->nExploit = MAX_GUILDMEM_EXPLOIT;
	if (pMember->nExploit < 0)
		pMember->nExploit = 0;

	if (bSave2DB)
	{
		// 通知数据库
		tagNDBC_ChangeExploit send;
		send.dwRoleID	= dwRoleID;
		send.nExploit	= pMember->nExploit;

		g_dbSession.Send(&send, send.dwSize);
	}

	// 同步到客户端
	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if (P_VALID(pRole))
	{
		tagNS_GuildExploit send;
		send.dwRoleID			= dwRoleID;
		send.nExploit		= pMember->nExploit;
		pRole->SendMessage(&send, send.dwSize);
		// Jason 2010-3-4 v1.4.0帮派功勋称号
		pRole->GetTitleMgr()->SigEvent( ETE_GUILD_EXPLOIT, GT_INVALID ,pMember->nExploit );
	}

}

//-------------------------------------------------------------------------------------------------------
// 给所有帮派成员添加BUFF
//-------------------------------------------------------------------------------------------------------
VOID GuildMember::AddBuffToAllOnLineMember(DWORD dwBuffID)
{
	const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(dwBuffID);
	if ( !P_VALID(pBuffProto)) return ;

	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		Role* pRole = NULL;
		pRole = g_roleMgr.GetRolePtrByID(pMember->dwRoleID);
		if ( NULL != pRole)
			pRole->TryAddBuff((Unit*)pRole, pBuffProto, NULL, NULL, NULL);
	}
}

//-------------------------------------------------------------------------------------------------------
// 移除所有帮派成员的BUFFF
//-------------------------------------------------------------------------------------------------------
VOID GuildMember::RemoveBuffFromAllOnLineMember(DWORD dwBuffID)
{
	const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(dwBuffID);
	if ( !P_VALID(pBuffProto)) return ;

	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		Role* pRole = NULL;
		pRole = g_roleMgr.GetRolePtrByID(pMember->dwRoleID);
		if ( NULL != pRole)
			pRole->RemoveBuff(Buff::GetIDFromTypeID(dwBuffID), TRUE);
	}
}

/// 重置所有公会成员的State
VOID GuildMember::UnsetStateOfAllGuildMember(EState eState)
{
	tagGuildMember* pMember = NULL;
	MapGuildMember::TMapIterator iter = m_mapMember.Begin();
	while(m_mapMember.PeekNext(iter, pMember))
	{
		Role* pRole = NULL;
		pRole = g_roleMgr.GetRolePtrByID(pMember->dwRoleID);
		if ( NULL != pRole)
			pRole->UnsetState(eState);
	}
}

//-------------------------------------------------------------------------------------------------------
// 得到帮主
//-------------------------------------------------------------------------------------------------------
tagGuildMember* GuildMember::GetMaster()
{
	tagGuildMember* pGuildMember = NULL;
	TMap<DWORD, tagGuildMember*>::TMapIterator itMember = m_mapMember.Begin();
	while(m_mapMember.PeekNext(itMember, pGuildMember))
	{
		if ( EGMP_HuiZhang == pGuildMember->eGuildPos )
			return pGuildMember;
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------------------
// 强制帮派所有成员检查一次脚本区域
//-------------------------------------------------------------------------------------------------------
VOID   GuildMember::CheckAllGuildMemberIsInGodArea()
{
	tagGuildMember* pGuildMember = NULL;
	TMap<DWORD, tagGuildMember*>::TMapIterator itMember = m_mapMember.Begin();
	while(m_mapMember.PeekNext(itMember, pGuildMember))
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(pGuildMember->dwRoleID);
		if ( P_VALID(pRole))
		{
			pRole->GetMap()->CheckArea(pRole);
		}
	}
}

INT32 GuildMember::GetOnlineGuildMemberCnt()
{
	tagGuildMember* pGuildMember = NULL;
	UINT16 nOnlineNum = 0;

	MapGuildMember::TMapIterator& iterator = m_mapMember.Begin();
	while (m_mapMember.PeekNext(iterator, pGuildMember))
	{
		if (!P_VALID(pGuildMember))
		{
			continue;
		}
		Role* pRole = g_roleMgr.GetRolePtrByID(pGuildMember->dwRoleID);
		if (P_VALID(pRole))
		{
			nOnlineNum++;
		}
	}
	return nOnlineNum;
}

// 给所有帮派成员发送在线人数变化消息
VOID GuildMember::SyncOnlineNum2AllGuildMumber(BOOL bLogout)
{
	tagGuildMember* pGuildMember = NULL;
	UINT16 nOnlineNum = GetOnlineGuildMemberCnt();
	MapGuildMember::TMapIterator& iterator = m_mapMember.Begin();

	while (m_mapMember.PeekNext(iterator, pGuildMember))
	{
		if (!P_VALID(pGuildMember))
		{
			continue;
		}
		Role* pRole = g_roleMgr.GetRolePtrByID(pGuildMember->dwRoleID);
		if (P_VALID(pRole))
		{
			// 同帮派在线人数发生变化，通知客户端
			pRole->SyncOnlineNum2Role(ESOR_Online_GuildMember_Num, (bLogout?(nOnlineNum-1):nOnlineNum));
		}
	}
}
