//-----------------------------------------------------------------------------
// filename: athletics_define.h
// author: Lamoair
// actor:
// data: 2010-2-25
// last:
// brief: 竞技系统定义
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

//房间状态
enum E_PvP_RoomState
{
	E_PvP_RoomStateNull = 0,
	E_PvP_Waiting,
	E_PvP_Fighting,
	E_PvP_FullPerson,   //人数已经满   
	E_PvP_CountDown,   //倒计时状态
	E_PvP_Delete,
};

//房间类型
enum E_PvP_RoomType
{
	E_PvP_RoomTypeNull = 0,
    E_PvP_Room3v3 = 1,
	E_PvP_Room6v6 = 2,
};

//房间等级
enum E_PvP_RoomLevel
{
	E_PvP_RoomLevelNull = 0,
	E_PvP_Junior,	//初级(新手竞技)     
	E_PvP_Middle,	//中级(普通竞技 )
	E_PvP_Senior,	//高级
};

enum E_PvP_RoleState
{
	E_PvP_RoleStateNull = 0,
	E_PvP_Preparing,       //准备  
	E_PvP_Ready,           //就绪
	//E_PvP_Start,
};

enum E_Role_BothSide
{
	ERBS_NULL           =  0,
	ERBS_DefenseSide    =  1,  //房间号奇数
	ERBS_Attack         =  2,  //房间号偶数
	ERBS_End,
};

enum E_Role_InPvP
{
	ERIP_NULL           = 0,
	ERIP_InPvP          = 1,
	ERIP_OffPvP         = 2,
};


//-----------------------------------------------------------------------------
// 房间角色信息
//-----------------------------------------------------------------------------


struct tagPvPRoleInfo
{
	DWORD			dwRoleID;
	E_PvP_RoleState eState;
	BYTE			ucPos;
	E_Role_BothSide eRoleInSide;         
	E_Role_InPvP    eRoleInPvP;  
	TCHAR			szName[X_SHORT_NAME];//角色名字
	DWORD			dwRoleLevel;        //角色等级
	DWORD			dwWuXun;            //武勋 
	EClassType	    ePlayerProfession;	//远程玩家职业
};


//-----------------------------------------------------------------------------
// 房间列表基本结构
//-----------------------------------------------------------------------------
struct tagPvPRoom
{
	E_PvP_RoomType				eRoomType;
	E_PvP_RoomLevel				eLevel;
	DWORD                       dwSerialID;             //房间序列号
    INT16	                    nRoomID ;	             //房间号
	E_PvP_RoomState             eRoomState;			    //房间状态
    BYTE                        ucTotalPersonNum;		//房间总人数
	INT			                iLevelUpperLimit;		//房间等级上限
	INT							iLevelLowerLimit;		//房间等级下限 
	DWORD						dwWuXunLimit;			//武勋限制
	DWORD						dwManagerID;				//房主
	TCHAR						szManagerName[X_SHORT_NAME];
	BYTE						ucCurrentPersonNum;		//当前人数
	tagPvPRoleInfo				pData[1];
};

//-----------------------------------------------------------------------------
// 房间简易信息结构
//-----------------------------------------------------------------------------
struct tagPvPSimRoom
{
	E_PvP_RoomType				eRoomType;
	E_PvP_RoomLevel				eLevel;
	DWORD                       dwSerialID;             //房间序列号
	INT16	                    nRoomID ;	             //房间号
	E_PvP_RoomState             eRoomState;			    //房间状态
	BYTE                        ucTotalPersonNum;		//房间总人数
	INT			                iLevelUpperLimit;		//房间等级上限
	INT							iLevelLowerLimit;		//房间等级下限 
	DWORD						dwWuXunLimit;			//武勋限制
	DWORD						dwManagerID;				//房主
	TCHAR						szManagerName[X_SHORT_NAME];
	BYTE						ucCurrentPersonNum;		//当前人数
};

//-----------------------------------------------------------------------------
//枚举 房间每格位置              以下 与客户端不相关不用改动！！！！！！
//-----------------------------------------------------------------------------
const INT    P3v3ROOM_TOTAL_PERSON_NUM       =  6;        //3v3的房间人数
const INT    P6v6ROOM_TOTAL_PERSON_NUM       =  12;       //6v6的房间人数
const INT    PVPROOM_ROLE_LOWERLEVEL         =  30;       //参与竞技的角色最低级别 

enum E_PvP_Room_Pos
{
	ERP_RoomPos_1        =    0,
	ERP_RoomPos_2        =    1,
	ERP_RoomPos_3        =    2,
	ERP_RoomPos_4        =    3,
	ERP_RoomPos_5        =    4,
	ERP_RoomPos_6        =    5,
};

//-----------------------------------------------------------------------------
// 枚举房间每格位置的状态
//-----------------------------------------------------------------------------
enum E_Room_Pos_State
{
	ERPS_Empty           =  0,      //没被人占
	ERPS_Solid           =  1,      //已被占
};

//-----------------------------------------------------------------------------
// 枚举房间ID的状态
//-----------------------------------------------------------------------------
enum E_RoomId_State
{
	ERIDS_NotCreated =    0, // 没有被创建过  
	ERIDS_Vacant     =    1, // 空闲状态
	ERIDS_Created    =   2, // 被创建
};



//-----------------------------------------------------------------------------
// 房间每格位置的状态
//-----------------------------------------------------------------------------
struct tagPvPRoomPosState
{
	BYTE               ucPos;        //房间位置
	E_Room_Pos_State   eRoomState;   //房间位置状态
};


//-----------------------------------------------------------------------------
// 房间ID状态
//-----------------------------------------------------------------------------
struct tagAthRoomId
{
	INT16                           nRoomID ;
	E_PvP_RoomLevel               eRoomLevel;
	E_RoomId_State                eRoomIDState;
};

#pragma pack(pop)
