#pragma once
#include "gameframe.h"

/** class	HolyBattleFrame
brief	   仙界大乱斗活动界面
*/ 

struct tagNS_SetArenaInterface;
struct tagNS_SetArenaName;
struct tagNS_SetArenaPercent;
struct tagNS_SetArenaMemNum;


class HolyBattleFrame :
	public GameFrame
{
public:
	HolyBattleFrame(void);
	~HolyBattleFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	VOID	ShowWnd(BOOL bOpen=TRUE);
	VOID	ShowMinWnd(BOOL bOpen=TRUE);
private:
	DWORD OnNS_SetArenaInterface(tagNS_SetArenaInterface *pNetCmd, DWORD);
	DWORD OnNS_SetArenaName(tagNS_SetArenaName *pNetCmd, DWORD);
	DWORD OnNS_SetArenaPercent(tagNS_SetArenaPercent *pNetCmd, DWORD);
	DWORD OnNS_SetArenaMemNum(tagNS_SetArenaMemNum *pNetCmd, DWORD);

	VOID ShowTips(DWORD dwIndex);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD MinEventHandler(tagGUIEvent* pEvent);

private:
	TSFPTrunk<HolyBattleFrame>		m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*				m_pWnd;
	GUIButton*			m_pBtnUnfold;							//关闭按钮
	GUIStatic*			m_pStcBattleName;				//战役名
	GUIProgress*		m_pProRedArmy;					//红军进度条
	GUIStatic*			m_pStcRedPercent;					//红军百分比
	GUIStatic*			m_pStcRedNum;					//红军百分比
	GUIProgress*		m_pProBlueArmy;					//蓝军进度条
	GUIStatic*			m_pStcBluePercent;				//蓝军百分比
	GUIStatic*			m_pStcBlueNum;				//蓝军百分比

	GUIWnd*				m_pWndMin;
	GUIButton*			m_pBtnWndFold;							//展开按钮

};
