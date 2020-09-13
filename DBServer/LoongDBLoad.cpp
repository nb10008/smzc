//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: LoongDBLoad.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: 数据库操作应用层实现 -- 读取数据相关
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/famehall_define.h"
#include "../WorldDefine/mall_define.h"
#include "../WorldDefine/master_apprentice.h"
#include "../WorldDefine/quest_board.h"
#include "../WorldDefine/msg_eggbroker.h"

#include "../ServerDefine/msg_shortcut_quickbar.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/msg_vcard.h"
#include "../ServerDefine/msg_mall.h"
#include "../ServerDefine/msg_vipstall.h"
#include "../ServerDefine/msg_rankings.h"
#include "../ServerDefine/activity_define.h"
#include "../ServerDefine/rankings_define.h"
#include "../ServerDefine/msg_city.h"
#include "../ServerDefine/master_apprentice.h"
#include "../ServerDefine/athletics_data_define.h"
#include "../ServerDefine/msg_draft_show.h"
#include "../ServerDefine/msg_guild_war_declare.h"
#include "../ServerDefine/msg_guild.h"
#include "../ServerDefine/msg_godbid.h"
#include "../ServerDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_fashion.h"
#include "../ServerDefine/data_collect.h"
#include "../ServerDefine/msg_account_reactive.h"
#include "LoongDB.h"
#include "../Beton/Field.h"
#include "Loong.h"

#include "../ServerDefine/msg_item.h"

const INT HolyEquip_Begin_Index = 152;
const INT Holy_Begin_Index = 137;
const INT Fabao_Begin_Index = 91;
const INT SoulCrystal_Begin_Index = 80;

//-------------------------------------------------------------------------------------------------------
// 读取玩家某个角色全部信息,内存由外部分配
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadRole(DWORD dwAccountID, DWORD dwRoleID, tagRoleDataLoad *pRoleData)
{
	// 初始化角色 -- 仅部分内存进行了初始化
	ZeroMemory(pRoleData, sizeof(tagRoleDataLoad));

	LPVOID pData = pRoleData;
	
	// 从roledata表中读取相关数据
	if(!LoadRoleData(pData, dwAccountID, dwRoleID))
	{
		pRoleData->dwSize = 0;
		return FALSE;
	}

	DWORD dwIndexes[50] = {0};
	INT nMsgNum = 0;
	LoadLeftMsgIndexes(dwIndexes, dwRoleID, nMsgNum);
	if (nMsgNum > 0)
	{
		pRoleData->RoleDataSave.bHasLeftMsg = TRUE;		
	}
	else
	{
		pRoleData->RoleDataSave.bHasLeftMsg = FALSE;
	}


	/**************************************************************************
	* 依次读取: 技能列表，状态列表，称号列表，物品列表，装备列表，
	*			好友列表，仇敌列表，任务列表，已完成任务列表, 物品冷却时间表，
	*			友好度表, 黑名单表，师徒表，  同门，已开启试炼状态表
	***************************************************************************/

	tagRoleDataSave *pDataSave = &pRoleData->RoleDataSave;
	pData = pDataSave->byData; // 记录当前指针位置

	// 检查是否做过数据转移
	if (P_VALID(pDataSave) && pDataSave->byTrainStatesTransfered == 0)
	{
		// 做<试炼状态表>的数据转移
		TransferTrainStates(dwRoleID);
	}
	
	// 1.技能列表
	LoadSkill(pData, pDataSave->nSkillNum, dwRoleID);
//	pData += pDataSave->nSkillNum * sizeof(tagSkill);

	// 2.状态列表
	LoadRoleBuff(pData, pDataSave->nBuffNum, dwRoleID);
//	pData += pDataSave->nBuffNum * sizeof(tagBuffSave);

	// 4.称号列表
	LoadTitle(pData, pDataSave->nTitleNum, dwRoleID);
//	pData += pDataSave->nTitleNum * sizeof(tagTitleSave);
	
	// 5.物品列表 + 6.装备列表
	LoadItem(pData, pDataSave->nItemNum, dwAccountID, dwRoleID);
//	pData += pDataSave->nItemNum * sizeof(tagItem);

	// 7.好友列表
	LoadFriend(pData, pDataSave->nFriendNum, dwRoleID);
//	pData += pDataSave->nFriendNum * sizeof(DWORD);
	
	// 8.仇敌列表
	LoadEnemy(pData, pDataSave->nEnemyNum, dwRoleID);
//	pData += pDataSave->nEnemyNum * sizeof(DWORD);
	
	// 9.任务列表
	LoadTask(pData, pDataSave->nQuestNum, dwRoleID);
//	pData += pDataSave->nQuestNum * sizeof(tagQuestSave);

	// 10.已完成任务列表
	LoadTaskDone(pData, pDataSave->nQuestDoneNum, dwRoleID);
//	pData += pDataSave->nQuestDoneNum * sizeof(tagQuestDoneSave);

	// 10.5 已完成日常任务列表 
    LoadDailyTaskDone(pData, pDataSave->nDailyQuestDoneNum, dwRoleID);

	// 11.物品冷却时间表
	LoadItemCDTime(pData, pDataSave->nItemCDTimeNum, dwRoleID);

	// 12.友好度表
	LoadFriendship(pData, pDataSave->nFriendshipNum, dwRoleID);
	INT nFriendNumEx = 0;
	LoadFriendshipEx(pData, nFriendNumEx, dwRoleID);
	pDataSave->nFriendshipNum += nFriendNumEx;

	// 13.黑名单表
	LoadBlackList(pData, pDataSave->nBlackNum, dwRoleID);

	// 14 角色名贴
	LoadVCard(pData, dwRoleID);

	// 15 读取氏族数据
	if(!LoadClanData(pData,dwRoleID))
	{
		pRoleData->dwSize = 0;
		return FALSE;
	}

	// 16 读取宠物
	if (!LoadPets(pData, dwRoleID, pDataSave->nPetsNum))
	{
		pRoleData->dwSize = 0;
		return FALSE;
	}

	// 17 读取师傅
	LPVOID p = static_cast<LPVOID>(pDataSave);
	if (!LoadMaster(p, pData, dwRoleID, pDataSave->dwMasterID))
	{
		pRoleData->dwSize = 0;
		return FALSE;
	}

	// 18 读取同门
	if (!LoadClassMates(pData, dwRoleID, pDataSave->dwMasterID))
	{
		pRoleData->dwSize = 0;
		return FALSE;
	}

	// 19 读取角色已经开启的试炼状态表
	LoadOpenTrainStates(pData, pDataSave->nOpenTrainCnt, dwRoleID);
	
	// 20 任务板任务
	LoadBoardQuest(pData, pDataSave->nBoardQuestNum, dwRoleID);

	//LoadCurMonthOnlineTime(pData, pDataSave->nMonthOnlineTimeCnt, dwAccountID, dwRoleID);

	// 加载砸金蛋信息
	LoadEggInfo(pData, pDataSave->nEggInfoCnt, dwAccountID, dwRoleID);

	// 加载衣橱信息
	LoadWardrobeLayerInfo(pData, pDataSave->nWardrobeLayerCnt, dwAccountID, dwRoleID);

	// 神职领域
	LoadArea(pData, pDataSave->nAreaNum, dwRoleID);

	// 购买商城物品计数
	LoadBuyMallItemCount(pData, pDataSave->nBuyMallItemLogCount, dwRoleID);

	//加载元神
	/*if ( !LoadHolySoulInfo(dwRoleID,pData))
	{
		pDataSave->bIsHolySoulActivated = false;
	}
	else
	{
		pDataSave->bIsHolySoulActivated = true;
	}*/

	// 计算读取数据大小
	pRoleData->dwSize = (sizeof(tagRoleDataLoad) - 1) + ((BYTE*)pData - pDataSave->byData);
	
	return TRUE;
}


//-------------------------------------------------------------------------------------------------------
// 读取所有玩家简易信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadAllRoleInfo(LPVOID pData, INT &nRoleNum)
{
	return Load(pData, &nRoleNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadAllRoleInfo, &CLoongDB::ProcResLoadAllRoleInfo);
}

//-------------------------------------------------------------------------------------------------------
// 获取某个帐号的所有角色基本信息,内存由外部分配
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadSimRole(DWORD dwAccountID, INT &nRoleNum, LPVOID pSimRole)
{
	return Load(pSimRole, &nRoleNum, dwAccountID, GT_INVALID, &CLoongDB::FormatLoadSimRole, &CLoongDB::ProcResLoadSimRole);
}
BOOL CLoongDB::LoadOneRoleInfo(DWORD dwAccountID, DWORD dwRoleID, LPVOID pRoleInfo)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream))
		return FALSE;

	// 清空流
	pStream->Clear();
	FormatLoadOneRoleInfo(pStream,dwAccountID,dwRoleID);
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		m_pDBLoong->ReturnStream(pStream);
		return FALSE;
	}
	BOOL re = FALSE;
	if (pResult->GetRowCount() == 1)
	{
		INT32 num = 0;
		ProcResLoadAllRoleInfo(pRoleInfo,&num,pResult);
		re = TRUE;
	}
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
	return re;
}
//-------------------------------------------------------------------------------------------------------
// 读取角色属性信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadRoleData(LPVOID &pData, DWORD dwAccountID, DWORD dwRoleID)
{
	return Load(pData, NULL, dwAccountID, dwRoleID, &CLoongDB::FormatLoadRoleData, &CLoongDB::ProcResLoadRoleData);
}

//-------------------------------------------------------------------------------------------------------
// 读取角色技能信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadSkill(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadSkill, &CLoongDB::ProcResLoadSkill);
}

//-------------------------------------------------------------------------------------------------------
// 读取角色任务板信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadBoardQuest(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadBoardQuest, &CLoongDB::ProcResLoadBoardQuest);
}

//-------------------------------------------------------------------------------------------------------
// 读取角色当月在线时间
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadCurMonthOnlineTime(LPVOID &pData, INT32 &nNum, DWORD dwAccountID, DWORD dwRoleID)
{
	return Load(pData, &nNum, dwAccountID, dwRoleID, &CLoongDB::FormatLoadCurMonthOnlineTime, &CLoongDB::ProcResLoadCurMonthOnlineTime);
}

BOOL CLoongDB::LoadEggInfo(LPVOID &pData, INT32 &nNum, DWORD dwAccountID, DWORD dwRoleID)
{
	return Load(pData, &nNum, dwAccountID, dwRoleID, &CLoongDB::FormatLoadEggInfo, &CLoongDB::ProcResLoadEggInfo);
}

BOOL CLoongDB::LoadWardrobeLayerInfo(LPVOID &pData, INT32 &nNum, DWORD dwAccountID, DWORD dwRoleID)
{
	return Load(pData, &nNum, dwAccountID, dwRoleID, &CLoongDB::FormatLoadWardrobeLayerInfo, &CLoongDB::ProcResLoadWardrobeLayerInfo);
}

//-------------------------------------------------------------------------------------------------------
// 读取角色buff信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadRoleBuff(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadRoleBuff, &CLoongDB::ProcResLoadRoleBuff);
}

//-------------------------------------------------------------------------------------------------------
// 读取称号信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadTitle(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadTitle, &CLoongDB::ProcResLoadTitle);
}

//-------------------------------------------------------------------------------------------------------
// 读取角色物品&装备信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadItem(LPVOID &pData, INT32 &nNum, DWORD dwAccountID, DWORD dwRoleID)
{
	return Load(pData, &nNum, dwAccountID, dwRoleID, &CLoongDB::FormatLoadItem, &CLoongDB::ProcResLoadItem);
}

//-------------------------------------------------------------------------------------------------------
// 读取好友信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadFriend(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadFriend, &CLoongDB::ProcResLoadFriend);
}

//-------------------------------------------------------------------------------------------------------
// 读取仇敌信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadEnemy(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadEnemy, &CLoongDB::ProcResLoadEnemy);
}

//-------------------------------------------------------------------------------------------------------
// 读取任务信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadTask(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadTask, &CLoongDB::ProcResLoadTask);
}

//-------------------------------------------------------------------------------------------------------
// 读取已完成任务信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadTaskDone(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadTaskDone, &CLoongDB::ProcResLoadTaskDone);
}

BOOL CLoongDB::LoadDailyTaskDone(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
    return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadDailyTaskDone, &CLoongDB::ProcResLoadDailyTaskDone);     
}

//-------------------------------------------------------------------------------------------------------
// 读取已完成任务简易信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadSimTaskDone(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadSimTaskDone, &CLoongDB::ProcResLoadSimTaskDone);
}

//-------------------------------------------------------------------------------------------------------
// 读取物品冷却时间信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadItemCDTime(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadItemCDTime, &CLoongDB::ProcResLoadItemCDTime);
}

//-------------------------------------------------------------------------------------------------------
// 读取友好度信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadFriendship(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadFriendship, &CLoongDB::ProcResLoadFriendship);
}

//-------------------------------------------------------------------------------------------------------
// 读取友好度信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadFriendshipEx(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadFriendshipEx, &CLoongDB::ProcResLoadFriendship);
}

//-------------------------------------------------------------------------------------------------------
// 读取友好度信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadBlackList(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadBlackList, &CLoongDB::ProcResLoadBlackList);
}

//-------------------------------------------------------------------------------------------------------
// 读取师徒信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadMaster(LPVOID &pDataSave, LPVOID &pData, DWORD dwRoleID, DWORD dwMasterID)
{
	/*
		读取师徒数据库时分两步，先在数据库master表中查找该玩家的RoleID，如果有记录，则保存其徒弟数据，
		精武点数，勤武点数和徒弟升级积分。
		然后在master表中查找该玩家师傅的RoleID，确认其师傅的弟子数据中是否有当前玩家的记录，如果没有则
		代表该玩家师傅在该玩家离线时将其逐出师门，所以应该重置该玩家的MasterID为-1，且清空精武点数
		如果查找时都没有记录则应初始化其数据
	*/

	INT32 nNum = 0;
	LPVOID pReset1 = reinterpret_cast<BYTE *>(pData);

	BOOL bRet1 = Load(pData, &nNum, dwMasterID, dwRoleID, &CLoongDB::FormatLoadMaster, &CLoongDB::ProcResLoadApprentice);
	if (bRet1)
	{
		pReset1 = reinterpret_cast<BYTE *>(pData) - sizeof(tagMasterApprentice2DB);
	}
	else
	{
		// 没有记录的话初始化数据
		MTRANS_POINTER(p, pData, tagMasterApprentice2DB);
		p->nJingWuPoint = 0;
		p->nQinWuPoint = 0;
		p->nLevelUpBonus = 0;
		p->nRewardNum = 0;
		ZeroMemory(p->sApprentice.dwData, sizeof(p->sApprentice));

		DWORD dwSize = sizeof(tagMasterApprentice2DB);
		pData = reinterpret_cast<BYTE *>(pData) + dwSize;
	}

	BOOL bRet2 = Load(pData, &nNum, dwRoleID, dwMasterID, &CLoongDB::FormatLoadMaster, &CLoongDB::ProcResLoadMaster);
	if (bRet2)
	{
		// 重获指针
		LPVOID pTmpData = (BYTE*)pData - sizeof(tagMasterApprentice2DB);
		MTRANS_POINTER(pAppData, pTmpData, tagMasterApprentice2DB);

		// 确定master表中是否dwRoleID的徒弟记录，如果没有则置空pDataSave的MasterID
		ASSERT(pAppData->dwID == dwMasterID);
		BOOL bHave = FALSE;
		nNum = sizeof(pAppData->sApprentice.dwData) / sizeof(DWORD);
		for (int i=0; i!=nNum; ++i)
		{
			if (pAppData->sApprentice.dwData[i] == dwRoleID)
			{
				bHave = TRUE;
				break;
			}
		}

		// 师傅弟子表中没有dwRoleID的记录，置空MasterID，清空精武点数
		if (!bHave)
		{
			MTRANS_POINTER(pSave, pDataSave, tagRoleDataSave);
			pAppData->nJingWuPoint = 0;
			pAppData->dwID = GT_INVALID;
		}
	}
	else
	{
		MTRANS_POINTER(p, pData, tagMasterApprentice2DB);
		p->dwID = GT_INVALID;

		DWORD dwSize = sizeof(tagMasterApprentice2DB);
		pData = reinterpret_cast<BYTE *>(pData) + dwSize;
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// 读取同门信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadClassMates(LPVOID &pData, DWORD dwRoleID, DWORD dwMasterID)
{
	INT32 nNum = 0;
	BOOL bRet = Load(pData, &nNum, dwRoleID, dwMasterID, &CLoongDB::FormatLoadMaster, &CLoongDB::ProcResLoadClassMates);

	// 没有记录则初始化数据
	if (!bRet)
	{
		MTRANS_POINTER(p, pData, tagClassMateData);
		*p = tagClassMateData();

		DWORD dwSize = sizeof(tagClassMateData);
		pData = (BYTE *)pData + dwSize;
	}
	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// 读取已开启试炼状态表
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadOpenTrainStates(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadOpenTrainStates, &CLoongDB::ProcResLoadOpenTrainStates);
}

VOID CLoongDB::TransferTrainStates(DWORD dwRoleID)
{
#if 0
	// 获取流
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 清空流
	pStream->Clear();

	// 格式化查询的角色数据
	pStream->FillString("select TrainStateTable from roledata where RoleID=")<< dwRoleID;

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		goto Exit;
	}

	// 检验并保存查询结果
	if (pResult->GetRowCount() > 0)
	{
		tagTrainStateTable TrainStateTable;
		(*pResult)[0].GetBlob(&TrainStateTable,sizeof(tagTrainStateTable));
		for (INT nCnt = 0; nCnt < TrainStateTable.nTrainCnt; nCnt++)
		{
			INT nState = 0;
			if ((TrainStateTable.pTrainState[nCnt]).eTrainState == ETS_Open)
			{
				nState = 0;
			}
			else if ((TrainStateTable.pTrainState[nCnt]).eTrainState == ETS_Pass)
			{
				nState = 1;
			}
			else
			{
				continue;
			}
			DWORD dwTrainID = (TrainStateTable.pTrainState[nCnt]).dwTrainID;
			SaveOpenTrainState(dwRoleID, dwTrainID, nState);
		}
	}

Exit:
#ifdef _DEBUG
	IMSG(_T("%s\r\n"), m_pUtil->AnsiToUnicode(pStream->GetBuf()));
	IMSG(_T("Results Row Count: %d\n"), P_VALID(pResult) ? pResult->GetRowCount() : 0);
#endif

	++m_dwReadTimes;

	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);
#endif
}

//-------------------------------------------------------------------------------------------------------
// 读取角色名贴信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadVCard(LPVOID &pData, DWORD dwRoleID)
{
	INT32 nNum = 0;
	BOOL bRtv = Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadVCard, &CLoongDB::ProcResLoadVCard);
// 	if (0 == nNum)
// 	{
// 		InsertVCard(dwRoleID);
// 		bRtv = Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadVCard, &CLoongDB::ProcResLoadVCard);
// 		ASSERT(1 == nNum);
// 	}
	return bRtv;
}

//-------------------------------------------------------------------------------------------------------
// 读取氏族信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadClanData(LPVOID &pData, DWORD dwRoleID)
{
	INT32 nNum = 0;
	BOOL bRtv = Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadClanData, &CLoongDB::ProcResLoadClanData);
	if (1 != nNum)
	{
		InsertClanData(dwRoleID);
		bRtv = Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadClanData, &CLoongDB::ProcResLoadClanData);
		ASSERT(1 == nNum);
	}
	return bRtv;
}

//-------------------------------------------------------------------------------------------------------
// 读取百宝袋物品
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadItemBaiBao(DWORD dwAccountID, INT32 nNumLoad, INT64 n64Serial, INT32 &nNumLoaded, INT32 &nDataSize, LPVOID pData)
{
	ResetLoadNumLim(nNumLoad);
	ResetLoadSerial(n64Serial);

	LPVOID	pTmpData = pData;
	BOOL	bRet = Load(pTmpData, &nNumLoaded, dwAccountID, GT_INVALID, &CLoongDB::FormatLoadItemBaiBao, &CLoongDB::ProcResLoadItem);

	if(bRet)
	{
		nDataSize = (INT32)((BYTE*)pTmpData - (BYTE*)pData);
	}

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// 读取百宝袋元宝
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadBaiBaoYuanBao(DWORD dwAccountID, INT32& nYuanBao)
{
	LPVOID pData = (LPVOID)&nYuanBao;
	return Load(pData, NULL, dwAccountID, GT_INVALID, &CLoongDB::FormatLoadBaiBaoYuanBao, &CLoongDB::ProcResLoadBaiBaoYuanBao);
}

//-------------------------------------------------------------------------------------------------------
// 读取百宝袋物品Log
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadBaibaoLogInfo(DWORD dwAccountID, INT32 &nNumLoaded, INT32 &nDataSize, LPVOID pData)
{
	LPVOID	pTmpData = pData;
	BOOL	bRet = Load(pData, &nNumLoaded, dwAccountID, GT_INVALID, &CLoongDB::FormatLoadBaiBaoLog, &CLoongDB::ProcResLoadBaiBaoLog);

	if(bRet)
	{
		nDataSize = (INT32)((BYTE*)pData - (BYTE*)pTmpData);
	}

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// 插入一条百宝袋Log信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertBaibaoLogInfo(DWORD dwAccountID, LPVOID pData)
{
	if (!GT_VALID(dwAccountID))
		return FALSE;

	// 判断记录数量上限, 删除多余的记录
	Load(pData, NULL, dwAccountID, GT_INVALID, &CLoongDB::FormatLoadBaiBaoLog, &CLoongDB::ProcResCountBaiBaoLog);

	// 插入记录
	return Update(dwAccountID, pData, 1, GT_INVALID, &CLoongDB::FormatInsertBaiBaoLog);
}

//-------------------------------------------------------------------------------------------------------
// 读取需记log的物品TypeID
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadItemNeedLog(LPVOID pTypeID, INT32 &nItemNum)
{
	return Load(pTypeID, &nItemNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadItemNeedLog, &CLoongDB::ProcResLoadItemNeedLog);
}

//-------------------------------------------------------------------------------------------------------
// 获取所有玩家元宝交易账户信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadAllYBAccount(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadYBAccount, &CLoongDB::ProcResLoadYBAccount);
}

//-------------------------------------------------------------------------------------------------------
// 获取所有玩家的元宝交易订单
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadAllYBOrder(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadYBOrder, &CLoongDB::ProcResLoadYBOrder);
}

//-------------------------------------------------------------------------------------------------------
// 获取一个玩家的元宝交易订单
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadRoleAllYBOrder(LPVOID pData, INT&nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadRoleYBOrder, &CLoongDB::ProcResLoadRoleOrder);
}

//-------------------------------------------------------------------------------------------------------
// 读取所有帮派团购信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadAllGPInfo(OUT PVOID pData, INT32 &nInfoNum)
{
	return Load(pData, &nInfoNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadAllGPInfo, &CLoongDB::ProcResLoadAllGPInfo);
}

//-------------------------------------------------------------------------------------------------------
// 读取所有限量商品信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadLimitItemInfo(OUT PVOID pData, INT32 &nInfoNum)
{
    return Load(pData, &nInfoNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadLimitItemInfo, &CLoongDB::ProcResLoadLimitItemInfo);
}

//-------------------------------------------------------------------------------------------------------
// 读取固定活动数据
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadActivityData(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatActivityData, &CLoongDB::ProcResLoadActivityData);
}

//-------------------------------------------------------------------------------------------------------
// 读取各种排行榜
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadStrengthRankings(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatStrengthRankings, &CLoongDB::ProcResLoadStrengthRankings);
}

BOOL CLoongDB::LoadFameRankings(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatFameRankings, &CLoongDB::ProcResLoadFameRankings);
}

BOOL CLoongDB::LoadLevelRankings(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLevelRankings, &CLoongDB::ProcResLoadLevelRankings);
}

BOOL CLoongDB::LoadPetRankings(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatPetRankings, &CLoongDB::ProcResLoadPetRankings);
}

BOOL CLoongDB::LoadEquipRankings(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatEquipRankings, &CLoongDB::ProcResLoadEquipRankings);
}

BOOL CLoongDB::LoadFlowerRankings(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatFlowerRankings, &CLoongDB::ProcResLoadFlowerRankings);
}

BOOL CLoongDB::LoadEggRankings(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatEggRankings, &CLoongDB::ProcResLoadEggRankings);
}
//-------------------------------------------------------------------------------------------------------
// 获取所有VIP摊位信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::GetAllVIPStallInfo(OUT PVOID pData)
{
	INT32 nVIPStallNum = 0;

	// 更新到数据库
	Load(pData, &nVIPStallNum, GT_INVALID, GT_INVALID, (&CLoongDB::FormatGetAllVIPStallInfo), (&CLoongDB::ProcResGetAllVIPStallInfo));

	if (nVIPStallNum < VIP_STALL_MAX_NUM)
	{
		tagVipStall2DB* pInfo = (tagVipStall2DB*)pData;

		for (int n=0; n<VIP_STALL_MAX_NUM; n++)
		{
			if (pInfo[n].byStallIndex == (BYTE)GT_INVALID)
			{
				pInfo[n].byStallIndex	= n;
				pInfo[n].dwOwnerID		= GT_INVALID;
				pInfo[n].nRemainTime	= GT_INVALID;
				Update(GT_INVALID, &pInfo[n], 1, GT_INVALID, (&CLoongDB::FormatInsertVIPStall));
			}
		}
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// 获取指定城市信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadCityInfo(DWORD dwCityID, LPVOID pData)
{
	return Load(pData, NULL, dwCityID, GT_INVALID, &CLoongDB::FormatLoadCityInfo, &CLoongDB::ProcResLoadCityInfo);
}

//-------------------------------------------------------------------------------------------------------
// 获取指定师徒信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadMasterApprenticeData(DWORD dwID, LPVOID pData)
{
	return Load(pData, NULL, dwID, GT_INVALID, &CLoongDB::FormatLoadMasterApprenticeData, &CLoongDB::ProcResLoadMasterApprenticeData);
}

//-------------------------------------------------------------------------------------------------------
// 获取帮派指定氏族声望
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::GetGuildClanVal(DWORD dwType, INT& nGuildNum, DWORD *pGuildID, LPVOID pData)
{
	INT nOutputNum = 0;

	for (int n=0; n<nGuildNum; n++)
	{
		if (!GT_VALID(pGuildID[n]) || (dwType < ECLT_BEGIN || dwType >= ECLT_END))
			continue;

		MTRANS_POINTER(pClanVal, pData, tagGuildClanValue);
		pClanVal->dwGuildID = pGuildID[n];

		if (Load(pData, NULL, pGuildID[n], dwType, &CLoongDB::FormatGetGuildClanVal, &CLoongDB::ProcResGetGuildClanVal))
		{
			nOutputNum++;
		}

	}

	nGuildNum = nOutputNum;

	return TRUE;
}

//**********************************************************************************************************

//-------------------------------------------------------------------------------------------------------
// 读取所有玩家简易信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadAllRoleInfo(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("roledata", "AccountID,RoleID,RoleName,RoleNameCrc,Sex,MapID,Level,LogoutTime,Class,LoverID,GuildID,MARelation,VipLevel,FamilyID,LoverID");
	pStream->SetWhere();
	pStream->FillString("RemoveFlag=") << 0;
}
VOID CLoongDB::FormatLoadOneRoleInfo(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("roledata", "AccountID,RoleID,RoleName,RoleNameCrc,Sex,MapID,Level,LogoutTime,Class,LoverID,GuildID,MARelation,VipLevel,FamilyID,LoverID");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
	pStream->FillString(" and RoleID=") << dwRoleID;
	pStream->FillString(" and RemoveFlag=") << 0;
}

//-------------------------------------------------------------------------------------------------------
// 读取所有玩家简易信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadAllRoleInfo(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pRoleInfo, pData, tagRoleInfo);
	*pNum = pRes->GetRowCount();

	// RoleID,RoleName,RoleNameCrc,Sex,MapID,Level
	for(INT i=0; i<*pNum; ++i)
	{
		(*pRes)[2].GetTCHAR(pRoleInfo[i].szRoleName, X_SHORT_NAME);
		pRoleInfo[i].dwAccountID = (*pRes)[0].GetDword();
		pRoleInfo[i].dwRoleID = (*pRes)[1].GetDword();
		pRoleInfo[i].dwRoleNameCrc = (*pRes)[3].GetDword();
		pRoleInfo[i].bySex = (*pRes)[4].GetByte();
		pRoleInfo[i].dwMapID = (*pRes)[5].GetDword();
		pRoleInfo[i].byLevel = (*pRes)[6].GetByte();
		DataTime2DwordTime(pRoleInfo[i].dwTimeLastLogout, (*pRes)[7].GetString(), (*pRes)[7].GetLen());
		pRoleInfo[i].byClass = (*pRes)[8].GetByte(); //职业
		pRoleInfo[i].dwLoverID = (*pRes)[9].GetDword(); //爱人
		pRoleInfo[i].dwGuildID = (*pRes)[10].GetDword();//帮派
		pRoleInfo[i].nMARelation = (*pRes)[11].GetInt();	// 是否有师徒关系
		pRoleInfo[i].nVIPLevel = (*pRes)[12].GetInt();
		pRoleInfo[i].dwFamilyID = (*pRes)[13].GetDword();//家族
		pRoleInfo[i].dwLoverID = (*pRes)[14].GetDword();
		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// 读取所有玩家元宝交易账户SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadYBAccount(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("ybaccount", "RoleID,YuanBao,Gold,Silver,bSellOrder,bBuyOrder");
}

//-------------------------------------------------------------------------------------------------------
// 读取所有玩家元宝交易订单SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadYBOrder(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("yuanbaoorder", "dwID,RoleID,OrderType,Price,Number,OrderMode,StartTime,EndTime,AvgPrice,DealNum");
	pStream->SetWhere();
	pStream->FillString("OrderMode=") << 0;
}

//-------------------------------------------------------------------------------------------------------
// 读取一个玩家元宝交易订单SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadRoleYBOrder(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("yuanbaoorder", "dwID,RoleID,OrderType,Price,Number,OrderMode,StartTime,EndTime,AvgPrice,DealNum");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取所有玩家元宝交易账户结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadYBAccount(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pYBAccount, pData, tagYBAccount);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum; ++i)
	{
		pYBAccount[i].dwRoleID = (*pRes)[0].GetDword();
		pYBAccount[i].nYuanBao = (*pRes)[1].GetDword();
		pYBAccount[i].nGold = (*pRes)[2].GetInt();
		pYBAccount[i].nSilver = (*pRes)[3].GetInt();
		pYBAccount[i].bSellOrder = (*pRes)[4].GetBool();
		pYBAccount[i].bBuyOrder = (*pRes)[5].GetBool();

		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// 读取所有玩家元宝交易订单结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadYBOrder(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pYBOrder, pData, tagYuanBaoOrder);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum; ++i)
	{
		pYBOrder[i].dwID = (*pRes)[0].GetDword();
		pYBOrder[i].dwRoleID = (*pRes)[1].GetDword();
		pYBOrder[i].eYBOType = (EYBOTYPE)(*pRes)[2].GetInt();
		pYBOrder[i].nPrice = (*pRes)[3].GetInt();
		pYBOrder[i].nNum = (*pRes)[4].GetInt();
		pYBOrder[i].eYBOMode = (EYBOMODE)(*pRes)[5].GetInt();
		pYBOrder[i].dwStartTime = (*pRes)[6].GetDword();
		pYBOrder[i].dwEndTime =  (*pRes)[7].GetDword();
		pYBOrder[i].nAvgPrice = (*pRes)[8].GetInt();
		pYBOrder[i].nDealNum = (*pRes)[9].GetInt();

		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// 读取一个玩家元宝交易订单结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadRoleOrder(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pYBOrder, pData, tagYuanBaoOrder);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum; ++i)
	{
		pYBOrder[i].dwID = (*pRes)[0].GetDword();
		pYBOrder[i].dwRoleID = (*pRes)[1].GetDword();
		pYBOrder[i].eYBOType = (EYBOTYPE)(*pRes)[2].GetInt();
		pYBOrder[i].nPrice = (*pRes)[3].GetInt();
		pYBOrder[i].nNum = (*pRes)[4].GetInt();
		pYBOrder[i].eYBOMode = (EYBOMODE)(*pRes)[5].GetInt();
		pYBOrder[i].dwStartTime = (*pRes)[6].GetDword();
		pYBOrder[i].dwEndTime =  (*pRes)[7].GetDword();
		pYBOrder[i].nAvgPrice = (*pRes)[8].GetInt();
		pYBOrder[i].nDealNum = (*pRes)[9].GetInt();

		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// 读取某个帐号的所有角色基本信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadSimRole(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("roledata", 
		"RoleName,RoleID,MapID,Level,"
		"HairModelID,HairColorID,FaceModelID,FaceDetailID,DressModelID,Sex,AvatarEquip,DisplaySet,PlayerBack,LogoutTime,PlayerBackDays");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
	pStream->FillString(" and RemoveFlag=") << 0;
}
//-------------------------------------------------------------------------------------------------------
// 读取某个帐号的所有角色基本信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadSimRole(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);
	
	MTRANS_POINTER(pSimRole, pData, tagSimRole);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_ROLENUM_ONEACCOUNT)
	{
		// 向监控发消息 //??

		ASSERT(*pNum <= MAX_ROLENUM_ONEACCOUNT);
		*pNum = MAX_ROLENUM_ONEACCOUNT;
	}

	// RoleName,RoleID,MapID,SafeCodeCrc,Level,
	// Status,HairModelID,HairColorID,FaceModelID,FaceDetailID,DressModelID
	for(INT i=0; i<*pNum; ++i)
	{
		(*pRes)[0].GetTCHAR(pSimRole[i].szRoleName, X_SHORT_NAME);
		pSimRole[i].dwRoleID = (*pRes)[1].GetDword();

		pSimRole[i].dwMapID = (*pRes)[2].GetDword();
		pSimRole[i].byLevel = (*pRes)[3].GetByte();

		pSimRole[i].Avatar.wHairMdlID = (*pRes)[4].GetShort();
		pSimRole[i].Avatar.wHairTexID = (*pRes)[5].GetShort();
		pSimRole[i].Avatar.wFaceMdlID = (*pRes)[6].GetShort();
		pSimRole[i].Avatar.wFaceDetailTexID = (*pRes)[7].GetShort();
		pSimRole[i].Avatar.wDressMdlID = (*pRes)[8].GetShort();
		pSimRole[i].Avatar.bySex = (BYTE)(*pRes)[9].GetShort();
		pSimRole[i].sDisplaySet = (*pRes)[11].GetByte();
		pSimRole[i].bOldPlayerBack = (*pRes)[12].GetByte();
		DataTime2DwordTime((tagDWORDTime&)pSimRole[i].timeLogout,	(*pRes)[13].GetString(), (*pRes)[13].GetLen());
		pSimRole[i].dwBackDays = (*pRes)[14].GetByte();

		ZeroMemory(&pSimRole[i].AvatarEquip, SIZE_AVATAR_EQUIP);
		if(!(*pRes)[10].IsNull())
		{
			//IMSG(_T("Role<id: %u> AvatarEquip size: %d\r\n"), pSimRole[i].dwRoleID, (*pRes)[10].GetLen());
			//ASSERT(SIZE_AVATAR_EQUIP == (*pRes)[9].GetLen());
			(*pRes)[10].GetBlob(&pSimRole[i].AvatarEquip, min((*pRes)[10].GetLen(), SIZE_AVATAR_EQUIP));
		}
		

		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// 读取角色属性信息SQL语句格式化//??0
// Jason 2010-1-0 v1.3.2 增加每日在线奖励相关字段
// Jason 2010-1-15 v1.3.2 增加离线挂机
// Jason 2010-1-30 v1.3.2宠物栏阀
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadRoleData(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{

	pStream->SetSelect("roledata left join guild_member b on roledata.roleid=b.roleid",	
									"RoleName,"						"RoleNameCrc,"							"DisplaySet,"					"Sex,"						"HairModelID,"
									"HairColorID,"					"FaceModelID,"							"FaceDetailID,"					"DressModelID,"				"MapID,"	
									"X,"							"Y,"									"Z,"							"FaceX,"					"FaceY,"	
									"FaceZ,"						"Class,"								"ClassEx,"						"Level,"					"ExpCurLevel,"	
									"HP,"							"MP,"									"AttPtAvail,"					"TalentPtAvail,"			"PhysiqueAdded,"	
									"StrengthAdded,"				"PneumaAdded,"							"InnerforceAdded,"				"TechniqueAdded,"			"AgilityAdded,"	
									"Injury,"						"Vitality,"								"Knowledge,"					"Morality,"					"Culture,"	
									"Credit,"						"Identity,"								"VIPPoint,"						"BagSize,"					"BagGold,"	
									"BagSilver,"					"BagYuanBao,"							"b.GuildID,"						"0,"						"0,"	
									"0,"							"CreateTime,"							"LoginTime,"					"LogoutTime,"				"OnlineTime,"	
									"CurOnlineTime,"				"Rage,"									"Endurance,"					"SGFlag,"					"CloseSGTime,"
									"TalentType1,"					"TalentType2,"							"TalentType3,"					"TalentType4,"				"TalentVal1,"
									"TalentVal2,"					"TalentVal3,"							"TalentVal4,"					"RebornMapID,"				"TotalTax,"	
									"Morale,"						"GetMallFreeTime,"						"RemoteOpenSet,"				"CurTitleID,"				"ExchangeVolume,"
									"ScriptData,"					"TreasureSum,"							"StallLevel,"					"StallDailyExp,"			"StallCurExp,"	
									"StallLastTime,"				"Hostility,"							"DeadUnSetSafeGuardCountDown,"	"ItemTransportMapID,"		"ItemTransportX,"	
									"ItemTransportZ,"				"ItemTransportY,"						"Strength," 					"StrengthNum,"				"LoverID,"			
									"HaveWedding,"					"LastLessingTime,"						"LastLessingLevel,"				"NeedPrisonRevive," 		"LevelPm,"			
									"EquipValue,"					"EquipPm,"								"LastLessingLoongDate,"			"CurrentDayOnlineTime,"		"OneDayFirstLoginedTime,"	
									"OnlineRewardPerDayRecTime,"	"OfflineExperienceRewardFlag,"			"Spirit,"						"FixSpirit,"				"ConsumptiveStrength,"
									"KillCount,"					"AdvanceStrength,"						"MasterID,"						"JingWuPoint,"				"QinWuPoint,"
									"FollowPetPocketValue,"			"RidingPetPocketValue,"	                "MiraclePrcNum,"				"MiracleResetTime,"			"SpeakOff,"
									"MARelation,"                   "WuXun,"								"WuJi,"							"LastLessingLevelEx,"		"GodMiraclePoints,"	
									"Weary,"                        "TrainStateTransfered"					",SoaringValue"					",SoaringSkillLearnTimes"	",Toughness"		
									",CompleteRefreshTime"			",IMRefreshTime"						",ConsolidateTimes"				",RoleState"				",VipLevel"
									",VipStartTime"					",FreeWorldTalkTimes"					",YaoJingValue"					",CanRankFlag"				",Buy50LvlItemFlag"
									",Buy60LvlItemFlag"				",Buy70LvlItemFlag"						",Buy80LvlItemFlag"				",YListCompleteRefreshTime"	",YListIMRefreshTime"
									",TakeGuildRewardTime"			",ExpPilularUseTimes"					",PickupModeSetting"				",VipMaxDays"			",PlayerBack"
									",PlayerBackDays"				",UseConstraintsMaxPKSafeGuardLevel"	",BePlayActLayerID"				",BePlayActLevel"			",GodHead"								
									",GodFaith"						",GodCondenced"							",ClergyMarsPoint"				",ClergyApolloPoint"		",ClergyRabbiPoint"						
									",ClergyPeacePoint"				",Clergy"								",PreClergy" 					",ClergyCandidateType"		",Clergy4SeniorOrJunior"
									",KeyCodeRewarded"				",FamilyID"								",LastUpgrade"						",TrainDate"			",TrainNum"
									",award_point"					",award_flag"							",BrotherTeacherID"				",BrotherEndTime"			",PulseRemainTime"
									",TigerTime"					",god_soul"								",monster_soul"					",god_point"				",monster_point"		
									",flower_num"					",egg_num"								",holy_value"					",role_hit_add"				",role_eei_all" );

	pStream->SetWhere();																		 
	pStream->FillString("roledata.RoleID=") << dwRoleID;
	pStream->FillString(" and AccountID=") << dwAccountID;
	pStream->FillString(" and RemoveFlag=") << 0;
}

//-------------------------------------------------------------------------------------------------------
// 读取角色属性信息结果检查及保存
// Jason 2010-1-0 v1.3.2 增加每日在线奖励相关字段
// Jason 2010-1-15 v1.3.2 增加离线挂机相关字段
// Jason 2010-1-30 v.1.3.2 当前宠物栏最大容量
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadRoleData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(NULL == pNum);
	
	MTRANS_POINTER(pRoleData, pData, tagRoleDataLoad);
	tagRoleDataConst	*pRoleDataConst = &pRoleData->RoleDataConst;
	tagRoleDataSave		*pRoleDataSave	= &pRoleData->RoleDataSave;

	if(pRes->GetRowCount() != 1)
	{
		// 向监控发消息//??

		ASSERT(pRes->GetRowCount() == 1);
	}

	(*pRes)[0].GetTCHAR(pRoleDataConst->szRoleName, X_SHORT_NAME);
	pRoleDataConst->dwRoleNameCrc				= (*pRes)[1].GetDword();
	pRoleDataConst->Avatar.bySex				= (*pRes)[3].GetByte();
	pRoleDataConst->Avatar.wHairMdlID			= (*pRes)[4].GetShort();
	pRoleDataConst->Avatar.wHairTexID			= (*pRes)[5].GetShort();
	pRoleDataConst->Avatar.wFaceMdlID			= (*pRes)[6].GetShort();
	pRoleDataConst->Avatar.wFaceDetailTexID		= (*pRes)[7].GetShort();
	pRoleDataConst->Avatar.wDressMdlID			= (*pRes)[8].GetShort();
	
	pRoleDataSave->DisplaySet					= (*pRes)[2].GetByte();
	pRoleDataSave->dwMapID						= (*pRes)[9].GetDword();
	pRoleDataSave->fCoordinate[0]				= (*pRes)[10].GetFloat();
	pRoleDataSave->fCoordinate[1]				= (*pRes)[11].GetFloat();
	pRoleDataSave->fCoordinate[2]				= (*pRes)[12].GetFloat();
	pRoleDataSave->fFace[0]						= (*pRes)[13].GetFloat();
	pRoleDataSave->fFace[1]						= (*pRes)[14].GetFloat();
	pRoleDataSave->fFace[2]						= (*pRes)[15].GetFloat();

	pRoleDataSave->eClass						= (EClassType)(*pRes)[16].GetInt();
	pRoleDataSave->eClassEx						= (EClassTypeEx)(*pRes)[17].GetInt();
	pRoleDataSave->nLevel						= (*pRes)[18].GetShort();
	pRoleDataSave->nCurExp						= (*pRes)[19].GetLong();
	pRoleDataSave->nHP							= (*pRes)[20].GetInt();
	pRoleDataSave->nMP							= (*pRes)[21].GetInt();

	pRoleDataSave->nAttPoint					= (*pRes)[22].GetInt();
	pRoleDataSave->nTalentPoint					= (*pRes)[23].GetInt();

	pRoleDataSave->nAttPointAdd[ERA_Physique]	= (*pRes)[24].GetInt();
	pRoleDataSave->nAttPointAdd[ERA_Strength]	= (*pRes)[25].GetInt();
	pRoleDataSave->nAttPointAdd[ERA_Pneuma]		= (*pRes)[26].GetInt();
	pRoleDataSave->nAttPointAdd[ERA_InnerForce]	= (*pRes)[27].GetInt();
	pRoleDataSave->nAttPointAdd[ERA_Technique]	= (*pRes)[28].GetInt();
	pRoleDataSave->nAttPointAdd[ERA_Agility]	= (*pRes)[29].GetInt();

	pRoleDataSave->nInjury						= (*pRes)[30].GetInt();
	pRoleDataSave->nVitality					= (*pRes)[31].GetInt();
	pRoleDataSave->nKnowledge					= (*pRes)[32].GetInt();
	pRoleDataSave->nMorality					= (*pRes)[33].GetInt();
	pRoleDataSave->nCulture						= (*pRes)[34].GetInt();
	pRoleDataSave->nCredit						= (*pRes)[35].GetInt();
	pRoleDataSave->nIdentity					= (*pRes)[36].GetInt();
	pRoleDataSave->nVIPPoint					= (*pRes)[37].GetInt();

	pRoleDataSave->n16BagSize					= (*pRes)[38].GetShort();
	pRoleDataSave->nBagGold						= (*pRes)[39].GetInt();
	pRoleDataSave->nBagSilver					= (*pRes)[40].GetInt();
	pRoleDataSave->nBagYuanBao					= (*pRes)[41].GetInt();

	pRoleDataSave->dwGuildID					= (*pRes)[42].GetDword();
	//pRoleDataSave->nWareGold					= (*pRes)[43].GetInt();
	//pRoleDataSave->nWareSilver					= (*pRes)[44].GetInt();
	//pRoleDataSave->nWareYuanBao					= (*pRes)[45].GetInt();

	if(!DataTime2DwordTime(pRoleDataConst->CreateTime,	(*pRes)[46].GetString(), (*pRes)[46].GetLen()))
	{
		IMSG(_T("Error: Func DataTime2DwordTime() run error in CLoongDB::FormatSaveRoleData()!!!!!\r\n"));
		IMSG(_T("Error: Role Name CRC32 is %u!!!!!\r\n"), pRoleDataConst->dwRoleNameCrc);
		ASSERT(0);
	}

	DataTime2DwordTime(pRoleDataSave->LoginTime,	(*pRes)[47].GetString(), (*pRes)[47].GetLen());
	DataTime2DwordTime(pRoleDataSave->LogoutTime,	(*pRes)[48].GetString(), (*pRes)[48].GetLen());

	pRoleDataSave->nOnlineTime					= (*pRes)[49].GetDword();
	pRoleDataSave->nCurOnlineTime				= (*pRes)[50].GetDword();

	pRoleDataSave->nRage						= (*pRes)[51].GetInt();
	pRoleDataSave->nEndurance					= (*pRes)[52].GetInt();
	pRoleDataSave->bSafeGuard					= (*pRes)[53].GetInt();

	//pRoleDataSave->CloseSafeGuardTime			= (*pRes)[54].GetDword();
	DataTime2DwordTime(pRoleDataSave->CloseSafeGuardTime,	(*pRes)[54].GetString(), (*pRes)[54].GetLen());

	pRoleDataSave->talent[0].eType				= (ETalentType)(*pRes)[55].GetInt();
	pRoleDataSave->talent[1].eType				= (ETalentType)(*pRes)[56].GetInt();
	//pRoleDataSave->talent[2].eType				= (ETalentType)(*pRes)[57].GetInt();
	//pRoleDataSave->talent[3].eType				= (ETalentType)(*pRes)[58].GetInt();
	pRoleDataSave->talent[0].nPoint				= (*pRes)[59].GetInt();
	pRoleDataSave->talent[1].nPoint				= (*pRes)[60].GetInt();
	//pRoleDataSave->talent[2].nPoint				= (*pRes)[61].GetInt();
	//pRoleDataSave->talent[3].nPoint				= (*pRes)[62].GetInt();

	pRoleDataSave->dwRebornMapID				= (*pRes)[63].GetDword();
	pRoleDataSave->nTotalTax					= (*pRes)[64].GetInt();
	pRoleDataSave->nMorale						= (*pRes)[65].GetInt();

	DataTime2DwordTime(*(tagDWORDTime*)&pRoleDataSave->dwTimeGetMallFree, (*pRes)[66].GetString(), (*pRes)[66].GetLen());
	pRoleDataSave->sRemoteOpenSet				= (*pRes)[67].GetDword();
	pRoleDataSave->u16ActiveTitleID				= (*pRes)[68].GetInt();
	pRoleDataSave->nExVolume					= (*pRes)[69].GetInt();
	(*pRes)[70].GetBlob(pRoleDataSave->Data.dwData, (*pRes)[70].GetLen());
	pRoleDataSave->nTreasureSum					= (*pRes)[71].GetInt();

	pRoleDataSave->byStallLevel					= (*pRes)[72].GetByte();
	pRoleDataSave->nStallDailyExp				= (*pRes)[73].GetInt();
	pRoleDataSave->nStallCurExp					= (*pRes)[74].GetInt();
	pRoleDataSave->dwLastStallTime				= (*pRes)[75].GetDword();
	pRoleDataSave->nHostility					= (*pRes)[76].GetInt();
	pRoleDataSave->nDeadUnSetSafeGuardCountDown = (*pRes)[77].GetInt();

	pRoleDataSave->dwItemTransportMapID			= (*pRes)[78].GetDword();
	pRoleDataSave->fItemTransportX			= (*pRes)[79].GetFloat();
	pRoleDataSave->fItemTransportZ			= (*pRes)[80].GetFloat();
	pRoleDataSave->fItemTransportY			= (*pRes)[81].GetFloat();

	pRoleDataSave->nStrength					= (*pRes)[82].GetInt();
	pRoleDataSave->nStrengthNum					= (*pRes)[83].GetInt();

	pRoleDataSave->dwLoverID					= (*pRes)[84].GetDword();
	pRoleDataSave->bHaveWedding					= (*pRes)[85].GetBool();

	// Jason 2009-12-7 神龙赐福
	pRoleDataSave->timeLastLessing					= (*pRes)[86].GetDword();
	pRoleDataSave->nLessingLevel					= (*pRes)[87].GetInt();
	pRoleDataSave->bNeedPrisonRevive				= (*pRes)[88].GetBool();

	pRoleDataSave->nLevelPm							= (*pRes)[89].GetInt();

	pRoleDataSave->nEquipValue						= (*pRes)[90].GetInt();
	pRoleDataSave->nEquipPm							= (*pRes)[91].GetInt();

	//===============================每日在线奖励============================================
	pRoleDataSave->dateLastLessingLoong				= (*pRes)[92].GetDword();
	pRoleDataSave->dwCurrentDayOnlineTime			= (*pRes)[93].GetDword();
	pRoleDataSave->timeOneDayFirstLogined			= (*pRes)[94].GetDword();
	pRoleDataSave->timeOnlineRewardPerDayRec		= (*pRes)[95].GetDword();
	//===============================每日在线奖励============================================


	//===============================离线挂机奖励============================================
	pRoleDataSave->bIsOpenOfflineReward				= (*pRes)[96].GetInt();
	//===============================离线挂机奖励============================================


	// 精力系统
	pRoleDataSave->nSpirit							= (*pRes)[97/*99*/].GetInt();
	pRoleDataSave->dwFixSpiritCD					= (*pRes)[98/*100*/].GetDword();

	pRoleDataSave->nConsumptiveStrength				= (*pRes)[99/*101*/].GetInt();
	pRoleDataSave->nKillScore						= (*pRes)[100].GetInt();
	pRoleDataSave->nAdvanceStrength					= (*pRes)[101].GetInt();

	// 师徒相关
	pRoleDataSave->dwMasterID						= (*pRes)[102].GetDword();
	pRoleDataSave->nJingWuPoint						= (*pRes)[103].GetDword();
	pRoleDataSave->nQinWuPoint						= (*pRes)[104].GetDword();
	// Jason 2010-1-30 v.1.3.2 当前宠物栏最大容量
	pRoleDataSave->u16FollowPetPocketValve			= (*pRes)[105].GetShort();
	pRoleDataSave->u16RidingPetPocketValve          = (*pRes)[106].GetShort();

	pRoleDataSave->n16MiraclePracticeNum			= (*pRes)[107].GetShort();
	pRoleDataSave->dwMiracleResetTime				= (*pRes)[108].GetDword();
	pRoleDataSave->bSpeakOff						= (*pRes)[109].GetBool();
	pRoleDataSave->nMARelation						= (*pRes)[110].GetInt();
	pRoleDataSave->nWunXun							= (*pRes)[111].GetInt();
	pRoleDataSave->nWunJi							= (*pRes)[112].GetInt();
	// Jason 2010-3-15 v1.5.0
	pRoleDataSave->nLessingLevelEx					= (*pRes)[113].GetInt();

	// Jason 2010-3-29 v1.5.1 神迹积分
	pRoleDataSave->nGodMiraclePoints				= (*pRes)[114].GetInt();
	pRoleDataSave->nWeary							= (*pRes)[115].GetInt();

	pRoleDataSave->byTrainStatesTransfered			= (*pRes)[116].GetByte();

	// Jason 2010-5-18 v2.1.2 飞升相关
	pRoleDataSave->nSoaringValue						= (*pRes)[117].GetInt();

	pRoleDataSave->nSoaringSkillLearnTimes				= (*pRes)[118].GetInt();
	pRoleDataSave->nToughness							= (*pRes)[119].GetInt();

	pRoleDataSave->nCompleteRefreshTime					= (*pRes)[120].GetInt();
	pRoleDataSave->nIMRefreshTime						= (*pRes)[121].GetInt();

	// Jason 2010-6-12 v1.0.0
	pRoleDataSave->nConsolidateTimes					= (*pRes)[122].GetInt();
	// Jason 2010-7-14 v1.0.0
	pRoleDataSave->dwRoleState						= (*pRes)[123].GetDword();

	pRoleDataSave->nVipLevel							= (*pRes)[124].GetInt();
	pRoleDataSave->dwVipStartTime					= (*pRes)[125].GetDword();
	pRoleDataSave->n16FreeWorldTalkTimes	= (*pRes)[126].GetShort();
	pRoleDataSave->nYaoJingValue					= (*pRes)[127].GetDword();
	pRoleDataSave->byCanRankFlag					= (*pRes)[128].GetInt();
	pRoleDataSave->byBuy50LvlItemFlag			= (*pRes)[129].GetInt();
	pRoleDataSave->byBuy60LvlItemFlag			= (*pRes)[130].GetInt();
	pRoleDataSave->byBuy70LvlItemFlag			= (*pRes)[131].GetInt();
	pRoleDataSave->byBuy80LvlItemFlag			= (*pRes)[132].GetInt();
	pRoleDataSave->nYellowListCompleteRefreshTime			= (*pRes)[133].GetInt();
	pRoleDataSave->nYellowListIMRefreshTime			= (*pRes)[134].GetInt();

	pRoleDataSave->timeTakeGuildReward			= (*pRes)[135].GetDword();

	pRoleDataSave->dwExpPilularUseTimes			= (*pRes)[136].GetShort();

	pRoleDataSave->dwPickupModeSetting			= (*pRes)[137].GetDword();

	pRoleDataSave->dwVipMaxDays				= (*pRes)[138].GetDword();
	pRoleDataSave->byPlayerBack					= (*pRes)[139].GetByte();
	pRoleDataSave->byPlayerBackDays				= (*pRes)[140].GetDword();

	pRoleDataSave->byUsingConstraintsPKSafeGuardLevel = (*pRes)[141].GetByte();
	pRoleDataSave->dwBePlayActLayerID			 = (*pRes)[142].GetDword();
	pRoleDataSave->byBePlayActLevel				 = (*pRes)[143].GetByte();
	pRoleDataSave->nGodhead						= (*pRes)[144].GetInt();
	pRoleDataSave->nGodFaith						= (*pRes)[145].GetInt();
	pRoleDataSave->nGodCondense					= (*pRes)[146].GetInt();

	// 神职相关
	pRoleDataSave->dwClergyMarsPoint			= (*pRes)[147].GetDword();
	pRoleDataSave->dwClergyApolloPoint			= (*pRes)[148].GetDword();
	pRoleDataSave->dwClergyRabbiPoint			= (*pRes)[149].GetDword();
	pRoleDataSave->dwClergyPeacePoint			= (*pRes)[150].GetDword();
	pRoleDataSave->dwClergy						= (*pRes)[151].GetDword();
	pRoleDataSave->dwPreClergy					= (*pRes)[152].GetDword();
	pRoleDataSave->eClergyCandidate				= (ECampaignGodType)(*pRes)[153].GetInt();
	pRoleDataSave->eSeniorOrJunior				= (ECandidateFlag)(*pRes)[154].GetByte();

	pRoleDataSave->dwKeyCodeRewarded			= (*pRes)[155].GetDword();
	pRoleDataSave->dwFamilyID					= (*pRes)[156].GetDword();

	pRoleDataSave->dwLastUpgrade				= (*pRes)[157].GetDword();

	pRoleDataSave->dwTrainDate					= (*pRes)[158].GetDword();

	pRoleDataSave->nTrainNum					= (*pRes)[159].GetDword();

	pRoleDataSave->nAwardPoint					= (*pRes)[160].GetInt();

	pRoleDataSave->nAwardFlag					= (*pRes)[161].GetInt();

	pRoleDataSave->dwBrotherTeatherID			= (*pRes)[162].GetDword();

	pRoleDataSave->timeBloodBrother				= (*pRes)[163].GetDword();

	pRoleDataSave->nPulseLearnRemainTime		= (*pRes)[164].GetInt();

	pRoleDataSave->nTigerTime					= (*pRes)[165].GetInt();

	pRoleDataSave->nGod							= (*pRes)[166].GetInt();		//神魂
	pRoleDataSave->nMonster						= (*pRes)[167].GetInt();		//魔魂
	pRoleDataSave->nGodPoint					= (*pRes)[168].GetInt();		//神魂声望
	pRoleDataSave->nMonsterPoint				= (*pRes)[169].GetInt();		//魔魂声望

	//添加鲜花鸡蛋属性
	pRoleDataSave->nFlower						= (*pRes)[170].GetInt();		//鲜花
	pRoleDataSave->nEgg							= (*pRes)[171].GetInt();		//鸡蛋
	pRoleDataSave->nHolyValue					= (*pRes)[172].GetInt();		//圣灵值
	pRoleDataSave->nHidAdd						= (*pRes)[173].GetInt();		//命中
	pRoleDataSave->nEEI_All						= (*pRes)[174].GetInt();		//元素全伤

	// 该字段在读取角色具体信息时不用读取
	//ZeroMemory(&pRoleDataSave->DisplaySet, sizeof(tagDisplaySet));
	//if(!(*pRes)[2].IsNull())
	//{
	//	(*pRes)[2].GetBlob(&pRoleDataSave->DisplaySet, (*pRes)[2].GetLen());
	//}
}

//-------------------------------------------------------------------------------------------------------
// 读取角色技能信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadSkill(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("skill", "ID,BiddenLevel,SelfLevel,CoolDown,Proficiency,active_time");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取角色任务板任务的SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadBoardQuest(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("quest_board", "RoleID,QuestID,BoardID,State,BoardType");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

VOID CLoongDB::FormatLoadEggInfo(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("egg_broker", "CurLayer,ColourEggPos1,ColourEggPos2,BrokePos1,ItemID1,BrokePos2,ItemID2,BrokePos3,ItemID3,BrokePos4,ItemID4,BrokePos5,ItemID5,BrokePos6,ItemID6,BrokePos7,ItemID7");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}
VOID CLoongDB::FormatLoadWardrobeLayerInfo(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("wardrobe", "RoleID,LayerID,Level,State,PlayActStartTime,PlayActFriendID");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

VOID CLoongDB::FormatLoadCurMonthOnlineTime(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
 	tagDWORDTime dwCurTime = GetCurrentDWORDTime();
	pStream->SetSelect("role_online_time_each_month", "January, February,March,April,May,June,July,August,September,October,November,December");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and AccountID=") << dwAccountID;
	pStream->FillString(" and Year=") << dwCurTime.year;
}
VOID CLoongDB::ProcResLoadWardrobeLayerInfo(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);
	MTRANS_POINTER(pWardrobeLayerInfo, pData, tagWardrobeLayer2DB);
	*pNum = pRes->GetRowCount();
	for(INT32 i=0; i<*pNum; ++i)
	{
		pWardrobeLayerInfo[i].dwRoleID = (*pRes)[0].GetDword();
		pWardrobeLayerInfo[i].dwLayerID = (*pRes)[1].GetDword();
		pWardrobeLayerInfo[i].byLevel = (BYTE)((*pRes)[2].GetDword());
		pWardrobeLayerInfo[i].byState = (BYTE)((*pRes)[3].GetDword());
		pWardrobeLayerInfo[i].dwPlayActStartTime = (*pRes)[4].GetDword();
		pWardrobeLayerInfo[i].dwFriendID = (*pRes)[5].GetDword();
		pRes->NextRow();
	}
	pData = &pWardrobeLayerInfo[*pNum];
}

VOID CLoongDB::ProcResLoadEggInfo(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);
	MTRANS_POINTER(pEggInfoInDB, pData, tagEggInfoInDB);
	*pNum = pRes->GetRowCount();
	pEggInfoInDB->nCurLayer = (*pRes)[0].GetInt();
	pEggInfoInDB->n16ColourEggPos1 = (*pRes)[1].GetInt();
	pEggInfoInDB->n16ColourEggPos2 = (*pRes)[2].GetInt();
	for (INT nCnt = 0; nCnt < MAX_EGG_LAYER; nCnt++)
	{
		pEggInfoInDB->sBrokenEggInfo[nCnt].nBrokePos = (*pRes)[(3+2*nCnt)].GetDword();
		pEggInfoInDB->sBrokenEggInfo[nCnt].dwItemID = (*pRes)[(4+2*nCnt)].GetDword();
	}
	pData = &pEggInfoInDB[*pNum];
}

VOID CLoongDB::ProcResLoadCurMonthOnlineTime(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pOnlineTime, pData, tagMonthOnlineTime);
	*pNum = pRes->GetRowCount();

	for (int nMonthIdx = EM_January; nMonthIdx < EM_End; nMonthIdx++)
	{
		pOnlineTime->n16OnlineTime[nMonthIdx] = static_cast<UINT16>((*pRes)[nMonthIdx].GetDword());
	}
	pOnlineTime->dwCurTime = 0;

	pData = &pOnlineTime[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 读取角色技能信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadSkill(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);
	
	MTRANS_POINTER(pSkill, pData, tagSkillSave);
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		pSkill[i].dwID			= (*pRes)[0].GetDword();
		pSkill[i].nSelfLevel	= (*pRes)[2].GetInt();
		pSkill[i].nLearnLevel	= (*pRes)[1].GetInt();
		pSkill[i].nCoolDown	    = (*pRes)[3].GetInt();
		pSkill[i].nProficiency	= (*pRes)[4].GetInt();
		pSkill[i].dwActiveTime	= (*pRes)[5].GetDword();
		

		pRes->NextRow();
	}

	pData = &pSkill[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 读取角色任务板任务信息结果
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadBoardQuest(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, tagBoardQuestSaveData);
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		p[i].dwRoleID			= (*pRes)[0].GetDword();
		p[i].u16QuestID			= (UINT16)(*pRes)[1].GetInt();
		p[i].u16BoardID			= (UINT16)(*pRes)[2].GetInt();
		p[i].eState				= (EBoardQuestState)(*pRes)[3].GetInt();
		p[i].byType				= (BYTE)(*pRes)[4].GetInt();
		
		pRes->NextRow();
	}

	pData = &p[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 读取角色buff信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadRoleBuff(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("buff", 
		"SrcUnitID,"		"SrcSkillID,"			"ItemTypeID,"		"ItemSerialID,"		"BuffID,"
		"CurTick,"			"Level,"				"CurLapTimes,"		"EffectSkillID");

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;


}

//-------------------------------------------------------------------------------------------------------
// 读取角色buff信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadRoleBuff(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pCurPointer, pData, BYTE);
	tagBuffSave *pRoleSave;
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		pRoleSave = (tagBuffSave*)pCurPointer;

		pRoleSave->dwSrcUnitID			= (*pRes)[0].GetDword();
		pRoleSave->dwSrcSkillID			= (*pRes)[1].GetDword();
		pRoleSave->dwItemTypeID			= (*pRes)[2].GetDword();
		pRoleSave->n64Serial			= (*pRes)[3].GetLong();

		pRoleSave->dwBuffID				= (*pRes)[4].GetDword();
		pRoleSave->nPersistTick			= (*pRes)[5].GetInt();
		pRoleSave->n8Level				= (INT8)(*pRes)[6].GetInt();
		pRoleSave->n8CurLapTimes		= (INT8)(*pRes)[7].GetInt();


		pRoleSave->n8ModifierNum		= (INT8)((*pRes)[8].GetLen()/sizeof(DWORD));

		pCurPointer = pRoleSave->byData;
		if(pRoleSave->n8ModifierNum > 0)
		{
			(*pRes)[8].GetBlob(pCurPointer, (*pRes)[8].GetLen());
			pCurPointer += (*pRes)[8].GetLen();
		}

		pRes->NextRow();
	}

	pData = pCurPointer;
}

//-------------------------------------------------------------------------------------------------------
// 读取称号信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadTitle(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("title", "ID,Count,TimeRemain");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取称号信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadTitle(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	*pNum = pRes->GetRowCount();

	MTRANS_POINTER(p, pData, tagTitleSave);
	for(INT32 i=0; i<*pNum; ++i)
	{
		p[i].u16TitleID		= (UINT16)(*pRes)[0].GetDword();
		p[i].dwStateMark	= (*pRes)[1].GetDword();
		p[i].dwTimeRemain	= (*pRes)[2].GetDword();

		pRes->NextRow();
	}

	pData = &p[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 初始化物品读物字段//??0
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::InitItemLoad(Beton::MyStream *pStream)
{
	// 注意下面的表字段，都是五个一行的，这样好计数
	pStream->FillString(
		"SerialNum,"	"Num,"				"TypeID,"		"Bind,"				"LockStat,"		
		"UseTimes,"		"FirstGainTime,"	"CreatorID,"	"CreateTime,"		"OwnerID,"
		"AccountID,"	"ContainerTypeID,"	"Suffix,"		"Quality,"			"PotValUsed,"
		"MinUseLevel,"	"MaxUseLevel,"		"WuHun,"		"MinDmg,"			"MaxDmg,"		
		"Armor,"		"PotVal,"			"RoleAttEffect,""PosyTimes,"		"PosyEffect,"
		"EngraveTimes,"	"EngraveAtt,"		"HoleNum,"		"HoleGemID,"		"BrandLevel,"
		"DerateVal,"	"LongFuLevel,"		"WuXing,"		"CanCut,"			"LongInnerID,"
		"LongOuterID,"	"0,"				"0,"			"SpecAtt,"			"Appearance,"
		"Rein,"			"Savvy,"			"Fortune,"		"ColorID,"			"QltyModPct,"
		"QltyModPctEx,"	"CreateMode,"		"CreateID,"		"AttALimMod,"		"AttALimModPct,"
		"FlareVal,"		"NameID,"			"PotValModPct,"	"PotIncTimes,"		"PurpleQltyIdfPct,"
		"UnlockTime,"	"Signed,"			"Signature,"	"SignRoleID"		",SoaringFlag"

		",CastingTimes"			",RoleAtt2EffectCount"		",ConsolidateLevel"		",ConsolidateAttEffect"		",LastGodStrengthenLevel"
		",GodStrengthenLevel"	",CurGodStrengthenExp"		",GodStrengthenTime"	",ItemSpecVal1"				",ItemSpecVal2"
		",GrowID"				",GrowValue"				",GrowLevel"			",GrowTimes"				",GrowUseIMTimes"
		",GrowStars"			",GrowAtt"					",DiamondLevel"			",byStarEnhanceLvl"			",byHolyHoleNum");
}

VOID CLoongDB::InitSoulCrystal(Beton::MyStream *pStream)
{
	//                                 78              79            80              81                82               83         84
	pStream->FillString(",SoulLevel, SoulQlty, SoulSkillID, SoulAttID1, SoulAttID2, SoulAttID3, SoulAttID4,"
	//         85               86               87            88
		"SoulAttID5, SoulAttID6, SoulAttID7, SoulAttID8");
}

VOID CLoongDB::InitSoulCrystalUpdate(BOOL bInsertOrNot,Beton::MyStream *pStream,Beton::Connection* pCon,const tagSoulCrystal * pSoul)
{
	if(bInsertOrNot)
	{
		pStream->FillString  ("REPLACE INTO soulcrystal SET SerialNum=") << pSoul->n64Serial;
		pStream->FillString(",");
	}
	else
		pStream->FillString  ("UPDATE soulcrystal SET ") ;

	pStream->FillString("SoulLevel") << pSoul->SoulSpec.bySoulCrystalLvl; 
	pStream->FillString(",SoulQlty") << pSoul->SoulSpec.bySoulCrystalQlty;
	pStream->FillString(",SoulSkillID") << pSoul->SoulSpec.dwStaticSkillID;

	for (int i=0; i<SOUL_CRYSTAL_ATT_MAX_NUM; ++i)
	{
		TCHAR szTmp[X_SHORT_STRING] = {0};
		_sntprintf(szTmp, X_SHORT_STRING, _T(",SoulAttID%d"), i+1);
		pStream->FillString(szTmp)<<pSoul->SoulSpec.nSoulAttID[i];
	}
}
// 装备部分修改字段，统一在这里添加或删除，另外，一定要严格按5列方式对齐
VOID CLoongDB::InitFabaoLoad(Beton::MyStream *pStream,BOOL bBaibao)
{
	// Jason 2010-4-14 v2.0.0
	//                                           89                          90
	pStream->FillString(",NativeIntelligence,			Stage,					PracticeValue,				AttTypes,			AttGrowings,"
							"WuxingType,				WuxingHoles,			WuxingBeadIDs,				SlotAttTypes,		SlotAttValue,"
							"UseLevel,					SlotNum,				AttGrowings0,				SlotLingzhuMatch,	SlotMacthNdx,"
							"Mood,						MoodTicks,				MoodBuffID,					EleInjuryType,		EleInjury,"
							"EleResisFir,				EleResisWater,			EleResisEarth,				EleResisWind,		EleResisBright,"
							"EleResisDark,				DamageUpgradeTimes,		ResistanceUpgradeTimes,		FairySkill1,		FairySkill2,"
							"FairySkill3,				FairySkill4,			FairySkill5,				UnsureSkill,			StoredExpOpen,"
							"CurStoredExp,				FabaoStatus,			StatusChangeTime,			BirthValue,			BirthTimes,"
							"LevelBirth,				IsGivenBirthTimes,		MaxIntelligence,			ElePierce,			EleInjPromoteTimes,"
							"EleResPromoteTimes,"
							// wcy 圣灵
							"DevourNum,					EquipmentNumber,		CoValue,					ToDayDevourNum,		EquipSerialIDs,"
							"holyman.CostHoly,			HolyDmg,				HolyDef,					Crit,				HolyCritRate,"		
							"ExDamage,					AttackTec,				NeglectToughness,			HolyValue,			MaxDevourNum,"

							// wcy 圣纹
							"holyequip.CostHoly,		HolyDmgChg,				HolyDefChg,					CritChg,				HolyCritRateChg,"
							"ExDamageChg,				AttackTecChg,			NeglectToughnessChg,		EnhanceCount"							
							);
	if( !bBaibao )
		pStream->FillString(" from item left join equip using(SerialNum)");
	else
		pStream->FillString(" from item_baibao left join equip_baibao using(SerialNum)");

	// wcy 圣纹
	pStream->FillString(" left join holyequip using(SerialNum)");
	// wcy 圣灵
	pStream->FillString(" left join holyman using(SerialNum)");
	// Jason 2010-4-14 v2.0.0
	pStream->FillString("  left join fabao using(SerialNum) ");
	pStream->FillString(" left join SoulCrystal using(SerialNum) ");
}

//-------------------------------------------------------------------------------------------------------
// 读取角色物品信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadItem(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->FillString("select ");
	InitItemLoad(pStream);
	InitSoulCrystal(pStream);
	InitFabaoLoad(pStream);

	pStream->SetWhere();
	pStream->FillString("(OwnerID=") << dwRoleID;
	pStream->FillString(" or (AccountID=") << dwAccountID;
	pStream->FillString(" and (ContainerTypeID=") << EICT_Baibao;
	pStream->FillString(" or ContainerTypeID=") << EICT_RoleWare;
	pStream->FillString(")))");
	
	//IsLoadShengLing(pStream);
}

//-------------------------------------------------------------------------------------------------------
// 读取角色物品信息结果检查及保存//??
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadItem(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	*pNum = pRes->GetRowCount();

	INT nFabaoFNdx = 0;

	for(INT32 i=0; i<*pNum; ++i)
	{
		INT32 * pLen = (INT32*)pData;
		pData = pLen + 1;
		MTRANS_POINTER(pItem, pData, tagItem);

		pItem->n64Serial 					= (*pRes)[0].GetLong();
		pItem->n16Num 						= (*pRes)[1].GetShort();
		pItem->dwTypeID 					= (*pRes)[2].GetDword();
		pItem->byBind 						= (*pRes)[3].GetInt();
		pItem->bLock 						= (*pRes)[4].GetBool();

		pItem->nUseTimes 					= (*pRes)[5].GetInt();
		pItem->dwCreatorID 					= (*pRes)[7].GetDword();
		pItem->dwOwnerID 					= (*pRes)[9].GetDword();
		pItem->dwAccountID 					= (*pRes)[10].GetDword();

		pItem->eConType 					= (EItemConType)(*pRes)[11].GetInt();
		pItem->n16Index 					= (*pRes)[12].GetShort();
		pItem->eCreateMode					= (EItemCreateMode)(*pRes)[46].GetInt();
		pItem->dwCreateID					= (*pRes)[47].GetDword();

		pItem->dwNameID						= (*pRes)[51].GetDword();
		pItem->dwUnlockTime					= (*pRes)[55].GetDword();
		pItem->dwSpecVal1	= (*pRes)[68].GetDword();
		pItem->dwSpecVal2	= (*pRes)[69].GetDword();

		if(!DataTime2DwordTime(pItem->dw1stGainTime, (*pRes)[6].GetString(), (*pRes)[6].GetLen()))
		{
			// 数据库中记录日期的字符串格式错误.应形如: 2008-11-03 11:47:00
			IMSG(_T("Error: Func DataTime2DwordTime() run error in CLoongDB::ProcResLoadItem()!!!!!\r\n"));
			IMSG(_T("Error: Item Serial Number is %lld!!!!!\r\n"), pItem->n64Serial);
			ASSERT(0);
		}

		if(!DataTime2DwordTime(pItem->dwCreateTime, (*pRes)[8].GetString(), (*pRes)[8].GetLen()))
		{
			// 数据库中记录日期的字符串格式错误.应形如: 2008-11-03 11:47:00
			IMSG(_T("Error: Func DataTime2DwordTime() run error in CLoongDB::ProcResLoadItem()!!!!!\r\n"));
			IMSG(_T("Error: Item Serial Number is %lld!!!!!\r\n"), pItem->n64Serial);
			ASSERT(0);
		}
		
		if(!MIsEquipment(pItem->dwTypeID))
		{
			*pLen = sizeof(tagItem);
			pData = &pItem[1];
			pRes->NextRow();
			continue;
		}

		MTRANS_POINTER(pEquip, pData, tagEquip);

		//------------------------------------------------------------------------------------
		// 装备属性
		//------------------------------------------------------------------------------------
		pEquip->equipSpec.byQuality 				= (*pRes)[13].GetByte();
		pEquip->equipSpec.nPotValUsed 				= (*pRes)[14].GetInt();
		pEquip->equipSpec.byMinUseLevel 			= (*pRes)[15].GetByte();
		pEquip->equipSpec.byMaxUseLevel 			= (*pRes)[16].GetByte();
		pEquip->equipSpec.n16WuHun 					= (*pRes)[17].GetShort();
		pEquip->equipSpec.n16MinDmg 				= (*pRes)[18].GetShort();
		pEquip->equipSpec.n16MaxDmg 				= (*pRes)[19].GetShort();
		pEquip->equipSpec.n16Armor 					= (*pRes)[20].GetShort();
		pEquip->equipSpec.nPotVal 					= (*pRes)[21].GetInt();
	
		pEquip->equipSpec.byPosyTimes 				= (*pRes)[23].GetByte();
		
		pEquip->equipSpec.byEngraveTimes 			= (*pRes)[25].GetByte();
		
		pEquip->equipSpec.byHoleNum 				= (*pRes)[27].GetByte();
		
		pEquip->equipSpec.byBrandLevel 				= (*pRes)[29].GetByte();
		
		pEquip->equipSpec.byLongfuLevel				= (*pRes)[31].GetByte();

		pEquip->equipSpec.bCanCut 					= (*pRes)[33].GetBool();
		pEquip->equipSpec.dwLongHunInnerID			= (*pRes)[34].GetDword();
		pEquip->equipSpec.dwLongHunOuterID			= (*pRes)[35].GetDword();
		//pEquip->equipSpec.dwLongHunOuterTriggerID	= (*pRes)[36].GetDword();
		//pEquip->equipSpec.dwLongHunOuterBuffID		= (*pRes)[37].GetDword();
		pEquip->equipSpec.bySpecAtt					= (*pRes)[38].GetInt();

		// 时装属性
		pEquip->equipSpec.n16Appearance				= (*pRes)[39].GetInt();
		pEquip->equipSpec.byRein					= (*pRes)[40].GetByte();
		pEquip->equipSpec.bySavvy					= (*pRes)[41].GetByte();
		pEquip->equipSpec.byFortune					= (*pRes)[42].GetByte();
		pEquip->equipSpec.n8ColorID					= (INT8)(*pRes)[43].GetInt();

		pEquip->equipSpec.n16QltyModPct				= (*pRes)[44].GetShort();
		pEquip->equipSpec.n16QltyModPctEx			= (*pRes)[45].GetShort();
		pEquip->equipSpec.n16PotValModPct			= (*pRes)[52].GetShort();
		// Jason 2009-12-1 装备潜力值提升次数影响
		pEquip->equipSpec.nPotIncTimes				= (*pRes)[53].GetShort();
		// Jason 2009-12-2 增加装备紫色鉴定几率，默认0
		pEquip->equipSpec.dwPurpleQltyIdfPct		= (*pRes)[54].GetDword();
		
		pEquip->equipSpec.n16AttALimMod				= (*pRes)[48].GetInt();
		pEquip->equipSpec.n16AttALimModPct			= (*pRes)[49].GetInt();
		pEquip->equipSpec.byFlareVal				= (*pRes)[50].GetByte();

		pEquip->equipSpec.bySigned					= (*pRes)[56].GetInt();
		(*pRes)[57].GetTCHAR(pEquip->equipSpec.szSignature, 10);
		pEquip->equipSpec.dwSignatruePlayerID		= (*pRes)[58].GetDword();
		pEquip->equipSpec.bSoaringFlag				= (*pRes)[59].GetDword();
		pEquip->equipSpec.nCastingTimes				= (*pRes)[60].GetDword();
		// Jason 2010-6-11
		pEquip->equipSpec.nRoleL2AttCount				= (*pRes)[61].GetInt();
		pEquip->equipSpec.byConsolidateLevel				= (*pRes)[62].GetInt();
		
		(*pRes)[22].GetBlob(pEquip->equipSpec.nRoleAttEffect, min((*pRes)[22].GetLen(), sizeof(pEquip->equipSpec.nRoleAttEffect)));
		(*pRes)[24].GetBlob(pEquip->equipSpec.PosyEffect, min((*pRes)[24].GetLen(), sizeof(pEquip->equipSpec.PosyEffect)));
		(*pRes)[26].GetBlob(pEquip->equipSpec.nEngraveAtt, min((*pRes)[26].GetLen(), sizeof(pEquip->equipSpec.nEngraveAtt)));
		(*pRes)[28].GetBlob(pEquip->equipSpec.dwHoleGemID, min((*pRes)[28].GetLen(), sizeof(pEquip->equipSpec.dwHoleGemID)));
		(*pRes)[30].GetBlob(pEquip->equipSpec.nBrandAtt, min((*pRes)[30].GetLen(), sizeof(pEquip->equipSpec.nBrandAtt)));
		(*pRes)[32].GetBlob(pEquip->equipSpec.byLongfuAtt, min((*pRes)[32].GetLen(), sizeof(pEquip->equipSpec.byLongfuAtt)));
		// Jason 2010-6-19 v1.0.0强化属性影响
		(*pRes)[63].GetBlob( pEquip->equipSpec.ConsolidateEffect,min((*pRes)[63].GetLen(), sizeof(pEquip->equipSpec.ConsolidateEffect)) );

		pEquip->equipSpec.byLastGodStrengthenLevel	= (*pRes)[64].GetInt();
		pEquip->equipSpec.byGodStrengthenLevel	= (*pRes)[65].GetInt();
		pEquip->equipSpec.dwCurGodStrengthenExp	= (*pRes)[66].GetDword();
		pEquip->equipSpec.dwGodStrengthenTime	= (*pRes)[67].GetDword();

		pEquip->equipSpec.dwGrowTypeID	= (*pRes)[70].GetDword();
		pEquip->equipSpec.dwGrowValue	= (*pRes)[71].GetDword();
		pEquip->equipSpec.byGrowLevel	= (BYTE)(*pRes)[72].GetInt();
		pEquip->equipSpec.nTotalGrowTimes	= (*pRes)[73].GetInt();
		pEquip->equipSpec.nTotalIMUseTimes	= (*pRes)[74].GetInt();
		(*pRes)[75].GetBlob( pEquip->equipSpec.eGrowStar, min((*pRes)[75].GetLen(), sizeof(pEquip->equipSpec.eGrowStar)) );
		(*pRes)[76].GetBlob( pEquip->equipSpec.nGrowAttValue, min((*pRes)[76].GetLen(), sizeof(pEquip->equipSpec.nGrowAttValue)) );

		pEquip->equipSpec.byDiamondLevel	= (BYTE)(*pRes)[77].GetInt();
		pEquip->equipSpec.byStarEnhanceLv	= (*pRes)[78].GetInt();
		pEquip->equipSpec.byHolyHoleNum		= (*pRes)[79].GetByte();


		// 注意前面有下面这两天语句，索引变了
		//pItem->dwSpecVal1	= (*pRes)[68].GetDword();
		//pItem->dwSpecVal2	= (*pRes)[69].GetDword();
		
		//-----------------------------------------------------------------------------------------
		// 魂晶
		//-----------------------------------------------------------------------------------------
		INT nSoulIdx = SoulCrystal_Begin_Index;
		if( (*pRes)[nSoulIdx+1].IsNull() )
		{
			
		}
		else
		{
			MTRANS_POINTER(pSoulCrystal, pData, tagSoulCrystal);
			pSoulCrystal->SoulSpec.bySoulCrystalLvl	= (*pRes)[nSoulIdx].GetByte();
			pSoulCrystal->SoulSpec.bySoulCrystalQlty	= (*pRes)[nSoulIdx+1].GetByte();
			pSoulCrystal->SoulSpec.dwStaticSkillID	= (*pRes)[nSoulIdx+2].GetDword();
			pSoulCrystal->SoulSpec.nSoulAttID[0]	= (*pRes)[nSoulIdx+3].GetInt();
			pSoulCrystal->SoulSpec.nSoulAttID[1]	= (*pRes)[nSoulIdx+4].GetInt();
			pSoulCrystal->SoulSpec.nSoulAttID[2]	= (*pRes)[nSoulIdx+5].GetInt();
			pSoulCrystal->SoulSpec.nSoulAttID[3]	= (*pRes)[nSoulIdx+6].GetInt();
			pSoulCrystal->SoulSpec.nSoulAttID[4]	= (*pRes)[nSoulIdx+7].GetInt();
			pSoulCrystal->SoulSpec.nSoulAttID[5]	= (*pRes)[nSoulIdx+8].GetInt();
			pSoulCrystal->SoulSpec.nSoulAttID[6]	= (*pRes)[nSoulIdx+9].GetInt();
			pSoulCrystal->SoulSpec.nSoulAttID[7]	= (*pRes)[nSoulIdx+10].GetInt();

			*pLen = sizeof(tagSoulCrystal);
			pData = &pSoulCrystal[1];
			pRes->NextRow();
			continue;
		}

		

		//-----------------------------------------------------------------------------------------
		// wcy 2012-04-18 圣纹相关修改
		//-----------------------------------------------------------------------------------------
		INT nHolyEquipFNdx = HolyEquip_Begin_Index;

		if( (*pRes)[nHolyEquipFNdx].IsNull() )
		{
			// 			*pLen = sizeof(tagEquip);
			// 			pData = &pEquip[1];
			// 			pRes->NextRow();
		}
		else
		{
			// 圣纹相关
			MTRANS_POINTER(pHolyEquip, pData, tagHolyEquip);
			pHolyEquip->nCostHoly	= (*pRes)[nHolyEquipFNdx].GetInt();			
			pHolyEquip->nHolyAttEffect[ESAT_Demage]	= (*pRes)[nHolyEquipFNdx+1].GetInt();
			pHolyEquip->nHolyAttEffect[ESAT_HolyDef]	= (*pRes)[nHolyEquipFNdx+2].GetInt();
			pHolyEquip->nHolyAttEffect[ESAT_Crit]	= (*pRes)[nHolyEquipFNdx+3].GetInt();
			pHolyEquip->nHolyAttEffect[ESAT_CritRate]	= (*pRes)[nHolyEquipFNdx+4].GetInt();
			pHolyEquip->nHolyAttEffect[ESAT_ExDmg]	= (*pRes)[nHolyEquipFNdx+5].GetInt();
			pHolyEquip->nHolyAttEffect[ESAT_AttackTec]	= (*pRes)[nHolyEquipFNdx+6].GetInt();
			pHolyEquip->nHolyAttEffect[ESAT_NeglectToughness]	= (*pRes)[nHolyEquipFNdx+7].GetInt();
			pHolyEquip->n16EnhanceCount = (*pRes)[nHolyEquipFNdx+8].GetInt();	

			*pLen = sizeof(tagHolyEquip);
			pData = &pHolyEquip[1];
			pRes->NextRow();
			continue;
		}

		//-----------------------------------------------------------------------------------------
		// wcy 2012-04-01 圣灵相关修改
		//-----------------------------------------------------------------------------------------
		INT nHolyFNdx = Holy_Begin_Index;

		if( (*pRes)[nHolyFNdx].IsNull() )
		{
// 			*pLen = sizeof(tagEquip);
// 			pData = &pEquip[1];
// 			pRes->NextRow();
		}
		else
		{
			// 圣灵相关
			MTRANS_POINTER(pHoly, pData, tagHolyMan);
			pHoly->nDevourNum	= (*pRes)[nHolyFNdx].GetInt();
			pHoly->n16EquipmentNumber	= (*pRes)[nHolyFNdx+1].GetInt();
			pHoly->nCoValue	= (*pRes)[nHolyFNdx+2].GetInt();
			pHoly->nToDayEatNum = (*pRes)[nHolyFNdx+3].GetInt();
			(*pRes)[nHolyFNdx+4].GetBlob(pHoly->n64EquipSerialIDs,min((*pRes)[nHolyFNdx+4].GetLen(), sizeof(pHoly->n64EquipSerialIDs)));
			pHoly->nCostHoly = (*pRes)[nHolyFNdx+5].GetInt();
			pHoly->dwLevelUpAtt[ESAT_Demage]	= (*pRes)[nHolyFNdx+6].GetDword();
			pHoly->dwLevelUpAtt[ESAT_HolyDef]	= (*pRes)[nHolyFNdx+7].GetDword();
			pHoly->dwLevelUpAtt[ESAT_Crit]	= (*pRes)[nHolyFNdx+8].GetDword();
			pHoly->dwLevelUpAtt[ESAT_CritRate]	= (*pRes)[nHolyFNdx+9].GetDword();
			pHoly->dwLevelUpAtt[ESAT_ExDmg]	= (*pRes)[nHolyFNdx+10].GetDword();
			pHoly->dwLevelUpAtt[ESAT_AttackTec]	= (*pRes)[nHolyFNdx+11].GetDword();
			pHoly->dwLevelUpAtt[ESAT_NeglectToughness]	= (*pRes)[nHolyFNdx+12].GetDword();
			pHoly->dwLevelUpAtt[ESAT_LingNeng]	= (*pRes)[nHolyFNdx+13].GetDword();
			pHoly->nMaxDevourNum = (*pRes)[nHolyFNdx+14].GetInt();

			*pLen = sizeof(tagHolyMan);
			pData = &pHoly[1];
			pRes->NextRow();
			continue;
		}
		
		//-----------------------------------------------------------------------------------------
		// Jason 2010-4-14 v2.0.0 法宝相关修改
		//-----------------------------------------------------------------------------------------
		INT nFabaoFNdx = Fabao_Begin_Index;

		if( (*pRes)[nFabaoFNdx].IsNull() )
		{
 			*pLen = sizeof(tagEquip);
 			pData = &pEquip[1];
 			pRes->NextRow();
		}
		else
		{
			// 法宝相关
			MTRANS_POINTER(pFabao, pData, tagFabao);
			pFabao->n16NativeIntelligence	= (*pRes)[nFabaoFNdx].GetInt();
			pFabao->n16Stage				= (*pRes)[nFabaoFNdx+1].GetInt();
			pFabao->nPracticeValue			= (*pRes)[nFabaoFNdx+2].GetInt();
			(*pRes)[nFabaoFNdx+3].GetBlob(pFabao->eAttType,min((*pRes)[nFabaoFNdx+3].GetLen(), sizeof(pFabao->eAttType)));
			(*pRes)[nFabaoFNdx+4].GetBlob(pFabao->nAttGrowing,min((*pRes)[nFabaoFNdx+4].GetLen(), sizeof(pFabao->nAttGrowing)));
			pFabao->eWuxing					= (EWuXing)(*pRes)[nFabaoFNdx+5].GetInt();
			(*pRes)[nFabaoFNdx+6].GetBlob(pFabao->eWuxingHole,min((*pRes)[nFabaoFNdx+6].GetLen(), sizeof(pFabao->eWuxingHole)));
			(*pRes)[nFabaoFNdx+7].GetBlob(pFabao->dwWuxingBeadID,min((*pRes)[nFabaoFNdx+7].GetLen(), sizeof(pFabao->dwWuxingBeadID)));
			(*pRes)[nFabaoFNdx+8].GetBlob(pFabao->eSlotAttType,min((*pRes)[nFabaoFNdx+8].GetLen(), sizeof(pFabao->eSlotAttType)));
			(*pRes)[nFabaoFNdx+9].GetBlob(pFabao->nSlotAttValue,min((*pRes)[nFabaoFNdx+9].GetLen(), sizeof(pFabao->nSlotAttValue)));
			//(*pRes)[nFabaoFNdx+10].GetBlob(pFabao->nWuxingMatchReward,min((*pRes)[nFabaoFNdx+10].GetLen(), sizeof(pFabao->nWuxingMatchReward)));
			pFabao->nUseLevel			= (*pRes)[nFabaoFNdx+10].GetInt();
			pFabao->nSlotNum			= (*pRes)[nFabaoFNdx+11].GetInt();
			(*pRes)[nFabaoFNdx+12].GetBlob(pFabao->nAttGrowing0,min((*pRes)[nFabaoFNdx+12].GetLen(), sizeof(pFabao->nAttGrowing0)));

			pFabao->bySlotLingzhuMatch	= (*pRes)[nFabaoFNdx+13].GetInt();
			(*pRes)[nFabaoFNdx+14].GetBlob(pFabao->bySlotMacthNdx,min((*pRes)[nFabaoFNdx+14].GetLen(),sizeof(pFabao->bySlotMacthNdx)));

			// 1.2.1妖精心情
			pFabao->nMood	= (*pRes)[nFabaoFNdx+15].GetInt();
			pFabao->nMoodTicks	= (*pRes)[nFabaoFNdx+16].GetInt();
			pFabao->dwMoodBuffID	= (*pRes)[nFabaoFNdx+17].GetDword();

			// 1.2.2妖精元素伤害抗性，以及特技等
			pFabao->eEleInjury	= (EElementInjury)(*pRes)[nFabaoFNdx+18].GetInt();
			pFabao->nEleInjury	= (*pRes)[nFabaoFNdx+19].GetInt();
			pFabao->nEleResistance[EER_Fire]	= (*pRes)[nFabaoFNdx+20].GetInt();
			pFabao->nEleResistance[EER_Water]	= (*pRes)[nFabaoFNdx+21].GetInt();
			pFabao->nEleResistance[EER_Earth]	= (*pRes)[nFabaoFNdx+22].GetInt();
			pFabao->nEleResistance[EER_Wind]	= (*pRes)[nFabaoFNdx+23].GetInt();
			pFabao->nEleResistance[EER_Bright]	= (*pRes)[nFabaoFNdx+24].GetInt();
			pFabao->nEleResistance[EER_Dark]	= (*pRes)[nFabaoFNdx+25].GetInt();
			// 
			pFabao->wDamageUpgradeTimes	= (*pRes)[nFabaoFNdx+26].GetInt();
			pFabao->wResistanceUpgradeTimes	= (*pRes)[nFabaoFNdx+27].GetInt();
			pFabao->dwFairySkill[0]	= (*pRes)[nFabaoFNdx+28].GetDword();
			pFabao->dwFairySkill[1]	= (*pRes)[nFabaoFNdx+29].GetDword();
			pFabao->dwFairySkill[2]	= (*pRes)[nFabaoFNdx+30].GetDword();
			pFabao->dwFairySkill[3]	= (*pRes)[nFabaoFNdx+31].GetDword();
			pFabao->dwFairySkill[4]	= (*pRes)[nFabaoFNdx+32].GetDword();
			pFabao->dwUnsureSkill		= (*pRes)[nFabaoFNdx+33].GetDword();
			pFabao->bStoredExpOpen = ((*pRes)[nFabaoFNdx+34].GetDword() == 0)?FALSE:TRUE;
			pFabao->nCurStoredExp = (*pRes)[nFabaoFNdx+35].GetInt();

			pFabao->eFabaoStatus			= (EFabaoStatus)(*pRes)[nFabaoFNdx+36].GetInt();
			pFabao->dwStatusChangeTime	= (*pRes)[nFabaoFNdx+37].GetDword();
			pFabao->nBirthValue			= (*pRes)[nFabaoFNdx+38].GetInt();
			pFabao->nBirthTimes			= (*pRes)[nFabaoFNdx+39].GetInt();
			(*pRes)[nFabaoFNdx+40].GetBlob(pFabao->byBirthLevel,min((*pRes)[nFabaoFNdx+40].GetLen(),sizeof(pFabao->byBirthLevel)));
			pFabao->byIsGivenBirthTimes		= (*pRes)[nFabaoFNdx+41].GetInt();
			pFabao->byMaxIntelligence = (*pRes)[nFabaoFNdx+42].GetInt();
			pFabao->nElePierce = (*pRes)[nFabaoFNdx+43].GetInt();
			pFabao->wEleInjPromoteTimes = (*pRes)[nFabaoFNdx+44].GetInt();
			pFabao->wEleResPromoteTimes = (*pRes)[nFabaoFNdx+45].GetInt();

			*pLen = sizeof(tagFabao);
			pData = &pFabao[1];
			pRes->NextRow();
		}

	}
}

//-------------------------------------------------------------------------------------------------------
// 读取好友信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadFriend(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("friend", "FriendID,GroupID");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取好友信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadFriend(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, tagFriendSave);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_FRIENDNUM)
	{
		// 向监控发消息//??

		ASSERT(*pNum <= MAX_FRIENDNUM);

		*pNum = MAX_FRIENDNUM; // 此种情况只取前面的
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		p[i].dwFriendID = (*pRes)[0].GetDword();
		p[i].nGroupID	= (*pRes)[1].GetInt();

		pRes->NextRow();
	}

	pData = &p[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 读取友好度信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadFriendship(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("friendship", "FriendID,FriVal");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取友好度信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadFriendshipEx(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("friendship", "RoleID,FriVal");
	pStream->SetWhere();
	pStream->FillString("FriendID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取友好度信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadFriendship(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, tagFriendshipSave);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_FRIENDNUM)
	{
		// 向监控发消息//??

		ASSERT(*pNum <= MAX_FRIENDNUM);

		*pNum = MAX_FRIENDNUM; // 此种情况只取前面的
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		p[i].dwFriendID = (*pRes)[0].GetDword();
		p[i].nFriVal	= (*pRes)[1].GetInt();

		pRes->NextRow();
	}

	pData = &p[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 读取黑名单信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadBlackList(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("blacklist", "BlackID");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取黑名单信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadBlackList(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, DWORD);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_BLACKLIST)
	{
		// 向监控发消息//??

		ASSERT(*pNum <= MAX_BLACKLIST);

		*pNum = MAX_BLACKLIST; // 此种情况只取前面的
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		p[i] = (*pRes)[0].GetDword();

		pRes->NextRow();
	}

	pData = &p[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 读取师徒信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadMaster(Beton::MyStream *pStream, DWORD dwRoleID, DWORD dwMasterID)
{
	pStream->SetSelect("Master", "dwID,JingWuPoint,QinWuPoint,LevelUpBonus,RewardNum,ApprenticeData");
	pStream->SetWhere();
	pStream->FillString("dwID=") << dwMasterID;
}

//-------------------------------------------------------------------------------------------------------
// 读取师傅信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadMaster(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, tagMasterApprentice2DB);

	p->dwID					=	(*pRes)[0].GetDword();
	//p->nJingWuPoint			=	(*pRes)[1].GetDword();
	//p->nQinWuPoint			=	(*pRes)[2].GetDword();
	(*pRes)[5].GetBlob(p->sApprentice.dwData, (*pRes)[5].GetLen());

	DWORD dwSize = sizeof(tagMasterApprentice2DB);
	pData = reinterpret_cast<BYTE *>(pData) + dwSize;
}

//-------------------------------------------------------------------------------------------------------
// 读取同门信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadClassMates(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, tagClassMateData);
	p->nJingWuPoint		= (*pRes)[1].GetInt();
	p->nQinWuPoint		= (*pRes)[2].GetInt();
	p->nLevelUpBonus	= (*pRes)[3].GetInt();
	(*pRes)[5].GetBlob(p->dwData, (*pRes)[5].GetLen());

	DWORD dwSize = sizeof(tagClassMateData);
	pData = reinterpret_cast<BYTE *>(pData) + dwSize;
}

//-------------------------------------------------------------------------------------------------------
// 读取徒弟信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadApprentice(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, tagMasterApprentice2DB);

	//p->dwID					=	(*pRes)[0].GetDword();
	p->nJingWuPoint			=	(*pRes)[1].GetInt();
	p->nQinWuPoint			=	(*pRes)[2].GetInt();
	p->nLevelUpBonus		=	(*pRes)[3].GetInt();
	p->nRewardNum			=	(*pRes)[4].GetInt();
	(*pRes)[5].GetBlob(p->sApprentice.dwData, (*pRes)[5].GetLen());

	DWORD dwSize = sizeof(tagMasterApprentice2DB);
	pData = reinterpret_cast<BYTE *>(pData) + dwSize;
}

//-------------------------------------------------------------------------------------------------------
// 读取角色名贴信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadVCard(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("visiting_card", "Level,Job,MateRoleID,FactionID,Position,Visibility,Sex,Constellation,ChineseZodiac,Area,Province,City,HeadUrl,Signature,NameHistory, RoleID");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}


//-------------------------------------------------------------------------------------------------------
// 读取角色名贴信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadVCard(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, tagVCardData);
	*pNum = pRes->GetRowCount();

	ASSERT(1 == *pNum);

	p->dwLevel			= (*pRes)[0].GetInt();
	p->dwJob			= (*pRes)[1].GetInt();
	p->dwMateRoleID		= (*pRes)[2].GetInt();
	p->dwFactionID		= (*pRes)[3].GetInt();
	p->dwPosition		= (*pRes)[4].GetInt();

	p->customVCardData.bVisibility		= (*pRes)[5].GetBool();
	p->customVCardData.bySex			= (*pRes)[6].GetByte();
	p->customVCardData.byConstellation	= (*pRes)[7].GetByte();
	p->customVCardData.byChineseZodiac	= (*pRes)[8].GetByte();
	p->customVCardData.byArea			= (*pRes)[9].GetByte();
	p->customVCardData.byProvince		= (*pRes)[10].GetByte();
	(*pRes)[11].GetTCHAR(p->customVCardData.chCity, LEN_CITY_NAME);
	p->customVCardData.byHeadUrlSize	= (*pRes)[12].GetTCHAR(reinterpret_cast<TCHAR *>(p->customVCardData.byData), 200) * sizeof(TCHAR);
	p->customVCardData.bySignatureSize	= (*pRes)[13].GetTCHAR(reinterpret_cast<TCHAR *>(&(p->customVCardData.byData[p->customVCardData.byHeadUrlSize])), 200) * sizeof(TCHAR);
	p->customVCardData.byNameHistory	= (*pRes)[14].GetTCHAR(reinterpret_cast<TCHAR *>(&(p->customVCardData.byData[p->customVCardData.byHeadUrlSize + p->customVCardData.bySignatureSize ])), 500, FALSE) * sizeof(TCHAR);

	p->dwRoleID		= (*pRes)[15].GetInt();

	DWORD dwVCardSize = sizeof(tagVCardData) - 1 + p->customVCardData.byHeadUrlSize + p->customVCardData.bySignatureSize + p->customVCardData.byNameHistory;

	pData = reinterpret_cast<BYTE *>(pData) + dwVCardSize;

}
//-------------------------------------------------------------------------------------------------------
// 读取氏族信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadClanData(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("clan_data", "RepXuanYuan,ConXuanYuan,ActCntXuanYuan,"
									"RepShenNong,ConShenNong,ActCntShenNong,"
									"RepFuXi,ConFuXi,ActCntFuXi,"
									"RepSanMiao,ConSanMiao,ActCntSanMiao,"
									"RepJiuLi,ConJiuLi,ActCntJiuLi,"
									"RepYueZhi,ConYueZhi,ActCntYueZhi,"
									"RepNvWa,ConNvWa,ActCntNvWa,"
									"RepGongGong,ConGongGong,ActCntGongGong,"
									"FameMask, RepRstTimeStamp,"
									"TotalFame, FamePm");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取氏族信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadClanData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);
	MTRANS_POINTER(p, pData, tagDBReputeData);

	*pNum = pRes->GetRowCount();
	ASSERT(1 == *pNum);

	
	for(INT nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
	{
		p->m_nReputation[nClanType]		= (*pRes)[nClanType * 3].GetInt();
		p->m_nContribution[nClanType]	= (*pRes)[nClanType * 3 + 1].GetInt();
		p->m_n8ActCount[nClanType]		= (*pRes)[nClanType * 3 + 2].GetByte();
		
	}
	p->m_u16FameMask	= (*pRes)[ECLT_NUM * 3].GetShort();
	p->m_dwtLastResetTime = (*pRes)[ECLT_NUM * 3 + 1].GetDword();
	p->m_dwTotalRep = (*pRes)[ECLT_NUM * 3 + 2].GetDword();
	p->m_nRepPm = (*pRes)[ECLT_NUM * 3 + 3].GetInt();

	pData = reinterpret_cast<BYTE *>(pData) + sizeof(tagDBReputeData);
}

//-------------------------------------------------------------------------------------------------------
// 读取仇敌信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadEnemy(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("enemy", "EnemyID");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取仇敌信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadEnemy(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pEnemyID, pData, DWORD);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_ENEMYNUM)
	{
		// 向监控发消息//??

		ASSERT(*pNum <= MAX_ENEMYNUM);

		*pNum = MAX_ENEMYNUM; // 此种情况只取前面的
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		pEnemyID[i] = (*pRes)[0].GetDword();

		pRes->NextRow();
	}

	pData = &pEnemyID[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 读取任务信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadTask(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("task", "TaskID,StartTime,TaskFlag,Monster0Num,Monster1Num,Monster2Num,"
						"Monster3Num,Monster4Num,Monster5Num,ChuandaoCount,ScriptData, DynamicTarget");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取任务信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadTask(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pQuest, pData, tagQuestSave);
	*pNum = pRes->GetRowCount();

	if(*pNum > QUEST_MAX_COUNT)
	{
		// 向监控发消息//??

		ASSERT(*pNum <= QUEST_MAX_COUNT);
		*pNum = QUEST_MAX_COUNT;	// 只读取前面的任务
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		pQuest[i].u16QuestID	= (UINT16)(*pRes)[0].GetDword();
		pQuest[i].dwAcceptTime	= (*pRes)[1].GetDword();
		pQuest[i].bQuestFlag	= (*pRes)[2].GetShort();
		
		for(INT32 j=0; j<QUEST_CREATURES_COUNT; ++j)
		{
			pQuest[i].n16MonsterNum[j] = (INT16)(*pRes)[j+3].GetInt();
		}

		pQuest[i].nChuandaoCount = (*pRes)[QUEST_CREATURES_COUNT+3].GetInt();
		(*pRes)[QUEST_CREATURES_COUNT+4].GetBlob(pQuest[i].Data.dwData, (*pRes)[QUEST_CREATURES_COUNT+4].GetLen());
		if((*pRes)[QUEST_CREATURES_COUNT+5].GetLen() > 0)
			(*pRes)[QUEST_CREATURES_COUNT+5].GetBlob(&pQuest[i].DynamicTarget, (*pRes)[QUEST_CREATURES_COUNT+5].GetLen());

		pRes->NextRow();
	}

	pData = &pQuest[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 读取已完成任务信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadTaskDone(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("task_done", "TaskID,Times,StartTime");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取已完成日常任务信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadDailyTaskDone(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("dailytask_done", "TaskID,Times,StartTime");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取已完成任务信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadTaskDone(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pQuest, pData, tagQuestDoneSave);
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		pQuest[i].u16QuestID = (UINT16)(*pRes)[0].GetDword();
		pQuest[i].dwStartTime = (*pRes)[2].GetDword();
		pQuest[i].nTimes = (*pRes)[1].GetInt();

		pRes->NextRow();
	}

	pData = &pQuest[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 读取已完成日常任务信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadDailyTaskDone(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pQuest, pData, tagQuestDoneSave);
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		pQuest[i].u16QuestID = (UINT16)(*pRes)[0].GetDword();
		pQuest[i].dwStartTime = (*pRes)[2].GetDword();
		pQuest[i].nTimes = (*pRes)[1].GetInt();

		pRes->NextRow();
	}

	pData = &pQuest[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 读取已完成任务简易信息SQL语句格式化(玩家登陆时用)
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadSimTaskDone(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("task_done", "TaskID");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 读取已完成任务简易信息结果检查及保存(玩家登陆时用)
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadSimTaskDone(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pQuest, pData, tagQuestDoneSimSave);
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		pQuest[i].u16QuestID = (UINT16)(*pRes)[0].GetDword();

		pRes->NextRow();
	}

	pData = &pQuest[*pNum];
}

//-------------------------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadItemCDTime(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("item_cdtime", "CDTime");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadItemCDTime(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	if((*pRes)[0].IsNull())
	{
		*pNum = 0;
		return;
	}

	*pNum = (*pRes)[0].GetLen() / sizeof(tagCDTime);
	(*pRes)[0].GetBlob(pData, (*pRes)[0].GetLen());	

	pData = (BYTE*)pData + (*pRes)[0].GetLen();
}

//-------------------------------------------------------------------------------------------------------
// 读取百宝袋中的物品信息SQL语句格式化(玩家登陆时用)
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadItemBaiBao(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->FillString("select ");
	InitItemLoad(pStream);
	InitSoulCrystal(pStream);
	InitFabaoLoad(pStream,TRUE);
		
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
	if (GT_VALID(m_n64LoadSerial))
	{
		pStream->FillString(" and SerialNum=") << m_n64LoadSerial;
	}

	//IsLoadShengLing(pStream);
	pStream->SetLimit(m_nLoadNumLim);
}

//-------------------------------------------------------------------------------------------------------
// 读取百宝袋中的元宝数量SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadBaiBaoYuanBao(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->SetSelect("account_common", "BaiBaoYuanBao");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//-------------------------------------------------------------------------------------------------------
// 读取百宝袋中的元宝数量
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadBaiBaoYuanBao(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	INT32* nYuanBao = (INT32*)pData;

	INT32 nNum	= pRes->GetRowCount();
	*nYuanBao	= 0;

	if (nNum != 0)
	{
		*nYuanBao = (*pRes)[0].GetInt();
	}
}

//-------------------------------------------------------------------------------------------------------
// 读取百宝袋中的物品记录SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadBaiBaoLog(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->SetSelect("log_baibao", "AccountID,RoleID,LogTime,n16Type,TypeID,LeaveWords");
	pStream->SetWhere();
	pStream->FillString("AccountID=") << dwAccountID;
}

//-------------------------------------------------------------------------------------------------------
// 保存读取到的百宝袋Log
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadBaiBaoLog(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pBaibaoLog, pData, tagBaiBaoRecord);
	*pNum = pRes->GetRowCount();

	for(INT16 i=0; i<*pNum; ++i)
	{
		pBaibaoLog->dwRoleID = (*pRes)[1].GetDword();
		pBaibaoLog->dwTypeID = (*pRes)[4].GetDword();
		pBaibaoLog->n16Type = (*pRes)[3].GetShort();
		DataTime2DwordTime(*(tagDWORDTime*)&(pBaibaoLog->dwTime), (*pRes)[2].GetString(), (*pRes)[2].GetLen());
		(*pRes)[5].GetBlob(pBaibaoLog->szWords, (*pRes)[5].GetLen());

		pBaibaoLog->n16Size = (INT16)(sizeof(tagBaiBaoRecord) + (*pRes)[5].GetLen() - sizeof(pBaibaoLog->szWords));

		pBaibaoLog = (tagBaiBaoRecord*)((BYTE*)pBaibaoLog + pBaibaoLog->n16Size);
		pRes->NextRow();
	}

	pData = pBaibaoLog;
}

////-------------------------------------------------------------------------------------------------------
//// 读取百宝袋中的物品信息结果检查及保存
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::ProcResLoadItemBaiBao(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
//{
//	ASSERT(pNum != NULL);
//
//	*pNum = pRes->GetRowCount();
//
//	for(INT32 i=0; i<*pNum; ++i)
//	{
//		MTRANS_POINTER(pItem, pData, tagItem);
//
//		pItem->n64Serial 					= (*pRes)[0].GetLong();
//		pItem->n16Num 						= (*pRes)[1].GetShort();
//		pItem->dwTypeID 					= (*pRes)[2].GetDword();
//		pItem->byBind 						= (*pRes)[3].GetInt();
//		pItem->bLock 						= (*pRes)[4].GetBool();
//
//		pItem->nUseTimes 					= (*pRes)[5].GetInt();
//		pItem->dwCreatorID 					= (*pRes)[7].GetDword();
//		pItem->dwOwnerID 					= (*pRes)[9].GetDword();
//		pItem->dwAccountID 					= (*pRes)[10].GetDword();
//
//		pItem->eConType 					= (EItemConType)(*pRes)[11].GetInt();
//		pItem->n16Index 					= (*pRes)[12].GetShort();
//		pItem->eCreateMode					= (EItemCreateMode)(*pRes)[45].GetInt();
//		pItem->dwCreateID					= (*pRes)[46].GetDword();
//
//		if(!DataTime2DwordTime(pItem->dw1stGainTime, (*pRes)[6].GetString(), (*pRes)[6].GetLen()))
//		{
//			// 数据库中记录日期的字符串格式错误.应形如: 2008-11-03 11:47:00
//			IMSG(_T("Error: Func DataTime2DwordTime() run error in CLoongDB::ProcResLoadItem()!!!!!\r\n"));
//			IMSG(_T("Error: Item Serial Number is %lld!!!!!\r\n"), pItem->n64Serial);
//			ASSERT(0);
//		}
//
//		if(!DataTime2DwordTime(pItem->dwCreateTime, (*pRes)[8].GetString(), (*pRes)[8].GetLen()))
//		{
//			// 数据库中记录日期的字符串格式错误.应形如: 2008-11-03 11:47:00
//			IMSG(_T("Error: Func DataTime2DwordTime() run error in CLoongDB::ProcResLoadItem()!!!!!\r\n"));
//			IMSG(_T("Error: Item Serial Number is %lld!!!!!\r\n"), pItem->n64Serial);
//			ASSERT(0);
//		}
//
//		if(!MIsEquipment(pItem->dwTypeID))
//		{
//			pData = &pItem[1];
//			pRes->NextRow();
//			continue;
//		}
//
//		MTRANS_POINTER(pEquip, pData, tagEquip);
//
//		// 装备属性
//		pEquip->equipSpec.byQuality 				= (*pRes)[13].GetByte();
//		pEquip->equipSpec.nPotValUsed 				= (*pRes)[14].GetInt();
//		pEquip->equipSpec.byMinUseLevel 			= (*pRes)[15].GetByte();
//		pEquip->equipSpec.byMaxUseLevel 			= (*pRes)[16].GetByte();
//		pEquip->equipSpec.n16WuHun 					= (*pRes)[17].GetShort();
//		pEquip->equipSpec.n16MinDmg 				= (*pRes)[18].GetShort();
//		pEquip->equipSpec.n16MaxDmg 				= (*pRes)[19].GetShort();
//		pEquip->equipSpec.n16Armor 					= (*pRes)[20].GetShort();
//		pEquip->equipSpec.nPotVal 					= (*pRes)[21].GetInt();
//
//		pEquip->equipSpec.byPosyTimes 				= (*pRes)[23].GetByte();
//
//		pEquip->equipSpec.byEngraveTimes 			= (*pRes)[25].GetByte();
//
//		pEquip->equipSpec.byHoleNum 				= (*pRes)[27].GetByte();
//
//		pEquip->equipSpec.byBrandLevel 				= (*pRes)[29].GetByte();
//
//		pEquip->equipSpec.byLongfuLevel				= (*pRes)[31].GetByte();
//
//		pEquip->equipSpec.bCanCut 					= (*pRes)[33].GetBool();
//		pEquip->equipSpec.dwLongHunInnerTriggerID	= (*pRes)[34].GetDword();
//		pEquip->equipSpec.dwLongHunInnerBuffID		= (*pRes)[35].GetDword();
//		pEquip->equipSpec.dwLongHunOuterTriggerID	= (*pRes)[36].GetDword();
//		pEquip->equipSpec.dwLongHunOuterBuffID		= (*pRes)[37].GetDword();
//		pEquip->equipSpec.bySpecAtt					= (*pRes)[38].GetInt();
//		//pEquip->equipSpec.dwSpecBuffID				= (*pRes)[39].GetDword();
//		pEquip->equipSpec.byFlareType				= (*pRes)[40].GetInt();
//		pEquip->equipSpec.byFlareEffect				= (*pRes)[41].GetInt();
//		pEquip->equipSpec.n16QltyModPct				= (*pRes)[42].GetShort();
//		pEquip->equipSpec.n16QltyModPctEx			= (*pRes)[43].GetShort();
//		pEquip->equipSpec.dwColor					= (*pRes)[44].GetDword();
//		
//		(*pRes)[22].GetBlob(pEquip->equipSpec.nRoleAttEffect, min((*pRes)[22].GetLen(), sizeof(pEquip->equipSpec.nRoleAttEffect)));
//		(*pRes)[24].GetBlob(pEquip->equipSpec.PosyEffect, min((*pRes)[24].GetLen(), sizeof(pEquip->equipSpec.PosyEffect)));
//		(*pRes)[26].GetBlob(pEquip->equipSpec.nEngraveAtt, min((*pRes)[26].GetLen(), sizeof(pEquip->equipSpec.nEngraveAtt)));
//		(*pRes)[28].GetBlob(pEquip->equipSpec.dwHoleGemID, min((*pRes)[28].GetLen(), sizeof(pEquip->equipSpec.dwHoleGemID)));
//		(*pRes)[30].GetBlob(pEquip->equipSpec.nBrandAtt, min((*pRes)[30].GetLen(), sizeof(pEquip->equipSpec.nBrandAtt)));
//		(*pRes)[32].GetBlob(pEquip->equipSpec.byLongfuAtt, min((*pRes)[32].GetLen(), sizeof(pEquip->equipSpec.byLongfuAtt)));
//
//		pData = &pEquip[1];
//		pRes->NextRow();
//	}
//}

//-------------------------------------------------------------------------------------------------------
// 读取需要记录log的物品TypeID信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadItemNeedLog(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("item_needlog", "TypeID");
	pStream->SetWhere();
	pStream->FillString("NeedLog=") << TRUE;
}

//-------------------------------------------------------------------------------------------------------
// 读取需要记录log的物品TypeID信息结果检查及保存
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadItemNeedLog(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pTypeID, pData, DWORD);
	*pNum = pRes->GetRowCount();

	if(*pNum > MAX_ITEM_NEEDLOG_NUM)
	{
		ASSERT(*pNum <= MAX_ITEM_NEEDLOG_NUM);
		*pNum = MAX_ITEM_NEEDLOG_NUM;
	}

	for(INT32 i=0; i<*pNum; ++i)
	{
		pTypeID[i] = (*pRes)[0].GetDword();

		pRes->NextRow();
	}

	pData = &pTypeID[*pNum];
}
//-------------------------------------------------------------------------------------------------------
// 读取所有团购信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadAllGPInfo(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->SetSelect("group_purchase", "GuildID,RoleID,MallID,ItemPrice,"
		"ParticipatorNum,RequireNum,RemainTime,ParticipatorList");
}

//-------------------------------------------------------------------------------------------------------
// 读取所有剩余物品信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadLimitItemInfo(Beton::MyStream *pStream, DWORD dwRoleID, DWORD dwID)
{
	pStream->Clear();
	pStream->SetSelect("LimitNumItemInfo", 
		"Id,ItemTypeID,RemainNum,IsEnd");
}

//-------------------------------------------------------------------------------------------------------
// 读取所有团购信息查询结果处理
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadAllGPInfo(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pGPInfo, pData, tagGPInfo);
	*pNum = pRes->GetRowCount();

	for(INT16 i=0; i<*pNum; ++i)
	{
		pGPInfo->dwGuildID			= (*pRes)[0].GetDword();
		pGPInfo->dwRoleID			= (*pRes)[1].GetDword();
		pGPInfo->dwMallID			= (*pRes)[2].GetDword();
		pGPInfo->nPrice				= (*pRes)[3].GetDword();
		pGPInfo->nParticipatorNum	= (*pRes)[4].GetShort();
		pGPInfo->nRequireNum		= (*pRes)[5].GetShort();
		pGPInfo->nRemainTime		= (*pRes)[6].GetInt();

		(*pRes)[7].GetBlob(pGPInfo->dwParticipators, (*pRes)[7].GetLen());

		pGPInfo = (tagGPInfo*)((BYTE*)pGPInfo + sizeof(tagGPInfo) + (pGPInfo->nParticipatorNum-1)*sizeof(DWORD));
		pRes->NextRow();
	}

	pData = pGPInfo;
}

//-------------------------------------------------------------------------------------------------------
// 读取所有限量物品查询结果处理
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadLimitItemInfo(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pInfo, pData, tagMallItemLimitNum);
	*pNum = pRes->GetRowCount();

	for(INT16 i=0; i<*pNum; ++i)
	{
		pInfo->dwID			        = (*pRes)[0].GetDword();
		pInfo->dwTypeID			    = (*pRes)[1].GetDword();
		pInfo->n16RemainNum			= (*pRes)[2].GetDword();
		pInfo->bIsEnd				= (*pRes)[3].GetDword();	

		pInfo = (tagMallItemLimitNum*)((BYTE*)pInfo + sizeof(tagMallItemLimitNum));
		pRes->NextRow();
	}

	pData = pInfo;
}

//-------------------------------------------------------------------------------------------------------
// 读取所有VIP摊位信息SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatGetAllVIPStallInfo(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->SetSelect("vip_stall", "StallID,RoleID,RemainTime");
}

//-------------------------------------------------------------------------------------------------------
// 读取所有VIP摊位信息查询结果处理
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResGetAllVIPStallInfo(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pVIPStallInfo, pData, tagVipStall2DB);

	*pNum = pRes->GetRowCount();

	for(INT16 i=0; i<*pNum; ++i)
	{
		BYTE index = (*pRes)[0].GetByte();
		pVIPStallInfo[index].byStallIndex	= index;
		pVIPStallInfo[index].dwOwnerID		= (*pRes)[1].GetDword();
		pVIPStallInfo[index].nRemainTime	= (*pRes)[2].GetInt();

		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// 读取所有固定活动数据SQL语句格式化
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatActivityData(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("activity", "dwID, ScriptData,ScriptData1,ScriptData2,ScriptData3,ScriptData4");
}

//-------------------------------------------------------------------------------------------------------
// 读取所有固定活动数据
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadActivityData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pActivityData, pData, tagActivityData);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum; ++i)
	{
		pActivityData[i].dwID = (*pRes)[0].GetDword();
		(*pRes)[1].GetBlob(pActivityData[i].Data.dwData, (*pRes)[1].GetLen());
		if ((*pRes)[2].GetLen() > 0)
		{
			pActivityData[i].MergedData[0].bHaveData = TRUE;
			(*pRes)[2].GetBlob(pActivityData[i].MergedData[0].MergedScriptData.dwData, (*pRes)[2].GetLen());
		}
		else
		{
			pActivityData[i].MergedData[0].bHaveData = FALSE;
		}

		if ((*pRes)[3].GetLen())
		{
			pActivityData[i].MergedData[1].bHaveData = TRUE;
			(*pRes)[3].GetBlob(pActivityData[i].MergedData[1].MergedScriptData.dwData, (*pRes)[3].GetLen());
		}
		else
		{
			pActivityData[i].MergedData[1].bHaveData = FALSE;
		}

		if ((*pRes)[4].GetLen())
		{
			pActivityData[i].MergedData[2].bHaveData = TRUE;
			(*pRes)[4].GetBlob(pActivityData[i].MergedData[2].MergedScriptData.dwData, (*pRes)[4].GetLen());
		}
		else
		{
			pActivityData[i].MergedData[2].bHaveData = FALSE;
		}

		if ((*pRes)[5].GetLen())
		{
			pActivityData[i].MergedData[3].bHaveData = TRUE;
			(*pRes)[5].GetBlob(pActivityData[i].MergedData[3].MergedScriptData.dwData, (*pRes)[5].GetLen());
		}
		else
		{
			pActivityData[i].MergedData[3].bHaveData = FALSE;
		}

		if ((*pRes)[1].GetLen() == 0 && ((*pRes)[2].GetLen() != 0 || (*pRes)[3].GetLen() != 0) || (*pRes)[4].GetLen() != 0 || (*pRes)[5].GetLen() != 0)
			pActivityData[i].bHaveMergedData = TRUE;
		else
			pActivityData[i].bHaveMergedData = FALSE;

		pRes->NextRow();
	}
}


VOID CLoongDB::FormatStrengthRankings(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("strength_rankings", "Num, PreNum, YaoJingValue, RoleName, RoleID");
}

VOID CLoongDB::FormatFameRankings(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("fame_rankings", "Num, PreNum, Fame, RoleID");
}

VOID CLoongDB::FormatLevelRankings(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("level_rankings", "Num, PreNum, Level, RoleName, RoleID");
}

VOID CLoongDB::FormatPetRankings(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("pet_rankings", "Num, PreNum, Pet, PetID, PetName, RoleID");
}

VOID CLoongDB::FormatEquipRankings(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("equip_rankings", "Num, PreNum, Equip, RoleID");
}

VOID CLoongDB::FormatFlowerRankings(Beton::MyStream* pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("like_rankings", " Num, PreNum, flowerNum, RoleName, RoleID");
}

VOID CLoongDB::FormatEggRankings(Beton::MyStream* pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("hate_rankings", "Num, PreNum, EggNum, RoleName, RoleID");
}

VOID CLoongDB::ProcResLoadStrengthRankings(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pRankingsNode, pData, tagRankingsNode);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum && i<MAX_RANKINGS_LEN; ++i)
	{
		// 依次取出各条记录
		
		INT nNum = (*pRes)[0].GetInt();
		INT nPreNum = (*pRes)[1].GetInt();
		INT64 n64Value = (INT64)(*pRes)[2].GetDword();
		TCHAR szRoleName[X_SHORT_NAME] = {0};
		(*pRes)[3].GetTCHAR(szRoleName, X_SHORT_NAME, FALSE);
		szRoleName[X_SHORT_NAME-1] = '\0';
		DWORD dwRoleID = (*pRes)[4].GetDword();

		if ( _tcscmp(szRoleName, _T("")) != 0 && nNum != 0 )
		{
			pRankingsNode[i].nNum = nNum;
			pRankingsNode[i].nPreNum = nPreNum;
			pRankingsNode[i].n64Value = n64Value;
			pRankingsNode[i].dwRoleID = dwRoleID;
			_tcscpy_s(pRankingsNode[i].szRoleName, X_SHORT_NAME, szRoleName);
		}

		pRes->NextRow();
	}
}

VOID CLoongDB::ProcResLoadFameRankings(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pRankingsNode, pData, tagRankingsNode);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum && i<MAX_RANKINGS_LEN; ++i)
	{
		// 依次取出各条记录
	
		INT nNum = (*pRes)[0].GetInt();
		INT nPreNum = (*pRes)[1].GetInt();
		INT64 n64Value = (INT64)(*pRes)[2].GetDword();
		DWORD dwRoleID = (*pRes)[3].GetDword();

		if ( dwRoleID != 0 && nNum != 0)
		{
			pRankingsNode[i].nNum = nNum;
			pRankingsNode[i].nPreNum = nPreNum;
			pRankingsNode[i].n64Value = n64Value;
			pRankingsNode[i].dwRoleID = dwRoleID;
		}
		pRes->NextRow();
	}
}

VOID CLoongDB::ProcResLoadPetRankings(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pRankingsNode, pData, tagPetRankingsNode);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum && i<MAX_RANKINGS_LEN; ++i)
	{
		// 依次取出各条记录

		INT nNum = (*pRes)[0].GetInt();
		INT nPreNum = (*pRes)[1].GetInt();
		INT64 n64Value = (*pRes)[2].GetLong();
		DWORD dwPetID = (*pRes)[3].GetDword();
		TCHAR szPetName[X_SHORT_NAME] = {0};
		(*pRes)[4].GetTCHAR(szPetName, X_SHORT_NAME, FALSE);
		szPetName[X_SHORT_NAME-1] = '\0';
		DWORD dwRoleID = (*pRes)[5].GetDword();

		if ( dwRoleID != 0 && nNum != 0)
		{
			pRankingsNode[i].nNum = nNum;
			pRankingsNode[i].nPreNum = nPreNum;
			pRankingsNode[i].n64Value = n64Value;
			pRankingsNode[i].dwPetID = dwPetID;
			_tcscpy_s( pRankingsNode[i].szPetName, X_SHORT_NAME, szPetName );
			pRankingsNode[i].dwRoleID = dwRoleID;
		}

		pRes->NextRow();
	}
}

VOID CLoongDB::ProcResLoadLevelRankings(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pRankingsNode, pData, tagRankingsNode);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum && i<MAX_RANKINGS_LEN; ++i)
	{
		// 依次取出各条记录

		INT nNum = (*pRes)[0].GetInt();
		INT nPreNum = (*pRes)[1].GetInt();
		INT64 n64Value = (INT64)(*pRes)[2].GetLong();
		TCHAR szRoleName[X_SHORT_NAME] = {0};
		(*pRes)[3].GetTCHAR(szRoleName, X_SHORT_NAME, FALSE);
		szRoleName[X_SHORT_NAME-1] = '\0';
		DWORD dwRoleID = (*pRes)[4].GetDword();

		if ( _tcscmp(szRoleName, _T("")) != 0 && nNum != 0 )
		{
			INT64 n64RealValue = n64Value/X_LONG_STR_4WORKED_LEVEL;
			pRankingsNode[i].nNum = nNum;
			pRankingsNode[i].nPreNum = nPreNum;
			pRankingsNode[i].n64Value = (n64RealValue == 0)?1:n64RealValue;
			pRankingsNode[i].dwRoleID = dwRoleID;
			_tcscpy_s(pRankingsNode[i].szRoleName, X_SHORT_NAME, szRoleName);
		}

		pRes->NextRow();
	}
}

VOID CLoongDB::ProcResLoadEquipRankings(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pRankingsNode, pData, tagRankingsNode);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum && i<MAX_RANKINGS_LEN; ++i)
	{
		// 依次取出各条记录

		INT nNum = (*pRes)[0].GetInt();
		INT nPreNum = (*pRes)[1].GetInt();
		INT64 n64Value = (INT64)(*pRes)[2].GetDword();
		DWORD dwRoleID = (*pRes)[3].GetDword();

		if ( dwRoleID != 0 && nNum != 0)
		{
			pRankingsNode[i].nNum = nNum;
			pRankingsNode[i].nPreNum = nPreNum;
			pRankingsNode[i].n64Value = n64Value;
			pRankingsNode[i].dwRoleID = dwRoleID;
		}
		pRes->NextRow();
	}
}

//鲜花榜
VOID CLoongDB::ProcResLoadFlowerRankings(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pRankingsNode, pData, tagRankingsNode);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum && i<MAX_FlowerEggRankings_LEN; ++i)
	{
		// 依次取出各条记录

		INT nNum = (*pRes)[0].GetInt();
		INT nPreNum = (*pRes)[1].GetInt();
		INT64 n64Value = (INT64)(*pRes)[2].GetLong();
		TCHAR szRoleName[X_SHORT_NAME] = {0};
		(*pRes)[3].GetTCHAR(szRoleName, X_SHORT_NAME, FALSE);
		szRoleName[X_SHORT_NAME-1] = '\0';
		DWORD dwRoleID = (*pRes)[4].GetDword();

		if ( _tcscmp(szRoleName, _T("")) != 0 && nNum != 0 )
		{
			pRankingsNode[i].nNum = nNum;
			pRankingsNode[i].nPreNum = nPreNum;
			pRankingsNode[i].n64Value = n64Value;
			pRankingsNode[i].dwRoleID = dwRoleID;
			_tcscpy_s(pRankingsNode[i].szRoleName, X_SHORT_NAME, szRoleName);
		}

		pRes->NextRow();
	}
}
//鸡蛋榜
VOID CLoongDB::ProcResLoadEggRankings(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pRankingsNode, pData, tagRankingsNode);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum && i<MAX_FlowerEggRankings_LEN; ++i)
	{
		// 依次取出各条记录

		INT nNum = (*pRes)[0].GetInt();
		INT nPreNum = (*pRes)[1].GetInt();
		INT64 n64Value = (INT64)(*pRes)[2].GetLong();
		TCHAR szRoleName[X_SHORT_NAME] = {0};
		(*pRes)[3].GetTCHAR(szRoleName, X_SHORT_NAME, FALSE);
		szRoleName[X_SHORT_NAME-1] = '\0';
		DWORD dwRoleID = (*pRes)[4].GetDword();

		if ( _tcscmp(szRoleName, _T("")) != 0 && nNum != 0 )
		{
			pRankingsNode[i].nNum = nNum;
			pRankingsNode[i].nPreNum = nPreNum;
			pRankingsNode[i].n64Value = n64Value;
			pRankingsNode[i].dwRoleID = dwRoleID;
			_tcscpy_s(pRankingsNode[i].szRoleName, X_SHORT_NAME, szRoleName);
		}

		pRes->NextRow();
	}
}

//-------------------------------------------------------------------------------------------------------
// 读取指定城市信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadCityInfo(Beton::MyStream *pStream, DWORD dwCityID, DWORD dwData)
{
	pStream->Clear();
	pStream->SetSelect("city", "id, guild_id, defence," 
		"eudemon_tally, tax_rate, tax_rate_time, taxation, prolificacy,"
		"signup_list");
	pStream->SetWhere();
	pStream->FillString("id=") << dwCityID;
}

//-------------------------------------------------------------------------------------------------------
// 处理读取的城市信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadCityInfo(LPVOID &pData, INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pCityInfo, pData, tagNDBS_LoadCityInfo);

	if (pRes->GetRowCount() <= 0)
	{
		return;
	}

	// 只处理一行数据
	pCityInfo->sCityAtt.dwCityID		= (*pRes)[0].GetDword();
	pCityInfo->sCityAtt.dwGuildID		= (*pRes)[1].GetDword();
	pCityInfo->sCityAtt.nDefence		= (*pRes)[2].GetInt();
	pCityInfo->sCityAtt.nEudemonTally	= (*pRes)[3].GetInt();
	pCityInfo->sCityAtt.nTaxRate		= (*pRes)[4].GetInt();
	pCityInfo->sCityAtt.dwTaxRateTime	= (*pRes)[5].GetDword();
	pCityInfo->sCityAtt.nTaxation		= (*pRes)[6].GetInt();
	pCityInfo->sCityAtt.nProlificacy	= (*pRes)[7].GetInt();

	pCityInfo->nSignUpNum				= (*pRes)[8].GetLen() / sizeof(tagCitySignUp);
	(*pRes)[8].GetBlob(pCityInfo->sSignUp, (*pRes)[8].GetLen());
}

//-------------------------------------------------------------------------------------------------------
// 读取帮派氏族声望信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatGetGuildClanVal(Beton::MyStream *pStream, DWORD dwGuildID, DWORD dwData)
{
	pStream->Clear();
	pStream->FillString("select sum(");

	switch ((ECLanType)dwData)
	{
	case ECLT_XuanYuan:
		pStream->FillString("RepXuanYuan");
		break;

	case ECLT_ShenNong:
		pStream->FillString("RepShenNong");
		break;

	case ECLT_FuXi:
		pStream->FillString("RepFuXi");
		break;

	case ECLT_SanMiao:
		pStream->FillString("RepSanMiao");
		break;

	case ECLT_JiuLi:
		pStream->FillString("RepJiuLi");
		break;

	case ECLT_YueZhi:
		pStream->FillString("RepYueZhi");
		break;

	case ECLT_NvWa:
		pStream->FillString("RepNvWa");
		break;

	case ECLT_GongGong:
		pStream->FillString("RepGongGong");
		break;
	}

	pStream->FillString(") from clan_data");
	pStream->SetWhere();
	pStream->FillString("RoleID in (select RoleID from roledata where GuildID=") << dwGuildID;
	pStream->FillString(")");
}

//-------------------------------------------------------------------------------------------------------
// 传出帮派氏族信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResGetGuildClanVal(LPVOID &pData, INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pClanVal, pData, tagGuildClanValue);

	if (pRes->GetRowCount() <= 0)
	{
		return;
	}

	// 只处理一行数据
	pClanVal->nClanValue	= (*pRes)[0].GetInt();

	// 偏移数据指针
	pData = &pClanVal[1];
}

//-------------------------------------------------------------------------------------------------------
// 读取指定师徒信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadMasterApprenticeData(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
	pStream->Clear();
	pStream->SetSelect("master", "dwID,JingWuPoint,QinWuPoint,LevelUpBonus,RewardNum,ApprenticeData");
	pStream->SetWhere();
	pStream->FillString("dwID=") << dwID;
}

//-------------------------------------------------------------------------------------------------------
// 处理读取的师徒信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadMasterApprenticeData(LPVOID &pData, INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pMAInfo, pData, tagMasterApprentice2DB);

	if (pRes->GetRowCount() <= 0)
	{
		return;
	}

	pMAInfo->dwID				= (*pRes)[0].GetDword();
	pMAInfo->nJingWuPoint		= (*pRes)[1].GetInt();
	pMAInfo->nQinWuPoint		= (*pRes)[2].GetInt();
	pMAInfo->nLevelUpBonus		= (*pRes)[3].GetInt();
	pMAInfo->nRewardNum			= (*pRes)[4].GetInt();
	(*pRes)[5].GetBlob(pMAInfo->sApprentice.dwData, (*pRes)[5].GetLen());

}



//-------------------------------------------------------------------------------------------------------
// 处理竞技场信息
//-------------------------------------------------------------------------------------------------------


BOOL CLoongDB::LoadArenaSystemSerialVal(DWORD dwSerialID,LPVOID pData)
{

	return Load(pData, NULL, dwSerialID, GT_INVALID, &CLoongDB::FormatLoadArenaSystemSerialVal, &CLoongDB::ProcResLoadArenaSystemSerialVal);  
}



VOID CLoongDB::FormatLoadArenaSystemSerialVal(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
	pStream->Clear();
	pStream->SetSelect("arena_data", "serial_id,serial_val");
	pStream->SetWhere();
	pStream->FillString("serial_id=") << dwID;
}

VOID CLoongDB::ProcResLoadArenaSystemSerialVal(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pGetSerialVal, pData, tagNDBS_GetSerialVal);


	if (pRes->GetRowCount() <= 0)
	{
		return;
	}
    pGetSerialVal->dwSerialId  = (*pRes)[0].GetDword();
	pGetSerialVal->dwSerialVal = (*pRes)[1].GetDword();
	
}

//-------------------------------------------------------------------------------------------------------
// 读取的快捷栏信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadShortCut_QuickBar(DWORD dwID, LPVOID pData)
{
	return Load(pData, NULL, dwID, GT_INVALID, &CLoongDB::FormatLoadShortCut_QuickBar, &CLoongDB::ProcResLoadShortCut_QuickBar);  
}


VOID CLoongDB::FormatLoadShortCut_QuickBar(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
	pStream->Clear();
	pStream->SetSelect("shortcut_quickbar", "RoleID,quickbardata");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwID;

}

//-------------------------------------------------------------------------------------------------------
//  处理读取的快捷栏信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadShortCut_QuickBar(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pQuickBarInfo, pData, tagNDBS_LoadShortCut_QuickBar);

	if (pRes->GetRowCount() <= 0)
	{
		return;
	}
    
	pQuickBarInfo->dwRoleID = (*pRes)[0].GetDword();
	(*pRes)[1].GetBlob(pQuickBarInfo->QuickBarData, (*pRes)[1].GetLen());
}

//-------------------------------------------------------------------------------------------------------
//  读取已开启试炼状态表
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatLoadOpenTrainStates(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->Clear();
	pStream->SetSelect("train_state", "TrainID,TrainState,Completeness");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
//  读取已开启试炼状态表(试炼状态--0:进行中, 1:已经完成)
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::ProcResLoadOpenTrainStates( OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes )
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pTrainState, pData, tagTrainState);
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		pTrainState[i].dwTrainID	= (UINT16)(*pRes)[0].GetDword();
		pTrainState[i].eTrainState	= static_cast<ETrainState>((*pRes)[1].GetByte());
		pTrainState[i].nCompleteness = (*pRes)[2].GetDword();
		pRes->NextRow();
	}

	pData = &pTrainState[*pNum];
}

BOOL CLoongDB::LoadShowPlayerData(DWORD dwID,LPVOID pData)
{
     return Load(pData, NULL, dwID, GT_INVALID, &CLoongDB::FormatLoadShowPlayerData, &CLoongDB::ProcResLoadShowPlayerData);  
}


BOOL CLoongDB::LoadVotePlayerData(DWORD dwID,LPVOID pData)
{
     return Load(pData, NULL, dwID, GT_INVALID, &CLoongDB::FormatLoadVotePlayerData, &CLoongDB::ProcResLoadVotePlayerData);  
}


VOID CLoongDB::FormatLoadShowPlayerData(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
	pStream->Clear();
	pStream->SetSelect("dragongirl_data", "sign_id,role_id,role_name,point,serial_id");
	pStream->SetWhere();
	pStream->FillString("serial_id=") << dwID;
}


VOID CLoongDB::FormatLoadVotePlayerData(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
	pStream->Clear();
	pStream->SetSelect("dragondefender_data", "serial_id,role_id,role_name,point");
	pStream->SetWhere();
	pStream->FillString("serial_id=") << dwID;
}



VOID CLoongDB::ProcResLoadShowPlayerData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pShowPlayer, pData, tagShowPlayer);

	if (pRes->GetRowCount() <= 0)
	{
		return;
	}

	pShowPlayer->dwAthleteSignId  = (*pRes)[0].GetDword();
	pShowPlayer->dwAthleteRoleId  = (*pRes)[1].GetDword();
	(*pRes)[2].GetTCHAR(pShowPlayer->tcAthleteRoleName, X_SHORT_NAME);
	pShowPlayer->dwAthletePoint   = (*pRes)[3].GetDword();
	pShowPlayer->dwAthleteSerialId =(*pRes)[4].GetDword();
}



VOID CLoongDB::ProcResLoadVotePlayerData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{

	MTRANS_POINTER(pVotePlayer, pData, tagVotePlayer);

	if (pRes->GetRowCount() <= 0)
	{
		return;
	}
    pVotePlayer->dwSerialId    = (*pRes)[0].GetDword();
	pVotePlayer->dwVoteRoleId  = (*pRes)[1].GetDword();
	(*pRes)[2].GetTCHAR(pVotePlayer->tcVoteRoleName, X_SHORT_NAME);
	pVotePlayer->dwVotePoint   = (*pRes)[3].GetDword();  
}

BOOL CLoongDB::GetShowPlayerDataCount(DWORD dwID,LPVOID pData)
{
     return Load(pData, NULL, dwID, GT_INVALID, &CLoongDB::FormatGetShowPlayerDataCount, &CLoongDB::ProcResGetShowPlayerDataCount);
}

BOOL CLoongDB::GetVotePlayerDataCount(DWORD dwID,LPVOID pData)
{
     return Load(pData, NULL, dwID, GT_INVALID, &CLoongDB::FormatGetVotePlayerDataCount, &CLoongDB::ProcResGetVotePlayerDataCount);
}

VOID CLoongDB::FormatGetShowPlayerDataCount(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
	pStream->Clear();
	pStream->SetSelect("dragongirl_data", "data_count");
	pStream->SetWhere();
	pStream->FillString("sign_id=") << 0;
}

VOID CLoongDB::FormatGetVotePlayerDataCount(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
	pStream->Clear();
	pStream->SetSelect("dragondefender_data", "data_count");
	pStream->SetWhere();
	pStream->FillString("role_id=") << 0;
}

VOID CLoongDB::ProcResGetShowPlayerDataCount(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pDataCount, pData, DWORD);

	if (pRes->GetRowCount() <= 0)
	{
		return;
	}

	*pDataCount  = (*pRes)[0].GetDword();
	
}

VOID CLoongDB::ProcResGetVotePlayerDataCount(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{  
	MTRANS_POINTER(pDataCount, pData, DWORD);

	if (pRes->GetRowCount() <= 0)
	{
		return;
	}

	*pDataCount  = (*pRes)[0].GetDword();

}


BOOL CLoongDB::LoadActivetyRankData(LPVOID pData, INT &nNum)
{
    return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatLoadActivetyRankData, &CLoongDB::ProcResLoadActivetyRankData);
}

VOID CLoongDB::FormatLoadActivetyRankData(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
     pStream->SetSelect("activity_rankdata", "role_id, role_name,role_level,datatype1,datatype2,datatype3,datatype4,datatype5,datatype6,datatype7,datatype8");
}

VOID CLoongDB::ProcResLoadActivetyRankData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{


	ASSERT(pNum != NULL);

	MTRANS_POINTER(pActivityRankData, pData, tagActiveRankData);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum; ++i)
	{
		pActivityRankData[i].dwRoleId    =  (*pRes)[0].GetDword();
		(*pRes)[1].GetTCHAR(pActivityRankData[i].ucRoleName, X_SHORT_NAME);
		pActivityRankData[i].n16RoleLevel = (*pRes)[2].GetDword();
        pActivityRankData[i].nDataType[0] = (*pRes)[3].GetDword();
		pActivityRankData[i].nDataType[1] = (*pRes)[4].GetDword();
		pActivityRankData[i].nDataType[2] = (*pRes)[5].GetDword();
		pActivityRankData[i].nDataType[3] = (*pRes)[6].GetDword();
		pActivityRankData[i].nDataType[4] = (*pRes)[7].GetDword();
		pActivityRankData[i].nDataType[5] = (*pRes)[8].GetDword();
		pActivityRankData[i].nDataType[6] = (*pRes)[9].GetDword();
		pActivityRankData[i].nDataType[7] = (*pRes)[10].GetDword();
         
		pRes->NextRow();
	}
}

BOOL CLoongDB::LoadForceBreakOutApprenticeList(VOID* pVoid)
{
	CLoong* pLoong = (CLoong*)pVoid;
	if (!P_VALID(pVoid))
		return FALSE;

	BOOL bRet = TRUE;

	// 获取流
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 清空流
	pStream->Clear();

	pStream->SetSelect("force_break_out_apprentice", "RoleID,MasterID,BreakoutInsertTime");

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bRet = FALSE;
		goto Exit;
	}

	// 检验并保存查询结果
	INT nResCnt = pResult->GetRowCount();
	if (nResCnt > 0)
	{
		DWORD dwMsgSize = sizeof(tagNDBS_LoadForceBreakOutApprenticeList) + (nResCnt-1)*sizeof(tagForceBreakOutApprentice);
		MCREATE_MSG(pSend, dwMsgSize, NDBS_LoadForceBreakOutApprenticeList);
		if (!P_VALID(pSend))
		{
			bRet = FALSE;
			goto Exit;
		}
		pSend->dwForceBreakOutApprenceCnt = nResCnt;
		for (INT nCnt = 0; nCnt < nResCnt; nCnt++)
		{
			pSend->sForceBreakOutApprentice[nCnt].dwApprenticeID = (*pResult)[0].GetDword();
			pSend->sForceBreakOutApprentice[nCnt].dwMasterID = (*pResult)[1].GetDword();
			pSend->sForceBreakOutApprentice[nCnt].BreakoutInsertTime = (*pResult)[2].GetDword();
			pResult->NextRow();
		}
		if (P_VALID(pSend))
		{
			pLoong->SendMsg(pSend, pSend->dwSize);
			bRet = E_Success;
			goto Exit;
			//return E_Success;
		}
	}
	else
	{
		bRet = FALSE;
	}

Exit:
	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return bRet;
}

DWORD CLoongDB::LoadGuildWarDeclare(VOID* pVoid)
{
	CLoong* pLoong = (CLoong*)pVoid;
	if (!P_VALID(pLoong))
		return FALSE;
	BOOL bRet = TRUE;

	// 获取流
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 清空流
	pStream->Clear();

	pStream->SetSelect("guild_war_declare", "AttackGuildID,DefenceGuildID,WarDeclareTime,WarEnded");

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bRet = FALSE;
		goto Exit;
	}

	// 检验并保存查询结果
	INT nResCnt = pResult->GetRowCount();
	if (nResCnt > 0)
	{
		// 创建返回消息
		DWORD dwMsgSize = sizeof(tagNDBS_LoadGuildWarDeclare) + (nResCnt-1)*sizeof(tagGuildWarDeclare);
		MCREATE_MSG(pSend, dwMsgSize, NDBS_LoadGuildWarDeclare);
		if (!P_VALID(pSend))
		{
			bRet = FALSE;
			goto Exit;
		}
		DWORD dwWarEnded = 0;
		pSend->dwGuildWarDeclareCnt = nResCnt;
		for (INT nCnt = 0; nCnt < nResCnt; nCnt++)
		{
			pSend->pGuildWarDeclare[nCnt].dwAttackGuildID = (*pResult)[0].GetDword();
			pSend->pGuildWarDeclare[nCnt].dwDefenceGuildID = (*pResult)[1].GetDword();
			pSend->pGuildWarDeclare[nCnt].dwWarDeclareTime = (*pResult)[2].GetDword();
			dwWarEnded = (*pResult)[3].GetDword();
			pSend->pGuildWarDeclare[nCnt].bWarEnded = (dwWarEnded == 0)?FALSE:TRUE;
			pResult->NextRow();
		}
		if (P_VALID(pSend))
		{
			pLoong->SendMsg(pSend, pSend->dwSize);
			bRet = E_Success;
			goto Exit;
			//return E_Success;
		}
	}
	else
	{
		bRet = FALSE;
	}

Exit:
	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return bRet;
}

DWORD CLoongDB::LoadBattleFieldAttackSignUpInfo(VOID* pVoid)
{
	CLoong* pLoong = (CLoong*)pVoid;
	if (!P_VALID(pLoong))
		return FALSE;
	BOOL bRet = TRUE;

	// 获取流
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 清空流
	pStream->Clear();

	pStream->SetSelect("battlefield_attack_signup", "BattleFieldID,GuildID,BidFund");

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bRet = FALSE;
		goto Exit;
	}

	// 检验并保存查询结果
	INT nResCnt = pResult->GetRowCount();
	if (nResCnt > 0)
	{
		DWORD dwMsgSize = sizeof(tagNDBS_LoadBattleFieldAttackSignUpInfo) + (nResCnt-1)*sizeof(tagSignUpGuild);
		MCREATE_MSG(pSend, dwMsgSize, NDBS_LoadBattleFieldAttackSignUpInfo);
		if (!P_VALID(pSend))
		{
			bRet = FALSE;
			goto Exit;
		}
		DWORD dwWarEnded = 0;
		pSend->dwInfoCnt = nResCnt;
		for (INT nCnt = 0; nCnt < nResCnt; nCnt++)
		{
			pSend->sSignUpGuild[nCnt].eGodID = static_cast<EGodMiracle>((*pResult)[0].GetDword());
			pSend->sSignUpGuild[nCnt].sSignUpGuildInfo.dwGuildID = (*pResult)[1].GetDword();
			pSend->sSignUpGuild[nCnt].sSignUpGuildInfo.dwBidFund = (*pResult)[2].GetDword();
			pResult->NextRow();
		}
		if (P_VALID(pSend))
		{
			pLoong->SendMsg(pSend, pSend->dwSize);
			bRet = E_Success;
			goto Exit;
			//return E_Success;
		}
	}
	else
	{
		bRet = FALSE;
	}

Exit:
	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return bRet;
}

DWORD CLoongDB::LoadAllicanceGuildInfo(VOID* pVoid)
{
	CLoong* pLoong = (CLoong*)pVoid;
	if (!P_VALID(pLoong))
		return FALSE;
	BOOL bRet = TRUE;

	// 获取流
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 清空流
	pStream->Clear();

	pStream->SetSelect("battlefield_alliance_signup", "BattleFieldID,GuildID,GuildCapType,BeConfirmed");

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bRet = FALSE;
		goto Exit;
	}

	// 检验并保存查询结果
	INT nResCnt = pResult->GetRowCount();
	if (nResCnt > 0)
	{
		DWORD dwMsgSize = sizeof(tagNDBS_LoadAllicanceGuildInfo) + (nResCnt-1)*sizeof(tagAllianceGuild);
		MCREATE_MSG(pSend, dwMsgSize, NDBS_LoadAllicanceGuildInfo);
		if (!P_VALID(pSend))
		{
			bRet = FALSE;
			goto Exit;
		}
		DWORD dwWarEnded = 0;
		pSend->dwInfoCnt = nResCnt;
		for (INT nCnt = 0; nCnt < nResCnt; nCnt++)
		{
			pSend->sAllianceGuild[nCnt].eGodID = static_cast<EGodMiracle>((*pResult)[0].GetDword());
			pSend->sAllianceGuild[nCnt].sAllianceGuildInfo.dwGuildID = (*pResult)[1].GetDword();
			pSend->sAllianceGuild[nCnt].sAllianceGuildInfo.eAllianceForceType = static_cast<EGuildCapType>((*pResult)[2].GetDword());
			pSend->sAllianceGuild[nCnt].sAllianceGuildInfo.bConfirmed = ((*pResult)[3].GetDword() == 1)?TRUE:FALSE;
			pResult->NextRow();
		}
		if (P_VALID(pSend))
		{
			pLoong->SendMsg(pSend, pSend->dwSize);
			bRet = E_Success;
			goto Exit;
			//return E_Success;
		}
	}
	else
	{
		bRet = FALSE;
	}

Exit:
	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return bRet;
}

DWORD CLoongDB::GetFabaoNirVanaData(DWORD pDBMsg,DWORD,tagNDBS_GetFabaoNirVanaData * pMsg)
{
	pMsg->dwErrorCode = GT_INVALID;

	if( !P_VALID(pDBMsg))
		return GT_INVALID;

	tagNDBC_GetFabaoNirVanaData * pCmd = (tagNDBC_GetFabaoNirVanaData*)pDBMsg;
	pMsg->n64NirvanID = pCmd->n64NirvanID;

	if( pCmd->nCurStage < 61 )
	{
		return GT_INVALID;
	}

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString ("select Stage,EleInjuryType,EleInjury,ElePierce,EleResisFire,EleResisWater,EleResisEarth,EleResisWind,");
		pStream->FillString("EleResisBright,EleResisDark,DamageUpgradeTimes,ResistanceUpgradeTimes, EleInjPromoteTimes,");
		pStream->FillString("EleResPromoteTimes from fabao_extend_his left join item using(SerialNum) ");
		pStream->FillString("where SerialNum = ") << pCmd->n64ID;
		pStream->FillString(" and item.OwnerID = ") << pCmd->dwRoleID;
		if( pCmd->bFirstOrLast ) // 直接退到60，或者最先保存的值
		{
			//pStream->FillString(" order by Stage asc ")/* << 60*/;
			pStream->FillString(" and Stage = ") << 60;
		}
		else	// 退到，最后一个保存的值
		{
			//pStream->FillString(" order by Stage desc ")/* << (pCmd->nCurStage - 1) / 10*/;
			INT nStage = ((pCmd->nCurStage-1) / 10) * 10;
			if( pCmd->nCurStage < 61 )
				nStage = 60;
			pStream->FillString(" and Stage = ") << nStage;
		}
		// 查询数据库
		Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
		if(!P_VALID(pResult))
		{
			m_pDBLoong->ReturnStream(pStream);
			return GT_INVALID;
		}

		INT nResCnt = pResult->GetRowCount();
		if (nResCnt > 0)
		{
			//for (INT nCnt = 0; nCnt < 1; nCnt++)
			{
				pMsg->dwErrorCode = E_Success;
				pMsg->n64ID = pCmd->n64ID;
				pMsg->dwAccountID			= pCmd->dwAccountID;
				pMsg->dwRoleID				= pCmd->dwRoleID;
				pMsg->data.nStage			= (*pResult)[0].GetInt();
				pMsg->data.nEleInjuryType	= (*pResult)[1].GetInt();
				pMsg->data.nEleInjury		= (*pResult)[2].GetInt();
				pMsg->data.nElePierce		= (*pResult)[3].GetInt();
				pMsg->data.nEleResistance[EER_Fire]	= (*pResult)[4].GetInt();
				pMsg->data.nEleResistance[EER_Water]	= (*pResult)[5].GetInt();
				pMsg->data.nEleResistance[EER_Earth]	= (*pResult)[6].GetInt();
				pMsg->data.nEleResistance[EER_Wind]	= (*pResult)[7].GetInt();
				pMsg->data.nEleResistance[EER_Bright]	= (*pResult)[8].GetInt();
				pMsg->data.nEleResistance[EER_Dark]	= (*pResult)[9].GetInt();
				pMsg->data.wDamageUpgradeTimes		= (*pResult)[10].GetInt();
				pMsg->data.wResistanceUpgradeTimes		= (*pResult)[11].GetInt();
				pMsg->data.wEleInjPromoteTimes			= (*pResult)[12].GetInt();
				pMsg->data.wEleResPromoteTimes			= (*pResult)[13].GetInt();

				//pResult->NextRow();
			}
		}

		m_pDBLoong->ReturnStream(pStream);
		m_pDBLoong->FreeQueryResult(pResult);
		return E_Success;
	}
	return GT_INVALID;
}


BOOL CLoongDB::LoadWuXunRankings(LPVOID pData, INT &nNum)
{
	return Load(pData, &nNum, GT_INVALID, GT_INVALID, &CLoongDB::FormatWuXunRankings, &CLoongDB::ProcResLoadWuXunRankings);
}



VOID CLoongDB::FormatWuXunRankings(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("wuxun_rankings", "Num, PreNum, wuxun, RoleName, RoleID");
}




VOID CLoongDB::ProcResLoadWuXunRankings(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(pRankingsNode, pData, tagRankingsNode);
	*pNum = pRes->GetRowCount();

	for(INT i=0; i<*pNum && i<MAX_RANKINGS_LEN; ++i)
	{
		// 依次取出各条记录

		INT nNum = (*pRes)[0].GetInt();
		INT nPreNum = (*pRes)[1].GetInt();
		INT64 n64Value = (INT64)(*pRes)[2].GetLong();
		TCHAR szRoleName[X_SHORT_NAME] = {0};
		(*pRes)[3].GetTCHAR(szRoleName, X_SHORT_NAME, FALSE);
		szRoleName[X_SHORT_NAME-1] = '\0';
		DWORD dwRoleID = (*pRes)[4].GetDword();

		if ( _tcscmp(szRoleName, _T("")) != 0 && nNum != 0 )
		{
			pRankingsNode[i].nNum = nNum;
			pRankingsNode[i].nPreNum = nPreNum;
			pRankingsNode[i].n64Value = (n64Value == 0)?1:n64Value;
			pRankingsNode[i].dwRoleID = dwRoleID;
			_tcscpy_s(pRankingsNode[i].szRoleName, X_SHORT_NAME, szRoleName);
		}

		pRes->NextRow();
	}
}

DWORD CLoongDB::LoadGodBidInfo(VOID* pVoid)
{
	CLoong* pLoong = (CLoong*)pVoid;
	if (!P_VALID(pLoong))
		return FALSE;
	BOOL bRet = TRUE;

	// 获取流
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 清空流
	pStream->Clear();
	pStream->SetSelect("god_bid", "RoleID,Bidden,BidType,BidID");

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bRet = FALSE;
		goto Exit;
	}

	// 检验并保存查询结果
	INT nResCnt = pResult->GetRowCount();
	if (nResCnt > 0)
	{
		DWORD dwMsgSize = sizeof(tagNDBS_LoadGodBidInfo) + (nResCnt-1)*sizeof(tagGodBidInfo2DB);
		MCREATE_MSG(pSend, dwMsgSize, NDBS_LoadGodBidInfo);
		if (!P_VALID(pSend))
		{
			bRet = FALSE;
			goto Exit;
		}
		DWORD dwWarEnded = 0;
		pSend->dwBidCount = nResCnt;
		for (INT nCnt = 0; nCnt < nResCnt; nCnt++)
		{
			pSend->GodBidInfo2DB[nCnt].dwRoleID = (*pResult)[0].GetDword();
			pSend->GodBidInfo2DB[nCnt].n64Bidden = (*pResult)[1].GetLong();
			pSend->GodBidInfo2DB[nCnt].byBidType = ((*pResult)[2].GetByte());
			pSend->GodBidInfo2DB[nCnt].dwBidID = ((*pResult)[3].GetDword());
			pResult->NextRow();
		}
		if (P_VALID(pSend))
		{
			pLoong->SendMsg(pSend, pSend->dwSize);
			return E_Success;
		}
	}
	else
	{
		bRet = FALSE;
	}

Exit:
	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return bRet;
}

BOOL CLoongDB::LoadClergyMarsRank(LPVOID pData, INT &nNum)
{
	nNum = 0;
	tagClergyNode* pTmpNode = (tagClergyNode*) pData;

	// 取女性第一名
	Beton::QueryResult* pResult1 = m_pDBLoong->Query("select * from Clergy_Mars where Clergy_Mars.Num = (select min(Num) from Clergy_Mars where sex = 0 and Clergy4SeniorOrJunior = 2)");
	if(!P_VALID(pResult1))
	{
		return FALSE;
	}

	// 检验并保存查询结果
	if (pResult1->GetRowCount() == 1)
	{
		pTmpNode->nNum = (*pResult1)[0].GetInt();
		pTmpNode->dwRoleID = (*pResult1)[1].GetDword();
		pTmpNode->dwPoint = (*pResult1)[4].GetDword();
		pTmpNode->bRecv = (*pResult1)[6].GetBool();
		pTmpNode->eClergy = ECT_WomenChamp;
	}

	nNum++;
	
	m_pDBLoong->FreeQueryResult(pResult1);

	// 指针后移
	pTmpNode++;
	
	// 取男性第一名
	Beton::QueryResult* pResult2 = m_pDBLoong->Query("select * from Clergy_Mars where Clergy_Mars.Num = (select min(Num) from Clergy_Mars where sex = 1 and Clergy4SeniorOrJunior = 2)");
	if(!P_VALID(pResult2))
	{
		return FALSE;
	}

	// 检验并保存查询结果
	if (pResult2->GetRowCount() == 1)
	{
		pTmpNode->nNum = (*pResult2)[0].GetInt();
		pTmpNode->dwRoleID = (*pResult2)[1].GetDword();
		pTmpNode->dwPoint = (*pResult2)[4].GetDword();
		pTmpNode->bRecv = (*pResult1)[6].GetBool();
		pTmpNode->eClergy = ECT_MenChamp;
	}

	nNum++;

	m_pDBLoong->FreeQueryResult(pResult2);

	// 指针后移
	pTmpNode++;
	
	// 选取其余八个神职获得者
	Beton::QueryResult* pResult3 = m_pDBLoong->Query("select * from Clergy_Mars limit 10");
	if(!P_VALID(pResult3))
	{
		return FALSE;
	}

	INT nResCnt = pResult3->GetRowCount();
	//nNum += nResCnt;
	if (nResCnt > 0)
	{
		for (INT nCnt = 0; nCnt < nResCnt; nCnt++)
		{
			DWORD dwRoleID = (*pResult3)[1].GetDword();
			if ( dwRoleID != ((tagClergyNode*)pData)->dwRoleID && dwRoleID != (((tagClergyNode*)pData+1))->dwRoleID )
			{
				nNum++;
				pTmpNode->nNum = (*pResult3)[0].GetInt();
				pTmpNode->dwRoleID = dwRoleID;
				pTmpNode->dwPoint = (*pResult3)[4].GetDword();
				pTmpNode->bRecv = (*pResult1)[6].GetBool();
				if(nNum<=MAX_CLERGY_WINNER)
					pTmpNode->eClergy = ECT_Winner;
				else
					pTmpNode->eClergy = ECT_Loser;

				if(nNum >= MAX_CampaignInfoNumPerType) break;

				pTmpNode++;
			}

			pResult3->NextRow();
		}
	}
// 	else
// 	{
// 		return FALSE;
// 	}

	m_pDBLoong->FreeQueryResult(pResult3);
	
	if (nNum > MAX_CampaignInfoNumPerType)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CLoongDB::LoadClergyApolloRank(LPVOID pData, INT &nNum)
{
	nNum = 0;
	tagClergyNode* pTmpNode = (tagClergyNode*) pData;

	// 取女性第一名
	Beton::QueryResult* pResult1 = m_pDBLoong->Query("select * from Clergy_Apollo where Clergy_Apollo.Num = (select min(Num) from Clergy_Apollo where sex = 0 and Clergy4SeniorOrJunior = 2)");
	if(!P_VALID(pResult1))
	{
		return FALSE;
	}

	// 检验并保存查询结果
	if (pResult1->GetRowCount() == 1)
	{
		pTmpNode->nNum = (*pResult1)[0].GetInt();
		pTmpNode->dwRoleID = (*pResult1)[1].GetDword();
		pTmpNode->dwPoint = (*pResult1)[4].GetDword();
		pTmpNode->bRecv = (*pResult1)[6].GetBool();
		pTmpNode->eClergy = ECT_WomenChamp;
	}

	nNum++;

	m_pDBLoong->FreeQueryResult(pResult1);

	// 指针后移
	pTmpNode++;
	
	// 取男性第一名
	Beton::QueryResult* pResult2 = m_pDBLoong->Query("select * from Clergy_Apollo where Clergy_Apollo.Num = (select min(Num) from Clergy_Apollo where sex = 1 and Clergy4SeniorOrJunior = 2)");
	if(!P_VALID(pResult2))
	{
		return FALSE;
	}

	// 检验并保存查询结果
	if (pResult2->GetRowCount() == 1)
	{
		pTmpNode->nNum = (*pResult2)[0].GetInt();
		pTmpNode->dwRoleID = (*pResult2)[1].GetDword();
		pTmpNode->dwPoint = (*pResult2)[4].GetDword();
		pTmpNode->bRecv = (*pResult1)[6].GetBool();
		pTmpNode->eClergy = ECT_MenChamp;
	}

	nNum++;

	m_pDBLoong->FreeQueryResult(pResult2);

	// 指针后移
	pTmpNode++;
	
	// 选取其余八个神职获得者
	Beton::QueryResult* pResult3 = m_pDBLoong->Query("select * from Clergy_Apollo limit 10");
	if(!P_VALID(pResult3))
	{
		return FALSE;
	}

	INT nResCnt = pResult3->GetRowCount();
	if (nResCnt > 0)
	{
		for (INT nCnt = 0; nCnt < nResCnt; nCnt++)
		{
			DWORD dwRoleID = (*pResult3)[1].GetDword();
			if ( dwRoleID != ((tagClergyNode*)pData)->dwRoleID && dwRoleID != (((tagClergyNode*)pData+1))->dwRoleID )
			{
				nNum++;
				pTmpNode->nNum = (*pResult3)[0].GetInt();
				pTmpNode->dwRoleID = dwRoleID;
				pTmpNode->dwPoint = (*pResult3)[4].GetDword();
				pTmpNode->bRecv = (*pResult1)[6].GetBool();
				if(nNum<=MAX_CLERGY_WINNER)
					pTmpNode->eClergy = ECT_Winner;
				else
					pTmpNode->eClergy = ECT_Loser;

				if(nNum >= MAX_CampaignInfoNumPerType) break;

				pTmpNode++;
			}
			pResult3->NextRow();
			
		}
	}
// 	else
// 	{
// 		return FALSE;
// 	}

	m_pDBLoong->FreeQueryResult(pResult3);

	if (nNum > MAX_CampaignInfoNumPerType)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CLoongDB::LoadClergyRabbiRank(LPVOID pData, INT &nNum)
{
	nNum = 0;
	tagClergyNode* pTmpNode = (tagClergyNode*) pData;

	// 取女性第一名
	Beton::QueryResult* pResult1 = m_pDBLoong->Query("select * from Clergy_Rabbi where Clergy_Rabbi.Num = (select min(Num) from Clergy_Rabbi where sex = 0 and Clergy4SeniorOrJunior = 2)");
	if(!P_VALID(pResult1))
	{
		return FALSE;
	}

	// 检验并保存查询结果
	if (pResult1->GetRowCount() == 1)
	{
		pTmpNode->nNum = (*pResult1)[0].GetInt();
		pTmpNode->dwRoleID = (*pResult1)[1].GetDword();
		pTmpNode->dwPoint = (*pResult1)[4].GetDword();
		pTmpNode->bRecv = (*pResult1)[6].GetBool();
		pTmpNode->eClergy = ECT_WomenChamp;
	}

	nNum++;

	m_pDBLoong->FreeQueryResult(pResult1);

	// 指针后移
	pTmpNode++;
	
	// 取男性第一名
	Beton::QueryResult* pResult2 = m_pDBLoong->Query("select * from Clergy_Rabbi where Clergy_Rabbi.Num = (select min(Num) from Clergy_Rabbi where sex = 1 and Clergy4SeniorOrJunior = 2)");
	if(!P_VALID(pResult2))
	{
		return FALSE;
	}

	// 检验并保存查询结果
	if (pResult2->GetRowCount() == 1)
	{
		pTmpNode->nNum = (*pResult2)[0].GetInt();
		pTmpNode->dwRoleID = (*pResult2)[1].GetDword();
		pTmpNode->dwPoint = (*pResult2)[4].GetDword();
		pTmpNode->bRecv = (*pResult2)[6].GetBool();
		pTmpNode->eClergy = ECT_MenChamp;
	}

	nNum++;

	m_pDBLoong->FreeQueryResult(pResult2);

	// 指针后移
	pTmpNode++;
	
	// 选取其余八个神职获得者
	Beton::QueryResult* pResult3 = m_pDBLoong->Query("select * from Clergy_Rabbi limit 10");
	if(!P_VALID(pResult3))
	{
		return FALSE;
	}

	INT nResCnt = pResult3->GetRowCount();
	if (nResCnt > 0)
	{
		for (INT nCnt = 0; nCnt < nResCnt; nCnt++)
		{
			DWORD dwRoleID = (*pResult3)[1].GetDword();
			if ( dwRoleID != ((tagClergyNode*)pData)->dwRoleID && dwRoleID != (((tagClergyNode*)pData+1))->dwRoleID )
			{
				nNum++;
				pTmpNode->nNum = (*pResult3)[0].GetInt();
				pTmpNode->dwRoleID = dwRoleID;
				pTmpNode->dwPoint = (*pResult3)[4].GetDword();
				pTmpNode->bRecv = (*pResult3)[6].GetBool();
				if(nNum<=MAX_CLERGY_WINNER)
					pTmpNode->eClergy = ECT_Winner;
				else
					pTmpNode->eClergy = ECT_Loser;

				if(nNum >= MAX_CampaignInfoNumPerType) break;


				pTmpNode++;
			}
			pResult3->NextRow();
		}
	}
// 	else
// 	{
// 		return FALSE;
// 	}

	m_pDBLoong->FreeQueryResult(pResult3);

	if (nNum > MAX_CampaignInfoNumPerType)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CLoongDB::LoadClergyPeaceRank(LPVOID pData, INT &nNum)
{
	nNum = 0;
	tagClergyNode* pTmpNode = (tagClergyNode*) pData;

	// 取女性第一名
	Beton::QueryResult* pResult1 = m_pDBLoong->Query("select * from Clergy_Peace where Clergy_Peace.Num = (select min(Num) from Clergy_Peace where sex = 0 and Clergy4SeniorOrJunior = 2)");
	if(!P_VALID(pResult1))
	{
		return FALSE;
	}

	// 检验并保存查询结果
	if (pResult1->GetRowCount() == 1)
	{
		pTmpNode->nNum = (*pResult1)[0].GetInt();
		pTmpNode->dwRoleID = (*pResult1)[1].GetDword();
		pTmpNode->dwPoint = (*pResult1)[4].GetDword();
		pTmpNode->bRecv = (*pResult1)[6].GetBool();
		pTmpNode->eClergy = ECT_WomenChamp;
	}

	nNum++;

	m_pDBLoong->FreeQueryResult(pResult1);

	// 指针后移
	pTmpNode++;
	

	// 取男性第一名
	Beton::QueryResult* pResult2 = m_pDBLoong->Query("select * from Clergy_Peace where Clergy_Peace.Num = (select min(Num) from Clergy_Peace where sex = 1 and Clergy4SeniorOrJunior = 2)");
	if(!P_VALID(pResult2))
	{
		return FALSE;
	}

	// 检验并保存查询结果
	if (pResult2->GetRowCount() == 1)
	{
		pTmpNode->nNum = (*pResult2)[0].GetInt();
		pTmpNode->dwRoleID = (*pResult2)[1].GetDword();
		pTmpNode->dwPoint = (*pResult2)[4].GetDword();
		pTmpNode->bRecv = (*pResult1)[6].GetBool();
		pTmpNode->eClergy = ECT_MenChamp;
	}

	nNum++;

	m_pDBLoong->FreeQueryResult(pResult2);

	// 指针后移
	pTmpNode++;
	

	// 选取其余八个神职获得者
	Beton::QueryResult* pResult3 = m_pDBLoong->Query("select * from Clergy_Peace limit 10");
	if(!P_VALID(pResult3))
	{
		return FALSE;
	}

	INT nResCnt = pResult3->GetRowCount();
	if (nResCnt > 0)
	{
		for (INT nCnt = 0; nCnt < nResCnt; nCnt++)
		{
			DWORD dwRoleID = (*pResult3)[1].GetDword();
			if ( dwRoleID != ((tagClergyNode*)pData)->dwRoleID && dwRoleID != (((tagClergyNode*)pData+1))->dwRoleID )
			{
				nNum++;
				pTmpNode->nNum = (*pResult3)[0].GetInt();
				pTmpNode->dwRoleID = dwRoleID;
				pTmpNode->dwPoint = (*pResult3)[4].GetDword();
				pTmpNode->bRecv = (*pResult1)[6].GetBool();
				if(nNum<=MAX_CLERGY_WINNER)
					pTmpNode->eClergy = ECT_Winner;
				else
					pTmpNode->eClergy = ECT_Loser;

				if(nNum >= MAX_CampaignInfoNumPerType) break;

				pTmpNode++;
			}
			pResult3->NextRow();
			
		}
	}
// 	else
// 	{
// 		return FALSE;
// 	}

	m_pDBLoong->FreeQueryResult(pResult3);

	if (nNum > MAX_CampaignInfoNumPerType)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CLoongDB::ChangeClergyCandidateRecvFlag(ECampaignGodType eType, DWORD dwRoleID)
{
	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( !P_VALID(pStream) ) return GT_INVALID;

	pStream->Clear();
	switch(eType)
	{
	case ECGT_War:
		pStream->SetUpdate("Clergy_Mars");
		break;
	case ECGT_Sun:
		pStream->SetUpdate("Clergy_Apollo");
		break;
	case ECGT_Mana:
		pStream->SetUpdate("Clergy_Rabbi");
		break;
	case ECGT_Peace:
		pStream->SetUpdate("Clergy_Peace");
	}

	// 格式化数据
	pStream->FillString("RecvOrNot=1");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	BOOL bRet = m_pDBLoong->Execute(pStream);
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::GetClergyCandidateCount(DWORD* dwCount)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( !P_VALID(pStream) ) return GT_INVALID;
	
	pStream->Clear();
	pStream->FillString ("select * from roledata where ClergyCandidateType=0");

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		m_pDBLoong->ReturnStream(pStream);
		return GT_INVALID;
	}

	dwCount[ECGT_War] = pResult->GetRowCount();
	m_pDBLoong->FreeQueryResult(pResult);
	
	//-------------------------------------------------------
	pStream->Clear();
	pStream->FillString ("select * from roledata where ClergyCandidateType=1");

	// 查询数据库
	pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		m_pDBLoong->ReturnStream(pStream);
		return GT_INVALID;
	}

	dwCount[ECGT_Sun] = pResult->GetRowCount();
	m_pDBLoong->FreeQueryResult(pResult);

	//-------------------------------------------------------
	pStream->Clear();
	pStream->FillString ("select * from roledata where ClergyCandidateType=2");

	// 查询数据库
	pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		m_pDBLoong->ReturnStream(pStream);
		return GT_INVALID;
	}

	dwCount[ECGT_Mana] = pResult->GetRowCount();
	m_pDBLoong->FreeQueryResult(pResult);

	//-------------------------------------------------------
	pStream->Clear();
	pStream->FillString ("select * from roledata where ClergyCandidateType=3");

	// 查询数据库
	pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		m_pDBLoong->ReturnStream(pStream);
		return GT_INVALID;
	}

	dwCount[ECGT_Peace] = pResult->GetRowCount();

	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return E_Success;	
}

DWORD CLoongDB::CancelPreClergy(INT nNum, DWORD* dwRoleIDs)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( !P_VALID(pStream) ) return GT_INVALID;

	for(int i=0; i<nNum; ++i)
	{
		pStream->Clear();
		pStream->FillString("update roledata set Clergy = PreClergy where RoleID=")<<dwRoleIDs[i];
		m_pDBLoong->Execute(pStream);
	}
	
	m_pDBLoong->ReturnStream(pStream);
	return E_Success;
}
//-------------------------------------------------------------------------------------------------------
// 读取角色领域信息
BOOL CLoongDB::LoadArea(LPVOID &pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadArea, &CLoongDB::ProcResLoadArea);
}
//-------------------------------------------------------------------------------------------------------
// 读取角色领域信息SQL语句格式化
VOID CLoongDB::FormatLoadArea(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("area", "AreaID,RoleID,Activation,CDTime");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}
//-------------------------------------------------------------------------------------------------------
// 读取角色领域信息结果检查及保存
VOID CLoongDB::ProcResLoadArea(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);
	
	MTRANS_POINTER(pArea, pData, tagAreaSave);
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		pArea[i].dwAreaID		= (*pRes)[0].GetDword();
		pArea[i].dwRoleID		= (*pRes)[1].GetInt();
		pArea[i].bActivation    = (*pRes)[2].GetInt();
		pArea[i].dwCD			= (*pRes)[3].GetInt();
		pRes->NextRow();
	}

	pData = &pArea[*pNum];
}
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::LoadBuyMallItemCount(LPVOID& pData, INT32 &nNum, DWORD dwRoleID)
{
	return Load(pData, &nNum, GT_INVALID, dwRoleID, &CLoongDB::FormatLoadBuyMallItemCount, &CLoongDB::ProcResLoadBuyMallItemCount);
}

VOID CLoongDB::FormatLoadBuyMallItemCount(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("role_buy_mall_item_count", "MallItemID,Num");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

VOID CLoongDB::ProcResLoadBuyMallItemCount(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	ASSERT(pNum != NULL);

	MTRANS_POINTER(p, pData, tagBuyMallItemCountSaveData);
	*pNum = pRes->GetRowCount();

	for(INT32 i=0; i<*pNum; ++i)
	{
		p[i].dwTypeID	= (*pRes)[0].GetDword();
		p[i].nNum = (*pRes)[1].GetInt();
		
		pRes->NextRow();
	}

	pData = &p[*pNum];
}

VOID CLoongDB::UpdateWorldDataLog(LPCSTR szTablePostfix, LPVOID pData)
{
	Update(NULL, pData, 1, 0, (&CLoongDB::FormatUpdateWorldDataLog));
}

BOOL CLoongDB::LoadWorldData(LPVOID p, INT32& nNum)
{
	nNum = 0;
	BOOL bRet = Load(p, &nNum, GT_INVALID, GT_INVALID,&CLoongDB::FormatLoadWorldData, &CLoongDB::ProcResLoadWorldData);
	return bRet && (p != NULL);
}

BOOL CLoongDB::InsertWorldData()
{
	INT32 nNum = 0;
	LPVOID p = NULL;
	return Load(p, &nNum, GT_INVALID, GT_INVALID,&CLoongDB::FormatInsertWorldDataLog, &CLoongDB::ProcResInsertWorldDataLog);
}

VOID CLoongDB::ProcResLoadWorldData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	//ASSERT(pNum != NULL && *pNum == 1);

	MTRANS_POINTER(p, pData, DWORD);
	*pNum = pRes->GetRowCount();
	
	if (*pNum != 1)
	{
		pData = NULL;
		return;
	}

	for(INT32 i=0; i<DataCollection_Max_Data_Num; ++i)
	{
		p[i]	= (*pRes)[i+1].GetDword();
	}

	//pData = &p[1];
}

VOID CLoongDB::InsertRoleDataLog(LPCSTR szTablePostfix, DWORD dwRoleID, LPVOID pData)
{
	Update(dwRoleID, pData, 1, 0, (&CLoongDB::FormatInsertRoleDataLog));
}

BOOL CLoongDB::LoadRoleLogData(LPVOID p, DWORD dwRoleID)
{
	INT32 nNum = 0;
	BOOL bRet = Load(p, &nNum, GT_INVALID, dwRoleID,&CLoongDB::FormatLoadRoleLogData, &CLoongDB::ProcResLoadRoleLogData);
	
	if (FALSE==bRet)
	{
		Update(dwRoleID,NULL, 1, 0, &CLoongDB::FormatInsertNewRoleLogData);
	}

	return bRet;
}

BOOL CLoongDB::LoadRoleTigerData(INT64 &n64)
{
	BOOL bRet = TRUE;

	// 获取流
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 清空流
	pStream->Clear();
	pStream->SetSelect("gm_server_data", "data_value");
	pStream->SetWhere();
	pStream->FillString("data_name='TigerProfit'");

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bRet = FALSE;
		goto Exit;
	}

	// 检验并保存查询结果
	INT nResCnt = pResult->GetRowCount();
	if (nResCnt > 0)
	{
		n64= (*pResult)[0].GetLong(); 
	}
	else
	{
		bRet = FALSE;
	}

Exit:
	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return bRet;
}

VOID CLoongDB::ProcResLoadRoleLogData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, DWORD);
	*pNum = pRes->GetRowCount();

	if (*pNum != 1)
	{
		pData = NULL;
		return;
	}

	for(INT32 i=0; i<DataCollection_Max_RoleData_Num; ++i)
	{
		p[i]	= (*pRes)[i+1].GetDword();
	}
}

//-------------------------------------------------------------------------------------------------------
// 结拜
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::ClearTeatherInfo(DWORD dwRoleID,DWORD dwBrotherID)
{
	LPVOID pData;
	INT32 nNum;
	return Load(pData, &nNum, dwRoleID, dwBrotherID, &CLoongDB::FormatLoadBloodBrother, &CLoongDB::ProcResLoadBloodBrother);
}

VOID CLoongDB::FormatLoadBloodBrother(Beton::MyStream *pStream, DWORD dwAccountID, DWORD dwRoleID)
{
	pStream->SetSelect("roledata", "RoleID");
	pStream->SetWhere();
	pStream->FillString("BrotherTeacherID=") << dwRoleID;
	pStream->FillString(" and RoleID=") << dwAccountID;
}

VOID CLoongDB::ProcResLoadBloodBrother(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(p, pData, tagFamilyLoad);

	*pNum = pRes->GetRowCount();
	if( 0 == *pNum )	return ;

	DWORD dwRoleID = (*pRes)[0].GetDword();

	Update(dwRoleID, NULL, 1, GT_INVALID, (&CLoongDB::FormatClearBloodBrother));
	
}

VOID CLoongDB::FormatLoadRoleTigerData(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
	pStream->Clear();
	pStream->SetSelect("gm_server_data", "data_value");
	pStream->SetWhere();
	pStream->FillString("data_name='TigerProfit'");

}

VOID CLoongDB::ProcResLoadRoleTigerData(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	if (pRes->GetRowCount() <= 0)
		return;
	
	*(DWORD*)pData= (*pRes)[0].GetDword(); 
}

BOOL CLoongDB::LoadAccountReactiveInfo(DWORD dwAccountID, tagAccountReactiveInfo* pData)
{
	if(P_VALID(pData))
		ZeroMemory(pData, sizeof(*pData));
	else
		return FALSE;

	// 获取流
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream))
	{
		ASSERT(P_VALID(pStream));
		return FALSE;
	}
	
	// 清空流
	pStream->Clear();
	pStream->SetSelect("Account_Reactive", "*");
	pStream->SetWhere();
	pStream->FillString("accountid=") << dwAccountID;

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		m_pDBLoong->ReturnStream(pStream);
		return FALSE;
	}

	// 检验并保存查询结果
	INT nResCnt = pResult->GetRowCount();
	if (nResCnt > 0)
	{
		pData->eIdentity = EARI_Intro;
		pData->dwKey = (*pResult)[1].GetDword();
		pData->nScore = (*pResult)[2].GetInt();
		pData->byRewardLevel = (*pResult)[3].GetShort();
	}
	else
	{
		pStream->Clear();
		pStream->SetSelect("Account_Reactiver", "accountid");
		pStream->SetWhere();
		pStream->FillString("accountid=") << dwAccountID;

		// 查询数据库
		Beton::QueryResult* pSResult = m_pDBLoong->Query(pStream);
		if(!P_VALID(pSResult))
		{
			m_pDBLoong->ReturnStream(pStream);
			m_pDBLoong->FreeQueryResult(pResult);
			return FALSE;
		}

		INT nSResCnt = pSResult->GetRowCount();
		if (nSResCnt > 0)
		{
			pData->eIdentity = EARI_BeIntro;
		}
		else
		{
			pData->eIdentity = EARI_Null;
		}

		m_pDBLoong->FreeQueryResult(pSResult);
	}

	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return TRUE;
}