#pragma once
#include "gameframe.h"

class PickStrategyFrame :
	public GameFrame
{
public:
	PickStrategyFrame(void);
	~PickStrategyFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();

	void UItoSetting();
	void SettingtoUI();
	void close();
	void RefreshColour(bool b);
	BOOL EventHandler(tagGUIEvent* pEvent);
private:
	TSFPTrunk<PickStrategyFrame>m_Trunk;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>	m_pFrameMgr;
	TObjRef<GUISystem>			m_pGUI;

	GUIWnd*		m_pWnd;
	vector<GUIPushButton*>		m_vecPushButtons;
	GUIButton*		m_pBnSave;
	GUIButton*		m_pBnClose;
	GUIButton*		m_pBnExit;
};
