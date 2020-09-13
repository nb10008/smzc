#include "StdAfx.h"
#include "HolyBattleFrame.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_Arena.h"

const FLOAT	g_fMaxPoint	= 100.0;
const INT			g_nMaxMemmber = 5;

HolyBattleFrame::HolyBattleFrame(void)
:m_Trunk(this)
,m_pWnd(NULL)
,m_pBtnUnfold(NULL)
,m_pStcBattleName(NULL)
,m_pProRedArmy(NULL)
,m_pStcRedPercent(NULL)
,m_pStcRedNum(NULL)
,m_pProBlueArmy(NULL)
,m_pStcBluePercent(NULL)
,m_pStcBlueNum(NULL)
{
}

HolyBattleFrame::~HolyBattleFrame(void)
{
}

BOOL HolyBattleFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	// 注册游戏事件处理函数
	//m_pMgr->RegisterEventHandle(_T("tagUpdateLPAttEvent"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&HolyBattleFrame::OnGameEvent));
	
	m_pCmdMgr->Register("NS_SetArenaInterface",	        (NETMSGPROC)m_Trunk.sfp2(&HolyBattleFrame::OnNS_SetArenaInterface),		_T("NS_SetArenaInterface"));
	m_pCmdMgr->Register("NS_SetArenaName",				(NETMSGPROC)m_Trunk.sfp2(&HolyBattleFrame::OnNS_SetArenaName),			_T("NS_SetArenaName"));
	m_pCmdMgr->Register("NS_SetArenaPercent",	        (NETMSGPROC)m_Trunk.sfp2(&HolyBattleFrame::OnNS_SetArenaPercent),		_T("NS_SetArenaPercent"));
	m_pCmdMgr->Register("NS_SetArenaMemNum",	    (NETMSGPROC)m_Trunk.sfp2(&HolyBattleFrame::OnNS_SetArenaMemNum),	_T("NS_SetArenaMemNum"));

	
	return bRet;
}

BOOL HolyBattleFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	if( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	
	if( P_VALID(m_pWndMin) )
	{
		m_pGUI->AddToDestroyList(m_pWndMin);
		m_pWndMin = NULL;
	}


	// 注销游戏事件处理函数
	//m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPAttEvent"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&HolyBattleFrame::OnGameEvent));
	
	m_pCmdMgr->UnRegister("NS_SetArenaInterface",		(NETMSGPROC)m_Trunk.sfp2(&HolyBattleFrame::OnNS_SetArenaInterface));
	m_pCmdMgr->UnRegister("NS_SetArenaName",			(NETMSGPROC)m_Trunk.sfp2(&HolyBattleFrame::OnNS_SetArenaName));
	m_pCmdMgr->UnRegister("NS_SetArenaPercent",		(NETMSGPROC)m_Trunk.sfp2(&HolyBattleFrame::OnNS_SetArenaPercent));
	m_pCmdMgr->UnRegister("NS_SetArenaMemNum",	(NETMSGPROC)m_Trunk.sfp2(&HolyBattleFrame::OnNS_SetArenaMemNum));

	
	return bRet;
}

BOOL HolyBattleFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\melee.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element); 
	
	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&HolyBattleFrame::EventHandler));

	m_pBtnUnfold			=	 (GUIButton*)m_pWnd->GetChild( _T("melee\\minimize") );
	m_pStcBattleName	=	 (GUIStatic*)m_pWnd->GetChild( _T("melee") );
	m_pProRedArmy		=	 (GUIProgress*)m_pWnd->GetChild( _T("melee\\pro0") );
	m_pStcRedPercent	=	 (GUIStatic*)m_pWnd->GetChild( _T("melee\\percent0") );
	m_pStcRedNum		=	 (GUIStatic*)m_pWnd->GetChild( _T("melee\\num0") );
	m_pProBlueArmy	=	 (GUIProgress*)m_pWnd->GetChild( _T("melee\\pro1") );
	m_pStcBluePercent	=	 (GUIStatic*)m_pWnd->GetChild( _T("melee\\percent1") );
	m_pStcBlueNum		=	 (GUIStatic*)m_pWnd->GetChild( _T("melee\\num1") );
	m_pWnd->SetInvisible(TRUE);

	//-----------元神状态界面------------
	XmlElement ele;
	tstring strPath1 = g_strLocalPath + _T("\\ui\\melee_mini.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath1.c_str());
	m_pWndMin = m_pGUI->CreateWnd(_T("\\desktop"), &ele); 

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWndMin->GetFullName().c_str(), m_Trunk.sfp1(&HolyBattleFrame::MinEventHandler));

	m_pBtnWndFold	=	 (GUIButton*)m_pWndMin->GetChild( _T("melee_mini") );
	m_pWndMin->SetInvisible(TRUE);

	return TRUE;
}

VOID HolyBattleFrame::Update()
{
	 
}

VOID HolyBattleFrame::ShowWnd(BOOL bOpen/*=TRUE*/)
{
	if (P_VALID(m_pWnd))
	{
		m_pWnd->SetInvisible( !bOpen );
	}
}

VOID HolyBattleFrame::ShowMinWnd(BOOL bOpen/*=TRUE*/)
{
	if (P_VALID(m_pWndMin))
	{
		m_pWndMin->SetInvisible( !bOpen );
	}
}

DWORD HolyBattleFrame::OnNS_SetArenaInterface(tagNS_SetArenaInterface *pNetCmd, DWORD)
{
	ShowWnd(pNetCmd->bOpen);
	ShowMinWnd(FALSE);
	if (P_VALID(m_pWnd)&&!pNetCmd->bOpen)
	{
		m_pStcBattleName->SetText(_T(""));
		m_pProRedArmy->SetValue(0,TRUE);
		m_pStcRedPercent->SetText(_T(""));
		m_pStcRedNum->SetText(_T(""));
		m_pProBlueArmy->SetValue(0,TRUE);
		m_pStcBluePercent->SetText(_T(""));
		m_pStcBlueNum->SetText(_T(""));
	}
	return 0;
}

DWORD HolyBattleFrame::OnNS_SetArenaName(tagNS_SetArenaName *pNetCmd, DWORD)
{
	if ( !P_VALID(m_pWnd) 
		|| ( pNetCmd->eRoleState!=ES_ArenaRed && pNetCmd->eRoleState!=ES_ArenaBlue ) 
		|| ( pNetCmd->eMode<EAMN_Wuhui || pNetCmd->eMode>EAMN_Pohuai ) )
		return 0;

	tstringstream tss;
	tss<<_T("HolyBattle_Tittle")<<DWORD(pNetCmd->eMode);
	m_pStcBattleName->SetText( g_StrTable[tss.str().c_str()] );

	return 0;
}

DWORD HolyBattleFrame::OnNS_SetArenaPercent(tagNS_SetArenaPercent *pNetCmd, DWORD)
{
	if ( !P_VALID(m_pWnd) || ( pNetCmd->eRoleState!=ES_ArenaRed && pNetCmd->eRoleState!=ES_ArenaBlue ) )
		return 0;

	tstringstream tss;
	FLOAT  fPercent = (FLOAT)pNetCmd->dwPercent;
	tss<<fPercent<<_T("%");
	if ( pNetCmd->eRoleState==ES_ArenaRed )
	{
		m_pProRedArmy	->SetMaxValue( g_fMaxPoint );
		m_pProRedArmy	->SetValue( fPercent, TRUE );
		m_pStcRedPercent->SetText( tss.str().c_str() );
		m_pStcRedPercent->FlipToTop();
	}
	else if ( pNetCmd->eRoleState==ES_ArenaBlue )
	{
		m_pProBlueArmy->SetMaxValue( g_fMaxPoint );
		m_pProBlueArmy->SetValue( fPercent, TRUE );
		m_pStcBluePercent	->SetText( tss.str().c_str() );
		m_pStcBluePercent->FlipToTop();
	}

	return 0;
}

DWORD HolyBattleFrame::OnNS_SetArenaMemNum(tagNS_SetArenaMemNum *pNetCmd, DWORD)
{
	if ( !P_VALID(m_pWnd) || ( pNetCmd->eRoleState!=ES_ArenaRed && pNetCmd->eRoleState!=ES_ArenaBlue ) )
		return 0;

	tstringstream tss;
	INT nNum = (INT)pNetCmd->dwNum;
	tss<<nNum<<_T(" / 5");
	if ( pNetCmd->eRoleState==ES_ArenaRed )
	{
		m_pStcRedNum->SetText( tss.str().c_str() );
	}
	else if ( pNetCmd->eRoleState==ES_ArenaBlue )
	{
		m_pStcBlueNum->SetText( tss.str().c_str() );
	}

	return 0;
}

VOID HolyBattleFrame::ShowTips(DWORD dwIndex)
{
	tstringstream tss;
	tss<<_T("HolyBattle_Tittle")<<dwIndex;
	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[tss.str().c_str()]);
}

DWORD HolyBattleFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd==m_pBtnUnfold )
			{
				ShowWnd(FALSE);
				ShowMinWnd(TRUE);
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
		}
		break;
	default:
		break;
	}
	return 1;
}

DWORD HolyBattleFrame::MinEventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd==m_pBtnWndFold )
			{
				ShowWnd(TRUE);
				ShowMinWnd(FALSE);
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
		}
		break;
	default:
		break;
	}
	return 1;
}