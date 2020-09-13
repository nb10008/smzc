#pragma once

struct tagNC_GetKeyCodeGift;
struct tagStartNPCTalkEvent;
struct tagNS_GetKeyCodeGift;
class CommKeyCode :
	public GameFrame
{
private:
	
public:
	CommKeyCode();
	~CommKeyCode();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual BOOL EscCancel();

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	DWORD OnStartNPCTalkEvent(tagStartNPCTalkEvent* pGameEvent);

	DWORD OnNS_GetKeyCodeGift(tagNS_GetKeyCodeGift* pMsg, DWORD pPrama);
private:
	TSFPTrunk<CommKeyCode>			m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pFrameMgr;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*						m_pWndMain;
	GUIEditBox*					m_pEboxInput;
	GUIButton*					m_pBtnOK;
	GUIButton*					m_pBtnCancel;
	GUIStatic*					m_pStcTip;

	DWORD						m_escapeTime;
};
