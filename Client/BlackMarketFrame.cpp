#include "StdAfx.h"
#include "BlackMarketFrame.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "..\WorldDefine\msg_GodbidDefine.h"
#include "CombatSysUtil.h"
#include "IconStatic.h"
#include "CurrencyMgr.h"
#include "..\WorldDefine\currency_define.h"
#include "ItemMgr.h"
#include "ItemProtoData.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "BidMgr.h"

const TCHAR* GOLD_PIC = _T("data\\ui\\Bag\\L_icon-jin.bmp");
const TCHAR* SHENGBI_PIC = _T("data\\ui\\Bag\\L_icon-bao.bmp");
extern const TCHAR* szBackPic;

BlackMarketFrame::BlackMarketFrame(void):m_Trunk(this)
,m_dwNPCID(GT_INVALID)
,m_escapeTime(0)
,m_dwLeftTime(0)
,m_nFinalPrice(0)
{
	m_eMode = ECBM_Gold;
}

BlackMarketFrame::~BlackMarketFrame(void)
{
}

BOOL BlackMarketFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register( "NS_BlackMarketShopOpen",		(NETMSGPROC)m_Trunk.sfp2(&BlackMarketFrame::OnNS_BlackMarketShopOpen),		_T("NS_BlackMarketShopOpen"));
	m_pCmdMgr->Register( "NS_BlackConPrice",		(NETMSGPROC)m_Trunk.sfp2(&BlackMarketFrame::OnNS_BlackConPrice),		_T("NS_BlackConPrice"));

	m_dwCurMinOffer = 0;
	return TRUE;
}
BOOL BlackMarketFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	if( P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;
	}

	if(P_VALID(GetObj("Bid_Cost_Black")))
		KillObj("Bid_Cost_Black");

	m_pCmdMgr->UnRegister( "NS_BlackMarketShopOpen",							(NETMSGPROC)m_Trunk.sfp2(&BlackMarketFrame::OnNS_BlackMarketShopOpen) );
	m_pCmdMgr->UnRegister( "NS_BlackConPrice",							(NETMSGPROC)m_Trunk.sfp2(&BlackMarketFrame::OnNS_BlackConPrice) );
	return bRet;
}
BOOL BlackMarketFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\blackmarket.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &element, "blackmarket\\market_back\\back\\itemback\\itempic\\item", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &element, "blackmarket\\market_back\\back\\itemback\\start\\original_price\\pic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &element, "blackmarket\\market_back\\back\\itemback\\start\\minimum\\pic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &element, "blackmarket\\market_back\\back\\auctionback\\final\\pic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &element, "blackmarket\\market_back\\back\\itemback\\itempic", "ClassName", "IconStatic");

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&BlackMarketFrame::EventHandler));

	TCHAR szTmp[X_LONG_NAME] = {0};

	m_pPbGold= (GUIPushButton*)m_pWnd->GetChild(_T("market_back\\bagchoose\\bagchoose1"));				// 游戏币
	m_pPbShengbi= (GUIPushButton*)m_pWnd->GetChild(_T("market_back\\bagchoose\\bagchoose2"));			// 圣币


	m_pIcnItemPic = (IconStatic*)m_pWnd->GetChild(_T("market_back\\back\\itemback\\itempic\\item"));		
	m_pIcnItemPicBack = (IconStatic*)m_pWnd->GetChild(_T("market_back\\back\\itemback\\itempic"));		
	m_pIcnGoldPic1 = (IconStatic*)m_pWnd->GetChild(_T("market_back\\back\\itemback\\start\\original_price\\pic"));
	m_pIcnGoldPic2 = (IconStatic*)m_pWnd->GetChild(_T("market_back\\back\\itemback\\start\\minimum\\pic"));
	m_pIcnGoldPic3 = (IconStatic*)m_pWnd->GetChild(_T("market_back\\back\\auctionback\\final\\pic"));
	m_pStItemName = (GUIStaticEx*)m_pWnd->GetChild(_T("market_back\\back\\itemback\\start\\itemname"));	
	m_pStCurPrice = (GUIStatic*)m_pWnd->GetChild(_T("market_back\\back\\itemback\\start\\original_price\\num"));		
	m_pStMinOfferPrice = (GUIStatic*)m_pWnd->GetChild(_T("market_back\\back\\itemback\\start\\minimum\\num"));


	m_pStFinalPrice = (GUIStaticEx*)m_pWnd->GetChild(_T("market_back\\back\\auctionback\\final\\num"));					// 最终出价

	m_pStMoneyNotEnoughPic = (GUIStatic*)m_pWnd->GetChild(_T("market_back\\back\\auctionback\\lackpic"));		// 资金不足界面

	m_pStTime_Minite = (GUIStatic*)m_pWnd->GetChild(_T("market_back\\word\\minute"));								// 剩余时间 分
	m_pStTime_Second = (GUIStatic*)m_pWnd->GetChild(_T("market_back\\word\\seconde"));							// 剩余时间 秒

	m_pBnPriceAddx1 = (GUIButton*)m_pWnd->GetChild(_T("market_back\\back\\auctionback\\onetime"));				// 1倍出价
	m_pBnPriceAddx10 = (GUIButton*)m_pWnd->GetChild(_T("market_back\\back\\auctionback\\decuple"));				// 10倍出价
	m_pBnPriceAddx100 = (GUIButton*)m_pWnd->GetChild(_T("market_back\\back\\auctionback\\hundredfold"));		// 100倍出价
	m_pBnPriceAdd_All = (GUIButton*)m_pWnd->GetChild(_T("market_back\\back\\auctionback\\all"));						// 所有

	m_pBnClearPrice = (GUIButton*)m_pWnd->GetChild(_T("market_back\\back\\auctionback\\clearbutt"));					// 清楚加价
	m_pBnConformPrice = (GUIButton*)m_pWnd->GetChild(_T("market_back\\back\\auctionback\\surebutt"));				// 确认出价
	m_pBnRefresh = (GUIButton*)m_pWnd->GetChild(_T("market_back\\refresh"));												// 刷新
	m_pBnClose= (GUIButton*)m_pWnd->GetChild(_T("market_back\\close"));

	m_pIcnGoldPic1->SetPic(GOLD_PIC);
	m_pIcnGoldPic2->SetPic(GOLD_PIC);
	m_pIcnGoldPic3->SetPic(GOLD_PIC);

	m_pIcnItemPic->SetPic(szBackPic);
	m_pStFinalPrice->SetTextColor(0xFF00FF00);
	m_pStFinalPrice->SetText(_T(""));
	m_pStFinalPrice->SetRefresh(true);

	m_pStMoneyNotEnoughPic->SetInvisible(true);
	m_pPbGold->SetState(EGUIBS_PushDown,true);

	m_pWnd->SetInvisible(false);
	return TRUE;
}
BOOL BlackMarketFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return true;
}

VOID BlackMarketFrame::Update()
{
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
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

		if(m_dwLeftTime == 0) m_dwLeftTime = 1;
		m_dwLeftTime--;

		//	时间倒计时
		TCHAR szText[64];
		_stprintf( szText, _T("%d"),  m_dwLeftTime/60);
		m_pStTime_Minite->SetText(szText);

		_stprintf( szText, _T("%d"),  m_dwLeftTime%60);
		m_pStTime_Second->SetText(szText);

		m_pStTime_Minite->SetRefresh(true);
		m_pStTime_Second->SetRefresh(true);

		m_nRefreshLimitTime++;
		if (m_nRefreshLimitTime > 2)
			m_pBnRefresh->SetEnable(true);
	}

}

DWORD BlackMarketFrame::OnNS_BlackMarketShopOpen(tagNS_BlackMarketShopOpen* pNetCmd,DWORD)
{
	if (pNetCmd->dwErrorCode == E_Success)
	{
		m_tagGoldItem = pNetCmd->GoldItemInfo;
		m_tagShengbiItem = pNetCmd->ShengBiItemInfo;
		m_dwLeftTime = pNetCmd->dwLeftTime;

		RefreshUI();
		RefreshMoney();
	}
	else if (pNetCmd->dwErrorCode == EGBEC_Black_OutOfTime)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodBid_OutOfTime")]);
		m_tagGoldItem.clear();
		m_tagShengbiItem.clear();
		m_dwLeftTime = 0;

		m_dwCurMinOffer = 0;
		m_pBnPriceAddx1->SetText(_T(""));
		m_pBnPriceAddx10->SetText(_T(""));
		m_pBnPriceAddx100->SetText(_T(""));

		RefreshUI();
		RefreshMoney();
		ClearMoney();
	}
	
	return 0;
}

DWORD BlackMarketFrame::OnNS_BlackConPrice(tagNS_BlackConPrice* pNetCmd,DWORD)
{
	if (pNetCmd->dwErrorCode == E_Success)
	{
		TCHAR szText[256];
		if(pNetCmd->bIsGod)
			_stprintf( szText,g_StrTable[_T("Bid_Cost_Gold")],  MSilver2DBGold(pNetCmd->n64Price));
		else
			_stprintf( szText,g_StrTable[_T("Bid_Cost_ShengBi")],  pNetCmd->n64Price);

		if(P_VALID(GetObj("Bid_Cost_Black")))
			KillObj("Bid_Cost_Black");

		CreateObj("Bid_Cost_Black", "MsgBox");
		TObjRef<MsgBox>("Bid_Cost_Black")->Init(_T(""), szText, _T(""));

		ClearMoney();
	}
	else if (pNetCmd->dwErrorCode == EGBEC_Black_Only_One_Chance)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodBid_OnlyOnce")]);
	}
	else if (pNetCmd->dwErrorCode == E_NPC_NotFound)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodBid_E_NPC_NotFound")]);
	}
	else if (pNetCmd->dwErrorCode == E_NPC_NotValid)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodBid_E_NPC_NotValid")]);
	}
	else if (pNetCmd->dwErrorCode == E_NPC_TooFar)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodBid_E_NPC_TooFar")]);
	}
	else if (pNetCmd->dwErrorCode == EGBEC_Black_OutOfTime)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodBid_OutOfTime")]);
	}
	
	return 0;
}


BOOL BlackMarketFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

 	switch( pEvent->eEvent )
 	{
 	case EGUIE_Click:
 		{
 			if ( pWnd == m_pBnPriceAddx1)
 			{
 				if(m_dwCurMinOffer == 0) break;
				if (m_nFinalPrice == 0)
				{
					m_nFinalPrice += GetCurStartPrice();
				} 
 				m_nFinalPrice += m_dwCurMinOffer;
 				RefreshMoney();
 			} 
 			else if ( pWnd == m_pBnPriceAddx10)
 			{
				if (m_nFinalPrice == 0)
				{
					m_nFinalPrice += GetCurStartPrice();
				} 
 				if(m_dwCurMinOffer == 0) break;
 				m_nFinalPrice += m_dwCurMinOffer*10;
 				RefreshMoney();
 			}
 			else if ( pWnd == m_pBnPriceAddx100)
 			{
				if (m_nFinalPrice == 0)
				{
					m_nFinalPrice += GetCurStartPrice();
				} 
 				if(m_dwCurMinOffer == 0) break;
 				m_nFinalPrice += m_dwCurMinOffer*100;
 				RefreshMoney();
 			}
 			else if ( pWnd == m_pBnPriceAdd_All)
 			{
 				if(m_dwCurMinOffer == 0) break;
 
				int nGold = 0;
				if (m_eMode == ECBM_Gold)
				{
					INT64 n64Silver = CurrencyMgr::Inst()->GetBagSilver();
					nGold= MSilver2DBGold(n64Silver);
				} 
				else if(m_eMode == ECBM_Shengbi)
				{
					nGold = CurrencyMgr::Inst()->GetBagYuanbao();
				}
 				int CanOffer = nGold - nGold%m_dwCurMinOffer;
 				m_nFinalPrice = CanOffer;
 				RefreshMoney();
 			}
 			else if ( pWnd == m_pBnClearPrice)	// 清除
 			{
 				m_nFinalPrice = 0;
 				m_nAddPrice = 0;
 				m_pStFinalPrice->SetText(_T(""));
 				m_pStMoneyNotEnoughPic->SetInvisible(true);
				m_pBnConformPrice->SetEnable(true);
 			}
 			else if ( pWnd == m_pBnConformPrice)	// 确认竞价
 			{
 				ComformPrice();
 			}
 			else if ( pWnd == m_pBnRefresh)	// 刷新
 			{
 				tagNC_BlackMarketShopOpen cmd;
 				cmd.dwNPCID = m_dwNPCID;
 				m_pSession->Send(&cmd);
 
 				// 2秒点一次
 				m_pBnRefresh->SetEnable(false);
 				m_nRefreshLimitTime = 0;
 			}
 			else if ( pWnd == m_pBnClose)	// 关闭
 			{
 				m_pFrameMgr->AddToDestroyList(this);
 			}
 		}
 		break;
	case EGUIE_PushButtonDown:
		{
			if (m_pPbGold == pWnd)
			{
				m_eMode = ECBM_Gold;
				RefreshUI();
				RefreshMoney();
			}
			else if (m_pPbShengbi == pWnd)
			{
				m_eMode = ECBM_Shengbi;
				RefreshUI();
				RefreshMoney();
			}
		}
 	case EGUIE_MoveIntoCtrl:
 		{
			if (m_pIcnItemPic == pWnd)
			{
				if (m_eMode == ECBM_Gold)
				{
					if(m_tagGoldItem.sBidData.dwID != 0)
					{
						const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(m_tagGoldItem.sBidData.dwItemID);
						if (P_VALID(pItem))
						{
							ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem->dwTypeID,pItem->byQuality).c_str());
							break;
						}
					}
				}
				else if (m_eMode == ECBM_Shengbi)
				{
					if(m_tagShengbiItem.sBidData.dwID != 0)
					{
						const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(m_tagShengbiItem.sBidData.dwItemID);
						if (P_VALID(pItem))
						{
							ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem->dwTypeID,pItem->byQuality).c_str());
							break;
						}
					}
				}
			}
 		}
 		break;
 	default:
 		break;
 	}
	return TRUE;
}

VOID BlackMarketFrame::ComformPrice()
{

	if(m_tagGoldItem.sBidData.dwID == 0 ||m_tagShengbiItem.sBidData.dwID == 0)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodBid_NotActive")]);
		return;
	}

	if (m_nFinalPrice == 0)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodBid_NoOfferMoney")]);
		return;
	}

	if (m_eMode == ECBM_Gold)
	{
		
		tagNC_BlackConPrice cmd;
		cmd.dwFormulaID = m_tagGoldItem.sBidData.dwID;
		cmd.n64Price = MGold2Silver(m_nFinalPrice - GetCurStartPrice());	// 转换成银
		m_pSession->Send(&cmd);
	} 
	else if(m_eMode == ECBM_Shengbi)
	{
		
		tagNC_BlackConPrice cmd;
		cmd.dwFormulaID = m_tagShengbiItem.sBidData.dwID;
		cmd.n64Price = m_nFinalPrice - GetCurStartPrice();
		m_pSession->Send(&cmd);
	}

}
VOID BlackMarketFrame::RefreshMoney()
{
	TCHAR szText[64];

	int nGold = 0;
	if (m_eMode == ECBM_Gold)
	{
		INT64 n64Silver = CurrencyMgr::Inst()->GetBagSilver();
		nGold = MSilver2DBGold(n64Silver);
	} 
	else if(m_eMode == ECBM_Shengbi)
	{
		nGold = CurrencyMgr::Inst()->GetBagYuanbao();
	}

	if(nGold < m_nFinalPrice)	
	{
		m_pStMoneyNotEnoughPic->SetInvisible(false);
		m_pStFinalPrice->SetTextColor(0xFFFF0000);
		m_pBnConformPrice->SetEnable(false);
	}
	else
	{
		m_pStMoneyNotEnoughPic->SetInvisible(true);
		m_pStFinalPrice->SetTextColor(0xFF00FF00);
		m_pBnConformPrice->SetEnable(true);
	}
	m_pBnConformPrice->SetRefresh(true);

	_stprintf( szText, _T("%d"), m_nFinalPrice);
	m_pStFinalPrice->SetText(szText);
	m_pStFinalPrice->SetRefresh(true);

}
VOID BlackMarketFrame::RefreshUI()
{
	if (m_eMode == ECBM_Gold)
	{
		m_pIcnGoldPic1->SetPic(GOLD_PIC);
		m_pIcnGoldPic2->SetPic(GOLD_PIC);
		m_pIcnGoldPic3->SetPic(GOLD_PIC);

		TCHAR szText[64];
		if(m_tagGoldItem.sBidData.dwID != 0)
		{
			// 出价底限
			_stprintf( szText, _T("%d"),  MSilver2DBGold(m_tagGoldItem.sBidData.dwMinBidPrice));
			m_pStMinOfferPrice->SetText(szText);

			// 原始底价
			_stprintf( szText, _T("%d"),  MSilver2DBGold(m_tagGoldItem.sBidData.dwStartPrice));
			m_pStCurPrice->SetText(szText);
			// 刷新奖励物品
			const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(m_tagGoldItem.sBidData.dwItemID);
			if (P_VALID(pItem))
			{
				tstring ss = pItem->pDisplayInfo->szName;
				if(m_tagGoldItem.bIsBid)
					ss += g_StrTable[_T("Bid_AlreadyBid")];
				else
					ss += g_StrTable[_T("Bid_UnBid")];
				m_pStItemName->SetText(ss.c_str());
				RsfreshPicBack(pItem->byQuality);
				m_pIcnItemPic->SetPic(ItemMgr::Inst()->GetItemIconPath(m_tagGoldItem.sBidData.dwItemID).c_str());
				_stprintf( szText, _T("%d"), m_tagGoldItem.sBidData.nNum );
				m_pIcnItemPic->SetText(szText);
			}

			m_dwCurMinOffer = MSilver2DBGold(m_tagGoldItem.sBidData.dwMinBidPrice);
			m_nFinalPrice = 0;
			_stprintf( szText, _T("+%d"), m_dwCurMinOffer);
			m_pBnPriceAddx1->SetText(szText);
			_stprintf( szText, _T("+%d"), m_dwCurMinOffer*10);
			m_pBnPriceAddx10->SetText(szText);
			_stprintf( szText, _T("+%d"), m_dwCurMinOffer*100);
			m_pBnPriceAddx100->SetText(szText);

			// 最终出价
			_stprintf( szText, _T("+%d"),m_nFinalPrice);
			m_pStFinalPrice->SetText(szText);
		}
	
	} 
	else if(m_eMode == ECBM_Shengbi)
	{
		m_pIcnGoldPic1->SetPic(SHENGBI_PIC);
		m_pIcnGoldPic2->SetPic(SHENGBI_PIC);
		m_pIcnGoldPic3->SetPic(SHENGBI_PIC);

		TCHAR szText[64];
		if (m_tagShengbiItem.sBidData.dwID !=0)
		{
			// 出价底限
			_stprintf( szText, _T("%d"),  (m_tagShengbiItem.sBidData.dwMinBidPrice));
			m_pStMinOfferPrice->SetText(szText);

			// 原始底价
			_stprintf( szText, _T("%d"),  (m_tagShengbiItem.sBidData.dwStartPrice));
			m_pStCurPrice->SetText(szText);
			// 刷新奖励物品
			const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(m_tagShengbiItem.sBidData.dwItemID);
			if (P_VALID(pItem))
			{
				tstring ss = pItem->pDisplayInfo->szName;
				if(m_tagShengbiItem.bIsBid)
					ss += g_StrTable[_T("Bid_AlreadyBid")];
				else
					ss += g_StrTable[_T("Bid_UnBid")];
				m_pStItemName->SetText(ss.c_str());
				RsfreshPicBack(pItem->byQuality);
				m_pIcnItemPic->SetPic(ItemMgr::Inst()->GetItemIconPath(m_tagShengbiItem.sBidData.dwItemID).c_str());
				_stprintf( szText, _T("%d"), m_tagShengbiItem.sBidData.nNum );
				m_pIcnItemPic->SetText(szText);
			}

			m_dwCurMinOffer = m_tagShengbiItem.sBidData.dwMinBidPrice;	// 圣币
			m_nFinalPrice = 0;
			_stprintf( szText, _T("+%d"), m_dwCurMinOffer);
			m_pBnPriceAddx1->SetText(szText);
			_stprintf( szText, _T("+%d"), m_dwCurMinOffer*10);
			m_pBnPriceAddx10->SetText(szText);
			_stprintf( szText, _T("+%d"), m_dwCurMinOffer*100);
			m_pBnPriceAddx100->SetText(szText);

			// 最终出价
			_stprintf( szText, _T("+%d"),m_nFinalPrice);
			m_pStFinalPrice->SetText(szText);
		}
	}
}

VOID BlackMarketFrame::RsfreshPicBack(BYTE byQuality)
{
	// 设置物品品级
	if (1 == byQuality)
	{
		m_pIcnItemPicBack->SetPic(_T("data\\ui\\bag\\quality_yellow.bmp"));
	}
	else if (2 == byQuality)
	{
		m_pIcnItemPicBack->SetPic(_T("data\\ui\\bag\\quality_green.bmp"));
	}
	else if (3 == byQuality)
	{
		m_pIcnItemPicBack->SetPic(_T("data\\ui\\bag\\quality_blue.bmp"));
	}
	else if (4 == byQuality)
	{
		m_pIcnItemPicBack->SetPic(_T("data\\ui\\bag\\quality_orange.bmp"));
	}
	else if( 5 == byQuality )
	{
		m_pIcnItemPicBack->SetPic(_T("data\\ui\\bag\\quality_purple.bmp"));
	}
	else if( 6 == byQuality )
	{
		m_pIcnItemPicBack->SetPic(_T("data\\ui\\bag\\quality_pink.bmp"));
	}

	m_pIcnItemPicBack->SetRefresh(TRUE);
}

VOID BlackMarketFrame::ClearMoney()
{
	m_nFinalPrice = 0;
	m_nAddPrice = 0;
	RefreshMoney();
}

DWORD BlackMarketFrame::GetCurStartPrice()
{
	if (m_eMode == ECBM_Gold)
	{
		return MSilver2DBGold(m_tagGoldItem.sBidData.dwStartPrice);
	}
	else if (m_eMode == ECBM_Shengbi)
	{
		return m_tagShengbiItem.sBidData.dwStartPrice;
	}
	
	return 0;
}