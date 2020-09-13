#include "StdAfx.h"
#include "CombatFrame_DeadMenu.h"
#include "CombatEvent.h"
#include "CombatSysUtil.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "ItemMgr.h"
#include "..\WorldDefine\specfunc_define.h"
#include "..\WorldDefine\mall_define.h"
#include "..\WorldDefine\pk_define.h"
#include "MallMgr.h"
#include "ItemProtoData.h"
#include "ItemButton.h"
#include "MallFrame.h"
#include "IconStatic.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "Item.h"                                      //添加了两个头文件
#include "Container.h"                                 //
#include "GuildMgr.h"
#include "MapMgr.h"
#include "ItemEvent.h"
#include "ClientConfigInCpk.h"

const DWORD ITEM_SCAPEGOAT_TYPEID0 = 3070021;
const DWORD ITEM_SCAPEGOAT_TYPEID1 = 3070020;
CombatFrame_DeadMenu::CombatFrame_DeadMenu(void)
: m_Trunk(this)
, m_pWnd(0)
, m_pBtnLocalRise(0)
, m_pBtnAcceptRise(0)
, m_pBtnBackRise(0)
, m_pStcHint(0)
,m_pRebornBack(NULL)
,m_pReborn(NULL)
,m_pStcMoney(NULL)
, m_bUpdateTime(false)
, m_nReviveTime(GT_INVALID)
, m_pBtnPerfectRise(0)
, m_bRefresh(FALSE)
, m_nReviewWite(0)
, m_bIsWar(false)
{
}

CombatFrame_DeadMenu::~CombatFrame_DeadMenu(void)
{
	
}

BOOL CombatFrame_DeadMenu::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	// 复活确认
	m_pMgr->RegisterEventHandle( _T("OnPvPDeadMenuConfirm"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&CombatFrame_DeadMenu::OnPvPDeadMenuConfirmMsgBox));

	//--注册网络消息处理函数
	m_pCmdMgr->Register("NS_RoleRevive", (NETMSGPROC)m_Trunk.sfp2(&CombatFrame_DeadMenu::NetRecvNS_RoleRevive),
		_T("NS_RoleRevive"));
	m_pCmdMgr->Register("NS_RoleReviveNotify", (NETMSGPROC)m_Trunk.sfp2(&CombatFrame_DeadMenu::NetRecvNS_RoleReviveNotify), 
		_T("NS_RoleReviveNotify"));
	m_pCmdMgr->Register("NS_CityStruggleRevive", (NETMSGPROC)(m_Trunk.sfp2(&CombatFrame_DeadMenu::NetRecvCityStruggleRevive)), 
		_T("NS_CityStruggleRevive"));
	//游戏事件
	m_pMgr->RegisterEventHandle(_T("Refresh_Deadmenu"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&CombatFrame_DeadMenu::RefreshDeadMenu));
	m_pMgr->RegisterEventHandle(_T("ItemEvent_UpdateItem"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&CombatFrame_DeadMenu::OnUpdateLPItem));

	if (!ClientConfigInCpk::Inst()->IsMallOpen())
	{
		m_pBtnRebornBuy->SetEnable(FALSE);
		m_pBtnScapegoatBuy->SetEnable(FALSE);
	}
	return TRUE;
}

BOOL CombatFrame_DeadMenu::Destroy()
{
	GameFrame::Destroy();

	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);

	m_pMgr->UnRegisterEventHandler( _T("OnPvPDeadMenuConfirm"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&CombatFrame_DeadMenu::OnPvPDeadMenuConfirmMsgBox));
	m_pMgr->UnRegisterEventHandler(_T("Refresh_Deadmenu"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&CombatFrame_DeadMenu::RefreshDeadMenu));
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_UpdateItem"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&CombatFrame_DeadMenu::OnUpdateLPItem));

	//--注册网络消息处理函数
	m_pCmdMgr->UnRegister("NS_RoleRevive", (NETMSGPROC)m_Trunk.sfp2(&CombatFrame_DeadMenu::NetRecvNS_RoleRevive));
	m_pCmdMgr->UnRegister("NS_RoleReviveNotify", (NETMSGPROC)m_Trunk.sfp2(&CombatFrame_DeadMenu::NetRecvNS_RoleReviveNotify));
	m_pCmdMgr->UnRegister("NS_CityStruggleRevive", (NETMSGPROC)(m_Trunk.sfp2(&CombatFrame_DeadMenu::NetRecvCityStruggleRevive)));

	return 0;
}

VOID CombatFrame_DeadMenu::Update()
{
	if (m_bUpdateTime)
	{
		m_nReviveTime -= static_cast<int>(Cool3D::Kernel::Inst()->GetDeltaTimeDW());
		if (m_nReviveTime >= 0)
		{
			TCHAR szBuffer[X_LONG_NAME] = {0};
			_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("Revive_CityWar")], m_nReviveTime/1000);
			m_pStcHint->SetText(szBuffer);
			m_pWnd->SetRefresh(TRUE);
		}
		else
			m_bUpdateTime = false;
	}
	if ( m_bRefresh == TRUE )
	{
		ShowTool();
		m_bRefresh = FALSE;
	}

	//新复活时间
	if(m_bIsWar)
	{
		if(m_nReviewWite>0)
		{
			m_nReviewWite -= static_cast<int>(Cool3D::Kernel::Inst()->GetDeltaTimeDW());
			TCHAR szBuffer[X_LONG_NAME] = {0};
			_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("Revive_CityWar")], m_nReviewWite/1000);
			m_pStcHint->SetText(szBuffer);
			m_pWnd->SetRefresh(TRUE);
		}
		else
		{
			m_pBtnBackRise->SetEnable(TRUE);
		}
	}

}

DWORD CombatFrame_DeadMenu::NetRecvNS_RoleRevive( tagNS_RoleRevive* pMsg, DWORD dwParam )
{
	DWORD dwLocalID = RoleMgr::Inst()->GetLocalPlayerID();

	if(pMsg->dwRoleID == dwLocalID)
	{
		switch(pMsg->dwErrorCode)
		{
		case E_Success:
			m_pWnd->SetInvisible(TRUE);
			m_pFrameMgr->AddToDestroyList(this);
			break;
		case E_Revive_NotDead:
			//m_pStcHint->SetText(g_StrTable[_T("Revive_NotDead")]);      //注释掉的原因是将提示信息改为中央屏显提示
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Revive_NotDead")]);
			break;
		case E_Revive_CanNotTransmit:
			//m_pStcHint->SetText(g_StrTable[_T("Revive_CanNotTransmit")]);
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Revive_CanNotTransmit")]);
			break;
		case E_Revive_ItemLimit:
			//m_pStcHint->SetText(g_StrTable[_T("Revive_ItemLimit")]);
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Revive_ItemLimit")]);
			break;
		case E_Revive_CanNotAccept:
			//m_pStcHint->SetText(g_StrTable[_T("Revive_CanNotAccept")]);
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Revive_CanNotAccept")]);
			break;
		case E_Revive_CanNotReviveOnCityStruggle:
			m_pStcHint->SetText( g_StrTable[_T("Revive_CanNotReviveOnCityStruggle")]);
			break;
		case E_Revive_CanNotReviveOnContestMap:
			m_pStcHint->SetText( g_StrTable[_T("Revive_CanNotReviveOnContestMap")]);
			break;
		case E_Revive_GodMiracleNoReviveLocusOrPerfect:
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Revive_CanNotReviveOnGuildBattle")]);
			break;
		default: break;
		}
		m_pWnd->SetRefresh(TRUE);
	}
	return 0;
}

DWORD CombatFrame_DeadMenu::NetRecvNS_RoleReviveNotify( tagNS_RoleReviveNotify* pMsg, DWORD dwParam )
{
	if(P_VALID(m_pBtnAcceptRise) && MapMgr::Inst()->GetCurMapID() != 2962677420)
	{
		m_pBtnAcceptRise->SetEnable(true);
		m_pWnd->SetRefresh(TRUE);
	}
	return 0;
}

DWORD CombatFrame_DeadMenu::NetRecvCityStruggleRevive(tagNS_CityStruggleRevive *pMsg, DWORD dwParam)
{
	m_nReviveTime = pMsg->nSec * 1000;
	m_bUpdateTime = true;
	TCHAR szBuffer[X_LONG_NAME] = {0};
	_sntprintf(szBuffer, X_LONG_NAME, g_StrTable[_T("Revive_CityWar")], pMsg->nSec);
	m_pStcHint->SetText(szBuffer);
	m_pWnd->SetRefresh(TRUE);
	return 0;
}

BOOL CombatFrame_DeadMenu::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\resurrection.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml(&element, "resurrection_win\\pic\\backpic\\item0\\itemback", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&element, "resurrection_win\\pic\\backpic\\item0\\itemback\\item", "ClassName", "MallButton");
	m_pGUI->ChangeXml(&element, "resurrection_win\\pic\\backpic\\item1\\itemback", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&element, "resurrection_win\\pic\\backpic\\item1\\itemback\\item", "ClassName", "MallButton");

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&CombatFrame_DeadMenu::EventHandler));

	GUIButton* m_pBtnClose = (GUIButton*) m_pWnd->GetChild(_T("pic\\close"));
	m_pBtnClose->SetInvisible(TRUE);
	m_pBtnLocalRise = (GUIButton*) m_pWnd->GetChild(_T("pic\\button1"));
	m_pBtnPerfectRise = (GUIButton*) m_pWnd->GetChild(_T("pic\\button0"));
	m_pBtnAcceptRise = (GUIButton*) m_pWnd->GetChild(_T("pic\\button2"));

	m_pStcRorReborn = (GUIStatic*) m_pWnd->GetChild(_T("pic\\backpic\\item0"));
	m_pRebornBack = (IconStatic*) m_pStcRorReborn->GetChild(_T("itemback"));
	m_pReborn = (MallButton*) m_pStcRorReborn->GetChild(_T("itemback\\item"));
	m_pStcMoney = (GUIStatic*) m_pStcRorReborn->GetChild(_T("yuanbaonum"));
	m_pStcRebornRemain = (GUIStatic*) m_pStcRorReborn->GetChild(_T("word\\num"));
	m_pBtnRebornBuy = (GUIButton*) m_pStcRorReborn->GetChild(_T("buybutton"));

	m_pStcRorScapegoat = (IconStatic*) m_pWnd->GetChild(_T("pic\\backpic\\item1"));
	m_pScapegoatBack = (IconStatic*) m_pStcRorScapegoat->GetChild(_T("itemback"));
	m_pScapegoat = (MallButton*) m_pStcRorScapegoat->GetChild(_T("itemback\\item"));
	m_pStcScapegoatMoney = (GUIStatic*) m_pStcRorScapegoat->GetChild(_T("yuanbaonum"));
	m_pStcScapegoatRemain = (GUIStatic*) m_pStcRorScapegoat->GetChild(_T("word\\num"));
	m_pBtnScapegoatBuy = (GUIButton*) m_pStcRorScapegoat->GetChild(_T("buybutton"));

	if(P_VALID(m_pBtnAcceptRise))
		m_pBtnAcceptRise->SetEnable(false);
	m_pBtnBackRise	= (GUIButton*) m_pWnd->GetChild(_T("pic\\button3"));
	m_pStcHint = (GUIStatic*) m_pWnd->GetChild(_T("pic\\word"));
	if( P_VALID(m_pStcHint) )
		m_pStcHint->SetText(g_StrTable[_T("Revive_Hint")]);

	// 取得本地玩家状态
	if (RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_CSAttack) || RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_CSDefence))
	{
		m_pBtnLocalRise->SetEnable(false);
		m_pBtnPerfectRise->SetEnable(false);
	}

	if (RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_PvPAttack) || RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_PvPDefence))
	{
		m_pBtnLocalRise->SetEnable(false);
		m_pBtnPerfectRise->SetEnable(false);
		m_pBtnAcceptRise->SetEnable(false);
	}
	m_bRefresh = TRUE;
	ShowTool();

	//如果是守护地图，则等待15秒复活,dmap1
	if(MapMgr::Inst()->GetCurMapID() == 2962677420)
	{
		m_nReviewWite = 30 * 1000;
		m_pBtnBackRise->SetEnable(FALSE);
		m_bIsWar = TRUE;
	}

	return TRUE;
}

VOID  CombatFrame_DeadMenu::ShowTool()
{
	//----------------------------------显示复活物品------------------------------------
	DWORD item_Reborn_TypeID = GT_INVALID;
	vector<tagUIItem> vecUIItem;
	vecUIItem = MallMgr::Inst()->GetEasyBuyItem();
	for( size_t i = 0; i < vecUIItem.size(); i++ )
	{
		TCHAR szBuff[X_LONG_NAME] = {0};
		INT itemQuantity = 0;
		if( vecUIItem[i].dwUIType == EBUT_RevivalUI )
		{
			if( vecUIItem[i].dwTypeID == ITEM_SCAPEGOAT_TYPEID0
			 || vecUIItem[i].dwTypeID == ITEM_SCAPEGOAT_TYPEID1 )
					continue;
			tagMallItemProto* pProItem = MallMgr::Inst()->GetMallItemByTypeid( vecUIItem[i].dwTypeID );
			if( P_VALID(pProItem) )		
			{
				item_Reborn_TypeID = vecUIItem[i].dwTypeID;
				const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(pProItem->dwTypeID);
				const tagItemDisplayInfo *pInfo = ItemProtoData::Inst()->FindItemDisplay(pProItem->dwTypeID);
				if( P_VALID(pItem) && P_VALID(pInfo) )
				{
					m_pReborn->RefreshItem(pProItem->dwTypeID, 1, pItem->byQuality);
					m_pReborn->SetBtnData(ESBT_Item, pProItem->dwID, pProItem->dwTypeID, pProItem->nIndexInServer);
					if( pProItem->nSalePrice!=GT_INVALID && pProItem->nSalePrice!=pProItem->nPrice )
						_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"), pProItem->nSalePrice);
					else 
						_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"), pProItem->nPrice);
					m_pStcMoney->SetText(szBuff);

				}
				//获取背包中莲花化身的数量
				TList<DWORD> typeList;
				DWORD dwTypeID = 0;
				typeList.Clear();
				ItemMgr::Inst()->GetPocket()->GetSpecFuncItemTypeList(EISF_RevieveItem,typeList);
				typeList.ResetIterator();
				while ( typeList.PeekNext(dwTypeID) )
				{
					itemQuantity += ItemMgr::Inst()->GetPocket()->GetItemQuantity( dwTypeID );
				}
				tstringstream ss;
				ss<<itemQuantity;
				m_pStcRebornRemain->SetText(ss.str().c_str());
			}
			else
			{
				m_pBtnRebornBuy->SetEnable(FALSE);
				m_pStcRorReborn->SetInvisible(TRUE);
			}
		}
	}
	//----------------------------------显示替身娃娃------------------------------------
	//19级以下不显示
	if ( RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel() <= 19 )
	{
		m_pStcRorScapegoat->SetInvisible(TRUE);
	}
	else
	{
		m_pStcRorScapegoat->SetInvisible(FALSE);
		vector<tagUIItem> vecUIItem;
		vecUIItem = MallMgr::Inst()->GetEasyBuyItem();
		for( size_t i = 0; i < vecUIItem.size(); i++ )
		{
			TCHAR szBuff[X_LONG_NAME] = {0};
			if( vecUIItem[i].dwTypeID == ITEM_SCAPEGOAT_TYPEID0
			 || vecUIItem[i].dwTypeID == ITEM_SCAPEGOAT_TYPEID1 )
			{
				tagMallItemProto* pProItem = MallMgr::Inst()->GetMallItemByTypeid( vecUIItem[i].dwTypeID );
				if( P_VALID(pProItem) )		
				{
					const tagItemProto* pItem = ItemProtoData::Inst()->FindItemProto(pProItem->dwTypeID);
					const tagItemDisplayInfo *pInfo = ItemProtoData::Inst()->FindItemDisplay(pProItem->dwTypeID);
					if( P_VALID(pItem) && P_VALID(pInfo) )
					{
						m_pScapegoat->RefreshItem(pProItem->dwTypeID, 1, pItem->byQuality);
						m_pScapegoat->SetBtnData(ESBT_Item, pProItem->dwID, pProItem->dwTypeID, pProItem->nIndexInServer);
						if( pProItem->nSalePrice!=GT_INVALID && pProItem->nSalePrice!=pProItem->nPrice )
							_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"), pProItem->nSalePrice);
						else 
							_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"), pProItem->nPrice);
						m_pStcScapegoatMoney->SetText(szBuff);
					}
					//获取背包中替身娃娃数量
					INT itemQuantity = ItemMgr::Inst()->GetPocket()->GetItemQuantity( ITEM_SCAPEGOAT_TYPEID0 );
					itemQuantity += ItemMgr::Inst()->GetPocket()->GetItemQuantity( ITEM_SCAPEGOAT_TYPEID1 );
					tstringstream ss;
					ss<<itemQuantity;
					m_pStcScapegoatRemain->SetText(ss.str().c_str());
				}
				else
				{
					m_pBtnScapegoatBuy->SetEnable(FALSE);
					m_pStcRorScapegoat->SetInvisible(TRUE);
				}
			}	
		}
	}
}

BOOL CombatFrame_DeadMenu::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnLocalRise )
			{
				INT64 n64ID = ItemMgr::Inst()->GetSpecfuncItemID(EISF_RevieveItem);
				if(n64ID == GT_INVALID)
				{
					//提示
					//if( P_VALID(m_pStcHint) )                                    //注释掉的原因是:将提示改成中央屏显提示
					//{
					//	m_pStcHint->SetText(g_StrTable[_T("Revive_ItemLimit")]);
					//	m_pWnd->SetRefresh(TRUE);
					//}
					CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Revive_ItemLimit")]);
					return FALSE;
				}

				tagNC_RoleRevive msg;
				msg.eType = ERRT_Locus;
				msg.n64ItemID = n64ID;
				m_pSession->Send(&msg);

			}
			else if( pWnd == m_pBtnAcceptRise )
			{
				tagNC_RoleRevive msg;
				msg.eType = ERRT_Accept;
				m_pSession->Send(&msg);
			}
			else if( pWnd == m_pBtnBackRise)
			{				
				tagNC_RoleRevive msg;
				// 如果角色处于城战状态,进行城战复活
// 				if ( RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_CSAttack) == true || 
// 					RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_CSDefence) == true )
				if(		MapMgr::Inst()->GetCurMapID() == 2216111541 
					||	MapMgr::Inst()->GetCurMapID() == 2216112053 
					||	MapMgr::Inst()->GetCurMapID() == 2216110261 )
				{
// 					msg.eType = ERRT_CityStruggle;
// 				}
// 				// 如果是神迹争夺或者群雄乱舞
// 				else if (GuildMgr::Inst()->IsInGodBattle() )
// 				{
					if( !m_bUpdateTime )
					{
						msg.eType = ERRT_GuildBattle;
						m_nReviveTime = 15 * 1000;
						m_bUpdateTime = true;
						msg.n64ItemID = 0;
					}
					else
						return TRUE;
				}
				// 其他情况
				else
				{
					if ( (RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_PvPAttack) == true || 
						RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_PvPDefence) == true) && MapMgr::Inst()->GetCurMapID() != 3373743144)	//在天选者之战中不弹框
					{
						if( P_VALID(GetObj("DeadMenuMsgBox")) )
							KillObj("DeadMenuMsgBox");

						CreateObj("DeadMenuMsgBox", "MsgBox");
						MsgBox* pMsgBox = TObjRef<MsgBox>("DeadMenuMsgBox");
						pMsgBox->Init( _T(""), g_StrTable[_T("PvP_UI_ConfirmRevive")], 
							_T("OnPvPDeadMenuConfirm"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);
						return TRUE;
					}
					msg.eType = ERRT_ReturnCity;
				}
				m_pSession->Send(&msg);
			}
			else if( pWnd == m_pBtnPerfectRise )
			{
				//--------------获取背包中莲花化身的数量---------
				int n = 0;
				std::list<Item*> lst;
				ItemMgr::Inst()->GetPocket()->GetSpecFuncMap( EISF_RevieveItem, lst );

				std::list<Item*>::iterator iter;
				for( iter = lst.begin(); iter != lst.end(); ++iter )
				{
					n+=(*iter)->GetItemQuantity();
				}
				if(n>=5)
				{
					tagNC_RoleRevive msg;
					msg.eType = ERRT_Perfect;
					m_pSession->Send(&msg);
				}
				else //if( P_VALID(m_pStcHint) )       //莲花化身不足5个                                               
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("Revive_ItemLack")]);
					//m_pStcHint->SetText(g_StrTable[_T("Revive_ItemLack")]);  //注释掉的原因是:将提示改成中央屏显提示
					//m_pWnd->SetRefresh(TRUE);
				}
				//------------------------------------------------

			}
			else if( pWnd == m_pBtnRebornBuy )
			{
				const TCHAR *szSelectItem = _T("data\\ui\\Common\\L_icon-l.bmp");
				if( m_pReborn->GetBtnData().GetData1() != GT_INVALID )
				{
					MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(m_pReborn->GetBtnData().GetData1());
					MallMgr::Inst()->GetMallFrame()->SetCurMenu( MallFrame::EMIT_All );
					m_pRebornBack->SetPic(szSelectItem);
					MallMgr::Inst()->GetMallFrame()->BuyMallItem();
					m_bRefresh = TRUE;
					break;
				}
			}
			else if( pWnd == m_pBtnScapegoatBuy )
			{
				const TCHAR *szSelectItem = _T("data\\ui\\Common\\L_icon-l.bmp");
				if( m_pScapegoat->GetBtnData().GetData1() != GT_INVALID )
				{
					MallMgr::Inst()->GetMallFrame()->SetHopeBuyItemID(m_pScapegoat->GetBtnData().GetData1());
					MallMgr::Inst()->GetMallFrame()->SetCurMenu( MallFrame::EMIT_All );
					m_pScapegoatBack->SetPic(szSelectItem);
					MallMgr::Inst()->GetMallFrame()->BuyMallItem();
					m_bRefresh = TRUE;
					break;
				}
			}
		}
		break;
	case EGUIE_Drag:
		{
			const TCHAR *szSelectItem = _T("data\\ui\\Common\\L_icon-l.bmp");
			if( pWnd == m_pReborn )
			{
				if( m_pReborn->GetBtnData().GetData1() != GT_INVALID )
				{
					m_pRebornBack->SetPic(szSelectItem);
					break;
				}
			} 
			if( pWnd == m_pScapegoat )
			{
				if( m_pScapegoat->GetBtnData().GetData1() != GT_INVALID )
				{
					m_pScapegoatBack->SetPic(szSelectItem);
					break;
				}
			} 
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if( pWnd == m_pReborn )
			{
				DWORD dwID =  m_pReborn->GetBtnData().GetData2();
				ShowTip( m_pReborn, ToolTipCreator::Inst()->GetItemTips(dwID).c_str());
			}
			if( pWnd == m_pScapegoat )
			{
				DWORD dwID =  m_pScapegoat->GetBtnData().GetData2();
				ShowTip( m_pScapegoat, ToolTipCreator::Inst()->GetItemTips(dwID).c_str());
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(NULL, _T(""));
		}
		break;
	default:
		break;
	}

	return TRUE;
}

DWORD CombatFrame_DeadMenu::OnPvPDeadMenuConfirmMsgBox(tagMsgBoxEvent* pEvent)
{
	if( MBF_OK == pEvent->eResult)
	{
		tagNC_RoleRevive msg;
		msg.eType = ERRT_ReturnCity;
		m_pSession->Send( &msg );
	}
	return 0;
}

DWORD CombatFrame_DeadMenu::RefreshDeadMenu( tagGameEvent* pGameEvent)
{
	ShowTool();
	m_pWnd->SetRefresh(TRUE);
	return 0;
}

DWORD CombatFrame_DeadMenu::OnUpdateLPItem(tagUpdateLPItemEvent* pGameEvent)
{
	if (P_VALID(m_pWnd))
	{
		if (!m_pWnd->IsInvisible())
		{
			m_bRefresh = TRUE;
			m_pWnd->SetRefresh(TRUE);
		}
	}
	return 0;
}

