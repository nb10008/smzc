//-----------------------------------------------------------------------------
// brief: 家族系统消息处理
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/creature_define.h"
#include "../WorldDefine/msg_family.h"
#include "../WorldDefine/family_define.h"
#include "../ServerDefine/login_issue.h"
#include "..\ServerDefine\family_define.h"
#include "role_mgr.h"
#include "role.h"
#include "family_member.h"
#include "family.h"
#include "family_join.h"
#include "family_mgr.h"
#include "player_session.h"
#include "login_session.h"

DWORD PlayerSession::HandleCreateFamily(tagNetCmd * pCmd)
{
	DWORD dwErrorCode = GT_INVALID;

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
		return GT_INVALID;
	
	MGET_MSG(pRecv, pCmd, NC_CreateFamily);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 判断NPC
	dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_FamilyNPC);
	if (dwErrorCode != E_Success && dwErrorCode != GT_INVALID)
	{
		g_FamilyMgr.SendFamilyProcFailed2Client(pRole, dwErrorCode);
		return dwErrorCode;
	}

	// 获取名字 -- 确保有字符串结束符
	INT32 nNameTCHAR = (pRecv->dwSize - FIELD_OFFSET(tagNC_CreateFamily, szFamilyName)) / sizeof(TCHAR);
	tstring strFamilyName(pRecv->szFamilyName, nNameTCHAR);

	dwErrorCode = g_FamilyMgr.CanCreateFamily(pRole, strFamilyName.c_str());
	if (dwErrorCode != E_FAMILY_SUCCESS && dwErrorCode != GT_INVALID)
	{
		g_FamilyMgr.SendFamilyProcFailed2Client(pRole, dwErrorCode);
		return dwErrorCode;
	}
	
	tagNWL_CreateFamily send;
	memset(send.szFamilyName,0,sizeof(send.szFamilyName));
	send.dwAccountID = m_dwAccountID;
	send.dwRoleID = pRole->GetID();
	send.dwNPCID = pRecv->dwNPCID;
	_tcscpy_s(send.szFamilyName, X_SHORT_NAME, strFamilyName.c_str());
	g_loginSession.Send(&send, send.dwSize);

	return dwErrorCode;
}
DWORD PlayerSession::HandleDismissFamily(tagNetCmd * pCmd)
{
	DWORD dwErrorCode = GT_INVALID;
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
		return GT_INVALID;

	MGET_MSG(pRecv, pCmd, NC_CreateFamily);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 判断NPC
	dwErrorCode = pRole->CheckFuncNPC(pRecv->dwNPCID, EFNPCT_FamilyNPC);
	if (dwErrorCode != E_Success && dwErrorCode != GT_INVALID)
	{
		g_FamilyMgr.SendFamilyProcFailed2Client(pRole, dwErrorCode);
		return dwErrorCode;
	}

	dwErrorCode = g_FamilyMgr.DismissFamily(pRole);
	if (dwErrorCode != E_FAMILY_SUCCESS && dwErrorCode != GT_INVALID)
	{
		g_FamilyMgr.SendFamilyProcFailed2Client(pRole, dwErrorCode);
		return dwErrorCode;
	}

	return dwErrorCode;
}
DWORD PlayerSession::HandleJoinFamilyReq(tagNetCmd * pCmd)
{
	DWORD dwErrorCode = GT_INVALID;
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
		return GT_INVALID;

	MGET_MSG(pRecv, pCmd, NC_FamilyJoinReq);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	DWORD dwInviteeID = GT_INVALID;
	if (pRecv->dwDstRoleID == GT_INVALID)
	{
		INT32 nNameTCHAR = (pRecv->dwSize - FIELD_OFFSET(tagNC_FamilyJoinReq, szDstRoleName)) / sizeof(TCHAR);
		tstring strRoleName(pRecv->szDstRoleName, nNameTCHAR);
		dwInviteeID = g_roleMgr.GetRoleIDByNameCrc(TObjRef<Util>()->Crc32(strRoleName.c_str()));
	}
	else
	{
		dwInviteeID = pRecv->dwDstRoleID;
	}

	dwErrorCode = g_FamilyMgr.InviteRoleJoin(pRole, dwInviteeID);
	if (dwErrorCode != E_FAMILY_SUCCESS && dwErrorCode != GT_INVALID)
	{
		g_FamilyMgr.SendFamilyProcFailed2Client(pRole, dwErrorCode);
		return dwErrorCode;
	}

	return dwErrorCode;
}
DWORD PlayerSession::HandleJoinFamilyReqRes(tagNetCmd * pCmd)
{
	DWORD dwErrorCode = GT_INVALID;
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
		return GT_INVALID;

	MGET_MSG(pRecv, pCmd, NC_FamilyJoinReqRes);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	if (!g_FamilyMgr.GetFamilyJoin().IsJoinReqExist(pRole->GetID()))
		return E_FamilyJoin_TimeOut;

	g_FamilyMgr.GetFamilyJoin().RemoveJoinReq(pRole->GetID());

	Family* pFamily = g_FamilyMgr.GetFamily(pRecv->dwDstFamilyID);
	if (!P_VALID(pFamily))
		return GT_INVALID;

	if (pRecv->dwErrorCode == E_FAMILY_SUCCESS)
		g_FamilyMgr.AddRole2Family(pFamily, pRole);

	Role* pInviter = g_roleMgr.GetRolePtrByID(pRecv->dwDstRoleID);
	if (P_VALID(pInviter))
	{
		tagNS_FamilyJoinReqRes send;
		send.dwErrorCode = pRecv->dwErrorCode;
		send.dwInviteeID = pRole->GetID();
		send.dwInviterID = pRecv->dwDstRoleID;
		pInviter->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

DWORD PlayerSession::HandleFamilyApply(tagNetCmd * pCmd)
{
	DWORD dwErrorCode = GT_INVALID;
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
		return GT_INVALID;

	MGET_MSG(pRecv, pCmd, NC_FamilyApply);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	DWORD dwApplyRoleID = GT_INVALID;
	if (pRecv->dwApplyRoleID == GT_INVALID)
	{
		INT32 nNameTCHAR = (pRecv->dwSize - FIELD_OFFSET(tagNC_FamilyJoinReq, szDstRoleName)) / sizeof(TCHAR);
		tstring strRoleName(pRecv->szDstRoleName, nNameTCHAR);
		dwApplyRoleID = g_roleMgr.GetRoleIDByNameCrc(TObjRef<Util>()->Crc32(strRoleName.c_str()));
	}
	else
	{
		dwApplyRoleID = pRecv->dwApplyRoleID;
	}

	dwErrorCode = g_FamilyMgr.ApplyRoleJoin(pRole, dwApplyRoleID);
	if (dwErrorCode != E_FAMILY_SUCCESS && dwErrorCode != GT_INVALID)
	{
		g_FamilyMgr.SendFamilyProcFailed2Client(pRole, dwErrorCode);
		return dwErrorCode;
	}

	return dwErrorCode;
}

DWORD PlayerSession::HandleFamilyApplyRes(tagNetCmd * pCmd)
{
	DWORD dwErrorCode = GT_INVALID;
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
		return GT_INVALID;

	MGET_MSG(pRecv, pCmd, NC_FamilyApplyRes);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	if (!g_FamilyMgr.GetFamilyJoin().IsJoinReqExist(pRecv->dwDstRoleID))
		return E_FamilyApply_TimeOut;

	g_FamilyMgr.GetFamilyJoin().RemoveJoinReq(pRecv->dwDstRoleID);

	Family* pFamily = g_FamilyMgr.GetFamily(pRecv->dwDstFamilyID);
	if (!P_VALID(pFamily))
		return GT_INVALID;

	Role* pApply = g_roleMgr.GetRolePtrByID(pRecv->dwDstRoleID);
	if (P_VALID(pApply))
	{
		if (pRecv->dwErrorCode == E_FAMILY_SUCCESS)
			g_FamilyMgr.AddRole2Family(pFamily, pApply);

		tagNS_FamilyApplyReqRes send;
		send.dwErrorCode = pRecv->dwErrorCode;
		send.dwDstRoleID = pRole->GetID();
		pApply->SendMessage(&send, send.dwSize);
	}

	return dwErrorCode;
}

DWORD PlayerSession::HandleLeaveFamily(tagNetCmd * pCmd)
{
	DWORD dwErrorCode = GT_INVALID;
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
		return GT_INVALID;

	MGET_MSG(pRecv, pCmd, NC_FamilyLeave);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	dwErrorCode = g_FamilyMgr.LeaveFamily(pRole);
	if (dwErrorCode != E_FAMILY_SUCCESS && dwErrorCode != GT_INVALID)
	{
		g_FamilyMgr.SendFamilyProcFailed2Client(pRole, dwErrorCode);
		return dwErrorCode;
	}

	return dwErrorCode;
}
DWORD PlayerSession::HandleKickFromFamily(tagNetCmd * pCmd)
{
	DWORD dwErrorCode = GT_INVALID;
	Role* pKicker = GetRole();
	if (!P_VALID(pKicker))
		return GT_INVALID;

	MGET_MSG(pRecv, pCmd, NC_FamilyKick);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	dwErrorCode = g_FamilyMgr.KickFromFamily(pKicker, pRecv->dwRoleID);
	if (dwErrorCode != E_FAMILY_SUCCESS && dwErrorCode != GT_INVALID)
	{
		g_FamilyMgr.SendFamilyProcFailed2Client(pKicker, dwErrorCode);
		return dwErrorCode;
	}

	return dwErrorCode;
}
DWORD PlayerSession::HandleHandOverFamily(tagNetCmd * pCmd)
{
	DWORD dwErrorCode = GT_INVALID;
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
		return GT_INVALID;

	MGET_MSG(pRecv, pCmd, NC_FamilyHandOver);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	dwErrorCode = g_FamilyMgr.LeaderHandOver(pRole, pRecv->dwRoleID);
	if (dwErrorCode != E_FAMILY_SUCCESS && dwErrorCode != GT_INVALID)
	{
		g_FamilyMgr.SendFamilyProcFailed2Client(pRole, dwErrorCode);
		return dwErrorCode;
	}

	return dwErrorCode;
}

DWORD PlayerSession::HandleGetFamilyName(tagNetCmd * pCmd)
{
	DWORD dwErrorCode = GT_INVALID;
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
		return GT_INVALID;

	MGET_MSG(pRecv, pCmd, NC_GetFamilyName);
	if (!P_VALID(pRecv))
		return GT_INVALID;
	
	dwErrorCode = g_FamilyMgr.GetFamilyName(pRole, pRecv->dwFamilyID);
	if (dwErrorCode != E_FAMILY_SUCCESS && dwErrorCode != GT_INVALID)
	{
		g_FamilyMgr.SendFamilyProcFailed2Client(pRole, dwErrorCode);
		return dwErrorCode;
	}

	return dwErrorCode;
}

DWORD PlayerSession::HandleGetFamilyInfo(tagNetCmd * pCmd)
{
	DWORD dwErrorCode = GT_INVALID;
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
		return GT_INVALID;

	MGET_MSG(pRecv, pCmd, NC_GetFamilyInfo);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	dwErrorCode = g_FamilyMgr.GetFamilyInfo(pRole, pRecv->dwFamilyID);
	if (dwErrorCode != E_FAMILY_SUCCESS && dwErrorCode != GT_INVALID)
	{
		g_FamilyMgr.SendFamilyProcFailed2Client(pRole, dwErrorCode);
		return dwErrorCode;
	}

	return dwErrorCode;
}
