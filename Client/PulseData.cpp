#include "StdAfx.h"
#include "PulseData.h"

PulseData m_PulseProto;
PulseData* PulseData::Inst()
{
	return &m_PulseProto;
}

PulseData::PulseData(void)
{
}

PulseData::~PulseData(void)
{
}

VOID PulseData::LoadFromFile()
{
	LPCTSTR szPulseProto = _T("data\\system\\attdata\\GodBlood_proto.xml");
	tstring strPath = g_strLocalPath + _T("\\attdata\\GodBlood_name.xml");
	LPCTSTR szPulseName = strPath.c_str(); 

	list<tstring> ClergyProtoFieldList;
	list<tstring> ClergyNameFieldList;
	list<tstring>::iterator iter;

	TCHAR szTemp[X_LONG_NAME] = {0};

	//---------------------------------------------------------------------------------
	//∂¡»°clergy_proto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarPulseProto = CreateObj("PulseProto", "VarContainer");
	if(!VarPulseProto->Load("VFS_System", szPulseProto, "id", &ClergyProtoFieldList))
		IMSG(_T("Load file PulseProto.xml failed\r\n"));

	for(iter = ClergyProtoFieldList.begin(); iter != ClergyProtoFieldList.end(); ++iter)
	{
		tagPulseClientProto stProto;

		stProto.dwId = VarPulseProto->GetDword(_T("id"),							(*iter).c_str(), GT_INVALID);
		stProto.dwLevel = VarPulseProto->GetDword(_T("level"),							(*iter).c_str(), GT_INVALID);
		stProto.dwGodLock =  VarPulseProto->GetDword(_T("godlock"),							(*iter).c_str(), GT_INVALID);
		stProto.dwLevelLock =  VarPulseProto->GetDword(_T("levellock"),							(*iter).c_str(), GT_INVALID);
		stProto.dwMaxLevel =  VarPulseProto->GetDword(_T("maxlevel"),							(*iter).c_str(), GT_INVALID);

		stProto.dwSkillID =  VarPulseProto->GetDword(_T("skillid"),							(*iter).c_str(), GT_INVALID);
		stProto.bIsAciveSkill =  (bool)VarPulseProto->GetDword(_T("skilltype"),							(*iter).c_str(), 0);
		stProto.dwSuccessRate =  VarPulseProto->GetDword(_T("success_rate"),							(*iter).c_str(), GT_INVALID);

		stProto.dwItemID =  VarPulseProto->GetDword(_T("delete_item"),							(*iter).c_str(), GT_INVALID);
		stProto.dwItemNum =  VarPulseProto->GetDword(_T("delete_item_num"),							(*iter).c_str(), GT_INVALID);

		stProto.dwItemID2 = VarPulseProto->GetDword(_T("delete_item2"),						(*iter).c_str(), GT_INVALID);
		stProto.dwItemNum2 = VarPulseProto->GetDword(_T("delete_item_num2"),				(*iter).c_str(), GT_INVALID);

		stProto.dwMoney =  VarPulseProto->GetDword(_T("delete_money"),							(*iter).c_str(), GT_INVALID);

		stProto.dwPerSkill1 =  VarPulseProto->GetDword(_T("pre1"),							(*iter).c_str(), GT_INVALID);
		stProto.dwPerSkill2 =  VarPulseProto->GetDword(_T("pre2"),							(*iter).c_str(), GT_INVALID);
		stProto.dwPerSkill3 =  VarPulseProto->GetDword(_T("pre3"),							(*iter).c_str(), GT_INVALID);

		stProto.dwNextSkill1 =  VarPulseProto->GetDword(_T("next1"),							(*iter).c_str(), GT_INVALID);
		stProto.dwNextSkill2 =  VarPulseProto->GetDword(_T("next2"),							(*iter).c_str(), GT_INVALID);
		stProto.dwNextSkill3 =  VarPulseProto->GetDword(_T("next3"),							(*iter).c_str(), GT_INVALID);
		

		m_mapPulse.insert(make_pair(stProto.dwId, stProto));
		m_mapPulseSkillIDIndex.insert(make_pair((stProto.dwSkillID*100+stProto.dwLevel),stProto));
	}

	KillObj("PulseProto");



	//---------------------------------------------------------------------------------
	//∂¡»°clergy_name
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarPulseName = CreateObj("PulseName", "VarContainer");
	if(!VarPulseName->Load("VFS_System", szPulseName, "id", &ClergyNameFieldList))
		IMSG(_T("Load file PulseName.xml failed\r\n"));

	for(iter = ClergyNameFieldList.begin(); iter != ClergyNameFieldList.end(); ++iter)
	{
		DWORD dwID = VarPulseName->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tagPulseClientProto* pProto= FindInMap(m_mapPulse, dwID);
		if(P_VALID(pProto))
		{
			pProto->bIsSpecSkill =  (bool)VarPulseName->GetDword(_T("type"),							(*iter).c_str(), 0);
			pProto->dwPageType = VarPulseName->GetDword(_T("page"),							(*iter).c_str(), GT_INVALID);
			pProto->dwPos = VarPulseName->GetDword(_T("pos"),							(*iter).c_str(), GT_INVALID);
			_tcsncpy( pProto->szName,			VarPulseName->GetString( _T("name"),				(*iter).c_str(), _T("")	), sizeof(pProto->szName)/sizeof(TCHAR));
			_tcsncpy( pProto->szIcon,			VarPulseName->GetString( _T("icon"),				(*iter).c_str(), _T("")	), sizeof(pProto->szIcon)/sizeof(TCHAR));
			_tcsncpy( pProto->szSkillDesc,			VarPulseName->GetString( _T("skilldisc"),				(*iter).c_str(), _T("")	), sizeof(pProto->szSkillDesc)/sizeof(TCHAR));
			_tcsncpy( pProto->szNextSkillDesc,			VarPulseName->GetString( _T("nextdisc"),				(*iter).c_str(), _T("")	), sizeof(pProto->szNextSkillDesc)/sizeof(TCHAR));
		}

		tagPulseClientProto* pProto2= FindInMap(m_mapPulseSkillIDIndex, pProto->dwSkillID*100+pProto->dwLevel);
		if(P_VALID(pProto2))
		{
			pProto2->bIsSpecSkill =  (bool)VarPulseName->GetDword(_T("type"),							(*iter).c_str(), 0);
			pProto2->dwPageType = VarPulseName->GetDword(_T("page"),							(*iter).c_str(), GT_INVALID);
			pProto2->dwPos = VarPulseName->GetDword(_T("pos"),							(*iter).c_str(), GT_INVALID);
			_tcsncpy( pProto2->szName,			VarPulseName->GetString( _T("name"),				(*iter).c_str(), _T("")	), sizeof(pProto2->szName)/sizeof(TCHAR));
			_tcsncpy( pProto2->szIcon,			VarPulseName->GetString( _T("icon"),				(*iter).c_str(), _T("")	), sizeof(pProto2->szIcon)/sizeof(TCHAR));
			_tcsncpy( pProto2->szSkillDesc,			VarPulseName->GetString( _T("skilldisc"),				(*iter).c_str(), _T("")	), sizeof(pProto2->szSkillDesc)/sizeof(TCHAR));
			_tcsncpy( pProto2->szNextSkillDesc,			VarPulseName->GetString( _T("nextdisc"),				(*iter).c_str(), _T("")	), sizeof(pProto2->szNextSkillDesc)/sizeof(TCHAR));
		}
	}

	KillObj("PulseName");

	map<DWORD, tagPulseClientProto>::iterator begin = m_mapPulse.begin();
	for ( ; begin != m_mapPulse.end(); ++begin)
	{
		tagPulseClientProto info;
		memcpy( &info, &(begin->second), sizeof(tagPulseClientProto) );


		if (0 == info.dwPageType)
		{
			m_mapFire.insert(make_pair(info.dwPos-1, info));
		}
		else if (1 == info.dwPageType)
		{
			m_mapWater.insert(make_pair(info.dwPos-1, info));
		}
		else if (2 == info.dwPageType)
		{
			m_mapThunder.insert(make_pair(info.dwPos-1, info));
		}
		else if (3 == info.dwPageType)
		{
			m_mapWind.insert(make_pair(info.dwPos-1, info));
		}
		else if (4 == info.dwPageType)
		{
			m_mapElement.insert(make_pair(info.dwPos-1, info));
		}
		
	}
}