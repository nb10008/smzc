#pragma  once 

#include "../WorldDefine/msg_master_apprentice.h"

class MasterFrame : public GameFrame
{
	// 便捷菜单
	enum EMenuItem
	{
		EMI_PlayerInfo,			// 玩家资料
		EMI_Chat,				// 设为私聊
		EMI_AddFriend,			// 加为好友
		EMI_InviteGroup,		// 邀请组队
		EMI_ApplyGroup,			// 申请组队
		EMI_Num,
	};
public:
	MasterFrame();
	~MasterFrame();

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL ReloadUI();
	virtual BOOL Destroy();
	virtual BOOL EscCancel();
	void ShowFrame();

	// 使用授业点数界面
	void ShowUseWnd();
private:
	DWORD EventHandler( tagGUIEvent *pEvent );
	void ShowMaster();
	void ShowStudent();

	// 移动便捷菜单
	VOID MoveMenu( GUIListBox* pListBox, const tagPoint& ptDest );
	// 更新便捷菜单
	VOID UpdateMenu();
	// 改变便捷菜单的大小
	VOID ResizeMenu( GUIListBox* pListBox, int row, int width );

	// 获取离线时间显示
	tstring GetOfflineTime( bool bIsOnline, bool bMasterMask, DWORD &dwColor, DWORD dwOfflineTime = GT_INVALID );


	// 查看我的师门界面
	DWORD OnNetGetMasterInfo(tagNS_MatesInfo *pNetCmd, DWORD);
	// 查看我的徒弟界面
	DWORD OnNetGetStudentInfo(tagNS_ApprenticeInfo *pNetCmd, DWORD);
	// 使用授业点数
	DWORD OnNetUseJingWuPoint(tagNS_UseJingWuPoint *pNetCmd, DWORD);
	// 分配授业点数消息
	DWORD OnNetAssignJingWuPoint( tagNS_GiveJingWuPoint *pNetCmd, DWORD );

	// 显示分配授业点数界面
	void ShowAssignWnd();
	// 是否可以分配授业点数
	bool CanAssignJingWuPoint();
	// 是否可以使用授业点数
	bool CanUseJingWuPoint();

	// 逐出师门
	DWORD OnNS_BreakOutApprentice( tagNS_BreakOutApprentice *pNetCmd, DWORD );
	// 逐出师门
	DWORD OnNS_BreakOutApprenticeConfirm( tagNS_BreakOutApprenticeConfirm *pNetCmd, DWORD );
	// 逐出师门确认
	DWORD OnLeaveApprentice( tagMsgBoxEvent *pGameEvent );

	// 脱离师门
	DWORD OnNS_BreakOutMaster( tagNS_BreakOutMaster *pNetCmd, DWORD );
	// 脱离师门
	DWORD OnNS_ForceBreakOutMaster( tagNS_ForceBreakOutMaster *pNetCmd, DWORD );
	// 脱离师门
	DWORD OnNS_BreakOutMasterConfirm( tagNS_BreakOutMasterConfirm *pNetCmd, DWORD );
	// 脱离师门确认
	DWORD OnLeaveMaster( tagMsgBoxEvent *pGameEvent );
	// 强行脱离师门
	DWORD OnForceLeaveMaster( tagMsgBoxEvent *pGameEvent );

private:
	TSFPTrunk<MasterFrame> m_Trunk;
	TObjRef<GUISystem> m_pGUI;
	TObjRef<GameFrameMgr> m_pFrameMgr;
	TObjRef<NetSession> m_pSession;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;

	GUIWnd *m_pWnd;									// 师徒主界面
	GUIPatch *m_pWndBack;
	GUIPatch *m_pMain;

	GUIFrameMutex *m_pTeaAndStu;					
	GUIPushButton *m_pBtnMaster;					// 我的师门
	GUIPushButton *m_pBtnStu;						// 我的徒弟

	GUIStatic *m_pStcName;							// 师尊名字
	GUIStatic *m_pStcLevel;							// 师尊等级
	GUIStatic *m_pStcNumOfQingWu;					// 师门授业点数
	GUIStatic *m_pStcNumOfQinWu;					// 师门师德点数

	GUIButton *m_pBtnName;							// 名字
	GUIButton *m_pBtnLevel;							// 等级
	GUIButton *m_pBtnTask;							// 师徒任务
	GUIButton *m_pBtnLastLogin;						// 上次在线时间
	GUIListBox *m_pList;							// 师门列表框

	GUIStatic *m_pStcInfo;							// 师门说明
	GUIButton *m_pBtnAssign;						// 分配或者使用授业点数
	GUIButton *m_pBtnLeave;							// 脱离师门
	GUIButton *m_pBtnClose;							// 关闭师门主界面
	GUIButton *m_pBtnQuit;							// 关闭师门主界面
	GUIButton *m_pBtnRule;							// 规则说明

	GUIWnd *m_pAssignWnd;							// 分配授业点数界面
	GUIStatic *m_pStcStuName;						// 弟子的名字
	GUIStatic *m_pStcJingWu;						// 升级需要的授业点数
	GUIEditBox *m_pEdtAssignJingWu;					// 分配给弟子的授业点数
	GUIStatic *m_pStcTotalJingWu;					// 可分配的授业点数
	GUIButton *m_pBtnOK;							// 确定分配
	GUIButton *m_pBtnCancle;						// 取消分配
	GUIButton *m_pBtnQuitA;							// 取消分配

	GUIWnd *m_pUseWnd;								// 使用授业点数界面
	GUIStaticEx *m_pStcUseInfo;						// 使用说明
	GUIButton *m_pBtnUseOK;							// 授业点数使用确定
	GUIButton *m_pBtnUseCancle;						// 授业点数使用取消

	GUIWnd *m_pMenuWnd;								// 便捷菜单界面
	GUIListBox *m_pMenuList;						// 便捷菜单列表框

	DWORD m_dwTotalRow;								
	DWORD m_dwClickRow;								// 当前选中的行
	DWORD m_dwRoleID;								// 当前选中弟子的ID

	GUIWnd *m_pRuleWnd;								// 规则说明
	GUIButton *m_pBtnRuleMaster;					// 拜师出师
	GUIButton *m_pBtnRuleMasterTask;				// 师徒任务
	GUIButton *m_pBtnRuleJingWu;					// 授业点数
	GUIButton *m_pBtnRuleQuit;						// 关闭

	vector<tagMatesAndApprenticeInfo> m_vecMyGuild;						// 我的师门
	vector<tagMatesAndApprenticeInfo> m_vecMyStudent;					// 我的徒弟
};