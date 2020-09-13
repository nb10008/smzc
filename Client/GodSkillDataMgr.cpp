#include "stdafx.h"
#include "GodSkillDataMgr.h"
#include "SkillProtoData.h"
#include "SkillMgr.h"
#include "GodSkillMsgBox.h"
#include "GodSkillFrame.h"
#include "../WorldDefine/msg_miracle.h"
#include "CombatSysUtil.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "SoarSkillFrame.h"


GodSkillDataMgr* GodSkillDataMgr::Inst()
{
	static GodSkillDataMgr inst;
	return &inst;
}

void GodSkillDataMgr::Init()
{
	tstring strPath = _T("data\\system\\attdata\\miracle_skill_proto.xml");
	bool bResult = m_godSkillData.Init(strPath);
	ASSERT(bResult);
	RegisterAllEventHandler();
	RegisterAllNetCmd();
	InitSkills();
	m_dwCurSkillID = GT_INVALID;
}

void GodSkillDataMgr::Destroy()
{
	m_godSkillData.Destroy();
	DestroySkills();
	UnRegisterAllEventHandler();
	UnRegisterAllNetCmd();
}

const tagGodSkillData* GodSkillDataMgr::GetGodSkillData(DWORD dwSkillTypeID) const
{
	return m_godSkillData.GetGodSkillData(dwSkillTypeID);
}

int GodSkillDataMgr::GetGodSkillNum() const
{
	return m_godSkillData.GetGodSkillNum();
}

const GodSkillData::GodSkillIDSet& GodSkillDataMgr::GetGodSkillSet() const
{
	return m_godSkillData.GetGodSkillSet();
}

const std::vector<DWORD>& GodSkillDataMgr::GetSkills(EGodSkillLevel eLv) const
{
	switch (eLv)
	{
	case EGSL_LowerImmortal: return m_vecLowerSkill;
	case EGSL_NormalImmortal: return m_vecNormalSkill;
	case EGSL_HighImmortal: return m_vecHighSkill;
	}
}


GodSkillDataMgr::GodSkillDataMgr()
: m_trunk(this)
{
	
}

void GodSkillDataMgr::RegisterAllEventHandler()
{
	m_pMgr->RegisterEventHandle(_T("Open_God_Skill"), (FRAMEEVENTPROC)m_trunk.sfp1(&GodSkillDataMgr::OnEventOpenGodSkill));
}

void GodSkillDataMgr::UnRegisterAllEventHandler()
{
	m_pMgr->UnRegisterEventHandler(_T("Open_God_Skill"), (FRAMEEVENTPROC)m_trunk.sfp1(&GodSkillDataMgr::OnEventOpenGodSkill));
}

void GodSkillDataMgr::RegisterAllNetCmd()
{
	m_pNetCmdMgr->Register("NS_PracticeMiracleSkill", (NETMSGPROC)(m_trunk.sfp2(&GodSkillDataMgr::OnNetPracticeMiracleSkill)), _T("NS_PracticeMiracleSkill"));
	m_pNetCmdMgr->Register("NS_ResetMiraclePrcNum",   (NETMSGPROC)(m_trunk.sfp2(&GodSkillDataMgr::OnNetResetMiraclePrcNum)), _T("NS_ResetMiraclePrcNum"));
}

void GodSkillDataMgr::UnRegisterAllNetCmd()
{
	m_pNetCmdMgr->UnRegister("NS_PracticeMiracleSkill", (NETMSGPROC)(m_trunk.sfp2(&GodSkillDataMgr::OnNetPracticeMiracleSkill)));
	m_pNetCmdMgr->UnRegister("NS_ResetMiraclePrcNum",   (NETMSGPROC)(m_trunk.sfp2(&GodSkillDataMgr::OnNetResetMiraclePrcNum)));
}

DWORD GodSkillDataMgr::OnEventOpenGodSkill(tagOpenGodSkillEvent *pEvent)
{
	// 打开仙界技能界面的时候关闭飞升技能界面
	SoarSkillFrame* pFrame = (SoarSkillFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("SoarSkillFrame"));
	if (P_VALID(pFrame))
	{
		m_pMgr->AddToDestroyList(pFrame);
	}

	if (!P_VALID(m_pMgr->GetFrame(_T("GodSkill"))))
	{
		switch (pEvent->eLv)
		{
		case GodSkillDataMgr::EGSL_LowerImmortal:
			{
				m_pMgr->CreateFrame(_T("World"), _T("GodSkill"), _T("GodSkillFrame"), static_cast<DWORD>(GodSkillDataMgr::EGSL_LowerImmortal));
			} break;
		case GodSkillDataMgr::EGSL_NormalImmortal:
			{
				m_pMgr->CreateFrame(_T("World"), _T("GodSkill"), _T("GodSkillFrame"), static_cast<DWORD>(GodSkillDataMgr::EGSL_NormalImmortal));
			} break;
		case GodSkillDataMgr::EGSL_HighImmortal:
			{
				m_pMgr->CreateFrame(_T("World"), _T("GodSkill"), _T("GodSkillFrame"), static_cast<DWORD>(GodSkillDataMgr::EGSL_HighImmortal));
			} break;
		}

	}
	return 0;
}

void GodSkillDataMgr::RegisterFrames(GameFrameMgr* pMgr)
{
	if (P_VALID(pMgr))
	{
		pMgr->Register(TWrap<GodSkillFrame>(),_T("GodSkillFrame"));
		pMgr->Register(TWrap<GodSkillMsgBox>(), _T("GodSkillMsgBox"));
	}
}

DWORD GodSkillDataMgr::OnNetPracticeMiracleSkill(tagNS_PracticeMiracleSkill *pNetCmd, DWORD)
{
	switch (pNetCmd->dwErrCode)
	{
	case E_Miracle_Success:
		{
			RoleMgr::Inst()->GetLocalPlayer()->SetMiraclePrcNum(static_cast<INT16>(pNetCmd->n16CurPrcNum));
			
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
			m_pMgr->SendEvent(&tagGameEvent(_T("Refresh_GodSkill"), NULL));
		} break;
	case E_Miracle_LackItem:
		{
			//CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodSkill_Error0")]);
			GodSkillMsgBox *pMsgBox = (GodSkillMsgBox*)m_pMgr->GetFrame(_T("GodSkillMsgBox"));
			if (!P_VALID(pMsgBox))
			{
				pMsgBox = (GodSkillMsgBox*)m_pMgr->CreateFrame(_T("World"), _T("GodSkillMsgBox"), _T("GodSkillMsgBox"), 0);
			}
			pMsgBox->SetText(g_StrTable[_T("GodSkill_Error0")], g_StrTable[_T("GodSkill_Error0_Detail")]);
		} break;
	case E_Miracle_LackPower:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodSkill_Error1")]);
			GodSkillMsgBox *pMsgBox = (GodSkillMsgBox*)m_pMgr->GetFrame(_T("GodSkillMsgBox"));
			if (!P_VALID(pMsgBox))
			{
				pMsgBox = (GodSkillMsgBox*)m_pMgr->CreateFrame(_T("World"), _T("GodSkillMsgBox"), _T("GodSkillMsgBox"), 0);
			}
			pMsgBox->SetText(g_StrTable[_T("GodSkill_Error1")], g_StrTable[_T("GodSkill_Error1_Detail")]);
		} break;
	case E_Miracle_LackMoney:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodSkill_Error2")]);
			GodSkillMsgBox *pMsgBox = (GodSkillMsgBox*)m_pMgr->GetFrame(_T("GodSkillMsgBox"));
			if (!P_VALID(pMsgBox))
			{
				pMsgBox = (GodSkillMsgBox*)m_pMgr->CreateFrame(_T("World"), _T("GodSkillMsgBox"), _T("GodSkillMsgBox"), 0);
			}
			pMsgBox->SetText(g_StrTable[_T("GodSkill_Error2")], g_StrTable[_T("GodSkill_Error2_Detail")]);
		} break;
	case E_Miracle_LackPracticeNum:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodSkill_Error3")]);
			GodSkillMsgBox *pMsgBox = (GodSkillMsgBox*)m_pMgr->GetFrame(_T("GodSkillMsgBox"));
			if (!P_VALID(pMsgBox))
			{
				pMsgBox = (GodSkillMsgBox*)m_pMgr->CreateFrame(_T("World"), _T("GodSkillMsgBox"), _T("GodSkillMsgBox"), 0);
			}
			pMsgBox->SetText(g_StrTable[_T("GodSkill_Error3")], g_StrTable[_T("GodSkill_Error3_Detail")]);
		} break;
	case E_LevelUpSkill_ExceedMaxLearnLevel:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodSkill_Error4")]);
		} break;
	default: break;
	}
	return 0;
}

void GodSkillDataMgr::SendPracticeMiracleSkill(DWORD dwSkillID)
{
	m_dwCurSkillID = dwSkillID;
	const tagSkillData *pSkillData = SkillMgr::Inst()->GetSkillData(dwSkillID);
	if (!P_VALID(pSkillData))
		m_nCurSkillLv = 0;
	else
		m_nCurSkillLv = pSkillData->nLevel;

	tagNC_PracticeMiracleSkill cmd;
	cmd.dwSkillID = dwSkillID;
	m_pSession->Send(&cmd);
}

DWORD GodSkillDataMgr::OnNetResetMiraclePrcNum(tagNS_ResetMiraclePrcNum *pNetCmd, DWORD)
{
	RoleMgr::Inst()->GetLocalPlayer()->SetMiraclePrcNum(static_cast<INT16>(MIRACLE_PRC_RESET_NUM));
	return 0;
}



void GodSkillDataMgr::InitSkills()
{
	// 给技能分类
	const GodSkillData::GodSkillIDSet& godSkillSet = GetGodSkillSet();
	GodSkillData::GodSkillIDSet::const_iterator end = godSkillSet.end();
	for (GodSkillData::GodSkillIDSet::const_iterator iter = godSkillSet.begin(); iter != end; ++iter)
	{
		DWORD dwSkillID = *iter;
		// 取第一级的typeid测试技能类型
		DWORD dwSkillTypeID = MTransSkillTypeID(dwSkillID, 1);
		const tagSkillProtoClient *pProto = SkillProtoData::Inst()->FindSkillProto(dwSkillTypeID);
		if (P_VALID(pProto))
		{
			ESkillTypeEx eSkillTypeEx = static_cast<ESkillTypeEx>(pProto->nType2);
			switch (eSkillTypeEx)
			{
			case ESSTE_LowerImmortal:
				{
					m_vecLowerSkill.push_back(dwSkillID);
				} break;
			case ESSTE_NormalImmortal:
				{
					m_vecNormalSkill.push_back(dwSkillID);
				} break;
			case ESSTE_HighImmortal:
				{
					m_vecHighSkill.push_back(dwSkillID);
				} break;
			default: break;
			}
		}
	}
}

void GodSkillDataMgr::DestroySkills()
{
	m_vecLowerSkill.clear();
	m_vecNormalSkill.clear();
	m_vecHighSkill.clear();
}