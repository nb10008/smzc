#pragma once
#include "gameframe.h"
#include "..\WorldDefine\godbid_define.h"
struct tagNS_GodBiderShopOpen;
struct tagNS_ConformPrice;

class ItemButton;
class IconStatic;
struct tagBidItem
{
	GUIStatic*	pIcnBack;					// 背景
	IconStatic*	pIcnItemPic;			// 物品图
	IconStatic*	pIcnItemPicBack;			// 物品图底图
	GUIStatic*	pIcnHighestPic;		// 最高出价
	GUIStatic*	pStUnOpen;				// 未开时
	GUIStatic*	pStOpen;					// 开始了
	GUIStatic*	pStItemName;			// 物品名称
	GUIStatic*	pStCurPrice;				// 当前价格
	GUIStatic*	pStMinOfferPrice;	// 最低出价
	GUIStatic*	pStChooseing;			// 选中框
};

class GodBidFrame :
	public GameFrame
{
public:
	GodBidFrame(void);
	~GodBidFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	
	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }
	VOID Update();
private:
	DWORD OnNS_GodBiderShopOpen(tagNS_GodBiderShopOpen* pNetCmd,DWORD);
	DWORD OnNS_ConformPrice(tagNS_ConformPrice* pNetCmd,DWORD);
	VOID ComformPrice();
	BOOL EventHandler(tagGUIEvent* pEvent);

	VOID RefreshUI(INT nRow);
	VOID RefreshMoney();
	VOID RefreshPicBack(IconStatic* pWnd,BYTE byQuality);

	VOID ClearMoney();
	DWORD GetCurrStartPrice();
	bool IsCurrItemPriceOffered();
private:
	TSFPTrunk<GodBidFrame>		m_Trunk;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;

	DWORD										m_dwNPCID;				// NPC的ID
	DWORD										m_escapeTime;

	map<int,tagGodBidItemInfo>	m_mapBidItemInfo;
	DWORD										m_dwLeftTime;			// 剩余时间
	int												m_nCurrRow;				// 第几行的商品
	DWORD										m_nFinalPrice;				// 最终出价（金 单位）
	DWORD										m_nAddPrice;				// 加价（金 单位）
	DWORD										m_dwCurPrice;				// 当前玩家已经出的价格（金 单位）
	DWORD										m_dwCurMinOffer;		// 当期的最低出价（金 单位）
	int												m_nRefreshLimitTime;// 刷新按钮间隔时间计时器

	tagBidItem									m_BidItems[MAX_GODBID_ITEM_NUM];
	GUIWnd*		m_pWnd;
	GUIStatic*	m_pStAlreadyOfferPrice;			//	已出价
	GUIStaticEx*	m_pStAddMoney;					// 本次增加价格
	GUIStaticEx*	m_pStFinalPrice;						// 最终出价

	GUIStatic*	m_pStMoneyNotEnoughPic;	// 资金不足界面
	GUIStatic*	m_pStHighestPricePic;			// 最高出价界面

	GUIStatic*	m_pStTime_Minite;					// 剩余时间 分
	GUIStatic*	m_pStTime_Second;				// 剩余时间 秒

	GUIButton*	m_pBnPriceAddx1;					// 1倍出价
	GUIButton*	m_pBnPriceAddx10;				// 10倍出价
	GUIButton*	m_pBnPriceAddx100;				// 100倍出价
	GUIButton*	m_pBnPriceAdd_All;				// 所有

	GUIButton*	m_pBnClearPrice;					// 清楚加价
	GUIButton*	m_pBnConformPrice;				// 确认出价
	GUIButton*	m_pBnRefresh;						// 刷新

	GUIButton*	m_pBnClose;
};
