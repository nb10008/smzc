#pragma once
#include "../WorldDefine/msg_VoteShow_define.h"

class ListFrame: public GameFrame
{
public:
	ListFrame();
	~ListFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();
	void ShowFrame( bool bShow );

private:
	DWORD EventHandler( tagGUIEvent *pEvent );
	DWORD OnNS_QueryDragonGirlRankings( tagNS_QueryDragonGirlRankings *pNetCmd, DWORD );
	DWORD OnNS_QueryDragonDefenderRankings( tagNS_QueryDragonDefenderRankings *pNetCmd, DWORD );

private:
	TSFPTrunk<ListFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetSession> m_pSession;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;

	GUIWnd *m_pWnd;
	GUIWnd *m_pWndBackPic;

	GUIListBox *m_pList;
	GUIButton *m_pBtnOk;
	GUIButton *m_pBtnExit;

	DWORD	m_dwNPCID;		// npcID
};