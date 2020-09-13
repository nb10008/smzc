#pragma once

class MiniMapStatic;
struct tagLeaveMapAreaEvent;
struct tagEnterMapAreaEvent;
struct tagSetFlagEvent;
struct tagMouseMoveFinishEvent;
struct tagMouseMoveEvent;
struct tagNS_CanGoToPos;
class  FlashButton;
struct tagSaveTime                                           //临时记录时间的结构
{
	INT 				savehour;
	INT					saveMin;
	tagSaveTime():		savehour(0), saveMin(0){}
};
/** \class MiniMapFrame
    \brief 小地图界面
*/
class MiniMapFrame : public GameFrame
{
	//friend class CommKeyCode;//add by xp
public:
	MiniMapFrame();
	~MiniMapFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();
private:
	VOID SafeDestroyUI();
	BOOL OnWnd_MaxSize(tagGUIEvent* pEvent);				      //最大化窗口事件响应函数
	BOOL OnWnd_MiniSize(tagGUIEvent* pEvent);				      //最小化窗口事件响应函数
	BOOL OnWnd_Set(tagGUIEvent* pEvent);				          //小地图配置窗口事件响应函数
	BOOL OnWnd_PosQuery(tagGUIEvent* pEvent);					  //手动输入坐标窗口事件相应函数
	VOID ResetMapData();
	/** \游戏事件响应
	*/
	DWORD RefreshQueryBtnFlash(tagGameEvent*);
	DWORD OnOpenMap(tagGameEvent*);
	DWORD OnLeaveMapArea( tagLeaveMapAreaEvent* pEvent );
	DWORD OnEnterMapArea( tagEnterMapAreaEvent* pEvent );
	DWORD OnSetFlagEvent( tagSetFlagEvent* pEvent );
	DWORD OnMouseMoveFinishEvent( tagMouseMoveFinishEvent* pEvent );

	/** \热键相应函数
	*/
	DWORD OnZoomIn_HotKey( DWORD dwID, BOOL bDown );
	DWORD OnZoomOut_HotKey( DWORD dwID, BOOL bDown );

	DWORD OnNS_CanGoToPos(tagNS_CanGoToPos* pNetCmd, DWORD dwParam);//网络回应手动输入鼠标事件

	INT EGStoEMME( const INT nEGS ) const;


	bool  CompareTime(int SrcHour, int SrcMinute, int DestHour, int DestMinute); //比较是否为活动时间(for 固定活动按钮)
	VOID  TaskQueryBtnFlash();		//固定活动按钮 闪烁提醒玩家

	VOID  StartPathfinding();

private:
	TSFPTrunk<MiniMapFrame>	        m_Trunk;
	TObjRef<GameInputMap>			m_pKeyMap;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<GameFrameMgr>			m_pGameFrameMgr;

	GUIWnd*                         m_pWnd_MaxSize;               //小地图最大化窗口
	MiniMapStatic*                  m_pMSt_MiniMap;               //小地图控件
	GUIButton*                      m_pBn_PosGoTo;                //手动输入坐标寻路界面按钮
	GUIStatic*                      m_pSt_MapName;                //地图名字标签
	GUIStatic*                      m_pSt_PosX;                   //玩家X坐标标签
	GUIStatic*                      m_pSt_PosZ;                   //玩家Z坐标标签
	GUIButton*                      m_pBn_MiniSize;               //最小化按钮
	GUIButton*                      m_pBn_Setting;                //小地图配置按钮
	GUIButton*                      m_pBn_AreaMap;                //区域地图按钮
	tagSaveTime                     m_clickTaskQueTime;			  //记录点击活动任务查询按钮的时间
	FlashButton*                    m_pBn_TaskQuery;              //活动任务查询按钮
	GUIButton*                      m_pBn_TaskTrace;              //任务追踪界面按钮
	GUIButton*                      m_pBn_ZoomIn;                 //放大按钮
	GUIButton*                      m_pBn_ZoomOut;                //缩小按钮
	GUIButton*						m_pBn_VipStall;				  //VIP摊位
	GUIButton*						m_pBn_OnlineTip;			  //上线提示
	GUIButton*						m_pBn_CityWarInfo;			  // 城站信息
	//GUIPushButton*					m_pPBtn_ContinuteSkill;		  //连接技开关   //delete by lib 跟显示隐藏信仰按钮冲突
	GUIButton*                      m_pBn_StyleAction;            //个性动作按钮
	GUIButton*						m_pBtnQuestHelp;			  //帮助
	GUIButton*						m_pBtnStrengthRanking;		  // 实力排行
	GUIButton*						m_pBtnKilling;				  // 击杀积分榜
	GUIButton*						m_pBtnShowHideDialyExp;		  //显示隐藏每日信仰
	GUIButton*						m_pBtnPaidashi;					//爱拍按钮

	GUIWnd*                         m_pWnd_MiniSize;              //小地图最小化窗口
	//GUIStatic*                      m_pSt_MapName_MiniSize;       //最小化窗口中的地图名字标签
	//GUIStatic*                      m_pSt_PosX_MiniSize;          //最小化窗口中的X坐标标签
	//GUIStatic*                      m_pSt_PosZ_MiniSize;          //最小化窗口中的Z坐标标签
	GUIButton*                      m_pBn_MaxSize;                //最大化按钮

    //GUIButton*                      m_pBt_PetSkillOnline;         // 灵兽内挂 //<!F计划中该功能废弃

	GUIWnd*                         m_pWnd_Set;                   //小地图配置窗口
	GUIPushButton*                  m_pBn_EleShow[EMME_Num-1];      //是否显示按钮
	GUIButton*                      m_pBn_Ok_Set;                 //确定
	GUIButton*                      m_pBn_Cancel_Set;             //取消
	GUIButton*                      m_pBn_Close_Set;              //关闭

	GUIWnd*                         m_pWnd_PosQuery;                   //手动输入坐标查询窗口
	GUIButton*                      m_pBn_Pos_Ok_Set;                 //确定
	GUIButton*                      m_pBn_Pos_Cancel_Set;             //关闭
	GUIEditBox*						m_pEd_PosX;						 //X坐标输入框
	GUIEditBox*						m_pEd_PosZ;						//Z坐标输入框

	tagPoint                        m_ptLocalPlayerPos;
	DWORD							m_dwSpaceTime;

	GUIButton*                      m_pBnFaceBook;                 //确定
	GUIButton*                      m_pBnPickStrategy;                 //拾取模式
	GUIButton*                      m_pBnSinaWeiBo;                  //新浪微博
	GUIButton*                      m_pBnSinaWeiBoCard;         //新浪微博卡
};
