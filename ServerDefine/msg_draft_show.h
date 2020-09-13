//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_draft_show.h
// author: Lamoair
// actor:
// data: 2010-03-31
// last:
// brief:  竞技场数据保存数据库
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

//------------------------------------------------------------------------------
// 报名选秀者列表
//------------------------------------------------------------------------------
struct tagShowPlayer            
{
    TCHAR    tcAthleteRoleName[X_SHORT_NAME];       //选手的名字
	DWORD    dwAthleteRoleId;                       
	DWORD    dwAthleteSignId;                      //选手的报名号
	DWORD    dwAthletePoint;
	DWORD    dwAthleteSerialId;                    //序列号
};


//------------------------------------------------------------------------------
// 投票者列表
//------------------------------------------------------------------------------
struct  tagVotePlayer
{
	TCHAR    tcVoteRoleName[X_SHORT_NAME];
	DWORD    dwVoteRoleId;
	DWORD    dwVotePoint;
	DWORD    dwSerialId;
};


//------------------------------------------------------------------------------
//选秀表保存
//------------------------------------------------------------------------------
CMD_START(NDBC_SaveShowPlayerData)
DWORD     dwCount;
tagShowPlayer   data[1];
CMD_END

//------------------------------------------------------------------------------
//选秀者的得票保存
//------------------------------------------------------------------------------
CMD_START(NDBC_SaveShowPlayerPoint)
tagShowPlayer showPlayer;
CMD_END

//------------------------------------------------------------------------------
//参与投票的得分保存
//------------------------------------------------------------------------------
CMD_START(NDBC_SaveVotePlayerPoint)
tagVotePlayer   VotePlayer;
CMD_END

//------------------------------------------------------------------------------
//选秀表数据条数
//------------------------------------------------------------------------------
CMD_START(NDBC_SaveShowPlayerDataCount)
INT16     n16SignId;
DWORD     dwTotalDataCount;
CMD_END

//------------------------------------------------------------------------------
//得票数据条数
//------------------------------------------------------------------------------
CMD_START(NDBC_SaveVotePlayerDataCount)
INT16     n16RoleId;
DWORD     dwTotalDataCount;
CMD_END

//------------------------------------------------------------------------------
//获得选秀者表
//------------------------------------------------------------------------------
CMD_START(NDBC_GetShowPlayerData)
CMD_END

//------------------------------------------------------------------------------
//获得参与投票表
//------------------------------------------------------------------------------
CMD_START(NDBC_GetVotePlayerData)
CMD_END


//------------------------------------------------------------------------------
//获得选秀者表
//------------------------------------------------------------------------------
CMD_START(NDBS_LoadShowPlayerData)
DWORD           dwCount;
tagShowPlayer   data[1];
CMD_END

//------------------------------------------------------------------------------
//获得参与投票表
//------------------------------------------------------------------------------
CMD_START(NDBS_LoadVotePlayerData)
DWORD           dwCount;
tagVotePlayer   data[1];
CMD_END


//------------------------------------------------------------------------------
//活动数据表的数据类型
//------------------------------------------------------------------------------
enum E_ActiveDataType
{
	E_DataTypeNull   = 0,
    E_DataType1      = 0,
	E_DataType2      = 1,
	E_DataType3      = 2,
	E_DataType4      = 3,
    E_DataType5      = 4,
	E_DataType6      = 5,
	E_DataType7      = 6,
	E_DataType8      = 7,
	E_DataTypeEnd,
};


struct tagActiveRankData
{
	DWORD  dwRoleId;
	TCHAR  ucRoleName[X_SHORT_NAME];
    INT16  n16RoleLevel;
	INT32  nDataType[E_DataTypeEnd];
	tagActiveRankData()
	{
		memset(this,0,sizeof(tagActiveRankData));
	}

};



CMD_START(NDBC_SaveActiveRankData)
tagActiveRankData   ActiveRankData;
CMD_END


CMD_START(NDBC_GetActiveRankData)
CMD_END

CMD_START(NDBS_LoadActiveRankData)
DWORD               dwCount;
tagActiveRankData   data[1];
CMD_END

#pragma pack(pop)