//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_rt_c.h
// author: 
// actor:
// data: 2009-04-27
// last:
// brief: GM服务端与GM工具之间的消息定义
//-----------------------------------------------------------------------------
#pragma once

//#include "../WorldDefine/base_define.h"
#include "rt_define.h"
#include "../ServerDefine/gm_tool_define.h"

#pragma pack(push, 1)

#define X_SHORT_NAME			32
//-----------------------------------------------------------------------
//GM创建
//-----------------------------------------------------------------------
CMD_START(NGMTC_GMCreate)
	TCHAR			szName[32];			//帐号
	DWORD			dwPswCrc;						//密码CRC
	EPrivilege		ePrivilege;						//权限
CMD_END

CMD_START(NGMTS_GMCreate)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//GM修改
//-----------------------------------------------------------------------
CMD_START(NGMTC_GMModify)
	TCHAR			szName[32];			//帐号
	EPrivilege		ePrivilege;						//权限
CMD_END

CMD_START(NGMTS_GMModify)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//GM删除
//-----------------------------------------------------------------------
CMD_START(NGMTC_GMDelete)
	TCHAR			szName[32];			//帐号
CMD_END

CMD_START(NGMTS_GMDelete)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//						登录查看msg_rt_c.h 
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//角色恢复
//-----------------------------------------------------------------------
CMD_START(NGMTC_RoleResume)
	INT				nOldID;							//角色原来的ID
	TCHAR			szNewName[32];		//角色新起的名字
	DWORD			dwSectionID;					//大区ID
	DWORD			dwWorldID;						//游戏世界ID
CMD_END

CMD_START(NGMTS_RoleResume)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//角色属性修改
//-----------------------------------------------------------------------
CMD_START(NGMTC_ProModify)
	TCHAR			szName[32];			//角色名字
	INT				nLel;							//等级
	INT				nSkill;							//天资点
	INT				nAttPoint;						//属性点
	INT				nYuanbao;						//元宝
	INT64			n64Exp;							//经验
	INT64			n64Money;						//钱	
	DWORD			dwSectionID;					//大区ID
	DWORD			dwWorldID;						//游戏世界ID
CMD_END

CMD_START(NGMTS_ProModify)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//物品生成
//-----------------------------------------------------------------------
CMD_START(NGMTC_ItemCreate)
	TCHAR			szName[32];			//角色
	DWORD			dwItemTypeID;					//物品TypeID
	INT				nItemNum;						//数量
	BYTE			byQuality;						//品质
	DWORD			dwSectionID;					//大区ID
	DWORD			dwWorldID;						//游戏世界ID
CMD_END

CMD_START(NGMTS_ItemCreate)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//物品删除
//-----------------------------------------------------------------------
CMD_START(NGMTC_ItemDelete)
	TCHAR			szName[32];			//角色
	//INT64			n64ItemID;						//物品全局ID
	DWORD			dwSectionID;					//大区ID
	DWORD			dwWorldID;						//游戏世界ID
	TCHAR			sz64IDString[1];				//要删除物品的64位ID组成的串		
CMD_END

CMD_START(NGMTS_ItemDelete)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//物品恢复
//-----------------------------------------------------------------------
CMD_START(NGMTC_ItemResume)
	TCHAR			szTargetRole[32];		//目标角色名
	DWORD			dwSectionID;					//大区ID
	DWORD			dwWorldID;						//游戏世界ID
	TCHAR			sz64IDString[1];				//物品全局ID
CMD_END

CMD_START(NGMTS_ItemResume)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 账号封停
//-----------------------------------------------------------------------
CMD_START(NGMTC_AccountForbid)
	TCHAR			szSectionName[32];		//大区
	TCHAR			szWorldName[32];			//游戏世界
	INT				nAction;							//0:解封 1：封
	TCHAR           szAccountStr[1];					//账号串,各账号之间逗号分隔
CMD_END

CMD_START(NGMTS_AccountForbid)
	//BOOL			bLast;
	TCHAR           szAccount[32];
	INT				nAction;
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//--双倍
//-----------------------------------------------------------------------
CMD_START(NGMTC_Double)
	EDoubleType		eDType;
	DWORD			dwOpenTime;						//开放时间
	DWORD			dwLastTime;						//持续时长
	DWORD			dwRatio;						//比例
	DWORD			dwSectionID;					//大区ID,
	DWORD			dwWorldID[1];					//游戏世界ID[n]     代表该大区下所有游戏世界		
CMD_END

//-----------------------------------------------------------------------
//--右下角公告
//-----------------------------------------------------------------------
CMD_START(NGMTC_RightNotice)								
	DWORD			dwNoticeTime;					//公告时间 tagDWORDTime 0为即时
	TCHAR			szTitle[Notice_Title];			//标题
	TCHAR			szContent[Notice_Content];		//内容
	TCHAR			szLink[Notice_Link];			//链接的网站
	DWORD			dwSectionID;					//大区ID,
	DWORD			dwWorldID[1];					//游戏世界ID[n]      游戏世界ID:GT_INVALID代表该大区下所有游戏世界											
CMD_END

//-----------------------------------------------------------------------
//--自动循环公告
//-----------------------------------------------------------------------
CMD_START(NGMTC_AutoNotice)
	TCHAR			szContent[Notice_Content];		//内容
	EColorType		eType;							//颜色
	DWORD			dwIssueTime;					//发布时间
	INT				nTimeInterval;					//发布间隔
	INT				nCirInterval;					//循环时间
	INT				nCirNum;						//循环次数
	DWORD			dwSectionID;					//大区ID,
	DWORD			dwWorldID[1];					//游戏世界ID[n]      游戏世界ID:GT_INVALID代表该大区下所有游戏世界	
CMD_END

//-----------------------------------------------------------------------
// 取消所有已设定的自动循环
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelAutoNotice)
CMD_END

CMD_START(NGMTS_CancelAutoNotice)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
//--与时间有关的返回消息
//-----------------------------------------------------------------------
CMD_START(NGMTS_TimeLimitedMsg)
	ETimeLimitedMsg	eType;	
	DWORD			dwErrorCode;					// 0-成功
	DWORD			dwWorldID;
CMD_END

//-----------------------------------------------------------------------
// 添加/删除黑名单IP
//-----------------------------------------------------------------------
CMD_START(NGMTC_Blacklist)
	TCHAR			szSectionName[32];		// 大区
	DWORD			dwIP;								// IP
	INT				nAction;							// 0:解封 1：封
CMD_END

CMD_START(NGMTS_Blacklist)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 聊天频道自动公告
//-----------------------------------------------------------------------
CMD_START(NGMTC_AutoChatNotice)
	TCHAR			szNotice[X_HUGE_STRING];			// 公告内容
	BYTE			byHour;								// 发送: 时
	BYTE			byMinute;							// 发送: 分
	DWORD			dwSectionID;						// 大区ID
	DWORD			dwWorldID[1];						// 游戏世界ID[n]      游戏世界ID:GT_INVALID代表该大区下所有游戏世界	// 发布时间
CMD_END

CMD_START(NGMTS_AutoChatNotice)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 取消所有已设定聊天频道消息
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelAutoChatNotice)
CMD_END

CMD_START(NGMTS_CancelAutoChatNotice)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 取消所有计时消息
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelTimeLimitedMsg)
CMD_END

CMD_START(NGMTS_CancelTimeLimitedMsg)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 取消所有双倍设置
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelDoubleMsg)
	//DWORD			dwSectionID;						// 大区ID
	//DWORD			dwWorldID[1];						// 游戏世界ID[n]      游戏世界ID:GT_INVALID代表该大区下所有游戏世界	// 发布时间
CMD_END

CMD_START(NGMTS_CancelDoubleMsg)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 取消所有已设定右下角
//-----------------------------------------------------------------------
CMD_START(NGMTC_CancelRightNotice)
CMD_END

CMD_START(NGMTS_CancelRightNotice)
	DWORD			dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 清空指定玩家的百宝袋
//-----------------------------------------------------------------------
CMD_START(NGMTC_ClearBaibao)
	TCHAR	szOwnerName[X_SHORT_NAME];
	DWORD	dwSectionID;
	DWORD	dwWorldID;
CMD_END

CMD_START(NGMTS_ClearBaibao)
	DWORD			dwErrorCode;
CMD_END

CMD_START(NGMTS_GMSetFackNumer)
	float			fFackRate;
CMD_END

//-----------------------------------------------------------------------
// 角色禁言消息
//-----------------------------------------------------------------------
CMD_START(NGMTC_RoleSpeak)
	TCHAR	szRoleName[X_SHORT_NAME];
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	bool	bAction;		// 0 禁言 1解禁
CMD_END

CMD_START(NGMTS_RoleSpeak)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 处理卡号的消息
//-----------------------------------------------------------------------
CMD_START(NGMTC_KickLockedRole)
	TCHAR	szAccountName[X_SHORT_NAME];
	DWORD	dwSectionID;
CMD_END

CMD_START(NGMTS_KickLockedRole)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 发称号的消息
//-----------------------------------------------------------------------
CMD_START(NGMTC_IssueTitle)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szRoleName[X_SHORT_NAME];
	DWORD	dwTitleID; 
CMD_END

CMD_START(NGMTS_IssueTitle)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 删称号的消息
//-----------------------------------------------------------------------
CMD_START(NGMTC_DelTitle)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szRoleName[X_SHORT_NAME];
	DWORD	dwTitleID; 
CMD_END

CMD_START(NGMTS_DelTitle)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 移动角色的消息
//-----------------------------------------------------------------------
CMD_START(NGMTC_MoveRole)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szRoleName[X_SHORT_NAME];
	TCHAR	szMapName[X_SHORT_NAME];
	INT		nX;
	INT		nZ;
CMD_END

CMD_START(NGMTS_MoveRole)
	DWORD	dwErrorCode;
	TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
// 清除角色已配的属性点的消息
//-----------------------------------------------------------------------
CMD_START(NGMTC_ClearAttPoint)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_ClearAttPoint)
	DWORD	dwErrorCode;
	TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
// 修改帮派成员的等级
//-----------------------------------------------------------------------
CMD_START(NGMTC_GuildChgMemLvl)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szGuildName[X_SHORT_NAME];
	TCHAR	szRoleName[X_SHORT_NAME];
	INT		nLvl;
CMD_END

CMD_START(NGMTS_GuildChgMemLvl)
	DWORD	dwErrorCode;
	//TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
// 修改帮派资金
//-----------------------------------------------------------------------
CMD_START(NGMTC_GuildChgFund)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szGuildName[X_SHORT_NAME];
	INT32	n32Fund;
CMD_END

CMD_START(NGMTS_GuildChgFund)
	DWORD	dwErrorCode;
	//TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
// 修改帮派状态
//-----------------------------------------------------------------------
CMD_START(NGMTC_GuildChgState)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	TCHAR	szGuildName[X_SHORT_NAME];
	DWORD	dwState;
CMD_END

CMD_START(NGMTS_GuildChgState)
	DWORD	dwErrorCode;
	//TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
//  PK log
//-----------------------------------------------------------------------
CMD_START(NGMTC_AddNeedPKLogRole)
	DWORD	dwSectionID;
	DWORD	dwWorldID;
	DWORD	dwRoleID;
	TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_AddNeedPKLogRole)
	DWORD	dwErrorCode;
CMD_END

enum EActionType;
struct tagLoadLogItem
{
	//EActionType eType;
	TCHAR	szGMName[X_SHORT_NAME];
	TCHAR	szTime[X_SHORT_STRING];
	TCHAR	szIP[X_SHORT_STRING];
	TCHAR	szDetails[X_HUGE_STRING];
};

CMD_START(NGMTC_LoadLog)
	EActionType	eType1;
	EActionType eType2;
CMD_END

CMD_START(NGMTS_LoadLog)
	INT	nNum;
	EActionType eType;
	tagLoadLogItem	item[1];
CMD_END

//添加广告过滤词
CMD_START(NGMTC_AddFilterWords)
DWORD	dwSectionID;
DWORD	dwWorldID;
TCHAR	szFilterWords[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_AddFilterWords)
DWORD  dwErrorCode;
CMD_END

//删除广告过滤词
CMD_START(NGMTC_DelFilterWords)
DWORD	dwSectionID;
DWORD	dwWorldID;
TCHAR	szFilterWords[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_DelFilterWords)
DWORD  dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 修改帮派据点战占领状态
//-----------------------------------------------------------------------
CMD_START(NGMTC_GuildGodMiracle)
DWORD	dwSectionID;
DWORD	dwWorldID;
TCHAR	szGuildName[X_SHORT_NAME];
DWORD	dwMiracleType;
CMD_END

CMD_START(NGMTS_GuildGodMiracle)
DWORD	dwErrorCode;
//TCHAR	szRoleName[X_SHORT_NAME];
CMD_END

//-----------------------------------------------------------------------
// 清空帐号安全码
//-----------------------------------------------------------------------
CMD_START(NGMTC_ClearSafeCode)
DWORD	dwSectionID;
DWORD	dwWorldID;
TCHAR	szAccountName[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_ClearSafeCode)
DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------
// 关闭防沉迷
//-----------------------------------------------------------------------
CMD_START(NGMTC_CloseGuard)
DWORD	dwSectionID;
DWORD	dwWorldID;
TCHAR	szAccountName[X_SHORT_NAME];
CMD_END

CMD_START(NGMTS_CloseGuard)
DWORD	dwErrorCode;
CMD_END

#pragma pack(pop)
