#pragma once
#include "EffectSetData.h"
class NPCSkillEffectData
{
    map<ULONGLONG, tagEffectSet> m_data;
    map<ULONGLONG, tagEffectSet> m_singdata;
public:
    static NPCSkillEffectData* Inst();
    const tagEffectSet& GetNPCEffectSet(tstring npc, DWORD typeId, DWORD skillId, bool sing);
};