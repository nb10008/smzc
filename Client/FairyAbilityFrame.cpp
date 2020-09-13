#include "StdAfx.h"
#include "Item.h"
#include "ItemMgr.h"
#include "ProgressEx.h"
#include "ItemProtoData.h"
#include "ToolTipFrame.h"
#include "CombatSysUtil.h"
#include "FairyUpgradeFrame.h"
#include "FairyAbilityFrame.h"
#include "..\WorldDefine\msg_compose.h"
#include "..\WorldDefine\currency_define.h"

FairyAbilityFrame::FairyAbilityFrame(void):
m_Trunk(this),
m_pWnd(NULL),
m_dwUnsureSkill(GT_INVALID)
{
	for( int i= 0; i < MaxFairySkillNum; i++ )
		m_dwFairySkill[i] = GT_INVALID;
}

FairyAbilityFrame::~FairyAbilityFrame()
{

}

BOOL FairyAbilityFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{
	if( !GameFrame::Init( pMgr, pFather, dwParam) )
		return FALSE;
	m_pCmdMgr->Register( "NS_FairyStuntBook",	(NETMSGPROC)m_Trunk.sfp2(&FairyAbilityFrame::NetLearnFairyBook),	_T("NS_FairyStuntBook"));
	m_pCmdMgr->Register( "NS_FairyStuntConfirm",	(NETMSGPROC)m_Trunk.sfp2(&FairyAbilityFrame::NetLearnFairySkill),_T("NS_FairyStuntConfirm"));
	m_pCmdMgr->Register( "NS_FairyAddPromoteTimes",	(NETMSGPROC)m_Trunk.sfp2(&FairyAbilityFrame::NetAddFairyPromoteTimes),_T("NS_FairyAddPromoteTimes"));//add by xp
	m_pMgr->RegisterEventHandle( _T("LearnFairySkill"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&FairyAbilityFrame::OnLearnFairySkill)) );
	m_pMgr->RegisterEventHandle( _T("QuitFairySkill"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&FairyAbilityFrame::OnQuitFairySkill)) );
	m_pMgr->RegisterEventHandle( _T("UpdateFairyAbility"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&FairyAbilityFrame::OnUpdateFairyAbility)) );
	m_pMgr->RegisterEventHandle( _T("FairyUpgradeTimes"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&FairyAbilityFrame::OnUpgradeFairyInjTimes)) );//add by xp
	m_pMgr->RegisterEventHandle( _T("FairyUpgradeResTimes"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&FairyAbilityFrame::OnUpgradeFairyResTimes)) );//add by xp
	return TRUE;
}

BOOL FairyAbilityFrame::Destroy()
{
	GameFrame::Destroy();
	if(P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);
	m_pCmdMgr->UnRegister( "NS_FairyStuntBook",	(NETMSGPROC)m_Trunk.sfp2(&FairyAbilityFrame::NetLearnFairyBook) );
	m_pCmdMgr->UnRegister( "NS_FairyStuntConfirm",	(NETMSGPROC)m_Trunk.sfp2(&FairyAbilityFrame::NetLearnFairySkill) );
	m_pCmdMgr->UnRegister( "NS_FairyAddPromoteTimes",	(NETMSGPROC)m_Trunk.sfp2(&FairyAbilityFrame::NetAddFairyPromoteTimes) );//add by xp
	m_pMgr->UnRegisterEventHandler( _T("LearnFairySkill"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&FairyAbilityFrame::OnLearnFairySkill)) );
	m_pMgr->UnRegisterEventHandler( _T("QuitFairySkill"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&FairyAbilityFrame::OnQuitFairySkill)) );
	m_pMgr->UnRegisterEventHandler( _T("UpdateFairyAbility"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&FairyAbilityFrame::OnUpdateFairyAbility)) );
	m_pMgr->UnRegisterEventHandler( _T("FairyUpgradeTimes"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&FairyAbilityFrame::OnUpgradeFairyInjTimes)) );//add by xp
	m_pMgr->UnRegisterEventHandler( _T("FairyUpgradeResTimes"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&FairyAbilityFrame::OnUpgradeFairyResTimes)) );//add by xp
	return TRUE;
}

DWORD FairyAbilityFrame::NetLearnFairySkill( tagNS_FairyStuntConfirm *pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode == E_Success)
	{
		if(pNetCmd->bConfirmOrNot)
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyLearnSkillSucc")]);
		else
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyQuitSkillSucc")]);
	}
	if (pNetCmd->dwErrorCode == E_Fabao_StuntConfirm_SameID)
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SameFairySkill")]);
	UpdateUI();
	return 0;
}

DWORD FairyAbilityFrame::NetLearnFairyBook( tagNS_FairyStuntBook *pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode == E_Success)
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyLearnSkillSucc")]);
	else if(pNetCmd->dwErrorCode == E_Fabao_StuntBook_SameID)
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SameFairySkill")]);
	else if(pNetCmd->dwErrorCode == E_Fabao_StuntBook_LevelLimit )
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FailyLevelCanNotLearnSkill")]);
	UpdateUI();
	return 0;
}

DWORD FairyAbilityFrame::NetAddFairyPromoteTimes( tagNS_FairyAddPromoteTimes *pNetCmd, DWORD )//add by xp
{
	if(pNetCmd->dwErrorCode==E_Success)
	{
		UpdateUI();
	}	
	else if(pNetCmd->dwErrorCode==E_FairyAddPromoteTimes_ToMax)
	{
		UpdateUI();
	}
	else if(pNetCmd->dwErrorCode==E_FairyAddPromoteTimes_LackMoney)
	{
		TCHAR szTmp[X_LONG_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("StallErrorMsg_30"));
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[szTmp]);

	}
	return 0;

}

VOID FairyAbilityFrame::Update()
{
	if(P_VALID(m_pWnd))
		m_pWnd->Align();
}

BOOL FairyAbilityFrame::ReloadUI()
{
	XmlElement e;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\fairy_ability.xml");
	if( !m_pGUI->LoadXml( &e, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &e, "fairy_ability_win\\fairy_ability_pic\\back0\\type\\progressback\\progressbar", "ClassName", "ProgressEx" );
	CHAR szTemp[X_LONG_NAME] = {0};
	for( int i = 0; i < EER_End; i++ )
	{
		_snprintf(szTemp, X_LONG_NAME, "fairy_ability_win\\fairy_ability_pic\\back1\\type%d\\progressback\\progressbar", i);
		m_pGUI->ChangeXml( &e, szTemp, "ClassName", "ProgressEx" );
	}

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &e );
	if(!P_VALID(m_pWnd))
		return FALSE;

	m_pStcUpgradeDamageTimes = (GUIStatic*)m_pWnd->GetChild(_T("fairy_ability_pic\\back0\\word1\\num"));
	m_pStcEleType = (GUIStaticEx*)m_pWnd->GetChild(_T("fairy_ability_pic\\back0\\type\\type"));
	m_pProDamage = (ProgressEx*)m_pWnd->GetChild(_T("fairy_ability_pic\\back0\\type\\progressback\\progressbar"));
	m_pProDamage->SetMaxValue(100.0f);
	m_pProDamage->SetSpeed(10000.0f);
	m_pStcDamgeValue = (GUIStatic*)m_pWnd->GetChild(_T("fairy_ability_pic\\back0\\type\\num"));
	m_pBtnUpgradeDamage = (GUIButton*)m_pWnd->GetChild(_T("fairy_ability_pic\\back0\\type\\upgrade"));
	//元素穿透
	m_pStcElePierceType = (GUIStaticEx*)m_pWnd->GetChild(_T("fairy_ability_pic\\back0\\type1\\type"));//add by xp 
	m_pProPierce = (ProgressEx*)m_pWnd->GetChild(_T("fairy_ability_pic\\back0\\type1\\progressback\\progressbar"));
	m_pProPierce->SetMaxValue(100.0f);
	m_pProPierce->SetSpeed(10000.0f);
	m_pStcPierceValue = (GUIStatic*)m_pWnd->GetChild(_T("fairy_ability_pic\\back0\\type1\\num"));//add by xp
	m_pBtnUpgradePierce = (GUIButton*)m_pWnd->GetChild(_T("fairy_ability_pic\\back0\\type1\\upgrade"));//add by xp

	m_pBtnAddEleDamgeTimes=(GUIButton*)m_pWnd->GetChild(_T("fairy_ability_pic\\back0\\butt"));//元素伤害增加次数按钮 add by xp  
	m_pBtnAddEleDefenceTimes=(GUIButton*)m_pWnd->GetChild(_T("fairy_ability_pic\\back1\\butt"));//元素抵抗增加次数按钮 add by xp


	

	m_pStcUpgradeDefenceTimes = (GUIStatic*)m_pWnd->GetChild(_T("fairy_ability_pic\\back1\\word1\\num"));

	TCHAR szInfo[X_LONG_NAME] = {0};
	for( int i = 0; i < EER_End; i++ )
	{
		_sntprintf(szInfo, X_LONG_NAME, _T("fairy_ability_pic\\back1\\type%d"), i);
		m_pStcElement[i] = (GUIStatic*)m_pWnd->GetChild(szInfo);
		m_pProDefence[i] = (ProgressEx*)m_pStcElement[i]->GetChild(_T("progressback\\progressbar"));
		m_pProDefence[i]->SetMaxValue(100.0f);
		m_pProDefence[i]->SetSpeed(10000.0f);
		m_pStcElementDefenceValue[i] =(GUIStatic*)m_pStcElement[i]->GetChild(_T("num"));
		m_pBtnUpgradeDefence[i] = (GUIButton*)m_pStcElement[i]->GetChild(_T("upgrade"));
	}

	for( int i = 0; i < MaxFairySkillNum; i++ )
	{
		_sntprintf(szInfo, X_LONG_NAME, _T("fairy_ability_pic\\back2\\skill%d\\name"), i);
		m_pStcFairySkill[i] = (GUIStaticEx*)m_pWnd->GetChild(szInfo);
	}
	m_pStcCurSkill = (GUIStaticEx*)m_pWnd->GetChild(_T("fairy_ability_pic\\back2\\skill\\name"));

	m_pBtnLearn = (GUIButton*)m_pWnd->GetChild(_T("fairy_ability_pic\\back2\\surebutton"));
	m_pBtnQuit = (GUIButton*)m_pWnd->GetChild(_T("fairy_ability_pic\\back2\\canclebutton"));
	if(P_VALID(m_pWnd))
		m_pWnd->SetInvisible(TRUE);

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&FairyAbilityFrame::EventHandler) );
	return TRUE;
}

DWORD FairyAbilityFrame::OnLearnFairySkill( tagMsgBoxEvent *pGameEvent )
{
	if( pGameEvent->eResult == MBF_OK )
	{
		tagNC_FairyStuntConfirm m;
		m.bConfirmOrNot = true;
		m_pSession->Send(&m);
	}
	return 0;
}

DWORD FairyAbilityFrame::OnQuitFairySkill( tagMsgBoxEvent *pGameEvent )
{
	if( pGameEvent->eResult == MBF_OK )
	{
		tagNC_FairyStuntConfirm m;
		m.bConfirmOrNot = false;
		m_pSession->Send(&m);
	}
	return 0;
}

DWORD FairyAbilityFrame::OnUpdateFairyAbility( tagGameEvent *pGameEvent )
{
	UpdateUI();
	return 0;
}

DWORD FairyAbilityFrame::OnUpgradeFairyInjTimes( tagMsgBoxEvent *pGameEvent )//add by xp
{
	if( pGameEvent->eResult == MBF_OK )
	{
		tagNC_FairyAddPromoteTimes m;
		m.byType=1;
		//tagNC_FairyStuntConfirm m;
		//m.bConfirmOrNot = true;
		m_pSession->Send(&m);
	}
	return 0;
}//add end

DWORD FairyAbilityFrame::OnUpgradeFairyResTimes( tagMsgBoxEvent *pGameEvent )//add by xp
{
	if( pGameEvent->eResult == MBF_OK )
	{
		tagNC_FairyAddPromoteTimes m;
		m.byType=2;
		m_pSession->Send(&m);
	}
	return 0;
}//add end
//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL FairyAbilityFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pBtnLearn)
			{
				if( P_VALID(GetObj("LearnFairySkill")) )
					KillObj("LearnFairySkill");
				CreateObj( "LearnFairySkill", "MsgBox" );
				TCHAR szTemp[X_LONG_NAME] = {0};
				const tagFairySkill *pSkill = ItemProtoData::Inst()->FindFairySkill(m_dwUnsureSkill);
				if(P_VALID(pSkill))
				{
					_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("LearnFairySkill")], pSkill->szName);
					TObjRef<MsgBox>("LearnFairySkill")->Init( _T(""), szTemp, _T("LearnFairySkill") );
				}
			}
			else if(pWnd == m_pBtnQuit)
			{
				if( P_VALID(GetObj("QuitFairySkill")) )
					KillObj("QuitFairySkill");
				CreateObj( "QuitFairySkill", "MsgBox" );
				TCHAR szTemp[X_LONG_NAME] = {0};
				const tagFairySkill *pSkill = ItemProtoData::Inst()->FindFairySkill(m_dwUnsureSkill);
				if(P_VALID(pSkill))
				{
					_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("QuitFairySkill")], pSkill->szName);
					TObjRef<MsgBox>("QuitFairySkill")->Init( _T(""), szTemp, _T("QuitFairySkill") );
				}
			}
			else if(pWnd==m_pBtnAddEleDamgeTimes)//元素伤害提升次数按钮add by xp
			{
				if( P_VALID(GetObj("FairyUpgradeTimes")) )
					KillObj("FairyUpgradeTimes");
				CreateObj( "FairyUpgradeTimes", "MsgBox" );
				TCHAR szTemp[X_LONG_NAME] = {0};
				const tagFairyUpgradeTimes *pFairyUpgradeTimes = ItemProtoData::Inst()->FindFairyUpgradeTimes(m_dwFairyUpgradeTimes + 1);
				if(P_VALID(pFairyUpgradeTimes))
				{
					_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("FairyUpgradeTimes")], MSilver2DBGold(pFairyUpgradeTimes->n32Money),MSilver2DBSilver(pFairyUpgradeTimes->n32Money));
					TObjRef<MsgBox>("FairyUpgradeTimes")->Init( _T(""), szTemp, _T("FairyUpgradeTimes") );
				}

			}
			else if(pWnd==m_pBtnAddEleDefenceTimes)//元素抵抗提升次数按钮add by xp
			{
				if( P_VALID(GetObj("FairyUpgradeResTimes")) )
					KillObj("FairyUpgradeResTimes");
				CreateObj( "FairyUpgradeResTimes", "MsgBox" );
				TCHAR szTemp[X_LONG_NAME] = {0};
				const tagFairyUpgradeTimes *pFairyUpgradeResTimes = ItemProtoData::Inst()->FindFairyUpgradeTimes(m_dwFairyUpgradeResTimes + 10001);
				if(P_VALID(pFairyUpgradeResTimes))
				{
					_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("FairyUpgradeTimes")], MSilver2DBGold(pFairyUpgradeResTimes->n32Money),MSilver2DBSilver(pFairyUpgradeResTimes->n32Money));
					TObjRef<MsgBox>("FairyUpgradeResTimes")->Init( _T(""), szTemp, _T("FairyUpgradeResTimes") );
				}

			}
			else if( pWnd == m_pBtnUpgradeDamage )
			{
				FairyUpgradeFrame *pFrame = (FairyUpgradeFrame*)(TObjRef<GameFrameMgr>()->GetFrame(_T("FairyUpgrade")));
				if(!P_VALID(pFrame))
					pFrame = (FairyUpgradeFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("FairyUpgrade"), _T("FairyUpgradeFrame"), 0 );
				if(P_VALID(pFrame))
					pFrame->SetInfo(m_byType, m_wDamageTimes, m_nDamageValue, 0);
			}
			else if( pWnd == m_pBtnUpgradePierce )//add by xp 
			{
				FairyUpgradeFrame *pFrame = (FairyUpgradeFrame*)(TObjRef<GameFrameMgr>()->GetFrame(_T("FairyUpgrade")));
				if(!P_VALID(pFrame))
					pFrame = (FairyUpgradeFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("FairyUpgrade"), _T("FairyUpgradeFrame"), 0 );
				if(P_VALID(pFrame))
					pFrame->SetInfo(m_byType, m_wDamageTimes, m_nPierceValue, 1);
			}//add by xp
			else
			{
				for( int i = 0; i < EER_End; i++ )
				{
					if(pWnd == m_pBtnUpgradeDefence[i])
					{
						FairyUpgradeFrame *pFrame = (FairyUpgradeFrame*)(TObjRef<GameFrameMgr>()->GetFrame(_T("FairyUpgrade")));
						if(!P_VALID(pFrame))
							pFrame = (FairyUpgradeFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("FairyUpgrade"), _T("FairyUpgradeFrame"), 0 );
						if(P_VALID(pFrame))
							pFrame->SetInfo((BYTE)i, m_wDefenceTimes, m_nDefenceValue[i],2);
					}
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if( pWnd == m_pStcCurSkill && m_dwUnsureSkill != GT_INVALID )
			{
				const tagFairySkill *pSkill = ItemProtoData::Inst()->FindFairySkill(m_dwUnsureSkill);
				if(P_VALID(pSkill))
					ShowTip(pWnd, pSkill->szDesc);
			}
			else
			{
				for( int i = 0; i < MaxFairySkillNum; i++)
				{
					if(pWnd == m_pStcFairySkill[i] && m_dwFairySkill[i] != GT_INVALID)
					{
						const tagFairySkill *pSkill = ItemProtoData::Inst()->FindFairySkill(m_dwFairySkill[i]);
						if(P_VALID(pSkill))
						{
							ShowTip(pWnd, pSkill->szDesc);
							break;
						}
					}
				}
			}
		}
		break;
	}
	return TRUE;
}

VOID FairyAbilityFrame::ShowUI(bool bShow)
{
	if(P_VALID(m_pWnd))
	{
		m_pWnd->SetInvisible(!bShow);
		if(bShow)
			UpdateUI();
	}
}

VOID FairyAbilityFrame::UpdateUI()
{
	if(m_pWnd->IsInvisible())
		return;
	Equipment *pFairy = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	if(!P_VALID(pFairy))
		return;

	const tagFabaoSpec *pFairySpe = pFairy->GetFaBaoSpe();
	if(!(pFairySpe))
		return;
	if(pFairySpe->n16Stage<60)
	{
		m_pWnd->SetInvisible(TRUE);
		return;
	}
	m_dwFairyUpgradeTimes=pFairySpe->wEleInjPromoteTimes;//add by xp
	m_dwFairyUpgradeResTimes=pFairySpe->wEleResPromoteTimes;//add by xp
	TCHAR szTemp[X_LONG_NAME] = {0};
	_sntprintf(szTemp, X_LONG_NAME, _T("FairyDamageType_%d"), pFairySpe->eEleInjury);
	m_pStcEleType->SetText(g_StrTable[szTemp]);
	
	m_byType = pFairySpe->eEleInjury;
	_sntprintf(szTemp, X_LONG_NAME, _T("%d"), pFairySpe->wDamageUpgradeTimes);
	m_pStcUpgradeDamageTimes->SetText(szTemp);
	m_pStcUpgradeDamageTimes->SetRefresh(TRUE);
	m_wDamageTimes = pFairySpe->wDamageUpgradeTimes;
	SetProsessColor(m_pProDamage, pFairySpe->nEleInjury);
	INT nDamageValue = pFairySpe->nEleInjury%100;
	if(nDamageValue == 0 && pFairySpe->nEleInjury != 0)
		m_pProDamage->SetValue(100.0f);
	else
		m_pProDamage->SetValue(float(nDamageValue));
	_sntprintf(szTemp, X_LONG_NAME, _T("%d"), pFairySpe->nEleInjury);
	m_pStcDamgeValue->SetText(szTemp);
	m_pStcDamgeValue->SetRefresh(TRUE);

	m_nDamageValue = pFairySpe->nEleInjury;
	if( pFairySpe->wDamageUpgradeTimes == 0 )
		m_pBtnUpgradeDamage->SetEnable(false);
	else
		m_pBtnUpgradeDamage->SetEnable(true);
	m_pBtnUpgradeDamage->SetRefresh(TRUE);

	//元素穿透
	_sntprintf(szTemp, X_LONG_NAME, _T("FairyPierceType_%d"), pFairySpe->eEleInjury);//add by xp
	m_pStcElePierceType->SetText(g_StrTable[szTemp]);//add by xp
	SetProsessColor(m_pProPierce, pFairySpe->nElePierce);
	INT nPierceValue = pFairySpe->nElePierce%100;
	if(nPierceValue == 0 && pFairySpe->nElePierce != 0)
		m_pProPierce->SetValue(100.0f);
	else
		m_pProPierce->SetValue(float(nPierceValue));
	

	_sntprintf(szTemp, X_LONG_NAME, _T("%d"), pFairySpe->nElePierce);//add by xp
	m_pStcPierceValue->SetText(szTemp);//add by xp
	m_pStcPierceValue->SetRefresh(TRUE);//add by xp

	m_nPierceValue = pFairySpe->nElePierce;//add by xp
	if( pFairySpe->wDamageUpgradeTimes == 0 )
		m_pBtnUpgradePierce->SetEnable(false);
	else
		m_pBtnUpgradePierce->SetEnable(true);
	m_pBtnUpgradeDamage->SetRefresh(TRUE);//add by xp
	const tagFairyUpgradeTimes *pFairyUpgradeTimes = ItemProtoData::Inst()->FindFairyUpgradeTimes(m_dwFairyUpgradeTimes + 1);//add by xp
	if(P_VALID(pFairyUpgradeTimes))
	{
		m_pBtnAddEleDamgeTimes->SetEnable(true);
	}
	else
	{
		m_pBtnAddEleDamgeTimes->SetEnable(false);
	}//add end
	const tagFairyUpgradeTimes *pFairyUpgradeResTimes = ItemProtoData::Inst()->FindFairyUpgradeTimes(m_dwFairyUpgradeResTimes + 10001);//add by xp
	if(P_VALID(pFairyUpgradeResTimes))
	{
		m_pBtnAddEleDefenceTimes->SetEnable(true);
	}
	else
	{
		m_pBtnAddEleDefenceTimes->SetEnable(false);
	}//add end


	m_wDefenceTimes = pFairySpe->wResistanceUpgradeTimes;
	_sntprintf(szTemp, X_LONG_NAME, _T("%d"), pFairySpe->wResistanceUpgradeTimes);
	m_pStcUpgradeDefenceTimes->SetText(szTemp);
	m_pStcUpgradeDefenceTimes->SetRefresh(TRUE);
	for( int i = 0; i < EER_End; i++ )
	{
		SetProsessColor(m_pProDefence[i], pFairySpe->nEleResistance[i]);
		INT nDefenceValue = pFairySpe->nEleResistance[i]%100;
		if(nDefenceValue == 0 && pFairySpe->nEleResistance[i] != 0)
			m_pProDefence[i]->SetValue(100.0f);
		else
			m_pProDefence[i]->SetValue(float(nDefenceValue));
		_sntprintf(szTemp, X_LONG_NAME, _T("%d"), pFairySpe->nEleResistance[i]);
		m_pStcElementDefenceValue[i]->SetText(szTemp);
		m_pStcElementDefenceValue[i]->SetRefresh(TRUE);
		if( pFairySpe->wResistanceUpgradeTimes == 0 )
			m_pBtnUpgradeDefence[i]->SetEnable(false);
		else
			m_pBtnUpgradeDefence[i]->SetEnable(true);
		m_pBtnUpgradeDefence[i]->SetRefresh(TRUE);
		m_nDefenceValue[i] = pFairySpe->nEleResistance[i];
	}

	for( int i = 0; i < MaxFairySkillNum; i++ )
	{
		m_dwFairySkill[i] = GT_INVALID;
		if( pFairySpe->dwFairySkill[i] == 0 )
			m_pStcFairySkill[i]->SetText(g_StrTable[_T("NoFairySkill")]);
		else if( pFairySpe->dwFairySkill[i] == GT_INVALID )
			m_pStcFairySkill[i]->SetText(g_StrTable[_T("NoFairySkillCao")]);
		else
		{
			const tagFairySkill *pSkill = ItemProtoData::Inst()->FindFairySkill(pFairySpe->dwFairySkill[i]);
			if(P_VALID(pSkill))
			{
				TCHAR szTemp[X_LONG_NAME] ={0};
				if(pSkill->nType == GT_INVALID || pSkill->nType == pFairySpe->eEleInjury)
					_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("UsfulFairySkill")], pSkill->szName);
				else
					_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("UnUsfulFairySkill")], pSkill->szName);
				m_pStcFairySkill[i]->SetText(szTemp);
				m_dwFairySkill[i] = pFairySpe->dwFairySkill[i];
			}
		}
		m_pStcFairySkill[i]->SetRefresh(TRUE);
	}

	if(pFairySpe->dwUnsureSkill == GT_INVALID)
	{
		m_pStcCurSkill->SetText(g_StrTable[_T("NoFairySkill")]);
		m_pBtnLearn->SetEnable(false);
		m_pBtnQuit->SetEnable(false);
		m_dwUnsureSkill = GT_INVALID;
	}
	else
	{
		const tagFairySkill *pSkill = ItemProtoData::Inst()->FindFairySkill(pFairySpe->dwUnsureSkill);
		if(P_VALID(pSkill))
		{
			m_pStcCurSkill->SetText(pSkill->szName);
			m_pBtnLearn->SetEnable(true);
			m_pBtnQuit->SetEnable(true);
			m_dwUnsureSkill = pFairySpe->dwUnsureSkill;
		}
	}
	m_pStcCurSkill->SetRefresh(TRUE);
}

VOID FairyAbilityFrame::SetProsessColor(ProgressEx *pPro, INT nValue)
{
	tstring strTemp;
	if(nValue < 100)
		strTemp = _T("data\\ui\\common\\L_jindu-glass-d.bmp");
	else if(nValue < 200)
		strTemp = _T("data\\ui\\common\\L_jindu-glass-e.bmp");
	else if(nValue < 300)
		strTemp = _T("data\\ui\\common\\L_jindu-glass-b-j.bmp");
	else if(nValue < 400)
		strTemp = _T("data\\ui\\common\\L_jindu-glass-b.bmp");
	else if(nValue < 500)
		strTemp = _T("data\\ui\\common\\L_jindu-glass-c.bmp");
	else
		strTemp = _T("data\\ui\\common\\L_jindu-glass-e-j.bmp");
	if(P_VALID(pPro))
		pPro->SetForePic(strTemp);
}