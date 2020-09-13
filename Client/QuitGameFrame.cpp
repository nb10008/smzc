#include "StdAfx.h"
#include "QuitGameFrame.h"
#include "WorldFrame.h"
#include "HackShield.h"
#include "..\WorldDefine\msg_return_roleselect.h"
#include "RoleMgr.h"
#include "SeleRoleMgr.h"
#include "CombatSysUtil.h"
#include "LoadingFrame.h"
#include "StaticEx.h"
#include "SDOAIgw.h"

#define QUIT_GAME_TIME (10*1000)	// 10秒离开游戏
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
QuitGameFrame::QuitGameFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndText  = NULL;				//倒计时
	m_pBtnGodCat  = NULL;			//打开精灵猫
	m_pBtnQuit  = NULL;				//退出游戏
	m_pBtnCancel  = NULL;			//取消
	m_pStcBackImg  = NULL;			//背景图片
	m_pBtnOne  = NULL;				//adv 1
	m_pBtnTwo  = NULL;				//adv 2
	m_pBtnThree  = NULL;			//adv 3
	m_pBtnClose = NULL;

	m_dwStartTime = GT_INVALID;
	m_bSendMsg = false;

	m_nBkImgIndex = 0;
	m_dwSwitchTime = GT_INVALID;
	
	m_vecImgs.resize( 3 );
	m_vecImgs[0] = g_StrTable[_T("QuitGame_AdvImg1")];
	m_vecImgs[1] = g_StrTable[_T("QuitGame_AdvImg2")];
	m_vecImgs[2] = g_StrTable[_T("QuitGame_AdvImg3")];
}


//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
QuitGameFrame::~QuitGameFrame()
{
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL QuitGameFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_dwStartTime = timeGetTime();
	m_dwSwitchTime = m_dwStartTime;
	m_pGUI->SetModal( m_pWnd );

	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL QuitGameFrame::Destroy()
{
	GameFrame::Destroy();

	if( P_VALID(m_pWnd) )
		m_pGUI->DestroyWnd(m_pWnd);

	m_pGUI->SetModal( NULL );
	return TRUE;
}



//-----------------------------------------------------------------------------
//! 事件处理
//! 
//-----------------------------------------------------------------------------
BOOL QuitGameFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnQuit )
				Logout();

			if( pWnd == m_pBtnClose )
				m_pFrameMgr->AddToDestroyList(this);

			if( pWnd == m_pBtnCancel )
				m_pFrameMgr->AddToDestroyList(this);

			if( pWnd == m_pBtnOne )
			{
				m_pStcBackImg->SetPicFile( m_vecImgs[0] );
				m_nBkImgIndex = 0;
				m_dwSwitchTime = timeGetTime();
			}

			if( pWnd == m_pBtnTwo )
			{
				m_pStcBackImg->SetPicFile( m_vecImgs[1] );
				m_nBkImgIndex = 1;
				m_dwSwitchTime = timeGetTime();
			}

			if( pWnd == m_pBtnThree )
			{
				m_pStcBackImg->SetPicFile( m_vecImgs[2] );
				m_nBkImgIndex = 2;
				m_dwSwitchTime = timeGetTime();
			}
			
			if( pWnd == m_pBtnGodCat )
			{
				//打开精灵猫
			}
		}
		break;
	case EGUIE_KeyDown:
		switch( pEvent->msg.dwParam1 )
		{
		case VK_RETURN:
			this->Logout();
			break;
		}
		break;
	}

	return FALSE;
}



//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL QuitGameFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\offline_exit.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str() );
	m_pGUI->ChangeXml( &element,"closeplay_win\\closeplay_pic\\back1\\img", "ClassName", "StaticEx" );

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&QuitGameFrame::EventHandler) );

	m_pWndText  = (GUIStatic*)m_pWnd->GetChild( _T("closeplay_pic\\back0\\word0\\num") );
	m_pBtnGodCat  = (GUIButton*)m_pWnd->GetChild( _T("closeplay_pic\\outbutt") );
	m_pBtnQuit  = (GUIButton*)m_pWnd->GetChild( _T("closeplay_pic\\leavebutt") );
	m_pBtnCancel  = (GUIButton*)m_pWnd->GetChild( _T("closeplay_pic\\shutbutt") );
	m_pStcBackImg  = (StaticEx*)m_pWnd->GetChild( _T("closeplay_pic\\back1\\img") );
	m_pBtnOne  = (GUIButton*)m_pWnd->GetChild( _T("closeplay_pic\\back1\\img\\new\\num3") );
	m_pBtnTwo  = (GUIButton*)m_pWnd->GetChild( _T("closeplay_pic\\back1\\img\\new\\num2") );
	m_pBtnThree  = (GUIButton*)m_pWnd->GetChild( _T("closeplay_pic\\back1\\img\\new\\num1") );
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild( _T("closeplay_pic\\closebutt") );

	m_pBtnGodCat->SetInvisible( TRUE );
	m_pStcBackImg->SetPicFile( m_vecImgs[0] );
	m_pGUI->SetActive( m_pWnd );
	m_pWnd->FlipToTop();

	return TRUE;
}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID QuitGameFrame::Update()
{
	GameFrame::Update();

	DWORD dwRunTime = timeGetTime() - m_dwStartTime;
	if( dwRunTime>QUIT_GAME_TIME && !m_bSendMsg )
	{
		tagGUIEvent event( m_pBtnQuit, EGUIE_Click );
		m_bSendMsg = TRUE;
		m_pBtnQuit->SendEvent( &event );

		return;
	}

	if(!m_bSendMsg)
	{
		TCHAR szBuff[128];
		_stprintf( szBuff, _T("%d"), (QUIT_GAME_TIME - dwRunTime)/1000 + 1 );
		m_pWndText->SetText(szBuff);
		m_pWndText->SetRefresh( TRUE );
	}

	DWORD dwTime = timeGetTime() - m_dwSwitchTime;
	if( dwTime > 3500 )
	{
		m_nBkImgIndex++;
		m_nBkImgIndex %= 3;
		m_pStcBackImg->SetPicFile( m_vecImgs[m_nBkImgIndex] );
		m_dwSwitchTime = timeGetTime();
	}

	if( !m_pSession->IsConnect() )
		m_pFrameMgr->AddToDestroyList( this );
}

BOOL QuitGameFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList( this );
	return TRUE;
}

//-----------------------------------------------------------------------------
// Logout
//-----------------------------------------------------------------------------
VOID QuitGameFrame::Logout()
{
	if( SDOAIgw::Inst()->GetSDOAFlag() )
		SDOAIgw::Inst()->LogOut();
	TObjRef<NetSession>()->Disconnect();
	HackShield::Inst()->StopApexClient();
	PostQuitMessage( 0 );

	m_bSendMsg = true;
	m_pFrameMgr->AddToDestroyList(this);
}