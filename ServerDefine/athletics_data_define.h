//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_data_define.h 
// author: Lamoair
// actor:
// data: 2010-03-24
// last:
// brief:  竞技场数据保存数据库
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

struct tagPvPSystemInfo
{
	INT16     n16SystemId;
	DWORD     dwSerialVal;      //序列id
};

struct tagPvPSystemData
{
	DWORD          dwSerialId; //保存当前的id值
	tagDWORDTime   saveTime;
	DWORD          dwHallPersonNum;
	DWORD          dwHallRoomNum;
	DWORD          dwFightRoomNum;
	INT16          n16Below120Sec;
	INT16          n16Below210Sec;
	INT16          n16Below300Sec;
	INT16          n16Over300Sec;
};

//-------------------------------------------------------------------------
// 保存竞技场的数据到数据库
//-------------------------------------------------------------------------
CMD_START(NDBC_SaveArenaDataToDB)	// 
   tagPvPSystemInfo   tPvPSystemInfo;
   tagPvPSystemData   tPvPSystemData;
CMD_END;


//-------------------------------------------------------------------------
//服务器启动 要向数据库获取序号值
//-------------------------------------------------------------------------
CMD_START(NDBC_GetSerialVal)	
    DWORD   dwSerialId;
CMD_END;

//-------------------------------------------------------------------------
//数据库向服务器发送序号值
//-------------------------------------------------------------------------
CMD_START(NDBS_GetSerialVal)   //获取序号值 
DWORD   dwSerialId;
DWORD   dwSerialVal;
CMD_END;

//-------------------------------------------------------------------------
// 保存序号值
//-------------------------------------------------------------------------
CMD_START(NDBC_SaveSerialValToDB)	
DWORD   dwSerialId;
DWORD   dwSerialVal;
CMD_END;


#pragma pack(pop)

