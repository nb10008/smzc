#include "stdafx.h"
#include "ItemMgr.h"
#include "ItemFrame.h"
#include "ItemEvent.h"
#include "ServerTime.h"
#include "ItemButton.h"
#include "ToolTipFrame.h"
#include "CombatSysUtil.h"
#include "ToolTipCreator.h"
#include "ConsolidateData.h"
#include "GodStrengthenFrame.h"
#include "..\WorldDefine\msg_compose.h"
#include "..\WorldDefine\compose_define.h"
#include "..\WorldDefine\currency_define.h"


const INT GODSTRENGTHEN_EQUIP_MIN_LEVEL = 70;
const INT MAX_GODSTRENGTHEN_LEVEL = 10;

GodStrengthenFrame::GodStrengthenFrame():m_Trunk(this)
{
	m_pWndMain		= NULL;
	m_pWndCaption	= NULL;
	m_pEquip		= NULL;
	m_pBtnExit		= NULL;
	m_pBtnOk		= NULL;
	m_pBtnCancel	= NULL;
	m_pStcGolden	= NULL;
	m_pStcSiller	= NULL;
	m_n64AssisEquipID = GT_INVALID;
	m_n64EquipID = GT_INVALID;
	m_bTime = FALSE;
}

GodStrengthenFrame::~GodStrengthenFrame()
{

}

BOOL GodStrengthenFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pMgr->RegisterEventHandle( _T("ItemEvent_Move2GodStrengthen"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GodStrengthenFrame::OnEventPutItem) );
	m_pMgr->RegisterEventHandle( _T("OnGodStrengthenExpValue"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&GodStrengthenFrame::OnGodStrengthenExpValue) );
	m_pCmdMgr->Register( "NS_GodStrengthen",							(NETMSGPROC)m_Trunk.sfp2(&GodStrengthenFrame::NetEquipStrengthen),		_T("NS_GodStrengthen"));
	return TRUE;
}

BOOL GodStrengthenFrame::Destroy()
{
	GameFrame::Destroy();
	if( P_VALID(m_pWndMain) )
		m_pGUI->AddToDestroyList(m_pWndMain);

	m_pMgr->UnRegisterEventHandler( _T("ItemEvent_Move2GodStrengthen"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&GodStrengthenFrame::OnEventPutItem));
	m_pMgr->UnRegisterEventHandler( _T("OnGodStrengthenExpValue"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&GodStrengthenFrame::OnGodStrengthenExpValue) );
	m_pCmdMgr->UnRegister( "NS_GodStrengthen",							(NETMSGPROC)m_Trunk.sfp2(&GodStrengthenFrame::NetEquipStrengthen) );

	LockItem(m_n64EquipID, false);
	LockItem(m_n64AssisEquipID, false);

	SetStrategy(EIST_ItemPocket);

	return TRUE;
}

BOOL GodStrengthenFrame::EscCancel()
{
	m_pMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL GodStrengthenFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\equip_foundry.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "foundry_win\\foundry_back\\equip_back0\\equip_pic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "foundry_win\\foundry_back\\equip_back\\equip_pic", "ClassName", "ItemButton");

	m_pWndMain = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pWndCaption	= m_pWndMain->GetChild( _T("foundry_back") );

	m_pEquip		= (ItemButton*)m_pWndCaption->GetChild( _T("equip_back0\\equip_pic") );
	m_pAssisEquip	= (ItemButton*)m_pWndCaption->GetChild( _T("equip_back\\equip_pic") );

	m_pStcEquipLevel	= (GUIStatic*)m_pWndCaption->GetChild( _T("back\\grade") );
	m_pStcTime	= (GUIStatic*)m_pWndCaption->GetChild( _T("back\\time") );
	m_pProExp	= (GUIProgress*)m_pWndCaption->GetChild( _T("back\\progpic0\\proqress") );
	m_pProValue	= (GUIStatic*)m_pWndCaption->GetChild( _T("back\\progpic0\\num") );
	m_pStcExp	= (GUIStatic*)m_pWndCaption->GetChild( _T("back\\time0") );
	m_pStcGolden	= (GUIStatic*)m_pWndCaption->GetChild( _T("back\\goldnum") );
	m_pStcGolden->SetText(_T("0"));
	m_pStcSiller	= (GUIStatic*)m_pWndCaption->GetChild( _T("back\\silvernum") );
	m_pStcSiller->SetText(_T("0"));
	m_pBtnOk		= (GUIButton*)m_pWndCaption->GetChild( _T("surebutt") );
	m_pBtnCancel	= (GUIButton*)m_pWndCaption->GetChild( _T("canclebutt") );
	m_pBtnExit		= (GUIButton*)m_pWndCaption->GetChild( _T("closebutt") );
	m_pBtnOk->SetEnable(false);

	m_pEquip->RefreshItem();
	m_pAssisEquip->RefreshItem();

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWndMain->GetFullName().c_str(), m_Trunk.sfp1(&GodStrengthenFrame::EventHandler));

	return TRUE;
}

DWORD GodStrengthenFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnExit || pWnd == m_pBtnCancel )
			{
				m_pMgr->AddToDestroyList(this);
			}
			else if( pWnd == m_pBtnOk )
			{
				tagNC_GodStrengthen m;
				m.dwEquipID = m_n64EquipID;
				m.dwAssitEquipID = m_n64AssisEquipID;
				m_pSession->Send(&m);
			}
		}
		break;
	case EGUISCB_Drop:
		if( pWnd == m_pEquip )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutItem((INT16)LOWORD(pEvent->dwParam2));
		}
		else if( pWnd == m_pAssisEquip )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutItem((INT16)LOWORD(pEvent->dwParam2), false);
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			Item *pItem = NULL;
			if( pWnd == m_pEquip && m_n64EquipID != GT_INVALID )
				pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64EquipID);
			else if( pWnd == m_pAssisEquip && m_n64AssisEquipID != GT_INVALID )
				pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64AssisEquipID);
			if(P_VALID(pItem))
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
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

	return FALSE;
}

DWORD GodStrengthenFrame::OnEventPutItem( tagItemPosChangeExEvent* pGameEvent )
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;
	PutItem( pGameEvent->n16IndexSrc );
	return 0;
}

VOID GodStrengthenFrame::PutItem( INT16 nPos, bool bMainEquip /* = true */ )
{
	Item *pItem = ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(pItem))
		return;

	const tagItemProto* pItemProto = pItem->GetProto();
	TCHAR szTemp[X_LONG_NAME] = {0};
	if( !P_VALID(pItemProto) )
		return;

	if(MIsEquipment(pItemProto->dwTypeID))
	{
		if( pItemProto->eType == EIT_FaBao  || pItemProto->eType == EIT_Fashion )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyAndFashonEquipCanNotGodStreng")]);
			return;
		}
		Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
		if(!P_VALID(pEquip))
			return;
		const tagEquipSpec *pEquipSpec = pEquip->GetEquip();
		if(!P_VALID(pEquipSpec))
			return;
		const tagEquipProto *pEquipProto = (tagEquipProto*)pItemProto;//pEquip->GetData();
		if(!P_VALID(pEquipProto))
			return;

		if(pEquipSpec->byQuality >= EIQ_Purple && bMainEquip)
		{
			if(pEquipSpec->byGodStrengthenLevel == MAX_GODSTRENGTHEN_LEVEL)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodStrengthenLevelIsTop")]);
				return;
			}
			LockItem(m_n64EquipID,false);
			m_n64EquipID = pEquip->GetItemId();
			LockItem(m_n64AssisEquipID,false);
			m_n64AssisEquipID = GT_INVALID;
			LockItem(m_n64EquipID,true);
			UpdateUI();
			return;
		}

		EEquipPos equipPos = EEP_Null;
		if(m_n64EquipID == GT_INVALID)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutGodStrengthenEquipFirst")]);
			return;
		}
		else
		{
			Equipment *pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64EquipID);
			if(P_VALID(pEquip))
				equipPos = pEquip->GetEquipPos();
		}

		if(pEquipSpec->byQuality < EIQ_Green)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyGreenEquipCanAssis")]);
			return;
		}
		else if(pEquipProto->byLevel < GODSTRENGTHEN_EQUIP_MIN_LEVEL)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("AssisEquipLevelIsTooLow")]);
			return;
		}
		else if(pEquipProto->dwTimeLimit != 0 && pEquipProto->dwTimeLimit != GT_INVALID)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TimeLimitEquipIsNotPermit")]);
			return;
		}
		else if(equipPos != pEquipProto->eEquipPos)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipPosIsNotSame")]);
			return;
		}
		else
		{
			LockItem(m_n64AssisEquipID,false);
			m_n64AssisEquipID = pEquip->GetItemId();
			LockItem(m_n64AssisEquipID,true);
			UpdateUI();
		}
	}
}

void GodStrengthenFrame::LockItem( INT64 nID, bool bOperable )
{
	Item *pItem = ItemMgr::Inst()->GetPocketItemByID(nID);
	if(P_VALID(pItem))
		pItem->SetOperable(!bOperable);
}

DWORD GodStrengthenFrame::NetEquipStrengthen( tagNS_GodStrengthen *pNetCmd, DWORD )
{
	if( pNetCmd->dwErrorCode == E_Success )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodStrengthenSucc")]);
		m_n64AssisEquipID = GT_INVALID;
	}
	else if( pNetCmd->dwErrorCode == E_Compose_NotEnough_Money )
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_NotEnough_Money")]);
	else if(pNetCmd->dwErrorCode == E_OnlyCanStrengthenByScript)
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_OnlyCanStrengthenByScript")]);
	else
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodStrengthenFail")]);
	UpdateUI();
	return 0;
}

VOID GodStrengthenFrame::UpdateUI()
{
	m_pEquip->RefreshItem();
	m_pAssisEquip->RefreshItem();
	m_pStcExp->SetText(_T(""));
	m_pStcEquipLevel->SetText(_T(""));
	m_pStcTime->SetText(_T("0"));
	m_pProExp->SetMaxValue(0);
	m_pProExp->SetValue(0);
	m_pProValue->SetText(_T(""));
	m_pStcGolden->SetText(_T(""));
	m_pStcSiller->SetText(_T(""));

	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64EquipID);
	if(!P_VALID(pEquip))
		return;
	m_pEquip->RefreshItem( pEquip->GetItemTypeID(), 0, pEquip->GetItemQuality() );
	TCHAR szTemp[X_LONG_NAME] = {0};
	const tagEquipSpec *pEquipSpec = pEquip->GetEquip();
	if(!P_VALID(pEquipSpec))
		return;
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pEquipSpec->byGodStrengthenLevel);
	m_pStcEquipLevel->SetText(szTemp);

	tagGodStrengthen *pGodStrengInfo = ConsolidateData::Inst()->FindGodStrengInfo(pEquipSpec->byGodStrengthenLevel);
	if(!P_VALID(pGodStrengInfo))
		return;

	DWORD dwSecond = 0;
	tagDWORDTime srcTime = pEquipSpec->dwGodStrengthenTime;
	if( srcTime != 0 )
	{
		tagDWORDTime dstTime = ServerTime::Inst()->CalCurrentServerDwordTime();
		dwSecond = CalcTimeDiff(dstTime, srcTime);
	}
	
	bool bCanGodStrengthen = false;
// 	if( dwSecond > 24*60*60 || dwSecond == 0 )
// 	{
		bCanGodStrengthen = true;
		_sntprintf(szTemp, X_LONG_NAME, _T("0"));
	//}
// 	else if( dwSecond > 23*60*60 )
// 		_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("Time_min")], (24*60*60-dwSecond)/60);
// 	else
// 		_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("GuildList_Offline_Hour")], (24*60*60 - dwSecond)/(60*60));
	m_pStcTime->SetText(szTemp);

	DWORD dwCurExp = pEquipSpec->dwCurGodStrengthenExp;
	DWORD dwTotalExp = 0;
	if( !bCanGodStrengthen )
		dwTotalExp = pGodStrengInfo->dwExp;
	else
	{
		tagGodStrengthen *pGodStrengInfo2 = ConsolidateData::Inst()->FindGodStrengInfo(pEquipSpec->byGodStrengthenLevel+1);
		if(!P_VALID(pGodStrengInfo2))
			dwTotalExp = pGodStrengInfo->dwExp;
		else
			dwTotalExp = pGodStrengInfo->dwExp + pGodStrengInfo2->dwExp;
	}
	DWORD dwCurLvExp = pGodStrengInfo->dwExp;
	m_pProExp->SetMaxValue((FLOAT)dwCurLvExp);
	m_pProExp->SetValue((FLOAT)dwCurExp, true);
	_sntprintf(szTemp, X_LONG_NAME, _T("%d/%d"), dwCurExp, dwCurLvExp);
	m_pProValue->SetText(szTemp);

	Equipment* pAssisEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64AssisEquipID);
	DWORD dwAddExp = 0;
	if(P_VALID(pAssisEquip))
	{
		m_pAssisEquip->RefreshItem( pAssisEquip->GetItemTypeID(), 0, pAssisEquip->GetItemQuality() );
		switch( pAssisEquip->GetItemQuality() )
		{
		case EIQ_Green:		
			dwAddExp = pGodStrengInfo->dwGreenExp; 
			break;
		case EIQ_Blue:		
			dwAddExp = pGodStrengInfo->dwBlueExp; 
			break;
		case EIQ_Orange:	
			dwAddExp = pGodStrengInfo->dwOrangeExp; 
			break;
		case EIQ_Purple:	
			dwAddExp = pGodStrengInfo->dwPurpleExp; 
			break;
		case EIQ_Pink:		
			dwAddExp = pGodStrengInfo->dwPinkExp; 
			break;
		}
		_sntprintf(szTemp, X_LONG_NAME, _T("%d"), dwAddExp);
		m_pStcExp->SetText(szTemp);
	}
	_stprintf(szTemp, _T("%d"), MSilver2DBGold(pGodStrengInfo->dwCost));
	m_pStcGolden->SetText(szTemp);
	_stprintf(szTemp, _T("%d"), MSilver2DBSilver(pGodStrengInfo->dwCost));
	m_pStcSiller->SetText(szTemp);

	if(m_n64AssisEquipID != GT_INVALID && m_n64EquipID != GT_INVALID )
	{
		if( dwCurExp + dwAddExp < dwTotalExp )
			m_pBtnOk->SetEnable(true);
		else if( pEquipSpec->byGodStrengthenLevel != MAX_GODSTRENGTHEN_LEVEL && dwCurExp == dwTotalExp - 1 && pEquipSpec->byGodStrengthenLevel != MAX_GODSTRENGTHEN_LEVEL - 1)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GodStrengthenCanNotLevel")]);
			m_pBtnOk->SetEnable(false);
		}
		else if( dwCurExp + dwAddExp >= dwTotalExp )
		{
			if(pEquipSpec->byGodStrengthenLevel != MAX_GODSTRENGTHEN_LEVEL - 1 || !bCanGodStrengthen)
				_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("GodStrengthenExpValue")], dwTotalExp - dwCurExp - 1 );
			else 
				_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("GodStrengthenExpValue")], dwTotalExp - dwCurExp );

			if( P_VALID(GetObj("GodStrengthenExpValue")) )
				KillObj("GodStrengthenExpValue");
			CreateObj( "GodStrengthenExpValue", "MsgBox" );
			TObjRef<MsgBox>("GodStrengthenExpValue")->Init( _T(""), szTemp, _T("OnGodStrengthenExpValue") );
		}
	}
	else
		m_pBtnOk->SetEnable(false);
	
}

DWORD GodStrengthenFrame::OnGodStrengthenExpValue( tagMsgBoxEvent *pGameEvent )
{
	if( pGameEvent->eResult == MBF_OK )
		m_pBtnOk->SetEnable(true);
	else
	{
		LockItem(m_n64AssisEquipID, false);
		m_n64AssisEquipID = GT_INVALID;
		UpdateUI();
	}
	return 0;
}