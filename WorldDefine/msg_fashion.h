// 时装系统丰富（2010.12.29）
#pragma once


#pragma pack(push, 1)
//--------------------------------------------------------------
// 消息定义
//--------------------------------------------------------------

enum eClothespressValue
{
	E_WardrobeLevelUp_Fail = 1, // 升级衣橱失败
	E_WardrobeLevelUp_NoItem1, // 没有紫檀木
	E_WardrobeLevelUp_NoItem2, // 没有神木
};
// 换装
CMD_START(NC_Rehanding)
	DWORD dwWardrobeLayerID;
	bool bTakeOffFashion; // 是否是脱装
CMD_END
CMD_START(NS_Rehanding)
	DWORD dwErrorCode;
CMD_END

// 升级衣橱栏位
CMD_START(NC_WardrobeLevelUp)
	DWORD dwWardrobeLayerID;
CMD_END
CMD_START(NS_WardrobeLevelUp)
	DWORD dwErrorCode;
CMD_END

// 装扮好友（客户端通知服务器，要给另外一个客户端装扮）
CMD_START(NC_DressUpAnotherRole)
	DWORD dwTargetRoleID;
	DWORD dwWardrobeLayerID;
CMD_END

// 装扮好友（服务器通知客户端，目标客户端是否同意装扮）
CMD_START(NS_DressUpAnotherRole)
	DWORD 	dwErrorCode;
	DWORD 	dwSrcRoleID;
	DWORD 	dwWardrobeLayerID;
	bool			bAgree;
CMD_END

// 装扮好友（服务器通知客户端，询问客户端是否同意装扮）
CMD_START(NS_IsDressUpAgree)
	DWORD 	dwSrcRoleID;
	DWORD 	dwWardrobeLayerID;
CMD_END

// 装扮好友（客户端通知服务器，是否同意装扮）
CMD_START(NC_IsDressUpAgree)
	DWORD dwTargetRoleID;
	bool			bAgree;
CMD_END

// 同步衣橱时装信息
CMD_START(NS_SyncFashionInfo)
	INT nSuitNum;				// 时装套数
	BYTE FashionInfo[1];		// 具体时装信息
CMD_END

#pragma pack(pop)