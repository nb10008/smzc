#pragma once 

class Item;
class Equipment;
class ItemButton;
struct tagItemPosChangeExEvent;
struct tagNS_FabaoPractice;
struct tagNS_RolePutDownItem;
class RefineTrumpFrame : public GameFrame
{
public:
	RefineTrumpFrame();
	~RefineTrumpFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
public:
	VOID ShowUI( bool bFlag );

private:
	DWORD EventHandler( tagGUIEvent *pEvent );
	DWORD OnEventTrumpRefine(tagItemPosChangeExEvent* pGameEvent);
	DWORD NetTrumpRefine( tagNS_FabaoPractice *pNetCmd, DWORD );
	DWORD NetRolePutDownItem( tagNS_RolePutDownItem *pNetCmd, DWORD );
	VOID LockItem( bool bOperable );
	VOID SendTrumpRefineMsg();
	VOID PutStone( INT16 nPos );
	VOID RefreshStone();
	VOID ClearInfo();
	BOOL Check();
	VOID UpdateUI();
	DWORD TooManyYuLingShi( tagMsgBoxEvent *pGameEvent );

private:
	TSFPTrunk<RefineTrumpFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<NetSession>	m_pSession;

	GUIWnd *m_pWnd;
	GUIWnd *m_pWndCaption;

	ItemButton *m_pIconTrump;
	GUIStatic *m_pStcTrumpName;
	GUIStatic *m_pStcTrumpLevel;

	GUIProgress *m_pProExp;
	GUIStatic *m_pStcExpShow;
	ItemButton *m_pIconStone;

	GUIButton *m_pButNext;
	GUIButton *m_pButPre;
	GUIEditBox *m_pStcNum;
	
	GUIProgress *m_pProTime;
	GUIButton *m_pButRefine;
	GUIButton *m_pButCancle;
	GUIButton *m_pButQuit;

	bool m_bProcessing;	// 是否走条中
	INT	m_nNum;			// 御灵石消耗的个数
	INT m_nMaxNum;		// 御灵石最大个数
	INT m_nJie;			// 法宝的当前阶数
	Equipment *m_pTrump;	// 法宝
	Item *m_pItem;			// 御灵石
};