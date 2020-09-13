#pragma once

struct tagNS_GetStrengthDetail;

class StrengthShowFrame : public GameFrame
{	
public:
	StrengthShowFrame();
	~StrengthShowFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
//	DWORD OnNetGetStrengthDetail(tagNS_GetStrengthDetail *pNetCmd, DWORD);
private:
	TSFPTrunk<StrengthShowFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pGFMgr;

	GUIWnd *m_pWnd;
	GUIStatic *m_pStcTotalStrength;	// 实力总值
	GUIStatic *m_pStcBaseStrength;	// 固有实力值
	GUIStatic *m_pStcLevel;			// 等级
	GUIStatic *m_pStcEquip;			// 装备
	GUIStatic *m_pStcPet;			// 灵兽
	GUIStatic *m_pStcFame;			// 声望
	GUIStatic *m_pStcTitle;			// 称号
	GUIStatic *m_pStcFriend;		// 好友
	GUIStatic *m_pStcQuest;			// 任务
	GUIStatic *m_pStcProduction;	// 生产
	GUIStatic *m_pStcAdvanceStrength;// 进阶实力值 
	GUIStatic *m_pStcActivity;		// 活动
	GUIStatic *m_pStcInstance;		// 副本
	GUIStatic *m_pStcRanQuest;			// 任务
	GUIStatic *m_pStckillPlayer;		// 战胜非白名玩家
	GUIStatic *m_pStcConsumptiveStrength;// 可消耗点数
	GUIButton *m_pBtnClose;			// 关闭
};	