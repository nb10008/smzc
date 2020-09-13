#pragma once
#include "..\WorldDefine\pulse_define.h"
#include "basedata.h"
class PulseData : public BaseData
{
public:
	PulseData(void);
	~PulseData(void);

	static PulseData* Inst();
	VOID LoadFromFile();
	// ID为索引
	tagPulseClientProto* FindPulse(DWORD dwID) {return FindInMap(m_mapPulse, dwID);}

	// pos为索引
	tagPulseClientProto* FindFirePulseByPos(DWORD dwPos) {return FindInMap(m_mapFire, dwPos);}
	tagPulseClientProto* FindWaterPulseByPos(DWORD dwPos) {return FindInMap(m_mapWater, dwPos);}
	tagPulseClientProto* FindThunderPulseByPos(DWORD dwPos) {return FindInMap(m_mapThunder, dwPos);}
	tagPulseClientProto* FindWindPulseByPos(DWORD dwPos) {return FindInMap(m_mapWind, dwPos);}
	tagPulseClientProto* FindElementPulseByPos(DWORD dwPos){return FindInMap(m_mapElement, dwPos);}

	// SkillID为索引 7位的
	tagPulseClientProto* FindPulseBySkillID(DWORD dwID) {return FindInMap(m_mapPulseSkillIDIndex, dwID);}

private:
	map<DWORD, tagPulseClientProto> m_mapPulse;	// ID为索引
	map<DWORD, tagPulseClientProto> m_mapPulseSkillIDIndex;	// SkillID为索引

	map<DWORD, tagPulseClientProto> m_mapFire;		// pos为索引
	map<DWORD, tagPulseClientProto> m_mapWater;	// pos为索引
	map<DWORD, tagPulseClientProto> m_mapThunder;		// pos为索引
	map<DWORD, tagPulseClientProto> m_mapWind;	// pos为索引
	map<DWORD, tagPulseClientProto> m_mapElement;	// pos为索引
	
};
