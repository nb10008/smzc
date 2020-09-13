/********************************************************************
	created:	2012/05/07
	created:	7:5:2012   17:17
	filename: 	e:\code\World\internet_cafes_session.h
	file ext:	h
	author:		zhangzhihua
	
	purpose:	游戏服务器与韩国网吧Time服务器对话相关消息
*********************************************************************/
#pragma once

#pragma pack(push, 1)

CONST INT SERVECE_TYPE = 79253;
CONST INT GAME_SSN = 355;

CONST INT INTERNET_CAFE_PING_INTERVAL = 5 * 60;

CONST DWORD INTERNET_CAFE_BUFF_LIST[] =
{
	9700101,
	9700201,
	9700301,
	9700401,
};

enum EIC_RET_CODE
{
	EICRC_SUCCESS = 0,
	EICRC_FAILED,
	EICRC_LACK_TIME,
	EICRC_INVALID_IP,
	EICRC_PROTOCOL_ERR,
	EICRC_INVALID_USER,
};

enum EIC_MSG_TYPE
{
	EICMT_Start			= 0,
	EICMT_Logout		= 1,
	EICMT_Ping			= 2,
	EICMT_LeftTime		= 20,
};

//---------------------------------------------------------------------------------
// 世界服务器发往dummyTs的消息
//---------------------------------------------------------------------------------
struct tagW2T_Package
{
	BYTE			byStart;		// 表示信息开头的 1byte 文字. 固定为 0x0e
	BYTE			byDummy;		// Dummy 1byte
	WORD			wSequence;		// 分出划分的 2byte
	WORD			wLength;		// Body length
	BYTE			byData[1];		// 根据Header的 body部分长度 2byte -> byte ordering 需要转换
};

//---------------------------------------------------------------------------------
// dummyTs发往世界服务器的消息
//---------------------------------------------------------------------------------
struct tagT2W_Package
{
	BYTE			byStart;		// 表示信息开头的 1byte 文字. 固定为 0x0e
	BYTE			byDummy;		// Dummy 1byte
	WORD			wSequence;		// 分出划分的 2byte
	WORD			wLength;		// Body length
	BYTE			byData[1];		// 根据Header的 body部分长度 2byte -> byte ordering 需要转换
};


#pragma pack(pop)
