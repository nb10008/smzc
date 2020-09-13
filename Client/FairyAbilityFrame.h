/********************************************************************

** 文件名: FairyAbilityFrame.h

** Copyright (c) 上海大承网络技术有限公司北京分公司 All Right Reserved

** 创建人: yfw

** 日  期: 10/22/2010

** 修改人: yfw

** 日  期: 10/22/2010

** 描  述: 妖精特殊能力

*********************************************************************/
#include "..\WorldDefine\ItemDefine.h"
#include ".\BaseData.h"//add by xp
#pragma once

class ProgressEx;
struct tagNS_FairyStuntBook;
struct tagNS_FairyStuntConfirm;
struct tagNS_FairyAddPromoteTimes;


class FairyAbilityFrame : public GameFrame
{
public:
	FairyAbilityFrame(void);
	~FairyAbilityFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	VOID ShowUI(bool bShow);
	VOID UpdateUI();

private:
	BOOL EventHandler( tagGUIEvent *pEvent );
	VOID SetProsessColor( ProgressEx *pPro, INT nValue);
	DWORD NetLearnFairySkill( tagNS_FairyStuntConfirm *pNetCmd, DWORD );
	DWORD NetLearnFairyBook( tagNS_FairyStuntBook *pNetCmd, DWORD );
	DWORD NetAddFairyPromoteTimes( tagNS_FairyAddPromoteTimes *pNetCmd, DWORD );//add by xp
	DWORD OnLearnFairySkill( tagMsgBoxEvent *pGameEvent );
	DWORD OnQuitFairySkill( tagMsgBoxEvent *pGameEvent );
	DWORD OnUpdateFairyAbility( tagGameEvent *pGameEvent );
	DWORD OnUpgradeFairyInjTimes( tagMsgBoxEvent *pGameEvent );//add by xp
	DWORD OnUpgradeFairyResTimes( tagMsgBoxEvent *pGameEvent );//add by xp

private:
	TSFPTrunk<FairyAbilityFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<Util> m_pUtil;
	TObjRef<NetSession> m_pSession;

	GUIWnd *m_pWnd;					// 主界面
	GUIStatic *m_pStcUpgradeDamageTimes;
	GUIStaticEx *m_pStcEleType;
	GUIStaticEx *m_pStcElePierceType;//元素穿透类型  
	ProgressEx *m_pProDamage;
	ProgressEx *m_pProPierce;//元素穿透进度条  
	GUIStatic *m_pStcDamgeValue;
	GUIStatic *m_pStcPierceValue;//元素穿透数值   
	GUIButton *m_pBtnUpgradeDamage;
	GUIButton *m_pBtnUpgradePierce;//元素穿透提升按钮
	GUIButton *m_pBtnAddEleDamgeTimes;//元素伤害\穿透 增加次数按钮
	GUIButton *m_pBtnAddEleDefenceTimes;//元素抵抗  增加次数按钮

	GUIStatic *m_pStcUpgradeDefenceTimes;
	GUIStatic *m_pStcElement[EER_End];
	GUIStatic *m_pStcElementDefenceValue[EER_End];
	ProgressEx *m_pProDefence[EER_End];
	GUIButton *m_pBtnUpgradeDefence[EER_End];

	GUIStaticEx *m_pStcFairySkill[MaxFairySkillNum];
	GUIStaticEx *m_pStcCurSkill;
	GUIButton *m_pBtnLearn;
	GUIButton *m_pBtnQuit;

	DWORD m_dwFairySkill[MaxFairySkillNum];
	DWORD m_dwUnsureSkill;
	DWORD m_dwFairyUpgradeTimes;//妖精伤害提升次数 add by xp
	DWORD m_dwFairyUpgradeResTimes;//妖精抵抗提升次数 add by xp

	BYTE m_byType;
	WORD m_wDamageTimes;
	WORD m_wDefenceTimes;
	INT m_nDamageValue;
	INT m_nPierceValue;//元素穿透数值
	INT m_nDefenceValue[EER_End];

	
};