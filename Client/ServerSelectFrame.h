#pragma once

#define RECOMMEND_GAMEWORLD 8
#define BOROUGH_MAX			12
#define GAMEWORLD_MAX		24
#define BOROUGH_MAX_KOREA		14
#define GAMEWORLD_MAX_KOREA		14

class IconStatic;

enum EServerState
{
	ESS_NULL = 0,
	ESS_New,
	ESS_Hot,
	ESS_All
};

struct tagGameWorld											//游戏世界
{
	DWORD	dwWorldRank;									//游戏世界的编号
	TCHAR	szWorldName[X_SHORT_NAME];						//游戏世界的名
	TCHAR	szBoroughName[X_SHORT_NAME];					// 大区名
	TCHAR	szRecommendName[X_SHORT_NAME];					// 推荐服务器名
	DWORD	dwWorldID;										//大区ID
	INT		nStatus;										//游戏世界的状态				
	DWORD	dwStatus;										//状态的颜色
	DWORD	dwWorldIP;										//游戏世界的IP
	DWORD	dwWorldPort;									//游戏世界的port
	BYTE	byRecommend;									//是否为推荐游戏世界
	DWORD	dwRecoRank;										//推荐游戏世界的编号
	BYTE	byGreenFlag;									//绿色标记
	EServerState    eState;
	TCHAR	szIcon[5][128];									//图标
	TCHAR	szTips[5][128];									//tips	
	TCHAR	szHttp[5][128];									//链接
};

struct tagBorough											//大区
{
	DWORD					dwBoroughRank;					//游戏世界的编号
	TCHAR					szBoroughName[X_SHORT_NAME];	//大区名
	DWORD					dwBoroughID;					//大区ID
	INT						dwWorldNum;						//大区下游戏世界个数
	vector<tagGameWorld>	vGameWorld;						//大区下游戏世界
};

struct tagServerState
{
	TCHAR szIcon[128];
	TCHAR szTips[128];
	TCHAR szHttp[128];
};

/** 大区、服务器选择
*/

class IconStatic;
class Client;
class ServerSelectFrame : public GameFrame
{
public:
	ServerSelectFrame();
	~ServerSelectFrame();

	//--GameFrame
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	BOOL OnFreeStyleWnd(tagGUIEvent* pEvent);
	// 得到服务器返回的游戏世界状态颜色
	DWORD GetWorldStatusColor(const INT nWorldStatus);

	// 计算日期
	void CalDate();

	//确认登录
	void ConfirmLogin();

	//帐号注册
	void AccountRegister();

	//帐号充值
	void AccountAbundance();

	//退出游戏
	void ExitGame();

	//读取大区游戏世界的配置文件
	void LoadConfigFile();

	//根据服务器得到IP  alex 2010-06-17 
	void GetWorldIDByWorldName(tstring strWorldName,tstring& strIp, tstring& strPort, tstring& strWorldID);

	//读取上次登录过的游戏世界
	void LoadLastLogin();

	//设置推荐游戏世界状态
	void SetRecommedPushButtonState();

	//设置游戏世界按钮状态
	void SetWorldPushButtonState();

	//显示游戏世界按钮
	void ShowWorldPushButton();

	//显示今天\明天的活动任务
	void ShowServerActive(); 

	// 初始化登陆模式
	void InitLoginMode();

	INT GetBorough( INT nGameWorld );
	INT GetWolrd( INT nGameWorld );

	VOID GetSpecialValue();


private:
	TSFPTrunk<ServerSelectFrame>m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<StreamTransport>	m_pStream;
	TObjRef<VarContainer>		m_pVarContainer;
	TObjRef<VarContainer>		m_pVarIPSaver;
	TObjRef<Client>				m_pClient;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;
	GUIPushButton*				m_pRecommend[RECOMMEND_GAMEWORLD];	//推荐游戏世界按钮
	IconStatic*					m_pRecIconHot[RECOMMEND_GAMEWORLD];
	IconStatic*					m_pRecIconNew[RECOMMEND_GAMEWORLD];
	GUIPushButton*				m_pBorough[BOROUGH_MAX];			//大区按钮
	GUIPushButton*				m_pGameWorld[GAMEWORLD_MAX];		//游戏世界按钮
	IconStatic*					m_pWorldIconHot[GAMEWORLD_MAX];
	IconStatic*					m_pWorldIconNew[GAMEWORLD_MAX];
	GUIPushButton*				m_pLastLogin;						//上次登录的游戏世界按钮		
	GUIStatic*					m_pLastServer;						//上次登录的大区
	GUIButton*					m_pOK;								//确认登录
	GUIButton*					m_pRegister;						//注册
	GUIButton*					m_pAbundance;						//充值
	GUIButton*					m_pExit;							//离开
	//GUIListBox*				m_pToday;							//今日活动
	//GUIListBox*				m_pTomorrow;
	IconStatic*					m_pStcTip[20];						//15个tip
	GUIButton*					m_pBtnHttp;							//打开网页按钮

	GUIWnd*						m_pWnd_Back;						//大背景
	GUIStatic*					m_pStc_Pic;							//背景贴图

	INT							m_nBorough;							//选择的大号编号
	INT							m_nGameWorld;						//选择的游戏世界编号
	INT							m_nRecoWorld;						//选择的推荐游戏世界编号
	vector<tagBorough>			m_vBrough;							//大区
	vector<tagGameWorld>		m_vRecommend;						//推荐的游戏世界
	vector<tagServerState>		m_vTip;								//三个推荐服对应15个提示
	TCHAR						szButtHppt[128];					//按钮链接		

	tstring						m_strRegister;						//注册帐号的网址
	tstring						m_strAbundance;						//帐号充值的网址

	tstring						m_strLastIP;						//上次登录的ip
	DWORD						m_dwLastPort;						//上次登录的port
	tstring						m_strLastWorldName;					//上次登录的游戏世界
	tstring						m_strLastServer;					//上次登录的大区

	tstring						m_strGameWorld;						//选择的游戏世界名
	tstring						m_strIP;							//选择的游戏世界IP
	DWORD						m_dwPort;							//选择的游戏世界Port
	
	INT							m_nLoginMode;						// 登陆模式
	bool						m_bOnlyGameWord;					// 不显示大区，只显示游戏世界

	bool                      m_bIfKoreaServer;                  //是否是韩国服务器（不显示推荐游戏世界,且界面有变化。）

	//和谐游戏相关
	GUIWnd*						m_pFreeStyleWnd;                  //自由对战窗口
	GUIPushButton*				m_pPBFreeStyleChoose;				//勾选自由对战
	GUIButton*					m_pBtnChoose;						//确定或退出按钮

	INT m_nLauncherValue;
	INT m_nDispValue;

	GUIWnd*						m_p360Wnd;                  //360
	GUIButton*					m_pBn360On;
	GUIButton*					m_pBn360Off;
	GUIPushButton*				m_pBn360Again;

	GUIStatic*					m_pStcKoreaPic;
	DWORD						m_dwKoreaPicShowTime;
	BOOL						m_bKoreaEnterGame;

private:
	BOOL						m_bYunLianOn;
};