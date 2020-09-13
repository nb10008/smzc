#include "StdAfx.h"
#include "GodAreaData.h"


GodAreaData::GodAreaData(void)
{
}

GodAreaData::~GodAreaData(void)
{
}
void GodAreaData::LoadFromFile()
{
	m_mapGodHeadAreaNum.clear();
	m_setTemp.clear();
	list<tstring> listProto;
	tstring szTrainProto = _T("data\\system\\attdata\\godarea_proto.xml");
	TObjRef<VarContainer> varTrainProto = CreateObj("GodAreaData", "VarContainer");
	if (!varTrainProto->Load("VFS_System", szTrainProto.c_str(), "id", &listProto))
	{
		IMSG(_T("Load GodAreaData file failed"));
		KillObj("GodAreaData");
		return;
	}

	list<tstring>::iterator end = listProto.end();
	for (list<tstring>::iterator begin = listProto.begin(); begin != end; ++begin)
	{
		tagGodAreaName data;

 		data.dwID = varTrainProto->GetDword(_T("id"), begin->c_str(),0);
 		data.eType = (EGodAreaType)varTrainProto->GetDword(_T("Type"), begin->c_str(),0);
 		data.eNatureType = (EGodAreaNatureType)varTrainProto->GetDword(_T("nature_type"), begin->c_str(),0);
 		data.eUseType =  (EGodAreaUseType)varTrainProto->GetDword(_T("use_type"), begin->c_str(),0);
 		data.nLevel = varTrainProto->GetInt(_T("level"), begin->c_str(),0);
 		data.nLevelLimit =varTrainProto->GetInt(_T("level_limit"), begin->c_str(),0);
		data.nGodheadLimit =  varTrainProto->GetDword(_T("godhead_limit"), begin->c_str(),0);
		data.nMaxNum =  varTrainProto->GetDword(_T("max_num"), begin->c_str(),0);
		data.dwPreID =  varTrainProto->GetDword(_T("pre_godarea_id"), begin->c_str(),0);
		data.nFaithValue =  varTrainProto->GetDword(_T("faith_val"), begin->c_str(),0);
		data.dwSkillID =  varTrainProto->GetDword(_T("skill_id"), begin->c_str(),0);
		data.dwBuffID =  varTrainProto->GetDword(_T("buff_id"), begin->c_str(),0);
		data.eFunctionType =  (EGodAreaFuncType)varTrainProto->GetDword(_T("function_type"), begin->c_str(),0);
		data.nFunctionValue =  varTrainProto->GetDword(_T("function_val"), begin->c_str(),0);
		data.nTime =  varTrainProto->GetDword(_T("skill_timecontrol"), begin->c_str(),0);

		m_mapGodAreaData.insert(make_pair(data.dwID,data));

		if(EGAT_OPTIONAL ==	data.eType )
		{
			if(m_setTemp.insert(data.dwID/100).second)
			{
				map<DWORD,DWORD>::iterator it = m_mapGodHeadAreaNum.find(data.nGodheadLimit);
				if (it != m_mapGodHeadAreaNum.end())
				{
					it->second++;
				}
				else
				{
					m_mapGodHeadAreaNum.insert(make_pair(data.nGodheadLimit,1));
				}
				m_mapNum_ID.insert(make_pair(data.nGodheadLimit,data.dwID/100));
			}
		}

	}
	KillObj("GodAreaData");

	list<tstring> listName;
	tstring szTrainName = g_strLocalPath + _T("\\attdata\\godarea_name.xml");
	TObjRef<VarContainer> varTrainName = CreateObj("GodareaName", "VarContainer");
	if (!varTrainName->Load("VFS_System", szTrainName.c_str(), "id", &listName))
	{
		IMSG(_T("Load GodareaName file failed"));
		KillObj("GodareaName");
		return ;
	}
	list<tstring>::iterator iend = listName.end();
	for (list<tstring>::iterator begin = listName.begin(); begin != iend; ++begin)

	{
		DWORD dwID = varTrainName->GetInt(_T("id"), begin->c_str(), 0);
		tagGodAreaName* pClergy = NULL;
		map<DWORD,tagGodAreaName> ::iterator pIter = m_mapGodAreaData.find(dwID);
		if(pIter != m_mapGodAreaData.end())
			pClergy = &pIter->second;
		if(P_VALID(pClergy))
		{
			_tcsncpy( pClergy->szName,			varTrainName->GetString( _T("name"),				begin->c_str(), _T("")	), sizeof(pClergy->szName)/sizeof(TCHAR));
			_tcsncpy( pClergy->szIcon,			varTrainName->GetString( _T("icon"),				begin->c_str(), _T("")	), sizeof(pClergy->szIcon)/sizeof(TCHAR));
			_tcsncpy( pClergy->szDesc,		varTrainName->GetString( _T("desc"),				begin->c_str(), _T("")	), sizeof(pClergy->szDesc)/sizeof(TCHAR));
			_tcsncpy( pClergy->szDescType,			varTrainName->GetString( _T("desc_type"),				begin->c_str(), _T("")	), sizeof(pClergy->szDescType)/sizeof(TCHAR));
			_tcsncpy( pClergy->szGetDesc,			varTrainName->GetString( _T("getdesc"),				begin->c_str(), _T("")	), sizeof(pClergy->szGetDesc)/sizeof(TCHAR));
		}
		/*
		DWORD id = varTrainName->GetInt(_T("id"), begin->c_str(), 0);
		tstring szName			  = varTrainName->GetString(_T("name"), begin->c_str(), _T(""));
		tstring szIcon   =	varTrainName->GetString(_T("icon"), begin->c_str(), _T(""));
		tstring szDesc = varTrainName->GetString(_T("desc"), begin->c_str(), _T(""));
		tstring szDescType  =	varTrainName->GetString(_T("desc_type"), begin->c_str(), _T(""));

		map<DWORD,tagGodAreaName>::iterator it = m_mapGodAreaData.find(id);
		if (it != m_mapGodAreaData.end())
		{
			memcpy(it->second.szName, szName.c_str(), X_LONG_NAME*sizeof(TCHAR));
			memcpy(it->second.szIcon, szIcon.c_str(), X_LONG_NAME*sizeof(TCHAR));
			memcpy(it->second.szDesc, szDesc.c_str(), X_LONG_NAME*sizeof(TCHAR));
			memcpy(it->second.szDescType, szDescType.c_str(), X_LONG_NAME*sizeof(TCHAR));
		}
		*/
	}
	KillObj("GodareaName");

}
const tagGodAreaName* GodAreaData::GetAreaDataByID(const DWORD& dwID)
{
	map<DWORD,tagGodAreaName>::iterator it = m_mapGodAreaData.find(dwID);
	if (it != m_mapGodAreaData.end())
	{
		return &(it->second);
	}
	return NULL;
}
int GodAreaData::GetFreePickAreaNumByGodHead(int nGodHead)
{
	map<DWORD,DWORD>::iterator it = m_mapGodHeadAreaNum.find(nGodHead);
	if (it != m_mapGodHeadAreaNum.end())
	{
		return it->second;
	}
	return 0;
}

void GodAreaData::GetFreePickAreaListByNum(int num ,TList<DWORD>& list)
{
	list.Clear();
	multimap<DWORD,DWORD>::iterator itBegin, itEnd;
	itBegin = m_mapNum_ID.lower_bound(num);
	itEnd   = m_mapNum_ID.upper_bound(num);
	while(itBegin != itEnd)
	{
		list.PushBack(itBegin->second);
		++itBegin;
	}
}