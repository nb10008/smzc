#pragma once

struct tagNS_PvPResultInfo;
struct tagNS_PvPGetPvPState;
struct tagNS_PvPNotifyKill;
struct tagNS_PvPRanRoomResultInfo;
struct tagNS_PvPSinglePlayerInfo;
struct tagNS_RemoteAttChangeSingle;
struct tagNS_PvPEnterPvP;
class ActiveProgress;

class IconStatic;
enum EPvPPlayerState;

#define MAX_RANDOM_PVP_MUN 6
#define	ENEMY_NUM 3

struct tagPvPResultPoint
{
	GUIStatic*		pName;			// 姓名
	GUIStaticEx*	pRongYu;		// 荣誉
	GUIStaticEx*	pRongYuChange;	// 荣誉变化
	GUIStatic*		pKills;			// 多少斩
	GUIStaticEx*	pSuccessRate;	// 胜率
};


struct tagPvPEnemy
{
	DWORD			dwPlayerID;
	INT				nCurHP;
	INT				nMaxHP;
	EPvPPlayerState eState;
	IconStatic*		pPic;	// 图片
	GUIStatic*		pName;	// 姓名
	ActiveProgress*	pHP;	// 血量
	GUIStatic*		pAllBack;
	GUIStatic*		pJobBack;
	//bool			bDead;
};

class PvPStateFrame: GameFrame
{
public:
	PvPStateFrame(void);
	~PvPStateFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	VOID SelectPlayer(DWORD dwPlayerID);
protected:
	DWORD RstWndEventHandler(tagGUIEvent* pEvent);
	DWORD OnCloseMap(tagGameEvent* pGameEvent);
	DWORD OnOpenMap(tagGameEvent* pGameEvent);

	// 处理显示战斗结果
	DWORD NetRecvResultInfo(tagNS_PvPResultInfo* pMsg, DWORD); 
	// 处理战斗状态
	DWORD NetRecvPvPState(tagNS_PvPGetPvPState* pMsg, DWORD); 
	// 处理杀人提醒
	DWORD NetRecvNotifyKill(tagNS_PvPNotifyKill* pMsg, DWORD);
	// 随机竞技场结果
	DWORD NetNS_PvPRanRoomResultInfo(tagNS_PvPRanRoomResultInfo* pMsg, DWORD);

	DWORD NetNS_PvPSinglePlayerInfo(tagNS_PvPSinglePlayerInfo* pMsg, DWORD);

	/** 远程角色单项属性变化 这里主要用这个来变化血条
	*/ 
	DWORD NetRemoteAttChangeSingle(tagNS_RemoteAttChangeSingle* pNetCmd, DWORD);

	// 开始战斗操作
	DWORD NetNS_PvPEnterPvP(tagNS_PvPEnterPvP* pMsg, DWORD);	

	void UpdateRstInfo(TCHAR* pMsg);
	void UpdateRanPvPRstInfo(TCHAR* pMsg);
	void ShowPvPState(DWORD redCount, DWORD greenCount);
	void HidePvPState();
	void ShowKillUser(DWORD nNum);
	void HideKillUser();
	DWORD EventHandler(tagGUIEvent* pEvent);

	void DeadCheck();
protected:
	TSFPTrunk<PvPStateFrame>				m_Trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>						m_pCmdMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<GUIRender>						m_pRender;

	GUIWindow*								m_pWndResult;
	GUIButton*								m_pBtnLeave;
	GUIButton*								m_pBtnClose;
	GUIStatic*								m_pStaticTime;
	GUIListBox*								m_pLstTeamRst;
	GUIListBox*								m_pLstEnemyRst;
	GUIStatic*								m_pStaticGain;
	GUIStatic*								m_pStaticTotal;
	DWORD									m_dwTime;

	GUIWindow*								m_pWndKill;
	IconStatic*								m_pStaticKillNum;
	DWORD									m_dwStartToShow;

	GUIWindow*								m_pWndFriend;
	GUIStatic*								m_pStaticFriend;

	GUIWindow*								m_pWndEnemy;
	GUIStatic*								m_pStaticEnemy;

	GUIWnd*									m_pFinalPointsWin;
	IconStatic*								m_pResultPic;
	tagPvPResultPoint						m_PvPResultWin[MAX_RANDOM_PVP_MUN];
	GUIStatic*								m_pLeftNum;		//打了多少场
	GUIButton*								m_pBnHide;
	GUIStatic*								m_pLeftTime;	//倒计时时间

	GUIWnd*									m_pWndEnemyState;
	GUIButton*								m_pBnMin;
	IconStatic*								m_pNumLeftPic;	// 剩余人数
	tagPvPEnemy								m_PVPEnemy[ENEMY_NUM];
	DWORD									m_dwEnemy[ENEMY_NUM];	// 记录玩家ID，对应上面的m_PVPEnemy[ENEMY_NUM];

	GUIWnd*									m_pWndEnemyStateMin;
	GUIButton*								m_pBnMax;
	IconStatic*								m_pNumPic;		// 剩余人数

	
	GUIWnd*									m_pWndKillNum;
	IconStatic*								m_pKillNumPic;		// 多少斩
	DWORD									m_dwKillShowTime;
	bool									m_bShow;

	bool									m_bStartDeadCheck;
};