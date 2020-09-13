#include "stdafx.h"
#include "ListFrame.h"
#include "QuestMgr.h"
#include "RoleMgr.h"

ListFrame::ListFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndBackPic = NULL;
	m_pList = NULL;
	m_pBtnExit = NULL;
	m_pBtnOk = NULL;
	m_dwNPCID = GT_INVALID;
}

ListFrame::~ListFrame()
{

}

BOOL ListFrame::Destroy()
{
	m_pNetCmdMgr->UnRegister( "NS_QueryDragonDefenderRankings", (NETMSGPROC)(m_Trunk.sfp2(&ListFrame::OnNS_QueryDragonDefenderRankings)) );
	m_pNetCmdMgr->UnRegister( "NS_QueryDragonGirlRankings", (NETMSGPROC)(m_Trunk.sfp2(&ListFrame::OnNS_QueryDragonGirlRankings)) );

	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);
	return GameFrame::Destroy();
}

BOOL ListFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;
	m_pNetCmdMgr->Register("NS_QueryDragonDefenderRankings", (NETMSGPROC)(m_Trunk.sfp2(&ListFrame::OnNS_QueryDragonDefenderRankings)), _T("NS_QueryDragonDefenderRankings"));
	m_pNetCmdMgr->Register("NS_QueryDragonGirlRankings", (NETMSGPROC)(m_Trunk.sfp2(&ListFrame::OnNS_QueryDragonGirlRankings)), _T("NS_QueryDragonGirlRankings"));
	return TRUE;
}

BOOL ListFrame::ReloadUI()
{
	tstring strPath = g_strLocalPath + _T("\\ui\\common_list.xml");
	m_pWnd = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	m_pWndBackPic = (GUIWnd*)m_pWnd->GetChild(_T("com_list_pic"));
	m_pBtnOk = (GUIButton*)m_pWndBackPic->GetChild(_T("surebutt"));
	m_pBtnExit = (GUIButton*)m_pWndBackPic->GetChild(_T("closebutt"));
	m_pList = (GUIListBox*)m_pWndBackPic->GetChild(_T("listback\\list"));
	m_pList->SetColNum( 3, 50 );
	m_pList->SetColWidth( 0, 60 );	// 名次
	m_pList->SetColWidth( 1, 158 );	// 名字
	m_pList->SetColWidth( 2, 60 );	// 票数
	m_dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
	ShowFrame(false);
	m_pWnd->FlipToTop();
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&ListFrame::EventHandler) );
	return true;
}

VOID ListFrame::Update()
{
	if (RoleMgr::Inst()->IsOutValidDist(m_dwNPCID))
	{
		ShowFrame(false);
	}
}

BOOL ListFrame::EscCancel()
{
	if( m_pWnd->IsInvisible() )
		return false;
	ShowFrame(false);
	return true;
}

DWORD ListFrame::EventHandler( tagGUIEvent *pEvent )
{
	GUIWnd *pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return false;
	switch(pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnExit || pWnd == m_pBtnOk )
				ShowFrame(false);
		}
	}
	return false;
}

void ListFrame::ShowFrame( bool bShow )
{
	if( P_VALID(m_pWnd) )
	{
		m_pWnd->SetInvisible( !bShow );
		if( bShow )	
			m_pWnd->FlipToTop();
	}
}

DWORD ListFrame::OnNS_QueryDragonDefenderRankings( tagNS_QueryDragonDefenderRankings *pNetCmd, DWORD )
{

	
	if(!P_VALID(m_pWnd))
		return 0;
	m_dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
	m_pList->Clear();
	for( int i = 0; i < pNetCmd->n16Count; i++ )
	{
		tstringstream num;
		num << i + 1;
		m_pList->SetText( i, 0, num.str().c_str() );	// 名次
		tstring strName = pNetCmd->data[i].tcRoleName;
		m_pList->SetText( i, 1, strName.c_str() );	// 名字
		tstringstream piao;
		piao << pNetCmd->data[i].dwPoint;
		m_pList->SetText( i, 2, piao.str().c_str() );	// 票数
	}
	tagPoint ptSizeOldListBox = m_pList->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pList->GetRowHeight() * pNetCmd->n16Count;
	m_pList->SetResizable( TRUE );
	m_pList->Resize( ptSizeListBox );
	m_pList->SetResizable( FALSE );
	m_pList->SetRefresh(TRUE);
	m_pList->SetCurSelectedRow(GT_INVALID);
	ShowFrame(true);
	return 0;
}

DWORD ListFrame::OnNS_QueryDragonGirlRankings( tagNS_QueryDragonGirlRankings *pNetCmd, DWORD )
{
	if(!P_VALID(m_pWnd))
		return 0;
    m_dwNPCID = QuestMgr::Inst()->GetTalkNPCGlobalID();
	m_pList->Clear();
	DWORD dwTotalRow = 0;
	for( int i = 0; i < pNetCmd->n16Count; i++ )
	{
		tstringstream num;
		num << dwTotalRow + 1;
		m_pList->SetText( dwTotalRow, 0, num.str().c_str() );	// 名次
		tstring strName = pNetCmd->data[i].tcRoleName;
		m_pList->SetText( dwTotalRow, 1, strName.c_str() );	// 名字
		tstringstream piao;
		piao << pNetCmd->data[i].dwPoint;
		m_pList->SetText( dwTotalRow, 2, piao.str().c_str() );	// 票数
		dwTotalRow++;
	}
	tagPoint ptSizeOldListBox = m_pList->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pList->GetRowHeight() * dwTotalRow;
	m_pList->SetResizable( TRUE );
	m_pList->Resize( ptSizeListBox );
	m_pList->SetResizable( FALSE );
	m_pList->SetRefresh(TRUE);
	m_pList->SetCurSelectedRow(GT_INVALID);
	ShowFrame(true);
	return 0;
}