#pragma once

struct tagPlayerPopupMenu;
struct tagRoleRenameEvent;
struct tagPvPRoomMenuEvent;

class PopUpMenuFrame : public GameFrame
{
	//右键菜单的现实类型，以方便不同右键菜单的扩展
	enum EShowPopUpMenuType
	{
		ESPPT_Null,
		ESPPT_Player,
		ESPPT_PvPRoomPos,
	};

	//对玩家菜单有效的菜单项枚举
	enum EMenuItemPlayer
	{
		EMIP_PrivateChat,       // 私    聊
		EMIP_Friend,            // 好    友
		EMIP_Trade,             // 交    易
		EMIP_Follow,            // 跟    随
		EMIP_TeamInvite,        // 组队邀请
		EMIP_FactionInvite,     // 帮派邀请
		EMIP_PlayerInfo,        // 玩家信息
		EMIP_AddToBlackList,    // 加黑名单
		EMIP_PetTrade,          // 灵兽交易
		EMIP_MountInvite,		// 邀请驾御
		EMIP_TeamApply,         // 申请入队
		EMIP_PlayerInfo2,		// 玩家名帖
		EMIP_MiniChat,          // 密语
        EMIP_ApprenticeToMaster,// 申请拜师
        EMIP_ToHaveApprentice,  // 收为徒弟
		EMIP_ApplySpritePart,  // 申请妖精配对
		EMI_BeautyFriends,		// 装扮好友
		EMI_InviteFamily,		//家族邀请
		EMI_ApplyFamily,		//家族申请
		EMIP_Num,
	};
public:
	PopUpMenuFrame( void );
	~PopUpMenuFrame( void );

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL Destroy();
	virtual BOOL EscCancel();

private:
	
	/** 事件处理函数
	*/
	BOOL EventHandler(tagGUIEvent* pEvent);

	/**  根据右键菜单的显示类型，把现实任务分发到适当的方法
	*/
	void UpdateMenuItem();

	/** 设置角色的右键菜单的现实内容
	*/
	void UpateMenuPlayer();

	/** 设置PvP的右键菜单的现实内容
	*/
	void UpdateMenuPvP();
	
	/**	处理角色右键菜单的选择事件
	*/
	void ProcessPlayerEvent( DWORD dwMenuItemIndex );

	/**	处理PvP右键菜单的选择事件
	*/
	void ProcessPvPEvent( DWORD dwMenuItemIndex );
	
	/** 根据显示的条目数改变菜单外观的大小
	*/
	VOID ForceResizeMenu( GUIListBox* pListBox, const INT nNumRow );
	
	/** 设置菜单项的显示内容
	*/
	VOID SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nLbIndex, const INT nStrIndex );
	
	/** 设置菜单项的显示内容
	*/
	VOID SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nNumRow, const INT nStartRow, const INT nEndRow, const BOOL bAutoResize );

	/** 隐藏菜单并设置显示模式为 null
	*/
	void ResetMenu();

	/** 将菜单移动到指定位置
	*/
	void ForceMoveTo(tagPoint& pt );

	DWORD OnPlayerPopUpMenu( tagPlayerPopupMenu* pEvent );

	DWORD OnPvPPopUpMenu( tagPvPRoomMenuEvent* pEvent );

	/** 
		\根据光标所在点到菜单的距离判断要不要关闭菜单
		\param pt 光标点
		\param ptView 菜单左上角点
		\param ptSize 菜单大小
		\param dist 距离大于这个就要关闭
	*/
	BOOL NeedClose( tagPoint pt, tagPoint ptView,tagPoint ptSize, LONG dist );

	VOID UpdateFashionData();

private:
	
	TSFPTrunk<PopUpMenuFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIWnd*						m_pMenu;						//菜单主界面
	GUIListBox*					m_pListBox;						//菜单项列表
	GUIWnd*						m_pWnd_SuitMenu;				// 时装菜单窗口
	GUIListBox*                 m_pLb_SuitMenu;					// 时装菜单列表框
	EShowPopUpMenuType			m_eShowType;					//当前右键菜单的现实类型
	INT							m_nMenuPanelHeightExt;			//菜单边框

	DWORD						m_dwCurShowRoleID;				//当玩家菜单被显示时，表示玩家的ID
	tstring						m_strRoleName;					//角色的名字
	INT							m_MenuItemPlayer[EMIP_Num];		//玩家右键菜单项

};
