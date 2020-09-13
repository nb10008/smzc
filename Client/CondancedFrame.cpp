#include "stdafx.h"
#include "ClergyFrame.h"

ClergyFrame::ClergyFrame()
:m_Trunk(this)
{

}
ClergyFrame::~ClergyFrame()
{

}

BOOL ClergyFrame::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;
	//m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_Move2Diamond"),        (FRAMEEVENTPROC)m_Trunk.sfp1( &DiamondFrame::OnItemEvent_Move2Diamond));

	//m_pNetCmdMgr->Register("NS_Chisel",	            (NETMSGPROC)m_Trunk.sfp2(&DiamondFrame::OnNS_Chisel),           _T("NS_Chisel"));
	return TRUE;
}

BOOL ClergyFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL ClergyFrame::Destroy()
{
	GameFrame::Destroy();
	//m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Diamond"),          (FRAMEEVENTPROC)m_Trunk.sfp1(&DiamondFrame::OnItemEvent_Move2Diamond));

	//m_pNetCmdMgr->UnRegister("NS_Chisel",	        (NETMSGPROC)m_Trunk.sfp2(&DiamondFrame::OnNS_Chisel));

	return TRUE;
}
BOOL ClergyFrame::ReloadUI()
{
	TCHAR szText[256] = {0};
	char temp[256] = {0};
	XmlElement ele;
	tstring strParth = g_strLocalPath + _T("\\ui\\condanced.xml");
	m_pGUI->LoadXml(&ele,"VFS_System",strParth.c_str());

	//m_pGUI->ChangeXml(&ele,"diamond_win\\diamond_pic\\back0\\imgback1\\img","ClassName","ComposeStatic"); //change

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"),&ele);
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(),m_Trunk.sfp1(&ClergyFrame::EventHandler));
	m_pWndCaption = m_pWnd->GetChild(_T(""));
	//m_pEquip        = (ComposeStatic*)m_pWndCaption->GetChild(_T("back0\\imgback1\\img"));	            //╪сть
	return TRUE;
}
VOID ClergyFrame::Update()
{
	GameFrame::Update();
}

BOOL ClergyFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if(!P_VALID(pWnd))
		return FALSE;
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		// 		if(pWnd == )
		// 		{
		// 		
		// 		}
		break;
	default:break;
	}
	return FALSE;
}
