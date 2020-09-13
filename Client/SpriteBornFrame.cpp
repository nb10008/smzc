#include "StdAfx.h"
#include "SpriteBornFrame.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "ItemFrame.h"
#include "ItemEvent.h"
#include "ItemMgr.h"
#include "ItemButton.h"
#include "../WorldDefine/currency_define.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "CombatSysUtil.h"

extern const TCHAR* szBackPic;

SpriteBornFrame::SpriteBornFrame(void):m_Trunk(this)
,m_dwNPCID(GT_INVALID)
,m_escapeTime(0)
,m_N64ItemID(0)
{
}

SpriteBornFrame::~SpriteBornFrame(void)
{
}

BOOL SpriteBornFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2FairyBorn"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SpriteBornFrame::OnEventMove2Born));
	m_pCmdMgr->Register( "NS_FairyBorn",		(NETMSGPROC)m_Trunk.sfp2(&SpriteBornFrame::OnNS_FairyBorn),		_T("NS_FairyBorn"));
	SetStrategy(EIST_FairyBorn);

	return TRUE;
}
BOOL SpriteBornFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	if( P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;
	}

	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2FairyBorn"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SpriteBornFrame::OnEventMove2Born));
	
	m_pCmdMgr->UnRegister( "NS_FairyBorn",							(NETMSGPROC)m_Trunk.sfp2(&SpriteBornFrame::OnNS_FairyBorn) );

	SetStrategy(EIST_ItemPocket);

	return bRet;
}
BOOL SpriteBornFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\fairy_breed.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml( &element, "fairybreed\\breed_back\\fairyback\\fairypic", "ClassName", "ItemButton");
	//m_pGUI->ChangeXml( &element, "fairybreed\\breed_back\\fairyback", "ClassName", "IconStatic");
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}
	
	m_pIcItem = (ItemButton*)m_pWnd->GetChild(_T("breed_back\\fairyback\\fairypic"));		
//	m_pIcItemBack = (IconStatic*)m_pWnd->GetChild(_T("breed_back\\fairyback\\fairypic"));		
	m_pStGold = (GUIStatic*)m_pWnd->GetChild(_T("breed_back\\goldnum"));		
	m_pStSilver = (GUIStatic*)m_pWnd->GetChild(_T("breed_back\\silvernum"));	

	m_pBnStart = (GUIButton*)m_pWnd->GetChild(_T("breed_back\\startbutt"));
	m_pBnClose = (GUIButton*)m_pWnd->GetChild(_T("breed_back\\leavebutt"));
	m_pBnExit = (GUIButton*)m_pWnd->GetChild(_T("breed_back\\close"));

	m_pLstProduct = (GUIListBox*)m_pWnd->GetChild(_T("breed_back\\listback\\breedlist"));
	m_pLstProduct->SetColNum(3, 100);
	m_pLstProduct->SetColWidth(0, 50);
	m_pLstProduct->SetColWidth(2, 100);

	m_pIcItem->RefreshItem();

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&SpriteBornFrame::EventHandler));

	return TRUE;
}
BOOL SpriteBornFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return true;
}

VOID SpriteBornFrame::Update()
{
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )		//Ã¿¸ô1Ãë¼ì²â¾àÀëÄ¿±êNPCµÄ¾àÀë
	{
		m_escapeTime = 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{
			Destroy();
			return;
		}
		if (RoleMgr::Inst()->IsOutValidDist( m_dwNPCID ) )
		{
			m_pFrameMgr->AddToDestroyList(this);
		}
	}
}
BOOL SpriteBornFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pBnClose ||pWnd == m_pBnExit )
			{
				m_pFrameMgr->AddToDestroyList(this);
			}
			else if (pWnd == m_pBnStart)
			{
				SendNetMessage();
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if (pWnd == m_pIcItem)
			{
				Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_N64ItemID);
				if (P_VALID(pItem))
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
				}
			}
		}
		break;
	case EGUISCB_Drop:
	case EGUISCB_DropCheck:
		{
			if( EDT_Item != (EDragType)pEvent->dwParam1 )
				break;
			if (m_pIcItem == pWnd)
			{
				INT nPos = (INT16)LOWORD(pEvent->dwParam2);
				Item *pItem = (Item*)ItemMgr::Inst()->GetPocketItem(nPos);
				if(!P_VALID(pItem))
					break;
				PutInSprite(pItem->GetItemId());
			}
		}
		break;
	default:
		break;
	}
	return TRUE;
}

VOID SpriteBornFrame::SendNetMessage()
{
	tagNC_FairyBorn cmd;
	cmd.dwNPCID = m_dwNPCID;
	cmd.n64FairyID = m_N64ItemID;
	m_pSession->Send(&cmd);
}


DWORD SpriteBornFrame::OnEventMove2Born(tagItemPosChangeExEvent* pGameEvent)
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;

	Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexSrc);
	if(!P_VALID(pItem))		return 0;

	PutInSprite(pItem->GetItemId());
	return 0;
}

VOID SpriteBornFrame::PutInSprite(INT64 n64ID)
{
	m_N64ItemID = n64ID;
	Item* pItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
	if(!P_VALID(pItem))		return;
	const tagItemProto* pItemProto = pItem->GetProto();
	if(!P_VALID(pItemProto))		return;
	// ÅÐ¶ÏÊÇ·ñÊÇÑý¾«
	if(MIsEquipment(pItemProto->dwTypeID) &&  pItemProto->eType == EIT_FaBao )
	{
		const tagFabaoProto *pTrumpProto = (tagFabaoProto*)pItemProto;
		const tagFabaoSpec	*pTrumpSpe = ((Equipment*)pItem)->GetFaBaoSpe();
		const tagEquipSpec *pEquipSpec = ((Equipment*)pItem)->GetEquip();
		if (((Equipment*)pItem)->GetItemQuality() < EIQ_Orange)	return;

		m_pIcItem->RefreshItem(pItemProto->dwTypeID,0,pEquipSpec->byQuality);

		INT64 n64CostMoney = 10000 * pTrumpSpe->n16NativeIntelligence / 10 * pItemProto->byLevel / 10;
		switch ( ((Equipment*)pItem)->GetItemQuality())
		{
		case EIQ_Orange:
			break;
		case EIQ_Purple:
			n64CostMoney *= 2;
			break;
		case EIQ_Pink:
			n64CostMoney *= 4;
			break;
		}

		TCHAR szBuffer[X_SHORT_NAME] = {0};
		int nGold = MSilver2DBGold(n64CostMoney);
		int nSilver = MSilver2DBSilver(n64CostMoney);
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nGold);
		m_pStGold->SetText(szBuffer);
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nSilver);
		m_pStSilver->SetText(szBuffer);

		RefreshListBox(pTrumpSpe->nBirthValue);
	}

}

VOID SpriteBornFrame::ClearInfo()
{
	m_N64ItemID = 0;
	m_pIcItem->RefreshItem();
	//m_pIcItemBack->SetPic(szBackPic);
	m_pLstProduct->Clear();
	m_pStGold->SetText(_T(""));
	m_pStSilver->SetText(_T(""));
}


VOID SpriteBornFrame::RefreshListBox(int nValue)
{
	m_pLstProduct->Clear();
	int nColNum =1;
	if (nValue <= 1099 )
	{
		m_pLstProduct->SetText( 0, nColNum, g_StrTable[_T("SpriteBorn_WeakSprite")] );
		m_pLstProduct->SetText( 1, nColNum, g_StrTable[_T("SpriteBorn_WeakHunhe")] );
	} 
	else if(nValue <= 1199)
	{
		m_pLstProduct->SetText( 0, nColNum, g_StrTable[_T("SpriteBorn_WeakSprite")] );
		m_pLstProduct->SetText( 1, nColNum, g_StrTable[_T("SpriteBorn_WeakHunhe")] );
		m_pLstProduct->SetText( 2, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_4")] );
	}
	else if(nValue <= 1299)
	{
		m_pLstProduct->SetText( 0, nColNum, g_StrTable[_T("SpriteBorn_StrongSprite")] );
		m_pLstProduct->SetText( 1, nColNum, g_StrTable[_T("SpriteBorn_WeakSprite")] );
		m_pLstProduct->SetText( 2, nColNum, g_StrTable[_T("SpriteBorn_SpriteHunhe")] );
		m_pLstProduct->SetText( 3, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_4")] );
	}
	else if(nValue <= 1399)
	{
		m_pLstProduct->SetText( 0, nColNum, g_StrTable[_T("SpriteBorn_StrongSprite")] );
		m_pLstProduct->SetText( 1, nColNum, g_StrTable[_T("SpriteBorn_WeakSprite")] );
		m_pLstProduct->SetText( 2, nColNum, g_StrTable[_T("SpriteBorn_SpriteHunhe")] );
		m_pLstProduct->SetText( 3, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_4")] );
	}
	else if(nValue <= 1499)
	{
		m_pLstProduct->SetText( 0, nColNum, g_StrTable[_T("SpriteBorn_BianyiSprite")] );
		m_pLstProduct->SetText( 1, nColNum, g_StrTable[_T("SpriteBorn_StrongSprite")] );
		m_pLstProduct->SetText( 2, nColNum, g_StrTable[_T("SpriteBorn_WeakSprite")] );
		m_pLstProduct->SetText( 3, nColNum, g_StrTable[_T("SpriteBorn_SpriteHunhe")] );
		m_pLstProduct->SetText( 4, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_4")] );
	}
	else if(nValue <= 1599)
	{
		m_pLstProduct->SetText( 0, nColNum, g_StrTable[_T("SpriteBorn_BianyiSprite")] );
		m_pLstProduct->SetText( 1, nColNum, g_StrTable[_T("SpriteBorn_StrongSprite")] );
		m_pLstProduct->SetText( 2, nColNum, g_StrTable[_T("SpriteBorn_WeakSprite")] );
		m_pLstProduct->SetText( 3, nColNum, g_StrTable[_T("SpriteBorn_StrongHunhe")] );
		m_pLstProduct->SetText( 4, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_4")] );
		m_pLstProduct->SetText( 5, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_5")] );
	}
	else if(nValue <= 1699)
	{
		m_pLstProduct->SetText( 0, nColNum, g_StrTable[_T("SpriteBorn_BianyiSprite")] );
		m_pLstProduct->SetText( 1, nColNum, g_StrTable[_T("SpriteBorn_StrongSprite")] );
		m_pLstProduct->SetText( 2, nColNum, g_StrTable[_T("SpriteBorn_WeakSprite")] );
		m_pLstProduct->SetText( 3, nColNum, g_StrTable[_T("SpriteBorn_StrongHunhe")] );
		m_pLstProduct->SetText( 4, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_4")] );
		m_pLstProduct->SetText( 5, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_5")] );
	}
	else if(nValue <= 1799)
	{
		m_pLstProduct->SetText( 0, nColNum, g_StrTable[_T("SpriteBorn_BianyiSprite")] );
		m_pLstProduct->SetText( 1, nColNum, g_StrTable[_T("SpriteBorn_StrongSprite")] );
		m_pLstProduct->SetText( 2, nColNum, g_StrTable[_T("SpriteBorn_WeakSprite")] );
		m_pLstProduct->SetText( 3, nColNum, g_StrTable[_T("SpriteBorn_StrongHunhe")] );
		m_pLstProduct->SetText( 4, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_5")] );
	}
	else if(nValue <= 1899)
	{
		m_pLstProduct->SetText( 0, nColNum, g_StrTable[_T("SpriteBorn_BianyiSprite")] );
		m_pLstProduct->SetText( 1, nColNum, g_StrTable[_T("SpriteBorn_StrongSprite")] );
		m_pLstProduct->SetText( 2, nColNum, g_StrTable[_T("SpriteBorn_StrongHunhe")] );
		m_pLstProduct->SetText( 3, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_5")] );
		m_pLstProduct->SetText( 4, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_6")] );
	}
	else
	{
		m_pLstProduct->SetText( 0, nColNum, g_StrTable[_T("SpriteBorn_BianyiSprite")] );
		m_pLstProduct->SetText( 1, nColNum, g_StrTable[_T("SpriteBorn_StrongSprite")] );
		m_pLstProduct->SetText( 2, nColNum, g_StrTable[_T("SpriteBorn_StrongHunhe")] );
		m_pLstProduct->SetText( 3, nColNum, g_StrTable[_T("SpriteBorn_BlueRock_6")] );
	}
}

DWORD SpriteBornFrame::OnNS_FairyBorn(tagNS_FairyBorn* pNetCmd,DWORD)
{
	if(pNetCmd->dwErrCode == E_Success)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpriteBorn_Success")]);
		m_pFrameMgr->AddToDestroyList(this);
	}
	else
	{
		switch (pNetCmd->dwErrCode)
		{
		case E_FabaoMating_RoleNotOnline:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_RoleNotOnline")]);
			break;
		case E_FabaoMating_QltyLow:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_QltyLow")]);
			break;
		case E_FabaoMating_QualLow:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_QualLow")]);
			break;
		case E_FabaoMating_CannotMating:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_CannotMating")]);
			break;
		case E_FabaoMating_BirthTimesLow:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_BirthTimesLow")]);
			break;
		case E_FabaoMating_LevelLow:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_LevelLow")]);
			break;
		case E_FabaoMating_OtherBagFull:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_OtherBagFull")]);
			break;
		case E_FabaoMating_NotInGiveBirth:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_NotInGiveBirth")]);
			break;
		case E_FabaoMating_CannotPutIn_ErrStatus:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_CannotPutIn_ErrStatus")]);
			break;
		case E_Bag_NotEnoughSpace:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoBornMating_BagFull")]);
			break;
		case E_BagSilver_NotEnough:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoBornMating_silvernotenough")]);
			break;
		}
	}
	return 0;
}

