#include "StdAfx.h"
#include "PickStrategyFrame.h"
#include "PickupMgr.h"
PickStrategyFrame::PickStrategyFrame(void):m_Trunk(this)
{

}

PickStrategyFrame::~PickStrategyFrame(void)
{

}

BOOL PickStrategyFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL ret = GameFrame::Init(pMgr,pFather,dwParam);

	SettingtoUI();
	return ret;
}
BOOL PickStrategyFrame::Destroy()
{
	BOOL ret = GameFrame::Destroy();
	if(P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}
	return ret;
}
BOOL PickStrategyFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\pickup_set.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;
	
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&PickStrategyFrame::EventHandler));

	m_pBnSave         = (GUIButton*)m_pWnd->GetChild( _T("pickup_backpic\\save") );
	m_pBnClose         = (GUIButton*)m_pWnd->GetChild( _T("pickup_backpic\\closebutt") );
	m_pBnExit         = (GUIButton*)m_pWnd->GetChild( _T("pickup_backpic\\close") );

	m_pBnSave->SetTips(_T(""));
	m_pBnClose->SetTips(_T(""));
	m_pBnExit->SetTips(_T(""));

	m_vecPushButtons.clear();
	GUIPushButton* pWnd = NULL;
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\openbutt7") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\openbutt6") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\openbutt0") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\openbutt1") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\openbutt2") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\openbutt3") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\openbutt4") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\openbutt5") );
	m_vecPushButtons.push_back(pWnd);

	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\colorback\\openbutt0") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\colorback\\openbutt1") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\colorback\\openbutt2") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\colorback\\openbutt3") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\colorback\\openbutt4") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\colorback\\openbutt5") );
	m_vecPushButtons.push_back(pWnd);
	pWnd= (GUIPushButton*)m_pWnd->GetChild( _T("pickup_backpic\\chooseback\\colorback\\openbutt6") );
	m_vecPushButtons.push_back(pWnd);

	return true;
}
BOOL PickStrategyFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return true;
}

void PickStrategyFrame::close()
{
	m_pFrameMgr->AddToDestroyList(this);
}

void PickStrategyFrame::UItoSetting()
{
	tagPickStrategy& data = PickupMgr::Inst()->GetPickStrategy();
	data.bMoney = ( EGUIBS_PushDown == m_vecPushButtons[EbMoney]->GetState() ) ? true : false;
	data.bEquip = ( EGUIBS_PushDown == m_vecPushButtons[EbEquip]->GetState() ) ? true : false;
	data.bMedicine = ( EGUIBS_PushDown == m_vecPushButtons[EbMedicine]->GetState() ) ? true : false;
	data.bStrengthen = ( EGUIBS_PushDown == m_vecPushButtons[EbStrengthen]->GetState() ) ? true : false;
	data.bGem = ( EGUIBS_PushDown == m_vecPushButtons[EbGem]->GetState() ) ? true : false;
	data.bQuest = ( EGUIBS_PushDown == m_vecPushButtons[EbQuest]->GetState() ) ? true : false;
	data.bSprite = ( EGUIBS_PushDown == m_vecPushButtons[EbSprite]->GetState() ) ? true : false;
	data.bOthers = ( EGUIBS_PushDown == m_vecPushButtons[EbOthers]->GetState() ) ? true : false;

	data.bWrite = ( EGUIBS_PushDown == m_vecPushButtons[EbWrite]->GetState() ) ? true : false;
	data.bYellow = ( EGUIBS_PushDown == m_vecPushButtons[EbYellow]->GetState() ) ? true : false;
	data.bGreen = ( EGUIBS_PushDown == m_vecPushButtons[EbGreen]->GetState() ) ? true : false;
	data.bBlue = ( EGUIBS_PushDown == m_vecPushButtons[EbBlue]->GetState() ) ? true : false;
	data.bOrange = ( EGUIBS_PushDown == m_vecPushButtons[EbOrange]->GetState() ) ? true : false;
	data.bPurple = ( EGUIBS_PushDown == m_vecPushButtons[EbPurple]->GetState() ) ? true : false;
	data.bPink = ( EGUIBS_PushDown == m_vecPushButtons[EbPink]->GetState() ) ? true : false;

	RefreshColour(data.bEquip);
}

void PickStrategyFrame::SettingtoUI()
{
	for (vector<GUIPushButton*>::iterator it = m_vecPushButtons.begin();
		it != m_vecPushButtons.end(); ++it)
	{
		(*it)->SetState( EGUIBS_PopUp, FALSE );
	}
	tagPickStrategy& data = PickupMgr::Inst()->GetPickStrategy();
	if(data.bMoney)			m_vecPushButtons[EbMoney]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bEquip)				m_vecPushButtons[EbEquip]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bMedicine)		m_vecPushButtons[EbMedicine]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bStrengthen)	m_vecPushButtons[EbStrengthen]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bGem)				m_vecPushButtons[EbGem]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bQuest)			m_vecPushButtons[EbQuest]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bSprite)			m_vecPushButtons[EbSprite]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bOthers)			m_vecPushButtons[EbOthers]->SetState( EGUIBS_PushDown, FALSE );

	if(data.bWrite)				m_vecPushButtons[EbWrite]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bYellow)			m_vecPushButtons[EbYellow]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bGreen)			m_vecPushButtons[EbGreen]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bBlue)				m_vecPushButtons[EbBlue]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bOrange)			m_vecPushButtons[EbOrange]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bPurple)			m_vecPushButtons[EbPurple]->SetState( EGUIBS_PushDown, FALSE );
	if(data.bPink)				m_vecPushButtons[EbPink]->SetState( EGUIBS_PushDown, FALSE );

	RefreshColour(data.bEquip);
}

BOOL PickStrategyFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if ( pWnd == m_pBnSave)
			{
				UItoSetting();
				PickupMgr::Inst()->SavePickStrategy();
			} 
			else if ( pWnd == m_pBnExit)
			{
				m_pFrameMgr->AddToDestroyList(this);
			}
			else if ( pWnd == m_pBnClose)
			{
				m_pFrameMgr->AddToDestroyList(this);
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if ( pWnd == m_vecPushButtons[EbEquip])
			{
				RefreshColour(EGUIBS_PushDown == m_vecPushButtons[EbEquip]->GetState());
			} 
		}
	default:
		break;
	}
	return TRUE;
}

void PickStrategyFrame::RefreshColour(bool b)
{
	m_vecPushButtons[EbWrite]->SetEnable(  b );
	m_vecPushButtons[EbYellow]->SetEnable(  b );
	m_vecPushButtons[EbGreen]->SetEnable(  b );
	m_vecPushButtons[EbBlue]->SetEnable(  b );
	m_vecPushButtons[EbOrange]->SetEnable(  b );
	m_vecPushButtons[EbPurple]->SetEnable(  b );
	m_vecPushButtons[EbPink]->SetEnable(  b );

	m_vecPushButtons[EbWrite]->SetRefresh(  true );
	m_vecPushButtons[EbYellow]->SetRefresh(  true );
	m_vecPushButtons[EbGreen]->SetRefresh(  true );
	m_vecPushButtons[EbBlue]->SetRefresh(  true );
	m_vecPushButtons[EbOrange]->SetRefresh(  true );
	m_vecPushButtons[EbPurple]->SetRefresh(  true );
	m_vecPushButtons[EbPink]->SetRefresh(  true );
}