#pragma once
#include "..\WorldDefine\compose_define.h"
#include ".\BaseData.h"

class ProduceData
	: public BaseData
{
public:
	ProduceData(void);
	~ProduceData(void);

	static ProduceData* Inst();

	/** 载入强化属性文件
	*/
	virtual void LoadFromFile();
	/** 获取合成配方基础结构
	*/
	tagProduceProto* FindProduceProto(EProduceType eType, DWORD dwID)
	{
		switch(eType)
		{
		case EPCT_Artisan:
			 return FindInMap(m_mapArtisan, dwID);
			break;
		case EPCT_DanTraining:
			return FindInMap(m_mapDanTraining, dwID);
			break;
		case EPCT_Smith:
			return FindInMap(m_mapSmith, dwID);
			break;
		case EPCT_Casting:
			return FindInMap(m_mapCasting, dwID);
			break;
		case EPCT_Dressmaker:
			return FindInMap(m_mapDressmaker, dwID);
			break;
		case EPCT_Aechnics:
			return FindInMap(m_mapAechnics, dwID);
			break;
		case EPCT_Smilt:
			return FindInMap(m_mapSmilt, dwID);
			break;
		case EPCT_GodArtisan:
			return FindInMap(m_mapGodArtisan, dwID);
			break;
		case EPCT_Gem:
			return FindInMap(m_mapBaoshi, dwID);
			break;
		case EPCT_Pet:
			return FindInMap(m_mapPet, dwID);
			break;
		case EPCT_EquipMaster:
			return FindInMap(m_mapEquipMaster, dwID);
			break;
		case EPCT_TreasureCom:
			return FindInMap(m_mapTreasureCom, dwID);
			break;
		case EPCT_FoodGod1:
			return FindInMap(m_mapFoodGod1, dwID);
			break;
		case EPCT_FoodGod2:
			return FindInMap(m_mapFoodGod2, dwID);
			break;
		case EPCT_FoodGod3:
			return FindInMap(m_mapFoodGod3, dwID);
			break;
		case EPCT_Perfume:
			return FindInMap(m_mapPerfume, dwID);
			break;
		case EPCT_EquipRefine:
			return FindInMap(m_mapEquipRefine, dwID);
			break;
		case EPCT_StuffRefine:
			return FindInMap(m_mapStuffRefine, dwID);
			break;
		case EPCT_TimeLimitEquip:
			return FindInMap(m_mapTimeLimitEquip, dwID);
			break;
		case EPCT_Crystal:
			return FindInMap(m_mapCryStal, dwID);
			break;
		case EPCT_Stole_52:
		case EPCT_Stole_67:
		case EPCT_Stole_82:
		case EPCT_Stole_Blade:
		case EPCT_Stole_Bow:
		case EPCT_Stole_Valor:
		case EPCT_Stole_Swing:
		case EPCT_SoulLinker:
		case EPCT_MoonCake:
		case EPCT_HallowmasGift:
		case EPCT_HallowmasName:
		case EPCT_MoShenEquip:
		case EPCT_FairySoulStren://add by xp
		case EPCT_FairySkillbookStren://add by xp
		case EPCT_BattleFieldCompose://add by xp
		case EPCT_ReplaceFabao://add by xp
			return FindInMap(m_mapStole, dwID);
			break;
		default:
			return NULL;
			break;
		}
		return NULL;
	}

	/**获取分解配方基础结构
	*/
	tagDeComposeProto* FinDeComposeProto(DWORD dwID)
	{
		return FindInMap(m_mapDeCompose, dwID);
	}

	/**获取生产合成map
	*/
	void GetProduceMap(map<DWORD, tagProduceProto>& outMap, EProduceType eType=EPCT_Artisan)
	{
		switch(eType)
		{
		case EPCT_Artisan:
			outMap = m_mapArtisan;
			break;
		case EPCT_DanTraining:
			outMap = m_mapDanTraining;
			break;
		case EPCT_Smith:
			outMap = m_mapSmith;
			break;
		case EPCT_Casting:
			outMap = m_mapCasting;
			break;
		case EPCT_Dressmaker:
			outMap = m_mapDressmaker;
			break;
		case EPCT_Aechnics:
			outMap = m_mapAechnics;
			break;
		case EPCT_Smilt:
			outMap = m_mapSmilt;
			break;
		case EPCT_GodArtisan:
			outMap = m_mapGodArtisan;
			break;
		case EPCT_Gem:
			outMap = m_mapBaoshi;
			break;
		case EPCT_Pet:
			outMap = m_mapPet;
			break;
		case EPCT_EquipMaster:
			outMap = m_mapEquipMaster;
			break;
		case EPCT_TreasureCom:
			outMap = m_mapTreasureCom;
			break;
		case EPCT_FoodGod1:
			outMap = m_mapFoodGod1;
			break;
		case EPCT_FoodGod2:
			outMap = m_mapFoodGod2;
			break;
		case EPCT_FoodGod3:
			outMap = m_mapFoodGod3;
			break;
		case EPCT_Perfume:
			outMap = m_mapPerfume;
			break;
		case EPCT_EquipRefine:
			outMap = m_mapEquipRefine;
			break;
		case EPCT_StuffRefine:
			outMap = m_mapStuffRefine;
			break;
		case EPCT_TimeLimitEquip:
			outMap = m_mapTimeLimitEquip;
			break;
		case EPCT_Crystal:
			outMap = m_mapCryStal;
			break;
		case EPCT_Stole_52:
		case EPCT_Stole_67:
		case EPCT_Stole_82:
		case EPCT_Stole_Blade:
		case EPCT_Stole_Bow:
		case EPCT_Stole_Valor:
		case EPCT_Stole_Swing:
		case EPCT_SoulLinker:
		case EPCT_MoonCake:
		case EPCT_HallowmasGift:
		case EPCT_HallowmasName:
		case EPCT_MoShenEquip:
		case EPCT_FairySoulStren://add by xp
		case EPCT_FairySkillbookStren://add by xp
		case EPCT_BattleFieldCompose://add by xp
		case EPCT_ReplaceFabao://add by xp
			outMap = m_mapStole;
			break;
		}
	}
	const map<DWORD, tagProduceProto>* GetProduceMap(EProduceType eType=EPCT_Artisan)
	{
		switch(eType)
		{
		case EPCT_Artisan:
			return &m_mapArtisan;
			break;
		case EPCT_DanTraining:
			return &m_mapDanTraining;
			break;
		case EPCT_Smith:
			return &m_mapSmith;
			break;
		case EPCT_Casting:
			return &m_mapCasting;
			break;
		case EPCT_Dressmaker:
			return &m_mapDressmaker;
			break;
		case EPCT_Aechnics:
			return &m_mapAechnics;
			break;
		case EPCT_Smilt:
			return &m_mapSmilt;
			break;
		case EPCT_GodArtisan:
			return &m_mapGodArtisan;
			break;
		case EPCT_Gem:
			return &m_mapBaoshi;
			break;
		case EPCT_Pet:
			return &m_mapPet;
			break;
		case EPCT_EquipMaster:
			return &m_mapEquipMaster;
			break;
		case EPCT_TreasureCom:
			return &m_mapTreasureCom;
			break;
		case EPCT_FoodGod1:
			return &m_mapFoodGod1;
			break;
		case EPCT_FoodGod2:
			return &m_mapFoodGod2;
			break;
		case EPCT_FoodGod3:
			return &m_mapFoodGod3;
			break;
		case EPCT_Perfume:
			return &m_mapPerfume;
			break;
		case EPCT_EquipRefine:
			return &m_mapEquipRefine;
			break;
		case EPCT_StuffRefine:
			return &m_mapStuffRefine;
			break;
		case EPCT_TimeLimitEquip:
			return &m_mapTimeLimitEquip;
			break;
		case EPCT_Crystal:
			return &m_mapCryStal;
			break;
		case EPCT_Stole_52:
		case EPCT_Stole_67:
		case EPCT_Stole_82:
		case EPCT_Stole_Blade:
		case EPCT_Stole_Bow:
		case EPCT_Stole_Valor:
		case EPCT_Stole_Swing:
		case EPCT_SoulLinker:
		case EPCT_MoonCake:
		case EPCT_HallowmasGift:
		case EPCT_HallowmasName:
		case EPCT_MoShenEquip:
		case EPCT_FairySoulStren://add by xp
		case EPCT_FairySkillbookStren://add by xp
		case EPCT_BattleFieldCompose://add by xp
		case EPCT_ReplaceFabao://add by xp
			return &m_mapStole;
			break;
		}
		return NULL;
	}

	/**得到点化map
	*/
	void GetDeComposeMap(map<DWORD, tagDeComposeProto>& outMap)
	{
		outMap = m_mapDeCompose;
	}
	
	map<DWORD, tagDeComposeProto>& GetDeComposeMap()
	{
		return m_mapDeCompose;
	}

	// 获取随机生产配方
	tagRandomProduceProto* FindRandomProduceProto(EAdvanceComposeType eType, DWORD dwID)
	{
		tagRandomProduceProto *pProto = FindInMap(m_mapRandomProduce, dwID);
		if(!P_VALID(pProto))
			return NULL;
		if(pProto->eRProType == eType)
			return pProto;
		else
			return NULL;
	}

	// 获取萃取配方
	tagExtractProto* FindExtractProto(DWORD dwEquipID)
	{
		tagExtractProto *pProto = FindInMap(m_mapExtractProto, dwEquipID);
		if(!P_VALID(pProto))
			return NULL;
		return pProto;
	}

private:
	map<DWORD, tagProduceProto>		m_mapArtisan;				//工匠
	map<DWORD, tagProduceProto>		m_mapDanTraining;			//练丹
	map<DWORD, tagProduceProto>		m_mapSmith;					//锻造
	map<DWORD, tagProduceProto>		m_mapCasting;				//铸甲
	map<DWORD, tagProduceProto>		m_mapDressmaker;			//裁缝	
	map<DWORD, tagProduceProto>		m_mapAechnics;				//工艺
	map<DWORD, tagProduceProto>		m_mapSmilt;					//冶练
	map<DWORD, tagDeComposeProto>	m_mapDeCompose;				//分解
	map<DWORD, tagProduceProto>		m_mapGodArtisan;			//天匠
	map<DWORD, tagProduceProto>		m_mapPet;					//灵兽
	map<DWORD, tagProduceProto>		m_mapEquipMaster;			// 装备匠师
	map<DWORD, tagProduceProto>		m_mapTreasureCom;			// 宝物合成
	map<DWORD, tagProduceProto>		m_mapFoodGod1;				// 食神1
	map<DWORD, tagProduceProto>		m_mapFoodGod2;				// 食神2
	map<DWORD, tagProduceProto>		m_mapFoodGod3;				// 食神3
	map<DWORD, tagProduceProto>		m_mapPerfume;				// 香水制作
	map<DWORD, tagProduceProto>		m_mapEquipRefine;			// 装备萃取
	map<DWORD, tagProduceProto>		m_mapStuffRefine;			// 材料凝炼
	map<DWORD, tagProduceProto>		m_mapTimeLimitEquip;		// 时限装备
	map<DWORD, tagProduceProto>		m_mapBaoshi;				// 宝石
	map<DWORD, tagRandomProduceProto>	m_mapRandomProduce;		// 随机生产
	map<DWORD, tagExtractProto>		m_mapExtractProto;			// 萃取
	map<DWORD, tagProduceProto>		m_mapCryStal;				// 水晶
	map<DWORD, tagProduceProto>		m_mapStole;					// 圣衣
	//OverSeaData	m_OverData;
};