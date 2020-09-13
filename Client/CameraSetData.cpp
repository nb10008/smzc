#include "StdAfx.h"
#include "CameraSetData.h"

CameraSetData::CameraSetData(void)
{
}

CameraSetData::~CameraSetData(void)
{
}

CameraSetData	g_CameraSetData;

CameraSetData* CameraSetData::Inst()
{	
	return &g_CameraSetData;
}

void CameraSetData::LoadFromFile()
{
	const TCHAR* szPath = _T("data\\system\\attdata\\CameraSetData.xml");

	list<tstring> DataList;
	list<tstring>::iterator iter;

	//读取经验
	TObjRef<VarContainer> varCameraSet = CreateObj( "CameraSetData", "VarContainer" );
	if( !varCameraSet->Load( "VFS_System", szPath, "id", &DataList ) )
		IMSG( _T("Load file CameraSetData.xml failed\r\n") );


	iter=DataList.begin();
	m_pData.dwID						= varCameraSet->GetDword( _T("id"),							(*iter).c_str(), 0 );
	m_pData.dwLoginCameraPos_x			= varCameraSet->GetDword( _T("LoginCameraPos_x"),			(*iter).c_str(), 0 );				//登陆摄像机设置
	m_pData.dwLoginCameraPos_y			= varCameraSet->GetDword( _T("LoginCameraPos_y"),			(*iter).c_str(), 0 );
	m_pData.dwLoginCameraPos_z			= varCameraSet->GetDword( _T("LoginCameraPos_z"),			(*iter).c_str(), 0 );
	m_pData.dwLoginCameraLookAt_x		= varCameraSet->GetDword( _T("LoginCameraLookAt_x"),		(*iter).c_str(), 0 );
	m_pData.dwLoginCameraLookAt_y		= varCameraSet->GetDword( _T("LoginCameraLookAt_y"),		(*iter).c_str(), 0 );
	m_pData.dwLoginCameraLookAt_z		= varCameraSet->GetDword( _T("LoginCameraLookAt_z"),		(*iter).c_str(), 0 );
	m_pData.dwSelectRoleCameraPos_x		= varCameraSet->GetDword( _T("SelectRoleCameraPos_x"),		(*iter).c_str(), 0 );				//选人摄像机设置
	m_pData.dwSelectRoleCameraPos_y		= varCameraSet->GetDword( _T("SelectRoleCameraPos_y"),		(*iter).c_str(), 0 );
	m_pData.dwSelectRoleCameraPos_z		= varCameraSet->GetDword( _T("SelectRoleCameraPos_z"),		(*iter).c_str(), 0 );
	m_pData.dwSelectRoleCameraLookAt_x	= varCameraSet->GetDword( _T("SelectRoleCameraLookAt_x"),	(*iter).c_str(), 0 );
	m_pData.dwSelectRoleCameraLookAt_y	= varCameraSet->GetDword( _T("SelectRoleCameraLookAt_y"),	(*iter).c_str(), 0 );
	m_pData.dwSelectRoleCameraLookAt_z	= varCameraSet->GetDword( _T("SelectRoleCameraLookAt_z"),	(*iter).c_str(), 0 );
	m_pData.dwRole1Pos_x				= varCameraSet->GetDword( _T("Role1Pos_x"),					(*iter).c_str(), 0 );				//角色1位置
	m_pData.dwRole1Pos_y				= varCameraSet->GetDword( _T("Role1Pos_y"),					(*iter).c_str(), 0 );
	m_pData.dwRole1Pos_z				= varCameraSet->GetDword( _T("Role1Pos_z"),					(*iter).c_str(), 0 );
	m_pData.dwRole2Pos_x				= varCameraSet->GetDword( _T("Role2Pos_x"),					(*iter).c_str(), 0 );				//角色2位置
	m_pData.dwRole2Pos_y				= varCameraSet->GetDword( _T("Role2Pos_y"),					(*iter).c_str(), 0 );
	m_pData.dwRole2Pos_z				= varCameraSet->GetDword( _T("Role2Pos_z"),					(*iter).c_str(), 0 );
	m_pData.dwRole3Pos_x				= varCameraSet->GetDword( _T("Role3Pos_x"),					(*iter).c_str(), 0 );				//角色3位置
	m_pData.dwRole3Pos_y				= varCameraSet->GetDword( _T("Role3Pos_y"),					(*iter).c_str(), 0 );
	m_pData.dwRole3Pos_z				= varCameraSet->GetDword( _T("Role3Pos_z"),					(*iter).c_str(), 0 );
	m_pData.bIsShake					= varCameraSet->GetDword( _T("IsShake"),					(*iter).c_str(), 0 );				//是否晃动
	m_pData.dwCreateRolePos_x			= varCameraSet->GetDword( _T("CreateRolePos_x"),			(*iter).c_str(), 0 );				//创建角色角色位置
	m_pData.dwCreateRolePos_y			= varCameraSet->GetDword( _T("CreateRolePos_y"),			(*iter).c_str(), 0 );
	m_pData.dwCreateRolePos_z			= varCameraSet->GetDword( _T("CreateRolePos_z"),			(*iter).c_str(), 0 );
	m_pData.dwCreateCameraLookat_x		= varCameraSet->GetDword( _T("CreateCameraLookat_x"),		(*iter).c_str(), 0 );				//创建角色摄像机朝向
	m_pData.dwCreateCameraLookat_y		= varCameraSet->GetDword( _T("CreateCameraLookat_y"),		(*iter).c_str(), 0 );
	m_pData.dwCreateCameraLookat_z		= varCameraSet->GetDword( _T("CreateCameraLookat_z"),		(*iter).c_str(), 0 );
	m_pData.dwCreateCameraNearLookat_x	= varCameraSet->GetDword( _T("CreateCameraNearLookat_x"),	(*iter).c_str(), 0 );
	m_pData.dwCreateCameraNearLookat_y	= varCameraSet->GetDword( _T("CreateCameraNearLookat_y"),	(*iter).c_str(), 0 );
	m_pData.dwCreateCameraNearLookat_z	= varCameraSet->GetDword( _T("CreateCameraNearLookat_z"),	(*iter).c_str(), 0 );
	m_pData.dwCreateCamPitch			= varCameraSet->GetDword( _T("CreateCamPitch"),				(*iter).c_str(), 0 );
	m_pData.dwCreateCamRoll				= varCameraSet->GetDword( _T("CreateCamRoll"),				(*iter).c_str(), 0 );
	m_pData.dwCreateCamYaw				= varCameraSet->GetDword( _T("CreateCamYaw"),				(*iter).c_str(), 0 );
	m_pData.dwCreateMAXCamDist			= varCameraSet->GetDword( _T("CreateMAXCamDist"),			(*iter).c_str(), 0 );

	KillObj("CameraSetData");
}