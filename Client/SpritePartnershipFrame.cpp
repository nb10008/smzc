#include "StdAfx.h"
#include "SpritePartnershipFrame.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "Item.h"
#include "ItemProtoData.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "ItemFrame.h"
#include "ItemEvent.h"
#include "ItemMgr.h"
#include "CombatSysUtil.h"
SpritePartnershipFrame::SpritePartnershipFrame(void):m_Trunk(this)
,m_dwNPCID(GT_INVALID)
,m_escapeTime(0)
,m_bOtherPalyerConform(false)
,m_eState(EBS_WaitSpriteConform)
,m_pItem(NULL)
,m_bCancelConform(false)
{
}

SpritePartnershipFrame::~SpritePartnershipFrame(void)
{
}
BOOL SpritePartnershipFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2PartnerShip"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SpritePartnershipFrame::OnEventMove2SpritePart));
	m_pCmdMgr->Register( "NS_PutFairyInto",		(NETMSGPROC)m_Trunk.sfp2(&SpritePartnershipFrame::OnNS_PutFairyInto),		_T("NS_PutFairyInto"));
	m_pCmdMgr->Register( "NS_TransmitPutFairyInto",		(NETMSGPROC)m_Trunk.sfp2(&SpritePartnershipFrame::OnNS_TransmitPutFairyInto),		_T("NS_TransmitPutFairyInto"));
	m_pCmdMgr->Register( "NS_TransmitFairyMating",		(NETMSGPROC)m_Trunk.sfp2(&SpritePartnershipFrame::OnNS_TransmitFairyMating),		_T("NS_TransmitFairyMating"));
	m_pCmdMgr->Register( "NS_FairyMating",		(NETMSGPROC)m_Trunk.sfp2(&SpritePartnershipFrame::OnNS_FairyMating),		_T("NS_FairyMating"));
	m_pCmdMgr->Register( "NS_FairyMatingCancelAll",		(NETMSGPROC)m_Trunk.sfp2(&SpritePartnershipFrame::OnNS_FairyMatingCancelAll),		_T("NS_FairyMatingCancelAll"));
	SetStrategy(EIST_FairyPartnerShip);
	return TRUE;
}
BOOL SpritePartnershipFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	if( P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}

	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2PartnerShip"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SpritePartnershipFrame::OnEventMove2SpritePart));
	m_pCmdMgr->UnRegister( "NS_PutFairyInto",							(NETMSGPROC)m_Trunk.sfp2(&SpritePartnershipFrame::OnNS_PutFairyInto) );
	m_pCmdMgr->UnRegister( "NS_TransmitPutFairyInto",							(NETMSGPROC)m_Trunk.sfp2(&SpritePartnershipFrame::OnNS_TransmitPutFairyInto) );
	m_pCmdMgr->UnRegister( "NS_TransmitFairyMating",							(NETMSGPROC)m_Trunk.sfp2(&SpritePartnershipFrame::OnNS_TransmitFairyMating) );
	m_pCmdMgr->UnRegister( "NS_FairyMating",							(NETMSGPROC)m_Trunk.sfp2(&SpritePartnershipFrame::OnNS_FairyMating) );
	m_pCmdMgr->UnRegister( "NS_FairyMatingCancelAll",							(NETMSGPROC)m_Trunk.sfp2(&SpritePartnershipFrame::OnNS_FairyMatingCancelAll) );

	LockItem(false);
	SetStrategy(EIST_ItemPocket);
	return bRet;
}
BOOL SpritePartnershipFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\fairy_counterpart.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml( &element, "counterpart_win\\counterpart_back\\pic\\fairy1\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml( &element, "counterpart_win\\counterpart_back\\pic\\fairy0\\icon", "ClassName", "ItemButton");
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}
	
	pStResult = (GUIStaticEx*)m_pWnd->GetChild(_T("counterpart_back\\pic\\resultback\\result"));
	pStProgressing = (GUIProgress*)m_pWnd->GetChild(_T("counterpart_back\\pic\\pro"));
	pBnClose = (GUIButton*)m_pWnd->GetChild(_T("counterpart_back\\closebutton"));
	
	m_GUISprite[MainSprite].pBnConform  = (GUIButton*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy0\\surebutt"));
	m_GUISprite[MainSprite].pItbItemPic  = (ItemButton*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy0\\icon"));
	m_GUISprite[MainSprite].pStIntelligence  = (GUIStaticEx*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy0\\word4\\num"));
	m_GUISprite[MainSprite].pStItemName  = (GUIStatic*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy0\\name"));
	m_GUISprite[MainSprite].pStLeftTimes  = (GUIStaticEx*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy0\\word2\\num"));
	m_GUISprite[MainSprite].pStQuality  = (GUIStaticEx*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy0\\word3\\color"));

	m_GUISprite[AssistantSprite].pBnConform  = (GUIButton*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy1\\surebutt"));
	m_GUISprite[AssistantSprite].pItbItemPic  = (ItemButton*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy1\\icon"));
	m_GUISprite[AssistantSprite].pStIntelligence  = (GUIStaticEx*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy1\\word4\\num"));
	m_GUISprite[AssistantSprite].pStItemName = (GUIStatic*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy1\\name"));
	m_GUISprite[AssistantSprite].pStLeftTimes  = (GUIStaticEx*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy1\\word2\\num"));
	m_GUISprite[AssistantSprite].pStQuality  = (GUIStaticEx*)m_pWnd->GetChild(_T("counterpart_back\\pic\\fairy1\\word3\\color"));

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&SpritePartnershipFrame::EventHandler));

	m_GUISprite[MainSprite].pBnConform->SetText(g_StrTable[_T("SpriteBorn_ConformPart")]);
	m_GUISprite[AssistantSprite].pBnConform->SetText(g_StrTable[_T("SpriteBorn_AssNotConform")]);
	m_GUISprite[AssistantSprite].pBnConform->SetEnable(false);

	m_GUISprite[MainSprite].pItbItemPic->RefreshItem();
	m_GUISprite[AssistantSprite].pItbItemPic->RefreshItem();

	return TRUE;
}
BOOL SpritePartnershipFrame::EscCancel()
{
	if(m_eState == EBS_WaitProssing)	return true;
	
	tagNC_FairyMatingCancelAll cmd;
	m_pSession->Send(&cmd);
	LockItem(false);
	m_pFrameMgr->AddToDestroyList(this);
	return true;
}

VOID SpritePartnershipFrame::Update()
{
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if (m_eState == EBS_WaitSpriteConform)
	{
		if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
		{
			m_escapeTime = 0;
			LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
			if( !P_VALID(pLocalPlayer) )
			{
				Destroy();
				return;
			}
			if (RoleMgr::Inst()->IsOutValidDist( m_dwNPCID ) )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_DistanceTooFar")]);
				tagNC_FairyMatingCancelAll cmd;
				m_pSession->Send(&cmd);
				m_pFrameMgr->AddToDestroyList(this);
			}
		}
	}
	else if (m_eState == EBS_WaitProssing)
	{
		pBnClose->SetEnable(false);
		m_GUISprite[MainSprite].pBnConform->SetEnable(false);
		m_GUISprite[MainSprite].pBnConform->SetRefresh(true);
		
		float deltaTime =  Cool3D::Kernel::Inst()->GetDeltaTime();
		float fcur = pStProgressing->GetCurrentValue();
		float fMax = pStProgressing->GetMaxValue();
		if(fcur < fMax)
		{
			fcur += deltaTime;
			pStProgressing->SetValue(fcur, true);
			pStProgressing->SetRefresh();
		}
		if(fcur >= fMax)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("SpriteParterShip_Success")]);
			m_pFrameMgr->AddToDestroyList(this);
		}
	}
	
}

BOOL SpritePartnershipFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if(pBnClose == pWnd)
			{
				tagNC_FairyMatingCancelAll cmd;
				m_pSession->Send(&cmd);
				LockItem(false);
				m_pFrameMgr->AddToDestroyList(this);
			}
			else if(m_GUISprite[MainSprite].pBnConform == pWnd)
			{
				ConformPart();
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if (m_GUISprite[MainSprite].pItbItemPic == pWnd)
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_GUISprite[MainSprite].strSpriteInfo.dwTypeID,m_GUISprite[MainSprite].strSpriteInfo.nQuality).c_str());
			} 
			else	if(m_GUISprite[AssistantSprite].pItbItemPic == pWnd)
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_GUISprite[AssistantSprite].strSpriteInfo.dwTypeID,m_GUISprite[AssistantSprite].strSpriteInfo.nQuality).c_str());
			}
		}
		break;
	case EGUISCB_Drop:
	case EGUISCB_DropCheck:
		{
			if( EDT_Item != (EDragType)pEvent->dwParam1 )
				break;
			if (m_GUISprite[MainSprite].pItbItemPic == pWnd)
			{
				INT nPos = (INT16)LOWORD(pEvent->dwParam2);
				Item *pItem = (Item*)ItemMgr::Inst()->GetPocketItem(nPos);
				if(!P_VALID(pItem))
					break;
				const tagItemProto	*pItemProto = ItemProtoData::Inst()->FindItemProto(pItem->GetItemTypeID());
				if(P_VALID(pItemProto) && MIsEquipment(pItemProto->dwTypeID) &&  pItemProto->eType == EIT_FaBao )
				{
					if(P_VALID(m_pItem))		LockItem(false);
					m_pItem = pItem;
					LockItem(true);
					const tagFabaoProto *pTrumpProto = (tagFabaoProto*)pItemProto;
					const tagFabaoSpec	*pTrumpSpe = ((Equipment*)pItem)->GetFaBaoSpe();
					const tagEquipSpec *pEquipSpec = ((Equipment*)pItem)->GetEquip();
					PutInMainSpriteCache(pItemProto->dwTypeID,pEquipSpec->byQuality,pTrumpSpe->n16NativeIntelligence,pTrumpSpe->nBirthTimes);
				}
			}
		}
		break;
	default:
		break;
	}
	return TRUE;
}

void SpritePartnershipFrame::LockItem( bool bOperable )
{
	if( P_VALID(m_pItem) )
		m_pItem->SetOperable(!bOperable);
}

VOID SpritePartnershipFrame::ConformPart()
{
	if ( !m_GUISprite[MainSprite].haveInfo() || !m_GUISprite[AssistantSprite].haveInfo() )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_All_Conform")]);
		return;
	}
	if (P_VALID(m_pItem))
	{
		if (!m_bCancelConform)
		{
			tagNC_FairyMating cmd;
			cmd.bAgreement = true;
			m_pSession->Send(&cmd);
			m_GUISprite[MainSprite].pBnConform->SetText(g_StrTable[_T("SpriteBorn_CancelConform")]);
			m_bCancelConform = true;
		}
		else
		{	
			tagNC_FairyMating cmd;
			cmd.bAgreement = false;
			m_pSession->Send(&cmd);
			m_GUISprite[MainSprite].pBnConform->SetText(g_StrTable[_T("SpriteBorn_ConformPart")]);
			m_bCancelConform = false;
		}
	}
}

VOID SpritePartnershipFrame::CheckResult()
{
	if(m_GUISprite[MainSprite].strSpriteInfo.dwTypeID == 0 || m_GUISprite[AssistantSprite].strSpriteInfo.dwTypeID == 0)
	{
		pStResult->SetText(_T(""));
		return;
	}
	if (	m_GUISprite[MainSprite].strSpriteInfo.nIntelligence <70 || m_GUISprite[AssistantSprite].strSpriteInfo.nIntelligence < 70
		||	m_GUISprite[MainSprite].strSpriteInfo.nQuality <4 || m_GUISprite[AssistantSprite].strSpriteInfo.nQuality < 4
		||	m_GUISprite[MainSprite].strSpriteInfo.nLeftTimes == 0 || m_GUISprite[AssistantSprite].strSpriteInfo.nLeftTimes == 0)
	{
		pStResult->SetText(g_StrTable[_T("SpriteBorn_PartResult0")]);
		return;
	}

	const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(m_GUISprite[MainSprite].strSpriteInfo.dwTypeID);
	const tagItemProto	*pItem2 = ItemProtoData::Inst()->FindItemProto(m_GUISprite[AssistantSprite].strSpriteInfo.dwTypeID);
	if (P_VALID(pItem) && P_VALID(pItem2))
	{
		int mianQ =m_GUISprite[MainSprite].strSpriteInfo.nQuality;
		ChangeQ(mianQ);
		int assQ =m_GUISprite[AssistantSprite].strSpriteInfo.nQuality;
		ChangeQ(assQ);
		int nResult = (m_GUISprite[MainSprite].strSpriteInfo.nIntelligence + pItem->byLevel /2 + mianQ)*8 +
							(m_GUISprite[AssistantSprite].strSpriteInfo.nIntelligence + pItem2->byLevel /2 + assQ)*2;

		if (nResult < 1199)
		{
			pStResult->SetText(g_StrTable[_T("SpriteBorn_PartResult1")]);
		} 
		else if(nResult < 1399)
		{
			pStResult->SetText(g_StrTable[_T("SpriteBorn_PartResult2")]);
		}
		else if(nResult < 1499)
		{
			pStResult->SetText(g_StrTable[_T("SpriteBorn_PartResult3")]);
		}
		else if(nResult < 1599)
		{
			pStResult->SetText(g_StrTable[_T("SpriteBorn_PartResult4")]);
		}
		else if(nResult < 1699)
		{
			pStResult->SetText(g_StrTable[_T("SpriteBorn_PartResult5")]);
		}
		else if(nResult < 1799)
		{
			pStResult->SetText(g_StrTable[_T("SpriteBorn_PartResult6")]);
		}
		else if(nResult < 1899)
		{
			pStResult->SetText(g_StrTable[_T("SpriteBorn_PartResult7")]);
		}
		else
		{
			pStResult->SetText(g_StrTable[_T("SpriteBorn_PartResult8")]);
		}
	}
	
}

VOID SpritePartnershipFrame::PutSprite(int eType,DWORD dwID,int nQlty,int nIntell,int nleftTime)
{
	if(eType >= SpriteNum)	return;

	m_GUISprite[eType].strSpriteInfo.dwTypeID = dwID;
	m_GUISprite[eType].strSpriteInfo.nQuality = nQlty;
	m_GUISprite[eType].strSpriteInfo.nIntelligence = nIntell;
	m_GUISprite[eType].strSpriteInfo.nLeftTimes = nleftTime;

	const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(dwID);
	if(P_VALID(pItem) && MIsEquipment(dwID) &&  pItem->eType == EIT_FaBao )
	{
		TCHAR szBuffer[X_SHORT_NAME] = {0};
		const tagFabaoProto *pTrumpProto = (tagFabaoProto*)pItem;

		m_GUISprite[eType].pItbItemPic->RefreshItem( dwID,0 ,nQlty);

		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nQlty);
		m_GUISprite[eType].pStQuality->SetText(szBuffer);
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), nIntell);
		m_GUISprite[eType].pStIntelligence->SetText(szBuffer);
		m_GUISprite[eType].pStItemName->SetText(pItem->pDisplayInfo->szName);
		_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"),nleftTime);
		m_GUISprite[eType].pStLeftTimes->SetText(szBuffer);
	}

	// 如果放置的是主妖精则通知一下对方
// 	if(eType == MainSprite)
// 	{
// 		SyncSprite();
// 	}

	CheckResult();
}

DWORD SpritePartnershipFrame::OnEventMove2SpritePart(tagItemPosChangeExEvent* pGameEvent)
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;

	Item* pItem = ItemMgr::Inst()->GetPocketItem(pGameEvent->n16IndexSrc);
	if(!P_VALID(pItem))		return 0;
	
	const tagItemProto	*pItemProto = ItemProtoData::Inst()->FindItemProto(pItem->GetItemTypeID());
	if(P_VALID(pItemProto) && MIsEquipment(pItemProto->dwTypeID) &&  pItemProto->eType == EIT_FaBao )
	{
		if(P_VALID(m_pItem))		LockItem(false);
		m_pItem = pItem;
		LockItem(true);
		const tagFabaoProto *pTrumpProto = (tagFabaoProto*)pItemProto;
		const tagFabaoSpec	*pTrumpSpe = ((Equipment*)pItem)->GetFaBaoSpe();
		const tagEquipSpec *pEquipSpec = ((Equipment*)pItem)->GetEquip();
		PutInMainSpriteCache(pItemProto->dwTypeID,pEquipSpec->byQuality,pTrumpSpe->n16NativeIntelligence,pTrumpSpe->nBirthTimes);
	}

	return 0;
}

VOID SpritePartnershipFrame::SyncSprite()
{
	if (P_VALID(m_pItem))
	{
		tagNC_PutFairyInto cmd;
		cmd.n64FairyID = m_pItem->GetItemId();
		m_pSession->Send(&cmd);
	}
}

DWORD SpritePartnershipFrame::OnNS_PutFairyInto(tagNS_PutFairyInto* pNetCmd,DWORD)
{
	if (pNetCmd->dwErrorCode != E_Success)
	{
		// 如果已经有妖精在界面中则不刷新界面
		if (m_GUISprite[MainSprite].strSpriteInfo.dwTypeID != 0)
		{
			return 0;
		}
		LockItem(false);
		m_pItem = NULL;
		m_GUISprite[MainSprite].clear();

		switch (pNetCmd->dwErrorCode)
		{
		case E_FabaoMating_RoleNotOnline:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_RoleNotOnline")]);
			break;
		case E_FabaoMating_QltyLow:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_QltyLow")]);
			break;
		case E_FabaoMating_QualLow:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_QualLow")]);
			break;
		case E_FabaoMating_CannotMating:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_CannotMating")]);
			break;
		case E_FabaoMating_BirthTimesLow:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_BirthTimesLow")]);
			break;
		case E_FabaoMating_LevelLow:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_LevelLow")]);
			break;
		case E_FabaoMating_OtherBagFull:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_OtherBagFull")]);
			break;
		case E_FabaoMating_NotInGiveBirth:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_NotInGiveBirth")]);
			break;
		case E_FabaoMating_CannotPutIn_ErrStatus:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_CannotPutIn_ErrStatus")]);
			break;
		}
	}
	else
	{
		if (m_MainSpriteCache.dwTypeID != 0)
		{
			PutSprite( MainSprite, m_MainSpriteCache.dwTypeID, m_MainSpriteCache.nQuality, m_MainSpriteCache.nIntelligence, m_MainSpriteCache.nLeftTimes);
		}
	}
	return 0;
}

DWORD SpritePartnershipFrame::OnNS_TransmitPutFairyInto(tagNS_TransmitPutFairyInto* pNetCmd,DWORD)
{
	PutSprite(AssistantSprite,pNetCmd->dwFairyTypeID,pNetCmd->byQlty,pNetCmd->nQualification,pNetCmd->nBirthTimes);
	return 0;
}

DWORD SpritePartnershipFrame::OnNS_TransmitFairyMating(tagNS_TransmitFairyMating* pNetCmd,DWORD)
{
	if (pNetCmd->dwRoleID == m_dwNPCID)
	{
		if (pNetCmd->bAgreement == TRUE)
		{
			m_GUISprite[AssistantSprite].pBnConform->SetText(g_StrTable[_T("SpriteBorn_AssConform")]);
		} 
		else
		{
			m_GUISprite[AssistantSprite].pBnConform->SetText(g_StrTable[_T("SpriteBorn_AssNotConform")]);
		}
	}
	return 0;
}

DWORD SpritePartnershipFrame::OnNS_FairyMating(tagNS_FairyMating* pNetCmd,DWORD)
{
	pStProgressing->SetSpeed(100);
	pStProgressing->SetValue(0, true);
	pStProgressing->SetMaxValue(3.0f);
	pStProgressing->SetRefresh();

	m_eState =EBS_WaitProssing;
	return 0;
}

DWORD SpritePartnershipFrame::OnNS_FairyMatingCancelAll(tagNS_FairyMatingCancelAll* pNetCmd,DWORD)
{
	LockItem(false);
	m_pFrameMgr->AddToDestroyList(this);
	return 0;
}
VOID SpritePartnershipFrame::ChangeQ(int & q)
{
	switch (q)
	{
	case EIQ_Orange:
		q = 15;
		break;
	case EIQ_Purple:
		q = 30;
		break;
	case EIQ_Pink:
		q = 50;
		break;
	}
}

VOID SpritePartnershipFrame::ClearUI()
{
	m_pItem = NULL;
	m_GUISprite[MainSprite].clear();
	m_GUISprite[AssistantSprite].clear();
}

VOID SpritePartnershipFrame::PutInMainSpriteCache(DWORD dwID,int nQlty,int nIntell,int nleftTime)
{
	m_MainSpriteCache.clear();
	m_MainSpriteCache.dwTypeID = dwID;
	m_MainSpriteCache.nQuality = nQlty;
	m_MainSpriteCache.nIntelligence = nIntell;
	m_MainSpriteCache.nLeftTimes = nleftTime;

	SyncSprite();
}