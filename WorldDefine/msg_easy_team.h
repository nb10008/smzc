/********************************************************************
	created:	2010/09/25
	created:	25:9:2010   19:38
	filename: 	e:\F-Project\src\WorldDefine\msg_easy_team.h
	file path:	e:\F-Project\src\WorldDefine
	file base:	msg_easy_team
	file ext:	h
	author:		Jason
	
	purpose:	快捷组队消息以及结构定义
*********************************************************************/

#ifndef msg_easy_team_h__
#define msg_easy_team_h__


#pragma pack(push, 1)

// 目前只发EETT_KillCreature目的的队伍信息
enum EEasyTeamType
{
	EETT_KillCreature	= 0 , // 杀怪
	EETT_Instance		= 1 , // 副本
	EETT_Activity		= 2 , // 活动
	EETT_Other			= 3 , // 其他
	EETT_All,				  // 所有
};

struct tagEasyTeamSimInfo
{
	BYTE	byTeamType;	// 组对目的，比如杀怪，EEasyTeamType类型
	DWORD	dwLeaderID;	// 队长id
	BYTE	byLeaderLevel;	// 队长等级
	WORD	wRoles;			// 队伍人数星系，两个字节，低字节总人数，高字节当前人数
	BYTE	byMemoLen;		// 杀怪情况下，该字段等于0，注意该字段是字节个数，不是tchar的个数哟
	BYTE	byInsAndActi;	// 副本或者活动枚举值
	BYTE	byContent[1];	// 该字段是组队目的，不过在杀怪目的下，这个没有实际意义，TCHAR*
};

struct tagClientEasyTeamSimInfo
{
	BYTE	byTeamType;	// 组对目的，比如杀怪，EEasyTeamType类型
	DWORD	dwLeaderID;	// 队长id
	BYTE	byLeaderLevel;	// 队长等级
	WORD	wRoles;			// 队伍人数星系，两个字节，低字节总人数，高字节当前人数
	BYTE	byMemoLen;		// 杀怪情况下，该字段等于0，注意该字段是字节个数，不是tchar的个数哟
	BYTE	byInsAndActi;	// 副本或者活动枚举值
	tstring strContent;
};

struct tagPersonalMode
{
	BYTE	byType;				// 求组类型， EEasyTeamType
	DWORD	dwRoleID;			// 玩家ID
	BYTE	byLevel;			// 玩家等级
	BYTE	byInsAndActi;		// 副本或者活动枚举值
	BYTE	byLen;				//
	BYTE	byContent[1];		// 备注
};

struct tagClientPersonalMode
{
	BYTE	byType;				// 求组类型， EEasyTeamType
	DWORD	dwRoleID;			// 玩家ID
	BYTE	byLevel;			// 玩家等级
	BYTE	byInsAndActi;		// 副本或者活动枚举值
	BYTE	byLen;				//
	tstring	strContent;			// 备注
};


// 获取便捷组队信息，客户端适当控制，刷新按钮的点击频率，否则会影响服务器性能
CMD_START( NC_GetEasyTeamInfo )
CMD_END

CMD_START( NS_GetEasyTeamInfo )
	WORD	wTeamNums;
	tagEasyTeamSimInfo	Teams[1];
CMD_END

// 获取附近未组队玩家
CMD_START( NC_GetSinglePlayers )
CMD_END

CMD_START( NS_GetSinglePlayers )
	WORD	wNums;
	DWORD	dwRoleID[1];
CMD_END

// 获取求组的玩家
CMD_START( NC_GetPersonalInfo )
CMD_END

CMD_START( NS_GetPersonalInfo )
	WORD	wNums;
	tagPersonalMode Person[1];
CMD_END

//  删除自己的队伍信息，不想让别人看到了
CMD_START( NC_DeleteMyTeamInfo )
	DWORD	dwRoleID;
CMD_END

//  删除自己的求组信息
CMD_START( NC_DeleteAppTeamInfo )
	DWORD	dwRoleID;
CMD_END


//  发布队伍或者求组信息
CMD_START( NC_PutOutInfo )
	BOOL bTeam;				// TRUE, 队伍信息；	FALSE, 求组信息
	BYTE byTeamType;
	BYTE byScene;
	BYTE byTeamTypeDetail;
	BYTE byTeamNum;
	BYTE byMinLevel;
	BYTE byMaxLevel;
	BYTE btLen;
	BYTE byContent[1];		// 备注
CMD_END

CMD_START( NS_PutOutInfo )
	DWORD dwErrorCode;
CMD_END


//////////////////////////////////////////////////////////////////////////
// 下面是服务器主动通知客户端用
enum EEasyTeamChangeType
{
	EETCT_NewTeam	= 0 , // 新增队
	EETCT_DelTeam	= 1 , // 删除队
	EETCT_AddMem	= 2 , // 增加成员
	EETCT_DelMem	= 3 , // 删除成员
	EETCT_LeaderChange = 4, // 移交队长
};
CMD_START( NS_EasyTeamEvent )
	BYTE	byTeamEvent;	// 删除队EEasyTeamChangeType
	BYTE	byTeamInfo[1];	// 队信息，对于删除队，该字段没有意义；对于其他类型，该字段就是tagEasyTeamSimInfo*
CMD_END


#pragma pack(pop)

#endif // msg_easy_team_h__




