#include "StdAfx.h"
#include "EasyBuyFrame.h"
#include "MallMgr.h"
#include "ItemButton.h"
#include "IconStatic.h"
#include "..\WorldDefine\ItemDefine.h"
#include "ItemProtoData.h"
#include "MallFrame.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "ServerTime.h"
#include "SaveAccount.h"

EasyBuyFrame::EasyBuyFrame( void ): m_Trunk(this)
{
	m_pWnd = NULL;
	m_pBtnMall = NULL;
	m_pBtnClose = NULL;
	for( int i = 0; i < UI_MAX_ITEM; i++ )
	{
		m_pItemSel[i] = NULL;
		m_pItem[i] = NULL;
		m_pStcYuanBao[i] = NULL;
		m_pStcBao[i] = NULL;
	}
}

EasyBuyFrame::~EasyBuyFrame( void )
{

}

BOOL EasyBuyFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	if(SaveAccount::Inst()->GetLoginMode() != 11)
	{
		m_pMgr->RegisterEventHandle(_T("Close_Package"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnClosePackageEasyBuyFrame));
		m_pMgr->RegisterEventHandle(_T("Close_Sociality"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseSocialEasyBuyFrame));
		m_pMgr->RegisterEventHandle(_T("Close_Ware"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseStorageEasyBuyFrame));
		m_pMgr->RegisterEventHandle(_T("Close_Task"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseQuestEasyBuyFrame));
		m_pMgr->RegisterEventHandle(_T("Close_Pet"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnClosePetEasyBuyFrame));
		m_pMgr->RegisterEventHandle(_T("Close_Pet_UI"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnClosePetEasyBuyFrame));
		m_pMgr->RegisterEventHandle(_T("Close_Role"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseRoleInfoEasyBuyFrame));
		m_pMgr->RegisterEventHandle(_T("Close_Dower"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseRoleSkillEasyBuyFrame));
		m_pMgr->RegisterEventHandle(_T("Close_Compose"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseComposeEasyBuyFrame));
	}


	return TRUE;
}

BOOL EasyBuyFrame::Destroy()
{
	GameFrame::Destroy();

	if(SaveAccount::Inst()->GetLoginMode() != 11)
	{
		m_pMgr->UnRegisterEventHandler(_T("Close_Package"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnClosePackageEasyBuyFrame));
		m_pMgr->UnRegisterEventHandler(_T("Close_Sociality"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseSocialEasyBuyFrame));
		m_pMgr->UnRegisterEventHandler(_T("Close_Ware"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseStorageEasyBuyFrame));
		m_pMgr->UnRegisterEventHandler(_T("Close_Task"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseQuestEasyBuyFrame));
		m_pMgr->UnRegisterEventHandler(_T("Close_Pet"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnClosePetEasyBuyFrame));
		m_pMgr->UnRegisterEventHandler(_T("Close_Pet_UI"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnClosePetEasyBuyFrame));
		m_pMgr->UnRegisterEventHandler(_T("Close_Role"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseRoleInfoEasyBuyFrame));
		m_pMgr->UnRegisterEventHandler(_T("Close_Dower"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseRoleSkillEasyBuyFrame));
		m_pMgr->UnRegisterEventHandler(_T("Close_Compose"), (FRAMEEVENTPROC)m_Trunk.sfp1(&EasyBuyFrame::OnCloseComposeEasyBuyFrame));
	}


	if( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	return TRUE;
}


BOOL EasyBuyFrame::ReloadUI()
{
	return TRUE;
}

BOOL EasyBuyFrame::ReloadUI( LPCSTR alignCtrl, LPCSTR alignType, LPCSTR szNewName/* = NULL*/ )
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\mall_convi.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	for( int i=0; i < UI_MAX_ITEM; i++ )
	{
		CHAR szFullName[X_LONG_NAME] = {0};
		_snprintf( szFullName, sizeof(szFullName), "mall_conviwin\\mall_comvipic\\back\\itemback%d", i+1 );
		m_pGUI->ChangeXml(&element, szFullName, "ClassName", "IconStatic");
		_snprintf( szFullName, sizeof(szFullName), "mall_conviwin\\mall_comvipic\\back\\itemback%d\\item", i+1 );
		m_pGUI->ChangeXml(&element, szFullName, "ClassName", "MallButton");
	}

	m_pGUI->ChangeXml( &element, "mall_conviwin", "AlignControl", alignCtrl);
	m_pGUI->ChangeXml( &element, "mall_conviwin", "AlignType", alignType);
	
	if( NULL != szNewName )
	{
		m_pGUI->ChangeXml( &element, "mall_conviwin", "Name", szNewName );
		LPCTSTR pStr = m_pUtil->AnsiToUnicode(alignType);
		std::stringstream streamX;
		if ( _tcsnccmp( pStr, _T("OuterLeftTop"), 12 ) == 0 )
		{
			streamX << 5;
			m_pGUI->ChangeXml( &element, szNewName, "AlignOffset_x", streamX.str().c_str() );
		}
		else if ( _tcsnccmp( pStr, _T("OuterRightTop"), 13 ) == 0 )
		{
			streamX << -5;
			m_pGUI->ChangeXml( &element, szNewName, "AlignOffset_x", streamX.str().c_str() );
		}
	}

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	if( !P_VALID( m_pWnd ) )
		return FALSE;

	m_pBtnMall = (GUIButton*)m_pWnd->GetChild( _T("mall_comvipic\\button") );
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild( _T("mall_comvipic\\title\\closebutt") );
	for( int i = 0; i < UI_MAX_ITEM; i++ )
	{
		TCHAR szFullName[X_LONG_NAME] = {0};
		_sntprintf( szFullName, sizeof(szFullName), _T("mall_comvipic\\back\\itemback%d"), i+1);
		m_pItemSel[i] = (IconStatic*)m_pWnd->GetChild(szFullName);
		m_pItemSel[i]->SetInvisible(true);
		_sntprintf(szFullName, sizeof(szFullName), _T("mall_comvipic\\back\\itemback%d\\item"), i+1);
		m_pItem[i] = (MallButton*)m_pWnd->GetChild(szFullName);
		m_pItem[i]->SetInvisible(true);
		_sntprintf(szFullName, sizeof(szFullName), _T("mall_comvipic\\back\\price%d"), i+1);
		m_pStcYuanBao[i] = (GUIStatic*)m_pWnd->GetChild(szFullName);
		m_pStcYuanBao[i]->SetInvisible(true);
		_sntprintf(szFullName, sizeof(szFullName), _T("mall_comvipic\\back\\word%d"), i);
		m_pStcBao[i] = (GUIStatic*)m_pWnd->GetChild(szFullName);
		m_pStcBao[i]->SetInvisible(true);
	}
	m_pWnd->SetInvisible( TRUE );

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&EasyBuyFrame::EventHandler) );
	Align();

	return TRUE;
}

BOOL EasyBuyFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnClose )
			{
				Hide();
			}
			else if ( pWnd == m_pBtnMall )
			{
				tagGameEvent eve(_T("Open_Mall_UI"), this);
				m_pFrameMgr->SendEvent(&eve);
			}
			else if (_tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0)
			{
				for( int i = 0; i < UI_MAX_ITEM; i++ )
				{
					const TCHAR *szSelectItem = _T("data\\ui\\Common\\L_icon-l.bmp");
					if( pWnd == m_pItem[i] )
					{
						RefreshItemFlag();
						if( m_pItem[i]->GetBtnData().GetData1() != GT_INVALID )
						{
							MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(m_pItem[i]->GetBtnData().GetData1());
							MallMgr::Inst()->GetMallFrame()->SetCurMenu( MallFrame::EMIT_All );
							m_pItemSel[i]->SetPic(szSelectItem);
							MallMgr::Inst()->GetMallFrame()->BuyMallItem();
							break;
						}
					}
				}
			}
		}
		break;
	case EGUIE_Drag:
		{
			if (_tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0)
			{
				for( int i = 0; i < UI_MAX_ITEM; i++ )
				{
					const TCHAR *szSelectItem = _T("data\\ui\\Common\\L_icon-l.bmp");
					if( pWnd == m_pItem[i] )
					{
						RefreshItemFlag();
						if( m_pItem[i]->GetBtnData().GetData1() != GT_INVALID )
						{
							m_pItemSel[i]->SetPic(szSelectItem);
							break;
						}
					}
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if( _tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0 )
			{
				for( int i = 0; i < UI_MAX_ITEM; i++ )
				{
					if( pWnd == m_pItem[i] )
					{
						DWORD dwID =  m_pItem[i]->GetBtnData().GetData2();
						ShowTip(m_pItem[i], ToolTipCreator::Inst()->GetItemTips(dwID).c_str());
						return 0;
					}
				}
			}

		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(NULL, _T(""));
		}
		break;
	default:
		break;
	}

	return TRUE;
}


VOID EasyBuyFrame::Align( void )
{
	if( P_VALID( m_pWnd ) )
		m_pWnd->Align();
}

BOOL EasyBuyFrame::IsVisible( void ) const
{
	if( P_VALID( m_pWnd ) && !m_pWnd->IsInvisible() )
		return TRUE;
	else
		return FALSE;
}

VOID EasyBuyFrame::Show( DWORD dwUiType )
{
	if(SaveAccount::Inst()->GetLoginMode() == 11)	//韩国不要快捷购买
		return;

	if( P_VALID(m_pWnd) ) 
	{
		vector<tagUIItem> vecUIItem;
		vecUIItem = MallMgr::Inst()->GetEasyBuyItem();
		int nNum = 0;
		for( size_t i = 0; i < vecUIItem.size(); i++ )
		{
			TCHAR szBuff[X_LONG_NAME] = {0};
			if( vecUIItem[i].dwUIType == dwUiType )
			{
				tagMallItemProto* pProItem = MallMgr::Inst()->GetMallItemByTypeid( vecUIItem[i].dwTypeID );
				if( P_VALID(pProItem) && nNum < 3 )		
				{
					const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(pProItem->dwTypeID);
					const tagItemDisplayInfo *pInfo = ItemProtoData::Inst()->FindItemDisplay(pProItem->dwTypeID);
					if( P_VALID(pItem) && P_VALID(pInfo) )
					{
						m_pItem[nNum]->RefreshItem(pProItem->dwTypeID, 1, pItem->byQuality);
						m_pItem[nNum]->SetBtnData(ESBT_Item, pProItem->dwID, pProItem->dwTypeID, pProItem->nIndexInServer);
						DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pProItem->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
						if( dwTime>0 && pProItem->dwTimeSaleEnd!=GT_INVALID
							&& pProItem->nSalePrice!=GT_INVALID && pProItem->nPrice!=pProItem->nSalePrice )
							_sntprintf(szBuff, sizeof(szBuff), _T("%d"), pProItem->nSalePrice);
						else 
							_sntprintf(szBuff, sizeof(szBuff), _T("%d"), pProItem->nPrice);
						m_pStcYuanBao[nNum]->SetText(szBuff);
						m_pItem[nNum]->SetInvisible(false);
						m_pItemSel[nNum]->SetInvisible(false);
						m_pStcYuanBao[nNum]->SetInvisible(false);
						m_pStcBao[nNum]->SetInvisible(false);
						nNum++;
					}
				}
			}
		}

		if( nNum != 0)
		{
			m_pWnd->SetInvisible(FALSE);
			m_pWnd->FlipToTop();
		}
	}
}

VOID EasyBuyFrame::Hide( void )
{
	if( P_VALID(m_pWnd) ) 
		m_pWnd->SetInvisible(TRUE);
}

BOOL EasyBuyFrame::EscCancel()
{
	if( P_VALID(m_pWnd) && !m_pWnd->IsInvisible() )
	{
		return TRUE;
	}
	return GameFrame::EscCancel();
}

void EasyBuyFrame::RefreshItemFlag()
{
	tstring strPic = _T("data\\ui\\common\\l_icon.bmp");
	for( int i=0; i< UI_MAX_ITEM; i++ )
	{
		m_pItemSel[i]->SetPic(strPic);
	}
}

DWORD EasyBuyFrame::OnClosePackageEasyBuyFrame( tagGameEvent* pGameEvent )
{
	EasyBuyFrame *pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame( _T("PocketEasyBuyFrame") );
	if( P_VALID(pFrame) )
		m_pFrameMgr->AddToDestroyList( pFrame );
	return 0;
}
DWORD EasyBuyFrame::OnCloseSocialEasyBuyFrame( tagGameEvent* pGameEvent )
{
	EasyBuyFrame *pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame( _T("SocialEasyBuyFrame") );
	if( P_VALID(pFrame) )
		m_pFrameMgr->AddToDestroyList( pFrame );
	return 0;
}
DWORD EasyBuyFrame::OnCloseStorageEasyBuyFrame( tagGameEvent* pGameEvent )
{
	EasyBuyFrame *pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame( _T("StorageEasyBuyFrame") );
	if( P_VALID(pFrame) )
		m_pFrameMgr->AddToDestroyList( pFrame );
	return 0;
}

DWORD EasyBuyFrame::OnCloseQuestEasyBuyFrame( tagGameEvent* pGameEvent )
{
	EasyBuyFrame *pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame( _T("QuestEasyBuyFrame") );
	if( P_VALID(pFrame) )
		m_pFrameMgr->AddToDestroyList( pFrame );
	return 0;
}

DWORD EasyBuyFrame::OnClosePetEasyBuyFrame( tagGameEvent* pGameEvent )
{
	EasyBuyFrame *pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame( _T("PetEasyBuyFrame") );
	if( P_VALID(pFrame) )
		m_pFrameMgr->AddToDestroyList( pFrame );
	return 0;
}

DWORD EasyBuyFrame::OnCloseRoleInfoEasyBuyFrame( tagGameEvent* pGameEvent )
{
	EasyBuyFrame *pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame( _T("RoleInfoEasyBuyFrame") );
	if( P_VALID(pFrame) )
		m_pFrameMgr->AddToDestroyList( pFrame );
	return 0;
}

DWORD EasyBuyFrame::OnCloseRoleSkillEasyBuyFrame( tagGameEvent* pGameEvent )
{
	EasyBuyFrame *pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame( _T("RoleSkillEasyBuyFrame") );
	if( P_VALID(pFrame) )
		m_pFrameMgr->AddToDestroyList( pFrame );
	return 0;
}

DWORD EasyBuyFrame::OnCloseComposeEasyBuyFrame( tagGameEvent* pGameEvent )
{
	EasyBuyFrame *pFrame = (EasyBuyFrame*)m_pFrameMgr->GetFrame( _T("ComposeEasyBuyFrame") );
	if( P_VALID(pFrame) )
		m_pFrameMgr->AddToDestroyList( pFrame );
	return 0;
}