//-----------------------------------------------------------------------------
// filename: msg_strength.h
// author: 
// actor:
// data: 2009-09-09
// last: 
// brief: 实力系统相关消息格式
//-----------------------------------------------------------------------------
#pragma once
#include "../ServerDefine/rankings_define.h"

#pragma pack(push, 1)
//-----------------------------------------------------------------------------
// 发自：world 发往：db server 请求初始化实力排行榜
//-----------------------------------------------------------------------------
CMD_START(NDBC_InitRankings)
CMD_END;

//-----------------------------------------------------------------------------
// 发自：db server 发往：world 通知world排行榜更新完毕
//-----------------------------------------------------------------------------
CMD_START(NDBC_UpdateRankingsDone)
CMD_END;

//-----------------------------------------------------------------------------
// 发自：db server 发往：world 通知world神职竞选完毕
//-----------------------------------------------------------------------------
// CMD_START(NDBS_ClergyVoteDone)
// CMD_END;

//-----------------------------------------------------------------------------
// 发自：db server 发往：world 通知world取消所有在上期神职竞选中获得神职的角色清除其神职
//-----------------------------------------------------------------------------
CMD_START(NDBS_ClearAllCompetitiveClergy)
CMD_END

//-----------------------------------------------------------------------------
// 发自：world 发往：db server 请求初始化实力排行榜
//-----------------------------------------------------------------------------
CMD_START(NDBC_RequestInitClergyRank)
CMD_END;

//-----------------------------------------------------------------------------
// 发自：db server 发往：world 将实力排行榜发给world
//-----------------------------------------------------------------------------
CMD_START(NDBS_InitRankings)
	INT						nSum;							// 总记录条数，也就是排行榜人数，最大500
	ERankingType			eType;							// 排行类型
	tagRankingsNode			Rankings[MAX_RANKINGS_LEN];		// 按名次发送各角色的信息
CMD_END;

CMD_START(NDBS_InitPetRankings)
	INT						nSum;							// 总记录条数，也就是排行榜人数，最大500
	ERankingType			eType;							// 排行类型
	tagPetRankingsNode		Rankings[MAX_RANKINGS_LEN];		// 按名次发送各角色的信息
CMD_END;

CMD_START(NDBS_InitFlowerRankings)
	INT						nSum;							// 总记录条数，最大60
	ERankingType			eType;							// 排行类型
	tagRankingsNode			Rankings[MAX_FlowerEggRankings_LEN];	//按名次发送各角色信息
CMD_END


//-----------------------------------------------------------------------------
// 发自：world  发到：db server  每天零点请求更新各排行榜
//-----------------------------------------------------------------------------
CMD_START(NDBC_UpdateRankings)
CMD_END;

//-----------------------------------------------------------------------------
// 发自：db server 发到：world  返回某个排行榜更新的结果
//-----------------------------------------------------------------------------
CMD_START(NDBS_UpdateRankings)
	ERankingType		eType;
	DWORD				dwErrorCode;
CMD_END;

//-----------------------------------------------------------------------------
// 发自：db server  发到：world  周一早八点通知world更新玩家的进阶实力值和可消耗实力值
//-----------------------------------------------------------------------------
CMD_START(NDBC_RenewAdvanceStrength)
CMD_END;

CMD_START(NDBC_SyncCanRankFlag)
	DWORD dwRoleID;
	BYTE	byCanRankFlag;
CMD_END;

//-----------------------------------------------------------------------------
// 发自：db server 发往：world 将神职竞选结果发给world
//-----------------------------------------------------------------------------
CMD_START(NDBS_InitClergyRank)
	INT						nSum;							// 总记录条数
	ECampaignGodType		eType;							// 神系
	tagClergyNode			Rankings[MAX_CampaignInfoNumPerType];	// 按名次发送各角色的信息
CMD_END;

//-----------------------------------------------------------------------------
// 发自：db server 发往：world 通知world更新所有在线用户的神职竞选报名状态
//-----------------------------------------------------------------------------
CMD_START(NDBS_RenewClergyCandidateType)
CMD_END;

//-----------------------------------------------------------------------------
// 改变指定角色的神职领取状态
//-----------------------------------------------------------------------------
CMD_START(NDBC_CandidateRecvClergy)
	ECampaignGodType	eType;
	INT					nNum;
	DWORD				dwRoleID[1];
CMD_END

//-----------------------------------------------------------------------------
// 请求获得当前各系神职报名人数
//-----------------------------------------------------------------------------
CMD_START(NDBC_Ask4CandidateCount)
CMD_END

CMD_START(NDBS_Ask4CandidateCount)
	DWORD				dwCount[ECGT_Num];
CMD_END

//-----------------------------------------------------------------------------
// 取消指定角色的神职，设置其恢复上期神职
//-----------------------------------------------------------------------------
CMD_START(NDBC_CancelPreClergy)
	INT					nNum;
	DWORD				dwRoleID[1];
CMD_END

#pragma pack(pop)