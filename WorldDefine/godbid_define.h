#pragma once

const int MAX_GODBID_ITEM_NUM = 3;

// 神之竞标系统类型
enum EGodBidType
{
	EGBT_Start = -1,

	EGBT_Normal = 0,								// 普通模式
	EGBT_BlackMarket_Gold = 1,				// 黑市金币
	EGBT_BlackMarket_ShengBi = 2,		// 黑市元宝

	EGBT_End
};

struct tagBidData
{
	DWORD	dwID;						// id
	DWORD  dwItemID;				// 物品ID
	int			nNum;						//	数量
	EGodBidType	eType;						// 类型
	DWORD	dwStartPrice;			// 起始价
	DWORD	dwMinBidPrice;		// 出价底限
	tagBidData():dwID(0),dwItemID(0),nNum(0),eType(EGBT_Start),dwStartPrice(0), dwMinBidPrice(0){}
	tagBidData& operator=(const tagBidData&rv)
	{
		if(this==&rv)
			return *this;

		dwID = rv.dwID;
		dwItemID = rv.dwItemID;
		nNum = rv.nNum;
		eType = rv.eType;
		dwStartPrice = rv.dwStartPrice;
		dwMinBidPrice = rv.dwMinBidPrice;
		return *this;
	}

	void clear()
	{
		dwID = 0;
		dwItemID = 0;
		eType = EGBT_Start;
		dwStartPrice = 0;
		dwMinBidPrice = 0;
	}
};

// 普通竞价物品信息
struct tagGodBidItemInfo
{
	tagBidData	sBidData;				
	INT64		n64CurPrice;				// 当前价格
	bool			bHighestPrice;				// 是否最高出价者
	INT64		n64PlayerOfferPrice;	// 玩家已经出的价钱

	tagGodBidItemInfo():n64CurPrice(0),n64PlayerOfferPrice(0),bHighestPrice(false){}
 	tagGodBidItemInfo& operator=(const tagGodBidItemInfo& rv)
 	{
		if(this==&rv)
			return *this;

		sBidData = rv.sBidData;
 		n64CurPrice = rv.n64CurPrice;
 		bHighestPrice = rv.bHighestPrice;
 		n64PlayerOfferPrice = rv.n64PlayerOfferPrice;
 		return *this;
 	}

	void clear()
	{
		n64CurPrice = 0;
		bHighestPrice = false;
		n64PlayerOfferPrice = 0;
		sBidData.clear();
	}
};

// 黑市竞价物品信息
struct tagBlackMarketItemInfo
{
	tagBidData	sBidData;				
	bool				bIsBid;							// 是否已竞价

	tagBlackMarketItemInfo():bIsBid(false){}
	tagBlackMarketItemInfo& operator= (const tagBlackMarketItemInfo& rv)
	{
		if(this==&rv)
			return *this;

		sBidData = rv.sBidData;
		bIsBid = rv.bIsBid;
		return *this;
	}

	void clear()
	{
		bIsBid = false;
		sBidData.clear();
	}
};