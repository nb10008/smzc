/********************************************************************
created:	2008/12/09
created:	09:12:2008   00:05
filename: 	d:\loong_client\Client\ToolTipScriptFuncImpl.cpp
file path:	d:\loong_client\Client
file base:	ToolTipScriptFuncImpl
file ext:	cpp
author:		lynx

purpose:	脚本函数接口实现，供策划调用
*********************************************************************/
#include "StdAfx.h"
#include "RoleMgr.h"
#include "Role.h"
#include "Item.h"
#include "PlayerNameTab.h"
#include "LocalPlayer.h"
#include "SkillMgr.h"
#include "ItemMgr.h"
#include "CurrencyMgr.h"
#include "LevelUpData.h"
#include "ItemProtoData.h"
#include "SkillProtoData.h"
#include "..\WorldDefine\shop_define.h"
#include "..\WorldDefine\currency_define.h"
#include "ConsolidateData.h"
#include "..\WorldDefine\compose_define.h"
#include "..\WorldDefine\suit_define.h"
#include "LonghunAttData.h"
#include "..\WorldDefine\longhun_define.h"
#include "ServerTime.h"
#include "TreasureMgr.h"
#include "ReputeMgr.h"
#include "PetProtoData.h"
#include "GuildDefine.h"
#include "GuildMgr.h"
#include "CityMgr.h"
#include "SoarMgr.h"
#include "ItemTimerMgr.h"
#include "GodAreaData.h"
#include "GodMgr.h"
#include "..\WorldDefine\pulse_define.h"
#include "HolyManMgr.h"

// Jason 2010-7-10 获得装备强化属性
static int SC_GetEqipConsolidateAtt(lua_State *L);

//-------------------------------------------------------------------------------------
//							       本地玩家信息相关函数
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// 获取本地玩家等级
//-------------------------------------------------------------------------------------
int SC_GetLocalPlayerLevel(lua_State* L)
{
	//GetLocalPlayerLevel()
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		lua_pushnumber(L, pLp->GetRoleLevel());
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取本地玩家神格
//-------------------------------------------------------------------------------------
int SC_GetLocalPlayerGodHead(lua_State* L)
{
	//GetLocalPlayerLevel()
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		lua_pushnumber(L, pLp->GetAttribute(ERA_God_Godhead));
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取本地玩家性别
//-------------------------------------------------------------------------------------
int SC_GetLocalPlayerSex(lua_State* L)
{
	//GetLocalPlayerSex()
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		lua_pushnumber(L, pLp->GetPlayerSex());
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取本地玩家职业
//-------------------------------------------------------------------------------------
int SC_GetLocalPlayerClass(lua_State* L)
{
	//GetLocalPlayerClass()

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取本地玩家某项氏族声望等级
//-------------------------------------------------------------------------------------
int SC_GetLocalPlayerRepution(lua_State* L)
{
	//GetLocalPlayerRepution(eClan)
	ECLanType eClan = (ECLanType)(DWORD)(lua_tonumber(L, -1));
	if(eClan > ECLT_NULL && eClan < ECLT_END)
	{
		lua_pushnumber(L, ReputeMgr::Inst()->GetReputeLevel(eClan)->eLevel);
		return 1;
	}

	return 0;
}
//-------------------------------------------------------------------------------------
// 获取本地玩家某项氏族贡献值
//-------------------------------------------------------------------------------------
int SC_GetLocalPlayerContribution(lua_State* L)
{
	//GetLocalPlayerContribution(eClan)
	ECLanType eClan = (ECLanType)(DWORD)(lua_tonumber(L, -1));
	if(eClan > ECLT_NULL && eClan < ECLT_END)
	{
		lua_pushnumber(L, CurrencyMgr::Inst()->GetClanContribute(eClan));
		return 1;
	}

	return 0;
}
//-------------------------------------------------------------------------------------
// 获取本地玩家某项天资投放点
//-------------------------------------------------------------------------------------
int SC_GetLocalPlayerTalentBidden(lua_State* L)
{
	//GetLocalPlayerTalentBidden(eTalent)
	ETalentType eTalent = (ETalentType)(DWORD)(lua_tonumber(L, -1));
	if(eTalent > ETT_Null && eTalent < ETT_End)
	{
		lua_pushnumber(L, SkillMgr::Inst()->GetTalentPoint(eTalent));
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取本地玩家属性
//-------------------------------------------------------------------------------------
int SC_GetLocalPlayerAttribute(lua_State* L)
{
	//GetLocalPlayerAttribute(eAtt)
	ERoleAttribute eAtt = (ERoleAttribute)(DWORD)(lua_tonumber(L, -1));
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		if(eAtt > ERA_Null && eAtt < ERA_End)
		{
			lua_pushnumber(L, pLp->GetAttribute(eAtt));
			return 1;
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取本地玩家属性投放值
//-------------------------------------------------------------------------------------
int SC_GetLocalPlayerAttBidden(lua_State* L)
{
	//GetLocalPlayerAttBidden(eAtt)
	ERoleAttribute eAtt = (ERoleAttribute)(DWORD)(lua_tonumber(L, -1));
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		if(eAtt >= ERA_AttA_Start && eAtt <= ERA_AttA_End)
		{
			lua_pushnumber(L, pLp->GetAttPointAdd(eAtt));
			return 1;
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------
// 获取本地玩家升级属性增加值
//------------------------------------------------------------------------------------
int SC_GetLocalPlayerAttLevelup(lua_State* L)
{
	//GetLocalPlayerAttLevelup(eAtt)
	ERoleAttribute eAtt = (ERoleAttribute)(DWORD)lua_tonumber(L, -1);
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		const tagExperience* pLevelup = LevelUpData::Inst()->FindExpData(pLp->GetRoleLevel());
		if(P_VALID(pLevelup))
		{
			if(eAtt >= ERA_AttA_Start && eAtt <= ERA_AttA_End)
			{
				lua_pushnumber(L, pLevelup->nAtt[eAtt]);
				return 1;
			}
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取本地玩家可消耗属性当前值
//-------------------------------------------------------------------------------------
int SC_GetLocalPlayerCostAttCurValue(lua_State* L)
{
	ESkillCostType eCost = (ESkillCostType)(DWORD)lua_tonumber(L, -1);
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp))
	{
		ERoleAttribute eType = ERA_Null;
		switch (eCost)
		{
		case ESCT_HP:
			eType = ERA_HP;	
			break;
		case ESCT_MP:
			eType = ERA_MP;
			break;
		case ESCT_Rage:
			eType = ERA_Rage;
			break;
		case ESCT_Endurance:
			eType = ERA_Endurance;
			break;
		case ESCT_Valicity:
			eType = ERA_Vitality;
			break;
		}
		if (ERA_Null != eType)
		{
			lua_pushnumber(L, pLp->GetAttribute(eType));
			return 1;
		}
	}	
	return 0;
}


//-------------------------------------------------------------------------------------
//							          物品信息相关函数
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// 获取物品名称
//-------------------------------------------------------------------------------------
int SC_GetItemName(lua_State *L)
{
	//GetItemName(obj)
	Item* pItem = (Item*)(lua_touserdata(L, -1));
	if(P_VALID(pItem) && pItem->IsItem())
	{
		tstring strTmp;
		//如果珍宝激活名称有效
		if(P_VALID(pItem->GetData()->dwNameID))
			strTmp += PlayerNameTab::Inst()->FindNameByNameID(pItem->GetData()->dwNameID);
		strTmp += pItem->GetDisplay()->szName;
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(strTmp.c_str());
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, strTmp.c_str(), strTmp.size());
#endif		
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品名称
//-------------------------------------------------------------------------------------
int SC_GetItemNameEx(lua_State *L)
{
	//GetItemNameEx(proto,name)
	const tagItemProto* pProto = (const tagItemProto*)(lua_touserdata(L, -2));
	DWORD dwNameId = (DWORD)(lua_tonumber(L, -1));
	if(P_VALID(pProto))
	{
		LPCTSTR szName;
		//如果珍宝激活名称有效
		if(P_VALID(dwNameId))
			szName = TreasureMgr::Inst()->GetTreasureName(dwNameId).c_str();
		else
			szName = pProto->pDisplayInfo->szName;
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szName);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, szName, strlen(szName));
#endif		
		return 1;
	}
	
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品描述
//-------------------------------------------------------------------------------------
int SC_GetItemDesc(lua_State *L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->pDisplayInfo->szDesc);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->pDisplayInfo->szDesc, strlen(pProto->pDisplayInfo->szDesc));
#endif
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取物品类型
//-------------------------------------------------------------------------------------
int SC_GetItemType(lua_State *L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->pDisplayInfo->szType);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else 
		lua_pushlstring(L, pProto->pDisplayInfo->szType, strlen(pProto->pDisplayInfo->szType));
#endif
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品类型
//-------------------------------------------------------------------------------------
int SC_GetEquipTypeDesc(lua_State *L)
{
	Item*				pItem  = (Item*)		(lua_touserdata(L, -2));
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		tstring szType = pProto->pDisplayInfo->szType;
		if( pProto->eType == EIT_FaBao ) // 法宝做特殊处理
		{
			const tagFabaoSpec* pFaBaoSpec = ((Equipment*)pItem)->GetFaBaoSpe();
			tagFabaoProto* pFaBaoProto = (tagFabaoProto*) pProto;
			BOOL bIsIdentified = !((Equipment*)pItem)->IsNotIdetified();
			szType = _T("");
			if(bIsIdentified)
			{
				switch(pFaBaoSpec->eWuxing)
				{
				case EWX_Metal:
					szType += g_StrTable[_T("Trump_EWuXing_0")];
					break;
				case EWX_Wood:
					szType += g_StrTable[_T("Trump_EWuXing_1")];
					break;
				case EWX_Water:
					szType += g_StrTable[_T("Trump_EWuXing_2")];
					break;
				case EWX_Fire:
					szType += g_StrTable[_T("Trump_EWuXing_3")];
					break;
				case EWX_Soil:
					szType += g_StrTable[_T("Trump_EWuXing_4")];
					break;
				}
			}
			switch(pFaBaoProto->eFarbaoType)
			{
			case EFB_Attack: // 攻击型法宝
				szType += g_StrTable[_T("TrumpType_0")];
				break;
			case EFB_Defense: // 防御型法宝
				szType += g_StrTable[_T("TrumpType_1")];
				break;
			case EFB_Assit: // 辅助型法宝
				szType += g_StrTable[_T("TrumpType_2")];
				break;
			}
			if(bIsIdentified)
			{
				TCHAR temp[100];
#ifdef UNICODE
				wsprintf(temp, _T("%d"), pFaBaoSpec->n16Stage);
#else 
				sprintf(temp,  _T("%d"), pFaBaoSpec->n16Stage);
#endif
				szType += _T("(");
				szType += temp;
				szType += _T(")");
			}
		}
#ifdef UNICODE
			LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szType.c_str());
			lua_pushlstring(L, szTmp, strlen(szTmp));
#else 
			lua_pushlstring(L, szType.c_str(), szType.length());
#endif
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品类型ID
//-------------------------------------------------------------------------------------
int SC_GetItemTypeID(lua_State *L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->dwTypeID );
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品品级
//-------------------------------------------------------------------------------------
int SC_GetItemQuality(lua_State *L)
{
	Item*				pItem  = (Item*)		(lua_touserdata(L, -2)); 
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		// 如果是装备，不能根据静态属性获得物品品级
		if(MIsEquipment(pProto->dwTypeID))
		{
			if(P_VALID(pItem) && pItem->IsItem())
			{
				Equipment* pEquip = (Equipment*)pItem;
				lua_pushnumber(L, pEquip->GetItemQuality());
				return 1;
			}
			else 
			{
				lua_pushnumber(L, (BYTE)GT_INVALID);
				return 1;
			}
		}
		else
		{
			lua_pushnumber(L, pProto->byQuality);
			return 1;
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取物品绑定类别
//-------------------------------------------------------------------------------------
int SC_GetItemBindType(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->byBindType);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取物品绑定角色名
//-------------------------------------------------------------------------------------
int SC_GetItemBindRoleName(lua_State* L)
{
    Item* pItem = (Item*)(lua_touserdata(L, -1)); 
    if(P_VALID(pItem))
    {
        LPCTSTR szRoleName = PlayerNameTab::Inst()->FindNameByID(pItem->GetItemBindingRole());
        if(szRoleName != _T(""))
        {
#ifdef UNICODE
            LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szRoleName);
            lua_pushlstring(L, szTmp, strlen(szTmp));
#else
            lua_pushlstring(L, szRoleName, strlen(szRoleName));
#endif
            return 1;
        }
    }
    return 0;
}


//-------------------------------------------------------------------------------------
// 获取物品约定名
//-------------------------------------------------------------------------------------
int SC_GetItemYueDingRoleName(lua_State* L)
{
	Item* pItem = (Item*)(lua_touserdata(L, -1)); 
	if(P_VALID(pItem))
	{
		const tagItemProto *pProto = pItem->GetProto();
		const tagItem *pData = pItem->GetData();
		if(P_VALID(pProto) && P_VALID(pData) && pData->dwSpecVal1 != 0 && 
			( pProto->eSpecFunc == EISF_ExpBall || EISF_ExpPilular == pProto->eSpecFunc ) )
		{
			LPCTSTR szRoleName = PlayerNameTab::Inst()->FindNameByID(pData->dwSpecVal1);
			if(szRoleName != _T(""))
			{
#ifdef UNICODE
				LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szRoleName);
				lua_pushlstring(L, szTmp, strlen(szTmp));
#else
				lua_pushlstring(L, szRoleName, strlen(szRoleName));
#endif
				return 1;
			}
		}
		
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品绑定状态
//-------------------------------------------------------------------------------------
int SC_GetItemBindState(lua_State* L)
{
	Item* pItem = (Item*)(lua_touserdata(L, -1)); 
	if(P_VALID(pItem) && pItem->IsItem())
	{
		lua_pushboolean(L, pItem->IsItemBinded());
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品账号绑定状态
//-------------------------------------------------------------------------------------
int SC_GetItemAccountBindState(lua_State* L)
{
	Item* pItem = (Item*)(lua_touserdata(L, -1)); 
	if(P_VALID(pItem) && pItem->IsItem())
	{
		lua_pushboolean(L, pItem->IsItemAccountBinded());
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取装备飞升状态
//-------------------------------------------------------------------------------------
int SC_IsEquipSoaring(lua_State* L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);	
	const tagItemProto* pProto = pEquip->GetProto();

	if(P_VALID(pEquip))
	{
		lua_pushboolean(L, pEquip->IsSoaring());
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取物品锁定状态
//-------------------------------------------------------------------------------------
int SC_GetItemLockState(lua_State* L)
{
	Item* pItem = (Item*)(lua_touserdata(L, -1)); 
	if(P_VALID(pItem) && pItem->IsItem())
	{
		lua_pushboolean(L, pItem->IsItemLocked());
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获得锁定字符串
//-------------------------------------------------------------------------------------
int SC_GetItemLockString(lua_State* L)
{
	Item* pItem = (Item*)(lua_touserdata(L, -1));
	if( P_VALID(pItem) && pItem->IsItem() )
	{
		if( pItem->IsItemLocked() && pItem->GetData()->dwUnlockTime != 0 )
		{
			CHAR szTime[X_LONG_NAME] = {0};
			const tagDWORDTime dwTime = pItem->GetData()->dwUnlockTime;
			DwordTime2DataTime( szTime,X_LONG_NAME, dwTime );
			lua_pushlstring( L, szTime, strlen(szTime) );
			return 1;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品性别限制
//-------------------------------------------------------------------------------------
int SC_GetItemSexLimit(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->eSexLimit);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 物品等级限制
//-------------------------------------------------------------------------------------
int SC_GetItemLevelLimit(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->byMinUseLevel);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 装备等级限制
//-------------------------------------------------------------------------------------
int SC_GetEquipLevelLimit(lua_State* L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);	
	const tagItemProto* pProto = pEquip->GetProto();
	if(P_VALID(pProto))
	{
		if(pProto->eType != EIT_FaBao)
		{
			lua_pushnumber(L, pProto->byMinUseLevel);
			return 1;
		}
		else
		{
			const tagFabaoSpec *pTrumpSpe = pEquip->GetFaBaoSpe();
			if(P_VALID(pTrumpSpe))
			{
				INT nMax = max(pProto->byMinUseLevel, pTrumpSpe->nUseLevel);
				lua_pushnumber(L, nMax);
				return 1;
			}
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 装备神格限制
//-------------------------------------------------------------------------------------
int SC_GetEquipGodHeadLimit(lua_State* L)
{
	tagEquipProto* pEquipProto = (tagEquipProto*) lua_touserdata(L, -1);
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -2);
	if (P_VALID(pEquipProto))
	{
		if(pEquipProto->eType != EIT_FaBao && P_VALID(pEquipProto->dwCondenseLimit))
		{
			lua_pushnumber(L, pEquipProto->dwCondenseLimit);
			return 1;
		}
		else
		{
			lua_pushnumber(L, 0);
			return 1;
		}
	}
	else if (P_VALID(pEquip))
	{
		const tagItemProto* pProto = pEquip->GetProto();
		if(P_VALID(pProto))
		{
			if(pProto->eType != EIT_FaBao && P_VALID(static_cast<const tagEquipProto*>(pProto)->dwCondenseLimit))
			{
				lua_pushnumber(L, static_cast<const tagEquipProto*>(pProto)->dwCondenseLimit);
				return 1;
			}
			else
			{
				lua_pushnumber(L, 0);
				return 1;
			}
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
//物品是否消耗信仰
//-------------------------------------------------------------------------------------
int SC_IsCostBelief(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(pProto->eSpecFunc != EISF_GlobalLimitedItem)
		return 0;
	const tagGodAreaName* pName = GodMgr::Inst()->GetAreaDataByID(pProto->nSpecFuncVal2);
	if(P_VALID(pName))
	{
		lua_pushnumber(L, pName->nFaithValue);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 物品氏族声望限制
//-------------------------------------------------------------------------------------
int SC_GetItemClanReputeLimit(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->eClanRepute);
		lua_pushnumber(L, pProto->nClanReputeVal);
		return 2;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 物品其他声望限制
//-------------------------------------------------------------------------------------
int SC_GetItemOtherClanReputeLimit(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->eOtherClanRepute);
		lua_pushnumber(L, pProto->nOtherClanReputeVal);
		return 2;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 物品恢复时间
//-------------------------------------------------------------------------------------
int SC_GetItemCoolDownTime(lua_State* L)
{
    const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
    if (P_VALID(pProto))
    {
        int nCdTime = pProto->dwCooldownTime;
        lua_pushnumber(L, nCdTime);
        return 1;
    }
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品最大使用次数
//-------------------------------------------------------------------------------------
int SC_GetItemMaxUseTimes(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->nMaxUseTimes);
		return 1;
	} 
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取物品当前使用次数
//-------------------------------------------------------------------------------------
int SC_GetItemUseTimes(lua_State* L)
{
	Item* pItem = (Item*)(lua_touserdata(L, -1)); 
	if(P_VALID(pItem) && pItem->IsItem())
	{
		lua_pushnumber(L, pItem->GetItemUseTimes());
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取物品制造者
//-------------------------------------------------------------------------------------
int SC_GetItemCreator(lua_State* L)
{
	Item* pItem = (Item*)(lua_touserdata(L, -1));
	if(P_VALID(pItem) && pItem->IsItem())
	{
		DWORD dwCreatorID = pItem->GetItemCreatorID();
		if(GT_VALID(dwCreatorID))
		{
			LPCTSTR szRoleName = PlayerNameTab::Inst()->FindNameByID(dwCreatorID);
			if(szRoleName != _T(""))
			{
#ifdef UNICODE
				LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szRoleName);
				lua_pushlstring(L, szTmp, strlen(szTmp));
#else
				lua_pushlstring(L, szRoleName, strlen(szRoleName));
#endif
				return 1;
			}
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 判断装备是否被署名
//-------------------------------------------------------------------------------------
int SC_IsEquipSignatured(lua_State* L)
{
	Item* pItem = (Item*)( lua_touserdata(L, -1) );
	if( !P_VALID(pItem) || !MIsEquipment(pItem->GetItemTypeID()) )
		return 0;

	Equipment* pEquip = (Equipment*)pItem;
	if( !P_VALID(pEquip) )
		return 0;

	if( pEquip->IsSignature() )
	{
		lua_pushboolean( L, 1 );
		return 1;
	}

	return 0;
}


//-------------------------------------------------------------------------------------
// 获取装备署名者的名字
//-------------------------------------------------------------------------------------
int SC_GetEquipSignaturePlayer(lua_State* L)
{
	Item* pItem = (Item*)( lua_touserdata(L, -1) );
	if( !P_VALID(pItem) || !MIsEquipment(pItem->GetItemTypeID()) )
		return 0;
	
	Equipment* pEquip = (Equipment*)pItem;
	if( !pEquip->IsSignature() )
		return 0;

	DWORD dwPlayerID = pEquip->GetSignatruePlayerID();
	if( !GT_VALID(dwPlayerID) )
		return 0;

	TCHAR szRoleName[X_SHORT_NAME] = {0};
	
	LPCTSTR szName = PlayerNameTab::Inst()->FindNameByID( dwPlayerID );
	if( _tcscmp(szName,_T("") ) == 0 )
		_tcscpy( szRoleName, g_StrTable[_T("EquipSignature_PlayerIsDeleted")] );
	else
		_tcscpy( szRoleName, szName );

#ifdef UNICODE
	LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szRoleName);
	lua_pushlstring(L, szTmp, strlen(szTmp));
#else
	lua_pushlstring(L, szRoleName, strlen(szRoleName));
#endif

	return 1;
}

//-------------------------------------------------------------------------------------
// 获取装备署名
//-------------------------------------------------------------------------------------
int SC_GetEquipSignature(lua_State* L)
{
	Item* pItem = (Item*)( lua_touserdata(L, -1) );
	if( !P_VALID(pItem) || !MIsEquipment(pItem->GetItemTypeID()) )
		return 0;

	Equipment* pEquip = (Equipment*)pItem;
	if( !pEquip->IsSignature() )
		return 0;

	LPCTSTR szSignature = pEquip->GetSignatrue();
	if( P_VALID(szSignature) )
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8( szSignature );
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, szSignature, strlen(szSignature));
#endif
		return 1;
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品使用类型
//-------------------------------------------------------------------------------------
INT SC_GetItemUseType(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		INT nType = 0;
		if (GT_VALID(pProto->dwBuffID0))
		{
			nType = 1;
		}
		else
		{
			if(EISF_Null != pProto->eSpecFunc)
			{
				if(EISF_AcceptQuest == pProto->eSpecFunc ||			// 接取任务
					EISF_LearnSkill == pProto->eSpecFunc ||			// 学习技能	
					EISF_FriendGift == pProto->eSpecFunc)			// 好友礼物
				{
					nType = 1;
				}
				else if(EISF_IdetifyEquip == pProto->eSpecFunc ||	// 鉴定装备
					EISF_HoleGem == pProto->eSpecFunc ||			// 镶嵌
					EISF_Brand == pProto->eSpecFunc ||				// 烙印
					EISF_LongHunShi == pProto->eSpecFunc ||			// 龙附
					EISF_Grind == pProto->eSpecFunc ||				// 磨石
					EISF_Chisel == pProto->eSpecFunc ||				// 开凿
					EISF_RockGod == pProto->eSpecFunc||			//神石
					EISF_NewChisel == pProto->eSpecFunc)			//新凿石	
				{
					nType = 2;
				}
				else if(EISF_Dye == pProto->eSpecFunc)				// 染料
				{
					nType = 3;
				}
				else if(EISF_ProtectSign == pProto->eSpecFunc)		// 保底符
				{
					nType = 4;
				}
				else if(EISF_ColorProbability == pProto->eSpecFunc)	// 颜色提升
				{
					nType = 5;
				}
			}
		}
		lua_pushnumber(L, nType);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品消耗潜力值
//-------------------------------------------------------------------------------------
INT SC_GetItemCostPotVal(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		if(EISF_HoleGem == pProto->eSpecFunc ||
			EISF_Brand == pProto->eSpecFunc ||
			EISF_LongHunShi == pProto->eSpecFunc)
		{
			const tagConsolidateItem* pItem = ConsolidateData::Inst()->FindConsolidateItemProto(pProto->dwTypeID);
			if(P_VALID(pItem))
			{
				lua_pushnumber(L, pItem->dwPotValConsume);
				return 1;
			}
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 显示商品使用的部位
//-------------------------------------------------------------------------------------
INT SC_ShowGoodsUsePart(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		if(EISF_HoleGem == pProto->eSpecFunc)
		{
			const tagConsolidateItem* pItem = ConsolidateData::Inst()->FindConsolidateItemProto(pProto->dwTypeID);
			if(P_VALID(pItem))
			{
				lua_pushboolean(L, TRUE);
				return 1;
			}
		}
	}
	return 0;
}


//-------------------------------------------------------------------------------------
// 显示商品使用的等级
//-------------------------------------------------------------------------------------
INT SC_ShowGoodsUseLevel(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		if(EISF_HoleGem == pProto->eSpecFunc )
		{
			const tagConsolidateItem* pItem = ConsolidateData::Inst()->FindConsolidateItemProto(pProto->dwTypeID);
			if(P_VALID(pItem))
			{
				lua_pushnumber(L, pProto->nSpecFuncVal1 );
				return 1;
			}
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 强化部位显示
//-------------------------------------------------------------------------------------
INT SC_ShowStrengthenPart(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		if(EISF_HoleGem == pProto->eSpecFunc ||
			EISF_Brand == pProto->eSpecFunc )
		{
			lua_pushboolean(L, TRUE);
			return 1;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 强化等级
//-------------------------------------------------------------------------------------
INT SC_StrengthenLevel(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		if(EISF_HoleGem == pProto->eSpecFunc ||
			EISF_Brand == pProto->eSpecFunc )
		{
			lua_pushnumber(L, pProto->nSpecFuncVal1 );
			return 1;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品可镶部位
//-------------------------------------------------------------------------------------
INT SC_GetItemHoldEquipPos(lua_State* L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -2));
	INT					nIndex =		   (INT)(lua_tonumber(L, -1));	
	if(P_VALID(pProto))
	{
		const tagConsolidateItem* pItem = ConsolidateData::Inst()->FindConsolidateItemProto(pProto->dwTypeID);
		if(P_VALID(pItem))
		{
			lua_pushnumber(L, pItem->ConsolidatePos[nIndex].byConsolidate);
			return 1;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品总使用时限
//-------------------------------------------------------------------------------------
INT SC_GetItemAllTimeLimit(lua_State* L)
{
    Item*				pItem  = (Item*)(lua_touserdata(L, -1));
    if(P_VALID(pItem) && pItem->IsItem())
    {
        int timeLimit = (int)pItem->GetProto()->dwTimeLimit;
        if (timeLimit > 0)
        {
            lua_pushnumber(L, timeLimit);
            return 1;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品总使用时限
//-------------------------------------------------------------------------------------
INT SC_GetItemProtoAllTimeLimit(lua_State* L)
{
    tagItemProto*   pItemProto  = (tagItemProto*)(lua_touserdata(L, -1));
    if(P_VALID(pItemProto))
    {
        int timeLimit = (int)pItemProto->dwTimeLimit;
        if (timeLimit > 0)
        {
            lua_pushnumber(L, timeLimit);
            return 1;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品使用时限
//-------------------------------------------------------------------------------------
INT SC_GetItemTimeLimit(lua_State* L)
{
	Item*				pItem  = (Item*)(lua_touserdata(L, -1));
	
	if(P_VALID(pItem) && pItem->IsItem())
	{
		const tagItemProto* pItemProto = pItem->GetProto();
		if(!P_VALID(pItemProto))
			return 0;

		if (pItemProto->eSpecFunc == EISF_ZhanDouFu && pItemProto->nSpecFuncVal1 == EAFSFV2_OMG)
		{
			//INT nTimeLimit = (INT)pItem->GetData()->dwSpecVal1 / TICK_PER_SECOND;
			INT nTimeLimit = ItemTimerMgr::Inst()->GetItemRemainTime(pItem->GetItemId());
			if(nTimeLimit > 0)
			{
				lua_pushnumber(L, nTimeLimit);
				return 1;
			}
			else if (0 == nTimeLimit)
			{
				nTimeLimit = (INT)pItem->GetProto()->dwTimeLimit;
				lua_pushnumber(L, nTimeLimit);
				return 1;
			}
		}
		else
		{
			tagDWORDTime times = pItem->GetData()->dw1stGainTime;
			if (	times.day == 0 && times.hour == 0 && times.min == 0 
				&&  times.month == 0 && times.sec == 0 && times.year == 0)
			{
				INT nTimeLimit = (INT)pItem->GetProto()->dwTimeLimit;
				lua_pushnumber(L, nTimeLimit);
				return 1;
			}

			INT nPostSecord = (INT)CalcTimeDiff(ServerTime::Inst()->CalCurrentServerDwordTime(), pItem->GetData()->dw1stGainTime);
			if(nPostSecord > 0)
			{
				INT nTimeLimit = (INT)pItem->GetProto()->dwTimeLimit - nPostSecord;
				if(nTimeLimit > 0)
				{
					lua_pushnumber(L, nTimeLimit);
					return 1;
				}
			}
		}
	}
	
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取物品可学技能（技能书）
//-------------------------------------------------------------------------------------
int SC_GetItemLearnSkill(lua_State* L)
{
	const tagItemProto* pProto = (const tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		if(EISF_LearnSkill == pProto->eSpecFunc)
		{
			const tagSkillProtoClient* pSkill = SkillProtoData::Inst()->FindSkillProto(pProto->nSpecFuncVal1);
			if(P_VALID(pSkill))
			{
				lua_pushlightuserdata(L, (void*)pSkill);
				return 1;	
			}
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品是否可出售
//-------------------------------------------------------------------------------------
INT SC_GetItemCanSell(lua_State* L)
{
    tagItemProto*   pItemProto  = (tagItemProto*)(lua_touserdata(L, -1));
    if(P_VALID(pItemProto))
    {
        int bCanSell = (int)pItemProto->bCanSell;
        lua_pushnumber(L, bCanSell);
        return 1;
    }

    return 0;
}

//-------------------------------------------------------------------------------------
// 物品是否在百宝袋内
//-------------------------------------------------------------------------------------
INT SC_IsItemInAmazingBag(lua_State* L)
{
	Item* pItem = (Item*)(lua_touserdata(L, -1)); 
	if(P_VALID(pItem) )
	{
		lua_pushboolean(L, pItem->GetData()->eConType == EICT_Baibao);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品起手时间
//-------------------------------------------------------------------------------------
INT SC_GetItemPreTime(lua_State* L)
{
    tagItemProto*   pItemProto  = (tagItemProto*)(lua_touserdata(L, -1));
    if(P_VALID(pItemProto))
    {
        int timeLimit = (int)pItemProto->nPrepareTime;
        if (timeLimit > 0)
        {
            lua_pushnumber(L, timeLimit);
            return 1;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------
// 获取物品可堆叠数量
//-------------------------------------------------------------------------------------
INT SC_GetItemMaxItem(lua_State* L)
{
    tagItemProto*   pItemProto  = (tagItemProto*)(lua_touserdata(L, -1));
    if(P_VALID(pItemProto))
    {
        int timeLimit = (int)pItemProto->n16MaxLapNum;
        if (timeLimit > 0)
        {
            lua_pushnumber(L, timeLimit);
            return 1;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------
//  领域技能书技能描述显示
//-------------------------------------------------------------------------------------
int SC_GetAreaSkillBookDesc(lua_State* L)
{
	tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(!P_VALID(pProto))
		return 0;
	if(pProto->eSpecFunc != EISF_GlobalLimitedItem)
		return 0;
	const tagGodAreaName* pName = GodMgr::Inst()->GetAreaDataByID(pProto->nSpecFuncVal2);
	if(P_VALID(pName))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pName->szDesc);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pName->szDesc, strlen(pName->szDesc));
#endif
		return 1;
	}
	return 0;

}



//-------------------------------------------------------------------------------------
//							            装备信息相关函数
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// 获取装备特性描述
//-------------------------------------------------------------------------------------
int SC_GetEquipDescEx(lua_State *L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->pDisplayInfo->szDescEx);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->pDisplayInfo->szDescEx, strlen(pProto->pDisplayInfo->szDescEx));
#endif
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备职业限制
//-------------------------------------------------------------------------------------
int SC_GetEquipClassLimit(lua_State* L)
{
	const tagEquipProto* pProto = (tagEquipProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{

	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备位置
//-------------------------------------------------------------------------------------
int SC_GetEquipPos(lua_State* L)
{
	const tagEquipProto* pProto = (tagEquipProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->eEquipPos);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取武器伤害
//-------------------------------------------------------------------------------------
int SC_GetEquipWeaponDamage(lua_State* L)
{
	Equipment*			 pEquip = (Equipment*)(lua_touserdata(L, -2));
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquipMinDmg());
		lua_pushnumber(L, pEquip->GetEquipMaxDmg());
		return 2;
	}
	else if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->n16MinDmg);
		lua_pushnumber(L, pProto->n16MaxDmg);

		return 2;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取武器武魂值
//-------------------------------------------------------------------------------------
int SC_GetEquipWeaponWuhun(lua_State* L)
{
	Equipment*			 pEquip = (Equipment*)(lua_touserdata(L, -2));
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquipWuhun());
		return 1;
	}
	else if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->n16WuHun);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取防具护甲值
//-------------------------------------------------------------------------------------
int SC_GetEquipArmorDefine(lua_State* L)
{
	Equipment*			 pEquip = (Equipment*)(lua_touserdata(L, -2));
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquipArmor());
		return 1;
	}
	else if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->n16Armor);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取武器暂新度
//-------------------------------------------------------------------------------------
int SC_GetEquipNewNess(lua_State* L)
{
	Equipment*			 pEquip = (Equipment*)(lua_touserdata(L, -2));
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquipNewness());
		return 1;
	}
	else if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->n16Newness);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备类型
//-------------------------------------------------------------------------------------
int SC_GetEquipType(lua_State* L)
{
    Equipment*			 pEquip = (Equipment*)(lua_touserdata(L, -2));
    const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -1));

    if(P_VALID(pProto))
    {
        lua_pushnumber(L, pProto->eType);
        return 1;
    }
    return 0;
}
//-------------------------------------------------------------------------------------
// 是否可吸魂
//-------------------------------------------------------------------------------------
int SC_IsCanXiHun(lua_State* L)
{
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -1));

	if(P_VALID(pProto))
	{
		lua_pushboolean(L, pProto->bSuckLimit);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备潜力值
//-------------------------------------------------------------------------------------
int SC_GetEquipPotVal(lua_State* L)
{
	Equipment*			 pEquip = (Equipment*)(lua_touserdata(L, -2));
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		if( pEquip->GetProto()->eType != EIT_Fashion )
		{
			lua_pushnumber(L, pEquip->GetEquipPotVal());
            lua_pushnumber(L, pEquip->GetEquipCurrPotMax());
			lua_pushnumber(L, pEquip->GetEquipPotMax());
			return 3;
		}
		else
		{
			lua_pushnumber(L, GT_INVALID );
			lua_pushnumber(L, GT_INVALID );
            lua_pushnumber(L, GT_INVALID );
			return 3;
		}
	}
	else if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->nPotVal);
		lua_pushnumber(L, pProto->nMaxPotVal);
		return 2;
	}
	return 0;
}
int SC_GetShopEquipPotVal(lua_State* L)
{
	Equipment*			 pEquip = (Equipment*)(lua_touserdata(L, -2));
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		if( pEquip->GetProto()->eType == EIT_FaBao )
		{
			lua_pushnumber(L, pEquip->GetEquipPotVal());
			lua_pushnumber(L, pEquip->GetEquipPotMax());
			lua_pushnumber(L, 12);
			return 3;
		}
		else if( pEquip->GetProto()->eType != EIT_Fashion )
		{
			lua_pushnumber(L, pEquip->GetEquipPotVal());
			lua_pushnumber(L, pEquip->GetEquipCurrPotMax());
			lua_pushnumber(L, pEquip->GetEquipPotMax());
			return 3;
		}
		else
		{
			lua_pushnumber(L, GT_INVALID );
			lua_pushnumber(L, GT_INVALID );
			lua_pushnumber(L, GT_INVALID );
			return 3;
		}
	}
	else if(P_VALID(pProto))
	{
		if( pProto->eType == EIT_FaBao )
		{
			lua_pushnumber(L, pProto->nPotVal);
			lua_pushnumber(L, pProto->nMaxPotVal);
			lua_pushnumber(L, 12);
			return 3;
		}
		else
		{
			lua_pushnumber(L, pProto->nPotVal);
			lua_pushnumber(L, pProto->nMaxPotVal);
		}
		return 2;
	}
	return 0;
};

//-------------------------------------------------------------------------------------
// 获取装备潜力可提升的次数
//-------------------------------------------------------------------------------------
int SC_GetEquipPotAdvanceTime(lua_State* L)
{
	Equipment*			 pEquip = (Equipment*)(lua_touserdata(L, -2));
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber( L, pEquip->GetEquipPotAdvanceTime() );
		return 1;
	}
	else if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->nPotIncTimes);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取装备属性原始加成
//-------------------------------------------------------------------------------------
int SC_GetEquipProtoAtt(lua_State* L)
{
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -1));
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -2));
	if(P_VALID(pProto) && P_VALID(pEquip) )
	{
		INT count = MAX_ROLEATT_BASE_EFFECT;
		const tagEquipSpec * pSpec = pEquip->GetEquip();
		if( !P_VALID(pSpec) )
			return 0;
		if( pSpec->byQuality < EIQ_End && pSpec->byQuality > EIQ_Null )
			count = pSpec->nRoleL2AttCount;
		if( count <= 0 )
			return 0;

		lua_newtable(L);
		for(INT i=0; i< count; ++i)	
		{
			lua_pushnumber(L, pProto->BaseEffect[i].eRoleAtt);
			lua_pushnumber(L, pProto->BaseEffect[i].nValue);
			lua_settable(L, -3);
		}	
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 熔铸装备属性加成
//-------------------------------------------------------------------------------------
int SC_GetCastingAttribute(lua_State* L)
{
	Equipment*			 pEquip = (Equipment*)(lua_touserdata(L, -1));
	const tagItemProto* pProto = pEquip->GetProto();
	if(P_VALID(pEquip) && pEquip->IsItem() && P_VALID(pProto))
	{
		TMap<ERoleAttribute, INT> Map;
		/*pEquip->GetEquipAttributePct(Map);*/
		int castingTimes = pEquip->GetEquipSoaringLevel();
		
		map<DWORD,tagSoarCastingProto> castingMap;
		castingMap = SoarMgr::Inst()->GetSoarCastingDataMap();
		for (map<DWORD,tagSoarCastingProto>::iterator iter = castingMap.begin();
			iter != castingMap.end();iter++)
		{
			if(iter->second.eType == ESCT_Weapon &&
				pProto->eType != EIT_Weapon)
				continue;

			if(iter->second.eType == ESCT_Decoration &&
				pProto->eType != EIT_Decoration)
				continue;

			if(iter->second.eType == ESCT_Defence &&
				(pProto->eType != EIT_ClothArmor && pProto->eType != EIT_Armor))
				continue;

			if(iter->second.byLevel <= castingTimes)
			{
				for (int i = 0; i< MAX_CASTING_ROLEATT;i++)
				{
					if(iter->second.CastingRoleAtt[i].eRoleAtt != -1 )
					{
						Map.ModifyValue(iter->second.CastingRoleAtt[i].eRoleAtt,iter->second.CastingRoleAtt[i].nAttVal);
					}
				}
			}
		}

		if(Map.Size() > 0)
		{
			lua_newtable(L);

			Map.ResetIterator();
			ERoleAttribute eAtt;
			INT nValue;
			while(Map.PeekNext(eAtt, nValue))
			{
				lua_pushnumber(L, eAtt);
				lua_pushnumber(L, nValue);
				lua_settable(L, -3);
			}	
			return 1;
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备属性加成
//-------------------------------------------------------------------------------------
int SC_GetEquipAttribute(lua_State* L)
{
	Equipment*			 pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		TMap<ERoleAttribute, INT> Map;
		pEquip->GetEquipAttribute(Map);
		if(Map.Size() > 0)
		{
			lua_newtable(L);

			Map.ResetIterator();
			ERoleAttribute eAtt;
			INT nValue;
			while(Map.PeekNext(eAtt, nValue))
			{
				lua_pushnumber(L, eAtt);
				lua_pushnumber(L, nValue);
				lua_settable(L, -3);
			}	
			return 1;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取装备属性百分比加成
//-------------------------------------------------------------------------------------
int SC_GetEquipAttributePct(lua_State* L)
{
	Equipment*			 pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		TMap<ERoleAttribute, INT> Map;
		pEquip->GetEquipAttributePct(Map);
		if(Map.Size() > 0)
		{
			lua_newtable(L);

			Map.ResetIterator();
			ERoleAttribute eAtt;
			INT nValue;
			while(Map.PeekNext(eAtt, nValue))
			{
				lua_pushnumber(L, eAtt);
				lua_pushnumber(L, nValue);
				lua_settable(L, -3);
			}	
			return 1;
		}
	}
	return 0;
}


//-------------------------------------------------------------------------------------
// 获取镌刻、铭文的生产状态
//-------------------------------------------------------------------------------------
int SC_GetEquipProduceState(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquipEngraveTimes());
		lua_pushnumber(L, pEquip->GetEquipPosyTimes());
		lua_pushnumber(L, pEquip->GetEquipSoaringLevel());
		return 3;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取烙印的生产状态
//-------------------------------------------------------------------------------------
int SC_GetEquipBrandLevel(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquipBrandLevel());
		return 1;
	}
	return 0;
}


//-------------------------------------------------------------------------------------
// 获取装备强化次数
//-------------------------------------------------------------------------------------
int SC_GetEquipStrengLevel(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquipStrengthLevel());
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取神铸等级
//-------------------------------------------------------------------------------------
int SC_Getequip_soar_level (lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquipGodStrengthenLevel());
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 装备是否可被神铸
//-------------------------------------------------------------------------------------
int SC_Getsoar_limit(lua_State* L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);	

	if(P_VALID(pEquip))
	{
		lua_pushboolean(L, pEquip->IfEquipCanGodStrengthen());
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取装备宝石镶嵌数量
//-------------------------------------------------------------------------------------
int SC_GetEquipHoldNum(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquipHoleNum());
		lua_pushnumber(L, pEquip->GetEquipMaxHoleNum());
		return 2;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备已镶嵌宝石
//-------------------------------------------------------------------------------------
int SC_GetEquipHoldGem(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -2));
	INT		   nIndex = (INT)		(lua_tonumber(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		if(nIndex >= 0 && nIndex < MAX_EQUIPHOLE_NUM)
		{
			DWORD dwGemID = pEquip->GetEquipHoldGemID(nIndex);
			const tagItemProto* pGem = ItemProtoData::Inst()->FindItemProto(dwGemID);
			if(P_VALID(pGem))
			{
#ifdef UNICODE
				LPCSTR szTmp1 = TObjRef<Util>()->UnicodeToUnicode8(pGem->pDisplayInfo->szName);
				lua_pushlstring(L, szTmp1, strlen(szTmp1));
#else
				lua_pushlstring(L, pGem->pDisplayInfo->szName, strlen(pGem->pDisplayInfo->szName));
#endif
#ifdef UNICODE
				LPCSTR szTmp2 = TObjRef<Util>()->UnicodeToUnicode8(pGem->pDisplayInfo->szDesc);
				lua_pushlstring(L, szTmp2, strlen(szTmp2));
#else
				lua_pushlstring(L, pGem->pDisplayInfo->szDesc, strlen(pGem->pDisplayInfo->szDesc));
#endif
				lua_pushnumber(L, pGem->byQuality);
				return 3;
			}
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备龙附等级
//-------------------------------------------------------------------------------------
int SC_GetEquipLongfu(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -2));
	EWuXing	   eType  = (EWuXing)(DWORD)(lua_tonumber(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		if(eType > EWX_Null && eType < EWX_End)
		{
			lua_pushnumber(L, pEquip->GetEquipWuxing(eType));
			return 1;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取装备龙魂能力（名称+描述）
//-------------------------------------------------------------------------------------
int SC_GetEquipLonghun(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -2));
	bool bInner = (lua_toboolean(L, -1) == 0 ? false : true);
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		DWORD dwID;
		if(bInner)
			dwID = pEquip->GetEquipInnerLonghun();
		else 
			dwID = pEquip->GetEquipOuterLonghun();

		if(P_VALID(dwID))
		{
			const tagLongHunProtoClient* pProto = LonghunAttData::Inst()->FindLonghunData(dwID);
			if(P_VALID(pProto))
			{
				lua_pushlightuserdata(L, (void*)pProto);
#ifdef UNICODE				
				LPCSTR szTmp1 = TObjRef<Util>()->UnicodeToUnicode8(pProto->szName);
				lua_pushlstring(L, szTmp1, strlen(szTmp1));
#else
				lua_pushlstring(L, pProto->szName, strlen(pProto->szName));
#endif
#ifdef UNICODE					
				LPCSTR szTmp2 = TObjRef<Util>()->UnicodeToUnicode8(pProto->szDesc);
				lua_pushlstring(L, szTmp2, strlen(szTmp2));
#else
				lua_pushlstring(L, pProto->szDesc, strlen(pProto->szDesc));
#endif
				return 3;
			}
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备龙魂能力激活条件
//-------------------------------------------------------------------------------------
int SC_GetEquipLonghunActive(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -2));
	bool bInner = (lua_toboolean(L, -1) == 0 ? false : true);
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushboolean(L, ItemMgr::Inst()->GetLonghun((EEquipPos)pEquip->GetPos(), pEquip->GetItemId()));
		EEquipPos ePos = pEquip->GetEquipPos();
		
		if(bInner)
		{
            if(EICT_Equip != pEquip->GetConType())
            {
                if (EEP_Finger1 == pEquip->GetEquipPos())
                {
                    lua_pushnumber(L, g_sLongHunActivePos[EEP_Finger1].eInner[0]);
                    lua_pushnumber(L, g_sLongHunActivePos[EEP_Finger1].eInner[1]);
                    lua_pushnumber(L, g_sLongHunActivePos[EEP_Finger2].eInner[0]);
                    lua_pushnumber(L, g_sLongHunActivePos[EEP_Finger2].eInner[1]);
                    return 5;
                }
                else
                {
                    ePos = pEquip->GetEquipPos();
                    lua_pushnumber(L, g_sLongHunActivePos[ePos].eInner[0]);
                    lua_pushnumber(L, g_sLongHunActivePos[ePos].eInner[1]);
                    lua_pushnumber(L, -1);
                    lua_pushnumber(L, -1);
                    return 5;
                }
            }
            else
            {
                ePos = (EEquipPos)pEquip->GetPos();
                lua_pushnumber(L, g_sLongHunActivePos[ePos].eInner[0]);
                lua_pushnumber(L, g_sLongHunActivePos[ePos].eInner[1]);
                lua_pushnumber(L, -1);
                lua_pushnumber(L, -1);
                return 5;
            }
		}
		else
		{
            if(EICT_Equip != pEquip->GetConType())
            {
                if (EEP_Finger1 == pEquip->GetEquipPos())
                {
                    lua_pushnumber(L, g_sLongHunActivePos[EEP_Finger1].eOuter);
                    lua_pushnumber(L, g_sLongHunActivePos[EEP_Finger2].eOuter);
                    return 3;
                }
                else
                {
                    ePos = pEquip->GetEquipPos();
                    lua_pushnumber(L, g_sLongHunActivePos[ePos].eOuter);
                    lua_pushnumber(L, -1);
                    return 3;
                }

            }
            else
            {
                ePos = (EEquipPos)pEquip->GetPos();
                lua_pushnumber(L, g_sLongHunActivePos[ePos].eOuter);
                lua_pushnumber(L, -1);
                return 3;
            }
		}
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取装备龙魂能力五行属性
//-------------------------------------------------------------------------------------
int SC_GetEquipLonghunFive(lua_State* L)
{
	const tagLongHunProtoClient* pProto = (tagLongHunProtoClient*)(lua_touserdata(L, -2));
	INT nIndex = (INT)(lua_tonumber(L, -1));

	if(P_VALID(pProto) && nIndex >= 0 && nIndex < X_WUXING_TYPE_NUM)
	{
		lua_pushnumber(L, pProto->byWuXingActive[nIndex]);
		return 1;
	}
	return 0;
};

//-------------------------------------------------------------------------------------
// 获取装备所属套装名称
//-------------------------------------------------------------------------------------
int SC_GetEquipSuitName(lua_State* L)
{
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -3));
	BYTE				 byQual = (BYTE)(lua_tonumber(L, -2));		
	INT					 nIndex	= (INT)(lua_tonumber(L, -1));
	if(P_VALID(pProto) && nIndex >= 0 && nIndex < MAX_PEREQUIP_SUIT_NUM
		&& pProto->bySuitMinQlty[nIndex] <= byQual)
	{
		const tagSuitProtoClient* pSuit = ItemProtoData::Inst()->FindSuitProto(pProto->dwSuitID[nIndex]);
		if (P_VALID(pSuit))
		{
			lua_pushlightuserdata(L, (void*)pSuit);
#ifdef UNICODE
			LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pSuit->szSuitName);
			lua_pushlstring(L, szTmp, strlen(szTmp));
#else
			lua_pushlstring(L, pSuit->szSuitName, strlen(pSuit->szSuitName));
#endif
			INT nNum = ItemMgr::Inst()->GetSuitNum(pSuit->dwID);
			lua_pushnumber(L, GT_INVALID == nNum ? 0 : nNum);
			
			// 计算套装件数最大值
			INT nMax = 0;
			for(INT i=0; i<MAX_SUIT_ATT_NUM; ++i)
			{
				if(!P_VALID(pSuit->dwTriggerID[i]))
					break;

				nMax = pSuit->n8ActiveNum[i];
			}
			lua_pushnumber(L, nMax);

			return 4;
		}
		
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备当前套装属性
//-------------------------------------------------------------------------------------
int SC_GetEquipSuitAttribute(lua_State* L)
{
	const tagSuitProtoClient* pSuit	= (const tagSuitProtoClient*)(lua_touserdata(L, -2));
	INT	nIndex = (INT)(lua_tonumber(L, -1));
	if (P_VALID(pSuit) && nIndex >= 0 && nIndex < MAX_SUIT_ATT_NUM)
	{
		
		lua_pushnumber(L, pSuit->n8ActiveNum[nIndex]);
		const tagBuffProtoClient* pBuff = SkillProtoData::Inst()->FindBuffProto(pSuit->dwBuffID[nIndex]);
		if (P_VALID(pBuff))
		{
#ifdef UNICODE
			LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pBuff->szDesc);
			lua_pushlstring(L, szTmp, strlen(szTmp));
#else 
			lua_pushlstring(L, pBuff->szDesc, strlen(pBuff->szDesc));
#endif
			
			return 2;
		}
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取装备当前属性限制
//-------------------------------------------------------------------------------------
int SC_GetEquipAttributeLimit(lua_State* L)
{
	Equipment*			 pEquip = (Equipment*)			(lua_touserdata(L, -3));
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -2));
	INT					 nIndex	=  	   			   (INT)(lua_tonumber(L, -1));
	if (P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquipAttLimit((ERoleAttribute)nIndex));
		return 1;
	}
	else if (P_VALID(pProto) && nIndex >= ERA_AttA_Start && nIndex <= ERA_AttA_End)
	{
		lua_pushnumber(L, pProto->n16AttALimit[nIndex]);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取装备特殊属性
//-------------------------------------------------------------------------------------
int SC_GetEquipSpecAttribute(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquipSpecAttribute()+1);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备鉴定修正率
//-------------------------------------------------------------------------------------
int SC_GetEquipQltyModPct(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		lua_pushnumber(L, pEquip->GetEquip()->n16QltyModPct);
		lua_pushnumber(L, pEquip->GetEquip()->n16QltyModPctEx);
        lua_pushnumber(L, pEquip->GetEquip()->dwPurpleQltyIdfPct);
		return 3;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备的颜色（主要是时装）
//-------------------------------------------------------------------------------------
int SC_GetEquipColor(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip) && pEquip->IsItem())
	{
		if(pEquip->GetProto()->eType == EIT_Fashion && pEquip->IsCanDye())
		{
			lua_pushnumber(L, pEquip->GetEquip()->n8ColorID);
			return 1;
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备的精纺阶段（主要是时装）
//-------------------------------------------------------------------------------------
int SC_GetFashionSpinStep(lua_State* L)
{
    Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
    if(P_VALID(pEquip) && pEquip->IsItem())
    {
        if(pEquip->GetProto()->eType == EIT_Fashion)
        {
            lua_pushnumber(L, (int)ItemProtoData::Inst()->FindFashionSpinStep(pEquip->GetItemTypeID()));
            return 1;
        }
    }
    return 0;
}
//-------------------------------------------------------------------------------------
// 获取装备的套装属性（主要是时装）
//-------------------------------------------------------------------------------------
int SC_GetFashionSuiteType(lua_State* L)
{
    Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
    if(P_VALID(pEquip) && pEquip->IsItem())
    {
        if(pEquip->GetProto()->eType == EIT_Fashion)
        {
            lua_pushnumber(L, (int)ItemProtoData::Inst()->FindFashionSuitType(pEquip->GetItemTypeID()));
            return 1;
        }
    }
    return 0;
}

//-------------------------------------------------------------------------------------
// 获取装备的品级影响比率
//------------------------------------------------------------------------------------- 
int SC_GetEquipQltyEffect(lua_State* L)
{
	INT nQlty = (INT)(lua_tonumber(L, -1));
	const tagEquipQltyEffect* pAtt = ItemProtoData::Inst()->FindEquipQltyEffect(nQlty);
	if(P_VALID(pAtt))
	{
		lua_pushnumber(L, pAtt->fWeaponFactor);
		lua_pushnumber(L, pAtt->fArmorFactor);
		lua_pushnumber(L, pAtt->fPotFactor);
		return 3;
	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取装备的最大强化等级
//-------------------------------------------------------------------------------------
int SC_GetEquipMaxStrengthenLevel(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip))
	{
		INT nQlty = pEquip->GetItemQuality();
		const tagEquipQltyEffect* pAtt = ItemProtoData::Inst()->FindEquipQltyEffect(nQlty);
		if(P_VALID(pAtt))
		{
			lua_pushnumber(L, (pAtt->nMaxStrengthenLevel));
			return 1;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 妖精秘药药效
//-------------------------------------------------------------------------------------
int SC_GetFairyPower(lua_State *L)
{
	Item* pItem = (Item*)(lua_touserdata(L, -1));
	if(P_VALID(pItem) && pItem->IsItem())
	{
		INT nPower = 0;
		const tagItem *pData = pItem->GetData();
		const tagItemProto *pProto = pItem->GetProto();
		if( P_VALID(pData) && P_VALID(pProto) && pProto->eSpecFunc == EISF_FairyPowder)
		{
			nPower = pProto->nSpecFuncVal1;
			if(nPower == GT_INVALID || nPower == 0 )
				nPower = pData->dwSpecVal1;
		}
		lua_pushnumber(L, nPower);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 妖精秘药药效
//-------------------------------------------------------------------------------------
int SC_GetProtoFairyPower(lua_State *L)
{
	const tagItemProto* pProto = (tagItemProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		INT nPower = 0;
		if( P_VALID(pProto) && pProto->eSpecFunc == EISF_FairyPowder)
			nPower = pProto->nSpecFuncVal1;
		lua_pushnumber(L, nPower);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取装备鉴定几率
//-------------------------------------------------------------------------------------
int SC_GetEquipQltyPct(lua_State* L)
{
	const tagEquipProto* pProto = (const tagEquipProto*)(lua_touserdata(L, -1));
	if(pProto->eType == EIT_FaBao) return 0;
	const tagEquipQltyPct* pAtt = ItemProtoData::Inst()->FindEquipQltyPctByItemID(pProto->dwTypeID);
	if(P_VALID(pAtt))
	{
		int i=0;
		lua_pushnumber(L, pAtt->nEquipQltyPct[i++]);
		lua_pushnumber(L, pAtt->nEquipQltyPct[i++]);
		lua_pushnumber(L, pAtt->nEquipQltyPct[i++]);
		lua_pushnumber(L, pAtt->nEquipQltyPct[i++]);
		lua_pushnumber(L, pAtt->nEquipQltyPct[i++]);
		lua_pushnumber(L, pAtt->nEquipQltyPct[i++]);
		return 6;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
//										技能信息相关函数
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// 获取技能名称
//-------------------------------------------------------------------------------------
int SC_GetSkillName(lua_State *L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szName);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->szName, strlen(pProto->szName));
#endif
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取领域名称
//-------------------------------------------------------------------------------------
int SC_GetAreaName(lua_State *L)
{
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szName);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->szName, strlen(pProto->szName));
#endif
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 经脉名字
//-------------------------------------------------------------------------------------
int SC_GetPulseName(lua_State *L)
{
	const tagPulseClientProto* pProto = (tagPulseClientProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szName);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->szName, strlen(pProto->szName));
#endif
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取技能描述
//-------------------------------------------------------------------------------------
int SC_GetSkillDesc(lua_State *L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szDesc);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->szDesc, strlen(pProto->szDesc));
#endif
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取领域描述
//-------------------------------------------------------------------------------------
int SC_GetAreaDesc(lua_State *L)
{
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szDesc);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->szDesc, strlen(pProto->szDesc));
#endif
		return 1;

	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取经脉描述
//-------------------------------------------------------------------------------------
int SC_GetPulseDesc(lua_State *L)
{
	const tagPulseClientProto* pProto = (tagPulseClientProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szSkillDesc);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->szSkillDesc, strlen(pProto->szSkillDesc));
#endif
		return 1;

	}

	return 0;
}


//-------------------------------------------------------------------------------------
// 获取经魂晶资质
//-------------------------------------------------------------------------------------
int SC_GetHunJingSpec(lua_State *L)
{

	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	INT nRet = 0;
	const tagItemProto* pProto = pEquip->GetProto();
	if(!P_VALID(pProto) || pProto->eType != EIT_SoulCrystal || pEquip->IsNotIdetified()) return nRet;
	const tagSoulCrystalSpec* pSpec = pEquip->GetSoulCrystalSpec();
	lua_pushinteger(L, (lua_Integer)pSpec->bySoulCrystalQlty); // 资质
	nRet++;

	DWORD dwNeng = 0;
	for (int i = 0; i<SOUL_CRYSTAL_ATT_MAX_NUM ; i++ )
	{
		tagHunJingName* pHunJing =  ReputeMgr::Inst()->FindHunJingName( pSpec->nSoulAttID[i]);
		if(!P_VALID(pHunJing))	continue;

		dwNeng += pHunJing->nLevel;
	}

	lua_pushinteger(L, (lua_Integer)dwNeng); //能力强度
	nRet++;



	return nRet;
}
//-------------------------------------------------------------------------------------
// 获取经魂晶技能描述
//-------------------------------------------------------------------------------------
int SC_GetHunJingSkillDesc(lua_State *L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagSoulCrystalSpec* pSpec = pEquip->GetSoulCrystalSpec();
	if(!P_VALID(pSpec))
		return 0;

	const tagSkillProtoClient* pProto = SkillProtoData::Inst()->FindSkillProto(pSpec->dwStaticSkillID);
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szDesc);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->szDesc, strlen(pProto->szDesc));
#endif
		return 1;

	}

	return 0;
}
//-------------------------------------------------------------------------------------
// 获取经魂晶技能名称
//-------------------------------------------------------------------------------------
int SC_GetHunJingSkillName(lua_State *L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagSoulCrystalSpec* pSpec = pEquip->GetSoulCrystalSpec();
	if(!P_VALID(pSpec))
		return 0;

	const tagSkillProtoClient* pProto = SkillProtoData::Inst()->FindSkillProto(pSpec->dwStaticSkillID);
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szName);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->szName, strlen(pProto->szName));
#endif
		return 1;

	}

	return 0;
}
//-------------------------------------------------------------------------------------
// 获取经魂晶ATT描述
//-------------------------------------------------------------------------------------
int SC_GetHunJingAttDesc(lua_State *L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagSoulCrystalSpec* pSpec = pEquip->GetSoulCrystalSpec();
	if(!P_VALID(pSpec))
		return 0;

	tstringstream ss;
	DWORD dwNeng = 0;
	for (int i = 0; i<SOUL_CRYSTAL_ATT_MAX_NUM ; i++ )
	{
		tagHunJingName* pHunJing =  ReputeMgr::Inst()->FindHunJingName( pSpec->nSoulAttID[i]);
		if(!P_VALID(pHunJing))	continue;

		ss << _T("<color=0xfffff717>") << pHunJing->szName << _T(" Lv") << pHunJing->nLevel <<  _T("  <color=0xff00a2ff>")<< pHunJing->szDesc <<  _T("\\n");
		dwNeng++;
	}

	if(dwNeng)
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(ss.str().c_str());
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, ss.str().c_str(), strlen(ss.str().c_str()));
#endif
		return 1;

	}

	return 0;
}

//-------------------------------------------------------------------------------------
// 获取经脉NEXT描述
//-------------------------------------------------------------------------------------
int SC_GetPulseNextDesc(lua_State *L)
{
	const tagPulseClientProto* pProto = (tagPulseClientProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szNextSkillDesc);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->szNextSkillDesc, strlen(pProto->szNextSkillDesc));
#endif
		return 1;

	}

	return 0;
}


//-------------------------------------------------------------------------------------
// 获取领域获得方式
//-------------------------------------------------------------------------------------
int SC_GetAreaGetDesc(lua_State *L)
{
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	const tagSkillData* pData = (tagSkillData*)(lua_touserdata(L, -2));
	if(P_VALID(pProto) && EGAT_OPTIONAL == pProto->eType)
	{
		DWORD dwSkillID = pProto->dwID;
		DWORD dwNextSkillID = dwSkillID;
		//if (P_VALID(pData))
		dwNextSkillID = dwSkillID/100*100 + (dwSkillID%100+1);

		const tagGodAreaName* pProtoG = GodMgr::Inst()->GetAreaDataByID(dwNextSkillID);
		if (P_VALID(pProtoG))
		{
#ifdef UNICODE
			LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProtoG->szGetDesc);
			lua_pushlstring(L, szTmp, strlen(szTmp));
#else
			lua_pushlstring(L, pProtoG->szGetDesc, strlen(pProtoG->szGetDesc));
#endif
			return 1;
		}
		else
		{
#ifdef UNICODE
			LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(g_StrTable[_T("AreaTips_MaxLevel")]);
			lua_pushlstring(L, szTmp, strlen(szTmp));
#else
			lua_pushlstring(L, szTmp, strlen(szTmp));
#endif
			return 1;
		}
	}

	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能类型
//-------------------------------------------------------------------------------------
int SC_GetSkillType(lua_State *L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->nType2);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能职业限制
//-------------------------------------------------------------------------------------
int SC_GetSkillClassLimit(lua_State* L)
{
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能施展类型
//-------------------------------------------------------------------------------------
int SC_GetSkillUseType(lua_State* L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->eUseType);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取神魔技能的前置技能
//-------------------------------------------------------------------------------------
int SC_GetGESkillNeedSkillName(lua_State* L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		const tagForceSkill* pSkill = ReputeMgr::Inst()->GetForceSkillInfo(pProto->dwID);
		if (P_VALID(pSkill))
		{
			const tagSkillProtoClient* pPreSkillProto = SkillProtoData::Inst()->FindSkillProto(pSkill->dwNeedSkill);
			if (P_VALID(pPreSkillProto))
			{
#ifdef UNICODE
				LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pPreSkillProto->szName);
				lua_pushlstring(L, szTmp, strlen(szTmp));
#else
				lua_pushlstring(L, pPreSkillProto->szName, strlen(pPreSkillProto->szName));
#endif
			}	
			else 
			{
				lua_pushlstring(L, " ", 1);
			}
			return 1;
		}

	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取技能施展类型
//-------------------------------------------------------------------------------------
int SC_GetAreaUseType(lua_State* L)
{
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->eUseType);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能性别限制
//-------------------------------------------------------------------------------------
int SC_GetSkillSexLimit(lua_State* L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->eSexLimit);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能位置限制
//-------------------------------------------------------------------------------------
int SC_GetSkillPosLimit(lua_State* L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->ePosType);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能骑战限制
//-------------------------------------------------------------------------------------
int SC_GetSkillMountLimit(lua_State* L)
{
	//const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	//if(P_VALID(pProto))
	//{
	//	lua_pushboolean(L, pProto->bMountLimit);
	//	return 1;
	//}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能是否已学习
//-------------------------------------------------------------------------------------
int SC_GetSkillIsLearn(lua_State* L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushboolean(L, SkillMgr::Inst()->IsHaveSkillByTypeID(pProto->dwID));
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能升级方式
//-------------------------------------------------------------------------------------
int SC_GetSkillLevelupType(lua_State* L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->eLevelUpType);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能前置技能
//-------------------------------------------------------------------------------------
int SC_GetSkillPreSkill(lua_State* L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		  (lua_touserdata(L, 1));	
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, 2));
	INT n = lua_tointeger(L, 3);
	if(P_VALID(pSkill))
	{
		const tagSkillProtoClient* pNext = SkillProtoData::Inst()->FindSkillProto(pSkill->dwTypeID + 1);
		
		if(P_VALID(pNext))
		{
			DWORD preSkillID = pNext->dwPreLevelSkillID;
			if(preSkillID < 10000)
			{
				// 盒子，需要获取第n个技能				
				if(n < tagSkillBoxProto::E_SBP_Size)
				{
					const tagSkillBoxProto* pSkillBoxPro = SkillProtoData::Inst()->FindSkillBoxProto(preSkillID);
					preSkillID = pSkillBoxPro->dwSkillIDs[n];
				}
				else
					return 0;
			}else if(n>0) return 0;
			const tagSkillProtoClient* pPre = SkillProtoData::Inst()->FindSkillProto(preSkillID);
			if(P_VALID(pPre))
			{
#ifdef UNICODE
				LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pPre->szName);
				lua_pushlstring(L, szTmp, strlen(szTmp));
#else 
				lua_pushlstring(L, pPre->szName, strlen(pPre->szName));
#endif
				lua_pushnumber(L, pPre->nLevel);
				lua_pushboolean(L, SkillMgr::Inst()->IsHaveSkillByTypeID(pPre->dwID));
				return 3;
			}
		}
	}
	else if(P_VALID(pProto))
	{
		DWORD preSkillID = pProto->dwPreLevelSkillID;
		if(preSkillID < 10000)
		{
			// 盒子，需要获取第n个技能				
			if(n < tagSkillBoxProto::E_SBP_Size)
			{
				const tagSkillBoxProto* pSkillBoxPro = SkillProtoData::Inst()->FindSkillBoxProto(preSkillID);
				preSkillID = pSkillBoxPro->dwSkillIDs[n];
			}
			else
				return 0;
		}else if(n>0) return 0;
		const tagSkillProtoClient* pPre = SkillProtoData::Inst()->FindSkillProto(preSkillID);
		if(P_VALID(pPre))
		{
#ifdef UNICODE
			LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pPre->szName);
			lua_pushlstring(L, szTmp, strlen(szTmp));
#else
			lua_pushlstring(L, pPre->szName, strlen(pPre->szName));
#endif
			lua_pushnumber(L, pPre->nLevel);
			lua_pushboolean(L, SkillMgr::Inst()->IsHaveSkillByTypeID(pPre->dwID));
			return 3;
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能学习等级限制
//-------------------------------------------------------------------------------------
int SC_GetSkillLevelLimit(lua_State* L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		  (lua_touserdata(L, -2));
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pSkill))
	{
		const tagSkillProtoClient* pNext = SkillProtoData::Inst()->FindSkillProto(pSkill->dwTypeID + 1);
		if(P_VALID(pNext))
		{
			lua_pushnumber(L, pNext->nNeedRoleLevel);
			return 1;
		}
	}
	else if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->nNeedRoleLevel);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能学习天资限制
//-------------------------------------------------------------------------------------
int SC_GetSkillTalentLimit(lua_State* L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		  (lua_touserdata(L, -2));
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pSkill))
	{
		const tagSkillProtoClient* pNext = SkillProtoData::Inst()->FindSkillProto(pSkill->dwTypeID + 1);
		if(P_VALID(pNext))
		{
			lua_pushnumber(L, pNext->nNeedTalentPoint);
			lua_pushnumber(L, pNext->eTalentType);
			return 2;
		}
	}
	else if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->nNeedTalentPoint);
		lua_pushnumber(L, pProto->eTalentType);
		return 2;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能伤害类型
//-------------------------------------------------------------------------------------
int SC_GetSkillDamageType(lua_State* L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->eDmgType);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能等级
//-------------------------------------------------------------------------------------
int SC_GetSkillLevel(lua_State* L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->nLevel);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取领域技能等级
//-------------------------------------------------------------------------------------
int SC_GetAreaSkillLevel(lua_State* L)
{
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->nLevel);
		return 1;
	}
	else
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取经脉技能等级
//-------------------------------------------------------------------------------------
int SC_GetPulseLevel(lua_State* L)
{
	const tagPulseClientProto* pProto = (tagPulseClientProto*)(lua_touserdata(L, -1));
	const tagSkillData* pData = (tagSkillData*)(lua_touserdata(L, -2));
	if(P_VALID(pProto))
	{
		if (P_VALID(pData))
			lua_pushnumber(L, pData->nLevel);
		else
			lua_pushnumber(L, 0);

		lua_pushnumber(L, pProto->dwMaxLevel);
		return 2;
	}
	else
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取魂晶等级
//-------------------------------------------------------------------------------------
int SC_GetHunJingLevel(lua_State* L)
{

	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	const tagItemProto* pProto = pEquip->GetProto();
	if(!P_VALID(pProto) || pProto->eType != EIT_SoulCrystal || pEquip->IsNotIdetified()) return 0;

	const tagSoulCrystalSpec* pSpec = pEquip->GetSoulCrystalSpec();
	if(P_VALID(pSpec))
	{

		lua_pushnumber(L, pSpec->bySoulCrystalLvl);
		return 1;
	}
	else
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	return 0;

}
//-------------------------------------------------------------------------------------
// 获取经脉技能等级限制
//-------------------------------------------------------------------------------------
int SC_GetPulseLevelLimit(lua_State* L)
{
	const tagPulseClientProto* pProto = (tagPulseClientProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->dwLevelLock);
		return 1;
	}
	else
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取经脉技能神格限制
//-------------------------------------------------------------------------------------
int SC_GetPulseGodLevelLimit(lua_State* L)
{
	const tagPulseClientProto* pProto = (tagPulseClientProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->dwGodLock);
		return 1;
	}
	else
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 判断当前经脉是否处于激活状态
//-------------------------------------------------------------------------------------
int SC_GetPulseActiveState(lua_State* L)
{
	const tagPulseClientProto* pProto = (tagPulseClientProto*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		if (GT_INVALID != pProto->dwPerSkill1)
		{
			const tagSkillData* pSkillData = SkillMgr::Inst()->GetSkillData(pProto->dwPerSkill1/100);
			if (!P_VALID(pSkillData))
			{
				lua_pushnumber(L, 0);
				return 1;
			}
			else
			{
				if (pSkillData->nLevel < pProto->dwPerSkill1%100)
				{
					lua_pushnumber(L, 0);
					return 1;
				}
			}
			
		}

		if (GT_INVALID != pProto->dwPerSkill2)
		{
			const tagSkillData* pSkillData = SkillMgr::Inst()->GetSkillData(pProto->dwPerSkill2/100);
			if (!P_VALID(pSkillData))
			{
				lua_pushnumber(L, 0);
				return 1;
			}
			else
			{
				if (pSkillData->nLevel < pProto->dwPerSkill2%100)
				{
					lua_pushnumber(L, 0);
					return 1;
				}
			}

		}

		if (GT_INVALID != pProto->dwPerSkill3)
		{
			const tagSkillData* pSkillData = SkillMgr::Inst()->GetSkillData(pProto->dwPerSkill3/100);
			if (!P_VALID(pSkillData))
			{
				lua_pushnumber(L, 0);
				return 1;
			}
			else
			{
				if (pSkillData->nLevel < pProto->dwPerSkill3%100)
				{
					lua_pushnumber(L, 0);
					return 1;
				}
			}

		}


		lua_pushnumber(L, 1);
		return 1;
	}
	else
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取领域技能等级show
//-------------------------------------------------------------------------------------
int SC_GetAreaSkillShowLevel(lua_State* L)
{
	const tagSkillData* pProto = (tagSkillData*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->nLevel);
		return 1;
	}
	else
	{
		lua_pushnumber(L, 0);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取领域激活技能等级
//-------------------------------------------------------------------------------------
int SC_GetAreaActiveLevel(lua_State* L)
{
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pProto) && EGAT_GOD != pProto->eType)
	{
		lua_pushnumber(L, pProto->nLevelLimit);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取领域状态
//-------------------------------------------------------------------------------------
int SC_GetAreaState(lua_State* L)
{
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->eNatureType);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取领域激活神格等级
//-------------------------------------------------------------------------------------
int SC_GetAreaGodHeadLevel(lua_State* L)
{
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pProto) && EGAT_GOD != pProto->eType)
	{
		lua_pushnumber(L, pProto->nGodheadLimit);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取领域激活神格等级
//-------------------------------------------------------------------------------------
int SC_GetAreaLeftTime(lua_State* L)
{
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		if (EGAT_RARE == pProto->eType)
		{
			lua_pushnumber(L, pProto->nTime);
		}
		else
			lua_pushnumber(L, 0);

		return 1;
	}
	return 0;
}


//-------------------------------------------------------------------------------------
// 获取技能最大等级
//-------------------------------------------------------------------------------------
int SC_GetSkillMaxLevel(lua_State* L)
{
    const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
    if(P_VALID(pProto))
    {
        lua_pushnumber(L, pProto->nMaxLevel);
        return 1;
    }
    return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能熟练度
//-------------------------------------------------------------------------------------
int SC_GetSkillExp(lua_State* L)
{
	const tagSkillData* pSkill = (tagSkillData*)(lua_touserdata(L, -1));
	if(P_VALID(pSkill))
	{
		lua_pushnumber(L, pSkill->nExp);
		lua_pushnumber(L, pSkill->pProto->nLevelUpExp);
		return 2;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能伤害
//-------------------------------------------------------------------------------------
int SC_GetSkillDamage(lua_State* L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		  (lua_touserdata(L, -2));
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pSkill))
	{
        INT nSingle = 0;
        if (pSkill->pProto->nDmgTimes > 0 && pSkill->pProto->nChannelDmg[0] > 100000)
        {
            nSingle = pSkill->pProto->nChannelDmg[0];
        }
        lua_pushnumber(L, nSingle);
		lua_pushnumber(L, pSkill->nSkillDmg);
		return 2;
	}
	else if(P_VALID(pProto))
	{
		//如果技能没找到，则只跟据相应模板属性
        INT nSingle = 0;
        if (pProto->nDmgTimes > 0 && pProto->nChannelDmg[0] > 100000)
        {
            nSingle = pProto->nChannelDmg[0];
        }

		INT nSum = 0;
		for(INT i=0; i<pProto->nDmgTimes; ++i)
		{
			nSum += pProto->nChannelDmg[i];
		}
        lua_pushnumber(L, nSingle);
		lua_pushnumber(L, nSum);
		return 2;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取领域技能伤害
//-------------------------------------------------------------------------------------
int SC_GetAreaDamage(lua_State* L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		  (lua_touserdata(L, -2));
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pSkill))
	{
		INT nSingle = 0;
		if (pSkill->pProto->nDmgTimes > 0 && pSkill->pProto->nChannelDmg[0] > 100000)
		{
			nSingle = pSkill->pProto->nChannelDmg[0];
		}
		lua_pushnumber(L, nSingle);
		lua_pushnumber(L, pSkill->nSkillDmg);
		return 2;
	}
	else if(P_VALID(pProto))
	{
		//如果技能没找到，则只跟据相应模板属性
		const tagSkillProtoClient* pClientSkill = SkillProtoData::Inst()->FindSkillProto(pProto->dwSkillID);
		if(P_VALID(pClientSkill))
		{
			INT nSingle = 0;
			if (pClientSkill->nDmgTimes > 0 && pClientSkill->nChannelDmg[0] > 100000)
			{
				nSingle = pClientSkill->nChannelDmg[0];
			}

			INT nSum = 0;
			for(INT i=0; i<pClientSkill->nDmgTimes; ++i)
			{
				nSum += pClientSkill->nChannelDmg[i];
			}
			lua_pushnumber(L, nSingle);
			lua_pushnumber(L, nSum);
			return 2;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取技能施展距离
//-------------------------------------------------------------------------------------
int SC_GetSkillOPDistance(lua_State* L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		  (lua_touserdata(L, -2));
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pSkill))
	{
		lua_pushnumber(L, pSkill->fOPDist);
		return 1;
	}
	else if(P_VALID(pProto))
	{
		//如果技能没找到，则只跟据相应模板属性
		lua_pushnumber(L, pProto->fOPDist);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能起手时间
//-------------------------------------------------------------------------------------
int SC_GetSkillPrepareTime(lua_State* L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		  (lua_touserdata(L, -2));
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pSkill))
	{
		lua_pushnumber(L, pSkill->nPrepareTime);
		return 1;
	}
	else if(P_VALID(pProto))
	{
		//如果技能没找到，则只跟据相应模板属性
		lua_pushnumber(L, pProto->nPrepareTime);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能恢复时间
//-------------------------------------------------------------------------------------
int SC_GetSkillCooldownTime(lua_State* L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		  (lua_touserdata(L, -2));
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pSkill))
	{
		lua_pushnumber(L, pSkill->nCooldown);
		return 1;
	}
	else if(P_VALID(pProto))
	{
		//如果技能没找到，则只跟据相应模板属性
		lua_pushnumber(L, pProto->nCoolDown);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能属性加强值
//-------------------------------------------------------------------------------------
int SC_GetSkillAttribute(lua_State* L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_newtable(L);
		pProto->mapRoleAttMod.ResetIterator();
		ERoleAttribute eAtt;
		INT nValue;
		while(pProto->mapRoleAttMod.PeekNext(eAtt, nValue))
		{
			lua_pushnumber(L, eAtt);
			lua_pushnumber(L, nValue);
			lua_settable(L, -3);
		}
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能属性百分比加强值
//-------------------------------------------------------------------------------------
int SC_GetSkillAttributePct(lua_State* L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -2));
	if(P_VALID(pProto))
	{
		lua_newtable(L);
		pProto->mapRoleAttModPct.ResetIterator();
		ERoleAttribute eAtt;
		INT nValue;
		while(pProto->mapRoleAttModPct.PeekNext(eAtt, nValue))
		{
			lua_pushnumber(L, eAtt);
			lua_pushnumber(L, nValue);
			lua_settable(L, -3);
		}
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能消耗
//-------------------------------------------------------------------------------------
int SC_GetSkillCost(lua_State* L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		   (lua_touserdata(L, -3));	
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*) (lua_touserdata(L, -2));
	ESkillCostType				eCost = (ESkillCostType)(DWORD)(lua_tonumber(L, -1));
	INT nValue = pProto->nSkillCost[eCost];
	if(nValue > 100000 )
	{
		LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
		INT nMaxValue = 0;
		if(P_VALID(pLp))
		{
			ERoleAttribute eType = ERA_Null;
			switch (eCost)
			{
			case ESCT_HP:
				eType = ERA_MaxHP;	
				break;
			case ESCT_MP:
				eType = ERA_MaxMP;
				break;
			case ESCT_Rage:
				eType = ERA_Rage;
				break;
			case ESCT_Endurance:
				eType = ERA_MaxEndurance;
				break;
			case ESCT_Valicity:
				eType = ERA_MaxVitality;
				break;
			}
			if (ERA_Null != eType && ERA_Rage != eType )
			{
				nMaxValue = pLp->GetAttribute(eType);
			}
			else
				nMaxValue = 100;
		}
		nValue = INT( (nValue-100000) * nMaxValue / 10000 );
		lua_pushnumber(L, nValue);
		return 1;
	}
	else
	{
		lua_pushnumber(L, nValue);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取领域消耗
//-------------------------------------------------------------------------------------
int SC_GetAreaCost(lua_State* L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		   (lua_touserdata(L, -3));	
	const tagGodAreaName* pProtoC = (tagGodAreaName*) (lua_touserdata(L, -2));
	ESkillCostType				eCost = (ESkillCostType)(DWORD)(lua_tonumber(L, -1));
	const tagSkillProtoClient* pProto = SkillProtoData::Inst()->FindSkillProto(pProtoC->dwSkillID);
	INT nValue = 0; 
	if(P_VALID(pProto))
		pProto->nSkillCost[eCost];
	if(nValue > 100000 )
	{
		LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
		INT nMaxValue = 0;
		if(P_VALID(pLp))
		{
			ERoleAttribute eType = ERA_Null;
			switch (eCost)
			{
			case ESCT_HP:
				eType = ERA_MaxHP;	
				break;
			case ESCT_MP:
				eType = ERA_MaxMP;
				break;
			case ESCT_Rage:
				eType = ERA_Rage;
				break;
			case ESCT_Endurance:
				eType = ERA_MaxEndurance;
				break;
			case ESCT_Valicity:
				eType = ERA_MaxVitality;
				break;
			}
			if (ERA_Null != eType && ERA_Rage != eType )
			{
				nMaxValue = pLp->GetAttribute(eType);
			}
			else
				nMaxValue = 100;
		}
		nValue = INT( (nValue-100000) * nMaxValue / 10000 );
		lua_pushnumber(L, nValue);
		return 1;
	}
	else
	{
		lua_pushnumber(L, nValue);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取技能下一级描述
//-------------------------------------------------------------------------------------
int SC_GetSkillNextLevel(lua_State* L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szNextDesc);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pProto->szNextDesc, strlen(pProto->szNextDesc));
#endif
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取伤害对象数量
//-------------------------------------------------------------------------------------
int SC_GetSkillDamageNum(lua_State *L)
{
	const tagSkillProtoClient* pProto = (tagSkillProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->iMaxAttackNum);
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取领域伤害对象数量
//-------------------------------------------------------------------------------------
int SC_GetAreaDamageNum(lua_State *L)
{
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		const tagSkillProtoClient* pClientSkill = SkillProtoData::Inst()->FindSkillProto(pProto->dwSkillID);
		if(P_VALID(pClientSkill))
		{
			lua_pushnumber(L, pClientSkill->iMaxAttackNum);
			return 1;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取领域释放距离
//-------------------------------------------------------------------------------------
int SC_GetAreaOPDistance(lua_State *L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		  (lua_touserdata(L, -2));
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pSkill))
	{
		lua_pushnumber(L, pSkill->fOPDist);
		return 1;
	}
	else if(P_VALID(pProto))
	{
		//如果技能没找到，则只跟据相应模板属性
		const tagSkillProtoClient* pClientSkill = SkillProtoData::Inst()->FindSkillProto(pProto->dwSkillID);
		if(P_VALID(pClientSkill))
		{
			lua_pushnumber(L, pClientSkill->fOPDist);
			return 1;
		}
	}
	return 0;

}

//-------------------------------------------------------------------------------------
// 获取领域恢复时间
//-------------------------------------------------------------------------------------
int SC_GetAreaCooldownTime(lua_State* L)
{
	const tagSkillData*		   pSkill = (tagSkillData*)		  (lua_touserdata(L, -2));
	const tagGodAreaName* pProto = (tagGodAreaName*)(lua_touserdata(L, -1));
	if(P_VALID(pSkill))
	{
		lua_pushnumber(L, pSkill->nCooldown);
		return 1;
	}
	else if(P_VALID(pProto))
	{
		//如果技能没找到，则只跟据相应模板属性
		const tagSkillProtoClient* pClientSkill = SkillProtoData::Inst()->FindSkillProto(pProto->dwSkillID);
		if(P_VALID(pClientSkill))
		{
			lua_pushnumber(L, pClientSkill->nCoolDown);
			return 1;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
//							               状态信息相关函数
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// 获取状态名称
//-------------------------------------------------------------------------------------
int SC_GetBuffName(lua_State *L)
{
	const tagBuffProtoClient* pProto = (tagBuffProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szName);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else 
		lua_pushlstring(L, pProto->szName, strlen(pProto->szName));
#endif
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取状态描述
//-------------------------------------------------------------------------------------
int SC_GetBuffDesc(lua_State *L)
{
	const tagBuffProtoClient* pProto = (tagBuffProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->szDesc);
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else 
		lua_pushlstring(L, pProto->szDesc, strlen(pProto->szDesc));
#endif
		return 1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// 获取状态类型
//-------------------------------------------------------------------------------------
int SC_GetBuffType(lua_State *L)
{
	const tagBuffProtoClient* pProto = (tagBuffProtoClient*)(lua_touserdata(L, -1));
	if(P_VALID(pProto))
	{
		lua_pushnumber(L, pProto->nType3);
		return 1;
	}
	return 0;
}
//-----------------------------------------------------------------------------------
// 获取状态剩余时间
//-----------------------------------------------------------------------------------
int SC_GetBuffRemainTime(lua_State* L)
{
	const tagRoleBuff* pBuff = (tagRoleBuff*)(lua_touserdata(L, -1));
	if(P_VALID(pBuff))
	{
		if(pBuff->nPersistTimeLeft > 0)
		{	
			INT nvalue = (INT)( Kernel::Inst()->GetAccumTimeDW() - pBuff->dwStartTime );
			INT	nRemainTime = pBuff->nPersistTimeLeft - nvalue + nvalue % 1000 ;
			lua_pushnumber(L, nRemainTime/1000);
		}
		else
		{
			lua_pushnumber(L, 0);
		}
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
//											价格相关
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// 获取价格金钱数
//-------------------------------------------------------------------------------------
int SC_GetPriceSilver(lua_State* L)
{
	const tagItemCost* pCost = (tagItemCost*)(lua_touserdata(L, -1));
	if(P_VALID(pCost))
	{
		lua_pushnumber(L, MSilver2DBGold(pCost->n64CostSilver));
		lua_pushnumber(L, MSilver2DBSilver(pCost->n64CostSilver));
		return 2;
	}
	return 0;
}

// 获取税收
int SC_GetPriceSilverByTax(lua_State *L)
{
	const tagItemCost *pCost = (tagItemCost *)(lua_touserdata(L, -1));
	if (P_VALID(pCost))
	{
		if (pCost->dwCityID != 0)
		{
			// 获取税率
			INT32 nTax = CityMgr::Inst()->GetTaxRate(pCost->dwCityID);
			INT64 n64Tax = static_cast<INT64>(pCost->n64CostSilver * (nTax / 100.0f));
			lua_pushnumber(L, MSilver2DBGold(n64Tax));
			lua_pushnumber(L, MSilver2DBSilver(n64Tax));
			return 2;
		}
	}
	return 0;
}

// 判断是否是税收商店的物品
int SC_IsTax(lua_State *L)
{
	const tagItemCost* pCost = (tagItemCost*)(lua_touserdata(L, -1));
	if (P_VALID(pCost))
	{
		if (pCost->dwCityID)
			lua_pushboolean(L, TRUE);
		else
			lua_pushboolean(L, FALSE);
		return 1;
	}
	return 0;
}


//------------------------------------------------------------------------------------
// 获取价格特殊消耗类型
//------------------------------------------------------------------------------------
int SC_GetPriceSpecCost(lua_State* L)
{
	const tagItemCost* pCost = (tagItemCost*)(lua_touserdata(L, -1));
	if(P_VALID(pCost) && ECCT_Null != pCost->eCostType)
	{
		lua_pushnumber(L, pCost->eCostType);
		lua_pushnumber(L, pCost->nCostNum);
		lua_pushnumber(L, pCost->nRepLevel);

		if(pCost->eCostType == ECCT_ItemExchange)
		{
			const tagItemDisplayInfo* pInfo = ItemProtoData::Inst()->FindItemDisplay(pCost->dwCostItemTypeID);
			if(P_VALID(pInfo))
			{
#ifdef UNICODE
				LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pInfo->szName);
				lua_pushlstring(L, szTmp, strlen(szTmp));
#else
				lua_pushlstring(L, pInfo->szName, strlen(pInfo->szName));
#endif		
				return 4;
			}
		}

		return 3;
	}
	return 0;
}


//------------------------------------------------------------------------------------
// 获取商货名字
//------------------------------------------------------------------------------------
int SC_GetCommerceName(lua_State *L)
{
	
	const tagGoodInfo* pInfo = (const tagGoodInfo*)(lua_touserdata(L, -1));
	if (P_VALID(pInfo))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pInfo->strName.c_str());
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pInfo->strName.c_str(), strlen(pInfo->strName.c_str()));
#endif
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------------
// 获取商货类型
//------------------------------------------------------------------------------------
int SC_GetCommerceType(lua_State *L)
{
	const tagGoodInfo* pInfo = (const tagGoodInfo*)(lua_touserdata(L, -1));
	if (P_VALID(pInfo))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pInfo->strDisplayType.c_str());
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pInfo->strDisplayType.c_str(), strlen(pInfo->strDisplayType.c_str()));
#endif
		return 1;
	}
	return 0;
}


//------------------------------------------------------------------------------------
// 获取商货描述
//------------------------------------------------------------------------------------
int SC_GetCommerceDesc(lua_State *L)
{
	const tagGoodInfo* pInfo = (const tagGoodInfo*)(lua_touserdata(L, -1));
	if (P_VALID(pInfo))
	{
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pInfo->strDesc.c_str());
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, pInfo->strDesc.c_str(), strlen(pInfo->strDesc.c_str()));
#endif
		return 1;
	}
	return 0;
}



//------------------------------------------------------------------------------------
// 获取宠物技能类型
//------------------------------------------------------------------------------------
int SC_GetPetSkillType(lua_State *L)
{
    const tagPetSkillData* pProto = (const tagPetSkillData*)(lua_touserdata(L, -1));
    if(P_VALID(pProto))
    {
#ifdef UNICODE
        LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->strType.c_str());
        lua_pushlstring(L, szTmp, strlen(szTmp));
#else 
        lua_pushlstring(L, pProto->strType.c_str(), strlen(pProto->strType.c_str()));
#endif
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------------
// 获取宠物技能名称
//------------------------------------------------------------------------------------
int SC_GetPetSkillName(lua_State *L)
{
    const tagPetSkillData* pProto = (tagPetSkillData*)(lua_touserdata(L, -1));
    if(P_VALID(pProto))
    {
#ifdef UNICODE
        LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->strName.c_str());
        lua_pushlstring(L, szTmp, strlen(szTmp));
#else 
        lua_pushlstring(L, pProto->strName.c_str(), strlen(pProto->strName.c_str()));
#endif
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------------
// 获取宠物技能描述
//------------------------------------------------------------------------------------
int SC_GetPetSkillDesc(lua_State *L)
{
    const tagPetSkillData* pProto = (tagPetSkillData*)(lua_touserdata(L, -1));
    if(P_VALID(pProto))
    {
#ifdef UNICODE
        LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pProto->strDesc.c_str());
        lua_pushlstring(L, szTmp, strlen(szTmp));
#else 
        lua_pushlstring(L, pProto->strDesc.c_str(), strlen(pProto->strDesc.c_str()));
#endif
        return 1;
    }
    return 0;
}

//------------------------------------------------------------------------------------
// 获取宠物技能等级
//------------------------------------------------------------------------------------
int SC_GetPetSkillLevel(lua_State *L)
{
    const tagPetSkillData* pProto = (tagPetSkillData*)(lua_touserdata(L, -1));
    if(P_VALID(pProto))
    {
        lua_pushnumber(L, pProto->nLevel);
        return 1;
    }
    return 0;
}


//------------------------------------------------------------------------------------
// 获取升级帮派技能所需资金
//------------------------------------------------------------------------------------
int SC_GetResearchGuildSkillCostFund(lua_State *L)
{
	const tagSkillProtoClient* pAtt = (const tagSkillProtoClient*)(lua_touserdata(L, -1));

	if (P_VALID(pAtt))
	{
		DWORD dwID = pAtt->dwID;
		const tagGuildSkillInfo *pInfo = GuildMgr::Inst()->FindSkillInfo(dwID);
		if (P_VALID(pInfo))
		{
			lua_pushnumber(L, (lua_Number)pInfo->nResearchFund);
			return 1;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------------
// 获取升级帮派技能所需资材
//------------------------------------------------------------------------------------
int SC_GetResearchGuildSkillCostMaterial(lua_State *L)
{
	const tagSkillProtoClient* pAtt = (const tagSkillProtoClient*)(lua_touserdata(L, -1));

	if (P_VALID(pAtt))
	{
		DWORD dwID = pAtt->dwID;
		const tagGuildSkillInfo *pInfo = GuildMgr::Inst()->FindSkillInfo(dwID);
		if (P_VALID(pInfo))
		{
			lua_pushnumber(L, (lua_Number)pInfo->nResearchMaterial);
			return 1;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
int SC_GetIsFaBaoQianNengOpen(lua_State* L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	const tagItemProto* pProto = pEquip->GetProto();
	if(!P_VALID(pProto) || pProto->eType != EIT_FaBao || pEquip->IsNotIdetified())
		return 0;
	const tagFabaoSpec* pFabao = pEquip->GetFaBaoSpe();	
	if(!P_VALID(pFabao))
		return 0;

	if(pFabao->byMaxIntelligence != 0)
	{
		lua_pushnumber(L, 1);
		return 1;
	}

	return 0;
}

//------------------------------------------------------------------------------------
// 获取法宝属性：资质，加属性
//------------------------------------------------------------------------------------
int SC_GetFaBaoSpec(lua_State *L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	INT nRet = 0;
	const tagItemProto* pProto = pEquip->GetProto();
	if(!P_VALID(pProto) || pProto->eType != EIT_FaBao || pEquip->IsNotIdetified()) return nRet;
	const tagFabaoSpec* pFabao = pEquip->GetFaBaoSpe();	
	lua_pushinteger(L, (lua_Integer)pFabao->n16NativeIntelligence); // 法宝资质
	nRet++;

	lua_pushinteger(L, (lua_Integer)pFabao->nBirthTimes); //妖精繁育次数
	nRet++;

	// 法宝加属性
	lua_newtable(L);
	for(INT i=0; i< MaxFabaoRoleAttNUM; ++i)	
	{
		if( ERA_Null != pFabao->eAttType[i] && GT_INVALID != pFabao->nAttGrowing[i] )
		{
			lua_pushinteger(L, (lua_Integer)pFabao->eAttType[i]);
			lua_pushinteger(L, (lua_Integer)pFabao->nAttGrowing[i]);
			lua_settable(L, -3);
		}
	}	
	nRet++;

	// 五行槽属性 与是否激活
	lua_newtable(L);
	for( int i = 0; i < MaxWuxingSlotNum; ++i )
	{
		lua_pushinteger(L, i+1);
		lua_pushinteger(L, (lua_Integer)pFabao->eWuxingHole[i]);		
		lua_settable(L, -3);
	}
	nRet++;

	// 五行槽是否激活
	lua_newtable(L);
	for( int i=0; i< MaxWuxingSlotNum; ++i)
	{
		lua_pushinteger(L, i+1);
		if(i< pFabao->nSlotNum && pFabao->bySlotMacthNdx[i] &&  ERA_Null != pFabao->eSlotAttType[i] && GT_INVALID != pFabao->nSlotAttValue[i])
			lua_pushboolean(L, 1);
		else
			lua_pushboolean(L, 0);
		lua_settable(L, -3);
	}
	nRet++;

	// 五行槽奖励类型
	lua_newtable(L);
	for( int i = 0; i < MaxWuxingSlotNum; ++i )
	{		
		lua_pushinteger(L, i+1);
		if( ERA_Null != pFabao->eSlotAttType[i] && GT_INVALID != pFabao->nSlotAttValue[i])
		{			
			lua_pushinteger(L, (lua_Integer)pFabao->eSlotAttType[i]);			
		}
		else
			lua_pushnil(L);
		lua_settable(L, -3);
	}
	nRet++;

	// 五行槽奖励值
	lua_newtable(L);
	for( int i = 0; i < MaxWuxingSlotNum; ++i )
	{		
		lua_pushinteger(L, i+1);
		if( ERA_Null != pFabao->eSlotAttType[i] && GT_INVALID != pFabao->nSlotAttValue[i])
		{			
			lua_pushinteger(L, (lua_Integer)pFabao->nSlotAttValue[i]);			
		}
		else
			lua_pushnil(L);
		lua_settable(L, -3);
	}
	nRet++;
			

	// 镶嵌灵珠名字
	lua_newtable(L);
	for( int i = 0; i < pFabao->nSlotNum; ++i )
	{
		BOOL bSet = TRUE;
		lua_pushinteger(L, lua_Integer(i+1));
		if( P_VALID(pFabao->dwWuxingBeadID[i]) )
		{
			const tagConsolidateItem* pConItem = ConsolidateData::Inst()->FindConsolidateItemProto(pFabao->dwWuxingBeadID[i]);
			const tagItemProto *pLinZhuProto  = ItemProtoData::Inst()->FindItemProto(pFabao->dwWuxingBeadID[i]);
			if(P_VALID(pLinZhuProto) && P_VALID(pConItem))
			{
				bSet = FALSE;
#ifdef UNICODE
				LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(pLinZhuProto->pDisplayInfo->szName);
				lua_pushlstring(L, szTmp, strlen(szTmp));
#else
				lua_pushlstring(L, pLinZhuProto->pDisplayInfo->szName, strlen(pLinZhuProto->pDisplayInfo->szName));
#endif	
			}
		}
		if(bSet) lua_pushnil(L);
		lua_settable(L, -3);
	}
	nRet++;


	// 镶嵌灵珠获取的属性
	for( int i = 0; i < 5; ++i )
	{
		lua_newtable(L);
		BOOL bSet = TRUE;
		if( P_VALID(pFabao->dwWuxingBeadID[i]) )
		{
			const tagConsolidateItem* pConItem = ConsolidateData::Inst()->FindConsolidateItemProto(pFabao->dwWuxingBeadID[i]);
			const tagItemProto *pLinZhuProto  = ItemProtoData::Inst()->FindItemProto(pFabao->dwWuxingBeadID[i]);
			if(P_VALID(pLinZhuProto) && P_VALID(pConItem))
			{
				const tagConsolidateItem* p = ConsolidateData::Inst()->FindConsolidateItemProto(pFabao->dwWuxingBeadID[i]);
				if( P_VALID(p) )
				{
					for(int j = 0; j < 3; j++)
					{
						lua_pushinteger(L, lua_Integer(j+1));
						if(p->tagRoleAtt[j].nAttVal != GT_INVALID)
							lua_pushinteger(L, (lua_Integer)p->tagRoleAtt[j].eRoleAtt);
						else
							lua_pushnil(L);
						lua_settable(L, -3);
					}
				}
			}
		}
		else
		{
			lua_pushinteger(L, lua_Integer(1));
			lua_pushnil(L);
			lua_settable(L,-3);
		}
		nRet++;
	}

	// 镶嵌灵珠获取的属性
	for( int i = 0; i < 5; ++i )
	{
		lua_newtable(L);
		BOOL bSet = TRUE;
		if( P_VALID(pFabao->dwWuxingBeadID[i]) )
		{
			const tagConsolidateItem* pConItem = ConsolidateData::Inst()->FindConsolidateItemProto(pFabao->dwWuxingBeadID[i]);
			const tagItemProto *pLinZhuProto  = ItemProtoData::Inst()->FindItemProto(pFabao->dwWuxingBeadID[i]);
			if(P_VALID(pConItem) && P_VALID(pLinZhuProto))
			{
				const tagConsolidateItem* p = ConsolidateData::Inst()->FindConsolidateItemProto(pFabao->dwWuxingBeadID[i]);
				if( P_VALID(p) ) 
				{
					for(int j = 0; j < 3; j++)
					{
						lua_pushinteger(L, lua_Integer(j+1));
						if(p->tagRoleAtt[j].nAttVal != GT_INVALID)
							lua_pushinteger(L, (lua_Integer)p->tagRoleAtt[j].nAttVal);
						else 
							lua_pushnil(L);
						lua_settable(L, -3);
					}
				}
			}	
		} 
		else
		{
			lua_pushinteger(L, lua_Integer(1));
			lua_pushnil(L);
			lua_settable(L, -3);
		}
		nRet++;
	}

	// 五行切合奖励
	lua_newtable(L);
	for(int i=0; i< MaxWuxingRewardNUM; i++)
	{
		DWORD dwAwardID = GT_INVALID;//pFabao->nWuxingMatchReward[i];
		BOOL bSet = TRUE;
		lua_pushinteger(L, lua_Integer(i+1));
		if( dwAwardID != GT_INVALID && dwAwardID != 0 )
		{
			tstring szDes = ItemProtoData::Inst()->FindTrumpAwardByID(dwAwardID);
#ifdef UNICODE
			LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szDes.c_str());
			lua_pushlstring(L, szTmp, strlen(szTmp));
#else
			lua_pushlstring(L, szDes.c_str(), szDes.length());
#endif
			bSet = FALSE;
		}
		if(bSet) lua_pushnil(L);
		lua_settable(L, -3);
	}
	nRet++;

	// 五行切合奖励是否激活
	lua_newtable(L);
	for(int i=0; i< MaxWuxingRewardNUM; i++)
	{
		lua_pushinteger(L, lua_Integer(i+1));
		int rewardCount = pFabao->bySlotLingzhuMatch - 3 - i;
		//if( rewardCount>=0 && GT_VALID(pFabao->nWuxingMatchReward[i]))
		//	lua_pushboolean(L,1);
		//else
			lua_pushboolean(L,0);
		lua_settable(L, -3);
	}
	nRet++;

	// 妖精技能
	lua_newtable(L);
	for(int i=0; i< MaxFairySkillNum; i++)
	{
		DWORD dwAwardID =pFabao->dwFairySkill[i];
		BOOL bSet = TRUE;
		lua_pushinteger(L, lua_Integer(i+1));
		if( dwAwardID != GT_INVALID && dwAwardID != 0 )
		{
			const tagFairySkill *pSkill = ItemProtoData::Inst()->FindFairySkill(dwAwardID);
			if(P_VALID(pSkill))
			{
				tstring szDes = pSkill->szName;
#ifdef UNICODE
				LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szDes.c_str());
				lua_pushlstring(L, szTmp, strlen(szTmp));
#else
				lua_pushlstring(L, szDes.c_str(), szDes.length());
#endif
				bSet = FALSE;
			}
		}
		if(bSet) lua_pushnil(L);
		lua_settable(L, -3);
	}
	nRet++;

	return nRet;
}

int SC_GetFabaoRoleAtt(lua_State* L)
{
	tagItemProto* pItemProto = (tagItemProto*) lua_touserdata(L, -1);
	if(!P_VALID(pItemProto)) 
		return 0;

	const tagFabaoProto* pFabao = ItemProtoData::Inst()->FindTrumpProto(pItemProto->dwTypeID);
	if(!P_VALID(pFabao))
		return 0;

	lua_pushinteger(L, pFabao->eAttType[0]);
	lua_pushinteger(L, pFabao->eAttType[1]);
	lua_pushinteger(L, pFabao->eAttType[2]);
	return 3;
}

int SC_GetEquipRoleAtt(lua_State* L)
{
	tagEquipProto* pEquipProto = (tagEquipProto*)lua_touserdata(L, -1);
	if(!P_VALID(pEquipProto))
		return 0;
	
	for(int i = 0; i < MAX_ROLEATT_BASE_EFFECT; i++)
	{
		lua_pushinteger(L, pEquipProto->BaseEffect[i].eRoleAtt);
		lua_pushinteger(L, pEquipProto->BaseEffect[i].nValue);
	}
	return MAX_ROLEATT_BASE_EFFECT*2;
}

//-----------------------------------------------------------------------------
//是否是具有成长属性的装备
//-----------------------------------------------------------------------------
int SC_GetIsGrowEquip(lua_State* L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagEquipSpec* pSpec = pEquip->GetEquip();
	if(!P_VALID(pSpec))
		return 0;

	if(pSpec->dwGrowTypeID != 0 && pSpec->dwGrowTypeID != GT_INVALID)
	{
		lua_pushnumber(L, 1);
		return 1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
//成长装备的强度
//-----------------------------------------------------------------------------
int SC_GetGrowEquipStrength(lua_State* L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagEquipSpec* pSpec = pEquip->GetEquip();
	if(!P_VALID(pSpec))
		return 0;

	const tagEquipGrowName* pProto = ItemProtoData::Inst()->FindEquipGrowProto(pSpec->dwGrowTypeID);
	if(!P_VALID(pProto))
		return 0;

	lua_pushnumber(L, pProto->byStrength);

	return 1;
}

//-----------------------------------------------------------------------------
//获取装备经验
//-----------------------------------------------------------------------------
int SC_GetEquipGrowExp(lua_State* L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagEquipSpec* pSpec = pEquip->GetEquip();
	if(!P_VALID(pSpec))
		return 0;

	const tagEquipGrowName* pProto = ItemProtoData::Inst()->FindEquipGrowProto(pSpec->dwGrowTypeID);
	if(!P_VALID(pProto))
		return 0;

	BYTE byEquipGrowLevel = ItemProtoData::Inst()->GetEquipGrowLevel(pSpec->dwGrowValue, pProto);
	DWORD dwEquipGrowCurMaxExp = ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel, pProto);

	lua_pushnumber(L, pSpec->dwGrowValue);	//当前经验
	lua_pushnumber(L, dwEquipGrowCurMaxExp);//当前最大经验

	return 2;
}

//-----------------------------------------------------------------------------
//获取星星颜色
//-----------------------------------------------------------------------------
int SC_GetEquipStarColor(lua_State* L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagEquipSpec* pSpec = pEquip->GetEquip();
	if(!P_VALID(pSpec))
		return 0;

	lua_newtable(L);
	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL - 1; i++)
	{
		lua_pushinteger(L, i);
		tstring szDes;
		switch(pSpec->eGrowStar[i])
		{
		case EEGS_Null:
			szDes = g_StrTable[_T("Star_Null")];
			break;
		case EEGS_Gray:
			szDes = g_StrTable[_T("Star_Gray")];
			break;
		case EEGS_Green:
			szDes = g_StrTable[_T("Star_Green")];
			break;
		case EEGS_Blue:
			szDes = g_StrTable[_T("Star_Blue")];
			break;
		case EEGS_Purple:
			szDes = g_StrTable[_T("Star_Purple")];
			break;
		}
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szDes.c_str());
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, szDes.c_str(), szDes.length());
#endif
		lua_settable(L, -3);
	}

	return 1;
}

//-----------------------------------------------------------------------------
//获取装备成长属性
//-----------------------------------------------------------------------------
int SC_GetEquipGrowAtt(lua_State* L)
{
	//MGetEquipGrowRatio
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagEquipSpec* pSpec = pEquip->GetEquip();
	if(!P_VALID(pSpec))
		return 0;

	const tagEquipGrowName* pProto = ItemProtoData::Inst()->FindEquipGrowProto(pSpec->dwGrowTypeID);
	if(!P_VALID(pProto))
		return 0;

	//5*11
	DWORD dwTempAttValue = 0;
	DWORD dwTempMaxAttValue = 0;
	float fTempCurRatio = 0.0f;
	TCHAR szText[128] = {0};
	lua_newtable(L);
	for(int i = 0; i < EQUIP_GROW_ADD_ATT_MAX_NUM; i++)
	{
		if(ERA_Null == pProto->eAtt[i])	//没有属性
			continue;
		lua_pushnumber(L, i);
		lua_pushnumber(L, pProto->eAtt[i]);
		lua_settable(L, -3);

	}

	lua_newtable(L);
	for(int i = 0; i < EQUIP_GROW_ADD_ATT_MAX_NUM; i++)
	{
		if(ERA_Null == pProto->eAtt[i])	//没有属性
			continue;

		dwTempAttValue	= 0;
		dwTempMaxAttValue = 0;
		fTempCurRatio	= 0.0f;

		BYTE byEquipGrowLevel = ItemProtoData::Inst()->GetEquipGrowLevel(pSpec->dwGrowValue, pProto);	//获得当前成长等级
		
		if(pProto->byAttOpenLevel[i] > byEquipGrowLevel+1)	//未开启,显示0/0
		{
			DWORD dwTempMaxAttValue0 = 0;
			for(int j = 0; j < EQUIP_GROW_MAX_LEVEL; j++)	//最大值还是要显示
			{
				dwTempMaxAttValue0 += pProto->dwAttValue[i][j];
			}


			lua_pushinteger(L, i);
			TCHAR szTempValue[56] = {0};
			_stprintf(szTempValue, _T(" +0(%d)"), dwTempMaxAttValue0);
			tstring szDes = szTempValue;
#ifdef UNICODE
			LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szDes.c_str());
			lua_pushlstring(L, szTmp, strlen(szTmp));
#else
			lua_pushlstring(L, szDes.c_str(), szDes.length());
#endif
			lua_settable(L, -3);
			continue;	//跳出去
		}

		for(int j = 1; j < byEquipGrowLevel; j++)	//累加前面几个星星的属性数据值，剩下最后一个单独处理
		{
			MGetEquipGrowRatio(pSpec->eGrowStar[j-1], fTempCurRatio);
			dwTempAttValue += pProto->dwAttValue[i][j] * fTempCurRatio;
		}
		DWORD dwEquipGrowCurMaxExp	= ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel, pProto);
		DWORD dwEquipGrowLastMaxExp = ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel - 1, pProto);
		if(byEquipGrowLevel != 0)
		{

			dwTempAttValue += pProto->dwAttValue[i][0];	//第一个数据不成星星倍率

			MGetEquipGrowRatio(pSpec->eGrowStar[byEquipGrowLevel-1], fTempCurRatio);
			dwTempMaxAttValue = dwTempAttValue + pProto->dwAttValue[i][byEquipGrowLevel] * fTempCurRatio;
			dwTempAttValue += pProto->dwAttValue[i][byEquipGrowLevel] * fTempCurRatio * (pSpec->dwGrowValue-ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel-1, pProto)) / pProto->dwExp[byEquipGrowLevel];	//最后一个数值计算公式：本阶段属性加成值*星星倍率*（本阶段最大经验值 - 上一阶段最大经验值）/本阶段最大经验值
		}
		else
		{
			dwTempMaxAttValue = pProto->dwAttValue[i][byEquipGrowLevel];
			dwTempAttValue += pProto->dwAttValue[i][byEquipGrowLevel] * (pSpec->dwGrowValue) / (ItemProtoData::Inst()->GetEquipGrowCurMaxExp(0, pProto));	//最后一个数值计算公式：本阶段属性加成值*星星倍率*（本阶段最大经验值 - 上一阶段最大经验值）/本阶段最大经验值
		}

		for(int j = byEquipGrowLevel+1; j < EQUIP_GROW_MAX_LEVEL; j++)
		{
			dwTempMaxAttValue += pProto->dwAttValue[i][j];
		}

		_stprintf(szText, _T(" +%d(%d)"), dwTempAttValue, dwTempMaxAttValue);
		lua_pushinteger(L, i);

		tstring szDes = szText;
#ifdef UNICODE
		LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szDes.c_str());
		lua_pushlstring(L, szTmp, strlen(szTmp));
#else
		lua_pushlstring(L, szDes.c_str(), szDes.length());
#endif
		lua_settable(L, -3);

	}

	return 2;
}

//获取宝石增幅级别
int SC_GetDiamondAddLevel(lua_State* L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	if(P_VALID(pEquip))
	{
		lua_pushinteger(L, pEquip->GetEquip()->byDiamondLevel);
		return 1;
	}

	return 0;
}

int SC_IsHolyMan(lua_State* L)
{
	tagEquipProto* pEquip = (tagEquipProto*)lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	if(pEquip->eType == EIT_Holy)
	{
		lua_pushinteger(L, 1);
		return 1;
	}

	return 0;
}

int SC_GetHolyManAtt(lua_State* L)
{
	Equipment* pEquip = (Equipment*)lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagHolySpec* pHolySpec = pEquip->GetHolySpec();
	if(!P_VALID(pHolySpec))
		return 0;

	const tagSoulAttName* pShengLingProto = ItemProtoData::Inst()->FindShenLingName(pEquip->GetItemTypeID());
	if(!P_VALID(pShengLingProto))
		return 0;

	lua_pushinteger(L, pHolySpec->nCoValue);	//默契
	lua_pushinteger(L, pHolySpec->nDevourNum);	//当前吃了多少只妖精了~
	lua_pushinteger(L, pHolySpec->n16EquipmentNumber);	//可装备数量
	lua_pushinteger(L, pHolySpec->dwLevelUpAtt[0]);		//圣灵伤害
	lua_pushinteger(L, pHolySpec->dwLevelUpAtt[1]);		//圣灵防御
	lua_pushinteger(L, pHolySpec->dwLevelUpAtt[2]);		//致命
	lua_pushinteger(L, pHolySpec->dwLevelUpAtt[3]);		//致命量
	lua_pushinteger(L, pHolySpec->dwLevelUpAtt[4]);		//伤害加深
	lua_pushinteger(L, pHolySpec->dwLevelUpAtt[5]);		//精准
	lua_pushinteger(L, pHolySpec->dwLevelUpAtt[6]);		//灵巧
	lua_pushinteger(L, pHolySpec->dwLevelUpAtt[7]);		//灵能最大值
	lua_pushinteger(L, pHolySpec->nMaxDevourNum);		//最大吞噬妖精次数
	lua_pushinteger(L, pHolySpec->dwLevelUpAtt[7] - pHolySpec->nCostHoly);			//消耗的灵能


	TCHAR szText[128] = {0};
	lua_newtable(L);
	for(int i = 0; i < MaxHolyEquipNum; i++)
	{
		lua_pushinteger(L, i);
		Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetConItemByID(EICT_HolyManEquip, pHolySpec->n64EquipSerialIDs[i]);
		const tagItemDisplayInfo* pProto = NULL;
		if(P_VALID(pEquip))
		{
			pProto = ItemProtoData::Inst()->FindItemDisplay(pEquip->GetItemTypeID());
			if(P_VALID(pProto))
			{
				if(pEquip->GetHolyEquipSpec()->n16EnhanceCount > 0)
					_stprintf(szText, _T("<color=%x>%s +%d"), GetColorValue[pEquip->GetItemQuality()],pProto->szName, pEquip->GetHolyEquipSpec()->n16EnhanceCount);
				else
					_stprintf(szText, _T("<color=%x>%s"), GetColorValue[pEquip->GetItemQuality()],pProto->szName);
				tstring szDes = szText;
#ifdef UNICODE
				LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szDes.c_str());
				lua_pushlstring(L, szTmp, strlen(szTmp));
#else
				lua_pushlstring(L, szDes.c_str(), szDes.length());
#endif
			}
		}
		else
		{
			DWORD dwTypeID = pHolySpec->n64EquipSerialIDs[i]/1000;
			DWORD dwLevel = pHolySpec->n64EquipSerialIDs[i]%1000;
			pProto = ItemProtoData::Inst()->FindItemDisplay(dwTypeID);
			const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(dwTypeID);
			if(P_VALID(pProto))
			{
				if(dwLevel > 0)
					_stprintf(szText, _T("<color=%x>%s +%d"), GetColorValue[pItemProto->byQuality], pProto->szName, dwLevel);
				else
					_stprintf(szText, _T("<color=%x>%s"), GetColorValue[pItemProto->byQuality], pProto->szName);
				tstring szDes = szText;
#ifdef UNICODE
				LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szDes.c_str());
				lua_pushlstring(L, szTmp, strlen(szTmp));
#else
				lua_pushlstring(L, szDes.c_str(), szDes.length());
#endif
			}
			else
			{
				tstring szDes = _T("");
#ifdef UNICODE
				LPCSTR szTmp = TObjRef<Util>()->UnicodeToUnicode8(szDes.c_str());
				lua_pushlstring(L, szTmp, strlen(szTmp));
#else
				lua_pushlstring(L, szDes.c_str(), szDes.length());
#endif
			}
		}
		lua_settable(L, -3);
	}

	return 14;
}

//-------------------------------------------------------------------------------------
// 判断是否是元神装备
//-------------------------------------------------------------------------------------
static int SC_IsHolyManEquip(lua_State* L)
{
	tagEquipProto* pEquip = (tagEquipProto*)lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	if(pEquip->eType == EIT_HolyEquip)
	{
		lua_pushinteger(L, 1);
		return 1;
	}
	return 0;
}

//-------------------------------------------------------------------------------------
// 获取圣灵装备属性
//-------------------------------------------------------------------------------------
static int SC_GetHolyEquipAtt(lua_State* L)
{
	Equipment* pEquip = (Equipment*)lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagHolyEquipSpec* pHolyEquipSpec = pEquip->GetHolyEquipSpec();
	if(!P_VALID(pHolyEquipSpec))
		return 0;

	const tagHolyEquipProto* pHolyEquipProto = ItemProtoData::Inst()->FindHolyEquipProto(pEquip->GetItemTypeID());
	if(!P_VALID(pHolyEquipProto))
		return 0;

	lua_pushnumber(L, pHolyEquipSpec->nCostHoly);	//消耗灵能值
	lua_pushnumber(L, pHolyEquipSpec->nHolyAttEffect[0]);//圣灵伤害
	lua_pushnumber(L, pHolyEquipSpec->nHolyAttEffect[1]);//圣灵防御
	lua_pushnumber(L, pHolyEquipSpec->nHolyAttEffect[2]);//致命
	lua_pushnumber(L, pHolyEquipSpec->nHolyAttEffect[3]);//致命量
	lua_pushnumber(L, pHolyEquipSpec->nHolyAttEffect[4]);//伤害加深
	lua_pushnumber(L, pHolyEquipSpec->nHolyAttEffect[5]);//精准0
	lua_pushnumber(L, pHolyEquipSpec->nHolyAttEffect[6]);//灵巧
	lua_pushnumber(L, pHolyEquipProto->nHolyLevel);
	return 9;
}

int SC_GetShengWenChongLevel(lua_State* L)
{
	Equipment* pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(!P_VALID(pEquip))
		return 0;

	const tagHolyEquipSpec* pSpec = pEquip->GetHolyEquipSpec();
	if(!P_VALID(pSpec))
		return 0;

	lua_pushnumber(L, pSpec->n16EnhanceCount);
	return 1;
}

int SC_GetGoodsHolyManAtt(lua_State* L)
{
	tagEquipProto* pEquip = (tagEquipProto*)lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagSoulAttName* pShengLingName = ItemProtoData::Inst()->FindShenLingName(pEquip->dwTypeID);
	if(!P_VALID(pShengLingName))
		return 0;

	lua_pushnumber(L, pShengLingName->nHolyDmg);			//伤害			
	lua_pushnumber(L, pShengLingName->nHolyDef);			//防御
	lua_pushnumber(L, pShengLingName->nHolyValue);			//灵能
	lua_pushnumber(L, pShengLingName->nCrit);				//致命
	lua_pushnumber(L, pShengLingName->n16HolyCritRate);		//致命量		
	lua_pushnumber(L, pShengLingName->nExDamage);			//伤害加深		
	lua_pushnumber(L, pShengLingName->nAttackTec);			//精准	
	lua_pushnumber(L, pShengLingName->nNeglectToughness);	//灵巧

	return 8;

}

int SC_GetGoodsHolyEquipAtt(lua_State* L)
{
	tagEquipProto* pEquip = (tagEquipProto*)lua_touserdata(L, -1);
	if(!P_VALID(pEquip))
		return 0;

	const tagHolyEquipProto* pShengWen = ItemProtoData::Inst()->FindHolyEquipProto(pEquip->dwTypeID);
	if(!P_VALID(pShengWen))
		return 0;

	lua_pushnumber(L, pShengWen->BaseEffect[0].eHolyAtt);	
	lua_pushnumber(L, pShengWen->BaseEffect[0].nValue);	
	lua_pushnumber(L, pShengWen->BaseEffect[1].eHolyAtt);
	lua_pushnumber(L, pShengWen->BaseEffect[1].nValue);	
	lua_pushnumber(L, pShengWen->BaseEffect[2].eHolyAtt);
	lua_pushnumber(L, pShengWen->BaseEffect[2].nValue);	
	lua_pushnumber(L, pShengWen->BaseEffect[3].eHolyAtt);
	lua_pushnumber(L, pShengWen->BaseEffect[3].nValue);	
	lua_pushnumber(L, pShengWen->BaseEffect[4].eHolyAtt);	
	lua_pushnumber(L, pShengWen->BaseEffect[4].nValue);	
	lua_pushnumber(L, pShengWen->BaseEffect[5].eHolyAtt);
	lua_pushnumber(L, pShengWen->BaseEffect[5].nValue);	
	lua_pushnumber(L, pShengWen->BaseEffect[6].eHolyAtt);	
	lua_pushnumber(L, pShengWen->BaseEffect[6].nValue);	

	return 14;
}

//-------------------------------------------------------------------------------------
// 元神装备加成属性
//-------------------------------------------------------------------------------------
int SC_HolyEquipAttDec(lua_State* L)
{
	tstringstream tss;
	Equipment*	pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip))
	{
		DWORD dwEquipID = pEquip->GetItemTypeID();
		tstring str = HolyManMgr::Inst()->GetHolyEquipAttTypeName(dwEquipID);
		BYTE byLevel = pEquip->GetHolyComposeLv();
		INT	nValue = HolyManMgr::Inst()->GetHolyEquipAttValue(dwEquipID);
		EHolyEquipAttType  eType	= HolyManMgr::Inst()->GetHolyEquipAttType(dwEquipID);
		INT nAddValue	= HolyManMgr::Inst()->GetStarEnhanceAttValue( byLevel , eType );

		tss<<str<<_T("  + ")<< (nValue+nAddValue) ;
	}
	LPCSTR  szTmp = TObjRef<Util>()->UnicodeToUnicode8(tss.str().c_str());
	lua_pushlstring(L,szTmp,strlen(szTmp));
	return 1;
}

//-------------------------------------------------------------------------------------
// 元神装备加成属性
//-------------------------------------------------------------------------------------
int SC_HolyEquipComposeLv(lua_State* L)
{
	INT nLevel = 0;
	Equipment*	pEquip = (Equipment*)(lua_touserdata(L, -1));
	if(P_VALID(pEquip))
	{
		BYTE byLevel = pEquip->GetHolyComposeLv();
		if ( byLevel > MAX_STARENHANCE_LEVEL )
			byLevel = MAX_STARENHANCE_LEVEL;
		else if ( byLevel < 0 )
			byLevel = 0;
		nLevel = (INT)byLevel;
	}
	lua_pushnumber(L, nLevel);
	return 1;
}

//-------------------------------------------------------------------------------------
// 返回五角星图形文字
//-------------------------------------------------------------------------------------
int SC_GetSpecSharp(lua_State* L)
{
	tstringstream tss;
	tss<<g_StrTable[_T("ColorPrint_WuJiaoXing")];
	LPCSTR  szTmp = TObjRef<Util>()->UnicodeToUnicode8(tss.str().c_str());
	lua_pushlstring(L,szTmp,strlen(szTmp));
	return 1;

}


//-----------------------------------------------------------------------------
// table of function names and entry points that will be registered with Lua
//-----------------------------------------------------------------------------
static const luaL_reg tooltipLib[] = 
{
	{"GetLocalPlayerLevel",				SC_GetLocalPlayerLevel},
	{"GetLocalPlayerSex",				SC_GetLocalPlayerSex},
	{"GetLocalPlayerClass",				SC_GetLocalPlayerClass},
	{"GetLocalPlayerRepution",			SC_GetLocalPlayerRepution},
	{"GetLocalPlayerContribution",		SC_GetLocalPlayerContribution},
	{"GetLocalPlayerTalentBidden",		SC_GetLocalPlayerTalentBidden},
	{"GetLocalPlayerAttribute",			SC_GetLocalPlayerAttribute},
	{"GetLocalPlayerAttBidden",			SC_GetLocalPlayerAttBidden},
	{"GetLocalPlayerAttLevelup",		SC_GetLocalPlayerAttLevelup},
	{"GetLocalPlayerCostAttCurValue",	SC_GetLocalPlayerCostAttCurValue},
	//more
	{"GetItemName",						SC_GetItemName},
	{"GetItemLockString",				SC_GetItemLockString},
	{"GetItemNameEx",					SC_GetItemNameEx},
	{"GetItemDesc",						SC_GetItemDesc},
	{"GetItemType",						SC_GetItemType},
	{"GetEquipTypeDesc",				SC_GetEquipTypeDesc},
	{"GetItemTypeID",					SC_GetItemTypeID},
	{"GetItemQuality",					SC_GetItemQuality},
	{"GetItemBindType",					SC_GetItemBindType},
	{"GetItemBindState",				SC_GetItemBindState},
	{"GetItemAccountBindState",SC_GetItemAccountBindState},
	{"GetItemLockState",				SC_GetItemLockState},
	{"GetItemSexLimit",					SC_GetItemSexLimit},
	{"GetItemLevelLimit",				SC_GetItemLevelLimit},
	{"GetEquipLevelLimit",				SC_GetEquipLevelLimit},
	{"GetItemMaxUseTimes",				SC_GetItemMaxUseTimes},
	{"GetItemUseTimes",					SC_GetItemUseTimes},
	{"GetItemCreator",					SC_GetItemCreator},
	{"IsEquipSignatured",				SC_IsEquipSignatured},
	{"GetEquipSignaturePlayer",			SC_GetEquipSignaturePlayer},
	{"GetEquipSignature",				SC_GetEquipSignature},
	{"GetItemUseType",					SC_GetItemUseType},
	{"GetItemCostPotVal",				SC_GetItemCostPotVal},
	{"ShowGoodsUsePart",              SC_ShowGoodsUsePart},
	{"ShowGoodsUseLevel",              SC_ShowGoodsUseLevel},
	{"ShowStrengthenPart",				SC_ShowStrengthenPart},
	{"StrengthenLevel",					SC_StrengthenLevel},
	{"GetItemHoldEquipPos",				SC_GetItemHoldEquipPos},
    {"GetItemAllTimeLimit",             SC_GetItemAllTimeLimit},
    {"GetItemProtoAllTimeLimit",        SC_GetItemProtoAllTimeLimit},
	{"GetItemTimeLimit",				SC_GetItemTimeLimit},
	{"GetItemLearnSkill",				SC_GetItemLearnSkill},
	{"GetItemClanReputeLimit",			SC_GetItemClanReputeLimit},
	{"GetItemOtherClanReputeLimit",		SC_GetItemOtherClanReputeLimit},
    {"GetItemCoolDownTime",             SC_GetItemCoolDownTime},
    {"GetItemCanSell",                  SC_GetItemCanSell},
	{"IsItemInAmazingBag",              SC_IsItemInAmazingBag},
    {"GetItemPreTime",                  SC_GetItemPreTime},
    {"GetItemMaxItem",                  SC_GetItemMaxItem},
	{"GetAreaSkillBookDesc",			SC_GetAreaSkillBookDesc},
    {"GetItemBindRoleName",             SC_GetItemBindRoleName},
	{"GetItemYueDingRoleName",          SC_GetItemYueDingRoleName},
	{"GetEquipGodHeadLimit",			SC_GetEquipGodHeadLimit},
	{"GetLocalPlayerGodHead",			SC_GetLocalPlayerGodHead},
	{"IsCostBelief",					SC_IsCostBelief},
    
	//more
	{"GetEquipDescEx",					SC_GetEquipDescEx},
	{"GetEquipClassLimit",				SC_GetEquipClassLimit},
	{"GetEquipPos",						SC_GetEquipPos},
	{"GetEquipWeaponDamage",			SC_GetEquipWeaponDamage},
	{"GetEquipWeaponWuhun",				SC_GetEquipWeaponWuhun},
	{"GetEquipArmorDefine",				SC_GetEquipArmorDefine},
	{"GetEquipNewNess",					SC_GetEquipNewNess},
	{"GetEquipPotTime",					SC_GetEquipPotAdvanceTime},
	{"GetEquipPotVal",					SC_GetEquipPotVal},
	{"GetShopEquipPotVal",					SC_GetShopEquipPotVal},
	{"GetEquipProtoAtt",				SC_GetEquipProtoAtt},
	{"GetEquipAttribute",				SC_GetEquipAttribute},
	{"GetEquipAttributePct",			SC_GetEquipAttributePct},
	{"GetCastingAttribute",				SC_GetCastingAttribute},
	{"GetEquipProduceState",			SC_GetEquipProduceState},
	{"GetEquipHoldNum",					SC_GetEquipHoldNum},
	{"GetEquipBrandLevel",				SC_GetEquipBrandLevel},
	{"GetEquipStrengLevel",				SC_GetEquipStrengLevel},
	{"GetEquipHoldGem",					SC_GetEquipHoldGem},
	{"GetEquipLongfu",					SC_GetEquipLongfu},
	{"GetEquipLonghun",					SC_GetEquipLonghun},
	{"GetEquipLonghunActive",			SC_GetEquipLonghunActive},
	{"GetEquipLonghunFive",				SC_GetEquipLonghunFive},
	{"GetEquipSpecAttribute",			SC_GetEquipSpecAttribute},
	{"GetEquipSuitName",				SC_GetEquipSuitName},
	{"GetEquipSuitAttribute",			SC_GetEquipSuitAttribute},
	{"GetEquipAttributeLimit",			SC_GetEquipAttributeLimit},
	{"GetEquipQltyModPct",				SC_GetEquipQltyModPct},
	{"GetEquipColor",					SC_GetEquipColor},
	{"GetEquipQltyEffect",				SC_GetEquipQltyEffect},
    {"GetEquipType",                    SC_GetEquipType},
	{"HolyEquipAttDec",					SC_HolyEquipAttDec},
	{"HolyEquipComposeLv",				SC_HolyEquipComposeLv}, //冲星等级
	{"GetSpecSharp",					SC_GetSpecSharp},	 //返回特殊图形(目前就返回五角星)
	// zxzhang  装备鉴定几率  2010-1-8
	{"GetEquipQltyPct",					SC_GetEquipQltyPct},

    {"GetFashionSpinStep",              SC_GetFashionSpinStep},
    {"GetFashionSuiteType",             SC_GetFashionSuiteType},
	
	//more
	{"GetSkillName",					SC_GetSkillName},
	{"GetSkillDesc",					SC_GetSkillDesc},
	{"GetSkillType",					SC_GetSkillType},
	{"GetSkillClassLimit",				SC_GetSkillClassLimit},
	{"GetSkillUseType",					SC_GetSkillUseType},
	{"GetSkillSexLimit",				SC_GetSkillSexLimit},
	{"GetSkillPosLimit",				SC_GetSkillPosLimit},
	{"GetSkillMountLimit",				SC_GetSkillMountLimit},
	{"GetSkillIsLearn",					SC_GetSkillIsLearn},
	{"GetSkillLevelupType",				SC_GetSkillLevelupType},
	{"GetSkillPreSkill",				SC_GetSkillPreSkill},
	{"GetSkillLevelLimit",				SC_GetSkillLevelLimit},
	{"GetSkillTalentLimit",				SC_GetSkillTalentLimit},
	{"GetSkillDamageType",				SC_GetSkillDamageType},
	{"GetSkillLevel",					SC_GetSkillLevel},
    {"GetSkillMaxLevel",				SC_GetSkillMaxLevel},
	{"GetSkillExp",						SC_GetSkillExp},
	{"GetSkillDamage",					SC_GetSkillDamage},
	{"GetSkillOPDistance",				SC_GetSkillOPDistance},
	{"GetSkillPrepareTime",				SC_GetSkillPrepareTime},
	{"GetSkillCooldownTime",			SC_GetSkillCooldownTime},
	{"GetSkillAttribute",				SC_GetSkillAttribute},
	{"GetSkillAttributePct",			SC_GetSkillAttributePct},
	{"GetSkillCost",					SC_GetSkillCost},
	{"GetSkillNextLevel",				SC_GetSkillNextLevel},
	{"GetSkillDamageNum",				SC_GetSkillDamageNum},
	//more
	{"GetBuffName",						SC_GetBuffName},
	{"GetBuffDesc",						SC_GetBuffDesc},
	{"GetBuffType",						SC_GetBuffType},
	{"GetBuffRemainTime",				SC_GetBuffRemainTime},
	//more
	{"GetPriceSilver",					SC_GetPriceSilver},
	{"IsTax",							SC_IsTax},
	{"GetPriceSilverByTax",				SC_GetPriceSilverByTax},
	{"GetPriceSpecCost",				SC_GetPriceSpecCost},
    //more
    {"GetPetSkillType",                 SC_GetPetSkillType},
    {"GetPetSkillName",                 SC_GetPetSkillName},
    {"GetPetSkillDesc",                 SC_GetPetSkillDesc},
    {"GetPetSkillLevel",                SC_GetPetSkillLevel},
	// 商货
	{"GetCommerceName",					SC_GetCommerceName},
	{"GetCommerceType",					SC_GetCommerceType},
	{"GetCommerceDesc",					SC_GetCommerceDesc},
	// 帮派技能
	{"GetResearchGuildSkillCostFund",				SC_GetResearchGuildSkillCostFund},
	{"GetResearchGuildSkillCostMaterial",			SC_GetResearchGuildSkillCostMaterial},
	// 法宝
	{"GetFaBaoSpec",					SC_GetFaBaoSpec},
	{"GetIsFaBaoQianNengOpen",			SC_GetIsFaBaoQianNengOpen},
	// 飞升
	{"IsEquipSoaring",					SC_IsEquipSoaring},
	// 获得装备强化属性
	{"GetEqipConsolidateAtt",			SC_GetEqipConsolidateAtt},
	// 神铸
	{"Getsoar_limit",					SC_Getsoar_limit},
	{"Getequip_soar_level",				SC_Getequip_soar_level},
	{"IsCanXiHun",						SC_IsCanXiHun},
	{"GetEquipMaxStrengthenLevel",		SC_GetEquipMaxStrengthenLevel},

	// 妖精秘药药效
	{"GetFairyPower",					SC_GetFairyPower},
	{"GetProtoFairyPower",				SC_GetProtoFairyPower},
	//	领域技能
	{"GetAreaUseType",					SC_GetAreaUseType},
	{"GetAreaName",						SC_GetAreaName},
	{"GetAreaSkillLevel",				SC_GetAreaSkillLevel},
	{"GetAreaActiveLevel",				SC_GetAreaActiveLevel},
	{"GetAreaGodHeadLevel",				SC_GetAreaGodHeadLevel},
	{"GetAreaLeftTime",					SC_GetAreaLeftTime},
	{"GetAreaState",					SC_GetAreaState},
	{"GetAreaDamage",					SC_GetAreaDamage},
	{"GetAreaDamageNum",				SC_GetAreaDamageNum},
	{"GetAreaOPDistance",				SC_GetAreaOPDistance},
	{"GetAreaCooldownTime",				SC_GetAreaCooldownTime},
	{"GetAreaCost",						SC_GetAreaCost},
	{"GetAreaDesc",						SC_GetAreaDesc},
	{"GetAreaGetDesc",					SC_GetAreaGetDesc},
	{"GetAreaSkillShowLevel",			SC_GetAreaSkillShowLevel},
	// 装备成长
	{"GetIsGrowEquip",					SC_GetIsGrowEquip},			//是否是具有成长属性的装备
	{"GetEquipGrowExp",					SC_GetEquipGrowExp},		//获取装备经验
	{"GetEquipStarColor",				SC_GetEquipStarColor},		//获取星星颜色
	{"GetEquipGrowAtt",					SC_GetEquipGrowAtt},		//获取装备成长属性
	{"GetGrowEquipStrength",			SC_GetGrowEquipStrength},	//获取成长装备强度

	// 经脉
	{"GetPulseName",					SC_GetPulseName},
	{"GetPulseLevel",					SC_GetPulseLevel},
	{"GetPulseLevelLimit",					SC_GetPulseLevelLimit},
	{"GetPulseGodLevelLimit",					SC_GetPulseGodLevelLimit},
	{"GetPulseActiveState",					SC_GetPulseActiveState},
	{"GetPulseDesc",				SC_GetPulseDesc},
	{"GetPulseNextDesc",				SC_GetPulseNextDesc},

	// 魂晶
	{"GetHunJingSkillDesc",					SC_GetHunJingSkillDesc},
	{"GetHunJingAttDesc",					SC_GetHunJingAttDesc},
	{"GetHunJingSpec",					SC_GetHunJingSpec},
	{"GetGESkillNeedSkillName",					SC_GetGESkillNeedSkillName},
	{"GetHunJingLevel",					SC_GetHunJingLevel},
	{"GetHunJingSkillName",					SC_GetHunJingSkillName},
	// 宝石增幅
	{"GetDiamondAddLevel",				SC_GetDiamondAddLevel},
	//
	{"GetFabaoRoleAtt",					SC_GetFabaoRoleAtt},
	{"GetEquipRoleAtt",					SC_GetEquipRoleAtt},
	
	// 圣灵
	{"IsHolyMan",						SC_IsHolyMan},		//是否是圣灵
	{"GetHolyManAtt",					SC_GetHolyManAtt},	//获取圣灵属性
	{"IsHolyManEquip",					SC_IsHolyManEquip},	//是否是圣灵装备
	{"GetHolyEquipAtt",					SC_GetHolyEquipAtt},//获取圣灵装备属性
	{"GetShengWenChongLevel",			SC_GetShengWenChongLevel},	//获取圣纹充能等级
	{"GetGoodsHolyManAtt",				SC_GetGoodsHolyManAtt},		//获取商品圣灵属性
	{"GetGoodsHolyEquipAtt",			SC_GetGoodsHolyEquipAtt},	//获取商品圣纹属性

	{NULL, NULL}
};

//-----------------------------------------------------------------------------
// LUA OPEN SCRIPT LIB
// Registers all the basic script support functions with Lua.
//-----------------------------------------------------------------------------
VOID LuaOpenToolTipLib(lua_State*  l)
{
	// register our library of routines under the table name Script
	luaI_openlib(l, "Tooltip",	tooltipLib, 0);
}

static int SC_GetEqipConsolidateAtt(lua_State *L)
{
	Equipment* pEquip = (Equipment*) lua_touserdata(L, -1);
	INT nRet = 0;
	if( !P_VALID(pEquip) )
		return nRet;

	const tagItemProto* pProto = pEquip->GetProto();
	if( !P_VALID(pProto) )
		return nRet;

	if( !MIsEquipment(pProto->dwTypeID) )
		return nRet;

	const tagEquipSpec* pSpec = pEquip->GetEquip();
	if( !P_VALID(pSpec) )
		return nRet;

	// 法宝加属性
	bool bCreated = false;
	for(INT i=0; i< MAX_FCONSOLIDATE_NUM; ++i)	
	{
		if( ERA_Null < pSpec->ConsolidateEffect[i].eRoleAtt && ERA_End > pSpec->ConsolidateEffect[i].eRoleAtt )
		{
			if( !bCreated )
			{
				bCreated = true;
				lua_newtable(L);
			}
			lua_pushinteger(L, (lua_Integer)pSpec->ConsolidateEffect[i].eRoleAtt);
			lua_pushinteger(L, (lua_Integer)pSpec->ConsolidateEffect[i].nValue);
			lua_settable(L, -3);
		}
	}	
	nRet = bCreated ? 1 : 0;
	return nRet;
}