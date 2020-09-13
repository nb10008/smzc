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
#include "family_sprite.h"
#include "../ServerDefine/msg_family_sprite.h"
#include "db_session.h"
#include "Role.h"
//---------------------------------------------------------------------------------------------------
FamilySprite::FamilySprite(DWORD dwFimilyID)
{	
	m_dwFimilyID = dwFimilyID;
	m_FamilySpriteData.un2Level = 1;
	m_FamilySpriteData.dwFamilyID = dwFimilyID;
	m_UpdateTraining = FALSE;
	ActivationFamilySpriteAbility();
}

//---------------------------------------------------------------------------------------------------
FamilySprite::~FamilySprite()
{
	Clear();
}

//----------------------------------------------------------------------------------------------------
VOID FamilySprite::SetName(const TCHAR *ptszName)
{
	if(!P_VALID(ptszName))
		return;

	_tcscpy_s(m_FamilySpriteData.tszName,X_SHORT_NAME, ptszName);
}

//----------------------------------------------------------------------------------------------------
//清除所有的修炼内容
VOID FamilySprite::ClearFamilySpriteTrain()
{
	FamilySpriteTrainMap::TMapIterator it = m_mapFamilySpriteTrain.Begin();
	tagFamilySpriteRequirementSave* pFamilySpriteRequirementSave = NULL;

	while( m_mapFamilySpriteTrain.PeekNext(it, pFamilySpriteRequirementSave) )
	{
		SAFE_DEL(pFamilySpriteRequirementSave);
	}
	m_mapFamilySpriteTrain.Clear();
}
//----------------------------------------------------------------------------------------------------
//清除所有的修炼内容到DB
VOID FamilySprite::ClearFamilySpriteTrain2DB()
{
	tagNDBC_DeleteALLFamilySpriteRequirement send;

	g_dbSession.Send(&send,send.dwSize);
}
//----------------------------------------------------------------------------------------------------
VOID FamilySprite::Clear()
{
	RoleSpriteMap::TMapIterator it = m_mapRoleSprite.Begin();
	tagRoleSpriteData* pRoleSpriteData = NULL;

	while( m_mapRoleSprite.PeekNext(it, pRoleSpriteData) )
	{
		SAFE_DEL(pRoleSpriteData);
	}
	m_mapRoleSprite.Clear();

	ClearFamilySpriteTrain();
}
//----------------------------------------------------------------------------------------------------
//是否存在修炼内容
BOOL FamilySprite::IsExistFamilySpriteTrain( DWORD dwID )
{
	return m_mapFamilySpriteTrain.IsExist(dwID);
}

//----------------------------------------------------------------------------------------------------
//设置家族妖精数据
VOID FamilySprite::SetFamilySpriteData(tagFamilySpriteSave *pFamilySpriteSave)
{
	if(!P_VALID(pFamilySpriteSave))
		return;

	if(m_FamilySpriteData.dwFamilyID == pFamilySpriteSave->dwFamilyID)
		m_FamilySpriteData = *pFamilySpriteSave;	
}
//----------------------------------------------------------------------------------------------------
//设置家族妖精修炼内容数据
VOID FamilySprite::SetFamilySpriteRequirementData( tagFamilySpriteRequirementSave *pFamilySpriteRequirementSave, INT nNum )
{
	if(!P_VALID(pFamilySpriteRequirementSave))
		return;

	ClearFamilySpriteTrain();

	tagFamilySpriteRequirementSave *p = pFamilySpriteRequirementSave;
	for( int i = 0; i < nNum; ++i )
	{
		tagFamilySpriteRequirementSave* pFamilySpriteRequirementSave = new tagFamilySpriteRequirementSave();
		if( P_VALID(pFamilySpriteRequirementSave) )
		{
			pFamilySpriteRequirementSave->dwRequirementID = p->dwRequirementID;
			pFamilySpriteRequirementSave->nRet = p->nRet;
			pFamilySpriteRequirementSave->bComplete = p->bComplete;
			_tcscpy_s( pFamilySpriteRequirementSave->tszRoleName, X_SHORT_NAME, p->tszRoleName);
			m_mapFamilySpriteTrain.Add( p->dwRequirementID ,pFamilySpriteRequirementSave );
		}
		p++;
	}
}
//----------------------------------------------------------------------------------------------------
VOID FamilySprite::UpdateFamilySpriteData2DB()
{
	tagNDBC_UpdateFamilySprite send;
	send.fs.dwFamilyID = m_FamilySpriteData.dwFamilyID;
	send.fs.un2Level = m_FamilySpriteData.un2Level;
	send.fs.n8Exp = m_FamilySpriteData.n8Exp;
	
	for( int i = 0; i < EFSA_Num; ++i )
	{
		send.fs.nAbility[i] = m_FamilySpriteData.nAbility[i];
	}

	_stprintf( send.fs.tszName, m_FamilySpriteData.tszName );

	g_dbSession.Send(&send,send.dwSize);
}
//----------------------------------------------------------------------------------------------------
//更新家族妖精修炼数据
VOID FamilySprite::UpdateFamilySpriteRequirementData( DWORD dwTrainID, BOOL bComplete, INT nRet, DWORD dwRoleID )
{
	tagFamilySpriteRequirementSave* pFamilySpriteRequirementSave = m_mapFamilySpriteTrain.Peek(dwTrainID);
	if( P_VALID(pFamilySpriteRequirementSave) )
	{
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
		if (!P_VALID(pRoleInfo))
			return;

		_tcscpy_s( pFamilySpriteRequirementSave->tszRoleName, X_SHORT_NAME, pRoleInfo->szRoleName);
		pFamilySpriteRequirementSave->nRet = nRet;
		pFamilySpriteRequirementSave->bComplete = bComplete;
		UpdateFamilySpriteRequirementData2DB( dwTrainID, bComplete, nRet, pRoleInfo->szRoleName  );
	}
}
//----------------------------------------------------------------------------------------------------
VOID FamilySprite::UpdateFamilySpriteRequirementData2DB( DWORD dwTrainID, BOOL bComplete, INT nRet, TCHAR *ptszRoleName )
{
	tagNDBC_UpdateFamilySpriteRequirement send;
	send.dwFamilyID = m_FamilySpriteData.dwFamilyID;
	send.fsr.dwRequirementID = dwTrainID;
	send.fsr.bComplete = bComplete;
	_tcscpy_s( send.fsr.tszRoleName, X_SHORT_NAME, ptszRoleName );
	send.fsr.nRet = nRet;
	g_dbSession.Send(&send,send.dwSize);

}
//----------------------------------------------------------------------------------------------------
VOID FamilySprite::CreateFamilySpriteRequirementData( DWORD dwTrainID, BOOL bComplete )
{
	if( m_mapFamilySpriteTrain.IsExist(dwTrainID) )
		return;

	tagFamilySpriteRequirementSave* pRoleSpriteTrainDataNew = new tagFamilySpriteRequirementSave();
	if( P_VALID(pRoleSpriteTrainDataNew) )
	{
		pRoleSpriteTrainDataNew->dwRequirementID = dwTrainID;
		pRoleSpriteTrainDataNew->bComplete = bComplete;
		m_mapFamilySpriteTrain.Add( dwTrainID, pRoleSpriteTrainDataNew );
	}
}
//----------------------------------------------------------------------------------------------------
//将家族妖精修炼数据发送到DB
VOID FamilySprite::CreateFamilySpriteRequirementData2DB()
{
	INT nNum = m_mapFamilySpriteTrain.Size();
	INT nMsgSz = sizeof(tagNDBC_CreateFamilySpriteRequirement) + sizeof(tagFamilySpriteRequirementSave) * (nNum-1);
	MCREATE_MSG(pSend, nMsgSz, NDBC_CreateFamilySpriteRequirement);

	pSend->dwFamilyID = m_dwFimilyID;
	pSend->nNum = nNum;
	
	tagFamilySpriteRequirementSave *p = pSend->fsr;
	FamilySpriteTrainMap::TMapIterator it = m_mapFamilySpriteTrain.Begin();
	tagFamilySpriteRequirementSave* pRoleSpriteTrainData = NULL;

	while( m_mapFamilySpriteTrain.PeekNext(it, pRoleSpriteTrainData) )
	{
		if( P_VALID(pRoleSpriteTrainData) )
		{
			p->dwRequirementID = pRoleSpriteTrainData->dwRequirementID;
			p->bComplete = pRoleSpriteTrainData->bComplete;
			p->nRet = pRoleSpriteTrainData->nRet;
			_tcscpy_s( p->tszRoleName, X_SHORT_NAME, pRoleSpriteTrainData->tszRoleName);
			p++;
		}	
	}

	g_dbSession.Send( pSend, pSend->dwSize );
	
	MDEL_MSG(pSend);
}
//----------------------------------------------------------------------------------------------------
//获得家族妖精修炼内容
DWORD FamilySprite::GetFamilySpriteRequirement(tagFamilySpriteRequirementSave *pFamilySpriteRequirementSave, INT nNum)
{
	if( !P_VALID(pFamilySpriteRequirementSave) )
		return GT_INVALID;

	tagFamilySpriteRequirementSave *p = pFamilySpriteRequirementSave;
	FamilySpriteTrainMap::TMapIterator it = m_mapFamilySpriteTrain.Begin();
	tagFamilySpriteRequirementSave *pRoleSpriteTrainData = NULL;
	INT nSize = 0;

	while( m_mapFamilySpriteTrain.PeekNext(it, pRoleSpriteTrainData) && nSize < nNum )
	{
		if( !P_VALID(pRoleSpriteTrainData) )
			continue;

		_tcscpy_s( p->tszRoleName, X_SHORT_NAME, pRoleSpriteTrainData->tszRoleName);
		p->nRet = pRoleSpriteTrainData->nRet;
		p->dwRequirementID = pRoleSpriteTrainData->dwRequirementID;
		p->bComplete = pRoleSpriteTrainData->bComplete;

		p++;
		nSize++;
	}

	return E_Success;
}
//----------------------------------------------------------------------------------------------------
//获得修炼内容状态
BOOL FamilySprite::IsFamilySpriteTrainComplete( DWORD dwID )
{
	tagFamilySpriteRequirementSave *pRoleSpriteTrainData = m_mapFamilySpriteTrain.Peek(dwID);
	if(P_VALID(pRoleSpriteTrainData))
	{
		return pRoleSpriteTrainData->bComplete;
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------------
//获得当前能力值
INT	FamilySprite::GetAbility(EFamilySpriteAtt eType)
{
	if(eType < 0 || eType >= EFSA_Num)
		return 0;

	return m_FamilySpriteData.nAbility[eType];
}
//----------------------------------------------------------------------------------------------------
//设置当前能力值
VOID FamilySprite::SetAbility(EFamilySpriteAtt eType, INT nData)
{
	if(eType < 0 || eType >= EFSA_Num)
		return;

	m_FamilySpriteData.nAbility[eType] = nData;
}
//----------------------------------------------------------------------------------------------------
//注册玩家法宝
tagFabao *FamilySprite::RegisterRoleSprite( DWORD dwRoleID, tagFabao *pFabao )
{
	if(!P_VALID(pFabao))
		return NULL;

	tagFabao *pFabaoCur = NULL;
	tagRoleSpriteData *pRoleSpriteDataCur = m_mapRoleSprite.Peek(dwRoleID);
	if(!P_VALID(pRoleSpriteDataCur))
	{
		tagRoleSpriteData *pRoleSpriteData = new tagRoleSpriteData( dwRoleID );
		if( P_VALID(pRoleSpriteData) )
		{
			tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwRoleID);
			if (P_VALID(pRoleInfo))
				_tcscpy_s( pRoleSpriteData->RoleSpriteDataSave.tszRoleName, X_SHORT_NAME, pRoleInfo->szRoleName);

			pRoleSpriteData->pFabao = pFabao;
			pRoleSpriteData->RoleSpriteDataSave.un2Level = pFabao->n16Stage;
			pRoleSpriteData->RoleSpriteDataSave.dwSpriteType = pFabao->dwTypeID;
			pRoleSpriteData->RoleSpriteDataSave.bQuality = pFabao->equipSpec.byQuality;


			m_mapRoleSprite.Add(dwRoleID,pRoleSpriteData);

			UpdateRoleAtt( dwRoleID );

			tagNDBC_CreateRoleSpriteData send;
			send.dwFamilyID = m_dwFimilyID;
			send.rsd = pRoleSpriteData->RoleSpriteDataSave;
			g_dbSession.Send( &send, send.dwSize );
		}
	}
	else
	{
		pFabaoCur = pRoleSpriteDataCur->pFabao;
		pRoleSpriteDataCur->pFabao = pFabao;
		pRoleSpriteDataCur->RoleSpriteDataSave.un2Level = pFabao->n16Stage;
		pRoleSpriteDataCur->RoleSpriteDataSave.dwSpriteType = pFabao->dwTypeID;
		pRoleSpriteDataCur->RoleSpriteDataSave.bQuality = pFabao->equipSpec.byQuality;

		UpdateRoleAtt( dwRoleID );

		tagNDBC_UpdateRoleSpriteData send;
		send.dwFamilyID = m_dwFimilyID;
		send.rsd = pRoleSpriteDataCur->RoleSpriteDataSave;
		g_dbSession.Send( &send, send.dwSize );

		return pFabaoCur;
	}

	return NULL;
}
//----------------------------------------------------------------------------------------------------
//设置玩家法宝，仅设置法宝，如果没有也不新建
VOID FamilySprite::SetRoleSprite(DWORD dwRoleID, tagFabao *pFabao)
{
	if(!P_VALID(pFabao))
		return;

	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(P_VALID(pRoleSpriteData))
	{	
		pRoleSpriteData->pFabao = pFabao;
	}
}
//----------------------------------------------------------------------------------------------------
//计算共鸣值
FLOAT FamilySprite::CaculateGongming( DWORD dwRoleID )
{
//首先计算登记妖精等级与守护妖精等级的系数A
//系数A=登记妖精等级÷守护妖精当前等级，取值在0%-100%
//再计算登记妖精等级与玩家自身当前等级的系数B
//系数B=登记妖精等级÷玩家当前等级，取值在0%-100%
//再计算登记妖精实力值与策划设定的实力值上限的系数C
//系数C=登记妖精实力值/策划设定值，取值在0-110%
	if( !m_mapRoleSprite.IsExist(dwRoleID) )
		return 0;

	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(!P_VALID(pRoleSpriteData))
		return 0;

	tagFabao *pFabao = pRoleSpriteData->pFabao;
	if(!P_VALID(pFabao))
		return 0;

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))
		return 0;

	//计算登记妖精等级与守护妖精等级的系数
	FLOAT fA = (FLOAT)( (FLOAT)pFabao->n16Stage / (FLOAT)m_FamilySpriteData.un2Level );
	if( fA > 1.0f )
		fA = 1.0f;
	
	if( fA < 0.000001f )
		fA = 0.0f;

	//计算登记妖精等级与玩家自身当前等级的系数
	FLOAT fB = (FLOAT)( (FLOAT)pFabao->n16Stage / (FLOAT)pRole->GetLevel() );
	if( fB > 1.0f )
		fB = 1.0f;
	
	if( fB < 0.000001f )
		fB = 0.0f;

	//计算登记妖精实力值与策划设定的实力值上限的系数
//资质*品质*3/2 + 5个灵魂加和(单个灵魂等级^2 + 单个灵魂等级*5) + 妖精当前等级*3+所镶嵌的宝石加和（宝石等级*对应加权*4）

	//const tagFamilySpriteProto *pFamilySpriteProto = g_attRes.GetFamilySpriteProto(pFabao->n16Stage);
	//if(!P_VALID(pFamilySpriteProto))
	//	return 0;

	INT nWuxingData = 0;
	for( int i = 0; i < MaxWuxingSlotNum; ++i )
	{
		const tagItemProto *pItemProto = g_attRes.GetItemProto( pFabao->dwWuxingBeadID[i] );
		if(!P_VALID(pItemProto))
			continue;

		BYTE byLevel = pItemProto->byLevel / 10;
		nWuxingData += (byLevel * byLevel + byLevel * 5);
	}

	//硬编码，只能先这样，在排行榜已经存在的东西
	INT nWeightedlValue[] = {1,1,
		2,1,
		3,1, 
		4,2,
		5,3,
		6,3,
		7,4,
		8,6,
		9,8,
		10,10};

	INT nGemData = 0;
	for( int i = 0; i < MaxWuxingSlotNum; ++i )
	{
		const tagItemProto *pItemProto = g_attRes.GetItemProto( pFabao->equipSpec.dwHoleGemID[i] );
		if(!P_VALID(pItemProto))
			continue;

		nGemData += ( pItemProto->byLevel * nWeightedlValue[(pItemProto->byLevel-1)*2+1] * 4 );
	}

	FLOAT fShili = ( (FLOAT)pFabao->n16NativeIntelligence * pFabao->equipSpec.byQuality * 3 / 2 + nWuxingData + pFabao->n16Stage * 3 + nGemData );

	const tagFamilySpriteProto *pFamilySpriteProto = g_attRes.GetFamilySpriteProto(pFabao->n16Stage);
	if(!P_VALID(pFamilySpriteProto))
		return 0;
	FLOAT fC = fShili / pFamilySpriteProto->dwShedingzhi;

//最终的共鸣程度=系数A^2×系数B^1.5×系数C，单位为“%”，取值在0%-150%之间，取小数点后1位
	FLOAT fGongming = pow(fA,2.0f) * pow(fB,1.5f) * fC;

	if (fGongming >= 100)
      fGongming = 66.6;

	pRoleSpriteData->RoleSpriteDataSave.fGongming = fGongming;
	pRoleSpriteData->RoleSpriteDataSave.fShili = fShili;

	return fGongming;
}
//----------------------------------------------------------------------------------------------------
//计算玩家属性加成
VOID FamilySprite::CaculateRoleAtt( DWORD dwRoleID, Role *pRoleIn/*=NULL*/ )
{
	if( !m_mapRoleSprite.IsExist(dwRoleID) )
		return;

	Role* pRole = pRoleIn;
	if(!P_VALID(pRole))
	{
		pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
		if(!P_VALID(pRole))
			return;
	}

	tagRoleSpriteData *pRoleSpriteDataCur = m_mapRoleSprite.Peek(dwRoleID);
	if(P_VALID(pRoleSpriteDataCur))
	{
		pRole->SetRoleFamilySpriteAtt( pRoleSpriteDataCur->RoleSpriteDataSave.fGongming, m_FamilySpriteData.nAbility );
	}
}
//----------------------------------------------------------------------------------------------------
//更新玩家角色的属性
VOID FamilySprite::UpdateAllRoleAtt()
{
	RoleSpriteMap::TMapIterator iter = m_mapRoleSprite.Begin();
	while( iter != m_mapRoleSprite.End() )
	{
		Role* pRole = g_roleMgr.GetRolePtrByID(iter->first);
		if(!P_VALID(pRole))
		{
			iter++;
			continue;
		}

		CaculateGongming( iter->first );

		CaculateRoleAtt(iter->first);

		pRole->ReCalRoleAtt();

		iter++;
	}
}
//----------------------------------------------------------------------------------------------------
//更新玩家角色的属性
VOID FamilySprite::UpdateRoleAtt( DWORD dwRoleID )
{
	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(!P_VALID(pRoleSpriteData))
		return;

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))
		return;

	CaculateGongming( dwRoleID );

	CaculateRoleAtt(dwRoleID);

	pRole->ReCalRoleAtt();
}
//----------------------------------------------------------------------------------------------------
//移除玩家角色妖精
tagFabao* FamilySprite::RemoveRoleSprite(DWORD dwRoleID)
{
	if( !m_mapRoleSprite.IsExist(dwRoleID) )
		return NULL;

	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(!P_VALID(pRoleSpriteData))
		return NULL;

	tagFabao *pFabao = pRoleSpriteData->pFabao;
	SAFE_DELETE(pRoleSpriteData);
	m_mapRoleSprite.Erase(dwRoleID);

	ClearRoleAtt(dwRoleID);

	tagNDBC_DeleteRoleSpriteData send;
	send.dwRoleID = dwRoleID;
	g_dbSession.Send( &send, send.dwSize );

	return pFabao;
}
//----------------------------------------------------------------------------------------------------
//获取玩家角色妖精
tagFabao* FamilySprite::GetRoleSprite(DWORD dwRoleID)
{
	if( !m_mapRoleSprite.IsExist(dwRoleID) )
		return NULL;

	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(P_VALID(pRoleSpriteData))
		return pRoleSpriteData->pFabao;

	return NULL;
}
//----------------------------------------------------------------------------------------------------
//获取玩家登记的妖精
DWORD FamilySprite::GetFamilyRoleSprite( tagRoleSpriteDataSave *pRoleSpriteDataSave, INT nNum )
{
	if(!P_VALID(pRoleSpriteDataSave))
		return GT_INVALID;

	tagRoleSpriteDataSave *p = pRoleSpriteDataSave;
	RoleSpriteMap::TMapIterator it = m_mapRoleSprite.Begin();

	tagRoleSpriteData *pRoleSpriteData = NULL;
	INT nSize = 0;
	while( m_mapRoleSprite.PeekNext(it, pRoleSpriteData) && nSize < nNum )
	{
		if(!P_VALID(pRoleSpriteData->pFabao))
			continue;

		*p = pRoleSpriteData->RoleSpriteDataSave;

		p++;
		nSize++;
	}

	return E_Success;
}
//----------------------------------------------------------------------------------------------------
//设置玩家妖精信息
VOID FamilySprite::SetRoleSpriteInfo( tagRoleSpriteDataSave *pRoleSpriteDataSave, INT nNumSprite )
{
	if(!P_VALID(pRoleSpriteDataSave))
		return;

	tagRoleSpriteDataSave *p = pRoleSpriteDataSave;
	for( int i = 0; i < nNumSprite; ++i )
	{
		if(!P_VALID(p))
			return;

		tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(p->dwRoleID);
		if(P_VALID(pRoleSpriteData))
		{	
			pRoleSpriteData->RoleSpriteDataSave = *p;
			UpdateRoleAtt( p->dwRoleID );
		}
		else
		{
			tagRoleSpriteData *pRoleSpriteDataNew = new tagRoleSpriteData(p->dwRoleID);
			pRoleSpriteDataNew->RoleSpriteDataSave = *p;
			m_mapRoleSprite.Add(p->dwRoleID,pRoleSpriteDataNew);
			UpdateRoleAtt( p->dwRoleID );
		}
		p++;
	}
}
//----------------------------------------------------------------------------------------------------
//移除所有玩家角色妖精
VOID FamilySprite::RemoveAllRoleSprite( )
{
	RoleSpriteMap::TMapIterator it = m_mapRoleSprite.Begin();
	tagRoleSpriteData* pRoleSpriteData = NULL;

	while( m_mapRoleSprite.PeekNext(it, pRoleSpriteData) )
	{
		if(P_VALID(pRoleSpriteData))
		{
			Role* pRole = g_roleMgr.GetRolePtrByID(pRoleSpriteData->RoleSpriteDataSave.dwRoleID);
			if( P_VALID(pRole) )
			{
				tagNDBC_DeleteRoleSpriteData send;
				send.dwRoleID = pRoleSpriteData->RoleSpriteDataSave.dwRoleID;
				g_dbSession.Send( &send, send.dwSize );

				tagFabao *pFabao = pRoleSpriteData->pFabao;
				m_mapRoleSprite.Erase(pRoleSpriteData->RoleSpriteDataSave.dwRoleID);
				SAFE_DELETE(pRoleSpriteData);

				ClearRoleAtt(pRole->GetID());

				pRole->AddRoleSpriteFromFamily(pFabao);
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------
//激活属性
VOID FamilySprite::ActivationFamilySpriteAbility()
{
	const tagFamilySpriteProto *pFamilySpriteProto = g_attRes.GetFamilySpriteProto(m_FamilySpriteData.un2Level);
	if( !P_VALID(pFamilySpriteProto) ) 
		return;

	for( int i = 0; i < EFSA_Num; ++i )
	{
		if( pFamilySpriteProto->dwAbilityUpperLimit[i] != -1 )
		{
			if( -1 == m_FamilySpriteData.nAbility[i] )
			{
				m_FamilySpriteData.nAbility[i] = 0;
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------
VOID FamilySprite::AddExp( int nAddExp )
{
	const tagFamilySpriteProto *pFamilySpriteProtoCur = g_attRes.GetFamilySpriteProto(GetLevel());
	if( !P_VALID(pFamilySpriteProtoCur) ) 
		return;

	SetExp(GetExp() + nAddExp);
	INT nRet = GetExp() - pFamilySpriteProtoCur->dwExp;
	while( nRet >= 0 )
	{
		const tagFamilySpriteProto *pFamilySpriteProtoNext = g_attRes.GetFamilySpriteProto( GetLevel() + 1 );
		if( P_VALID(pFamilySpriteProtoNext) ) 
		{
			SetLevel( GetLevel() + 1 );
			SetExp(nRet);

			//升级的时候需要检查一下有没有被激活的属性
			ActivationFamilySpriteAbility();
		}
		else
		{
			SetExp(nRet);
			break;
		}

		const tagFamilySpriteProto *pFamilySpriteProtoTemp = g_attRes.GetFamilySpriteProto(GetLevel());
		if( !P_VALID(pFamilySpriteProtoTemp) ) 
			break;

		nRet = GetExp() - pFamilySpriteProtoTemp->dwExp;
	}

	//更新家族妖精数据到DB
	UpdateFamilySpriteData2DB();
}
//----------------------------------------------------------------------------------------------------
//清除玩家属性加成
VOID FamilySprite::ClearRoleAtt( DWORD dwRoleID )
{
	//if( !m_mapRoleSprite.IsExist(dwRoleID) )
	//	return;

	Role* pRole = g_roleMgr.GetRolePtrByID(dwRoleID);
	if(!P_VALID(pRole))
		return;

	pRole->ClearRoleFamilySpriteAtt();
	pRole->ReCalRoleAtt();
}
//----------------------------------------------------------------------------------------------------
//移除玩家角色妖精(角色离线状态)
VOID FamilySprite::RemoveRoleSpriteOffline(DWORD dwRoleID)
{
	if( !m_mapRoleSprite.IsExist(dwRoleID) )
		return;

	tagRoleSpriteData *pRoleSpriteData = m_mapRoleSprite.Peek(dwRoleID);
	if(!P_VALID(pRoleSpriteData))
		return;

	SAFE_DELETE(pRoleSpriteData);
	m_mapRoleSprite.Erase(dwRoleID);

	tagNDBC_DeleteRoleSpriteData send;
	send.dwRoleID = dwRoleID;
	g_dbSession.Send( &send, send.dwSize );
}
//----------------------------------------------------------------------------------------------------












