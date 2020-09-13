#pragma once

/*!
\class PlayerTelFrame
\brief 获取玩家手机号的界面。。。
\author lib
\base GameFrame
\remark
*/
struct tagNC_NotSaveTelNum;
struct tagNC_SaveTelNum;
class PlayerTelFrame : public GameFrame
{
public:
	PlayerTelFrame(void);
	~PlayerTelFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	TSFPTrunk<PlayerTelFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	//GUI控件
	GUIWnd*						m_pWnd;
	GUIButton*					m_pBtnSend;
	GUIButton*					m_pBtnNever;
	GUIButton*					m_pBtnNext;
	GUIEditBox*					m_pEboxTelNum;
};
