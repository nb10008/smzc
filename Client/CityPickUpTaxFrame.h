#pragma once

// 查看当前税收、提取税收
class CityPickUpTaxFrame : public GameFrame
{
public:
	CityPickUpTaxFrame();
	~CityPickUpTaxFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	VOID SetCurTax(INT32 nTax);

private:
	DWORD OnEventPersonalPickTax(tagMsgMoneyBoxEvent *pEvent);
	DWORD OnEventGuildPickTax(tagMsgMoneyBoxEvent *pEvent);
private:
	TSFPTrunk<CityPickUpTaxFrame> m_Trunk;
	TObjRef<NetSession> m_pSession;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pFrameMgr;

	GUIWnd *m_pWnd;
	
	GUIStaticEx *m_pStcCurTax;
	GUIButton *m_pBtnPersonal;
	GUIButton *m_pBtnGuild;
	GUIButton *m_pBtnCancel;

	DWORD EventHandler(tagGUIEvent *pEvent);
	INT32 m_curTax;
};

