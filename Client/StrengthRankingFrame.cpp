#include "stdafx.h"
#include "StrengthRankingFrame.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\currency_define.h"
#include "..\WorldDefine\role_att.h"
#include "..\WorldDefine\msg_reputation.h"
#include "TargetCharacterFrame.h"
#include "SocialEvent.h"

StrengthRankingFrame::StrengthRankingFrame()
: m_Trunk(this)
, m_pWnd(NULL)
, m_pPbnStrength(NULL)
, m_pPbnLevel(NULL)
, m_pBtnEquip(NULL)
, m_pPbnFame(NULL)
, m_pPbnPet(NULL)
, m_pPbnFlower(NULL)
, m_pPbnEgg(NULL)
, m_pStcCommon(NULL)
, m_pLstRanking(NULL)
, m_pBtnPre(NULL)
, m_pBtnNext(NULL)
, m_pStcCurNum(NULL)
, m_pStcLocalRank(NULL)
, m_pBtnClose1(NULL)
, m_pBtnClose2(NULL)
, m_nCurPageNum(1)
, m_dwLastRow(GT_INVALID)
{
	ZeroMemory(m_dwRank, sizeof(m_dwRank));
}

StrengthRankingFrame::~StrengthRankingFrame()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
}

BOOL StrengthRankingFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&StrengthRankingFrame::EventHandler)));
	m_pCmdMgr->Register("NS_GetRankingInfo", (NETMSGPROC)(m_Trunk.sfp1(&StrengthRankingFrame::OnNetGetRankInfo)), _T("NS_GetRankingInfo"));
	m_pCmdMgr->Register("NS_GetPetRankingInfo", (NETMSGPROC)(m_Trunk.sfp1(&StrengthRankingFrame::OnNetGetPetRankInfo)), _T("NS_GetPetRankingInfo"));
	m_pCmdMgr->Register("NS_GetOfflineEquipInfo", (NETMSGPROC)(m_Trunk.sfp1(&StrengthRankingFrame::OnNetGetOfflineEquipInfo)), _T("NS_GetOfflineEquipInfo"));
	m_pCmdMgr->Register("NS_GetOfflineReputation", (NETMSGPROC)(m_Trunk.sfp1(&StrengthRankingFrame::OnNetGetOfflineReputation)), _T("NS_GetOfflineReputation"));

	m_pPbnLevel->SetState(EGUIBS_PushDown);

	return TRUE;
}

BOOL StrengthRankingFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;
	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_Trunk.sfp1(&StrengthRankingFrame::EventHandler)));
	if(P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	m_pCmdMgr->UnRegister("NS_GetRankingInfo", (NETMSGPROC)(m_Trunk.sfp1(&StrengthRankingFrame::OnNetGetRankInfo)));
	m_pCmdMgr->UnRegister("NS_GetPetRankingInfo", (NETMSGPROC)(m_Trunk.sfp1(&StrengthRankingFrame::OnNetGetPetRankInfo)));
	m_pCmdMgr->UnRegister("NS_GetOfflineEquipInfo", (NETMSGPROC)(m_Trunk.sfp1(&StrengthRankingFrame::OnNetGetOfflineEquipInfo)));
	m_pCmdMgr->UnRegister("NS_GetOfflineReputation", (NETMSGPROC)(m_Trunk.sfp1(&StrengthRankingFrame::OnNetGetOfflineReputation)));

	if (P_VALID(GetObj("InfoMsgBox")))
		KillObj("InfoMsgBox");

	return TRUE;
}

BOOL StrengthRankingFrame::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\strength.xml");
	m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str());
	m_pPbnStrength = (GUIPushButton*)m_pWnd->GetChild(_T("stren_pic\\fraback\\optbutt1"));
	m_pPbnLevel = (GUIPushButton*)m_pWnd->GetChild(_T("stren_pic\\fraback\\optbutt2"));
	m_pBtnEquip = (GUIPushButton*)m_pWnd->GetChild(_T("stren_pic\\fraback\\optbutt3"));
	m_pPbnFame = (GUIPushButton*)m_pWnd->GetChild(_T("stren_pic\\fraback\\optbutt4"));
	m_pPbnPet = (GUIPushButton*)m_pWnd->GetChild(_T("stren_pic\\fraback\\optbutt5"));
	m_pPbnFlower = (GUIPushButton*)m_pWnd->GetChild(_T("stren_pic\\fraback\\optbutt5"));
	m_pPbnEgg = (GUIPushButton*)m_pWnd->GetChild(_T("stren_pic\\fraback\\optbutt6"));
	m_pStcCommon = (GUIStatic*)m_pWnd->GetChild(_T("stren_pic\\listback\\word4"));
	m_pLstRanking = (GUIListBox*)m_pWnd->GetChild(_T("stren_pic\\listback\\strenlist"));
	m_pBtnPre = (GUIButton*)m_pWnd->GetChild(_T("stren_pic\\backbutt"));
	m_pBtnNext = (GUIButton*)m_pWnd->GetChild(_T("stren_pic\\nextbutt"));
	m_pStcCurNum = (GUIStatic*)m_pWnd->GetChild(_T("stren_pic\\num"));
	m_pStcLocalRank = (GUIStatic*)m_pWnd->GetChild(_T("stren_pic\\word2"));
	m_pBtnClose1 = (GUIButton*)m_pWnd->GetChild(_T("stren_pic\\closebutt"));
	m_pBtnClose2 = (GUIButton*)m_pWnd->GetChild(_T("stren_pic\\canclebutt"));
	m_pBtnInfo = (GUIButton*)m_pWnd->GetChild(_T("stren_pic\\infobutt"));

	m_pLstRanking->SetColNum(4, 80);
	m_pLstRanking->SetColWidth(0, 55);
	m_pLstRanking->SetColWidth(1, 58);
	m_pLstRanking->SetColWidth(2, 96);
	m_pLstRanking->SetColWidth(3, 88);

	m_pBtnPre->SetEnable(false);
	m_pBtnNext->SetEnable(true);

	m_pPbnFame->SetInvisible(false);
	m_pPbnFame->SetText(g_StrTable[_T("PvP_WuXunText")]);

	m_nCurPageNum = 1;				 // 当前页数默认为1
	m_eCurType = ERT_Level;			 // 默认类型为等级
	
	TCHAR szBuffer[X_SHORT_NAME];
	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), m_nCurPageNum);
	m_pStcCurNum->SetText(szBuffer);
	m_pStcCommon->SetText(g_StrTable[_T("Ranking_Level")]);
	return TRUE;
}

BOOL StrengthRankingFrame::EscCancel()
{
	if( m_pWnd->IsInvisible() )
		return FALSE;
	m_pGameFrameMgr->AddToDestroyList(this);
	return TRUE;
}



VOID StrengthRankingFrame::SendGetRankInfo(int nPage, ERankingType eType)
{
	tagNC_GetRankingInfo cmd;
	cmd.eType = eType;
	cmd.nPage = nPage;
	m_pSession->Send(&cmd);
}


DWORD StrengthRankingFrame::EventHandler(tagGUIEvent *pGUIEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pGUIEvent->dwWndID);
	if (!P_VALID(pWnd))
		return 0;

	switch (pGUIEvent->eEvent)
	{
	case EGUIE_ItemClick:
		{
			if (pWnd == m_pLstRanking)
			{
				if (m_eCurType == ERT_Equip)
				{
					DWORD dwCurRow = pGUIEvent->dwParam1;
					DWORD dwRoleID = m_dwRank[dwCurRow];
					if (P_VALID(dwRoleID) && m_dwLastRow != dwCurRow)
					{
						tagOpenEquipEvent e(_T("Open_Equip"), this);
						e.dwRoleID = dwRoleID;
						m_pGameFrameMgr->SendEvent(&e);
					}
					m_dwLastRow = dwCurRow;
					return 0;
				}
				else if (m_eCurType == ERT_Fame)
				{
					DWORD dwCurRow = pGUIEvent->dwParam1;
					DWORD dwRoleID = m_dwRank[dwCurRow];
					if (P_VALID(dwRoleID) && m_dwLastRow != dwCurRow)
					{
						tagOpenReputeEvent e(_T("Open_Repute"), this);
						e.dwRoleID = dwRoleID;
						m_pGameFrameMgr->SendEvent(&e);
					}
					m_dwLastRow = dwCurRow;
					return 0;
				}
			}
		}
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnPre)
			{
				--m_nCurPageNum;
			}
			else if (pWnd == m_pBtnNext)
			{
				++m_nCurPageNum;	
			}
			else if (pWnd == m_pBtnClose1)
			{
				m_pGameFrameMgr->AddToDestroyList(this);
				return 0;
			}
			else if (pWnd == m_pBtnClose2)
			{
				m_pGameFrameMgr->AddToDestroyList(this);
				return 0;
			}
			else if (pWnd == m_pBtnInfo)
			{
				if (P_VALID(GetObj("InfoMsgBox")))
					KillObj("InfoMsgBox");
				if (m_eCurType == ERT_Level)
				{
					CreateObj("InfoMsgBox", "MsgBox");
					TObjRef<MsgBox>("InfoMsgBox")->Init(_T(""), g_StrTable[_T("Ranking_Detail1")], NULL, (MsgBoxFlag)(MBF_OK | MBF_Bigger |MBF_Cancel), TRUE);
				}
				else if (m_eCurType == ERT_Fame)
				{
					CreateObj("InfoMsgBox", "MsgBox");
					TObjRef<MsgBox>("InfoMsgBox")->Init(_T(""), g_StrTable[_T("Ranking_Detail4")], NULL, (MsgBoxFlag)(MBF_OK | MBF_Bigger |MBF_Cancel), TRUE);
				}
				else if (m_eCurType == ERT_Equip)
				{
					CreateObj("InfoMsgBox", "MsgBox");
					TObjRef<MsgBox>("InfoMsgBox")->Init(_T(""), g_StrTable[_T("Ranking_Detail2")], NULL, (MsgBoxFlag)(MBF_OK | MBF_Bigger |MBF_Cancel), TRUE);
				}
				else if (m_eCurType == ERT_Pet)
				{
					CreateObj("InfoMsgBox", "MsgBox");
					TObjRef<MsgBox>("InfoMsgBox")->Init(_T(""), g_StrTable[_T("Ranking_Detail3")], NULL, (MsgBoxFlag)(MBF_OK | MBF_Bigger |MBF_Cancel), TRUE);
				}
				else if (m_eCurType == ERT_Strength)
				{
					CreateObj("InfoMsgBox", "MsgBox");
					TObjRef<MsgBox>("InfoMsgBox")->Init(_T(""), g_StrTable[_T("Ranking_Detail5")], NULL, (MsgBoxFlag)(MBF_OK | MBF_Bigger |MBF_Cancel), TRUE);
				}
				else if (m_eCurType == ERT_WuXun)
				{
					CreateObj("InfoMsgBox", "MsgBox");
					TObjRef<MsgBox>("InfoMsgBox")->Init(_T(""), g_StrTable[_T("Ranking_Detail6")], NULL, (MsgBoxFlag)(MBF_OK | MBF_Bigger |MBF_Cancel), TRUE);
				}
				return 0;
			}
		} break;
	case EGUIE_PushButtonDown:
		{
			if (pWnd == m_pPbnStrength)
			{
				m_nCurPageNum = 1;
				m_dwLastRow = GT_INVALID;
				m_eCurType = ERT_Strength;	
			}
			else if (pWnd == m_pPbnLevel)
			{
				m_nCurPageNum = 1;
				m_dwLastRow = GT_INVALID;
				m_eCurType = ERT_Level;	
			}
			else if (pWnd == m_pBtnEquip)
			{
				m_nCurPageNum = 1;
				m_dwLastRow = GT_INVALID;
				m_eCurType = ERT_Equip;		
			}
			else if (pWnd == m_pPbnFame)
			{
				m_nCurPageNum = 1;
				m_dwLastRow = GT_INVALID;
				m_eCurType = ERT_WuXun;
			}
// 			else if (pWnd == m_pPbnPet)	//搓代码
// 			{
// 				m_nCurPageNum = 1;
// 				m_dwLastRow = GT_INVALID;
// 				m_eCurType = ERT_Pet;
// 			}
			else if(pWnd == m_pPbnFlower)
			{
				m_nCurPageNum = 1;
				m_dwLastRow = GT_INVALID;
				m_eCurType = ERT_Flower;
			}
			else if(pWnd == m_pPbnEgg)
			{
				m_nCurPageNum = 1;
				m_dwLastRow = GT_INVALID;
				m_eCurType = ERT_Egg;
			}
		} break;
	default: return 0;
	}
	UpdateUI();
	SendGetRankInfo(m_nCurPageNum, m_eCurType);
	return 0;
}

DWORD StrengthRankingFrame::OnNetGetRankInfo(tagNS_GetRankingInfo *pNetCmd)
{
	// 如果标记不相等,不处理
	if (m_nCurPageNum != pNetCmd->nPage)
		return 0;
	if (m_eCurType != pNetCmd->eType)
		return 0;

	ZeroMemory(m_dwRank, sizeof(m_dwRank));

	m_pLstRanking->Clear();

	switch (pNetCmd->eType)
	{
	case ERT_Strength:
		{
			m_pStcCommon->SetText(g_StrTable[_T("Ranking_Strength")]);
		} break;
	case ERT_Level:
		{
			m_pStcCommon->SetText(g_StrTable[_T("Ranking_Level")]);
		} break;
	case ERT_Fame:
		{
			m_pStcCommon->SetText(g_StrTable[_T("Ranking_Fame")]);
		} break;
	case ERT_Equip:
		{
			m_pStcCommon->SetText(g_StrTable[_T("Ranking_Equip")]);
		} break;
	case ERT_Pet:
		{
			m_pStcCommon->SetText(g_StrTable[_T("Ranking_Pet")]);
		} break;
	case ERT_WuXun:
		{
			m_pStcCommon->SetText(g_StrTable[_T("Ranking_WuXun")]);
		} break;
	case ERT_Flower:
		{
			m_pStcCommon->SetText(g_StrTable[_T("Ranking_Flower")]);
		} break;
	case ERT_Egg:
		{
			m_pStcCommon->SetText(g_StrTable[_T("Ranking_Egg")]);
		} break;
	default: break;
	}

	TCHAR szBuffer[128];

	for (int i = 0; i < RANKINGS_PAGE_LEN; ++i)
	{
		tagRankingInfo *pRankInfo = &(pNetCmd->sData[i]);
		if (!P_VALID(pRankInfo))
			return 0;
		if (pRankInfo->nCurRanking == 0)
		{
			m_dwRank[i] = GT_INVALID;
			continue;
		}

		m_dwRank[i] = pRankInfo->sIDName.dwID;

		_sntprintf(szBuffer, 128, _T("%d"), pRankInfo->nCurRanking);
		m_pLstRanking->SetText(i, 0, szBuffer);

		int nChange = pRankInfo->nRankingChange;

		// 如果排名
		if (nChange < 0)
		{
			_sntprintf(szBuffer, 128, g_StrTable[_T("StrengthRanking_Down")], -nChange);
			m_pLstRanking->SetText(i, 1, szBuffer, 0xFFFF0000);
		}
		else if (nChange == 0)
		{
			_sntprintf(szBuffer, 128, g_StrTable[_T("StrengthRanking_NoChange")]);
			m_pLstRanking->SetText(i, 1, szBuffer);
		}
		else if (nChange >= 500)
		{
			_sntprintf(szBuffer, 128, g_StrTable[_T("StrengthRanking_Up_More")], (nChange - 500));
			m_pLstRanking->SetText(i, 1, szBuffer, 0xFF00FF00);
		}
		else
		{
			_sntprintf(szBuffer, 128, g_StrTable[_T("StrengthRanking_Up")], nChange);
			m_pLstRanking->SetText(i, 1, szBuffer, 0xFF00FF00);
		}		

		if (m_eCurType != ERT_Pet)
		{
			m_pLstRanking->SetText(i, 2, pRankInfo->sIDName.szName);
			_sntprintf(szBuffer, 128, _T("%d"), (INT32)pRankInfo->n64Value);
			m_pLstRanking->SetText(i, 3, szBuffer);
		}
	}

	if (pNetCmd->nLocalPlayerRank != 0)
	{
		_sntprintf(szBuffer, 128, _T("%d"), pNetCmd->nLocalPlayerRank);
	}
	else
	{
		_sntprintf(szBuffer, 128, g_StrTable[_T("StrengthRanking_Local")]);
	}
	m_pStcLocalRank->SetText(szBuffer);

	m_pWnd->SetRefresh(TRUE);
	return 0;
}

DWORD StrengthRankingFrame::OnNetGetPetRankInfo(tagNS_GetPetRankingInfo *pNetCmd)
{
	if (m_nCurPageNum != pNetCmd->nPage)
		return 0;
	m_pLstRanking->Clear();

	m_pStcCommon->SetText(g_StrTable[_T("Ranking_Pet")]);

	TCHAR szBuffer[128];
	for (int i = 0; i < RANKINGS_PAGE_LEN; ++i)
	{
		tagPetRankingInfo *pRankInfo = &(pNetCmd->sData[i]);
		if (!P_VALID(pRankInfo))
			return 0;
		if (pRankInfo->nCurRanking == 0)
			continue;
		_sntprintf(szBuffer, 128, _T("%d"), pRankInfo->nCurRanking);
		m_pLstRanking->SetText(i, 0, szBuffer);

		int nChange = pRankInfo->nRankingChange;
		if (nChange < 0)
		{
			_sntprintf(szBuffer, 128, g_StrTable[_T("StrengthRanking_Down")], -nChange);
			m_pLstRanking->SetText(i, 1, szBuffer, 0xFFFF0000);
		}
		else if (nChange == 0)
		{
			_sntprintf(szBuffer, 128, g_StrTable[_T("StrengthRanking_NoChange")]);
			m_pLstRanking->SetText(i, 1, szBuffer);
		}
		else if (nChange >= 500)
		{
			_sntprintf(szBuffer, 128, g_StrTable[_T("StrengthRanking_Up_More")], (nChange - 500));
			m_pLstRanking->SetText(i, 1, szBuffer, 0xFF00FF00);
		}
		else
		{
			_sntprintf(szBuffer, 128, g_StrTable[_T("StrengthRanking_Up")], nChange);
			m_pLstRanking->SetText(i, 1, szBuffer, 0xFF00FF00);
		}

		m_pLstRanking->SetText(i, 2, pRankInfo->szPetName);
		m_pLstRanking->SetText(i, 3, pRankInfo->sIDName.szName);
	}

	if (pNetCmd->nLocalPlayerRank != 0)
	{
		_sntprintf(szBuffer, 128, _T("%d"), pNetCmd->nLocalPlayerRank);
	}
	else
	{
		_sntprintf(szBuffer, 128, g_StrTable[_T("StrengthRanking_Local")]);
	}

	m_pStcLocalRank->SetText(szBuffer);

	m_pWnd->SetRefresh(TRUE);
	return 0;

}

DWORD StrengthRankingFrame::OnNetGetOfflineEquipInfo(tagNS_GetOfflineEquipInfo *pNetCmd)
{
	m_pGameFrameMgr->SendEvent(&tagGameEvent(_T("Offline_Player_Equip"), this));
	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Ranking_Offline1")]);
	return 0;
}

DWORD StrengthRankingFrame::OnNetGetOfflineReputation(tagNS_GetOfflineReputation *pNetCmd)
{
	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Ranking_Offline2")]);
	return 0;
}



VOID StrengthRankingFrame::UpdateUI()
{
	if (m_nCurPageNum > 1 && m_nCurPageNum < 25)
	{
		m_pBtnPre->SetEnable(true);
		m_pBtnNext->SetEnable(true);
	}
	if (m_nCurPageNum == 1)
	{
		m_pBtnPre->SetEnable(false);
		m_pBtnNext->SetEnable(true);
	}
	if (m_nCurPageNum == 25)
	{
		m_pBtnPre->SetEnable(true);
		m_pBtnNext->SetEnable(false);
	}
	if( m_nCurPageNum == 3 && (m_eCurType == ERT_Flower||m_eCurType == ERT_Egg) )	//鲜花鸡蛋排行只3件
	{
		m_pBtnPre->SetEnable(true);
		m_pBtnNext->SetEnable(false);
	}

	TCHAR szBuffer[X_SHORT_NAME];
	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), m_nCurPageNum);
	m_pStcCurNum->SetText(szBuffer);
	m_pWnd->SetRefresh(TRUE);
}

