#include "stdafx.h"
#include "GodSkillFrame.h"
#include "IconStatic.h"
#include "SkillProtoData.h"
#include "SkillMgr.h"
#include "../WorldDefine/currency_define.h"
#include "../WorldDefine/role_att.h"
#include "CurrencyMgr.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "ItemProtoData.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "QuestMgr.h"



const tstring ICON_DEFAULT_BACKGROUND = _T("data\\ui\\main\\l_icon_back.bmp");
const tstring ICON_DEFAULT_CHOOSE_BACKGROUND = _T("data\\ui\\Common\\L_icon-l.bmp");
const tstring ICON_DEFAULT_NO_CHOOSE_BACKGROUND= _T("data\\ui\\common\\l_icon.bmp");
const int MAX_GOD_SKILL_LEVEL = 40;


GodSkillFrame::GodSkillFrame()
: m_trunk(this)
, m_pWnd(NULL)
, m_pIstSkillImg(NULL)
, m_pStcSkillName(NULL)
, m_pStcSkillLv(NULL)
, m_pStcOwnGold(NULL)
, m_pStcOwnSilver(NULL)
, m_pStcCurStrength(NULL)
, m_pStcCurCount(NULL)
, m_pProgress(NULL)
, m_pBtnPre(NULL)
, m_pBtnNext(NULL)
, m_pStcCurPageNum(NULL)
, m_pStcGold(NULL)
, m_pStcSilver(NULL)
, m_pStcStrength(NULL)
, m_pStcItem(NULL)
, m_pBtnLearn(NULL)
, m_pBtnClose(NULL)
, m_pBtnX(NULL)
, m_nCurItem(GT_INVALID)
, m_nCurPageNum(0)
, m_nTotalPageNum(0)
, m_dwCurTime(0)
{
	ZeroMemory(m_pIstSkillBacks, sizeof(m_pIstSkillBacks));
	ZeroMemory(m_pIstSkills, sizeof(m_pIstSkills));
	ZeroMemory(m_pStcSkillNames, sizeof(m_pStcSkillNames));
	ZeroMemory(m_pStcSkillGrades, sizeof(m_pStcSkillGrades));
	
}

GodSkillFrame::~GodSkillFrame()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
}

BOOL GodSkillFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
	{
		return FALSE;
	}
	
	if (dwParam > GodSkillDataMgr::EGSL_HighImmortal || dwParam < GodSkillDataMgr::EGSL_LowerImmortal)
		return FALSE;
	else
		m_eCurGodSkillLv = static_cast<GodSkillDataMgr::EGodSkillLevel>(dwParam);

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_trunk.sfp1(&GodSkillFrame::EventHandler)));
	m_pMgr->RegisterEventHandle(_T("Refresh_GodSkill"), (FRAMEEVENTPROC)(m_trunk.sfp1(&GodSkillFrame::OnEventRefresh)));
	TObjRef<NetCmdMgr>()->Register("NS_GetConsumptiveStrength", (NETMSGPROC)(m_trunk.sfp2(&GodSkillFrame::OnNetGetConsumptiveStrength)), _T("NS_GetConsumptiveStrength"));

	tagNC_GetConsumptiveStrength cmd;
	TObjRef<NetSession>()->Send(&cmd);

	SetDefaultUI();
	UpdateUI();
	
	

	return TRUE;
}


BOOL GodSkillFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;

	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_trunk.sfp1(&GodSkillFrame::EventHandler)));
	m_pMgr->UnRegisterEventHandler(_T("Refresh_GodSkill"), (FRAMEEVENTPROC)(m_trunk.sfp1(&GodSkillFrame::OnEventRefresh)));
	TObjRef<NetCmdMgr>()->UnRegister("NS_GetConsumptiveStrength", (NETMSGPROC)(m_trunk.sfp2(&GodSkillFrame::OnNetGetConsumptiveStrength)));
	return TRUE;
}

BOOL GodSkillFrame::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\god_skill.xml");
	XmlElement ele;
	if (!m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str()))
		return FALSE;

	m_pGUI->ChangeXml(&ele, "party_skill_win\\party_skill_back\\guildskill_back\\doingskillback_pic\\skillback\\skill", "ClassName", "IconStatic");

	char szBuffer[X_LONG_NAME] = {0};
	for (int i = 0; i < SKILL_NUM_PER_PAGE; ++i)
	{
		_snprintf(szBuffer, X_LONG_NAME, "party_skill_win\\party_skill_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillback%d", i, i);
		m_pGUI->ChangeXml(&ele, szBuffer, "ClassName", "IconStatic");
		_snprintf(szBuffer, X_LONG_NAME, "party_skill_win\\party_skill_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillback%d\\skill%d", i, i, i);
		m_pGUI->ChangeXml(&ele, szBuffer, "ClassName", "IconStatic");
	}
	
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pIstSkillImg = (IconStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\doingskillback_pic\\skillback\\skill"));
	m_pStcSkillName = (GUIStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\doingskillback_pic\\num0"));
	m_pStcSkillLv = (GUIStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\doingskillback_pic\\num1"));
	m_pStcOwnGold = (GUIStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\doingskillback_pic\\num2"));
	m_pStcOwnSilver = (GUIStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\doingskillback_pic\\num3"));
	m_pStcCurStrength = (GUIStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\doingskillback_pic\\num5"));
	m_pStcCurCount = (GUIStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\doingskillback_pic\\num6"));

	m_pProgress = (GUIProgress*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\doingskillback_pic\\progressbarback\\progressbar"));
	
	m_pBtnPre = (GUIButton*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\doingskillback_pic2\\backbutt"));
	m_pBtnNext = (GUIButton*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\doingskillback_pic2\\nextbutt"));
	m_pStcCurPageNum = (GUIStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\doingskillback_pic2\\pagenum"));
	
	m_pStcGold = (GUIStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\needingback_pic\\num1"));
	m_pStcSilver = (GUIStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\needingback_pic\\num2"));

	m_pStcStrength = (GUIStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\needingback_pic\\num4"));
	m_pStcItem = (GUIStatic*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\needingback_pic\\num3"));

	m_pBtnLearn = (GUIButton*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\gskillbutt5"));
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild(_T("party_skill_back\\guildskill_back\\close"));
	m_pBtnX = (GUIButton*)m_pWnd->GetChild(_T("party_skill_back\\closebutt"));

	TCHAR szWBuffer[X_LONG_NAME] = {0};
	for (int i = 0; i < SKILL_NUM_PER_PAGE; ++i)
	{
		_sntprintf(szWBuffer, X_LONG_NAME, _T("party_skill_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillback%d"), i, i);
		m_pIstSkillBacks[i] = (IconStatic*)m_pWnd->GetChild(szWBuffer);
		_sntprintf(szWBuffer, X_LONG_NAME, _T("party_skill_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillback%d\\skill%d"), i, i, i);
		m_pIstSkills[i] = (IconStatic*)m_pWnd->GetChild(szWBuffer);
		_sntprintf(szWBuffer, X_LONG_NAME, _T("party_skill_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillname%d"), i, i);
		m_pStcSkillNames[i] = (GUIStatic*)m_pWnd->GetChild(szWBuffer);
		_sntprintf(szWBuffer, X_LONG_NAME, _T("party_skill_back\\guildskill_back\\doingskillback_pic2\\back%d\\skillgrade%d"), i, i);
		m_pStcSkillGrades[i] = (GUIStatic*)m_pWnd->GetChild(szWBuffer);
	}
	return TRUE;
}

BOOL GodSkillFrame::EscCancel()
{
	m_pMgr->AddToDestroyList(this);
	return TRUE;
}

DWORD GodSkillFrame::EventHandler(tagGUIEvent *pEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (m_pBtnClose == pWnd || m_pBtnX == pWnd)
			{
				m_pMgr->AddToDestroyList(this);
			}
			if (m_pBtnLearn == pWnd)
			{
				OnSkillLearnClick();
			}
		} break;
	case EGUIE_Drag:
		{
			for (int i = 0; i < SKILL_NUM_PER_PAGE; ++i)
			{
				if (pWnd == m_pIstSkills[i])
				{
					OnSkillButtonClick(i);
				}
			}
		} break;
	case EGUIE_MoveIntoCtrl:
		{
			for (int i = 0; i < SKILL_NUM_PER_PAGE; ++i)
			{
				if (pWnd == m_pIstSkills[i])
				{
					int nChooseItem = m_nCurPageNum * (SKILL_NUM_PER_PAGE - 1) + i;
					const std::vector<DWORD>& vecSkills = GodSkillDataMgr::Inst()->GetSkills(m_eCurGodSkillLv);
					if (nChooseItem < static_cast<int>(vecSkills.size()))
					{
						DWORD dwSkillID = vecSkills[nChooseItem];
						// 计算玩家当前技能等级
						int nPlayerSkillLv;
						const tagSkillData* pSkillData = SkillMgr::Inst()->GetSkillData(dwSkillID);
						if (!P_VALID(pSkillData))
							nPlayerSkillLv = 0;
						else
							nPlayerSkillLv = pSkillData->nLevel;
						if (nPlayerSkillLv < MAX_GOD_SKILL_LEVEL)
						{
							DWORD dwSkillTypeID = MTransSkillTypeID(dwSkillID, nPlayerSkillLv + 1);
							tstring strTip = ToolTipCreator::Inst()->GetTalentTips(dwSkillTypeID);
							tstring strRealTip = strTip.substr(strTip.find_first_of(_T(" ")), strTip.length() - strTip.find_first_of(_T(" ")));
							ShowTip(m_pIstSkills[i], strRealTip.c_str());
						}
						else
						{
							DWORD dwSkillTypeID = MTransSkillTypeID(dwSkillID, nPlayerSkillLv);
							tstring strTip = ToolTipCreator::Inst()->GetTalentTips(dwSkillTypeID);	
							ShowTip(m_pIstSkills[i], strTip.c_str());
						}
					}
				}
			} break;
		}
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		} break;
	}
	return 0;
}

void GodSkillFrame::SetDefaultUI()
{
	m_pIstSkillImg->SetPic(ICON_DEFAULT_BACKGROUND);
	for (int i = 0; i < SKILL_NUM_PER_PAGE; ++i)
	{
		m_pIstSkills[i]->SetPic(ICON_DEFAULT_BACKGROUND);
		m_pIstSkillBacks[i]->SetPic(ICON_DEFAULT_NO_CHOOSE_BACKGROUND);
	}
	
	m_pStcSkillName->SetText(_T(""));
	m_pStcSkillLv->SetText(_T(""));
	m_pStcCurCount->SetText(_T(""));
	m_pStcGold->SetText(_T(""));
	m_pStcSilver->SetText(_T(""));
	m_pStcStrength->SetText(_T(""));
	m_pStcItem->SetText(_T(""));
	m_pProgress->SetValue(0.0f, true);
	m_pWnd->SetRefresh(TRUE);
}

void GodSkillFrame::UpdateUI()
{
	TCHAR szBuffer[X_SHORT_NAME] = {0};
	TCHAR szBigBuffer[X_LONG_NAME] = {0};

	const std::vector<DWORD>& vecSkills = GodSkillDataMgr::Inst()->GetSkills(m_eCurGodSkillLv);
	// 取得技能数量
	int nSkillNum = static_cast<int>(vecSkills.size());
	// 计算总页数
	m_nTotalPageNum = (nSkillNum / SKILL_NUM_PER_PAGE) + 1;

	if (m_nCurPageNum > 0)
		m_pBtnPre->SetEnable(true);
	else
		m_pBtnPre->SetEnable(false);

	if (m_nCurPageNum < m_nTotalPageNum - 1)
		m_pBtnNext->SetEnable(true);
	else
		m_pBtnNext->SetEnable(false);

	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), m_nCurPageNum + 1);
	m_pStcCurPageNum->SetText(szBuffer);
	
	// 计算当前skill的初始位置
	int nSkillPos = m_nCurPageNum * SKILL_NUM_PER_PAGE;
	
	while (nSkillPos < static_cast<int>(vecSkills.size()))
	{
		int nPos = nSkillPos % SKILL_NUM_PER_PAGE;
		DWORD dwSkillID = vecSkills[nSkillPos];
		// 使用第一级的技能TypeID获取图标
		DWORD dwSkillTypeID = MTransSkillTypeID(dwSkillID, 1);
		const tagSkillProtoClient *pSkillProto = SkillProtoData::Inst()->FindSkillProto(dwSkillTypeID);
		if (P_VALID(pSkillProto))
		{
			_sntprintf(szBigBuffer, X_LONG_NAME, _T("data\\ui\\Icon\\%s.tga"), pSkillProto->szIcon);
			m_pIstSkills[nPos]->SetPic(szBigBuffer);
			m_pStcSkillNames[nPos]->SetText(pSkillProto->szName);
		}
		
		// 玩家所学技能等级
		int nPlayerSkillLv;
	    const tagSkillData* pSkillData = SkillMgr::Inst()->GetSkillData(dwSkillID);
		if (!P_VALID(pSkillData))
			nPlayerSkillLv = 0;
		else
			nPlayerSkillLv = pSkillData->nLevel;
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d/%d"), nPlayerSkillLv, MAX_GOD_SKILL_LEVEL);
		m_pStcSkillGrades[nPos]->SetText(szBuffer);
		++nSkillPos;
	}
	
	INT64 n64Silver = CurrencyMgr::Inst()->GetBagSilver();
	int nGold = MSilver2DBGold(n64Silver);
	int nSilver = MSilver2DBSilver(n64Silver);
	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nGold);
	m_pStcOwnGold->SetText(szBuffer);
	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nSilver);
	m_pStcOwnSilver->SetText(szBuffer);
	INT16 n16Num = RoleMgr::Inst()->GetLocalPlayer()->GetMiraclePrcNum();
	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), n16Num);
	m_pStcCurCount->SetText(szBuffer);

	if (m_nCurItem != GT_INVALID)
	{
		int nCurChoosePos = (m_nCurPageNum * SKILL_NUM_PER_PAGE + m_nCurItem);
		if (nCurChoosePos >= static_cast<int>(vecSkills.size()))
		{
			m_pBtnLearn->SetEnable(false);
			return;
		}
		m_pBtnLearn->SetEnable(true);

		m_pIstSkillBacks[m_nCurItem]->SetPic(ICON_DEFAULT_CHOOSE_BACKGROUND);	

		DWORD dwSkillID = vecSkills[nCurChoosePos];
		// 玩家所学技能等级
		int nPlayerSkillLv;
		const tagSkillData* pSkillData = SkillMgr::Inst()->GetSkillData(dwSkillID);
		if (!P_VALID(pSkillData))
			nPlayerSkillLv = 0;
		else
			nPlayerSkillLv = pSkillData->nLevel;
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nPlayerSkillLv);
		m_pStcSkillLv->SetText(szBuffer);

		// 统一用第一级的技能图标吧
		DWORD dwSkillTypeID = MTransSkillTypeID(dwSkillID, 1);
		const tagSkillProtoClient *pProto = SkillProtoData::Inst()->FindSkillProto(dwSkillTypeID);
		if (P_VALID(pProto))
		{
			m_pStcSkillName->SetText(pProto->szName);
			_sntprintf(szBigBuffer, X_LONG_NAME, _T("data\\ui\\Icon\\%s.tga"), pProto->szIcon);
			m_pIstSkillImg->SetPic(szBigBuffer);
		}
		// 取技能熟练度
		if (nPlayerSkillLv == 0)
		{
		}
		else
		{
			// 取升级所需熟练度
			const tagSkillProtoClient *pProto = SkillProtoData::Inst()->FindSkillProto(MTransSkillTypeID(dwSkillID, nPlayerSkillLv));
			if (P_VALID(pProto))
			{
				pProto->nLevelUpExp;
			}
			m_pProgress->SetMaxValue(static_cast<float>(pProto->nLevelUpExp));
			m_pProgress->SetValue(static_cast<float>(pSkillData->nExp), true);
		}
	// 计算下一级
		int nPlayerSkillNextLv;
		if (nPlayerSkillLv == MAX_GOD_SKILL_LEVEL)
			nPlayerSkillNextLv = MAX_GOD_SKILL_LEVEL;
		else
			nPlayerSkillNextLv = nPlayerSkillLv + 1;
		
		DWORD dwTypeID = MTransSkillTypeID(dwSkillID, nPlayerSkillNextLv);
		const tagGodSkillData *pData = GodSkillDataMgr::Inst()->GetGodSkillData(dwTypeID);
		if (P_VALID(pData))
		{
			INT64 n64Silver = pData->n64Silver;
			int nGold = MSilver2DBGold(n64Silver);
			int nSilver = MSilver2DBSilver(n64Silver);
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nGold);
			m_pStcGold->SetText(szBuffer);
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nSilver);
			m_pStcSilver->SetText(szBuffer);
			int nStrength = pData->nStrength;
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nStrength);
			m_pStcStrength->SetText(szBuffer);
			DWORD dwItemID = pData->dwItemTypeID;
			const tagItemDisplayInfo *pItemInfo = ItemProtoData::Inst()->FindItemDisplay(dwItemID);
			if (P_VALID(pItemInfo))
			{
				_sntprintf(szBuffer, X_SHORT_NAME, _T("%s X %d"), pItemInfo->szName, pData->nItemNum);
				m_pStcItem->SetText(szBuffer);
			}
		}
	}
	else
	{
		m_pBtnLearn->SetEnable(false);
	}

	m_pWnd->SetRefresh(TRUE);
}

void GodSkillFrame::OnSkillButtonClick(int nPos)
{
	m_nCurItem = nPos;
	SetDefaultUI();
	UpdateUI();
}

void GodSkillFrame::OnSkillLearnClick()
{
	const std::vector<DWORD>& vecSkills = GodSkillDataMgr::Inst()->GetSkills(m_eCurGodSkillLv);
	int nCurChoosePos = (m_nCurPageNum * SKILL_NUM_PER_PAGE + m_nCurItem);
	DWORD dwSkillID = vecSkills[nCurChoosePos];
	GodSkillDataMgr::Inst()->SendPracticeMiracleSkill(dwSkillID);
	


}

VOID GodSkillFrame::Update()
{
	m_dwCurTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_dwCurTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_dwCurTime = 0;

		if (RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID() ) )
		{
			m_pMgr->AddToDestroyList(this);
		}
	}
	GameFrame::Update();
}

DWORD GodSkillFrame::OnEventRefresh(tagGameEvent *pEvent)
{
	SetDefaultUI();
	UpdateUI();
	tagNC_GetConsumptiveStrength cmd;
	TObjRef<NetSession>()->Send(&cmd);
	return 0;
}

DWORD GodSkillFrame::OnNetGetConsumptiveStrength(tagNS_GetConsumptiveStrength *pNetCmd, DWORD)
{
	TCHAR szBuffer[X_SHORT_NAME] = {0};
	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), pNetCmd->nConsumptiveStrength);
	m_pStcCurStrength->SetText(szBuffer);
	m_pWnd->SetRefresh();
	return 0;
}