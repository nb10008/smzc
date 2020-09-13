#pragma once
#include "godbid_define.h"
#pragma pack(push, 1)

enum EGodBidErrorCode
{
	EGBEC_Success = 0,
	EGBEC_Black_Only_One_Chance = 1,		// 黑市竞标只有一次机会
	EGBEC_Black_OutOfTime = 1,				// 不在竞价时间
};

//-----------------------------------------------------------------------------
// 打开神之竞价商
//-----------------------------------------------------------------------------
CMD_START(NC_GodBiderShopOpen)
	DWORD		dwNPCID;
CMD_END

CMD_START(NS_GodBiderShopOpen)
	DWORD						dwErrorCode;
	tagGodBidItemInfo		bidItemInfo[MAX_GODBID_ITEM_NUM];	// 竞标物品信息
	DWORD						dwLeftTime;	// 还剩余多少时间结束，以秒计算
CMD_END

//-----------------------------------------------------------------------------
// 确认竞价(普通商人)
//-----------------------------------------------------------------------------
CMD_START(NC_ConformPrice)
	INT64			n64Price;				//	价钱
	DWORD		dwFormulaID;		// 公式ID
CMD_END

CMD_START(NS_ConformPrice)
	DWORD		dwErrorCode;
	INT64			n64Price;				//	价钱
CMD_END

//-----------------------------------------------------------------------------
// 打开黑市商人
//-----------------------------------------------------------------------------
CMD_START(NC_BlackMarketShopOpen)
	DWORD		dwNPCID;
CMD_END

CMD_START(NS_BlackMarketShopOpen)
	DWORD								dwErrorCode;
	tagBlackMarketItemInfo		GoldItemInfo;			// 游戏币物品信息
	tagBlackMarketItemInfo		ShengBiItemInfo;	// 圣币物品信息
	DWORD								dwLeftTime;			// 还剩余多少时间结束，以秒计算
CMD_END

//-----------------------------------------------------------------------------
// 确认竞价(黑市商人)
//-----------------------------------------------------------------------------
CMD_START(NC_BlackConPrice)
	INT64			n64Price;				//	价钱
	DWORD		dwFormulaID;		// 公式ID（交易类型是圣币还是金，可以在公式中查找）
CMD_END

CMD_START(NS_BlackConPrice)
	DWORD		dwErrorCode;
	bool				bIsGod;				// ture 是金，false是圣币
	INT64			n64Price;				//	价钱
CMD_END

#pragma pack(pop)