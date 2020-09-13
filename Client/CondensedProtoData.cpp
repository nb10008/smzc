#include "StdAfx.h"
#include "CondensedProtoData.h"


CondensedProtoData::CondensedProtoData(void)
{
}

CondensedProtoData::~CondensedProtoData(void)
{
}

CondensedProtoData m_CondensedProto;
CondensedProtoData* CondensedProtoData::Inst()
{
	return &m_CondensedProto;
}

VOID CondensedProtoData::LoadFromFile()
{
	LPCTSTR szCondensedProto = _T("data\\system\\attdata\\godhead.xml");

	list<tstring> CondensedProtoFieldList;
	list<tstring>::iterator iter;

	TCHAR szTemp[X_LONG_NAME] = {0};

	//---------------------------------------------------------------------------------
	//∂¡»°shop_proto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarCondensedProto = CreateObj("CondensedProto", "VarContainer");
	if(!VarCondensedProto->Load("VFS_System", szCondensedProto, "id", &CondensedProtoFieldList))
		IMSG(_T("Load file godhead.xml failed\r\n"));

	for(iter = CondensedProtoFieldList.begin(); iter != CondensedProtoFieldList.end(); ++iter)
	{
		tagCondensedProto Condensed;
		Condensed.dwID				= VarCondensedProto->GetDword(_T("id"),						(*iter).c_str(), GT_INVALID);
		Condensed.n8LowerLevel      = VarCondensedProto->GetInt(_T("lower_level"),				(*iter).c_str(), 0);
		Condensed.dwAgglomerate_val = VarCondensedProto->GetDword(_T("agglomerate_val"),		(*iter).c_str(), GT_INVALID);
		Condensed.nCanActNum      = VarCondensedProto->GetInt(_T("can_act_num"),				(*iter).c_str(), 0);

		m_mapCondensedProto.insert(make_pair(Condensed.dwID, Condensed));
	}

	KillObj("CondensedProto");

}

