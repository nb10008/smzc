//-----------------------------------------------------------------------
// 神之竞标系统
//-----------------------------------------------------------------------

#pragma once

struct tagGodBidProto;
struct tagGodBidInfo2DB;

#define GodBid_Time_limit 3600		// 神之竞标出价时限（单位为秒）

const DWORD GODBID_START_BROADCAST					= 300011;		// 神之竞标活动开始
const DWORD GODBID_END_BROADCAST						= 300012;		// 神之竞标活动结束
const DWORD GODBID_NORMAL_END_BROADCAST		= 300013;		// 恭喜xx以xx金竞得xx物品
const DWORD GODBID_BLACK_GOLD_BROADCAST		= 300014;		// 黑市--恭喜xx以xx金竞得xx物品
const DWORD GODBID_BLACK_SHENGBI_BROADCAST	= 300015;		// 黑市--恭喜xx以xx圣币竞得xx物品
const DWORD GODBID_ALL_END_BROADCAST				= 300016;		// 竞价所得物品已发送至百宝袋内领，未竞得物品的玩家，出价资金已全数返还至背包内

// 竞标竞价信息
struct tagBidInfo 
{
	DWORD	dwRoleID;		// 角色ID
	INT64		n64Bidden;		// 出价
};

typedef TMap<DWORD, INT64> BidInfoMap;
typedef TList<INT16>	BidStartTimeList;

// 竞标竞价信息节点
struct tagBidInfoNode
{
	DWORD			dwGodBidProtoID;
	tagBidInfo		CurHighestBidInfo;				// 当前最高的出价信息
	BidInfoMap*	pBidInfoMap;

	tagBidInfoNode()
	{
		dwGodBidProtoID = GT_INVALID;
		CurHighestBidInfo.dwRoleID = 0;
		CurHighestBidInfo.n64Bidden = 0;
		pBidInfoMap = NULL;
	}
};

class GodBidMgr
{
public:
	GodBidMgr();
	~GodBidMgr();

	VOID Update();

	DWORD GetBidEndLeftTime();

	// 初始化竞标开始时间list
	VOID InitFixedBidStartTimeList(TObjRef<VarContainer>& pVar, BOOL bFixed);
	VOID InitGodBidSwitch(TObjRef<VarContainer>& pVar);

	BOOL IsBidTime() { return m_bBidTime; }
	VOID SetIsBidTime(BOOL bBidTime) { m_bBidTime = bBidTime; }
	BOOL IsSwitchOn() { return m_bSwitchOn; }
	VOID SetBidEnding(BOOL bBidEnding) { m_bBidEnding = bBidEnding; }
	BOOL IsBidEnding() { return m_bBidEnding; }
	VOID ClearGodBidInfoFromMemory();

	BOOL ReLoadGodBidProto();

	// 处理客户端请求
	VOID SendNormalBidInfo2Client(Role* pRole);
	VOID SendBlackMarketBidInfo2Client(Role* pRole);
	VOID ProcessNormalBidConformPrice(DWORD dwProtoID, Role* pRole, INT64 n64Price);
	VOID ProcessBlackBidConformPrice(DWORD dwProtoID, Role* pRole, INT64 n64Price);

	// 处理数据库操作
	VOID InitGodBidInfoFromDB(DWORD dwBidCount, tagGodBidInfo2DB* pGodBidInfo2DB);
	VOID SaveOneGodBidInfo2DB(DWORD dwRoleID, INT64 n64Bidden, BYTE byBidType, DWORD dwBidID);
	VOID ClearGodBidInfoFromDB(BOOL bBidEnd);
	VOID LoadGodBidInfoFromDB();

	VOID BidEnd();

private:
	VOID BidStart();
	BOOL CheckCurIsBidTime();
	BOOL CheckIsBidTime(BidStartTimeList& listBidStartTime, tagDWORDTime dwTime);
	VOID InitBidItems();
	VOID SendBroadcast(DWORD dwBroadcastID);
	VOID SendBroadcast(DWORD dwBroadcastID, DWORD dwRoleID, DWORD dwMoney, DWORD dwItemID);
	VOID NormalBidEnd();

	// 注意，调用此方法要在上层检查eGodBidType和dwIdx的合法性
	VOID EachBidEnd(EGodBidType eGodBidType, DWORD dwIdx = GT_INVALID);

	void GiveMoneyBack2Player( BidInfoMap* pBidInfoMap, EGodBidType eGodBidType );
	tagBidInfoNode* GetBidInfoNode(EGodBidType eGodBidType, DWORD dwIdx = GT_INVALID);

	VOID LogGodBid(DWORD dwRoleID, DWORD dwAccountID, DWORD dwItemID, BYTE byMoneyType, DWORD dwMoney);

private:
	DWORD m_bBidTime;																			// 当前是否处于竞标时间
	BidStartTimeList m_FixedBidStartTimeList;											// 每周固定竞标开始时间list
	BidStartTimeList m_WeekendAddedBidStartTimeList;							// 周末加开的竞标开始时间list
	tagBidInfoNode m_NormalBidInfoList[MAX_GODBID_ITEM_NUM];	// 普通竞标竞价信息列表
	tagBidInfoNode m_GoldBlackMarketBidInfo;										// 金币黑市竞标竞价信息列表
	tagBidInfoNode m_ShengBiBlackMarketBidInfo;									// 圣币黑市竞标竞价信息列表
	BOOL m_bBidStartUpdated;																	// 竞标开始更新标记
	BOOL m_bBidEndUpdated;																	// 竞标结束更新标记
	BOOL m_bSwitchOn;																				// 开关
	BOOL m_bBidEnding;																				// 数据库正在清除投标记录
};

extern GodBidMgr g_GodBidMgr;
