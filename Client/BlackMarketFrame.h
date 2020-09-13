#pragma once
#include "gameframe.h"
#include "..\WorldDefine\godbid_define.h"

struct tagNS_BlackMarketShopOpen;
struct tagNS_BlackConPrice;
class IconStatic;

enum ECurrentBidMode
{
	ECBM_Gold,			// 金币竞价
	ECBM_Shengbi		// 圣币竞价
};

class BlackMarketFrame :
	public GameFrame
{
public:
	
	BlackMarketFrame(void);
	~BlackMarketFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();

	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }
	VOID Update();
	VOID ComformPrice();
	VOID RefreshMoney();
	VOID RefreshUI();
	VOID RsfreshPicBack(BYTE byQuality);

	DWORD GetCurStartPrice();

	VOID ClearMoney();
private:
	DWORD OnNS_BlackMarketShopOpen(tagNS_BlackMarketShopOpen* pNetCmd,DWORD);
	DWORD OnNS_BlackConPrice(tagNS_BlackConPrice* pNetCmd,DWORD);
	BOOL EventHandler(tagGUIEvent* pEvent);
private:
	TSFPTrunk<BlackMarketFrame>		m_Trunk;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<GUISystem>						m_pGUI;

	DWORD												m_dwNPCID;				// NPC的ID
	DWORD												m_escapeTime;
	DWORD												m_dwLeftTime;			// 剩余时间

	DWORD												m_nFinalPrice;				// 最终出价
	DWORD												m_nAddPrice;				// 加价
	DWORD												m_dwCurMinOffer;		// 当前的最低出价
	int														m_nRefreshLimitTime;// 刷新按钮间隔时间计时器

	tagBlackMarketItemInfo						m_tagGoldItem;			// 金币拍卖物品
	tagBlackMarketItemInfo						m_tagShengbiItem;		// 圣币拍卖物品
	
	ECurrentBidMode								m_eMode;					// 竞价模式
	DWORD												m_nPreparePrice;		// 准备出价

	GUIWnd*												m_pWnd;
	GUIPushButton*									m_pPbGold;				// 游戏币
	GUIPushButton*									m_pPbShengbi;			// 圣币

	IconStatic*	m_pIcnItemPic;						// 物品图
	IconStatic*	m_pIcnItemPicBack;						// 物品图底图
	IconStatic*	m_pIcnGoldPic1;
	IconStatic*	m_pIcnGoldPic2;
	IconStatic*	m_pIcnGoldPic3;
	GUIStaticEx*	m_pStItemName;					// 物品名称
	GUIStatic*	m_pStCurPrice;						// 原始底价
	GUIStatic*	m_pStMinOfferPrice;				// 最低出价

	GUIStaticEx*	m_pStFinalPrice;						// 最终出价

	GUIStatic*	m_pStMoneyNotEnoughPic;	// 资金不足界面

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
