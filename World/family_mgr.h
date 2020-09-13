// 家族管理器
#pragma once
#include "family_join.h"

class Family;
class Role;
class FamilyJoin;
struct tagRoleInfo;
class FamilySprite;
struct tagFamilyload;

class FamilyMgr
{
public:
	FamilyMgr();
	~FamilyMgr();

	BOOL	Init();
	VOID	Update();
	VOID	Destroy();
	VOID	SetQuestInitOK();

	INT32	GetFamilyNum()
	{
		return m_mapFamily.Size();
	}
	Family*	GetFamily(DWORD dwFamilyID)
	{
		return m_mapFamily.Peek(dwFamilyID);
	}

	DWORD	CanCreateFamily(Role* pCreator, LPCTSTR szFamilyName);
	DWORD	CreateFamily(Role* pCreator, LPCTSTR szFamilyName);

	DWORD	DismissFamily(Role* pLeader);

	VOID SendFamilyProcFailed2Client(Role* pRole, DWORD dwErrorCode);

	template<typename UnitOperate>
	VOID ForEachFamily(UnitOperate fun);

	FamilyJoin& GetFamilyJoin()
	{
		return m_FamilyJoin;
	}

	DWORD InviteRoleJoin(Role* pInviter, DWORD dwInviteeID);
	DWORD ApplyRoleJoin(Role* pApply, DWORD dwMemberID);

	VOID AddRole2Family(Family* pFamily, Role* pRole);

	DWORD LeaveFamily(Role* pRole);
	DWORD KickFromFamily(Role* pKicker, DWORD dwRoleID);
	
	DWORD LeaderHandOver(Role* pLeader, DWORD dwHeritorID);

	DWORD GetFamilyName(Role* pRole, DWORD dwFamilyID);
	DWORD GetFamilyInfo(Role* pRole, DWORD dwFamilyID);

	VOID LoadAllFamily(tagFamilyLoad& sFamilyLoad);
	VOID LoadFamilyMember(INT nMemNum, tagFamilyMember* pFamilyMember);

	BOOL IsFamilyInitOK();

	VOID SendAllFamilyInfo2DB();

	VOID OnMemberLogin(Role* pRole);
private:
	VOID SendCreateFamily2DB(tagFamilyAtt& sFamilyAtt, LPCTSTR pFamilyName);
	VOID SendDismissFamily2DB(DWORD dwFamilyID);

public:
	typedef TMap<DWORD, Family*> FamilyMap;

private:
	FamilyMap m_mapFamily;
	FamilyJoin m_FamilyJoin;
};

template<typename UnitOperate>
VOID FamilyMgr::ForEachFamily(UnitOperate fun)
{
	if (m_mapFamily.Size() == 0)
		return;

	Family* pFamily = NULL;
	FamilyMap::TMapIterator iterator = m_mapFamily.Begin();
	while (m_mapFamily.PeekNext(iterator, pFamily))
	{
		if (!P_VALID(pFamily))
			continue;
		fun(pFamily);
	}
}

struct SendMsg2Member
{
	SendMsg2Member(tagNetCmd * pNetCmd)
		:m_pNetCmd(pNetCmd)
	{

	}
	VOID operator()(tagFamilyMember* pFamilyMember)
	{
		if (P_VALID(pFamilyMember) && P_VALID(m_pNetCmd))
		{
			Role* pMember = g_roleMgr.GetRolePtrByID(pFamilyMember->dwRoleID);
			if (P_VALID(pMember))
				pMember->SendMessage(m_pNetCmd, m_pNetCmd->dwSize);
		}
	}
private:
	tagNetCmd * m_pNetCmd;
};

extern FamilyMgr g_FamilyMgr;
