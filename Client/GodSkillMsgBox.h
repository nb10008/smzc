#pragma once

class GodSkillMsgBox : public GameFrame
{
public:
	GodSkillMsgBox();
	~GodSkillMsgBox();
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	void SetText(const tstring& strSimple, const tstring& strDetail);

private:
	TObjRef<GUISystem> m_pGUI;
	TSFPTrunk<GodSkillMsgBox> m_trunk;

	GUIWnd *m_pWnd;
	GUIStaticEx *m_pStcSimple;
	GUIStaticEx *m_pStcDetail;
	GUIButton   *m_pBtnOK;

	DWORD EventHandler(tagGUIEvent *pEvent);
};