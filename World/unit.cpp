//-------------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: role.h
// author: Aslan
// actor:
// data: 2008-7-11
// last:
// brief: 人物数据结构
//-------------------------------------------------------------------------------
#include "StdAfx.h"

#include "../ServerDefine/base_define.h"
#include "../WorldDefine/role_att.h"
#include "../WorldDefine/buff_define.h"
#include "../WorldDefine/msg_combat.h"
#include "../WorldDefine/action.h"
#include "../WorldDefine/move_define.h"
#include "../WorldDefine/remote_role.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "../WorldDefine/ItemDefine.h"
#include "world.h"
#include "role.h"
#include "title_mgr.h"
#include "creature.h"
#include "creature_ai.h"
#include "map.h"
#include "map_creator.h"
#include "map_mgr.h"
#include "att_res.h"
#include "buff.h"
#include "buff_effect.h"
#include "unit.h"
#include "group_mgr.h"
#include "athletics_room_mgr.h"
#include "athletics_room.h"
#include "athletics_system.h"
#include "map_instance_stable.h"
#include "athletics_pvp.h"
#include "athletics_pvp_mgr.h"
#include "../WorldDefine/family_define.h"
#include "event_mgr.h"
#include "HolySoul.h"

extern AthleticsSystem g_AthleticsSystem;
//-------------------------------------------------------------------------------
// constructor
//-------------------------------------------------------------------------------
Unit::Unit(DWORD dwID, DWORD dwMapID, FLOAT fX, FLOAT fY, FLOAT fZ, FLOAT fFaceX, FLOAT fFaceY, FLOAT fFaceZ)
: m_dwID(dwID), m_Size(X_DEF_ROLE_SIZE_X, X_DEF_ROLE_SIZE_Y, X_DEF_ROLE_SIZE_Z), m_fXZSpeed(X_DEF_XZ_SPEED), 
  m_fYSpeed(X_DEF_Y_SPEED), m_fSwimXZSpeed(X_DEF_XZ_SPEED), m_fDropXZSpeed(X_DEF_DROP_XZ_SPEED), m_fSlideSpeed(X_DEF_SLIDE_SPEED), 
  m_dwMapID(dwMapID), m_pMap(NULL), m_dwInstanceID(GT_INVALID), m_bNeedRecal(FALSE), m_StateMgr(), m_nHPMPRegainTickCountDown(HP_MP_REGAIN_INTER_TICK),
  m_nVitalityRegainTickCountDown(VITALITY_REGAIN_INTER_TICK), m_nEnduranceRegainTickCountDown(ENDURANCE_REGAIN_INTER_TICK), m_nProduceSkillNum(0),
  m_nInjuryRegainTickCountDown(INJURY_REGAIN_INTER_TICK),m_bTriggerOff(FALSE), m_eCampType(ECamp_Null), m_byHostileCamp(0),m_nHolyValueTickCountDown(HOLY_INCREASE_VAL)
{
	ZeroMemory(m_nAtt, sizeof(m_nAtt));
	ZeroMemory(m_nBaseAtt, sizeof(m_nBaseAtt));
	ZeroMemory(m_nAttMod, sizeof(m_nAttMod));
	ZeroMemory(m_nAttModPct, sizeof(m_nAttModPct));
	ZeroMemory(m_bAttRecalFlag, sizeof(m_bAttRecalFlag));

	m_MoveData.Init(this, fX, fY, fZ, fFaceX, fFaceY, fFaceZ);
	m_CombatHandler.Init(this);
	m_nModTalentAvail_limit = 0 ;

	if(IsRole())
	{
		for (int i = 0; i<X_NEED_PK_LOG_ROLE_NUM; ++i)
		{
			if(m_dwID == g_dwNeedPKLogRoleID[i])
			{
				m_CombatHandler.SetNeedPKLogOn();
				break;
			}
		}
	}

	for(int i = 0; i < EFSA_Num; ++i )
	{
		m_nAbility[i] = -1;
		m_nOldAbility[i] = -1;
	}

	m_nOutPrisonMoraleRegainTickCountDown = g_world.GetMoralityTime() * TICK_PER_SECOND;
}

//-------------------------------------------------------------------------------
// destructor
//-------------------------------------------------------------------------------
Unit::~Unit()
{
	// 停止当前的技能
	m_CombatHandler.End();
	// 删除所有技能
	TMap<DWORD, Skill*>::TMapIterator itSkill = m_mapSkill.Begin();
	Skill* pSkill = NULL;

	while( m_mapSkill.PeekNext(itSkill, pSkill) )
	{
		SAFE_DEL(pSkill);
	}
	m_mapSkill.Clear();

	// 删除所有的buff修改器
	TList<DWORD>* pListModifier = NULL;
	TMap<DWORD, TList<DWORD>*>::TMapIterator itModifier = m_mapBuffModifier.Begin();
	while( m_mapBuffModifier.PeekNext(itModifier, pListModifier) )
	{
		SAFE_DEL(pListModifier);
	}
	m_mapBuffModifier.Clear();

	// 删除所有trigger修改器
	itModifier = m_mapTriggerModifier.Begin();
	while( m_mapTriggerModifier.PeekNext(itModifier, pListModifier) )
	{
		SAFE_DEL(pListModifier);
	}
	m_mapTriggerModifier.Clear();

	m_vecSpecSkillID.clear();
}

//-------------------------------------------------------------------------------
// 更新随时间更新的函数
// Jason 2010-6-22 每在线一分钟，增加道德值1
//-------------------------------------------------------------------------------
VOID Unit::UpdateTimedIssue()
{
	if( IsDead() ) return;

	// 回血回蓝
	if( --m_nHPMPRegainTickCountDown <= 0 )
	{
		m_nHPMPRegainTickCountDown = HP_MP_REGAIN_INTER_TICK;

		if( GetAttValue(ERA_HPRegainRate) != 0 )
		{
			if( GetAttValue(ERA_HP) < GetAttValue(ERA_MaxHP) && GetAttValue(ERA_HP) >= 0 )
				ChangeHP(GetAttValue(ERA_HPRegainRate), this);
		}
		if( GetAttValue(ERA_MPRegainRate) != 0 )
		{
			if( GetAttValue(ERA_MP) < GetAttValue(ERA_MaxMP) && GetAttValue(ERA_MP) >= 0 )
				ChangeMP(GetAttValue(ERA_MPRegainRate));
		}
	}

	// 回活力
	if( --m_nVitalityRegainTickCountDown <= 0 )
	{
		m_nVitalityRegainTickCountDown = VITALITY_REGAIN_INTER_TICK;

		if( GetAttValue(ERA_VitalityRegainRate) != 0 )
		{
			ChangeVitality(GetAttValue(ERA_VitalityRegainRate));
		}
	}

	// 回持久力，不再恢复神力
	//if( --m_nEnduranceRegainTickCountDown <= 0 )
	//{
	//	m_nEnduranceRegainTickCountDown = ENDURANCE_REGAIN_INTER_TICK;

	//	ChangeEndurance(5);
	//}

	// 回内伤
	if( --m_nInjuryRegainTickCountDown <= 0 )
	{
		m_nInjuryRegainTickCountDown = INJURY_REGAIN_INTER_TICK;
		if (GetAttValue(ERA_Injury) > 0)
		{
			ModAttValue(ERA_Injury, INJURY_INCREASE_VAL);
		}		
	}

	// 其它的一些随时间变化的

	if( IsRole() )
	{
		Role* pRole = static_cast<Role *>(this);
		ASSERT(P_VALID(pRole));

		// 如果没有召唤圣灵，每3秒角色圣灵值加1		//这个也不要了
// 		if( !pRole->IsInRoleState(ERS_CALLHOLY) && --m_nHolyValueTickCountDown <= 0 )
// 		{
// 			m_nHolyValueTickCountDown = HOLY_VALUE_INTER_TICK;
// 			if (GetAttValue(ERA_Holy)  < MAX_ROLE_HOLY_VALUE )
// 			{
// 				ModAttValue(ERA_Holy, HOLY_INCREASE_VAL);
// 			}	
// 			
// 		}

		// 狱中角色回道德
		if( pRole->IsInRoleState(ERS_PrisonArea) && --m_nInPrisonMoraleRegainTickCountDown <= 0 )
		{
			m_nInPrisonMoraleRegainTickCountDown = INPRISON_MORALE_REGAIN_INTER_TICK;

			INT nCurMorale = pRole->GetAttValue(ERA_Morality);
			INT nNeed = 0;
			if (nCurMorale < 0)
			{
				nNeed = 0 - nCurMorale;
				if (nNeed > INPRISON_MORAL_INCREASE_VAL)
				{
					nNeed = INPRISON_MORAL_INCREASE_VAL;
				}
			}
			else
			{
				DWORD	dwPrisonMapID	= g_mapCreator.GetPrisonMapID();
				Vector3	PutOutPoint		= g_mapCreator.GetPutOutPrisonPoint();

				pRole->GotoNewMap(dwPrisonMapID, PutOutPoint.x, PutOutPoint.y, PutOutPoint.z);
			}

			ModAttValue(ERA_Morality, nNeed);
			return;
		}	

		// 非牢狱区域内，道德值每15分钟+1
		if( !pRole->IsInRoleState(ERS_PrisonArea) && --m_nOutPrisonMoraleRegainTickCountDown <= 0 )
		{
			//m_nOutPrisonMoraleRegainTickCountDown = /*15 **/ INPRISON_MORALE_REGAIN_INTER_TICK;
			m_nOutPrisonMoraleRegainTickCountDown = g_world.GetMoralityTime() * TICK_PER_SECOND;
			
			INT nCurMorale = pRole->GetAttValue(ERA_Morality);
			if (nCurMorale < 0)
			{
				//ModAttValue(ERA_Morality, 1);
				Buff* pBuff = GetBuffPtr(90034);
				if(P_VALID(pBuff))
				{
					if(3 == pBuff->GetLevel())
					{
						ModAttValue(ERA_Morality, 3*(g_world.GetReturnMorality()));
					}
					else
					{
						ModAttValue(ERA_Morality, g_world.GetReturnMorality());
					}
				}
				else
				{
					ModAttValue(ERA_Morality, g_world.GetReturnMorality());
				}
				//if(this->IsHaveBuff(90034)) // 特殊的一个buff，有它则道德值恢复量为原来的3倍
				//	ModAttValue(ERA_Morality, 3*(g_world.GetReturnMorality()));
				//else
				//	ModAttValue(ERA_Morality, g_world.GetReturnMorality());
			}
		}
	}
}

BOOL Unit::IsSameType(Unit* pDst)
{
	if (!P_VALID(pDst))
	{
		return FALSE;
	}

	DWORD dwID = pDst->GetID();
	if (IS_PLAYER(m_dwID) && IS_PLAYER(dwID))
	{
		return TRUE;
	}

	if (IS_CREATURE(m_dwID) && IS_CREATURE(dwID))
	{
		if ((((Creature*)this)->IsMonster() && ((Creature*)pDst)->IsMonster()) ||
			(((Creature*)this)->IsNPC() && ((Creature*)pDst)->IsNPC()))
		{
			return TRUE;
		}
	}

	return FALSE;
}

//--------------------------------------------------------------------------------
// 根据重算标志位进行当前属性重算
// F-Project 
// 2010-6-10 Jason 调整属性
//--------------------------------------------------------------------------------
VOID Unit::RecalAtt(BOOL bSendMsg)
{
	bool bAttARecal[X_ERA_ATTA_NUM] = {false};		// 一级属性是否进行了重算
	bool bAttBRecal[X_ERA_AttB_NUM] = {false};		// 二级属性是否进行了重算

	// 首先将某些“当前属性”提取出来
	INT nHP				=	m_nAtt[ERA_HP];
	INT nMP				=	m_nAtt[ERA_MP];
	INT nVitality		=	m_nAtt[ERA_Vitality];
	INT nEndurance		=	m_nAtt[ERA_Endurance];
	INT nKnowledge		=	m_nAtt[ERA_Knowledge];
	INT nInjury			=	m_nAtt[ERA_Injury];
	INT nMorale			=	m_nAtt[ERA_Morale];
	INT nMorality		=	m_nAtt[ERA_Morality];
	INT nCulture		=	m_nAtt[ERA_Culture];
	INT nAttPoint		=	m_nAtt[ERA_AttPoint];
	INT nTalentPoint	=	m_nAtt[ERA_TalentPoint];
	// 妖精元素伤害，以及抗性
	//INT nEEI_Fire		=	m_nAtt[ERA_EEI_Fire];
	//INT nEEI_Water		=	m_nAtt[ERA_EEI_Water];
	//INT nEEI_Earth		=	m_nAtt[ERA_EEI_Earth];
	//INT nEEI_Wind		=	m_nAtt[ERA_EEI_Wind];
	//INT nEEI_Bright		=	m_nAtt[ERA_EEI_Bright];
	//INT nEEI_Dark		=	m_nAtt[ERA_EEI_Dark];
	//INT nEER_Fire		=	m_nAtt[ERA_EER_Fire];
	//INT nEER_Water		=	m_nAtt[ERA_EER_Water];
	//INT nEER_Earth		=	m_nAtt[ERA_EER_Earth];
	//INT nEER_Wind		=	m_nAtt[ERA_EER_Wind];
	//INT nEER_Bright		=	m_nAtt[ERA_EER_Bright];
	//INT nEER_Dark		=	m_nAtt[ERA_EER_Dark];
	INT nMaxHP			=	m_nAtt[ERA_MaxHP];
	INT nMaxMP			=	m_nAtt[ERA_MaxMP];

	INT nGodCondence	=	m_nAtt[ERA_God_Condenced];
	INT nGodFaith		=	m_nAtt[ERA_God_Faith];
	INT nGodHead		=	m_nAtt[ERA_God_Godhead];

	// 开始计算
	for(INT n = 0; n < ERA_End; n++)
	{
		if( false == GetAttRecalFlag(n) )
			continue;

		// 该属性需要重新计算
		m_nAtt[n] = CalAttMod(m_nBaseAtt[n], n);

		if( n >= ERA_AttA_Start && n <= ERA_AttA_End )
		{
			bAttARecal[MTransERAAttA2Index(n)] = true;
		}
		else if( n >= ERA_AttB_Start && n < ERA_AttB_End )
		{
			bAttBRecal[MTransERAAttB2Index(n)] = true;
		}
	}


	// 如果需要重新计算属性的为人物，则需要判断一级属性和二级属性之间的关联
	if( IsRole() )
	{
		// 首先查看是否有一级属性进行了重新计算
		for(INT n = 0; n < X_ERA_ATTA_NUM; ++n)
		{
			if( !bAttARecal[n] ) continue;

				// 得到是哪个一级属性
			ERoleAttribute eType = (ERoleAttribute)MTransIndex2ERAATTA(n);

			switch(eType)
			{
				// 筋骨，则二级属性的最大体力和外功防御要重新计算
			case ERA_Physique:
				{
					// 计算最大体力
					m_nAtt[ERA_MaxHP] = m_nAtt[ERA_Physique] * 6 + m_nBaseAtt[ERA_MaxHP];
					m_nAtt[ERA_MaxHP] = CalAttMod(m_nAtt[ERA_MaxHP], ERA_MaxHP);
					if(m_nAbility[EFSA_HP] > 0)
					{
						m_nAtt[ERA_MaxHP] += m_nAbility[EFSA_HP];
					}

					// 计算外功防御
					m_nAtt[ERA_ExDefense] = m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
					m_nAtt[ERA_ExDefense] = CalAttMod(m_nAtt[ERA_ExDefense], ERA_ExDefense);
					if(m_nAbility[EFSA_EXDefense] > 0)
					{
						m_nAtt[ERA_ExDefense] += m_nAbility[EFSA_EXDefense];
					}

					// 计算内功防御
					m_nAtt[ERA_InDefense] = m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
					m_nAtt[ERA_InDefense] = CalAttMod(m_nAtt[ERA_InDefense], ERA_InDefense);
					if(m_nAbility[EFSA_InDefense] > 0)
					{
						m_nAtt[ERA_InDefense] += m_nAbility[EFSA_InDefense];
					}

					// 已经重新计算过的二级属性下面就不需要重新计算了
					bAttBRecal[MTransERAAttB2Index(ERA_MaxHP)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_ExDefense)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_InDefense)] = false;

					// 如果这些二级属性本来不需要重新计算，则设置上，以作为发消息的参考
					if( !m_bAttRecalFlag[ERA_MaxHP] ) m_bAttRecalFlag[ERA_MaxHP] = true;
					if( !m_bAttRecalFlag[ERA_ExDefense] ) m_bAttRecalFlag[ERA_ExDefense] = true;
					if( !m_bAttRecalFlag[ERA_InDefense] ) m_bAttRecalFlag[ERA_InDefense] = true;
				}
				break;

				// 劲力，则二级属性的外功攻击，外功防御和持久力需要重新计算
			case ERA_Strength:
				{
					// 计算外功攻击
					m_nAtt[ERA_ExAttack] = m_nAtt[ERA_Strength] * 10;
					m_nAtt[ERA_ExAttack] = CalAttMod(m_nAtt[ERA_ExAttack], ERA_ExAttack);
					if(m_nAbility[EFSA_EXAttack] > 0)
					{
						m_nAtt[ERA_ExAttack] += m_nAbility[EFSA_EXAttack];
					}

					// 计算外功防御
					m_nAtt[ERA_ExDefense] = m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
					m_nAtt[ERA_ExDefense] = CalAttMod(m_nAtt[ERA_ExDefense], ERA_ExDefense);
					if(m_nAbility[EFSA_EXDefense] > 0)
					{
						m_nAtt[ERA_ExDefense] += m_nAbility[EFSA_EXDefense];
					}

					// 计算内功防御
					m_nAtt[ERA_InDefense] = m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
					m_nAtt[ERA_InDefense] = CalAttMod(m_nAtt[ERA_InDefense], ERA_InDefense);
					if(m_nAbility[EFSA_InDefense] > 0)
					{
						m_nAtt[ERA_InDefense] += m_nAbility[EFSA_InDefense];
					}

					// 已经重新计算过的二级属性下面就不需要重新计算了
					bAttBRecal[MTransERAAttB2Index(ERA_ExAttack)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_ExDefense)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_InDefense)] = false;

					// 如果这些二级属性本来不需要重新计算，则设置上，以作为发消息的参考
					if( !m_bAttRecalFlag[ERA_ExAttack] ) m_bAttRecalFlag[ERA_ExAttack] = true;
					if( !m_bAttRecalFlag[ERA_ExDefense] ) m_bAttRecalFlag[ERA_ExDefense] = true;
					if( !m_bAttRecalFlag[ERA_InDefense] ) m_bAttRecalFlag[ERA_InDefense] = true;
			}
				break;

				// 元气，则二级属性的最大真气和内功防御需要重新计算
			case ERA_Pneuma:
				{
					// 计算最大体力
					m_nAtt[ERA_MaxMP] = m_nAtt[ERA_Pneuma] * 10 + m_nBaseAtt[ERA_MaxMP];
					m_nAtt[ERA_MaxMP] = CalAttMod(m_nAtt[ERA_MaxMP], ERA_MaxMP);

					// 计算内功防御
					m_nAtt[ERA_InDefense] = m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
					m_nAtt[ERA_InDefense] = CalAttMod(m_nAtt[ERA_InDefense], ERA_InDefense);
					if(m_nAbility[EFSA_InDefense] > 0)
					{
						m_nAtt[ERA_InDefense] += m_nAbility[EFSA_InDefense];
					}

					// 计算外功防御
					m_nAtt[ERA_ExDefense] = m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
					m_nAtt[ERA_ExDefense] = CalAttMod(m_nAtt[ERA_ExDefense], ERA_ExDefense);
					if(m_nAbility[EFSA_EXDefense] > 0)
					{
						m_nAtt[ERA_ExDefense] += m_nAbility[EFSA_EXDefense];
					}

					// 已经重新计算过的二级属性下面就不需要重新计算了
					bAttBRecal[MTransERAAttB2Index(ERA_MaxMP)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_InDefense)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_ExDefense)] = false;

					// 如果这些二级属性本来不需要重新计算，则设置上，以作为发消息的参考
					if( !m_bAttRecalFlag[ERA_MaxMP] ) m_bAttRecalFlag[ERA_MaxMP] = true;
					if( !m_bAttRecalFlag[ERA_InDefense] ) m_bAttRecalFlag[ERA_InDefense] = true;
					if( !m_bAttRecalFlag[ERA_ExDefense] ) m_bAttRecalFlag[ERA_ExDefense] = true;
				}
				break;

				// 内力，则二级属性的内功攻击，内功防御和持久力要重新计算
			case ERA_InnerForce:
				{
					// 计算内功攻击
					m_nAtt[ERA_InAttack] = m_nAtt[ERA_InnerForce] * 10;
					m_nAtt[ERA_InAttack] = CalAttMod(m_nAtt[ERA_InAttack], ERA_InAttack);
					if(m_nAbility[EFSA_InAttack] > 0)
					{
						m_nAtt[ERA_InAttack] += m_nAbility[EFSA_InAttack];
					}

					// 计算内功防御
					m_nAtt[ERA_InDefense] = m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
					m_nAtt[ERA_InDefense] = CalAttMod(m_nAtt[ERA_InDefense], ERA_InDefense);
					if(m_nAbility[EFSA_InDefense] > 0)
					{
						m_nAtt[ERA_InDefense] += m_nAbility[EFSA_InDefense];
					}
					// 计算外功防御
					m_nAtt[ERA_ExDefense] = m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
					m_nAtt[ERA_ExDefense] = CalAttMod(m_nAtt[ERA_ExDefense], ERA_ExDefense);
					if(m_nAbility[EFSA_EXDefense] > 0)
					{
						m_nAtt[ERA_ExDefense] += m_nAbility[EFSA_EXDefense];
					}

					// 已经重新计算过的二级属性下面就不需要重新计算了
					bAttBRecal[MTransERAAttB2Index(ERA_InAttack)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_InDefense)] = false;
					bAttBRecal[MTransERAAttB2Index(ERA_ExDefense)] = false;

					// 如果这些二级属性本来不需要重新计算，则设置上，以作为发消息的参考
					if( !m_bAttRecalFlag[ERA_InAttack] ) m_bAttRecalFlag[ERA_InAttack] = true;
					if( !m_bAttRecalFlag[ERA_InDefense] ) m_bAttRecalFlag[ERA_InDefense] = true;
					if( !m_bAttRecalFlag[ERA_ExDefense] ) m_bAttRecalFlag[ERA_ExDefense] = true;
				}
				break;

				// 技力，则二级属性中的攻击技巧和命中需要重新计算
			//case ERA_Technique:
			//	{
			//		// 计算攻击技巧
			//		//m_nAtt[ERA_AttackTec] = m_nAtt[ERA_Technique] * 6;
			//		//m_nAtt[ERA_AttackTec] = CalAttMod(m_nAtt[ERA_AttackTec], ERA_AttackTec);

			//		// 计算命中
			//		//m_nAtt[ERA_HitRate]	= m_nAtt[ERA_Technique] * 10;
			//		//m_nAtt[ERA_HitRate] = CalAttMod(m_nAtt[ERA_HitRate], ERA_HitRate);

			//		// 计算持久力
			//		//m_nAtt[ERA_MaxEndurance] = (m_nAtt[ERA_Technique] + m_nAtt[ERA_Agility]) / 10 + 298;
			//		//const tagLevelUpEffect* pLevelupEff = g_attRes.GetLevelUpEffect(GetLevel());
			//		//if( P_VALID(pLevelupEff) )
			//		//	m_nAtt[ERA_MaxEndurance]=	100 + pLevelupEff->n16Durance;
			//		//else
			//		//	m_nAtt[ERA_MaxEndurance]=	0;//(m_nAtt[ERA_Technique] + m_nAtt[ERA_Agility]) / 10 + 298;

			//		// 已经重新计算过的二级属性下面就不需要重新计算了
			//		//bAttBRecal[MTransERAAttB2Index(ERA_AttackTec)] = false;
			//		//bAttBRecal[MTransERAAttB2Index(ERA_HitRate)] = false;
			//		//bAttBRecal[MTransERAAttB2Index(ERA_MaxEndurance)] = false;

			//		// 如果这些二级属性本来不需要重新计算，则设置上，以作为发消息的参考
			//		//if( !m_bAttRecalFlag[ERA_AttackTec] ) m_bAttRecalFlag[ERA_AttackTec] = true;
			//		//if( !m_bAttRecalFlag[ERA_HitRate] ) m_bAttRecalFlag[ERA_HitRate] = true;
			//		//if( !m_bAttRecalFlag[ERA_MaxEndurance] ) m_bAttRecalFlag[ERA_MaxEndurance] = true;
			//	}
			//	break;

			//	// 身法，则二级属性中的防御技巧和闪避需要重新计算
			//case ERA_Agility:
			//	{
			//		// 计算防御技巧
			//		//m_nAtt[ERA_DefenseTec] = m_nAtt[ERA_Agility] * 4;
			//		//m_nAtt[ERA_DefenseTec] = CalAttMod(m_nAtt[ERA_DefenseTec], ERA_DefenseTec);

			//		// 计算闪避
			//		//m_nAtt[ERA_Dodge] =	m_nAtt[ERA_Agility] * 10;
			//		//m_nAtt[ERA_Dodge] = CalAttMod(m_nAtt[ERA_Dodge], ERA_Dodge);

			//		// 计算持久力
			//		//m_nAtt[ERA_MaxEndurance] = (m_nAtt[ERA_Technique] + m_nAtt[ERA_Agility]) / 10 + 298;
			//		//const tagLevelUpEffect* pLevelupEff = g_attRes.GetLevelUpEffect(GetLevel());
			//		//if( P_VALID(pLevelupEff) )
			//		//	m_nAtt[ERA_MaxEndurance]=	100 + pLevelupEff->n16Durance;
			//		//else
			//		//	m_nAtt[ERA_MaxEndurance]=	0;//(m_nAtt[ERA_Technique] + m_nAtt[ERA_Agility]) / 10 + 298;

			//		//// 已经重新计算过的二级属性下面就不需要重新计算了
			//		//bAttBRecal[MTransERAAttB2Index(ERA_DefenseTec)] = false;
			//		//bAttBRecal[MTransERAAttB2Index(ERA_Dodge)] = false;
			//		//bAttBRecal[MTransERAAttB2Index(ERA_MaxEndurance)] = false;

			//		//// 如果这些二级属性本来不需要重新计算，则设置上，以作为发消息的参考
			//		//if( !m_bAttRecalFlag[ERA_DefenseTec] ) m_bAttRecalFlag[ERA_DefenseTec] = true;
			//		//if( !m_bAttRecalFlag[ERA_Dodge] ) m_bAttRecalFlag[ERA_Dodge] = true;
			//		//if( !m_bAttRecalFlag[ERA_MaxEndurance] ) m_bAttRecalFlag[ERA_MaxEndurance] = true;
			//	}
			//	break;

			default:
				break;
			}
		}

		// 再判断计算过后是否还有二级属性需要计算
		for(INT n = 0; n < X_ERA_AttB_NUM; ++n)
		{
			if( !bAttBRecal[n] ) continue;

			// 得到是哪个二级属性
			ERoleAttribute eType = (ERoleAttribute)MTransIndex2ERAATTB(n);

			switch(eType)
			{
				// 最大体力
			case ERA_MaxHP:
				{
					// 计算最大体力
					m_nAtt[ERA_MaxHP] = m_nAtt[ERA_Physique] * 6 + m_nBaseAtt[ERA_MaxHP];
					m_nAtt[ERA_MaxHP] = CalAttMod(m_nAtt[ERA_MaxHP], ERA_MaxHP);
					if(m_nAbility[EFSA_HP] > 0)
					{
						m_nAtt[ERA_MaxHP] += m_nAbility[EFSA_HP];
					}
				}
				break;

				// 最大真气
			case ERA_MaxMP:
				{
					// 计算最大体力
					m_nAtt[ERA_MaxMP] = m_nAtt[ERA_Pneuma] * 10 + m_nBaseAtt[ERA_MaxMP];
					m_nAtt[ERA_MaxMP] = CalAttMod(m_nAtt[ERA_MaxMP], ERA_MaxMP);
				}
				break;

				// 外功攻击
			case ERA_ExAttack:
				{
					// 计算外功攻击
					m_nAtt[ERA_ExAttack] = m_nAtt[ERA_Strength] * 10;
					m_nAtt[ERA_ExAttack] = CalAttMod(m_nAtt[ERA_ExAttack], ERA_ExAttack);
					if(m_nAbility[EFSA_EXAttack] > 0)
					{
						m_nAtt[ERA_ExAttack] += m_nAbility[EFSA_EXAttack];
					}
				}
				break;

				// 内功防御
			case ERA_InDefense:
				// 外功防御
			case ERA_ExDefense:
				{
					// 计算外功防御
					m_nAtt[ERA_ExDefense] = m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
					m_nAtt[ERA_ExDefense] = CalAttMod(m_nAtt[ERA_ExDefense], ERA_ExDefense);
					if(m_nAbility[EFSA_EXDefense] > 0)
					{
						m_nAtt[ERA_ExDefense] += m_nAbility[EFSA_EXDefense];
					}

					// 计算内功防御
					m_nAtt[ERA_InDefense] = m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
					m_nAtt[ERA_InDefense] = CalAttMod(m_nAtt[ERA_InDefense], ERA_InDefense);
					if(m_nAbility[EFSA_InDefense] > 0)
					{
						m_nAtt[ERA_InDefense] += m_nAbility[EFSA_InDefense];
					}

				}
				break;

				// 内功攻击
			case ERA_InAttack:
				{
					// 计算内功攻击
					m_nAtt[ERA_InAttack] = m_nAtt[ERA_InnerForce] * 10;
					m_nAtt[ERA_InAttack] = CalAttMod(m_nAtt[ERA_InAttack], ERA_InAttack);
					if(m_nAbility[EFSA_InAttack] > 0)
					{
						m_nAtt[ERA_InAttack] += m_nAbility[EFSA_InAttack];
					}
				}
				break;

				// 内功防御
			//case ERA_InDefense:
			//	{
			//		// 计算内功防御
			//		m_nAtt[ERA_InDefense] = m_nAtt[ERA_Physique] * 6 + m_nAtt[ERA_Pneuma] * 4;
			//		m_nAtt[ERA_InDefense] = CalAttMod(m_nAtt[ERA_InDefense], ERA_InDefense);
			//	}
			//	break;

				// 攻击技巧
			case ERA_AttackTec:
				{
					// 计算攻击技巧
					//m_nAtt[ERA_AttackTec] = m_nAtt[ERA_Technique] * 6;
					//m_nAtt[ERA_AttackTec] =CalAttMod(m_nAtt[ERA_AttackTec], ERA_AttackTec);
				}
				break;

				// 防御技巧
			case ERA_DefenseTec:
				{
					// 计算防御技巧
					//m_nAtt[ERA_DefenseTec] = m_nAtt[ERA_Agility] * 4;
					//m_nAtt[ERA_DefenseTec] = CalAttMod(m_nAtt[ERA_DefenseTec], ERA_DefenseTec);
				}
				break;

				// 命中
			//case ERA_HitRate:
			//	{
			//		// 计算命中
			//		//m_nAtt[ERA_HitRate]	= m_nAtt[ERA_Technique] * 10;
			//		m_nAtt[ERA_HitRate] = 0;//CalAttMod(m_nAtt[ERA_HitRate], ERA_HitRate);
			//	}
			//	break;

			//	// 闪避
			//case ERA_Dodge:
			//	{
			//		// 计算闪避
			//		//m_nAtt[ERA_Dodge] =	m_nAtt[ERA_Agility] * 10;
			//		m_nAtt[ERA_Dodge] = 0;//CalAttMod(m_nAtt[ERA_Dodge], ERA_Dodge);
			//	}
			//	break;

				// 持久力
			case ERA_MaxEndurance:
				{
					// 计算持久力
					//m_nAtt[ERA_MaxEndurance] = GetLevel() * 10;
					const tagLevelUpEffect* pLevelupEff = g_attRes.GetLevelUpEffect(GetLevel());
					if( P_VALID(pLevelupEff) )
						m_nAtt[ERA_MaxEndurance]=	100 + pLevelupEff->n16Durance;
					else
						m_nAtt[ERA_MaxEndurance]=	0;//(m_nAtt[ERA_Technique] + m_nAtt[ERA_Agility]) / 10 + 298;
				}
				break;

			case ERA_Endurance:
				// 考虑到回持久力的问题，这里要这样才算合理，但是也无法完全弥补由于速度增加而损失的(会少计算)
				nEndurance		=	max(nEndurance,m_nAtt[ERA_Endurance]);
				break;

			default:
				break;
			}
		}
	}

	for(INT n = 0; n < ERA_End; n++)
	{
		if( !GetAttRecalFlag(n) )
			continue;

		switch(n)
		{
		case ERA_transform_ExAttack:
		{
			if(m_nAbility[EFSA_EXAttackDeeper] >= 0)
			{
				m_nAtt[ERA_transform_ExAttack] += (m_nAbility[EFSA_EXAttackDeeper]);
			}
		}
		break;

		case ERA_transform_InAttack:
			{
				if(m_nAbility[EFSA_InAttackDeeper] >= 0)
				{
					m_nAtt[ERA_transform_InAttack] += (m_nAbility[EFSA_InAttackDeeper]);
				}
			}
			break;

		case ERA_Derate_ExAttack:
			{
				if(m_nAbility[EFSA_EXAttackResistance] >= 0)
				{
					m_nAtt[ERA_Derate_ExAttack] += (m_nAbility[EFSA_EXAttackResistance]);
				}
			}
			break;

		case ERA_Derate_InAttack:
			{
				if(m_nAbility[EFSA_InAttackResistance] >= 0)
				{
					m_nAtt[ERA_Derate_InAttack] += (m_nAbility[EFSA_InAttackResistance]);
				}
			}
			break;

		case ERA_Toughness_god:
			{
				if(m_nAbility[EFSA_Toughness] >= 0)
				{
					m_nAtt[ERA_Toughness_god] += (m_nAbility[EFSA_Toughness]);
				}
			}
			break;

		case ERA_Toughness_strength:
			{
				if(m_nAbility[EFSA_CritDes] >= 0)
				{
					m_nAtt[ERA_Toughness_strength] += (m_nAbility[EFSA_CritDes]);
				}
			}
			break;

		case ERA_Contrel_effect_deepen:
			{
				if(m_nAbility[ EFSA_ControleffectDeepen] >= 0)
				{
					m_nAtt[ERA_Contrel_effect_deepen] += (m_nAbility[EFSA_ControleffectDeepen]);
				}
			}
			break;

		case ERA_Resist_Choas:
			{
				if(m_nAbility[ EFSA_ControleffectResistance] >= 0)
				{
					m_nAtt[ERA_Resist_Choas] += (m_nAbility[EFSA_ControleffectResistance]);
				}
			}
			break;

		case ERA_Slowing_effect_strengthening:
			{
				if(m_nAbility[ EFSA_SlowingeffectDeepen] >= 0)
				{
					m_nAtt[ERA_Slowing_effect_strengthening] += (m_nAbility[EFSA_SlowingeffectDeepen]);
				}
			}
			break;

		case ERA_Resist_Special:
			{
				if(m_nAbility[ EFSA_SlowingeffectResistance] >= 0)
				{
					m_nAtt[ERA_Resist_Special] += (m_nAbility[EFSA_SlowingeffectResistance]);
				}
			}
			break;

		case ERA_Fixed_damage_aggrandizement:
			{
				if(m_nAbility[ EFSA_FixedeffectDeepen] >= 0)
				{
					m_nAtt[ERA_Fixed_damage_aggrandizement] += (m_nAbility[EFSA_FixedeffectDeepen]);
				}
			}
			break;

		case ERA_Resist_Bleeding:
			{
				if(m_nAbility[ EFSA_FixedeffectResistance] >= 0)
				{
					m_nAtt[ERA_Resist_Bleeding] += (m_nAbility[EFSA_FixedeffectResistance]);
				}
			}
			break;

		case ERA_Aging_effect_strengthening:
			{
				if(m_nAbility[ EFSA_AgingeffectDeepen] >= 0)
				{
					m_nAtt[ERA_Aging_effect_strengthening] += (m_nAbility[EFSA_AgingeffectDeepen]);
				}
			}
			break;

		case ERA_Resist_Weak:
			{
				if(m_nAbility[ EFSA_AgingeffectResistance] >= 0)
				{
					m_nAtt[ERA_Resist_Weak] += (m_nAbility[EFSA_AgingeffectResistance]);
				}
			}
			break;
		default:
			break;
		}
	}

	//加上家族妖精的影响
	//for(int i = 0; i < EFSA_Num; ++i)
	//{
	//	if(m_nAbility[i] >= 0 && m_bRecalAbility[i] )
	//	{
	//		ERoleAttribute eType = GetRoleIndexFromFamilySprite((EFamilySpriteAtt)i);
	//		if(eType > ERA_Null && eType < ERA_End)
	//		{
	//			m_nAtt[eType] += m_nAbility[i];
	//			m_bRecalAbility[i] = FALSE;
	//		}
	//	}
	//}

	// 附上原先保存的属性
	m_nAtt[ERA_Knowledge]	=	nKnowledge;
	m_nAtt[ERA_Injury]		=	nInjury;
	m_nAtt[ERA_Morale]		=	nMorale;
	m_nAtt[ERA_Morality]	=	nMorality;
	m_nAtt[ERA_Culture]		=	nCulture;
	m_nAtt[ERA_AttPoint]	=	nAttPoint;
	m_nAtt[ERA_TalentPoint]	=	nTalentPoint;

	// 估算血量魔力变化
	INT nOldHP = nHP;
	INT nOldMP = nMP;
	//nHP += m_nAtt[ERA_MaxHP] - nMaxHP;
	//nMP += m_nAtt[ERA_MaxMP] - nMaxMP;
	// 将之前保存的某些“当前属性”与对应的“最大值”取最小值
	m_nAtt[ERA_HP] = min(nHP, m_nAtt[ERA_MaxHP]);
	m_nAtt[ERA_MP] = min(nMP, m_nAtt[ERA_MaxMP]);
	m_nAtt[ERA_Vitality] = min(nVitality, m_nAtt[ERA_MaxVitality]);
	m_nAtt[ERA_Endurance] = min(nEndurance, m_nAtt[ERA_MaxEndurance]);
	if( nOldMP != nMP )
		m_bAttRecalFlag[ERA_MP] = true;
	if( nOldHP != nHP )
		m_bAttRecalFlag[ERA_HP] = true;
	// 妖精元素伤害抗性恢复
	//m_nAtt[ERA_EEI_Fire]		=	nEEI_Fire		;
	//m_nAtt[ERA_EEI_Water]		=	nEEI_Water	;
	//m_nAtt[ERA_EEI_Earth]		=	nEEI_Earth	;
	//m_nAtt[ERA_EEI_Wind]		=	nEEI_Wind		;
	//m_nAtt[ERA_EEI_Bright]	=	nEEI_Bright	;
	//m_nAtt[ERA_EEI_Dark]		=	nEEI_Dark		;
	//m_nAtt[ERA_EER_Fire]		=	nEER_Fire		;
	//m_nAtt[ERA_EER_Water]		=	nEER_Water	;
	//m_nAtt[ERA_EER_Earth]		=	nEER_Earth	;
	//m_nAtt[ERA_EER_Wind]		=	nEER_Wind		;
	//m_nAtt[ERA_EER_Bright]	=	nEER_Bright	;
	//m_nAtt[ERA_EER_Dark]		=	nEER_Dark		;


	// 比较原来的HP和新的HP之间是否一致，如果不一致，且血原来不需要重新计算，则设置需要重新计算，以便发消息
	if( m_nAtt[ERA_HP] != nHP && !m_bAttRecalFlag[ERA_HP] ) m_bAttRecalFlag[ERA_HP] = true;
	if( m_nAtt[ERA_MP] != nMP && !m_bAttRecalFlag[ERA_MP] ) m_bAttRecalFlag[ERA_MP] = true;
	if( m_nAtt[ERA_Vitality] != nVitality && !m_bAttRecalFlag[ERA_Vitality] ) m_bAttRecalFlag[ERA_Vitality] = true;
	if( m_nAtt[ERA_Endurance] != nEndurance && !m_bAttRecalFlag[ERA_Endurance] ) m_bAttRecalFlag[ERA_Endurance] = true;

	m_nAtt[ERA_God_Condenced]		= nGodCondence;
	m_nAtt[ERA_God_Faith]			= nGodFaith;
	m_nAtt[ERA_God_Godhead]		= nGodHead;


	// 如果需要发送消息
	if( bSendMsg )
	{
		// 首先查看是否需要广播（远程玩家需要知道的一些属性）
		BOOL bNeedBroadcast = FALSE;			// 是否需要广播
		bool bRemoteRoleAtt[ERRA_End] = {false};// 远程玩家属性.
		INT nRemoteRoleAtt[ERRA_End] = {0};		// 远程玩家属性
		INT nRoleAttNum = 0;					// 本地玩家属性改变的数量
		INT nRemoteRoleAttNum = 0;				// 远程玩家属性改变的数量

		// 计算以上参数
		for(INT n = 0; n < ERA_End; ++n)
		{
			if( m_bAttRecalFlag[n] )
			{
				++nRoleAttNum;
				ERemoteRoleAtt eType = ERA2ERRA((ERoleAttribute)n);
				if( eType != ERRA_Null )
				{
					++nRemoteRoleAttNum;
					bRemoteRoleAtt[eType] = true;
					nRemoteRoleAtt[eType] = m_nAtt[n];

					if( !bNeedBroadcast ) bNeedBroadcast = TRUE;
				}
			}
		}

		// 首先查看是否需要广播
		if( bNeedBroadcast && nRemoteRoleAttNum > 0 )
		{
			DWORD dwSize = sizeof(tagNS_RemoteAttChangeMutiple) + sizeof(tagRemoteAttValue) * (nRemoteRoleAttNum - 1);
			MCREATE_MSG(pSend, dwSize, NS_RemoteAttChangeMutiple);

			pSend->dwSize = dwSize;
			pSend->dwRoleID = m_dwID;
			pSend->nNum = nRemoteRoleAttNum;

			INT nIndex = 0;
			for(INT n = 0; n < ERRA_End; ++n)
			{
				if( bRemoteRoleAtt[n] )
				{
					pSend->value[nIndex].eType = (ERemoteRoleAtt)n;
					pSend->value[nIndex].nValue = nRemoteRoleAtt[n];
					++nIndex;
				}
			}

			if( P_VALID(GetMap()) )
				GetMap()->SendBigVisTileMsg(this, pSend, pSend->dwSize);

			MDEL_MSG(pSend);
		}

		// 如果是人物并且需要发送，则发送给本地玩家
		if( IsRole() && nRoleAttNum > 0 )
		{
			Role* pRole = reinterpret_cast<Role*>(this);

			DWORD dwSize = sizeof(tagNS_RoleAttChangeMutiple) + sizeof(tagRoleAttValue) * (nRoleAttNum - 1);
			MCREATE_MSG(pSend, dwSize, NS_RoleAttChangeMutiple);

			pSend->dwSize = dwSize;
			pSend->nNum = nRoleAttNum;

			INT nIndex = 0;
			for(INT n = 0; n < ERA_End; ++n)
			{
				if( GetAttRecalFlag(n) )
				{
					pSend->value[nIndex].eType = (ERoleAttribute)n;
					pSend->value[nIndex].nValue = m_nAtt[n];
					++nIndex;
				}
			}

			pRole->SendMessage(pSend, pSend->dwSize);

			MDEL_MSG(pSend);
		}
	}

	// 设置某些因为属性改变而影响的值
	OnAttChange(m_bAttRecalFlag);

	// 清空重算标志位
	ClearAttRecalFlag();
}

//--------------------------------------------------------------------------------
// 计算属性加成最终值，并取上下限
//--------------------------------------------------------------------------------
INT Unit::CalAttMod(INT nBase, INT nIndex)
{
	// 检查nIndex是否合法
	ASSERT( nIndex > ERRA_Null && nIndex < ERA_End );
	if( nIndex < 0 || nIndex >= ERA_End )
		return 0;

	INT nValue = nBase + m_nAttMod[nIndex] + INT((FLOAT)nBase * (FLOAT(m_nAttModPct[nIndex]) / 10000.0f));

	if( nValue < g_attRes.GetAttMin(nIndex) ) nValue = g_attRes.GetAttMin(nIndex);
	if( nValue > g_attRes.GetAttMax(nIndex) ) nValue = g_attRes.GetAttMax(nIndex);

	return nValue;
}

//--------------------------------------------------------------------------------
// 发送某个属性变化给客户端，如果该属性变化需要广播则广播
//--------------------------------------------------------------------------------
VOID Unit::SendAttChange(INT nIndex)
{
	ASSERT( nIndex > ERA_Null && nIndex < ERA_End );
	if( nIndex < 0 || nIndex >= ERA_End )
		return;

	ERemoteRoleAtt eRemote = ERA2ERRA((ERoleAttribute)nIndex);

	// 生物
	if( IsCreature() )
	{
		if( ERRA_Null != eRemote )
		{
			// 发送远程同步命令
			tagNS_RemoteAttChangeSingle send;
			send.dwRoleID = GetID();
			send.eType = eRemote;
			send.nValue = m_nAtt[nIndex];

			if( GetMap() )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}
	// 玩家
	else if( IsRole() )
	{
		// 首先发送给自己
		Role* pRole = reinterpret_cast<Role*>(this);

		tagNS_RoleAttChangeSingle send;
		send.eType = (ERoleAttribute)nIndex;
		send.nValue = m_nAtt[nIndex];
		pRole->SendMessage(&send, send.dwSize);

		// 如果需要同步给远程玩家
		if( ERRA_Null != eRemote )
		{
			// 发送远程同步命令
			tagNS_RemoteAttChangeSingle send;
			send.dwRoleID = GetID();
			send.eType = eRemote;
			send.nValue = m_nAtt[nIndex];

			if( GetMap() )
			{
				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
			
			// 同步属性给小队玩家
			if(pRole->GetTeamID() != GT_INVALID && IsTeamRemoteAtt(eRemote))
				g_groupMgr.SendTeamMesOutBigVis(pRole, pRole->GetTeamID(), &send, send.dwSize);

			if ( g_AthleticsSystem.GetPvPState() == 1 )
			{

				if (  ERRA_MaxHP == eRemote || ERRA_HP == eRemote)
				{
					RoomBase* pRoom = g_AthleticsSystem.GetRoomMgr().GetPvPRoom(pRole->GetID(),E_PvP_Junior,0);
					if ( P_VALID(pRoom) )
					{
						Athletics3V3Room* p3V3Room = (Athletics3V3Room*)pRoom;
						p3V3Room->SendRemoteAttToPvPRole(pRole->GetID());
					}
				}
			}
		}
	}
}

// 设置阵营 [4/5/2012 zhangzhihua]
VOID Unit::SetCampType(ECampType eType)
{
	if (eType < ECamp_Null || eType >= ECamp_End)
	{
		return;
	}
	m_eCampType = eType;

	if (IsRole())
	{
		tagNS_SyncCamp msg;
		msg.dwRoleID = GetID();
		msg.eCamp = m_eCampType;
		((Role*)this)->SendMessage(&msg, msg.dwSize, EMR_Around);
	}
}

// 获得阵营 [4/5/2012 zhangzhihua]
ECampType Unit::GetCampType()
{
	if (ECamp_End == m_eCampType)
	{
		return ECamp_Null;
	}
	return m_eCampType;
}

// 是否有阵营 [4/5/2012 zhangzhihua]
bool Unit::IsInCamp()
{
	return ECamp_Null != m_eCampType;
}

VOID Unit::SetHostileCamp(ECampType eType)
{
	m_byHostileCamp |= (0x01f << eType);
}

BOOL Unit::GetHostileCamp(ECampType eType)
{
	if (m_byHostileCamp & (0x01f << eType))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------------------
// 得到自身状态标志位
//-----------------------------------------------------------------------------------------
DWORD Unit::GetStateFlag()
{
	DWORD dwStateFlag = 0;

	dwStateFlag |=	( IsInState(ES_Dead)		?	ESF_Dead		:	ESF_NoDead );
	dwStateFlag |=	( IsInState(ES_Dizzy)		?	ESF_Dizzy		:	ESF_NoDizzy );
	dwStateFlag |=	( IsInState(ES_Spor)		?	ESF_Spor		:	ESF_NoSpor );
	dwStateFlag |=	( IsInState(ES_Tie)			?	ESF_Tie			:	ESF_NoTie );
	dwStateFlag |=	( IsInState(ES_Invincible)	?	ESF_Invincible	:	ESF_NoInvincible );
	dwStateFlag |=	( IsInState(ES_Lurk)		?	ESF_Lurk		:	ESF_NoLurk );
	dwStateFlag |=	( IsInState(ES_DisArm)		?	ESF_DisArm		:	ESF_NoDisArm );
	dwStateFlag |=	( IsInState(ES_NoSkill)		?	ESF_NoSkill		:	ESF_NoNoSkill );

	return dwStateFlag;
}

// 阵营敌我关系 [4/11/2012 zhangzhihua]
DWORD Unit::FriendEnemyOfCamp(Unit* pTarget)
{
	DWORD dwSelfFlag = 0;
	if (IsInCamp() && pTarget->IsInCamp())
	{
		// 阵营优先 [4/10/2012 zhangzhihua]
		if (GetCampType() == pTarget->GetCampType())
		{
			// 同阵营规则 [4/10/2012 zhangzhihua]
			if (TRUE == IsSameType(pTarget))
			{
				// 类型相同则为友方 [4/10/2012 zhangzhihua]
				dwSelfFlag |= ETFE_Friendly;
			}
			else
			{
				// 类型相同则为中立 [4/10/2012 zhangzhihua]
				dwSelfFlag |= ETFE_Independent;
			}
		}
		else
		{
			// 不同阵营规则 [4/10/2012 zhangzhihua]
			if (ECamp_Neutrality != GetCampType() &&
				ECamp_Neutrality != pTarget->GetCampType())
			{
				dwSelfFlag |= ETFE_Hostile;
			}

			// 被攻击后：敌方 未被攻击：中立 [4/11/2012 zhangzhihua]
			if (ECamp_Neutrality == GetCampType())
			{
				if (TRUE == GetHostileCamp(pTarget->GetCampType()))
				{
					dwSelfFlag |= ETFE_Hostile;
				}
				else
				{
					dwSelfFlag |= ETFE_Independent;
				}
			}
			// 攻击前：怪物和NPC：中立 玩家：敌方 攻击后：全部敌方 [4/11/2012 zhangzhihua]
			else if (ECamp_Neutrality == pTarget->GetCampType())
			{
				if (TRUE == pTarget->GetHostileCamp(GetCampType()))
				{
					dwSelfFlag |= ETFE_Hostile;
				}
				else
				{
					if (TRUE == IsRole())
					{
						dwSelfFlag |= ETFE_Hostile;
					}
					else if (TRUE == IsCreature())
					{
						dwSelfFlag |= ETFE_Independent;
					}
				}
			}
		}

		// 安全区
		if(pTarget->IsRole() &&
			(((Role*)this)->IsInRoleState(ERS_SafeArea) ||
			(static_cast<Role*>(pTarget))->IsInRoleState(ERS_SafeArea)))
		{
			if(dwSelfFlag & ETFE_Hostile)
			{
				dwSelfFlag ^= ETFE_Hostile;
			}
		}
	}

	return dwSelfFlag;
}


//-----------------------------------------------------------------------------
// 计算某个技能的影响
//-----------------------------------------------------------------------------
VOID Unit::AddSkillMod(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;

	ESkillTargetType eTarget = pSkill->GetTargetType();

	switch(eTarget)
	{
		// 影响技能
	case ESTT_Skill:
		{
			// 影响技能
			DWORD dwTargetSkillID = pSkill->GetTargetSkillID();
			if (dwTargetSkillID > 0 && dwTargetSkillID < 10000)
			{
				// 技能盒子
				const tagSkillBoxProto * pSkillBoxProto = g_attRes.GetOneSkillProto(dwTargetSkillID);
				if( !P_VALID(pSkillBoxProto) )
					break;
				for (int nCnt = 0; nCnt < tagSkillBoxProto::E_SBP_Size; nCnt++)
				{
					Skill* pTargetSkill = m_mapSkill.Peek(pSkillBoxProto->dwSkillIDs[nCnt]);
					if( !P_VALID(pTargetSkill) ) return;

					if( pTargetSkill->SetMod(pSkill->GetProto()) && IsRole() )
					{
						Role* pRole = reinterpret_cast<Role*>(this);

						tagNS_UpdateSkill send;
						pTargetSkill->GenerateMsgInfo(&send.Skill);
						pRole->SendMessage(&send, send.dwSize);
					}
				}
			}
			else
			{
				Skill* pTargetSkill = m_mapSkill.Peek(dwTargetSkillID);
				if( !P_VALID(pTargetSkill) ) return;

				if( pTargetSkill->SetMod(pSkill->GetProto()) && IsRole() )
				{
					Role* pRole = reinterpret_cast<Role*>(this);

					tagNS_UpdateSkill send;
					pTargetSkill->GenerateMsgInfo(&send.Skill);
					pRole->SendMessage(&send, send.dwSize);
				}
			}
		}
		break;

		// 影响Buff
	case ESTT_Buff:
		{
			RegisterBuffModifier(pSkill);
		}
		break;

		// 影响Trigger
	case ESTT_Trigger:
		{
			RegisterTriggerModifier(pSkill);
		}
		break;

	default:
		break;
	}
}

//------------------------------------------------------------------------------
// 计算当前技能列表中的所有技能对某个技能的影响
//------------------------------------------------------------------------------
VOID Unit::AddSkillBeMod(Skill* pSkill)
{
	// 查看当前技能列表中是否存在能影响该技能的技能
	tagSkillModify* pModify = g_attRes.GetSkillModifier(pSkill->GetID());

	if( !P_VALID(pModify) || pModify->listModify.Empty() )
		return;

	// 通过列表查找技能列表中有无影响技能
	TList<DWORD>& list = pModify->listModify;

	TList<DWORD>::TListIterator it = list.Begin();

	Skill* pModSkill = NULL;
	DWORD dwModSkillID = GT_INVALID;
	while( list.PeekNext(it, dwModSkillID) )
	{
		pModSkill = m_mapSkill.Peek(dwModSkillID);
		if( !P_VALID(pModSkill) ) continue;

		// 发现一个技能，计算影响
		pSkill->SetMod(pModSkill->GetProto());
	}
}

//-----------------------------------------------------------------------------
// 去掉某个技能的影响
//-----------------------------------------------------------------------------
VOID Unit::RemoveSkillMod(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;

	ESkillTargetType eTarget = pSkill->GetTargetType();

	switch(eTarget)
	{
		// 影响技能
	case ESTT_Skill:
		{
			DWORD dwTargetSkillID = pSkill->GetTargetSkillID();
			if (dwTargetSkillID > 0 && dwTargetSkillID < 10000)
			{
				// 技能盒子
				const tagSkillBoxProto * pSkillBoxProto = g_attRes.GetOneSkillProto(dwTargetSkillID);
				if( !P_VALID(pSkillBoxProto) )
					break;
				for (int nCnt = 0; nCnt < tagSkillBoxProto::E_SBP_Size; nCnt++)
				{
					Skill* pTargetSkill = m_mapSkill.Peek(pSkillBoxProto->dwSkillIDs[nCnt]);
					if( !P_VALID(pTargetSkill) ) return;

					if( pTargetSkill->UnSetMod(pSkill->GetProto()) && IsRole() )
					{
						Role* pRole = reinterpret_cast<Role*>(this);

						tagNS_UpdateSkill send;
						pTargetSkill->GenerateMsgInfo(&send.Skill);
						pRole->SendMessage(&send, send.dwSize);
					}
				}
			}
			else
			{
				Skill* pTargetSkill = m_mapSkill.Peek(dwTargetSkillID);
				if( !P_VALID(pTargetSkill) ) return;

				if( pTargetSkill->UnSetMod(pSkill->GetProto()) && IsRole() )
				{
					Role* pRole = reinterpret_cast<Role*>(this);

					tagNS_UpdateSkill send;
					pTargetSkill->GenerateMsgInfo(&send.Skill);
					pRole->SendMessage(&send, send.dwSize);
				}
			}
		}
		break;

		// 影响Buff
	case ESTT_Buff:
		{
			UnRegisterBuffModifier(pSkill);
		}
		break;

		// 影响Trigger
	case ESTT_Trigger:
		{
			UnRegisterTriggerModifier(pSkill);
		}
		break;

	default:
		break;
	}
}

//----------------------------------------------------------------------------------
// 增加一个技能
//----------------------------------------------------------------------------------
VOID Unit::AddSkill(Skill* pSkill, BOOL bSendMsg)
{
	ASSERT( P_VALID(pSkill) );

	AddSkillMod(pSkill);
	AddSkillBeMod(pSkill);

	pSkill->SetOwner(this);
	m_mapSkill.Add(pSkill->GetID(), pSkill);
	if( pSkill->GetCoolDownCountDown() > 0 )
	{
		m_listSkillCoolDown.PushBack(pSkill->GetID());
	}

	pSkill->SetOwnerMod();

	// 如果是生产技能，则自身所有的生产技能个数加1
	if( ESSTE_Produce == pSkill->GetTypeEx() )
	{
		++m_nProduceSkillNum;
	}
}

//----------------------------------------------------------------------------------
// 去掉一个技能
//----------------------------------------------------------------------------------
VOID Unit::RemoveSkill(DWORD dwSkillID,BOOL bCalMod/*=TRUE*/)
{
	Skill* pSkill = m_mapSkill.Peek(dwSkillID);
	if( !P_VALID(pSkill) ) return;

	// 如果是生产技能，则自身所有的生产技能个数减1
	if( ESSTE_Produce == pSkill->GetTypeEx() )
	{
		--m_nProduceSkillNum;
	}

	// 清掉自身由该技能产生的所有buff
	if(bCalMod)
		RemoveAllBuffBelongSkill(pSkill->GetID(), FALSE);

	// 撤销影响
	if(bCalMod)
		pSkill->UnsetOwnerMod();

	m_mapSkill.Erase(dwSkillID);
	m_listSkillCoolDown.Erase(dwSkillID);
	m_listSkillActiveTime.Erase(dwSkillID);

	pSkill->SetOwner(NULL);

	if(bCalMod)
		RemoveSkillMod(pSkill);	

	SAFE_DEL(pSkill);
}

//----------------------------------------------------------------------------------
// 当技能升级或降级
//----------------------------------------------------------------------------------
VOID Unit::ChangeSkillLevel(Skill* pSkill, ESkillLevelChange eType, INT nChange)
{
	if( !P_VALID(pSkill) ) return;
	if( 0 == nChange ) return;

	// 首先先去掉人物属性影响，并去掉对其它技能影响
	pSkill->UnsetOwnerMod();
	RemoveSkillMod(pSkill);

	// 改变技能等级
	if( nChange > 0 )
		pSkill->IncLevel(eType, nChange);
	else
		pSkill->DecLevel(eType, nChange);

	// 重新计算属性影响
	AddSkillMod(pSkill);
	pSkill->SetOwnerMod();
}

//----------------------------------------------------------------------------------
// 改变技能熟练度
//----------------------------------------------------------------------------------
VOID Unit::ChangeSkillExp(Skill *pSkill, INT nValue)
{
	if( !P_VALID(pSkill) )	return;
	if( nValue <= 0 )	return;
	if( ESLUT_Exp != pSkill->GetLevelUpType() ) return;
	if( pSkill->GetLevel() == pSkill->GetMaxLevel() ) return;
	
	// 防沉迷
	if ( IsRole() )
	{
		Role* pRole = static_cast<Role*>(this);
		FLOAT fEarnRate = pRole->GetEarnRate() / 10000.0f;
		nValue = INT(nValue * fEarnRate);
	}

	if( nValue <= 0 ) return;

	// 是否升级了
	BOOL bLevelChanged = FALSE;

	// 得到该技能的升级经验
	INT nRemainProficency = pSkill->GetLevelUpExp() - pSkill->GetProficiency();

	// 如果该经验小于升级经验
	if( nValue < nRemainProficency )
	{
		pSkill->AddProficiency(nValue);
	}
	// 如果该经验大于等于升级经验
	else
	{
		INT nLastVal = pSkill->GetLevel();
		// 查看需要升几级
		while( nValue >= nRemainProficency )
		{
			// 已经到了最高等级
			if( pSkill->GetLevel() >= pSkill->GetMaxLevel() )
				break;

			nValue -= nRemainProficency;
			ChangeSkillLevel(pSkill, ESLC_Self);
			if( !bLevelChanged ) bLevelChanged = TRUE;

			// 得到下一级的升级经验
			nRemainProficency = pSkill->GetLevelUpExp();
		}

		// 查看如何来累加经验
		if( nValue >= nRemainProficency )
		{
			pSkill->SetProficiency(nRemainProficency);
		}
		else
		{
			pSkill->SetProficiency(nValue);
		}
		const SkillScript * pSkillScript = pSkill->GetSkillScript();
		if( P_VALID(pSkillScript) )
		{
			pSkillScript->OnSkillChange(GetMap(),GetID(),pSkill->GetID(),nLastVal,pSkill->GetLevel());
		}

		if (IsRole())
		{
			const RoleScript * pRoleScript = g_ScriptMgr.GetRoleScript();
			Role *pRole = dynamic_cast<Role *>(this);
			if (P_VALID(pRoleScript) && P_VALID(pSkill))
			{
				DWORD dwSkillIDWithLevel = pSkill->GetID()*100+pSkill->GetLevel();
				pRoleScript->OnLearnSkill(pRole, dwSkillIDWithLevel);
			}
		}
	}

	// 称号事件触发
	Role *pRole = dynamic_cast<Role *>(this);
	if (NULL != pRole)
	{
		pRole->GetTitleMgr()->SigEvent(ETE_SKILL_LEVEL, pSkill->GetID(), pSkill->GetLevel());
	}

	// 如果升级并造成了属性改变
	if( bLevelChanged && NeedRecalAtt() )
	{
		RecalAtt();
	}
}

//----------------------------------------------------------------------------------
// 技能走CD
//----------------------------------------------------------------------------------
VOID Unit::StartSkillCoolDown(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;

	pSkill->StartCoolDown();

	m_listSkillCoolDown.PushBack(pSkill->GetID());
}

//----------------------------------------------------------------------------------
// 清空某个技能的CD
//----------------------------------------------------------------------------------
VOID Unit::ClearSkillCoolDown(DWORD dwSkillID)
{
	Skill* pSkill = GetSkill(dwSkillID);
	if( !P_VALID(pSkill) ) return;

	pSkill->ClearCoolDown();
	m_listSkillCoolDown.Erase(dwSkillID);
}

//------------------------------------------------------------------------------
// 主动技能状态触发
//------------------------------------------------------------------------------
BOOL Unit::OnActiveSkillBuffTrigger(Skill* pSkill, TList<DWORD>& listTarget, ETriggerEventType eEvent, 
									DWORD dwEventMisc1/*=GT_INVALID*/, DWORD dwEventMisc2/*=GT_INVALID*/)
{
	// 技能不存在或者不是主动技能
	if( !P_VALID(pSkill) || FALSE == pSkill->IsActive() ) return FALSE;

	// 技能脚本
	const SkillScript* pScript = pSkill->GetSkillScript();
	if (P_VALID(pScript))
	{
		BOOL bIgnore = FALSE;

		if (pSkill->GetProto()->nType2 == ESSTE_SingleTarget || pSkill->GetProto()->nType2 == ESSTE_SingleTargetC)
		{
			if(listTarget.Empty()) // 这种触发是只针对对自己使用的技能的
				pScript->CastSingleTargetSkill(GetMap(), pSkill->GetID(), GetID(), GetID() ,bIgnore);
			else
				pScript->CastSingleTargetSkill(GetMap(), pSkill->GetID(), GetID(), *(listTarget.Begin()) ,bIgnore);
		}
		else
			pScript->CastSkill(GetMap(), pSkill->GetID(), GetID(), bIgnore);
		
		if (bIgnore)
		{
			return TRUE;
		}
	}

	// 检查技能是用skillbuff还是用属性表中的buffid
	for(INT n = 0; n < MAX_BUFF_PER_SKILL; ++n)
	{
		DWORD dwBuffTypeID = pSkill->GetBuffTypeID(n);
		DWORD dwTriggerID = pSkill->GetTriggerID(n);
		if( !P_VALID(dwBuffTypeID) || !P_VALID(dwTriggerID) ) continue;

		const tagBuffProto* pBuffProto = g_attRes.GetBuffProto(dwBuffTypeID);
		const tagTriggerProto* pTriggerProto = g_attRes.GetTriggerProto(dwTriggerID);
		if( !P_VALID(pBuffProto) || !P_VALID(pTriggerProto) ) continue;

		// 查看trigger类型是否相同，主要是为了优化
		if( ETEE_Null != pTriggerProto->eEventType && eEvent != pTriggerProto->eEventType )
			continue;

		// 通过buff的添加对象进行添加
		DWORD dwTargetUnitID = GT_INVALID;
		TList<DWORD>::TListIterator it = listTarget.Begin();
		while( listTarget.PeekNext(it, dwTargetUnitID) )
		{
			Unit* pTarget = GetMap()->FindUnit(dwTargetUnitID);
			if( !P_VALID(pTarget) ) continue;

			pTarget->TryAddBuff(this, pBuffProto, pTriggerProto, pSkill, NULL, TRUE, dwEventMisc1, dwEventMisc2);
		}

		// 尝试给自身添加
		// 主动技能自身buff添加规则：
		// （1）如果技能的目标是自身，则buff的添加对象类型里只要包含自身就尝试添加
		// （2）如果技能的目标非自身，则仅当buff的添加对象为自身时才尝试添加
		DWORD dwSkillTargetUnitID = GetCombatHandler().GetTargetUnitID();
		if( GT_INVALID == dwSkillTargetUnitID || GetID() == dwSkillTargetUnitID )
		{
			if( pBuffProto->dwTargetAddLimit & ETF_Self )
				TryAddBuff(this, pBuffProto, pTriggerProto, pSkill, NULL, TRUE, dwEventMisc1, dwEventMisc2);
		}
		else
		{
			if( pBuffProto->dwTargetAddLimit == ETF_Self )
				TryAddBuff(this, pBuffProto, pTriggerProto, pSkill, NULL, TRUE, dwEventMisc1, dwEventMisc2);
		}
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// Buff触发
//-----------------------------------------------------------------------------------
BOOL Unit::OnBuffTrigger(Unit* pTarget, ETriggerEventType eEvent, DWORD dwEventMisc1/* =GT_INVALID */, DWORD dwEventMisc2/* =GT_INVALID */)
{
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		m_Buff[n].OnTrigger(pTarget, eEvent, dwEventMisc1, dwEventMisc2);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------
// 注册Buff修改器
//-----------------------------------------------------------------------------------
VOID Unit::RegisterBuffModifier(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;
	if( ESTT_Buff != pSkill->GetTargetType() ) return;

	DWORD dwBuffID = pSkill->GetTargetBuffID();
	if( FALSE == P_VALID(dwBuffID) ) return;

	TList<DWORD>* pList = m_mapBuffModifier.Peek(dwBuffID);
	if( !P_VALID(pList) )
	{
		pList = new TList<DWORD>;
		m_mapBuffModifier.Add(dwBuffID, pList);
	}

	DWORD dwSkillTypeID = pSkill->GetTypeID();
	pList->PushBack(dwSkillTypeID);				// 里面注册的是技能的属性id
	
}

//-----------------------------------------------------------------------------------
// 撤销Buff修改器
//-----------------------------------------------------------------------------------
VOID Unit::UnRegisterBuffModifier(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;
	if( ESTT_Buff != pSkill->GetTargetType() ) return;

	DWORD dwBuffID = pSkill->GetTargetBuffID();
	if( FALSE == P_VALID(dwBuffID) ) return;

	TList<DWORD>* pList = m_mapBuffModifier.Peek(dwBuffID);
	if( P_VALID(pList) )
	{
		DWORD dwSkillTypeID = pSkill->GetTypeID();
		pList->Erase(dwSkillTypeID);
	}
}

//-----------------------------------------------------------------------------------
// 注册Trigger修改器
//-----------------------------------------------------------------------------------
VOID Unit::RegisterTriggerModifier(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;
	if( ESTT_Trigger != pSkill->GetTargetType() ) return;

	DWORD dwTriggerID = pSkill->GetTargetTriggerID();
	if( FALSE == P_VALID(dwTriggerID) ) return;

	TList<DWORD>* pList = m_mapTriggerModifier.Peek(dwTriggerID);
	if( !P_VALID(pList) )
	{
		pList = new TList<DWORD>;
		m_mapTriggerModifier.Add(dwTriggerID, pList);
	}
	DWORD dwSkillTypeID = pSkill->GetTypeID();
	pList->PushBack(dwSkillTypeID);				// 里面注册的是技能的属性id

}

//-----------------------------------------------------------------------------------
// 撤销Buff修改器
//-----------------------------------------------------------------------------------
VOID Unit::UnRegisterTriggerModifier(Skill* pSkill)
{
	if( !P_VALID(pSkill) ) return;
	if( ESTT_Trigger != pSkill->GetTargetType() ) return;

	DWORD dwTriggerID = pSkill->GetTargetTriggerID();
	if( FALSE == P_VALID(dwTriggerID) ) return;

	TList<DWORD>* pList = m_mapTriggerModifier.Peek(dwTriggerID);
	if( P_VALID(pList) )
	{
		DWORD dwSkillTypeID = pSkill->GetTypeID();
		pList->Erase(dwSkillTypeID);
	}
}

//-----------------------------------------------------------------------------------
// 尝试添加Buff
// Jason 2010-10-27 v1.2.2
// dwEventMisc1，字段默认是触发几率加成吧，是妖精特技效果的心情因素开始添加的。单位从0-100/200等，
// 因为是百分比，因此，他要被除于100.
//-----------------------------------------------------------------------------------
BOOL Unit::TryAddBuff(Unit* pSrc, const tagBuffProto* pBuffProto, const tagTriggerProto* pTriggerProto, 
					  Skill* pSkill, tagItem* pItem, BOOL bRecalAtt, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	if( !P_VALID(pBuffProto) ) return FALSE;

	// 通过属性ID得到ID和等级
	DWORD dwBuffID = Buff::GetIDFromTypeID(pBuffProto->dwID);
	INT nBuffLevel = Buff::GetLevelFromTypeID(pBuffProto->dwID);

	// 判断trigger
	if( P_VALID(pSrc) && P_VALID(pTriggerProto) )
	{
		if( !pSrc->TestTrigger(this, pTriggerProto, dwEventMisc1, dwEventMisc2) )
			return FALSE;

		// 称号事件触发
		Role *pRole = dynamic_cast<Role *>(pSrc);
		if (NULL != pRole)
		{
			pRole->GetTitleMgr()->SigEvent(ETE_TRIGGER, pTriggerProto->dwID, GT_INVALID);
		}
	}

	// 如果添加者存在，则检查添加者和自身的限制
	if( P_VALID(pSrc) )
	{
		// 首先检查对象类型限制
		DWORD dwTargetFlag = pSrc->TargetTypeFlag(this);
		if( !(pBuffProto->dwTargetAddLimit & dwTargetFlag) )
			return FALSE;

		// 不进行敌我判断
	}

	// 再检测buff添加状态限制
	DWORD dwStatFlag = GetStateFlag();
	if( (dwStatFlag & pBuffProto->dwTargetAddStateLimit) != dwStatFlag )
	{
		return FALSE;
	}

	// 判断抗性类型
	ERoleAttribute eReistType = BuffResistType2ERA(pBuffProto->eResistType);
	ERoleAttribute eEffectReStrength = GetEffectReStrength(eReistType);
	if( ERA_Null != eReistType && ERA_Regain_Addtion != eReistType && ERA_Null != eEffectReStrength)
	{
		// 计算命中率
		// 公式：命中率=（1-##抗性/200）
		FLOAT fRealReistValue = (FLOAT)GetAttValue(eReistType) - (FLOAT)pSrc->GetAttValue(eEffectReStrength);
		INT nProp = 100 - INT(fRealReistValue / 2.0f);
		if( nProp <= 0 ) //nProp = 0;
			return FALSE;
		if( (IUTIL->Rand() % 100) > nProp )
			return FALSE;
	}

	// 判断抵消和叠加
	INT nIndex = GT_INVALID;
	INT nRet = BuffCounteractAndWrap(pSrc, dwBuffID, nBuffLevel, pBuffProto->nLevel, pBuffProto->dwGroupFlag, pBuffProto->bBenifit, nIndex);

	if( EBCAWR_CanNotAdd == nRet ) return FALSE;
	else if( EBCAWR_Wraped == nRet ) return TRUE;

	ASSERT( EBCAWR_CanAdd == nRet );

	// 添加buff
	AddBuff(pBuffProto, pSrc, (P_VALID(pSkill) ? pSkill->GetID() : GT_INVALID), pItem, nIndex, bRecalAtt);
	return TRUE;
}

ERoleAttribute Unit::GetEffectReStrength(ERoleAttribute eReistType)
{
	switch (eReistType)
	{
	case ERA_Resist_Bleeding:	 // 玄伤抗性——命中几率
		return ERA_Fixed_damage_aggrandizement;		//固定伤害强化
	case ERA_Resist_Weak:	  	// 衰老抗性——命中几率
		return ERA_Aging_effect_strengthening;			//衰老效果强化
	case ERA_Resist_Choas:		// 心乱抗性——命中几率
		return ERA_Contrel_effect_deepen;					//控制效果加深
	case ERA_Resist_Special:	// 封速抗性——命中几率
		return ERA_Slowing_effect_strengthening;			//减速效果强化
	default:
		return ERA_Null;
	}
}

//-----------------------------------------------------------------------------------
// 用GM命令添加Buff
//-----------------------------------------------------------------------------------
BOOL Unit::GMTryAddBuff(Unit* pSrc, const tagBuffProto* pBuffProto, const tagTriggerProto* pTriggerProto, 
					  Skill* pSkill, tagItem* pItem, BOOL bRecalAtt, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	if( !P_VALID(pBuffProto) ) return FALSE;

	// 通过属性ID得到ID和等级
	DWORD dwBuffID = Buff::GetIDFromTypeID(pBuffProto->dwID);
	INT nBuffLevel = Buff::GetLevelFromTypeID(pBuffProto->dwID);

	// 判断trigger
	if( P_VALID(pSrc) && P_VALID(pTriggerProto) )
	{
		if( !pSrc->TestTrigger(this, pTriggerProto, dwEventMisc1, dwEventMisc2) )
			return FALSE;

		// 称号事件触发
		Role *pRole = dynamic_cast<Role *>(pSrc);
		if (NULL != pRole)
		{
			pRole->GetTitleMgr()->SigEvent(ETE_TRIGGER, pTriggerProto->dwID, GT_INVALID);
		}
	}

	// 如果添加者存在，则检查添加者和自身的限制
	//if( P_VALID(pSrc) )
	//{
	//	// 首先检查对象类型限制tbc:inves
	//	DWORD dwTargetFlag = pSrc->TargetTypeFlag(this);
	//	if( !(pBuffProto->dwTargetAddLimit & dwTargetFlag) )
	//		return FALSE;

	//	// 不进行敌我判断
	//}

	// 判断抗性类型
	ERoleAttribute eReistType = BuffResistType2ERA(pBuffProto->eResistType);
	if( ERA_Null != eReistType && ERA_Regain_Addtion != eReistType )
	{
		// 计算命中率
		// 公式：命中率=（1-##抗性/200）
		INT nProp = 100 - INT((FLOAT)GetAttValue(eReistType) / 2.0f);
		if( nProp < 0 ) nProp = 0;
		if( (IUTIL->Rand() % 100) > nProp )
			return FALSE;
	}

	// 判断抵消和叠加
	INT nIndex = GT_INVALID;
	INT nRet = BuffCounteractAndWrap(pSrc, dwBuffID, nBuffLevel, pBuffProto->nLevel, pBuffProto->dwGroupFlag, pBuffProto->bBenifit, nIndex);

	if( EBCAWR_CanNotAdd == nRet ) return FALSE;
	else if( EBCAWR_Wraped == nRet ) return TRUE;

	ASSERT( EBCAWR_CanAdd == nRet );

	// 添加buff
	for (int i= ERA_AttA_Start; i< ERA_End; i++)
	{
		SetAttRecalFlag(i);
	}
	AddBuff(pBuffProto, pSrc, (P_VALID(pSkill) ? pSkill->GetID() : GT_INVALID), pItem, nIndex, bRecalAtt);
	return TRUE;
}

//----------------------------------------------------------------------------------
// 增加一个状态
//----------------------------------------------------------------------------------
VOID Unit::AddBuff(const DWORD dwBuffID, Unit* pSrc, DWORD dwSrcSkillID, const tagItem* pItem, BOOL bRecalAtt)
{
	const tagBuffProto* pProto = g_attRes.GetBuffProto(dwBuffID);
	if( !P_VALID(pProto) ) return;

	INT nBuffLevel = Buff::GetLevelFromTypeID(pProto->dwID);

	INT nIndex = GT_INVALID;
	INT nRet = BuffCounteractAndWrap(pSrc, dwBuffID, nBuffLevel, pProto->nLevel, pProto->dwGroupFlag, pProto->bBenifit, nIndex);

	if( EBCAWR_CanNotAdd == nRet ) return;
	else if( EBCAWR_Wraped == nRet ) return;

	ASSERT( EBCAWR_CanAdd == nRet );

	// 添加buff
	for (int i= ERA_AttA_Start; i< ERA_End; i++)
	{
		SetAttRecalFlag(i);
	}

	AddBuff(pProto, pSrc, dwSrcSkillID, pItem, nIndex, bRecalAtt);
}

//----------------------------------------------------------------------------------
// 增加一个状态
//----------------------------------------------------------------------------------
VOID Unit::AddBuff(const tagBuffProto* pBuffProto, Unit* pSrc, DWORD dwSrcSkillID, const tagItem* pItem, INT nIndex, BOOL bRecalAtt)
{
	if( !P_VALID(pBuffProto) ) return;
	if( nIndex < 0 || nIndex >= MAX_BUFF_NUM ) return;

	// 通过BuffID找到该Buff是否有Mod
	DWORD dwBuffID = Buff::GetIDFromTypeID(pBuffProto->dwID);
	TList<DWORD>* pListModifier = NULL;
	if( P_VALID(pSrc) ) pListModifier = pSrc->GetBuffModifier(dwBuffID);

	// 如果是瞬时buff
	if( pBuffProto->bInstant )
	{
		// 计算瞬时效果
		if( !P_VALID(pListModifier) || pListModifier->Empty() )
		{
			CalBuffInstantEffect(pSrc, EBEM_Instant, pBuffProto, NULL);
		}
		else
		{
			// 临时生成一个mod结构
			tagBuffMod mod;
			TList<DWORD>::TListIterator it = pListModifier->Begin();
			DWORD dwSkillTypeID = GT_INVALID;

			while( pListModifier->PeekNext(it, dwSkillTypeID) )
			{
				const tagSkillProto* pSkillProto = g_attRes.GetSkillProto(dwSkillTypeID);
				if( !P_VALID(pSkillProto) ) continue;

				mod.SetMod(pSkillProto);
			}

			CalBuffInstantEffect(pSrc, EBEM_Instant, pBuffProto, &mod);
		}

		// 发送消息
		if( P_VALID(GetMap()) )
		{
			if( pBuffProto->bInstant && !pBuffProto->bEffectExist)
			{
				;// 瞬时没有特效的buff不需要同步
			}
			else
			{
				tagNS_AddRoleBuff send;
				send.dwRoleID = GetID();
				send.Buff.dwBuffTypeID = pBuffProto->dwID;
				send.Buff.nWarpTimes = 1;
				send.Buff.nMaxPersistTime = GT_INVALID;
				send.Buff.nPersistTimeLeft = GT_INVALID;

				GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
			}
		}
	}
	else
	{
		m_Buff[nIndex].Init(pBuffProto, this, pSrc, dwSrcSkillID, pItem, nIndex, pListModifier);
		m_mapBuff.Add(m_Buff[nIndex].GetID(), &m_Buff[nIndex]);

		// 发送消息
		if( GetMap() )
		{
			if( pBuffProto->bInstant && !pBuffProto->bEffectExist)
			{
				;// 瞬时没有特效的buff不需要同步
			}
			else
			{
				tagNS_AddRoleBuff send;
				send.dwRoleID = GetID();
				send.Buff.dwBuffTypeID = m_Buff[nIndex].GetTypeID();
				send.Buff.nWarpTimes = m_Buff[nIndex].GetWrapTimes();
				send.Buff.nMaxPersistTime = m_Buff[nIndex].GetPersistTime();
				send.Buff.nPersistTimeLeft = m_Buff[nIndex].GetPersistTimeLeft();

				if( P_VALID(GetMap()) )
				{
					GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
				}
			}
		}
	}

// 	if ( HOLYSOUL_BUFFID == Buff::GetIDFromTypeID(pBuffProto->dwID))
// 	{
// 		if ( pSrc->IsRole() )
// 		{
// 			Role *pRole = dynamic_cast<Role *>(pSrc);
// 			if (P_VALID(pRole))
// 			{
// 				pRole->GetHolySoul().AddEvent(EHSS_Awake,EVT_SetSoul_State,0,NULL);
// 			}
// 		}	
// 	}
	// 重新计算基本属性
	if( bRecalAtt && NeedRecalAtt() ) RecalAtt();
}

//----------------------------------------------------------------------------------
// 去掉一个状态
//----------------------------------------------------------------------------------
INT Unit::RemoveBuff(DWORD dwBuffID, BOOL bRecalAtt, BOOL bSelf)
{
	Buff* pBuff = m_mapBuff.Peek(dwBuffID);
	if( !P_VALID(pBuff) ) return GT_INVALID;

	// 得到Buff当前的状态
	Buff::EBuffState eState = pBuff->GetState();

	// 正在删除的Buff，不需要再处理
	if( Buff::EBS_Destroying == eState )
	{
		return GT_INVALID;
	}

	// 当前正在初始化或更新的Buff，要添加到删除列表
	if( Buff::EBS_Initing	== eState ||
		Buff::EBS_Updating	== eState )
	{
		m_listDestroyBuffID.PushBack(pBuff->GetID());
		return GT_INVALID;
	}

//	if ( HOLYSOUL_BUFFID == Buff::GetIDFromTypeID(pBuff->GetTypeID()))
// 	{
// 		if ( this->IsRole() )
// 		{
// 			Role *pRole = dynamic_cast<Role *>(this);
// 			if (P_VALID(pRole))
// 			{
// 				pRole->GetHolySoul().AddEvent(EHSS_Asleep,EVT_SetSoul_State,0,NULL);
// 			}
// 		}	
// 	}

	// 当前在空闲状态的Buff，可以立即删除
	if( GetMap() )
	{
		tagNS_RemoveRoleBuff send;
		send.dwRoleID = GetID();
		send.dwBuffTypeID = pBuff->GetTypeID();

		if( P_VALID(GetMap()) )
		{
			GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
		}
	}

	DWORD dwBuffInterruptID = pBuff->GetBuffInterruptID();

	INT nIndex = pBuff->GetIndex();
	m_mapBuff.Erase(dwBuffID);
	pBuff->Destroy(bSelf);

	// 删除伴随BuffID
	if( P_VALID(dwBuffInterruptID) )
	{
		RemoveBuff(dwBuffInterruptID, FALSE);
	}

	// 重新计算基本属性
	if( bRecalAtt && NeedRecalAtt() ) RecalAtt();

	return nIndex;
}
//-----------------------------------------------------------------------------------
// 去掉所有由某个技能产生的buff
//-----------------------------------------------------------------------------------
VOID Unit::RemoveAllBuffBelongSkill(DWORD dwSkillID, BOOL bRecalAtt)
{
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		// 必须是自己施放的并且由该技能产生
		if( m_Buff[n].GetSrcUnitID() != GetID() || m_Buff[n].GetSrcSkillID() != dwSkillID )
			continue;

		RemoveBuff(m_Buff[n].GetID(), FALSE);
	}

	if( bRecalAtt && NeedRecalAtt() )
	{
		RecalAtt();
	}
}

//-----------------------------------------------------------------------------------
// 去掉所有Buff
//-----------------------------------------------------------------------------------
VOID Unit::RemoveAllBuff()
{
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		RemoveBuff(m_Buff[n].GetID(), FALSE);
	}

	if( NeedRecalAtt() )
		RecalAtt();
}

//-----------------------------------------------------------------------------------
// 去掉所有有益或有害Buff
//-----------------------------------------------------------------------------------
VOID Unit::RemoveAllBuff(BOOL bBenefit)
{
	INT nStart = 0, nEnd = MAX_BENEFIT_BUFF_NUM;

	if( !bBenefit )
	{
		nStart = MAX_BENEFIT_BUFF_NUM;
		nEnd = MAX_BUFF_NUM;
	}

	INT nIndex = GT_INVALID;
	for(INT n = nStart; n < nEnd; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;
		RemoveBuff(m_Buff[n].GetID(), FALSE);
	}

	if( NeedRecalAtt() )
		RecalAtt();
}

//-----------------------------------------------------------------------------------
// 更新Buff
//-----------------------------------------------------------------------------------
VOID Unit::UpdateBuff()
{
	// 首先更新所有存在的Buff
	for(INT n = 0; n < MAX_BUFF_NUM; n++)
	{
		BOOL IsValid = m_Buff[n].IsValid();
		if( !IsValid) continue;

		// 调用该Buff的Update
		if( m_Buff[n].Update() )
		{
			RemoveBuff(m_Buff[n].GetID(), TRUE, TRUE);
		}
	}

	// 再删除删除列表里的Buff
	if( !m_listDestroyBuffID.Empty() )
	{
		DWORD dwBuffID = m_listDestroyBuffID.PopFront();
		while( P_VALID(dwBuffID) )
		{
			RemoveBuff(dwBuffID, TRUE);

			dwBuffID = m_listDestroyBuffID.PopFront();
		}
	}
}

//-----------------------------------------------------------------------------------
// 手动取消一个Buff
//-----------------------------------------------------------------------------------
BOOL Unit::CancelBuff(DWORD dwBuffID)
{
	Buff* pBuff = GetBuffPtr(dwBuffID);
	if( !P_VALID(pBuff) ) return FALSE;

	// 看是否能手动打断
	if( !pBuff->Interrupt(EBIF_Manual) ) return FALSE;

	// 删除Buff
	RemoveBuff(dwBuffID, TRUE);

	return TRUE;
}

//-----------------------------------------------------------------------------------
// 打断Buff事件
//-----------------------------------------------------------------------------------
VOID Unit::OnInterruptBuffEvent(EBuffInterruptFlag eFlag, INT nMisc/* =GT_INVALID */)
{
	BOOL bNeedRecal = FALSE;
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;
		if( !m_Buff[n].Interrupt(eFlag, nMisc) ) continue;
		
		RemoveBuff(m_Buff[n].GetID(), FALSE);
		bNeedRecal = TRUE;
	}

	if( bNeedRecal && NeedRecalAtt() )	RecalAtt();
}

//-----------------------------------------------------------------------------------
// 驱散最后一个Buff（参数指定是Buff还是Debuff）
//-----------------------------------------------------------------------------------
VOID Unit::DispelBuff(BOOL bBenefit)
{
	INT nStart = 0, nEnd = MAX_BENEFIT_BUFF_NUM;

	if( !bBenefit )
	{
		nStart = MAX_BENEFIT_BUFF_NUM;
		nEnd = MAX_BUFF_NUM;
	}

	INT nIndex = GT_INVALID;
	for(INT n = nStart; n < nEnd; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;
		nIndex = n;
	}

	if( GT_INVALID != nIndex )
	{
		RemoveBuff(m_Buff[nIndex].GetID(), TRUE);
	}
}

//-----------------------------------------------------------------------------------
// 驱散最后一个抗性类型为eType的Buff
//-----------------------------------------------------------------------------------
VOID Unit::DispelBuff(EBuffResistType eType)
{
	INT nIndex = GT_INVALID;
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		if( m_Buff[n].GetResistType() != eType ) continue;

		nIndex = n;
	}

	if( GT_INVALID != nIndex )
	{
		RemoveBuff(m_Buff[nIndex].GetID(), TRUE);
	}
}

//-----------------------------------------------------------------------------------
// 驱散某一个指定ID的Buff
//-----------------------------------------------------------------------------------
VOID Unit::DispelBuff(DWORD dwBuffID)
{
	RemoveBuff(dwBuffID, TRUE);
}

//-----------------------------------------------------------------------------------
// 驱散某一个指定效果类型的buff
//-----------------------------------------------------------------------------------
VOID Unit::DispelBuff(INT nType, BOOL bLastOne)
{
	//if (!(eType == EBET_Dizzy || eType == EBET_NoSkill || eType == EBET_Spor || eType == EBET_Tie || eType == EBET_Invincible
	//	  || eType == EBET_DisArm || eType == EBET_NoPrepare || eType == EBET_IgnoreArmor || eType == EBET_Sneer || eType == EBET_Immunity))
	//	return;
	EBuffEffectType eType = EBET_Null;
	switch(nType)
	{
	case 1:
		eType = EBET_Dizzy;
		break;
	case 2:
		eType = EBET_NoSkill;
		break;
	case 3:
		eType = EBET_Spor;
		break;
	case 4:
		eType = EBET_Tie;
		break;
	case 5:
		eType = EBET_Invincible;
		break;
	case 6:
		eType = EBET_DisArm;
		break;
	case 7:
		eType = EBET_NoPrepare;
		break;
	case 8:
		eType = EBET_IgnoreArmor;
		break;
	case 9:
		eType = EBET_Sneer;
		break;
	case 10:
		eType = EBET_Immunity;
		break;
	default:
		break;
	}

	INT nIndex = GT_INVALID;
	for(INT n = 0; n < MAX_BUFF_NUM; ++n)
	{
		if( !m_Buff[n].IsValid() ) continue;

		if( m_Buff[n].GetEffectType(EBEM_Persist) != eType ) continue;

		if(bLastOne)
			nIndex = n;
		else
			RemoveBuff(m_Buff[n].GetID(), TRUE);
	}

	if( GT_INVALID != nIndex && bLastOne)
	{
		RemoveBuff(m_Buff[nIndex].GetID(), TRUE);
	}
}

//-----------------------------------------------------------------------------------
// 计算Buff的瞬时类效果，瞬时类效果包括：瞬时效果，间隔作用效果和结束时效果
//-----------------------------------------------------------------------------------
VOID Unit::CalBuffInstantEffect(Unit* pSrc, EBuffEffectMode eMode, const tagBuffProto* pProto, const tagBuffMod* pMod, INT nWrapTimes, Unit* pTriggerTarget)
{
	if( !P_VALID(pProto) ) return;
	if( EBEM_Instant != eMode && EBEM_Inter != eMode && EBEM_Finish != eMode ) return;

	// 瞬时效果不关心叠加次数
	if( EBEM_Instant == eMode ) nWrapTimes = 1;

	BOOL bHaveMod = FALSE;
	if( P_VALID(pMod) && pMod->IsValid() && pMod->eModBuffEffectMode == eMode )
	{
		bHaveMod = TRUE;
	}

	// 属性加成影响
	INT nAttMod[EBEA_End] = {0};
	const INT* pnAttMod = NULL;
	switch(eMode)
	{
	case EBEM_Instant:
		pnAttMod = pProto->nInstantAttMod;
		break;

	case EBEM_Inter:
		pnAttMod = pProto->nInterAttMod;
		break;

	case EBEM_Finish:
		pnAttMod = pProto->nFinishAttMod;
		break;

	default:
		break;
	}

	// 效果
	EBuffEffectType eEffect = pProto->eEffect[eMode];
	DWORD dwEffectMisc1 = pProto->dwEffectMisc1[eMode];
	DWORD dwEffectMisc2 = pProto->dwEffectMisc2[eMode];

	// mod对其的影响
	if( bHaveMod )
	{
		for(INT n = 0; n < EBEA_End; ++n)
		{
			nAttMod[n] = pnAttMod[n] + pMod->nEffectAttMod[n];

			if( abs(nAttMod[n]) > 100000 )	// 百分比
			{
				INT nAtt = 0;
				switch(n)
				{
				case EBEA_HP:
					nAtt = GetAttValue(ERA_MaxHP);
					break;

				case EBEA_MP:
					nAtt = GetAttValue(ERA_MaxMP);
					break;

				case EBEA_Vitality:
					nAtt = GetAttValue(ERA_MaxVitality);
					break;

				case ERA_Endurance:
					nAtt = GetAttValue(ERA_MaxEndurance);
					break;

				default:
					break;
				}

				nAttMod[n] = (nAttMod[n] > 0 ? 1 : -1) * INT((FLOAT)nAtt * (FLOAT(abs(nAttMod[n]) - 100000) / 10000.0f));
			}

			nAttMod[n] *= nWrapTimes;
		}
		dwEffectMisc1 += pMod->nEffectMisc1Mod;
		dwEffectMisc2 += pMod->nEffectMisc2Mod;
	}
	else
	{
		for(INT n = 0; n < EBEA_End; ++n)
		{
			nAttMod[n] = pnAttMod[n];

			if( abs(nAttMod[n]) > 100000 )	// 百分比
			{
				INT nAtt = 0;
				switch(n)
				{
				case EBEA_HP:
					nAtt = GetAttValue(ERA_MaxHP);
					break;

				case EBEA_MP:
					nAtt = GetAttValue(ERA_MaxMP);
					break;

				case EBEA_Vitality:
					nAtt = GetAttValue(ERA_MaxVitality);
					break;

				case ERA_Endurance:
					nAtt = GetAttValue(ERA_MaxEndurance);
					break;

				default:
					break;
				}

				nAttMod[n] = (nAttMod[n] > 0 ? 1 : -1) * INT((FLOAT)nAtt * (FLOAT(abs(nAttMod[n]) - 100000) / 10000.0f));
			}

			nAttMod[n] *= nWrapTimes;
		}
	}
	INT iMaxAttackNum = pProto->iMaxAttackNum;
	if(P_VALID(pMod))
		iMaxAttackNum += pMod->iMaxAttackNumMod;

	// 根据攻击类型，攻击距离和攻击范围来确定周围人
	TList<Unit*> listTargetUnit;
	CalBuffTargetList(pSrc, pProto->eOPType, pProto->fOPDistance, pProto->fOPRadius, 
						pProto->eFriendly, pProto->dwTargetLimit, pProto->dwTargetStateLimit, iMaxAttackNum, listTargetUnit, pTriggerTarget);

	Unit* pTarget = listTargetUnit.PopFront();
	while( P_VALID(pTarget) )
	{
		// 计算属性影响
		for(INT n = 0; n < EBEA_End; ++n)
		{
			if( 0 == nAttMod[n] ) continue;

			switch(n)
			{
			case EBEA_HP:
				{
					// 如果是恢复类buff
					if (pProto->eResistType == EBRT_Regain)
					{
						// 元气影响恢复百分比＝0.4－等级×1.8÷当前元气值；（最小值取0）
						FLOAT fPneuma = (FLOAT)max( 0.0, (0.4 - (FLOAT)(pSrc->GetLevel()) * 1.8 / (FLOAT)(pSrc->GetAttValue(ERA_Pneuma))) );

						// 气力影响恢复百分比＝气力÷（角色等级×50＋50）
						FLOAT fRegain = (FLOAT)(pSrc->GetAttValue(ERA_Regain_Addtion)) / (FLOAT)((pSrc->GetLevel())*50 + 50);

						nAttMod[n] = (INT)((FLOAT)nAttMod[n] * (FLOAT)(1.0 + fPneuma + fRegain));
					}

					// 如果是暗伤
					if (pProto->eResistType == EBRT_Bleeding)
					{
						// 元气影响暗伤百分比＝0.2－等级×0.9÷当前元气值；（最小值取0）
						FLOAT fPneuma = (FLOAT)max( 0.0, (0.2 - (FLOAT)(pSrc->GetLevel()) * 0.9 / (FLOAT)(pSrc->GetAttValue(ERA_Pneuma))) );

						// 气力影响暗伤百分比＝气力÷（角色等级×100＋100）
						FLOAT fRegain = (FLOAT)(pSrc->GetAttValue(ERA_Regain_Addtion)) / (FLOAT)(pSrc->GetLevel()*50 + 50);

						// 状态作用对象的暗伤抗性百分比=暗伤抗性÷200
						FLOAT fResist = (FLOAT)GetAttValue(ERA_Resist_Bleeding) / 200.0;

						// 最终暗伤效果＝状态暗伤值×（1＋元气影响暗伤百分比＋气力影响暗伤百分比－
						// 状态作用对象的暗伤抗性÷200）
						nAttMod[n] = (INT)((FLOAT)nAttMod[n] * (FLOAT)(1.0 + fPneuma + fRegain - fResist));
					}

					if( pTarget->IsCreature() )
					{
						Creature* pCreature = static_cast<Creature*>(pTarget);
						if( !P_VALID(pCreature) )	break;
						
						// Jason 2010-11-22 由于加上怪物仇恨目标不可到达判断，会导致怪物不攻击“非可到达”目标，但是该目标可以攻击怪
						// 在于策划等协商后，先播放怪被攻击效果，然后在判断怪物目标全不合法的情况下，给怪物回满血
						// Jason 2010-11-26 提到减血前，因为减血会清仇恨
						AIController* pController = pCreature->GetAI();
						if( P_VALID(pController) )
						{
							pController->CalMaxEnmity();
							if( pController->GetMaxEnmityUnitID() == GT_INVALID )
							{
								pTarget->SetAttValue(ERA_HP, pTarget->GetAttValue(ERA_MaxHP));
								tagNS_CreatureCanntBeAck send;
								send.dwErrorCode = E_UseSkill_CreatureCannotAck_FullBlood;

								Unit * pSrcUnit = NULL;
								if( pTarget->IsHaveBuff(Buff::GetIDFromTypeID(pProto->dwID)) )
								{
									Buff * pBuff = pTarget->GetBuffPtr(Buff::GetIDFromTypeID(pProto->dwID));
									if( P_VALID(pBuff) )
									{
										DWORD dwSrcID = pBuff->GetSrcUnitID();
										// 只用找玩家，其他的不管
										pSrcUnit = g_roleMgr.GetRolePtrByID(dwSrcID);
									}
								}
								if( P_VALID(pSrcUnit) && pSrcUnit->IsRole() )
								{
									Role * pMe = (Role*)pSrcUnit;
									pMe->SendMessage(&send,send.dwSize);
								}
								break;
							}
						}
						// 如果两者都是怪物，则不会触发BuffInjury
						if (!this->IsCreature())
						{
							pCreature->OnBuffInjury(pSrc);
						}
					}

					pTarget->ChangeHP(nAttMod[n], pSrc, NULL, pProto);
				}
				break;

			case EBEA_MP:
				{
					pTarget->ChangeMP(nAttMod[n]);
				}
				break;

			case EBEA_Rage:
				{
					pTarget->ChangeRage(nAttMod[n]);
				}
				break;

			case EBEA_Vitality:
				{
					//INT nVitality = pTarget->GetSpecSkillValue(ESSF_Valicity,ESSV_ALL,nAttMod[n]);
					pTarget->ChangeVitality(nAttMod[n]);
					//pTarget->ChangeVitality(nVitality);
				}
				break;

			case EBEA_Endurance:
				{
					pTarget->ChangeEndurance(nAttMod[n]);
				}
				break;

			case EBEA_Morale:
				{
					pTarget->ModAttValue(ERA_Morale, nAttMod[n]);
				}
				break;

			case EBEA_Injury:
				{
					pTarget->ModAttValue(ERA_Injury, nAttMod[n]);
				}
				break;

			default:
				break;
			}
		}

		// 计算特殊效果
		if( EBET_Null != eEffect )
		{
			BuffEffect::CalBuffEffect(pTarget, pSrc, eEffect, dwEffectMisc1, dwEffectMisc2, TRUE, pProto);
		}

		// 再取一个
		pTarget = listTargetUnit.PopFront();
	}
}

//-----------------------------------------------------------------------------------
// 计算Buff的持续性效果，只能对自身作用
//-----------------------------------------------------------------------------------
VOID Unit::CalBuffPersistEffect(Unit* pSrc, const tagBuffProto* pProto, const tagBuffMod* pMod, INT nWrapTimes, BOOL bSet/* =TRUE */)
{
	if( !P_VALID(pProto) ) return;

	BOOL bHaveMod = FALSE;
	if( P_VALID(pMod) && pMod->IsValid() && EBEM_Persist == pMod->eModBuffEffectMode )
	{
		bHaveMod = TRUE;
	}

	EBuffEffectType eEffect = pProto->eEffect[EBEM_Persist];
	DWORD dwEffectMisc1 = pProto->dwEffectMisc1[EBEM_Persist];
	DWORD dwEffectMisc2 = pProto->dwEffectMisc2[EBEM_Persist];

	if( bHaveMod )
	{
		dwEffectMisc1 += pMod->nEffectMisc1Mod;
		dwEffectMisc2 += pMod->nEffectMisc2Mod;
	}

	// 先计算属性加成
	ERoleAttribute eAtt = ERA_Null;
	INT nValue = 0;
	TMap<ERoleAttribute, INT>::TMapIterator it;
	TMap<ERoleAttribute, INT>::TMapIterator itPct;

	// 先计算静态属性的
	it = pProto->mapRoleAttMod.Begin();
	while( pProto->mapRoleAttMod.PeekNext(it, eAtt, nValue) )
	{
		ModAttModValue(eAtt, (bSet ? nValue : -nValue) * nWrapTimes);
	}

	itPct = pProto->mapRoleAttModPct.Begin();
	while( pProto->mapRoleAttModPct.PeekNext(itPct, eAtt, nValue) )
	{
		ModAttModValuePct(eAtt, (bSet ? nValue : -nValue) * nWrapTimes);
	}

	// 在计算mod的
	if( bHaveMod )
	{
		it = pMod->mapRoleAttMod.Begin();
		while( pMod->mapRoleAttMod.PeekNext(it, eAtt, nValue) )
		{
			ModAttModValue(eAtt, (bSet ? nValue : -nValue) * nWrapTimes);
		}

		itPct = pMod->mapRoleAttModPct.Begin();
		while( pMod->mapRoleAttModPct.PeekNext(itPct, eAtt, nValue) )
		{
			ModAttModValuePct(eAtt, (bSet ? nValue : -nValue) * nWrapTimes);
		}
	}

	// 再计算效果
	if( EBET_Null != eEffect )
	{
		//while(--nWrapTimes >= 0 && )
		// Jason 解决非属性buff，可能的叠加消除问题
		if(!bSet)
		{
			// 类型判断
			if( pProto->eEffect[EBEM_Persist] == EBET_NoPrepare ||
				pProto->eEffect[EBEM_Persist] == EBET_IgnoreArmor )
				BuffEffect::CalBuffEffect(this, pSrc, eEffect, dwEffectMisc1 * nWrapTimes, dwEffectMisc2, bSet, pProto);
			else
				BuffEffect::CalBuffEffect(this, pSrc, eEffect, dwEffectMisc1, dwEffectMisc2, bSet, pProto);
		}
		else
			BuffEffect::CalBuffEffect(this, pSrc, eEffect, dwEffectMisc1, dwEffectMisc2, bSet, pProto);
	}
}

//-----------------------------------------------------------------------------------
// 叠加Buff的持续性效果
//-----------------------------------------------------------------------------------
VOID Unit::WrapBuffPersistEffect(Unit* pSrc, const tagBuffProto* pProto, const tagBuffMod* pMod)
{
	CalBuffPersistEffect(pSrc, pProto, pMod, 1, TRUE);
}


//-----------------------------------------------------------------------------------
// 计算Buff的作用对象
//-----------------------------------------------------------------------------------
VOID Unit::CalBuffTargetList(Unit* pSrc, EBuffOPType eOPType, FLOAT fOPDist, FLOAT fOPRadius, 
							 EBuffFriendEnemy eFriendEnemy, DWORD dwTargetLimit, DWORD dwTargetStateLimit, INT iMaxAttackNum, TList<Unit*>& listTarget, Unit* pTarget)
{
	// 如果添加者已经不存在了，直接返回
	if( !P_VALID(pSrc) || iMaxAttackNum<=0 ) return;

	// 首先检查一下自身
	if( pSrc->IsBuffTargetValid(this, eFriendEnemy, dwTargetLimit, dwTargetStateLimit) )
	{
		listTarget.PushBack(this);
		if(--iMaxAttackNum <=0) return;
	}

	// 再检查一下目标
	if( P_VALID(pTarget) && pSrc->IsBuffTargetValid(pTarget, eFriendEnemy, dwTargetLimit, dwTargetStateLimit) )
	{
		listTarget.PushBack(pTarget);
		if(--iMaxAttackNum <=0) return;
	}

	// 预先检查一下攻击范围
	if( 0.0f == fOPRadius ) return;

	// 如果攻击范围不为0，则以目标为球心检测
	FLOAT fOPRadiusSQ = fOPRadius * fOPRadius;

	tagVisTile* pVisTile[ED_End] = {0};

	// 得到攻击范围内的vistile列表
	GetMap()->GetVisTile(GetCurPos(), fOPRadius, pVisTile);
	Role*		pRole		= NULL;
	Creature*	pCreature	= NULL;

	for(INT n = ED_Center; n < ED_End; n++)
	{
		if( !P_VALID(pVisTile[n]) ) continue;

		// 首先检测人物
		TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
		TMap<DWORD, Role*>::TMapIterator it = mapRole.Begin();

		while( mapRole.PeekNext(it, pRole) )
		{
			if( pRole == this ) continue;

			if( FALSE == pSrc->IsBuffTargetValid(pRole, eFriendEnemy, dwTargetLimit, dwTargetStateLimit) )
				continue;

			// 距离判断
			FLOAT fDistSQ = Vec3DistSq(GetCurPos(), pRole->GetCurPos());
			if( fDistSQ > fOPRadiusSQ  ) continue;

			// 射线检测

			// 判断通过，则将玩家加入到列表中
			listTarget.PushBack(pRole);

			if(--iMaxAttackNum<=0) return;
		}

		// 再检测生物
		TMap<DWORD, Creature*>& mapCreature = pVisTile[n]->mapCreature;
		TMap<DWORD, Creature*>::TMapIterator it2 = mapCreature.Begin();

		while( mapCreature.PeekNext(it2, pCreature) )
		{
			if( pCreature == this ) continue;

			if( FALSE == pSrc->IsBuffTargetValid(pCreature, eFriendEnemy, dwTargetLimit, dwTargetStateLimit) )
				continue;

			// 距离判断
			FLOAT fDistSQ = Vec3DistSq(GetCurPos(), pCreature->GetCurPos());
			if( fDistSQ > fOPRadiusSQ  ) continue;

			// 射线检测

			// 判断通过，则将生物加入到列表中
			listTarget.PushBack(pCreature);

			if(--iMaxAttackNum<=0) return;
		}
	}
}

//-----------------------------------------------------------------------------------
// 取得指定技能施放的buff
//-----------------------------------------------------------------------------------
Buff* Unit::GetRelativeSkillBuff(DWORD dwSkillID, BOOL bBenefit /*= TRUE*/)
{
	INT nStart = 0;
	INT nEnd = MAX_BENEFIT_BUFF_NUM;

	if( !bBenefit )
	{
		nStart = MAX_BENEFIT_BUFF_NUM;
		nEnd = MAX_BUFF_NUM;
	}

	for(INT n = nStart; n < nEnd; n++)
	{
		// 空的位置
		if( !m_Buff[n].IsValid() )
		{
			continue;
		}
		
		// 技能ID相同
		if (m_Buff[n].GetSrcSkillID() == dwSkillID)
		{
			return &m_Buff[n];
		}
	}

	return NULL;
}

//-----------------------------------------------------------------------------------
// 检查某个目标是不是Buff的合法目标
//-----------------------------------------------------------------------------------
BOOL Unit::IsBuffTargetValid(Unit* pTarget, EBuffFriendEnemy eFriendEnemy, DWORD dwTargetLimit, DWORD dwTargetStateLimit)
{
	if( !P_VALID(pTarget) ) return FALSE;

	// 类型判断
	DWORD dwTargetFlag = TargetTypeFlag(pTarget);
	if( !(dwTargetFlag & dwTargetLimit) )
		return FALSE;

	// 状态判断
	DWORD dwTargetStateFlag = pTarget->GetStateFlag();
	if( (dwTargetStateFlag & dwTargetStateLimit) != dwTargetStateFlag )
		return FALSE;

	// 敌我判断
	DWORD dwFriendEnemy = FriendEnemy(pTarget);
	if( EBFE_Friendly == eFriendEnemy )
	{
		if( !(dwFriendEnemy & ETFE_Friendly) )
			return FALSE;
	}
	else if( EBFE_Hostile == eFriendEnemy )
	{
		if( !(dwFriendEnemy & ETFE_Hostile) )
			return FALSE;
	}
	else if( EBFE_Independent == eFriendEnemy )
	{
		if( !(dwFriendEnemy & ETFE_Independent) )
			return FALSE;
	}
	else if( EBFE_All == eFriendEnemy )
	{
		if( !(dwFriendEnemy & (ETFE_Friendly | ETFE_Hostile | ETFE_Independent)) )
			return FALSE;
	}
	else
	{
		return FALSE;
	}

	// 判断通过
	return TRUE;
}

//-----------------------------------------------------------------------------------
// 计算Buff的抵消和叠加
//-----------------------------------------------------------------------------------
INT Unit::BuffCounteractAndWrap(Unit* pSrc, DWORD dwBuffID, INT nBuffLevel, INT nGroupLevel, DWORD dwGroupFlag, BOOL bBenefit, INT& nIndex)
{
	// 首先检测ID
	Buff* pBuff = m_mapBuff.Peek(dwBuffID);

	// 如果存在一个同样的buff
	if( P_VALID(pBuff) && Buff::EBS_Idle == pBuff->GetState() )
	{
		if( pBuff->GetLevel() > nBuffLevel )
		{
			return EBCAWR_CanNotAdd;
		}
		else if( pBuff->GetLevel() == nBuffLevel )
		{
			// 检测看看能不能叠加
// 			if (!pBuff->CanWrapSelf())
// 			{
// 				return EBCAWR_CanNotAdd;
// 			}
			
			if( P_VALID(pSrc) && pBuff->GetSrcUnitID() == pSrc->GetID() )
			{
				pBuff->Wrap();
				if( NeedRecalAtt() ) RecalAtt();

				if( P_VALID(GetMap()) )
				{
					tagNS_UpdateRoleBuff send;
					send.dwRoleID = GetID();
					send.Buff.dwBuffTypeID = pBuff->GetTypeID();
					send.Buff.nWarpTimes = pBuff->GetWrapTimes();
					send.Buff.nMaxPersistTime = pBuff->GetPersistTime();
					send.Buff.nPersistTimeLeft = pBuff->GetPersistTimeLeft();

					GetMap()->SendBigVisTileMsg(this, &send, send.dwSize);
				}

				return EBCAWR_Wraped;
			}
			else
			{
				// 顶掉，返回可以添加
				nIndex = RemoveBuff(pBuff->GetID(), TRUE);
				ASSERT( GT_INVALID != nIndex );
				return EBCAWR_CanAdd;
			}
		}
		else
		{
			// 顶掉该buff，返回可以添加
			nIndex = RemoveBuff(pBuff->GetID(), TRUE);
			ASSERT( nIndex != GT_INVALID );
			return EBCAWR_CanAdd;
			
		}
	}
	// 如果不存在一个同样的buff
	else
	{
		INT nStart = 0;
		INT nEnd = MAX_BENEFIT_BUFF_NUM;

		if( !bBenefit )
		{
			nStart = MAX_BENEFIT_BUFF_NUM;
			nEnd = MAX_BUFF_NUM;
		}

		INT nBlankIndex = GT_INVALID;		// 空位
		INT nFullIndex = GT_INVALID;		// 如果Buff满时，则代表可以被顶掉的索引
		for(INT n = nStart; n < nEnd; n++)
		{
			// 空的位置
			if( !m_Buff[n].IsValid() )
			{
				if( GT_INVALID == nBlankIndex )
				{
					nBlankIndex = n;
					if( GT_INVALID == dwGroupFlag ) break;
				}
				continue;
			}
			// group_flag相同
			else if( (dwGroupFlag != GT_INVALID) && 
					 (m_Buff[n].GetGroupFlag() == dwGroupFlag) )
			{
				// 如果当前不出于Idle状态，则不能顶掉
				if( Buff::EBS_Idle != m_Buff[n].GetState() )
				{
					return EBCAWR_CanNotAdd;
				}

				// 比较等级（这个是组等级）
				if( m_Buff[n].GetGroupLevel() > nGroupLevel )
				{
					return EBCAWR_CanNotAdd;
				}
				else
				{
					nIndex = RemoveBuff(m_Buff[n].GetID(), TRUE);
					return EBCAWR_CanAdd;
				}
			}
			// buff满时顶掉
			else if( GT_INVALID == nFullIndex && 
					 Buff::EBS_Idle == m_Buff[n].GetState() &&
					 m_Buff[n].Interrupt(EBIF_BuffFull) )
			{
				nFullIndex = n;
			}
		}

		if( GT_INVALID != nBlankIndex )
		{
			nIndex = nBlankIndex;
			return EBCAWR_CanAdd;
		}
		else if( GT_INVALID != nFullIndex )
		{
			RemoveBuff(m_Buff[nFullIndex].GetID(), TRUE);
			nIndex = nFullIndex;
			return EBCAWR_CanAdd;
		}
		else
		{
			return EBCAWR_CanNotAdd;
		}
	}
}

//------------------------------------------------------------------------------------------------------
// 测试某个普通触发器是否满足条件
// Jason 2010-10-27 v1.2.2
// dwEventMisc1，字段默认是几率加成吧，是妖精特技效果的心情因素开始添加的。单位从0-100/200等，
// 因为是百分比，因此，他要被除于100.
//------------------------------------------------------------------------------------------------------
BOOL Unit::TestTrigger(Unit* pTarget, const tagTriggerProto* pProto, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	if( !P_VALID(pProto) ) return TRUE;

	// 检查一下该触发器是否有被动技能影响
	TList<DWORD>* pListModifier = m_mapTriggerModifier.Peek(pProto->dwID);

	if( !P_VALID(pListModifier) || pListModifier->Empty() )
	{
		// 没有影响，直接计算
		return TestEventTrigger(pTarget, pProto, NULL, dwEventMisc1, dwEventMisc2) &&
			   TestStateTrigger(pTarget, pProto, NULL);
	}
	else
	{
		// 计算触发器影响
		tagTriggerMod mod;

		TList<DWORD>::TListIterator it = pListModifier->Begin();
		DWORD dwSkillTypeID = GT_INVALID;

		while( pListModifier->PeekNext(it, dwSkillTypeID) )
		{
			const tagSkillProto* pSkillProto = g_attRes.GetSkillProto(dwSkillTypeID);
			if( !P_VALID(pSkillProto) ) continue;

			mod.SetMod(pSkillProto);
		}

		// 触发器影响计算完毕，开始计算触发器结果
		return TestEventTrigger(pTarget, pProto, &mod, dwEventMisc1, dwEventMisc2) &&
			   TestStateTrigger(pTarget, pProto, &mod);
	}
}

//----------------------------------------------------------------------------------------------------------
// 测试某个触发器的事件条件是否满足
// Jason 2010-10-27 v1.2.2
// dwEventMisc1，字段默认是几率加成吧，是妖精特技效果的心情因素开始添加的。单位从0-100/200等，
// 因为是百分比，因此，他要被除于100.
//----------------------------------------------------------------------------------------------------------
BOOL Unit::TestEventTrigger(Unit* pTarget, const tagTriggerProto* pProto, const tagTriggerMod* pMod, DWORD dwEventMisc1, DWORD dwEventMisc2)
{
	// 没有事件要求，则直接返回成功
	if( ETEE_Null == pProto->eEventType ) return TRUE;

	// 不需要判断触发率，则直接返回成功
	if (m_bTriggerOff == TRUE)
		return TRUE;

	INT nPctAdd = 0;
	if( dwEventMisc1 != GT_INVALID )
		nPctAdd = (INT)dwEventMisc1;

	// 接下来测试几率
	INT nProp = pProto->nEventProp + ( P_VALID(pMod) ? pMod->nPropMod : 0 );
	if( nPctAdd > 0 )
		nProp = (FLOAT)nProp * ((FLOAT)nPctAdd / 100);
	if( IUTIL->Rand() % 10000 > nProp ) return FALSE;

	// 接下来根据某些特殊的事件类型来进行判断，目前没有需要特殊判断的

	return TRUE;
}

//------------------------------------------------------------------------------------------------------------
// 测试某个触发器的状态条件是否满足
//------------------------------------------------------------------------------------------------------------
BOOL Unit::TestStateTrigger(Unit* pTarget, const tagTriggerProto* pProto, const tagTriggerMod* pMod)
{
	if( ETEE_Null == pProto->eStateType ) return TRUE;

	DWORD dwStateMisc1 = pProto->dwStateMisc1 + ( P_VALID(pMod) ? pMod->nStateMisc1Mod : 0 );
	DWORD dwStateMisc2 = pProto->dwStateMisc2 + ( P_VALID(pMod) ? pMod->nStateMisc2Mod : 0 );

	// 查看具体的trigger类型做出判断
	switch(pProto->eStateType)
	{
		// 体力高于
	case ETST_HPHigher:
		{
			INT nHPLimit = (INT)dwStateMisc1;

			if( nHPLimit < 100000 )
			{
				return GetAttValue(ERA_HP) > nHPLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxHP) > 0 )
					return (INT)((FLOAT)GetAttValue(ERA_HP) / (FLOAT)GetAttValue(ERA_MaxHP) * 10000)
									>= (nHPLimit - 100000);
				else
					return FALSE;
			}
		}
		break;

		// 体力低于
	case ETST_HPLower:
		{
			INT nHPLimit = (INT)dwStateMisc1;

			if( nHPLimit < 100000 )
			{
				return GetAttValue(ERA_HP) < nHPLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxHP) > 0 )
				{
					return (INT)((FLOAT)GetAttValue(ERA_HP) / (FLOAT)GetAttValue(ERA_MaxHP) * 10000) 
										< (nHPLimit - 100000);
				}
				else
					return FALSE;
			}
		}
		break;

		// 真气高于
	case ETST_MPHigher:
		{
			INT nMPLimit = (INT)dwStateMisc1;

			if( nMPLimit < 100000 )
			{
				return GetAttValue(ERA_MP) > nMPLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxMP) > 0 )
					return (INT)((FLOAT)GetAttValue(ERA_MP) / (FLOAT)GetAttValue(ERA_MaxMP) * 10000)
									>= (nMPLimit - 100000);
				else
					return FALSE;
			}
		}
		break;

		// 真气低于
	case ETST_MPLower:
		{
			INT nMPLimit = (INT)dwStateMisc1;

			if( nMPLimit < 100000 )
			{
				return GetAttValue(ERA_MP) < nMPLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxMP) > 0 )
					return (INT)((FLOAT)GetAttValue(ERA_MP) / (FLOAT)GetAttValue(ERA_MaxMP) * 10000)
									< (nMPLimit - 100000);
				else
					return FALSE;
			}
		}
		break;

		// 怒气高于
	case ETST_RageHigher:
		{
			INT nRageLimit = (INT)dwStateMisc1;
			return GetAttValue(ERA_Rage) > nRageLimit;
		}
		break;

		// 怒气低于
	case ETST_RageLower:
		{
			INT nRageLimit = (INT)dwStateMisc1;
			return GetAttValue(ERA_Rage) < nRageLimit;
		}
		break;

		// 持久力高于
	case ETST_EnduranceHigher:
		{
			INT nEnduranceLimit = (INT)dwStateMisc1;

			if( nEnduranceLimit < 100000 )
			{
				return GetAttValue(ERA_Endurance) > nEnduranceLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxEndurance) > 0 )
					return (INT)((FLOAT)GetAttValue(ERA_Endurance) / (FLOAT)GetAttValue(ERA_MaxEndurance) * 10000)
									>= (nEnduranceLimit - 100000);
				else
					return FALSE;
			}
		}
		break;

		// 持久力低于
	case ETST_EnduranceLower:
		{
			INT nEnduranceLimit = (INT)dwStateMisc1;

			if( nEnduranceLimit < 100000 )
			{
				return GetAttValue(ERA_Endurance) < nEnduranceLimit;
			}
			else
			{
				if( GetAttValue(ERA_MaxEndurance) > 0 )
					return (INT)((FLOAT)GetAttValue(ERA_Endurance) / (FLOAT)GetAttValue(ERA_MaxEndurance) * 10000)
									< (nEnduranceLimit - 100000);
				else
					return FALSE;
			}
		}
		break;

		// 自身拥有某buff
	case ETST_SelfHaveBuff:
		{
			DWORD dwBuffID = dwStateMisc1;
			return IsHaveBuff(dwBuffID);
		}
		break;

		// 目标拥有某buff
	case ETST_TargetHaveBuff:
		{
			DWORD dwBuffID = dwStateMisc2;
			if( !P_VALID(pTarget) ) return FALSE;

			return pTarget->IsHaveBuff(dwBuffID);
		}
		break;

	default:
		break;
	}

	return FALSE;
}

//----------------------------------------------------------------------------------
// 添加怪物到该玩家仇恨的怪物列表
//----------------------------------------------------------------------------------
VOID Unit::AddEnmityCreature(Unit *pUnit)
{
	m_mapEnmityCreature.Add(pUnit->GetID(), pUnit->GetID());
}

//----------------------------------------------------------------------------------
// 删除怪物从该玩家仇恨的怪物列表
//----------------------------------------------------------------------------------
VOID Unit::DelEnmityCreature(Unit *pUnit)
{
	m_mapEnmityCreature.Erase(pUnit->GetID());
}

//----------------------------------------------------------------------------------
// 清空玩家仇恨的怪物列表
//----------------------------------------------------------------------------------
VOID Unit::ClearEnmityCreature()
{
	DWORD	dwCreatureID = GT_INVALID;

	Creature* pCreature = (Creature*)GT_INVALID;
	m_mapEnmityCreature.ResetIterator();
	while (m_mapEnmityCreature.PeekNext(dwCreatureID))
	{
		pCreature = GetMap()->FindCreature(dwCreatureID);
		if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()))
			continue;

		pCreature->GetAI()->ClearEnmity(GetID());
	}

	m_mapEnmityCreature.Clear();
}

//----------------------------------------------------------------------------------
// 按照百分比改变仇恨
//----------------------------------------------------------------------------------
VOID Unit::ChangeEnmityCreatureValue(DWORD dwRate)
{
	DWORD	dwCreatureID	=	GT_INVALID;
	INT		nEnmityMod		=	0;
	INT		nEnmityTotal	=	0;

	Creature* pCreature = (Creature*)GT_INVALID;
	m_mapEnmityCreature.ResetIterator();
	while (m_mapEnmityCreature.PeekNext(dwCreatureID))
	{
		pCreature = GetMap()->FindCreature(dwCreatureID);
		if( !P_VALID(pCreature) || !P_VALID(pCreature->GetAI()))
			continue;

		nEnmityTotal = pCreature->GetAI()->GetBaseEnmityValue(GetID());
		nEnmityMod = (FLOAT)nEnmityTotal * (10000.0 - (FLOAT)dwRate) / 10000.0;
		if( 0 == dwRate )
			pCreature->GetAI()->AddEnmity(this, -nEnmityMod, FALSE,TRUE);
		else
			pCreature->GetAI()->AddEnmity(this, -nEnmityMod, FALSE);
	}
}

FLOAT Unit::GetXZSpeed()
{
	if (this->IsRole())
	{
		Role* pRole = static_cast<Role*>(this);
		if (pRole->IsInRoleStateAny(ERS_Mount | ERS_Mount2))
		{
			return m_fMountXZSpeed;
		}
		else
		{
			return m_fXZSpeed;
		}
	}
	else
	{
		return m_fXZSpeed;
	}
}

VOID Unit::AddTargetBy(Role *pRole)
{
	m_mapTargetBy.Add(pRole->GetID(), pRole);
	SyncTargetOfTargetTo(pRole);
	if (m_mapTargetBy.Size() > 100)
	{
		IMSG(_T("AddTargetBy reached 100!\r\n"));
	}
}

VOID Unit::DelTargetBy(Role *pRole)
{
	m_mapTargetBy.Erase(pRole->GetID());
}

VOID Unit::ClearTargetBy()
{
	m_mapTargetBy.Clear();
}

VOID Unit::ChangeTarget(Unit *pUnit, BOOL bSync)
{
	// 清除上次选中目标的被选中列表,你的状态不用告诉哥了
	if (IsRole())
	{
		DWORD dwLastTarget = GetAttValue(ERA_Target);
		Map* pMap = GetMap();
		Unit* pLastUnit = NULL;
		if (P_VALID(pMap))
		{
			pLastUnit = pMap->FindUnit(dwLastTarget);
		}

		if (P_VALID(pLastUnit))
		{
			pLastUnit->DelTargetBy((Role*)this);
		}
	}

	if (P_VALID(pUnit))
	{
		// 选中某人，不为怪物的时候将对方的目标同步，并将自己加入到同步列表中去
		SetAttValue(ERA_Target, pUnit->GetID(), bSync);
		if (IsRole())
		{
			pUnit->SyncTargetOfTargetTo((Role*)this);
			pUnit->AddTargetBy((Role*)this);
		}
	}
	else
	{
		// 取消选中状态
		SetAttValue(ERA_Target, GT_INVALID, bSync);
	}

	// 看着哥的人注意，哥状态改变了
	DWORD dwRoleID = GT_INVALID;
	Role* pRole = (Role*)GT_INVALID;
	m_mapTargetBy.ResetIterator();
	while (m_mapTargetBy.PeekNext(dwRoleID, pRole))
	{
		Map* pMap = GetMap();
		Role* pLastRole = NULL;
		if (P_VALID(pMap))
		{
			pLastRole = pMap->FindRole(dwRoleID);
		}

		if (!P_VALID(pLastRole) || pLastRole != pRole || !pLastRole->IsRole())
		{
			m_mapTargetBy.Erase(dwRoleID);
			continue;
		}

		SyncTargetOfTargetTo(pRole);
	}
}

VOID Unit::SyncTargetOfTargetTo(Role *pRole)
{
	if (!P_VALID(pRole))
	{
		return;
	}
	tagNS_TargetOfTarget msg;

	msg.dwPlayerID = GetID();
	msg.dwTargetID = GetAttValue(ERA_Target);
	pRole->SendMessage(&msg,msg.dwSize);
}

//----------------------------------------------------------------------------------
// 击飞:只对怪物
//----------------------------------------------------------------------------------
BOOL Unit::HitFly(Unit* pTarget)
{
	if(!P_VALID(pTarget))
		return FALSE;

	// 目标已经死亡，直接返回
	if( pTarget->IsDead() )
		return FALSE;

	// 以8格子为为击飞的最远距离
	FLOAT fDistAbs = FLOAT(8 * TILE_SCALE);	// 绝对距离

	// 得到两个目标之间的向量
	Vector3 vVec = GetCurPos() - pTarget->GetCurPos();

	// 假如玩家和怪物在同一点上
	if ( GetCurPos() == pTarget->GetCurPos())
	{

		//vDest = pTarget->GetCurPos() + fDistAbs;
		vVec = pTarget->GetFaceTo();
	}

	// 对该向量进行归一化
	Vec3Normalize(vVec);

	// 得到终点坐标
	Vector3 vDest = pTarget->GetCurPos() + (-1) * vVec * fDistAbs;

	// 得到一个最大合理的终点
	Vector3 vRealDest;
	NavCollider* pCollider = pTarget->GetMap()->GetNavMap()->GetCollider();

	Creature* pTargetCreature = static_cast<Creature*>(pTarget);

	// 非碰撞怪
	if( !pTargetCreature->NeedCollide() )
	{
		POINT nearPos;
		if( !pTargetCreature->GetMap()->IfCanDirectGo(pTargetCreature->GetCurPos().x, pTargetCreature->GetCurPos().z, vDest.x, vDest.z, &nearPos) )
		{
			vRealDest.x = FLOAT(nearPos.x * TILE_SCALE);
			vRealDest.z = FLOAT(nearPos.y * TILE_SCALE);
			vRealDest.y = pTargetCreature->GetMap()->GetGroundHeight(vRealDest.x, vRealDest.z);
		}
		else
		{
			vRealDest = vDest;
		}
	}
	// 碰撞怪
	else
	{
		NavCollider_NPCMove collider;
		collider.IfCanGo(pCollider, pTarget->GetCurPos(), vDest, pTarget->GetSize(), &vRealDest);
	}

	// 如果两个点不相等,发送击飞消息
	if( pTarget->GetCurPos() != vRealDest )
	{
		// 给客户端发送消息
		tagNS_HitFly send;
		send.dwRoleID    = pTarget->GetID();				// 目标ID
		send.curPos      = pTarget->GetCurPos();			// 目标当前坐标
		send.destPos	 = vRealDest;						// 目标的终点
		send.faceTo	     = vVec;							// 击飞的朝向
		send.dwSkillID   = GetCombatHandler().GetSkillID();	// 使用技能ID
		send.dwSrcRoleID = GetID();							// 发起者ID
		send.bCollide    = 1;								// 需要碰撞检测

		// 瞬移，但不发送消息
		pTarget->GetMoveData().ForceTeleport(vRealDest, FALSE);

		pTarget->GetMap()->SendBigVisTileMsg(pTarget, &send, send.dwSize);
		
		// 结束当前释放的技能
		pTarget->GetCombatHandler().End();
	}
	
	return TRUE;
}


//----------------------------------------------------------------------------------
// 开启和关闭宏状态后对隐匿玩家的特殊处理
//----------------------------------------------------------------------------------
BOOL Unit::OpenMacroToDealWithLuckState()
{
	// 是都在隐匿状态
	if ( IsInStateInvisible() )
	{
		Map* pMap = GetMap();
		if( !P_VALID(pMap) )
			return FALSE;
		
		// 得到九宫格内的所有玩家
		tagVisTile* pVisTile[ED_End] = {0};

		// 获得周围九宫格的可视地砖
		pMap->GetVisTile( GetVisTileIndex(), pVisTile);
		

		// 同步给可视列表内玩家
		tagNS_RemoveRemote sendRemove;
		sendRemove.dwRoleID[0] = GetID();

		for(INT n = 0; n < ED_End; n++)
		{
			if( NULL == pVisTile[n] )
				continue;
			
			// 找到每个地砖的人
			TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
			mapRole.ResetIterator();
			Role* pRole = NULL;

			while( mapRole.PeekNext(pRole) )
			{
				
				if( !P_VALID(pRole) )
					continue;

				// 好友不处理
				if( pRole->CanSeeTargetEntirely(this) )
				{
					continue;
				}
				
				// 完全透明处理
				if( pRole->IsInVisDist(this) || !pRole->IsInVisDistForOpenMacre(this))
				{
					pRole->RemoveFromVisList(GetID());
					pRole->SendMessage(&sendRemove, sendRemove.dwSize);
				}
			}
		}
		

	}

	return  TRUE;
}

BOOL Unit::CloseMacroToDealWithLuckState()
{
	// 是都在隐匿状态
	if ( IsInStateInvisible() )
	{
		Map* pMap = GetMap();
		if( !P_VALID(pMap) )
			return FALSE;

		BYTE	byMsg[1024] = {0};
		DWORD	dwSize = pMap->CalMovementMsgEx(this, byMsg, 1024);
		if( 0 == dwSize  ) return FALSE;


		// 得到九宫格内的所有玩家
		tagVisTile* pVisTile[ED_End] = {0};

		// 获得周围九宫格的可视地砖
		pMap->GetVisTile( GetVisTileIndex(), pVisTile);

		for(INT n = 0; n < ED_End; n++)
		{
			if( NULL == pVisTile[n] )
				continue;

			// 找到每个地砖的人
			TMap<DWORD, Role*>& mapRole = pVisTile[n]->mapRole;
			mapRole.ResetIterator();
			Role* pRole = NULL;

			while( mapRole.PeekNext(pRole) )
			{

				if( !P_VALID(pRole) )
					continue;

				// 好友不处理
				if( pRole->CanSeeTargetEntirely(this) )
				{
					continue;
				}
				
				//  回复半透明
				if(  pRole->IsInVisDist(this) || !pRole->IsInVisDistForOpenMacre(this))
				{
					pRole->Add2VisList(GetID());
					pRole->SendMessage(byMsg, dwSize);
				}
				
			}
		}
	
	}

	return TRUE;
}

//----------------------------------------------------------------------------------
// 判断自身和目标之间的射线检测是否通过
//----------------------------------------------------------------------------------
BOOL Unit::IsRayCollide(Unit& target)
{
	Map* pMap = GetMap();
	if( !P_VALID(pMap) || !P_VALID(pMap->GetNavMap()) )
		return FALSE;

	Vector3 vSrc = GetCurPosTop();
	Vector3 vDest = target.GetCurPosTop();

	// 如果是门
	if (target.IsCreature())
	{
		Unit* pUnit = &target;
		Creature* pCreature = static_cast<Creature*>(pUnit);
		const tagCreatureProto* pProto = pCreature->GetProto();
		if (P_VALID(pProto) && pProto->nType2 == EMTT_Door)
		{
			Ray ray;
			ray.origin = vSrc;
			ray.dir = vDest - vSrc;
			ray.length = Vec3Dist(vSrc, vDest);
			Vec3Normalize(ray.dir);

			if( pMap->GetNavMap()->GetCollider()->RayCollideBoxAndTrn(ray, NavSceneNode::EFlag_NotDoor) )
			{
				return TRUE;
			}

			return FALSE;
		}
	}

	if( pMap->IsRayCollide(vSrc, vDest) )
	{
		return TRUE;
	}

	return FALSE;
}

FLOAT Unit::CalAdditionalDmg(Skill * pSkill)
{
	return 0;
}
FLOAT Unit::CalAdditionalResistance(Skill * pSkill)
{
	return 0;
}
//---------------------------------------------------------------------------------
VOID Unit::AddSpecSkill(Skill *pSkill)
{
	if(!P_VALID(pSkill))
		return;

	if( pSkill->GetSpecFunc()> ESSF_NULL &&  pSkill->GetSpecFunc() < ESSF_END )
	{
		if(pSkill->IsPassive())
			m_vecSpecSkillID.push_back(pSkill->GetID());
	}
}
//---------------------------------------------------------------------------------
INT Unit::GetSpecSkillValue(ESkillSpecFunc eSkillSpec, INT nVal1, INT nVal2/* = 0*/)
{//不是一种好的实现方法，暂时这样
	if(eSkillSpec <= ESSF_NULL && eSkillSpec >= ESSF_END)
		return 0;

	INT nRet = 0;
	//INT nMin = -1;
	vector<DWORD>::iterator it;
	for( it = m_vecSpecSkillID.begin(); it != m_vecSpecSkillID.end(); ++it )
	{
		Skill *pSkill = m_mapSkill.Peek(*it);

		if(P_VALID(pSkill) && pSkill->IsPassive() && pSkill->GetCanUseByMap())
		{
			if(eSkillSpec == pSkill->GetSpecFunc())
			{
				switch(eSkillSpec)
				{
				case ESSF_Strengthen:				// 强化成功率相关
					{
						INT nSpecFuncVal1 = pSkill->GetSpecFuncVal1();
						if( nSpecFuncVal1 >= nVal1 )
						{
							nRet += pSkill->GetSpecFuncVal2();
						}
					}
					break;
				case ESSF_Valicity:					// 活力消耗相关
					{
						ESkillSpecVitality eSkillSpecVitality = (ESkillSpecVitality)pSkill->GetSpecFuncVal1();
						INT nValue2 = 0;
						switch(eSkillSpecVitality)
						{
						case ESSV_ALL:
							{
								nValue2 = pSkill->GetSpecFuncVal2();
							}
							break;
						case ESSV_COMPOSE:
							{
								if(  ESSV_COMPOSE == (ESkillSpecVitality)nVal1 )
									nValue2 = pSkill->GetSpecFuncVal2();
							}
							break;
						case ESSV_CONSOLODATE:
							{
								if(  ESSV_CONSOLODATE == (ESkillSpecVitality)nVal1 )
									nValue2 = pSkill->GetSpecFuncVal2();
							}
							break;
						}

						if( nValue2 != 0 )
						{
							if( nValue2 > 100000 )
							{
								nValue2 = (nValue2 > 0 ? 1 : -1) * INT((FLOAT)nVal2 * (FLOAT(abs(nValue2) - 100000) / 10000.0f));
							}

							//if( -1 == nMin || nMin > nValue2 )
							//{
							//	nMin = nValue2;
							//}

							//nRet = nMin;

							return nValue2;
						}
					}
					break;
				case ESSF_FabaoExp:					// 妖精经验相关
					{
						INT nSpecFuncVal1 = pSkill->GetSpecFuncVal1();
						INT nValue = 0;
						if(nSpecFuncVal1 > 100000 || nSpecFuncVal1 < -100000)
						{
							//nValue = (nSpecFuncVal1 > 0 ? 1 : -1) * INT( (FLOAT)nVal1 * ((FLOAT(abs(nSpecFuncVal1)) - 100000)/10000.0f) ) + nVal1;

							//FLOAT fRet = ((nSpecFuncVal1 > 0 ? 1 : -1) * (FLOAT( abs(nSpecFuncVal1) - 100000))/10000.0f) + 0.2f;
							nValue = (nSpecFuncVal1 > 0 ? 1 : -1) * ( nVal1 * (abs(nSpecFuncVal1) - 100000 + 2000) / 10000 );
							//nValue = nVal1 + nRet;	
						}
						else
						{
							nValue = nSpecFuncVal1 + nVal1 * 0.2f;
						}

						//应策划要求，不会出现多个同类被动技能同时存在的问题，所以直接返回
						return nValue;
					}
					break;
				case ESSF_Gem:						// 宝石升级相关
					{
						INT nValue = 0;
						INT nSpecFuncVal1 = pSkill->GetSpecFuncVal1();
						tagItemProto *pProto = g_attRes.GetItemProto(nVal1);
						if(!P_VALID(pProto))
							return 0;

						if((INT)pProto->byLevel-1 == nSpecFuncVal1)
						{
							INT nSpecFuncVal2 = pSkill->GetSpecFuncVal2();
							if(nSpecFuncVal2 > 100000)
							{
								//nValue = nSpecFuncVal2 - 100000;
								nRet += (nSpecFuncVal2 - 100000);
							}
						}

						//应策划要求，不会出现多个同类被动技能同时存在的问题，所以直接返回
						//return nValue;
					}
					break;
				case ESSF_Brand:					// 圣印升级相关
					{
						INT nValue = 0;
						INT nSpecFuncVal1 = pSkill->GetSpecFuncVal1();
						INT nValue1 = nVal1 % 7;
						nValue1 %= 5;
						if(nValue1 == 0)
							nValue1 = 5;

						if(nValue1 == nSpecFuncVal1)
						{
							INT nSpecFuncVal2 = pSkill->GetSpecFuncVal2();
							if(nSpecFuncVal2 > 100000)
							{
								nValue = nSpecFuncVal2 - 100000;
							}
						}

						//应策划要求，不会出现多个同类被动技能同时存在的问题，所以直接返回
						return nValue;
					}
					break;
				case ESSF_SkillMap:					// 技能地图限制相关	
					{
					}
					break;
				case ESSF_Strengthen1:				// 强化成功率相关
					{
						INT nSpecFuncVal1 = pSkill->GetSpecFuncVal1();
						if( nSpecFuncVal1 <= nVal1 )
						{
							nRet += pSkill->GetSpecFuncVal2();
						}
					}
					break;
				case ESSF_FreeOpenFace:					// 随机开界面	
					{
					}
					break;
				}
			}
		}
	}

	//没有该技能直接返回
	switch(eSkillSpec)
	{
	case ESSF_Valicity:
		{
			return nVal2;
		}
		break;
	case ESSF_FabaoExp:
		{
			return nVal1 * 0.2f;
		}
		break;
	}


	return nRet;
}
//---------------------------------------------------------------------------------
VOID Unit::RemoveSpecSkill(DWORD dwSkillID)
{
	vector<DWORD>::iterator it;
	for( it = m_vecSpecSkillID.begin(); it != m_vecSpecSkillID.end(); ++it )
	{
		if(dwSkillID == *it)
		{
			m_vecSpecSkillID.erase(it);
			return;
		}
	}
}
//---------------------------------------------------------------------------------
//设置由家族妖精共鸣产生的属性加成
VOID Unit::SetRoleFamilySpriteAtt( FLOAT fGongming, INT *pnAbility )
{
	if( !P_VALID(pnAbility) ) 
		return;

	//INT nOldAbility[EFSA_Num] = {0};
	for( int i = 0; i < EFSA_Num; ++i )
	{
		if( pnAbility[i] >= 0)
		{
			m_nOldAbility[i] = m_nAbility[i];
			m_nAbility[i] = pnAbility[i] * fGongming;
			if(m_nOldAbility[i] != m_nAbility[i])
			{
				SetRecalAbility((EFamilySpriteAtt)i,TRUE);
				ERoleAttribute eType = GetRoleIndexFromFamilySprite((EFamilySpriteAtt)i);
				SetAttRecalFlag(eType);
			}
		}
		else
		{
			m_nAbility[i] = pnAbility[i];
		}
	}
}
//---------------------------------------------------------------------------------
VOID Unit::ClearRoleFamilySpriteAtt()
{
	for( int i = 0; i < EFSA_Num; ++i )
	{
		if( m_nAbility[i] >= 0)
		{
			m_nOldAbility[i] = m_nAbility[i];

			if(m_nAbility[i] != -1)
				m_nAbility[i] = 0;

			if(m_nOldAbility[i] != m_nAbility[i])
			{
				SetRecalAbility((EFamilySpriteAtt)i,TRUE);
				ERoleAttribute eType = GetRoleIndexFromFamilySprite((EFamilySpriteAtt)i);
				SetAttRecalFlag(eType);
			}
		}
	}
}
//---------------------------------------------------------------------------------
ERoleAttribute Unit::GetRoleIndexFromFamilySprite(EFamilySpriteAtt eType)
{
	switch(eType)
	{
	case EFSA_HP:
		{
			return ERA_MaxHP;
		}
	case EFSA_EXAttack:
		{
			return ERA_ExAttack;
		}
	case EFSA_InAttack:
		{
			return ERA_InAttack;
		}
	case EFSA_EXDefense:
		{
			return ERA_ExDefense;
		}
	case EFSA_InDefense:
		{
			return ERA_InDefense;
		}
	case EFSA_EXAttackDeeper:
		{
			return ERA_transform_ExAttack;
		}
	case EFSA_InAttackDeeper:
		{
			return ERA_transform_InAttack;
		}
	case EFSA_EXAttackResistance:
		{
			return ERA_Derate_ExAttack;
		}
	case EFSA_InAttackResistance:
		{
			return ERA_Derate_InAttack;
		}
	case EFSA_Toughness:
		{
			return ERA_Toughness_god;
		}
	case EFSA_CritDes:
		{
			return ERA_Toughness_strength;
		}
	case EFSA_ControleffectDeepen:
		{
			return ERA_Contrel_effect_deepen;
		}
	case EFSA_ControleffectResistance:
		{
			return ERA_Resist_Choas;
		}
	case EFSA_SlowingeffectDeepen:
		{
			return ERA_Slowing_effect_strengthening;
		}
	case EFSA_SlowingeffectResistance:
		{
			return ERA_Resist_Special;
		}
	case EFSA_FixedeffectDeepen:
		{
			return ERA_Fixed_damage_aggrandizement;
		}
	case EFSA_FixedeffectResistance:
		{
			return ERA_Resist_Bleeding;
		}
	case EFSA_AgingeffectDeepen:
		{
			return ERA_Aging_effect_strengthening;
		}
	case EFSA_AgingeffectResistance:
		{
			return ERA_Resist_Weak;
		}
	}

	return ERA_Null;
}
//---------------------------------------------------------------------------------
// 更新
//----------------------------------------------------------------------------------
VOID Unit::UpdateSkill()
{
	if( !m_listSkillCoolDown.Empty() )
	{
		DWORD dwSkillID = GT_INVALID;
		TList<DWORD>::TListIterator it = m_listSkillCoolDown.Begin();
		while( m_listSkillCoolDown.PeekNext(it, dwSkillID) )
		{
			Skill* pSkill = m_mapSkill.Peek(dwSkillID);
			if( P_VALID(pSkill) && pSkill->CountDownCoolDown() )
			{
				m_listSkillCoolDown.Erase(dwSkillID);
			}
		}
	}

	if( !m_listSkillActiveTime.Empty() )
	{
		DWORD dwSkillID = GT_INVALID;
		TList<DWORD>::TListIterator it = m_listSkillActiveTime.Begin();
		while( m_listSkillActiveTime.PeekNext(it, dwSkillID) )
		{
			Skill* pSkill = m_mapSkill.Peek(dwSkillID);
			if( P_VALID(pSkill) && pSkill->CountActiveTime() )
			{
				if(IsRole())
				{
					Role* pRole = reinterpret_cast<Role*>(this);
					//tagNS_ActiveSoulSkill send;
					//send.dwSkillID = pSkill->GetTypeID();	//技能ID
					//send.dwResult = E_Success;				//结果
					//send.byOpt = 1;
					//pRole->SendMsg(send,send.dwSize);
					pRole->RemoveSkill(pSkill->GetID());
				}
				pSkill->SetActiveTime(0);
				m_listSkillActiveTime.Erase(dwSkillID);
			}
		}
	}
}
//----------------------------------------------------------------------------------






