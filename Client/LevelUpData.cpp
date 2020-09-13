#include "stdafx.h"
#include "LevelUpData.h"

LevelUpData::LevelUpData( void )
{

}

LevelUpData::~LevelUpData( void )
{

}

LevelUpData	g_LevelUpData;
LevelUpData* LevelUpData::Inst()
{	
	return &g_LevelUpData;
}

void LevelUpData::LoadFromFile()
{
	const TCHAR* szExpProto = _T("data\\system\\attdata\\role_att_level_up.xml");

	list<tstring> expList;
	list<tstring>::iterator iter;

	//读取经验
	TObjRef<VarContainer> varExp = CreateObj( "ExpProto", "VarContainer" );
	if(!varExp->Load("VFS_System", szExpProto, "id", &expList))
		IMSG(_T("Load file role_att_level_up.xml failed\r\n"));

	for(iter=expList.begin(); iter!=expList.end(); ++iter)
	{
		tagExperience exp;
		exp.nLevel				 = varExp->GetInt(_T("id"),			(*iter).c_str(), NULL);
		exp.nExp				 = varExp->GetInt64(_T("ExpLevelUp"),	(*iter).c_str(), NULL);
		exp.nAtt[ERA_Physique]	 = varExp->GetInt(_T("Physique"),	(*iter).c_str(), NULL);
		exp.nAtt[ERA_Strength]	 = varExp->GetInt(_T("Strength"),	(*iter).c_str(), NULL);
		exp.nAtt[ERA_Pneuma]	 = varExp->GetInt(_T("Pneuma"),		(*iter).c_str(), NULL);
		exp.nAtt[ERA_InnerForce] = varExp->GetInt(_T("InnerForce"),	(*iter).c_str(), NULL);
		exp.nAtt[ERA_Technique]	 = varExp->GetInt(_T("Technique"),	(*iter).c_str(), NULL);
		exp.nAtt[ERA_Agility]	 = varExp->GetInt(_T("Agility"),	(*iter).c_str(), NULL);
		exp.nCost[ESCT_HP]		 = varExp->GetInt(_T("HP"),			(*iter).c_str(), NULL);
		exp.nCost[ESCT_MP]		 = varExp->GetInt(_T("MP"),			(*iter).c_str(), NULL);
		exp.nCost[ESCT_Valicity] = varExp->GetInt(_T("Vitality"),	(*iter).c_str(), NULL);
        exp.nRoleAttAvail        = varExp->GetInt(_T("RoleAttAvail_e"),	(*iter).c_str(), NULL);
        exp.nTalentAvail         = varExp->GetInt(_T("TalentAvail_e"),	(*iter).c_str(), NULL);
		exp.nTalentAvail_limit	 = varExp->GetInt( _T("TalentAvail_limit"), (*iter).c_str(), GT_INVALID );

		m_mapExp.insert(make_pair(exp.nLevel, exp));
	}

	KillObj("ExpProto");
}

int LevelUpData::GetRoleAttInc( ERoleAttribute eType, DWORD dwLevel )
{
    const tagExperience* levelupData = FindExpData(dwLevel);
    if (!P_VALID(levelupData))
        return GT_INVALID;

    const tagExperience* levelupDataPre = FindExpData(dwLevel - 1);

    switch(eType)
    {
    case ERA_Physique:
        {
            // 筋骨
            return P_VALID(levelupDataPre) ? levelupData->nAtt[ERA_Physique] - levelupDataPre->nAtt[ERA_Physique] : levelupData->nAtt[ERA_Physique];
        }
        break;
    case ERA_Strength:
        {
            // 劲力
            return P_VALID(levelupDataPre) ? levelupData->nAtt[ERA_Strength] - levelupDataPre->nAtt[ERA_Strength] : levelupData->nAtt[ERA_Strength];
        }
        break;
    case ERA_Pneuma:
        {
            // 元气
            return P_VALID(levelupDataPre) ? levelupData->nAtt[ERA_Pneuma] - levelupDataPre->nAtt[ERA_Pneuma] : levelupData->nAtt[ERA_Pneuma];
        }
        break;
    case ERA_InnerForce:
        {
            // 内力
            return P_VALID(levelupDataPre) ? levelupData->nAtt[ERA_InnerForce] - levelupDataPre->nAtt[ERA_InnerForce] : levelupData->nAtt[ERA_InnerForce];
        }
        break;
    case ERA_Technique:
        {
            // 技力
            return P_VALID(levelupDataPre) ? levelupData->nAtt[ERA_Technique] - levelupDataPre->nAtt[ERA_Technique] : levelupData->nAtt[ERA_Technique];
        }
        break;
    case ERA_Agility:
        {
            // 身法
            return P_VALID(levelupDataPre) ? levelupData->nAtt[ERA_Agility] - levelupDataPre->nAtt[ERA_Agility] : levelupData->nAtt[ERA_Agility];
        }
        break;
    case ERA_AttPoint:
        {
            // 属性点
            return levelupData->nRoleAttAvail;
        }
        break;
    case ERA_TalentPoint:
        {
            // 天资点
            return levelupData->nTalentAvail;
        }
        break;
    case ERA_HP:
        {
            // HP
            return P_VALID(levelupDataPre) ? levelupData->nCost[ESCT_HP] - levelupDataPre->nCost[ESCT_HP] : levelupData->nCost[ESCT_HP];
        }
        break;
    case ERA_MP:
        {
            // MP
            return P_VALID(levelupDataPre) ? levelupData->nCost[ESCT_MP] - levelupDataPre->nCost[ESCT_MP] : levelupData->nCost[ESCT_MP];
        }
    case ERA_Vitality:
        {
            // 活力
            return P_VALID(levelupDataPre) ? levelupData->nCost[ESCT_Valicity] - levelupDataPre->nCost[ESCT_Valicity] : levelupData->nCost[ESCT_Valicity];
        }
    }

    return GT_INVALID;
}