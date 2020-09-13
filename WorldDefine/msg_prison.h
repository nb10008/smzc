//-----------------------------------------------------------------------------
// filename: msg_prison.h
// author: client/server leili/xlguo
// actor:
// data: 2009-02-10
// last: 
// brief: 牢狱消息定义
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)
#include "msg_common_errorcode.h"
//----------------------------------------------------------------------------
// 错误码枚举
//----------------------------------------------------------------------------
enum
{
	E_Prison_Success					= E_Success,	// 成功
	E_Prison_UnknownError					,			// 未知错误
};

//-----------------------------------------------------------------------------
// 送入监狱
//-----------------------------------------------------------------------------
CMD_START(NS_SendToPrison)
	DWORD			dwCaptorRoleID;			// 为民除害的玩家
	DWORD			dwCatchedRoleID;		// 被送入监狱的玩家
CMD_END
// Jason 2010-6-18 v1.0.0
CMD_START(NS_SendToPrisonEx)
	DWORD			dwCaptorRoleID;			// 为民除害的玩家
	DWORD			dwCatchedRoleID;		// 被送入监狱的玩家
	TCHAR			strCaptorRoleName[X_SHORT_NAME];
	TCHAR			strCatcherRoleName[X_SHORT_NAME];
CMD_END

#pragma pack(pop)