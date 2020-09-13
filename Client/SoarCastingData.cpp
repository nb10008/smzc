#include "StdAfx.h"
#include "SoarCastingData.h"

SoarCastingData::SoarCastingData(void)
{
}

SoarCastingData::~SoarCastingData(void)
{
}

bool SoarCastingData::Init()
{
	m_mapCasting.clear();
	LoadFromFile();
	return true;
}
void SoarCastingData::Destroy()
{
	m_mapCasting.clear();
}
BOOL SoarCastingData::LoadFromFile()
{
	list<tstring> listProto;
	tstring szTrainProto = _T("data\\system\\attdata\\casting_proto.xml");
	TObjRef<VarContainer> varTrainProto = CreateObj("SoarCastingProto", "VarContainer");
	if (!varTrainProto->Load("VFS_System", szTrainProto.c_str(), "id", &listProto))
	{
		IMSG(_T("Load SoarCastingProto file failed"));
		KillObj("SoarCastingProto");
		return FALSE;
	}

	list<tstring>::iterator end = listProto.end();
	for (list<tstring>::iterator begin = listProto.begin(); begin != end; ++begin)
	{
		tagSoarCastingProto data;

		data.dwID = varTrainProto->GetInt(_T("id"), begin->c_str(),0);
		data.byLevel = varTrainProto->GetInt(_T("level"), begin->c_str(),0);
		data.eType = static_cast<ESoarCastingType>(varTrainProto->GetInt(_T("type"), begin->c_str(),0));
		data.dwPotvalConsume = varTrainProto->GetDword(_T("potval_consume"), begin->c_str(),0);
		data.nSuccessRate    = varTrainProto->GetInt(_T("probability"), begin->c_str(),0);

		data.CastingRoleAtt[0].eRoleAtt = static_cast<ERoleAttribute>(varTrainProto->GetInt(_T("att1"), begin->c_str(),0));
		data.CastingRoleAtt[0].nAttVal  = varTrainProto->GetInt(_T("val1"), begin->c_str(),0);
		data.CastingRoleAtt[1].eRoleAtt = static_cast<ERoleAttribute>(varTrainProto->GetInt(_T("att2"), begin->c_str(),0));
		data.CastingRoleAtt[1].nAttVal  = varTrainProto->GetInt(_T("val2"), begin->c_str(),0);
		data.CastingRoleAtt[2].eRoleAtt = static_cast<ERoleAttribute>(varTrainProto->GetInt(_T("att3"), begin->c_str(),0));
		data.CastingRoleAtt[2].nAttVal  = varTrainProto->GetInt(_T("val3"), begin->c_str(),0);
		data.CastingRoleAtt[3].eRoleAtt = static_cast<ERoleAttribute>(varTrainProto->GetInt(_T("att4"), begin->c_str(),0));
		data.CastingRoleAtt[3].nAttVal  = varTrainProto->GetInt(_T("val4"), begin->c_str(),0);
		data.CastingRoleAtt[4].eRoleAtt = static_cast<ERoleAttribute>(varTrainProto->GetInt(_T("att5"), begin->c_str(),0));
		data.CastingRoleAtt[4].nAttVal  = varTrainProto->GetInt(_T("val5"), begin->c_str(),0);

		data.CastingStuff[0].dwID		= varTrainProto->GetDword(_T("material1"), begin->c_str(),0);
		data.CastingStuff[0].dwStuffNum = varTrainProto->GetInt(_T("num1"), begin->c_str(),0);
		data.CastingStuff[1].dwID		= varTrainProto->GetDword(_T("material2"), begin->c_str(),0);
		data.CastingStuff[1].dwStuffNum = varTrainProto->GetInt(_T("num2"), begin->c_str(),0);
		data.CastingStuff[2].dwID		= varTrainProto->GetDword(_T("material3"), begin->c_str(),0);
		data.CastingStuff[2].dwStuffNum = varTrainProto->GetInt(_T("num3"), begin->c_str(),0);
		data.CastingStuff[3].dwID		= varTrainProto->GetDword(_T("material4"), begin->c_str(),0);
		data.CastingStuff[3].dwStuffNum = varTrainProto->GetInt(_T("num4"), begin->c_str(),0);
		data.CastingStuff[4].dwID		= varTrainProto->GetDword(_T("material5"), begin->c_str(),0);
		data.CastingStuff[4].dwStuffNum = varTrainProto->GetInt(_T("num5"), begin->c_str(),0);

		m_mapCasting.insert(make_pair(data.dwID,data));

	}
	KillObj("SoarCastingProto");


	list<tstring> listName;
	tstring szTrainName = g_strLocalPath + _T("\\attdata\\casting_name.xml");
	TObjRef<VarContainer> varCastingName = CreateObj("SoarCastingName", "VarContainer");
	if (!varCastingName->Load("VFS_System", szTrainName.c_str(), "id", &listName))
	{
		IMSG(_T("Load SoarCastingName file failed"));
		KillObj("SoarCastingName");
		return FALSE;
	}
	list<tstring>::iterator iend = listName.end();
	for (list<tstring>::iterator begin = listName.begin(); begin != iend; ++begin)
	{
		DWORD id = varCastingName->GetDword(_T("id"), begin->c_str(), 0);
		INT32 ids = varCastingName->GetInt(_T("id"), begin->c_str(), 0);
		tstring name = varCastingName->GetString(_T("name"), begin->c_str(), _T("")) ;
		SoarCastingDataMap::iterator it = m_mapCasting.find(id);
		if (it != m_mapCasting.end())
		{
			_tcscpy( it->second.szName, name.c_str() );	
		}
	}
	KillObj("SoarCastingName");

	return TRUE;
}

const tagSoarCastingProto* SoarCastingData::GetSoarCastingProtoData(DWORD dwID) const
{
	SoarCastingDataMap::const_iterator iter = m_mapCasting.find(dwID);
	if (iter != m_mapCasting.end())
		return &(iter->second);
	return NULL;
}

const SoarCastingData::SoarCastingDataMap& SoarCastingData::GetSoarCastingDataMap() const
{
	return m_mapCasting;
}