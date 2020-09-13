#include "StdAfx.h"
#include "ClergyProtoData.h"


ClergyProtoData::ClergyProtoData(void)
{
}

ClergyProtoData::~ClergyProtoData(void)
{
}

ClergyProtoData m_clergyroto;
ClergyProtoData* ClergyProtoData::Inst()
{
	return &m_clergyroto;
}

VOID ClergyProtoData::LoadFromFile()
{
	LPCTSTR szClergyProto = _T("data\\system\\attdata\\clergy_proto.xml");
	tstring strPath = g_strLocalPath + _T("\\attdata\\clergy_name.xml");
	LPCTSTR szClergyName = strPath.c_str(); 

	list<tstring> ClergyProtoFieldList;
	list<tstring> ClergyNameFieldList;
	list<tstring>::iterator iter;

	TCHAR szTemp[X_LONG_NAME] = {0};

	//---------------------------------------------------------------------------------
	//∂¡»°clergy_proto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarClergyProto = CreateObj("ClergyProto", "VarContainer");
	if(!VarClergyProto->Load("VFS_System", szClergyProto, "id", &ClergyProtoFieldList))
		IMSG(_T("Load file clergy_proto.xml failed\r\n"));

	for(iter = ClergyProtoFieldList.begin(); iter != ClergyProtoFieldList.end(); ++iter)
	{
		tagClergyName clergy;
		clergy.dwID				= VarClergyProto->GetDword(_T("id"),							(*iter).c_str(), GT_INVALID);
		clergy.n8ClergyLevel	= VarClergyProto->GetInt(_T("godheadLimit"),					(*iter).c_str(), 0);
		clergy.nNum				= VarClergyProto->GetInt(_T("num"),								(*iter).c_str(), 0);
		clergy.ePromotionType	= (EPromotionType)VarClergyProto->GetInt(_T("promotionType"),	(*iter).c_str(), EPTT_NULL);
		clergy.dwClassLimit		= (EClassType)VarClergyProto->GetInt(_T("classLimit"),			(*iter).c_str(), EV_Null);
		clergy.nGodheadLimit	= VarClergyProto->GetInt(_T("godheadLimit"),					(*iter).c_str(), 0);
		clergy.nLevelLimit		= VarClergyProto->GetInt(_T("levelLimit"),						(*iter).c_str(), 0);
		clergy.eSexLimit		= (ESex)VarClergyProto->GetInt(_T("sexLimit"),					(*iter).c_str(), 0);
		clergy.eAttType			= (ERoleAttribute)VarClergyProto->GetInt(_T("attType"),			(*iter).c_str(), ERA_Null);
		clergy.dwAttValue		= VarClergyProto->GetDword(_T("attValue"),						(*iter).c_str(), 0);
		clergy.n8FieldNum		= VarClergyProto->GetInt(_T("fieldNum"),						(*iter).c_str(), 0);
		for(int i=0; i<clergy.n8FieldNum; i++)
		{
			clergy.dwFieldID[i] = 0;
			_stprintf(szTemp, _T("feileID%d"), i);
			clergy.dwFieldID[i] = VarClergyProto->GetInt(szTemp, (*iter).c_str(), 0);
		}
		//more

		m_mapClergyName.insert(make_pair(clergy.dwID, clergy));
	}

	KillObj("ClergyProto");



	//---------------------------------------------------------------------------------
	//∂¡»°clergy_name
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarClergyName = CreateObj("ClergyName", "VarContainer");
	if(!VarClergyName->Load("VFS_System", szClergyName, "id", &ClergyNameFieldList))
		IMSG(_T("Load file clergy_name.xml failed\r\n"));

	for(iter = ClergyNameFieldList.begin(); iter != ClergyNameFieldList.end(); ++iter)
	{
		DWORD dwID = VarClergyName->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tagClergyName* pClergy = FindInMap(m_mapClergyName, dwID);
		if(P_VALID(pClergy))
		{
			_tcsncpy( pClergy->szName,			VarClergyName->GetString( _T("name"),				(*iter).c_str(), _T("")	), sizeof(pClergy->szName)/sizeof(TCHAR));
			_tcsncpy( pClergy->szIcon,			VarClergyName->GetString( _T("icon"),				(*iter).c_str(), _T("")	), sizeof(pClergy->szIcon)/sizeof(TCHAR));
			_tcsncpy( pClergy->szEffect,		VarClergyName->GetString( _T("effect"),				(*iter).c_str(), _T("")	), sizeof(pClergy->szEffect)/sizeof(TCHAR));
			_tcsncpy( pClergy->szText,			VarClergyName->GetString( _T("text"),				(*iter).c_str(), _T("")	), sizeof(pClergy->szText)/sizeof(TCHAR));
			_tcsncpy( pClergy->szClergyText,	VarClergyName->GetString( _T("godheadLevel"),		(*iter).c_str(), _T("") ), sizeof(pClergy->szClergyText)/sizeof(TCHAR));
		}
	}

	KillObj("ClergyName");
}

