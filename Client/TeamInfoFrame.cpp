#include "StdAfx.h"
#include "TeamInfoFrame.h"
#include "CreateTeamFrame.h"
#include "PlayerNameTab.h"
#include "RoleMgr.h"
#include "TeamEvent.h"

TeamInfoFrame::TeamInfoFrame(void) : 
m_Trunk(this),
m_pWnd(NULL),
m_pWndBack(NULL),
m_curType(EETT_All),
m_dwCurType(GT_INVALID),
m_dwSceneIndex(GT_INVALID)
{
	m_vecID.clear();
	m_vecPerID.clear();
}

TeamInfoFrame::~TeamInfoFrame(void)
{
}

VOID TeamInfoFrame::Update()
{
	if( !m_bFind )
	{
		if( m_pBtnTeam->GetState() == EGUIBS_PushDown )
			ShowTeamInfo();
		else
			ShowPerInfo();
	}
	GameFrame::Update();
}

BOOL TeamInfoFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
	m_pCmdMgr->Register("NS_GetPersonalInfo", (NETMSGPROC)(m_Trunk.sfp2(&TeamInfoFrame::OnNetGetPersonalInfo)), _T("NS_GetPersonalInfo"));
	return bRet;
}

BOOL TeamInfoFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	if( P_VALID(m_pWnd) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;
	}
	m_pCmdMgr->UnRegister("NS_GetPersonalInfo", (NETMSGPROC)(m_Trunk.sfp2(&TeamInfoFrame::OnNetGetPersonalInfo)));
	return bRet;
}

BOOL TeamInfoFrame::EscCancel()
{
	if(m_pWnd->IsInvisible() )
		return FALSE;
	m_pFrameMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL TeamInfoFrame::ReloadUI()
{
	tstring strPath = g_strLocalPath + _T("\\ui\\team_plat.xml");
	m_pWnd = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&TeamInfoFrame::EventHandler));
	m_pWndBack = (GUIPatch*)m_pWnd->GetChild( _T("teamplat_back") );
	m_pBtnTeam = (GUIPushButton*)m_pWndBack->GetChild( _T("modechoose\\teammode") );
	m_pBtnPersonal = (GUIPushButton*)m_pWndBack->GetChild( _T("modechoose\\personalmode") );

	m_pComboBoxTeamType = (GUIComboBox*)m_pWndBack->GetChild( _T("team_mode_back\\chooselist0") );
	m_pComboBoxTeamScene = (GUIComboBox*)m_pWndBack->GetChild( _T("team_mode_back\\chooselist2") );
	m_pComboBoxTeamTypeDetail = (GUIComboBox*)m_pWndBack->GetChild( _T("team_mode_back\\chooselist1") );
	m_pTeamList = (GUIListBox*)m_pWndBack->GetChild( _T("team_mode_back\\listback\\member_list") );
	m_pTeamList->SetColNum(5, 80);
	m_pTeamList->SetColWidth(0, 120);
	m_pTeamList->SetColWidth(1, 60);
	m_pTeamList->SetColWidth(2, 60);
	m_pTeamList->SetColWidth(3, 160);
	m_pTeamList->SetColWidth(4, 190);
	m_pBtnCreateTeam = (GUIButton*)m_pWndBack->GetChild( _T("team_mode_back\\createbutton") );
	m_pBtnRefreshTeam = (GUIButton*)m_pWndBack->GetChild( _T("team_mode_back\\refreshbutton") );
	m_pBtnTeamApp = (GUIButton*)m_pWndBack->GetChild( _T("team_mode_back\\applybutton") );
	m_pWndTeam = (GUIPatch*)m_pWndBack->GetChild( _T("team_mode_back") );

	m_pComboBoxPerType = (GUIComboBox*)m_pWndBack->GetChild( _T("personal_mode_back\\chooselist0") );
	m_pComboBoxPerScene = (GUIComboBox*)m_pWndBack->GetChild( _T("personal_mode_back\\chooselist2") );
	m_pComboBoxPerTypeDetail = (GUIComboBox*)m_pWndBack->GetChild( _T("personal_mode_back\\chooselist1") );
	m_pPerList = (GUIListBox*)m_pWndBack->GetChild( _T("personal_mode_back\\listback\\member_list") );
	m_pPerList->SetColNum(4, 80);
	m_pPerList->SetColWidth(0, 120);
	m_pPerList->SetColWidth(1, 50);
	m_pPerList->SetColWidth(2, 160);
	m_pPerList->SetColWidth(3, 260);
	m_pBtnCreatePer = (GUIButton*)m_pWndBack->GetChild( _T("personal_mode_back\\applybutton") );
	m_pBtnRefreshPer = (GUIButton*)m_pWndBack->GetChild( _T("personal_mode_back\\refreshbutton") );
	m_pBtnPerInv = (GUIButton*)m_pWndBack->GetChild( _T("personal_mode_back\\invitebutton") );
	m_pWndPer = (GUIPatch*)m_pWndBack->GetChild( _T("personal_mode_back") );

	m_pBtnExit = (GUIButton*)m_pWndBack->GetChild( _T("closebutton") );
	m_pBtnTeam->SetState(EGUIBS_PushDown);
	m_pWndTeam->SetInvisible(FALSE);
	m_pWndPer->SetInvisible(TRUE);

	InitData();
	return TRUE;
}

DWORD TeamInfoFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Active:
		if( pWnd == m_pComboBoxPerType->GetEditBox() )
			m_pGUI->SetActive( m_pComboBoxPerType->GetListBox() );
		else if( pWnd == m_pComboBoxPerTypeDetail->GetEditBox() )
			m_pGUI->SetActive( m_pComboBoxPerTypeDetail->GetListBox() );
		else if( pWnd == m_pComboBoxTeamType->GetEditBox() )
			m_pGUI->SetActive( m_pComboBoxTeamType->GetListBox() );
		else if( pWnd == m_pComboBoxTeamTypeDetail->GetEditBox() )
			m_pGUI->SetActive( m_pComboBoxTeamTypeDetail->GetListBox() );
		break;
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnExit )
				m_pMgr->AddToDestroyList(this);
			else if( pWnd == m_pBtnCreateTeam || m_pBtnCreatePer == pWnd )
			{
				CreateTeamFrame *pFrame = (CreateTeamFrame*)(TObjRef<GameFrameMgr>()->GetFrame(_T("CreateTeam")));
				if(!P_VALID(pFrame))
					pFrame = (CreateTeamFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("CreateTeam"), _T("CreateTeamFrame"), 0 );
				if(P_VALID(pFrame))
				{
					if(pWnd == m_pBtnCreateTeam)
						pFrame->SetTeamFlag(TRUE);
					else
						pFrame->SetTeamFlag(FALSE);
				}
				m_pMgr->AddToDestroyList(this);
			}
			else if( pWnd == m_pBtnRefreshTeam )
			{
				m_dwRoleID = GT_INVALID;
				tagNC_GetEasyTeamInfo m;
				m_pSession->Send(&m);
			}
			else if( pWnd == m_pBtnRefreshPer )
			{
				m_dwRoleID = GT_INVALID;
				tagNC_GetPersonalInfo m;
				m_pSession->Send(&m);
			}
			else if( pWnd == m_pBtnTeamApp )
			{
				if(m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
				{
					tagNC_DeleteMyTeamInfo m;
					m.dwRoleID = m_dwRoleID;
					m_pSession->Send(&m);
					tagNC_GetEasyTeamInfo msg;
					m_pSession->Send(&msg);
				}
				else
				{
					tagApplyJoinTeamEvent evt( _T("tagApplyJoinTeamEvent"),this );
					evt.dwRoleID = m_dwRoleID;
					m_pMgr->SendEvent( &evt );
				}
			}
			else if( pWnd == m_pBtnPerInv )
			{
				if(m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID())
				{
					tagNC_DeleteAppTeamInfo m;
					m.dwRoleID = m_dwRoleID;
					m_pSession->Send(&m);
					tagNC_GetPersonalInfo msg;
					m_pSession->Send(&msg);
				}
				else
				{
					tagInviteJoinTeamEvent e(_T("tagInviteJoinTeamEvent"), this);
					e.dwRoleID = m_dwRoleID;
					m_pMgr->SendEvent(&e);
				}
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if(pWnd == m_pBtnTeam)
			{
				m_curType = EETT_All;
				m_dwCurType = GT_INVALID;
				m_dwSceneIndex = GT_INVALID;
				m_dwRoleID = GT_INVALID;
				m_pBtnTeamApp->SetText(g_StrTable[_T("AppJoin")]);
				m_pBtnTeamApp->SetEnable(false);
				m_pWndTeam->SetInvisible(FALSE);
				m_pWndPer->SetInvisible(TRUE);
				ChangeScene();
				ChangeTeamTypeDetail();
				ShowTeamInfo();
			}
			else if( pWnd == m_pBtnPersonal )
			{
				m_curType = EETT_All;
				m_dwCurType = GT_INVALID;
				m_dwSceneIndex = GT_INVALID;
				m_dwRoleID = GT_INVALID;
				m_pBtnPerInv->SetText(g_StrTable[_T("InvJoin")]);
				m_pBtnPerInv->SetEnable(false);
				m_pWndTeam->SetInvisible(TRUE);
				m_pWndPer->SetInvisible(FALSE);
				ChangeScene();
				ChangeTeamTypeDetail();
				ShowPerInfo();
				if(m_vecPerID.empty())
				{
					tagNC_GetPersonalInfo m;
					m_pSession->Send(&m);
				}
			}
		}
		break;
	case EGUIE_ItemClick:
		{
			if( pWnd == m_pTeamList && pEvent->dwParam1 != GT_INVALID && m_vecID.size() > pEvent->dwParam1 )
			{
				m_dwRoleID = m_vecID[pEvent->dwParam1];
				if( m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
				{
					m_pBtnTeamApp->SetText(g_StrTable[_T("DeleteTeam")]);
					m_pBtnTeamApp->SetEnable(true);
					m_pBtnTeamApp->SetRefresh(TRUE);
				}
				else if( m_dwRoleID != GT_INVALID && !TeamSys::Inst()->IsInTeam() )
				{
					m_pBtnTeamApp->SetText(g_StrTable[_T("AppJoin")]);
					m_pBtnTeamApp->SetEnable(true);
					m_pBtnTeamApp->SetRefresh(TRUE);
				}
			}
			else if( pWnd == m_pPerList && pEvent->dwParam1 != GT_INVALID && m_vecPerID.size() > pEvent->dwParam1 )
			{
				m_dwRoleID = m_vecPerID[pEvent->dwParam1];
				if( m_dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
				{
					m_pBtnPerInv->SetText(g_StrTable[_T("DeleteAppTeam")]);
					m_pBtnPerInv->SetEnable(true);
					m_pBtnPerInv->SetRefresh(TRUE);
				}
				else if( m_dwRoleID != GT_INVALID && 
					( (TeamSys::Inst()->IsLeader(m_dwRoleID) && !TeamSys::Inst()->IsTeamFull()) || !TeamSys::Inst()->IsInTeam() ) )
				{
					m_pBtnPerInv->SetText(g_StrTable[_T("InvJoin")]);
					m_pBtnPerInv->SetEnable(true);
					m_pBtnPerInv->SetRefresh(TRUE);
				}
			}
			else if( pWnd == m_pComboBoxTeamType->GetListBox() )
			{
				m_curType = (EEasyTeamType)(pEvent->dwParam1 + 1);
				ChangeScene();
				ChangeTeamTypeDetail();
				ShowTeamInfo();
			}
			else if( pWnd == m_pComboBoxPerType->GetListBox() )
			{
				m_curType = (EEasyTeamType)(pEvent->dwParam1 + 1);
				ChangeScene();
				ChangeTeamTypeDetail();
				ShowPerInfo();
			}
			else if( pWnd == m_pComboBoxTeamScene->GetListBox() )
			{
				if(!m_vecScene.empty())
					m_dwSceneIndex = m_vecScene[pEvent->dwParam1];
				ChangeTeamTypeDetail();
				ShowTeamInfo();
			}
			else if( pWnd == m_pComboBoxPerScene->GetListBox() )
			{
				if(!m_vecScene.empty())
					m_dwSceneIndex = m_vecScene[pEvent->dwParam1];
				ChangeTeamTypeDetail();
				ShowPerInfo();
			}
			else if( pWnd == m_pComboBoxTeamTypeDetail->GetListBox() )
			{
				if(!m_vecSceneAndActive.empty())
					m_dwCurType = m_vecSceneAndActive[pEvent->dwParam1];
				ShowTeamInfo();
			}
			else if( pWnd == m_pComboBoxPerTypeDetail->GetListBox() )
			{
				if(!m_vecSceneAndActive.empty())
					m_dwCurType = m_vecSceneAndActive[pEvent->dwParam1];
				ShowPerInfo();
			}
		}
		break;
	}
	return 0;
}

VOID TeamInfoFrame::ShowTeamInfo()
{
	m_bFind = TRUE;
	m_vecID.clear();
	m_pTeamList->Clear();
	INT nTotalRow = 0;
	DWORD dwColor = GT_INVALID;
	if( m_curType == EETT_Other || EETT_All == m_curType )
	{
		m_dwCurType = GT_INVALID;
		m_dwSceneIndex = GT_INVALID;
	}
	const vector<tagClientEasyTeamSimInfo> vecTeamInfo = TeamSys::Inst()->GetTeamList(m_curType);
	if( m_dwCurType == GT_INVALID )
	{
		for( size_t i = 0; i < vecTeamInfo.size(); i++ )
		{
			m_vecID.push_back(vecTeamInfo[i].dwLeaderID);
			tstring strName = PlayerNameTab::Inst()->FindNameByID( vecTeamInfo[i].dwLeaderID );
			if(strName.empty())
				m_bFind = FALSE;
			m_pTeamList->SetText( nTotalRow, 0, strName.c_str(), dwColor );
			TCHAR szTmp[X_SHORT_NAME];
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), vecTeamInfo[i].byLeaderLevel);
			m_pTeamList->SetText( nTotalRow, 1, szTmp, dwColor );
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), (vecTeamInfo[i].wRoles >> 8) & 0xFF, vecTeamInfo[i].wRoles & 0xFF);
			m_pTeamList->SetText( nTotalRow, 2, szTmp, dwColor );
			strName = TeamSys::Inst()->GetInstanceOrActivityName(vecTeamInfo[i].byInsAndActi);
			m_pTeamList->SetText( nTotalRow, 3, strName.c_str(), dwColor );
			m_pTeamList->SetText( nTotalRow, 4, vecTeamInfo[i].strContent.c_str(), dwColor );
			nTotalRow++;
		}
	}
	else
	{
		for( size_t i = 0; i < vecTeamInfo.size(); i++ )
		{
			BYTE bySceneIndex = TeamSys::Inst()->GetSceneIndex(vecTeamInfo[i].byInsAndActi);
			if( m_dwCurType == vecTeamInfo[i].byInsAndActi && m_dwSceneIndex == bySceneIndex )
			{
				m_vecID.push_back(vecTeamInfo[i].dwLeaderID);
				tstring strName = PlayerNameTab::Inst()->FindNameByID( vecTeamInfo[i].dwLeaderID );
				if(strName.empty())
					m_bFind = FALSE;
				m_pTeamList->SetText( nTotalRow, 0, strName.c_str(), dwColor );
				TCHAR szTmp[X_SHORT_NAME];
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), vecTeamInfo[i].byLeaderLevel);
				m_pTeamList->SetText( nTotalRow, 1, szTmp, dwColor );
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), (vecTeamInfo[i].wRoles >> 8) & 0xFF, vecTeamInfo[i].wRoles & 0xFF);
				m_pTeamList->SetText( nTotalRow, 2, szTmp, dwColor );
				strName = TeamSys::Inst()->GetInstanceOrActivityName(vecTeamInfo[i].byInsAndActi);
				m_pTeamList->SetText( nTotalRow, 3, strName.c_str(), dwColor );
				m_pTeamList->SetText( nTotalRow, 4, vecTeamInfo[i].strContent.c_str(), dwColor );
				nTotalRow++;
			}
		}
	}

	tagPoint ptSizeOldListBox = m_pTeamList->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pTeamList->GetRowHeight() * nTotalRow;
	m_pTeamList->SetResizable( TRUE );
	m_pTeamList->Resize( ptSizeListBox );
	m_pTeamList->SetResizable( FALSE );
	m_pTeamList->SetRefresh(TRUE);
	m_pTeamList->SetCurSelectedRow(GT_INVALID);
}

DWORD TeamInfoFrame::OnNetGetPersonalInfo(tagNS_GetPersonalInfo *pNetCmd, DWORD)
{
	m_vecPerMode.clear();
	m_vecID.clear();
	BYTE *pTemp = (BYTE*)&pNetCmd->Person[0];
	for( int i = 0; i < pNetCmd->wNums; i++ )
	{
		tagClientPersonalMode PerInfo;
		PerInfo.byType = *(BYTE*)pTemp;
		pTemp += sizeof(BYTE);
		PerInfo.dwRoleID = *(DWORD*)pTemp;
		pTemp += sizeof(DWORD);
		PerInfo.byLevel = *(BYTE*)pTemp;
		pTemp += sizeof(BYTE);
		PerInfo.byInsAndActi = *(BYTE*)pTemp;
		pTemp += sizeof(BYTE);
		PerInfo.byLen = *(BYTE*)pTemp;
		pTemp += sizeof(BYTE);
		if(PerInfo.byLen >0)
		{
			PerInfo.strContent.resize(PerInfo.byLen+2);
			_tcsncpy(const_cast<TCHAR*>(PerInfo.strContent.c_str()),(TCHAR*)pTemp,PerInfo.byLen);
			PerInfo.strContent[PerInfo.byLen] = 0;
			pTemp += PerInfo.byLen*sizeof(TCHAR);
		}
		else
			pTemp += sizeof(BYTE);
		m_vecPerMode.push_back(PerInfo);
		m_vecID.push_back(PerInfo.dwRoleID);
	}
	ShowPerInfo();
	return 0;
}


VOID TeamInfoFrame::ShowPerInfo()
{
	m_bFind = TRUE;
	m_vecPerID.clear();
	m_pPerList->Clear();
	INT nTotalRow = 0;
	DWORD dwColor = GT_INVALID;

	if( m_dwCurType == GT_INVALID )
	{
		for( size_t i = 0; i < m_vecPerMode.size(); i++ )
		{
			if( m_curType == EETT_All || m_curType == m_vecPerMode[i].byType )
			{
				m_vecPerID.push_back(m_vecPerMode[i].dwRoleID);
				tstring strName = PlayerNameTab::Inst()->FindNameByID( m_vecPerMode[i].dwRoleID );
				if(strName.empty())
					m_bFind = FALSE;
				m_pPerList->SetText( nTotalRow, 0, strName.c_str(), dwColor );
				TCHAR szTmp[X_SHORT_NAME];
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_vecPerMode[i].byLevel);
				m_pPerList->SetText( nTotalRow, 1, szTmp, dwColor );
				strName = TeamSys::Inst()->GetInstanceOrActivityName(m_vecPerMode[i].byInsAndActi);
				m_pPerList->SetText( nTotalRow, 2, strName.c_str(), dwColor );
				m_pPerList->SetText( nTotalRow, 3, m_vecPerMode[i].strContent.c_str(), dwColor );
				nTotalRow++;
			}
		}
	}
	else
	{
		for( size_t i = 0; i < m_vecPerMode.size(); i++ )
		{
			BYTE bySceneIndex = TeamSys::Inst()->GetSceneIndex(m_vecPerMode[i].byInsAndActi);
			if( m_dwCurType == m_vecPerMode[i].byInsAndActi && m_curType == m_vecPerMode[i].byType && bySceneIndex == m_dwSceneIndex )
			{
				m_vecPerID.push_back(m_vecPerMode[i].dwRoleID);
				tstring strName = PlayerNameTab::Inst()->FindNameByID( m_vecPerMode[i].dwRoleID );
				if(strName.empty())
					m_bFind = FALSE;
				m_pPerList->SetText( nTotalRow, 0, strName.c_str(), dwColor );
				TCHAR szTmp[X_SHORT_NAME];
				_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_vecPerMode[i].byLevel);
				m_pPerList->SetText( nTotalRow, 1, szTmp, dwColor );
				strName = TeamSys::Inst()->GetInstanceOrActivityName(m_vecPerMode[i].byInsAndActi);
				m_pPerList->SetText( nTotalRow, 2, strName.c_str(), dwColor );
				m_pPerList->SetText( nTotalRow, 3, m_vecPerMode[i].strContent.c_str(), dwColor );
				nTotalRow++;
			}
		}
	}

	tagPoint ptSizeOldListBox = m_pPerList->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pPerList->GetRowHeight() * nTotalRow;
	m_pPerList->SetResizable( TRUE );
	m_pPerList->Resize( ptSizeListBox );
	m_pPerList->SetResizable( FALSE );
	m_pPerList->SetRefresh(TRUE);
	m_pPerList->SetCurSelectedRow(GT_INVALID);
}

VOID TeamInfoFrame::InitData()
{
	//填充队伍类型
	{
		// 副本
		m_pComboBoxTeamType->GetListBox()->SetText(0, 0, g_StrTable[_T("Team_Instance")]);
		// 活动
		m_pComboBoxTeamType->GetListBox()->SetText(1, 0, g_StrTable[_T("Team_Activity")]);
		// 其他
		m_pComboBoxTeamType->GetListBox()->SetText(2, 0, g_StrTable[_T("Team_Other")]);
		// 所有
		m_pComboBoxTeamType->GetListBox()->SetText(3, 0, g_StrTable[_T("Team_All")]);
		tagPoint ptSize	= m_pComboBoxTeamType->GetListBox()->GetSize();
		ptSize.x = m_pComboBoxTeamType->GetEditBox()->GetSize().x;
		ptSize.y = m_pComboBoxTeamType->GetListBox()->GetRowHeight() * EETT_All;
		m_pComboBoxTeamType->GetListBox()->SetResizable(TRUE);
		m_pComboBoxTeamType->GetListBox()->SetColWidth(0, ptSize.x);
		m_pComboBoxTeamType->GetListBox()->Resize(ptSize);
		m_pComboBoxTeamType->GetListBox()->SetResizable(FALSE);
		tstring strTemp = m_pComboBoxTeamType->GetListBox()->GetText(3, 0);
		m_pComboBoxTeamType->GetEditBox()->SetText(strTemp.c_str());

		m_pComboBoxTeamScene->GetListBox()->SetText(0, 0, g_StrTable[_T("Team_Detail")]);
		ptSize	= m_pComboBoxTeamScene->GetListBox()->GetSize();
		ptSize.x = m_pComboBoxTeamScene->GetEditBox()->GetSize().x;
		ptSize.y = m_pComboBoxTeamScene->GetListBox()->GetRowHeight() * 1;
		m_pComboBoxTeamScene->GetListBox()->SetResizable(TRUE);
		m_pComboBoxTeamScene->GetListBox()->SetColWidth(0, ptSize.x);
		m_pComboBoxTeamScene->GetListBox()->Resize(ptSize);
		m_pComboBoxTeamScene->GetListBox()->SetResizable(FALSE);
		m_pComboBoxTeamScene->GetEditBox()->SetText(g_StrTable[_T("Team_Detail")]);

		m_pComboBoxTeamTypeDetail->GetListBox()->SetText(0, 0, g_StrTable[_T("Team_Detail")]);
		ptSize	= m_pComboBoxTeamTypeDetail->GetListBox()->GetSize();
		ptSize.x = m_pComboBoxTeamTypeDetail->GetEditBox()->GetSize().x;
		ptSize.y = m_pComboBoxTeamTypeDetail->GetListBox()->GetRowHeight() * 1;
		m_pComboBoxTeamTypeDetail->GetListBox()->SetResizable(TRUE);
		m_pComboBoxTeamTypeDetail->GetListBox()->SetColWidth(0, ptSize.x);
		m_pComboBoxTeamTypeDetail->GetListBox()->Resize(ptSize);
		m_pComboBoxTeamTypeDetail->GetListBox()->SetResizable(FALSE);
		m_pComboBoxTeamTypeDetail->GetEditBox()->SetText(g_StrTable[_T("Team_Detail")]);

		// 副本
		m_pComboBoxPerType->GetListBox()->SetText(0, 0, g_StrTable[_T("Team_Instance")]);
		// 活动
		m_pComboBoxPerType->GetListBox()->SetText(1, 0, g_StrTable[_T("Team_Activity")]);
		// 其他
		m_pComboBoxPerType->GetListBox()->SetText(2, 0, g_StrTable[_T("Team_Other")]);
		// 所有
		m_pComboBoxPerType->GetListBox()->SetText(3, 0, g_StrTable[_T("Team_All")]);
		ptSize	= m_pComboBoxPerType->GetListBox()->GetSize();
		ptSize.x = m_pComboBoxPerType->GetEditBox()->GetSize().x;
		ptSize.y = m_pComboBoxPerType->GetListBox()->GetRowHeight() * EETT_All;
		m_pComboBoxPerType->GetListBox()->SetResizable(TRUE);
		m_pComboBoxPerType->GetListBox()->SetColWidth(0, ptSize.x);
		m_pComboBoxPerType->GetListBox()->Resize(ptSize);
		m_pComboBoxPerType->GetListBox()->SetResizable(FALSE);
		strTemp = m_pComboBoxPerType->GetListBox()->GetText(3, 0);
		m_pComboBoxPerType->GetEditBox()->SetText(strTemp.c_str());

		m_pComboBoxPerScene->GetListBox()->SetText(0, 0, g_StrTable[_T("Team_Detail")]);
		ptSize	= m_pComboBoxPerScene->GetListBox()->GetSize();
		ptSize.x = m_pComboBoxPerScene->GetEditBox()->GetSize().x;
		ptSize.y = m_pComboBoxPerScene->GetListBox()->GetRowHeight() * 1;
		m_pComboBoxPerScene->GetListBox()->SetResizable(TRUE);
		m_pComboBoxPerScene->GetListBox()->SetColWidth(0, ptSize.x);
		m_pComboBoxPerScene->GetListBox()->Resize(ptSize);
		m_pComboBoxPerScene->GetListBox()->SetResizable(FALSE);
		m_pComboBoxPerScene->GetEditBox()->SetText(g_StrTable[_T("Team_Detail")]);

		m_pComboBoxPerTypeDetail->GetListBox()->SetText(0, 0, g_StrTable[_T("Team_Detail")]);
		ptSize	= m_pComboBoxPerTypeDetail->GetListBox()->GetSize();
		ptSize.x = m_pComboBoxPerTypeDetail->GetEditBox()->GetSize().x;
		ptSize.y = m_pComboBoxPerTypeDetail->GetListBox()->GetRowHeight() * 1;
		m_pComboBoxPerTypeDetail->GetListBox()->SetResizable(TRUE);
		m_pComboBoxPerTypeDetail->GetListBox()->SetColWidth(0, ptSize.x);
		m_pComboBoxPerTypeDetail->GetListBox()->Resize(ptSize);
		m_pComboBoxPerTypeDetail->GetListBox()->SetResizable(FALSE);
		m_pComboBoxPerTypeDetail->GetEditBox()->SetText(g_StrTable[_T("Team_Detail")]);
	}
}

VOID TeamInfoFrame::ChangeTeamTypeDetail()
{
	if( m_curType != EETT_KillCreature )
	{
		tstring strTemp = m_pComboBoxTeamType->GetListBox()->GetText(m_curType-1, 0);
		m_pComboBoxTeamType->GetEditBox()->SetText(strTemp.c_str());
		m_pComboBoxPerType->GetEditBox()->SetText(strTemp.c_str());
	}
	m_vecSceneAndActive.clear();
	GUIComboBox *pName;
	if(m_pBtnTeam->GetState() == EGUIBS_PushDown )
		pName = m_pComboBoxTeamTypeDetail;
	else
		pName = m_pComboBoxPerTypeDetail;
	INT nRow = 0;

	switch(m_curType)
	{
	case EETT_Instance:
	case EETT_Activity:
		{
			const vector<tagTeamData> vecTeamData = TeamSys::Inst()->GetTeamData();
			for( size_t i = 0; i < vecTeamData.size(); i++ )
			{
				if( vecTeamData[i].byType == m_curType && m_dwSceneIndex == vecTeamData[i].bySceneIndex )
				{
					pName->GetListBox()->SetText(nRow, 0, vecTeamData[i].strTypeName.c_str());
					m_vecSceneAndActive.push_back((BYTE)vecTeamData[i].dwID);
					nRow++;
				}
			}
		}
		break;
	case EETT_Other:
	case EETT_All:
	default:
		pName->GetListBox()->SetText(nRow, 0, g_StrTable[_T("Team_Detail")]);
		m_dwCurType = GT_INVALID;
		nRow++;
		break;
	}

	tagPoint ptSize	= pName->GetListBox()->GetSize();
	ptSize.x = pName->GetEditBox()->GetSize().x;
	ptSize.y = pName->GetListBox()->GetRowHeight() * nRow;
	pName->GetListBox()->SetResizable(TRUE);
	pName->GetListBox()->SetColWidth(0, ptSize.x);
	pName->GetListBox()->Resize(ptSize);
	pName->GetListBox()->SetResizable(FALSE);
	tstring strTemp = pName->GetListBox()->GetText(0, 0);
	pName->GetEditBox()->SetText(strTemp.c_str());
	if(!m_vecSceneAndActive.empty())
		m_dwCurType = m_vecSceneAndActive[0];
}


VOID TeamInfoFrame::ChangeScene()
{
	if( m_curType != EETT_KillCreature )
	{
		tstring strTemp = m_pComboBoxTeamType->GetListBox()->GetText(m_curType-1, 0);
		m_pComboBoxTeamType->GetEditBox()->SetText(strTemp.c_str());
		m_pComboBoxPerType->GetEditBox()->SetText(strTemp.c_str());
	}
	m_vecScene.clear();
	GUIComboBox *pName;
	if(m_pBtnTeam->GetState() == EGUIBS_PushDown )
		pName = m_pComboBoxTeamScene;
	else
		pName = m_pComboBoxPerScene;
	INT nRow = 0;

	switch(m_curType)
	{
	case EETT_Instance:
	case EETT_Activity:
		{
			const vector<tagTeamData> vecTeamData = TeamSys::Inst()->GetTeamData();
			for( size_t i = 0; i < vecTeamData.size(); i++ )
			{
				vector<BYTE>::iterator iter = find(m_vecScene.begin(), m_vecScene.end(), vecTeamData[i].bySceneIndex);
				if( iter == m_vecScene.end() && m_curType == vecTeamData[i].byType )
				{
					pName->GetListBox()->SetText(nRow, 0, vecTeamData[i].strSceneName.c_str());
					m_vecScene.push_back((BYTE)vecTeamData[i].bySceneIndex);
					nRow++;
				}
			}
		}
		break;
	case EETT_Other:
	case EETT_All:
	default:
		pName->GetListBox()->SetText(nRow, 0, g_StrTable[_T("Team_Detail")]);
		m_dwSceneIndex = GT_INVALID;
		nRow++;
		break;
	}

	tagPoint ptSize	= pName->GetListBox()->GetSize();
	ptSize.x = pName->GetEditBox()->GetSize().x;
	ptSize.y = pName->GetListBox()->GetRowHeight() * nRow;
	pName->GetListBox()->SetResizable(TRUE);
	pName->GetListBox()->SetColWidth(0, ptSize.x);
	pName->GetListBox()->Resize(ptSize);
	pName->GetListBox()->SetResizable(FALSE);
	tstring strTemp = pName->GetListBox()->GetText(0, 0);
	pName->GetEditBox()->SetText(strTemp.c_str());
	if(!m_vecScene.empty())
		m_dwSceneIndex = m_vecScene[0];
}