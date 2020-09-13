#pragma once
#include "gameframe.h"

class GlobalHelpFrame : public GameFrame
{
public:
	GlobalHelpFrame(void);
	~GlobalHelpFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	void ShowWnd( BOOL bShow );

private:
	DWORD EventHandler( tagGUIEvent *pEvent );

private:

	TSFPTrunk<GlobalHelpFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;

	GUIWnd*		m_pWnd;
	GUIButton*	m_pBtnNewOne;
	GUIButton*	m_pBtnHelpMenu;
	GUIButton*	m_pBtnOrg;
	GUIButton*	m_pBtnCancel;
	GUIButton*	m_pBtnLoongBao;
	GUIButton*  m_pBtnClose;

	GUIButton*  m_pBtnIdle;				//目前没用--隐藏之
};
