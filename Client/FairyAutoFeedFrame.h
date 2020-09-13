/********************************************************************

** 文件名: FairyAutoFeedFrame.h

** Copyright (c) 上海大承网络技术有限公司北京分公司 All Right Reserved

** 创建人: yfw

** 日  期: 10/18/2010

** 修改人: yfw

** 日  期: 10/18/2010

** 描  述: 妖精自动喂养

*********************************************************************/
#pragma once

class IconStatic;
struct tagFairyMood;
struct tagUseFairyFoodEvent;
struct tagItemPosChangeExEvent;
struct tagNS_RolePutDownItem;
struct tagNS_FabaoFeeding;
struct tagRoleGetNameEvent;

class FairyAutoFeedFrame : public GameFrame
{
public:
	FairyAutoFeedFrame(void);
	~FairyAutoFeedFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	VOID ShowUI(bool bShow);

private:
	BOOL EventHandler( tagGUIEvent *pEvent );
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnRoleGetNameEvent(tagRoleGetNameEvent* pEvent);
	VOID PutItem( INT16 nPos, bool bMainFood = true );
	VOID UpdateUI( DWORD &dwFoodID );
	VOID ReadSettingFromFile();
	VOID SaveSettingToFile();
	DWORD OnUpdateFairyMood(tagFairyMood* pEvent);
	DWORD OnEventChangeFairy(tagGameEvent* pGameEvent);
	DWORD OnNoFairyFood( tagMsgBoxEvent *pGameEvent );
	DWORD OnUseFairyFood( tagUseFairyFoodEvent *pGameEvent );
	VOID LoadFromFile();
	bool IfCanEat(DWORD dwTypeID, bool bKnownFairy = false, bool bShowTips = true );
	DWORD NetFairyFeed( tagNS_FabaoFeeding *pNetCmd, DWORD );
	DWORD NetRolePutDownItem( tagNS_RolePutDownItem *pNetCmd, DWORD );

private:
	TSFPTrunk<FairyAutoFeedFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<Util> m_pUtil;
	TObjRef<NetSession> m_pSession;

	GUIWnd *m_pWnd;					// 主界面
	GUIEditBox *m_pStcMoodRate;		// 心情比率
	GUIEditBox *m_pStcIntervalTime;	// 间隔时间
	IconStatic *m_pIconMainFood;	// 优先食物
	IconStatic *m_pIconSecFood;		// 候补食物
	GUIStatic *m_pStcMainFoodName;
	GUIStatic *m_pStcMainFoodNum;
	GUIStatic *m_pStcSecFoodName;
	GUIStatic *m_pStcSecFoodNum;
	GUIButton *m_pBtnAuto;
	GUIButton *m_pBtnClose;
	GUIButton *m_pBtnCancle;
	INT m_nMoodRate;
	INT m_nTime;
	DWORD m_dwEscapeTime;
	DWORD m_dwMainFood;
	DWORD m_dwSecFood;
	INT m_nFairyMood;
	bool m_bAutoFeed;
	bool m_bWarning;
	map<DWORD, INT> m_mapFairyFood;
};