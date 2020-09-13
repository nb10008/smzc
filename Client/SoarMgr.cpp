#include "StdAfx.h"
#include "SoarMgr.h"
#include "SoarSkillFrame.h"
#include "SoarSkillData.h"
#include "CreatureData.h"
#include "QuestEvent.h"
#include "SoarEquipFrame.h"
#include "SkillMgr.h"
#include "..\WorldDefine\msg_miracle.h"
#include "SkillProtoData.h"
#include "CombatSysUtil.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "GodSkillFrame.h"

SoarMgr* SoarMgr::Inst()
{
	static SoarMgr inst;
	return &inst;
}

SoarMgr::SoarMgr(void)
:m_trunk(this)
{
}

SoarMgr::~SoarMgr(void)
{
}

void SoarMgr::RegisterFrames(GameFrameMgr* pMgr)
{
	if (P_VALID(pMgr))
	{
		//pMgr->Register(TWrap<SoarSkillFrame>(),_T("SoarSkillFrame"));
	}
}
void SoarMgr::Init()
{
	tstring strPath = _T("data\\system\\attdata\\soar_skill_proto.xml");
	bool bResult = m_soarSkillData.Init(strPath);
	ASSERT(bResult);

	m_soarCastingData.Init();

	m_pMgr->RegisterEventHandle(_T("Open_Soar"), (FRAMEEVENTPROC)(m_trunk.sfp1(&SoarMgr::OnOpenSoarSkillFrame)));
	m_pMgr->RegisterEventHandle(_T("Close_Soar"), (FRAMEEVENTPROC)(m_trunk.sfp1(&SoarMgr::OnCloseSoarSkillFrame)));
	m_dwCurSkillID = GT_INVALID;
	m_pMgr->RegisterEventHandle( _T("tagStartNPCTalkEvent"),(FRAMEEVENTPROC)(m_trunk.sfp1(&SoarMgr::OnOpenSoarEquipFrame)));

	m_pNetCmdMgr->Register("NS_PracticeSoaringSkill", (NETMSGPROC)(m_trunk.sfp2(&SoarMgr::OnNS_PracticeSoarSkill)), _T("NS_PracticeSoaringSkill"));
	m_pNetCmdMgr->Register("NS_ResetSoarSkillLearnTimes",   (NETMSGPROC)(m_trunk.sfp2(&SoarMgr::OnNS_ResetSoarSkillLearnTimes)), _T("NS_ResetSoarSkillLearnTimes"));
}

void SoarMgr::Destroy()
{
	m_pMgr->UnRegisterEventHandler(_T("Open_Soar"), (FRAMEEVENTPROC)(m_trunk.sfp1(&SoarMgr::OnOpenSoarSkillFrame)));
	m_pMgr->UnRegisterEventHandler(_T("Close_Soar"), (FRAMEEVENTPROC)(m_trunk.sfp1(&SoarMgr::OnCloseSoarSkillFrame)));
	m_pMgr->UnRegisterEventHandler( _T("tagStartNPCTalkEvent"), (FRAMEEVENTPROC)(m_trunk.sfp1(&SoarMgr::OnOpenSoarEquipFrame)));

	m_pNetCmdMgr->UnRegister("NS_PracticeSoaringSkill",   (NETMSGPROC)(m_trunk.sfp2(&SoarMgr::OnNS_PracticeSoarSkill)));
	m_pNetCmdMgr->UnRegister("NS_ResetSoarSkillLearnTimes",   (NETMSGPROC)(m_trunk.sfp2(&SoarMgr::OnNS_ResetSoarSkillLearnTimes)));
	
	m_soarSkillData.Destroy();
	m_soarCastingData.Destroy();
}

const tagSoarSkillData* SoarMgr::GetSoarSkillData(DWORD dwSkillTypeID) const
{
	return m_soarSkillData.GetSoarSkillData(dwSkillTypeID);
}

int SoarMgr::GetSoarSkillNum() const
{
	return m_soarSkillData.GetSoarSkillNum();
}

const std::vector<DWORD>& SoarMgr::GetSoarSkillVector() const
{
	return m_soarSkillData.GetSoarSkillVector();
}

DWORD SoarMgr::OnOpenSoarSkillFrame(tagGameEvent* pEvent)
{
	// 打开飞升技能界面的时候关闭仙界技能界面
	GodSkillFrame* pFrame2 = (GodSkillFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("GodSkill"));
	if(P_VALID(pFrame2))
	{
		m_pMgr->AddToDestroyList(pFrame2);
	}

	SoarSkillFrame* pFrame = (SoarSkillFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("SoarSkillFrame"));
	if (!P_VALID(pFrame))
	{
		m_pMgr->CreateFrame(_T("World"), _T("SoarSkillFrame"), _T("SoarSkillFrame"),0);
	}
	return 0;
}

DWORD SoarMgr::OnOpenSoarEquipFrame(tagStartNPCTalkEvent* pEvt)
{
	const tagCreatureProto* pCreatureProto = CreatureData::Inst()->FindNpcAtt(pEvt->typeID);
	//飞升炼器NPC
	if(pCreatureProto->eFunctionType==EFNPCT_SoarEquipment)
	{
		if (!P_VALID(m_pMgr->GetFrame(_T("SoarEquip"))))
		{
			m_pMgr->CreateFrame(_T("World"), _T("SoarEquip"), _T("SoarEquipFrame"),0);
			SoarEquipFrame* pSoarEquipFrame = (SoarEquipFrame*) m_pMgr->GetFrame(_T("SoarEquip"));
			pSoarEquipFrame->ReceiveNPCID(pEvt->globalID);

		}
		else
		{
			SoarEquipFrame* pSoarEquipFrame = (SoarEquipFrame*) m_pMgr->GetFrame(_T("SoarEquip"));
			pSoarEquipFrame->ShowMixFrame();
			pSoarEquipFrame->ReceiveNPCID(pEvt->globalID);
		}
		
	}
	return 0;
}

const tagSoarCastingProto* SoarMgr::GetSoarCastingData(DWORD dwID) const
{
	return m_soarCastingData.GetSoarCastingProtoData(dwID);
}

const SoarCastingData::SoarCastingDataMap& SoarMgr::GetSoarCastingDataMap() const
{
	return m_soarCastingData.GetSoarCastingDataMap();
}

DWORD SoarMgr::OnCloseSoarSkillFrame(tagGameEvent* pEvent)
{
	SoarSkillFrame* pFrame = (SoarSkillFrame*)m_pMgr->GetFrame(_T("SoarSkillFrame"));
	if (P_VALID(pFrame))
	{
		TObjRef<GameFrameMgr>()->AddToDestroyList(pFrame);	
	}
	return 0;
}

void SoarMgr::SendPracticeSoarSkill(DWORD dwSkillID)
{
	m_dwCurSkillID = dwSkillID;
	const tagSkillData *pSkillData = SkillMgr::Inst()->GetSkillData(dwSkillID);
	if (!P_VALID(pSkillData))
		m_nCurSkillLv = 0;
	else
		m_nCurSkillLv = pSkillData->nLevel;

	tagNC_PracticeSoaringSkill cmd;
	cmd.dwSkillID = dwSkillID;
	m_pSession->Send(&cmd);
}

DWORD SoarMgr::OnNS_PracticeSoarSkill(tagNS_PracticeSoaringSkill* pNetCmd,DWORD)
{
	switch (pNetCmd->dwErrCode)
	{
	case ERSSEC_Success:
		{
			RoleMgr::Inst()->GetLocalPlayer()->SetSoarSkillLearnTimes(static_cast<INT16>(pNetCmd->n16CurPrcNum));

			// 看看技能等级是否提升了
			int nSkillLv = 0;
			const tagSkillData *pSkillData = SkillMgr::Inst()->GetSkillData(m_dwCurSkillID);
			if (!P_VALID(pSkillData))
				nSkillLv = 0;
			else
				nSkillLv = pSkillData->nLevel;
			// 等级提升咯
			if (nSkillLv > m_nCurSkillLv)
			{
				// 取名字
				DWORD dwSkillTypeID = MTransSkillTypeID(m_dwCurSkillID, nSkillLv);
				const tagSkillProtoClient *pProto = SkillProtoData::Inst()->FindSkillProto(dwSkillTypeID);
				if (P_VALID(pProto))
				{
					TCHAR szBuffer[X_LONG_NAME] = {0};
					_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("GodSkill_Success")], pProto->szName, nSkillLv);
					CombatSysUtil::Inst()->ShowScreenCenterMsg(szBuffer);
				}
			}
			// 重置
			m_dwCurSkillID = GT_INVALID;
			// 通知刷新
			m_pMgr->SendEvent(&tagGameEvent(_T("Refresh_SoarSkill"), NULL));
		} 
		break;
	case ERSSEC_TimesLimitted:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ERSSEC_TimesLimitted_Soar")]);
		}
		break;
	case ERSSEC_SilverLimitted:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ERSSEC_SilverLimitted_Soar")]);
		}
		break;
	case ERSSEC_ExpLimitted:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ERSSEC_ExpLimitted_Soar")]);
		}
		break;
	case ERSSEC_ItemLimitted:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ERSSEC_ItemLimitted_Soar")]);
		}
		break;
	case ERSSEC_SkillPracticeTimesLimitted:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ERSSEC_SkillPracticeTimesLimitted_Soar")]);
		}
		break;
	case E_LearnSkill_NeedMoreLevel:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_LearnSkill_NeedMoreLevel_Soar")]);
		}
		break;
	case E_LearnSkill_NoPreSkill:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_LearnSkill_NoPreSkill_Soar")]);
		}
		break;
	case E_LearnSkill_ProtoNotFound:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_LearnSkill_ProtoNotFound_Soar")]);
		}
		break;
	case E_LevelUpSkill_ExceedMaxLearnLevel:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_LevelUpSkill_ExceedMaxLearnLevel_Soar")]);
		}
		break;
	case E_LearnSkill_ClassLimit:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_LearnSkill_ClassLimit_Soar")]);
		}
		break;
	default:
		break;
	}
	return 0;
}

DWORD SoarMgr::GetSoarSkillDatabyPos(int pos)
{
	return m_soarSkillData.GetSoarSkillID(pos);
}

DWORD SoarMgr::OnNS_ResetSoarSkillLearnTimes(tagNS_ResetSoarSkillLearnTimes* pNetCmd,DWORD)
{
	RoleMgr::Inst()->GetLocalPlayer()->SetSoarSkillLearnTimes(pNetCmd->nCurrLeftNum);
	return 0;
}