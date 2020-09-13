#include "stdafx.h"
#include "..\WorldDefine\city_define.h"
#include "..\WorldDefine\msg_city.h"
#include "QuestMgr.h"
#include "CityMgr.h"
#include "CityTaxRateFrame.h"

CityTaxRateFrame::CityTaxRateFrame()
 : m_Trunk(this)
 , m_pWnd(NULL)
 , m_pStcCurRate(NULL)
 , m_pEdbRateSet(NULL)
 , m_pBtnOK(NULL)
 , m_pBtnCancel(NULL)
{

}

CityTaxRateFrame::~CityTaxRateFrame()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
}

BOOL CityTaxRateFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&CityTaxRateFrame::EventHandler));
	return TRUE;
}

BOOL CityTaxRateFrame::Destroy()
{
	if (GameFrame::Destroy())
		return FALSE;
	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&CityTaxRateFrame::EventHandler));
	return TRUE;
}

BOOL CityTaxRateFrame::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\common14.xml");
	m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());
	m_pStcCurRate = static_cast<GUIStaticEx*>(m_pWnd->GetChild(_T("comm14_pic\\comm_words")));
	m_pEdbRateSet = static_cast<GUIEditBox*>(m_pWnd->GetChild(_T("comm14_pic\\putinpic\\putin")));
	m_pBtnOK = static_cast<GUIButton*>(m_pWnd->GetChild(_T("comm14_pic\\surebutt")));
	m_pBtnCancel = static_cast<GUIButton*>(m_pWnd->GetChild(_T("comm14_pic\\canclebutt")));
	m_pEdbRateSet->SetMaxValue(10);

	DWORD dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
	CityMgr::Inst()->SendGetAppointedCityAtt(dwNPCID, ECAT_TaxRate);

	return TRUE;
}



VOID CityTaxRateFrame::SetCurTaxRateText(DWORD dwRate)
{
	TCHAR szBuffer[256];
	_sntprintf(szBuffer, 256, g_StrTable[_T("City_CurTaxRate")], dwRate);
	m_pStcCurRate->SetText(szBuffer);
	m_pStcCurRate->SetRefresh(TRUE);

}


DWORD CityTaxRateFrame::EventHandler(tagGUIEvent *pEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnOK)
			{
				LPCTSTR str = m_pEdbRateSet->GetText();
				INT nTaxRate = _ttoi(str);
				DWORD dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
				CityMgr::Inst()->SendSetTaxRate(dwNPCID, nTaxRate);
				m_pFrameMgr->AddToDestroyList(this);
			}
			if (pWnd == m_pBtnCancel)
			{
				m_pFrameMgr->AddToDestroyList(this);
			}
		} break;
	} 
	return 0;
}

BOOL CityTaxRateFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return TRUE;
}

