#pragma once

#define FASHION_SUIT_NUM_OF_ONE_PAGE 4
enum EFashion
{
	EF_NULL = -1,
	EF_Hat,
	EF_Clothes,
	EF_Glove,
	EF_Boot,
	EF_Back,
	EF_End,
};
class PicStatic;
class ItemButton;
class PlayerTryDressWnd;
struct tagItemPosChangeExEvent;
struct tagNS_WardrobeLevelUp;
struct tagUpdateLPItemEvent;

class ClothespressFrame : public GameFrame
{
public:
	ClothespressFrame(void);
	~ClothespressFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	virtual VOID Render3D();
	VOID ShowUI();

private:
	VOID PutItem(INT16 nPos, int nCol, bool bDiy = false, int nRow = 0);
	BOOL EventHandler( tagGUIEvent *pEvent );
	DWORD OnEventHideUI(tagGameEvent* pGameEvent);
	EEquipPos Change2EquipPos(int nIndex);
	void UpdateAvatar();
	void JudgeTryDress( INT64 n64ID );
	void InitBackPic();
	VOID UpdateCurPage();
	VOID UpdateDIYFashion();
	DWORD GetCurCollectFashionNum();
	VOID RefreshCollectNum();
	VOID GetCurSuitID(INT nIndex);
	DWORD OnEventUpdateFashionInfo(tagGameEvent* pEvent);
	DWORD OnNS_WardrobeLevelUp( tagNS_WardrobeLevelUp* pNetCmd, DWORD dwParam );
	// 自己更换衣橱中的时装
	DWORD OnNS_NetRehanding( tagNS_Rehanding* pNetCmd, DWORD dwParam );
	//--处理物品刷新事件
	DWORD OnUpdateItemEvent(tagUpdateLPItemEvent* pGameEvent);
	VOID TryDress(BOOL bDIY = FALSE);
	BOOL IsDiySuitIsComplete();
	BOOL ISSuitComplete(DWORD dwSuitID);
	DWORD OnEventTransform(tagGameEvent* pEvent);

	DWORD OnOffFashionClothes(tagGameEvent* pEvent);
	DWORD GetTotalAttrativeValue();
private:
	TSFPTrunk<ClothespressFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<Util> m_pUtil;
	TObjRef<NetSession> m_pSession;

	GUIWnd *m_pWnd;
	GUIPatch *m_pWndBack;

	GUIPushButton *m_pBtnMale;
	GUIPushButton *m_pBtnFemale;

	PicStatic *m_pIconSuit[FASHION_SUIT_NUM_OF_ONE_PAGE];

	PicStatic *m_pStcTitle[FASHION_SUIT_NUM_OF_ONE_PAGE];
	PicStatic *m_pStcClothesexpressLevel[FASHION_SUIT_NUM_OF_ONE_PAGE];
	PicStatic *m_pStcSuitName[FASHION_SUIT_NUM_OF_ONE_PAGE];

	GUIStatic *m_pStcWear[FASHION_SUIT_NUM_OF_ONE_PAGE];
	GUIStatic *m_pStcFriend[FASHION_SUIT_NUM_OF_ONE_PAGE];

	GUIStaticEx *m_pStcAttrativeValue[FASHION_SUIT_NUM_OF_ONE_PAGE];
	GUIStaticEx *m_pStcSuitTips[FASHION_SUIT_NUM_OF_ONE_PAGE];
	GUIStaticEx *m_pStcSuitState[FASHION_SUIT_NUM_OF_ONE_PAGE];
	ItemButton *m_pFashion[FASHION_SUIT_NUM_OF_ONE_PAGE][EF_End];

	GUIStatic *m_pStcCurPage;
	GUIButton *m_pBtnNext;
	GUIButton *m_pBtnPre;

	PicStatic *m_pIconDiySuit;
	PicStatic *m_pStcDiyTitle;
	PicStatic *m_pStcDiyClothesexpressLevel;
	PicStatic *m_pStcDiySuitName;
	GUIStatic *m_pStcDiyWear;
	GUIStaticEx *m_pStcDiyAttrativeValue;
	GUIStaticEx *m_pStcDiySuitTips;
	GUIStaticEx *m_pStcDiySuitState;
	ItemButton *m_pDiyFashion[EF_End];

	PlayerTryDressWnd *m_pWndTryDress;
	GUIButton *m_pBtnReset;
	GUIStatic *m_pStcCurSuitNum;
	GUIStatic *m_pStcCollectRate;
	GUIButton *m_pBtnChangeSuit;
	GUIButton *m_pBtnUpgradeClothes;

	GUIButton *m_pBtnClose;

	GUIStaticEx *m_pStcAttrativeTotalValue;

	INT16 m_n16CurPage;
	DWORD m_dwCurSuitIndex;
	INT16 m_n16MaxMaleFashion;
	INT16 m_n16MaxFemalFashion;
};