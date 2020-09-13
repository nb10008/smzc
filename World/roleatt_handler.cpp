//-----------------------------------------------------------------------------
//!\file roleatt_handler.cpp
//!\author Aslan
//!
//!\date 2008-07-28
//! last 2008-07-28
//!
//!\brief 移动消息处理类
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/role_att.h"
#include "../WorldDefine/role_info.h"
#include "../WorldDefine/selectrole.h"
#include "../WorldDefine/filter.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/msg_common.h"
#include "..\ServerDefine\family_define.h"
#include "pet_pocket.h"
#include "pet_soul.h"
#include "player_session.h"
#include "role.h"
#include "creature.h"
#include "role_mgr.h"
#include "map.h"
#include "pet.h"
#include "guild.h"
#include "guild_mgr.h"
#include "title_mgr.h"
#include "family_join.h"
#include "family.h"
#include "family_mgr.h"
#include "..\WorldDefine\msg_pulse.h"
#include "..\WorldDefine\msg_bloodbrother.h"

//------------------------------------------------------------------------------
// 获取人物属性
//------------------------------------------------------------------------------
DWORD PlayerSession::HandleGetRoleInitAtt(tagNetCmd* pCmd)
{
	tagNC_GetRoleInitState* pRecv = (tagNC_GetRoleInitState*)pCmd;

	if( pRecv->eType <= ERIT_Null || pRecv->eType >= ERIT_End )
		return GT_INVALID;

	Role* pRole = GetRole();
	if( !P_VALID(pRole) ) return GT_INVALID;


	switch(pRecv->eType)
	{
	case ERIT_Att:
		pRole->SendInitStateAtt();
		pRole->SendInitPersonalSet();
		pRole->CalPKState();
		break;
	case ERIT_Skill:
		pRole->SendInitStateSkill();
		break;
	case ERIT_Item:
		pRole->SendInitStateItem();
		pRole->SendInitStateSuit();
		pRole->SendInitStateLongHun();
		break;
	case ERIT_CompleteQuest:
		pRole->SendInitStateCompleteQuest();
		break;
	case ERIT_DailyCompleteQuest:
		pRole->SendInitStateDailyCompleteQuest();
		break;
	case ERIT_IncompleteQuest:
		pRole->SendInitStateIncompleteQuest();
		break;
	case ERIT_Money:
		pRole->SendInitStateMoney();
		break;
	case ERIT_Reputation:
		pRole->SendInitStateReputation();
		break;
	case ERIT_FrindAndEnemy:
		pRole->SendFriendAndEnemy();
		break;
	case ERIT_Guild:
		pRole->SendInitStateGuild();
		break;
	case ERIT_City:
		pRole->SendInitStateCity();
		break;
	case ERIT_Area:
		pRole->SendInitStateArea();
		break;
	case ERIT_ClientConfig:
		pRole->SendInitStateClientConfig();
		break;
	//case ERIT_HolySoulInfo:
		//pRole->SendInitHolySoulInfo();
		//break;
	default:
		break;
	}

	return 0;
}
//-----------------------------------------------------------------------------
// 获得可消耗实力值
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetConsumptiveStrength(tagNetCmd* pCmd)
{
	tagNC_GetConsumptiveStrength* pRecv = (tagNC_GetConsumptiveStrength*) pCmd;

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_GetConsumptiveStrength send;

	send.nConsumptiveStrength = pRole->GetStrength().nConsumptiveStrength;


	SendMessage(&send, send.dwSize);

	return 0;
}
//-----------------------------------------------------------------------------
// 获取角色的详细实力值
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetStrengthDetail(tagNetCmd* pCmd)
{
	tagNC_GetStrengthDetail* pRecv = (tagNC_GetStrengthDetail*) pCmd;

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_GetStrengthDetail send;
// 	send.nBaseStrength = pRole->GetStrength().nBaseStrength;
	send.nConsumptiveStrength = pRole->GetStrength().nConsumptiveStrength;
// 	send.nAdvanceStrength = pRole->GetStrength().nAdvanceStrength;
	
	send.nLevel = pRole->GetStrength().nLevel;
// 	send.nFame = pRole->GetStrength().nFame;
// 	send.nTitle = pRole->GetStrength().nTitle;
	send.nEquip = pRole->GetStrength().nEquipValue;
	// 客户端使用的是宠物，来显示妖精，
	send.nPet = pRole->GetStrength().nFairy;
// 	send.nFriendship = pRole->GetStrength().nFriendship;
// 	send.nQuest = pRole->GetStrength().nQuest;
// 	send.nProduction = pRole->GetStrength().nProduce;
	
// 	send.nRanQuest = pRole->GetStrength().nRanQuest;
// 	send.nActivity = pRole->GetStrength().nActivity;
// 	send.nInstance = pRole->GetStrength().nInstance;
// 	send.nNoWhiteNamePlayer = pRole->GetStrength().nKill;

	SendMessage(&send, send.dwSize);

	return 0;
}

DWORD PlayerSession::HandleGetPvPStatics(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}
	tagNS_SynPvPStatics send;
	send.nTotal = pRole->GetScriptData(24);
	send.nWinCount = pRole->GetScriptData(25);

	SendMessage(&send, send.dwSize);
	return 0;
}

DWORD PlayerSession::HandleRefreshBaoXiang(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	pRole->OnRefreshBaoXiang();

	return 0;
}

DWORD PlayerSession::HandleGetWarBaoXiang(tagNetCmd* pCmd)
{
	tagNC_GetWarBaoXiang* pRecv = (tagNC_GetWarBaoXiang*) pCmd;
	Role* pRole = GetRole();
	if (!P_VALID(pRole) || !P_VALID(pRecv))
	{
		return GT_INVALID;
	}

	DWORD dwRet = pRole->OnGetBaoXiang(pRecv->byIndex, pRecv->dwItemID);

	tagNS_GetWarBaoXiang msg;
	msg.dwErrorCode = dwRet;
	SendMessage(&msg, msg.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------
// 获取远程人物或生物属性（是不是要做些处理，如果玩家一直在发怎么办）
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleGetRemoteUnitAtt(tagNetCmd* pCmd)
{
	tagNC_GetRemoteRoleState* pRecv = (tagNC_GetRemoteRoleState*)pCmd;

	Role* pSelf = GetRole();
	if( !P_VALID(pSelf) || GT_INVALID == pSelf->GetVisTileIndex() ) return GT_INVALID;

	Map* pMap = pSelf->GetMap();
	if( !P_VALID(pMap) ) return GT_INVALID;

	if( pRecv->nRoleNum > 50 ) return GT_INVALID;	// 最多发50个

	// 顺序的轮询地图中的ID
	for(INT n = 0; n < pRecv->nRoleNum; n++)
	{
		if( !P_VALID(pRecv->dwRoleID[n]) )
			break;

		// 检查是人物还是生物
		if( IS_PLAYER(pRecv->dwRoleID[n]) )
		{
			Role* pRemoteRole = pMap->FindRole(pRecv->dwRoleID[n]);
			if( P_VALID(pRemoteRole) )
			{
				INT nBuffNum = pRemoteRole->GetBuffNum();
				DWORD dwSize = sizeof(tagNS_GetRemoteRoleState) + ((nBuffNum > 0) ? (nBuffNum - 1)*sizeof(tagBuffMsgInfo) : 0);

				// 发送远程人物属性同步消息
				MCREATE_MSG(pSend, dwSize, NS_GetRemoteRoleState);

				pSend->RoleData.dwID = pRemoteRole->GetID();
				pSend->RoleData.nLevel = pRemoteRole->GetLevel();

				pSend->RoleData.byStallLevel = pRemoteRole->GetStallModeLevel();
				pSend->RoleData.eClassType = pRemoteRole->GetClass();
				pSend->RoleData.eClassTypeEx = pRemoteRole->GetClassEx();

				pSend->RoleData.dwLoverID = pRemoteRole->GetLoverID();
				pSend->RoleData.bHaveWedding = pRemoteRole->IsRoleWedded();

				pRemoteRole->SynRemoteBodyFabaoInfo(pSend->RoleData.FabaoInfo);

				// 位置
				Vector3 vPos = pRemoteRole->GetCurPos();
				pSend->RoleData.fPos[0] = vPos.x;
				pSend->RoleData.fPos[1] = vPos.y;
				pSend->RoleData.fPos[2] = vPos.z;

				// 朝向
				Vector3 vFace = pRemoteRole->GetFaceTo();
				pSend->RoleData.fFaceTo[0] = vFace.x;
				pSend->RoleData.fFaceTo[1] = vFace.y;
				pSend->RoleData.fFaceTo[2] = vFace.z;

				// 状态
				pSend->RoleData.dwState = pRemoteRole->GetState();
				pSend->RoleData.dwRoleState = pRemoteRole->GetRoleState();
				pSend->RoleData.ePKState = pRemoteRole->GetPKState();

				// 帮派
				pSend->RoleData.dwGuildID = pRemoteRole->GetGuildID();
				pSend->RoleData.n8GuildPos = EGMP_Null;
				if(pRemoteRole->IsInGuild())
				{
					Guild *pGuild = g_guildMgr.GetGuild(pRemoteRole->GetGuildID());
					if(P_VALID(pGuild))
					{
						tagGuildMember *pMember = pGuild->GetMember(pRemoteRole->GetID());
						if(P_VALID(pMember))
						{
							pSend->RoleData.n8GuildPos = pMember->eGuildPos;
						}
					}
				}

				// 骑乘宠物属性
				PetSoul* pSoul = pRemoteRole->GetPetPocket()->GetMountPetSoul();
				if (P_VALID(pSoul))
				{
					pSend->RoleData.dwMountPetID = pSoul->GetID();
					pSend->RoleData.dwMountPetTypeID = pSoul->GetProtoID();
				}
				else
				{
					pSend->RoleData.dwMountPetID = GT_INVALID;
					pSend->RoleData.dwMountPetTypeID = GT_INVALID;
				}

				if (pRemoteRole->GetPetPocket()->IsRideAfter())
				{
					pSend->RoleData.dwMountPetID = pRemoteRole->GetPetPocket()->GetHostPetID();
				}

				// 属性
				pSend->RoleData.nAtt[ERRA_MaxHP]		= pRemoteRole->GetAttValue(ERA_MaxHP);
				pSend->RoleData.nAtt[ERRA_HP]			= pRemoteRole->GetAttValue(ERA_HP);
				pSend->RoleData.nAtt[ERRA_MaxMP]		= pRemoteRole->GetAttValue(ERA_MaxMP);
				pSend->RoleData.nAtt[ERRA_MP]			= pRemoteRole->GetAttValue(ERA_MP);
				pSend->RoleData.nAtt[ERRA_Rage]			= pRemoteRole->GetAttValue(ERA_Rage);
				pSend->RoleData.nAtt[ERRA_Speed_XZ]		= pRemoteRole->GetAttValue(ERA_Speed_XZ);
				pSend->RoleData.nAtt[ERRA_Speed_Y]		= pRemoteRole->GetAttValue(ERA_Speed_Y);
				pSend->RoleData.nAtt[ERRA_Speed_Swim]	= pRemoteRole->GetAttValue(ERA_Speed_Swim);
				pSend->RoleData.nAtt[ERRA_Speed_Mount]	= pRemoteRole->GetAttValue(ERA_Speed_Mount);
				pSend->RoleData.nAtt[ERRA_Shape]		= pRemoteRole->GetAttValue(ERA_Shape);
				pSend->RoleData.nAtt[ERRA_Hostility]	= pRemoteRole->GetAttValue(ERA_Hostility);
				pSend->RoleData.nAtt[ERRA_Soaring]		= pRemoteRole->GetAttValue(ERA_Soar);
				pSend->RoleData.nAtt[ERRA_VipLevel]		= pRemoteRole->GetAttValue(ERA_VipLevel);
				

				// 对远端玩家公开信息设置
				pSend->RoleData.sRemoteOpenSet			= pRemoteRole->GetRemoteOpenSet();

				// 当前称号
				pSend->RoleData.u16CurActTitleID		= pRemoteRole->GetTitleMgr()->GetActiviteTitle();

				// 外观
				pSend->RoleData.sDisplaySet				= pRemoteRole->GetDisplaySet();
				IFASTCODE->MemCpy(&pSend->RoleData.Avatar, pRemoteRole->GetAvatar(), sizeof(tagAvatarAtt));
				//pSend->RoleData.Avatar.byTransSkinIndex = pSelf->GetTransSkinNdx();
				IFASTCODE->MemCpy(&pSend->RoleData.AvatarEquip, &pRemoteRole->GetAvatarEquip(), sizeof(tagAvatarEquip));

				pSend->RoleData.byFashionTemperament = pRemoteRole->GetWardrobe().GetCurFashionTemperament();

				// 神职
				pSend->RoleData.dwClergyID = pRemoteRole->GetClergy();

				// 家族信息
				Family* pFamily = g_FamilyMgr.GetFamily(pRemoteRole->GetFamilyID());
				pSend->RoleData.sRoleFamilyInfo.dwFamilyID = P_VALID(pFamily)?pRemoteRole->GetFamilyID():GT_INVALID;
				pSend->RoleData.sRoleFamilyInfo.bLeader = P_VALID(pFamily)?pFamily->MemberIsLeader(pRemoteRole->GetID()):FALSE;

				// 状态列表
				pSend->RoleData.nBuffNum = nBuffNum;
				if( nBuffNum > 0 )
				{
					pRemoteRole->GetAllBuffMsgInfo(pSend->RoleData.Buff, nBuffNum);
				}

				// 如果是召唤圣灵状态，发送圣灵Id和圣灵吃妖精次数
				if (pRemoteRole->IsInRoleState(ERS_CALLHOLY))
				{
					tagHolyMan * pHoly = (tagHolyMan *)pRemoteRole->GetItemMgr().GetEquipBarEquip((INT16)EEP_Holy);
					if(P_VALID(pHoly))
					{
						pSend->RoleData.dwHolySoulID = pHoly->dwTypeID;
						pSend->RoleData.dwEatFairyNum = pHoly->nDevourNum;
					}
				}

				// 阵营
				pSend->RoleData.eCamp = pRemoteRole->GetCampType();

				SendMessage(pSend, pSend->dwSize);

				MDEL_MSG(pSend);
			}
		}

		else if( IS_CREATURE(pRecv->dwRoleID[n]))
		{
			Creature* pCreature = pMap->FindCreature(pRecv->dwRoleID[n]);
			if(!P_VALID(pCreature)) pCreature = pMap->FindDeadDoor(pRecv->dwRoleID[n]);
			if( P_VALID(pCreature) )
			{
				INT nBuffNum = pCreature->GetBuffNum();
				DWORD dwSize = sizeof(tagNS_GetRemoteCreatureState) + ((nBuffNum > 0) ? (nBuffNum - 1)*sizeof(tagBuffMsgInfo) : 0);

				// 发送远程生物属性同步消息
				MCREATE_MSG(pSend, dwSize, NS_GetRemoteCreatureState);
				
				pSend->CreatureData.dwID = pCreature->GetID();
				pSend->CreatureData.dwTypeID = pCreature->GetTypeID();
				pSend->CreatureData.nLevel = pCreature->GetLevel();
				
				pSend->CreatureData.bOpen = pCreature->GetDoorStatus();
				pSend->CreatureData.dwMapObjID = pCreature->GetMapObjID();
			
				// 位置
				Vector3 vPos = pCreature->GetCurPos();
				pSend->CreatureData.fPos[0] = vPos.x;
				pSend->CreatureData.fPos[1] = vPos.y;
				pSend->CreatureData.fPos[2] = vPos.z;

				// 状态
				pSend->CreatureData.dwState = pCreature->GetState();

				// 所属
				pSend->CreatureData.dwTaggedOwner = pCreature->GetTaggedOwner();

				// 朝向
				Vector3 vFace = pCreature->GetFaceTo();
				pSend->CreatureData.fFaceTo[0] = vFace.x;
				pSend->CreatureData.fFaceTo[1] = vFace.y;
				pSend->CreatureData.fFaceTo[2] = vFace.z;

				// 属性
				pSend->CreatureData.nAtt[ERRA_MaxHP]		= pCreature->GetAttValue(ERA_MaxHP);
				pSend->CreatureData.nAtt[ERRA_HP]			= pCreature->GetAttValue(ERA_HP);
				pSend->CreatureData.nAtt[ERRA_MaxMP]		= pCreature->GetAttValue(ERA_MaxMP);
				pSend->CreatureData.nAtt[ERRA_MP]			= pCreature->GetAttValue(ERA_MP);
				pSend->CreatureData.nAtt[ERRA_Rage]			= pCreature->GetAttValue(ERA_Rage);
				pSend->CreatureData.nAtt[ERRA_Speed_XZ]		= pCreature->GetAttValue(ERA_Speed_XZ);
				pSend->CreatureData.nAtt[ERRA_Speed_Y]		= pCreature->GetAttValue(ERA_Speed_Y);
				pSend->CreatureData.nAtt[ERRA_Shape]		= pCreature->GetAttValue(ERA_Shape);
				pSend->CreatureData.nAtt[ERRA_Soaring]		= 0;
				pSend->CreatureData.nAtt[ERRA_VipLevel]	= EVL_None;

				pSend->CreatureData.eCamp					= pCreature->GetCampType();
				// 状态列表
				pSend->CreatureData.nBuffNum = nBuffNum;
				if( nBuffNum > 0 )
				{
					pCreature->GetAllBuffMsgInfo(pSend->CreatureData.Buff, nBuffNum);
				}

				SendMessage(pSend, pSend->dwSize);

				MDEL_MSG(pSend);

				//-- 再次发送走，防止漂移
				BYTE	byMsg[1024] = {0};
				DWORD	dwSize1 = pMap->CalMovementMsgEx(pCreature, byMsg, 1024);
				SendMessage(byMsg, dwSize1);
			}
		}
		else if (IS_PET(pRecv->dwRoleID[n]))
		{
			Pet* pPet = pMap->FindPet(pRecv->dwRoleID[n]);
			if (P_VALID(pPet))
			{
				tagNS_GetRemotePetState send;
				send.PetData.dwID		= pPet->GetID();
				send.PetData.dwProtoID	= pPet->GetTypeID();
				send.PetData.uState.byPetState		= pPet->GetPetState();
				Role* pMaster			= pPet->GetMaster();
				send.PetData.dwRoleID	= P_VALID(pMaster) ? pMaster->GetID() : GT_INVALID;

				SendMessage(&send, send.dwSize);
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理玩家得到其他人的RoleID
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetID(tagNetCmd *pCmd)
{
	tagNC_RoleGetID* pRecv = (tagNC_RoleGetID*)pCmd;
	DWORD dwNameCrc = pRecv->dwRoleNameCrc;
	DWORD dwRoleID = g_roleMgr.GetRoleIDByNameCrc(dwNameCrc);

	tagNS_RoleGetID send;
	send.dwRoleNameCrc = dwNameCrc;
	send.dwRoleID = dwRoleID;
	if (GT_VALID(dwRoleID))
	{
		g_roleMgr.GetRoleNameByNameID(dwRoleID, send.szName);
	}

	SendMessage(&send, send.dwSize);
	return 0;
}

//-----------------------------------------------------------------------------
// 通过NameID得到玩家的名字
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetNameByNameID(tagNetCmd* pCmd)
{
	
	MGET_MSG(pRecv, pCmd, NC_GetNamebyNameID);
	
	tagNS_GetNamebyNameID send;
	send.bResult = TRUE;
	send.dwNameID = pRecv->dwNameID;
	g_roleMgr.GetRoleNameByNameID(pRecv->dwNameID, send.szName);

	SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------------
// 客户端获取多个玩家名字
//------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleGetSomeName(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_RoleGetSomeName);

	INT		nNum = pRecv->nNum;

	if(nNum <= 0)
		return 0;

	if(nNum > 50)
		return 0;

	DWORD	dwSize = sizeof(tagNS_RoleGetSomeName) + (nNum - 1) * sizeof(tagRoleIDName);
	MCREATE_MSG(pSend, dwSize, NS_RoleGetSomeName);

	pSend->nUserData = pRecv->nUserData;
	pSend->nNum = nNum;

	for(INT n = 0; n < nNum; ++n)
	{
		pSend->IdName[n].dwID = pRecv->dwAllID[n];
		g_roleMgr.GetRoleNameByID(pRecv->dwAllID[n], pSend->IdName[n].szName);
	}

	SendMessage(pSend, pSend->dwSize);

	MDEL_MSG(pSend);

	return 0;
}

//------------------------------------------------------------------------------
// 投点
//------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleBidAttPoint(tagNetCmd* pCmd)
{
	tagNC_RoleBidAttPoint* pRecv = (tagNC_RoleBidAttPoint*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	DWORD dwErrorCode = m_pRole->BidAttPoint(pRecv->nAttPointAdd);

	tagNS_RoleBidAttPoint send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------
// 洗点
//-------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleClearAttPoint(tagNetCmd* pCmd)
{
	tagNC_RoleClearAttPoint* pRecv = (tagNC_RoleClearAttPoint*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	DWORD dwErrorCode = m_pRole->ClearAttPoint(pRecv->n64ItemID);

	tagNS_RoleClearAttPoint send;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------
// 角色外观显示模式设置
//-------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetFashion(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_Fashion);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 获取地图
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}
	
	// 检查是否和当前显示模式相同
	if(pRole->GetDisplaySet().bFashionDisplay == p->bFashion)
	{
		return 0;
	}

	// 设置显示模式
	pRole->SetFashionMode(p->bFashion);

	// 向周围玩家发送新的外观信息
	tagNS_AvatarEquip send;
	send.dwRoleID		= pRole->GetID();
	send.bFashion		= p->bFashion;
	send.sAvatarEquip	= pRole->GetAvatarEquip();
	pMap->SendBigVisTileMsg(pRole, &send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------
// 角色外观显示部位设置
//-------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleSetDisplay(tagNetCmd* pCmd)
{
	MGET_MSG(p, pCmd, NC_RoleSetDisplay);

	// 获取角色
	Role *pRole = GetRole();
	if(!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 获取地图
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	// 检查是否和当前部位设置属性相同
	const tagDisplaySet& sDisplaySet = pRole->GetDisplaySet();
	if(sDisplaySet.bHideBack == p->sDisplaySet.bHideBack
		&& sDisplaySet.bHideFace == p->sDisplaySet.bHideFace
		&& sDisplaySet.bHideHead == p->sDisplaySet.bHideHead)
	{
		return 0;
	}

	// 设置部位显示属性
	pRole->SetDisplaySet(p->sDisplaySet.bHideHead, p->sDisplaySet.bHideFace, p->sDisplaySet.bHideBack);

	// 向周围玩家发送消息
	tagNS_RoleSetDisplay send;
	send.dwRoleID		= pRole->GetID();
	send.sDisplaySet	= sDisplaySet;
	pMap->SendBigVisTileMsg(pRole, &send, send.dwSize);

	return 0;
}

//-------------------------------------------------------------------------------
// 角色改名(使用易名贴)
//-------------------------------------------------------------------------------
DWORD PlayerSession::HandleChangeRoleName(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_LPRename);

	DWORD dwErrorCode = E_Success;

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 获取地图
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}

	pRecv->szNewName[X_SHORT_NAME-1] = 0;
	tagItem *pItem = pRole->GetItemMgr().GetBagItem(pRecv->n64RenameItemID);
	if(!P_VALID(pItem))
	{
		dwErrorCode = E_UseItem_ItemNotExist;
		
		tagNS_LPRename send;
		send.dwRoleID = pRole->GetID();
		send.dwErrorCode = dwErrorCode;
		_tcscpy_s(send.szNewName, X_SHORT_NAME, pRecv->szNewName);
		ZeroMemory(send.szOldName, X_SHORT_NAME);
		//_tcscpy_s(send.szOldName, X_SHORT_NAME, szOldName );
		SendMessage(&send, send.dwSize);

		return dwErrorCode;
	}

	// 检查道具的合法性
	if(pItem->pProtoType->eSpecFunc != EISF_RenameItem)
	{
		dwErrorCode = E_UseItem_SpecFunError;

		tagNS_LPRename send;
		send.dwRoleID = pRole->GetID();
		send.dwErrorCode = dwErrorCode;
		_tcscpy_s(send.szNewName, X_SHORT_NAME, pRecv->szNewName);
		ZeroMemory(send.szOldName, X_SHORT_NAME);
		//_tcscpy_s(send.szOldName, X_SHORT_NAME, szOldName );
		SendMessage(&send, send.dwSize);

		return dwErrorCode;
	}

	// 检查名字合法性
	TCHAR buf[X_SHORT_NAME] = {0};
	_tcsncpy(buf, pRecv->szNewName, X_SHORT_NAME);
	buf[X_SHORT_NAME-1] = _T('\0');
	_tcslwr(buf);
	DWORD dwNameCrc = IUTIL->Crc32(buf);

	if ( g_roleMgr.GetRoleIDByNameCrc(dwNameCrc) != (DWORD)GT_INVALID )
	{
		//RoleName已经存在
		dwErrorCode = E_CreateRole_NameExist;
	}
	else
	{
		// 检查名字长度,合法性由客户端保证
		dwErrorCode = Filter::CheckName(buf, g_attRes.GetVariableLen().nRoleNameMax, g_attRes.GetVariableLen().nRoleNameMin, g_attRes.GetNameFilterWords());
	}

	if ( dwErrorCode != E_Success )
	{
		tagNS_LPRename send;
		send.dwRoleID = pRole->GetID();
		send.dwErrorCode = dwErrorCode;
		_tcscpy_s(send.szNewName, X_SHORT_NAME, pRecv->szNewName);
		ZeroMemory(send.szOldName, X_SHORT_NAME);
		//_tcscpy_s(send.szOldName, X_SHORT_NAME, szOldName );
		SendMessage(&send, send.dwSize);

		return dwErrorCode;
	}

	// 将新名字发往DB进行验证
	pRecv->szNewName[X_SHORT_NAME-1] = '\0';
	tagNDBC_CheckName send;
	send.dwRoleID = pRole->GetID();
	send.n64ItemSerial = pItem->n64Serial;
	g_roleMgr.GetRoleNameByID( pRole->GetID(), send.szOldName );
	_tcscpy_s(send.szNewName, X_SHORT_NAME, pRecv->szNewName);
	g_dbSession.Send(&send, send.dwSize);	

	return TRUE;
}

//-------------------------------------------------------------------------------
// 设置角色个人设置
//-------------------------------------------------------------------------------
DWORD PlayerSession::HandleSetPersonalSet(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_SetPersonalSet);

	//改变内存中的配置信息
	m_sAccountCommon.stPersonalSet = pRecv->PersonalSet;


	//更新数据库
	tagNDBC_PersonalSet send;
	send.dwAccountID = GetSessionID();
	send.stPersonalSet = m_sAccountCommon.stPersonalSet;

	g_dbSession.Send(&send,send.dwSize);

	return TRUE;
}	

//-------------------------------------------------------------------------------
// 检查远端玩家的信息开发类型
//-------------------------------------------------------------------------------
DWORD PlayerSession::HandleCheckRemoteRoleInfoOpenType(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_CheckRemoteRoleInfoOpenType);


	DWORD dwErrorCode = ECRI_E_Success;

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 获取地图
	Map *pMap = pRole->GetMap();
	if(!P_VALID(pMap))
	{
		return GT_INVALID;
	}
	// 被申请者检查
	Role *pTarget = pMap->FindRole(pRecv->dwRoleID);
	if(!P_VALID(pTarget))
	{
		return GT_INVALID;
	}

	//仅好友可见
	if(pTarget->GetSession()->GetPersonalSet().eInfoOpenType == EPIS_Friend)
	{
		tagFriend* pFriRole = pRole->GetFriendPtr(pRecv->dwRoleID);
		if( !P_VALID(pFriRole) )
		{
			dwErrorCode = ECRI_E_FriendOnly;
		}
	}
	//隐私
	else if(pTarget->GetSession()->GetPersonalSet().eInfoOpenType == EPIS_Private)
	{
		dwErrorCode = ECRI_E_Private;
	}

	tagNS_CheckRemoteRoleInfoOpenType send;
	send.dwRoleID = pRecv->dwRoleID;
	send.dwErrorCode = dwErrorCode;

	SendMessage(&send, send.dwSize);

	return 0;
};
// Jason 2010-5-18 v2.1.2 飞升相关

DWORD PlayerSession::HandleQuerySoaringSwitch(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_QuerySoaringSwitch);
	if( !P_VALID(pRecv) )
		return GT_INVALID;
	Role* pRole = GetRole();
	if( !P_VALID(pRole) )
		return GT_INVALID;
	tagNS_QuerySoaringSwitch msg;
	if( !g_world.IsSoaringOpen() )
		msg.dwErrorCode = ESEC_ServiceCannotBeProvided;
	else if( pRole->GetLevel() < 80 )
		msg.dwErrorCode = ESEC_LevelLimitted;
	else if( pRole->GetAttValue(ERA_Soar) == ESV_SoaringUp )
		msg.dwErrorCode = ESEC_AlreadyOpened;
	else
	{
		INT val = pRole->GetAttValue(ERA_Soar);
		if( pRecv->bQueryOrOpen ) // 要开启飞升
		{
			switch(val)
			{
			case ESV_NotCompletedSoaring:
				msg.dwErrorCode = ESEC_NotCompleted;
				if( !pRole->IsHaveQuest(40001) && !pRole->IsHaveDoneQuest(40001) )
				{
					msg.dwErrorCode = ESEC_CanOpen;
					pRole->OnReadyToSoaring();
				}
				break;
			default:
				//pRole->SetAttValue(ERA_Soar,ESV_NotCompletedSoaring,FALSE) ;
				msg.dwErrorCode = ESEC_CanOpen;
				pRole->OnReadyToSoaring();
				break;
			}
		}
		else
			msg.dwErrorCode = ESEC_CanOpen;
	}

	SendMessage(&msg,msg.dwSize);

	return E_Success;
}

DWORD	PlayerSession::HandleTargetOfTarget(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_TargetOfTarget);
	if( !P_VALID(pRecv) )
		return GT_INVALID;
	Role* pRole = GetRole();
	if( !P_VALID(pRole) )
		return GT_INVALID;

	if (pRole->GetAttValue(ERA_Target) == pRecv->dwTargetID)
	{
		return GT_INVALID;
	}

	Map * pMap = pRole->GetMap();
	if( !P_VALID(pMap) )
		return GT_INVALID;

	Unit * pUnit = pMap->FindUnit(pRecv->dwTargetID);
	pRole->ChangeTarget(pUnit);

	return E_Success;
}

DWORD PlayerSession::HandleRoleInitDone(tagNetCmd * pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_RoleInitDone);
	if( !P_VALID(pRecv) )
		return GT_INVALID;
	Role* pRole = GetRole();
	if( !P_VALID(pRole) )
		return GT_INVALID;

	// 同步变身buff
	pRole->SendAvatarEquip2Client();

	// 同步经脉剩余修炼次数
	tagNS_PulseCanLearnNum msg;
	msg.dwNum = pRole->GetPulseLearnTime();
	SendMessage(&msg,msg.dwSize);

	// 同步角色剩余结拜时间
	if (!pRole->GetBrotherMgr().CanMakeBrother())
	{
		tagNS_BrotherLeftSecond msg2;
		msg2.dwTeacherID = pRole->GetBrotherMgr().GetTeatherID();
		msg2.dwEndTime = pRole->GetBrotherMgr().GetEndTime();
		SendMessage(&msg2,msg2.dwSize);
	}

	return E_Success;
}

