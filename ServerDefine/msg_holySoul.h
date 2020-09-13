//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_holySoul.h
// author: zxzhang
// actor:
// data: 2010-06-13
// last:
// brief:  元神系统数据保存数据库
//-----------------------------------------------------------------------------

#pragma once

#include "../WorldDefine/ItemDefine.h"
#pragma pack(push, 1)


//------------------------------------------------------------------------------
//元神属性
//------------------------------------------------------------------------------
struct tagHolySoulInfoEx
{
	INT64	         n64CurLevelExp;	   //元神当前经验
	DWORD            dwSoulID;             //元神ID
	INT		         nCurSoulValue;		   //当前元神值
	INT		         nCurCovalue;	       //当前默契值	
	EHolySoulState   eHolySoulState;       //元神状态
    tagSoulAtt       HolySoulAtt;          //元神属性
    tagHolySoulInfoEx()
	{
		memset(this,0,sizeof(*this));
	}
};

//----------------------------------------------------------------------------
// 元神属性类型
//----------------------------------------------------------------------------
enum ESoulAttTypeEx
{
	ESoulAttEx_Null	   = 0,		//空属性
	ESoulAttEx_SoulVal = 1,		//元神值
	ESoulAttEx_Covalue = 2,		//元神默契值
	ESoulAttEx_End,
};

//------------------------------------------------------------------------------
//插入元神信息
//------------------------------------------------------------------------------
CMD_START(NDBC_InsertHolySoulInfo)
DWORD				dwRoleID;
tagHolySoulInfoEx   SoulAtt;
CMD_END


//------------------------------------------------------------------------------
//更新元神信息
//------------------------------------------------------------------------------
CMD_START(NDBC_UpdateHolySoulInfo)
DWORD				dwRoleID;
tagHolySoulInfoEx   SoulAtt;
CMD_END

//------------------------------------------------------------------------------
//更新经验属性
//------------------------------------------------------------------------------
CMD_START(NDBC_UpdateHolySoulExpAtt)
DWORD				dwRoleID;
INT64				n64Exp;				//经验
CMD_END


//------------------------------------------------------------------------------
//更新元神属性
//------------------------------------------------------------------------------
CMD_START(NDBC_UpdateHolySoulAttInfo)
ESoulAttTypeEx      eSoulAttTypeEx;			//元神属性类型
DWORD				dwRoleID;
DWORD 				dwAttVal;				//元神属性值
CMD_END

#pragma pack(pop)