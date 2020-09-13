//-----------------------------------------------------------------------------
//!\file msg_shop.h
//!
//!\date 2008-09-27
//!
//!\brief 客户端与服务器之间关于商店的消息
//-----------------------------------------------------------------------------
#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// 错误码枚举
//----------------------------------------------------------------------------
enum 
{
	E_Shop_NPCNotValid				= 1,	// NPC非法
	E_Shop_NotValid					= 2,	// 商店非法
	E_Shop_ItemNotFind				= 3,	// 没有找到指定商品
	E_Shop_ItemCannotSell			= 4,	// 指定商品不可出售
	E_Shop_ItemNotEnough			= 5,	// 商品个数不足
	E_Shop_CreateItem_Failed		= 6,	// 创建出售商品时失败(1.内存申请失败;2.没有找到物品装备原型)
	E_Shop_Equip_Sold				= 7,	// 指定装备已经被出售
	E_Shop_CreateEquip_Failed		= 8,	// 出售装备时,创建新装备失败(1.内存申请失败)
	E_Shop_NotEnough_SpaceInBag		= 9,	// 背包中没有足够存储空间
	E_Shop_RepLevel_Low				= 10,	// 声望阶段不够
	E_Shop_NotEnough_SilverInBag	= 11,	// 背包中金钱不足
	E_Shop_NotEnough_SpecCost		= 12,	// 特殊消耗不足
	E_Shop_ItemMaxHold				= 13,	// 物品达到可拥有上限
	E_Shop_ItemNotActived			= 14,	// 氏族宝库中物品没有激活
	E_Shop_Tax_CityNotHaveHolder	= 15,	// 税收商店 但对应城市无通知者
	E_Shop_IsClosed					= 16,   // 商店还没有营业
	E_Shop_WillBeFull_SilverInBag	= 17,	// 背包中金钱不足
	E_Shop_Cannot_Buy				= 18,	// 您不能买卖
	E_Shop_WuXun_Low				= 19,	// 武勋等级不够
	E_Shop_NotEnough_WuJi			= 20,	// 武技值不足
	E_Shop_NotEnough_GuildContribution = 21,	// 公会贡献不足
	E_Shop_NotEnough_GuildExploit = 22,	// 公会功勋不足
	E_Shop_NotEnough_GodSoul = 23,	// 神魂声望不足
	E_Shop_NotEnough_EvilSoul = 24,	// 魔魂声望不足
	E_Shop_Holy_CountLimit = 25, // 圣灵数量超过最大限制
	E_Shop_NotEnough_WarMoney = 26,	//战场金币不足
};


//-----------------------------------------------------------------------------
// 得到商店中商品列表
//-----------------------------------------------------------------------------
CMD_START(NC_GetShopItems)
	DWORD 			dwNPCID; 		// NPC ID
	BYTE			byShelf;		// 货架号
	DWORD			dwNPCTypeID;
CMD_END

CMD_START(NS_GetShopItems)
	DWORD 			dwNPCID; 		// NPC ID
	DWORD			dwShopID;		// shop ID
	BYTE			byShelf;		// 货架号
	BYTE			byDummy;		// 对齐用
	INT16			n16RareItemNum;	// 稀有物品个数
	DWORD			dwNPCTypeID;
	BYTE			byData[1];		// tagMsgShopItem
CMD_END

CMD_START(NC_GetShopEquips)
	DWORD 			dwNPCID; 		// NPC ID
	BYTE			byShelf;		// 货架号
	DWORD			dwNPCTypeID;
CMD_END

CMD_START(NS_GetShopEquips)
	DWORD 			dwNPCID; 		// NPC ID
	DWORD			dwShopID;		// shop ID
	BYTE			byShelf;		// 货架号
	BYTE			byDummy;		// 对齐用
	INT16			n16EquipNum;	// 所有装备个数
	DWORD			dwNPCTypeID;
	BYTE			byData[1];		// tagMsgShopEquip
CMD_END

//-----------------------------------------------------------------------------
// 玩家从商店买商品
//-----------------------------------------------------------------------------
CMD_START(NC_BuyShopItem)
	DWORD 			dwNPCID; 		// NPC ID
	BYTE			byShelf;		// 货架号
	BYTE			byDummy;		// 对齐用
	INT16			n16ItemNum;		// 物品个数
	DWORD 			dwTypeID; 		// 玩家要购买的物品Type ID
	DWORD			dwNPCTypeID;
CMD_END

CMD_START(NS_BuyShopItem)
	DWORD 			dwNPCID; 		// NPC ID
	BYTE			byShelf;		// 货架号
	BYTE			byIndex;		// 对应位置
	INT16			n16RemainNum;	// 剩余个数(该值为GT_INVALID时，表示该商品不限量)
	DWORD 			dwTypeID;		// 玩家要购买的物品Type ID
	DWORD			dwNPCTypeID;
CMD_END

CMD_START(NC_BuyShopEquip)
	DWORD 			dwNPCID; 		// NPC ID
	INT64			n64Serial;		// 要购买装备的64位ID
	DWORD			dwTypeID;		// 将购装备的Type ID
	BYTE			byShelf;		// 货架号
	DWORD			dwNPCTypeID;	
CMD_END

CMD_START(NS_BuyShopEquip)
	DWORD 			dwNPCID; 		// NPC ID
	BYTE			byShelf;		// 货架号
	BYTE			byIndex;		// 对应位置
	INT16			n16RemainNum;	// 剩余个数(该值为GT_INVALID时，表示该商品不限量)
	DWORD			dwNPCTypeID;
	BYTE			byData[1];		// n16RemainNum != 0 && dwErrorCode == E_Success时, 生成新的待售装备 -- tagEquip
CMD_END

//-----------------------------------------------------------------------------
//	玩家向商店卖物品
//-----------------------------------------------------------------------------
CMD_START(NC_SellToShop)
	DWORD 			dwNPCID; 		// NPC ID
	INT64			n64Serial; 		// 玩家要卖的64位物品ID
	DWORD			dwNPCTypeID;
CMD_END

//-----------------------------------------------------------------------------
// 商店相关消息处理失败后,反馈给客户端的消息结构
//-----------------------------------------------------------------------------
CMD_START(NS_FeedbackFromShop)
	DWORD			dwErrorCode;
	DWORD 			dwNPCID; 		// NPC ID
CMD_END

#pragma pack(pop) 
