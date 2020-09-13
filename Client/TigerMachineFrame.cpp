#include "StdAfx.h"
#include "TigerMachineFrame.h"
#include "RollStatic.h"
#include "..\WorldDefine\msg_tiger.h"
#include "..\WorldDefine\tiger_define.h"
#include "Container.h"
#include "CombatSysUtil.h"
#include "ItemMgr.h"
#include "ChangeStatic.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"

const TCHAR* szTiger= _T("data\\ui\\slots\\slots2.bmp");
const TCHAR* szTigerMain= _T("data\\ui\\slots\\slots1.bmp");

TigerMachineFrame::TigerMachineFrame(void):m_trunk(this)
,m_dwNPCID(GT_INVALID)
,m_escapeTime(0)
{
}

TigerMachineFrame::~TigerMachineFrame(void)
{
}

BOOL TigerMachineFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register( "NS_GetTigerResult",		(NETMSGPROC)m_trunk.sfp2(&TigerMachineFrame::OnNS_GetTigerResult),		_T("NS_GetTigerResult"));
	m_pCmdMgr->Register( "NS_ItemAdd",		(NETMSGPROC)m_trunk.sfp2(&TigerMachineFrame::NetItemAdd),		_T("NS_ItemAdd"));
	m_pCmdMgr->Register( "NS_NewItemAdd",		(NETMSGPROC)m_trunk.sfp2(&TigerMachineFrame::NetNewItemAdd),		_T("NS_NewItemAdd"));
	for (int i =0; i< TAGER_NUM; i++)
	{
		m_pStTiger[i]->SetPic(szTiger);
		m_pStTiger[i]->InitData();

		m_iStopNum[i] = 0;
	}
	
	m_pIsLights->SetPic(szTigerMain);
	m_pIsLights->SetWAndH(530,259);
	m_pIsLights->PushPoint(0,735);
	m_pIsLights->PushPoint(533,735);
	m_pIsLights->PushPoint(0,1002);
	m_pIsLights->SetTimeSpace(1000);
	m_pIsLights->InitData();
	m_pIsLights->Loop(true);

	m_dwLightCount = 5;


	m_nType = 1;
	m_bStarted =false;
	m_pBnX1->SetState(EGUIBS_PushDown);
	m_pWnd->SetInvisible(TRUE);
	m_bStopMark = false;
	m_nCurWin = 0;
	m_bRolling = false;

	return TRUE;
}
BOOL TigerMachineFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pCmdMgr->UnRegister( "NS_GetTigerResult",							(NETMSGPROC)m_trunk.sfp2(&TigerMachineFrame::OnNS_GetTigerResult) );
	m_pCmdMgr->UnRegister( "NS_ItemAdd",							(NETMSGPROC)m_trunk.sfp2(&TigerMachineFrame::NetItemAdd) );
	m_pCmdMgr->UnRegister( "NS_NewItemAdd",							(NETMSGPROC)m_trunk.sfp2(&TigerMachineFrame::NetNewItemAdd) );


	if( P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}


	return bRet;
}
BOOL TigerMachineFrame::ReloadUI()
{


	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\slots.xml");
	if( !m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &ele, "slotswin\\slots\\pic0", "ClassName", "RollStatic");
	m_pGUI->ChangeXml( &ele, "slotswin\\slots\\pic1", "ClassName", "RollStatic");
	m_pGUI->ChangeXml( &ele, "slotswin\\slots\\pic2", "ClassName", "RollStatic");
	m_pGUI->ChangeXml( &ele, "slotswin\\slots\\numarea\\lights", "ClassName", "ChangeStatic");

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &ele );
	if( !P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}

	m_pStTiger[0]= (RollStatic*)m_pWnd->GetChild(_T("slots\\pic0"));
	m_pStTiger[1]= (RollStatic*)m_pWnd->GetChild(_T("slots\\pic1"));
	m_pStTiger[2]= (RollStatic*)m_pWnd->GetChild(_T("slots\\pic2"));

	m_pBnX1 = (GUIPushButton*)m_pWnd->GetChild(_T("slots\\frame\\1time"));
	m_pBnX5 = (GUIPushButton*)m_pWnd->GetChild(_T("slots\\frame\\5times"));
	m_pBnX10 = (GUIPushButton*)m_pWnd->GetChild(_T("slots\\frame\\10times"));
	m_pBnX50 = (GUIPushButton*)m_pWnd->GetChild(_T("slots\\frame\\50times"));

	m_pBnStart = (GUIPushButton*)m_pWnd->GetChild(_T("slots\\start"));
	m_pBnClose = (GUIButton*)m_pWnd->GetChild(_T("slots\\close"));

	m_pStCurrWin= (GUIStatic*)m_pWnd->GetChild(_T("slots\\win"));
	m_pStTotal= (GUIStatic*)m_pWnd->GetChild(_T("slots\\total"));

	m_pIsLights = (ChangeStatic*)m_pWnd->GetChild(_T("slots\\numarea\\lights"));
		
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_trunk.sfp1(&TigerMachineFrame::EventHandler));


	return TRUE;
}
BOOL TigerMachineFrame::EscCancel()
{
	if (!m_pWnd->IsInvisible())
	{
		m_pWnd->SetInvisible(TRUE);
		if (m_bRolling)
		{
			GenerateRandomResult(0);
			m_nCurNum = 0;
			m_bRolling = false;
			for (int i =0; i< TAGER_NUM;i++)
			{
				m_pStTiger[i]->StopImmediately(m_iStopNum[i]);
			}
			m_pBnStart->SetState(EGUIBS_PopUp,TRUE,FALSE);
		}

		return TRUE;
	}
	return FALSE;
}

VOID TigerMachineFrame::Update()
{

	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_dwLightCount++;
		if(5 == m_dwLightCount)
		{
			m_pIsLights->SetTimeSpace(1000);
		}
// 		if (m_bRolling)
// 		{
// 			Cool3D::AudioSystem::Inst()->Play2DEventSound( _T("data\\sound\\mess\\roll.wav"), 10 );
// 		}
		m_escapeTime = 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{

			return;
		}
		if (RoleMgr::Inst()->IsOutValidDist( m_dwNPCID ) )
		{
			m_pWnd->SetInvisible(TRUE);
		}

	}

	if (m_bStopMark)
	{
		m_bRolling = false;
		if (m_nCurNum < TAGER_NUM)
		{
			m_dwStopTime += Kernel::Inst()->GetDeltaTimeDW();
			if (m_dwStopTime > 300)
			{
				m_pStTiger[m_nCurNum]->StopImmediately(m_iStopNum[m_nCurNum]);
				Cool3D::AudioSystem::Inst()->Play2DEventSound( _T("data\\sound\\mess\\stop.wav"), 10 );
				m_dwStopTime = 0;
				m_nCurNum++;
			}
		}
		else
		{
			m_bStopMark = false;
			m_pBnStart->SetEnable(true);
			RefreshProfitUI();
		}
			
	}
}

BOOL TigerMachineFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBnClose)
			{
				m_pWnd->SetInvisible(TRUE);
				if (m_bRolling)
				{
					GenerateRandomResult(0);
					m_nCurNum = 0;
					m_bRolling = false;
					for (int i =0; i< TAGER_NUM;i++)
					{
						m_pStTiger[i]->StopImmediately(m_iStopNum[i]);
					}
					m_pBnStart->SetState(EGUIBS_PopUp,TRUE,FALSE);
				}
				
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if (pWnd == m_pBnX1)
				m_nType = 1;
			else if(pWnd == m_pBnX5)
				m_nType = 5;
			else if(pWnd == m_pBnX10)
				m_nType = 10;
			else if(pWnd == m_pBnX50)
				m_nType = 50;
			else if (pWnd == m_pBnStart)
			{
				if (m_bStarted)
				{
					tagNC_GetTigerResult cmd;
					cmd.nType = m_nType;
					m_pSession->Send(&cmd);

					m_pBnStart->SetEnable(false);

				}
				else
				{
					if(!CheckItem(m_nType))	 
					{
						m_pBnStart->SetState((m_pBnStart->GetState() == EGUIBS_PushDown?EGUIBS_PopUp :EGUIBS_PushDown),TRUE,FALSE);
						return FALSE;
					}

					for (int i = 0; i < TAGER_NUM ; i++)
					{
						m_pStTiger[i]->Start();
					}

					m_bRolling = true;
				}

				m_bStarted = !m_bStarted;
			}
		}
		break;
	}
	return TRUE;
}

DWORD TigerMachineFrame::OnNS_GetTigerResult(tagNS_GetTigerResult* pNetCmd,DWORD)
{
	if (pNetCmd->dwErrorCode == E_Tiger_success)
	{
		GenerateRandomResult(pNetCmd->dwResult);
		m_nCurWin = pNetCmd->dwNum;

		m_nCurNum = 0;
		m_bStopMark = true;
		m_dwStopTime = 1000;
		
		if (pNetCmd->dwResult > 1 && pNetCmd->dwResult <5)
		{
			Cool3D::AudioSystem::Inst()->Play2DEventSound( _T("data\\sound\\mess\\win.wav"), 10 );
		}
		else if (pNetCmd->dwResult == 1)
		{
			Cool3D::AudioSystem::Inst()->Play2DEventSound( _T("data\\sound\\mess\\win1.wav"), 10 );
		}
	}

	return 0;
}

void TigerMachineFrame::GenerateRandomResult(int nResult)
{
	if(!nResult)
		m_dwLightCount = 0;

	if (E_Tiger_Result_Null == nResult )
	{
		int random = rand() % (4);
		m_iStopNum[0] = random;

		random = rand() % (4);
		while(m_iStopNum[0]  == random)
			random = rand() % (4);
		
		m_iStopNum[1] = random;

		random = rand() % (4);
		while (m_iStopNum[0]  == random || m_iStopNum[1]  == random)
			random = rand() % (4);

		m_iStopNum[2] = random;

	}
	else if (E_Tiger_Result_bounty1 == nResult)
	{
		m_iStopNum[0] = 3;
		m_iStopNum[1] = 3;
		m_iStopNum[2] = 3;
		m_pIsLights->SetTimeSpace(100);
	}
	else if (E_Tiger_Result_bounty2 == nResult)
	{
		m_iStopNum[0] = 0;
		m_iStopNum[1] = 0;
		m_iStopNum[2] = 0;
		m_pIsLights->SetTimeSpace(100);
	}
	else if (E_Tiger_Result_bounty3 == nResult)
	{
		m_iStopNum[0] = 1;
		m_iStopNum[1] = 1;
		m_iStopNum[2] = 1;
		m_pIsLights->SetTimeSpace(100);
	}
	else if (E_Tiger_Result_bounty4 == nResult)
	{
		m_iStopNum[0] = 2;
		m_iStopNum[1] = 2;
		m_iStopNum[2] = 2;
		m_pIsLights->SetTimeSpace(100);
	}
	else if (E_Tiger_Result_bounty5== nResult)
	{
		m_iStopNum[0] = 4;
		m_iStopNum[1] = 4;
		m_iStopNum[2] = 4;
		m_pIsLights->SetTimeSpace(500);
	}
	else if (E_Tiger_Result_bounty6 == nResult)
	{
		m_iStopNum[0] = 4;
		m_iStopNum[1] = 4;
		m_iStopNum[2] = 4;
		int pos = rand() % (3);
		m_iStopNum[pos] = rand() % (4);

		m_pIsLights->SetTimeSpace(500);
	}
	else if (E_Tiger_Result_bounty7 == nResult)
	{
		m_iStopNum[0] = rand() % (4);
		m_iStopNum[1] = rand() % (4);
		m_iStopNum[2] = rand() % (4);

		int pos = rand() % (3);
		m_iStopNum[pos] = 4;

		m_pIsLights->SetTimeSpace(500);
	}
}

bool TigerMachineFrame::CheckItem(int nType)
{
	ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
	if(!P_VALID(pContainer))		return false;
	Item* pItem = pContainer->GetSpecFuncItem(EICT_Bag,EISF_TigerCoins);
	if(!P_VALID(pItem))		
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Tiger_NotChooseGoods")]);
		return false;
	}
	else
	{
		int nItemCount = pContainer->GetItemQuantity(pItem->GetItemTypeID());
		if (nItemCount < nType)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Tiger_NotChooseGoods")]);
			return false;
		}
	}

	return true;
}

void TigerMachineFrame::RefreshProfitUI()
{
	int nItemCount = 0;
	ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
	if(!P_VALID(pContainer))		return;
	Item* pItem = pContainer->GetSpecFuncItem(EICT_Bag,EISF_TigerCoins);
	if(!P_VALID(pItem))		
	{
		m_pStTotal->SetText(_T("0"));
		m_pStTotal->SetRefresh(true);
		return;
	}

	nItemCount = pContainer->GetItemQuantity(pItem->GetItemTypeID());
	TCHAR buffer[64];
	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), nItemCount);
	m_pStTotal->SetText(buffer);
	m_pStTotal->SetRefresh(true);

	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), m_nCurWin);
	m_pStCurrWin->SetText(buffer);
	m_pStCurrWin->SetRefresh(true);
}

// 增加一个已知物品
DWORD TigerMachineFrame::NetItemAdd(tagNS_ItemAdd* pNetCmd, DWORD)
{
	RefreshProfitUI();
	return 0;
}
// 新增一个物品
DWORD TigerMachineFrame::NetNewItemAdd(tagNS_NewItemAdd* pNetCmd, DWORD)
{
	RefreshProfitUI();
	return 0;
}

void TigerMachineFrame::ShowFrame(BOOL b)
{
	m_pWnd->SetInvisible(!b); 
	if(b) 
	{
		RefreshProfitUI();
		m_bStarted =false;
		m_pBnX1->SetState(EGUIBS_PushDown);
		m_bStopMark = false;
		m_nCurWin = 0;
		m_bRolling = false;
	}
}