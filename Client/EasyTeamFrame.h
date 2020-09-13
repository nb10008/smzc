#pragma  once 
#include "..\WorldDefine\msg_easy_team.h"

class EasyTeamFrame : public GameFrame
{
public:
	EasyTeamFrame();
	~EasyTeamFrame();

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL ReloadUI();
	virtual BOOL Destroy();
	virtual BOOL EscCancel();
	virtual VOID Update();

public:
	void ShowTeamInfo();
	
private:
	DWORD EventHandler( tagGUIEvent *pEvent );
	DWORD OnNetGetSinglePlayers(tagNS_GetSinglePlayers *pNetCmd, DWORD);
	void ShowNearbyPlayerInfo();

private:
	TSFPTrunk<EasyTeamFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pFrameMgr;
	TObjRef<NetSession> m_pSession;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;

	GUIWnd *m_pWnd;								
	GUIPatch *m_pWndBack;
			
	GUIPushButton *m_pBtnTeam;
	GUIPushButton *m_pBtnPerson;
	GUIPushButton *m_pBtnActive;
	GUIButton *m_pBtnPerInv;
	GUIButton *m_pBtnTeamInv;
	GUIButton *m_pBtnRefresh;
	GUIButton *m_pBtnPerRefresh;
	GUIButton *m_pBtnQuit;
	GUIListBox *m_pTeamList;
	GUIListBox *m_pPlayerList;
	GUIWnd *m_pWndTeam;
	GUIWnd *m_pWndPlayer;

	vector<DWORD> m_vecKillCreTeam;
	vector<DWORD> m_vecNearbyPlayer;

	DWORD m_dwRoleID;

	bool m_bStartCountTime;
	DWORD m_dwDisableTime;
	bool m_bDestroy;
};