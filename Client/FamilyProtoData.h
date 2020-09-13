#pragma once
#include "basedata.h"
#include "..\WorldDefine\family_define.h"


class FamilyProtoData : public BaseData
{
public:
	FamilyProtoData(void);
	~FamilyProtoData(void);
	VOID LoadFromFile();
	static FamilyProtoData* Inst();
public:
	// common----------------------------------------------------------------------------------------------------

	// 家族成员----------------------------------------------------------------------------------------------------

	// 家族任务----------------------------------------------------------------------------------------------------
	tagFamilyQuestName* FindQuestName(DWORD dwID) {return FindInMap(m_mapFamilyQuestName, dwID);}
	const tagFamilyQuestAttProto* FindQuestAttProto(DWORD dwID) {return FindInMap(m_mapFamilyQuestAttProto, dwID);}
	const tagFamilyQuestRewardProto* FindQuestRewardProto(DWORD dwID){return FindInMap(m_mapFamilyQuestRewardProto, dwID);}
	// 家族妖精----------------------------------------------------------------------------------------------------
	const tagFamilySpriteProto* FindFamilySpriteProto(DWORD dwID){return FindInMap(m_mapFamilySpriteProto, dwID);}
	// 这里不是const 因为可能会修改部分数据
	tagSpriteTrainClientProto* FindSpriteTrainProto(DWORD dwID){return FindInMap(m_mapFamilySpriteTrain, dwID);}
	const map<DWORD, tagSpriteTrainClientProto>& GetTrainMap(){return m_mapFamilySpriteTrain;}
private:
	// common----------------------------------------------------------------------------------------------------

	// 家族成员----------------------------------------------------------------------------------------------------

	// 家族任务----------------------------------------------------------------------------------------------------
	map<DWORD, tagFamilyQuestName> m_mapFamilyQuestName;				//家族任务表
	map<DWORD, tagFamilyQuestAttProto> m_mapFamilyQuestAttProto;		//集合表
	map<DWORD, tagFamilyQuestRewardProto> m_mapFamilyQuestRewardProto;			//奖励表

	// 家族妖精----------------------------------------------------------------------------------------------------
	map<DWORD, tagFamilySpriteProto>	m_mapFamilySpriteProto;
	map<DWORD, tagSpriteTrainClientProto>	m_mapFamilySpriteTrain;
};
