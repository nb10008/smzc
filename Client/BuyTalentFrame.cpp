#include "StdAfx.h"
#include "BuyTalentFrame.h"
#include "SkillMgr.h"
#include "RoleMgr.h"
#include "CurrencyMgr.h"
#include "CombatSysUtil.h"
#include "LocalPlayer.h"
#include "BuyTalentData.h"
#include "LevelUpData.h"
#include "BuyTalentData.h"
#include "..\WorldDefine\msg_talent.h"

BuyTalentFrame::BuyTalentFrame(void): m_Trunk(this),
m_pWnd(	NULL ),
m_pBtnOk( NULL ),
m_pBtnCancel( NULL ),
m_pBtnClose( NULL ),
m_pStcMaxPoint( NULL ),
m_pStcCanBuyPoint( NULL ),
m_pStcGold( NULL ),
m_pStcSilver( NULL ),
m_pStcHaveGold( NULL ),
m_pStcHaveSilver( NULL ),
m_nWillCostGold( NULL ),
m_nWillCostSilver( NULL )
{
	m_nWillCostGold = 0;
	m_nWillCostSilver = 0;
}

BuyTalentFrame::~BuyTalentFrame(void)
{
}

BOOL BuyTalentFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init( pMgr, pFather, dwParam );
	
	m_pCmdMgr->Register("NS_RolePracticeTalentPoint",	(NETMSGPROC)m_Trunk.sfp2(&BuyTalentFrame::OnNS_RolePracticeTalentPoint), _T("NS_RolePracticeTalentPoint") );
	return bRet;
}

BOOL BuyTalentFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pCmdMgr->UnRegister("NS_RolePracticeTalentPoint",	(NETMSGPROC)m_Trunk.sfp2(&BuyTalentFrame::OnNS_RolePracticeTalentPoint) );

	if ( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	return bRet;
}

BOOL BuyTalentFrame::EscCancel()
{
	if( m_pWnd->IsInvisible() )
		return FALSE;

	m_pWnd->SetInvisible( TRUE );
	m_pMgr->AddToDestroyList( this );
	return TRUE;
}

BOOL BuyTalentFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\skill_shopping.xml");
	m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() );

 	m_pGUI->ChangeXml( &ele, "skill_shopping_win\\skill_shopping_pic\\textback\\num0", "ClassName", "CGUIStaticEx" );
 	m_pGUI->ChangeXml( &ele, "skill_shopping_win\\skill_shopping_pic\\textback\\num1", "ClassName", "CGUIStaticEx" );
 	m_pGUI->ChangeXml( &ele, "skill_shopping_win\\skill_shopping_pic\\num1",	   "ClassName", "CGUIStaticEx" );
 	m_pGUI->ChangeXml( &ele, "skill_shopping_win\\skill_shopping_pic\\num1000",	   "ClassName", "CGUIStaticEx" );
 	m_pGUI->ChangeXml( &ele, "skill_shopping_win\\skill_shopping_pic\\num100",	   "ClassName", "CGUIStaticEx" );
	m_pGUI->ChangeXml( &ele, "skill_shopping_win\\skill_shopping_pic\\num10",	   "ClassName", "CGUIStaticEx" );

	// 创建
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &ele );

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&BuyTalentFrame::EventHandler) );

	m_pBtnOk =		(GUIButton*)m_pWnd->GetChild( _T("skill_shopping_pic\\pay") );
	m_pBtnCancel =	(GUIButton*)m_pWnd->GetChild( _T("skill_shopping_pic\\cancel") );
	m_pBtnClose =	(GUIButton*)m_pWnd->GetChild( _T("skill_shopping_pic\\closebutt") );
	m_pStcMaxPoint =	(GUIStaticEx*)m_pWnd->GetChild( _T("skill_shopping_pic\\textback\\num0") );
	m_pStcCanBuyPoint =	(GUIStaticEx*)m_pWnd->GetChild( _T("skill_shopping_pic\\textback\\num1") );
	m_pStcGold =		(GUIStaticEx*)m_pWnd->GetChild( _T("skill_shopping_pic\\num1") );
	m_pStcSilver =		(GUIStaticEx*)m_pWnd->GetChild( _T("skill_shopping_pic\\num1000") );
	m_pStcHaveGold =	(GUIStaticEx*)m_pWnd->GetChild( _T("skill_shopping_pic\\num100") );
	m_pStcHaveSilver =  (GUIStaticEx*)m_pWnd->GetChild( _T("skill_shopping_pic\\num10") );

	m_pWnd->SetInvisible( TRUE );
	return TRUE;
}

DWORD BuyTalentFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return 0;

	switch ( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if ( pWnd == m_pBtnClose )
			{
				m_pMgr->AddToDestroyList( this );
			}
			else if( pWnd == m_pBtnCancel )
			{
				m_pMgr->AddToDestroyList( this );
			}
			else if( pWnd == m_pBtnOk )
			{
				OnBuyTalent();
			}
		}
		break;
	default :
		break;
	}

	return 0;
}

VOID BuyTalentFrame::ShowWnd( BOOL bShow )
{
	if( bShow )
	{
		RefreshDisplay();
	}

	m_pWnd->SetInvisible( !bShow );
	m_pWnd->SetRefresh( TRUE );
}

void BuyTalentFrame::RefreshDisplay()
{
	TCHAR szNum[X_SHORT_NAME] = {0};
	
	//刷新当前级别可得到的天资上限
	INT nMaxTalentPoint = SkillMgr::Inst()->GetTalentLimit( RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel() );
	m_pStcMaxPoint->SetText( _itot( nMaxTalentPoint, szNum, 10 ) ); 
	m_pStcMaxPoint->SetRefresh( TRUE );

	//刷新当前可购买的天资点数
	INT nCanBuyPoint = SkillMgr::Inst()->GetExtraTalentPointNum();
	m_pStcCanBuyPoint->SetText( _itot(nCanBuyPoint, szNum, 10 ) );
	m_pStcCanBuyPoint->SetRefresh( TRUE );
	
	//刷新当前背包里的金
	INT64 n64Money = CurrencyMgr::Inst()->GetBagSilver();
	INT64 n64Gold = n64Money / 10000;
	m_pStcHaveGold->SetText( _i64tot( n64Gold, szNum, 10 ) );
	m_pStcHaveGold->SetRefresh( TRUE );

	//刷新当前背包里的银
	INT64 n64Silver = n64Money % 10000;
	m_pStcHaveSilver->SetText( _i64tot( n64Silver, szNum, 10 ) );
	m_pStcHaveSilver->SetRefresh( TRUE );

	//刷新购买当前点数要花费的钱
	INT64 nCost = CalculateCost( 1 );
	
	if( nCanBuyPoint > 0 )
	{
		m_nWillCostGold = nCost / 10000;
		m_nWillCostSilver = nCost % 10000;
		if( m_nWillCostGold < 0 )
			m_nWillCostGold = 0;
		if( m_nWillCostSilver < 0 )
			m_nWillCostSilver = 0;
	}
	else
	{
		m_nWillCostGold = 0;
		m_nWillCostSilver = 0;
	}

	m_pStcGold->SetText( _i64tot( m_nWillCostGold, szNum, 10 ) );
	m_pStcGold->SetRefresh( TRUE );
	m_pStcSilver->SetText( _i64tot( m_nWillCostSilver, szNum, 10 ) );
	m_pStcSilver->SetRefresh( TRUE );
}

DWORD BuyTalentFrame::OnBuyTalent()
{
	INT64 nNeedSilver = m_nWillCostGold * 10000 + m_nWillCostSilver;
	if( nNeedSilver > CurrencyMgr::Inst()->GetBagSilver() )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("BuyTalent_LackOfMoney")] );
		return 0;
	}

	INT nMaxPoint = SkillMgr::Inst()->GetTalentLimit( RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel() );
	INT nCurPoint = SkillMgr::Inst()->GetTotalBidden() + RoleMgr::Inst()->GetLocalPlayer()->GetAttribute( ERA_TalentPoint );
	if( nCurPoint >= nMaxPoint )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("BuyTalent_RequestPointTooMuch")] );
		return 0;
	}

	tagNC_RolePracticeTalentPoint cmd;
	cmd.nTalentPointAdd = 1;
	m_pSession->Send( &cmd );

	return 0;
}

INT64 BuyTalentFrame::CalculateCost( DWORD dwTalentPointNum )
{
	INT nCurLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
	const tagExperience* pExp = LevelUpData::Inst()->FindExpData( nCurLevel );
	
	//当前级别的天资上限
	INT nLimit = pExp->nTalentAvail_limit;

	//当前级别通过升级得到的天资点数总和
	INT nLevelPoint = 0;
	for( int i=1; i<= nCurLevel; i++ )
	{
		pExp = LevelUpData::Inst()->FindExpData( i );
		nLevelPoint += pExp->nTalentAvail;
	}

	//当前拥有的天资点
	INT nOwnPoint = SkillMgr::Inst()->GetTotalBidden() + RoleMgr::Inst()->GetLocalPlayer()->GetAttribute( ERA_TalentPoint );

	//已经购买了 nBuyAlredy 点天资
	INT nBuyAlredy = nOwnPoint - nLevelPoint;

	//计算购买 dwTalentPointNum 天资的花费
	INT64 n64Cost = 0;
	for( DWORD i = 1; i<= dwTalentPointNum; i++ )
	{
		//购买第 nBuyAlredy + i 点天资需要的游戏币
		n64Cost += BuyTalentData::Inst()->FindCost( nBuyAlredy + i );
	}

	return n64Cost;
}

DWORD BuyTalentFrame::OnNS_RolePracticeTalentPoint( tagNS_RolePracticeTalentPoint* pNetCmd, DWORD dwParam )
{
	RefreshDisplay();
	switch( pNetCmd->dwErrorCode )
	{
	case ERPT_Success:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("BuyTalent_Success")] );
		}
		break;
	case ERPT_NoEnoughMonney:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("BuyTalent_LackOfMoney")] );
		}
		break;
	case ERPT_OverStepMaxTalent:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("BuyTalent_RequestPointTooMuch")] );
		}
		break;
	default:
		break;
	}

	return 0;
}