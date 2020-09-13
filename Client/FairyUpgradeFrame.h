/********************************************************************

** 文件名: FairyUpgradeFrame.h

** Copyright (c) 上海大承网络技术有限公司北京分公司 All Right Reserved

** 创建人: yfw

** 日  期: 10/26/2010

** 修改人: yfw

** 日  期: 10/26/2010

** 描  述: 妖精能力提升

*********************************************************************/
#pragma once
class IconStatic;
struct tagItemPosChangeExEvent;
struct tagNS_FairyIncEleInjury;

class FairyUpgradeFrame : public GameFrame
{
public:
	FairyUpgradeFrame(void);
	~FairyUpgradeFrame(void);

	virtual BOOL Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	//VOID SetInfo( BYTE byType, BYTE byTimes, INT nValue, bool bDamage = false );
	VOID SetInfo( BYTE byType, WORD wTimes, INT nValue, BYTE bAttType );//modify by xp

private:
	BOOL EventHandler( tagGUIEvent *pEvent );
	VOID LockItem(INT64 nID, bool bOperable);
	DWORD NetFairyUpgrade( tagNS_FairyIncEleInjury *pNetCmd, DWORD );
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	VOID PutItem(INT16 nPos, DWORD dwIndex = GT_INVALID);
	VOID UpdateUI();

private:
	TSFPTrunk<FairyUpgradeFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr> m_pCmdMgr;
	TObjRef<Util> m_pUtil;
	TObjRef<NetSession> m_pSession;

	GUIWnd *m_pWnd;										// 主界面
	GUIStatic *m_pStcFairyDamage;
	GUIStatic *m_pStcDamageBaseValue;
	GUIStatic *m_pStcAddMin;
	GUIStatic *m_pStcAddMax;

	IconStatic *m_pIconFruit[3];
	GUIStatic *m_pStcFruitName[3];
	GUIStatic *m_pStcFruitAddMin[3];
	GUIStatic *m_pStcFruitAddMax[3];

	IconStatic *m_pIconStone;
	GUIStatic *m_pStcRate;
	GUIStatic *m_pStcUpgradeTime;

	GUIProgress *m_pProTime;

	GUIButton *m_pBtnUpgrade;
	GUIButton *m_pBtnQuit;
	GUIButton *m_pBtnExit;

	bool m_bProcessing;

	INT m_nValue;
	WORD m_wTime;
	BYTE m_byType;
// 	BOOL m_bDamage;
	BYTE m_bDamageType;//元素伤害类型，0代表伤害，1代表穿透
	BYTE  m_bAttType;//add by xp 用来区分伤害、穿透和抵抗0伤害，1穿透，2抵抗
	INT m_nMin;
	INT m_nMax;

	INT64 m_n64FruitID[3];
	INT64 m_n64StoneID;
};