#include "StdAfx.h"
#include "FamilySpriteFrame.h"
#include "FamilyFrame.h"
#include "SpriteRegisterFrame.h"
#include "SpriteTrainingFrame.h"
#include "FamilyProtoData.h"
#include "..\WorldDefine\msg_family.h"
#include "FamilyMgr.h"

void GUISpriteAtt::RefreshUI(DWORD dwValue, DWORD dwMaxValue )
{
	TCHAR szTmp2[X_SHORT_NAME] = {0};
	DWORD dwTemp = dwValue;
	if (GT_INVALID == dwTemp)
	{
		dwTemp = 0;
		pBack->SetInvisible(FALSE);
		pBack->FlipToTop();
	}
	else
	{
		pBack->SetInvisible(TRUE);
	}
	_stprintf(szTmp2, _T("%d/%d"), dwTemp,dwMaxValue);
	pPgValue->SetValue(dwTemp);
	pPgValue->SetMaxValue(dwMaxValue);
	pPgValue->SetSpeed(10000000.0f);
	pPgValue->SetPicColor(0x00000000); 
	pPgValue->SetForePicColor(0xFF00A2FF);
	pNum->SetText(szTmp2);
}

void GUISpriteAtt::clear()
{
	pPgValue->SetValue(0);
	pPgValue->SetMaxValue(0);
	pPgValue->SetSpeed(10000000.0f);
	pPgValue->SetPicColor(0x00000000); 
	pPgValue->SetForePicColor(0xFF00A2FF);
	pNum->SetText(_T(""));
}

FamilySpriteFrame::FamilySpriteFrame(void):m_trunk(this)
,m_pSpriteRegisterFrame(NULL)
,m_pSpriteTrainingFrame(NULL)
{
}

FamilySpriteFrame::~FamilySpriteFrame(void)
{
}

BOOL FamilySpriteFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register( "NS_GetFamilySpriteInfo",		(NETMSGPROC)m_trunk.sfp2(&FamilySpriteFrame::OnNS_GetFamilySpriteInfo),		_T("NS_GetFamilySpriteInfo"));
	return TRUE;
}

BOOL FamilySpriteFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	if(P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}

	m_pCmdMgr->UnRegister( "NS_GetFamilySpriteInfo",							(NETMSGPROC)m_trunk.sfp2(&FamilySpriteFrame::OnNS_GetFamilySpriteInfo) );
	return bRet;
}

BOOL FamilySpriteFrame::ReloadUI()
{
	m_pWndFather = m_pGUI->GetWnd(((FamilyFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("fairyback"));
	TCHAR buffer[X_LONG_NAME];
	
	for (int i = EFSA_Begin; i <EFSA_Num; ++i)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("ability\\ability%d"), i);
		GUIWnd*		tempWnd = m_pWnd->GetChild(buffer);
		m_sSpriteAtt[i].pNum = (GUIStatic*)tempWnd->GetChild(_T("num"));
		m_sSpriteAtt[i].pBack = (GUIStatic*)tempWnd->GetChild(_T("close"));
		m_sSpriteAtt[i].pPgValue = (GUIProgress*)tempWnd->GetChild(_T("back\\pro"));
		m_sSpriteAtt[i].pBack->SetUnClickable(TRUE);
	}

	m_pBnRegister = (GUIButton*)m_pWnd->GetChild(_T("butt0"));
	m_pBnTrain = (GUIButton*)m_pWnd->GetChild(_T("butt1"));
	m_pStLevel = (GUIStatic*)m_pWnd->GetChild(_T("level"));
	m_pPgSpriteExp = (GUIProgress*)m_pWnd->GetChild(_T("upgrowth\\pro"));

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_trunk.sfp1(&FamilySpriteFrame::EventHandler));

	m_pSpriteRegisterFrame = (SpriteRegisterFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("SpriteRegister"), _T("SpriteRegisterFrame"), 0);
	m_pSpriteTrainingFrame = (SpriteTrainingFrame*)m_pFrameMgr->CreateFrame(_T("World"), _T("SpriteTraining"), _T("SpriteTrainingFrame"), 0);
	return TRUE;
}

BOOL FamilySpriteFrame::EscCancel()
{
	return FALSE;
}

VOID FamilySpriteFrame::Update()
{

}

DWORD FamilySpriteFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pBnRegister)
			{
				if(m_pSpriteRegisterFrame->IsInvisable())
					m_pSpriteRegisterFrame->ShowFrame();
				else
					m_pSpriteRegisterFrame->ShowFrame(FALSE);
			}
			else if (pWnd == m_pBnTrain)
			{
				if(m_pSpriteTrainingFrame->IsInvisable())
					m_pSpriteTrainingFrame->ShowFrame();
				else
					m_pSpriteTrainingFrame->ShowFrame(FALSE);
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			
		}
		break;
	case EGUIE_Drag:
		{

		}
		break;
	}
	return 0;
}

VOID FamilySpriteFrame::ShowFrame(BOOL bShow)
{
	m_pWnd->SetInvisible(!bShow);
}

VOID FamilySpriteFrame::RefreshSpriteInfo()
{

}

DWORD FamilySpriteFrame::OnNS_GetFamilySpriteInfo(tagNS_GetFamilySpriteInfo* pNetCmd,DWORD)
{
	FamilyMgr::Inst()->SetSpriteLevel(pNetCmd->nLevel);
	const tagFamilySpriteProto* pData = FamilyProtoData::Inst()->FindFamilySpriteProto(pNetCmd->nLevel);
	if(!P_VALID(pData))	return 0;

	TCHAR buffer[X_LONG_NAME];
	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("Lv %d"), pNetCmd->nLevel);
	m_pStLevel->SetText(buffer);
	DWORD dwValue =pNetCmd->dwExp;
	DWORD dwMaxValue = 0;
	dwMaxValue = pData->dwExp;
	_stprintf(buffer, _T("%d/%d"),dwValue,dwMaxValue);
	m_pPgSpriteExp->SetValue(dwValue);
	m_pPgSpriteExp->SetMaxValue(dwMaxValue);
	m_pPgSpriteExp->SetSpeed(10000000.0f);
	m_pPgSpriteExp->SetPicColor(0x00000000); 
	m_pPgSpriteExp->SetForePicColor(0xFF00A2FF);
	m_pPgSpriteExp->SetText(buffer);

	for (int i = EFSA_Begin; i< EFSA_Num; ++i)
	{
		m_sSpriteAtt[i].RefreshUI(pNetCmd->dwSpriteAtt[i],pData->dwAbilityUpperLimit[i]);
		FamilyMgr::Inst()->SetSpriteAtt(i,pNetCmd->dwSpriteAtt[i]);
	}

	return 0;
}