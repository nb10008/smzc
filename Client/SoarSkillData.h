#pragma once

struct tagSoarSkillData
{
	DWORD dwSkillTypeID;
	INT64 n64Silver;
	DWORD nExp;
	DWORD dwItemTypeID;
	INT   nItemNum;
};


class SoarSkillData
{
public:
	typedef std::map<DWORD,tagSoarSkillData*> SoarSkillDataMap;
	typedef std::set<DWORD> SoarSkillIDSet;
	SoarSkillData(void);
	~SoarSkillData(void);
	bool Init(const tstring& strPath);
	void Destroy();
	const tagSoarSkillData* GetSoarSkillData(DWORD dwSkillTypeID) const;
	int GetSoarSkillNum() const;
	const SoarSkillDataMap& GetSoarSkillDataMap() const;
	const std::vector<DWORD>& GetSoarSkillVector() const;
	DWORD GetSoarSkillID(int pos);
private:
	SoarSkillDataMap	m_mapSoarSkillData;
	std::vector<DWORD>	m_vecSoarSkills;	//存放小ID
	SoarSkillIDSet		m_setSoarSkills;	// 策划需求理解错误，这里先加一个实现功能

};
