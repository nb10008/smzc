#pragma once
struct tagNS_RoleAttChangeSingle;
struct tagNS_GetRoleInitState_Att;
struct tagNS_SpiritReward;
struct tagNS_StrengthReward;
struct tagNS_GetStrengthDetail;
class MallButton;
class ActiveStatic;
class SpiritFrame: public GameFrame
{
public:
    SpiritFrame();
    ~SpiritFrame();
    static BOOL Enabled();
    virtual VOID Update();
    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual BOOL EscCancel();
private:
    void ClearShoppingSelection();
    static const TCHAR* ConvertErrorCode(DWORD errorcode);
    DWORD NetSpiritReward(tagNS_SpiritReward* pNetCmd, DWORD);
    DWORD NetStrengthReward(tagNS_StrengthReward *pNetCmd, DWORD);
    DWORD NetInitLocalPlayerAtt(tagNS_GetRoleInitState_Att* pNetCmd, DWORD);
    DWORD NetRoleAttChangeSingle(tagNS_RoleAttChangeSingle* pNetCmd, DWORD);
    DWORD NetGetStrengthDetail(tagNS_GetStrengthDetail* pNetCmd, DWORD);
    BOOL GUIEventHandler(tagGUIEvent* pEvent);
    BOOL DescriptionEventHandler(tagGUIEvent* pEvent);
    BOOL ExchangeEventHandler(tagGUIEvent* pEvent);
    BOOL ShoppingEventHandler(tagGUIEvent* pEvent);
    BOOL RewardEventHandler(tagGUIEvent* pEvent);
    DWORD OnExchangeInputEventHandler(tagMsgInputBoxEvent* pGameEvent);
    VOID UpdateSpirit(INT spirit);
    VOID UpdateTips();

    TSFPTrunk<SpiritFrame>	m_Trunk;
    TObjRef<GUISystem> m_pGUI;
    TObjRef<NetCmdMgr> m_pCmdMgr;
    TObjRef<NetSession> m_pSession;

    GUIWnd* m_pSpiritControl;
    GUIWnd* m_pSpiritExchange;
    GUIWnd* m_pSpiritReward;
    GUIWnd* m_pSpiritShopping;
    GUIWnd* m_pSpiritDescription;

    GUIButton* m_pBtnSpirit;


    GUIButton* m_pBtnSpiritDescOK;
    GUIButton* m_pBtnSpiritDescCancel;

    GUIButton* m_pBtnSpiritExchangeBuy;
    GUIButton* m_pBtnSpiritExchangeClose;
    GUIButton* m_pBtnSpiritExchangeCancel;

    GUIButton* m_pBtnSpiritShoppingBuy;
    GUIButton* m_pBtnSpiritShoppingClose;
    GUIButton* m_pBtnSpiritShoppingCancel;

    GUIButton* m_pBtnSpiritRewardSure;
    GUIButton* m_pBtnSpiritRewardClose;
    GUIButton* m_pBtnSpiritRewardCancel;

    MallButton* mb[6];
    MallButton* mbExchange[2];
    ActiveStatic* m_pASpiritBackground;
    INT m_spirit;
    INT m_constrength;
    INT m_curselectedexchgitem;
    DWORD m_curbuyitem;
    INT m_exchangedprice[2];
    vector<INT> m_pSpriteLevels;
    INT m_SpiritRewardVal;
    clock_t m_RewardCountDown;
};