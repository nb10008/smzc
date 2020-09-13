#include "stdafx.h"
#include "Item.h"
#include "RoleMgr.h"
#include "ItemMgr.h"
#include "ItemFrame.h"
#include "ItemEvent.h"
#include "ItemButton.h"
#include "IconStatic.h"
#include "CurrencyMgr.h"
#include "ToolTipFrame.h"
#include "CombatSysUtil.h"
#include "ToolTipCreator.h"
#include "ConsolidateData.h"
#include "EquipStrengthenFrame.h"
#include "..\WorldDefine\msg_compose.h"
#include "..\WorldDefine\msg_loot.h"
#include "..\WorldDefine\currency_define.h"
#include "GUIEffectMgr.h"
#include "ItemProtoData.h"

EquipStrengthenFrame::EquipStrengthenFrame():m_Trunk(this)
{
	m_pWndMain		= NULL;
	m_pWndCaption	= NULL;
	m_pEquip		= NULL;
	m_pStcEquipName	= NULL;
	m_pStone		= NULL;
	m_pStcStoneName	= NULL;
	m_pLucky		= NULL;
	m_pStcLuckyName	= NULL;
	m_pGuard		= NULL;
	m_pStcGuardName	= NULL;
	m_pStcSucRate	= NULL;
	m_pBtnExit		= NULL;
	m_pBtnOk		= NULL;
	m_pBtnCancel	= NULL;
	m_pStcGolden	= NULL;
	m_pStcSiller	= NULL;
	m_pProTime		= NULL;
	m_pItemEquip	= NULL;
	m_pItemStone	= NULL;
	m_pItemLucky	= NULL;
	m_pItemGuard	= NULL;
	m_pStcEquipState = NULL;
	m_dwNPCID		= GT_INVALID;
	m_escapeTime	= GT_INVALID;
	m_dwRecivMegTime = GT_INVALID;
	m_bProcessing = false;
	m_pStcResult = NULL;
}

EquipStrengthenFrame::~EquipStrengthenFrame()
{

}

BOOL EquipStrengthenFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pMgr->RegisterEventHandle( _T("ItemEvent_Move2EquipStrengthen"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipStrengthenFrame::OnEventPutItem) );
	m_pMgr->RegisterEventHandle( _T("OnNoGuardFu"),						(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipStrengthenFrame::OnNoGuardFu) );
	m_pMgr->RegisterEventHandle( _T("ItemEvent_UpdateItem"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipStrengthenFrame::OnItemEvent_UpdateItem) );
	m_pCmdMgr->Register( "NS_StrengthenEquip",							(NETMSGPROC)m_Trunk.sfp2(&EquipStrengthenFrame::NetEquipStrengthen),		_T("NS_StrengthenEquip"));
	m_pCmdMgr->Register( "NS_RolePutDownItem",							(NETMSGPROC)m_Trunk.sfp2(&EquipStrengthenFrame::NetRolePutDownItem),		_T("NS_RolePutDownItem"));
	return TRUE;
}

BOOL EquipStrengthenFrame::Destroy()
{
	CloseAllUiEffect();

	GameFrame::Destroy();
	if( P_VALID(m_pWndMain) )
		m_pGUI->AddToDestroyList(m_pWndMain);

	m_pMgr->UnRegisterEventHandler( _T("ItemEvent_Move2EquipStrengthen"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipStrengthenFrame::OnEventPutItem));
	m_pMgr->UnRegisterEventHandler( _T("OnNoGuardFu"),						(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipStrengthenFrame::OnNoGuardFu) );
	m_pMgr->UnRegisterEventHandler( _T("ItemEvent_UpdateItem"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipStrengthenFrame::OnItemEvent_UpdateItem) );
	m_pCmdMgr->UnRegister( "NS_StrengthenEquip",							(NETMSGPROC)m_Trunk.sfp2(&EquipStrengthenFrame::NetEquipStrengthen) );
	m_pCmdMgr->UnRegister( "NS_RolePutDownItem",							(NETMSGPROC)m_Trunk.sfp2(&EquipStrengthenFrame::NetRolePutDownItem) );

	if( P_VALID(GetObj("NoGuardFu")) )
		KillObj("NoGuardFu");
	LockItem(FALSE);

	SetStrategy(EIST_ItemPocket);

	return TRUE;
}

BOOL EquipStrengthenFrame::EscCancel()
{
	if( m_bProcessing )
	{
		LockItem(false);
		m_bProcessing = false;
		m_pProTime->SetValue(0, true);
		return TRUE;
	}
	m_pMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL EquipStrengthenFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\equip_aggrandizement.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "decomposition\\decompositionbackpic\\equipback\\equipback\\equip", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "decomposition\\decompositionbackpic\\materialback\\materialback\\material", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "decomposition\\decompositionbackpic\\assuranceback\\assruancelback\\assurance", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "decomposition\\decompositionbackpic\\luckback\\luckback\\lucky", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&element, "decomposition\\decompositionbackpic\\pic", "ClassName", "IconStatic");

	m_pWndMain = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pWndCaption	= m_pWndMain->GetChild( _T("decompositionbackpic") );

	m_pEquip		= (ItemButton*)m_pWndCaption->GetChild( _T("equipback\\equipback\\equip") );
	m_pStcEquipName	= (GUIStaticEx*)m_pWndCaption->GetChild( _T("equipback\\equipname") );
	m_pStcEquipState= (GUIStatic*)m_pWndCaption->GetChild( _T("equipback\\equipstate") );
	m_pStone		= (ItemButton*)m_pWndCaption->GetChild( _T("materialback\\materialback\\material") );
	m_pStcStoneName	= (GUIStatic*)m_pWndCaption->GetChild( _T("materialback\\materialname") );
	m_pLucky		= (ItemButton*)m_pWndCaption->GetChild( _T("luckback\\luckback\\lucky") );
	m_pStcLuckyName	= (GUIStatic*)m_pWndCaption->GetChild( _T("luckback\\name") );
	m_pGuard		= (ItemButton*)m_pWndCaption->GetChild( _T("assuranceback\\assruancelback\\assurance") );
	m_pStcGuardName	= (GUIStatic*)m_pWndCaption->GetChild( _T("assuranceback\\name") );
	m_pProTime		= (GUIProgress*)m_pWndCaption->GetChild( _T("timepic\\time") );
	m_pStcSucRate	= (GUIStaticEx*)m_pWndCaption->GetChild( _T("wordback\\num0") );
	m_pStcSucRate->SetText(_T(""));
	m_pStcGolden	= (GUIStatic*)m_pWndCaption->GetChild( _T("wordback\\goldnum") );
	m_pStcGolden->SetText(_T("0"));
	m_pStcSiller	= (GUIStatic*)m_pWndCaption->GetChild( _T("wordback\\silvernum") );
	m_pStcSiller->SetText(_T("0"));
	m_pBtnOk		= (GUIButton*)m_pWndCaption->GetChild( _T("startbutton") );
	m_pBtnCancel	= (GUIButton*)m_pWndCaption->GetChild( _T("closebutton") );
	m_pBtnExit		= (GUIButton*)m_pWndCaption->GetChild( _T("close") );
	m_pStcResult	= (IconStatic*)m_pWndCaption->GetChild( _T("pic") );
	m_pStcResult->SetInvisible(TRUE);
	m_pBtnOk->SetEnable(false);

	m_pEquip->RefreshItem();
	m_pStone->RefreshItem();
	m_pLucky->RefreshItem();
	m_pGuard->RefreshItem();

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWndMain->GetFullName().c_str(), m_Trunk.sfp1(&EquipStrengthenFrame::EventHandler));

	return TRUE;
}

VOID EquipStrengthenFrame::Update()
{
	GameFrame::Update();

	if(m_bProcessing)
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
			tagNC_StrengthenEquip msg;
			msg.n64ToBeStrengthenedEquip = m_pItemEquip->GetData()->n64Serial;
			msg.n64StrengthenStone = m_pItemStone->GetData()->n64Serial;
			msg.n64LuckySign = P_VALID(m_pItemLucky) ? m_pItemLucky->GetData()->n64Serial : GT_INVALID;
			msg.n64ProtectedSign = P_VALID(m_pItemGuard) ? m_pItemGuard->GetData()->n64Serial : GT_INVALID;
			m_pSession->Send(&msg);
			m_bProcessing = false;
			LockItem(FALSE);
		}
	}

	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		if( m_dwRecivMegTime != GT_INVALID )
			m_pStcResult->SetInvisible(TRUE);
		m_escapeTime = 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{
			Destroy();
			return;
		}
		if (RoleMgr::Inst()->IsOutValidDist( m_dwNPCID ) )
		{
			m_pMgr->AddToDestroyList(this);
		}
	}
}

DWORD EquipStrengthenFrame::EventHandler(tagGUIEvent* pEvent)
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
				if(m_bProcessing)
					LockItem(false);
				m_pMgr->AddToDestroyList(this);
			}
			else if( pWnd == m_pBtnOk )
			{
				if( CanStartStrengthen() )
				{
					if( P_VALID(m_pItemGuard) || (P_VALID(m_pItemLucky) && m_pItemLucky->GetProto()->nSpecFuncVal1 == 6 ) )
					{
						LockItem(true);
						m_pProTime->SetSpeed(100);
						m_pProTime->SetValue(0, true);
						m_pProTime->SetMaxValue(3.0f);
						m_pProTime->SetRefresh();
						m_bProcessing = true;
						m_pBtnOk->SetEnable(false);

						GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
						GUIEffectMgr::Inst()->StopEffectByType(GUIEffectMgr::EET_EnforcSucess);
						GUIEffectMgr::Inst()->StopEffectByType(GUIEffectMgr::EET_EnforcFail);
						GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui012"),pDesktop->GetView(),pDesktop->GetSize(),GUIEffectMgr::EET_Enforcing);

					}
					else
					{
						if( P_VALID(GetObj("NoGuardFu")) )
							KillObj("NoGuardFu");
						CreateObj( "NoGuardFu", "MsgBox" );
						TObjRef<MsgBox>("NoGuardFu")->Init( _T(""), g_StrTable[_T("NoGuardFu")], _T("OnNoGuardFu") );
					}
				}
			}
		}
		break;
	case EGUISCB_Drop:
		if( pWnd == m_pEquip || pWnd == m_pStone ||
			pWnd == m_pLucky || pWnd == m_pGuard )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutItem((INT16)LOWORD(pEvent->dwParam2));
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if( pWnd == m_pEquip && P_VALID(m_pItemEquip) )
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItemEquip).c_str());
			}
			else if( pWnd == m_pStone && P_VALID(m_pItemStone) )
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItemStone).c_str());
			}
			else if( pWnd == m_pLucky && P_VALID(m_pItemLucky) )
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItemLucky).c_str());
			}
			else if( pWnd == m_pGuard && P_VALID(m_pItemGuard) )
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItemGuard).c_str());
			}
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

DWORD EquipStrengthenFrame::OnEventPutItem( tagItemPosChangeExEvent* pGameEvent )
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;
	PutItem( pGameEvent->n16IndexSrc );
	return 0;
}

VOID EquipStrengthenFrame::PutItem( INT16 nPos )
{
	if( m_bProcessing )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotChangeItem")]);
		return;
	}

	if(!P_VALID(m_pItemLucky))	//没有幸运符技能加成也要显示
	{ 
		TCHAR szText[128] = {0};
		DWORD dwSkillPct = 0;
		if(P_VALID(m_pItemEquip))
			dwSkillPct = ConsolidateData::Inst()->FindSkillPct(m_pItemEquip->GetItemId());
		if(dwSkillPct != 0)
		{
			dwSkillPct /= 100;
			_sntprintf( szText, X_LONG_NAME, _T("<color=0xffff0000>%d%%"), dwSkillPct );
			m_pStcSucRate->SetText(szText);
		}
	}

	Item *pItem = ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(pItem))
		return;

	const tagItemProto* pItemProto = pItem->GetProto();
	TCHAR szTemp[X_LONG_NAME] = {0};
	if( !P_VALID(pItemProto) && !P_VALID(pItemProto->pDisplayInfo) )
		return;

	if(MIsEquipment(pItemProto->dwTypeID))
	{
		if( pItemProto->eType == EIT_FaBao  || pItemProto->eType == EIT_Fashion )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FaBaoCanNotStrength")]);
			return;
		}
		Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
		if(!P_VALID(pEquip) || !P_VALID(pEquip->GetEquip()))
			return;
		if(pEquip->GetEquip()->byConsolidateLevel == tagEquipSpec::ECL_AftConMaxLevel)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipHasTopLevel")]);
			return;
		}
		ClearUI();
		m_pEquip->RefreshItem( pEquip->GetItemTypeID(), 0, pEquip->GetItemQuality() );
		DWORD dwColor = 0;
		switch( pEquip->GetItemQuality() )
		{
		case EIQ_White:		dwColor = 0xFFFFFFFF; break;
		case EIQ_Yellow:	dwColor = 0xFFFFFF00; break;
		case EIQ_Green:		dwColor = 0xFF00FF00; break;
		case EIQ_Blue:		dwColor = 0xFF0000FF; break;
		case EIQ_Orange:	dwColor = 0xFFFF8000; break;
		case EIQ_Purple:	dwColor = 0xFF9932CD; break;
		case EIQ_Pink:		dwColor = 0xFFFFD7EA; break;
		}
		_sntprintf( szTemp, X_LONG_NAME, _T("<color=0x%x>%s+%d"), dwColor, pItemProto->pDisplayInfo->szName, pEquip->GetEquip()->byConsolidateLevel );
		m_pStcEquipName->SetText(szTemp);
		// 神铸状态
		if(pEquip->GetEquip()->byGodStrengthenLevel != 0 && 
			pEquip->GetEquip()->byGodStrengthenLevel != GT_INVALID )
			m_pStcEquipState->SetText(g_StrTable[_T("EquipSoaring")]);
		else
			m_pStcEquipState->SetText(g_StrTable[_T("EquipNoSoaring")]);
		DWORD dwMoney = ConsolidateData::Inst()->FindEquipStrengConst( pItemProto->eTypeEx, pEquip->GetEquip()->byConsolidateLevel + 1);
		_stprintf(szTemp, _T("%d"), MSilver2DBGold(dwMoney));
		m_pStcGolden->SetText(szTemp);
		_stprintf(szTemp, _T("%d"), MSilver2DBSilver(dwMoney));
		m_pStcSiller->SetText(szTemp);
		m_pItemEquip = pEquip;
		m_pBtnOk->SetEnable(false);

		if(!P_VALID(m_pItemLucky))	//没有幸运符技能加成也要显示
		{ 
			TCHAR szText[128] = {0};
			DWORD dwSkillPct = 0;
			if(P_VALID(m_pItemEquip))
				dwSkillPct = ConsolidateData::Inst()->FindSkillPct(m_pItemEquip->GetItemId());
			if(dwSkillPct != 0)
			{
				dwSkillPct /= 100;
				_sntprintf( szText, X_LONG_NAME, _T("<color=0xffff0000>%d%%"), dwSkillPct );
				m_pStcSucRate->SetText(szText);
			}
		}

		return;
	}

	if( m_pItemEquip == NULL )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Decompose_Error4")]);
		return;
	}

	const tagItemProto *pEquipProto = m_pItemEquip->GetProto();
	const tagEquipSpec *pEquipSpe = m_pItemEquip->GetEquip();
	if( !P_VALID(pEquipProto) || !P_VALID(pEquipSpe) )
		return;

	if( pItemProto->eStuffType == ConsolidateData::Inst()->FindEquipStrengStuff(pEquipProto->eTypeEx, pEquipSpe->byConsolidateLevel + 1) )
	{
		if( pItemProto->nSpecFuncVal1 < pEquipProto->byLevel)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotPutQiangHuaStone")]);
			return;
		}
		m_pStone->RefreshItem( pItem->GetItemTypeID(), 0, pItem->GetItemQuality() );
		m_pStcStoneName->SetText(pItemProto->pDisplayInfo->szName);
		m_pItemStone = pItem;
		if(P_VALID(m_pItemEquip) && P_VALID(m_pItemStone))
			m_pBtnOk->SetEnable(true);
	}
	else if( pItemProto->eSpecFunc == EISF_BaoDiFu )
	{
		if( P_VALID(pItemProto->nSpecFuncVal2 ) && pEquipSpe->byConsolidateLevel > pItemProto->nSpecFuncVal2)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotUseGuildFu")]);
			return;
		}

		if(P_VALID(m_pItemLucky))
		{
			const tagItemProto* pItemProtoT = m_pItemLucky->GetProto();
			if ( P_VALID(pItemProtoT) && pItemProtoT->nSpecFuncVal1 == 6 )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("notneedtouse_linker")]);
				return;
			}
		}
		m_pGuard->RefreshItem( pItem->GetItemTypeID(), 0, pItem->GetItemQuality() );
		m_pStcGuardName->SetText(pItemProto->pDisplayInfo->szName);
		m_pItemGuard = pItem;
	}
	else if( pItemProto->eSpecFunc == EISF_LuckyFu )
	{
		if( pItemProto->nSpecFuncVal1 == 5 )
		{
			if( pEquipSpe->byGodStrengthenLevel == 0 || pEquipSpe->byGodStrengthenLevel == GT_INVALID )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipNotSoared")]);
				return;
			}
			else if( (pEquipSpe->byGodStrengthenLevel != 0 && pEquipSpe->byGodStrengthenLevel != GT_INVALID) && 
				pEquipSpe->byConsolidateLevel < tagEquipSpec::ECL_BefConMaxLevel )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotUseShenZhuFu")]);
				return;
			}
		}
		else if (pItemProto->nSpecFuncVal1 == 6 &&  pEquipSpe->byConsolidateLevel >= pItemProto->nSpecFuncVal2 )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotUse_min_soullinker")]);
			return;
		}
		else if( pItemProto->nSpecFuncVal1 != 5 &&  pItemProto->nSpecFuncVal1 != 6 &&
			pEquipSpe->byGodStrengthenLevel != 0 && 
			pEquipSpe->byGodStrengthenLevel != GT_INVALID && pEquipSpe->byConsolidateLevel >= tagEquipSpec::ECL_BefConMaxLevel )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyShenZhuFuCanUse")]);
			return;
		}

		if (pItemProto->nSpecFuncVal1 == 6 && P_VALID(m_pItemGuard))
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("notneedtouse_linker")]);
		}
		m_pLucky->RefreshItem( pItem->GetItemTypeID(), 0, pItem->GetItemQuality() );
		m_pStcLuckyName->SetText(pItemProto->pDisplayInfo->szName);
		m_pItemLucky = pItem;
		DWORD dwIndex = pItemProto->nSpecFuncVal1 * 100 + pEquipSpe->byConsolidateLevel + 1;
		DWORD dwPct = ConsolidateData::Inst()->FindLuckyPct(dwIndex);
		DWORD dwSkillPct = 0;
		if(P_VALID(m_pItemEquip))
			dwSkillPct = ConsolidateData::Inst()->FindSkillPct(m_pItemEquip->GetItemId());
		if(dwPct != 0 || dwSkillPct != 0)
		{
			dwPct /= 100;
			dwSkillPct /= 100;
			_sntprintf( szTemp, X_LONG_NAME, _T("%d%%<color=0xffff0000>+%d%%"), dwPct, dwSkillPct );
			m_pStcSucRate->SetText(szTemp);
		}
	}
	else if( !MIsEquipment(pItemProto->dwTypeID) )
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotPutInCurUI")]);
}


void EquipStrengthenFrame::LockItem( bool bOperable )
{
	if( P_VALID(m_pItemEquip) )
		m_pItemEquip->SetOperable(!bOperable);
	if( P_VALID(m_pItemStone) )
		m_pItemStone->SetOperable(!bOperable);
	if( P_VALID(m_pItemLucky) )
		m_pItemLucky->SetOperable(!bOperable);
	if( P_VALID(m_pItemGuard) )
		m_pItemGuard->SetOperable(!bOperable);
}

DWORD EquipStrengthenFrame::OnNoGuardFu( tagMsgBoxEvent *pGameEvent )
{
	if( pGameEvent->eResult == MBF_OK )
	{
		if(!m_pBtnOk->IsEnable())
			return 0;
		LockItem(true);
		m_pProTime->SetSpeed(100);
		m_pProTime->SetValue(0, true);
		m_pProTime->SetMaxValue(3.0f);
		m_pProTime->SetRefresh();
		m_bProcessing = true;
		m_pBtnOk->SetEnable(false);

		GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
		GUIEffectMgr::Inst()->StopEffectByType(GUIEffectMgr::EET_EnforcSucess);
		GUIEffectMgr::Inst()->StopEffectByType(GUIEffectMgr::EET_EnforcFail);
		GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui012"),pDesktop->GetView(),pDesktop->GetSize(),GUIEffectMgr::EET_Enforcing);
	}
	return 0;
}

DWORD EquipStrengthenFrame::NetRolePutDownItem( tagNS_RolePutDownItem *pNetCmd, DWORD )
{
	if( pNetCmd->dwErrorCode == E_Success )
	{
		if( P_VALID(m_pItemEquip) )
		{
			INT64 n64ID = m_pItemEquip->GetData()->n64Serial;
			Item *pItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
			if(!P_VALID(pItem))
			{
				m_pEquip->RefreshItem();
				m_pStcEquipName->SetText(_T(""));
				m_pStcEquipState->SetText(_T(""));
				m_pStcGolden->SetText(_T(""));
				m_pStcSiller->SetText(_T(""));
				m_pItemEquip = NULL;

				if(!P_VALID(m_pItemLucky))	//没有幸运符技能加成也要显示
				{ 
					TCHAR szText[128] = {0};
					DWORD dwSkillPct = 0;
					if(P_VALID(m_pItemEquip))
						dwSkillPct = ConsolidateData::Inst()->FindSkillPct(m_pItemEquip->GetItemId());
					if(dwSkillPct != 0)
					{
						dwSkillPct /= 100;
						_sntprintf( szText, X_LONG_NAME, _T("<color=0xffff0000>%d%%"), dwSkillPct );
						m_pStcSucRate->SetText(szText);
					}
				}

				m_pBtnOk->SetEnable(false);
				return 0;
			}
		}
		if( P_VALID(m_pItemStone) )
		{
			INT64 n64ID = m_pItemStone->GetData()->n64Serial;
			Item *pItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
			if(!P_VALID(pItem))
			{
				m_pStone->RefreshItem();
				m_pStcStoneName->SetText(_T(""));
				m_pItemStone = NULL;
				m_pBtnOk->SetEnable(false);
				return 0;
			}
			
		}
		if( P_VALID(m_pItemGuard) )
		{
			INT64 n64ID = m_pItemGuard->GetData()->n64Serial;
			Item *pItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
			if(!P_VALID(pItem))
			{
				m_pGuard->RefreshItem();
				m_pStcGuardName->SetText(_T(""));
				m_pItemGuard = NULL;
				return 0;
			}
		}
		if( P_VALID(m_pItemLucky) )
		{
			INT64 n64ID = m_pItemLucky->GetData()->n64Serial;
			Item *pItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
			if(!P_VALID(pItem))
			{
				m_pLucky->RefreshItem();
				m_pStcLuckyName->SetText(_T(""));
				m_pItemLucky = NULL;

				TCHAR szText[128] = {0};
				DWORD dwSkillPct = 0;
				if(P_VALID(m_pItemEquip))
					dwSkillPct = ConsolidateData::Inst()->FindSkillPct(m_pItemEquip->GetItemId());
				if(dwSkillPct != 0)
				{
					dwSkillPct /= 100;
					_sntprintf( szText, X_LONG_NAME, _T("<color=0xffff0000>%d%%"), dwSkillPct );
					m_pStcSucRate->SetText(szText);
				}
				return 0;
			}	
		}
	}
	return 0;
}

DWORD EquipStrengthenFrame::NetEquipStrengthen( tagNS_StrengthenEquip *pNetCmd, DWORD )
{
	GUIEffectMgr::Inst()->StopEffectByType(GUIEffectMgr::EET_Enforcing);

	if( pNetCmd->dwErrorCode == E_Success )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipStrengthenSucc")]);
// 		TCHAR* szSucc = _T("data\\ui\\compose\\l_smith-sucess.dds");
// 		m_pStcResult->SetPic( szSucc, tagRect(0, 0, 220, 100) );
// 		m_pStcResult->FlipToTop();
		GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
		GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui013"),pDesktop->GetView(),pDesktop->GetSize(),GUIEffectMgr::EET_EnforcSucess);
	}
	else if (pNetCmd->dwErrorCode == E_OnlyCanStrengthenByScript)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_OnlyCanStrengthenByScript")]);
	}
	else
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipStrengthenFail")]);
// 		TCHAR* szFail = _T("data\\ui\\compose\\l_smith-lose.dds");
// 		m_pStcResult->SetPic( szFail, tagRect(0, 0, 220, 100) );
// 		m_pStcResult->FlipToTop();
		GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
		GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui014"),pDesktop->GetView(),pDesktop->GetSize(),GUIEffectMgr::EET_EnforcFail);
	}
	UpdateUI();
	m_pProTime->SetValue(0, true);
	m_pStcResult->SetInvisible(FALSE);
	m_dwRecivMegTime = Kernel::Inst()->GetAccumTimeDW();
	return 0;
}

BOOL EquipStrengthenFrame::CanStartStrengthen()
{
	const tagItemProto	*pItemProto = m_pItemEquip->GetProto();
	const tagEquipSpec	*pEquipSpec = m_pItemEquip->GetEquip();
	if( !P_VALID(pItemProto) || !P_VALID(pEquipSpec) )
		return FALSE;
	DWORD dwMoney = ConsolidateData::Inst()->FindEquipStrengConst( pItemProto->eTypeEx, pEquipSpec->byConsolidateLevel + 1 );
	INT64 n64TotalMoney = CurrencyMgr::Inst()->GetBagSilver();
	if( dwMoney > n64TotalMoney )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ConsolidateMoneyError")]);
		return FALSE;
	}
	INT nQlty = m_pItemEquip->GetItemQuality();
	const tagEquipQltyEffect* pAtt = ItemProtoData::Inst()->FindEquipQltyEffect(nQlty);
	if( P_VALID(pAtt) && pEquipSpec->byConsolidateLevel >= pAtt->nMaxStrengthenLevel + pEquipSpec->byGodStrengthenLevel )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipHasTopLevel")]);
		return FALSE;
	}

	if (P_VALID(m_pItemGuard) && (P_VALID(m_pItemLucky) && m_pItemLucky->GetProto()->nSpecFuncVal1 == 6 ))
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("notneedtouse_linker")]);
		return FALSE;
	}

	if (P_VALID(m_pItemLucky) && m_pItemLucky->GetProto()->nSpecFuncVal1 == 6 )
	{
		const tagEquipSpec *pEquipSpe = m_pItemEquip->GetEquip();
		if (P_VALID(pEquipSpe) &&  pEquipSpe->byConsolidateLevel >=m_pItemLucky->GetProto()->nSpecFuncVal2 )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotUse_min_soullinker")]);

			m_pItemLucky = NULL;
			m_pLucky->RefreshItem();
			m_pStcLuckyName->SetText(_T(""));
			
			m_pItemStone = NULL;
			m_pStone->RefreshItem();
			m_pStcStoneName->SetText(_T(""));

			m_pWndMain->SetRefresh(true);

			return FALSE;
		}

	}

	return TRUE;
}

VOID EquipStrengthenFrame::ClearUI()
{
	LockItem(FALSE);
	m_pItemEquip = NULL;
	m_pItemStone = NULL;
	m_pItemLucky = NULL;
	m_pItemGuard = NULL;
	m_pEquip->RefreshItem();
	m_pStone->RefreshItem();
	m_pLucky->RefreshItem();
	m_pGuard->RefreshItem();
	m_pStcEquipName->SetText(_T(""));
	m_pStcStoneName->SetText(_T(""));
	m_pStcLuckyName->SetText(_T(""));
	m_pStcGuardName->SetText(_T(""));
	m_pStcEquipState->SetText(_T(""));
	m_pProTime->SetValue(0);
	m_pStcSucRate->SetText(_T(""));
	m_pStcGolden->SetText(_T("0"));
	m_pStcSiller->SetText(_T("0"));
	m_pWndMain->SetRefresh(true);
}

VOID EquipStrengthenFrame::UpdateUI()
{
	if( P_VALID(m_pItemEquip) )
	{
		INT64 n64ID = m_pItemEquip->GetData()->n64Serial;
		Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(n64ID);
		if(!P_VALID(pEquip))
		{
			ClearUI();
			return;
		}

		DWORD dwColor = 0;
		switch( pEquip->GetItemQuality() )
		{
		case EIQ_White:		dwColor = 0xFFFFFFFF; break;
		case EIQ_Yellow:	dwColor = 0xFFFFFF00; break;
		case EIQ_Green:		dwColor = 0xFF00FF00; break;
		case EIQ_Blue:		dwColor = 0xFF0000FF; break;
		case EIQ_Orange:	dwColor = 0xFFFF8000; break;
		case EIQ_Purple:	dwColor = 0xFF9932CD; break;
		case EIQ_Pink:		dwColor = 0xFFFFD7EA; break;
		}
		TCHAR szTemp[X_LONG_NAME] = {0};
		_sntprintf( szTemp, X_LONG_NAME, _T("<color=0x%x>%s+%d"), dwColor, pEquip->GetProto()->pDisplayInfo->szName, pEquip->GetEquip()->byConsolidateLevel );
		m_pStcEquipName->SetText(szTemp);

		DWORD dwMoney = ConsolidateData::Inst()->FindEquipStrengConst( pEquip->GetProto()->eTypeEx, pEquip->GetEquip()->byConsolidateLevel + 1);
		_stprintf(szTemp, _T("%d"), MSilver2DBGold(dwMoney));
		m_pStcGolden->SetText(szTemp);
		_stprintf(szTemp, _T("%d"), MSilver2DBSilver(dwMoney));
		m_pStcSiller->SetText(szTemp);
	}
	if( P_VALID(m_pItemStone) )
	{
		INT64 n64ID = m_pItemStone->GetData()->n64Serial;
		Item *pItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
		if(!P_VALID(pItem))
		{
			m_pStone->RefreshItem();
			m_pStcStoneName->SetText(_T(""));
			m_pItemStone = NULL;
		}
		if( P_VALID(pItem) && pItem->GetProto()->eStuffType == ConsolidateData::Inst()->FindEquipStrengStuff(m_pItemEquip->GetProto()->eTypeEx, m_pItemEquip->GetEquip()->byConsolidateLevel + 1) )
		{
			if( pItem->GetProto()->nSpecFuncVal1 < (m_pItemEquip->GetProto()->byLevel) )
			{
				m_pStone->RefreshItem();
				m_pStcStoneName->SetText(_T(""));
				m_pItemStone = NULL;
			}
		}
		else
		{
			m_pStone->RefreshItem();
			m_pStcStoneName->SetText(_T(""));
			m_pItemStone = NULL;
		}
	}

	if(P_VALID(m_pItemEquip) && P_VALID(m_pItemStone))
		m_pBtnOk->SetEnable(true);

	if( P_VALID(m_pItemGuard) )
	{
		INT64 n64ID = m_pItemGuard->GetData()->n64Serial;
		Item *pItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
		if( !P_VALID(pItem) || 
			( P_VALID(pItem) && 
			 P_VALID( m_pItemGuard->GetProto()->nSpecFuncVal2 ) &&
			(m_pItemEquip->GetEquip()->byConsolidateLevel > m_pItemGuard->GetProto()->nSpecFuncVal2 ) ) )
		{
			m_pGuard->RefreshItem();
			m_pStcGuardName->SetText(_T(""));
			m_pItemGuard = NULL;
		}
	}
	if( P_VALID(m_pItemLucky) )
	{
		INT64 n64ID = m_pItemLucky->GetData()->n64Serial;
		Item *pItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
		if(!P_VALID(pItem))
		{
			m_pLucky->RefreshItem();
			m_pStcLuckyName->SetText(_T(""));
			m_pItemLucky = NULL;

			TCHAR szText[128] = {0};
			DWORD dwSkillPct = 0;
			if(P_VALID(m_pItemEquip))
				dwSkillPct = ConsolidateData::Inst()->FindSkillPct(m_pItemEquip->GetItemId());
			if(dwSkillPct != 0)
			{
				dwSkillPct /= 100;
				_sntprintf( szText, X_LONG_NAME, _T("<color=0xffff0000>%d%%"), dwSkillPct );
				m_pStcSucRate->SetText(szText);
			}
		}
		else
		{
			const tagItemProto *pItemProto = m_pItemLucky->GetProto();
			DWORD dwIndex = pItemProto->nSpecFuncVal1 * 100 + m_pItemEquip->GetEquip()->byConsolidateLevel + 1;
			DWORD dwPct = ConsolidateData::Inst()->FindLuckyPct(dwIndex);
			DWORD dwSkillPct = 0;
			TCHAR szTemp[128] = {0};
			if(P_VALID(m_pItemEquip))
				dwSkillPct = ConsolidateData::Inst()->FindSkillPct(m_pItemEquip->GetItemId());
			if(dwPct != 0 || dwSkillPct != 0)
			{
				dwPct /= 100;
				dwSkillPct /= 100;
				_sntprintf( szTemp, X_LONG_NAME, _T("%d%%<color=0xffff0000>+%d%%"), dwPct, dwSkillPct );
				m_pStcSucRate->SetText(szTemp);
			}
		}
	}
}

VOID EquipStrengthenFrame::CloseAllUiEffect()
{
	GUIEffectMgr::Inst()->RemoveOneEffectByType(GUIEffectMgr::EET_Enforcing);
	GUIEffectMgr::Inst()->RemoveOneEffectByType(GUIEffectMgr::EET_EnforcSucess);
	GUIEffectMgr::Inst()->RemoveOneEffectByType(GUIEffectMgr::EET_EnforcFail);
}

DWORD EquipStrengthenFrame::OnItemEvent_UpdateItem(tagGameEvent* pEvent)
{
	if(P_VALID(m_pItemGuard))	//判断保底
	{
		Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_pItemGuard->GetData()->n64Serial);
		if(!P_VALID(pItem))
		{
			m_pItemGuard = NULL;
			m_pGuard->RefreshItem();
		}
	}

	if(P_VALID(m_pItemLucky))	//判断幸运符
	{
		Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_pItemLucky->GetData()->n64Serial);
		if(!P_VALID(pItem))
		{
			m_pItemLucky = NULL;
			m_pLucky->RefreshItem();
		}
	}

	if(P_VALID(m_pItemStone))	//判断强化石
	{
		Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_pItemStone->GetData()->n64Serial);
		if(!P_VALID(pItem))
		{
			m_pItemStone = NULL;
			m_pStone->RefreshItem();
		}
	}

	return 0;
}	