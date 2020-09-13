#pragma once

class CondancedFrame : GameFrame
{
public:
	CondancedFrame();
	~CondancedFrame();
	virtual BOOL Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

private:
	TSFPTrunk<CondancedFrame> m_Trunk;
	TObjRef<GUISystem>		   m_pGUI;
	TObjRef<GameFrameMgr>	   m_pFrameMgr;
	TObjRef<NetSession>        m_pNetSession;
	TObjRef<NetCmdMgr>         m_pNetCmdMgr;

	GUIWnd                  *m_pWnd;
	GUIWnd                  *m_pWndCaption;

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
};