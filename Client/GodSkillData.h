#pragma once

struct tagGodSkillData
{
	DWORD dwSkillTypeID;
	INT64 n64Silver;
	INT   nStrength;
	DWORD dwItemTypeID;
	INT   nItemNum;
};

class GodSkillData
{
public:
	typedef std::map<DWORD, tagGodSkillData*> GodSkillDataMap;
	typedef std::set<DWORD> GodSkillIDSet;
	GodSkillData(){}
	~GodSkillData(){}
	bool Init(const tstring& strPath);
	void Destroy();
	const tagGodSkillData* GetGodSkillData(DWORD dwSkillTypeID) const;
	int GetGodSkillNum() const;
	const GodSkillIDSet& GetGodSkillSet() const;
private:
	GodSkillDataMap m_mapGodSkillData;
	GodSkillIDSet m_setGodSkillID;
};