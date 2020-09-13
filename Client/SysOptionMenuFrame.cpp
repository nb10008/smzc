#include "stdafx.h"
#include "SysOptionMenuFrame.h"
#include "SeleRoleMgr.h"
#include "WorldFrame.h"
#include "MapMgr.h"
#include "CombatSysUtil.h"
#include "QuitFrame.h"
#include "RoleMgr.h"
#include "Player.h"
#include "LocalPlayer.h"
#include "DragBtn.h"
#include "SaveAccount.h"

SysOptionMenuFrame::SysOptionMenuFrame() :
m_Trunk( this ),
m_pWnd( NULL ),
m_pPanel( NULL ),
m_pBn_ResumeGame( NULL ),
m_pBn_GameSettings( NULL ),
m_pBn_InputSettings( NULL ),
m_pBn_ResumeRoleSelect( NULL ),
m_pBn_ResumeLogin( NULL ),
m_pBn_QuitGame( NULL ),
m_pWndKoreaGameLevel(NULL)
{
   m_nPolicy = GT_INVALID;
   m_pFrame = NULL;
}

SysOptionMenuFrame::~SysOptionMenuFrame()
{
}


//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------
BOOL SysOptionMenuFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;
    m_pFrame = (ShortcutBarFrame*)m_pFrameMgr->GetFrame(_T("ShortcutBar"));
	LoadLoginMode();
	m_pFrameMgr->RegisterEventHandle(_T("tagFixFPSsuggestionEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SysOptionMenuFrame::OnFixFPS));
	return TRUE;
}

//-----------------------------------------------------------------------------
// Destroy
//-----------------------------------------------------------------------------
BOOL SysOptionMenuFrame::Destroy()
{
	GameFrame::Destroy();

	m_pFrameMgr->UnRegisterEventHandler(_T("tagFixFPSsuggestionEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SysOptionMenuFrame::OnFixFPS));

	SafeDestroyUI();

	DragBtn* pDrag = (DragBtn*)TObjRef<GUISystem>()->GetWnd(_T("\\desktop\\DragButton"));
	if(P_VALID(pDrag))
	{
		if (!pDrag->IsInvisible())
		{
			TObjRef<GUISystem>()->SetActive(pDrag);
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// ReloadUI
//-----------------------------------------------------------------------------
BOOL SysOptionMenuFrame::ReloadUI()
{
	SafeDestroyUI();
	LoadLoginMode();
	// 创建
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\systemmenu.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	if ( m_nPolicy == 5 )
	{
		m_pGUI->ChangeXml(&ele, "systemmenu\\pic\\restartbutt", "Invisible", "yes");
		std::stringstream streamX, streamY;
		streamX << 45;
		streamY << 152;
		m_pGUI->ChangeXml(&ele, "systemmenu\\pic\\leavebutt", "LogicPos_x", streamX.str().c_str());
		m_pGUI->ChangeXml(&ele, "systemmenu\\pic\\leavebutt", "LogicPos_y", streamY.str().c_str());
		m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	} 
	else
	{
		m_pWnd = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	}

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&SysOptionMenuFrame::GUIEventHandler));

	m_pPanel = m_pWnd->GetChild( _T("pic") );
	m_pBn_ResumeGame =		  (GUIButton*)m_pPanel->GetChild( _T("resumebutt") );
	m_pBn_GameSettings =      (GUIButton*)m_pPanel->GetChild( _T("setbutton") );
	m_pBn_InputSettings =     (GUIButton*)m_pPanel->GetChild( _T("hotkeybutt") );
	m_pBn_ResumeRoleSelect =  (GUIButton*)m_pPanel->GetChild( _T("backbutt") );
	m_pBn_ResumeLogin =       (GUIButton*)m_pPanel->GetChild( _T("restartbutt") );
	m_pBn_QuitGame =          (GUIButton*)m_pPanel->GetChild( _T("leavebutt") );
	m_pBn_KoreaGameLevel =    (GUIButton*)m_pPanel->GetChild( _T("introbutton"));
	if ( m_nPolicy == 5 || m_nPolicy == 7 || m_nPolicy == 9 )
	{
		m_pBn_ResumeLogin->SetEnable(FALSE);
		m_pBn_ResumeLogin->SetInvisible(TRUE);
	}

	//m_pBn_ResumeRoleSelect->SetEnable( false );
	m_pWnd->SetInvisible( FALSE );
	m_pWnd->SetTopMost( TRUE );
	m_pGUI->SetActive( m_pWnd );

	//韩国游戏等级控件
	if(SaveAccount::Inst()->GetLoginMode() == 11)
	{
		XmlElement ele1;
		tstring strPath1 = g_strLocalPath + _T("\\ui\\Koreanbanpic.xml");
		m_pGUI->LoadXml(&ele1, "VFS_System", strPath1.c_str());
		m_pWndKoreaGameLevel = m_pGUI->CreateWnd(_T("\\desktop"), &ele1);
		m_pWndKoreaGameLevel->SetInvisible(TRUE);
	}

	return TRUE;
}


BOOL SysOptionMenuFrame::EscCancel()
{
	m_pMgr->AddToDestroyList( this );
	return TRUE;
}

VOID SysOptionMenuFrame::SafeDestroyUI()
{
	if( P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd( m_pWnd );
		m_pWnd = NULL;
	}
	if( P_VALID(m_pWndKoreaGameLevel) )
	{
		m_pGUI->DestroyWnd(m_pWndKoreaGameLevel);
		m_pWndKoreaGameLevel = NULL;
	}
}

//-----------------------------------------------------------------------------
// 读取登录验证策略
//-----------------------------------------------------------------------------
VOID SysOptionMenuFrame::LoadLoginMode()
{
	TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
	TCHAR szTemp[X_LONG_NAME*2];

	_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
	tstring strAppPath = szTemp;

	tstring strIniFileFullName = strAppPath + szIniFileName;
	::GetPrivateProfileString(_T("Launcher"), _T("LoginMode"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
	tstring policy = szTemp;
	if( policy == _T("") )
	{
		MessageBox(NULL, _T("Don't find LoginMode !"), _T(""), MB_OK);
		return;
	}
	m_nPolicy = _ttoi(policy.c_str());
}

//-----------------------------------------------------------------------------
//! 系统选单事件处理
//! 
//-----------------------------------------------------------------------------
BOOL SysOptionMenuFrame::GUIEventHandler(tagGUIEvent* pEvent)
{
	const GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBn_ResumeGame )                        // 返回游戏
		{
			m_pMgr->AddToDestroyList( this );
		}
		else if( pWnd == m_pBn_GameSettings )                 // 打开游戏设置界面
		{
			OnOpenGameSettings();
		}
		else if( pWnd == m_pBn_InputSettings )                // 打开自定义键位界面
		{
			OnOpenInputSettings();
		}
		else if( pWnd == m_pBn_ResumeRoleSelect )             // 返回角色选择
		{
			OnLogOut( QuitFrame::ELOT_ResumeRoleSelect );
		}
		else if( pWnd == m_pBn_ResumeLogin )                  // 返回帐号登陆
		{
			OnLogOut( QuitFrame::ELOT_ResumLogin );
		}
		else if( pWnd == m_pBn_QuitGame )                     // 离开游戏
		{
			OnSaveData();                                     // 快捷栏数据保存至服务器
			OnQuitGame();
			//if( RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel() < 10 )		//玩家小于10级不弹出提示框
			//{
			//	OnQuitGame();
			//}
			//else
			//{
			//	GameFrame* pExitPromptFrame = m_pMgr->GetFrame( _T("ExitPromptFrame") );
			//	if( !P_VALID( pExitPromptFrame ) )
			//		m_pMgr->CreateFrame( _T("Root"), _T("ExitPromptFrame"), _T("ExitPromptFrame"), 0 );
			//	m_pMgr->AddToDestroyList( this );
			//}
		}
		else if( pWnd == m_pBn_KoreaGameLevel )
		{
			if(P_VALID(m_pWndKoreaGameLevel))
				m_pWndKoreaGameLevel->SetInvisible(!m_pWndKoreaGameLevel->IsInvisible());
		}
		break;
	default:
		break;
	}

	return FALSE;
}

VOID SysOptionMenuFrame::OnOpenGameSettings()
{
	GameFrame* pFrame = m_pMgr->GetFrame( _T("GameSettings") );
	if( !P_VALID( pFrame ) )
		m_pMgr->CreateFrame( _T("World"), _T("GameSettings"), _T("GameSettingsFrame"), 0);

	m_pMgr->AddToDestroyList( this );
}

VOID SysOptionMenuFrame::OnOpenInputSettings()
{
	GameFrame* pFrame = m_pMgr->GetFrame( _T("GameInput") );
	if( !P_VALID( pFrame ) )
		m_pMgr->CreateFrame(_T("Root"), _T("GameInput"), _T("GameInputFrame"), 0);

	m_pMgr->AddToDestroyList( this );
}

VOID SysOptionMenuFrame::OnLogOut( const int nLogType )
{
	if( RoleMgr::Inst()->GetLocalPlayer()->GetRoleState() & ERS_Combat )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("Logout_Err_InCombat")] );
	}
	else
	{
		OnSaveData();
		GameFrame* pQuitFrame = m_pMgr->GetFrame( _T("Logout") );
		if( !P_VALID( pQuitFrame ) )
			m_pMgr->CreateFrame( _T("Root"), _T("Logout"), _T("QuitFrame"), nLogType );
	}
	m_pMgr->AddToDestroyList( this );
}

void SysOptionMenuFrame::OnQuitGame()
{
	GameFrame* pQuitFrame = m_pMgr->GetFrame( _T("QuitGame") );
	if( !P_VALID( pQuitFrame ) )
		m_pMgr->CreateFrame( _T("Root"), _T("QuitGame"), _T("QuitGameFrame"),0 );

	m_pMgr->AddToDestroyList( this );
}

void SysOptionMenuFrame::OnSaveData()
{
    if ( P_VALID( m_pFrame ))
	{
		m_pFrame->SetIsChange(TRUE);
		m_pFrame->SaveQuickBarToDB();
		
	}
}

DWORD SysOptionMenuFrame::OnFixFPS(tagMsgBoxEvent* pEvent)
{
	if (MBF_OK == pEvent->eResult)
	{
		OnOpenGameSettings();

		tagGameEvent event(_T("tagFixGameSetting"),NULL);
		m_pFrameMgr->SendEvent(&event);
	}
	return 0;
}

VOID SysOptionMenuFrame::HideWindow()
{
	m_pWnd->SetInvisible(true);
}
