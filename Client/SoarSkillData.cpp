#include "StdAfx.h"
#include "SoarSkillData.h"
#include "SkillMgr.h"

SoarSkillData::SoarSkillData(void)
{
}

SoarSkillData::~SoarSkillData(void)
{
}

bool SoarSkillData::Init(const tstring& strPath)
{
	//m_mapSoarSkillData.clear();
	for (SoarSkillDataMap::iterator iter = m_mapSoarSkillData.begin(); iter != m_mapSoarSkillData.end(); ++iter)
	{
		SAFE_DEL(iter->second);
	}
	m_mapSoarSkillData.clear();
	std::list<tstring> GodSkillList;
	if (P_VALID(GetObj("SoarSkill")))
		KillObj("SoarSkill");
	TObjRef<VarContainer> con = CreateObj("SoarSkill", "VarContainer");
	if (!con->Load("VFS_System", strPath.c_str(), "id", &GodSkillList))
		return false;
	std::list<tstring>::iterator end = GodSkillList.end();
	for (std::list<tstring>::iterator iter = GodSkillList.begin(); iter != end; ++iter)
	{
		tagSoarSkillData *pGodSkillData = new tagSoarSkillData;
		pGodSkillData->dwSkillTypeID = con->GetDword(_T("id"), iter->c_str(), GT_INVALID);
		pGodSkillData->n64Silver = static_cast<INT64>(con->GetInt(_T("learn_silver"), iter->c_str(), 0));
		pGodSkillData->nExp = con->GetDword(_T("learn_exp"), iter->c_str(), 0);
		pGodSkillData->dwItemTypeID = con->GetDword(_T("learn_item_typeid"), iter->c_str(), GT_INVALID);
		pGodSkillData->nItemNum = con->GetInt(_T("learn_item_num"), iter->c_str(), 0);

		m_mapSoarSkillData.insert(make_pair(pGodSkillData->dwSkillTypeID,pGodSkillData));
		DWORD dwSkillID = MTransSkillID(pGodSkillData->dwSkillTypeID);
		m_setSoarSkills.insert(dwSkillID);
	
	}
	KillObj("SoarSkill");

	for (SoarSkillIDSet::iterator iter = m_setSoarSkills.begin();
		iter != m_setSoarSkills.end(); ++iter)
	{
		DWORD dwSkillID = *iter;
		m_vecSoarSkills.push_back(dwSkillID);
	}
	return true;
}

void SoarSkillData::Destroy()
{
	SoarSkillDataMap::iterator end = m_mapSoarSkillData.end();
	for (SoarSkillDataMap::iterator iter = m_mapSoarSkillData.begin(); iter != end; ++iter)
	{
		SAFE_DEL(iter->second);
	}
	m_mapSoarSkillData.clear();
	m_vecSoarSkills.clear();
	m_setSoarSkills.clear();

}

const tagSoarSkillData* SoarSkillData::GetSoarSkillData(DWORD dwSkillTypeID) const
{
	SoarSkillDataMap::const_iterator iter = m_mapSoarSkillData.find(dwSkillTypeID);
	if (iter != m_mapSoarSkillData.end())
		return iter->second;
	return static_cast<const tagSoarSkillData*>(NULL);
}

int SoarSkillData::GetSoarSkillNum() const
{
	return static_cast<int>(m_mapSoarSkillData.size());
}

const SoarSkillData::SoarSkillDataMap& SoarSkillData::GetSoarSkillDataMap() const
{
	return m_mapSoarSkillData;
}

const std::vector<DWORD>& SoarSkillData::GetSoarSkillVector() const
{
	return m_vecSoarSkills;
}
DWORD SoarSkillData::GetSoarSkillID(int pos)
{
	if (pos > m_mapSoarSkillData.size())
	{
		return 0;
	}
	SoarSkillDataMap::const_iterator iter = m_mapSoarSkillData.begin();
	for ( int i = 0;i<pos;i++)
	{
		++iter;
	}
	return iter->second->dwSkillTypeID;
}