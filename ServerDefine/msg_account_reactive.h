#pragma once
#pragma pack(push, 1)

enum EAccountReactiveIdentity
{
	EARI_Null = 0,
	EARI_Intro = 1,
	EARI_BeIntro = 2,
};

struct tagAccountReactiveInfo
{
	EAccountReactiveIdentity eIdentity; // 1: 介绍人 2:被回捞人 0:无参与资格的人
	DWORD dwKey;	// 是介绍人的话，代表他的回捞码
	INT nScore; // 是介绍人的话，代表他的回捞积分
	BYTE byRewardLevel; // 领取到了几级奖励

	tagAccountReactiveInfo() { ZeroMemory(this, sizeof(this));}
};

// 合服工具合并account_reactiver
struct tagAccountReactiverInfo
{
	DWORD dwKeyCode; 
	BYTE byLevel;
	DWORD dwExp;

	tagAccountReactiverInfo() { ZeroMemory(this, sizeof(this));}
};

CMD_START(NDBC_AccountReactiveCode)
	DWORD dwAccountID;
	DWORD dwKeyCode;
	BYTE byLevel;
	DWORD dwExp;
CMD_END

CMD_START(NDBS_AccountReactiveCode)
	DWORD dwErrorCode;
	INT nScore;
	DWORD dwAccountID; // 归来者
	DWORD dwCAccountID; // 介绍人
CMD_END

CMD_START(NDBC_InitAccountReactiveInfo)
	DWORD dwAccountID;
CMD_END

CMD_START(NDBS_InitAccountReactiveInfo)
	DWORD dwAccountID;
	DWORD dwRoleID; 
	tagAccountReactiveInfo info;
CMD_END

CMD_START(NDBC_UpdateRewardReactiveLevel)
	DWORD dwAccountID;
	BYTE byLevel;
CMD_END

#pragma pack(pop)