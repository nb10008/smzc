#include "StdAfx.h"
#include "GodBidFrame.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "..\WorldDefine\msg_GodbidDefine.h"
#include "CombatSysUtil.h"
#include "IconStatic.h"
#include "ItemButton.h"
#include "BidMgr.h"
#include "ItemMgr.h"
#include "ItemProtoData.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "CurrencyMgr.h"
#include "..\WorldDefine\currency_define.h"
#include "IconStatic.h"
#include "ItemMgr.h"

extern const TCHAR* szBackPic;// = _T("data\\ui\\Bag\\L-main-icon-lock.bmp");

GodBidFrame::GodBidFrame(void):m_Trunk(this)
,m_dwNPCID(GT_INVALID)
,m_escapeTime(0)
,m_nCurrRow(0)
,m_nFinalPrice(0)
,m_dwCurMinOffer(0)
,m_nAddPrice(0)
,m_nRefreshLimitTime(0)
,m_dwLeftTime(0)
{
}

GodBidFrame::~GodBidFrame(void)
{
}

BOOL GodBidFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register( "NS_GodBiderShopOpen",		(NETMSGPROC)m_Trunk.sfp2(&GodBidFrame::OnNS_GodBiderShopOpen),		_T("NS_GodBiderShopOpen"));
	m_pCmdMgr->Register( "NS_ConformPrice",		(NETMSGPROC)m_Trunk.sfp2(&GodBidFrame::OnNS_ConformPrice),		_T("NS_ConformPrice"));

	m_mapBidItemInfo.clear();
	return TRUE;
}
BOOL GodBidFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	m_pCmdMgr->UnRegister( "NS_GodBiderShopOpen",							(NETMSGPROC)m_Trunk.sfp2(&GodBidFrame::OnNS_GodBiderShopOpen) );
	m_pCmdMgr->UnRegister( "NS_ConformPrice",							(NETMSGPROC)m_Trunk.sfp2(&GodBidFrame::OnNS_ConformPrice) );

	if(P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}

	if(P_VALID(GetObj("Bid_Cost_God")))
		KillObj("Bid_Cost_God");

	return bRet;
}
BOOL GodBidFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\auction.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &element, "auction\\auction_back\\itemback\\itemback0\\itempic\\item", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &element, "auction\\auction_back\\itemback\\itemback1\\itempic\\item", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &element, "auction\\auction_back\\itemback\\itemback2\\itempic\\item", "ClassName", "IconStatic");

	m_pGUI->ChangeXml( &element, "auction\\auction_back\\itemback\\itemback0\\itempic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &element, "auction\\auction_back\\itemback\\itemback1\\itempic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &element, "auction\\auction_back\\itemback\\itemback2\\itempic", "ClassName", "IconStatic");

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&GodBidFrame::EventHandler));

	TCHAR szTmp[X_LONG_NAME] = {0};
	for (int i=0; i<MAX_GODBID_ITEM_NUM; i++)
	{
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("auction_back\\itemback\\itemback%d"), i);
		m_BidItems[i].pIcnBack = (GUIStatic*)m_pWnd->GetChild(szTmp);	
		
		m_BidItems[i].pIcnHighestPic = (GUIStatic*)m_BidItems[i].pIcnBack->GetChild(_T("highestpic"));
		m_BidItems[i].pIcnItemPic = (IconStatic*)m_BidItems[i].pIcnBack->GetChild(_T("itempic\\item"));
		m_BidItems[i].pIcnItemPicBack = (IconStatic*)m_BidItems[i].pIcnBack->GetChild(_T("itempic"));
		m_BidItems[i].pStChooseing = (GUIStatic*)m_BidItems[i].pIcnBack->GetChild(_T("choosepic"));
		m_BidItems[i].pStCurPrice = (GUIStatic*)m_BidItems[i].pIcnBack->GetChild(_T("start\\current_price\\num"));
		m_BidItems[i].pStItemName = (GUIStatic*)m_BidItems[i].pIcnBack->GetChild(_T("start\\itemname"));
		m_BidItems[i].pStMinOfferPrice = (GUIStatic*)m_BidItems[i].pIcnBack->GetChild(_T("start\\minimum\\num"));
		m_BidItems[i].pStOpen = (GUIStatic*)m_BidItems[i].pIcnBack->GetChild(_T("start"));
		m_BidItems[i].pStUnOpen = (GUIStatic*)m_BidItems[i].pIcnBack->GetChild(_T("end"));

		m_BidItems[i].pIcnItemPic->SetPic(szBackPic);
		m_BidItems[i].pIcnHighestPic->SetInvisible(true);
		m_BidItems[i].pStOpen->SetInvisible(true);
		m_BidItems[i].pStUnOpen->SetInvisible(false);
		m_BidItems[i].pStChooseing->SetInvisible(true);
		m_BidItems[i].pStChooseing->SetUnClickable(true);
		m_BidItems[i].pStOpen->SetUnClickable(true);
		m_BidItems[i].pStUnOpen->SetUnClickable(true);
		m_BidItems[i].pIcnHighestPic->SetUnClickable(true);
		//m_BidItems[i].pIcnItemPic->SetUnClickable(true);
	}
	
	m_pStAlreadyOfferPrice = (GUIStatic*)m_pWnd->GetChild(_T("auction_back\\auctionback\\payment\\num"));	//	已出价
	m_pStAddMoney = (GUIStaticEx*)m_pWnd->GetChild(_T("auction_back\\auctionback\\add\\num"));					// 本次增加价格
	m_pStFinalPrice = (GUIStaticEx*)m_pWnd->GetChild(_T("auction_back\\auctionback\\final\\num"));					// 最终出价

	m_pStMoneyNotEnoughPic = (GUIStatic*)m_pWnd->GetChild(_T("auction_back\\auctionback\\lackpic"));		// 资金不足界面
	m_pStHighestPricePic = (GUIStatic*)m_pWnd->GetChild(_T("auction_back\\auctionback\\maxpic"));				// 最高出价界面

	m_pStTime_Minite = (GUIStatic*)m_pWnd->GetChild(_T("auction_back\\word\\minute"));								// 剩余时间 分
	m_pStTime_Second = (GUIStatic*)m_pWnd->GetChild(_T("auction_back\\word\\seconde"));							// 剩余时间 秒

	m_pBnPriceAddx1 = (GUIButton*)m_pWnd->GetChild(_T("auction_back\\auctionback\\onetime"));				// 1倍出价
	m_pBnPriceAddx10 = (GUIButton*)m_pWnd->GetChild(_T("auction_back\\auctionback\\decuple"));				// 10倍出价
	m_pBnPriceAddx100 = (GUIButton*)m_pWnd->GetChild(_T("auction_back\\auctionback\\hundredfold"));		// 100倍出价
	m_pBnPriceAdd_All = (GUIButton*)m_pWnd->GetChild(_T("auction_back\\auctionback\\all"));						// 所有

	m_pBnClearPrice = (GUIButton*)m_pWnd->GetChild(_T("auction_back\\auctionback\\clearbutt"));					// 清楚加价
	m_pBnConformPrice = (GUIButton*)m_pWnd->GetChild(_T("auction_back\\auctionback\\surebutt"));				// 确认出价
	m_pBnRefresh = (GUIButton*)m_pWnd->GetChild(_T("auction_back\\refresh"));												// 刷新
	m_pBnClose= (GUIButton*)m_pWnd->GetChild(_T("auction_back\\close"));

	m_pStMoneyNotEnoughPic->SetInvisible(true);
	m_pStHighestPricePic->SetInvisible(true);
	m_pStFinalPrice->SetTextColor(0xFF00FF00);
	m_pStFinalPrice->SetText(_T(""));
	m_pStFinalPrice->SetRefresh(true);
	m_pWnd->SetInvisible(false);

	return TRUE;
}
BOOL GodBidFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return true;
}

VOID GodBidFrame::Update()
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

DWORD GodBidFrame::OnNS_GodBiderShopOpen(tagNS_GodBiderShopOpen* pNetCmd,DWORD)
{
	if (pNetCmd->dwErrorCode == E_Success)
	{
		m_mapBidItemInfo.clear();

		for (int i=0; i<MAX_GODBID_ITEM_NUM; i++)
		{
			m_BidItems[i].pStOpen->SetInvisible(false);
			m_BidItems[i].pStUnOpen->SetInvisible(true);
			m_mapBidItemInfo.insert(make_pair(i,pNetCmd->bidItemInfo[i]));
		}
		// 更新时间
		m_dwLeftTime = pNetCmd->dwLeftTime;

		RefreshUI(m_nCurrRow);
	}
	else{
		for (int i=0; i<MAX_GODBID_ITEM_NUM; i++)
		{
			m_BidItems[i].pStOpen->SetInvisible(true);
			m_BidItems[i].pStUnOpen->SetInvisible(false);
			m_BidItems[i].pIcnItemPic->SetPic(szBackPic);
			m_BidItems[i].pIcnItemPic->SetText(_T(""));
			m_BidItems[i].pIcnItemPicBack->SetPic(szBackPic);
			m_BidItems[i].pIcnHighestPic->SetInvisible(true);
		}
	
		m_dwCurMinOffer =0;
		m_pBnPriceAddx1->SetText(_T(""));
		m_pBnPriceAddx10->SetText(_T(""));
		m_pBnPriceAddx100->SetText(_T(""));
		m_pStAlreadyOfferPrice->SetText(_T(""));
		ClearMoney();
		if (pNetCmd->dwErrorCode == EGBEC_Black_OutOfTime)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodBid_OutOfTime")]);
		}
	}
	return 0;
}

DWORD GodBidFrame::OnNS_ConformPrice(tagNS_ConformPrice* pNetCmd,DWORD)
{
	if (pNetCmd->dwErrorCode == E_Success)
	{
		TCHAR szText[256];
		_stprintf( szText,g_StrTable[_T("Bid_Cost_Gold")],  MSilver2DBGold(pNetCmd->n64Price));

		if(P_VALID(GetObj("Bid_Cost_God")))
			KillObj("Bid_Cost_God");

		CreateObj("Bid_Cost_God", "MsgBox");
		TObjRef<MsgBox>("Bid_Cost_God")->Init(_T(""), szText, _T(""));

		ClearMoney();
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
VOID GodBidFrame::ComformPrice()
{
	if(m_mapBidItemInfo.empty())
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodBid_NotActive")]);
		return;
	}

	if (m_nAddPrice == 0)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodBid_NoOfferMoney")]);
		return;
	}
	map<int,tagGodBidItemInfo>::iterator it = m_mapBidItemInfo.find(m_nCurrRow);
	if (it != m_mapBidItemInfo.end())
	{
		if(!IsCurrItemPriceOffered())
			m_nAddPrice -= GetCurrStartPrice();

		tagNC_ConformPrice cmd;
		cmd.dwFormulaID = it->second.sBidData.dwID;
		cmd.n64Price = MGold2Silver(m_nAddPrice);	// 转换成银
		m_pSession->Send(&cmd);
	}

}

BOOL GodBidFrame::EventHandler(tagGUIEvent* pEvent)
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
				if(m_nAddPrice == 0 && !IsCurrItemPriceOffered())
				{
					m_nAddPrice =+ GetCurrStartPrice();
				}
				m_nAddPrice += m_dwCurMinOffer;
				m_nFinalPrice = m_dwCurPrice + m_nAddPrice;
				RefreshMoney();
			} 
			else if ( pWnd == m_pBnPriceAddx10)
			{
				if(m_dwCurMinOffer == 0) break;
				if(m_nAddPrice == 0 && !IsCurrItemPriceOffered())
				{
					m_nAddPrice =+ GetCurrStartPrice();
				}
				m_nAddPrice += m_dwCurMinOffer*10;
				m_nFinalPrice = m_dwCurPrice + m_nAddPrice;
				RefreshMoney();
			}
			else if ( pWnd == m_pBnPriceAddx100)
			{
				if(m_dwCurMinOffer == 0) break;
				if(m_nAddPrice == 0 && !IsCurrItemPriceOffered())
				{
					m_nAddPrice =+ GetCurrStartPrice();
				}
				m_nAddPrice += m_dwCurMinOffer*100;
				m_nFinalPrice = m_dwCurPrice + m_nAddPrice;
				RefreshMoney();
			}
			else if ( pWnd == m_pBnPriceAdd_All)
			{
				if(m_dwCurMinOffer == 0) break;

				INT64 n64Silver = CurrencyMgr::Inst()->GetBagSilver();
				int nGold = MSilver2DBGold(n64Silver);
				int CanOffer = nGold - nGold%m_dwCurMinOffer;
				m_nAddPrice = CanOffer;
				m_nFinalPrice = m_dwCurPrice + m_nAddPrice;
				RefreshMoney();
			}
			else if ( pWnd == m_pBnClearPrice)	// 清除
			{
				m_nFinalPrice = 0;
				m_nAddPrice = 0;
				m_pStAddMoney->SetText(_T(""));
				m_pStFinalPrice->SetText(_T(""));
				m_pStMoneyNotEnoughPic->SetInvisible(true);
				m_pStHighestPricePic->SetInvisible(true);
				m_pBnConformPrice->SetEnable(true);
			}
			else if ( pWnd == m_pBnConformPrice)	// 确认竞价
			{
				ComformPrice();
			}
			else if ( pWnd == m_pBnRefresh)	// 刷新
			{
				tagNC_GodBiderShopOpen cmd;
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
	case EGUIE_MoveIntoCtrl:
		{
			for (int i = 0; i < MAX_GODBID_ITEM_NUM; ++i)
			{
				if (m_BidItems[i].pIcnItemPic == pWnd)
				{
					map<int,tagGodBidItemInfo>::iterator iter = m_mapBidItemInfo.find(i);
					if (iter != m_mapBidItemInfo.end()) 
					{
						if (iter->second.sBidData.dwID != 0)
						{
							const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(iter->second.sBidData.dwItemID);
							if (P_VALID(pItem))
							{
								ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem->dwTypeID,pItem->byQuality).c_str());
								break;
							}
						}
					}
				}
			}
		}
		break;
	case EGUIE_DragStart:
		{
			for (int i = 0; i < MAX_GODBID_ITEM_NUM; ++i)
			{
				if (m_BidItems[i].pIcnBack == pWnd ||	m_BidItems[i].pIcnItemPic == pWnd )	// 选择其他的item
				{
					m_nCurrRow = i;
					RefreshUI(m_nCurrRow);
					m_nAddPrice =0;
					m_nFinalPrice =0;
					RefreshMoney();
					break;
				}
			}
		}
	default:
		break;
	}
	return TRUE;
}

VOID GodBidFrame::RefreshUI(INT nRow)
{
	if(nRow >= MAX_GODBID_ITEM_NUM) return;

	if(m_mapBidItemInfo.empty())	return;

	TCHAR szText[64];

	int i =0;
	// 先刷新所有竞标物品UI
	for (map<int,tagGodBidItemInfo>::iterator it = m_mapBidItemInfo.begin();
		it != m_mapBidItemInfo.end(); ++it)
	{
		if (it->second.sBidData.dwID != 0)
		{
			m_BidItems[i].pIcnHighestPic->SetInvisible(true);
			// 是否最高出价
			if(it->second.bHighestPrice)		m_BidItems[i].pIcnHighestPic->SetInvisible(false);
			// 当前出价
			if (it->second.n64CurPrice == 0)
			{
				_stprintf( szText, _T("%d"),  MSilver2DBGold(it->second.sBidData.dwStartPrice));
				m_BidItems[i].pStCurPrice->SetText(szText);
			}
			else{
				_stprintf( szText, _T("%d"),  MSilver2DBGold(it->second.n64CurPrice));
				m_BidItems[i].pStCurPrice->SetText(szText);
			}
			// 出价底限
			_stprintf( szText, _T("%d"),  MSilver2DBGold(it->second.sBidData.dwMinBidPrice));
			m_BidItems[i].pStMinOfferPrice->SetText(szText);
			// 刷新奖励物品
			const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(it->second.sBidData.dwItemID);
			if (P_VALID(pItem))
			{
				m_BidItems[i].pStItemName->SetText(pItem->pDisplayInfo->szName);
				RefreshPicBack(m_BidItems[i].pIcnItemPicBack,pItem->byQuality);
				m_BidItems[i].pIcnItemPic->SetPic(ItemMgr::Inst()->GetItemIconPath(it->second.sBidData.dwItemID).c_str());
				_stprintf( szText, _T("%d"), it->second.sBidData.nNum );
				m_BidItems[i].pIcnItemPic->SetText(szText);
			}
		}

		m_BidItems[i].pStChooseing->SetInvisible(true);

		++i;
	}


 	map<int,tagGodBidItemInfo>::iterator iter = m_mapBidItemInfo.find(nRow);
 	if (iter != m_mapBidItemInfo.end()) 
 	{
		if (iter->second.sBidData.dwID != 0)
		{
			// 设置出价按钮
			m_dwCurMinOffer = MSilver2DBGold(iter->second.sBidData.dwMinBidPrice);
			m_dwCurPrice = MSilver2DBGold(iter->second.n64PlayerOfferPrice);
			_stprintf( szText, _T("+%d"), m_dwCurMinOffer);
			m_pBnPriceAddx1->SetText(szText);
			_stprintf( szText, _T("+%d"), m_dwCurMinOffer*10);
			m_pBnPriceAddx10->SetText(szText);
			_stprintf( szText, _T("+%d"), m_dwCurMinOffer*100);
			m_pBnPriceAddx100->SetText(szText);

			// 当前玩家已经出价
			_stprintf( szText, _T("%d"), MSilver2DBGold(iter->second.n64PlayerOfferPrice));
			m_pStAlreadyOfferPrice->SetText(szText);
		}

 	}

 	m_BidItems[nRow].pStChooseing->SetInvisible(false);
 	m_BidItems[nRow].pStChooseing->FlipToTop();
}

VOID GodBidFrame::RefreshMoney()
{
	TCHAR szText[64];
	_stprintf( szText, _T("%d"), m_nAddPrice);
	m_pStAddMoney->SetText(szText);
	

	// 资金不足界面刷新
	INT64 n64Silver = CurrencyMgr::Inst()->GetBagSilver();
	int nGold = MSilver2DBGold(n64Silver);
	if(nGold < m_nAddPrice)	
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
	
	// 最高出价界面刷新
	map<int,tagGodBidItemInfo>::iterator iter = m_mapBidItemInfo.find(m_nCurrRow);
	if (iter != m_mapBidItemInfo.end()) 
	{
		if ( m_nFinalPrice > MSilver2DBGold(iter->second.n64CurPrice))
			m_pStHighestPricePic->SetInvisible(false);
		else
			m_pStHighestPricePic->SetInvisible(true);
	}

	m_pBnConformPrice->SetRefresh(true);
	m_pStAddMoney->SetRefresh(true);

	_stprintf( szText, _T("%d"), m_nFinalPrice);
	m_pStFinalPrice->SetText(szText);
	m_pStFinalPrice->SetRefresh(true);
}

VOID GodBidFrame::RefreshPicBack(IconStatic* pWnd,BYTE byQuality)
{
	if(!P_VALID(pWnd))		return;

	// 设置物品品级
	if (1 == byQuality)
	{
		pWnd->SetPic(_T("data\\ui\\bag\\quality_yellow.bmp"));
	}
	else if (2 == byQuality)
	{
		pWnd->SetPic(_T("data\\ui\\bag\\quality_green.bmp"));
	}
	else if (3 == byQuality)
	{
		pWnd->SetPic(_T("data\\ui\\bag\\quality_blue.bmp"));
	}
	else if (4 == byQuality)
	{
		pWnd->SetPic(_T("data\\ui\\bag\\quality_orange.bmp"));
	}
	else if( 5 == byQuality )
	{
		pWnd->SetPic(_T("data\\ui\\bag\\quality_purple.bmp"));
	}
	else if( 6 == byQuality )
	{
		pWnd->SetPic(_T("data\\ui\\bag\\quality_pink.bmp"));
	}
	pWnd->SetRefresh(TRUE);
}

VOID GodBidFrame::ClearMoney()
{
	m_nFinalPrice = 0;
	m_nAddPrice = 0;
	m_dwCurPrice = 0;
	RefreshMoney();
}

DWORD GodBidFrame::GetCurrStartPrice()
{
	map<int,tagGodBidItemInfo>::iterator it = m_mapBidItemInfo.find(m_nCurrRow);
	if (it != m_mapBidItemInfo.end())
	{
		return MSilver2DBGold(it->second.sBidData.dwStartPrice);
	}
	return 0;
}

bool GodBidFrame::IsCurrItemPriceOffered()
{
	map<int,tagGodBidItemInfo>::iterator it = m_mapBidItemInfo.find(m_nCurrRow);
	if (it != m_mapBidItemInfo.end())
	{
		return !(0 ==  it->second.n64PlayerOfferPrice );	// 玩家是否出过价
	}
	return false;
}