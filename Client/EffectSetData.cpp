#include "stdafx.h"
#include "EffectSetData.h"

EffectSetData g_effectdata;
EffectSetData* EffectSetData::Inst()
{
    return &g_effectdata;
}

void EffectSetData::LoadFromFile()
{
    tstring filepath = g_strLocalPath + _T("\\attdata\\effect_set.xml");
	if (P_VALID(GetObj("EffectSetData")))
		KillObj("EffectSetData");
    TObjRef<VarContainer> VarEffectSetData = CreateObj("EffectSetData", "VarContainer");
    list<tstring> effect_ids;
    if(!VarEffectSetData->Load("VFS_System", filepath.c_str(), "effect_id", &effect_ids))
        IMSG(_T("Load file effect_set.xml failed\r\n"));
    for(list<tstring>::iterator it = effect_ids.begin(); it != effect_ids.end(); ++it)
    {
        int i = 0;
        tstring modelname;
        tagEffectSet effectset;
        do
        {
            TCHAR postfix[5];
            _itot_s(i, postfix, 10);
            modelname = VarEffectSetData->GetString(_T("effect_model"), postfix, it->c_str(), _T(""));
            if (modelname != _T(""))
            {
                tstring nodename = VarEffectSetData->GetString(_T("tag_effect_model"), postfix, it->c_str(), _T(""));
                if(nodename==_T("-1"))
                    nodename = _T("");
                effectset.model_node.push_back(make_pair(modelname, nodename));
            }
            ++i;
        }
        while(modelname != _T(""));
        if (effectset.model_node.size()>0)
        {
            m_mapEffectSet.insert(make_pair(*it, effectset));
        }
    }
	KillObj("EffectSetData");
}

tagEffectSet* EffectSetData::FindEffectSet( tstring id )
{
    return FindInMap(m_mapEffectSet, id);
}