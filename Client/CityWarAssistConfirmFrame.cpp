#include "stdafx.h"
#include "CityEvent.h"
#include "GuildMgr.h"
#include "CityWarAssistConfirmFrame.h"
#include "..\WorldDefine\msg_city.h"
#include "QuestMgr.h"
#include "CombatSysUtil.h"

CityWarAssistConfirmFrame::CityWarAssistConfirmFrame()
: m_Trunk(this)
, m_pWnd(NULL)
, m_pLstGuilds(NULL)
, m_pLstAssist(NULL)
, m_pBtnConfirm(NULL)
, m_pBtnNotConfirm(NULL)
, m_pBtnOK(NULL)
, m_pBtnCancel(NULL)
, m_nGuildRow(0)
, m_nAssistRow(0)
, m_dwSelectedGuildID(GT_INVALID)
, m_nCurSelectGuildRow(0)
, m_nCurSelectAssistRow(0)
{
	
}

CityWarAssistConfirmFrame::~CityWarAssistConfirmFrame()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
}

BOOL CityWarAssistConfirmFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;
	
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&CityWarAssistConfirmFrame::EventHandler)));
	m_pGameFrameMgr->RegisterEventHandle(_T("tagCityWarAssistConfirmRefreshEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarAssistConfirmFrame::OnEventRefreshUI)));
	m_pGameFrameMgr->RegisterEventHandle(_T("AssistConfirmMsgBoxEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarAssistConfirmFrame::OnMsgBoxEventAssistConfirm)));
	

	return TRUE;
}

BOOL CityWarAssistConfirmFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;
	
	
	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&CityWarAssistConfirmFrame::EventHandler)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("tagCityWarAssistConfirmRefreshEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarAssistConfirmFrame::OnEventRefreshUI)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("AssistConfirmMsgBoxEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarAssistConfirmFrame::OnMsgBoxEventAssistConfirm)));
	
	return TRUE;
}

BOOL CityWarAssistConfirmFrame::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\city_war_yes.xml");
	m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());

	m_pLstGuilds = (GUIListBox*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\listpic0\\list1"));
	m_pLstAssist = (GUIListBox*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\listpic\\list1"));
	m_pBtnConfirm = (GUIButton*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\butt3"));
	m_pBtnNotConfirm = (GUIButton*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\butt4"));
	m_pBtnOK = (GUIButton*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\butt1"));
	m_pBtnCancel = (GUIButton*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\butt2"));
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild(_T("citywarinfo_pic\\close"));

	m_pLstGuilds->SetColNum(1, 162);
	m_pLstAssist->SetColNum(1, 162);

	m_pBtnOK->SetEnable(false);
	return TRUE;
}

DWORD CityWarAssistConfirmFrame::EventHandler(tagGUIEvent* pGUIEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pGUIEvent->dwWndID);
	switch (pGUIEvent->eEvent)
	{
	case EGUIE_ItemClick:
		{
			if (pWnd == m_pLstGuilds)
			{
				m_nCurSelectGuildRow = pGUIEvent->dwParam1;
				if (m_nCurSelectGuildRow >= m_nGuildRow)
				{
					m_pBtnConfirm->SetEnable(false);
					m_pBtnConfirm->SetRefresh();
				}
				else
				{
					m_pBtnConfirm->SetEnable(true);
					m_pBtnConfirm->SetRefresh();
				}
				UpdateUI();
			}
			if (pWnd == m_pLstAssist)
			{
				m_nCurSelectAssistRow = pGUIEvent->dwParam1;
				if (m_nCurSelectAssistRow >= m_nAssistRow)
				{
					m_pBtnNotConfirm->SetEnable(false);
					m_pBtnNotConfirm->SetRefresh();
				}
				else
				{
					m_pBtnNotConfirm->SetEnable(true);
					m_pBtnNotConfirm->SetRefresh();
				}
				UpdateUI();
			}
		} break;
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnOK)
			{

				tstring strAssistGuildNames;
				for (size_t i = 0; i < m_vecAssistGuilds.size(); ++i)
				{
					tstring strGuildName = GuildMgr::Inst()->GetGuildName(m_vecAssistGuilds[i]);
					strAssistGuildNames.append(strGuildName.c_str());
					strAssistGuildNames.append(_T(","));
				}
				strAssistGuildNames.erase(strAssistGuildNames.length() -1 , 1);
				TCHAR szBuffer[256];
				_sntprintf(szBuffer, 256, g_StrTable[_T("CityWar_Assist_Be_Confirmed")], strAssistGuildNames.c_str());
				if (P_VALID(GetObj("AssistConfirm")))
					KillObj("AssistConfirm");
				CreateObj("AssistConfirm", "MsgBox");
				TObjRef<MsgBox>("AssistConfirm")->Init(_T(""), szBuffer, _T("AssistConfirmMsgBoxEvent"), (MsgBoxFlag)(MBF_OK | MBF_Cancel), TRUE);
			}
			if (pWnd == m_pBtnCancel)
			{
				m_pGameFrameMgr->AddToDestroyList(this);
			}
			if (pWnd == m_pBtnClose)
			{
				m_pGameFrameMgr->AddToDestroyList(this);
			}
			if (pWnd == m_pBtnConfirm)
			{
				GuildArray::iterator end = m_vecGuilds.end();
				DWORD dwGuildID = m_vecGuilds[m_nCurSelectGuildRow];
				for (GuildArray::iterator iter = m_vecGuilds.begin(); iter != end; ++iter)
				{
					if (*iter == dwGuildID)
					{
						m_vecGuilds.erase(iter);
						break;
					}
				}
				m_vecAssistGuilds.push_back(dwGuildID);
				m_nCurSelectGuildRow = 0;
				UpdateUI();
			}
			if (pWnd == m_pBtnNotConfirm)
			{
				GuildArray::iterator end = m_vecAssistGuilds.end();
				DWORD dwGuildID = m_vecAssistGuilds[m_nCurSelectAssistRow];
				for (GuildArray::iterator iter = m_vecAssistGuilds.begin(); iter != end; ++iter)
				{
					if (*iter == dwGuildID)
					{
						m_vecAssistGuilds.erase(iter);
						break;
					}
				}
				m_vecGuilds.push_back(dwGuildID);
				m_nCurSelectAssistRow = 0;
				UpdateUI();
			}
			
		} break;
	default: break;
	}
	return 0;
}

DWORD CityWarAssistConfirmFrame::OnEventRefreshUI(tagCityWarAssistConfirmRefreshEvent *pEvent)
{
	m_vecAssistGuilds.clear();
	m_vecGuilds.resize(pEvent->vecDefenseIDs.size());
	std::copy(pEvent->vecDefenseIDs.begin(), pEvent->vecDefenseIDs.end(), m_vecGuilds.begin());
	m_pLstGuilds->Clear();
	m_pLstAssist->Clear();
	
	for (size_t i = 0; i < m_vecGuilds.size(); ++i)
	{
		tstring strGuildName = GuildMgr::Inst()->GetGuildName(m_vecGuilds[i]);
		m_pLstGuilds->SetText(i, 0, strGuildName.c_str());
		m_nGuildRow++;
	}

	UpdateUI();

	return 0;
}





VOID CityWarAssistConfirmFrame::UpdateUI()
{
	m_pLstGuilds->Clear();
	m_pLstAssist->Clear();

	if (m_vecGuilds.empty())
		m_pBtnConfirm->SetEnable(false);
	else
		m_pBtnConfirm->SetEnable(true);

	if (m_vecAssistGuilds.empty())
	{
		m_pBtnNotConfirm->SetEnable(false);
		m_pBtnOK->SetEnable(false);
	}

	else
	{
		m_pBtnNotConfirm->SetEnable(true);
		m_pBtnOK->SetEnable(true);
	}

	if (m_vecAssistGuilds.size() >= 4)
	{
		m_pBtnConfirm->SetEnable(false);
	}

	// ÷ÿª≠listbox
	m_nGuildRow = 0;
	for (size_t i = 0; i < m_vecGuilds.size(); ++i)
	{
		tstring strGuildName = GuildMgr::Inst()->GetGuildName(m_vecGuilds[i]);
		m_pLstGuilds->SetText(i, 0, strGuildName.c_str());
		m_nGuildRow++;
	}
	m_nAssistRow = 0;
	for (size_t i = 0; i < m_vecAssistGuilds.size(); ++i)
	{
		tstring strGuildName = GuildMgr::Inst()->GetGuildName(m_vecAssistGuilds[i]);
		m_pLstAssist->SetText(i, 0, strGuildName.c_str());
		m_nAssistRow++;
	}
	m_pWnd->SetRefresh(TRUE);
}

DWORD CityWarAssistConfirmFrame::OnMsgBoxEventAssistConfirm(tagMsgBoxEvent *pEvent)
{
	if (pEvent->eResult == MBF_OK)
	{
		tagNC_ConfirmDefenceForce cmd;
		cmd.dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
		for (int i = 0; i < MAX_CITY_DEFENCE_FORCE; ++i)
		{
			if ((size_t)i < m_vecAssistGuilds.size())
			{
				cmd.dwGuildID[i] = m_vecAssistGuilds[i];
				continue;
			}
			cmd.dwGuildID[i] = GT_INVALID;
		}
		TObjRef<NetSession>()->Send(&cmd);
		m_pGameFrameMgr->AddToDestroyList(this);
	}
	return 0;
}

BOOL CityWarAssistConfirmFrame::EscCancel()
{
	m_pGameFrameMgr->AddToDestroyList(this);
	return TRUE;
}