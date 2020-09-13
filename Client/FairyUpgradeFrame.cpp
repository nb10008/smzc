#include "StdAfx.h"
#include "RoleMgr.h"
#include "ItemMgr.h"
#include "ItemFrame.h"
#include "ItemEvent.h"
#include "IconStatic.h"
#include "ItemProtoData.h"
#include "CombatSysUtil.h"
#include "FairyUpgradeFrame.h"
#include "..\WorldDefine\msg_compose.h"

FairyUpgradeFrame::FairyUpgradeFrame(void):
m_Trunk(this),
m_pWnd(NULL),
m_bProcessing(false),
m_nValue(0),
m_byType(0),
m_wTime(0),
m_nMin(0),
m_nMax(0),
m_n64StoneID(GT_INVALID)
{
	for( int i = 0; i < 3; i++ )
	{
		m_pIconFruit[i] = NULL;
		m_n64FruitID[i] = GT_INVALID;
	}
}

FairyUpgradeFrame::~FairyUpgradeFrame()
{

}

BOOL FairyUpgradeFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{
	if( !GameFrame::Init( pMgr, pFather, dwParam) )
		return FALSE;
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2FairyDamageUpgrade"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyUpgradeFrame::OnEventPutItem));
	m_pCmdMgr->Register("NS_FairyIncEleInjury",									(NETMSGPROC)m_Trunk.sfp2(&FairyUpgradeFrame::NetFairyUpgrade),			_T("NS_FairyIncEleInjury"));
	SetStrategy(EIST_FairyDamageUpgrade);

	return TRUE;
}

BOOL FairyUpgradeFrame::Destroy()
{
	SetStrategy(EIST_ItemPocket);
	GameFrame::Destroy();
	if(P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);
	for( int i = 0; i < 3; i++ )
		LockItem(m_n64FruitID[i], false);
	LockItem(m_n64StoneID, false);

	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2FairyDamageUpgrade"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyUpgradeFrame::OnEventPutItem));
	m_pCmdMgr->UnRegister("NS_FairyIncEleInjury",								(NETMSGPROC)m_Trunk.sfp2(&FairyUpgradeFrame::NetFairyUpgrade));
	if( P_VALID(GetObj("FairyUpgrade")) )
		KillObj("FairyUpgrade");

	return TRUE;
}

DWORD FairyUpgradeFrame::NetFairyUpgrade( tagNS_FairyIncEleInjury *pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode == E_Success)
	{
		UpdateUI();
		TCHAR szTemp[X_LONG_NAME] = {0};
		m_nValue += pNetCmd->nAdded;
		_sntprintf(szTemp, X_LONG_NAME, _T("%d"), m_nValue);
		m_pStcDamageBaseValue->SetText(szTemp);
		if( P_VALID(GetObj("FairyUpgrade")) )
			KillObj("FairyUpgrade");
		CreateObj( "FairyUpgrade", "MsgBoxEx" );
		if( 0==m_bAttType )
		{
			_sntprintf(szTemp ,X_LONG_NAME, _T("FairyDamageType_%d"), m_byType);
		}
		else if(1==m_bAttType)
		{
			_sntprintf(szTemp ,X_LONG_NAME, _T("FairyPierceType_%d"), m_byType);
		}//add by xp
		else if(2==m_bAttType)
			_sntprintf(szTemp ,X_LONG_NAME, _T("FairyDefenceType_%d"), m_byType);
		TCHAR szInfo[X_LONG_NAME] = {0};
		_sntprintf(szInfo ,X_LONG_NAME, _T("%s+%d"), g_StrTable[szTemp],  pNetCmd->nAdded);
		TObjRef<MsgBoxEx>("FairyUpgrade")->Init( _T(""), szInfo );
		tagGameEvent m(_T("UpdateFairyAbility"), NULL);
		m_pMgr->SendEvent(&m);
	}
	return 0;
}

DWORD FairyUpgradeFrame::OnEventPutItem( tagItemPosChangeExEvent* pGameEvent )
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;
	PutItem(pGameEvent->n16IndexSrc);
	return 0;
}

BOOL FairyUpgradeFrame::ReloadUI()
{
	XmlElement e;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\ability_upgrade.xml");
	if( !m_pGUI->LoadXml( &e, "VFS_System", strPath.c_str() ) )
		return FALSE;

	for( int i = 0; i < 3; i++ )
	{
		CHAR szTemp[X_LONG_NAME];
		_snprintf( szTemp, X_LONG_NAME, "ability_upgrade_win\\ability_upgrade_pic\\back0\\item%d\\itemback\\item", i);
		m_pGUI->ChangeXml( &e, szTemp, "ClassName", "IconStatic" );
	}
	m_pGUI->ChangeXml( &e, "ability_upgrade_win\\ability_upgrade_pic\\back1\\itemback\\item", "ClassName", "IconStatic" );
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &e );
	if(!P_VALID(m_pWnd))
		return FALSE;

	m_pStcFairyDamage = (IconStatic*)m_pWnd->GetChild(_T("ability_upgrade_pic\\back0\\damage_type"));
	m_pStcDamageBaseValue = (GUIStatic*)m_pWnd->GetChild(_T("ability_upgrade_pic\\back0\\originalnum"));
	m_pStcAddMin = (GUIStatic*)m_pWnd->GetChild(_T("ability_upgrade_pic\\back0\\plus\\num0"));
	m_pStcAddMax = (GUIStatic*)m_pWnd->GetChild(_T("ability_upgrade_pic\\back0\\plus\\num1"));

	for( int i = 0; i < 3; i++ )
	{
		TCHAR szTemp[X_LONG_NAME] = {0};
		_sntprintf(szTemp, X_LONG_NAME, _T("ability_upgrade_pic\\back0\\item%d\\itemback\\item"), i );
		m_pIconFruit[i] = (IconStatic*)m_pWnd->GetChild(szTemp);
		_sntprintf(szTemp, X_LONG_NAME, _T("ability_upgrade_pic\\back0\\item%d\\itemname"), i );
		m_pStcFruitName[i] = (GUIStatic*)m_pWnd->GetChild(szTemp);
		_sntprintf(szTemp, X_LONG_NAME, _T("ability_upgrade_pic\\back0\\item%d\\itemeffect\\num0"), i );
		m_pStcFruitAddMin[i] = (GUIStatic*)m_pWnd->GetChild(szTemp);
		_sntprintf(szTemp, X_LONG_NAME, _T("ability_upgrade_pic\\back0\\item%d\\itemeffect\\num1"), i );
		m_pStcFruitAddMax[i] = (GUIStatic*)m_pWnd->GetChild(szTemp);
	}

	m_pIconStone = (IconStatic*)m_pWnd->GetChild(_T("ability_upgrade_pic\\back1\\itemback\\item"));
	m_pStcRate = (GUIStatic*)m_pWnd->GetChild(_T("ability_upgrade_pic\\back1\\word3"));
	m_pStcRate->SetText(g_StrTable[_T("Low")]);
	m_pStcUpgradeTime = (GUIStatic*)m_pWnd->GetChild(_T("ability_upgrade_pic\\back1\\num"));

	m_pProTime = (GUIProgress*)m_pWnd->GetChild(_T("ability_upgrade_pic\\progpic\\progress"));

	m_pBtnUpgrade = (GUIButton*)m_pWnd->GetChild(_T("ability_upgrade_pic\\upgrade"));
	m_pBtnUpgrade->SetEnable(false);
	m_pBtnExit = (GUIButton*)m_pWnd->GetChild(_T("ability_upgrade_pic\\cancle"));
	m_pBtnQuit = (GUIButton*)m_pWnd->GetChild(_T("ability_upgrade_pic\\closebutt"));
	// 注册事件处理函数
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&FairyUpgradeFrame::EventHandler) );
	m_pWnd->FlipToTop();
	return TRUE;
}


//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL FairyUpgradeFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBtnExit || pWnd == m_pBtnQuit )
			m_pMgr->AddToDestroyList(this);
		else if( pWnd == m_pBtnUpgrade)
		{
			m_pProTime->SetSpeed(100);
			m_pProTime->SetValue(0, true);
			m_pProTime->SetMaxValue(3.0f);
			m_pProTime->SetRefresh();
			m_bProcessing = true;
			m_pBtnUpgrade->SetEnable(false);
		}
		break;
	case EGUISCB_Drop:
		if( pWnd == m_pIconFruit[0] )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutItem((INT16)LOWORD(pEvent->dwParam2), 0);
		}
		else if( pWnd == m_pIconFruit[1] )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutItem((INT16)LOWORD(pEvent->dwParam2), 1);
		}
		else if( pWnd == m_pIconFruit[2] )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutItem((INT16)LOWORD(pEvent->dwParam2), 2);
		}
		else if(pWnd==m_pIconStone)//add by xp
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutItem((INT16)LOWORD(pEvent->dwParam2), 3);
		}                                                 //add by xp
		break;
	}
	return TRUE;
}

VOID FairyUpgradeFrame::Update()
{
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
			tagNC_FairyIncEleInjury m;
			m.n64Fruit1 = m_n64FruitID[0];
			m.n64Fruit2 = m_n64FruitID[1];
			m.n64Fruit3 = m_n64FruitID[2];
			m.n64VerityOfStone = m_n64StoneID;
			m.byType = m_byType;
			m.bAttType=m_bAttType;
			//m.bDamage = m_bDamage;
			m_pSession->Send(&m);
			m_bProcessing = false;
		}
	}
}
//最后一个参数：0代表元素伤害，1代表元素穿透，2代表元素抵抗
VOID FairyUpgradeFrame::SetInfo( BYTE byType, WORD wTimes, INT nValue, BYTE bAttType/*bool bDamage  = false */ )
{
	m_nValue = nValue;
	m_wTime = wTimes;
	m_byType = byType;
	m_bAttType = bAttType;//add by xp 
	//m_bDamageType=bDamageType;//add by xp

	TCHAR szTemp[X_LONG_NAME] = {0};
	if( 0==bAttType )//设置字体为 元素伤害
		_sntprintf(szTemp ,X_LONG_NAME, _T("FairyDamageType_%d"), m_byType);
	else if(1==bAttType)//设置字体为 元素穿透
		_sntprintf(szTemp ,X_LONG_NAME, _T("FairyPierceType_%d"), m_byType);
	else if(2==bAttType)//设置字体为 元素抵抗
		_sntprintf(szTemp ,X_LONG_NAME, _T("FairyDefenceType_%d"), m_byType);
	m_pStcFairyDamage->SetText(g_StrTable[szTemp]);
	_sntprintf(szTemp ,X_LONG_NAME, _T("%d"), m_nValue);
	m_pStcDamageBaseValue->SetText(szTemp);
	_sntprintf(szTemp ,X_LONG_NAME, _T("%d"), m_wTime);
	m_pStcUpgradeTime->SetText(szTemp);
	m_pStcAddMin->SetText(_T("0"));
	m_pStcAddMax->SetText(_T("0"));
	m_pWnd->FlipToTop();
}

VOID FairyUpgradeFrame::LockItem(INT64 nID, bool bOperable)
{
	Item *pItem = ItemMgr::Inst()->GetPocketItemByID(nID);
	if(P_VALID(pItem))
		pItem->SetOperable(!bOperable);
}

VOID FairyUpgradeFrame::PutItem(INT16 nPos, DWORD dwIndex /* = GT_INVALID */)
{
	if(m_bProcessing)
		return;
	Item *pItem = ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(pItem))
		return;

	const tagItemProto* pItemProto = pItem->GetProto();
	if( !P_VALID(pItemProto) )
		return;

	if(pItemProto->eSpecFunc == EISF_VerityOfStone)
	{
		LockItem(m_n64StoneID, false);
		tstring strIcon = ItemMgr::Inst()->GetItemIconPath(pItemProto->dwTypeID);
		m_pIconStone->SetPic(strIcon);
		m_pStcRate->SetText(g_StrTable[_T("High")]);
		m_n64StoneID = pItem->GetItemId();
		LockItem(m_n64StoneID, true);
	}
	else if(pItemProto->eSpecFunc == EISF_FairyFruit&&dwIndex!=3)//modify by xp 
	{
		if(dwIndex == GT_INVALID)
		{
			INT i = 0;
			for( ; i < 3; i++ )
			{
				if(m_n64FruitID[i] == GT_INVALID)
				{
					dwIndex = i;
					break;
				}
			}
			if( i == 3 )
				dwIndex = 0;
		}

		LockItem(m_n64FruitID[dwIndex], false);
		const tagFairyFruit *pFairyFruit = ItemProtoData::Inst()->FindFairyFruit(pItemProto->dwTypeID);
		TCHAR szTemp[X_SHORT_NAME] = {0};
		if(P_VALID(pFairyFruit))
		{
			tstring strIcon = ItemMgr::Inst()->GetItemIconPath(pItemProto->dwTypeID);
			m_pIconFruit[dwIndex]->SetPic(strIcon);
			m_pStcFruitName[dwIndex]->SetText(pItem->GetDisplay()->szName);
			INT nMin = pFairyFruit->byMin;
			INT nMax = pFairyFruit->byMax;
			if( m_byType == pFairyFruit->byType )
			{
				nMin += pFairyFruit->byValue;
				nMax += pFairyFruit->byValue;
			}
			_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), nMin);
			m_pStcFruitAddMin[dwIndex]->SetText(szTemp);
			_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), nMax);
			m_pStcFruitAddMax[dwIndex]->SetText(szTemp);
			m_n64FruitID[dwIndex] = pItem->GetItemId();
			LockItem(m_n64FruitID[dwIndex], true);
			if(m_wTime != 0)
				m_pBtnUpgrade->SetEnable(true);
		}
		m_nMin = 0;
		m_nMax = 0;
		for( int i = 0; i < 3; i++ )
		{
			Item *pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64FruitID[i]);
			if(P_VALID(pItem))
			{
				const tagFairyFruit *pFairyFruit = ItemProtoData::Inst()->FindFairyFruit(pItem->GetItemTypeID());
				if(P_VALID(pFairyFruit))
				{
					m_nMin += pFairyFruit->byMin;
					m_nMax += pFairyFruit->byMax;
					if( m_byType == pFairyFruit->byType )
					{
						m_nMin += pFairyFruit->byValue;
						m_nMax += pFairyFruit->byValue;
					}
				}
			}
		}
		_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_nMin);
		m_pStcAddMin->SetText(szTemp);
		_sntprintf( szTemp, X_SHORT_NAME, _T("%d"), m_nMax);
		m_pStcAddMax->SetText(szTemp);
	}
	else
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotPutInCurUI")]);
	TCHAR szTemp[X_LONG_NAME] = {0};
}

VOID FairyUpgradeFrame::UpdateUI()
{	
	m_wTime--;
	if(m_wTime<0)
		m_wTime = 0;
	TCHAR szTemp[X_SHORT_NAME] = {0};
	_sntprintf(szTemp, X_SHORT_NAME, _T("%d"), m_wTime);
	m_pStcUpgradeTime->SetText(szTemp);
	m_pStcUpgradeTime->SetRefresh(TRUE);
	for( int i = 0; i < 3; i++ )
	{
		Item *pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64FruitID[i]);
		if(!P_VALID(pItem))
		{
			m_n64FruitID[i] = GT_INVALID;
			m_pIconFruit[i]->SetPic(_T("data\\ui\\common\\l_diban-b.bmp"));
			m_pStcFruitName[i]->SetText(_T(""));
			m_pStcFruitAddMin[i]->SetText(_T(""));
			m_pStcFruitAddMax[i]->SetText(_T(""));
		}
		else if( m_wTime > 0 )
			m_pBtnUpgrade->SetEnable(true);
	}
	Item *pStone = ItemMgr::Inst()->GetPocketItemByID(m_n64StoneID);
	if(!P_VALID(pStone))
	{
		m_n64StoneID = GT_INVALID;
		m_pIconStone->SetPic(_T("data\\ui\\common\\l_diban-b.bmp"));
		m_pStcRate->SetText(g_StrTable[_T("Low")]);
	}
	m_pProTime->SetValue(0.0f);
}

BOOL FairyUpgradeFrame::EscCancel()
{
	if(m_pWnd->IsInvisible() )
		return FALSE;
	else if( m_bProcessing )
	{
		m_bProcessing = false;
		m_pProTime->SetValue(0, true);
		m_pBtnUpgrade->SetEnable(true);
		return TRUE;
	}
	m_pMgr->AddToDestroyList(this);
	return TRUE;
}