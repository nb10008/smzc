//-----------------------------------------------------------------------------
//!\file msg_spirit.h
//!
//!\date 2010-01-15
//!
//!\brief 客户端与服务器之间关于精力值消息
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

enum ESpiritError
{
	E_InalidSprirtValue = 1,
	E_BagIsFull,
	E_WrongRequestNum,
	E_NoRequestItem,
	E_StrengthNotEnough,
};

CMD_START(NC_SpiritReward)				// 获取精力值的回报
CMD_END

CMD_START(NS_SpiritReward)				// 获取精力值的回报
	DWORD	dwError;
CMD_END

CMD_START(NC_StrengthReward)			// 实力值换取精力值道具
	DWORD	dwTypeID;
	INT		nNum;	
CMD_END

CMD_START(NS_StrengthReward)			// 实力值换取精力值道具
	DWORD	dwError;	
CMD_END

#pragma pack(pop)