//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role_consolidate.cpp
// author: Wjiang
// actor:
// data: 
// last:
// brief: 装备强化
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../WorldDefine/msg_compose.h"
#include "../WorldDefine/specfunc_define.h"
#include "../WorldDefine/compose_define.h"
#include "../ServerDefine/consolidate_define.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "../ServerDefine/item_define.h"

#include "role.h"
#include "map.h"
#include "creature.h"
#include "item_creator.h"
#include "title_mgr.h"
#include <math.h>
#include "shielding_mgr.h"

//-----------------------------------------------------------------------------
// 铭纹
// Jason 2009-12-1 增加紫色品质材料的加成因子
// Jason 2009-12-4 修改：只有指定类型的材料，才计入影响系数统计
//-----------------------------------------------------------------------------
DWORD Role::PosyEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	return GT_INVALID;
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Posy) )
		return E_Consolidate_NPCCanNotPosy;

	// 找到强化数据
	const tagPosyProtoSer *pPosyProto = g_attRes.GetPosyProto(dwFormulaID);
	if(!P_VALID(pPosyProto))
		return E_Compose_Formula_Not_Exist;

	// 找到被强化装备
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// 检测铭纹次数
	if(pEquip->equipSpec.byPosyTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondPosyTime;

	// 检测强化配方是否合法
	if((pEquip->equipSpec.byPosyTimes + 1 ) != pPosyProto->byPosyTimes)
		return E_Consolidate_FormulaNotMatch;

	// 检查是否能强化配方对应的装备属性
	if(!g_attRes.IsPosyPos(pPosyProto->ePosyAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotPosy;

	// 检查装备潜力值是否足够
	if(pEquip->equipSpec.nPotVal < (INT)pPosyProto->nPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// 检查玩家金钱是否足够
	if(GetCurMgr().GetBagSilver() < pPosyProto->dwMoneyConsume)
		return E_Consolidate_NotEnough_Money;

	// 检测玩家材料是否足够(顺便计算总材料数量）
	INT nItemQualityNum[EIQ_End];
	ZeroMemory(nItemQualityNum, sizeof(nItemQualityNum));

	INT nTotleStuff = 0;

	for(INT n = 0; n < MAX_CONSOLIDATE_STUFF_QUANTITY; ++n)
	{
		if(pPosyProto->ConsolidateStuff[n].dwStuffID == GT_INVALID 
			&& pPosyProto->ConsolidateStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 

		if(!P_VALID(pItemStuff))
			return E_Consolidate_NotEnough_Stuff;

		// 检测材料64位ID是否重复
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Consolidate_NotEnough_Stuff;
		}

		if(pItemStuff->dwTypeID != pPosyProto->ConsolidateStuff[n].dwStuffID 
			&& pItemStuff->pProtoType->eStuffType != pPosyProto->ConsolidateStuff[n].eStuffType)
			return E_Consolidate_NotEnough_Stuff;

		if(pItemStuff->GetNum() < (INT)pPosyProto->ConsolidateStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;

		// Jason 必须是指定某种类型的材料，否则，不统计
		if( EST_Null != pPosyProto->ConsolidateStuff[n].eStuffType )
		{
			nItemQualityNum[pItemStuff->pProtoType->byQuality] += pPosyProto->ConsolidateStuff[n].dwStuffNum;
			nTotleStuff += pPosyProto->ConsolidateStuff[n].dwStuffNum;
		}
	}

	// 计算IM道具的影响
	tagIMEffect			IMEffect;
	tagPosyProtoSer *pProto = const_cast<tagPosyProtoSer*>(pPosyProto);		
	CalIMEffect(ECTS_Posy, IMEffect, n64IMID, pProto);

	// 输入材料消耗
	for(INT n = 0; n < nArraySz; ++n)
	{
		 GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Posy, (INT16)pPosyProto->ConsolidateStuff[n].dwStuffNum);
	}

	// 金钱消耗
	GetCurMgr().DecBagSilver(pPosyProto->dwMoneyConsume, ELCID_Compose_Posy);

	// 计算成功率
	// 最终成功率=基础成功率+（-10%×白品材料数量/总材料数量+0%×黄品材料数量/总材料数量+5%×绿品材料数量
	// /总材料数量+10%×蓝品材料数量/总材料数量+20%×橙品材料数量/总材料数量）×[1+（装备等级-75）/150]
	// +（角色福缘/1000）+IM道具加成                                      最终成功率的取值为0%-100%
	FLOAT fProp = 0;
	if( nTotleStuff )
		fProp = (FLOAT)pPosyProto->nSuccessRate + (((-0.1f * (FLOAT)nItemQualityNum[EIQ_White] 
				+ 0.05f * (FLOAT)nItemQualityNum[EIQ_Green] + 0.1f * (FLOAT)nItemQualityNum[EIQ_Blue] 
				+ 0.2f * (FLOAT)nItemQualityNum[EIQ_Orange] + 0.5 * (FLOAT)nItemQualityNum[EIQ_Purple] ) / (FLOAT)nTotleStuff) 
				* (1.0f + ((FLOAT)pEquip->pProtoType->byLevel - 75.0f) / 150.0f) + ((FLOAT) GetAttValue(ERA_Fortune) / 1000.0f)) * 10000;

	// 计算B类属性对成功率影响
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_Posy);

	// 检测玩家是否用了提高成功率的IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)			// 成功
	{
		// 称号事件触发
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);
		
		//------------------------------------------------------------------------------
		// 铭纹 装备强化事件触发
		// zxzhang  2009-12-29
		//------------------------------------------------------------------------------
		GetTitleMgr()->SigEvent( ETE_STRENGTHEN_SKILL_SUCCESS,static_cast<DWORD>(ETEST_Posy),GT_INVALID);
		
		// 装备铭纹次数加一
		pEquip->equipSpec.byPosyTimes++;

		// 增加装备对应强化值
		// 加值=（装备等级×fcoefficientA + fcoefficientB）/ fcoefficientC ×（1+材料品质加成）
		/* 材料品质加成=（-20%×白品材料数量/总材料数量+0%×黄品材料数量/总材料数量+20%
		   ×绿品材料数量/总材料数量+50%×蓝品材料数量/总材料数量+100%×橙品材料数量/总材料数量）
		   材料品质加成的取值为0%-100%  */

		// 基本加成
		FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pPosyProto->fcoefficientA 
							+ pPosyProto->fcoefficientB) / pPosyProto->fcoefficientC;

		// 材料品质加成
		FLOAT fStuffAttInc = 0;
		if( nTotleStuff )
			fStuffAttInc = (-0.3f * (FLOAT)nItemQualityNum[EIQ_White] + 0.5f * (FLOAT)nItemQualityNum[EIQ_Green] 
							  + (FLOAT)nItemQualityNum[EIQ_Blue] + 2.0f * (FLOAT)nItemQualityNum[EIQ_Orange] 
							  + 3.0 * (FLOAT)nItemQualityNum[EIQ_Purple] )
							  / (FLOAT)nTotleStuff; 

		//if(fStuffAttInc < 0.0f)
		//	fStuffAttInc = 0.0f;
		//if(fStuffAttInc > 1.0f)
		//	fStuffAttInc = 1.0f;

		// 属性转换 
		ERoleAttribute eRoleAtt = ERA_Null;
		ConsolidatePosyAttConvert(eRoleAtt, pPosyProto->ePosyAtt);

		for(INT n = 0; n < MAX_ROLEATT_POSY_EFFECT; ++n)
		{
			if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == eRoleAtt)
			{
				pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + fStuffAttInc));
				break;
			}
			else if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == ERA_Null)
			{
				pEquip->equipSpec.PosyEffect[n].eRoleAtt = eRoleAtt;
				pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + fStuffAttInc));
				break;
			}
		}

		// 潜力值消耗
		// 生产完美率=生产成功率×1/20+角色福缘/1000
		FLOAT fPefectProp = (fProp * 0.05f) / 10000.0f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// 计算B类属性对完美率影响
		fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// 计算装备光晕
		CalEquipFlare(pEquip);

		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// 消耗装备潜力值为铭纹成功的75%
			pEquip->ChangePotVal(-pPosyProto->nPotValConsume * 75 / 100);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			pEquip->ChangePotVal(-pPosyProto->nPotValConsume);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else				// 失败
	{
		// 装备潜力值消耗5点
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal(-20/*5*/); // Jason 2009-11-28

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// GM铭纹
//-----------------------------------------------------------------------------
DWORD Role::GMPosyEquip(DWORD dwFormulaID, INT16 n16ItemIndex)
{
	return GT_INVALID;
	// 找到强化数据
	const tagPosyProtoSer *pPosyProto = g_attRes.GetPosyProto(dwFormulaID);
	if(!P_VALID(pPosyProto))
		return E_Compose_Formula_Not_Exist;

	// 找到被强化装备
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n16ItemIndex);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// 检测铭纹次数
	if(pEquip->equipSpec.byPosyTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondPosyTime;

	// 检测强化配方是否合法
	if((pEquip->equipSpec.byPosyTimes + 1 ) != pPosyProto->byPosyTimes)
		return E_Consolidate_FormulaNotMatch;

	// 检查是否能强化配方对应的装备属性
	if(!g_attRes.IsPosyPos(pPosyProto->ePosyAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotPosy;

	// 装备铭纹次数加一
	pEquip->equipSpec.byPosyTimes++;

	// 增加装备对应强化值
	// 加值=（装备等级×fcoefficientA + fcoefficientB）/ fcoefficientC ×（1+材料品质加成）

	// 基本加成
	FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pPosyProto->fcoefficientA 
		+ pPosyProto->fcoefficientB) / pPosyProto->fcoefficientC;

	// 属性转换 
	ERoleAttribute eRoleAtt = ERA_Null;
	ConsolidatePosyAttConvert(eRoleAtt, pPosyProto->ePosyAtt);

	for(INT n = 0; n < MAX_ROLEATT_POSY_EFFECT; ++n)
	{
		if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == eRoleAtt)
		{
			pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + 0));
			break;
		}
		else if(pEquip->equipSpec.PosyEffect[n].eRoleAtt == ERA_Null)
		{
			pEquip->equipSpec.PosyEffect[n].eRoleAtt = eRoleAtt;
			pEquip->equipSpec.PosyEffect[n].nValue += (INT32)(fBaseAttInc * (1.0f + 0));
			break;
		}
	}

	// 计算装备光晕
	CalEquipFlare(pEquip);
		
	GetItemMgr().UpdateEquipSpec(*pEquip);
	return E_Compose_Consolidate_Success;
}

//-----------------------------------------------------------------------------
// 镌刻
// Jason 2009-12-1 增加紫色品级材料影响
// Jason 2009-12-4 只统计材料类型明确的材料
//-----------------------------------------------------------------------------
DWORD Role::EngraveEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	return GT_INVALID;
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Engrave) )
		return E_Compose_NPCCanNotEngrave;

	// 找到强化数据
	const tagEngraveProtoSer *pEngraveProto = g_attRes.GetEngraveProto(dwFormulaID);
	if(!P_VALID(pEngraveProto))
		return E_Compose_Formula_Not_Exist;

	// 找到被强化装备
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_EquipCanNotEngrave;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// 检测镌刻次数
	if(pEquip->equipSpec.byEngraveTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondEngraveTime;

	// 检测强化配方是否合法
	if((pEquip->equipSpec.byEngraveTimes + 1 ) != pEngraveProto->byEngraveTimes)
		return E_Consolidate_FormulaNotMatch;

	// 检查是否能强化配方对应的装备属性
	if(!g_attRes.IsEngravePos(pEngraveProto->eEngraveAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotEngrave;

	// 检查装备潜力值是否足够
	if(pEquip->equipSpec.nPotVal < (INT)pEngraveProto->nPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// 检查玩家金钱是否足够
	if(GetCurMgr().GetBagSilver() < pEngraveProto->dwMoneyConsume)
		return E_Consolidate_NotEnough_Money;

	// 检测玩家材料是否足够(顺便计算总材料数量）
	INT nItemQualityNum[EIQ_End];
	ZeroMemory(nItemQualityNum, sizeof(nItemQualityNum));

	INT nTotleStuff = 0;

	for(INT n = 0; n < MAX_CONSOLIDATE_STUFF_QUANTITY; ++n)
	{
		if(pEngraveProto->ConsolidateStuff[n].dwStuffID == GT_INVALID 
			&& pEngraveProto->ConsolidateStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 

		if(!P_VALID(pItemStuff))
			return E_Consolidate_NotEnough_Stuff;

		// 检测材料64位ID是否重复
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Consolidate_NotEnough_Stuff;
		}

		if(pItemStuff->dwTypeID != pEngraveProto->ConsolidateStuff[n].dwStuffID 
			&& pItemStuff->pProtoType->eStuffType != pEngraveProto->ConsolidateStuff[n].eStuffType)
			return E_Consolidate_NotEnough_Stuff;

		if(pItemStuff->GetNum() < (INT)pEngraveProto->ConsolidateStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;

		if( EST_Null != pEngraveProto->ConsolidateStuff[n].eStuffType )
		{
			nItemQualityNum[pItemStuff->pProtoType->byQuality] += pEngraveProto->ConsolidateStuff[n].dwStuffNum;
			nTotleStuff += pEngraveProto->ConsolidateStuff[n].dwStuffNum;
		}
	}

	// 计算IM道具的影响
	tagIMEffect			IMEffect;
	tagEngraveProtoSer  *pProto = const_cast<tagEngraveProtoSer*>(pEngraveProto);
	CalIMEffect(ECTS_Engrave, IMEffect, n64IMID, pProto);

	// 输入材料消耗
	for(INT n = 0; n < nArraySz; ++n)
	{
		GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Engrave, (INT16)pEngraveProto->ConsolidateStuff[n].dwStuffNum);
	}

	// 金钱消耗
	GetCurMgr().DecBagSilver(pEngraveProto->dwMoneyConsume, ELCID_Compose_Engrave);

	// 计算成功率
	// 最终成功率=基础成功率+（-10%×白品材料数量/总材料数量+0%×黄品材料数量/总材料数量+5%×绿品材料数量
	// /总材料数量+10%×蓝品材料数量/总材料数量+20%×橙品材料数量/总材料数量）×[1+（装备等级-75）/150]
	// +（角色福缘/1000）+IM道具加成                                      最终成功率的取值为0%-100%
	FLOAT fProp = 0;
	if( nTotleStuff )
		fProp = (FLOAT)pEngraveProto->nSuccessRate + (((-0.1f * (FLOAT)nItemQualityNum[EIQ_White] 
				+ 0.05f * (FLOAT)nItemQualityNum[EIQ_Green] + 0.1f * (FLOAT)nItemQualityNum[EIQ_Blue] 
				+ 0.2f * (FLOAT)nItemQualityNum[EIQ_Orange] + 0.5 * (FLOAT)nItemQualityNum[EIQ_Purple] ) / (FLOAT)nTotleStuff) 
				* (1.0f + ((FLOAT)pEquip->pProtoType->byLevel - 75.0f) / 150.0f) 
				+ ((FLOAT) GetAttValue(ERA_Fortune) / 1000.0f)) * 10000;

	// 计算B类属性对成功率影响
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_Engrave);

	// 检测玩家是否用了提高成功率的IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)			// 成功
	{
		// 称号事件触发
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);

	

		//--------------------------------------------------------------------------------------------
        // 镌刻 装备强化事件触发
		// zxzhang 2009-12-29
		//--------------------------------------------------------------------------------------------
        GetTitleMgr()->SigEvent( ETE_STRENGTHEN_SKILL_SUCCESS,static_cast<WORD>(ETEST_Engrave), GT_INVALID); 

 		// 装备铭纹次数加一
		pEquip->equipSpec.byEngraveTimes++;

		// 增加装备对应强化值
		// 加值=（装备等级×fcoefficientA + fcoefficientB）/ fcoefficientC ×（1+材料品质加成）
		/* 材料品质加成=（-20%×白品材料数量/总材料数量+0%×黄品材料数量/总材料数量+20%
		×绿品材料数量/总材料数量+50%×蓝品材料数量/总材料数量+100%×橙品材料数量/总材料数量）
		材料品质加成的取值为0%-50%  */

		// 基本加成
		FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pEngraveProto->fcoefficientA 
			+ pEngraveProto->fcoefficientB) / pEngraveProto->fcoefficientC;

		// 材料品质加成
		FLOAT fStuffAttInc = 0;
		if( nTotleStuff )
			fStuffAttInc = (-0.3f * (FLOAT)nItemQualityNum[EIQ_White] + 0.5f * (FLOAT)nItemQualityNum[EIQ_Green] 
							  + (FLOAT)nItemQualityNum[EIQ_Blue] + 2.0f * (FLOAT)nItemQualityNum[EIQ_Orange] 
							  + 3.0f * (FLOAT)nItemQualityNum[EIQ_Purple] )
							  / (FLOAT)nTotleStuff; 

		//if(fStuffAttInc < 0.0f)
		//	fStuffAttInc = 0.0f;
		//if(fStuffAttInc > 1.0f)
		//	fStuffAttInc = 1.0f;

		// 最终加成
		INT nEngraveAttInc = (INT)(fBaseAttInc * (1.0f + fStuffAttInc));

		switch(pEngraveProto->eEngraveAtt)
		{
		case EEngraveAtt_WeaponDmg:
			pEquip->equipSpec.nEngraveAtt[0] += nEngraveAttInc;
			pEquip->equipSpec.nEngraveAtt[1] += nEngraveAttInc;
			break;
		case EEngraveAtt_WeaponSoul:
			pEquip->equipSpec.nEngraveAtt[2] += nEngraveAttInc;
			break;
		case EEngraveAtt_Armor:
			pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
			break;
		case EEngraveAtt_Deration:
			pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
			break;
		default:
			break;
		}

		// 潜力值消耗
		// 生产完美率=生产成功率×1/20+角色福缘/1000
		FLOAT fPefectProp = (fProp * 0.05f) / 10000.0f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// 计算B类属性对完美率影响
		fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// 计算装备光晕
		CalEquipFlare(pEquip);

		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// 消耗装备潜力值为铭纹成功的75%
			pEquip->ChangePotVal(-pEngraveProto->nPotValConsume * 75 / 100);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			pEquip->ChangePotVal(-pEngraveProto->nPotValConsume);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else				// 失败
	{
		// 装备潜力值消耗5点
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal(-20/*5*/); //Jason 2009-11-28

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// GM 镌刻
//-----------------------------------------------------------------------------
DWORD Role::GMEngraveEquip(DWORD dwFormulaID, INT16 n16ItemIndex)
{
	return GT_INVALID;
	// 找到强化数据
	const tagEngraveProtoSer *pEngraveProto = g_attRes.GetEngraveProto(dwFormulaID);
	if(!P_VALID(pEngraveProto))
		return E_Compose_Formula_Not_Exist;

	// 找到被强化装备
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n16ItemIndex);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_EquipCanNotEngrave;

	// 检测镌刻次数
	if(pEquip->equipSpec.byEngraveTimes >= MAX_ENGRAVE_TIMES)
		return E_Consolidate_BeyondEngraveTime;

	// 检测强化配方是否合法
	if((pEquip->equipSpec.byEngraveTimes + 1 ) != pEngraveProto->byEngraveTimes)
		return E_Consolidate_FormulaNotMatch;

	// 检查是否能强化配方对应的装备属性
	if(!g_attRes.IsEngravePos(pEngraveProto->eEngraveAtt, pEquip->pEquipProto->eEquipPos))
		return E_Consolidate_EquipCanNotEngrave;

	// 装备铭纹次数加一
	pEquip->equipSpec.byEngraveTimes++;

	// 增加装备对应强化值
	// 加值=（装备等级×fcoefficientA + fcoefficientB）/ fcoefficientC ×（1+材料品质加成）

	// 基本加成
	FLOAT fBaseAttInc = ((FLOAT)pEquip->pProtoType->byLevel * pEngraveProto->fcoefficientA 
		+ pEngraveProto->fcoefficientB) / pEngraveProto->fcoefficientC;

	// 最终加成
	INT nEngraveAttInc = (INT)(fBaseAttInc * (1.0f + 0));

	switch(pEngraveProto->eEngraveAtt)
	{
	case EEngraveAtt_WeaponDmg:
		pEquip->equipSpec.nEngraveAtt[0] += nEngraveAttInc;
		pEquip->equipSpec.nEngraveAtt[1] += nEngraveAttInc;
		break;
	case EEngraveAtt_WeaponSoul:
		pEquip->equipSpec.nEngraveAtt[2] += nEngraveAttInc;
		break;
	case EEngraveAtt_Armor:
		pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
		break;
	case EEngraveAtt_Deration:
		pEquip->equipSpec.nEngraveAtt[3] += nEngraveAttInc;
		break;
	default:
		break;
	}
	
	// 计算装备光晕
	CalEquipFlare(pEquip);

	GetItemMgr().UpdateEquipSpec(*pEquip);
	return E_Compose_Consolidate_Success;
}

//-----------------------------------------------------------------------------
// 镶嵌
//-----------------------------------------------------------------------------
DWORD Role::InlayEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID)
{
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// 检查宝石是否被锁定
	tagItem *pItemGem = GetItemMgr().GetBagItem(n64SrcItemID);
	if (!P_VALID(pItemGem))
	{
		return E_Consolidate_Gem_Not_Exit;
	}
	else
	{
		if (pItemGem->bLock)
		{
			return E_Consolidate_Gem_Locked;
		}
	}

	BOOL bEquiped = FALSE;

	// 找到被强化装备
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(P_VALID(pEquip))
	{
		// 背包
		bEquiped = FALSE;
	}
	else
	{
		pEquip = GetItemMgr().GetEquipBarEquip(n64DstItemID);
		if(P_VALID(pEquip))
		{
			// 装备栏
			bEquiped = TRUE;
		}
		else
		{
			return E_Consolidate_Equip_Not_Exist;
		}
	}

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	// Jason 2010-7-16 v1.0.0
	//if( MIsFaBao(pEquip) )
	//{
	//	if( pItemGem->pProtoType->nSpecFuncVal1 > ((tagFabao*)pEquip)->nUseLevel )
	//		return E_ConsolidateInlay_EquipLevelLower;
	//}
	//else
	if( pItemGem->pProtoType->nSpecFuncVal1 > pEquip->pEquipProto->byLevel )
		return E_ConsolidateInlay_EquipLevelLower;

	// 找到强化数据
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemGem->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	if((INT)pEquip->equipSpec.byHoleNum == 0)
		return E_Consolidate_Gem_Not_Hole;

	// 检测是否宝石已经镶满
	//if(pEquip->equipSpec.dwHoleGemID[pEquip->equipSpec.byHoleNum - 1] != GT_INVALID 
	//	&& pEquip->equipSpec.dwHoleGemID[pEquip->equipSpec.byHoleNum - 1] != 0 )
	bool bFull = true;
	for(int i = 0; i < pEquip->equipSpec.byHoleNum;++i)
	{
		if( !P_VALID(pEquip->equipSpec.dwHoleGemID[i]) )
		{
			bFull = false;
			break;
		}
	}
	if( bFull )
		return E_Consolidate_Gem_Full;

	// 检测宝石是否能镶嵌到装备上
	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotInlay;

	// 检查装备潜力值是否足够
	//if(pEquip->equipSpec.nPotVal < (INT)pConsolidateProto->dwPotValConsume)
	//	return E_Consolidate_ValConsume_Inadequate;

	if(bEquiped)
	{
		SaveTheValueBeforeStrengthen();
		ProcEquipEffect(NULL, pEquip, pEquip->n16Index, TRUE, FALSE);
	}

	// 镶嵌宝石
	for(INT i = 0; i < (INT)pEquip->equipSpec.byHoleNum; ++i)
	{
		if(pEquip->equipSpec.dwHoleGemID[i] == GT_INVALID || pEquip->equipSpec.dwHoleGemID[i] == 0)
		{
			pEquip->equipSpec.dwHoleGemID[i] = pItemGem->dwTypeID;
			break;
		}
	}

	// 根据宝石的绑定状态修改非绑定的装备为绑定
	if ( pItemGem->IsBind() || pItemGem->IsAccountBind() )
	{
		if ( !pEquip->IsBind() )
		{
			pEquip->byBind = EBS_Bind;
			// 改变装备的绑定状态后通知DB和客户端
			pEquip->SetUpdate(EUDBS_Update);
			GetItemMgr().SendItemBindStatus(pEquip->n64Serial, pEquip->eConType, pEquip->byBind);
		}		
	}

	// 计算装备光晕
	CalEquipFlare(pEquip);

	// 消耗装备潜力值
	//pEquip->ChangePotVal(-(INT)pConsolidateProto->dwPotValConsume);
	GetItemMgr().UpdateEquipSpec(*pEquip);

	if(bEquiped)
	{
		ProcEquipEffect(pEquip, NULL, pEquip->n16Index, TRUE, TRUE);
		RestoreTheValueAfterStrengthen();
	}

	// 删除材料
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Compose_Enchase, 1);

	// 称号事件触发
	GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);


	//-----------------------------------------------------------------------------------------------------
	// 镶嵌 装备称号事件触发 
	// zxzhang 2009-12-29
	//-----------------------------------------------------------------------------------------------------
	GetTitleMgr()->SigEvent( ETE_STRENGTHEN_SKILL_SUCCESS,static_cast<DWORD>(ETEST_Inlay),GT_INVALID);
	return E_Compose_Consolidate_Success;	
}

//-----------------------------------------------------------------------------
// 开凿
//-----------------------------------------------------------------------------
DWORD Role::ChiselEquip(INT64 n64SrcItemID, INT64 n64SuffID, DWORD dwCmdID)
{
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	BOOL bEquipd = FALSE;

	// 找到被强化装备
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64SrcItemID);
	if (P_VALID(pEquip))
	{
		bEquipd = FALSE;
	}
	else
	{
		pEquip = (tagEquip*)GetItemMgr().GetEquipBarEquip(n64SrcItemID);
		if (P_VALID(pEquip))
		{
			bEquipd = TRUE;
		}
		else
		{
			return E_Consolidate_Equip_Not_Exist;
		}

	}	

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if(!MIsIdentified(pEquip))
		return E_Consolidate_NotIdentified;

	if(pEquip->equipSpec.bCanCut == false)
		return E_Consolidate_Equip_CanNot_Chisel;

	// 找到凿石
	tagItem *pItemChisel = GetItemMgr().GetBagItem(n64SuffID);
	if(!P_VALID(pItemChisel) )
	{
		return E_Consolidate_Chisel_Not_Exit;		
	}

	// 开启5个以下的孔必须用 原有凿石
	if(pEquip->equipSpec.byHoleNum < 5 && pItemChisel->pProtoType->eSpecFunc != EISF_Chisel )
	{
		return E_Consolidate_Chisel_Not_Exit;	
	}

	// 开启5个以上的孔必须用 新凿石
	if(pEquip->equipSpec.byHoleNum >= 5 && pEquip->equipSpec.byHoleNum < MAX_EQUIPHOLE_NUM && pItemChisel->pProtoType->eSpecFunc != EISF_NewChisel )
	{
		return E_Consolidate_Chisel_Not_Exit;	
	}

	// 等级小于所开凿的装备等级，则不可进行开凿
	if(pEquip->pProtoType->byLevel > (pItemChisel->pProtoType->byLevel * 10))
		return E_Consolidate_Chisel_Not_Match;

	// 计算成功率
	FLOAT fProp = 0.0;

	if (pItemChisel->pProtoType->eSpecFunc == EISF_Chisel)
	{
		// 检测装备镶嵌空数
		if(pEquip->equipSpec.byHoleNum >= 5)
			return E_Consolidate_Equip_Hole_Full;


		// 计算成功率(开凿最终成功率=凿石基础成功率×（1-装备当前镶嵌孔数量/5）+（角色福缘/1000）)
		fProp = (((FLOAT)pItemChisel->pProtoType->nSpecFuncVal1 * (pow( 1.0f - (FLOAT)pEquip->equipSpec.byHoleNum / 5.0f,1.5f)) ) / 10000.0f 
			+ (FLOAT)GetAttValue(ERA_Fortune) / 5000.0f) *10000.0f;

		// 计算B类属性对成功率影响
		fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_Cut);
	}

	if (pItemChisel->pProtoType->eSpecFunc == EISF_NewChisel)
	{
		// 检测装备镶嵌空数
		if(pEquip->equipSpec.byHoleNum >= MAX_EQUIPHOLE_NUM)
			return E_Consolidate_Equip_Hole_Full;


		// 新凿石，开孔的成功率为100%
		fProp = 10000.0f;	
	}


	// 材料消耗
	GetItemMgr().DelFromBag(n64SuffID, (DWORD)ELCID_Compose_Chisel, 1);

	

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)		// 成功
	{

		//----------------------------------------------------------------------------------------------
		// 强化所有装备 称号事件触发
		// zxzhang 2009-12-29
		//----------------------------------------------------------------------------------------------
        GetTitleMgr()->SigEvent( ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID );
		 
		//----------------------------------------------------------------------------------------------
		//开凿 称号事件触发
		// zxzhang 2009-12-29
		//----------------------------------------------------------------------------------------------
		GetTitleMgr()->SigEvent( ETE_STRENGTHEN_SKILL_SUCCESS, static_cast<DWORD>(ETEST_Chisel), GT_INVALID );

		// Jason 2010-6-12 去掉完美率
		// 计算完美率
		// 完美率=1%+（角色福缘/1000）【取值在0-100%】
		//FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// 计算B类属性对完美率影响
		//fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		//BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		//if(bPefect)
		//{
		//	pEquip->equipSpec.byHoleNum = ((pEquip->equipSpec.byHoleNum + 2 > MAX_EQUIPHOLE_NUM) ? MAX_EQUIPHOLE_NUM : (pEquip->equipSpec.byHoleNum + 2));
		//	GetItemMgr().UpdateEquipSpec(*pEquip);
		//	return  E_Compose_Consolidate_Perfect;
		//}
		//else
		{
			++(pEquip->equipSpec.byHoleNum);
			GetItemMgr().UpdateEquipSpec(*pEquip);
			// 触发脚本
			const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
			if (P_VALID(pRoleScript))
				pRoleScript->OnDigHoleOnEquip(this, pEquip->dwTypeID, bResult, pEquip->equipSpec.byHoleNum);
			return E_Compose_Consolidate_Success;
		}
	}

	// 触发脚本
	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if (P_VALID(pRoleScript))
		pRoleScript->OnDigHoleOnEquip(this, pEquip->dwTypeID, bResult, pEquip->equipSpec.byHoleNum);

	return E_Compose_Consolidate_Lose;
}

//-----------------------------------------------------------------------------
//得到精纺阶段
//-----------------------------------------------------------------------------
INT Role::GetSpinStep(BYTE byMinUseLevel)
{
	switch(byMinUseLevel)
	{
	case ESSEL_Null:
		return ESpinStep_Null;
	case ESSEL_Basic:
		return ESpinStep_Basic;
	case ESSEL_Middle:
		return ESpinStep_Middle;
	case ESSEL_Senior:
		return ESpinStep_Senior;
	default:
		return ESpinStep_Null;
	}

	return ESpinStep_Null;
}

//-----------------------------------------------------------------------------
//得到精纺装备的品级
//-----------------------------------------------------------------------------
EItemQuality Role::GetSpinItemQuality(ESpinStep eStep)
{
	switch(eStep)
	{
	case ESpinStep_Null:
		return EIQ_White;

	case ESpinStep_Basic:
		return EIQ_Green;

	case ESpinStep_Middle:
		return EIQ_Blue;

	case ESpinStep_Senior:
		return EIQ_Orange;

	default:
		return EIQ_White;
	}

	return EIQ_White;
}
//-----------------------------------------------------------------------------
// 精纺时装
//-----------------------------------------------------------------------------
DWORD Role::SpinFashion(ESuitAttType eSpinType, INT64 n64EquipSerial, DWORD dwStuffTypeID,  DWORD dwStuffItemNum, DWORD dwCmdID)
{

	DWORD dwErrCode = E_Compose_Consolidate_Success;

	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	//得到精纺的成功率
	const tagSpinSuccessRateProto *pSuccessRate = g_attRes.GetSpinRateProto(dwStuffItemNum);
	if(!P_VALID(pSuccessRate))
		return GT_INVALID;

	if(eSpinType >= ESSAT_End || eSpinType < ESSAT_Null )
		return GT_INVALID;

	//////////////////////////////////////////////////////////////////////////


	//得到要精纺的时装
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64EquipSerial);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_CanNotSpin;

	//是否为可精纺时装
	if(pEquip->pEquipProto->eTypeReserved != EITR_Spin)
		return E_Consolidate_CanNotSpin;

	//取精纺用的物品原型 验证特殊使用类型是否为EISF_SpinFashionDressItem
	tagItemProto* pStuffItem = g_attRes.GetItemProto(dwStuffTypeID);
	if(!P_VALID(pStuffItem) || pStuffItem->eSpecFunc != EISF_SpinFashionDressItem)
		return GT_INVALID;

	//判断个数
	if(dwStuffItemNum > GetItemMgr().GetBagSameItemCount(dwStuffTypeID))
		return E_Compose_Stuff_Not_Enough;


	//得到该服装处于精纺的哪个阶段
	INT nStep = this->GetSpinStep(pEquip->pEquipProto->byMinUseLevel);

	//看看是否可以再精纺
	if(++nStep == ESpinStep_End)
		return E_Consolidate_Spin_TopLevel;

//////////////////////////////////////////////////////////////////////////

	//寻找当前物品的精纺原型
	const tagSpinProto *pSpinProto = NULL;
	for(INT nLoop = ESSAT_Brave; nLoop != ESSAT_End; nLoop++)
	{
		//得到精纺原型
		pSpinProto = g_attRes.GetSpinProto(tagSpinKey(pEquip->dwTypeID,(ESuitAttType)nLoop));
		if(P_VALID(pSpinProto))
			break;
	}
	if(!P_VALID(pSpinProto))
		return E_Consolidate_Equip_Not_Exist;

	//根据未精纺物品的typeid和目标精纺属性类型 取目标精纺原型
	const tagSpinProto *pSpinAfterProto = g_attRes.GetSpinProto(tagSpinKey(pSpinProto->dwSpinFashionTypeID[ESpinStep_Null],eSpinType));
	if(!P_VALID(pSpinAfterProto))
		return E_Consolidate_Equip_Not_Exist;

//////////////////////////////////////////////////////////////////////////
	if( nStep < ESpinStep_Null || nStep >= ESpinStep_End)
		return GT_INVALID;
	//判断是否成功
	BOOL bPefect = IUTIL->Rand() % 10000 <= pSuccessRate->dwSuccessRate[nStep];

//////////////////////////////////////////////////////////////////////////
	//找升级后的装备原型
	tagEquipProto *pSpinAfterEquipProto = g_attRes.GetEquipProto(pSpinAfterProto->dwSpinFashionTypeID[nStep]);
	if(!P_VALID(pSpinAfterEquipProto))
		return GT_INVALID;
/*
	//如果策划表加个suitid会比较快
	//找对应的套装属性的套装id
	const tagSuitProto* pSuitProto = NULL;
	BOOL bFinded = FALSE;
	for(INT nSuitLoop = 0; nSuitLoop<MAX_PEREQUIP_SUIT_NUM; ++nSuitLoop)
	{
		const tagSuitProto* pSuitProto = g_attRes.GetSuitProto(pSpinAfterEquipProto->dwSuitID[nSuitLoop]);
		if(pSpinAfterEquipProto->dwSuitID[nSuitLoop] == GT_INVALID || !P_VALID(pSuitProto))
			continue;

		//套装属性类型相同
		if(pSpinAfterProto->eAttType != ESSAT_Null && pSuitProto->eAttType == pSpinAfterProto->eAttType)
		{
			bFinded = TRUE;
			break;
		}
	}
	
//////////////////////////////////////////////////////////////////////////
	if(!P_VALID(pSuitProto) || bFinded == FALSE)
		return GT_INVALID;
*/
	//成功的话
	if(bPefect == TRUE)
	{
		 //删除被强化物品
		GetItemMgr().DelFromBag(n64EquipSerial, (DWORD)ELCLD_Compose_Spin, 1);

		EItemQuality eQuality = this->GetSpinItemQuality((ESpinStep )nStep);
		//给个升级后新的
		tagItem *pEquip = ItemCreator::CreateEx(EICM_Spin, GT_INVALID, pSpinAfterProto->dwSpinFashionTypeID[nStep], 1, eQuality);

		//改基础属性
		if(!P_VALID(pEquip))
			goto SPIN_END_LINE;

		if(GetItemMgr().Add2Bag(pEquip, ELCLD_Compose_Spin, TRUE, TRUE) != E_Success)
		{
			Destroy(pEquip);
			return E_Compose_Consolidate_Lose;
		}

	}
	//失败
	else
	{
		dwErrCode = E_Compose_Consolidate_Lose;
	}

SPIN_END_LINE:

	//删除材料
	GetItemMgr().RemoveFromRole(dwStuffTypeID, dwStuffItemNum, ELCLD_Compose_Spin );

	return dwErrCode;
}

//-----------------------------------------------------------------------------
// 时装染色
//-----------------------------------------------------------------------------
DWORD Role::DyeFashion(INT64 n64DyeSN, INT64 n64EquipSerial, DWORD dwCmdID)
{
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// 找到被染色时装
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64EquipSerial);
	if(!P_VALID(pEquip))
		return E_Dye_NotExist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Dye_OtherItem;

	if(!MIsFashion(pEquip))
		return E_Dye_OtherItem;

	if( MIsFaBao(pEquip) )
		return E_Dye_OtherItem;

	if (pEquip->pEquipProto->bCanDye == false)
		return E_Dye_OtherItem;

	// 找到染料
	tagItem *pItemDye = GetItemMgr().GetBagItem(n64DyeSN);
	if(!P_VALID(pItemDye) || pItemDye->pProtoType->eSpecFunc != EISF_Dye )
	{
		return E_Dye_NotExist;
	}

	// 检测颜色是否相同
	if(pEquip->equipSpec.n8ColorID == pItemDye->pProtoType->nSpecFuncVal1)
		return E_Dye_Exist;

	// 等级小于所染色的装备等级，则不可进行染色
	if(pEquip->pProtoType->byLevel > (pItemDye->pProtoType->byLevel * 10))
		return E_Dye_LvLimit;

	// 改变时装颜色
	pEquip->equipSpec.n8ColorID = pItemDye->pProtoType->nSpecFuncVal1;
	GetItemMgr().UpdateEquipSpec(*pEquip);

	// 材料消耗
	GetItemMgr().DelFromBag(n64DyeSN, (DWORD)ELCID_Compose_Dye, 1);

	//---------------------------------------------------------------------------
	// 时装染色称号事件触发
	// zxzhang 2009-12-29 
    //---------------------------------------------------------------------------
	GetTitleMgr()->SigEvent( ETE_CLOTHCOLOR,GT_INVALID ,GT_INVALID );

	return E_Success;
}

//-----------------------------------------------------------------------------
// 计算B类属性对强化成功率的影响
//-----------------------------------------------------------------------------
FLOAT Role::CalSpecAttEffectSuc(EEquipSpecAtt eSpecAtt, FLOAT fProp, EEquipSpecAtt eSpecType)
{
	switch (eSpecAtt)
	{
	case EESA_Guard_Posy:
	case EESA_Guard_Brand:
	case EESA_Guard_Engrave:
	case EESA_Guard_LongFu:
	case EESA_Guard_Quench:
	case EESA_Guard_Cut:
		{
			if(eSpecAtt != eSpecType)
				return fProp;
			else
				return 10000.0f;
		}
	case EESA_Guard_AllProduce:
		return 10000.0f;
		break;
	case EESA_Guard_Talisman:
		return fProp * 1.05f;
		break;
	default:
		return fProp;
		break;
	}
}

// 计算B类属性对强化完美率的影响
FLOAT Role::CalSpecAttEffectPef(EEquipSpecAtt eSpecAtt, FLOAT fPerfect)
{
	switch(eSpecAtt)
	{
	case EESA_Guard_Fortune:
		return fPerfect * 1.05f;
		break;
	default:
		return fPerfect;
		break;
	}
}

//-----------------------------------------------------------------------------
// 烙印
//-----------------------------------------------------------------------------
DWORD Role::BrandEquip(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD dwCmdID)
{
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// 找到被强化装备
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_EquipCanNotBrand;

	// 找到印记
	tagItem *pItemBrand = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemBrand) || pItemBrand->pProtoType->eSpecFunc != EISF_Brand )
		return E_Consolidate_Brand_Not_Exit;

	// 找到强化数据
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemBrand->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	// 检测印记等级是否达到上限
	if((pEquip->equipSpec.byBrandLevel + pItemBrand->pProtoType->byLevel) > MAX_BRAND_LEVEL)
		return E_Consolidate_Brand_TopLevel;

	// 检测装备是否能被印记
	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotBrand;

	// 检查装备潜力值是否足够
	if(pEquip->equipSpec.nPotVal < (INT)pConsolidateProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// 记录印记等级
	BYTE byLevel = pItemBrand->pProtoType->byLevel;

	DWORD dwBrandItemTypeID = pItemBrand->dwTypeID;

	// 删除材料
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Compose_Brand, 1);

	// 成功率=圣印基础成功率×（1-当前追加次数/10）^1.2+幸运/5000
	FLOAT fProp = pConsolidateProto->nSuccessRate * pow((1.0f - (FLOAT)(pEquip->equipSpec.byBrandLevel)/10.0f), 1.2f) + (FLOAT)GetAttValue(ERA_Fortune) / 5000.0f; 
	
	// 计算B类属性对成功率影响
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_Brand);

	// 计算IM道具的影响
	tagIMEffect			IMEffect;
	tagConsolidateItem *pProto = const_cast<tagConsolidateItem*>(pConsolidateProto);		
	CalIMEffect(ECTS_Brand, IMEffect, n64IMID, pProto);

	// 计算成功率 
	BOOL bResult = IUTIL->Rand() % 10000 <= fProp;
	const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
	if(bResult)		// 成功
	{
		// 称号事件触发
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);

		//----------------------------------------------------------------------------------------
		// 强化烙印 称号  zxzhang 
		//----------------------------------------------------------------------------------------
   
        GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, static_cast<DWORD>(ETEST_Brand), GT_INVALID); 
  

		// 装备烙印等级(增加量为印记等级)
		pEquip->equipSpec.byBrandLevel += byLevel;

		// 装备增加对应强化属性
		for(INT n = 0; n < MAX_CONSOLIDATE_ROLEATT; ++n)
		{
			INT nIdx = 0;
			if (pConsolidateProto->tagRoleAtt[n].eRoleAtt >= ERA_Derate_Start &&
				pConsolidateProto->tagRoleAtt[n].eRoleAtt <= ERA_Derate_End)
			{
				nIdx = MTransERADerate2Index(pConsolidateProto->tagRoleAtt[n].eRoleAtt);
			}
			else if (pConsolidateProto->tagRoleAtt[n].eRoleAtt >= ERA_transform_Start &&
				pConsolidateProto->tagRoleAtt[n].eRoleAtt <= ERA_transform_End)
			{
				nIdx = MTransERATransform2Index(pConsolidateProto->tagRoleAtt[n].eRoleAtt);
			}
			pEquip->equipSpec.nBrandAtt[nIdx] += pConsolidateProto->tagRoleAtt[n].nAttVal;
		}

		// 计算是否完美（生产完美率=生产成功率×1/20+角色福缘/1000)
		FLOAT fPefectProp = (pConsolidateProto->nSuccessRate * 0.05f) / 10000.0f+ (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// 计算B类属性对完美率影响
		fPefectProp = CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// 计算装备光晕
		CalEquipFlare(pEquip);

#if 0
		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// 消耗装备潜力值为烙印成功的75%
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume * 75 / 100));
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
#endif
		{
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume));
			GetItemMgr().UpdateEquipSpec(*pEquip);

			if (P_VALID(pRoleScript))
				pRoleScript->OnBrandEquip(this, pEquip->dwTypeID, dwBrandItemTypeID, TRUE, pEquip->equipSpec.byBrandLevel);

			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		// 装备潜力值消耗10点
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal(-40/*10*/);// jason 2009-11-28

		GetItemMgr().UpdateEquipSpec(*pEquip);

		if (P_VALID(pRoleScript))
			pRoleScript->OnBrandEquip(this, pEquip->dwTypeID, dwBrandItemTypeID, FALSE, pEquip->equipSpec.byBrandLevel);
		
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// 龙附
//-----------------------------------------------------------------------------
DWORD Role::LoongSoulEquip(INT64 n64SrcItemID, INT64 n64DstItemID, DWORD dwCmdID)
{
	return GT_INVALID;
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// 找到被强化装备
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_EquipCanNotLoongSoul;

	// 找到龙魂石
	tagItem *pItemLoongSoul = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemLoongSoul) || pItemLoongSoul->pProtoType->eSpecFunc != EISF_LongHunShi )
		return E_Consolidate_LoongSoul_Not_Exit;

	// 找到强化数据
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemLoongSoul->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	// 检测印记等级是否达到上限
	if((pEquip->equipSpec.byLongfuLevel + pItemLoongSoul->pProtoType->byLevel) > MAX_LONGHUN_LEVEL)
		return E_Consolidate_LoongSoul_TopLevel;

	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotLoongSoul;

	// 检查装备潜力值是否足够
	if(pEquip->equipSpec.nPotVal < (INT)pConsolidateProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// 记录龙魂石等级
	BYTE byLevel = pItemLoongSoul->pProtoType->byLevel;

	// 删除材料
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Compose_LongFu, 1);

	// 计算成功率(成功率=基础成功率×[1+（装备等级-75）/300]+（角色福缘/1000))
	FLOAT fProp = pConsolidateProto->nSuccessRate * (1.0f + (FLOAT)(pEquip->pProtoType->byLevel - 75.0f) / 300.0f)
					+ ((FLOAT)GetAttValue(ERA_Fortune) / 1000.0f) * 10000;

	// 计算B类属性对成功率影响
	fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fProp, EESA_Guard_LongFu);

	BOOL bResult = IUTIL->Rand() % 10000 <= (INT)fProp;

	if(bResult)		// 成功
	{
		// 称号事件触发和所有强化技能
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, GT_INVALID, GT_INVALID);
		GetTitleMgr()->SigEvent(ETE_STRENGTHEN_SKILL_SUCCESS, ETEST_LoongSoul, GT_INVALID);

		// 装备龙魂等级(增加量为龙魂石等级)
		pEquip->equipSpec.byLongfuLevel += byLevel;

		// 装备增加对应强化属性
		pEquip->equipSpec.byLongfuAtt[pConsolidateProto->eWuXing] += byLevel;
		
		// 计算是否完美（完美率=1%+（角色福缘/1000))
		FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		// 计算B类属性对完美率影响
		fPefectProp =  CalSpecAttEffectPef((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), fPefectProp);

		// 计算装备光晕
		CalEquipFlare(pEquip);

		BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			// 消耗装备潜力值为龙魂成功的75%
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume * 75 / 100));
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			pEquip->ChangePotVal(-((INT)pConsolidateProto->dwPotValConsume));
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		// 装备对应的龙附等级归0，消耗龙魂，不消耗装备潜力值，装备的龙魂属性清空
		pEquip->equipSpec.byLongfuLevel = 0;
		ZeroMemory(pEquip->equipSpec.byLongfuAtt, sizeof(pEquip->equipSpec.byLongfuAtt));
		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}
}

//-----------------------------------------------------------------------------
// GM龙附
//-----------------------------------------------------------------------------
DWORD Role::GMLoongSoulEquip(INT64 n64SrcItemID, INT16 n16DstItemIndex)
{
	// 找到被强化装备
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n16DstItemIndex);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_EquipCanNotLoongSoul;

	// 找到龙魂石
	tagItem *pItemLoongSoul = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemLoongSoul) || pItemLoongSoul->pProtoType->eSpecFunc != EISF_LongHunShi )
		return E_Consolidate_LoongSoul_Not_Exit;

	// 找到强化数据
	const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemLoongSoul->dwTypeID);
	if(!P_VALID(pConsolidateProto))
		return E_Compose_Formula_Not_Exist;

	// 检测印记等级是否达到上限
	if((pEquip->equipSpec.byLongfuLevel + pItemLoongSoul->pProtoType->byLevel) > MAX_LONGHUN_LEVEL)
		return E_Consolidate_LoongSoul_TopLevel;

	BOOL bConsolidate = FALSE;
	for(INT m = 0; m < MAX_CONSOLIDATE_POS_QUANTITY; ++m)
	{
		if(pEquip->pEquipProto->eEquipPos != pConsolidateProto->ConsolidatePos[m].ePos)
			continue;
		else
		{
			if( 1 == pConsolidateProto->ConsolidatePos[m].byConsolidate)
			{
				bConsolidate = TRUE;
				break;
			}
		}
	}

	if(!bConsolidate)
		return E_Consolidate_EquipCanNotLoongSoul;

	// 记录龙魂石等级
	BYTE byLevel = pItemLoongSoul->pProtoType->byLevel;

	// 装备龙魂等级(增加量为龙魂石等级)
	pEquip->equipSpec.byLongfuLevel += byLevel;

	// 装备增加对应强化属性
	pEquip->equipSpec.byLongfuAtt[pConsolidateProto->eWuXing] += byLevel;

	// 计算装备光晕
	CalEquipFlare(pEquip);

	GetItemMgr().UpdateEquipSpec(*pEquip);
	return E_Compose_Consolidate_Success;
}

//---------------------------------------------------------------------------------
// 淬火
//---------------------------------------------------------------------------------
DWORD Role::QuenchEquip(DWORD dwNPCID, DWORD dwFormulaID, INT64 n64ItemID, INT64 n64IMID, 
											INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	return GT_INVALID;
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Quench) )
		return E_Consolidate_NPCCanNotQuench;

	// 找到强化数据
	const tagQuenchProtoSer *pQuenchProto = g_attRes.GetQuenchProto(dwFormulaID);
	if(!P_VALID(pQuenchProto))
		return E_Compose_Formula_Not_Exist;

	// 找到被强化装备
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64ItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;

	if( MIsFaBao(pEquip) )
		return E_Consolidate_False;

	// 检查装备潜力值是否足够
	if(pEquip->equipSpec.nPotVal < (INT)pQuenchProto->dwPotValConsume)
		return E_Consolidate_ValConsume_Inadequate;

	// 检查玩家金钱是否足够
	if(GetCurMgr().GetBagSilver() < pQuenchProto->dwMoneyConsume)
		return E_Consolidate_NotEnough_Money;

	// 原属性的属性值是否足够
	if(pEquip->equipSpec.byLongfuAtt[pQuenchProto->srcQuenchAtt.eWuXing] < pQuenchProto->srcQuenchAtt.nWuXingValue)
		return E_Consolidate_WXValueInadequate;

	for(INT n = 0; n < MAX_CONSOLIDATE_STUFF_QUANTITY; ++n)
	{
		if(pQuenchProto->ConsolidateStuff[n].dwStuffID == GT_INVALID 
			&& pQuenchProto->ConsolidateStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 

		if(!P_VALID(pItemStuff))
			return E_Consolidate_NotEnough_Stuff;

		// 检测材料64位ID是否重复
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Consolidate_NotEnough_Stuff;
		}

		if(pItemStuff->dwTypeID != pQuenchProto->ConsolidateStuff[n].dwStuffID 
			&& pItemStuff->pProtoType->eStuffType != pQuenchProto->ConsolidateStuff[n].eStuffType)
			return E_Consolidate_NotEnough_Stuff;

		if(pItemStuff->GetNum() < (INT)pQuenchProto->ConsolidateStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;
	}

	// 计算IM道具的影响
	tagIMEffect			IMEffect;
	tagQuenchProtoSer *pProto = const_cast<tagQuenchProtoSer*>(pQuenchProto);
	CalIMEffect(ECTS_Quench, IMEffect, n64IMID, pProto);

	// 输入材料消耗
	for(INT n = 0; n < nArraySz; ++n)
	{
		GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Quench, (INT16)pQuenchProto->ConsolidateStuff[n].dwStuffNum);
	}

	// 装备潜力值消耗
	pEquip->ChangePotVal(-(INT)(pQuenchProto->dwPotValConsume));

	// 金钱消耗
	GetCurMgr().DecBagSilver(pQuenchProto->dwMoneyConsume, ELCID_Compose_Quench);

	// 计算B类属性对成功率影响
	FLOAT fProp = CalSpecAttEffectSuc((EEquipSpecAtt)(pEquip->equipSpec.bySpecAtt), (FLOAT)pQuenchProto->nSuccessRate, EESA_Guard_Quench);

	// 检测玩家是否用了提高成功率的IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= fProp;
	if(bResult)
	{
		// 增加目标属性
		pEquip->equipSpec.byLongfuAtt[pQuenchProto->srcQuenchAtt.eWuXing] -= pQuenchProto->srcQuenchAtt.nWuXingValue;
		pEquip->equipSpec.byLongfuAtt[pQuenchProto->dstQuenchAtt.eWuXing] += pQuenchProto->dstQuenchAtt.nWuXingValue;
		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Success;
	}
	else		// 失误
	{
		// 增加的属性类型在除了原属性和目标属性外的其它三种属性中随机
		EWuXing		eType = EWX_Null; 
		INT			nWuXingValue = pQuenchProto->dstQuenchAtt.nWuXingValue;
		BYTE		byTotalValue = 0;
		while (TRUE)
		{
			eType =(EWuXing)(IUTIL->Rand() % 5);
			if(pQuenchProto->srcQuenchAtt.eWuXing != eType && pQuenchProto->dstQuenchAtt.eWuXing != eType)
				break;
		}

		// 配方表中所填的目标属性增加值为n，则失误时实际增加值在[n-2，n+1]间随机，
		// 取值范围为[1，10]，且淬火后所有龙魂属性总合不能超过10
		nWuXingValue = IUTIL->Rand() % 3 + (nWuXingValue - 2);

		for(INT i = 0; i < EWX_End; ++i)
		{
			byTotalValue += pEquip->equipSpec.byLongfuAtt[i];
		}

		nWuXingValue = (nWuXingValue > (10 - byTotalValue)) ? (10 - byTotalValue) : nWuXingValue;
		pEquip->equipSpec.byLongfuAtt[eType] += nWuXingValue;
		pEquip->equipSpec.byLongfuAtt[pQuenchProto->srcQuenchAtt.eWuXing] -= pQuenchProto->srcQuenchAtt.nWuXingValue;

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Consolidate_False;

	}
}
//---------------------------------------------------------------------------------
// 生产合成物品
//---------------------------------------------------------------------------------
DWORD Role::ProduceItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID,
								INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	if( IsShieldingFormula(dwFormulaID) )
		return E_Compose_Formula_Not_Exist;
	// 找到合成表数据
	const tagProduceProtoSer* pProduceProto = g_attRes.GetProduceProto(dwFormulaID);

	if(!P_VALID(pProduceProto))
		return E_Compose_Formula_Not_Exist;

	DWORD dwErrorCode = E_Success;

	dwErrorCode = CheckProduceLimit(dwNPCID, dwSkillID, n64ItemID, dwFormulaID, pProduceProto, n64StuffID, nArraySz);

	if( dwErrorCode != E_Success)
		return dwErrorCode;

	// 计算IM道具的影响
	tagIMEffect			IMEffect;
	tagProduceProtoSer *pProto = const_cast<tagProduceProtoSer*>(pProduceProto);
	CalIMEffect(ECTS_Produce, IMEffect, n64IMID, pProto);

	// 金钱消耗
	GetCurMgr().DecBagSilver(pProduceProto->dwMoneyConsume, ELCID_Compose_Produce);

	if (P_VALID(m_pScript))
	{
		m_pScript->DecSilverByCompose(this, pProduceProto->dwProItemTypeID, pProduceProto->dwMoneyConsume);
	}

	// 增加合成技能熟练度(只有技能合成才会增加技能熟练度）
	Skill* pSkill = GetSkill(dwSkillID);

	if(P_VALID(pSkill) && GT_INVALID == dwNPCID && GT_INVALID == n64ItemID)
	{
		DWORD	dwSkillExp = (DWORD)((FLOAT)pProduceProto->dwMasterIncre * (1.0f + (FLOAT)GetAttValue(ERA_Savvy) / 100.0f));
		ChangeSkillExp(pSkill, dwSkillExp);
	}

	// 扣除玩家的活力值
	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_CONSOLODATE,pProduceProto->dwVitalityConsume);
	//ChangeVitality(-(INT)pProduceProto->dwVitalityConsume);
	ChangeVitality(-(INT)abs(nVitality));

	// 计算成功率
	FLOAT	fProp = (FLOAT)pProduceProto->nSuccessRate;

	// 检测玩家是否用了提高成功率的IM
	if( IMEffect.eEffect ==  EIME_ComAdvance)
	{
		fProp += (FLOAT)IMEffect.dwParam2;
	}

	INT nSuc = 0;
	if( EPCT_Gem == pProduceProto->eProType )
	{
		nSuc = GetSpecSkillValue(ESSF_Gem,pProduceProto->dwProItemTypeID);
	}

	if( EPCT_GodArtisan == pProduceProto->eProType )
	{
		nSuc = GetSpecSkillValue(ESSF_Brand,pProduceProto->dwProItemTypeID);
	}

	BOOL bResult = IUTIL->Rand() % 10000 <= ((INT)fProp+nSuc);

	//ILOG->Write(_T("合成成功率 %d \r\n"), (INT)fProp+nSuc);

	// 称号事件触发
	if (bResult && GT_VALID(dwNPCID))
		GetTitleMgr()->SigEvent(ETE_COMPOSITE_SKILL_SUCCESS	, GT_INVALID, GT_INVALID);

	if(bResult)			// 成功
	{
		// 输入材料消耗
		for(INT n = 0; n < nArraySz; ++n)
		{
			if(!GetMap()->IsWar())
				GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwStuffNum);
			else
				GetItemMgr().DelFromWarBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwStuffNum);
		}
		// 获得合成道具
		if(pProduceProto->eComType == ECOMT_RefineDan)
		{
			// 计算完美率
			//FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;
			//BOOL bPefect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);

			//// 完美后所制造的物品数量翻倍
			//if(bPefect)
			//{
			//	tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
			//					pProduceProto->dwProItemTypeID, (INT16)(pProduceProto->dwProQuantity * 2), m_dwID);
			//	if( P_VALID(pItem) )
			//	{
			//		GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);

			//		// 玩家获得一件道具的事件接口
			//		g_ScriptMgr.GetRoleScript()->OnObtainItem(this, pItem->dwTypeID, pItem->eCreateMode);
			//	}

			//	return E_Compose_Consolidate_Perfect;
			//}
			//else
			{
				tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
								pProduceProto->dwProItemTypeID, (INT16)pProduceProto->dwProQuantity, m_dwID);
				if( P_VALID(pItem) )
				{
					GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);

					// 玩家获得一件道具的事件接口
					g_ScriptMgr.GetRoleScript()->OnObtainItem(this, pItem->dwTypeID, pItem->eCreateMode);
				}

				return E_Compose_Consolidate_Success;
			}
		}
		else
		{
			tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
								pProduceProto->dwProItemTypeID, (INT16)pProduceProto->dwProQuantity, m_dwID);
			if( P_VALID(pItem) )
			{
				GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);

				// 玩家获得一件道具的事件接口
				g_ScriptMgr.GetRoleScript()->OnObtainItem(this, pItem->dwTypeID, pItem->eCreateMode);
			}


			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		// 输入材料消耗
		for(INT n = 0; n < nArraySz; ++n)
		{
			if(!GetMap()->IsWar())
				GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwFailDeductionNum);
			else
				GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwFailDeductionNum);
		}
		return E_Compose_Consolidate_Lose;
	}

}

//---------------------------------------------------------------------------------
// 生产合成装备
// Jason 2009-12-4 按材料类型计算加成
// Jason 2010-4-2 材料类型可以为空，这时材料必须是装备
//--------------------------------------------------------------------------------
DWORD Role::ProduceEquip(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
										INT64 n64IMID, INT64 n64StuffID[], INT32 nArraySz, DWORD dwCmdID)
{
	if( IsShieldingFormula(dwFormulaID) )
		return E_Compose_Formula_Not_Exist;
	// 找到合成表数据
	const tagProduceProtoSer* pProduceProto = g_attRes.GetProduceProto(dwFormulaID);

	if(!P_VALID(pProduceProto))
		return E_Compose_Formula_Not_Exist;

	DWORD dwErrorCode = E_Success;

	dwErrorCode = CheckProduceLimit(dwNPCID, dwSkillID, n64ItemID, dwFormulaID, pProduceProto, n64StuffID, nArraySz);

	if( dwErrorCode != E_Success)
		return dwErrorCode;

	BOOL bFixQlty = TRUE;
	if (pProduceProto->dwQltyFixLimit == 0)
		bFixQlty = FALSE;

	// 输入材料消耗(顺便计算材料品质）
	INT nStuffNum[EIQ_End];
	ZeroMemory(nStuffNum, sizeof(nStuffNum));
	INT nTotalNum = 0;

	// 对指定typeid情况先做检查
	for(INT n = 0; n < nArraySz; ++n)
	{	
		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]);
	
		if(!P_VALID(pItemStuff))
			pItemStuff = GetItemMgr().GetWarBagItem(n64StuffID[n]);

		if( !P_VALID(pItemStuff) )
			continue;

		if( EST_Null == pProduceProto->ProduceStuff[n].eStuffType ) // 指定id情况要判断
		{
			if( MIsEquipment(pItemStuff->dwTypeID) )
			{
				tagEquip	*pEquip = (tagEquip*)pItemStuff;
				if( P_VALID(pEquip) )
				{
					if( EIQ_White > pEquip->equipSpec.byQuality || EIQ_End <= pEquip->equipSpec.byQuality )
						return E_Compose_EquipUnidentify;
				}
			}
		}
	}

	// Jason 2010-7-12 v1.0.0 增加成功率判断
	INT nSucPct = pProduceProto->nSuccessRate / 100;
	if( IUTIL->RandomInRange(0,100) > nSucPct )
	{
		for(INT n = 0; n < nArraySz; ++n)
		{
			INT16 deductnum = pProduceProto->ProduceStuff[n].dwFailDeductionNum;
			if( deductnum <= 0 )
				continue;
			tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]);
			if( !P_VALID(pItemStuff) )
				continue;
			if(!GetMap()->IsWar())
				GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, deductnum);
			else
				GetItemMgr().DelFromWarBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, deductnum);
		}
		// 金钱消耗
		GetCurMgr().DecBagSilver(pProduceProto->dwMoneyConsume, ELCID_Compose_Produce);
		return E_Compose_Consolidate_Lose;
	}

	for(INT n = 0; n < nArraySz; ++n)
	{	
		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]);

		if(!P_VALID(pItemStuff))
			pItemStuff = GetItemMgr().GetWarBagItem(n64StuffID[n]);

		if( !P_VALID(pItemStuff) )
			continue;

		if( EST_Null != pProduceProto->ProduceStuff[n].eStuffType ) // Jason，只统计类型明确的材料数量
		{
			nStuffNum[pItemStuff->pProtoType->byQuality] += pProduceProto->ProduceStuff[n].dwStuffNum;
			nTotalNum += pProduceProto->ProduceStuff[n].dwStuffNum;
		}
		else
		{
			if( MIsEquipment(pItemStuff->dwTypeID) )
			{
				tagEquip	*pEquip = (tagEquip*)pItemStuff;
				if( P_VALID(pEquip) )
				{
					if( EIQ_Start < pEquip->equipSpec.byQuality && EIQ_End > pEquip->equipSpec.byQuality )
					{
						nStuffNum[pEquip->equipSpec.byQuality] += pProduceProto->ProduceStuff[n].dwStuffNum;
						nTotalNum += pProduceProto->ProduceStuff[n].dwStuffNum;
					}
				}
			}
		}

		if(!GetMap()->IsWar())
			GetItemMgr().DelFromBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwStuffNum);
		else
			GetItemMgr().DelFromWarBag(n64StuffID[n], (DWORD)ELCID_Compose_Produce, (INT16)pProduceProto->ProduceStuff[n].dwStuffNum);
	}

	// 金钱消耗
	GetCurMgr().DecBagSilver(pProduceProto->dwMoneyConsume, ELCID_Compose_Produce);

	// 增加合成技能熟练度(只有技能合成才会增加技能熟练度）
	Skill* pSkill = GetSkill(dwSkillID);

	if(P_VALID(pSkill) && GT_INVALID == dwNPCID && GT_INVALID == n64ItemID)
	{
		DWORD	dwSkillExp = (DWORD)((FLOAT)pProduceProto->dwMasterIncre * (1.0f + (FLOAT)GetAttValue(ERA_Savvy) / 100.0f));
		ChangeSkillExp(pSkill, dwSkillExp);
	}

	// 扣除玩家的活力值
	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_CONSOLODATE,pProduceProto->dwVitalityConsume);
	//ChangeVitality(-(INT)pProduceProto->dwVitalityConsume);
	ChangeVitality(-(INT)abs(nVitality));

	// 获得合成道具
	/* 1级修正率=-50%×白品材料数量/材料总数量+10%×黄品材料数量/材料总数量+15%×绿品材料数量
				 /材料总数量+50%×蓝品材料数量/材料总数量+100%×橙品材料数量/材料总数量

	   2级修正率=5%×绿品材料数量/材料总数量+20%×蓝品材料数量/材料总数量+30%×橙品材料数量/材料总数量						*/
	FLOAT fModify1 = 0;
	FLOAT fModify2 = 0;
	FLOAT fModify3 = 0;
	FLOAT fPurplePct = 0;
	INT nPotAdding = 0;

	if( nTotalNum )
	{
		fModify1 = ((-0.5f * (FLOAT)nStuffNum[EIQ_White] + 0.03f * (FLOAT)nStuffNum[EIQ_Yellow]
		+ 0.15f * (FLOAT)nStuffNum[EIQ_Green] + 0.6f * (FLOAT)nStuffNum[EIQ_Blue] 
		+ 1.0f * (FLOAT)nStuffNum[EIQ_Orange]
		// 0.0f留着，将来可能会改
		+ 0.0f * (FLOAT)nStuffNum[EIQ_Purple]  ) / (FLOAT)nTotalNum) * 10000.0f;

		fModify2 = ( ( 0.8f * (FLOAT)nStuffNum[EIQ_Orange] + nStuffNum[EIQ_Purple] ) / (FLOAT)nTotalNum ) * 10000.0f;

		// 去掉潜力值影响
		// 潜力值修正率
		// 潜力值影响系数 = 0.7×白品材料数量/材料总数量+0.8×黄品材料数量/材料总数量+0.9×绿品材料数量/材料总数量+1.0×蓝品材料数量/材料总数量+1.1×橙品材料数量/材料总数量
		//fModify3 = ((0.7f * (FLOAT)nStuffNum[EIQ_White] + 0.8f * (FLOAT)nStuffNum[EIQ_Yellow]
		//+ 0.9f * (FLOAT)nStuffNum[EIQ_Green] + 1.0f * (FLOAT)nStuffNum[EIQ_Blue]
		//+ 1.1f * (FLOAT)nStuffNum[EIQ_Orange] 
		//+ 1.2  * nStuffNum[EIQ_Purple] ) / (FLOAT)nTotalNum) * 10000.0f;

		fPurplePct = 0;//( 0.05 * (FLOAT)nStuffNum[EIQ_Blue] + 0.22 * (FLOAT)nStuffNum[EIQ_Orange] + nStuffNum[EIQ_Purple] ) / nTotalNum;

		// Jason 2009-12-1
		//生产装备可提升潜力值次数 = 
		//静态属性值 + （N白/N总*0+N黄/N总*0+N绿/N总*0+N蓝/N总*3+N橙/N总*6+N紫/N总*10）
		//该值向下取整
		nPotAdding = 0;//( nStuffNum[EIQ_Blue] * 3 + nStuffNum[EIQ_Orange] * 6 + nStuffNum[EIQ_Purple] * 10 ) / nTotalNum;
	}


	FLOAT nRnd = ( (FLOAT) IUTIL->RandomInRange(0,100) ) / 100;
	


	tagItem *pItem = NULL;
	//if( fPurplePct == 0 ) // 当nTotalNum=0时，又恰逢nRnd=0；原来的版本，就会鉴定出紫装!
		pItem = ItemCreator::Create(EICM_Product,  m_dwID, 
		pProduceProto->dwProItemTypeID, (INT16)(pProduceProto->dwProQuantity), m_dwID, (INT16)fModify1, (INT16)fModify2, (INT16)fModify3,nPotAdding, -1, bFixQlty);
	//if( nRnd > fPurplePct  )//如果紫色品级鉴定几率为0，也得走老的鉴定方法，Jason 2009-12-23 fix
	//{
	//	pItem = ItemCreator::Create(EICM_Product,  m_dwID, 
	//		pProduceProto->dwProItemTypeID, (INT16)(pProduceProto->dwProQuantity), m_dwID, (INT16)fModify1, (INT16)fModify2, (INT16)fModify3,nPotAdding);
	//	if( P_VALID(pItem) && MIsEquipment(pItem->dwTypeID) )
	//	{
	//		tagEquip * pEquip = static_cast<tagEquip*>(pItem);
	//		pEquip->equipSpec.dwPurpleQltyIdfPct = fPurplePct*100;
	//	}
	//}
	//else
	//	pItem = ItemCreator::CreateByProduce(EICM_Product,  m_dwID, 
	//	pProduceProto->dwProItemTypeID,EIQ_Purple, nPotAdding ,(DWORD)(fPurplePct * 100),(INT16)(pProduceProto->dwProQuantity), m_dwID, (INT16)fModify1, (INT16)fModify2, (INT16)fModify3);

	// 判断是否为时装，是则鉴定
	if(P_VALID(pItem) && MIsFashion(pItem))
	{
		// 计算IM道具的影响
		tagIMEffect			IMEffect;
		tagProduceProtoSer *pProto = const_cast<tagProduceProtoSer*>(pProduceProto);
		CalIMEffect(ECTS_Produce, IMEffect, n64IMID, pProto);

		ItemCreator::IdentifyEquip((tagEquip*)pItem, EIQ_Null, &IMEffect, bFixQlty);
	}
	
	if( P_VALID(pItem) )
	{
		GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);

		// 玩家获得一件道具的事件接口
		g_ScriptMgr.GetRoleScript()->OnObtainItem(this, pItem->dwTypeID, pItem->eCreateMode);

		tagEquip* pEquip = (tagEquip*)pItem;
		const RoleScript* pRoleScript = g_ScriptMgr.GetRoleScript();
		if (P_VALID(pRoleScript))
			pRoleScript->OnProduceEquip(this, pEquip->dwTypeID, pEquip->equipSpec.byQuality);
	}

	// 称号事件触发
	if (GT_VALID(dwNPCID))
		GetTitleMgr()->SigEvent(ETE_COMPOSITE_SKILL_SUCCESS, GT_INVALID, GT_INVALID);


	return E_Compose_Consolidate_Success;
}

//---------------------------------------------------------------------------------
// 点化,装备分解
//---------------------------------------------------------------------------------
DWORD Role::DeComposeItem(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
								INT64 n64IMID, INT64 n64Item, DWORD dwCmdID)
{
	// 找到合成表数据
	const tagDeComposeProtoSer* pDeComposeProto = g_attRes.GetDeComposeProto(dwFormulaID);

	if(!P_VALID(pDeComposeProto))
		return E_Compose_Formula_Not_Exist;

	// 找到出入材料
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64Item); 

	DWORD dwErrorCode = E_Success;

	dwErrorCode = CheckDeComposeLimit(dwNPCID, dwSkillID, n64ItemID, dwFormulaID, pDeComposeProto, n64Item, pEquip);

	if( dwErrorCode != E_Success)
		return dwErrorCode;

	// 点化只限于黄色及以上装备，分解则不限品质
	if(pDeComposeProto->eComType == ECOMT_PointUp && pEquip->equipSpec.byQuality == (BYTE)EIQ_White)
		return E_Compose_Quality_Not_Match;

	if( MIsFaBao(pEquip) )
		return E_Compose_Not_Fashion;

	// 面具及时装不可点化或分解
	if(pEquip->pProtoType->eType == EIT_Fashion || pEquip->pEquipProto->eEquipPos == EEP_Face)
		return E_Compose_Not_Fashion;

	// 已锁定的装备不可点化或分解
	if(pEquip->bLock == true)
		return E_Compose_Equip_Lock;

	// 有时间限制的装备不可点化或分解
	if(pEquip->pProtoType->dwTimeLimit != GT_INVALID)
		return E_Compose_Equip_Time_Limit;

	// 未鉴定的装备不可点化或分解
	if(pEquip->equipSpec.byQuality == (BYTE)EIQ_Null)
		return E_Compose_Equip_Not_identify;

#if 0 // 《F计划》点化不再消耗金钱
	// 金钱消耗
	GetCurMgr().DecBagSilver(pDeComposeProto->dwMoneyConsume, ELCID_Compose_Decompose);
#endif

	// 增加合成技能熟练度(只有技能合成才会增加技能熟练度）
	Skill* pSkill = GetSkill(dwSkillID);

	if(P_VALID(pSkill) && GT_INVALID == dwNPCID && GT_INVALID == n64ItemID)
	{
		DWORD	dwSkillExp = (DWORD)((FLOAT)pDeComposeProto->dwMasterIncre * (1.0f + (FLOAT)GetAttValue(ERA_Savvy) / 100.0f));
		ChangeSkillExp(pSkill, dwSkillExp);
	}

	// 扣除玩家的活力值
	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_COMPOSE,pDeComposeProto->dwVitalityConsume);
	//ChangeVitality(-(INT)pDeComposeProto->dwVitalityConsume);
	ChangeVitality(-(INT)abs(nVitality));

	BOOL	bSuccess = FALSE;
#if 0 // 《F计划》点化不再进行完美率判断
	// 计算是否完美
	// 已消耗潜力值 > 潜力值上限/5 
	// 当以上条件不满足时，计算完美率的公式为：完美率=1%+角色福缘/1000)
	BOOL	bPerfect = FALSE;
	if( pEquip->equipSpec.nPotValUsed > pEquip->pEquipProto->nMaxPotVal / 5)
		bPerfect = TRUE;
	else
	{
		FLOAT fPefectProp = 0.01f + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;
		bPerfect = IUTIL->Rand() % 10000 <= (fPefectProp * 10000);
	}
#endif
	// 输入材料消耗
	tagItem *pItem = GetItemMgr().GetBag().GetItem(n64Item);
	if(P_VALID(pItem))
	{
		const WorldScript* pScript = g_ScriptMgr.GetWorldScript();
		if(P_VALID(pScript) && P_VALID(pItem->pProtoType) )
		{
			pScript->DelItem( 1, pItem->dwTypeID, pItem->pProtoType->byQuality );
		}
	}

	GetItemMgr().DelFromBag(n64Item, (DWORD)ELCID_Compose_Decompose, (INT16)pEquip->GetNum());


	for(INT i = 0; i < MAX_OUTPUT_QUANTITY; ++i)
	{
		if(pDeComposeProto->OutputStuff[i].dwStuffTypeID == GT_INVALID)
			break;

		// 计算成功率
		BOOL bResult = IUTIL->Rand() % 10000 <= (pDeComposeProto->OutputStuff[i].nRate);
		
		if(bResult)
		{
			bSuccess = TRUE;

#if 0 // 《F计划》点化不再进行完美率判断

			// 完美
			if(bPerfect)
			{
				INT nStuffNum = IUTIL->Rand() % (pDeComposeProto->OutputStuff[i].nPerMaxVal + 1);

				if(nStuffNum < pDeComposeProto->OutputStuff[i].nPerMinVal)
					nStuffNum = pDeComposeProto->OutputStuff[i].nPerMinVal;

				tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
					pDeComposeProto->OutputStuff[i].dwStuffTypeID, (INT16)nStuffNum, m_dwID);

				if( P_VALID(pItem) )
				{
					GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);
				}
			}
			else	// 成功
#endif
			{
				INT nStuffNum = IUTIL->Rand() % (pDeComposeProto->OutputStuff[i].nSucMaxVal + 1);

				if(nStuffNum < pDeComposeProto->OutputStuff[i].nSucMinVal)
					nStuffNum = pDeComposeProto->OutputStuff[i].nSucMinVal;

				tagItem *pItem = CreateItem(EICM_Product,  m_dwID, 
					pDeComposeProto->OutputStuff[i].dwStuffTypeID, (INT16)nStuffNum, m_dwID);

				if( P_VALID(pItem) )
				{
					GetItemMgr().Add2Bag(pItem, ELCID_Compose_Produce, TRUE);
				}
			}
		}		
	}

	if(bSuccess)
	{
#if 0 // 《F计划》点化不再进行完美率判断
		if(bPerfect)
			return E_Compose_Consolidate_Perfect;
		else	
#endif
			return E_Compose_Consolidate_Success;
	}
	else
		return E_Compose_Consolidate_Lose;
}

//---------------------------------------------------------------------------------
// 检测生产合成时的限制
//---------------------------------------------------------------------------------
DWORD Role::CheckProduceLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
								const tagProduceProtoSer* &pProduceProto, INT64 n64StuffID[], INT32 nArraySz)
{
	// 技能合成方式
	if(( pProduceProto->eFormFrom == EFormulaFrom_Skill || pProduceProto->eFormFrom == EFormulaFrom_SkillorItem
		|| pProduceProto->eFormFrom == EFormulaFrom_SkillorNPC) && GT_INVALID != dwSkillID)
	{
		Skill* pSkill = GetSkill(dwSkillID);

		if(!P_VALID(pSkill))
			return E_Compose_Skill_Not_Exist;

		// 检测技能类型是否匹配
		if(Skill2ProduceType(pSkill->GetTypeEx2()) != pProduceProto->eProType)
			return E_Compose_FormulaNotMatch;

		// 检测技能等级与配方等级是否匹配
		if(pSkill->GetLevel() < pProduceProto->nFormLvl)
			return E_Compose_FormulaNotMatch;		
	}
	// 物品合成方式
	else if(( pProduceProto->eFormFrom == EFormulaFrom_Item || pProduceProto->eFormFrom == EFormulaFrom_SkillorItem
		|| pProduceProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != n64ItemID)
	{
		// 物品是否在背包内

		// 是否为合成类物品
	}
	// NPC合成方式
	else if(( pProduceProto->eFormFrom == EFormulaFrom_NPC || pProduceProto->eFormFrom == EFormulaFrom_SkillorNPC
		|| pProduceProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != dwNPCID)
	{
		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC)) 
			return E_Compose_NPC_Not_Exist;

		if( FALSE == pNPC->CheckNPCTalkDistance(this) )
			return E_Compose_NPC_Distance;

		if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Compose) )
			return E_Compose_NPCCanNotCompose;

		// 还要检测NPC是否挂有合成配方
	}

	// 若角色背包已满				
	ItemMgr& itemMgr = GetItemMgr();	
	if (itemMgr.GetBagFreeSize() <= 0)
		return E_Compose_Bag_Full;

	// 玩家活力值是否足够
	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_COMPOSE,pProduceProto->dwVitalityConsume);

	//if(GetAttValue(ERA_Vitality) < (INT)pProduceProto->dwVitalityConsume)
	//	return E_Compose_Vitality_Inadequate;
	
	if(GetAttValue(ERA_Vitality) < (nVitality))
		return E_Compose_Vitality_Inadequate;


	// 检查玩家金钱是否足够
	if(GetCurMgr().GetBagSilver() < pProduceProto->dwMoneyConsume)
		return E_Compose_NotEnough_Money;

	// 检测玩家材料是否足够
	for(INT n = 0; n < MAX_PRODUCE_STUFF_QUANTITY; ++n)
	{
		if(pProduceProto->ProduceStuff[n].dwStuffID == GT_INVALID 
			&& pProduceProto->ProduceStuff[n].eStuffType == EST_Null)
			break;

		tagItem *pItemStuff = GetItemMgr().GetBagItem(n64StuffID[n]); 
		if(!P_VALID(pItemStuff))
			pItemStuff = GetItemMgr().GetWarBagItem(n64StuffID[n]);

		if(!P_VALID(pItemStuff))
			return E_Compose_Stuff_Not_Enough;

		// 检测材料64位ID是否重复
		for(INT i = 0; i < n; ++i)
		{
			if(n64StuffID[i] == n64StuffID[n])
				return E_Compose_Stuff_Not_Enough;
		}

		if(pProduceProto->ProduceStuff[n].dwStuffID == GT_INVALID )
		{
			if(pItemStuff->pProtoType->eStuffType != pProduceProto->ProduceStuff[n].eStuffType)
				return E_Compose_Stuff_Not_Enough;
		}
		else
		{
			if(pItemStuff->pProtoType->dwTypeID != pProduceProto->ProduceStuff[n].dwStuffID)
				return E_Compose_Stuff_Not_Enough;
		}

		if(pItemStuff->GetNum() < (INT)pProduceProto->ProduceStuff[n].dwStuffNum)
			return E_Consolidate_NotEnough_Stuff;
	}

	return E_Success;
}

//---------------------------------------------------------------------------------
// 检测生产合成时的限制
//---------------------------------------------------------------------------------
DWORD Role::CheckDeComposeLimit(DWORD dwNPCID, DWORD dwSkillID, INT64 n64ItemID, DWORD dwFormulaID, 
						 const tagDeComposeProtoSer* &pDeComposeProto, INT64 n64Item, tagEquip *pEquip)
{
	// 技能合成方式
	if(( pDeComposeProto->eFormFrom == EFormulaFrom_Skill || pDeComposeProto->eFormFrom == EFormulaFrom_SkillorItem
		|| pDeComposeProto->eFormFrom == EFormulaFrom_SkillorNPC) && GT_INVALID != dwSkillID)
	{
		Skill* pSkill = GetSkill(dwSkillID);

		if(!P_VALID(pSkill))
			return E_Compose_Skill_Not_Exist;

		// 检测技能类型是否匹配
		if(pSkill->GetTypeEx2() != ESSTE2_PointUp)
			return E_Compose_FormulaNotMatch;

		// 检测技能等级与配方等级是否匹配
		if(pSkill->GetLevel() < pDeComposeProto->nFormLvl)
			return E_Compose_FormulaNotMatch;		
	}
	// 物品合成方式
	else if(( pDeComposeProto->eFormFrom == EFormulaFrom_NPC || pDeComposeProto->eFormFrom == EFormulaFrom_SkillorNPC
			|| pDeComposeProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != n64ItemID )
	{
		// 物品是否在背包内

		// 是否为合成类物品
	}
	// NPC合成方式
	else if( ( pDeComposeProto->eFormFrom == EFormulaFrom_Item || pDeComposeProto->eFormFrom == EFormulaFrom_SkillorItem
			|| pDeComposeProto->eFormFrom == EFormulaFrom_NPCorItem) && GT_INVALID != dwNPCID )
	{
		Creature* pNPC = GetMap()->FindCreature(dwNPCID);
		if( !P_VALID(pNPC)) 
			return E_Compose_NPC_Not_Exist;

		if( FALSE == pNPC->CheckNPCTalkDistance(this) )
			return E_Compose_NPC_Distance;

		if( FALSE == pNPC->IsFunctionNPC(EFNPCT_Compose) )
			return E_Compose_NPCCanNotCompose;

		// 还要检测NPC是否挂有合成配方
	}

	// 若角色背包已满				
	ItemMgr& itemMgr = GetItemMgr();	
	if (itemMgr.GetBagFreeSize() <= 0)
		return E_Compose_Bag_Full;

	// 玩家活力值是否足够
	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_COMPOSE,pDeComposeProto->dwVitalityConsume);

	//if(GetAttValue(ERA_Vitality) < (INT)pDeComposeProto->dwVitalityConsume)
	//	return E_Compose_Vitality_Inadequate;

	if(GetAttValue(ERA_Vitality) < (nVitality))
		return E_Compose_Vitality_Inadequate;

	if(!P_VALID(pEquip))
		return E_Compose_Stuff_Not_Enough;

	// 检查玩家金钱是否足够
	if(GetCurMgr().GetBagSilver() < pDeComposeProto->dwMoneyConsume)
		return E_Compose_NotEnough_Money;

	// 检查装备等级是否满足
	if((pEquip->pProtoType->byLevel / 10) < pDeComposeProto->byLevel)
		return E_Compose_Equip_Level_Inadequate;

	// 检测装备品质是否满足要求
	if(pEquip->equipSpec.byQuality < pDeComposeProto->byQuality)
		return E_Compose_Equip_Quality_Inadequate;

	// 检测武器类型是否满足配方要求
	if(pEquip->pProtoType->eType == EIT_Weapon && pEquip->pProtoType->eTypeEx != pDeComposeProto->eTypeEx)
		return E_Compose_Equip_Type_Inadequate;

	// 检测装备是否满足配方要求
	if((pEquip->pProtoType->eType == EIT_ClothArmor 
		|| pEquip->pProtoType->eType == EIT_Decoration 
		|| pEquip->pProtoType->eType == EIT_Armor) 
		&& pEquip->pEquipProto->eEquipPos != pDeComposeProto->ePos)

		return E_Compose_Equip_Type_Inadequate;

	return E_Success;
}

//---------------------------------------------------------------------------------
// 生产技能类型转换为生产类型
//---------------------------------------------------------------------------------
EProduceType Role::Skill2ProduceType(ESkillTypeEx2 eSkillType)					 
{													 				 
	switch(eSkillType)								 
	{												 
	case ESSTE2_Smilt:					return EPCT_Smilt;							 
	case ESSTE2_Artisan:				return EPCT_Artisan;						 
	case ESSTE2_DanTraining:			return EPCT_DanTraining;					 
	case ESSTE2_Smith:					return EPCT_Smith;							 
	case ESSTE2_Casting:				return EPCT_Casting;						 
	case ESSTE2_Dressmaker:				return EPCT_Dressmaker;						 
	case ESSTE2_Aechnics:				return EPCT_Aechnics;
	default:							return EPCT_NULL;
	}												 
}

//---------------------------------------------------------------------------------
// 生产物品（是否需要记生产者）
//---------------------------------------------------------------------------------
tagItem* Role::CreateItem(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num, DWORD dwCreator)
{
	tagItemProto *pProto = g_attRes.GetItemProto(dwTypeID);
	if(!P_VALID(pProto))
		return (tagItem*)GT_INVALID;

	if(pProto->n16MaxLapNum > 1)
		return ItemCreator::Create(EICM_Product,  m_dwID, dwTypeID, n16Num);
	else
		return ItemCreator::Create(EICM_Product,  m_dwID, dwTypeID, n16Num, m_dwID);		
}

//---------------------------------------------------------------------------------
// 计算IM物品的影响
//---------------------------------------------------------------------------------
VOID Role::CalIMEffect(EConsolidateTypeSer eConType, tagIMEffect &IMEffect, INT64 n64IMID, const LPVOID pProto)
{

	// 检测玩家是否用了提高成功率的IM
	if( GT_INVALID == n64IMID)
		return;

	tagItem *pItemIM = GetItemMgr().GetBagItem(n64IMID); 
	if(!P_VALID(pItemIM))
		return;

	switch (eConType)
	{
	case ECTS_Posy:
		{
			const tagPosyProtoSer*  pPosyProto = (tagPosyProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
			   if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_Posy 
				  || pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_AllConsolidate
				  || pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
			   {
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// 删除IM道具
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Posy, 1);
			   }
			   break;
			}

			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// 删除IM道具
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Posy, 1);
			}
		}
		break;
	case ECTS_Engrave:
		{
			const tagEngraveProtoSer* pEngraveProto = (tagEngraveProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
				if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_Engrave 
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_AllConsolidate
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
				{
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// 删除IM道具
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Engrave, 1);
				}
				break;
			}

			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// 删除IM道具
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Engrave, 1);
			}
		}
		break;
	case ECTS_Inlay:
		{

		}
		break;
	case ECTS_Brand:
		{
			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// 删除IM道具
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Brand, 1);
			}
		}
		break;
	case ECTS_LoongSoul:
		{

		}
		break;
	case ECTS_Quench:
		{
			const tagQuenchProtoSer *pQuenchProto  = (tagQuenchProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
				if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_Quench 
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_AllConsolidate
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
				{
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// 删除IM道具
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Quench, 1);
				}
				break;
			}
		}
		break;
	case ECTS_Produce:
		{
			const tagProduceProtoSer*  pProduceProto = (tagProduceProtoSer*)pProto;
			if(pItemIM->pProtoType->eSpecFunc == EISF_ComposeAdvance)
			{
				if(pItemIM->pProtoType->nSpecFuncVal1 == (INT)pProduceProto->eComType 
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)ESFCA_AllProduce
					|| pItemIM->pProtoType->nSpecFuncVal1 == (INT)EISFC_All)
				{
					IMEffect.eEffect = EIME_ComAdvance;
					IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

					// 删除IM道具
					GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Produce, 1);
				}
				break;
			}
			else if (pItemIM->pProtoType->eSpecFunc == EISF_ColorProbability)
			{
				IMEffect.eEffect = EIME_Color;
				IMEffect.dwParam1 = pItemIM->pProtoType->nSpecFuncVal1;
				IMEffect.dwParam2 = pItemIM->pProtoType->nSpecFuncVal2;

				// 删除IM道具
				GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Compose_Produce, 1);
			}
		}
		break;
	case ECTS_Decompose:
		{

		}
		break;
	case ECTS_RaisePotVal:
		{
			if(pItemIM->pProtoType->eSpecFunc == EISF_ProtectSign/*EISF_EquipAddPotValue*/)
			{
				IMEffect.eEffect = EIME_ProtectSign;
				// 删除IM道具
			}
			GetItemMgr().DelFromBag(n64IMID, (DWORD)ELCID_Raise_PotVal, 1);
		}
		break;
	}
}


//---------------------------------------------------------------------------------
// 计算装备光晕
//---------------------------------------------------------------------------------
VOID Role::CalEquipFlare(tagEquip* pEquip)
{
	BYTE byFlareVal = 0;
	BYTE byHoleBNum = 0;
	// 计算镶嵌次数
	for(INT i = 0; i < (INT)pEquip->equipSpec.byHoleNum; ++i)
	{
		if(pEquip->equipSpec.dwHoleGemID[i] == GT_INVALID || pEquip->equipSpec.dwHoleGemID[i] == 0)
			continue;
		else
			byHoleBNum++;
	}

	// 光晕值 = 2×装备强化等级 + 4×镶嵌次数 + 2×追加等级
	byFlareVal = pEquip->equipSpec.byConsolidateLevel*2 + byHoleBNum*4 + pEquip->equipSpec.byBrandLevel*2;

	pEquip->SetFlareVal(byFlareVal);
}



DWORD Role::RaiseEquipPotVal(INT64 n64SrcItemID, INT64 n64DstItemID, INT64 n64IMID, DWORD & dwPotValAdded)
{
	dwPotValAdded = 0;
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	// 找到待提升装备
	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(n64DstItemID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	if(!MIsEquipment(pEquip->dwTypeID))
		return E_Consolidate_NotEquipment;
	else if( !( EIT_Weapon		== pEquip->pEquipProto->eType ||
				EIT_ClothArmor	== pEquip->pEquipProto->eType ||
				EIT_Decoration	== pEquip->pEquipProto->eType ||
				EIT_Armor		== pEquip->pEquipProto->eType) )
		return E_Consolidate_NotEquipment;

	// 找到潜力提升道具
	tagItem *pItemRaiser = GetItemMgr().GetBagItem(n64SrcItemID);
	if(!P_VALID(pItemRaiser) || pItemRaiser->pProtoType->eSpecFunc != EISF_EquipAddPotValue )
		return E_Compose_Stuff_Not_Exist/*E_Consolidate_Brand_Not_Exit*/;

	// 找到强化数据，这个不需要
	//const tagConsolidateItem *pConsolidateProto = g_attRes.GetConsolidateProto(pItemRaiser->dwTypeID);
	//if(!P_VALID(pConsolidateProto))
	//	return E_Compose_Formula_Not_Exist;

	// 检测可提升次数限制
	if( 0 == pEquip->equipSpec.nPotIncTimes )
		return E_Raise_PotVal_Times_Zero;

	// 检测潜力提升上限，不要用原型里面的最大值
	INT32 nMaxPotVal = pEquip->pEquipProto->nMaxPotVal/*pEquip->equipSpec.nPotVal + pEquip->equipSpec.nPotValUsed*/;
	if( pEquip->equipSpec.nPotVal >= nMaxPotVal/*pEquip->pEquipProto->nMaxPotVal*/ )
		return E_Raise_PotVal_Not_Needing;

	INT nPotDelta = pItemRaiser->pProtoType->nSpecFuncVal2;

	// 最终可提升装备多少潜力
	if((pEquip->equipSpec.nPotVal + nPotDelta ) > nMaxPotVal/*pEquip->pEquipProto->nMaxPotVal*/ )
		nPotDelta = nMaxPotVal/*pEquip->pEquipProto->nMaxPotVal*/ - pEquip->equipSpec.nPotVal;

	// 计算成功率
	FLOAT fProp = /*(*/ pItemRaiser->pProtoType->nSpecFuncVal1 + (FLOAT)GetAttValue(ERA_Fortune) /*)*/ / 1000.0f;

	// 删除材料
	GetItemMgr().DelFromBag(n64SrcItemID, (DWORD)ELCID_Raise_PotVal, 1);

	// 计算IM道具的影响
	tagIMEffect			IMEffect;
	IMEffect.eEffect	= EIME_Null;
	//tagConsolidateItem *pProto = const_cast<tagConsolidateItem*>(pConsolidateProto);		
	CalIMEffect(ECTS_RaisePotVal, IMEffect, n64IMID, NULL);

	// 计算成功率
	INT rand_n = IUTIL->Rand();
	BOOL bResult = /*IUTIL->RandomInRange(0,100) <= fProp;*/rand_n  % 10000 <= fProp;

	if(bResult)		// 成功
	{
		// 递减提升潜力值次数
		--pEquip->equipSpec.nPotIncTimes;
		if( pEquip->equipSpec.nPotIncTimes < 0 )
			pEquip->equipSpec.nPotIncTimes = 0;

		// 计算是否完美（生产完美率=生产成功率×1/20+角色福缘/1000)
		FLOAT fPefectProp = 0.01 + (FLOAT)GetAttValue(ERA_Fortune) / 1000.0f;

		rand_n = IUTIL->Rand();
		BOOL bPefect = rand_n % 10000 <= (fPefectProp * 10000);

		if(bPefect)
		{
			dwPotValAdded = nPotDelta*2;//pEquip->equipSpec.nPotVal;
			pEquip->ChangePotVal( dwPotValAdded );
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Perfect;
		}
		else
		{
			dwPotValAdded = nPotDelta;
			pEquip->ChangePotVal( dwPotValAdded );
			GetItemMgr().UpdateEquipSpec(*pEquip);
			return E_Compose_Consolidate_Success;
		}
	}
	else
	{
		// 装备潜力值消耗100点
		if(IMEffect.eEffect !=  EIME_ProtectSign)
			pEquip->ChangePotVal( -100 );

		GetItemMgr().UpdateEquipSpec(*pEquip);
		return E_Compose_Consolidate_Lose;
	}

	return 0;
}
// Jason Gem removal info v1.3.1 2009-12-21
DWORD Role::GetGemRemovalInfo(DWORD dwNPCID,INT64 equipID,INT8 & num,DWORD dwGemIDs[MAX_EQUIPHOLE_NUM])
{
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}

	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;

	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_GemRemoval) )
		return E_Gem_NPCCanNotRemoveGem;



	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(equipID);
	if(!P_VALID(pEquip))
		return E_Consolidate_Equip_Not_Exist;

	num = pEquip->equipSpec.byHoleNum;
	if(num > MAX_EQUIPHOLE_NUM)
		num = MAX_EQUIPHOLE_NUM;
	else if( num < 0 )
		num = 0;

	memset(dwGemIDs,0,sizeof(dwGemIDs));

	for(int i = 0; i < num ; ++i)
		dwGemIDs[i] = pEquip->equipSpec.dwHoleGemID[i];

	if( 0 == num )
		return E_Consolidate_Gem_Not_Exit;

	return E_Success;
}

namespace jm_Gemtool
{
struct tagTem
{
	INT64 sid;
	int count;
	int index[MAX_EQUIPHOLE_NUM];
} ;
struct tagTems
{
	tagTem data[MAX_EQUIPHOLE_NUM];
	tagTems()
	{
		memset(data,0,sizeof(data));
	}
	void add(INT64 id, int index)
	{
		for(int i = 0; i < MAX_EQUIPHOLE_NUM; ++i)
		{
			if(data[i].sid == 0)
			{
				data[i].sid = id;
				data[i].count = 1;
				data[i].index[data[i].count - 1] = index;
				break;
			}
			else if(data[i].sid == id)
			{
				data[i].count ++;
				data[i].index[data[i].count - 1] = index;
				break;
			}
		}
	}
	int countNoZero()
	{
		int count = 0;
		for(int i = 0; i < MAX_EQUIPHOLE_NUM; ++i)
		{
			if(data[i].sid > 0)
				++count;
		}
		return count;
	}
	int countOneSid(INT64 id)
	{
		for(int i = 0; i < MAX_EQUIPHOLE_NUM; ++i)
		{
			if( id == data[i].sid )
			{
				return data[i].count;
			}
		}
		return 0;
	}
	int decreaseOneSid(INT64 id)
	{
		for(int i = 0; i < MAX_EQUIPHOLE_NUM; ++i)
		{
			if( id == data[i].sid )
			{
				if( data[i].count > 1 )
					data[i].count--;
				return data[i].count;
			}
		}
		return 0;
	}
	int decreaseOneSid(INT64 id,int num,INT64* pOuter)
	{
		if( !pOuter )
			return 0;
		for(int i = 0; i < MAX_EQUIPHOLE_NUM; ++i)
		{
			if( id == data[i].sid )
			{
				for(int a = num ; a < data[i].count; ++a )
					pOuter[data[i].index[a]] = 0;
				return data[i].count;
			}
		}
		return 0;
	}
};
}


DWORD Role::RemoveGemFromEquip(DWORD dwNPCID,INT64 equipID,INT8 num,INT64 SignIDs[MAX_EQUIPHOLE_NUM])
{
	// 获得地图
	Map *pMap = GetMap();
	if(!P_VALID(pMap))
	{
		ASSERT(P_VALID(pMap));
		return GT_INVALID;
	}


	Creature* pNPC = pMap->FindCreature(dwNPCID);
	if( !P_VALID(pNPC) ) 
		return E_Compose_NPC_Not_Exist;

	if( FALSE == pNPC->CheckNPCTalkDistance(this) )
		return E_Compose_NPC_Distance;
	if( FALSE == pNPC->IsFunctionNPC(EFNPCT_GemRemoval) )
		return E_Gem_NPCCanNotRemoveGem;

	tagEquip *pEquip = (tagEquip*)GetItemMgr().GetBagItem(equipID);
	if(!P_VALID(pEquip))
		return E_Compose_Stuff_Not_Exist;

	INT8 n8Num = pEquip->equipSpec.byHoleNum;


	tagItem* pItems[MAX_EQUIPHOLE_NUM] = {0};
	INT64 signs[MAX_EQUIPHOLE_NUM] = {0};
	DWORD gems[MAX_EQUIPHOLE_NUM] = {0};
	int count = 0;

	jm_Gemtool::tagTems temSigns;
	for(int i = 0; i < n8Num ; ++i)
		temSigns.add(SignIDs[i],i);

	for(int i = 0; i < n8Num; ++i)
	{
		if( SignIDs[i] == 0 || GT_INVALID == SignIDs[i] )
			continue;

		tagItem * pItem = GetItemMgr().GetBagItem(SignIDs[i]);
		if(!P_VALID(pItem))
		{
			SignIDs[i] = 0;
			continue;
		}

		INT count = temSigns.countOneSid  (SignIDs[i]);
		if( count > 0 )
		{
			if(pItem->GetNum() < count)
			{
				temSigns.decreaseOneSid(SignIDs[i],pItem->GetNum(),SignIDs);
			}
		}
	}


	for(int i = 0,j = 0; i < n8Num ; ++i)
	{
		if( SignIDs[i] == 0 || GT_INVALID == SignIDs[i] )
			continue;

		if( !P_VALID( GetItemMgr().GetBagItem(SignIDs[i]) ) )
			continue;

		tagItemProto * pItemProto = g_attRes.GetItemProto( pEquip->equipSpec.dwHoleGemID[i] );
		tagItem *pItemGemSign = GetItemMgr().GetBagItem(SignIDs[i]);
		if( P_VALID(pItemProto) && P_VALID(pItemGemSign) && pItemGemSign->pProtoType->eSpecFunc == EISF_GemRemovalSign )
		{
			//if( pItemProto->byQuality == pItemGemSign->pProtoType->byQuality )
			if( pItemProto->byLevel == pItemGemSign->pProtoType->byLevel )
			{
				signs[i] = SignIDs[i];
				gems[i] = pItemProto->dwTypeID;
				++count;
			}
		}
	}

	//for( int i = 0; i < count; ++i )
	//{
	//	if( ( MIsEquipment(gems[i]) && ( GetItemMgr().GetBagFreeSize() < count ) ) || 
	//		( !MIsEquipment(pCur->dwRewardItemTypeID) && ( GetItemMgr  ().CanAddMaxHoldItem(pCur->dwRewardItemTypeID,pCur->nItemCount) == FALSE || GetItemMgr().GetBagFreeSize() < 1 )
	//		( GetItemMgr  ().CanAddMaxHoldItem(pCur->dwRewardItemTypeID,pCur->nItemCount) == FALSE  || 
	//}
																																											  
	//if( GetItemMgr  ().GetBag().GetFreeSpaceSize() < count )
	//	return E_Compose_Bag_Full;

	if( count > 0 )
	{
		if( GetItemMgr().GetBagFreeSize() < count )
			return E_Compose_Bag_Full;

		INT64 totalSilver = count * 10 * 10000;
		if( GetCurMgr  ().GetBagSilver() >= totalSilver )
		{
			GetCurMgr().DecBagSilver(totalSilver, ELCID_GemRemoval);
			// 宝石拆除不删掉装备
			//GetItemMgr  ().DelFromBag  (equipID,ELCID_GemRemoval,1);
			int a = 0;
			for (int i = 0 ; i < n8Num ; ++i)
			{
				if( signs[i] == 0 || GT_INVALID == signs[i] )
					continue;

				bool bFinded = false;
				if( P_VALID(pItems[a-1]) )
				{
					for( int j = 0 ; j < MAX_EQUIPHOLE_NUM; ++j )
					{
						if( !P_VALID(pEquip->equipSpec.dwHoleGemID[j]) )
							continue;
						if( pEquip->equipSpec.dwHoleGemID[j] == gems[i] && j == i )
						{
							pEquip->equipSpec.dwHoleGemID[j] = GT_INVALID;
							//if(pEquip->equipSpec.byHoleNum > MAX_EQUIPHOLE_NUM)
							//	pEquip->equipSpec.byHoleNum = MAX_EQUIPHOLE_NUM;
							//else if(pEquip->equipSpec.byHoleNum > 0)
							//	pEquip->equipSpec.byHoleNum--;
							bFinded = true;
							break;
						}
					}
				}
				//else
				if( bFinded )
				{
					pItems[a++] = ItemCreator::Create  (EICM_GemRemoval,GetID  (),gems[i]);
					GetItemMgr  ().DelFromBag  (signs[i],ELCID_GemRemoval,1);
				}
			}
			
			// 判断装备是否镶有绑定的宝石
			bool bHasBindGemItem = false;
			for(INT i = 0; i < (INT)pEquip->equipSpec.byHoleNum; ++i)
			{
				if(pEquip->equipSpec.dwHoleGemID[i] == GT_INVALID || pEquip->equipSpec.dwHoleGemID[i] == 0)
				{
					continue;
				}
				BYTE byGemBindType = g_attRes.GetItemProto(pEquip->equipSpec.dwHoleGemID[i])->byBindType;
				if ( byGemBindType == EBM_Gain || byGemBindType == EBM_AccountBind )
				{
					bHasBindGemItem = true;
					break;
				}
			}

			// 装备上没有绑定的宝石和绑定附魂石时，还原其至装备表中默认的绑定状态
			if (!bHasBindGemItem && !pEquip->IsAttachSoulBind())
			{
				pEquip->Bind();
				// 改变装备的绑定状态后通知DB和客户端
				pEquip->SetUpdate(EUDBS_Update);
				GetItemMgr().SendItemBindStatus(pEquip->n64Serial, pEquip->eConType, pEquip->byBind);
			}

			// 重新计算光晕值
			CalEquipFlare(pEquip);

			GetItemMgr  ().UpdateEquipSpec(*pEquip);
			
			if( count != a )//应该不会发生不等情况
				count = a;

			GetItemMgr().Add2Bag  (pItems,count,ELCID_GemRemoval,TRUE);
		}
		else
			return E_Compose_NotEnough_Money;
	}
	else
		return E_Compose_Stuff_Not_Exist;

	return E_Compose_Consolidate_Success;
}

/**
 * 修炼法宝
 * \author Jason
 * \date 2010-4-15
 * \version v2.0.0
 * \param srcItemID 
 * \param num 
 * \param fabaoID 
 * \return 
 */
DWORD	Role::PracticeFabao(INT64 srcItemID,INT num,INT64 fabaoID,tagFabaoSpec & fabaoSpec)
{
	if(!g_world.IsFabaoOpen())
		return E_Compose_Consolidate_Lose;

	tagItem * pItem = GetItemMgr().GetBagItem(srcItemID);
	tagItem * pDstItem = GetItemMgr().GetEquipBarEquip(fabaoID);
	if( !P_VALID(pItem) )
		return E_Fabao_Practice_NoYuLingshi;
	if( EISF_YuLingZhu != pItem->pProtoType->eSpecFunc )
		return E_Fabao_Practice_NotYuLingshi;

	if( !P_VALID(pDstItem) )
		return E_Fabao_FabaoNotExist;
	if( !MIsFaBao(pDstItem) )
		return E_Fabao_Practice_NotFabao;

	INT nMaxFabaoStage = g_attRes.GetFabaoMaxStage();
	nMaxFabaoStage = min(nMaxFabaoStage,g_world.GetFabaoLevelLimit());

	tagFabao * pFabao = (tagFabao*)pDstItem;
	const tagFabaoLevelProto * pFabaoLevelProto = NULL,*pFabaoLevelProto0 = NULL;
	pFabaoLevelProto0 = g_attRes.GetFabaoLevelProto(pFabao->nSlotNum * 10 + 1);
	pFabaoLevelProto = g_attRes.GetFabaoLevelProto(nMaxFabaoStage);
	if( !P_VALID(pFabaoLevelProto) )
		return E_Fabao_Practice_NoResource;
	// 总的值
	INT nMaxValue = pFabaoLevelProto->nMinValue + pFabaoLevelProto->nLevelUpWhated;

	if( g_world.GetFabaoLevelLimit() <= pFabao->n16Stage )
		return E_Fabao_Practice_FabaoStageFull;
	if( pFabao->nPracticeValue >= nMaxValue )
		return E_Fabao_Practice_FabaoStageFull;

	if( num > pItem->GetNum() )
		num = pItem->GetNum();

	if( num > pItem->pProtoType->n16MaxLapNum )
		return E_Fabao_Practice_LingshiNumCrossBorder;
	// nMaxValue变成了当前升级需要的值
	//if( P_VALID(pFabaoLevelProto0) )
	//	nMaxValue = pFabaoLevelProto0->nMinValue ;//+ pFabaoLevelProto0->nLevelUpWhated;

	INT addValue = pItem->pProtoType->nSpecFuncVal1 * num;
	BOOL change = FALSE;
	INT nLastLevel = pFabao->n16Stage;

	if( pFabao->nPracticeValue + addValue >= nMaxValue )
	{
		tagFabaoLevelProto * p = NULL;
		//if( 40 >= pFabao->n16Stage )
		//{
		//	p = const_cast<tagFabaoLevelProto *>(g_attRes.GetFabaoLevelProto(pFabao->nSlotNum * 10 + 1));
		//}
		//else
		{
			p = const_cast<tagFabaoLevelProto *>(g_attRes.GetFabaoLevelProto(nMaxFabaoStage));
		}
		if( P_VALID(p) && pItem->pProtoType->nSpecFuncVal1 > 0 )
		{
			if( g_world.GetFabaoLevelLimit() < p->nID )
				return E_Compose_Consolidate_Lose;
		//INT temNum = ( p->nMinValue + p->nLevelUpWhated - pFabao->nPracticeValue - 1 ) / pItem->pProtoType->nSpecFuncVal1;
			//if( temNum > num )
			//	temNum = num;
			//if( temNum > 0 )
			//{
			//	addValue = pItem->pProtoType->nSpecFuncVal1 * temNum;
			//	num = temNum;
			//	change = TRUE;
			//	//for( int j = 0; j < pFabao->nSlotNum; ++j )
			//	//	if( !P_VALID(pFabao->dwWuxingBeadID[j]) )
			//	//		return E_Fabao_LingzhuWanted;
			//}
			/*else*/ if( p->nMinValue + p->nLevelUpWhated - pFabao->nPracticeValue >= 0 )
			{
				addValue = p->nMinValue + p->nLevelUpWhated - pFabao->nPracticeValue;
				num = addValue % pItem->pProtoType->nSpecFuncVal1 > 0 ? addValue / pItem->pProtoType->nSpecFuncVal1 + 1 : addValue / pItem->pProtoType->nSpecFuncVal1 ;
			}
			else
				return E_Compose_Consolidate_Lose;
		}
		else
			return E_Compose_Consolidate_Lose;
	}

	// 法宝级别限制判断，要升阶了
	INT temVal = pFabao->nPracticeValue + addValue;
	for( int i = pFabao->n16Stage; i <= nMaxFabaoStage; ++i )
	{
		const tagFabaoLevelProto * p = g_attRes.GetFabaoLevelProto(i);
		if( P_VALID(p) )
		{
			if( i > 59 && (i%10 == 0) )
			{
				if( i != pFabao->n16Stage )
				{
					if( temVal >= p->nMinValue && temVal >= p->nMinValue + p->nLevelUpWhated )
					{
						if( p->nMinUseLevel > GetLevel() )
							return E_Fabao_Level_Limitted;
						addValue = p->nMinValue + p->nLevelUpWhated - pFabao->nPracticeValue - 1;
						num = addValue % pItem->pProtoType->nSpecFuncVal1 > 0 ? addValue / pItem->pProtoType->nSpecFuncVal1 + 1 : addValue / pItem->pProtoType->nSpecFuncVal1 ;
						break;
					}
				}
			}
			if( temVal >= p->nMinValue && temVal < p->nMinValue + p->nLevelUpWhated )
			{
				if( p->nMinUseLevel > GetLevel() )
					return E_Fabao_Level_Limitted;
				break;
			}
		}
	}

	INT nHP = m_nAtt[ERA_HP];
	INT nMP = m_nAtt[ERA_MP];
	ProcEquipEffect(NULL,pFabao,pFabao->pEquipProto->eEquipPos,TRUE,FALSE);
	GetItemMgr().DelFromBag(srcItemID,ELCID_Fabao_RemoveYuLingzhu,num);

	INT nOldZhiZhi = pFabao->n16NativeIntelligence;

	pFabao->nPracticeValue += addValue;
	ItemCreator::RecalFabaoAtt(pFabao);

	if( nLastLevel != pFabao->n16Stage )
		change = TRUE;

	if( change )
	{
		//memcpy(&fabaoSpec,(tagFabaoSpec*)pFabao,sizeof(tagFabaoSpec));
		//tagNS_FairyLevelChange fabao_levelup_msg;
		//fabao_levelup_msg.dwRoleID		= GetID();
		//fabao_levelup_msg.nLevel		= pFabao->n16Stage;
		//SendMessage(&fabao_levelup_msg,fabao_levelup_msg.dwSize);
		if( nLastLevel / 10 != pFabao->n16Stage / 10 )
			SynRemoteFabaoInfo(TRUE);
		if(g_world.GetFabaoElemStuntFlag() && pFabao->n16Stage > 59)
		{
			INT nTemLast = nLastLevel;
			if( nTemLast < 60 )
				nTemLast = 59;
			pFabao->wDamageUpgradeTimes		+= pFabao->n16Stage - nTemLast;
			pFabao->wResistanceUpgradeTimes	+= pFabao->n16Stage - nTemLast;
			if( nLastLevel < 60 )
			{
				pFabao->dwFairySkill[0] = 0;
				pFabao->dwFairySkill[1] = 0;
			}
			//if( nOldZhiZhi != pFabao->n16NativeIntelligence )
			{
				if( pFabao->n16NativeIntelligence < 80 )
				{
					for( int nSkillNdx = 0; nSkillNdx < 2; ++nSkillNdx )
						if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
							pFabao->dwFairySkill[nSkillNdx] = 0;
					//pFabao->dwFairySkill[2] = 0;
				}
				else if( pFabao->n16NativeIntelligence < 90 )
				{
					for( int nSkillNdx = 0; nSkillNdx < 3; ++nSkillNdx )
						if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
							pFabao->dwFairySkill[nSkillNdx] = 0;
				}
				else if( pFabao->n16NativeIntelligence < 95 )
				{
					for( int nSkillNdx = 0; nSkillNdx < 4; ++nSkillNdx )
						if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
							pFabao->dwFairySkill[nSkillNdx] = 0;
				}
				else if( pFabao->n16NativeIntelligence >= 95 )
				{
					for( int nSkillNdx = 0; nSkillNdx < 5; ++nSkillNdx )
						if( GT_INVALID == pFabao->dwFairySkill[nSkillNdx] )
							pFabao->dwFairySkill[nSkillNdx] = 0;
				}
				if( nLastLevel < 60 )
				{
					if( P_VALID( ((tagFabaoProto*)(pFabao->pEquipProto))->dwDef60StuntID ) )
					{
						//for( int nSkillN = 0; nSkillN < MaxFairySkillNum; ++nSkillN )
							if( pFabao->dwFairySkill[0] == 0 )
								pFabao->dwFairySkill[0] = ((tagFabaoProto*)(pFabao->pEquipProto))->dwDef60StuntID;
					}
				}
			}
			if( pFabao->n16Stage % 10 == 0  || (nLastLevel / 10 != pFabao->n16Stage / 10) )
				GetItemMgr().UpdateFabaoNirVanaData2DB(*pFabao);
			ItemCreator::RealizeFabaoStunt(pFabao,nLastLevel);
		}
		// 给予配对次数奖励
		pFabao->IncBirthTimes(nLastLevel);
		if( P_VALID(m_pScript) )
			m_pScript->OnFabaoAttChange(this,pFabao,RFAE_LevelUp,pFabao->n16Stage);
	}
	ProcEquipEffect(pFabao,NULL,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);
	INT nMaxHP = GetAttValue(ERA_MaxHP);
	INT nMaxMP = GetAttValue(ERA_MaxMP);
	nHP = min(nHP, nMaxHP);
	nMP = min(nMP, nMaxMP);
	SetAttValue(ERA_HP,nHP/*,FALSE*/);
	SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 	RecalAtt();

	GetItemMgr().UpdateFabaoSpec(*pFabao);

	if( change )
	{
		tagNS_FairyLevelChange fabao_levelup_msg;
		fabao_levelup_msg.dwRoleID		= GetID();
		fabao_levelup_msg.nLevel		= pFabao->n16Stage;
		SendMessage(&fabao_levelup_msg,fabao_levelup_msg.dwSize);
	}

	return E_Success;
}
// 镶嵌法宝灵珠
DWORD	Role::InlayFabao(INT64 n64FabaoID,INT num,INT64 * pBeadIDs,tagFabaoSpec & fabaoSpec)
{
	if(!g_world.IsFabaoOpen())
		return E_Compose_Consolidate_Lose;

	tagItem * pDstItem = GetItemMgr().GetEquipBarEquip(n64FabaoID);
	if( !P_VALID(pDstItem) )
	{
		return E_Fabao_FabaoNotExist;
	}
	tagFabao * pFabao = (tagFabao*)pDstItem;
	INT nSlotnums = ItemCreator::CalFabaoSlotNum(pFabao->n16Stage );
	if(nSlotnums > MaxWuxingSlotNum)
		nSlotnums = MaxWuxingSlotNum;
	if( pFabao->nSlotNum != nSlotnums )
	{
		pFabao->nSlotNum = nSlotnums;
	}
	if( pFabao->nSlotNum < num )
		return E_Fabao_Inlay_SlotLTLingzhu;

	if( !P_VALID(pBeadIDs) )
		return E_Fabao_Inlay_NotLingzhu;

	const tagFabaoProto * pFabaoProto = g_attRes.GetFabaoProto(pFabao->dwTypeID);
	if( !P_VALID(pFabaoProto) )
		return E_Fabao_UnkownError;

	INT nHP = m_nAtt[ERA_HP];
	INT nMP = m_nAtt[ERA_MP];
	ProcEquipEffect(NULL,pFabao,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);
	pFabao->bySlotLingzhuMatch = 0;

	BOOL re = E_Success;

	for( int i = 0 ; i < num ; ++i )
	{
		tagItem * pItem = GetItemMgr().GetBagItem(pBeadIDs[0]);
		if( P_VALID(pItem) )
		{
			if( EISF_LingZhu == pItem->pProtoType->eSpecFunc )
			{
				if( pItem->pProtoType->nSpecFuncVal1 > pFabao->n16Stage || ( pItem->pProtoType->nSpecFuncVal2 != EFB_NULL && pItem->pProtoType->nSpecFuncVal2 != ((tagFabaoProto*)pFabao->pProtoType)->eFarbaoType) )
				{
					re = E_Fabao_Inlay_Mismatch;
					break;
				}
				const tagConsolidateItem* p = g_attRes.GetConsolidateProto(pItem->dwTypeID);
				if( P_VALID(p) )
				{
					int ndx = ItemCreator::RandomFabaoWuxingHole(pFabao);
					if( ndx == GT_INVALID )
					{
						re = E_Fabao_UnkownError;
						break;
					}

					pFabao->bySlotMacthNdx[ndx] = 0;
					if( p->eWuXing == pFabao->eWuxingHole[ndx] ) //灵槽匹配
					{
						pFabao->bySlotMacthNdx[ndx] = 1;
					}
					//if( !pFabao->IsBind() )
					//	pFabao->Bind();
					pFabao->dwWuxingBeadID[ndx] = pItem->dwTypeID;
				}
				GetItemMgr().DelFromBag(pBeadIDs[i],ELCID_Fabao_RemoveLingzhu,1);
			}
		}
	}
	for(int i = 0; i < pFabao->nSlotNum; ++i)
		if( pFabao->bySlotMacthNdx[i] )
			pFabao->bySlotLingzhuMatch++;
	ProcEquipEffect(pFabao,NULL,pFabao->pEquipProto->eEquipPos,TRUE,TRUE);
	INT nMaxHP = GetAttValue(ERA_MaxHP);
	INT nMaxMP = GetAttValue(ERA_MaxMP);
	nHP = min(nHP, nMaxHP);
	nMP = min(nMP, nMaxMP);
	SetAttValue(ERA_HP,nHP/*,FALSE*/);
	SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 	RecalAtt();
	GetItemMgr().UpdateFabaoSpec(*pFabao);
	return re;
}

// 法宝五行逆转
DWORD	Role::ReverseFabao(INT64 dstFabaoID,INT64 srcFabaoID,tagFabaoSpec & fabaoSpec)
{
	if(!g_world.IsFabaoOpen())
		return E_Compose_Consolidate_Lose;

	tagEquip * pMainFabao0 = GetItemMgr().GetEquipBarEquip(dstFabaoID);
	tagItem * pSecFabao0	= GetItemMgr().GetBagItem(srcFabaoID);
	if( !P_VALID(pMainFabao0) || !P_VALID(pSecFabao0) )
		return E_Fabao_Not_Exist;

	if( !MIsFaBao(pMainFabao0) && !MIsFaBao(pSecFabao0) )
		return E_Fabao_TypeMismatch;

	tagFabao * pMainFabao = (tagFabao*)pMainFabao0;
	tagFabao * pSecFabao	= (tagFabao*)pSecFabao0;

	INT nMaxFabaoStage = g_attRes.GetFabaoMaxStage();

	if(pMainFabao->nSlotNum < MaxWuxingSlotNum )
		return E_Fabao_LevelLimmit;
	if( !((tagFabaoProto*)pMainFabao->pProtoType)->bCanReset )
		return E_Fabao_NotPermitted;
	if( !((tagFabaoProto*)pSecFabao->pProtoType)->bCanUse )
		return E_Fabao_CannotBeAider;

	if( pSecFabao->n16Stage < 20 )
		return E_Fabao_SecondeFabaoLevelLower;

	INT nHP = m_nAtt[ERA_HP];
	INT nMP = m_nAtt[ERA_MP];
	ProcEquipEffect(NULL,pMainFabao,pMainFabao->pEquipProto->eEquipPos,TRUE,FALSE);

	EWuXing tem0[] = {EWX_Metal,EWX_Wood,EWX_Water,EWX_Fire,EWX_Soil};

	INT adierFabaoStage = pSecFabao->n16Stage;
	if( adierFabaoStage > nMaxFabaoStage )
		adierFabaoStage = nMaxFabaoStage;
	//int nWuxingMatchCount = 0;
	//for( int i = 0 ; i < MaxWuxingSlotNum; ++i)
	//	if( pSecFabao->eWuxing == pSecFabao->eWuxingHole[i] )
	//		++nWuxingMatchCount;
	INT nSecFabaoStage = pSecFabao->n16Stage;
	if( nSecFabaoStage > 40 )
		nSecFabaoStage = 40;
	INT nQv = pMainFabao->n16NativeIntelligence / 2 + pSecFabao->n16NativeIntelligence / 4 + nSecFabaoStage / 4;//nWuxingMatchCount * 10 + adierFabaoStage;

	ZeroMemory(pMainFabao->bySlotMacthNdx,sizeof(pMainFabao->bySlotMacthNdx));
	pMainFabao->bySlotLingzhuMatch = 0;
	ItemCreator::MergeWuxingHoleAtt(nQv,pMainFabao,pSecFabao);

	DWORD dwSecondFabaoTypeID = pSecFabao->pProtoType->dwTypeID;

	GetItemMgr().DelFromBag(srcFabaoID,ELCID_Fabao_ReversalCosted,1);

	// 计算契合以及相生奖励
	pMainFabao->bySlotLingzhuMatch = 0;
	for( int i = 0 ; i < pMainFabao->nSlotNum ; ++i )
	{
		const tagItemProto * pBeadProto = g_attRes.GetItemProto(pMainFabao->dwWuxingBeadID[i]);
		if( !P_VALID(pBeadProto) )
			continue;

		if( EISF_LingZhu == pBeadProto->eSpecFunc )
		{
			const tagFabaoProto * pFabaoProto = (tagFabaoProto*)pMainFabao->pProtoType;
			if( pBeadProto->nSpecFuncVal1 > pMainFabao->n16Stage || ( pBeadProto->nSpecFuncVal2 != GT_INVALID && pBeadProto->nSpecFuncVal2 != pFabaoProto->eFarbaoType) )
			{
				// E_Fabao_Inlay_Mismatch;
				continue;
			}
			const tagConsolidateItem* p = g_attRes.GetConsolidateProto(pMainFabao->dwWuxingBeadID[i]);
			if( P_VALID(p) )
			{
				int ndx = i;

				pMainFabao->bySlotMacthNdx[ndx] = 0;
				if( p->eWuXing == pMainFabao->eWuxingHole[ndx] ) //灵槽匹配
				{
					pMainFabao->bySlotMacthNdx[ndx] = 1;
				}
				if( !pMainFabao->IsBind() )
					pMainFabao->Bind();

			}
		}
	}
	for(int i = 0; i < pMainFabao->nSlotNum; ++i)
		if( pMainFabao->bySlotMacthNdx[i] )
			pMainFabao->bySlotLingzhuMatch++;

	ProcEquipEffect(pMainFabao,NULL,pMainFabao->pEquipProto->eEquipPos,TRUE,TRUE);
	INT nMaxHP = GetAttValue(ERA_MaxHP);
	INT nMaxMP = GetAttValue(ERA_MaxMP);
	nHP = min(nHP, nMaxHP);
	nMP = min(nMP, nMaxMP);
	SetAttValue(ERA_HP,nHP/*,FALSE*/);
	SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 	RecalAtt();

	GetItemMgr().UpdateFabaoSpec(*pMainFabao);
	
	if( P_VALID(m_pScript) )
		m_pScript->OnFabaoAttChange(this,pMainFabao,RFAE_HoleAttReset,dwSecondFabaoTypeID);

	return E_Success;
}

// 法宝五行转生
DWORD	Role::MergeFabao(INT64 dstFabaoID,INT64 srcFabaoID,tagFabaoSpec & fabaoSpec)
{
	if(!g_world.IsFabaoOpen())
		return E_Compose_Consolidate_Lose;

	tagEquip * pMainFabao0 = GetItemMgr().GetEquipBarEquip(dstFabaoID);
	tagItem * pSecFabao0	= GetItemMgr().GetBagItem(srcFabaoID);
	if( !P_VALID(pMainFabao0) || !P_VALID(pSecFabao0) )
		return E_Fabao_Not_Exist;

	if( !MIsFaBao(pMainFabao0) && !MIsFaBao(pSecFabao0) )
		return E_Fabao_TypeMismatch;

	tagFabao * pMainFabao = (tagFabao*)pMainFabao0;
	tagFabao * pSecFabao	= (tagFabao*)pSecFabao0;

	if(pMainFabao->n16Stage < (MaxWuxingSlotNum + 1) * 10 )
		return E_Fabao_LevelLimmit;
	if( !((tagFabaoProto*)pMainFabao->pProtoType)->bCanReset )
		return E_Fabao_NotPermitted;
	if( !((tagFabaoProto*)pSecFabao->pProtoType)->bCanUse )
		return E_Fabao_CannotBeAider;

	DWORD dwSecondFabaoTypeID = pSecFabao->pProtoType->dwTypeID;

	INT nHP = m_nAtt[ERA_HP];
	INT nMP = m_nAtt[ERA_MP];
	ProcEquipEffect(NULL,pMainFabao,pMainFabao->pEquipProto->eEquipPos,TRUE,FALSE);

	INT nDifficulty = pSecFabao->n16NativeIntelligence;//pSecFabao->n16NativeIntelligence / 2 + pSecFabao->n16Stage / 2 + pMainFabao->n16NativeIntelligence / 2 + pMainFabao->n16Stage / 2;
	const tagFabaoProto * pFabaoProto = (tagFabaoProto*)pMainFabao->pProtoType;

	//INT nRandV = IUTIL->RandomInRange(0,100);
	//if( /*nRandV > 20 && */nRandV <= 10 )
	//{
	//	for( int i = 0; i < MaxWuxingRewardNUM ; ++i )
	//	{
	//		pMainFabao->nWuxingMatchReward[i] = pSecFabao->nWuxingMatchReward[i];
	//	}
	//}
	//else //if( nRandV > 30 )
	//{
	//	INT v = 0;
	//	for( int i = 0; i < MaxWuxingRewardNUM; ++i )
	//	{
	//		v = ItemCreator::SpecRandFabaoWuxingRewardID(nDifficulty,pMainFabao->n16NativeIntelligence,pFabaoProto->eFarbaoType,i+1,pMainFabao->nWuxingMatchReward);
	//		if( v > 0 )
	//			pMainFabao->nWuxingMatchReward[i] = v;
	//	}
	//}

	GetItemMgr().DelFromBag(srcFabaoID,ELCID_Fabao_MergingCosted,1);

	ProcEquipEffect(pMainFabao,NULL,pMainFabao->pEquipProto->eEquipPos,TRUE,TRUE);
	INT nMaxHP = GetAttValue(ERA_MaxHP);
	INT nMaxMP = GetAttValue(ERA_MaxMP);
	nHP = min(nHP, nMaxHP);
	nMP = min(nMP, nMaxMP);
	SetAttValue(ERA_HP,nHP/*,FALSE*/);
	SetAttValue(ERA_MP,nMP/*,FALSE*/);
// 	RecalAtt();

	GetItemMgr().UpdateFabaoSpec(*pMainFabao);

	if( P_VALID(m_pScript) )
		m_pScript->OnFabaoAttChange(this,pMainFabao,RFAE_WuxingXSReset,dwSecondFabaoTypeID);

	return E_Success;
}

DWORD Role::Extraction(DWORD dwNPCID, INT64 n64Item)
{
	tagItem* pItem = this->GetItemMgr().GetBagItem(n64Item);
	if (!P_VALID(pItem))
	{
		return E_Extraction_EquipInvalid;
	}

	if (!MIsEquipment(pItem->dwTypeID))
	{
		return E_Extraction_NotEquip;
	}
	tagEquip* pEquip = (tagEquip*)pItem;
	const tagExtractProto* pProto = g_attRes.GetExtractProto(pItem->dwTypeID);
	if (!P_VALID(pProto))
	{
		return E_Extraction_NoFormula;
	}

	if (pEquip->equipSpec.byQuality < pProto->byQuality)
	{
		return E_Extraction_QualityLimit;
	}

	if (GetAttValue(ERA_Vitality) < pProto->dwVitalityConsume)
	{
		return E_Extraction_VitalityLimit;
	}

	if (GetCurMgr().GetBagSilver() < pProto->dwMoneyConsume)
	{
		return E_Extraction_MoneyLimit;
	}

	INT nMaxOutNum = 0;
	for(int j=0; j<MAX_EXTRACT_OUTPUT; ++j)
	{
		if (!P_VALID(pProto->dwItemID[j]))
		{
			continue;
		}

		nMaxOutNum++;
	}

	if (GetItemMgr().GetBagFreeSize() < nMaxOutNum)
	{
		return E_Extraction_BagFull; 
	}

	for(INT i = 0; i < MAX_EXTRACT_OUTPUT; ++i)
	{
		if(pProto->dwItemID[i] == GT_INVALID)
			break;

		// 计算成功率
		BOOL bResult = IUTIL->Rand() % 10000 <= (pProto->dwRate[i]);

		if(bResult)
		{
			const INT nMin = pProto->nItemMinNum[i];
			const INT nMax = pProto->nItemMaxNum[i];
			INT nStuffNum = IUTIL->RandomInRange(nMin, nMax);

			tagItem *pItem = CreateItem(EICM_Product,  m_dwID, pProto->dwItemID[i], (INT16)nStuffNum, m_dwID);

			if( P_VALID(pItem) )
			{
				GetItemMgr().Add2Bag(pItem, ELCID_Compose_Extract, TRUE);
			}
			
		}		
	}

	INT nVitality = GetSpecSkillValue(ESSF_Valicity,ESSV_COMPOSE,pProto->dwVitalityConsume);

	GetCurMgr().DecBagSilver((INT64)pProto->dwMoneyConsume, ELCID_Compose_Extract);
	//ChangeVitality(-((INT)pProto->dwVitalityConsume));
	ChangeVitality(-((INT)nVitality));
	GetItemMgr().ItemUsedFromBag(n64Item, 1, ELCID_Compose_Extract);

	return E_Success;
}

//计算绑定和非绑定老君仙露需要消耗个数
// VOID Role::CalculateItemCostNum(DWORD dwTypeID, TList<tagItem*>& itemListBind,TList<tagItem*>& itemListUnBind,INT& nLjxlNum ,INT& nItemBindNum, INT& nItemUnBindNum)
// {
// 	if(GetItemMgr().GetBagSameItemCount(dwTypeID) < nLjxlNum)
// 	{
// 		//nLjxlNum = GetItemMgr().GetBagSameItemCount(pItem->dwTypeID);
// 		if (REBORN_STUFF_BINDING == dwTypeID)
// 		{
// 			nItemBindNum = GetItemMgr().GetBagSameItemList(itemListBind,dwTypeID);
// 			nItemUnBindNum = GetItemMgr().GetBagSameItemList(itemListUnBind,REBORN_STUFF_UNBINDING,nLjxlNum - nItemBindNum);
// 
// 			//如果两种老君仙露个数之和不够
// 			if(nItemBindNum + nItemUnBindNum < nLjxlNum)
// 			{
// 				nLjxlNum = nItemBindNum + nItemUnBindNum;
// 			}
// 			else
// 			{
// 				nItemUnBindNum = nLjxlNum - nItemBindNum;
// 			}
// 		}
// 		else if(REBORN_STUFF_UNBINDING == dwTypeID)
// 		{
// 			nItemUnBindNum = GetItemMgr().GetBagSameItemList(itemListUnBind,dwTypeID);
// 			nItemBindNum = GetItemMgr().GetBagSameItemList(itemListBind,REBORN_STUFF_BINDING,nLjxlNum - nItemUnBindNum);
// 			//如果两种老君仙露个数之和不够
// 			if(nItemBindNum + nItemUnBindNum < nLjxlNum)
// 			{
// 				nLjxlNum = nItemBindNum + nItemUnBindNum;
// 			}
// 			else
// 			{
// 				nItemBindNum = nLjxlNum - nItemUnBindNum;
// 			}
// 		}
// 	}
// 	else 
// 	{
// 		if (REBORN_STUFF_BINDING == dwTypeID)
// 		{
// 			nItemBindNum = GetItemMgr().GetBagSameItemList(itemListBind,dwTypeID,nLjxlNum);
// 			nItemBindNum = nLjxlNum;
// 		}
// 		else if(REBORN_STUFF_UNBINDING == dwTypeID)
// 		{
// 			nItemUnBindNum = GetItemMgr().GetBagSameItemList(itemListUnBind,dwTypeID,nLjxlNum);	
// 			nItemUnBindNum = nLjxlNum;
// 		}
// 	}
// }

