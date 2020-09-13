//-----------------------------------------------------------------------------
// brief: 砸金蛋
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

struct tagEggInfo
{
	int nBrokePos;					// 此层砸中的位置
	DWORD dwItemID;			// 砸出的物品ID
	tagEggInfo():nBrokePos(GT_INVALID),dwItemID(0)
	{}
};

struct tagEggInfoInDB
{
	INT nCurLayer;			// 当前可以砸的层
	INT16 n16ColourEggPos1;       // 彩蛋位置1
	INT16 n16ColourEggPos2;       // 彩蛋位置2
	tagEggInfo sBrokenEggInfo[MAX_EGG_LAYER];     // 已经砸过的金蛋信息
};

// 消息定义
CMD_START(NDBC_SaveEggInfo)
	DWORD dwRoleID;
	INT nCurLayer;			// 当前可以砸的层
	INT16 n16ColourEggPos1;       // 彩蛋位置1
	INT16 n16ColourEggPos2;       // 彩蛋位置2
	tagEggInfo sBrokenEggInfo[MAX_EGG_LAYER];     // 已经砸过的金蛋信息
CMD_END

CMD_START(NDBC_ClearEggInfo)
	DWORD dwRoleID;
CMD_END


#pragma pack(pop)