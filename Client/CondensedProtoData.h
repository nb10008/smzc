#pragma once
#include "basedata.h"
#include "..\WorldDefine\clergy_define.h"

/** breaf:Äý¾ÛProto
*/

class CondensedProtoData :
	public BaseData
{
public:
	CondensedProtoData(void);
	~CondensedProtoData(void);

	static CondensedProtoData* Inst();

	virtual VOID LoadFromFile();

	const tagCondensedProto* FindCondensedProto(DWORD dwID){return FindInMap(m_mapCondensedProto, dwID);}

private:

	map<DWORD, tagCondensedProto>	m_mapCondensedProto;
};
