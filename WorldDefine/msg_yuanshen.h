//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_soul.h
// author: 
// actor:
// data: 2011-06-9
// last:
// brief: 客户端和服务器端间消息 -- 元神
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"
#include "ItemDefine.h"
#include "container_define.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// 元神装备相关消息(注意结构中字段顺序最好保证内存对齐)
//----------------------------------------------------------------------------
enum EHolySoulErrorCode
{

	EHolySoul_NotActivated               =  1,           //元神为被激活
	EHolySoul_NextLevelNoExist           =  2,           //下一等级的元神不存在
	EHolySoul_EquipHolySoulFailed		 =  3,           //穿上元神装备失败
	EHolySoul_UnEquipHolySoulFailed      =  4,			 //脱下元神装备失败
	EHolySoul_BagItemNotFound            =  5,           //背包没有元神装备 
	EHolySoul_NotEquipment               =  6,           //不是元神装备
	EHolySoul_InitFailed                 =  7,           //元神初始化失败
	EHolySoul_NoExist                    =  8,           //该元神不存在
	EHolySoul_PourExpFailed				 =  9,			 //灌注失败
	EHolySoul_NotHolySoulEquip			 =  10,			 //非元神装备不能穿
	EHolySoul_EquipPosNotExist			 =  11,			 //装备位置不匹配不能穿
	EHolySoul_EquipNotExist				 =  12,			 //元神装备不存在
	EHolySoul_BagNoEnoughSpace			 =  13,			 //背包没有足够空间
	EHolySoul_ConsidatePosNotExist       =  14,          //元神装备加成元神属性的类型不正确
	EHolySoul_AwakePourExpFaied			 =  15,			 //元神觉醒状态灌注经验失败
	EHolySoul_LevelUpMaxLvl				 =  16,			 //升到最高等级
	EHolySoul_RebormMaxLvl				 =  17,			 //转生达到最大等级
};

//元神操作类型
enum ESoulAttOptType
{
	ESAOType_Call		=  1,  //召唤
	ESAOType_CancelCall =  2,  //取消召唤
};

//----------------------------------------------------------------------------
// 激活元神
//----------------------------------------------------------------------------
CMD_START(NS_ActivateHolySoul)	
CMD_END

//----------------------------------------------------------------------------
// 元神属性发生变化
//----------------------------------------------------------------------------
CMD_START(NS_HolySoulAttChange)	
EHolyEquipAttType eHolyEquipAttType;
DWORD		      dwVal;
CMD_END

//----------------------------------------------------------------------------
// 元神远程属性发生变化
//----------------------------------------------------------------------------
CMD_START(NS_RemoteHolySoulAttChange)
ESoulAttOptType		eOptType; 
DWORD				dwSoulID;		//元神ID
DWORD				dwRoleID;		//角色ID
CMD_END

//----------------------------------------------------------------------------
// 换上元神装备
//----------------------------------------------------------------------------
CMD_START(NC_SoulEquip)	// 装备
EHolyEquipPos		ePosDst;		// 装备部位
INT64					n64Serial;		// 装备64位序列号
CMD_END

CMD_START(NS_SoulEquip) // 反回装备结果
DWORD	dwErrorCode;
INT64		n64Serial;		// 装备64位序列号
CMD_END

//----------------------------------------------------------------------------
// 脱元神装备
//----------------------------------------------------------------------------
CMD_START(NC_UnSoulEquip)
INT16		n16PosDst;	// 待放入背包中位置
INT64		n64Serial;		// 装备64位序列号
CMD_END

CMD_START(NS_UnSoulEquip)
DWORD	dwErrorCode;
INT64		n64Serial;		// 装备64位序列号
CMD_END

//----------------------------------------------------------------------------
// 初始化元神信息
//----------------------------------------------------------------------------
CMD_START(NS_GetSoulInfo)
tagHolySoulInfo	 HolySoulInfo;
CMD_END

//----------------------------------------------------------------------------
//  元神灌注
//----------------------------------------------------------------------------
CMD_START(NC_GetHolyAffuse)
INT64 n64AffuseExp;		//灌注经验
CMD_END

CMD_START(NS_GetHolyAffuse)
DWORD dwErrCode;
CMD_END

//----------------------------------------------------------------------------
//  元神升级
//----------------------------------------------------------------------------
CMD_START(NS_HolySoulLvUp)
CMD_END


//----------------------------------------------------------------------------
//  元神转生
//----------------------------------------------------------------------------
CMD_START(NC_GetHolyReborn)
INT64 n64ItemID;			//转生道具 幻元玉魄
INT64 n64StuffID;			//转生道具 老君仙露
INT	 nNumStuff;
CMD_END

CMD_START(NS_GetHolyReborn)
DWORD dwErrCode;
CMD_END

//----------------------------------------------------------------------------
//  元神装备冲星
//----------------------------------------------------------------------------
CMD_START(NC_StarEnhance)
INT64 n64EquipId;			
CMD_END

CMD_START(NS_StarEnhance)
DWORD dwErrCode;
CMD_END

//更新元神经验
CMD_START(NS_UpdateHolySoulExp)
INT64	n64Exp;
CMD_END

//元神装备凿孔
CMD_START(NC_HolyEquipHole)
INT64				n64SrcEquipID;	//装备64ID
INT64				n64ItemID;		//穿云箭64ID
CMD_END

CMD_START(NS_HolyEquipHole)
DWORD				dwErrorCode;	//返回码
CMD_END

//元神装备镶嵌
CMD_START(NC_HolyEquipInlay)
INT64				n64SrcEquipID;	//装备64ID
INT64				n64StoneID;		//宝石64ID
INT64				n64ItemID;		//老君仙露64ID
INT					nItemNum;		//放入老君仙露的个数
BYTE				byHoleID;		//选择的孔位
CMD_END

CMD_START(NS_HolyEquipInlay)
DWORD				dwErrorCode;	//返回码
CMD_END

//元神装备摘除
CMD_START(NC_HolyEquipRemove)
INT64				n64SrcEquipID;	//装备64ID
BYTE				byHoleID;		//选择的孔位
CMD_END

CMD_START(NS_HolyEquipRemove)
DWORD				dwErrorCode;	//返回码
CMD_END

#pragma pack(pop)
