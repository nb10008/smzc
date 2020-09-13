#pragma once
#include "gameframe.h"

class FamilyMemberFrame;
class FamilySpriteFrame;
class FamilyQuestFrame;
struct tagNC_OpenFamilyQuestPage;
struct tagFamilyNameEvent;

class FamilyFrame :
	public GameFrame
{
public:
	FamilyFrame(void);
	~FamilyFrame(void);
	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID Update();
	VOID ShowWnd(BOOL bShow=TRUE);
	VOID RefreshUI();
	BOOL IsShowing(); 
	// common----------------------------------------------------------------------------------------------------
	tstring GetGUIFullName()
	{
		if(P_VALID(m_pWndCaption))
			return m_pWndCaption->GetFullName();
		else 
			return _T("");
	};
	// 家族成员----------------------------------------------------------------------------------------------------

	// 家族任务----------------------------------------------------------------------------------------------------

	// 家族妖精----------------------------------------------------------------------------------------------------
private:
	// 家族成员----------------------------------------------------------------------------------------------------

	// 家族任务----------------------------------------------------------------------------------------------------

	// 家族妖精----------------------------------------------------------------------------------------------------
	
	// common----------------------------------------------------------------------------------------------------
	DWORD EventHandler(tagGUIEvent* pEvent);

	DWORD OnEventFamilyOpen( tagGameEvent* pGameEvent );
	DWORD OnEventFamilyClose( tagGameEvent* pGameEvent );
	DWORD OnEventGetName( tagFamilyNameEvent* pGameEvent );

private:
	TSFPTrunk<FamilyFrame>			m_trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<GameFrameMgr>       m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;
	// common----------------------------------------------------------------------------------------------------
	GUIWnd*										m_pWnd;
	GUIWnd*										m_pWndCaption;
	GUIPushButton*							m_pPbnState;		// 家族成员状态
	GUIPushButton*							m_pPbnSprite;		// 家族妖精
	GUIPushButton*							m_pPbnQuest;		// 家族任务
	
	GUIButton*								m_pClose;
	GUIStatic*								m_pStcFamilyName;
	GUIStatic*								m_pStcLeaderName;
	GUIStatic*								m_pStcMenbersNum;
	GUIStatic*								m_pStcTodayActiv;
	
	// 家族成员----------------------------------------------------------------------------------------------------
	FamilyMemberFrame*						m_pWndMember;		//任务面板
	// 家族任务----------------------------------------------------------------------------------------------------
	FamilyQuestFrame*						m_pFamilyQuestFrame;
	// 家族妖精----------------------------------------------------------------------------------------------------
	FamilySpriteFrame*						m_pSpriteFrame;
};
