// 家族
#include "stdafx.h"
#include "..\WorldDefine\family_define.h"
#include "..\ServerDefine\msg_family.h"
#include "..\WorldDefine\msg_family.h"
#include "role.h"
#include "family_member.h"
#include "family.h"
#include "family_join.h"
#include "family_mgr.h"
#include "family_sprite.h"
#include "family_quest.h"
#include "family_sprite_mgr.h"
#include "db_session.h"

Family::Family(tagFamilyLoad& sFamilyLoad)
{
	tstring strName(sFamilyLoad.strFamilyName);
	SetFamilyName(strName);
	m_FamilyAtt = sFamilyLoad.sFamilyAtt;
	m_FamilyQuest.Init(this);
}

VOID Family::Update()
{
	m_FamilyMem.Update();
	m_FamilyQuest.Update();
}
VOID Family::Destory()
{
	
}

VOID Family::AddMember(DWORD dwRoleID, DWORD dwJoinTime)
{
	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
	if (P_VALID(pRoleInfo))
	{
		pRoleInfo->dwFamilyID = m_FamilyAtt.dwFamilyID;
		m_FamilyMem.AddMember(m_FamilyAtt.dwFamilyID, dwRoleID, dwJoinTime);
	}
}
VOID Family::KickMember(DWORD dwRoleID)
{
	tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
	if (P_VALID(pRoleInfo))
	{
		pRoleInfo->dwFamilyID = GT_INVALID;
	}

	m_FamilyMem.KickMember(dwRoleID);
}

VOID Family::SetMemberLeader(DWORD dwRoleID)
{
	if (m_FamilyMem.IsMemberExist(m_FamilyAtt.dwLeaderID))
		m_FamilyMem.SetMemberLeader(m_FamilyAtt.dwLeaderID, FALSE);

	m_FamilyAtt.dwLeaderID = dwRoleID;

	if (m_FamilyMem.IsMemberExist(dwRoleID))
		m_FamilyMem.SetMemberLeader(dwRoleID, TRUE);

	SendFamilyAtt2DB(m_FamilyAtt);
}

VOID Family::OnMemberKillMonster(DWORD dwRoleID, DWORD dwMonsterID, INT nLevel, DWORD dwMapCrc)
{
	m_FamilyQuest.OnAnyMemberKillMonster(dwMonsterID, nLevel, dwMapCrc);
}

//----------------------------------------------------------------------------------
//获得家族妖精等级
UINT16 Family::GetFamilySpriteLevel()
{ 
	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite(m_FamilyAtt.dwFamilyID);
	if(P_VALID(pFamilySprite)) 
		return pFamilySprite->GetLevel(); 

	return 0;
}

//----------------------------------------------------------------------------------
//获得一个妖精
//FamilySprite* Family::CreateFamilySprite( DWORD dwFamilyID, TCHAR *pszName)
//{
//	return g_FamilySpriteMgr.CreateFamilySprite( dwFamilyID, pszName );
//}
////----------------------------------------------------------------------------------
////设置家族妖精
//VOID Family::SetFamilySprite( FamilySprite* pFamilySprite )
//{
//	if(P_VALID(pFamilySprite)) 
//		m_pFamilySprite = pFamilySprite;
//}
//----------------------------------------------------------------------------------

BOOL Family::IsFamilyInitOK()
{
	BOOL bRet = TRUE;

	// 家族成员
	if(GetFamilyMember().GetFamilyMemberCnt() <= 0)
	{
		ILOG->Write(_T("Family<id: %u> members load failed! Please check db! \r\n"), GetFamilyAtt().dwFamilyID);
		bRet = FALSE;
	}

	// 家族任务

	// 家族妖精

	return bRet;
}

VOID Family::LoadFamilyMember(tagFamilyMember* pFamilyMember)
{
	if (!P_VALID(pFamilyMember))
		return;

	tagFamilyMember* pNewFamilyMember = new tagFamilyMember();
	*pNewFamilyMember = *pFamilyMember;
	m_FamilyMem.AddMember(pNewFamilyMember);
}

VOID Family::SendRoleJoinFamily2DB(DWORD dwMemberID)
{
	tagFamilyMember* pFamilyMember = m_FamilyMem.GetFamilyMember(dwMemberID);
	if (P_VALID(pFamilyMember))
	{
		tagNDBC_JoinFamily send;
		send.sFamilyMemInfo = *pFamilyMember;
		g_dbSession.Send(&send, send.dwSize);
	}
}

VOID Family::SendSetLeader2DB(DWORD dwRoleID, BOOL bLeader)
{
	tagNDBC_SetLeader setLeader;
	setLeader.dwRoleID = dwRoleID;
	setLeader.bLeader = bLeader;
	g_dbSession.Send(&setLeader, setLeader.dwSize);
}

VOID Family::SendLeaveFamily2DB(DWORD dwRoleID)
{
	tagNDBC_LeaveFamily leaveFamily;
	leaveFamily.dwRoleID = dwRoleID;
	g_dbSession.Send(&leaveFamily, leaveFamily.dwSize);
}

VOID Family::ChangeFamilyActive(INT nActive)
{
	GetFamilyAtt().SetFamilyActive(nActive);

	// 活跃度发生变化，发给所有在线家族成员
	tagNS_SyncFamilyActive send;
	send.nActive = nActive;
	GetFamilyMember().ForEachFamilyMember(SendMsg2Member((tagNetCmd*)&send));

	tagNDBC_SaveFamilyActive SaveFamilyActive;
	SaveFamilyActive.dwFamilyID = GetFamilyAtt().dwFamilyID;
	SaveFamilyActive.nActive = GetFamilyAtt().nActive;
	g_dbSession.Send(&SaveFamilyActive, SaveFamilyActive.dwSize);
}

VOID Family::SendFamilyAtt2DB(tagFamilyAtt& sFamilyAtt)
{
	tagNDBC_SaveFamilyAtt send;
	send.sFamilyAtt = sFamilyAtt;
	g_dbSession.Send(&send, send.dwSize);
}
