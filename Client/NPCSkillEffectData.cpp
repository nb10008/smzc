#include "stdafx.h"
#include "NPCSkillEffectData.h"
#include "..\Cool3D\Util\CSVFile.h"

NPCSkillEffectData g_NPCSkillEffect;
NPCSkillEffectData* NPCSkillEffectData::Inst()
{
    return &g_NPCSkillEffect;
}

const tagEffectSet& NPCSkillEffectData::GetNPCEffectSet(tstring npc, DWORD typeId, DWORD skillId, bool sing)
{
    ULONGLONG id = ((ULONGLONG)typeId<<32)+(ULONGLONG)skillId;
    if(sing)
    {
        map<ULONGLONG, tagEffectSet>::iterator iter = m_singdata.find(id);
        if(iter==m_singdata.end())
        {
            tagEffectSet sfxset;
            tstringstream stream;
            stream << _T("data\\animation\\npc\\") << npc <<
                _T("\\") << typeId << "_" << skillId<<_T("_sing")<<_T(".csv");
            tstring filename = stream.str();
            IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
            DWORD dwFileSize=pIFS->GetSize(filename.c_str());
            if(dwFileSize>0 && dwFileSize!=GT_INVALID)
            {
                CSVFile csvFile;
                if(csvFile.OpenFile(pIFS,filename.c_str()))
                {
                    //只有一行
                    csvFile.BeginReadLine();
                    const vector<tstring>& line = csvFile.GetNextLine();
                    int numToken=line.size();
                    for(int i=0;i<numToken;i+=2)
                    {
                        if(i+1<numToken)
                        {
                            sfxset.model_node.push_back(make_pair(line[i], line[i+1]));
                        }
                    }
                    csvFile.CloseFile();
                }
            }
            m_singdata[id] = sfxset;
            return m_singdata[id];
        }
        else
        {
            return iter->second;
        }
    }
    else
    {
        map<ULONGLONG, tagEffectSet>::iterator iter = m_data.find(id);
        if(iter==m_data.end())
        {
            tagEffectSet sfxset;
            tstringstream stream;
            stream << _T("data\\animation\\npc\\") << npc <<
                _T("\\") << typeId << "_" << skillId<<_T(".csv");
            tstring filename = stream.str();
            IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
            DWORD dwFileSize=pIFS->GetSize(filename.c_str());
            if(dwFileSize>0 && dwFileSize!=GT_INVALID)
            {
                CSVFile csvFile;
                if(csvFile.OpenFile(pIFS,filename.c_str()))
                {
                    //只有一行
                    csvFile.BeginReadLine();
                    const vector<tstring>& line = csvFile.GetNextLine();
                    int numToken=line.size();
                    for(int i=0;i<numToken;i+=2)
                    {
                        if(i+1<numToken)
                        {
                            sfxset.model_node.push_back(make_pair(line[i], line[i+1]));
                        }
                    }
                    csvFile.CloseFile();
                }
            }
            m_data[id] = sfxset;
            return m_data[id];
        }
        else
        {
            return iter->second;
        }
    }
}