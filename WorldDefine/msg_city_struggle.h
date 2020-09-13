//-----------------------------------------------------------------------------
//!\file msg_city_struggle.h
//!
//!\date 2009-09-24
//!\author	twan
//!\brief 客户端与服务器之间关于城战的消息
//-----------------------------------------------------------------------------

#pragma once
#pragma pack(push, 1)

#include "msg_common_errorcode.h"
#include "city_struggle_define.h"

//----------------------------------------------------------------------------
// 玩家进入城战场景
//----------------------------------------------------------------------------
CMD_START(NS_RoleEnterCityStruggle)
	INT32	nNum;						// 敌对阵营帮派数量
	DWORD	dwEnemyGuildID[1];			// 敌对阵营帮派ID
CMD_END

//----------------------------------------------------------------------------
// 玩家离开城战场景
//----------------------------------------------------------------------------
CMD_START(NS_RoleLeaveCityStruggle)

CMD_END

//----------------------------------------------------------------------------
// 城战信息同步给客户端（所有信息）
//----------------------------------------------------------------------------
CMD_START(NS_CityStruggleInfo)
	tagLongZhuState		sLongZhuState;		// 龙珠当前状态
	tagYinYangZhuState	sYinYangZhuState[MAX_YINYANGZHU_NUM];		// 阴阳柱当前状态
	tagCityDoorState	sDoorState[MAX_CITY_DOOR_NUM];				// 城门数量
	INT32				nAtkPlayerNum;		// 进攻联盟玩家数
	INT32				nDefPlayerNum;		// 防御联盟玩家数
	INT32				nWuZiKuNum;			// 防御联盟物资库数量
CMD_END

//----------------------------------------------------------------------------
// 城战信息同步给客户端（单条改变的信息）
//----------------------------------------------------------------------------
/*
	不同类型所发数据不同：
	ECSCT_PlayerSumChange		-->		nType2为势力枚举，nData为人数
	ECSCT_LongZhuBeSealing		-->		nType2默认为0，无意义，nData为ECityForceType枚举
	ECSCT_LongZhuCancelSeal		-->		nType2默认为0，无意义，nData为ECityForceType枚举
	ECSCT_LongZhuBeSeized		-->		nType2为龙珠封印后倒计时（单位分钟），nData为ECityForceType枚举
	ECSCT_YinYangZhuSeized		-->		nType2为阴阳柱索引或者TypeID，nData为阴阳柱占领势力ECityForceType枚举
	ECSCT_WuZiKu				-->		nType2默认为0，无意义，nData为物资库数量
	ECSCT_DoorDestroied			-->		nType2为ECityStruggleDoorPos枚举，表示城门方位，nData为门修复时间，若为-1则无法再次修复
	ECSCT_DoorRepaired			-->		nType2为ECityStruggleDoorPos枚举，表示城门方位，nData默认为0
*/
CMD_START(NS_CityStruggleInfoChange)
	ECityStruggleChangeType		eType1;			// 类型1
	INT32						nType2;			// 类型2
	INT32						nData;			// 状态改变的数据
CMD_END

//----------------------------------------------------------------------------
// 城战个人战绩
//----------------------------------------------------------------------------
CMD_START(NC_CSRoleRecord)

CMD_END

CMD_START(NS_CSRoleRecord)
	tagCSRoleRankData			RoleRank[MAX_CS_RANK_NUM];	// 战绩排名
	tagCSRoleData2Client		RoleData;					// 个人战绩
	ECityStruggleResult			eResult;					// 城战结果
	INT							nExploit;					// 帮派功勋
	INT							nContribute;				// 帮派贡献
	INT							nMoney;						// 金钱奖励
CMD_END

//----------------------------------------------------------------------------
// 等待召唤
//----------------------------------------------------------------------------
CMD_START(NC_WaitSummon)
	DWORD	dwCreatureID;			// 守护石ID
CMD_END

CMD_START(NS_WaitSummon)
	BOOL	bSuccess;				// 是否成功激活守护石
CMD_END

//----------------------------------------------------------------------------
// 取消召唤
//----------------------------------------------------------------------------
CMD_START(NC_CancelSummon)
	DWORD	dwCreatureID;			// 守护石ID
CMD_END

//----------------------------------------------------------------------------
// 召唤人数改变
//----------------------------------------------------------------------------
CMD_START(NS_SummonPersonChange)
	INT32	nPerson;				// 当前召唤人数
	INT32	nNeedPerson;			// 需要人数
CMD_END

//----------------------------------------------------------------------------
// 开始召唤
//----------------------------------------------------------------------------
CMD_START(NS_BeginSummon)
	INT		nCountDown;				// 召唤时间
CMD_END

#pragma pack(pop)