#pragma once 

class ItemButton;
class Equipment;
class Item;
struct tagNS_FabaoWuxingReversal;
struct tagNS_FabaoWuxingMerging;
struct tagItemPosChangeExEvent;

struct tagNS_ItemRemove;

const tstring TRUMP_DIBAN      =   _T("data\\ui\\sys\\l_diban-a.bmp");

class TrumpRebornFrame : public GameFrame
{
public:
	TrumpRebornFrame();
	~TrumpRebornFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
public:
	VOID ShowUI( bool bFlag );
	VOID UpdateUI();
	VOID ClearInfo();
	VOID PutAssisTrumpLeft(INT nPos);
	//VOID PutAssisTrumpRight(INT nPos);

private:
	DWORD EventHandler( tagGUIEvent *pEvent );
	DWORD NS_FabaoWuxingReversal(tagNS_FabaoWuxingReversal *pNetCmd,DWORD);
	DWORD NS_FabaoWuxingMerging(tagNS_FabaoWuxingMerging *pNetCmd,DWORD);
	BOOL CheckMainTrumpLevel();
	DWORD OnEventItemEvent_Move2Reborn(tagItemPosChangeExEvent* pGameEvent);
	DWORD UseTrumpReversalCheck(tagMsgBoxEvent* pGameEvent);
	//DWORD UseTrumpMergingCheck(tagMsgBoxEvent* pGameEvent);
	DWORD NetItemRemove(tagNS_ItemRemove *pNetCmd,DWORD);

private:
	TSFPTrunk<TrumpRebornFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;

	GUIWnd *m_pWnd;
	GUIWnd *m_pWndCaption;

	ItemButton *m_pMainReverseTrump;
	GUIStatic *m_pMainReverseTrumpName;
	GUIStatic *m_pMainReverseTrumpLevel;
	ItemButton *m_pAssisReverseTrump;
	GUIStatic *m_pStcLikely;
	GUIButton *m_pBtnReverseOK;
	GUIStaticEx *m_pStcReverseRule;

	//ItemButton *m_pMainGrownTrump;
	//GUIStatic *m_pMainGrownTrumpName;
	//GUIStatic *m_pMainGrownTrumpLevel;
	//ItemButton *m_pAssisGrownTrump;
	//GUIButton *m_pBtnGrownOK;
	//GUIStaticEx *m_pStcGrownRule;

	GUIButton *m_pButCancle;
	GUIButton *m_pButExit;

	Equipment *m_pTrump;	// 法宝

	Equipment *m_pItem_Left;	// 左副法宝
	//Item *m_pItem_Right;// 右副法宝

	GUIStatic *m_pAssisReverseTrumpName;
	GUIStatic *m_pAssisReverseTrumpLevel;

	//GUIStatic *m_pAssisGrownTrumpName;
	//GUIStatic *m_pAssisGrownTrumpLevel;
};