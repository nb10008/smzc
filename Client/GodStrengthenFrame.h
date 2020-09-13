#pragma once

class ItemButton;
struct tagNS_GodStrengthen;
struct tagItemPosChangeExEvent;

class GodStrengthenFrame :public GameFrame
{

public:
	GodStrengthenFrame(void);
	~GodStrengthenFrame(void);

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	DWORD NetEquipStrengthen( tagNS_GodStrengthen *pNetCmd, DWORD );
	VOID PutItem( INT16 nPos, bool bMainEquip = true );
	void LockItem( INT64 nID, bool bOperable );
	VOID UpdateUI();
	DWORD OnGodStrengthenExpValue( tagMsgBoxEvent *pGameEvent );

private:
	TSFPTrunk<GodStrengthenFrame>	m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*						m_pWndMain;
	GUIWnd*						m_pWndCaption;
	ItemButton*					m_pEquip;				// 将要强化的装备
	ItemButton*					m_pAssisEquip;			// 辅助强化装备
	GUIStatic*					m_pStcEquipLevel;		// 神铸等级
	GUIStatic*					m_pStcTime;				// 下次神铸时间
	GUIProgress*				m_pProExp;				// 经验进度条
	GUIStatic*					m_pProValue;			// 经验进度条
	GUIStatic*					m_pStcExp;				// 本次操作增加的经验值
	GUIStatic*					m_pStcGolden;			// 金
	GUIStatic*					m_pStcSiller;			// 银
	GUIButton*					m_pBtnOk;				// 开始神铸
	GUIButton*					m_pBtnCancel;			// 取消
	GUIButton*					m_pBtnExit;				// x

	INT64						m_n64EquipID;
	INT64						m_n64AssisEquipID;
	BOOL						m_bTime;				// 是否已过24小时
};