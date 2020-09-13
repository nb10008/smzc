#pragma once
#include "gameframe.h"
#include "../WorldDefine/msg_external_links.h"

class ExternalLinksFrame : public GameFrame
{
public:
	ExternalLinksFrame(void);
	~ExternalLinksFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	void ShowWnd( BOOL bShow );

private:
	DWORD EventHandler( tagGUIEvent *pEvent );

	DWORD OnNS_GetExternalLinks( tagNS_GetExternalLinks* pNetCmd, DWORD dwParams );

private:
	TSFPTrunk<ExternalLinksFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr>	m_pNetCmdMgr;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;

	GUIWnd	*m_pWnd;
	GUIButton* m_linkBtns[EXTERNAL_LINKS_NUM];
	GUIButton *m_pBtnClose;

	tagExternalLink			m_links[EXTERNAL_LINKS_NUM];
	DWORD					m_nLinkNum;
};
