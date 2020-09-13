#pragma once
#include "basedata.h"
#include "../WorldDefine/clergy_define.h"

class GodAreaData
{
public:
	GodAreaData(void);
	~GodAreaData(void);

	void LoadFromFile();
	const tagGodAreaName* GetAreaDataByID(const DWORD& dwID);
	int GetFreePickAreaNumByGodHead(int nGodHead);
	void GetFreePickAreaListByNum(int num ,TList<DWORD>& list);
private:
	map<DWORD,tagGodAreaName> m_mapGodAreaData;
	map<DWORD,DWORD> m_mapGodHeadAreaNum;	//神格，对应的领域数量
	multimap<DWORD,DWORD> m_mapNum_ID;
	std::set<DWORD> m_setTemp;
};
