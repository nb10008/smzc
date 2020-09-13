//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: guild_handler.h
// author: 
// actor:
// data: 2009-04-17
// last:
// brief: 帮派消息处理
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "player_session.h"
#include "role.h"
#include "role_mgr.h"
#include "guild.h"
#include "guild_mgr.h"
#include "guild_CofC.h"
#include "../WorldDefine/msg_guild.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_guild.h"
#include "../ServerDefine/login_issue.h"
#include "login_session.h"
// Jason 2010-02-20 v1.3.2 申请入帮派相关
#include "guild_apply_to_join.h"
#include "city_mgr.h"
#include "att_res.h"
#include "GuildBattle.h"
#include "../WorldDefine/filter.h"
#include "guild_war_declare_mgr.h"
#include "../WorldDefine/msg_guild_war_declare.h"
#include "MsgInfoMgr.h"
#include "../WorldDefine/msg_GuildBattle.h"
#include "creature.h"
#include "item_creator.h"

//-----------------------------------------------------------------------------
// 创建帮派
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleCreateGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildCreate);
    
	
	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 创建帮派前，先判断角色身上是否有“帮派决裂”的buff(bufferID：LEAVE_GUILD_BUFF_ID)
	if (pRole->IsHaveBuff(LEAVE_GUILD_BUFF_TYPEID))
	{
		tagNS_RoleInJustLeaveGuildState msg;
		msg.dwErrorCode = E_Guild_Can_Not_Join_Or_Create;
		pRole->SendMessage(&msg, msg.dwSize);
		return GT_INVALID;
	}

	DWORD dwErrorCode = GT_INVALID;

	INT32 nNameTCHAR = (pRecv->dwSize - FIELD_OFFSET(tagNC_GuildCreate, szGuildName)) / sizeof(TCHAR);
	// 获取名字 -- 确保有字符串结束符
	tstring szTmpGuildName(pRecv->szGuildName, nNameTCHAR);

	// 帮派名合法性判断
	dwErrorCode = Filter::CheckName(szTmpGuildName.c_str(), g_attRes.GetVariableLen().nGuildNameMax, 
		g_attRes.GetVariableLen().nGuildNameMin, g_attRes.GetNameFilterWords());
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		return dwErrorCode;
	}

	// 是否与已建帮派重名
	DWORD dwNewGuildID = g_world.LowerCrc32(szTmpGuildName.c_str());
	if(g_guildMgr.GetMapGuild().IsExist(dwNewGuildID))
	{
		dwErrorCode = E_Guild_Create_NameExist;
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		return dwErrorCode;
	}

	// 等级是否达到20级
	if(pRole->GetLevel() < g_guildMgr.GetGuildCfg().nCreateRoleMinLevel)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, E_Guild_Create_LevelLimit);
		return dwErrorCode;
	}

	// 现金是否足够(10金)
	INT64 n64Cost = MGold2Silver(g_guildMgr.GetGuildCfg().nGoldCreateNeeded);
	if(pRole->GetCurMgr().GetBagSilver() < n64Cost)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, E_BagSilver_NotEnough);
		return E_BagSilver_NotEnough;
	}
	// NPC检查
	dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Guild);

	if (dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		return dwErrorCode;
	}

	tagNWL_CreateGuild msg;
	memset(msg.szGuildName,0,sizeof(msg.szGuildName));
	msg.dwAccountID = m_dwAccountID;
	msg.dwNPCID  = pRecv->dwNPCID;
	msg.dwRoleID = pRole->GetID();
	_tcscpy_s(msg.szGuildName,X_SHORT_NAME,szTmpGuildName.c_str());
	g_loginSession.Send(&msg,msg.dwSize);
	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 解散帮派
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDismissGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildDismiss);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = GT_INVALID;
	DWORD dwGuildID = pRole->GetGuildID();

	// NPC检查
	dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_Guild);
	if(E_Success == dwErrorCode)
	{
		// 帮派状态判断
		Guild *pGuild = g_guildMgr.GetGuild(dwGuildID);
		if( !P_VALID(pGuild) )
		{
			return GT_INVALID;
		}
		else if( pGuild->IsInGuildState(EGDSS_Warfare) )
		{
			dwErrorCode = E_Guild_State_Limit;
		}

		if (P_VALID(CGodMiracleMgr::GetInst()))
		{
			dwErrorCode = CGodMiracleMgr::GetInst()->CanGuildDismiss(dwGuildID);
		}

		if (E_Success == dwErrorCode)
		{
			dwErrorCode = g_guildMgr.DismissGuild(pRole, dwGuildID);
			if( E_Success == dwErrorCode )
			{
				ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
				if( P_VALID(pToolkit) && P_VALID(pRole) )
				{
					pToolkit->DelGuild(dwGuildID);
				}
			}
		}
	}

	// 向客户端反馈
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 邀请加入
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleJoinGuildReq(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildJoinReq);
	
	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 邀请前，先判断角色身上是否有“帮派决裂”的buff(bufferID：LEAVE_GUILD_BUFF_ID)
	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwDstRoleID);
	if (!P_VALID(pDestRole))
	{
		return GT_INVALID;
	}
	if (pDestRole->IsHaveBuff(LEAVE_GUILD_BUFF_TYPEID))
	{
		tagNS_RoleInJustLeaveGuildState msg;
		msg.dwErrorCode = E_Guild_Can_Not_Be_Invited;
		pRole->SendMessage(&msg, msg.dwSize);
		return GT_INVALID;
	}

	// 是否对自己操作
	if(pRole->GetID() == pRecv->dwDstRoleID)
	{
		return GT_INVALID;
	}

	// 得到帮派
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// 帮派状态判断
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// 处理
	Role *pInvitee = NULL;
	DWORD dwErrorCode = pGuild->CanInviteJoin(pRole->GetID(), pRecv->dwDstRoleID, &pInvitee);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		ASSERT(P_VALID(pInvitee));

		tagNS_GuildJoinReq send;
		send.dwSrcRoleID	= pRole->GetID();
		send.dwGuildID		= pRole->GetGuildID();
		pInvitee->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 被邀请玩家反馈
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleJoinGuildReqRes(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildJoinReqRes);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 得到帮派
	Guild *pGuild = g_guildMgr.GetGuild(pRecv->dwDstGuildID);
	if(!P_VALID(pGuild))
	{ 
		return GT_INVALID;
	}

	if(pRecv->dwErrorCode != E_Success)
	{
		// 清除该邀请帖信息
		pGuild->EraseInviteJoin(pRole->GetID());	

		// 检查邀请者是否仍在线
		Role *pInviter = g_roleMgr.GetRolePtrByID(pRecv->dwDstRoleID);
		if(P_VALID(pInviter))
		{
			// 向邀请者发消息
			tagNS_GuildJoinReqRes	send;
			send.dwErrorCode = E_Guild_Join_BeRefused;
			send.dwInviterID = pRecv->dwDstRoleID;
			send.dwInviteeID = pRole->GetID();
			pInviter->SendMessage(&send, send.dwSize);
		}

		return E_Success;
	}

	// 处理
	DWORD dwErrorCode = pGuild->InviteJoin(pRecv->dwDstRoleID, pRole->GetID());
	if(dwErrorCode != E_Success)
	{		
		// 向双方发消息
		tagNS_GuildJoinReqRes	send;
		send.dwErrorCode = dwErrorCode;
		send.dwInviterID = pRecv->dwDstRoleID;
		send.dwInviteeID = pRole->GetID();
		SendMessage(&send, send.dwSize);

		// 检查邀请者是否仍在线
		Role *pInviter = g_roleMgr.GetRolePtrByID(pRecv->dwDstRoleID);
		if(P_VALID(pInviter))
		{
			pInviter->SendMessage(&send, send.dwSize);
		}
	}
	else
	{
		// 角色加入帮派，检查是否有公会宣战关系
		g_guildWarDeclareMgr.OnRoleJoinGuild(pRole);

		// 帮派广播
		tagRoleInfo *pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());
		if (P_VALID(pRoleInfo))
		{
			INT32 nRoleNameCnt = _tcsclen(pRoleInfo->szRoleName);
			INT32 nMsgSz = sizeof(tagNS_GuildJoinBroad) + nRoleNameCnt * sizeof(TCHAR);

			MCREATE_MSG(pSend, nMsgSz, NS_GuildJoinBroad);
			pSend->dwRoleID		= pRole->GetID();
			pSend->dwRoleNameID	= pRole->GetNameID();

			memcpy(pSend->szRoleName, pRoleInfo->szRoleName, (nRoleNameCnt + 1) * sizeof(TCHAR));

			pGuild->SendGuildMsg(pSend, pSend->dwSize);
			MDEL_MSG(pSend);
		}

		// 同步到周围玩家
		tagGuildMember* pMember = pGuild->GetMember(pRole->GetID());
		Map*			pMap	= pRole->GetMap();
		if (P_VALID(pMember) && P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send;
			send.dwGuildID		= pRecv->dwDstGuildID;
			send.dwRoleID		= pRole->GetID();
			send.n8GuildPos		= pMember->eGuildPos;

			pMap->SendBigVisTileMsg(pRole, &send, send.dwSize);
		}

		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) && P_VALID(pRole) )
		{
			pToolkit->ChangeGuildMember(pRecv->dwDstGuildID,pRole->GetID());
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 离开帮派
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleLeaveGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildLeave);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 得到帮派
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// 检查帮派状态限制
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// 处理
	DWORD dwErrorCode = pGuild->LeaveGuild(pRole);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// 帮派广播
		tagNS_GuildLeaveBroad send;
		send.dwSrcRoleID = pRole->GetID();
		pGuild->SendGuildMsg(&send, send.dwSize);

		// 离开者
		SendMessage(&send, send.dwSize);

		// 角色离开帮派，检查是否有公会宣战关系
		g_guildWarDeclareMgr.OnRoleLeaveGuild(pRole);

		// 同步到周围玩家
		Map* pMap	= pRole->GetMap();
		if (P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= GT_INVALID;
			send_broad.dwRoleID		= pRole->GetID();
			send_broad.n8GuildPos	= EGMP_Null;

			pMap->SendBigVisTileMsg(pRole, &send_broad, send_broad.dwSize);
		}

		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) && P_VALID(pRole) )
		{
			pToolkit->ChangeGuildMember(pGuild->GetGuildAtt().dwID,pRole->GetID());
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 从帮派中踢出
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleKickFromGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildKick);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 是否对自己操作
	if(pRole->GetID() == pRecv->dwRoleID)
	{
		return GT_INVALID;
	}

	// 得到帮派
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// 检查帮派状态限制
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// 处理
	DWORD dwErrorCode = pGuild->KickMember(pRole->GetID(), pRecv->dwRoleID);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildKickBroad send;
		send.dwDstRoleID = pRecv->dwRoleID;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// 检查被踢者是否在线
		Role *pKicked = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if(P_VALID(pKicked))
		{
			pKicked->SendMessage(&send, send.dwSize);

			// 同步到周围玩家
			Map* pMap	= pKicked->GetMap();
			if (P_VALID(pMap))
			{
				tagNS_RemoteRoleGuildInfoChange send_broad;
				send_broad.dwGuildID	= GT_INVALID;
				send_broad.dwRoleID		= pKicked->GetID();
				send_broad.n8GuildPos	= EGMP_Null;

				pMap->SendBigVisTileMsg(pKicked, &send_broad, send_broad.dwSize);
			}

			// 角色离开帮派，检查是否有公会宣战关系
			g_guildWarDeclareMgr.OnRoleLeaveGuild(pKicked);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 移交帮主
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleTurnoverGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildTurnover);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 是否对自己操作
	if(pRole->GetID() == pRecv->dwRoleID)
	{
		return GT_INVALID;
	}

	// 得到帮派
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// 检查帮派状态限制
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// 处理
	DWORD dwErrorCode = pGuild->TurnoverLeader(pRole->GetID(), pRecv->dwRoleID);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildTurnoverBroad send;
		send.dwSrcRoleID	= pRole->GetID();
		send.dwDstRoleID	= pRecv->dwRoleID;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// 同步到周围玩家(原帮主和先帮主)
		tagGuildMember* pMember = pGuild->GetMember(pRole->GetID());
		Map*			pMap	= pRole->GetMap();
		if (P_VALID(pMember) && P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= pRole->GetGuildID();
			send_broad.dwRoleID		= pRole->GetID();
			send_broad.n8GuildPos	= pMember->eGuildPos;

			pMap->SendBigVisTileMsg(pRole, &send_broad, send_broad.dwSize);
		}

		// 判断先帮主是否在线
		Role* pNewLeader = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if (P_VALID(pNewLeader))
		{
			pMember = pGuild->GetMember(pNewLeader->GetID());
			pMap	= pNewLeader->GetMap();
			if (P_VALID(pMember) && P_VALID(pMap))
			{
				tagNS_RemoteRoleGuildInfoChange send_broad;
				send_broad.dwGuildID	= pNewLeader->GetGuildID();
				send_broad.dwRoleID		= pNewLeader->GetID();
				send_broad.n8GuildPos	= pMember->eGuildPos;

				pMap->SendBigVisTileMsg(pNewLeader, &send_broad, send_broad.dwSize);
			}
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 辞职
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDemissFromGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildDemiss);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 得到帮派
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// 检查帮派状态限制
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// 处理
	INT8 n8OldGuildPos = EGMP_HuiYuan;
	DWORD dwErrorCode = pGuild->DemissPostion(pRole->GetID(), n8OldGuildPos);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildDemissBroad send;
		send.dwRoleID		= pRole->GetID();
		send.n8OldGuildPos	= n8OldGuildPos;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// 同步到周围玩家
		Map* pMap	= pRole->GetMap();
		if (P_VALID(pMap))
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= pRole->GetGuildID();
			send_broad.dwRoleID		= pRole->GetID();
			send_broad.n8GuildPos	= EGMP_HuiYuan;

			pMap->SendBigVisTileMsg(pRole, &send_broad, send_broad.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 任命职位
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleAppointForGuild(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildAppoint);

	// 获取角色
	Role *pAppointor = GetRole();
	if(!P_VALID(pAppointor))
	{
		return GT_INVALID;
	}

	// 是否对自己操作
	if(pAppointor->GetID() == pRecv->dwRoleID)
	{
		return GT_INVALID;
	}

	// 得到帮派
	Guild *pGuild = g_guildMgr.GetGuild(pAppointor->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}
	
	// 检查帮派状态限制
	if( pGuild->IsInGuildState(EGDSS_Warfare) )
	{
		return E_Guild_State_Limit;
	}

	// 处理
	DWORD dwErrorCode = pGuild->Appoint(pAppointor->GetID(), pRecv->dwRoleID, pRecv->ePos);
	if(dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_GuildAppointBroad send;
		send.dwSrcRoleID	= pAppointor->GetID();
		send.dwDstRoleID	= pRecv->dwRoleID;
		send.ePos			= pRecv->ePos;
		pGuild->SendGuildMsg(&send, send.dwSize);

		// 同步到周围玩家
		Role* pDstRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if( P_VALID(pDstRole) && P_VALID(pDstRole->GetMap()) )
		{
			tagNS_RemoteRoleGuildInfoChange send_broad;
			send_broad.dwGuildID	= pDstRole->GetGuildID();
			send_broad.dwRoleID		= pRecv->dwRoleID;
			send_broad.n8GuildPos	= pRecv->ePos;

			pDstRole->GetMap()->SendBigVisTileMsg(pDstRole, &send_broad, send_broad.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 修改帮派宗旨
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleChangeGuildTenet(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildTenetChange);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 得到帮派
	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// 处理
	INT32 nTenetCnt = (pRecv->dwSize - FIELD_OFFSET(tagNC_GuildTenetChange, szTenet)) / sizeof(TCHAR);
	DWORD dwErrorCode = pGuild->ModifyTenet(pRole->GetID(), pRecv->szTenet, nTenetCnt);
	if(dwErrorCode != E_Success)
	{
		// 判断是否需要返回
		if (dwErrorCode != E_Return_NotNeedFeed)
		{
			g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		}
	}
	else
	{
		INT32 nMsgSz = sizeof(tagNS_GuildTenetChangeBroad) + nTenetCnt * sizeof(TCHAR);
		MCREATE_MSG(pSend, nMsgSz, NS_GuildTenetChangeBroad);
		pSend->dwRoleID	= pRole->GetID();
		memcpy(pSend->szTenet, pRecv->szTenet, (nTenetCnt + 1) * sizeof(TCHAR));
		pSend->szTenet[nTenetCnt] = _T('\0');
		pGuild->SendGuildMsg(pSend, pSend->dwSize);
		MDEL_MSG(pSend);
	}

	return dwErrorCode;
}


//********************** 获取帮派相关信息消息处理 **********************//

//-----------------------------------------------------------------------------
// 帮派修改名字
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGuildChangeName(tagNetCmd* pCmd)
{ 
	MGET_MSG(pRecv, pCmd, NC_GuildChangeName);
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
    DWORD dwError = E_Success ;
	INT32 nNameTCHAR = (pRecv->dwSize - FIELD_OFFSET(tagNC_GuildChangeName, szGuildName)) / sizeof(TCHAR);
	tstring szTmpGuildName(pRecv->szGuildName, nNameTCHAR);

	// 帮派名合法性判断
	dwError = Filter::CheckName(szTmpGuildName.c_str(), g_attRes.GetVariableLen().nGuildNameMax, 
		g_attRes.GetVariableLen().nGuildNameMin, g_attRes.GetNameFilterWords());
	if(dwError != E_Success)
	{
		tagNS_GuildChangeName send;
		send.dwErrorCode = E_GuildChangeName_NameInvalid ;
		pRole->SendMessage(&send,send.dwSize);
		return 0 ;

	}


	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if ( !P_VALID(pGuild))
	{
        return GT_INVALID;
	}
    

	tagGuildMember* pGuildMember = pGuild->GetMember(pRole->GetID());
	if ( P_VALID(pGuildMember) )
	{
		if ( pGuildMember->eGuildPos !=  EGMP_HuiZhang)
		{
             tagNS_GuildChangeName send;
			 send.dwErrorCode = E_GuildChangeName_NotBangZhu ;
			 pRole->SendMessage(&send,send.dwSize);
			 return 0 ;
		}
	}

	if ( pGuild->GetGuildState() ==  EGDSS_Warfare)
	{
		tagNS_GuildChangeName send;
		send.dwErrorCode = E_GuildChangeName_Warfare ;
		pRole->SendMessage(&send,send.dwSize);
		return 0 ;
	}

	//判断本帮派是否处在城战报名状态
	DWORD dwGuildId = pGuild->GetGuildAtt().dwID  ;
	if(g_cityMgr.IsAlreadySignup(dwGuildId,ECFT_Attack)
		||g_cityMgr.IsAlreadySignup(dwGuildId,ECFT_Defence)
		||g_cityMgr.IsAlreadySignup(dwGuildId,ECFT_Request))
	{
	
		tagNS_GuildChangeName send;
		send.dwErrorCode = E_GuildChangeName_AlreadySigup ;
		pRole->SendMessage(&send,send.dwSize);
		return 0 ;
	}

	//判断本帮派是否处在神迹争夺报名状态
	CGodMiracle* pGodMiracle = CGodMiracleMgr::GetInst()->IsInCaptureGodRange(dwGuildId);
	if ( P_VALID(pGodMiracle) )
	{
		tagNS_GuildChangeName send;
		send.dwErrorCode = E_GuildChangeName_IsInGodRange ;
		pRole->SendMessage(&send,send.dwSize);
		return 0 ;
	}
    
	// 帮派是否重名
	DWORD dwNewGuildID = g_world.LowerCrc32(szTmpGuildName.c_str());
    if( P_VALID(g_guildMgr.GetGuild(dwNewGuildID)))
	{
		tagNS_GuildChangeName send;
		send.dwErrorCode = E_GuildChangeName_Alreadyexist ;
		pRole->SendMessage(&send,send.dwSize);
		return 0 ;
	}
	
	
	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64Serial);
    if ( !P_VALID(pItem))
    {
		return GT_INVALID;
    }
	else
	{
		dwError = pRole->GetItemMgr().DelFromBag(pRecv->n64Serial,ELCID_Guild_ChangeName,1);
		if(dwError == E_Success)
		{
             pGuild->ChangeName(szTmpGuildName.c_str()); 
			 INT nMsgSz = sizeof(tagNS_GuildChangeName) + ((pGuild->GetGuildAtt().strName.size()) * sizeof(TCHAR));
			 MCREATE_MSG(pSend, nMsgSz, NS_GuildChangeName);
			 pSend->dwErrorCode = E_Success ;
			 memcpy(pSend->szGuildName,pGuild->GetGuildAtt().strName.c_str(),pGuild->GetGuildAtt().strName.size()*sizeof(TCHAR)) ;
			 pGuild->SendGuildMsg(pSend,pSend->dwSize);
             
			 //向每个帮派成员周围9空格广播名字变化
			 pGuild->SendChangeGuildNameMsg(NULL,0);
             
			

			  MDEL_MSG(pSend);
			 return 0 ; 
		}
		
		
	}
   return E_Success;





}


//-----------------------------------------------------------------------------
// 获取帮派所有成员
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildMembers(tagNetCmd* pCmd)
{
	//MGET_MSG(p, pCmd, NC_GuildGetAllMembers);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if(!pRole->IsInGuild())
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		// 执行到此处，说明帮派管理有问题 -- 帮派里没有成员时才可以删除帮派
		ASSERT(0);
		return GT_INVALID;
	}

	pGuild->SendAllMembers2Client(pRole);
	//// Jason 2010-3-9 v1.4.0 帮派功勋通知消息
	//tagNS_GuildExploit msg_guild_exploit;
	//msg_guild_exploit.dwRoleID = pRole->GetID();
	//msg_guild_exploit.nExploit = 0;

	//tagGuildMember* pMember = pGuild->GetMember(pRole->GetID());
	//if( P_VALID(pMember) )
	//{
	//	msg_guild_exploit.nExploit = pMember->nExploit;
	//}

	//pRole->SendMessage(&msg_guild_exploit,msg_guild_exploit.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 获取帮派指定成员扩展信息
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildMemberEx(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_GuildGetMemberEx);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if(!pRole->IsInGuild())
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		// 执行到此处，说明帮派管理有问题 -- 帮派里没有成员时才可以删除帮派
		ASSERT(0);
		return GT_INVALID;
	}

	return pGuild->SendSpecMemberEx2Client(pRole, p->dwRoleID);
}

//-----------------------------------------------------------------------------
// 刷新帮派指定成员信息
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRefreshGuildMember(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_GuildRefMember);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if(!pRole->IsInGuild())
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		// 执行到此处，说明帮派管理有问题 -- 帮派里没有成员时才可以删除帮派
		ASSERT(0);
		return GT_INVALID;
	}

	return pGuild->SendSpecMember2Client(pRole, p->dwRoleID);
}

//-----------------------------------------------------------------------------
// 获取帮派名称
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildName(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildName);

	Guild *pGuild = g_guildMgr.GetGuild(pRecv->dwGuildID);
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	return pGuild->SendGuildName2Client(this);
}
//-----------------------------------------------------------------------------
// 批量获取帮派名称
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetSomeGuildName(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetSomeGuildName);

	INT		nNum = pRecv->nNum;

	if(nNum <= 0)
		return 0;

	if(nNum > 50)
		return 0;

	DWORD	dwSize = sizeof(tagNS_GetSomeGuildName) + (nNum - 1) * sizeof(tagGuildIDName);
	MCREATE_MSG(pSend, dwSize, NS_GetSomeGuildName);

	pSend->nNum = nNum; 
	pSend->nUserData = pRecv->nUserData;

	for(INT n = 0; n < nNum; ++n)
	{
		pSend->IdName[n].dwID = pRecv->dwAllID[n];
		Guild *pGuild = g_guildMgr.GetGuild(pRecv->dwAllID[n]);
		if(P_VALID(pGuild))
		{
			pRecv->dwAllID[n] = pGuild->GetGuildAtt().dwID;
			_tcscpy(pSend->IdName[n].szName, pGuild->GetGuildAtt().strName.c_str());	
		}
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return 0;
}
//-----------------------------------------------------------------------------
// 获取帮派宗旨
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildTenet(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildTenet);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	return pGuild->SendGuildTenet2Client(this);
}

//-----------------------------------------------------------------------------
// 获取帮派仓库中物品信息
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildWareItems(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildWare);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// NPC检查
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
	if(E_Success != dwErrorCode)
	{
		return GT_INVALID;
	}

	// 创建足够大的缓冲区
	MCREATE_MSG(pSend, TEMP_GUILD_BUFF_SIZE, NS_GetGuildWare);
	pSend->dwLastUpdateTime = pRecv->dwLastUpdateTime;

	INT32 nItemSize = 0;
	pSend->dwErrorCode = pGuild->GetGuildWarehouse().GetGuildWareInfo(pSend->byData, pSend->nItemNum, pSend->dwLastUpdateTime, pSend->nSzGuildWare, nItemSize);

	// 重新设置消息大小
	pSend->dwSize = sizeof(tagNS_GetGuildWare) + nItemSize;

	// 发送给客户端
	SendMessage(pSend, pSend->dwSize);

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 获取帮派仓库操作权限列表
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildWarePriList(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildWarePriList);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// NPC检查
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->GetGuildWarehouse().SendGuildWarePriList2Client(pRole);
	}

	if (GT_VALID(dwErrorCode) && (E_Success != dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return GT_INVALID;
}

//-----------------------------------------------------------------------------
// 变更指定成员的帮派仓库操作权限
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGuildWarePrivilege(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildWarePri);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// NPC检查
	DWORD dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildWare);
	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->SetGuildWarePrivilege(pRole, pRecv->dwRoleID, pRecv->bCanUse);
	}
	
	if (!GT_VALID(dwErrorCode))
	{
		return GT_INVALID;
	}

	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// 通知操作者更新列表
		tagNS_GuildWarePri send;
		send.dwRoleID	= pRecv->dwRoleID;
		send.bCanUse	= pRecv->bCanUse;
		SendMessage(&send, send.dwSize);

		// 通知被操作者权限变更
		Role* pMember = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
		if (P_VALID(pMember))
		{
			pMember->SendMessage(&send, send.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 获取帮派设施升级信息
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildFacilitiesInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetGuildUpgradeInfo);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = E_Success;

	// NPC检查
	if (pRecv->dwNPCID != GT_INVALID)
	{
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildMgr);
	}

	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->SendFacilitiesInfo2Client(pRole);
	}

	if (GT_VALID(dwErrorCode) && (E_Success != dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 玩家上缴帮派设施升级所需物品
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleHandInItems(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_HandInItems);

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	// 检查帮派设施是否已经满级
	if (pGuild->CheckLevelFull(pRecv->eType) == TRUE)
	{
		return GT_INVALID;
	}

	// NPC检查
	DWORD dwErrorCode = E_Success;
	if (pRecv->dwNPCID != GT_INVALID)
	{
		dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_GuildMgr);
	}
	if(E_Success == dwErrorCode)
	{
		dwErrorCode = pGuild->GetGuildFacilities().HandInItems(pRole, pRecv->eType);
	}

	if (GT_VALID(dwErrorCode))
	{
		if (E_Success != dwErrorCode)
		{
			g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		}
		else
		{
			// 返回变化后的设施信息
			tagNS_UpdateFacilitiesInfo send;
			pGuild->GetGuildFacilities().GetGuildFacilitiesInfo(&send.sFacilitiesInfo, pRecv->eType);
			pRole->SendMessage(&send, send.dwSize);
		}
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 发布帮派事务
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSpreadGuildAffair(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SpreadGuildAffair);
	if (!GT_VALID(pRecv->dwBuffID))
	{
		return GT_INVALID;
	}

	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildAffair().SpreadGuildAffair(pRole, pRecv->dwBuffID);

	if (GT_VALID(dwErrorCode) && (dwErrorCode != E_Success))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 取得帮派技能信息
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetGuildSkillInfo(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild *pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if(!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	MCREATE_MSG(pSend, TEMP_GUILD_BUFF_SIZE, NS_GetGuildSkillInfo);
	DWORD dwErrorCode = pGuild->GetGuildSkill().GetGuildSkillInfo(pSend->dwCurSkillID, pSend->n16Progress,
		pSend->nSkillNum, pSend->dwSkillInfo);

	// 重新计算消息大小
	if (dwErrorCode == E_Success)
	{
		pSend->dwSize = sizeof(tagNS_GetGuildSkillInfo) + (pSend->nSkillNum - 1) * sizeof(tagGuildSkills);
		SendMessage(pSend, pSend->dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 上缴帮派技能典籍
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleUpgradeGuildSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GuildSkillUpgrade);
	if (!GT_VALID(pRecv->n64ItemSerial))
	{
		return GT_INVALID;
	}

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_GuildSkillUpgrade send;
	DWORD dwErrorCode = pGuild->GetGuildSkill().SkillUpgradeByHandIn(pRole, pRecv->n64ItemSerial, send.dwSkillID, send.n16Progress);
	if (E_Success == dwErrorCode)
	{
		SendMessage(&send, send.dwSize);
	}
	else if (GT_VALID(dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 学习帮派技能
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleLearnGuildSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_LearnGuildSkill);
	if (!GT_VALID(pRecv->dwSkillID))
	{
		return GT_INVALID;
	}

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_LearnGuildSkill send;
	BOOL bLearnError = FALSE;
	INT nLevel = 0;
	DWORD dwErrorCode = pGuild->GetGuildSkill().LearnGuildSkill(pRole, pRecv->dwSkillID, nLevel, bLearnError);

	if (GT_VALID(dwErrorCode))
	{
		if (bLearnError)
		{
			send.dwErrorCode	= dwErrorCode;
			send.dwSkillID		= pRecv->dwSkillID * 100 + nLevel;
			SendMessage(&send, send.dwSize);
		}
		else if (dwErrorCode != E_Success)
		{
			g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
		}
	}
	
	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 设置当前研究帮派技能
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSetResearchSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetResearchSkill);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_SetResearchSkill send;
	INT nLevel = 0;
	DWORD dwErrorCode = pGuild->GetGuildSkill().SetCurUpgradeGuildSkill(pRole->GetID(), pRecv->dwSkillID, nLevel, send.n16Progress);
	if (!GT_VALID(dwErrorCode))
	{
		return GT_INVALID;
	}

	if (dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		if (GT_VALID(pRecv->dwSkillID))
		{
			send.dwSkillID	= pRecv->dwSkillID * 100 + nLevel;
		}
		pGuild->SendGuildMsg(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 激活公会技能
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleActiveGuildSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ActiveGuildSkill);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagGuildMember* pMaster = pGuild->GetMemberMgr().GetMaster();
	if (!P_VALID(pMaster))
	{
		return GT_INVALID;
	}

	// 检查是不是会长
	tagGuildMember* pGuildMember = pGuild->GetMember(pRole->GetID());
	if (!P_VALID(pGuildMember))
	{
		return GT_INVALID;
	}
	if (pGuildMember->eGuildPos != EGMP_HuiZhang && pGuildMember->eGuildPos != EGMP_FuHuiZhang)
	{
		return GT_INVALID;
	}

	tagNS_ActiveGuildSkill send;
	INT nLevel = 0;
	DWORD dwErrorCode = pGuild->GetGuildSkill().ActiveGuildSkill(pRole->GetID(), pRecv->n64ItemID, pRole->GetGuildID(), pRecv->dwGuildSkillID);
	if (!GT_VALID(dwErrorCode))
	{
		return GT_INVALID;
	}

	if (dwErrorCode != E_Success)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		if (GT_VALID(pRecv->dwGuildSkillID))
		{
			send.dwGuildSkillID	= pRecv->dwGuildSkillID;
		}
		pGuild->SendGuildMsg(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 同步帮派基本信息
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSyncGuildInfo(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_SyncGuildInfo send;
	send.sGuildInfo = (tagGuildBase)pGuild->GetGuildAtt();

	SendMessage(&send, send.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 获取售卖商会信息	-- 地图线程
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCofCInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCofCInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	GuildCofC* pCofC = pMap->GetCofC(pRecv->dwNPCID);
	if (!P_VALID(pCofC))
	{
		// 不需要通知客户端
		return GT_INVALID;
	}

	return pCofC->SendCommerceGoodsInfo2Role(pRole);
}

//-----------------------------------------------------------------------------
// 关闭商会界面		-- 地图线程
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleCloseCofC(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_CloseCofC);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Map* pMap = pRole->GetMap();
	if (!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	GuildCofC* pCofC = pMap->GetCofC(pRecv->dwNPCID);
	if (!P_VALID(pCofC))
	{
		return GT_INVALID;
	}

	// 移出观察者列表
	pCofC->RemoveObserver(pRole->GetID());

	return E_Success;
}

//-----------------------------------------------------------------------------
// 购买商会商货		-- 地图线程
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleBuyCofCGoods(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BuyCofCGoods);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得帮派信息
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().BuyGoods(pRole, pRecv->dwNPCID, pRecv->dwGoodID, pRecv->byBuyNum);
	if ((E_Success != dwErrorCode) && GT_VALID(dwErrorCode))
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 出售商货给商会	-- 地图线程
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSellCofCGoods(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SellCofCGoods);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得帮派信息
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().SellGoods(pRole, pRecv->dwNPCID, pRecv->dwGoodID, pRecv->bySellNum);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 获取跑商容器信息	-- 地图线程
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCommodityInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GetCommodityInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得帮派信息
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	INT nMsgSize = sizeof(tagNS_GetCommodityInfo) + sizeof(tagCommerceGoodInfo) * (MAX_COMMODITY_CAPACITY - 1);
	MCREATE_MSG(pSend, nMsgSize, NS_GetCommodityInfo);
	// 读取商货信息
	DWORD dwErrorCode = pGuild->GetGuildCommerce().GetCommodityGoodInfo(pRole, pSend->sGoodInfo, pSend->nGoodNum, pSend->nCurTael, pSend->nLevel);

	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// 调整消息大小
		pSend->dwSize = sizeof(tagNS_GetCommodityInfo) + sizeof(tagCommerceGoodInfo) * (pSend->nGoodNum - 1);
		SendMessage(pSend, pSend->dwSize);
	}

	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 获取跑商初始信息	-- 地图线程
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetTaelInfo(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得帮派信息
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	tagNS_GetTaelInfo send;
	tagTaelInfo sTaelInfo;
	DWORD dwErrorCode = pGuild->GetGuildCommerce().GetCommerceBeginningInfo(pRole->GetID(), send.nLevel, sTaelInfo);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

//-----------------------------------------------------------------------------
// 获取帮派跑商排行	-- 地图线程
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetCommerceRank(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得帮派信息
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	INT nMsgSize = sizeof(tagNS_GetCommerceRank) + sizeof(tagCommerceRank) * (MAX_COMMERCE_RANK_INFO - 1);
	MCREATE_MSG(pSend, nMsgSize, NS_GetCommerceRank);
	// 读取排行信息
	DWORD dwErrorCode = pGuild->GetGuildCommerce().GetCommerceRankInfo(pSend->sRankInfo, pSend->nRankNum, pSend->bCommend);

	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		// 调整消息大小
		pSend->dwSize = sizeof(tagNS_GetCommerceRank) + sizeof(tagCommerceRank) * (pSend->nRankNum - 1);
		SendMessage(pSend, pSend->dwSize);
	}

	MDEL_MSG(pSend);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 接收跑商
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleAcceptCommerce(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得帮派信息
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().AcceptCommerce(pRole);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		INT nLevel;
		tagTaelInfo sTaelInfo;
		tagNS_AcceptCommerce send;
		dwErrorCode = pGuild->GetGuildCommerce().GetCommerceBeginningInfo(pRole->GetID(), nLevel, sTaelInfo);
		if (dwErrorCode == E_Success)
		{
			send.nBeginningTael = sTaelInfo.nBeginningTael;
			SendMessage(&send, send.dwSize);
		}
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// 上缴商银
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleCompleteCommerce(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得帮派信息
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	INT32 nFund = 0;
	DWORD dwErrorCode = pGuild->GetGuildCommerce().CompleteCommerce(pRole, nFund);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_CompleteCommerce send;
		send.dwRoleID	= pRole->GetID();
		send.nFund		= nFund;
		pGuild->SendGuildMsg(&send, send.dwSize);
	}

	return E_Success;
}

//-----------------------------------------------------------------------------
// 放弃跑商
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleAbandonCommerce(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得帮派信息
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().AbandonCommerce(pRole->GetID());
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	
	return E_Success;
}

//-----------------------------------------------------------------------------
// 设置跑商嘉奖状态
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleSwitchCommendation(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SwitchCommendation);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 取得帮派信息
	Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
	if (!P_VALID(pGuild))
	{
		return GT_INVALID;
	}

	DWORD dwErrorCode = pGuild->GetGuildCommerce().SwitchCommendation(pRole->GetID(), pRecv->bSwitchON);
	if (E_Success != dwErrorCode)
	{
		g_guildMgr.SendGuildProcFailed2Client(this, dwErrorCode);
	}
	else
	{
		tagNS_SwitchCommendation send;
		send.dwRoleID	= pRole->GetID();
		send.bSwitchON	= pRecv->bSwitchON;
		pGuild->SendGuildMsg(&send, send.dwSize);
	}

	return E_Success;
}

DWORD PlayerSession::HandleGetGuildPurpose(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_GetGuildPurpose);
	if( P_VALID(pRecv) && P_VALID(pRecv->dwGuildID) )
	{
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) )
		{
			return pToolkit->GetGuildTenet(GetRole(),pRecv->dwGuildID);
		}
	}
	return GT_INVALID;
}

DWORD PlayerSession::HandleGetGuildSmallInfo(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_GetAllGuildSmallInfo);
	if( P_VALID(pRecv) )
	{
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) )
		{
			return pToolkit->GetGuildRanking(GetRole());
		}
	}
	return GT_INVALID;
}

DWORD PlayerSession::HandleApplyToJoinGuild(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_WantToJoinGuild);
	if( P_VALID(pRecv) )
	{
		// 添加申请前，先判断角色身上是否有“帮派决裂”的buff(bufferID：LEAVE_GUILD_BUFF_ID)
		Role* pRole = GetRole();
		if(P_VALID(pRole))
		{
			if (pRole->IsHaveBuff(LEAVE_GUILD_BUFF_TYPEID))
			{
				tagNS_WantToJoinGuild msg;
				msg.dwErrorCode = E_Guild_Can_Not_Join_Or_Create;
				pRole->SendMessage(&msg, msg.dwSize);
				return GT_INVALID;
			}
		}
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) )
		{
			return pToolkit->ApplyToJoinGuild(GetRole(),pRecv->dwGuildID);
		}
	}
	return GT_INVALID;
}

DWORD PlayerSession::HandleContactGuildLeader(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_ContactGuildLeader);
	if( P_VALID(pRecv) )
	{
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) )
		{
			return pToolkit->ConstractGuildMaster(GetRole(),pRecv->dwGuildID);
		}
	}
	return GT_INVALID;
}

DWORD PlayerSession::HandleGetGuildApplierBill(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_GetGuildApplierBill);
	Role * pRole = GetRole();
	if( P_VALID(pRecv) )
	{
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) && P_VALID(pRole) )
		{
			return pToolkit->GetApplyBill(pRole,pRole->GetGuildID());
		}
	}
	return GT_INVALID;
}

DWORD PlayerSession::HandleApproveToJoinGuild(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_ApproveToJoinGuild);
	if( P_VALID(pRecv) )
	{
		Role * pRole = GetRole();
		Role * pApplier = g_roleMgr.GetRolePtrByID(pRecv->dwApplierID);
// 		tagRoleInfo * pRoleInfo;
		if( P_VALID(pRole) /*&& P_VALID(pApplier)*/ )
		{
		//	pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetID());
		//	tagNS_ApproveToJoinGuildTrans msg;
		//	DWORD dwGuildID = pRole->GetGuildID();
		//	msg.dwErrorCode = pRecv->dwErrorCode;
		//	if( P_VALID(dwGuildID) && P_VALID(pRoleInfo) )
		//	{
		//		Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
		//		if( P_VALID(pGuild) )
		//		{
		//			const tagGuild & guildInfo = pGuild->GetGuildAtt();
		//			if( guildInfo.dwLeaderRoleID == pRole->GetID() )
		//			{
		//				msg.dwGuildMasterID = guildInfo.dwLeaderRoleID;
		//				INT len = _tcslen(pRoleInfo->szRoleName);
		//				if( len > X_SHORT_NAME - 1 )
		//					len = X_SHORT_NAME - 1;
		//				_tcsncpy(msg.szMasterName,pRoleInfo->szRoleName,len);
		//				msg.szMasterName[len] = 0;

		//				pApplier->SendMessage(&msg,msg.dwSize);
		//				return E_Success;
		//			}
		//		}
		//	}

			// 添加申请前，先判断角色身上是否有“帮派决裂”的buff(bufferID：LEAVE_GUILD_BUFF_ID)
			if(P_VALID(pApplier) && pRecv->dwErrorCode == 0)
			{
				if (pApplier->IsHaveBuff(LEAVE_GUILD_BUFF_TYPEID))
				{
					tagNS_RoleInJustLeaveGuildState msg;
					msg.dwErrorCode = E_Guild_Can_Not_Be_Joined;
					pRole->SendMessage(&msg, msg.dwSize);
					return GT_INVALID;
				}
			}

			ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
			if( P_VALID(pToolkit) && P_VALID(pRole) )
			{
				return pToolkit->ApproveToJoinGuild(pRole,pRecv->dwApplierID,pRecv->dwErrorCode == 0);
			}
		}
	}
	return GT_INVALID;
}


DWORD PlayerSession::HandleSearchGuild(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_SearchGuild);
	Role * pRole = GetRole();
	if( P_VALID(pRecv) )
	{
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) && P_VALID(pRole) )
		{
			pRecv->szName[X_SHORT_NAME-1] = 0;
			tstring what(pRecv->szName);
			return pToolkit->SearchGuild(pRole,pRecv->dwSearchMode,what);
		}
	}
	return GT_INVALID;
}

//------------------------------------------------------------------------
// 公会战争
//------------------------------------------------------------------------
DWORD PlayerSession::HandleDeclareGuildWar(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_DeclareGuildWar);
	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 客户端做过检查了，以防万一，做一下检查，如果跟自己宣战，不予理睬
	if (pRecv->dwAttackGuildID == pRecv->dwDefenceGuildID)
	{
		return GT_INVALID;
	}

	Guild* pAttackGuild = g_guildMgr.GetGuild(pRecv->dwAttackGuildID);
	Guild* pDefenceGuild = g_guildMgr.GetGuild(pRecv->dwDefenceGuildID);
	if (!P_VALID(pAttackGuild))
	{
		return GT_INVALID;
	}

	tagNS_DeclareGuildWar send;

	// 被宣战帮派不存在了
	if (!P_VALID(pDefenceGuild))
	{
		send.dwErrorCode = E_GuildDeclareWar_Guild_Not_Exist;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	tagGuildMember* pMaster = pAttackGuild->GetMemberMgr().GetMaster();
	if (!P_VALID(pMaster))
	{
		return GT_INVALID;
	}

	// 检查是不是会长
	if (pMaster->dwRoleID != pRole->GetID())
	{
		send.dwErrorCode = E_GuildDeclareWar_Not_Guild_Leader;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// 检查背包是否有如下道具：（暂定1w金币？）
	if (pAttackGuild->GetGuildAtt().nFund < DECLEAR_WAR_SILVER_COST)
	{
		send.dwErrorCode = E_GuildDeclareWar_Gold_Not_Enough;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// 检查背包是否有如下道具：（暂定“宣战令牌”道具）
	tagItem* pItem = pRole->GetItemMgr().GetItemByTypeID(DECLEAR_WAR_ITEM_TYPE_ID);
	if (!P_VALID(pItem))
	{
		send.dwErrorCode = E_GuildDeclareWar_No_Spec_Item;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	//检查两个帮派的宣战关系是不是已经存在了
	if (g_guildWarDeclareMgr.IsInGuildWarState(pRecv->dwAttackGuildID, pRecv->dwDefenceGuildID))
	{
		send.dwErrorCode = E_GuildDeclareWar_Declare_Exist;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// 宣战次数检查
	send.byTodayDeclareWarTimes = g_guildWarDeclareMgr.GetTodayDeclareWarTimes(pRecv->dwAttackGuildID);
	if (send.byTodayDeclareWarTimes >= MAX_GUILD_DECLARE_WAR_TIMES_IN_ONEDAY)
	{
		send.dwErrorCode = E_GuildDeclareWar_Declare_Times_Full;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}
	
	// 对同一个工会每天宣战次数检查
	if (g_guildWarDeclareMgr.GetTodaySameGuildBeWarTimes(pRecv->dwAttackGuildID, pRecv->dwDefenceGuildID) >= MAX_SAME_GUILD_BE_DECLARE_WAR_TIMES)
	{
		send.dwErrorCode = E_GuildDeclareWar_Same_Guild_Full;
		pRole->SendMessage(&send, send.dwSize);

		return GT_INVALID;
	}

	// 宣战成功, 发广播"您的公会已经和XXX进行了行会战争"
	DWORD	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	DWORD   dwTmpNum = 100036;
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwTmpNum);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &pRecv->dwDefenceGuildID);
	g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, pAttackGuild);
	
	// 宣战成功, 发广播"您的公会已经和XXX进行了行会战争"
	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	dwTmpNum = 100036;
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwTmpNum);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &pRecv->dwAttackGuildID);
	g_MsgInfoMgr.DispatchGuildMsgEvent(dwMsgInfoID, pDefenceGuild);

	// 宣战成功, 世界发广播"XXX公会已经和XXX公会进行了行会战争"
	dwMsgInfoID		=	(DWORD)g_MsgInfoMgr.BeginMsgEvent();
	dwTmpNum = 100035;
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_NewBroadcast, &dwTmpNum);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &pRecv->dwAttackGuildID);
	g_MsgInfoMgr.AddMsgEvent(dwMsgInfoID, EMUT_GuildID, &pRecv->dwDefenceGuildID);
	g_MsgInfoMgr.DispatchWorldMsgEvent(dwMsgInfoID);

	// 宣战成功，扣除公会战宣战道具和宣战资金
	pAttackGuild->DecGuildFund(pRole->GetID(), DECLEAR_WAR_SILVER_COST, ELCID_Guild_DeclareWar);
	pRole->GetItemMgr().DelFromBag(pItem->n64Serial, ELCID_Guild_DeclareWar, 1);

	// 给两个帮派发送宣战成功消息
	g_guildWarDeclareMgr.AddGuildWarDeclare2Map(pRecv->dwAttackGuildID, pRecv->dwDefenceGuildID);
	send.dwErrorCode = E_GuildDeclareWar_Success;
	send.byTodayDeclareWarTimes = g_guildWarDeclareMgr.GetTodayDeclareWarTimes(pRecv->dwAttackGuildID);
	send.dwDefenceGuildID = pRecv->dwDefenceGuildID;
	send.dwAttackGuildID = pRecv->dwAttackGuildID;
	pAttackGuild->SendGuildMsg(&send, send.dwSize);
	pDefenceGuild->SendGuildMsg(&send, send.dwSize);

	return E_Success;
}

// 取同盟列表
DWORD PlayerSession::HandleGetAllianceList(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_GetAllianceList);

	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (P_VALID(CGodMiracleMgr::GetInst()))
	{
		CGodMiracleMgr::GetInst()->SendAllianceList2Client(pRecv->eGodID, pRole);
		return E_Success;
	}
	return GT_INVALID;
}

// 战场同盟公会报名
DWORD PlayerSession::HandleApplyForAlliance(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_ApplyForAlliance);

	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (P_VALID(CGodMiracleMgr::GetInst()))
	{
		tagNS_ApplyForAlliance send;
		send.dwGodID = pRecv->dwGodID;

		// 判断角色是否是公会会长
		Guild* pGuild = g_guildMgr.GetGuild(pRole->GetGuildID());
		ASSERT(P_VALID(pGuild));
		if (!P_VALID(pGuild))
			return E_SystemError;

		if ( EGMP_HuiZhang != pGuild->GetMemberMgr().GetMember(pRole->GetID())->eGuildPos)
			send.dwErrorCode = EEGB_ApplyNoGuildMaster;
		else
			send.dwErrorCode = CGodMiracleMgr::GetInst()->Add2AllianceGuildMap(pRecv->dwGodID, pRole->GetGuildID(), pRecv->eGuildCapType, FALSE);
		pRole->SendMessage(&send, send.dwSize);
	}
	return E_Success;
}

// 确认同盟
DWORD PlayerSession::HandleConfirmAllianceGuild(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_ConfirmAllianceGuild);

	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (P_VALID(CGodMiracleMgr::GetInst()) && P_VALID(CGodMiracleMgr::GetInst()->GetGodMiracle(pRecv->dwGodID)))
	{
		tagNS_ConfirmAllianceGuild send;
		send.dwGodID = pRecv->dwGodID;
		send.dwGuildID = pRecv->dwGuildID;
		send.dwErrorCode = CGodMiracleMgr::GetInst()->ConfirmAllianceGuild(pRecv->dwGodID, pRecv->dwGuildID);
		send.eGuildCapType = CGodMiracleMgr::GetInst()->GetGodMiracle(pRecv->dwGodID)->GetAllianceCapType(pRecv->dwGuildID);
		pRole->SendMessage(&send, send.dwSize);
	}

	return E_Success;
}

DWORD PlayerSession::HandleGuildWarOccupyedNum(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_GuildOccupyedNum);

	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_GuildOccupyedNum backmsg;

	DWORD dwGuildID = pRole->GetGuildID() ;
	if( !P_VALID(dwGuildID) )
	{
		backmsg.bAlreadyTake = FALSE;
		backmsg.nNum			= 0;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
	if( !P_VALID(pGuild) )
	{
		backmsg.bAlreadyTake = FALSE;
		backmsg.nNum			= 0;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	CGodMiracleMgr* pGod = CGodMiracleMgr::GetInst();
	if( !P_VALID(pGod) )
	{
		backmsg.bAlreadyTake = FALSE;
		backmsg.nNum			= 0;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	backmsg.nNum = pGod->GetGuildDefendGodMiracle(dwGuildID);
	backmsg.bAlreadyTake = pRole->HasTakenGuildDailyReward() ? true : false;
	SendMessage(&backmsg,backmsg.dwSize);
	return E_Success;
}

DWORD PlayerSession::HandleGuildWarTakeWarReword(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv,pCmd,NC_TakeGuildWarReword);

	if (!P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	Role * pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_TakeGuildWarReword backmsg;

	if( pRole->HasTakenGuildDailyReward() )
	{
		backmsg.dwErrorCode = EGEC_DailyReward_AlreadyTaken;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	// 获得地图
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(pRecv->dwNPCID);
	if( !P_VALID(pNPC) ) 
	{
		backmsg.dwErrorCode = EGEC_NPC_NotExist;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_NPC_NotExist;
	}

	if( FALSE == pNPC->CheckNPCTalkDistance(pRole) )
	{
		backmsg.dwErrorCode = EGEC_NPC_DisFar;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_NPC_DisFar;
	}

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_GuildWarRewordNPC) )
	{
		backmsg.dwErrorCode = EGEC_NPC_TypeMismatch;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_NPC_TypeMismatch;
	}


	DWORD dwGuildID = pRole->GetGuildID() ;
	if( !P_VALID(dwGuildID) )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	Guild * pGuild = g_guildMgr.GetGuild(dwGuildID);
	if( !P_VALID(pGuild) )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	CGodMiracleMgr* pGod = CGodMiracleMgr::GetInst();
	if( !P_VALID(pGod) )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	if( pRole->GetLevel() < 40 )
	{
		backmsg.dwErrorCode = EGEC_DailyReward_LevelLimit;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_DailyReward_LevelLimit;
	}

	const tagGuild & guildAtt = pGuild->GetGuildAtt();
	GuildMember & guildMem = pGuild->GetMemberMgr();
	tagGuildMember * pMem = NULL;
	pMem = guildMem.GetMember(pRole->GetID());

	if( !P_VALID(pMem) )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}
	if( EGMP_Start > pMem->eGuildPos || pMem->eGuildPos > EGMP_End )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	const tagGuildDailyRewardProto * pReward = g_attRes.GetGuildDailyRewardProto(pMem->eGuildPos,pGod->GetGuildDefendGodMiracle(dwGuildID));
	if( !P_VALID(pReward) )
	{
		backmsg.dwErrorCode = EGEC_DailyReward_CannotFindProto;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_DailyReward_CannotFindProto;
	}

	GuildPos & guildPos = pGuild->GetGuildPos();

	if( guildAtt.wGuildPosTakeRewardTimes[pMem->eGuildPos] >= guildPos.GetPosNum(pMem->eGuildPos) )
	{
		backmsg.dwErrorCode = EGEC_DailyReward_GuildPosLimit;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}

	if( pRole->GetItemMgr().GetBagFreeSize() < 1 )
	{
		backmsg.dwErrorCode = EGEC_DailyReward_BagFull;
		SendMessage(&backmsg,backmsg.dwSize);
		return EGEC_DailyReward_BagFull;
	}

	if( P_VALID(pReward->dwBuffID) )
	{
		pRole->TryAddBuffToMyself(pReward->dwBuffID);
	}

	tagItem * pItem = ItemCreator::Create(EICM_GuildDailyReward,  GT_INVALID, pReward->dwItemTypeID );
	if( !P_VALID(pItem) )
	{
		backmsg.dwErrorCode = GT_INVALID;
		SendMessage(&backmsg,backmsg.dwSize);
		return GT_INVALID;
	}
	pRole->GetItemMgr().Add2Bag(pItem, ELCID_GuildDailyReward, TRUE);
	// 保存到数据库
	const_cast<tagGuild&>(guildAtt).wGuildPosTakeRewardTimes[pMem->eGuildPos] ++;
	pGuild->SynGuildAtt2DB();
	pRole->ResetGuildDailyRewardTime();

	backmsg.dwErrorCode = E_Success;
	SendMessage(&backmsg,backmsg.dwSize);

	return E_Success;
}



