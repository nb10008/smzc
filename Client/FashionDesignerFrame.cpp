#include "StdAfx.h"
#include "Item.h"
#include "ItemMgr.h"
#include "RoleMgr.h"
#include "ItemEvent.h"
#include "ItemFrame.h"
#include "IconStatic.h"
#include "LocalPlayer.h"
#include "CurrencyMgr.h"
#include "ProduceData.h"
#include "ToolTipFrame.h"
#include "ItemProtoData.h"
#include "CombatSysUtil.h"
#include "ToolTipCreator.h"
#include "FashionDesignerFrame.h"
#include "..\WorldDefine\msg_fashion.h"
#include "..\WorldDefine\currency_define.h"
#include "..\WorldDefine\msg_compose.h"
#include "..\WorldDefine\creature_define.h"

const TCHAR* szItemPicInit = _T("data\\ui\\common\\L_diban-b.bmp");

FashionDesignerFrame::FashionDesignerFrame(void):
m_Trunk(this),
m_pWnd(NULL),
m_bProcessing(false),
m_dwEscapeTime(GT_INVALID),
m_dwNPCID(GT_INVALID),
m_eRanType(EADVCOMT_NULL),
m_n64Designer(GT_INVALID)
{
	for(int i = 0; i < MAX_RANDOMPRODUCE_STUFF; i++)
	{
		m_dwStuff[i] = GT_INVALID;
		m_n16StuffNum[i] = GT_INVALID;
		m_n64Stuff[i] = GT_INVALID;
		m_bHaveItem[i] = false;
	}
}

FashionDesignerFrame::~FashionDesignerFrame()
{

}

BOOL FashionDesignerFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{
	if( !GameFrame::Init( pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register("NS_AdvancedCompose", (NETMSGPROC)m_Trunk.sfp2(&FashionDesignerFrame::NetProduceFashion), _T("NS_AdvancedCompose"));
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2FashionDesigner"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&FashionDesignerFrame::OnEventPutItem));
	return TRUE;
}

BOOL FashionDesignerFrame::Destroy()
{
	SetStrategy(EIST_ItemPocket);
	GameFrame::Destroy();
	if(P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	m_pCmdMgr->UnRegister("NS_AdvancedCompose", (NETMSGPROC)m_Trunk.sfp2(&FashionDesignerFrame::NetProduceFashion));
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2FashionDesigner"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FashionDesignerFrame::OnEventPutItem));
	LockItem(m_n64Designer, false);
	for( int i = 0; i < MAX_RANDOMPRODUCE_STUFF; i++)
		LockItem(m_n64Stuff[i], false);
	return TRUE;
}
BOOL FashionDesignerFrame::EscCancel()
{
	if(m_pWnd->IsInvisible() )
		return FALSE;
	else if( m_bProcessing )
	{
		m_bProcessing = false;
		m_pProTime->SetValue(0, true);
		m_pBtnStart->SetEnable(true);
		return TRUE;
	}
	m_pMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL FashionDesignerFrame::ReloadUI()
{
	XmlElement e;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\fashion.xml");
	if( !m_pGUI->LoadXml( &e, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &e, "fashion\\fashionback\\designback\\back\\icon", "ClassName", "IconStatic" );
	m_pGUI->ChangeXml( &e, "fashion\\fashionback\\materialback0\\back\\icon", "ClassName", "IconStatic" );
	m_pGUI->ChangeXml( &e, "fashion\\fashionback\\materialback1\\back\\icon", "ClassName", "IconStatic" );
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &e );
	if(!P_VALID(m_pWnd))
		return FALSE;

	m_pStcDesc = (GUIStatic*)m_pWnd->GetChild(_T("fashionback\\designback\\word"));
	m_pStcNPCBack = (GUIPatch*)m_pWnd->GetChild(_T("fashionback"));
	m_pIconDesigner = (IconStatic*)m_pWnd->GetChild(_T("fashionback\\designback\\back\\icon"));
	m_pIconDesigner->SetPic(szItemPicInit);
	TCHAR szTemp[X_LONG_NAME] = {0};
	for( int i = 0; i < MAX_RANDOMPRODUCE_STUFF; i++)
	{
		_sntprintf(szTemp, X_LONG_NAME, _T("fashionback\\materialback%d\\back\\icon"), i);
		m_pIconStuff[i] = (IconStatic*)m_pWnd->GetChild(szTemp);
		m_pIconStuff[i]->SetPic(szItemPicInit);
		_sntprintf(szTemp, X_LONG_NAME, _T("fashionback\\materialback%d\\name"), i);
		m_pStcStuffName[i] = (GUIStatic*)m_pWnd->GetChild(szTemp);
		_sntprintf(szTemp, X_LONG_NAME, _T("fashionback\\materialback%d\\num"), i);
		m_pStcStuffValue[i] = (GUIStaticEx*)m_pWnd->GetChild(szTemp);
	}
	m_pProTime = (GUIProgress*)m_pWnd->GetChild(_T("fashionback\\propic\\pro"));
	m_pStcHuoLi = (GUIStatic*)m_pWnd->GetChild(_T("fashionback\\wordback\\num0"));
	m_pStcGold = (GUIStatic*)m_pWnd->GetChild(_T("fashionback\\wordback\\goldnum"));
	m_pStcSilver = (GUIStatic*)m_pWnd->GetChild(_T("fashionback\\wordback\\silvernum"));
	m_pBtnStart = (GUIButton*)m_pWnd->GetChild(_T("fashionback\\startbutton"));
	m_pBtnStart->SetEnable(false);
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild(_T("fashionback\\close"));
	m_pListFashion = (GUIListBox*)m_pWnd->GetChild(_T("fashionback\\listback\\list"));
	tagPoint ptSizeOldListBox = m_pListFashion->GetSize();
	m_pListFashion->SetColWidth(0, ptSizeOldListBox.x);
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = 0;
	m_pListFashion->SetResizable( TRUE );
	m_pListFashion->Resize( ptSizeListBox );
	m_pListFashion->SetResizable( FALSE );
	m_pListFashion->SetRefresh(TRUE);
	
	// 注册事件处理函数
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&FashionDesignerFrame::EventHandler) );
	SetStrategy(EIST_RandomProduce);
	return TRUE;
}


//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL FashionDesignerFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pBtnClose)
				m_pMgr->AddToDestroyList(this);
			else if( pWnd == m_pBtnStart)
			{
				if(IfCanStart())
				{
					m_pProTime->SetSpeed(100);
					m_pProTime->SetValue(0, true);
					m_pProTime->SetMaxValue(3.0f);
					m_pProTime->SetRefresh();
					m_bProcessing = true;
					m_pBtnStart->SetEnable(false);
				}
			}
		}
		break;
	case EGUISCB_Drop:
		if( pWnd == m_pIconDesigner || 
			pWnd == m_pIconStuff[0] || 
			pWnd == m_pIconStuff[1] )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutItem((INT16)LOWORD(pEvent->dwParam2));
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{	
			if( pWnd == m_pIconDesigner )
			{
				Item *pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64Designer);
				if(P_VALID(pItem))
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
			}
			else if( pWnd == m_pIconStuff[0] && m_dwStuff[0] != GT_INVALID )
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_dwStuff[0]).c_str());
			else if( pWnd == m_pIconStuff[1] && m_dwStuff[1] != GT_INVALID )
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_dwStuff[1]).c_str());
		}
		break;
	}
	return TRUE;
}

VOID FashionDesignerFrame::LockItem( INT64 nID, bool bOperable )
{
	Item *pItem = ItemMgr::Inst()->GetPocketItemByID(nID);
	if(P_VALID(pItem))
		pItem->SetOperable(!bOperable);
}

BOOL FashionDesignerFrame::IfCanStart()
{
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLP))
		return FALSE;
	Item *pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64Designer);
	if(!P_VALID(pItem) || !P_VALID(pItem->GetProto()))
		return FALSE;
	tagRandomProduceProto *pRProducePro = ProduceData::Inst()->FindRandomProduceProto(m_eRanType, pItem->GetProto()->dwTypeID);
	if(!P_VALID(pRProducePro))
		return FALSE;

	INT64 n64MyMoney = CurrencyMgr::Inst()->GetBagSilver();

	if(MSilver2DBGold(n64MyMoney) >= pRProducePro->dwMoneyConsume)
		return TRUE;
	else
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NoEnoughMoneyOrVitality")]);
	return FALSE;
}

VOID FashionDesignerFrame::PutItem( INT16 nPos )
{
	if(m_bProcessing)
		return;
	Item *pItem = ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(pItem))
		return;

	const tagItemProto* pItemProto = pItem->GetProto();
	if( !P_VALID(pItemProto) )
		return;

	const tagItem* pItemData = pItem->GetData();
	if( !P_VALID(pItemData) )
		return;

	tagRandomProduceProto *pRProducePro = ProduceData::Inst()->FindRandomProduceProto(m_eRanType, pItemProto->dwTypeID);
	TCHAR szTemp[X_LONG_NAME] = {0};
	if(pRProducePro == NULL)				//  材料或者没有配方
	{
		if(m_n64Designer == GT_INVALID)
		{
			if(m_eRanType == EADVCOMT_Fashion)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutDesignBook")]);
				return;
			}
			else if(m_eRanType == EADVCOMT_Gem)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutGem2UI")]);
				return;
			}
			else if(m_eRanType == EADVCOMT_GemBluePrint)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutGemBookBluePrint")]);
				return;
			}
			else if(m_eRanType == EADVCOMT_YuanGu)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutYuanGu")]);
				return;
			}
			else if(m_eRanType == EADVCOMT_ShengWen)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutShengWen")]);
				return;
			}
			
		}
		else if(m_dwStuff[0] == pItemProto->dwTypeID || m_dwStuff[1] == pItemProto->dwTypeID)
		{
			INT nIndex = 0;
			INT64 n64Item = GT_INVALID;
			if( m_dwStuff[0] == pItemProto->dwTypeID )
				nIndex = 0;
			else
				nIndex = 1;
			tstring strPic = ItemMgr::Inst()->GetItemIconPath(pItemProto->dwTypeID);
			if(!strPic.empty())
			{
				LockItem(m_n64Stuff[nIndex], false);
				m_pIconStuff[nIndex]->SetPic(strPic);
				if(pItemData->n16Num < m_n16StuffNum[nIndex])
					_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("NoEnoughItem")], pItemData->n16Num, m_n16StuffNum[nIndex]);
				else
				{
					_sntprintf(szTemp, X_LONG_NAME, _T("%d/%d"), pItemData->n16Num, m_n16StuffNum[nIndex]);
					m_bHaveItem[nIndex] = true;
				}
				m_pStcStuffValue[nIndex]->SetText(szTemp);
				m_n64Stuff[nIndex] = pItemData->n64Serial;
				LockItem(m_n64Stuff[nIndex], true);
				m_pIconStuff[nIndex]->SetRefresh(TRUE);
				m_pStcStuffValue[nIndex]->SetRefresh(TRUE);
				if(m_bHaveItem[0] && m_bHaveItem[1])
					m_pBtnStart->SetEnable(TRUE);
			}
		}
		else
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotPutItem2UI")]);
			return;
		}
	}
	else
	{
		tstring strPic = ItemMgr::Inst()->GetItemIconPath(pItemProto->dwTypeID);
		if(!strPic.empty())
		{
			m_pBtnStart->SetEnable(false);
			LockItem(m_n64Designer,false);
			m_pIconDesigner->SetPic(strPic);
			m_n64Designer = pItemData->n64Serial;
			LockItem(m_n64Designer,true);
			for(int i = 0; i < MAX_RANDOMPRODUCE_STUFF; i++)
			{
				LockItem(m_n64Stuff[i],false);
				m_n64Stuff[i] = GT_INVALID;
				m_bHaveItem[i] = false;
				m_pIconStuff[i]->SetPic(szItemPicInit);
				m_dwStuff[i] = pRProducePro->dwStuff[i];
				m_n16StuffNum[i] = pRProducePro->n16StuffNum[i];
				const tagItemProto *pProto = ItemProtoData::Inst()->FindItemProto(m_dwStuff[i]);
				if(P_VALID(pProto))
				{
					m_pStcStuffName[i]->SetText(pProto->pDisplayInfo->szName);
					_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("NoEnoughItem")], 0, m_n16StuffNum[i]);
					m_pStcStuffValue[i]->SetText(szTemp);
				}
				else
				{
					m_pStcStuffName[i]->SetText(_T(""));
					m_pStcStuffValue[i]->SetText(_T(""));
					m_bHaveItem[i] = true;
				}
			}
			_stprintf(szTemp, _T("%d"), pRProducePro->dwMoneyConsume);
			m_pStcGold->SetText(szTemp);
			_stprintf(szTemp, _T("%d"), (pRProducePro->dwMoneyConsume * 10000)%10000);
			m_pStcSilver->SetText(szTemp);
			_stprintf(szTemp, _T("%d"), pRProducePro->dwVitalityConsume);
			m_pStcHuoLi->SetText(szTemp);

			m_pListFashion->Clear();
			INT nRow = 0;
			for(int i = 0; i < MAX_RANDOMPRODUCE_NUM; i++)
			{
				const tagItemProto *pProto = ItemProtoData::Inst()->FindItemProto(pRProducePro->dwItemTypeID[i]);
				if(P_VALID(pProto) && pProto->pDisplayInfo)
				{
					m_pListFashion->SetText(nRow, 0, pProto->pDisplayInfo->szName);
					nRow++;
				}
			}

			tagPoint ptSizeOldListBox = m_pListFashion->GetSize();
			tagPoint ptSizeListBox = ptSizeOldListBox;
			ptSizeListBox.y = m_pListFashion->GetRowHeight() * nRow;
			m_pListFashion->SetResizable( TRUE );
			m_pListFashion->Resize( ptSizeListBox );
			m_pListFashion->SetResizable( FALSE );
			m_pListFashion->SetRefresh(TRUE);
			m_pWnd->SetRefresh(TRUE);
		}
	}
}

DWORD FashionDesignerFrame::OnEventPutItem( tagItemPosChangeExEvent* pGameEvent )
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;
	PutItem(pGameEvent->n16IndexSrc);
	return 0;
}

VOID FashionDesignerFrame::Update()
{
	m_dwEscapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_dwEscapeTime > 1000 )
	{
		m_dwEscapeTime = 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{
			Destroy();
		}
		else if(RoleMgr::Inst()->IsOutValidDist(m_dwNPCID))
		{
			m_pMgr->AddToDestroyList(this);
		}
	}

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
			tagNC_AdvancedCompose m;
			m.n64DesignID = m_n64Designer;
			m.n64Material1 = m_n64Stuff[0];
			m.n64Material2 = m_n64Stuff[1];
			m.dwNPCID = m_dwNPCID;
			m_pSession->Send(&m);
		}
	}
}

VOID FashionDesignerFrame::SetNPCID(DWORD dwID, int npcType)
{
	m_dwNPCID = dwID;
	switch(npcType)
	{
	case EADVCOMT_Fashion:
		{
			m_eRanType = EADVCOMT_Fashion;
			m_pStcNPCBack->SetText(g_StrTable[_T("FashionDesigner")]);
			m_pStcDesc->SetText(g_StrTable[_T("PutFashionBook")]);
		}
		break;
	case EADVCOMT_Gem:
		{
			m_eRanType = EADVCOMT_Gem;
			m_pStcNPCBack->SetText(g_StrTable[_T("GemDesigner")]);
			m_pStcDesc->SetText(g_StrTable[_T("PutGemBook")]);
		}
		break;
	case EADVCOMT_GemBluePrint:
		{
			m_eRanType = EADVCOMT_GemBluePrint;
			m_pStcNPCBack->SetText(g_StrTable[_T("GemDesignerBluePrint")]);
			m_pStcDesc->SetText(g_StrTable[_T("PutGemBookBluePrint")]);
		}
		break;
	case EADVCOMT_YuanGu:
		{
			m_eRanType = EADVCOMT_YuanGu;
			m_pStcNPCBack->SetText(g_StrTable[_T("YuanGuDesigner")]);
			m_pStcDesc->SetText(g_StrTable[_T("PutYuanGu")]);
		}
		break;
	case EADVCOMT_ShengWen:
		{
			m_eRanType = EADVCOMT_ShengWen;
			m_pStcNPCBack->SetText(g_StrTable[_T("ShengWenDesigner")]);
			m_pStcDesc->SetText(g_StrTable[_T("PutShengWen")]);
		}
		break;
	default:
		m_eRanType = EADVCOMT_Fashion;
		m_pStcNPCBack->SetText(g_StrTable[_T("FashionDesigner")]);
		m_pStcDesc->SetText(g_StrTable[_T("PutFashionBook")]);
		break;
	}
	m_pWnd->SetRefresh(TRUE);
}

DWORD FashionDesignerFrame::NetProduceFashion( tagNS_AdvancedCompose *pNetCmd, DWORD )
{
	switch(m_eRanType)
	{
	case EADVCOMT_Fashion:
		{
			if(pNetCmd->dwErrorCode == E_Success)
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("FashionProduceSucc")]);
			else if(pNetCmd->dwErrorCode == E_Bag_NotEnoughSpace)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr40")]);
			else if(pNetCmd->dwErrorCode == E_Compose_NotEnough_Money)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionProduce_LessMoney")]);
			else if(pNetCmd->dwErrorCode == E_Compose_Vitality_Inadequate)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionProduce_LessHuoli")]);
			else
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("FashionProduceFail")]);
		}
		break;
	case EADVCOMT_Gem:
		{
			if(pNetCmd->dwErrorCode == E_Success)
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GemTransferSucc")]);
			else if(pNetCmd->dwErrorCode == E_Bag_NotEnoughSpace)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr40")]);
			else if(pNetCmd->dwErrorCode == E_Compose_NotEnough_Money)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionProduce_LessMoney")]);
			else if(pNetCmd->dwErrorCode == E_Compose_Vitality_Inadequate)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionProduce_LessHuoli")]);
			else
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GemTransferFail")]);
		}
		break;
	case EADVCOMT_GemBluePrint:
		{
			if(pNetCmd->dwErrorCode == E_Success)
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GemMakeferSucc")]);
			else if(pNetCmd->dwErrorCode == E_Bag_NotEnoughSpace)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr40")]);
			else if(pNetCmd->dwErrorCode == E_Compose_NotEnough_Money)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionProduce_LessMoney")]);
			else if(pNetCmd->dwErrorCode == E_Compose_Vitality_Inadequate)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionProduce_LessHuoli")]);
			else
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GemMakeferFail")]);
		}
		break;
	case EADVCOMT_YuanGu:
		{
			if(pNetCmd->dwErrorCode == E_Success)
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("YuanGuSucc")]);
			else if(pNetCmd->dwErrorCode == E_Bag_NotEnoughSpace)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr40")]);
			else if(pNetCmd->dwErrorCode == E_Compose_NotEnough_Money)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionProduce_LessMoney")]);
			else if(pNetCmd->dwErrorCode == E_Compose_Vitality_Inadequate)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionProduce_LessHuoli")]);
			else
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("YuanGuFail")]);
		}
		break;
	case EADVCOMT_ShengWen:
		{
			if(pNetCmd->dwErrorCode == E_Success)
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("ShengWenSucc")]);
			else if(pNetCmd->dwErrorCode == E_Bag_NotEnoughSpace)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr40")]);
			else if(pNetCmd->dwErrorCode == E_Compose_NotEnough_Money)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionProduce_LessMoney")]);
			else if(pNetCmd->dwErrorCode == E_Compose_Vitality_Inadequate)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionProduce_LessHuoli")]);
			else
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("ShengWenFail")]);
		}
		break;
	}
	ClearAll();
	return 0;
}

VOID FashionDesignerFrame::ClearAll()
{
	LockItem(m_n64Designer, false);
	m_pIconDesigner->SetPic(szItemPicInit);
	for(int i = 0; i < MAX_RANDOMPRODUCE_STUFF; i++)
	{
		LockItem(m_n64Stuff[i], false);
		m_pIconStuff[i]->SetPic(szItemPicInit);
		m_pStcStuffName[i]->SetText(_T(""));
		m_pStcStuffValue[i]->SetText(_T(""));
		m_n64Stuff[i] = GT_INVALID;
		m_bHaveItem[i] = false;
		m_n16StuffNum[i] = GT_INVALID;
	}
	m_pProTime->SetValue(0.0f,TRUE);
	m_pStcHuoLi->SetText(_T(""));
	m_pStcGold->SetText(_T(""));
	m_pStcSilver->SetText(_T(""));
	tagPoint ptSizeOldListBox = m_pListFashion->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = 0;
	m_pListFashion->SetResizable( TRUE );
	m_pListFashion->Resize( ptSizeListBox );
	m_pListFashion->SetResizable( FALSE );
	m_pListFashion->SetRefresh(TRUE);
	m_n64Designer = GT_INVALID;
}