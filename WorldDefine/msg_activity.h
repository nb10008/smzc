//-----------------------------------------------------------------------------
//!\file msg_activity.h
//!
//!\date 2009-03-29
//!
//!\brief 客户端与服务器之间关于活动的消息
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// 脚本的缺省消息（NC_DefaultRequest)事件类型定义（该类型定义在lua中使用）
//-----------------------------------------------------------------------------
/*
dwEventType = 1				// 客户端请求PVP固定活动界面 
dwEventType = 2				// 查询玩家在固定活动中的排名
*/

CMD_START(NS_ActivityTaskState)				// 广播活动当前状态给服务器内所有玩家
	INT		nState;							// 0-即将开始 1-开始 2-即将结束 3-结束 
	TCHAR	szName[X_SHORT_NAME];			// 活动名称
	BYTE	byHour;
	BYTE	byMinute;
CMD_END

CMD_START(NS_RoleInitOk)				// 玩家初始化结束
CMD_END


CMD_START(NC_AllGodMiracleInfo)		// 获取当前所有神迹的信息
	DWORD	dwActiveID;				// 活动ID
CMD_END	

// 神迹信息的结构定义
struct tagGodMiracleInfo
{
	DWORD	dwGodMiracleID;		// 神迹ID
	DWORD	dwCaptureGuildID;	// 占领神迹的帮派ID(等于0则无占领，否则被占领)
	DWORD	dwLastCpatureTime;	// 剩余占领神迹的时间(秒)
	tagGodMiracleInfo():dwGodMiracleID(0),dwCaptureGuildID(0),dwLastCpatureTime(0)
	{}
};

CMD_START(NS_AllGodMiracleInfo)			// 玩家初始化结束
	int				    nGodNum;		// 数量
	tagGodMiracleInfo	sGodMiracle[1];
CMD_END	

//-----------------------------------------------------------------------------
// 夺宝奇兵活动消息
//-----------------------------------------------------------------------------
// 同步宝箱可开启剩余时间(单位为秒)
CMD_START(NS_SyncOpenChaseRemainTime)
	DWORD	dwOpenChaseRemainTime;
CMD_END

// 宝箱被强制掉落
CMD_START(NS_ForceLootChase)
CMD_END

//-----------------------------------------------------------------------------
// 天选者之战活动  dwMsgType == 1脚本传入
//-----------------------------------------------------------------------------
CMD_START(NS_DotaMsg)
DWORD dwMsgType;
DWORD dwMsgSize;
CHAR  msg[1];
CMD_END 

#pragma pack(pop)