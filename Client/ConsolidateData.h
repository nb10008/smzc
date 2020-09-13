#pragma once
#include "..\WorldDefine\compose_define.h"
#include ".\BaseData.h"

class ConsolidateData
	: public BaseData
{
public:
	struct tagEquipStreng
	{
		DWORD dwID;					// 配方ID
		EItemTypeEx eTypeEx;		// 装备类型
		DWORD dwCost;				// 金钱消耗
		BYTE byTimes;				// 强化次数
		INT nStuffType;				// 需要的强化材料类型
	};
public:
	ConsolidateData(void);
	~ConsolidateData(void);

	static ConsolidateData* Inst();

	/** 载入强化属性文件
	*/
	virtual void LoadFromFile();

	/** 获取铭纹配方基础结构
	*/
	tagPosyProto* FindPosyProto(DWORD dwID)
	{
		return FindInMap(m_mapPosy, dwID);
	}
	/**获取镌刻配方基础结构
	*/
	tagEngraveProto* FindEngraveProto(DWORD dwID)
	{
		return FindInMap(m_mapEngrave, dwID);
	}
	/**获取强化道具基础结构
	*/
	tagConsolidateItem* FindConsolidateItemProto(DWORD dwID)
	{
		return FindInMap(m_mapConsolidateItem, dwID);
	}
	/**获取淬火西方基础结构
	*/
	tagQuenchProto* FindQuenchProto(DWORD dwID)
	{
		return FindInMap(m_mapQuench, dwID);
	}

	tagGodStrengthen* FindGodStrengInfo( DWORD dwID )
	{
		return FindInMap(m_mapGodStreng, dwID);
	}

	/** 获取幸运符几率
	*/
	DWORD FindLuckyPct(DWORD dwID);

	DWORD FindEquipStrengConst( EItemTypeEx eTypeEx, BYTE byTimes );

	DWORD FindEquipStrengStuff( EItemTypeEx eTypeEx, BYTE byTimes );

	DWORD FindSkillPct(INT64 n64EquipID);	//技能也能增加几率~

	/**获取铭纹map
	*/
	const map<DWORD, tagPosyProto>& GetPosyMap( )
	{
		return m_mapPosy;
	}

	/**获取镌刻map
	*/
	const map<DWORD, tagEngraveProto>& GetEngraveMap( )
	{
		return m_mapEngrave;
	}

	/**获取强化道具map
	*/
	const map<DWORD, tagConsolidateItem>& GetConsolidateItemMap( )
	{
		return  m_mapConsolidateItem;
	}
	/**获取淬火map
	*/
	const map<DWORD, tagQuenchProto>& GetQuenchMap()
	{
		return m_mapQuench;
	}
	// 获取幸运符几率表
	const map<DWORD, DWORD>& GetLuckyMap()
	{
		return m_mapLucky;
	}
	// 获取强化表
	const map<DWORD, tagEquipStreng>& GetEquipStrengMap()
	{
		return m_mapEquipStreng;
	}

private:
	map<DWORD, tagPosyProto>		m_mapPosy;			//铭纹
	map<DWORD, tagEngraveProto>		m_mapEngrave;		//镌刻
	map<DWORD, tagQuenchProto>		m_mapQuench;		//淬火
	map<DWORD, tagConsolidateItem>  m_mapConsolidateItem;	//强化道具(镶嵌,烙印,龙附)
	map<DWORD, DWORD>				m_mapLucky;			// 幸运符
	map<DWORD, tagEquipStreng>		m_mapEquipStreng;	// 强化表
	map<DWORD, tagGodStrengthen>	m_mapGodStreng;		// 神铸表
};