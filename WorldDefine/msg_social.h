//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_social.h
// author: 
// actor:
// data: 2008-12-04
// last:
// brief: 社会关系相关消息
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)

// 错误码
enum
{
	E_Friend_Target_Not_Exist			=	1,		// 目标不存在
	E_Friend_Target_Not_Sel				=	2,		// 不能加自己为好友
	E_Friend_Target_Black_List			=	3,		// 黑名单内的玩家无法被添加为好友
	E_Friend_Target_Already_Exit		=	4,		// 该好友已存在于好友列表
	E_Friend_Max_Num					=	5,		// 好友列表名单是否达到上限100人
	E_Friend_Group_Not_Exit				=	6,		// 分组不存在
	E_Friend_Not_Friend					=	7,		// 不是该玩家好友
	E_Friend_CanNotDel_Spouse			=	8,		// 不能删除爱人
	E_Friend_SystemRefuse				=	9,		// 对方游戏设置：拒绝任何人加自己为好友
	E_Friend_CanNotDel_Master			=	10,		// 不能删除师徒

	E_Black_Targer_Not_Sel				=	20,		// 不能加自己到黑名单
	E_Black_Max_Num						=	21,		// 黑名单已满
	E_Black_Target_Already_Exit			=	22,		// 玩家已在黑名单中
	E_Black_Target_Not_Exit				=	23,		// 玩家不在黑名单中

	E_Enemy_Target_Not_Exist			=	24,		// 玩家不在仇敌列表中
	E_Enemy_Target_Already_Exist		=	25,		// 玩家已在仇敌名单中
	E_Enemy_Max_Num						=	26,		// 仇敌名单已满

	E_Gift_Not_Exit						=	30,		// 礼物不存在
	E_Gift_Not_Both_Friend				=	31,		// 不是双向好友
	E_Gift_Friend_Not_Online			=	32,		// 好友不在线
	E_Gift_Not_Gift						=	33,		// 所送的物品不是礼物
};

CMD_START(NS_LoginToFriend)		// 玩家上线时通知好友
	DWORD	dwRoleID;
CMD_END;

CMD_START(NS_LogoutToFriend)	// 玩家下线时通知好友
	DWORD	dwRoleID;
CMD_END;

CMD_START(NC_RoleMakeFriend)	// 想和谁结交为好友
	DWORD	dwDestRoleID;	
CMD_END;

CMD_START(NS_RoleMakeFriend)
	DWORD	dwDestRoleID;
	BOOL	bOnline;
	INT		nVipLevel;
	INT		nLevel;				// 角色等级
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_RoleCancelFriend)	// 取消好友关系
	DWORD   dwDestRoleID;
CMD_END;

CMD_START(NS_RoleCancelFriend)	
	DWORD   dwDestRoleID;
	BYTE	byGroup;
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_UpdateFriGroup)	// 好友分组
	DWORD	dwDestRoleID;
	BYTE	byGroup;
CMD_END;

CMD_START(NS_UpdateFriGroup)
	DWORD	dwDestRoleID;
	BYTE	byOldGroup;			// 1-4：好友列表
	BYTE	byNewGroup;
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_MoveToBlackList)	// 加入玩家到黑名单
	DWORD	dwDestRoleID;
CMD_END;

CMD_START(NS_MoveToBlackList)
	DWORD	dwDestRoleID;
	BYTE	byOldGroup;
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_DeleteBlackList)	// 将玩家从黑名单删除
	DWORD	dwDestRoleID;
CMD_END;

CMD_START(NS_DeleteBlackList)
	DWORD	dwDestRoleID;
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_SendGift)			// 送礼物
	DWORD	dwDestRoleID;		// 礼物接收者
	INT64	n64ItemID;			// 礼物ID
CMD_END;

CMD_START(NS_SendGiftToFri)		// 收礼物
	DWORD	dwSrcRoleID;		// 礼物发送者
	DWORD	dwTypeID;			// 物品类型ID
CMD_END;

CMD_START(NS_SendGiftToSender)	// 收礼物
	DWORD	dwErrorCode;
CMD_END;

CMD_START(NC_SendGiftReply)		// 收到礼物后回复
	DWORD	dwSrcRoleID;		// 礼物发送者
	DWORD	dwTypeID;			// 礼物TypeID
	BOOL	bResult;			// 是否接收
CMD_END;

CMD_START(NS_SendGiftBroadcast)	// 广播
	DWORD	dwSrcRoleID;
	DWORD	dwDestRoleID;
	DWORD	dwTypeID;
	BOOL	bResult;
CMD_END;

CMD_START(NS_SendBlackList)		// 发送黑名单信息
	DWORD	dwRolesID[MAX_BLACKLIST];
CMD_END;

CMD_START(NC_UpdateFriEnemyState)
CMD_END;

CMD_START(NS_UpdateFriEnemyState)
	tagEnemyInfo	dwEnemysID[MAX_ENEMYNUM];// 增加仇敌名单
	INT				nNum;
	tagFriUpdate	FriUpdate[1];
CMD_END;

CMD_START(NS_MakeFriNotice)
	DWORD	dwRoleID;			
CMD_END;

CMD_START(NS_UpdateFriValue)
	DWORD   dwRoleID;
	INT		nFriVal;
CMD_END;

//---------------------------------------------------------------
// 仇敌列表相关消息
//---------------------------------------------------------------
// 插入仇敌信息
CMD_START(NS_InsertEnemy)
	DWORD			dwErrorCode;
	tagEnemyInfo	dwEnemyID;		//要添加的仇敌的角色ID
CMD_END;

// 删除仇敌信息
CMD_START(NC_DeleteEnemy)
	DWORD	dwEnemyID;		//要删除的仇敌的角色ID
CMD_END;

CMD_START(NS_DeleteEnemy)
	DWORD	dwErrorCode;
	DWORD	dwEnemyID;		//要删除的仇敌的角色ID
CMD_END;

// 仇敌上线时通知玩家角色
CMD_START(NS_EnemyLogin)
	DWORD	dwRoleID;
CMD_END;

#pragma pack(pop)