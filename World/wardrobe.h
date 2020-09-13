//-------------------------------------------------------------------------------------
// 衣橱系统
//-------------------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/fashion_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../ServerDefine/msg_fashion.h"

enum EWardrobeLayerType
{
	EWLT_Null = 0,
	EWLT_Normal = 1, // 需放入指定时装
	EWLT_Diy = 2,		   // 自由衣橱栏

	EWLT_MAX
};

enum EWardrobeLayerSex
{
	EWLS_Null = 0,
	EWLS_Men = 1,
	EWLS_Women = 2,

	EWLS_MAX
};

#define IsWardrobeLayerTypeValid(type) (((type) > EWLT_Null) && ((type) < EWLT_MAX))
#define IsWardrobeLayerSexValid(type,sex) ((((sex) > EWLS_Null) && ((sex) < EWLS_MAX)) || (((type)==EWLT_Diy)&&((sex)==EWLS_Null)))

const INT32 MAX_FASHION_PERLAYER = EWFP_End;		// 每个栏位时装件数
const INT32 MAX_LEVEL_PERLAYER = 3;		// 每个栏位最高等级
#define PLAYACT_CDTIME 3600
#define PLAYACT_REQUEST_TIMEOUT 30
#define CalLayerID(Idx) (DWORD)((Idx/MAX_FASHION_PERLAYER)+1)

struct tagWardrobeProto
{
	DWORD dwLayerID;			// 衣橱栏位ID
	EWardrobeLayerType eLayerType;
	EWardrobeLayerSex eLayerSex;
	DWORD dwFashionID[MAX_FASHION_PERLAYER];
	DWORD dwBuffIDByLvl[MAX_LEVEL_PERLAYER];
	DWORD dwOtherBuffIDByLvl[MAX_LEVEL_PERLAYER];
};

struct tagWardrobeLayer
{
	DWORD dwLayerID;
	BYTE byLevel;						// 衣橱栏等级
	EFashionState eState;			// 衣橱栏状态
	tagDWORDTime dwPlayActStartTime;		// 装扮开始时间
	DWORD dwFriendID;									// 装扮的好友ID
	BYTE byFashionTemperament;		// 时装气质值
	tagWardrobeLayer(DWORD dwLayer)
		:byLevel(1),eState(EFS_None),dwPlayActStartTime(0),dwFriendID(GT_INVALID),dwLayerID(dwLayer),byFashionTemperament(0)
	{}
	tagWardrobeLayer(BYTE byLvl, EFashionState e, tagDWORDTime dwStartTime, DWORD dwLayer, BYTE byFT, DWORD dwFID)
	{
		byLevel = byLvl;
		eState = e;
		dwPlayActStartTime = dwStartTime;
		dwLayerID = dwLayer;
		byFashionTemperament = byFT;
		dwFriendID = dwFID;
	}
};

class Wardrobe
{
public:
	typedef TMap<DWORD, tagWardrobeLayer*> WardrobeInfoMap;
	typedef TMap<DWORD, DWORD> PlayActMap;
public:
	Wardrobe(DWORD dwRoleID);
	~Wardrobe();

	DWORD PlayActMyself(DWORD dwLayerID);

	// 改变衣橱栏位状态
	VOID ChangeLayerState(DWORD dwLayer, EFashionState eFashionState);

	// 升级衣橱
	DWORD LevelUpWardrobeLayer(Role* pRole, DWORD dwLayerID);

	// 开始装扮好友
	DWORD StartPlayActFriend(DWORD dwLayerID, DWORD dwFriendID, INT nPlayActFriendLimit);

	// 结束装扮好友
	DWORD EndPlayActFriend(DWORD dwFriendID, BYTE& byLevel, DWORD& dwLayerID, BOOL bAgree);

	// 更新衣橱状态
	VOID Update();

	// 当前时装脱装和穿装操作
	DWORD PutOffCurFashion();
	DWORD PutOnCurFashion(Role* pRole, BOOL bInitPutOnFashion = FALSE);
	DWORD PutOnBePlayActFashion(Role* pRole, BOOL bInitPutOnFashion = FALSE);

	// 换装操作
	DWORD PutOnFashion(Role* pRole, DWORD dwWardrobeLayerID, BYTE byLevel, BOOL bSelf = TRUE, BOOL bSendMsg = TRUE, BOOL bInitPutOnFashion = FALSE);

	BOOL CanAddFashion(INT16 n16TargetIndex, tagItem* pItem);
	VOID SetFashionTemperament(DWORD dwLayerID, BYTE byFashionTemperament);
	BYTE GetCurFashionTemperament();
	VOID SyncWardrobeInfo(Role* pRole);
	VOID InitWardrobe(const BYTE* &pData, DWORD dwLayerCnt, Role* pRole);
	VOID CancelBePlayActing(Role* pRole);

	DWORD GetCurWareLayer() { return m_dwCurWareLayer; }
	tagWardrobeLayer* GetWardrobeLayer(DWORD dwLayerID) { return m_mapWardrobeInfo.Peek(dwLayerID); }
	BOOL IsPlayActing() { return (m_dwCurWareLayer != GT_INVALID); }
	BOOL IsBePlayActing() { return m_dwBePlayActLayerID != GT_INVALID; }
	VOID SetBePlayActLayerID(DWORD dwBePlayActLayerID) { m_dwBePlayActLayerID = dwBePlayActLayerID; }
	VOID SetBePlayActLevel(BYTE byBePlayActLevel) { m_byBePlayActLevel = byBePlayActLevel; }
	DWORD GetBePlayActLayerID() { return m_dwBePlayActLayerID; }
	BYTE GetBePlayActLevel() { return m_byBePlayActLevel; }

	// 结婚换装
	VOID MaggriageMakeUp(DWORD dwRoleID, DWORD dwLayerID);

	VOID GetFashionTemperamentSum(INT& mQZSum, INT& nSuit1Sum, INT& nSuit2Sum, INT& nSuit3Sum, INT& nFashionNum);
private:
	VOID InitWardrobe(Role* pRole, DWORD dwLayerCnt, tagWardrobeLayer2DB* pWardrobeLayer);
	VOID UpdatePlayActCDTime();
	VOID UpdatePlayActRequest();
	VOID UpdateBePlayAct();
	VOID SyncLayerInfo(DWORD dwLayerID, BYTE byLevel, EFashionState eFashionState);
	VOID FixWardrobeLayerState(tagWardrobeLayer* pNewWardrobeLayer, Role* pRole);
	VOID SaveWardrobeLayerInfo2DB(DWORD dwLayerID);
private:
	DWORD m_dwRoleID;
	WardrobeInfoMap m_mapWardrobeInfo;
	DWORD m_dwCurWareLayer;				// 当前穿着栏位
	PlayActMap m_mapPlayAct;				// 当前给好友装扮的列表
	
	// 处理装扮好友请求
	DWORD m_dwPlayActingFriend;		// 在装扮好友的ID
	DWORD m_dwPlayActingLayerID;		// 在装扮好友的LayerID
	tagDWORDTime m_dwPlayActRequestStartTime;

	// 处理被好友装扮
	DWORD m_dwBePlayActLayerID;			// 被好友装扮
	BYTE m_byBePlayActLevel;				// 被好友装扮
};

