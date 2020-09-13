#include "stdafx.h"
#include "TreasureChestFrame.h"
#include "IconStatic.h"
#include "CombatSysUtil.h"
#include "ItemProtoData.h"
#include "ItemMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "ItemButton.h"

#define One_Minnus 1000*60
const DWORD BASE_TIME = 100;
const DWORD BASE_STEP = 50;
const INT BASE_DIS = 7;

const TCHAR* szChestSelect = _T("data\\ui\\Common\\L_icon-l.bmp");
const TCHAR* szChestInit = _T("data\\ui\\common\\l_icon.bmp");
const TCHAR* szChestItem = _T("data\\ui\\main\\l_icon_back.bmp");

TreasureChestFrame::TreasureChestFrame( void ):m_Trunk(this)
{
	m_pWnd				= NULL;
	m_pPatWaption		= NULL;
	m_pBtnMFunction		= NULL;
	//m_pBtnAgain			= NULL;
	for( int i=0; i<sci_ItemNum; i++ )
	{
		m_pStcAuto[i]	= NULL;
		m_pStcItem[i]	= NULL;
	}
	m_pLastAuto			= NULL;
	m_eOperateState		= EOperateState_NULL;
	m_dwStopTime		= GT_INVALID;
	m_dwItemTypeID		= GT_INVALID;
	//m_dwStopCount		= 0;
	m_nCurPos			= 0;
	m_nFinalPos			= GT_INVALID;
	m_dwRunTime			= GT_INVALID;
	m_dwStartTime		= GT_INVALID;
	m_nItemCount		= GT_INVALID;
	m_bCircleAgain		= FALSE;
	m_pStcFinalItem		= NULL;
	m_pBtnGetItem		= NULL;
	m_pStcFinalBack		= NULL;
	m_dwChestID = GT_INVALID;
	m_vecItem.clear();
}

TreasureChestFrame::~TreasureChestFrame( void )
{
	
}

BOOL TreasureChestFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{	
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	//--注册游戏事件
	m_pFrameMgr->RegisterEventHandle(_T("tagEOSAaginEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&TreasureChestFrame::OnEOSAaginEvent));

	//--注册网络消息
	m_pCmdMgr->Register("NS_StartTreasureChest",	(NETMSGPROC)m_Trunk.sfp2(&TreasureChestFrame::OnNS_StartTreasureChest),	_T("NS_StartTreasureChest"));
	m_pCmdMgr->Register("NS_TreasureChest",			(NETMSGPROC)m_Trunk.sfp2(&TreasureChestFrame::OnNS_TreasureChest),		_T("NS_TreasureChest"));
	m_pCmdMgr->Register("NS_StopTreasureChest",		(NETMSGPROC)m_Trunk.sfp2(&TreasureChestFrame::OnNS_StopTreasureChest),	_T("NS_StopTreasureChest"));
	m_pCmdMgr->Register("NS_AgainTreasureChest",	(NETMSGPROC)m_Trunk.sfp2(&TreasureChestFrame::OnNS_AgainTreasureChest),	_T("NS_AgainTreasureChest"));
	m_pCmdMgr->Register("NS_ChestGetItem",			(NETMSGPROC)m_Trunk.sfp2(&TreasureChestFrame::OnNS_ChestGetItem),	_T("NS_ChestGetItem"));

	return TRUE;
}

BOOL TreasureChestFrame::Destroy()
{
	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);

	m_vecItem.clear();

	//--注册游戏事件
	m_pFrameMgr->UnRegisterEventHandler(_T("tagEOSAaginEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&TreasureChestFrame::OnEOSAaginEvent));
	//--注销网络消息
	m_pCmdMgr->UnRegister("NS_StartTreasureChest",		(NETMSGPROC)m_Trunk.sfp2(&TreasureChestFrame::OnNS_StartTreasureChest));
	m_pCmdMgr->UnRegister("NS_TreasureChest",			(NETMSGPROC)m_Trunk.sfp2(&TreasureChestFrame::OnNS_TreasureChest));
	m_pCmdMgr->UnRegister("NS_StopTreasureChest",		(NETMSGPROC)m_Trunk.sfp2(&TreasureChestFrame::OnNS_StopTreasureChest));
	m_pCmdMgr->UnRegister("NS_AgainTreasureChest",		(NETMSGPROC)m_Trunk.sfp2(&TreasureChestFrame::OnNS_AgainTreasureChest));
	m_pCmdMgr->UnRegister("NS_ChestGetItem",			(NETMSGPROC)m_Trunk.sfp2(&TreasureChestFrame::OnNS_ChestGetItem));

	if( P_VALID(GetObj("EOSAaginMsgBox")) ) 
		KillObj("EOSAaginMsgBox");
	if( P_VALID(GetObj("ChestBadMsgBoxEx")) ) 
		KillObj("ChestBadMsgBoxEx");

	return GameFrame::Destroy();
}

void TreasureChestFrame::InitTreasureChestUI()
{
	m_eOperateState = EOperateState_NULL;
 	m_pBtnMFunction->SetText(g_StrTable[_T("Chest_Start")]);
	m_pBtnMFunction->SetUserData(0, EOperateState_NULL);
	m_pBtnMFunction->SetEnable(true);
	m_pBtnGetItem->SetEnable(false);
	//m_pBtnAgain->SetEnable(false);
	//m_pBtnAgain->SetUserData(0, EOperateState_NULL);
	//m_dwStopCount = 0;
	m_dwRunTime = GT_INVALID;
	for( int i=0; i<sci_ItemNum; i++ )
	{
		m_pStcAuto[i]->SetPic(szChestInit);
		m_pStcItem[i]->RefreshItem();
		//m_pStcItem[i]->SetPic(szChestItem);
	}
	m_pStcFinalItem->RefreshItem();
	m_vecItem.clear();
	m_vecItem.resize(sci_ItemNum);
}

void TreasureChestFrame::RefreshUI()
{
	if( P_VALID(m_pLastAuto) )
		m_pLastAuto->SetPic(szChestInit);
	/*for( int i=0; i<sci_ItemNum; i++ )
	{
		if( P_VALID(m_pStcAuto[i]) )
		{
			m_pStcAuto[i]->SetPic(szChestInit);
		}
	}*/
}

void TreasureChestFrame::SetTreasureChestUI()
{
	for( int i=0; i<(int)m_vecItem.size(); i++ )
	{
		//tstring strIcon = ItemMgr::Inst()->GetItemIconPath(m_vecItem[i].first);	
		//if( !strIcon.empty() ) 
		//{
		//	m_pStcItem[i]->SetPic(strIcon);

		//	TCHAR szNum[5];
		//	_stprintf(szNum, _T("%4d"), m_vecItem[i].second);
		//	m_pStcItem[i]->SetText(szNum);
		//}

		m_pStcItem[i]->RefreshItem(m_vecItem[i].dwTypeId, m_vecItem[i].nNum, m_vecItem[i].byQuality);
	}
}

BOOL TreasureChestFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\chest.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	char szFullName[X_LONG_NAME] = {0};
	for(int i=0; i<sci_ItemNum; ++i)
	{
		_snprintf( szFullName, X_LONG_NAME, "chest_win\\back\\item%d_back", i );
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "IconStatic");
		_snprintf( szFullName, X_LONG_NAME, "chest_win\\back\\item%d_back\\item", i );
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "ItemButton");
	}
	m_pGUI->ChangeXml(&ele, "chest_win\\back\\final\\itemback\\item", "ClassName", "ItemButton");
	m_pWnd				= m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pWnd->SetInvisible(TRUE);

	//注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&TreasureChestFrame::EventHandler));
	
	m_pPatWaption		= (GUIStatic*)m_pWnd->GetChild(_T("back"));
	m_pBtnMFunction		= (GUIButton*)m_pWnd->GetChild(_T("back\\startbutton"));
	m_pBtnMFunction->SetText(g_StrTable[_T("Chest_Start")]);
	m_pBtnMFunction->SetUserData(0, EOperateState_NULL);
	//m_pBtnAgain			= (GUIButton*)m_pWnd->GetChild(_T("chest_pic\\new0"));
	//m_pBtnAgain->SetText(g_StrTable[_T("Chest_Retry")]);
	//m_pBtnAgain->SetEnable(false);
	//m_pBtnAgain->SetUserData(0, EOperateState_NULL);
	m_pStcFinalBack = (GUIButton*)m_pWnd->GetChild(_T("back\\final"));
	m_pStcFinalBack->SetInvisible(TRUE);
	m_pBtnGetItem = (GUIButton*)m_pWnd->GetChild(_T("back\\final\\choosebutton"));
	m_pBtnGetItem->SetEnable(FALSE);

	m_pStcFinalItem = (ItemButton*)m_pWnd->GetChild(_T("back\\final\\itemback\\item"));
	TCHAR szBuff[X_LONG_NAME] = {0};
	for(int i=0; i<sci_ItemNum; i++)
	{
		_sntprintf( szBuff, X_LONG_NAME, _T("back\\item%d_back"), i );
		m_pStcAuto[i] = (IconStatic*)m_pWnd->GetChild(szBuff);
		_sntprintf( szBuff, X_LONG_NAME, _T("back\\item%d_back\\item"), i );
		m_pStcItem[i] = (ItemButton*)m_pWnd->GetChild(szBuff);
	}
	return TRUE;
}

VOID TreasureChestFrame::Update()
{
	if( m_pWnd->IsInvisible() )
		return;
	switch( m_eOperateState )
	{
	case EOperateState_Begin:		
		{
			//1分钟后无操作,发送停止
			DWORD dwTime = Kernel::Inst()->GetAccumTimeDW();
			if( dwTime-m_dwStartTime >= One_Minnus )
			{
				this->SendStopTreasureMsg();
				m_dwStartTime = dwTime;
				return;
			}

			if( timeGetTime() - m_dwRunTime > BASE_TIME )
			{
				this->RefreshUI();
				if( m_nCurPos >= 0 )
				{
					m_pStcAuto[m_nCurPos]->SetPic(szChestSelect);
					m_pLastAuto = m_pStcAuto[m_nCurPos];
				}
				++m_nCurPos;
				if( m_nCurPos >= sci_ItemNum )
					m_nCurPos = 0;
				m_dwRunTime = timeGetTime();
			}
		}
		break;
	case EOperateState_Stop:
		{
			DWORD dwTime = GT_INVALID;
			INT nDis = m_nFinalPos - m_nCurPos;
			INT nCount = m_nCurPos - m_nFinalPos;
			if( nDis > BASE_DIS || nCount <= sci_ItemNum - BASE_DIS - 1 )
				m_bCircleAgain = FALSE;

			switch(nDis)
			{
			case 0:
				dwTime = BASE_TIME + BASE_STEP * 9;
				break;
			case 1:
				dwTime = BASE_TIME + BASE_STEP * 8;
				break;
			case 2:
				dwTime = BASE_TIME + BASE_STEP * 7;
				break;
			case 3:
				dwTime = BASE_TIME + BASE_STEP * 6;
				break;
			case 4:
				dwTime = BASE_TIME + BASE_STEP * 5;
				break;
			case 5:
				dwTime = BASE_TIME + BASE_STEP * 4;
				break;
			case 6:
				dwTime = BASE_TIME + BASE_STEP * 3;
				break;
			case 7:
				dwTime = BASE_TIME + BASE_STEP * 2;
				break;
			default:
				dwTime = BASE_TIME + BASE_STEP;
				break;
			}
			if( timeGetTime() - m_dwStopTime > dwTime )
			{
				this->RefreshUI();
				m_pStcAuto[m_nCurPos]->SetPic(szChestSelect);
				m_pLastAuto = m_pStcAuto[m_nCurPos];
				if( m_nCurPos == m_nFinalPos && 
					m_vecItem[m_nCurPos].dwTypeId == m_dwItemTypeID && 
					m_vecItem[m_nCurPos].nNum == m_nItemCount && m_bCircleAgain == FALSE )
				{
					/*tstring strIcon = ItemMgr::Inst()->GetItemIconPath(m_vecItem[m_nFinalPos].first);	
					if( !strIcon.empty() ) 
					{
						m_pStcFinalItem->SetPic(strIcon);
						TCHAR szNum[5];
						_stprintf(szNum, _T("%4d"), m_vecItem[m_nFinalPos].second);
						m_pStcFinalItem->SetText(szNum);
						m_pStcFinalBack->SetInvisible(FALSE);
					}*/
					m_pStcFinalItem->RefreshItem(m_vecItem[m_nFinalPos].dwTypeId, m_vecItem[m_nFinalPos].nNum, m_vecItem[m_nFinalPos].byQuality );
					if( m_pBtnMFunction->GetUserData(0) == EOperateState_NULL  )
						m_pBtnMFunction->SetEnable(true);
					m_pBtnGetItem->SetEnable(true);
					m_pStcFinalBack->SetInvisible(FALSE);
					return;
				}
				else
				{
					++m_nCurPos;
					if( m_nCurPos >= sci_ItemNum )
						m_nCurPos = 0;
					m_dwStopTime = timeGetTime();
				}
			}
		}
		break;
	}
	GameFrame::Update();
}

void TreasureChestFrame::SetCaption(DWORD dwChestTypeID)
{
	if( P_VALID(m_pPatWaption) )
	{
		const tagItemDisplayInfo* pChestInfo = ItemProtoData::Inst()->FindItemDisplay(dwChestTypeID);
		if( P_VALID(pChestInfo) )
		{
			m_pPatWaption->SetText(pChestInfo->szName);
		}
		else
		{
			m_pPatWaption->SetText(g_StrTable[_T("DigBao18")]);
		}
	}
}

BOOL TreasureChestFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;
	switch(pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnMFunction )
			{
				switch( m_eOperateState )
				{
				case EOperateState_NULL:
					{
						tagNC_StartTreasureChest msg;
						msg.n64ChestID	= m_dwChestID;
						m_pSession->Send(&msg);
						m_pBtnMFunction->SetEnable(false);
					}
					break;
				case EOperateState_Begin:
					this->SendStopTreasureMsg();
					break;
				case EOperateState_Stop:
					{
						if( P_VALID(GetObj("EOSAaginMsgBox")) ) 
							KillObj("EOSAaginMsgBox");
						CreateObj("EOSAaginMsgBox", "MsgBox");

						TObjRef<MsgBox>("EOSAaginMsgBox")->Init(_T(""), g_StrTable[_T("DigBao17")], 
							_T("tagEOSAaginEvent"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);
					}
					break;
				}
			}
			else if( pWnd == m_pBtnGetItem )
				this->SendGetTreasureMsg();
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if (_tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0)
			{
				for( int i=0; i<sci_ItemNum; i++ )
				{
					if( pWnd==m_pStcItem[i] && i<(int)m_vecItem.size() )
					{
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_vecItem[i].dwTypeId, m_vecItem[i].byQuality ).c_str());
					}
				}
			}
			if( pWnd == m_pStcFinalItem && m_nFinalPos != GT_INVALID && m_nFinalPos < (INT)m_vecItem.size() )
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_vecItem[m_nFinalPos].dwTypeId, m_vecItem[m_nFinalPos].byQuality).c_str());
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	default:
		break;
	}

	return TRUE;
}

void TreasureChestFrame::SetOperateState( EOperateState eState )
{
	m_eOperateState = eState;
	switch( m_eOperateState )
	{
	case EOperateState_Begin:
		{
			m_pPatWaption->SetText(g_StrTable[_T("DigBao19")]);
			m_pBtnMFunction->SetText(g_StrTable[_T("DigBao19")]);
			m_pBtnMFunction->SetEnable(true);
			m_pBtnGetItem->SetEnable(false);
			//m_pStcFinalItem->SetPic(szChestItem);
			//m_pStcFinalItem->SetText(_T(""));
			m_nCurPos = m_pUtil->Rand()%sci_ItemNum;
			m_dwStartTime = Kernel::Inst()->GetAccumTimeDW();
			m_pStcFinalBack->SetInvisible(TRUE);
		}
		break;
	case EOperateState_Stop:
		{
			m_pPatWaption->SetText(g_StrTable[_T("DigBao20")]);
			m_pBtnMFunction->SetText(g_StrTable[_T("Chest_Retry")]);
			//if( m_pBtnAgain->GetUserData(0) == EOperateState_NULL  )
			//	m_pBtnAgain->SetEnable(true);
		}
		break;
	case EOperateState_Again:
		break;
	}
	m_pWnd->SetRefresh(TRUE);
}

DWORD TreasureChestFrame::OnEOSAaginEvent( tagMsgBoxEvent* pGameEvent )
{
	//再次一试,50%机率,宝箱毁坏,必须向服务器发消息
	if( pGameEvent->eResult == MBF_OK )
	{
		tagNC_AgainTreasureChest msg;
		m_pSession->Send(&msg);
	}
	return 0;
}

void TreasureChestFrame::SendStopTreasureMsg()
{
	tagNC_StopTreasureChest msg;
	m_pSession->Send(&msg);
	m_pBtnMFunction->SetEnable(false);
}

void TreasureChestFrame::SendGetTreasureMsg()
{
	tagNC_ChestGetItem msg;
	msg.dwTypeID = m_dwItemTypeID;
	m_pSession->Send(&msg);
	m_nItemCount = GT_INVALID;

	m_pBtnMFunction->SetEnable(false);
	m_pBtnGetItem->SetEnable(false);
	//m_pBtnAgain->SetEnable(false);
	m_eOperateState = EOperateState_NULL;
	m_pWnd->SetRefresh(TRUE);
	m_bCircleAgain = FALSE;
}

DWORD TreasureChestFrame::OnNS_TreasureChest( tagNS_TreasureChest* pMsg, DWORD dwParam )
{
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			m_dwChestID = pMsg->n64ChestID;
			this->SetCaption(pMsg->dwChestTypeID);
			//保存物品TypeID(未完成)

			this->InitTreasureChestUI();	
			m_vecItem.clear();
			map<INT,tagTreasure> mapRandItem;
			for( int i=0; i<MAX_CHEST_NUM; i++ )
			{
				tagTreasure chestITem;
				chestITem.dwTypeId = pMsg->dwTypeID[i];
				chestITem.nNum = pMsg->nNum[i];
				chestITem.byQuality = pMsg->byQuality[i];
				mapRandItem.insert(make_pair(m_pUtil->Rand(), chestITem));
			}
			map<INT,tagTreasure>::iterator it = mapRandItem.begin();
			while(it!=mapRandItem.end())
			{
				m_vecItem.push_back(it->second);
				++it;
			}
			mapRandItem.clear();
			this->SetTreasureChestUI();
			m_pWnd->SetInvisible(FALSE);
			m_pWnd->FlipToTop();
			m_pStcFinalBack->SetInvisible(TRUE);
		}
		break;
	case E_Bag_NotEnoughSpace:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr40")]);
		}
		break;
	case E_SystemError:
		{			
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("DigBao21")]);
		}
		break;
	default:
		break;
	}
	return 0;
}

DWORD TreasureChestFrame::OnNS_StopTreasureChest( tagNS_StopTreasureChest* pMsg, DWORD dwParam )
{
	m_dwItemTypeID	= pMsg->dwTypeID;
	m_nItemCount	= pMsg->nNum;
	m_dwStopTime	= ::timeGetTime();
	for( size_t i = 0; i < m_vecItem.size(); i++ )
	{
		if( m_vecItem[i].dwTypeId == m_dwItemTypeID && m_vecItem[i].nNum == m_nItemCount )
		{
			m_nFinalPos = i;
			break;
		}
	}
	INT nDis = m_nFinalPos - m_nCurPos;
	INT nCount = m_nCurPos - m_nFinalPos;
	if( nDis <= BASE_DIS || nCount > sci_ItemNum - BASE_DIS - 1 )
		m_bCircleAgain = TRUE;

	this->SetOperateState(EOperateState_Stop);
	return 0;
}

DWORD TreasureChestFrame::OnNS_AgainTreasureChest( tagNS_AgainTreasureChest* pMsg, DWORD dwParam )
{
	if( pMsg->byDestroy == 0 )
	{
		this->SetOperateState(EOperateState_Begin);
		m_pBtnMFunction->SetUserData(0, EOperateState_Again);
		//m_pBtnAgain->SetEnable(false);
		//m_pBtnAgain->SetUserData(0, EOperateState_Again);
		//m_dwStopCount = 0;
	}
	else
	{
		if( P_VALID(GetObj("ChestBadMsgBoxEx")) ) 
			KillObj("ChestBadMsgBoxEx");
		CreateObj("ChestBadMsgBoxEx", "MsgBoxEx");

		TObjRef<MsgBoxEx>("ChestBadMsgBoxEx")->Init(_T(""), g_StrTable[_T("DigBao16")], 
			_T("tagChestBadEvent"), (MsgBoxFlag)(MBF_Cancel), TRUE);
		this->InitTreasureChestUI();
		m_pWnd->SetInvisible(TRUE);
		m_pStcFinalBack->SetInvisible(TRUE);
		m_dwItemTypeID = GT_INVALID;
	}
	return 0;
}

DWORD TreasureChestFrame::OnNS_ChestGetItem( tagNS_ChestGetItem* pMsg, DWORD dwParam )
{
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			m_nFinalPos = GT_INVALID;
			m_pStcFinalBack->SetInvisible(TRUE);
			m_pWnd->SetInvisible(TRUE);
			m_dwItemTypeID = GT_INVALID;
		}
		break;
	default:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PocketNoSpace")]);
			m_pStcFinalBack->SetInvisible(false);
			m_pBtnGetItem->SetEnable(TRUE);
			m_pBtnGetItem->SetRefresh(TRUE);
		}
		break;
	}
	return 0;
}

DWORD TreasureChestFrame::OnNS_StartTreasureChest( tagNS_StartTreasureChest* pMsg, DWORD dwParam )
{
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			this->SetOperateState(EOperateState_Begin);
		}
		break;
	}
	return 0;
}