#pragma once

class CityWarApplyMoneyFrame : public GameFrame
{
public:
	CityWarApplyMoneyFrame();
	~CityWarApplyMoneyFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID SetLowerText(int nFund);
	VOID SetAverageText(int nFund);
private:
	DWORD EventHandler(tagGUIEvent *pGUIEvent);
	DWORD OnMsgBoxEventConfirmAttack(tagMsgBoxEvent *pEvent);
	DWORD OnEventClose(tagGameEvent *pEvent);

private:
	TSFPTrunk<CityWarApplyMoneyFrame> m_Trunk;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<GUISystem> m_pGUI;
	
	GUIWnd *m_pWnd;
	GUIEditBox *m_pInputGold;		// 输入金
	GUIStatic *m_pStcGold;			// 报名下限费用金
	GUIStatic *m_pStcAverageGold;	// 报名平均费用金
	
	GUIButton *m_pBtnOK;			// 确定
	GUIButton *m_pBtnCancel;		// 取消

	INT32 m_nInputGold;			// 输入金额
	DWORD m_dwCityID;			// 城市ID
};

