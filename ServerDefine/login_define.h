//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: login_define.h
// author: 
// actor:
// data: 2008-06-04
// last:
// brief: 登陆服务器相关数据结构
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/time.h"
#include "../WorldDefine/base_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// 游戏世界状态
//-----------------------------------------------------------------------------
enum EWorldStatus
{
	EWS_Well			=	0,		// 良好
	EWS_InitNotDone		=	1,		// 未初始化完毕
	EWS_SystemError		=	2,		// 系统出现故障
	EWS_ProofError		=	3,		// 验证服务器故障
	EWS_DBBlock			=	4,		// 数据库阻塞
};

//------------------------------------------------------------------------------
// 认证类型
//------------------------------------------------------------------------------
enum EProofPolicy
{
	EPP_Null	=  -1,
	EPP_Test	=	0,		// 测试专用
	EPP_Own		=	1,		// 自己公司验证策略
	EPP_XunLei	=	2,		// xunlei
	EPP_Orange	=	3,		// game orange
	EPP_QuanQuan=	4,		// 盛大圈圈
	EPP_Corn    =   5,      // 韩国验证策略
	EPP_Vietnam =	6,		// 越南版本
	EPP_Omg =	7,			// 台湾OMG版本
	EPP_Russia =	10,		// 俄罗斯
	EPP_Korea	=	11,		// 另一家韩国公司

	EPP_End		=	12,
};



//------------------------------------------------------------------------------
// 玩家登录时的状态
//------------------------------------------------------------------------------
enum  EPlayerLoginStatus
{
	EPLS_Null		=	-1,		// 无效
	EPLS_OffLine	=	0,		// 离线
	EPLS_Online		=	1,		// 在线
	EPLS_Loging		=	2,		// 登录中
	EPLS_Unknown	=	3,		// 未知
};

//------------------------------------------------------------------------------
// 玩家封停类型  会员中心封停0x01、GM工具封停0x02、手机锁0x04、密保卡0x08
//------------------------------------------------------------------------------
enum  EPlayerFrobidMask
{
	EPLM_MemberCenter	=	0x01,		// 会员中心
	EPLM_GMTool			=	0x02,		// GM工具
	EPLM_CellPhone		=	0x04,		// 手机锁
	EPLM_MiBao			=	0x08,		// 密保卡封停
	EPLM_WaiGua			=	0x10,		// 反外挂自动封停
	EPLM_Level			=	0x20,		// 等级异常
};
//-----------------------------------------------------------------------------
// 验证结果结构
//-----------------------------------------------------------------------------
struct tagProofResult
{
	DWORD					dwClientID;			// 客户端ID
	DWORD					dwAccountID;		// 帐号ID
	BOOL					bGuard;				// 防沉迷
	INT						nGuardAccuTime;		// 防沉迷累积时间
	EPlayerLoginStatus		eLoginStatus;		// 登陆状态
	DWORD					dwWorldNameCrc;		// 上一次登陆的游戏世界
	DWORD					dwSeondPsdCrc;		// 二次密码CRC值
	DWORD					dwFrobidMask;		// 封停掩码 每1位对应一种封停类型
	BYTE					byPrivilege;		// 权限
	bool					bNeedMibao;			// 是否使用密保
	bool					bSaveTelNum;		// 是否填写手机号
	bool					bIsFirstLogin;		// 是否第一次登入
	DWORD					dwPreLoginIP;		// 上次登录ip
	tagDWORDTime			dwPreLoginTime;		// 上次登录时间
	CHAR					szAccountName[X_SHORT_NAME]; //用户名 用于橘子登录
	BYTE					byReserved[2];		// 保留，对齐

	CHAR					szUserID[X_SHORT_NAME];		// UserID
	CHAR					szAppKey[X_APPKEY];			// AppKey
	CHAR					szToken[X_TOKEN];			// Token

	tagProofResult()
	{
		dwClientID		=	GT_INVALID;
		dwAccountID		=	GT_INVALID;
		bGuard			=	FALSE;
		nGuardAccuTime	=	0;
		eLoginStatus	=	EPLS_OffLine;
		byPrivilege		=	0;
		dwSeondPsdCrc	=   GT_INVALID;
		bNeedMibao		=	false;
		bSaveTelNum		=   0;
		dwFrobidMask	=   0;
		dwPreLoginIP    =	0;
		dwPreLoginTime	=   0;
		bIsFirstLogin	=	false;
		memset(szAccountName,0,X_SHORT_NAME);
		memset(szUserID,0,X_SHORT_NAME);
		memset(szAppKey,0,X_APPKEY);
		memset(szToken,0,X_TOKEN);
	}
};

struct tagProofResultFull : public tagProofResult
{
	INT nRet;		// 返回结果
};

//-----------------------------------------------------------------------------
// 验证回调函数
//-----------------------------------------------------------------------------
typedef UINT (WINAPI* PROOFCALLBACK)(INT nRet, tagProofResult* pResult);


#pragma pack(pop)