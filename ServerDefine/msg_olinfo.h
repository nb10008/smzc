/********************************************************************
	created:	2010/08/20
	created:	20:8:2010   11:14
	filename: 	e:\F-Project\src\ServerDefine\msg_olinfo.h
	file path:	e:\F-Project\src\ServerDefine
	file base:	msg_olinfo
	file ext:	h
	author:		Jason
	
	purpose:	world在线状态信息记录协议，用于world定时回报自己的一些状态信息，记录到db中
*********************************************************************/
#pragma  once

#pragma pack(push)
#pragma pack(1)

#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/base_define.h"

struct tagWorldMapOnlineInfo
{
	DWORD	dwMapID;
	BOOL		bIsInst;
	INT			nOnlineNum;
	INT			nOpenNum;
	// 时间戳，db自己打
};
// Jason 记录world在线信息
CMD_START( NDBC_WorldMapOlInfo  )
	INT			nNum;
	BYTE		data[1]; //tagWorldMapOnlineInfo*
CMD_END

#pragma pack(pop)

