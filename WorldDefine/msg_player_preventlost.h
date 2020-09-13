#pragma once
#pragma pack(push, 1)

#include "msg_common_errorcode.h"

CONST	UINT16		DOUBLE_EXP_QUEST_ID		=	10848;		// 双倍经验任务ID
CONST	UINT16		FISHING_QUEST_ID		=	79;			// 钓鱼任务ID

enum ELoongBenediction
{
	ELBD_Success			= 0,			// 成功
	ELBD_Bag_NotEnoughSpace	= 1,			// 背包已经满了
	ELBD_Time_NotArrive		= 2,			// 时间还未到
	ELBD_No_LessingProto	= 3,			// 没有找到赐福原型配置信息
	ELBD_Item_Create_Failed	= 4,			// 物品创建失败
	ELBD_No_Benediction		= 5,			// 没有了

};

enum EOfflineExp
{
	EOFFE_NotEnoughYuanBao = 1
};

//-----------------------------------------------------------------------------
// 防止玩家流失，包括下线提示，神龙赐福
//-----------------------------------------------------------------------------

CMD_START(NC_ExitGamePrompt)
CMD_END

CMD_START(NS_ExitGamePrompt)
	BOOL		bDualExpQuestDo;				//今日是否完成,双倍杀怪经验的任务 id = 10848
	BOOL		bFishQuestDo;					//今日是否完成，钓鱼任务 id = 79
	INT32		nExprence;						//钓鱼一小时获得的经验
	BOOL		bCanGetBenediction;				//现在是可以领取神龙赐福的(已经领完了前20次，并且今天还未领取过)
	DWORD		dwHour;							//距离领取下一次神龙赐福还要几个小时的在线时间
	DWORD		dwMinute;						//距离领取下一个神龙赐福还要多少分钟在线时间
CMD_END


//-----------------------------------------------------------------------------
// 神龙赐福
//-----------------------------------------------------------------------------
CMD_START(NC_GetLoongBenediction)			//获取奖励
CMD_END

CMD_START(NS_GetLoongBenediction)
	DWORD		dwErrorCode;
	DWORD		nItemTypeID;				//奖励的物品的ID
	INT			nItemCount;					//物品奖励的个数
	INT64		nExprence;					//奖励的经验的ID
CMD_END

CMD_START(NS_SetNextBenediction)
	BOOL			isEnd;					//神龙赐福是否结束的标志
	DWORD			nextServerTime;			//下一次神龙赐福的时间，剩余秒数
	DWORD			nItemTypeID;			//获得的物品奖励
	INT				nItemCount;				//物品奖励的个数
	INT64			n32Exprence;			//经验奖励
CMD_END

CMD_START(NS_LoongBenedictionInit)			//告诉客户端需要初始化这个模块
	INT				nType;					//领取模式，0 按照次数领默认20次，1按照每日在线时间领
CMD_END

//-----------------------------------------------------------------------------
// 离线挂机
//-----------------------------------------------------------------------------
CMD_START(NC_GetOfflineExp)					// 获取离线经验
	BYTE			byType;					// 领取类型，0表示领取免费经验，1表示领取全部经验
CMD_END

CMD_START(NS_GetOfflineExp)
	DWORD			dwErrorCode;
	DWORD			dwExperience;			//或者的经验
CMD_END

// Jason 2010-1-15 v1.3.2 离线挂机 接收经验奖励协议
CMD_START(NS_OfflineExperienceReward)
	BYTE			byHour;					// 离线小时数
	BYTE			byMinute;				// 离线分钟数
	INT64			n64FreeExperience;		// 免费经验奖励
	INT64			n64PayNeededExperience;	// 需付费经验奖励
	INT				nYuanBao;				// 提取额外经验需要的元宝
CMD_END

// Jason 2010-1-15 v1.3.2 玩家回归
CMD_START( NS_PlayerRegression )
	DWORD dwItemTypeID;						// 领取物品的typeID
	INT   nItemNum;							// 物品个数
	BYTE  byQuality;						// 物品品质
CMD_END

CMD_START( NC_PlayerRegressionOk )			// 老玩家点击领取
CMD_END

CMD_START( NS_PlayerRegressionOk )			// 老玩家点击领取
	DWORD dwErrorCode;
CMD_END
// 活动结束
//CMD_START( NS_PlayerRegressionEnd ) 
//CMD_END


#pragma pack(pop)
