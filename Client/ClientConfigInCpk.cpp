#include "StdAfx.h"
#include "ClientConfigInCpk.h"

ClientConfigInCpk ClientConfigInCpk::s_clientConfigMgr;
ClientConfigInCpk* ClientConfigInCpk::Inst()
{
	return &s_clientConfigMgr;
}
ClientConfigInCpk::ClientConfigInCpk(void):m_Trunk(this)
,m_dwOpenMall(0)
,m_dwFaceBookMode(0)
,m_dw360(0)
{
}

ClientConfigInCpk::~ClientConfigInCpk(void)
{
}

VOID ClientConfigInCpk::Init()
{
	CreateObj("GameConfigInCPK", "VarContainer");

	
	_tcscpy( m_szFilename, _T("data\\local\\zhCN\\attdata\\GameCfgInCPK.xml" ) );
	m_pCfgVars = "GameConfigInCPK";
	if (m_pCfgVars->Load( "VFS_System", m_szFilename ))
	{
		m_dwOpenMall  = m_pCfgVars->GetDword( _T("mall_open"), NULL, 0 );
		m_dwFaceBookMode = m_pCfgVars->GetDword( _T("facebook"), NULL, 0 );
		m_dw360= m_pCfgVars->GetDword( _T("360_on"), NULL, 0 );
	}
}
