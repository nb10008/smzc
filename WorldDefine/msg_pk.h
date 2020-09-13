//-----------------------------------------------------------------------------
//!\file msg_pk.h
//!
//!\date 2008-09-27
//!
//!\brief 客户端与服务器之间关于PK模式及PK状态相关的消息
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "RoleDefine.h"

#pragma pack(push, 1)

enum
{
	E_SafeGuard_Already			=	1,		// 已经是当前状态
	E_SafeGuard_PKStateInvalid	=	2,		// PK状态为非和平状态，不能切换到保护模式
	E_SafeGuard_TimeLimit		=	3,		// 距离上次开启自由模式的时间未到，不能切换到保护模式
	E_SafeGuard_LevelLimit		=	4,		// 等级限制
	E_SafeGuard_ItemLimit		=	5,		// 使用的IM物品限制，无法切换到保护模式
	E_SafeGuard_OpenLimit		=	6,		// 您现在不可调整PK模式！

	E_PK_Already				=	10,		// 已经处于行凶状态（能攻击所有人）
	E_PK_SafeGuard				=	11,		// 保护模式下不能开启
	E_PKEX_Already				=	12,		// 已经处于行凶状态（只能攻击行凶玩家）
	E_PK_PeaceModelAlready		=	13,		// 已经是练级模式了
};
//-----------------------------------------------------------------------------
// PK保护
//-----------------------------------------------------------------------------
CMD_START(NC_SafeGuard)
	BOOL	bOpen;			// 是否开启
	INT64	n64ItemID;		// 开启所使用的物品的64位ID
CMD_END

CMD_START(NS_SafeGuard)
	DWORD	dwErrorCode;
	BOOL	bOpen;
CMD_END

//-----------------------------------------------------------------------------
// 开启行凶状态
//-----------------------------------------------------------------------------
CMD_START(NC_PK)
	BOOL	bOnlyPK;							// TRUE:对抗模式; FALSE:杀戮模式; -1:练级模式
CMD_END

CMD_START(NS_PK)
	DWORD	dwErrorCode;
	BOOL	bReset;								// 是否为攻击中重置
	BOOL	bOnlyPK;							// TRUE:对抗模式; FALSE:杀戮模式; -1:练级模式
CMD_END

//-----------------------------------------------------------------------------
// PK状态改变
//-----------------------------------------------------------------------------
CMD_START(NS_RolePKStateChange)
	DWORD			dwRoleID;					// 角色ID
	ERolePKState	eState;						// 新的PK状态
CMD_END

//-----------------------------------------------------------------------------
// PK道德值恢复数据
//-----------------------------------------------------------------------------
CMD_START(NS_RolePKMorality)
	INT		nTime;
	INT		nValue;
CMD_END

//-----------------------------------------------------------------------------
// 警告玩家正遭受其他玩家的攻击
//-----------------------------------------------------------------------------
CMD_START(NS_RoleBeAttacked)
	DWORD	dwEvilRoleID;						// 攻击者ID
	BOOL	bWarning;							// 是否该预警，v1.0.0, Jason 2010-7-17
CMD_END

CMD_START(NS_AskToClosePKSafeGuard)
	INT		nPKSafeGuardMaxLevel;	// pk保护
CMD_END
CMD_START(NC_AnswerToClosePKSafeGuard)
	DWORD	dwCloseOrNot;
CMD_END

#pragma pack(pop)