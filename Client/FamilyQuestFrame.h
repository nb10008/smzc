#pragma once
#include "..\WorldDefine\family_define.h"
#include "FamilyMgr.h"
/** \class FamilyQuestFrame
\brief 家族任务
*/

class IconStatic;
struct tagNS_OpenFamilyQuestPage;
struct tagNS_RewardFamilyMember;
struct tagNS_GiveInFamilyQuestItem;

class FamilyQuestFrame :
	public GameFrame
{
public:
	FamilyQuestFrame();
	~FamilyQuestFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	void SetFamilyQuestInfo(tagFamilyQuestDisplay* pData);	//刷新今日信息
	void ShowFrame(BOOL bIsShow);
	void ClearHandInWnd();
	void ClearRewardWnd();
	void ClearInfo();
	void SetRewardInfo();
	void RefrashInfo();
	void ShowErrorCode(DWORD dwErrCode);

private:
	//--处理GUI事件
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnSetFamilyQuestInfo(tagGameEvent* pEvent);

	//--网络消息
	DWORD OnNS_RewardFamilyMember(tagNS_RewardFamilyMember* pCmd, DWORD);
	DWORD OnNS_GiveInFamilyQuestItem(tagNS_GiveInFamilyQuestItem* pCmd, DWORD);

private:
	TSFPTrunk<FamilyQuestFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>		    m_pSession;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;
	GUIStatic*					m_pStcYFinishDegree;						//昨日完成度
	GUIButton*					m_pBtnReceive;								//领取奖励
	GUIStatic*					m_pStcFinishTip;							//是否可领取奖励提示

	GUIStatic*					m_pStcTFinishDegree;						//今日完成度
	GUIStatic*					m_pStcPersent[MAX_FAMILY_QUESTS_NUM];		//任务完成度
	GUIStatic*					m_pStcQuestDesc[MAX_FAMILY_QUESTS_NUM];		//任务描述
	GUIStatic*					m_pStcCurDegree[MAX_FAMILY_QUESTS_NUM];		//当前完成进度
	GUIButton*					m_pBtnHandIn[MAX_FAMILY_QUESTS_NUM];		//上交
	GUIStatic*					m_pStcKillTip[MAX_FAMILY_QUESTS_NUM];		//击杀生效..
	GUIStatic*					m_pStcHasFinishIcon[MAX_FAMILY_QUESTS_NUM];	//已完成的那个标志..

	GUIWnd*						m_pWndReward;								//领取奖励面板
	GUIStatic*					m_pStcExp;
	GUIStatic*					m_pStcGold;
	GUIStatic*					m_pStcSilver;
	GUIButton*					m_pBtnClose;
	GUIButton*					m_pBtnGetReward;
	GUIButton*					m_pBtnCancel;
	IconStatic*					m_pIStcItem[MAX_FQ_RewardNum];	
	IconStatic*					m_pIStcItemEx[MAX_FQ_RewardNum];
	GUIStatic*					m_pStc80Got;	//达成80%的标签
	GUIStatic*					m_pStc100Got;	//达成100%的标签

	GUIWnd*						m_pWndHandIn;								//上交物品
	GUIPatch*					m_pWndHandInCaption;						//
	IconStatic*					m_pIStcItemIcon;
	GUIStatic*					m_pStcItemName;
	GUIButton*					m_pBtnHandInClose;
	GUIButton*					m_pBtnOK;
	GUIEditBox*					m_pEBoxNum;

	INT8						m_n8QuestIndex;
	DWORD 						m_dwItemTypeID;
	INT64						m_n64EquipID;


	tagTodayFamilyQuest			m_sFamilyQuestInfo[MAX_FAMILY_QUESTS_NUM];
	DWORD						m_dwYesterdayDegree;	//昨天完成度
	BOOL						m_bIsGetReward;			//是否领取昨天的奖励

};
