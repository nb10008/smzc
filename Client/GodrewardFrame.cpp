
#include "stdafx.h"
#include "GodRewardFrame.h"
#include "PlayerNameTab.h"
#include "GuildEvent.h"
#include "..\WorldDefine\msg_GuildBattle.h"

GodRewardFrame::GodRewardFrame()
: m_Trunk(this)
, m_pWnd(NULL)
, m_pStcWin(NULL)
, m_pStcNum1(NULL)
, m_pStcNum2(NULL)
, m_pStcNum3(NULL)
, m_pStcNum4(NULL)
, m_pStcNum5(NULL)
, m_pStcTime1(NULL)
, m_pStcTime2(NULL)
, m_pStcTime3(NULL)
, m_pStcTime4(NULL)
, m_pStcTime5(NULL)
, m_pStcKillNum(NULL)
, m_pStcLiveTime(NULL)
, m_pStcExploit(NULL)
, m_pStcContribution(NULL)
, m_pBtnOK(NULL)
, m_pBtnClose(NULL)
, m_dwRoleID1(GT_INVALID)
, m_dwRoleID2(GT_INVALID)
, m_dwRoleID3(GT_INVALID)
, m_dwRoleID4(GT_INVALID)
, m_dwRoleID5(GT_INVALID)
, m_pStcIntegral(NULL)
, m_pStcReward(NULL)
, m_dwElapseTime(0)
{

}

GodRewardFrame::~GodRewardFrame()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = 0;
	}
}

BOOL GodRewardFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)m_Trunk.sfp1(&GodRewardFrame::EventHandler));
	m_pMgr->RegisterEventHandle(_T("tagRefreshGodRewardEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GodRewardFrame::OnEventRefresh));
	m_pMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GodRewardFrame::OnEventRoleGetName));
	return TRUE;
}

BOOL GodRewardFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;
	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)m_Trunk.sfp1(&GodRewardFrame::EventHandler));
	m_pMgr->UnRegisterEventHandler(_T("tagRefreshGodRewardEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GodRewardFrame::OnEventRefresh));
	m_pMgr->UnRegisterEventHandler(_T("tagRoleGetNameEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&GodRewardFrame::OnEventRoleGetName));
	return TRUE;
}

BOOL GodRewardFrame::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\god_battle.xml");
	m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());
	
	m_pStcWin = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\word"));
	m_pStcNum1 = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back1\\rank0"));
	m_pStcNum2 = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back1\\rank1"));
	m_pStcNum3 = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back1\\rank2"));
	m_pStcNum4 = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back1\\word3"));
	m_pStcNum5 = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back1\\word4"));

	m_pStcTime1 = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back1\\num0"));
	m_pStcTime2 = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back1\\num1"));
	m_pStcTime3 = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back1\\num2"));
	m_pStcTime4 = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back1\\num3"));
	m_pStcTime5 = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back1\\num4"));
	
	m_pStcKillNum = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back2\\num1"));
	m_pStcLiveTime = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back2\\num2"));

	m_pStcExploit = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back3\\num1"));
	m_pStcContribution = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back3\\num2"));
	m_pStcIntegral = (GUIStatic*)m_pWnd->GetChild(_T("god_battle_pic\\back3\\num3"));
	m_pStcReward = (GUIStaticEx*)m_pWnd->GetChild(_T("god_battle_pic\\word0"));
	//m_pStcReward->SetText(g_StrTable[_T("GodBattle_Reward")]);
	m_pStcReward->SetText(_T(""));
	m_pBtnOK = (GUIButton*)m_pWnd->GetChild(_T("god_battle_pic\\butt"));
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild(_T("god_battle_pic\\closebutt"));
	m_pWnd->SetInvisible(TRUE);

	return TRUE;
}

BOOL GodRewardFrame::EscCancel()
{
	m_pMgr->AddToDestroyList(this);
	return TRUE;
}

VOID GodRewardFrame::Update()
{
	m_dwElapseTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_dwElapseTime > 1200 && P_VALID(m_pWnd) && m_pWnd->IsInvisible())
		m_pWnd->SetInvisible(FALSE);
}

DWORD GodRewardFrame::EventHandler(tagGUIEvent *pEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnOK || pWnd == m_pBtnClose)
			{
				m_pMgr->AddToDestroyList(this);
				return 0;
			}
		} break;
	default: break;
	}

	return 0;
}

DWORD GodRewardFrame::OnEventRefresh(tagRefreshGodRewardEvent *pEvent)
{
	if (pEvent->cmd.bWin)
	{
		m_pStcWin->SetText(g_StrTable[_T("GodBattle_Win")]);
		m_pStcWin->SetTextColor(0xFF00FF00);
	}
	else
	{
		m_pStcWin->SetText(g_StrTable[_T("GodBattle_Lose")]);
		m_pStcWin->SetTextColor(0xFFD1D1D1);
	}

	m_dwRoleID1 = pEvent->cmd.sRoleIndo[0].dwRoleID;
	m_dwRoleID2 = pEvent->cmd.sRoleIndo[1].dwRoleID;
	m_dwRoleID3 = pEvent->cmd.sRoleIndo[2].dwRoleID;
	m_dwRoleID4 = pEvent->cmd.sRoleIndo[3].dwRoleID;
	m_dwRoleID5 = pEvent->cmd.sRoleIndo[4].dwRoleID;
	
	TCHAR szBuffer[X_LONG_NAME] = {0};
	m_pStcNum1->SetText(PlayerNameTab::Inst()->FindNameByID(m_dwRoleID1));
	m_pStcNum1->SetTextColor(0xFFFB48F2);
	m_pStcNum2->SetText(PlayerNameTab::Inst()->FindNameByID(m_dwRoleID2));
	m_pStcNum3->SetText(PlayerNameTab::Inst()->FindNameByID(m_dwRoleID3));
	m_pStcNum4->SetText(PlayerNameTab::Inst()->FindNameByID(m_dwRoleID4));
	m_pStcNum5->SetText(PlayerNameTab::Inst()->FindNameByID(m_dwRoleID5));
	
	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("God_Kill")], pEvent->cmd.sRoleIndo[0].nKillTime);
	m_pStcTime1->SetText(szBuffer);
	m_pStcTime1->SetTextColor(0xFFFB48F2);
	
	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("God_Kill")], pEvent->cmd.sRoleIndo[1].nKillTime);
	m_pStcTime2->SetText(szBuffer);

	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("God_Kill")], pEvent->cmd.sRoleIndo[2].nKillTime);
	m_pStcTime3->SetText(szBuffer);

	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("God_Kill")], pEvent->cmd.sRoleIndo[3].nKillTime);
	m_pStcTime4->SetText(szBuffer);

	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("God_Kill")], pEvent->cmd.sRoleIndo[4].nKillTime);
	m_pStcTime5->SetText(szBuffer);


	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Kill")], pEvent->cmd.nKillTime);
	m_pStcKillNum->SetText(szBuffer);
	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Time")], pEvent->cmd.dwLiveTime);
	m_pStcLiveTime->SetText(szBuffer);
	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Exploit")], pEvent->cmd.nExploit);
	m_pStcExploit->SetText(szBuffer);
	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Contribute")], pEvent->cmd.nContribute);
	m_pStcContribution->SetText(szBuffer);
	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodBattle_Integral")], pEvent->cmd.dwLiveTime * 2 + pEvent->cmd.nKillTime );
	m_pStcIntegral->SetText(szBuffer);


	m_pWnd->SetRefresh(TRUE);
	return 0;
}

DWORD GodRewardFrame::OnEventRoleGetName(tagRoleGetNameEvent *pEvent)
{
	if (pEvent->bResult)
	{
		if (m_dwRoleID1 == pEvent->dwRoleID)
		{
			m_pStcNum1->SetText(pEvent->strRoleName.c_str());
			m_pStcNum1->SetTextColor(0xFFFB48F2);
		}
		else if (m_dwRoleID2 == pEvent->dwRoleID)
		{
			m_pStcNum2->SetText(pEvent->strRoleName.c_str());
		}
		else if (m_dwRoleID3 == pEvent->dwRoleID)
		{
			m_pStcNum2->SetText(pEvent->strRoleName.c_str());
		}
		else if (m_dwRoleID4 == pEvent->dwRoleID)
		{
			m_pStcNum2->SetText(pEvent->strRoleName.c_str());
		}
		else if (m_dwRoleID5 == pEvent->dwRoleID)
		{
			m_pStcNum2->SetText(pEvent->strRoleName.c_str());
		}
	}

	m_pWnd->SetRefresh(TRUE);
	return 0;
}
