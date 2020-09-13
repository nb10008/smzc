//--------------------------------------------------------------------------------
//!\file quest_board.h
//!\author zhangrong
//!
//!\date 2010-06
//! last 2010-06
//!
//!\brief 任务板头文件
//!
//--------------------------------------------------------------------------------
#pragma once
#include "../WorldDefine/msg_quest_board.h"
#include "../ServerDefine/msg_quest.h"
#include "db_session.h"

//class Role;

#define BOARD_QUEST_MAX_RULE_NUM	17	// 生成规则总数
#define	IsBoardQuest(u16ID)			( u16ID >= QUEST_PANEL_START_TASK_ID && u16ID <= QUEST_PANEL_END_TASK_ID )
#define	IsRewardBoardQuest(u16ID)	( u16ID >= QUEST_PANEL_START_TASK_ID && u16ID < QUEST_PANEL_START_TASK_ID + QUEST_BOARD_MAX_CAPACITY)
#define	IsYellowListQuest(u16ID)	( u16ID >= YELLOW_LIST_START_TASK_ID && u16ID < YELLOW_LIST_START_TASK_ID + QUEST_BOARD_MAX_CAPACITY)

// 任务板刷新方式
enum EBoardRefreshType
{
	EBRT_CompleteQuest	= 0,			// 交任务
	EBRT_IM				= 1,			// 使用道具
	EBRT_Level			= 2,			// 等级适应
};

// 玩家身上获得的任务板任务的结构
struct tagRoleBoardQuest
{
	UINT16				u16ID;			// 对应预留的六个任务ID
	UINT16				u16BoardID;		// 任务板ID
	EBoardQuestState	eState;			// 状态
	
	tagRoleBoardQuest(INT16 a, UINT16 b, EBoardQuestState c)
	{
		u16ID = a;
		u16BoardID = b;
		eState = c;
	}
};

typedef TMap<UINT16, tagRoleBoardQuest*> RoleBoardQuestMap;

// 悬赏任务生成法则结构
struct tagBoardQuestRandomRate
{
	BYTE byIndex;
	INT16 n16WhiteRate;
	INT16 n16YellowRate;
	INT16 n16GreenRate;
	INT16 n16BlueRate;
	INT16 n16OrangeRate;
	INT16 n16PurpleRate;
};

// 任务板管理器定义
class BoardQuestMgr
{
public:
	BoardQuestMgr(Role*);
	~BoardQuestMgr();
	
	// 初始化
	VOID Init(BYTE byLevel, EQuestBoardType eQuestBoardType)
	{
		m_byLevel = byLevel;
		m_eQuestBoardType = eQuestBoardType;
		if (eQuestBoardType == EQBT_Reward)
			m_n16QuestPanelStartTaskID = QUEST_PANEL_START_TASK_ID;
		else if (eQuestBoardType == EQBT_YellowList)
			m_n16QuestPanelStartTaskID = YELLOW_LIST_START_TASK_ID;
	}
	BOOL Init(const BYTE* &pData, INT nNum, Role* pRole, BOOL bMovePointer = TRUE);

	// 任务板是否为空
	BOOL IsBoardEmpty()						{ return m_BoardQuest.Empty();		}	
	BOOL IsHaveThisQuest(UINT16);

	// 增加删除任务
	VOID Add(UINT16 u16ID, INT16 u16BoardID, EBoardQuestState eState = EQS_NotAccept);
	VOID Del(UINT16 u16ID);

	// Get函数
	RoleBoardQuestMap& GetRoleBoardQuest()	{ return m_BoardQuest;				}

	// 修改任务状态的接口
	VOID ChangeQuestState(UINT16 u16ID, EBoardQuestState eState);

	// 根据任务ID获得其对应的任务板ID
	UINT16 GetBoardIDFromID(UINT16 u16ID);
	BYTE GetQuestBoardLevel()				{ return m_byLevel;					}
	INT GetCompleteReFreshTime()			{ return m_nCompleteRefreshTime;	}
	INT GetIMRefreshTime()					{ return m_nIMRefreshTime;			}
	INT GetQuestMaxCnt(Role* pRole);

	// 刷新任务板
	VOID RefreshBoard(Role*pRole, EBoardRefreshType eType, INT64 n64Serial=0, DWORD dwNPCID=GT_INVALID);
	BOOL RefreshBoardByQuestColor(Role* pRole, BYTE byLevel, EBoardQuestQuality eColor, INT nNum, UINT16 nQuestID);

	// 刷新次数累加及赋值
	VOID IncreaseCompleteRefreshTime()		{ m_nCompleteRefreshTime += 1;		}
	VOID IncreaseIMRefreshTime()			{ m_nIMRefreshTime += 1;			}
	VOID SetCompleteRefreshTime(INT nValue)	{ m_nCompleteRefreshTime = nValue;	}
	VOID SetIMRefreshTime(INT nValue)		{ m_nIMRefreshTime = nValue;		}
	VOID SetBoardQuestLevel(BYTE byLevel)	{ m_byLevel = byLevel;				}

	// 根据当前的刷新次数获得本次刷新要使用的规则 
	tagBoardQuestRandomRate* GetNowtimeRule(EBoardRefreshType eType, BOOL bSecond/* 是否要使用二次刷新规则*/);
	tagBoardQuestRandomRate* GetRewardQuestRule(EBoardRefreshType eType, BOOL bSecond/* 是否要使用二次刷新规则*/);
	tagBoardQuestRandomRate* GetYellowListRule(EBoardRefreshType eType, BOOL bSecond/* 是否要使用二次刷新规则*/);

	VOID SaveBoardQuest2DB();

	INT GetScriptDataIdx()
	{
		if (m_eQuestBoardType == EQBT_Reward)
			return RewardBoardQuestLvlIdx;
		else if (m_eQuestBoardType == EQBT_Guild)
			return GuildBoardQuestLvlIdx;
		else if (m_eQuestBoardType == EQBT_YellowList)
			return YellowListBoardQuestLvlIdx;
		return -1;
	}
private:
	Role*				m_pRole;					// 管理器的所有者
	BYTE				m_byLevel;					// 任务板等级，记录的是任务板中等级最小的任务的等级
	RoleBoardQuestMap	m_BoardQuest;				// 角色身上的任务板任务
	INT					m_nCompleteRefreshTime;		// 通过完成任务获得刷新的次数
	INT					m_nIMRefreshTime;			// 通过使用IM道具获得刷新的次数
	EQuestBoardType m_eQuestBoardType;	// 任务板的种类
	UINT16			m_n16QuestPanelStartTaskID;	// 任务板起始任务ID
};

inline VOID BoardQuestMgr::Add(UINT16 u16ID, INT16 u16BoardID, EBoardQuestState eState /* = EQS_NotAccept */)
{
	tagRoleBoardQuest* p = new tagRoleBoardQuest(u16ID, u16BoardID, eState);
	if (!P_VALID(p))
	{
		return;
	}

	if(m_BoardQuest.IsExist(u16ID))
		return;

	m_BoardQuest.Add(u16ID, p);
}

inline VOID BoardQuestMgr::Del(UINT16 u16ID)
{
	tagRoleBoardQuest* p = m_BoardQuest.Peek(u16ID);
	if (P_VALID(p))
	{
		SAFE_DEL(p);
	}

	m_BoardQuest.Erase(u16ID);
}

inline UINT16 BoardQuestMgr::GetBoardIDFromID(UINT16 u16ID)
{
	tagRoleBoardQuest* p = m_BoardQuest.Peek(u16ID);
	if(!P_VALID(p)) return (UINT16)-1;
	return p->u16BoardID;
}