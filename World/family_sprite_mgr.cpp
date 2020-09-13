//-------------------------------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: domain_mgr.cpp
// author: liutingting
// actor:
// data: 2011-03-24
// last:
// brief: 家族法宝相关内容操作
//-------------------------------------------------------------------------------------------------------
#pragma once

#include "StdAfx.h"
#include "family_sprite_mgr.h"
#include "family_sprite.h"
#include "att_res.h"

FamilySpriteMgr g_FamilySpriteMgr;
//---------------------------------------------------------------------
FamilySpriteMgr::FamilySpriteMgr()
{
	m_dwRequirementTime = 0;
}

//---------------------------------------------------------------------
FamilySpriteMgr::~FamilySpriteMgr()
{
	Clear();
}

//---------------------------------------------------------------------
//生成一个家族妖精
FamilySprite* FamilySpriteMgr::CreateFamilySprite( DWORD dwFamilyID, const TCHAR *pszName )
{
	if( !P_VALID(pszName) )
		return NULL;

	FamilySprite* pFamilySprite = new FamilySprite(dwFamilyID);
	if( P_VALID(pFamilySprite) )
	{
		pFamilySprite->SetName(pszName);

		m_mapFamilySprite.Add( dwFamilyID, pFamilySprite );
	}

	tagNDBC_CreateFamilySprite send;
	send.fs = *(pFamilySprite->GetFamilySpriteData());
	g_dbSession.Send( &send, send.dwSize );

	CreateFamilySpriteRequirement(dwFamilyID);

	return pFamilySprite;
}

//---------------------------------------------------------------------
//清除
VOID FamilySpriteMgr::Clear()
{
	FamilySpriteMap::TMapIterator it = m_mapFamilySprite.Begin();
	FamilySprite* pFamilySprite = NULL;

	while( m_mapFamilySprite.PeekNext(it, pFamilySprite) )
	{
		SAFE_DEL(pFamilySprite);
	}
	m_mapFamilySprite.Clear();
}

//---------------------------------------------------------------------
//Update
VOID FamilySpriteMgr::Update()
{
	tagDWORDTime now = GetCurrentDWORDTime();
	if ( (now.hour == 0 && now.min == 0 ) && ( m_dwRequirementTime.day != now.day ))
	{
		//先清除原有修炼需求
		ClearAllFamilySpriteRequirement();

		//生成守护妖精的修炼需求
		CreateFamilySpriteRequirement();

		m_dwRequirementTime = now;
	}
}

//---------------------------------------------------------------------
//获得一个妖精，如果该家族没有，则创造一个
FamilySprite* FamilySpriteMgr::FindFamilySprite( DWORD dwFamilyID )
{
	return m_mapFamilySprite.Peek(dwFamilyID);
}

//---------------------------------------------------------------------
//找一个家族登记妖精
tagFabao* FamilySpriteMgr::FindRoleFamilySprite( DWORD dwFamilyID, DWORD dwRoleID )
{
	FamilySprite* pFamilySprite = m_mapFamilySprite.Peek(dwFamilyID);
	if( P_VALID(pFamilySprite) )
	{
		return pFamilySprite->GetRoleSprite(dwRoleID);
	}

	return NULL;
}
//---------------------------------------------------------------------
//生成守护妖精的修炼需求
VOID FamilySpriteMgr::CreateFamilySpriteRequirement()
{
	FamilySpriteMap::TMapIterator it = m_mapFamilySprite.Begin();
	FamilySprite* pFamilySprite = NULL;

	while( m_mapFamilySprite.PeekNext(it, pFamilySprite) )
	{
		if( P_VALID(pFamilySprite) )
		{
			DWORD dwPickTypeNum[2] = {0};
			const tagFamilySpriteProto *pFamilySpriteProto = g_attRes.GetFamilySpriteProto(pFamilySprite->GetLevel());
			if(P_VALID(pFamilySpriteProto))
			{
				dwPickTypeNum[0] = pFamilySpriteProto->dwPickTypeNum[0];
				dwPickTypeNum[1] = pFamilySpriteProto->dwPickTypeNum[1];

				//随机获得修炼需求
				RandGetSpriteTrain(pFamilySprite,dwPickTypeNum[0],dwPickTypeNum[1]);

				pFamilySprite->CreateFamilySpriteRequirementData2DB();

			}

			pFamilySprite->SetUpdateSpriteRequirementFlag(FALSE);
		}
	}
}

//---------------------------------------------------------------------
//全部存库,存储家族妖精基本数据，修炼数据实时存储
VOID FamilySpriteMgr::SaveAll2DB()
{
	FamilySpriteMap::TMapIterator it = m_mapFamilySprite.Begin();
	FamilySprite* pFamilySprite = NULL;

	INT nNum = m_mapFamilySprite.Size();
	INT nMsgSz = sizeof(tagNDBC_SaveAllFamilySprite) + (nNum-1) * sizeof(tagFamilySpriteSave);
	MCREATE_MSG(pSend, nMsgSz, NDBC_SaveAllFamilySprite);

	if( !P_VALID(pSend) )
		return;

	INT nIndex = 0;
	while( m_mapFamilySprite.PeekNext(it, pFamilySprite) || nIndex < nNum)
	{
		if( P_VALID(pFamilySprite) )
		{
			//家族妖精基本数据
			tagFamilySpriteSave *pFamilySpriteSave = pFamilySprite->GetFamilySpriteData();
			if( P_VALID(pFamilySpriteSave) )
			{
				pSend->fs[nIndex] = *pFamilySpriteSave;
			}
			nIndex++;
		}
	}

	g_dbSession.Send(pSend,pSend->dwSize);

	MDEL_MSG(pSend);
}
//---------------------------------------------------------------------
//从库初始化
VOID FamilySpriteMgr::InitFromDB(tagFamilySpriteSave *pFamilySpriteSave,tagFamilySpriteRequirementSave *pFamilySpriteRequirementSave, INT nNumRequirement, tagRoleSpriteDataSave *pRoleSpriteDataSave, INT nNumSprite)
{
	if(!P_VALID(pFamilySpriteSave))
		return;

	FamilySprite* pFamilySprite = NULL;
	pFamilySprite = m_mapFamilySprite.Peek(pFamilySpriteSave->dwFamilyID);
	if( P_VALID(pFamilySprite) )
	{
		pFamilySprite->SetFamilySpriteData(pFamilySpriteSave);

		pFamilySprite->SetFamilySpriteRequirementData(pFamilySpriteRequirementSave,nNumRequirement);

		pFamilySprite->SetRoleSpriteInfo(pRoleSpriteDataSave,nNumSprite);
	}
	else
	{
		pFamilySprite = new FamilySprite(pFamilySpriteSave->dwFamilyID);
		if( !P_VALID(pFamilySprite) )
			return;

		pFamilySprite->SetFamilySpriteData(pFamilySpriteSave);

		pFamilySprite->SetFamilySpriteRequirementData(pFamilySpriteRequirementSave,nNumRequirement);

		pFamilySprite->SetRoleSpriteInfo(pRoleSpriteDataSave,nNumSprite);

		m_mapFamilySprite.Add(pFamilySpriteSave->dwFamilyID,pFamilySprite);
	}
}
//---------------------------------------------------------------------
//随机获得家族妖精修炼内容
VOID FamilySpriteMgr::RandGetSpriteTrain( FamilySprite *pFamilySprite, DWORD dwPickTypeNum1, DWORD dwPickTypeNum2 )
{
	if( !P_VALID(pFamilySprite) )
		return;

	DWORD dwNum1 = dwPickTypeNum1; 
	DWORD dwNum2 = dwPickTypeNum2;
	INT	nCount = 0;

	while(dwNum1 > 0)
	{
		if(nCount > 1000)//暂时定义的1000次退出 TODO liu
			break;

		const tagSpriteTrainProto *pSpriteTrainProto = g_attRes.RandGetSpriteTrainProto(pFamilySprite->GetLevel(),EFSPT_1);
		if(P_VALID(pSpriteTrainProto))
		{
			if( pFamilySprite->IsExistFamilySpriteTrain(pSpriteTrainProto->nID) )
			{
				nCount++;
				continue;
			}

			if( dwNum1 <= 0 )
			{
				nCount++;
				continue;
			}

			pFamilySprite->CreateFamilySpriteRequirementData(pSpriteTrainProto->nID,FALSE);
			dwNum1--;
		}
		nCount++;
	}

	nCount = 0;
	while(dwNum2 > 0)
	{
		if(nCount > 1000)//暂时定义的1000次退出 TODO liu
			break;

		const tagSpriteTrainProto *pSpriteTrainProto = g_attRes.RandGetSpriteTrainProto(pFamilySprite->GetLevel(),EFSPT_2);
		if(P_VALID(pSpriteTrainProto))
		{
			if( pFamilySprite->IsExistFamilySpriteTrain(pSpriteTrainProto->nID) )
			{
				nCount++;
				continue;
			}

			if( dwNum2 <= 0 )
			{
				nCount++;
				continue;
			}

			pFamilySprite->CreateFamilySpriteRequirementData(pSpriteTrainProto->nID,FALSE);
			dwNum2--;
		}
		nCount++;
	}
}
//---------------------------------------------------------------------
//清除家族妖精修炼需求
VOID FamilySpriteMgr::ClearAllFamilySpriteRequirement()
{
	FamilySpriteMap::TMapIterator it = m_mapFamilySprite.Begin();
	FamilySprite* pFamilySprite = NULL;

	while( m_mapFamilySprite.PeekNext(it, pFamilySprite) )
	{
		if( P_VALID(pFamilySprite) )
		{
			pFamilySprite->SetUpdateSpriteRequirementFlag(TRUE);
			pFamilySprite->ClearFamilySpriteTrain();
		}
	}
	pFamilySprite->ClearFamilySpriteTrain2DB();
}
//---------------------------------------------------------------------
//去掉一个家族妖精
VOID FamilySpriteMgr::DelFamilySprite(DWORD dwFamilyID)
{
	FamilySprite* pFamilySprite = m_mapFamilySprite.Peek(dwFamilyID);
	if( P_VALID(pFamilySprite) )
	{
		m_mapFamilySprite.Erase(dwFamilyID);
		SAFE_DEL(pFamilySprite);
	}

	tagNDBC_DeleteFamilySprite send;
	send.dwFamilyID = dwFamilyID;
	g_dbSession.Send(&send,send.dwSize);
}
//---------------------------------------------------------------------
//生成某一个家族守护妖精的修炼需求
VOID FamilySpriteMgr::CreateFamilySpriteRequirement(DWORD dwFamilyID)
{
	FamilySprite* pFamilySprite = m_mapFamilySprite.Peek(dwFamilyID);
	if( P_VALID(pFamilySprite) )
	{
		DWORD dwPickTypeNum[2] = {0};
		const tagFamilySpriteProto *pFamilySpriteProto = g_attRes.GetFamilySpriteProto(pFamilySprite->GetLevel());
		if(P_VALID(pFamilySpriteProto))
		{
			dwPickTypeNum[0] = pFamilySpriteProto->dwPickTypeNum[0];
			dwPickTypeNum[1] = pFamilySpriteProto->dwPickTypeNum[1];

			//随机获得修炼需求
			RandGetSpriteTrain(pFamilySprite,dwPickTypeNum[0],dwPickTypeNum[1]);

			pFamilySprite->CreateFamilySpriteRequirementData2DB();

		}

		pFamilySprite->SetUpdateSpriteRequirementFlag(FALSE);
	}
}
//---------------------------------------------------------------------



