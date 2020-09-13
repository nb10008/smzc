/********************************************************************

** 文件名: FairyTrainFrame.h

** Copyright (c) 上海大承网络技术有限公司北京分公司 All Right Reserved

** 创建人: yfw

** 日  期: 08/25/2010

** 修改人: yfw

** 日  期: 08/25/2010

** 描  述: 妖精契约

*********************************************************************/
#pragma once

class IconStatic;
struct tagNS_OpenFairyTrain;
struct tagNS_ArrestingFairy;
struct tagShowDeadMenuEvent;

class FairyTrainFrame : public GameFrame
{
public:
	FairyTrainFrame(void);
	~FairyTrainFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

public:
	VOID SetNPCID( DWORD ID ) { m_dwNPCID = ID; }

private:
	BOOL EventHandler( tagGUIEvent *pEvent );
	DWORD OnNS_OpenFairyTrain(tagNS_OpenFairyTrain* pMsg, DWORD dwParam);
	DWORD OnNS_ArrestingFairy(tagNS_ArrestingFairy* pMsg, DWORD dwParam);
	DWORD OnLPDeadEvent(tagShowDeadMenuEvent* pEvent);
	VOID UpdateUI();
	VOID CloseAllUiEffect();
	VOID PlayEffect();

private:
	TSFPTrunk<FairyTrainFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<Util> m_pUtil;
	TObjRef<NetSession> m_pSession;

	GUIWnd *m_pWnd;										// 主界面
	GUIStatic *m_pStcFairyName;							// 妖精灵魂名字
	GUIStatic *m_pStcCurTameVale;						// 当前的驯服值
	GUIProgress *m_pProTameRate;						// 
	GUIStatic *m_pStcMaxTameValue;						// 契约值
	IconStatic *m_pTameItem;							// 契约物品
	GUIProgress *m_pProTime;							// 契约时间；
	IconStatic *m_pItem[5];								// 契约所需物品
	GUIButton *m_pBtnItemNum[5];						// 玩家契约物品个数
	DWORD m_dwItemTypeID[5];							// 契约物品ID
	DWORD m_dwCurItemID;								// 当前契约物品ID
	INT m_nCurTameValue;								// 当前契约值
	INT m_nMaxTameValue;								// 最大契约值
	GUIButton *m_pBtnClose;
	GUIButton *m_pBtnQuit;
	GUIPatch*m_pPcBack;

	DWORD m_dwNPCID;									// 妖精魂魄ID
	DWORD m_dwEscapeTime;
	vector<DWORD> m_vecItemID;							// 契约物品ID
	bool m_bChoosing;									// 是否选择中
};