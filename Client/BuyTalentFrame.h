#pragma once
#include "gameframe.h"

struct tagNS_RolePracticeTalentPoint;

class BuyTalentFrame : public GameFrame
{
public:
	BuyTalentFrame(void);
	~BuyTalentFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID ShowWnd( BOOL bShow );

protected:
	
	DWORD EventHandler( tagGUIEvent* pEvent );

	DWORD OnBuyTalent();

	void RefreshDisplay();

	INT64 CalculateCost( DWORD dwTalentPointNum );

	DWORD OnNS_RolePracticeTalentPoint( tagNS_RolePracticeTalentPoint* pNetCmd, DWORD dwParam );

private:
	TSFPTrunk<BuyTalentFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd*					m_pWnd;
	GUIButton*				m_pBtnAdd;				//修炼天资按钮
	GUIButton*				m_pBtnOk;				//确定按钮
	GUIButton*				m_pBtnCancel;			//取消按钮
	GUIButton*				m_pBtnClose;			//关闭按钮

	GUIStaticEx*			m_pStcMaxPoint;			//当前级别可得到的最大天资点数
	GUIStaticEx*			m_pStcCanBuyPoint;		//还可购买的天资点数
	GUIStaticEx*			m_pStcGold;				//将要花费的金
	GUIStaticEx*			m_pStcSilver;			//将要话费的银
	GUIStaticEx*			m_pStcHaveGold;			//角色当前还有的金
	GUIStaticEx*			m_pStcHaveSilver;		//角色当前还有的银

	INT64					m_nWillCostGold;		//将要话费的金
	INT64					m_nWillCostSilver;		//将要话费的银
};
