#pragma once

#include "..\WorldDefine\compose_define.h"

class SoarCastingData
{
public:
	typedef std::map<DWORD,tagSoarCastingProto> SoarCastingDataMap;
	SoarCastingData(void);
	~SoarCastingData(void);

	bool Init();
	void Destroy();
	BOOL LoadFromFile();
	const tagSoarCastingProto* GetSoarCastingProtoData(DWORD dwID) const;
	const SoarCastingDataMap& GetSoarCastingDataMap() const;
private:
	SoarCastingDataMap	m_mapCasting;
};
