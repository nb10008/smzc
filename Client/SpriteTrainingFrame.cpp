#include "StdAfx.h"
#include "SpriteTrainingFrame.h"
#include "FamilyProtoData.h"
#include "ItemProtoData.h"
#include "CombatSysUtil.h"
#include "ItemButton.h"
#include "..\WorldDefine\msg_family.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "FamilyMgr.h"
#include "ItemMgr.h"
#include "Item.h"
#include "ItemFrame.h"
#include "ItemEvent.h"


SpriteTrainingFrame::SpriteTrainingFrame(void):m_trunk(this)
,m_eState(ESSS_Base)
,m_dwCurrTrainID(0)
,m_bStart(false)
,m_n64TrainItemID(0)
,m_pTItem(NULL)
{
}

SpriteTrainingFrame::~SpriteTrainingFrame(void)
{
}

BOOL SpriteTrainingFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;
	m_pCmdMgr->Register( "NS_FamilySpriteTrain",		(NETMSGPROC)m_trunk.sfp2(&SpriteTrainingFrame::OnNS_FamilySpriteTrain),		_T("NS_FamilySpriteTrain"));
	m_pCmdMgr->Register( "NS_GetFamilySpriteTrain",		(NETMSGPROC)m_trunk.sfp2(&SpriteTrainingFrame::OnNS_GetFamilySpriteTrain),		_T("NS_GetFamilySpriteTrain"));

	m_pMgr->RegisterEventHandle(_T("MsgBox_SpriteTrain_Check"),			(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteTrainingFrame::OnMsgBox_SpriteTrain_Check));
	m_pMgr->RegisterEventHandle(_T("Close_Family_AllUi"),			(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteTrainingFrame::OnClose_Family_AllUi));
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2SpriteTrain"),		(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteTrainingFrame::OnEventPutItem));

	return TRUE;
}

BOOL SpriteTrainingFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	m_pCmdMgr->UnRegister( "NS_FamilySpriteTrain",							(NETMSGPROC)m_trunk.sfp2(&SpriteTrainingFrame::OnNS_FamilySpriteTrain) );
	m_pCmdMgr->UnRegister( "NS_GetFamilySpriteTrain",							(NETMSGPROC)m_trunk.sfp2(&SpriteTrainingFrame::OnNS_GetFamilySpriteTrain) );

	m_pMgr->UnRegisterEventHandler(_T("MsgBox_SpriteTrain_Check"),				(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteTrainingFrame::OnMsgBox_SpriteTrain_Check));
	m_pMgr->UnRegisterEventHandler(_T("Close_Family_AllUi"),				(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteTrainingFrame::OnClose_Family_AllUi));
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2SpriteTrain"),	(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteTrainingFrame::OnEventPutItem));

	if(P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}

	if(P_VALID( m_pWndTrain ) )
	{
		m_pGUI->DestroyWnd(m_pWndTrain);
		m_pWndTrain = NULL;		
	}

	if(P_VALID(GetObj("SpriteTrain_Check")))
		KillObj("SpriteTrain_Check");
	
	return bRet;
}

BOOL SpriteTrainingFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\fairy_refinelist.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pBnSubmit = (GUIButton*)m_pWnd->GetChild(_T("refineback\\refer"));
	m_pBnClose= (GUIButton*)m_pWnd->GetChild(_T("refineback\\close"));
	m_pBnCloseX = (GUIButton*)m_pWnd->GetChild(_T("refineback\\closebutt"));

	m_pStTodayNum = (GUIStatic*)m_pWnd->GetChild(_T("refineback\\word0\\num"));
	m_pPbHideDone = (GUIPushButton*)m_pWnd->GetChild(_T("refineback\\optbutt"));
	m_pLbShow = (GUIListBox*)m_pWnd->GetChild(_T("refineback\\listback\\list"));

	m_pPbBase = (GUIPushButton*)m_pWnd->GetChild(_T("refineback\\choose\\choose0"));
	m_pPbAttack = (GUIPushButton*)m_pWnd->GetChild(_T("refineback\\choose\\choose1"));
	m_pPbState = (GUIPushButton*)m_pWnd->GetChild(_T("refineback\\choose\\choose2"));

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_trunk.sfp1(&SpriteTrainingFrame::EventHandler));

	//  [7/13/2010 zhangsu]
	strPath = g_strLocalPath + _T("\\ui\\fairy_refine.xml");
	if( !m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml(&ele, "family_refine_sure\\sureback\\itemback0\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "family_refine_sure\\sureback\\itemback1\\icon", "ClassName", "ItemButton");

	m_pWndTrain = m_pGUI->CreateWnd( _T("\\desktop"), &ele );

	m_pBnTrain =  (GUIButton*)m_pWndTrain->GetChild(_T("sureback\\surebutt"));
	m_pBnTrainClose=  (GUIButton*)m_pWndTrain->GetChild(_T("sureback\\closebutt"));
	m_pIbNeedItem=  (ItemButton*)m_pWndTrain->GetChild(_T("sureback\\itemback0\\icon"));
	m_pIbRewordItem=  (ItemButton*)m_pWndTrain->GetChild(_T("sureback\\itemback1\\icon"));

	m_pStName = (GUIPatch*)m_pWndTrain->GetChild(_T("sureback\\contentback"));
	m_pStAtt = (GUIStatic*)m_pWndTrain->GetChild(_T("sureback\\contentback\\num"));
	m_pStAtt2 = (GUIPatch*)m_pWndTrain->GetChild(_T("sureback\\resultback"));
	m_pStItem = (GUIStatic*)m_pWndTrain->GetChild(_T("sureback\\name"));
	m_pStSpriteExp = (GUIStatic*)m_pWndTrain->GetChild(_T("sureback\\expback0\\num"));
	m_pStIExp = (GUIStatic*)m_pWndTrain->GetChild(_T("sureback\\expback\\num"));
	m_pPgProgress = (GUIProgress*)m_pWndTrain->GetChild(_T("sureback\\timepic\\time"));
	m_pStIResultNum = (GUIStatic*)m_pWndTrain->GetChild(_T("sureback\\resultback\\num"));

	m_pGUI->RegisterEventHandler( m_pWndTrain->GetFullName().c_str(), m_trunk.sfp1(&SpriteTrainingFrame::EventHandler) );

	m_pWnd->SetInvisible(TRUE);
	m_pWndTrain->SetInvisible(TRUE);

	m_pLbShow->SetColNum(7,20);
	m_pLbShow->SetColWidth(0,100);
	m_pLbShow->SetColWidth(1,80);
	m_pLbShow->SetColWidth(2,150);
	m_pLbShow->SetColWidth(3,70);
	m_pLbShow->SetColWidth(4,80);
	m_pLbShow->SetColWidth(5,150);

	m_pPbBase->SetState(EGUIBS_PushDown);

	return TRUE;
}

BOOL SpriteTrainingFrame::EscCancel()
{
	if (!m_pWndTrain->IsInvisible())
	{
		m_pWndTrain->SetInvisible(TRUE);
		SetStrategy(EIST_ItemPocket);
		return TRUE;
	}

	if (!m_pWnd->IsInvisible())
	{
		m_pWnd->SetInvisible(TRUE);
		return TRUE;
	}

	return FALSE;
}

VOID SpriteTrainingFrame::Update()
{
	if (m_bStart)
	{
		float deltaTime =  Cool3D::Kernel::Inst()->GetDeltaTime();
		float fcur = m_pPgProgress->GetCurrentValue();
		float fMax = m_pPgProgress->GetMaxValue();
		if(fcur < fMax)
		{
			fcur += deltaTime;
			m_pPgProgress->SetValue(fcur, true);
			m_pPgProgress->SetRefresh();
		}
		if(fcur >= fMax)
		{
			tagNC_FamilySpriteTrain cmd;
			cmd.dwTrainID = m_dwCurrTrainID;
			cmd.n64ID =	m_n64TrainItemID;
			m_pSession->Send(&cmd);
			
			m_pWndTrain->SetInvisible(TRUE);
			m_bStart = false;

			m_n64TrainItemID = 0;
			m_pTItem  =NULL;
			m_pIbNeedItem->RefreshItem();
			SetStrategy(EIST_ItemPocket);
		}
	}
}

DWORD SpriteTrainingFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBnClose || pWnd == m_pBnCloseX)
			{
				ShowFrame(FALSE);
			}
			else if (pWnd == m_pBnSubmit)
			{
				if(m_vecCurrShowList.size() > m_pLbShow->GetCurSelectedRow())
					SubmitTrain(m_vecCurrShowList[m_pLbShow->GetCurSelectedRow()]);
			}
			else if (pWnd == m_pBnTrainClose)
			{
				m_pWndTrain->SetInvisible(TRUE);
				m_pWndTrain->FlipToTop();
				SetStrategy(EIST_ItemPocket);
				m_dwCurrTrainID = GT_INVALID;
			}
			else if (pWnd == m_pBnTrain)
			{
				const tagSpriteTrainClientProto* pData = FamilyProtoData::Inst()->FindSpriteTrainProto(m_dwCurrTrainID);
				if(!P_VALID(pData))	break;
				const tagFamilySpriteProto* pData2 = FamilyProtoData::Inst()->FindFamilySpriteProto(FamilyMgr::Inst()->GetSpriteLevel());
				if(!P_VALID(pData2))	break ;

				LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
				if(!P_VALID(plp))	break;

				if (plp->GetRoleLevel() < pData->nLevelLimit)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpriteTrain_LevelLowEx")]);
					break;
				}

				if(pData2->dwAbilityUpperLimit[pData->dwAbilityType] == FamilyMgr::Inst()->GetSpriteAtt(pData->dwAbilityType))
				{
					if(P_VALID(GetObj("SpriteTrain_Check")))
						KillObj("SpriteTrain_Check");

					CreateObj("SpriteTrain_Check", "MsgBox");
					TObjRef<MsgBox>("SpriteTrain_Check")->Init(_T(""), g_StrTable[_T("SpriteTrain_Check")], _T("MsgBox_SpriteTrain_Check"));
					break;
				}

				CommitTrain();
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if (pWnd == m_pPbBase)
			{
				m_eState = ESSS_Base;
				RefreshUI();
			}
			else if (pWnd == m_pPbAttack)
			{
				m_eState = ESSS_Attack;
				RefreshUI();
			}
			else if (pWnd == m_pPbState)
			{
				m_eState = ESSS_State;
				RefreshUI();
			}
			else if (pWnd == m_pPbHideDone)
			{
				RefreshUI();
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			const tagSpriteTrainClientProto* pData = FamilyProtoData::Inst()->FindSpriteTrainProto(m_dwCurrTrainID);
			if(!P_VALID(pData))	break;
			
			BYTE bQuality = 0;
			
			if (pWnd == m_pIbNeedItem)
			{
				if(P_VALID(m_pTItem))
				{
					if (MIsEquipment(m_pTItem->GetItemTypeID()))
					{
						bQuality = pData->dwItemQuality;
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pTItem->GetItemTypeID(),bQuality).c_str());
					}
					else
					{
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pTItem->GetItemTypeID(),m_pTItem->GetItemQuality()).c_str());
					}
				}
				//ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pData->dwItemID,bQuality).c_str());
			}
			else if (pWnd == m_pIbRewordItem)
			{
				if (MIsEquipment(pData->dwItemReword))
				{
					bQuality = pData->dwItemRewordQuality;
				}
				else
				{
					const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pData->dwItemReword);
					if (P_VALID(pProto))
					{
						bQuality = pProto->byQuality;
					}
				}

				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pData->dwItemReword,bQuality).c_str());
			}
		}
		break;
	case EGUISCB_Drop:
	case EGUISCB_DropCheck:
		{
			if( EDT_Item != (EDragType)pEvent->dwParam1 )
				break;
			if (m_pIbNeedItem == pWnd)
			{
				Item *pItem = (Item*)ItemMgr::Inst()->GetPocketItem((INT16)LOWORD(pEvent->dwParam2));
				if(!P_VALID(pItem))	break;
				m_pTItem = pItem;
				m_n64TrainItemID = pItem->GetItemId();

				const tagSpriteTrainClientProto* pData = FamilyProtoData::Inst()->FindSpriteTrainProto(m_dwCurrTrainID);
				if(!P_VALID(pData))	break;

				if (MIsEquipment(m_pTItem->GetItemTypeID()))
				{
					m_pIbNeedItem->RefreshItem(pData->dwItemID,pData->dwItemNum,pData->dwItemQuality);
				}
				else
				{
					const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(m_pTItem->GetItemTypeID());
					if (P_VALID(pProto))
					{
						m_pIbNeedItem->RefreshItem(m_pTItem->GetItemTypeID(),m_pTItem->GetItemQuantity(),m_pTItem->GetItemQuality());
					}
					else
						m_pIbNeedItem->RefreshItem();
				}

			}
		}
		break;

	}
	return 0;
}

VOID SpriteTrainingFrame::ShowFrame(BOOL bShow)
{
	m_pWnd->SetInvisible(!bShow);

	if(bShow)
	{
		tagNC_GetFamilySpriteTrain cmd;
		m_pSession->Send(&cmd);

		m_pWnd->FlipToTop();
	}
}

VOID SpriteTrainingFrame::RefreshUI()
{
	m_pLbShow->Clear();
	bool hide = m_pPbHideDone->GetState() == EGUIBS_PushDown ?  true : false;
	DWORD curRow = 0;
	DWORD color  = 0xFFFFFFFF;
	TCHAR buffer[X_LONG_NAME];
	if (m_vecFamilySpriteAtt.empty())
		return;

	int nAcitveNum = 0;
	m_vecCurrShowList.clear();
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(plp))	return ;
	vector<tagFamilySpriteRequirementSave>::iterator iter = m_vecFamilySpriteAtt.begin();
	for (; iter != m_vecFamilySpriteAtt.end(); ++iter)
	{
		if(iter->bComplete)
			nAcitveNum++;

		tagSpriteTrainClientProto* pData = FamilyProtoData::Inst()->FindSpriteTrainProto(iter->dwRequirementID);
		if(!P_VALID(pData))	continue;
	
		if ( hide && iter->bComplete )
			continue;

		if(pData->nPageType != m_eState)
			continue;

		m_vecCurrShowList.push_back(pData->nID);

		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("FamilySpriteAbility_%d"), pData->dwAbilityType);
		m_pLbShow->SetText(curRow, 0,g_StrTable[buffer]	, color);
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d-%d"), pData->dwPowerLowLimit ,pData->dwPowerUpLimit);
		m_pLbShow->SetText(curRow, 1,buffer, color);
		const tagItemDisplayInfo* pProto = ItemProtoData::Inst()->FindItemDisplay(pData->dwItemID);
		if(P_VALID(pProto))
		{
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%s x %d"), pProto->szName , pData->dwItemNum);
			m_pLbShow->SetText(curRow, 2,buffer, color);
		}
		if(0 == iter->nRet)
		{
			m_pLbShow->SetText(curRow, 3,g_StrTable[_T("FMSpriteNoProssing")], color);
		}
		else
		{
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), iter->nRet);
			m_pLbShow->SetText(curRow, 3,buffer, color);
		}
		
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), pData->dwExpReword);
		m_pLbShow->SetText(curRow, 4,buffer, color);

		TCHAR		szName[X_SHORT_NAME];
		_stprintf( szName, _T("%s"), plp->GetRoleName().c_str() );
		if(wcscmp( iter->tszRoleName, szName ) == 0)
			color = 0xFFFF0000;

		if(wcscmp(iter->tszRoleName, _T("")) == 0)
		{
			if(plp->GetRoleLevel() < pData->nLevelLimit)
				m_pLbShow->SetText(curRow, 5,g_StrTable[_T("SpriteTrain_LevelLow")], color);
			else
			{
				color = 0xFF888888;
				m_pLbShow->SetText(curRow, 5,g_StrTable[_T("SpriteTrain_NullPeopleFinish")], color);
				color = 0xFFFFFFFF;
			}
				
		}
		else
			m_pLbShow->SetText(curRow, 5,iter->tszRoleName, color);
		color  = 0xFFFFFFFF;

		curRow++;
	}

	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d/%d"), nAcitveNum , m_vecFamilySpriteAtt.size());
	m_pStTodayNum->SetText(buffer);
	m_pStTodayNum->SetRefresh(TRUE);
}

VOID SpriteTrainingFrame::SubmitTrain(DWORD dwTrainID)
{
	m_n64TrainItemID = 0;
	m_pTItem = 0;

	const tagSpriteTrainClientProto* pData = FamilyProtoData::Inst()->FindSpriteTrainProto(dwTrainID);
	if(!P_VALID(pData))	return;

	if (pData->dwRealResult != GT_INVALID )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpriteTrain_Done")]);
		return;
	}

	TCHAR buffer[X_LONG_NAME];
// 	if (MIsEquipment(pData->dwItemID))
// 	{
// 		m_pIbNeedItem->RefreshItem(pData->dwItemID,pData->dwItemNum,pData->dwItemQuality);
// 	}
// 	else
// 	{
// 		const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pData->dwItemID);
// 		if (P_VALID(pProto))
// 		{
// 			m_pIbNeedItem->RefreshItem(pData->dwItemID,pData->dwItemNum,pProto->byQuality);
// 		}
// 		else
// 			m_pIbNeedItem->RefreshItem();
// 	}

	m_pIbNeedItem->RefreshItem();

	if (MIsEquipment(pData->dwItemReword))
	{
		m_pIbRewordItem->RefreshItem(pData->dwItemReword,pData->dwItemRewordNum,pData->dwItemRewordQuality);
	}
	else
	{
		const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(pData->dwItemReword);
		if (P_VALID(pProto))
		{
			m_pIbRewordItem->RefreshItem(pData->dwItemReword,pData->dwItemRewordNum,pProto->byQuality);
		}
		else
			m_pIbRewordItem->RefreshItem();
	}

	const tagItemDisplayInfo* pProto = ItemProtoData::Inst()->FindItemDisplay(pData->dwItemID);
	if(P_VALID(pProto))
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%s x %d"), pProto->szName , pData->dwItemNum);
		m_pStItem->SetText(buffer);
	}

	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("FamilySpriteAbility_%d"), pData->dwAbilityType);
	m_pStName->SetText(g_StrTable[buffer]);

	m_pStAtt2->SetText(g_StrTable[buffer]);

	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d-%d"), pData->dwPowerLowLimit , pData->dwPowerUpLimit);
	m_pStAtt->SetText(buffer);

	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), pData->dwSpriteExpReword);
	m_pStSpriteExp->SetText(buffer);

	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), pData->dwExpReword);
	m_pStIExp->SetText(buffer);

	m_pStIResultNum->SetText(_T(""));

	m_dwCurrTrainID = dwTrainID;

	m_pWndTrain->SetInvisible(FALSE);
	m_pWndTrain->FlipToTop();

	SetStrategy(EIST_FamilySpriteTrain);
}

VOID SpriteTrainingFrame::CommitTrain()
{
	m_bStart = true;
}

DWORD SpriteTrainingFrame::OnNS_FamilySpriteTrain(tagNS_FamilySpriteTrain* pNetCmd,DWORD)
{
	if ( 0 ==  pNetCmd->dwErrorCode)
	{
		LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
		if(!P_VALID(plp))	return 0;

		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpriteTrain_OK")]);
		TCHAR buffer[X_LONG_NAME];
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), pNetCmd->nTrainRet);
		m_pStIResultNum->SetText(buffer);

		vector<tagFamilySpriteRequirementSave>::iterator iter = m_vecFamilySpriteAtt.begin();
		for (; iter != m_vecFamilySpriteAtt.end(); ++iter)
		{
			if (iter->dwRequirementID == pNetCmd->dwTrainID)
			{
				iter->nRet = pNetCmd->nTrainRet;
				iter->bComplete = true;

				TCHAR		szName[X_SHORT_NAME];
				_stprintf( szName, _T("%s"), plp->GetRoleName().c_str() );
				memcpy(iter->tszRoleName, szName, X_SHORT_NAME*sizeof(TCHAR));
				RefreshUI();
			}	
		}

		tagNC_GetFamilySpriteInfo cmd3;
		m_pSession->Send(&cmd3);
	}
	else if ( EFSE_USE_ITEM_MAX ==  pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpriteTrain_num")]);
	}
	else
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpriteTrain_Fail")]);
	}

	m_pPgProgress->SetValue(0, true);
	m_pPgProgress->SetRefresh();
	return 0;
}

DWORD SpriteTrainingFrame::OnNS_GetFamilySpriteTrain(tagNS_GetFamilySpriteTrain* pNetCmd,DWORD)
{
	if (0 == pNetCmd->dwErrorCode)
	{
// 		tagNC_GetFamilySpriteInfo cmd3;
// 		m_pSession->Send(&cmd3);

		m_vecFamilySpriteAtt.clear();
		for (int i=0; i< pNetCmd->nNum ; ++i)
		{
			tagFamilySpriteRequirementSave data;
			data = pNetCmd->ftsi[i];
			m_vecFamilySpriteAtt.push_back(data);
		}

		RefreshUI();
	}
	return 0;
}

DWORD SpriteTrainingFrame::OnMsgBox_SpriteTrain_Check(tagMsgBoxEvent* pGameEvent)
{
	if(MBF_OK == pGameEvent->eResult )
	{
		CommitTrain();
	}
	return 0;
}

DWORD SpriteTrainingFrame::OnClose_Family_AllUi(tagGameEvent* pGameEvent)
{

	m_pWndTrain->SetInvisible(TRUE);
	m_pWnd->SetInvisible(TRUE);
	return 0;
}

DWORD SpriteTrainingFrame::OnEventPutItem(tagItemPosChangeExEvent* pGameEvent)
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;

	Item *pItem = (Item*)ItemMgr::Inst()->GetPocketItem((INT16)LOWORD(pGameEvent->n16IndexSrc));
	if(!P_VALID(pItem))	return 0;
	m_pTItem = pItem;
	m_n64TrainItemID = pItem->GetItemId();

	const tagSpriteTrainClientProto* pData = FamilyProtoData::Inst()->FindSpriteTrainProto(m_dwCurrTrainID);
	if(!P_VALID(pData))	return 0;

	if (MIsEquipment(m_pTItem->GetItemTypeID()))
	{
		m_pIbNeedItem->RefreshItem(pData->dwItemID,pData->dwItemNum,pData->dwItemQuality);
	}
	else
	{
		const tagItemProto* pProto = ItemProtoData::Inst()->FindItemProto(m_pTItem->GetItemTypeID());
		if (P_VALID(pProto))
		{
			m_pIbNeedItem->RefreshItem(m_pTItem->GetItemTypeID(),m_pTItem->GetItemQuantity(),m_pTItem->GetItemQuality());
		}
		else
			m_pIbNeedItem->RefreshItem();
	}

	return 0;
}