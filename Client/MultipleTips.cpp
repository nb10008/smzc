#include "StdAfx.h"
#include "MultipleTips.h"
#include "CombatSysUtil.h"
MultipleTips::MultipleTips(void)
: m_Trunk(this)
, m_pGUI(NULL)
, m_pGameFrameMgr(NULL)
, m_pTurnWnd(NULL)
, m_pExpWnd(NULL)
,m_dwRestTime_Trun(0)
,m_dwRestTime_Exp(0)
{
}


static TCHAR szTmpItemDPTime[X_SHORT_NAME] = {0}; 
static TCHAR szTmpItemDPPercent[X_SHORT_NAME] = {0};
static BOOL bTurnWndVisible = FALSE;
static INT nRestTime_Trun = -1;
static BOOL bActiveItemFlag = FALSE; //是否开启活动

static TCHAR szTmpExpDPTime[X_SHORT_NAME] = {0}; 
static TCHAR szTmpExpDPPercent[X_SHORT_NAME] = {0};
static BOOL bExpWndVisible = FALSE;
static INT nRestTime_Exp = -1;
static BOOL bActiveExpFlag = FALSE; //是否开启活动



MultipleTips::~MultipleTips(void)
{
}
BOOL MultipleTips::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;

	m_pCmdMgr->Register("NS_DoubleDropProbabilityChange", (NETMSGPROC)m_Trunk.sfp2(&MultipleTips::NetRecvServerDoubleInfo), _T("NS_DoubleDropProbabilityChange") );
	m_pTurnWnd->SetInvisible(FALSE);
	m_pExpWnd ->SetInvisible(FALSE);
	return TRUE;
}

BOOL MultipleTips::Destroy()
{
	GameFrame::Destroy();
	m_pCmdMgr->UnRegister("NS_DoubleDropProbabilityChange",(NETMSGPROC)m_Trunk.sfp2(&MultipleTips::NetRecvServerDoubleInfo));
	if (P_VALID(m_pTurnWnd))
	{
		m_pGUI->AddToDestroyList(m_pTurnWnd);
		m_pTurnWnd = NULL;
		bTurnWndVisible = FALSE;
	}
	if (P_VALID(m_pExpWnd))
	{
		m_pGUI->AddToDestroyList(m_pExpWnd);
		m_pExpWnd = NULL;
		bExpWndVisible = FALSE;
	}
	return TRUE;
}
VOID MultipleTips::Update()
{
//	 
//
	m_dwEscapeTime += Kernel::Inst()->GetDeltaTimeDW();
//	if( m_dwEscapeTime > 60000 )//每隔60秒检测 
//	{
//		m_dwEscapeTime = 0;
//		if (m_dwRestTime_Trun)
//		{
//			m_dwRestTime_Trun--;
//		}
//		if (m_dwRestTime_Exp)
//		{
//			m_dwRestTime_Exp--;
//		} 
//
//	}
	if( m_dwEscapeTime > 1000 )
	{
		m_dwEscapeTime = 0;
	//	秒更新
		// 双倍物品掉落显示
		/*if( nRestTime_Exp == 0 )
		{
			_stprintf(szTmpExpDPTime, g_StrTable[_T("MULTIPLETIPS_EXP")]);
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(szTmpExpDPTime);
		}*/
		if( bExpWndVisible )
		{
			_sntprintf(szTmpExpDPTime,sizeof(szTmpExpDPTime)/sizeof(TCHAR),_T("%d"),(nRestTime_Exp/60 + 1));
			m_pStcRestExpTime->SetText(szTmpExpDPTime);
			m_pStcServerExp->SetText(szTmpExpDPPercent);
			m_pExpWnd->SetRefresh(TRUE);
		}
		else if (bActiveExpFlag)
		{
			bActiveExpFlag = FALSE;
			if (P_VALID(m_pExpWnd)&&(!m_pExpWnd->IsInvisible()))
			{
				_stprintf(szTmpExpDPTime, g_StrTable[_T("MULTIPLETIPS_EXP")]);
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(szTmpExpDPTime);
			}
		}
		m_pExpWnd->SetInvisible(!bExpWndVisible);
		if( nRestTime_Exp >= 0 )
		{
			--nRestTime_Exp;
		}
		if( nRestTime_Exp < 0 )
			bExpWndVisible = FALSE;

		// 双倍物品掉落显示
		/*if( nRestTime_Trun == 0 )
		{
			_stprintf(szTmpItemDPTime, g_StrTable[_T("MULTIPLETIPS_TURN")]);
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(szTmpItemDPTime);
		}*/
		if( bTurnWndVisible )
		{
			_sntprintf(szTmpItemDPTime,sizeof(szTmpItemDPTime)/sizeof(TCHAR),_T("%d"),(nRestTime_Trun/60 + 1));
			m_pStcRestTurnTime->SetText(szTmpItemDPTime);
			m_pStcServerTurn->SetText(szTmpItemDPPercent);
			m_pTurnWnd->SetRefresh(TRUE);
		}
		else if (bActiveItemFlag)
		{
			bActiveItemFlag = FALSE;
			if (P_VALID(m_pTurnWnd)&&(!m_pTurnWnd->IsInvisible()))
			{
				_stprintf(szTmpItemDPTime, g_StrTable[_T("MULTIPLETIPS_TURN")]);
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(szTmpItemDPTime);
			}
		}
		m_pTurnWnd->SetInvisible(!bTurnWndVisible);
		if( nRestTime_Trun >= 0 )
		{
			--nRestTime_Trun;
		}
		if( nRestTime_Trun < 0 )
			bTurnWndVisible = FALSE;
	}
	GameFrame::Update();
}
DWORD MultipleTips::NetRecvServerDoubleInfo(tagNS_DoubleDropProbabilityChange* pNetCmd, DWORD dwParam)
{
	EDDPType  recvDoubleType=pNetCmd->eType;
	
	DWORD dwTime = timeGetTime();
	//DWORD dwRestMin = (timeGetTime()-dwTime)/60;
	TCHAR temBuf[X_SHORT_NAME] = {0};
 
	INT nMult = (INT)((pNetCmd->nProbability)*100.f);
	switch (recvDoubleType)
	{
	case EDDP_Exp:
		{
			bActiveExpFlag = TRUE;
			bExpWndVisible = pNetCmd->bOpen;
			if(bExpWndVisible)
			{
				nRestTime_Exp = pNetCmd->dwTimeLeft;
				if (nRestTime_Exp<=0)   // 当到达时间后屏显提示：“本次多倍经验活动结束”
				{
					//_stprintf(szTmpExpDPTime, g_StrTable[_T("MULTIPLETIPS_EXP")]);
				} 
				else
				{
					_sntprintf(szTmpExpDPTime,sizeof(szTmpExpDPTime)/sizeof(TCHAR),_T("%d"),(nRestTime_Exp/60));
					_tcscpy(temBuf,_T("%d "));
					_tcscat(temBuf,g_StrTable[_T("PERCENT")]);
					_sntprintf(szTmpExpDPPercent,sizeof(szTmpExpDPPercent)/sizeof(TCHAR),temBuf,nMult);
				}

			}
			//else
			//	m_pMgr->AddToDestroyList(this);
		}
		break;
	case EDDP_Item:
		{
			bActiveItemFlag = TRUE;
			bTurnWndVisible = pNetCmd->bOpen;
			if(bTurnWndVisible)
			{
				nRestTime_Trun = pNetCmd->dwTimeLeft;
				if (nRestTime_Trun<=0)   // 当到达时间后屏显提示：“本次多倍掉率活动结束”
				{
					//_stprintf(szTmpItemDPTime, g_StrTable[_T("MULTIPLETIPS_TURN")]);
				} 
				else
				{
					_sntprintf(szTmpItemDPTime,sizeof(szTmpItemDPTime)/sizeof(TCHAR),_T("%d"),(nRestTime_Trun/60));
					_tcscpy(temBuf,_T("%d "));
					_tcscat(temBuf,g_StrTable[_T("PERCENT")]);
					_sntprintf(szTmpItemDPPercent,sizeof(szTmpItemDPPercent)/sizeof(TCHAR),temBuf,nMult);
				}

			}
			//else
			//	m_pMgr->AddToDestroyList(this);

		}
		break;
	}
	return 0;
}

BOOL MultipleTips::ReloadUI()
{
	XmlElement ele;

	if (P_VALID(m_pTurnWnd))
		m_pGUI->AddToDestroyList(m_pTurnWnd);
 	tstring strPath1 = g_strLocalPath + _T("\\ui\\server_turn.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath1.c_str());
	m_pTurnWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pStcRestTurnTime = static_cast<GUIStatic*>(m_pTurnWnd->GetChild(_T("picback\\word1\\num")));
	m_pStcServerTurn   = static_cast<GUIStatic*>(m_pTurnWnd->GetChild(_T("picback\\num")));

	if (P_VALID(m_pExpWnd))
		m_pGUI->AddToDestroyList(m_pExpWnd);
	tstring strPath2 = g_strLocalPath + _T("\\ui\\server_exp.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath2.c_str());
	m_pExpWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pStcRestExpTime = static_cast<GUIStatic*>(m_pExpWnd->GetChild(_T("picback\\word1\\num")));
	m_pStcServerExp = static_cast<GUIStatic*>(m_pExpWnd->GetChild(_T("picback\\num")));

	return TRUE;

}
