//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: msg_item.h
// author: 
// actor:
// data: 2008-09-04
// last:
// brief: 物品相关
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push)
#pragma pack(1)
//-----------------------------------------------------------------------------


#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/ItemDefine.h"
#include "item_define.h"

//-----------------------------------------------------------------------------
// 枚举
//-----------------------------------------------------------------------------
enum
{
	
};

//-----------------------------------------------------------------------------
// 消息结构
//-----------------------------------------------------------------------------
CMD_START(NDBC_GetItemInfo)	// 获得游戏世界中全局物品相关信息
CMD_END;

CMD_START(NDBS_GetItemInfo)
	DWORD	dwDummy;
	INT64	n64MaxSerial;
	INT64	n64MinSerial;
CMD_END;

CMD_START(NDBC_NewItem)
	tagItem		item;
CMD_END;

CMD_START(NDBC_NewEquip)
	tagEquip	equip;
CMD_END;

CMD_START(NDBC_DelItem)
	INT64		n64Serial;
CMD_END;

// 注：只能删除一件装备
CMD_START(NDBC_DelEquipFromRole)
	DWORD		dwRoleID;
	DWORD		dwTypeID;
CMD_END;

CMD_START(NDBC_DelEquip)
	INT64		n64Serial;
CMD_END;

CMD_START(NDBC_NewBaiBaoItem)
	tagItem		item;
CMD_END;

CMD_START(NDBC_NewBaiBaoEquip)
	tagEquip	equip;
CMD_END;

CMD_START(NDBC_DelBaiBaoItem)
	INT64		n64Serial;
CMD_END;

CMD_START(NDBC_DelBaiBaoEquip)
	INT64		n64Serial;
CMD_END;

//CMD_START(NDBC_UpdateItemPos)	// 更新物品位置
//	DWORD		dwAccountID;
//	INT64		n64Serial;
//	DWORD		dwRoleID;
//	INT32		nUseTimes;		// 物品上技能使用次数
//	INT16		n16Num;			// 物品个数
//	INT16		n16Index;		// 容器中位置
//	BYTE		byConType;		// 容器类型
//CMD_END;

CMD_START(NDBC_UpdateEquipAtt)	// 更新装备属性，即装备属性表
	tagEquipSpecUpdate	equipSpecUpdate;
CMD_END;

CMD_START(NDBC_UpdateEquipSingleAtt)	// 更新装备单个属性属性，即装备属性表
tagEquipSingleSpec	equipSingleSpecUpdate;
CMD_END;

//CMD_START(NDBC_ReplaceItemCDTime)	// 存储物品冷却时间
//	DWORD			dwRoleID;
//	INT32			nNum;			// 个数
//	BYTE			byCDTime[1];	// tagCDTime
//CMD_END;

CMD_START(NDBC_EquipSignature)		// 保存装备签名
	INT64			n64Serial;		// 被署名装备的64位id
	DWORD			dwSignRoleID;	// 署名者的id
	TCHAR			szContent[10];	// 署名内容
CMD_END;

//-----------------------------------------------------------------------
// 物品锁定道具
//-----------------------------------------------------------------------
CMD_START(NDBC_LockItem)
	INT64			n64Item;		// 要锁定物品的64位ID
	BYTE			byType;			// 0：未锁定状态，1：锁定状态
	DWORD			dwUnlockTime;	// 解锁时间
CMD_END

//-----------------------------------------------------------------------
// 物品署名道具导致的绑定
//-----------------------------------------------------------------------
CMD_START(NDBC_BindItem)
	INT64			n64Item;		// 要绑定物品的64位ID
	BYTE			byType;			// 0：未绑定状态，1：绑定状态
CMD_END

//-----------------------------------------------------------------------------
// 压测专用
//-----------------------------------------------------------------------------
CMD_START(NDBC_TestSaveItem)
	tagItem		item;
CMD_END;

CMD_START(NDBC_TestSaveEquip)
	tagEquip	equip; 
CMD_END;

// Jason 2010-4-14 v2.0.0 法宝相关修改
CMD_START(NDBC_NewFabao)
	tagFabao	fabao;
CMD_END;
CMD_START(NDBC_UpdateFabaoAtt)	// 更新法宝属性
	INT64			n64FabaoID;		// 要更新法宝的64位ID
	tagFabaoSpec	fabaoSpec;
CMD_END;

CMD_START(NDBC_UpdateHolyAtt)	// 更新圣灵属性
INT64			n64HolyID;		// 要更新圣灵的64位ID
tagHolySpec		holySpec;
CMD_END;

CMD_START(NDBC_UpdateHolyEquipAtt)	// 更新圣纹属性
INT64			n64ID;				// 要更新圣纹的64位ID
tagHolyEquipSpec		holyEquipSpec;
CMD_END;

// 妖精心情变化
CMD_START(NDBC_FabaoMoodChanged)
	INT64	n64SerialID;
	INT		nMood;
	INT		nMoodTicks;
CMD_END

// 圣灵默契值变化
CMD_START(NDBC_HolyCoValue)
INT64	n64SerialID;
INT		nCoValue;
CMD_END

CMD_START( NDBC_UpdateFabaoEleInjuryType )
	INT64	n64FabaoID;
	EElementInjury eInjury;
CMD_END

// [wcy 2010-03-31] 圣灵相关修改
CMD_START(NDBC_NewHoly)
tagHolyMan	holy;
CMD_END;

// [wcy 2010-04-18] 圣纹相关修改
CMD_START(NDBC_NewHolyEquip)
tagHolyEquip	holyEquip;
CMD_END;

// 妖精重生用数据
struct tagFabaoNirVanaData
{
	INT	nStage;		// 等级
	INT	nEleInjuryType;	// 元素伤害类型
	INT	nEleInjury;		// 元素伤害
	INT	nElePierce;		// 元素穿透
	WORD		wEleInjPromoteTimes;		// 伤害提升次数 [3/12/2012 zhangzhihua]	
	WORD		wEleResPromoteTimes;		// 抗性提升次数 [3/12/2012 zhangzhihua]
	INT			nEleResistance[EER_End];	// 元素抗性，以元素抗性枚举做索引访问
	WORD		wDamageUpgradeTimes;				// 伤害可提升次数
	WORD		wResistanceUpgradeTimes;			// 抗性可提升次数
	//DWORD		dwFairySkill[MaxFairySkillNum];		// 已有特技，默认GT_INVALID	
	//DWORD		dwUnsureSkill;						// 待确认特技，默认GT_INVALID
};
// 记录妖精等级到60级以上的特技属性等，每10级记录一次
CMD_START( NDBC_UpdateFabaoNirVanaData )
	INT64					_n64ID;		// 妖精serialnum
	DWORD			dwAccountID;
	DWORD			dwRoleID;
	tagFabaoNirVanaData _data;
CMD_END

// 得到妖精重生数据
CMD_START( NDBC_GetFabaoNirVanaData )
	INT64			n64ID;
	DWORD			dwAccountID;
	DWORD			dwRoleID;
	INT				nCurStage;		// 当前等级
	BOOL			bFirstOrLast;	// 第一个是60级；否则，就是取最近一个（就是上一个）
	INT64		n64NirvanID;	// 重生物品
CMD_END
CMD_START( NDBS_GetFabaoNirVanaData )
	DWORD			dwErrorCode;
	INT64			n64ID;
	DWORD			dwAccountID;
	DWORD			dwRoleID;
	tagFabaoNirVanaData data;
	INT64		n64NirvanID;	// 重生物品
CMD_END

// 首次补偿妖精配对次数
CMD_START( NDBC_UpdateFabaoGivenBirthTimes )
	INT64					_n64ID;		// 妖精serialnum
	INT						nTimes;
CMD_END

// 获取稀有物品信息
CMD_START( NDBC_LoadGlobalLimitedItem )
CMD_END

CMD_START( NDBS_LoadGlobalLimitedItem )
	INT32 nNum;
	tagGlobalLimitedItem data[1]; 
CMD_END

// 保存稀有物品信息
CMD_START( NDBC_SaveGlobalLimitedItem )
	tagGlobalLimitedItem data;
CMD_END

CMD_START( NDBC_UnSaveGlobalLimitedItem )
	DWORD dwItemID;
	INT64 n64Serial;
CMD_END

//CMD_START( NDBC_CreateGlobalLimitedItem )
//	tagGlobalLimitedItem item;
//CMD_END
CMD_START( NDBC_UpdateGlobalLimitedItem )
	tagGlobalLimitedItem item;
	INT64	nOriginal64ID;
CMD_END

//CMD_START( NDBC_DeleteGlobalLimitedItem )
//	tagGlobalLimitedItem item;
//CMD_END

//CMD_START( NDBC_DelGlobalLimitedItem )
//	DWORD dwItemID;
//	INT64 n64Serial;
//CMD_END

CMD_START(NDBC_NewSoulCrystal)
	tagSoulCrystal	obj;
CMD_END;
CMD_START(NDBC_UpdateSoulCrystal)	// 更新法宝属性
	INT64			n64ID;			// 要更新法宝的64位ID
	tagSoulCrystalSpec	objspec;
CMD_END;

CMD_START(NDBC_KeyCodeAddGift)
	DWORD dwRoleID;
CMD_END

CMD_START(NDBS_KeyCodeAddGift)
	DWORD dwRoleID;
	BOOL bIsSuccessful;
CMD_END

CMD_START(NDBC_ReceiveYuanBao)
	DWORD dwRoleID;
CMD_END

CMD_START(NDBS_ReceiveYuanBao)
	DWORD dwRoleID;
	int nNum;
CMD_END
//-----------------------------------------------------------------------------
#pragma pack(pop)
