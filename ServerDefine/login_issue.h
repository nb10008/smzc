//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: login_issue.h
// author: 
// actor:
// data: 2008-06-04
// last:
// brief: 登陆服务器与区域服务器相关消息
//-----------------------------------------------------------------------------
#pragma once

#include "..\WorldDefine\msg_common_errorcode.h"
#include "..\WorldDefine\selectrole.h"
#include "..\ServerDefine\role_data.h"
#include "login_define.h"
#include "vip_netbar_define.h"

#pragma pack(push, 1)

enum
{
	E_PlayerWillLogin_PlayerNumLimit	=	1,	// 超出人数限制

	E_PlayerLogin_VerifyCodeError		=	11,	// 验证码错误
	E_PlayerLogin_TimeOut				=	12,	// 超时
	E_PlayerLogin_PKRunawayPunishmentState = 13,// PK逃逸惩罚状态
};

//------------------------------------------------------------------------------
// 区域服务器与登陆服务器认证消息
//------------------------------------------------------------------------------
CMD_START(NLW_Certification)
	DWORD			dwGoldenCode;
	BOOL			bOrangeVipBar;
CMD_END

CMD_START(NWL_Certification)
	DWORD			dwGoldenCode;
	TCHAR			szWorldName[X_SHORT_NAME];
	DWORD			dwPort;	//ZoneServer端口
	DWORD			dwIP;	//服务器IP,用于广域网
	INT				nOLAccountNum;
	DWORD			dwOLAccountID[1];// 所有当前在线玩家
CMD_END

//-------------------------------------------------------------------------------
// 心跳
//-------------------------------------------------------------------------------
CMD_START(NLW_HeartBeat)
CMD_END

//-------------------------------------------------------------------------------
// 区域服务器通知登陆服务器当前服务器状态
//-------------------------------------------------------------------------------
CMD_START(NWL_WorldStatus)
	EWorldStatus	eStatus;			// 当前服务器状态
	INT				nPlayerNumLimit;	// 当前服务器的人数限制
	INT				nCurPlayerNum;		// 当前人数（包括真正在线的和等待进入的）
	INT				nCurPlayerDisplay;		// 当前人数（包括真正在线的和等待进入的）
CMD_END

//--------------------------------------------------------------------------------
// 登陆服务器通知区域服务器帐号登入
//--------------------------------------------------------------------------------
CMD_START(NLW_PlayerWillLogin)
	DWORD				dwAccountID;
	BYTE				byPrivilege;				// 权限
	DWORD				dwVerifyCode;
	DWORD               dwChannelID;                //混服运营商ID
	BOOL				bGuard;						// 防沉迷
	DWORD				dwAccOLSec;					// 累计登录时间
	char				szAccount[X_SHORT_NAME];	// 玩家账号
	CHAR				szKrUserID[X_SHORT_NAME];	//韩国版本用

	tagDWORDTime		dwPreLoginTime;				// 上次登录时间
	DWORD				dwPreLoginIP;				// 上次登录ip
CMD_END

//--------------------------------------------------------------------------------
// 区域服务器通知登陆服务器帐号可以登入
//--------------------------------------------------------------------------------
CMD_START(NWL_PlayerWillLogin)
	DWORD			dwErrorCode;
	DWORD			dwAccountID;
CMD_END


//--------------------------------------------------------------------------------
// 区域服务器通知登陆服务器玩家登入
//--------------------------------------------------------------------------------
CMD_START(NWL_PlayerLogin)
	DWORD			dwAccountID;
	DWORD			dwIP;
	DWORD			dwErrorCode;
	DWORD			dwWorldID;
	CHAR            szMac[MAX_MAC_LENGTH];
CMD_END

//---------------------------------------------------------------------------------
// 区域服务器通知登陆服务器玩家登出
//---------------------------------------------------------------------------------
CMD_START(NWL_PlayerLogout)
	DWORD			dwAccountID;
	DWORD			dwWorldID;
CMD_END

//---------------------------------------------------------------------------------
// 登陆服务器通知区域服务器踢出指定玩家(已经在区域服务器中的玩家再次登录时, 就要踢被区域服务器掉)
//---------------------------------------------------------------------------------
CMD_START(NLW_KickPlayer)
	DWORD			dwAccountID;
CMD_END

//---------------------------------------------------------------------------------
// 区域服务器提出玩家时，通知给登陆服务器做记录
//---------------------------------------------------------------------------------
CMD_START(NWL_KickLog)
	DWORD			dwAccountID;	// 账号
	DWORD			dwKickTime;		// 踢出时间
	DWORD			u16ErrCode;		// 反外挂系统提供的错误号
	BYTE			bySeal;			// 是否封号
CMD_END


//---------------------------------------------------------------------------------
// 区域服务器被关闭时，登陆服务器返回
//---------------------------------------------------------------------------------
CMD_START(NLW_WorldColsed)

CMD_END
//---------------------------------------------------------------------------------
// 区域服务器被关闭时，通知给登陆服务器改变玩家登陆状态
//---------------------------------------------------------------------------------
CMD_START(NWL_WorldColsed)

CMD_END

//---------------------------------------------------------------------------------
// 封号
//---------------------------------------------------------------------------------
CMD_START(NWL_ForbidAccount)
	DWORD			dwAccountID;	// 帐号
	DWORD			dwForbidCode;	// 封号码
CMD_END

// Jason 2010-4-27 v2.1.0
CMD_START(NWL_PlayerPKRunaway)
	DWORD			dwAccountID;
	DWORD			dwTicksWantedToWait;
CMD_END

//---------------------------------------------------------------------------------
// （越南版本防沉迷）通知登陆服务器开始和结束防沉迷计时
//---------------------------------------------------------------------------------
CMD_START(NWL_FatigueTimer)
	DWORD			dwAccountID;	// 账号
	BYTE			byEnterMap;		// 1:角色进入地图 0:角色离开地图
CMD_END

//---------------------------------------------------------------------------------
//创建角色名字
//---------------------------------------------------------------------------------
CMD_START(NWL_CreateRole)
DWORD			dwAccountID;	// 账号
TCHAR			szRoleName[X_SHORT_NAME];
EPreChapStatus	eStatus;
tagAvatarAtt	AvatarAtt;
CMD_END

//---------------------------------------------------------------------------------
//创建角色名字
//---------------------------------------------------------------------------------
CMD_START(NLW_CreateRole)
TCHAR			szRoleName[X_SHORT_NAME];
DWORD			dwAccountID;	// 账号
DWORD           dwErrorCode;
EPreChapStatus	eStatus;
tagAvatarAtt	AvatarAtt;
CMD_END

//---------------------------------------------------------------------------------
//创建帮派名字
//---------------------------------------------------------------------------------
CMD_START(NWL_CreateGuild)
DWORD		dwAccountID;	// 账号
DWORD		dwNPCID;
DWORD       dwRoleID;
TCHAR		szGuildName[X_SHORT_NAME];
CMD_END

//---------------------------------------------------------------------------------
//创建帮派名字
//---------------------------------------------------------------------------------
CMD_START(NLW_CreateGuild)
DWORD		dwAccountID;	// 账号
DWORD       dwErrorCode;
DWORD		dwNPCID;
DWORD       dwRoleID;
TCHAR		szGuildName[X_SHORT_NAME];

CMD_END

//---------------------------------------------------------------------------------
//创建家族名字
//---------------------------------------------------------------------------------
CMD_START(NWL_CreateFamily)
	DWORD		dwAccountID;	// 账号
	DWORD		dwNPCID;
	DWORD       dwRoleID;
	TCHAR		szFamilyName[X_SHORT_NAME];
CMD_END

//---------------------------------------------------------------------------------
//创建家族名字
//---------------------------------------------------------------------------------
CMD_START(NLW_CreateFamily)
	DWORD		dwAccountID;	// 账号
	DWORD       dwErrorCode;
	DWORD		dwNPCID;
	DWORD       dwRoleID;
	TCHAR		szFamilyName[X_SHORT_NAME];
CMD_END


// 金牌网吧数据
CMD_START( NWL_GetVipNetbarInfo )
CMD_END

//---------------------------------------------------------------------------------
//保存玩家手机号
//---------------------------------------------------------------------------------
CMD_START(NWL_SaveTelNum)
DWORD	dwAccountID;	// 账号
char	szAccount[X_SHORT_NAME];			// 玩家账号
DWORD	dwRoleID;	
INT64	n64TelNum;			// 玩家手机号
CMD_END

//---------------------------------------------------------------------------------
//保存玩家手机号
//---------------------------------------------------------------------------------
CMD_START(NLW_SaveTelNum)
DWORD	dwRoleID;	
DWORD   dwErrorCode;	// 结果代码
CMD_END

//---------------------------------------------------------------------------------
//玩家不保存手机号
//---------------------------------------------------------------------------------
CMD_START(NWL_NotSaveTelNum)
DWORD	dwAccountID;	// 账号
CMD_END



struct tagStrIPRange
{
	TCHAR strIPMin[20]; // xxx.xxx.xxx.xxx
	TCHAR strIPMax[20]; // xxx.xxx.xxx.xxx
};
struct tagVipNetVarDBProto
{
	DWORD	dwID;
	TCHAR	szName[X_SHORT_NAME];
	//INT		nPlayerNum;
	DWORD	dwBuffID;		// 金牌网吧登录，就有的持续时间8小时
	DWORD	dwInstBuffID;	// 进入副本才增加的buff

	tagStrIPRange	IpStrRange[MAX_VNB_IP_NUM];	// 一一对应的ip端，与dwIpMax对应
};
// 获取金牌网吧数据
CMD_START( NLW_IssueVipNetbarInfo )
	INT16	nNetbarNum;
	tagVipNetVarDBProto data[1];
CMD_END

#pragma pack(pop)