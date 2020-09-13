#include "stdafx.h"
#include "GodSkillData.h"
#include "SkillMgr.h"


bool GodSkillData::Init(const tstring& strPath)
{
	m_mapGodSkillData.clear();
	std::list<tstring> GodSkillList;
	if (P_VALID(GetObj("GodSkill")))
		KillObj("GodSkill");
	TObjRef<VarContainer> con = CreateObj("GodSkill", "VarContainer");
	if (!con->Load("VFS_System", strPath.c_str(), "id", &GodSkillList))
		return false;
	std::list<tstring>::iterator end = GodSkillList.end();
	for (std::list<tstring>::iterator iter = GodSkillList.begin(); iter != end; ++iter)
	{
		tagGodSkillData *pGodSkillData = new tagGodSkillData;
		pGodSkillData->dwSkillTypeID = con->GetDword(_T("id"), iter->c_str(), GT_INVALID);
		pGodSkillData->n64Silver = static_cast<INT64>(con->GetInt(_T("learn_silver"), iter->c_str(), 0));
		pGodSkillData->nStrength = con->GetInt(_T("learn_power"), iter->c_str(), 0);
		pGodSkillData->dwItemTypeID = con->GetDword(_T("learn_item_typeid"), iter->c_str(), GT_INVALID);
		pGodSkillData->nItemNum = con->GetInt(_T("learn_item_num"), iter->c_str(), 0);
		m_mapGodSkillData.insert(std::make_pair(pGodSkillData->dwSkillTypeID, pGodSkillData));

		DWORD dwSkillID = MTransSkillID(pGodSkillData->dwSkillTypeID);
		m_setGodSkillID.insert(dwSkillID);

	}
	KillObj("GodSkill");
	return true;
}

void GodSkillData::Destroy()
{
	GodSkillDataMap::iterator end = m_mapGodSkillData.end();
	for (GodSkillDataMap::iterator iter = m_mapGodSkillData.begin(); iter != end; ++iter)
	{
		SAFE_DEL(iter->second);
	}
	m_mapGodSkillData.clear();
	m_setGodSkillID.clear();
}

const tagGodSkillData* GodSkillData::GetGodSkillData(DWORD dwSkillTypeID) const
{
	GodSkillDataMap::const_iterator iter = m_mapGodSkillData.find(dwSkillTypeID);
	if (iter != m_mapGodSkillData.end())
		return iter->second;
	return static_cast<const tagGodSkillData*>(NULL);
}

int GodSkillData::GetGodSkillNum() const
{
	return static_cast<int>(m_setGodSkillID.size());
}

const GodSkillData::GodSkillIDSet& GodSkillData::GetGodSkillSet() const
{
	return m_setGodSkillID;
}


