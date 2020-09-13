//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_common.h
// author: 
// actor:
// data: 2009-03-25
// last:
// brief: 账号下角色通用数据相关消息
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// 消息结构
//-----------------------------------------------------------------------------

struct tagGodBidInfo2DB
{
	DWORD	dwRoleID;
	INT64		n64Bidden;
	BYTE		byBidType;
	DWORD	dwBidID;
	tagGodBidInfo2DB()
	{
		dwRoleID = GT_INVALID;
		n64Bidden = 0;
		byBidType = GT_INVALID;
		dwBidID = GT_INVALID;
	}
};

// 加载竞标信息
CMD_START(NDBC_LoadGodBidInfo)
CMD_END

CMD_START(NDBS_LoadGodBidInfo)
	DWORD dwBidCount;
	tagGodBidInfo2DB GodBidInfo2DB[1];
CMD_END

// 保存一条竞标信息
CMD_START(NDBC_SaveOneGodBidInfo)
	tagGodBidInfo2DB GodBidInfo2DB;
CMD_END

// 清空竞标信息
CMD_START(NDBC_ClearGodBidInfo)
	BOOL bBidEnd;
CMD_END
CMD_START(NDBS_ClearGodBidInfo)
	BOOL bClearSuccess;
CMD_END

#pragma pack(pop)
