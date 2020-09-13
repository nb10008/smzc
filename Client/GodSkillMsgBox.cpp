#include "stdafx.h"
#include "GodSkillMsgBox.h"


GodSkillMsgBox::GodSkillMsgBox()
: m_trunk(this)
, m_pWnd(0)
, m_pStcSimple(0)
, m_pStcDetail(0)
, m_pBtnOK(0)
{
}

GodSkillMsgBox::~GodSkillMsgBox()
{
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = 0;
	}
}

BOOL GodSkillMsgBox::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;
	m_pWnd->FlipToTop();
	m_pGUI->SetModal(m_pWnd);

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_trunk.sfp1(&GodSkillMsgBox::EventHandler)));
	
	return TRUE;
}

BOOL GodSkillMsgBox::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;

	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName(), (VUIEVENTHANDLE)(m_trunk.sfp1(&GodSkillMsgBox::EventHandler)));

	return TRUE;
}

BOOL GodSkillMsgBox::ReloadUI()
{
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\common17.xml");
	m_pWnd = m_pGUI->CreateFromFile("VFS_System", strPath.c_str(), 0);
	m_pStcSimple = (GUIStaticEx*)m_pWnd->GetChild(_T("comm17_pic\\word"));
	m_pStcDetail = (GUIStaticEx*)m_pWnd->GetChild(_T("comm17_pic\\back\\win\\word"));
	m_pBtnOK = (GUIButton*)m_pWnd->GetChild(_T("comm17_pic\\surebutt"));
	
	return TRUE;
}

BOOL GodSkillMsgBox::EscCancel()
{
	return FALSE;
}

DWORD GodSkillMsgBox::EventHandler(tagGUIEvent *pEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnOK)
			{
				m_pGUI->SetModal(NULL);
				m_pMgr->AddToDestroyList(this);
			}
		} break;
	default: break;
	}
	return 0;
}

void GodSkillMsgBox::SetText(const tstring& strSimple, const tstring& strDetail)
{
	m_pStcSimple->SetText(strSimple.c_str());
	m_pStcDetail->SetText(strDetail.c_str());
	m_pWnd->SetRefresh(TRUE);
}