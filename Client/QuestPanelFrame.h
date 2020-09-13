/********************************************************************

** 文件名: QuestPanelFrame.h

** Copyright (c) 上海大承网络技术有限公司北京分公司 All Right Reserved

** 创建人: yfw

** 日  期: 06/03/2010

** 修改人: yfw

** 日  期: 06/05/2010

** 描  述: 任务版，随机任务

*********************************************************************/
#pragma once
#include "QuestMgr.h"

class IconStatic;
class ListBoxEx;
struct tagNS_OpenQuestBoard;

class QuestPanelFrame : public GameFrame
{
public:
	QuestPanelFrame(void);
	~QuestPanelFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	BOOL EscCancel();
	VOID SetNPCID(DWORD id, EFunctionNPCType eType, DWORD dwNpcTypeID = 0 ) { m_dwNpcID = id; m_eNPCType = eType; m_dwNpcTypeID = dwNpcTypeID; }

public:
	VOID UpdateQuestList();
	UINT16 GetQuestStartID();

private:
	BOOL EventHandler( tagGUIEvent *pEvent );
	// 更新任务信息
	VOID UpdateQuestsDetail( UINT id );
	VOID UpdateQuestReward( UINT16 id );

private:
	TSFPTrunk<QuestPanelFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;

	GUIWnd *m_pWnd;										// 主界面
	GUIPatch *m_pCaption;								
	ListBoxEx *m_pLBQuestPanel;							// 任务列表
	GUIStaticEx *m_pStcExQuestDeatail;					// 任务详述
	GUIStatic *m_pStcExpAwarad;							// 经验奖励
	GUIStatic *m_pStcGoldAward;							// 金币奖励
	GUIStatic *m_pStcSilverAward;						// 银币奖励
	IconStatic *m_pPicItemAward[QUEST_REW_ITEM];		// 物品奖励
	DWORD m_dwRewardItemID[QUEST_REW_ITEM];				// 物品ID

	GUIButton *m_pBtnRefreshQuest;						// 刷新任务列表
	GUIButton *m_pBtnAcceptQuest;						// 接取任务
	GUIButton *m_pBtnDeliverQuest;						// 交付任务
	GUIButton *m_pBtnExit;								// 退出

	DWORD m_dwNpcID;									// NPC ID
	DWORD m_dwNpcTypeID;	
	DWORD m_dwEscapeTime;
	INT m_nSelRow;										// 选择的任务行
	EFunctionNPCType m_eNPCType;
	vector<tagIDLink> m_idLinks;
	vector<tagPosLink> m_posLinks;
};