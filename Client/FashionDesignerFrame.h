#pragma once

class IconStatic;
struct tagItemPosChangeExEvent;
struct tagNS_AdvancedCompose;

class FashionDesignerFrame : public GameFrame
{
public:
	FashionDesignerFrame(void);
	~FashionDesignerFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	VOID SetNPCID(DWORD dwID, int npcType);

private:
	BOOL IfCanStart();
	VOID PutItem( INT16 nPos );
	BOOL EventHandler( tagGUIEvent *pEvent );
	VOID LockItem( INT64 nID, bool bOperable );
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	DWORD NetProduceFashion( tagNS_AdvancedCompose *pNetCmd, DWORD );
	VOID ClearAll();

private:
	TSFPTrunk<FashionDesignerFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<Util> m_pUtil;
	TObjRef<NetSession> m_pSession;

	GUIWnd *m_pWnd;
	IconStatic *m_pIconDesigner;
	IconStatic *m_pIconStuff[MAX_RANDOMPRODUCE_STUFF];
	GUIStatic *m_pStcStuffName[MAX_RANDOMPRODUCE_STUFF];
	GUIStaticEx *m_pStcStuffValue[MAX_RANDOMPRODUCE_STUFF];
	GUIProgress *m_pProTime;
	GUIStatic *m_pStcHuoLi;
	GUIStatic *m_pStcGold;
	GUIStatic *m_pStcSilver;
	GUIListBox *m_pListFashion;
	GUIButton *m_pBtnStart;
	GUIButton *m_pBtnClose;
	GUIPatch *m_pStcNPCBack;
	GUIStatic *m_pStcDesc;

	INT64 m_n64Designer;									// 设计图64位ID
	DWORD m_dwStuff[MAX_RANDOMPRODUCE_STUFF];				// 所需材料
	INT16 m_n16StuffNum[MAX_RANDOMPRODUCE_STUFF];			// 所需材料数目
	INT64 m_n64Stuff[MAX_RANDOMPRODUCE_STUFF];				// 所需材料64位ID,锁定用
	bool m_bHaveItem[MAX_RANDOMPRODUCE_STUFF];
	
	bool m_bProcessing;
	DWORD m_dwNPCID;
	DWORD m_dwEscapeTime;
	EAdvanceComposeType m_eRanType;
};