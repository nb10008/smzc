#include "stdafx.h"
#include "SkillProtoData.h"


bool string_split( tstring is, const TCHAR c, vector<tstring>& os )
{
	os.clear();

	tstring::size_type pos_find = is.find_first_of( c, 0 );
	tstring::size_type pos_last_find = -1;
	while( tstring::npos != pos_find )
	{
		tstring::size_type num_char = pos_find - pos_last_find - 1;
		if( num_char > 0 ) os.push_back( is.substr( pos_last_find + 1, num_char ) );
		pos_last_find = pos_find;
		pos_find = is.find_first_of( c, pos_last_find + 1 );
	}
	tstring::size_type num_char = is.length() - pos_last_find;
	if( num_char > is.length() ) num_char = is.length();
	tstring sub = is.substr( pos_last_find + 1, num_char );
	if( !sub.empty() ) os.push_back( sub );
	return true;
}

SkillProtoData::SkillProtoData()
{
}

SkillProtoData::~SkillProtoData()
{
}

SkillProtoData g_skillproto;
SkillProtoData* SkillProtoData::Inst()
{
	return &g_skillproto;
}

void SkillProtoData::LoadFromFile()
{
	LPCTSTR szSkillProto = _T("data\\system\\attdata\\skill_proto.xml");
	tstring strPath1 = g_strLocalPath + _T("\\attdata\\skill_name.xml");
	LPCTSTR szSkillDisplay = strPath1.c_str();
	LPCTSTR szBuffProto = _T("data\\system\\attdata\\buff_proto.xml");
	tstring strPath2 = g_strLocalPath + _T("\\attdata\\buff_name.xml");
	LPCTSTR szBuffDisplay = strPath2.c_str();
	LPCTSTR	szTriggerProto = _T("data\\system\\attdata\\trigger_proto.xml");
	LPCTSTR szConSkillProto = _T("data\\system\\attdata\\conti_skill.xml");
	LPCTSTR szSkillBoxProto = _T("data\\system\\attdata\\skill_box.xml");
	LPCTSTR szSkillMapProto = _T("data\\system\\attdata\\mapbox_proto.xml");

	list<tstring> SkillProtoFieldList;
	list<tstring> SkillDisplayFieldList;
	list<tstring> BuffProtoFieldList;
	list<tstring> BuffDisplayFieldList;
	list<tstring> TriggerProtoFieldList;
	list<tstring> ConSkillProtoFiledList;
	list<tstring> skillBoxFiledList;
	list<tstring> skillMapFiledList;
	list<tstring>::iterator iter;
	
	//---------------------------------------------------------------------------------
	//读取skill_proto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarSkillProto = CreateObj("SkillProto", "VarContainer");
	if(!VarSkillProto->Load("VFS_System", szSkillProto, "id", &SkillProtoFieldList))
		IMSG(_T("Load file skill_proto.xml failed\r\n"));

	for(iter = SkillProtoFieldList.begin(); iter != SkillProtoFieldList.end(); ++iter)
	{
		tagSkillProtoClient skill;
		skill.dwID				=						VarSkillProto->GetDword(_T("id"),				(*iter).c_str(),	GT_INVALID);
		skill.eType				= (ESkillType)			VarSkillProto->GetDword(_T("type"),				(*iter).c_str(),	ESST_Null);
		skill.nType2			= (INT)					VarSkillProto->GetDword(_T("type2"),			(*iter).c_str(),	0);
		skill.nType3			= (INT)					VarSkillProto->GetDword(_T("type3"),			(*iter).c_str(),	0);
        skill.eOPType			= (ESkillOPType)		VarSkillProto->GetDword(_T("op_type"),	        (*iter).c_str(),	0);
		skill.fOPDist			= (FLOAT)				VarSkillProto->GetDword(_T("op_distance"),		(*iter).c_str(),	0);
		skill.fOPRadius			= (FLOAT)				VarSkillProto->GetDword(_T("op_raduis"),		(*iter).c_str(),	0);
		skill.nPrepareTime		= (INT)					VarSkillProto->GetDword(_T("prepare_time"),		(*iter).c_str(),	0);
		skill.nCoolDown			= (INT)					VarSkillProto->GetDword(_T("cooldown"),			(*iter).c_str(),	0);	
		skill.nMaxLearnLevel	= (INT)					VarSkillProto->GetDword(_T("max_learn_level"),	(*iter).c_str(),	0);
		skill.eTalentType		= (ETalentType)			VarSkillProto->GetDword(_T("talent_type"),		(*iter).c_str(),	ETT_Null);
		skill.eUseType			= (ESkillUseType)		VarSkillProto->GetDword(_T("use_type"),			(*iter).c_str(),	ESUT_Null);
		skill.eDmgType			= (ESkillDmgType)		VarSkillProto->GetDword(_T("dmg_type"),			(*iter).c_str(),	ESDGT_Null);
		skill.eLevelUpType		= (ESkillLevelUpType)	VarSkillProto->GetDword(_T("levelup_type"),		(*iter).c_str(),	ESLUT_Fixed);
		skill.nLevelUpExp		= (INT)					VarSkillProto->GetDword(_T("exp"),				(*iter).c_str(),	0);	
		skill.nNeedRoleLevel	= (INT)					VarSkillProto->GetDword(_T("need_role_level"),	(*iter).c_str(),	0);
		skill.nNeedTalentPoint	= (INT)					VarSkillProto->GetDword(_T("need_talent_point"),(*iter).c_str(),	0);
		skill.dwPreLevelSkillID	=						VarSkillProto->GetDword(_T("pre_skill_id"),		(*iter).c_str(),	GT_INVALID);
		//skill.eNeedClassType	= (EClassType)			VarSkillProto->GetDword(_T("career_type"),		(*iter).c_str(),	EV_Base);
		skill.dwTargetLimit		=						VarSkillProto->GetDword(_T("target_limit"),		(*iter).c_str(),	0);
		skill.bFriendly			= (BOOL)				VarSkillProto->GetDword(_T("friendly"),			(*iter).c_str(),	TRUE);
		skill.bHostile			= (BOOL)				VarSkillProto->GetDword(_T("hostile"),			(*iter).c_str(),	TRUE);
		skill.bIndependent		= (BOOL)				VarSkillProto->GetDword(_T("independent"),		(*iter).c_str(),	TRUE);
		skill.nLevel			=						VarSkillProto->GetInt(  _T("level"),			(*iter).c_str(),	0);
		skill.nMaxLevel			=						VarSkillProto->GetInt(  _T("max_level"),		(*iter).c_str(),	0);
		skill.nMaxLearnLevel	=						VarSkillProto->GetInt(  _T("max_learn_level"),	(*iter).c_str(),	0);
		skill.nUseHPPctLimit	=						VarSkillProto->GetInt(  _T("hp_limit"),			(*iter).c_str(),	0);
		skill.nUseMPPctLimit	=						VarSkillProto->GetInt(  _T("mp_limit"),			(*iter).c_str(),	0);
		skill.eSexLimit			= (ESkillSexLimit)		VarSkillProto->GetInt(  _T("sex_limit"),		(*iter).c_str(),	ESSL_Null);
		skill.eClassLimit		= (EClassType)			VarSkillProto->GetInt(  _T("career_limit"),		(*iter).c_str(),	EV_Null);
		skill.nWeaponLimit		=						VarSkillProto->GetInt(  _T("weapon_limit"),		(*iter).c_str(),	0);
		skill.ePosType			= (ESkillPosType)		VarSkillProto->GetInt(  _T("pos_limit"),		(*iter).c_str(),	ESPT_NUll);
		//skill.bMountLimit		= (BOOL)				VarSkillProto->GetDword(_T("mount_limit"),		(*iter).c_str(),	false);
		skill.dwTargetLimit		=						VarSkillProto->GetDword(_T("target_limit"),		(*iter).c_str(),	GT_INVALID);
		skill.bInterCombat		= (BOOL)				VarSkillProto->GetDword(_T("inter_combat"),		(*iter).c_str(),	false);
		skill.dwBuffLimitID		=						VarSkillProto->GetDword(_T("buff_limit_id"),	(*iter).c_str(),	GT_INVALID);
		skill.dwTargetBuffLimitID	=					VarSkillProto->GetDword(_T("target_buff_limit_id"),	(*iter).c_str(),	GT_INVALID);
		skill.nChannelDmg[0]	= (INT)					VarSkillProto->GetDword(_T("channel_dmg1"),		(*iter).c_str(),	0);	
		skill.nChannelDmg[1]	= (INT)					VarSkillProto->GetDword(_T("channel_dmg2"),		(*iter).c_str(),	0);	
		skill.nChannelDmg[2]	= (INT)					VarSkillProto->GetDword(_T("channel_dmg3"),		(*iter).c_str(),	0);	
		skill.nChannelDmg[3]	= (INT)					VarSkillProto->GetDword(_T("channel_dmg4"),		(*iter).c_str(),	0);	
		skill.nChannelDmg[4]	= (INT)					VarSkillProto->GetDword(_T("channel_dmg5"),		(*iter).c_str(),	0);	
		skill.dwSelfStateLimit	=						VarSkillProto->GetDword(  _T("self_state_limit"),(*iter).c_str(),	0);
		skill.dwTargetStateLimit=						VarSkillProto->GetDword(  _T("target_state_limit"),(*iter).c_str(),	0);
		skill.bMoveable			= (BOOL)				VarSkillProto->GetDword(_T("moveable"),			(*iter).c_str(),	FALSE);
        skill.dwBuffID[0]       =                       VarSkillProto->GetDword(_T("buff1_id"),			(*iter).c_str(),	0);
		// Jason 2010-3-17
		skill.bInterruptMove	= (BOOL)				VarSkillProto->GetDword(_T("interrupt_move"),	(*iter).c_str(),	FALSE);
		skill.iMaxAttackNum		= (INT)					VarSkillProto->GetDword(_T("atk_num"),			(*iter).c_str(),	0);	
		skill.eSpecFunc		= (ESkillSpecFunc)					VarSkillProto->GetDword(_T("spec_func"),			(*iter).c_str(),	0);	
		skill.nSpecFuncVal1		= (INT)					VarSkillProto->GetDword(_T("SpecFuncVal1"),			(*iter).c_str(),	0);
		skill.nSpecFuncVal2		= (INT)					VarSkillProto->GetDword(_T("SpecFuncVal2"),			(*iter).c_str(),	0);
		skill.nDmgTimes = 0;
		for(int i=0; i<MAX_CHANNEL_TIMES; i++)
		{
			if(skill.nChannelDmg[i]>0)
				skill.nDmgTimes++;
		}

		skill.dwCostItemID					=			VarSkillProto->GetDword(_T("cost_item_id"),		(*iter).c_str(), GT_INVALID);
		skill.nCostItemNum					=	(INT)	VarSkillProto->GetDword(_T("cost_item_num"),	(*iter).c_str(), 0);
		skill.nSkillCost[ESCT_HP]			=	(INT)	VarSkillProto->GetDword(_T("cost_hp"),			(*iter).c_str(), 0);
		skill.nSkillCost[ESCT_MP]			=	(INT)	VarSkillProto->GetDword(_T("cost_mp"),			(*iter).c_str(), 0);
		skill.nSkillCost[ESCT_Rage]			=	(INT)	VarSkillProto->GetDword(_T("cost_rage"),		(*iter).c_str(), 0);
		skill.nSkillCost[ESCT_Endurance]	=	(INT)	VarSkillProto->GetDword(_T("cost_durance"),		(*iter).c_str(), 0);
		skill.nSkillCost[ESCT_Valicity]		=	(INT)	VarSkillProto->GetDword(_T("cost_valicity"),	(*iter).c_str(), 0);
		skill.dwCutChannel					=			VarSkillProto->GetDword(_T("interrupt_limit"),	(*iter).c_str(), 6);
		skill.mapRoleAttMod.Clear();
		skill.mapRoleAttModPct.Clear();

		// 人物属性加成
		INT nERAPhysique				=	VarSkillProto->GetInt(_T("ERA_Physique"),				(*iter).c_str(), 0);
		INT nERAStrength				=	VarSkillProto->GetInt(_T("ERA_Strength"),				(*iter).c_str(), 0);
		INT nERAPneuma					=	VarSkillProto->GetInt(_T("ERA_Pneuma"),					(*iter).c_str(), 0);
		INT nERAInnerForce				=	VarSkillProto->GetInt(_T("ERA_InnerForce"),				(*iter).c_str(), 0);
		INT nERATechnique				=	VarSkillProto->GetInt(_T("ERA_Technique"),				(*iter).c_str(), 0);
		INT nERAAgility					=	VarSkillProto->GetInt(_T("ERA_Agility"),				(*iter).c_str(), 0);
		INT nERAMaxHP					=	VarSkillProto->GetInt(_T("ERA_MaxHP"),					(*iter).c_str(), 0);
		INT nERAMaxMP					=	VarSkillProto->GetInt(_T("ERA_MaxMP"),					(*iter).c_str(), 0);
		INT nERAExAttack				=	VarSkillProto->GetInt(_T("ERA_ExAttack"),				(*iter).c_str(), 0);
		INT nERAExDefense				=	VarSkillProto->GetInt(_T("ERA_ExDefense"),				(*iter).c_str(), 0);
		INT nERAInAttack				=	VarSkillProto->GetInt(_T("ERA_InAttack"),				(*iter).c_str(), 0);
		INT nERAInDefense				=	VarSkillProto->GetInt(_T("ERA_InDefense"),				(*iter).c_str(), 0);
		INT nERAAttackTec				=	VarSkillProto->GetInt(_T("ERA_AttackTec"),				(*iter).c_str(), 0);
		INT nERADefenseTec				=	VarSkillProto->GetInt(_T("ERA_DefenseTec"),				(*iter).c_str(), 0);
		INT nERAHitRate					=	VarSkillProto->GetInt(_T("ERA_HitRate"),				(*iter).c_str(), 0);
		INT nERADodge					=	VarSkillProto->GetInt(_T("ERA_Dodge"),					(*iter).c_str(), 0);
		INT nERAMaxEndurance			=	VarSkillProto->GetInt(_T("ERA_MaxEndurance"),			(*iter).c_str(), 0);
		INT nERAHPRegainRate			=	VarSkillProto->GetInt(_T("ERA_HPRegainRate"),			(*iter).c_str(), 0);
		INT nERAMPRegainRate			=	VarSkillProto->GetInt(_T("ERA_MPRegainRate"),			(*iter).c_str(), 0);
		INT nERAMaxVitality				=	VarSkillProto->GetInt(_T("ERA_MaxVitality"),			(*iter).c_str(), 0);
		INT nERAVitalityRegainRate		=	VarSkillProto->GetInt(_T("ERA_VitalityRegainRate"),		(*iter).c_str(), 0);
		INT nERASpeed_XZ				=	VarSkillProto->GetInt(_T("ERA_Speed_XZ"),				(*iter).c_str(), 0);
		INT nERASpeed_Y					=	VarSkillProto->GetInt(_T("ERA_Speed_Y"),				(*iter).c_str(), 0);
		INT nERASpeedSwim				=	VarSkillProto->GetInt(_T("ERA_Speed_Swin"),				(*iter).c_str(), 0);
		INT nERASpeedMount				=	VarSkillProto->GetInt(_T("ERA_Speed_Mount"),			(*iter).c_str(), 0);
		INT nERAWeaponDmgMin			=	VarSkillProto->GetInt(_T("ERA_WeaponDmgMin"),			(*iter).c_str(), 0);
		INT nERAWeaponDmgMax			=	VarSkillProto->GetInt(_T("ERA_WeaponDmgMax"),			(*iter).c_str(), 0);
		INT nERAWeaponSoul				=	VarSkillProto->GetInt(_T("ERA_WeaponSoul"),				(*iter).c_str(), 0);
		INT nERAArmor					=	VarSkillProto->GetInt(_T("ERA_Armor"),					(*iter).c_str(), 0);
		INT nERADerate_Ordinary			=	VarSkillProto->GetInt(_T("ERA_Derate_Ordinary"),		(*iter).c_str(), 0);
		INT nERADerate_Bleeding			=	VarSkillProto->GetInt(_T("ERA_Derate_Bleeding"),		(*iter).c_str(), 0);
		INT nERADerate_Brunt			=	VarSkillProto->GetInt(_T("ERA_Derate_Brunt"),			(*iter).c_str(), 0);
		INT nERADerate_Bang				=	VarSkillProto->GetInt(_T("ERA_Derate_Bang"),			(*iter).c_str(), 0);
		INT nERADerate_Poison			=	VarSkillProto->GetInt(_T("ERA_Derate_Poison"),			(*iter).c_str(), 0);
		INT nERADerate_Thinker			=	VarSkillProto->GetInt(_T("ERA_Derate_Thinker"),			(*iter).c_str(), 0);
		INT nERADerate_Injury			=	VarSkillProto->GetInt(_T("ERA_Derate_Injury"),			(*iter).c_str(), 0);
		INT nERADerate_Stunt			=	VarSkillProto->GetInt(_T("ERA_Derate_Stunt"),			(*iter).c_str(), 0);
		INT nERADerate_ExAttack			=	VarSkillProto->GetInt(_T("ERA_Derate_ExAttack"),		(*iter).c_str(), 0);
		INT nERADerate_InAttack			=	VarSkillProto->GetInt(_T("ERA_Derate_InAttack"),		(*iter).c_str(), 0);
		INT nERADerate_All				=	VarSkillProto->GetInt(_T("ERA_Derate_All"),				(*iter).c_str(), 0);
		INT nERAExDamage				=	VarSkillProto->GetInt(_T("ERA_ExDamage"),				(*iter).c_str(), 0);
		INT nERAExDamage_Absorb			=	VarSkillProto->GetInt(_T("ERA_ExDamage_Absorb"),		(*iter).c_str(), 0);
		INT nERAResist_Bleeding			=	VarSkillProto->GetInt(_T("ERA_Resist_Bleeding"),		(*iter).c_str(), 0);
		INT nERAResist_Weak				=	VarSkillProto->GetInt(_T("ERA_Resist_Weak"),			(*iter).c_str(), 0);
		INT nERAResist_Choas			=	VarSkillProto->GetInt(_T("ERA_Resist_Choas"),			(*iter).c_str(), 0);
		INT nERAResist_Special			=	VarSkillProto->GetInt(_T("ERA_Resist_Special"),			(*iter).c_str(), 0);
		INT nERARegain_Addtion			=	VarSkillProto->GetInt(_T("ERA_Regain_Addtion"),			(*iter).c_str(), 0);
		INT nERAAttack_MissRate			=	VarSkillProto->GetInt(_T("ERA_Attack_MissRate"),		(*iter).c_str(), 0);
		INT nERACloseAttack_DodgeRate	=	VarSkillProto->GetInt(_T("ERA_CloseAttack_DodgeRate"),	(*iter).c_str(), 0);
		INT nERARemoteAttack_DodgeRate	=	VarSkillProto->GetInt(_T("ERA_RemoteAttack_DodgeRate"), (*iter).c_str(), 0);
		INT nERACrit_Rate				=	VarSkillProto->GetInt(_T("ERA_Crit_Rate"),				(*iter).c_str(), 0);
		INT nERACrit_Amount				=	VarSkillProto->GetInt(_T("ERA_Crit_Amount"),			(*iter).c_str(), 0);
		INT nERABlock_Rate				=	VarSkillProto->GetInt(_T("ERA_Block_Rate"),				(*iter).c_str(), 0);
		INT nERAInspiration				=	VarSkillProto->GetInt(_T("ERA_Inspiration"),			(*iter).c_str(), 0);
		INT nERALurk					=	VarSkillProto->GetInt(_T("ERA_Lurk"),					(*iter).c_str(), 0);
		INT nERASavvy					=	VarSkillProto->GetInt(_T("ERA_Savvy"),					(*iter).c_str(), 0);
		INT nERAEnmity_Degree			=	VarSkillProto->GetInt(_T("ERA_Enmity_Degree"),			(*iter).c_str(), 0);
		INT nERAShape					=	VarSkillProto->GetInt(_T("ERA_Shape"),					(*iter).c_str(), 0);
		INT nERAExp_Add_Rate			=	VarSkillProto->GetInt(_T("ERA_Exp_Add_Rate"),			(*iter).c_str(), 0);
		INT nERAMoney_Add_Rate			=	VarSkillProto->GetInt(_T("ERA_Money_Add_Rate"),			(*iter).c_str(), 0);
		INT nERALoot_Add_Rate			=	VarSkillProto->GetInt(_T("ERA_Loot_Add_Rate"),			(*iter).c_str(), 0);

		INT nERA_transform_Bang			=	VarSkillProto->GetInt(_T("ERA_transform_Bang"),			(*iter).c_str(), 0);
		INT nERA_transform_Brunt		=	VarSkillProto->GetInt(_T("ERA_transform_Brunt"),		(*iter).c_str(), 0);
		INT nERA_transform_Injury		=	VarSkillProto->GetInt(_T("ERA_transform_Injury"),		(*iter).c_str(), 0);
		INT nERA_transform_Thinker		=	VarSkillProto->GetInt(_T("ERA_transform_Thinker"),		(*iter).c_str(), 0);

		AddSkillRoleAtt(skill, ERA_Physique,				nERAPhysique);
		AddSkillRoleAtt(skill, ERA_Strength,				nERAStrength);
		AddSkillRoleAtt(skill, ERA_Pneuma,					nERAPneuma);
		AddSkillRoleAtt(skill, ERA_InnerForce,				nERAInnerForce);
		AddSkillRoleAtt(skill, ERA_Technique,				nERATechnique);
		AddSkillRoleAtt(skill, ERA_Agility,					nERAAgility);
		AddSkillRoleAtt(skill, ERA_MaxHP,					nERAMaxHP);
		AddSkillRoleAtt(skill, ERA_MaxMP,					nERAMaxMP);
		AddSkillRoleAtt(skill, ERA_ExAttack,				nERAExAttack);
		AddSkillRoleAtt(skill, ERA_ExDefense,				nERAExDefense);
		AddSkillRoleAtt(skill, ERA_InAttack,				nERAInAttack);
		AddSkillRoleAtt(skill, ERA_InDefense,				nERAInDefense);
		AddSkillRoleAtt(skill, ERA_AttackTec,				nERAAttackTec);
		AddSkillRoleAtt(skill, ERA_DefenseTec,				nERADefenseTec);
		AddSkillRoleAtt(skill, ERA_HitRate,					nERAHitRate);
		AddSkillRoleAtt(skill, ERA_Dodge,					nERADodge);
		AddSkillRoleAtt(skill, ERA_MaxEndurance,			nERAMaxEndurance);
		AddSkillRoleAtt(skill, ERA_HPRegainRate,			nERAHPRegainRate);
		AddSkillRoleAtt(skill, ERA_MPRegainRate,			nERAMPRegainRate);
		AddSkillRoleAtt(skill, ERA_MaxVitality,				nERAMaxVitality);
		AddSkillRoleAtt(skill, ERA_VitalityRegainRate,		nERAVitalityRegainRate);
		AddSkillRoleAtt(skill, ERA_Speed_XZ,				nERASpeed_XZ);
		AddSkillRoleAtt(skill, ERA_Speed_Y,					nERASpeed_Y);
		AddSkillRoleAtt(skill, ERA_Speed_Swim,				nERASpeedSwim);
		AddSkillRoleAtt(skill, ERA_Speed_Mount,				nERASpeedMount);
		AddSkillRoleAtt(skill, ERA_WeaponDmgMin,			nERAWeaponDmgMin);
		AddSkillRoleAtt(skill, ERA_WeaponDmgMax,			nERAWeaponDmgMax);
		AddSkillRoleAtt(skill, ERA_WeaponSoul,				nERAWeaponSoul);
		AddSkillRoleAtt(skill, ERA_Armor,					nERAArmor);
		AddSkillRoleAtt(skill, ERA_Derate_Ordinary,			nERADerate_Ordinary);
		AddSkillRoleAtt(skill, ERA_Derate_Bleeding,			nERADerate_Bleeding);
		AddSkillRoleAtt(skill, ERA_Derate_Brunt,			nERADerate_Brunt);
		AddSkillRoleAtt(skill, ERA_Derate_Bang,				nERADerate_Bang);
		AddSkillRoleAtt(skill, ERA_Derate_Poison,			nERADerate_Poison);
		AddSkillRoleAtt(skill, ERA_Derate_Thinker,			nERADerate_Thinker);
		AddSkillRoleAtt(skill, ERA_Derate_Injury,			nERADerate_Injury);
		AddSkillRoleAtt(skill, ERA_Derate_Stunt,			nERADerate_Stunt);
		AddSkillRoleAtt(skill, ERA_Derate_ExAttack,			nERADerate_ExAttack);
		AddSkillRoleAtt(skill, ERA_Derate_InAttack,			nERADerate_InAttack);
		AddSkillRoleAtt(skill, ERA_Derate_ALL,				nERADerate_All);
		AddSkillRoleAtt(skill, ERA_ExDamage,				nERAExDamage);
		AddSkillRoleAtt(skill, ERA_ExDamage_Absorb,			nERAExDamage_Absorb);	
		AddSkillRoleAtt(skill, ERA_Resist_Bleeding,			nERAResist_Bleeding);	
		AddSkillRoleAtt(skill, ERA_Resist_Weak,				nERAResist_Weak);	
		AddSkillRoleAtt(skill, ERA_Resist_Choas,			nERAResist_Choas);	
		AddSkillRoleAtt(skill, ERA_Resist_Special,			nERAResist_Special);	
		AddSkillRoleAtt(skill, ERA_Regain_Addtion,			nERARegain_Addtion);	
		AddSkillRoleAtt(skill, ERA_Attack_MissRate,			nERAAttack_MissRate);	
		AddSkillRoleAtt(skill, ERA_CloseAttack_DodgeRate,	nERACloseAttack_DodgeRate);	
		AddSkillRoleAtt(skill, ERA_RemoteAttack_DodgeRate,	nERARemoteAttack_DodgeRate);	
		AddSkillRoleAtt(skill, ERA_Crit_Rate,				nERACrit_Rate);	
		AddSkillRoleAtt(skill, ERA_Crit_Amount,				nERACrit_Amount);	
		AddSkillRoleAtt(skill, ERA_Block_Rate,				nERABlock_Rate);	
		AddSkillRoleAtt(skill, ERA_Inspiration,				nERAInspiration);	
		AddSkillRoleAtt(skill, ERA_Lurk,					nERALurk);	
		AddSkillRoleAtt(skill, ERA_Savvy,					nERASavvy);	
		AddSkillRoleAtt(skill, ERA_Enmity_Degree,			nERAEnmity_Degree);
		AddSkillRoleAtt(skill, ERA_Shape,					nERAShape);
		AddSkillRoleAtt(skill, ERA_Exp_Add_Rate,			nERAExp_Add_Rate);
		AddSkillRoleAtt(skill, ERA_Money_Add_Rate,			nERAMoney_Add_Rate);
		AddSkillRoleAtt(skill, ERA_Loot_Add_Rate,			nERALoot_Add_Rate);
		AddSkillRoleAtt(skill, ERA_transform_Bang,			nERA_transform_Bang);
		AddSkillRoleAtt(skill, ERA_transform_Brunt,			nERA_transform_Brunt);
		AddSkillRoleAtt(skill, ERA_transform_Injury,		nERA_transform_Injury);
		AddSkillRoleAtt(skill, ERA_transform_Thinker,		nERA_transform_Thinker);

		//more
		m_mapSkillProto.insert(make_pair(skill.dwID, skill));
	}

	KillObj("SkillProto");


	//---------------------------------------------------------------------------------
	//读取skill_name
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarSkillDisplay = CreateObj("SkillDisplay", "VarContainer");
	if(!VarSkillDisplay->Load("VFS_System", szSkillDisplay, "id", &SkillDisplayFieldList))
		IMSG(_T("Load file skill_name.xml failed\r\n"));

	for(iter = SkillDisplayFieldList.begin(); iter != SkillDisplayFieldList.end(); ++iter)
	{
		DWORD dwID = VarSkillDisplay->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tagSkillProtoClient* pSkill = FindInMap(m_mapSkillProto, dwID);
		if(P_VALID(pSkill))
		{
			_tcsncpy( pSkill->szName,		VarSkillDisplay->GetString( _T("name"),				(*iter).c_str(), _T("")	), sizeof(pSkill->szName)/sizeof(TCHAR));
			_tcsncpy( pSkill->szDesc,		VarSkillDisplay->GetString( _T("desc"),				(*iter).c_str(), _T("")	), sizeof(pSkill->szDesc)/sizeof(TCHAR));
			_tcsncpy( pSkill->szIcon,		VarSkillDisplay->GetString( _T("icon"),				(*iter).c_str(), _T("") ), sizeof(pSkill->szIcon)/sizeof(TCHAR));
			_tcsncpy( pSkill->szActionMod,	VarSkillDisplay->GetString( _T("action_model"),		(*iter).c_str(), _T("") ), sizeof(pSkill->szActionMod)/sizeof(TCHAR));
			_tcsncpy( pSkill->szPrepareMod,	VarSkillDisplay->GetString( _T("prepare_action"),	(*iter).c_str(), _T("") ), sizeof(pSkill->szPrepareMod)/sizeof(TCHAR));
			_tcsncpy( pSkill->szNextDesc,	VarSkillDisplay->GetString( _T("next_desc"),		(*iter).c_str(), _T("") ), sizeof(pSkill->szNextDesc)/sizeof(TCHAR));
			_tcsncpy( pSkill->szSound,		VarSkillDisplay->GetString( _T("sound"),			(*iter).c_str(), _T("") ), sizeof(pSkill->szSound)/sizeof(TCHAR));

			// 起手和攻击特效支持多个挂载点
			_tcsncpy( pSkill->szPreSfx,		VarSkillDisplay->GetString( _T("pre_sfx"),			(*iter).c_str(), _T("") ), sizeof(pSkill->szPreSfx)/sizeof(TCHAR));
			string_split( VarSkillDisplay->GetString( _T("tag_pre_sfx"), (*iter).c_str(), _T("") ), _T(','), pSkill->vecTagPreSfx );
			if( pSkill->vecTagPreSfx.empty() && _T('\0') != pSkill->szPreSfx[0] )
				pSkill->vecTagPreSfx.push_back( _T("") );
			_tcsncpy( pSkill->szAtkSfx,		VarSkillDisplay->GetString( _T("atk_sfx"),			(*iter).c_str(), _T("") ), sizeof(pSkill->szAtkSfx)/sizeof(TCHAR));
			string_split( VarSkillDisplay->GetString( _T("tag_atk_sfx"), (*iter).c_str(), _T("") ), _T(','), pSkill->vecTagAtkSfx );
			if( pSkill->vecTagAtkSfx.empty() && _T('\0') != pSkill->szAtkSfx[0] )
				pSkill->vecTagAtkSfx.push_back( _T("") );


			_tcsncpy( pSkill->szLchSfx,		VarSkillDisplay->GetString( _T("lch_sfx"),			(*iter).c_str(), _T("") ), sizeof(pSkill->szLchSfx)/sizeof(TCHAR));
			_tcsncpy( pSkill->szTagLchSfx,	VarSkillDisplay->GetString( _T("tag_lch_sfx"),		(*iter).c_str(), _T("") ), sizeof(pSkill->szTagLchSfx)/sizeof(TCHAR));
			_tcsncpy( pSkill->szBatkSfx,	VarSkillDisplay->GetString( _T("batk_sfx"),			(*iter).c_str(), _T("") ), sizeof(pSkill->szBatkSfx)/sizeof(TCHAR));
			pSkill->nNumBatkSfx		=		VarSkillDisplay->GetInt	  ( _T("num_batk_sfx"),		(*iter).c_str(), 0);

			pSkill->bActionBlend		= (BOOL)VarSkillDisplay->GetDword(_T("action_blend"),		(*iter).c_str(),	FALSE);
			pSkill->bAutoNormalAttack	= (BOOL)VarSkillDisplay->GetDword(_T("auto_normal_attack"),	(*iter).c_str(),	TRUE);

			pSkill->dwBatkAction[0]	= (BOOL)VarSkillDisplay->GetDword(_T("batk_model1"),	(*iter).c_str(),	GT_INVALID);
			pSkill->dwBatkAction[1]	= (BOOL)VarSkillDisplay->GetDword(_T("batk_model2"),	(*iter).c_str(),	GT_INVALID);
			pSkill->dwBatkAction[2]	= (BOOL)VarSkillDisplay->GetDword(_T("batk_model3"),	(*iter).c_str(),	GT_INVALID);
			pSkill->dwBatkAction[3]	= (BOOL)VarSkillDisplay->GetDword(_T("batk_model4"),	(*iter).c_str(),	GT_INVALID);
			pSkill->dwBatkAction[4]	= (BOOL)VarSkillDisplay->GetDword(_T("batk_model5"),	(*iter).c_str(),	GT_INVALID);
			//more
		}
	}
	KillObj("SkillDisplay");


	//---------------------------------------------------------------------------------
	//读取buff_proto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarBuffProto = CreateObj("BuffProto", "VarContainer");
	if(!VarBuffProto->Load("VFS_System", szBuffProto, "id", &BuffProtoFieldList))
		IMSG(_T("Load file buff_proto.xml failed\r\n"));

	for(iter = BuffProtoFieldList.begin(); iter != BuffProtoFieldList.end(); ++iter)
	{
		tagBuffProtoClient buff;
		buff.dwID		=				VarBuffProto->GetDword( _T("id"),		(*iter).c_str(), GT_INVALID );
		buff.eType		= (EBuffType)	VarBuffProto->GetDword( _T("type"),		(*iter).c_str(), EBT_Null );
		buff.nType2		= (INT)			VarBuffProto->GetDword( _T("type2"),	(*iter).c_str(), 0 );
		buff.nType3		= (INT)			VarBuffProto->GetDword( _T("type3"),	(*iter).c_str(), 0 );
		buff.bInstant	= (BOOL)        VarBuffProto->GetDword( _T("instant"),	(*iter).c_str(), 0 );

		buff.eEffect[EBEM_Persist]			=	(enum EBuffEffectType)VarBuffProto->GetInt(_T("effect_type"), (*iter).c_str(), EBET_Null);
		buff.eEffect[EBEM_Instant]			=	(enum EBuffEffectType)VarBuffProto->GetInt(_T("instant_effect_type"), (*iter).c_str(), EBET_Null);
		buff.eEffect[EBEM_Inter]				=	(enum EBuffEffectType)VarBuffProto->GetInt(_T("inter_effect_type"), (*iter).c_str(), EBET_Null);
		buff.eEffect[EBEM_Finish]				=	(enum EBuffEffectType)VarBuffProto->GetInt(_T("end_effect_type"), (*iter).c_str(), EBET_Null);

		buff.dwInterruptFlag = 0;
		BOOL bManual = (BOOL)VarBuffProto->GetDword( _T("interrupt_manual"),	(*iter).c_str(), 0 );
		if( bManual )
			buff.dwInterruptFlag |= EBIF_Manual;

        buff.mapRoleAttrEffect.Clear();
        buff.mapRoleAttrEffectPct.Clear();

        // 人物属性加成
        INT nERAPhysique				=	VarBuffProto->GetInt(_T("ERA_Physique"),				(*iter).c_str(), 0);
        INT nERAStrength				=	VarBuffProto->GetInt(_T("ERA_Strength"),				(*iter).c_str(), 0);
        INT nERAPneuma					=	VarBuffProto->GetInt(_T("ERA_Pneuma"),					(*iter).c_str(), 0);
        INT nERAInnerForce				=	VarBuffProto->GetInt(_T("ERA_InnerForce"),				(*iter).c_str(), 0);
        INT nERATechnique				=	VarBuffProto->GetInt(_T("ERA_Technique"),				(*iter).c_str(), 0);
        INT nERAAgility					=	VarBuffProto->GetInt(_T("ERA_Agility"),				(*iter).c_str(), 0);
        INT nERAMaxHP					=	VarBuffProto->GetInt(_T("ERA_MaxHP"),					(*iter).c_str(), 0);
        INT nERAMaxMP					=	VarBuffProto->GetInt(_T("ERA_MaxMP"),					(*iter).c_str(), 0);
        INT nERAExAttack				=	VarBuffProto->GetInt(_T("ERA_ExAttack"),				(*iter).c_str(), 0);
        INT nERAExDefense				=	VarBuffProto->GetInt(_T("ERA_ExDefense"),				(*iter).c_str(), 0);
        INT nERAInAttack				=	VarBuffProto->GetInt(_T("ERA_InAttack"),				(*iter).c_str(), 0);
        INT nERAInDefense				=	VarBuffProto->GetInt(_T("ERA_InDefense"),				(*iter).c_str(), 0);
        INT nERAAttackTec				=	VarBuffProto->GetInt(_T("ERA_AttackTec"),				(*iter).c_str(), 0);
        INT nERADefenseTec				=	VarBuffProto->GetInt(_T("ERA_DefenseTec"),				(*iter).c_str(), 0);
        INT nERAHitRate					=	VarBuffProto->GetInt(_T("ERA_HitRate"),				(*iter).c_str(), 0);
        INT nERADodge					=	VarBuffProto->GetInt(_T("ERA_Dodge"),					(*iter).c_str(), 0);
        INT nERAMaxEndurance			=	VarBuffProto->GetInt(_T("ERA_MaxEndurance"),			(*iter).c_str(), 0);
        INT nERAHPRegainRate			=	VarBuffProto->GetInt(_T("ERA_HPRegainRate"),			(*iter).c_str(), 0);
        INT nERAMPRegainRate			=	VarBuffProto->GetInt(_T("ERA_MPRegainRate"),			(*iter).c_str(), 0);
        INT nERAMaxVitality				=	VarBuffProto->GetInt(_T("ERA_MaxVitality"),			(*iter).c_str(), 0);
        INT nERAVitalityRegainRate		=	VarBuffProto->GetInt(_T("ERA_VitalityRegainRate"),		(*iter).c_str(), 0);
        INT nERASpeed_XZ				=	VarBuffProto->GetInt(_T("ERA_Speed_XZ"),				(*iter).c_str(), 0);
        INT nERASpeed_Y					=	VarBuffProto->GetInt(_T("ERA_Speed_Y"),				(*iter).c_str(), 0);
        INT nERASpeedSwim				=	VarBuffProto->GetInt(_T("ERA_Speed_Swin"),				(*iter).c_str(), 0);
        INT nERASpeedMount				=	VarBuffProto->GetInt(_T("ERA_Speed_Mount"),			(*iter).c_str(), 0);
        INT nERAWeaponDmgMin			=	VarBuffProto->GetInt(_T("ERA_WeaponDmgMin"),			(*iter).c_str(), 0);
        INT nERAWeaponDmgMax			=	VarBuffProto->GetInt(_T("ERA_WeaponDmgMax"),			(*iter).c_str(), 0);
        INT nERAWeaponSoul				=	VarBuffProto->GetInt(_T("ERA_WeaponSoul"),				(*iter).c_str(), 0);
        INT nERAArmor					=	VarBuffProto->GetInt(_T("ERA_Armor"),					(*iter).c_str(), 0);
        INT nERADerate_Ordinary			=	VarBuffProto->GetInt(_T("ERA_Derate_Ordinary"),		(*iter).c_str(), 0);
        INT nERADerate_Bleeding			=	VarBuffProto->GetInt(_T("ERA_Derate_Bleeding"),		(*iter).c_str(), 0);
        INT nERADerate_Brunt			=	VarBuffProto->GetInt(_T("ERA_Derate_Brunt"),			(*iter).c_str(), 0);
        INT nERADerate_Bang				=	VarBuffProto->GetInt(_T("ERA_Derate_Bang"),			(*iter).c_str(), 0);
        INT nERADerate_Poison			=	VarBuffProto->GetInt(_T("ERA_Derate_Poison"),			(*iter).c_str(), 0);
        INT nERADerate_Thinker			=	VarBuffProto->GetInt(_T("ERA_Derate_Thinker"),			(*iter).c_str(), 0);
        INT nERADerate_Injury			=	VarBuffProto->GetInt(_T("ERA_Derate_Injury"),			(*iter).c_str(), 0);
        INT nERADerate_Stunt			=	VarBuffProto->GetInt(_T("ERA_Derate_Stunt"),			(*iter).c_str(), 0);
        INT nERADerate_ExAttack			=	VarBuffProto->GetInt(_T("ERA_Derate_ExAttack"),		(*iter).c_str(), 0);
        INT nERADerate_InAttack			=	VarBuffProto->GetInt(_T("ERA_Derate_InAttack"),		(*iter).c_str(), 0);
        INT nERADerate_All				=	VarBuffProto->GetInt(_T("ERA_Derate_All"),				(*iter).c_str(), 0);
        INT nERAExDamage				=	VarBuffProto->GetInt(_T("ERA_ExDamage"),				(*iter).c_str(), 0);
        INT nERAExDamage_Absorb			=	VarBuffProto->GetInt(_T("ERA_ExDamage_Absorb"),		(*iter).c_str(), 0);
        INT nERAResist_Bleeding			=	VarBuffProto->GetInt(_T("ERA_Resist_Bleeding"),		(*iter).c_str(), 0);
        INT nERAResist_Weak				=	VarBuffProto->GetInt(_T("ERA_Resist_Weak"),			(*iter).c_str(), 0);
        INT nERAResist_Choas			=	VarBuffProto->GetInt(_T("ERA_Resist_Choas"),			(*iter).c_str(), 0);
        INT nERAResist_Special			=	VarBuffProto->GetInt(_T("ERA_Resist_Special"),			(*iter).c_str(), 0);
        INT nERARegain_Addtion			=	VarBuffProto->GetInt(_T("ERA_Regain_Addtion"),			(*iter).c_str(), 0);
        INT nERAAttack_MissRate			=	VarBuffProto->GetInt(_T("ERA_Attack_MissRate"),		(*iter).c_str(), 0);
        INT nERACloseAttack_DodgeRate	=	VarBuffProto->GetInt(_T("ERA_CloseAttack_DodgeRate"),	(*iter).c_str(), 0);
        INT nERARemoteAttack_DodgeRate	=	VarBuffProto->GetInt(_T("ERA_RemoteAttack_DodgeRate"), (*iter).c_str(), 0);
        INT nERACrit_Rate				=	VarBuffProto->GetInt(_T("ERA_Crit_Rate"),				(*iter).c_str(), 0);
        INT nERACrit_Amount				=	VarBuffProto->GetInt(_T("ERA_Crit_Amount"),			(*iter).c_str(), 0);
        INT nERABlock_Rate				=	VarBuffProto->GetInt(_T("ERA_Block_Rate"),				(*iter).c_str(), 0);
        INT nERAInspiration				=	VarBuffProto->GetInt(_T("ERA_Inspiration"),			(*iter).c_str(), 0);
        INT nERALurk					=	VarBuffProto->GetInt(_T("ERA_Lurk"),					(*iter).c_str(), 0);
        INT nERASavvy					=	VarBuffProto->GetInt(_T("ERA_Savvy"),					(*iter).c_str(), 0);
        INT nERAEnmity_Degree			=	VarBuffProto->GetInt(_T("ERA_Enmity_Degree"),			(*iter).c_str(), 0);
        INT nERAShape					=	VarBuffProto->GetInt(_T("ERA_Shape"),					(*iter).c_str(), 0);
        INT nERAExp_Add_Rate			=	VarBuffProto->GetInt(_T("ERA_Exp_Add_Rate"),			(*iter).c_str(), 0);
        INT nERAMoney_Add_Rate			=	VarBuffProto->GetInt(_T("ERA_Money_Add_Rate"),			(*iter).c_str(), 0);
        INT nERALoot_Add_Rate			=	VarBuffProto->GetInt(_T("ERA_Loot_Add_Rate"),			(*iter).c_str(), 0);
        INT nERA_Fortune			    =	VarBuffProto->GetInt(_T("ERA_Fortune"),                (*iter).c_str(), 0);
        INT nERA_Appearance			    =	VarBuffProto->GetInt(_T("ERA_Appearance"),			    (*iter).c_str(), 0);
        INT nERA_Rein			        =	VarBuffProto->GetInt(_T("ERA_Rein"),			        (*iter).c_str(), 0);
        INT nERA_Knowledge			    =	VarBuffProto->GetInt(_T("ERA_Knowledge"),			    (*iter).c_str(), 0);
        INT nERA_Morality			    =	VarBuffProto->GetInt(_T("ERA_Morality"),			    (*iter).c_str(), 0);
        INT nERA_Culture			    =	VarBuffProto->GetInt(_T("ERA_Culture"),			    (*iter).c_str(), 0);
        INT nERA_Hostility			    =	VarBuffProto->GetInt(_T("ERA_Hostility"),			    (*iter).c_str(), 0);

        AddBuffRoleAtt(buff, ERA_Physique,				nERAPhysique);
        AddBuffRoleAtt(buff, ERA_Strength,				nERAStrength);
        AddBuffRoleAtt(buff, ERA_Pneuma,					nERAPneuma);
        AddBuffRoleAtt(buff, ERA_InnerForce,				nERAInnerForce);
        AddBuffRoleAtt(buff, ERA_Technique,				nERATechnique);
        AddBuffRoleAtt(buff, ERA_Agility,					nERAAgility);
        AddBuffRoleAtt(buff, ERA_MaxHP,					nERAMaxHP);
        AddBuffRoleAtt(buff, ERA_MaxMP,					nERAMaxMP);
        AddBuffRoleAtt(buff, ERA_ExAttack,				nERAExAttack);
        AddBuffRoleAtt(buff, ERA_ExDefense,				nERAExDefense);
        AddBuffRoleAtt(buff, ERA_InAttack,				nERAInAttack);
        AddBuffRoleAtt(buff, ERA_InDefense,				nERAInDefense);
        AddBuffRoleAtt(buff, ERA_AttackTec,				nERAAttackTec);
        AddBuffRoleAtt(buff, ERA_DefenseTec,				nERADefenseTec);
        AddBuffRoleAtt(buff, ERA_HitRate,					nERAHitRate);
        AddBuffRoleAtt(buff, ERA_Dodge,					nERADodge);
        AddBuffRoleAtt(buff, ERA_MaxEndurance,			nERAMaxEndurance);
        AddBuffRoleAtt(buff, ERA_HPRegainRate,			nERAHPRegainRate);
        AddBuffRoleAtt(buff, ERA_MPRegainRate,			nERAMPRegainRate);
        AddBuffRoleAtt(buff, ERA_MaxVitality,				nERAMaxVitality);
        AddBuffRoleAtt(buff, ERA_VitalityRegainRate,		nERAVitalityRegainRate);
        AddBuffRoleAtt(buff, ERA_Speed_XZ,				nERASpeed_XZ);
        AddBuffRoleAtt(buff, ERA_Speed_Y,					nERASpeed_Y);
        AddBuffRoleAtt(buff, ERA_Speed_Swim,				nERASpeedSwim);
        AddBuffRoleAtt(buff, ERA_Speed_Mount,				nERASpeedMount);
        AddBuffRoleAtt(buff, ERA_WeaponDmgMin,			nERAWeaponDmgMin);
        AddBuffRoleAtt(buff, ERA_WeaponDmgMax,			nERAWeaponDmgMax);
        AddBuffRoleAtt(buff, ERA_WeaponSoul,				nERAWeaponSoul);
        AddBuffRoleAtt(buff, ERA_Armor,					nERAArmor);
        AddBuffRoleAtt(buff, ERA_Derate_Ordinary,			nERADerate_Ordinary);
        AddBuffRoleAtt(buff, ERA_Derate_Bleeding,			nERADerate_Bleeding);
        AddBuffRoleAtt(buff, ERA_Derate_Brunt,			nERADerate_Brunt);
        AddBuffRoleAtt(buff, ERA_Derate_Bang,				nERADerate_Bang);
        AddBuffRoleAtt(buff, ERA_Derate_Poison,			nERADerate_Poison);
        AddBuffRoleAtt(buff, ERA_Derate_Thinker,			nERADerate_Thinker);
        AddBuffRoleAtt(buff, ERA_Derate_Injury,			nERADerate_Injury);
        AddBuffRoleAtt(buff, ERA_Derate_Stunt,			nERADerate_Stunt);
        AddBuffRoleAtt(buff, ERA_Derate_ExAttack,			nERADerate_ExAttack);
        AddBuffRoleAtt(buff, ERA_Derate_InAttack,			nERADerate_InAttack);
        AddBuffRoleAtt(buff, ERA_Derate_ALL,				nERADerate_All);
        AddBuffRoleAtt(buff, ERA_ExDamage,				nERAExDamage);
        AddBuffRoleAtt(buff, ERA_ExDamage_Absorb,			nERAExDamage_Absorb);	
        AddBuffRoleAtt(buff, ERA_Resist_Bleeding,			nERAResist_Bleeding);	
        AddBuffRoleAtt(buff, ERA_Resist_Weak,				nERAResist_Weak);	
        AddBuffRoleAtt(buff, ERA_Resist_Choas,			nERAResist_Choas);	
        AddBuffRoleAtt(buff, ERA_Resist_Special,			nERAResist_Special);	
        AddBuffRoleAtt(buff, ERA_Regain_Addtion,			nERARegain_Addtion);	
        AddBuffRoleAtt(buff, ERA_Attack_MissRate,			nERAAttack_MissRate);	
        AddBuffRoleAtt(buff, ERA_CloseAttack_DodgeRate,	nERACloseAttack_DodgeRate);	
        AddBuffRoleAtt(buff, ERA_RemoteAttack_DodgeRate,	nERARemoteAttack_DodgeRate);	
        AddBuffRoleAtt(buff, ERA_Crit_Rate,				nERACrit_Rate);	
        AddBuffRoleAtt(buff, ERA_Crit_Amount,				nERACrit_Amount);	
        AddBuffRoleAtt(buff, ERA_Block_Rate,				nERABlock_Rate);	
        AddBuffRoleAtt(buff, ERA_Inspiration,				nERAInspiration);	
        AddBuffRoleAtt(buff, ERA_Lurk,					nERALurk);	
        AddBuffRoleAtt(buff, ERA_Savvy,					nERASavvy);	
        AddBuffRoleAtt(buff, ERA_Enmity_Degree,			nERAEnmity_Degree);
        AddBuffRoleAtt(buff, ERA_Shape,					nERAShape);
        AddBuffRoleAtt(buff, ERA_Exp_Add_Rate,			nERAExp_Add_Rate);
        AddBuffRoleAtt(buff, ERA_Money_Add_Rate,			nERAMoney_Add_Rate);
        AddBuffRoleAtt(buff, ERA_Loot_Add_Rate,			nERALoot_Add_Rate);

        AddBuffRoleAtt(buff, ERA_Fortune,			nERA_Fortune);
        AddBuffRoleAtt(buff, ERA_Appearance,			ERA_Appearance);
        AddBuffRoleAtt(buff, ERA_Rein,			nERA_Rein);
        AddBuffRoleAtt(buff, ERA_Knowledge,			nERA_Knowledge);
        AddBuffRoleAtt(buff, ERA_Morality,			nERA_Morality);
        AddBuffRoleAtt(buff, ERA_Culture,			nERA_Culture);
        AddBuffRoleAtt(buff, ERA_Hostility,			nERA_Hostility);

		//more

		m_mapBuffProto.insert(make_pair(buff.dwID, buff));
	}

	KillObj("BuffProto");


	//---------------------------------------------------------------------------------
	//读取buff_name
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarBuffDisplay = CreateObj("BuffDisplay", "VarContainer");
	if(!VarBuffDisplay->Load("VFS_System", szBuffDisplay, "id", &BuffDisplayFieldList))
		IMSG(_T("Load file buff_name.xml failed\r\n"));

	for(iter = BuffDisplayFieldList.begin(); iter != BuffDisplayFieldList.end(); ++iter)
	{
		DWORD dwID = VarBuffDisplay->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tagBuffProtoClient* pBuff = FindInMap(m_mapBuffProto, dwID);
		if(P_VALID(pBuff))
		{
			_tcsncpy( pBuff->szName,		VarBuffDisplay->GetString( _T("name"),				(*iter).c_str(), _T("")	), sizeof(pBuff->szName)/sizeof(TCHAR));
			_tcsncpy( pBuff->szDesc,		VarBuffDisplay->GetString( _T("desc"),				(*iter).c_str(), _T("")	), sizeof(pBuff->szDesc)/sizeof(TCHAR));
			_tcsncpy( pBuff->szIcon,		VarBuffDisplay->GetString( _T("icon"),				(*iter).c_str(), _T("") ), sizeof(pBuff->szIcon)/sizeof(TCHAR));
			_tcsncpy( pBuff->szActionMod,	VarBuffDisplay->GetString( _T("action_model"),		(*iter).c_str(), _T("") ), sizeof(pBuff->szActionMod)/sizeof(TCHAR));
			_tcsncpy( pBuff->szEffectMod,	VarBuffDisplay->GetString( _T("effect_model"),		(*iter).c_str(), _T("") ), sizeof(pBuff->szEffectMod)/sizeof(TCHAR));
			_tcsncpy( pBuff->szAdjMod,		VarBuffDisplay->GetString( _T("adj_model"),			(*iter).c_str(), _T("") ), sizeof(pBuff->szAdjMod)/sizeof(TCHAR));
			_tcsncpy( pBuff->szSelfMod,		VarBuffDisplay->GetString( _T("self_model"),		(*iter).c_str(), _T("") ), sizeof(pBuff->szSelfMod)/sizeof(TCHAR));
			_tcsncpy( pBuff->szCountDown,	VarBuffDisplay->GetString( _T("count_down_text"),	(*iter).c_str(), _T("") ), sizeof(pBuff->szCountDown)/sizeof(TCHAR));
			pBuff->nIfHideLWeapon = VarBuffDisplay->GetInt( _T("weapon_show_limit"), (*iter).c_str(), 0 );
			//more
		}
	}
	KillObj("BuffDisplay");


	//---------------------------------------------------------------------------------
	//读取trigger_proto
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> VarTriggerProto = CreateObj("TriggerProto", "VarContainer");
	if(!VarTriggerProto->Load("VFS_System", szTriggerProto, "id", &TriggerProtoFieldList))
		IMSG(_T("Load file trigger_proto.xml failed\r\n"));

	for(iter = TriggerProtoFieldList.begin(); iter != TriggerProtoFieldList.end(); ++iter)
	{
		tagTriggerProto trigger;
		trigger.dwID			=						VarTriggerProto->GetDword( _T("id"),		(*iter).c_str(), 0 );
		trigger.eEventType		= (ETriggerEventType)	VarTriggerProto->GetDword( _T("type"),		(*iter).c_str(), 0 );
		
		//more

		m_mapTriggerProto.insert(make_pair(trigger.dwID, trigger));
	}

	KillObj("TriggerProto");

	//-----------------------------------------------------------------------------------
	//读取连接技
	//-----------------------------------------------------------------------------------
	TObjRef<VarContainer> VarConSkillProto = CreateObj("ConSkillProto", "VarContainer");
	if(!VarConSkillProto->Load("VFS_System", szConSkillProto, "id", &ConSkillProtoFiledList))
		IMSG(_T("Load file skill_proto.xml failed\r\n"));

	for(iter = ConSkillProtoFiledList.begin(); iter != ConSkillProtoFiledList.end(); ++iter)
	{
		tagContinuteSkill skill;
		skill.dwTypeID = VarConSkillProto->GetDword( _T("id"),		(*iter).c_str(), GT_INVALID );
		skill.nTarget = VarConSkillProto->GetDword( _T("buff_target"),		(*iter).c_str(), GT_INVALID );
		for( int i=0; i<MAX_CONTINUE_SKILL_NUM; ++i )
		{
			TCHAR szBuff[X_SHORT_NAME] = {0};
			_sntprintf( szBuff, X_SHORT_NAME, _T("skill_id%d"), i+1 );
			skill.dwSkill[i] = VarConSkillProto->GetDword( szBuff,		(*iter).c_str(), GT_INVALID );
		}
		m_mapConSkillProto.insert( make_pair(skill.dwTypeID, skill) );
	}

	KillObj("ConSkillProto");

	//---------------------------------------------------------------------------------
	//读取skill_box
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> varSkillBox = CreateObj("SkillBox", "VarContainer");
	if(!varSkillBox->Load("VFS_System", szSkillBoxProto, "id", &skillBoxFiledList))
		IMSG(_T("Load file skill_box.xml failed\r\n"));
	for(iter = skillBoxFiledList.begin(); iter != skillBoxFiledList.end(); ++iter)
	{
		tagSkillBoxProto skillBox;
		skillBox.dwID = varSkillBox->GetDword( _T("id"),		(*iter).c_str(), GT_INVALID );
		skillBox.bCanBeLearned = varSkillBox->GetDword( _T("learn_limit"),		(*iter).c_str(), 0 )!=0?TRUE:FALSE;
		for( int i=0; i<tagSkillBoxProto::E_SBP_Size; ++i )
		{
			TCHAR szBuff[X_SHORT_NAME] = {0};
			_sntprintf( szBuff, X_SHORT_NAME, _T("skill_id%d"), i+1 );
			skillBox.dwSkillIDs[i] = varSkillBox->GetDword( szBuff,		(*iter).c_str(), GT_INVALID );
		}
		m_mapSkillBox.insert( make_pair(skillBox.dwID, skillBox) );
	}
	KillObj("SkillBox");

	//---------------------------------------------------------------------------------
	//读取skill_maps
	//---------------------------------------------------------------------------------
	TObjRef<VarContainer> varSkillMaps = CreateObj("SkillMap", "VarContainer");
	if(!varSkillMaps->Load("VFS_System", szSkillMapProto, "id", &skillMapFiledList))
		IMSG(_T("Load file mapbox_proto.xml failed\r\n"));
	for(iter = skillMapFiledList.begin(); iter != skillMapFiledList.end(); ++iter)
	{
		tagSkillableMaps skillmap;
		skillmap.dwID = varSkillMaps->GetDword( _T("id"),		(*iter).c_str(), 0 );
		for (int i = 0; i < MAX_SKILL_MAP; i++)
		{
			TCHAR szBuff[X_SHORT_NAME] = {0};
			_sntprintf( szBuff, X_SHORT_NAME, _T("map_id%d"), i+1 );
			skillmap.dwMapID[i] = varSkillMaps->GetDword( szBuff,		(*iter).c_str(), 0 );
		}

		m_mapSkillMaps.insert( make_pair(skillmap.dwID, skillmap) );
	}
	KillObj("SkillMap");
}

VOID SkillProtoData::AddSkillRoleAtt( tagSkillProto& sSkill, ERoleAttribute eType, INT nValue )
{
	ASSERT(eType > ERA_Null && eType < ERA_End);

	if(0 == nValue)
		return;

	if(MIsValuePct(nValue))
	{
		sSkill.mapRoleAttModPct.Add(eType, MValuePctTrans(nValue));
	}
	else
	{
		sSkill.mapRoleAttMod.Add(eType, nValue);
	}
}

VOID SkillProtoData::AddBuffRoleAtt( tagBuffProto& sBuff, ERoleAttribute eType, INT nValue )
{
    ASSERT(eType > ERA_Null && eType < ERA_End);

    if(0 == nValue)
        return;

    if(MIsValuePct(nValue))
    {
        sBuff.mapRoleAttrEffectPct.Add(eType, MValuePctTrans(nValue));
    }
    else
    {
        sBuff.mapRoleAttrEffect.Add(eType, nValue);
    }
}