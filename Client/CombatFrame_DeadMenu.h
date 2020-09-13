#pragma once
#include "gameframe.h"
#include "..\WorldDefine\msg_combat.h"

/** \class CombatFrame_DeadMenu
	\brief 死亡菜单
*/
class IconStatic;
class MallButton;
struct tagUpdateLPItemEvent;

class CombatFrame_DeadMenu :
	public GameFrame
{
public:
	CombatFrame_DeadMenu(void);
	virtual ~CombatFrame_DeadMenu(void);

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();

	VOID  ShowTool();

	DWORD NetRecvNS_RoleRevive(tagNS_RoleRevive* pMsg, DWORD dwParam);
	DWORD NetRecvNS_RoleReviveNotify(tagNS_RoleReviveNotify* pMsg, DWORD dwParam);
	DWORD NetRecvCityStruggleRevive(tagNS_CityStruggleRevive *pMsg, DWORD dwParam);
private:
	BOOL  EventHandler(tagGUIEvent* pEvent);
	DWORD OnPvPDeadMenuConfirmMsgBox(tagMsgBoxEvent* pEvent);
	DWORD RefreshDeadMenu( tagGameEvent* pGameEvent);
	DWORD OnUpdateLPItem(tagUpdateLPItemEvent* pGameEvent);
private:
	TSFPTrunk<CombatFrame_DeadMenu>			m_Trunk;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;

	GUIWnd*						m_pWnd;				//主界面
	GUIButton*					m_pBtnLocalRise;	//原地复活
	GUIButton*					m_pBtnAcceptRise;	//接受复活
	GUIButton*					m_pBtnBackRise;		//回城复活
	GUIStatic*					m_pStcHint;			//提示
	GUIButton*                  m_pBtnPerfectRise;  //完美复活

	//复活道具
	GUIStatic*					m_pStcRorReborn;
	IconStatic*					m_pRebornBack;		//item背景
	MallButton*					m_pReborn;			//item按钮
	GUIStatic*					m_pStcMoney;		//元宝数量
	GUIButton*					m_pBtnRebornBuy;	//购买按钮
	GUIStatic*					m_pStcRebornRemain; //剩余个数
	//替身wawa
	GUIStatic*					m_pStcRorScapegoat;
	IconStatic*					m_pScapegoatBack;		//item背景
	MallButton*					m_pScapegoat;			//item按钮
	GUIStatic*					m_pStcScapegoatMoney;	//元宝数量
	GUIButton*					m_pBtnScapegoatBuy;	    //购买按钮
	GUIStatic*					m_pStcScapegoatRemain;  //剩余个数

	bool						m_bUpdateTime;		// 是否需要刷新复活时间
	int							m_nReviveTime;		// 复活所需时间
	BOOL						m_bRefresh;

	int						    m_nReviewWite;		// 复活等待时间
	bool						m_bIsWar;			// 是否在守护地图里~
};
