#pragma once
#include "TeamSys.h"

struct tagNS_GetPersonalInfo;

class TeamInfoFrame : public GameFrame
{
public:
	TeamInfoFrame(void);
	~TeamInfoFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();
public:
	VOID ShowTeamInfo();
private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	VOID ShowPerInfo();
	DWORD OnNetGetPersonalInfo(tagNS_GetPersonalInfo *pNetCmd, DWORD);
	VOID InitData();
	VOID ChangeTeamTypeDetail();
	VOID ChangeScene();
private:
	TSFPTrunk<TeamInfoFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	GUIWnd*							m_pWnd;
	GUIPatch*						m_pWndBack;

	GUIPushButton*					m_pBtnTeam;
	GUIPatch*						m_pWndTeam;
	GUIComboBox*					m_pComboBoxTeamType;
	GUIComboBox*					m_pComboBoxTeamScene;
	GUIComboBox*					m_pComboBoxTeamTypeDetail;
	GUIListBox*						m_pTeamList;
	GUIButton*						m_pBtnCreateTeam;
	GUIButton*						m_pBtnRefreshTeam;
	GUIButton*						m_pBtnTeamApp;

	GUIPushButton*					m_pBtnPersonal;
	GUIPatch*						m_pWndPer;
	GUIComboBox*					m_pComboBoxPerType;
	GUIComboBox*					m_pComboBoxPerScene;
	GUIComboBox*					m_pComboBoxPerTypeDetail;
	GUIListBox*						m_pPerList;
	GUIButton*						m_pBtnCreatePer;
	GUIButton*						m_pBtnRefreshPer;
	GUIButton*						m_pBtnPerInv;

	DWORD							m_dwRoleID;

	GUIButton*						m_pBtnExit;
	EEasyTeamType					m_curType;
	DWORD							m_dwCurType;
	DWORD							m_dwSceneIndex;
	vector<DWORD>					m_vecID;
	vector<DWORD>					m_vecPerID;
	vector<tagClientPersonalMode>	m_vecPerMode;
	vector<BYTE>					m_vecSceneAndActive;
	vector<BYTE>					m_vecScene;
	BOOL							m_bFind;
};
