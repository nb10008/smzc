#pragma once
#include "..\WorldDefine\selectrole.h"
#include "..\WorldDefine\ItemDefine.h"
#include "CreateRolePreviewData.h"


enum EGUI_Mouse_Event
{
	EMouse_LeftDown = EGUIE_User + 110,		//鼠标左键一直按下旋转按钮(原事件中没有鼠标一直按下的事件)
};

class Player;
class NetSession;
/** \class CreateRoleFrame
\brief 创建角色框架
*/
class CreateRoleFrame :
	public GameFrame
{
public:
	enum ECRState
	{
		ECRState_Cartoon = 0,				//片头动画
		ECRState_Inputing,					//等待玩家输入姓名、选定所有数据
		ECRState_WaitRepose,				//等待服务器响应
		ECRState_PlayAction,				//玩家输入数据后播放一段动画
		ECRState_Close,						//关闭
		ECRState_End,						
	};

	enum EStep
	{
		EStep_EnterCombat,	  //进入战斗
		EStep_HandUpWeapon,   //拔武器
		EStep_OpenEquip,      //展开装备（变形）
		EStep_Fighting,		  //战斗状态
		EStep_CloseEquip,     //关闭装备（变形）
		EStep_HandDownWeapon, //收武器
		EStep_End,			  //结束,播放站立动作
		EStep_Over			  //一组动作结束
	};

	enum ECreateRolePreview	  //装备预览用到，标志点击的是那种类型的装备
	{
		ECRP_Null = -1,
		ECRP_Sword = 0,		 //剑
		ECRP_Blade,			 //刀
		ECRP_Wand,			 //长兵
		ECRP_Bow,			 //弓
		ECRP_Organ,			 //法器
		ECRP_LongShou,		 //拢手
		ECRP_Qin,			 //琴
		ECRP_Amor,			 //铠甲
		ECRP_ClothAmor,		 //布甲
		ECRP_FashionOne,	 //时装壹
		ECRP_FashionTwo		 //时装贰
	};
 
public:
	CreateRoleFrame(void);
	~CreateRoleFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID Render3D();

	EPreChapStatus GetChapterStatus() { return m_eStatus; }

	//开始创建角色
	void BeginCreateRole();

	//结束创建角色
	void EndCreateRole();

	//获得角色的序章身份
	ECRState GetCurState()const { return m_curState; }

	//接收服务器消息(参数待定)
	DWORD NetRecvCreateRole(tagNS_CreateRole* pMsg, DWORD pPrama);

private:

	//将点击的窗体转换成武器的类型
//	ECreateRolePreview ParseEquipSelect( GUIWnd* pWnd );

	//武器切换事件 处理的入口
	void ProcessEquipPreview( ECreateRolePreview eCreateRole );

	//根绝点击类型作出适当处理
	void PreviewSkeletonHelp( ECreateRolePreview eCreateRole );

	//预览时 动画和特效的辅助函数
	void PrepareEffectSkeleton( const tagCreateRolePreviewItem* pCfgItem, ECreateRolePreview ePreview,ECreateRolePreview eLeftPreview, BOOL bPlayCombat = TRUE );

	//刷新的时候调用此方法，播放特效和动画
	void PlayEquipSkeleton();

	/** 得到特效的名字
		\ param  index 因为只配置了三个扩展特效，所以 对于扩展特效 index 取值范围是 [0-2]
		\ param  pBtye 是龙魂特效的数字编号，pByte 是一个有效指针，就赋值为龙魂特效的编号
	*/
	tstring GetEffectName( ECreateRolePreview eEquipType, BYTE index, LPBYTE pByte = NULL );

	//状态转换开关
	void SwitchToState(ECRState nextState);

	DWORD EventHandler(tagGUIEvent* pEvent);

	//当玩家输入了姓名后并且没有发过创建消息，确定按钮才可用
	BOOL CheckInputedName(); 

	//随机设置角色属性
	void RandAvatarAtt(tagAvatarAtt& att, bool bRandAll=false);

	//更新属性设置
	void UpdateAttUI(bool bSave);

	void Clear();

	//鼠标左键按下旋转按钮
	void CheckMouseLeftDown(float deltaTime);

	//角色旋转
	void RoleRotate(float deltaTime);

	//是否拾取了角色
	bool IsPickRole(int mouseX, int mouseY);

	void CanRolerAnimation(float time);

	void RefreshCamera(bool bFar);

	VOID FilterString(tstring& str);

	VOID RefreshCareerIntro();

	VOID ShowCareerEquip( INT nCareerType );
	void CreateBackGroud( INT nCareer );

    //void WeaponIntro(ECreateRolePreview eWeaponType);	 //显示武器说明页面

    //INT  PageMaxNum(ECreateRolePreview eWeaponType);

    //bool WeaponIntroEnvent(tagGUIEvent* pEvent );   //武器说明页面时间响应


private:

	TSFPTrunk<CreateRoleFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<VarContainer>			m_pVarContainer;
	TObjRef<InputDX8>				m_pInput;

	CreateRolePreviewData			m_previewData;		//预览配置文件
	EStep							m_step;				//武器预览的动画阶段
	DWORD							m_dwStartTime;		//计时
	tstring							m_szEnterTrackName; //进入战斗
	tstring							m_szLeaveTrackName;
	bool							m_bPlayPreview;		//是否播放预览动画
	ECreateRolePreview				m_eEquipType;		//当前主手武器的类型
	ECreateRolePreview				m_eLeftEquipType;		//当前副手武器的类型

	GUIWnd*							m_pWnd;
	GUIWnd*							m_pWndCaption;
	GUIEditBox*						m_pName;			//姓名输入控件
	GUIStatic*						m_pStcErrHint;		//错误提示static
	GUIStatic*						m_pStcSex;			//
	GUIFrameMutex*					m_pSexMutex;		//性别互斥控件
	GUIPushButton*					m_pMalePBtn;		//男性pushbutton
	GUIPushButton*					m_pFmalePBtn;		//女性pushbutton
	GUIPushButton*					m_pStatusPBtn[EPCS_End];	//序章身份pushbutton

	GUIButton*						m_pLBtnHType;		//发型左按钮
	GUIButton*						m_pRBtnHType;		//发型右按钮
	GUIStatic*						m_pStcHType;		//发型号显示static

	GUIButton*						m_pLBtnHColor;		//发色左按钮
	GUIButton*						m_pRBtnHColor;		//发色右按钮
	GUIStatic*						m_pStcHColor;		//发色号显示static

	GUIButton*						m_pLBtnFace;		//面部左按钮
	GUIButton*						m_pRBtnFace;		//面部右按钮
	GUIStatic*						m_pStcFace;			//面部号显示static

	GUIButton*						m_pLBtnFDetail;		//面部细节左按钮
	GUIButton*						m_pRBtnFDetail;		//面部细节右按钮
	GUIStatic*						m_pStcFDetail;		//面部细节号显示static

	GUIButton*						m_pLBtnDress;		//服式左按钮
	GUIButton*						m_pRBtnDress;		//服式右按钮
	GUIStatic*						m_pStcFDress;		//服式号显示static

	GUIButton*						m_pBtnOK;			//确定
	GUIButton*						m_pBtnRandom;		//随机
	GUIButton*						m_pBtnCancel;		//返回

	GUIWnd*							m_pWnd_Control;		//
	GUIButton*						m_pLBtnRotate;		//左旋转按钮
	GUIButton*						m_pRBtnRotate;		//右旋转按钮
	GUIPushButton*					m_pPBtn_Push;
	GUIPushButton*					m_pPBtn_Pull;


	//GUIButton*						m_pBtnSword;		//剑
	//GUIButton*						m_pBtnBlade;		//刀
	//GUIButton*						m_pBtnWand;			//长兵
	//GUIButton*						m_pBtnBow;			//弓
	//GUIButton*						m_pBtnOrgan;		//法器
	//GUIButton*						m_pBtnLongShou;		//拢手
	//GUIButton*						m_pBtnQin;			//琴
	//GUIButton*						m_pBtnAmorSuit;		//铠甲套装
	//GUIButton*						m_pBtnClothAmorSuit;//布甲套装
	//GUIButton*						m_pBtnFashion1;		//时装1
	//GUIButton*						m_pBtnFashion2;		//时装2

	ECreateRolePreview              m_ECurWeapon;       //当前选择的武器 
	//INT                             m_iMaxTextNum;		//武器说明窗口最大页数 初值为1
	//INT                             m_iCurTextNum;		//武器说明窗口当前页数 初值为1
	//GUIWnd*							m_pWndWeaponIntro;  //武器说明窗口
	//GUIStatic*						m_pStcIntroNum;     //武器说明页数显示
	//GUIStaticEx*					m_pStcIntroText;    //武器说明文本显示
	//GUIButton*						m_pBtnPreIntro;     //武器说明页前一页按钮
	//GUIButton*						m_pBtnNextIntro;    //武器说明页后一页按钮
    //TMap<INT CurTextMun, tstring *ptstr>   m_WeaponIntroMap; 
	   //              // 武器说明map，first表示当前页，second为当前页面的text
	
	GUIWnd		*m_pWndJob;
	GUIButton	*m_pBtnShenBing;	// 神兵
	GUIButton	*m_pBtnXuanLing;	// 玄翎
	GUIButton	*m_pBtnQianKun;		// 乾坤
	GUIButton	*m_pBtnXianYin;		// 仙音
	GUIStaticEx *m_pStcIntor;		// 职业介绍
	GUIScrollBar *m_pScrollBar;
	ECRState						m_curState;			//当前状态
	float							m_stateTime;		//当前状态持续的时间
	bool							m_bRotate;			//人物是否旋转
	bool							m_bLRotate;			//人物左旋转
	bool							m_bRRotate;			//人物右旋转
	EPreChapStatus					m_eStatus;			//序章身份
	bool							m_bSentCreate;		//是否已经发送过创建角色消息,防止重复发送
	tagAvatarAtt					m_avatarAtt;		//角色属性
	tagAvatarEquip					m_avatarEquip;      //角色装备（新手时装）
	float							m_fRoleYaw;			//角色旋转的航偏值(绕Y轴)
	Player*							m_pPlayer;			//角色	
	INT								m_nRoleNameLength;	//人物姓名的最大长度
	bool							m_bPickRole;		//是否拾取了角色
	BOOL							m_bAnimation;
	float							m_fRolerTimer;
	float							m_fAnimDelay;

	ResTexture						*m_pTex;			// 职业背景图
	RECT							m_rectSrc;
	RECT							m_rectDest;

	Vector3 						PREROLE_POS;
	Vector3 						LookPos;		
	Vector3 						Lookat;		
	Vector3 						LookPosNear;	
	Vector3 						LookatNear;	

};
