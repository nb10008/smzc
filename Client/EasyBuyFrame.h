#pragma once

#include "..\WorldDefine\easybuy.h"

class IconStatic;
class MallButton;
class EasyBuyFrame : public GameFrame
{
public:
	EasyBuyFrame(void);
	~EasyBuyFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID		 Show( DWORD dwUiType );
	VOID		 Hide(void);

	BOOL		 IsVisible(void) const;
	BOOL		 ReloadUI(LPCSTR alignCtrl, LPCSTR alignType, LPCSTR szNewName = NULL );
	VOID		 Align(void);

private:
	BOOL		 EventHandler(tagGUIEvent* pEvent);
	void		 RefreshItemFlag();
	DWORD		 OnClosePackageEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseSocialEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseStorageEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseQuestEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseRoleSkillEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseRoleInfoEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnClosePetEasyBuyFrame( tagGameEvent* pGameEvent );
	DWORD		 OnCloseComposeEasyBuyFrame( tagGameEvent* pGameEvent );


private:
	TSFPTrunk<EasyBuyFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIWnd						*m_pWnd;							// 主界面

	IconStatic*					m_pItemSel[UI_MAX_ITEM];			// 商品选择框
	MallButton*					m_pItem[UI_MAX_ITEM];				// 商品
	GUIStatic					*m_pStcYuanBao[UI_MAX_ITEM];		// 商品价格
	GUIStatic					*m_pStcBao[UI_MAX_ITEM];			// 宝

	GUIButton					*m_pBtnMall;						// 商城界面
	GUIButton					*m_pBtnClose;						// 关闭窗口按钮
};