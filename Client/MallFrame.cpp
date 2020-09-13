#include "stdafx.h"
#include "MallFrame.h"
#include "NetSession.h"
#include "IconStatic.h"
#include "ItemButton.h"
#include "PlayerTryDressWnd.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "..\WorldDefine\ItemDefine.h"
#include "ItemProtoData.h"
#include "MallMgr.h"
#include "RoleMgr.h"
#include "CurrencyMgr.h"
#include "MallData.h"
#include "MallGiftFrame.h"
#include "PlayerNameTab.h"
#include "CombatSysUtil.h"
#include "ServerTime.h"
#include "YuanbaoTradeMgr.h"
#include "GroupBuyFrame.h"
#include "shellapi.h"
#include "HttpPicEx.h"
#include "SaveAccount.h"
#include "..\WorldDefine\currency_define.h"
#include "GlintPushButton.h"
#include "LevelPromoteFrame.h"
#include "..\WorldDefine\login.h"
#include "SaveAccount.h"
#include "ItemMgr.h"
#include "Container.h"

//const TCHAR *szPromoteHave	= _T("data\\ui\\mall\\L_cu.bmp");
//const TCHAR *szPromoteNot	= _T("data\\ui\\mall\\L_cu_daizhong.bmp");
//const TCHAR *szGroupHave	= _T("data\\ui\\mall\\L_tuan.bmp");
//const TCHAR *szGroupNot		= _T("data\\ui\\mall\\L_tuan_daizhong.bmp");
//const TCHAR *szPresentHave	= _T("data\\ui\\mall\\L_zeng.bmp");
//const TCHAR *szPresentNot	= _T("data\\ui\\mall\\L_zeng_daizhong.bmp");
//const TCHAR *szNumHave		= _T("data\\ui\\mall\\L_xian.bmp");
//const TCHAR *szNumNot		= _T("data\\ui\\mall\\L_xian_daizhong.bmp");
//const TCHAR *szExchangeHove	= _T("data\\ui\\mall\\L_dui.bmp");
//const TCHAR *szExchangeNot	= _T("data\\ui\\mall\\L_dui_daizhong.bmp");
//
//const TCHAR *szSelectItem	= _T("data\\ui\\Common\\L_icon-l.bmp");
//const TCHAR *szSelectPack	= _T("data\\ui\\mall\\L_kuangjia-d-L.bmp");
const TCHAR *szDeletePic	= _T("data\\ui\\mall\\L-mall08.bmp");
const TCHAR *szSelectItem	= _T("data\\ui\\Common\\l_icon.bmp");
const TCHAR *szPickItem		= _T("data\\ui\\Common\\L_icon-l2.bmp");
const TCHAR *szUnpickItem	= _T("data\\ui\\mall\\L-mall07.bmp");
const TCHAR *szSelectPack	= _T("data\\ui\\Common\\L_icon-l2.bmp");
const TCHAR *szUnselectPack	= _T("data\\ui\\mall\\L-mall5.bmp");
const TCHAR *SELECT_DIBAN	= _T("data\\ui\\sys\\l_diban-a.bmp");
const TCHAR *szState_New	= _T("data\\ui\\mall\\L-a1.bmp");
const TCHAR *szState_Hot	= _T("data\\ui\\mall\\L-b1.bmp");
const TCHAR *szState_Dis	= _T("data\\ui\\mall\\L-c1.bmp");
const TCHAR *szState_Lim	= _T("data\\ui\\mall\\L-d1.bmp");
#define MAX_PRESENT_ITEM_TIME 2000

MallFrame::MallFrame( void ):m_Trunk(this)
{
	m_pWnd					= NULL;
	m_pWndCaption			= NULL;		
	m_pWndTryDress			= NULL;							
	m_pStcYuanBao			= NULL;	
	m_pStcGold				= NULL;
	m_pStcSilver			= NULL;
	m_pExchangeVolume		= NULL;
	//m_pPBtnGroupBuy			= NULL;
	m_pBtnClose				= NULL;							
	m_pPBtnYuanBaoTrade		= NULL;						
	m_pBtnYuanBaoAbundance	= NULL;					
	m_pBtnReset				= NULL;							

	m_pStcFetchBack			= NULL;
	m_pStcFetchSel			= NULL;							
	m_pFetchUnitItem		= NULL;							
	m_pBtnFetch				= NULL;							
	m_pBtnFetchTitle		= NULL;		
	m_pStcGoodsBack			= NULL;
	m_pStcGoodsBackEx		= NULL;
	for(int i=0; i<MALL_MAXITEM_PERPAGE; i++)
	{
		m_pUnitItemBack[i]	= NULL;
		m_pUnitItemSel[i]	= NULL;
		m_pUnitItem[i]		= NULL;
		m_pStcItemName[i]	= NULL;
		m_pStcInfo[i]		= NULL;
		for(int j=0; j<MALL_ITEM_STATE; j++)
		{
			m_pStcState[i][j] = NULL;
		}
	}
	m_pBtnPDItem			= NULL;
	m_pBtnPUItem			= NULL;
	m_pStcCPItem			= NULL;
	m_pBtnPDPack			= NULL;
	m_pBtnPUPack			= NULL;
	m_pBtnCPPack			= NULL;
	m_pBtnBuy				= NULL;
	m_pBtnPresent			= NULL;
	//m_pBtnGroupBuy			= NULL;
	//m_pBtnExchange			= NULL;
	m_pStcProBack			= NULL;
	for(int i=0; i<MALL_MAX_PROMOTE_ITEM; i++)
	{
		m_pProItemBack[i]	= NULL;
		m_pUnitProItemSel[i] = NULL;
		m_pUnitProItem[i]	= NULL;
		m_pProItemName[i]	= NULL;
		//m_pProItmeInfo[i]	= NULL;
	}
	for (int i=0;i<MALL_MAX_PROMOTE_ITEM_PerPage;++i)
	{
		for(int j=0; j<MALL_PROMOTE_ITEM_STATE; j++)
		{
			m_pProItemState[i][j] = NULL;
			m_pProItemStateInv[i][j] = NULL;
		}
	}
	m_pPBtnAll				= NULL;
	m_pPBtnPackage			= NULL;
	for(int i=0; i<MALL_ITEM_SUBTYPE-2; i++)
	{
		m_pPBtnSubMall[i]	= NULL;
	}
	m_pPBtnPresent = NULL;
	m_pPBtnMark = NULL;
	m_pPBtnMall				= NULL;
	m_dwSelectItemID		= GT_INVALID;
	m_n16BuyNum				= GT_INVALID;
	m_bSelectPromote		= false;
	//m_pStcGroupBuyBack		= NULL;
	m_eOperateState			= EMIOS_NULL;
	//m_pWndNotice			= NULL;
	//m_pHttp_MallNotice		= NULL;

	for( int i = 0; i < MALL_DAY_PART; i++ )
	{
		m_pStcDyeTips[i] = NULL;
		m_pCBDayFation[i] = NULL;
		m_dwTryDressID[i] = GT_INVALID;
	}
	m_nPolicy = 0;
	m_pStcProPageText = 0;						// 推荐商品页数显示	
	m_pBtnFormerPage = 0;						// 推荐商品向前换页
	m_pBtnLatterPage = 0;						// 推荐商品向后换页
	m_iPromoteItemShowPage = 1;
	m_eShopingDistrict = Shopping_Item;
	m_eClickType = EMIT_All;
	m_bLevelLimit = TRUE;
	m_n8SecondMenu = 0;
	m_bIsExterior = FALSE;
	m_nCurMenu = 0;
	m_nCurSubMenu = 0;

	for (int i=0; i<MALL_ITEM_SUBTYPE; ++i)
	{
		for (int j=0; j<MALL_ITEM_SECOND_SUBTYPE; ++j)
		{
			m_nCurPage[i][j] = 0;
			m_nCurMaxPage[i][j] = 0;
		}
	}

	for (int k=0; k<SPECIAL_TYPE; ++k)
	{
		m_nSpecialPage[k] = 0;			// 全部:0 打包:1
		m_nSpecialMaxPage[k] = 0;
	}

	for (int l=0; l<LEVEL_NUM_CHOOSE; ++l)
	{
	    m_pGPBLevel[l] = NULL;
	}
	
	for(int i = 0; i < Mall_ShopingCart_Num; i++)
	{
		m_pShopingItem[i] = NULL;
	}
	for(int i = 0; i < EMCA_End; i++)
	{
		m_dwMallCfg[i] = 0;
	}
}

MallFrame::~MallFrame( void )
{

}

BOOL MallFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam );

	//--注册游戏事件
	m_pMgr->RegisterEventHandle(_T("BuyMallItemNum_Event"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnBuyMallItemInputEvent));
	m_pMgr->RegisterEventHandle(_T("BuyMallItemPromoteNum_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnBuyMallPromoteItemInputEvet));
	m_pMgr->RegisterEventHandle(_T("AffirmBuyMallItem_Event"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnAffirmBuyMallItemEvent));
	m_pMgr->RegisterEventHandle(_T("tagMsgMallGiftEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnMsgMallGiftEvent));
	m_pMgr->RegisterEventHandle(_T("Bag_Yuanbao"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnBagYuanbaoChange));
	m_pMgr->RegisterEventHandle(_T("Bag_Silver"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnBagMoneyChange));
	m_pMgr->RegisterEventHandle(_T("tagExchangeVolumeChange"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnExchangeVolumeChange));
	m_pMgr->RegisterEventHandle(_T("tagMarkChange"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnMarkChange));
	m_pMgr->RegisterEventHandle(_T("ExchangePackItemNum_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnExchangePackItem));
	m_pMgr->RegisterEventHandle(_T("ExchangeMallItemNum_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnExchangeMallItem));
	
	m_pMgr->RegisterEventHandle(_T("AffirmExchangePackItem_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnAffirmExchangePackItem));
	m_pMgr->RegisterEventHandle(_T("AffirmExchangeMallItem_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnAffirmExchangeMallItem));
	m_pMgr->RegisterEventHandle(_T("UpdateMallDataEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnNS_MallUpdate));
	m_pMgr->RegisterEventHandle(_T("AddToShopingCart_Event"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnAddToShopingCart));
	m_pMgr->RegisterEventHandle(_T("ConfirmShopingCart_Event"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnConfirmShopingCart_Event));
	//--注册网络消息
	m_pCmdMgr->Register("NS_MallBuyItem",			(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallBuyItem),		_T("NS_MallBuyItem"));
	m_pCmdMgr->Register("NS_MallFreeGetItem",		(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallFreeGetItem),	_T("NS_MallFreeGetItem"));
	m_pCmdMgr->Register("NS_MallPresentItem",		(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallPresentItem),	_T("NS_MallPresentItem"));
	m_pCmdMgr->Register("NS_MallBuyPack",			(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallBuyPack),		_T("NS_MallBuyPack"));
	m_pCmdMgr->Register("NS_MallPresentPack",		(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallPresentPack),	_T("NS_MallPresentPack"));
	m_pCmdMgr->Register("NS_MallItemExchange",		(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallItemExchange),_T("NS_MallItemExchange"));

	m_pCmdMgr->Register("NS_MallUpdateCfg",			(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallUpdateCfg), _T("NS_MallUpdateCfg"));
	m_pCmdMgr->Register("NS_MallShoppingCart",		(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallShoppingCart), _T("NS_MallShoppingCart"));
	
	m_nCurMenu = EMIT_NULL;
	m_nCurSubMenu = 0;
	return bRet;
}

BOOL MallFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	//--注销游戏事件
	m_pMgr->UnRegisterEventHandler(_T("BuyMallItemNum_Event"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnBuyMallItemInputEvent));
	m_pMgr->UnRegisterEventHandler(_T("BuyMallItemPromoteNum_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnBuyMallPromoteItemInputEvet));
	m_pMgr->UnRegisterEventHandler(_T("AffirmBuyMallItem_Event"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnAffirmBuyMallItemEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagMsgMallGiftEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnMsgMallGiftEvent));
	m_pMgr->UnRegisterEventHandler(_T("Bag_Yuanbao"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnBagYuanbaoChange));
	m_pMgr->UnRegisterEventHandler(_T("Bag_Silver"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnBagMoneyChange));
	m_pMgr->UnRegisterEventHandler(_T("tagExchangeVolumeChange"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnExchangeVolumeChange));
	m_pMgr->UnRegisterEventHandler(_T("tagMarkChange"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnMarkChange));
	m_pMgr->UnRegisterEventHandler(_T("ExchangePackItemNum_Event"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnExchangePackItem));
	m_pMgr->UnRegisterEventHandler(_T("ExchangeMallItemNum_Event"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnExchangeMallItem));
	m_pMgr->UnRegisterEventHandler(_T("AffirmExchangePackItem_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnAffirmExchangePackItem));
	m_pMgr->UnRegisterEventHandler(_T("AffirmExchangeMallItem_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnAffirmExchangeMallItem));
	m_pMgr->UnRegisterEventHandler(_T("UpdateMallDataEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnNS_MallUpdate));
	m_pMgr->UnRegisterEventHandler(_T("AddToShopingCart_Event"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnAddToShopingCart));
	m_pMgr->UnRegisterEventHandler(_T("ConfirmShopingCart_Event"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallFrame::OnConfirmShopingCart_Event));
	//--注销网络消息
	m_pCmdMgr->UnRegister("NS_MallBuyItem",			(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallBuyItem));
	m_pCmdMgr->UnRegister("NS_MallFreeGetItem",		(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallFreeGetItem));
	m_pCmdMgr->UnRegister("NS_MallPresentItem",		(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallPresentItem));
	m_pCmdMgr->UnRegister("NS_MallBuyPack",			(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallBuyPack));
	m_pCmdMgr->UnRegister("NS_MallPresentPack",		(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallPresentPack));
	m_pCmdMgr->UnRegister("NS_MallItemExchange",	(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallItemExchange));
	m_pCmdMgr->UnRegister("NS_MallPackExchange",	(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallPackExchange));

	m_pCmdMgr->UnRegister("NS_MallUpdateCfg",		(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallUpdateCfg));
	m_pCmdMgr->UnRegister("NS_MallShoppingCart",	(NETMSGPROC)m_Trunk.sfp2(&MallFrame::OnNS_MallShoppingCart));


	//--防止出错
	if( P_VALID(GetObj("BuyMallItemNum_MsgBox")) )
		KillObj("BuyMallItemNum_MsgBox");
	if( P_VALID(GetObj("BuyMallPromoteItemNum_MsgBox")) )
		KillObj("BuyMallPromoteItemNum_MsgBox");
	if( P_VALID(GetObj("AffirmBuyMallItem_MsgBox")) )
		KillObj("AffirmBuyMallItem_MsgBox");
	if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
		KillObj("NotBuyMallItem_MsgBox");
	if( P_VALID(GetObj("NotGiftMallItem_MsgBox")) )
		KillObj("NotGiftMallItem_MsgBox");
	if( P_VALID(GetObj("LauncherSucess_MsgBox")) )
		KillObj("LauncherSucess_MsgBox");
	if( P_VALID(GetObj("LauncherError_MsgBox")) )
		KillObj("LauncherError_MsgBox");
	if( P_VALID(GetObj("ExchangeMallItemNum_MsgBox")) )
		KillObj("ExchangeMallItemNum_MsgBox");
	if( P_VALID(GetObj("AffirmExchangePackItem_MsgBox")) )
		KillObj("AffirmExchangePackItem_MsgBox");
	if( P_VALID(GetObj("AffirmExchangeMallItem_MsgBox")) )
		KillObj("AffirmExchangeMallItem_MsgBox");

	if(P_VALID(m_pWnd))
	{
		m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&MallFrame::EventHandler));
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	m_n8SecondMenu = 0;

	return bRet;
}

BOOL MallFrame::EscCancel()
{
	if( m_pWnd->IsInvisible() )
		return FALSE;

	tagGameEvent event(_T("Close_Mall_UI"), this);
	m_pFrameMgr->SendEvent(&event);
	tagGameEvent eventGift(_T("Close_Gift_UI"), NULL);
	m_pFrameMgr->SendEvent(&eventGift);
	return TRUE;
}

BOOL MallFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\mall.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	//试穿窗口
	m_pGUI->ChangeXml(&ele, "mallwin\\mallbackpic\\fittingroom", "ClassName", "PlayerTryDressWnd");
	m_pGUI->ChangeXml(&ele, "mallwin\\mallbackpic\\back\\item1", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "mallwin\\mallbackpic\\back\\item1\\itembutt", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "mallwin\\mallbackpic\\adv_back\\adv_pic", "ClassName", "HttpPicEx");
	m_pGUI->ChangeXml(&ele, "mallwin\\mallbackpic\\back", "ClassName", "IconStatic");

	//商品区
	for(int i=0; i<MALL_MAXITEM_PERPAGE; i++)
	{
		CHAR szFullName[X_LONG_NAME] = {0};
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\mallshow\\itemback%d\\itempic", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\mallshow\\itemback%d\\itempic\\item", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "MallButton");
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\mallshow\\itemback%d\\num1", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\mallshow\\itemback%d\\num1\\pic", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		for(int j=0; j<MALL_ITEM_STATE; j++)
		{
			_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\mallshow\\pic%d", i+1, MALL_ITEM_STATE*i+j+1);
			m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		}
	}
	//推荐区
	for(int i=0; i<MALL_MAX_PROMOTE_ITEM; i++)
	{
		CHAR szFullName[X_LONG_NAME] = {0};
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallshow1\\itemback%d\\itempic", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallshow1\\itemback%d\\itempic\\item", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "MallButton");
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallshow1\\itemback%d\\num1", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallshow1\\itemback%d\\num1\\pic", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		for(int j=0; j<MALL_PROMOTE_ITEM_STATE; j++)
		{
			_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallshow1\\pic%d", i+1, MALL_PROMOTE_ITEM_STATE*i+(j+1));
			m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		}
	}
	//礼包
	for(int i=0; i<MALL_MAXPACK_PERPAGE; i++)
	{
		CHAR szFullName[X_LONG_NAME] = {0};
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\packagebackpic\\package%d", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\packagebackpic\\package%d\\packagename", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\packagebackpic\\package%d\\packageprice", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\packagebackpic\\package%d\\word3", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\packagebackpic\\package%d\\num1", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\packagebackpic\\package%d\\num1\\pic", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		for(int j=0; j<MALL_PACKPRO_NUM; j++)
		{
			_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\packagebackpic\\package%d\\item%d", i+1, j+1);
			m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
			_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\packagebackpic\\package%d\\item%d\\itembutt", i+1, j+1);
			m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "MallButton");
		}
		for(int k=0; k<MALL_PACKPRO_STATE; k++)
		{	
			_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\mallback\\packagebackpic\\pic%d", i+1, MALL_PACKPRO_STATE*i+(k+1));
			m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");			
		}
	}

	for (int l=0; l<LEVEL_NUM_CHOOSE; ++l)
	{
		CHAR szFullName[X_LONG_NAME] = {0};
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\preferential_area\\bag%d", l );
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "GlintPushButton");
	}

	for(int i = 0; i < Mall_ShopingCart_Num; i++)
	{
		CHAR szFullName[X_LONG_NAME] = {0};
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\sp\\back\\item%d\\tb", i);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
	}

	for(int i = 0; i < Mall_ShopingCart_RewardNum; i++)
	{
		CHAR szFullName[X_LONG_NAME] = {0};
		_snprintf(szFullName, sizeof(szFullName), "mallwin\\mallbackpic\\sp1\\back\\item%d\\tb", i);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
	}

	////团购
	//for( int i=0; i<Max_GBInfo_PerPage; ++i )
	//{
	//	CHAR szFullName[X_LONG_NAME] = {0};
	//	_snprintf( szFullName, X_LONG_NAME, "mallwin\\mallbackpic\\groupbuy_page\\itemlist\\itemback%d\\itempic", i+1 );
	//	m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
	//	_snprintf( szFullName, X_LONG_NAME, "mallwin\\mallbackpic\\groupbuy_page\\itemlist\\itemback%d\\itempic\\item", i+1 );
	//	m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "MallButton");
	//	for( int j=0; j<Max_Item_Pro; ++j )
	//	{
	//		_snprintf( szFullName, X_LONG_NAME, "mallwin\\mallbackpic\\groupbuy_page\\itemlist\\itemback%d\\pic%d", i+1, j+1 );
	//		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
	//	}
	//}

	// 创建
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pWndCaption = m_pWnd->GetChild(_T("mallbackpic"));
	/*m_pWndNotice = (GUIStatic*)m_pWndCaption->GetChild(_T("adv_back"));
	m_pWndNotice->SetInvisible(TRUE);*/
	m_pWndTryDress = (PlayerTryDressWnd*)m_pWndCaption->GetChild(_T("fittingroom"));
	m_pBtnClose	= (GUIButton*)m_pWndCaption->GetChild(_T("closebutt"));
	m_pPBtnYuanBaoTrade = (GUIPushButton*)m_pWndCaption->GetChild(_T("mallfra\\trade"));
	//韩服商城不显示元宝交易
	TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
	TCHAR szTemp[X_LONG_NAME*2];
	_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
	tstring strAppPath = szTemp;
	tstring strIniFileFullName = strAppPath + szIniFileName;
	::GetPrivateProfileString(_T("Launcher"), _T("LoginMode"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
	tstring policy = szTemp;
	m_nPolicy = _ttoi(policy.c_str());
	m_pBtnYuanBaoAbundance = (GUIButton*)m_pWndCaption->GetChild(_T("butt1"));
	//开启等级限制
	if ( 7 ==  SaveAccount::Inst()->GetLoginMode() || 8 == SaveAccount::Inst()->GetLoginMode() )
		m_bLevelLimit = FALSE;

	m_pPBLevelLimit = (GUIPushButton*)m_pWndCaption->GetChild(_T("mallback\\openbutt3"));
	if( m_bLevelLimit )
		m_pPBLevelLimit->SetState( EGUIBS_PushDown, FALSE );
	else
		m_pPBLevelLimit->SetState( EGUIBS_PopUp, FALSE );
	m_pStcGold = (GUIStatic*)m_pWndCaption->GetChild(_T("num1"));
	m_pStcSilver = (GUIStatic*)m_pWndCaption->GetChild(_T("num2"));
	m_pStcYuanBao = (GUIStatic*)m_pWndCaption->GetChild(_T("moneynum"));
	m_pExchangeVolume = (GUIStatic*)m_pWndCaption->GetChild(_T("moneynum1"));
	m_pStcMark = (GUIStatic*)m_pWndCaption->GetChild(_T("moneynum3"));
	//m_pPBtnGroupBuy = (GUIPushButton*)m_pWndCaption->GetChild(_T("mallfra\\groupbuy"));
	m_pBtnReset = (GUIButton*)m_pWndCaption->GetChild(_T("butt10"));
	m_pPBtnMall = (GUIPushButton*)m_pWndCaption->GetChild(_T("mallfra\\commodities"));
	//m_pPBtnNotice = (GUIPushButton*)m_pWndCaption->GetChild(_T("mallfra\\post"));
	//m_pHttp_MallNotice = (HttpPicEx*)m_pWndNotice->GetChild(_T("adv_pic"));

	//推荐区按钮
	m_pStcProPageText = (GUIStatic*)m_pWndCaption->GetChild(_T("mallshow1\\pagenum"));// 推荐商品页数显示	
	m_pBtnFormerPage  = (GUIButton*)m_pWndCaption->GetChild(_T("mallshow1\\pageback"));// 推荐商品向前换页
	m_pBtnLatterPage  = (GUIButton*)m_pWndCaption->GetChild(_T("mallshow1\\pageback0"));// 推荐商品向后换页

	// 染色控件
	for( int i = 0; i < MALL_DAY_PART; i++ )
	{
		TCHAR szDyeTips[X_SHORT_NAME] = {0};
		_sntprintf( szDyeTips, X_SHORT_NAME, _T("fittingroom\\word%d"), i + 1);
		m_pStcDyeTips[i] = (GUIStatic*)m_pWndCaption->GetChild(szDyeTips);
		m_pStcDyeTips[i]->SetInvisible(true);

		TCHAR szDayPart[X_SHORT_NAME] = {0};
		_sntprintf( szDayPart, X_SHORT_NAME, _T("fittingroom\\combo%d"), i + 1);
		m_pCBDayFation[i] = (GUIComboBox*)m_pWndCaption->GetChild(szDayPart);

		for( int j = 0; j <= EC_End; j++ )
		{
			TCHAR szDay[64] = {0};
			_stprintf( szDay,_T("%s%d"), _T("FationDay_Color"), j );
			m_pCBDayFation[i]->GetListBox()->SetText(j, 0, g_StrTable[szDay] );
		}

		tagPoint ptSize	= m_pCBDayFation[i]->GetListBox()->GetSize();
		ptSize.x = m_pCBDayFation[i]->GetEditBox()->GetSize().x;
		ptSize.y = m_pCBDayFation[i]->GetListBox()->GetRowHeight() * (EC_End + 1);
		m_pCBDayFation[i]->GetListBox()->SetResizable(TRUE);
		m_pCBDayFation[i]->GetListBox()->SetColWidth(0, ptSize.x);
		m_pCBDayFation[i]->GetListBox()->Resize(ptSize);
		m_pCBDayFation[i]->GetListBox()->SetResizable(FALSE);
		LPCTSTR szTemp = m_pCBDayFation[i]->GetListBox()->GetText(EC_Val10, 0);
		m_pCBDayFation[i]->GetEditBox()->SetText(szTemp);
		m_pCBDayFation[i]->SetInvisible(true);
	}

	m_pStcFetchBack = (IconStatic*)m_pWndCaption->GetChild(_T("back"));
	m_pStcFetchBack->SetInvisible(TRUE);
	m_pStcFetchSel = (IconStatic*)m_pWndCaption->GetChild(_T("back\\item1"));
	m_pFetchUnitItem = (ItemButton*)m_pWndCaption->GetChild(_T("back\\item1\\itembutt"));
	m_pBtnFetch = (GUIButton*)m_pWndCaption->GetChild(_T("back\\butt1"));
	m_pBtnFetchTitle = (GUIStatic*)m_pWndCaption->GetChild(_T("back\\word"));

	m_pPBtnAll = (GUIPushButton*)m_pWndCaption->GetChild(_T("mallback\\fra\\all"));
	m_pPBtnAll->SetText(g_StrTable[_T("MallTrade0")]);
	m_pPBtnPackage = (GUIPushButton*)m_pWndCaption->GetChild(_T("mallback\\fra\\package"));
	m_pPBtnPackage->SetText(g_StrTable[_T("MallTrade1")]);

	m_pStcGoodsBack = (GUIStatic*)m_pWndCaption->GetChild(_T("mallback"));
    m_pStcYuanbaoBack = (GUIStatic*)m_pWndCaption->GetChild(_T("tradeback"));
	/*m_pStcGroupBuyBack = (GUIStatic*)m_pWndCaption->GetChild(_T("groupbuy_page"));
	m_pStcGroupBuyBack->SetInvisible(TRUE);*/
	for(int i=0; i<MALL_ITEM_SUBTYPE-2; i++) //多出来的2个作为积分和赠卷的显示
	{
		TCHAR szFullName[X_LONG_NAME] = {0};
		_sntprintf(szFullName, sizeof(szFullName), _T("fra\\page%d"), i+1);
		m_pPBtnSubMall[i] = (GUIPushButton*)m_pStcGoodsBack->GetChild(szFullName);
		tagMallTitleData* pData = MallData::Inst()->FindMallTitleData(i);
		if( !P_VALID(pData) )
			continue;
		if( pData->strTitleName!=_T("") )
		{
			m_pPBtnSubMall[i]->SetText(pData->strTitleName.c_str());
			m_pPBtnSubMall[i]->SetInvisible(FALSE);
		}
		else
		{
			m_pPBtnSubMall[i]->SetInvisible(TRUE);
		}
	}
	m_pPBtnPresent = (GUIPushButton*)m_pStcGoodsBack->GetChild(_T("fra\\page7")); //赠卷
	//m_pPBtnPresent->SetText(g_StrTable[_T("MallTrade_Present_Zone")]);
	m_pPBtnMark = (GUIPushButton*)m_pStcGoodsBack->GetChild(_T("fra\\page8"));	  //积分
	//m_pPBtnMark->SetText(g_StrTable[_T("MallTrade_Mark_Zone")]);
	for (int i = 0; i < MALL_ITEM_SECOND_SUBTYPE; ++i)
	{
		TCHAR szFullName[X_LONG_NAME] = {0};
		_sntprintf(szFullName, sizeof(szFullName), _T("com_fra\\optbutt%d"), i);

		m_pPBtnMallType2[i] = (GUIPushButton*)m_pStcGoodsBack->GetChild(szFullName);

		m_pPBtnMallType2[i]->SetInvisible(TRUE);
	}

	m_pStcGoodsBackEx = (GUIStatic*)m_pWndCaption->GetChild(_T("mallback\\mallshow"));
	m_pStcGoodsBackEx->SetInvisible(TRUE);
	m_pBtnPDItem	= (GUIButton*)m_pStcGoodsBackEx->GetChild(_T("pageback0"));
	m_pBtnPUItem = (GUIButton*)m_pStcGoodsBackEx->GetChild(_T("pageback"));
	m_pStcCPItem = (GUIStatic*)m_pStcGoodsBackEx->GetChild(_T("pagenum"));
	m_pBtnBuy = (GUIButton*)m_pWndCaption->GetChild(_T("mallback\\butt1"));
	m_pBtnPresent = (GUIButton*)m_pWndCaption->GetChild(_T("mallback\\butt10"));
	//m_pBtnGroupBuy = (GUIButton*)m_pStcGoodsBack->GetChild(_T("wholesale"));
	//m_pBtnExchange = (GUIButton*)m_pStcGoodsBack->GetChild(_T("exchange"));
	//m_pBtnExchange->SetEnable(false);

	for(int i=0; i<MALL_MAXITEM_PERPAGE; i++)
	{
		TCHAR szFullName[X_LONG_NAME] = {0};
		//商品
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d"), i+1);
		m_pUnitItemBack[i] = (IconStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
		//名字
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\name"), i+1);
		m_pStcItemName[i] = (GUIStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
		//剩余数目
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\word1"), i+1);
		m_pStcGoodsRest[i] =(GUIStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
		//赠品
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\word2"), i+1);
		m_pStcGoodsGift[i] = (GUIStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
		m_pStcGoodsGift[i]->SetText(g_StrTable[_T("MallTrade_Package_Gift")]);
		m_pStcGoodsGift[i]->SetInvisible(TRUE);
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\word5"), i+1);
		m_pStcGoodsGiftNum[i] = (GUIStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
		//原价
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\word3"), i+1);
		m_pStcGoodsInfo[i] = (GUIStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
		m_pStcGoodsInfo[i]->SetText(g_StrTable[_T("MallTrade_Package_OldPrice")]);
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\num1"), i+1);
		m_pStcGoodsPrice[i] = (IconStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\num1\\pic"), i+1);
		m_pStcGoodsPriceDel[i] = (IconStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
		//现价
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\word4"), i+1);
		m_pStcGoodsInfoNow[i] = (GUIStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
		m_pStcGoodsInfoNow[i]->SetText(g_StrTable[_T("MallTrade_Package_NowPrice")]);
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\num2"), i+1);
		m_pStcGoodsPriceNow[i] = (GUIStatic*)m_pStcGoodsBackEx->GetChild(szFullName);

		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\itempic"), i+1);
		m_pUnitItemSel[i] = (IconStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\itempic\\item"), i+1);
		m_pUnitItem[i] = (MallButton*)m_pStcGoodsBackEx->GetChild(szFullName);
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\price"), i+1);
		m_pStcInfo[i] = (GUIStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
		
		for(int j=0; j<MALL_ITEM_STATE; j++)
		{
			_sntprintf(szFullName, sizeof(szFullName), _T("pic%d"),MALL_ITEM_STATE*i+(j+1));
			m_pStcState[i][j] = (IconStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
			m_pStcState[i][j]->SetInvisible(TRUE);
			_sntprintf(szFullName, sizeof(szFullName), _T("pic%d\\pic"),MALL_ITEM_STATE*i+(j+1));
			m_pStcStateInv[i][j] = (GUIStatic*)m_pStcGoodsBackEx->GetChild(szFullName);
			m_pStcStateInv[i][j]->SetInvisible(TRUE);
		}
	}

	m_pStcProBack = (GUIStatic*)m_pWndCaption->GetChild(_T("mallshow1"));
	for(int i=0; i<MALL_MAX_PROMOTE_ITEM; i++)
	{
		TCHAR szFullName[X_LONG_NAME] = {0};
		//推荐商品
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d"), i+1);
		m_pProItemBack[i] = (IconStatic*)m_pStcProBack->GetChild(szFullName);
		//名字
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\name"), i+1);
		m_pProItemName[i] = (GUIStatic*)m_pStcProBack->GetChild(szFullName);
		//剩余数目
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\word1"), i+1);
		m_pStcProRest[i] =(GUIStatic*)m_pStcProBack->GetChild(szFullName);
		//赠品
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\price"), i+1);
		m_pStcProGift[i] = (GUIStatic*)m_pStcProBack->GetChild(szFullName);
		m_pStcProGift[i]->SetText(g_StrTable[_T("MallTrade_Package_Gift")]);
		m_pStcProGift[i]->SetInvisible(TRUE);
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\word5"), i+1);
		m_pStcProGiftNum[i] = (GUIStatic*)m_pStcProBack->GetChild(szFullName);
		//原价
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\word3"), i+1);
		m_pStcProInfo[i] = (GUIStatic*)m_pStcProBack->GetChild(szFullName);
		m_pStcProInfo[i]->SetText(g_StrTable[_T("MallTrade_Package_OldPrice")]);
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\num1"), i+1);
		m_pStcProPrice[i] = (IconStatic*)m_pStcProBack->GetChild(szFullName);
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\num1\\pic"), i+1);
		m_pStcProPriceDel[i] = (IconStatic*)m_pStcProBack->GetChild(szFullName);
		//现价
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\word4"), i+1);
		m_pStcProInfoNow[i] = (GUIStatic*)m_pStcProBack->GetChild(szFullName);
		m_pStcProInfoNow[i]->SetText(g_StrTable[_T("MallTrade_Package_NowPrice")]);
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\num2"), i+1);
		m_pStcProPriceNow[i] = (GUIStatic*)m_pStcProBack->GetChild(szFullName);

		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\itempic"), i+1);
		m_pUnitProItemSel[i] = (IconStatic*)m_pStcProBack->GetChild(szFullName);
		_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\itempic\\item"), i+1);
		m_pUnitProItem[i] = (MallButton*)m_pStcProBack->GetChild(szFullName);
		/*_sntprintf(szFullName, sizeof(szFullName), _T("itemback%d\\"), i+1);
		m_pProItmeInfo[i] = (GUIStatic*)m_pStcProBack->GetChild(szFullName);*/		
	}

	for(int i=0; i<MALL_MAX_PROMOTE_ITEM_PerPage; i++)
	{
		for(int j=0; j<MALL_PROMOTE_ITEM_STATE; j++)
		{
			TCHAR szFullName[X_LONG_NAME] = {0};
			_sntprintf(szFullName, sizeof(szFullName), _T("pic%d"), MALL_PROMOTE_ITEM_STATE*i+j+1);
			m_pProItemState[i][j] = (IconStatic*)m_pStcProBack->GetChild(szFullName);
			m_pProItemState[i][j]->SetInvisible(TRUE);
			_sntprintf(szFullName, sizeof(szFullName), _T("pic%d\\pic"),MALL_PROMOTE_ITEM_STATE*i+j+1);
			m_pProItemStateInv[i][j] = (GUIStatic*)m_pStcProBack->GetChild(szFullName);
			m_pProItemStateInv[i][j]->SetInvisible(TRUE);
		}
	}

	m_pStcPackBack = (GUIStatic*)m_pWndCaption->GetChild(_T("mallback\\packagebackpic"));
	m_pStcPackBack->SetInvisible(TRUE);
	m_pBtnPDPack = (GUIButton*)m_pStcPackBack->GetChild(_T("pageback0"));
	m_pBtnPUPack = (GUIButton*)m_pStcPackBack->GetChild(_T("pageback"));
	m_pBtnCPPack = (GUIStatic*)m_pStcPackBack->GetChild(_T("pagenum"));
	for(int i=0; i<MALL_MAXPACK_PERPAGE; i++)
	{
		TCHAR szFullName[X_LONG_NAME] = {0};
		//礼包
		_sntprintf(szFullName, sizeof(szFullName), _T("package%d"), i+1);
		m_pUnitPackBack[i] = (IconStatic*)m_pStcPackBack->GetChild(szFullName);
		//名字
		_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\packagename"), i+1);
		m_pStcPackName[i] =(IconStatic*)m_pStcPackBack->GetChild(szFullName);
		//按钮
		_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\butt"), i+1);
		m_pBtnPackBack[i] = (GUIButton*)m_pStcPackBack->GetChild(szFullName);
		//剩余数目
		_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\word1"), i+1);
		m_pStcPackRest[i] =(GUIStatic*)m_pStcPackBack->GetChild(szFullName);
		//赠品
		_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\packageprice"), i+1);
		m_pStcPackGift[i] = (GUIStatic*)m_pStcPackBack->GetChild(szFullName);
		m_pStcPackGift[i]->SetText(g_StrTable[_T("MallTrade_Package_Gift")]);
		m_pStcPackGift[i]->SetInvisible(TRUE);
		_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\word2"), i+1);
		m_pStcPackGiftNum[i] = (GUIStatic*)m_pStcPackBack->GetChild(szFullName);
		//原价
		_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\word3"), i+1);
		m_pStcPackInfo[i] = (GUIStatic*)m_pStcPackBack->GetChild(szFullName);
		m_pStcPackInfo[i]->SetText(g_StrTable[_T("MallTrade_Package_OldPrice")]);
		_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\num1"), i+1);
		m_pStcPackPrice[i] = (IconStatic*)m_pStcPackBack->GetChild(szFullName);
		_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\num1\\pic"), i+1);
		m_pStcPackPriceDel[i] = (IconStatic*)m_pStcPackBack->GetChild(szFullName);
		//现价
		_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\word4"), i+1);
		m_pStcPackInfoNow[i] = (GUIStatic*)m_pStcPackBack->GetChild(szFullName);
		m_pStcPackInfoNow[i]->SetText(g_StrTable[_T("MallTrade_Package_NowPrice")]);
		_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\num2"), i+1);
		m_pStcPackPriceNow[i] = (GUIStatic*)m_pStcPackBack->GetChild(szFullName);

		//_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\new"), i+1);
		//m_pStcPackNew[i] = (GUIStatic*)m_pStcPackBack->GetChild(szFullName);
		//_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\hot"), i+1);
		//m_pStcPackHot[i] = (GUIStatic*)m_pStcPackBack->GetChild(szFullName);
		for(int j=0; j<MALL_PACKPRO_NUM; j++)
		{
			_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\item%d"), i+1, j+1);
			m_pUnitPackSel[i][j] = (IconStatic*)m_pStcPackBack->GetChild(szFullName);
			_sntprintf(szFullName, sizeof(szFullName), _T("package%d\\item%d\\itembutt"), i+1, j+1);
			m_pUnitPack[i][j] = (MallButton*)m_pStcPackBack->GetChild(szFullName);			
		}
		for(int k=0; k<MALL_PACKPRO_STATE; k++)
		{	
			_sntprintf(szFullName, sizeof(szFullName), _T("pic%d"), MALL_PACKPRO_STATE*i+(k+1));
			m_pStcPackState[i][k] = (IconStatic*)m_pStcPackBack->GetChild(szFullName);		
			m_pStcPackState[i][k]->SetInvisible(TRUE);
			_sntprintf(szFullName, sizeof(szFullName), _T("pic%d\\pic"),MALL_PACKPRO_STATE*i+(k+1));
			m_pStcPackStateInv[i][k] = (GUIStatic*)m_pStcPackBack->GetChild(szFullName);
			m_pStcPackStateInv[i][k]->SetInvisible(TRUE);
		}
	}

	for (int l=0; l<LEVEL_NUM_CHOOSE; ++l)
	{
		TCHAR szFullName[X_LONG_NAME] = {0};
		_sntprintf(szFullName, sizeof(szFullName), _T("mallbackpic\\preferential_area\\bag%d"),l );
		m_pGPBLevel[l] = (GlintPushButton*)m_pWnd->GetChild(szFullName);
		m_pGPBLevel[l]->SetGlintStart(FALSE);
	}
	//加载购物车相关控件
	for(int i = 0; i < MALL_MAXITEM_PERPAGE; i++)
	{
		TCHAR szText[128] = {0};
		_stprintf(szText, _T("mallbackpic\\mallback\\mallshow\\itemback%d\\butt"), i+1);
		m_pBtnShopingCart[i] = (GUIButton*)m_pWnd->GetChild(szText);					//购物车按钮
	}

	for(int i = 0; i < Mall_ShopingCart_Num; i++)
	{
		TCHAR szText[128] = {0};
		_stprintf(szText, _T("mallbackpic\\sp\\back\\item%d\\tb"), i);
		m_pIStcShopingCartItem[i] = (IconStatic*)m_pWnd->GetChild(szText);				//购物车内物品
	}
	
	for(int i = 0; i < Mall_ShopingCart_RewardNum; i++)
	{
		TCHAR szText[128] = {0};
		_stprintf(szText, _T("mallbackpic\\sp1\\back\\item%d\\tb"), i);
		m_pIStcShopingCartReward[i] = (IconStatic*)m_pWnd->GetChild(szText);			//奖励物品
	}

	for(int i = 0; i < Mall_ShopingCart_Num; i++)
	{
		TCHAR szText[128] = {0};
		_stprintf(szText, _T("mallbackpic\\sp\\back\\ra_butt%d"), i*2);
		m_pBtnItemAdd[i] = (GUIButton*)m_pWnd->GetChild(szText);			//加
		_stprintf(szText, _T("mallbackpic\\sp\\back\\ra_butt%d"), i*2+1);
		m_pBtnItemRed[i] = (GUIButton*)m_pWnd->GetChild(szText);
	}
	m_pBtnCheckout			= (GUIButton*)m_pWnd->GetChild(_T("mallbackpic\\butt2"));												//结账按钮
	m_pBtnClearSHopingCart	= (GUIButton*)m_pWnd->GetChild(_T("mallbackpic\\butt3"));										//清空购物车按钮
	m_pStcYuanBaoCast		= (GUIStatic*)m_pWnd->GetChild(_T("mallbackpic\\sp1\\back\\word\\new"));											//元宝消耗数
	m_pStcRewardNum			= (GUIStatic*)m_pWnd->GetChild(_T("mallbackpic\\sp1\\back\\cheng"));											//奖励次数（胜币总价值/获取奖励的最低胜币数）
	m_pStcShopingTips		= (GUIStaticEx*)m_pWnd->GetChild(_T("mallbackpic\\sp1\\back\\word\\word"));
	m_pWndShopingText		= m_pWnd->GetChild(_T("mallbackpic\\word0"));
	m_pWndShopingCatrWnd	= m_pWnd->GetChild(_T("mallbackpic\\sp"));

	HideShopingCart();
	HideShopingReward();

	m_pWndShowBox = m_pWnd->GetChild(_T("mallbackpic\\fittingroom"));
	if(!m_pSession->IsEnableShopingCart())
	{
		m_pWndShowBox->SetInvisible(FALSE);
		m_pWndShowBox->SetTopMost(TRUE);
		m_pBtnCheckout->SetInvisible(TRUE);
		m_pBtnClearSHopingCart->SetInvisible(TRUE);
		m_pWndShopingText->SetInvisible(TRUE);
		m_pWndShopingCatrWnd->SetInvisible(TRUE);
		for(int i = 0; i < MALL_MAXITEM_PERPAGE; i++)
		{
			m_pBtnShopingCart[i]->SetInvisible(TRUE);
		}
	}

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&MallFrame::EventHandler));

	m_pStcGoodsBack->SetInvisible(TRUE);

	m_pWnd->SetInvisible(TRUE);

    // 创建元宝交易frame
    MallMgr::Inst()->CreateYuanBaoFrame(m_strName.c_str());

	//// 创建团购frame
	//MallMgr::Inst()->CreateGroupBuyFrame(m_strName.c_str());

    // 默认显示商城而非元宝界面
    m_pPBtnMall->SetState(EGUIBS_PushDown);

	//SetOperateState();
	if ( m_nPolicy == 5 )
	{
		m_pPBtnYuanBaoTrade->SetInvisible(TRUE);
		m_pBtnBuy->SetEnable(false);
		m_pBtnPresent->SetEnable(false);
	}

	return TRUE;
}

void MallFrame::UpdateAvatar()
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp) && P_VALID(m_pWndTryDress))
	{
		m_pWndTryDress->UpdataAvatar( pLp );
	}
}

VOID MallFrame::Render3D()
{
	if(P_VALID(m_pWndTryDress))
		m_pWndTryDress->Render3D();
	GameFrame::Render3D();
}

VOID MallFrame::Update()
{
	GameFrame::Update();

	LevelPromoteFrame *pLevelPromote = (LevelPromoteFrame*) m_pFrameMgr->GetFrame(_T("LevelPromoteFrame"));
	if(P_VALID(m_pWnd)&&P_VALID(pLevelPromote)&&(!m_pWnd->IsInvisible()))
	{
		if (!(pLevelPromote->IsShow()))
		{
			for (int i=0; i<LEVEL_NUM_CHOOSE; ++i)
			{
				m_pGPBLevel[i]->SetState(EGUIBS_PopUp);
			}
		}
	}

	if(TObjRef<NetSession>()->IsEnableExpressPay())
		m_pBtnYuanBaoAbundance->SetInvisible(FALSE);
	else
		m_pBtnYuanBaoAbundance->SetInvisible(TRUE);
	if( m_vecGiftEvent.size() == 0 )
		return;
	for(vector<tagMsgMallGiftEvent>::iterator it=m_vecGiftEvent.begin();
		it!=m_vecGiftEvent.end(); )
	{
		if(timeGetTime() - it->dwTime >= MAX_PRESENT_ITEM_TIME)
		{
			tagMsgMallGiftEvent evt  = (*it);
			OnMsgMallGiftEvent(&evt);
			it = m_vecGiftEvent.erase(it);
			break;
		}
		else
			++it;
	}

}

DWORD MallFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Active:
		for( int i = 0; i < MALL_DAY_PART; i++ )
		{
			if( pWnd == m_pCBDayFation[i]->GetEditBox() )
				m_pGUI->SetActive( m_pCBDayFation[i]->GetListBox() );
		}
		break;
	case EGUIE_Click:
		{
			//关闭
			if( pWnd == m_pBtnClose )
			{
				GroupBuyFrame* pFrame = MallMgr::Inst()->GetGroupBuyFrame();
				if( P_VALID(pFrame) )
				{
					pFrame->ShowWnd();
				}
				tagGameEvent event(_T("Close_Mall_UI"), this);
				m_pFrameMgr->SendEvent(&event);
				tagGameEvent eventGift(_T("Close_Gift_UI"), NULL);
				m_pFrameMgr->SendEvent(&eventGift);
				m_n8SecondMenu = 0; //二级分页清零
			}
			//下一页
			else if( pWnd==m_pBtnPDItem || pWnd==m_pBtnPDPack )
			{
				RefreshMallItemFlag();
				m_dwSelectItemID = GT_INVALID;
				PageDown();
			}
			//上一页
			else if( pWnd==m_pBtnPUItem || pWnd==m_pBtnPUPack )
			{
				RefreshMallItemFlag();
				m_dwSelectItemID = GT_INVALID;
				PageUp();
			}
			//重置试穿
			else if( pWnd == m_pBtnReset )
			{
				if( P_VALID(m_pWndTryDress) )
				{
					m_pWndTryDress->RotateYaw(0.0f);
					m_pWndTryDress->ReLoadTryEquip();
					for( int i = 0; i < MALL_DAY_PART; i++ )
					{
						LPCTSTR szTemp = m_pCBDayFation[i]->GetListBox()->GetText(EC_Val10, 0);
						m_pCBDayFation[i]->GetEditBox()->SetText(szTemp);
						m_pCBDayFation[i]->SetInvisible(true);
						m_pStcDyeTips[i]->SetInvisible(true);
						m_dwTryDressID[i] = GT_INVALID;
					}
				}
			}
			//元宝冲值
			else if( pWnd == m_pBtnYuanBaoAbundance )
			{
				YuanBaoAbundance();
			}
			/*else if( pWnd == m_pBtnGroupBuy  )
			{
				GroupBuyFrame* pFrame = MallMgr::Inst()->GetGroupBuyFrame();
				if( P_VALID(pFrame) )
				{
					pFrame->ShowWnd(GroupBuyFrame::EGroupBuy_Purchase);
				}
			}*/
			/*else if( pWnd == m_pBtnExchange )
			{
				ExchangeMallItem();
			}	*/
			//购买
			else if( pWnd == m_pBtnBuy )
			{
				if( m_bSelectPromote )
					BuyMallItemPromote();
				else
					BuyMallItem();
			}
			//赠送
			else if( pWnd == m_pBtnPresent )
			{
				if( m_dwSelectItemID == GT_INVALID )
					return 0;
				MallMgr::Inst()->CreateMallGiftFrame();
				this->SetPresentMaxNum();
			}
			//领取
			else if( pWnd == m_pBtnFetch )
			{
				m_pStcFetchSel->SetPic(szSelectItem);
				m_pStcFetchBack->SetPic(szPickItem);
				SetHopeBuyItemID(m_pFetchUnitItem->GetBtnData().GetData1());
				m_bSelectPromote = false;
				FreeGetMallItem();
			}
			//右键单击,购买商品
			else if (_tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0)
			{
				//先刷新赠送数量，防止赠送不可堆叠物品赠送bug
				MallMgr::Inst()->InitMallGiftFrameNum();
				//右键购买响应
				for(int i=0; i<MALL_MAXITEM_PERPAGE; i++)
				{
					if( pWnd == m_pUnitItem[i] )
					{
						RefreshMallItemFlag();
						SetHopeBuyItemID(m_pUnitItem[i]->GetBtnData().GetData1());						
						m_pUnitItemSel[i]->SetPic(szSelectItem);
						m_pUnitItemBack[i]->SetPic(szPickItem);
						m_bSelectPromote = false;
						const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
						BuyMallItem();
						return 0;
					}
				}
				for(int i=0; i<MALL_MAX_PROMOTE_ITEM; i++)
				{
					if( pWnd == m_pUnitProItem[i] )
					{
						RefreshMallItemFlag();
						SetHopeBuyItemID(m_pUnitProItem[i]->GetBtnData().GetData1());
						m_pUnitProItemSel[i]->SetPic(szSelectItem);
						m_pProItemBack[i]->SetPic(szPickItem);
						m_bSelectPromote = true;
						BuyMallItemPromote();
						return 0;
					}
				}
				for( int i=0; i<MALL_MAXPACK_PERPAGE; i++ )
				{
					if( pWnd == m_pBtnPackBack[i] )
					{
						RefreshMallItemFlag();
						m_dwSelectItemID = GT_INVALID;
						m_pUnitPackBack[i]->SetPic(szSelectPack);
					}
					for(int j=0; j<MALL_PACKPRO_NUM; j++)
					{
						if( pWnd == m_pUnitPack[i][j] )
						{
							RefreshMallItemFlag();
							SetHopeBuyItemID(m_pUnitPack[i][j]->GetBtnData().GetData1());							
							m_bSelectPromote = false;
							BuyMallItem();
							return 0;
						}
					}
					
				}
				if( pWnd == m_pFetchUnitItem )
				{
					m_pStcFetchSel->SetPic(szSelectItem);
					m_pStcFetchBack->SetPic(szPickItem);
					SetHopeBuyItemID(m_pFetchUnitItem->GetBtnData().GetData1());
					m_bSelectPromote = false;
					FreeGetMallItem();
					return 0;
				}
			}
			else if(_tcsnccmp(pWnd->GetName().c_str(), _T("package"), 7) == 0)
			{
				RefreshMallItemFlag();
				for( int i=0; i<MALL_MAXPACK_PERPAGE; i++ )
				{
					if( pWnd==m_pUnitPackBack[i] || pWnd==m_pStcPackName[i] 
					 || pWnd==m_pStcPackInfo[i]  || pWnd==m_pStcPackInfoNow[i] 
					 || pWnd==m_pStcPackPrice[i] || pWnd==m_pStcPackPriceNow[i]
					 || pWnd==m_pStcPackGift[i]  || pWnd==m_pStcPackGiftNum[i] )
					{
						m_pUnitPackBack[i]->SetPic(szSelectPack);
						SetHopeBuyItemID(m_pUnitPack[i][0]->GetBtnData().GetData1());
						m_pUnitPackBack[i]->SetPic(szSelectPack);
						m_bSelectPromote = false;
						BuyMallItem();
						return 0;
					}
				}
			}
			else if ( _tcsnccmp(pWnd->GetName().c_str(), _T("butt"), 4) == 0 )
			{
				RefreshMallItemFlag();
				for( int i=0; i<MALL_MAXPACK_PERPAGE; i++ )
				{
					if( pWnd == m_pBtnPackBack[i] )
					{
						m_pUnitPackBack[i]->SetPic(szSelectPack);
						SetHopeBuyItemID(m_pUnitPack[i][0]->GetBtnData().GetData1());
						m_pUnitPackBack[i]->SetPic(szSelectPack);
						m_bSelectPromote = false;
						BuyMallItem();
						return 0;
					}
				}

				for(int  i = 0; i < MALL_MAXITEM_PERPAGE; i++)	//点击加入购物车按钮
				{
					if( pWnd == m_pBtnShopingCart[i] )
					{
						m_dwAddItemId = m_pUnitItem[i]->GetBtnData().GetData1();

						const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwAddItemId);
						if( !P_VALID(pItem) )
							return  0;
						const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pItem->dwTypeID);
						if( !P_VALID(pProto) )
							return 0;
						int nMaxValue = pProto->n16MaxLapNum;


						if( P_VALID(GetObj("AddToShopingCart_MsgBox")) )
							KillObj("AddToShopingCart_MsgBox");
						CreateObj("AddToShopingCart_MsgBox", "MsgInputBox");



						TObjRef<MsgInputBox>("AddToShopingCart_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade4")], 
							_T("AddToShopingCart_Event"), nMaxValue, (MsgBoxInputType)(MBIT_Number), _T("1"), TRUE);
					}	
				}

				if (pWnd == m_pBtnCheckout)
				{
					bool bHasItem = false;
					for(int i = 0; i < Mall_ShopingCart_Num; i++)
					{
						if(P_VALID(m_pShopingItem[i]))
						{
							bHasItem = true;
							break;
						}
					}

					if(!bHasItem)
						return 0;

					//二次确认框
					if( P_VALID(GetObj("ConfirmShopingCart_MsgBox")) )
						KillObj("ConfirmShopingCart_MsgBox");
					CreateObj("ConfirmShopingCart_MsgBox", "MsgBox");

					TCHAR szText[128] = {0};
					_stprintf(szText, g_StrTable[_T("MallTrade77")], m_pStcYuanBaoCast->GetText().c_str());
					TObjRef<MsgBox>("ConfirmShopingCart_MsgBox")->Init(_T(""), szText,
						_T("ConfirmShopingCart_Event"), (MsgBoxFlag)(MBF_OK|MBF_Cancel));
				}
				else if( pWnd == m_pBtnClearSHopingCart )
				{
					ClearShopingCart();
				}
			}
			else if ( pWnd == m_pBtnFormerPage )
			{
				if ( m_iPromoteItemShowPage > 1 )
				{
					m_iPromoteItemShowPage--;
					RefreshPromoteItemState();
					ShowPromoteMallItem();
				}
			} 
			else if ( pWnd == m_pBtnLatterPage )
			{
				INT MaxProItem =  MallMgr::Inst()->GetMaxPromotePage();
				if ( m_iPromoteItemShowPage < MaxProItem )
				{
					m_iPromoteItemShowPage++;
					RefreshPromoteItemState();
					ShowPromoteMallItem();
				}
			}
			else
			{
				for(int i = 0; i < Mall_ShopingCart_Num; i++)
				{
					if(pWnd == m_pIStcShopingCartItem[i] && m_pShopingItem[i] != NULL)
					{
						m_pShopingItem[i] = NULL;
						m_nShopingIetmNum[i] = 0;
						m_pIStcShopingCartItem[i]->SetInvisible(TRUE);
						RefreshShopingCart();
						return 0;
					}

				}

				for(int i = 0; i < Mall_ShopingCart_Num; i++)
				{
					if(pWnd == m_pBtnItemAdd[i])	//加数量
					{
						if(!P_VALID(m_pShopingItem[i]))	//无物品
							return 0;

						const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(m_pShopingItem[i]->dwTypeID);
						if( !P_VALID(pProto) )
							return 0;
						if(m_nShopingIetmNum[i] == pProto->n16MaxLapNum)	//等于堆叠最大数
							return 0;
						
						m_nShopingIetmNum[i]++;
						RefreshShopingCart();
					}
					else if(pWnd == m_pBtnItemRed[i])	//减数量
					{
						if(!P_VALID(m_pShopingItem[i]))	//无物品
							return 0;

						if(m_nShopingIetmNum[i] == 0 || m_nShopingIetmNum[i] == 1)	//为0了不能再减
							return 0;

						m_nShopingIetmNum[i]--;
						RefreshShopingCart();
					}
				}
			}
		}
		break;
	case EGUIE_Drag:
		{
			//左键单击,选中商品
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0)
			{
				//先刷新赠送数量，防止赠送不可堆叠物品赠送bug
				MallMgr::Inst()->InitMallGiftFrameNum();
				//左键购买响应
				for(int i=0; i<MALL_MAXITEM_PERPAGE; i++)
				{
					if( pWnd == m_pUnitItem[i] )
					{
						RefreshMallItemFlag();
						BtnData data = m_pUnitItem[i]->GetBtnData();
						SetHopeBuyItemID(data.GetData1());
						m_pUnitItemSel[i]->SetPic(szSelectItem);
						m_pUnitItemBack[i]->SetPic(szPickItem);
						JudgeTryDress(data.GetData2());
						m_bSelectPromote = false;
						//SetOperateState(EMIOS_NormalItem);
						return 0;
					}
				}
				for(int i=0; i<MALL_MAX_PROMOTE_ITEM; i++)
				{
					if( pWnd == m_pUnitProItem[i] )
					{
						RefreshMallItemFlag();
						BtnData data = m_pUnitProItem[i]->GetBtnData();
						SetHopeBuyItemID(data.GetData1());
						m_pUnitProItemSel[i]->SetPic(szSelectItem);
						m_pProItemBack[i]->SetPic(szPickItem);
						JudgeTryDress(data.GetData2());
						m_bSelectPromote = true;
						//SetOperateState(EMIOS_Promote);
						return 0;
					}
				}
				for( int i=0; i<MALL_MAXPACK_PERPAGE; i++ )
				{
					if( pWnd==m_pUnitPackBack[i] || pWnd==m_pStcPackName[i])
					{
						RefreshMallItemFlag();
						SetHopeBuyItemID(m_pUnitPack[i][0]->GetBtnData().GetData1());
						m_pUnitPackBack[i]->SetPic(szSelectPack);
						m_bSelectPromote = false;
						//SetOperateState(EMIOS_PackItem);
						return 0;
					}

					for(int j=0; j<MALL_PACKPRO_NUM; j++)
					{
						if( pWnd == m_pUnitPack[i][j] )
						{
							RefreshMallItemFlag();
							SetHopeBuyItemID(m_pUnitPack[i][j]->GetBtnData().GetData1());
							m_pUnitPackBack[i]->SetPic(szSelectPack);
							m_bSelectPromote = false;
							//SetOperateState(EMIOS_PackItem);
							return 0;
						}
					}
				}
				if( pWnd == m_pFetchUnitItem )
				{
					m_pStcFetchSel->SetPic(szSelectItem);
					m_pStcFetchBack->SetPic(szPickItem);
					SetHopeBuyItemID(m_pFetchUnitItem->GetBtnData().GetData1());
					m_bSelectPromote = false;
					//SetOperateState(EMIOS_Free);
					return 0;
				}
			}
			else if( _tcsnccmp(pWnd->GetName().c_str(), _T("package"), 7) == 0 )
			{
				RefreshMallItemFlag();
				for( int i=0; i<MALL_MAXPACK_PERPAGE; i++ )
				{					
					if( pWnd==m_pUnitPackBack[i] || pWnd==m_pStcPackName[i] 
					|| pWnd==m_pStcPackInfo[i]  || pWnd==m_pStcPackInfoNow[i] 
					|| pWnd==m_pStcPackPrice[i] || pWnd==m_pStcPackPriceNow[i]
					|| pWnd==m_pStcPackGift[i]  || pWnd==m_pStcPackGiftNum[i] )
					{
						DWORD dwCurPackID = GetCurPackID(i);
						SetHopeBuyItemID(dwCurPackID);
						m_pUnitPackBack[i]->SetPic(szSelectPack);
						m_bSelectPromote = false;
						//SetOperateState(EMIOS_PackItem);
						const tagMallPackProto* pItem = MallMgr::Inst()->GetMallPackByID(m_dwSelectItemID);
						if( P_VALID(pItem) )
						{
							m_pWndTryDress->ReLoadTryEquip();
							for( int i = 0; i < MALL_DAY_PART; i++ )
							{
								LPCTSTR szTemp = m_pCBDayFation[i]->GetListBox()->GetText(EC_Val10, 0);
								m_pCBDayFation[i]->GetEditBox()->SetText(szTemp);
								m_pCBDayFation[i]->SetInvisible(true);
								m_pStcDyeTips[i]->SetInvisible(true);
								m_dwTryDressID[i] = GT_INVALID;
							}
							for( int i = 0; i <= pItem->n8ItemKind; i++ )
							{
								JudgeTryDress( pItem->dwTypeID[i] );
							}
							if( pItem->dwPresentID != GT_INVALID )
								JudgeTryDress(pItem->dwPresentID);
							UpdateAvatar();
						}
						return 0;
					}
				}
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if (pWnd == m_pPBLevelLimit )
			{
				if( pEvent->dwParam1 == EGUIBS_PushDown )
				{
					m_bLevelLimit = TRUE;
				}
				else
				{
					m_bLevelLimit = FALSE;
				}
				if ( EMIT_Package != m_eClickType )
				{
					SetSubMenuText(m_eClickType);
					RefreshMallItemFlag();
					m_dwSelectItemID = GT_INVALID;
					JudgeShowMallItem(m_eClickType, m_n8SecondMenu);
					pWnd->SetRefresh(TRUE);
				}
				break;
			}
			if( pEvent->dwParam1 != 1 )
				return 0;

			if ( EGUIBS_PushDown == m_pPBtnPackage->GetState()
			  || EGUIBS_PushDown == m_pPBtnPresent->GetState()
			  || EGUIBS_PushDown == m_pPBtnMark->GetState() )   //礼包、积分、赠卷屏蔽赠送以及购物车
			{
				if ( P_VALID(m_pBtnPresent) )
				{
					m_pBtnPresent->SetEnable(FALSE);
					tagGameEvent eventGift(_T("Close_Gift_UI"), NULL);
					m_pFrameMgr->SendEvent(&eventGift);
				} 

				HideAddToShopingCart();
			} 
			else
			{
				if(m_pSession->IsEnableShopingCart())
					HideAddToShopingCart(FALSE);
				if ( P_VALID(m_pBtnPresent) )
				{
					m_pBtnPresent->SetEnable(TRUE);
				} 
			}
			//SetOperateState();
			//商品区
			if( pWnd == m_pPBtnMall )
			{
				m_eClickType = EMIT_All;
				m_eShopingDistrict = Shopping_Item;
				RefreshMallItemFlag();
				m_dwSelectItemID = GT_INVALID;
				m_pStcGoodsBack->SetInvisible(FALSE);
				m_pStcYuanbaoBack->SetInvisible(TRUE);
				m_n8SecondMenu = 0; //二级分页清零
				//m_pWndNotice->SetInvisible(TRUE);
				////团购隐藏
				//GroupBuyFrame* pFrame = MallMgr::Inst()->GetGroupBuyFrame();
				//if( P_VALID(pFrame) )
				//{
				//	pFrame->ShowWnd();
				//}			
			}
			//元宝交换区
			else if( pWnd == m_pPBtnYuanBaoTrade )
			{
				if ( m_nPolicy != 5 )
				{
					RefreshMallItemFlag();
					m_dwSelectItemID = GT_INVALID;
					m_pStcGoodsBack->SetInvisible(TRUE);
					//元宝交易显示s
					m_pStcYuanbaoBack->SetInvisible(FALSE);
					//m_pWndNotice->SetInvisible(TRUE);
					YuanbaoTradeMgr::Inst()->BeginNC_SynYBTradeInfo();
					////团购隐藏
					//GroupBuyFrame* pFrame = MallMgr::Inst()->GetGroupBuyFrame();
					//if( P_VALID(pFrame) )
					//{
					//	pFrame->ShowWnd();
					//}
				}
				m_n8SecondMenu = 0; //二级分页清零
			}
			//1元计划
			else if (_tcsnccmp(pWnd->GetName().c_str(), _T("bag"), 3) == 0)
			{
				for(int i=0; i<LEVEL_NUM_CHOOSE; ++i)
				{
					if( pWnd == m_pGPBLevel[i] )
					{
						BOOL bGlint = m_pGPBLevel[i]->IsGlintNow();
						if (bGlint)
						{
							m_pGPBLevel[i]->SetGlintStart(FALSE);
							MallMgr::Inst()->SetIndexForClint(i,FALSE);
						}
						tagOpenLevelPromoteFrame event(_T("OpenLevelPromoteFrame"), this);
						event.nIndex = i;
						m_pFrameMgr->SendEvent(&event);
					}
					else
					{
						m_pGPBLevel[i]->SetState(EGUIBS_PopUp);
					}
				}
				
			}
			////公告区
			//else if( pWnd == m_pPBtnNotice )
			//{
			//	//隐藏其他3个区
			//	m_pStcGoodsBack->SetInvisible(TRUE);
			//	m_pStcYuanbaoBack->SetInvisible(TRUE);				
			//	GroupBuyFrame* pFrame = MallMgr::Inst()->GetGroupBuyFrame();
			//	if( P_VALID(pFrame) )
			//	{
			//		pFrame->ShowWnd();
			//	}
			//	ShowMallNotice(TRUE);
			//}
			////团购区
			//else if( pWnd == m_pPBtnGroupBuy )
			//{
			//	RefreshMallItemFlag();
			//	m_pStcGoodsBack->SetInvisible(TRUE);
			//	m_pStcYuanbaoBack->SetInvisible(TRUE);
			//	m_pWndNotice->SetInvisible(TRUE);
			//	//响应团购
			//	GroupBuyFrame* pFrame = MallMgr::Inst()->GetGroupBuyFrame();
			//	if( P_VALID(pFrame) )
			//	{
			//		pFrame->ShowWnd(GroupBuyFrame::EGroupBuy_Info);
			//	}

			//}
			else if( pWnd == m_pPBtnAll )
			{
				m_eClickType = EMIT_All;
				m_eShopingDistrict = Shopping_Item;
				m_pBtnBuy->SetEnable(true);
				if ( m_nPolicy == 5 )
				{
					m_pBtnBuy->SetEnable(false);
				}
				JudgeShowMallItem(EMIT_All, 0);
				RefreshMallItemFlag();
				//如果为外部调用，m_dwSelectItemID不清
				if (m_bIsExterior)
				{
					m_bIsExterior = FALSE;
				}
				//商城内换区就要清
				else
				{
					m_dwSelectItemID = GT_INVALID;
				}
				SetSubMenuText(EMIT_All);
				m_n8SecondMenu = 0; //二级分页清零
			}
			else if( pWnd == m_pPBtnPackage )
			{
				m_eClickType = EMIT_Package;
				m_eShopingDistrict = Shopping_Item;
				m_pBtnBuy->SetEnable(true);
				if ( m_nPolicy == 5 )
				{
					m_pBtnBuy->SetEnable(false);
				}
				JudgeShowMallItem(EMIT_Package, 0);
				RefreshMallItemFlag();
				m_dwSelectItemID = GT_INVALID;
				SetSubMenuText(EMIT_Package);
				m_n8SecondMenu = 0; //二级分页清零
			}
			else
			{
				m_n8SecondMenu = 0; //二级分页清零
				if (_tcsnccmp(pWnd->GetName().c_str(), _T("page"), 4) == 0)
				{
					for(int i=0; i<MALL_ITEM_SUBTYPE-2; i++)
					{
						if( pWnd == m_pPBtnSubMall[i] )
						{
							m_eClickType = (EMallItemType)i;
							m_eShopingDistrict = Shopping_Item;
							SetSubMenuText(i);
							if(i!=5)           //modify by xuepeng  2012.2.6  实现商城的活动庆典默认显示为活动
								m_pPBtnMallType2[0]->SetState(EGUIBS_PushDown);
							else
								m_pPBtnMallType2[1]->SetState(EGUIBS_PushDown);//modify end
							RefreshMallItemFlag();
							m_dwSelectItemID = GT_INVALID;
							JudgeShowMallItem(i, 0);
							m_pBtnBuy->SetEnable(true);
						}
					}
					if( pWnd == m_pPBtnPresent ) //<!F计划 赠卷，原社保点
					{
						m_eClickType = EMIT_Volume;
						m_eShopingDistrict = Shoppint_Coupons;
						SetSubMenuText(EMIT_Volume);
						m_pPBtnMallType2[0]->SetState(EGUIBS_PushDown);
						RefreshMallItemFlag();
						m_dwSelectItemID = GT_INVALID;
						JudgeShowMallItem(6, 0); //EMIT_Volume
						m_pBtnBuy->SetEnable(true);

						tagGameEvent eventGift(_T("Close_Gift_UI"), NULL);
						m_pFrameMgr->SendEvent(&eventGift);
					}
					if( pWnd == m_pPBtnMark ) //<!F计划 积分
					{
						m_eClickType = EMIT_Mark;
						m_eShopingDistrict = Shopping_VipPoint;
						SetSubMenuText(EMIT_Mark);
						m_pPBtnMallType2[0]->SetState(EGUIBS_PushDown);
						JudgeShowMallItem(7, 0);
						RefreshMallItemFlag();
						m_dwSelectItemID = GT_INVALID;
						m_pBtnBuy->SetEnable(true);

						tagGameEvent eventGift(_T("Close_Gift_UI"), NULL);
						m_pFrameMgr->SendEvent(&eventGift);
					}
				}

				// 二级分类按钮
				if (_tcsnccmp(pWnd->GetName().c_str(), _T("optbutt"), 6) == 0)
				{
					for (int i = 0; i < MALL_ITEM_SECOND_SUBTYPE; ++i)
					{
						if (pWnd == m_pPBtnMallType2[i])
						{
							JudgeShowMallItem(m_nCurMenu, i);
							m_n8SecondMenu = i;
							RefreshMallItemFlag();
							m_dwSelectItemID = GT_INVALID;
						}
					}
				}
			}
			
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if( _tcsnccmp(pWnd->GetName().c_str(), _T("pic"), 3) == 0 )
			{
				for(int i=0; i<MALL_MAXITEM_PERPAGE; ++i)
				{
					for(int j=0; j<MALL_ITEM_STATE; ++j)
					{
						if( pWnd == m_pStcState[i][j] )
						{
							ShowMallGoodsTips(i, j);
							return 0;
						}
					}
				}
				
				for( int i=0; i<MALL_MAX_PROMOTE_ITEM; ++i )
				{
					for( int j=0; j<MALL_PROMOTE_ITEM_STATE; ++j )
					{
						if( pWnd == m_pProItemState[i][j] )
						{
							ShowMallPromoteGoodsTips(i, j);
							return 0;
						}
					}
				}

				for( int i=0; i<MALL_MAXPACK_PERPAGE; i++ )
				{
					for( int j=0; j<MALL_PACKPRO_STATE; j++ )
					{
						if( pWnd == m_pStcPackState[i][j] )
						{
							ShowMallPackTips(i, j);
							return 0;
						}
					}
				}
				
			}

			if( _tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0 )
			{
				for( int i=0; i<MALL_MAXITEM_PERPAGE; ++i )
				{	
					if( pWnd == m_pUnitItem[i] )  
					{
						DWORD dwID =  m_pUnitItem[i]->GetBtnData().GetData2();
						ShowTip(m_pUnitItem[i], ToolTipCreator::Inst()->GetItemTips(dwID).c_str());
						return 0;
					}
				}

				for( int i=0; i<MALL_MAX_PROMOTE_ITEM; ++i )
				{
					if( pWnd == m_pUnitProItem[i] )
					{
						DWORD dwID =  m_pUnitProItem[i]->GetBtnData().GetData2();
						ShowTip(m_pUnitProItem[i], ToolTipCreator::Inst()->GetItemTips(dwID).c_str());
						return 0;
					}
				}

				for( int i=0; i<MALL_MAXPACK_PERPAGE; i++ )
				{
					for( int j=0; j<MALL_PACKPRO_NUM; j++ )
					{
						if( pWnd == m_pUnitPack[i][j] )
						{
							DWORD dwID =  m_pUnitPack[i][j]->GetBtnData().GetData2();
							ShowTip(m_pUnitPack[i][j], ToolTipCreator::Inst()->GetItemTips(dwID).c_str());
							return 0;
						}
					}
				}

				if( pWnd == m_pFetchUnitItem ) 
				{
					DWORD dwID = m_pFetchUnitItem->GetBtnData().GetData1();
					ShowTip(m_pFetchUnitItem, ToolTipCreator::Inst()->GetItemTips(dwID).c_str());
					return 0;
				}
			}

			for(int m = 0; m < Mall_ShopingCart_Num; m++)
			{
				if(pWnd == m_pIStcShopingCartItem[m])
				{
					if(m_pShopingItem[m] != NULL)
					{
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pShopingItem[m]->dwTypeID).c_str());
						return 0;
					}
				}

				

			}

			for(int n = 0; n < Mall_ShopingCart_RewardNum; n++)
			{
				if(pWnd == m_pIStcShopingCartReward[n])
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_dwMallCfg[EMCA_BuyActivityRewardItem0+n]).c_str());
					return 0;
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(NULL, _T(""));
		}
		break;
	case EGUIE_ItemClick:
		for( int i = 0; i < MALL_DAY_PART; i++ )
		{
			if( pWnd == m_pCBDayFation[i]->GetListBox() )
				JudgeTryDress(m_dwTryDressID[i]);
		}
		break;
	}
	return 1;
}

void MallFrame::RefreshMallItemFlag()
{
	/*tstring strPic = _T("data\\ui\\common\\l_icon.bmp");
	tstring strPicPack = _T("data\\ui\\mall\\L_kuangjia-d.bmp");
	for(int i=0; i<MALL_MAXITEM_PERPAGE; i++)
	{
		m_pUnitItemSel[i]->SetPic(strPic);
	}
	for(int i=0; i<MALL_MAX_PROMOTE_ITEM; i++)
	{
		m_pUnitProItemSel[i]->SetPic(strPic);
	}
	for(int i=0; i<MALL_MAXPACK_PERPAGE; i++)
	{
		m_pUnitPackBack[i]->SetPic(strPicPack);
	}
	m_pStcFetchSel->SetPic(strPic);*/
	for(int i=0; i<MALL_MAXITEM_PERPAGE; i++)
	{
		m_pUnitItemBack[i]->SetPic(szUnpickItem);
	}
	for(int i=0; i<MALL_MAXPACK_PERPAGE; i++)
	{
		m_pUnitPackBack[i]->SetPic(szUnselectPack);
	}
	for(int i=0; i<MALL_MAX_PROMOTE_ITEM_PerPage; i++)
	{
		m_pProItemBack[i]->SetPic(szUnpickItem);
	}
	//m_dwSelectItemID = GT_INVALID;
}

void MallFrame::SetYuanBao()
{ 
	if(P_VALID(m_pStcYuanBao))
	{
		TCHAR szBuff[X_SHORT_NAME] = {0};
		_itot(CurrencyMgr::Inst()->GetBagYuanbao(), szBuff, 10);
		m_pStcYuanBao->SetText(szBuff) ;
	}
}

VOID MallFrame::SetMoney(INT64 n64Value)
{
	TCHAR szTmp[X_SHORT_NAME] = {0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), n64Value/GOLD2SILVER);
	m_pStcGold->SetText(szTmp);
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), n64Value%GOLD2SILVER);
	m_pStcSilver->SetText(szTmp);

	m_pStcGold->SetRefresh(TRUE);
	m_pStcSilver->SetRefresh(TRUE);
}

void MallFrame::SetExchangeVolume()
{
	if( P_VALID(m_pExchangeVolume) ) 
	{
		TCHAR szBuff[X_SHORT_NAME] = {0};
		_itot(CurrencyMgr::Inst()->GetExchangeVolume(), szBuff, 10);
		m_pExchangeVolume->SetText(szBuff);
	}
}

void MallFrame::SetMark()
{
	if( P_VALID(m_pStcMark) ) 
	{
		TCHAR szBuff[X_SHORT_NAME] = {0};
		_itot(CurrencyMgr::Inst()->GetMark(), szBuff, 10);
		m_pStcMark->SetText(szBuff);
	}
}

void MallFrame::ClassifiedMallItem()
{
	// 先生成辅助查询表
	// 注意，一级分类是从0开始的，要做从原型数据1到0的转换，二级分类不用转换
	const multimap<DWORD, tagMallTitle2Data>* pMap1to2;
	pMap1to2 = MallData::Inst()->GetMall1to2Data();

	for (int i = 0; i < MALL_ITEM_SUBTYPE; ++i)
	{
		multimap<DWORD, tagMallTitle2Data>::const_iterator itBg, itEd;
		itBg = pMap1to2->lower_bound(i + 1);
		itEd = pMap1to2->upper_bound(i + 1);

		int index = 0;
		while (itBg != itEd)
		{
			if ( i== EMIT_Mark || i == EMIT_Volume )
			{
				m_map1and2toBtIndex.insert(make_pair(make_pair(i, itBg->second.nIndex), 0));
				m_map1andBtIndexTo2.insert(make_pair(make_pair(i, 0), itBg->second.nIndex));
			}
			else
			{
				m_map1and2toBtIndex.insert(make_pair(make_pair(i, itBg->second.nIndex), index));
				m_map1andBtIndexTo2.insert(make_pair(make_pair(i, index), itBg->second.nIndex));
			}
			++itBg;
			++index;
		}
	}

	//分类时先clear
	for(int i=0; i<MALL_ITEM_SUBTYPE; i++)
	{
		for (int j = 0; j < MALL_ITEM_SECOND_SUBTYPE; ++j)
		{
			m_mapMallItem[i][j].Clear();
		}
	}

	tagMallItemProto* item = NULL;
	TMap<DWORD, tagMallItemProto*> mapitem = MallMgr::Inst()->GetMallProto();
	mapitem.ResetIterator();
	while(mapitem.PeekNext(item))
	{
		if( !P_VALID(item)
			|| (item->n8Kind-1)>MALL_ITEM_SUBTYPE 
			|| item->n8Kind==0
			|| (item->dwID==GT_INVALID || item->dwID==0) )
			continue;

		int btIndex = 0;
		map<pair<DWORD, DWORD>, int>::iterator it = m_map1and2toBtIndex.find(make_pair(item->n8Kind - 1, item->n8Kind2));
		int   nBtnNum = 0;					//按钮编号
		INT8  nTypeFirst  = item->n8Kind;	//一级分类
		INT8  nTypeSecond = item->n8Kind2;	//二级分类
		if (it != m_map1and2toBtIndex.end())
		{
			nBtnNum = it->second;	
			if (nBtnNum < MALL_ITEM_SECOND_SUBTYPE && nTypeFirst-1 != MALL_ITEM_SUBTYPE )
			{
				if ( (nTypeFirst-1) != EMIT_Mark		// 积分 EMIT_Mark
				  && (nTypeFirst-1) != EMIT_Volume )		//赠卷 EMIT_Volume 原社保点
				{
					m_mapMallItem[nTypeFirst-1][nBtnNum].Add(item->dwID, item);
				}
				else
				{
					m_mapMallItem[nTypeFirst-1][0].Add(item->dwID, item);
				}
			}
		}
		//if( JudgeExchangePro(item)) //赠卷 EMIT_Volume 原社保点
		//	m_mapMallItem[EMIT_Volume][0].Add(item->dwID, item);
	}
	
	//计算每个分类下的商品共个有多少页
	for(int i=0; i<MALL_ITEM_SUBTYPE; i++)
	{
		for (int j = 0; j < MALL_ITEM_SECOND_SUBTYPE; ++j)
		{
			m_nCurMaxPage[i][i] = m_mapMallItem[i][j].Size() / MALL_MAXITEM_PERPAGE;
		}
	}

	this->ShowPromoteMallItem();  //推荐
	this->RefreshFreeItem();
	this->SetYuanBao();
	SetMoney( CurrencyMgr::Inst()->GetBagSilver() );
	this->SetExchangeVolume();
	this->SetMark();
	this->SetGiftPlan();
	m_pPBtnMall->SetState(EGUIBS_PushDown);
	m_pPBtnAll->SetState(EGUIBS_PushDown);
	/*LocalPlayer* pLocal = RoleMgr::Inst()->GetLocalPlayer();
	if( P_VALID(pLocal)
		&& !pLocal->IsInGuild() )
	{
		m_pPBtnGroupBuy->SetEnable(false);
		m_pBtnGroupBuy->SetEnable(false);
	}*/
	m_pWnd->FlipToTop();
}

void MallFrame::ShowCurPageMallItemByType( INT nPage )
{
	HideCtrlMallItem();
	m_nCurPage[m_nCurMenu][m_nCurSubMenu] = nPage;
	TMap<DWORD, tagMallItemProto*>::TMapIterator it = m_mapMallItem[m_nCurMenu][m_nCurSubMenu].Begin();
	int i=0; 
	for(; i<m_nCurPage[m_nCurMenu][m_nCurSubMenu]*MALL_MAXITEM_PERPAGE; ++i)
	{
		if (m_bLevelLimit == TRUE)		//不符合等级限制的不算在内
		{
			tagMallItemProto* pTempProto = it->second;
			if( P_VALID(pTempProto) )
			{
				const tagItemProto* pTempItem = ItemProtoData::Inst()->FindItemProto(pTempProto->dwTypeID);
				if( P_VALID(pTempItem))
				{
					INT nTempRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
					INT nTempMaxLevel = (INT)pTempItem->byMaxUseLevel;
					INT nTempMinLevel = (INT)pTempItem->byMinUseLevel;
					if ( nTempRoleLevel < nTempMinLevel || nTempRoleLevel > nTempMaxLevel )
					{
						--i;
					}		 
				}
			}
		}
		++it;
		if( i > m_mapMallItem[m_nCurMenu][m_nCurSubMenu].Size() )
			break;
	}
	i = 0;
	tagMallItemProto* pProto = NULL;
	DWORD pos = 0;
	TCHAR szBuff[X_LONG_NAME] = {0};

	while(i<MALL_MAXITEM_PERPAGE && m_mapMallItem[m_nCurMenu][m_nCurSubMenu].PeekNext(it, pos, pProto))
	{
		if( P_VALID(pProto) )
		{		
			const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(pProto->dwTypeID);
			const tagItemDisplayInfo *pInfo = ItemProtoData::Inst()->FindItemDisplay(pProto->dwTypeID);
			if( P_VALID(pItem) && P_VALID(pInfo) )
			{
				//是否开启等级筛选
				if ( m_bLevelLimit == TRUE )
				{
					//开启
					INT nRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
					INT nMaxLevel = (INT)pItem->byMaxUseLevel ;
					INT nMinLevel = (INT)pItem->byMinUseLevel;
					if ( nRoleLevel < nMinLevel || nRoleLevel > nMaxLevel )
					{
						continue;
					}
				}	

				m_pUnitItemBack[i]->SetInvisible(FALSE);
				m_pUnitItem[i]->RefreshItem(pProto->dwTypeID, 1, pItem->byQuality);
				m_pUnitItem[i]->SetBtnData(ESBT_Item, pProto->dwID, pProto->dwTypeID, pProto->nIndexInServer);
				m_pStcItemName[i]->SetText(pInfo->szName);

				//赠品
				const tagItemDisplayInfo *pPresentInfo = ItemProtoData::Inst()->FindItemDisplay(pProto->dwPresentID);
				if( P_VALID(pPresentInfo) )
				{
					m_pStcGoodsGift[i]->SetInvisible(FALSE);
					m_pStcGoodsGiftNum[i]->SetInvisible(FALSE);
					tstringstream tss;
					tss<<pPresentInfo->szName<<g_StrTable[_T("Mall_Signal_Multply")]<<pProto->nPresentNum;
					m_pStcGoodsGiftNum[i]->SetText(tss.str().c_str());
				}
				else
				{
					m_pStcGoodsGift[i]->SetInvisible(TRUE);
					m_pStcGoodsGiftNum[i]->SetInvisible(TRUE);
				}
				//剩余数量
				if ( pProto->nNum > 0 )
				{
					DWORD dwNum = pProto->nNum;
					if(SaveAccount::Inst()->GetLoginMode() == 7)
					{
						if(dwNum > MALL_ITEM_PERSONAL_NUM_LIMIT_LINE)
						{
							dwNum -= MALL_ITEM_PERSONAL_NUM_LIMIT_LINE;
							_stprintf(szBuff, g_StrTable[_T("MallTrade_Limit_Num")], dwNum);
							m_pStcGoodsRest[i]->SetText(szBuff);
						}
						else
						{
							_stprintf(szBuff, g_StrTable[_T("MallTrade_Rest_Num")], dwNum);
							m_pStcGoodsRest[i]->SetText(szBuff);
						}
					}
					else
					{
						_stprintf(szBuff, g_StrTable[_T("MallTrade_Rest_Num")], dwNum);
						m_pStcGoodsRest[i]->SetText(szBuff);
					}
					m_pStcGoodsRest[i]->SetInvisible(FALSE);
				}
				else
				{
					m_pStcGoodsRest[i]->SetInvisible(TRUE);
				}
				
				DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pProto->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
				if( dwTime>0 && pProto->dwTimeSaleEnd!=GT_INVALID
					&& pProto->nSalePrice!=GT_INVALID && pProto->nPrice!=pProto->nSalePrice )
				{					
					//_stprintf(szBuff, g_StrTable[_T("MallTrade2")], pProto->nPrice, pProto->nSalePrice);
					//单位类别：0~5：宝， 6：赠卷，7：积分
					if ( m_nCurMenu == EMIT_Mark )
					{
						_stprintf(szBuff, g_StrTable[_T("MallTrade71")], pProto->nPrice);
					} 
					else if ( m_nCurMenu == EMIT_Volume )
					{
						_stprintf( szBuff, g_StrTable[_T("MallTrade70")], pProto->nPrice );
					} 
					else
					{
						_stprintf( szBuff, g_StrTable[_T("MallTrade69")], pProto->nPrice );
					}
					m_pStcGoodsPrice[i]->SetText(szBuff);
					m_pStcGoodsPrice[i]->SetTextColor(0xFF2F5EE8);
					m_pStcGoodsPrice[i]->SetPic(szDeletePic);
					m_pStcGoodsPriceDel[i]->SetPic( szDeletePic, tagRect(0, 0, 53, 15), EGUIAW_Add );
					m_pStcGoodsPriceDel[i]->SetInvisible(FALSE);
					if ( m_nCurMenu == EMIT_Mark )
					{
						_stprintf(szBuff, g_StrTable[_T("MallTrade71")], pProto->nSalePrice );
					} 
					else if ( m_nCurMenu == EMIT_Volume )
					{
						_stprintf( szBuff, g_StrTable[_T("MallTrade70")], pProto->nSalePrice );
					} 
					else
					{
						_stprintf( szBuff, g_StrTable[_T("MallTrade69")], pProto->nSalePrice );
					}
					m_pStcGoodsPriceNow[i]->SetText(szBuff);
					m_pStcGoodsPriceNow[i]->SetTextColor(0xFF3AD352);
					m_pStcGoodsInfoNow[i]->SetInvisible(FALSE);
					m_pStcGoodsPriceNow[i]->SetInvisible(FALSE);
				}
				else 
				{
					//_stprintf(szBuff, g_StrTable[_T("MallTrade3")], pProto->nPrice);
					if ( m_nCurMenu == EMIT_Mark )
					{
						_stprintf(szBuff, g_StrTable[_T("MallTrade71")], pProto->nPrice);
					} 
					else if ( m_nCurMenu == EMIT_Volume )
					{
						_stprintf( szBuff, g_StrTable[_T("MallTrade70")], pProto->nPrice );
					} 
					else
					{
						_stprintf( szBuff, g_StrTable[_T("MallTrade69")], pProto->nPrice );
					}
					m_pStcGoodsPrice[i]->SetText(szBuff);
					m_pStcGoodsPrice[i]->SetTextColor(0xFF3AD352);
					m_pStcGoodsPrice[i]->SetNoPic();
					m_pStcGoodsPriceDel[i]->SetInvisible(TRUE);
					m_pStcGoodsPriceNow[i]->SetInvisible(TRUE);
					m_pStcGoodsInfoNow[i]->SetInvisible(TRUE);
				}
				//m_pStcInfo[i]->SetText(szBuff);
				
				INT  nHaveState = 0;
				for (int n=0; n<MALL_ITEM_STATE; n++)
				{
					m_pStcState[i][n]->SetInvisible(TRUE);
				}		
				//新
				if( pProto->bNew )
				{
					m_pStcState[i][nHaveState]->SetInvisible(FALSE);
					m_pStcState[i][nHaveState]->SetPic(szState_New);
					nHaveState++;
				}
				//热
				if( pProto->bHot )
				{
					m_pStcState[i][nHaveState]->SetInvisible(FALSE);
					m_pStcState[i][nHaveState]->SetPic(szState_Hot);
					nHaveState++;
				}
				//促
				if( JudgePromotePro(pProto) )
				{
					m_pStcState[i][nHaveState]->SetInvisible(FALSE);
					m_pStcState[i][nHaveState]->SetPic(szState_Dis);
					nHaveState++;
				}
				////团
				//if( JudgeGroupBuyPro(pProto) )
				//{
				//	m_pStcState[i][EMIST_GroupBuy]->SetPic(szGroupHave);
				//	m_pStcState[i][EMIST_GroupBuy]->SetInvisible(FALSE);
				//}
				//else
				//{
				//	m_pStcState[i][EMIST_GroupBuy]->SetPic(szGroupNot);
				//	m_pStcState[i][EMIST_GroupBuy]->SetInvisible(TRUE);
				//}
				////赠
				//if( JudgePresentPro(pProto) )
				//{
				//	m_pStcState[i][EMIST_Present]->SetPic(szPresentHave);
				//	m_pStcState[i][EMIST_Present]->SetInvisible(FALSE);
				//}
				//else
				//{
				//	m_pStcState[i][EMIST_Present]->SetPic(szPresentNot);
				//	m_pStcState[i][EMIST_Present]->SetInvisible(TRUE);
				//}
				//限
				if( JudgeNumPro(pProto) )
				{
					m_pStcState[i][nHaveState]->SetInvisible(FALSE);
					m_pStcState[i][nHaveState]->SetPic(szState_Lim);
					nHaveState++;
				}
				////兑
				//if( JudgeExchangePro(pProto) )
				//{
				//	m_pStcState[i][EMIST_Exchange]->SetPic(szExchangeHove);
				//	m_pStcState[i][EMIST_Exchange]->SetInvisible(FALSE);
				//}
				//else 
				//{
				//	m_pStcState[i][EMIST_Exchange]->SetPic(szExchangeNot);
				//	m_pStcState[i][EMIST_Exchange]->SetInvisible(TRUE);
				//}
			}
		}
		++i;
	}
	SetCurrentPage();
}

VOID MallFrame::ShowPromoteMallItem()
{
	//根据人物等级筛选所要显示的推荐商品
	MallMgr::Inst()->PromoteItemShowMgr();

	HideCtrlPromoteItem();

	map<DWORD, tagMallPromoteItemShow> mapProItemShow = MallMgr::Inst()->GetProItemShow();
	map<DWORD, tagMallPromoteItemShow>::iterator   iterShow = mapProItemShow.begin(); 
	
	while (( iterShow != mapProItemShow.end() ) && ( !mapProItemShow.empty() ) ) 
	{
		//在推荐商品map中查找当前页对应的item项
		if ( m_iPromoteItemShowPage == (iterShow->second).iPage)
		{
			int i = (iterShow->second).iNum - 1;
			if ( i>= MALL_MAX_PROMOTE_ITEM_PerPage )
			{
				iterShow++;
				continue;
			}
			DWORD  dwProItemID = iterShow->first;   //即(iterShow->second).dwProItemID;  
			tagMallItemProto* pProto = MallMgr::Inst()->GetMallItemByTypeid(dwProItemID);
				
			if( !P_VALID(pProto)) 
			{
				iterShow++;
				continue;
			}

			if( pProto->n8Kind-1 == EMIT_Volume 
			 || pProto->n8Kind-1 == EMIT_Mark 
			 || pProto->n8Kind   == EMIT_GiftPlan )
			{
				iterShow++;
				continue;
			}

			const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(pProto->dwTypeID);
			const tagItemDisplayInfo *pInfo = ItemProtoData::Inst()->FindItemDisplay(pProto->dwTypeID);
			if( !P_VALID(pItem) || !P_VALID(pInfo) )
			{
				iterShow++;
				continue;
			}

			m_pProItemBack[i]->SetInvisible(FALSE);
			m_pUnitProItem[i]->RefreshItem(pProto->dwTypeID, 1, pItem->byQuality);
			m_pUnitProItem[i]->SetBtnData(ESBT_Item, pProto->dwID, pProto->dwTypeID, pProto->nIndexInServer);
			m_pProItemName[i]->SetText(pInfo->szName);
			
			DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pProto->dwTimeSaleEnd)), 
				ServerTime::Inst()->CalCurrentServerDwordTime());
			TCHAR szBuff[X_LONG_NAME] = {0};
			if( dwTime>0
				&& pProto->dwTimeSaleEnd!=GT_INVALID
				&& pProto->nSalePrice!=GT_INVALID 
				&& pProto->nPrice!=pProto->nSalePrice )
			{					
				//_stprintf(szBuff, g_StrTable[_T("MallTrade2")], pProto->nPrice, pProto->nSalePrice);
				_stprintf(szBuff, g_StrTable[_T("MallTrade69")], pProto->nPrice);
				m_pStcProPrice[i]->SetText(szBuff);
				m_pStcProPrice[i]->SetTextColor(0xFF2F5EE8);
				m_pStcProPrice[i]->SetPic(szDeletePic);
				m_pStcProPriceDel[i]->SetPic( szDeletePic, tagRect(0, 0, 53, 15), EGUIAW_Add );
				m_pStcProPriceDel[i]->SetInvisible(FALSE);
				_stprintf(szBuff, g_StrTable[_T("MallTrade69")], pProto->nSalePrice);
				m_pStcProPriceNow[i]->SetText(szBuff);
				m_pStcProPriceNow[i]->SetTextColor(0xFF3AD352);
				m_pStcProInfoNow[i]->SetInvisible(FALSE);
				m_pStcProPriceNow[i]->SetInvisible(FALSE);
			}
			else 
			{
				//_stprintf(szBuff, g_StrTable[_T("MallTrade3")], pProto->nPrice);
				_stprintf(szBuff, g_StrTable[_T("MallTrade69")], pProto->nPrice);
				m_pStcProPrice[i]->SetText(szBuff);
				m_pStcProPrice[i]->SetTextColor(0xFF3AD352);
				m_pStcProPrice[i]->SetNoPic();
				m_pStcProPriceDel[i]->SetInvisible(TRUE);
				m_pStcProPriceNow[i]->SetInvisible(TRUE);
				m_pStcProInfoNow[i]->SetInvisible(TRUE);
			}
			
			//赠品
			const tagItemDisplayInfo *pPresentInfo = ItemProtoData::Inst()->FindItemDisplay(pProto->dwPresentID);
			if( P_VALID(pPresentInfo) )
			{
				m_pStcProGift[i]->SetInvisible(FALSE);
				m_pStcProGiftNum[i]->SetInvisible(FALSE);
				tstringstream tss;
				tss<<pPresentInfo->szName<<g_StrTable[_T("Mall_Signal_Multply")]<<pProto->nPresentNum;
				m_pStcProGiftNum[i]->SetText(tss.str().c_str());
			}
			else
			{
				m_pStcProGift[i]->SetInvisible(TRUE);
				m_pStcProGiftNum[i]->SetInvisible(TRUE);
			}
			//剩余数量
			if ( pProto->nNum > 0 )
			{
				DWORD dwNum = pProto->nNum;
				if(SaveAccount::Inst()->GetLoginMode() == 7)
				{
					if(dwNum > MALL_ITEM_PERSONAL_NUM_LIMIT_LINE)
					{
						dwNum -= MALL_ITEM_PERSONAL_NUM_LIMIT_LINE;
						_stprintf(szBuff, g_StrTable[_T("MallTrade_Limit_Num")], dwNum);	//台湾限购
						m_pStcProRest[i]->SetText(szBuff);
					}
					else
					{
						_stprintf(szBuff, g_StrTable[_T("MallTrade_Rest_Num")], dwNum);		//台湾全服限购
						m_pStcProRest[i]->SetText(szBuff);
					}
				}
				else
				{
					_stprintf(szBuff, g_StrTable[_T("MallTrade_Rest_Num")], dwNum);			//大陆全服限购
					m_pStcProRest[i]->SetText(szBuff);
				}
				m_pStcProRest[i]->SetInvisible(FALSE);
				
			}
			else
			{
				m_pStcProRest[i]->SetInvisible(TRUE);
			}

			INT  nHaveState = 0;
			for (int n=0; n<MALL_PROMOTE_ITEM_STATE; n++)
			{
				m_pProItemState[i][n]->SetInvisible(TRUE);
			}
			//新
			if( pProto->bNew )
			{
				m_pProItemState[i][nHaveState]->SetInvisible(FALSE);
				m_pProItemState[i][nHaveState]->SetPic(szState_New);
				nHaveState++;
			}
			 
			//限
			if( JudgeNumPro(pProto) )
			{
				m_pProItemState[i][nHaveState]->SetInvisible(FALSE);
				m_pProItemState[i][nHaveState]->SetPic(szState_Lim);
				nHaveState++;
			}
			 
		}
		iterShow++;
	}

	//显示页码
	tstringstream    tempPageTstr1;
	tempPageTstr1 << m_iPromoteItemShowPage<<_T("/")<<MallMgr::Inst()->GetMaxPromotePage();
	m_pStcProPageText->SetText(tempPageTstr1.str().c_str());

}

void MallFrame::ShowCurPageMallItemByAll(INT nPage)
{
	HideCtrlMallItem();
	m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE] = nPage;
	TMap<DWORD, tagMallItemProto*> mapAll = MallMgr::Inst()->GetMallProto();
	TMap<DWORD, tagMallItemProto*>::TMapIterator it = mapAll.Begin();
	int i=0; 
	for(; i<m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE]*MALL_MAXITEM_PERPAGE; ++i)
	{
		bool bFlag = false;
		while( P_VALID(it->second) && it != mapAll.End() //JudgeExchangePro(it->second) &&
			 &&(it->second->n8Kind-1 == EMIT_Volume 
			 || it->second->n8Kind-1 == EMIT_Mark
			 || it->second->n8Kind	 == EMIT_GiftPlan) )
		{
			bFlag = true;
			++it;
		}
		//找到符合等级限制的
		if ( m_bLevelLimit == TRUE )	//if (m_pPBLevelLimit->GetState() == EGUIBS_PushDown)
		{
			tagMallItemProto* pTempProto = it->second;
			if( P_VALID(pTempProto) )
			{
				const tagItemProto* pTempItem = ItemProtoData::Inst()->FindItemProto(pTempProto->dwTypeID);
				if( P_VALID(pTempItem))
				{
					INT nTempRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
					INT nTempMaxLevel = (INT)pTempItem->byMaxUseLevel;
					INT nTempMinLevel = (INT)pTempItem->byMinUseLevel;
					if ( nTempRoleLevel < nTempMinLevel || nTempRoleLevel > nTempMaxLevel )
					{
						bFlag = true;
						++it;
					}		 
				}
			}
		}

		if( !bFlag ) //符合的
			++it;
		else
			--i;

		
		if( i > mapAll.Size() )
			break;
	}
	i = 0;
	tagMallItemProto* pProto=NULL;
	DWORD pos = 0;
	TCHAR szBuff[X_LONG_NAME] = {0};
	while ( i<MALL_MAXITEM_PERPAGE && mapAll.PeekNext(it, pos, pProto) )
	{
		if( P_VALID(pProto) )
		{
			if( (pProto->n8Kind-1 == EMIT_Volume ) 
			 || (pProto->n8Kind-1 == EMIT_Mark)
			 || (pProto->n8Kind	  == EMIT_GiftPlan) ) //JudgeExchangePro(pProto) &&
				continue;
			const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(pProto->dwTypeID);
			const tagItemDisplayInfo *pInfo = ItemProtoData::Inst()->FindItemDisplay(pProto->dwTypeID);
			if( P_VALID(pItem) && P_VALID(pInfo) )
			{
				//是否开启等级筛选
				if ( m_bLevelLimit == TRUE )
				{
					//开启
					INT nRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
					INT nMaxLevel = (INT)pItem->byMaxUseLevel ;
					INT nMinLevel = (INT)pItem->byMinUseLevel;
					if ( nRoleLevel < nMinLevel || nRoleLevel > nMaxLevel )
					{
						continue;
					}
				}	

				m_pUnitItemBack[i]->SetInvisible(FALSE);
				m_pUnitItem[i]->RefreshItem(pProto->dwTypeID, 1, pItem->byQuality);
				m_pUnitItem[i]->SetBtnData(ESBT_Item, pProto->dwID, pProto->dwTypeID, pProto->nIndexInServer);
				m_pStcItemName[i]->SetText(pInfo->szName);
				
				//赠品
				const tagItemDisplayInfo *pPresentInfo = ItemProtoData::Inst()->FindItemDisplay(pProto->dwPresentID);
				if( P_VALID(pPresentInfo) )
				{
					m_pStcGoodsGift[i]->SetInvisible(FALSE);
					m_pStcGoodsGiftNum[i]->SetInvisible(FALSE);
					tstringstream tss;
					tss<<pPresentInfo->szName<<g_StrTable[_T("Mall_Signal_Multply")]<<pProto->nPresentNum;
					m_pStcGoodsGiftNum[i]->SetText(tss.str().c_str());
				}
				else
				{
					m_pStcGoodsGift[i]->SetInvisible(TRUE);
					m_pStcGoodsGiftNum[i]->SetInvisible(TRUE);
				}
				//剩余数量
				if ( pProto->nNum > 0 )
				{
					DWORD dwNum = pProto->nNum;
					if(SaveAccount::Inst()->GetLoginMode() == 7)
					{
						if(dwNum > MALL_ITEM_PERSONAL_NUM_LIMIT_LINE)
						{
							dwNum -= MALL_ITEM_PERSONAL_NUM_LIMIT_LINE;
							_stprintf(szBuff, g_StrTable[_T("MallTrade_Limit_Num")], dwNum);
							m_pStcGoodsRest[i]->SetText(szBuff);
						}
						else
						{
							_stprintf(szBuff, g_StrTable[_T("MallTrade_Rest_Num")], dwNum);
							m_pStcGoodsRest[i]->SetText(szBuff);
						}
					}
					else
					{
						_stprintf(szBuff, g_StrTable[_T("MallTrade_Rest_Num")], dwNum);
						m_pStcGoodsRest[i]->SetText(szBuff);
					}
					m_pStcGoodsRest[i]->SetInvisible(FALSE);
				}
				else
				{
					m_pStcGoodsRest[i]->SetInvisible(TRUE);
				}
				
				DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pProto->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
				if( dwTime>0 && pProto->dwTimeSaleEnd!=GT_INVALID
					&& pProto->nSalePrice!=GT_INVALID && pProto->nPrice!=pProto->nSalePrice )
				{					
					//_stprintf(szBuff, g_StrTable[_T("MallTrade2")], pProto->nPrice, pProto->nSalePrice);
					_stprintf(szBuff, g_StrTable[_T("MallTrade69")], pProto->nPrice);
					m_pStcGoodsPrice[i]->SetText(szBuff);
					m_pStcGoodsPrice[i]->SetTextColor(0xFF2F5EE8);
					m_pStcGoodsPrice[i]->SetPic(szDeletePic);
					m_pStcGoodsPriceDel[i]->SetPic( szDeletePic, tagRect(0, 0, 53, 15), EGUIAW_Add );
					m_pStcGoodsPriceDel[i]->SetInvisible(FALSE);
					_stprintf(szBuff, g_StrTable[_T("MallTrade69")], pProto->nSalePrice);
					m_pStcGoodsPriceNow[i]->SetText(szBuff);
					m_pStcGoodsPriceNow[i]->SetTextColor(0xFF3AD352);
					m_pStcGoodsInfoNow[i]->SetInvisible(FALSE);
					m_pStcGoodsPriceNow[i]->SetInvisible(FALSE);
				}
				else 
				{
					//_stprintf(szBuff, g_StrTable[_T("MallTrade3")], pProto->nPrice);
					_stprintf(szBuff, g_StrTable[_T("MallTrade69")], pProto->nPrice);
					m_pStcGoodsPrice[i]->SetText(szBuff);
					m_pStcGoodsPrice[i]->SetTextColor(0xFF3AD352);
					m_pStcGoodsPrice[i]->SetNoPic();
					m_pStcGoodsPriceDel[i]->SetInvisible(TRUE);
					m_pStcGoodsPriceNow[i]->SetInvisible(TRUE);
					m_pStcGoodsInfoNow[i]->SetInvisible(TRUE);
				}
				//m_pStcInfo[i]->SetText(szBuff);
				
				INT  nHaveState = 0;
				for (int n=0; n<MALL_ITEM_STATE; n++)
				{
					m_pStcState[i][n]->SetInvisible(TRUE);
				}
				//新
				if( pProto->bNew )
				{
					m_pStcState[i][nHaveState]->SetInvisible(FALSE);
					m_pStcState[i][nHaveState]->SetPic(szState_New);
					nHaveState++;
				}
				//热
				if( pProto->bHot )
				{
					m_pStcState[i][nHaveState]->SetInvisible(FALSE);
					m_pStcState[i][nHaveState]->SetPic(szState_Hot);
					nHaveState++;
				}
				//促
				if( JudgePromotePro(pProto) )
				{
					m_pStcState[i][nHaveState]->SetInvisible(FALSE);
					m_pStcState[i][nHaveState]->SetPic(szState_Dis);
					nHaveState++;
				}
				////团
				//if( JudgeGroupBuyPro(pProto) )
				//{
				//	m_pStcState[i][EMIST_GroupBuy]->SetPic(szGroupHave);
				//	m_pStcState[i][EMIST_GroupBuy]->SetInvisible(FALSE);
				//}
				//else
				//{
				//	m_pStcState[i][EMIST_GroupBuy]->SetPic(szGroupNot);
				//	m_pStcState[i][EMIST_GroupBuy]->SetInvisible(TRUE);
				//}
				////赠
				//if( JudgePresentPro(pProto) )
				//{
				//	m_pStcState[i][EMIST_Present]->SetPic(szPresentHave);
				//	m_pStcState[i][EMIST_Present]->SetInvisible(FALSE);
				//}
				//else
				//{
				//	m_pStcState[i][EMIST_Present]->SetPic(szPresentNot);
				//	m_pStcState[i][EMIST_Present]->SetInvisible(TRUE);
				//}
				//限
				if( JudgeNumPro(pProto) )
				{
					m_pStcState[i][nHaveState]->SetInvisible(FALSE);
					m_pStcState[i][nHaveState]->SetPic(szState_Lim);
					nHaveState++;
				}
				////兑
				//if( JudgeExchangePro(pProto) )
				//{
				//	m_pStcState[i][EMIST_Exchange]->SetPic(szExchangeHove);
				//	m_pStcState[i][EMIST_Exchange]->SetInvisible(FALSE);
				//}
				//else 
				//{
				//	m_pStcState[i][EMIST_Exchange]->SetPic(szExchangeNot);
				//	m_pStcState[i][EMIST_Exchange]->SetInvisible(TRUE);
				//}
			}
		}
		++i;
	}
	SetCurrentPage();
}

//显示当前页的礼包
void MallFrame::ShowCurPageMallPack(INT nPage)
{
	HideCtrlMallPack();
	m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE] = nPage;
	list<tagMallPackProto*> listMallPack = MallMgr::Inst()->GetMallPack();
	list<tagMallPackProto*>::iterator it = listMallPack.begin();
	int i=0; 
	for(; i<m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE]*MALL_MAXPACK_PERPAGE; ++i)
	{
		++it;
		if( i > int(listMallPack.size()) )
			break;
	}
	i = 0;
	tagMallPackProto* pPack = NULL;
	TCHAR szBuff[X_LONG_NAME] = {0};
	TCHAR szTmpPackName[X_SHORT_NAME] = {0};
	while ( i < MALL_MAXPACK_PERPAGE && it != listMallPack.end() ) 
	{
		pPack = *it;
		if( P_VALID(pPack) )
		{
			m_pUnitPackBack[i]->SetInvisible(FALSE);
			for(int j=0; j<MALL_PACKPRO_NUM; j++)
			{
				if( pPack->dwTypeID[j]==GT_INVALID || pPack->dwTypeID[j]==0 )
					continue;
				const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(pPack->dwTypeID[j]);
				const tagItemDisplayInfo *pInfo = ItemProtoData::Inst()->FindItemDisplay(pPack->dwTypeID[j]);
				if( P_VALID(pItem) && P_VALID(pInfo) )  
				{
					m_pUnitPackSel[i][j]->SetInvisible(FALSE);
					m_pUnitPack[i][j]->SetInvisible(FALSE);
					m_pUnitPack[i][j]->RefreshItem(pPack->dwTypeID[j], pPack->byItemNum[j], pItem->byQuality);
					m_pUnitPack[i][j]->SetBtnData(ESBT_Item, pPack->dwID, pPack->dwTypeID[j], pPack->nIndexInServer);	
				}
			}

			//赠品
			const tagItemDisplayInfo *pPresentInfo = ItemProtoData::Inst()->FindItemDisplay(pPack->dwPresentID);
			if( P_VALID(pPresentInfo) )
			{
				m_pStcPackGift[i]->SetInvisible(FALSE);
				m_pStcPackGiftNum[i]->SetInvisible(FALSE);
				tstringstream tss;
				tss<<pPresentInfo->szName<<g_StrTable[_T("Mall_Signal_Multply")]<<pPack->nPresentNum;
				m_pStcPackGiftNum[i]->SetText(tss.str().c_str());
			}
			else
			{
				m_pStcPackGift[i]->SetInvisible(TRUE);
				m_pStcPackGiftNum[i]->SetInvisible(TRUE);
			}
			//剩余数量
			if ( pPack->nNum > 0 )
			{
				DWORD dwNum = pPack->nNum;
				if(SaveAccount::Inst()->GetLoginMode() == 7)
				{
					if(dwNum > MALL_ITEM_PERSONAL_NUM_LIMIT_LINE)
					{
						dwNum -= MALL_ITEM_PERSONAL_NUM_LIMIT_LINE;
						_stprintf(szBuff, g_StrTable[_T("MallTrade_Limit_Num")], dwNum);
						m_pStcPackRest[i]->SetText(szBuff);
					}
					else
					{
						_stprintf(szBuff, g_StrTable[_T("MallTrade_Rest_Num")], dwNum);
						m_pStcPackRest[i]->SetText(szBuff);
					}
				}
				else
				{
					_stprintf(szBuff, g_StrTable[_T("MallTrade_Rest_Num")], dwNum);
					m_pStcPackRest[i]->SetText(szBuff);
				}
				m_pStcPackRest[i]->SetInvisible(FALSE);
			}
			else
			{
				m_pStcPackRest[i]->SetInvisible(TRUE);
			}

			DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pPack->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
			if( dwTime>0 && pPack->dwTimeSaleEnd!=GT_INVALID
				&& pPack->nSalePrice!=GT_INVALID && pPack->nPrice!=pPack->nSalePrice )
			{					
				//_stprintf(szBuff, g_StrTable[_T("MallTrade2")], pPack->nPrice, pPack->nSalePrice);
				_stprintf(szBuff, g_StrTable[_T("MallTrade69")], pPack->nPrice);
				m_pStcPackPrice[i]->SetText(szBuff);
				m_pStcPackPrice[i]->SetTextColor(0xFF2F5EE8);
				m_pStcPackPrice[i]->SetPic(szDeletePic);
				m_pStcPackPriceDel[i]->SetPic( szDeletePic, tagRect(0, 0, 53, 15), EGUIAW_Add );
				m_pStcPackPriceDel[i]->SetInvisible(FALSE);
				_stprintf(szBuff, g_StrTable[_T("MallTrade69")], pPack->nSalePrice);
				m_pStcPackPriceNow[i]->SetText(szBuff);
				m_pStcPackPriceNow[i]->SetTextColor(0xFF3AD352);
				m_pStcPackInfoNow[i]->SetInvisible(FALSE);
				m_pStcPackPriceNow[i]->SetInvisible(FALSE);
			}
			else 
			{
				//_stprintf(szBuff, g_StrTable[_T("MallTrade3")], pPack->nPrice);
				_stprintf(szBuff, g_StrTable[_T("MallTrade69")], pPack->nPrice);
				m_pStcPackPrice[i]->SetText(szBuff);
				m_pStcPackPrice[i]->SetTextColor(0xFF3AD352);
				m_pStcPackPrice[i]->SetNoPic();
				m_pStcPackPriceDel[i]->SetInvisible(TRUE);
				m_pStcPackPriceNow[i]->SetInvisible(TRUE);
				m_pStcPackInfoNow[i]->SetInvisible(TRUE);
			}
			//m_pStcPackInfo[i]->SetText(szBuff);	
			ZeroMemory( szTmpPackName, X_SHORT_NAME );
			_tcscpy( szTmpPackName, pPack->tszPackName );
			m_pStcPackName[i]->SetText( szTmpPackName );

			INT  nHaveState = 0;
			for (int n=0; n<MALL_ITEM_STATE; n++)
			{
				m_pStcPackState[i][n]->SetInvisible(TRUE);
			}
			//新
			if( pPack->bNew )
			{
				m_pStcPackState[i][nHaveState]->SetInvisible(FALSE);
				m_pStcPackState[i][nHaveState]->SetPic(szState_New);
				nHaveState++;
			}
			//热
			if( pPack->bHot )
			{
				m_pStcPackState[i][nHaveState]->SetInvisible(FALSE);
				m_pStcPackState[i][nHaveState]->SetPic(szState_Hot);
				nHaveState++;
			}
			//促
			if( JudgePromotePro(pPack) )
			{
				m_pStcPackState[i][nHaveState]->SetInvisible(FALSE);
				m_pStcPackState[i][nHaveState]->SetPic(szState_Dis);
				nHaveState++;
			}
			////团
			//if( JudgeGroupBuyPro(pPack) )
			//{
			//	m_pStcPackState[i][EMIST_GroupBuy]->SetPic(szGroupHave);
			//	m_pStcPackState[i][EMIST_GroupBuy]->SetInvisible(FALSE);
			//}
			//else
			//{
			//	m_pStcPackState[i][EMIST_GroupBuy]->SetPic(szGroupNot);
			//	m_pStcPackState[i][EMIST_GroupBuy]->SetInvisible(TRUE);
			//}
			////赠
			//if( JudgePresentPro(pPack) )
			//{
			//	m_pStcPackState[i][EMIST_Present]->SetPic(szPresentHave);
			//	m_pStcPackState[i][EMIST_Present]->SetInvisible(FALSE);
			//}
			//else
			//{
			//	m_pStcPackState[i][EMIST_Present]->SetPic(szPresentNot);
			//	m_pStcPackState[i][EMIST_Present]->SetInvisible(TRUE);
			//}
			//限
			if( JudgeNumPro(pPack) )
			{
				m_pStcPackState[i][nHaveState]->SetInvisible(FALSE);
				m_pStcPackState[i][nHaveState]->SetPic(szState_Lim);
				nHaveState++;
			}
			////兑
			//if( JudgeExchangePro(pPack) )
			//{
			//	m_pStcPackState[i][EMIST_Exchange]->SetPic(szExchangeHove);
			//	m_pStcPackState[i][EMIST_Exchange]->SetInvisible(FALSE);
			//}
			//else 
			//{
			//	m_pStcPackState[i][EMIST_Exchange]->SetPic(szExchangeNot);
			//	m_pStcPackState[i][EMIST_Exchange]->SetInvisible(TRUE);
			//}
		}
		++it;
		++i;
	}
	SetCurrentPage();

}

void MallFrame::JudgeShowMallItem( INT nMenu, INT nSubMenu )
{
	if ( nMenu>=EMIT_END || nMenu<=EMIT_NULL )
	{
		nMenu = EMIT_All;
		nSubMenu = 0;
	}
	switch(nMenu)
	{
	case EMIT_Package:
		{
			m_pStcGoodsBackEx->SetInvisible(TRUE);
			m_pStcPackBack->SetInvisible(FALSE);
			m_nCurMenu = nMenu;
			m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE] = 0;
			if( MallMgr::Inst()->GetPackProtoSize()%MALL_MAXPACK_PERPAGE  == 0)
				m_nSpecialMaxPage[m_nCurMenu+SPECIAL_TYPE] = MallMgr::Inst()->GetPackProtoSize() / MALL_MAXPACK_PERPAGE ;
			else
				m_nSpecialMaxPage[m_nCurMenu+SPECIAL_TYPE] = MallMgr::Inst()->GetPackProtoSize() / MALL_MAXPACK_PERPAGE + 1;
			ShowCurPageMallPack(0);
		}
		break;
	case EMIT_All:
		{
			m_pStcGoodsBackEx->SetInvisible(FALSE);
			m_pStcPackBack->SetInvisible(TRUE);
			m_nCurMenu = nMenu;
			m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE] = 0;

			INT nTotal = 0;
			TMap<DWORD, tagMallItemProto*> mapAll = MallMgr::Inst()->GetMallProto();
			TMap<DWORD, tagMallItemProto*>::TMapIterator it = mapAll.Begin();

			if ( m_bLevelLimit == TRUE )	//开启等级限制
			{
				for( it; it != mapAll.End(); it++ )
				{ 
					if( P_VALID(it->second) 
						&& it->second->n8Kind-1 != EMIT_Volume 
						&& it->second->n8Kind-1 != EMIT_Mark 
						&& it->second->n8Kind   != EMIT_GiftPlan  )
					{
						tagMallItemProto* pTempProto = it->second;
						if( P_VALID(pTempProto) )
						{
							const tagItemProto* pTempItem = ItemProtoData::Inst()->FindItemProto(pTempProto->dwTypeID);
							if( P_VALID(pTempItem))
							{
								INT nTempRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
								INT nTempMaxLevel = (INT)pTempItem->byMaxUseLevel;
								INT nTempMinLevel = (INT)pTempItem->byMinUseLevel;
								if ( nTempRoleLevel >= nTempMinLevel && nTempRoleLevel <= nTempMaxLevel )
									++nTotal;		 
							}
						}
					}
				}
			}
			else
			{
				for( it; it != mapAll.End(); it++ )
				{ 
					if( P_VALID(it->second) 
						&& it->second->n8Kind-1 != EMIT_Volume 
						&& it->second->n8Kind-1 != EMIT_Mark
						&& it->second->n8Kind   != EMIT_GiftPlan)
						++nTotal;
				}
			}

			if( nTotal%MALL_MAXITEM_PERPAGE == 0)
				m_nSpecialMaxPage[m_nCurMenu+SPECIAL_TYPE] = nTotal / MALL_MAXITEM_PERPAGE;
			else 
				m_nSpecialMaxPage[m_nCurMenu+SPECIAL_TYPE] = nTotal / MALL_MAXITEM_PERPAGE + 1;
			ShowCurPageMallItemByAll(0);
		}
		break;
	case EMIT_Mark:
	case EMIT_Volume:
		{
			m_pStcGoodsBackEx->SetInvisible(FALSE);
			m_pStcPackBack->SetInvisible(TRUE);
			m_nCurMenu = nMenu;
			m_nCurSubMenu = nSubMenu;
			m_nCurPage[m_nCurMenu][m_nCurSubMenu] = 0;

			TMap<DWORD, tagMallItemProto*>::TMapIterator it = m_mapMallItem[m_nCurMenu][m_nCurSubMenu].Begin();
			INT nTotal = m_mapMallItem[m_nCurMenu][m_nCurSubMenu].Size();
			if (m_bLevelLimit == TRUE)		//不符合等级限制的不算在内
			{
				while(it!=m_mapMallItem[m_nCurMenu][m_nCurSubMenu].End())
				{
					tagMallItemProto* pTempProto = it->second;
					if( P_VALID(pTempProto) )
					{
						const tagItemProto* pTempItem = ItemProtoData::Inst()->FindItemProto(pTempProto->dwTypeID);
						if( P_VALID(pTempItem))
						{
							INT nTempRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
							INT nTempMaxLevel = (INT)pTempItem->byMaxUseLevel;
							INT nTempMinLevel = (INT)pTempItem->byMinUseLevel;
							if ( nTempRoleLevel < nTempMinLevel || nTempRoleLevel > nTempMaxLevel )
								nTotal--;		 
						}
					}
					++it;
				}
			}

			if( nTotal%MALL_MAXITEM_PERPAGE == 0 )
				m_nCurMaxPage[m_nCurMenu][m_nCurSubMenu] = nTotal / MALL_MAXITEM_PERPAGE;
			else
				m_nCurMaxPage[m_nCurMenu][m_nCurSubMenu] = nTotal / MALL_MAXITEM_PERPAGE + 1;
			ShowCurPageMallItemByType(0);		
		}
		break;
	default:
		{
			if(nMenu>=MALL_ITEM_SUBTYPE-2) //少的2种就是EMIT_Mark、EMIT_Volume
				return;
			m_pStcGoodsBackEx->SetInvisible(FALSE);
			m_pStcPackBack->SetInvisible(TRUE);
			m_nCurMenu = nMenu;
			m_nCurSubMenu = nSubMenu;
			m_nCurPage[m_nCurMenu][m_nCurSubMenu] = 0;

			TMap<DWORD, tagMallItemProto*>::TMapIterator it = m_mapMallItem[m_nCurMenu][m_nCurSubMenu].Begin();
			INT nTotal = m_mapMallItem[m_nCurMenu][m_nCurSubMenu].Size();
			if (m_bLevelLimit == TRUE)		//不符合等级限制的不算在内
			{
				while(it!=m_mapMallItem[m_nCurMenu][m_nCurSubMenu].End())
				{
					tagMallItemProto* pTempProto = it->second;
					if( P_VALID(pTempProto) )
					{
						const tagItemProto* pTempItem = ItemProtoData::Inst()->FindItemProto(pTempProto->dwTypeID);
						if( P_VALID(pTempItem))
						{
							INT nTempRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
							INT nTempMaxLevel = (INT)pTempItem->byMaxUseLevel;
							INT nTempMinLevel = (INT)pTempItem->byMinUseLevel;
							if ( nTempRoleLevel < nTempMinLevel || nTempRoleLevel > nTempMaxLevel )
								nTotal--;		 
						}
					}
					++it;
				}
			}

			if( nTotal%MALL_MAXITEM_PERPAGE == 0 )
				m_nCurMaxPage[m_nCurMenu][m_nCurSubMenu] = nTotal / MALL_MAXITEM_PERPAGE;
			else
				m_nCurMaxPage[m_nCurMenu][m_nCurSubMenu] = nTotal / MALL_MAXITEM_PERPAGE + 1;
			ShowCurPageMallItemByType(0);		
		}
		break;
	}
}

void MallFrame::PageUp()
{
	switch(m_nCurMenu)
	{
	case EMIT_Package:
		{
			--m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE];
			if( m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE] < 0 )
			{
				++m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE];
			}
			ShowCurPageMallPack(m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE]);
		}
		break;
	case EMIT_All:
		{
			--m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE];
			if( m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE] < 0 )
			{
				++m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE];
			}
			ShowCurPageMallItemByAll(m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE]);
		}
		break;
	default:
		{
			--m_nCurPage[m_nCurMenu][m_nCurSubMenu];
			if (m_nCurPage[m_nCurMenu][m_nCurSubMenu] < 0)
			{
				++m_nCurPage[m_nCurMenu][m_nCurSubMenu];
			}
			ShowCurPageMallItemByType(m_nCurPage[m_nCurMenu][m_nCurSubMenu]);
		}
		break;
	}
}

void MallFrame::PageDown()
{
	switch(m_nCurMenu)
	{
	case EMIT_Package:
		{
			++m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE];
			if( m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE] >= m_nSpecialMaxPage[m_nCurMenu+SPECIAL_TYPE] )
			{
				--m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE];
			}
			ShowCurPageMallPack(m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE]);
		}
		break;
	case EMIT_All:
		{
			++m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE];
			if( m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE] >= m_nSpecialMaxPage[m_nCurMenu+SPECIAL_TYPE] )
			{
				--m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE];
			}
			ShowCurPageMallItemByAll(m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE]);
		}
		break;
	default:
		{
			++m_nCurPage[m_nCurMenu][m_nCurSubMenu];
			if (m_nCurPage[m_nCurMenu][m_nCurSubMenu] >= m_nCurMaxPage[m_nCurMenu][m_nCurSubMenu])
			{
				--m_nCurPage[m_nCurMenu][m_nCurSubMenu];
			}
			ShowCurPageMallItemByType(m_nCurPage[m_nCurMenu][m_nCurSubMenu]);
		}
		break;
	}
}

void MallFrame::SetCurrentPage()
{
	switch(m_nCurMenu)
	{
	case EMIT_Package:
		{
			TCHAR szPage[X_SHORT_NAME] = {0};
			if ( m_nSpecialMaxPage[m_nCurMenu+SPECIAL_TYPE] == 0) //无商品时
			{
				 m_pBtnCPPack->SetInvisible(TRUE);
				 m_pBtnPDPack->SetInvisible(TRUE);
				 m_pBtnPUPack->SetInvisible(TRUE);
			}
			else
			{
				m_pBtnCPPack->SetInvisible(FALSE);
				m_pBtnPDPack->SetInvisible(FALSE);
				m_pBtnPUPack->SetInvisible(FALSE);
				_sntprintf(szPage, sizeof(szPage)/sizeof(TCHAR), _T("%d/%d"), m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE]+1, m_nSpecialMaxPage[m_nCurMenu+SPECIAL_TYPE]);
			}
			m_pBtnCPPack->SetText(szPage);
		}
		break;
	case EMIT_All:
		{
			TCHAR szPage[X_SHORT_NAME] = {0};
			
			if ( m_nSpecialMaxPage[m_nCurMenu+SPECIAL_TYPE] == 0) //无商品时
			{
				 m_pStcCPItem->SetInvisible(TRUE);
				 m_pBtnPDItem->SetInvisible(TRUE);
				 m_pBtnPUItem->SetInvisible(TRUE);
			}
			else
			{
				m_pStcCPItem->SetInvisible(FALSE);
				m_pBtnPDItem->SetInvisible(FALSE);
				m_pBtnPUItem->SetInvisible(FALSE);
				_sntprintf(szPage, sizeof(szPage)/sizeof(TCHAR), _T("%d/%d"), m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE]+1, m_nSpecialMaxPage[m_nCurMenu+SPECIAL_TYPE]);
			}
			m_pStcCPItem->SetText(szPage);
		}
		break;
	default:
		{
			TCHAR szPage[X_SHORT_NAME] = {0};
			
			if ( m_nCurMaxPage[m_nCurMenu][m_nCurSubMenu] == 0) //无商品时
			{
				m_pStcCPItem->SetInvisible(TRUE);
				m_pBtnPDItem->SetInvisible(TRUE);
				m_pBtnPUItem->SetInvisible(TRUE);
			}
			else
			{
				m_pStcCPItem->SetInvisible(FALSE);
				m_pBtnPDItem->SetInvisible(FALSE);
				m_pBtnPUItem->SetInvisible(FALSE);
				_sntprintf(szPage, sizeof(szPage)/sizeof(TCHAR), _T("%d/%d"), m_nCurPage[m_nCurMenu][m_nCurSubMenu]+1, m_nCurMaxPage[m_nCurMenu][m_nCurSubMenu]);
			}
			m_pStcCPItem->SetText(szPage);
		}
		break;
	}
}

void MallFrame::BuyMallItem()
{
	INT nMaxValue = GT_INVALID;
	switch(m_nCurMenu)
	{
	case EMIT_Package:
		{
			const tagMallPackProto* pItem = MallMgr::Inst()->GetMallPackByID(m_dwSelectItemID);
			if( !P_VALID(pItem) )
				return;
			nMaxValue = 1;
		}
		break;
	case EMIT_All:
	default:
		{
			const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
			if( !P_VALID(pItem) )
				return;
			const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pItem->dwTypeID);
			if( !P_VALID(pProto) )
				return;
			nMaxValue = pProto->n16MaxLapNum;
		}
		break;
	}
	if(nMaxValue==1)
	{
		tagMsgInputBoxEvent evt(_T("BuyMallItemNum_Event"), NULL);
		evt.eInputType = MBIT_Number;
		evt.eResult = MBF_OK;
		evt.nInputNum = nMaxValue;
		m_pFrameMgr->SendEvent(&evt);
	}
	else
	{
		if( P_VALID(GetObj("BuyMallItemNum_MsgBox")) )
			KillObj("BuyMallItemNum_MsgBox");
		CreateObj("BuyMallItemNum_MsgBox", "MsgInputBox");

		

		TObjRef<MsgInputBox>("BuyMallItemNum_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade4")], 
			_T("BuyMallItemNum_Event"), nMaxValue, (MsgBoxInputType)(MBIT_Number), _T("1"), TRUE);
	}
}

void MallFrame::BuyMallItemPromote()
{
	INT nMaxValue = GT_INVALID;
	const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
	if( !P_VALID(pItem) )
		return;
	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pItem->dwTypeID);
	if( !P_VALID(pProto) )
		return;
	nMaxValue = pProto->n16MaxLapNum;

	if(nMaxValue==1)
	{
		tagMsgInputBoxEvent evt(_T("BuyMallItemPromoteNum_Event"), NULL);
		evt.eInputType = MBIT_Number;
		evt.eResult = MBF_OK;
		evt.nInputNum = nMaxValue;
		m_pFrameMgr->SendEvent(&evt);
	}
	else
	{
		if( P_VALID(GetObj("BuyMallPromoteItemNum_MsgBox")) )
			KillObj("BuyMallPromoteItemNum_MsgBox");
		CreateObj("BuyMallPromoteItemNum_MsgBox", "MsgInputBox");



		TObjRef<MsgInputBox>("BuyMallPromoteItemNum_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade4")], 
			_T("BuyMallItemPromoteNum_Event"), nMaxValue, (MsgBoxInputType)(MBIT_Number), _T("1"), TRUE);
	}
}

void MallFrame::FreeGetMallItem()
{
	const tagMallFreeItem* pItem = MallMgr::Inst()->GetMallFreeItem();
	if( P_VALID(pItem) && m_dwSelectItemID==pItem->dwTypeID)
	{
		tagNC_MallFreeGetItem msg;
		msg.dwTypeID = m_dwSelectItemID;
		m_pSession->Send(&msg);
	}
	else 
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("MallTrade26")]);
}

DWORD MallFrame::OnBuyMallPromoteItemInputEvet(tagMsgInputBoxEvent* pGameEvent)
{
	if(MBF_OK == pGameEvent->eResult && MBIT_Number == pGameEvent->eInputType)
	{
		const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
		m_n16BuyNum = pGameEvent->nInputNum;
		if( m_n16BuyNum==0 )
		{
			if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
				KillObj("NotBuyMallItem_MsgBox");
			CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade61")], 
				_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
			return 0;
		}
		if( P_VALID(pItem) )
		{
			INT nYuanBao = 0;
			DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pItem->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
			if( dwTime==0 || pItem->dwTimeSaleEnd==GT_INVALID)
				nYuanBao = pItem->nPrice;
			else 
				nYuanBao = pItem->nSalePrice;
			if( m_n16BuyNum > pItem->nNum && pItem->nNum != GT_INVALID )
			{
				if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
					KillObj("NotBuyMallItem_MsgBox");
				CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

				TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade21")], 
					_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
				return 0;
			}

			if( nYuanBao*m_n16BuyNum <= CurrencyMgr::Inst()->GetBagYuanbao() || nYuanBao*m_n16BuyNum <= ItemMgr::Inst()->GetPocket()->GetItemQuantity(YuanBaoDaiBi) )
			{
				if( P_VALID(GetObj("AffirmBuyMallItem_MsgBox")) )
					KillObj("AffirmBuyMallItem_MsgBox");
				CreateObj("AffirmBuyMallItem_MsgBox", "MsgBox");
				tstringstream PromoteItemInfo;
				TCHAR szBuff[X_LONG_NAME] = {0};
				_stprintf( szBuff, g_StrTable[_T("MallTrade5")], nYuanBao*m_n16BuyNum );
				PromoteItemInfo <<szBuff << _T("\\n");
				const tagItemDisplayInfo* pDisplayInfo = ItemProtoData::Inst()->FindItemDisplay(pItem->dwTypeID);
				if( P_VALID(pDisplayInfo))
				{
					TCHAR szNum[X_LONG_NAME] = {0};
					_stprintf( szNum, g_StrTable[_T("MallTrade66")], m_n16BuyNum );
					PromoteItemInfo << pDisplayInfo->szName << szNum << _T("\\n");
				}

				const tagItemDisplayInfo* pDisplayIn = ItemProtoData::Inst()->FindItemDisplay(pItem->dwPresentID);
				if( P_VALID(pDisplayIn) )
				{
					TCHAR szPresent[X_LONG_NAME] = {0};
					_stprintf( szPresent, g_StrTable[_T("MallTrade63")], pItem->nExAssign * m_n16BuyNum );
					PromoteItemInfo << szPresent << _T("\\n");
					TCHAR szNum[X_LONG_NAME] = {0};
					_stprintf( szNum, g_StrTable[_T("MallTrade66")], m_n16BuyNum * pItem->nPresentNum );
					PromoteItemInfo << pDisplayIn->szName << szNum << _T("\\n");
				}
				else if( pItem->nExAssign > 0 )
				{
					TCHAR szPresent[X_LONG_NAME] = {0};
					_stprintf( szPresent, g_StrTable[_T("MallTrade64")], pItem->nExAssign * m_n16BuyNum );
					PromoteItemInfo << szPresent << _T("\\n");
				}
				TObjRef<MsgBox>("AffirmBuyMallItem_MsgBox")->Init(_T(""), PromoteItemInfo.str().c_str(), 
					_T("AffirmBuyMallItem_Event"), MsgBoxFlag(MBF_Bigger), TRUE);
			}
			else 
			{
				if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
					KillObj("NotBuyMallItem_MsgBox");
				CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

				TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade6")], 
					_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
			}
		}
	}
	return 0;
}

DWORD MallFrame::OnBuyMallItemInputEvent( tagMsgInputBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult && MBIT_Number == pGameEvent->eInputType)
	{
		switch(m_nCurMenu)
		{
		case EMIT_Package:
			{
				const tagMallPackProto* pPack = MallMgr::Inst()->GetMallPackByID(m_dwSelectItemID);
				m_n16BuyNum = pGameEvent->nInputNum;
				if( m_n16BuyNum==0 )
				{
					if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
						KillObj("NotBuyMallItem_MsgBox");
					CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

					TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade61")], 
						_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_Cancel), TRUE);
					return 0;
				}
				if( P_VALID(pPack) )
				{
					INT nYuanBao = 0;
					DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pPack->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
					if( dwTime==0 || pPack->dwTimeSaleEnd==GT_INVALID)
						nYuanBao = pPack->nPrice;
					else 
						nYuanBao = pPack->nSalePrice;
					if( m_n16BuyNum > pPack->nNum && pPack->nNum != GT_INVALID )
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade21")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_Cancel), TRUE);
						return 0;
					}

					if( nYuanBao*m_n16BuyNum <= CurrencyMgr::Inst()->GetBagYuanbao()  || nYuanBao*m_n16BuyNum <= ItemMgr::Inst()->GetPocket()->GetItemQuantity(YuanBaoDaiBi) )
					{
						if( P_VALID(GetObj("AffirmBuyMallItem_MsgBox")) )
							KillObj("AffirmBuyMallItem_MsgBox");
						CreateObj("AffirmBuyMallItem_MsgBox", "MsgBox");
						tstringstream packInfo;
						TCHAR szBuff[X_LONG_NAME] = {0};
						_stprintf( szBuff, g_StrTable[_T("MallTrade5")], nYuanBao*m_n16BuyNum );
						packInfo << szBuff << _T("\\n");
						for( int i = 0; i <= pPack->n8ItemKind; i++ )
						{
							const tagItemDisplayInfo* pDisplayInfo = ItemProtoData::Inst()->FindItemDisplay( pPack->dwTypeID[i] );
							if( P_VALID(pDisplayInfo) )
							{
								TCHAR szNum[X_LONG_NAME] = {0};
								_stprintf( szNum, g_StrTable[_T("MallTrade66")], pPack->byItemNum[i] );
								packInfo << pDisplayInfo->szName << szNum << _T("\\n");
							}
						}
						const tagItemDisplayInfo* pDisplayInfo = ItemProtoData::Inst()->FindItemDisplay(pPack->dwPresentID);
						if( P_VALID(pDisplayInfo) )
						{
							TCHAR szPresent[X_LONG_NAME] = {0};
							_stprintf( szPresent, g_StrTable[_T("MallTrade63")], pPack->nExAssign );
							packInfo << szPresent << _T("\\n");
							TCHAR szNum[X_LONG_NAME] = {0};
							_stprintf( szNum, g_StrTable[_T("MallTrade66")], pPack->nPresentNum );
							packInfo << pDisplayInfo->szName << szNum << _T("\\n");
						}
						else if( pPack->nExAssign > 0 )
						{
							TCHAR szPresent[X_LONG_NAME] = {0};
							_stprintf( szPresent, g_StrTable[_T("MallTrade64")], pPack->nExAssign );
							packInfo << szPresent << _T("\\n");
						}
						//packInfo << g_StrTable[_T("MallTrade65")] << _T("\\n"); 

						TObjRef<MsgBox>("AffirmBuyMallItem_MsgBox")->Init(_T(""), packInfo.str().c_str(), 
							_T("AffirmBuyMallItem_Event"), MsgBoxFlag(MBF_Bigger), TRUE);
					}
					else 
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade6")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
					}
				}
			}
			break;
		case EMIT_All:
			{
				const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
				m_n16BuyNum = pGameEvent->nInputNum;
				if( m_n16BuyNum==0 )
				{
					if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
						KillObj("NotBuyMallItem_MsgBox");
					CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

					TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade61")], 
						_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_Cancel), TRUE);
					return 0;
				}
				if( P_VALID(pItem) )
				{
					INT nYuanBao = 0;
					DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pItem->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
					if( dwTime==0 || pItem->dwTimeSaleEnd==GT_INVALID)
						nYuanBao = pItem->nPrice;
					else 
						nYuanBao = pItem->nSalePrice;
					if( m_n16BuyNum > pItem->nNum && pItem->nNum != GT_INVALID )
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade21")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						return 0;
					}

					if( nYuanBao*m_n16BuyNum <= CurrencyMgr::Inst()->GetBagYuanbao() || nYuanBao*m_n16BuyNum <= ItemMgr::Inst()->GetPocket()->GetItemQuantity(YuanBaoDaiBi))
					{
						if( P_VALID(GetObj("AffirmBuyMallItem_MsgBox")) )
							KillObj("AffirmBuyMallItem_MsgBox");
						CreateObj("AffirmBuyMallItem_MsgBox", "MsgBox");
						tstringstream packInfo;
						TCHAR szBuff[X_LONG_NAME] = {0};
						_stprintf( szBuff, g_StrTable[_T("MallTrade5")], nYuanBao*m_n16BuyNum );
						packInfo <<szBuff << _T("\\n");
						const tagItemDisplayInfo* pDisplayInfo = ItemProtoData::Inst()->FindItemDisplay(pItem->dwTypeID);
						if( P_VALID(pDisplayInfo))
						{
							TCHAR szNum[X_LONG_NAME] = {0};
							_stprintf( szNum, g_StrTable[_T("MallTrade66")], m_n16BuyNum );
							packInfo << pDisplayInfo->szName << szNum << _T("\\n");
						}
					
						const tagItemDisplayInfo* pDisplayIn = ItemProtoData::Inst()->FindItemDisplay(pItem->dwPresentID);
						if( P_VALID(pDisplayIn) )
						{
							TCHAR szPresent[X_LONG_NAME] = {0};
							_stprintf( szPresent, g_StrTable[_T("MallTrade63")], pItem->nExAssign * m_n16BuyNum );
							packInfo << szPresent << _T("\\n");
							TCHAR szNum[X_LONG_NAME] = {0};
							_stprintf( szNum, g_StrTable[_T("MallTrade66")], m_n16BuyNum * pItem->nPresentNum );
							packInfo << pDisplayIn->szName << szNum << _T("\\n");
						}
						else if( pItem->nExAssign > 0 )
						{
							TCHAR szPresent[X_LONG_NAME] = {0};
							_stprintf( szPresent, g_StrTable[_T("MallTrade64")], pItem->nExAssign * m_n16BuyNum );
							packInfo << szPresent << _T("\\n");
						}
						//packInfo << g_StrTable[_T("MallTrade65")] << _T("\\n"); 
						TObjRef<MsgBox>("AffirmBuyMallItem_MsgBox")->Init(_T(""), packInfo.str().c_str(), 
							_T("AffirmBuyMallItem_Event"), MsgBoxFlag(MBF_Bigger), TRUE);
					}
					else 
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade6")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
					}
				}
			}
			break;
		case EMIT_Volume:
			{
				const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
				m_n16BuyNum = pGameEvent->nInputNum;
				if( m_n16BuyNum==0 )
				{
					if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
						KillObj("NotBuyMallItem_MsgBox");
					CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

					TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade61")], 
						_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_Cancel), TRUE);
					return 0;
				}
				if( P_VALID(pItem) )
				{
					INT nVolume = 0;
					DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pItem->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
					if( dwTime==0 || pItem->dwTimeSaleEnd==GT_INVALID)
						nVolume = pItem->nPrice;
					else 
						nVolume = pItem->nSalePrice;
					if( m_n16BuyNum > pItem->nNum && pItem->nNum != GT_INVALID )
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade21")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						return 0;
					}

					if( nVolume*m_n16BuyNum <= CurrencyMgr::Inst()->GetExchangeVolume())
					{
						if( P_VALID(GetObj("AffirmBuyMallItem_MsgBox")) )
							KillObj("AffirmBuyMallItem_MsgBox");
						CreateObj("AffirmBuyMallItem_MsgBox", "MsgBox");
						tstringstream packInfo;
						TCHAR szBuff[X_LONG_NAME] = {0};
						_stprintf( szBuff, g_StrTable[_T("MallTrade72")], nVolume*m_n16BuyNum );
						packInfo <<szBuff << _T("\\n");
						const tagItemDisplayInfo* pDisplayInfo = ItemProtoData::Inst()->FindItemDisplay(pItem->dwTypeID);
						if( P_VALID(pDisplayInfo))
						{
							TCHAR szNum[X_LONG_NAME] = {0};
							_stprintf( szNum, g_StrTable[_T("MallTrade66")], m_n16BuyNum );
							packInfo << pDisplayInfo->szName << szNum << _T("\\n");
						}

						const tagItemDisplayInfo* pDisplayIn = ItemProtoData::Inst()->FindItemDisplay(pItem->dwPresentID);
						if( P_VALID(pDisplayIn) )
						{
							TCHAR szPresent[X_LONG_NAME] = {0};
							_stprintf( szPresent, g_StrTable[_T("MallTrade63")], pItem->nExAssign * m_n16BuyNum );
							packInfo << szPresent << _T("\\n");
							TCHAR szNum[X_LONG_NAME] = {0};
							_stprintf( szNum, g_StrTable[_T("MallTrade66")], m_n16BuyNum * pItem->nPresentNum );
							packInfo << pDisplayIn->szName << szNum << _T("\\n");
						}
						else if( pItem->nExAssign > 0 )
						{
							TCHAR szPresent[X_LONG_NAME] = {0};
							_stprintf( szPresent, g_StrTable[_T("MallTrade64")], pItem->nExAssign * m_n16BuyNum );
							packInfo << szPresent << _T("\\n");
						}
						//packInfo << g_StrTable[_T("MallTrade65")] << _T("\\n"); 
						TObjRef<MsgBox>("AffirmBuyMallItem_MsgBox")->Init(_T(""), packInfo.str().c_str(), 
							_T("AffirmBuyMallItem_Event"), MsgBoxFlag(MBF_Bigger), TRUE);
					}
					else 
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade74")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
					}
				}
			}
			break;
		case EMIT_Mark:
			{
				const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
				m_n16BuyNum = pGameEvent->nInputNum;
				if( m_n16BuyNum==0 )
				{
					if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
						KillObj("NotBuyMallItem_MsgBox");
					CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

					TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade61")], 
						_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_Cancel), TRUE);
					return 0;
				}
				if( P_VALID(pItem) )
				{
					INT nMark = 0;
					DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pItem->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
					if( dwTime==0 || pItem->dwTimeSaleEnd==GT_INVALID)
						nMark = pItem->nPrice;
					else 
						nMark = pItem->nSalePrice;
					if( m_n16BuyNum > pItem->nNum && pItem->nNum != GT_INVALID )
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade21")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						return 0;
					}

					if( nMark*m_n16BuyNum <= CurrencyMgr::Inst()->GetMark() )
					{
						if( P_VALID(GetObj("AffirmBuyMallItem_MsgBox")) )
							KillObj("AffirmBuyMallItem_MsgBox");
						CreateObj("AffirmBuyMallItem_MsgBox", "MsgBox");
						tstringstream packInfo;
						TCHAR szBuff[X_LONG_NAME] = {0};
						_stprintf( szBuff, g_StrTable[_T("MallTrade73")], nMark*m_n16BuyNum );
						packInfo <<szBuff << _T("\\n");
						const tagItemDisplayInfo* pDisplayInfo = ItemProtoData::Inst()->FindItemDisplay(pItem->dwTypeID);
						if( P_VALID(pDisplayInfo))
						{
							TCHAR szNum[X_LONG_NAME] = {0};
							_stprintf( szNum, g_StrTable[_T("MallTrade66")], m_n16BuyNum );
							packInfo << pDisplayInfo->szName << szNum << _T("\\n");
						}

						const tagItemDisplayInfo* pDisplayIn = ItemProtoData::Inst()->FindItemDisplay(pItem->dwPresentID);
						if( P_VALID(pDisplayIn) )
						{
							TCHAR szPresent[X_LONG_NAME] = {0};
							_stprintf( szPresent, g_StrTable[_T("MallTrade63")], pItem->nExAssign * m_n16BuyNum );
							packInfo << szPresent << _T("\\n");
							TCHAR szNum[X_LONG_NAME] = {0};
							_stprintf( szNum, g_StrTable[_T("MallTrade66")], m_n16BuyNum * pItem->nPresentNum );
							packInfo << pDisplayIn->szName << szNum << _T("\\n");
						}
						else if( pItem->nExAssign > 0 )
						{
							TCHAR szPresent[X_LONG_NAME] = {0};
							_stprintf( szPresent, g_StrTable[_T("MallTrade64")], pItem->nExAssign * m_n16BuyNum );
							packInfo << szPresent << _T("\\n");
						}
						//packInfo << g_StrTable[_T("MallTrade65")] << _T("\\n"); 
						TObjRef<MsgBox>("AffirmBuyMallItem_MsgBox")->Init(_T(""), packInfo.str().c_str(), 
							_T("AffirmBuyMallItem_Event"), MsgBoxFlag(MBF_Bigger), TRUE);
					}
					else 
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade75")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
					}
				}
			}
			break;
		default:
			{
				const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
				m_n16BuyNum = pGameEvent->nInputNum;
				if( m_n16BuyNum==0 )
				{
					if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
						KillObj("NotBuyMallItem_MsgBox");
					CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

					TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade61")], 
						_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
					return 0;
				}
				if( P_VALID(pItem) )
				{
					INT nYuanBao = 0;
					DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pItem->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
					if( dwTime==0 || pItem->dwTimeSaleEnd==GT_INVALID)
						nYuanBao = pItem->nPrice;
					else 
						nYuanBao = pItem->nSalePrice;
					if( m_n16BuyNum > pItem->nNum && pItem->nNum != GT_INVALID )
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade21")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						return 0;
					}
					
					if( nYuanBao*m_n16BuyNum <= CurrencyMgr::Inst()->GetBagYuanbao() || nYuanBao*m_n16BuyNum <= ItemMgr::Inst()->GetPocket()->GetItemQuantity(YuanBaoDaiBi) )
					{
						if( P_VALID(GetObj("AffirmBuyMallItem_MsgBox")) )
							KillObj("AffirmBuyMallItem_MsgBox");
						CreateObj("AffirmBuyMallItem_MsgBox", "MsgBox");
						tstringstream packInfo;
						TCHAR szBuff[X_LONG_NAME] = {0};
						_stprintf( szBuff, g_StrTable[_T("MallTrade5")], nYuanBao*m_n16BuyNum );
						packInfo <<szBuff << _T("\\n");
						const tagItemDisplayInfo* pDisplayInfo = ItemProtoData::Inst()->FindItemDisplay(pItem->dwTypeID);
						if( P_VALID(pDisplayInfo))
						{
							TCHAR szNum[X_LONG_NAME] = {0};
							_stprintf( szNum, g_StrTable[_T("MallTrade66")], m_n16BuyNum );
							packInfo << pDisplayInfo->szName << szNum << _T("\\n");
						}

						const tagItemDisplayInfo* pDisplayIn = ItemProtoData::Inst()->FindItemDisplay(pItem->dwPresentID);
						if( P_VALID(pDisplayIn) )
						{
							TCHAR szPresent[X_LONG_NAME] = {0};
							_stprintf( szPresent, g_StrTable[_T("MallTrade63")], pItem->nExAssign * m_n16BuyNum );
							packInfo << szPresent << _T("\\n");
							TCHAR szNum[X_LONG_NAME] = {0};
							_stprintf( szNum, g_StrTable[_T("MallTrade66")], m_n16BuyNum * pItem->nPresentNum );
							packInfo << pDisplayIn->szName << szNum << _T("\\n");
						}
						else if( pItem->nExAssign > 0 )
						{
							TCHAR szPresent[X_LONG_NAME] = {0};
							_stprintf( szPresent, g_StrTable[_T("MallTrade64")], pItem->nExAssign * m_n16BuyNum );
							packInfo << szPresent << _T("\\n");
						}
						//packInfo << g_StrTable[_T("MallTrade65")] << _T("\\n"); 
						TObjRef<MsgBox>("AffirmBuyMallItem_MsgBox")->Init(_T(""), packInfo.str().c_str(), 
							_T("AffirmBuyMallItem_Event"), MsgBoxFlag(MBF_Bigger), TRUE);
					}
					else
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade6")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
					}
				}
			}
			break;
		}
	}
	return 0;
}

DWORD MallFrame::OnAffirmBuyMallItemEvent( tagMsgBoxEvent* pGameEvent )
{
	if(MBF_OK == pGameEvent->eResult)
	{
		if( !m_bSelectPromote )
		{
			switch(m_nCurMenu)
			{
			case EMIT_Package:
				{
					const tagMallPackProto* pPack = MallMgr::Inst()->GetMallPackByID(m_dwSelectItemID);
					OnSendBuyMallPack(pPack, m_n16BuyNum);
				}
				break;
			case EMIT_All:
			default:
				{
					const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
					OnSendBuyMallItem(pItem, m_n16BuyNum);
				}
				break;
			}
		}
		else
		{
			const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
			OnSendBuyMallItem(pItem, m_n16BuyNum);
		}
	}
	return 0;
}

DWORD MallFrame::OnMsgMallGiftEvent(tagMsgMallGiftEvent* pGameEvent)
{
	if(MGF_OK == pGameEvent->eRet)
	{
		//没有输入名字
		if(pGameEvent->strName.empty())
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("MallTrade7")]);
			return 0;
		}

		//查ID
		DWORD dwRoleID = PlayerNameTab::Inst()->FindIDByName(pGameEvent->strName.c_str());
		if( dwRoleID==GT_INVALID )
		{
			bool bHaveSave = false;
			vector<tagMsgMallGiftEvent>::iterator it = m_vecGiftEvent.begin();
			for( ;it!=m_vecGiftEvent.end(); ++it )
			{
				if(it->strName == pGameEvent->strName)
				{
					bHaveSave = true;
					break;
				}
			}
			if( !bHaveSave )
			{
				m_vecGiftEvent.push_back(*pGameEvent);
			}
			else
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("MallTrade28")]);
			}
			return 0;
		}

		//推荐商品特殊处理
		if( m_bSelectPromote )
		{
			const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
			if( P_VALID(pItem) )
			{
				INT nYuanBao = 0;
				DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pItem->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
				if( dwTime==0 || pItem->dwTimeSaleEnd==GT_INVALID)
					nYuanBao = pItem->nPrice;
				else 
					nYuanBao = pItem->nSalePrice;

				if( pGameEvent->n16Num<=0 )
				{
					if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
						KillObj("NotBuyMallItem_MsgBox");
					CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

					TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade61")], 
						_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
					return 0;
				}

				if( pGameEvent->n16Num > pItem->nNum && pItem->nNum != GT_INVALID)
				{
					if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
						KillObj("NotBuyMallItem_MsgBox");
					CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

					TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade29")], 
						_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
					return 0;
				}

				

				if( nYuanBao*pGameEvent->n16Num <= CurrencyMgr::Inst()->GetBagYuanbao() )
				{
					tagNC_MallPresentItem msg;
					msg.eShopingDistrict = m_eShopingDistrict;
					msg.dwTypeID = pItem->dwID;
					msg.dwRoleTgtID = dwRoleID;
					msg.nPrice = nYuanBao;
					msg.n16BuyNum = pGameEvent->n16Num;
					_tcsncpy( msg.szLeave, pGameEvent->strWord.c_str(), sizeof(msg.szLeave)/sizeof(TCHAR) );
					msg.nIndexInServer = pItem->nIndexInServer;
					m_pSession->Send(&msg);
				}
				else 
				{
					if( P_VALID(GetObj("NotGiftMallItem_MsgBox")) )
						KillObj("NotGiftMallItem_MsgBox");
					CreateObj("NotGiftMallItem_MsgBox", "MsgBoxEx");

					if (m_eShopingDistrict == Shoppint_Coupons)
					{
						TObjRef<MsgBoxEx>("NotGiftMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Volume_Not_Enough")], 
							_T("NotGiftMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
					} 
					else if (m_eShopingDistrict == Shopping_VipPoint)
					{
						TObjRef<MsgBoxEx>("NotGiftMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Mark_Not_Enough")], 
							_T("NotGiftMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
					}
					else //m_eShopingDistrict == Shopping_Item
					{
						TObjRef<MsgBoxEx>("NotGiftMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_YuanBao_Not_Enough")], 
							_T("NotGiftMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
					}
				}
			}
			return 0;
		}

		switch(m_nCurMenu)
		{
		case EMIT_Package:
			{
				const tagMallPackProto* pPack = MallMgr::Inst()->GetMallPackByID(m_dwSelectItemID);
				if( P_VALID(pPack) )  
				{
					INT nYuanBao = 0;
					DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pPack->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
					if( dwTime==0 || pPack->dwTimeSaleEnd==GT_INVALID)
						nYuanBao = pPack->nPrice;
					else 
						nYuanBao = pPack->nSalePrice;
					if( pGameEvent->n16Num<=0 )
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade61")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						return 0;
					}
					if( pGameEvent->n16Num > pPack->nNum )
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade29")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						return 0;
					}
					if( nYuanBao*pGameEvent->n16Num <= CurrencyMgr::Inst()->GetBagYuanbao() )
					{
						tagNC_MallPresentPack msg;
						msg.eShopingDistrict = m_eShopingDistrict;
						msg.dwTypeID = pPack->dwID;
						msg.dwRoleTgtID = dwRoleID;
						msg.nPrice = nYuanBao;
						msg.n16BuyNum = pGameEvent->n16Num;
						_tcsncpy( msg.szLeave, pGameEvent->strWord.c_str(), sizeof(msg.szLeave)/sizeof(TCHAR) );
						msg.nIndexInServer = pPack->nIndexInServer;
						m_pSession->Send(&msg);
					}
					else 
					{
						if( P_VALID(GetObj("NotGiftMallItem_MsgBox")) )
							KillObj("NotGiftMallItem_MsgBox");
						CreateObj("NotGiftMallItem_MsgBox", "MsgBoxEx");

						if (m_eShopingDistrict == Shoppint_Coupons)
						{
							TObjRef<MsgBoxEx>("NotGiftMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Volume_Not_Enough")], 
								_T("NotGiftMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						} 
						else if (m_eShopingDistrict == Shopping_VipPoint)
						{
							TObjRef<MsgBoxEx>("NotGiftMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Mark_Not_Enough")], 
								_T("NotGiftMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						}
						else //m_eShopingDistrict == Shopping_Item
						{
							TObjRef<MsgBoxEx>("NotGiftMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_YuanBao_Not_Enough")], 
								_T("NotGiftMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						}
					}
				}
			}
			break;
		case EMIT_All:
		default:
			{
				const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
				if( P_VALID(pItem) )
				{
					INT nYuanBao = 0;
					DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pItem->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
					if( dwTime==0 || pItem->dwTimeSaleEnd==GT_INVALID)
						nYuanBao = pItem->nPrice;
					else 
						nYuanBao = pItem->nSalePrice;
					if( pGameEvent->n16Num<=0 )
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade61")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						return 0;
					}
					if( pGameEvent->n16Num > pItem->nNum && pItem->nNum != GT_INVALID)
					{
						if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )
							KillObj("NotBuyMallItem_MsgBox");
						CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");

						TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade29")], 
							_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						return 0;
					}
					if( nYuanBao*pGameEvent->n16Num <= CurrencyMgr::Inst()->GetBagYuanbao() )
					{
						tagNC_MallPresentItem msg;
						msg.eShopingDistrict = m_eShopingDistrict;
						msg.dwTypeID = pItem->dwID;
						msg.dwRoleTgtID = dwRoleID;
						msg.nPrice = nYuanBao;
						msg.n16BuyNum = pGameEvent->n16Num;
						_tcsncpy( msg.szLeave, pGameEvent->strWord.c_str(), sizeof(msg.szLeave)/sizeof(TCHAR) );
						msg.nIndexInServer = pItem->nIndexInServer;
						m_pSession->Send(&msg);
					}
					else 
					{
						if( P_VALID(GetObj("NotGiftMallItem_MsgBox")) )
							KillObj("NotGiftMallItem_MsgBox");
						CreateObj("NotGiftMallItem_MsgBox", "MsgBoxEx");

						if (m_eShopingDistrict == Shoppint_Coupons)
						{
							TObjRef<MsgBoxEx>("NotGiftMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Volume_Not_Enough")], 
								_T("NotGiftMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						} 
						else if (m_eShopingDistrict == Shopping_VipPoint)
						{
							TObjRef<MsgBoxEx>("NotGiftMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Mark_Not_Enough")], 
								_T("NotGiftMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						}
						else //m_eShopingDistrict == Shopping_Item
						{
							TObjRef<MsgBoxEx>("NotGiftMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_YuanBao_Not_Enough")], 
								_T("NotGiftMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);
						}
					}
				}
			}
			break;
		}
	}
	return 0;
}

VOID MallFrame::OnSendBuyMallItem( const tagMallItemProto* pItem, INT16 n16Num )
{
	if( P_VALID( pItem ) )
	{
		tagNC_MallBuyItem msg;
		msg.eShopingDistrict = m_eShopingDistrict;
		msg.nIndexInServer = pItem->nIndexInServer;
		msg.dwTypeID = pItem->dwID;
		msg.n16BuyNum = n16Num;
		DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pItem->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
		if( dwTime==0 || pItem->dwTimeSaleEnd==GT_INVALID)
			msg.nPrice = pItem->nPrice;
		else 
			msg.nPrice = pItem->nSalePrice;
		m_pSession->Send(&msg);
	}
}

VOID MallFrame::OnSendBuyMallPack(const tagMallPackProto* pPack, INT16 n16Num)
{
	tagNC_MallBuyPack msg;
	msg.eShopingDistrict = m_eShopingDistrict;
	msg.nIndexInServer = pPack->nIndexInServer;
	msg.dwTypeID = pPack->dwID;
	msg.n16BuyNum = n16Num;
	DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(pPack->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
	if( dwTime==0 || pPack->dwTimeSaleEnd==GT_INVALID)
		msg.nPrice = pPack->nPrice;
	else 
		msg.nPrice = pPack->nSalePrice;
	m_pSession->Send(&msg);
}

void MallFrame::RefreshFreeItem()
{
	tagMallFreeItem* pProItem = MallMgr::Inst()->GetMallFreeItem();
	if( P_VALID(pProItem) )
	{
		const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(pProItem->dwTypeID);
		const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(pProItem->dwTypeID);
		if( P_VALID(pItem) && P_VALID(pItemProto) )
		{
			m_pStcFetchBack->SetInvisible(FALSE);
			m_pFetchUnitItem->RefreshItem(pProItem->dwTypeID, pProItem->byNum, pItem->byQuality);
			m_pFetchUnitItem->SetBtnData(ESBT_Item, pProItem->dwTypeID, pProItem->nUnitPrice, pProItem->byNum);
			const tagItemDisplayInfo* pInfo = ItemProtoData::Inst()->FindItemDisplay(pProItem->dwTypeID);
			if( P_VALID( pInfo ) )
			{
				m_pBtnFetchTitle->SetText(pInfo->szName);
			}
		}
		else 
		{
			m_pStcFetchBack->SetInvisible(TRUE);
		}
	}
}

void MallFrame::RefreshPromoteItemState()
{
	for(int i=0; i<MALL_MAX_PROMOTE_ITEM; i++)
	{
		for(int j=0; j<MALL_PROMOTE_ITEM_STATE; j++)
		{
			m_pProItemState[i][j]->SetInvisible(TRUE);
		}
	}
	m_dwSelectItemID = GT_INVALID;
}

DWORD MallFrame::OnNS_MallBuyItem(tagNS_MallBuyItem* pMsg, DWORD dwParam)
{
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			//if( P_VALID(GetObj("LauncherSucess_MsgBox")) )
			//	KillObj("LauncherSucess_MsgBox");
			//CreateObj("LauncherSucess_MsgBox", "MsgBoxEx");

			//TObjRef<MsgBoxEx>("LauncherSucess_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade9")], 
			//	_T("LauncherSucess_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_BagYuanBao_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughYuanbao")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_ExVolume_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughVolume")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_VipPoint_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughMark")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Bag_NotEnoughSpace:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("PickGroundItemErr1")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Item_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade21")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_ID_Error:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade23")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Con_PswNotPass:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade27")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_YuanBao_Error:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade24")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_CreateItem_Failed:// 商品创建失败
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade59")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_CreatePres_Failed:	// 赠品创建失败
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade60")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Trade_ItemNum_MaxHoldLimit:	// 商品最大持有数限制
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade67")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_AlreadyBuy:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Only_Once")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_Level_Limit:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("Gift_Plan_Level_Not_Enough")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_PersonalBuyLimit:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("Mall_PersonalBuyLimit")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	default:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_ServerError")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	}
	m_pMgr->SendEvent( &tagGameEvent( _T("Refresh_Deadmenu"), this ) );
	return 0;
}

DWORD MallFrame::OnNS_MallFreeGetItem( tagNS_MallFreeGetItem* pMsg, DWORD dwParam )
{
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			if( P_VALID(GetObj("LauncherSucess_MsgBox")) )
				KillObj("LauncherSucess_MsgBox");
			CreateObj("LauncherSucess_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherSucess_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade10")], 
				_T("LauncherSucess_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_BagYuanBao_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughYuanbao")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_ExVolume_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughVolume")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_VipPoint_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughMark")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Bag_NotEnoughSpace:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("PickGroundItemErr1")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Free_AlreadyGet:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade20")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_ID_Error:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade23")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Free_NotExist:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade25")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Con_PswNotPass:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade27")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_CreateItem_Failed:// 商品创建失败
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade59")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_CreatePres_Failed:	// 赠品创建失败
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade60")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_AlreadyBuy:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Only_Once")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_Level_Limit:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("Gift_Plan_Level_Not_Enough")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	default:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_ServerError")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	}
	m_pStcFetchBack->SetPic(szUnpickItem);
	return 0;
}

DWORD MallFrame::OnNS_MallPresentItem( tagNS_MallPresentItem* pMsg, DWORD dwParam )
{
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			if( P_VALID(GetObj("LauncherSucess_MsgBox")) )
				KillObj("LauncherSucess_MsgBox");
			CreateObj("LauncherSucess_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherSucess_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade11")], 
				_T("LauncherSucess_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_BagYuanBao_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughYuanbao")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_ExVolume_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughVolume")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_VipPoint_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughMark")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Bag_NotEnoughSpace:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("PickGroundItemErr1")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Item_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade21")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_ID_Error:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade23")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_YuanBao_Error:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade24")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Con_PswNotPass:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade27")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Role_Not_ExistIn_World:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade28")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_CreateItem_Failed:// 商品创建失败
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade59")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_CreatePres_Failed:	// 赠品创建失败
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade60")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Trade_ItemNum_MaxHoldLimit:	// 商品最大持有数限制
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade67")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_AlreadyBuy:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Only_Once")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_Level_Limit:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("Gift_Plan_Level_Not_Enough")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_PersonalBuyLimit:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("Mall_PersonalBuyLimit")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	default:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_ServerError")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	}
	return 0;
}

DWORD MallFrame::OnNS_MallBuyPack( tagNS_MallBuyPack* pMsg, DWORD dwParam )
{
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			//if( P_VALID(GetObj("LauncherSucess_MsgBox")) )
			//	KillObj("LauncherSucess_MsgBox");
			//CreateObj("LauncherSucess_MsgBox", "MsgBoxEx");

			//TObjRef<MsgBoxEx>("LauncherSucess_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade9")], 
			//	_T("LauncherSucess_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_BagYuanBao_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughYuanbao")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_ExVolume_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughVolume")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_VipPoint_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughMark")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Bag_NotEnoughSpace:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("PickGroundItemErr1")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Pack_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade22")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_ID_Error:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade23")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_YuanBao_Error:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade24")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Con_PswNotPass:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade27")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_CreateItem_Failed:// 商品创建失败
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade59")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_CreatePres_Failed:	// 赠品创建失败
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade60")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Trade_ItemNum_MaxHoldLimit:	// 商品最大持有数限制
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade67")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_AlreadyBuy:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Only_Once")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_Level_Limit:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("Gift_Plan_Level_Not_Enough")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	default:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_ServerError")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	}
	return 0;
}

DWORD MallFrame::OnNS_MallPresentPack( tagNS_MallPresentPack* pMsg, DWORD dwParam )
{
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			if( P_VALID(GetObj("LauncherSucess_MsgBox")) )
				KillObj("LauncherSucess_MsgBox");
			CreateObj("LauncherSucess_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherSucess_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade11")], 
				_T("LauncherSucess_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_BagYuanBao_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughYuanbao")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_ExVolume_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughVolume")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_VipPoint_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("CurrencyError_NotEnoughMark")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Bag_NotEnoughSpace:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("PickGroundItemErr1")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Pack_NotEnough:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade22")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_ID_Error:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade23")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_YuanBao_Error:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade24")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Con_PswNotPass:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade27")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Role_Not_ExistIn_World:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade28")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_CreateItem_Failed:// 商品创建失败
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade59")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_CreatePres_Failed:	// 赠品创建失败
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade60")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Trade_ItemNum_MaxHoldLimit:	// 商品最大持有数限制
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade67")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_AlreadyBuy:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Only_Once")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_Level_Limit:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("Gift_Plan_Level_Not_Enough")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	default:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_ServerError")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	}
	return 0;
}

void MallFrame::ShowMallGoodsTips( int nGoodsPos, int nStatePos )
{
	switch(m_nCurMenu)
	{
	case EMIT_Package:
		{
			ASSERT(0 && _T("mall frame goods tips error!\n"));
		}
		break;
	case EMIT_All:
	default:
		{
			DWORD dwID = m_pUnitItem[nGoodsPos]->GetBtnData().GetData1();
			const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(dwID);
			if( P_VALID(pItem) )
			{
				nStatePos = TipsPos2State( pItem, nStatePos );
				switch(nStatePos)
				{
				case EMIST_Promote:
					{
						ShowPromoteProTips(pItem, nGoodsPos, nStatePos);
					}
					break;
				/*case EMIST_GroupBuy:
					{
						ShowGroupBuyProTips(pItem, nGoodsPos, nStatePos);
					}
					break;*/
				/*case EMIST_Present:
					{
						ShowPresentProTips(pItem, nGoodsPos, nStatePos);
					}
					break;*/
				case EMIST_Num:
					{
						ShowNumProTips(pItem, nGoodsPos, nStatePos);
					}
					break;
				/*case EMIST_Exchange:
					{
						ShowExchangeProTips(pItem, nGoodsPos, nStatePos);
					}
					break;*/
				}
			}
		}
		break;
	}
}

void MallFrame::ShowMallPromoteGoodsTips( int nGoodsPos, int nStatePos )
{
	DWORD dwID = m_pUnitProItem[nGoodsPos]->GetBtnData().GetData1();
	const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(dwID);
	if( P_VALID(pItem) )
	{
		nStatePos = TipsPos2State( pItem, nStatePos );
		switch(nStatePos)
		{
		case EMIST_Promote:
			{
				ShowPromoteProTips(pItem, nGoodsPos, nStatePos, EMITT_Promote);
			}
			break;
		/*case EMIST_GroupBuy:
			{
				ShowGroupBuyProTips(pItem, nGoodsPos, nStatePos, EMITT_Promote);
			}
			break;*/
		/*case EMIST_Present:
			{
				ShowPresentProTips(pItem, nGoodsPos, nStatePos, EMITT_Promote);
			}
			break;*/
		case EMIST_Num:
			{
				ShowNumProTips(pItem, nGoodsPos, nStatePos, EMITT_Promote);
			}
			break;
		/*case EMIST_Exchange:
			{
				ShowExchangeProTips(pItem, nGoodsPos, nStatePos, EMITT_Promote);
			}
			break;*/
		}
	}
}

void MallFrame::ShowMallPackTips(int nGoodsPos, int nStatePos)
{
	DWORD dwID = m_pUnitPack[nGoodsPos][0]->GetBtnData().GetData1();
	const tagMallPackProto* pPack = MallMgr::Inst()->GetMallPackByID(dwID);
	if( P_VALID(pPack) )
	{
		nStatePos = TipsPos2State( pPack, nStatePos );
		switch(nStatePos)
		{
		case EMIST_Promote:
			{
				ShowPromoteProTips(pPack, nGoodsPos, nStatePos, EMITT_Package);
			}
			break;
		/*case EMIST_GroupBuy:
			{
				ShowGroupBuyProTips(pPack, nGoodsPos, nStatePos, EMITT_Package);
			}
			break;*/
		/*case EMIST_Present:
			{
				ShowPresentProTips(pPack, nGoodsPos, nStatePos, EMITT_Package);
			}
			break;*/
		case EMIST_Num:
			{
				ShowNumProTips(pPack, nGoodsPos, nStatePos, EMITT_Package);
			}
			break;
		/*case EMIST_Exchange:
			{
				ShowExchangeProTips(pPack, nGoodsPos, nStatePos, EMITT_Package);
			}
			break;*/

		}
	}
}

void MallFrame::ShowPromoteProTips( const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType/*=EMITT_All*/ )
{
	TCHAR szBuff[X_LONG_NAME];
	if( pItem->dwTimeSaleEnd != GT_INVALID )
	{
		DWORD dwTimeSaleEnd = pItem->dwTimeSaleEnd;
		tagDWORDTime TimeSaleEnd = dwTimeSaleEnd;
		tagDWORDTime TimeCur = ServerTime::Inst()->CalCurrentServerDwordTime();
		DWORD dwTimeDiff = CalcTimeDiff(TimeSaleEnd, TimeCur);
		if(dwTimeDiff <= 0)
		{
			_stprintf( szBuff, g_StrTable[_T("MallTrade12")] );
		}
		else 
		{
			DWORD dwDay = MallMgr::Inst()->GetDay(dwTimeDiff);
			DWORD dwHour = MallMgr::Inst()->GetHour(dwTimeDiff, dwDay);
			DWORD dwMin = MallMgr::Inst()->GetMin(dwTimeDiff, dwDay, dwHour);
			_stprintf( szBuff, g_StrTable[_T("MallTrade13")], 
				dwDay, dwHour, dwMin, pItem->nSalePrice );
		}
		tstring strTips = g_StrTable[_T("TipsDefaultFont")];
		strTips += szBuff;
		switch(eType)
		{
		case EMITT_All:
			ShowTip( m_pStcState[nGoodsPos][nStatePos], strTips.c_str() );
			break;
		case EMITT_Package:			
			ShowTip( m_pStcPackState[nGoodsPos][nStatePos], strTips.c_str() );
			break;
		case EMITT_Promote:
			ShowTip( m_pProItemState[nGoodsPos][nStatePos], strTips.c_str() );
			break;
		}
	}
}

void MallFrame::ShowGroupBuyProTips( const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType/*=EMITT_All*/ )
{
	TCHAR szBuff[X_LONG_NAME];
	tstring strTips;
	strTips = g_StrTable[_T("TipsDefaultFont")];
	if( pItem->bySmallGroupDiscount != (BYTE)GT_INVALID )
	{
		_stprintf(szBuff, g_StrTable[_T("MallTrade14")], pItem->bySmallGroupDiscount);
		strTips = strTips + szBuff + _T("\\n");
	}
	if( pItem->byMediumGroupDiscount != (BYTE)GT_INVALID )
	{
		_stprintf(szBuff, g_StrTable[_T("MallTrade15")], pItem->byMediumGroupDiscount);
		strTips = strTips + szBuff + _T("\\n");
	}
	if( pItem->byBigGroupDiscount != (BYTE)GT_INVALID )
	{
		_stprintf(szBuff, g_StrTable[_T("MallTrade16")], pItem->byBigGroupDiscount);		
		strTips = strTips + szBuff + _T("\\n");;
	}
	switch(eType)
	{
	case EMITT_All:
		ShowTip( m_pStcState[nGoodsPos][nStatePos], strTips.c_str() );
		break;
	case EMITT_Package:			
		ShowTip( m_pStcPackState[nGoodsPos][nStatePos], strTips.c_str() );
		break;
	case EMITT_Promote:
		ShowTip( m_pProItemState[nGoodsPos][nStatePos], strTips.c_str() );
		break;
	}
}

void MallFrame::ShowPresentProTips( const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType/*=EMITT_All*/ )
{
	TCHAR szBuff[X_LONG_NAME];
	if( pItem->dwPresentID != GT_INVALID )  
	{
		const tagItemDisplayInfo* pInfo = ItemProtoData::Inst()->FindItemDisplay(pItem->dwPresentID);
		if( P_VALID(pInfo) )
		{
			_stprintf( szBuff, _T("%s x %d"), pInfo->szName, pItem->nPresentNum );
			tstring strTips = g_StrTable[_T("TipsDefaultFont")];
			strTips += szBuff;
			switch(eType)
			{
			case EMITT_All:
				ShowTip( m_pStcState[nGoodsPos][nStatePos], strTips.c_str() );
				break;
			case EMITT_Package:			
				ShowTip( m_pStcPackState[nGoodsPos][nStatePos], strTips.c_str() );
				break;
			case EMITT_Promote:
				ShowTip( m_pProItemState[nGoodsPos][nStatePos], strTips.c_str() );
				break;
			}
		}
	}
}

void MallFrame::ShowNumProTips( const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType/*=EMITT_All*/ )
{
	TCHAR szBuff[X_LONG_NAME];
	tstring strTips;
	if( pItem->nNum != GT_INVALID )
	{
		DWORD dwNum = pItem->nNum;
		if(SaveAccount::Inst()->GetLoginMode() == 7)	
		{
			if(dwNum > MALL_ITEM_PERSONAL_NUM_LIMIT_LINE)	//台湾个人限购
			{
				dwNum -= MALL_ITEM_PERSONAL_NUM_LIMIT_LINE;
				_stprintf( szBuff, g_StrTable[_T("MallTrade76")], dwNum );
			}
			else
			{
				_stprintf( szBuff, g_StrTable[_T("MallTrade17")], dwNum );	//台湾全服限购
			}
		}
		else
		{
			_stprintf( szBuff, g_StrTable[_T("MallTrade17")], dwNum );	//大陆全服限购
		}
		strTips = g_StrTable[_T("TipsDefaultFont")];
		strTips += szBuff;
	}
	else 
	{
		_stprintf( szBuff, g_StrTable[_T("MallTrade18")] );
		strTips = g_StrTable[_T("TipsDefaultFont")];
		strTips += szBuff;
	}
	switch(eType)
	{
	case EMITT_All:
		ShowTip( m_pStcState[nGoodsPos][nStatePos], strTips.c_str() );
		break;
	case EMITT_Package:			
		ShowTip( m_pStcPackState[nGoodsPos][nStatePos], strTips.c_str() );
		break;
	case EMITT_Promote:
		ShowTip( m_pProItemState[nGoodsPos][nStatePos], strTips.c_str() );
		break;
	}
}

void MallFrame::ShowExchangeProTips( const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType/*=EMITT_All*/)
{
	TCHAR szBuff[X_LONG_NAME];
	if( pItem->nExNum!=0 && pItem->nExNum!=GT_INVALID )
	{
		/*const tagItemDisplayInfo* pInfo = ItemProtoData::Inst()->FindItemDisplay(pItem->dwExTypeID);
		if( P_VALID(pInfo) )
		{*/
			_stprintf( szBuff, g_StrTable[_T("MallTrade19")], /*pInfo->szName,*/ pItem->nExNum );
			tstring strTips = g_StrTable[_T("TipsDefaultFont")];
			strTips += szBuff;
			switch(eType)
			{
			case EMITT_All:
				ShowTip( m_pStcState[nGoodsPos][nStatePos], strTips.c_str() );
				break;
			case EMITT_Package:			
				ShowTip( m_pStcPackState[nGoodsPos][nStatePos], strTips.c_str() );
				break;
			case EMITT_Promote:
				ShowTip( m_pProItemState[nGoodsPos][nStatePos], strTips.c_str() );
				break;
			}
		//}
	}
}

void MallFrame::HideCtrlMallItem()
{
	for( int i=0; i<MALL_MAXITEM_PERPAGE; ++i )
	{
		m_pUnitItemBack[i]->SetInvisible(TRUE);
		for(int j=0; j<MALL_ITEM_STATE; j++)
		{
			m_pStcState[i][j]->SetInvisible(TRUE);
		}
		m_pStcGoodsPriceDel[i]->SetInvisible(TRUE);
	}
}

void MallFrame::HideCtrlPromoteItem()
{
	for( int i=0; i<MALL_MAX_PROMOTE_ITEM; ++i )
	{
		m_pProItemBack[i]->SetInvisible(TRUE);
		m_pStcProPriceDel[i]->SetInvisible(TRUE);
		for( int k=0; k<MALL_PROMOTE_ITEM_STATE; ++k )
		{
			m_pProItemState[i][k]->SetInvisible(TRUE);
		}
	}
}

void MallFrame::HideCtrlMallPack()
{
	for( int i=0; i<MALL_MAXPACK_PERPAGE; ++i )
	{
		m_pUnitPackBack[i]->SetInvisible(TRUE);
		for( int j=0; j<MALL_PACKPRO_NUM; ++j )
		{
			m_pUnitPackSel[i][j]->SetInvisible(TRUE);
			m_pUnitPack[i][j]->SetInvisible(TRUE);

		}
		for( int k=0; k<MALL_PACKPRO_STATE; ++k )
		{
			m_pStcPackState[i][k]->SetInvisible(TRUE);
		}
		m_pStcPackPriceDel[i]->SetInvisible(TRUE);
	}
}

bool MallFrame::JudgePromotePro( const tagMallItemProtoBase* pItem )
{
	if( pItem->dwTimeSaleEnd != GT_INVALID )
	{
		DWORD dwTimeSaleEnd = pItem->dwTimeSaleEnd;

		tagDWORDTime TimeSaleEnd = dwTimeSaleEnd;
		tagDWORDTime TimeCur = ServerTime::Inst()->CalCurrentServerDwordTime();
		DWORD dwTimeDiff = CalcTimeDiff(TimeSaleEnd, TimeCur);
		if( dwTimeDiff <= 0 )
			return false;
		else
			return true;
	}

	return false;
}

bool MallFrame::JudgeGroupBuyPro( const tagMallItemProtoBase* pItem )
{
	if( pItem->bySmallGroupDiscount != (BYTE)GT_INVALID )
	{
		return true;
	}
	else if( pItem->byMediumGroupDiscount != (BYTE)GT_INVALID )
	{
		return true;
	}
	else if( pItem->byBigGroupDiscount != (BYTE)GT_INVALID )
	{
		return true;
	}
	
	return false;
}

bool MallFrame::JudgePresentPro( const tagMallItemProtoBase* pItem )
{
	if( pItem->dwPresentID != GT_INVALID )  
	{
		const tagItemDisplayInfo* pInfo = ItemProtoData::Inst()->FindItemDisplay(pItem->dwPresentID);
		if( P_VALID(pInfo) )
			return true;
		else
			return false;
	}
	return false;
}

bool MallFrame::JudgeNumPro( const tagMallItemProtoBase* pItem )
{
	if( pItem->nNum != GT_INVALID )
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool MallFrame::JudgeExchangePro( const tagMallItemProtoBase* pItem )
{
	if( pItem->nExNum!=0 && pItem->nExNum!=GT_INVALID)
	{
		return true;
	}

	return false;
}

DWORD MallFrame::OnBagMoneyChange( tagRoleSilverChange* pGameEvent )
{
	SetMoney(pGameEvent->n64Silver);
	return 0;
}

DWORD MallFrame::OnBagYuanbaoChange( tagRoleYuanbaoChange* pGameEvent )
{
	if(P_VALID(m_pStcYuanBao))
	{
		TCHAR szBuff[X_SHORT_NAME] = {0};
		_sntprintf(szBuff, sizeof(szBuff), _T("%d"), pGameEvent->nYuanbao);
		_itot(CurrencyMgr::Inst()->GetBagYuanbao(), szBuff, 10);
		m_pStcYuanBao->SetText(szBuff) ;

		m_pWnd->SetRefresh(TRUE);
	}
	return 0;
}

DWORD MallFrame::OnExchangeVolumeChange(tagExchangeVolumeChange* pGameEvent)
{
	if( P_VALID(m_pExchangeVolume) )
	{
		TCHAR szBuff[X_SHORT_NAME] = {0};
		_sntprintf(szBuff, sizeof(szBuff), _T("%d"), pGameEvent->nCurExVolume);
		_itot(CurrencyMgr::Inst()->GetExchangeVolume(), szBuff, 10);
		m_pExchangeVolume->SetText(szBuff);

		m_pWnd->SetRefresh(TRUE);
	}
	return 0;
}

DWORD MallFrame::OnMarkChange(tagMarkChange* pGameEvent)
{
	if( P_VALID(m_pStcMark) )
	{
		TCHAR szBuff[X_SHORT_NAME] = {0};
		_sntprintf(szBuff, sizeof(szBuff), _T("%d"), pGameEvent->nCurMark);
		_itot(CurrencyMgr::Inst()->GetMark(), szBuff, 10);
		m_pStcMark->SetText(szBuff);

		m_pWnd->SetRefresh(TRUE);
	}
	return 0;
}

void MallFrame::JudgeTryDress( DWORD dwTypeID )
{
	if( !MIsEquipment(dwTypeID) )
		return;
	const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(dwTypeID);
	LocalPlayer* pLRole = RoleMgr::Inst()->GetLocalPlayer();
	if( P_VALID(pItem) && P_VALID(pLRole) )
	{
		AvatarEquipNode* pNode = (AvatarEquipNode*)pLRole->GetSceneNode();
		if( pItem->eType==EIT_Fashion && P_VALID(pNode) 
		&&( pNode->GetAtt().bySex==(BYTE)pItem->eSexLimit || -1==(BYTE)pItem->eSexLimit) )
		{
			tagEquipProto* pEquip = (tagEquipProto*)pItem;
			if( P_VALID(pEquip) )
			{
				EAvatarElement eAvat = (EAvatarElement)MallMgr::Inst()->GetEquipDisplayPos((INT16)pEquip->eEquipPos);
				tagEquipDisplay ep;
				ep.dwTypeID = pEquip->dwTypeID;
				ep.byDisplayPos = eAvat;
				ep.n8ColorID = HandleFation(eAvat, pEquip->bCanDye, dwTypeID );
				m_pWndTryDress->SetEquip(eAvat, ep);
				UpdateAvatar();
			}
		}
	}
}

void MallFrame::SetPresentMaxNum()
{
	INT nMaxValue = GT_INVALID;
	if( !m_bSelectPromote )
	{
		switch(m_nCurMenu)
		{
		case EMIT_Package:
			{
				nMaxValue = 1;
			}
			break;
		case EMIT_All:
		default:
			{
				const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
				if( !P_VALID(pItem) )
					return;
				const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pItem->dwTypeID);
				if( !P_VALID(pProto) )
					return;
				nMaxValue = pProto->n16MaxLapNum;
			}
			break;
		}
	}
	else 
	{
		const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);
		if( !P_VALID(pItem) )
			return;
		const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pItem->dwTypeID);
		if( !P_VALID(pProto) )
			return;
		nMaxValue = pProto->n16MaxLapNum;
	}
	if( nMaxValue>=0 )
		MallMgr::Inst()->SetMallGiftFrameMaxBuyNum(nMaxValue);
}

void MallFrame::ShowWnd( BOOL bVal )
{ 
    if(P_VALID(m_pWnd)) m_pWnd->SetInvisible(!bVal);
    if (bVal)
        YuanbaoTradeMgr::Inst()->BeginNC_SynYBTradeInfo();
}

void MallFrame::SetOperateState( EMallItemOperateState eType/*=EMIOS_NULL*/ )
{	
	//<! 注释原因：龙商城废弃功能
	//m_eOperateState = eType;		
	//switch(m_eOperateState)	
	//{	
	//case EMIOS_NULL:	
	//case EMIOS_Free:		
	//	{
	//		//m_pBtnExchange->SetEnable(false);		
	//	}	
	//case EMIOS_PackItem:		
	//	{
	//		//m_pBtnGroupBuy->SetEnable(false);			
	//		/*if( m_eOperateState==EMIOS_PackItem )			
	//		{
	//			const tagMallPackProto* pItem = MallMgr::Inst()->GetMallPackByID(m_dwSelectItemID);	
	//			if( P_VALID(pItem) )				
	//			{					
	//				if( JudgeExchangePro(pItem) )						
	//					m_pBtnExchange->SetEnable(true);				
	//				else
	//					m_pBtnExchange->SetEnable(false);		
	//			}
	//		else 					
	//			m_pBtnExchange->SetEnable(false);			
	//		}	*/
	//	}		
	//break;	
	//case EMIOS_NormalItem:	
	//case EMIOS_Promote:		
	//	{
	//	const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);			
	//	if( P_VALID(pItem) )			
	//	{				
	//		/*if( pItem->bySmallGroupDiscount!=(BYTE)GT_INVALID|| pItem->byMediumGroupDiscount!=(BYTE)GT_INVALID	
	//			|| pItem->byBigGroupDiscount!=(BYTE)GT_INVALID )			
	//		{					
	//			LocalPlayer* pLocal = RoleMgr::Inst()->GetLocalPlayer();					
	//			if( P_VALID(pLocal) && pLocal->IsInGuild() )					
	//			{						
	//				m_pBtnGroupBuy->SetEnable(true);					
	//			}				
	//		}				
	//		else					
	//			m_pBtnGroupBuy->SetEnable(false);*/				
	//		/*if( JudgeExchangePro(pItem) )					
	//			m_pBtnExchange->SetEnable(true);				
	//		else					
	//			m_pBtnExchange->SetEnable(false);		*/	
	//	}			
	//	else			
	//	{				
	//		//m_pBtnGroupBuy->SetEnable(false);				
	//		//m_pBtnExchange->SetEnable(false);			
	//	}		
	//	}		
	//	break;	
	//}
}

void MallFrame::ExchangeMallItem()
{	
	INT nMaxValue = GT_INVALID;		
	//推荐商品特殊处理	
	if( m_bSelectPromote )  	
	{		
		const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);		
		if( !P_VALID(pItem) )			
			return;		
		const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pItem->dwTypeID);		
		if( !P_VALID(pProto) )			
			return;		
		nMaxValue = pProto->n16MaxLapNum;	
	}	
	else	
	{		
		switch(m_nCurMenu)		
		{		
		case EMIT_Package:			
			{				
				const tagMallPackProto* pItem = MallMgr::Inst()->GetMallPackByID(m_dwSelectItemID);				
				if( !P_VALID(pItem) )					
					return;				
				nMaxValue = 1;			
			}			
			break;		
		case EMIT_All:		
		default:			
			{				
				const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);	
				if( !P_VALID(pItem) )					
					return;				
				const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pItem->dwTypeID);				
				if( !P_VALID(pProto) )					
					return;				
				nMaxValue = pProto->n16MaxLapNum;			
			}			
			break;		
		}	
	}	
	if(nMaxValue==1)	
	{		
		if( m_nCurMenu == EMIT_Package )
		{
			tagMsgInputBoxEvent evt(_T("ExchangePackItemNum_Event"), NULL);
			evt.eInputType = MBIT_Number;		
			evt.eResult = MBF_OK;		
			evt.nInputNum = nMaxValue;
			m_pFrameMgr->SendEvent(&evt);	
		}
		else
		{
			tagMsgInputBoxEvent evt(_T("ExchangeMallItemNum_Event"), NULL);
			evt.eInputType = MBIT_Number;		
			evt.eResult = MBF_OK;		
			evt.nInputNum = nMaxValue;	
			m_pFrameMgr->SendEvent(&evt);	
		}
	}	
	else
	{		
		if( P_VALID(GetObj("ExchangeMallItemNum_MsgBox")) )
			KillObj("ExchangeMallItemNum_MsgBox");
		CreateObj("ExchangeMallItemNum_MsgBox", "MsgInputBox");		
		TObjRef<MsgInputBox>("ExchangeMallItemNum_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade51")], 			
			_T("ExchangeMallItemNum_Event"), nMaxValue, (MsgBoxInputType)(MBIT_Number), _T("1"), TRUE);	
	}
}

DWORD MallFrame::OnExchangePackItem( tagMsgInputBoxEvent* pGameEvent )
{	
	if(MBF_OK == pGameEvent->eResult && MBIT_Number == pGameEvent->eInputType)	
	{		
		const tagMallPackProto* pPack = MallMgr::Inst()->GetMallPackByID(m_dwSelectItemID);		
		m_n16BuyNum = pGameEvent->nInputNum;		
		if( P_VALID(pPack) )		
		{						
			if( m_n16BuyNum > pPack->nNum )			
			{				
				if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )					
					KillObj("NotBuyMallItem_MsgBox");				
				CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");				
				TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade53")], 		
					_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);				
				return 0;			
			}						
			INT nExVolume = 0;			
			nExVolume = pPack->nExNum;			
			if( nExVolume*m_n16BuyNum <= CurrencyMgr::Inst()->GetExchangeVolume() )			
			{				
				if( P_VALID(GetObj("AffirmExchangePackItem_MsgBox")) )			
					KillObj("AffirmExchangePackItem_MsgBox");			
				CreateObj("AffirmExchangePackItem_MsgBox", "MsgBox");			
				TCHAR szBuff[X_LONG_NAME] = {0};		
				_stprintf( szBuff, g_StrTable[_T("MallTrade54")], nExVolume*m_n16BuyNum );		
				TObjRef<MsgBox>("AffirmExchangePackItem_MsgBox")->Init(_T(""), szBuff, 		
					_T("AffirmExchangePackItem_Event"), MsgBoxFlag(MBF_OK|MBF_Cancel), TRUE);		
			}			
			else 			
			{				
				if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )			
					KillObj("NotBuyMallItem_MsgBox");			
				CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");			
				TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade52")], 			
					_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);			
			}		
		}	
	}
	return 0;
}
DWORD MallFrame::OnExchangeMallItem( tagMsgInputBoxEvent* pGameEvent )
{	if(MBF_OK == pGameEvent->eResult && MBIT_Number == pGameEvent->eInputType)	
	{		
		const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);	
		m_n16BuyNum = pGameEvent->nInputNum;		
		if( P_VALID(pItem) )		
		{			
			if( m_n16BuyNum > pItem->nNum && pItem->nNum != GT_INVALID)	
			{			
				if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )			
					KillObj("NotBuyMallItem_MsgBox");			
				CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");			
				TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade53")], 	
					_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);			
				return 0;			
			}					
			INT nExVolume = 0;			
			nExVolume = pItem->nExNum;			
			if( nExVolume*m_n16BuyNum <= CurrencyMgr::Inst()->GetExchangeVolume() )		
			{				
				if( P_VALID(GetObj("AffirmExchangeMallItem_MsgBox")) )			
					KillObj("AffirmExchangeMallItem_MsgBox");				
				CreateObj("AffirmExchangeMallItem_MsgBox", "MsgBox");			
				TCHAR szBuff[X_LONG_NAME] = {0};			
				_stprintf( szBuff, g_StrTable[_T("MallTrade54")], nExVolume*m_n16BuyNum );		
				TObjRef<MsgBox>("AffirmExchangeMallItem_MsgBox")->Init(_T(""), szBuff, 			
					_T("AffirmExchangeMallItem_Event"), MsgBoxFlag(MBF_OK|MBF_Cancel), TRUE);		
			}			
			else 	
			{		
				if( P_VALID(GetObj("NotBuyMallItem_MsgBox")) )		
					KillObj("NotBuyMallItem_MsgBox");				
				CreateObj("NotBuyMallItem_MsgBox", "MsgBoxEx");			
				TObjRef<MsgBoxEx>("NotBuyMallItem_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade52")], 			
					_T("NotBuyMallItem_Event"), MsgBoxFlag(MBF_OK), TRUE);		
			}	
		}	
	}
	return 0;
}

DWORD MallFrame::OnAffirmExchangePackItem( tagMsgBoxEvent* pGameEvent )
{	
	if(MBF_OK == pGameEvent->eResult)
	{		
		const tagMallPackProto* pPack = MallMgr::Inst()->GetMallPackByID(m_dwSelectItemID);		
		if( P_VALID(pPack) )	
		{			
			tagNC_MallPackExchange msg;
			//msg.eShopingDistrict
			msg.dwMallID = pPack->dwID;		
			msg.nIndexInServer = pPack->nIndexInServer;	
			msg.nPrice = pPack->nExNum;	
			m_pSession->Send(&msg);		
		}		
	}
	return 0;
}

DWORD MallFrame::OnAffirmExchangeMallItem( tagMsgBoxEvent* pGameEvent )
{	
	if(MBF_OK == pGameEvent->eResult)	
	{		
		const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectItemID);	
		if( P_VALID(pItem) )	
		{		
			tagNC_MallItemExchange msg;			
			msg.dwMallID = pItem->dwID;		
			msg.nIndexInServer = pItem->nIndexInServer;	
			msg.nPrice = pItem->nExNum;		
			msg.n16BuyNum = m_n16BuyNum;		
			m_pSession->Send(&msg);	
		}		
	}
	return 0;
}

DWORD MallFrame::OnNS_MallItemExchange( tagNS_MallItemExchange* pMsg, DWORD dwParam )
{	
	switch(pMsg->dwErrorCode)	
	{	
	case E_Success:		
		{		
			if( P_VALID(GetObj("LauncherSucess_MsgBox")) )		
				KillObj("LauncherSucess_MsgBox");			
			CreateObj("LauncherSucess_MsgBox", "MsgBoxEx");		
			TObjRef<MsgBoxEx>("LauncherSucess_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade55")], 	
				_T("LauncherSucess_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}	
		break;	
	case E_Mall_Close:	
		{			
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade46")], 		
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}		
		break;	
	case E_Mall_Item_NotEnough:	
		{		
			if( P_VALID(GetObj("LauncherError_MsgBox")) )			
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");		
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade53")], 	
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}		
		break;	
	case E_Mall_ID_Error:	
		{			
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");	
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade56")], 	
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}		
		break;	
	case E_Mall_ExVolume_Error:	
		{			
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade58")], 		
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}		
		break;	
	case E_Mall_Exchange_NotAllowable:		
		{			
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade57")], 	
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}		
		break;	
	case E_ExVolume_NotEnough:  //赠点不足		
		{			
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");	
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");		
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade52")], 		
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}		
		break;	
	case E_Mall_CreateItem_Failed:// 商品创建失败	
		{		
			if( P_VALID(GetObj("LauncherError_MsgBox")) )		
				KillObj("LauncherError_MsgBox");			
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade59")], 		
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}		
		break;	
	case E_Mall_CreatePres_Failed:	// 赠品创建失败	
		{		
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade60")], 	
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}		
		break;	
	case E_Bag_NotEnoughSpace:  //背包空间不足		
		{			
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");	
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("PickGroundItemErr1")], 	
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}		
		break;	
	case E_Con_PswNotPass: //行囊没解锁		
		{			
			if( P_VALID(GetObj("LauncherError_MsgBox")) )		
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade47")], 	
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}		
		break;	
	case E_Mall_Offer_AlreadyBuy:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Only_Once")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_Level_Limit:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("Gift_Plan_Level_Not_Enough")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	default:	
		{			
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_ServerError")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}		
		break;	
	}
	return 0;
}
DWORD MallFrame::OnNS_MallPackExchange( tagNS_MallPackExchange* pMsg, DWORD dwParam )
{	
	switch(pMsg->dwErrorCode)	
	{	
	case E_Success:		
		{		
			if( P_VALID(GetObj("LauncherSucess_MsgBox")) )	
				KillObj("LauncherSucess_MsgBox");			
			CreateObj("LauncherSucess_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherSucess_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade55")], 
				_T("LauncherSucess_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}		
		break;	
	case E_Mall_Close:	
		{			
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade46")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}		
		break;	
	case E_Mall_Pack_NotEnough:		
		{		
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");		
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade53")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}		
		break;	
	case E_Mall_ID_Error:	
		{		
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade56")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}		
		break;	
	case E_Mall_ExVolume_Error:	
		{		
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade58")], 	
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}		
		break;	
	case E_Mall_Exchange_NotAllowable:	
		{		
			if( P_VALID(GetObj("LauncherError_MsgBox")) )		
				KillObj("LauncherError_MsgBox");			
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade57")], 	
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}		
		break;	
	case E_ExVolume_NotEnough:  //赠点不足		
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");		
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade52")], 	
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}	
		break;	
	case E_Mall_CreateItem_Failed:// 商品创建失败	
		{		
			if( P_VALID(GetObj("LauncherError_MsgBox")) )				
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");		
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade59")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}		
		break;	
	case E_Mall_CreatePres_Failed:	// 赠品创建失败	
		{		
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");			
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");	
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade60")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);		
		}		
		break;	
	case E_Bag_NotEnoughSpace:  //背包空间不足	
		{			
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");		
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("PickGroundItemErr1")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}		
		break;	
	case E_Con_PswNotPass: //行囊没解锁	
		{			
			if( P_VALID(GetObj("LauncherError_MsgBox")) )	
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");		
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade47")], 	
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}		
		break;	
	case E_Mall_Offer_AlreadyBuy:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_Only_Once")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	case E_Mall_Offer_Level_Limit:
		{
			if( P_VALID(GetObj("LauncherError_MsgBox")) )
				KillObj("LauncherError_MsgBox");
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");

			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("Gift_Plan_Level_Not_Enough")], 
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);
		}
		break;
	default:	
		{		
			if( P_VALID(GetObj("LauncherError_MsgBox")) )		
				KillObj("LauncherError_MsgBox");		
			CreateObj("LauncherError_MsgBox", "MsgBoxEx");		
			TObjRef<MsgBoxEx>("LauncherError_MsgBox")->Init(_T(""), g_StrTable[_T("MallTrade_ServerError")], 	
				_T("LauncherError_Event"), MsgBoxFlag(MBF_OK), TRUE);	
		}
		break;	
	}
	return 0;
}

int EncodeBase64(BYTE *btSrc, int iSrcLen, BYTE *btRet, int *piRetLen);

void MallFrame::YuanBaoAbundance()
{	
	tstring url;
	switch( TObjRef<NetSession>()->GetLoginMode())
	{
	case 1:
		{
			url = TObjRef<NetSession>()->GetExpressPayUrl();
		}
		break;
	case 2:  //迅雷特殊处理
		{
			url = _T("http://pay.xunlei.com/ourproducts_long.html");
			ShellExecute(NULL, _T("open"), url.c_str(), _T(""), NULL, SW_MAXIMIZE);
			return ;
		}
		break;
	default:
		{
			url = TObjRef<NetSession>()->GetExpressPayUrl();
			ShellExecute(NULL, _T("open"), url.c_str(), _T(""), NULL, SW_MAXIMIZE);
			return ;
		}
		break;
	}
	
	//读取帐号
	tstring strAccountTemp = SaveAccount::Inst()->GetAccount();

	tstring strBoroghID = SaveAccount::Inst()->GetWorldID();
	DWORD dwAccountID = SaveAccount::Inst()->GetAccountID();
	string strCannelID = SaveAccount::Inst()->GetJSKK_Channel();
	DWORD dwCropid = SaveAccount::Inst()->GetCropID();
	/*TObjRef<VarContainer> serInfo = CreateObj("lastlogin", "VarContainer");
	const TCHAR* szFileName = _T("config\\lastlogin.xml");
	if( serInfo->Load(NULL, szFileName) )
	{
		strBoroghID = serInfo->GetString(_T("world_id"), _T("last_login_server"), _T(""));
	}
	else
	{	
		strBoroghID = _T("");
	}
	KillObj("lastlogin");*/

	//Base64编码
	tstringstream ss2;
	if( ECT_JinShanKuaiKuai ==  dwCropid )
	{
		ss2 << strAccountTemp << _T("||") << ECT_JinShanKuaiKuai;
	}
	else
	{
		ss2 << strAccountTemp;
	}

	if (ECT_YYYuYinPingTai ==SaveAccount::Inst()->GetYYCropID() )	
		dwCropid = ECT_YYYuYinPingTai;

	string strAtemp = m_pUtil->UnicodeToAnsi(ss2.str().c_str());
	char btSrc[X_LONG_NAME] = {0};
	char btRet[X_LONG_NAME] = {0};
	int iRetLen = sizeof(btRet);
	_snprintf( btSrc, X_LONG_NAME, strAtemp.c_str() );
	EncodeBase64((BYTE*)btSrc, strlen(btSrc), (BYTE*)btRet, &iRetLen);

	tstring strAccount = m_pUtil->AnsiToUnicode(btRet);
	tstring strWchannel = m_pUtil->AnsiToUnicode(strCannelID.c_str());
	tstringstream ss;
	ss << _T("?cropid=") << dwCropid << _T("&accountname=") << strAccount << _T("&worldid=") << strBoroghID<< _T("&channel=") << strWchannel; 
	tstring strYuanBaoAbundance = url + ss.str();

	//打开充值网页
	string str = SaveAccount::Inst()->GetChargeUrl(); 
	if (str.empty())
		ShellExecute(NULL, _T("open"), strYuanBaoAbundance.c_str(), _T(""), NULL, SW_MAXIMIZE);
	else
	{
		tstring szUrl = m_pUtil->AnsiToUnicode( str.c_str() );
		DWORD dwCropid = SaveAccount::Inst()->GetCropID();
		tstring world = SaveAccount::Inst()->GetWorldID();
		tstringstream ts;
		ts<<szUrl; 
		ts<<_T("cropid=")<< dwCropid;
		ts<<_T("&accountname=")<< strAccount;
		ts<<_T("&worldid=")<< world;
		ShellExecute(NULL, _T("open"), ts.str().c_str(), _T(""), NULL, SW_MAXIMIZE);
	}
}

void MallFrame::ShowMallNotice( BOOL bVal )
{
	//<!F计划关闭商城公告，接口保留功能暂时注掉
	//m_pWndNotice->SetInvisible(!bVal);

	////读取URL
	//TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
	//TCHAR szTemp[X_LONG_NAME*2];

	//_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
	//tstring strAppPath = szTemp;

	//tstring strIniFileFullName = strAppPath + szIniFileName;
	//::GetPrivateProfileString(_T("Launcher"), _T("MallNoticeURL"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
	//tstring url = szTemp;
	//if( url == _T("") )
	//{
	//	MessageBox(NULL, _T("Don't find mall notice url!"), _T(""), MB_OK);
	//	return;
	//}

	//if( m_strMallNotice!=url )
	//{
	//	m_strMallNotice = url;
	//	m_pHttp_MallNotice->SetPicURL(m_strMallNotice);
	//}
}

int MallFrame::HandleFation( EAvatarElement eAvat, bool bCanDye, DWORD dwTypeID )
{
	int nColor = 0;
	switch( eAvat )
	{
	case EAE_Head:
		nColor = m_pCBDayFation[eAvat]->GetListBox()->GetCurSelectedRow();
		m_dwTryDressID[eAvat] = dwTypeID;
		m_pCBDayFation[eAvat]->SetInvisible(!bCanDye);
		m_pStcDyeTips[eAvat]->SetInvisible(!bCanDye);
		break;
	case EAE_Upper:
	case EAE_Lower:
	case EAE_Glove:
	case EAE_Boot:
	case EAE_Back:
		nColor = m_pCBDayFation[eAvat - 1]->GetListBox()->GetCurSelectedRow();
		m_dwTryDressID[eAvat - 1] = dwTypeID;
		m_pCBDayFation[eAvat - 1]->SetInvisible(!bCanDye);
		m_pStcDyeTips[eAvat - 1]->SetInvisible(!bCanDye);
		break;
	}
	return nColor;
}

void MallFrame::SetSubMenuText(INT nMenu)
{
	for (int i = 0; i < MALL_ITEM_SECOND_SUBTYPE; ++i)
	{
		map<pair<DWORD, int>, DWORD>::const_iterator it = m_map1andBtIndexTo2.find(make_pair(nMenu, i));
		if (it != m_map1andBtIndexTo2.end())
		{
			const tagMallTitle2Data* pTitle2 = MallData::Inst()->FindMall2Data(it->second);
			if (P_VALID(pTitle2))
			{
				m_pPBtnMallType2[i]->SetInvisible(FALSE);
				m_pPBtnMallType2[i]->SetText(pTitle2->strTitle2Name.c_str());
				m_pPBtnMallType2[i]->SetRefresh(TRUE);
			}
			else
			{
				m_pPBtnMallType2[i]->SetInvisible(TRUE);
			}
		}
		else
		{
			m_pPBtnMallType2[i]->SetInvisible(TRUE);
		}
	}

    if (nMenu == EMIT_All || nMenu == EMIT_Package 
	  ||nMenu == EMIT_Mark || nMenu == EMIT_Volume )
    {
        for (int i = 0; i < MALL_ITEM_SECOND_SUBTYPE; ++i)
        {
            m_pPBtnMallType2[i]->SetInvisible(TRUE);
        }
    }
}
/*====================================================================
Base64编码函数
btSrc指向被编码的数据缓冲区
iSrcLen被编码的数据的大小（字节）
btRet指向存放Base64编码的数据缓冲区
piRetLen指向存放Base64编码的数据缓冲区的长度
若btRet为NULL函数返回0，piRetLen传回btSrc的Base64编码所需缓冲区的大小
若btRet指向的缓冲区太小，函数返回-1
否则函数返回实际btSrc的Base64编码所需缓冲区的大小
=====================================================================*/
int EncodeBase64(BYTE *btSrc, int iSrcLen, BYTE *btRet, int *piRetLen) 
{
	int i = 0, j = 0, k = 0 ;
	BYTE EncodeBase64Map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	i = (iSrcLen+2) / 3 * 4 ; //获取btSrc的Base64编码所需缓冲区的大小
	if(btRet != NULL)
	{
		if(*piRetLen < i) //Base64编码所需缓冲区偏小
			return -1 ;
		*piRetLen = i ; //*piRetLen返回btSrc的Base64编码的缓冲区大小
	}
	else
	{
		*piRetLen = i ;
		return 0 ;
	}

	k = iSrcLen - iSrcLen % 3 ;
	for(i=j=0; i<k; i+=3) //编码
	{
		btRet[j++] = EncodeBase64Map[(btSrc[i]>>2)&0x3F] ;
		btRet[j++] = EncodeBase64Map[((btSrc[i]<<4) + (btSrc[i+1]>>4))&0x3F] ;
		btRet[j++] = EncodeBase64Map[((btSrc[i+1]<<2) + (btSrc[i+2]>>6))&0x3F] ;
		btRet[j++] = EncodeBase64Map[btSrc[i+2]&0x3F] ;
	}
	k = iSrcLen - k ;
	if(1 == k)
	{
		btRet[j++] = EncodeBase64Map[(btSrc[i]>>2)&0x3F] ;
		btRet[j++] = EncodeBase64Map[(btSrc[i]<<4)&0x3F] ;
		btRet[j++] = '=' ;
		btRet[j] = '=' ;
	}
	else if(2 == k)
	{
		btRet[j++] = EncodeBase64Map[(btSrc[i]>>2)&0x3F] ;
		btRet[j++] = EncodeBase64Map[((btSrc[i]<<4) + (btSrc[i+1]>>4))&0x3F] ;
		btRet[j++] = EncodeBase64Map[(btSrc[i+1]<<2)&0x3F] ;
		btRet[j] = '=' ;
	}

	return ++j ;
}

DWORD MallFrame::GetCurPackID(INT index)
{
	list<tagMallPackProto*> listMallPack = MallMgr::Inst()->GetMallPack();
	list<tagMallPackProto*>::iterator it = listMallPack.begin();
	int j=0; 
	for(; j<m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE]*MALL_MAXPACK_PERPAGE; ++j)
	{
		++it;
		if( j > int(listMallPack.size()) )
			break;
	}

	int i = 0;
	tagMallPackProto* pPack = NULL;
	while ( i < MALL_MAXPACK_PERPAGE && it != listMallPack.end() ) 
	{
		pPack = *it;
		if( P_VALID(pPack) )
		{
			if ( index == i)
			{
				return pPack->dwID;
			}
		}
		++it;
		++i;
	}
	return GT_INVALID;
}
DWORD MallFrame::OnNS_MallUpdate(tagUpdateMallDataEvent* pGameEvent)
{
	if (!P_VALID(m_pWnd))
	{
		return 0;
	}
	//重新刷新一下
	if ( m_nCurMenu>EMIT_NULL && m_nCurMenu<EMIT_END)
	{
		switch(m_nCurMenu)
		{
		case EMIT_Package:
			{
				ShowCurPageMallPack(m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE]);
			}
			break;
		case EMIT_All:
			{
				ShowCurPageMallItemByAll(m_nSpecialPage[m_nCurMenu+SPECIAL_TYPE]);
			}
			break;
		default:
			{
				ShowCurPageMallItemByType(m_nCurPage[m_nCurMenu][m_nCurSubMenu]);
			}
			break;
		}
	}
	
	return 0;
}

int MallFrame::TipsPos2State( const tagMallItemProtoBase* pItem,  int nStatePos )
{
	//新 热 促 限， 索引值分别对应 0 1 2 3
	int nHaveStateNum = 0;
	int nStateNum = 0;
	//新
	if( pItem->bNew )
	{
		if ( nHaveStateNum == nStatePos )
			return nStateNum;
		nHaveStateNum++;
	}
	nStateNum++;

	//热
	if( pItem->bHot )	
	{
		if ( nHaveStateNum == nStatePos )
			return nStateNum;
		nHaveStateNum++;
	}
	nStateNum++;
		 
	//促
	if( JudgePromotePro(pItem) )
	{
		if ( nHaveStateNum == nStatePos )
			return nStateNum;
		nHaveStateNum++;
	}
	nStateNum++;
		 
	//限
	if( JudgeNumPro(pItem) )
	{
		if ( nHaveStateNum == nStatePos )
			return nStateNum;
	}
	return 0;
}

//设置当前选中类型，是不是礼品包，供外部调用（如死亡界面，便捷购买）
void MallFrame::SetCurMenu( EMallItemType eMallItemType )
{
	PopUpAll();
	m_bIsExterior = TRUE;
	m_nCurMenu	  = eMallItemType;
	if ( EMIT_All == m_nCurMenu )
	{
		 m_pPBtnAll->SetState(EGUIBS_PushDown,FALSE);	
	} 
	else if ( EMIT_Package == m_nCurMenu )
	{
		m_pPBtnPackage->SetState(EGUIBS_PushDown,FALSE);	
	} 
	else if ( EMIT_Volume == m_nCurMenu )
	{
		m_pPBtnPresent->SetState(EGUIBS_PushDown,FALSE);	
	} 
	else if ( EMIT_Mark == m_nCurMenu )
	{
		m_pPBtnMark->SetState(EGUIBS_PushDown,FALSE);	
	}
	//1级分页类（不包括EMIT_Volume，EMIT_Mark）
	else if (eMallItemType>=0&&eMallItemType<=5)
	{
		m_pPBtnSubMall[eMallItemType]->SetState(EGUIBS_PushDown,FALSE);	
	}
}

VOID MallFrame::PopUpAll()
{
	// 全部商品按钮
	m_pPBtnAll->SetState(EGUIBS_PopUp);	 
	// 打包商品按钮
	m_pPBtnPackage->SetState(EGUIBS_PopUp);	
	// 商品分类按钮 一级分类
	for (int i=0;i<6;i++)
	{
		m_pPBtnSubMall[i];
	}
	// 赠卷分类按钮
	m_pPBtnPresent->SetState(EGUIBS_PopUp);	
	// 讥积分分类按钮
	m_pPBtnMark->SetState(EGUIBS_PopUp);

	for(int i=0; i<LEVEL_NUM_CHOOSE; ++i)
	{
		m_pGPBLevel[i]->SetState(EGUIBS_PopUp);
	}

}

VOID MallFrame::SetGiftPlan()
{
	if (!P_VALID(m_pWnd))
	{
		return;
	}
	
	for (int i=0; i<LEVEL_NUM_CHOOSE; ++i)
	{
		BOOL bNeedGlint = MallMgr::Inst()->InquireIndexForClint(i);
		m_pGPBLevel[i]->SetGlintStart(bNeedGlint);
	}

	m_pWnd->SetRefresh(TRUE);
}

VOID MallFrame::RefreshShopingCart()
{
	TCHAR szText[128] = {0};
	DWORD dwYuanBao = 0;
	for(int i = 0; i < Mall_ShopingCart_Num; i++)
	{
		if(m_pShopingItem[i] != NULL)
		{
			m_pIStcShopingCartItem[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(m_pShopingItem[i]->dwTypeID));
			m_pIStcShopingCartItem[i]->SetInvisible(FALSE);
			_stprintf(szText, _T("%d"), m_nShopingIetmNum[i]);
			m_pIStcShopingCartItem[i]->SetText(szText);

			//计算花费胜币数
			DWORD dwTime = CalcTimeDiff(tagDWORDTime(DWORD(m_pShopingItem[i]->dwTimeSaleEnd)), ServerTime::Inst()->CalCurrentServerDwordTime());
			if( dwTime==0 || m_pShopingItem[i]->dwTimeSaleEnd==GT_INVALID)
				dwYuanBao += m_pShopingItem[i]->nPrice * m_nShopingIetmNum[i];
			else 
				dwYuanBao += m_pShopingItem[i]->nSalePrice * m_nShopingIetmNum[i];
		}
	}

	_stprintf(szText, _T("%d"), dwYuanBao);
	m_pStcYuanBaoCast->SetText(szText);

	if(m_dwMallCfg[EMCA_BuyActivityRewardLine] != 0 && m_dwMallCfg[EMCA_BuyActivityOpen] != 0)
	{
		_stprintf(szText, g_StrTable[_T("bei")], (dwYuanBao/m_dwMallCfg[EMCA_BuyActivityRewardLine]));
		m_pStcRewardNum->SetText(szText);
	}

}

VOID MallFrame::RefreshShopingReward()
{
	DWORD dwTime = GetCurrentDWORDTime();	
	if(m_dwMallCfg[EMCA_BuyActivityOpen] == 0)	//活动关闭
	{
		for(int i = 0; i < Mall_ShopingCart_RewardNum; i++)
		{
			m_pIStcShopingCartReward[i]->SetInvisible(TRUE);
		}
		m_pStcShopingTips->SetText(g_StrTable[_T("Dak_YuanBao")]);
		return;
	}

	//刷新物品图标数量
	for(int i = 0; i < Mall_ShopingCart_RewardNum; i++)
	{
		if(m_dwMallCfg[(EMCA_BuyActivityRewardItem0 + i)] != 0 && m_dwMallCfg[(EMCA_BuyActivityRewardItem0 + i)] != GT_INVALID)
		{
			m_pIStcShopingCartReward[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(m_dwMallCfg[(EMCA_BuyActivityRewardItem0 + i)]));
			TCHAR szText[128] = {0};
			_stprintf(szText, _T("%d"), m_dwMallCfg[(EMCA_BuyActivityRewardItemNum0 + i)]);
				m_pIStcShopingCartReward[i]->SetText(szText);
			m_pIStcShopingCartReward[i]->SetInvisible(FALSE);
		}
	}

	TCHAR szText[128] = {0};
	_stprintf(szText, g_StrTable[_T("MallTrade79")], m_dwMallCfg[EMCA_BuyActivityRewardLine]);
	m_pStcShopingTips->SetText(szText);
}

VOID MallFrame::HideShopingCart(bool bHide)
{
	for(int i = 0; i < Mall_ShopingCart_Num; i++)
	{
		m_pIStcShopingCartItem[i]->SetInvisible(bHide);
	}
}

VOID MallFrame::HideShopingReward(bool bHide)
{
	for(int i = 0; i < Mall_ShopingCart_RewardNum; i++)
	{
		m_pIStcShopingCartReward[i]->SetInvisible(bHide);
	}
}

VOID MallFrame::HideAddToShopingCart(bool bHide)
{
	for(int i = 0; i < MALL_MAXITEM_PERPAGE; i++)
	{
		m_pBtnShopingCart[i]->SetInvisible(bHide);
	}
}

VOID MallFrame::AddToShopingCart(int nNum)
{
	const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwAddItemId);
	const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pItem->dwTypeID);
	if( !P_VALID(pProto) )
		return;

	for(int i = 0; i < Mall_ShopingCart_Num; i++)
	{
		//判断是否已经添加过了
		if(m_pShopingItem[i] != NULL && pItem->dwID == m_pShopingItem[i]->dwID && pProto->n16MaxLapNum >= (m_nShopingIetmNum[i] + nNum))
		{
			m_nShopingIetmNum[i] += nNum;
			break;
		}
		else if(m_pShopingItem[i] != NULL && pItem->dwID == m_pShopingItem[i]->dwID && pProto->n16MaxLapNum < (m_nShopingIetmNum[i] + nNum))
		{
			nNum -= (pProto->n16MaxLapNum - m_nShopingIetmNum[i]);
			m_nShopingIetmNum[i] = pProto->n16MaxLapNum;
		}
		else if(m_pShopingItem[i] == NULL)
		{
			m_pShopingItem[i] = pItem;
			m_nShopingIetmNum[i] = nNum;
			break;
		}
	}
	RefreshShopingCart();
}

DWORD MallFrame::OnAddToShopingCart(tagMsgInputBoxEvent* pGameEvent)
{
	if(pGameEvent->eResult == MBF_OK)
		AddToShopingCart(pGameEvent->nInputNum);

	return 0;
}

VOID MallFrame::ClearShopingCart()
{
	for(int i = 0; i < Mall_ShopingCart_Num; i++)
	{
		m_pShopingItem[i] = NULL;
		m_nShopingIetmNum[i] = 0;
	}
	m_dwAddItemId = GT_INVALID;
	HideShopingCart();
	m_pStcYuanBaoCast->SetText(_T(""));
	m_pStcRewardNum->SetText(_T(""));
}

DWORD MallFrame::OnNS_MallUpdateCfg(tagNS_MallUpdateCfg* pMsg, DWORD dwParam)
{
	HideShopingReward();
	ClearShopingCart();
	for(int i = 0; i < EMCA_End; i++)
	{
		m_dwMallCfg[i] = pMsg->dwMallCfg[i];
	}

	RefreshShopingReward();

	return 0;
}

DWORD MallFrame::OnNS_MallShoppingCart(tagNS_MallShoppingCart* pmsg, DWORD dwParam)
{
	switch(pmsg->dwErrorCode)
	{
	case E_Success:
		ClearShopingCart();
		break;
	case E_Mall_Close:	//商城关闭
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("MallTrade46")]);
		break;
	case E_Con_PswNotPass:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("MallTrade47")]);
		break;
	case E_Bag_NotEnoughSpace:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PickGroundItemErr1")]);
		break;
	case E_Mall_ID_Error:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("MallTrade23")]);
		break;
	case E_Trade_ItemNum_MaxHoldLimit:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("MallTrade67")]);
		break;
	case E_Mall_PersonalBuyLimit:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Mall_PersonalBuyLimit")]);
		break;
	case E_Mall_Item_NotEnough:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("MallTrade21")]);
		break;
	case E_BagYuanBao_NotEnough:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CurrencyError_NotEnoughYuanbao")]);
		break;
	case E_Mall_CreateItem_Failed:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("MallTrade59")]);
		break;
	}

	return 0;
}

DWORD MallFrame::OnConfirmShopingCart_Event(tagMsgBoxEvent* pGameEvent)
{
	if(pGameEvent->eResult == MBF_OK)
	{
		tagNC_MallShoppingCart cmd;
		DWORD dwTypeID[MAX_MALL_CART_NUM];		// 商城商品ID
		memset(dwTypeID, 0, sizeof(dwTypeID));
		INT16 n16BuyNum[MAX_MALL_CART_NUM];		// 购买个数
		memset(n16BuyNum, 0, sizeof(n16BuyNum));
		INT nIndexInServer[MAX_MALL_CART_NUM];	// 在服务器中的位置	
		memset(nIndexInServer, 0, sizeof(nIndexInServer));

		int index = 0;
		for(int i = 0; i < MAX_MALL_CART_NUM; i++)
		{
			if(m_pShopingItem[i] != NULL)
			{
				cmd.dwTypeID[index] = m_pShopingItem[i]->dwID;
				cmd.n16BuyNum[index] = m_nShopingIetmNum[i];
				cmd.nIndexInServer[index] = m_pShopingItem[i]->nIndexInServer;
				index++;
			}
		}
		m_pSession->Send(&cmd);
	}

	return 0;
}