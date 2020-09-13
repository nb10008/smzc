#include "StdAfx.h"
#include "ActionSkillAttack_NPC.h"
#include "NPC.h"
#include "RoleMgr.h"
#include "CombatEvent.h"
#include "SkillProtoData.h"
#include "EffectMgr.h"
#include "AudioSys.h"
#include "CreatureData.h"
#include "CombatSysUtil.h"
#include "NPCSkillEffectData.h"
ActionSkillAttack_NPC::ActionSkillAttack_NPC(void)
{
	m_bClosed=true;
	m_dwSerial=0;
}

ActionSkillAttack_NPC::~ActionSkillAttack_NPC(void)
{}

void ActionSkillAttack_NPC::Active( NPC* pRole,Role* pTarget,DWORD skillID,DWORD dwSerial )
{	
	m_bClosed=false;

	//--记录技能ID
	m_skillID=skillID;
	m_targetID=pTarget ? pTarget->GetID() : GT_INVALID;
	m_dwSerial=dwSerial;

	//--获取技能属性
	const tagSkillProtoClient* pSkillProto=SkillProtoData::Inst()->FindSkillProto(skillID);
	//THROW_NULL(pSkillProto,_T("cannot find skillproto"));
	if( !P_VALID(pSkillProto) )
	{
		m_bClosed=true;
		return;
	}

	m_trackName=pSkillProto->szActionMod;
	m_dmgTimes=pSkillProto->nDmgTimes;
	m_bSingle=pSkillProto->fOPRadius==0;

	//--保存特效/音效数据（避免重复查表）
	if (!P_VALID(pRole->GetProto()))
		return;

    const tagEffectSet& sfxset = NPCSkillEffectData::Inst()->GetNPCEffectSet(pRole->GetProto()->szModelName, pRole->GetTypeID(), m_skillID, false);
    m_vecAtkSfx.resize(sfxset.model_node.size());
    m_vecAtkSfxTagNode.resize(sfxset.model_node.size());
    for(size_t i = 0; i<sfxset.model_node.size(); ++i)
    {
        m_vecAtkSfxTagNode[i] = sfxset.model_node[i].first;
        m_vecAtkSfx[i] = sfxset.model_node[i].second;
    }

	m_strLchSfx=pSkillProto->szLchSfx;
	m_strLchSfxTagNode=CreatureData::Inst()->FindNpcBoneName( pRole->GetTypeID(), pSkillProto->szTagLchSfx );
	m_vecAttackEffectID.resize(m_vecAtkSfxTagNode.size(),GT_INVALID);
	m_strAtkSound=pSkillProto->szSound;

	//--播放攻击动作
	pRole->PlayTrack(m_trackName.c_str(),false,true);

	//--自动朝向目标
	if(pTarget)
	{
		Vector3 dir=pTarget->GetPos()-pRole->GetPos();
		pRole->SetYaw(CalcYaw(dir));
	}

	//--播放攻击叫喊音效
	if(pSkillProto->bHostile)
	{
		if( m_trackName==_T("natk") )
		{
			//--普通攻击10%几率播放
			if( 0 == rand() % 10 )
				CombatSysUtil::Inst()->PlayMonsterRoarSound(pRole);
		}
		else
		{
			CombatSysUtil::Inst()->PlayMonsterRoarSound(pRole);
		}
	}
}

void ActionSkillAttack_NPC::Update(NPC* pRole,Role* pTarget)
{
	if(m_bClosed)
		return;

	const vector<DWORD>& msgCodes = pRole->GetMsgCodes();
	for( size_t nMsgCode = 0; nMsgCode < msgCodes.size(); nMsgCode++ )
	{
		OnMsgCode(pRole,msgCodes[nMsgCode]);
	}

	//--
	if(pRole->IsTrackEnd(m_trackName.c_str()))
	{
		m_bClosed=true;
		StopAtkSfx();
		return;
	}
}

void ActionSkillAttack_NPC::Stop(NPC* pRole)
{
	if(m_bClosed)
	{
		return;
	}

	//--发送命中目标事件
	tagHitTargetEvent event(_T("tagHitTargetEvent"),NULL);
	event.dwSrcRoleID=pRole->GetID();
	event.dwTarRoleID=m_targetID;
	event.bSingle=m_bSingle;
	event.bLast=true;
	event.eCause=EHTC_Skill;
	event.dwMisc=m_skillID;
	event.dwMisc2=max(m_dmgTimes-1,0);
	event.dwSerial=m_dwSerial;
	m_pGameFrameMgr->SendEvent(&event);

	m_bClosed=true;

	//--停止特效
	StopAtkSfx();
}

void ActionSkillAttack_NPC::OnMsgCode(NPC* pRole,const DWORD dwMsgCode)
{
	//--发送命中目标事件
	TCHAR szAniMsg[5];
	ZeroMemory(szAniMsg,sizeof(szAniMsg));
	_FourCC2Str(dwMsgCode,szAniMsg);
	if(_tcsncmp(szAniMsg,_T("bak"),3)==0)
	{
		int channel=_tstoi(&szAniMsg[3]);

		tagHitTargetEvent event(_T("tagHitTargetEvent"),NULL);
		event.dwSrcRoleID=pRole->GetID();
		event.dwTarRoleID=m_targetID;
		event.bSingle=m_bSingle;
		event.bLast=(channel+1>=m_dmgTimes);
		event.eCause=EHTC_Skill;
		event.dwMisc=m_skillID;
		event.dwMisc2=channel;
		event.dwSerial=m_dwSerial;
		m_pGameFrameMgr->SendEvent(&event);
	}

	//--播放/停止攻击特效
	if(_tcsncmp(szAniMsg,_T("atk"),3)==0 )
	{
		if( _T('e') != szAniMsg[3] )
        {
            for( size_t nEff = 0; nEff < m_vecAtkSfxTagNode.size(); nEff++ )
            {
                //tstring strBoneName = CreatureData::Inst()->FindNpcBoneName( pRole->GetTypeID(), sfxset.model_node[nEff].first.c_str() );
                m_vecAttackEffectID[nEff]=EffectMgr::Inst()->PlayRoleEffect(pRole->GetID(),m_vecAtkSfx[nEff].c_str(), m_vecAtkSfxTagNode[nEff].c_str(), EffectMgr::EEST_RoleScale, NULL);
            }
        }
		else
			StopAtkSfx();
	}

	//--播放发射特效
	if(_tcsncmp(szAniMsg,_T("lch"),3)==0)
	{
		int channel=_tstoi(&szAniMsg[3]);

		tagHitTargetEvent event(_T("tagHitTargetEvent"),NULL);
		event.dwSrcRoleID=pRole->GetID();
		event.dwTarRoleID=m_targetID;
		event.eCause=EHTC_Skill;
		event.bSingle=m_bSingle;
		event.bLast=(channel+1>=m_dmgTimes);
		event.dwMisc=m_skillID;
		event.dwMisc2=channel;
		event.dwSerial=m_dwSerial;
		if( m_strLchSfx.empty() )
			TObjRef<GameFrameMgr>()->SendEvent(&event);
		else
			EffectMgr::Inst()->PlayLaunchEffect(event,m_strLchSfx.c_str(),m_strLchSfxTagNode.c_str());

		//--播放攻击音效
		if(m_strAtkSound != _T(""))
			TObjRef<AudioSys>()->Play3DSound(m_strAtkSound.c_str(),100.0f,100.0f*10.0f,pRole->GetPos(),SOUND_WISPER);
	}
}

void ActionSkillAttack_NPC::StopAtkSfx()
{
	for( size_t nEff = 0; nEff < m_vecAttackEffectID.size(); nEff++ )
	{
		if( GT_INVALID != m_vecAttackEffectID[nEff] )
		{
			EffectMgr::Inst()->StopEffect(m_vecAttackEffectID[nEff]);
			m_vecAttackEffectID[nEff] = GT_INVALID;
		}
	}
}