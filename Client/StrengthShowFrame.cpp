#include "stdafx.h"
#include "StrengthShowFrame.h"
#include "..\WorldDefine\role_att.h"


StrengthShowFrame::StrengthShowFrame()
: m_Trunk(this)
, m_pWnd(NULL)
, m_pStcTotalStrength(NULL)
, m_pStcBaseStrength(NULL)
, m_pStcLevel(NULL)
, m_pStcEquip(NULL)
, m_pStcPet(NULL)
, m_pStcFame(NULL)
, m_pStcTitle(NULL)
, m_pStcFriend(NULL)
, m_pStcQuest(NULL)
, m_pStcProduction(NULL)
, m_pStcAdvanceStrength(NULL)
, m_pStcActivity(NULL)
, m_pStcInstance(NULL)
, m_pStcRanQuest(NULL)
, m_pStcConsumptiveStrength(NULL)
, m_pStckillPlayer(NULL)
, m_pBtnClose(NULL)
{

}


StrengthShowFrame::~StrengthShowFrame()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = 0;
	}
}

BOOL StrengthShowFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;
	
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&StrengthShowFrame::EventHandler)));
	//TObjRef<NetCmdMgr>()->Register("NS_GetStrengthDetail", (NETMSGPROC)(m_Trunk.sfp2(&StrengthShowFrame::OnNetGetStrengthDetail)), _T("NS_GetStrengthDetail"));
	
	tagNC_GetStrengthDetail cmd;
	TObjRef<NetSession>()->Send(&cmd);
	return TRUE;
}

BOOL StrengthShowFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;
	
	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&StrengthShowFrame::EventHandler)));
	//TObjRef<NetCmdMgr>()->UnRegister("NS_GetStrengthDetail", (NETMSGPROC)(m_Trunk.sfp2(&StrengthShowFrame::OnNetGetStrengthDetail)));
	return TRUE;
}

BOOL StrengthShowFrame::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\strengthshow.xml");
	m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());
	m_pStcTotalStrength = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\num0"));
	m_pStcBaseStrength = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back1\\num"));
	m_pStcLevel = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back1\\num0"));
	m_pStcEquip = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back1\\num1"));
	m_pStcPet = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back1\\num2"));
	m_pStcFame = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back1\\num3"));
	m_pStcTitle = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back1\\num4"));
	m_pStcFriend = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back1\\num5"));
	m_pStcQuest = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back1\\num6"));
	m_pStcProduction = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back1\\num7"));
	m_pStcAdvanceStrength = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back2\\num"));
	m_pStcActivity = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back2\\num0"));
	m_pStcInstance = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back2\\num1"));
	m_pStcRanQuest = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back2\\num2"));
	m_pStckillPlayer = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\back2\\num3"));
	m_pStcConsumptiveStrength = (GUIStatic*)m_pWnd->GetChild(_T("strenshow_pic\\num"));
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild(_T("strenshow_pic\\closebutt"));
	return TRUE;
}

BOOL StrengthShowFrame::EscCancel()
{
	m_pGFMgr->AddToDestroyList(this);
	return TRUE;
}

DWORD StrengthShowFrame::EventHandler(tagGUIEvent *pEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch(pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnClose)
			{
				m_pGFMgr->AddToDestroyList(this);
			}
		} break;
	default: break;
	}
	return 0;
}
//
//DWORD StrengthShowFrame::OnNetGetStrengthDetail(tagNS_GetStrengthDetail *pNetCmd, DWORD)
//{
//	TCHAR szBuffer[X_LONG_NAME] = {0};
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nBaseStrength + pNetCmd->nAdvanceStrength);
//	m_pStcTotalStrength->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nBaseStrength);
//	m_pStcBaseStrength->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nAdvanceStrength);
//	m_pStcAdvanceStrength->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nConsumptiveStrength);
//	m_pStcConsumptiveStrength->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nLevel);
//	m_pStcLevel->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nEquip);
//	m_pStcEquip->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nPet);
//	m_pStcPet->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nFame);
//	m_pStcFame->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nTitle);
//	m_pStcTitle->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nFriendship);
//	m_pStcFriend->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nQuest);
//	m_pStcQuest->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nProduction);
//	m_pStcProduction->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nActivity);
//	m_pStcActivity->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nRanQuest);
//	m_pStcRanQuest->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nInstance);
//	m_pStcInstance->SetText(szBuffer);
//	_sntprintf(szBuffer, X_LONG_NAME, _T("%d"), pNetCmd->nNoWhiteNamePlayer);
//	m_pStckillPlayer->SetText(szBuffer);
//	return 0;
//}

