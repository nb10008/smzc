#include "stdafx.h"
#include "CityWarApplyMoneyFrame.h"
#include "CityMgr.h"
#include "GuildMgr.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_city.h"


CityWarApplyMoneyFrame::CityWarApplyMoneyFrame()
: m_Trunk(this)
, m_pWnd(NULL)
, m_pInputGold(NULL)
, m_pStcGold(NULL)
, m_pStcAverageGold(NULL)
, m_pBtnOK(NULL)
, m_pBtnCancel(NULL)
, m_nInputGold(0)
, m_dwCityID(GT_INVALID)
{
	
}

CityWarApplyMoneyFrame::~CityWarApplyMoneyFrame()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
}

BOOL CityWarApplyMoneyFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;
	m_dwCityID = dwParam;
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&CityWarApplyMoneyFrame::EventHandler)));
	m_pGameFrameMgr->RegisterEventHandle(_T("ConfirmAttackMsgBoxEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyMoneyFrame::OnMsgBoxEventConfirmAttack)));
	m_pGameFrameMgr->RegisterEventHandle(_T("Close_CityWarApplyMoney"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyMoneyFrame::OnEventClose)));

	return TRUE;
}

BOOL CityWarApplyMoneyFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;
	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&CityWarApplyMoneyFrame::EventHandler)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("ConfirmAttackMsgBoxEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyMoneyFrame::OnMsgBoxEventConfirmAttack)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("Close_CityWarApplyMoney"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyMoneyFrame::OnEventClose)));
	

	return TRUE;
}

BOOL CityWarApplyMoneyFrame::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\common16.xml");
	m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());
	m_pInputGold = (GUIEditBox*)m_pWnd->GetChild(_T("comm16_pic\\putinpic1\\putin"));
	m_pStcGold = (GUIStatic*)m_pWnd->GetChild(_T("comm16_pic\\comm_words1"));
	m_pStcAverageGold = (GUIStatic*)m_pWnd->GetChild(_T("comm16_pic\\comm_words3"));
	m_pBtnOK = (GUIButton*)m_pWnd->GetChild(_T("comm16_pic\\surebutt"));
	m_pBtnCancel = (GUIButton*)m_pWnd->GetChild(_T("comm16_pic\\canclebutt"));
	m_pInputGold->SetText(_T("0"));
	m_pInputGold->SetMaxValue(2147483647);
	return TRUE;
}

DWORD CityWarApplyMoneyFrame::EventHandler(tagGUIEvent *pGUIEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pGUIEvent->dwWndID);
	switch (pGUIEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnOK)
			{
				m_nInputGold = _tcstol(m_pInputGold->GetText(), NULL, 10);
				if (P_VALID(GetObj("ConfirmAttackMsgBox")))
					KillObj("ConfirmAttackMsgBox");
				CreateObj("ConfirmAttackMsgBox", "MsgBox");
				TCHAR szBuffer[512];
				tstring strCityName = CityMgr::Inst()->GetCityName(m_dwCityID);
				_sntprintf(szBuffer, 512, g_StrTable[_T("CityWar_Attack_Confirm")], strCityName.c_str(), m_nInputGold);
				TObjRef<MsgBox>("ConfirmAttackMsgBox")->Init(_T(""), szBuffer, _T("ConfirmAttackMsgBoxEvent"), (MsgBoxFlag)(MBF_OK | MBF_Cancel), TRUE);
				return 0;
			}
			else if (pWnd == m_pBtnCancel)
			{
				m_pGameFrameMgr->AddToDestroyList(this);
				return 0;
			}
		} break;
	default: break;
	}
	return 0;
}


DWORD CityWarApplyMoneyFrame::OnMsgBoxEventConfirmAttack(tagMsgBoxEvent *pEvent)
{
	if (pEvent->eResult == MBF_OK)
	{
		const tagGuildBase *pInfo = GuildMgr::Inst()->GetGuildInfo();
		if (pInfo->nFund < m_nInputGold)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyAttack_Error4")]);
			return 0;
		}
		tagNC_SignupForCitywar cmd;
		cmd.eForce = ECFT_Attack;
		cmd.nFare = m_nInputGold;
		TObjRef<NetSession>()->Send(&cmd);
	}

	tagGameEvent event(_T("Close_CityWarApplyMoney"), this);
	m_pGameFrameMgr->SendEvent(&event);
	
	return 0;
}


DWORD CityWarApplyMoneyFrame::OnEventClose(tagGameEvent *pEvent)
{
	m_pGameFrameMgr->AddToDestroyList(this);
	return 0;
}


VOID CityWarApplyMoneyFrame::SetLowerText(int nFund)
{
	TCHAR szBuffer[256];
	_sntprintf(szBuffer, 256, _T("%d"), nFund);
	m_pStcGold->SetText(szBuffer);
}

VOID CityWarApplyMoneyFrame::SetAverageText(int nFund)
{
	TCHAR szBuffer[256];
	_sntprintf(szBuffer, 256, _T("%d"), nFund);
	m_pStcAverageGold->SetText(szBuffer);	
}

BOOL CityWarApplyMoneyFrame::EscCancel()
{
	m_pGameFrameMgr->AddToDestroyList(this);
	return TRUE;
}