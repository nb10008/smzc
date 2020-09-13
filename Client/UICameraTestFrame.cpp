#include "StdAfx.h"
#include "UICameraTestFrame.h"
#include "CombatSysUtil.h"
#include "GUIEffectMgr.h"



UICameraTestFrame::UICameraTestFrame(void):m_Trunk(this)
{
}

UICameraTestFrame::~UICameraTestFrame(void)
{
}

BOOL UICameraTestFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather,dwParam);

//#ifdef _UICAMERA_TOOL
	m_pGUI->RegisterEventHandler( m_pMainWnd->GetFullName().c_str(),   m_Trunk.sfp1(&UICameraTestFrame::EventHandler));
	m_pFrameMgr->RegisterEventHandle(_T("OpenUICameraToolEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&UICameraTestFrame::OnOpenUICameraTool));//add by xp
	m_v3Pos = Vector3(0.0f, 0.0f, -190.0f  );
	m_v3LookAt = Vector3( 0.0f, 0.0f, 0.0f );
	m_pEd_PosX->SetText(_T("0.0"));
	m_pEd_PosY->SetText(_T("0.0"));
	m_pEd_PosZ->SetText(_T("-190.0"));

	m_pEd_LookAtX->SetText(_T("0.0"));
	m_pEd_LookAtY->SetText(_T("0.0"));
	m_pEd_LookAtZ->SetText(_T("0.0"));

	m_pEd_EffectPosX->SetText(_T("0.0"));
	m_pEd_EffectPosY->SetText(_T("0.0"));
	m_pEd_EffectPosZ->SetText(_T("0.0"));

	m_pEd_EffectName->SetText(_T("txui001"));
	m_pEd_WndPath->SetText(_T("\\desktop"));

	m_strWndPath = _T("\\desktop");
	m_strEffectName = _T("txui001");

	m_pMainWnd->SetInvisible(TRUE);

//#endif
	return bRet;
}

BOOL UICameraTestFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

//#ifdef _UICAMERA_TOOL	
	if (P_VALID(m_pMainWnd))
	{
		m_pGUI->AddToDestroyList(m_pMainWnd);
		m_pMainWnd = NULL;
	}
//#endif

	return bRet;
}

BOOL UICameraTestFrame::ReloadUI()
{

//#ifdef _UICAMERA_TOOL

 	XmlElement ele;
 	tstring strPath = g_strLocalPath + _T("\\ui\\camera_set.xml");
 	if(!m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str()))
 		return FALSE;
 	
 	m_pMainWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
 
 	m_pEd_PosX = static_cast<GUIEditBox*>(m_pMainWnd->GetChild(_T("camera_back\\putinpic\\putin")));
 	m_pEd_PosY = static_cast<GUIEditBox*>(m_pMainWnd->GetChild(_T("camera_back\\putinpic2\\putin")));
 	m_pEd_PosZ = static_cast<GUIEditBox*>(m_pMainWnd->GetChild(_T("camera_back\\putinpic3\\putin")));
 
 	m_pEd_LookAtX = static_cast<GUIEditBox*>(m_pMainWnd->GetChild(_T("camera_back\\putinpic0\\putin")));
 	m_pEd_LookAtY = static_cast<GUIEditBox*>(m_pMainWnd->GetChild(_T("camera_back\\putinpic4\\putin")));
 	m_pEd_LookAtZ = static_cast<GUIEditBox*>(m_pMainWnd->GetChild(_T("camera_back\\putinpic5\\putin")));
 
 	m_pEd_EffectPosX = static_cast<GUIEditBox*>(m_pMainWnd->GetChild(_T("camera_back\\putinpic8\\putin")));
 	m_pEd_EffectPosY = static_cast<GUIEditBox*>(m_pMainWnd->GetChild(_T("camera_back\\putinpic9\\putin")));
 	m_pEd_EffectPosZ = static_cast<GUIEditBox*>(m_pMainWnd->GetChild(_T("camera_back\\putinpic10\\putin")));
 
 	m_pEd_EffectName = static_cast<GUIEditBox*>(m_pMainWnd->GetChild(_T("camera_back\\putinpic6\\putin")));
 	m_pEd_WndPath = static_cast<GUIEditBox*>(m_pMainWnd->GetChild(_T("camera_back\\putinpic7\\putin")));
 
 	m_pBnGO = static_cast<GUIButton*>(m_pMainWnd->GetChild(_T("camera_back\\gobutt")));
 	m_pBnClose = static_cast<GUIButton*>(m_pMainWnd->GetChild(_T("camera_back\\close")));
	m_pBnClose1 = static_cast<GUIButton*>(m_pMainWnd->GetChild(_T("camera_back\\closebutt")));

//#endif
	return TRUE;
}

VOID UICameraTestFrame::GO()
{
//#ifdef _UICAMERA_TOOL
 	tstring strings;
 
 	strings.clear();
 	strings= m_pEd_PosX->GetText();
 	m_v3Pos.x = (FLOAT)_tstof(strings.c_str());
 
 	strings.clear();
 	strings= m_pEd_PosY->GetText();
 	m_v3Pos.y = (FLOAT)_tstof(strings.c_str());
 
 	strings.clear();
 	strings= m_pEd_PosZ->GetText();
 	m_v3Pos.z = (FLOAT)_tstof(strings.c_str());
 
 
 	strings.clear();
 	strings= m_pEd_LookAtX->GetText();
 	m_v3LookAt.x = (FLOAT)_tstof(strings.c_str());
 
 	strings.clear();
 	strings= m_pEd_LookAtY->GetText();
 	m_v3LookAt.y = (FLOAT)_tstof(strings.c_str());
 
 	strings.clear();
 	strings= m_pEd_LookAtZ->GetText();
 	m_v3LookAt.z = (FLOAT)_tstof(strings.c_str());
 
 	strings.clear();
 	strings= m_pEd_EffectPosX->GetText();
 	m_v3EffectPos.x = (FLOAT)_tstof(strings.c_str());
 
 	strings.clear();
 	strings= m_pEd_EffectPosY->GetText();
 	m_v3EffectPos.y = (FLOAT)_tstof(strings.c_str());
 
 	strings.clear();
 	strings= m_pEd_EffectPosZ->GetText();
 	m_v3EffectPos.z = (FLOAT)_tstof(strings.c_str());
 
 	m_strEffectName.clear();
 	m_strEffectName = m_pEd_EffectName->GetText();
 
 	m_strWndPath.clear();
 	m_strWndPath = m_pEd_WndPath->GetText();
 
 	tstring ss = _T("\\\\");
 	while ((int)m_strWndPath.find(ss) != -1)
 	{
 		int pos =  (int)m_strWndPath.find(ss);
 		m_strWndPath.erase(pos,1);
 	}
 
 	GUIWnd* pTestWnd = m_pGUI->GetWnd(m_strWndPath.c_str());
 	if (!P_VALID(pTestWnd))
 	{
 		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GUICamera_WndPathNotValuable")]);
 		return;
 	}
 
	tagPoint posWnd = pTestWnd->GetView();
	tagPoint sizeWnd = pTestWnd->GetSize();
	LONG top, left, right, bottom;
	top		= posWnd.x - sizeWnd.x/2;
	left		= posWnd.y-sizeWnd.y;
	right		= top + 2*sizeWnd.x;
	bottom	= left + 2*sizeWnd.y;
	tagRect rect(top,left,right,bottom);
 	GUIEffectMgr::Inst()->SetUICamera(m_v3Pos,m_v3LookAt);
 	GUIEffectMgr::Inst()->PlayGuiEffect(m_strEffectName.c_str(),pTestWnd,rect, m_v3EffectPos,Vector3(1.0f, 1.0f, 1.0f ));

//#endif
}

BOOL UICameraTestFrame::EventHandler(tagGUIEvent* pEvent)
{
//#ifdef _UICAMERA_TOOL

 	GUIWnd* m_pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
 	if( !P_VALID(m_pWnd) )
 		return FALSE;
 
 	switch( pEvent->eEvent )
 	{
 	case EGUIE_Click:
 		if (m_pWnd == m_pBnGO)
 		{
 			GO();
 		}
 		if (m_pWnd == m_pBnClose)
 		{
 			GUIEffectMgr::Inst()->CloseAllEffect();	
 		}
		if (m_pWnd == m_pBnClose1)//add by xp
		{
			m_pMainWnd->SetInvisible(TRUE);	
		}//add end
 		break;
 	case EGUIE_EditBoxEnter:
 		{
 			if (	m_pWnd == m_pEd_EffectName 
 				||	m_pWnd == m_pEd_WndPath
 				||	m_pWnd == m_pEd_PosX
 				||	m_pWnd == m_pEd_PosY
 				||	m_pWnd == m_pEd_PosZ
 				||	m_pWnd == m_pEd_LookAtX
 				||	m_pWnd == m_pEd_LookAtY
 				||	m_pWnd == m_pEd_LookAtZ
 				||	m_pWnd == m_pEd_EffectPosX
 				||	m_pWnd == m_pEd_EffectPosY
 				||	m_pWnd == m_pEd_EffectPosZ
 				)
 			{
 				GO();
 			}
 		}
 		break;
 	default:
 		break;
 	}
//#endif
	return TRUE;
}

DWORD UICameraTestFrame::OnOpenUICameraTool(tagGameEvent* pGameEvent)
{
	m_pMainWnd->SetInvisible(FALSE);
	return 0;
}