#include "stdafx.h"
#include "CityMgr.h"
#include "GuildMgr.h"
#include "QuestMgr.h"
#include "..\WorldDefine\city_define.h"
#include "..\WorldDefine\msg_city.h"
#include "..\WorldDefine\currency_define.h"
#include "CityPickUpTaxFrame.h"
#include "CombatSysUtil.h"


CityPickUpTaxFrame::CityPickUpTaxFrame()
 : m_Trunk(this)
 , m_curTax(GT_INVALID)
{

}


CityPickUpTaxFrame::~CityPickUpTaxFrame()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
}

BOOL CityPickUpTaxFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;
	
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&CityPickUpTaxFrame::EventHandler));
	m_pFrameMgr->RegisterEventHandle(_T("Personal_Pick_Tax"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityPickUpTaxFrame::OnEventPersonalPickTax)));
	m_pFrameMgr->RegisterEventHandle(_T("Guild_Pick_Tax"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityPickUpTaxFrame::OnEventGuildPickTax)));

	return TRUE;
}

BOOL CityPickUpTaxFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;

	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&CityPickUpTaxFrame::EventHandler));
	m_pFrameMgr->UnRegisterEventHandler(_T("Personal_Pick_Tax"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityPickUpTaxFrame::OnEventPersonalPickTax)));
	m_pFrameMgr->UnRegisterEventHandler(_T("Guild_Pick_Tax"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityPickUpTaxFrame::OnEventGuildPickTax)));
	
	return TRUE;
}

BOOL CityPickUpTaxFrame::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\common13.xml");

	m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());
	m_pStcCurTax = static_cast<GUIStaticEx*>(m_pWnd->GetChild(_T("comm13_pic\\comm13_words1")));
	m_pBtnPersonal = static_cast<GUIButton*>(m_pWnd->GetChild(_T("comm13_pic\\butt1")));
	m_pBtnGuild = static_cast<GUIButton*>(m_pWnd->GetChild(_T("comm13_pic\\butt2")));
	m_pBtnCancel = static_cast<GUIButton*>(m_pWnd->GetChild(_T("comm13_pic\\butt3")));

	m_pBtnPersonal->SetText(g_StrTable[_T("City_Text01")]);
	m_pBtnGuild->SetText(g_StrTable[_T("City_Text02")]);
	m_pBtnCancel->SetText(g_StrTable[_T("City_Text03")]);

	
	DWORD dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();

	CityMgr::Inst()->SendGetAppointedCityAtt(dwNPCID, ECAT_Taxation);

	

	return TRUE;

}


VOID CityPickUpTaxFrame::SetCurTax(INT32 nTax)
{
	m_curTax = nTax;
	TCHAR szBuffer[256];
	_sntprintf(szBuffer, 256, g_StrTable[_T("City_CurTax")], MSilver2DBGold(nTax), MSilver2DBSilver(nTax));
	m_pStcCurTax->SetText(szBuffer);
	m_pStcCurTax->SetRefresh(TRUE);
}

DWORD CityPickUpTaxFrame::EventHandler( tagGUIEvent *pEvent )
{

	GUIWnd *pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnPersonal)
			{
				EGuildMemberPos ePos = GuildMgr::Inst()->GetLpInfo()->eGuildPos;
				if (ePos != EGMP_HuiZhang)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Error04")]);
					return 0;
				}
			
				if (P_VALID(GetObj("PersonalPickupTax")))
					KillObj("PersonalPickupTax");
				CreateObj("PersonalPickupTax", "MsgMoneyInputBox");
				TObjRef<MsgMoneyInputBox>("PersonalPickupTax")->Init(_T(""), g_StrTable[_T("City_Pick_Tax")], _T("Personal_Pick_Tax"), m_curTax, 0, 0, 0, MBMT_Silver, TRUE);
				
				return 0;
			}
			else if (pWnd == m_pBtnGuild)
			{
				EGuildMemberPos ePos = GuildMgr::Inst()->GetLpInfo()->eGuildPos;
				if (ePos != EGMP_HuiZhang)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Error04")]);
					return 0;
				}
				if (P_VALID(GetObj("GuildPickupTax")))
					KillObj("GuildPickupTax");
				CreateObj("GuildPickupTax", "MsgMoneyInputBox");
				TObjRef<MsgMoneyInputBox>("GuildPickupTax")->Init(_T(""), g_StrTable[_T("City_Pick_Tax")], _T("Guild_Pick_Tax"), m_curTax, 0, 0, 0, MBMT_Silver, TRUE);
				
				return 0;
			}
			else if (pWnd == m_pBtnCancel)
			{
				m_pFrameMgr->AddToDestroyList(this);
			}
		} break;
	default: break;
	}

	return 0;
}

DWORD CityPickUpTaxFrame::OnEventPersonalPickTax(tagMsgMoneyBoxEvent *pEvent)
{
	switch (pEvent->eResult)
	{
	case MBF_OK:
		{
			DWORD dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
			ETaxTransformType eType = ETTT_PersonalSilver;
			INT32 nTax = (INT32)pEvent->n64Silver;
			CityMgr::Inst()->SendTransformTax(dwNPCID, eType, nTax);
			CityMgr::Inst()->SendGetAppointedCityAtt(dwNPCID, ECAT_Taxation);
		} break;
	default: break;
	}
	return 0;
}

DWORD CityPickUpTaxFrame::OnEventGuildPickTax( tagMsgMoneyBoxEvent *pEvent )
{
	switch (pEvent->eResult)
	{
	case MBF_OK:
		{
			DWORD dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
			ETaxTransformType eType = ETTT_GuildFund;
			INT32 nTax = (INT32)pEvent->n64Silver;
			CityMgr::Inst()->SendTransformTax(dwNPCID, eType, nTax);
			CityMgr::Inst()->SendGetAppointedCityAtt(dwNPCID, ECAT_Taxation);
		} break;
	default: break;
	}

	return 0;
}

BOOL CityPickUpTaxFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return TRUE;
}


