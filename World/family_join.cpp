// 加入家族，包含申请加入和邀请加入
#include "stdafx.h"
#include "../WorldDefine/family_define.h"
#include "../WorldDefine/msg_family.h"
#include "family_join.h"
#include "role.h"

FamilyJoin::FamilyJoin()
{
	m_mapJoinReq.Clear();
}
FamilyJoin::~FamilyJoin()
{
	m_mapJoinReq.Clear();
}

VOID FamilyJoin::Update()
{
	if (m_mapJoinReq.Size() <=0)
		return;
	JoinReqMap::TMapIterator iterator = m_mapJoinReq.Begin();
	tagJoinFamilyReq JoinReq;
	while(m_mapJoinReq.PeekNext(iterator, JoinReq))
	{
		if (CalcTimeDiff(GetCurrentDWORDTime(), JoinReq.dwAddReqTime) >= JOIN_FAMILY_REQ_TIMEOUT)
		{
			OnReqTimeOut(JoinReq);
			RemoveJoinReq(JoinReq.dwJoinerID);
		}
	}
}

VOID FamilyJoin::AddJoinReq(tagJoinFamilyReq& JoinReq)
{
	m_mapJoinReq.Add(JoinReq.dwJoinerID, JoinReq);
}
VOID FamilyJoin::RemoveJoinReq(DWORD dwJoinerID)
{
	m_mapJoinReq.Erase(dwJoinerID);
}
BOOL FamilyJoin::IsJoinReqExist(DWORD dwJoinerID)
{
	return m_mapJoinReq.IsExist(dwJoinerID);
}
VOID FamilyJoin::OnReqTimeOut(tagJoinFamilyReq& JoinReq)
{
	DWORD dwRoleID = JoinReq.bInviteJoin?JoinReq.dwMemberID:JoinReq.dwJoinerID;
	Role* pMember = g_roleMgr.GetRolePtrByID(JoinReq.dwMemberID);
	Role* pJoiner = g_roleMgr.GetRolePtrByID(JoinReq.dwJoinerID);
	if (JoinReq.bInviteJoin)
	{
		tagNS_FamilyJoinReqRes send;
		send.dwInviteeID = JoinReq.dwJoinerID;
		send.dwInviterID = JoinReq.dwMemberID;
		send.dwErrorCode = E_FamilyJoin_Refuse;
		if(P_VALID(pMember)) pMember->SendMessage(&send, send.dwSize);
		send.dwErrorCode = E_FamilyJoin_TimeOut;
		if(P_VALID(pJoiner)) pJoiner->SendMessage(&send, send.dwSize);
	}
	else
	{
		tagNS_FamilyApplyReqRes send;
		send.dwDstRoleID = JoinReq.dwMemberID;
		send.dwErrorCode = E_FamilyApply_Refuse;
		if(P_VALID(pJoiner)) pJoiner->SendMessage(&send, send.dwSize);
		send.dwErrorCode = E_FamilyApply_TimeOut;
		if(P_VALID(pMember)) pMember->SendMessage(&send, send.dwSize);
	}
}
