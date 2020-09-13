#include "StdAfx.h"
#include "CampaignResultFrame.h"
#include "..\WorldDefine\msg_god.h"

namespace SortCampaign
{
	class SortByStrength
	{
	public:
		bool operator()(const tagPersonInfo &op1, const tagPersonInfo &op2)
		{
			return op1.dwStrength > op2.dwStrength;
		}
	};
}

CampaignResultFrame::CampaignResultFrame(void):m_Trunk(this)
{
}

CampaignResultFrame::~CampaignResultFrame(void)
{
}

BOOL CampaignResultFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register( "NS_CampaignResult",		(NETMSGPROC)m_Trunk.sfp2(&CampaignResultFrame::OnNS_CampaignResult),		_T("NS_CampaignResult"));

	return TRUE;
}
BOOL CampaignResultFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pCmdMgr->UnRegister( "NS_CampaignResult",							(NETMSGPROC)m_Trunk.sfp2(&CampaignResultFrame::OnNS_CampaignResult) );

	if(P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}

	return bRet;
}
BOOL CampaignResultFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\god_result.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&CampaignResultFrame::EventHandler));

	m_pPbnWar = (GUIPushButton*)m_pWnd->GetChild(_T("back\\choose\\choose0"));
	m_pPbnSun= (GUIPushButton*)m_pWnd->GetChild(_T("back\\choose\\choose1"));
	m_pPbnMana= (GUIPushButton*)m_pWnd->GetChild(_T("back\\choose\\choose2"));
	m_pPbnPeace= (GUIPushButton*)m_pWnd->GetChild(_T("back\\choose\\choose3"));
	m_pListInfo= (GUIListBox*)m_pWnd->GetChild(_T("back\\background\\list"));
	m_pBnClose= (GUIButton*)m_pWnd->GetChild(_T("back\\closebutton"));
	m_pBnX= (GUIButton*)m_pWnd->GetChild(_T("back\\canclebutt"));

	m_pListInfo->SetColNum(5, 84);
	m_pListInfo->SetColWidth(0, 45);
	m_pListInfo->SetColWidth(1, 170);
	m_pListInfo->SetColWidth(2, 90);
	m_pListInfo->SetColWidth(3, 90);

	m_pWnd->SetInvisible(false);

	return TRUE;
}
BOOL CampaignResultFrame::EscCancel()
{
	if (m_pWnd->IsInvisible())
	{
		return false;
	} 
	else
	{
		m_pWnd->SetInvisible(TRUE);
		return true;
	}
	return true;
}


BOOL CampaignResultFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if (m_pBnClose ==	pWnd || m_pBnX ==	pWnd )
			{
				m_pWnd->SetInvisible(TRUE);
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if (m_pPbnWar ==	pWnd)
			{
				AcquireInfo(ECGT_War);
			} 
			else if(m_pPbnSun ==	pWnd)
			{
				AcquireInfo(ECGT_Sun);
			}
			else if(m_pPbnMana ==	pWnd)
			{
				AcquireInfo(ECGT_Mana);
			}
			else if(m_pPbnPeace ==	pWnd)
			{
				AcquireInfo(ECGT_Peace);
			}
		}
		break;
	default:
		break;
	}
	return TRUE;
}

void CampaignResultFrame::RefreshList(ECampaignGodType eType,int num)
{
	tagGodCampaignInfo sInfo = m_ResultInfo.sCampaignResult[eType];
	SortGodCampaignInfo(sInfo);
	TCHAR szBuffer[X_SHORT_NAME];
	for (int i =0; i< MAX_CampaignInfoNumPerType ; i++)
	{
		if (i >= num )
		{
			m_pListInfo->SetText(i,0,_T("--"));
			m_pListInfo->SetText(i,1,_T("--"));
			m_pListInfo->SetText(i,2,_T("--"));
			m_pListInfo->SetText(i,3,_T("--"));
			continue;
		}
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), i+1);
		m_pListInfo->SetText(i,0,szBuffer);
		m_pListInfo->SetText(i,1,sInfo.sPlayerInfo[i].szName);
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), sInfo.sPlayerInfo[i].dwStrength);
		m_pListInfo->SetText(i,2,szBuffer);
		if ( ECT_Null !=  sInfo.sPlayerInfo[i].eClergy  &&	ECT_Loser !=  sInfo.sPlayerInfo[i].eClergy )
		{
			_sntprintf(szBuffer, X_SHORT_NAME, _T("Campaign_%d_%d"), int(eType),sInfo.sPlayerInfo[i].eClergy);
			m_pListInfo->SetText(i,3,g_StrTable[szBuffer]);
		}
		else
		{
			m_pListInfo->SetText(i,3,_T("--"));
		}
	}
}

void CampaignResultFrame::AcquireInfo(ECampaignGodType eType)
{
	tagNC_CampaignResult cmd;
	cmd.eType = eType;
	m_pSession->Send(&cmd);
}

DWORD CampaignResultFrame::OnNS_CampaignResult(tagNS_CampaignResult* pNetCmd,DWORD)
{
	m_ResultInfo.sCampaignResult[pNetCmd->eType] =pNetCmd->sResult;
	RefreshList(pNetCmd->eType,pNetCmd->nNum);
	return 0;
}

void CampaignResultFrame::SortGodCampaignInfo(tagGodCampaignInfo& info)
{
	vector<tagPersonInfo> vec(info.sPlayerInfo,info.sPlayerInfo+MAX_CampaignInfoNumPerType);
	std::sort(vec.begin(),vec.end(),SortCampaign::SortByStrength());
	for (int i =0;i <MAX_CampaignInfoNumPerType;i++ )
	{
		info.sPlayerInfo[i] = vec[i];
	}
}