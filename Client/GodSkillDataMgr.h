#pragma once
#pragma warning(disable : 4715)

#include "GodSkillData.h"

struct tagOpenGodSkillEvent;
struct tagNS_PracticeMiracleSkill;
struct tagNS_ResetMiraclePrcNum;

class GodSkillDataMgr
{
public:
	enum EGodSkillLevel
	{
		EGSL_LowerImmortal = 1,
		EGSL_NormalImmortal = 2,
		EGSL_HighImmortal = 3
	};

	~GodSkillDataMgr(){}
	static GodSkillDataMgr* Inst();
	static void RegisterFrames(GameFrameMgr* pMgr);
	void Init();
	void Destroy();
	const tagGodSkillData* GetGodSkillData(DWORD dwSkillTypeID) const;
	int GetGodSkillNum() const;
	const GodSkillData::GodSkillIDSet& GetGodSkillSet() const;
	EGodSkillLevel GetGodSkillLevel(DWORD dwSkillID) const;
	const std::vector<DWORD>& GetSkills(EGodSkillLevel eLv) const;
	void SendPracticeMiracleSkill(DWORD dwSkillID);
private:
	TObjRef<GameFrameMgr> m_pMgr;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;
	TObjRef<NetSession> m_pSession;
	TSFPTrunk<GodSkillDataMgr> m_trunk;
	

	GodSkillData m_godSkillData;

	std::vector<DWORD> m_vecLowerSkill;
	std::vector<DWORD> m_vecNormalSkill;
	std::vector<DWORD> m_vecHighSkill;

	DWORD m_dwCurSkillID;			// 记录当前玩家学习的技能
	INT   m_nCurSkillLv;			// 记录当前玩家的技能等级
private:
	GodSkillDataMgr();
	void RegisterAllEventHandler();
	void RegisterAllNetCmd();
	void UnRegisterAllEventHandler();
	void UnRegisterAllNetCmd();

	DWORD OnEventOpenGodSkill(tagOpenGodSkillEvent *pEvent);

	DWORD OnNetPracticeMiracleSkill(tagNS_PracticeMiracleSkill *pNetCmd, DWORD);
	DWORD OnNetResetMiraclePrcNum(tagNS_ResetMiraclePrcNum *pNetCmd, DWORD);
	void InitSkills();
	void DestroySkills();
};

struct tagOpenGodSkillEvent : public tagGameEvent
{
	tagOpenGodSkillEvent(LPCTSTR szEventName, GameFrame* pSenderFrame):tagGameEvent(szEventName, pSenderFrame){}
	GodSkillDataMgr::EGodSkillLevel eLv;
};
