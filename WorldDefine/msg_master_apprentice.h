//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_master_apprentice.h
// author: twan
// actor:
// data: 2010-01-18
// last: 
// brief: 师徒相关
//-----------------------------------------------------------------------------
#pragma once

#include "master_apprentice.h"
#include "msg_common_errorcode.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// 拜师相关
//-----------------------------------------------------------------------------

// 徒弟申请拜师
CMD_START(NC_ApprenticeApplyMaster)
	DWORD		dwRoleID;		// 师傅ID
CMD_END

// 返回给徒弟的错误码
CMD_START(NS_ApprenticeApplyMaster)
	DWORD		dwErrorCode;
CMD_END

// 发送给师傅的提示消息
CMD_START(NS_MasterNotice)
	DWORD		dwRoleID;		// 徒弟ID
	INT32		nLevel;			// 徒弟等级
CMD_END

// 师傅的回复
CMD_START(NC_MasterReply)
	DWORD		dwRoleID;		// 徒弟ID
	BOOL		bAccept;		// 是否接受
CMD_END

// 返回错误码给师傅
CMD_START(NS_MasterReply)
	DWORD		dwErrorCode;
CMD_END

// 师傅是否接受邀请的结果发送给徒弟
CMD_START(NS_MasterReplyApprentice)
	DWORD		dwRoleID;		// 徒弟ID
	DWORD		dwErrorCode;
	BOOL		bAccept;
CMD_END


//-----------------------------------------------------------------------------
// 收徒相关
//-----------------------------------------------------------------------------

// 师傅主动收徒
CMD_START(NC_MasterAcceptApprentice)
	DWORD		dwRoleID;			// 徒弟ID
CMD_END

// 返回给师傅的错误码
CMD_START(NS_MasterAcceptApprentice)
	DWORD		dwErrorCode;
CMD_END

// 发送给徒弟的提示消息
CMD_START(NS_ApprenticeNotice)
	DWORD		dwRoleID;		// 师傅ID
	INT32		nLevel;			// 师傅等级
CMD_END

// 徒弟的回复
CMD_START(NC_ApprenticeReply)
	DWORD		dwRoleID;		// 师傅ID
	BOOL		bAccept;		// 是否接受
CMD_END

// 返回错误码给徒弟
CMD_START(NS_ApprenticeReply)
	DWORD		dwErrorCode;
CMD_END

// 徒弟是否接受邀请的结果发送给师傅
CMD_START(NS_ApprenticeReplyMaster)
	DWORD		dwRoleID;		// 师傅ID
	DWORD		dwErrorCode;
	BOOL		bAccept;
CMD_END

//-----------------------------------------------------------------------------
// 师门相关
//-----------------------------------------------------------------------------

// 查看我的师门
CMD_START(NC_MatesInfo)
	
CMD_END

CMD_START(NS_MatesInfo)
	INT							nNum;
	INT32						nJingWuPoint;				// 可使用的授业点数
	tagMasterInfo				sMasterInfo;				// 师尊信息
	tagMatesAndApprenticeInfo	sMatesInfo[1];				// 同门信息
CMD_END

// 查看我的徒弟
CMD_START(NC_ApprenticeInfo)

CMD_END

CMD_START(NS_ApprenticeInfo)
	INT							nNum;
	tagMasterInfo				sMasterInfo;				// 师尊信息
	tagMatesAndApprenticeInfo	sApprenticeInfo[1];			// 同门信息
CMD_END

//-----------------------------------------------------------------------------
// 授业点数相关
//-----------------------------------------------------------------------------

// 师傅分配授业点数
CMD_START(NC_GiveJingWuPoint)
	DWORD				dwRoleID;			// 徒弟ID
	INT32				nPoint;				// 分配的点数				
CMD_END

CMD_START(NS_GiveJingWuPoint)
	DWORD				dwErrorCode;		// 错误码		
CMD_END

// 徒弟从师傅处得到授业点数
CMD_START(NS_GetJingWuPoint)
	INT32				nPoint;				
CMD_END

// 授业点数界面
CMD_START(NC_JingWu2Client)

CMD_END

CMD_START(NS_JingWu2Client)
	INT32				nNeedJingWuPoint;		// 需要的授业点数
	INT32				nJingWuPoint;			// 当前授业点数
	INT32				nNeedYuanBao;			// 需要的元宝
	INT32				nYuanBao;				// 当前元宝
CMD_END

// 徒弟使用授业点数
CMD_START(NC_UseJingWuPoint)
	
CMD_END

CMD_START(NS_UseJingWuPoint)
	DWORD				dwErrorCode;		// 错误码
	INT32				nCurJingWuPoint;	// 当前授业点数
CMD_END

//-----------------------------------------------------------------------------
// 解除师徒关系相关
//-----------------------------------------------------------------------------

// 徒弟脱离师门
CMD_START(NC_BreakOutMaster)
	DWORD				dwRoleID;			// 师傅ID
CMD_END

CMD_START(NS_BreakOutMaster)
	DWORD				dwErrorCode;
CMD_END

CMD_START(NC_ForceBreakOutMaster)
	DWORD				dwRoleID;			// 师傅ID
CMD_END

CMD_START(NS_ForceBreakOutMaster)
	DWORD				dwErrorCode;
	INT					dwRemainTime;
CMD_END

// 再次确认
CMD_START(NC_BreakOutMasterConfirm)
	DWORD				dwRoleID;			// 师傅ID
CMD_END

// 此消息将发送给师徒双方
CMD_START(NS_BreakOutMasterConfirm)
	DWORD				dwRoleID;
CMD_END


// 师傅逐出徒弟
CMD_START(NC_BreakOutApprentice)
	DWORD				dwRoleID;			// 徒弟ID
CMD_END

CMD_START(NS_BreakOutApprentice)
	DWORD				dwErrorCode;
CMD_END

// 再次确认
CMD_START(NC_BreakOutApprenticeConfirm)
	DWORD				dwRoleID;			// 徒弟ID
CMD_END

// 此消息将发送给师徒双方
CMD_START(NS_BreakOutApprenticeConfirm)
	DWORD				dwRoleID;
CMD_END

//-----------------------------------------------------------------------------
// 出师
//-----------------------------------------------------------------------------
CMD_START(NS_GraduateNotice)
	DWORD				dwRoleID;			// 徒弟ID
CMD_END

#pragma pack(pop)
