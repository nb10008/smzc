#pragma once
#pragma pack(push, 1)

#define m02ID		3017298127
#define m03ID		3017298383
#define m04ID		3017299663
#define m05ID		3017299919
#define m06ID		3017299151
#define m061ID		1146339967
#define m07ID		3017299407
#define m20ID		2983744207

#define PLAYER_TRACK_LIMIT_BUFF 10922

enum ESpecItemUseError
{
	E_ItemLock_HaveLocked = 1,		//物品已经被锁定
	E_ItemLock_NoLocked,			//物品尚未被锁定
	E_ItemLock_TargetError,			//目标不合法
	E_ItemLock_TargetInvalid,		//目标物品不是装备或宝石
	E_ItemLock_HaveNotItem,			//没有锁定或者解锁 道具
	E_ItemLock_OtherError,			//其它错误
	
	E_ItemTrack_HaveNotItem,		//你没有追踪道具
	E_ItemTrack_ItemUseTimeLimit,	//道具达到最大使用次数	
	E_ItemTrack_PlayerOffLine,		//目标玩家不在线
	E_ItemTrack_PlayerNotExists,	//玩家不存在
	E_ItemTrack_BuffLimit,			// 身上有禁止用此功能的buff
	//E_ItemTrack_PetsLimit,			//带着宠物的玩家不能传

	E_ItemTrack_DeadState,			//死亡状态不能传送
	E_ItemTrack_DizzyState,			//眩晕状态不能传送
	E_ItemTrack_TieState,			//定身状态不能传送
	E_ItemTrack_SporState,			//昏睡状态不能传送
	E_ItemTrack_PKState,			//PK状态不能传送
	E_ItemTrack_CommerceState,		//跑商状态不能传送
	E_ItemTrack_StallState,			//摆摊状态不能传送
	E_ItemTrack_PrisonAreaState,		//本地玩家牢狱里
	E_ItemTrack_TargetPrisonAreaState,	//目标玩家在牢狱里
	E_ItemTrack_Mount,				//骑乘状态下不能传送
	E_ItemTrack_XiulianChang,		//目标玩家在修炼场
	E_ItemTrack_Instance,			//目标玩家在副本
	E_ItemTrack_JinCity,			//目标玩家在禁城
	E_ItemTrack_ShenJi,				//目标玩家处于神迹区域

	E_EquipSig_HaveNoItem,			//道具非法
	E_EquipSig_NoTarget,			//要署名的对象不存在
	E_EquipSig_NotEquip,			//要署名的对象不是装备
	E_EquipSig_AlreadySig,			//要署名的装备已被署名
	E_EquipSig_ContentLimit,		//署名内容含违规文字

	E_ScriptItem_ItemInvalid,		// 该道具不合法
	E_ScriptItem_ScriptUnreg,		// 没有注册对用的脚本
	E_ScriptItem_UseLimit,			// 物品不可使用
	E_ScriptItem_DesRoleOffline,	// 目标角色不在线
	E_ScriptItem_DesRoleError,		// 目标角色不应是使用物品的角色

	E_PetLifeTime_ItemInvalid,		// 道具不合法
	E_PetLifeTime_SoulNotExist,		// 该宠物不存在
	E_PetLifeTime_NotTimeLimit,		// 不是时限制宠物
	E_PetLifeTime_Failed,			// 操作失败
};


//-----------------------------------------------------------------------
// 物品锁定道具
//-----------------------------------------------------------------------

CMD_START(NC_LockItem)
	INT64			n64Item;		// 道具的64位ID
	INT64			n64Target;		// 要绑定物品的64位ID
CMD_END

CMD_START(NS_LockItem)
	DWORD			dwErrorCode;	// 锁定结果
	BYTE			byType;			// 0：锁定操作的返回，1：解锁操作的返回
	DWORD			dwUnlockTime;	// 解锁操作时，返回解锁时间
	INT64			n64Serial;		// 目标物品ID
CMD_END

// Jason 2010-8-25
CMD_START( NS_UnlockItemNotice )
	UINT64		u64ItemID;
	BYTE			byContainerType;
CMD_END


//-----------------------------------------------------------------------
// 角色追踪道具
//-----------------------------------------------------------------------
CMD_START(NC_TrackPlayerPos)
	INT64			n64Item;						// 道具的64位ID
	TCHAR			szPlayerName[X_SHORT_NAME];		// 玩家的名字
CMD_END

CMD_START(NS_TrackPlayerPos)
	DWORD			dwErrorCode;					// 锁定结果
	TCHAR			szPlayerName[X_SHORT_NAME];		// 查询的玩家的名字
	DWORD			dwMapID;						// 玩家所在的大地图的ID
	Vector3			vPosition;						// 坐标
CMD_END

CMD_START(NC_Move2Player4Track)
	INT64			n64ItemTrack;					// 使用追踪道具的64位ID
	TCHAR			szPlayerName[X_SHORT_NAME];		// 要传送到此玩家附近
CMD_END

CMD_START(NS_Move2Player4Track)
	DWORD			dwErrorCode;					// 错误码
	DWORD			dwPlayerID;						// 要追踪的玩家ID
	TCHAR			szPlayerName[X_SHORT_NAME];		// 要追踪的玩家的名字
	DWORD			dwMapID;						// 传送到的地图的ID
	Vector3			pos;							// 传送过去后所在位置，传送失败时该字段无意义
	Vector3			faceTo;							// 传送过去后的朝向，传送失败时该字段无意义
CMD_END

CMD_START(NC_GetPositionAfterTrack)					// 传送过去后，每隔10秒请求一次玩家的最新位置
	INT64			n64Item;						// 道具的64位ID
	DWORD			dwRoleID;						// 要追踪的玩家的ID	
CMD_END

CMD_START(NS_GetPositionAfterTrack)
	DWORD			dwErrorCode;					// 错误码
	TCHAR			szPlayerName[X_SHORT_NAME];		// 要追踪的玩家的名字
	DWORD			dwMapID;						// 玩家当前所在的地图的ID
	Vector3			pos;							// 玩家的最新位置
CMD_END

//-----------------------------------------------------------------------
// 装备署名道具
//-----------------------------------------------------------------------
CMD_START(NC_EquipSignature)
	INT64			n64Item;			// 道具的64位id
	INT64			n64EquipID;			// 要署名装备的id
	TCHAR			szContent[11];		// 署名内容最多10个字符
CMD_END

CMD_START(NS_EquipSignature)
	DWORD			dwErrorCode;		
	INT64			n64EquipID;			// 要署名装备的64位id 
CMD_END


//-----------------------------------------------------------------------
// 仙界修行符 返回NS_UseItem
//-----------------------------------------------------------------------
CMD_START(NC_AddMiraclePrcNumItem)
	INT64   n64ItemID;      // 仙界修行符
CMD_END

//-----------------------------------------------------------------------
// 需要调用脚本的特殊物品的公共消息
//-----------------------------------------------------------------------
CMD_START(NC_SpecScriptItem)
	INT64   n64ItemID;						// 物品的64位ID
	TCHAR	szDesRoleName[X_SHORT_NAME];	// 物品作用于的角色的名字
CMD_END

CMD_START(NS_SpecScriptItem)
	DWORD	dwErrCode;
CMD_END

//-----------------------------------------------------------------------
// 增加灵兽的寿命
//-----------------------------------------------------------------------
CMD_START(NC_AddPetLiveTime)
	INT64   n64ItemID;						// 物品的64位ID
	DWORD	dwPetID;
CMD_END

CMD_START(NS_AddPetLiveTime)
	DWORD	dwErrCode;
CMD_END

#pragma pack(pop)