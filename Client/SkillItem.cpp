#include "stdafx.h"
#include "SkillItem.h"
#include "SkillButton.h"
#include "SkillMgr.h"

SkillItem::SkillItem(void)
{
	m_pSkill = NULL;
	m_pStcBack = NULL;
	m_pStcName = NULL;
	m_pStcLv = NULL;
	m_pProExp = NULL;
}

SkillItem::~SkillItem(void)
{
}

VOID SkillItem::ReloadUI(GUIWnd* pWnd, INT nIndex)
{
	TCHAR szTmp[X_SHORT_NAME]={0};
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("skill%d\\name"), nIndex);
	m_pStcName = (GUIStatic*)pWnd->GetChild(szTmp);
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("skill%d\\skill"), nIndex);
	m_pStcBack = (GUIStatic*)pWnd->GetChild(szTmp);
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("skbutt"), nIndex);
	m_pSkill = (SkillButton*)m_pStcBack->GetChild(szTmp);	
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("skill%d\\pro"), nIndex);
	m_pProExp = (GUIProgress*)pWnd->GetChild(szTmp);
}

VOID SkillItem::SetInvisible(BOOL b)
{
	m_pStcBack->SetInvisible(b);
	m_pStcName->SetInvisible(b);
	m_pProExp->SetInvisible(b);
	m_pSkill->SetInvisible(b);
	if (b)
	{
		m_pStcBack->SetRefresh();
		m_pStcName->SetRefresh();
		m_pProExp->SetRefresh();
		m_pSkill->SetRefresh();
	}
}

GUIWnd* SkillItem::GetSkillWnd()
{
	return m_pSkill;
}

GUIWnd* SkillItem::GetProcessWnd()
{
	return m_pProExp;
}

VOID SkillItem::SetName(LPCTSTR szName)
{
	m_pStcName->SetText(szName);
	m_pStcName->FlipToTop();
}

VOID SkillItem::SetExp(INT nExp, INT nMax)
{
	if(NULL == nMax)//--最大值为零，则表示没有经验，不显示经验条
	{
		m_pProExp->SetInvisible(TRUE);
		return;
	}

	m_pProExp->SetMaxValue((FLOAT)nMax);
	m_pProExp->SetValue((FLOAT)nExp, true);

	//TCHAR szTmp[X_SHORT_NAME] = {0};
	//_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d/%d"), nExp, nMax);
	//m_pProExp->SetText(szTmp);
	m_pProExp->SetInvisible(FALSE);
	m_pProExp->SetRefresh();
}

VOID SkillItem::UpdateSkill(DWORD dwID)
{
	FLOAT fCDCount = 0.0f;
	FLOAT fRestoreRatio = 0.0f;

	SetInvisible(FALSE);
	const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillData(dwID);
	if(P_VALID(pSkill))
	{
		SetName(pSkill->pProto->szName);

		SetExp(pSkill->nExp, pSkill->pProto->nLevelUpExp);
		
		m_pSkill->ReflashSkill(pSkill->dwTypeID);
		
		//设置冷却
		SkillMgr::Inst()->GetSkillCDTime(dwID, fCDCount, fRestoreRatio);
		m_pSkill->SetCDTime(fCDCount);
		m_pSkill->SetRestoreRatio(fRestoreRatio);

		return;
	}
		
	SetInvisible(TRUE);
}

DWORD SkillItem::GetSkillID()
{
	return m_pSkill->GetBtnData().GetData1();
}

tstring SkillItem::GetExp()
{
	TCHAR szBuff[X_SHORT_NAME] = {0};
	_stprintf( szBuff, _T("%d/%d"), (DWORD)m_pProExp->GetCurrentValue(), (DWORD)m_pProExp->GetMaxValue() );
	return szBuff;
}
