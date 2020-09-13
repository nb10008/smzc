//-----------------------------------------------------------------------------
//!\file msg_script.h
//!
//!\date 2009-4-16
//!
//!\brief 客户端与服务器之间脚本消息
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

enum EMsgUnitType;
enum EDlgOption;

//-----------------------------------------------------------------------------
// NS_ScriptBroad中dwMisc字段的位类型定义
//-----------------------------------------------------------------------------
CMD_START(NS_MsgBlob)
	BYTE byData[1];
CMD_END

//-----------------------------------------------------------------------------
// 客户端对话框发给服务的缺省消息
//-----------------------------------------------------------------------------
CMD_START(NC_DlgDefaultMsg)
	EMsgUnitType	eDlgTarget;				// 客户端对话框操作所对应的目标（任务，物品， 玩家）
	DWORD			dwTargetID;				// 目标ID
	EDlgOption		eDlgOption;				// 玩家返回的对话框选项
CMD_END

//只有单RoleID
CMD_START(NS_MsgBroadCastBlob)
	BYTE byData[1];
CMD_END

//-----------------------------------------------------------------------------
// 客户端触发服务器脚本的缺省消息
//-----------------------------------------------------------------------------
CMD_START(NC_DefaultRequest)
	EMsgUnitType	eDlgTarget;				// 触发服务器脚本的对应目标（固定活动）
	DWORD			dwTargetID;				// 目标ID
	DWORD			dwEventType;			// 触发的事件类型
CMD_END

// Jason 2010-6-1 拉人消息
// 服务器通知客户端的消息
CMD_START( NS_PullRoleMsg )
	DWORD			dwAppID;		// 应用id，客户端，只需要在下面的协议里传回这个值
	DWORD			dwActID;		// 活动id
	DWORD			dwIdMsgID;		// id_msg.xml中的id值
	DWORD			dwMapID;
	INT				x;				// x，
	INT				y;
	INT				z;
CMD_END
// 客户端反馈的消息，比如ok0，false非零等
CMD_START( NC_PullRoleMsgCallback )
	DWORD			dwAppID;
	DWORD			dwActID;
	DWORD			dwIdMsgID;		// id_msg.xml中的id值
	DWORD			param1;
	DWORD			param2;
	DWORD			param3;
	DWORD			param4;
CMD_END

#pragma pack(pop)