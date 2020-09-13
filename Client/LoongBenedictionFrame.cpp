#include "StdAfx.h"
#include "LoongBenedictionFrame.h"
#include "IconStatic.h"
#include "ServerTime.h"
#include "ItemProtoData.h"
#include "CombatSysUtil.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "ItemMgr.h"
#include "Container.h"
#include "GlintStatic.h"
#include "FlashPushButton.h"
#include "../WorldDefine/msg_player_preventlost.h"

LoongBenedictionFrame::LoongBenedictionFrame(void) : 
m_Trunk( this ),
m_pWndExpand( NULL ),
m_pStcWaitTime( NULL ),
m_pBtnGetBenediction( NULL ),
m_pBtnArrow( NULL ),
m_pStcItem( NULL ),
m_pWndFold( NULL ),
//m_pStcFoldWaitTime( NULL ),
m_pBtnFoldGetBenediction( NULL ),
m_pBtnFoldArrow( NULL ),
m_pWndCurDisplay( NULL ),
m_dwNeedTime( GT_INVALID ),
m_dwStartTimer( 0 ),
m_bCanUpdate( FALSE),
m_step(EBeneStep_Null),
m_pBtnClose( NULL ),
m_bSendAtZero(FALSE),
m_pBtnFoldClose(NULL)
{
}

LoongBenedictionFrame::~LoongBenedictionFrame(void)
{
}

BOOL LoongBenedictionFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam  )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register("NS_GetLoongBenediction", (NETMSGPROC)m_Trunk.sfp2(&LoongBenedictionFrame::OnNS_GetBenediction), _T("NS_GetLoongBenediction"));
	m_pCmdMgr->Register("NS_SetNextBenediction", (NETMSGPROC)m_Trunk.sfp2(&LoongBenedictionFrame::OnNS_SetNextBenediction), _T("NS_SetNextBenediction") );

	m_pWndCurDisplay = m_pWndExpand;
	return TRUE;
}

BOOL LoongBenedictionFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	
	SafeDestroyUI();

	m_pCmdMgr->UnRegister("NS_GetLoongBenediction", (NETMSGPROC)m_Trunk.sfp2(&LoongBenedictionFrame::OnNS_GetBenediction) );
	m_pCmdMgr->UnRegister("NS_SetNextBenediction", (NETMSGPROC)m_Trunk.sfp2(&LoongBenedictionFrame::OnNS_SetNextBenediction) );

	return TRUE;
}

BOOL LoongBenedictionFrame::ReloadUI()
{
	BOOL bRet = GameFrame::ReloadUI();

	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\lucky.xml");
	m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() );
	m_pGUI->ChangeXml( &ele, "lucky_win\\luck_pic\\iconback\\img", "ClassName", "IconStatic" );
	m_pGUI->ChangeXml( &ele, "lucky_win\\luck_pic\\butt1", "ClassName", "FlashButton" );

	m_pWndExpand = m_pGUI->CreateWnd( _T("\\desktop"), &ele );
	m_pStcWaitTime = (GUIStatic*)m_pWndExpand->GetChild( _T("luck_pic\\num") );
	m_pBtnGetBenediction = (FlashButton*)m_pWndExpand->GetChild( _T("luck_pic\\butt1") );
	m_pBtnGetBenediction->SetText(g_StrTable[_T("OnLineReward")]);
	m_pBtnArrow = (GUIButton*)m_pWndExpand->GetChild( _T("luck_pic\\butt2") );
	m_pStcItem = (IconStatic*)m_pWndExpand->GetChild( _T("luck_pic\\iconback\\img") );
	m_pBtnClose = (GUIButton*)m_pWndExpand->GetChild( _T("luck_pic\\closebutt") );

	strPath = g_strLocalPath + _T("\\ui\\lucky_fold.xml");
	m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() );
	m_pGUI->ChangeXml( &ele, "lucky_fold_win\\num2", "ClassName", "GlintStatic" );
	m_pGUI->ChangeXml( &ele, "lucky_fold_win\\lucky_fold_pic\\butt1", "ClassName", "FlashButton" );

	m_pWndFold = m_pGUI->CreateWnd( _T("\\desktop"), &ele );
	//m_pStcFoldWaitTime = (GlintStatic*)m_pWndFold->GetChild( _T("num2") );
	m_pBtnFoldGetBenediction = (FlashButton*)m_pWndFold->GetChild( _T("lucky_fold_pic\\butt1") );
	m_pBtnFoldGetBenediction->SetText(g_StrTable[_T("OnLineReward")]);
	m_pBtnFoldArrow = (GUIButton*)m_pWndFold->GetChild( _T("lucky_fold_pic\\butt2") );
	m_pBtnFoldClose = (GUIButton*)m_pWndFold->GetChild( _T("lucky_fold_pic\\closebutt") );
	
	//m_pStcWaitTime->SetGlintTextColor( 0xff00ff00 );			//闪烁字颜色为	 绿色
	//m_pStcFoldWaitTime->SetGlintTextColor( 0xff00ff00 );		//闪烁字颜色为	 绿色
	//m_pBtnGetBenediction->SetGlintTextColor( 0xff00ff00 );		//闪烁字的颜色为 绿色
	//m_pBtnFoldGetBenediction->SetGlintTextColor( 0xff00ff00 );	//闪烁字的颜色为 绿色

	ShowWnd( m_pWndExpand,FALSE );
	ShowWnd( m_pWndFold,FALSE );
	EnableWnd( m_pBtnGetBenediction, FALSE );
	EnableWnd( m_pBtnFoldGetBenediction, FALSE );

	if( (m_step == EBeneStep_Null || m_step == EBeneStep_Count) && P_VALID(m_pBtnClose) && P_VALID(m_pBtnFoldClose) )
	{
		m_pBtnClose->SetInvisible( TRUE );
		m_pBtnFoldClose->SetInvisible( TRUE );
	}

	m_pGUI->RegisterEventHandler(m_pWndExpand->GetFullName().c_str(), m_Trunk.sfp1( &LoongBenedictionFrame::EventHandler) );
	m_pGUI->RegisterEventHandler(m_pWndFold->GetFullName().c_str(), m_Trunk.sfp1( &LoongBenedictionFrame::EventHandler) );

	return bRet;
}

BOOL LoongBenedictionFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd*	pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnArrow )
			{
				ShowWnd( m_pWndExpand, FALSE );
				ShowWnd( m_pWndFold, TRUE );
				m_pWndCurDisplay = m_pWndFold;
			}
			else if( pWnd == m_pBtnFoldArrow )
			{
				ShowWnd( m_pWndExpand, TRUE );
				ShowWnd( m_pWndFold, FALSE );
				m_pWndCurDisplay = m_pWndExpand;
			}
			else if( pWnd == m_pBtnClose || pWnd == m_pBtnFoldClose )
			{
				ShowWnd( m_pWndExpand, FALSE );
				ShowWnd( m_pWndFold, FALSE );
			}
			else if( pWnd == m_pBtnGetBenediction || pWnd == m_pBtnFoldGetBenediction )
			{
				GetBenediction();
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if( pWnd == m_pStcItem )
			{
				ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips( m_nItemTypeId ).c_str() );
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			if( pWnd == m_pStcItem )
			{
				ShowTip( pWnd, NULL );
			}
		}
		break;
	default:
		break;
	}
	
	return TRUE;
}


VOID LoongBenedictionFrame::Update()
{
	//零点时要和服务器同步一次，清空前一天的上线时间按累计，得到今天的倒计时时间和奖励
	tagDWORDTime dwTime = GetCurrentDWORDTime();
	if( dwTime.hour == 0 )
	{
		if( m_step == EBeneStep_Time && !m_bSendAtZero )
		{
			//tagNC_GetLoongBenediction evt;
			//m_pSession->Send( &evt );
			m_bSendAtZero = TRUE;
		}
	}
	//过了零点，恢复为原始值
	else
	{
		m_bSendAtZero  = FALSE;
	}

	if( !m_bCanUpdate )
		return;

	//剩余的秒数
	DWORD dwCurTime = timeGetTime();
	float dwElapse = ( dwCurTime - m_dwStartTimer)/1000.0f;
	m_fStepTime -= dwElapse;
	m_dwStartTimer = dwCurTime;

	
	//可以领取了
	if( m_fStepTime > 0 )
	{
		m_pBtnGetBenediction->SetText(g_StrTable[_T("OnLineReward")]);
		m_pBtnFoldGetBenediction->SetText(g_StrTable[_T("OnLineReward")]);
		EnableWnd( m_pBtnFoldGetBenediction, FALSE );
		EnableWnd( m_pBtnGetBenediction, FALSE );
	}
	else
	{
		m_pBtnGetBenediction->SetText(g_StrTable[_T("TakeReward")]);
		m_pBtnFoldGetBenediction->SetText(g_StrTable[_T("TakeReward")]);
		EnableWnd( m_pBtnFoldGetBenediction, TRUE );
		EnableWnd( m_pBtnGetBenediction, TRUE );

		BeginGlint();

		//如果当前神龙赐福的领取方式是按照每天在线时间来领取，而当前正隐藏赐福窗口，弹出
		if( m_step == EBeneStep_Time && m_pWndExpand->IsInvisible() && m_pWndFold->IsInvisible() )
		{
			m_pWndExpand->SetInvisible( FALSE );
		}
	}

	int nSecond = ((int)ceil(m_fStepTime))%60;
	int nMinute = ((int)ceil(m_fStepTime) - nSecond)/60;
	nSecond = nSecond < 0 ? 0 : nSecond;
	nMinute = nMinute < 0 ? 0 : nMinute;

	TCHAR szText[X_SHORT_NAME] = {0};
	_sntprintf( szText, X_SHORT_NAME, g_StrTable[_T("LoongBenediction_Time")], nMinute, nSecond );
	if( P_VALID(m_pStcWaitTime) )
	{
		m_pStcWaitTime->SetText( szText );
		m_pStcWaitTime->SetRefresh( TRUE );
	}
	//if( P_VALID(m_pStcFoldWaitTime) )
	//{
	//	m_pStcFoldWaitTime->SetText( szText );
	//	m_pStcFoldWaitTime->SetRefresh( TRUE );
	//}
}

VOID LoongBenedictionFrame::SafeDestroyUI()
{
	if( P_VALID( m_pWndExpand ) )
	{
		m_pGUI->DestroyWnd( m_pWndExpand );
		m_pWndExpand = NULL;
	}

	if( P_VALID( m_pWndFold ) )
	{
		m_pGUI->DestroyWnd( m_pWndFold );
		m_pWndFold = NULL;
	}
}

VOID LoongBenedictionFrame::GetBenediction()
{
	if( m_fStepTime > 0 )
	{
		if( P_VALID(GetObj("LoongBenedictionMsgBox")) )
			KillObj( "LoongBenedictionMsgBox" );

		CreateObj( "LoongBenedictionMsgBox","MsgBox" );
		TObjRef<MsgBox>("LoongBenedictionMsgBox")->Init( _T(""), g_StrTable[_T("LoongBenediction_NeedWait")],
			_T("tagMsgBoxEvent"),(MsgBoxFlag)(MBF_OK),TRUE );
		return;
	}
	
	INT nBlank;
	ItemMgr::Inst()->GetPocket()->IsFull(nBlank);
	if( nBlank <= 0 )
	{
		if( P_VALID(GetObj("LoongBenedictionMsgBox")) )
			KillObj( "LoongBenedictionMsgBox" );

		CreateObj( "LoongBenedictionMsgBox","MsgBox" );
		TObjRef<MsgBox>("LoongBenedictionMsgBox")->Init( _T(""), g_StrTable[_T("LoongBenediction_BagSpaceNotEnough")],
			_T("tagMsgBoxEvent"),(MsgBoxFlag)(MBF_OK),TRUE );
		
		return;
	}

	tagNC_GetLoongBenediction evt;
	m_pSession->Send( &evt );
}


DWORD LoongBenedictionFrame::OnNS_GetBenediction( tagNS_GetLoongBenediction* pMsg, DWORD dwParam )
{
	switch( pMsg->dwErrorCode )
	{
	case ELBD_Success:
		{
			if( GT_VALID(pMsg->nItemTypeID) )
			{
				const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto( pMsg->nItemTypeID );
				if( P_VALID(pProto) && P_VALID(pProto->pDisplayInfo) )
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("LoongBenediction_YouGetItem")], pMsg->nItemCount,pProto->pDisplayInfo->szName );
				}
			}
			//if( GT_VALID(pMsg->nExprence) )
			//{
			//	CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("LoongBenediction_YouGetExprence")], pMsg->nExprence );
			//}

			ShowWnd( m_pWndFold, FALSE );
			ShowWnd( m_pWndExpand, FALSE );
			m_bCanUpdate = FALSE;
			EndGlint();
		}
		break;
	case ELBD_Bag_NotEnoughSpace:
		{
			if( P_VALID(GetObj("LoongBenedictionMsgBox")) )
				KillObj( "LoongBenedictionMsgBox" );

			CreateObj( "LoongBenedictionMsgBox","MsgBox" );
			TObjRef<MsgBox>("LoongBenedictionMsgBox")->Init( _T(""), g_StrTable[_T("LoongBenediction_BagSpaceNotEnough")],
				_T("tagMsgBoxEvent"),(MsgBoxFlag)(MBF_OK),TRUE );
		}
		break;
	case ELBD_Time_NotArrive:
		{
			if( P_VALID(GetObj("LoongBenedictionMsgBox")) )
				KillObj( "LoongBenedictionMsgBox" );
			
			CreateObj( "LoongBenedictionMsgBox","MsgBox" );
			TObjRef<MsgBox>("LoongBenedictionMsgBox")->Init( _T(""), g_StrTable[_T("LoongBenediction_ServerTimeDontArrive")],
				_T("tagMsgBoxEvent"),(MsgBoxFlag)(MBF_OK),TRUE );
		}
		break;
	default:
		break;
	}

	return 0;
}

DWORD LoongBenedictionFrame::OnNS_SetNextBenediction( tagNS_SetNextBenediction* pMsg, DWORD dwParam )
{
	//神龙赐福奖励结束(该领的奖励都领完了)
	if( pMsg->isEnd )
	{
		if( m_step == EBeneStep_Count )
		{
			if( !GetObj("LoongBenedicitionCountOver_MsgBox") )
				KillObj( "LoongBenedicitionCountOver_MsgBox" );

			CreateObj( "LoongBenedicitionCountOver_MsgBox", "MsgBoxEx" );
			TObjRef<MsgBoxEx>("LoongBenedicitionCountOver_MsgBox")->Init( _T(""), g_StrTable[_T("LoongBebediction_GetCountOver")],_T("LoongBenedictionCountOver_MsgBoxEvent") );
		}

		m_pFrameMgr->AddToDestroyList( this );

		return 0;
	}

	if( pMsg->nextServerTime < 0 )
		return 0;
	
	m_dwNeedTime = pMsg->nextServerTime;
	m_fStepTime = (float)m_dwNeedTime;
	m_dwStartTimer = timeGetTime();

	if( !P_VALID(m_pWndCurDisplay) )
	{
		m_pWndCurDisplay  = m_pWndExpand;
		ShowWnd( m_pWndExpand, TRUE );
		ShowWnd( m_pWndFold, FALSE );
	}

	m_bCanUpdate = TRUE;
	ShowWnd( m_pWndCurDisplay, TRUE );
	EnableWnd( m_pBtnGetBenediction, FALSE );
	EnableWnd( m_pBtnFoldGetBenediction, FALSE );

	m_nItemTypeId = pMsg->nItemTypeID;
	m_nItemCount = pMsg->nItemCount;

	if( GT_VALID(pMsg->nItemTypeID) && pMsg->nItemCount > 0 )
	{
		const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto( pMsg->nItemTypeID );
		if( P_VALID(pItemProto) && P_VALID(pItemProto->pDisplayInfo ) )
		{
			TCHAR szPath[X_LONG_NAME] = {0};
			_sntprintf(szPath, X_LONG_NAME, _T("data\\ui\\Icon\\%s.tga"), pItemProto->pDisplayInfo->szMaleIcon );
			m_pStcItem->SetPic( szPath );
		}

		TCHAR szNum[10] = {0};
		_sntprintf( szNum, 10, _T("%d"), pMsg->nItemCount );
		m_pStcItem->SetText( szNum );
		ShowWnd( m_pStcItem,TRUE );
	}
	else
	{
		ShowWnd( m_pStcItem,FALSE );
	}

	if( GT_VALID(pMsg->n32Exprence ) )
	{
		/*TCHAR szNum[20] = {0};
		_sntprintf( szNum, 20, _T("%d"), pMsg->n32Exprence );
		m_pStcExprence->SetText( szNum  );
		m_pStcExprence->SetRefresh( TRUE );*/
	}

	return 0;
}


BOOL LoongBenedictionFrame::IsWindowExpand()
{
	return m_pWndCurDisplay == m_pWndExpand;
}

VOID LoongBenedictionFrame::ShowWnd( GUIWnd* pWnd, BOOL bShow )
{
	if( P_VALID( pWnd ) )
	{
		pWnd->SetInvisible( !bShow );
		pWnd->SetRefresh( TRUE );
	}
}

VOID LoongBenedictionFrame::EnableWnd( GUIButton* pBtn, bool bEnable )
{
	if( P_VALID( pBtn ) )
	{
		pBtn->SetEnable( bEnable );
		pBtn->SetRefresh( TRUE );
	}
}

void LoongBenedictionFrame::BeginGlint()
{
	//if( !m_pStcWaitTime->IsGlintNow() )
	//	m_pStcWaitTime->BeginGlint();
	//
	//if( !m_pStcFoldWaitTime->IsGlintNow() )
	//	m_pStcFoldWaitTime->BeginGlint();
	
	//if( !m_pBtnGetBenediction->IsGlintNow() )
	//	m_pBtnGetBenediction->BeginGlint();
	//
	//if( !m_pBtnFoldGetBenediction->IsGlintNow() )
	//	m_pBtnFoldGetBenediction->BeginGlint();

	m_pBtnGetBenediction->SetFlash(true,true);
	m_pBtnGetBenediction->SetRefresh(true);
	m_pBtnFoldGetBenediction->SetFlash(true,true);
	m_pBtnFoldGetBenediction->SetRefresh(true);
}

void LoongBenedictionFrame::EndGlint()
{
	//if( m_pStcWaitTime->IsGlintNow() )
	//	m_pStcWaitTime->EndGlint();

	//if( m_pStcFoldWaitTime->IsGlintNow() )
	//	m_pStcFoldWaitTime->EndGlint();

	//if( m_pBtnGetBenediction->IsGlintNow() )
	//	m_pBtnGetBenediction->EndGlint();

	//if( m_pBtnFoldGetBenediction->IsGlintNow() )
	//	m_pBtnFoldGetBenediction->EndGlint();

	m_pBtnGetBenediction->SetFlash(false,false);
	m_pBtnFoldGetBenediction->SetFlash(false,false);
}