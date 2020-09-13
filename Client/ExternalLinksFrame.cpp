#include "StdAfx.h"
#include "ExternalLinksFrame.h"
#include "../WorldDefine/msg_external_links.h"


ExternalLinksFrame::ExternalLinksFrame(void) : 
m_Trunk(this),
m_pWnd(NULL),
m_pBtnClose(NULL),
m_nLinkNum( 0 )
{
	memset( m_links,0, sizeof(m_links) );
}

ExternalLinksFrame::~ExternalLinksFrame(void)
{
}

BOOL ExternalLinksFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{
	if ( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1( &ExternalLinksFrame::EventHandler ) );
	m_pNetCmdMgr->Register( "NS_GetExternalLinks", (NETMSGPROC)m_Trunk.sfp2(&ExternalLinksFrame::OnNS_GetExternalLinks), _T("NS_GetExternalLinks") );

	//默认初始化, 第一个当然是咱们龙的首页了
	_tcscpy_s(m_links[0].linkName, sizeof(m_links[0].linkName)/sizeof(TCHAR), _T("DACN") );
	_tcscpy_s(m_links[0].linkValue, sizeof(m_links[0].linkValue)/sizeof(TCHAR), _T("http://sm.kongzhong.com") );
	m_linkBtns[0]->SetText( m_links[0].linkName );
	m_linkBtns[0]->SetRefresh( TRUE );

	//其余的默认都是无效
	for( int i=1; i< EXTERNAL_LINKS_NUM; i++ )
	{
		m_linkBtns[i]->SetText( _T("") );
		m_linkBtns[i]->SetEnable( FALSE );
		m_linkBtns[i]->SetRefresh( TRUE );
	}
	
	//窗口水平方向拉到 屏幕的中间(其实是 10 + nWidth)
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	m_pWnd->Move( tagPoint(nWidth/2, 0) );
	
	return TRUE;
}

BOOL ExternalLinksFrame::Destroy()
{
	GameFrame::Destroy();
	
	m_pGUI->UnRegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&ExternalLinksFrame::EventHandler) );
	m_pNetCmdMgr->UnRegister( "NS_GetExternalLinks", (NETMSGPROC)m_Trunk.sfp2(&ExternalLinksFrame::OnNS_GetExternalLinks) );

	if ( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	return TRUE;
}

BOOL ExternalLinksFrame::ReloadUI()
{
	if ( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList( m_pWnd );

	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\onlinetips.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	m_linkBtns[0] = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt1") );
	m_linkBtns[1] = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt2") );
	m_linkBtns[2] = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt3") );
	m_linkBtns[3] = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt4") );
	m_linkBtns[4] = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt40") );
	m_linkBtns[5] = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\butt400") );
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild( _T("onlinetips_backpic\\closebutt") );

	m_pWnd->SetInvisible( TRUE );
	m_linkBtns[0]->SetRefresh( TRUE );
	return TRUE;
}

DWORD ExternalLinksFrame::EventHandler(tagGUIEvent *pEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	
	if( !P_VALID(pWnd) )
		return TRUE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnClose )
			{
				ShowWnd( FALSE );
				return FALSE;
			}

			for( int i=0; i < EXTERNAL_LINKS_NUM; i++ )
			{
				if( pWnd == m_linkBtns[i] )
				{
					if( _tcslen( m_links[i].linkValue ) != 0 )
						ShellExecute(NULL, _T("open"), m_links[i].linkValue, NULL, NULL, SW_MAXIMIZE);
				}
			}
		}
		break;
	}

	return FALSE;
}

void ExternalLinksFrame::ShowWnd( BOOL bShow )
{
	if( P_VALID(m_pWnd) )
	{
		m_pWnd->SetInvisible( !bShow );
		m_pWnd->SetRefresh( TRUE );
	}
}

BOOL ExternalLinksFrame::EscCancel()
{
	if( m_pWnd->IsInvisible() )
		return FALSE;

	ShowWnd(FALSE);

	return TRUE;
}


DWORD ExternalLinksFrame::OnNS_GetExternalLinks( tagNS_GetExternalLinks* pNetCmd, DWORD dwParams )
{
	m_nLinkNum = pNetCmd->dwElementNum;
	if( m_nLinkNum > EXTERNAL_LINKS_NUM )			//最多有6个链接，因为界面最多显示下6个
		m_nLinkNum = EXTERNAL_LINKS_NUM;

	for( DWORD i=0; i< m_nLinkNum; i++ )
	{
		memset( &m_links[i], 0, sizeof(tagExternalLink) );
		memcpy_s( &m_links[i].linkName, sizeof(m_links[i].linkName), (void*)(pNetCmd->links[i].linkName), sizeof(m_links[i].linkName)-1 );
		memcpy_s( &m_links[i].linkValue, sizeof(m_links[i].linkValue), (void*)(pNetCmd->links[i].linkValue), sizeof(m_links[i].linkValue)-1 );
	}

	//接收到数据后更新界面上的内容
	//m_nLinkNum 有效链接数据的个数, 可能是小于EXTERNAL_LINKS_NUM
	DWORD i =0;
	for( ; i< m_nLinkNum; i++ )
	{
		if( _tcslen( m_links[i].linkName ) == 0 || _tcslen( m_links[i].linkValue) == 0 )
			continue;

		m_linkBtns[i]->SetText( m_links[i].linkName );
		m_linkBtns[i]->SetEnable( TRUE );
		m_linkBtns[i]->SetRefresh( TRUE );
	}

	//如果链接个数不够其余的界面
	for( ; i< EXTERNAL_LINKS_NUM; i++ )
	{
		m_linkBtns[i]->SetText( _T("") );
		m_linkBtns[i]->SetEnable( FALSE );
		m_linkBtns[i]->SetRefresh( TRUE );
	}

	return 0;
}