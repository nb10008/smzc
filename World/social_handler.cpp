//-----------------------------------------------------------------------------
//!\file talent_handler.cpp
//!\author wjiang

//!\date 2008-12-04
//! last 
//!
//!\brief 玩家社会交流消息处理类
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/group_define.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/msg_group.h"
#include "../WorldDefine/msg_social.h"

#include "player_session.h"
#include "unit.h"
#include "role.h"
#include "group_mgr.h"
#include "social_mgr.h"
#include "../WorldDefine/msg_easy_team.h"
#include "convenient_team.h"

//-----------------------------------------------------------------------------
// 添加好友
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleMakeFriend(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_RoleMakeFriend* pRecv = (tagNC_RoleMakeFriend*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_MakeFriend, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 删除好友
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleCancelFriend(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_RoleCancelFriend* pRecv = (tagNC_RoleCancelFriend*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_CancelFriend, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 删除好友
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleUpdateFriendGroup(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_UpdateFriGroup* pRecv = (tagNC_UpdateFriGroup*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_FriendGrp, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 将玩家移入黑名单
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleMoveBlackList(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_MoveToBlackList* pRecv = (tagNC_MoveToBlackList*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_InsertBlkList, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 将玩家移出黑名单
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleDeleteBlackList(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_DeleteBlackList* pRecv = (tagNC_DeleteBlackList*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_DeleteBlkList, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 送礼物
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSendGift(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_SendGift* pRecv = (tagNC_SendGift*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_SendGift, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 收到礼物后回复
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSendGiftReply(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_SendGiftReply* pRecv = (tagNC_SendGiftReply*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_SendGiftRpy, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 邀请玩家加入小队
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleJoinTeam(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_InviteJoinTeam* pRecv = (tagNC_InviteJoinTeam*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_JoinTeam, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 返回玩家是否同意加入小队
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleJoinTeamReply(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_InviteReply* pRecv = (tagNC_InviteReply*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_JoinTeamRepley, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 踢玩家出队
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleKickMember(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_KickMember* pRecv = (tagNC_KickMember*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_KickMember, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 玩家离开小队
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleLeaveTeam(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	tagNC_LeaveTeam* pRecv = (tagNC_LeaveTeam*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_LeaveTeam, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 设置小队拾取模式
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetPickMol(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole))	return GT_INVALID;

	tagNC_SetPickMode* pRecv = (tagNC_SetPickMode*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_SetPickMol, pCmd->dwSize, pRecv);

	return 0;
}

//-----------------------------------------------------------------------------
// 改变小队队长
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleChangeLeader(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole))	return GT_INVALID;

	tagNC_ChangeLeader* pRecv = (tagNC_ChangeLeader*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_ChangeLeader, pCmd->dwSize, pRecv);

	return 0;
}


//-----------------------------------------------------------------------------
// 同步单向好友是否在线
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleUpdateFriOnline(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole))	return GT_INVALID;

	tagNC_UpdateFriEnemyState* pRecv = (tagNC_UpdateFriEnemyState*)pCmd;

	m_pRole->UpdateFriOnline();
	return 0;
}


//Jason 申请入队相关协议

DWORD	PlayerSession::HandleApplyJoinTeam(tagNetCmd *pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNC_ApplyToJoinTeam* pRecv = (tagNC_ApplyToJoinTeam*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_ApplyToJoinTeam, pCmd->dwSize, pRecv);

	return 0;
}
DWORD	PlayerSession::HandleReplyForApplingToJoinTeam(tagNetCmd *pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNC_ApplyReply* pRecv = (tagNC_ApplyReply*)pCmd;

	g_groupMgr.AddEvent(m_pRole->GetID(), EVT_ApplyReply, pCmd->dwSize, pRecv);

	return 0;
}

// 组队增益
DWORD PlayerSession::HandleTeamProfit(tagNetCmd* pCmd)
{
	if (!P_VALID(pCmd))	return GT_INVALID;

	MTRANS_POINTER(pRecv, pCmd, tagNC_TeamBufChange);

	Role* pRole = GetRole();
	if(!P_VALID(pRole)) return GT_INVALID;

	// 组队增益的触发条件变为1
	if (pRecv->bTeamBuffer) 
	{
		DWORD dwTeamID = pRole->GetTeamID();
		if (dwTeamID == GT_INVALID)
		{
			// 若玩家无小队，那么消息验证不通过，不受理
			return GT_INVALID;
		}
		else
		{
			if (pRole->CanGetTeamProfit())
			{
				pRole->GetTeamProfit();
				return E_Success;
			}
		}
	}
	// 组队增益的触发条件变为0
	else
	{
		DWORD dwTeamID = pRole->GetTeamID();
		if (dwTeamID == GT_INVALID)
		{
			// 若玩家无小队，那么消息属实，取消组队增益
			pRole->CancelTeamProfit();
			return E_Success;
		}
		else
		{
			// 验证在100格子内是否有队友
			if (!pRole->CanGetTeamProfit())
			{
				pRole->CancelTeamProfit();
				return E_Success;
			}
		}
	}

	return GT_INVALID;
}

//---------------------------------------------------------------
// 仇敌列表相关消息
//---------------------------------------------------------------
// 删除仇敌
DWORD PlayerSession::HandleDeleteEnemy(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNC_DeleteEnemy* pRecv = (tagNC_DeleteEnemy*)pCmd;

	g_socialMgr.AddEvent(m_pRole->GetID(), EVT_DeleteEnemy, pCmd->dwSize, pRecv);

	return 0;
}

// 便捷组队信息，这个不要在g_groupMgr里面处理，因为消息可能很多，放进地图线程吧，无奈啊。。。
DWORD PlayerSession::HandleGetEasyTeamInfo(tagNetCmd * pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNS_GetEasyTeamInfo * pMsg = NULL;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	if( EasyTeam.GetEasyTeamInfo(pMsg,m_pRole,&g_MemPoolSafe) )
	{
		SendMessage(pMsg,pMsg->dwSize);
		g_MemPoolSafe.Free(pMsg);
	}

	return 0;
}

DWORD PlayerSession::HandleGetFreePlayers(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNS_GetSinglePlayers * pMsg = NULL;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	if( EasyTeam.GetFreePlayers(pMsg,m_pRole,&g_MemPoolSafe) )
	{
		SendMessage(pMsg,pMsg->dwSize);
		g_MemPoolSafe.Free(pMsg);
	}

	return 0;
}

DWORD PlayerSession::HandleGetPersonalInfo(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNS_GetPersonalInfo * pMsg = NULL;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	if( EasyTeam.GetPersonalInfo(pMsg,m_pRole,&g_MemPoolSafe) )
	{
		SendMessage(pMsg,pMsg->dwSize);
		g_MemPoolSafe.Free(pMsg);
	}

	return 0;
}
DWORD PlayerSession::HandleNCPutOutInfo(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	tagNS_PutOutInfo msg;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	msg.dwErrorCode = EasyTeam.IssueCTInfo((tagNC_PutOutInfo*)pCmd,m_pRole);
	SendMessage(&msg,msg.dwSize);

	return 0;
}
DWORD PlayerSession::HandleNCDeleteAppTeamInfo(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	EasyTeam.DeleteCTInfoOfPlayer(m_pRole);

	return 0;
}
DWORD PlayerSession::HandleNCDeleteMyTeamInfo(tagNetCmd* pCmd)
{
	if( !P_VALID(m_pRole) || !P_VALID(pCmd) ) return GT_INVALID;

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	EasyTeam.DeleteCTInfoOfPlayer(m_pRole);

	return 0;
}




