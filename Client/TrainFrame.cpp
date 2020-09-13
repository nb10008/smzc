#include "StdAfx.h"
#include "TrainFrame.h"
#include "..\WorldDefine\msg_train.h"
#include "ItemMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "CombatSysUtil.h"

struct tagNC_GetTrainList;
struct tagNC_FinishTrain;

TrainFrame::TrainFrame(void)
:m_Trunk(this),
m_pWnd_TrainOpen(NULL),
m_pStc_TrainName(NULL),
m_pBn_Ok(NULL),
m_pBn_Cancel(NULL),
m_pPb_UnOpen(NULL),				
m_pPb_Open(NULL),					
m_pTe_Title(NULL),					
m_pStcE_Content(NULL),				
m_pStc_TrainNameMain(NULL),		
m_pStc_Reward_Capabilitys(NULL),	
m_pStc_CurrentState(NULL),			
m_pStc_AlreadyPassed(NULL),		
m_pBn_Reward_Item1(NULL),			
m_pBn_Reward_Item2(NULL),			
m_pBn_Finish(NULL),				
m_pBn_CloseMain(NULL),
m_pBn_X(NULL),
m_bIsFirstOpen(true),
m_dwCurrTrainID(NULL),
m_pSrlCondition(NULL),
m_pWnd_Pic(NULL),
m_pStc_Pic(NULL),
TimeMark(FALSE),
m_pSrlContent(NULL),
timeCounter(NULL)
{
}

TrainFrame::~TrainFrame(void)
{
}

BOOL TrainFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;
	
	//--注册网络消息处理函数
// 	m_pCmdMgr->Register("NS_GetTrainList",		(NETMSGPROC)m_Trunk.sfp2(&TrainFrame::OnNS_FirstOpenTrainList),	_T("NS_GetTrainList"));
// 	m_pCmdMgr->Register("NS_FinishTrain",		(NETMSGPROC)m_Trunk.sfp2(&TrainFrame::OnNS_FinishTrain),		_T("NS_FinishTrain"));
// 	m_pCmdMgr->Register("NS_OpenTrain",			(NETMSGPROC)m_Trunk.sfp2(&TrainFrame::OnNS_OpenTrain),			_T("NS_OpenTrain"));
// 
// 	m_pFrameMgr->RegisterEventHandle(_T("Close_Train"), (FRAMEEVENTPROC)m_Trunk.sfp1(&TrainFrame::OnEvent_CloseTrainMain));
// 	m_pFrameMgr->RegisterEventHandle(_T("Open_Train"), (FRAMEEVENTPROC)m_Trunk.sfp1(&TrainFrame::OnEvent_OpenTrainMain));

	
	return TRUE;
}

BOOL TrainFrame::Destroy()
{
	//--注销网络消息处理函数
// 	m_pCmdMgr->UnRegister("NS_GetTrainList",		(NETMSGPROC)m_Trunk.sfp2(&TrainFrame::OnNS_FirstOpenTrainList));
// 	m_pCmdMgr->UnRegister("NS_FinishTrain",			(NETMSGPROC)m_Trunk.sfp2(&TrainFrame::OnNS_FinishTrain));
// 	m_pCmdMgr->UnRegister("NS_OpenTrain",			(NETMSGPROC)m_Trunk.sfp2(&TrainFrame::OnNS_OpenTrain));
// 
// 	m_pFrameMgr->UnRegisterEventHandler(_T("Close_Train"), (FRAMEEVENTPROC)m_Trunk.sfp1(&TrainFrame::OnEvent_CloseTrainMain));
// 	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Train"), (FRAMEEVENTPROC)m_Trunk.sfp1(&TrainFrame::OnEvent_OpenTrainMain));


	BOOL bRet = GameFrame::Destroy();

	SafeDestroyUI();
	return bRet;
}

BOOL TrainFrame::ReloadUI()
{

	//加载界面
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\try_start.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &element, "try_start_win\\try_start_pic\\directionpic\\direction", "ClassName", "AppliedTree");
	m_pGUI->ChangeXml( &element, "try_start_win\\try_start_pic\\titlelistpic\\titlelist", "ClassName", "AppliedTree");
	m_pGUI->ChangeXml( &element, "try_start_win\\try_start_pic\\tryback1\\skill1", "ClassName", "ItemButton");
	m_pGUI->ChangeXml( &element, "try_start_win\\try_start_pic\\tryback2\\skill1", "ClassName", "ItemButton");
	m_pGUI->ChangeXml( &element, "try_start_win\\try_start_pic\\word0", "ClassName", "ItemButton");
	m_pGUI->ChangeXml( &element, "try_start_win\\try_start_pic\\tryback1\\skill1", "ClassName", "ItemButton");
	m_pGUI->ChangeXml( &element, "try_start_win\\try_start_pic\\tryback2\\skill1", "ClassName", "ItemButton");
												
	m_pWnd_TrainMain = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd_TrainMain ) )
	{
		m_pGUI->DestroyWnd(m_pWnd_TrainMain);
		m_pWnd_TrainMain = NULL;		
		return FALSE;
	}
	strPath = vEngine::g_strLocalPath + _T("\\ui\\try_sure.xml");
	m_pWnd_TrainOpen = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str());
	if( !P_VALID( m_pWnd_TrainOpen )  )
	{
		m_pGUI->DestroyWnd(m_pWnd_TrainOpen);
		m_pWnd_TrainOpen = NULL;		
		return FALSE;
	}

	strPath = vEngine::g_strLocalPath + _T("\\ui\\compet_finish.xml");
	m_pWnd_Pic = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str());
	if( !P_VALID( m_pWnd_Pic )  )
	{
		m_pGUI->DestroyWnd(m_pWnd_Pic);
		m_pWnd_Pic = NULL;		
		return FALSE;
	}

	m_pGUI->RegisterEventHandler( m_pWnd_TrainOpen->GetFullName().c_str(),   m_Trunk.sfp1(&TrainFrame::OnWnd_TrainOpen));
	m_pGUI->RegisterEventHandler( m_pWnd_TrainMain->GetFullName().c_str(),   m_Trunk.sfp1(&TrainFrame::OnWnd_LoongTrainMain));

	m_pBn_Ok					= (GUIButton*)m_pWnd_TrainOpen->GetChild( _T("try_practice_pic\\surerbutt") );
	m_pBn_Cancel				= (GUIButton*)m_pWnd_TrainOpen->GetChild( _T("try_practice_pic\\closebutt") );
	m_pStc_TrainName			= (GUIButton*)m_pWnd_TrainOpen->GetChild( _T("try_practice_pic\\word1") );

	m_pPb_UnOpen				= (GUIPushButton*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\com_fra\\optbutt1"));
	m_pPb_Open					= (GUIPushButton*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\com_fra\\optbutt0"));
	m_pTe_Title					= (AppliedTree*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\directionpic\\direction"));
	m_pStcE_Content				= (GUIStaticEx*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\titlelistpic\\win\\word"));
	m_pStc_TrainNameMain		= (GUIStatic*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\pic1\\num"));
	m_pStc_Reward_Capabilitys	= (GUIStatic*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\pic2\\num"));
	m_pStc_CurrentState			= (ItemButton*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\word0"));
	m_pStc_AlreadyPassed		= (GUIStatic*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\num"));
	m_pBn_Reward_Item1			= (ItemButton*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\tryback1\\skill1"));
	m_pBn_Reward_Item2			= (ItemButton*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\tryback2\\skill1"));
	m_pBn_Finish				= (GUIButton*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\finishbutt"));
	m_pBn_CloseMain				= (GUIButton*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\shutbutt"));
	m_pBn_X						= (GUIButton*)m_pWnd_TrainMain->GetChild( _T("try_start_pic\\closebutt"));
	m_pSrlCondition				= (GUIScrollBar*)m_pWnd_TrainMain->GetChild(_T("try_start_pic\\titlelistpic\\win\\rt_scr"));
	m_pSrlContent				= (GUIScrollBar*)m_pWnd_TrainMain->GetChild(_T("try_start_pic\\directionpic\\direction\\list_scr"));

	m_pStc_Pic					= (GUIStatic*)m_pWnd_Pic->GetChild( _T("compet_finish_back\\word"));
	

	m_pWnd_TrainMain->SetInvisible( TRUE );
	m_pWnd_TrainOpen->SetInvisible( TRUE );
	m_pWnd_Pic->SetInvisible( TRUE );
	
	InitAppliedTrees();

	return TRUE;
}

VOID TrainFrame::SafeDestroyUI()
{
	if( P_VALID(m_pWnd_TrainMain) )
	{
		m_pGUI->DestroyWnd( m_pWnd_TrainMain );
		m_pWnd_TrainMain = NULL;
	}
	if( P_VALID(m_pWnd_TrainOpen) )
	{
		m_pGUI->DestroyWnd( m_pWnd_TrainOpen );
		m_pWnd_TrainOpen = NULL;
	}
	if( P_VALID(m_pWnd_Pic) )
	{
		m_pGUI->DestroyWnd( m_pWnd_Pic );
		m_pWnd_Pic = NULL;
	}
}

BOOL TrainFrame::EscCancel()
{
	if (m_pWnd_TrainMain->IsInvisible())
		return FALSE;
	m_pWnd_TrainMain->SetInvisible( TRUE );
	return TRUE;
}

BOOL TrainFrame::IsWindowVisible()
{
	BOOL bShow = m_pWnd_TrainMain->IsInvisible();
	return !bShow;
}

VOID TrainFrame::SetInvisibleOrNot()
{
	return;
}

DWORD TrainFrame::OnNS_FirstOpenTrainList(tagNS_GetTrainList* pNetCmd,DWORD dwParam)
{
	if ( !pNetCmd->eTrainErrCode )
	{
		for ( int i = 0; i <= pNetCmd->nTrainCnt ; i++)
		{
			TrainMgr::Inst()->SetNewState(pNetCmd->pTrainAttTable[i].dwTrainID,pNetCmd->pTrainAttTable[i].eTrainState);
		}
		ReloadTree();
		m_pPb_Open->SetState(EGUIBS_PushDown);		
	}

	return 0;

}

DWORD TrainFrame::OnNS_FinishTrain(tagNS_FinishTrain* pNetCmd, DWORD dwParam)
{
	if (E_Train_Pass != pNetCmd->eTrainErrCode)
	{
		ShowErrorTrainMessage(pNetCmd->eTrainErrCode);
		return 0;
	}
	//弹出试炼成功界面
	const tagTrainProtoData* data = TrainMgr::Inst()->GetData().GetTrainProto(pNetCmd->dwTrainID);
	if (!P_VALID(data))
		return FALSE;

	//m_pStc_Pic->SetText(data->Name.c_str());
	//m_pWnd_Pic->SetInvisible(FALSE);
	//m_pWnd_Pic->FlipToTop();
	//m_pWnd_Pic->SetTopMost(TRUE);
	//m_pWnd_Pic->SetRefresh(TRUE);
	//TimeMark=true;

	ShowPicCenter(data->Name);

	TrainMgr::Inst()->SetNewState(pNetCmd->dwTrainID,pNetCmd->eTrainState);
	ReloadTree();

	DWORD tempID = GT_INVALID;
	for (TrainTreeMap::iterator it = m_mapTrainTreeMap.begin();it != m_mapTrainTreeMap.end();++it)
	{
		if (it->second == m_dwCurrTrainID)
		{
			tempID = it->first;
			break;
		}
	}
	RefreshUI();
	m_pTe_Title->SetCurrentSelItem(tempID);

	return 0;
}
const VOID TrainFrame::ShowPicCenter(tstring str)
{
	m_pStc_Pic->SetText(str.c_str());
	m_pWnd_Pic->SetInvisible(FALSE);
	m_pWnd_Pic->FlipToTop();
	m_pWnd_Pic->SetTopMost(TRUE);
	m_pWnd_Pic->SetRefresh(TRUE);
	TimeMark=true;
}
DWORD TrainFrame::OnNS_OpenTrain(tagNS_OpenTrain* pNetCmd, DWORD deParam)
{
	const tagTrainProtoData* data = TrainMgr::Inst()->GetData().GetTrainProto(pNetCmd->dwTrainID);
	if (!P_VALID(data))
		return FALSE;

	m_pStc_TrainName->SetText(data->Name.c_str());
	TrainMgr::Inst()->SetNewState(pNetCmd->dwTrainID,ETrainState::ETS_Open);
	ReloadTree();

	m_pWnd_TrainOpen->SetInvisible(FALSE);
	m_pWnd_TrainOpen->FlipToTop();
	m_pWnd_TrainOpen->SetRefresh(TRUE);

	m_dwCurrTrainID = pNetCmd->dwTrainID;

	return FALSE;
}

BOOL  TrainFrame::OnWnd_TrainOpen(tagGUIEvent* pEvent)
{
	
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;
	
	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBn_Ok )
		{	
			m_pFrameMgr->SendEvent( &tagGameEvent( _T("Open_Train"), this ) );
			m_pWnd_TrainOpen->SetInvisible( TRUE );
		}
		if( pWnd == m_pBn_Cancel )
		{
			m_pWnd_TrainOpen->SetInvisible( TRUE );
		}
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL TrainFrame::OnWnd_LoongTrainMain(tagGUIEvent* pEvent)
{
	//只有第一次发消息
	if ( m_bIsFirstOpen )
	{
		tagNC_GetTrainList cmd;
		TObjRef<NetSession>()->Send(&cmd);
		m_bIsFirstOpen = false;
		m_pPb_Open->SetState(EGUIBS_PushDown);
	}

	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBn_Finish )
		{
			if ( NULL != m_dwCurrTrainID)
			{
				tagNC_FinishTrain cmd;
				cmd.dwTrainID = m_dwCurrTrainID;
				m_pSession->Send(&cmd);
			}
		}
		if( pWnd == m_pBn_CloseMain )
		{
			m_pFrameMgr->SendEvent( &tagGameEvent( _T("Close_Train_UI"), this ) );
			m_pWnd_TrainMain->SetInvisible( TRUE );
		}
		if( pWnd == m_pBn_X )
		{
			m_pFrameMgr->SendEvent( &tagGameEvent( _T("Close_Train_UI"), this ) );
			m_pWnd_TrainMain->SetInvisible( TRUE );
		}
		
		break;
	case EGUIE_ItemClick:
		if ( pWnd == m_pTe_Title )
		{
			TrainTreeMap::iterator ite = m_mapTrainTreeMap.find(pEvent->dwParam1);
			if ( ite != m_mapTrainTreeMap.end())
			{
				m_dwCurrTrainID = ite->second;
				RefreshUI();
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			const tagTrainProtoData* datas= TrainMgr::Inst()->GetData().GetTrainProto(m_dwCurrTrainID);
			if (!P_VALID(datas))
				break;
			if( pWnd == m_pBn_Reward_Item1 )
			{
				ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(datas->dwItem1ID ,(BYTE)datas->byItem1Quality).c_str() );
			}
			if( pWnd == m_pBn_Reward_Item2 )
			{
				ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(datas->dwItem2ID,(BYTE)datas->byItem2Quality).c_str() );
			}
		}
		break;
	case EGUIE_PushButtonDown:
		if( pWnd == m_pPb_UnOpen )
		{
			ReloadTree();
		}
		if( pWnd == m_pPb_Open )
		{
			ReloadTree();
		}
		break;
	default:
		break;
	}

	//通过试炼比例
	tstringstream passed;
	INT pass = 0;
	INT total = TrainMgr::Inst()->GetData().GetData().size();
	TrainProtoData::TrainProtoMap::const_iterator iter = TrainMgr::Inst()->GetData().GetData().begin();
	for (;iter != TrainMgr::Inst()->GetData().GetData().end();++iter)
	{
		if(iter->second.eType == ETS_Pass)
			pass++;
	}	
	passed << pass << _T("/") << total;
	m_pStc_AlreadyPassed->SetText(passed.str().c_str());
	return TRUE;
}

VOID TrainFrame::InitAppliedTrees()
{
	m_mapTrainTreeMap.clear();

	m_pTe_Title->InsterItem(NULL, _T("Root"), _T("Root"));

	TrainProtoData::TrainProtoMap::const_iterator iter = TrainMgr::Inst()->GetData().GetData().begin();
	for (;iter != TrainMgr::Inst()->GetData().GetData().end();++iter)
	{
		tstring strFather(_T("\\Root"));
		DWORD dwID = m_pTe_Title->InsterItem(strFather.c_str(), iter->second.Name.c_str(),iter->second.Name.c_str());
		m_mapTrainTreeMap.insert(make_pair( dwID,iter->first));
	}	

	TrainTreeMap::iterator ite = m_mapTrainTreeMap.begin();
	if (ite!= m_mapTrainTreeMap.end())
	{
		m_dwCurrTrainID = ite->second;
	}
}

void TrainFrame::OnEvent_CloseTrainMain(tagGameEvent *pEvent)
{
	m_pWnd_TrainMain->SetInvisible(true);
}
void TrainFrame::OnEvent_OpenTrainMain(tagGameEvent *pEvent)
{
	ReloadTree();
	const tagTrainProtoData* data= TrainMgr::Inst()->GetData().GetTrainProto(m_dwCurrTrainID);
	if (!P_VALID(data))
		return ;

	DWORD tempID = GT_INVALID;
	for (TrainTreeMap::iterator it = m_mapTrainTreeMap.begin();it != m_mapTrainTreeMap.end();++it)
	{
		if (it->second == m_dwCurrTrainID)
		{
			tempID = it->first;
			break;
		}
	}
	RefreshUI();
	m_pTe_Title->SetCurrentSelItem(tempID);

	m_pWnd_TrainMain->SetInvisible(false);
}
VOID TrainFrame::ReloadTree()
{
	m_pTe_Title->RemoveItem( m_pUtil->Crc32( _T("\\Root") ) );

	m_pTe_Title->InsterItem(NULL, _T("Root"), _T("Root"));

	//如果现在显示的是已经开启的状态
	if (m_pPb_Open->GetState() == EGUIBS_PushDown)
	{
		TrainProtoData::TrainProtoMap::const_iterator iter = TrainMgr::Inst()->GetData().GetData().begin();
		for (;iter != TrainMgr::Inst()->GetData().GetData().end();++iter)
		{
			if (iter->second.eType == ETS_Open)
			{
				tstring strFather(_T("\\Root"));
				DWORD dwID = m_pTe_Title->InsterItem(strFather.c_str(), iter->second.Name.c_str(),iter->second.Name.c_str());
			}
		}
		TrainProtoData::TrainProtoMap::const_iterator iter2 = TrainMgr::Inst()->GetData().GetData().begin();
		for (;iter2 != TrainMgr::Inst()->GetData().GetData().end();++iter2)
		{
			if (iter2->second.eType == ETS_Pass)
			{
				tstring strFather(_T("\\Root"));
				tstringstream pass;
				pass << iter2->second.Name << _T(" [通过]");
				DWORD dwID = m_pTe_Title->InsterItem(strFather.c_str(), iter2->second.Name.c_str(),pass.str().c_str());
			}
		}	
		m_pBn_Finish->SetEnable(TRUE);
	}
	if (m_pPb_UnOpen->GetState() == EGUIBS_PushDown)
	{
		TrainProtoData::TrainProtoMap::const_iterator iter = TrainMgr::Inst()->GetData().GetData().begin();
		for (;iter != TrainMgr::Inst()->GetData().GetData().end();++iter)
		{
			if (iter->second.eType == ETS_Close)
			{
				tstring strFather(_T("\\Root"));
				DWORD dwID = m_pTe_Title->InsterItem(strFather.c_str(), iter->second.Name.c_str(),iter->second.Name.c_str());
			}
		}	
		m_pBn_Finish->SetEnable(FALSE);
	}
	m_pTe_Title->SetRefresh(true);
}

VOID TrainFrame::RefreshUI()
{
	const tagTrainProtoData* data= TrainMgr::Inst()->GetData().GetTrainProto(m_dwCurrTrainID);
	if (!P_VALID(data))
		return;
	//试炼状态
	ETrainState state = data->eType;
	switch (state)
	{
	case ETS_Close:
		{
			//试炼描述
			tstringstream streams;
			streams 	<< g_StrTable[_T("Train_OpenDesc")]	<<_T("\\n")
				<< data->StartDescription ;
			m_pStcE_Content->SetText(streams.str().c_str());
			m_pStc_CurrentState->SetPicFileName(LOCK_PIC);
			m_pBn_Finish->SetEnable(FALSE);
		}
		break;
	case ETS_Open:
		{
			//试炼描述
			tstringstream streams;
			streams << g_StrTable[_T("Train_GoalDesc")] <<_T("\\n")
				<< data->GoalDescription	<<_T("\\n\\n")
				<< g_StrTable[_T("Train_FinishDesc")]	<<_T("\\n")
				<< data->FinishDescription ;
			m_pStcE_Content->SetText(streams.str().c_str());
			m_pStc_CurrentState->SetPicFileName(PROCEED_PIC);
			m_pBn_Finish->SetEnable(TRUE);
		}
		break;
	case ETS_Pass:
		{
			//试炼描述
			tstringstream streams;
			streams << g_StrTable[_T("Train_GoalDesc")] <<_T("\\n")
				<< data->GoalDescription	<<_T("\\n\\n")
				<< g_StrTable[_T("Train_FinishDesc")]	<<_T("\\n")
				<< data->FinishDescription ;
			m_pStcE_Content->SetText(streams.str().c_str());
			m_pStc_CurrentState->SetPicFileName(PASS_PIC);
			m_pBn_Finish->SetEnable(FALSE);
		}
		break;
	default:
		break;
	}
	tagPoint size = m_pStcE_Content->GetSize();
	if( m_pStcE_Content->GetTextTotalHeight() > size.y )
	{
		size.y = m_pStcE_Content->GetTextTotalHeight();
		m_pStcE_Content->SetResizable(TRUE);
		m_pStcE_Content->Resize( size );
		m_pStcE_Content->SetResizable(FALSE);
	}
	m_pStc_CurrentState->SetRefresh(true);
	//试炼奖励实力值
	tstringstream cap;
	cap << data->nCapability;
	m_pStc_Reward_Capabilitys->SetText(cap.str().c_str());
	m_pStc_Reward_Capabilitys->SetRefresh(true);
	//试炼名称
	tstring name = data->Name;
	m_pStc_TrainNameMain->SetText(name.c_str());
	m_pStc_TrainNameMain->SetRefresh(true);

	//奖励物品1
	m_pBn_Reward_Item1->RefreshItem(data->dwItem1ID,data->nItem1Num,data->byItem1Quality);
	//奖励物品2
	m_pBn_Reward_Item2->RefreshItem(data->dwItem2ID,data->nItem2Num,data->byItem2Quality);

}

VOID TrainFrame::AutoMessage( DWORD dwTimedelay)
{
	if (TimeMark){
		timeCounter = Kernel::Inst()->GetAccumTimeDW();
		TimeMark = false;
	}
	if (Kernel::Inst()->GetAccumTimeDW() - timeCounter > dwTimedelay)
	{
		m_pWnd_Pic->SetInvisible(TRUE);
	}
}
VOID TrainFrame::Update()
{
	AutoMessage(SHOWTIME);
}
VOID TrainFrame::ShowErrorTrainMessage(ETrainErrCode error)
{
	switch (error)
	{
	case E_Train_Bag_Full:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Train_Bag_Full")]);
		break;
	case E_Train_List_Null:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_Train_List_Null")]);
		break;
	default:
		break;
	}
}
