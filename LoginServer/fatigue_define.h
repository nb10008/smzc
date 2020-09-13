#pragma once

#pragma pack(push,1)

//-------------------------------------------------------------------------
// login与中心激活服务器的防沉迷消息
//-------------------------------------------------------------------------
CMD_START2(NC_LOGIN_CM)
	DWORD	dwSectionID;		// 大区编号
CMD_END2

CMD_START2(NC_ANTIQUERYSTATE)
	CHAR	szAccountName[16];	// 帐号名
CMD_END2

CMD_START2(NC_ANTIENTHRALL)
	DWORD	dwAccountNum;		// 帐号数目
	CHAR	szAccountName[1];	// 帐号名
CMD_END2

CMD_START2(NS_ANTIENTHRALL)
	CHAR	szAccountName[16];	// 帐号名
	BYTE	byColor;			// 状态
	DWORD	dwAccOLSeconds;		// 累计在线时间秒
CMD_END2

#pragma pack(pop)