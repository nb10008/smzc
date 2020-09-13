#include "StdAfx.h"
#include "FairyFusionFrame.h"
#include "ItemEvent.h"
#include "QuestEvent.h"
#include "QuestMgr.h"
#include "RoleMgr.h"
#include "CreatureData.h"
#include "Item.h"
#include "ItemMgr.h"
#include "ItemProtoData.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_compose.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "MsgBox.h"
#include "ItemEvent.h"
#include "Container.h"

FairyFusionFrame::FairyFusionFrame(void):m_Trunk(this)
{
	m_escapeTime = 0;
	m_n64FirstID = GT_INVALID;
	m_n64SecondID = GT_INVALID;
	m_n64IMID = GT_INVALID;
	m_byIMNum = 0;
	m_bSendMsg = FALSE;
	m_pLockFirstItem = NULL;
	m_pLockSecondItem = NULL;
	m_pLockIMItem = NULL;
}

FairyFusionFrame::~FairyFusionFrame(void)
{

}

BOOL FairyFusionFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	//--注册游戏事件
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("tagStartNPCTalkEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyFusionFrame::OnStartNPCTalkEvent));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("ItemEvent_Move2Compose"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyFusionFrame::OnEventProduceAddItem));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("OpenFairyFusion"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyFusionFrame::OnOpenFairyFusion));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("BeginFusionEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyFusionFrame::OnBeginFusionEvent));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("ItemEvent_UpdateItem"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyFusionFrame::OnItemEvent_UpdateItem));

	m_pCmdMgr->Register("NS_FairyFusion",		(NETMSGPROC)m_Trunk.sfp2(&FairyFusionFrame::OnNS_FairyFusion), 			_T("NS_FairyFusion"));
	m_pCmdMgr->Register("NS_GetFusionFairy",	(NETMSGPROC)m_Trunk.sfp2(&FairyFusionFrame::OnNS_GetFusionFairy),		_T("NS_GetFusionFairy"));

	return TRUE;
}

BOOL FairyFusionFrame::Destroy()
{
	GameFrame::Destroy();
	if( P_VALID(m_pWndMain) )
		m_pGUI->AddToDestroyList(m_pWndMain);

	if(P_VALID(m_pWndGetFairy))
		m_pGUI->AddToDestroyList(m_pWndGetFairy);

	//--注销游戏事件
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("tagStartNPCTalkEvent"), 	(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyFusionFrame::OnStartNPCTalkEvent));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("ItemEvent_Move2Compose"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyFusionFrame::OnEventProduceAddItem));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("OpenFairyFusion"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyFusionFrame::OnOpenFairyFusion));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("BeginFusionEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyFusionFrame::OnBeginFusionEvent)); 
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("ItemEvent_UpdateItem"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&FairyFusionFrame::OnItemEvent_UpdateItem));

	m_pCmdMgr->UnRegister("NS_FairyFusion", (NETMSGPROC)m_Trunk.sfp2(&FairyFusionFrame::OnNS_FairyFusion));
	m_pCmdMgr->UnRegister("NS_GetFusionFairy", (NETMSGPROC)m_Trunk.sfp2(&FairyFusionFrame::OnNS_GetFusionFairy));
	return TRUE;
}

BOOL FairyFusionFrame::EscCancel()
{
	if(!m_pWndMain->IsInvisible())
	{
		m_pWndMain->SetInvisible(TRUE);
		ClearData();
	}
	if(!m_pWndGetFairy->IsInvisible())
	{
		m_pWndGetFairy->SetInvisible(TRUE);
		SendGetFairyMsg(TRUE);
	}

	return FALSE;
}

BOOL FairyFusionFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\fairyfusion.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "fairyfusion1\\fairyfusion1\\fairyback\\fairypic", "ClassName", "ComposeStatic");
	m_pGUI->ChangeXml(&element, "fairyfusion1\\fairyfusion1\\fairyback0\\fairypic", "ClassName", "ComposeStatic");
	m_pGUI->ChangeXml(&element, "fairyfusion1\\fairyfusion1\\fairyback1\\fairypic", "ClassName", "ComposeStatic");
	m_pGUI->ChangeXml(&element, "fairyfusion1\\fairyfusion1\\progpic\\progress", "ClassName", "ActiveProgress");
	m_pWndMain				= m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pFirstFairyIcon		= (ComposeStatic*)m_pWndMain->GetChild(_T("fairyfusion1\\fairyback\\fairypic"));
	m_pSecondFairyIcon		= (ComposeStatic*)m_pWndMain->GetChild(_T("fairyfusion1\\fairyback0\\fairypic"));
	m_pIMIcon				= (ComposeStatic*)m_pWndMain->GetChild(_T("fairyfusion1\\fairyback1\\fairypic"));
	m_pBtnClose				= (GUIButton*)m_pWndMain->GetChild(_T("fairyfusion1\\close"));
	m_pBtnFusion			= (GUIButton*)m_pWndMain->GetChild(_T("fairyfusion1\\startbutt"));
	m_pStcTip				= (GUIStatic*)m_pWndMain->GetChild(_T("fairyfusion1\\listback\\breedlist\\tu1"));			//熔炼前的提示信息
	m_pStcRetTip			= (GUIStatic*)m_pWndMain->GetChild(_T("fairyfusion1\\listback\\breedlist\\tu2"));			//熔炼时几率tip
	m_pStcPinkRet			= (GUIStatic*)m_pWndMain->GetChild(_T("fairyfusion1\\listback\\breedlist\\tu2\\word4"));	//粉色成功率
	m_pStcPurpleRet			= (GUIStatic*)m_pWndMain->GetChild(_T("fairyfusion1\\listback\\breedlist\\tu2\\word5"));	//紫色成功率
	m_pStcOrangeRet			= (GUIStatic*)m_pWndMain->GetChild(_T("fairyfusion1\\listback\\breedlist\\tu2\\word6"));	//橙色成功率
	m_pStcIMNum				= (GUIStatic*)m_pWndMain->GetChild(_T("fairyfusion1\\putinback\\putin"));		//道具数量
	m_pBtnUp				= (GUIButton*)m_pWndMain->GetChild(_T("fairyfusion1\\nextbutt"));			//增加
	m_pBtnDown				= (GUIButton*)m_pWndMain->GetChild(_T("fairyfusion1\\backbutt"));			//减少
	m_pProgress				= (ActiveProgress*)m_pWndMain->GetChild(_T("fairyfusion1\\progpic\\progress"));

	m_pFirstFairyIcon->SetPic(NULLIconPic);
	m_pSecondFairyIcon->SetPic(NULLIconPic);
	m_pIMIcon->SetPic(NULLIconPic);
	m_pWndMain->SetRefresh(TRUE);

	m_pWndMain->SetInvisible(TRUE);

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWndMain->GetFullName().c_str(), m_Trunk.sfp1(&FairyFusionFrame::EventHandler));

	XmlElement element1;
	tstring strPath1 = g_strLocalPath + _T("\\ui\\fairy_win.xml");
	m_pGUI->LoadXml(&element1, "VFS_System", strPath1.c_str());
	m_pGUI->ChangeXml(&element1, "fairy_win\\fairy_pic\\itempic1\\item", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&element1, "fairy_win\\fairy_pic\\itempic2\\item", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&element1, "fairy_win\\fairy_pic\\itempic1", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&element1, "fairy_win\\fairy_pic\\itempic2", "ClassName", "IconStatic");
	m_pWndGetFairy			= m_pGUI->CreateWnd(_T("\\desktop"), &element1);
	m_pIStcFirstFairy		= (IconStatic*)m_pWndGetFairy->GetChild(_T("fairy_pic\\itempic1\\item"));	//第一只妖精
	m_pIStcSecondFairy		= (IconStatic*)m_pWndGetFairy->GetChild(_T("fairy_pic\\itempic2\\item"));	//第二只妖精
	m_pBtnRecive			= (GUIButton*)m_pWndGetFairy->GetChild(_T("fairy_pic\\butt2"));		//领取妖精
	m_pBtnCloseGetWnd		= (GUIButton*)m_pWndGetFairy->GetChild(_T("fairy_pic\\butt1"));		//关闭按钮
	m_pIStcFirstFairyPic	= (IconStatic*)m_pWndGetFairy->GetChild(_T("fairy_pic\\itempic1"));
	m_pIStcSecondFairyPic	= (IconStatic*)m_pWndGetFairy->GetChild(_T("fairy_pic\\itempic2"));
	m_pWndGetFairy->SetInvisible(TRUE);
	m_pGUI->RegisterEventHandler(m_pWndGetFairy->GetFullName().c_str(), m_Trunk.sfp1(&FairyFusionFrame::EventHandler));

	return TRUE;
}

VOID FairyFusionFrame::OnEvent(tagGameEvent* pEvent)
{

}

VOID FairyFusionFrame::Update()
{
	GameFrame::Update();

 	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
 	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
 	{
 		m_escapeTime = 0;
 
 		if (RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID() ) )
 		{
 			m_pWndMain->SetInvisible(TRUE);
			if(!m_pWndGetFairy->IsInvisible())
			{
				m_pWndGetFairy->SetInvisible(TRUE);
				SendGetFairyMsg(TRUE);
			}

 		}
 	}

	if(m_bSendMsg)
	{
		float deltaTime =  Cool3D::Kernel::Inst()->GetDeltaTime();
		float fcur = m_pProgress->GetCurrentValue();
		float fMax = m_pProgress->GetMaxValue();
		if(fcur < fMax)
		{
			fcur += deltaTime;
			m_pProgress->SetValue(fcur, true);
			m_pProgress->SetRefresh();
		}
		//发送合成消息
		if(fcur >= fMax)
		{	
			if(m_n64FirstID != m_n64SecondID)
			{
				tagNC_FairyFusion cmd;
				cmd.n64FirstFairy = m_n64FirstID;
				cmd.n64SecondFairy = m_n64SecondID;
				cmd.n64IMID = m_n64IMID;
				cmd.byItemNum = m_byIMNum;
				m_pSession->Send(&cmd);
			}

			m_bSendMsg = FALSE;
		}
	}
	
}


DWORD FairyFusionFrame::OnStartNPCTalkEvent(tagStartNPCTalkEvent* pGameEvent)
{
	const tagCreatureProto* pCreatureProto = CreatureData::Inst()->FindNpcAtt(pGameEvent->typeID);
	if(!P_VALID(pCreatureProto))
		return 0;

	return 0;
}

BOOL FairyFusionFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
				if(pWnd == m_pBtnClose)
				{
					m_pWndMain->SetInvisible(TRUE);
					ClearData();
				}
				else if(pWnd == m_pBtnFusion)
				{
					if(m_n64FirstID != GT_INVALID && m_n64SecondID != GT_INVALID)
					{
						if(P_VALID(GetObj("BeginFusionMsgBox")))
							KillObj("BeginFusionMsgBox");

						CreateObj("BeginFusionMsgBox", "MsgBox");
						TObjRef<MsgBox>("BeginFusionMsgBox")->Init(_T(""),g_StrTable[_T("FairyMessageBox")] , _T("BeginFusionEvent"));
					}
				}
				else if(pWnd == m_pBtnUp)
				{
					if(m_byIMNum != 99 && m_n64IMID != GT_INVALID)
					{
						Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64IMID);
						if(P_VALID(pItem) && pItem->GetItemQuantity() > m_byIMNum)
						{
							m_byIMNum++;
							TCHAR szText[128] = {0};
							_stprintf(szText, _T("%d"), m_byIMNum);
							m_pStcIMNum->SetText(szText);
							RefreshRetUI();
						}

					}
				}
				else if(pWnd == m_pBtnDown)
				{
					if(m_byIMNum != 0 && m_n64IMID != GT_INVALID)
					{
						m_byIMNum--;
						TCHAR szText[128] = {0};
						_stprintf(szText, _T("%d"), m_byIMNum);
						m_pStcIMNum->SetText(szText);
						RefreshRetUI();
					}
				}
				else if(pWnd == m_pBtnRecive)
				{
					SendGetFairyMsg(FALSE);
				}
				else if(pWnd == m_pBtnCloseGetWnd)
				{
					SendGetFairyMsg(TRUE);
				}
		}
		break;
	case EOnEventSendDropGUI_Item:
		{
			if(m_pWndMain->IsInvisible())
				break;
			if(pWnd == m_pFirstFairyIcon)
			{
				INT16 nPos = INT16(pEvent->dwParam2);
				Equipment* pItem = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
				if(P_VALID(pItem))
				{
					const tagFabaoProto* pItemProto = (tagFabaoProto*)pItem->GetProto();
					if(!P_VALID(pItemProto) || pItemProto->eType != EIT_FaBao)	//判断不为妖精
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_NotFabao")]);
						return TRUE;
					}

					const tagFabaoSpec* pFabaoSpec = pItem->GetFaBaoSpe();
					if(!P_VALID(pFabaoSpec))
						return TRUE;


					if(pFabaoSpec->n16Stage < 40)	//等级小与40了
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_LowLevel")]);
						return TRUE;
					}

					if(pFabaoSpec->n16NativeIntelligence < 60)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_LowNativeIntelligence")]);
						return TRUE;
					}

					if( pItem->GetItemQuality() < EIQ_Orange )
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_LowQuality")]);
						return TRUE;
					}

					if(!pItemProto->CanFusion())
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_FairyError")]);
						return TRUE;
					}

					if(pItem->GetFaBaoSpe()->eFabaoStatus != EEFS_Normal && pItem->GetFaBaoSpe()->eFabaoStatus != EEFS_Waiting)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_StatusError")]);
						return TRUE;
					}

					m_n64FirstID = pItem->GetItemId();
					m_n64TmepFirstFairy = m_n64FirstID;

				}
				RefreshRetUI();
			}
			else if(pWnd == m_pSecondFairyIcon)
			{
				INT16 nPos = INT16(pEvent->dwParam2);
				Equipment* pItem = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
				if(P_VALID(pItem))
				{
					const tagFabaoProto* pItemProto = (tagFabaoProto*)pItem->GetProto();
					if(!P_VALID(pItemProto) || pItemProto->eType != EIT_FaBao)	//判断不为妖精
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_NotFabao")]);
						return TRUE;
					}

					const tagFabaoSpec* pFabaoSpec = pItem->GetFaBaoSpe();
					if(!P_VALID(pFabaoSpec))
						return TRUE;


					if(pFabaoSpec->n16Stage < 40)	//等级小与40了
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_LowLevel")]);
						return TRUE;
					}

					if(pFabaoSpec->n16NativeIntelligence < 60)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_LowNativeIntelligence")]);
						return TRUE;
					}

					if( pItem->GetItemQuality() < EIQ_Orange )
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_LowQuality")]);
						return TRUE;
					}

					if(!pItemProto->CanFusion())
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_FairyError")]);
						return TRUE;
					}

					if(pItem->GetFaBaoSpe()->eFabaoStatus != EEFS_Normal && pItem->GetFaBaoSpe()->eFabaoStatus != EEFS_Waiting)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_StatusError")]);
						return TRUE;
					}

					m_n64SecondID = pItem->GetItemId();

				}
				RefreshRetUI();
			}
			else if(pWnd == m_pIMIcon)
			{
				INT16 nPos = INT16(pEvent->dwParam2);
				Item* pItem = ItemMgr::Inst()->GetPocketItem(nPos);
				if(pItem->GetItemSpecFunc() != EISF_FairyFusion)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_ItemError")]);
				}
				else
				{
					m_n64IMID = pItem->GetItemId();
					RefreshRetUI();
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		if( pWnd == m_pFirstFairyIcon )
		{
			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64FirstID);
			if( P_VALID(pEquip) )
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
			}
		}
		else if( pWnd == m_pSecondFairyIcon )
		{
			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64SecondID);
			if( P_VALID(pEquip) )
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
			}
		}
		else if( pWnd == m_pIMIcon )
		{
			Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64IMID);
			if( P_VALID(pItem) )
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
			}
		}
		else if( pWnd == m_pIStcFirstFairy )
		{
			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64TmepFirstFairy);
			if( P_VALID(pEquip) )
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
			}
		}
		else if(pWnd == m_pIStcSecondFairy)
		{
 			if(P_VALID(m_pResultFairy))
 			{
 				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pResultFairy).c_str());
 			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	case EGUIE_Drag:
		{
			if(pWnd == m_pIStcFirstFairy)
			{
				m_pIStcFirstFairyPic->SetPic(SelectIconPic);
				m_pIStcSecondFairyPic->SetPic(DefaultIconPic);
				m_bChoseFirst = TRUE;
			}
			else if(pWnd == m_pIStcSecondFairy)
			{
				m_pIStcSecondFairyPic->SetPic(SelectIconPic);
				m_pIStcFirstFairyPic->SetPic(DefaultIconPic);
				m_bChoseFirst = FALSE;
			}
		}
		break;
	}

	return TRUE;
}

DWORD FairyFusionFrame::OnEventProduceAddItem( tagItemPosChangeExEvent* pGameEvent )
{	
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;

// 	//正在分解中,错误提示
// 	if( m_eCurState != EDeCompose_NULL )
// 	{
// 		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Processing")]);
// 		return 0;
// 	}
// 
// 	DWORD dwTypeID = pGameEvent->dwTypeIDSrc;
// 	if( m_eProType==EPCT_PointUp )
// 	{
// 		//判断是否为装备
// 		if(MIsEquipment(dwTypeID))
// 		{
// 			INT16 nPos = INT16(pGameEvent->n16IndexSrc);
// 			Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
// 			if(!P_VALID(pEquip))
// 				return 0;
// 
// 
// 			if( pEquip->IsNotIdetified() )
// 			{
// 				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Equip_NoIdetified")]);
// 				return 0;
// 			}
// 
// 			if( m_eComType == ECOMT_PointUp ) 
// 			{
// 				if(  pEquip->GetItemLevel() < 30 )
// 				{
// 					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Decompose_Error3")]);
// 					return 0;
// 				}
// 				else if( pEquip->GetItemQuality() < 1 )
// 				{
// 					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Decompose_Error7")]);
// 					return 0;
// 				}
// 			}
// 
// 
// 			ClearUIandStuff();
// 
// 			JudgeDecompositonFormula(dwTypeID, pEquip->GetItemId(), pEquip->GetEquipPos());
// 			m_pWndMain->SetRefresh(TRUE);
// 		}
// 	
	return 0;
}

VOID FairyFusionFrame::RefreshRetUI()
{
	if(m_n64FirstID == GT_INVALID && m_n64SecondID == GT_INVALID)
	{
		m_pStcTip->SetInvisible(FALSE);
		m_pStcRetTip->SetInvisible(TRUE);

		m_pFirstFairyIcon->SetPic(NULLIconPic);
		m_pFirstFairyIcon->SetRefresh(TRUE);
		m_pSecondFairyIcon->SetPic(NULLIconPic);
		m_pSecondFairyIcon->SetRefresh(TRUE);
		m_pIMIcon->SetPic(NULLIconPic);
		m_pIMIcon->SetRefresh(TRUE);

		m_n64IMID = GT_INVALID;
		if(P_VALID(m_pLockIMItem))
			m_pLockIMItem->SetOperable(TRUE);

		return;
	}
	else if(m_n64FirstID != GT_INVALID && m_n64SecondID != GT_INVALID)
	{
		m_pStcTip->SetInvisible(TRUE);
		m_pStcRetTip->SetInvisible(FALSE);
	}


	//计算值=[（1号妖精资质+1号妖精等级/2+1号妖精品质对应值）+（2号妖精资质+2号妖精等级/2+2号妖精品质对应值）]/2
	//品质对应 橙=15 紫=25 粉=50
	//计算数值取整
	Equipment* pFirstFairy	= (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64FirstID);
	Equipment* pSecondFairy = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64SecondID);
	Item* pIMItem = ItemMgr::Inst()->GetPocketItemByID(m_n64IMID);
	
	if(P_VALID(pFirstFairy))
	{
		m_pFirstFairyIcon->SetPic(ItemMgr::Inst()->GetItemIconPath(pFirstFairy->GetItemTypeID()));
		if(P_VALID(m_pLockFirstItem))
			m_pLockFirstItem->SetOperable(TRUE);
		pFirstFairy->SetOperable(FALSE);
		m_pLockFirstItem = pFirstFairy;
	}
	else
		m_pFirstFairyIcon->SetPic(NULLIconPic);
	m_pFirstFairyIcon->SetRefresh(TRUE);

	if(P_VALID(pSecondFairy))
	{
		m_pSecondFairyIcon->SetPic(ItemMgr::Inst()->GetItemIconPath(pSecondFairy->GetItemTypeID()));
		if(P_VALID(m_pLockSecondItem))
			m_pLockSecondItem->SetOperable(TRUE);
		pSecondFairy->SetOperable(FALSE);
		m_pLockSecondItem = pSecondFairy;
	}
	else 
		m_pSecondFairyIcon->SetPic(NULLIconPic);
	m_pSecondFairyIcon->SetRefresh(TRUE);

	if(P_VALID(pIMItem))
	{
		m_pIMIcon->SetPic(ItemMgr::Inst()->GetItemIconPath(pIMItem->GetItemTypeID()));
		if(P_VALID(m_pLockIMItem))
			m_pLockIMItem->SetOperable(TRUE);
		pIMItem->SetOperable(FALSE);
		m_pLockIMItem = pIMItem;
	}
	else
		m_pIMIcon->SetPic(NULLIconPic);
	m_pIMIcon->SetRefresh(TRUE);

	if(!P_VALID(pFirstFairy) || !P_VALID(pSecondFairy))
	{
		if(P_VALID(pIMItem))
		{
			m_pIMIcon->SetPic(NULLIconPic);
			if(P_VALID(m_pLockIMItem))
				m_pLockIMItem->SetOperable(TRUE);
			m_n64IMID = GT_INVALID;
		}
		return;
	}

	m_pStcTip->SetInvisible(TRUE);
	m_pStcRetTip->SetInvisible(FALSE);

	INT nScore = 0;
	BYTE byFirstQuilityScore = GetFushionScoreByQuality(pFirstFairy->GetItemQuality());
	BYTE bySecondQuilityScore = GetFushionScoreByQuality(pSecondFairy->GetItemQuality());

	INT nFirstLevel = pFirstFairy->GetFaBaoSpe()->n16Stage;
	INT nSecondLevel = pSecondFairy->GetFaBaoSpe()->n16Stage;
	INT nFirstIntelligence = pFirstFairy->GetFaBaoSpe()->n16NativeIntelligence;
	INT nSecondIntelligence = pSecondFairy->GetFaBaoSpe()->n16NativeIntelligence;
	//nScore += (pFirstFairy->GetFaBaoSpe()->n16NativeIntelligence + pFirstFairy->GetFaBaoSpe()->n16Stage/2 + byFirstQuilityScore + pSecondFairy->GetFaBaoSpe()->n16NativeIntelligence + pSecondFairy->GetFaBaoSpe()->n16Stage/2 + bySecondQuilityScore);
	nScore = (nFirstIntelligence + nFirstLevel / 2 + byFirstQuilityScore) + ( nSecondIntelligence + nSecondLevel / 2 + bySecondQuilityScore );
	nScore /= 2;

	DWORD dwPinkRet = 0;
	DWORD dwPurpleRet = 0;
	DWORD dwOrangeRet = 0;
	DWORD dwFailRet = 0;
	//从表里取各种成功率~~
	const tagFairyFusionRetProto* pProto = ItemProtoData::Inst()->GetFauryFusionRet(nScore);
	if(P_VALID(pProto))
	{
		dwPinkRet = pProto->dwPinkRet;
		dwPurpleRet = pProto->dwPurpleRet;
		dwOrangeRet = pProto->dwOrangeRet;
		dwFailRet = pProto->dwFailRet;
	}
	//IM道具影响
	if(dwFailRet != 0 && m_n64IMID != GT_INVALID)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotUseFusionItem")]);	//提示有几率失败不让用道具
		if(P_VALID(m_pLockIMItem))
			m_pLockIMItem->SetOperable(TRUE);
		m_n64IMID = GT_INVALID;
		m_pIMIcon->SetPic(NULLIconPic);
		m_byIMNum = 0;
		m_pStcIMNum->SetText(_T(""));
	}

	dwPinkRet += 100*m_byIMNum;
	if(dwOrangeRet != 0 && dwOrangeRet >= 100*m_byIMNum)
	{
		dwOrangeRet -= 100*m_byIMNum;
	}
	else if(dwOrangeRet != 0 && dwOrangeRet < 100*m_byIMNum)
	{
		if(dwPurpleRet < 100*m_byIMNum - dwOrangeRet)
			dwPurpleRet = 0;
		else
			dwPurpleRet -= (100*m_byIMNum - dwOrangeRet);
		dwOrangeRet = 0;
	}
	TCHAR szText[128] = {0};
	_stprintf(szText, _T("%d%%"), dwPinkRet/100);
	m_pStcPinkRet->SetTextColor(0xffff80c0);
	m_pStcPinkRet->SetText(szText);
	_stprintf(szText, _T("%d%%"), dwPurpleRet/100);
	m_pStcPurpleRet->SetTextColor(0xFF9932CD);
	m_pStcPurpleRet->SetText(szText);
	_stprintf(szText, _T("%d%%"), dwOrangeRet/100);
	m_pStcOrangeRet->SetTextColor(0xFFFF8000);
	m_pStcOrangeRet->SetText(szText);
}

DWORD FairyFusionFrame::OnOpenFairyFusion(tagGameEvent* pEvent)
{
	m_pWndMain->SetInvisible(FALSE);
	ClearData();
	RefreshRetUI();

	return 0;
}

VOID FairyFusionFrame::ClearData()
{
	m_escapeTime = 0;
	m_n64FirstID = GT_INVALID;
	m_n64SecondID = GT_INVALID;
	m_n64IMID = GT_INVALID;
	m_byIMNum = 0;
	m_bSendMsg = FALSE;
	m_pProgress->SetValue(0, TRUE);
	if(P_VALID(m_pLockFirstItem))
		m_pLockFirstItem->SetOperable(TRUE);
	if(P_VALID(m_pLockSecondItem))
		m_pLockSecondItem->SetOperable(TRUE);
	if(P_VALID(m_pLockIMItem))
		m_pLockIMItem->SetOperable(TRUE);
	m_pStcIMNum->SetText(_T("0"));
}

DWORD FairyFusionFrame::OnNS_FairyFusion(tagNS_FairyFusion* pMsg, LPVOID pPrama)
{
	switch(pMsg->dwErrorCode)
	{
	case E_FairyFusion_LowQuality:// 妖精品质不足（要求橙色以上） [3/22/2012 zhangzhihua]
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_LowQuality")]);
		break;
	case E_FairyFusion_LowNativeIntelligence:// 妖精资质不足60 [3/22/2012 zhangzhihua]
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_LowNativeIntelligence")]);
		break;
	case E_FairyFusion_LowLevel:// 妖精等级不足40 [3/22/2012 zhangzhihua]
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_LowLevel")]);
		break;
	case E_FairyFusion_StatusError:// 妖精不能在孕育，生育，休憩阶段 [3/22/2012 zhangzhihua]
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_StatusError")]);
		break;
	case E_FairyFusion_FairyError:// 特殊妖精，无法融合 [3/22/2012 zhangzhihua]
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_FairyError")]);
		break;
	case E_FairyFusion_CannotUseItem:// 不能使用道具 [3/23/2012 zhangzhihua]
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_CannotUseItem")]);
		break;
	case E_FairyFusion_Faild:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyFusion_Faild")]);
		break;
	}

	ClearData();
	RefreshRetUI();

	return 0;
}

DWORD FairyFusionFrame::OnNS_GetFusionFairy(tagNS_GetFusionFairy* pMsg, LPVOID pPrama)
{
	
	return 0;
}

DWORD FairyFusionFrame::OnBeginFusionEvent(tagMsgBoxEvent* pEvent)
{
	if(pEvent->eResult == MBF_OK)
	{
		m_pProgress->SetValue(0, TRUE);
		m_pProgress->SetSpeed(100);
		m_pProgress->SetMaxValue(5);
		m_bSendMsg = TRUE;
	}
	return 0;
}

DWORD FairyFusionFrame::OnItemEvent_UpdateItem(tagUpdateLPItemEvent* pEvent)
{
	if(pEvent->eType == EICT_TempBag)
	{
		m_pResultFairy = (Equipment*)ItemMgr::Inst()->GetTempCpntainer()->GetValue(0);
		if(!P_VALID(m_pResultFairy))
		{
			return 0;
		}
		m_pWndGetFairy->SetInvisible(FALSE);
		m_pWndGetFairy->FlipToTop();
		m_pIStcSecondFairy->SetPic(ItemMgr::Inst()->GetItemIconPath(m_pResultFairy->GetItemTypeID()));
		Equipment* pOldFairy = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64FirstID);
		if(P_VALID(pOldFairy))
			m_pIStcFirstFairy->SetPic(ItemMgr::Inst()->GetItemIconPath(pOldFairy->GetItemTypeID()));
	}
	return 0;
}

VOID FairyFusionFrame::SendGetFairyMsg(bool isAutoSend)
{
	tagNC_GetFusionFairy cmd;
	if(isAutoSend)
		cmd.bWhichOne = TRUE;
	else
		cmd.bWhichOne = m_bChoseFirst;
	m_pSession->Send(&cmd);
	m_n64TmepFirstFairy = GT_INVALID;
	m_pWndGetFairy->SetInvisible(TRUE);

}