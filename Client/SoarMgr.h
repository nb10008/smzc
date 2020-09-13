#pragma once

#include "SoarSkillData.h"
#include "SoarCastingData.h"
struct tagStartNPCTalkEvent;
class SoarEquipFrame;
struct tagNS_PracticeSoaringSkill;
struct tagNS_ResetSoarSkillLearnTimes;

const INT32 SOAR_SKILL_LEARN_TIMES	= 100; 

class SoarMgr
{
public:
	SoarMgr(void);
	~SoarMgr(void);

	static SoarMgr* Inst();
	static void RegisterFrames(GameFrameMgr* pMgr);
	void Init();
	void Destroy();
	const tagSoarSkillData* GetSoarSkillData(DWORD dwSkillTypeID) const;
	int GetSoarSkillNum() const;
	const std::vector<DWORD>& GetSoarSkillVector() const;
	void SendPracticeSoarSkill(DWORD dwSkillID);
	DWORD OnNS_PracticeSoarSkill(tagNS_PracticeSoaringSkill* pNetCmd,DWORD);
	DWORD GetSoarSkillDatabyPos(int pos);
	DWORD OnNS_ResetSoarSkillLearnTimes(tagNS_ResetSoarSkillLearnTimes* pNetCmd,DWORD);
	
	
	//  ---------------------------------------熔铸
	const tagSoarCastingProto* GetSoarCastingData(DWORD dwID) const;
	const SoarCastingData::SoarCastingDataMap& GetSoarCastingDataMap() const;

private:
	DWORD OnOpenSoarSkillFrame(tagGameEvent* pEvent);
	DWORD OnCloseSoarSkillFrame(tagGameEvent* pEvent);
	DWORD OnOpenSoarEquipFrame(tagStartNPCTalkEvent* pEvt);

private:
	TObjRef<GameFrameMgr> m_pMgr;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;
	TObjRef<NetSession> m_pSession;
	TSFPTrunk<SoarMgr> m_trunk;

	SoarSkillData m_soarSkillData;

	SoarCastingData m_soarCastingData;

	std::vector<DWORD> m_vecSoarSkills;

	DWORD m_dwCurSkillID;			// 记录当前玩家学习的技能
	INT   m_nCurSkillLv;			// 记录当前玩家的技能等级
};
