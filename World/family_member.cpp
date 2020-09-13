// 家族成员
#include "stdafx.h"
#include "../WorldDefine/family_define.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/msg_social.h"
#include "../ServerDefine/msg_family.h"
#include "family_member.h"
#include "role_mgr.h"
#include "role.h"
#include "social_mgr.h"

struct SafeDelMember 
{
	VOID operator() (tagFamilyMember* pFamilyMember)
	{
		SAFE_DEL(pFamilyMember);
	}
};

struct UpdateMember 
{
	VOID operator() (tagFamilyMember* pFamilyMember)
	{
		if (!P_VALID(pFamilyMember))
			return;
	}
};

struct MakeFriend
{
	MakeFriend(DWORD dwRoleID)
		:m_dwRoleID(dwRoleID)
	{
	}
	VOID operator() (tagFamilyMember* pFamilyMember)
	{
		if (!P_VALID(pFamilyMember))
			return;
		Role* pRole = g_roleMgr.GetRolePtrByID(m_dwRoleID);
		Role* pMember = g_roleMgr.GetRolePtrByID(pFamilyMember->dwRoleID);
		if (!P_VALID(pRole) || !P_VALID(pMember))
			return;

		tagNC_RoleMakeFriend send2Role;
		tagNC_RoleMakeFriend send2Member;
		send2Role.dwDestRoleID = pFamilyMember->dwRoleID;
		send2Member.dwDestRoleID = m_dwRoleID;
		g_socialMgr.MakeFriend(m_dwRoleID, &send2Role, FALSE);
		g_socialMgr.MakeFriend(pFamilyMember->dwRoleID, &send2Member, FALSE);
	}
	DWORD m_dwRoleID;
};

VOID FamilyMember::Destory()
{
	ForEachFamilyMember(SafeDelMember());
	m_mapMember.Clear();
}
VOID FamilyMember::Update()
{
	ForEachFamilyMember(UpdateMember());
}

VOID FamilyMember::AddMember(DWORD dwFamilyID, DWORD dwRoleID, DWORD dwJoinTime)
{
	tagFamilyMember *pFamilyMember = new tagFamilyMember(dwRoleID, dwFamilyID, dwJoinTime, FALSE, FALSE);
	AddMember(pFamilyMember);
}
VOID FamilyMember::AddMember(tagFamilyMember *pFamilyMember)
{
	if (P_VALID(pFamilyMember))
		m_mapMember.Add(pFamilyMember->dwRoleID, pFamilyMember);
}
VOID FamilyMember::KickMember(DWORD dwRoleID)
{
	tagFamilyMember* pFamilyMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pFamilyMember))
		return;
	m_mapMember.Erase(dwRoleID);
	SAFE_DEL(pFamilyMember);
}

VOID FamilyMember::SetMemberLeader(DWORD dwRoleID, BOOL bLeader)
{
	tagFamilyMember* pFamilyMember = m_mapMember.Peek(dwRoleID);
	if (!P_VALID(pFamilyMember))
		return;
	pFamilyMember->bLeader = bLeader;

	tagNDBC_SetLeader send;
	send.dwRoleID = dwRoleID;
	send.bLeader = bLeader;
	g_dbSession.Send(&send, send.dwSize);
}

VOID FamilyMember::MakeFriendWithOthers(DWORD dwRoleID)
{
	ForEachFamilyMember(MakeFriend(dwRoleID));
}

DWORD FamilyMember::GetMemberJoinTime(DWORD dwRoleID)
{
	if(!m_mapMember.IsExist(dwRoleID))
		return 0;

	tagFamilyMember* pMember = m_mapMember.Peek(dwRoleID);
	if(!P_VALID(pMember))
		return 0;

	return pMember->dwJoinTime;
}

VOID FamilyMember::ClearQuestRewardedFlag()
{
	FamilyMemberMap::TMapIterator it = m_mapMember.Begin();
	tagFamilyMember* pMem = NULL;
	while(m_mapMember.PeekNext(it, pMem))
	{
		if (!P_VALID(pMem))
		{
			continue;
		}

		SetQuestRewardedFlag(pMem->dwRoleID, FALSE);
	}
}

DWORD FamilyMember::GetFamilyAllMember(DWORD* pIDStr)
{
	if (!P_VALID(pIDStr))
	{
		return 0;
	}

	DWORD dwCount = GetFamilyMemberCnt();
	if (dwCount <= 0)
	{
		return 0;
	}

	FamilyMemberMap::TMapIterator it = m_mapMember.Begin();
	DWORD dwMemberID = 0;
	tagFamilyMember* pMember = NULL;
	DWORD dwNum = 0;
	while (m_mapMember.PeekNext(it, dwMemberID, pMember))
	{
		if (!P_VALID(dwMemberID) || !P_VALID(pMember) || !P_VALID(pMember->dwRoleID))
		{
			continue;
		}

		pIDStr[dwNum]= pMember->dwRoleID;
		dwNum++;
	}

	return dwNum;
}