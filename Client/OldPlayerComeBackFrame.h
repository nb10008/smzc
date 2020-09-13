#pragma once
#include "gameframe.h"
class ItemButton;
struct tagNS_PlayerRegressionOk;

class OldPlayerComeBackFrame : public GameFrame
{
public:
	OldPlayerComeBackFrame(void);
	~OldPlayerComeBackFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	void Show( DWORD dwTypeID, INT nCount, BYTE byQuality );

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	DWORD OnNetNS_PlayerRegressionOk( tagNS_PlayerRegressionOk *pNetCmd, DWORD );
	
private:
	TSFPTrunk<OldPlayerComeBackFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd*						m_pWndWorld;
	GUIButton*					m_pBtnGet;
	ItemButton*					m_pIconItem;
	DWORD						m_dwItemTypeID;
};
