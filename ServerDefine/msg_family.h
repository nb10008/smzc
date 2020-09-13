//////////////////////////////////////////////////////////////////////////
// 家族系统消息定义
//////////////////////////////////////////////////////////////////////////
#pragma once
#pragma pack(push, 1)

#include "family_define.h"

struct tagFamilyAtt;
struct tagFamilyMember;

//////////////////////////////////////////////////////////////////////////
// common
//////////////////////////////////////////////////////////////////////////
CMD_START(NDBC_CreateFamily)
	tagFamilyLoad		sFamilyLoad;
CMD_END

CMD_START(NDBC_DismissFamily)
	DWORD		dwFamilyID;
CMD_END

CMD_START(NDBC_LoadAllFamily)
CMD_END

CMD_START(NDBS_LoadAllFamily)	// 每次返回一个家族信息
	tagFamilyLoad		sFamilyLoad;
CMD_END

CMD_START(NDBC_LoadFamilyMember)
CMD_END

CMD_START(NDBS_LoadFamilyMember)
	INT				nMemberNum;
	tagFamilyMember	sFamilyMember[1];
CMD_END

CMD_START(NDBS_FamilyMemberInitOK)
CMD_END

CMD_START(NDBC_FamilyInitOK)
CMD_END

CMD_START(NDBS_FamilyInitOK)
CMD_END

//-----------------------------------------------------------------------------
// 关闭游戏服务器时，保存家族属性
//-----------------------------------------------------------------------------
CMD_START(NDBC_SaveAllFamily)
	INT					   nFamilyNum;
	tagFamilyLoad	   pFamilyLoad[1];
CMD_END

//-----------------------------------------------------------------------------
// 属性变更时保存家族属性
//-----------------------------------------------------------------------------
CMD_START(NDBC_SaveFamilyAtt)
	tagFamilyAtt   sFamilyAtt;
CMD_END

//-----------------------------------------------------------------------------
// 家族活跃度变更时保存家族属性
//-----------------------------------------------------------------------------
CMD_START(NDBC_SaveFamilyActive)
	DWORD   dwFamilyID;
	INT           nActive;             // 家族活跃度
CMD_END

//-----------------------------------------------------------------------------
// 家族成员
//-----------------------------------------------------------------------------
CMD_START(NDBC_JoinFamily)			// 加入家族
	tagFamilyMember	sFamilyMemInfo;
CMD_END

CMD_START(NDBC_LeaveFamily)			// 离开家族或被踢
	DWORD		dwRoleID;
CMD_END

CMD_START(NDBC_SetLeader)		// 族长任命或移交
	DWORD		dwRoleID;
	BOOL			bLeader;
CMD_END

//////////////////////////////////////////////////////////////////////////
// 家族妖精相关
//////////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////////////
// 家族任务相关
//////////////////////////////////////////////////////////////////////////

// 刷新家族任务
// 伴随这个消息要做的事情：1，将发布状态置为已发布。2，清空成员领取奖励标识
CMD_START(NDBC_RenewFamilyQuest)
	DWORD dwFamilyID;
	INT nProcess;
	BYTE byNum;
	tagFamilyQuest Quests[MAX_FAMILY_QUESTS_NUM];
CMD_END

// 同步家族任务信息
CMD_START(NDBC_SaveFamilyQuestDetail)
	DWORD dwFamilyID;
	tagFamilyQuest Quests[MAX_FAMILY_QUESTS_NUM];
CMD_END

// 任务发布状态重置
CMD_START(NDBC_ChangFamilyQuestIssueState)
	DWORD dwFamilyID;
	bool bIssueFlag;
CMD_END

// 初始化家族任务
CMD_START(NDBC_LoadFamilyQuest)
CMD_END

CMD_START(NDBS_LoadFamilyQuest)
	DWORD dwErrorCode;
	bool bLastOne; 
	INT nNum;
	tagFamilyQuestLoadData  QuestData[1];
CMD_END

// 初始化家族任务
CMD_START(NDBC_SaveRoleFamilyQuestRewarded)
	DWORD dwFamilyID;
	DWORD dwRoleID;
CMD_END

// 请求家族任务信息
CMD_START(NDBC_RequestFamilyQuest)
	DWORD dwFamilyID;
CMD_END

CMD_START(NDBC_DeleteFamilyQuest)
	DWORD dwFamilyID;
CMD_END

#pragma pack(pop)