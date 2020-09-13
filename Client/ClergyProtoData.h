#pragma once
#include "basedata.h"
#include "..\WorldDefine\clergy_define.h"

class ClergyProtoData :
	public BaseData
{
public:
	ClergyProtoData(void);
	~ClergyProtoData(void);

	static ClergyProtoData* Inst();

	virtual VOID LoadFromFile();

	const tagClergyName* FindClergyProto(DWORD dwID){return FindInMap(m_mapClergyName, dwID);}
	DWORD GetNum(){return m_mapClergyName.size();}

private:

	map<DWORD, tagClergyName>	m_mapClergyName;
};
