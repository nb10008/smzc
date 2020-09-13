#pragma once
#include "BaseData.h"
struct tagEffectSet
{
    vector<pair<tstring, tstring>>model_node;
};
class EffectSetData: public BaseData
{
public:
    static EffectSetData* Inst();
    virtual void LoadFromFile();
    tagEffectSet* FindEffectSet(tstring id);
private:
    map<tstring, tagEffectSet> m_mapEffectSet;
};