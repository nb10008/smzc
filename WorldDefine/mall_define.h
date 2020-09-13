//--------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: mall_define.h
// author: 
// actor:
// data: 2009-1-5
// last:
// brief: 商城相关定义
//--------------------------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)
//--------------------------------------------------------------------------------------------
// 常量
//--------------------------------------------------------------------------------------------
const INT	MALL_PACK_ITEM_NUM		= 5;	// 物品包中最多物品种类数
const INT	Max_PackName_Length		= 10;	// 礼包名称最多10个字
const INT	Max_LeaveWord_Length		= 20;	//留言20个字
const INT	MAX_PromoteItem			= 5;	//推荐商品个数
const static INT   MAX_PromoteItemNum     = 10;   //显示推荐商品图标最大数目
const static INT   MAX_PromoteItemPage	   = 2;    //显示推荐商品图标最大页码数
const INT64 MAX_YB_NUM				= 9999999;		// 元宝交易账户最大元宝数
const INT64 MAX_SILVER_NUM			= 9999999999;	// 元宝交易账户最大银币数
#define MALL_ITEM_PERSONAL_NUM_LIMIT_LINE	10000000 // 用于角色在购买商城物品个数限制相关的
const INT	MAX_MALL_CART_NUM		= 6;	// 商城购物车商品最大个数 [6/5/2012 zhangzhihua]

enum EMallCfgAttr
{
	EMCA_BuyActivityOpen			= 0,	//商城消费活动开关
	//EMCA_BuyActivityBeginTime,				//活动开始时间
	//EMCA_BuyActivityEndTime,				//活动结束时间
	EMCA_BuyActivityRewardLine,				//活动奖励最低消耗圣币
	EMCA_BuyActivityRewardItem0,			//活动奖励道具1
	EMCA_BuyActivityRewardItem1,
	EMCA_BuyActivityRewardItem2,
	EMCA_BuyActivityRewardItem3,
	//EMCA_BuyActivityRewardItem4,
	EMCA_BuyActivityRewardItemNum0,			//活动奖励道具1数量
	EMCA_BuyActivityRewardItemNum1,
	EMCA_BuyActivityRewardItemNum2,
	EMCA_BuyActivityRewardItemNum3,
	//EMCA_BuyActivityRewardItemNum4,

	EMCA_End,
};

//--------------------------------------------------------------------------------------------
// 静态结构
//--------------------------------------------------------------------------------------------
struct tagMallItemProtoBase
{
	DWORD			dwID;							// ID
	INT				nPrice;							// 定价
	INT				nSalePrice;						// 促销价
	DWORD			dwTimeSaleEnd;					// 促销截止时间
	DWORD			dwPresentID;					// 赠品ID
	INT				nNum;							// 待售数量
	INT				nPresentNum;					// 赠品数量
	INT				nExAssign;						// 赠送赠点数量
	INT				nExNum;							// 需要赠点数量
	BYTE			bySmallGroupHeadcount;			// 小型团购人数
	BYTE			bySmallGroupDiscount;			// 小型团购折扣[0，100]
	BYTE			byMediumGroupHeadcount;			// 中型团购人数
	BYTE			byMediumGroupDiscount;			// 中型团购折扣
	BYTE			byBigGroupHeadcount;			// 大型团购人数
	BYTE			byBigGroupDiscount;				// 大型团购折扣
	bool			bNew	: 1;					// 是否为新品
	bool			bHot	: 1;					// 是否热销
	INT				nIndexInServer;					// 在服务器中的位置
	DWORD			dwPersistTime;					// 团购持续时间
};


struct tagMallItemProto: public tagMallItemProtoBase
{
	DWORD			dwTypeID;						// 物品TypeID
	INT8			n8Kind;							// 所属分类

	INT8            n8Kind2;                        // 二级分类 
	BYTE			byRank;							// 热销排名
	BYTE			byGroupPurchaseAmount;			// 团购数量
	BYTE			byDummy[1];
};

struct tagMallPromoteItemShow
{
	INT   iPage;
	INT	  iNum;
	DWORD  dwProItemID;
};

struct tagMallItemPrototeGood
{
	INT         id;                          //编号
	INT			iMinLevel;						// 最小等级
	INT        iMaxLevel;						// 最大等级
	DWORD	   dwItemID[MAX_PromoteItemNum];    // 显示的商品
	tagMallItemPrototeGood()
	{
		for(INT i=0; i<MAX_PromoteItemNum; ++i)
		{
			dwItemID[i] = GT_INVALID;
		}
	}
};

struct tagMallPackProto: public tagMallItemProtoBase
{
	DWORD			dwTypeID[MALL_PACK_ITEM_NUM];	// 物品ID
	INT				nItemPrice[MALL_PACK_ITEM_NUM];	// 各个物品价格(总价)
	INT			byItemNum[MALL_PACK_ITEM_NUM];	// 物品个数
	INT8			n8ItemKind;						// 物品种类(ID)个数
	BYTE			byDummy[2];
	TCHAR			tszPackName[Max_PackName_Length];// 礼包名称
	tagMallPackProto()
	{
		ZeroMemory(this, sizeof(*this));
		for(INT i=0; i<MALL_PACK_ITEM_NUM; ++i)
		{
			dwTypeID[i] = GT_INVALID;
		}
	}
};

struct tagMallFreeItem
{
	DWORD		dwTypeID;	// 物品ID
	INT			nUnitPrice;	// 原单价
	BYTE		byNum;		// 领取个数

	tagMallFreeItem()
	{
		Clear();
	}

	VOID Clear()
	{
		dwTypeID	= GT_INVALID;
		nUnitPrice	= 0;
		byNum		= 0;
	}
};

//--------------------------------------------------------------------------------------------
// 推荐物品数据结构　
//--------------------------------------------------------------------------------------------
struct  tagRecommendingItemInfo
{
    DWORD    dwID ;
	INT16    n16MinLevel;
	INT16    n16MaxLevel;

	DWORD    dwItem1TypeID;
	DWORD    dwItem2TypeID;
	DWORD    dwItem3TypeID;
	DWORD    dwItem4TypeID;
	DWORD    dwItem5TypeID;

	DWORD    dwItem6TypeID;
	DWORD    dwItem7TypeID;
	DWORD    dwItem8TypeID;
	DWORD    dwItem9TypeID;
	DWORD    dwItem10TypeID;	

	tagRecommendingItemInfo()
	{
		memset(this,0,sizeof(*this));
	}
};
//--------------------------------------------------------------------------------------------
// 服务器数据结构
//--------------------------------------------------------------------------------------------
//struct tagMallGoods
//{
//	DWORD	dwID;		// 商品或礼品包ID
//	BYTE	byCurNum;	// 当前待售数量(GT_INVALID表示不限量)
//	BYTE	byDummy[3];
//};

struct tagMallGoods
{
	union
	{
		const tagMallItemProto	*pMallItem;
		const tagMallPackProto	*pMallPack;
		const tagMallFreeItem	*pMallFreeItem;
	};

	INT	nCurNum;	// 当前待售数量(GT_INVALID表示不限量)
	//BYTE	byDummy[3];
};

//--------------------------------------------------------------------------------------------
// 客户端与服务器商品个数同步结构
//--------------------------------------------------------------------------------------------
struct tagMallUpdate
{
	DWORD		dwID;					//商品ID
	INT 		byRemainNum;			//剩余个数
};

//--------------------------------------------------------------------------------------------
// 帮派团购
//--------------------------------------------------------------------------------------------
enum EGPSCOPE
{
	EGPS_NULL		= -1,
	EGPS_SMALL,							//小型团购
	EGPS_MEDIUM,						//中型团购
	EGPS_BIG,							//大型团购
};

struct tagGroupPurchase
{
	DWORD			dwGuildID;			//帮派ID
	DWORD			dwRoleID;			//发起者ID
	DWORD			dwMallID;			//商品ID
	INT32			nPrice;				//团购价格
	INT16			nParticipatorNum;	//响应人数
	INT16			nRequireNum;		//团购所需人数
	DWORD			dwRemainTime;		//剩余时间
	TList<DWORD>*	listParticipators;	//响应者列表

	tagGroupPurchase()
	{
		ZeroMemory(this, sizeof(*this));
	}

	~tagGroupPurchase()
	{
		SAFE_DEL(listParticipators);
	}
};

enum EYBOTYPE							//元宝订单类型
{
	EYBOT_BUY,							//收购
	EYBOT_SELL,							//寄售
};

enum EYBOMODE							//元宝交易状态
{
	EYBOM_Submit,						//委托中
	EYBOM_Cancel,						//撤销
	EYBOM_Close,						//关闭
};

//--------------------------------------------------------------------------------------------
// 元宝交易订单
//--------------------------------------------------------------------------------------------
struct tagYuanBaoOrder
{
	DWORD				dwID;					//订单ID
	DWORD				dwRoleID;				//玩家ID
	EYBOTYPE			eYBOType;				//订单类型
	INT					nPrice;					//单价
	INT					nNum;					//数量
	EYBOMODE			eYBOMode;				//元宝交易状态
	tagDWORDTime		dwStartTime;			//开始时间
	tagDWORDTime		dwEndTime;				//结束时间
	INT					nAvgPrice;				//成交均价
	INT					nDealNum;				//成交数量
	tagYuanBaoOrder()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
		dwStartTime = GT_INVALID;
		dwEndTime = GT_INVALID;
	}

	INT		GetRemainNum() { return nNum - nDealNum; }	
};

//--------------------------------------------------------------------------------------------
// 创建元宝交易订单(插入数据库时使用）
//--------------------------------------------------------------------------------------------
struct tagYBOrder
{
	DWORD				dwID;
	DWORD				dwRoleID;
	INT16				nType;
	INT					nPrice;
	INT					nNum;
	INT16				nYBMode;
	DWORD				dwStartTime;
	tagYBOrder()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
		dwStartTime = GT_INVALID;
	}
};

// 元宝交易价格列表
struct tagYBPriceList
{
	INT					nPrice;
	INT					nNum;
};
 
// 元宝交易账户
struct tagYBAccount
{
	DWORD				dwRoleID;				//玩家ID
	INT					nYuanBao;				//元宝数量
	INT					nGold;					//金币数量
	INT					nSilver;				//银币数量
	BOOL				bSellOrder;				//是否有出售订单
	BOOL				bBuyOrder;				//是否有收购订单
};

// 撤销或关闭元宝交易订单
struct tagCompleteOrder
{
	DWORD				dwID;
	INT16				nYBOMode;
	DWORD				dwEndTime;
	tagCompleteOrder()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
		dwEndTime = GT_INVALID;
	}
};

// 更新元宝交易订单
struct tagRefresh
{	
	DWORD				dwID;
	INT					nAvgPrice;
	INT					nDealNum;
	tagRefresh()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
	}
};

// 挂载在界面上的商品信息
struct tagUIItem
{
	DWORD dwTypeID;
	DWORD dwUIType;
	tagUIItem()
	{
		ZeroMemory(this, sizeof(*this));
		dwTypeID = GT_INVALID;
		dwUIType = GT_INVALID;
	}
};

// 给客户端发送的一元计划商品原型
struct tagMallOfferProto2Client
{
	DWORD dwTypeID;
	UINT16 nLevel;
	tagMallOfferProto2Client()
	{
		ZeroMemory(this, sizeof(*this));
		dwTypeID = GT_INVALID;
		nLevel = GT_INVALID;
	}
};

// 1元计划
const INT MAX_GIFT_PER_PAGE = 4; //每页显示
const INT LEVEL_NUM_CHOOSE	= 4; //可供选择的等级
const INT PLAN_GIFT_TYPE_NUM = 10;//1元计划对应一级分类号数
// 一元计划商品原型
struct tagMallOfferProto
{
	DWORD dwID;
	DWORD dwPrice;
	tagMallOfferProto2Client MallOfferProto2Client;

	tagMallOfferProto()
	{
		ZeroMemory(this, sizeof(*this));
		dwID = GT_INVALID;
		MallOfferProto2Client.dwTypeID = GT_INVALID;
		MallOfferProto2Client.nLevel = GT_INVALID;
		dwPrice = GT_INVALID;
	}
};

struct tagBuyMallItemCountSaveData
{
	//DWORD dwRoleID;
	DWORD dwTypeID;
	INT nNum;
};

//--------------------------------------------------------------------------------------------
#pragma pack(pop)