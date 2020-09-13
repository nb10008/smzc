//-----------------------------------------------------------------------------
// Copyright (c) 2011 KONG.NET All rights reserved.
// filename: soul_handler.cpp
// author: liutingting
// actor:
// data: 2011-10-9
// last:
// brief: 
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "../WorldDefine/msg_soul.h"
#include "player_session.h"
#include "role.h"

//-----------------------------------------------------------------------------
// 激活神魔技能
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleActiveSoulSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ActiveSoulSkill);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_ActiveSoulSkill send;
	send.dwSkillID = pRecv->dwSkillID;
	send.dwResult = pRole->ActiveSoulSkill(pRecv->dwSkillID,send.dwTime);
	
	SendMessage( &send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------
// 延续神魔技能时间
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleContinueSoulSkill(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ContinueSoulSkill);
	
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_ContinueSoulSkill send;
	send.dwSkillID = pRecv->dwSkillID;
	send.dwResult = pRole->ContinueSoulSkill(pRecv->dwSkillID,pRecv->n64ItemID,send.dwTime);

	SendMessage( &send, send.dwSize);

	return 0;
}
