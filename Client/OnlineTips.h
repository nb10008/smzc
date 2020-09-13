#pragma once

/*************
\!brief (40级以下)角色登陆提示界面
*************/
#include "..\WorldDefine\role_att.h"

class FlashButton;
class OnlineTips : public GameFrame
{
public:
	OnlineTips();
	~OnlineTips();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	BOOL IsShow() const { return m_bShow; }
	void ShowWnd(BOOL bShow);
	

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	DWORD OnRefreshTips(tagGUIEvent *pEvent);
	DWORD NetRecvServerOLInfo(tagNS_SyncOnlineNum* pNetCmd, DWORD dwParam);
	void RefreshNotCompleteQuestsNum();
	void RefreshCanAssignAttNum();
	void RefreshCanAssignTalentNum();
	void RefreshBBYuanBaoNum();
	void RefreshOLNum();

private:
	TSFPTrunk<OnlineTips> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<NetCmdMgr>	 m_pCmdMgr;
	TObjRef<GameFrameMgr> m_pGameFrameMgr;
	TObjRef<Util>					m_pUtil;

	GUIWnd *m_pWnd;
	GUIButton *m_pBtnNotCompleteQuestsNum;	// 未完成任务数量
	FlashButton *m_pBtnCanAssignAttNum;		// 可分配属性点
	FlashButton *m_pBtnCanAssignTalentNum;	// 可分配天资点
	GUIButton *m_pBtnBBYuanBaoNum;			// 百宝袋元宝数量
	FlashButton *m_pBtnTodayFocus;            // 今日热门活动
	bool      m_bFirstClick;              //首次点击今日热门活动
	GUIButton *m_pBtnTaskQuery;				//固定任务
  	GUIButton *m_pBtnClose;					// 关闭

    GUIStatic *m_pStcFriendsOL;            //在线好友
	GUIStatic *m_pStcGuildMemberOL;            //在线同帮
    GUIStatic *m_pStcClassMateOL;             //在线师门
	INT			m_iFriendsNumOL;            //在线好友人数
	INT			m_iGuildMemberNumOL;			 //在线同帮人数
	INT			m_iClassMateNumOL;				//在线师门人数	

	int m_nLastAttNum;					// 上次的属性点
	int m_nLastTalentNum;				// 上次的天资点

	GUIButton* m_BnFriendsOnline;
	GUIButton* m_BnGuildOnline;
	GUIButton* m_BnTeacherOnline;

	BOOL m_bShow;
};

