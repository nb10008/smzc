#include "StdAfx.h"
#include "NPC.h"
#include "Role.h"
#include "RoleMgr.h"
#include "ItemMgr.h"
#include "Container.h"
#include "IconStatic.h"
#include "ToolTipFrame.h"
#include "CombatSysUtil.h"
#include "ToolTipCreator.h"
#include "FairyTrainFrame.h"
#include "../WorldDefine/msg_combat.h"
#include "GUIEffectMgr.h"
#include "EffectMgr.h"
#include "LocalPlayer.h"

FairyTrainFrame::FairyTrainFrame(void):
m_Trunk(this),
m_pWnd(NULL),
m_pStcFairyName(NULL),
m_pStcCurTameVale(NULL),
m_pProTameRate(NULL),
m_pStcMaxTameValue(NULL),
m_pTameItem(NULL),
m_pProTime(NULL),
m_pBtnClose(NULL),
m_pBtnQuit(NULL),
m_dwEscapeTime(0),
m_bChoosing(false)
{
	for( int i = 0; i < 5; i++ )
	{
		m_pItem[i] = NULL;
		m_pBtnItemNum[i] = NULL;
		m_dwItemTypeID[i] = GT_INVALID;
	}
}

FairyTrainFrame::~FairyTrainFrame()
{

}

BOOL FairyTrainFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{
	if( !GameFrame::Init( pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register("NS_ArrestingFairy", (NETMSGPROC)m_Trunk.sfp2(&FairyTrainFrame::OnNS_ArrestingFairy), _T("NS_ArrestingFairy"));
	m_pCmdMgr->Register("NS_OpenFairyTrain", (NETMSGPROC)m_Trunk.sfp2(&FairyTrainFrame::OnNS_OpenFairyTrain), _T("NS_OpenFairyTrain"));
	m_pMgr->RegisterEventHandle(_T("tagShowDeadMenuEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyTrainFrame::OnLPDeadEvent));
	return TRUE;
}

BOOL FairyTrainFrame::Destroy()
{
	CloseAllUiEffect();
	GameFrame::Destroy();
	if(P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);

	tagNC_CloseFairyTrain msg;
	msg.dwFairyID	= m_dwNPCID;
	m_pSession->Send(&msg);

	m_pCmdMgr->UnRegister("NS_ArrestingFairy", (NETMSGPROC)m_Trunk.sfp2(&FairyTrainFrame::OnNS_ArrestingFairy));
	m_pCmdMgr->UnRegister("NS_OpenFairyTrain", (NETMSGPROC)m_Trunk.sfp2(&FairyTrainFrame::OnNS_OpenFairyTrain));
	m_pMgr->UnRegisterEventHandler(_T("tagShowDeadMenuEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyTrainFrame::OnLPDeadEvent));
	return TRUE;
}

BOOL FairyTrainFrame::ReloadUI()
{
	XmlElement e;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\contract.xml");
	if( !m_pGUI->LoadXml( &e, "VFS_System", strPath.c_str() ) )
		return FALSE;

	for( int i = 0; i < 5; i++ )
	{
		CHAR szTemp[X_LONG_NAME];
		_snprintf( szTemp, X_LONG_NAME, "contractwin\\contract_pic\\listback00\\pic_back%d\\pic", i + 1 );
		m_pGUI->ChangeXml( &e, szTemp, "ClassName", "IconStatic" );
	}
	m_pGUI->ChangeXml( &e, "contractwin\\contract_pic\\listback00\\pic_back0\\pic", "ClassName", "IconStatic" );
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &e );
	if(!P_VALID(m_pWnd))
		return FALSE;

	m_pStcFairyName = (GUIStatic*)m_pWnd->GetChild(_T("contract_pic\\listback0\\word0\\word"));
	m_pProTameRate = (GUIProgress*)m_pWnd->GetChild(_T("contract_pic\\listback0\\timepic\\time"));
	m_pStcCurTameVale = (GUIStatic*)m_pWnd->GetChild(_T("contract_pic\\listback0\\num"));
	m_pStcMaxTameValue = (GUIStatic*)m_pWnd->GetChild(_T("contract_pic\\listback0\\word2\\num"));

	m_pTameItem = (IconStatic*)m_pWnd->GetChild(_T("contract_pic\\listback00\\pic_back0\\pic"));
	m_pTameItem->SetPic(_T("data\\ui\\common\\l_icon.bmp"));
	m_pProTime = (GUIProgress*)m_pWnd->GetChild(_T("contract_pic\\listback00\\timepic\\time"));

	m_pPcBack = (GUIPatch*)m_pWnd->GetChild(_T("contract_pic\\listback00"));
	for( int i = 0; i < 5; i++ )
	{
		TCHAR szTemp[X_LONG_NAME];
		_sntprintf( szTemp, X_LONG_NAME, _T("contract_pic\\listback00\\pic_back%d\\pic"), i + 1 );
		m_pItem[i] = (IconStatic*)m_pWnd->GetChild(szTemp);
		m_pItem[i]->SetPic(_T("data\\ui\\common\\l_icon.bmp"));
		_sntprintf( szTemp, X_LONG_NAME, _T("contract_pic\\listback00\\butt%d"), i);
		m_pBtnItemNum[i] = (GUIButton*)m_pWnd->GetChild(szTemp);
	}

	m_pBtnClose = (GUIButton*)m_pWnd->GetChild(_T("contract_pic\\closebutt"));
	m_pBtnQuit = (GUIButton*)m_pWnd->GetChild(_T("contract_pic\\butt"));
	// 注册事件处理函数
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&FairyTrainFrame::EventHandler) );
	return TRUE;
}


//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL FairyTrainFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBtnClose || pWnd == m_pBtnQuit )
			m_pMgr->AddToDestroyList(this);
		else
		{
			for( size_t i = 0; i < 5; i++ )
			{
				if(pWnd == m_pBtnItemNum[i] && i < m_vecItemID.size() )
				{
					tagNC_ArrestingFairy msg;
					msg.dwItemID = m_vecItemID[i];
					msg.dwFairyID = m_dwNPCID;
					m_pSession->Send(&msg);
					m_bChoosing = false;
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{	
			for( size_t i = 0; i < 5; i++ )
			{
				if(pWnd == m_pItem[i] && i < m_vecItemID.size() )
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_vecItemID[i]).c_str());
			}
		}
	}
	return TRUE;
}

VOID FairyTrainFrame::Update()
{
	m_dwEscapeTime += Kernel::Inst()->GetDeltaTimeDW();
	//每隔1秒检测距离目标NPC的距离
	if( m_dwEscapeTime > 1000 )
	{
		m_dwEscapeTime = 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{
			Destroy();
		}
		else if(RoleMgr::Inst()->IsOutValidDist(m_dwNPCID))
		{
			m_pMgr->AddToDestroyList(this);
		}
	}

	if(m_bChoosing)
	{
		float deltaTime =  Cool3D::Kernel::Inst()->GetDeltaTime();
		float fcur = m_pProTime->GetCurrentValue();
		float fMax = m_pProTime->GetMaxValue();
		if(fcur < fMax)
		{
			fcur += deltaTime;
			m_pProTime->SetValue(fcur, true);
			m_pProTime->SetRefresh();
		}
		if(fcur >= fMax)
		{
			tagNC_ArrestingFairy msg;
			msg.dwItemID = GT_INVALID;
			msg.dwFairyID = m_dwNPCID;
			m_pSession->Send(&msg);
			m_bChoosing = false;
		}
	}
}

DWORD FairyTrainFrame::OnNS_OpenFairyTrain( tagNS_OpenFairyTrain* pMsg, DWORD dwParam )
{
	DWORD *pItemID = pMsg->dwItemID;
	for(int i = 0; i < pMsg->nItemKind && i < 5 ; i++)
	{
		m_dwItemTypeID[i] = pItemID[i];
	}
	Role* pRole = RoleMgr::Inst()->FindRole(m_dwNPCID);
	if(!P_VALID(pRole))
		return 0;
	m_pStcFairyName->SetText(pRole->GetRoleName().c_str());
	TCHAR szTemp[X_SHORT_NAME];
	_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), pMsg->nMaxContractValue );
	m_pStcMaxTameValue->SetText(szTemp);
	m_nMaxTameValue = pMsg->nMaxContractValue;
	m_nCurTameValue = 0;
	m_dwCurItemID = pMsg->dwCurItemID;
	UpdateUI();
	return 0;
}

DWORD FairyTrainFrame::OnNS_ArrestingFairy(tagNS_ArrestingFairy* pMsg, DWORD dwParam)
{
	if( pMsg->dwErrorCode == E_Success )
	{
		if( !pMsg->bDone )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("SelectRightItem")]);
			PlayEffect();
		}
		else
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("TrainFairySucc")]);
			m_pMgr->AddToDestroyList(this);
		}
	}
	else if(pMsg->dwErrorCode == E_UseItem_ItemNotExist )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("FairySoulNotExit")]);
		m_pMgr->AddToDestroyList(this);
	}
	else
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("SelectWrongItem")]);
	m_dwCurItemID = pMsg->dwItemID;
	m_nCurTameValue = pMsg->nCurContractValue;
	UpdateUI();
	return 0;
}

VOID FairyTrainFrame::UpdateUI()
{
	TCHAR szTemp[X_SHORT_NAME];
	_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_nCurTameValue );
	m_pStcCurTameVale->SetText(szTemp);
	m_pProTameRate->SetMaxValue((float)m_nMaxTameValue);
	m_pProTameRate->SetValue((float)m_nCurTameValue, true);
	m_vecItemID.clear();
	map<INT, DWORD> mapTemp;
	for( int i = 0; i < 5; i++ )
		mapTemp.insert(make_pair(m_pUtil->Rand(), m_dwItemTypeID[i]));
	map<INT,DWORD>::iterator it = mapTemp.begin();
	while( it != mapTemp.end() )
	{
		m_vecItemID.push_back(it->second);
		++it;
	}
	mapTemp.clear();
	ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
	for( size_t j = 0; j < m_vecItemID.size(); j++ )
	{
		tstring strIcon = ItemMgr::Inst()->GetItemIconPath(m_vecItemID[j]);	
		if( !strIcon.empty() ) 
		{
			m_pItem[j]->SetPic(strIcon);
		}
		INT nNum = pContainer->GetItemQuantity(m_vecItemID[j]);
		if(nNum == 0)
			m_pBtnItemNum[j]->SetEnable(false);
		else
			m_pBtnItemNum[j]->SetEnable(true);
		_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), nNum );
		m_pBtnItemNum[j]->SetText(szTemp);
		m_pBtnItemNum[j]->SetRefresh(TRUE);
	}
	tstring strIcon = ItemMgr::Inst()->GetItemIconPath(m_dwCurItemID);	
	if( !strIcon.empty() ) 
	{
		m_pTameItem->SetPic(strIcon);
	}
	m_pProTime->SetSpeed(100);
	m_pProTime->SetValue(0, true);
	if( m_nCurTameValue > m_nMaxTameValue/2 )
		m_pProTime->SetMaxValue(2.0f);
	else
		m_pProTime->SetMaxValue(3.0f);
	m_pProTime->SetRefresh();
	m_bChoosing = true;
}

DWORD FairyTrainFrame::OnLPDeadEvent(tagShowDeadMenuEvent* pEvent)
{
	m_pMgr->AddToDestroyList(this);
	return 0;
}
VOID FairyTrainFrame::CloseAllUiEffect()
{
	GUIEffectMgr::Inst()->RemoveOneEffectByType(GUIEffectMgr::EET_Sprite);
}

VOID FairyTrainFrame::PlayEffect()
{
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(plp))
		return;

	GUIEffectMgr::Inst()->PlayGuiEffect(_T("txui026"),m_pPcBack,GUIEffectMgr::EET_Sprite);

	EffectMgr::Inst()->PlayRoleEquipEffect(plp->GetID(),_T("txfzt33"), _T("Bip01 R Forearm") ,EffectMgr::EBET_Wrist_R);
	EffectMgr::Inst()->PlayRoleEquipEffect(plp->GetID(),_T("txfzt33"), _T("Bip01 L Forearm") ,EffectMgr::EBET_Wrist_L);

	EffectMgr::Inst()->PlayRoleEffect(m_dwNPCID,_T("txfzt34"));
}