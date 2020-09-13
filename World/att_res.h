//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: att_res.h
// author: Sxg
// actor:
// data: 2008-05-28
// last:
// brief: 资源管理器定义
//-----------------------------------------------------------------------------
#pragma once

#include "../ServerDefine/role_data_define.h"
#include "../WorldDefine/ItemDefine.h"
#include "../WorldDefine/skill_define.h"
#include "../WorldDefine/buff_define.h"
#include "../WorldDefine/trigger_define.h"
#include "../WorldDefine/compose_define.h"
#include "../WorldDefine/MapAttDefine.h"
#include "../WorldDefine/mall_define.h"
#include "../WorldDefine/variable_len.h"
#include "../WorldDefine/guild_define.h"
#include "../WorldDefine/stall_define.h"
#include "../WorldDefine/family_define.h"
#include "../ServerDefine/pet_define.h"
#include "../ServerDefine/consolidate_define.h"
#include "../ServerDefine/title_define.h"
#include "../ServerDefine/instance_define.h"
#include "../ServerDefine/att_res_define.h"
#include "../WorldDefine/pet_define.h"
#include "../ServerDefine/guild_define.h"
#include "famehall_part.h"
#include "../ServerDefine/item_define.h"
#include "../WorldDefine/master_apprentice.h"
#include "../worlddefine/questdef.h"
#include "../Worlddefine/quest_board.h"
#include "quest_board.h"
#include "../ServerDefine/consolidate_define.h"
#include "wardrobe.h"
#include "..\WorldDefine\pulse_define.h"
#include "..\WorldDefine\msg_serverinfo.h"

struct	tagMotionProto;
struct	tagPetProto;
struct	tagPetSkillProto;
struct	tagPetEquipProto;
struct	tagPetGatherProto;
struct	tagPetWuXingProto;
struct	tagVNBEquipProto;
struct	tagVNBGiftProto;
struct	tagItemProto;
struct	tagShopProto;
struct	tagDakProto;
struct	tagSuitProto;
struct	tagMallItemProto;
struct	tagMallPackProto;
struct	tagSSpawnPointProto;
struct	tagGuildFacilities;
struct	tagGuildUpgradeNeed;
struct	tagCofCSellGood; 
struct	tagCofCBuyGood;
struct	tagCityProto;
struct	tagMiracleSkill;
struct	tagMasterApprenticeProto;
struct  tagTalentAvailCost;
struct  tagWuXunProto;
struct  tagQuestUnimpededToken;
struct	tagBoardQuestProto;
struct	tagFConsolidateLuckySignPct;
struct  tagFAttachSoulProto;
struct  tagRecommendingItemInfo;
struct	tagClergyProto;
struct	tagGodAreaProto;
struct	tagExtractProto;
struct	tagFamilySpriteProto;
struct  tagSpriteTrainProto;
struct  tagSoulAttProto;

const INT                   MAX_WUXUN_LEVEL        = 10;                // 角色最大武勋等级
class	Quest;

//-----------------------------------------------------------------------------
class AttRes
{
public:
	typedef VOID* (WINAPI* Fun_p)(LPVOID, LPCTSTR, INT32);

public:
	AttRes();
	BOOL Init();
	VOID Destroy();

public: // 上层确保互斥 -- 最好是在地图线程的上层线程执行
	// 重新加载指定属性文件
	BOOL ReloadItemProto();

	// 重新加载商城数据
	BOOL ReloadMallProto();

	// 重新加载神之竞标数据
	BOOL ReloadGodBidProto();

	// 重新加载砸金蛋概率表
	BOOL ReLoadGoldEggPctProto();

	// 重新加载需要记log的物品id -- item_needlog表
	// 发消息tagNDBC_LoadItemNeedLog即可

public:
	// 根据TypeID重新设置是否记录log
	VOID ResetItemLog(DWORD dwTypeID[], INT32 nNum);

public:
	//-------------------------------------------------------------------------------------------
	// 升级
	//-------------------------------------------------------------------------------------------
	const tagLevelUpEffect* GetLevelUpEffect(INT32 nLevel) const { return &m_LevelUpEffect[nLevel]; }
	const tagWuXunProto&  GetWuXunProto(INT nIndex)const   {  ASSERT(nIndex <= MAX_WUXUN_LEVEL && nIndex > -1); return m_WuXunProto[nIndex]; }
	//-------------------------------------------------------------------------------------------
	// 购买天资
	//-------------------------------------------------------------------------------------------
     tagTalentAvailCost* GetTalentAvailCost(INT32 nTalentNum) { return m_mapTalentAvailCost.Peek(nTalentNum);}
     
	 //-------------------------------------------------------------------------------------------
	 // 进地图的Buff
	 //-------------------------------------------------------------------------------------------
	 tagMapBuffInfo*  GetMapBuffInfo(DWORD dwMapID)      {  return m_mapMapBuffInfo.Peek(dwMapID); }  
	 //-------------------------------------------------------------------------------------------
	 // 各种开关
	 //-------------------------------------------------------------------------------------------
	 bool				GetServerFunctionSwitch(EServerFunctionSwitch eVal);
	 DWORD				GetServerFunctionSwitch();
	 //-------------------------------------------------------------------------------------------
	 // 默认值
	 //-------------------------------------------------------------------------------------------
	INT GetAttDefRole(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nDefRole; }
	INT GetAttDefCreature(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nDefCreature; }
	INT GetAttMin(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nMin; }
	INT GetAttMax(INT nIndex) { ASSERT( nIndex > ERA_Null && nIndex < ERA_End ); return m_AttDefMinMax[nIndex].nMax; }

	//-------------------------------------------------------------------------------------------
	// 过滤词表
	//-------------------------------------------------------------------------------------------
	std::vector<tstring>* GetNameFilterWords() 	{ return &m_vectNameFilter; }
	//std::vector<tstring>* GetChatFilterWords() 	{ return &m_vectChatFilter; }

	//-------------------------------------------------------------------------------------------
	// 不同语言版本名称长度
	//-------------------------------------------------------------------------------------------
	const tagVariableLen& GetVariableLen() const { return m_VarLen; }

	//-------------------------------------------------------------------------------
	// 获取权限表
	//-------------------------------------------------------------------------------
	const tagGuildPower& GetGuildPower(EGuildMemberPos eGuildPos) const
	{
		return m_GuildPowerDefault[eGuildPos];
	}

	const tagGuildKick& GetGuildKick(EGuildMemberPos eGuildPos) const
	{
		return m_GuildKickDefault[eGuildPos];
	}

	const tagGuildAppoint& GetGuildAppoint(EGuildMemberPos eGuildPos) const
	{
		return m_GuildAppointDefault[eGuildPos];
	}

	//-------------------------------------------------------------------------------
	// 获取帮派设施升级需求信息
	//-------------------------------------------------------------------------------
	BOOL GetGuildUpgradeBaseInfo(BYTE eType, BYTE byLevel, OUT tagGuildFacilities& sInfo);
	BOOL GetGuildUpgradeItemInfo(BYTE eType, BYTE byLevel, OUT tagGuildFacilities& sInfo);

	//-------------------------------------------------------------------------------
	// 获取帮派事务信息
	//-------------------------------------------------------------------------------
	const tagGuildAffairInfo* GetGuildAffairInfo(DWORD dwBuffID)	{ return m_GuildAffairInfo.Peek(dwBuffID); }

	//-------------------------------------------------------------------------------
	// 获取帮派技能信息
	//-------------------------------------------------------------------------------
	BOOL GetGuildSkillInfo(DWORD dwSkillID, INT nLevel, tagGuildSkill& sGuildSkill);
	BOOL LoadGuildSkillInfo(TMap<DWORD, tagGuildSkill*>& mapGuildSkill);
	const tagGuildSkill* GetGuildSkillProto(DWORD dwSkillID)		{ return m_GuildSkillInfo.Peek(dwSkillID); }
	VOID InitGuildSkillMaxLvlInfo();			// 初始化公会技能上限map
	INT GetGuildSkillMaxLvl(DWORD dwSkillID);	// 取最大公会技能最大等级，没有返回-1

	//-------------------------------------------------------------------------------
	// 获取帮派跑商信息
	//-------------------------------------------------------------------------------
	const tagCommerceInfo*		GetGuildCommerceInfo(INT nLevel);
	const tagCommodityProto*	GetCommodityProto(DWORD dwGoodID)	{ return m_GuildCommodityProto.Peek(dwGoodID); }
	const tagCofCSPProto*		GetCofCSPProto(DWORD dwCofCID)		{ return m_CofCSPProto.Peek(dwCofCID); }
	const FLOAT					GetCofCProfit(DWORD dwDstID, DWORD dwSrcID);
	BOOL LoadCofCGoodInfo(DWORD dwCofCID, TMap<DWORD, tagCofCSellGood*>& mapGoodSell,
		TMap<DWORD, tagCofCBuyGood*>& mapGoodBuy);
	
	//-------------------------------------------------------------------------------------------
	// 物品
	//-------------------------------------------------------------------------------------------
	tagItemProto* GetItemProto(DWORD dwTypeID)			{ return m_mapItemProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// 装备
	//-------------------------------------------------------------------------------------------
	tagEquipProto* GetEquipProto(DWORD dwTypeID)		{ return m_mapEquipProto.Peek(dwTypeID); }
	//-------------------------------------------------------------------------------------------
	// 精华(宝石,印记,龙魂等)
	//-------------------------------------------------------------------------------------------
	tagGemProto* GetGemProto(DWORD dwTypeID)			{ return m_mapGemProto.Peek(dwTypeID); }
	//-------------------------------------------------------------------------------------------
	// 商店
	//-------------------------------------------------------------------------------------------
	tagShopProto* GetShopProto(DWORD dwShopID)			{ return m_mapShopProto.Peek(dwShopID); }

	//-------------------------------------------------------------------------------------------
	// 商场商品, 礼品包及免费物品
	//-------------------------------------------------------------------------------------------
	const tagMallItemProto* GetMallItemProto(DWORD dwID){ return m_mapMallItemProto.Peek(dwID); }
	const tagMallPackProto* GetMallPackProto(DWORD dwID){ return m_mapMallPackProto.Peek(dwID); }
	const tagMallFreeItem*	GetMallFreeProto()			{ return &m_MallFreeItemProto; }
	const tagRecommendingItemInfo* GetMallRecommendingItemInfo(DWORD dwID) { return  m_mapMallRecommendingItemInfo.Peek(dwID);}
	const DWORD				GetMallCfg(EMallCfgAttr eAtt){ return (EMCA_End == eAtt) ? 0 : m_dwMallCfg[eAtt]; }
	
	TMap<DWORD, tagMallItemProto*>& GetMallItem()		 { return m_mapMallItemProto; }
	TMap<DWORD, tagMallPackProto*>& GetMallPack()		 { return m_mapMallPackProto; }
	TMap<DWORD, tagRecommendingItemInfo*>& GetMallRecommendingItem() { return m_mapMallRecommendingItemInfo; }

	INT	GetMallItemNum()								{ return m_mapMallItemProto.Size(); }
	INT GetMallPackNum()								{ return m_mapMallPackProto.Size(); }
	INT GetMallRecommendingItemNum()                    { return m_mapMallRecommendingItemInfo.Size(); }

	//-------------------------------------------------------------------------------------------
	// 驿站
	//-------------------------------------------------------------------------------------------
	tagDakProto* GetDakProto(DWORD dwDakID)				{ return m_mapDakProto.Peek(dwDakID); }
	//-------------------------------------------------------------------------------------------
	// 装备品级鉴定几率
	//-------------------------------------------------------------------------------------------
	tagEquipQltyPct* GetEquipQltyPct(DWORD dwTypeID)	{ return m_mapEquipQltyPct.Peek(dwTypeID); }
	//-------------------------------------------------------------------------------------------
	// 套装静态属性
	//-------------------------------------------------------------------------------------------
	const tagSuitProto* GetSuitProto(DWORD dwSuitID)	{ return m_mapSuitProto.Peek(dwSuitID); }
	//-------------------------------------------------------------------------------------------
	// 品级装备属性参数
	//-------------------------------------------------------------------------------------------
	const tagEquipQltyEffect* GetEquipQltyEffect(INT32 nQlty) const { return &m_EquipQltyEffect[nQlty]; }

	//-------------------------------------------------------------------------------------------
	// 元神信息属性
	//-------------------------------------------------------------------------------------------
	const tagSoulAttProto* GetSoulAttProto(DWORD dwID) { return m_mapSoulAttProto.Peek(dwID); }
	//-------------------------------------------------------------------------------------------
	// 元神装备强化属性
	//-------------------------------------------------------------------------------------------
	const tagStarEnhanceProto* GetStarEnhanceProto(BYTE byID) { return m_mapStarEnhanceProto.Peek(byID); }

	//-------------------------------------------------------------------------------------------
	// 元神转生属性
	//-------------------------------------------------------------------------------------------
	const tagHolySoulReborn* GetSoulRebornProto(BYTE byNum) { return m_mapSoulRebornProto.Peek(byNum); }

	// 元神凿孔属性
	//-------------------------------------------------------------------------------------------
	const tagHolySoulEquipHole* GetSoulEquipHoleProto(BYTE byID) { return m_mapSoulEquipHoleProto.Peek(byID); }

	// 元神镶嵌属性
	//-------------------------------------------------------------------------------------------
	const tagHolySoulEquipInlay* GetSoulEquipInlayProto(BYTE byID) { return m_mapSoulEquipInlayProto.Peek(byID); }

	//-------------------------------------------------------------------------------------------
	// 龙魂能力
	//-------------------------------------------------------------------------------------------
	const tagLongHunProto* GetLongHunProto(DWORD dwID) { return m_mapLongHunProto.Peek(dwID); }
	
	const std::vector<DWORD>& GetLongHunSpecVect(INT nType, INT nTypeEx, INT nQlty) const
	{
		return m_vectLongHun[nType][nTypeEx][nQlty];
	}

	//-------------------------------------------------------------------------------------------
	// 时装品级对生成影响参数
	//-------------------------------------------------------------------------------------------
	const tagFashionGen* GetFashionQltyEffect(INT32 nQlty) const { return &m_FashionGen[nQlty]; }

	//-------------------------------------------------------------------------------------------
	// 时装颜色生成概率参数
	//-------------------------------------------------------------------------------------------
	const tagFashionColorPct* GetFashionColorPct(INT32 nQlty) const { return &m_FashionColorPct[nQlty]; }
	
	//-------------------------------------------------------------------------------------------
	// 得到满足要求所有物品的指针列表
	//-------------------------------------------------------------------------------------------
	TList<tagItemProto*> GetItemProtoList();
	//-------------------------------------------------------------------------------------------
	// 得到铭纹属性对应的装备是否可强化
	//-------------------------------------------------------------------------------------------
	BOOL IsPosyPos(EPosyAtt ePosyAtt, EEquipPos eEquipPos);
	//-------------------------------------------------------------------------------------------
	// 得到镌刻属性对应的装备是否可强化
	//-------------------------------------------------------------------------------------------
	BOOL IsEngravePos(EEngraveAtt ePosyAtt, EEquipPos eEquipPos);

	//-------------------------------------------------------------------------------------------
	// 技能
	//-------------------------------------------------------------------------------------------
	const tagSkillProto* GetSkillProto(DWORD dwTypeID) { return m_mapSkillProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// 状态
	//-------------------------------------------------------------------------------------------
	const tagBuffProto* GetBuffProto(DWORD dwTypeID) { return m_mapBuffProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// 触发器
	//-------------------------------------------------------------------------------------------
	const tagTriggerProto* GetTriggerProto(DWORD dwTriggerID) { return m_mapTriggerProto.Peek(dwTriggerID); }

	//-------------------------------------------------------------------------------------------
	// 某个技能是否能被其它技能影响
	//-------------------------------------------------------------------------------------------
	BOOL CanBeModified(DWORD dwSkillID) { return m_mapSkillModify.IsExist(dwSkillID); }

	//-------------------------------------------------------------------------------------------
	// 得到能影响某个技能的技能列表
	//-------------------------------------------------------------------------------------------
	tagSkillModify* GetSkillModifier(DWORD dwSkillID) { return m_mapSkillModify.Peek(dwSkillID); }

	//-------------------------------------------------------------------------------------------
	// 怪物
	//-------------------------------------------------------------------------------------------
	const tagCreatureProto* GetCreatureProto(DWORD dwTypeID) { return m_mapCreatureProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// 怪物AI
	//-------------------------------------------------------------------------------------------
	const tagCreatureAI* GetCreatureAI(DWORD dwAIID) { return m_mapCreatureAI.Peek(dwAIID); }

	//-------------------------------------------------------------------------------------------
	// 非随机副本地图刷怪点
	//-------------------------------------------------------------------------------------------
	const tagSSpawnPointProto *GetSSpawnPointProto(DWORD dwSpawnPointID) { return m_mapSSpawnPoint.Peek(dwSpawnPointID); }

	//-------------------------------------------------------------------------------------------
	// 副本随机刷怪点
	//-------------------------------------------------------------------------------------------
	const tagRandSpawnPointInfo* GetSpawnPointProto(DWORD dwSpawnPoint) { return m_mapSpawnPointProto.Peek(dwSpawnPoint); }

	//-------------------------------------------------------------------------------------------
	// 副本随机刷怪点
	//-------------------------------------------------------------------------------------------
	const tagInstance*	GetInstanceProto(DWORD dwMapID)	{ return m_mapInstanceProto.Peek(dwMapID); }

	//-------------------------------------------------------------------------------------------
	// 副本中不能使用的物品
	//-------------------------------------------------------------------------------------------
	const tagInstanceItem* GetInstanceItem(DWORD dwMapID) { return m_mapInstanceItem.Peek(dwMapID); }

	//-------------------------------------------------------------------------------------------
	// 副本中不能使用的技能
	//-------------------------------------------------------------------------------------------
	const tagInstanceSkill* GetInstanceSkill(DWORD dwMapID) { return m_mapInstanceSkill.Peek(dwMapID); }

	//-------------------------------------------------------------------------------------------
	// 副本随机刷怪点等级映射表
	//-------------------------------------------------------------------------------------------
	const tagLevelMapping* GetLevelMapping(INT nLevel) { return m_mapLevelMapping.Peek(nLevel); }

	//-------------------------------------------------------------------------------------------
	// 铭纹
	//-------------------------------------------------------------------------------------------
	const tagPosyProtoSer* GetPosyProto(DWORD dwPosyID) { return m_mapPosyProto.Peek(dwPosyID); }

	//-------------------------------------------------------------------------------------------
	// 镌刻
	//-------------------------------------------------------------------------------------------
	const tagEngraveProtoSer* GetEngraveProto(DWORD dwEngraveID) { return m_mapEngraveProto.Peek(dwEngraveID); }

	//-------------------------------------------------------------------------------------------
	// 镶嵌,烙印,龙附
	//-------------------------------------------------------------------------------------------
	const tagConsolidateItem* GetConsolidateProto(DWORD dwTypeID) { return m_mapConsolidateProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// 合成
	//-------------------------------------------------------------------------------------------
	const tagProduceProtoSer* GetProduceProto(DWORD dwFormulaID) { return m_mapProduceProto.Peek(dwFormulaID); }

	//-------------------------------------------------------------------------------------------
	// 点化,装备分解
	//-------------------------------------------------------------------------------------------
	const tagDeComposeProtoSer* GetDeComposeProto(DWORD dwFormulaID) { return m_mapDeComposeProto.Peek(dwFormulaID); }

	//-------------------------------------------------------------------------------------------
	// 萃取装备
	//-------------------------------------------------------------------------------------------
	const tagExtractProto* GetExtractProto(DWORD dwEquipTypeID) { return m_mapExtractProto.Peek(dwEquipTypeID); }

	//-------------------------------------------------------------------------------------------
	// 淬火
	//-------------------------------------------------------------------------------------------
	const tagQuenchProtoSer *GetQuenchProto(DWORD dwFormulaID) { return m_mapQuenchProto.Peek(dwFormulaID); }

	//-------------------------------------------------------------------------------------------
	// 称号
	//-------------------------------------------------------------------------------------------
	const tagTitleProto *GetTitleProto(UINT16 u16TitleID) { return &m_TitleProto[u16TitleID]; }

	//-------------------------------------------------------------------------------------------
	// 氏族珍宝
	//-------------------------------------------------------------------------------------------
	const tagClanTreasureProto *GetClanTreasureProto(UINT16 u16TreasureID) { return &m_ClanTreasureProto[u16TreasureID]; }

	VOID GetRandVNBEquipProto(std::list<tagVNBEquipProto*>& listEquips)	
	{	
		m_mapVNBEquipProto.ExportAllValue(listEquips);
	}

	const tagVNBGiftProto*	GetRandVNBGiftProto()	
	{	
		tagVNBGiftProto* pProto = NULL;
		DWORD dwId = GT_INVALID;
		if (!m_mapVNBGiftProto.RandPeek(dwId, pProto) || !P_VALID(pProto))
		{
			return NULL;
		}
		else
		{
			return pProto;
		}
	}

	const tagCreatureAI* RandGetCreatureAI()
	{
		DWORD dwAIID = GT_INVALID;
		tagCreatureAI* pAI = NULL;

		m_mapCreatureAI.RandPeek(dwAIID, pAI);

		return pAI;
	}
	const tagCreatureAI* RandGetCreatureAIInGroup(DWORD dwGroupID)
	{
		TList<DWORD>* pList = m_mapCreatureAIGroup.Peek(dwGroupID);
		if( P_VALID(pList) && pList->Size() > 0 )
		{
			DWORD dwAIID = GT_INVALID;
			pList->RandPeek(dwAIID);

			return m_mapCreatureAI.Peek(dwAIID);
		}
		else
		{
			return NULL;
		}
	}

	//-------------------------------------------------------------------------------------------
	// 获得宠物属性默认最小最大值
	//-------------------------------------------------------------------------------------------
// 	INT GetPetDef(INT nPetAtt) { ASSERT(IS_EPA(nPetAtt));	return m_nPetAttDefMinMax[nPetAtt].nDef;	}
// 	INT GetPetMin(INT nPetAtt) { ASSERT(IS_EPA(nPetAtt));	return m_nPetAttDefMinMax[nPetAtt].nMin;	}
// 	INT GetPetMax(INT nPetAtt) { ASSERT(IS_EPA(nPetAtt));	return m_nPetAttDefMinMax[nPetAtt].nMax;	}

	//-------------------------------------------------------------------------------------------
	// 获得宠物原型
	//-------------------------------------------------------------------------------------------
	const tagPetProto* GetPetProto(DWORD dwPetTypeID) { return m_mapPetProto.Peek(dwPetTypeID); }

	//-------------------------------------------------------------------------------------------
	// 获得宠物升级原型
	//-------------------------------------------------------------------------------------------
	const tagPetLvlUpProto* GetPetLvlUpProto(DWORD dwVLevel) { return m_mapPetLvlUpProto.Peek(dwVLevel); }

	//-------------------------------------------------------------------------------------------
	// 获得宠物升级原型
	//-------------------------------------------------------------------------------------------
	const tagPetLvlUpItemProto* GetPetLvlUpItemProto(DWORD dwTypeID) { return m_mapPetLvlUpItemProto.Peek(dwTypeID); }

	//-------------------------------------------------------------------------------------------
	// 获得宠物技能原型
	//-------------------------------------------------------------------------------------------
	const tagPetSkillProto* GetPetSkillProto(DWORD dwPetSkillTypeID) { return m_mapPetSkillProto.Peek(dwPetSkillTypeID); }

	//-------------------------------------------------------------------------------------------
	// 获得宠物装备原型
	//-------------------------------------------------------------------------------------------
	const tagPetEquipProto* GetPetEquipProto(DWORD dwPetEquipTypeID) { return m_mapPetEquipProto.Peek(dwPetEquipTypeID); }

	//-------------------------------------------------------------------------------------------
	// 获得宠物五行凝结原型
	//-------------------------------------------------------------------------------------------
	const tagPetWuXingProto* GetPetWuXingProto(DWORD dwPetWuXingTypeID) { return m_mapPetWuXingProto.Peek(dwPetWuXingTypeID); }

	//-------------------------------------------------------------------------------------------
	// 获得宠物技能数组
	//-------------------------------------------------------------------------------------------
	const std::list<DWORD>& GetPetNormalSkillList(INT nPetLevel) { return m_PetLevelSkillVec[nPetLevel - 1]; }

	//-------------------------------------------------------------------------------------------
	// VIP摊位信息
	//-------------------------------------------------------------------------------------------
	const INT32 GetVIPStallRent(INT nIndex) { return m_nVIPStallRent[nIndex]; }

	//-------------------------------------------------------------------------------------------
	// 获得动作原型
	//-------------------------------------------------------------------------------------------
	const tagMotionProto* GetMotionProto(DWORD dwTypeID) { return m_mapMotionProto.Peek(dwTypeID); }

	
	//-------------------------------------------------------------------------------------------
	// 获取任务特殊道具处理原型表
	//-------------------------------------------------------------------------------------------
	const tagQuestUseSpeciItem * GetQuestUseSpeciItem(INT16 n16QuestID) {return m_mapQuestUseSpeciItemProto.Peek(n16QuestID); }


	//-------------------------------------------------------------------------------------------
	// 获取神迹击杀对手获得经验原型表
	//-------------------------------------------------------------------------------------------
	const tagKillerExp*	GetKillObjExpProto(INT16 n16Level)	{ return m_mapKillObjExp.Peek(n16Level); }
	//-------------------------------------------------------------------------------------------
	// 城市属性信息
	//-------------------------------------------------------------------------------------------
	const tagCityProto* GetCityProto(DWORD dwCityID)	{ return m_mapCityProto.Peek(dwCityID); }
	VOID GetCityIDRange(DWORD& dwMinID, DWORD& dwMaxID);

	//-------------------------------------------------------------------------------------------
	// 获取师徒精武原型表
	//-------------------------------------------------------------------------------------------
	const tagMasterApprenticeProto* GetMasterApprenticeProto(DWORD dwLevelID)	{ return m_mapMasterApprenticeProto.Peek(dwLevelID); }

	//-------------------------------------------------------------------------------------------
	// 龙之试炼系统
	//-------------------------------------------------------------------------------------------
	const tagTrainAttProto* GetOneTrainAttProto(DWORD dwTrainID)	{ return m_mapTrainAttProto.Peek(dwTrainID); }
	TMap<DWORD, tagTrainAttProto*>& GetTrainAttProto()	{ return m_mapTrainAttProto; }

	//-------------------------------------------------------------------------------------------
	// 获取一个角色天生技能原型
	//-------------------------------------------------------------------------------------------
	TMap<DWORD, tagInitialSkill*>& GetInitialSkillMap()	{ return m_mapInitialSkill; }
	tagInitialSkill* GetInitialSkillProto(DWORD dwID)		{ return m_mapInitialSkill.Peek(dwID); }
	DWORD GetInitialSkillCount()		{ return m_mapInitialSkill.Size(); }

	// 聊天频道开关
	DWORD GetChatFlag()const {return m_dwChatChannelFlag; }

	//-------------------------------------------------------------------------------------------
	// 仙界技能
	//-------------------------------------------------------------------------------------------
	const tagMiracleSkill* GetMSkillProto(DWORD dwSkillTypeID)	{ return m_mapMiracleSkillProto.Peek(dwSkillTypeID); }

	//-------------------------------------------------------------------------------------------
	// 精纺概率
	//-------------------------------------------------------------------------------------------
	const tagSpinSuccessRateProto* GetSpinRateProto(DWORD dwItemNum);

	//-------------------------------------------------------------------------------------------
	// 精纺原型
	//-------------------------------------------------------------------------------------------
	const tagSpinProto*	GetSpinProto(tagSpinKey key) { return m_mapSpinProto.Peek(key); }	

private:
	//-------------------------------------------------------------------------------------------
	// 设置初始化某个资源的一条记录的处理函数
	//-------------------------------------------------------------------------------------------
	VOID SetInitOneRes(Fun_p pFun) { m_pFun = pFun; }

	//-------------------------------------------------------------------------------------------
	// 初始化资源的模版函数 -- 存入map中
	//-------------------------------------------------------------------------------------------
	template<class K, class T> 
	BOOL LoadResMap(TMap<K, T*> &mapRes, LPCTSTR szFileName, LPCTSTR szFileName2 = NULL);

	//-------------------------------------------------------------------------------------------
	// 释放资源的模版函数
	//-------------------------------------------------------------------------------------------
	template<class K, class T> VOID FreeResMap(TMap<K, T*> &mapRes);

	//-------------------------------------------------------------------------------------------
	// 初始化资源的模版函数 -- 存入array中
	//-------------------------------------------------------------------------------------------
	template<class T>
	BOOL LoadResArray(T *arrayRes, INT32 nIndexStart, INT32 nIndexEnd, LPCTSTR szFileName);

private:
	//-------------------------------------------------------------------------------------------
	// 初始化过滤词表
	//-------------------------------------------------------------------------------------------
	BOOL InitFilterWords(OUT vector<tstring>& vectFilterWords, LPCTSTR szFileName);
	BOOL InitChatFilterWords(OUT vector<tstring>& vectFilterWords, LPCTSTR szFileName);

	//-------------------------------------------------------------------------------------------
	// 从脚本管理器中获取变量长度
	//-------------------------------------------------------------------------------------------
	VOID InitVarLen();

private:

	//-------------------------------------------------------------------------------------------
	// 加载购买天资的相应花费数据
	//-------------------------------------------------------------------------------------------
	VOID* InitTalentAvailCost(OUT LPVOID pData, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 加载能进指定地图的buff数据
	//-------------------------------------------------------------------------------------------
	VOID* InitMapBuffInfo(OUT LPVOID pData, IN LPCTSTR szField, INT32 nDummy);
    
	//-------------------------------------------------------------------------------------------
	// 初始化物品的一条记录
	//-------------------------------------------------------------------------------------------
	VOID* InitOneItemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化装备的一条记录
	//-------------------------------------------------------------------------------------------
	VOID* InitOneEquipProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化宝石等的一条记录
	//-------------------------------------------------------------------------------------------
	VOID* InitOneGemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化套装的一条记录
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSuitProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化一类装备品级鉴定几率
	//-------------------------------------------------------------------------------------------
	VOID* InitOneEquipQltyPct(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化元神基本属性表
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSoulAttProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);
	//-------------------------------------------------------------------------------------------
	// 初始化元神装备强化属性表
	//-------------------------------------------------------------------------------------------
	VOID* InitOneStarEnhanceProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	//初始化元神转生属性表
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSoulRebornProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	//初始化元神凿孔属性表
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSoulEquipHoleProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);
	//-------------------------------------------------------------------------------------------
	//初始化元神镶嵌属性表
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSoulEquipInlayProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化龙魂能力表
	//-------------------------------------------------------------------------------------------
	VOID* InitOneLongHunProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化商店
	//-------------------------------------------------------------------------------------------
	VOID* InitOneShopProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化商场商品及礼品包
	//-------------------------------------------------------------------------------------------
	VOID  InitOneMallItemProtoBase(OUT LPVOID pProtoType, IN LPCTSTR szField);
	VOID* InitOneMallItemProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);
	VOID* InitOneMallPackProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);
	VOID* InitOneMallRecommendingItemInfo(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化驿站
	//-------------------------------------------------------------------------------------------
	VOID* InitOneDakProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化技能的一条记录
	//-------------------------------------------------------------------------------------------
    VOID* InitOneSkillProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化buff的一条记录
	//-------------------------------------------------------------------------------------------
	VOID* InitOneBuffProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化trigger的一条记录
	//-------------------------------------------------------------------------------------------
	VOID* InitOneTriggerProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化Creature的一条记录
	//-------------------------------------------------------------------------------------------
	VOID* InitOneCreatureProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT32 nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化Creature_ai的一条记录
	//-------------------------------------------------------------------------------------------
	VOID* InitOneCreatureAIProto(OUT LPVOID pProtoType, IN LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化铭纹的一条记录
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePosyProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化镌刻的一条记录
	//--------------------------------------------------------------------------------------------
	VOID* InitOneEngraveProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化镶嵌,烙印,龙附的一条记录
	//--------------------------------------------------------------------------------------------
	VOID* InitOneConsolidateProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化生活技能的一条记录
	//--------------------------------------------------------------------------------------------
	VOID* InitOneProduceProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 点火,通用分解
	//--------------------------------------------------------------------------------------------
	VOID* InitOneDeComposeProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 萃取
	//--------------------------------------------------------------------------------------------
	VOID* InitOneExtractProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 淬火
	//--------------------------------------------------------------------------------------------
	VOID* InitOneQuenchProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 副本中不能使用的物品
	//--------------------------------------------------------------------------------------------
	VOID* InitOneInsItemProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 副本中不能使用的技能
	//--------------------------------------------------------------------------------------------
	VOID* InitOneInsSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 副本随机刷怪点
	//--------------------------------------------------------------------------------------------
	VOID* InitOneSpawnPointProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 副本动态刷怪点等级映射表
	//--------------------------------------------------------------------------------------------
	VOID* InitOneLevelMapping(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化副本静态数据
	//--------------------------------------------------------------------------------------------
	VOID* InitOneInstanceProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化非副本刷怪点原型
	//--------------------------------------------------------------------------------------------
	VOID* InitOneSSpawnPointProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化宠物属性默认最小最大值
	//--------------------------------------------------------------------------------------------
//	VOID InitPetAttDefMinMax(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//--------------------------------------------------------------------------------------------
	// 初始化宠物原型
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化宠物升级原型
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetLvlUpProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化宠物升级物品原型
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetLvlUpItemProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化宠物技能原型
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化宠物装备原型
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetEquipProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化宠物五行凝结表原型
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetWuXingProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化金牌网吧礼品
	//--------------------------------------------------------------------------------------------
	VOID* InitOneVNBGiftProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化金牌网吧礼品
	//--------------------------------------------------------------------------------------------
	VOID* InitOneVNBEquipProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化宠物采集表
	//--------------------------------------------------------------------------------------------
	VOID* InitOnePetGatherProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	//--------------------------------------------------------------------------------------------
	// 初始化宠物技能原型
	//--------------------------------------------------------------------------------------------
	VOID InitPetSkillsVec();

	//--------------------------------------------------------------------------------------------
	// 初始化帮派设施升级需求信息
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildUpgradeProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
    
	//--------------------------------------------------------------------------------------------
	// 初始化帮派事务信息
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildAffairProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化帮派技能信息
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化帮派跑商信息
	//--------------------------------------------------------------------------------------------
	VOID* InitOneGuildCommerceProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneCommodityProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneCofCProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneCofCSPProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化个性动作
	//--------------------------------------------------------------------------------------------
	VOID* InitOneMotionProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化任务特殊道具原型表
	//--------------------------------------------------------------------------------------------
	VOID* InitQuestUseSpeciItemProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//--------------------------------------------------------------------------------------------
	// 初始化击杀对手所得经验表
	//--------------------------------------------------------------------------------------------
	VOID* InitKillObjExp(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化城市属性
	//-------------------------------------------------------------------------------------------
	VOID* InitOneCityProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);


	//-------------------------------------------------------------------------------------------
	// 初始化仙界技能
	//-------------------------------------------------------------------------------------------
	VOID* InitMiracleSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化师徒精武属性
	//-------------------------------------------------------------------------------------------
	VOID* InitOneMasterApprenticeProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//-------------------------------------------------------------------------------------------
	// 初始化精纺成功率
	//-------------------------------------------------------------------------------------------
	VOID* InitOneSpinRateProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//-------------------------------------------------------------------------------
	// 龙之试炼系统-初始化试炼属性表
	//-------------------------------------------------------------------------------
	VOID* InitOneTrainProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy );

	//-------------------------------------------------------------------------------
	// 天生技能原型
	//-------------------------------------------------------------------------------
	VOID* InitOneInitialSkillProto( OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy );

	//-------------------------------------------------------------------------------------------
	// 初始化精纺原型
	//-------------------------------------------------------------------------------------------
	BOOL InitSpinProto(OUT TMap<tagSpinKey ,tagSpinProto*>	&mapSpinProto, LPCTSTR szFileName);

	//-------------------------------------------------------------------------------------------
	// 精力系统
	//-------------------------------------------------------------------------------------------
	VOID InitSpiritSystem();

	//-------------------------------------------------------------------------------
	// 只能放在背包的物品
	//-------------------------------------------------------------------------------
	VOID InitContainerLimit();
private:
	//-------------------------------------------------------------------------------------------
	// 加载人物升级相关影响静态数据
	//-------------------------------------------------------------------------------------------
	VOID InitRoleAttLevelUp(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 加载武勋属性表数据
	//-------------------------------------------------------------------------------------------
	VOID IniWuXunProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 加载各种开关数据数据
	//-------------------------------------------------------------------------------------------
	VOID IniServerFunctionSwitch(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 加载人物怪物属性的默认值，最小值和最大值
	//-------------------------------------------------------------------------------------------
	VOID InitAttDefMinMax(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 加载品级装备属性参数
	//-------------------------------------------------------------------------------------------
	VOID InitEquipQltyEffect(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 加载时装生成参数
	//-------------------------------------------------------------------------------------------
	VOID InitFashionQltyEffect(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 加载时装生成时颜色概率
	//-------------------------------------------------------------------------------------------
	VOID InitFashionColorPct(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 可铭纹装备部位的一条记录
	//-------------------------------------------------------------------------------------------
	VOID InitOnePosyPosProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 称号的一条记录
	//-------------------------------------------------------------------------------------------
	VOID InitOneTitleProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 氏族珍宝的一条记录
	//-------------------------------------------------------------------------------------------
	VOID InitOneClanTreasureProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 初始化商城免费物品(仅有一个)
	//-------------------------------------------------------------------------------------------
	VOID InitOneMallFreeItemProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 初始化商城配置文件
	//-------------------------------------------------------------------------------------------
	VOID InitOneMallCfgProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 初始化帮派某个职位权限
	//-------------------------------------------------------------------------------------------
	VOID InitOnePosGuildPower(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);
	VOID InitOnePosGuildKick(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);
	VOID InitOnePosGuildAppoint(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

	//-------------------------------------------------------------------------------------------
	// 初始化VIP摊位
	//-------------------------------------------------------------------------------------------
	VOID InitOneVIPStallProto(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);

private:
	//-------------------------------------------------------------------------------------------
	// 加载影响某一技能，某一状态，某一触发器的所有技能列表
	//-------------------------------------------------------------------------------------------
	VOID LoadModifyMap();

	//-------------------------------------------------------------------------------------------
	// 怪物AI分组
	//-------------------------------------------------------------------------------------------
	VOID GroupCreatureAI();

	//-------------------------------------------------------------------------------------------
	// 初始化龙魂能力辅助数组
	//-------------------------------------------------------------------------------------------
	VOID InitLongHunAss();
	VOID InitLongHunCanActivePos();

private:
	TSFPTrunk<AttRes>		m_Trunk;
	TObjRef<Util>			m_pUtil;
	//TObjRef<VarContainer>	m_pVar;
	Fun_p					m_pFun;

	std::vector<tstring>				m_vectNameFilter;
	std::vector<tstring>				m_vectChatFilter;
	//std::vector<tstring>				m_vectChatFilter;

	tagVariableLen						m_VarLen;
	
	TMap<DWORD, tagItemProto*>			m_mapItemProto;
	TMap<DWORD, tagEquipProto*>			m_mapEquipProto;
	//std::vector<DWORD>					m_vecLimitedItem;

	TMap<DWORD, tagGemProto*>			m_mapGemProto;
	TMap<DWORD, tagEquipQltyPct*>		m_mapEquipQltyPct;

	TMap<DWORD, tagSoulAttProto*>       m_mapSoulAttProto;
	TMap<BYTE, tagStarEnhanceProto*>	m_mapStarEnhanceProto;
	TMap<BYTE, tagHolySoulReborn*>		m_mapSoulRebornProto;
	TMap<BYTE, tagHolySoulEquipHole*>	m_mapSoulEquipHoleProto;
	TMap<BYTE, tagHolySoulEquipInlay*>	m_mapSoulEquipInlayProto;	

	TMap<DWORD, tagLongHunProto*>		m_mapLongHunProto;
	std::vector<DWORD>					m_vectLongHun[X_LONGHUN_TYPE_NUM][X_LONGHUN_TYPEEX_NUM][X_EQUIP_QUALITY_NUM]; // 随龙魂能力辅助结构

	TMap<DWORD, tagShopProto*>			m_mapShopProto;
	TMap<DWORD, tagDakProto*>			m_mapDakProto;
   
	TMap<DWORD, tagMapBuffInfo*>        m_mapMapBuffInfo;   
	TMap<DWORD, tagMallItemProto*>		m_mapMallItemProto;
	TMap<DWORD, tagMallPackProto*>		m_mapMallPackProto;
	tagMallFreeItem						m_MallFreeItemProto;
    TMap<DWORD, tagRecommendingItemInfo*> m_mapMallRecommendingItemInfo;
	DWORD								m_dwMallCfg[EMCA_End];

	TMap<DWORD, tagSuitProto*>			m_mapSuitProto;

    TMap<DWORD, tagSkillProto*>			m_mapSkillProto;
	TMap<DWORD, tagBuffProto*>			m_mapBuffProto;
	TMap<DWORD, tagTriggerProto*>		m_mapTriggerProto;

	TMap<DWORD, tagSkillModify*>		m_mapSkillModify;

	TMap<DWORD, tagCreatureProto*>		m_mapCreatureProto;
	TMap<DWORD, tagCreatureAI*>			m_mapCreatureAI;
	TMap<DWORD, TList<DWORD>*>			m_mapCreatureAIGroup;

	// Jason 2009-12-7 神龙赐福配置文件信息
	TMap<int,	tagConfigLessingOfLoong*>	m_mapConfigInfoOfLessingLoong;

	// 购买天资的花费
	TMap<DWORD,tagTalentAvailCost*>     m_mapTalentAvailCost; 
    
	  
	tagWuXunProto                       m_WuXunProto[MAX_WUXUN_LEVEL+1];
	tagLevelUpEffect					m_LevelUpEffect[MAX_ROLE_LEVEL+1];		// 下标和等级对应
	BitSetServerFunctionSwitch			m_bsServerFunctionSwitch;				// 各种开关 [6/13/2012 zhangzhihua]
	tagAttDefMinMax						m_AttDefMinMax[ERA_End];
	tagEquipQltyEffect					m_EquipQltyEffect[X_EQUIP_QUALITY_NUM];
	tagFashionGen						m_FashionGen[X_EQUIP_QUALITY_NUM];
	tagFashionColorPct					m_FashionColorPct[X_EQUIP_QUALITY_NUM];

	// 默认帮派成员权限表
	tagGuildPower						m_GuildPowerDefault[X_GUILD_POS_NUM];
	tagGuildKick						m_GuildKickDefault[X_GUILD_POS_NUM];
	tagGuildAppoint						m_GuildAppointDefault[X_GUILD_POS_NUM];

	// 帮派设施升级所需物品
	TMap<DWORD, tagGuildUpgradeNeed*>	m_GuildUpgradeNeedInfo;

	// 帮派事务
	TMap<DWORD, tagGuildAffairInfo*>	m_GuildAffairInfo;

	// 帮派技能
	TMap<DWORD, tagGuildSkill*>			m_GuildSkillInfo;
	TMap<DWORD, INT>					m_mapSkillMaxLvl;

	// 帮派跑商
	TMap<DWORD, tagCommerceInfo*>		m_GuildCommerceInfo;
	TMap<DWORD, tagCommodityProto*>		m_GuildCommodityProto;
	TMap<INT64, tagCofCProto*>			m_CofCProto;
	TMap<DWORD, tagCofCSPProto*>		m_CofCSPProto;

	// 可铭纹装备部位 
	tagConsolidatePos					m_PosyPos[EPosyAtt_End][MAX_CONSOLIDATE_POS_QUANTITY];
	TMap<DWORD, tagPosyProtoSer*>		m_mapPosyProto;			// 铭纹静态表
	// 可镌刻装备部位
	tagConsolidatePos					m_EngravePos[EEngraveAtt_End][MAX_CONSOLIDATE_POS_QUANTITY];
	TMap<DWORD, tagEngraveProtoSer*>	m_mapEngraveProto;		// 镌刻静态表
	// 镶嵌,烙印,龙附静态表
	TMap<DWORD, tagConsolidateItem*>	m_mapConsolidateProto;
	// 合成
	TMap<DWORD, tagProduceProtoSer*>	m_mapProduceProto;
	// 淬火
	TMap<DWORD, tagQuenchProtoSer*>		m_mapQuenchProto;
	// 点化,装备分解
	TMap<DWORD, tagDeComposeProtoSer*>	m_mapDeComposeProto;
	//萃取
	TMap<DWORD, tagExtractProto*>		m_mapExtractProto;

	// 称号
	// Jason 2010-3-30
	//tagTitleProto						m_TitleProto[MAX_TITLE_NUM];
	tagTitleProto	*					m_TitleProto;
	// 氏族珍宝
	tagClanTreasureProto				m_ClanTreasureProto[CLAN_TREASURE_NUM];
	// 副本随机刷怪点
	TMap<DWORD, tagRandSpawnPointInfo*> m_mapSpawnPointProto;
	// 副本静态属性
	TMap<DWORD, tagInstance*>			m_mapInstanceProto;
	// 副本中不能使用的物品
	TMap<DWORD, tagInstanceItem*>		m_mapInstanceItem;
	// 副本中不能使用的技能
	TMap<DWORD, tagInstanceSkill*>		m_mapInstanceSkill;
	// 副本随机刷怪点等级映射表
	TMap<INT, tagLevelMapping*>			m_mapLevelMapping;
	// 非副本刷怪点
	TMap<INT, tagSSpawnPointProto*>		m_mapSSpawnPoint;

	// 宠物原型
	TMap<DWORD, tagPetProto*>			m_mapPetProto;

	// 宠物升级原型
	TMap<DWORD, tagPetLvlUpProto*>		m_mapPetLvlUpProto;

	// 宠物升级原型
	TMap<DWORD, tagPetLvlUpItemProto*>	m_mapPetLvlUpItemProto;

	// 宠物技能原型
	TMap<DWORD, tagPetSkillProto*>		m_mapPetSkillProto;
	
	// 宠物等级对应固定技能id
	std::list<DWORD>					m_PetLevelSkillVec[NUM_PET_VLEVEL];

	// 宠物装备原型
	TMap<DWORD, tagPetEquipProto*>		m_mapPetEquipProto;

	// 宠物采集表
	TMap<DWORD, tagPetGatherProto*>		m_mapPetGatherProto;

	// 宠物五行凝结表
	TMap<DWORD, tagPetWuXingProto*>		m_mapPetWuXingProto;

	TMap<DWORD, tagVNBGiftProto*>		m_mapVNBGiftProto;
	TMap<DWORD, tagVNBEquipProto*>		m_mapVNBEquipProto;
	
	// 任务对应特殊道具
	TMap<INT16, tagQuestUseSpeciItem *> m_mapQuestUseSpeciItemProto;
	
	// 神迹争夺击杀对手所得经验
	TMap<INT16, tagKillerExp *>			m_mapKillObjExp;
	
	// 宠物属性值
//	tagPetDefMinMax						m_nPetAttDefMinMax[EPA_NUM];

	// VIP摊位租金(下标与ID对应)
	INT32								m_nVIPStallRent[VIP_STALL_MAX_NUM];

	TMap<DWORD, tagMotionProto*>		m_mapMotionProto;

	// 城市属性
	TMap<DWORD, tagCityProto*>			m_mapCityProto;
	
	// 仙界技能
	TMap<DWORD, tagMiracleSkill*>		m_mapMiracleSkillProto;

	//精力系统
	BOOL								m_bSpiSysOn;						// 精力系统是否开启
	std::map<DWORD, float>				m_mapSpiRate;						// 精力对应的加成百分比
	DWORD								m_dwSpiStartLevel;					// 起效的级别
	DWORD								m_dwSpiRecoverySpeed;				// 离线恢复速度 点/分钟
	DWORD								m_dwSpiReward;						// 领奖点数
	std::map<DWORD,std::pair<int, EItemQuality> > m_mapSpiReward;			// 领取的奖品
	std::map<DWORD,int>					m_mapStrengthReward;				// 实力值兑换道具

	// 师徒精武属性
	TMap<DWORD, tagMasterApprenticeProto*>		m_mapMasterApprenticeProto;

	// 龙之试炼系统-试炼属性表
	TMap<DWORD, tagTrainAttProto*>		m_mapTrainAttProto;

	// 角色天生技能原型表
	TMap<DWORD, tagInitialSkill*>		m_mapInitialSkill;

	// 聊天频道开关
	DWORD								m_dwChatChannelFlag;

	TMap<tagSpinKey ,tagSpinProto*>			m_mapSpinProto;				// 精纺

	TMap<DWORD, tagSpinSuccessRateProto*>		m_mapSpinRateProto;			// 精纺成功率

	std::list<DWORD>					m_dwContainerLimitedItems;

	// 神职原型表
	TMap<DWORD, tagClergyProto*>		m_mapClergyProto;
	
	// 竞选神职-神系与神职ID映射表，便于根据神系直接找到该系各种神职的ID
	TMap<ECampaignGodType, DWORD>		m_mapClergyBase;
	TMap<ECampaignGodType, DWORD>		m_mapFemaleClergyBoss;
	TMap<ECampaignGodType, DWORD>		m_mapMaleClergyBoss;

	// 神职领域原型表
	TMap<DWORD, tagGodAreaProto*>		m_mapGodAreaProto;

	// 地图盒子原型表
	TMap<DWORD, tagMapBoxProto*>		m_mapMapBoxProto;

	// 领域特效原型表
	TMap<DWORD, tagAreaEffectProto*>	m_mapAreaEffectProto;
	// 具体有多少种稀有领域特效
	TMap<DWORD, DWORD>					m_mapALLAreaBuffID;

	// 守护妖精属性表
	TMap<DWORD, tagFamilySpriteProto*>	m_mapFamilySpriteProto;

	
	// 家族守护妖精修炼属性表
	TMap<DWORD, tagSpriteTrainProto*>	m_mapSpriteTrainProto;
	TMap<DWORD, tagSpriteTrainData*>	m_mapSpriteTrainData;


public:
	std::vector<tstring>* GetChatFilterWordsVec() { return &m_vectChatFilter; }

public:
	// GM命令调用
	TMap<DWORD, tagEquipProto*>			GetEquipMap(){return m_mapEquipProto;}	


	// Jason 2009-12-7 神龙赐福配置文件信息
	tagConfigLessingOfLoong* GetConfigInfoOfLessingLoong(const int & ndx);
private:
	VOID* AttRes::InitLessingLoong(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	TMap<INT64, tagPhilosophersStoneConfig*>		m_mapPhilosophersStoneConfig;
	VOID* InitPhilosophersStoneConfig(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	TObjRef<VarContainer>	m_pVar;
public:
	tagPhilosophersStoneConfig* GetConfigInfoOfPhilosophersStone(INT nStoneType,DWORD srcItemTypeID);

	// Jason 2010-1-18 v1.3.2 玩家回归相关
	template<class T,class F>
	BOOL LoadResToOtherObject(T * Obj,F f,LPCTSTR szFileName);

	//精力系统
	BOOL IsSpiritSysOn()const { return m_bSpiSysOn; }
	DWORD SpiritStartLevel()const { return m_dwSpiStartLevel; }	
	DWORD GetSpiritRSpeed()const { return m_dwSpiRecoverySpeed; }
	DWORD GetSpiritRewardValue()const { return m_dwSpiReward; }
	const std::map<DWORD, std::pair<int, EItemQuality> >& GetSpiritReward()const { return m_mapSpiReward; }
	const std::map<DWORD, float>&	GetSpiriteRate()const { return m_mapSpiRate; }						
	const std::map<DWORD, int>& GetStrengthReward()const { return m_mapStrengthReward; }

	// 物品是否只能放在背包中
	BOOL IsLimited2Bag(DWORD dwID)const { return std::binary_search(m_dwContainerLimitedItems.begin(), m_dwContainerLimitedItems.end(), dwID);}
// Jason 2010-4-13 v2.0.0法宝系统相关
private:
	TMap<INT,tagFabaoLevelProto*>	m_mapFabaoLevelProto;		// Jason 2010-4-13 v2.0.0 法宝等阶原型
	TMap<INT,tagFabaoWuxingProto*>	m_mapFabaoWuxingProto;		// Jason 2010-4-13 v2.0.0 法宝五行相生奖励原型
	TMap<DWORD,tagFabaoProto*>			m_mapFabaoProto;			// Jason 2010-4-13 v2.0.0 法宝原型

	TMap<DWORD,tagHolyProto*>			m_mapHolyProto;			// 圣灵原型
	TMap<DWORD,tagHolyLevelUpProto*>	m_mapHolyLevelUp;		// 圣灵升级原型	
	TMap<DWORD,tagHolyEquipProto*>		m_mapHolyEquipProto;	// 圣纹属性
	TMap<DWORD,tagHolyEquipEnhance*>	m_mapHolyEquipEnhance;	// 圣纹属性强化

	TMap<DWORD,tagQuestUnimpededToken*> m_mapQuestUnimpededToken;	//任务无阻令牌映射
	// Jason 2010-5-24 v2.1.2 飞升技能原型
	TMap<DWORD,tagLearnSoaringSkillProto *> m_mapLearnSoaringSkill;
	TMap<DWORD,tagCastingProto *> m_mapCastingProto;
	TMap<DWORD,tagMsgDeliver * > m_mapMsgDeliverProto;
	TMap<DWORD,tagSkillBoxProto*> m_mapSkillBoxProto;
	INT m_nFabaoMaxStage;

	VOID* InitOneFabaoLevelProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneFabaoWuxingProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	VOID* InitOneFabaoProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// 加载圣灵附加属性[wcy 2012-03-30]
	VOID* InitOneHolyProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// 加载圣灵升级表[wcy 2012-04-09]
	VOID* InitOneHolyLevelUp(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);	

	// 加载圣纹附加属性[wcy 2012-04-17]
	VOID* InitOneHolyEquipProto(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);

	// 加载圣纹强化属性[wcy 2012-04-26]
	VOID* InitOneHolyEquipEnhance(OUT LPVOID pProtoType, LPCTSTR szField, INT nDummy);
		
	VOID * InitOneQuestUnimpededToken(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy);
	// Jason
	VOID * InitOneLearnSoaringSkillProto(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy);
	VOID * InitOneCastingProto(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy);
	VOID * InitOneMsgDeliverProto(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy);

	VOID * InitOneSkillBoxProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
public:
	const tagFabaoProto * GetFabaoProto(DWORD dwTypeID);
	const tagFabaoLevelProto * GetFabaoLevelProto(INT ID);
	const tagFabaoWuxingProto * GetFabaoWuxingProto(INT ID);
	const TMap<INT,tagFabaoWuxingProto*> & GetFabaoWuxingProtoMap() const{return m_mapFabaoWuxingProto;}
	const tagQuestUnimpededToken * GetOneUnimpededToken(DWORD dwQuestID) ;

	const tagLearnSoaringSkillProto * GetOneLearnSoaringSkillProto(DWORD dwID);
	const tagCastingProto * GetOneCastingProto(DWORD dwID);
	const TMap<DWORD,tagCastingProto *> & GetCastingProto(){return m_mapCastingProto;}
	const tagMsgDeliver * GetOneMsgDeliverProto(DWORD dwID);

	const tagSkillBoxProto * GetOneSkillProto(DWORD dwID);

	const tagHolyProto * GetHolyProto(DWORD dwTypeID);

	const tagHolyEquipProto * GetHolyEquipProto(DWORD dwTypeID);

	

// 任务板相关
private:
	// 悬赏任务属性加载
	TMap<UINT16, tagBoardQuestProto*> m_mapBoardQuestProto;
	VOID* InitOneBoardQuest(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	// 悬赏任务生成规则
	tagBoardQuestRandomRate	m_BoardQuestRandRule[BOARD_QUEST_MAX_RULE_NUM];
	VOID InitOneBoardQuestRandRule(OUT LPVOID pArray, IN LPCTSTR szField, INT32 nIndex);
	// Jason 2010-6-18 强化表
	TMap<DWORD,tagFConsolidateProto*> m_mapFConsolidateProto;
	VOID * InitOneFConsolidateProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 强化之幸运符
	TMap<DWORD,tagFConsolidateLuckySignPct*> m_mapFConsolidateLuckySignPct;
	VOID * InitOneFConsolidateLuckySignPct(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 吸魂
	TMap<DWORD ,tagFAttachSoulProto*> m_mapFAttachSoulStone;
	VOID* InitOneFAttachSoulProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 神铸
	TMap<DWORD, tagGodStrengthen * > m_mapGodStrengthen;
	VOID* InitOneGodStrengthenProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 妖精食物原型
	TMap<DWORD, tagFabaoFoodProto*> m_mapFabaoFood;
	VOID * InitFabaoFoodProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 妖精果实
	TMap<DWORD, tagFabaoFruitProto*> m_mapFabaoFruit;
	VOID * InitFabaoFruitProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 妖精特技
	TMap<DWORD,tagFabaoStuntProto *> m_mapFabaoStunt;
	VOID * InitFabaoStuntProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 妖精点数提升 [3/13/2012 zhangzhihua]
	TMap<DWORD,tagFabaoAddPromoteTimesProto *> m_mapFabaoAddPromoteTimes;
	VOID * InitFabaoAddPromoteTimesProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 妖精融合 [3/22/2012 zhangzhihua]
	TMap<DWORD,tagFairyFusionQualityProto *> m_mapFairyFusionQuality;
	VOID * InitFairyFusionQualityProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	TMap<DWORD,tagFairyFusionTypeProto *> m_mapFairyFusionType;
	VOID * InitFairyFusionTypeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	TMap<DWORD,tagWarRewardPointProto *> m_mapWarRewardPoint;
	VOID * InitWarRewardPointProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 公会每日商店奖励
	TMap<DWORD,tagGuildDailyRewardProto *> m_mapGuildDailyReward;
	VOID * InitGuildDailyRewardProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 神之竞标
	TMap<DWORD,tagGodBidProto *> m_mapGodBidProto;
	VOID * InitGodBidProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

public:
	TMap<UINT16, tagBoardQuestProto*>& GetBoardQuestMap()			{ return m_mapBoardQuestProto;}
	tagBoardQuestProto* GetBoardQuestProto(UINT16 u16ID)			{ return m_mapBoardQuestProto.Peek(u16ID); }
	tagBoardQuestRandomRate* GetBoardQuestRule(INT nIndex)			{ return &m_BoardQuestRandRule[nIndex]; }

	const tagFConsolidateProto * GetOneFConsolidateProto(DWORD dwConsID);
	TMap<DWORD,tagFConsolidateProto*> & GetFConsolidateProto() {return m_mapFConsolidateProto;}

	const tagFConsolidateLuckySignPct* GetOneFConsolidateLuckySignPct(DWORD dwID);
	const tagFAttachSoulProto* GetOneFuhunStone(DWORD dwConLevelID);
	TMap<DWORD ,tagFAttachSoulProto*> & GetFAttachSoulProto(){return m_mapFAttachSoulStone;}

	const INT GetFabaoMaxStage(VOID) const { return m_nFabaoMaxStage;}

	const tagGodStrengthen * GetGodStrengthenProto(DWORD dwLevel);
	TMap<DWORD, tagGodStrengthen * > & GetGodStrengthenMap(void){return m_mapGodStrengthen;}

	// 妖精食物。。。
	const tagFabaoFoodProto* GetFabaoFoodProto(DWORD dwTypeID);
	// 查找某个类型的物品是否为妖精食物
	BOOL IsFabaoFood(DWORD dwItemTypeID);

	TMap<DWORD,tagGodBidProto *>& GetGodBidProtoMap() { return m_mapGodBidProto; }
	tagGodBidProto *GetGodBidProto(DWORD dwProtoID) { return m_mapGodBidProto.Peek(dwProtoID); }
public:
	TMap<DWORD, tagMallOfferProto*>& GetMallOfferProtoMap() { return m_mapMallOfferProto; }
	tagMallOfferProto* GetMallOfferProto(DWORD dwTypeID) { return m_mapMallOfferProto.Peek(dwTypeID); }
private:
	VOID * InitOneMallOfferProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	TMap<DWORD, tagMallOfferProto*>  m_mapMallOfferProto;

	// 妖精繁殖相关
	TMap<DWORD,TFabaoBirthProto *>		m_mapFabaoBirthProto;
	// 妖精繁殖类型细表
	TMap<DWORD,tagFabaoBirthItemTypeProto *> m_mapFabaoBirthItemTypeProto;

	// 砸金蛋
	TMap<DWORD,tagGoldEggProto *> m_mapGoldEggProto;
	TMap<DWORD,tagGoldEggPctProto *> m_mapGoldEggPctProto;

	// 神之血脉
	TMap<DWORD,tagPulseProto *> m_mapPulseProto;

	// 老虎机
	TMap<DWORD,tagTigerProto *> m_mapTigerProto;


	VOID * InitOneFabaoBirthProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID * InitOneFabaoBirthItemTypeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 砸金蛋
	VOID * InitOneGoldEggProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID * InitOneGoldEggPctProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	//神职属性
	VOID * InitOneClergyProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	TMap<DWORD,tagGodHeadProto*> m_mapGodHeadProto;
	VOID * InitOneGodHeadProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	//神职领域属性
	VOID * InitOneGodAreaProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// 经脉
	VOID * InitOnePulseProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);

	// 老虎机
	VOID * InitOneTigerProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
public:
	// 找到妖精果实
	const tagFabaoFruitProto * GetFabaoFruitProto(DWORD dwTypeID);

	TMap<DWORD,tagFabaoStuntProto *> & GetFabaoStuntProtos(VOID);
	const tagFabaoStuntProto * GetFabaoStuntProto(DWORD dwID);

	TMap<DWORD,tagFabaoAddPromoteTimesProto *> & GetFabaoAddPromoteTimesProtos(VOID);
	const tagFabaoAddPromoteTimesProto * GetFabaoAddPromoteTimesProto(DWORD dwID);

	TMap<DWORD,tagFairyFusionQualityProto *> & GetFairyFusionQualityProtos(VOID);
	const tagFairyFusionQualityProto * GetFairyFusionQualityProto(DWORD dwID);

	TMap<DWORD,tagFairyFusionTypeProto *> & GetFairyFusionTypeProtos(VOID);
	const tagFairyFusionTypeProto * GetFairyFusionTypeProto(DWORD dwID);

	const tagWarRewardPointProto * GetWarRewardPointProto(DWORD dwID);

	const tagGuildDailyRewardProto * GetGuildDailyRewardProto(INT nGuildPos, INT nWinNum);
	const tagGuildDailyRewardProto * GetGuildDailyRewardProto(DWORD dwID);

	// 取出妖精繁殖出的妖精typeid
	DWORD	RandomPeekFabaoBirthItem(INT nFabaoBirthValue,DWORD dwMainFabaoTypeID,DWORD * pFariyBirthType = NULL);

	// 砸金蛋
	tagGoldEggProto *GetGoldEggProto(DWORD dwLayer) { return m_mapGoldEggProto.Peek(dwLayer); }
	tagGoldEggPctProto *GetGoldEggPctProto(DWORD dwLayer) { return m_mapGoldEggPctProto.Peek(dwLayer); }

	// 查找神职属性原型
	const tagClergyProto *GetClergyProto(DWORD dwClergyID);
	const tagClergyProto *GetClergyBase(ECampaignGodType eType);
	const tagClergyProto *GetClergyBoss(ECampaignGodType eType, ESex eSex);

	const tagGodHeadProto * GetOneGodHeadProto(DWORD dwID);

	// 查找神职领域属性原型
	const tagGodAreaProto *GetGodAreaProto(DWORD dwGodAreaID);

	//地图盒子载入
	VOID* InitOneMapBoxProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// 查找地图盒子属性原型
	const tagMapBoxProto *GetMapBoxProto(DWORD dwID);

	//领域特效载入
	VOID* InitOneAreaEffectProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// 查找领域特效属性原型
	const tagAreaEffectProto* GetAreaEffectProto(DWORD dwID);

	//加载守护妖精属性表
	VOID* InitOneFamilySpriteProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// 查找守护妖精属性原型
	const tagFamilySpriteProto *GetFamilySpriteProto(DWORD dwID);

	//加载家族守护妖精修炼属性表
	VOID* InitOneSpriteTrainProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	// 查找家族守护妖精修炼属性原型
	const tagSpriteTrainProto *GetSpriteTrainProto(DWORD dwID);

	// 随机获得家族守护妖精修炼属性原型
	const tagSpriteTrainProto *RandGetSpriteTrainProto(INT nLevel,EFamilySpritePickType eType);

	// 经脉
	tagPulseProto *GetPulseProto(DWORD dwID) { return m_mapPulseProto.Peek(dwID); }

	// 老虎机
	tagTigerProto *GetTigerProto(DWORD dwID) { return m_mapTigerProto.Peek(dwID); }

	// 根据妖精得分获取修改圣灵属性的范围
	const tagHolyLevelUpProto * GetHolyLevelUpProto(INT nScore);

	//  获取圣纹强化道具对应的强化属性
	const tagHolyEquipEnhance * GetEquipEnhanceProto(INT dwTypeId);

public:
	DWORD GetWardrobeLayerCnt() { return m_dwWardrobeLayerCnt; }
	tagWardrobeProto* GetWardrobeProto(DWORD dwLayer) { return m_mapWardrobeProto.Peek(dwLayer); }
	tagAdvancedComposeProto* GetAdvancedComposeProto(DWORD dwDesignID) { return m_mapAdvancedComposeProto.Peek(dwDesignID); }
	TMap<DWORD,tagWardrobeProto *>& GetWardrobeProtoMap() { return m_mapWardrobeProto; }
private:
	VOID * InitOneWardrobeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID * InitOneAdvancedComposeProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID IncWardrobeLayerCnt() { m_dwWardrobeLayerCnt++; }
	TMap<DWORD,tagWardrobeProto *> m_mapWardrobeProto;
	TMap<DWORD,tagAdvancedComposeProto *> m_mapAdvancedComposeProto;
	DWORD m_dwWardrobeLayerCnt;		// 层数

// key码领取奖励表
public:
	static const int M_MaxKeyCodeRewardNum = 3;
	struct tagKeyCodeRewardProto
	{
		DWORD dwID;
		BOOL bOnlyOnce;
		DWORD dwItemTypeID[M_MaxKeyCodeRewardNum];
		INT nItemNum[M_MaxKeyCodeRewardNum];
		
		tagKeyCodeRewardProto()
		{
			ZeroMemory(this, sizeof(*this));
		}
	};
	tagKeyCodeRewardProto* GetKeyCodeReward(DWORD dwKeyCodeCrc)	{ return m_mapKeyCodeRewardProto.Peek(dwKeyCodeCrc); }
private:
	TMap<DWORD, tagKeyCodeRewardProto*> m_mapKeyCodeRewardProto;
	VOID* InitOneKeyCodeRewardProto(OUT LPVOID nProtoType,LPCTSTR szField, INT nDummy);

// 家族任务相关
private:
	TMap<DWORD, tagFamilyQuestAttProto*>	m_mapFamilyQuestCondition;
	TMap<DWORD, tagFamilyQuestProto*>		m_mapFamilyQuests;
	TMap<DWORD, tagFamilyQuestRewardProto*> m_mapFamilyRewardProto;
	VOID* InitOneFamilyQuestCondition(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID* InitOneFamilyQuestProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
	VOID* InitOneFamilyRewardProto(OUT LPVOID nProtoType,LPCTSTR szField,INT nDummy);
public:
	TMap<DWORD, tagFamilyQuestProto*>& GetFamilyQuestProto()							{ return m_mapFamilyQuests; }
	const tagFamilyQuestAttProto* GetOneFamilyQuestCondition(DWORD dwConditionID)		{ return m_mapFamilyQuestCondition.Peek(dwConditionID); }
	const tagFamilyQuestProto* GetOneFamilyQuestProto(DWORD dwQuestID)					{ return m_mapFamilyQuests.Peek(dwQuestID); }
	const tagFamilyQuestRewardProto* GetOneFamilyRewardProto(DWORD dwRoleLvl)			{ return m_mapFamilyRewardProto.Peek(dwRoleLvl);}


private:
	TMap<DWORD, tagIDBuyBack*>	m_mapIdbuybackProto;

public:
	VOID* InitOneIdbuybackProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

	const tagIDBuyBack *GetIdbuybackProto(DWORD dwID) { return m_mapIdbuybackProto.Peek(dwID); }

// 装备成长
private:
	TMap<DWORD, tagEquipGrowProto*>	m_mapEquipGrowProto;
public:
	const tagEquipGrowProto* GetEquipGrowProto(DWORD dwID) { return m_mapEquipGrowProto.Peek(dwID); }
	VOID* InitOneEquipGrowProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

//离线经验
private:
	TMap<DWORD,tagExpOfflineProto*> m_mapExpOfflineProto;

public:
	VOID* InitOneExpOfflineProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	const tagExpOfflineProto* GetExpOfflineProto(DWORD dwID) { return m_mapExpOfflineProto.Peek(dwID); }

private:
	TMap<BYTE, tagSoulCrystalLvlLine*> m_mapSoulCrystalLvlLine;
	TMap<INT, tagSoulCrystalAttData*> m_mapSoulCrystalAttData;
public:
	const tagSoulCrystalLvlLine* GetSoulCrystalLvlLine(BYTE byLvl) {return m_mapSoulCrystalLvlLine.Peek(byLvl);}
	VOID* InitOneSoulCrystalLvlLine(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	const tagSoulCrystalAttData* GetSoulCrystalAttData(INT nId) {return m_mapSoulCrystalAttData.Peek(nId);}
	VOID* InitOneSoulCrystalAttData(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);

// 神之战灵技能
private:
	TMap<DWORD,tagForceSkillProto*> m_mapForceSkillProto;
	TMap<DWORD,tagForceLevelProto*> m_mapForceLevelProto;

public:
	VOID* InitOneForceSkillProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	const tagForceSkillProto* GetForceSkillProto(DWORD dwID) { return m_mapForceSkillProto.Peek(dwID); }
	const tagForceSkillProto* GetForceSkillProtoEx(DWORD dwID);

	VOID* InitOneForceLevelProto(OUT LPVOID nProtoType, LPCTSTR szField, INT nDummy);
	const tagForceLevelProto* GetForceLevelProto(DWORD dwID) { return m_mapForceLevelProto.Peek(dwID); }
};

//*********************************** 模版方法 ************************************

//-----------------------------------------------------------------------------
// 读取资源文件模版函数(如果使用szFileName2，则field中对应的字段应该为key)
//-----------------------------------------------------------------------------
template<class K, class T>
BOOL AttRes::LoadResMap(TMap<K, T*> &mapRes, LPCTSTR szFileName, LPCTSTR szFileName2/* = NULL*/)
{
	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	// 读入文件
	m_pVar->Clear();
	if(!m_pVar->Load(NULL, szFileName, "id", &ListField))
	{
		IMSG(_T("There is something wrong when loading file<%s>!\r\n"), szFileName);
		return FALSE;
	}

	for(it = ListField.begin(); it != ListField.end(); ++it)
	{
		T* pResNode = new T;

		// 初始化
		// Jason 2009-12-12
		K * p = (K*)m_pFun(pResNode, it->c_str(), X_READ_FILE_1);
		if( !P_VALID(p) )
		{
			//IMSG(_T("There is something wrong in reading proto<%s>!\n"), szFileName);
			delete pResNode;
			continue;
		}
		K key = *p;

		// 放入map中
		mapRes.Add(key, pResNode);
	}

	// 发送读入基本物品数目信息
	IMSG(_T("Read %d records from file<%s>!\n"), mapRes.Size(), szFileName);

	// 清空容器
	m_pVar->Clear();

	// 如果文件2不为NULL，也同时读入
	if(szFileName2 != NULL)
	{
		ListField.clear();

		// 注意：此处用id2，是为了避免读入Field时与文件1冲突
		m_pVar->Load(NULL, szFileName2, "id2", &ListField);

		for(it = ListField.begin(); it != ListField.end(); ++it)
		{
			K key = (K)_atoi64(m_pUtil->UnicodeToUnicode8(it->c_str()));
			T* pResNode = mapRes.Peek(key);
			if(!P_VALID(pResNode))
			{
				ASSERT(0);
				//IMSG(_T("There is something wrong in reading proto<%s>!\n"), szFileName2);
				continue;
			}

			// 初始化
			m_pFun(pResNode, it->c_str(), X_READ_FILE_2);
		}

		// 发送读入基本物品数目信息
		IMSG(_T("Read %d records from file<%s>!\n"), ListField.size(), szFileName2);

		// 清空容器
		m_pVar->Clear();
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// 释放资源的模版函数
//-----------------------------------------------------------------------------
template<class K, class T> 
VOID AttRes::FreeResMap(TMap<K, T*> &mapRes)
{
	T *pResNode = NULL;

	mapRes.ResetIterator();
	while(mapRes.PeekNext(pResNode))
	{
		SAFE_DEL(pResNode);
	}

	mapRes.Clear();
}

//-----------------------------------------------------------------------------
// 读取资源文件模版函数
//-----------------------------------------------------------------------------
template<class T>
BOOL AttRes::LoadResArray(T *arrayRes, INT32 nIndexStart, INT32 nIndexEnd, LPCTSTR szFileName)
{
	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	// 读入文件
	if(!m_pVar->Load(NULL, szFileName,"id", &ListField))
	{
		return FALSE;
	}

	// 按顺序读取
	TCHAR szField[X_SHORT_STRING];
	for(INT32 i = nIndexStart; i <= nIndexEnd; ++i)
	{
		_stprintf(szField, _T("%d"), i);

		// 检查指定Field是否存在，如果不存在，说明读取的文件有问题
		for(it = ListField.begin(); it != ListField.end(); ++it)
		{
			if(szField == *it)
			{
				break;
			}
		}

		// 如果此处断住，说明读取的文件可能有问题
		// ASSERT(it != ListField.end());
		if(it == ListField.end())
		{
			//IMSG(_T("There is something wrong in reading proto<%s> in <%s>!\n"), szField, szFileName);
			continue;
		}

		// 初始化
		m_pFun(arrayRes, szField, i);
	}

	// 清空容器
	m_pVar->Clear();

	return TRUE;
}

template<class T,class F>
BOOL AttRes::LoadResToOtherObject(T * Obj,F f, LPCTSTR szFileName)
{
	std::list<tstring>				ListField;
	std::list<tstring>::iterator	it;

	// 读入文件
	if(!m_pVar->Load(NULL, szFileName,"id", &ListField))
	{
		return FALSE;
	}

	// 按顺序读取
	TCHAR szField[X_SHORT_STRING];
	INT nIndexStart = 0,nIndexEnd = m_pVar->GetVarNum();
	for(INT32 i = nIndexStart; i <= nIndexEnd; ++i)
	{
		_stprintf(szField, _T("%d"), i);

		// 检查指定Field是否存在，如果不存在，说明读取的文件有问题
		it = find(ListField.begin(),ListField.end(),szField);
		// 如果此处断住，说明读取的文件可能有问题
		if(it == ListField.end())
			continue;

		// 初始化
		f(Obj,m_pVar,szField);
	}

	// 清空容器
	m_pVar->Clear();

	return TRUE;
}

extern AttRes g_attRes;
