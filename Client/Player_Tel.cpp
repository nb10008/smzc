#include "StdAfx.h"
#include "Player_Tel.h"
#include "..\WorldDefine\msg_item.h"
#include "CombatSysUtil.h"
#include "SaveAccount.h"

PlayerTelFrame::PlayerTelFrame(void) : m_Trunk(this)
{
}

PlayerTelFrame::~PlayerTelFrame(void)
{

}

BOOL PlayerTelFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
	return bRet;
}

BOOL PlayerTelFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	if(P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);

	return bRet;
}

BOOL PlayerTelFrame::ReloadUI()
{
	TCHAR szText[128] = {0};
	// ´´½¨
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\player_num.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pBtnSend			= (GUIButton*)m_pWnd->GetChild(_T("player_num\\surebutt"));
	m_pBtnNever			= (GUIButton*)m_pWnd->GetChild(_T("player_num\\surebutt0"));
	m_pBtnNext			= (GUIButton*)m_pWnd->GetChild(_T("player_num\\surebutt1"));
	m_pEboxTelNum		= (GUIEditBox*)m_pWnd->GetChild(_T("player_num\\back\\putin"));

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&PlayerTelFrame::EventHandler));

	return TRUE;
}

VOID PlayerTelFrame::Update()
{
	if (m_pWnd->IsInvisible())
		return;
}

BOOL PlayerTelFrame::EscCancel()
{
	return FALSE;
}

DWORD PlayerTelFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pBtnSend)
			{
				tagNC_SaveTelNum cmd;
				cmd.n64TelNum = _ttoi64(m_pEboxTelNum->GetText());
				if(cmd.n64TelNum <= 10000000000 || cmd.n64TelNum >= 19999999999)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TelNumError")]);
					return 0;
				}
				m_pSession->Send(&cmd);
				SaveAccount::Inst()->SetIsNeedSetTelNum(FALSE);
				m_pWnd->SetInvisible(TRUE);
			}
			else if(pWnd == m_pBtnNever)
			{
				tagNC_NotSaveTelNum cmd;
				m_pSession->Send(&cmd);
				SaveAccount::Inst()->SetIsNeedSetTelNum(FALSE);
				m_pWnd->SetInvisible(TRUE);
			}
			else if(pWnd == m_pBtnNext)
			{
				m_pWnd->SetInvisible(TRUE);
				SaveAccount::Inst()->SetIsNeedSetTelNum(FALSE);
			}
		}
		break;
	}
	return 0;
}
