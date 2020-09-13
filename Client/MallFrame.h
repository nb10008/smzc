#pragma once
#include "..\WorldDefine\mall_define.h"
#include "..\WorldDefine\msg_mall.h"

/** class MallFrame
	brief 商城
*/
class NetSession;
class IconStatic;
class PlayerTryDressWnd;
class ItemButton;
class MallButton;
class GlintPushButton;

struct tagMsgMallGiftEvent;
struct tagRoleYuanbaoChange;
struct tagRoleSilverChange;
struct tagExchangeVolumeChange;
struct tagMarkChange;
class HttpPicEx;
#define MALL_MAXITEM_PERPAGE 10 //18
#define MALL_ITEM_STATE 4//5		//商品状态
#define MALL_PROMOTE_ITEM_STATE 2		//推荐商品状态
#define MALL_MAX_PROMOTE_ITEM 5
#define MALL_MAX_PROMOTE_ITEM_PerPage 5
#define MALL_ITEM_SUBTYPE	8
#define MALL_ITEM_SECOND_SUBTYPE 5
#define SPECIAL_TYPE 2
#define MALL_MAXPACK_PERPAGE 5 //7
#define MALL_PACKPRO_NUM 5			//礼包打包物品数
#define MALL_PACKPRO_STATE 4		//礼包状态
#define MALL_DAY_PART 6
#define Mall_ShopingCart_Num 6		//购物车内物品数量
#define Mall_ShopingCart_RewardNum 4 //奖励物品数量

struct tagUpdateMallDataEvent : public tagGameEvent
{

	tagUpdateMallDataEvent():tagGameEvent(_T("tagUpdateMallDataEvent"),NULL){}
	tagUpdateMallDataEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame){}
};

class MallFrame : public GameFrame
{
public:
	enum EMallItemType 
	{
		EMIT_NULL		= -3,	//空
		EMIT_All		= -2,
		EMIT_Package	= -1,
		EMIT_0			=  0, //第一个一级菜单
		EMIT_1			=  1,
		EMIT_2			=  2,
		EMIT_3			=  3,
		EMIT_4			=  4,
		EMIT_5			=  5,
		EMIT_Volume		=  6,
		EMIT_Mark		=  7,
		EMIT_END		=  8,  //结束

		EMIT_GiftPlan		=  10, //1原计划
	};

	enum EMallItemStateType			//商品状态图标
	{
		EMIST_New		= 0,		//新品
		EMIST_Hot		= 1,		//热销
		EMIST_Promote	= 2,		//促销
		EMIST_Num		= 3,		//限量
	  //EMIST_Present	= 5,		//赠送
	  //EMIST_Exchange	= 6,		//兑换
	  //EMIST_GroupBuy	= 7,		//团购
	};

	enum EMallIPromoteItemStateType	 //推荐商品状态图标
	{
		EMPIST_New		= 0,		//新品
		EMPIST_Num		= 1,		//限量
	};

	enum EMamllItemTipsType
	{
		EMITT_All		= 0,
		EMITT_Package,
		EMITT_Promote,
	};

	enum EMallItemOperateState
	{
		EMIOS_NULL				= 0,
		EMIOS_NormalItem,
		EMIOS_PackItem,
		EMIOS_Promote,
		EMIOS_Free,
	};

public:
	MallFrame(void);
	~MallFrame(void);

    tstring GetGUIFullName()
    {
        if(P_VALID(m_pWnd))
            return m_pWnd->GetFullName();
        else 
            return _T("");
    };

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Render3D();
	virtual VOID Update();
	virtual BOOL EscCancel();

	// 非打包商品归类
	void ClassifiedMallItem();

	void ShowWnd(BOOL bVal);

	//更新avatar
	void UpdateAvatar();

	//获得先择商品
	DWORD GetMallItemHopeTypeID() { return m_dwSelectItemID; }
	EMallItemOperateState GetOpereateState() { return m_eOperateState; }

public:
	//设置要购买的商品ID
	void SetHopeBuyItemID(DWORD dwID) 
	{ 
		m_dwSelectItemID = dwID; 
	}

	//设置当前选中类型,供外部调用（如死亡界面，便捷购买）
	void SetCurMenu( EMallItemType eMallItemType );

	//购买商品
	void BuyMallItem();

	//1元计划 闪烁设置
	VOID SetGiftPlan();
private:
	// 处理GUI消息
	DWORD EventHandler(tagGUIEvent* pEvent);

	//刷新商品选择框
	void RefreshMallItemFlag();

	//刷新推荐商品
	void RefreshPromoteItemState();

	//显示推荐商品
	VOID ShowPromoteMallItem();

	//刷新免费领取商品
	void RefreshFreeItem();

	//显示当前页的商品(按类别)
	void ShowCurPageMallItemByType(INT nPage);

	//显示当前页的商品(全部)
	void ShowCurPageMallItemByAll(INT nPage);

	//显示当前页的礼包
	void ShowCurPageMallPack(INT nPage);

	//判断显示商品
	void JudgeShowMallItem(INT nMenu, INT nSubMenu);

	//设置二级分类按钮显示
	void SetSubMenuText(INT nMenu);

	//获得当前礼包ID，可用来索引礼包
	DWORD GetCurPackID(INT index);
	
	//--翻页
	void PageUp();
	void PageDown();

	//显示当前页码
	void SetCurrentPage();

	// 设置金钱
	void SetMoney( INT64 n64Value );
	//设置元宝
	void SetYuanBao();
	void SetExchangeVolume();
	void SetMark();

	//兑换商品
	void ExchangeMallItem();

	void BuyMallItemPromote();

	//领取免费商品
	void FreeGetMallItem();

	//隐藏没有物品的普通商品控件
	void HideCtrlMallItem();
	void HideCtrlPromoteItem();
	void HideCtrlMallPack();
	int TipsPos2State( const tagMallItemProtoBase* pItem,  int nStatePos );
	//--处理游戏事件
	DWORD OnBuyMallItemInputEvent(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnBuyMallPromoteItemInputEvet(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnAffirmBuyMallItemEvent(tagMsgBoxEvent* pGameEvent);
	DWORD OnMsgMallGiftEvent(tagMsgMallGiftEvent* pGameEvent);
	DWORD OnBagYuanbaoChange(tagRoleYuanbaoChange* pGameEvent);
	DWORD OnBagMoneyChange(tagRoleSilverChange* pGameEvent);
	DWORD OnExchangeVolumeChange(tagExchangeVolumeChange* pGameEvent);
	DWORD OnMarkChange(tagMarkChange* pGameEvent);
	DWORD OnExchangePackItem(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnExchangeMallItem(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnAffirmExchangePackItem(tagMsgBoxEvent* pGameEvent);
	DWORD OnAffirmExchangeMallItem(tagMsgBoxEvent* pGameEvent);
	DWORD OnNS_MallUpdate(tagUpdateMallDataEvent* pGameEvent);
	DWORD OnAddToShopingCart(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnConfirmShopingCart_Event(tagMsgBoxEvent* pGameEvent);
	//--处理网络消息

	//--发送网络消息
	VOID OnSendBuyMallItem(const tagMallItemProto* pItem, INT16 n16Num); 
	VOID OnSendBuyMallPack(const tagMallPackProto* pPack, INT16 n16Num);

	//--处理网络消息
	DWORD OnNS_MallBuyItem(tagNS_MallBuyItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallFreeGetItem(tagNS_MallFreeGetItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallPresentItem(tagNS_MallPresentItem* pMsg, DWORD dwParam);
	DWORD OnNS_MallBuyPack(tagNS_MallBuyPack* pMsg, DWORD dwParam);
	DWORD OnNS_MallPresentPack(tagNS_MallPresentPack* pMsg, DWORD dwParam);
	DWORD OnNS_MallItemExchange(tagNS_MallItemExchange* pMsg, DWORD dwParam);
	DWORD OnNS_MallPackExchange(tagNS_MallPackExchange* pMsg, DWORD dwParam);

	DWORD OnNS_MallUpdateCfg(tagNS_MallUpdateCfg* pMsg, DWORD dwParam);
	DWORD OnNS_MallShoppingCart(tagNS_MallShoppingCart* pmsg, DWORD dwParam);
	
	//--显示物品Tips
	void ShowMallGoodsTips(int nGoodsPos, int nStatePos);
	void ShowMallPromoteGoodsTips(int nGoodsPos, int nStatePos);
	void ShowMallPackTips(int nGoodsPos, int nStatePos);

	//--各属性Tips
	void ShowPromoteProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowGroupBuyProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowPresentProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowNumProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);
	void ShowExchangeProTips(const tagMallItemProtoBase* pItem, int nGoodsPos, int nStatePos, EMamllItemTipsType eType=EMITT_All);

	//--各属性判断
	bool JudgePromotePro(const tagMallItemProtoBase* pItem);
	bool JudgeGroupBuyPro(const tagMallItemProtoBase* pItem);
	bool JudgePresentPro(const tagMallItemProtoBase* pItem);
	bool JudgeNumPro(const tagMallItemProtoBase* pItem);
	bool JudgeExchangePro(const tagMallItemProtoBase* pItem);

	// 试穿
	void JudgeTryDress(DWORD dwTypeID);

	// 设置赠送最大个数
	void SetPresentMaxNum();

	void SetOperateState(EMallItemOperateState eType=EMIOS_NULL);	
	
	//直充
	void YuanBaoAbundance();

	//公告区
	void ShowMallNotice(BOOL bVal);

	// 试穿时装颜色
	int HandleFation( EAvatarElement eAvat, bool bCanDye, DWORD dwTypeID );

	// 一级分页的按钮状态
	VOID PopUpAll();
	//-------------------------------------------------------------------------------------------
	//元宝交易
	//-------------------------------------------------------------------------------------------

	//刷新购物车物品
	VOID RefreshShopingCart();
	//刷新奖励物品
	VOID RefreshShopingReward();
	//显示隐藏购物车物品
	VOID HideShopingCart(bool bHide = TRUE);
	//显示隐藏奖励物品
	VOID HideShopingReward(bool bHide = TRUE);
	//显示隐藏加入购物车按钮
	VOID HideAddToShopingCart(bool bHide = TRUE);
	//添加到购物车
	VOID AddToShopingCart(int nNum);
	//清空购物车
	VOID ClearShopingCart();
private:

	TSFPTrunk<MallFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;		
	PlayerTryDressWnd*			m_pWndTryDress;							// 试穿窗口
	GUIStatic*					m_pStcGold;								// 金币
	GUIStatic*					m_pStcSilver;							// 银币
	GUIStatic*					m_pStcYuanBao;							// 元宝
	GUIStatic*					m_pExchangeVolume;						// 兑换卷 
	GUIStatic*					m_pStcMark;								// 积分
	GUIButton*					m_pBtnClose;							// 关闭
	GUIPushButton*				m_pPBtnYuanBaoTrade;					// 元宝交易
	//GUIPushButton*				m_pPBtnGroupBuy;						// 帮派团购
	//GUIPushButton*				m_pPBtnNotice;							// 商城公告
	GUIButton*					m_pBtnYuanBaoAbundance;					// 元宝充值
	GUIButton*					m_pBtnReset;							// 试穿重置

	IconStatic*					m_pStcFetchBack;						// 免费领取背景
	IconStatic*					m_pStcFetchSel;							// 免费领取选择框
	ItemButton*					m_pFetchUnitItem;						// 免费领取物品
	GUIButton*					m_pBtnFetch;							// 免费领取按钮	
	GUIStatic*					m_pBtnFetchTitle;						// 免费领取物品名

	GUIStatic*					m_pStcGoodsBack;						// 商器区背景
	GUIStatic*					m_pStcGoodsBackEx;						// 普通商品
	IconStatic*					m_pUnitItemBack[MALL_MAXITEM_PERPAGE];	// 商品区
	IconStatic*					m_pUnitItemSel[MALL_MAXITEM_PERPAGE];	// 商品选择框
	MallButton*					m_pUnitItem[MALL_MAXITEM_PERPAGE];		// 商品
	GUIStatic*					m_pStcItemName[MALL_MAXITEM_PERPAGE];	// 商品名字
	GUIStatic*					m_pStcInfo[MALL_MAXITEM_PERPAGE];		// 商品信息

	GUIStatic*					m_pStcGoodsRest[MALL_MAXITEM_PERPAGE];   // 商品剩余数量
	GUIStatic*					m_pStcGoodsInfo[MALL_MAXITEM_PERPAGE];	 // 商品原价字样
	IconStatic*					m_pStcGoodsPrice[MALL_MAXITEM_PERPAGE];  // 商品原价值显示
	IconStatic*					m_pStcGoodsPriceDel[MALL_MAXITEM_PERPAGE];	// 商品原价值显示删除号
	GUIStatic*					m_pStcGoodsInfoNow[MALL_MAXITEM_PERPAGE];	// 商品现价字样
	GUIStatic*					m_pStcGoodsPriceNow[MALL_MAXITEM_PERPAGE];  // 商品现价显示
	GUIStatic*					m_pStcGoodsGift[MALL_MAXITEM_PERPAGE];	   // 商品赠品字样
	GUIStatic*					m_pStcGoodsGiftNum[MALL_MAXITEM_PERPAGE];  // 商品赠品数量显示

	IconStatic*					m_pStcState[MALL_MAXITEM_PERPAGE][MALL_ITEM_STATE];// 商品状态
	GUIStatic*					m_pStcStateInv[MALL_MAXITEM_PERPAGE][MALL_ITEM_STATE];// 商品状态底图
	GUIButton*					m_pBtnPDItem;							// 下一页(普通商品)
	GUIButton*					m_pBtnPUItem;							// 上一页(普通商品)
	GUIStatic*					m_pStcCPItem;							// 当前页提示(普通商品)
	GUIButton*					m_pBtnPDPack;							// 下一页(礼包商品)
	GUIButton*					m_pBtnPUPack;							// 上一页(礼包商品)
	GUIStatic*					m_pBtnCPPack;							// 当前页提示(礼包商品)

	GUIButton*					m_pBtnBuy;								// 购买
	GUIButton*					m_pBtnPresent;							// 赠送
	//GUIButton*					m_pBtnGroupBuy;							// 团购
	//GUIButton*					m_pBtnExchange;							// 兑换

	GUIStatic*					m_pStcProBack;							// 推荐商品背景

	GUIStatic*					m_pStcProRest[MALL_MAX_PROMOTE_ITEM_PerPage];   // 推荐商品剩余数量
	GUIStatic*					m_pStcProInfo[MALL_MAX_PROMOTE_ITEM_PerPage];	 // 推荐商品原价字样
	IconStatic*					m_pStcProPrice[MALL_MAX_PROMOTE_ITEM_PerPage];  // 推荐商品原价值显示
	IconStatic*					m_pStcProPriceDel[MALL_MAX_PROMOTE_ITEM_PerPage];		// 推荐商品原价值显示删除号
	GUIStatic*					m_pStcProInfoNow[MALL_MAX_PROMOTE_ITEM_PerPage];	// 推荐商品现价字样
	GUIStatic*					m_pStcProPriceNow[MALL_MAX_PROMOTE_ITEM_PerPage];  // 推荐商品现价显示
	GUIStatic*					m_pStcProGift[MALL_MAX_PROMOTE_ITEM_PerPage];	   // 推荐商品赠品字样
	GUIStatic*					m_pStcProGiftNum[MALL_MAX_PROMOTE_ITEM_PerPage];  // 推荐商品赠品数量显示

	GUIStatic*					m_pStcProPageText;						// 推荐商品页数显示	
	GUIButton*					m_pBtnFormerPage;						// 推荐商品向前换页
	GUIButton*					m_pBtnLatterPage;						// 推荐商品向后换页
	IconStatic*					m_pProItemBack[MALL_MAX_PROMOTE_ITEM_PerPage];
	IconStatic*					m_pUnitProItemSel[MALL_MAX_PROMOTE_ITEM_PerPage];
	MallButton*					m_pUnitProItem[MALL_MAX_PROMOTE_ITEM_PerPage];
	IconStatic*					m_pProItemState[MALL_MAX_PROMOTE_ITEM_PerPage][MALL_PROMOTE_ITEM_STATE];// 推荐商品状态
	GUIStatic*					m_pProItemStateInv[MALL_MAX_PROMOTE_ITEM_PerPage][MALL_PROMOTE_ITEM_STATE];// 推荐商品状态前景
	GUIStatic*					m_pProItemName[MALL_MAX_PROMOTE_ITEM_PerPage]; 
	//GUIStatic*					m_pProItmeInfo[MALL_MAX_PROMOTE_ITEM];

	GUIStatic*					m_pStcPackBack;							//礼包背景

	IconStatic*					m_pUnitPackBack[MALL_MAXPACK_PERPAGE];	//每个礼包
	GUIButton*					m_pBtnPackBack[MALL_MAXPACK_PERPAGE];	//礼包背景button
	IconStatic*					m_pUnitPackSel[MALL_MAXPACK_PERPAGE][MALL_PACKPRO_NUM];	//礼包选择框
	MallButton*					m_pUnitPack[MALL_MAXPACK_PERPAGE][MALL_PACKPRO_NUM];	//礼包里的商品
	IconStatic*					m_pStcPackName[MALL_MAXPACK_PERPAGE];	// 礼包名字

	GUIStatic*					m_pStcPackRest[MALL_MAXPACK_PERPAGE];  // 礼包剩余数量
	GUIStatic*					m_pStcPackInfo[MALL_MAXPACK_PERPAGE];	// 礼包原价字样
	IconStatic*					m_pStcPackPrice[MALL_MAXPACK_PERPAGE];  // 礼包原价值显示
	IconStatic*					m_pStcPackPriceDel[MALL_MAXPACK_PERPAGE];	// 礼包原价值显示删除号
	GUIStatic*					m_pStcPackInfoNow[MALL_MAXPACK_PERPAGE];	// 礼包现价字样
	GUIStatic*					m_pStcPackPriceNow[MALL_MAXPACK_PERPAGE];  // 礼包现价显示
	GUIStatic*					m_pStcPackGift[MALL_MAXPACK_PERPAGE];	  // 礼包赠品字样
	GUIStatic*					m_pStcPackGiftNum[MALL_MAXPACK_PERPAGE];  // 礼包赠品数量显示

	IconStatic*					m_pStcPackState[MALL_MAXPACK_PERPAGE][MALL_PACKPRO_STATE];// 礼包状态
	GUIStatic*					m_pStcPackStateInv[MALL_MAXPACK_PERPAGE][MALL_PACKPRO_STATE];// 礼包状态前景
	GUIStatic*					m_pStcPackNew[MALL_MAXPACK_PERPAGE];	// 新礼包新提示
	GUIStatic*					m_pStcPackHot[MALL_MAXPACK_PERPAGE];	// 新礼包热提示

	// 时装预览染色
	GUIComboBox*				m_pCBDayFation[MALL_DAY_PART];
	GUIStatic*					m_pStcDyeTips[MALL_DAY_PART];

	GUIPushButton*				m_pPBtnAll;								// 全部商品按钮
	GUIPushButton*				m_pPBtnPackage;							// 打包商品按钮
	GUIPushButton*				m_pPBtnSubMall[MALL_ITEM_SUBTYPE-2];		// 商品分类按钮 一级分类
	GUIPushButton*				m_pPBtnPresent;							// 赠卷分类按钮
	GUIPushButton*				m_pPBtnMark;							// 积分分类按钮
	GUIPushButton*				m_pPBtnMallType2[MALL_ITEM_SECOND_SUBTYPE];	// 商品二级分类
	GUIPushButton*				m_pPBtnMall;							// 刷新商品区按钮

	//GUIStatic*					m_pWndNotice;							// 公告
	//HttpPicEx*					m_pHttp_MallNotice;				

	GUIWnd*						m_pWndShowBox;




	TMap<DWORD, tagMallItemProto*>	m_mapMallItem[MALL_ITEM_SUBTYPE][MALL_ITEM_SECOND_SUBTYPE];	// 商品分类列表
	map<pair<DWORD, DWORD>, int>	m_map1and2toBtIndex;	// 通过一级分类和二级分类取按钮编号
	map<pair<DWORD, int>, DWORD>	m_map1andBtIndexTo2;	// 通过一级分类和按钮编号取二级分类

	INT							m_nCurMenu;								// 当前商品分类货架:0-7 全部:-2 打包:-1
	INT							m_nCurSubMenu;							// 当前商品二级货架分类:0-4
	INT							m_nCurPage[MALL_ITEM_SUBTYPE][MALL_ITEM_SECOND_SUBTYPE];			// 当前页
	INT							m_nCurMaxPage[MALL_ITEM_SUBTYPE][MALL_ITEM_SECOND_SUBTYPE];		// 当前货架最大页

	INT							m_nSpecialPage[SPECIAL_TYPE];			// 全部:0 打包:1
	INT							m_nSpecialMaxPage[SPECIAL_TYPE];

	DWORD						m_dwSelectItemID;						// 要购买的商品ID
	INT16						m_n16BuyNum;							// 购买的数量
	vector<tagMsgMallGiftEvent>	m_vecGiftEvent;							// 缓存赠送
	bool						m_bSelectPromote;
	EMallItemOperateState		m_eOperateState;
	tstring						m_strMallNotice;						// 缓存的商城公告

	//-------------------------------------------------------------------------------------------
	//元宝交易
	//-------------------------------------------------------------------------------------------
    GUIStatic*					m_pStcYuanbaoBack;						// 元宝交易背景

	//-------------------------------------------------------------------------------------------
	//团购
	//-------------------------------------------------------------------------------------------
	//GUIStatic*					m_pStcGroupBuyBack;						// 团购背景

	DWORD						m_dwTryDressID[MALL_DAY_PART];			// 试穿时装ID

	INT							m_nPolicy;//登陆模式

	INT							m_iPromoteItemShowPage;					//推荐商品在第几页

	EShoppingDistrict			m_eShopingDistrict;        // 购物区　

	GUIPushButton				*m_pPBLevelLimit;			//开启等级限制
	BOOL						m_bLevelLimit;
	EMallItemType				m_eClickType;				//点击菜单类型

	INT8						m_n8SecondMenu;				//记录二级分页页码
	
	//是否为外部调用（如死亡界面，便捷购买）
	BOOL						m_bIsExterior;  

	//1元计划
	GlintPushButton*			m_pGPBLevel[LEVEL_NUM_CHOOSE];


	//购物车
	GUIButton*					m_pBtnShopingCart[MALL_MAXITEM_PERPAGE];					//购物车按钮
	IconStatic*					m_pIStcShopingCartItem[Mall_ShopingCart_Num];				//购物车内物品
	IconStatic*					m_pIStcShopingCartReward[Mall_ShopingCart_RewardNum];		//奖励物品
	GUIButton*					m_pBtnCheckout;												//结账按钮
	GUIButton*					m_pBtnClearSHopingCart;										//清空购物车按钮
	GUIStatic*					m_pStcYuanBaoCast;											//元宝消耗数
	GUIStatic*					m_pStcRewardNum;											//奖励次数（胜币总价值/获取奖励的最低胜币数）
	GUIButton*					m_pBtnItemAdd[Mall_ShopingCart_Num];
	GUIButton*					m_pBtnItemRed[Mall_ShopingCart_Num];
	GUIStaticEx*				m_pStcShopingTips;
	GUIWnd*						m_pWndShopingText;											//购物车那几个字，如果开关没开药隐藏掉
	GUIWnd*						m_pWndShopingCatrWnd;										//如果开关没开要隐藏掉

	const tagMallItemProto*		m_pShopingItem[Mall_ShopingCart_Num];						//购物车内的商城物品
	INT							m_nShopingIetmNum[Mall_ShopingCart_Num];					//商品数量
	DWORD						m_dwAddItemId;
	DWORD						m_dwMallCfg[EMCA_End];
};