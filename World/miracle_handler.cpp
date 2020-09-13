//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: marriage_handler.cpp
// author: songg
// actor:
// data: 2010-02-02
// last:
// brief: 仙界修行的消息函数
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "player_session.h"
#include "map.h"
#include "role.h"
#include "role_mgr.h"
#include "../WorldDefine/msg_miracle.h"
#include "../ServerDefine/msg_miracle.h"

//////////////////////////////////////////////////////////////////////////
//练习仙界技能
//////////////////////////////////////////////////////////////////////////
DWORD PlayerSession::HandlePracticeMiracleSkill(tagNetCmd* pCmd)
{
	// 接收网络消息
	MGET_MSG(pRecv, pCmd, NC_PracticeMiracleSkill);

	// 获取角色
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		IMSG(_T("Cant't find the role object of the session when tackling the LearnMiracleSkill message\r\n"));
		return GT_INVALID;
	}

	tagNS_PracticeMiracleSkill send2c;

	send2c.dwErrCode = pRole->PracticeMiracleSkill(pRecv->dwSkillID);

	send2c.n16CurPrcNum = pRole->GetMiraclePrcNum();
	SendMessage(&send2c,send2c.dwSize);

	//如果成功
	if(send2c.dwErrCode == E_Miracle_Success)
	{
		tagNDBC_SaveMiraclePrcNum send2db;
		send2db.dwRoleID = pRole->GetID();
		send2db.n16MiraclePrcNum = pRole->GetMiraclePrcNum();
		g_dbSession.Send(&send2db,send2db.dwSize);

	}

	return send2c.dwErrCode;
}


//////////////////////////////////////////////////////////////////////////
//增加仙界技能点儿
//////////////////////////////////////////////////////////////////////////
DWORD PlayerSession::HandleUseAddMiraclePrcNumItem(tagNetCmd* pCmd)
{
	
	// 接收网络消息
	MGET_MSG(pRecv, pCmd, NC_UseAddMiraclePrcNumItem);

	// 获取角色
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		IMSG(_T("Cant't find the role object of the session when tackling the UseAddMiraclePrcNumItem message\r\n"));
		return GT_INVALID;
	}

	tagNS_UseAddMiraclePrcNumItem send2c;

	send2c.dwErrCode = pRole->UseAddMiraclePrcNumItem(pRecv->n64ItemID);
	send2c.n16CurPrcNum = pRole->GetMiraclePrcNum();
	SendMessage(&send2c,send2c.dwSize);

	if(send2c.dwErrCode == E_Miracle_Success)
	{
		tagNDBC_SaveMiraclePrcNum send2db;
		send2db.dwRoleID = pRole->GetID();
		send2db.n16MiraclePrcNum = pRole->GetMiraclePrcNum();
		g_dbSession.Send(&send2db,send2db.dwSize);

	}

	return send2c.dwErrCode;
}