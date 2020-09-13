#include "StdAfx.h"
#include "AchievementFrame.h"
#include "ItemButton.h"
#include "AppliedTree.h"
#include "TrainProtoData.h"
#include "TrainMgr.h"
#include "..\WorldDefine\currency_define.h"
#include "IconStatic.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_train.h"
#include "ItemMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "LevelUpShowFrame.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "GUIEffectMgr.h"

const TCHAR* szUnopen = _T("data\\ui\\Common\\L-weikaiqi.bmp");
const TCHAR* szDone= _T("data\\ui\\Common\\L-yidacheng.bmp");
const DWORD dwOrgenClour = 0xFFFFAE00;	//  橙色
const DWORD GrayColour = 0xFF808080;	//  灰色
const DWORD NormalColor = 16777215;		// 普通颜色
const DWORD SHOW_TIME = 4000;
const DWORD SHOW_SPACE = 500;

AchievementFrame::AchievementFrame(void):m_Trunk(this)
{
}

AchievementFrame::~AchievementFrame(void)
{
}


BOOL AchievementFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pFrameMgr->RegisterEventHandle(_T("Close_Train"), (FRAMEEVENTPROC)m_Trunk.sfp1(&AchievementFrame::OnEvent_CloseTrainMain));
	m_pFrameMgr->RegisterEventHandle(_T("Open_Train"), (FRAMEEVENTPROC)m_Trunk.sfp1(&AchievementFrame::OnEvent_OpenTrainMain));

	m_pCmdMgr->Register("NS_GetTrainList",		(NETMSGPROC)m_Trunk.sfp2(&AchievementFrame::OnNS_FirstOpenTrainList),	_T("NS_GetTrainList"));
	m_pCmdMgr->Register("NS_FinishTrain",		(NETMSGPROC)m_Trunk.sfp2(&AchievementFrame::OnNS_FinishTrain),		_T("NS_FinishTrain"));
	m_pCmdMgr->Register("NS_OpenTrain",			(NETMSGPROC)m_Trunk.sfp2(&AchievementFrame::OnNS_OpenTrain),			_T("NS_OpenTrain"));
	m_pCmdMgr->Register("NS_SyncTrainCompleteness",			(NETMSGPROC)m_Trunk.sfp2(&AchievementFrame::OnNS_SyncTrainCompleteness),_T("NS_SyncTrainCompleteness"));

	m_pPbShowDone->SetState(EGUIBS_PushDown);
	m_pPbShowUnOpen->SetState(EGUIBS_PushDown);
	m_nCurPage = 0;
	m_bNeedSpace = false;
	m_pOpenWnd->SetInvisible(TRUE);
	m_dwTimeMark = 5000;
	m_pOpenWnd->SetUnClickable(TRUE);

	return TRUE;
}

BOOL AchievementFrame::Destroy()
{
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_Train"), (FRAMEEVENTPROC)m_Trunk.sfp1(&AchievementFrame::OnEvent_CloseTrainMain));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Train"), (FRAMEEVENTPROC)m_Trunk.sfp1(&AchievementFrame::OnEvent_OpenTrainMain));

	m_pCmdMgr->UnRegister("NS_GetTrainList",		(NETMSGPROC)m_Trunk.sfp2(&AchievementFrame::OnNS_FirstOpenTrainList));
	m_pCmdMgr->UnRegister("NS_FinishTrain",			(NETMSGPROC)m_Trunk.sfp2(&AchievementFrame::OnNS_FinishTrain));
	m_pCmdMgr->UnRegister("NS_OpenTrain",			(NETMSGPROC)m_Trunk.sfp2(&AchievementFrame::OnNS_OpenTrain));
	m_pCmdMgr->UnRegister("NS_SyncTrainCompleteness",			(NETMSGPROC)m_Trunk.sfp2(&AchievementFrame::OnNS_SyncTrainCompleteness));

	BOOL bRet = GameFrame::Destroy();

	if( P_VALID(m_pMainWnd) )
	{
		m_pGUI->DestroyWnd( m_pMainWnd );
		m_pMainWnd = NULL;
	}

	if( P_VALID(m_pOpenWnd) )
	{
		m_pGUI->DestroyWnd( m_pOpenWnd );
		m_pOpenWnd = NULL;
	}

	return bRet;
}

BOOL AchievementFrame::ReloadUI()
{

	//加载界面
 	XmlElement element;
 	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\success.xml");
 	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
 		return FALSE;
 
 	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\directionpic\\direction", "ClassName", "AppliedTree");
 	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback00\\pic_back1\\pic", "ClassName", "ItemButton");
 	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback00\\pic_back2\\pic", "ClassName", "ItemButton");
 	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback0000\\pic_back30\\pic", "ClassName", "ItemButton");
 	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback0000\\pic_back3\\pic", "ClassName", "ItemButton");
 	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback000\\pic_back30\\pic", "ClassName", "ItemButton");
 	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback000\\pic_back3\\pic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback00\\pic3", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback0000\\pic3", "ClassName", "IconStatic");
	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback000\\pic3", "ClassName", "IconStatic");
/*
	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\title\\name2", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback00\\word1", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\title0\\name2", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback0000\\word10", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\title1\\name2", "ClassName", "GUIStaticEx");
	m_pGUI->ChangeXml( &element, "successwin\\success_pic\\listback0\\listback000\\word10", "ClassName", "GUIStaticEx");
*/
 	m_pMainWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
 	if( !P_VALID( m_pMainWnd ) )
 	{
 		return FALSE;
 	}

	strPath = vEngine::g_strLocalPath + _T("\\ui\\achievement.xml");
	m_pOpenWnd = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str());
	if( !P_VALID( m_pOpenWnd )  )
	{
		return FALSE;
	}

 	m_pGUI->RegisterEventHandler( m_pMainWnd->GetFullName().c_str(),   m_Trunk.sfp1(&AchievementFrame::EventHandler));

	m_strAchievement[0].pStcName = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\title\\name1") );
	m_strAchievement[0].pStcOpenCondition = (GUIStaticEx*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\title\\name2") );
	m_strAchievement[0].pStcDesc = (GUIStaticEx*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\word1") );
	m_strAchievement[0].pStcEContent = (GUIStaticEx*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\word2") );
	m_strAchievement[0].pIbRewardOne = (ItemButton*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\pic_back1\\pic") );
	m_strAchievement[0].pIbRewardTwo = (ItemButton*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\pic_back2\\pic") );
	m_strAchievement[0].pStcExp = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\num1") );
	m_strAchievement[0].pStcGold = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\num2") );
	m_strAchievement[0].pStcSilver = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\num3") );
	m_strAchievement[0].pBnTake = (GUIButton*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\butt") );
	m_strAchievement[0].pStcFinish = (IconStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\pic3") );
	m_strAchievement[0].pStcAlreadyTake = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\pic4") );
	m_strAchievement[0].pStcItem1Back = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\pic_back1") );
	m_strAchievement[0].pStcItem2Back = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00\\pic_back2") );
	m_strAchievement[0].pPthBackUp = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\title") );
	m_strAchievement[0].pPthBackDown = (GUIPatch*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback00") );

	m_strAchievement[1].pStcName = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\title0\\name1") );
	m_strAchievement[1].pStcOpenCondition = (GUIStaticEx*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\title0\\name2") );
	m_strAchievement[1].pStcDesc = (GUIStaticEx*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\word10") );
	m_strAchievement[1].pStcEContent = (GUIStaticEx*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\new") );
	m_strAchievement[1].pIbRewardOne = (ItemButton*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\pic_back30\\pic") );
	m_strAchievement[1].pIbRewardTwo = (ItemButton*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\pic_back3\\pic") );
	m_strAchievement[1].pStcExp = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\num1") );
	m_strAchievement[1].pStcGold = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\num2") );
	m_strAchievement[1].pStcSilver = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\num3") );
	m_strAchievement[1].pBnTake = (GUIButton*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\butt") );
	m_strAchievement[1].pStcFinish = (IconStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\pic3") );
	m_strAchievement[1].pStcAlreadyTake = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\pic4") );
	m_strAchievement[1].pStcItem1Back = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\pic_back30") );
	m_strAchievement[1].pStcItem2Back = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000\\pic_back3") );
	m_strAchievement[1].pPthBackUp = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\title0") );
	m_strAchievement[1].pPthBackDown = (GUIPatch*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback0000") );

	m_strAchievement[2].pStcName = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\title1\\name1") );
	m_strAchievement[2].pStcOpenCondition = (GUIStaticEx*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\title1\\name2") );
	m_strAchievement[2].pStcDesc = (GUIStaticEx*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\word10") );
	m_strAchievement[2].pStcEContent = (GUIStaticEx*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\new") );
	m_strAchievement[2].pIbRewardOne = (ItemButton*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\pic_back30\\pic") );
	m_strAchievement[2].pIbRewardTwo = (ItemButton*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\pic_back3\\pic") );
	m_strAchievement[2].pStcExp = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\num1") );
	m_strAchievement[2].pStcGold = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\num2") );
	m_strAchievement[2].pStcSilver = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\num3") );
	m_strAchievement[2].pBnTake = (GUIButton*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\butt") );
	m_strAchievement[2].pStcFinish = (IconStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\pic3") );
	m_strAchievement[2].pStcAlreadyTake = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\pic4") );
	m_strAchievement[2].pStcItem1Back = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\pic_back30") );
	m_strAchievement[2].pStcItem2Back = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000\\pic_back3") );
	m_strAchievement[2].pPthBackUp = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\title1") );
	m_strAchievement[2].pPthBackDown = (GUIPatch*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\listback000") );


	m_pPbShowDone = (GUIPushButton*)m_pMainWnd->GetChild( _T("success_pic\\optbutt1") );
	m_pPbShowUnOpen = (GUIPushButton*)m_pMainWnd->GetChild( _T("success_pic\\optbutt2") );
	m_pTree		  = (AppliedTree*)m_pMainWnd->GetChild( _T("success_pic\\directionpic\\direction") );

	m_pStcPercent = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\num1") );
	m_pStcCurentPage = (GUIStatic*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\pagenum") );

	m_pPageUp = (GUIButton*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\butt1") );
	m_pPageDown = (GUIButton*)m_pMainWnd->GetChild( _T("success_pic\\listback0\\butt2") );
	m_pClose = (GUIButton*)m_pMainWnd->GetChild( _T("success_pic\\closebutt") );

	m_pOpenText = (GUIStatic*)m_pOpenWnd->GetChild( _T("achievement_pic\\achievement") );

	m_strAchievement[0].pStcAlreadyTake->FlipToTop();
	m_strAchievement[0].pStcAlreadyTake->SetUnClickable(TRUE);
	m_strAchievement[1].pStcAlreadyTake->FlipToTop();
	m_strAchievement[1].pStcAlreadyTake->SetUnClickable(TRUE);
	m_strAchievement[2].pStcAlreadyTake->FlipToTop();
	m_strAchievement[2].pStcAlreadyTake->SetUnClickable(TRUE);

	m_strAchievement[0].pStcFinish->SetUnClickable(TRUE);
	m_strAchievement[0].pStcFinish->SetInvisible(TRUE);
	m_strAchievement[1].pStcFinish->SetUnClickable(TRUE);
	m_strAchievement[1].pStcFinish->SetInvisible(TRUE);
	m_strAchievement[2].pStcFinish->SetUnClickable(TRUE);
	m_strAchievement[2].pStcFinish->SetInvisible(TRUE);

 	InitAppliedTrees();

	m_pMainWnd->SetInvisible(TRUE);
	return TRUE;
}

BOOL AchievementFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		for (int i =0;i<MAX_SHOW_NUM;i++)
		{
			if( pWnd == m_strAchievement[i].pBnTake )
			{
				if ( NULL != m_dwCurrTrainID)
				{
					INT nStartPos = MAX_SHOW_NUM*m_nCurPage;
					const tagTrainProtoData* data= TrainMgr::Inst()->GetData().GetTrainProto(m_vecAchieID[nStartPos+i]);
					if (!P_VALID(data))
						break;

					tagNC_FinishTrain cmd;
					cmd.dwTrainID = data->dwTrainID;
					m_pSession->Send(&cmd);
					break;
				}
			}
		}
		if( pWnd == m_pClose )
		{
			m_pFrameMgr->SendEvent( &tagGameEvent( _T("Close_Train_UI"), this ) );
			m_pMainWnd->SetInvisible( TRUE );
		}
		if( pWnd == m_pPageUp )
		{
			m_nCurPage--;
			if(m_nCurPage < 0)
				m_nCurPage = 0;

			if (IfCanShow())
			{
				RefreshUI();
			}
		}
		if( pWnd == m_pPageDown )
		{
			m_nCurPage++;
			if (m_nCurPage*MAX_SHOW_NUM >= m_vecAchieID.size())
			{
				m_nCurPage--;
				break;
			}
			if (IfCanShow())
			{
				RefreshUI();
			}
		}
		break;
	case EGUIE_ItemClick:
		if ( pWnd == m_pTree )
		{
			TrainTreeMap::iterator ite = m_mapTrainTreeMap.find(pEvent->dwParam1);
			if ( ite != m_mapTrainTreeMap.end())
			{
				m_dwCurrTrainID = ite->second;
				if (IfCanShow())
				{
					m_nCurPage = 0;
					ReloadIDVector();
					RefreshUI();
				}
			}
		}
		break;
	case EGUIE_PushButtonDown:
		if( pWnd == m_pPbShowDone )
		{
			if (IfCanShow())
			{
				m_nCurPage = 0;
				ReloadIDVector();
				RefreshUI();
			}
		}
		if( pWnd == m_pPbShowUnOpen )
		{
			if (IfCanShow())
			{
				m_nCurPage = 0;
				ReloadIDVector();
				RefreshUI();
			}
		}
		break;
	case EGUIE_Drag:
		{
// 			for (int i =0;i<MAX_SHOW_NUM;i++)
// 			{
// 				if( pWnd == m_strAchievement[i].pBnTake )
// 				{
// 					if ( NULL != m_dwCurrTrainID)
// 					{
// 						INT nStartPos = MAX_SHOW_NUM*m_nCurPage;
// 						const tagTrainProtoData* data= TrainMgr::Inst()->GetData().GetTrainProto(m_vecAchieID[nStartPos+i]);
// 						if (!P_VALID(data))
// 							break;
// 
// 						tagNC_FinishTrain cmd;
// 						cmd.dwTrainID = data->dwTrainID;
// 						m_pSession->Send(&cmd);
// 					}
// 				}
// 			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for (int i =0;i<MAX_SHOW_NUM;i++)
			{
				if( pWnd == m_strAchievement[i].pIbRewardOne )
				{
					if ( NULL != m_dwCurrTrainID)
					{
						INT nStartPos = MAX_SHOW_NUM*m_nCurPage;
						const tagTrainProtoData* data= TrainMgr::Inst()->GetData().GetTrainProto(m_vecAchieID[nStartPos+i]);
						if (!P_VALID(data))
							break;

						ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(data->dwItem1ID ,(BYTE)data->byItem1Quality).c_str() );
					}
				}
				else if (pWnd == m_strAchievement[i].pIbRewardTwo)
				{
					if ( NULL != m_dwCurrTrainID)
					{
						INT nStartPos = MAX_SHOW_NUM*m_nCurPage;
						const tagTrainProtoData* data= TrainMgr::Inst()->GetData().GetTrainProto(m_vecAchieID[nStartPos+i]);
						if (!P_VALID(data))
							break;

						ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(data->dwItem2ID ,(BYTE)data->byItem2Quality).c_str() );
					}
				}
			}
		}
		break;
	default:
		break;

		
	}

	return TRUE;
}

VOID AchievementFrame::InitAppliedTrees()
{
	m_pTree->InsterItem(NULL, _T("Root"), _T("Root"));
	m_pTree->InsterItem(_T("\\Root"), _T("Chengjiu_Level"), g_StrTable[_T("Chengjiu_Level")],0,dwOrgenClour);
	//m_pTree->InsterItem(_T("\\Root"), _T("Chengjiu_Spec"), g_StrTable[_T("Chengjiu_Spec")],0,dwOrgenClour);
	tstring strLevel(_T("\\Root\\Chengjiu_Level"));
	tstring strSpec(_T("\\Root\\Chengjiu_Spec"));

	TrainProtoData::TrainProtoMap::const_iterator iter = TrainMgr::Inst()->GetData().GetData().begin();
	for (;iter != TrainMgr::Inst()->GetData().GetData().end();++iter)
	{
		DWORD dwID = 0;
		if (iter->second.nLevelOneType == 1)
		{
			dwID = m_pTree->InsterItem(strLevel.c_str(), iter->second.DescLevelTwo.c_str(),iter->second.DescLevelTwo.c_str());
		}
		else if (iter->second.nLevelOneType == 2)
		{
			//dwID = m_pTree->InsterItem(strSpec.c_str(), iter->second.DescLevelTwo.c_str(),iter->second.DescLevelTwo.c_str());
		}
		m_mapTrainTreeMap.insert(make_pair( dwID,iter->second.nLevelTwoType));
	}	

// 	TrainTreeMap::iterator ite = m_mapTrainTreeMap.begin();
// 	if (ite!= m_mapTrainTreeMap.end())
// 	{
// 		m_dwCurrTrainID = ite->second;
// 	}
	m_dwCurrTrainID = 10;

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
	m_pTree->SetCurrentSelItem(tempID);
	m_nCurPage = 0;
}

VOID AchievementFrame::ReloadIDVector()
{
	m_vecAchieID.clear();
	TrainProtoData::TrainProtoMap::const_iterator iter = TrainMgr::Inst()->GetData().GetData().begin();
	for (;iter != TrainMgr::Inst()->GetData().GetData().end();++iter)
	{
		if (iter->second.nLevelTwoType == m_dwCurrTrainID)
		{
			// 判断是否可见
			if(!iter->second.bVisible && (iter->second.eType == ETS_Close || iter->second.eType == ETS_Open))
				continue;
			if (iter->second.eType == ETS_Pass && m_pPbShowDone->GetState() != EGUIBS_PushDown)
				continue;
			if (iter->second.eType == ETS_Close && m_pPbShowUnOpen->GetState() != EGUIBS_PushDown)
				continue;
			m_vecAchieID.push_back(iter->second.dwTrainID);
		}
	}

	std::vector<DWORD> m_TempVec;
	m_TempVec.clear();
	swap(m_TempVec,m_vecAchieID);

	for (std::vector<DWORD>::iterator iter = m_TempVec.begin(); 
		iter != m_TempVec.end(); ++iter)
	{
		const tagTrainProtoData* data= TrainMgr::Inst()->GetData().GetTrainProto(*iter);
		if (!P_VALID(data))
			continue;

		if (data->eType == ETS_Finish)
			m_vecAchieID.push_back(*iter);
	}

	for (std::vector<DWORD>::iterator iter = m_TempVec.begin(); 
		iter != m_TempVec.end(); ++iter)
	{
		const tagTrainProtoData* data= TrainMgr::Inst()->GetData().GetTrainProto(*iter);
		if (!P_VALID(data))
			continue;

		if (data->eType == ETS_Open)
			m_vecAchieID.push_back(*iter);
	}

	for (std::vector<DWORD>::iterator iter = m_TempVec.begin(); 
		iter != m_TempVec.end(); ++iter)
	{
		const tagTrainProtoData* data= TrainMgr::Inst()->GetData().GetTrainProto(*iter);
		if (!P_VALID(data))
			continue;

		if (data->eType == ETS_Close)
			m_vecAchieID.push_back(*iter);
	}

	for (std::vector<DWORD>::iterator iter = m_TempVec.begin(); 
		iter != m_TempVec.end(); ++iter)
	{
		const tagTrainProtoData* data= TrainMgr::Inst()->GetData().GetTrainProto(*iter);
		if (!P_VALID(data))
			continue;

		if (data->eType == ETS_Pass)
			m_vecAchieID.push_back(*iter);
	}
}
BOOL AchievementFrame::EscCancel()
{
	if (m_pMainWnd->IsInvisible())
		return FALSE;
	else{
		m_pFrameMgr->SendEvent( &tagGameEvent( _T("Close_Train_UI"), this ) );
		m_pMainWnd->SetInvisible( TRUE );
	}
	
	return TRUE;
}
VOID AchievementFrame::Update()
{
	 if(!((LevelUpShowFrame*)(TObjRef<GameFrameMgr>()->GetFrame(_T("LevelUpShow"))))->IsWndInvisible())
		 return;

	m_dwTimeMark += Kernel::Inst()->GetDeltaTimeDW();
	if (!m_pOpenWnd->IsInvisible())
	{
		if (m_dwTimeMark >= SHOW_TIME)
			m_pOpenWnd->SetInvisible(TRUE);
	}
	else
	{
		if (m_vecShowVector.empty())
			return;

		if (m_bNeedSpace && m_dwTimeMark < SHOW_SPACE)
		{
			m_pOpenWnd->SetInvisible(TRUE);
		}
		else
			m_bNeedSpace = false;

		if (m_dwTimeMark > SHOW_TIME)
		{
			ShowPic(m_vecShowVector.front());
			m_vecShowVector.pop_front();
			m_bNeedSpace = true;
		}
	}
}

VOID AchievementFrame::ShowPic(tstring str)
{
	m_pOpenWnd->SetInvisible(FALSE);
	m_pOpenText->SetText(str.c_str());

	m_dwTimeMark = 0;
}

VOID AchievementFrame::RefreshUI()
{
	INT nStartPos = MAX_SHOW_NUM*m_nCurPage;
	for (int i =0; i<MAX_SHOW_NUM ;i++)
	{
		if (m_vecAchieID.size() <= nStartPos +i)
		{
			ClearInfo(m_strAchievement[i]);
			continue;
		}
		
		m_strAchievement[i].pPthBackUp->SetInvisible(FALSE);
		m_strAchievement[i].pPthBackDown->SetInvisible(FALSE);

		const tagTrainProtoData* data= TrainMgr::Inst()->GetData().GetTrainProto(m_vecAchieID[nStartPos+i]);
		if (!P_VALID(data))
			return;

		TCHAR szText[X_SHORT_NAME];

		ETrainState state = data->eType;
		switch (state)
		{
		case ETS_Close:
			{
				m_strAchievement[i].pStcAlreadyTake->SetInvisible(TRUE);
				m_strAchievement[i].pStcFinish->SetInvisible(FALSE);
				m_strAchievement[i].pStcFinish->SetPic(szUnopen);
				m_strAchievement[i].pBnTake->SetEnable(FALSE);
				m_strAchievement[i].pStcOpenCondition->SetText(data->StartDescription.c_str());
				
			}
			break;
		case ETS_Open:
			{
				m_strAchievement[i].pStcAlreadyTake->SetInvisible(TRUE);
				m_strAchievement[i].pStcFinish->SetInvisible(TRUE);
				m_strAchievement[i].pBnTake->SetEnable(FALSE);

				if (data->nCurrentNum > data->nMaxNum)
					_stprintf(szText,g_StrTable[_T("Achievment_PassPercrnt")],data->nMaxNum,data->nMaxNum);
				else
					_stprintf(szText,g_StrTable[_T("Achievment_PassPercrnt")],data->nCurrentNum,data->nMaxNum);

				m_strAchievement[i].pStcOpenCondition->SetText(szText);
			}
			break;
		case ETS_Pass:
			{
 				m_strAchievement[i].pStcAlreadyTake->SetInvisible(FALSE);
				m_strAchievement[i].pStcFinish->SetInvisible(FALSE);
 				m_strAchievement[i].pStcFinish->SetPic(szDone);
				m_strAchievement[i].pBnTake->SetEnable(FALSE);

				if (data->nCurrentNum > data->nMaxNum)
					_stprintf(szText,g_StrTable[_T("Achievment_PassPercrnt")],data->nMaxNum,data->nMaxNum);
				else
					_stprintf(szText,g_StrTable[_T("Achievment_PassPercrnt")],data->nCurrentNum,data->nMaxNum);

				m_strAchievement[i].pStcOpenCondition->SetText(szText);
			}
			break;
		case ETS_Finish:
			{
				m_strAchievement[i].pStcAlreadyTake->SetInvisible(TRUE);
				m_strAchievement[i].pStcFinish->SetInvisible(FALSE);
				m_strAchievement[i].pStcFinish->SetPic(szDone);
				m_strAchievement[i].pBnTake->SetEnable(TRUE);

				if (data->nCurrentNum > data->nMaxNum)
					_stprintf(szText,g_StrTable[_T("Achievment_PassPercrnt")],data->nMaxNum,data->nMaxNum);
				else
					_stprintf(szText,g_StrTable[_T("Achievment_PassPercrnt")],data->nCurrentNum,data->nMaxNum);

				m_strAchievement[i].pStcOpenCondition->SetText(szText);
			}
			break;
		default:
			break;
		}
		

		m_strAchievement[i].pStcName->SetText(data->Name.c_str());
		//m_strAchievement[i].pStcOpenCondition->SetText(data->StartDescription.c_str());
		m_strAchievement[i].pStcDesc->SetText(data->GoalDescription.c_str());
		m_strAchievement[i].pStcEContent->SetText(data->FinishDescription.c_str());
		if (data->dwItem1ID)
		{
			m_strAchievement[i].pIbRewardOne->RefreshItem(data->dwItem1ID,data->nItem1Num,data->byItem1Quality);
			m_strAchievement[i].pIbRewardOne->SetInvisible(FALSE);
			m_strAchievement[i].pStcItem1Back->SetInvisible(FALSE);
		}
		else
		{
			m_strAchievement[i].pIbRewardOne->SetInvisible(TRUE);
			m_strAchievement[i].pStcItem1Back->SetInvisible(TRUE);
		}
			
			
		if (data->dwItem2ID)
		{
			m_strAchievement[i].pIbRewardTwo->RefreshItem(data->dwItem2ID,data->nItem2Num,data->byItem2Quality);
			m_strAchievement[i].pIbRewardTwo->SetInvisible(FALSE);
			m_strAchievement[i].pStcItem2Back->SetInvisible(FALSE);
		}
		else
		{
			m_strAchievement[i].pIbRewardTwo->SetInvisible(TRUE);
			m_strAchievement[i].pStcItem2Back->SetInvisible(TRUE);
		}
		
		_stprintf( szText, _T("%d"),  data->dwExp);
		m_strAchievement[i].pStcExp->SetText(szText);
		INT64 n64Silver = data->dwMoney;
		int nGold = MSilver2DBGold(n64Silver);
		int nSilver = MSilver2DBSilver(n64Silver);
		_sntprintf(szText, X_SHORT_NAME, _T("%d"), nGold);
		m_strAchievement[i].pStcGold->SetText(szText);
		_sntprintf(szText, X_SHORT_NAME, _T("%d"), nSilver);
		m_strAchievement[i].pStcSilver->SetText(szText);
		m_strAchievement[i].pStcAlreadyTake->FlipToTop();

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
		m_pStcPercent->SetText(passed.str().c_str());

		_stprintf( szText, _T("%d"),  m_nCurPage+1);
		m_pStcCurentPage->SetText(szText);
	}
	
}

void AchievementFrame::OnEvent_CloseTrainMain(tagGameEvent *pEvent)
{
	m_pMainWnd->SetInvisible(TRUE);
}
void AchievementFrame::OnEvent_OpenTrainMain(tagGameEvent *pEvent)
{
	ReloadTrees();

	m_dwCurrTrainID = 10;

	DWORD tempID = GT_INVALID;
	for (TrainTreeMap::iterator it = m_mapTrainTreeMap.begin();it != m_mapTrainTreeMap.end();++it)
	{
		if (it->second == m_dwCurrTrainID)
		{
			tempID = it->first;
			break;
		}
	}
	ReloadIDVector();
	RefreshUI();
	m_pTree->SetCurrentSelItem(tempID);
	m_nCurPage = 0;

	m_pMainWnd->SetInvisible(FALSE);
}

DWORD AchievementFrame::OnNS_FirstOpenTrainList(tagNS_GetTrainList* pNetCmd,DWORD dwParam)
{
	if ( !pNetCmd->eTrainErrCode )
	{
		for ( int i = 0; i <= pNetCmd->nTrainCnt ; i++)
		{
			TrainMgr::Inst()->SetNewState(pNetCmd->pTrainAttTable[i].dwTrainID,pNetCmd->pTrainAttTable[i].eTrainState);
			TrainMgr::Inst()->SetCompletNum(pNetCmd->pTrainAttTable[i].dwTrainID,pNetCmd->pTrainAttTable[i].nCompleteness);
		}	
	}
	ReloadIDVector();
	RefreshUI();
	return 0;

}

DWORD AchievementFrame::OnNS_FinishTrain(tagNS_FinishTrain* pNetCmd, DWORD dwParam)
{
	if (E_Train_Pass != pNetCmd->eTrainErrCode  && E_Train_Success != pNetCmd->eTrainErrCode)
	{
		ShowErrorTrainMessage(pNetCmd->eTrainErrCode);
		return 0;
	}
	//弹出试炼成功界面
	const tagTrainProtoData* data = TrainMgr::Inst()->GetData().GetTrainProto(pNetCmd->dwTrainID);
	if (!P_VALID(data))
		return FALSE;

	if (pNetCmd->eTrainState == ETS_Finish)
		m_vecShowVector.push_back(data->Name);
	
	TrainMgr::Inst()->SetNewState(pNetCmd->dwTrainID,pNetCmd->eTrainState);
	//ReloadIDVector();
	RefreshUI();

	if (pNetCmd->eTrainState == ETS_Finish)
	{
		GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
		GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui025"),pDesktop->GetView(),pDesktop->GetSize());
	}

	return 0;
}

VOID AchievementFrame::ShowErrorTrainMessage(ETrainErrCode error)
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

DWORD AchievementFrame::OnNS_OpenTrain(tagNS_OpenTrain* pNetCmd, DWORD deParam)
{
	const tagTrainProtoData* data = TrainMgr::Inst()->GetData().GetTrainProto(pNetCmd->dwTrainID);
	if (!P_VALID(data))
		return FALSE;

	TrainMgr::Inst()->SetNewState(pNetCmd->dwTrainID,ETrainState::ETS_Open);

	ReloadIDVector();
	RefreshUI();

	m_pFrameMgr->SendEvent( &tagGameEvent( _T("Flash_Train"), NULL ) );

	GUIWnd* pDesktop = TObjRef<GUISystem>()->GetDesktop();
	GUIEffectMgr::Inst()->PlayScreenEffect(_T("txui024"),pDesktop->GetView(),pDesktop->GetSize());

	return FALSE;
}

DWORD AchievementFrame::OnNS_SyncTrainCompleteness(tagNS_SyncTrainCompleteness* pNetCmd, DWORD deParam)
{
	const tagTrainProtoData* data = TrainMgr::Inst()->GetData().GetTrainProto(pNetCmd->dwTrainID);
	if (!P_VALID(data))
		return FALSE;

	TrainMgr::Inst()->SetCompletNum(pNetCmd->dwTrainID,pNetCmd->dwCompleteness);

	ReloadIDVector();
	RefreshUI();

	return FALSE;
}

BOOL AchievementFrame::IsWindowVisible()
{
	BOOL bShow = m_pMainWnd->IsInvisible();
	return !bShow;
}

VOID AchievementFrame::ClearInfo(tagAchievement &strAch)
{
	strAch.pStcName->SetText(_T(""));
	strAch.pStcOpenCondition->SetText(_T(""));
	strAch.pStcDesc->SetText(_T(""));
	strAch.pStcEContent->SetText(_T(""));
	strAch.pStcExp->SetText(_T(""));
	strAch.pStcGold->SetText(_T(""));
	strAch.pStcSilver->SetText(_T(""));
	strAch.pStcFinish->SetInvisible(TRUE);
	strAch.pStcAlreadyTake->SetInvisible(TRUE);
	strAch.pIbRewardOne->RefreshItem();
	strAch.pIbRewardTwo->RefreshItem();
	strAch.pIbRewardOne->SetInvisible(TRUE);
	strAch.pIbRewardTwo->SetInvisible(TRUE);
	strAch.pStcItem1Back->SetInvisible(TRUE);
	strAch.pStcItem2Back->SetInvisible(TRUE);
	strAch.pPthBackUp->SetInvisible(TRUE);
	strAch.pPthBackDown->SetInvisible(TRUE);

}

VOID AchievementFrame::ReloadTrees()
{
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (!P_VALID(plp))
		return;

	m_pTree->RemoveItem( m_pUtil->Crc32( _T("\\Root") ) );

	m_pTree->InsterItem(NULL, _T("Root"), _T("Root"));
	m_pTree->InsterItem(_T("\\Root"), _T("Chengjiu_Level"), g_StrTable[_T("Chengjiu_Level")],0,dwOrgenClour);
	//m_pTree->InsterItem(_T("\\Root"), _T("Chengjiu_Spec"), g_StrTable[_T("Chengjiu_Spec")],0,dwOrgenClour);
	tstring strLevel(_T("\\Root\\Chengjiu_Level"));
	tstring strSpec(_T("\\Root\\Chengjiu_Spec"));

	int LocalPlayerLevel = plp->GetRoleLevel();

	TrainProtoData::TrainProtoMap::const_iterator iter = TrainMgr::Inst()->GetData().GetData().begin();
	for (;iter != TrainMgr::Inst()->GetData().GetData().end();++iter)
	{
		DWORD dwID = 0;
		if (iter->second.nLevelOneType == 1)
		{
			if ( iter->second.nLevelTwoType <= LocalPlayerLevel )
				dwID = m_pTree->InsterItem(strLevel.c_str(), iter->second.DescLevelTwo.c_str(),iter->second.DescLevelTwo.c_str());
			else
				dwID = m_pTree->InsterItem(strLevel.c_str(), iter->second.DescLevelTwo.c_str(),iter->second.DescLevelTwo.c_str(),0,GrayColour);

		}
		else if (iter->second.nLevelOneType == 2)
		{
// 			if ( iter->second.nLevelTwoType <= LocalPlayerLevel )
// 				dwID = m_pTree->InsterItem(strSpec.c_str(), iter->second.DescLevelTwo.c_str(),iter->second.DescLevelTwo.c_str());
// 			else
// 				dwID = m_pTree->InsterItem(strSpec.c_str(), iter->second.DescLevelTwo.c_str(),iter->second.DescLevelTwo.c_str(),0,GrayColour);
		}
		m_mapTrainTreeMap.insert(make_pair( dwID,iter->second.nLevelTwoType));
	}	

	m_dwCurrTrainID = 10;

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
	m_pTree->SetCurrentSelItem(tempID);
	m_nCurPage = 0;
}

bool AchievementFrame::IfCanShow()
{
	// test
	return true;
	TrainProtoData::TrainProtoMap::const_iterator iter = TrainMgr::Inst()->GetData().GetData().begin();
	for (;iter != TrainMgr::Inst()->GetData().GetData().end();++iter)
	{
		if (iter->second.nLevelTwoType == m_dwCurrTrainID)
		{
			if(iter->second.eType != ETS_Close)
				return true;
		}
	}
	return false;
}