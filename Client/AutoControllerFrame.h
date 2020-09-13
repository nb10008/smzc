#pragma once
#include "gameframe.h"
#include "ItemButton.h"
#include "NPC.h"
#include "Role.h"
#include "RoleMgr.h"

struct tagNS_UseZhanDouFu;
struct tagNS_ZhanDouFuTimeOut;
struct tagSpecItemEvent;
struct tagNS_SyncAutoFightState;
struct tagUpdateLPSkillEvent;
struct tagRolePickEvent;
class SkillButton;
struct tagHitTargetEvent;
struct tagShowDeadMenuEvent;
struct tagNS_RoleExpChange;

const INT MAX_SKILL         =   6;
const INT MAX_ITEM			=   2;


enum EAutoState
{
	EAS_NONE	  = 0,	//非自动战斗状态
	EAS_COMBAT	  = 1,	//战斗
	EAS_SEARCHING = 2,	//搜索
	EAS_NOT_ALL_AUTO = 3,	//关闭了自动选怪功能
	EAS_PATHFING	= 4,	//目标被遮挡住路径搜索过程
};


struct tagAutoSkillData
{
	DWORD	dwSkillID;	//技能ID
	INT		nTimeDelay;	//时间延迟(默认1秒钟)
	DWORD     nCurrentTime;	//当前时间
	bool	bIsActive;	//是否已经激活(默认非激活)
	tagAutoSkillData():dwSkillID(0),nTimeDelay(1000),nCurrentTime(0),bIsActive(false){}
	void ClearSkillData()
	{
		dwSkillID = 0;
		nTimeDelay = 1000;
		nCurrentTime = 0;
		bIsActive = false;
	}
};

struct tagAutoItemData
{
	DWORD			dwItemID;		//物品ID
	EItemConType	eType;	//物品位置
	INT16			n16Index;//容器中的位置
	INT				nTimeDelay;		//设定时间延迟
	DWORD			nCurrentTime;	//当前时间
	INT				nUsePercent;	//百分比
	bool			bIsActive;		//是否激活
	INT64			n64ID;
	tagAutoItemData():dwItemID(0),n16Index(0),nTimeDelay(1000),nCurrentTime(0),nUsePercent(50),bIsActive(false),n64ID(0){
		eType = EICT_Bag;
	}
	void ClearItemData()
	{
		dwItemID = 0;
		nTimeDelay = 1000;
		nUsePercent = 50;
		nCurrentTime = 0;
		n16Index = 0;
		eType = EICT_Bag;
		bIsActive = false;
		n64ID = 0;
	}
};

class AutoControllerFrame :
	public GameFrame
{
public:
	AutoControllerFrame(void);
	~AutoControllerFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

	bool IsAutoCombating()	{  return !( EAS_NONE == eState);	}

private:
	BOOL OnWnd_AutoCombat(tagGUIEvent* pEvent);
	BOOL OnWnd_Setting(tagGUIEvent* pEvent);
	BOOL OnWnd_Min(tagGUIEvent* pEvent);

	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pGameEvent);
	DWORD OnShowDeadMenuEvent(tagShowDeadMenuEvent* pEvent);
	/**
	开始自动战斗
	*/
	VOID StartAutoCombating();	
	/**
	结束自动战斗
	*/
	VOID EndAutoCombating();	
	/**
	刷新战斗界面UI
	*/
	VOID RefreashCombatUI();
	/**
	重置战斗界面UI
	*/
	VOID SetDefaultCombatUI();
	/**
	查找下一个怪物
	*/
	bool SearchNextMonster();
	/**
	使用技能
	*/
	bool UseAutoSkill();
	/**
	使用物品
	*/
	VOID UseAutoItem();
	/**
	回去定点
	*/
	VOID GoBack();
	/**
	回去上次看见怪的地方
	*/
	VOID GoBackToLastPositionSeeMonster();
	/**
	去怪物当前的位置
	*/
	VOID GoToMonsterCurrentPosition();
	/**
	更新自动战斗逻辑
	*/
	VOID UpdateCombat();
	/**
	默认设置UI
	*/
	VOID SetDefaultSettingUI();
	/**
	保存设置
	*/
	VOID ReadSettingFromFile();
	/**
	保存设置
	*/
	VOID SaveSetting();
	VOID SaveSettingToLocalFile();

	VOID RefreshSettingToUI();

	/**
	更新冷却时间
	*/
	VOID UpdateCDtimes();

	VOID AutoBaoQi();

	DWORD OnOpenAutoHook(tagSpecItemEvent* pEvent);
	
	DWORD OnNS_UseZhanDouFu(tagNS_UseZhanDouFu* pMsg,DWORD pParam);
	DWORD OnNS_ZhanDouFuTimeOut(tagNS_ZhanDouFuTimeOut* pMsg,DWORD pParam);
	DWORD OnNS_SyncAutoFightState(tagNS_SyncAutoFightState* pMsg,DWORD pParam);

	DWORD OnNS_RoleExpChange(tagNS_RoleExpChange* pNetCmd, DWORD dwParam);

	VOID BlockCheck();
	VOID IdleCheck();

	bool MonsterIsValid(DWORD dwID);

	DWORD OnRolePickEvent( tagRolePickEvent* pEvent );
	DWORD OnHitTargetEvent(tagHitTargetEvent* pEvent);

	VOID UpdateUsetimeInfo();
private:

	TSFPTrunk<AutoControllerFrame>	m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	SkillButton*					m_pBtSkills[MAX_SKILL]; //技能图标
	ItemButton*						m_pBtItems[MAX_ITEM];	//物品图标

	GUIButton*						m_pBnStartCombat;		//开始战斗
	GUIButton*						m_pBnAutoSetting;		//设置

	GUIButton*						m_pBnMinSize;			//最小化
	GUIButton*						m_pBnClose;				//关闭


	GUIWnd*							m_pWnd;					//主窗口
	GUIWnd*							m_pWndSetting;			//set窗口
	GUIWnd*							m_pWndMin;				//最小化窗口
	GUIButton*						m_pBnMin;				//最小化按钮

	EAutoState						eState;					//战斗状态

	tagAutoSkillData				m_skillData[MAX_SKILL];	//技能信息列表
	tagAutoItemData					m_itemData[MAX_ITEM];	//物品信息列表

	Vector3							m_staticPoint;			//定点打怪的点

	bool							m_bAutoSelectMonster;	//自动选怪
	bool							m_bAutoBaoQi;			//自动爆气
	bool							m_bPointStand;			//定点打怪

	DWORD							m_LockTargetID;			//锁定的攻击目标

	DWORD							m_dwCurrentSkillTime;
	DWORD							m_dwTimeMark;

	//Setting 相关

	GUIPushButton*					m_pPbItem0Active;	//启用自动使用真气药
	GUIPushButton*					m_pPbItem1Active;	//启用自动使用体力药

	GUIEditBox*						m_pEd_Item0Percent;	
	GUIEditBox*						m_pEd_Item1Percent;

	GUIPushButton*					m_pPbSkill0Active;	//启用自动使用技能0
	GUIPushButton*					m_pPbSkill1Active;	//启用自动使用技能1
	GUIPushButton*					m_pPbSkill2Active;	//启用自动使用技能2
	GUIPushButton*					m_pPbSkill3Active;	//启用自动使用技能3	
	GUIPushButton*					m_pPbSkill4Active;	//启用自动使用技能4
	GUIPushButton*					m_pPbSkill5Active;	//启用自动使用技能5
	
	GUIPushButton*					m_pPbAutoSelectActive;	//启用自动选怪
	GUIPushButton*					m_pPbAutoBaoQiActive;	//启用自动曝气
	GUIPushButton*					m_pPbStaticPointActive;	//启用定点打怪

	GUIEditBox*						m_pEd_Item0Time;	
	GUIEditBox*						m_pEd_Item1Time;

	GUIEditBox*						m_pEd_Skill0Time;	
	GUIEditBox*						m_pEd_Skill1Time;
	GUIEditBox*						m_pEd_Skill2Time;	
	GUIEditBox*						m_pEd_Skill3Time;
	GUIEditBox*						m_pEd_Skill4Time;	
	GUIEditBox*						m_pEd_Skill5Time;

	GUIButton*						m_pBnSaveSettings;
	GUIButton*						m_pBnCloseSetting;

	GUIStatic*						m_pStLeftTime;
	GUIStatic*						m_pStStartTime;
	GUIStatic*						m_pStExpHaveGot;

	bool							m_bFristOpen;

	INT64							m_n64ID;

	DWORD							m_dwTimeSpace;				//卡死时间计时器
	Vector3							m_LastFightPoint;			//上次打怪的地点
	DWORD							m_dwTargetHP;				//怪物HP
	list<DWORD>						m_lsBlockIDList;				//卡死怪物ID

	DWORD							m_dwIdleTime;				//角色空闲状态的时间

	DWORD							m_dwTimeEclipse;				//挂机间隔时间

	DWORD							m_dwPathfingTimeEclipse;		//寻路时间

	DWORD							m_dwUseTime;		// 开始使用了的时间，秒为单位
	INT64								m_dwUseExp;		// 开始使用时获得的经验
	tagDWORDTime				m_sGetTime;		// 战斗服获得的时间
	DWORD							m_dwLeftTime;
};
