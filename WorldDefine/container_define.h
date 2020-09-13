//--------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: container_define.h
// author: 
// actor:
// data: 2008-8-4
// last:
// brief: 人物装备栏、行囊、仓库等相关定义
//--------------------------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
// 常量定义
//-----------------------------------------------------------------------------
const INT32 MAX_BAG_NUM				= 5;							// 背包最大页数
const INT32 SPACE_ONE_BAG			= 40;							// 单个背包格数
const INT32 SPACE_ALL_BAG			= SPACE_ONE_BAG * MAX_BAG_NUM;	// 最大背包格数

const INT32 SPACE_QUEST_BAG			= 40;							// 任务物品栏最大格数	
const INT32 SPACE_BAIBAO_BAG		= 20;							// 百宝袋最大格数
const INT32 SPACE_WAR_BAG			= 9;							// 专属背包最大各数 [4/25/2012 zhangzhihua]

const INT32 MAX_BAIBAO_LOG_NUM		= 40;							// 百宝袋最大Log数

const INT32 MAX_WARE_NUM			= 6;							// 仓库最大页数
const INT32 SPACE_ONE_WARE			= 40;							// 单个仓库格数
const INT32 SPACE_ALL_WARE			= SPACE_ONE_WARE * MAX_WARE_NUM;// 最大仓库格数

const INT32 MIN_PASSWORD_NUM		= 6;							// 密码最小长度
const INT32 MAX_PASSWORD_NUM		= 14;							// 密码最大长度

const INT32 SPACE_WARE_PER_EXTEND	= 8;							// 角色仓库一次扩充格子数
const INT32 SPACE_BAG_PER_EXTEND	= 8;							// 背包一次扩充格子数

const INT32 MAX_WARE_EX_TIMES_SILVER= 4;							// 使用游戏币最多能扩充的仓库次数
const INT32 MAX_BAG_EX_TIMES_SILVER	= 4;							// 使用游戏币最多能扩充的背包次数

const INT32 MAX_GUILDWARE_NUM       =   5;                          // 帮派仓库最大页数
const INT32 SPACE_ONE_GUILDWARE     =   35;                         // 单个帮派仓库格数
const INT32 SPACE_ALL_GUILDWARE     =   SPACE_ONE_GUILDWARE * MAX_GUILDWARE_NUM;

//元神相关
//const INT32 HOLY_EQUIP_BLANK = 10;
//圣灵装备容器最大格子数
const INT32 SHENGLING_EQUIP_BLANK = 60;

//-----------------------------------------------------------------------------
// 枚举定义
//-----------------------------------------------------------------------------
// 容器类型
enum EItemConType
{
	EICT_Start		= 0,
	EICT_Null		= 0,

	EICT_Bag		= 1, // 背包
	EICT_Quest		= 2, // 任务物品栏
	EICT_Baibao		= 3, // 百宝袋
	EICT_RoleWare	= 4, // 角色仓库
	EICT_Equip		= 5, // 装备栏

	EICT_Shop		= 6, // 商店
	EICT_Ground		= 7, // 掉落

    EICT_PetEquip   = 8, // 宠物装备栏

	EICT_GuildWare	= 9, // 帮派仓库

	EICT_Wardrobe = 10, // 衣橱

	EICT_Family = 11, // 家族妖精

	EICT_TempBag	= 12,// 临时背包，存放妖精融合产生的新妖精 [3/29/2012 zhangzhihua]

	EICT_HolyEquip	= 13, //元神装备栏

	EICT_HolyManEquip	= 14, //圣灵装备栏



	EICT_WarBag		= 15,// 专属背包 [4/25/2012 zhangzhihua]

	EICT_End		= 16,
};

// 装备栏
enum EEquipPos	// 装备部位
{
	//EEP_Null			= -1,
	//EEP_Start			= -1, 

	//EEP_Equip_Start		= 0,
	//EEP_Head			= 0, // 头部
	//EEP_Neck			= 1, // 项链
	//EEP_Body			= 2, // 上身
	//EEP_Legs			= 3, // 下身
	//EEP_Back			= 4, // 背部
	//EEP_Wrist			= 5, // 腕部
	//EEP_Feet			= 6, // 足部

	//EEP_Finger1			= 7, // 戒指
	//EEP_Finger2			= 8, // 戒指
	//EEP_Waist			= 9, // 腰饰

	//EEP_RightHand		= 10, // 主手武器
	//EEP_LeftHand		= 11, // 副手武器
	//EEP_Face			= 12, // 法宝
	//EEP_Equip_End		= 12,

	//EEP_MaxEquip		= 13, // 装备栏栏位数

	//EEP_FashionHead		= 13, // 头部
	//EEP_FashionBody		= 14, // 上身
	//EEP_FashionBack		= 15, // 背部
	//EEP_FashionWrist	= 16, // 手部
	//EEP_FashionFeet		= 17, // 足部

	////EEP_MaxEquip		= 18, //F计划改变
	//EEP_End				= 18,
	EEP_Null			= -1,
	EEP_Start			= -1, 

	EEP_Equip_Start		= 0,
	EEP_Head			= 0, // 头部
	EEP_Face			= 1, // 法宝
	EEP_Body			= 2, // 上身
	EEP_Legs			= 3, // 下身
	EEP_Back			= 4, // 背部
	EEP_Wrist			= 5, // 腕部
	EEP_Feet			= 6, // 足部

	EEP_Finger1			= 7, // 戒指
	EEP_Finger2			= 8, // 戒指
	EEP_Waist			= 9, // 腰饰
	EEP_Neck			= 10, // 项链

	EEP_RightHand		= 11, // 主手武器
	EEP_LeftHand		= 12, // 副手武器
	EEP_HunJing			= 13, // 魂晶
	EEP_Holy			= 14,	//圣灵
	EEP_Equip_End		= 14,

	EEP_MaxEquip		= 15, // 装备栏栏位数

	EEP_FashionHead		= 15, // 头部
	EEP_FashionFace		= 16, // 面部（没有了）
	EEP_FashionBody		= 17, // 上身
	EEP_FashionLegs		= 18, // 下身（没有了）
	EEP_FashionBack		= 19, // 背部
	EEP_FashionWrist	= 20, // 腕部
	EEP_FashionFeet		= 21, // 足部


	EEP_End				= 22,


};

enum EHolyEquipPos		//元神装备位置索引
{	
	EHEFI_Start   			= -1,			//序号	typeEx	
	EHEFI_TianChong     	= 0,			//天冲	0			45
	EHEFI_LingHui			= 1,			//灵慧	1			46
	EHEFI_QiPo				= 2,			//气魄	2			47
	EHEFI_LiPo				= 3,			//力魄	3			48	
	EHEFI_ZhongShu	        = 4,			//中枢	4			49
	EHEFI_JingPo			= 5,			//精魄	5			50
	EHEFI_YingPo			= 6,			//英魄	6			51
	EHEFI_TianHun	    	= 7,			//天魂	7			42
	EHEFI_DiHun			    = 8,			//地魂	8			43
	EHEFI_MingHun	    	= 9,			//命魂	9			44

	EHEFI_End		    	= 10,		
};

enum EHolyEquipAttType	//元神装备伤害类型
{
	EHEAT_NULL				= 0,
	EHEAT_NULLEX			= 1,
	EHEAT_Demage			= 2,			//元神绝技伤害
	EHEAT_Hit				= 3,			//元神命中
	EHEAT_Crit				= 4,			//元神致命
	EHEAT_CritAmount		= 5,			//元神致命量
	EHEAT_Exattack			= 6,			//元神外功攻击
	EHEAT_InExattack		= 7,			//元神内功攻击
	EHEAT_Accurate			= 8,			//元神精准
	EHEAT_Exdamage			= 9,			//元神额外伤害
	EHEAT_Toughness			= 10,			//元神忽视韧性spp
	EHEAT_Morale			= 11,			//元神士气
	EHEAT_Covalue			= 12,			//元神默契值
	EHEAT_SoulValue			= 13,			//元神值
	EHEAT_MaxSoulValue		= 14,			//元神最大值

	EHEAT_End				= 15,
};


const INT32 X_EQUIP_BAR_SIZE = EEP_End - EEP_Start - 1;
const INT32 X_HolySoul_BAR_SIZE = EHEFI_End;
#define MIsInEquipPos(n16Pos)	(n16Pos > EEP_Start && n16Pos < EEP_End)

// 武器装备部位
enum EWeaponPos
{
	EWP_NULL			= 0,	// 无
	EWP_Back1           = 1,    // 背后  琴
	EWP_Back2           = 2,    // 背后  双持
	EWP_Back3           = 3,    // 背后  枪
	EWP_Back4           = 4,    // 背后  弓 法器
	EWP_Hand			= 5,	// 手部
	EWP_dun			= 6,	// 手部
};

const INT32 X_WEAPONPOS_NUM = EWP_dun + 1;

// 百宝袋记录类型
enum EBaiBaoRecordType
{
	EBBRT_System,			// 系统划拨
	EBBRT_Mall,				// 商城赠品
	EBBRT_Friend,			// 好友赠送
	EBBRT_Myself,			// 自己领取
	EBBRT_GroupPurchase,	// 帮派团购
	EBBRT_VipNetBarGift,	// 金牌网吧礼品
};

// 藏衣阁时装位置
enum EWardrobeFashionPos
{
	EWFP_Null = -1,

	EWFP_Head = 0,		// 头部
	EWFP_Body = 1,		// 衣服
	EWFP_Wrist = 2,		// 腕部
	EWFP_Feet = 3,		// 足部
	EWFP_Back = 4,		// 背部

	EWFP_End
};

//-----------------------------------------------------------------------------
// 结构
//-----------------------------------------------------------------------------
// 自动整理
struct tagItemOrder
{
	INT16	n16OldIndex;
	INT16	n16NewIndex;
};

// 百宝袋记录
struct tagBaiBaoRecord
{
	INT16	n16Size;		// 单条记录长度
	INT16	n16Type;		// EBaiBaoRecordType
	DWORD	dwTypeID;		// 物品的TypeID
	DWORD	dwRoleID;		// 操作玩家的ID，GT_INVALID为系统划拨
	DWORD	dwTime;			// 操作时间 tagDWORDTime
	TCHAR	szWords[1];		// 赠言
};

//-----------------------------------------------------------------------------
// 宏
//-----------------------------------------------------------------------------

// 仓库扩容次数=（当前仓库以开放栏位数-24）/6
#define MCalWareExTimes(n16CurWareSpace)	((n16CurWareSpace - 24) / 6)

// 若使用游戏币扩充，收费=（仓库扩容次数+1）^2×3000[单位游戏币]
#define MCalWareExSilver(nExTimes)	((nExTimes + 1) * (nExTimes + 1) * 3000)

// 若使用元宝扩充，则收费=（仓库扩容次数+1）×5[单位元宝]
#define MCalWareExYuanBao(nExTimes)	((nExTimes + 1) * 5)


// 行囊扩容次数=（当前行囊以开放栏位数-20）/5
#define MCalBagExTimes(n16CurBagSpace)	((n16CurBagSpace - 20) / 5)

// 若使用游戏币扩充，则收费=（行囊扩容次数+1）^2×8000[单位游戏币]
#define MCalBagExSilver(nExTimes)	((nExTimes + 1) * (nExTimes + 1) * 8000)

// 若使用元宝扩充，则收费=（行囊扩容次数+1）×10[单位元宝]
#define MCalBagExYuanBao(nExTimes)	((nExTimes + 1) * 10)

