#pragma once

#pragma pack(push, 1)

#define IsFashionStateValid(state) (((state)>EFS_NULL) && (state)<EFS_End)

enum EFashionErrorCode
{
	EFEC_Success = 0,
	EFEC_None = 1, // 一件时装也没有
	EFEC_Incomplete = 2, // 未集齐
	EFEC_UseBySelf = 3, // 自己穿戴中
	EFEC_UseForFriends = 4, // 好友装扮中
	EFEC_Sex_NotMatch = 5, // 时装性别不匹配
	EFEC_NoLevelUpItem = 6, // 缺少升级衣橱栏位所需道具
	EFEC_FriendNotOnLine = 7, // 好友不在线
	EFEC_IsPlayActing = 8, // 已经穿着了自己衣橱里面的时装
	EFEC_IsWaitingPlayActFriendReq = 9, // 正在等待好友的请求结果
	EFEC_PlayActCountLimit = 10, // 已到达可以装扮好友的最大数
	EFEC_DiyLayerPlayActLimit = 11, // 自由栏位不可以装扮好友

	EFEC_IsBePlayActing = 4, // 已经被装饰
};

// 衣橱栏状态
enum EFashionState
{
	EFS_NULL = -1,
	EFS_None = 0,					// 空，一件时装也没有
	EFS_Incomplete = 1,		// 没有集齐，有几件时装
	EFS_Complete = 2,			// 集齐，未使用
	EFS_UseBySelf = 3,			// 自己穿戴中
	EFS_UseForFriends = 4,	// 好友装扮中
	EFS_End,
};

#define MIsFashionSuitComplete(state) (state == EFS_Complete || state == EFS_UseBySelf || state == EFS_UseForFriends)

struct tagFashionInfo
{
	DWORD dwIndex;					// 衣橱栏索引
	BYTE byLevel;					// 衣橱栏等级
	EFashionState eState;			// 衣橱栏状态
};

#pragma pack(pop)