#pragma once

struct tagRefreshGodRewardEvent;
struct tagRoleGetNameEvent;

class GodRewardFrame : public GameFrame
{
public:
	GodRewardFrame();
	~GodRewardFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();
	
private:
	TObjRef<GUISystem> m_pGUI;
	TSFPTrunk<GodRewardFrame> m_Trunk;

	GUIWnd *m_pWnd;
	
	GUIStatic *m_pStcWin;
	GUIStatic *m_pStcNum1;	// 第1名
	GUIStatic *m_pStcNum2;	// 第2名
	GUIStatic *m_pStcNum3;	// 第3名
	GUIStatic *m_pStcNum4;	// 第4名
	GUIStatic *m_pStcNum5;	// 第5名

	GUIStatic *m_pStcTime1;
	GUIStatic *m_pStcTime2;
	GUIStatic *m_pStcTime3;
	GUIStatic *m_pStcTime4;
	GUIStatic *m_pStcTime5;


	GUIStatic *m_pStcKillNum;	// 击杀次数
	GUIStatic *m_pStcLiveTime;	// 活跃时间

	GUIStatic *m_pStcExploit;		// 帮派功勋
	GUIStatic *m_pStcContribution;	// 帮派贡献
	GUIStatic *m_pStcIntegral;		// 帮派积分
	GUIStaticEx *m_pStcReward;		// 奖励领取人

	GUIButton *m_pBtnOK;	// 确定
	GUIButton *m_pBtnClose;	// X

	DWORD m_dwRoleID1;
	DWORD m_dwRoleID2;
	DWORD m_dwRoleID3;
	DWORD m_dwRoleID4;
	DWORD m_dwRoleID5;


	DWORD EventHandler(tagGUIEvent *pEvent);
	DWORD OnEventRefresh(tagRefreshGodRewardEvent *pEvent);
	DWORD OnEventRoleGetName(tagRoleGetNameEvent *pEvent);

	DWORD m_dwElapseTime;
};