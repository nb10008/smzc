#include "stdafx.h"
#include "ExtractionFrame.h"
#include "ComposeStatic.h"
#include "RoleMgr.h"
#include "ItemFrame.h"
#include "ItemMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "CombatSysUtil.h"
#include "SkillMgr.h"
#include "..\WorldDefine\compose_define.h"
#include "ProduceData.h"
#include "ItemProtoData.h"
#include "AudioSys.h"
#include "..\WorldDefine\msg_compose.h"
#include "QuestMgr.h"

const TCHAR *szExtractDefaultPic = _T("data\\ui\\main\\l_icon_back.bmp");

ExtractionFrame::ExtractionFrame( void ):m_Trunk(this)
{
	m_pWndMain		= NULL;
	m_pWndCaption	= NULL;
	m_pBtnExit		= NULL;
	m_pBtnCancel	= NULL;
	m_pBtnOk		= NULL;
	m_pDeStuffList	= NULL;
	m_pDeUnit		= NULL;
	m_pStcPotVal	= NULL;
	m_eCurState		= EDeCompose_NULL;
	m_pStcTip		= NULL;
	m_pStcGoldCost	= NULL;
	m_pStcSilverCost= NULL;
}

ExtractionFrame::~ExtractionFrame( void )
{

}

BOOL ExtractionFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	//--注册游戏事件
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2Compose"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ExtractionFrame::OnEventProduceAddItem));
	m_pMgr->RegisterEventHandle(_T("OpenExtractFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ExtractionFrame::OnOpenExtractFrame));


	//--注册网络消息
	m_pCmdMgr->Register("NS_Extraction", (NETMSGPROC)m_Trunk.sfp2(&ExtractionFrame::OnNS_Extraction), _T("NS_Extraction"));

	return TRUE;
}

BOOL ExtractionFrame::Destroy()
{
	GameFrame::Destroy();
	if( P_VALID(m_pWndMain) )
		m_pGUI->AddToDestroyList(m_pWndMain);

	//--注册游戏事件
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Compose"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ExtractionFrame::OnEventProduceAddItem));
	m_pMgr->UnRegisterEventHandler(_T("OpenExtractFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ExtractionFrame::OnOpenExtractFrame));
	//m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPSkillEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ExtractionFrame::OnUpdateLPSkillEvent));


	//--注销网络消息
	m_pCmdMgr->UnRegister("NS_Extraction", (NETMSGPROC)m_Trunk.sfp2(&ExtractionFrame::OnNS_Extraction));

	return TRUE;
}

BOOL ExtractionFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\Extract.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "Extract\\Extractbackpic\\itemback\\item", "ClassName", "ComposeStatic");
	m_pWndMain = m_pGUI->CreateWnd(_T("\\desktop"), &element);

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWndMain->GetFullName().c_str(), m_Trunk.sfp1(&ExtractionFrame::EventHandler));

	m_pWndCaption		= (GUIPatch*)m_pWndMain->GetChild( _T("Extractbackpic") );
	m_pBtnExit			= (GUIButton*)m_pWndCaption->GetChild( _T("close") );
	m_pBtnOk			= (GUIButton*)m_pWndCaption->GetChild( _T("startbutt") );
	m_pBtnOk->SetEnable(false);
	m_pBtnCancel		= (GUIButton*)m_pWndCaption->GetChild( _T("breakbutt") );
	m_pBtnCancel->SetEnable(false);
	m_pDeStuffList		= (GUIListBox*)m_pWndCaption->GetChild( _T("listback\\list") );
	m_pDeStuffList->SetColNum(1, 120);
	m_pDeUnit			= (ComposeStatic*)m_pWndCaption->GetChild( _T("itemback\\item") );
	m_pDeUnit->SetPic(szExtractDefaultPic);
	m_pStcPotVal		= (GUIStatic*)m_pWndCaption->GetChild( _T("num1") );
	m_pProTime			= (GUIProgress*)m_pWndCaption->GetChild( _T("timepic\\time") );
	m_pStcTip			= (GUIStatic*)m_pWndCaption->GetChild(_T("new"));
	m_pStcTip->SetText(g_StrTable[_T("ExtractTip")]);
	//m_pStcTip->SetInvisible(TRUE);	//隐藏点化提示
	m_pStcGoldCost		= (GUIStatic*)m_pWndCaption->GetChild(_T("word3\\goldnum"));
	m_pStcSilverCost	= (GUIStatic*)m_pWndCaption->GetChild(_T("word3\\silvernum"));
	m_pWndCaption->SetText(g_StrTable[_T("Extract")]);
	

	m_pWndMain->SetInvisible(TRUE);

	return TRUE;
}

VOID ExtractionFrame::Update()
{
	GameFrame::Update();
	
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_escapeTime = 0;

		if (RoleMgr::Inst()->IsOutValidDist( m_dwNPCGolobelID ) )
		{
			ClearUIandStuff();
			m_pMgr->AddToDestroyList(this);
			return;
		}
	}

	switch(m_eCurState)
	{
	case EDeCompose_SendMsg:
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
			//发送合成消息
			if(fcur >= fMax)
			{				
				SendDecomposeMsg();
			}
		}
		break;
	case EDeCompose_RecvMsg:
		{
			if(Kernel::Inst()->GetAccumTimeDW()- m_dwRecvMsgTime >= 1000)
			{
				ClearUIandStuff();
			}// end if
		}
		break;
	default:
		break;
	}

}

VOID ExtractionFrame::OnEvent( tagGameEvent* pEvent )
{

}

BOOL ExtractionFrame::EscCancel()
{
	if(m_pWndMain->IsInvisible())
		return FALSE;

	m_pFrameMgr->AddToDestroyList(this);
	SetStrategy(EIST_ItemPocket);
	return TRUE;
}

DWORD ExtractionFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnExit )				//退出
			{
				ClearUIandStuff();
				m_pFrameMgr->AddToDestroyList(this);
				SetStrategy(EIST_ItemPocket);
			}
			else if( pWnd == m_pBtnOk )
			{
				JudgeSendDecompose();
			}
			else if( pWnd==m_pBtnCancel && m_eCurState==EDeCompose_SendMsg )				//取消
			{
				ResetStartUIState();
			}
		}
		break;
	case EOnEventSendDropGUI_Item:
		{
			if(m_eCurState!=EDeCompose_NULL)
				return FALSE;

			return HandleDragStuff(pEvent);
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if( pWnd == m_pDeUnit )  
			{
				tagMainStuff mainStuff;
				m_Container.GetMainStuff(mainStuff);
				Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(mainStuff.n64ID);
				if( P_VALID(pEquip) )
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	}

	return TRUE;
}

void ExtractionFrame::ClearUIandStuff()
{
	m_eCurState = EDeCompose_NULL;
	m_pDeStuffList->Clear();
	m_pDeUnit->SetPic(szExtractDefaultPic);
	m_pStcPotVal->SetText(_T(""));
	m_pBtnOk->SetEnable(false);	
	m_pBtnCancel->SetEnable(false);
	m_pProTime->SetValue(0, true);
	m_pWndMain->SetRefresh(TRUE);
	m_Container.ClearAllStuff();
	m_dwEquipID = GT_INVALID;
}

DWORD ExtractionFrame::OnEventProduceAddItem( tagItemPosChangeExEvent* pGameEvent )
{	
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;

	//正在分解中,错误提示
	if( m_eCurState != EDeCompose_NULL )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Processing")]);
		return 0;
	}

	//判断是否为装备
	if(MIsEquipment(pGameEvent->dwTypeIDSrc))
	{
		INT16 nPos = INT16(pGameEvent->n16IndexSrc);
		Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
		if(!P_VALID(pEquip))
			return 0;


		if( pEquip->IsNotIdetified() )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Equip_NoIdetified")]);
			return 0;
		}

		ClearUIandStuff();
		m_dwEquipID = pGameEvent->dwTypeIDSrc;

		JudgeDecompositonFormula(m_dwEquipID, pEquip->GetItemId(), pEquip->GetEquipPos());
		m_pWndMain->SetRefresh(TRUE);
	}
	return 0;
}

void ExtractionFrame::JudgeDecompositonFormula(DWORD dwItemID, INT64 n64ID, EEquipPos ePos)
{
	//
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(n64ID);
	if(!P_VALID(pEquip))
		return ;

	tagExtractProto* pProto = ProduceData::Inst()->FindExtractProto(pEquip->GetItemTypeID());


	if( P_VALID(pProto) )
	{
		if(pEquip->GetItemQuality() < pProto->byQuality)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CannotExtract")]);
			return ;
		}
		tstring strPic = ItemMgr::Inst()->GetItemIconPath(dwItemID);
		m_pDeUnit->SetPic(strPic);

		tagMainStuff stuff;
		stuff.n64ID = n64ID;
		stuff.euqipPos = ePos;
		m_Container.Add(stuff);

		TCHAR szText[64];

		//--活力值消耗
		_stprintf( szText, _T("%u"),  pProto->dwVitalityConsume);
		m_pStcPotVal->SetText(szText);

		//--金钱消耗
		_stprintf(szText, _T("%u"), pProto->dwMoneyConsume/10000);
		m_pStcGoldCost->SetText(szText);
		_stprintf(szText, _T("%u"), pProto->dwMoneyConsume%10000);
		m_pStcSilverCost->SetText(szText);


		//--提示
		m_pDeStuffList->Clear();
		TCHAR szBuff[X_SHORT_NAME] = {0};
		for(int i=0; i<MAX_PRODUCE_STUFF_QUANTITY; i++)
		{
			DWORD dwTypeID = pProto->dwItemID[i];
			if(dwTypeID!=GT_INVALID)
			{
				const tagItemDisplayInfo* pDisplayInfo = ItemProtoData::Inst()->FindItemDisplay(dwTypeID);
				if( P_VALID(pDisplayInfo) )
				{
					//_sntprintf( szBuff, X_SHORT_NAME, _T("%d%%"), pDeComposeProto->OutputStuff[i].nRate/100 );
					//m_pDeStuffList->SetText(i, 0, szBuff);
					m_pDeStuffList->SetText(i, 0, pDisplayInfo->szName);
				}
			}
		}
		m_pBtnOk->SetEnable(true);
	}
	else
	{
		//todo:提示信息
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CannotExtract")]);
	}
}

DWORD ExtractionFrame::HandleDragStuff( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	
	if( pWnd == m_pDeUnit )
	{
		//判断是否为装备
		if(!MIsEquipment(pEvent->dwWndID))
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_NotEquip")]);
			return FALSE;
		}

		INT16 nPos = INT16(pEvent->dwParam2);
		Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItem(nPos);
		if(!P_VALID(pEquip))
			return FALSE;

		//未鉴定不能点化分解
		if( pEquip->IsNotIdetified() )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Equip_NoIdetified")]);
			return FALSE;
		}

		ClearUIandStuff();
		m_dwEquipID = pEvent->dwParam3;

		JudgeDecompositonFormula(m_dwEquipID, pEquip->GetItemId(), pEquip->GetEquipPos());
		m_pWndMain->SetRefresh(TRUE);
	}
	return TRUE;
}

void ExtractionFrame::JudgeSendDecompose()
{
	const tagExtractProto* pProto = NULL;
	pProto = ProduceData::Inst()->FindExtractProto(m_dwEquipID);	
	if( !P_VALID(pProto) )
		return;

	m_pProTime->SetSpeed(100);
	m_pProTime->SetValue(0, true);
	m_pProTime->SetMaxValue((FLOAT)pProto->dwProduceTime/1000);
	m_pProTime->SetRefresh();
	m_eCurState = EDeCompose_SendMsg;
	m_pBtnOk->SetEnable(false);
	m_pBtnCancel->SetEnable(true);
	m_pWndMain->SetRefresh(TRUE);

	// 读条音效
	TObjRef<AudioSys>()->Play2DSound(_T("make_item"), SOUND_NORMAL);
}

void ExtractionFrame::ResetStartUIState()
{
	m_eCurState = EDeCompose_NULL;
	m_pBtnOk->SetEnable(true);
	m_pBtnCancel->SetEnable(false);
	m_pProTime->SetValue(0, true);
	m_pWndMain->SetRefresh(TRUE);
}

DWORD ExtractionFrame::OnOpenExtractFrame(tagGameEvent* pGameEvent)
{
	Show();
	return 0;
}

void ExtractionFrame::SendDecomposeMsg()
{
	TCHAR szMsg[1024] = {0};
	tagNC_Extraction *pMsg = (tagNC_Extraction*)szMsg;
	pMsg->dwID		= m_pUtil->Crc32("NC_Extraction");

	pMsg->dwNPCID = m_dwNPCGolobelID;

// 	tagIMStuff imStuff;					//辅助材料
// 	m_Container.GetIMStuff(imStuff);
// 	pMsg->n64Item = imStuff.n64ID;

	tagMainStuff mainStuff;				//装备
	m_Container.GetMainStuff(mainStuff);
	pMsg->n64Item = mainStuff.n64ID;

	pMsg->dwSize = sizeof(tagNC_Extraction);

	m_pSession->Send(pMsg);
	m_eCurState = EDeCompose_Waiting;
}

DWORD ExtractionFrame::OnNS_Extraction(tagNS_Extraction* pCmd, DWORD)
{
	if(pCmd->dwErrorCode != E_Success)
	{
		switch(pCmd->dwErrorCode)
		{
			case E_Extraction_EquipInvalid:					//萃取的装备不存在
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Extraction_EquipInvalid")]);
				break;
			}
			case E_Extraction_NotEquip:						//要萃取的不是装备
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Extraction_NotEquip")]);
				break;
			}
			case E_Extraction_NoFormula:					//没有对应的萃取配方
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Extraction_NoFormula")]);
				break;
			}
			case E_Extraction_QualityLimit:					//品级限制
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Extraction_QualityLimit")]);
				break;
			}
			case E_Extraction_MoneyLimit:					//金钱限制
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Extraction_MoneyLimit")]);
				break;
			}
			case E_Extraction_VitalityLimit:				//活力限制
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Extraction_VitalityLimit")]);
				break;
			}
			case E_Extraction_BagFull:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Extraction_BagFull")]);
				break;
			}
			default:;
		}
	}	
	else 
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Compose_Consolidate_Success")]);
	}

	ClearUIandStuff();
	return 0;
}

void ExtractionFrame::Show()
{
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("OnCloseComposeFrameEvent"),NULL));

	m_dwNPCGolobelID = QuestMgr::Inst()->GetTalkNPCGlobalID();
	m_pWndMain->SetInvisible(FALSE);
	ItemFrame* pFrame = (ItemFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Item"));
	if(P_VALID(pFrame))
		pFrame->SetCurStrategy(EIST_ItemSmithCompose);
}