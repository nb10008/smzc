//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: combat_handler.cpp
// author: Aslan
// actor:
// data: 2008-09-16
// last:
// brief: 玩家间交易消息处理
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_combat.h"
#include "../WorldDefine/msg_map.h"

#include "role_mgr.h"
#include "player_session.h"
#include "map.h"
#include "unit.h"
#include "role.h"
#include "city_struggle.h"
#include "city_mgr.h"
#include "city.h"
#include "GuildBattle.h"
#include "../ServerDefine/mini_tool.h"
#include "fairy_contract.h"
#include "const_define.h"

//--------------------------------------------------------------------------------
// 进入战斗状态
//--------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleEnterCombat(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	// 设置人物现在处于战斗状态
	pRole->SetRoleState(ERS_Combat);
	return 0;
}

//--------------------------------------------------------------------------------
// 离开战斗状态
//--------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleLeaveCombat(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	// 设置人物现在处于战斗状态
	pRole->UnsetRoleState(ERS_Combat);
	// Jason 2010-7-17 v1.0.0 pk预警信息
	pRole->ClearAttackDefenseRelationship();
	return 0;
}


//--------------------------------------------------------------------------------
// 使用技能
//--------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSkill(tagNetCmd* pCmd)
{
	tagNC_Skill* pRecv = (tagNC_Skill*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) ) return GT_INVALID;

	// 看能否同步到发招地点
	Vector3 oldPos = pRole->GetMoveData().m_vPos;
	if (pRole->GetMoveData().IsInValidDistance(pRecv->vSrcPos))
	{
		pRole->GetMoveData().ForceTeleport(pRecv->vSrcPos,FALSE);	// not sync pos to client
	}
	else
	{
		tagNS_Skill send;
		send.dwSrcRoleID = pRole->GetID();
		send.dwTarRoleID = pRecv->dwTargetRoleID;
		send.dwSkillID = pRecv->dwSkillID;
		send.dwSerial = pRecv->dwSerial;
		send.dwErrorCode = E_UseSkill_DistLimit;

		SendMessage(&send, send.dwSize);		
		return GT_INVALID;
	}

	// 召唤圣灵状态下不能使用骑乘技能
	if ( Skill::GetIDFromTypeID(pRecv->dwSkillID) == MOUNT_SKILL && pRole->IsInRoleState(ERS_CALLHOLY) )
	{
		// 取消召唤圣灵
		pRole->UnsetRoleState(ERS_CALLHOLY, FALSE);
		// 通知客户端 召唤圣灵消失
		tagNS_CallShengLingDisappear msg;
		msg.dwPlayerID = pRole->GetID();
		Map* pMap = pRole->GetMap();
		if (P_VALID(pMap))
		{
			pMap->SendBigVisTileMsg(pRole, &msg, msg.dwSize);
		}	
	}
		
	// 使用技能
	const tagSkillProto* pSkillProto = g_attRes.GetSkillProto(pRecv->dwSkillID);
	if (!P_VALID(pSkillProto))
	{
		return GT_INVALID;
	}

	INT nRet = 0;
	if((ESkillTypeEx)pSkillProto->nType2 == ESSTE_SoulSkill)
	{
		DWORD dwSkillID = pRole->GetSkillIDFromSoulCrystal();
		if(P_VALID(dwSkillID))
		{
			Skill* pSkill = new Skill(Skill::GetIDFromTypeID(dwSkillID), 1, 0, 0, 0);
			if(!P_VALID(pSkill)) return GT_INVALID;
			pRole->AddSkill(pSkill, FALSE);
			nRet = pRole->GetCombatHandler().UseSkill(Skill::GetIDFromTypeID(dwSkillID), pRecv->dwTargetRoleID, pRecv->dwSerial);
			
			if(E_Success != nRet)
				pRole->RemoveSkill(Skill::GetIDFromTypeID(dwSkillID));

			//魂晶技能冷却
			Skill* pSkill1 = pRole->GetSkill(pSkillProto->dwID/100);
			if(P_VALID(pSkill))
				pSkill1->StartCoolDown();
		}
		else
			return GT_INVALID;
	}
	else
	{
		nRet = pRole->GetCombatHandler().UseSkill(Skill::GetIDFromTypeID(pRecv->dwSkillID), pRecv->dwTargetRoleID, pRecv->dwSerial);
	}

	if ( pRole->GetCombatHandler().GetNeedPKLogOn() )
	{
		ILOG->Write(_T("Role useskill.role id:%d,skill id:%d\r\n"),pRole->GetID(),pRecv->dwSkillID);
	}

	if( E_Success != nRet )
	{
		pRole->GetMoveData().ForceTeleport(oldPos,TRUE);	// force to old pos

		tagNS_Skill send;
		send.dwSrcRoleID = pRole->GetID();
		send.dwTarRoleID = pRecv->dwTargetRoleID;
		send.dwSkillID = pRecv->dwSkillID;
		send.dwSerial = pRecv->dwSerial;
		send.dwErrorCode = DWORD(nRet);

		SendMessage(&send, send.dwSize);
	}

	else
	{
		if (pRole->IsInRoleState(ERS_HoldChest) && Skill::GetIDFromTypeID(pRecv->dwSkillID) == MOUNT_SKILL)
		{
			const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
			if (P_VALID(pRoleScript))
				pRoleScript->ForceLootChase(pRole);
		}
		pRole->GetMap()->SyncMovementToBigVisTile(pRole);	// sync pos change to client on success
		pRole->StopMount();
		tagNS_Skill send;
		send.dwSrcRoleID = pRole->GetID();
		send.dwTarRoleID = pRecv->dwTargetRoleID;
		send.dwSkillID = pRecv->dwSkillID;
		send.dwSerial = pRecv->dwSerial;
		send.nSpellTime = pRole->GetCombatHandler().GetSkillPrepareCountDown();
		send.dwErrorCode = E_Success;

		if( send.nSpellTime > TICK_TIME )
		{
			send.nSpellTime -= TICK_TIME;
		}

		if( P_VALID(pRole->GetMap()) )
		{
			const tagSkillProto* pSkillProto = g_attRes.GetSkillProto(send.dwSkillID);
			if( P_VALID(pSkillProto) && ( pSkillProto->nType3 == ESSTE2_GodCondense || ESSF_FreeOpenFace == pSkillProto->eSpecFunc )) // 凝聚
				SendMessage(&send, send.dwSize);
			else
				pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------
// 打断技能
//--------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleInterruptSkill(tagNetCmd* pCmd)
{
	tagNC_SkillInterrupt* pRecv = (tagNC_SkillInterrupt*)pCmd;

	Role *pRole = GetRole();
	if( !P_VALID(pRole)) return GT_INVALID;

	pRole->GetCombatHandler().CancelSkillUse(pRecv->dwSkillID, pRecv->dwSerial);

	return 0;	
}

//--------------------------------------------------------------------------------
// 使用物品
//--------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleUseItem(tagNetCmd *pCmd)
{
	tagNC_UseItem* pRecv = (tagNC_UseItem*)pCmd;

	Role *pRole = GetRole();
	if( !P_VALID(pRole))
		return GT_INVALID;

	Map* pMap = pRole->GetMap();
	if( !P_VALID(pMap) ) return GT_INVALID;

	DWORD dwTypeID = GT_INVALID;
	bool bImmediate = false;

	// 使用物品
	INT nRet = pRole->GetCombatHandler().UseItem(pRecv->n64ItemID, pRecv->dwTargetRoleID, pRecv->dwSerial, dwTypeID, bImmediate);

	if( E_Success != nRet)
	{
		tagNS_UseItem send;
		send.dwSrcRoleID = pRole->GetID();
		send.dwTarRoleID = pRecv->dwTargetRoleID;
		send.n64ItemID = pRecv->n64ItemID;
		send.dwTypeID = dwTypeID;
		send.dwSerial = pRecv->dwSerial;
		send.dwErrorCode = DWORD(nRet);
		send.bInmmediate = bImmediate;

		SendMessage(&send, send.dwSize);
	}
	else
	{
		if( !bImmediate )
		{
			// 如果不是立即可以使用的物品，则需要停下来
			if (pRole->GetMoveData().IsInValidDistance(pRecv->vSrcPos))
			{
				pRole->GetMoveData().ForceTeleport(pRecv->vSrcPos, TRUE);
			}
		}

		tagNS_UseItem send;
		send.dwSrcRoleID = pRole->GetID();
		send.dwTarRoleID = pRecv->dwTargetRoleID;
		send.n64ItemID = pRecv->n64ItemID;
		send.dwTypeID = dwTypeID;
		send.dwSerial = pRecv->dwSerial;
		send.nSpellTime = pRole->GetCombatHandler().GetItemPrepareCountDown();
		send.dwErrorCode = E_Success;
		send.bInmmediate = bImmediate;

		if( send.nSpellTime > TICK_TIME )
		{
			send.nSpellTime -= TICK_TIME;
		}

		if( P_VALID(pRole->GetMap()) )
		{
			pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------
// 打断使用物品
//--------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleInterruptUseItem(tagNetCmd* pCmd)
{
	tagNC_UseItemInterrupt* pRecv = (tagNC_UseItemInterrupt*)pCmd;

	Role *pRole = GetRole();
	if( !P_VALID(pRole)) return GT_INVALID;

	pRole->GetCombatHandler().CancelItemUse(pRecv->n64ItemID);

	return 0;	
}

//------------------------------------------------------------------------
// 绑定复活地图
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleBindRebornMap(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_BindRebornMap);
	
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	
	tagNS_BindRebornMap send;
	send.dwErrorCode = pRole->SetRebornMap(p->dwNPCID, send.dwBornMapID, send.vBornPos);

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}

//------------------------------------------------------------------------
// 复活
//------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleRevive(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_RoleRevive);

	// 检查消息合法性
	if(p->eType < ERRT_Start || p->eType > ERRT_End)
	{
		return GT_INVALID;
	}
	
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 城战复活  加入待复活队列，复活倒计时结束时才复活
	if (p->eType == ERRT_CityStruggle)
	{
		DWORD dwMapID = pRole->GetMapID();
		City* pCity = g_cityMgr.GetCity(pRole);
		if (P_VALID(pCity))
		{
			pCity->AddToReviveMap(pRole);
			return E_Success;
		}
	}

	// 帮战复活
	if ( p->eType == ERRT_GuildBattle)
	{
		CGodMiracleMgr::GetInst()->AddRevieRole(pRole->GetID());
		return E_Success;
	}

	tagNS_RoleRevive send;
	send.dwRoleID		= pRole->GetID();
	if ( p->eType == ERRT_Locus || p->eType == ERRT_Perfect)
	{
		if ( CGodMiracleMgr::GetInst()->IsInCaptureGodMiracleGuild(pRole) == TRUE)
		{
			send.dwErrorCode = E_Revive_GodMiracleNoReviveLocusOrPerfect;
			pRole->SendMessage(&send, send.dwSize);
			return E_Revive_GodMiracleNoReviveLocusOrPerfect;
		}
	}

	send.dwErrorCode	= pRole->Revive(p->eType, p->n64ItemID);

	if( P_VALID(pRole->GetMap()) )
	{
		pRole->GetMap()->SendBigVisTileMsg(pRole, &send, send.dwSize);
	}

	if(E_Success == send.dwErrorCode)
	{
		// 复活到指定地点
		pRole->GotoNewMap(pRole->m_Revive.dwMapID, 
			pRole->m_Revive.fX, pRole->m_Revive.fY, pRole->m_Revive.fZ);
	}

	return send.dwErrorCode;
}

//------------------------------------------------------------------------------
// 玩家点掉Buff
//------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleCancelBuff(tagNetCmd* pCmd)
{
	tagNC_CancelBuff* pRecv = (tagNC_CancelBuff*)pCmd;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	// 取消Buff
	pRole->CancelBuff(Buff::GetIDFromTypeID(pRecv->dwBuffTypeID));

	return 0;
}
// 开启妖精契约
DWORD PlayerSession::HandleOpenFairyTrain(tagNetCmd * pCmd)
{
	tagNC_OpenFairyTrain* pRecv = (tagNC_OpenFairyTrain*)pCmd;

	if( !g_world.IsFairyContractOpen() )
		return GT_INVALID;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	Map * pMap = pRole->GetMap();
	if( !P_VALID(pMap) )
		return GT_INVALID;

	MapFairyContract * pContract = pMap->GetFairyContract();
	if( !P_VALID(pContract) )
		return GT_INVALID;

	FairyContract * pTable = pContract->GetContract(pRecv->dwFairyID);
	if( !P_VALID(pTable) )
		return GT_INVALID;

	CContractItem * pTableItem = pTable->GetItem(pRole->GetID());
	
	if( !P_VALID(pTableItem) )
		return GT_INVALID;

	INT mxValue = 0,num = 5;
	DWORD dwCurID = 0;
	num = pTableItem->GetContractContent(mxValue,dwCurID,num,NULL,0);
	if( num > 0 )
	{
		jm_tool::TArray<DWORD> _dwArray(num);
		pTableItem->GetContractContent(mxValue,dwCurID,num,_dwArray,num);

		INT size_msg = sizeof(tagNS_OpenFairyTrain) + (num - 1) * sizeof(DWORD);
		MCREATE_MSG(pSend,size_msg,NS_OpenFairyTrain);
		pSend->dwCurItemID		= dwCurID;
		pSend->nItemKind			= num;
		pSend->nMaxContractValue	= mxValue;
		for( int i = 0; i < num ; ++i )
			pSend->dwItemID[i]		= _dwArray[i];
		pRole->SendMessage(pSend,pSend->dwSize);
		MDEL_MSG(pSend);
	}
	if(!pTable->IsStarted())
		pTable->Start();

	return 0;
}
// 兑现契约物品，也即捕捉妖精
DWORD PlayerSession::HandleArrestingFairy(tagNetCmd * pCmd)
{
	tagNC_ArrestingFairy* pRecv = (tagNC_ArrestingFairy*)pCmd;

	if( !g_world.IsFairyContractOpen() )
		return GT_INVALID;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	Map * pMap = pRole->GetMap();
	if( !P_VALID(pMap) )
		return GT_INVALID;

	MapFairyContract * pContract = pMap->GetFairyContract();
	if( !P_VALID(pContract) )
		return GT_INVALID;

	FairyContract * pTable = pContract->GetContract(pRecv->dwFairyID);
	if( !P_VALID(pTable) )
		return GT_INVALID;

	CContractItem * pTableItem = pTable->GetItem(pRole->GetID());

	if( !P_VALID(pTableItem) )
		return GT_INVALID;

	pTableItem->SetUserSelectedItem(pRecv->dwItemID);

	return 0;
}

DWORD PlayerSession::HandleCloseFairyTrain(tagNetCmd * pCmd)
{
	tagNC_CloseFairyTrain* pRecv = (tagNC_CloseFairyTrain*)pCmd;

	if( !g_world.IsFairyContractOpen() )
		return GT_INVALID;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	Map * pMap = pRole->GetMap();
	if( !P_VALID(pMap) )
		return GT_INVALID;

	MapFairyContract * pContract = pMap->GetFairyContract();
	if( !P_VALID(pContract) )
		return GT_INVALID;

	FairyContract * pTable = pContract->GetContract(pRecv->dwFairyID);
	if( !P_VALID(pTable) )
		return GT_INVALID;

	CContractItem * pTableItem = pTable->GetItem(pRole->GetID());

	if( !P_VALID(pTableItem) )
		return GT_INVALID;

	pTableItem->SetDiscarded(TRUE);

	return 0;
}

DWORD PlayerSession::HandleStopGodCondense(tagNetCmd * pCmd)
{
	tagNC_StopCondensed* pRecv = (tagNC_StopCondensed*)pCmd;

	if( !g_world.IsGodSystemOpen() )
		return GT_INVALID;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	pRole->StopCondense(pRecv->nType,E_Success,TRUE);

	if(pRecv->nType == 2)
	{
		if( pRole->IsHaveBuff (Buff::GetIDFromTypeID(GODCONDENSE_BUFFID)) )
		{
			pRole->RemoveBuff(Buff::GetIDFromTypeID(GODCONDENSE_BUFFID), TRUE);
		}
	}

	return E_Success;
}

DWORD PlayerSession::HandleStartGodCondense(tagNetCmd * pCmd)
{
	tagNC_BeginCondensed* pRecv = (tagNC_BeginCondensed*)pCmd;

	if( !g_world.IsGodSystemOpen() )
		return GT_INVALID;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;

	BOOL re = pRole->StartCondense();

	tagNS_BeginCondensed msg;
	msg.dwErrorCode = re ? E_Success : GT_INVALID;
	SendMessage(&msg,msg.dwSize);

	return E_Success;
}

//-----------------------------------------------------------------------------
// 通过道具增加领域
DWORD PlayerSession::HandleRoleUseLimitedItem(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_UseLimitedItem);

	// 获取人物
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_UseLimitedItem send;

	tagItem* pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64ItemID);
	if (P_VALID(pItem))
	{
		if (pItem->pProtoType->eSpecFunc == EISF_GlobalLimitedItem)
		{
			send.dwErrorCode = E_Success;
		}
		else
		{
			send.dwErrorCode = E_UseItem_ScriptError;
		}
	}
	else
	{
		send.dwErrorCode = E_UseItem_ItemNotExist;
	}
	
	if(E_Success == send.dwErrorCode)
	{
		send.dwErrorCode = pRole->GetItemMgr().AddAreaByItme(pRecv->n64ItemID);
	}

	SendMessage(&send, send.dwSize);

	return send.dwErrorCode;
}
//-----------------------------------------------------------------------------

