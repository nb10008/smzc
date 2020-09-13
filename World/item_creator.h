//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: item_creator.cpp
// author: Sxg
// actor:
// data: 2008-07-29
// last:
// brief: 物品&装备生成器 -- 物品&装备生产,及武器鉴定(按品级生成属性)
//-----------------------------------------------------------------------------
#pragma once

#include "mutex.h"

struct tagEquip;
struct tagEquipProto;
struct tagIMEffect;
struct tagFConsolidateProto;
class Role;
//-----------------------------------------------------------------------------
class ItemCreator
{
	friend class GMCommandMgr;

public:
	ItemCreator();
	~ItemCreator();

public:
	// 根据数据库读取的数据创建物品
	static tagItem* CreateItemByData(PVOID pData);
	static tagEquip* CreateEquipByData(PVOID pData);
	// 根据已有物品生成新的堆物品
	static tagItem* Create(const tagItem &item, INT16 n16Num);
	
	// 生成未鉴定物品&装备
	static tagItem* Create(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT nNum = 1,  
							DWORD dwCreator = GT_INVALID, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0, INT16 n16PotValPct = 10000,INT nPotAdding = 0,INT qlty = -1, BOOL bFixQlty = TRUE);
	// Jason 2009-12-1 增加紫色品级
	static tagItem* CreateByProduce(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID,EItemQuality eQlty, INT nPotAdding,DWORD dwPurpleQltyIdfPct = 0, INT16 n16Num = 1, 
		DWORD dwCreator = GT_INVALID, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0, INT16 n16PotValPct = 10000,INT qlty = -1);

	// 生成未鉴定珍宝(物品&装备)
	static tagItem* CreateTreasure(DWORD dwNameID, EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, INT16 n16Num = 1, 
							DWORD dwCreator = GT_INVALID, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0);

	// 生成指定品级的物品&装备
	static tagItem* CreateEx(EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, 
							INT nNum = 1, EItemQuality eQlty = EIQ_Null, DWORD dwCreator = GT_INVALID, 
							const tagIMEffect *pIMEffect = NULL)
	{
		if (eCreateMode < 0 || eCreateMode >= EICM_End)
		{
			return NULL;
		}

		tagItem *pNewItem = Create(eCreateMode, dwCreateID, dwTypeID, nNum,GT_INVALID,0,0,10000,0,eQlty);
		if(!P_VALID(pNewItem))
		{
			return NULL;
		}

		//if(MIsEquipment(dwTypeID)/* && eQlty > EIQ_Start && eQlty < EIQ_End*/)
		//{
		//	IdentifyEquip((tagEquip*)pNewItem, eQlty, pIMEffect);
		//}

		return pNewItem;
	}

	// 生成指定品级的珍宝(物品&装备)
	static tagItem* CreateTreasureEx(DWORD dwNameID, EItemCreateMode eCreateMode, DWORD dwCreateID, DWORD dwTypeID, 
		INT16 n16Num = 1, EItemQuality eQlty = EIQ_Null, DWORD dwCreator = GT_INVALID, 
		const tagIMEffect *pIMEffect = NULL);


	// 设置物品装备序号
	static VOID SetItemSerial(INT64 n64Max, INT64 n64Min) { m_n64MaxSerial = n64Max; m_n64MinSerial = n64Min; }

	// 生成世界唯一号(注意要做互锁机制)
	static VOID CreateItemSerial(INT64 &n64NewSerial);

public:
	static VOID IdentifyEquip(IN OUT tagEquip *pEquip, 
						EItemQuality eQlty = EIQ_Null, const tagIMEffect *pIMEffect = NULL, BOOL bFixQlty = TRUE);

private: 	
	// 参数合法性由上层调用函数保证
	static VOID InitItem(tagItem &item, EItemCreateMode eCreateMode, const tagItemProto *pProto, DWORD dwCreateID, INT64 n64Serial, INT nNum, DWORD dwCreator, DWORD dwCreateTime, DWORD dwAccountID, DWORD dwOwnerID);
	static VOID InitEquipSpec(tagEquipSpec &equipSpec, INT16 n16QltyModPct = 0, INT16 n16QltyModPctEx = 0, INT16 n16PotValPct = 10000);

	static INT32 GenBaseEquipQlty(DWORD dwTypeID);
	static INT32 ModEquipQltyByProduce(const tagEquip *pEquip, INT32 nQuality);
	static DWORD GenLongHunRandID(INT nLongHunType, INT nEquipPos, INT nQlty);
	
	static VOID CreateEquipQltyRel(OUT tagEquip *pEquip, const tagEquipProto *pEquipProto, EItemQuality eQlty);
	static BOOL CreateFashionQltyRel(OUT tagEquip *pEquip, const tagEquipProto *pEquipProto, 
									EItemQuality eQlty, const tagIMEffect *pIMEffect = NULL);

	static VOID ProcEquipAttBySpecAtt(tagEquip *pEquip);

	static BOOL IsGMItemNoInit(tagItem* pTmpItem);
	static BOOL InitGMItem(tagItem* pTmpItem);

	// Jason 2010-4-13 v2.0.0
	static VOID IdentifyFabao(tagFabao * pFabao,EItemQuality qlty = EIQ_Null);

	// wcy 2012-03-31 圣灵
	static VOID IdentifyHoly(tagHolyMan * pHoly,EItemQuality qlty = EIQ_Null);

	// wcy 2012-04-18 圣纹
	static VOID IdentifyHolyEquip(tagHolyEquip * pHolyEquip,EItemQuality qlty = EIQ_Null);	

	static VOID CreateEquipRoleL1Att(tagEquip * pEquip, const tagEquipProto *pEquipProto,const tagEquipQltyEffect * pEquipQltyEffect);
	static VOID CreateEquipRoleL2Att(tagEquip * pEquip, const tagEquipProto *pEquipProto);
public:
	static VOID RecalFaBaoCurStoredExp(tagFabao * pFabao);
	static BOOL RecalFabaoAtt(tagFabao * pFabao);
	// 重新计算成长等基础属性，不计算等级，为了回退使用
	static BOOL RecalFabaoAttWithoutCalStage(tagFabao * pFabao);
	static BOOL CanLevelupFabao(tagFabao * pFabao,INT nPracticeValue);
	static INT	RandomFabaoWuxingHole(tagFabao * pFabao);
	static INT RandFabaoWuxingRewardID(INT nDifficulty,INT QltyIntl,ENUM_FabaoType type,INT pos);
	static INT SpecRandFabaoWuxingRewardID(INT nDifficulty,INT QltyIntl,ENUM_FabaoType type,INT pos,INT nWuxingMatchReward[MaxWuxingRewardNUM]);
	static INT MergeWuxingHoleAtt(INT nQvalue,tagFabao * pFabao,tagFabao* pSecFabao);
	enum {EFABAO_RaiseIntelligence_Success = 0,EFABAO_RaiseIntelligence_Failse,EFABAO_RaiseIntelligence_NoChange,EFABAO_RaiseIntelligence_AlreadyFull};
	static INT ChangeFabaoNativeIntelligence(tagFabao * pFabao,INT addvalue = 1,INT nEfficacy = 0);
	static INT	CalFabaoSlotNum(INT stage);

	// Jason 2010-7-12 v1.0.0重新计算强化属性（当装备强化等级发生变化的时候调用）
	static INT RecalEquipStrengthAtt(tagEquip * pEquip,INT level);

	// 调整妖精心情
	static INT	ModFabaoMood(tagFabao * pFabao,INT nMood);
	// 获取妖精心情区间
	struct tagFabaoMoodSection
	{
		INT nMin;
		INT nMax;
	};
	static INT GetFabaoMoodSection(tagFabaoMoodSection * Mood,INT nSize,INT & num);
	// 得到妖精经验分成系数
	static FLOAT GetFabaoExpPercent(tagFabao * pFabao);
	// 得到妖精能力系数
	static FLOAT GetFabaoCapabilityPercent(tagFabao * pFabao);
	// 得到妖精心情降低量
	static INT GetFabaoFeelingdown(tagFabao * pFabao);

	// 妖精领悟特技
	static DWORD RealizeFabaoStunt(tagFabao * pFabao,INT nOldStage);

	// 洗装备一级二级属性
	static DWORD ResetEquipAtt(tagEquip * pEquip);
private:
	static INT64			m_n64MaxSerial;
	static INT64			m_n64MinSerial;
	static Mutex			m_Mutex;

};