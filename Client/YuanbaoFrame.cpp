#include "StdAfx.h"
#include "YuanbaoFrame.h"
#include "MsgBox.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\currency_define.h"
#include "CurrencyMgr.h"

const float g_fTax = 0.95f; //  交易税率
YuanbaoFrame::YuanbaoFrame(void) : m_Trunk(this), m_bisSell(true)
{
	m_bBuyFlag = false;
	m_bSellFlag = false;

	m_dwTime = 0;
	m_bCanSend = TRUE;
	m_pOrderCannel = NULL;
	m_pOrderWnd = NULL;

}

YuanbaoFrame::~YuanbaoFrame(void)
{
}

VOID YuanbaoFrame::Update()
{
	if( ( m_bBuyFlag || m_bSellFlag ) && !m_pWnd->IsInvisible() )
	{
		if(m_bSellFlag)
			UpdateSellList();
		if(m_bBuyFlag)
			UpdateBuyList();
	}

	if ( P_VALID(m_pOrderWnd) && ( !m_pOrderWnd->IsInvisible() ) )
	{
		m_dwTime += Kernel::Inst()->GetDeltaTimeDW();
		if ( m_dwTime > 5000 ) // 延迟5秒
		{
			m_dwTime = 0;
			if (P_VALID(m_pOrderCannel))
			{
				m_pOrderCannel->SetEnable(TRUE);
				m_pOrderWnd->SetRefresh();
			} 
		}
	}
	
	
	GameFrame::Update();
}

BOOL YuanbaoFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    BOOL ret    =   GameFrame::Init(pMgr, pFather, dwParam);
    m_pCurrencyMgr  =   CurrencyMgr::Inst();

	m_dwTime = 0;			//刷新消息发送时间
	m_bCanSend = TRUE;
    // 注册游戏事件
    m_pFrameMgr->RegisterEventHandle(_T("tagYBReturnEvnt"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnYuanBaoTradeRet));

    m_pFrameMgr->RegisterEventHandle(_T("SaveMoney"),           (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnSaveMoney));
    m_pFrameMgr->RegisterEventHandle(_T("DepositMoney"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnDepositMoney));
    m_pFrameMgr->RegisterEventHandle(_T("SellYuanBao"),         (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnSellYuanBao));
    m_pFrameMgr->RegisterEventHandle(_T("BuyYuanBao"),          (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnBuyYuanBao));
	m_pFrameMgr->RegisterEventHandle(_T("TooMuchMoney"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnTooMuchMoney));
	m_pFrameMgr->RegisterEventHandle(_T("TooMuchYuanBao"),      (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnTooMuchYuanBao));
    return ret;
}

BOOL YuanbaoFrame::Destroy()
{
    BOOL ret    =   GameFrame::Destroy();

    // 注销游戏事件
    m_pFrameMgr->UnRegisterEventHandler(_T("tagYBReturnEvnt"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnYuanBaoTradeRet));

    m_pFrameMgr->UnRegisterEventHandler(_T("SaveMoney"),           (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnSaveMoney));
    m_pFrameMgr->UnRegisterEventHandler(_T("DepositMoney"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnDepositMoney));
    m_pFrameMgr->UnRegisterEventHandler(_T("SellYuanBao"),         (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnSellYuanBao));
    m_pFrameMgr->UnRegisterEventHandler(_T("BuyYuanBao"),          (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnBuyYuanBao));
	m_pFrameMgr->UnRegisterEventHandler(_T("TooMuchMoney"),        (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnTooMuchMoney));
	m_pFrameMgr->UnRegisterEventHandler(_T("TooMuchYuanBao"),      (FRAMEEVENTPROC)m_Trunk.sfp1(&YuanbaoFrame::OnTooMuchYuanBao));

	DestroyObjects();
    // 销毁订单查询窗体
    if(P_VALID(m_pOrderWnd))
    {
        m_pGUI->UnRegisterEventHandler(m_pOrderWnd->GetFullName().c_str(), m_Trunk.sfp1(&YuanbaoFrame::OrderQEventHandler));
        m_pGUI->AddToDestroyList(m_pOrderWnd);
        m_pOrderWnd = NULL;
    }



    return ret;
}

BOOL YuanbaoFrame::ReloadUI()
{
    // 创建UI
    m_pWndFather            =   m_pGUI->GetWnd(((MallFrame*)m_pFather)->GetGUIFullName().c_str());

    // 元宝交易子窗体
    m_pWnd                  =   m_pWndFather->GetChild(_T("mallbackpic\\tradeback"));

    m_pBtSalePrice          =   (GUIButton*)m_pWnd->GetChild(_T("saleprice"));
    m_pBtSaleNum            =   (GUIButton*)m_pWnd->GetChild(_T("salenum"));
    m_pBtBuyPrice           =   (GUIButton*)m_pWnd->GetChild(_T("buyprice"));
    m_pBtBuyNum             =   (GUIButton*)m_pWnd->GetChild(_T("buynum"));
    m_pLBSale               =   (GUIListBox*)m_pWnd->GetChild(_T("salelist"));
    m_pLBBuy                =   (GUIListBox*)m_pWnd->GetChild(_T("buylist"));

    m_pBtWannaSell          =   (GUIPushButton*)m_pWnd->GetChild(_T("fra\\sale"));
    m_pBtWannaBuy           =   (GUIPushButton*)m_pWnd->GetChild(_T("fra\\buy"));
    m_pEdtWannaPriceGold    =   (GUIEditBox*)m_pWnd->GetChild(_T("putinback1\\putin"));
    m_pEdtWannaPriceSilver  =   (GUIEditBox*)m_pWnd->GetChild(_T("putinback2\\putin"));
    m_pEdtWannaNum          =   (GUIEditBox*)m_pWnd->GetChild(_T("putinback3\\putin"));
    m_pBtWannaConfirm       =   (GUIButton*)m_pWnd->GetChild(_T("donebutt"));
	m_pStcTotalGold         =   (GUIStatic*)m_pWnd->GetChild(_T("num3"));
	m_pStcTotalSilver       =   (GUIStatic*)m_pWnd->GetChild(_T("num7"));
	m_pStcTaxGold           =   (GUIStatic*)m_pWnd->GetChild(_T("num8"));
	m_pStcTaxSilver         =   (GUIStatic*)m_pWnd->GetChild(_T("num9"));

    m_pStcAcctGold          =   (GUIStatic*)m_pWnd->GetChild(_T("num4"));
    m_pStcAcctSilver        =   (GUIStatic*)m_pWnd->GetChild(_T("num5"));
    m_pBtMoneySave          =   (GUIButton*)m_pWnd->GetChild(_T("remove1"));
    m_pBtMoneyLoad          =   (GUIButton*)m_pWnd->GetChild(_T("save1"));
    m_pStcYuanbao           =   (GUIStatic*)m_pWnd->GetChild(_T("num6"));

    m_pBtRefresh            =   (GUIButton*)m_pWnd->GetChild(_T("refreshbutt"));
    m_pBtOrderQuery         =   (GUIButton*)m_pWnd->GetChild(_T("querybutt"));
	m_pStcRules				=	(GUIStaticEx*)m_pWnd->GetChild(_T("intr_back\\intr_list"));

	tagPoint size = m_pStcRules->GetSize();
	if( m_pStcRules->GetTextTotalHeight() > size.y )
	{
		size.y = m_pStcRules->GetTextTotalHeight();
		m_pStcRules->SetResizable(TRUE);
		m_pStcRules->Resize( size );
		m_pStcRules->SetResizable(FALSE);
	}

	m_pStcRules->SetRefresh(TRUE);


    // 创建订单查询窗体
    XmlElement ele;
    tstring strPath = g_strLocalPath + _T("\\ui\\orderinquiry.xml");
    m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

    m_pOrderWnd             =   m_pGUI->CreateWnd(_T("\\desktop"), &ele);
    m_pOrderWnd->SetInvisible(TRUE);

    m_pOrderClose           =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\closebutt"));

    m_pOrderTable           =   (GUIListBox*)m_pOrderWnd->GetChild(_T("oi_pic\\oi_listbackpic\\orderinquiry_list"));

    m_pOrderID              =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\oi_listbackpic\\idnum"));
    m_pOrderType            =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\oi_listbackpic\\type"));
    m_pOrderPrice           =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\oi_listbackpic\\pricing"));
    m_pOrderNum             =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\oi_listbackpic\\amount"));
    m_pOrderStus            =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\oi_listbackpic\\state"));
    m_pOrderTimeFrom        =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\oi_listbackpic\\starttime"));
    m_pOrderTimeTo          =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\oi_listbackpic\\overtime"));
    m_pOrderAveragePrice    =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\oi_listbackpic\\averageprice"));
    m_pOrderFinalNum        =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\oi_listbackpic\\averageprice0"));

    m_pOrderCannel          =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\delete"));
    m_pOrderRefresh         =   (GUIButton*)m_pOrderWnd->GetChild(_T("oi_pic\\refresh"));

    m_pBtWannaSell->SetText(g_StrTable[_T("YuanBaoTrade4")]);
    m_pBtWannaBuy->SetText(g_StrTable[_T("YuanBaoTrade5")]);

    m_pBtWannaSell->SetState(EGUIBS_PushDown);

    // 初始化ListBox
    InitListBox();

    UpdateYuanbaoTrade();
    
    // 注册UI事件处理函数
    m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&YuanbaoFrame::YuanbaoEventHandler));
    m_pGUI->RegisterEventHandler(m_pOrderWnd->GetFullName().c_str(), m_Trunk.sfp1(&YuanbaoFrame::OrderQEventHandler));

    return TRUE;
}

DWORD YuanbaoFrame::YuanbaoEventHandler( tagGUIEvent* pEvent )
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch(pEvent->eEvent)
    {
	case EGUIE_KeyDown:
		{
			if( m_pGUI->GetActive() == m_pEdtWannaPriceGold || 
				m_pGUI->GetActive() == m_pEdtWannaPriceSilver || 
				m_pGUI->GetActive() == m_pEdtWannaNum )
				CalcTradeMoney();
		}
		break;
    case EGUIE_PushButtonDown:
        if (pWnd == m_pBtWannaSell)
        {
            // 我要出售选项卡
            m_bisSell   =   true;
            m_pBtWannaConfirm->SetText(g_StrTable[_T("YuanBaoTrade8")]);
			ResetInput();
        }
        else if (pWnd == m_pBtWannaBuy)
        {
            // 我要收购选项卡
            m_bisSell   =   false;
            m_pBtWannaConfirm->SetText(g_StrTable[_T("YuanBaoTrade9")]);
			ResetInput();
        }
		break;
    case EGUIE_Click:
		if (pWnd == m_pBtOrderQuery)
		{
			// 打开订单查询
			ShowOrderQuery(m_pOrderWnd->IsInvisible());
		}
		else if (pWnd == m_pBtRefresh)
		{
			// 刷新
			YuanbaoTradeMgr::Inst()->BeginNC_SynYBTradeInfo();
			UpdateYuanbaoTrade();
		}
		else if (pWnd == m_pBtWannaConfirm)
		{
			// 开始出售/收购

			// 计算总价
			m_nInputGold        =   _ttoi(m_pEdtWannaPriceGold->GetText());
			m_nInputSilver      =   _ttoi(m_pEdtWannaPriceSilver->GetText());
			m_nInputNum         =   _ttoi(m_pEdtWannaNum->GetText());

			INT64   nSum        =   CalSum(m_nInputGold, m_nInputSilver, m_nInputNum);
			INT     nSumG       =   MSilver2DBGold(nSum);
			INT     nSumS       =   MSilver2DBSilver(nSum);
			INT64   nCharge     =   CalCharge(m_nInputGold, m_nInputSilver, m_nInputNum);
			INT     nChargeG    =   MSilver2DBGold(nCharge);
			INT     nChargeS    =   MSilver2DBSilver(nCharge);

			// 弹出消息框
			if (m_bisSell)
			{
				// Jason 2010-4-7 v1.5.1
				INT64 accountSilver = 0;
				const tstring & temstr = m_pStcAcctGold->GetText();
				if( !temstr.empty() )
					accountSilver = _ttoi(temstr.c_str());
				const tstring & temSilver = m_pStcAcctSilver->GetText();
				if( !temSilver.empty() )
					accountSilver = MGold2Silver(accountSilver) + _ttoi(temSilver.c_str());

				if( //nSum*g_fTax + CurrencyMgr::Inst()->GetBagSilver() > MAX_BAG_SILVER_NUM || 
					nSum + accountSilver > MAX_BAG_SILVER_NUM )
				{
					if (P_VALID(GetObj("TooMuchMoney")))
						KillObj("TooMuchMoney");
					CreateObj("TooMuchMoney", "MsgBox");
					TObjRef<MsgBox>("TooMuchMoney")->Init(_T(""), g_StrTable[_T("TooMuchMoney")], _T("TooMuchMoney"));
				}
				else
				{
					if (P_VALID(GetObj("SellYuanBao")))
						KillObj("SellYuanBao");
					CreateObj("SellYuanBao", "MsgBox");
					TCHAR tbuff[100];
					_sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), g_StrTable[_T("YuanBaoTrade6")], m_nInputNum, nSumG, nSumS, nChargeG, nChargeS);
					TObjRef<MsgBox>("SellYuanBao")->Init(_T(""), tbuff, _T("SellYuanBao"));

					m_pWndFather->SetUnClickable(TRUE);
				}
			}
			else
			{
				// Jason 2010-4-7 v1.5.1
				INT yuanbaoAccount = 0;
				const tstring & temYuanbao = m_pStcYuanbao->GetText();
				if( !temYuanbao.empty() )
					yuanbaoAccount = _ttoi(temYuanbao.c_str());

				if( //m_nInputNum + CurrencyMgr::Inst()->GetBagYuanbao() > MAX_BAG_YUANBAO_NUM ||
					yuanbaoAccount + m_nInputNum > MAX_BAG_YUANBAO_NUM )
				{
					if (P_VALID(GetObj("TooMuchYuanBao")))
						KillObj("TooMuchYuanBao");
					CreateObj("TooMuchYuanBao", "MsgBox");
					TObjRef<MsgBox>("TooMuchYuanBao")->Init(_T(""), g_StrTable[_T("TooMuchYuanBao")], _T("TooMuchYuanBao"));
				}
				else
				{
					if (P_VALID(GetObj("BuyYuanBao")))
						KillObj("BuyYuanBao");
					CreateObj("BuyYuanBao", "MsgBox");
					TCHAR tbuff[100];
					_sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), g_StrTable[_T("YuanBaoTrade7")], m_nInputNum, nSumG, nSumS );
					TObjRef<MsgBox>("BuyYuanBao")->Init(_T(""), tbuff, _T("BuyYuanBao"));

					m_pWndFather->SetUnClickable(TRUE);
				}
			}
		}
		else if (pWnd == m_pBtMoneySave)
		{
			if (P_VALID(GetObj("SaveMoney")))
				KillObj("SaveMoney");
			CreateObj("SaveMoney", "MsgMoneyInputBox");
			TObjRef<MsgMoneyInputBox>("SaveMoney")->Init(_T(""), g_StrTable[_T("YuanBaoTrade0")], _T("SaveMoney"),  min(MAX_BAG_SILVER_NUM, CurrencyMgr::Inst()->GetBagSilver()), min(999999, CurrencyMgr::Inst()->GetBagYuanbao() ), 0, 0);

			m_pWndFather->SetUnClickable(TRUE);
		}
		else if (pWnd == m_pBtMoneyLoad)
		{
			if (P_VALID(GetObj("DepositMoney")))
				KillObj("DepositMoney");
			CreateObj("DepositMoney", "MsgMoneyInputBox");
			TObjRef<MsgMoneyInputBox>("DepositMoney")->Init(_T(""), g_StrTable[_T("YuanBaoTrade2")], _T("DepositMoney"), min(MAX_BAG_SILVER_NUM, YuanbaoTradeMgr::Inst()->GetMoneyInAccount()), min(999999, YuanbaoTradeMgr::Inst()->GetYuanBaoInAccount() ), 0, 0);

			m_pWndFather->SetUnClickable(TRUE);
		}
        else if (pWnd == m_pBtSalePrice)
        {
            // 出售单价排序

        }
        else if (pWnd == m_pBtSaleNum)
        {
            // 出售数量排序

        }
        else if (pWnd == m_pBtBuyPrice)
        {
            // 收购单价排序

        }
        else if (pWnd == m_pBtBuyNum)
        {
            // 收购数量排序

        }
        break;
    }

    return 0;
}

DWORD YuanbaoFrame::OrderQEventHandler( tagGUIEvent* pEvent )
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch(pEvent->eEvent)
    {
    case EGUIE_Click:
        if (pWnd == m_pOrderCannel)
        {
            // 撤销订单
            CanelOrder();
			 m_dwTime = 0;			//刷新消息发送时间,删除延迟5秒
			 if (P_VALID(m_pOrderCannel))
			 {
				 m_pOrderCannel->SetEnable(FALSE);
			 }
        }
        else if (pWnd == m_pOrderRefresh)
        {
            // 刷新订单
            UpdateOrder();
        }
        else if (pWnd == m_pOrderClose)
        {
            // 关闭
            ShowOrderQuery(FALSE);
        }
        break;
    }

    return 0;
}

VOID YuanbaoFrame::ShowOrderQuery( BOOL bVal )
{
    m_pOrderWnd->SetInvisible(!bVal);
    if (bVal)
    {
		if (P_VALID(m_pOrderCannel))
		{
			m_pOrderCannel->SetEnable(FALSE);
		}
        // 刷新订单
		BeginUpdateOrder();
        //YuanbaoTradeMgr::Inst()->BeginNC_GetYuanbaoOrder();
        UpdateOrder();
        m_pOrderWnd->FlipToTop();
        m_pOrderWnd->SetRefresh();
		
    }
	m_dwTime = 0;			//刷新消息发送时间 
	
}

DWORD YuanbaoFrame::OnYuanBaoTradeRet( tagYBReturnEvnt* pEvent )
{
    tstring str;

    switch (pEvent->eType)
    {
    case EYBR_SaveYB2Account:
		switch (pEvent->dwErrCode)
		{
		case E_Trade_BagYB_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr14")];
			break;
		case E_Trade_BagSilver_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr15")];
			break;
		case E_Trade_AccountYB_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr16")];
			break;
		case E_Trade_AccountSilver_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr17")];
			break;
		case E_Trade_SellOrder_Exit:
			str = g_StrTable[_T("YuanBaoTradeErr18")];
			break;
		case E_Trade_BuyOrder_Exit:
			str = g_StrTable[_T("YuanBaoTradeErr19")];
			break;
		case E_Trade_Tax_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr20")];
			break;
		case E_Trade_BagYB_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr21")];
			break;
		case E_Trade_BagSilver_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr22")];
			break;
		case E_Trade_AccountSilver_Full:
			str = g_StrTable[_T("YuanBaoTradeErr23")];
			break;
		case E_Trade_AccountYB_Full:
			str = g_StrTable[_T("YuanBaoTradeErr24")];
			break;
		case E_Trade_AmountOfTheTradeTooBig:
			str = g_StrTable[_T("YuanBaoTradeErr25")];
			break;
		default:
			str = pEvent->bRet ? g_StrTable[_T("YuanBaoTradeErr0")] : g_StrTable[_T("YuanBaoTradeErr1")];
		}
    	break;
    case EYBR_DepositAccountYB:
		switch (pEvent->dwErrCode)
		{
		case E_Trade_BagYB_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr14")];
			break;
		case E_Trade_BagSilver_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr15")];
			break;
		case E_Trade_AccountYB_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr16")];
			break;
		case E_Trade_AccountSilver_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr17")];
			break;
		case E_Trade_SellOrder_Exit:
			str = g_StrTable[_T("YuanBaoTradeErr18")];
			break;
		case E_Trade_BuyOrder_Exit:
			str = g_StrTable[_T("YuanBaoTradeErr19")];
			break;
		case E_Trade_Tax_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr20")];
			break;
		case E_Trade_BagYB_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr21")];
			break;
		case E_Trade_BagSilver_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr22")];
			break;
		case E_Trade_AccountSilver_Full:
			str = g_StrTable[_T("YuanBaoTradeErr23")];
			break;
		case E_Trade_AccountYB_Full:
			str = g_StrTable[_T("YuanBaoTradeErr24")];
			break;
		case E_Trade_AmountOfTheTradeTooBig:
			str = g_StrTable[_T("YuanBaoTradeErr25")];
			break;
		default:
			str = pEvent->bRet ? g_StrTable[_T("YuanBaoTradeErr2")] : g_StrTable[_T("YuanBaoTradeErr3")];
		}
        break;
    case EYBR_SaveSilver2Account:
		switch (pEvent->dwErrCode)
		{
		case E_Trade_BagYB_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr14")];
			break;
		case E_Trade_BagSilver_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr15")];
			break;
		case E_Trade_AccountYB_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr16")];
			break;
		case E_Trade_AccountSilver_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr17")];
			break;
		case E_Trade_SellOrder_Exit:
			str = g_StrTable[_T("YuanBaoTradeErr18")];
			break;
		case E_Trade_BuyOrder_Exit:
			str = g_StrTable[_T("YuanBaoTradeErr19")];
			break;
		case E_Trade_Tax_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr20")];
			break;
		case E_Trade_BagYB_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr21")];
			break;
		case E_Trade_BagSilver_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr22")];
			break;
		case E_Trade_AccountSilver_Full:
			str = g_StrTable[_T("YuanBaoTradeErr23")];
			break;
		case E_Trade_AccountYB_Full:
			str = g_StrTable[_T("YuanBaoTradeErr24")];
			break;
		case E_Trade_AmountOfTheTradeTooBig:
			str = g_StrTable[_T("YuanBaoTradeErr25")];
			break;
		default:
			str = pEvent->bRet ? g_StrTable[_T("YuanBaoTradeErr4")] : g_StrTable[_T("YuanBaoTradeErr5")];
		}
        break;
    case EYBR_DepositAccountSilver:
		switch (pEvent->dwErrCode)
		{
		case E_Trade_BagYB_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr14")];
			break;
		case E_Trade_BagSilver_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr15")];
			break;
		case E_Trade_AccountYB_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr16")];
			break;
		case E_Trade_AccountSilver_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr17")];
			break;
		case E_Trade_SellOrder_Exit:
			str = g_StrTable[_T("YuanBaoTradeErr18")];
			break;
		case E_Trade_BuyOrder_Exit:
			str = g_StrTable[_T("YuanBaoTradeErr19")];
			break;
		case E_Trade_Tax_NotEnough:
			str = g_StrTable[_T("YuanBaoTradeErr20")];
			break;
		case E_Trade_BagYB_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr21")];
			break;
		case E_Trade_BagSilver_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr22")];
			break;
		case E_Trade_AccountSilver_Full:
			str = g_StrTable[_T("YuanBaoTradeErr23")];
			break;
		case E_Trade_AccountYB_Full:
			str = g_StrTable[_T("YuanBaoTradeErr24")];
			break;
		case E_Trade_AmountOfTheTradeTooBig:
			str = g_StrTable[_T("YuanBaoTradeErr25")];
			break;
		default:
			str = pEvent->bRet ? g_StrTable[_T("YuanBaoTradeErr6")] : g_StrTable[_T("YuanBaoTradeErr7")];
		}
        break;
    case EYBR_SubmitSellOrder:
        str = pEvent->bRet ? g_StrTable[_T("YuanBaoTradeErr8")] : g_StrTable[_T("YuanBaoTradeErr9")];
        switch (pEvent->dwErrCode)
        {
        case E_Trade_BagYB_NotEnough:
            str = g_StrTable[_T("YuanBaoTradeErr14")];
        	break;
        case E_Trade_BagSilver_NotEnough:
            str = g_StrTable[_T("YuanBaoTradeErr15")];
            break;
        case E_Trade_AccountYB_NotEnough:
            str = g_StrTable[_T("YuanBaoTradeErr16")];
            break;
        case E_Trade_AccountSilver_NotEnough:
            str = g_StrTable[_T("YuanBaoTradeErr17")];
            break;
        case E_Trade_SellOrder_Exit:
            str = g_StrTable[_T("YuanBaoTradeErr18")];
            break;
        case E_Trade_BuyOrder_Exit:
            str = g_StrTable[_T("YuanBaoTradeErr19")];
            break;
        case E_Trade_Tax_NotEnough:
            str = g_StrTable[_T("YuanBaoTradeErr20")];
            break;
		case E_Trade_BagYB_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr21")];
			break;
		case E_Trade_BagSilver_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr22")];
			break;
		case E_Trade_AccountSilver_Full:
			str = g_StrTable[_T("YuanBaoTradeErr23")];
			break;
		case E_Trade_AccountYB_Full:
			str = g_StrTable[_T("YuanBaoTradeErr24")];
			break;
		case E_Trade_AmountOfTheTradeTooBig:
			str = g_StrTable[_T("YuanBaoTradeErr25")];
			break;
        }
        if (pEvent->bRet)
            BeginUpdateOrder();
        break;
    case EYBR_SubmitBuyOrder:
        str = pEvent->bRet ? g_StrTable[_T("YuanBaoTradeErr10")] : g_StrTable[_T("YuanBaoTradeErr11")];
        switch (pEvent->dwErrCode)
        {
        case E_Trade_BagYB_NotEnough:
            str = g_StrTable[_T("YuanBaoTradeErr14")];
            break;
        case E_Trade_BagSilver_NotEnough:
            str = g_StrTable[_T("YuanBaoTradeErr15")];
            break;
        case E_Trade_AccountYB_NotEnough:
            str = g_StrTable[_T("YuanBaoTradeErr16")];
            break;
        case E_Trade_AccountSilver_NotEnough:
            str = g_StrTable[_T("YuanBaoTradeErr17")];
            break;
        case E_Trade_SellOrder_Exit:
            str = g_StrTable[_T("YuanBaoTradeErr18")];
            break;
        case E_Trade_BuyOrder_Exit:
            str = g_StrTable[_T("YuanBaoTradeErr19")];
            break;
        case E_Trade_Tax_NotEnough:
            str = g_StrTable[_T("YuanBaoTradeErr20")];
            break;
		case E_Trade_BagYB_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr21")];
			break;
		case E_Trade_BagSilver_WillBeFull:
			str = g_StrTable[_T("YuanBaoTradeErr22")];
			break;
		case E_Trade_AccountSilver_Full:
			str = g_StrTable[_T("YuanBaoTradeErr23")];
			break;
		case E_Trade_AccountYB_Full:
			str = g_StrTable[_T("YuanBaoTradeErr24")];
			break;
		case E_Trade_AmountOfTheTradeTooBig:
			str = g_StrTable[_T("YuanBaoTradeErr25")];
			break;
        }
        if (pEvent->bRet)
            BeginUpdateOrder();
        break;
    case EYBR_GetYuanbaoOrder:
        UpdateOrder();
        return 0;
        break;
    case EYBR_DeleteOrder:
        str = pEvent->bRet ? g_StrTable[_T("YuanBaoTradeErr12")] : g_StrTable[_T("YuanBaoTradeErr13")];
		 // 刷新订单
		 BeginUpdateOrder();
        break;
    }

    UpdateYuanbaoTrade();
    CombatSysUtil::Inst()->ShowScreenCenterMsgEx(str.c_str());

    return 0;
}

DWORD YuanbaoFrame::OnSaveMoney( tagMsgMoneyBoxEvent* pEvent )
{
    if (MBF_OK == pEvent->eResult)
    {
        YuanbaoTradeMgr::Inst()->BeginNC_SaveSilver2Account(pEvent->n64Silver);
		YuanbaoTradeMgr::Inst()->BeginNC_SaveYB2Account(pEvent->nYuanbao);
    }

    m_pWndFather->SetUnClickable(FALSE);

    return 0;
}

DWORD YuanbaoFrame::OnDepositMoney( tagMsgMoneyBoxEvent* pEvent )
{
    if (MBF_OK == pEvent->eResult)
    {
        YuanbaoTradeMgr::Inst()->BeginNC_DepositAccountSilver(pEvent->n64Silver);
		YuanbaoTradeMgr::Inst()->BeginNC_DepositAccountYB(pEvent->nYuanbao);
    }

    m_pWndFather->SetUnClickable(FALSE);

    return 0;
}

INT64 YuanbaoFrame::CalSum( INT64 nGold, INT64 nSilver, INT nNum )
{
    return (MGold2Silver(nGold) + nSilver) * nNum;
}

INT64 YuanbaoFrame::CalCharge( INT64 nGold, INT64 nSilver, INT nNum )
{
	return (INT64)(CalSum(nGold, nSilver, nNum) * 0.05) > 0 ? (INT64)(CalSum(nGold, nSilver, nNum) * 0.05) : 1;
}

DWORD YuanbaoFrame::OnSellYuanBao( tagMsgBoxEvent* pEvent )
{
    if (MBF_OK == pEvent->eResult)
    {
        INT64 nMoney    =   MGold2Silver(m_nInputGold) + m_nInputSilver;
        YuanbaoTradeMgr::Inst()->BeginNC_SubmitSellOrder(m_nInputNum, nMoney);
		BeginUpdateOrder();
		ShowOrderQuery( FALSE );
		//YuanbaoTradeMgr::Inst()->BeginNC_GetYuanbaoOrder();
		m_bSellFlag = true;
    }

    m_pWndFather->SetUnClickable(FALSE);

    return 0;
}

DWORD YuanbaoFrame::OnBuyYuanBao( tagMsgBoxEvent* pEvent )
{
    if (MBF_OK == pEvent->eResult)
    {
		INT64 nMoney    =   MGold2Silver(m_nInputGold) + m_nInputSilver;
		YuanbaoTradeMgr::Inst()->BeginNC_SubmitBuyOrder(m_nInputNum, nMoney);
		BeginUpdateOrder();
		ShowOrderQuery( FALSE );
		//YuanbaoTradeMgr::Inst()->BeginNC_GetYuanbaoOrder();
		m_bBuyFlag = true;
    }

    m_pWndFather->SetUnClickable(FALSE);

    return 0;
}

DWORD YuanbaoFrame::OnTooMuchMoney( tagMsgBoxEvent* pEvent )
{
	if (MBF_OK == pEvent->eResult)
	{
		INT64   nSum        =   CalSum(m_nInputGold, m_nInputSilver, m_nInputNum);
		INT     nSumG       =   MSilver2DBGold(nSum);
		INT     nSumS       =   MSilver2DBSilver(nSum);
		INT64   nCharge     =   CalCharge(m_nInputGold, m_nInputSilver, m_nInputNum);
		INT     nChargeG    =   MSilver2DBGold(nCharge);
		INT     nChargeS    =   MSilver2DBSilver(nCharge);

		if (P_VALID(GetObj("SellYuanBao")))
			KillObj("SellYuanBao");
		CreateObj("SellYuanBao", "MsgBox");
		TCHAR tbuff[100];
		_sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), g_StrTable[_T("YuanBaoTrade6")], m_nInputNum, nSumG, nSumS, nChargeG, nChargeS);
		TObjRef<MsgBox>("SellYuanBao")->Init(_T(""), tbuff, _T("SellYuanBao"));

		m_pWndFather->SetUnClickable(TRUE);
	}

	m_pWndFather->SetUnClickable(FALSE);

	return 0;
}

DWORD YuanbaoFrame::OnTooMuchYuanBao( tagMsgBoxEvent* pEvent )
{
	if (MBF_OK == pEvent->eResult)
	{
		INT64   nSum        =   CalSum(m_nInputGold, m_nInputSilver, m_nInputNum);
		INT     nSumG       =   MSilver2DBGold(nSum);
		INT     nSumS       =   MSilver2DBSilver(nSum);

		if (P_VALID(GetObj("BuyYuanBao")))
			KillObj("BuyYuanBao");
		CreateObj("BuyYuanBao", "MsgBox");
		TCHAR tbuff[100];
		_sntprintf(tbuff, sizeof(tbuff)/sizeof(TCHAR), g_StrTable[_T("YuanBaoTrade7")], m_nInputNum, nSumG, nSumS );
		TObjRef<MsgBox>("BuyYuanBao")->Init(_T(""), tbuff, _T("BuyYuanBao"));

		m_pWndFather->SetUnClickable(TRUE);
	}

	m_pWndFather->SetUnClickable(FALSE);

	return 0;
}

VOID YuanbaoFrame::UpdateYuanbaoTrade()
{
    // 刷新出售list
    UpdateSellList();
    // 刷新收购list
    UpdateBuyList();
    // 刷新我要出售/收购
    ResetInput();
    // 刷新交易账户
    UpdateYBAccount();
}

VOID YuanbaoFrame::UpdateSellList()
{
    // 获得当前列表
    const YuanbaoTradeMgr::_MapPrice* pMapList = YuanbaoTradeMgr::Inst()->GetPrice2SellList();
    YuanbaoTradeMgr::_MapPrice::const_iterator cit;
    
    // 重置控件
    ResetSellList();

    // 填充数据
    INT i = 0;
    for (cit = pMapList->begin(); cit != pMapList->end(); ++cit)
    {
        INT nGold   =   MSilver2DBGold(cit->second->nPrice);
        INT nSilver =   MSilver2DBSilver(cit->second->nPrice);
        INT nNum    =   cit->second->nNum;
        
		if( YuanbaoTradeMgr::Inst()->IsInOrderList( cit->second->nPrice, EYBOT_SELL ) )
		{
			m_bSellFlag = false;
			tagRect rcRect(0, 0, 18, 18);
			m_pLBSale->SetItemPic( i, 0, _T("data\\ui\\mall\\L_xiaoren.bmp"), rcRect);  // 自己参与的
		}
        TCHAR   tbuff[30];
        size_t  count   =   sizeof(tbuff) / sizeof(TCHAR);
        _sntprintf(tbuff, count, _T("%d %s"), nGold,    g_StrTable[_T("YuanBaoTrade10")]);
        m_pLBSale->SetText(i, 1, tbuff);
        _sntprintf(tbuff, count, _T("%d %s"), nSilver,  g_StrTable[_T("YuanBaoTrade11")]);
        m_pLBSale->SetText(i, 2, tbuff);
		m_pLBSale->SetText(i, 3, g_StrTable[_T("YuanBao_sell")]);
        _sntprintf(tbuff, count, _T("%d %s"), nNum,     g_StrTable[_T("YuanBaoTrade12")]);
        m_pLBSale->SetText(i, 4, tbuff);

        // 下一行
        ++i;
    }
}

VOID YuanbaoFrame::UpdateBuyList()
{
    // 获得当前列表
    const YuanbaoTradeMgr::_mapBuy* pMapList = YuanbaoTradeMgr::Inst()->GetPrice2BuyList();
    YuanbaoTradeMgr::_mapBuy::const_iterator cit;

    // 重置控件
    ResetBuyList();

    // 填充数据
    INT i = 0;
    for (cit = pMapList->begin(); cit != pMapList->end(); ++cit)
    {
        INT nGold   =   MSilver2DBGold(cit->second->nPrice);
        INT nSilver =   MSilver2DBSilver(cit->second->nPrice);
        INT nNum    =   cit->second->nNum;

		if( YuanbaoTradeMgr::Inst()->IsInOrderList( cit->second->nPrice, EYBOT_BUY ) )
		{
			m_bBuyFlag = false;
			tagRect rcRect(0, 0, 18, 18);
			m_pLBBuy->SetItemPic( i, 0, _T("data\\ui\\mall\\L_xiaoren.bmp"), rcRect);  // 自己参与的
		}
        TCHAR   tbuff[30];
        size_t  count   =   sizeof(tbuff) / sizeof(TCHAR);
        _sntprintf(tbuff, count, _T("%d %s"), nGold,    g_StrTable[_T("YuanBaoTrade10")]);
        m_pLBBuy->SetText(i, 1, tbuff);
        _sntprintf(tbuff, count, _T("%d %s"), nSilver,  g_StrTable[_T("YuanBaoTrade11")]);
        m_pLBBuy->SetText(i, 2, tbuff);
		m_pLBBuy->SetText(i, 3, g_StrTable[_T("YuanBao_buy")]);
        _sntprintf(tbuff, count, _T("%d %s"), nNum,     g_StrTable[_T("YuanBaoTrade12")]);
        m_pLBBuy->SetText(i, 4, tbuff);

        // 下一行
        ++i;
    }
}

VOID YuanbaoFrame::ResetInput()
{
    m_pEdtWannaPriceGold->SetText(_T(""));
    m_pEdtWannaPriceSilver->SetText(_T(""));
    m_pEdtWannaNum->SetText(_T(""));
	m_pStcTotalGold->SetText(_T(""));
	m_pStcTotalSilver->SetText(_T(""));
	m_pStcTaxGold->SetText(_T(""));
	m_pStcTaxSilver->SetText(_T(""));

}

VOID YuanbaoFrame::UpdateYBAccount()
{
    // 获得当前元宝交易账户余额
    INT nGold       =   MSilver2DBGold(YuanbaoTradeMgr::Inst()->GetMoneyInAccount());
    INT nSilver     =   MSilver2DBSilver(YuanbaoTradeMgr::Inst()->GetMoneyInAccount());
    INT nYuanBao    =   YuanbaoTradeMgr::Inst()->GetYuanBaoInAccount();

    // 显示到界面
    TCHAR tbuff[30];
    _itot(nGold, tbuff, 10);
    m_pStcAcctGold->SetText(tbuff);
    m_pStcAcctGold->SetRefresh(TRUE);
    _itot(nSilver, tbuff, 10);
    m_pStcAcctSilver->SetText(tbuff);
    m_pStcAcctSilver->SetRefresh(TRUE);
    _itot(nYuanBao, tbuff, 10);
    m_pStcYuanbao->SetText(tbuff);
    m_pStcYuanbao->SetRefresh(TRUE);
}

VOID YuanbaoFrame::UpdateOrder()
{
    // 如果订单窗体未打开，则不刷新
    if (m_pOrderWnd->IsInvisible())
        return;

    // 获得订单列表
    const YuanbaoTradeMgr::_MapOrder* pOrderList = YuanbaoTradeMgr::Inst()->GetID2OrderList();
    YuanbaoTradeMgr::_MapOrder::const_reverse_iterator cit;

    // 重置控件
    ResetOrderList();

    // 显示到界面
    INT i = 0;
    for (cit = pOrderList->rbegin(); cit != pOrderList->rend(); ++cit)
    {
		DWORD dwColor = GetOrderColor(cit->second->eYBOMode);
        TCHAR tbuff[100];
        size_t count = sizeof(tbuff) / sizeof(TCHAR);

        _sntprintf(tbuff, count, _T("%d"), cit->second->dwID);
        m_pOrderTable->SetText(i, 0, tbuff, dwColor );

        m_pOrderTable->SetText(i, 1, GetTradeType(cit->second->eYBOType), dwColor );

        _sntprintf(tbuff, count, _T("%d%s"), MSilver2DBGold(cit->second->nPrice), g_StrTable[_T("YuanBaoTrade10")]);
        m_pOrderTable->SetText(i, 2, tbuff, dwColor );

        _sntprintf(tbuff, count, _T("%d%s"), MSilver2DBSilver(cit->second->nPrice), g_StrTable[_T("YuanBaoTrade11")]);
        m_pOrderTable->SetText(i, 3, tbuff, dwColor );

        _sntprintf(tbuff, count, _T("%d%s"), cit->second->nNum, g_StrTable[_T("YuanBaoTrade12")]);
        m_pOrderTable->SetText(i, 4, tbuff, dwColor );

        m_pOrderTable->SetText(i, 5,  GetOrderStus(cit->second->eYBOMode), dwColor );

        tagDWORDTime bgtime = cit->second->dwStartTime;
        _sntprintf(tbuff, count, _T("%02d-%02d-%02d %02d:%02d"), bgtime.year, bgtime.month, bgtime.day, bgtime.hour, bgtime.min);
        m_pOrderTable->SetText(i, 6, tbuff, dwColor );
        
        tagDWORDTime edtime = cit->second->dwEndTime;
        _sntprintf(tbuff, count, _T("%02d-%02d-%02d %02d:%02d"), edtime.year, edtime.month, edtime.day, edtime.hour, edtime.min);
        m_pOrderTable->SetText(i, 7, tbuff, dwColor );
        if (EYBOM_Submit == cit->second->eYBOMode)
            m_pOrderTable->SetText(i, 7, _T("-"), dwColor );

        _sntprintf(tbuff, count, _T("%d%s"), MSilver2DBGold(cit->second->nAvgPrice), g_StrTable[_T("YuanBaoTrade10")]);
        m_pOrderTable->SetText(i, 8, tbuff, dwColor );

        _sntprintf(tbuff, count, _T("%d%s"), MSilver2DBSilver(cit->second->nAvgPrice), g_StrTable[_T("YuanBaoTrade11")]);
        m_pOrderTable->SetText(i, 9, tbuff, dwColor );

        _sntprintf(tbuff, count, _T("%d%s"), cit->second->nDealNum, g_StrTable[_T("YuanBaoTrade12")]);
        m_pOrderTable->SetText(i, 10, tbuff, dwColor );

        // 插入对照表
        m_mapRow2OrderID.insert(make_pair(i, cit->second->dwID));

        // 下一行
        ++i;
    }
}

VOID YuanbaoFrame::InitListBox()
{
    ResetSellList();
    ResetBuyList();
    ResetOrderList();
}

VOID YuanbaoFrame::ResetSellList()
{
    // 寄售列表
    m_pLBSale->Clear();
    m_pLBSale->SetColNum(YB_SELL_LIST_COL,50);
	m_pLBSale->SetColWidth(0, 25);
    m_pLBSale->SetColWidth(1, 55);
    m_pLBSale->SetColWidth(2, 55);
	m_pLBSale->SetColWidth(3, 40 );
    m_pLBSale->SetColWidth(4, 50);
}

VOID YuanbaoFrame::ResetBuyList()
{
    // 出售列表
    m_pLBBuy->Clear();
    m_pLBBuy->SetColNum(YB_BUY_LIST_COL, 50);
	m_pLBBuy->SetColWidth(0, 25);
	m_pLBBuy->SetColWidth(1, 55);
	m_pLBBuy->SetColWidth(2, 55);
	m_pLBBuy->SetColWidth(3, 40 );
	m_pLBBuy->SetColWidth(4, 50);
}

VOID YuanbaoFrame::ResetOrderList()
{
    // 订单列表
    m_pOrderTable->Clear();
    m_pOrderTable->SetColNum(YB_ORDER_LIST_COL, 100);
    m_pOrderTable->SetColWidth(0, m_pOrderID->GetSize().x);
    m_pOrderTable->SetColWidth(1, m_pOrderType->GetSize().x);
    m_pOrderTable->SetColWidth(2, m_pOrderPrice->GetSize().x / 2);
    m_pOrderTable->SetColWidth(3, m_pOrderPrice->GetSize().x / 2);
    m_pOrderTable->SetColWidth(4, m_pOrderNum->GetSize().x);
    m_pOrderTable->SetColWidth(5, m_pOrderStus->GetSize().x);
    m_pOrderTable->SetColWidth(6, m_pOrderTimeFrom->GetSize().x);
    m_pOrderTable->SetColWidth(7, m_pOrderTimeTo->GetSize().x);
    m_pOrderTable->SetColWidth(8, m_pOrderAveragePrice->GetSize().x / 2);
    m_pOrderTable->SetColWidth(9, m_pOrderAveragePrice->GetSize().x / 2);
    m_pOrderTable->SetColWidth(10, m_pOrderFinalNum->GetSize().x);

    // 清空对照表
    m_mapRow2OrderID.clear();
}

VOID YuanbaoFrame::CanelOrder()
{
    INT nSelRow     =   m_pOrderTable->GetCurSelectedRow();
    map<INT, DWORD>::const_iterator cit = m_mapRow2OrderID.find(nSelRow);

    if (cit != m_mapRow2OrderID.end())
    {
        const YuanbaoTradeMgr::_MapOrder* pOrders = YuanbaoTradeMgr::Inst()->GetID2OrderList();
        YuanbaoTradeMgr::_MapOrder::const_iterator citOr = pOrders->find(cit->second);
        if (citOr != pOrders->end())
        {
                YuanbaoTradeMgr::Inst()->BeginNC_DeleteOrder(cit->second, citOr->second->eYBOType);
        }
    }
}

VOID YuanbaoFrame::BeginUpdateOrder()
{
    // 如果订单窗体未打开，则不刷新
   /* if (m_pOrderWnd->IsInvisible())
        return;*/
	m_dwTime = 0;			//刷新消息发送时间
    YuanbaoTradeMgr::Inst()->BeginNC_GetYuanbaoOrder();

}

BOOL YuanbaoFrame::EscCancel()
{
    BOOL ret = FALSE;

    if (m_pOrderWnd->IsInvisible())
    {
    }
    else
    {
        ShowOrderQuery(FALSE);
        ret = TRUE;
    }

    return ret;
}

DWORD YuanbaoFrame::GetOrderColor(EYBOMODE e)
{
	switch (e)
	{
	case EYBOM_Submit:
		return 0xFF00FF00;
	case EYBOM_Cancel:
		return 0xFF808080;
	default:
		return 0xFFFFFFFF;
	}
	return 0xFFFFFFFF;
}

VOID YuanbaoFrame::CalcTradeMoney()
{
	m_pStcTotalGold->SetText(_T(""));
	m_pStcTotalSilver->SetText(_T(""));
	m_pStcTaxGold->SetText(_T(""));
	m_pStcTaxSilver->SetText(_T(""));

	m_nInputGold        =   _ttoi(m_pEdtWannaPriceGold->GetText());
	m_nInputSilver      =   _ttoi(m_pEdtWannaPriceSilver->GetText());
	m_nInputNum         =   _ttoi(m_pEdtWannaNum->GetText());

	INT64   nSum        =   CalSum(m_nInputGold, m_nInputSilver, m_nInputNum);
	INT     nSumG       =   MSilver2DBGold(nSum);
	INT     nSumS       =   MSilver2DBSilver(nSum);
	INT64   nCharge     =   CalCharge(m_nInputGold, m_nInputSilver, m_nInputNum);
	INT     nChargeG    =   MSilver2DBGold(nCharge);
	INT     nChargeS    =   MSilver2DBSilver(nCharge);

	if( nSum > 0 )
	{
		TCHAR szTmp[X_SHORT_NAME] = {0};
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nSumG );
		m_pStcTotalGold->SetText(szTmp);
		m_pStcTotalGold->SetRefresh(true);

		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nSumS );
		m_pStcTotalSilver->SetText(szTmp);
		m_pStcTotalSilver->SetRefresh(true);

		if( m_bisSell )
		{
			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nChargeG );
			m_pStcTaxGold->SetText(szTmp);
			m_pStcTaxGold->SetRefresh(true);

			_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), nChargeS );
			m_pStcTaxSilver->SetText(szTmp);
			m_pStcTaxSilver->SetRefresh(true);
		}
	}
}

VOID YuanbaoFrame::DestroyObjects()
{
	if (P_VALID(GetObj("TooMuchYuanBao")))
		KillObj("TooMuchYuanBao");
	if (P_VALID(GetObj("BuyYuanBao")))
		KillObj("BuyYuanBao");
	if (P_VALID(GetObj("SellYuanBao")))
		KillObj("SellYuanBao");
	if (P_VALID(GetObj("SaveMoney")))
		KillObj("SaveMoney");
	if (P_VALID(GetObj("DepositMoney")))
		KillObj("DepositMoney");
}
