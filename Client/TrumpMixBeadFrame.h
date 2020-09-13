#pragma once 
#include "..\WorldDefine\ItemDefine.h"

class Item;
class ItemButton;
class Equipment;
struct tagItemPosChangeExEvent;
struct tagNS_FabaoInlay;
struct tagNS_RolePutDownItem;
class TrumpMixBeadFrame : public GameFrame
{
public:
	TrumpMixBeadFrame();
	~TrumpMixBeadFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
public:
	VOID ShowUI( bool bFlag );

private:
	DWORD EventHandler( tagGUIEvent *pEvent );
	DWORD OnTrumpBind(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMixBead(tagItemPosChangeExEvent* pGameEvent);
	DWORD NetTrumpMixBead( tagNS_FabaoInlay *pNetCmd, DWORD );
	DWORD NetRolePutDownItem( tagNS_RolePutDownItem *pNetCmd, DWORD );
	VOID ResetUI();
	VOID LockItem( bool bOperable );
	VOID PutLingZhu(INT nPos);
	VOID UpdateUI();
	INT GetLingCaoNum();

private:
	TSFPTrunk<TrumpMixBeadFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<NetSession>	m_pSession;

	GUIWnd *m_pWnd;
	GUIWnd *m_pWndCaption;

	ItemButton *m_pIconTrump;
	GUIStatic *m_pStcTrumpName;
	GUIStatic *m_pStcTrumpLevel;
	ItemButton *m_pIconLingCao[EWX_End];
	ItemButton *m_pIconLingZhu;
	GUIProgress *m_pProTime;
	GUIButton *m_pButMix;
	GUIButton *m_pButCancle;
	GUIStaticEx *m_pStcMixRule;
	GUIButton *m_pBtnQuit;

	Equipment *m_pTrump;
	Item *m_pItem;
	DWORD m_dwTypeID;
	bool m_bProcessing;
	BYTE m_byMatchNum;
};