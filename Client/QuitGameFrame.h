#pragma once
#include "gameframe.h"


class StaticEx;
struct tagNS_ReturnRoleSelect;
//-----------------------------------------------------------------------------
//!\brief loading frame
//!
//!
//-----------------------------------------------------------------------------
class QuitGameFrame : public GameFrame
{
public:
	QuitGameFrame();
	~QuitGameFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

private:
	TSFPTrunk<QuitGameFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	GUIWnd*						m_pWnd;
	GUIStatic*					m_pWndText;				//倒计时
	GUIButton*					m_pBtnGodCat;			//打开精灵猫
	GUIButton*					m_pBtnQuit;				//退出游戏
	GUIButton*					m_pBtnCancel;			//取消
	GUIButton*					m_pBtnClose;			//关闭按钮

	StaticEx*					m_pStcBackImg;			//背景图片
	GUIButton*					m_pBtnOne;				//adv 1
	GUIButton*					m_pBtnTwo;				//adv 2
	GUIButton*					m_pBtnThree;			//adv 3

	DWORD						m_dwStartTime;
	BOOL						m_bSendMsg;

	DWORD						m_dwSwitchTime;			//切换图片的时间点
	int							m_nBkImgIndex;			//显示图片的索引
	std::vector<tstring>		m_vecImgs;				//背景图片

	BOOL EventHandler(tagGUIEvent* pEvent);
	VOID Logout();
};
