#include "stdafx.h"
#include "EquipGrowFrame.h"
#include "ItemFrame.h"
#include "ItemEvent.h"
#include "ItemMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "AudioSys.h"
#include "Container.h"
#include "ItemMgr.h"
#include "ItemProtoData.h"
#include "RoleMgr.h"
#include "QuestMgr.h"
#include "CurrencyMgr.h"


EquipGrowFrame::EquipGrowFrame( void ):m_Trunk(this)
{
	m_pWndMain = NULL;
	m_updateTimeDistance = 0;

}

EquipGrowFrame::~EquipGrowFrame( void )
{

}

BOOL EquipGrowFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	//--注册游戏事件
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2Compose"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipGrowFrame::OnEventProduceAddItem));
	m_pMgr->RegisterEventHandle(_T("Open_EquipGrow"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipGrowFrame::OnOpen_EquipGrow));
	m_pMgr->RegisterEventHandle(_T("ItemEvent_UpdateItem"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipGrowFrame::OnItemEvent_UpdateItem));

	TObjRef<NetCmdMgr>()->Register("NS_EquipGrow",			(NETMSGPROC)m_Trunk.sfp2(&EquipGrowFrame::OnNS_EquipGrow),		_T("NS_EquipGrow"));
	TObjRef<NetCmdMgr>()->Register("NS_EquipGrowTrans",		(NETMSGPROC)m_Trunk.sfp2(&EquipGrowFrame::OnNS_EquipGrowTrans), _T("NS_EquipGrowTrans"));
	TObjRef<NetCmdMgr>()->Register("NS_EquipGrowDec",		(NETMSGPROC)m_Trunk.sfp2(&EquipGrowFrame::OnNS_EquipGrowDec),	_T("NS_EquipGrowDec"));

	return TRUE;
}

BOOL EquipGrowFrame::EscCancel()
{
	if(m_pWndMain->IsInvisible())
		return FALSE;

	Hide();
	SetStrategy(EIST_ItemPocket);
	return TRUE;
}

BOOL EquipGrowFrame::Destroy()
{
	GameFrame::Destroy();
	if( P_VALID(m_pWndMain) )
		m_pGUI->AddToDestroyList(m_pWndMain);

	SetStrategy(EIST_ItemPocket);

	//--注册游戏事件
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Compose"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipGrowFrame::OnEventProduceAddItem));
	m_pMgr->UnRegisterEventHandler(_T("Open_EquipGrow"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipGrowFrame::OnOpen_EquipGrow));
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_UpdateItem"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&EquipGrowFrame::OnItemEvent_UpdateItem));

	TObjRef<NetCmdMgr>()->UnRegister("NS_EquipGrow",			(NETMSGPROC)m_Trunk.sfp2(&EquipGrowFrame::OnNS_EquipGrow));
	TObjRef<NetCmdMgr>()->UnRegister("NS_EquipGrowTrans",		(NETMSGPROC)m_Trunk.sfp2(&EquipGrowFrame::OnNS_EquipGrowTrans));
	TObjRef<NetCmdMgr>()->UnRegister("NS_EquipGrowDec",			(NETMSGPROC)m_Trunk.sfp2(&EquipGrowFrame::OnNS_EquipGrowDec));


	return TRUE;
}

BOOL EquipGrowFrame::ReloadUI()
{
	XmlElement element;
	TCHAR szText[128] = {0};
	tstring strPath = g_strLocalPath + _T("\\ui\\equipment.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "equwin\\equ\\back1\\k2\\back1\\itemback\\item", "ClassName", "ComposeStatic");
	m_pGUI->ChangeXml(&element, "equwin\\equ\\back1\\k2\\back2\\itemback\\item", "ClassName", "ComposeStatic");
	m_pGUI->ChangeXml(&element, "equwin\\equ\\back1\\k1\\itemback\\item", "ClassName", "ComposeStatic");
 	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
 	{
 		CHAR szTmp[X_LONG_NAME] = {0};
 		_snprintf(szTmp, sizeof(szTmp)/sizeof(CHAR), "equwin\\equ\\back1\\k1\\word3\\pic%d" ,i+1);
 		m_pGUI->ChangeXml(&element, szTmp, "ClassName", "IconStatic");
 	}
 	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
 	{
 		CHAR szTmp[X_LONG_NAME] = {0};
 		_snprintf(szTmp, sizeof(szTmp)/sizeof(CHAR), "\\desktop\\equwin\\equ\\back1\\k2\\back1\\word3\\pic%d", i+1);
 		m_pGUI->ChangeXml(&element, szTmp, "ClassName", "IconStatic");
 	}

	m_pWndMain = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pPBtnEquipGrow		= (GUIPushButton*)m_pWndMain->GetChild(_T("equ\\bagchoose\\bagchoose1"));					//装备成长PushButton
	m_pPBtnEquipInherit		= (GUIPushButton*)m_pWndMain->GetChild(_T("equ\\bagchoose\\bagchoose2"));					//装备继承PushButton
	m_pBtnClose				= (GUIButton*)m_pWndMain->GetChild(_T("equ\\closebutton"));
	//--------------------装备成长-----------------------
	m_pWndEquipGrow			= (GUIWnd*)m_pWndMain->GetChild(_T("equ\\back1\\k1"));										//装备成长面板
	m_pCStcGrowEquip		= (ComposeStatic*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\itemback\\item"));				//装备成长装备图标
	//m_pIStcQiSoulIcon		= (IconStatic*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\itemback1\\item"));					//器魂碎片图标
	//m_pLBoxQiSoul			= (GUIListBox*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\constellation\\numlist1"));			//器魂列表
	//m_pEBoxQiSoul			= (GUIEditBox*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\constellation\\num1"));
	//m_pEBoxQiSoul->SetUnClickable(TRUE);
	m_pIStcLuckSign			= (IconStatic*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\itemback2\\item"));					//幸运符图标
	m_pLBoxLuckSign			= (GUIListBox*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\constellation1\\numlist1"));		//幸运符列表
	m_pEBoxLuckSign			= (GUIEditBox*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\constellation1\\num1"));
	m_pEBoxLuckSign->SetUnClickable(TRUE);
	for(int i = 0; i < MAX_GROWATT_NUM; i++)
	{
		_stprintf(szText, _T("equ\\back1\\k1\\back\\word%d"), i);
		m_pStcEquipAtt[i] = (GUIStatic*)m_pWndMain->GetChild(szText);
	}
	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
	{
		_stprintf(szText, _T("equ\\back1\\k1\\word3\\pic%d"), i+1);
		m_pIStcGrowStar[i] = (IconStatic*)m_pWndMain->GetChild(szText);
	}
	m_pEBoxGrowNum			= (GUIEditBox*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\pic1\\num1"));
	m_pEBoxGrowNum->SetMaxValue(20);	//最多可输入20次~~
	m_pProGroExp			= (ActiveProgress*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\progpic\\progress"));
	m_pStcGrowExp			= (GUIStatic*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\word2"));
	m_pBtnBeginGrow			= (GUIButton*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\butt"));								//开始成长
	m_pBtnStarReduice		= (GUIButton*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\butt1"));
	m_pStcGrowTips			= (GUIStatic*)m_pWndMain->GetChild(_T("equ\\back1\\k1\\word8"));
	//--------------------装备继承-----------------------
	m_pWndEquipInherit		= (GUIWnd*)m_pWndMain->GetChild(_T("equ\\back1\\k2"));										//装备继承面板
	m_pCStcOriginalEquip	= (ComposeStatic*)m_pWndMain->GetChild(_T("equ\\back1\\k2\\back1\\itemback\\item"));		//原始装备（带成长属性的装备）
	m_pCStcDestEquip		= (ComposeStatic*)m_pWndMain->GetChild(_T("equ\\back1\\k2\\back2\\itemback\\item"));		//目标装备（要增加成长属性的装备）
	m_pIStcGrowInheritSign	= (IconStatic*)m_pWndMain->GetChild(_T("equ\\back1\\k2\\back2\\itemback1\\item"));			//成长继承符图标
	m_pLBoxGrowInheritSign	= (GUIListBox*)m_pWndMain->GetChild(_T("equ\\back1\\k2\\back2\\constellation\\numlist1"));	//成长继承符列表
	m_pEBoxGrowInheritSign	= (GUIEditBox*)m_pWndMain->GetChild(_T("equ\\back1\\k2\\back2\\constellation\\num1"));
	m_pEBoxGrowInheritSign->SetUnClickable(TRUE);
	for(int i = 0; i < MAX_GROWATT_NUM; i++)
	{
		_stprintf(szText, _T("equ\\back1\\k2\\back1\\back\\word%d"), i);
		m_pEBoxGrowInheritAtt[i] = (GUIStatic*)m_pWndMain->GetChild(szText);
	}
	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
	{
		_stprintf(szText, _T("equ\\back1\\k2\\back1\\word3\\pic%d"), i+1);
		m_pIStcInheritStar[i] = (IconStatic*)m_pWndMain->GetChild(szText);
	}
	m_pStcInheritExp		= (GUIStatic*)m_pWndMain->GetChild(_T("equ\\back1\\k2\\back1\\word2"));
	m_pBtnBeginInherit		= (GUIButton*)m_pWndMain->GetChild(_T("equ\\back1\\k2\\back2\\butt"));						//开始继承

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWndMain->GetFullName().c_str(), m_Trunk.sfp1(&EquipGrowFrame::EventHandler));

	m_pWndMain->SetInvisible(TRUE);

	return TRUE;
}

VOID EquipGrowFrame::Update()
{
	GameFrame::Update();
	if(m_pWndMain->IsInvisible())
		return;

	if(Kernel::Inst()->GetAccumTimeDW() - m_updateTimeDistance > 1000)//每隔1秒检测距离目标NPC的距离
	{
		if (RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID() ) )
		{
			Hide();
			return;
		}
	}
}

VOID EquipGrowFrame::OnEvent( tagGameEvent* pEvent )
{

}

DWORD EquipGrowFrame::EventHandler( tagGUIEvent* pEvent )
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
				Hide();
			}
			else if(pWnd == m_pBtnBeginGrow)	//发送成长消息
			{
				tagNC_EquipGrow cmd;
				//cmd.dwClips = m_dwEquipGrowItemID;
				cmd.dwIM = m_dwEquipGrowLuckSignID;
				cmd.n64Equip = m_n64GrowEquipID;
				cmd.dwGrowValue = _ttoi(m_pEBoxGrowNum->GetText());
				if(cmd.dwGrowValue != 0)
					m_pSession->Send(&cmd);
			}
			else if(pWnd == m_pBtnStarReduice)	//发送降星消息
			{
				tagNC_EquipGrowDec cmd;
				cmd.n64Equip = m_n64GrowEquipID;
				cmd.n64IM = ItemMgr::Inst()->GetSpecfuncItemID(EISF_EquipReduceIM);
				if(cmd.n64Equip != GT_INVALID && cmd.n64IM != GT_INVALID)
				{
					m_pSession->Send(&cmd);
				}
				else
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrowDec_EquipImInvalid")]);
				}

			}
			else if(pWnd == m_pBtnBeginInherit)	//发送继承消息
			{
				tagNC_EquipGrowTrans cmd;
				cmd.n64IM = m_n64EquipInheritID;
				cmd.n64NewEquip = m_n64DestEquipID;
				cmd.n64OldEquip = m_n64InheritEquipID;
				if(cmd.n64NewEquip != cmd.n64OldEquip)
					m_pSession->Send(&cmd);
				else
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrowTrans_EquipNotSame")]);
				}
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if(pWnd == m_pPBtnEquipGrow)
			{
				m_pWndEquipGrow->SetInvisible(FALSE);
				m_pWndEquipInherit->SetInvisible(TRUE);
				//FindQiSoulFromBag();
				FindGrowLuckSign();
				ClearUI();
				//默认添加第一个器魂碎片
// 				m_Tempit = m_mapQiHunItem.begin();
// 				if(m_Tempit != m_mapQiHunItem.end())
// 				{
// 					m_nListQiHunIndex = 0;
// 					DWORD dwItemTypeID = GetItemTypeIDByMap(m_mapQiHunItem, m_nListQiHunIndex);
// 					if(dwItemTypeID != GT_INVALID)
// 					{
// 						m_pIStcQiSoulIcon->SetPic(ItemMgr::Inst()->GetItemIconPath(dwItemTypeID));
// 						m_pWndMain->SetRefresh(TRUE);
// 						m_dwEquipGrowItemID = dwItemTypeID;
// 						m_pEBoxQiSoul->SetText(ItemProtoData::Inst()->FindItemDisplay(dwItemTypeID)->szName);
// 					}
// 					else
// 					{
// 						m_pIStcQiSoulIcon->SetPic(NullIconPatch);
// 						m_pWndMain->SetRefresh(TRUE);
// 						m_dwEquipGrowItemID = GT_INVALID;
// 						m_pEBoxQiSoul->SetText(_T(""));
// 					}
// 				}
			}
			else if(pWnd == m_pPBtnEquipInherit)
			{
				m_pWndEquipGrow->SetInvisible(TRUE);
				m_pWndEquipInherit->SetInvisible(FALSE);
				FindInheritSign();
				ClearUI();
				//默认添加第一个继承符
				m_Tempit = m_mapInheritItem.begin();
				if(m_Tempit != m_mapInheritItem.end())
				{
					m_nListInheritIndex = 0;
					INT64 n64ItemTypeID = GetItem64IDByMap(m_mapInheritItem, m_nListInheritIndex);
					DWORD dwItemTypeID = GetItemTypeIDByMap(m_mapInheritItem, m_nListInheritIndex);
					if(dwItemTypeID != GT_INVALID)
					{
						m_pIStcGrowInheritSign->SetPic(ItemMgr::Inst()->GetItemIconPath(dwItemTypeID));
						m_pWndMain->SetRefresh(TRUE);
						m_n64EquipInheritID = n64ItemTypeID;
						m_pEBoxGrowInheritSign->SetText(ItemProtoData::Inst()->FindItemDisplay(dwItemTypeID)->szName);
					}
					else
					{
						m_pIStcGrowInheritSign->SetPic(NullIconPatch);
						m_pWndMain->SetRefresh(TRUE);
						m_n64EquipInheritID = GT_INVALID;
						m_pEBoxGrowInheritSign->SetText(_T(""));
					}
				}
			}
		}
		break;
	case EGUIE_ItemClick:
		{
// 			if(pWnd == m_pLBoxQiSoul)
// 			{
// 				m_nListQiHunIndex = (INT)pEvent->dwParam1;
// 				m_pGUI->SetActive(m_pWndMain);
// 				DWORD dwItemTypeID = GetItemTypeIDByMap(m_mapQiHunItem, m_nListQiHunIndex);
// 				if(dwItemTypeID != GT_INVALID)
// 				{
// 					m_pIStcQiSoulIcon->SetPic(ItemMgr::Inst()->GetItemIconPath(dwItemTypeID));
// 					m_pWndMain->SetRefresh(TRUE);
// 					m_dwEquipGrowItemID = dwItemTypeID;
// 				}
// 				else
// 				{
// 					m_pIStcQiSoulIcon->SetPic(NullIconPatch);
// 					m_pWndMain->SetRefresh(TRUE);
// 					m_dwEquipGrowItemID = GT_INVALID;
// 					m_pEBoxQiSoul->SetText(_T(""));
// 				}
// 
// 			}
			//else 
			if(pWnd == m_pLBoxLuckSign)
			{
				m_nListLuckSignIndex = (INT)pEvent->dwParam1 - 1;	//第一个放无
				m_pGUI->SetActive(m_pWndMain);
				DWORD dwItemTypeID = GetItemTypeIDByMap(m_mapLuckSignItem, m_nListLuckSignIndex);
				if(dwItemTypeID != GT_INVALID)
				{
					m_pIStcLuckSign->SetPic(ItemMgr::Inst()->GetItemIconPath(dwItemTypeID));
					m_pWndMain->SetRefresh(TRUE);
					m_dwEquipGrowLuckSignID = dwItemTypeID;
				}
				else
				{
					m_pIStcLuckSign->SetPic(NullIconPatch);
					m_pWndMain->SetRefresh(TRUE);
					m_dwEquipGrowLuckSignID = GT_INVALID;
					m_pEBoxLuckSign->SetText(_T(""));
				}

			}
			else if(pWnd == m_pLBoxGrowInheritSign)
			{
				m_nListInheritIndex = (INT)pEvent->dwParam1;
				m_pGUI->SetActive(m_pWndMain);
				INT64 n64ItemTypeID = GetItem64IDByMap(m_mapInheritItem, m_nListInheritIndex);
				DWORD dwItemTypeID = GetItemTypeIDByMap(m_mapInheritItem, m_nListInheritIndex);
				if(dwItemTypeID != GT_INVALID)
				{
					m_pIStcGrowInheritSign->SetPic(ItemMgr::Inst()->GetItemIconPath(dwItemTypeID));
					m_pWndMain->SetRefresh(TRUE);
					m_n64EquipInheritID = n64ItemTypeID;
				}
				else
				{
					m_pIStcGrowInheritSign->SetPic(NullIconPatch);
					m_pWndMain->SetRefresh(TRUE);
					m_n64EquipInheritID = GT_INVALID;
					m_pEBoxGrowInheritSign->SetText(_T(""));
				}

			}
		}
		break;
	case EOnEventSendDropGUI_Item:
		{
			//拖动释放
			HandleDragStuff(pEvent);
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if(pWnd == m_pCStcGrowEquip)
			{
				Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64GrowEquipID);
				if(P_VALID(pItem))
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
				}
			}
			else if(pWnd == m_pCStcOriginalEquip)
			{
				Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64InheritEquipID);
				if(P_VALID(pItem))
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
				}
			}
			else if(pWnd == m_pCStcDestEquip)
			{
				Item* pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64DestEquipID);
				if(P_VALID(pItem))
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
				}
			}
// 			else if(pWnd == m_pIStcQiSoulIcon)
// 			{
// 				if(m_dwEquipGrowItemID != GT_INVALID)
// 				{
// 					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_dwEquipGrowItemID).c_str());
// 				}
// 			}
			else if(pWnd == m_pIStcLuckSign)
			{
				if(m_dwEquipGrowLuckSignID != GT_INVALID)
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_dwEquipGrowLuckSignID).c_str());
				}
			}
			else if(pWnd == m_pIStcGrowInheritSign)
			{
				if(m_n64EquipInheritID != GT_INVALID)
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_n64EquipInheritID).c_str());
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{

		}
		break;
	}

	return TRUE;
}

DWORD EquipGrowFrame::OnEventProduceAddItem(tagItemPosChangeExEvent* pEvent)
{
	if(m_pWndMain->IsInvisible())
		return 0;
	 
	if(pEvent->eTypeSrc != EICT_Bag)
		return 0;
 
	Item* pItem = ItemMgr::Inst()->GetConItem(pEvent->eTypeSrc, pEvent->n16IndexSrc);
	if(!P_VALID(pItem))
		return 0;

	if(!MIsEquipment(pEvent->dwTypeIDSrc ))
		return 0;

	if(!m_pWndEquipGrow->IsInvisible())
	{
		Equipment* pEquip = (Equipment*)pItem;
		if(pEquip->GetEquip()->dwGrowTypeID == 0 || pEquip->GetEquip()->dwGrowTypeID == GT_INVALID)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotGrowEquip")]);
			return 0;
		}
		m_n64GrowEquipID = pEquip->GetItemId();
		m_pCStcGrowEquip->SetPic(ItemMgr::Inst()->GetItemIconPath(pEquip->GetItemTypeID()));
	}
	RefreshInfo();

	m_pWndMain->SetRefresh(TRUE);
	return 0;
}

DWORD EquipGrowFrame::OnOpen_EquipGrow(tagGameEvent* pEvent)
{
	m_pPBtnEquipGrow->SetState(EGUIBS_PushDown);
	Show();

	return 0;
}

DWORD EquipGrowFrame::OnItemEvent_UpdateItem(tagUpdateLPItemEvent* pGameEvent)
{
	//FindQiSoulFromBag();
	FindGrowLuckSign();
	FindInheritSign();
	return 0;
}

VOID EquipGrowFrame::ClearUI()
{
	//装备成长清理
	m_pCStcGrowEquip->SetPic(NullIconPatch);
	for(int i = 0; i < MAX_GROWATT_NUM; i++)
	{
		m_pStcEquipAtt[i]->SetText(_T(""));
	}
	//m_pIStcQiSoulIcon->SetPic(NullIconPatch);
	m_pIStcLuckSign->SetPic(NullIconPatch);
	//m_pEBoxQiSoul->SetText(_T(""));
	m_pEBoxLuckSign->SetText(g_StrTable[_T("NULLText")]);
	m_pEBoxGrowNum->SetText(_T("1"));

	//装备继承清理
	m_pCStcOriginalEquip->SetPic(NullIconPatch);
	m_pCStcDestEquip->SetPic(NullIconPatch);
	m_pIStcGrowInheritSign->SetPic(NullIconPatch);
	m_pEBoxGrowInheritSign->SetText(_T(""));
	for(int i = 0; i < MAX_GROWATT_NUM; i++)
	{
		m_pEBoxGrowInheritAtt[i]->SetText(_T(""));
	}

	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
	{
		m_pIStcGrowStar[i]->SetPic(g_StrTable[_T("Star_0")]);
	}
	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
	{
		m_pIStcInheritStar[i]->SetPic(g_StrTable[_T("Star_0")]);
	}

	//清理数据
	m_nListQiHunIndex		= GT_INVALID;		//器魂选中第几项
	m_nListLuckSignIndex	= GT_INVALID;		//幸运符选中第几项
	m_nListInheritIndex		= GT_INVALID;		//继承符选中第几项

	m_n64GrowEquipID		= GT_INVALID;		//成长装备ID
	m_n64InheritEquipID		= GT_INVALID;		//继承装备ID，带成长属性的那件
	m_n64DestEquipID		= GT_INVALID;		//继承装备ID，后面那个..

	m_nListQiHunIndex		= GT_INVALID;	
	m_nListLuckSignIndex	= GT_INVALID;
	m_nListInheritIndex		= GT_INVALID;

	//m_dwEquipGrowItemID		= GT_INVALID;		//器魂碎片TypeID	
	m_dwEquipGrowLuckSignID = GT_INVALID;		//幸运符ID
	m_n64EquipInheritID		= GT_INVALID;		//装备继承道具ID


	m_pProGroExp->SetMaxValue(100);
	m_pProGroExp->SetValue(0,TRUE);
	m_pStcGrowExp->SetText(_T(""));

	m_pStcInheritExp->SetText(_T(""));

	m_pWndMain->SetRefresh(TRUE);
}

VOID EquipGrowFrame::ClearTransUI()
{
	//装备继承清理
	m_pCStcOriginalEquip->SetPic(NullIconPatch);
	for(int i = 0; i < MAX_GROWATT_NUM; i++)
	{
		m_pEBoxGrowInheritAtt[i]->SetText(_T(""));
	}
	for(int i = 0; i < EQUIP_GROW_MAX_LEVEL-1; i++)
	{
		m_pIStcInheritStar[i]->SetPic(g_StrTable[_T("Star_0")]);
	}
	m_pStcInheritExp->SetText(_T(""));
	//清理数据
	m_n64InheritEquipID		= GT_INVALID;		//继承装备ID，带成长属性的那件

	m_pWndMain->SetRefresh(TRUE);
}

VOID EquipGrowFrame::HandleDragStuff(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return ;

	if(!P_VALID(m_pWndMain))
		return;

	if(m_pWndMain->IsInvisible())
		return ;
	
	DWORD dwTypeID = pEvent->dwParam3;
	INT16 nPos = INT16(pEvent->dwParam2);
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(pEquip))
		return;

	if(!MIsEquipment(dwTypeID))
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotGrowEquip")]);
		return;
	}

	if(pWnd == m_pCStcGrowEquip)
	{
		if(pEquip->GetEquip()->dwGrowTypeID == 0 || pEquip->GetEquip()->dwGrowTypeID == GT_INVALID)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotGrowEquip")]);
			return;
		}
		m_pCStcGrowEquip->SetPic(ItemMgr::Inst()->GetItemIconPath(dwTypeID));
		m_n64GrowEquipID = pEquip->GetItemId();
	}
	else if(pWnd == m_pCStcOriginalEquip)
	{
		if(pEquip->GetEquip()->dwGrowTypeID == 0 || pEquip->GetEquip()->dwGrowTypeID == GT_INVALID)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NotGrowEquip")]);
			return;
		}
		if(pEquip->IsItemLocked())
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrow_CannotLockItem")]);
			return;
		}
		m_pCStcOriginalEquip->SetPic(ItemMgr::Inst()->GetItemIconPath(dwTypeID));
		m_n64InheritEquipID = pEquip->GetItemId();
	}
	else if(pWnd == m_pCStcDestEquip)
	{
		const tagEquipProto* pProto = ItemProtoData::Inst()->FindEquipProto(dwTypeID);
		if(P_VALID(pProto) && (pProto->eEquipPos >= EEP_MaxEquip && pProto->eEquipPos <= EEP_End))
		{
			//时装不可继承属性
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionCannotGrow")]);
		}
		else if(P_VALID(pProto) && pProto->bOnlyScriptStrengthen)
		{
			//该装备不能继承属性
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TimeEquipCannotGrow")]);
		}
		else
		{
			m_pCStcDestEquip->SetPic(ItemMgr::Inst()->GetItemIconPath(dwTypeID));
			m_n64DestEquipID = pEquip->GetItemId();
		}
	}
	RefreshInfo();
	m_pWndMain->SetRefresh(TRUE);
}

VOID EquipGrowFrame::Show()
{
	ClearUI();
	m_pWndMain->SetInvisible(FALSE);
	SetStrategy(EIST_ItemSmithCompose);
}

VOID EquipGrowFrame::Hide()
{
	m_pWndMain->SetInvisible(TRUE);
	SetStrategy(EIST_ItemPocket);
}

// VOID EquipGrowFrame::FindQiSoulFromBag()
// {
// 	m_listQiHunItem.clear();
// 	m_mapQiHunItem.clear();
// 	m_nListQiHunIndex = GT_INVALID;	
// 	m_pLBoxQiSoul->SetText(0, 0, _T(""));
// 	m_pLBoxQiSoul->Resize(tagPoint(100,19));
// 	m_pLBoxQiSoul->SetColNum(1, 100);
// 
// 	ItemContainer* pBagContainer = ItemMgr::Inst()->GetPocket();
// 	pBagContainer->GetSpecFuncMap(EISF_EquipGrowClips, m_listQiHunItem);
// 	int i = 0;
// 	for(i = 0, m_QlistiHunIt = m_listQiHunItem.begin(); m_QlistiHunIt != m_listQiHunItem.end(); ++m_QlistiHunIt, i++)
// 	{
// 		m_Tempit = m_mapQiHunItem.find((*m_QlistiHunIt)->GetItemTypeID());
// 		if(m_Tempit == m_mapQiHunItem.end())
// 		{
// 			m_mapQiHunItem.insert(make_pair((*m_QlistiHunIt)->GetItemTypeID(), (*m_QlistiHunIt)->GetItemId()));
// 		}
// 	}
// 	tstring strTemp;
// 	for(i = 0, m_Tempit = m_mapQiHunItem.begin(); m_Tempit != m_mapQiHunItem.end(); ++m_Tempit, i++)
// 	{
// 		const tagItemDisplayInfo* pItemInfo = ItemProtoData::Inst()->FindItemDisplay((*m_Tempit).first);
// 		if(P_VALID(pItemInfo))
// 		{
// 			m_pLBoxQiSoul->SetText(i, 0, pItemInfo->szName);
// 			m_pLBoxQiSoul->Resize(tagPoint(100,19*(i+1)));
// 			m_pLBoxQiSoul->SetColNum(i+1, 100);
// 		}
// 	}
// }

VOID EquipGrowFrame::FindGrowLuckSign()
{
	m_listQiHunItem.clear();
	m_mapLuckSignItem.clear();	
	m_nListLuckSignIndex = GT_INVALID;	

	ItemContainer* pBagContainer = ItemMgr::Inst()->GetPocket();
	pBagContainer->GetSpecFuncMap(EISF_EquipGrowLuckSign, m_listQiHunItem);
	int i = 0;
	for(i = 0, m_QlistiHunIt = m_listQiHunItem.begin(); m_QlistiHunIt != m_listQiHunItem.end(); ++m_QlistiHunIt, i++)
	{
		m_Tempit = m_mapLuckSignItem.find((*m_QlistiHunIt)->GetItemTypeID());
		if(m_Tempit == m_mapLuckSignItem.end())
		{
			m_mapLuckSignItem.insert(make_pair((*m_QlistiHunIt)->GetItemTypeID(), (*m_QlistiHunIt)->GetItemId()));
		}
	}
	tstring strTemp;
	//第一个放无
	m_pLBoxLuckSign->SetText(0, 0, g_StrTable[_T("NULLText")]);
	m_pLBoxLuckSign->Resize(tagPoint(100,19));
	m_pLBoxLuckSign->SetColNum(1, 100);
	for(i = 1, m_Tempit = m_mapLuckSignItem.begin(); m_Tempit != m_mapLuckSignItem.end(); ++m_Tempit, i++)
	{
		const tagItemDisplayInfo* pItemInfo = ItemProtoData::Inst()->FindItemDisplay((*m_Tempit).first);
		if(P_VALID(pItemInfo))
		{
			m_pLBoxLuckSign->SetText(i, 0, pItemInfo->szName);
			m_pLBoxLuckSign->Resize(tagPoint(100,19*(i+1)));
			m_pLBoxLuckSign->SetColNum(i+1, 100);
		}
	}
}

VOID EquipGrowFrame::FindInheritSign()
{
	m_listQiHunItem.clear();
	m_mapInheritItem.clear();
	m_nListInheritIndex = GT_INVALID;

	m_pLBoxGrowInheritSign->SetText(0, 0, _T(""));
	m_pLBoxGrowInheritSign->Resize(tagPoint(100,19));
	m_pLBoxGrowInheritSign->SetColNum(1, 100);

	ItemContainer* pBagContainer = ItemMgr::Inst()->GetPocket();
	//普通继承符
	pBagContainer->GetSpecFuncMap(EISF_EquipGrowTransIM, m_listQiHunItem);	
	int i = 0;
	for(i = 0, m_QlistiHunIt = m_listQiHunItem.begin(); m_QlistiHunIt != m_listQiHunItem.end(); ++m_QlistiHunIt, i++)
	{
		m_Tempit = m_mapInheritItem.find((*m_QlistiHunIt)->GetItemTypeID());
		if(m_Tempit == m_mapInheritItem.end())
		{
			m_mapInheritItem.insert(make_pair((*m_QlistiHunIt)->GetItemTypeID(), (*m_QlistiHunIt)->GetItemId()));
		}
	}
	//完美继承符
	m_listQiHunItem.clear();
	pBagContainer->GetSpecFuncMap(EISF_EquipGrowTransPIM, m_listQiHunItem);	
	for(i = 0, m_QlistiHunIt = m_listQiHunItem.begin(); m_QlistiHunIt != m_listQiHunItem.end(); ++m_QlistiHunIt, i++)
	{
		m_Tempit = m_mapInheritItem.find((*m_QlistiHunIt)->GetItemTypeID());
		if(m_Tempit == m_mapInheritItem.end())
		{
			m_mapInheritItem.insert(make_pair((*m_QlistiHunIt)->GetItemTypeID(), (*m_QlistiHunIt)->GetItemId()));
		}
	}
	tstring strTemp;
	for(i = 0, m_Tempit = m_mapInheritItem.begin(); m_Tempit != m_mapInheritItem.end(); ++m_Tempit, i++)
	{
		const tagItemDisplayInfo* pItemInfo = ItemProtoData::Inst()->FindItemDisplay((*m_Tempit).first);
		if(P_VALID(pItemInfo))
		{
			m_pLBoxGrowInheritSign->SetText(i, 0, pItemInfo->szName);
			m_pLBoxGrowInheritSign->Resize(tagPoint(100,19*(i+1)));
			m_pLBoxGrowInheritSign->SetColNum(i+1, 100);
		}
	}
}

DWORD EquipGrowFrame::GetItemTypeIDByMap(map<DWORD, INT64>& DestMap, INT nIndex)
{
	int i = 0;
	for( i = 0, m_Tempit = DestMap.begin(); m_Tempit!= DestMap.end(); ++m_Tempit,i++)
	{
		if(i == nIndex)
		{
			return (*m_Tempit).first;
		}
	}
	return GT_INVALID;
}

INT64 EquipGrowFrame::GetItem64IDByMap(map<DWORD, INT64>& DestMap, INT nIndex)
{
	int i = 0;
	for( i = 0, m_Tempit = DestMap.begin(); m_Tempit!= DestMap.end(); ++m_Tempit,i++)
	{
		if(i == nIndex)
		{
			return (*m_Tempit).second;
		}
	}
	return GT_INVALID;
}

VOID EquipGrowFrame::RefreshInfo()
{
	TCHAR szText[X_LONG_NAME] = {0};
	Equipment*pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64GrowEquipID);
	if(P_VALID(pEquip))
	{
		const tagEquipSpec* pSpec = pEquip->GetEquip();

		//刷新星星信息
		for(int i = 0; i < EQUIP_GROW_MAX_LEVEL - 1; i++)
		{
			_stprintf(szText, _T("Star_%d"), pSpec->eGrowStar[i]);
			m_pIStcGrowStar[i]->SetPic(g_StrTable[szText]);
		}

		//刷新经验条
		const tagEquipGrowName* pProto = ItemProtoData::Inst()->FindEquipGrowProto(pSpec->dwGrowTypeID);
		if(P_VALID(pProto))
		{
			BYTE byEquipGrowLevel = ItemProtoData::Inst()->GetEquipGrowLevel(pSpec->dwGrowValue, pProto);
			DWORD dwEquipGrowCurMaxExp = pProto->dwExp[byEquipGrowLevel];
			_stprintf(szText, _T("%s%d/%d"), g_StrTable[_T("EquipGrow")], pSpec->dwGrowValue - ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel-1, pProto), dwEquipGrowCurMaxExp);
			m_pProGroExp->SetMaxValue(dwEquipGrowCurMaxExp);
			m_pProGroExp->SetValue(pSpec->dwGrowValue - ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel-1, pProto), TRUE);
			m_pStcGrowExp->SetText(szText);

			//设置提升当前等级所需要的经验
			DWORD dwValue = dwEquipGrowCurMaxExp - (pSpec->dwGrowValue - ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel-1, pProto));
			TCHAR szTextValue[128] = {0};
			_stprintf(szTextValue, g_StrTable[_T("GrowTip")], dwValue);
			m_pStcGrowTips->SetText(szTextValue);
			INT64 n64BagSilver = CurrencyMgr::Inst()->GetBagSilver();
			INT64 n64BagLimitExpValue = n64BagSilver/1500;	//根据钱判断最大提升值
			INT64 n64EquipLimitValue = ItemProtoData::Inst()->GetEquipGrowCurMaxExp(10, pProto) - pSpec->dwGrowValue;
			if(n64BagSilver < dwValue*1500)	//背包钱不够
			{
				dwValue = n64BagLimitExpValue;	//调整
			}
			_stprintf(szTextValue, _T("%d"), dwValue);
			m_pEBoxGrowNum->SetText(szTextValue);	//默认升级所需经验，如果金币不够则判断最大经验数
			m_pEBoxGrowNum->SetMaxValue(n64BagLimitExpValue<n64EquipLimitValue ? n64BagLimitExpValue : n64EquipLimitValue);
		}
		//刷新恶心的属性
		DWORD dwTempAttValue = 0;
		DWORD dwTempMaxAttValue = 0;
		float fTempCurRatio = 0.0f;
		for(int i = 0; i < EQUIP_GROW_ADD_ATT_MAX_NUM; i++)
		{
			if(ERA_Null == pProto->eAtt[i])	//没有属性
			{
				m_pStcEquipAtt[i]->SetText(_T(""));
				continue;
			}

			dwTempAttValue	= 0;
			dwTempMaxAttValue = 0;
			fTempCurRatio	= 0.0f;

			BYTE byEquipGrowLevel = ItemProtoData::Inst()->GetEquipGrowLevel(pSpec->dwGrowValue, pProto);	//获得当前成长等级

			if(pProto->byAttOpenLevel[i] > byEquipGrowLevel+1)	//未开启,显示0/0
			{
				DWORD dwTempMaxAttValue0 = 0;
				for(int j = 0; j < EQUIP_GROW_MAX_LEVEL; j++)	//最大值还是要显示
				{
					dwTempMaxAttValue0 += pProto->dwAttValue[i][j];
				}
				_stprintf(szText, _T("ERoleAttribute_%d"), pProto->eAtt[i]);
				_stprintf(szText, _T("%s +0(%d)"), g_StrTable[szText], dwTempMaxAttValue0);
				m_pStcEquipAtt[i]->SetText(szText);
				continue;	//跳出去
			}

			for(int j = 1; j < byEquipGrowLevel; j++)	//累加前面几个星星的属性数据值，剩下最后一个单独处理
			{
				MGetEquipGrowRatio(pSpec->eGrowStar[j-1], fTempCurRatio);
				dwTempAttValue += pProto->dwAttValue[i][j] * fTempCurRatio;
			}
			DWORD dwEquipGrowCurMaxExp	= ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel, pProto);
			DWORD dwEquipGrowLastMaxExp = ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel - 1, pProto);
			if(byEquipGrowLevel != 0)
			{

				dwTempAttValue += pProto->dwAttValue[i][0];	//第一个数据不成星星倍率

				MGetEquipGrowRatio(pSpec->eGrowStar[byEquipGrowLevel-1], fTempCurRatio);
				dwTempMaxAttValue = dwTempAttValue + pProto->dwAttValue[i][byEquipGrowLevel] * fTempCurRatio;
				dwTempAttValue += pProto->dwAttValue[i][byEquipGrowLevel] * fTempCurRatio * (pSpec->dwGrowValue-ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel-1, pProto)) / pProto->dwExp[byEquipGrowLevel];	//最后一个数值计算公式：本阶段属性加成值*星星倍率*（本阶段最大经验值 - 上一阶段最大经验值）/本阶段最大经验值
			}
			else
			{
				dwTempMaxAttValue = pProto->dwAttValue[i][byEquipGrowLevel];
				dwTempAttValue += pProto->dwAttValue[i][byEquipGrowLevel] * (pSpec->dwGrowValue) / (ItemProtoData::Inst()->GetEquipGrowCurMaxExp(0, pProto));	//最后一个数值计算公式：本阶段属性加成值*星星倍率*（本阶段最大经验值 - 上一阶段最大经验值）/本阶段最大经验值
			}

			for(int j = byEquipGrowLevel+1; j < EQUIP_GROW_MAX_LEVEL; j++)
			{
				dwTempMaxAttValue += pProto->dwAttValue[i][j];
			}

			_stprintf(szText, _T("ERoleAttribute_%d"), pProto->eAtt[i]);
			_stprintf(szText, _T("%s +%d(%d)"), g_StrTable[szText], dwTempAttValue, dwTempMaxAttValue);
			m_pStcEquipAtt[i]->SetText(szText);
		}
	}

	pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_n64InheritEquipID);
	if(P_VALID(pEquip))
	{
		const tagEquipSpec* pSpec = pEquip->GetEquip();

		//刷新继承小星星
		for(int i = 0; i < EQUIP_GROW_MAX_LEVEL - 1; i++)
		{
			_stprintf(szText, _T("Star_%d"), pSpec->eGrowStar[i]);
			m_pIStcInheritStar[i]->SetPic(g_StrTable[szText]);
		}

		//刷新经验条
		const tagEquipGrowName* pProto = ItemProtoData::Inst()->FindEquipGrowProto(pSpec->dwGrowTypeID);
		if(P_VALID(pProto))
		{
			BYTE byEquipGrowLevel = ItemProtoData::Inst()->GetEquipGrowLevel(pSpec->dwGrowValue, pProto);
			DWORD dwEquipGrowCurMaxExp = pProto->dwExp[byEquipGrowLevel];
			_stprintf(szText, _T("%s%d/%d"), g_StrTable[_T("EquipGrow")], pSpec->dwGrowValue - ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel-1, pProto), dwEquipGrowCurMaxExp);
			m_pStcInheritExp->SetText(szText);
		}

		//刷新恶心的属性
		DWORD dwTempAttValue = 0;
		DWORD dwTempMaxAttValue = 0;
		float fTempCurRatio = 0.0f;
		for(int i = 0; i < EQUIP_GROW_ADD_ATT_MAX_NUM; i++)
		{
			if(ERA_Null == pProto->eAtt[i])	//没有属性
			{
				m_pEBoxGrowInheritAtt[i]->SetText(_T(""));
				continue;
			}

			dwTempAttValue	= 0;
			dwTempMaxAttValue = 0;
			fTempCurRatio	= 0.0f;

			BYTE byEquipGrowLevel = ItemProtoData::Inst()->GetEquipGrowLevel(pSpec->dwGrowValue, pProto);	//获得当前成长等级

			if(pProto->byAttOpenLevel[i] > byEquipGrowLevel+1)	//未开启,显示0/0
			{
				DWORD dwTempMaxAttValue0 = 0;
				for(int j = 0; j < EQUIP_GROW_MAX_LEVEL; j++)	//最大值还是要显示
				{
					dwTempMaxAttValue0 += pProto->dwAttValue[i][j];
				}
				_stprintf(szText, _T("ERoleAttribute_%d"), pProto->eAtt[i]);
				_stprintf(szText, _T("%s +0(%d)"), g_StrTable[szText], dwTempMaxAttValue0);
				m_pEBoxGrowInheritAtt[i]->SetText(szText);
				continue;	//跳出去
			}

			for(int j = 1; j < byEquipGrowLevel; j++)	//累加前面几个星星的属性数据值，剩下最后一个单独处理
			{
				MGetEquipGrowRatio(pSpec->eGrowStar[j-1], fTempCurRatio);
				dwTempAttValue += pProto->dwAttValue[i][j] * fTempCurRatio;
			}
			DWORD dwEquipGrowCurMaxExp	= ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel, pProto);
			DWORD dwEquipGrowLastMaxExp = ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel - 1, pProto);
			if(byEquipGrowLevel != 0)
			{

				dwTempAttValue += pProto->dwAttValue[i][0];	//第一个数据不成星星倍率

				MGetEquipGrowRatio(pSpec->eGrowStar[byEquipGrowLevel-1], fTempCurRatio);
				dwTempMaxAttValue = dwTempAttValue + pProto->dwAttValue[i][byEquipGrowLevel] * fTempCurRatio;
				dwTempAttValue += pProto->dwAttValue[i][byEquipGrowLevel] * fTempCurRatio * (pSpec->dwGrowValue-ItemProtoData::Inst()->GetEquipGrowCurMaxExp(byEquipGrowLevel-1, pProto)) / pProto->dwExp[byEquipGrowLevel];	//最后一个数值计算公式：本阶段属性加成值*星星倍率*（本阶段最大经验值 - 上一阶段最大经验值）/本阶段最大经验值
			}
			else
			{
				dwTempMaxAttValue = pProto->dwAttValue[i][byEquipGrowLevel];
				dwTempAttValue += pProto->dwAttValue[i][byEquipGrowLevel] * (pSpec->dwGrowValue) / (ItemProtoData::Inst()->GetEquipGrowCurMaxExp(0, pProto));	//最后一个数值计算公式：本阶段属性加成值*星星倍率*（本阶段最大经验值 - 上一阶段最大经验值）/本阶段最大经验值
			}

			for(int j = byEquipGrowLevel+1; j < EQUIP_GROW_MAX_LEVEL; j++)
			{
				dwTempMaxAttValue += pProto->dwAttValue[i][j];
			}

			_stprintf(szText, _T("ERoleAttribute_%d"), pProto->eAtt[i]);
			_stprintf(szText, _T("%s +%d(%d)"), g_StrTable[szText], dwTempAttValue, dwTempMaxAttValue);
			m_pEBoxGrowInheritAtt[i]->SetText(szText);
		}
	}

}

DWORD EquipGrowFrame::OnNS_EquipGrow(tagNS_EquipGrow* pCmd, DWORD)
{
	if(pCmd->dwErrorCode != E_Success)
	{
		ShowErrorCode(pCmd->dwErrorCode);
	}
	else
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
		RefreshInfo();
	}
	
	return 0;
}

DWORD EquipGrowFrame::OnNS_EquipGrowTrans(tagNS_EquipGrowTrans* pCmd, DWORD)
{
	if(pCmd->dwErrorCode != E_Success)
	{
		ShowErrorCode(pCmd->dwErrorCode);
		ClearUI();
	}
	else
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
		ClearTransUI();
	}

	return 0;
}

DWORD EquipGrowFrame::OnNS_EquipGrowDec(tagNS_EquipGrowDec* pCmd, DWORD)
{
	if(pCmd->dwErrorCode != E_Success)
	{
		ShowErrorCode(pCmd->dwErrorCode);
	}
	else
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
	}
	RefreshInfo();

	return 0;
}

VOID EquipGrowFrame::ShowErrorCode(DWORD dwErrorCode)
{
	switch(dwErrorCode)
	{
	case E_EquipGrow_ClipsInvalid: // 器魂碎片无效
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrow_ClipsInvalid")]);
		break;
	case E_EquipGrow_TimesInvalid: // 输入的成长次数不合法
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrow_TimesInvalid")]);
		break;
	case E_EquipGrow_ReachTop: // 无法继续成长
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrow_ReachTop")]);
		break;
	case E_EquipGrow_Error: // 发生未知错误
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrow_Error")]);
		break;
	case E_EquipGrow_EquipInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrow_EquipInvalid")]);
		break;
	case E_EquipGrowTrans_OldEquipInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrowTrans_OldEquipInvalid")]);
		break;
	case E_EquipGrowTrans_NewEquipInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrowTrans_NewEquipInvalid")]);
		break;
	case E_EquipGrowTrans_PositionNoMatch:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrowTrans_PositionNoMatch")]);
		break;
	case E_EquipGrowTrans_NewEquipLevelLimit:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrowTrans_NewEquipLevelLimit")]);
		break;
	case E_EquipGrowTrans_IMInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrowTrans_IMInvalid")]);
		break;
	case E_EquipGrowTrans_Error:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrowTrans_Error")]);
		break;
	case E_EquipGrowDec_EquipInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrowDec_EquipInvalid")]);
		break;
	case E_EquipGrowDec_IMInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipGrowDec_IMInvalid")]);
		break;
	}
}