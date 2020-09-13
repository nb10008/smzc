#pragma once
#include ".\WorldFrame.h"
#include "..\WorldDefine\msg_quickbar.h"
class ButtonEx;
class ShortcutButton;
class DragBtn;


struct tagUpdateLPItemEvent;
struct tagUpdateLPSkillEvent;
struct tagRoleGetNameEvent;
struct tagShortcutCacheEvent;
struct tagRolePickEvent;
struct tagNS_VirginOnline;
struct tagNS_MsgQuickBarData;
struct tagQuickBarState;
struct tagNS_MsgIsSavedQuickBarData;
struct tagUpdateLPAttEvent;
struct tagUpdateLPSkillEvent;
//-----------------------------------------------------------------------------
// !class ShortcutBarFrame.h
// !brief 快捷栏框架
//-----------------------------------------------------------------------------
class ShortcutBarFrame :
	public GameFrame
{
public:
	ShortcutBarFrame(void);
	~ShortcutBarFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID OnEvent(tagGameEvent* pEvent);
	virtual VOID Update();

	//控件事件处理函数
	DWORD EventHandler(tagGUIEvent* pEvent);

	//加载快捷按钮
	BOOL LinkShortcutBtn(TCHAR* szName, int nBar);

	//创建扩展快捷按钮
	void CreateShortcutBtnEx(char* szName);

	//经验条刷新
	void RefreshRoleExp();

	//获得指定的快捷按钮
	ShortcutButton* GetShortcutButtion(int group, int num);

	void ReflashCDTime();
	void SetBtnEnable();

	// 保存快捷拦数据	
	void SaveBtnData();
    
	void SetIsChange(BOOL bIschange);
	//保存至服务器
	void SaveQuickBarToDB();

    // 自动添加新技能到主快捷栏栏位
    void AddNewSkill(DWORD dwTypeID);

private:
    // 角色初次登录设置快捷按钮
    DWORD OnNS_VirginOnline(tagNS_VirginOnline* pMsg, DWORD);
    // 角色登陆时读取数据库快捷栏数据
	DWORD OnNS_MsgQuickBarData(tagNS_MsgQuickBarData* pMsg, DWORD);
    // 服务器发来数据库是否有数的消息
	DWORD OnNS_MsgIsSavedQuickBarData(tagNS_MsgIsSavedQuickBarData* pMsg, DWORD);
	

	// 宠物放生时，删除相应的快捷方式
	DWORD OnDeletePet( tagGameEvent* pEvent );

	//刷新普通攻击图片
	DWORD OnWeaponSwap(tagGameEvent* pEvent);

	DWORD OnActiveHolySkillEvent(tagUpdateLPSkillEvent* pEvent);

	//显示/隐藏快捷按钮
	void ShowBar(int bar);
	void HideBar(int bar);

	DWORD OnKey(DWORD dwID, BOOL bDown);
	DWORD OnKeyChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnKeyEx(DWORD dwID, BOOL bDown);
	DWORD OnKeyExChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnKey2Move(DWORD dwID, BOOL bDown);
	DWORD OnKey2MoveChange(DWORD dwID, DWORD key1, DWORD key2);
	DWORD OnKey2Move1(DWORD dwID, BOOL bDown);
	DWORD OnKey2MoveChange1(DWORD dwID, DWORD key1, DWORD key2);

	DWORD OnKeyTransSkill(DWORD dwID, BOOL bDown);
	DWORD OnKeyTransSkillChangel(DWORD dwID, DWORD key1, DWORD key2);

	DWORD OnRolePickEvent(tagRolePickEvent* pEvent);
	DWORD OnInitLPAttEvent(tagGameEvent* pEvent);
	DWORD OnResetSkillIdleTime(tagGameEvent* pEvent);
	DWORD OnKeyUpDown(DWORD dwID, BOOL bDown);
	DWORD OnKey2MoveOpenOrClose(DWORD dwID, BOOL bDown);

	// 由DIK得到按键字符串名
	LPCTSTR GetKeyName(DIK key);

	//注册键盘
	void RegisterKeyBoard();

	//更新按钮的可用属性
	void UpdateBtnEnable();		

	//新手技能快捷栏处理
	BOOL AutoAdaptFreshHandSkill(ShortcutButton* pBtn, DWORD dwSkillID);

	// 与背包同步数据
	DWORD OnUpdateLPItemEvent(tagUpdateLPItemEvent* pEvent);
	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pEvent);
	DWORD OnInitedSkillEvent(tagGameEvent* pEvent);

	DWORD OnEventClearTalent(tagGameEvent* pGameEvent);

	// 
	DWORD OnShortcutCacheEvent(tagShortcutCacheEvent* pGameEvent);

	// 读取快捷拦数据	
	void LoadBtnData(DWORD dwLocalID);
	void ReflashShortcutBtn( bool bOnlySkill = false );

	DWORD OnRecvNetRoleGetName( tagRoleGetNameEvent* pMsg);

	void ShowMoveShortcutBar(BOOL bShow);
	void ShowMove1ShortcutBar(BOOL bShow);

	DWORD ShowAddAtt(tagGameEvent*);    //处理显示或隐藏属性和天资投点的小按钮
	DWORD HideAddAtt(tagGameEvent*);
	DWORD ShowAddTelent(tagGameEvent*);
	DWORD HideAddTelent(tagGameEvent*);

	DWORD OnUpdateLPAttEvent(tagUpdateLPAttEvent* pEvent);

	// 显示变身技能表
	DWORD ShowTransSkill(tagGameEvent*);
	// 隐藏变身技能表
	DWORD HideTransSkill(tagGameEvent*); 
private:

	TSFPTrunk<ShortcutBarFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameInputMap>		m_pKeyMap;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	
	static const int			BAR_COUNT = 11;					//快捷栏分组数
	static const int			BAR_BUTTON_COUNT = 8;			//每组快捷键个数
	static const int			TRANS_BUTTON_COUNT = 6;			//变身快捷键个数
	static const int			BAR_TITLE_COUNT = 12;
	static const int			BAR_COUNT_TIPS = 4;
	static TCHAR*				m_szAccArray[BAR_TITLE_COUNT+BAR_COUNT_TIPS];
	static int					m_nAccArray[ BAR_TITLE_COUNT+BAR_COUNT_TIPS ];
	static TCHAR*				m_szMoveArray[BAR_BUTTON_COUNT];
	static byte					m_byMoveArray[BAR_BUTTON_COUNT];
	static TCHAR*				m_szMoveArray1[BAR_BUTTON_COUNT];
	static int					m_nMoveArray1[BAR_BUTTON_COUNT];
	static TCHAR*				m_szTransSkill[TRANS_BUTTON_COUNT];
	static int					m_nTransSkill[TRANS_BUTTON_COUNT];
	static TCHAR*				m_szBarGroup[BAR_COUNT_TIPS];
	static byte					m_byBarGroup[BAR_COUNT_TIPS];
	static TCHAR*				m_szKeyUpDown[2];
	static int					m_nKeyUpDown[2];
	static TCHAR*				m_szKeyMoveOC[2];
	static int					m_nKeyMoveOC[2];
	int							m_nBarGroupCount;				//记录当前快捷栏组号
	FLOAT						m_fCurrentExp;					//当前经验
	DWORD						m_LastSuccSkill;

	GUIWnd*						m_pWnd;
	GUIStatic*					m_pPthBack;						//快捷栏背景
	GUIStatic*					m_pPthShortcutBack;				//快捷栏按钮背景
	GUIStatic*					m_pPthExpBack;					//经验条背景
	GUIProgress*				m_pProExp;						//经验条
	GUIStatic*					m_pStcChoose;					//快捷栏组提示F1-F4
	GUIButton*					m_pBtnMoveDn;					//移动快捷栏组切换向下切换按钮
	GUIButton*					m_pBtnMoveUp;					//移动快捷栏组切换向上切换按钮
	GUIButton*					m_pBtnDn;						//快捷栏组切换向下切换按钮
	GUIButton*					m_pBtnUp;						//快捷栏组切换向上切换按钮
  	GUIButton*                  m_pAddatt;
  	GUIButton*                  m_pAddTelent;

	tagQuickBarState            m_QuickBarFlags[BAR_COUNT][BAR_BUTTON_COUNT];      //快捷栏状态变化
	ShortcutButton*				m_pShortcutBtn[BAR_COUNT][BAR_BUTTON_COUNT];		//4组快捷按钮
	BOOL						m_bInitShortcut;				//是否读取过数据
	BOOL                        m_bIsLoadFromDB;                  //是否保存到数据
	INT                         m_nQuickBar_Col_Count;           //保存快捷栏第几行
	BOOL                        m_bHaveNotSavedQuickBarData;     //数据库是否存过快栏数据的标示
	BOOL                        m_bIsChange;                     //通知服务器存数据的开关
	BOOL                        m_bIsQuit;                       //是否退出过
	BOOL                        m_bIsReQuest;                    //是否请求过过去是否存过数据

	GUIWnd*						m_pWnd_Move1;
	GUIWnd*						m_pWnd_Move2;

	GUIWnd*						m_pWnd_TransSkill;

	INT64							m_nCurExp;						//当前经验
	INT							m_nCurLev;						//当前等级
	INT64							m_nCurTotalExp;					//下一次需要的经验
	INT							m_nPctNExp;						//N%经验达成
	
	bool						m_bLPAttIsOK;
	ShortcutButton				*m_pWndEvent;
	float						m_fTime;
	bool						m_bSend;
	DWORD				m_dwATTcheckTimer;
	//元神
	GUIButton*				m_pBtnHolyMen;

public:
	ShortcutButton*				m_pLastPress;					//上次按的快捷键

private:
	VOID WG_CheckingInPut();
private:
	DWORD						m_dwIdleTime;
	bool						m_bCheckOn;
	bool						m_bNetSessionStartCheck;

};
