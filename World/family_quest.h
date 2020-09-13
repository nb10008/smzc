#pragma once
//#include "world.h"
//#include "../WorldDefine/family_define.h"

struct tagFamilyQuestLoadData;

// 根据家族精灵等级获得家族任务个数
#define GetFamilyQuestNum(spirite_level, num) \
	if(spirite_level>=1 && spirite_level<20) num = 1; \
	else if(spirite_level>=20 && spirite_level<40) num = 2; \
	else if(spirite_level>=40 && spirite_level<60) num = 3; \
	else if(spirite_level>=60) num = 4;

struct tagFamilyQuestDetail : tagFamilyQuest
{
	const tagFamilyQuestProto*		m_pQuestProto;

	tagFamilyQuestDetail()
	{
		ZeroMemory(this, sizeof(this));
	}
};

class Family;
class FamilyQuest
{
	// 零点刷新相关操作
	VOID	SetRenewFlag();
	
	// 计算当前家族任务的总完成度
	INT		CalCompletePercent();

public:
	~FamilyQuest(){ Destroy(); }

	VOID	Init(Family* p, bool bNewFamily = false);
	VOID	Init(tagFamilyQuestLoadData* pData);
	VOID	Destroy();
	
	VOID	SaveFamilyQuestDetail();
	VOID	Update();

	VOID	RenewQuest();
	INT		GetYesterdayFamilyQuestProcess() { return m_nProcess; }
	VOID	FillFamilyQuestData(BYTE*);
	BYTE	GetQuestNum() { return m_byQuestNum; }
	VOID	SetInitOK() { m_bInitOk = TRUE; }

	VOID	IncFamilySpiriteExp();

	// 杀怪计数
	VOID	OnAnyMemberKillMonster(DWORD dwMonsterID, INT nMonsterLevel, DWORD dwMapCrc);
	// 上缴物品
	DWORD	OnAnyMemberGiveInItem(DWORD dwRoleID, DWORD dwQuestID, DWORD dwTypeID, INT64 n64Serial, INT nNum, INT& nSum);

	DWORD	CanGetReward(DWORD dwRoleID);
	BOOL	CanGetItemRewardI();
	BOOL	CanGetItemRewardII();
	
private:
	Family*					m_pFamily;						// 所属家族
	BOOL						m_bInitOk;						// 是否以成功初始化
	tagFamilyQuest*		m_pQuests;						// 进行中的家族任务
	BYTE						m_byQuestNum;				// 进行中的家族任务个数
	INT							m_nProcess;					// 上期的任务完成度
	bool							m_bIssueState;				// 发布状态,false未发布 true已发布
	tagDWORDTime		m_dwLatestRenewTime;	// 一个防止重复刷新任务的时间
	DWORD					m_dwLatestSaveTick;		// 最后一次保存数据库的tick

	CRITICAL_SECTION	m_renewlock;
};

inline VOID FamilyQuest::FillFamilyQuestData(BYTE* pTargetMem)
{
	if(!P_VALID(pTargetMem))
		return;

	tagFamilyQuest* pMem = (tagFamilyQuest*)pTargetMem;
	for (int i=0;i<m_byQuestNum;++i)
	{
		if(P_VALID(m_pQuests+i))
		{
			IFASTCODE->MemCpy(pMem, m_pQuests+i, sizeof(tagFamilyQuest));
			++pMem;
		}
	}
}