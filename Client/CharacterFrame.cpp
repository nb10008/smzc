#include "stdafx.h"
#include "CharacterFrame.h"
#include "ReputeFrame.h"
#include "RoleTitleFrame.h"
#include "InformationFrame.h"
#include "StateFrame.h"
#include "EquipmentFrame.h"
#include "..\WorldDefine\reputation.h"
#include "EasyBuyFrame.h"
#include "ItemMgr.h"
#include "TrumpFrame.h"
#include "RoleMgr.h"
#include "TrumpRefineFrame.h"
#include "TrumpMixBeadFrame.h"
#include "TrumpRebornFrame.h"
#include "LocalPlayer.h"
#include "FairyAbilityFrame.h"
#include ".\FairyAutoFeedFrame.h"
#include ".\FairyUpgradeFrame.h"
#include "BlinkStatic.h"
#include "ShengLingFrame.h"

CharacterFrame::CharacterFrame(void):m_Trunk(this)
{
	m_pState = NULL;
	m_pEquipment = NULL;
	m_pWnd = NULL;
	m_pWndCaption = NULL;
	m_pPbnState = NULL;
	m_pPbnCareer = NULL;
	m_pPbnRepute = NULL;
	m_pPbnRoleTitle = NULL;
	m_pPbnTrump = NULL;
	m_pBtnClose = NULL;
	m_pWndState	= NULL;
	m_pWndRepute = NULL;
	m_pWndCareer = NULL;
	m_pWndRoleTitle = NULL;
	m_pWndTrump = NULL;
}

CharacterFrame::~CharacterFrame(void)
{
}

BOOL CharacterFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	m_pMgr->RegisterEventHandle(_T("Open_Role"),    (FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenCharacter));
	m_pMgr->RegisterEventHandle(_T("Close_Role"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnCloseCharacter));

    m_pMgr->RegisterEventHandle(_T("OpenEquipState"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenEquipState));
    m_pMgr->RegisterEventHandle(_T("OpenInfomation"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenInfomation));
    m_pMgr->RegisterEventHandle(_T("OpenReputation"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenReputation));
    m_pMgr->RegisterEventHandle(_T("OpenTitle"),	    (FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenTitle));
	m_pMgr->RegisterEventHandle(_T("OpenTrump"),	    (FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenTrump));
	m_pMgr->RegisterEventHandle(_T("Open_Holy"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenHoly));

	TObjRef<NetCmdMgr>()->Register("NS_ItemPosChangeEx",			(NETMSGPROC)m_Trunk.sfp2(&CharacterFrame::NetItemPosChangeEx),		_T("NS_ItemPosChangeEx"));
	TObjRef<NetCmdMgr>()->Register("NS_GetRoleInitState_Item",		(NETMSGPROC)m_Trunk.sfp2(&CharacterFrame::NetInitLocalPlayerItem),	_T("NS_GetRoleInitState_Item"));

	return bRet;
}

BOOL CharacterFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	
	if( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	
	m_pState = NULL;
	m_pEquipment = NULL;
	m_pRoleTitle = NULL;
	m_pRepute = NULL;
	m_pInformation = NULL;
	m_pTrump = NULL;

	m_pMgr->UnRegisterEventHandler(_T("Open_Role"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenCharacter));
	m_pMgr->UnRegisterEventHandler(_T("Close_Role"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnCloseCharacter));

    m_pMgr->UnRegisterEventHandler(_T("OpenEquipState"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenEquipState));
    m_pMgr->UnRegisterEventHandler(_T("OpenInfomation"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenInfomation));
    m_pMgr->UnRegisterEventHandler(_T("OpenReputation"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenReputation));
    m_pMgr->UnRegisterEventHandler(_T("OpenTitle"),	        (FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenTitle));
	m_pMgr->UnRegisterEventHandler(_T("OpenTrump"),	        (FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenTrump));
	m_pMgr->UnRegisterEventHandler(_T("Open_Holy"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&CharacterFrame::OnOpenHoly));
	TObjRef<NetCmdMgr>()->UnRegister("NS_ItemPosChangeEx",			(NETMSGPROC)m_Trunk.sfp2(&CharacterFrame::NetItemPosChangeEx));
	TObjRef<NetCmdMgr>()->UnRegister("NS_GetRoleInitState_Item",		(NETMSGPROC)m_Trunk.sfp2(&CharacterFrame::NetInitLocalPlayerItem));

	return bRet;
}

BOOL CharacterFrame::ReloadUI()
{
	// 创建
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\rolestate.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\role", "ClassName", "PlayerViewWindow");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\sash\\sashpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\glove\\glovepic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\fashionback\\backpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\fashionboot\\bootpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\fashioncloth\\clothpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\fashionglove\\glovepic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\ring2\\ring2pic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\boot\\bootpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\minorwp\\minorwppic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\minorwp1\\minorwppic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\talisman\\talismanpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\mainwp\\mainwppic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\mantle\\mantlepic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\ring1\\ring1pic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\upper\\upperpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\necklace\\necklacepic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\hat\\hatpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\fashionhat\\hatpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\lower\\lowerpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\sl\\talismanpic", "ClassName", "ItemButton");

	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\sash\\sashpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\glove\\glovepic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\fashionback\\backpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\fashionboot\\bootpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\fashioncloth\\clothpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\fashionglove\\glovepic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\ring2\\ring2pic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\boot\\bootpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\minorwp\\minorwppic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\minorwp1\\minorwppic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\talisman\\talismanpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\mainwp\\mainwppic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\mantle\\mantlepic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\ring1\\ring1pic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\upper\\upperpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\necklace\\necklacepic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\hat\\hatpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\fashionhat\\hatpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\lower\\lowerpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\equipback\\sl\\talismanpic\\pic", "ClassName", "BlinkStatic");

	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\fairy_back\\basic\\pic_back\\pic", "ClassName", "ItemButton");

	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\rs_pic2\\hpback\\hpbar", "ClassName", "ActiveProgress");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\rstatepic\\rs_pic2\\mpback\\mpbar", "ClassName", "ActiveProgress");

	for (int i = 0; i < 5; ++i)
	{
		char cbuff[128];
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),
			"ra_win\\ra_pic\\fairy_back\\picback\\pic\\pic%d",
			i);
		m_pGUI->ChangeXml(&ele, cbuff, "ClassName", "ItemButton");
	}

	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\fairy_back\\basic\\moodpic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\fairy_back\\basic\\feedpic", "ClassName", "IconStatic");

	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\fairy_back\\expback\\picback\\pic0", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\fairy_back\\expback\\picback\\pic1", "ClassName", "IconStatic");

	for (int i = 0; i < ECLT_NUM; ++i)
	{
		char cbuff[128];
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),
			"ra_win\\ra_pic\\repute\\reputebackpic\\propic%d\\pro%d",
			i + 1, i + 1);
		m_pGUI->ChangeXml(&ele, cbuff, "ClassName", "ProgressEx");
	}
	
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\roletitle\\titlelistpic\\titlelist", "ClassName", "TreeEx");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\informationpic\\information\\headpicback", "ClassName", "IconStatic");
    m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\informationpic\\information\\cityname\\cityname_input", "MaxTextCount", "6");
    m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\informationpic\\information\\presentationpic\\presentation", "MaxTextCount", "52");

	//圣灵界面控件类型change
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\sl_back\\basic\\pic_back\\pic", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\sl_back\\basic\\aptitudeprogress\\progressbar", "ClassName", "ActiveProgress");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\sl_back\\basic\\cultivationprogress", "ClassName", "ActiveProgress");
	m_pGUI->ChangeXml(&ele, "ra_win\\ra_pic\\sl_back\\basic\\moodprogress\\progressbar", "ClassName", "ActiveProgress");
	for(int i = 0; i < 6; i++)
	{
		char cText[128] = {0};
		_snprintf(cText, sizeof(cText)/sizeof(char),"ra_win\\ra_pic\\sl_back\\picback\\pic\\pic%d", i);
		m_pGUI->ChangeXml(&ele, cText, "ClassName", "IconStatic");
	}

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&CharacterFrame::EventHandler));

	m_pWndCaption = m_pWnd->GetChild(_T("ra_pic"));
	m_pWndState = m_pWndCaption->GetChild(_T("rstatepic"));
	m_pWndCareer = m_pWndCaption->GetChild(_T("informationpic"));
	m_pWndRepute = m_pWndCaption->GetChild(_T("repute"));
	m_pWndRoleTitle = m_pWndCaption->GetChild(_T("roletitle"));
	m_pWndTrump = m_pWndCaption->GetChild(_T("fairy_back"));
	m_pWndShengLing = m_pWndCaption->GetChild(_T("sl_back"));

	m_pPbnState = (GUIPushButton*)m_pWndCaption->GetChild(_T("winopt\\optbutt0"));
	m_pPbnCareer = (GUIPushButton*)m_pWndCaption->GetChild(_T("winopt\\optbutt1"));
	m_pPbnRepute = (GUIPushButton*)m_pWndCaption->GetChild(_T("winopt\\optbutt2"));
	m_pPbnRoleTitle = (GUIPushButton*)m_pWndCaption->GetChild(_T("winopt\\optbutt3"));
	m_pPbnTrump = (GUIPushButton*)m_pWndCaption->GetChild(_T("winopt\\optbutt4"));
	m_pPbnShengLing = (GUIPushButton*)m_pWndCaption->GetChild(_T("winopt\\optbutt5"));

	m_pPbnState->SetEnable(true);
	m_pPbnCareer->SetEnable(true);
	m_pPbnRepute->SetEnable(true);
	m_pPbnRoleTitle->SetEnable(true);
	m_pPbnTrump->SetEnable(false);
	m_pPbnShengLing->SetEnable(false);

	m_pPbnState->SetState(EGUIBS_PushDown, FALSE);

	m_pBtnClose = (GUIButton*)m_pWndCaption->GetChild(_T("new"));

	m_pState = (StateFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("State"), _T("StateFrame"), 0);

	m_pEquipment = (EquipmentFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("Equip"), _T("EquipmentFrame"), 0);

	m_pRepute = (ReputeFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("Repute"), _T("ReputeFrame"), 0);
	if(!P_VALID(m_pRepute))
		IMSG(_T("Create ReputeFrame failed"));

	m_pRoleTitle = (RoleTitleFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("RoleTitle"), _T("RoleTitleFrame"), 0);
	if(!P_VALID(m_pRoleTitle))
		IMSG(_T("Create RoleTitleFrame failed"));

	m_pInformation = (InformationFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("Information"), _T("InformationFrame"), 0);
	if(!P_VALID(m_pInformation))
		IMSG(_T("Create InformationFrame failed"));

	m_pTrump = (TrumpFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("Trump"), _T("TrumpFrame"), 0);
	if(!P_VALID(m_pTrump))
		IMSG(_T("Create TrumpFrame failed"));

	m_pShengLing = (ShengLingFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("ShengLingFrame"), _T("ShengLingFrame"), 0);
	if(!P_VALID(m_pShengLing))
		IMSG(_T("Create ShengLingFrame failed"));

	m_pWnd->SetInvisible(TRUE);

	return TRUE;
}

DWORD CharacterFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnClose)
			{
				m_pMgr->SendEvent( &tagGameEvent( _T("Close_Role_UI"), this ) );
				TrumpFrame *pTrumpFrame = (TrumpFrame*)m_pFrameMgr->GetFrame(_T("Trump"));
				if (P_VALID(pTrumpFrame))
					pTrumpFrame->LockItem(false);
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if (pEvent->dwParam1 != 1)//不是按下不处理
				return 0;

			FairyAbilityFrame *pFrame = (FairyAbilityFrame*)m_pFrameMgr->GetFrame(_T("FairyAbility"));
			TrumpFrame *pTrumpFrame = (TrumpFrame*)m_pFrameMgr->GetFrame(_T("Trump"));
            if (pWnd == m_pPbnState)//角色
            {
                m_pWndState->SetInvisible(FALSE);
                m_pWndCareer->SetInvisible(TRUE);
                m_pWndRepute->SetInvisible(TRUE);
                m_pWndRoleTitle->SetInvisible(TRUE);
				m_pWndTrump->SetInvisible(TRUE);
				m_pWndShengLing->SetInvisible(TRUE);
				m_pTrump->ClearExpSaveInfo();
				if(P_VALID(pFrame))
					pFrame->ShowUI(FALSE);
				if (P_VALID(pTrumpFrame))
					pTrumpFrame->LockItem(false);
				m_pShengLing->CloseShengLingWnd();
            }
            else if (pWnd == m_pPbnCareer)//名帖
            {
                m_pWndState->SetInvisible(TRUE);
                m_pWndCareer->SetInvisible(FALSE);
                m_pWndRepute->SetInvisible(TRUE);
                m_pWndRoleTitle->SetInvisible(TRUE);
				m_pWndTrump->SetInvisible(TRUE);
				m_pWndShengLing->SetInvisible(TRUE);
				m_pTrump->ClearExpSaveInfo();
				if(P_VALID(pFrame))
					pFrame->ShowUI(FALSE);
				if (P_VALID(pTrumpFrame))
					pTrumpFrame->LockItem(false);
				m_pShengLing->CloseShengLingWnd();
            }
            else if (pWnd == m_pPbnRepute)//声望
            {
                m_pWndState->SetInvisible(TRUE);
                m_pWndCareer->SetInvisible(TRUE);
                m_pWndRepute->SetInvisible(FALSE);
                m_pWndRoleTitle->SetInvisible(TRUE);
				m_pWndTrump->SetInvisible(TRUE);
				m_pWndShengLing->SetInvisible(TRUE);
				m_pTrump->ClearExpSaveInfo();
				if(P_VALID(pFrame))
					pFrame->ShowUI(FALSE);
				if (P_VALID(pTrumpFrame))
					pTrumpFrame->LockItem(false);
				m_pShengLing->CloseShengLingWnd();
            }
			else if (pWnd == m_pPbnRoleTitle)//称号
			{
				m_pWndState->SetInvisible(TRUE);
				m_pWndCareer->SetInvisible(TRUE);
				m_pWndRepute->SetInvisible(TRUE);
				m_pWndRoleTitle->SetInvisible(FALSE);
				m_pWndTrump->SetInvisible(TRUE);
				m_pWndShengLing->SetInvisible(TRUE);
				m_pTrump->ClearExpSaveInfo();
				if(P_VALID(pFrame))
					pFrame->ShowUI(FALSE);
				if (P_VALID(pTrumpFrame))
					pTrumpFrame->LockItem(false);
				m_pShengLing->CloseShengLingWnd();
			}
			else if (pWnd == m_pPbnTrump) //法宝
			{
				m_pWndState->SetInvisible(TRUE);
				m_pWndCareer->SetInvisible(TRUE);
				m_pWndRepute->SetInvisible(TRUE);
				m_pWndRoleTitle->SetInvisible(TRUE);
				m_pWndTrump->SetInvisible(FALSE);
				m_pWndShengLing->SetInvisible(TRUE);
				m_pTrump->UpdateUI();
				if(P_VALID(pFrame))
					pFrame->ShowUI(TRUE);
				m_pShengLing->CloseShengLingWnd();
			}
			else if( pWnd == m_pPbnShengLing )	//圣灵
			{
				m_pWndState->SetInvisible(TRUE);
				m_pWndCareer->SetInvisible(TRUE);
				m_pWndRepute->SetInvisible(TRUE);
				m_pWndRoleTitle->SetInvisible(TRUE);
				m_pWndTrump->SetInvisible(TRUE);
				m_pWndShengLing->SetInvisible(FALSE);
				if(P_VALID(pFrame))
					pFrame->ShowUI(FALSE);
				//刷新圣灵属性
				tagGameEvent evt(_T("UpdateHolyAtt"), NULL);
				m_pFrameMgr->SendEvent(&evt);
			}
		}
		break;
	case EGUIE_Drag:
		{
			EasyBuyFrame *pFrame = (EasyBuyFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("RoleInfoEasyBuyFrame"));
			if( P_VALID(pFrame) )
			{
				pFrame->Align();
			}

			if (P_VALID(m_pState))
			{
				m_pState->AlignAttEXShowWnd();
			}
		}
		break;
	}
	return 0;
}

VOID CharacterFrame::ShowWnd( BOOL bVal )
{
	m_pWnd->SetInvisible(!bVal);
	if(bVal)
	{
		m_pPbnState->SetState(EGUIBS_PushDown);  //打开就显示第一页任务状态
		m_pEquipment->UpdateEquipAvatar();
		m_pState->UpdatePalyerAtt();
		m_pRoleTitle->UpdateRoleTitle();
		m_pInformation->UpdateInformation();
        m_pRepute->UpdateRepute();

		m_pWnd->FlipToTop();
		m_pWnd->SetRefresh();
	}
	else
	{
		/*! 关闭名人堂 */
		m_pFrameMgr->SendEvent(&tagGameEvent(_T("SetFamousClose"), this));
		FairyAbilityFrame *pFrame = (FairyAbilityFrame*)m_pFrameMgr->GetFrame(_T("FairyAbility"));
		if(P_VALID(pFrame))
			pFrame->ShowUI(FALSE);
		m_pTrump->ClearExpSaveInfo();
		m_pShengLing->CloseShengLingWnd();
	}
}


DWORD CharacterFrame::OnOpenCharacter( tagGameEvent* pGameEvent )
{
	ShowWnd(TRUE);
	FairyAutoFeedFrame *pFeedFrame = (FairyAutoFeedFrame*)m_pFrameMgr->GetFrame(_T("FairyAutoFeed"));
	if(P_VALID(pFeedFrame))
		pFeedFrame->ShowUI(FALSE);
	FairyUpgradeFrame *pUpgradeFrame = (FairyUpgradeFrame*)m_pFrameMgr->GetFrame(_T("FairyUpgrade"));
	if(P_VALID(pUpgradeFrame))
		m_pMgr->DestroyFrame(pUpgradeFrame);
	return 0;
}

DWORD CharacterFrame::OnCloseCharacter( tagGameEvent* pGameEvent )
{
	ShowWnd(FALSE);
	m_pMgr->SendEvent( &tagGameEvent( _T("Close_Role_UI"), this ) );//add by xp解决按C取消人物界面时人物扩展界面不消失的问题

	return 0;
}

BOOL CharacterFrame::EscCancel()
{
	// 界面隐藏则返回FALSE
	if(m_pWnd->IsInvisible())
		return FALSE;
	// 否则发送关闭事件，返回TRUE
	m_pMgr->SendEvent( &tagGameEvent( _T("Close_Role_UI"), this ) );
	m_pTrump->ClearExpSaveInfo();

	TrumpFrame *pTrumpFrame = (TrumpFrame*)m_pFrameMgr->GetFrame(_T("Trump"));
	if (P_VALID(pTrumpFrame))
		pTrumpFrame->LockItem(false);
	return TRUE;
}

DWORD CharacterFrame::OnOpenEquipState( tagGameEvent* pEvent )
{
    if( EGUIBS_PushDown != m_pPbnState->GetState() )
        m_pPbnState->SetState( EGUIBS_PushDown );

	return 0;
}

DWORD CharacterFrame::OnOpenInfomation( tagGameEvent* pEvent )
{
    if( EGUIBS_PushDown != m_pPbnCareer->GetState() )
        m_pPbnCareer->SetState( EGUIBS_PushDown );
	
	return 0;
}

DWORD CharacterFrame::OnOpenReputation( tagGameEvent* pEvent )
{
    if( EGUIBS_PushDown != m_pPbnRepute->GetState() )
        m_pPbnRepute->SetState( EGUIBS_PushDown );

	return 0;
}

DWORD CharacterFrame::OnOpenTitle( tagGameEvent* pEvent )
{
    if( EGUIBS_PushDown != m_pPbnRoleTitle->GetState() )
        m_pPbnRoleTitle->SetState( EGUIBS_PushDown );

	return 0;
}

DWORD CharacterFrame::OnOpenTrump( tagGameEvent* pEvent )
{
	if( EGUIBS_PushDown != m_pPbnRoleTitle->GetState() )
		m_pPbnTrump->SetState( EGUIBS_PushDown );

	return 0;
}

DWORD CharacterFrame::OnOpenHoly(tagGameEvent* pEvent)
{
	m_pWnd->SetInvisible(FALSE);
	if( EGUIBS_PushDown != m_pPbnShengLing->GetState() )
		m_pPbnShengLing->SetState( EGUIBS_PushDown );
	return 0;
}

DWORD CharacterFrame::NetItemPosChangeEx(tagNS_ItemPosChangeEx* pNetCmd, DWORD)
{
	Equipment *pTrump = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	Equipment* pShengLing = ItemMgr::Inst()->GetCurEquip(EEP_Holy);
	if(P_VALID(pTrump))
	{
		m_pPbnTrump->SetEnable(true);
		if(P_VALID(m_pTrump))
		{
			m_pTrump->UpdateUI();
			FairyAbilityFrame *pFrame = (FairyAbilityFrame*)m_pFrameMgr->GetFrame(_T("FairyAbility"));
			if(P_VALID(pFrame))
				pFrame->UpdateUI();
			tagGameEvent e(_T("ChangeFairy"), NULL );
			m_pMgr->SendEvent(&e);
		}
	}
	else
	{
		m_pPbnTrump->SetEnable(false);

		RefineTrumpFrame *pRefineFrame = (RefineTrumpFrame*)m_pFrameMgr->GetFrame(_T("RefineTrump"));
		if(P_VALID(pRefineFrame))
			pRefineFrame->ShowUI(false);

		TrumpMixBeadFrame *pMixBeadFrame = (TrumpMixBeadFrame*)m_pFrameMgr->GetFrame(_T("TrumpMixBead"));
		if(P_VALID(pMixBeadFrame))
			pMixBeadFrame->ShowUI(false);

		TrumpRebornFrame *pRebornFrame = (TrumpRebornFrame*)m_pFrameMgr->GetFrame(_T("TrumpReborn"));
		if(P_VALID(pRebornFrame))
			pRebornFrame->ShowUI(false);
	}
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if ( (P_VALID(pLp) && P_VALID(pLp->GetMount())) || !P_VALID(pTrump) )
		pLp->ClearTrumpEffect();
	else
		pLp->PlayTrumpEffect();
	//圣灵
	if(P_VALID(pShengLing))
	{
		m_pPbnShengLing->SetEnable(TRUE);
	}
	else
	{
		m_pPbnShengLing->SetEnable(FALSE);
	}

	return 0;
}

DWORD CharacterFrame::NetInitLocalPlayerItem(tagNS_GetRoleInitState_Item* pNetCmd, DWORD)
{
	Equipment *pTrump = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	Equipment* pShengLing = ItemMgr::Inst()->GetCurEquip(EEP_Holy);
	if(P_VALID(pTrump))
	{
		LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
		if (P_VALID(pLp) && P_VALID(pLp->GetMount()))
			pLp->ClearTrumpEffect();
		else
			pLp->PlayTrumpEffect();
		m_pPbnTrump->SetEnable(true);
	}
	if(P_VALID(pShengLing))
	{
		m_pPbnShengLing->SetEnable(true);
	}
	return 0;
}