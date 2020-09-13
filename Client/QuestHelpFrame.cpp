#include "StdAfx.h"
#include "QuestHelpFrame.h"
#include "IconStatic.h"
#include "GameSet.h"
#include "QuestMgr.h"
#include "CreatureData.h"
#include "ItemProtoData.h"
#include "CombatSysUtil.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "CombatEvent.h"

QuestHelpFrame::QuestHelpFrame():m_Trunk(this),
m_pWnd(NULL),
m_pWndCaption(NULL),
m_pButClose(NULL),
m_pButExit(NULL),
m_pButHideFrame(NULL),
m_pButNext(NULL),
m_pButPre(NULL),
m_pButShowFrame(NULL),
m_pShowFrame(NULL),
m_pstcPage(NULL),
m_pStcQuestHelpPic(NULL),
m_dwCurPage(GT_INVALID),
m_dwTotalPicNum(GT_INVALID)
{

}

QuestHelpFrame::~QuestHelpFrame()
{

}

BOOL QuestHelpFrame::Destroy()
{
	GameFrame::Destroy();
	if( P_VALID(m_pWnd) )
		m_pGUI->DestroyWnd(m_pWnd);
	if(P_VALID(GetObj("QuestHelpInfo")))
		KillObj("QuestHelpInfo");
	return true;
}

VOID QuestHelpFrame::Update()
{
	float deltaTime =  Cool3D::Kernel::Inst()->GetDeltaTime();
	//if( !m_pWnd->IsInvisible() )
	//	InitNewGuysQuest();
	GameFrame::Update();
}

BOOL QuestHelpFrame::EscCancel()
{
	if(m_pWnd->IsInvisible())
		return FALSE;

	QuestMgr::Inst()->CloseQuestHelpFrame();
	return TRUE;
}

BOOL QuestHelpFrame::ReloadUI()
{
	tstring strPath = g_strLocalPath + _T("\\ui\\guide.xml");
	XmlElement element;
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&element, "guide_win\\guide_pic\\pic\\img", "ClassName", "IconStatic");
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);

	m_pWndCaption = (GUIWnd*)m_pWnd->GetChild(_T("guide_pic"));
	m_pButClose = (GUIButton*)m_pWndCaption->GetChild(_T("close"));
	m_pButExit = (GUIButton*)m_pWndCaption->GetChild(_T("closebutt"));
	m_pButNext = (GUIButton*)m_pWndCaption->GetChild(_T("rightbutt"));
	m_pButPre = (GUIButton*)m_pWndCaption->GetChild(_T("leftbutt"));
	m_pstcPage = (GUIStatic*)m_pWndCaption->GetChild(_T("word2\\num"));
	m_pShowFrame = (GUIFrameMutex*)m_pWndCaption->GetChild(_T("word\\frm"));
	m_pButShowFrame = (GUIButton*)m_pShowFrame->GetChild(_T("yesbutt"));
	m_pButHideFrame = (GUIButton*)m_pShowFrame->GetChild(_T("nobutt"));
	m_pStcQuestHelpPic = (IconStatic*)m_pWndCaption->GetChild(_T("pic\\img"));
	//m_pListQuest = (GUIListBox*)m_pWndCaption->GetChild(_T("pic0\\list1"));
	//m_pStcQuestTips = (GUIStaticEx*)m_pWndCaption->GetChild(_T("pic1\\win\\text"));
	//m_pListQuest->SetColWidth( 0, 60 );
	//m_pListQuest->SetColWidth( 1, 30 );
	//m_pListQuest->SetColWidth( 2, 131 );

	if( P_VALID(m_pShowFrame) )
	{
		m_bShowFrame = GameSet::Inst()->Get( EGS_QuestHelp ) ? true : false;
		if( m_bShowFrame )
			m_pShowFrame->SetPush(m_pButShowFrame);
		else
			m_pShowFrame->SetPush(m_pButHideFrame);
	}
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&QuestHelpFrame::EventHandler) );
	return true;
}

BOOL QuestHelpFrame::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if( !GameFrame::Init( pMgr, pFather, dwParam ) )
		return false;

	if(P_VALID(GetObj("QuestHelpInfo")))
		KillObj("QuestHelpInfo");
	m_pVarContainer = CreateObj("QuestHelpInfo", "VarContainer");
	LoadQuestHelpInfoFile();
	
	if( m_vecQuestHelpInfo.size() > 0 )
	{
		m_dwCurPage = m_vecQuestHelpInfo[0].dwStartPicPage;
		SetPicName(m_dwCurPage);
	}

	return true;
}

void QuestHelpFrame::LoadQuestHelpInfoFile()
{
	const TCHAR* szQuestHelpFile = _T("data\\system\\quest\\QuestHelpFrame_Info.xml");
	list<tstring> QuestList;
	m_pVarContainer->Load("VFS_System", szQuestHelpFile, "QuestId", &QuestList);
	list<tstring>::iterator iter = QuestList.begin();

	for(; iter!=QuestList.end(); iter++)
	{
		tagQuestHelpInfo QuestInfo;
		QuestInfo.dwQuestId = m_pVarContainer->GetDword( _T("QuestId"), (*iter).c_str(), GT_INVALID );
		_tcscpy( QuestInfo.szStartPicName, m_pVarContainer->GetString(_T("StartPicName"), (*iter).c_str(), _T("")) );
		_tcscpy( QuestInfo.szFinishPicName, m_pVarContainer->GetString(_T("FinishPicName"), (*iter).c_str(), _T("")) );
		QuestInfo.dwStartPicPage = m_pVarContainer->GetDword( _T("StartPicPage"), (*iter).c_str(), GT_INVALID );
		QuestInfo.dwFinishPicPage = m_pVarContainer->GetDword( _T("FinishPicPage"), (*iter).c_str(), GT_INVALID );
		m_vecQuestHelpInfo.push_back( QuestInfo );
	}
	if( !m_vecQuestHelpInfo.empty() )
	{
		size_t nNum = m_vecQuestHelpInfo.size() - 1;
		if( m_vecQuestHelpInfo[nNum].dwFinishPicPage == GT_INVALID )
			m_dwTotalPicNum = m_vecQuestHelpInfo[nNum].dwStartPicPage;
		else if( m_vecQuestHelpInfo[nNum].dwStartPicPage == GT_INVALID )
			m_dwTotalPicNum = m_vecQuestHelpInfo[nNum].dwFinishPicPage;
		else
		{
			m_dwTotalPicNum = ( m_vecQuestHelpInfo[nNum].dwFinishPicPage > m_vecQuestHelpInfo[nNum].dwStartPicPage ) ? 
				m_vecQuestHelpInfo[nNum].dwFinishPicPage : m_vecQuestHelpInfo[nNum].dwStartPicPage;
		}
	}
}

DWORD QuestHelpFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pButExit || pWnd == m_pButClose )	
			{
				QuestMgr::Inst()->CloseQuestHelpFrame();
			}
			else if( pWnd == m_pButPre )
			{
				if(m_dwCurPage == 1 )
					m_dwCurPage = m_dwTotalPicNum;
				else
					m_dwCurPage--;
				SetPicName(m_dwCurPage);
				ShowFrame();
				
			}
			else if( pWnd == m_pButNext )
			{
				
				if(m_dwCurPage == m_dwTotalPicNum )
					m_dwCurPage = 1;
				else
					m_dwCurPage++;
				SetPicName(m_dwCurPage);
				ShowFrame();
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if( pWnd == m_pButShowFrame )
			{
				GameSet::Inst()->Set( EGS_QuestHelp, true );
				GameSet::Inst()->SaveQuestHelpSet();
				m_bShowFrame = true;
			}
			else if( pWnd == m_pButHideFrame )
			{
				GameSet::Inst()->Set( EGS_QuestHelp, false );
				GameSet::Inst()->SaveQuestHelpSet();
				m_bShowFrame = false;
			}
		}
	case EGUIE_ItemClick:
		{
			//if( pWnd == m_pListQuest )		// 任务具体信息
			//{
			//	const vector<DWORD> vecQuest = QuestMgr::Inst()->GetData()->GetNewGuysQuest();
			//	if( pEvent->dwParam1 < vecQuest.size() )
			//	{
			//		ShowQuestDetail(vecQuest[pEvent->dwParam1] );
			//	}
			//}
		}
		break;
	case EGUIE_StaticExLinkClick:
		{
			Vector3 pos;
			QuestData *pData = QuestMgr::Inst()->GetData();
			ASSERT( P_VALID(pData) );
			QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
			ASSERT( P_VALID(pQuery) );

			if( !pQuery->GetNPCPosition( pEvent->dwParam1, pos ) )//不在本地图，进行跨场景寻路
			{
				tstring mapName;
				if( !pData->GetPathPointMap( pEvent->dwParam1, mapName ) )//获得地图名称
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("quest_cannot_find_npc")]);
					break;
				}

				QuestMgr::Inst()->MoveToMap(mapName.c_str(),pos);
			}

			LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();

			tagMouseMoveEvent event( _T("tagMouseMoveEvent"), NULL );
			event.start = pLP->GetPos();
			event.end = pos;
			event.validDist = 100.0f;
			event.dwNpcTypeID = pEvent->dwParam1;

			m_pMgr->SendEvent( &event );
		}
		break;
	}

	return FALSE;
}

VOID QuestHelpFrame::ShowFrame()
{
	if( P_VALID(m_pWnd) )
	{
		m_pWnd->SetInvisible(FALSE);
		TCHAR szPage[X_SHORT_NAME] = {0};
		_sntprintf(szPage, sizeof(szPage)/sizeof(TCHAR), _T("%d"), m_dwCurPage );
		m_pstcPage->SetText(szPage);
		m_pstcPage->FlipToTop();
		m_pStcQuestHelpPic->SetPic(m_strPicName);
		//InitNewGuysQuest();
	}
}

VOID QuestHelpFrame::HideFrame()
{
	if( P_VALID(m_pWnd) )
	{
		m_pWnd->SetInvisible(TRUE);
	}
}

VOID QuestHelpFrame::SetPicName( const DWORD dwPage )
{
	for( size_t i = 0; i < m_vecQuestHelpInfo.size(); i++ )
	{
		if( dwPage == m_vecQuestHelpInfo[i].dwStartPicPage )
		{
			m_strPicName = m_vecQuestHelpInfo[i].szStartPicName;
			break;
		}
		if( dwPage == m_vecQuestHelpInfo[i].dwFinishPicPage )
		{
			m_strPicName = m_vecQuestHelpInfo[i].szFinishPicName;
			break;
		}
	}
}

//void QuestHelpFrame::InitNewGuysQuest()
//{
//	int nRow = 0;
//	int nCol = 0;
//	vector<DWORD> vecQuest = QuestMgr::Inst()->GetData()->GetNewGuysQuest();
//	for( size_t i = 0; i < vecQuest.size(); i++ )
//	{
//		// 取得新手任务的状态 0：暂未接取； 1：进行中； 2：已完成
//		switch( QuestMgr::Inst()->GetQuestState( vecQuest[i]) )
//		{
//		case 0:
//			m_pListQuest->SetText( nRow, nCol++, g_StrTable[_T("Quest_No")], 0xFFFFF7E0 );
//			break;
//		case 1:
//			m_pListQuest->SetText( nRow, nCol++, g_StrTable[_T("Quest_Doing")], 0xFF00FF00 );
//			break;
//		case 2:
//			m_pListQuest->SetText( nRow, nCol++, g_StrTable[_T("Quest_Done")], 0xFFfff7e0 );
//			break;
//		}
//		INT16 nType = QuestMgr::Inst()->GetData()->GetQuestCategoryByQuestID((INT16)vecQuest[i]);
//		m_pListQuest->SetText( nRow, nCol++, QuestMgr::Inst()->GetData()->GetTaskTypeName(nType).c_str() );
//		const tagQuestProto* pProto = QuestMgr::Inst()->GetData()->GetQuestProto((UINT16)vecQuest[i]);
//		if( P_VALID(pProto) )
//		{
//			m_pListQuest->SetText( nRow, nCol++, pProto->name );
//		}
//		nRow++;
//		nCol = 0;
//	}
//	//m_pListQuest->SetCurSelectedRow(GT_INVALID);
//}
//
//void QuestHelpFrame::ShowQuestDetail( DWORD questid )
//{
//	m_pStcQuestTips->SetText(_T(""));
//	m_pStcQuestTips->SetRefresh(true);
//	tstringstream questInfo;
//	const tagQuestProto* pProto = QuestMgr::Inst()->GetData()->GetQuestProto( (UINT16)questid );
//	if( P_VALID(pProto) )
//	{
//		// 任务名称
//		questInfo << pProto->name << _T("\\n");
//		TCHAR szTips[X_HUGE_NAME] = {0};
//		bool bHave = false;
//		TCHAR szNames[X_LONG_NAME] = {0};
//		TCHAR szComNPC[X_LONG_NAME] = {0};
//		TCHAR szAccCon[X_LONG_NAME] = {0};
//		// 任务接取人
//		for( int i = 0; i < QUEST_ACCEPT_NPC; i++ )
//		{
//			if( pProto->accept_quest_npc[i] == 0 )
//				continue;
//			const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pProto->accept_quest_npc[i] );
//			ASSERT( creatureProto != NULL );
//			if( _tcslen(szNames) > 0 )
//				_stprintf( szNames, _T("%s,<color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0><color=0xFFFFF7E0>"), szNames, creatureProto->dwTypeID, creatureProto->szName );
//			else
//				_stprintf( szNames, _T("<color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0><color=0xFFFFF7E0>"), creatureProto->dwTypeID, creatureProto->szName );
//			bHave = true;
//		}
//		if( !bHave )
//			questInfo << g_StrTable[_T("Quest_AutoAdd")] << _T("\\n");
//		else
//		{
//			_sntprintf( szTips, X_HUGE_NAME, g_StrTable[_T("AcceptQuest_NPC")], szNames );
//			questInfo << szTips << _T("\\n");
//		}
//
//		// 任务交付人
//		bHave = false;
//		for( int i = 0; i < QUEST_COMPLETE_NPC; i++ )
//		{
//			if( pProto->complete_quest_npc[i] == 0 )
//				continue;
//
//			const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pProto->complete_quest_npc[i] );
//			ASSERT( creatureProto != NULL );
//			if( _tcslen(szComNPC) > 0 )
//				_stprintf( szComNPC, _T("%s,<color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0><color=0xFFFFF7E0>"), szComNPC, creatureProto->dwTypeID, creatureProto->szName );
//			else
//				_stprintf( szComNPC, _T("<color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0><color=0xFFFFF7E0>"), creatureProto->dwTypeID, creatureProto->szName );
//			bHave = true;
//		}
//		if( bHave )
//		{
//			_sntprintf( szTips, X_HUGE_NAME, g_StrTable[_T("DoneQuest_NPC")], szComNPC );
//			questInfo << szTips << _T("\\n");
//		}
//
//		// 任务接取条件
//		if( pProto->prev_quest_relation )
//		{
//			const tagQuestProto* ppProto = QuestMgr::Inst()->GetData()->GetQuestProto( pProto->prev_quest_id );
//			if( P_VALID(ppProto) )
//			{
//				_stprintf( szAccCon, _T("<color=0xFFABABFF>%s<color=0xFFFFF7E0>"), ppProto->name );
//				const tagQuestProto* ppProto = QuestMgr::Inst()->GetData()->GetQuestProto( pProto->prev_quest2_id );
//				if( P_VALID(ppProto) )
//				{
//					_stprintf( szAccCon, _T("%s, <color=0xFFABABFF>%s<color=0xFFFFF7E0>"), szAccCon, ppProto->name );
//				}
//			}
//			_sntprintf( szTips, X_HUGE_NAME, g_StrTable[_T("AcceptQuest_Con2")], szAccCon );
//			questInfo << szTips << _T("\\n");
//		}
//		else if( pProto->prev_quest_id != 0 )
//		{
//			bHave = false;
//			for( int i = 0; i < QUEST_ITEMS_COUNT; i++ )
//			{
//				if( pProto->accept_req_item[i] == 0 )
//					continue;
//				const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(pProto->accept_req_item[i]);
//				if(_tcslen(szAccCon) > 0 )
//					_stprintf( szAccCon, _T("%s, <color=0xFFABABFF>%s<color=0xFFFFF7E0>"), szAccCon, pItemProto->pDisplayInfo->szName );
//				else
//					_stprintf( szAccCon, _T("<color=0xFFABABFF>%s<color=0xFFFFF7E0>"), pItemProto->pDisplayInfo->szName );
//				bHave = true;
//			}
//			const tagQuestProto* ppProto = QuestMgr::Inst()->GetData()->GetQuestProto( pProto->prev_quest_id );
//			if( P_VALID(ppProto) )
//			{
//				if( bHave )
//				{
//					_sntprintf( szTips, X_HUGE_NAME, g_StrTable[_T("AcceptQuest_Con3")], ppProto->name, szAccCon );
//					questInfo << szTips << _T("\\n");
//				}
//				else
//				{
//					_sntprintf( szTips, X_HUGE_NAME, g_StrTable[_T("AcceptQuest_Con")], ppProto->name );
//					questInfo << szTips << _T("\\n");
//				}
//			}
//		}
//		else
//			questInfo << g_StrTable[_T("AcceptQuest_ConNone")] << _T("\\n");
//
//		// 任务完成条件
//		//if( pProto->only_one_npc )
//		//{
//		//	const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pProto->complete_req_npc[0] );
//		//	ASSERT( creatureProto != NULL );
//		//	_sntprintf(szTips, X_HUGE_NAME, g_StrTable[_T("Complete_NPC")], creatureProto->szName );
//		//	questInfo << szTips << _T("\n");
//		//}
//		//else if( pProto->only_one_creature )
//		//{
//		//	QuestData *pData = QuestMgr::Inst()->GetData();
//		//	DWORD creatureID;
//		//	UINT16 creatureSum;
//		//	for( int i=0; i<QUEST_CREATURES_COUNT; i++ )
//		//	{
//		//		if( pData->GetQuestRequiredCreatureCount( (UINT16)questid, i, creatureID, creatureSum ) )
//		//		{
//		//			const tagCreatureProto* creatureProto = CreatureData::Inst()->FindNpcAtt(creatureID);
//		//			ASSERT( creatureProto != NULL );
//		//			_sntprintf( szTips, X_HUGE_NAME, g_StrTable[_T("Kill_monster")], creatureSum, creatureProto->szName );
//		//			questInfo << szTips << _T("\\n");
//		//		}
//		//	}
//		//}
//		//else if( pProto->only_one_item )
//		//{
//		//	QuestData *pData = QuestMgr::Inst()->GetData();
//		//	DWORD itemID;
//		//	UINT16 itemSum;
//		//	if( pData->GetQuestRequiredItemCount( (UINT16)questid, 0, itemID, itemSum ) )
//		//	{
//		//		const tagItemDisplayInfo* itemProto = ItemProtoData::Inst()->FindItemDisplay(itemID);
//		//		ASSERT( itemProto != NULL );
//		//		_sntprintf( szTips, X_HUGE_NAME, g_StrTable[_T("Kill_monster")], itemSum, itemProto->szName );
//		//		questInfo << szTips << _T("\\n");
//		//	}
//		//}
//		QuestData *pData = QuestMgr::Inst()->GetData();
//		if( pData->QuestRequireItemOrCreature((UINT16)questid) )
//		{
//			questInfo << g_StrTable[_T("Kill_monster")];
//			bHave = false;
//			const tagQuestDynamicTarget *pDynamicTarget = pData->GetQuestDynamicTarget((UINT16)questid);
//			if( P_VALID(pDynamicTarget) )//动态
//			{
//				UINT16 itemSum,creatureSum;
//				DWORD itemID,creatureID;
//
//				switch( pDynamicTarget->eTargetType )
//				{
//				case EQTT_Kill:
//					{
//						// 怪物信息
//						for( int i=0; i<1; i++ )
//						{
//							if( pData->GetQuestRequiredCreatureCount( (UINT16)questid , i, creatureID, creatureSum ) )
//							{
//								const tagCreatureProto* creatureProto = CreatureData::Inst()->FindNpcAtt(creatureID);
//								ASSERT( creatureProto != NULL );
//
//								TCHAR szCreatureID[15];
//								_ui64tot(creatureID,szCreatureID,16);
//								questInfo << g_StrTable[_T("Quest_TrackKillCreatureDesc")]
//									<< creatureProto->szName << creatureSum << _T("/") << creatureSum << _T("\\n");
//								bHave = true;
//							}
//						}
//					}
//					break;
//				case EQTT_Collect:
//					{
//						// 物品信息
//						for( int i=0; i<DYNAMIC_TARGET_COUNT; i++ )
//						{
//							if( pData->GetQuestRequiredItemCount( (UINT16)questid , i, itemID, itemSum ) )
//							{
//								const tagItemDisplayInfo* itemProto = ItemProtoData::Inst()->FindItemDisplay(itemID);
//								ASSERT( itemProto != NULL );
//
//								TCHAR szItemID[15];
//								_ui64tot(itemID,szItemID,16);
//
//								questInfo << g_StrTable[_T("Quest_TrackGetItemDesc")]
//									<< itemProto->szName << itemSum << _T("/") << itemSum << _T("\\n");
//								bHave = true;
//							}
//						}
//					}
//					break;
//				}
//			}
//			else
//			{
//				UINT16 itemSum,creatureSum;
//				DWORD itemID,creatureID;
//
//				// 物品信息
//				for( int i=0; i<QUEST_ITEMS_COUNT; i++ )
//				{
//					if( pData->GetQuestRequiredItemCount( (UINT16)questid , i, itemID, itemSum ) )
//					{
//						const tagItemDisplayInfo* itemProto = ItemProtoData::Inst()->FindItemDisplay(itemID);
//						ASSERT( itemProto != NULL );
//
//						TCHAR szItemID[15];
//						_ui64tot(itemID,szItemID,16);
//						questInfo << g_StrTable[_T("Quest_TrackGetItemDesc")]
//							<< itemProto->szName << itemSum << _T("/") << itemSum << _T("\\n");
//						bHave = true;
//					}
//				}
//
//				// 怪物信息
//				for( int i=0; i<QUEST_CREATURES_COUNT; i++ )
//				{
//					if( pData->GetQuestRequiredCreatureCount( (UINT16)questid , i, creatureID, creatureSum ) )
//					{
//						const tagCreatureProto* creatureProto = CreatureData::Inst()->FindNpcAtt(creatureID);
//						ASSERT( creatureProto != NULL );
//
//						TCHAR szCreatureID[15];
//						_ui64tot(creatureID,szCreatureID,16);
//						questInfo << g_StrTable[_T("Quest_TrackKillCreatureDesc")]
//							<< creatureProto->szName << creatureSum << _T("/") << creatureSum << _T("\\n");
//						bHave = true;
//					}
//				}
//			}
//		}
//		else
//		{
//			_sntprintf( szTips, X_HUGE_NAME, g_StrTable[_T("Talk_NPC")], szComNPC );
//			questInfo << szTips << _T("\\n");
//		}
//
//		tstring strView = pProto->over_view;
//		QuestMgr::Inst()->ParseQuestView( strView, m_idLinks, m_posLinks );
//
//		questInfo << g_StrTable[_T("Quest_DescOverview")] << _T("\\n    ") << strView;
//	}
//	m_pStcQuestTips->SetText( questInfo.str().c_str() );
//
//	// 滑动条拖动设置
//	tagPoint size = m_pStcQuestTips->GetSize();
//	if( m_pStcQuestTips->GetTextTotalHeight() > size.y )
//	{
//		size.y = m_pStcQuestTips->GetTextTotalHeight();
//		m_pStcQuestTips->SetResizable(TRUE);
//		m_pStcQuestTips->Resize( size );
//		m_pStcQuestTips->SetResizable(FALSE);
//	}
//
//	m_pStcQuestTips->SetRefresh(TRUE);
//}