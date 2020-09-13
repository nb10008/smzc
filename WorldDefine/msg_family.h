//////////////////////////////////////////////////////////////////////////
// 家族系统消息定义
//////////////////////////////////////////////////////////////////////////
#pragma once
#pragma pack(push, 1)
#include "..\WorldDefine\family_define.h"

//////////////////////////////////////////////////////////////////////////
// common
//////////////////////////////////////////////////////////////////////////
enum
{
	E_FAMILY_SUCCESS =	0,

	E_CreateFamily_NoTeam =1,									// 必须4人或4人以上组队才可创建家族
	E_CreateFamily_NotLeader=2,								// 只有队长才可创建家族
	E_CreateFamily_TeamMemNotEnough=3,				// 您的队伍人数必须≥4人，否则无法创建家族
	E_CreateFamily_MemberHaveFamily=4,					// 您的队伍成员有人已经加入了其他家族，无法创建
	E_CreateFamily_MemberJustLeaveFamily=5,			// 您的队伍成员里有人刚脱离家族，无法创建
	E_CreateFamily_NameExist=6,								// 重名啦

	E_DismissFamily_NotLeader=7,								// 您不是族长，无法解散家族
	E_DismissFamily_FamilyNotEmpty=8,					// 家族里还有其他成员，解散家族必须在家族里没有其他任何成员的情况下进行

	E_InviteRole_NoRole=9,											// 角色不存在，无法进行邀请
	E_InviteRole_RoleHadFamily=10,								// 对方拥有其他家族，无法进行邀请
	E_InviteRole_FamilyFull=11,										// 家族的人数已经饱和，无法再邀请新的玩家
	E_InviteRole_JustLeaveFamily=12,							// 对方刚刚脱离其他家族，暂时无法加入新的家族

	E_ApplyJoin_JustLeaveFamily=13,							// 您刚脱离家族，暂时还无法加入新的家族
	E_ApplyJoin_DestFamilyFull=14,								// 对方的家族人数已满，无法申请
	E_ApplyJoin_DestRoleOffLine=15,							// 目标已不在线，无法申请
	E_ApplyJoin_FamilyNotExist=16,								// 目标家族不存在
	E_ApplyJoin_RoleHadFamily=17,								// 您已经属于其他家族，无法进行申请
	
	E_LeaveFamily_BeLeader=18,									// 您是族长，无法脱离家族
	E_LeaveFamily_BeMarried=19,									// 您已经结婚，若想脱离家族，必须先解除您的夫妻关系

	E_KickMember_CanNotKickSelf=20,							// 您无法开除自己
	E_KickMember_NotLeader=21,									// 您无法开除自己

	E_HandOverLeader_NotLeader=22,							// 只有族长可以移交权限
	E_HandOverLeader_TargetNotInFamily=23,				// 目标不是家族成员，不能移交族长给他

	// 家族任务相关错误码
	E_FamilyQuest_GiveInItem_RoleInvalid = 30,	// 上缴物品的角色无效
	E_FamilyQuest_GiveInItem_QuestInvalid=31,			// 家族任务ID无效
	E_FamilyQuest_GiveInItem_QuestNotMatch=32,	// 角色的家族无此任务
	E_FamilyQuest_GiveInItem_ItemNotEnough=33,		// 背包内物品不足
	
	E_FamilyQuest_Rewarded_Error=34,
	E_FamilyQuest_Rewarded_ProcessLimit=35,
	E_FamilyQuest_Rewarded_NewJoiner=36,
	E_FamilyQuest_Rewarded_AlreadyDone=37,
	E_FamilyQuest_Rewarded_BagFull=38,
	
	E_FamilyQuest_GiveInItem_NotQuestItem = 39,	// 上缴的物品不合法
	E_FamilyQuest_GiveInItem_QuestItemEnough = 40,
	// 

	E_FamilyJoin_Refuse			= 51,					// 拒绝加入
	E_FamilyApply_Refuse		= 52,					// 拒绝申请
	E_FamilyJoin_TimeOut		= 53,					// 邀请超时
	E_FamilyApply_TimeOut     = 54,					// 申请超时
};

// 操作失败反馈
CMD_START(NS_FamilyProcFailed)
	DWORD			dwErrorCode;
CMD_END

// 创建家族
CMD_START(NC_CreateFamily)
	DWORD		dwNPCID;
	TCHAR		szFamilyName[1];
CMD_END

CMD_START(NS_CreateFamilyBroad)
	DWORD		dwRoleID;		// 创建者id
	DWORD		dwFamilyID;
	TCHAR			szName[1];		// 角色名+家族名
CMD_END

CMD_START(NS_CreateFamilyAllBroad)
	TCHAR		szRoleName[X_SHORT_NAME];		// 创建者名字
	TCHAR		szFamilyName[X_SHORT_NAME];		// 家族名
CMD_END

// 解散家族
CMD_START(NC_FamilyDismiss)
	DWORD		dwNPCID;
CMD_END

CMD_START(NS_FamilyDismiss)
CMD_END

// 邀请加入
CMD_START(NC_FamilyJoinReq)		// 玩家请求目标加入家族
	DWORD		dwDstRoleID;
	TCHAR		szDstRoleName[1];
CMD_END

CMD_START(NS_FamilyJoinReq)		// 服务器将请求发送给目标玩家
	DWORD		dwSrcRoleID;
	DWORD		dwFamilyID;
	TCHAR		szSrcRoleName[1];
CMD_END

CMD_START(NC_FamilyJoinReqRes)	// 目标玩家反馈该请求
	DWORD		dwErrorCode;
	DWORD		dwDstRoleID;
	DWORD		dwDstFamilyID;
CMD_END

CMD_START(NS_FamilyJoinReqRes)	// 服务器转发给目标玩家反馈
	DWORD		dwErrorCode;
	DWORD		dwInviterID;
	DWORD		dwInviteeID;
CMD_END

CMD_START(NS_FamilyJoinBroad)	// 家族内
	DWORD		dwRoleID;
	DWORD		dwFamilyID;
	TCHAR		szRoleName[1];
CMD_END

// 申请加入
CMD_START(NC_FamilyApply)		// 玩家申请加入家族
	DWORD		dwApplyRoleID;
	TCHAR		szDstRoleName[1];
CMD_END

CMD_START(NS_FamilyApply)		// 服务器将请求发送给族长
	DWORD		dwApplyRoleID;
	TCHAR		szApplyRoleName[1];
CMD_END

CMD_START(NC_FamilyApplyRes)	// 族长反馈该请求
	DWORD		dwErrorCode;
	DWORD		dwDstRoleID;
	DWORD		dwDstFamilyID;
CMD_END

CMD_START(NS_FamilyApplyReqRes)	// 服务器转发给目标玩家反馈
	DWORD		dwErrorCode;
	DWORD		dwDstRoleID;
CMD_END

//此条与邀请加入的公用
//CMD_START(NS_FamilyJoinBroad)	// 家族内

// 玩家离开家族
CMD_START(NC_FamilyLeave)
CMD_END

CMD_START(NS_FamilyLeaveBroad)
	DWORD		dwSrcRoleID;
CMD_END

// 开除玩家出家族
CMD_START(NC_FamilyKick)
	DWORD		dwRoleID;
CMD_END

CMD_START(NS_FamilyKickBroad)
	DWORD		dwDstRoleID;
CMD_END

// 移交族长
CMD_START(NC_FamilyHandOver)
	DWORD		dwRoleID;
CMD_END

CMD_START(NS_FamilyHandOverBroad)
	DWORD		dwSrcRoleID;
	DWORD		dwDstRoleID;
CMD_END

// 获取家族名称
CMD_START(NC_GetFamilyName)
DWORD		dwFamilyID;
CMD_END

CMD_START(NS_GetFamilyName)
DWORD		dwFamilyID;
TCHAR		szFamilyName[1];
CMD_END

// 获取家族信息（客户端）
CMD_START(NC_GetFamilyInfo)
DWORD		dwFamilyID;
CMD_END

CMD_START(NS_GetFamilyInfo)
INT8					n8MemberNum;
tagRoleFamilyInfo		sFamilyInfo[1]; 
CMD_END

// 同步家族活跃度
CMD_START(NS_SyncFamilyActive)
	INT nActive;
CMD_END

//////////////////////////////////////////////////////////////////////////
// 家族妖精相关
//////////////////////////////////////////////////////////////////////////

//妖精修炼
CMD_START(NC_FamilySpriteTrain) 
	DWORD dwTrainID;		// 修炼ID
	INT64	n64ID;				// 修炼物品的64ID
	BOOL bAgain;			// 是否再次判断
CMD_END

CMD_START(NS_FamilySpriteTrain)
	DWORD	dwTrainID;		//修炼ID
	INT		nTrainRet;		//提高的数量
	DWORD	dwErrorCode;
CMD_END


//妖精登记
CMD_START(NC_FamilySpriteRegister) 
	INT64 n64id;
	//BOOL  bAgain;			// 是否再次判断
CMD_END

CMD_START(NS_FamilySpriteRegister)
	DWORD dwErrorCode;
CMD_END

//取回妖精
CMD_START(NC_FamilySpriteGetBack) 
CMD_END

CMD_START(NS_FamilySpriteGetBack)
	DWORD dwErrorCode;
CMD_END

//获取妖精修炼内容
CMD_START(NC_GetFamilySpriteTrain) 
CMD_END

CMD_START(NS_GetFamilySpriteTrain)
	DWORD	dwFamilyID;		// 家族ID
	DWORD	dwErrorCode;
	INT		nNum;
	tagFamilySpriteRequirementSave	ftsi[1];
CMD_END

//获取妖精登记信息
CMD_START(NC_GetFamilyRoleSprite) 
CMD_END

CMD_START(NS_GetFamilyRoleSprite)
	DWORD	dwFamilyID;		// 家族ID
	DWORD	dwErrorCode;
	INT		nNum;
	tagRoleSpriteDataSave frsi[1];
CMD_END

//获取妖精登记信息
CMD_START(NC_GetFamilySpriteAtt) 
CMD_END

CMD_START(NS_GetFamilySpriteAtt)
	DWORD dwSpriteAtt[EFSA_Num];		// -1代表未开启
CMD_END

//###完成守护妖精的修炼，守护妖精的###（能力类型）提升###（提升量）
CMD_START(NS_FamilySpritTrainBroad) 
	TCHAR	tszRoleName[X_SHORT_NAME];	
	DWORD	dwType;
	INT		nRet;
CMD_END

//获取家族妖精信息
CMD_START(NC_GetFamilySpriteInfo) 
CMD_END

CMD_START(NS_GetFamilySpriteInfo)
	INT		   nLevel;		// 等级
	DWORD dwExp;		// 当前的经验
	DWORD dwSpriteAtt[EFSA_Num];		// 当前的att值 
CMD_END
//////////////////////////////////////////////////////////////////////////
// 家族任务相关
//////////////////////////////////////////////////////////////////////////

// 打开家族任务界面
CMD_START(NC_OpenFamilyQuestPage) 
CMD_END

CMD_START(NS_OpenFamilyQuestPage)
	INT				nProcess;			// 昨天完成度
	bool			bIsReceive;			// 是否领取奖励
	BYTE			byQuestNum;			// 家族任务个数
	tagFamilyQuest	Quests[1];			// 具体任务
CMD_END

// 昨日家族任务完成情况
CMD_START(NC_YesterdayQuestInfo)
CMD_END

CMD_START(NS_YesterdayQuestInfo)
	INT				nYesterdayPercent;	// 昨日完成度
	bool			bIsGetReward;		// 是否领取奖励
CMD_END

// 上缴任务物品
CMD_START(NC_GiveInFamilyQuestItem)
	DWORD			dwQuestID;			// 任务ID
	DWORD			dwTypeID;			// 上缴物品的TypeID(上缴物品的时候有效)
	INT64				n64EquipSerial;	// 上缴装备的64位ID(上缴装备的时候有效)
	INT				nNum;				// 物品数量
CMD_END

CMD_START(NS_GiveInFamilyQuestItem)
	DWORD			dwErrorCode;
	DWORD			dwQuestID;			// 任务ID
	INT				nCount;				// 任务计数
CMD_END

// 领取奖励
CMD_START(NC_RewardFamilyMember) 
CMD_END

CMD_START(NS_RewardFamilyMember)
	DWORD			dwErrorCode;
CMD_END

#pragma pack(pop)