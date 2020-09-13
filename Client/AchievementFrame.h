#pragma once
#include "gameframe.h"
#include "TrainMgr.h"
#include "StaticEx.h"
#include "SysMenuFrame.h"

#define MAX_SHOW_NUM 3

class ItemButton;
class AppliedTree;
class IconStatic;
struct tagNS_GetTrainList;
struct tagNS_FinishTrain;
struct tagNS_OpenTrain;
struct tagNS_SyncTrainCompleteness;

struct tagAchievement
{
	GUIStatic*		pStcName;			// 成就名称
	GUIStaticEx*	pStcOpenCondition;	// 成就开启条件
	GUIStaticEx*	pStcDesc;			// 成就描述
	GUIStaticEx*	pStcEContent;		// 内容
	ItemButton*		pIbRewardOne;		// 奖励1
	ItemButton*		pIbRewardTwo;		// 奖励2
	GUIStatic*		pStcExp;			// 奖励经验
	GUIStatic*		pStcGold;			// 金
	GUIStatic*		pStcSilver;			// 银
	GUIButton*		pBnTake;			// 领取奖励按钮
	IconStatic*		pStcFinish;			// 开启标示图片
	GUIStatic*		pStcAlreadyTake;	// 是否领取奖励图片
	GUIStatic*		pStcItem1Back;		// 物品奖励一底图
	GUIStatic*		pStcItem2Back;		// 物品奖励二底图

	GUIPatch*		pPthBackDown;		// 说明背景
	GUIStatic*		pPthBackUp;			// title背景
};


class AchievementFrame :
	public GameFrame
{
public:
	AchievementFrame(void);
	~AchievementFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

	BOOL IsWindowVisible();
private:
	VOID InitAppliedTrees();
	VOID ReloadTrees();
	VOID ReloadIDVector();
	BOOL EventHandler(tagGUIEvent* pEvent);
	VOID RefreshUI();
	void OnEvent_CloseTrainMain(tagGameEvent *pEvent);
	void OnEvent_OpenTrainMain(tagGameEvent *pEvent);

	DWORD OnNS_FirstOpenTrainList(tagNS_GetTrainList* pNetCmd,DWORD dwParam);
	DWORD OnNS_FinishTrain(tagNS_FinishTrain* pNetCmd, DWORD dwParam);
	DWORD OnNS_OpenTrain(tagNS_OpenTrain* pNetCmd, DWORD deParam);
	DWORD OnNS_SyncTrainCompleteness(tagNS_SyncTrainCompleteness* pNetCmd, DWORD deParam);
	VOID ShowErrorTrainMessage(ETrainErrCode error);

	VOID ClearInfo(tagAchievement &strAch);
	bool IfCanShow();
	VOID ShowPic(tstring str);

private:
	TSFPTrunk<AchievementFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	// 控件数据
	GUIWnd*							m_pMainWnd;
	tagAchievement					m_strAchievement[MAX_SHOW_NUM];
	GUIPushButton*					m_pPbShowDone;	//已完成的成就
	GUIPushButton*					m_pPbShowUnOpen;//未开启的成就
	AppliedTree*					m_pTree;
	GUIStatic*						m_pStcPercent;	//完成比例
	GUIStatic*						m_pStcCurentPage;	//当前页数
	GUIButton*						m_pPageUp;		
	GUIButton*						m_pPageDown;
	GUIButton*						m_pClose;

	// 逻辑数据
	INT								m_nCurPage;
	std::vector<DWORD>				m_vecAchieID;	// 当前需要显示的所有公式ID集合,用于显示3级信息
	typedef std::map<DWORD, DWORD>	TrainTreeMap;
	TrainTreeMap					m_mapTrainTreeMap;  // 试炼图key,appliedtree中的ID，value试炼type2
	DWORD							m_dwCurrTrainID;	// appliedtree中的目前选中的ID（2级）

	std::list<tstring>				m_vecShowVector;	// 需要显示完成的成就
	DWORD							m_dwTimeMark;
	bool							m_bNeedSpace;

	GUIWnd*							m_pOpenWnd;
	GUIStatic*						m_pOpenText;
};
