#pragma once
#include "..\WorldDefine\family_define.h"

class FamilyFrame;

struct tagRoleFamilyInfo;

struct tagNS_CreateFamilyBroad;
struct tagNS_FamilyDismiss;
struct tagNS_FamilyJoinReq;
struct tagNS_FamilyJoinReqRes;
struct tagNS_FamilyJoinBroad;
struct tagNS_FamilyApply;
struct tagNS_FamilyApplyReqRes;
struct tagNS_FamilyHandOverBroad;
struct tagNS_FamilyKickBroad;
struct tagNS_FamilyLeaveBroad;
struct tagNS_GetFamilyInfo;
struct tagNS_GetFamilyName;
struct tagNS_FamilyProcFailed;
struct tagNS_FamilySpritTrainBroad;
struct tagNS_CreateFamilyAllBroad;

struct tagFamilyInviteEvent; 
struct tagFamilyApplyEvent;
struct tagFamilyEvent;

struct tagNS_OpenFamilyQuestPage;

struct tagTodayFamilyQuest
{
	DWORD QuestID;
	DWORD dwCurDegree;	//完成数
	tagTodayFamilyQuest& operator= (const tagTodayFamilyQuest& tag)
	{
		QuestID = tag.QuestID;
		dwCurDegree = tag.dwCurDegree;
		return *this;
	}
};

struct tagFamilyQuestDisplay
{
	tagTodayFamilyQuest			m_sFamilyQuestInfo[MAX_FAMILY_QUESTS_NUM];
	DWORD						m_dwYesterdayDegree;	//昨天完成度
	BOOL						m_bIsGetReward;			//是否领取昨天的奖励
};

class FamilyMgr
{
public:
	FamilyMgr(void);
	~FamilyMgr(void);
	void Init();
	void Destroy();
	static FamilyMgr* Inst();
// common----------------------------------------------------------------------------------------------------
	enum ENameUsedType
	{
		ENUT_Nothing = 0,	//单纯的名字申请，无使用 
		ENUT_Invite,		//用于邀请
		ENUT_Apply,			//用于申请
		ENUT_Frame,			//用于界面显示
	};
	struct tagID2Name
	{
		DWORD dwRoleID;
		tstring    strName;
	};
	typedef		std::map<DWORD, tagRoleFamilyInfo*>		FamilyMemberMap;
// 家族成员----------------------------------------------------------------------------------------------------
	//家族信息请求
	VOID SendFamilyInfoReq(DWORD dwID);
	//返回已经重排过的ID序列(key 从0开始，0为族长,之后婚姻可能用到)
	const std::map<DWORD, DWORD>& GetFamilyMemberIDList(){ return m_mapFamilyMember;}
	//ID查找相关的成员信息
	const tagRoleFamilyInfo* GetMemberByID(DWORD dwID);
	//查找族长ID
	DWORD GetFamilyLeaderID();
	//成员数量
	INT GetFamilyMemberNum(){ return (INT)m_mapFamilyMember.size();}
	//玩家申请获得家族名称
	tstring GetFamilyName(DWORD dwFamilyID,ENameUsedType eType=ENUT_Nothing ,DWORD dwID=GT_INVALID, tstring roleName = _T(""),bool bSendCmd = true );
	//判断角色是否与自己在同一家族
	bool JudgeIsSameFamily(DWORD dwRoleID);

// 家族任务----------------------------------------------------------------------------------------------------
	tagFamilyQuestDisplay* GetFamilyQuestInfo()
	{
		return &m_pFamilyQuestInfo;
	}

	BOOL IsFamilyQuestMonster(DWORD dwID);

// 家族妖精----------------------------------------------------------------------------------------------------
	VOID SetSpriteAtt(int pos, DWORD dwValue){m_dwSpriteAtt[pos] = dwValue;}
	DWORD GetSpriteAtt(int pos){return m_dwSpriteAtt[pos];}

	VOID SetSpriteLevel(DWORD dwLevel){m_dwSpriteLevel =dwLevel; }
	DWORD GetSpriteLevel(){return m_dwSpriteLevel;}
private:
	TSFPTrunk<FamilyMgr>				m_trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	// common----------------------------------------------------------------------------------------------------
	FamilyFrame * m_pFamilyFrame;
// 家族成员----------------------------------------------------------------------------------------------------
	//---变量---
	std::set<DWORD>								m_setNameReq;		//家族名称请求缓存表
	std::map<DWORD, tstring>				m_mapFamilyName;	//家族map
	std::map<DWORD, tagID2Name>		m_mapInivteCache;	//邀请缓存,<家族ID，邀请者ID>
	std::map<DWORD, DWORD>			m_mapApplyCache;	//申请缓存,<家族ID，申请者ID>
	std::map<DWORD, DWORD>			m_mapFamilyMember;	//家庭成员排列<序号，ID>（便于婚姻排序，序号从0开始，0为家长）
	FamilyMemberMap							m_mapFamilyInfo;	//家庭成员总属性结构(无序)
	BOOL												m_bMsgBoxMutex;  // 弹出框互斥
	//---函数---
	DWORD OnEventFamilyCreate( tagGameEvent* pGameEvent );
	DWORD OnEventFamilyBreak( tagGameEvent* pGameEvent );
	DWORD OnEventFamilyInvite( tagFamilyInviteEvent* pGameEvent );
	DWORD OnEventFamilyApply( tagFamilyApplyEvent* pGameEvent );
	DWORD OnEventFamilyEvent( tagFamilyEvent* pGameEvent );

	DWORD OnMsgbox_CreateFamily( tagMsgBoxEvent* pGameEvent );
	DWORD OnMsgBox_InputFamilyName( tagMsgInputBoxEvent* pGameEvent );
	DWORD OnMsgBox_BreakFamily( tagMsgBoxEvent* pGameEvent );
	DWORD OnMsgBox_FamilyJoinReq( tagMsgBoxEvent* pGameEvent );
	DWORD OnMsgBox_FamilyApplyReq( tagMsgBoxEvent* pGameEvent );
	DWORD OnMsgBox_FamilyEvent( tagMsgBoxEvent* pGameEvent );

	DWORD OnNS_FamilyProcFailed(tagNS_FamilyProcFailed* pNetCmd, DWORD);
	DWORD OnNS_CreateFamilyBroad(tagNS_CreateFamilyBroad* pNetCmd, DWORD);
	DWORD OnNS_FamilyDismiss(tagNS_FamilyDismiss* pNetCmd, DWORD);
	DWORD OnNS_FamilyJoinReq(tagNS_FamilyJoinReq* pNetCmd, DWORD);
	DWORD OnNS_FamilyJoinReqRes(tagNS_FamilyJoinReqRes* pNetCmd, DWORD);
	DWORD OnNS_FamilyApply(tagNS_FamilyApply* pNetCmd, DWORD);
	DWORD OnNS_FamilyApplyReqRes(tagNS_FamilyApplyReqRes* pNetCmd, DWORD);
	DWORD OnNS_FamilyJoinBroad(tagNS_FamilyJoinBroad* pNetCmd, DWORD);
	DWORD OnNS_FamilyHandOverBroad(tagNS_FamilyHandOverBroad* pNetCmd, DWORD);
	DWORD OnNS_FamilyKickBroad(tagNS_FamilyKickBroad* pNetCmd, DWORD);
	DWORD OnNS_FamilyLeaveBroad(tagNS_FamilyLeaveBroad* pNetCmd, DWORD);
	DWORD OnNS_GetFamilyName(tagNS_GetFamilyName* pNetCmd, DWORD);
	DWORD OnNS_GetFamilyInfo(tagNS_GetFamilyInfo* pNetCmd, DWORD);
	DWORD OnNS_CreateFamilyAllBroad(tagNS_CreateFamilyAllBroad* pNetCmd, DWORD);

	//显示错误码
	VOID ShowErrorMsg(DWORD dwErrorCode);	
	//kill obj
	VOID  KillObj_NPCTalk();
	//弹出邀请框
	BOOL  ShowInvite(DWORD dwFamilyID,DWORD dwInviterID,tstring strSrcName = _T("") );
	//弹出邀请框
	BOOL  ShowApply(DWORD dwApplyID, tstring strAppRoleName);
	//在家族频道添加信息
	void  ShowFamilyMsg(const TCHAR* szFormat,...);
	//清空家族信息
	VOID ClearFamilyInfo();
	//发送名字请求消息
	VOID SendFamilyNameReq(DWORD dwID);
	//清空家族成员
	VOID ClearFamilyMemberMap();
	
	
// 家族任务----------------------------------------------------------------------------------------------------
	DWORD OnNS_OpenFamilyQuestPage(tagNS_OpenFamilyQuestPage* pCmd, DWORD);

	tagFamilyQuestDisplay m_pFamilyQuestInfo;

// 家族妖精----------------------------------------------------------------------------------------------------
	DWORD OnNS_FamilySpritTrainBroad(tagNS_FamilySpritTrainBroad* pCmd, DWORD);

	DWORD m_dwSpriteAtt[EFSA_Num];		// 当前的att值 

	DWORD m_dwSpriteLevel;

};
