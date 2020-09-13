#pragma once


struct tagCityWarAssistConfirmRefreshEvent;

class CityWarAssistConfirmFrame : public GameFrame
{
public:
	CityWarAssistConfirmFrame();
	~CityWarAssistConfirmFrame();
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent* pGUIEvent);
	DWORD OnEventRefreshUI(tagCityWarAssistConfirmRefreshEvent *pEvent);
	DWORD OnMsgBoxEventAssistConfirm(tagMsgBoxEvent *pEvent);
	VOID UpdateUI();
private:
	TSFPTrunk<CityWarAssistConfirmFrame> m_Trunk;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<GUISystem> m_pGUI;

	GUIWnd *m_pWnd;
	GUIListBox *m_pLstGuilds;	// 报名防御帮派
	GUIListBox *m_pLstAssist;	// 确认协助帮派
	GUIButton *m_pBtnConfirm;	// 同意
	GUIButton *m_pBtnNotConfirm;// 撤销
	GUIButton *m_pBtnOK;		// 确定
	GUIButton *m_pBtnCancel;	// 取消
	GUIButton *m_pBtnClose;		// 关闭

	typedef std::vector<DWORD> GuildArray;
	GuildArray m_vecGuilds;				// 报名防御的帮派
	GuildArray m_vecAssistGuilds;		// 确认的帮派
	int m_nGuildRow;
	int m_nAssistRow;
	DWORD m_dwSelectedGuildID;
	int m_nCurSelectGuildRow;
	int m_nCurSelectAssistRow;
};