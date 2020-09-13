#include "stdafx.h"
#include "IDBuyBackFrame.h"
#include "ItemFrame.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "AudioSys.h"
#include "ItemProtoData.h"
#include "ItemMgr.h"
#include "ItemProtoData.h"
#include "..\WorldDefine\msg_item.h"
#include "RoleEvent.h"


IDButBackFrame::IDButBackFrame():m_Trunk(this)
{
	m_pWndLiLianDu = NULL;									//带完成度
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		m_pIStcItemIcon[i] = NULL;							//物品图标
		m_pStcItemName[i] = NULL;							//物品名称
	}

	m_pProProcess = NULL;	
	for(int i = 0; i < MAX_GIFT_ITEM_SHOW_NUM; i++)			//进度条	
	{
		m_pIStcItemShowIcon[i] = NULL;						//显示完成
		m_pStcItemShowName[i] = NULL;						//
		m_pStcDesc[i] = NULL;								//物品描述
	}

	m_pBtndLiLianDuClose = NULL;							//
	m_pStcCompleteDesc = NULL;								//完成度描

	m_pWndReceiveGift = NULL;								//领取奖励
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		m_pIStcCurItem[i] = NULL;							//当前奖励
		m_pStcCurItemName[i] = NULL;						//当前奖励
		m_pIStcNextItem[i] =NULL;							//下个奖励
		m_pStcNextItemName[i] = NULL;						//下个奖励
	}

	m_pBtnOk = NULL;										//确定领取
	m_pBtnReceiveClose = NULL;								//关闭

	m_pWndIDbuyBack;										//账号回收
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		m_pIStcBuyBackItemIcon[i] = NULL;					//物品图标
		m_pStcBuyBackItemName[i] = NULL;					//物品名称
	}

	m_pBtnBuyBackOk = NULL;									//确认收回
	m_pBtnBuyBackReceiveGigt = NULL;						//领取礼品
	m_pBtnBuyThink = NULL;									//再想想~
	m_pBtnBuyClose = NULL;									//关闭按钮
	for(int i = 0; i < MAX_GIFT_ITEM_SHOW_NUM; i++)
	{
		m_pIStcBuyBackShowItem[i] = NULL;		
		m_pIStcBuyBackShowItemName[i] = NULL;
		m_pStcButBackDesc[i] = NULL;
	}


	m_pWndHaveReceive = NULL;								//重复领取
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		m_pIStcHaveReceiveItemIcon[i] = NULL;
	}
	m_pBtnHaveReceiveClose = NULL;							//关闭按钮
	m_pBtnHaveReceiveOK = NULL;								//确定按钮

	m_dwCurLiLianExp = 0;	
	m_nGetGiftNum = 0;		
	m_dwMaxExp = 0;

	m_eOpenWhictFrame = EFrameNULL;

}

IDButBackFrame::~IDButBackFrame()
{

}

BOOL IDButBackFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	//--注册游戏事件
	m_pMgr->RegisterEventHandle(_T("ViewLiLian"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&IDButBackFrame::OnViewLiLian));
	m_pMgr->RegisterEventHandle(_T("ReceiveGift"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&IDButBackFrame::OnReceiveGift));
	m_pMgr->RegisterEventHandle(_T("IDBuyBack"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&IDButBackFrame::OnIDBuyBack));
	m_pMgr->RegisterEventHandle(_T("HaveReceive"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&IDButBackFrame::OnHaveReceive));

	TObjRef<NetCmdMgr>()->Register("NS_GetLiLianExp",	(NETMSGPROC)m_Trunk.sfp2(&IDButBackFrame::OnNS_GetLiLianExp),	_T("NS_GetLiLianExp"));
	TObjRef<NetCmdMgr>()->Register("NS_ReceiveGift",	(NETMSGPROC)m_Trunk.sfp2(&IDButBackFrame::OnNS_ReceiveGift),	_T("NS_ReceiveGift"));
	TObjRef<NetCmdMgr>()->Register("NS_LiLianChange",	(NETMSGPROC)m_Trunk.sfp2(&IDButBackFrame::OnNS_LiLianChange),	_T("NS_LiLianChange"));

	m_IDBuyBackEx = ItemProtoData::Inst()->GetIDBuyBackEx();
	const tagIDBuyBack* pMaxIDBuyBack = ItemProtoData::Inst()->FindIDBuyBack(100);
	m_dwMaxExp = pMaxIDBuyBack->dwExp;
	return TRUE;
}

BOOL IDButBackFrame::EscCancel()
{
	BOOL bRet = FALSE;
	if(!m_pWndLiLianDu->IsInvisible())
	{
		m_pWndLiLianDu->SetInvisible(TRUE);
		bRet = TRUE;
	}

	if(!m_pWndIDbuyBack->IsInvisible())
	{
		m_pWndIDbuyBack->SetInvisible(TRUE);
		bRet = TRUE;
	}

	if(!m_pWndHaveReceive->IsInvisible())
	{
		m_pWndHaveReceive->SetInvisible(TRUE);
		bRet = TRUE;
	}

	if(!m_pWndReceiveGift->IsInvisible())
	{
		m_pWndReceiveGift->SetInvisible(TRUE);
	}

	return bRet;
}

BOOL IDButBackFrame::Destroy()
{
	GameFrame::Destroy();

	if( P_VALID(m_pWndLiLianDu) )
	{
		m_pGUI->AddToDestroyList(m_pWndLiLianDu);
		m_pWndLiLianDu = NULL;
	}
	if( P_VALID(m_pWndIDbuyBack) )
	{
		m_pGUI->AddToDestroyList(m_pWndIDbuyBack);
		m_pWndIDbuyBack = NULL;
	}
	if( P_VALID(m_pWndHaveReceive) )
	{
		m_pGUI->AddToDestroyList(m_pWndHaveReceive);
		m_pWndHaveReceive = NULL;
	}
	if( P_VALID(m_pWndReceiveGift) )
	{
		m_pGUI->AddToDestroyList(m_pWndReceiveGift);
		m_pWndReceiveGift = NULL;
	}


	//--注销游戏事件
	m_pMgr->UnRegisterEventHandler(_T("ViewLiLian"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&IDButBackFrame::OnViewLiLian));
	m_pMgr->UnRegisterEventHandler(_T("ReceiveGift"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&IDButBackFrame::OnReceiveGift));
	m_pMgr->UnRegisterEventHandler(_T("IDBuyBack"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&IDButBackFrame::OnIDBuyBack));
	m_pMgr->UnRegisterEventHandler(_T("HaveReceive"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&IDButBackFrame::OnHaveReceive));

	TObjRef<NetCmdMgr>()->UnRegister("NS_GetLiLianExp",	(NETMSGPROC)m_Trunk.sfp2(&IDButBackFrame::OnNS_GetLiLianExp));
	TObjRef<NetCmdMgr>()->UnRegister("NS_ReceiveGift",	(NETMSGPROC)m_Trunk.sfp2(&IDButBackFrame::OnNS_ReceiveGift));
	TObjRef<NetCmdMgr>()->UnRegister("NS_LiLianChange", (NETMSGPROC)m_Trunk.sfp2(&IDButBackFrame::OnNS_LiLianChange));

	return TRUE;
}

BOOL IDButBackFrame::ReloadUI()
{
	XmlElement element;
	TCHAR szText[128] = {0};
	char cbuff[128];
	//第一个界面
	tstring strPath = g_strLocalPath + _T("\\ui\\npctsk_1.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),"npctsk_1\\npctsk_1\\yi\\itemback%d\\item",i);
		m_pGUI->ChangeXml(&element, cbuff, "ClassName", "IconStatic");
	}
	for(int i = 0; i < MAX_GIFT_ITEM_SHOW_NUM; i++)
	{
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),"\\desktop\\npctsk_1\\npctsk_1\\er\\path%d\\itemback0\\item",i+1);
		m_pGUI->ChangeXml(&element, cbuff, "ClassName", "IconStatic");
	}
	m_pGUI->ChangeXml(&element, "npctsk_1\\npctsk_1\\yi\\timepic\\time", "ClassName", "ActiveProgress");
	m_pGUI->ChangeXml(&element, "npctsk_1\\npctsk_1\\yi\\timepic1\\time", "ClassName", "ActiveProgress");

	m_pWndLiLianDu = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		_stprintf(szText, _T("npctsk_1\\yi\\itemback%d\\item"), i);
		m_pIStcItemIcon[i] = (IconStatic*)m_pWndLiLianDu->GetChild(szText);			//物品图标
		m_pIStcItemIcon[i]->SetPic(NullIconPatch);
		_stprintf(szText, _T("npctsk_1\\yi\\word%d"), i+1);
		m_pStcItemName[i] = (GUIStatic*)m_pWndLiLianDu->GetChild(szText);			//物品名称
		m_pStcItemName[i]->SetText(_T(""));
	}
	m_pProProcess = (ActiveProgress*)m_pWndLiLianDu->GetChild(_T("npctsk_1\\yi\\timepic1\\time"));								//总历练度
	m_pProCurProcess = (ActiveProgress*)m_pWndLiLianDu->GetChild(_T("npctsk_1\\yi\\timepic\\time"));							//当前历练度
	for(int  i = 0; i < MAX_GIFT_ITEM_SHOW_NUM; i++)
	{
		_stprintf(szText, _T("npctsk_1\\er\\path%d\\itemback0\\item"), i+1);			 
		m_pIStcItemShowIcon[i] = (IconStatic*)m_pWndLiLianDu->GetChild(szText);//显示完成度高的奖励
		_stprintf(szText, _T("npctsk_1\\er\\path%d\\word1"), i+1);
		m_pStcItemShowName[i] = (GUIStatic*)m_pWndLiLianDu->GetChild(szText);	//
		_stprintf(szText, _T("npctsk_1\\er\\path%d\\word2"), i+1);
		m_pStcDesc[i] = (GUIStatic*)m_pWndLiLianDu->GetChild(szText);			//物品描述
	}
	m_pBtndLiLianDuClose = (GUIButton*)m_pWndLiLianDu->GetChild(_T("npctsk_1\\close"));								//关闭按钮
	m_pStcCompleteDesc = (GUIStatic*)m_pWndLiLianDu->GetChild(_T("npctsk_1\\yi\\timepic1\\word"));							//完成度描述
	m_pStcCurCompleteDesc = (GUIStatic*)m_pWndLiLianDu->GetChild(_T("npctsk_1\\yi\\timepic\\word"));

	//第二个界面
	tstring strPath1 = g_strLocalPath + _T("\\ui\\npctsk_2.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath1.c_str());
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),"npctsk_2\\yi\\itemback%d\\item",i);
		m_pGUI->ChangeXml(&element, cbuff, "ClassName", "IconStatic");
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),"npctsk_2\\er\\itemback%d\\item",i);
		m_pGUI->ChangeXml(&element, cbuff, "ClassName", "IconStatic");
	}

	m_pWndReceiveGift = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		_stprintf(szText, _T("npctsk_2\\yi\\itemback%d\\item"), i);
		m_pIStcCurItem[i] = (IconStatic*)m_pWndReceiveGift->GetChild(szText);			//当前奖励物品图标
		_stprintf(szText, _T("npctsk_2\\yi\\word%d"), i+1);
		m_pStcCurItemName[i] = (GUIStatic*)m_pWndReceiveGift->GetChild(szText);			//当前奖励物品名字
		_stprintf(szText, _T("npctsk_2\\er\\itemback%d\\item"), i);
		m_pIStcNextItem[i] = (IconStatic*)m_pWndReceiveGift->GetChild(szText);			//下个奖励物品图标
		_stprintf(szText, _T("npctsk_2\\er\\word%d"), i+1);
		m_pStcNextItemName[i] = (GUIStatic*)m_pWndReceiveGift->GetChild(szText);			//下个奖励物品名字
	}
	m_pBtnOk = (GUIButton*)m_pWndReceiveGift->GetChild(_T("npctsk_2\\yi\\encbutt"));									//确定领取
	m_pBtnReceiveClose = (GUIButton*)m_pWndReceiveGift->GetChild(_T("npctsk_2\\close"));							//关闭

	//第三个界面
	tstring strPath2 = g_strLocalPath + _T("\\ui\\npctsk_3.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath2.c_str());
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),"npctsk_3\\npctsk_3\\yi\\itemback%d\\item",i);
		m_pGUI->ChangeXml(&element, cbuff, "ClassName", "IconStatic");
	}
	for(int i = 0; i < MAX_GIFT_ITEM_SHOW_NUM; i++)
	{
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char), "npctsk_3\\npctsk_3\\er\\path%d\\itemback0\\item", i);
	}

	m_pWndIDbuyBack = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		_stprintf(szText, _T("npctsk_3\\yi\\itemback%d\\item"), i);
		m_pIStcBuyBackItemIcon[i] = (IconStatic*)m_pWndIDbuyBack->GetChild(szText);	//物品图标
		m_pIStcBuyBackItemIcon[i]->SetPic(NullIconPatch);
		_stprintf(szText, _T("npctsk_3\\yi\\word%d"), i+1);
		m_pStcBuyBackItemName[i] = (GUIStatic*)m_pWndIDbuyBack->GetChild(szText);	//物品名称
		m_pStcBuyBackItemName[i]->SetText(_T(""));
	}
	m_pBtnBuyBackOk = (GUIButton*)m_pWndIDbuyBack->GetChild(_T("npctsk_3\\yi\\butt1"));							//确认收回账号
	m_pBtnBuyBackReceiveGigt = (GUIButton*)m_pWndIDbuyBack->GetChild(_T("npctsk_3\\yi\\butt2"));					//领取礼品
	m_pBtnBuyThink = (GUIButton*)m_pWndIDbuyBack->GetChild(_T("npctsk_3\\yi\\butt3"));								//再想想~
	m_pBtnBuyClose = (GUIButton*)m_pWndIDbuyBack->GetChild(_T("npctsk_3\\close"));								//关闭按钮
	for(int i = 0; i < MAX_GIFT_ITEM_SHOW_NUM; i++)
	{
		_stprintf(szText, _T("npctsk_3\\er\\path%d\\itemback0\\item"), i+1);
		m_pIStcBuyBackShowItem[i] = (IconStatic*)m_pWndIDbuyBack->GetChild(szText);	
		_stprintf(szText, _T("npctsk_3\\er\\path%d\\word1"), i+1);
		m_pIStcBuyBackShowItemName[i] = (GUIStatic*)m_pWndIDbuyBack->GetChild(szText);
		_stprintf(szText, _T("npctsk_3\\er\\path%d\\word2"), i+1);
		m_pStcButBackDesc[i] = (GUIStatic*)m_pWndIDbuyBack->GetChild(szText);
	}
	//第四个界面
	tstring strPath3 = g_strLocalPath + _T("\\ui\\npctsk_4.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath3.c_str());
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),"npctsk_4\\itemback%d\\item",i);
		m_pGUI->ChangeXml(&element, cbuff, "ClassName", "IconStatic");
	}
	m_pWndHaveReceive = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
	{
		_stprintf(szText, _T("npctsk_4\\itemback%d\\item"), i);
		m_pIStcHaveReceiveItemIcon[i] = (IconStatic*)m_pWndHaveReceive->GetChild(szText);	//下一阶段奖励图标..
		_stprintf(szText, _T("npctsk_4\\word%d"), i+1);
		m_pStcReceiveItemName[i] = (GUIStatic*)m_pWndHaveReceive->GetChild(szText);
	}
	m_pBtnHaveReceiveClose = (GUIButton*)m_pWndHaveReceive->GetChild(_T("npctsk_4\\close"));						//关闭按钮
	m_pBtnHaveReceiveOK = (GUIButton*)m_pWndHaveReceive->GetChild(_T("npctsk_4\\butt"));						//确定按钮
	m_pStcHaveReceiveTips = (GUIStatic*)m_pWndHaveReceive->GetChild(_T("npctsk_4\\word0"));

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWndLiLianDu->GetFullName().c_str(), m_Trunk.sfp1(&IDButBackFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndReceiveGift->GetFullName().c_str(), m_Trunk.sfp1(&IDButBackFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndIDbuyBack->GetFullName().c_str(), m_Trunk.sfp1(&IDButBackFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndHaveReceive->GetFullName().c_str(), m_Trunk.sfp1(&IDButBackFrame::EventHandler));

	CloseAllWnd();

	return TRUE;
}

VOID IDButBackFrame::Update()
{
	GameFrame::Update();
}

VOID IDButBackFrame::OnEvent( tagGameEvent* pEvent )
{

}

DWORD IDButBackFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pBtndLiLianDuClose)
			{
				m_pWndLiLianDu->SetInvisible(TRUE);
			}
			else if(pWnd == m_pBtnReceiveClose)
			{
				m_pWndReceiveGift->SetInvisible(TRUE);
			}
			else if(pWnd == m_pBtnOk)
			{
				int nLevel = ItemProtoData::Inst()->GetIDBuyBackLevel(m_dwCurLiLianExp);
				if( (nLevel/10) <= m_nGetGiftNum )
				{
					m_pFrameMgr->SendEvent(&tagGameEvent(_T("HaveReceive"), NULL));
				}
				else
				{
					tagNC_ReceiveGift cmd;
					m_pSession->Send(&cmd);
				}

			}
			else if(pWnd == m_pBtnBuyClose || pWnd == m_pBtnBuyThink)
			{
				m_pWndIDbuyBack->SetInvisible(TRUE);
			}
			else if(pWnd == m_pBtnBuyBackReceiveGigt)
			{
				m_pFrameMgr->SendEvent(&tagGameEvent(_T("ReceiveGift"), NULL));
			}
			else if(pWnd == m_pBtnBuyBackOk)
			{
				ShellExecute(NULL, _T("open"), _T("http://sm.kongzhong.com"), NULL, NULL, SW_MAXIMIZE);
			}
			else if(pWnd == m_pBtnHaveReceiveClose || m_pBtnHaveReceiveOK)
			{
				m_pWndHaveReceive->SetInvisible(TRUE);
			}
		}
		break;
	case EOnEventSendDropGUI_Item:
		{

		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			int nLevel = ItemProtoData::Inst()->GetIDBuyBackLevel(m_dwCurLiLianExp);
			int nCurGiftID = 0, nNextGiftID = 0;
			if(nLevel < m_nGetGiftNum*10)
			{
				nCurGiftID = (m_nGetGiftNum-1)*10+1;
				nNextGiftID = (m_nGetGiftNum)*10+1;
			}
			else
			{
				nCurGiftID = m_nGetGiftNum*10+1;
				nNextGiftID = (m_nGetGiftNum+1)*10+1;
			}
			const tagIDBuyBack* pIDBuyBackCur = ItemProtoData::Inst()->FindIDBuyBack(nCurGiftID);	//当前级别的奖励物品
			const tagIDBuyBack* pIDBuyBackNext = ItemProtoData::Inst()->FindIDBuyBack(nNextGiftID);
			const tagIDBuyBack* pIDBuyBackCurLevel = ItemProtoData::Inst()->FindIDBuyBack(nLevel);	
			const tagIDBuyBack* pIDBuyBackNextLevel = ItemProtoData::Inst()->FindIDBuyBack((nLevel/10+1)*10);

			BOOL bRet = TRUE;
			for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
			{
				if(pWnd == m_pIStcItemIcon[i])	//历练度里的那个图标
				{
					if(P_VALID(pIDBuyBackCurLevel))
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pIDBuyBackNextLevel->dwItemID[i]).c_str());
					bRet = FALSE;
				}
				else if(pWnd == m_pIStcCurItem[i])	//领取奖励里的当前图标
				{
					if(P_VALID(pIDBuyBackCur))
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pIDBuyBackCur->dwItemID[i]).c_str());
					bRet = FALSE;
				}
				else if(pWnd == m_pIStcNextItem[i])	//领取奖励里的下个阶段图标
				{
					if(P_VALID(pIDBuyBackNext))
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pIDBuyBackNext->dwItemID[i]).c_str());
					bRet = FALSE;
				}
				else if(pWnd == m_pIStcBuyBackItemIcon[i])	//账号回收里面的当前图标
				{
					if(P_VALID(pIDBuyBackCur))
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pIDBuyBackNextLevel->dwItemID[i]).c_str());
					bRet = FALSE;
				}
				else if(pWnd == m_pIStcHaveReceiveItemIcon[i])	//重复领取里面的下阶段图标
				{
					if(P_VALID(pIDBuyBackNext))
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pIDBuyBackNextLevel->dwItemID[i]).c_str());
					bRet = FALSE;
				}
			}
			if(bRet)
			{
				for(int i = 0; i < MAX_GIFT_ITEM_SHOW_NUM; i++)
				{
					if(pWnd == m_pIStcItemShowIcon[i] || pWnd == m_pIStcBuyBackShowItem[i])
					{
						if(P_VALID(m_IDBuyBackEx))
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_IDBuyBackEx->dwItemID[i]).c_str());
					}
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

DWORD IDButBackFrame::OnViewLiLian(tagGameEvent* pEvent)
{
	m_eOpenWhictFrame = EFrameLiLianDu;
	CloseAllWnd();

	return 0;
}
DWORD IDButBackFrame::OnReceiveGift(tagGameEvent* pEvent)
{
	m_eOpenWhictFrame = EFrameReceiveGift;
	CloseAllWnd();

	return 0;
}
DWORD IDButBackFrame::OnIDBuyBack(tagGameEvent* pEvent)
{
	m_eOpenWhictFrame = EFrameIDbuyBack;
	CloseAllWnd();

	return 0;
}

DWORD IDButBackFrame::OnHaveReceive(tagGameEvent* pEvent)
{
	m_eOpenWhictFrame = EFrameHaveReceive;
	CloseAllWnd();

	return 0;
}

VOID IDButBackFrame::CloseAllWnd()
{
	m_pWndHaveReceive->SetInvisible(TRUE);
	m_pWndIDbuyBack->SetInvisible(TRUE);
	m_pWndLiLianDu->SetInvisible(TRUE);
	m_pWndReceiveGift->SetInvisible(TRUE);

	tagNC_GetLiLianExp cmd;
	m_pSession->Send(&cmd);

}

VOID IDButBackFrame::SetInfo()
{
	if(m_nGetGiftNum == 10)
	{
		if(m_eOpenWhictFrame != EFrameNULL)
		{
			m_pWndLiLianDu->SetInvisible(TRUE);
			m_pWndHaveReceive->SetInvisible(TRUE);
			m_pWndIDbuyBack->SetInvisible(TRUE);
			m_pWndReceiveGift->SetInvisible(TRUE);

			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("IDBuyBackReceiveAll")]);
		}

		return;
	}
	m_eOpenWhictFrame = EFrameNULL;
	if(!m_pWndLiLianDu->IsInvisible())	//历练度
	{
		//进度条
		int nLevel = ItemProtoData::Inst()->GetIDBuyBackLevel(m_dwCurLiLianExp);
		TCHAR szText[128] = {0};

		const tagIDBuyBack* ptemp1 = ItemProtoData::Inst()->FindIDBuyBack((nLevel/10+1)*10);
		const tagIDBuyBack* ptemp2 = ItemProtoData::Inst()->FindIDBuyBack(nLevel/10*10);
		int nMaxExp;
		int nCurExp;
		if(!P_VALID(ptemp1))
			ptemp1 = ItemProtoData::Inst()->FindIDBuyBack(100);
		if(!P_VALID(ptemp2))
		{
			nMaxExp = ptemp1->dwExp - 0 ;
			nCurExp = m_dwCurLiLianExp - 0;
		}
		else
		{
			nMaxExp = ptemp1->dwExp - ptemp2->dwExp ;
			nCurExp = m_dwCurLiLianExp - ptemp2->dwExp;
		}
		if(nMaxExp != 0)
			_stprintf(szText, _T("%d%%(%d/%d)"), nCurExp*100/nMaxExp, nCurExp, nMaxExp);

		m_pProCurProcess->SetMaxValue(nMaxExp);
		m_pProCurProcess->SetValue(nCurExp, TRUE);
		m_pStcCurCompleteDesc->SetText(szText);
		
		_stprintf(szText, _T("%d%%"), nLevel);//, m_dwCurLiLianExp, m_dwMaxExp
		m_pProProcess->SetMaxValue(100);
		m_pProProcess->SetValue(nLevel, TRUE);
		m_pStcCompleteDesc->SetText(szText);

		//奖励图标
		const tagIDBuyBack* pIDBuyBack = ItemProtoData::Inst()->FindIDBuyBack((nLevel/10+1)*10);
		const tagItemDisplayInfo* pItemDisplay = NULL;
		if(P_VALID(pIDBuyBack))
		{
			for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
			{
				pItemDisplay = ItemProtoData::Inst()->FindItemDisplay(pIDBuyBack->dwItemID[i]);
				if(P_VALID(pItemDisplay))
				{
					m_pIStcItemIcon[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(pIDBuyBack->dwItemID[i]));
					m_pStcItemName[i]->SetText(pItemDisplay->szName);
				}
				else
				{
					m_pIStcItemIcon[i]->SetPic(NullIconPatch);
					m_pStcItemName[i]->SetText(_T(""));
				}
			}
		}
		//完成全部奖励的那些显示
		for(int i = 0; i < MAX_GIFT_ITEM_SHOW_NUM; i++)
		{
			pItemDisplay = ItemProtoData::Inst()->FindItemDisplay(m_IDBuyBackEx->dwItemID[i]);
			if(P_VALID(pItemDisplay))
			{
				m_pIStcItemShowIcon[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(m_IDBuyBackEx->dwItemID[i]));
				m_pStcItemShowName[i]->SetText(pItemDisplay->szName);
				m_pStcDesc[i]->SetText(m_IDBuyBackEx->strDesc[i].c_str());
			}
		}

		m_pWndLiLianDu->SetRefresh(TRUE);

	}

	if(!m_pWndIDbuyBack->IsInvisible())	//账号回购
	{
		int nLevel = ItemProtoData::Inst()->GetIDBuyBackLevel(m_dwCurLiLianExp);
		//奖励图标
		const tagIDBuyBack* pIDBuyBack = ItemProtoData::Inst()->FindIDBuyBack((nLevel/10+1)*10);
		const tagItemDisplayInfo* pItemDisplay = NULL;
		if(P_VALID(pIDBuyBack))
		{
			for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
			{
				pItemDisplay = ItemProtoData::Inst()->FindItemDisplay(pIDBuyBack->dwItemID[i]);
				if(P_VALID(pItemDisplay))
				{
					m_pIStcBuyBackItemIcon[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(pIDBuyBack->dwItemID[i]));
					m_pStcBuyBackItemName[i]->SetText(pItemDisplay->szName);
				}
				else
				{
					m_pIStcBuyBackItemIcon[i]->SetPic(NullIconPatch);
					m_pStcBuyBackItemName[i]->SetText(_T(""));
				}
			}
		}
		//完成全部奖励的那些显示
		for(int i = 0; i < MAX_GIFT_ITEM_SHOW_NUM; i++)
		{
			pItemDisplay = ItemProtoData::Inst()->FindItemDisplay(m_IDBuyBackEx->dwItemID[i]);
			if(P_VALID(pItemDisplay))
			{
				m_pIStcBuyBackShowItem[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(m_IDBuyBackEx->dwItemID[i]));
				m_pIStcBuyBackShowItemName[i]->SetText(pItemDisplay->szName);
				m_pStcButBackDesc[i]->SetText((m_IDBuyBackEx->strDesc[i]).c_str());
			}
		}
		
	}

	if(!m_pWndReceiveGift->IsInvisible())	//礼包领取
	{
		//当前奖励
		int nGiftID = 0;
		int nLevel = ItemProtoData::Inst()->GetIDBuyBackLevel(m_dwCurLiLianExp);
		if(nLevel < m_nGetGiftNum*10)
			nGiftID = (m_nGetGiftNum-1)*10+1;
		else
			nGiftID = m_nGetGiftNum*10+1;
		const tagIDBuyBack* pIDBuyBack = ItemProtoData::Inst()->FindIDBuyBack(nGiftID);
		const tagItemDisplayInfo* pDisplay = NULL;
		if(P_VALID(pIDBuyBack))
		{
			for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
			{
				pDisplay = ItemProtoData::Inst()->FindItemDisplay(pIDBuyBack->dwItemID[i]);
				if(P_VALID(pDisplay))
				{
					m_pIStcCurItem[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(pIDBuyBack->dwItemID[i]));
					m_pStcCurItemName[i]->SetText(pDisplay->szName);
				}
				else
				{
					m_pIStcCurItem[i]->SetPic(NullIconPatch);
					m_pStcCurItemName[i]->SetText(_T(""));
				}
			}
		}
		else
		{
			for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
			{
				m_pIStcCurItem[i]->SetPic(NullIconPatch);
				m_pStcCurItemName[i]->SetText(_T(""));
			}
		}
		//下一个阶段
		if(nLevel < m_nGetGiftNum*10)
			nGiftID = (m_nGetGiftNum)*10+1;
		else
			nGiftID = (m_nGetGiftNum+1)*10+1;

		pIDBuyBack = ItemProtoData::Inst()->FindIDBuyBack(nGiftID);
		if(P_VALID(pIDBuyBack))
		{
			for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
			{
				pDisplay = ItemProtoData::Inst()->FindItemDisplay(pIDBuyBack->dwItemID[i]);
				if(P_VALID(pDisplay))
				{
					m_pIStcNextItem[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(pIDBuyBack->dwItemID[i]));
					m_pStcNextItemName[i]->SetText(pDisplay->szName);
				}
				else
				{
					m_pIStcNextItem[i]->SetPic(NullIconPatch);
					m_pStcNextItemName[i]->SetText(_T(""));
				}
			}
		}
		else	//领到最后一次了
		{
			for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
			{
				m_pIStcNextItem[i]->SetPic(NullIconPatch);
				m_pStcNextItemName[i]->SetText(_T(""));
			}
		}
	}

	if(!m_pWndHaveReceive->IsInvisible())
	{
		int nGiftID = 0;
		int nLevel = ItemProtoData::Inst()->GetIDBuyBackLevel(m_dwCurLiLianExp);

		const tagIDBuyBack* pIDBuyBack = ItemProtoData::Inst()->FindIDBuyBack((nLevel/10+1)*10);
		const tagItemDisplayInfo* pDisplay = NULL;

		if(m_nGetGiftNum == 0)
		{
			m_pStcHaveReceiveTips->SetText(g_StrTable[_T("IDButBackCanNotReceive")]);
		}
		else
		{
			m_pStcHaveReceiveTips->SetText(g_StrTable[_T("IDBuyBackHaveReceive")]);
		}

		if(P_VALID(pIDBuyBack))
		{
			for(int i = 0; i < MAX_GIFT_ITEM_NUM; i++)
			{
				pDisplay = ItemProtoData::Inst()->FindItemDisplay(pIDBuyBack->dwItemID[i]);
				if(P_VALID(pDisplay))
				{
					m_pIStcHaveReceiveItemIcon[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(pIDBuyBack->dwItemID[i]));
					m_pStcReceiveItemName[i]->SetText(pDisplay->szName);
				}
				else
				{
					m_pIStcHaveReceiveItemIcon[i]->SetPic(NullIconPatch);
					m_pStcReceiveItemName[i]->SetText(_T(""));
				}
			}
		}
	}
}

DWORD IDButBackFrame::OnNS_GetLiLianExp(tagNS_GetLiLianExp* pCmd, DWORD)
{
	m_dwCurLiLianExp = pCmd->dwExp;
	m_nGetGiftNum = pCmd->dwReceiveNum;

	if(m_nGetGiftNum < 10)
	{
		switch(m_eOpenWhictFrame)
		{
		case EFrameHaveReceive:
			m_pWndHaveReceive->SetInvisible(FALSE);
			break;
		case EFrameIDbuyBack:
			m_pWndIDbuyBack->SetInvisible(FALSE);
			break;
		case EFrameLiLianDu:
			m_pWndLiLianDu->SetInvisible(FALSE);
			break;
		case EFrameReceiveGift:
			m_pWndReceiveGift->SetInvisible(FALSE);
			break;
		}
	}

	SetInfo();

	return 0;
}

DWORD IDButBackFrame::OnNS_ReceiveGift(tagNS_ReceiveGift* pCmd, DWORD)
{
	if(pCmd->dwErrcode != E_Success)
	{
		switch(pCmd->dwErrcode)
		{
		case E_NOT_GET_GIFT:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("KeyCode_Msg428")]);
				break;
			}
		}
	}

	tagNC_GetLiLianExp cmd;
	m_pSession->Send(&cmd);
	return 0;
}

DWORD IDButBackFrame::OnNS_LiLianChange(tagNS_LiLianChange* pCmd, DWORD)
{
	tagPctNExpAchieve event(_T("UpdateLPLiLianEvent"), NULL);
	event.nPctMultiplied = pCmd->dwExp;
	m_pFrameMgr->SendEvent(&event);

	return 0;
}