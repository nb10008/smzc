#include "StdAfx.h"
#include "TrumpRefineFrame.h"
#include "ItemButton.h"
#include "ItemMgr.h"
#include "Item.h"
#include "ItemEvent.h"
#include "CombatSysUtil.h"
#include "ButtonEx.h"
#include "DragBtn.h"
#include "ItemFrame.h"
#include "RoleMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "..\WorldDefine\msg_compose.h"
#include "..\WorldDefine\msg_loot.h"

const INT TRUMP_TOP_LEVEL = 100;
RefineTrumpFrame::RefineTrumpFrame():m_Trunk(this),
m_pWnd(NULL),
m_pWndCaption(NULL),
m_pButNext(NULL),
m_pButPre(NULL),
m_pStcNum(NULL),
m_pIconTrump(NULL),
m_pStcTrumpName(NULL),
m_pStcTrumpLevel(NULL),
m_pProExp(NULL),
m_pIconStone(NULL),
m_pProTime(NULL),
m_pButRefine(NULL),
m_pButCancle(NULL),
m_pButQuit(NULL),
m_nNum(0),
m_nMaxNum(0),
m_bProcessing(false),
m_pTrump(NULL),
m_pStcExpShow(NULL),
m_nJie(0),
m_pItem(NULL)
{

}

RefineTrumpFrame::~RefineTrumpFrame()
{

}

BOOL RefineTrumpFrame::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if( !GameFrame::Init( pMgr, pFather, dwParam ) )
		return false;

	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2TrumpRefine"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RefineTrumpFrame::OnEventTrumpRefine));
	m_pCmdMgr->Register("NS_FabaoPractice",							(NETMSGPROC)m_Trunk.sfp2(&RefineTrumpFrame::NetTrumpRefine),			_T("NS_FabaoPractice"));
	m_pCmdMgr->Register("NS_RolePutDownItem",						(NETMSGPROC)m_Trunk.sfp2(&RefineTrumpFrame::NetRolePutDownItem),		_T("NS_RolePutDownItem"));
	m_pMgr->RegisterEventHandle( _T("TooManyYuLingShi"),			(FRAMEEVENTPROC)(m_Trunk.sfp1(&RefineTrumpFrame::TooManyYuLingShi)) );
	return true;
}

BOOL RefineTrumpFrame::Destroy()
{
	GameFrame::Destroy();
	LockItem(true);
	if( P_VALID(m_pWnd) )
		m_pGUI->DestroyWnd(m_pWnd);

	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2TrumpRefine"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RefineTrumpFrame::OnEventTrumpRefine));
	m_pCmdMgr->UnRegister("NS_FabaoPractice",							(NETMSGPROC)m_Trunk.sfp2(&RefineTrumpFrame::NetTrumpRefine));
	m_pCmdMgr->UnRegister("NS_RolePutDownItem",							(NETMSGPROC)m_Trunk.sfp2(&RefineTrumpFrame::NetRolePutDownItem));
	m_pMgr->UnRegisterEventHandler( _T("TooManyYuLingShi"),				(FRAMEEVENTPROC)(m_Trunk.sfp1(&RefineTrumpFrame::TooManyYuLingShi)) );

	if( P_VALID(GetObj("NoYuLingShi")) )
		KillObj("NoYuLingShi");
	if( P_VALID(GetObj("TooManyYuLingShi")) )
		KillObj("TooManyYuLingShi");
	//if( P_VALID(GetObj("TrumpLevelUp")) )
	//	KillObj("TrumpLevelUp");
	if( P_VALID(GetObj("NeedLevelUp")) )
		KillObj("NeedLevelUp");
	if( P_VALID(GetObj("NeedLevelUpEx")) )
		KillObj("NeedLevelUpEx");
	if( P_VALID(GetObj("TrumpLevelIsTop")) )
		KillObj("TrumpLevelIsTop");

	return true;
}


BOOL RefineTrumpFrame::ReloadUI()
{
	tstring strPath = g_strLocalPath + _T("\\ui\\fairy_cultivate.xml");
	XmlElement element;
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "fairy_cultivate_win\\fairy_cultivate_pic\\fairy_back\\fairy_pic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "fairy_cultivate_win\\fairy_cultivate_pic\\itemback\\itempic", "ClassName", "ItemButton");
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);

	m_pWndCaption = (GUIWnd*)m_pWnd->GetChild(_T("fairy_cultivate_pic"));
	m_pIconTrump = (ItemButton*)m_pWndCaption->GetChild(_T("fairy_back\\fairy_pic"));
	m_pStcTrumpName = (GUIStatic*)m_pWndCaption->GetChild(_T("name"));
	m_pStcTrumpLevel = (GUIStatic*)m_pWndCaption->GetChild(_T("grade"));
	m_pProExp = (GUIProgress*)m_pWndCaption->GetChild(_T("progpic0\\proqress"));
	m_pStcExpShow = (GUIStatic*)m_pWndCaption->GetChild(_T("progpic0\\num1"));
	m_pIconStone = (ItemButton*)m_pWndCaption->GetChild(_T("itemback\\itempic"));

	m_pStcNum = (GUIEditBox*)m_pWndCaption->GetChild(_T("cultivateback\\word4\\putinback\\putin"));
	m_pButNext = (GUIButton*)m_pWndCaption->GetChild(_T("cultivateback\\word4\\nextbutt"));
	m_pButPre = (GUIButton*)m_pWndCaption->GetChild(_T("cultivateback\\word4\\backbutt"));

	m_pProTime = (GUIProgress*)m_pWndCaption->GetChild(_T("cultivateback\\progpic\\progress"));

	m_pButRefine = (GUIButton*)m_pWndCaption->GetChild(_T("cultivateback\\cultivatebutt"));
	m_pButCancle = (GUIButton*)m_pWndCaption->GetChild(_T("cultivateback\\canclebutt"));
	m_pButQuit = (GUIButton*)m_pWndCaption->GetChild(_T("closebutt"));
	m_pWnd->SetInvisible(TRUE);

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&RefineTrumpFrame::EventHandler) );
	return true;
}

DWORD RefineTrumpFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Active:
		{
			if( m_bProcessing && pWnd == m_pStcNum )
				m_pGUI->SetActive(m_pButRefine);
		}
		break;
	case EGUIE_KeyDown:
		{
			if( m_pGUI->GetActive() == m_pStcNum && P_VALID(m_pItem) )
			{
				m_nNum = _tcstol(m_pStcNum->GetText(), NULL, 10);
				m_pIconStone->RefreshItem( m_pItem->GetItemTypeID(), m_nNum, m_pItem->GetItemQuality() );
			}
		}
		break;

	case EGUIE_Click:
		{
			if( pWnd == m_pButCancle || pWnd == m_pButQuit )
				ShowUI(FALSE);
			else if( pWnd == m_pButPre )
			{
				m_nNum = _tcstol(m_pStcNum->GetText(), NULL, 10);
				if(m_nNum <= 1)
					m_nNum = m_nMaxNum;
				else
					m_nNum--;
				RefreshStone();
			}
			else if( pWnd == m_pButNext )
			{
				m_nNum = _tcstol(m_pStcNum->GetText(), NULL, 10);
				if(m_nNum >= m_nMaxNum )
					m_nNum = 1;
				else
					m_nNum++;
				RefreshStone();
			}
			else if( pWnd == m_pButRefine )
			{
				if( Check() )
				{
					m_pProTime->SetSpeed(100);
					m_pProTime->SetValue(0, true);
					m_pProTime->SetMaxValue(3.0f);
					m_pProTime->SetRefresh();
					m_bProcessing = true;
					m_pButRefine->SetEnable(false);
					m_pButNext->SetEnable(false);
					m_pButPre->SetEnable(false);
				}
			}
		}
		break;
	case EGUISCB_Drop:
		if( pWnd == m_pIconStone )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutStone((INT16)LOWORD(pEvent->dwParam2));
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if( pWnd == m_pIconStone && P_VALID(m_pItem) )
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItem).c_str());
			if( pWnd == m_pIconTrump && P_VALID(m_pTrump) )
				ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(m_pTrump).c_str() );
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
	}

	return FALSE;
}

VOID RefineTrumpFrame::ShowUI( bool bFlag )
{
	if( bFlag )
	{
		if(P_VALID(m_pWnd) && m_pWnd->IsInvisible() )
		{
			m_pWnd->SetInvisible(!bFlag);
			m_pWnd->FlipToTop();
			UpdateUI();
		}
	}
	else
	{
		if( m_bProcessing )
		{
			m_bProcessing = false;
			m_pProTime->SetValue(0, true);
			m_pButRefine->SetEnable(true);
			m_pButNext->SetEnable(true);
			m_pButPre->SetEnable(true);
		}
		m_pWnd->SetInvisible(!bFlag);
		ClearInfo();
		SetStrategy(EIST_ItemPocket);
	}
}

VOID RefineTrumpFrame::UpdateUI()
{
	ClearInfo();
	m_pTrump = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	if( P_VALID(m_pTrump) )
	{
		DWORD dwTypeID = m_pTrump->GetItemTypeID();
		BYTE byQuality = m_pTrump->GetItemQuality();
		m_pIconTrump->RefreshItem( dwTypeID, 0, byQuality );
		tstring strName = m_pTrump->GetDisplay()->szName;
		m_pStcTrumpName->SetText(strName.c_str());

		const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
		const tagFabaoProto *pTrumpProto = (tagFabaoProto*)m_pTrump->GetProto();
		if(!P_VALID(pTrumpProto) && !(pTrumpSpe))
			return;

		TCHAR szTemp[X_LONG_NAME] = {0};
		_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pTrumpSpe->n16Stage );
		m_pStcTrumpLevel->SetText(szTemp);

		INT nCurExp = pTrumpSpe->nPracticeValue - ItemMgr::Inst()->GetTrumpCurExp( pTrumpSpe->n16Stage );
		INT nTotalExp = ItemMgr::Inst()->GetTrumpNextLevelExp( pTrumpSpe->n16Stage );
		m_pProExp->SetMaxValue((float)nTotalExp);
		m_pProExp->SetValue((float)nCurExp);
		m_pProExp->SetSpeed(10000000000.0f);
		_sntprintf( szTemp, X_LONG_NAME, _T("%d/%d"), nCurExp, nTotalExp );
		m_pStcExpShow->SetText(szTemp);
		m_pStcExpShow->FlipToTop();

		_sntprintf( szTemp, X_LONG_NAME, _T("%d"), m_nNum );;
		m_pStcNum->SetText(szTemp);
		m_pIconStone->RefreshItem();
	}
}

DWORD RefineTrumpFrame::OnEventTrumpRefine( tagItemPosChangeExEvent* pGameEvent )
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;
	PutStone(pGameEvent->n16IndexSrc);
	return 0;
}

void RefineTrumpFrame::LockItem( bool bOperable )
{
	if( P_VALID(m_pItem) )
		m_pItem->SetOperable(bOperable);
}

VOID RefineTrumpFrame::SendTrumpRefineMsg()
{
	m_bProcessing = false;
	tagNC_FabaoPractice msg;
	msg.n64FabaoID = m_pTrump->GetData()->n64Serial;
	msg.n64SrcItemID = m_pItem->GetData()->n64Serial;
	msg.nSrcItemNum = _tcstol(m_pStcNum->GetText(), NULL, 10);
	m_pSession->Send(&msg);
}

VOID RefineTrumpFrame::PutStone( INT16 nPos )
{
	LockItem(true);
	Item *pItem = ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(pItem))
		return;

	const tagItemProto* pItemProto = pItem->GetProto();
	if( pItemProto->eSpecFunc != EISF_YuLingZhu )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TrumpRefine_Error")]);
		return;
	}
	m_nMaxNum = pItem->GetItemQuantity();
	m_pStcNum->SetMaxValue(m_nMaxNum);
	m_nNum = 1;
	m_pItem = pItem;
	RefreshStone();
	LockItem(false);
}

DWORD RefineTrumpFrame::NetTrumpRefine( tagNS_FabaoPractice *pNetCmd, DWORD )
{
	m_pButRefine->SetEnable(true);
	m_pButNext->SetEnable(true);
	m_pButPre->SetEnable(true);
	if( pNetCmd->dwErrorCode == E_Success )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TrumpRefineSuccess")]);
		m_pTrump = ItemMgr::Inst()->GetCurEquip(EEP_Face);
		const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
		if(P_VALID(pTrumpSpe))
		{
			INT nStage = pTrumpSpe->n16Stage;
			if( m_nJie != pTrumpSpe->nSlotNum )
			{
				m_nJie = pTrumpSpe->nSlotNum;
				TCHAR szTemp[X_LONG_NAME] = {0};
				if( pTrumpSpe->nSlotNum <= 5)
					_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("TrumpLevelUp")], pTrumpSpe->n16Stage, pTrumpSpe->nSlotNum );
				else
					_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("TrumpLevelUp")], pTrumpSpe->n16Stage, 5 );
				CombatSysUtil::Inst()->ShowScreenCenterMsg(szTemp);
				LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
				if (P_VALID(pLp) && P_VALID(pLp->GetMount()))
					pLp->ClearTrumpEffect();
				else
					pLp->PlayTrumpEffect();
			}
		}
	}
	else if( pNetCmd->dwErrorCode == E_Fabao_LingzhuWanted )
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("LingzhuWanted")]);
	else
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TrumpRefineFail")]);
	UpdateUI();	
	return 0;
}

VOID RefineTrumpFrame::RefreshStone()
{
	if( !P_VALID(m_pItem) )
		return;
	m_pIconStone->RefreshItem( m_pItem->GetItemTypeID(), m_nNum, m_pItem->GetItemQuality() );
	m_pStcNum->SetText(_T(""));
	m_pStcNum->SetRefresh(true);
	TCHAR szCount[X_SHORT_NAME];
	_sntprintf( szCount, X_SHORT_NAME, _T("%d"), m_nNum );
	m_pStcNum->SetText(szCount);
}

VOID RefineTrumpFrame::ClearInfo()
{
	LockItem(true);
	m_pIconStone->RefreshItem();
	m_pStcTrumpName->SetText(_T(""));
	m_pStcTrumpLevel->SetText(_T(""));
	m_pProExp->SetText(_T(""));
	m_pIconTrump->RefreshItem();
	m_pStcNum->SetText(_T(""));
	m_pProTime->SetValue( 0.0f, true );
	m_pStcExpShow->SetText(_T(""));
	m_bProcessing = false;
	m_nJie = 0;
	m_nNum = 0;
	m_pItem = NULL;
	m_pWnd->SetRefresh(true);
}

BOOL RefineTrumpFrame::Check()
{
	if( m_nNum == 0 || !P_VALID(m_pTrump) || !P_VALID(m_pItem) )
	{
		if( P_VALID(GetObj("NoYuLingShi")) )
			KillObj("NoYuLingShi");
		CreateObj( "NoYuLingShi", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("NoYuLingShi")->Init( _T(""), g_StrTable[_T("NoYuLingShi")] );
		return FALSE;
	}
	const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
	const tagItemProto* pItemProto = m_pItem->GetProto();
	if( !P_VALID(pTrumpSpe) && !P_VALID(pItemProto) )
	{
		UpdateUI();
		return FALSE;
	}
	INT nCurExp = pTrumpSpe->nPracticeValue;
	if( nCurExp >= ItemMgr::Inst()->GetTrumpMaxExp() )
	{
		if( P_VALID(GetObj("TrumpLevelIsTop")) )
			KillObj("TrumpLevelIsTop");
		CreateObj( "TrumpLevelIsTop", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("TrumpLevelIsTop")->Init( _T(""), g_StrTable[_T("TrumpLevelIsTop")] );
		return FALSE;
	}
	INT nLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
	INT nTrumpMaxLevel = ItemMgr::Inst()->GetTrumpLevel( TRUMP_TOP_LEVEL );
	INT nMaxExp = ItemMgr::Inst()->GetTrumpRefineExp( nLevel ) - nCurExp;
	if( pItemProto->nSpecFuncVal1 > nMaxExp && nLevel < nTrumpMaxLevel )
	{
		INT nLPLevel = 0;
		INT nDengJie = 0;
		INT nTotal = nCurExp + pItemProto->nSpecFuncVal1;
		ItemMgr::Inst()->GetEquipTrumpLevel( nTotal, nLPLevel, nDengJie );
		if( nLPLevel - nLevel > 0 )
		{
			if( P_VALID(GetObj("NeedLevelUpEx")) )
				KillObj("NeedLevelUpEx");
			CreateObj( "NeedLevelUpEx", "MsgBoxEx" );
			TCHAR szTemp[X_LONG_NAME] = {0};
			_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("NeedLevelUpEx")], nDengJie, nLPLevel );
			TObjRef<MsgBoxEx>("NeedLevelUpEx")->Init( _T(""), szTemp );
		}
		//else
		//{
		//	if( P_VALID(GetObj("NeedLevelUp")) )
		//		KillObj("NeedLevelUp");
		//	CreateObj( "NeedLevelUp", "MsgBoxEx" );
		//	TCHAR szTemp[X_LONG_NAME] = {0};
		//	_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("NeedLevelUp")], nLPLevel );
		//	TObjRef<MsgBoxEx>("NeedLevelUp")->Init( _T(""), szTemp );
		//}
		
		return FALSE;
	}
	else if( pItemProto->nSpecFuncVal1 * m_nNum > nMaxExp )
	{
		INT nNum = nMaxExp / pItemProto->nSpecFuncVal1;
		if( nNum == 0 )
			return TRUE;
		if( P_VALID(GetObj("TooManyYuLingShi")) )
			KillObj("TooManyYuLingShi");
		CreateObj( "TooManyYuLingShi", "MsgBox" );
		TCHAR szTemp[X_LONG_NAME] = {0};
		_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("TooManyYuLingShi")], nNum );
		m_nNum = nNum;
		TObjRef<MsgBox>("TooManyYuLingShi")->Init( _T(""), szTemp, _T("TooManyYuLingShi"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE );
		return FALSE;
	}
	return TRUE;
}


VOID RefineTrumpFrame::Update()
{
	GameFrame::Update();

	if(m_bProcessing)
	{
		float deltaTime =  Cool3D::Kernel::Inst()->GetDeltaTime();
		float fcur = m_pProTime->GetCurrentValue();
		float fMax = m_pProTime->GetMaxValue();
		if(fcur < fMax)
		{
			fcur += deltaTime;
			m_pProTime->SetValue(fcur, true);
			m_pProTime->SetRefresh();
		}
		if(fcur >= fMax)
		{
			SendTrumpRefineMsg();
		}
	}
}

BOOL RefineTrumpFrame::EscCancel()
{
	if(m_pWnd->IsInvisible())
		return FALSE;
	else if( m_bProcessing )
	{
		m_bProcessing = false;
		m_pProTime->SetValue(0, true);
		m_pButRefine->SetEnable(true);
		m_pButNext->SetEnable(true);
		m_pButPre->SetEnable(true);
		return TRUE;
	}
	m_pWnd->SetInvisible(TRUE);
	ClearInfo();
	SetStrategy(EIST_ItemPocket);
	return TRUE;
}

DWORD RefineTrumpFrame::NetRolePutDownItem( tagNS_RolePutDownItem *pNetCmd, DWORD )
{
	if( pNetCmd->dwErrorCode == E_Success && P_VALID(m_pItem) )
	{
		INT64 n64ID = m_pItem->GetData()->n64Serial;
		Item *pItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
		if(!P_VALID(pItem))
			UpdateUI();
	}
	return 0;
}

DWORD RefineTrumpFrame::TooManyYuLingShi( tagMsgBoxEvent *pGameEvent )
{
	if( !P_VALID(m_pItem) )
		return 0;
	if( pGameEvent->eResult != MBF_OK )
		m_nNum = 1;
	TCHAR szTemp[X_SHORT_NAME] = {0};
	_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_nNum );
	m_pStcNum->SetText(szTemp);
	DWORD dwTypeID = m_pItem->GetItemTypeID();
	BYTE byQuality = m_pItem->GetItemQuality();
	m_pIconStone->RefreshItem( dwTypeID, m_nNum, byQuality );
	return 0;
}