#pragma once

class ItemButton;
class Equipment;
class IconStatic;
struct tagItemPosChangeExEvent;
struct tagNS_StrengthenEquip;
struct tagNS_RolePutDownItem;

class EquipStrengthenFrame :public GameFrame
{

public:
	EquipStrengthenFrame(void);
	~EquipStrengthenFrame(void);

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnEventPutItem(tagItemPosChangeExEvent* pGameEvent);
	DWORD OnNoGuardFu( tagMsgBoxEvent *pGameEvent );
	DWORD NetEquipStrengthen( tagNS_StrengthenEquip *pNetCmd, DWORD );
	DWORD NetRolePutDownItem( tagNS_RolePutDownItem *pNetCmd, DWORD );
	VOID PutItem( INT16 nPos );
	void LockItem( bool bOperable );
	BOOL CanStartStrengthen();
	VOID ClearUI();
	VOID UpdateUI();
	VOID CloseAllUiEffect();
	DWORD OnItemEvent_UpdateItem(tagGameEvent* pEvent);

private:
	TSFPTrunk<EquipStrengthenFrame>	m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<NetSession>					m_pSession;
	TObjRef<NetCmdMgr>					m_pCmdMgr;

	GUIWnd*						m_pWndMain;
	GUIWnd*						m_pWndCaption;
	ItemButton*					m_pEquip;				// 将要强化的装备
	ItemButton*					m_pStone;				// 强化石
	ItemButton*					m_pLucky;				// 幸运符
	ItemButton*					m_pGuard;				// 守护符
	GUIProgress*				m_pProTime;				// 进度条
	GUIStaticEx*				m_pStcEquipName;
	GUIStatic*					m_pStcEquipState;		// 装备的神铸状态
	GUIStatic*					m_pStcStoneName;
	GUIStatic*					m_pStcLuckyName;
	GUIStatic*					m_pStcGuardName;
	GUIStaticEx*				m_pStcSucRate;			// 增加的成功率
	GUIStatic*					m_pStcGolden;			// 金
	GUIStatic*					m_pStcSiller;			// 银
	GUIButton*					m_pBtnOk;				// 开始强化
	GUIButton*					m_pBtnCancel;			// 关闭
	GUIButton*					m_pBtnExit;				// 退出
	IconStatic*					m_pStcResult;			// 强化结果
	DWORD						m_dwRecivMegTime;

	DWORD						m_dwNPCID;				// NPC的ID
	DWORD						m_escapeTime;

	Equipment*					m_pItemEquip;	// 装备
	Item*						m_pItemStone;	// 强化石
	Item*						m_pItemLucky;	// 幸运符
	Item*						m_pItemGuard;	// 保底符
	BOOL						m_bProcessing;	// 是否强化中
};