#include "StdAfx.h"
#include "MasterMgr.h"
#include "CombatSysUtil.h"
#include "MasterFrame.h"
#include "RoleMgr.h"
#include "PlayerNameTab.h"
#include "..\WorldDefine\master_apprentice.h"
#include "..\WorldDefine\msg_master_apprentice.h"

MasterMgr::MasterMgr(void) :
    m_Trunk(this)
{
	m_dwMyJingWu = 0;
	m_dwMyQinWu = 0;
}

MasterMgr::~MasterMgr(void)
{
}

void MasterMgr::Init()
{
    // get the tools
    m_pCmdMgr       =   TObjRef<NetCmdMgr>();
    m_pUtil         =   TObjRef<Util>();
    m_pFrameMgr     =   TObjRef<GameFrameMgr>();
    m_pSession      =   TObjRef<NetSession>();

	m_pCmdMgr->Register( "NS_GetJingWuPoint", (NETMSGPROC)(m_Trunk.sfp2(&MasterMgr::OnNetGetJingWuPoint)), _T("NS_GetJingWuPoint") );
	m_pCmdMgr->Register( "NS_GraduateNotice", (NETMSGPROC)(m_Trunk.sfp2(&MasterMgr::OnStuGraduate)), _T("NS_GraduateNotice") );
	m_pFrameMgr->RegisterEventHandle( _T("OnUseJingWuPoint"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&MasterMgr::OnUseJingWuPoint)) );

	if(P_VALID(GetObj("StuLevelUpInfo")))
		KillObj("StuLevelUpInfo");
	m_pVarContainer = CreateObj("StuLevelUpInfo", "VarContainer");
	LoadStuLevelUpInfoFile();
}

void MasterMgr::Destroy()
{
	m_pCmdMgr->UnRegister( "NS_GetJingWuPoint", (NETMSGPROC)(m_Trunk.sfp2(&MasterMgr::OnNetGetJingWuPoint)) );
	m_pCmdMgr->UnRegister( "NS_GraduateNotice", (NETMSGPROC)(m_Trunk.sfp2(&MasterMgr::OnStuGraduate)) );
	m_pFrameMgr->UnRegisterEventHandler( _T("OnUseJingWuPoint"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&MasterMgr::OnUseJingWuPoint)));

	if(P_VALID(GetObj("StuLevelUpInfo")))
		KillObj("StuLevelUpInfo");
}

void MasterMgr::Update()
{

}

MasterMgr g_MasterMgr;
MasterMgr* MasterMgr::inst()
{
    return &g_MasterMgr;
}

bool MasterMgr::ShowErrMsg( DWORD dwErrCode )
{
    if (dwErrCode > E_MA_Success &&
        dwErrCode < E_MA_ErrorCode_End)
    {
        tstringstream tss;
        tss << _T("MasterErrorMsg_") << dwErrCode;
        CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[tss.str().c_str()]);

        return true;
    }

    return false;
}

void MasterMgr::LoadStuLevelUpInfoFile()
{
	const TCHAR* szStuFile = _T("data\\system\\attdata\\master_apprentice.xml");
	list<tstring> StuLevelList;
	m_pVarContainer->Load("VFS_System", szStuFile, "id", &StuLevelList);
	list<tstring>::iterator iter = StuLevelList.begin();

	for(; iter!=StuLevelList.end(); iter++)
	{
		tagStuLevelUpInfo StulevelUpInfo;
		StulevelUpInfo.dwLevel = m_pVarContainer->GetDword( _T("id"), (*iter).c_str(), GT_INVALID );
		StulevelUpInfo.dwAddJingWu = m_pVarContainer->GetDword( _T("level_up_add_point"), (*iter).c_str(), GT_INVALID );
		StulevelUpInfo.dwNeedJingWu = m_pVarContainer->GetDword( _T("level_up_need_point"), (*iter).c_str(), GT_INVALID );
		StulevelUpInfo.dwNeedNum = m_pVarContainer->GetDword( _T("level_up_need_yuanbao"), (*iter).c_str(), GT_INVALID );
		m_vecStuLevelUpInfo.push_back( StulevelUpInfo );
	}
}

DWORD MasterMgr::NeedJingWuCurLevel( DWORD dwLevel )
{
	for( size_t i = 0; i < m_vecStuLevelUpInfo.size(); i++ )
	{
		if( dwLevel == m_vecStuLevelUpInfo[i].dwLevel )
		{
			return m_vecStuLevelUpInfo[i].dwNeedJingWu;
		}
	}
	return GT_INVALID;
}

DWORD MasterMgr::NeedItemNumCurLevel( DWORD dwLevel )
{
	for( size_t i = 0; i < m_vecStuLevelUpInfo.size(); i++ )
	{
		if( dwLevel == m_vecStuLevelUpInfo[i].dwLevel )
		{
			return m_vecStuLevelUpInfo[i].dwNeedNum;
		}
	}
	return GT_INVALID;
}

DWORD MasterMgr::OnNetGetJingWuPoint( tagNS_GetJingWuPoint *pMsg, DWORD )
{
	m_dwMyJingWu += pMsg->nPoint;
	if(P_VALID(GetObj("GetJingWuPoint")))
		KillObj("GetJingWuPoint");
	CreateObj("GetJingWuPoint", "MsgBox");
	TObjRef<MsgBox>("GetJingWuPoint")->Init( _T(""), g_StrTable[_T("UseJingWuPoint")], _T("OnUseJingWuPoint") );
	return 0;
}

DWORD MasterMgr::OnUseJingWuPoint( tagMsgBoxEvent *pGameEvent )
{
	if( pGameEvent->eResult == MBF_OK )
	{
		MasterFrame *pFrame =(MasterFrame*) m_pFrameMgr->GetFrame(_T("Master"));
		if( !P_VALID(pFrame) )
		{
			pFrame = (MasterFrame*)m_pFrameMgr->CreateFrame( _T("World"), _T("Master"), _T("MasterFrame"), 0 );
		}
		if( P_VALID(pFrame) )
		{
			pFrame->ShowUseWnd();
		}
	}
	return 0;
}

DWORD MasterMgr::OnStuGraduate( tagNS_GraduateNotice *pMsg, DWORD )
{
	if( pMsg->dwRoleID != GT_INVALID && pMsg->dwRoleID != RoleMgr::Inst()->GetLocalPlayerID() )
	{
		tstring strName = PlayerNameTab::Inst()->FindNameByID( pMsg->dwRoleID );
		if( !strName.empty() )
		{
			TCHAR szTips[X_LONG_NAME] = {0};
			_sntprintf( szTips, X_LONG_NAME, g_StrTable[_T("StuGraduate")], strName.c_str() );
			if( P_VALID(GetObj("StuGraduate")))
				KillObj("StuGraduate");
			CreateObj( "StuGraduate", "MsgBoxEx");
			TObjRef<MsgBoxEx>("StuGraduate")->Init( _T(""), szTips );
		}
	}
	else if( pMsg->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		if( P_VALID(GetObj("IGraduate")))
			KillObj("IGraduate");
		CreateObj( "IGraduate", "MsgBoxEx");
		TObjRef<MsgBoxEx>("IGraduate")->Init( _T(""), g_StrTable[_T("IGraduate")],NULL ,MBF_OK,FALSE );
	}
	return 0;
}