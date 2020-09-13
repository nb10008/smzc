#pragma once
struct tagNS_PutOutInfo;

class CreateTeamFrame : public GameFrame
{
public:
	CreateTeamFrame(void);
	~CreateTeamFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
public:
	VOID SetTeamFlag(BOOL bFlag);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnNetPutOutInfo(tagNS_PutOutInfo *pNetCmd, DWORD);
	VOID InitData();
	VOID ChangeTeamType();
	VOID ChangeScene();
	VOID ChangeTeamTypeDetail();
private:
	TSFPTrunk<CreateTeamFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	GUIWnd*							m_pWnd;
	GUIPatch*						m_pWndBack;

	GUIComboBox*					m_pComboBoxType;
	GUIComboBox*					m_pComboBoxScene;
	GUIComboBox*					m_pComboBoxDetail;
	GUIComboBox*					m_pComboBoxTeamNum;
	GUIStatic*						m_pStcTeamNum;

	GUIEditBox*						m_pEditMinLevel;
	GUIEditBox*						m_pEditMaxLevel;
	GUIEditBox*						m_pEditTeamDetail;

	GUIButton*						m_pBtnCreate;
	GUIButton*						m_pBtnExit;

	BYTE							m_byTeamType;
	BYTE							m_byTeamScene;
	BYTE							m_byTeamTypeDetail;
	BYTE							m_byTeamMaxNum;
	BOOL							m_bTeam;

	vector<BYTE>					m_vecScene;
	vector<BYTE>					m_vecTypeDetail;
};
