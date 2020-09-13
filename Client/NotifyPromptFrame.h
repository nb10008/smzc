#pragma once
#include "gameframe.h"
#include "QuestMgr.h"


/** 分析服务器的消息，控制此模块是否被弹出，主要用于服务器对特殊活动的广播
	\brief 客户端由脚本控制此模块
*/
class NotifyPromptFrame : public GameFrame
{

public:
	NotifyPromptFrame(void);
	~NotifyPromptFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();

	VOID Show( const tstring& strContent );

private:
	BOOL EventHandler( tagGUIEvent* pEvent );
	VOID Navigate( tagGUIEvent* pEvent );
	
	
private:
	TSFPTrunk<NotifyPromptFrame>		m_Trunk;
	TObjRef<GUISystem>					m_pGUI;
	TObjRef<Util>						m_pUtil;
	TObjRef<GameFrameMgr>				m_pMgr;

	vector<tagIDLink>					m_vecIdLinks;			//NPC导航的链接
	vector<tagPosLink>					m_vecPosLinks;			//位置导航的链接

	GUIWnd*								m_pWnd;					//主窗口
	GUIButton*							m_pOk;					//确定
	GUIStaticEx*						m_pStcContent;			//显示内容的面板
	GUIScrollBar*						m_pScrollBar;			//滚动条
};
