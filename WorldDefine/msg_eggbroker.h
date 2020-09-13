//////////////////////////////////////////////////////////////////////////
//砸金蛋
//////////////////////////////////////////////////////////////////////////
#pragma once
#pragma pack(push, 1)

#define MAX_EGG_LAYER 7
#define MAX_EGG_PER_LAYER 7
#define MAX_COLOUR_EGG 2
#define IsEggPosValid(layer, pos) (((pos)>=0)&&(pos)<(MAX_EGG_PER_LAYER-(layer)))
#define IsEggLayerValid(layer) (((layer)>=0)&&((layer)<MAX_EGG_LAYER))

struct tagEggStartInfo
{
	int nLayer;				// 层数	（是-1表示没有信息）
	int nPos;					// 位置
	bool bIsColourEgg;	// 是不是彩蛋
	DWORD dwItemID;	// 物品ID
};

enum EEggBrokerErrorCode
{
	EEBEC_Success = E_Success,
	EEBEC_TopLayer,		// 已经砸到顶层
	EEBEC_EmptyEgg,	// 砸到空蛋
	EEBEC_HammerNotEnough,	// 锤子不足
	EEBEC_CannotBreak,		// 此蛋不可砸
	EEBEC_ChooseNoItem,	// 选中的层数没有宝物
	EEBEC_BaiBao,		// 发送到百宝袋中了
};

//-----------------------------------------------------------------------------
// 打开砸金蛋界面
//-----------------------------------------------------------------------------
CMD_START(NC_EggBegin)
CMD_END

CMD_START(NS_EggBegin)
	int	nCurrLayer;		// 当前是第几层(如果是第一层则后面信息不再加载进客户端)
	int	nColourEggPos_1;		//	彩蛋位置1（-1为没有）	当前层中彩蛋的位置
	int	nColourEggPos_2;		// 彩蛋位置2							当前层中彩蛋的位置
	tagEggStartInfo	EggStartInfo[MAX_EGG_LAYER];
CMD_END

//-----------------------------------------------------------------------------
// 砸蛋
//-----------------------------------------------------------------------------
CMD_START(NC_BreakEgg)
	int nLayer;
	int nPos;
CMD_END

CMD_START(NS_BreakEgg)
	DWORD dwErrorCode;
	int nLastEggLayer;			// 刚才砸的蛋是哪一行的
	int nLastEggPos;				// 刚才砸的蛋位置
	bool bLastEggIsColour;		// 刚才砸过的蛋是不是彩蛋（如果是填true）
	int nNextLayer;				// 现在应该砸的层数
	int nColourEggPos_1;		//	彩蛋位置1（-1为没有）
	int nColourEggPos_2;		// 彩蛋位置2
	DWORD dwItemID;			// 刚刚砸蛋获得的物品ID
CMD_END

//-----------------------------------------------------------------------------
// 取宝物
//-----------------------------------------------------------------------------
CMD_START(NC_TakeEggTreasure)
	int nlayer;		// 第几层的宝物
CMD_END

CMD_START(NS_TakeEggTreasure)
	DWORD	dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
// 取消所有逻辑，界面关闭
//-----------------------------------------------------------------------------
CMD_START(NC_FinishEggGame)
CMD_END

#pragma pack(pop)