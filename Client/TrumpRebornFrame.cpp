#include "StdAfx.h"
#include "TrumpRebornFrame.h"
#include "ItemMgr.h"
#include "ItemProtoData.h"
#include "ItemEvent.h"
#include "Item.h"
#include "CombatSysUtil.h"
#include "ItemButton.h"
#include "ItemFrame.h"
#include "..\WorldDefine\msg_compose.h"
#include "ConsolidateData.h"
#include "TrainMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "..\WorldDefine\msg_item.h"

TrumpRebornFrame::TrumpRebornFrame():m_Trunk(this),
m_pWnd(NULL),
m_pWndCaption(NULL),
m_pMainReverseTrump(NULL),
m_pMainReverseTrumpName(NULL),
m_pMainReverseTrumpLevel(NULL),
m_pAssisReverseTrump(NULL),
m_pStcLikely(NULL),
m_pBtnReverseOK(NULL),
m_pStcReverseRule(NULL),
//m_pMainGrownTrump(NULL),
//m_pMainGrownTrumpName(NULL),
//m_pMainGrownTrumpLevel(NULL),
//m_pAssisGrownTrump(NULL),
//m_pBtnGrownOK(NULL),
//m_pStcGrownRule(NULL),
m_pButCancle(NULL),
m_pButExit(NULL),
m_pTrump(NULL),
m_pItem_Left(NULL),
//m_pItem_Right(NULL),
m_pAssisReverseTrumpName(NULL),
m_pAssisReverseTrumpLevel(NULL)
//m_pAssisGrownTrumpName(NULL),
//m_pAssisGrownTrumpLevel(NULL)
{

}

TrumpRebornFrame::~TrumpRebornFrame()
{

}

BOOL TrumpRebornFrame::Destroy()
{
	m_pCmdMgr->UnRegister("NS_FabaoWuxingReversal",			(NETMSGPROC)m_Trunk.sfp2(&TrumpRebornFrame::NS_FabaoWuxingReversal));
	m_pCmdMgr->UnRegister("NS_FabaoWuxingMerging",			(NETMSGPROC)m_Trunk.sfp2(&TrumpRebornFrame::NS_FabaoWuxingMerging));
	m_pCmdMgr->UnRegister("NS_ItemRemove",				(NETMSGPROC)m_Trunk.sfp2(&TrumpRebornFrame::NetItemRemove));
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Reborn"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpRebornFrame::OnEventItemEvent_Move2Reborn));
	m_pMgr->UnRegisterEventHandler(_T("UseTrumpReversalCheck"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpRebornFrame::UseTrumpReversalCheck));
	//m_pMgr->UnRegisterEventHandler(_T("UseTrumpMergingCheck"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpRebornFrame::UseTrumpMergingCheck));

	GameFrame::Destroy();
	if( P_VALID(m_pWnd) )
		m_pGUI->DestroyWnd(m_pWnd);

	if(P_VALID(GetObj("UseTrumpReversalCheck")))
		KillObj("UseTrumpReversalCheck");

	//if(P_VALID(GetObj("UseTrumpMergingCheck")))
	//	KillObj("UseTrumpMergingCheck");
	return true;
}

VOID TrumpRebornFrame::Update()
{
}

BOOL TrumpRebornFrame::EscCancel()
{
	if(m_pWnd->IsInvisible())
		return FALSE;
	m_pItem_Left = NULL;
	//m_pItem_Right = NULL;
	m_pWnd->SetInvisible(TRUE);
	ClearInfo();
	SetStrategy(EIST_ItemPocket);
	return TRUE;
}

BOOL TrumpRebornFrame::ReloadUI()
{
	tstring strPath = g_strLocalPath + _T("\\ui\\fairy_fuse.xml");
	XmlElement element;
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "fuse_win\\fuse_pic\\fairyback0\\fairypic", "ClassName", "ItemButton");
	//m_pGUI->ChangeXml(&element, "five_win\\five_pic\\back1\\skilback2\\skill1", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "fuse_win\\fuse_pic\\fairyback1\\fairypic", "ClassName", "ItemButton");
	//m_pGUI->ChangeXml(&element, "five_win\\five_pic\\back2\\skilback2\\skill1", "ClassName", "ItemButton");
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);

	m_pWndCaption = (GUIWnd*)m_pWnd->GetChild(_T("fuse_pic"));
	m_pMainReverseTrump = (ItemButton*)m_pWndCaption->GetChild(_T("fairyback0\\fairypic"));
	m_pMainReverseTrumpName = (GUIStatic*)m_pWndCaption->GetChild(_T("name0"));
	m_pMainReverseTrumpLevel = (GUIStatic*)m_pWndCaption->GetChild(_T("grade0"));
	m_pAssisReverseTrump= (ItemButton*)m_pWndCaption->GetChild(_T("fairyback1\\fairypic"));
	m_pStcLikely = (GUIStatic*)m_pWndCaption->GetChild(_T("word3"));
	m_pBtnReverseOK = (GUIButton*)m_pWndCaption->GetChild(_T("fusebutton"));
	m_pStcReverseRule = (GUIStaticEx*)m_pWndCaption->GetChild(_T("tt_conpic\\tcontent"));

	//m_pMainGrownTrump = (ItemButton*)m_pWndCaption->GetChild(_T("back2\\skilback1\\skill1"));
	//m_pMainGrownTrumpName = (GUIStatic*)m_pWndCaption->GetChild(_T("back2\\word2"));
	//m_pMainGrownTrumpLevel = (GUIStatic*)m_pWndCaption->GetChild(_T("back2\\word3"));
	//m_pAssisGrownTrump= (ItemButton*)m_pWndCaption->GetChild(_T("back2\\skilback2\\skill1"));
	//m_pBtnGrownOK = (GUIButton*)m_pWndCaption->GetChild(_T("back2\\transferbutt"));
	//m_pStcGrownRule = (GUIStaticEx*)m_pWndCaption->GetChild(_T("back2\\tt_conpic\\contentWnd\\tcontent"));

	m_pButCancle = (GUIButton*)m_pWndCaption->GetChild(_T("closebutt"));
	m_pButExit = (GUIButton*)m_pWndCaption->GetChild(_T("canclebutt"));

	m_pAssisReverseTrumpName = (GUIStatic*)m_pWndCaption->GetChild(_T("name1"));
	m_pAssisReverseTrumpLevel= (GUIStatic*)m_pWndCaption->GetChild(_T("grade1"));

	//m_pAssisGrownTrumpName	 =	(GUIStatic*)m_pWndCaption->GetChild(_T("back2\\word6"));
	//m_pAssisGrownTrumpLevel  =	(GUIStatic*)m_pWndCaption->GetChild(_T("back2\\word7"));

	ClearInfo();
	UpdateUI();
	return true;
}

BOOL TrumpRebornFrame::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if( !GameFrame::Init( pMgr, pFather, dwParam ) )
		return false;

	m_pCmdMgr->Register("NS_FabaoWuxingReversal",	(NETMSGPROC)m_Trunk.sfp2(&TrumpRebornFrame::NS_FabaoWuxingReversal),		_T("NS_FabaoWuxingReversal"));
	m_pCmdMgr->Register("NS_FabaoWuxingMerging",	(NETMSGPROC)m_Trunk.sfp2(&TrumpRebornFrame::NS_FabaoWuxingMerging),		_T("NS_FabaoWuxingMerging"));
	m_pCmdMgr->Register("NS_ItemRemove",			(NETMSGPROC)m_Trunk.sfp2(&TrumpRebornFrame::NetItemRemove),			_T("NS_ItemRemove"));
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2Reborn"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpRebornFrame::OnEventItemEvent_Move2Reborn));
	m_pMgr->RegisterEventHandle(_T("UseTrumpReversalCheck"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpRebornFrame::UseTrumpReversalCheck));
	//m_pMgr->RegisterEventHandle(_T("UseTrumpMergingCheck"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpRebornFrame::UseTrumpMergingCheck));

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&TrumpRebornFrame::EventHandler) );
	return true;
}

DWORD TrumpRebornFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pButCancle || pWnd == m_pButExit )
			{
				m_pWnd->SetInvisible(TRUE);
				ClearInfo();
				m_pItem_Left = NULL;
				//m_pItem_Right = NULL;
				SetStrategy(EIST_ItemPocket);
			}
			if( pWnd == m_pBtnReverseOK )
			{
				if(!CheckMainTrumpLevel())
					break;
				if (!P_VALID(m_pItem_Left))
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Trump_NoAssisTrump")]);
					break;
				}
				if(P_VALID(GetObj("UseTrumpReversalCheck")))
					KillObj("UseTrumpReversalCheck");

				CreateObj("UseTrumpReversalCheck", "MsgBox");
				TObjRef<MsgBox>("UseTrumpReversalCheck")->Init(_T(""), g_StrTable[_T("Trump_FabaoWuxingReversalCheck")], _T("UseTrumpReversalCheck"));

				break;
			}
			//if( pWnd == m_pBtnGrownOK )
			//{
			//	if(!CheckMainTrumpLevel())
			//		break;
			//	if (!P_VALID(m_pItem_Right))
			//	{
			//		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Trump_NoAssisTrump")]);
			//		break;
			//	}

			//	if(P_VALID(GetObj("UseTrumpMergingCheck")))
			//		KillObj("UseTrumpMergingCheck");

			//	CreateObj("UseTrumpMergingCheck", "MsgBox");
			//	TObjRef<MsgBox>("UseTrumpMergingCheck")->Init(_T(""), g_StrTable[_T("Trump_FabaoWuxingMerging")], _T("UseTrumpMergingCheck"));

			//	break;
			//}
		}
		break;
	case EGUISCB_Drop:
		if( pWnd == m_pAssisReverseTrump )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutAssisTrumpLeft((INT16)LOWORD(pEvent->dwParam2));
			UpdateUI();
		}
		//if( pWnd == m_pAssisGrownTrump )
		//{
		//	if( EDT_Item == (EDragType)pEvent->dwParam1 )
		//		PutAssisTrumpRight((INT16)LOWORD(pEvent->dwParam2));
		//	UpdateUI();
		//}
		break;
	case EGUIE_Drag:
		if ( pWnd == m_pAssisReverseTrump )
		{
			m_pItem_Left = NULL;
			m_pAssisReverseTrump->SetPicFileName(TRUMP_DIBAN);
			UpdateUI();
			break;
		}
		//if ( pWnd == m_pAssisGrownTrump )
		//{
		//	m_pItem_Right = NULL;
		//	m_pAssisGrownTrump->SetPicFileName(TRUMP_DIBAN);
		//	UpdateUI();
		//	break;
		//}
	case EGUIE_MoveIntoCtrl:
		if (pWnd == m_pMainReverseTrump ) // || pWnd == m_pMainGrownTrump)
		{
			m_pTrump = ItemMgr::Inst()->GetCurEquip(EEP_Face);
			if( P_VALID(m_pTrump) )
			{
				const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
				if(!P_VALID(pTrumpSpe))
					return false;

				ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(m_pTrump).c_str() );
			}
		}
		if (pWnd == m_pAssisReverseTrump)
		{
			if (P_VALID(m_pItem_Left))
			{
				ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItem_Left).c_str() );
			}	
		}
		//if (pWnd == m_pAssisGrownTrump)
		//{
		//	if (P_VALID(m_pItem_Right))
		//	{
		//		ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItem_Right).c_str() );
		//	}	
		//}
		break;
	}

	return FALSE;
}

VOID TrumpRebornFrame::ShowUI( bool bFlag )
{
	if(P_VALID(m_pWnd) && m_pWnd->IsInvisible() )
		m_pWnd->SetInvisible(!bFlag);
	else
		m_pWnd->SetInvisible(!bFlag);

	if(bFlag)
	{
		UpdateUI();
	}
	else
	{
		ClearInfo();
		m_pItem_Left = NULL;
		//m_pItem_Right = NULL;
		SetStrategy(EIST_ItemPocket);
	}
}

VOID TrumpRebornFrame::UpdateUI()
{
	m_pTrump = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	if( P_VALID(m_pTrump) )
	{
		DWORD dwTypeID = m_pTrump->GetItemTypeID();
		BYTE byQuality = m_pTrump->GetItemQuality();
		m_pMainReverseTrump->RefreshItem( dwTypeID, 0, byQuality );
		//m_pMainGrownTrump->RefreshItem( dwTypeID, 0, byQuality );
		tstring strName = m_pTrump->GetDisplay()->szName;
		m_pMainReverseTrumpName->SetText(strName.c_str());
		//m_pMainGrownTrumpName->SetText(strName.c_str());

		const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
		const tagFabaoProto *pTrumpProto = (tagFabaoProto*)m_pTrump->GetProto();
		if(!P_VALID(pTrumpProto) && !(pTrumpSpe))
			return;

		TCHAR szTemp[X_LONG_NAME] = {0};
		_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pTrumpSpe->n16Stage );
		m_pMainReverseTrumpLevel->SetText(szTemp);
		//m_pMainGrownTrumpLevel->SetText(szTemp);

		m_pStcLikely->SetText(_T(""));// 相似率
		m_pAssisReverseTrumpName->SetText(_T(""));
		m_pAssisReverseTrumpLevel->SetText(_T(""));
		//m_pAssisGrownTrumpName->SetText(_T(""));
		//m_pAssisGrownTrumpLevel->SetText(_T(""));
		m_pAssisReverseTrump->RefreshItem();
		//m_pAssisGrownTrump->RefreshItem();

		// 左显示副法宝
		if (P_VALID(m_pItem_Left))
		{
			const tagItemProto *pProto_Left  = m_pItem_Left->GetProto();
			if( P_VALID(pProto_Left) )
			{
				dwTypeID = pProto_Left->dwTypeID;
				byQuality = pProto_Left->byQuality;
				m_pAssisReverseTrump->RefreshItem( m_pItem_Left->GetItemTypeID(), 0, m_pItem_Left->GetItemQuality() );

				tstring strNamel = m_pItem_Left->GetDisplay()->szName;
				m_pAssisReverseTrumpName->SetText(strNamel.c_str());
				const tagFabaoSpec *pTrumpSpel = static_cast<Equipment*>(m_pItem_Left)->GetFaBaoSpe();
				TCHAR szTempl[X_LONG_NAME] = {0};
				_sntprintf( szTempl, X_LONG_NAME, _T("%d"), pTrumpSpel->n16Stage );
				m_pAssisReverseTrumpLevel->SetText(szTempl);

				const tagFabaoSpec *pTrumpSpe_Left = m_pItem_Left->GetFaBaoSpe();
				if( !P_VALID(pTrumpSpe_Left) )
					return;

				INT GoodState = pTrumpSpe->n16NativeIntelligence / 2 + 
					( pTrumpSpe_Left->n16NativeIntelligence + pTrumpSpe_Left->n16Stage ) / 4;

				if (GoodState >= 90)
				{
					m_pStcLikely->SetText( g_StrTable[_T("Trump_VeryNice")] );
				}
				else if (GoodState >= 80)
				{
					m_pStcLikely->SetText( g_StrTable[_T("Trump_Nice")] );
				}
				else if (GoodState >= 70)
				{
					m_pStcLikely->SetText( g_StrTable[_T("Trump_Better")] );
				}
				else if (GoodState >= 60)
				{
					m_pStcLikely->SetText( g_StrTable[_T("Trump_LittleBetter")] );
				}
				else
					m_pStcLikely->SetText( g_StrTable[_T("Trump_Normal")] );
			}
		}
		
		// 右显示副法宝
		//if (P_VALID(m_pItem_Right))
		//{
		//	const tagItemProto *pProto_Right  = m_pItem_Right->GetProto();
		//	if( P_VALID(pProto_Right) )
		//	{
		//		tstring strNamel = m_pItem_Right->GetDisplay()->szName;
		//		m_pAssisGrownTrumpName->SetText(strNamel.c_str());
		//		const tagFabaoSpec *pTrumpSpel = static_cast<Equipment*>(m_pItem_Right)->GetFaBaoSpe();
		//		TCHAR szTempl[X_LONG_NAME] = {0};
		//		INT nStagel = pTrumpSpel->n16Stage;
		//		INT nJiel = ( nStagel - 1 ) / 10 + 1;
		//		INT nDengl = nStagel % 10;
		//		if ( 0 == nDengl)
		//		{
		//			nDengl = 10;
		//		}
		//		_sntprintf( szTempl, X_LONG_NAME, g_StrTable[_T("TrumpLevel")], nJiel, nDengl );
		//		m_pAssisGrownTrumpLevel->SetText(szTempl);

		//		dwTypeID = pProto_Right->dwTypeID;
		//		byQuality = pProto_Right->byQuality;
		//		m_pAssisGrownTrump->RefreshItem( m_pItem_Right->GetItemTypeID(), 0, m_pItem_Right->GetItemQuality() );
		//	}
		//}
	}
}

VOID TrumpRebornFrame::ClearInfo()
{

	m_pMainReverseTrumpName->SetText(_T(""));
	m_pMainReverseTrumpLevel->SetText(_T(""));
	m_pStcLikely->SetText(_T(""));
	m_pStcReverseRule->SetText(g_StrTable[_T("Trump_ReversalDesc")]);
	//m_pMainGrownTrumpName->SetText(_T(""));
	//m_pMainGrownTrumpLevel->SetText(_T(""));
	//m_pStcGrownRule->SetText(g_StrTable[_T("Trump_GrownDesc")]);

	m_pAssisReverseTrumpName->SetText(_T(""));
	m_pAssisReverseTrumpLevel->SetText(_T(""));
	//m_pAssisGrownTrumpName->SetText(_T(""));
	//m_pAssisGrownTrumpLevel->SetText(_T(""));


	m_pMainReverseTrump->RefreshItem();
	m_pAssisReverseTrump->RefreshItem();
	//m_pMainGrownTrump->RefreshItem();
	//m_pAssisGrownTrump->RefreshItem();

	//tagPoint size = m_pStcReverseRule->GetSize();
	//if( m_pStcReverseRule->GetTextTotalHeight() > size.y )
	//{
	//	size.y = m_pStcReverseRule->GetTextTotalHeight();
	//	m_pStcReverseRule->SetResizable(TRUE);
	//	m_pStcReverseRule->Resize( size );
	//	m_pStcReverseRule->SetResizable(FALSE);
	//}
	//m_pStcReverseRule->SetRefresh(TRUE);

	//tagPoint size2 = m_pStcGrownRule->GetSize();
	//if( m_pStcGrownRule->GetTextTotalHeight() > size2.y )
	//{
	//	size2.y = m_pStcGrownRule->GetTextTotalHeight();
	//	m_pStcGrownRule->SetResizable(TRUE);
	//	m_pStcGrownRule->Resize( size2 );
	//	m_pStcGrownRule->SetResizable(FALSE);
	//}
	//m_pStcGrownRule->SetRefresh(TRUE);

}

VOID TrumpRebornFrame::PutAssisTrumpLeft(INT nPos)
{
	m_pItem_Left = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(m_pItem_Left))
		return;

	const tagItemProto* pItemProto = m_pItem_Left->GetProto();
	const tagFabaoProto *pTrumpProto = (tagFabaoProto*)m_pTrump->GetProto();
	const tagFabaoSpec	*pTrumpSpe = m_pTrump->GetFaBaoSpe();
	if ( pItemProto->eType != EIT_FaBao)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Trump_NotTrump")]);
		m_pItem_Left = NULL;
		return;
	}
	//else if ( P_VALID(m_pItem_Right))
	//{
	//	if (m_pItem_Left->GetItemId() == m_pItem_Right->GetItemId())
	//	{
	//		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Trump_SameTrump")]);
	//		m_pItem_Left = NULL;
	//		return;
	//	}
	//}
	else if( static_cast<Equipment*>(m_pItem_Left)->IsNotIdetified())
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Trump_NotJianDing")]);
		m_pItem_Left = NULL;
		return;
	}
	else if( pTrumpSpe->n16Stage < 20 )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Trump_LevelIsTooLow")]);
		m_pItem_Left = NULL;
		return;
	}
	else
	{
		m_pAssisReverseTrump->RefreshItem( m_pItem_Left->GetItemTypeID(), 0, m_pItem_Left->GetItemQuality() );
	}

	UpdateUI();

}

//VOID TrumpRebornFrame::PutAssisTrumpRight(INT nPos)
//{
//	m_pItem_Right = ItemMgr::Inst()->GetPocketItem(nPos);
//	if(!P_VALID(m_pItem_Right))
//		return;
//
//	const tagItemProto* pItemProto = m_pItem_Right->GetProto();
//	const tagFabaoProto *pTrumpProto = (tagFabaoProto*)m_pTrump->GetProto();
//	const tagFabaoSpec	*pTrumpSpe = m_pTrump->GetFaBaoSpe();
//	if ( pItemProto->eType != EIT_FaBao)
//	{
//		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Trump_NotTrump")]);
//		m_pItem_Right = NULL;
//		return;
//	}
//	else if ( P_VALID(m_pItem_Left) )
//	{
//		if (m_pItem_Right->GetItemId() == m_pItem_Left->GetItemId())
//		{
//			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Trump_SameTrump")]);
//			m_pItem_Right = NULL;
//			return;
//		}
//	}
//	else if( static_cast<Equipment*>(m_pItem_Right)->IsNotIdetified())
//	{
//		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Trump_NotJianDing")]);
//		m_pItem_Right = NULL;
//		return;
//	}
//	else
//	{
//		m_pAssisGrownTrump->RefreshItem( m_pItem_Right->GetItemTypeID(), 0, m_pItem_Right->GetItemQuality() );
//	}
//
//	UpdateUI();
//}

DWORD TrumpRebornFrame::NS_FabaoWuxingReversal(tagNS_FabaoWuxingReversal *pNetCmd,DWORD)
{
    if(!pNetCmd->dwErrorCode)
	{
		//TrainMgr::Inst()->ShowCenterMsgPic(g_StrTable[_T("Trump_ReversalSucess")]);
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Trump_ReversalSucess")]);
		return 0;
	}
	else if (pNetCmd->dwErrorCode)
	{
		switch (pNetCmd->dwErrorCode)
		{
		case E_Fabao_Not_Exist:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_Not_Exist")]);
			break;
		case E_Fabao_TypeMismatch:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_TypeMismatch")]);
			break;
		case E_Fabao_LevelLimmit:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_LevelLimmit")]);
			break;
		case E_Fabao_NotPermitted:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_NotPermitted")]);
			break;
		case E_Fabao_CannotBeAider:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_CannotBeAider")]);
			break;
		case E_Fabao_SecondeFabaoLevelLower:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_SecondeFabaoLevelLower")]);
			break;
		default:
			break;
		}
	}
	return 0;
}
DWORD TrumpRebornFrame::NS_FabaoWuxingMerging(tagNS_FabaoWuxingMerging *pNetCmd,DWORD)
{
	if(!pNetCmd->dwErrorCode)
	{
		//TrainMgr::Inst()->ShowCenterMsgPic(g_StrTable[_T("Trump_ReversalSucess")]);
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Trump_ReversalSucess")]);
		return 0;
	}
	else if (pNetCmd->dwErrorCode)
	{
		switch (pNetCmd->dwErrorCode)
		{
		case E_Fabao_Not_Exist:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_Not_Exist")]);
			break;
		case E_Fabao_TypeMismatch:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_TypeMismatch")]);
			break;
		case E_Fabao_LevelLimmit:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_LevelLimmit")]);
			break;
		case E_Fabao_NotPermitted:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_NotPermitted")]);
			break;
		case E_Fabao_CannotBeAider:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_CannotBeAider")]);
			break;
		case E_Fabao_SecondeFabaoLevelLower:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_SecondeFabaoLevelLower")]);
			break;
		default:
			break;
		}
	}
	return 0;
}

BOOL TrumpRebornFrame::CheckMainTrumpLevel()
{
	m_pTrump = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	if( P_VALID(m_pTrump) )
	{
		const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
		if(!P_VALID(pTrumpSpe))
			return false;

		if(pTrumpSpe->n16Stage <40)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Fabao_LevelNotEnoughToConversion")]);
			return false;
		}
	}
	return true;

}

DWORD TrumpRebornFrame::OnEventItemEvent_Move2Reborn(tagItemPosChangeExEvent* pGameEvent)
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;
	PutAssisTrumpLeft(INT16(pGameEvent->n16IndexSrc));
	return 0;
}
DWORD TrumpRebornFrame::UseTrumpReversalCheck(tagMsgBoxEvent* pGameEvent)
{
	if(MBF_OK == pGameEvent->eResult)
	{

		if(P_VALID(m_pTrump)&&P_VALID(m_pItem_Left))
		{
			tagNC_FabaoWuxingReversal cmd;
			cmd.n64MainFabaoID = m_pTrump->GetItemId();
			cmd.n64SecondaryFabaoID = m_pItem_Left->GetItemId();
			TObjRef<NetSession>()->Send(&cmd);
			m_pItem_Left = NULL;
			m_pAssisReverseTrump->SetPicFileName(TRUMP_DIBAN);
			UpdateUI();
		}
	}
	return 0;
}
//DWORD TrumpRebornFrame::UseTrumpMergingCheck(tagMsgBoxEvent* pGameEvent)
//{
//	if(MBF_OK == pGameEvent->eResult)
//	{
//		
//		if(P_VALID(m_pTrump)&&P_VALID(m_pItem_Right))
//		{
//			tagNC_FabaoWuxingMerging cmd;
//			cmd.n64MainFabaoID = m_pTrump->GetItemId();
//			cmd.n64SecondaryFabaoID = m_pItem_Right->GetItemId();
//			TObjRef<NetSession>()->Send(&cmd);
//			m_pItem_Right = NULL;
//			m_pAssisGrownTrump->SetPicFileName(TRUMP_DIBAN);
//			UpdateUI();
//		}
//	}
//	return 0;
//}

DWORD TrumpRebornFrame::NetItemRemove(tagNS_ItemRemove *pNetCmd,DWORD)
{
	if (P_VALID(m_pItem_Left))
	{
		INT64 n64ID = m_pItem_Left->GetData()->n64Serial;
		Item *pItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
		if(!P_VALID(pItem))
		{
			m_pItem_Left = NULL;
			m_pStcLikely->SetText(_T(""));// 相似率
			m_pAssisReverseTrumpName->SetText(_T(""));
			m_pAssisReverseTrumpLevel->SetText(_T(""));
			m_pAssisReverseTrump->RefreshItem();
		}
	}
	//if (P_VALID(m_pItem_Right))
	//{
	//	INT64 n64ID = m_pItem_Right->GetData()->n64Serial;
	//	Item *pItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
	//	if(!P_VALID(pItem))
	//	{
	//		m_pItem_Right = NULL;
	//		m_pAssisGrownTrumpName->SetText(_T(""));
	//		m_pAssisGrownTrumpLevel->SetText(_T(""));
	//		m_pAssisGrownTrump->RefreshItem();
	//	}
	//}
	return 0;
}