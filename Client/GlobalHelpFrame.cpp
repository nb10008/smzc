#include "StdAfx.h"
#include "GlobalHelpFrame.h"
#include "QuestMgr.h"
#include "HelpFrame.h"
#include "ExternalLinksFrame.h"

GlobalHelpFrame::GlobalHelpFrame(void):
m_Trunk( this ),
m_pWnd(	NULL ),
m_pBtnNewOne( NULL ),
m_pBtnHelpMenu( NULL ),
m_pBtnOrg( NULL ),
m_pBtnCancel( NULL ),
m_pBtnLoongBao( NULL ),
m_pBtnIdle( NULL ),
m_pBtnClose( NULL )
{

}

GlobalHelpFrame::~GlobalHelpFrame(void)
{

}


BOOL GlobalHelpFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{
	if ( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1( &GlobalHelpFrame::EventHandler ) );

	//窗口水平方向拉到 屏幕的中间(其实是 10 + nWidth)
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	m_pWnd->Move( tagPoint(nWidth/2, 0) );

	return TRUE;
}

BOOL GlobalHelpFrame::Destroy()
{
	GameFrame::Destroy();

	m_pGUI->UnRegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1( &GlobalHelpFrame::EventHandler ) );

	if ( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList( m_pWnd );
		m_pWnd = NULL;
	}

	return TRUE;
}

BOOL GlobalHelpFrame::ReloadUI()
{
	if ( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList( m_pWnd );

	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\onlinetips.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	m_pBtnNewOne = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt1") );
	if( P_VALID(m_pBtnNewOne) )
		m_pBtnNewOne->SetText( g_StrTable[_T("GlobalHelp_NewOne")] );

	m_pBtnHelpMenu = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt2") );
	if( P_VALID(m_pBtnHelpMenu) )
		m_pBtnHelpMenu->SetText( g_StrTable[_T("GlobalHelp_HelpMenu")] );

	m_pBtnOrg = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt3") );
	if( P_VALID(m_pBtnOrg) )
		m_pBtnOrg->SetText( g_StrTable[_T("GlobalHelp_OrgSite")] );

	m_pBtnLoongBao = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt4") );
	if( P_VALID(m_pBtnLoongBao) )
		m_pBtnLoongBao->SetText( g_StrTable[_T("GlobalHelp_LoongBao")] );

	m_pBtnCancel = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt40") );
	if( P_VALID(m_pBtnCancel) )
		m_pBtnCancel->SetText( g_StrTable[_T("GlobalHelp_Cancel")] );

	m_pBtnIdle = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt400") );
	if( P_VALID(m_pBtnIdle) )
		m_pBtnIdle->SetInvisible( TRUE );

	m_pBtnClose = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\closebutt") );
	
	m_pBtnLoongBao->SetEnable( FALSE );
	m_pWnd->SetInvisible( TRUE );

	return TRUE;
}

DWORD GlobalHelpFrame::EventHandler(tagGUIEvent *pEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	if( !P_VALID(pWnd) )
		return TRUE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnClose )
			{
				m_pGameFrameMgr->AddToDestroyList( this );
			}
			else if( pWnd == m_pBtnCancel )
			{
				m_pGameFrameMgr->AddToDestroyList( this );
			}
			else if( pWnd == m_pBtnNewOne )
			{
				if( !QuestMgr::Inst()->IsQuestHelpFrameOpened() )
					QuestMgr::Inst()->OpenQuestHelpFrame();

				m_pGameFrameMgr->AddToDestroyList( this );
			}
			else if( pWnd == m_pBtnHelpMenu )
			{
				HelpFrame *pFrame = (HelpFrame*)(m_pMgr->GetFrame( _T("HelpFrame")));
				if( P_VALID(pFrame) )
				{
					if( !pFrame->IsVisible() )
						m_pGameFrameMgr->SendEvent( &tagGameEvent( _T("Open_NewHelp"), this ) );
				}

				m_pGameFrameMgr->AddToDestroyList( this );
			}
			else if( pWnd == m_pBtnOrg )
			{
				ExternalLinksFrame *pFrame = (ExternalLinksFrame*)(m_pMgr->GetFrame(  _T("ExternalLinks") ) );
				if( P_VALID( pFrame ) )
				{
					pFrame->ShowWnd( TRUE );
				}
				m_pGameFrameMgr->AddToDestroyList( this );
			}
			else if( pWnd == m_pBtnLoongBao )
			{
				m_pGameFrameMgr->AddToDestroyList( this );
			}
		}
		break;
	}

	return FALSE;
}

void GlobalHelpFrame::ShowWnd( BOOL bShow )
{
	if( P_VALID(m_pWnd) )
	{
		m_pWnd->SetInvisible( !bShow );
		m_pWnd->SetRefresh( TRUE );
	}
}

BOOL GlobalHelpFrame::EscCancel()
{
	if( m_pWnd->IsInvisible() )
		return FALSE;

	m_pGameFrameMgr->AddToDestroyList( this );

	return TRUE;
}

