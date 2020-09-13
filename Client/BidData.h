#pragma once

enum EBidType
{
	EBT_Normal = 0,
	EBT_BlackGold,
	EBT_BlackShengBi
};
struct tagBidDataLocal
{
	DWORD	dwID;						// id
	DWORD  dwItemID;				// 物品ID
	int			nNum;						//	数量
	EBidType	eType;						// 类型
	DWORD	dwStartPrice;			// 起始价
	DWORD	dwMinBidPrice;		// 出价底限
};

class BidData
{
	typedef map<DWORD,tagBidDataLocal> BidMap;
public:
	BidData(void);
	~BidData(void);

	void LoadFormFile();
	const tagBidDataLocal* GetBidData(const DWORD& id) const; 
private:
	BidMap m_mapBidMap;
};
