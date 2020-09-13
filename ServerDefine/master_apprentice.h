//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: master_apprentice.h
// author: twan
// actor:
// data: 2010-01-20
// last:
// brief: 师徒与数据库交互结构定义
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// 师徒数据枚举
//-----------------------------------------------------------------------------
enum EOneMasterApprenticeData
{
	EOMAD_JingWu,					// 精武点数
	EOMAD_QinWu,					// 勤武点数
	EOMAD_LevelUpBonus,				// 徒弟升级积分
	EOMAD_RewardNum,				// 出师奖励次数
	EOMAD_Apprentice,				// 徒弟数据
};

//-----------------------------------------------------------------------------
// 更新数据库方式
//-----------------------------------------------------------------------------
enum EMasterApprenticeModifyMode
{
	EMAMM_Set,						// 替换
	EMAMM_Mod,					    // 增量
};

//-----------------------------------------------------------------------------
// 存一项师徒数据
//-----------------------------------------------------------------------------
struct tagOneMAData2DB
{
	EOneMasterApprenticeData		eType;
	EMasterApprenticeModifyMode		eMode;
	BYTE							byData[4];

	tagOneMAData2DB& operator = (tagOneMAData2DB& other)
	{
		eType = other.eType;
		eMode = other.eMode;
		memcpy(byData, other.byData, sizeof(BYTE) * 4);
		return *this;
	}
};

//-----------------------------------------------------------------------------
// 存数据库结构
//-----------------------------------------------------------------------------
struct tagMasterApprentice2DB
{
	DWORD				dwID;				// 表索引（即师傅RoleID）
	INT					nJingWuPoint;		// 精武点数
	INT					nQinWuPoint;		// 勤武点数
	INT					nLevelUpBonus;		// 徒弟升级积分
	INT					nRewardNum;			// 出师奖励次数
	tagApprenticeData	sApprentice;		// 徒弟ID
};

struct tagForceBreakOutApprentice
{
	DWORD			dwApprenticeID;			// 徒弟ID
	DWORD			dwMasterID;				// 师傅ID
	tagDWORDTime	BreakoutInsertTime;		// 徒弟申请强制脱离师门时候的时间
};


CMD_START(NDBC_LoadMasterApprenticeData)
	DWORD						dwMasterID;
CMD_END

CMD_START(NDBS_LoadMasterApprenticeData)
	tagMasterApprentice2DB		sMasterApprentice;
CMD_END

CMD_START(NDBC_SaveAllMasterApprenticeData)				// 保存所有的师徒数据
	tagMasterApprentice2DB		sMasterApprentice;
CMD_END

CMD_START(NDBC_SaveOneMasterApprenticeData)
	DWORD						dwMasterID;
	tagOneMAData2DB				sOneMAData;
CMD_END

CMD_START(NDBC_InsertMasterApprenticeData)
	tagMasterApprentice2DB		sMasterApprentice;
CMD_END

CMD_START(NDBC_DeleteAnApprentice)
	DWORD						dwMasterID;				// 表索引ID
	DWORD						dwApprenticeID;			// 徒弟ID
CMD_END

CMD_START(NDBC_LoadMasterApprenticeCount)
	DWORD						dwRoleID;
CMD_END

CMD_START(NDBS_LoadMasterApprenticeCount)
	DWORD						dwRoleID;
	DWORD						dwMasterID;
	DWORD						dwApprenticeCount;
CMD_END

CMD_START(NDBC_LoadForceBreakOutApprenticeList)
CMD_END

CMD_START(NDBS_LoadForceBreakOutApprenticeList)
	DWORD						dwForceBreakOutApprenceCnt;
	tagForceBreakOutApprentice	sForceBreakOutApprentice[1];
CMD_END

CMD_START(NDBC_InsertForceBreakOutApprentice)
	tagForceBreakOutApprentice sForceBreakOutApprentice;
CMD_END

CMD_START(NDBC_RemoveForceBreakOutApprentice)
	tagForceBreakOutApprentice sForceBreakOutApprentice;
CMD_END

CMD_START(NDBC_ModifyApprenticeJingWuPoint)
	DWORD	dwApprenticeID;
	DWORD	dwJingWuPoint;
CMD_END

//-----------------------------------------------------------------------------
// 存一项师徒数据（含tagMasterApprentice2DB，调用数据库update通用模板时用）
//-----------------------------------------------------------------------------
struct tagOneMASaveData
{
	tagMasterApprentice2DB		loadData;
	tagOneMAData2DB				oneMAData;
};

#pragma pack(pop)