//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: LoongDBSave.cpp
// author: Sxg
// actor:
// data: 2008-05-12
// last:
// brief: 数据库操作应用层实现 -- 保存数据相关
//-------------------------------------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/QuestDef.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/selectrole.h"
#include "../WorldDefine/vcard_define.h"
#include "../WorldDefine/quest_board.h"
#include "../WorldDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_item.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/clandata_define.h"
#include "../ServerDefine/item_define.h"
#include "../ServerDefine/activity_define.h"
#include "../ServerDefine/msg_city.h"
#include "../ServerDefine/master_apprentice.h"
#include "../ServerDefine/msg_shortcut_quickbar.h"
#include "../ServerDefine/athletics_data_define.h"
#include "../ServerDefine/msg_draft_show.h"
#include "../ServerDefine/msg_common.h"
#include "../WorldDefine/guild_define.h"
#include "../WorldDefine/msg_quest_board.h"
#include "../WorldDefine/godbid_define.h"
#include "../ServerDefine/msg_godbid.h"
#include "../ServerDefine/msg_eggbroker.h"
#include "../ServerDefine/msg_fashion.h"
#include "../WorldDefine/mall_define.h"
#include "../ServerDefine/msg_holySoul.h"

#include "LoongDB.h"

//-------------------------------------------------------------------------------------------------------
// 删除物品装备相关定义
//-------------------------------------------------------------------------------------------------------
enum EDelItemTable
{
	EDIT_Item		= 0,	// item表
	EDIT_ItemBaiBao	= 1,	// item_baibao表
};

struct tagDelItem
{
	INT64	n64Serial;
	CHAR*	pTable;
};
struct tagDelItemFromRole
{
	DWORD dwRoleID;
	DWORD dwTypeID;
	CHAR* pTable;
};

struct tagNewItemEquip
{
	union
	{
		const tagItem*	pItem;
		const tagEquip* pEquip;
	};

	const CHAR*		pTable;
};


//#define MGET_TABLE(pTable, pDelItem)		\
//	char *pTable = "item";					\
//	if(EDIT_ItemBaiBao == pDelItem->eTable)	\
//		pTable = "item_baibao";

////-------------------------------------------------------------------------------------------------------
//// 删除玩家的buff，功夫，item/equip等.保存玩家数据前用于清除多余的记录
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::DeleteRecord(DWORD dwRoleID, tagRecordDel *pRecord)
//{
//	/*************************************************************************************
//	* 依次删除各表中指定的记录: 技能列表，状态列表，物品列表，装备列表，任务列表
//	**************************************************************************************/
//	
//	BYTE *pData = pRecord->byData; // 记录当前指针位置
//
//	// 1.技能列表
//	Update(dwRoleID, pData, pRecord->nSkillNum, sizeof(DWORD), (&CLoongDB::FormatDelRecSkill));
//	pData += pRecord->nSkillNum * sizeof(DWORD);
//
//	// 2.状态列表
//	Update(dwRoleID, pData, pRecord->nBuffNum, sizeof(DWORD), (&CLoongDB::FormatDelRecBuff));
//	pData += pRecord->nBuffNum * sizeof(DWORD);
//
//	//// 4.物品列表
//	//Update(dwRoleID, pData, pRecord->nItemNum, sizeof(INT64), (&CLoongDB::FormatDelRecItem));
//	//pData += pRecord->nItemNum * sizeof(INT64);
//
//	//// 5.装备列表
//	//Update(dwRoleID, pData, pRecord->nEquipNum, sizeof(INT64), (&CLoongDB::FormatDelRecEquip));
//	//pData += pRecord->nEquipNum * sizeof(INT64);
//
//	// 6.任务列表
//	Update(dwRoleID, pData, pRecord->nQuestNum, sizeof(DWORD), (&CLoongDB::FormatDelRecTask));
//	pData += pRecord->nQuestNum * sizeof(DWORD);
//}


//-------------------------------------------------------------------------------------------------------
// 存储角色相关信息 -- 除需即时存储的，其他属性均在此处初始化存储数据
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::SaveRole(DWORD dwRoleID, tagRoleDataSave *pRoleData)
{
	// 保存角色基本属性信息
	SaveRoleData(dwRoleID, pRoleData);

	/**************************************************************************
	* 依次保存: 技能列表，状态列表，称号列表，物品列表，装备列表，
	*			好友列表，仇敌列表，任务列表，已完成任务列表， 物品冷却时间表，
	*			
	***************************************************************************/

	LPVOID pData = pRoleData->byData; // 记录当前指针位置

	// 1.技能列表
	SaveSkill(dwRoleID, pData, pRoleData->nSkillNum, &pData);

	// 2.状态列表
	SaveRoleBuff(dwRoleID, pData, pRoleData->nBuffNum, &pData);

	// 4.称号列表
	SaveTitle(dwRoleID, pData, pRoleData->nTitleNum, &pData);

	// 5.物品列表 6.装备列表 // 分消息保存
	//SaveItemEquip(dwRoleID, pData, pRoleData->nItemNum, &pData);
	UpdateItemPosAndUseRel(dwRoleID, pData, pRoleData->nItemNum, &pData);

	// 7.好友列表 
	// 8.仇敌列表 // 这2个表中的数据是即时保存的

	// 9.任务列表
	SaveTask(dwRoleID, pData, pRoleData->nQuestNum, &pData);

	// 10.已完成任务列表
	//SaveTaskDone(dwRoleID, pData, pRoleData->nQuestDoneNum);

	// 11.物品冷却时间表
	ReplaceItemCDTime(dwRoleID, pData, pRoleData->nItemCDTimeNum, &pData);

	// 12.保存氏族数据
	SaveClanData(dwRoleID, pData, pRoleData->bClanDataChg, &pData);

	// 13.保存宠物
	SavePets(dwRoleID, pData, pRoleData->nPetsNum, &pData);

	// 14.保存师徒
	SaveMaster(dwRoleID, pData, &pData);

	// 任务板任务
// 	SaveBoardQuest(dwRoleID, pData, pRoleData->nBoardQuestNum, &pData);

	// 15.保存神系统领域
	SaveArea(dwRoleID, pData, pRoleData->nAreaNum, &pData);

	// 16.商城限量物品购买计数
	SaveBuyMallItemCount(dwRoleID, pData, pRoleData->nBuyMallItemLogCount, &pData);


	// 17.元神
	//if ( pRoleData->bIsHolySoulActivated )
	//{
	//	SaveHolySoulInfo(pData,dwRoleID);
	//}
}


//********************************************************************************************************
//-------------------------------------------------------------------------------------------------------
// 插入新物品
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertItem(const tagItem &item, const CHAR* pTable)
{
	tagNewItemEquip sNewItem;
	sNewItem.pItem	= &item;
	sNewItem.pTable	= pTable;
	return Update(GT_INVALID, (LPVOID)&sNewItem, 1, SIZE_ITEM, (&CLoongDB::FormatInsertItem));
}

//-------------------------------------------------------------------------------------------------------
// 插入新装备专用属性
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertEquipSpec(const tagEquip &equip, const CHAR* pTable)
{
	tagNewItemEquip sNewItem;
	sNewItem.pEquip	= &equip;
	sNewItem.pTable	= pTable;
	return Update(GT_INVALID, (LPVOID)&sNewItem, 1, SIZE_EQUIP, (&CLoongDB::FormatInsertEquipSpec));
}

//-------------------------------------------------------------------------------------------------------
// 插入新装备 -- 装备要更新item表和equip表，需要保证数据的完整性
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertEquip(const tagEquip &equip, BOOL bBaiBao/* = FALSE*/)
{
	CHAR* pTableItem	= "item";
	CHAR* pTableEquip	= "equip";

	if(bBaiBao)
	{
		pTableItem	= "item_baibao";
		pTableEquip = "equip_baibao";
	}
	
	BOOL bRes = InsertItem(equip, pTableItem);
	if(!bRes)
	{
		return bRes;
	}

	bRes = InsertEquipSpec(equip, pTableEquip);
	if(!bRes)
	{
		// equip表插入失败，从item表中删除相应记录
		DelItem(equip.n64Serial, pTableItem);
	}

	return bRes;
}

// Jason 2010-4-14 v2.0.0
BOOL CLoongDB::InsertFabao(const tagFabao & fabao, BOOL bBaiBao )
{
	InsertEquip(fabao,bBaiBao);
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();

		InitFabaoUpdate(TRUE,pStream,pCon,&fabao);

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}
BOOL CLoongDB::SaveFabao(const tagFabao & fabao)
{
	if( SaveEquip(fabao) )
	{
		Beton::MyStream* pStream = m_pDBLoong->GetStream();
		if( P_VALID(pStream) )
		{
			Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
			if( !P_VALID(pCon) )
			{
				m_pDBLoong->ReturnStream(pStream);
				return FALSE;
			}

			BOOL r = TRUE;
			pStream->Clear();

			InitFabaoUpdate(FALSE,pStream,pCon,&fabao);

			pStream->FillString(" WHERE SerialNum=") << fabao.n64Serial;

			if( !m_pDBLoong->Execute(pStream) )
				r = FALSE;
			m_pDBLoong->ReturnStream(pStream);

			m_pDBLoong->ReturnConnection(pCon);
			return r;
		}
	}
	return FALSE;
}
BOOL CLoongDB::UpdateFabaoSpec(INT64 id,const tagFabaoSpec & fabao)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();
		pStream->FillString  ("UPDATE fabao SET ") ;
		InitFabaoSpecUpdate(pStream,pCon,(tagFabaoSpec*)&fabao);
		pStream->FillString(" WHERE SerialNum=") << id;

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}

BOOL CLoongDB::InsertSoulCrystal(const tagSoulCrystal & soul, BOOL bBaiBao )
{
	InsertEquip(soul,bBaiBao);
	
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		
		pStream->Clear();
		pStream->SetInsert("soulcrystal");
		pStream->FillString("SerialNum=") << soul.n64Serial;
		pStream->FillString(",SoulLevel=") << soul.SoulSpec.bySoulCrystalLvl; 
		pStream->FillString(",SoulQlty=") << soul.SoulSpec.bySoulCrystalQlty;
		pStream->FillString(",SoulSkillID=") << soul.SoulSpec.dwStaticSkillID;
		for (int i=0; i<SOUL_CRYSTAL_ATT_MAX_NUM; ++i)
		{
			TCHAR szTmp[X_SHORT_STRING] = {0};
			_sntprintf(szTmp, X_SHORT_STRING, _T(",SoulAttID%d="), i+1);
			pStream->FillString(szTmp)<<soul.SoulSpec.nSoulAttID[i];
		}

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		
		m_pDBLoong->ReturnStream(pStream);
		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}

//BOOL SaveFabao(const tagFabao & fabao);
BOOL CLoongDB::UpdateSoulCrystalSpec(INT64 id,const tagSoulCrystalSpec & SoulSpec)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();
		pStream->FillString  ("UPDATE soulcrystal SET ") ;
		pStream->FillString("SoulLevel=") << SoulSpec.bySoulCrystalLvl; 
		pStream->FillString(",SoulQlty=") << SoulSpec.bySoulCrystalQlty;
		pStream->FillString(",SoulSkillID=") << SoulSpec.dwStaticSkillID;
		for (int i=0; i<SOUL_CRYSTAL_ATT_MAX_NUM; ++i)
		{
			TCHAR szTmp[X_SHORT_STRING] = {0};
			_sntprintf(szTmp, X_SHORT_STRING, _T(",SoulAttID%d="), i+1);
			pStream->FillString(szTmp)<<SoulSpec.nSoulAttID[i];
		}
		pStream->FillString(" WHERE SerialNum=") << id;

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;

		m_pDBLoong->ReturnStream(pStream);
		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}

// wcy 2012-03-31 圣灵保存
BOOL CLoongDB::InsertHoly(const tagHolyMan & holy, BOOL bBaiBao)
{
	InsertEquip(holy,bBaiBao);
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();
		
		InitHolyUpdate(TRUE,pStream,pCon,&holy);


		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}


// wcy 2012-04-18 圣纹保存
BOOL CLoongDB::InsertHolyEquip(const tagHolyEquip & holyEquip, BOOL bBaiBao )
{
	InsertEquip(holyEquip,bBaiBao);
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();

		InitHolyEquipUpdate(TRUE,pStream,pCon,&holyEquip);


		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}

BOOL CLoongDB::UpdateHolySpec(INT64 id,const tagHolySpec & holy)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();
		pStream->FillString  ("UPDATE holyman SET ") ;
		InitHolySpecUpdate(pStream,pCon,(tagHolySpec*)&holy);
		pStream->FillString(" WHERE SerialNum=") << id;

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
	
}

BOOL CLoongDB::UpdateHolyEquipSpec(INT64 id,const tagHolyEquipSpec & holyEquip)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if( P_VALID(pStream) )
	{
		Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
		if( !P_VALID(pCon) )
		{
			m_pDBLoong->ReturnStream(pStream);
			return FALSE;
		}

		BOOL r = TRUE;
		pStream->Clear();
		pStream->FillString  ("UPDATE holyequip SET ") ;
		InitHolyEquipSpecUpdate(pStream,pCon,(tagHolyEquipSpec*)&holyEquip);
		pStream->FillString(" WHERE SerialNum=") << id;

		if( !m_pDBLoong->Execute(pStream) )
			r = FALSE;
		m_pDBLoong->ReturnStream(pStream);

		m_pDBLoong->ReturnConnection(pCon);
		return r;
	}
	return FALSE;
}
//-------------------------------------------------------------------------------------------------------
// 物品
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveItem(const tagItem &item)
{
	return Update(GT_INVALID, (LPVOID)&item, 1, SIZE_ITEM, (&CLoongDB::FormatSaveItem));
}

//-------------------------------------------------------------------------------------------------------
// 装备专用属性
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveEquipSpec(const tagEquip &equip)
{
	return Update(GT_INVALID, (LPVOID)&equip, 1, SIZE_EQUIP, (&CLoongDB::FormatSaveEquipSpec));
}

//-------------------------------------------------------------------------------------------------------
// 装备专用属性
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateEquipSpec(const tagEquipSpecUpdate &equipSpecUpdate)
{
	return Update(GT_INVALID, (LPVOID)&equipSpecUpdate, 1, sizeof(tagEquipSpecUpdate), (&CLoongDB::FormatUpdateEquipSpec));
}

//-------------------------------------------------------------------------------------------------------
// 装备专用单个属性属性
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateEquipSingleAtt(const tagEquipSingleSpec &equipSingleSpecUpdate)
{
	return Update(GT_INVALID, (LPVOID)&equipSingleSpecUpdate, 1, sizeof(tagEquipSingleSpec), (&CLoongDB::FormatUpdateEquipSingleSpec));
}

//-------------------------------------------------------------------------------------------------------
// 保存装备署名
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveEquipSignature(INT64 n64Serial, DWORD dwSignRoleID, TCHAR* szContent)
{
	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->SetUpdate("equip");
	pStream->FillString("Signature='").FillString(szContent).FillString("'");
	pStream->FillString(",SignRoleID=") << dwSignRoleID;
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// 装备 -- 装备要更新item表和equip表，需要保证数据的完整性
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveEquip(const tagEquip &equip)
{
	BOOL bRes = SaveItem(equip);
	if(!bRes)
	{
		return bRes;
	}

	bRes = SaveEquipSpec(equip);
	if(!bRes)
	{
		// equip表插入失败，从item表中删除相应记录
		//DelItem(equip.n64Serial);//??
	}

	return bRes;
}

//-------------------------------------------------------------------------------------------------------
// 删除物品
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelItem(INT64 n64Serial, CHAR *pTable/* = "item"*/)
{
	tagDelItem delItem;
	delItem.n64Serial	= n64Serial;
	delItem.pTable		= pTable;
	return Update(GT_INVALID, (LPVOID)&delItem, 1, sizeof(INT64), (&CLoongDB::FormatDelItem));
}

//-------------------------------------------------------------------------------------------------------
// 从装备表中删除装备专用属性
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelEquipSpec(INT64 n64Serial, CHAR *pTable/* = "equip"*/)
{
	tagDelItem delItem;
	delItem.n64Serial	= n64Serial;
	delItem.pTable		= pTable;
	return Update(GT_INVALID, (LPVOID)&delItem, 1, sizeof(INT64), (&CLoongDB::FormatDelEquipSpec));
}

//-------------------------------------------------------------------------------------------------------
// 删除装备
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelEquip(INT64 n64Serial)
{
	tagDWORDTime now = GetCurrentDWORDTime();
	CHAR szTime[X_DATATIME_LEN + 1] = {0};
	if(!DwordTime2DataTime(szTime, sizeof(szTime), now))
	{
		return FALSE;
	}

	if( !(UpdateDelEquipTime(n64Serial, szTime) && UpdateDelItemTime(n64Serial, szTime)) )
		return FALSE;

	if ( !(SaveDelEquipInfo(n64Serial) && SaveDelItemInfo(n64Serial)) )
		return FALSE;	

	if(DelItem(n64Serial))
	{
		return DelEquipSpec(n64Serial);
	}

	return FALSE;
}

//-------------------------------------------------------------------------------------------------------
// 从装备表和物品中删除属于指定角色的指定件数的指定类别的装备
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelEquipFromRole(DWORD dwRoleID, DWORD dwTypeID)
{
	tagDWORDTime now = GetCurrentDWORDTime();
	CHAR szTime[X_DATATIME_LEN + 1] = {0};
	if(!DwordTime2DataTime(szTime, sizeof(szTime), now))
	{
		return FALSE;
	}

	if( !(UpdateDelEquipTime(dwRoleID, dwTypeID, szTime) && UpdateDelItemTime(dwRoleID, dwTypeID, szTime)) )
		return FALSE;

	if ( !(SaveDelEquipInfo(dwRoleID, dwTypeID) && SaveDelItemInfo(dwRoleID, dwTypeID)) )
		return FALSE;	

	if(DelItem(dwRoleID, dwTypeID))
	{
		return DelEquipSpec(dwRoleID, dwTypeID);
	}

	return FALSE;
}

INT64 CLoongDB::GetDelEquip64Serial(DWORD dwRoleID, DWORD dwTypeID)
{
	INT64 n64Serial = 0;

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	pStream->FillString("select SerialNum from item where TypeID=") << dwTypeID;
	pStream->FillString(" and OwnerID=") << dwRoleID;
	pStream->SetLimit(1);

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		// 释放流
		m_pDBLoong->ReturnStream(pStream);
		return 0;
	}

	// 检验并保存查询结果
	if (pResult->GetRowCount() > 0)
	{
		n64Serial = (*pResult)[0].GetLong();
	}
	else
	{
		n64Serial = 0;
	}

	// 释放流
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return n64Serial;
}

BOOL CLoongDB::SaveDelEquipInfo(DWORD dwRoleID, DWORD dwTypeID)
{
	INT64 n64Serial = GetDelEquip64Serial(dwRoleID, dwTypeID);
	if (n64Serial == 0)
	{
		return FALSE;
	}

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	// 删除操作
	pStream->SetCopyItem("equip_del", "*", "equip");
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
BOOL CLoongDB::SaveDelItemInfo(DWORD dwRoleID, DWORD dwTypeID)
{
	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	// 删除操作
	pStream->SetCopyItem("item_del", "*", "item");
	pStream->SetWhere();
	pStream->FillString("TypeID=") << dwTypeID;
	pStream->FillString(" and OwnerID=") << dwRoleID;
	pStream->SetLimit(1);

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
BOOL CLoongDB::DelItem(DWORD dwRoleID, DWORD dwTypeID, CHAR *pTable/* = "item"*/)
{
	tagDelItemFromRole delItem;
	delItem.dwRoleID	= dwRoleID;
	delItem.dwTypeID	= dwTypeID;
	delItem.pTable		= pTable;
	return Update(GT_INVALID, (LPVOID)&delItem, 1, sizeof(INT64), (&CLoongDB::FormatDelItemFromRole));
}
BOOL CLoongDB::DelEquipSpec(DWORD dwRoleID, DWORD dwTypeID, CHAR *pTable/* = "equip"*/)
{
	INT64 n64Serial = GetDelEquip64Serial(dwRoleID, dwTypeID);
	if (n64Serial == 0)
	{
		return FALSE;
	}

	tagDelItem delItem;
	delItem.n64Serial	= n64Serial;
	delItem.pTable		= pTable;
	return Update(GT_INVALID, (LPVOID)&delItem, 1, sizeof(INT64), (&CLoongDB::FormatDelEquipSpec));
}

VOID CLoongDB::FormatDelItemFromRole(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagDelItemFromRole);

	pStream->SetDelete(p->pTable);

	pStream->SetWhere();
	pStream->FillString("TypeID=") << p->dwTypeID;
	pStream->FillString(" and OwnerID=") << p->dwRoleID;
	pStream->SetLimit(1);
}
VOID CLoongDB::FormatDelEquipSpecFromRole(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagDelItemFromRole);

	pStream->SetDelete(p->pTable);

	pStream->SetWhere();
	pStream->FillString("TypeID=") << p->dwTypeID;
	pStream->FillString(" and OwnerID=") << p->dwRoleID;
	pStream->SetLimit(1);
}

BOOL CLoongDB::SaveDelEquipInfo(INT64 n64Serial)
{
	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 删除操作
	pStream->SetCopyItem("equip_del", "*", "equip");
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

BOOL CLoongDB::SaveDelItemInfo(INT64 n64Serial)
{
	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 删除操作
	pStream->SetCopyItem("item_del", "*", "item");
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// 记删除时间
// 	INT64 n64SerialSet[1] = {n64Serial};
// 	bRet = bRet && LogItemDelTime(pStream, "item_del", n64SerialSet, 1);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

//-------------------------------------------------------------------------------------------------------
// 保存角色属性信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveRoleData(DWORD dwRoleID, const tagRoleDataSave *pRoleData)
{
	return Update(dwRoleID, (LPVOID)pRoleData, 1, GT_INVALID, (&CLoongDB::FormatSaveRoleData));
}

//-------------------------------------------------------------------------------------------------------
// 保存角色简易信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveRoleInfo(DWORD dwRoleID, const tagRoleInfo *pRoleInfo)
{
	return Update(dwRoleID, (LPVOID)pRoleInfo, 1, GT_INVALID, (&CLoongDB::FormatSaveRoleInfo));
}

//-------------------------------------------------------------------------------------------------------
// 保存氏族信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveClanData(DWORD dwRoleID, IN LPVOID pData, BOOL bChange, OUT LPVOID* ppOutPointer)
{
	if (!bChange)
	{
		*ppOutPointer = pData;
		return TRUE;
	}
	*ppOutPointer = reinterpret_cast<BYTE*>(pData) + sizeof(tagDBReputeData);
	return Update(dwRoleID, pData, 1, sizeof(tagDBReputeData), (&CLoongDB::FormatUpdateClanData));
}


//-------------------------------------------------------------------------------------------------------
// 保存物品信息
//-------------------------------------------------------------------------------------------------------
//BOOL CLoongDB::SaveItemEquip(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
//{
//	tagItem *pItem;
//	BYTE	*pCurPointer = (BYTE*)pData;
//	for(INT32 i=0; i<nNum; ++i)
//	{
//		pItem = (tagItem*)pCurPointer;
//		if(MIsEquipment(pItem->dwTypeID))
//		{
//			SaveEquip(*(tagEquip*)pItem);
//			pCurPointer += SIZE_EQUIP;
//		}
//		else
//		{
//			SaveItem(*pItem);
//			pCurPointer += SIZE_ITEM;
//		}
//	}
//
//	*ppOutPointer = pCurPointer;
//
//	return TRUE;
//}

//-------------------------------------------------------------------------------------------------------
// 更新物品位置及使用相关属性
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateItemPosAndUseRel(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagItemUpdate), (&CLoongDB::FormatUpdateItemPosAndUseRel), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// 插入角色技能信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertSkill(DWORD dwRoleID, tagSkillSave *pData)
{
	return Update(dwRoleID, pData, 1, sizeof(tagSkillSave), (&CLoongDB::FormatInsertSkill));
}

//-------------------------------------------------------------------------------------------------------
// 删除角色技能信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelSkill(DWORD dwRoleID, DWORD dwSkillID)
{
	return Update(dwRoleID, &dwSkillID, 1, sizeof(DWORD), (&CLoongDB::FormatDelSkill));
}

//-------------------------------------------------------------------------------------------------------
// 删除角色技能信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateSkill(DWORD dwRoleID, tagSkillSave *pData)
{
	LPVOID pSkillData = pData;
	return SaveSkill(dwRoleID, pSkillData, 1, &pSkillData);
}

//-------------------------------------------------------------------------------------------------------
// 保存角色技能信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveSkill(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagSkillSave), (&CLoongDB::FormatSaveSkill), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// 保存角色buff信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveRoleBuff(DWORD dwRoleID, LPVOID pBuff, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	// 先删除buff
	Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatDelAllRoleBuff));

	BYTE		*pCurPointer = (BYTE*)pBuff;
	tagBuffSave *pBuffSave;
	for(INT32 i=0; i<nNum; ++i)
	{
		pBuffSave = (tagBuffSave *)pCurPointer;
		Update(dwRoleID, pBuffSave, 1, sizeof(tagBuffSave), (&CLoongDB::FormatSaveRoleBuff));
		pCurPointer = pBuffSave->byData + pBuffSave->n8ModifierNum * sizeof(DWORD);
	}

	*ppOutPointer = pCurPointer;
	
	return TRUE;
}

//元神
VOID CLoongDB::SaveHolySoulInfo(LPVOID& pHolySoulInfo, DWORD dwID)
{
	if ( UpdateHolySoulInfo(pHolySoulInfo,dwID) )
	{
		pHolySoulInfo = ((BYTE*)pHolySoulInfo + sizeof(tagHolySoulInfoEx));
	}
}

BOOL CLoongDB::LoadHolySoulInfo(DWORD dwID,LPVOID& pData)
{
	return Load(pData, NULL, dwID, GT_INVALID, &CLoongDB::FormatLoadHolySoulInfo, &CLoongDB::ProcResLoadHolySoulInfo);  
}

BOOL CLoongDB::InsertHolySoulInfo(LPVOID pHolySoulInfo, DWORD dwID)
{
	return Update(dwID, pHolySoulInfo, 1, sizeof(tagHolySoulInfoEx), (&CLoongDB::FormatInsertHolySoulInfo));
}

BOOL CLoongDB::UpdateHolySoulInfo(LPVOID pHolySoulInfo, DWORD dwID)
{
	return Update(dwID, pHolySoulInfo, 1, sizeof(tagHolySoulInfoEx), (&CLoongDB::FormatUpdateHolySoulInfo));
}

BOOL CLoongDB::UpdateHolySoulExpAtt(LPVOID pData, DWORD dwID)
{
	return Update(dwID, pData, 1, sizeof(tagNDBC_UpdateHolySoulExpAtt), (&CLoongDB::FormatUpdateHolySoulExpAtt));
}

VOID CLoongDB::FormatLoadHolySoulInfo(Beton::MyStream *pStream, DWORD dwID, DWORD dwData)
{
	pStream->Clear();
	pStream->SetSelect("HolySoulInfo", "SoulID,CurLevelExp,CurSoulValue,CurCovalue,HolySoulState,RebornLv,SoulLevel,Demage,Hit,Crit,CritAmount,Exattack,Inattack,Accrate,Exdamage,Toughness,Morale");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwID;
}

VOID CLoongDB::ProcResLoadHolySoulInfo(OUT LPVOID &pData, OUT INT32 *pNum, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pHolySoulInfo, pData, tagHolySoulInfoEx);
	if (pRes->GetRowCount() <= 0)
	{
		return;
	}
	pHolySoulInfo->dwSoulID																	= (*pRes)[0].GetDword();
	pHolySoulInfo->n64CurLevelExp															= (*pRes)[1].GetLong();
	pHolySoulInfo->nCurSoulValue															= (*pRes)[2].GetDword();
	pHolySoulInfo->nCurCovalue																= (*pRes)[3].GetDword();
	pHolySoulInfo->eHolySoulState															= (EHolySoulState)((*pRes)[4].GetDword());
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_RebornLv]										= (*pRes)[5].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_SoulLevel]										= (*pRes)[6].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Demage]										= (*pRes)[7].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Hit]											= (*pRes)[8].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Crit]											= (*pRes)[9].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_CritAmount]									= (*pRes)[10].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Exattack]										= (*pRes)[11].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Inattack]										= (*pRes)[12].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Accrate]										= (*pRes)[13].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Exdamage]										= (*pRes)[14].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Toughness]										= (*pRes)[15].GetDword();
// 	pHolySoulInfo->HolySoulAtt.nSoulAtt[ESAT_Morale]										= (*pRes)[16].GetDword();

	pData =  ((BYTE*)pData + sizeof(tagHolySoulInfoEx)); 
}


VOID CLoongDB::FormatInsertHolySoulInfo(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{  
	MTRANS_POINTER(p, pData, tagHolySoulInfoEx); 

	pStream->Clear();
	pStream->SetInsert("HolySoulInfo");
	pStream->FillString("SoulID=") << p->dwSoulID;
	pStream->FillString(",RoleID=") << dwID;
	pStream->FillString(",CurLevelExp=") << p->n64CurLevelExp;
	pStream->FillString(",CurSoulValue=") << p->nCurSoulValue;
	pStream->FillString(",CurCovalue=") << p->nCurCovalue;
	pStream->FillString(",HolySoulState=") << p->eHolySoulState;
// 	pStream->FillString(",RebornLv=") << p->HolySoulAtt.nSoulAtt[ESAT_RebornLv];
// 	pStream->FillString(",SoulLevel=") << p->HolySoulAtt.nSoulAtt[ESAT_SoulLevel];
// 	pStream->FillString(",Demage=") << p->HolySoulAtt.nSoulAtt[ESAT_Demage];
// 	pStream->FillString(",Hit=") << p->HolySoulAtt.nSoulAtt[ESAT_Hit];
// 	pStream->FillString(",Crit=") << p->HolySoulAtt.nSoulAtt[ESAT_Crit];
// 	pStream->FillString(",CritAmount=") << p->HolySoulAtt.nSoulAtt[ESAT_CritAmount];
// 	pStream->FillString(",Exattack=") << p->HolySoulAtt.nSoulAtt[ESAT_Exattack];
// 	pStream->FillString(",Inattack=") << p->HolySoulAtt.nSoulAtt[ESAT_Inattack];
// 	pStream->FillString(",Accrate=") << p->HolySoulAtt.nSoulAtt[ESAT_Accrate];
// 	pStream->FillString(",Exdamage=") << p->HolySoulAtt.nSoulAtt[ESAT_Exdamage];
// 	pStream->FillString(",Toughness=") << p->HolySoulAtt.nSoulAtt[ESAT_Toughness];
// 	pStream->FillString(",Morale=") << p->HolySoulAtt.nSoulAtt[ESAT_Morale];


}

VOID CLoongDB::FormatUpdateHolySoulInfo(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{

	MTRANS_POINTER(p, pData, tagHolySoulInfoEx); 

	pStream->Clear();
	pStream->SetUpdate("HolySoulInfo");
	pStream->FillString("SoulID=") << p->dwSoulID;
	pStream->FillString(",CurLevelExp=") << p->n64CurLevelExp;
	pStream->FillString(",CurSoulValue=") << p->nCurSoulValue;
	pStream->FillString(",CurCovalue=") << p->nCurCovalue;
	pStream->FillString(",HolySoulState=") << p->eHolySoulState;
// 	pStream->FillString(",RebornLv=") << p->HolySoulAtt.nSoulAtt[ESAT_RebornLv];
// 	pStream->FillString(",SoulLevel=") << p->HolySoulAtt.nSoulAtt[ESAT_SoulLevel];
// 	pStream->FillString(",Demage=") << p->HolySoulAtt.nSoulAtt[ESAT_Demage];
// 	pStream->FillString(",Hit=") << p->HolySoulAtt.nSoulAtt[ESAT_Hit];
// 	pStream->FillString(",Crit=") << p->HolySoulAtt.nSoulAtt[ESAT_Crit];
// 	pStream->FillString(",CritAmount=") << p->HolySoulAtt.nSoulAtt[ESAT_CritAmount];
// 	pStream->FillString(",Exattack=") << p->HolySoulAtt.nSoulAtt[ESAT_Exattack];
// 	pStream->FillString(",Inattack=") << p->HolySoulAtt.nSoulAtt[ESAT_Inattack];
// 	pStream->FillString(",Accrate=") << p->HolySoulAtt.nSoulAtt[ESAT_Accrate];
// 	pStream->FillString(",Exdamage=") << p->HolySoulAtt.nSoulAtt[ESAT_Exdamage];
// 	pStream->FillString(",Toughness=") << p->HolySoulAtt.nSoulAtt[ESAT_Toughness];
// 	pStream->FillString(",Morale=") << p->HolySoulAtt.nSoulAtt[ESAT_Morale];


	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwID;
}

VOID CLoongDB::FormatUpdateHolySoulExpAtt(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_UpdateHolySoulExpAtt); 

	pStream->Clear();
	pStream->SetUpdate("HolySoulInfo");
	pStream->FillString("CurLevelExp=") << p->n64Exp;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << p->dwRoleID;	
}

BOOL CLoongDB::UpdateHolySoulAttInfo(LPVOID pData, DWORD dwID)
{
	return Update(dwID, pData, 1, sizeof(tagNDBC_UpdateHolySoulAttInfo), (&CLoongDB::FormatUpdateHolySoulAttInfo));
}

VOID CLoongDB::FormatUpdateHolySoulAttInfo(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_UpdateHolySoulAttInfo); 

	pStream->Clear();
	pStream->SetUpdate("HolySoulInfo");

	if ( ESoulAttEx_SoulVal == p->eSoulAttTypeEx )
	{
		pStream->FillString("CurSoulValue=") << p->dwAttVal;
	}
	else if ( ESoulAttEx_Covalue == p->eSoulAttTypeEx )
	{
		pStream->FillString("CurCovalue=") << p->dwAttVal;
	}


	pStream->SetWhere();
	pStream->FillString("RoleID=") << p->dwRoleID;	

}

//-------------------------------------------------------------------------------------------------------
// 保存称号信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveTitle(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagTitleSave), (&CLoongDB::FormatSaveTitle), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// 接受任务
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertTask(DWORD dwRoleID, LPVOID pAcceptQuest)
{
	return Update(dwRoleID, pAcceptQuest, 1, sizeof(tagAcceptQuest), (&CLoongDB::FormatInsertTask));
}


//-------------------------------------------------------------------------------------------------------
// 保存任务信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveTask(DWORD dwRoleID, LPVOID pTask, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pTask, nNum, sizeof(tagQuestSave), (&CLoongDB::FormatSaveTask), ppOutPointer);
}

//-------------------------------------------------------------------------------------------------------
// 保存已完成任务信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveTaskDone(DWORD dwRoleID, LPVOID pTask, INT32 nNum)
{
	return Update(dwRoleID, pTask, nNum, sizeof(tagQuestDoneSave), (&CLoongDB::FormatSaveTaskDone));
}

//-------------------------------------------------------------------------------------------------------
// 保存已完成日常任务信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveDailyTaskDone(DWORD dwRoleID, LPVOID pTask, INT32 nNum)
{ 
    return Update(dwRoleID, pTask, nNum, sizeof(tagQuestDoneSave), (&CLoongDB::FormatSaveDailyTaskDone));
}

//-------------------------------------------------------------------------------------------------------
// 创建角色名贴信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertVCard(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatInsertVCard));
}

//-------------------------------------------------------------------------------------------------------
// 更新角色名贴信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::UpdateVCard(DWORD dwRoleID, const PVOID pData)
{
	return Update(dwRoleID, pData, 1, sizeof(tagVCardData), (&CLoongDB::FormatUpdateVCard));
}

//-------------------------------------------------------------------------------------------------------
// 删除角色名贴信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelVCard(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatDelVCard));
}

//-------------------------------------------------------------------------------------------------------
// 创建氏族信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::InsertClanData(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatInsertClanData));
}

//-------------------------------------------------------------------------------------------------------
// 更新氏族信息
//-------------------------------------------------------------------------------------------------------
// BOOL CLoongDB::UpdateClanData(DWORD dwRoleID, const PVOID pData)
// {
// 	return Update(dwRoleID, pData, 1, sizeof(tagDBReputeData), (&CLoongDB::FormatUpdateClanData));
// }

//-------------------------------------------------------------------------------------------------------
// 删除氏族信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DelClanData(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatDelClanData));
}

BOOL CLoongDB::SaveActivityData(DWORD dwID, LPVOID pActivityData)
{
	return Update(dwID, pActivityData, 1, sizeof(tagActivityData), (&CLoongDB::FormatSaveActivity));
}

//-------------------------------------------------------------------------------------------------------
// 城市信息管理
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::CreateCityInfo(INT nCityNum, LPVOID pData)
{
	// 包括查询和更新的操作，需要锁表
	LockTables("city");

	for (int n=0; n<nCityNum; n++)
	{
		BOOL bCreate = FALSE;

		// 获取流
		Beton::MyStream* pStream = m_pDBLoong->GetStream();
		ASSERT(P_VALID(pStream));

		// 格式化查询的角色数据
		pStream->Clear();
		pStream->SetSelect("city", "id");
		pStream->SetWhere();
		pStream->FillString("id=") << ((DWORD*)pData)[n];

		// 查询数据库
		Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
		if(!P_VALID(pResult))
		{
			bCreate = TRUE;
		}
		else if (pResult->GetRowCount() <= 0)
		{
			bCreate = TRUE;
		}
		++m_dwReadTimes;

		// 释放流和结果
		m_pDBLoong->ReturnStream(pStream);
		m_pDBLoong->FreeQueryResult(pResult);

		// 创建城市信息
		if (bCreate)
		{
			Update(((DWORD*)pData)[n], NULL, 1, 0, &CLoongDB::FormatCreateCityInfo);
		}
	}

	// 解锁
	UnlockTables();

	return TRUE;
}

BOOL CLoongDB::SaveCityInfo(LPVOID pData)
{
	return Update(GT_INVALID, pData, 1, 0, &CLoongDB::FormatSaveCityInfo);
}

BOOL CLoongDB::UpdateCityInfo(DWORD dwCityID, LPVOID pData)
{
	return Update(dwCityID, pData, 1, 0, &CLoongDB::FormatUpdateCityInfo);
}

VOID CLoongDB::SaveMaster(DWORD dwRoleID, IN LPVOID pData, OUT LPVOID* ppOutPointer)
{
	MTRANS_POINTER(p, pData, tagMasterApprentice2DB);

	pData = (BYTE*)pData + sizeof(tagMasterApprentice2DB);
	*ppOutPointer = pData;
}

BOOL CLoongDB::SaveAllMasterApprenticeData(LPVOID pData)
{
	return Update(GT_INVALID, pData, 1, 0, &CLoongDB::FormatSaveAllMasterApprenticeData);
}

BOOL CLoongDB::SaveOneMasterApprenticeData(DWORD dwID, LPVOID pData)
{
	tagOneMASaveData tmpData;
	MTRANS_POINTER(pLoadData, &tmpData.loadData, void);
	BOOL bRet = Load(pLoadData, NULL, dwID, GT_INVALID, &CLoongDB::FormatLoadMasterApprenticeData, &CLoongDB::ProcResLoadMasterApprenticeData);

	if (!bRet)
	{
		return FALSE;
	}

	// 将pData赋值给tmpData然后传进FormatSaveOneMasterApprenticeData
	MTRANS_POINTER(pOneMAData, pData, tagOneMAData2DB);
	tmpData.oneMAData = *pOneMAData;				

	MTRANS_POINTER(pSaveData, &tmpData, void);
	return Update(dwID, pSaveData, 1, 0, &CLoongDB::FormatSaveOneMasterApprenticeData);
}

BOOL CLoongDB::InsertMasterApprenticeData(DWORD dwID, LPVOID pData)
{
	
	//LockTables("master");

	BOOL bInsert = FALSE;

	// 获取流
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 格式化查询的角色数据
	pStream->Clear();
	pStream->SetSelect("master", "dwID");
	pStream->SetWhere();
	pStream->FillString("dwID=") << dwID;

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bInsert = TRUE;
	}
	else if (pResult->GetRowCount() <= 0)
	{
		bInsert = TRUE;
	}
	++m_dwReadTimes;

	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	// 插入师徒信息
	if (bInsert)
	{
		Update( dwID, pData, 1, 0, &CLoongDB::FormatInsertMasterApprenticeData );
	}


	// 解锁
	//UnlockTables();

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// 删除一条徒弟信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::DeleteAnApprentice(DWORD dwID, DWORD dwApprenticeID)
{
	//LockTables("master");

	BOOL bDelete = TRUE;

	// 获取流
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 格式化查询的角色数据
	pStream->Clear();
	pStream->SetSelect("master", "dwID,JingWuPoint,QinWuPoint,LevelUpBonus,RewardNum,ApprenticeData");
	pStream->SetWhere();
	pStream->FillString("dwID=") << dwID;

	// 查询数据库
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		bDelete = FALSE;
	}
	else if (pResult->GetRowCount() <= 0)
	{
		bDelete = FALSE;
	}
	++m_dwReadTimes;
	
	// 删除一条徒弟信息
	if (bDelete)
	{
		tagMasterApprentice2DB appData;
		appData.dwID			=	(*pResult)[0].GetInt();
		appData.nJingWuPoint	=	(*pResult)[1].GetInt();
		appData.nQinWuPoint		=	(*pResult)[2].GetInt();
		appData.nLevelUpBonus	=	(*pResult)[3].GetInt();
		appData.nRewardNum		=	(*pResult)[4].GetInt();
		(*pResult)[5].GetBlob(appData.sApprentice.dwData, (*pResult)[5].GetLen());

		// 除去该徒弟记录
		int nSize = sizeof(appData.sApprentice.dwData) / sizeof(DWORD);
		DWORD	dwTmp[MAX_APPRENTICE_NUM];
		ZeroMemory(dwTmp, sizeof(dwTmp));
		int n = 0;
		for (int i=0; i!=nSize; ++i)
		{
			if (dwApprenticeID == appData.sApprentice.dwData[i])
			{
				continue;
			}
			dwTmp[n++] = appData.sApprentice.dwData[i];
		}

		memcpy(appData.sApprentice.dwData, dwTmp, sizeof(dwTmp));

		Update( dwID, &appData, 1, 0, &CLoongDB::FormatSaveAllMasterApprenticeData );
	}


	// 释放流和结果
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	// 解锁
	//UnlockTables();

	return TRUE;
}

//-------------------------------------------------------------------------------------------------------
// 竞技场信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveArenaData(DWORD dwSerialID,LPVOID pData)
{
	return Update(dwSerialID, pData, 1, 0, &CLoongDB::FormatSaveArenaData);
	
}

BOOL  CLoongDB::SaveSerialVal(DWORD dwSerialID,LPVOID pData)
{
    return Update(dwSerialID, pData, 1, 0, &CLoongDB::FormatSaveSerialVal);
}

VOID CLoongDB::FormatSaveSerialVal(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_SaveSerialValToDB);
	pStream->Clear();
	pStream->SetUpdate("arena_data");
	pStream->FillString("serial_val=") << p->dwSerialVal;
	pStream->SetWhere();
	pStream->FillString("serial_id=") << p->dwSerialId;
}
VOID CLoongDB::FormatSaveArenaData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{

	MTRANS_POINTER(p, pData, tagNDBC_SaveArenaDataToDB);
    pStream->Clear();
	pStream->SetReplace("arena_data");
    
	pStream->FillString("serial_id=") << p->tPvPSystemData.dwSerialId;
	pStream->FillString(",system_id=") << p->tPvPSystemInfo.n16SystemId;
	pStream->FillString(",serial_val=") << p->tPvPSystemInfo.dwSerialVal;
    CHAR cTimeRange[32];
	DwordTime2DataTime(cTimeRange, sizeof(cTimeRange), p->tPvPSystemData.saveTime);
	pStream->FillString(",time_range='").FillString(cTimeRange);

	
	pStream->FillString("'");
	pStream->FillString(",hall_persom_num=") << p->tPvPSystemData.dwHallPersonNum;

	pStream->FillString(",hall_room_num=") << p->tPvPSystemData.dwHallRoomNum;
	pStream->FillString(",fighting_room_num=") << p->tPvPSystemData.dwFightRoomNum;
	pStream->FillString(",below120Second=") << p->tPvPSystemData.n16Below120Sec;
	pStream->FillString(",below210Second=") << p->tPvPSystemData.n16Below210Sec;
    pStream->FillString(",below300Second=") << p->tPvPSystemData.n16Below300Sec;
	pStream->FillString(",over300Second=") << p->tPvPSystemData.n16Over300Sec;
	
}

//-------------------------------------------------------------------------------------------------------
// 保存快捷栏信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveShortCut_QuickBar(DWORD dwRoleID,LPVOID pData)
{
	return Update(dwRoleID, pData, 1, 0, &CLoongDB::FormatSaveShortCut_QuickBar);
}

//-------------------------------------------------------------------------------------------------------
// 保存快捷栏SQL格式化语句
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveShortCut_QuickBar(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{  
	
	MTRANS_POINTER(p, pData, tagNDBC_SaveShortCut_QuickBar);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->Clear();
	pStream->SetReplace("shortcut_quickbar");
	pStream->FillString("RoleID=") << dwID;
	pStream->FillString(",quickbardata='").FillBlob(p->QuickBarData, sizeof(tagQuickData)*QUICK_BAR_TOTALCOUNT, pCon);
	pStream->FillString("'");

	m_pDBLoong->ReturnConnection(pCon);

}

//-------------------------------------------------------------------------------------------------------
// 选秀活动相关信息
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
// 保存报名选秀表信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveShowPlayerData(DWORD dwID,LPVOID pData)
{
	return Update(dwID, pData,1, 0, &CLoongDB::FormatSaveShowPlayerData);
}


//-------------------------------------------------------------------------------------------------------
// 保存选秀者选手得分信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveShowPlayerPoint(DWORD dwID,LPVOID pData)
{
    return Update(dwID, pData, 1, 0, &CLoongDB::FormatSaveShowPlayerPoint);
}

//-------------------------------------------------------------------------------------------------------
// 保存投票人得分信息
//-------------------------------------------------------------------------------------------------------
BOOL CLoongDB::SaveVotePlayerPoint(DWORD dwID,LPVOID pData)
{
    return Update(dwID, pData, 1, 0, &CLoongDB::FormatSaveVotePlayerPoint);
}

BOOL CLoongDB::SaveShowPlayerDataCount(DWORD dwID,LPVOID pData)
{
     return Update(dwID, pData, 1, 0, &CLoongDB::FormatSaveShowPlayerDataCount);
}

BOOL CLoongDB::SaveVotePlayerDataCount(DWORD dwID,LPVOID pData)
{
      return Update(dwID, pData, 1, 0, &CLoongDB::FormatSaveVotePlayerDataCount);
}

//-------------------------------------------------------------------------------------------------------
// 保存活动排行信息
//-------------------------------------------------------------------------------------------------------

BOOL CLoongDB::SaveActivityRankData(DWORD dwID, LPVOID pActivityRankData)
{
	 return Update(dwID, pActivityRankData, 1, 0, &CLoongDB::FormatSaveActivityRankData);
}

//-------------------------------------------------------------------------------------------------------
// 保存活动排行信息
//-------------------------------------------------------------------------------------------------------

VOID CLoongDB::FormatSaveActivityRankData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	
	//　MTRANS_POINTER(p, pData, tagNDBC_SaveActiveRankData);
      MTRANS_POINTER(p, pData, tagActiveRankData);
	  pStream->Clear();
	  pStream->SetReplace("activity_rankdata");
	  pStream->FillString("role_id=") << dwID;
	  pStream->FillString(",role_name='").FillString(p->ucRoleName);
      pStream->FillString("'");

	  pStream->FillString(",role_level=") << p->n16RoleLevel;
	  pStream->FillString(",datatype1=") << p->nDataType[0];
	  pStream->FillString(",datatype2=") << p->nDataType[1];
	  pStream->FillString(",datatype3=") << p->nDataType[2];
	  pStream->FillString(",datatype4=") << p->nDataType[3];
	  pStream->FillString(",datatype5=") << p->nDataType[4];
	  pStream->FillString(",datatype6=") << p->nDataType[5];
	  pStream->FillString(",datatype7=") << p->nDataType[6];
	  pStream->FillString(",datatype8=") << p->nDataType[7];
}




VOID CLoongDB::FormatSaveShowPlayerDataCount(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_SaveShowPlayerDataCount);

	pStream->Clear();
	pStream->SetUpdate("dragongirl_data");
	pStream->FillString("serial_id=") << dwID;
	pStream->FillString(",data_count=") << p->dwTotalDataCount;
}

VOID CLoongDB::FormatSaveVotePlayerDataCount(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{  
	MTRANS_POINTER(p, pData, tagNDBC_SaveVotePlayerDataCount);

	pStream->Clear();
	pStream->SetReplace("dragondefender_data");
    pStream->FillString("serial_id=")<<0;
	pStream->FillString(",role_id=") << 0;
	pStream->FillString(",role_name='").FillString(_T("Record data count flags"));
	pStream->FillString("'");
	pStream->FillString(",point=") <<0;
	pStream->FillString(",data_count=") <<p->dwTotalDataCount;
}
//-------------------------------------------------------------------------------------------------------
// 保存报名选秀表信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveShowPlayerData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagShowPlayer);
	pStream->Clear();
	pStream->SetReplace("dragongirl_data");
	pStream->FillString("sign_id=") << dwID;
	pStream->FillString(",role_id=") <<p->dwAthleteRoleId;

	pStream->FillString(",role_name='").FillString(p->tcAthleteRoleName);
	pStream->FillString("'");
    pStream->FillString(",point=") <<p->dwAthletePoint;
	pStream->FillString(",serial_id=") <<p->dwAthleteSerialId;
	pStream->FillString(",data_count=") <<(DWORD)0;
}

//-------------------------------------------------------------------------------------------------------
// 保存选秀者选手得分信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveShowPlayerPoint(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_SaveShowPlayerPoint);  

	pStream->Clear();

	pStream->SetReplace("dragongirl_data");
	pStream->FillString("sign_id=") << dwID;
	pStream->FillString(",role_id=") <<p->showPlayer.dwAthleteRoleId;
	pStream->FillString(",role_name='").FillString(p->showPlayer.tcAthleteRoleName);
	pStream->FillString("'");
	pStream->FillString(",point=") <<p->showPlayer.dwAthletePoint;
    pStream->FillString(",serial_id=") <<p->showPlayer.dwAthleteSerialId;
	pStream->FillString(",data_count=") <<(DWORD)0;
}

//-------------------------------------------------------------------------------------------------------
// 保存投票人得分信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveVotePlayerPoint(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNDBC_SaveVotePlayerPoint);

	pStream->Clear();
	pStream->SetReplace("dragondefender_data");
    pStream->FillString("serial_id=") << p->VotePlayer.dwSerialId;
	pStream->FillString(",role_id=") << dwID;
	pStream->FillString(",role_name='").FillString(p->VotePlayer.tcVoteRoleName);
	pStream->FillString("'");
	pStream->FillString(",point=") <<p->VotePlayer.dwVotePoint;
	pStream->FillString(",data_count=") <<(DWORD)0;
}

//********************************************************************************************************
//-------------------------------------------------------------------------------------------------------
// 插入新物品
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(pEx, pData, tagNewItemEquip);
	const tagItem *p = pEx->pItem;

	pStream->SetInsert(pEx->pTable);

	pStream->FillString("SerialNum=") << p->n64Serial;
	pStream->FillString(",TypeID=") << p->dwTypeID;

	pStream->FillString(",Num=") << p->n16Num;
	pStream->FillString(",Bind=") << p->byBind;
	pStream->FillString(",LockStat=") << p->bLock;
	pStream->FillString(",UnlockTime=") << p->dwUnlockTime;
	pStream->FillString(",UseTimes=") << p->nUseTimes;
	
	pStream->FillString(",CreateMode=") << p->eCreateMode;
	pStream->FillString(",CreateID=") << p->dwCreateID;

	pStream->FillString(",CreatorID=") << p->dwCreatorID;
	pStream->FillString(",NameID=") << p->dwNameID;

	DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwCreateTime);
	pStream->FillString(",CreateTime='").FillString(m_szTime);
	pStream->FillString("'");

	DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dw1stGainTime);
	pStream->FillString(",FirstGainTime='").FillString(m_szTime);
	pStream->FillString("'");

	pStream->FillString(",OwnerID=") << p->dwOwnerID;
	pStream->FillString(",AccountID=") << p->dwAccountID;
	pStream->FillString(",ContainerTypeID=") << p->eConType;
	pStream->FillString(",Suffix=") << p->n16Index;

	pStream->FillString(",ItemSpecVal1=") << p->dwSpecVal1;
	pStream->FillString(",ItemSpecVal2=") << p->dwSpecVal2;
}

//-------------------------------------------------------------------------------------------------------
// 格式化装备属性通用部分(insert,update)
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatCommonEquipSpec(Beton::MyStream *pStream, const tagEquipSpec *pEquipSpec)
{
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->FillString("Quality=") << pEquipSpec->byQuality;
	pStream->FillString(",MinUseLevel=") << pEquipSpec->byMinUseLevel;
	pStream->FillString(",MaxUseLevel=") << pEquipSpec->byMaxUseLevel;
	pStream->FillString(",AttALimMod=") << pEquipSpec->n16AttALimMod;
	pStream->FillString(",AttALimModPct=") << pEquipSpec->n16AttALimModPct;

	pStream->FillString(",WuHun=") << pEquipSpec->n16WuHun;
	pStream->FillString(",MinDmg=") << pEquipSpec->n16MinDmg;
	pStream->FillString(",MaxDmg=") << pEquipSpec->n16MaxDmg;
	pStream->FillString(",Armor=") << pEquipSpec->n16Armor;
	pStream->FillString(",PotVal=") << pEquipSpec->nPotVal;
	pStream->FillString(",PotValUsed=") << pEquipSpec->nPotValUsed;
	// Jason 2009-12-1装备潜力值提升次数，默认0
	pStream->FillString(",PotIncTimes=") << pEquipSpec->nPotIncTimes;
	// Jason 2009-12-2 补充装备紫色品级鉴定几率，默认0
	pStream->FillString  (",PurpleQltyIdfPct=") << pEquipSpec->dwPurpleQltyIdfPct;
	// Jason 2010-6-11
	pStream->FillString(",RoleAtt2EffectCount=") << pEquipSpec->nRoleL2AttCount;

	pStream->FillString(",PosyTimes=") << pEquipSpec->byPosyTimes;
	pStream->FillString(",EngraveTimes=") << pEquipSpec->byEngraveTimes;
	pStream->FillString(",HoleNum=") << pEquipSpec->byHoleNum;
	pStream->FillString(",BrandLevel=") << pEquipSpec->byBrandLevel;
	pStream->FillString(",LongFuLevel=") << pEquipSpec->byLongfuLevel;

	pStream->FillString(",CanCut=") << pEquipSpec->bCanCut;
	pStream->FillString(",LongInnerID=") << pEquipSpec->dwLongHunInnerID;
	pStream->FillString(",LongOuterID=") << pEquipSpec->dwLongHunOuterID;
	pStream->FillString(",SpecAtt=") << pEquipSpec->bySpecAtt;
	pStream->FillString(",FlareVal=") << pEquipSpec->byFlareVal;
	pStream->FillString(",QltyModPct=") << pEquipSpec->n16QltyModPct;
	pStream->FillString(",QltyModPctEx=") << pEquipSpec->n16QltyModPctEx;
	pStream->FillString(",PotValModPct=") << pEquipSpec->n16PotValModPct;

	pStream->FillString(",Signed=") << pEquipSpec->bySigned;
	pStream->FillString(",SignRoleID=") << pEquipSpec->dwSignatruePlayerID;
	pStream->FillString(",Signature='").FillString(pEquipSpec->szSignature).FillString("'");

	// 时装属性
	pStream->FillString(",Appearance=") << pEquipSpec->n16Appearance;
	pStream->FillString(",Rein=") << pEquipSpec->byRein;
	pStream->FillString(",Savvy=") << pEquipSpec->bySavvy;
	pStream->FillString(",Fortune=") << pEquipSpec->byFortune;
	pStream->FillString(",ColorID=") << pEquipSpec->n8ColorID;
	// Jason 2010-5-18 v2.1.2
	pStream->FillString(",SoaringFlag=") << pEquipSpec->bSoaringFlag;
	pStream->FillString(",CastingTimes=") << pEquipSpec->nCastingTimes;
	// Jason 2010-6-13 v1.0.0
	pStream->FillString(",ConsolidateLevel=") << pEquipSpec->byConsolidateLevel;

	pStream->FillString(",RoleAttEffect='").FillBlob(pEquipSpec->nRoleAttEffect, sizeof(pEquipSpec->nRoleAttEffect), pCon);
	pStream->FillString("',PosyEffect='").FillBlob(pEquipSpec->PosyEffect, sizeof(pEquipSpec->PosyEffect), pCon);
	pStream->FillString("',EngraveAtt='").FillBlob(pEquipSpec->nEngraveAtt, sizeof(pEquipSpec->nEngraveAtt), pCon);
	pStream->FillString("',HoleGemID='").FillBlob(pEquipSpec->dwHoleGemID, sizeof(pEquipSpec->dwHoleGemID), pCon);
	pStream->FillString("',DerateVal='").FillBlob(pEquipSpec->nBrandAtt, sizeof(pEquipSpec->nBrandAtt), pCon);
	pStream->FillString("',WuXing='").FillBlob(pEquipSpec->byLongfuAtt, sizeof(pEquipSpec->byLongfuAtt), pCon);
	pStream->FillString("'");
	// Jason 2010-6-19 v1.0.0 强化属性影响
	pStream->FillString(",ConsolidateAttEffect='").FillBlob(pEquipSpec->ConsolidateEffect, sizeof(pEquipSpec->ConsolidateEffect), pCon);
	pStream->FillString("'");

	// Jason 2010-9-7 神铸
	pStream->FillString(",LastGodStrengthenLevel=")	<< pEquipSpec->byLastGodStrengthenLevel;
	pStream->FillString(",GodStrengthenLevel=")		<< pEquipSpec->byGodStrengthenLevel;
	pStream->FillString(",CurGodStrengthenExp=")		<< pEquipSpec->dwCurGodStrengthenExp;
	pStream->FillString(",GodStrengthenTime=")		<< ((DWORD)const_cast<tagEquipSpec*>(pEquipSpec)->dwGodStrengthenTime);

	// 装备成长
	pStream->FillString(",GrowID=") << pEquipSpec->dwGrowTypeID;
	pStream->FillString(",GrowValue=") << pEquipSpec->dwGrowValue;
	pStream->FillString(",GrowLevel=") << pEquipSpec->byGrowLevel;
	pStream->FillString(",GrowTimes=") << pEquipSpec->nTotalGrowTimes;
	pStream->FillString(",GrowUseIMTimes=") << pEquipSpec->nTotalIMUseTimes;
	pStream->FillString(",GrowStars='").FillBlob(pEquipSpec->eGrowStar, sizeof(pEquipSpec->eGrowStar), pCon);
	pStream->FillString("',GrowAtt='").FillBlob(pEquipSpec->nGrowAttValue, sizeof(pEquipSpec->nGrowAttValue), pCon);
	pStream->FillString("'");
	// 宝石增幅属性
	pStream->FillString(",DiamondLevel=")<< pEquipSpec->byDiamondLevel;
	//元神强化
	pStream->FillString(",byStarEnhanceLvl=") << pEquipSpec->byStarEnhanceLv;
	pStream->FillString(",byHolyHoleNum=") << pEquipSpec->byHolyHoleNum;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// 插入新装备
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagNewItemEquip);

	pStream->SetInsert(p->pTable);
	
	pStream->FillString("SerialNum=") << p->pEquip->n64Serial;
	pStream->FillString(",");
	FormatCommonEquipSpec(pStream, &p->pEquip->equipSpec);
}

//-------------------------------------------------------------------------------------------------------
// 保存角色的物品信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pItem)
{
	MTRANS_POINTER(p, pItem, tagItem);

	pStream->SetUpdate("item");

	//pStream->FillString("SerialNum=") << p->n64Serial;
	//pStream->FillString(",TypeID=") << p->dwTypeID;

	pStream->FillString("Num=") << p->n16Num;
	pStream->FillString(",Bind=") << p->byBind;
	pStream->FillString(",UnlockTime=") << p->dwUnlockTime;
	pStream->FillString(",LockStat=") << p->bLock;
	pStream->FillString(",UseTimes=") << p->nUseTimes;
	
	//pStream->FillString(",CreateMode=") << p->eCreateMode;
	//pStream->FillString(",CreateID=") << p->dwCreateID;

	//pStream->FillString(",CreatorID=") << p->dwCreatorID;

	//DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwCreateTime);
	//pStream->FillString(",CreateTime='").FillString(m_szTime);
	//pStream->FillString("'");

	DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dw1stGainTime);
	pStream->FillString(",FirstGainTime='").FillString(m_szTime);
	pStream->FillString("'");

	pStream->FillString(",OwnerID=") << p->dwOwnerID;
	pStream->FillString(",AccountID=") << p->dwAccountID;
	pStream->FillString(",ContainerTypeID=") << p->eConType;
	pStream->FillString(",Suffix=") << p->n16Index;

	pStream->FillString(",ItemSpecVal1=") << p->dwSpecVal1;
	pStream->FillString(",ItemSpecVal2=") << p->dwSpecVal2;

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// 保存装备信息 -- 修改equip表
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pEquip)
{
	MTRANS_POINTER(p, pEquip, tagEquip);

	pStream->SetUpdate("equip");
	FormatCommonEquipSpec(pStream, &p->equipSpec);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// 更新装备属性
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pEquipSpecUpdate)
{
	MTRANS_POINTER(p, pEquipSpecUpdate, tagEquipSpecUpdate);

	pStream->SetUpdate("equip");
	FormatCommonEquipSpec(pStream, &p->equipSpec);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}
//-------------------------------------------------------------------------------------------------------
// 更新装备单个属性
//-------------------------------------------------------------------------------------------------------

VOID CLoongDB::FormatUpdateEquipSingleSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pEquipSingleSpecUpdate)
{
	MTRANS_POINTER(p, pEquipSingleSpecUpdate, tagEquipSingleSpec);
	if ( EESSType_Null == p->eEquipSingleSpecType )
	{
		return ;
	}
	pStream->SetUpdate("equip");
	switch( p->eEquipSingleSpecType )
	{
	case EESSType_StarEnhance:
		pStream->FillString("byStarEnhanceLvl=") << p->dwVal;
		break;
	case EESSType_HolyEquipHole:
		pStream->FillString("byHolyHoleNum=") << p->dwVal;
		break;
	default:
		return;
	}	
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// 保存角色属性信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagDelItem);

	pStream->SetDelete(p->pTable);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// 保存角色属性信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelEquipSpec(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagDelItem);

	pStream->SetDelete(p->pTable);

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}


//-------------------------------------------------------------------------------------------------------
// 保存角色属性信息
// Jason 2010-1-0 v1.3.2 增加每日在线奖励相关字段
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveRoleData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	tagRoleDataSave *p = (tagRoleDataSave *)pData;

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	
	pStream->SetUpdate("roledata");
	pStream->FillString("AvatarEquip='").FillBlob(&p->AvatarEquip, SIZE_AVATAR_EQUIP, pCon);
	pStream->FillString("',DisplaySet=")<< (BYTE)p->DisplaySet;
	pStream->FillString(",MapID=") << p->dwMapID;
	pStream->FillString(",X=") << p->fCoordinate[0];// 坐标
	pStream->FillString(",Y=") << p->fCoordinate[1];
	pStream->FillString(",Z=") << p->fCoordinate[2];
	pStream->FillString(",FaceX=") << p->fFace[0];// 朝向
	pStream->FillString(",FaceY=") << p->fFace[1];
	pStream->FillString(",FaceZ=") << p->fFace[2];

	pStream->FillString(",RebornMapID=") << p->dwRebornMapID;
	pStream->FillString(",SpeakOff=") << p->bSpeakOff;
	pStream->FillString(",Class=") << (INT32)p->eClass;
	pStream->FillString(",ClassEx=") << p->eClassEx;
	pStream->FillString(",Level=") << p->nLevel;
	pStream->FillString(",LastUpgrade=") << p->dwLastUpgrade;
	pStream->FillString(",ExpCurLevel=") << p->nCurExp;

	pStream->FillString(",HP=") << p->nHP;
	pStream->FillString(",MP=") << p->nMP;
	pStream->FillString(",Rage=") << p->nRage;
	pStream->FillString(",Endurance=") << p->nEndurance;
	pStream->FillString(",Vitality=") << p->nVitality;
	pStream->FillString(",Injury=") << p->nInjury;
	pStream->FillString(",Knowledge=") << p->nKnowledge;
	pStream->FillString(",Morale=") << p->nMorale;
	pStream->FillString(",Morality=") << p->nMorality;
	pStream->FillString(",Culture=") << p->nCulture;
	pStream->FillString(",Hostility=") << p->nHostility;
	pStream->FillString(",Credit=") << p->nCredit;
	pStream->FillString(",Identity=") << p->nIdentity;
	pStream->FillString(",Spirit=") << p->nSpirit;
	pStream->FillString(",WuXun=") << p->nWunXun;
	pStream->FillString(",WuJi=") << p->nWunJi;
	pStream->FillString(",Weary=") << p->nWeary;
	pStream->FillString(",FixSpirit=") << p->dwFixSpiritCD;
	pStream->FillString(",VIPPoint=") << p->nVIPPoint;
	pStream->FillString(",VipLevel=") << p->nVipLevel;
	pStream->FillString(",VipStartTime=") << p->dwVipStartTime;
	pStream->FillString(",VipMaxDays=") << p->dwVipMaxDays;
	pStream->FillString(",FreeWorldTalkTimes=") << p->n16FreeWorldTalkTimes;
	pStream->FillString(",AttPtAvail=") << p->nAttPoint;
	pStream->FillString(",TalentPtAvail=") << p->nTalentPoint;

	pStream->FillString(",Strength=") << p->nStrength;
	pStream->FillString(",EquipValue=") << p->nEquipValue;
	pStream->FillString(",YaoJingValue=") << p->nYaoJingValue;
	pStream->FillString(",AdvanceStrength=") << p->nAdvanceStrength;
	pStream->FillString(",ConsumptiveStrength=") << p->nConsumptiveStrength;
	pStream->FillString(",KillCount=") << p->nKillScore;

	pStream->FillString(",PhysiqueAdded=") << p->nAttPointAdd[ERA_Physique];
	pStream->FillString(",StrengthAdded=") << p->nAttPointAdd[ERA_Strength];
	pStream->FillString(",PneumaAdded=") << p->nAttPointAdd[ERA_Pneuma];
	pStream->FillString(",InnerforceAdded=") << p->nAttPointAdd[ERA_InnerForce];
	pStream->FillString(",TechniqueAdded=") << p->nAttPointAdd[ERA_Technique];
	pStream->FillString(",AgilityAdded=") << p->nAttPointAdd[ERA_Agility];

	pStream->FillString(",TalentType1=") << p->talent[0].eType;
	pStream->FillString(",TalentType2=") << p->talent[1].eType;
	//pStream->FillString(",TalentType3=") << p->talent[2].eType;
	//pStream->FillString(",TalentType4=") << p->talent[3].eType;
	pStream->FillString(",TalentVal1=") << p->talent[0].nPoint;
	pStream->FillString(",TalentVal2=") << p->talent[1].nPoint;
	//pStream->FillString(",TalentVal3=") << p->talent[2].nPoint;
	//pStream->FillString(",TalentVal4=") << p->talent[3].nPoint;

	pStream->FillString(",SGFlag=") << p->bSafeGuard;
	
	pStream->FillString(",BagSize=") << p->n16BagSize;		// 背包
	pStream->FillString(",BagGold=") << p->nBagGold;
	pStream->FillString(",BagSilver=") << p->nBagSilver;
	pStream->FillString(",BagYuanBao=") << p->nBagYuanBao;
	pStream->FillString(",ExchangeVolume=") << p->nExVolume;

	//pStream->FillString(",WareSize=") << p->n16WareSize;		// 账号仓库
	//pStream->FillString(",WareGold=") << p->nWareGold;
	//pStream->FillString(",WareSilver=") << p->nWareSilver;
	//pStream->FillString(",WareYuanBao=") << p->nWareYuanBao;

	pStream->FillString(",TotalTax=") << p->nTotalTax;	// 缴税总额
	pStream->FillString(",RemoteOpenSet=") << (DWORD)p->sRemoteOpenSet;
	pStream->FillString(",CurTitleID=") << p->u16ActiveTitleID;

	if(DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->LoginTime))
	{
		pStream->FillString(",LoginTime='").FillString(m_szTime);
		pStream->FillString("'");

		DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->LogoutTime);
		pStream->FillString(",LogoutTime='").FillString(m_szTime);
		pStream->FillString("'");
		
		DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->CloseSafeGuardTime);
		pStream->FillString(",CloseSGTime='").FillString(m_szTime);
		pStream->FillString("'");

		DwordTime2DataTime(m_szTime, sizeof(m_szTime), p->dwTimeGetMallFree);
		pStream->FillString(",GetMallFreeTime='").FillString(m_szTime);
		pStream->FillString("'");
	}
	else
	{
		IMSG(_T("Error: Func DwordTime2DataTime() run error in CLoongDB::FormatSaveRoleData()!!!!!\r\n"));
		ASSERT(0);
	}

	pStream->FillString(",OnlineTime=") << p->nOnlineTime;
	pStream->FillString(",CurOnlineTime=") << p->nCurOnlineTime;

	pStream->FillString(",ScriptData='");
	pStream->FillBlob(p->Data.dwData, sizeof(DWORD)*ESD_Role, pCon);
	pStream->FillString("'");

	pStream->FillString(",StallLevel=") << p->byStallLevel;
	pStream->FillString(",StallDailyExp=") << p->nStallDailyExp;
	pStream->FillString(",StallCurExp=") << p->nStallCurExp;
	pStream->FillString(",StallLastTime=") << p->dwLastStallTime;

	pStream->FillString(",DeadUnSetSafeGuardCountDown=") << p->nDeadUnSetSafeGuardCountDown;
	pStream->FillString(",NeedPrisonRevive=") << p->bNeedPrisonRevive;

	pStream->FillString(",LoverID=") << p->dwLoverID;
	pStream->FillString(",HaveWedding=") << p->bHaveWedding;

	// Jason 2009-12-7 增加神龙赐福相关字段
	pStream->FillString(",LastLessingTime=") << (DWORD)p->timeLastLessing;
	pStream->FillString(",LastLessingLevel=") << p->nLessingLevel;
	//===============================每日在线奖励============================================
	// Jason 2010-3-15 v1.5.0神龙赐福扩展更新
	pStream->FillString(",LastLessingLevelEx=") << p->nLessingLevelEx;
	pStream->FillString(",LastLessingLoongDate=") << (DWORD)p->dateLastLessingLoong;
	pStream->FillString(",CurrentDayOnlineTime=") << p->dwCurrentDayOnlineTime;
	pStream->FillString(",OneDayFirstLoginedTime=") << (DWORD)p->timeOneDayFirstLogined;
	pStream->FillString(",OnlineRewardPerDayRecTime=") << (DWORD)p->timeOnlineRewardPerDayRec;
	//===============================每日在线奖励============================================
	//--------------------------------------------------------------------------------------------------
	// Jason 2010-1-30 v1.3.2宠物栏阀
	pStream->FillString(",FollowPetPocketValue=") << (DWORD)p->u16FollowPetPocketValve;
	pStream->FillString(",RidingPetPocketValue=") << (DWORD)p->u16RidingPetPocketValve;
	//--------------------------------------------------------------------------------------------------

	// 师徒相关
	pStream->FillString(",MasterID=") << (DWORD)p->dwMasterID;

	int nQinWuPoint = p->nQinWuPoint;
	int nJingWuPoint = p->nJingWuPoint;

	// 2147483647=2的31次方减1
	if (nJingWuPoint < 0)
	{
		nJingWuPoint = 0;
	}
	else if (nJingWuPoint > 2147483647)
	{
		nJingWuPoint = 2147483647;
	}
	// 2147483647=2的31次方减1
	if (nQinWuPoint < 0)
	{
		nQinWuPoint = 0;
	}
	else if (nQinWuPoint > 2147483647)
	{
		nQinWuPoint = 2147483647;
	}

	pStream->FillString(",JingWuPoint=") << nJingWuPoint;
	pStream->FillString(",QinWuPoint=") << nQinWuPoint;
	pStream->FillString(",MARelation=") << p->nMARelation;
	pStream->FillString(",MiraclePrcNum=") << (INT16)p->n16MiraclePracticeNum;

	pStream->FillString(",Buy50LvlItemFlag=") << p->byBuy50LvlItemFlag;
	pStream->FillString(",Buy60LvlItemFlag=") << p->byBuy60LvlItemFlag;
	pStream->FillString(",Buy70LvlItemFlag=") << p->byBuy70LvlItemFlag;
	pStream->FillString(",Buy80LvlItemFlag=") << p->byBuy80LvlItemFlag;
	pStream->FillString(",ExpPilularUseTimes=") << p->dwExpPilularUseTimes;

	// Jason 2010-3-29 v1.5.1 神迹积分
	pStream->FillString(",GodMiraclePoints=") << (INT16)p->nGodMiraclePoints;

	pStream->FillString(",TrainStateTransfered=") << p->byTrainStatesTransfered;

	// Jason 2010-5-18 v2.1.2 飞升标志
	pStream->FillString(",SoaringValue =") << p->nSoaringValue;
	pStream->FillString(",SoaringSkillLearnTimes =") << p->nSoaringSkillLearnTimes;
	pStream->FillString(",Toughness =") << p->nToughness;

	// 通过计算获得WorkedLevel字段，该字段由DB计算
	INT64 n64Tmp = 0;
	if ( p->dwLastUpgrade != 0)
	{
		n64Tmp = (X_LONG_STR_4WORKED_LEVEL * p->nLevel) + (X_LONG_STR_4WORKED_LEVEL - p->dwLastUpgrade);
		pStream->FillString(",WorkedLevel=") << n64Tmp;
	}
	else
	{
		n64Tmp = (X_LONG_STR_4WORKED_LEVEL * p->nLevel) + (X_LONG_STR_4WORKED_LEVEL - 1);
		pStream->FillString(",WorkedLevel=") << n64Tmp;
	}
	
	pStream->FillString(",CompleteRefreshTime=") << p->nCompleteRefreshTime;
	pStream->FillString(",IMRefreshTime=") << p->nIMRefreshTime;
	pStream->FillString(",YListCompleteRefreshTime=") << p->nYellowListCompleteRefreshTime;
	pStream->FillString(",YListIMRefreshTime=") << p->nYellowListIMRefreshTime;

	// Jason 2010-6-12 v1.0.0
	pStream->FillString(",ConsolidateTimes=") << p->nConsolidateTimes;
	pStream->FillString(",RoleState=") << p->dwRoleState;

	// Jason 2010-11-10 v1.2.2 公会战场每日领取在线奖励时间记录
	pStream->FillString(",TakeGuildRewardTime=") << (DWORD)p->timeTakeGuildReward;

	pStream->FillString(",PickupModeSetting=") << p->dwPickupModeSetting;

	pStream->FillString(",PlayerBack=") << p->byPlayerBack;
	pStream->FillString(",PlayerBackDays=") << p->byPlayerBackDays;

	pStream->FillString(",UseConstraintsMaxPKSafeGuardLevel=") << p->byUsingConstraintsPKSafeGuardLevel;
	pStream->FillString(",BePlayActLayerID=") << p->dwBePlayActLayerID;
	pStream->FillString(",BePlayActLevel=") << p->byBePlayActLevel;

	pStream->FillString(",GodHead=") << p->nGodhead;
	pStream->FillString(",GodFaith=") << p->nGodFaith;
	pStream->FillString(",GodCondenced=") << p->nGodCondense;

	// 神职相关
	pStream->FillString(",Clergy=") << p->dwClergy;
	pStream->FillString(",PreClergy=") << p->dwPreClergy;
	pStream->FillString(",ClergyMarsPoint=") << p->dwClergyMarsPoint;
	pStream->FillString(",ClergyApolloPoint=") << p->dwClergyApolloPoint;
	pStream->FillString(",ClergyRabbiPoint=") << p->dwClergyRabbiPoint;
	pStream->FillString(",ClergyPeacePoint=") << p->dwClergyPeacePoint;
	pStream->FillString(",ClergyCandidateType=") << p->eClergyCandidate;
	pStream->FillString(",Clergy4SeniorOrJunior=") << (p->eSeniorOrJunior);

	// KeyCode
	pStream->FillString(",KeyCodeRewarded=") << p->dwKeyCodeRewarded;

	// 家族ID
	pStream->FillString(",FamilyID=") << p->dwFamilyID;

	pStream->FillString(",TrainDate=") << p->dwTrainDate;

	pStream->FillString(",TrainNum=") << p->nTrainNum;

	pStream->FillString(",award_point=") << p->nAwardPoint;

	pStream->FillString(",award_flag=") << p->nAwardFlag;

	pStream->FillString(",BrotherTeacherID=") << p->dwBrotherTeatherID;

	pStream->FillString(",BrotherEndTime=") << p->timeBloodBrother;

	pStream->FillString(",PulseRemainTime=") << p->nPulseLearnRemainTime;

	pStream->FillString(",TigerTime=") << p->nTigerTime;

	pStream->FillString(",god_soul=") << p->nGod;
	pStream->FillString(",monster_soul=") << p->nMonster;
	pStream->FillString(",god_point=") << p->nGodPoint;
	pStream->FillString(",monster_point=") << p->nMonsterPoint;

	//鲜花鸡蛋排行by lib
	pStream->FillString(",flower_num=") << p->nFlower;
	pStream->FillString(",egg_num=") << p->nEgg;

	// 人物圣灵值
	pStream->FillString(",holy_value=") << p->nHolyValue;	
	// 命中
	pStream->FillString(",role_hit_add=") << p->nHidAdd;
	// 元素全伤
	pStream->FillString(",role_eei_all=") << p->nEEI_All;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	m_pDBLoong->ReturnConnection(pCon);
}

VOID CLoongDB::FormatSaveRoleInfo(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	tagRoleInfo *p = (tagRoleInfo *)pData;

	pStream->SetUpdate("roledata");
	pStream->FillString("MapID=") << p->dwMapID;
	pStream->FillString(",LoverID=") << p->dwLoverID;
	pStream->FillString(",MARelation=") << p->nMARelation;
	pStream->FillString(",VipLevel=") << p->nVIPLevel;
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 创建角色名贴信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertVCard(Beton::MyStream *pStream, DWORD dwRoleID, PVOID pDum)
{
	ASSERT(!P_VALID(pDum));

	pStream->SetInsert("visiting_card");
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 更新角色名贴信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateVCard(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagVCardData);
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetUpdate("visiting_card");

	pStream->FillString("Level=") << (INT32)p->dwLevel;
	pStream->FillString(",Job=") << (INT32)p->dwJob;
	pStream->FillString(",MateRoleID=") << (INT32)p->dwMateRoleID;
	pStream->FillString(",FactionID=") << (INT32)p->dwFactionID;
	pStream->FillString(",Position=") << (INT32)p->dwPosition;

	pStream->FillString(",Visibility=") << p->customVCardData.bVisibility;
	pStream->FillString(",Sex=") << p->customVCardData.bySex;
	pStream->FillString(",Constellation=") << p->customVCardData.byConstellation;
	pStream->FillString(",ChineseZodiac=") << p->customVCardData.byChineseZodiac;
	pStream->FillString(",Area=") << p->customVCardData.byArea;
	pStream->FillString(",Province=") << p->customVCardData.byProvince;
	pStream->FillString(",City='").FillString(p->customVCardData.chCity, pCon).FillString("'");
	pStream->FillString(",HeadUrl='").FillString(reinterpret_cast<TCHAR *>(p->customVCardData.byData), pCon).FillString("'");
	pStream->FillString(",Signature='").FillString(reinterpret_cast<TCHAR *>(&(p->customVCardData.byData[p->customVCardData.byHeadUrlSize])), pCon).FillString("'");
	pStream->FillString(",NameHistory='").FillString(reinterpret_cast<TCHAR *>(&(p->customVCardData.byData[p->customVCardData.byHeadUrlSize + p->customVCardData.bySignatureSize])), pCon).FillString("'");

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// 更新角色名贴信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelVCard(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDum)
{
	ASSERT(!P_VALID(pDum));
	pStream->SetDelete("visiting_card");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 创建角色名贴信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertClanData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDum)
{
	ASSERT(!P_VALID(pDum));
	
	pStream->SetInsert("clan_data");
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 更新角色氏族信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateClanData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	ASSERT(P_VALID(pData));
	MTRANS_POINTER(pDBRepData, pData, tagDBReputeData);

	pStream->SetUpdate("clan_data");

	pStream->FillString("RepXuanYuan=")		<< pDBRepData->m_nReputation[ECLT_XuanYuan];
	pStream->FillString(",ConXuanYuan=")	<< pDBRepData->m_nContribution[ECLT_XuanYuan];
	pStream->FillString(",ActCntXuanYuan=")	<< pDBRepData->m_n8ActCount[ECLT_XuanYuan];

	pStream->FillString(",RepShenNong=")	<< pDBRepData->m_nReputation[ECLT_ShenNong];
	pStream->FillString(",ConShenNong=")	<< pDBRepData->m_nContribution[ECLT_ShenNong];
	pStream->FillString(",ActCntShenNong=")	<< pDBRepData->m_n8ActCount[ECLT_ShenNong];

	pStream->FillString(",RepFuXi=")		<< pDBRepData->m_nReputation[ECLT_FuXi];
	pStream->FillString(",ConFuXi=")		<< pDBRepData->m_nContribution[ECLT_FuXi];
	pStream->FillString(",ActCntFuXi=")		<< pDBRepData->m_n8ActCount[ECLT_FuXi];

	pStream->FillString(",RepSanMiao=")		<< pDBRepData->m_nReputation[ECLT_SanMiao];
	pStream->FillString(",ConSanMiao=")		<< pDBRepData->m_nContribution[ECLT_SanMiao];
	pStream->FillString(",ActCntSanMiao=")	<< pDBRepData->m_n8ActCount[ECLT_SanMiao];

	pStream->FillString(",RepJiuLi=")		<< pDBRepData->m_nReputation[ECLT_JiuLi];
	pStream->FillString(",ConJiuLi=")		<< pDBRepData->m_nContribution[ECLT_JiuLi];
	pStream->FillString(",ActCntJiuLi=")	<< pDBRepData->m_n8ActCount[ECLT_JiuLi];

	pStream->FillString(",RepYueZhi=")		<< pDBRepData->m_nReputation[ECLT_YueZhi];
	pStream->FillString(",ConYueZhi=")		<< pDBRepData->m_nContribution[ECLT_YueZhi];
	pStream->FillString(",ActCntYueZhi=")	<< pDBRepData->m_n8ActCount[ECLT_YueZhi];

	pStream->FillString(",RepNvWa=")		<< pDBRepData->m_nReputation[ECLT_NvWa];
	pStream->FillString(",ConNvWa=")		<< pDBRepData->m_nContribution[ECLT_NvWa];
	pStream->FillString(",ActCntNvWa=")		<< pDBRepData->m_n8ActCount[ECLT_NvWa];

	pStream->FillString(",RepGongGong=")	<< pDBRepData->m_nReputation[ECLT_GongGong];
	pStream->FillString(",ConGongGong=")	<< pDBRepData->m_nContribution[ECLT_GongGong];
	pStream->FillString(",ActCntGongGong=")	<< pDBRepData->m_n8ActCount[ECLT_GongGong];

	pStream->FillString(",FameMask=")		<< pDBRepData->m_u16FameMask;
	pStream->FillString(",RepRstTimeStamp=")<< pDBRepData->m_dwtLastResetTime;

	pStream->FillString(",TotalFame=")		<< pDBRepData->m_dwTotalRep;
	//pStream->FillString(",FamePm=")			<< pDBRepData->m_nRepPm;

	pStream->SetWhere();
	pStream->FillString("RoleID=")			<< dwRoleID;

}

//-------------------------------------------------------------------------------------------------------
// 删除角色名贴信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelClanData(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDum)
{
	ASSERT(!P_VALID(pDum));

	pStream->SetDelete("clan_data");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 插入角色技能信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateItemPosAndUseRel(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagItemUpdate);

	pStream->SetUpdate("item");
	pStream->FillString("Num=") << p->n16Num;
	pStream->FillString(",UseTimes=") << p->nUseTimes;
	pStream->FillString(",OwnerID=") << p->dwOwnerID;
	pStream->FillString(",AccountID=") << p->dwAccountID;
	pStream->FillString(",ContainerTypeID=") << p->byConType;
	pStream->FillString(",Suffix=") << p->n16Index;
	pStream->FillString(",Bind=") << p->byBindType;
// 	pStream->FillString(",UnlockTime=") << p->dwUnlockTime;
	pStream->FillString(",ItemSpecVal1=") << p->dwSpecVal1;
	pStream->FillString(",ItemSpecVal2=") << p->dwSpecVal2;

	CHAR szTime[X_DATATIME_LEN + 1];	// 时间转换用字符串
	DwordTime2DataTime(szTime, sizeof(szTime), p->dw1stGainTime);
	pStream->FillString(",FirstGainTime='").FillString(szTime);
	pStream->FillString("'");

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << p->n64Serial;
}

//-------------------------------------------------------------------------------------------------------
// 插入角色技能信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertSkill(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagSkillSave);

	pStream->SetInsert("skill");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",ID=") << p->dwID;
	pStream->FillString(",BiddenLevel=") << p->nLearnLevel;
	pStream->FillString(",SelfLevel=") << p->nSelfLevel;
	pStream->FillString(",Proficiency=") << p->nProficiency;
	pStream->FillString(",CoolDown=") << p->nCoolDown;
	pStream->FillString(",active_time=") << p->dwActiveTime;
	
}

//-------------------------------------------------------------------------------------------------------
// 插入角色技能信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelSkill(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetDelete("skill");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and ID=") << (*(DWORD*)pData);
}

//-------------------------------------------------------------------------------------------------------
// 保存角色技能信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveSkill(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagSkillSave);

	pStream->SetReplace("skill");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",ID=") << p->dwID;
	pStream->FillString(",BiddenLevel=") << p->nLearnLevel;
	pStream->FillString(",SelfLevel=") << p->nSelfLevel;
	pStream->FillString(",Proficiency=") << p->nProficiency;
	pStream->FillString(",CoolDown=") << p->nCoolDown;
	pStream->FillString(",active_time=") << p->dwActiveTime;
}

//-------------------------------------------------------------------------------------------------------
// 保存角色buff信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveRoleBuff(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pBuff)
{
	MTRANS_POINTER(p, pBuff, tagBuffSave);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetInsert("buff");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",SrcUnitID=") << p->dwSrcUnitID;
	pStream->FillString(",SrcSkillID=") << p->dwSrcSkillID;
	pStream->FillString(",ItemTypeID=") << p->dwItemTypeID;
	pStream->FillString(",ItemSerialID=") << p->n64Serial;

	pStream->FillString(",BuffID=") << p->dwBuffID;
	pStream->FillString(",CurTick=") << p->nPersistTick;
	pStream->FillString(",Level=") << p->n8Level;
	pStream->FillString(",CurLapTimes=") << p->n8CurLapTimes;

	if(p->n8ModifierNum > 0)
	{
		pStream->FillString(",EffectSkillID='");
		pStream->FillBlob(p->byData, p->n8ModifierNum * sizeof(DWORD), pCon);
		pStream->FillString("'");
	}

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// 保存称号信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveTitle(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagTitleSave);

	pStream->SetUpdate("title");
	pStream->FillString("Count=") << p->dwStateMark;
	pStream->FillString(",TimeRemain=")<< p->dwTimeRemain;

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and ID=") << p->u16TitleID;
}

//-------------------------------------------------------------------------------------------------------
// 保存任务信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveTask(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTask)
{
	tagQuestSave *p = (tagQuestSave *)pTask;

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->SetReplace("task");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",TaskID=") << p->u16QuestID;
	pStream->FillString(",TaskFlag=") << p->bQuestFlag;
	pStream->FillString(",StartTime=") << p->dwAcceptTime;

	for(INT32 i=0; i<QUEST_CREATURES_COUNT; ++i)
	{
		pStream->FillString(",Monster") << i;
		pStream->FillString("Num=") << (INT16)p->n16MonsterNum[i];
	}

	pStream->FillString(",ChuandaoCount=") << p->nChuandaoCount;
	pStream->FillString(",ScriptData='");
	pStream->FillBlob(p->Data.dwData, sizeof(DWORD)*ESD_Quest, pCon);
	pStream->FillString("'");

	if(p->DynamicTarget.eTargetType != EQTT_NULL)
	{
		pStream->FillString(",DynamicTarget='");
		pStream->FillBlob(&p->DynamicTarget, sizeof(tagQuestDynamicTarget), pCon);
		pStream->FillString("'");
	}
	
	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// 保存已完成任务信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveTaskDone(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTask)
{
	tagQuestDoneSave *p = (tagQuestDoneSave *)pTask;

	pStream->SetReplace("task_done");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",TaskID=") << p->u16QuestID;
	pStream->FillString(",StartTime=") << p->dwStartTime;
	pStream->FillString(",Times=") << p->nTimes;
}

//-------------------------------------------------------------------------------------------------------
// 保存已完成日常任务信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveDailyTaskDone(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pTask)
{
	tagQuestDoneSave *p = (tagQuestDoneSave *)pTask;

	pStream->SetReplace("dailytask_done");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",TaskID=") << p->u16QuestID;
	pStream->FillString(",StartTime=") << p->dwStartTime;
	pStream->FillString(",Times=") << p->nTimes;
}

//-------------------------------------------------------------------------------------------------------
// 从状态表中删除指定记录
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatDelAllRoleBuff(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pDummy)
{
	pStream->SetDelete("buff");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
}

//-------------------------------------------------------------------------------------------------------
// 保存固定活动脚本数据
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveActivity(Beton::MyStream *pStream, DWORD dwID, LPVOID pActivityData)
{
	tagActivityData *p = (tagActivityData *)pActivityData;
	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();
	pStream->SetReplace("activity");
	pStream->FillString("dwID=") << dwID;
	pStream->FillString(",ScriptData='");
	pStream->FillBlob(&p->Data.dwData, sizeof(DWORD)*ESD_Activity, pCon);
	pStream->FillString("'");
	pStream->FillString(" ,ScriptData1='");
	pStream->FillBlob(&p->MergedData[0].MergedScriptData.dwData, sizeof(DWORD)*ESD_Activity, pCon);
	pStream->FillString("'");
	pStream->FillString(" ,ScriptData2='");
	pStream->FillBlob(&p->MergedData[1].MergedScriptData.dwData, sizeof(DWORD)*ESD_Activity, pCon);
	pStream->FillString("'");
	pStream->FillString(" ,ScriptData3='");
	pStream->FillBlob(&p->MergedData[2].MergedScriptData.dwData, sizeof(DWORD)*ESD_Activity, pCon);
	pStream->FillString("'");
	pStream->FillString(" ,ScriptData4='");
	pStream->FillBlob(&p->MergedData[3].MergedScriptData.dwData, sizeof(DWORD)*ESD_Activity, pCon);
	pStream->FillString("'");
	m_pDBLoong->ReturnConnection(pCon);
}
//-------------------------------------------------------------------------------------------------------
// 创建默认城市信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatCreateCityInfo(Beton::MyStream *pStream, DWORD dwCityID, PVOID pData)
{
	pStream->Clear();
	pStream->SetInsert("city");
	pStream->FillString("id=") << dwCityID;
}

//-------------------------------------------------------------------------------------------------------
// 保存城市信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveCityInfo(Beton::MyStream *pStream, DWORD dwCityID, PVOID pData)
{
	MTRANS_POINTER(p, pData, tagCityAtt2DB);

	pStream->Clear();
	pStream->SetUpdate("city");
	pStream->FillString("guild_id=") << p->dwGuildID;
	pStream->FillString(",defence=") << p->nDefence;
	pStream->FillString(",eudemon_tally=") << p->nEudemonTally;
	pStream->FillString(",tax_rate=") << p->nTaxRate;
	pStream->FillString(",tax_rate_time=") << p->dwTaxRateTime;
	pStream->FillString(",taxation=") << p->nTaxation;
	pStream->FillString(",prolificacy=") << p->nProlificacy;
	pStream->SetWhere();
	pStream->FillString("id=") << p->dwCityID;
}

//-------------------------------------------------------------------------------------------------------
// 更新指定城市属性
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatUpdateCityInfo(Beton::MyStream *pStream, DWORD dwCityID, PVOID pData)
{
	MTRANS_POINTER(p, pData, tagCityAppointedAtt);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->Clear();
	pStream->SetUpdate("city");

	switch (p->eType)
	{
	case ECAT_Holder:
		pStream->FillString("guild_id=") << *(DWORD*)(p->byData);
		break;

	case ECAT_Defence:
		pStream->FillString("defence=") << *(INT32*)(p->byData);
		break;

	case ECAT_EudemonTally:
		pStream->FillString("eudemon_tally=") << *(INT32*)(p->byData);
		break;

	case ECAT_TaxRate:
		pStream->FillString("tax_rate=") << *(INT32*)(p->byData);
		break;
		
	case ECAT_TaxRateTime:
		pStream->FillString("tax_rate_time=") << *(DWORD*)(p->byData);
		break;

	case ECAT_Taxation:
		pStream->FillString("taxation=") << *(INT32*)(p->byData);
		break;

	case ECAT_Prolificacy:
		pStream->FillString("prolificacy=") << *(INT32*)(p->byData);
		break;

	case ECAT_AttackSignUp:
	case ECAT_DefenceSignUp:
	case ECAT_RequestSignUp:
		{
			pStream->FillString("signup_list='").FillBlob(p->byData, sizeof(tagCitySignUp) * p->nDataNum, pCon);
			pStream->FillString("'");
		}
		break;
	default:
		pStream->FillString("id=") << dwCityID;
		break;
	}

	pStream->SetWhere();
	pStream->FillString("id=") << dwCityID;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// 保存师徒信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveAllMasterApprenticeData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagMasterApprentice2DB);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	int nQinWuPoint = p->nQinWuPoint;
	int nJingWuPoint = p->nJingWuPoint;

	// 2147483647=2的31次方减1
	if (nJingWuPoint < 0)
	{
		nJingWuPoint = 0;
	}
	else if (nJingWuPoint > 2147483647)
	{
		nJingWuPoint = 2147483647;
	}
	// 2147483647=2的31次方减1
	if (nQinWuPoint < 0)
	{
		nQinWuPoint = 0;
	}
	else if (nQinWuPoint > 2147483647)
	{
		nQinWuPoint = 2147483647;
	}

	pStream->Clear();
	pStream->SetUpdate("master");
	pStream->FillString("dwID=") << p->dwID;
	pStream->FillString(",JingWuPoint=") << nJingWuPoint;
	pStream->FillString(",QinWuPoint=") << nQinWuPoint;
	pStream->FillString(",LevelUpBonus=") << p->nLevelUpBonus;
	pStream->FillString(",ApprenticeData='").FillBlob(p->sApprentice.dwData, sizeof(tagApprenticeData), pCon);
	pStream->FillString("'");
	pStream->SetWhere();
	pStream->FillString("dwID=") << p->dwID;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// 保存一项师徒信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatSaveOneMasterApprenticeData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagOneMASaveData);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->Clear();
	pStream->SetUpdate("master");

	switch(p->oneMAData.eType)
	{
	case EOMAD_JingWu:
		{
			INT nJingWuPoint = 0;
			if (p->oneMAData.eMode == EMAMM_Mod)
			{
				nJingWuPoint = p->loadData.nJingWuPoint + *(INT*)(p->oneMAData.byData);
			}
			else
			{
				nJingWuPoint = *(INT*)p->oneMAData.byData;
			}

			// 2147483647=2的31次方减1
			if (nJingWuPoint < 0)
			{
				nJingWuPoint = 0;
			}
			else if (nJingWuPoint > 2147483647)
			{
				nJingWuPoint = 2147483647;
			}
			pStream->FillString("JingWuPoint=") << nJingWuPoint;
			break;
		}
		

	case EOMAD_QinWu:
		{
			INT nQinWuPoint = 0;
			if (p->oneMAData.eMode == EMAMM_Mod)
			{
				nQinWuPoint = p->loadData.nQinWuPoint + *(INT*)(p->oneMAData.byData);
			}
			else
			{
				nQinWuPoint = *(INT*)(p->oneMAData.byData);
			}
			// 2147483647=2的31次方减1
			if (nQinWuPoint < 0)
			{
				nQinWuPoint = 0;
			}
			else if (nQinWuPoint > 2147483647)
			{
				nQinWuPoint = 2147483647;
			}
			pStream->FillString("QinWuPoint=") << nQinWuPoint;
			break;
		}
		

	case EOMAD_LevelUpBonus:
		{
			INT nLevelUpBonus = 0;
			if (p->oneMAData.eMode == EMAMM_Mod)
			{
				nLevelUpBonus = p->loadData.nLevelUpBonus + *(INT*)(p->oneMAData.byData);
			}
			else
			{
				nLevelUpBonus = *(INT*)(p->oneMAData.byData);
			}

			pStream->FillString("LevelUpBonus=") << nLevelUpBonus;
			break;
		}


	case EOMAD_RewardNum:
		{
			INT nRewardNum = 0;
			if (p->oneMAData.eMode == EMAMM_Mod)
			{
				nRewardNum = p->loadData.nRewardNum + *(INT*)(p->oneMAData.byData);
			}
			else
			{
				nRewardNum = *(INT*)(p->oneMAData.byData);
			}

			pStream->FillString("RewardNum=") << nRewardNum;
			break;
		}


	case EOMAD_Apprentice:
		{
			if (p->oneMAData.eMode == EMAMM_Mod)
			{
				int nNum = sizeof(p->loadData.sApprentice.dwData) / sizeof(DWORD);
				int nIndex;
				for (nIndex=0; nIndex!=nNum; ++nIndex)
				{
					if (p->loadData.sApprentice.dwData[nIndex] == 0)
					{
						break;
					}
				}

				p->loadData.sApprentice.dwData[nIndex] = *(DWORD*)(p->oneMAData.byData);
				pStream->FillString("ApprenticeData='").FillBlob(p->loadData.sApprentice.dwData, sizeof(tagApprenticeData), pCon);
				pStream->FillString("'");
			}

			else
			{
				pStream->FillString("ApprenticeData='").FillBlob((DWORD*)(p->oneMAData.byData), sizeof(tagApprenticeData), pCon);
				pStream->FillString("'");
			}

			break;
		}
		

	default:
		break;
	}

	pStream->SetWhere();
	pStream->FillString("dwID=") << dwID;

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// 插入师徒信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::FormatInsertMasterApprenticeData(Beton::MyStream *pStream, DWORD dwID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagMasterApprentice2DB);

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	int nQinWuPoint = p->nQinWuPoint;
	int nJingWuPoint = p->nJingWuPoint;

	// 2147483647=2的31次方减1
	if (nJingWuPoint < 0)
	{
		nJingWuPoint = 0;
	}
	else if (nJingWuPoint > 2147483647)
	{
		nJingWuPoint = 2147483647;
	}
	// 2147483647=2的31次方减1
	if (nQinWuPoint < 0)
	{
		nQinWuPoint = 0;
	}
	else if (nQinWuPoint > 2147483647)
	{
		nQinWuPoint = 2147483647;
	}

	pStream->Clear();
	pStream->SetInsert("master");
	pStream->FillString("dwID=") << p->dwID;
	pStream->FillString(",JingWuPoint=") << nJingWuPoint;
	pStream->FillString(",QinWuPoint=") << nQinWuPoint;
	pStream->FillString(",LevelUpBonus=") << p->nLevelUpBonus;
	pStream->FillString(",ApprenticeData='").FillBlob(p->sApprentice.dwData, sizeof(tagApprenticeData), pCon);
	pStream->FillString("'");

	m_pDBLoong->ReturnConnection(pCon);
}

//-------------------------------------------------------------------------------------------------------
// 保存物品锁定相关信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::SaveItemLockInfo(INT64 n64ItemID, BYTE byType, DWORD dwUnlockTime)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return;

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->Clear();
	pStream->SetUpdate("item");
	pStream->FillString("LockStat=") << byType;
	pStream->FillString(",UnlockTime=") << dwUnlockTime;
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64ItemID;

	// 释放连接
	m_pDBLoong->ReturnConnection(pCon);

	// 查询
	m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);
}

//-------------------------------------------------------------------------------------------------------
// 保存物品绑定状态
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::SaveItemBindInfo(INT64 n64ItemID, BYTE byType)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return;

	Beton::Connection* pCon = m_pDBLoong->GetFreeConnection();

	pStream->Clear();
	pStream->SetUpdate("item");
	pStream->FillString("Bind=") << byType;
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64ItemID;

	// 释放连接
	m_pDBLoong->ReturnConnection(pCon);

	// 查询
	m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);
}

VOID CLoongDB::SaveRoleSpeak(DWORD dwRoleID, bool bState)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return;

	INT nTmp = (bState) ? 0:1;

	pStream->SetUpdate("roledata");
	pStream->FillString("SpeakOff=") << nTmp;
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// 执行
	m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);
}

DWORD CLoongDB::CheckName(LPCTSTR szName)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return E_SystemError;

	pStream->SetSelect("roledata", "*");
	pStream->SetWhere();
	pStream->FillString("RoleName='").FillString(szName).FillString("'");

	DWORD dwErr = E_Success;
	Beton::QueryResult* pResult = m_pDBLoong->Query(pStream);
	if(!P_VALID(pResult))
	{
		dwErr = E_SystemError;
	}
	else if (pResult->GetRowCount() > 0)
	{
		dwErr = E_CreateRole_NameExist;
	}

	// 释放流
	m_pDBLoong->ReturnStream(pStream);
	m_pDBLoong->FreeQueryResult(pResult);

	return dwErr;
}

////-------------------------------------------------------------------------------------------------------
//// 从物品表中删除指定记录
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::FormatDelRecItem(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	INT64 *p = (INT64 *)pData;
//
//	pStream->SetDelete("item");
//	pStream->SetWhere();
//	pStream->FillString("RoleID=") << dwRoleID;
//	pStream->FillString(" and SerialNum=") << *p;
//}
//
////-------------------------------------------------------------------------------------------------------
//// 从装备表中删除指定记录
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::FormatDelRecEquip(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	INT64 *p = (INT64 *)pData;
//
//	pStream->SetDelete("equip");
//	pStream->SetWhere();
//	pStream->FillString("RoleID=") << dwRoleID;
//	pStream->FillString(" and SerialNum=") << *p;
//}
//
////-------------------------------------------------------------------------------------------------------
//// 从任务表中删除指定记录
////-------------------------------------------------------------------------------------------------------
//VOID CLoongDB::FormatDelRecTask(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
//{
//	DWORD *p = (DWORD *)pData;
//
//	pStream->SetDelete("task");
//	pStream->SetWhere();
//	pStream->FillString("RoleID=") << dwRoleID;
//	pStream->FillString(" and TaskID=") << *p;
//}

//-------------------------------------------------------------------------------------------------------
//  保存一个试炼的状态（nTrainState--0:未开启; 1:进行中; 2:已通过(已经领取完奖励); 3:已完成(没有领取奖励)）
//-------------------------------------------------------------------------------------------------------
VOID CLoongDB::SaveOpenTrainState(DWORD dwRoleID, DWORD dwTrainID, INT nTrainState, DWORD dwCompleteness)
{
	if (nTrainState < 0 || nTrainState >3)
		return;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return;

	pStream->Clear();
	pStream->SetReplace("train_state");
	pStream->FillString("TrainState=") << nTrainState;
	pStream->FillString(",Completeness=") << dwCompleteness;
	pStream->FillString(",RoleID=") << dwRoleID;
	pStream->FillString(",TrainID=") <<dwTrainID;

	// 执行
	m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);
}

VOID CLoongDB::SaveAvatarFaceAndHair(DWORD dwRoleID,
						   WORD wHairMdlID,			// 发型
						   WORD wHairTexID,			// 发色
						   WORD wFaceMdlID,			// 面部五官
						   WORD wFaceDetailTexID)	// 面部细节
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return;

	pStream->Clear();
	pStream->FillString("update roledata set HairModelID=") << wHairMdlID;
	pStream->FillString(",HairColorID=") << wHairTexID;
	pStream->FillString(",FaceModelID=") << wFaceMdlID;
	pStream->FillString(",FaceDetailID=") << wFaceDetailTexID;
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// 执行
	m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);
}

// BOOL CLoongDB::SaveBoardQuest(DWORD dwID, LPVOID pData, INT32 nNum, LPVOID* ppOutPointer)
// {
// 	if ( nNum < QUEST_BOARD_MIN_CAPACITY || nNum > QUEST_BOARD_MAX_CAPACITY )
// 	{
// 		return FALSE;
// 	}
// 
// 	Beton::MyStream* pStream = m_pDBLoong->GetStream();
// 	if(!P_VALID(pStream)) return FALSE;
// 
// 	tagBoardQuestSaveData *pCurPointer = (tagBoardQuestSaveData*)pData;
// 
// 	for(INT32 i=0; i<nNum; ++i)
// 	{
// 		if ( !P_VALID(pCurPointer->u16BoardID) || !P_VALID(pCurPointer->u16QuestID) 
// 			|| pCurPointer->u16QuestID > QUEST_PANEL_END_TASK_ID
// 			|| pCurPointer->u16QuestID < QUEST_PANEL_START_TASK_ID 
// 			|| pCurPointer->byType <= EQBT_Null
// 			|| pCurPointer->byType > EQBT_Guild)
// 		{
// 			continue;
// 		}
// 
// 		pStream->Clear();
// 		pStream->SetReplace("quest_board");
// 		pStream->FillString("BoardID=") << pCurPointer->u16BoardID;
// 		pStream->FillString(",State=") << pCurPointer->eState;
// 		pStream->FillString(",RoleID=") << dwID;
// 		pStream->FillString(",QuestID=") << pCurPointer->u16QuestID;
// 		pStream->FillString(",BoardType=") << pCurPointer->byType;
// 
// 		// 执行
// 		m_pDBLoong->Execute(pStream);
// 
// 		++pCurPointer;
// 	}
// 
// 	*ppOutPointer = pCurPointer;
// 
// 	// 释放流
// 	m_pDBLoong->ReturnStream(pStream);
// 
// 	return TRUE;
// }

BOOL CLoongDB::InsertGuildWarDeclare(DWORD			dwAttackGuildID,			// 宣战工会ID
									  DWORD			dwDefenceGuildID,			// 被宣战工会ID
									  tagDWORDTime	dwWarDeclareTime,			// 宣战时间
									  BOOL			bWarEnded)					// 是不是已经结束过了
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;
	INT nWarEnded = bWarEnded?1:0;

	pStream->Clear();
	pStream->SetInsert("guild_war_declare");
	pStream->FillString("AttackGuildID=") << dwAttackGuildID;
	pStream->FillString(",DefenceGuildID=") << dwDefenceGuildID;
	pStream->FillString(",WarDeclareTime=") << dwWarDeclareTime;
	pStream->FillString(",WarEnded=") << nWarEnded;

	// 执行
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

BOOL CLoongDB::SetGuildWarDeclareEnded(DWORD		dwAttackGuildID,			// 宣战工会ID
									   DWORD		dwDefenceGuildID,			// 被宣战工会ID
									   tagDWORDTime	dwWarDeclareTime)			// 宣战时间
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetUpdate("guild_war_declare");
	pStream->FillString("WarEnded=") << 1;
	pStream->SetWhere();
	pStream->FillString("AttackGuildID=") << dwAttackGuildID;
	pStream->FillString(" and DefenceGuildID=") << dwDefenceGuildID;
	pStream->FillString(" and WarDeclareTime=") << dwWarDeclareTime;

	// 执行
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

BOOL CLoongDB::InsertIntoAttackSignUpList(EGodMiracle eGodID, DWORD dwGuildID, DWORD dwBidFund)
{
	// 参数合法性检查一下
	if (eGodID < EGM_NH || eGodID >= EGM_MAX)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetInsert("battlefield_attack_signup");
	pStream->FillString("BattleFieldID=") << static_cast<DWORD>(eGodID);
	pStream->FillString(",GuildID=") << dwGuildID;
	pStream->FillString(",BidFund=") << dwBidFund;

	// 执行
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

BOOL CLoongDB::InsertIntoAllianceList(EGodMiracle eGodID, DWORD dwGuildID, EGuildCapType eAllianceType, BOOL bConfirmed)
{
	// 参数合法性检查一下
	if (eGodID < EGM_NH || eGodID >= EGM_MAX)
		return FALSE;
	if (eAllianceType != EGCT_Defend && eAllianceType != EGCT_Capture)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	DWORD dwBeConfirmed = (bConfirmed==FALSE)?0:1;
	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetInsert("battlefield_alliance_signup");
	pStream->FillString("BattleFieldID=") << static_cast<DWORD>(eGodID);
	pStream->FillString(",GuildID=") << dwGuildID;
	pStream->FillString(",GuildCapType=") << static_cast<DWORD>(eAllianceType);
	pStream->FillString(",BeConfirmed=") << dwBeConfirmed;

	// 执行
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::ConfirmAlliance(EGodMiracle eGodID, DWORD dwGuildID)
{
	// 参数合法性检查一下
	if (eGodID < EGM_NH || eGodID >= EGM_MAX)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetUpdate("battlefield_alliance_signup");
	pStream->FillString("BeConfirmed=") << 1;
	pStream->SetWhere();
	pStream->FillString("BattleFieldID=") << static_cast<DWORD>(eGodID);
	pStream->FillString(" and GuildID=") << dwGuildID;

	// 执行
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::ModifyBidFund(EGodMiracle eGodID, DWORD dwGuildID, DWORD dwBidFund)
{
	// 参数合法性检查一下
	if (eGodID < EGM_NH || eGodID >= EGM_MAX)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetUpdate("battlefield_attack_signup");
	pStream->FillString("BidFund=") << dwBidFund;
	pStream->SetWhere();
	pStream->FillString("BattleFieldID=") << static_cast<DWORD>(eGodID);
	pStream->FillString(" and GuildID=") << dwGuildID;

	// 执行
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::SaveOnlineTimeEachMonth(DWORD dwAccountID, DWORD dwRoleID, BYTE byYear, UINT16* pn16OnlineTime)
{
	if (!P_VALID(pn16OnlineTime))
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;
	pStream->Clear();
	pStream->SetReplace("role_online_time_each_month");
	pStream->FillString("AccountID=")<<dwAccountID;
	pStream->FillString(",RoleID=")<<dwRoleID;
	pStream->FillString(",Year=")<<byYear;
	pStream->FillString(",January=")<<pn16OnlineTime[EM_January];
	pStream->FillString(",February=")<<pn16OnlineTime[EM_February];
	pStream->FillString(",March=")<<pn16OnlineTime[EM_March];
	pStream->FillString(",April=")<<pn16OnlineTime[EM_April];
	pStream->FillString(",May=")<<pn16OnlineTime[EM_May];
	pStream->FillString(",June=")<<pn16OnlineTime[EM_June];
	pStream->FillString(",July=")<<pn16OnlineTime[EM_July];
	pStream->FillString(",August=")<<pn16OnlineTime[EM_August];
	pStream->FillString(",September=")<<pn16OnlineTime[EM_September];
	pStream->FillString(",October=")<<pn16OnlineTime[EM_October];
	pStream->FillString(",November=")<<pn16OnlineTime[EM_November];
	pStream->FillString(",December=")<<pn16OnlineTime[EM_December];
	pStream->FillString(",LastUpdateTime=now()");

	// 执行
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::QuestBoardSaveAllQuest(LPVOID pData, INT32 nNum)
{
	if ( nNum < QUEST_BOARD_MIN_CAPACITY || nNum > QUEST_BOARD_MAX_CAPACITY )
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	tagBoardQuestSaveData *pCurPointer = (tagBoardQuestSaveData*)pData;

	for(INT32 i=0; i<nNum; ++i)
	{
		if ( !P_VALID(pCurPointer->u16BoardID) || !P_VALID(pCurPointer->u16QuestID) 
			|| pCurPointer->u16QuestID > QUEST_PANEL_END_TASK_ID
			|| pCurPointer->u16QuestID < QUEST_PANEL_START_TASK_ID
 			|| pCurPointer->byType <= EQBT_Null
 			|| pCurPointer->byType > EQBT_Guild)
		{
			continue;
		}

		pStream->Clear();
		pStream->SetReplace("quest_board");
		pStream->FillString("BoardID=") << pCurPointer->u16BoardID;
		pStream->FillString(",State=") << pCurPointer->eState;
		pStream->FillString(",RoleID=") << pCurPointer->dwRoleID;
		pStream->FillString(",QuestID=") << pCurPointer->u16QuestID;
		pStream->FillString(",BoardType=") << pCurPointer->byType;

		// 执行
		m_pDBLoong->Execute(pStream);

		++pCurPointer;
	}

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return TRUE;
}

DWORD CLoongDB::QuestBoardSaveOneQuest(LPVOID pData)
{
	tagBoardQuestSaveData *pCurPointer = (tagBoardQuestSaveData*)pData;
	if ( !P_VALID(pCurPointer->u16BoardID) || !P_VALID(pCurPointer->u16QuestID) 
		|| pCurPointer->u16QuestID > QUEST_PANEL_END_TASK_ID
		|| pCurPointer->u16QuestID < QUEST_PANEL_START_TASK_ID
		|| pCurPointer->byType <= EQBT_Null
		|| pCurPointer->byType > EQBT_Guild)
	{
		return FALSE;
	}

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->Clear();
	pStream->SetReplace("quest_board");
	pStream->FillString("BoardID=") << pCurPointer->u16BoardID;
	pStream->FillString(",State=") << pCurPointer->eState;
	pStream->FillString(",RoleID=") << pCurPointer->dwRoleID;
	pStream->FillString(",QuestID=") << pCurPointer->u16QuestID;
	pStream->FillString(",BoardType=") << pCurPointer->byType;

	// 执行
	m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return TRUE;
}

DWORD CLoongDB::SyncCanRankFlag(DWORD dwRoleID, BYTE byCanRankFlag)
{
	if (byCanRankFlag != 0 && byCanRankFlag != 1)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	pStream->Clear();
	pStream->SetUpdate("roledata");
	pStream->FillString("CanRankFlag=") << byCanRankFlag;
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// 执行
	m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return TRUE;
}

DWORD CLoongDB::SaveMediaOnlineNum(DWORD num,VOID * pData)
{
	if( num <= 0 || !P_VALID(pData))
		return GT_INVALID;

	tagMediaOnlineNum * pOlData = (tagMediaOnlineNum*)pData;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		while( num-- > 0 )
		{
			pStream->Clear();
			pStream->FillString  ("REPLACE mediaonline SET DistributionID = ") << pOlData->nDistributionID;
			pStream->FillString(",OnlineNum = ") << pOlData->dwOnlineNum;
			pStream->FillString(" ,RecTime = now()") ;
			m_pDBLoong->Execute  ( pStream ) ;
			++pOlData;
		}
		m_pDBLoong->ReturnStream(pStream);
		return 0;
	}
	return GT_INVALID;
}

DWORD CLoongDB::UpdateFabaoMood(DWORD pDBMsg,DWORD dwDummy)
{
	if( !P_VALID(pDBMsg))
		return GT_INVALID;

	tagNDBC_FabaoMoodChanged * pCmd = (tagNDBC_FabaoMoodChanged*)pDBMsg;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString  ("UPDATE fabao SET Mood = ") << pCmd->nMood;
		pStream->FillString(",MoodTicks = ") << pCmd->nMoodTicks;
		pStream->FillString(" WHERE SerialNum = ") << pCmd->n64SerialID;
		m_pDBLoong->Execute  ( pStream ) ;
		m_pDBLoong->ReturnStream(pStream);
		return E_Success;
	}
	return GT_INVALID;
}

DWORD CLoongDB::UpdateHolyCoValue(DWORD pDBMsg,DWORD dwDummy)
{
	if( !P_VALID(pDBMsg))
		return GT_INVALID;

	tagNDBC_HolyCoValue * pCmd = (tagNDBC_HolyCoValue*)pDBMsg;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString  ("UPDATE holyman SET CoValue = ") << pCmd->nCoValue;		
		pStream->FillString(" WHERE SerialNum = ") << pCmd->n64SerialID;
		m_pDBLoong->Execute  ( pStream ) ;
		m_pDBLoong->ReturnStream(pStream);
		return E_Success;
	}
	return GT_INVALID;
}

DWORD	CLoongDB::UpdateFabaoEleInjuryType(DWORD pDBMsg,DWORD dwDummy)
{
	if( !P_VALID(pDBMsg))
		return GT_INVALID;

	tagNDBC_UpdateFabaoEleInjuryType * pCmd = (tagNDBC_UpdateFabaoEleInjuryType*)pDBMsg;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString  ("UPDATE fabao SET EleInjuryType = ") << pCmd->eInjury;
		pStream->FillString(" WHERE SerialNum = ") << pCmd->n64FabaoID;
		m_pDBLoong->Execute  ( pStream ) ;
		m_pDBLoong->ReturnStream(pStream);
		return E_Success;
	}
	return GT_INVALID;
}

DWORD CLoongDB::UpdateFabaoNirVanaData(DWORD pDBMsg,DWORD)
{
	if( !P_VALID(pDBMsg))
		return GT_INVALID;

	tagNDBC_UpdateFabaoNirVanaData * pCmd = (tagNDBC_UpdateFabaoNirVanaData*)pDBMsg;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	//ASSERT(P_VALID(pStream));
	if( P_VALID(pStream) )
	{
		pStream->Clear();
		pStream->FillString  ("Replace fabao_extend_his SET SerialNum = ") << pCmd->_n64ID;
		pStream->FillString(" ,Stage = ") << pCmd->_data.nStage;
		pStream->FillString(" ,EleInjuryType = ") << pCmd->_data.nEleInjuryType;
		pStream->FillString(" ,EleInjury = ") << pCmd->_data.nEleInjury;
		pStream->FillString(" ,ElePierce = ") << pCmd->_data.nElePierce;
		pStream->FillString(" ,EleResisFire = ") << pCmd->_data.nEleResistance[EER_Fire];
		pStream->FillString(" ,EleResisWater = ") << pCmd->_data.nEleResistance[EER_Water];
		pStream->FillString(" ,EleResisEarth = ") << pCmd->_data.nEleResistance[EER_Earth];
		pStream->FillString(" ,EleResisWind = ") << pCmd->_data.nEleResistance[EER_Wind];
		pStream->FillString(" ,EleResisBright = ") << pCmd->_data.nEleResistance[EER_Bright];
		pStream->FillString(" ,EleResisDark = ") << pCmd->_data.nEleResistance[EER_Dark];
		pStream->FillString(" ,DamageUpgradeTimes = ") << pCmd->_data.wDamageUpgradeTimes;
		pStream->FillString(" ,ResistanceUpgradeTimes = ") << pCmd->_data.wResistanceUpgradeTimes;
		pStream->FillString(" ,EleInjPromoteTimes = ") << pCmd->_data.wEleInjPromoteTimes;
		pStream->FillString(" ,EleResPromoteTimes = ") << pCmd->_data.wEleResPromoteTimes;
		pStream->FillString(",RoleID = ") << pCmd->dwRoleID;
		pStream->FillString(",AccountID = ") << pCmd->dwAccountID;
		m_pDBLoong->Execute  ( pStream ) ;
		m_pDBLoong->ReturnStream(pStream);
		return E_Success;
	}
	return GT_INVALID;
}

VOID CLoongDB::InitFabaoUpdate(BOOL bInsertOrNot,Beton::MyStream *pStream,Beton::Connection* pCon,const tagFabao * pFabao)
{
	if(bInsertOrNot)
	{
		pStream->FillString  ("REPLACE INTO fabao SET SerialNum=") << pFabao->n64Serial;
		pStream->FillString(",");
	}
	else
		pStream->FillString  ("UPDATE fabao SET ") ;
	InitFabaoSpecUpdate(pStream,pCon,(tagFabaoSpec *)pFabao);
}

VOID CLoongDB::InitFabaoSpecUpdate(Beton::MyStream *pStream,Beton::Connection* pCon,const tagFabaoSpec * pFabao)
{
	pStream->FillString  (" NativeIntelligence=") << pFabao->n16NativeIntelligence;
	pStream->FillString  (", Stage=") << pFabao->n16Stage;
	pStream->FillString  (", PracticeValue=") << pFabao->nPracticeValue;
	DWORD dwStoredExpOpen = (pFabao->bStoredExpOpen == TRUE)?1:0;
	pStream->FillString  (", StoredExpOpen=") << dwStoredExpOpen;
	pStream->FillString  (", CurStoredExp=") << pFabao->nCurStoredExp;
	pStream->FillString  (", AttTypes='") ;
	pStream->FillBlob((LPVOID)pFabao->eAttType, sizeof(pFabao->eAttType),pCon);
	pStream->FillString  ("', AttGrowings='") ;
	pStream->FillBlob((LPVOID)pFabao->nAttGrowing, sizeof(pFabao->nAttGrowing),pCon);
	pStream->FillString  ("', WuxingType=") << pFabao->eWuxing ;

	pStream->FillString  (", WuxingHoles='") ;
	pStream->FillBlob((LPVOID)pFabao->eWuxingHole, sizeof(pFabao->eWuxingHole),pCon);
	pStream->FillString  ("', WuxingBeadIDs='") ;
	pStream->FillBlob((LPVOID)pFabao->dwWuxingBeadID, sizeof(pFabao->dwWuxingBeadID),pCon);

	pStream->FillString  ("', SlotAttTypes='") ;
	pStream->FillBlob((LPVOID)pFabao->eSlotAttType, sizeof(pFabao->eSlotAttType),pCon);
	pStream->FillString  ("', SlotAttValue='") ;
	pStream->FillBlob((LPVOID)pFabao->nSlotAttValue, sizeof(pFabao->nSlotAttValue),pCon);

	//pStream->FillString  ("', WuxingMatchReward='") ;
	//pStream->FillBlob((LPVOID)pFabao->nWuxingMatchReward, sizeof(pFabao->nWuxingMatchReward),pCon);
	pStream->FillString  ("', UseLevel=") << pFabao->nUseLevel;
	pStream->FillString(", SlotNum=") << pFabao->nSlotNum;

	pStream->FillString  (", AttGrowings0='") ;
	pStream->FillBlob((LPVOID)pFabao->nAttGrowing0, sizeof(pFabao->nAttGrowing0),pCon);
	pStream->FillString  ("'") ;

	pStream->FillString  (", SlotLingzhuMatch=") << pFabao->bySlotLingzhuMatch;
	pStream->FillString  (", SlotMacthNdx='") ;
	pStream->FillBlob((LPVOID)pFabao->bySlotMacthNdx, sizeof(pFabao->bySlotMacthNdx),pCon);
	pStream->FillString  ("'") ;

	// 1.2.1 妖精心情
	pStream->FillString(", Mood=") << pFabao->nMood;
	pStream->FillString(", MoodTicks=") << pFabao->nMoodTicks;
	pStream->FillString(", MoodBuffID=") << pFabao->dwMoodBuffID;

	// 1.2.2 妖精元素伤害以及抗性，特技等
	pStream->FillString(",EleInjuryType=") << pFabao->eEleInjury;
	pStream->FillString(",EleInjury=") << pFabao->nEleInjury;
	pStream->FillString(",ElePierce=") << pFabao->nElePierce;
	pStream->FillString(",EleInjPromoteTimes=") << pFabao->wEleInjPromoteTimes;
	pStream->FillString(",EleResPromoteTimes=") << pFabao->wEleResPromoteTimes;
	pStream->FillString(",EleResisFir=") << pFabao->nEleResistance[EER_Fire];
	pStream->FillString(",EleResisWater=") << pFabao->nEleResistance[EER_Water];
	pStream->FillString(",EleResisEarth=") << pFabao->nEleResistance[EER_Earth];
	pStream->FillString(",EleResisWind=") << pFabao->nEleResistance[EER_Wind];
	pStream->FillString(",EleResisBright=") << pFabao->nEleResistance[EER_Bright];
	pStream->FillString(",EleResisDark=") << pFabao->nEleResistance[EER_Dark];
	pStream->FillString(",DamageUpgradeTimes=") << pFabao->wDamageUpgradeTimes;
	pStream->FillString(",ResistanceUpgradeTimes=") << pFabao->wResistanceUpgradeTimes;
	pStream->FillString(",FairySkill1=") << pFabao->dwFairySkill[0];
	pStream->FillString(",FairySkill2=") << pFabao->dwFairySkill[1];
	pStream->FillString(",FairySkill3=") << pFabao->dwFairySkill[2];
	pStream->FillString(",FairySkill4=") << pFabao->dwFairySkill[3];
	pStream->FillString(",FairySkill5=") << pFabao->dwFairySkill[4];
	pStream->FillString(",UnsureSkill=") << pFabao->dwUnsureSkill;

	// 1.3.0 Jason 2010-12-6 妖精繁殖相关属性
	pStream->FillString(",FabaoStatus=") << pFabao->eFabaoStatus;
	pStream->FillString(",StatusChangeTime=") << pFabao->dwStatusChangeTime;
	pStream->FillString(",BirthValue=") << pFabao->nBirthValue;
	pStream->FillString(",BirthTimes=") << pFabao->nBirthTimes;
	pStream->FillString(",IsGivenBirthTimes=") << pFabao->byIsGivenBirthTimes;

	pStream->FillString  (", LevelBirth='") ;
	pStream->FillBlob((LPVOID)pFabao->byBirthLevel, sizeof(pFabao->byBirthLevel),pCon);
	pStream->FillString  ("'") ;
	pStream->FillString(",MaxIntelligence=") << pFabao->byMaxIntelligence;
}

// wcy 2012-03-31 初始化保存圣灵sql
VOID CLoongDB::InitHolyUpdate(BOOL bInsertOrNot,Beton::MyStream *pStream,Beton::Connection* pCon,const tagHolyMan * pHoly)
{
	if(bInsertOrNot)
	{
		pStream->FillString  ("REPLACE INTO holyman SET SerialNum=") << pHoly->n64Serial;
		pStream->FillString(",");
	}
	else
		pStream->FillString  ("UPDATE holyman SET ") ;
	InitHolySpecUpdate(pStream,pCon,(tagHolySpec *)pHoly);
}

// wcy 2012-04-18 初始化保存圣纹sql
VOID CLoongDB::InitHolyEquipUpdate(BOOL bInsertOrNot,Beton::MyStream *pStream,Beton::Connection* pCon,const tagHolyEquip * pHolyEquip)
{
	if(bInsertOrNot)
	{
		pStream->FillString  ("REPLACE INTO holyequip SET SerialNum=") << pHolyEquip->n64Serial;
		pStream->FillString(",");
	}
	else
		pStream->FillString  ("UPDATE holyequip SET ") ;
	InitHolyEquipSpecUpdate(pStream,pCon,(tagHolyEquipSpec *)pHolyEquip);
}

VOID CLoongDB::InitHolySpecUpdate(Beton::MyStream *pStream,Beton::Connection* pCon,const tagHolySpec * pHoly)
{
	pStream->FillString  (" DevourNum=") << pHoly->nDevourNum;
	pStream->FillString  (" ,EquipmentNumber=") << pHoly->n16EquipmentNumber;
	pStream->FillString  (" ,ToDayDevourNum=") << pHoly->nToDayEatNum;
	pStream->FillString  (" , EquipSerialIDs='") ;
	pStream->FillBlob((LPVOID)pHoly->n64EquipSerialIDs, sizeof(pHoly->n64EquipSerialIDs),pCon);
	pStream->FillString  ("',CoValue=") << pHoly->nCoValue;
	pStream->FillString  (" ,CostHoly=") << pHoly->nCostHoly;
	pStream->FillString  (" ,HolyDmg=") << pHoly->dwLevelUpAtt[ESAT_Demage];
	pStream->FillString  (" ,HolyDef=") << pHoly->dwLevelUpAtt[ESAT_HolyDef];
	pStream->FillString  (" ,Crit=") << pHoly->dwLevelUpAtt[ESAT_Crit];
	pStream->FillString  (" ,HolyCritRate=") << pHoly->dwLevelUpAtt[ESAT_CritRate];
	pStream->FillString  (" ,ExDamage=") << pHoly->dwLevelUpAtt[ESAT_ExDmg];
	pStream->FillString  (" ,AttackTec=") << pHoly->dwLevelUpAtt[ESAT_AttackTec];
	pStream->FillString  (" ,NeglectToughness=") << pHoly->dwLevelUpAtt[ESAT_NeglectToughness];
	pStream->FillString  (" ,HolyValue=") << pHoly->dwLevelUpAtt[ESAT_LingNeng];
	pStream->FillString  (" ,MaxDevourNum=") << pHoly->nMaxDevourNum;
}

VOID CLoongDB::InitHolyEquipSpecUpdate(Beton::MyStream *pStream,Beton::Connection* pCon,const tagHolyEquipSpec * pHolyEquip)
{
	pStream->FillString  (" CostHoly=") << pHolyEquip->nCostHoly;
	pStream->FillString  (" ,EnhanceCount=") << pHolyEquip->n16EnhanceCount;	
	pStream->FillString  (" ,HolyDmgChg=") << pHolyEquip->nHolyAttEffect[ESAT_Demage];
	pStream->FillString  (" ,HolyDefChg=") << pHolyEquip->nHolyAttEffect[ESAT_HolyDef];
	pStream->FillString  (" ,CritChg=") << pHolyEquip->nHolyAttEffect[ESAT_Crit];
	pStream->FillString  (" ,HolyCritRateChg=") << pHolyEquip->nHolyAttEffect[ESAT_CritRate];
	pStream->FillString  (" ,ExDamageChg=") << pHolyEquip->nHolyAttEffect[ESAT_ExDmg];
	pStream->FillString  (" ,AttackTecChg=") << pHolyEquip->nHolyAttEffect[ESAT_AttackTec];
	pStream->FillString  (" ,NeglectToughnessChg=") << pHolyEquip->nHolyAttEffect[ESAT_NeglectToughness];	
}


DWORD CLoongDB::SaveOneGodBidInfo(DWORD dwRoleID, INT64 n64Bidden, BYTE byBidType, DWORD dwBidID)
{
	if (byBidType >= EGBT_End || byBidType == EGBT_Start)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetReplace("god_bid");
	pStream->FillString("RoleID=")<<dwRoleID;
	pStream->FillString(",Bidden=")<<n64Bidden;
	pStream->FillString(",BidType=")<<byBidType;
	pStream->FillString(",BidID=")<<dwBidID;

	// 执行
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::UpdateFabaoGivenBirthTimes(INT64 n64FabaoID,INT nTimes)
{
	if (nTimes < 1)
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->FillString("UPDATE fabao SET IsGivenBirthTimes =1,BirthTimes=")<<nTimes;
	pStream->FillString(" WHERE SerialNum=")<<n64FabaoID;

	// 执行
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

DWORD CLoongDB::SaveEggInfo(DWORD dwRoleID, INT nCurLayer, INT16 n16ColourEggPos1, INT16 n16ColourEggPos2, tagEggInfo* pEggInfo)
{
	if (!P_VALID(pEggInfo))
		return FALSE;

	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;

	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetReplace("egg_broker");
	pStream->FillString("RoleID=")<<dwRoleID;
	pStream->FillString(",CurLayer=")<<nCurLayer;
	pStream->FillString(",ColourEggPos1=")<<n16ColourEggPos1;
	pStream->FillString(",ColourEggPos2=")<<n16ColourEggPos2;
	for (INT nCnt = 0; nCnt < MAX_EGG_LAYER; nCnt++)
	{
		char szItemID[100] = {0};
		char szBrokePos[100] = {0};
		sprintf(szItemID, ",ItemID%d=%d", nCnt+1, pEggInfo[nCnt].dwItemID);
		sprintf(szBrokePos, ",BrokePos%d=%d", nCnt+1, pEggInfo[nCnt].nBrokePos);
		pStream->FillString((const char*)szItemID);
		pStream->FillString((const char*)szBrokePos);
	}

	// 执行
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

BOOL CLoongDB::SaveWardrobeLayerInfo(tagWardrobeLayer2DB& WardrobeLayerInfo)
{
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	if(!P_VALID(pStream)) return FALSE;
	BOOL bRet = TRUE;

	pStream->Clear();
	pStream->SetReplace("wardrobe");
	pStream->FillString("RoleID=")<<WardrobeLayerInfo.dwRoleID;
	pStream->FillString(",LayerID=")<<WardrobeLayerInfo.dwLayerID;
	pStream->FillString(",Level=")<<WardrobeLayerInfo.byLevel;
	pStream->FillString(",State=")<<WardrobeLayerInfo.byState;
	pStream->FillString(",PlayActStartTime=")<<WardrobeLayerInfo.dwPlayActStartTime;
	pStream->FillString(",PlayActFriendID=")<<WardrobeLayerInfo.dwFriendID;

	// 执行
	if (!m_pDBLoong->Execute(pStream))
		bRet = FALSE;

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//-------------------------------------------------------------------------------------------------------
// 保存角色领域信息
BOOL CLoongDB::SaveArea(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagAreaSave), (&CLoongDB::FormatSaveArea), ppOutPointer);
}
//-------------------------------------------------------------------------------------------------------
// 保存角色领域信息
VOID CLoongDB::FormatSaveArea(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagAreaSave);

	pStream->SetReplace("area");
	pStream->FillString("AreaID=") << p->dwAreaID;
	pStream->FillString(",RoleID=") << dwRoleID;
	pStream->FillString(",Activation=") << p->bActivation;
	pStream->FillString(",CDTime=") << p->dwCD;
}
//-------------------------------------------------------------------------------------------------------
// 删除角色领域信息
BOOL CLoongDB::DelArea(DWORD dwRoleID, DWORD dwAreaID)
{
	return Update(dwRoleID, &dwAreaID, 1, sizeof(DWORD), (&CLoongDB::FormatDelArea));
}
//-------------------------------------------------------------------------------------------------------
// 删除角色领域信息
VOID CLoongDB::FormatDelArea(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->SetDelete("area");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(" and AreaID=") << (*(DWORD*)pData);
}
//-------------------------------------------------------------------------------------------------------
// 插入角色领域信息
BOOL CLoongDB::InsertArea(DWORD dwRoleID, tagAreaSave *pData)
{
	return Update(dwRoleID, pData, 1, sizeof(tagAreaSave), (&CLoongDB::FormatInsertArea));
}
//-------------------------------------------------------------------------------------------------------
// 插入角色领域信息
VOID CLoongDB::FormatInsertArea(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagAreaSave);

	pStream->SetInsert("area");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",AreaID=") << p->dwAreaID;
	pStream->FillString(",Activation=") << p->bActivation;
	pStream->FillString(",CDTime=") << p->dwCD;
}
//-------------------------------------------------------------------------------------------------------




BOOL CLoongDB::LogItemDelTime(Beton::MyStream* pStream, const char* szTableName, const INT64 n64SerialNum[], INT nNum)
{
	CHAR szTime[X_DATATIME_LEN+1] = {0};
	DwordTime2DataTime(szTime, sizeof(szTime), GetCurrentDWORDTime());

	BOOL bRet = TRUE;
	for (int i=0;i<nNum;++i)
	{
		pStream->Clear();

		pStream->SetUpdate(szTableName);
		pStream->FillString("CreateTime='").FillString(szTime).FillString("'");
		pStream->SetWhere();
		pStream->FillString("SerialNum=") << n64SerialNum[i];

		bRet = bRet && m_pDBLoong->Execute(pStream);
	}

	return bRet;
}

//-------------------------------------------------------------------------------
BOOL CLoongDB::DelItemDel( const CHAR* pszTime )
{
	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	// 删除操作
	pStream->SetDelete("item_del");
	pStream->SetWhere();
	pStream->FillString("del_time < '").FillString(pszTime).FillString("'");

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}


BOOL CLoongDB::DelEquipDel( const CHAR* pszTime )
{
	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	// 删除操作
	pStream->SetDelete("equip_del");
	pStream->SetWhere();
	pStream->FillString("deltime < '").FillString(pszTime).FillString("'");

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}

//---------------------------------------------------------
BOOL CLoongDB::UpdateDelEquipTime(INT64 n64Serial, CHAR *pszTime)
{
	if(!P_VALID(pszTime))
		return FALSE;

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 更新操作
	pStream->SetUpdate("equip");
	pStream->FillString("deltime='").FillString(pszTime);
	pStream->FillString("'");

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//---------------------------------------------------------
BOOL CLoongDB::UpdateDelItemTime(INT64 n64Serial, CHAR *pszTime)
{
	if(!P_VALID(pszTime))
		return FALSE;

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	// 更新操作
	pStream->SetUpdate("item");
	pStream->FillString("del_time='").FillString(pszTime);
	pStream->FillString("'");

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//---------------------------------------------------------
BOOL CLoongDB::UpdateDelEquipTime(DWORD dwRoleID, DWORD dwTypeID, CHAR *pszTime)
{
	if(!P_VALID(pszTime))
		return FALSE;

	INT64 n64Serial = GetDelEquip64Serial(dwRoleID, dwTypeID);
	if (n64Serial == 0)
	{
		return FALSE;
	}

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();

	// 更新操作
	pStream->SetUpdate("equip");
	pStream->FillString("deltime='").FillString(pszTime);
	pStream->FillString("'");

	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//---------------------------------------------------------
BOOL CLoongDB::UpdateDelItemTime(DWORD dwRoleID, DWORD dwTypeID, CHAR *pszTime)
{
	if(!P_VALID(pszTime))
		return FALSE;

	// 获取流和连接
	Beton::MyStream* pStream = m_pDBLoong->GetStream();
	ASSERT(P_VALID(pStream));

	pStream->Clear();


	// 更新操作
	pStream->SetUpdate("item");
	pStream->FillString("del_time='").FillString(pszTime);
	pStream->FillString("'");

	pStream->SetWhere();
	pStream->FillString("TypeID=") << dwTypeID;
	pStream->FillString(" and OwnerID=") << dwRoleID;
	pStream->SetLimit(1);

	// 执行
	BOOL bRet = m_pDBLoong->Execute(pStream);

	// 释放流
	m_pDBLoong->ReturnStream(pStream);

	return bRet;
}
//---------------------------------------------------------
BOOL CLoongDB::SaveBuyMallItemCount(DWORD dwRoleID, LPVOID pData, INT32 nNum, OUT LPVOID *ppOutPointer)
{
	return Update(dwRoleID, pData, nNum, sizeof(tagBuyMallItemCountSaveData), (&CLoongDB::FormatSaveBuyMallItemCount), ppOutPointer);
}

// 保存角色领域信息
VOID CLoongDB::FormatSaveBuyMallItemCount(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	MTRANS_POINTER(p, pData, tagBuyMallItemCountSaveData);

	pStream->SetReplace("role_buy_mall_item_count");
	pStream->FillString("RoleID=") << dwRoleID;
	pStream->FillString(",MallItemID=") << p->dwTypeID;
	pStream->FillString(",Num=") << p->nNum;
}

BOOL CLoongDB::SaveAccountSpec(DWORD dwAccountID, EAccountSpec eSpec)
{
	return Update(dwAccountID, &eSpec, 1, 0, (&CLoongDB::FormatSaveAccountSpec));
}

VOID CLoongDB::FormatSaveAccountSpec(Beton::MyStream* pStream, DWORD dwAccountID, LPVOID pData)
{
	pStream->Clear();
	pStream->SetUpdate("account_common");
	pStream->FillString("EMacc=") << INT(*(EAccountSpec*)pData);
	pStream->SetWhere();
	pStream->FillString("accountid=") << dwAccountID;
}

//插入领取过Key码物品的角色id
BOOL CLoongDB::InsertUseKeyCodeRoleID(DWORD dwRoleID)
{
	return Update(dwRoleID, NULL, 1, 0, (&CLoongDB::FormatInsertUseKeyCodeRoleID));
}
//插入领取过Key码物品的角色id
VOID CLoongDB::FormatInsertUseKeyCodeRoleID(Beton::MyStream *pStream, DWORD dwRoleID, LPVOID pData)
{
	pStream->Clear();
	pStream->SetInsert("UseKeyCode_roleID");
	pStream->FillString("RoleID=") << dwRoleID;
}