#pragma once
#include "gameframe.h"
#include "..\WorldDefine\mall_define.h"

class ItemButton;
class IconStatic;
struct tagOpenLevelPromoteFrame;
struct tagNS_RoleLevelChange;

class LevelPromoteFrame :
	public GameFrame
{
public:
	LevelPromoteFrame(void);
	~LevelPromoteFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

	// 显示团购界面
	VOID ShowWnd(BOOL b);
	BOOL IsShow();

private:
	DWORD EventHandler(tagGUIEvent* pEvent);

	VOID  ButtonEnableSet();

	DWORD OnOpenLevelPromoteFrame(tagOpenLevelPromoteFrame* pEvent);
	VOID  InitData();
	VOID  ClearUI();
	VOID  RefreshUI();
	void  RefreshItemFlag();
	VOID  OnConfirmMsg(tagMsgBoxEvent* pGameEvent);
	DWORD OnNS_RoleLevelChange( tagNS_RoleLevelChange* pNetCmd, DWORD );
	//直充
	int EncodeBase64(BYTE *btSrc, int iSrcLen, BYTE *btRet, int *piRetLen); 
	void YuanBaoAbundance();
private:
	TSFPTrunk<LevelPromoteFrame>	m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	GUIWnd*							m_pWnd;

	IconStatic*						m_pGiftBack[MAX_GIFT_PER_PAGE];
	ItemButton*						m_pIBnGift[MAX_GIFT_PER_PAGE];
	GUIStatic*						m_pStcName[MAX_GIFT_PER_PAGE];
	GUIStatic*						m_pStcPrice[MAX_GIFT_PER_PAGE];

	GUIStatic*						m_pPickPatch[LEVEL_NUM_CHOOSE];
	GUIPushButton*					m_pPBnLevel[LEVEL_NUM_CHOOSE];
	
	GUIButton*						m_pBtnBuy;

	GUIButton*						m_pClose;

	GUIButton*						m_pBtnPay;					// 充值


	INT								m_CurBtnLevel;

	DWORD							m_dwSelectID;
	INT								m_nSelectPrice;						

	BOOL							m_bOnceGlintFlag[LEVEL_NUM_CHOOSE];

};
