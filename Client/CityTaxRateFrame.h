#pragma once

// …Ë÷√À∞ ’
class CityTaxRateFrame : public GameFrame
{
public:
	CityTaxRateFrame();
	~CityTaxRateFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();

	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	
	VOID SetCurTaxRateText(DWORD dwRate);

private:
	DWORD EventHandler(tagGUIEvent *pEvent);

private:
	TSFPTrunk<CityTaxRateFrame> m_Trunk;
	TObjRef<NetSession> m_pSession;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pFrameMgr;

	GUIWnd *m_pWnd;
	GUIStaticEx *m_pStcCurRate;
	GUIEditBox *m_pEdbRateSet;
	GUIButton *m_pBtnOK;
	GUIButton *m_pBtnCancel;
	
};