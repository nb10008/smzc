#pragma once

#include "BaseData.h"

struct tagNPCSay
{
	DWORD dwNPCID;		
	INT nRate;
	tstring strSay;
};

class NPCSayTable : public BaseData
{
public:
	NPCSayTable(void);
	~NPCSayTable(void);

	static NPCSayTable* Inst();
	virtual void LoadFromFile();
	tagNPCSay* FindNPCSayTable(DWORD dwID)
	{
		return FindInMap(m_mapSayProto, dwID);
	}
private:
	map<DWORD, tagNPCSay> m_mapSayProto;
};