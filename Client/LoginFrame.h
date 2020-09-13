//-----------------------------------------------------------------------------
//!\file LoginFrame.h
//!\author Lyp
//!
//!\date 2008-02-17
//! last 2008-04-15
//!
//!\brief login frame
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "..\Cool3D\Animation\AnimationCtrl.h"
#include "..\WorldDefine\selectrole.h"

#define  OPEN_PASSPORT_LOG		0
#define  OPEN_PASSPORT		1
const int  g_nUrlNum = 5;

//-----------------------------------------------------------------------------
//!\brief login frame
//!
//！		登录模式：0			内测使用
//！		登录模式：1			大陆
//！		登录模式：2			迅雷
//！		登录模式：3			台湾（橘子）
//！		登录模式：4			盛大
//！		登录模式：5			韩国
//！		登录模式：6			越南
//！		登录模式：7			台湾	OMG
//!			登录模式：8			新加坡  OMG	在登录模式0的基础上增加了保存密码的功能
//!			登录模式：9			豆客登录模式
//！		登录模式：10		俄罗斯
//!			登陆模式：11		韩国SM
//-----------------------------------------------------------------------------
class NetSession;
class Client;
struct tagNLS_GetTokenID;
class HttpPicEx;
struct tagWeb_URLInfo;
struct tagNLC_SafeProof;
struct tagURLInfo
{
	DWORD	dwID;
	string		strUrl;
	tstring		szDesc;
	HttpDownload*  pHttp;
	tagURLInfo(): dwID(GT_INVALID),strUrl(""),pHttp(NULL){}
};

class LoginFrame : public GameFrame
{
public:
	LoginFrame();
	~LoginFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Render3D();
	virtual VOID Update();
	
	//VOID SetPlayState(int nState) { m_nPlayState = nState; }

	VOID Connect(bool bUnrealLogin = TRUE);

	void SetEncrypt( const char* szEncry , const char* szUserName ); 
	void SetUserNameUI( const char* szUserName );
	void SetYY_GetTokenIDorNot(bool b){m_bAskTokenID =b; }

	DWORD OnMsgbox_KoreaLoginErr3Times(tagMsgBoxEvent* pGameEvent);

private:
	TSFPTrunk<LoginFrame>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<XClient>			m_pStream;
	TObjRef<VarContainer>		m_pVarContainer;
	TObjRef<NetSession>			m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<Client>				m_pClient;
	
	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndKey;
	//GUIStatic*					m_pWndCaption;
	//GUIStatic*					m_pWndPswHint;
	GUIStatic*					m_pStcAccountText;
	GUIStatic*					m_pStcPsswordText;
	GUIStatic*					m_pStcAccountBack;
	GUIStatic*					m_pStcPsswordBack;
	GUIEditBox*					m_pEditAccount;
	GUIEditBox*					m_pEditPassword;
	GUIButton*					m_pButtonLogin;
	GUIButton*					m_pButtonQuit;
	GUIButton*					m_pBtnReturn;
	GUIStatic*					m_pStcHint;				//错误提示
	GUIPushButton*				m_pPBtn;
	GUIPushButton*				m_pPBtnSaveAccount;
	GUIPushButton*				m_pPBtnSavePassword;
	GUIPushButton*				m_pPBtnCamera;
	GUIPushButton*				m_pPBtn_Tab;			//大小写切换
	GUIStatic*					m_pStcSavePass;
	//GUIStatic*					m_pStcUser;
	GUIEditBox*					m_pLastActive;
	GUIStatic*					m_pStcSavePasswordBack;

	GUIWnd*						m_pWndWorldList;		//服务器选择
	GUIStatic*					m_pStcWLHint;
	GUIButton*					m_pBtnOKWorld;
	GUIButton*					m_pBtnCancelWorld;
	GUIListBox*					m_pWorldList;

	GUIWnd*						m_pWndQueue;
	GUIStatic*					m_pStcTime;				//排队剩余时间
	GUIStatic*					m_pStcPosition;			//排队的位置

	GUIStatic*					m_pWndCancelQueue;		//取消排队界面
	GUIButton*					m_pBtnCancelQueue;		//取消排队
	GUIStatic*					m_pStcTips;				//建议提示

	GUIWnd*						m_pWndSecurity;			//密保
	GUIStatic*					m_pStcSecuHint;			//密保提示
	GUIEditBox*					m_pStcSecuNum;			//提示的数字

	GUIWnd*						m_pWndSeInputBack;		//密保输入背景
	GUIButton*					m_pBtnSecuInput[10];	//
	GUIStatic*					m_pStcInputHint;		//
	GUIButton*					m_pBtnSecuOK;			//
	GUIButton*					m_pBtnSecuCancel;	
	GUIStatic*					m_pStcWebLoginTips;		//三方登陆提示

	//韩国二次密码确认控件
	GUIWnd*						m_pWndKoreaPassWord;	//韩国二次密码设置
	GUIButton*					m_pBtnKoreaPassWordClose;//关闭
	GUIButton*					m_pBtnKoreaChangeWorld;	//韩国修改二次密码
	GUIButton*					m_pBtnKoreaLoginOK;		//韩国二次密码点击确定按钮，可能是登陆、确认等
	GUIButton*					m_pBtnKoreaLoginCancel;	//韩国二次密码取消按钮
	GUIButton*					m_pBtnKoreaWordKey[10];	//数字0～9
	GUIButton*					m_pBtnKoreaClear;		//清零按钮
	GUIButton*					m_pBtnKoreaBack;		//回退按钮
	//设置密码面板相关控件
	GUIWnd*						m_pWndFirstSetPassWord;	//第一次设置密码面板
	GUIStatic*					m_pStcKoreaFTip;		//那个提示信息框
	GUIStatic*					m_pStcFirstPassWord;	//第一次输入密码
	GUIStatic*					m_pStcSecondPassWord;	//第二次密码确认
	GUIStatic*					m_pStcFirstPassWordPic;	//选择第一次后显示的那个框
	GUIStatic*					m_pStcSecondPassWordPic;//选择第二次后显示的那个框
	//登陆面板
	GUIWnd*						m_pWndKoreaLogin;		//韩国二次密码登陆面板
	GUIStatic*					m_pStcKoreaLoginTip;	//登陆提示
	GUIStatic*					m_pStcLoginPassWord;	//登陆密码
	//修改密码面板
	GUIWnd*						m_pWndKoreaChangePassWord;	//韩国修改二次密码
	GUIStatic*					m_pStcChangeWordTip;		//修改密码提示
	GUIStatic*					m_pStcOldPassWord;			//旧密码
	GUIStatic*					m_pStcNewPassWord;			//新密码
	GUIStatic*					m_pStcNewPassWordConfirm;	//新密码确认
	GUIStatic*					m_pStcOldPassWordPic;		//旧密码底图
	GUIStatic*					m_pStcNewPassWordPic;		//新密码底图
	GUIStatic*					m_pStcNewPassWordConfirmPic;//新密码确认底图

	bool						m_bLoginType;			// 是否是命令行登陆方式

	string						m_strKoreaLoginNumber;	//韩国登陆密码
	string						m_strKoreaSetPassWordOne;//韩国设置二次密码第一次输入
	string						m_strKoreaSetPassWordTwo;//韩国设置二次密码第二次输入
	string						m_strKoreaChangePassWordOne;	//韩国修改二次密码第一次输入
	string						m_strKoreaChangePassWordTwo;	//韩国修改二次密码第二次输入
	string						m_strKoreaChangePassWordOld;	//韩国修改二次密码旧密码
	INT8						m_n8KoreaTryTimes;			//韩国登陆尝试次数

	VOID SetKoreaTimaOutError();		//韩国连接超时显示
	
	enum EMBPhase
	{
		EMBP_NULL	= -1,					// 开始
		EMBP_First	= 0,					// 密保第一阶段
		EMBP_Second	= 2,					// 密保第二阶段
		EMBP_Third	= 4,					// 密保第三阶段
		EMBP_End	= 6,					// 结束
	};

	enum ELoginState
	{
		ELS_WaitInput,
		ELS_Connect,
		ELS_WaitProofResult,
		ELS_MiBao,
		ELS_Queue,
		ELS_ConnectZoneServer,		// 连接区域服务器
		ELS_SentJoinGame,			// 发送JoinGame消息
		ELS_JoinGame,				// 接收到JoinGame消息后
		ELS_Launcher,				// 版本不对,启动更新程序
	};

	enum
	{
		Loading	= 0,				//0：读盘状态，
		Preface,					//1：播放开场动画，
		Loop,						//2：播放中间动画，
		Over,						//3：播放结束动画，
		Enter,						//4：进入人物选择界面
	};

	enum EXLHint
	{
		EXLHint_User,				//用户名密码提示
		EXLHint_UserError,			//用户名密码错误提示
		EXLHint_SendMsg,			//发送消息提示
		EXLHint_ConnectFail,		//超时
		EXLHint_Account_No_Match,	
		EXLHint_Account_In_Use,
		EXLHint_Account_Freezed,
		EXLHint_Wrong_Build_Number,
		EXLHint_Disabled,
		EXLHint_NoFindLauncher,
		EXLHint_Wrong_Type,
		EXLHint_Server_Full,
		EXLHint_Server_Suspended,
		EXLHint_Server_Maintenance,
		EXLHint_Failed,
		EXLHint_Short_Time,
		EXLHint_No_Select,
		EXLHint_GameWorldName_Wrong,
		EXLHint_SystemError,
		EXLHint_ProofResult_Forbid_MemberCenter,
		EXLHint_ProofResult_Forbid_GMTool,
		EXLHint_ProofResult_Forbid_CellPhone,
		EXLHint_ProofResult_Forbid_MiBao,
	};

	enum KoreaInputType
	{
		EKIT_Null,
		EKIT_SetPassWordOne,		//设置密码第一个
		EKIT_SetPassWordTwo,		//设置密码第二个
		EKIT_ChangePassWordOld,		//修改密码，旧密码
		EKIT_ChangePassWordOne,		//修改密码，第一次输入
		EKIT_ChangePassWordTwo,		//修改密码，第二次输入
	};
	KoreaInputType m_eKoreaInputType;

	enum EKoreaChangeUI
	{
		EKCU_SetPassWord,
		EKCU_Login,
		EKCU_ChangePassWord,
	};

	ELoginState					m_eState;
	DWORD						m_dwStateTime;
	DWORD						m_dwZoneIP;			//选择的Zone服务器IP
	DWORD						m_dwZonePort;		//选择的Zone服务器port
	FLOAT						m_fPingTime;		//ping时间
	//NullSceneGraph*				m_pSG;				//场景
	//CameraEuler					m_Camera;			//摄像机
	//EntityNode*					m_EntityNode;		//地物
	//VertexAnimationCtrl*		m_pAniCtrl;			//关键帧动画
	//float						m_pTime;			//每段动画的开始时间
	//ResRenderable*				m_pResRenderable;	//资源
	//int							m_nPlayState;		//动画播放的状态
	BOOL						m_bEnter;			//是否进入人物选择界面
	DWORD						m_dwAccountID;		//joingame用到
	DWORD						m_dwVerifyCode;		//joingame用到
	DWORD						m_dwSentJoinGame;
	BOOL						m_bSelectedWorld;
	BOOL						m_bGuard;			//是否防沉迷账号
	BOOL						m_bSave;
	BOOL						m_bSavePassword;

	INT							m_nPolicy;			//验证策略
	EMBPhase					m_eMB;				//输入密保阶段
	BOOL						m_bMiBao;			//是否进入密保界面
	tstring						m_strRand;			//记录随机字符串
	string						m_strMiBao;			//记录密保提示
	tstring						m_strInputMiBao;	//记录输入的密保
	BOOL						m_bTab;				//大小写

	char						m_szEncrypt[100];		//迅雷接口返回的加密
	char						m_szUserName[20];		//迅雷接口返回的帐号
	EXLHint						m_eHint;			//迅雷接口提示
	HINSTANCE					m_hDll;

	char						m_szPass[64];	// 台湾密码

   tstring						m_szEncryptCode;    // 韩服用户名
   tstring						m_szPssword;    // 韩服密码

	tstring						m_szOMGUserName;	//OMG模式用户名（豆客登陆模式也用它）
	tstring						m_szOMGPassWord;	//OMG模式密码（豆客登陆模式也用它）

	tstring						m_szKoreaWebUID;	//韩国Web UID

	BOOL						m_bSendSDOAInfo;

	INT							m_nBase;
	Vector3						m_LookAt;
	Vector3						m_LookFrom;
	bool						m_bIsShake;
	bool						m_bLeft;
	bool						m_bRight;
	float						m_fMAXTime;

	VOID SetState(ELoginState eState);

	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD EventHandlerForSecurity(tagGUIEvent* pEvent);
	DWORD EventKoreaLoginEvent(tagGUIEvent* pEvent);
	
	VOID WaitProofResult();	
	VOID WaitQueue();
	VOID TryIsConnectZoneSer();

	void JudegPolicy();
	void SendLoginMsg();
	
	// 加载动画
	VOID LoadMap();		

	// 显示用户名\密码错误信息
	VOID ShowUserPwdInfo(DWORD nInfo);

	// 设置UI
	VOID SetUIEnable(BOOL bVale);

	// 随机交换键盘按钮位置
	VOID SetKeyButtonPos();	

	// 处理JoinGame消息
	DWORD NetRecvJoinGame(tagNS_JoinGame* pMsg, DWORD pPrama); 

	// 断开与zone服务器的连接
	VOID CutOffZone();

	// 断开与login服务器的连接
	VOID CutOffLogin();

	//从注册表读取登录信息
	VOID LoadRegistLoginInfo();

	//从命令行读取登录信息
	BOOL LoadCMDLoginInfo();

	//登录信息OMG模式
	BOOL LoadOMGLoginTWInfo();

	//登录信息豆客模式 dk mode
	BOOL LoadDKLoginInfo();

	//登陆信息韩国Web模式
	BOOL LoadKoreaWebInfo();

	// 从注册表读取当前用户名和密码
	VOID LoadLastLoginInfo();

	// 记录当前用户名和密码到注册表
	VOID SaveLastLoginInfo();

	VOID SaveLastPassword();
	// 过滤用户名和密码中的回车换行等无意义字符
	VOID FilterString(tstring& str);

	// 随机置换密保按钮数字
	void RandSetKeyMB();

	// 判断密判输入阶段
	void MBPhaseHint();
	void ClearMBPhase(){ m_eMB = EMBP_NULL; } 
	void SetMBWndShow(BOOL bVal);

	LPCTSTR GetXunLeiHint(EXLHint eType);

	INT GetWindowMode(DWORD dwIndex);

	void ShellExecuteXunLei();

	void LetterTab();

	bool GetCmdLineUserAndPassword();

	void HideLoginFrame( bool bHide );

	void LoginGame();
	
	// password 加密
	tstring& InPassWord(tstring& ss);
	// password	解密
	tstring& OutPassWord(tstring& ss);

	DWORD OnExitLoadingFrame(tagGameEvent* pGameEvent);

	//韩国登陆，生成随机数字
	VOID GetRandomNumber();
	//韩国登陆，刷新显示哪个输入框
	VOID KoreaRefrashInput(KoreaInputType eNextState);
	//韩国登陆，清除密码信息
	VOID KoreaClearPassWordInfo();
	//韩国登陆，切换设置、登陆、修改密码界面
	VOID KoreaChangeUI(EKoreaChangeUI eType);
	tagNLC_SafeProof* m_sendSave;	//韩国保存一次登录消息
	BOOL			  m_bIsNeedSendAgain;	//韩国验证失败再次发送一次
	
	ResTexture			*m_pTex;
	RECT				m_rectSrc;
	RECT				m_rectDest;
	INT					m_nAlpha;
	DWORD				m_dwColor;
	bool				m_bRender;
	float				m_fParam;

	BYTE m_pMsgBuff[1024];
	DWORD m_dwCurPos;
	BOOL			m_bAutoLogin;
	bool				m_b8sendLogin;
	bool            m_bAskTokenID;		// 是否要过tokenid
	bool            m_bShowID;				// 是否需要显示YY
	DWORD		m_dwConnID;

	bool				m_bCloseMapOnce;	//是否已经关闭地图
	//------ open passport 相关
	std::vector<tagURLInfo> m_OpenPassportInfo;
	std::vector<tagURLInfo>::iterator m_itOpenPassPortInfo;
	BYTE				m_byOpenPassPotIndex;	//当前是哪一个合作商的索引
	std::vector<HttpPicEx*> m_vecHttpPic;
	//HttpPicEx*			m_pHttpPicCurr;			//三方登陆，登陆按钮上那个图标
	GUIPatch*			m_pOpenPassportPath;	//底板
	HttpPicEx*			m_pStcUrlPic[g_nUrlNum];		//url pic
	GUIPushButton*	m_pPBUrl[g_nUrlNum];		//url pic
	GUIButton*			m_pBtnPrev;
	GUIButton*			m_pBtnNext;
	GUIButton*			m_pBtnNormal;		//返回正常登陆
	INT						m_nCurUrlPage;  //从0开始
	INT						m_nCurChoose;	//从0开始
	DWORD				m_UpdateUrlTime;
	BOOL					m_bOpenPassport;		//用open passport登陆还是 常规登陆
	DWORD				m_dwDownLoadTime;
	BOOL					m_bSendUrlLoginMsg;	//发送用url登陆消息与否
	BOOL					m_bOpenWaitFlag;	// 等待时间标记
	GUIStatic*			m_pStcOpenPassName[g_nUrlNum];		// 合作商名字
	DWORD				m_dwCropID;		//合作商id  为0常规登陆否则为合作商模式登陆
	GUIStatic*			m_pStcLoginPic;	//login底图

	VOID	RefreshUrlPic();
	VOID   OpenPassportUpdate();
	VOID	SendUnrealLoginInfo();
	VOID	SendChooseUrlMsg(INT index);
	VOID   UrlModeHideWedget( BOOL bHide );
	VOID   OpenPassportLog(string sLogInfo);
	VOID	DownloadFunction();	// 下载
	VOID	RedownloadFunction();	// 重下载
	BOOL	NewHttpDownload( tstring tsPath, INT nIndexID );
	BOOL	NewHttpDownloadPic( tstring tsPath, INT nIndexID );
	BOOL	IsFinishAllDownload();
	DWORD OnFinishCloseLoading(tagGameEvent* pGameEvent);
	VOID	HideNormalLoginUI(BOOL isHide);	//隐藏常规登陆相关按钮等
	//------open passport end

	//云联合作，登陆后发送http请求
	void SendYunLianHttpPost();
	//关闭各种第三方登陆
	void CloseOtherLogin();
};

