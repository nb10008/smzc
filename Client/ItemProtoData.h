#pragma once

#include ".\BaseData.h"
#include "..\WorldDefine\ItemDefine.h"
#include "..\WorldDefine\suit_define.h"
#include "..\WorldDefine\pet_equip_define.h"
#include "..\WorldDefine\longhun_define.h"
#include "..\WorldDefine\compose_define.h"

struct tagSpinProtoList
{
    DWORD   dwBasic;
    DWORD   dwMiddle;
    DWORD   dwSenior;
};

// 妖精果实
struct tagFairyFruit
{
	DWORD dwTypeID;
	BYTE byMin;
	BYTE byMax;
	BYTE byType;
	BYTE byValue;
};
//妖精提升次数add by xp
struct tagFairyUpgradeTimes
{
	DWORD dwID;
	BYTE byType;
	INT32 n32Money;
};

//! \struct tagTreasureProto
//! \brief 氏族珍宝列表,与itemid关联，有可能是物品也有可能是装备
//! \author hyu
struct tagTreasureProto
{
    DWORD		dwTreasureID;	//!< 氏族珍宝id,与物品id无关
    DWORD		dwItemID;		//!< 物品id,与itemid关联
    INT			nConNeed;		//!< 所需消耗氏族贡献值
    ECLanType	eCtype;			//!< 所属氏族类型
    DWORD		dwShopID;		//!< 商店ID

    tagTreasureProto() {ZeroMemory(this, sizeof(*this));}
};
//! \struct tagEquipQltyEffect
//! \brief 装备品级影响表
//! \author hqzhang
struct tagEquipQltyEffect
{
	// 基础属性影响系数
	FLOAT fWeaponFactor;
	FLOAT fArmorFactor;

	// 一级属性
	INT32 nAttAFactor;
	FLOAT fAttAFactor;
	INT32 nAttANumEffect;

	// 潜力值
	FLOAT fPotFactor;

	// 镶嵌孔数量 -- 记录出现的几率
	INT32 nHoleNumPct[MAX_EQUIPHOLE_NUM + 1];

	// 龙魂能力 -- 表里
	INT32 nLonghunPct[X_LONGHUN_TYPE_NUM];

	// 特殊属性
	INT32 nSpecAttPct;

	// 最大可强化等级
	INT32 nMaxStrengthenLevel;
};

/** \class	ItemProtoData
	\brief	物品、装备原始属性结构存储
*/
class ItemProtoData
	: public BaseData
{
public:
	ItemProtoData(void);
	~ItemProtoData(void);

	static ItemProtoData* Inst();

	/** 载入属性文件
	*/
	virtual void LoadFromFile();
	/** 获取物品基础结构
	*/
	const tagItemProto* FindItemProto(DWORD dwID)
	{
		if(MIsEquipment(dwID))
			return FindInMap(m_mapEquipProto, dwID);
	
		return FindInMap(m_mapItemProto, dwID);
	}
	/** 获取装备基础结构
	*/
	const tagEquipProto* FindEquipProto(DWORD dwID)
	{
		return FindInMap(m_mapEquipProto, dwID);
	}
	/** 获取法宝基础结构
	*/
	const tagFabaoProto* FindTrumpProto(DWORD dwID)
	{
		return FindInMap(m_mapTrumpProto, dwID);
	}
	/** 获取套装基础结构
	*/
	const tagSuitProtoClient* FindSuitProto(DWORD dwID)
	{
		return FindInMap(m_mapSuitProto, dwID);
	}
	/** 获取物品的显示结构
	*/
	const tagItemDisplayInfo* FindItemDisplay(DWORD dwID)
	{
		const tagItemProto* pProto = FindItemProto(dwID);
		if(P_VALID(pProto))
		{
			return pProto->pDisplayInfo;
		}
		return NULL;
	}
	//! 获得珍宝
	const tagTreasureProto* FindTreasureByTrID(DWORD dwID)
	{
		return FindInMap(m_mapTreasureProto, dwID);
	}

    //! 通过itemid和氏族类型获得第一个珍宝原型
	const tagTreasureProto* FindTreasureByItemID(DWORD dwID, ECLanType eType)
	{
        multimap<DWORD, tagTreasureProto>::_Paircc range;
        multimap<DWORD, tagTreasureProto>::const_iterator it;

        range = m_mapTreasureProtoByItemID.equal_range(dwID);
        for (it = range.first; it != range.second; ++it)
        {
            if (it->second.eCtype == eType)
            {
                return &it->second;
            }
        }

        return NULL;
	}

	//zxzhang  通过装备ID来获取装备鉴定几率 2010-1-8
	const tagEquipQltyPct* FindEquipQltyPctByItemID(DWORD dwID)
	{
		return FindInMap(m_mapEquipQltyPct, dwID);
	}
             

	const map<DWORD, tagTreasureProto>* GetTreasureProto()
	{
		return &m_mapTreasureProto;
	}

    //! 通过物品原型id获得宠物装备原型
    const tagPetEquipProto* FindPetEquipByItemID(DWORD dwID);
    //! 通过宠物装备原型id获得宠物装备
    const tagPetEquipProto* FindPetEquipByEquipID(DWORD dwID);

	const tagEquipQltyEffect* FindEquipQltyEffect(INT nQlty)
	{
		return FindInMap(m_mapEquipQlty, nQlty);
	}

	const tagHolyEquipProto* FindHolyEquipProto(DWORD dwID)
	{
		return FindInMap(m_mapHolyEquipProto, dwID);
	}

    ESuitAttType FindFashionSuitType(DWORD dwTypeID);
    ESpinStep FindFashionSpinStep(DWORD dwTypeID);
    DWORD FindNextFashionSpinID(DWORD dwTypeID, ESuitAttType eType);

    int FindFashionSpinRate(int itemCount, ESpinStep eStep);

	tstring FindTrumpAwardByID( DWORD dwID );

	tstring GetFairyEffect( DWORD dwTypeID, INT nLevel );
	tstring GetFairyPic( DWORD dwTypeID, INT nLevel );
	const tagFairyFruit* FindFairyFruit(DWORD dwTypeID);
	const tagFairySkill* FindFairySkill(DWORD dwTypeID);
	const tagFairyUpgradeTimes* FindFairyUpgradeTimes(DWORD dwTypeID);//add by xp

	const vector<DWORD>& GetFairyExpBall()
	{
		return m_vecFairyExpBall;
	}

	const tagIDBuyBack* FindIDBuyBack(DWORD dwID)	//根据id查询结构体
	{
		return FindInMap(m_mapIDBuyBack, dwID);
	}
	int GetIDBuyBackLevel(DWORD dwExp)
	{
		const tagIDBuyBack* pTemp1 = FindIDBuyBack(1);
		const tagIDBuyBack* pTemp2;
		if(!P_VALID(pTemp1) || dwExp <= 0)
			return 0;

		if(dwExp <= pTemp1->dwExp)
			return 1;

		for(int i = 2; i <= 100; i++)
		{
			pTemp1 = FindIDBuyBack(i-1);
			pTemp2 = FindIDBuyBack(i);
			if(P_VALID(pTemp1) && P_VALID(pTemp2))
			{
				if(dwExp >= pTemp1->dwExp && dwExp < pTemp2->dwExp)
					return i-1;
			}
			if( i == 100)
				return 100;
		}

		return 0;
	}
	tagIDBuyBackEx* GetIDBuyBackEx()
	{
		return &m_tagIDBuyBackEx;
	}

	const tagEquipGrowName* FindEquipGrowProto(DWORD dwID)	//获取装备成长Proto
	{
		return FindInMap(m_mapEquipGrow, dwID);
	}

	BYTE GetEquipGrowLevel(DWORD dwExp, const tagEquipGrowName* pEquipGrowName)
	{
		if(!P_VALID(pEquipGrowName))
			return 0;

		DWORD dwTotalValue = 0;
		for(int i = 0; i < EQUIP_GROW_MAX_LEVEL; i++)
		{
			dwTotalValue = GetEquipGrowCurMaxExp(i, pEquipGrowName);
			if( dwExp < dwTotalValue)
				return i;

			if(i == EQUIP_GROW_MAX_LEVEL-1)
				return EQUIP_GROW_MAX_LEVEL - 1;
		}

		return 0;
	}

	DWORD GetEquipGrowCurMaxExp(BYTE byLevel, const tagEquipGrowName* pEquipGrowname)
	{
		if(byLevel < 0 || byLevel == 255)
			return 0;

		DWORD dwResult = 0;
		for(int i = 0; i <= byLevel; i++)
		{
			dwResult += pEquipGrowname->dwExp[i];
		}
		return dwResult;
	}

	const tagFairyFusionRetProto* GetFauryFusionRet(INT byScore)
	{
		map<DWORD, tagFairyFusionRetProto>::iterator it;
		for(it = m_mapFairyFusionRet.begin(); it != m_mapFairyFusionRet.end(); ++it)
		{
			if(byScore >= (*it).second.byLowScore && byScore <= (*it).second.byHighScore)
				return &((*it).second);
		}
		return NULL;
	}

	const tagSoulAttName* FindShenLingName(DWORD dwID)	//圣灵属性表
	{
		return FindInMap(m_mapShengLingName, dwID);
	}

	const tagHolyEquipEnhance* FindHolyEquipEnhance(DWORD dwID)	//圣纹强化表
	{
		return FindInMap(m_mapHolyEquipEnhance, dwID);
	}

	const tagHolyLevelUpProto* GetHolyLevelProto(DWORD dwSocre)
	{
		map<DWORD, tagHolyLevelUpProto>::iterator it;
		for(it = m_mapShengLingLevelUp.begin();it != m_mapShengLingLevelUp.end(); ++it)
		{
			if( dwSocre <= (*it).second.dwID )
				return &((*it).second);
		}

		return NULL;
	}

	const tagWarReward* GetWarReward(DWORD dwLevel)
	{
		return FindInMap(m_mapWarReward, dwLevel);
	}

private:
	map<DWORD, tagItemProto>		    m_mapItemProto;				//物品原始结构
	map<DWORD, tagEquipProto>		    m_mapEquipProto;			//装备原始结构
	map<DWORD, tagFabaoProto>			m_mapTrumpProto;			//法宝原始结构
	map<DWORD, tagSuitProtoClient>	    m_mapSuitProto;				//套装原始结构
	map<DWORD, tagTreasureProto>	    m_mapTreasureProto;			//氏族珍宝原始数据
	map<DWORD, tagTrumpWuXingAward>	    m_mapTrumpWuXingAward;		//五行奖励原始结构
	map<DWORD, tagFairyEffect>			m_mapFairyEffect;			//精灵特效
	multimap<DWORD, tagTreasureProto>	m_mapTreasureProtoByItemID;	//通过itemid查找氏族珍宝
    map<DWORD, tagPetEquipProto>        m_mapPetEquipProtoByID;     //通过宠物装备id查找装备
	map<INT, tagEquipQltyEffect>		m_mapEquipQlty;				//装备品级影响表
	map<DWORD, tagSoulAttName>			m_mapShengLingName;			//圣灵属性表 
	map<DWORD, tagHolyLevelUpProto>		m_mapShengLingLevelUp;		//圣灵升级表
	map<DWORD, tagHolyEquipProto>		m_mapHolyEquipProto;		//圣灵装备表
	map<DWORD, tagHolyEquipEnhance>		m_mapHolyEquipEnhance;		//圣纹强化道具表
	map<DWORD, tagWarReward>			m_mapWarReward;				//战场奖励表

	//zxzhang 装备鉴定几率原始属性表  2010-1-8
	map<DWORD, tagEquipQltyPct>			m_mapEquipQltyPct;		// 装备鉴定几率原始属性表

    // 时装精纺原始数据
    map<DWORD, ESuitAttType>            m_mapSpinSuitType;      // 时装TypeId和套装类型对照表

    map<pair<DWORD, ESuitAttType>, tagSpinProtoList> m_mapDirector; // 精纺时装成长方向

    map<pair<DWORD, ESuitAttType>, DWORD>        m_mapSpinNull;          // 无精纺时装(value为初级精纺时装)
    map<pair<DWORD, ESuitAttType>, DWORD>        m_mapSpinBasic;         // 初级精纺时装(value为中级精纺时装)
    map<pair<DWORD, ESuitAttType>, DWORD>        m_mapSpinMiddle;        // 中级精纺时装(value为高级精纺时装)

    set<DWORD>                          m_SetSpinBasic;         // 初级精纺的时装
    set<DWORD>                          m_setSpinMiddle;        // 中级精纺的时装
    set<DWORD>                          m_setSpinSenior;        // 高级精纺的时装

    map<pair<int, ESpinStep>, int>      m_mapSpinRate;          // 时装精纺机率

	map<DWORD, tagFairyFruit> m_mapFairyFruit;	// 妖精水果
	map<DWORD, tagFairySkill> m_mapFairySkill;	// 妖精技能
	map<DWORD, tagFairyUpgradeTimes> m_mapFairyUpgradeTimes;	// 妖精增加次数add by xp
	vector<DWORD> m_vecFairyExpBall;

	map<DWORD, tagIDBuyBack> m_mapIDBuyBack;	//账号回购
	tagIDBuyBackEx m_tagIDBuyBackEx;

	map<DWORD, tagEquipGrowName> m_mapEquipGrow;	//装备成长属性表
	map<DWORD, tagFairyFusionRetProto> m_mapFairyFusionRet;	//妖精融合几率表
};