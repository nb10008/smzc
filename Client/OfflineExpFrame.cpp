#include "StdAfx.h"
#include "OfflineExpFrame.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_player_preventlost.h"

OfflineExpFrame::OfflineExpFrame(void): m_Trunk(this)
{
	m_pWnd			= NULL;
	m_pBtnGet		= NULL;
	m_pBtnReturn	= NULL;
	//m_pBtnClose	= NULL;
	m_pStcTitle		= NULL;
	m_pStcContent	= NULL;

	m_dwOffHour		= 0;
	m_dwOffMinute	= 0;
	m_dwTotalExp	= 0;
	m_dwFreeExp		= 0;
	m_dwYuanBao		= 0;
	m_bWaitServerReturn = false;
	m_eWndType = EHET_Init;
}

OfflineExpFrame::~OfflineExpFrame(void)
{
}

BOOL OfflineExpFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register("NS_GetOfflineExp", (NETMSGPROC)m_Trunk.sfp2(&OfflineExpFrame::OnNS_GetOfflineExp), _T("NS_GetOfflineExp") );

	return TRUE;
}


BOOL OfflineExpFrame::Destroy()
{
	GameFrame::Destroy();
	
	if( P_VALID(m_pWnd) )
		m_pGUI->DestroyWnd(m_pWnd);

	m_pCmdMgr->UnRegister("NS_GetOfflineExp", (NETMSGPROC)m_Trunk.sfp2(&OfflineExpFrame::OnNS_GetOfflineExp) );

	return TRUE;
}


BOOL OfflineExpFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\offline_experience.xml");
	m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() );

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&OfflineExpFrame::EventHandler) );

	m_pStcTitle  = (GUIStatic*)m_pWnd->GetChild( _T("experience_pic\\title") );
	m_pStcContent  = (GUIStaticEx*)m_pWnd->GetChild( _T("experience_pic\\back0\\word0") );
	m_pBtnGet  = (GUIButton*)m_pWnd->GetChild( _T("experience_pic\\experiencebutt1") );
	m_pBtnReturn  = (GUIButton*)m_pWnd->GetChild( _T("experience_pic\\experiencebutt2") );
	//m_pBtnClose = (GUIButton*)m_pWnd->GetChild( _T("experience_pic\\closebutt") );
	
	m_pWnd->SetInvisible( TRUE );
	return TRUE;
}

BOOL OfflineExpFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnReturn )
			{
				if( m_eWndType == EHET_Init )
					m_eWndType = EHET_All;
				else
					m_eWndType = EHET_Init;

				RefreshContent();
			}
			else if( pWnd == m_pBtnGet )
			{
				if( m_eWndType == EHET_Init )
				{
					m_eWndType = EHET_Free;
					RefreshContent();
				}
				else
				{
					SendHarvestExpMessage();
				}
			}
		}
		break;
	}

	return FALSE;
}

void OfflineExpFrame::RefreshContent()
{
	switch( m_eWndType )
	{
	case EHET_Init:
		{
			m_pStcTitle->SetText( g_StrTable[_T("OfflineExp_TitleInit")] );
			m_pStcTitle->SetRefresh( TRUE );

			TCHAR szContent[X_LONG_NAME] = {0};
			TCHAR szText1[X_LONG_NAME] = {0};
			TCHAR szText2[X_LONG_NAME] = {0};
			TCHAR szText3[X_LONG_NAME] = {0};
			TCHAR szText4[X_LONG_NAME] = {0};
			_itot(m_dwOffHour, szText1, 10);
			_itot(m_dwOffMinute, szText2, 10);
			_itot(m_dwTotalExp, szText3, 10);
			_itot(m_dwFreeExp, szText4, 10);
			_stprintf( szContent, g_StrTable[_T("OfflineExp_ContentInit")], szText1, szText2, szText3, szText4 );
			m_pStcContent->SetText( szContent );
			m_pStcContent->SetRefresh( TRUE );

			m_pBtnGet->SetText( g_StrTable[_T("OfflineExp_Free")] );
			m_pBtnGet->SetRefresh( TRUE );

			m_pBtnReturn->SetText( g_StrTable[_T("OfflineExp_All")] );
			m_pBtnReturn->SetRefresh( TRUE );
		}
		break;
	case EHET_Free:
		{
			m_pStcTitle->SetText( g_StrTable[_T("OfflineExp_Free")] );
			m_pStcTitle->SetRefresh( TRUE );

			TCHAR szContent[X_LONG_NAME] = {0};
			_stprintf( szContent, g_StrTable[_T("OfflineExp_ContentFree")], m_dwTotalExp - m_dwFreeExp );
			m_pStcContent->SetText( szContent );
			m_pStcContent->SetRefresh( TRUE );

			m_pBtnGet->SetText( g_StrTable[_T("OfflineExp_Free")] );
			m_pBtnGet->SetRefresh( TRUE );

			m_pBtnReturn->SetText( g_StrTable[_T("OfflineExp_Return")] );
			m_pBtnReturn->SetRefresh( TRUE );
		}
		break;
	case EHET_All:
		{
			m_pStcTitle->SetText( g_StrTable[_T("OfflineExp_All")] );
			m_pStcTitle->SetRefresh( TRUE );

			TCHAR szContent[X_LONG_NAME] = {0};
			TCHAR szText1[X_LONG_NAME] = {0};
			TCHAR szText2[X_LONG_NAME] = {0};
			TCHAR szText3[X_LONG_NAME] = {0};
			TCHAR szText4[X_LONG_NAME] = {0};
			_itot(m_dwFreeExp, szText1, 10);
			_itot(m_dwTotalExp - m_dwFreeExp, szText2, 10);
			_itot(m_dwYuanBao/10000, szText3, 10);
			_itot(m_dwYuanBao%10000, szText4, 10);
			_stprintf( szContent, g_StrTable[_T("OfflineExp_ContentAll")], szText1, szText2, szText3, szText4 );	//离线改成消耗金银
			m_pStcContent->SetText( szContent );
			m_pStcContent->SetRefresh( TRUE );

			m_pBtnGet->SetText( g_StrTable[_T("OfflineExp_All")] );
			m_pBtnGet->SetRefresh( TRUE );

			m_pBtnReturn->SetText( g_StrTable[_T("OfflineExp_Return")] );
			m_pBtnReturn->SetRefresh( TRUE );
		}
		break;
	default:
		break;
	}
}

void OfflineExpFrame::SendHarvestExpMessage()
{
	if( m_bWaitServerReturn )
		return;

	tagNC_GetOfflineExp cmd;
	if( m_eWndType == EHET_Free )
	{
		cmd.byType = 0;
	}
	else if( m_eWndType == EHET_All )
	{
		cmd.byType = 1;
	}
	else
	{
		return;
	}

	m_pSession->Send( &cmd );
	m_bWaitServerReturn  = true;
}

DWORD OfflineExpFrame::OnNS_GetOfflineExp( tagNS_GetOfflineExp* pMsg, DWORD dwParam )
{
	m_bWaitServerReturn = false;
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			TCHAR szDesc[X_LONG_NAME] = {0};
			_stprintf(szDesc, g_StrTable[_T("OfflineExp_GetSuccess")], pMsg->dwExperience );
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szDesc );

			m_pFrameMgr->AddToDestroyList( this );
		}
		break;
	case EOFFE_NotEnoughYuanBao:
		{
			//m_eWndType = EHET_ErrorTips;
			//RefreshContent();
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("OfflineExp_NotEnoughYuanbao")] );

		}
		break;
	default:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("OfflineExp_GetFailed")] );
			m_pFrameMgr->AddToDestroyList(this);  //关闭窗口
		}
		break;
	}

	return 0;
}