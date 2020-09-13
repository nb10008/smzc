// 家族成员
#pragma once
#include "../WorldDefine/family_define.h"

class FamilyMember
{
public:
	typedef TMap<DWORD, tagFamilyMember*> FamilyMemberMap;

public:
	FamilyMember()
	{
		m_mapMember.Clear();
	}
	~FamilyMember()
	{
		Destory();
	}

	VOID Destory();
	VOID Update();

	VOID AddMember(DWORD dwFamilyID, DWORD dwRoleID, DWORD dwJoinTime);
	VOID AddMember(tagFamilyMember *pFamilyMember);

	VOID KickMember(DWORD dwRoleID);

	VOID SetMemberLeader(DWORD dwRoleID, BOOL bLeader);

	VOID MakeFriendWithOthers(DWORD dwRoleID);

	template <typename UnitOperate>
	VOID ForEachFamilyMember(UnitOperate fun);

	BOOL IsMemberExist(DWORD dwRoleID)
	{
		return m_mapMember.IsExist(dwRoleID);
	}

	DWORD GetFamilyMemberCnt()
	{
		return m_mapMember.Size();
	}

	DWORD GetFamilyAllMember(DWORD* pIDStr);

	DWORD GetMemberJoinTime(DWORD dwRoleID);

	VOID SetQuestRewardedFlag(DWORD dwRoleID, BOOL bState) 
	{
		if (P_VALID(m_mapMember.Peek(dwRoleID))) 
			m_mapMember.Peek(dwRoleID)->bRewarded = bState;
	}

	BOOL IsMemberRewarded(DWORD dwRoleID)
	{ 
		if (P_VALID(m_mapMember.Peek(dwRoleID))) 
			return m_mapMember.Peek(dwRoleID)->bRewarded;
		else
			return TRUE;
	}

	VOID ClearQuestRewardedFlag();

	tagFamilyMember* GetFamilyMember(DWORD dwMemberID)
	{
		return m_mapMember.Peek(dwMemberID);
	}
private:
	FamilyMemberMap m_mapMember;
};


template <typename UnitOperate>
VOID FamilyMember::ForEachFamilyMember(UnitOperate fun)
{
 	if (m_mapMember.Size() == 0)
 		return;
 	tagFamilyMember *pFamilyMember = NULL;
 	FamilyMemberMap::TMapIterator iterator = m_mapMember.Begin();
 	while(m_mapMember.PeekNext(iterator, pFamilyMember))
 	{
 		if (!P_VALID(pFamilyMember))
 			continue;
 		fun(pFamilyMember);
 	}
}
