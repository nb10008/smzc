#include "StdAfx.h"
#include "TrumpMixBeadFrame.h"
#include "ItemMgr.h"
#include "ItemProtoData.h"
#include "ItemEvent.h"
#include "Item.h"
#include "CombatSysUtil.h"
#include "ItemButton.h"
#include "ItemFrame.h"
#include "..\WorldDefine\msg_compose.h"
#include "ConsolidateData.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "..\WorldDefine\msg_loot.h"

TrumpMixBeadFrame::TrumpMixBeadFrame():m_Trunk(this),
m_pWnd(NULL),
m_pWndCaption(NULL),
m_pIconTrump(NULL),
m_pStcTrumpName(NULL),
m_pStcTrumpLevel(NULL),
m_pIconLingZhu(NULL),
m_pProTime(NULL),
m_pButMix(NULL),
m_pButCancle(NULL),
m_pBtnQuit(NULL),
m_pStcMixRule(NULL),
m_pTrump(NULL),
m_bProcessing(false),
m_byMatchNum(0),
m_dwTypeID(GT_INVALID)
{
	for( int i = 0; i < EWX_End; i++ )
		m_pIconLingCao[i] = NULL;
}

TrumpMixBeadFrame::~TrumpMixBeadFrame()
{

}

BOOL TrumpMixBeadFrame::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if( !GameFrame::Init( pMgr, pFather, dwParam ) )
		return false;

	m_pMgr->RegisterEventHandle(_T("TrumpBind"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpMixBeadFrame::OnTrumpBind));
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2MixBead"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpMixBeadFrame::OnEventMixBead));
	m_pCmdMgr->Register("NS_FabaoInlay",							(NETMSGPROC)m_Trunk.sfp2(&TrumpMixBeadFrame::NetTrumpMixBead),			_T("NS_FabaoInlay"));
	m_pCmdMgr->Register("NS_RolePutDownItem",						(NETMSGPROC)m_Trunk.sfp2(&TrumpMixBeadFrame::NetRolePutDownItem),		_T("NS_RolePutDownItem"));
	return true;
}

BOOL TrumpMixBeadFrame::Destroy()
{
	GameFrame::Destroy();
	LockItem(true);
	if( P_VALID(m_pWnd) )
		m_pGUI->DestroyWnd(m_pWnd);
	m_pMgr->UnRegisterEventHandler(_T("TrumpBind"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpMixBeadFrame::OnTrumpBind));
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2MixBead"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpMixBeadFrame::OnEventMixBead));
	m_pCmdMgr->UnRegister("NS_FabaoInlay",							(NETMSGPROC)m_Trunk.sfp2(&TrumpMixBeadFrame::NetTrumpMixBead));
	m_pCmdMgr->UnRegister("NS_RolePutDownItem",						(NETMSGPROC)m_Trunk.sfp2(&TrumpMixBeadFrame::NetRolePutDownItem));
	return true;
}

VOID TrumpMixBeadFrame::Update()
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
			m_bProcessing = false;
			tagNC_FabaoInlay msg;
			msg.n64FabaoID = m_pTrump->GetData()->n64Serial;
			msg.n64Bead = m_pItem->GetData()->n64Serial;
			m_pSession->Send(&msg);
		}
	}
}

BOOL TrumpMixBeadFrame::EscCancel()
{
	if(m_pWnd->IsInvisible())
		return FALSE;
	else if( m_bProcessing )
	{
		m_bProcessing = false;
		m_pProTime->SetValue(0, true);
		return TRUE;
	}
	m_pWnd->SetInvisible(TRUE);
	ResetUI();
	SetStrategy(EIST_ItemPocket);
	return TRUE;
}

BOOL TrumpMixBeadFrame::ReloadUI()
{
	tstring strPath = g_strLocalPath + _T("\\ui\\fairy_soul_fuse.xml");
	XmlElement element;
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "soul_fuse_win\\fuse_pic\\fairyback\\fairypic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "soul_fuse_win\\fuse_pic\\itemback5\\item", "ClassName", "ItemButton");
	for (int i = 0; i < EWX_End; ++i)
	{
		char cbuff[128];
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),
			"soul_fuse_win\\fuse_pic\\itemback%d\\item", i);
		m_pGUI->ChangeXml(&element, cbuff, "ClassName", "ItemButton");
	}
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pWnd->FlipToTop();

	m_pWndCaption = (GUIWnd*)m_pWnd->GetChild(_T("fuse_pic"));
	m_pIconTrump = (ItemButton*)m_pWndCaption->GetChild(_T("fairyback\\fairypic"));
	m_pStcTrumpName = (GUIStatic*)m_pWndCaption->GetChild(_T("name"));
	m_pStcTrumpLevel = (GUIStatic*)m_pWndCaption->GetChild(_T("grade"));
	m_pIconLingZhu = (ItemButton*)m_pWndCaption->GetChild(_T("itemback5\\item"));
	m_pProTime = (GUIProgress*)m_pWndCaption->GetChild(_T("progpic\\new"));
	m_pButMix = (GUIButton*)m_pWndCaption->GetChild(_T("confluentbutt"));
	m_pButMix->SetEnable(false);
	m_pButCancle = (GUIButton*)m_pWndCaption->GetChild(_T("canclebutt"));
	m_pBtnQuit = (GUIButton*)m_pWndCaption->GetChild(_T("closebutt"));
	m_pStcMixRule = (GUIStaticEx*)m_pWndCaption->GetChild(_T("tt_conpic\\tcontent"));

	m_pStcMixRule->SetText(g_StrTable[_T("MixBeadRule")]);
	tagPoint size = m_pStcMixRule->GetSize();
	if( m_pStcMixRule->GetTextTotalHeight() > size.y )
	{
		size.y = m_pStcMixRule->GetTextTotalHeight();
		m_pStcMixRule->SetResizable(TRUE);
		m_pStcMixRule->Resize( size );
		m_pStcMixRule->SetResizable(FALSE);
	}

	m_pStcMixRule->SetRefresh(TRUE);

	for( int i = 0; i < EWX_End; i++ )
	{
		TCHAR szName[X_SHORT_NAME] = {0};
		_sntprintf( szName, X_SHORT_NAME, _T("itemback%d\\item"), i);
		m_pIconLingCao[i] = (ItemButton*)m_pWndCaption->GetChild(szName);
	}
	m_pWnd->SetInvisible(TRUE);

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&TrumpMixBeadFrame::EventHandler) );
	return true;
}

DWORD TrumpMixBeadFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pButCancle || pWnd == m_pBtnQuit )
				ShowUI(FALSE);
			else if( pWnd == m_pButMix )
			{
				if( m_pTrump->GetData()->byBind == 0 )
				{
					if( P_VALID(GetObj("TrumpBind")) )
						KillObj("TrumpBind");
					CreateObj( "TrumpBind", "MsgBox" );
					TObjRef<MsgBox>("TrumpBind")->Init( _T(""), g_StrTable[_T("TrumpBind")], _T("TrumpBind") );
				}
				else
				{
					m_pProTime->SetSpeed(100);
					m_pProTime->SetValue(0, true);
					m_pProTime->SetMaxValue(3.0f);
					m_pProTime->SetRefresh();
					m_pButMix->SetEnable(false);
					m_bProcessing = true;
				}
			}
		}
		break;
	case EGUISCB_Drop:
		if( pWnd == m_pIconLingZhu )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutLingZhu((INT16)LOWORD(pEvent->dwParam2));
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for( int i = 0; i < EWX_End; i ++ )
			{
				if( pWnd == m_pIconLingCao[i] && P_VALID(m_pTrump) )
				{
					const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
					if( P_VALID(pTrumpSpe) )
					{
						DWORD dwTypeID = pTrumpSpe->dwWuxingBeadID[i];
						if( dwTypeID != GT_INVALID && dwTypeID != 0 )
						{
							tstring strTip = ToolTipCreator::Inst()->GetItemTips(dwTypeID);
							ShowTip( m_pIconLingCao[i], strTip.c_str() );
						}
						else if( dwTypeID == GT_INVALID )
						{
							INT nLevel = 0;
							switch(i)
							{
							case 0:
								nLevel = 10;
								break;
							case 1:
								nLevel = 20;
								break;
							case 2:
								nLevel = 30;
								break;
							case 3:
								nLevel = 40;
								break;
							case 4:
								nLevel = 60;
								break;
							}
							TCHAR szTemp[X_LONG_NAME] = {0};
							_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("OpenLingCao")], nLevel);
							ShowTip( m_pIconLingCao[i], szTemp );
						}
						else if( dwTypeID == 0 )
						{
							ShowTip( m_pIconLingCao[i], g_StrTable[_T("CanHaveLingZhu")] );
						}
					}
				}
			}
			if( pWnd == m_pIconLingZhu && P_VALID(m_pItem) )
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItem).c_str());
			if( pWnd == m_pIconTrump && P_VALID(m_pTrump) )
				ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(m_pTrump).c_str() );
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	}

	return FALSE;
}

VOID TrumpMixBeadFrame::ShowUI( bool bFlag )
{
	if(bFlag)
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
		}
		LockItem(true);
		m_pWnd->SetInvisible(!bFlag);
		ResetUI();
		SetStrategy(EIST_ItemPocket);
	}
}

DWORD TrumpMixBeadFrame::OnEventMixBead( tagItemPosChangeExEvent* pGameEvent )
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;
	PutLingZhu(INT16(pGameEvent->n16IndexSrc));
	return 0;
}

INT TrumpMixBeadFrame::GetLingCaoNum()
{
	if( !P_VALID(m_pTrump) )
		return 0;

	const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
	if( !P_VALID(pTrumpSpe) )
		return 0;
	if( pTrumpSpe->n16Stage >= 60 )
		return 5;
	else if( pTrumpSpe->n16Stage >= 40 )
		return 4;
	else if( pTrumpSpe->n16Stage >= 30 )
		return 3;
	else if( pTrumpSpe->n16Stage >= 20 )
		return 2;
	else if( pTrumpSpe->n16Stage >= 10 )
		return 1;
	else
		return 0;
}

VOID TrumpMixBeadFrame::UpdateUI()
{
	ResetUI();
	m_pTrump = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	if( P_VALID(m_pTrump) )
	{
		DWORD dwTypeID = m_pTrump->GetItemTypeID();
		BYTE byQuality = m_pTrump->GetItemQuality();
		m_pIconTrump->RefreshItem( dwTypeID, 0, byQuality );
		tstring strName = m_pTrump->GetDisplay()->szName;
		m_pStcTrumpName->SetText(strName.c_str());

		const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
		m_byMatchNum = pTrumpSpe->bySlotLingzhuMatch;
		const tagFabaoProto *pTrumpProto = (tagFabaoProto*)m_pTrump->GetProto();
		if(!P_VALID(pTrumpProto) && !(pTrumpSpe))
			return;

		TCHAR szTemp[X_LONG_NAME] = {0};
		_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("Skill_Level")], pTrumpSpe->n16Stage );
		m_pStcTrumpLevel->SetText(szTemp);

		INT nNum = GetLingCaoNum();
		for( int i = 0; i < EWX_End; i++ )
		{
			if( i < nNum )
			{
				dwTypeID = pTrumpSpe->dwWuxingBeadID[i];
				const tagItemProto *pProto  = ItemProtoData::Inst()->FindItemProto(dwTypeID);
				if( P_VALID(pProto) )
				{
					dwTypeID = pProto->dwTypeID;
					byQuality = pProto->byQuality;
					m_pIconLingCao[i]->RefreshItem( dwTypeID, 0, byQuality );
					m_pIconLingCao[i]->SetRefresh(true);
				}
				else
					m_pIconLingCao[i]->RefreshItem();
			}
		}
	}
}

void TrumpMixBeadFrame::LockItem( bool bOperable )
{
	if( P_VALID(m_pItem) )
		m_pItem->SetOperable(bOperable);
}

VOID TrumpMixBeadFrame::ResetUI()
{
	m_pIconTrump->RefreshItem();
	m_pIconLingZhu->RefreshItem();
	for( int i = 0; i < EWX_End; i++ )
		m_pIconLingCao[i]->RefreshItem(0);
	m_pStcTrumpName->SetText(_T(""));
	m_pStcTrumpLevel->SetText(_T(""));
	m_pProTime->SetValue(0.0f, true);
	m_pButMix->SetEnable(false);
	m_byMatchNum = 0;
	m_dwTypeID = GT_INVALID;
	m_pItem = NULL;
}

VOID TrumpMixBeadFrame::PutLingZhu(INT nPos )
{
	if(m_bProcessing)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("MixingBead")] );
		return;
	}
	LockItem(true);
	Item *pItem = ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(pItem))
		return;

	m_dwTypeID = pItem->GetData()->dwTypeID;
	const tagItemProto* pItemProto = pItem->GetProto();
	const tagFabaoProto *pTrumpProto = (tagFabaoProto*)m_pTrump->GetProto();
	const tagFabaoSpec	*pTrumpSpe = m_pTrump->GetFaBaoSpe();
	if( pItemProto->eSpecFunc != EISF_LingZhu )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyLingZhu")]);
		return;
	}
	else if( pItemProto->nSpecFuncVal2 != pTrumpProto->eFarbaoType && pItemProto->nSpecFuncVal2 != GT_INVALID )
	{
		TCHAR szDes[X_LONG_NAME] = {0};
		_sntprintf( szDes, X_LONG_NAME, _T("OnlySuitTrump_%d"), pItemProto->nSpecFuncVal2 );
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[szDes]);
		return;
	}
	else if( pItemProto->nSpecFuncVal1 > pTrumpSpe->n16Stage )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TrumpLevelTooLow")]);
		return;
	}
	
	m_pItem = pItem;
	m_pIconLingZhu->RefreshItem( m_pItem->GetItemTypeID(), 0, m_pItem->GetItemQuality() );
	m_pButMix->SetEnable(true);
	LockItem(false);
}

DWORD TrumpMixBeadFrame::NetTrumpMixBead( tagNS_FabaoInlay *pNetCmd, DWORD )
{
	LockItem(true);
	if( pNetCmd->dwErrorCode == E_Success )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TrumpMixBeadSuccess")]);
		const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
		if( !P_VALID(pTrumpSpe) )
			return 0;
		if( m_byMatchNum < pTrumpSpe->bySlotLingzhuMatch )
		{
			//if( P_VALID(GetObj("NewTrumpAward")) )
			//	KillObj("NewTrumpAward");
			//CreateObj( "NewTrumpAward", "MsgBoxEx" );
			//TObjRef<MsgBoxEx>("NewTrumpAward")->Init( _T(""), g_StrTable[_T("NewTrumpAward")],NULL,MBF_OK,FALSE, TRUE );
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NewTrumpAward")]);
		}
		if ( pTrumpSpe->bySlotLingzhuMatch >= 3 && m_byMatchNum < pTrumpSpe->bySlotLingzhuMatch )
		{
			/*if( P_VALID(GetObj("ActiveSpecialTrumpSkill")) )
				KillObj("ActiveSpecialTrumpSkill");
			CreateObj( "ActiveSpecialTrumpSkill", "MsgBoxEx" );
			TObjRef<MsgBoxEx>("ActiveSpecialTrumpSkill")->Init( _T(""), g_StrTable[_T("ActiveSpecialTrumpSkill")],NULL,MBF_OK,FALSE, TRUE  );*/
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ActiveSpecialTrumpSkill")]);
		}
	}
	else
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TrumpMixBeadFailed")]);
	UpdateUI();
	return 0;
}

DWORD TrumpMixBeadFrame::OnTrumpBind(tagMsgBoxEvent* pGameEvent)
{
	if( pGameEvent->eResult == MBF_OK )
	{
		m_pProTime->SetSpeed(100);
		m_pProTime->SetValue(0, true);
		m_pProTime->SetMaxValue(3.0f);
		m_pProTime->SetRefresh();
		m_bProcessing = true;
		m_pButMix->SetEnable(false);
	}
	return 0;
}

DWORD TrumpMixBeadFrame::NetRolePutDownItem( tagNS_RolePutDownItem *pNetCmd, DWORD )
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
