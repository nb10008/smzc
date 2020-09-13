// 家族
#pragma once
#include "family_quest.h"
#include "family_member.h"

struct tagFamilyAtt;
class FamilyMember;
class FamilySprite;
struct tagFamilyLoad;

class Family
{
public:
	Family(bool bCreateNew = false)
	{
		m_bResetFamilyActive = FALSE;
		//m_pFamilySprite = NULL;
		m_FamilyQuest.Init(this, bCreateNew);
	}
	Family(tagFamilyLoad& sFamilyLoad);
	~Family()
	{
		Destory();
	}

	VOID	Update();
	VOID	Destory();

	VOID AddMember(DWORD dwRoleID, DWORD dwJoinTime);
	VOID KickMember(DWORD dwRoleID);

	VOID SetMemberLeader(DWORD dwRoleID);
	VOID SetQuestInitOK() { m_FamilyQuest.SetInitOK(); }

	BOOL MemberIsLeader(DWORD dwRoleID)
	{
		tagFamilyMember* pFamilyMember = GetFamilyMember().GetFamilyMember(dwRoleID);
		if (P_VALID(pFamilyMember))
			return pFamilyMember->bLeader;
		return FALSE;
	}

	VOID	OnMemberKillMonster(DWORD dwRoleID, DWORD dwMonsterID, INT nLevel, DWORD dwMapCrc);

	FamilyMember& GetFamilyMember()
	{
		return m_FamilyMem;
	}

	tagFamilyAtt& GetFamilyAtt()
	{
		return m_FamilyAtt;
	}

	VOID SetFamilyName(tstring strName)
	{
		m_strName = strName;
	}
	LPCTSTR GetFamilyName()
	{
		return m_strName.c_str();
	}

	DWORD			GetID()				{ return m_FamilyAtt.dwFamilyID; }

	FamilyQuest&	GetFamilyQuest()	{ return m_FamilyQuest; }


	//获得一个妖精，没有则造一个
	//FamilySprite* CreateFamilySprite( DWORD dwFamilyID, TCHAR *pszName );

	//设置家族妖精
	//VOID SetFamilySprite( FamilySprite* pFamilySprite );

	//获得家族妖精等级
	UINT16	GetFamilySpriteLevel();

	//获得家族妖精
	//FamilySprite* GetFamilySprite(){return m_pFamilySprite;}

	BOOL IsFamilyInitOK();
	VOID LoadFamilyMember(tagFamilyMember* pFamilyMember);

	VOID SendRoleJoinFamily2DB(DWORD dwMemberID);
	VOID SendSetLeader2DB(DWORD dwRoleID, BOOL bLeader);
	VOID SendLeaveFamily2DB(DWORD dwRoleID);
	VOID SendFamilyAtt2DB(tagFamilyAtt& sFamilyAtt);
	VOID ChangeFamilyActive(INT nActive);

	BOOL IsResetFamilyActive()
	{
		return m_bResetFamilyActive;
	}
	VOID SetResetFamilyActive(BOOL bResetFamilyActive)
	{
		m_bResetFamilyActive = bResetFamilyActive;
	}
	VOID ResetFamilyActive()
	{
		ChangeFamilyActive(0);
	}

private:
	// 家族名称
	tstring					m_strName;	
	
	// 家族属性
	tagFamilyAtt			m_FamilyAtt;

	// 家族成员
	FamilyMember			m_FamilyMem;

	// 家族任务
	FamilyQuest				m_FamilyQuest;

	//家族妖精
	//FamilySprite			*m_pFamilySprite;

	BOOL m_bResetFamilyActive;
};

