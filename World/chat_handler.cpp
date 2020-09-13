//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: character_handler.cpp
// author: lh
// actor:
// data: 2008-08-27
// last:
// brief: 客户端和服务器端间消息处理 -- 聊天相关
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "player_session.h"
#include "role.h"
#include "role_mgr.h"
#include "map.h"
#include "../WorldDefine/chat.h"
#include "../WorldDefine/chat_define.h"
#include "../WorldDefine/msg_GroupChat.h"
#include "../WorldDefine/msg_show_item.h"
#include "../WorldDefine/filter.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "att_res.h"
#include "group_chat.h"
#include "group_mgr.h"
#include "guild.h"
#include "guild_mgr.h"
#include "ps_bomb.h"
#include "chat_mgr.h"

//------------------------------------------------------------------------------------
// 聊天
//------------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleChat(tagNetCmd *pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (pRole->SpeakOff())
	{
		tagNS_RoleChat send;
		send.dwErrorCode = E_RoleChat_ForbidChat;
		SendMessage(&send, send.dwSize);

		return 0;
	}
	
	tagNC_RoleChat* pRecv = (tagNC_RoleChat*)pCmd;

	if (pRole->IsHaveBuff(NoSpeakBuffID))
	{
		if(pRecv->byChannel == 0 || pRecv->byChannel == 5)	//修改为只禁言世界和场景频道
		{
			tagNS_RoleChat send;
			send.dwErrorCode = E_RoleChat_ForbidChat;
			SendMessage(&send, send.dwSize);

			return 0;
		}
	}
	
	BOOL bFindFlag = FALSE;
	INT nContentLength = pRecv->dwSize - (sizeof(tagNC_RoleChat)-sizeof(TCHAR));
	tstring strContent = pRecv->szMsg;
	Filter::WordsFilterForChat( strContent, bFindFlag, g_attRes.GetChatFilterWordsVec());
	if (TRUE == bFindFlag)
	{
		INT nSize = sizeof(tagNS_RoleChat) + nContentLength - sizeof(TCHAR);
		//tagNS_RoleChat *pSend = (tagNS_RoleChat*)new  BYTE(nSize);
		MCREATE_MSG(pSend, nSize, NS_RoleChat);
		if(P_VALID(pSend))
		{
			pSend->dwSize = nSize;
			pSend->dwID = IUTIL->Crc32("NS_RoleChat");
			pSend->dwErrorCode = E_RoleChat_MsgBanned;
			pSend->dwSrcRoleID = pRole->GetID();
			pSend->byChannel = pRecv->byChannel;
			_tcscpy_s(pSend->szMsg, nContentLength, pRecv->szMsg);
			SendMessage(pSend, pSend->dwSize);
		}
		return 0;
	}

	DWORD dwRtv = g_msgMgr.HandleChatMsg(pRecv, GetRole());
	if (E_Success != dwRtv)
	{
		tagNS_RoleChat send;
		send.dwErrorCode = dwRtv;
		SendMessage(&send, send.dwSize);
	}

	return 0 ;

}

//------------------------------------------------------------------------
// 装备展示
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleShowEquip(tagNetCmd* pCmd)
{
	tagNC_RoleShowEquip *pRecv = (tagNC_RoleShowEquip*)pCmd;

	DWORD dwRtv = g_msgMgr.HandleChatMsg(pRecv, GetRole());
	if (E_Success != dwRtv)
	{
		tagNS_RoleShowEquip send;
		send.dwErrorCode = dwRtv;
		SendMessage((LPVOID)&send, send.dwSize);
	}

	return E_Success;
}

//------------------------------------------------------------------------
// 物品展示
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleShowItem(tagNetCmd* pCmd)
{
	tagNC_RoleShowItem *pRecv = (tagNC_RoleShowItem*)pCmd;

	DWORD dwRtv = g_msgMgr.HandleChatMsg(pRecv, GetRole());
	if (E_Success != dwRtv)
	{
		tagNS_RoleShowItem send;
		send.dwErrorCode = dwRtv;
		SendMessage(&send, send.dwSize);
	}
	return E_Success;

}

//------------------------------------------------------------------------
// 获取留言
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleLoadLeftMsg(tagNetCmd* pCmd)
{
	if (!P_VALID(GetRole()))
	{
		return GT_INVALID;
	}

	DWORD dwRtv = g_msgMgr.LoadLeftMsg(GetRole()->GetID());
	
	return E_Success;
}

//------------------------------------------------------------------------
// 创建群
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleCreateGroupChat(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_CreateGroupChat);
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}	
	DWORD dwError = 0 ;
	INT32 nNameTCHAR = (pRecv->dwSize - FIELD_OFFSET(tagNC_CreateGroupChat, szGroupChatName)) / sizeof(TCHAR);
	tstring szTmpGroupChatName(pRecv->szGroupChatName, nNameTCHAR);
	// 帮派名合法性判断
	dwError = Filter::CheckName(szTmpGroupChatName.c_str(), MAX_GROUPCHAT_LEN, 	1, g_attRes.GetNameFilterWords());
	if(dwError != E_Success)
	{
		tagNS_CreateGroupChat send;
		send.dwErrorCode = GRC_GroupChatNameInvalid ;
		pRole->SendMessage(&send,send.dwSize);
		return 0 ;

	}

	DWORD dwGroupChatId    = g_GroupChatMgr.CreateGroupChatId();
	group_chat* pGroupChat = g_GroupChatMgr.CreateGroupChat(dwGroupChatId);
	if ( P_VALID(pGroupChat) )
	{
		pGroupChat->Init(szTmpGroupChatName.c_str(),pRole->GetID(),dwGroupChatId);
		tagNS_CreateGroupChat send;
		send.dwErrorCode    = E_Success;
        _tcscpy_s(send.szGroupChatName,nNameTCHAR,szTmpGroupChatName.c_str());
		send.dwGroupChatId  = dwGroupChatId;
		pRole->SendMessage(&send,send.dwSize); 

		pGroupChat->AddRoleToGroup(pRole);

		DWORD dwNum = pGroupChat->GetGroupMemberNum() ;
		DWORD dwMsgSize =  sizeof(tagNS_AddRoleToGroupChat)+ (dwNum - 1)*sizeof(tagGroupChatMember);   //多分配几个字节
		MCREATE_MSG(pSend,dwMsgSize, NS_AddRoleToGroupChat);
		pSend->dwError             = 0 ;
		pSend->dwMemberNum         = dwNum;
		pSend->dwGroupChatId       = dwGroupChatId;
		pSend->dwGroupChatMasterId = pRole->GetID();
		_tcscpy_s(pSend->szGroupChatName,nNameTCHAR,szTmpGroupChatName.c_str());
		std::list<DWORD> TempListId;
		TSafeList<DWORD>& listGroupChatId = pGroupChat->GetListGroupMemberId();
		listGroupChatId.Export(TempListId);
		std::list<DWORD>::iterator it = TempListId.begin();
		for (INT i = 0 ; i <(INT)dwNum ; i++)
		{

			if ( it == TempListId.end())
			{
				break;
			}
			g_roleMgr.GetRoleNameByNameID(*it,pSend->GroupChatMember[i].szRoleName);
			pSend->GroupChatMember[i].dwRoleId = *it;
			it++;


		}
		pRole->SendMessage(pSend,pSend->dwSize);
		MDEL_MSG(pSend);


	}



	return 0;
}

//------------------------------------------------------------------------
// 删除群
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleDeleteGroupChat(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_DeleteGroupChat);
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}
	group_chat* pGroupChat = g_GroupChatMgr.GetGroupChat(pRecv->dwGroupChatId);
	if (!P_VALID(pGroupChat))
	{
		tagNS_DeleteGroupChat msg;
		msg.dwError = GRC_GroupChatNotExist;
		msg.dwGroupChatId  = pRecv->dwGroupChatId ;
		pRole->SendMessage(&msg,msg.dwSize);
		return msg.dwError;
		return GRC_GroupChatNotExist;
	}
	else
	{
		if ( pGroupChat->GetGroupChatMasterId() != pRole->GetID() )
		{ 
			tagNS_DeleteGroupChat msg;
			msg.dwError = GRC_NotGroupChatMaster;
			msg.dwGroupChatId  = pRecv->dwGroupChatId ;
			pRole->SendMessage(&msg,msg.dwSize);
			return msg.dwError;
		}
		tagNS_DeleteGroupChat msg;
		msg.dwError        = E_Success;
		msg.dwGroupChatId  = pRecv->dwGroupChatId ;
		pGroupChat->SendGroupMsg(&msg,msg.dwSize);
		pGroupChat->RoleDeleteGroup();
		g_GroupChatMgr.DeleteGroupChat(pRecv->dwGroupChatId);

	}


	return 0;
}

//------------------------------------------------------------------------
// 退出群
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleLoginOutGroupChat(tagNetCmd* pCmd)
{

	MGET_MSG(pRecv, pCmd, NC_LoginOutGroupChat);
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}
	group_chat* pGroupChat  = g_GroupChatMgr.GetGroupChat(pRecv->dwGroupChatId);
	if (!P_VALID(pGroupChat))
	{
		tagNS_LoginOutGroupChat msg;
		msg.dwError  = GRC_GroupChatNotExist;
		msg.dwRoleId       = GT_INVALID;
		msg.dwGroupChatId = pRecv->dwGroupChatId;
		msg.dwGroupChatMasterId  = GT_INVALID;
		pRole->SendMessage(&msg,msg.dwSize);
		return GRC_GroupChatNotExist;
	} 
	DWORD dwError = pGroupChat->LoginOutGroup(pRole);
	if (dwError != E_Success)
	{
		tagNS_LoginOutGroupChat msg;
		msg.dwError       = dwError;
		msg.dwRoleId      = 0;
		msg.dwGroupChatId = pRecv->dwGroupChatId;
		msg.dwGroupChatMasterId  = pGroupChat->GetGroupChatMasterId();
		pRole->SendMessage(&msg,msg.dwSize);
	}
	else
	{   
		

		tagNS_LoginOutGroupChat msg;
		msg.dwError       = dwError;
		msg.dwRoleId      = pRole->GetID();
		msg.dwGroupChatId = pRecv->dwGroupChatId;
		msg.dwGroupChatMasterId  = pGroupChat->GetGroupChatMasterId();
		pGroupChat->SendGroupMsg(&msg,msg.dwSize);
		pRole->SendMessage(&msg,msg.dwSize);
		if ( pGroupChat->GetGroupMemberNum() <= 0)
		{
			g_GroupChatMgr.DeleteGroupChat(pRecv->dwGroupChatId);
		}
	}



	return 0;
}

//------------------------------------------------------------------------
// 邀请加入
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleInviteRoleToGroupChat(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_InviteRoleToGroupChat);
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}

	if (!GT_VALID(pRecv->dwDestRoleID))
	{
		return GT_INVALID;
	}
	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwDestRoleID);
	if ( !P_VALID(pDestRole))
	{
		tagNS_InviteRoleToGroupChat Msg;
		Msg.dwError = GRC_RoleNotOnline;
		pRole->SendMessage(&Msg,Msg.dwSize);
		return Msg.dwError;
	}
	else
	{
		group_chat* pGroupChat = g_GroupChatMgr.GetGroupChat(pRecv->dwGroupChatId);
		if ( P_VALID(pGroupChat))
		{
			if ( pRole->GetID() != pGroupChat->GetGroupChatMasterId())
			{
				tagNS_InviteRoleToGroupChat Msg;
				Msg.dwError = GRC_NotGroupChatMaster;
				pRole->SendMessage(&Msg,Msg.dwSize);
				return Msg.dwError;
			}
			BOOL bIsExist = pGroupChat->GetGroupChatRole(pRecv->dwDestRoleID);
			if ( bIsExist )
			{ 
				tagNS_InviteRoleToGroupChat Msg;
				Msg.dwError = GRC_AlreadyInGroupChat;
				pRole->SendMessage(&Msg,Msg.dwSize);
				return Msg.dwError;

			}

			tagNS_InviteRoleJoinToGroupChat msg;
			msg.dwInviteRoleId = pRole->GetID();
			msg.dwGroupChatId  = pRecv->dwGroupChatId;
			_tcscpy_s(msg.szGroupChatName,MAX_GROUPCHAT_LEN,pGroupChat->GetGroupChatName());
			pDestRole->SendMessage(&msg,msg.dwSize);
			return  0 ;   
		}
		return GT_INVALID;
	}


}

//------------------------------------------------------------------------
// 剔出群成员
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleKickRoleFromGroupChat(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_KickRoleFromGroupChat);
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}
	Role* pKickedRole  = g_roleMgr.GetRolePtrByID(pRecv->dwRoleId);
	if ( !P_VALID(pKickedRole) )
	{
		return GT_INVALID;
	}
	group_chat* pGroupChat = g_GroupChatMgr.GetGroupChat(pRecv->dwGroupChatId);
	if ( !P_VALID(pGroupChat) )
	{
		tagNS_KickRoleFromGroupChat msg;
		msg.dwError  = GRC_GroupChatNotExist ;
		msg.dwRoleId = 0 ;
		msg.dwGroupChatId = pRecv->dwGroupChatId;
		pRole->SendMessage(&msg,msg.dwSize);
		return GRC_GroupChatNotExist;
	}
	if ( pGroupChat->GetGroupChatMasterId() != pRole->GetID())
	{
		tagNS_KickRoleFromGroupChat msg;
		msg.dwError  = GRC_NotGroupChatMaster ;
		msg.dwRoleId = 0 ;
		msg.dwGroupChatId = pRecv->dwGroupChatId;
		pRole->SendMessage(&msg,msg.dwSize);
		return GRC_GroupChatNotExist;
	}
	pKickedRole->DeleteGroupChat(pRecv->dwGroupChatId);
	DWORD dwError = pGroupChat->KickRoleFrouGroup(pRecv->dwRoleId);
	if ( dwError != E_Success)
	{
		tagNS_KickRoleFromGroupChat msg;
		msg.dwError  = dwError;
		msg.dwRoleId = 0 ;
		msg.dwGroupChatId = pRecv->dwGroupChatId;
		pRole->SendMessage(&msg,msg.dwSize);
	}
	else
	{
		tagNS_KickRoleFromGroupChat msg;
		msg.dwError  = dwError;
		msg.dwRoleId = pRecv->dwRoleId;
		msg.dwGroupChatId = pRecv->dwGroupChatId;
		pGroupChat->SendGroupMsg(&msg,msg.dwSize);
		pKickedRole->SendMessage(&msg,msg.dwSize);
	}



	return 0;
}

//------------------------------------------------------------------------
// 加入群
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleAddRoleToGroupChat(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_AddRoleToGroupChat);
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}
	Role* pInviteRole = g_roleMgr.GetRolePtrByID(pRecv->dwInviteRoleId);

	if ( !P_VALID(pInviteRole) )
	{
		return GT_INVALID;
	}
	if ( !pRecv->bIsAccepted)
	{
		tagNS_InviteRoleToGroupChat send;
		send.dwError = GRC_RefuseJoinGroupChat ;
		g_roleMgr.GetRoleNameByID(pRole->GetID(),send.szName);
		pInviteRole->SendMessage(&send,send.dwSize);
		return GRC_RefuseJoinGroupChat;

	}
	DWORD dwError  = 0 ;
	group_chat* pGroupChat = g_GroupChatMgr.GetGroupChat(pRecv->dwGroupChatId);
	if ( !P_VALID(pGroupChat) )
	{
		tagNS_AddRoleToGroupChat  msg;
		memset(&msg,0,sizeof(msg));
		msg.dwError = GRC_GroupChatNotExist;
		pRole->SendMessage(&msg,msg.dwSize);
		return GRC_GroupChatNotExist ;
	}
	else
	{
		dwError = pGroupChat->AddRoleToGroup(pRole);
		if ( E_Success != dwError )
		{
			tagNS_AddRoleToGroupChat  msg;
			memset(&msg,0,sizeof(msg));
			msg.dwError = dwError;
			pRole->SendMessage(&msg,msg.dwSize);
			return dwError;
		}
		//把群的每个成员的信息通知给被邀请人
		DWORD dwNum = pGroupChat->GetGroupMemberNum() ;
		DWORD dwMsgSize =  sizeof(tagNS_AddRoleToGroupChat) + (dwNum - 1)* sizeof(tagGroupChatMember) + (dwNum*sizeof(TCHAR)*X_SHORT_NAME);   //多分配几个字节
		MCREATE_MSG(pSend,dwMsgSize, NS_AddRoleToGroupChat);
		pSend->dwError             = 0 ;
		pSend->dwMemberNum         = dwNum;
		pSend->dwGroupChatId       = pGroupChat->GetGroupChatId();
		pSend->dwGroupChatMasterId = pGroupChat->GetGroupChatMasterId();
		_tcscpy_s(pSend->szGroupChatName,MAX_GROUPCHAT_LEN,pGroupChat->GetGroupChatName());
		std::list<DWORD> TempListId;
		TSafeList<DWORD>& listGroupChatId = pGroupChat->GetListGroupMemberId();
		listGroupChatId.Export(TempListId);
		std::list<DWORD>::iterator it = TempListId.begin();
		for (INT i = 0 ; i < (INT)dwNum ; i++)
		{

			if (it == TempListId.end())
			{
				break;
			}
			g_roleMgr.GetRoleNameByNameID(*it,pSend->GroupChatMember[i].szRoleName);
			pSend->GroupChatMember[i].dwRoleId = *it ;
			it++;


		}
		pRole->SendMessage(pSend,pSend->dwSize);
		MDEL_MSG(pSend);

		////把被邀请人的信息通知给群里的每个成员
		DWORD dwSize = sizeof(tagNS_RoleJoinToGroupChat)+sizeof(TCHAR)*X_SHORT_NAME;
		MCREATE_MSG(pSendMsg,dwSize,NS_RoleJoinToGroupChat);
		g_roleMgr.GetRoleNameByID(pRole->GetID(),pSendMsg->szName);
		pSendMsg->dwRoleID = pRole->GetID();
		pSendMsg->dwGroupChatId = pRecv->dwGroupChatId;
		pGroupChat->SendGroupMsg(pSendMsg,pSendMsg->dwSize);
		MDEL_MSG(pSend);

	}
	return 0;
}

//------------------------------------------------------------------------
// 发送信息给群成员
//------------------------------------------------------------------------
DWORD	PlayerSession::HandleSendGroupChatMsg(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SendGroupChatMsg);
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}
	return 0;
}