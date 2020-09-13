//-----------------------------------------------------------------------------
//!\file msg_athletics_system.h
//!
//!\date 2010-02-25
//!
//!\brief 客户端与服务器之间关于竞技系统的消息定义
//-----------------------------------------------------------------------------


#pragma once


enum E_PvP_RoomState;
enum E_PvP_RoomType;
enum E_PvP_RoomLevel;
enum E_PvP_RoleState;
struct tagPvPRoleInfo;
struct tagPvPRoom;
struct tagPvPSimRoom;

#include "RoleDefine.h"

#pragma pack(push, 1)

const  DWORD  MAX_LEVEL_LIMIT  = 40;     //进竞技场最低等级
struct tagRanPvPInfo
{
	INT			nPos;					//在随机房间中的位置
	INT			nPlayerLevel;			//远程玩家等级
	EClassType	ePlayerProfession;		//远程玩家职业
	DWORD		dwPlayerID;				//玩家ID
	TCHAR		szName[X_SHORT_NAME];	//玩家姓名
};

enum EPvPResult
{
	EPR_Win,	//胜利
	EPR_Prefect,//完胜
	EPR_Lose,	//失败
	EPR_Tie,	//平局
	EPR_Leave,	//离开
};

enum EPvPPlayerState
{
	EPPS_Leave = 1,	// 离开
	EPPS_Dead,		// 死亡
	EPPS_Alive,		//	活着
};
struct tagRanPvPResultInfo
{
	DWORD		dwPlayerID;		//玩家ID
	INT			nGloryChange;	//玩家荣誉值变化
	INT			nKillNum;		//杀人数
	float		fSuccRate;		//胜率
	EPvPResult	eResult;		//结果
};
//参加竞技的错误码
enum EPvPError
{
	E_PvP_Success			= 0,
	E_PvP_Unknown			= 1,	// 未知错误
	E_PvP_TooFar			= 2,	// NPC距离太远
	E_PvP_InvalidLevel		= 3,	// 等级不符合
	E_PvP_NoEnoughWuxin		= 4,	// 武勋不足
	E_PvP_NotInRoom			= 5,	// 还没有进入房间
	E_PvP_PosIsNotEmpty		= 6,	// 该位置已经有人	
	E_PvP_AlreadyInRoom		= 7,	// 已经在一个房间内 （创建房间时） 
	E_PvP_NoRoom			= 8,	// 请求的房间不存在
	E_PvP_CannotKick		= 9,	// 无法踢人
	E_PvP_RoomFighting		= 10,	// 无法加入开打的房间
	E_PvP_NotInHall			= 11,	// 尚未进入大厅
	E_PvP_RoomIsFull		= 12,	// 人数已满
	E_PvP_CreateRoomFail	= 13,	// 创建房间失败
	E_PvP_LevelLimitError	= 14,	// 无法创建，自身等级不符合
	E_PvP_WuxunLimitError	= 15,	// 无法创建，自身武勋不符合
	E_PvP_LeaveRoomFail		= 16,	// 离开失败
	E_PvP_RoleInReadyState	= 17,	// 就绪状态，无法离开
	E_PvP_RoomInFighting	= 18,	// 已经开打，无法退出
	E_PvP_ChangePosFail		= 19,	// 改变位置失败
	E_PvP_PosIsSolid		= 20,	// 位置上已经有玩家
	E_PvP_PrepareFail		= 21,	// 准备失败
	E_PvP_KickPlayerFail	= 22,	// 踢人失败
	E_PvP_StartFail			= 23,	// 开战失败
	E_PvP_CannotStart		= 24,	// 不能开战	
	E_PvP_NeedMorePlayer	= 25,   // 人数不足，无法开战
	E_PvP_PlayerNotReady	= 26,	// 有人没有准备好，无法开战
	E_PvP_UnOpened          = 27,   // 竞技场未开放状态
	E_PvP_InviteNotFriend   = 28,   // 竞技场未开放状态
	E_PvP_InviteFull		= 29,   // 竞技场未开放状态
	E_PvP_InviteRefused		= 30,	// 拒绝邀请
	E_PvP_InviteCannotJoin	= 31,	// 无法加入
	E_PvP_InviteNotOnLine   = 32,   // 不在线
	E_PvP_InviteInvaild     =  33,  // 邀请无效
	E_PvP_FrindLevelLimitError  =  34,  // 受邀玩家等级不符合
	E_PvP_FrindWuXunLimitError  = 35 ,  // 受邀玩家武勋等级不符合
        E_PvP_CanNotJoinRoom       = 36,    //无法加入房间
	E_PvP_End,
};

//-----------------------------------------------------------------------------
//	进入大厅
//-----------------------------------------------------------------------------
CMD_START(NC_PvPEnterHall)
	E_PvP_RoomType eRoomType ;  //房间类型	
CMD_END;

CMD_START(NS_PvPEnterHall)
	DWORD dwErrorCode; 	
CMD_END;

//-----------------------------------------------------------------------------
//	离开大厅
//-----------------------------------------------------------------------------
CMD_START(NC_PvPLeaveHall)
CMD_END;

CMD_START(NS_PvPLeaveHall)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	获取房间列表
//-----------------------------------------------------------------------------
CMD_START(NC_PvPGetRoomList)
	E_PvP_RoomLevel eRoomLevel ;//房间等级	
CMD_END;

CMD_START(NS_PvPGetRoomList)
	E_PvP_RoomLevel eRoomLevel ;//房间等级	
	DWORD dwCount;
	INT16  pData[1];
CMD_END;

//-----------------------------------------------------------------------------
//	获取房间简易信息
//-----------------------------------------------------------------------------
CMD_START(NC_PvPGetSimRoomInfo)
	E_PvP_RoomLevel eRoomLevel;	//房间等级	
	DWORD			dwCount;
	INT16	pData[1];
CMD_END;

CMD_START(NS_PvPGetSimRoomInfo)
	DWORD	dwErrorCode;
	DWORD	dwCount;
	tagPvPSimRoom	pData[1];
CMD_END;

//-----------------------------------------------------------------------------
//	获取房间信息
//-----------------------------------------------------------------------------
CMD_START(NC_PvPGetRoomInfo)
	E_PvP_RoomLevel eRoomLevel ;//房间等级	
	DWORD dwCount;
	INT16	pData[1];
CMD_END;

CMD_START(NS_PvPGetRoomInfo)
	DWORD	dwErrorCode;
	DWORD dwCount;
	tagPvPRoom	pData[1];
CMD_END;


//-----------------------------------------------------------------------------
//	参加竞技
//-----------------------------------------------------------------------------
CMD_START(NC_PvPJoinRoom)
	E_PvP_RoomLevel eRoomLevel ;//房间等级	
	INT16	nRoomID; 
CMD_END;

CMD_START(NS_PvPJoinRoom)
	E_PvP_RoomLevel eRoomLevel ;//房间等级	
	INT16	nRoomID; 
	DWORD	dwLocalPlayerRankLevel;		//请求玩家的 “排名”
	float	fLocalPlayerSuccessRate;	//请求玩家的 “胜率”
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	参加竞技(自动寻找房间)
//-----------------------------------------------------------------------------
CMD_START(NC_PvPAutoJoinRoom)
	E_PvP_RoomLevel eRoomLevel ;//房间等级	
CMD_END;

//-----------------------------------------------------------------------------
//	创建房间
//-----------------------------------------------------------------------------
CMD_START(NC_PvPCreateRoom)
	E_PvP_RoomLevel eRoomLevel ;//房间等级	
	INT		iLevelUpperLimit;		//房间等级上限
	INT		iLevelLowerLimit;		//房间等级下限 
	DWORD	dwWuXunLimit;			//武勋限制
CMD_END;

CMD_START(NS_PvPCreateRoom)
	DWORD	dwErrorCode; 
	E_PvP_RoomLevel eRoomLevel ;//房间等级	
	INT16	nRoomID;
CMD_END;

//-----------------------------------------------------------------------------
//	离开房间
//-----------------------------------------------------------------------------
CMD_START(NC_PvPLeaveRoom)
CMD_END;

CMD_START(NS_PvPLeaveRoom)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	换位置
//-----------------------------------------------------------------------------
CMD_START(NC_PvPChangePosition)
	BYTE	ucNewPos;
CMD_END;

CMD_START(NS_PvPChangePosition)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	准备
//-----------------------------------------------------------------------------
CMD_START(NC_PvPPrepare)
CMD_END;

CMD_START(NS_PvPPrepare)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	取消准备
//-----------------------------------------------------------------------------
CMD_START(NC_PvPUnPrepare)
CMD_END;

CMD_START(NS_PvPUnPrepare)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	踢人
//-----------------------------------------------------------------------------
CMD_START(NC_PvPKickPlayer)
	DWORD dwPlayerID;
CMD_END;

CMD_START(NS_PvPKickPlayer)
	DWORD	dwErrorCode; 
CMD_END;

CMD_START(NS_PvPKicked)
	DWORD dwManagerID;
CMD_END;

//-----------------------------------------------------------------------------
//	邀请(A发起)
//-----------------------------------------------------------------------------
CMD_START(NC_PvPInvite)
	E_PvP_RoomType eRoomType;
	E_PvP_RoomLevel eRoomLevel ;//房间等级
	INT16 nRoomID;
	DWORD dwFriendID;
CMD_END;

CMD_START(NS_PvPInvite)
	DWORD dwErrorCode;
	E_PvP_RoomType eRoomType;
	E_PvP_RoomLevel eRoomLevel ;//房间等级
	INT16 nRoomID;
	DWORD dwFriendID;
CMD_END;

//-----------------------------------------------------------------------------
//	邀请（服务器发给B）
//-----------------------------------------------------------------------------
CMD_START(NS_PvPInviteJoin)
	E_PvP_RoomType eRoomType;
	E_PvP_RoomLevel eRoomLevel ;//房间等级
	INT16 nRoomID;
	DWORD dwInvitorID;
CMD_END;

CMD_START(NC_PvPInviteJoin)
	BOOL	bIsAccpeted;		// 是否接受
	E_PvP_RoomType eRoomType;
	E_PvP_RoomLevel eRoomLevel ;//房间等级
	INT16 nRoomID;
	DWORD dwInvitorID;
CMD_END;

//-----------------------------------------------------------------------------
//	开战
//-----------------------------------------------------------------------------
CMD_START(NC_PvPStart)
CMD_END;

CMD_START(NS_PvPStart)
	DWORD	dwErrorCode; 
CMD_END;

//-----------------------------------------------------------------------------
//	通知房间所有客户端开战
//-----------------------------------------------------------------------------
CMD_START(NS_PvPStarted)
CMD_END;

//-----------------------------------------------------------------------------
//	通知房间所有客户端开始传送
//-----------------------------------------------------------------------------
CMD_START(NS_PvPEnterPvP)
CMD_END;

//-----------------------------------------------------------------------------
//	更新竞技场竞技状态
//-----------------------------------------------------------------------------
CMD_START(NS_PvPGetPvPState)
	DWORD dwRedCount;
	DWORD dwGreenCount;
CMD_END;

//-----------------------------------------------------------------------------
//	杀人通知
//-----------------------------------------------------------------------------
CMD_START(NS_PvPNotifyKill)
	DWORD dwKillCount;
CMD_END;

//-----------------------------------------------------------------------------
//	通知战斗结果
//-----------------------------------------------------------------------------
CMD_START(NS_PvPResultInfo)
	TCHAR pData[1];
CMD_END;

//-----------------------------------------------------------------------------
//	通知战斗后进入房间
//-----------------------------------------------------------------------------
CMD_START(NS_PvPSetIntoRoom)
	E_PvP_RoomType hallType;
	E_PvP_RoomLevel eLevel;
	INT16 roomID;
CMD_END;

//  [10/18/2010 zhangsu] 随机PVP房间 圣魔之血新加------------------------------
//-----------------------------------------------------------------------------
//	通知战斗后进入房间
//-----------------------------------------------------------------------------
CMD_START(NS_PvPRanRoomInfo)
	BYTE	bCount;						//当前房间的人数
	tagRanPvPInfo pInfo[1];				//就发有的就可以了，因为里面有位置信息
CMD_END;

//-----------------------------------------------------------------------------
//	房间单个玩家信息更新消息
//-----------------------------------------------------------------------------
CMD_START(NS_PvPRanRoomInfoSingleChange)
	bool	bIsJoin;					//是否是加入房间，如果是离开房间则发（false）
	tagRanPvPInfo pInfo;				
CMD_END;

//-----------------------------------------------------------------------------
//	PVP结果
//-----------------------------------------------------------------------------
CMD_START(NS_PvPRanRoomResultInfo)
	INT					nTodayNum;		//今天打了多少场了
	tagRanPvPResultInfo	pInfo[1];		// 6 个人的信息，包括本地玩家的  记得，是6个！！！！
CMD_END;

//-----------------------------------------------------------------------------
//	PVP单个玩家状态变化 （进入场景地图，离开场景）
//-----------------------------------------------------------------------------
CMD_START(NS_PvPSinglePlayerInfo)
	DWORD				dwPlayerID;
	EPvPPlayerState		eState;
CMD_END;

//-----------------------------------------------------------------------------
//	切换一个随机房间
//-----------------------------------------------------------------------------
CMD_START(NS_PvPChangeRandomRoom)
CMD_END;

#pragma pack(pop)

//end msg_athletics_system.h