#include "StdAfx.h"
#include "Role.h"
#include "Player.h"
#include "RoleMgr.h"
#include "TeamSys.h"
#include "TeamEvent.h"
#include "EasyTeamFrame.h"
#include "PlayerNameTab.h"
#include "TeamInfoFrame.h"

EasyTeamFrame::EasyTeamFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndBack = NULL;
	m_pTeamList = NULL;
	m_pPlayerList = NULL;
	m_pBtnTeam = NULL;
	m_pBtnPerson = NULL;
	m_pBtnQuit = NULL;
	m_pBtnTeamInv = NULL;
	m_pBtnPerInv = NULL;
	m_pBtnRefresh = NULL;
	m_pWndTeam = NULL;
	m_pWndPlayer = NULL;
	m_dwRoleID = GT_INVALID;
	m_pBtnPerRefresh = NULL;
	m_bDestroy = false;
}

EasyTeamFrame::~EasyTeamFrame()
{

}

BOOL EasyTeamFrame::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if( !GameFrame::Init( pMgr, pFather, dwParam ) )
		return false;

	m_pNetCmdMgr->Register("NS_GetSinglePlayers", (NETMSGPROC)(m_Trunk.sfp2(&EasyTeamFrame::OnNetGetSinglePlayers)), _T("NS_GetSinglePlayers"));

	m_bStartCountTime = false;
	return true;
}

BOOL EasyTeamFrame::Destroy()
{
	GameFrame::Destroy();
	if( P_VALID(m_pWnd) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;
	}

	m_pNetCmdMgr->UnRegister("NS_GetSinglePlayers", (NETMSGPROC)(m_Trunk.sfp2(&EasyTeamFrame::OnNetGetSinglePlayers)));
	return true;
}

BOOL EasyTeamFrame::EscCancel()
{
	if(m_pWnd->IsInvisible() )
		return FALSE;
	m_pFrameMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL EasyTeamFrame::ReloadUI()
{
	tstring strPath = g_strLocalPath + _T("\\ui\\team_convi.xml");
	m_pWnd = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	m_pWndBack = (GUIPatch*)m_pWnd->GetChild( _T("team_convi_back") );
	m_pWndTeam = (GUIPatch*)m_pWndBack->GetChild( _T("teamback") );
	m_pWndPlayer = (GUIPatch*)m_pWndBack->GetChild( _T("personback") );
	m_pBtnTeam = (GUIPushButton*)m_pWndBack->GetChild(_T("teamchoose\\teamchoose0"));
	m_pBtnPerson = (GUIPushButton*)m_pWndBack->GetChild(_T("teamchoose\\teamchoose1"));
	m_pBtnActive = (GUIPushButton*)m_pWndBack->GetChild(_T("teamchoose\\teamchoose2"));
	m_pTeamList = (GUIListBox*)m_pWndBack->GetChild(_T("teamback\\listpic\\list"));
	m_pTeamList->SetColNum(3, 80);
	m_pTeamList->SetColWidth(0, 130);
	m_pTeamList->SetColWidth(1, 90);
	m_pTeamList->SetColWidth(2, 90);
	m_pPlayerList = (GUIListBox*)m_pWndBack->GetChild(_T("personback\\listpic\\list"));
	m_pPlayerList->SetColNum(2, 80);
	m_pPlayerList->SetColWidth(0, 170);
	m_pPlayerList->SetColWidth(1, 140);
	m_pBtnPerInv = (GUIButton*)m_pWndBack->GetChild(_T("personback\\invite_button"));
	m_pBtnTeamInv = (GUIButton*)m_pWndBack->GetChild(_T("teamback\\apply_button"));
	m_pBtnQuit = (GUIButton*)m_pWndBack->GetChild(_T("close"));
	m_pBtnRefresh = (GUIButton*)m_pWndBack->GetChild(_T("teamback\\refresh_button"));
	m_pBtnPerRefresh = (GUIButton*)m_pWndBack->GetChild(_T("personback\\refresh_button"));
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&EasyTeamFrame::EventHandler) );
	m_pBtnTeam->SetState(EGUIBS_PushDown,TRUE);
	tagNC_GetEasyTeamInfo m;
	m_pSession->Send(&m);
	return true;
}

DWORD EasyTeamFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnQuit )
				m_pFrameMgr->AddToDestroyList(this);
			else if( pWnd == m_pBtnRefresh )
			{
				tagNC_GetEasyTeamInfo m;
				m_pSession->Send(&m);
				m_pBtnRefresh->SetEnable(FALSE);
				m_dwDisableTime = 0;
				m_bStartCountTime = true;
			}
			else if( pWnd == m_pBtnPerRefresh )
			{
				tagNC_GetSinglePlayers m;
				m_pSession->Send(&m);
			}
			else if( pWnd == m_pBtnTeamInv )
			{
				tagApplyJoinTeamEvent evt( _T("tagApplyJoinTeamEvent"),this );
				evt.dwRoleID = m_dwRoleID;
				m_pMgr->SendEvent( &evt );
			}
			else if( pWnd == m_pBtnPerInv )
			{
				tagInviteJoinTeamEvent e(_T("tagInviteJoinTeamEvent"), this);
				e.dwRoleID = m_dwRoleID;
				m_pMgr->SendEvent(&e);
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if( pWnd == m_pBtnTeam )
			{
				ShowTeamInfo();
				m_pWndTeam->SetInvisible(FALSE);
				m_pWndPlayer->SetInvisible(TRUE);
				m_dwRoleID = GT_INVALID;
			}
			else if( pWnd == m_pBtnPerson )
			{
				m_pWndTeam->SetInvisible(TRUE);
				m_pWndPlayer->SetInvisible(FALSE);
				m_dwRoleID = GT_INVALID;
				tagNC_GetSinglePlayers m;
				m_pSession->Send(&m);
			}
			else if( pWnd == m_pBtnActive )
			{
				TeamInfoFrame *pFrame = (TeamInfoFrame*)(TObjRef<GameFrameMgr>()->GetFrame(_T("TeamInfo")));
				if(!P_VALID(pFrame))
					TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("TeamInfo"), _T("TeamInfoFrame"), 0 );
				m_bDestroy = true;
			}
		}
		break;
	case EGUIE_ItemClick:
		{
			if( pWnd == m_pTeamList && pEvent->dwParam1 != GT_INVALID && m_vecKillCreTeam.size() > pEvent->dwParam1 )
				m_dwRoleID = m_vecKillCreTeam[pEvent->dwParam1];	
			else if( pWnd == m_pPlayerList && pEvent->dwParam1 != GT_INVALID && m_vecNearbyPlayer.size() > pEvent->dwParam1 )
				m_dwRoleID = m_vecNearbyPlayer[pEvent->dwParam1];		
		}
		break;
	default:
		break;
	}

	return FALSE;
}

void EasyTeamFrame::ShowTeamInfo()
{		
	m_pTeamList->Clear();
	INT nTotalRow = 0;
	DWORD dwColor = GT_INVALID;
	vector<tagClientEasyTeamSimInfo> vecTeamInfo = TeamSys::Inst()->GetTeamList(EETT_KillCreature);
	for( size_t i = 0; i < vecTeamInfo.size(); i++ )
	{
		m_vecKillCreTeam.push_back(vecTeamInfo[i].dwLeaderID);
		tstring strName = PlayerNameTab::Inst()->FindNameByID( vecTeamInfo[i].dwLeaderID );
		m_pTeamList->SetText( nTotalRow, 0, strName.c_str(), dwColor );
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), vecTeamInfo[i].byLeaderLevel);
		m_pTeamList->SetText( nTotalRow, 1, szTmp, dwColor );
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), (vecTeamInfo[i].wRoles >> 8) & 0xFF, vecTeamInfo[i].wRoles & 0xFF);
		m_pTeamList->SetText( nTotalRow, 2, szTmp, dwColor );
		nTotalRow++;
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

void EasyTeamFrame::Update()
{
	if(m_pBtnPerson->GetState() == EGUIBS_PushDown)
	{
		if( !m_vecNearbyPlayer.empty() && m_dwRoleID != GT_INVALID && 
			((TeamSys::Inst()->IsInTeam() && TeamSys::Inst()->IsLeader(RoleMgr::Inst()->GetLocalPlayerID())) || !TeamSys::Inst()->IsInTeam()) )
			m_pBtnPerInv->SetEnable(true);
		else
			m_pBtnPerInv->SetEnable(true/*false*/);
		m_pBtnPerInv->SetRefresh(true);
	}
	else if( m_pBtnTeam->GetState() == EGUIBS_PushDown)
	{
		if(TeamSys::Inst()->IsInTeam() || m_vecKillCreTeam.empty() || 
			m_dwRoleID == GT_INVALID)
			m_pBtnTeamInv->SetEnable(false);
		else
			m_pBtnTeamInv->SetEnable(true);
		m_pBtnTeamInv->SetRefresh(true);
	}

	if(m_bStartCountTime)
	{
		m_dwDisableTime += Kernel::Inst()->GetDeltaTimeDW();
		if (m_dwDisableTime > 1000)
		{
			m_bStartCountTime = false;
			m_pBtnRefresh->SetEnable(TRUE);
		}
	}
	if(m_bDestroy)
		m_pFrameMgr->AddToDestroyList(this);
}

void EasyTeamFrame::ShowNearbyPlayerInfo()
{
	m_pPlayerList->Clear();
	INT nTotalRow = 0;
	DWORD dwColor = GT_INVALID;
	for( size_t i = 0; i < m_vecNearbyPlayer.size(); i++ )
	{
		Player* pPlayer = (Player*)RoleMgr::Inst()->FindRole(m_vecNearbyPlayer[i]);
		if(!P_VALID(pPlayer))
			continue;
		m_pPlayerList->SetText( nTotalRow, 0, pPlayer->GetRoleName().c_str(), dwColor );
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), pPlayer->GetRoleLevel() );
		m_pPlayerList->SetText( nTotalRow, 1, szTmp, dwColor );
		nTotalRow++;
	}

	tagPoint ptSizeOldListBox = m_pPlayerList->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pPlayerList->GetRowHeight() * nTotalRow;
	m_pPlayerList->SetResizable( TRUE );
	m_pPlayerList->Resize( ptSizeListBox );
	m_pPlayerList->SetResizable( FALSE );
	m_pPlayerList->SetRefresh(TRUE);
	m_pPlayerList->SetCurSelectedRow(GT_INVALID);
}

DWORD EasyTeamFrame::OnNetGetSinglePlayers(tagNS_GetSinglePlayers *pNetCmd, DWORD)
{
	if(!P_VALID(m_pWnd))
		return 0;

	m_vecNearbyPlayer.clear();
	for( int i = 0; i < pNetCmd->wNums; i++ )
		m_vecNearbyPlayer.push_back(pNetCmd->dwRoleID[i]);
	ShowNearbyPlayerInfo();
	return 0;
}
