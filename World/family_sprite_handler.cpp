//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: family_sprite_handle.cpp
// author: 
// actor:
// data: 2011-01-12
// last:
// brief: 家族妖精
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_family.h"
#include "player_session.h"
#include "role.h"
#include "family_sprite.h"
#include "family_sprite_mgr.h"
//-----------------------------------------------------------------------------
//获取妖精修炼内容
DWORD PlayerSession::HandleGetFamilySpriteTrain(tagNetCmd* pCmd)
{
	tagNC_GetFamilySpriteTrain* pRecv = (tagNC_GetFamilySpriteTrain*)pCmd;

	if( !P_VALID(pRecv) ) 
		return GT_INVALID;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) 
		return GT_INVALID;

	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite( pRole->GetFamilyID() );
	if( !P_VALID(pFamilySprite) ) 
		return GT_INVALID;

	if( pFamilySprite->GetUpdateSpriteRequirementFlag() )
		return GT_INVALID;

	INT nNum = pFamilySprite->GetFamilySpriteRequirementNum();
	INT nMsgSz = sizeof(tagNS_GetFamilySpriteTrain) + sizeof(tagFamilySpriteRequirementSave) * (nNum-1);
	MCREATE_MSG(pSend, nMsgSz, NS_GetFamilySpriteTrain);

	pSend->dwFamilyID = pRole->GetFamilyID();
	pSend->dwErrorCode = pFamilySprite->GetFamilySpriteRequirement(pSend->ftsi,nNum);
	pSend->nNum = nNum;

	SendMessage(pSend,pSend->dwSize);
	
	MDEL_MSG(pSend);

	return E_Success;
}
//-----------------------------------------------------------------------------
//妖精修炼
DWORD PlayerSession::HandleFamilySpriteTrain(tagNetCmd* pCmd)
{
	tagNC_FamilySpriteTrain* pRecv = (tagNC_FamilySpriteTrain*)pCmd;

	if( !P_VALID(pRecv) ) 
		return GT_INVALID;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) 
		return GT_INVALID;

	INT nTrainRet = 0;
	tagNS_FamilySpriteTrain send;
	send.dwTrainID = pRecv->dwTrainID;
	send.dwErrorCode = pRole->TrainFamilySprite(pRecv->dwTrainID,nTrainRet,pRecv->n64ID,pRecv->bAgain);
	send.nTrainRet = nTrainRet;
	
	SendMessage(&send,send.dwSize);

	return E_Success;
}	
//-----------------------------------------------------------------------------
//妖精登记
DWORD PlayerSession::HandleFamilySpriteRegister(tagNetCmd* pCmd)
{
	tagNC_FamilySpriteRegister* pRecv = (tagNC_FamilySpriteRegister*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) 
		return GT_INVALID;

	tagNS_FamilySpriteRegister send;
	send.dwErrorCode = pRole->FamilySpriteRegister(pRecv->n64id);

	SendMessage(&send,send.dwSize);

	return E_Success;
}
//-----------------------------------------------------------------------------
//取回妖精
DWORD PlayerSession::HandleFamilySpriteGetBack(tagNetCmd* pCmd)
{
	tagNC_FamilySpriteGetBack* pRecv = (tagNC_FamilySpriteGetBack*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) 
		return GT_INVALID;

	tagNS_FamilySpriteGetBack send;
	send.dwErrorCode = pRole->FamilySpriteGetBack();

	SendMessage(&send,send.dwSize);

	return E_Success;
}
//-----------------------------------------------------------------------------
//获取家族内所有登记的妖精信息
DWORD PlayerSession::HandleGetFamilyRoleSprite(tagNetCmd* pCmd)
{
	tagNC_GetFamilyRoleSprite* pRecv = (tagNC_GetFamilyRoleSprite*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) 
		return GT_INVALID;

	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite( pRole->GetFamilyID() );
	if( !P_VALID(pFamilySprite) ) 
		return GT_INVALID;

	INT nNum = pFamilySprite->GetFamilyRoleSpriteNum();
	INT nMsgSz = sizeof(tagNS_GetFamilyRoleSprite) + sizeof(tagRoleSpriteDataSave) * (nNum-1);
	MCREATE_MSG(pSend, nMsgSz, NS_GetFamilyRoleSprite);

	pSend->dwFamilyID = pRole->GetFamilyID();
	pSend->nNum = nNum;
	pSend->dwErrorCode = pFamilySprite->GetFamilyRoleSprite(pSend->frsi,nNum);
	
	SendMessage(pSend,pSend->dwSize);
	
	MDEL_MSG(pSend);

	return E_Success;
}
//-----------------------------------------------------------------------------
//获取妖精登记信息
DWORD PlayerSession::HandleGetFamilySpriteAtt(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) 
		return GT_INVALID;

	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite( pRole->GetFamilyID() );
	if( !P_VALID(pFamilySprite) ) 
		return GT_INVALID;

	tagNS_GetFamilySpriteAtt send;
	for( int i = 0; i < EFSA_Num; ++i )
	{
		send.dwSpriteAtt[i] = pRole->m_nAbility[i];
	}

	SendMessage( &send, send.dwSize );
	
	return E_Success;
}
//-----------------------------------------------------------------------------
//获取家族妖精信息
DWORD PlayerSession::HandleGetFamilySpriteInfo(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) 
		return GT_INVALID;

	FamilySprite* pFamilySprite = g_FamilySpriteMgr.FindFamilySprite( pRole->GetFamilyID() );
	if( !P_VALID(pFamilySprite) ) 
		return GT_INVALID;

	tagNS_GetFamilySpriteInfo send;

	send.nLevel = pFamilySprite->GetLevel();
	send.dwExp = pFamilySprite->GetExp();
	for( int i = 0; i < EFSA_Num; ++i )
	{
		send.dwSpriteAtt[i] = pFamilySprite->GetAbility((EFamilySpriteAtt)i);
	}
	

	SendMessage( &send, send.dwSize );
	
	return E_Success;
}
//-----------------------------------------------------------------------------





