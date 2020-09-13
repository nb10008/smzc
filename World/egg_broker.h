// 砸金蛋功能

#pragma once

const INT16 COLOUR_EGG_POS[] = {
	60,
	50, 51,
	40, 41, 42,
	30, 31, 32, 33,
	20, 21, 22, 23, 24,
	10, 11, 12, 13, 14, 15,
};

// 金蛋信息存储数据库逻辑，如果正常结束，数据库中就不会有记录存在。
class EggBroker
{
public:
	EggBroker():m_nCurLayer(0),m_n16ColourEggPos1(GT_INVALID),m_n16ColourEggPos2(GT_INVALID)
	{}
	~EggBroker(){}

	VOID InitEggInfoFromDB(const BYTE* &pData, INT nNum);
	DWORD SendEggInfo2Client(Role* pRole);
	DWORD BreakEgg(Role* pRole, int nLayer, int nPos);
	DWORD TakeEggTreasure(Role* pRole, int nLayer);
	DWORD FinishEggGame(DWORD dwRoleID);

private:
	VOID InitEggInfoFromDB(INT nCurLayer, INT16 n16ColourEggPos1, INT16 n16ColourEggPos2, tagEggInfo* pEggInfo);
	VOID InitColourEggPos();
	VOID GetColourEggPosByLayer(INT nLayer, INT& nColourEggPos1, INT& nColourEggPos2);
	VOID SaveEggInfo2DB(DWORD dwRoleID);
	VOID ClearEggInfoFromDB(DWORD dwRoleID);
	BOOL IsColourEggPosValid(INT16 n16ColourEggPos);
	BOOL IsColourEgg(INT16 nLayer, INT16 nPos);
	VOID SendBroadcast(INT nLayer, DWORD dwRoleID, DWORD dwItemID);
	DWORD GetBroadcastIDByLayer(INT nLayer);

private:
	INT m_nCurLayer;			// 当前可以砸的层
	INT16 m_n16ColourEggPos1;       // 彩蛋位置1
	INT16 m_n16ColourEggPos2;       // 彩蛋位置2
	tagEggInfo m_BrokenEggInfo[MAX_EGG_LAYER];     // 已经砸过的金蛋信息
};
