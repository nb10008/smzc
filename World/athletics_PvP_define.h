
//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: athletics_PvP_define.h
// author: zxzhang
// actor:
// data: 2010-3-10
// last:
// brief: 竞技场定义
//-----------------------------------------------------------------------------

#pragma once

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// 进入竞技场三个入口地图
//-----------------------------------------------------------------------------
const DWORD  ATHLETICS_ENTER_SPOT1  =  3017299663 ;  //m04地图入口点
const DWORD  ATHLETICS_ENTER_SPOT2  =  2092184628 ;  //a04地图入口点
const DWORD  ATHLETICS_ENTER_SPOT3  =  2092184884 ;  //a05地图入口点
const DWORD  ATHLETICS_MAPID        =  2092185140 ;  //竞技场副本地图ID
const DWORD  ATHLETICS_BUFF_ID      =  2025501;      //竞技场被晕的技能 




//-----------------------------------------------------------------------------
// 枚举玩家的生命状态信息
//----------------------------------------------------------------------------
enum E_Role_Life_State
{
	ERLS_NULL        =  0,
	ERLS_RoleAlive   =  1,
	ERLS_RoleDead    =  2,
	ERLS_End,
};

//-----------------------------------------------------------------------------
// 竞技所在房间的基本信息
//-----------------------------------------------------------------------------
struct tagAthleticsPvPRoomInfo
{
	INT16 nRoomId;                //房间号
	BYTE  cnInPvPPersonNum;       //房间在线人数
    BYTE  cnTotalPersonNum;       //房间总人数
	DWORD dwRoomValiantMeritLevel;//房间武勋等级
	DWORD dwRoomType;             //房间类型
	DWORD dwManagerId;            //房主
    DWORD dwSerialId;             //房间序列号
	
};

//-----------------------------------------------------------------------------
// 竞技所在房间的角色的基本信息
//-----------------------------------------------------------------------------
struct tagPvPRoomRoleInfo
{
    BYTE               cnPos ;
	DWORD              dwRoleId;
	DWORD              dwRoleInSide;
    E_Role_Life_State  eRoleLifeState;
};





#pragma pack(pop)  //end athletics_PvP_define.h