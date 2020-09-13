// 衣橱相关DBServer消息
#pragma once

#pragma pack(push, 1)

struct tagWardrobeLayer2DB
{
	DWORD dwRoleID;
	DWORD dwLayerID;
	BYTE byLevel;						// 衣橱栏等级
	BYTE byState;						// 衣橱栏状态
	tagDWORDTime dwPlayActStartTime;		// 装扮开始时间
	DWORD dwFriendID;									// 装扮的好友ID
};

// 保存一条记录
CMD_START(NDBC_SaveWardrobeLayerInfo)
	tagWardrobeLayer2DB WardrobeLayerInfo;
CMD_END

#pragma pack(pop)