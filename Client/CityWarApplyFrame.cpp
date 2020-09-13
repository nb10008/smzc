#include "stdafx.h"
#include "CityWarApplyFrame.h"
#include "CityEvent.h"
#include "CityMgr.h"
#include "RoleMgr.h"
#include "GuildMgr.h"
#include "localplayer.h"
#include "GuildEvent.h"
#include "CombatSysUtil.h"
#include "ServerTime.h"
#include "..\WorldDefine\msg_city.h"
#include "..\WorldDefine\\QuestDef.h"
#include "CityWarApplyMoneyFrame.h"
#include "QuestMgr.h"





CityWarApplyFrame::CityWarApplyFrame()
: m_Trunk(this)
, m_pWnd(NULL)
, m_pStcCityName(NULL)
, m_pIstCityIcon(NULL)
, m_pBtnCityInfo(NULL)
, m_pStcOwnGuild(NULL)
, m_pStcNextWarTime(NULL)
, m_pStcApplyCostLowLimit(NULL)
, m_pStcApplycostAverage(NULL)
, m_pLstAttackGuildList(NULL)
, m_pLstDefenseGuildList(NULL)
, m_pBtnApplyManage(NULL)
, m_pBtnConfirmAssistDefense(NULL)
, m_pBtnAttackApply(NULL)
, m_pBtnDefenseApply(NULL)
, m_pBtnCancel(NULL)
, m_nGuildNameGetFlag(0)
, m_dwCityID(GT_INVALID)
, m_dwOwnGuildID(GT_INVALID)
, m_eApplyType(EAT_ApplyManage)
, m_nCurRowAttack(0)
, m_nCurRowDefense(0)
, m_dwAvgMoney(GT_INVALID)
, m_dwTime(0)
{

}


CityWarApplyFrame::~CityWarApplyFrame()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
}

BOOL CityWarApplyFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&CityWarApplyFrame::EventHandler)));
	m_pGameFrameMgr->RegisterEventHandle(_T("tagCityWarApplyInfoRefreshEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyFrame::OnEventApplyInfoRefresh)));
	m_pGameFrameMgr->RegisterEventHandle(_T("GetSomeGuildName"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyFrame::OnEventGetSomeGuildName)));
	m_pGameFrameMgr->RegisterEventHandle(_T("ApplyManageMsgBoxEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyFrame::OnMsgBoxEventApplyManage)));
	m_pGameFrameMgr->RegisterEventHandle(_T("ConfirmDefenceMsgBoxEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyFrame::OnMsgBoxEventConfirmDefence)));

	m_pNetCmdMgr->Register("NS_SignupForCitywar", (NETMSGPROC)(m_Trunk.sfp1(&CityWarApplyFrame::OnNetSignupForCitywar)), _T("NS_SignupForCitywar"));
	
	return TRUE;
}

BOOL CityWarApplyFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;
	
	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&CityWarApplyFrame::EventHandler)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("tagCityWarApplyInfoRefreshEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyFrame::OnEventApplyInfoRefresh)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("GetSomeGuildName"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyFrame::OnEventGetSomeGuildName)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("ApplyManageMsgBoxEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyFrame::OnMsgBoxEventApplyManage)));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("ConfirmDefenceMsgBoxEvent"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&CityWarApplyFrame::OnMsgBoxEventConfirmDefence)));
	
	m_pNetCmdMgr->UnRegister("NS_SignupForCitywar", (NETMSGPROC)(m_Trunk.sfp1(&CityWarApplyFrame::OnNetSignupForCitywar)));
	


	if (P_VALID(GetObj("ConfirmDefenceMsgBox")))
		KillObj("ConfirmDefenceMsgBox");
	if (P_VALID(GetObj("ApplyManageMsgBox")))
		KillObj("ApplyManageMsgBox");


	return TRUE;
}

BOOL CityWarApplyFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\city_war.xml");
	if (!m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str()))
		return FALSE;
	
	m_pGUI->ChangeXml(&element, "citywar_win\\citywar_pic\\back\\imgback\\img", "ClassName", "IconStatic");
	
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pStcCityName = (GUIStatic*)m_pWnd->GetChild(_T("citywar_pic\\back\\word1"));
	m_pIstCityIcon = (IconStatic*)m_pWnd->GetChild(_T("citywar_pic\\back\\imgback\\img"));
	m_pBtnCityInfo = (GUIButton*)m_pWnd->GetChild(_T("citywar_pic\\back\\butt6"));
	m_pStcOwnGuild = (GUIStatic*)m_pWnd->GetChild(_T("citywar_pic\\back\\back\\num1"));
	m_pStcNextWarTime = (GUIStatic*)m_pWnd->GetChild(_T("citywar_pic\\back\\back\\num2"));
	m_pStcApplyCostLowLimit = (GUIStatic*)m_pWnd->GetChild(_T("citywar_pic\\back\\back\\num3"));
	m_pStcApplycostAverage = (GUIStatic*)m_pWnd->GetChild(_T("citywar_pic\\back\\back\\num4"));
	
	m_pLstAttackGuildList = (GUIListBox*)m_pWnd->GetChild(_T("citywar_pic\\back\\listpic\\list1"));
	m_pLstAttackGuildList->SetColNum(1, 162);
	m_pLstDefenseGuildList = (GUIListBox*)m_pWnd->GetChild(_T("citywar_pic\\back\\listpic\\list2"));
	m_pLstDefenseGuildList->SetColNum(1, 162);

	m_pBtnApplyManage = (GUIButton*)m_pWnd->GetChild(_T("citywar_pic\\back\\butt1"));
	m_pBtnConfirmAssistDefense = (GUIButton*)m_pWnd->GetChild(_T("citywar_pic\\back\\butt2"));
	m_pBtnAttackApply = (GUIButton*)m_pWnd->GetChild(_T("citywar_pic\\back\\butt3"));
	m_pBtnDefenseApply = (GUIButton*)m_pWnd->GetChild(_T("citywar_pic\\back\\butt4"));
	m_pBtnCancel = (GUIButton*)m_pWnd->GetChild(_T("citywar_pic\\back\\butt5"));
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild(_T("citywar_pic\\close"));

	return TRUE;

}

DWORD CityWarApplyFrame::EventHandler(tagGUIEvent *pGUIEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pGUIEvent->dwWndID);
	switch (pGUIEvent->eEvent)
	{
	case EGUIE_Click:
		// 申请管理城市
		if (pWnd == m_pBtnApplyManage)
		{
			m_eApplyType = EAT_ApplyManage;
			if (P_VALID(GetObj("ApplyManageMsgBox")))
				KillObj("ApplyManageMsgBox");
			CreateObj("ApplyManageMsgBox", "MsgBox");
			TObjRef<MsgBox>("ApplyManageMsgBox")->Init(_T(""), g_StrTable[_T("CityWar_ApplyManage_MsgBox")], _T("ApplyManageMsgBoxEvent"), (MsgBoxFlag)(MBF_OK | MBF_Cancel), TRUE);
			return 0;
		}
		else if (pWnd == m_pBtnAttackApply)
		{
			m_eApplyType = EAT_ApplyAttack;
			// 判断玩家是否是帮主
			if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() != static_cast<int>(EGMP_HuiZhang))
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyAttack_Error1")]);
				return 0;
			}
			// 判断是否符合报名条件帮派等级>=2,帮主等级>=60
			const tagGuildBase* pGuildBae = GuildMgr::Inst()->GetGuildInfo();
			INT nLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
			if (pGuildBae->byLevel < 2 || nLevel < 60)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyAttack_Error2")]);
				return 0;
			}

			// 判断是否占领城市的数量已经达到最大值
			bool bMax = true;
			for (int i = 0; i < MAX_GUILD_HOLDCITY; ++i)
			{
				if (pGuildBae->byHoldCity[i] == 0)
				{
					bMax = false;
					break;
				}
			}

			if (bMax)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyAttack_Error7")]);
				return 0;
			}


			// 判断时间
			/*EWeek eWeekDay = ServerTime::Inst()->GetCurrentWeekDay();
			if (eWeekDay == EWeek_SUN || eWeekDay == EWeek_SAT)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyAttack_Error3")]);
				return 0;
			}*/

			// 创建确认界面
			GameFrame *pFrame = m_pGameFrameMgr->GetFrame(_T("CityWarApplyMoney"));
			if (!P_VALID(pFrame))
			{
				pFrame = m_pGameFrameMgr->CreateFrame(_T("World"), _T("CityWarApplyMoney"), _T("CityWarApplyMoneyFrame"), m_dwCityID);
				(static_cast<CityWarApplyMoneyFrame*>(pFrame))->SetLowerText(CityMgr::Inst()->GetMinSigfare(m_dwCityID));
				(static_cast<CityWarApplyMoneyFrame*>(pFrame))->SetAverageText(m_dwAvgMoney);
			}
		}
		else if (pWnd == m_pBtnDefenseApply)
		{
			m_eApplyType = EAT_ApplyDefence;
			// 判断玩家是否是帮主
			if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() != static_cast<int>(EGMP_HuiZhang))
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyDefence_Error1")]);
				return 0;
			}
			// 判断是否符合报名条件帮派等级>=2,帮主等级>=60
			const tagGuildBase* pGuildBae = GuildMgr::Inst()->GetGuildInfo();
			INT nLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
			if (pGuildBae->byLevel < 2 || nLevel < 60)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyDefence_Error2")]);
				return 0;
			}
			// 判断时间
			/*EWeek eWeekDay = ServerTime::Inst()->GetCurrentWeekDay();
			if (eWeekDay == EWeek_SUN || eWeekDay == EWeek_SAT)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyDefence_Error3")]);
				return 0;
			}*/
			tstring strCityName = CityMgr::Inst()->GetCityName(m_dwCityID);
			TCHAR szBuffer[512];
			_sntprintf(szBuffer, 512, g_StrTable[_T("CityWar_Defence_Confirm")], strCityName.c_str());
			if (P_VALID(GetObj("ConfirmDefenceMsgBox")))
				KillObj("ConfirmDefenceMsgBox");
			CreateObj("ConfirmDefenceMsgBox", "MsgBox");
			TObjRef<MsgBox>("ConfirmDefenceMsgBox")->Init(_T(""), szBuffer, _T("ConfirmDefenceMsgBoxEvent"), (MsgBoxFlag)(MBF_OK | MBF_Cancel), TRUE);
			
		}
		else if (pWnd == m_pBtnConfirmAssistDefense)
		{
			if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() != static_cast<int>(EGMP_HuiZhang))
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Assist_Confirm_Error1")]);
				return 0;
			}
		/*	EWeek eWeekDay = ServerTime::Inst()->GetCurrentWeekDay();
			if (eWeekDay == EWeek_SUN || eWeekDay == EWeek_SAT)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Assist_Confirm_Error2")]);
				return 0;
			}*/
			GameFrame *pFrame = m_pGameFrameMgr->GetFrame(_T("CityWarAssistConfirm"));
			if (!P_VALID(pFrame))
			{
				pFrame = m_pGameFrameMgr->CreateFrame(_T("World"), _T("CityWarAssistConfirm"), _T("CityWarAssistConfirmFrame"), 0);
				tagCityWarAssistConfirmRefreshEvent event(_T("tagCityWarAssistConfirmRefreshEvent"), this);
				//event.vecDefenseIDs.resize(m_vecDefenseIDs.size() - m_vecConfirmIDs.size());
				std::vector<DWORD>::iterator end = m_vecDefenseIDs.end();
				for (std::vector<DWORD>::iterator iter = m_vecDefenseIDs.begin(); iter != end; ++iter)
				{
					DWORD dwID = *iter;
					bool bIn = false;
					std::vector<DWORD>::iterator end1 = m_vecConfirmIDs.end();
					for (std::vector<DWORD>::iterator  iter1 = m_vecConfirmIDs.begin(); iter1 != end1; ++iter1)
					{
						if (dwID == *iter1)
							bIn = true;
					}
					if (!bIn)
						event.vecDefenseIDs.push_back(dwID);
				}
				m_pGameFrameMgr->SendEvent(&event);	
			}
			
		}
		else if (pWnd == m_pBtnClose)
		{
			m_pGameFrameMgr->AddToDestroyList(this);
		}
		else if (pWnd == m_pBtnCancel)
		{
			m_pGameFrameMgr->AddToDestroyList(this);
		}
		else if (pWnd == m_pBtnCityInfo)
		{
			tagGameEvent event(_T("Open_City"), NULL);
			TObjRef<GameFrameMgr>()->SendEvent(&event);
			return 0;
		}
	}
	return 0;
}

DWORD CityWarApplyFrame::OnEventApplyInfoRefresh(tagCityWarApplyInfoRefreshEvent *pEvent)
{
	m_dwAvgMoney = pEvent->dwAvgAckMoney;

	m_nGuildNameGetFlag = pEvent->nGetGuildNameFlag;
	m_dwOwnGuildID = pEvent->dwOwnGuild;
	m_vecAttackIDs.resize(pEvent->vecAttackIDs.size());
	std::copy(pEvent->vecAttackIDs.begin(), pEvent->vecAttackIDs.end(), m_vecAttackIDs.begin());
	
	m_vecDefenseIDs.resize(pEvent->vecDefenseIDs.size());
	std::copy(pEvent->vecDefenseIDs.begin(), pEvent->vecDefenseIDs.end(), m_vecDefenseIDs.begin());

	m_vecConfirmIDs.resize(pEvent->vecConfirmIDs.size());
	std::copy(pEvent->vecConfirmIDs.begin(), pEvent->vecConfirmIDs.end(), m_vecConfirmIDs.begin());

	m_dwCityID = pEvent->dwCityID;

	tstring strCityName = CityMgr::Inst()->GetCityName(pEvent->dwCityID);
	m_pStcCityName->SetText(strCityName.c_str());
	tstring strCityIcon = CityMgr::Inst()->GetCityIcon(pEvent->dwCityID);
	if (!strCityIcon.empty())
	{
		m_pIstCityIcon->SetPic(strCityIcon);
	}
	DWORD dwMinSigFare = CityMgr::Inst()->GetMinSigfare(pEvent->dwCityID);
	TCHAR szBuffer[128];
	_sntprintf(szBuffer, 128, _T("%d"), dwMinSigFare);
	m_pStcApplyCostLowLimit->SetText(szBuffer);
	_sntprintf(szBuffer, 128, _T("%d"), pEvent->dwAvgAckMoney);
	m_pStcApplycostAverage->SetText(szBuffer);
	
	// 星期
	EWeek eWeek = (EWeek)(pEvent->warTime.WarWeekDay);
	tstring strWeek;
	switch (eWeek)
	{
	case EWeek_MON:		strWeek = g_StrTable[_T("QuestDate_MON")];		break;
	case EWeek_TUES:	strWeek = g_StrTable[_T("QuestDate_TUES")];		break;
	case EWeek_WEDNES:	strWeek = g_StrTable[_T("QuestDate_WEDNES")];	break;
	case EWeek_THURS:	strWeek = g_StrTable[_T("QuestDate_THURS")];	break;
	case EWeek_FRI:		strWeek = g_StrTable[_T("QuestDate_FRI")];		break;
	case EWeek_SAT:		strWeek = g_StrTable[_T("QuestDate_SAT")];		break;
	case EWeek_SUN:		strWeek = g_StrTable[_T("QuestDate_SUN")];		break;
	}
	_sntprintf(szBuffer, 128, _T(" %02d:%02d"), pEvent->warTime.BegHour, pEvent->warTime.BegMinutemin);
	strWeek.append(szBuffer);
	m_pStcNextWarTime->SetText(strWeek.c_str());

	if (pEvent->bAllNameCanFind == true)
	{
		UpdateUI();
	}
	m_pWnd->SetRefresh(TRUE);
	return 0;
}

DWORD CityWarApplyFrame::OnEventGetSomeGuildName(tagGetSomeGuildNameEvent *pEvent)
{
	if (m_nGuildNameGetFlag == pEvent->nFlag)
	{
		UpdateUI();
	}
	m_pWnd->SetRefresh(TRUE);
	return 0;
}


DWORD CityWarApplyFrame::OnMsgBoxEventApplyManage(tagMsgBoxEvent *pEvent)
{
	if (pEvent->eResult == MBF_OK)
	{
		// 判断玩家是否是帮主
		if (RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildPos() != static_cast<int>(EGMP_HuiZhang))
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyManage_Error1")]);
			return 0;
		}

		// 判断是否符合报名条件帮派等级>=2,帮主等级>=60
		const tagGuildBase* pGuildBae = GuildMgr::Inst()->GetGuildInfo();
		INT nLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
		if (pGuildBae->byLevel < 2 || nLevel < 60)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyManage_Error2")]);
			return 0;
		}
		
		// 判断是否占领城市的数量已经达到最大值
		bool bMax = true;
		for (int i = 0; i < MAX_GUILD_HOLDCITY; ++i)
		{
			if (pGuildBae->byHoldCity[i] == 0)
			{
				bMax = false;
				break;
			}
		}

		if (bMax)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyManage_Error6")]);
			return 0;
		}

		// 判断是否在周一到周五之间
		/*EWeek eWeekDay = ServerTime::Inst()->GetCurrentWeekDay();
		if (eWeekDay == EWeek_SUN || eWeekDay == EWeek_SAT)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyManage_Error3")]);
			return 0;
		}*/

		// 发出申请
		tagNC_SignupForCitywar cmd;
		cmd.eForce = ECFT_Request;
		cmd.nFare = 0;
		m_pSession->Send(&cmd);
	}
	return 0;
}




VOID CityWarApplyFrame::UpdateUI()
{
	if (m_dwOwnGuildID != GT_INVALID)
	{
		tstring strName = GuildMgr::Inst()->GetGuildName(m_dwOwnGuildID);
		m_pStcOwnGuild->SetText(strName.c_str());
	}
	else
	{
		m_pStcOwnGuild->SetText(_T(""));
	}
	
	m_pLstAttackGuildList->Clear();
	m_nCurRowAttack = 0;
	m_pLstDefenseGuildList->Clear();
	m_nCurRowDefense = 0;
	
	for (size_t i = 0; i < m_vecAttackIDs.size(); ++i)
	{
		tstring strName = GuildMgr::Inst()->GetGuildName(m_vecAttackIDs[i]);
		m_pLstAttackGuildList->SetText(m_nCurRowAttack, 0, strName.c_str());
		m_nCurRowAttack++;
	}
	
	for (size_t i = 0; i < m_vecDefenseIDs.size(); ++i)
	{
		tstring strName = GuildMgr::Inst()->GetGuildName(m_vecDefenseIDs[i]);
		m_pLstDefenseGuildList->SetText(m_nCurRowDefense, 0, strName.c_str());
		m_nCurRowDefense++;
	}
	
	// 如果城市没有帮派占领,即第一次争夺所有权，进攻报名和防御报名置灰
	if (m_dwOwnGuildID == GT_INVALID)
	{	
		m_pBtnApplyManage->SetEnable(true);
		m_pBtnConfirmAssistDefense->SetEnable(false);
		m_pBtnAttackApply->SetEnable(false);
		m_pBtnDefenseApply->SetEnable(false);
	}
	else
	{
		m_pBtnApplyManage->SetEnable(false);
		m_pBtnConfirmAssistDefense->SetEnable(true);
		m_pBtnAttackApply->SetEnable(true);
		m_pBtnDefenseApply->SetEnable(true);
	}
	// 如果统治权帮派和本地玩家帮派不同,防御确认置灰
	DWORD dwLocalPlayerGuildID = RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID();
	if (m_dwOwnGuildID != dwLocalPlayerGuildID)
	{
		m_pBtnConfirmAssistDefense->SetEnable(false);
	}
	else
	{
		m_pBtnConfirmAssistDefense->SetEnable(true);
	}

	// 如果已经确认帮派已经满了
	if (m_vecConfirmIDs.size() >= 4)
		m_pBtnConfirmAssistDefense->SetEnable(false);
	m_pWnd->SetRefresh(TRUE);
}

DWORD CityWarApplyFrame::OnNetSignupForCitywar(tagNS_SignupForCitywar *pNetCmd)
{
	if (m_eApplyType == EAT_ApplyManage)
	{
		switch (pNetCmd->dwErrorCode)
		{
		case E_Success:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyManage_Success")]);
				m_pBtnApplyManage->SetEnable(false);
				m_pBtnApplyManage->SetRefresh();
			} break;
		case E_City_SignUp_InsideCityConflict:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyManage_Error5")]);
			} break;
		case E_City_SignUp_InterCityConflict:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyManage_Error4")]);
			} break;
		case E_City_Time_Limit:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyManage_Error3")]);
			} break;
		case E_City_SignUp_Hold_CityMax:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyManage_Error6")]);
			} break;
		case E_City_Struggle_Not_Open:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Not_Open")]);
			} break;
		default: break;
		}
	}
	else if (m_eApplyType == EAT_ApplyAttack)
	{
		switch (pNetCmd->dwErrorCode)
		{
		case E_Success:
			{
				TCHAR szBuffer[256];
				tstring strCityName = CityMgr::Inst()->GetCityName(m_dwCityID);
				_sntprintf(szBuffer, 256, g_StrTable[_T("CityWar_ApplyAttack_Success")], strCityName.c_str());
				CombatSysUtil::Inst()->ShowScreenCenterMsg(szBuffer);
				DWORD dwGuildID = RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID();
				tstring strGuildName = GuildMgr::Inst()->GetGuildName(dwGuildID);
				m_pLstAttackGuildList->SetText(m_nCurRowAttack++, 0, strGuildName.c_str());
			} break;
		case E_City_SignUp_HolderConflict:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyAttack_Error8")]);
			} break;
		case E_City_SignUp_InsideCityConflict:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyAttack_Error5")]);
			} break;
		case E_City_SignUp_InterCityConflict:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyAttack_Error6")]);
			} break;
		case E_City_SignUp_Hold_CityMax:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyAttack_Error7")]);
			} break;
		case E_City_SignUp_Minfare_Limite:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyAttack_Error9")]);
			} break;
		case E_City_Time_Limit:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyAttack_Error3")]);
			} break;
		case E_City_Struggle_Not_Open:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Not_Open")]);
			} break;
		default: break;
		}
	}
	else if (m_eApplyType == EAT_ApplyDefence)
	{
		switch (pNetCmd->dwErrorCode)
		{
		case E_Success:
			{
				TCHAR szBuffer[256];
				tstring strCityName = CityMgr::Inst()->GetCityName(m_dwCityID);
				_sntprintf(szBuffer, 256, g_StrTable[_T("CityWar_ApplyDefence_Success")], strCityName.c_str());
				CombatSysUtil::Inst()->ShowScreenCenterMsg(szBuffer);
				DWORD dwGuildID = RoleMgr::Inst()->GetLocalPlayer()->GetRoleGuildID();
				tstring strGuildName = GuildMgr::Inst()->GetGuildName(dwGuildID);
				m_pLstDefenseGuildList->SetText(m_nCurRowDefense++, 0, strGuildName.c_str());
			} break;
		case E_City_SignUp_HolderConflict:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyDefence_Error6")]);
			} break;
		case E_City_SignUp_InsideCityConflict:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyDefence_Error4")]);
			} break;
		case E_City_SignUp_InterCityConflict:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyDefence_Error5")]);
			} break;
		case E_City_Time_Limit:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_ApplyDefence_Error3")]);
			} break;
		case E_City_Struggle_Not_Open:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CityWar_Not_Open")]);
			} break;
		default: break;
		}
	}

	return 0;
}

DWORD CityWarApplyFrame::OnMsgBoxEventConfirmDefence(tagMsgBoxEvent *pEvent)
{
	if (pEvent->eResult == MBF_OK)
	{
		tagNC_SignupForCitywar cmd;
		cmd.eForce = ECFT_Defence;
		m_pSession->Send(&cmd);
	}
	return 0;
}

BOOL CityWarApplyFrame::EscCancel()
{
	m_pGameFrameMgr->AddToDestroyList(this);
	return TRUE;
}

VOID CityWarApplyFrame::Update()
{
	m_dwTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_dwTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_dwTime = 0;

		if (RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID() ) )
		{
			m_pGameFrameMgr->AddToDestroyList(this);
		}
	}

	GameFrame::Update();
}