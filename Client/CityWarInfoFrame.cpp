#include "stdafx.h"
#include "CityWarInfoFrame.h"
#include "IconStatic.h"
#include "CityEvent.h"
#include "CityMgr.h"
#include "..\WorldDefine\msg_city.h"
#include "..\WorldDefine\currency_define.h"
#include "GuildEvent.h"
#include "GuildMgr.h"
#include "CurrencyMgr.h"
#include "CombatSysUtil.h"

CityWarInfoFrame::CityWarInfoFrame()
: m_Trunk(this)
, m_pWnd(NULL)
, m_pStcCityName(NULL)
, m_pIstCityIcon(NULL)
, m_pBtnCityInfo(NULL)
, m_pLstWarCity(NULL)
, m_pLstAttacks(NULL)
, m_pLstDefences(NULL)
, m_pBtnClose(NULL)
, m_nCityRow(0)
, m_nCurCityRow(0)
{
}

CityWarInfoFrame::~CityWarInfoFrame()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
}



BOOL CityWarInfoFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;
	
	m_pGameFrameMgr->RegisterEventHandle(_T("tagCityWarCityRefreshEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp2(&CityWarInfoFrame::OnEventCityWarCityRefresh)));
	m_pGameFrameMgr->RegisterEventHandle(_T("GetSomeGuildName"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarInfoFrame::OnEventGetSomeGuildName)));
	m_pGameFrameMgr->RegisterEventHandle(_T("OnViewCityInfoMsgBoxEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarInfoFrame::OnEventViewCityInfoMsgBox)));
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&CityWarInfoFrame::EventHandler)));
	TObjRef<NetCmdMgr>()->Register("NS_GetCityWarInfo", (NETMSGPROC)(m_Trunk.sfp2(&CityWarInfoFrame::OnNetGetCityWarInfo)), _T("NS_GetCityWarInfo"));
	TObjRef<NetCmdMgr>()->Register("NS_GetCityWarCity", (NETMSGPROC)(m_Trunk.sfp2(&CityWarInfoFrame::OnNetGetCityWarCity)), _T("NS_GetCityWarCity"));
	
	CityMgr::Inst()->SendGetCityWarCity();
	return TRUE;
}


BOOL CityWarInfoFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;

	m_pGameFrameMgr->UnRegisterEventHandler(_T("tagCityWarCityRefreshEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp2(&CityWarInfoFrame::OnEventCityWarCityRefresh)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("GetSomeGuildName"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarInfoFrame::OnEventGetSomeGuildName)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("OnViewCityInfoMsgBoxEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarInfoFrame::OnEventViewCityInfoMsgBox)));
	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&CityWarInfoFrame::EventHandler)));
	TObjRef<NetCmdMgr>()->UnRegister("NS_GetCityWarInfo", (NETMSGPROC)(m_Trunk.sfp2(&CityWarInfoFrame::OnNetGetCityWarInfo)));
	TObjRef<NetCmdMgr>()->UnRegister("NS_GetCityWarCity", (NETMSGPROC)(m_Trunk.sfp2(&CityWarInfoFrame::OnNetGetCityWarCity)));

	if (P_VALID(GetObj("ViewCityInfoMsgBox")))
		KillObj("ViewCityInfoMsgBox");
	return TRUE;
}

BOOL CityWarInfoFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\city_war_info.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "citywarinfo_win\\citywarinfo_pic\\back\\imgback\\img", "ClassName", "IconStatic");
	
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pStcCityName = (GUIStatic*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\word1"));
	m_pIstCityIcon = (IconStatic*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\imgback\\img"));
	m_pBtnCityInfo = (GUIButton*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\butt6"));
	m_pLstWarCity = (GUIListBox*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\listpic0\\list1"));
	m_pLstAttacks = (GUIListBox*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\listpic\\list1"));
	m_pLstDefences = (GUIListBox*)m_pWnd->GetChild(_T("citywarinfo_pic\\back\\listpic\\list2"));
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild(_T("citywarinfo_pic\\close"));
	
	m_pLstWarCity->SetColNum(1, 207);
	m_pLstAttacks->SetColNum(1, 162);
	m_pLstDefences->SetColNum(1, 162);



	return TRUE;
}

DWORD CityWarInfoFrame::EventHandler(tagGUIEvent *pGUIEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pGUIEvent->dwWndID);
	
	switch (pGUIEvent->eEvent)
	{
	case EGUIE_ItemClick:
		{
			if (pWnd == m_pLstWarCity)
			{
				m_nCurCityRow = pGUIEvent->dwParam1;
				if (m_nCurCityRow >= m_nCityRow)
				{
					m_pLstAttacks->Clear();
					m_pLstDefences->Clear();
					m_pStcCityName->SetText(_T(""));
					m_pBtnCityInfo->SetEnable(false);
					m_pIstCityIcon->SetPic(_T(""));
					m_pIstCityIcon->SetInvisible(TRUE);
					m_pWnd->SetRefresh();
				}
				else
				{
					// 向服务器发出申请
					tagNC_GetCityWarInfo cmd;
					cmd.dwCityID = m_vecCities[m_nCurCityRow];
					TObjRef<NetSession>()->Send(&cmd);
					tstring strName = CityMgr::Inst()->GetCityName(m_vecCities[m_nCurCityRow]);
					m_pStcCityName->SetText(strName.c_str());
					m_pIstCityIcon->SetPic(CityMgr::Inst()->GetCityIcon(m_vecCities[m_nCurCityRow]));
					m_pIstCityIcon->SetInvisible(FALSE);
					m_pBtnCityInfo->SetEnable(true);
					m_pWnd->SetRefresh();
				}
			}
		} break;
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnCityInfo)
			{
				// 打开城市信息
				if (P_VALID(GetObj("ViewCityInfoMsgBox")))
					KillObj("ViewCityInfoMsgBox");
				
				CreateObj("ViewCityInfoMsgBox", "MsgBox");
				TObjRef<MsgBox>("ViewCityInfoMsgBox")->Init(_T(""), g_StrTable[_T("City_Text04")], _T("OnViewCityInfoMsgBoxEvent"), (MsgBoxFlag)(MBF_OK | MBF_Cancel), TRUE);
				return 0;
			} 
			else if (pWnd == m_pBtnClose)
			{
				m_pGameFrameMgr->AddToDestroyList(this);
			}
		} break;
	default: break;
	}
	return 0;
}


DWORD CityWarInfoFrame::OnNetGetCityWarCity(tagNS_GetCityWarCity *pNetCmd, DWORD)
{
	m_pLstWarCity->Clear();
	m_nCityRow = 0;
	m_vecCities.clear();
	for (int i = 0; i < pNetCmd->nNum; ++i)
	{
		m_vecCities.push_back(pNetCmd->dwCityID[i]);
	}
	for (size_t i = 0; i < m_vecCities.size(); ++i)
	{
		tstring strCityName = CityMgr::Inst()->GetCityName(m_vecCities[i]);
		m_pLstWarCity->SetText(m_nCityRow, 0, strCityName.c_str());
		m_nCityRow++;
	}
	m_pLstWarCity->SetRefresh();

	tagGUIEvent evt(m_pLstWarCity, EGUIE_ItemClick);
	evt.dwParam1 = 0;
	m_pLstWarCity->SendEvent(&evt);

	return 0;
}



DWORD CityWarInfoFrame::OnEventCityWarCityRefresh(tagCityWarCityRefreshEvent *pEvent)
{
	return 0;
}

DWORD CityWarInfoFrame::OnNetGetCityWarInfo(tagNS_GetCityWarInfo *pNetCmd, DWORD)
{

	m_vecAttacks.clear();
	m_vecDefenses.clear();

	std::vector<DWORD> vecReqNameList;

	if (pNetCmd->dwErrorCode == E_Success)
	{
		for (int i = 0; i < MAX_CITY_ATTACK_FORCE; ++i)
		{
			if (pNetCmd->dwAttacks[i] != GT_INVALID)
			{
				bool bFind = GuildMgr::Inst()->IsGuildNameExist(pNetCmd->dwAttacks[i]);
				if (!bFind)
				{
					vecReqNameList.push_back(pNetCmd->dwAttacks[i]);
				}
				m_vecAttacks.push_back(pNetCmd->dwAttacks[i]);
			}
		}
	
		for (int i = 0; i < MAX_CITY_DEFENCE_FORCE + 1; ++i)
		{
			if (pNetCmd->dwDefenses[i] != GT_INVALID)
			{
				bool bFind = GuildMgr::Inst()->IsGuildNameExist(pNetCmd->dwDefenses[i]);
				if (!bFind)
				{
					vecReqNameList.push_back(pNetCmd->dwDefenses[i]);
				}
				m_vecDefenses.push_back(pNetCmd->dwDefenses[i]);
			}
		}
	}
	
	if (!vecReqNameList.empty())
	{
		GuildMgr::Inst()->SendGetSomeGuildName(m_nGuildNameGetFlag, vecReqNameList);
	}
	else
	{
		UpdateUI();
	}

	return 0;
}

void CityWarInfoFrame::UpdateUI()
{
	m_pLstAttacks->Clear();
	m_pLstDefences->Clear();

	for (int i = 0; i < (int)m_vecAttacks.size(); ++i)
	{
		tstring strName = GuildMgr::Inst()->GetGuildName(m_vecAttacks[i]);
		m_pLstAttacks->SetText(i, 0, strName.c_str());
	}
	for (int i = 0; i < (int)m_vecDefenses.size(); ++i)
	{
		tstring strName = GuildMgr::Inst()->GetGuildName(m_vecDefenses[i]);
		m_pLstDefences->SetText(i, 0, strName.c_str());
	}
	m_pWnd->SetRefresh(TRUE);
}

DWORD CityWarInfoFrame::OnEventGetSomeGuildName(tagGetSomeGuildNameEvent *pEvent)
{
	if (m_nGuildNameGetFlag == pEvent->nFlag)
	{
		UpdateUI();
	}
	return 0;
}

BOOL CityWarInfoFrame::EscCancel()
{
	m_pGameFrameMgr->AddToDestroyList(this);
	return TRUE;
}

DWORD CityWarInfoFrame::OnEventViewCityInfoMsgBox(tagMsgBoxEvent *pEvent)
{
	if (pEvent->eResult == MBF_OK)
	{
		// 检查玩家的金钱
		INT64 n64Silver = CurrencyMgr::Inst()->GetBagSilver();
		if (n64Silver < 10 * GOLD2SILVER)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("City_Error07")]);
			return 0;
		}
		tagOpenCityEvent event(_T("Open_City_By_ID"), NULL);
		event.dwCityID = m_vecCities[m_nCurCityRow];
		TObjRef<GameFrameMgr>()->SendEvent(&event);
	}
	return 0;
}