#include "StdAfx.h"
#include "ItemMgr.h"
#include "ItemButton.h"
#include "ToolTipFrame.h"
#include "ToolTipCreator.h"
#include "OldPlayerComeBackFrame.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_player_preventlost.h"

OldPlayerComeBackFrame::OldPlayerComeBackFrame(void):m_Trunk(this)
{
	m_pWndWorld	= NULL;
	m_pBtnGet = NULL;
	m_pIconItem = NULL;
	m_dwItemTypeID = GT_INVALID;
}

OldPlayerComeBackFrame::~OldPlayerComeBackFrame(void)
{
}

BOOL OldPlayerComeBackFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register( "NS_PlayerRegressionOk",	(NETMSGPROC)m_Trunk.sfp2(&OldPlayerComeBackFrame::OnNetNS_PlayerRegressionOk),	_T("NS_PlayerRegressionOk"));

	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL OldPlayerComeBackFrame::Destroy()
{
	GameFrame::Destroy();
	if( P_VALID(m_pWndWorld) )
		m_pGUI->DestroyWnd(m_pWndWorld);

	m_pCmdMgr->UnRegister( "NS_PlayerRegressionOk",	(NETMSGPROC)m_Trunk.sfp2(&OldPlayerComeBackFrame::OnNetNS_PlayerRegressionOk));

	return TRUE;
}

DWORD OldPlayerComeBackFrame::OnNetNS_PlayerRegressionOk( tagNS_PlayerRegressionOk *pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode == E_Success)
		m_pFrameMgr->AddToDestroyList( this );
	else if(pNetCmd->dwErrorCode == E_Bag_NotEnoughSpace)
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr40")]);
	return 0;
}

BOOL OldPlayerComeBackFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnGet )
			{
				tagNC_PlayerRegressionOk cmd;
				m_pSession->Send( &cmd );
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if(pWnd == m_pIconItem )
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_dwItemTypeID, GT_INVALID).c_str());
		}
		break;
	default:
		break;
	}

	return FALSE;
}


BOOL OldPlayerComeBackFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\oldplayer.xml");
	
	m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() );
	m_pGUI->ChangeXml( &element, "oldplayer_win\\oldplayer_pic\\iconback\\img", "ClassName", "ItemButton" );
	
	m_pWndWorld = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	m_pGUI->RegisterEventHandler( m_pWndWorld->GetFullName().c_str(), m_Trunk.sfp1( &OldPlayerComeBackFrame::EventHandler ) );

	m_pBtnGet  = (GUIButton*)m_pWndWorld->GetChild( _T("oldplayer_pic\\button") );
	m_pIconItem = (ItemButton*)m_pWndWorld->GetChild( _T("oldplayer_pic\\iconback\\img") );

	m_pWndWorld->SetInvisible( TRUE );
	return TRUE;
}

void OldPlayerComeBackFrame::Show( DWORD dwTypeID, INT nCount, BYTE byQuality )
{
	if(dwTypeID == GT_INVALID || 0 == dwTypeID )
		return;
	m_pIconItem->RefreshItem( dwTypeID, nCount, byQuality );
	m_dwItemTypeID = dwTypeID;
	m_pWndWorld->SetInvisible(FALSE);
}