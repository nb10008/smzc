#include "StdAfx.h"
#include "Item.h"
#include "RoleMgr.h"
#include "GameSet.h"
#include "ItemMgr.h"
#include "RoleEvent.h"
#include "Container.h"
#include "ItemEvent.h"
#include "ItemFrame.h"
#include "IconStatic.h"
#include "ToolTipFrame.h"
#include "PlayerNameTab.h"
#include "ToolTipCreator.h"
#include "GameClientDefine.h"
#include "CombatSysUtil.h"
#include "FairyAutoFeedFrame.h"
#include "CombatEvent.h"
#include "..\WorldDefine\msg_loot.h"

const TCHAR* szItemInit = _T("data\\ui\\common\\L_diban-b.bmp");

FairyAutoFeedFrame::FairyAutoFeedFrame(void):
m_Trunk(this),
m_pWnd(NULL),
m_dwMainFood(GT_INVALID),
m_dwSecFood(GT_INVALID),
m_nMoodRate(80),
m_nTime(6),
m_dwEscapeTime(0),
m_nFairyMood(0),
m_bAutoFeed(false),
m_bWarning(false)
{
}

FairyAutoFeedFrame::~FairyAutoFeedFrame()
{

}

VOID FairyAutoFeedFrame::Update()
{
	if(!m_bAutoFeed)
		return;
	m_dwEscapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if(m_dwEscapeTime < DWORD(m_nTime * 60 * 1000))
		return;
	Equipment *pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	const tagFabaoSpec *pTrumpSpe = pEquip->GetFaBaoSpe();
	if( !P_VALID(pEquip) || !P_VALID(pTrumpSpe))
	{
		m_dwEscapeTime = 0;
		return;
	}

	if(pTrumpSpe->n16Stage < 30 && P_VALID(m_pWnd))
	{
		ShowUI(FALSE);
		m_dwEscapeTime = 0;
		return;
	}
	m_nFairyMood = pTrumpSpe->nMood;

	if( m_nFairyMood > 1000 * m_nMoodRate / 100 )
		m_dwEscapeTime = 0;

	DWORD dwFoodID = GT_INVALID;
	if( m_dwMainFood != GT_INVALID )
		dwFoodID = m_dwMainFood;
	else if( m_dwSecFood != GT_INVALID )
		dwFoodID = m_dwSecFood;
	else if( m_bWarning )
	{
		if( P_VALID(GetObj("NoFairyFood")) )
			KillObj("NoFairyFood");
		CreateObj( "NoFairyFood", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("NoFairyFood")->Init( _T(""), g_StrTable[_T("NoFairyFood")], _T("OnNoFairyFood") );
		m_bWarning = false;
	}
	if( dwFoodID != GT_INVALID )
	{
		tagNC_FabaoFeeding m;
		m.dwFoodTypeID = dwFoodID;
		m_pSession->Send(&m);
	}
	m_dwEscapeTime = 0;
}

BOOL FairyAutoFeedFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{
	if( !GameFrame::Init( pMgr, pFather, dwParam) )
		return FALSE;

	m_pMgr->RegisterEventHandle(_T("ChangeFairy"),						(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnEventChangeFairy));
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2FairyAutoFeed"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnEventPutItem));
	m_pMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnRoleGetNameEvent));
	m_pMgr->RegisterEventHandle(_T("UpdateFairyMood"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnUpdateFairyMood));
	m_pMgr->RegisterEventHandle(_T("OnNoFairyFood"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnNoFairyFood));
	m_pMgr->RegisterEventHandle(_T("tagUseFairyFoodEvent"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnUseFairyFood));
	m_pCmdMgr->Register( "NS_FabaoFeeding",								(NETMSGPROC)m_Trunk.sfp2(&FairyAutoFeedFrame::NetFairyFeed),			_T("NS_FabaoFeeding"));
	m_pCmdMgr->Register( "NS_RolePutDownItem",							(NETMSGPROC)m_Trunk.sfp2(&FairyAutoFeedFrame::NetRolePutDownItem),		_T("NS_RolePutDownItem"));
	LoadFromFile();
	return TRUE;
}

BOOL FairyAutoFeedFrame::Destroy()
{
	GameFrame::Destroy();
	if(P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);
	m_pMgr->UnRegisterEventHandler(_T("ChangeFairy"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnEventChangeFairy));
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2FairyAutoFeed"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnEventPutItem));
	m_pMgr->UnRegisterEventHandler(_T("tagRoleGetNameEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnRoleGetNameEvent));
	m_pMgr->UnRegisterEventHandler(_T("UpdateFairyMood"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnUpdateFairyMood));
	m_pMgr->UnRegisterEventHandler(_T("OnNoFairyFood"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnNoFairyFood));
	m_pMgr->UnRegisterEventHandler(_T("tagUseFairyFoodEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyAutoFeedFrame::OnUseFairyFood));
	m_pCmdMgr->UnRegister( "NS_FabaoFeeding",							(NETMSGPROC)m_Trunk.sfp2(&FairyAutoFeedFrame::NetFairyFeed));
	m_pCmdMgr->UnRegister( "NS_RolePutDownItem",						(NETMSGPROC)m_Trunk.sfp2(&FairyAutoFeedFrame::NetRolePutDownItem));
	SaveSettingToFile();
	if( P_VALID(GetObj("NoFairyFood")) )
		KillObj("NoFairyFood");
	return TRUE;
}

DWORD FairyAutoFeedFrame::OnUseFairyFood( tagUseFairyFoodEvent *pGameEvent )
{
	if(IfCanEat(pGameEvent->dwItemTypeID))
	{
		tagNC_FabaoFeeding m;
		m.dwFoodTypeID = pGameEvent->dwItemTypeID;
		m_pSession->Send(&m);
	}
	return 0;
}

DWORD FairyAutoFeedFrame::OnNoFairyFood( tagMsgBoxEvent *pGameEvent )
{
	if( pGameEvent->eResult == MBF_OK && m_pWnd->IsInvisible() )
		ShowUI(TRUE);
	return 0;
}

DWORD FairyAutoFeedFrame::OnUpdateFairyMood(tagFairyMood* pEvent)
{
	m_nFairyMood = pEvent->nMood;
	return 0;
}

DWORD FairyAutoFeedFrame::OnRoleGetNameEvent(tagRoleGetNameEvent* pEvent)
{
	if( pEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		ReadSettingFromFile();
		//UpdateUI(m_dwMainFood);
		//UpdateUI(m_dwSecFood);
		TCHAR szTemp[X_SHORT_NAME] = {0};
		_sntprintf(szTemp, X_SHORT_NAME, _T("%d"), m_nMoodRate);
		m_pStcMoodRate->SetText(szTemp);
		_sntprintf(szTemp, X_SHORT_NAME, _T("%d"), m_nTime);
		m_pStcIntervalTime->SetText(szTemp);
		if( GameSet::Inst()->Get(EGS_FairyAutoFeed) )
		{
			m_bAutoFeed = true;
			m_pBtnAuto->SetText(g_StrTable[_T("FairyAutoFeed_Off")]);
		}
		else
		{
			m_bAutoFeed = false;
			m_pBtnAuto->SetText(g_StrTable[_T("FairyAutoFeed_On")]);
		}
		//Equipment *pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Face);
		//if( P_VALID(pEquip) )
		//{
		//	const tagFabaoSpec *pTrumpSpe = pEquip->GetFaBaoSpe();
		//	if(P_VALID(pTrumpSpe))
		//		m_nFairyMood = pTrumpSpe->nMood;
		//}
	}
	
	return 0;
}

BOOL FairyAutoFeedFrame::EscCancel()
{
	if(m_pWnd->IsInvisible() )
		return FALSE;
	ShowUI(false);
	return TRUE;
}

BOOL FairyAutoFeedFrame::ReloadUI()
{
	XmlElement e;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\foraging.xml");
	if( !m_pGUI->LoadXml( &e, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &e, "forage_win\\forage_backpic\\itemback0\\item", "ClassName", "IconStatic" );
	m_pGUI->ChangeXml( &e, "forage_win\\forage_backpic\\itemback1\\item", "ClassName", "IconStatic" );
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &e );
	if(!P_VALID(m_pWnd))
		return FALSE;

	m_pWnd->SetInvisible(TRUE);
	m_pStcMoodRate = (GUIEditBox*)m_pWnd->GetChild(_T("forage_backpic\\word0\\num"));
	m_pStcMoodRate->SetMaxValue(99);
	m_pStcIntervalTime = (GUIEditBox*)m_pWnd->GetChild(_T("forage_backpic\\word1\\num"));
	m_pStcIntervalTime->SetMaxValue(120);

	m_pIconMainFood = (IconStatic*)m_pWnd->GetChild(_T("forage_backpic\\itemback0\\item"));
	m_pIconMainFood->SetPic(szItemInit);
	m_pStcMainFoodName = (GUIStatic*)m_pWnd->GetChild(_T("forage_backpic\\item0_name"));
	m_pStcMainFoodNum = (GUIStatic*)m_pWnd->GetChild(_T("forage_backpic\\num0"));

	m_pIconSecFood = (IconStatic*)m_pWnd->GetChild(_T("forage_backpic\\itemback1\\item"));
	m_pIconSecFood->SetPic(szItemInit);
	m_pStcSecFoodName = (GUIStatic*)m_pWnd->GetChild(_T("forage_backpic\\item1_name"));
	m_pStcSecFoodNum = (GUIStatic*)m_pWnd->GetChild(_T("forage_backpic\\num1"));

	m_pBtnAuto = (GUIButton*)m_pWnd->GetChild(_T("forage_backpic\\savebutton"));
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild(_T("forage_backpic\\closebutt"));
	m_pBtnCancle = (GUIButton*)m_pWnd->GetChild(_T("forage_backpic\\closebutton"));

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&FairyAutoFeedFrame::EventHandler) );
	return TRUE;
}
 

//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL FairyAutoFeedFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBtnClose || pWnd == m_pBtnCancle )
			ShowUI(false);
		else if( pWnd == m_pBtnAuto )
		{
			if( GameSet::Inst()->Get(EGS_FairyAutoFeed) )
			{
				m_pBtnAuto->SetText(g_StrTable[_T("FairyAutoFeed_On")]);
				GameSet::Inst()->Set( EGS_FairyAutoFeed, false );
				GameSet::Inst()->SaveAutoFeedSet();
				m_bAutoFeed = false;
			}
			else
			{
				m_dwEscapeTime = 0;
				m_pBtnAuto->SetText(g_StrTable[_T("FairyAutoFeed_Off")]);
				GameSet::Inst()->Set( EGS_FairyAutoFeed, true );
				GameSet::Inst()->SaveAutoFeedSet();
				m_bAutoFeed = true;
			}
		}
		break;
	case EGUIE_KeyDown:
		{
			if( m_pGUI->GetActive() == m_pStcMoodRate )
			{
				m_nMoodRate = _tcstol(m_pStcMoodRate->GetText(), NULL, 10);
				if(m_nMoodRate == 0)
				{
					m_nMoodRate = 1;
					m_pStcMoodRate->SetText(_T("1"));
				}
				SaveSettingToFile();
			}
			else if( m_pGUI->GetActive() == m_pStcIntervalTime )
			{
				m_nTime = _tcstol(m_pStcIntervalTime->GetText(), NULL, 10);
				if(m_nTime == 0)
				{
					m_nTime = 1;
					m_pStcIntervalTime->SetText(_T("1"));
				}
				SaveSettingToFile();
			}
		}
		break;
	case EGUISCB_Drop:
		if( pWnd == m_pIconMainFood )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
			{
				m_dwMainFood = GT_INVALID;
				PutItem((INT16)LOWORD(pEvent->dwParam2));
			}
		}
		else if( pWnd == m_pIconSecFood )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutItem((INT16)LOWORD(pEvent->dwParam2), false);
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{	
			if( pWnd == m_pIconMainFood && m_dwMainFood != GT_INVALID )
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_dwMainFood).c_str());
			else if( pWnd == m_pIconSecFood && m_dwSecFood != GT_INVALID )
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_dwSecFood).c_str());
		}
		break;
	}
	return TRUE;
}

DWORD FairyAutoFeedFrame::OnEventPutItem( tagItemPosChangeExEvent* pGameEvent )
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;
	PutItem( pGameEvent->n16IndexSrc );
	return 0;
}

DWORD FairyAutoFeedFrame::OnEventChangeFairy(tagGameEvent* pGameEvent)
{
	Equipment *pFairy = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	if( !P_VALID(pFairy) )
		return 0;

	const tagFabaoSpec *pFairySpe = pFairy->GetFaBaoSpe();
	const tagFabaoProto *pFairyProto = (tagFabaoProto*)pFairy->GetProto();
	if(!P_VALID(pFairySpe) && !P_VALID(pFairyProto) )
		return 0;
	if(!IfCanEat(m_dwMainFood, true, false))
		m_dwMainFood = GT_INVALID;
	if(!IfCanEat(m_dwSecFood, true, false))
		m_dwSecFood = GT_INVALID;
	UpdateUI(m_dwMainFood);
	UpdateUI(m_dwSecFood);
	return 0;
}


VOID FairyAutoFeedFrame::PutItem( INT16 nPos, bool bMainFood /* = true */ )
{
	Item *pItem = ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(pItem))
		return;

	const tagItemProto* pItemProto = pItem->GetProto();
	TCHAR szTemp[X_LONG_NAME] = {0};
	if( !P_VALID(pItemProto) )
		return;

	if(!IfCanEat(pItemProto->dwTypeID, true))
		return; 
	if( m_dwMainFood == GT_INVALID && bMainFood )
	{
		m_dwMainFood = pItemProto->dwTypeID;
		if(m_dwSecFood == m_dwMainFood)
		{
			m_dwMainFood = GT_INVALID;
			return;
		}
		UpdateUI(m_dwMainFood);
	}
	else
	{
		m_dwSecFood = pItemProto->dwTypeID;
		if(m_dwSecFood == m_dwMainFood)
		{
			m_dwSecFood = GT_INVALID;
			return;
		}
		UpdateUI(m_dwSecFood);
	}
	SaveSettingToFile();
}

VOID FairyAutoFeedFrame::UpdateUI( DWORD &dwFoodID )
{
	IconStatic *pItemIcon = NULL;
	GUIStatic *pItemName = NULL;
	GUIStatic *pItemNum = NULL;
	if( dwFoodID == m_dwMainFood )
	{
		pItemIcon = m_pIconMainFood;
		pItemName = m_pStcMainFoodName;
		pItemNum = m_pStcMainFoodNum;
	}
	else
	{
		pItemIcon = m_pIconSecFood;
		pItemName = m_pStcSecFoodName;
		pItemNum = m_pStcSecFoodNum;
	}
	
	ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
	INT nNum = pContainer->GetItemQuantity(dwFoodID);
	if( nNum != 0 )
	{
		Item *pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(dwFoodID);
		if(!P_VALID(pItem))
			return;
		tstring strIcon = ItemMgr::Inst()->GetItemIconPath(dwFoodID);	
		pItemIcon->SetPic(strIcon);
		pItemName->SetText(pItem->GetItemName().c_str());
		TCHAR szTemp[X_LONG_NAME] = {0};
		_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), nNum );
		pItemNum->SetText(szTemp);
		pItemNum->SetRefresh(TRUE);
	}
	else
	{
		dwFoodID = GT_INVALID;
		pItemIcon->SetPic(szItemInit);
		pItemName->SetText(_T(""));
		pItemNum->SetText(_T(""));
		SaveSettingToFile();
	}
	if( m_dwMainFood == GT_INVALID && m_dwSecFood == GT_INVALID )
		m_bWarning = true;
}

VOID FairyAutoFeedFrame::ShowUI(bool bShow)
{
	if(P_VALID(m_pWnd))
		m_pWnd->SetInvisible(!bShow);
	if(bShow)
		SetStrategy(EIST_FairyAutoFeed);
	else 
	{
		ItemFrame* pFrame = (ItemFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Item"));
		if(P_VALID(pFrame) && EIST_FairyAutoFeed == pFrame->GetCurStrategy() )
			SetStrategy(EIST_ItemPocket);
	}
}

VOID FairyAutoFeedFrame::ReadSettingFromFile()
{
	LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return;
	DWORD dwLPID = pLp->GetID();
	tstring strPath = Kernel::Inst()->GetWorkPath();
	tstring strLocalName = PlayerNameTab::Inst()->FindNameByID(dwLPID);
	strPath += _T("config\\");
	strPath += strLocalName;
	strPath += _T("\\");
	strPath += _T("autoFeed.dat");

	FILE *fp= _tfopen( strPath.c_str(), _T("rb") );
	if( fp != NULL )
	{
		DWORD dwRoleID = GT_INVALID;
		fread(&dwRoleID, sizeof(DWORD), 1, fp);
		if(dwRoleID!=dwLPID)
			return;
		fread(&m_nMoodRate, sizeof(INT), 1, fp);
		fread(&m_nTime, sizeof(INT), 1, fp);
		fread(&m_dwMainFood, sizeof(DWORD), 1, fp);
		fread(&m_dwSecFood, sizeof(DWORD), 1, fp);
		fclose(fp);
	}
}

VOID FairyAutoFeedFrame::SaveSettingToFile()
{
	tstring strPath = Kernel::Inst()->GetWorkPath();
	DWORD dwLPID = RoleMgr::Inst()->GetLocalPlayerID();
	tstring strLocalName = PlayerNameTab::Inst()->FindNameByID(dwLPID);
	if( strLocalName.empty() )
		return;

	strPath += _T("config\\");
	strPath += strLocalName;
	strPath += _T("\\");

	TCHAR DirName[256];
	const TCHAR* p = strPath.c_str();
	TCHAR* q = DirName;	

	while(*p)
	{
		if ((_T('\\') == *p) || (_T('/') == *p))
		{
			if (_T(':') != *(p-1))
			{
				CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = _T('\0');
	}
	CreateDirectory(DirName, NULL);

	strPath += _T("autoFeed.dat");

	FILE* fp = NULL;
	fp=_tfopen( strPath.c_str(), _T("wb") );

	if( fp != NULL)
	{
		DWORD dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();
		fwrite(&dwRoleID, sizeof(DWORD), 1, fp);
		fwrite(&m_nMoodRate, sizeof(INT), 1, fp);
		fwrite(&m_nTime, sizeof(INT), 1, fp);
		fwrite(&m_dwMainFood, sizeof(DWORD), 1, fp);
		fwrite(&m_dwSecFood, sizeof(DWORD), 1, fp);
		fclose(fp);
	}
}

VOID FairyAutoFeedFrame::LoadFromFile()
{
	m_mapFairyFood.clear();
	tstring strPath = _T("data\\system\\attdata\\fabaofood_proto.xml");
	list<tstring> FairyFoodList;
	TObjRef<VarContainer> con = CreateObj( "FairyFood", "VarContainer" );
	if(!con->Load("VFS_System", strPath.c_str(), "id", &FairyFoodList))
	{
		IMSG(_T("Load file fabaofood_proto.xml failed\r\n"));
		return;
	}
	list<tstring>::iterator end = FairyFoodList.end();
	for (list<tstring>::iterator iter = FairyFoodList.begin(); iter != end; ++iter)
	{
		DWORD dwTypeID = con->GetDword(_T("item_id"), iter->c_str(), 0);
		INT nLevel = con->GetInt(_T("level"), iter->c_str(), 0);
		m_mapFairyFood.insert(make_pair(dwTypeID, nLevel));
	}
	KillObj("FairyFood");
}

bool FairyAutoFeedFrame::IfCanEat(DWORD dwTypeID, bool bKnownFairy /* = false */, bool bShowTips /* = true */ )
{
	map<DWORD, INT>::const_iterator iter = m_mapFairyFood.find(dwTypeID);
	if( iter == m_mapFairyFood.end() )
	{
		//if(bShowTips)
		//	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotFeedFairy")]);
		return false;
	}
		
	Equipment *pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	if( P_VALID(pEquip) )
	{
		const tagFabaoSpec *pTrumpSpe = pEquip->GetFaBaoSpe();
		Item *pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(dwTypeID);
		if(!P_VALID(pTrumpSpe) || !P_VALID(pItem))
			return false;
		if(pTrumpSpe->n16Stage < iter->second)
		{
			if(bShowTips)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyLevelTooLow")]);
			return false;
		}
		else if( pTrumpSpe->nMood >= 1000 && !bKnownFairy && 
			pItem->GetProto()->eSpecFunc == EISF_Null )
		{
			if(bShowTips)
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyMoodIsFull")]);
			return false;
		}
	}
	else if( !bKnownFairy )
	{
		if(bShowTips)
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotFeed")]);
		return false;
	}
	return true;
}


DWORD FairyAutoFeedFrame::NetRolePutDownItem( tagNS_RolePutDownItem *pNetCmd, DWORD )
{
	if( pNetCmd->dwErrorCode == E_Success )
	{
		Item *pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(m_dwMainFood);
		if(!P_VALID(pItem))
			UpdateUI(m_dwMainFood);

		Item *ppItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(m_dwSecFood);
		if(!P_VALID(ppItem))
			UpdateUI(m_dwSecFood);
	}
	return 0;
}

DWORD FairyAutoFeedFrame::NetFairyFeed( tagNS_FabaoFeeding *pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode == E_Success)
	{
		tagFairyExcited e(_T("tagFairyExcited"), this);
		e.dwBuffID = pNetCmd->dwBuffID;
		m_pMgr->SendEvent(&e);
	}
	UpdateUI(m_dwMainFood);
	UpdateUI(m_dwSecFood);
	return 0;
}