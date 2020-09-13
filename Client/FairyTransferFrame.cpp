#include "StdAfx.h"
#include "RoleMgr.h"
#include "ItemMgr.h"
#include "ItemFrame.h"
#include "ItemEvent.h"
#include "IconStatic.h"
#include "CurrencyMgr.h"
#include "CombatSysUtil.h"
#include "ItemProtoData.h"
#include "Container.h"
#include "FairyTransferFrame.h"
#include "..\WorldDefine\msg_compose.h"
#include "..\WorldDefine\currency_define.h"

FairyTransferFrame::FairyTransferFrame(void):
m_Trunk(this),
m_pWnd(NULL),
m_pStcFairyName(NULL),
m_bProcessing(false),
m_n64FairyID(GT_INVALID),
m_n64ItemID(GT_INVALID),
m_bHaveBook(FALSE),
m_bHaveMoney(FALSE)
{
	for( int i = 0; i < 6; i++ )
		m_pIconFairyDamageType[i] = NULL;
	m_dwItem[0] = 4400025;
	m_dwItem[1] = 4400026;
	m_dwItem[2] = 4400027;
	m_dwItem[3] = 4400028;
	m_dwItem[4] = 4400029;
	m_dwItem[5] = 4400030;
}

FairyTransferFrame::~FairyTransferFrame()
{

}

BOOL FairyTransferFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{
	if( !GameFrame::Init( pMgr, pFather, dwParam) )
		return FALSE;
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2FairyTransfer"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyTransferFrame::OnEventPutItem));
	m_pCmdMgr->Register("NS_FairyEleInjuryChange",								(NETMSGPROC)m_Trunk.sfp2(&FairyTransferFrame::NetFairyTranfer),			_T("NS_FairyEleInjuryChange"));
	SetStrategy(EIST_FairyDamageTransfer);

	return TRUE;
}

BOOL FairyTransferFrame::Destroy()
{
	SetStrategy(EIST_ItemPocket);
	GameFrame::Destroy();
	if(P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);
	LockItem(m_n64ItemID, false);
	LockItem(m_n64FairyID, false);

	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2FairyTransfer"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyTransferFrame::OnEventPutItem));
	m_pCmdMgr->UnRegister("NS_FairyEleInjuryChange",							(NETMSGPROC)m_Trunk.sfp2(&FairyTransferFrame::NetFairyTranfer));

	return TRUE;
}


DWORD FairyTransferFrame::NetFairyTranfer( tagNS_FairyEleInjuryChange *pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode == E_Success)
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("FairyTransferSucc")]);
	else
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("FairyTransferFail")]);
	m_pMgr->AddToDestroyList(this);
	return 0;
}

DWORD FairyTransferFrame::OnEventPutItem( tagItemPosChangeExEvent* pGameEvent )
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;
	PutFairy(pGameEvent->n16IndexSrc);
	return 0;
}

BOOL FairyTransferFrame::ReloadUI()
{
	XmlElement e;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\fairy_transfer.xml");
	if( !m_pGUI->LoadXml( &e, "VFS_System", strPath.c_str() ) )
		return FALSE;

	for( int i = 0; i < 6; i++ )
	{
		CHAR szTemp[X_LONG_NAME];
		_snprintf( szTemp, X_LONG_NAME, "fairy_transfer_win\\fairy_transfer_pic\\back1\\type%d", i);
		m_pGUI->ChangeXml( &e, szTemp, "ClassName", "IconStatic" );
	}
	m_pGUI->ChangeXml( &e, "fairy_transfer_win\\fairy_transfer_pic\\back1\\type0\\fire", "ClassName", "CGUIButton" );
	m_pGUI->ChangeXml( &e, "fairy_transfer_win\\fairy_transfer_pic\\back1\\type1\\earth", "ClassName", "CGUIButton" );
	m_pGUI->ChangeXml( &e, "fairy_transfer_win\\fairy_transfer_pic\\back1\\type2\\light", "ClassName", "CGUIButton" );
	m_pGUI->ChangeXml( &e, "fairy_transfer_win\\fairy_transfer_pic\\back1\\type3\\dark", "ClassName", "CGUIButton" );
	m_pGUI->ChangeXml( &e, "fairy_transfer_win\\fairy_transfer_pic\\back1\\type4\\wind", "ClassName", "CGUIButton" );
	m_pGUI->ChangeXml( &e, "fairy_transfer_win\\fairy_transfer_pic\\back1\\type5\\water", "ClassName", "CGUIButton" );
	m_pGUI->ChangeXml( &e, "fairy_transfer_win\\fairy_transfer_pic\\back0\\fairy_back\\fairy_pic", "ClassName", "IconStatic" );
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &e );
	if(!P_VALID(m_pWnd))
		return FALSE;

	m_pBtnFairyDamageType[0] = (GUIButton*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type0\\fire"));
	m_pBtnFairyDamageType[1] = (GUIButton*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type5\\water"));
	m_pBtnFairyDamageType[2] = (GUIButton*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type1\\earth"));
	m_pBtnFairyDamageType[3] = (GUIButton*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type4\\wind"));
	m_pBtnFairyDamageType[4] = (GUIButton*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type2\\light"));
	m_pBtnFairyDamageType[5] = (GUIButton*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type3\\dark"));

	m_pIconFairy = (IconStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back0\\fairy_back\\fairy_pic"));
	m_pStcFairyName = (GUIStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back0\\name"));
	m_pStcFairyLevel = (GUIStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back0\\grade"));
	m_pStcFairyDamageType = (GUIStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back0\\type"));

	m_pStcBookName = (GUIStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back2\\itemback\\itemname"));
	m_pStcBookNeedNum = (GUIStaticEx*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back2\\itemback\\num0"));
	m_pStcBookTotalNum = (GUIStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back2\\itemback\\num1"));
	m_pStcBookTotalNum->SetText(_T("1"));

	m_pStcGold = (GUIStaticEx*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back2\\moneyback\\goldnum"));
	m_pStcSilver = (GUIStaticEx*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back2\\moneyback\\silvernum"));

	m_pProTime = (GUIProgress*)m_pWnd->GetChild(_T("fairy_transfer_pic\\progpic\\progress"));


	m_pIconFairyDamageType[0] = (IconStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type0"));
	m_pIconFairyDamageType[1] = (IconStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type5"));
	m_pIconFairyDamageType[2] = (IconStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type1"));
	m_pIconFairyDamageType[3] = (IconStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type4"));
	m_pIconFairyDamageType[4] = (IconStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type2"));
	m_pIconFairyDamageType[5] = (IconStatic*)m_pWnd->GetChild(_T("fairy_transfer_pic\\back1\\type3"));
	for( int i = 0; i < 6; i++ )
		m_pIconFairyDamageType[i]->SetPic(_T("data\\ui\\common\\l_icon.bmp"));

	m_pBtnTranfer = (GUIButton*)m_pWnd->GetChild(_T("fairy_transfer_pic\\startbutton"));
	m_pBtnTranfer->SetEnable(false);
	m_pBtnExit = (GUIButton*)m_pWnd->GetChild(_T("fairy_transfer_pic\\leavebutton"));
	m_pBtnQuit = (GUIButton*)m_pWnd->GetChild(_T("fairy_transfer_pic\\closebutt"));
	// 注册事件处理函数
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&FairyTransferFrame::EventHandler) );
	return TRUE;
}


//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL FairyTransferFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBtnExit || pWnd == m_pBtnQuit )
			m_pMgr->AddToDestroyList(this);
		else if( pWnd == m_pBtnTranfer )
		{
			m_pProTime->SetSpeed(100);
			m_pProTime->SetValue(0, true);
			m_pProTime->SetMaxValue(3.0f);
			m_pProTime->SetRefresh();
			m_bProcessing = true;
			m_pBtnTranfer->SetEnable(false);
		}
		else
		{
			for(size_t i = 0; i < 6; i++)
			{
				m_pIconFairyDamageType[i]->SetPic(_T("data\\ui\\common\\l_icon.bmp"));
				if(pWnd == m_pBtnFairyDamageType[i])
				{
					m_pIconFairyDamageType[i]->SetPic(_T("data\\ui\\Common\\L_icon-l.bmp"));
					m_byDamageType = (BYTE)i;
					SetBook();
				}
			}
		}
		break;
	case EGUISCB_Drop:
		if( pWnd == m_pIconFairy )
		{
			if( EDT_Item == (EDragType)pEvent->dwParam1 )
				PutFairy((INT16)LOWORD(pEvent->dwParam2));
		}
		break;
	}
	return TRUE;
}

VOID FairyTransferFrame::Update()
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
			 tagNC_FairyEleInjuryChange m;
			 m.n64FairyID = m_n64FairyID;
			 m.n64ItemID = m_n64ItemID;
			 m.dwNPCID = m_dwNPCID;
			 m.eEleInjuryType = (EElementInjury)m_byDamageType;
			 m_pSession->Send(&m);
			 m_bProcessing = false;
		}
	}
}

VOID FairyTransferFrame::PutFairy(INT16 nPos)
{
	ClearInfo();
	Item *pItem = (Item*)ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(pItem))
		return;

	const tagItemProto *pFairyProto = pItem->GetProto();
	if(!P_VALID(pFairyProto) || pFairyProto->eType != EIT_FaBao)
		return;
	Equipment* pFairy = (Equipment*)pItem;
	const tagFabaoSpec *pFairySpe = pFairy->GetFaBaoSpe();
	if(!P_VALID(pFairySpe))
		return;
	if( pFairySpe->n16Stage < 60 )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotPutFairyToFrame")]);
		return;
	}

	LockItem(m_n64FairyID, false);
	tstring strIcon = ItemMgr::Inst()->GetItemIconPath(pFairyProto->dwTypeID);
	m_pIconFairy->SetPic(strIcon);

	tstring strName = pFairyProto->pDisplayInfo->szName;
	m_pStcFairyName->SetText(strName.c_str());

	TCHAR szTemp[X_LONG_NAME] = {0};
	_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("Role_Level")], pFairySpe->n16Stage );
	m_pStcFairyLevel->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("FairyElement_%d"), pFairySpe->eEleInjury );
	m_pStcFairyDamageType->SetText(g_StrTable[szTemp]);
	m_n64FairyID = pFairy->GetItemId();
	LockItem(m_n64FairyID, true);
	INT64 n64NeedMoney = 400000*(pFairySpe->n16Stage - 60)*(pFairySpe->n16Stage - 60)/500;
	INT64 n64MyMoney = CurrencyMgr::Inst()->GetBagSilver();
	TCHAR szTmp[X_LONG_NAME] = {0};
	if( n64NeedMoney > n64MyMoney )
	{
		m_bHaveMoney = FALSE;
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ConsolidateMoneyError")]);
		_stprintf(szTmp, _T("<color=0xFFFF0000>%d"), MSilver2DBGold(n64NeedMoney));
		m_pStcGold->SetText(szTmp);
		_stprintf(szTmp, _T("<color=0xFFFF0000>%d"), MSilver2DBSilver(n64NeedMoney));
		m_pStcSilver->SetText(szTmp);
	}
	else
	{
		m_bHaveMoney = TRUE;
		_stprintf(szTmp, _T("%d"), MSilver2DBGold(n64NeedMoney));
		m_pStcGold->SetText(szTmp);
		_stprintf(szTmp, _T("%d"), MSilver2DBSilver(n64NeedMoney));
		m_pStcSilver->SetText(szTmp);
	}
	for( int i = 0; i < 6; i++ )
	{
		m_pIconFairyDamageType[i]->SetUnClickable(FALSE);
		if( i == pFairySpe->eEleInjury )
			m_pIconFairyDamageType[i]->SetUnClickable(TRUE);
	}
	if( m_bHaveBook && m_bHaveMoney )
		m_pBtnTranfer->SetEnable(TRUE);
}

void FairyTransferFrame::LockItem( INT64 nID, bool bOperable )
{
	Item *pItem = ItemMgr::Inst()->GetPocketItemByID(nID);
	if(P_VALID(pItem))
		pItem->SetOperable(!bOperable);
}

VOID FairyTransferFrame::SetBook()
{
	if(m_bProcessing)
		return;
	LockItem(m_n64ItemID,false);
	tstring strName =  ItemProtoData::Inst()->FindItemDisplay(m_dwItem[m_byDamageType])->szName;
	m_pStcBookName->SetText(strName.c_str());
	INT nNum = ItemMgr::Inst()->GetPocket()->GetItemQuantity(m_dwItem[m_byDamageType]);
	TCHAR szTemp[X_LONG_NAME] = {0};
	if( nNum == 0 )
	{
		_sntprintf(szTemp, X_LONG_NAME, _T("<color=0xFFFF0000>%d"), nNum);
		TCHAR szTips[X_LONG_NAME] = {0};
		_sntprintf(szTips, X_LONG_NAME, g_StrTable[_T("BagNoBook")], strName.c_str());
		CombatSysUtil::Inst()->ShowScreenCenterMsg(szTips);
	}
	else
	{
		m_bHaveBook = TRUE;
		_sntprintf(szTemp, X_LONG_NAME, _T("%d"), nNum);
	}
	m_pStcBookNeedNum->SetText(szTemp);
	Item *pItem = ItemMgr::Inst()->GetPocketFirstItemByTypeID(m_dwItem[m_byDamageType]);
	if(!P_VALID(pItem))
		return;
	m_n64ItemID = pItem->GetData()->n64Serial;
	LockItem(m_n64ItemID, true);
	if( m_bHaveBook && m_bHaveMoney )
		m_pBtnTranfer->SetEnable(TRUE);
}

VOID FairyTransferFrame::ClearInfo()
{
	m_pBtnTranfer->SetEnable(false);
	m_bHaveMoney = FALSE;
	m_bHaveBook = FALSE;
	for(size_t i = 0; i < 6; i++)
		m_pIconFairyDamageType[i]->SetPic(_T("data\\ui\\common\\l_icon.bmp"));
}

BOOL FairyTransferFrame::EscCancel()
{
	if(m_pWnd->IsInvisible() )
		return FALSE;
	else if( m_bProcessing )
	{
		m_bProcessing = false;
		m_pProTime->SetValue(0, true);
		m_pBtnTranfer->SetEnable(true);
		return TRUE;
	}
	m_pMgr->AddToDestroyList(this);
	return TRUE;
}