#include "StdAfx.h"
#include "EggBrokerFrame.h"
#include "GameClientDefine.h"
#include "ItemProtoData.h"
#include "ItemMgr.h"
#include "..\WorldDefine\msg_eggbroker.h"
#include "EggButton.h"
#include "ToolTipFrame.h"
#include "CombatSysUtil.h"
#include "Container.h"
#include "ToolTipCreator.h"
#include "ItemButton.h"
#include "IconStatic.h"
#include "GUIEffectMgr.h"
#include "ItemProtoData.h"
#include "MallFrame.h"
#include "MallMgr.h"

extern const TCHAR* szBackPic;
const TCHAR *strPic_ChooseGoodsBack = _T("data\\ui\\common\\l_icon.bmp");
extern const TCHAR *szSelectGoods = _T("data\\ui\\Common\\L_icon-l.bmp");
//////////////////////////////////////////////////////////////////////////
// Struct
//////////////////////////////////////////////////////////////////////////

void  tagGoods::clear()
{
	dwItemID = 0;
	m_pIsGoods->RefreshItem();
	m_pChoosePic->SetPic(strPic_ChooseGoodsBack);
}

void tagGoods::RefreshGoods(DWORD dwID,BYTE bQuality)
{
	dwItemID = dwID;
	bQuality =bQuality;
	const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(dwItemID);
	if (P_VALID(pItem))
	{
		m_pIsGoods->RefreshItem(dwItemID,0,pItem->byQuality);
	}
}


void	tagGoods::Choose(bool b)
{
	if (b)
		m_pChoosePic->SetPic(szSelectGoods);
	else
		m_pChoosePic->SetPic(strPic_ChooseGoodsBack);
}

//////////////////////////////////////////////////////////////////////////
// Class
//////////////////////////////////////////////////////////////////////////
EggBrokerFrame::EggBrokerFrame(void):m_Trunk(this)
{
}

EggBrokerFrame::~EggBrokerFrame(void)
{
}
BOOL EggBrokerFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register( "NS_EggBegin",		(NETMSGPROC)m_Trunk.sfp2(&EggBrokerFrame::OnNS_EggBegin),		_T("NS_EggBegin"));
	m_pCmdMgr->Register( "NS_BreakEgg",		(NETMSGPROC)m_Trunk.sfp2(&EggBrokerFrame::OnNS_BreakEgg),		_T("NS_BreakEgg"));
	m_pCmdMgr->Register( "NS_TakeEggTreasure",		(NETMSGPROC)m_Trunk.sfp2(&EggBrokerFrame::OnNS_TakeEggTreasure),		_T("NS_TakeEggTreasure"));
	m_pCmdMgr->Register("NS_ItemAdd",				(NETMSGPROC)m_Trunk.sfp2(&EggBrokerFrame::NetItemAdd),				_T("NS_ItemAdd"));
	m_pCmdMgr->Register("NS_NewItemAdd",			(NETMSGPROC)m_Trunk.sfp2(&EggBrokerFrame::NetNewItemAdd),			_T("NS_NewItemAdd"));


	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_EggBroker_Check"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EggBrokerFrame::OnMsgBox_EggBroker_Check));
	m_pFrameMgr->RegisterEventHandle(_T("MsgBox_EggBroker_EmptyEgg"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EggBrokerFrame::OnMsgBox_EggBroker_EmptyEgg));
	m_nCurrTreasureLayer = GT_INVALID;

	m_LayerData.LoadFromFile();
	
	GenerateHammerNums();
	ClearAllGoods();
	m_bCanSendMsg=true;
	return TRUE;
}
BOOL EggBrokerFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	if(P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}

	if(P_VALID( m_pEasyBuyWnd ) )
	{
		m_pGUI->DestroyWnd(m_pEasyBuyWnd);
		m_pEasyBuyWnd = NULL;		
	}

	if(P_VALID(GetObj("EggBroker_Check")))
		KillObj("EggBroker_Check");

	if(P_VALID(GetObj("EggBroker_EmptyEgg")))
		KillObj("EggBroker_EmptyEgg");

	m_pCmdMgr->UnRegister( "NS_EggBegin",							(NETMSGPROC)m_Trunk.sfp2(&EggBrokerFrame::OnNS_EggBegin) );
	m_pCmdMgr->UnRegister( "NS_BreakEgg",							(NETMSGPROC)m_Trunk.sfp2(&EggBrokerFrame::OnNS_BreakEgg) );
	m_pCmdMgr->UnRegister( "NS_TakeEggTreasure",							(NETMSGPROC)m_Trunk.sfp2(&EggBrokerFrame::OnNS_TakeEggTreasure) );
	m_pCmdMgr->UnRegister("NS_ItemAdd",					(NETMSGPROC)m_Trunk.sfp2(&EggBrokerFrame::NetItemAdd));
	m_pCmdMgr->UnRegister("NS_NewItemAdd",				(NETMSGPROC)m_Trunk.sfp2(&EggBrokerFrame::NetNewItemAdd));

	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_EggBroker_Check"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&EggBrokerFrame::OnMsgBox_EggBroker_Check));
	m_pFrameMgr->UnRegisterEventHandler(_T("MsgBox_EggBroker_EmptyEgg"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&EggBrokerFrame::OnMsgBox_EggBroker_EmptyEgg));

	return bRet;
}
BOOL EggBrokerFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\goldegg.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;
	
	TCHAR buffer[256];
	string sTempName;
	// 第1层
 	for (int i =0; i<=6;i++)
 	{
 		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor0\\egg%dbutt"), i);
 		sTempName = m_pUtil->UnicodeToAnsi(buffer);
 		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "EggButton");
 
 		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor0\\egg%d"), i);
 		sTempName = m_pUtil->UnicodeToAnsi(buffer);
 		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "IconStatic");
 	}
	
	// 第2层
	for (int i =0; i<=5;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor1\\egg%dbutt"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "EggButton");

		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor1\\egg%d"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "IconStatic");
	}
	// 第3层
	for (int i =0; i<=4;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor2\\egg%dbutt"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "EggButton");

		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor2\\egg%d"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "IconStatic");
	}
	// 第4层
	for (int i =0; i<=3;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor3\\egg%dbutt"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "EggButton");

		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor3\\egg%d"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "IconStatic");
	}
	// 第5层
	for (int i =0; i<=2;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor4\\egg%dbutt"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "EggButton");

		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor4\\egg%d"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "IconStatic");
	}
	// 第6层
	for (int i =0; i<=1;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor5\\egg%dbutt"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "EggButton");

		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor5\\egg%d"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "IconStatic");
	}
	// 第7层
	for (int i =0; i<=0;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor6\\egg%dbutt"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "EggButton");

		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\eggback\\floor6\\egg%d"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "IconStatic");
	}
	// Goods
	for (int i =0; i<=6;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\choose%d\\item%d"), i,i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "ItemButton");

		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_win\\goldegg_back\\choose%d"), i);
		sTempName = m_pUtil->UnicodeToAnsi(buffer);
		m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "IconStatic");
	}

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
		return FALSE;
	}

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&EggBrokerFrame::EventHandler));

	// ui
	m_pBnClose = (GUIButton*)m_pWnd->GetChild(_T("goldegg_back\\closebutton"));
	m_pBnTakeTreasure = (GUIButton*)m_pWnd->GetChild(_T("goldegg_back\\getbutton"));
	m_pStCurrHammers = (GUIStatic*)m_pWnd->GetChild(_T("goldegg_back\\totalnum"));
	
	// hammers
	for (int i =0; i<=6;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_back\\numback\\floor%d"), i);
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(buffer);
		if (i == 0)
		{
			m_Hammers[i].m_pStHammer = (GUIStatic*)pTempWud->GetChild(_T("num0"));
			m_Hammers[i].m_pStHammer_colour = (GUIStatic*)pTempWud->GetChild(_T("num0"));
		}
		else
		{	
			m_Hammers[i].m_pStHammer = (GUIStatic*)pTempWud->GetChild(_T("num0"));
			m_Hammers[i].m_pStHammer_colour = (GUIStatic*)pTempWud->GetChild(_T("num1"));
		}
	}

	// goods
	for (int i =0; i<=6;i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_back\\choose%d\\item%d"), i,i);
		m_Goods[i].m_pIsGoods = (ItemButton*)m_pWnd->GetChild(buffer);
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("goldegg_back\\choose%d"), i);
		m_Goods[i].m_pChoosePic = (IconStatic*)m_pWnd->GetChild(buffer);
	}

	// egg
	TCHAR Btbuffer[64];
	TCHAR Stbuffer[64];
	// 第1层
	for (int i =0; i<=6;i++)
	{
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(_T("goldegg_back\\eggback\\floor0"));
		_sntprintf(Btbuffer, sizeof(Btbuffer)/sizeof(TCHAR), _T("egg%dbutt"), i);
		_sntprintf(Stbuffer, sizeof(Stbuffer)/sizeof(TCHAR), _T("egg%d"), i);
		m_Layers[EELN_1].pushback((EggButton*)pTempWud->GetChild(Btbuffer),(IconStatic*)pTempWud->GetChild(Stbuffer));
	}
	m_Layers[EELN_1].init(EELN_1);
	// 第2层
	for (int i =0; i<=5;i++)
	{
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(_T("goldegg_back\\eggback\\floor1"));
		_sntprintf(Btbuffer, sizeof(Btbuffer)/sizeof(TCHAR), _T("egg%dbutt"), i);
		_sntprintf(Stbuffer, sizeof(Stbuffer)/sizeof(TCHAR), _T("egg%d"), i);
		m_Layers[EELN_2].pushback((EggButton*)pTempWud->GetChild(Btbuffer),(IconStatic*)pTempWud->GetChild(Stbuffer));
	}
	m_Layers[EELN_2].init(EELN_2);
	// 第3层
	for (int i =0; i<=4;i++)
	{
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(_T("goldegg_back\\eggback\\floor2"));
		_sntprintf(Btbuffer, sizeof(Btbuffer)/sizeof(TCHAR), _T("egg%dbutt"), i);
		_sntprintf(Stbuffer, sizeof(Stbuffer)/sizeof(TCHAR), _T("egg%d"), i);
		m_Layers[EELN_3].pushback((EggButton*)pTempWud->GetChild(Btbuffer),(IconStatic*)pTempWud->GetChild(Stbuffer));
	}
	m_Layers[EELN_3].init(EELN_3);
	// 第4层
	for (int i =0; i<=3;i++)
	{
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(_T("goldegg_back\\eggback\\floor3"));
		_sntprintf(Btbuffer, sizeof(Btbuffer)/sizeof(TCHAR), _T("egg%dbutt"), i);
		_sntprintf(Stbuffer, sizeof(Stbuffer)/sizeof(TCHAR), _T("egg%d"), i);
		m_Layers[EELN_4].pushback((EggButton*)pTempWud->GetChild(Btbuffer),(IconStatic*)pTempWud->GetChild(Stbuffer));
	}
	m_Layers[EELN_4].init(EELN_4);
	// 第5层
	for (int i =0; i<=2;i++)
	{
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(_T("goldegg_back\\eggback\\floor4"));
		_sntprintf(Btbuffer, sizeof(Btbuffer)/sizeof(TCHAR), _T("egg%dbutt"), i);
		_sntprintf(Stbuffer, sizeof(Stbuffer)/sizeof(TCHAR), _T("egg%d"), i);
		m_Layers[EELN_5].pushback((EggButton*)pTempWud->GetChild(Btbuffer),(IconStatic*)pTempWud->GetChild(Stbuffer));
	}
	m_Layers[EELN_5].init(EELN_5);
	// 第6层
	for (int i =0; i<=1;i++)
	{
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(_T("goldegg_back\\eggback\\floor5"));
		_sntprintf(Btbuffer, sizeof(Btbuffer)/sizeof(TCHAR), _T("egg%dbutt"), i);
		_sntprintf(Stbuffer, sizeof(Stbuffer)/sizeof(TCHAR), _T("egg%d"), i);
		m_Layers[EELN_6].pushback((EggButton*)pTempWud->GetChild(Btbuffer),(IconStatic*)pTempWud->GetChild(Stbuffer));
	}
	m_Layers[EELN_6].init(EELN_6);
	// 第7层
	for (int i =0; i<=0;i++)
	{
		GUIStatic* pTempWud = (GUIStatic*)m_pWnd->GetChild(_T("goldegg_back\\eggback\\floor6"));
		_sntprintf(Btbuffer, sizeof(Btbuffer)/sizeof(TCHAR), _T("egg%dbutt"), i);
		_sntprintf(Stbuffer, sizeof(Stbuffer)/sizeof(TCHAR), _T("egg%d"), i);
		m_Layers[EELN_7].pushback((EggButton*)pTempWud->GetChild(Btbuffer),(IconStatic*)pTempWud->GetChild(Stbuffer));
	}
	m_Layers[EELN_7].init(EELN_7);


	strPath = vEngine::g_strLocalPath + _T("\\ui\\buyhammer.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &element, "buyhammer_win\\pic\\backpic\\item0\\itemback\\item", "ClassName", "ItemButton");

	m_pEasyBuyWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pEasyBuyWnd ) )
	{
		m_pGUI->DestroyWnd(m_pEasyBuyWnd);
		m_pEasyBuyWnd = NULL;		
		return FALSE;
	}

	m_pItHammer = (ItemButton*)m_pEasyBuyWnd->GetChild(_T("pic\\backpic\\item0\\itemback\\item"));
	m_pStPrice = (GUIStatic*)m_pEasyBuyWnd->GetChild(_T("pic\\backpic\\item0\\pricenumnum"));
	m_pStLeft = (GUIStatic*)m_pEasyBuyWnd->GetChild(_T("pic\\backpic\\item0\\word\\num"));
	m_pBnClose_2= (GUIButton*)m_pEasyBuyWnd->GetChild(_T("pic\\close"));
	m_pBnBuy= (GUIButton*)m_pEasyBuyWnd->GetChild(_T("pic\\backpic\\buybutton"));

	m_pGUI->RegisterEventHandler( m_pEasyBuyWnd->GetFullName().c_str(),   m_Trunk.sfp1(&EggBrokerFrame::EventHandler));

	m_pWnd->SetInvisible(TRUE);
	m_pEasyBuyWnd->SetInvisible(TRUE);

	return TRUE;
}
BOOL EggBrokerFrame::EscCancel()
{
	if(m_pWnd->IsInvisible())	return FALSE;

	if(P_VALID(GetObj("EggBroker_Check")))
		KillObj("EggBroker_Check");

	CreateObj("EggBroker_Check", "MsgBox");
	TObjRef<MsgBox>("EggBroker_Check")->Init(_T(""), g_StrTable[_T("MsgBox_EggBroker_Check")], _T("MsgBox_EggBroker_Check"));

	return TRUE;
}

BOOL EggBrokerFrame::EventHandler(tagGUIEvent* pEvent)
{

	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if ( pWnd == m_pBnClose)
			{
				if(P_VALID(GetObj("EggBroker_Check")))
					KillObj("EggBroker_Check");

				CreateObj("EggBroker_Check", "MsgBox");
				TObjRef<MsgBox>("EggBroker_Check")->Init(_T(""), g_StrTable[_T("MsgBox_EggBroker_Check")], _T("MsgBox_EggBroker_Check"));
			}
			else if (pWnd == m_pBnTakeTreasure)
			{
				TakeTreasure();
			}
			else if (pWnd == m_pBnClose_2)
			{
// 				if (m_nCurrEnableLayer ==EELN_Null )
// 				{
// 					ClearAllGoods();
// 					for (int i = EELN_Begin; i <=EELN_END; i++)
// 					{
// 						m_Layers[i].clear();
// 					}
// 					EnableLayer(EELN_1);
// 				}
				m_pEasyBuyWnd->SetInvisible(TRUE);
			}
			else if (pWnd == m_pBnBuy)
			{
				tagMallItemProto* pProItem = MallMgr::Inst()->GetMallItemByTypeid( 4510020 );
				if( P_VALID(pProItem) )		
				{
					MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(pProItem->dwID);
					MallMgr::Inst()->GetMallFrame()->SetCurMenu( MallFrame::EMIT_All );
					MallMgr::Inst()->GetMallFrame()->BuyMallItem();
				}
				
			}
		}
		break;
	case EGUIUD_EggClick:		// 自定义点击金蛋事件
		{
			if (m_bCanSendMsg)
			{
				tagNC_BreakEgg cmd;
				cmd.nLayer = pEvent->dwParam1;
				cmd.nPos = pEvent->dwParam2;
				TObjRef<NetSession>()->Send(&cmd);
			}
			m_bCanSendMsg = false;
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for (int i = EELN_Begin; i <= EELN_END; i++)
			{
				for (vector<EggButton*>::iterator it = m_Layers[i].GetEggButtonList().begin();
					it != m_Layers[i].GetEggButtonList().end();++it)
				{
					if (pWnd == (*it)  && !(*it)->IsInvisible())
					{
						ShowTip(pWnd, GetLayerTips((EEggLayerNum)i).c_str());
						break;
					}
				}
			}

			for (int i = EELN_Begin; i <= EELN_END; i++)
			{
				if (pWnd == m_Goods[i].m_pIsGoods || pWnd == m_Goods[i].m_pChoosePic)
				{
					const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto( m_Goods[i].dwItemID);
					if (P_VALID(pItem))
					{
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem->dwTypeID,pItem->byQuality).c_str());
						break;
					}
				}
			}
		}
		break;
	case EGUIE_DragStart:
		{
			for (int j = EELN_Begin; j <= EELN_END; j++)
			{
				if (pWnd == m_Goods[j].m_pIsGoods)
				{
					for (int i = EELN_Begin; i <= EELN_END; i++)
					{
						if (i == j)
							m_Goods[i].Choose(true);
						else
							m_Goods[i].Choose(false);
					}
					m_nCurrTreasureLayer = j;
					break;
				}
			}
		}
		break;
	}
	return TRUE;
}

VOID EggBrokerFrame::OpenFrame()
{
	m_pWnd->SetInvisible(FALSE);
}
VOID EggBrokerFrame::CloseFrame()
{
	m_pWnd->SetInvisible(TRUE);
}

tstring EggBrokerFrame::GetLayerTips(EEggLayerNum layer)
{
	const tagEggLayerData* data = m_LayerData.GetEggLayerInfo(layer);
	if (P_VALID(data))
	{
		tstringstream ss;
		ss << g_StrTable[_T("EggBreaker_GoodsTips_Begin")] << _T("\\n");
		TCHAR szVer[128];
		TCHAR szQuality[128];
		int i =0;
		for (list<DWORD>::const_iterator it = data->list_Items.begin();
			it != data->list_Items.end(); ++it)
		{
			const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(*it);
			if (P_VALID(pItem))
			{
				_sntprintf(szQuality,128,_T("EggBreaker_GoodsTips_Quality_%d"),pItem->byQuality);
				_sntprintf(szVer,128,g_StrTable[szQuality],pItem->pDisplayInfo->szName);
				tstring strTemp =_T(" ");
				if(++i%2 == 0)
					strTemp = _T("\\n");
				ss << szVer << strTemp;
			}
		}
		return ss.str();
	}
	else
	{
		return _T("");
	}

}

VOID EggBrokerFrame::GenerateHammerNums()
{
	TCHAR buffer[64];
	for (int i = EELN_Begin; i <= EELN_END; i++)
	{
		const tagEggLayerData* data = m_LayerData.GetEggLayerInfo(i);
		if (P_VALID(data))
		{
			if(i ==EELN_1 )
			{
				_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), data->nNormalHammers);
				m_Hammers[i].m_pStHammer->SetText(buffer);
			}
			else
			{
				_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), data->nNormalHammers);
				m_Hammers[i].m_pStHammer->SetText(buffer);
				_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), data->nColourHammers);
				m_Hammers[i].m_pStHammer_colour->SetText(buffer);
			}
		}
	}
	
}

VOID EggBrokerFrame::ClearAllGoods()
{
	for (int i = EELN_Begin; i <= EELN_END; i++)
	{
		m_Goods[i].clear();
	}
	m_nCurrTreasureLayer = GT_INVALID;
}

VOID EggBrokerFrame::TakeTreasure()
{
	if (m_nCurrTreasureLayer == GT_INVALID)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EggBreaker_NotChooseGoods")]);
		return;
	}
	tagNC_TakeEggTreasure cmd;
	cmd.nlayer = m_nCurrTreasureLayer;
	TObjRef<NetSession>()->Send(&cmd);
}

VOID EggBrokerFrame::EnableLayer(EEggLayerNum layer)
{
	for (int i = EELN_1; i <= EELN_END; i++)
	{
		if(i == layer)
			m_Layers[i].SetEnable(true);
		else
			m_Layers[i].SetEnable(false);
	}
	m_nCurrEnableLayer = layer;
}

VOID EggBrokerFrame::RefreshCurrHammers()
{
	int nItemCount = 0;
	ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
	if(!P_VALID(pContainer))		return;
	Item* pItem = pContainer->GetSpecFuncItem(EICT_Bag,EISF_EggHammer);
	if(!P_VALID(pItem))		
	{
		m_pStCurrHammers->SetText(_T("0"));
		m_pStCurrHammers->SetRefresh(true);
		return;
	}

	nItemCount = pContainer->GetItemQuantity(pItem->GetItemTypeID());
	TCHAR buffer[64];
	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), nItemCount);
	m_pStCurrHammers->SetText(buffer);
	m_pStCurrHammers->SetRefresh(true);
}
//////////////////////////////////////////////////////////////////////////  NetCmd

DWORD EggBrokerFrame::OnNS_EggBegin(tagNS_EggBegin* pNetCmd,DWORD)
{
	if (pNetCmd->nCurrLayer == EELN_Begin)
	{
		ClearAllGoods();
		for (int i = EELN_Begin; i <=EELN_END; i++)
		{
			m_Layers[i].clear();
		}
		EnableLayer(EELN_1);
	}
	else		// 恢复数据
	{
		int CurrentLayer = pNetCmd->nCurrLayer;
		for (int i = EELN_Begin; i <=EELN_END; i++)
		{
			if (i >= CurrentLayer )
			{
				m_Layers[i].clear();
			}
			// 标记砸过的蛋
			if (pNetCmd->EggStartInfo[i].bIsColourEgg)
				m_Layers[pNetCmd->EggStartInfo[i].nLayer].SetBrokedEgg((EEggPosNum)pNetCmd->EggStartInfo[i].nPos,false,true);
			else
				m_Layers[pNetCmd->EggStartInfo[i].nLayer].SetBrokedEgg((EEggPosNum)pNetCmd->EggStartInfo[i].nPos,false);
				
			// 标记宝物
			const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(pNetCmd->EggStartInfo[i].dwItemID);
			if (P_VALID(pItem))
			{
				m_Goods[pNetCmd->EggStartInfo[i].nLayer].RefreshGoods(pItem->dwTypeID,pItem->byQuality);
			}
			
			// 标记当前层中的彩蛋
			if (i == CurrentLayer)
			{
				if(pNetCmd->nColourEggPos_1 != GT_INVALID)
					m_Layers[CurrentLayer].SetColourEgg((EEggPosNum)pNetCmd->nColourEggPos_1);
				if(pNetCmd->nColourEggPos_2 != GT_INVALID)
					m_Layers[CurrentLayer].SetColourEgg((EEggPosNum)pNetCmd->nColourEggPos_2);
			}
		}
		EnableLayer((EEggLayerNum)pNetCmd->nCurrLayer);
	}
	RefreshCurrHammers();
	OpenFrame();
	m_bCanSendMsg = true;
	return 0;
}
DWORD EggBrokerFrame::OnNS_BreakEgg(tagNS_BreakEgg* pNetCmd,DWORD)
{
	if (pNetCmd->dwErrorCode == E_Success)
	{
		GUIEffectMgr::Inst()->PlayGuiEffect(_T("txui028"),m_pWnd,GUIEffectMgr::EET_Common);
		// 标记宝物
		const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(pNetCmd->dwItemID);
		if (P_VALID(pItem))
		{
			m_Goods[pNetCmd->nLastEggLayer].RefreshGoods(pItem->dwTypeID,pItem->byQuality);
		}
	
		// 标记砸过的蛋
		if(pNetCmd->bLastEggIsColour)
			m_Layers[pNetCmd->nLastEggLayer].SetBrokedEgg((EEggPosNum)pNetCmd->nLastEggPos,false,true);
		else
			m_Layers[pNetCmd->nLastEggLayer].SetBrokedEgg((EEggPosNum)pNetCmd->nLastEggPos,false);
		// 清除所有蛋标记
		m_Layers[pNetCmd->nNextLayer].clear();

		// 设置彩蛋
		if(pNetCmd->nColourEggPos_1 != GT_INVALID)
			m_Layers[pNetCmd->nNextLayer].SetColourEgg((EEggPosNum)pNetCmd->nColourEggPos_1);
		if(pNetCmd->nColourEggPos_2 != GT_INVALID)
			m_Layers[pNetCmd->nNextLayer].SetColourEgg((EEggPosNum)pNetCmd->nColourEggPos_2);

		// 启动下一行
		EnableLayer((EEggLayerNum)pNetCmd->nNextLayer);
	}
	else if (pNetCmd->dwErrorCode == EEBEC_EmptyEgg)
	{
		GUIEffectMgr::Inst()->PlayGuiEffect(_T("txui029"),m_pWnd,GUIEffectMgr::EET_Common);
		// 标记出空蛋
		m_Layers[pNetCmd->nLastEggLayer].SetBrokedEgg((EEggPosNum)pNetCmd->nLastEggPos,true);

 		if(P_VALID(GetObj("EggBroker_EmptyEgg")))
 			KillObj("EggBroker_EmptyEgg");
 
 		CreateObj("EggBroker_EmptyEgg", "MsgBoxEx");
 		TObjRef<MsgBoxEx>("EggBroker_EmptyEgg")->Init(_T(""), g_StrTable[_T("MsgBox_EggBroker_EmptyEgg")], _T("MsgBox_EggBroker_EmptyEgg"),MBF_OK,TRUE);
	
		EnableLayer(EELN_Null);

	}
	else if (pNetCmd->dwErrorCode == EEBEC_HammerNotEnough)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EEBEC_HammerNotEnough")]);
		FillEasyBuyInfo();
		m_pEasyBuyWnd->SetInvisible(FALSE);
		m_pEasyBuyWnd->FlipToTop();
	}
	else if (pNetCmd->dwErrorCode == EEBEC_TopLayer)
	{
		GUIEffectMgr::Inst()->PlayGuiEffect(_T("txui028"),m_pWnd,GUIEffectMgr::EET_Common);
		// 标记宝物
		const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(pNetCmd->dwItemID);
		if (P_VALID(pItem))
		{
			m_Goods[pNetCmd->nLastEggLayer].RefreshGoods(pItem->dwTypeID,pItem->byQuality);
		}

		// 标记砸过的蛋
		if(pNetCmd->bLastEggIsColour)
			m_Layers[pNetCmd->nLastEggLayer].SetBrokedEgg((EEggPosNum)pNetCmd->nLastEggPos,false,true);
		else
			m_Layers[pNetCmd->nLastEggLayer].SetBrokedEgg((EEggPosNum)pNetCmd->nLastEggPos,false);

		EnableLayer(EELN_Null);
	}
	RefreshCurrHammers();
	m_bCanSendMsg = true;
	return 0;
}
DWORD EggBrokerFrame::OnNS_TakeEggTreasure(tagNS_TakeEggTreasure* pNetCmd,DWORD)
{
	if (E_Success ==  pNetCmd->dwErrorCode)
	{
		ClearAllGoods();
		for (int i = EELN_Begin; i <=EELN_END; i++)
		{
			m_Layers[i].clear();
		}
		EnableLayer(EELN_1);
	}
	else if (EEBEC_ChooseNoItem ==  pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EggBreaker_NotChooseGoods")]);
	}
	else if (EEBEC_BaiBao ==  pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EggBreaker_BaiBao")]);
		ClearAllGoods();
		for (int i = EELN_Begin; i <=EELN_END; i++)
		{
			m_Layers[i].clear();
		}
		EnableLayer(EELN_1);
	}
	m_bCanSendMsg = true;
	return 0;
}

DWORD EggBrokerFrame::NetItemAdd(tagNS_ItemAdd* pNetCmd, DWORD)
{
	RefreshCurrHammers();

	int nItemCount = 0;
	ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
	if(!P_VALID(pContainer))		return 0;
	Item* pItem = pContainer->GetSpecFuncItem(EICT_Bag,EISF_EggHammer);
	if(!P_VALID(pItem))		
	{
		m_pStLeft->SetText(_T("0"));
		m_pStLeft->SetRefresh(true);
	}
	else
	{
		nItemCount = pContainer->GetItemQuantity(pItem->GetItemTypeID());
		TCHAR buffer[64];
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), nItemCount);
		m_pStLeft->SetText(buffer);
		m_pStLeft->SetRefresh(true);
	}
	return 0;
}

DWORD EggBrokerFrame::NetNewItemAdd(tagNS_NewItemAdd* pNetCmd, DWORD)
{
	RefreshCurrHammers();

	int nItemCount = 0;
	ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
	if(!P_VALID(pContainer))		return 0;
	Item* pItem = pContainer->GetSpecFuncItem(EICT_Bag,EISF_EggHammer);
	if(!P_VALID(pItem))		
	{
		m_pStLeft->SetText(_T("0"));
		m_pStLeft->SetRefresh(true);
	}
	else
	{
		nItemCount = pContainer->GetItemQuantity(pItem->GetItemTypeID());
		TCHAR buffer[64];
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), nItemCount);
		m_pStLeft->SetText(buffer);
		m_pStLeft->SetRefresh(true);
	}
	return 0;
}


////////////////////////////////////////////////////////////////////////// Event
DWORD EggBrokerFrame::OnMsgBox_EggBroker_Check(tagMsgBoxEvent* pGameEvent)
{
	if(MBF_OK == pGameEvent->eResult )
	{
		tagNC_FinishEggGame cmd;
		TObjRef<NetSession>()->Send(&cmd);

		CloseFrame();
	}
	return 0;
}

DWORD EggBrokerFrame::OnMsgBox_EggBroker_EmptyEgg(tagMsgBoxEvent* pGameEvent)
{
	if(MBF_OK == pGameEvent->eResult  || MBF_Cancel == pGameEvent->eResult )
	{
		ClearAllGoods();
		for (int i = EELN_Begin; i <=EELN_END; i++)
		{
			m_Layers[i].clear();
		}
		EnableLayer(EELN_1);
	}
	return 0;
}

VOID EggBrokerFrame::FillEasyBuyInfo()
{
	// num
	int nItemCount = 0;
	ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
	if(!P_VALID(pContainer))		return;
	Item* pItem = pContainer->GetSpecFuncItem(EICT_Bag,EISF_EggHammer);
	if(!P_VALID(pItem))		
	{
		m_pStLeft->SetText(_T("0"));
		m_pStLeft->SetRefresh(true);
	}
	else
	{
		nItemCount = pContainer->GetItemQuantity(pItem->GetItemTypeID());
		TCHAR buffer[64];
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), nItemCount);
		m_pStLeft->SetText(buffer);
		m_pStLeft->SetRefresh(true);
	}

	//price
	TCHAR szBuff[X_LONG_NAME] = {0};
	tagMallItemProto* pProItem = MallMgr::Inst()->GetMallItemByTypeid( 4510020 );
	if ( P_VALID(pProItem))
	{
		if( pProItem->nSalePrice!=GT_INVALID && pProItem->nSalePrice!=pProItem->nPrice )
			_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"), pProItem->nSalePrice);
		else 
			_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"), pProItem->nPrice);
		m_pStPrice->SetText(szBuff);

	}
	// itembutton
	m_pItHammer->RefreshItem(4510020,0,5);
}