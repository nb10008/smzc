#include "StdAfx.h"
#include "CreateRolePreviewData.h"

CreateRolePreviewData::CreateRolePreviewData(void)
{
	
}

CreateRolePreviewData::~CreateRolePreviewData(void)
{

}

void CreateRolePreviewData::LoadFromFile()
{
	tstring szPath = g_strLocalPath + _T("\\attdata\\CreateRolePreview.xml");
	list<tstring> lstField;
	list<tstring>::iterator iter;

	TObjRef<VarContainer> VarCreateRole = CreateObj("CreateRolePreview", "VarContainer");
	if( !VarCreateRole->Load( "VFS_System", szPath.c_str(), "id", &lstField ) )
		IMSG( _T("Load file CreateRolePreview.xml failed\r\n") );

	for( iter = lstField.begin(); iter != lstField.end(); ++iter )
	{
		tagCreateRolePreviewItem item;
		INT nIndex = (INT)VarCreateRole->GetInt( _T("id"), (*iter).c_str(), GT_INVALID );
		item.bSex = (byte)VarCreateRole->GetInt( _T("sex"), (*iter).c_str(), GT_INVALID );
		item.nCareerType = (INT)VarCreateRole->GetInt( _T("CareerType"), (*iter).c_str(), GT_INVALID );
		item.dwWeapon = VarCreateRole->GetDword( _T("WeaponTypeID"),(*iter).c_str(), 0 );
		item.strWeaponEffect1 = VarCreateRole->GetString( _T("WeaponEffect1"),	(*iter).c_str(), _T("") );
		item.strWeaponEffect2 = VarCreateRole->GetString( _T("WeaponEffect2"),	(*iter).c_str(), _T("") );
		item.strWeaponEffect3 = VarCreateRole->GetString( _T("WeaponEffect3"),	(*iter).c_str(), _T("") );
		item.dwHat = VarCreateRole->GetDword( _T("HatTypeID"),(*iter).c_str(), 0 );
		item.dwUpper = VarCreateRole->GetDword( _T("UpperTypeID"),(*iter).c_str(), 0 );
		item.dwLower = VarCreateRole->GetDword( _T("LowerTypeID"),(*iter).c_str(), 0 );
		item.dwGlove= VarCreateRole->GetDword( _T("GloveTypeID"),(*iter).c_str(), 0 );
		item.dwBoot = VarCreateRole->GetDword( _T("BootTypeID"),(*iter).c_str(), 0 );
		item.dwEffectID = VarCreateRole->GetDword( _T("EffectID"),(*iter).c_str(), 0 );
		m_mapPreviewCfg.insert( make_pair(nIndex, item) );
	}

	KillObj( "CreateRolePreview" );
}


