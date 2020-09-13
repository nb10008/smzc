#include "StdAfx.h"
#include "ItemMgr.h"
#include "RoleMgr.h"
#include "ListBoxEx.h"
#include "Container.h"
#include "IconStatic.h"
#include "LocalPlayer.h"
#include "ToolTipFrame.h"
#include "CreatureData.h"
#include "CombatSysUtil.h"
#include "ItemProtoData.h"
#include "ToolTipCreator.h"
#include "QuestPanelFrame.h"
#include "..\WorldDefine\quest_board.h"
#include "..\WorldDefine\msg_quest_board.h"
#include "..\WorldDefine\currency_define.h"

QuestPanelFrame::QuestPanelFrame(void):
m_Trunk(this),
m_pWnd(NULL),
m_pCaption(NULL),
m_pLBQuestPanel(NULL),
m_pStcExQuestDeatail(NULL),
m_pStcExpAwarad(NULL),
m_pStcGoldAward(NULL),
m_pStcSilverAward(NULL),
m_pBtnAcceptQuest(NULL),
m_pBtnDeliverQuest(NULL),
m_pBtnRefreshQuest(NULL),
m_pBtnExit(NULL),
m_dwEscapeTime(0),
m_nSelRow(GT_INVALID),
m_dwNpcID(0),
m_dwNpcTypeID(0)
{
	for( int i = 0; i < QUEST_REW_ITEM; i++ )
		m_pPicItemAward[i] = NULL;
}

QuestPanelFrame::~QuestPanelFrame()
{

}

BOOL QuestPanelFrame::EscCancel()
{
	m_pMgr->AddToDestroyList(this);
	return TRUE;
}


BOOL QuestPanelFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{
	if( !GameFrame::Init( pMgr, pFather, dwParam) )
		return FALSE;
	return TRUE;
}

BOOL QuestPanelFrame::Destroy()
{
	GameFrame::Destroy();
	if(P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);
	return TRUE;
}

BOOL QuestPanelFrame::ReloadUI()
{
	XmlElement e;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\taskboard.xml");
	if( !m_pGUI->LoadXml( &e, "VFS_System", strPath.c_str() ) )
		return FALSE;

	for( int i = 0; i < QUEST_REW_ITEM; i++ )
	{
		CHAR szTemp[X_LONG_NAME];
		_snprintf( szTemp, X_LONG_NAME, "taskboard\\taskboard_win\\itempic%d\\item%d", i, i );
		m_pGUI->ChangeXml( &e, szTemp, "ClassName", "IconStatic" );
	}
	m_pGUI->ChangeXml( &e, "taskboard\\taskboard_win\\tasklistpic\\tasklist", "ClassName", "ListBoxEx" );
	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &e );
	if(!P_VALID(m_pWnd))
		return FALSE;
	m_pCaption = (GUIPatch*)m_pWnd->GetChild(_T("taskboard_win"));
	m_pStcExQuestDeatail = (GUIStaticEx*)m_pCaption->GetChild(_T("conwinpic\\contentwnd\\content"));
	m_pLBQuestPanel = (ListBoxEx*)m_pCaption->GetChild(_T("tasklistpic\\tasklist"));
	m_pLBQuestPanel->SetColNum( 3, 100 );
	m_pLBQuestPanel->SetColWidth( 0, 50 );	// 任务等级
	m_pLBQuestPanel->SetColWidth( 1, 214 );	// 任务名称
	m_pLBQuestPanel->SetColWidth( 2, 50 );	// 任务状态

	tagPoint ptSizeOldListBox = m_pLBQuestPanel->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = 0;
	m_pLBQuestPanel->SetResizable( TRUE );
	m_pLBQuestPanel->Resize( ptSizeListBox );
	m_pLBQuestPanel->SetResizable( FALSE );
	m_pLBQuestPanel->SetRefresh(TRUE);

	m_pStcExpAwarad = (GUIStatic*)m_pCaption->GetChild(_T("expnum"));
	m_pStcGoldAward = (GUIStatic*)m_pCaption->GetChild(_T("goldnum"));
	m_pStcSilverAward = (GUIStatic*)m_pCaption->GetChild(_T("silvernum"));
	for( int i = 0; i < QUEST_REW_ITEM; i++ )
	{
		TCHAR szTemp[X_LONG_NAME];
		_sntprintf( szTemp, X_LONG_NAME, _T("itempic%d\\item%d"), i, i );
		m_pPicItemAward[i] = (IconStatic*)m_pCaption->GetChild(szTemp);
		m_pPicItemAward[i]->SetInvisible(TRUE);
	}

	m_pBtnRefreshQuest = (GUIButton*)m_pCaption->GetChild(_T("refreshbutton"));
	m_pBtnAcceptQuest = (GUIButton*)m_pCaption->GetChild(_T("acceptbutton"));
	m_pBtnDeliverQuest = (GUIButton*)m_pCaption->GetChild(_T("finishbutton"));
	m_pBtnExit = (GUIButton*)m_pCaption->GetChild(_T("closebutton"));
	// 注册事件处理函数
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&QuestPanelFrame::EventHandler) );
	return TRUE;
}


//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL QuestPanelFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBtnExit )
			m_pMgr->AddToDestroyList(this);
		else if( pWnd == m_pBtnRefreshQuest )
		{
			ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
			INT nNum = pContainer->GetItemQuantity(REFRESH_QUEST_PANEL_FIRST_ID);
			DWORD dwID = 0;
			if( nNum <= 0 )
			{
				nNum = pContainer->GetItemQuantity(REFRESH_QUEST_PANEL_ITEM_ID1);
				dwID = REFRESH_QUEST_PANEL_ITEM_ID1;
			}
			else
				dwID = REFRESH_QUEST_PANEL_FIRST_ID;
			
			if( nNum <= 0 )
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NoRefreshQuestPannelItem")]);
				break;
			}
			else
			{
				tagNC_RenewQuestBoard m;
				m.n64ItemSerial = pContainer->GetItemSerialID(dwID);
				m.dwNPCID = m_dwNpcID;
				m.dwNPCTypeID = m_dwNpcTypeID;
				TObjRef<NetSession>()->Send(&m);
			}
		}
		else if( pWnd == m_pBtnAcceptQuest )
		{
			if( m_nSelRow >= 0 || m_nSelRow < QUEST_BOARD_MAX_CAPACITY )
			{
				UINT16 questID = m_nSelRow + GetQuestStartID();
				QuestMgr::Inst()->SendAcceptQuest(questID);
				//m_pBtnAcceptQuest->SetEnable(FALSE);
				//m_pLBQuestPanel->SetText( m_nSelRow, 2, g_StrTable[_T("QuestPanelQuestState_Accept")], 0xFF80FF00 );	// 任务状态
			}
			else
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestPanel_SelQuest")]);
		}
		else if( pWnd == m_pBtnDeliverQuest )
		{
			if( m_nSelRow >= 0 || m_nSelRow < QUEST_BOARD_MAX_CAPACITY )
			{
				UINT16 questID = m_nSelRow + GetQuestStartID();
				QuestMgr::Inst()->SendCompleteQuest( questID, 0, EQCT_Normal );
				m_pBtnDeliverQuest->SetEnable(FALSE);
			}
			else
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestPanel_SelQuest")]);
		}
		break;
	case EGUIE_ItemClick:
		if( pWnd == m_pLBQuestPanel )
		{
			m_nSelRow = pEvent->dwParam1;
			UINT16 questID = m_nSelRow + GetQuestStartID();
			QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
			if( !P_VALID(pQuery) )
				break;
			const QuestQuery::QuestsMap& quests = pQuery->GetCurrentQuests();
			QuestQuery::QuestsMap::const_iterator finder = quests.find(questID);
			if( finder != quests.end() )
			{
				if( pQuery->QuestCanComplete(questID) )
				{
					m_pBtnDeliverQuest->SetEnable(true);
					m_pBtnAcceptQuest->SetEnable(false);
				}
				else
				{
					m_pBtnDeliverQuest->SetEnable(false);
					m_pBtnAcceptQuest->SetEnable(false);
				}
			}
			else
			{
				m_pBtnDeliverQuest->SetEnable(false);
				m_pBtnAcceptQuest->SetEnable(true);
			}
			BYTE byType = 0;
			switch(m_eNPCType)
			{
			case EFNPCT_QuestPanelReward:
				byType = EQBT_Reward;
				break;
			case EFNPCT_QuestPanelYellowList:
				byType = EQBT_YellowList;
				break;
			}
			const vector<UINT16> &vecQuest = QuestMgr::Inst()->GetQuestPanelTask(byType);
			if( m_nSelRow >=0 && m_nSelRow < (UINT16)vecQuest.size() )
			{
				UINT16 questID = vecQuest[m_nSelRow];
				UpdateQuestsDetail(questID);
				UpdateQuestReward(questID);
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{	
			for( int i=0; i<QUEST_REW_ITEM; i++ )
			{
				if( pWnd == (GUIWnd*)m_pPicItemAward[i] )
				{
					if( m_dwRewardItemID[i] != 0 && m_dwRewardItemID[i] != GT_INVALID )
					{
						BYTE byType = 0;
						switch(m_eNPCType)
						{
						case EFNPCT_QuestPanelReward:
							byType = EQBT_Reward;
							break;
						case EFNPCT_QuestPanelYellowList:
							byType = EQBT_YellowList;
							break;
						}
						const vector<UINT16> &vecQuest = QuestMgr::Inst()->GetQuestPanelTask(byType);
						if( m_nSelRow >=0 && m_nSelRow < (UINT16)vecQuest.size() )
						{
							UINT16 questID = vecQuest[m_nSelRow];
							QuestData *pData = QuestMgr::Inst()->GetData();
							if( !P_VALID(pData) )
								break;
							const tagBoardQuestProto *pProto = pData->GetQuestPanelProto(questID);
							if(P_VALID(pProto))
							{
								BYTE quality = pProto->RewardItem[i].byQuality;
								ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips( m_dwRewardItemID[i],(BYTE)quality ).c_str() );
								break;
							}
						}
					}
				}
			}
		}

		
	}
	return TRUE;
}

VOID QuestPanelFrame::Update()
{
	m_dwEscapeTime += Kernel::Inst()->GetDeltaTimeDW();
	//每隔1秒检测距离目标NPC的距离
	if( m_dwEscapeTime > 1000 )
	{
		m_dwEscapeTime = 0;
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{
			Destroy();
		}
		else if (m_dwNpcID > 0 && RoleMgr::Inst()->IsOutValidDist( m_dwNpcID ) )
		{
			m_pMgr->AddToDestroyList(this);
		}
	}
}

VOID QuestPanelFrame::UpdateQuestList()
{
	m_pLBQuestPanel->Clear();
	QuestData *pData = QuestMgr::Inst()->GetData();
	QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
	if( !P_VALID(pData) || !P_VALID(pQuery) )
		return;
	BYTE byType = 0;
	switch(m_eNPCType)
	{
	case EFNPCT_QuestPanelReward:
		byType = EQBT_Reward;
		break;
	case EFNPCT_QuestPanelYellowList:
		byType = EQBT_YellowList;
		break;
	}
	const vector<UINT16> &vecQuest = QuestMgr::Inst()->GetQuestPanelTask(byType);
	if( vecQuest.empty() )
		return;
	INT nTotal = 0;
	for( size_t i = 0; i < vecQuest.size(); i++ )
	{
		const tagBoardQuestProto *pProto = pData->GetQuestPanelProto(vecQuest[i]);
		DWORD dwColor = GT_INVALID;
		TCHAR szTmp[X_LONG_NAME] = {0};
		if(P_VALID(pProto))
		{
			switch(pProto->eQuality)
			{
			case EBQQ_Yellow:
				dwColor = 0xFFFFFF00;
				break;
			case EBQQ_Green:
				dwColor = 0xFF00FF00;
				break;
			case EBQQ_Blue:
				dwColor = 0xFF39A0FD;
				break;
			case EBQQ_Orange:
				dwColor = 0xFFFF8000;
				break;
			case EBQQ_Purple:
				dwColor = 0xFF9932CD;
				break;
			default:
				dwColor = 0xFFFFFFFF;
				break;
			}
		}
		const tagBoardQuestProto* pQuest = pData->GetQuestPanelProto( vecQuest[i] );
		if(!P_VALID(pQuest))
			continue;
		INT QuestLvl = (INT)pQuest->byLevel;
		_sntprintf( szTmp, X_LONG_NAME, g_StrTable[_T("Role_Level")], QuestLvl );
		m_pLBQuestPanel->SetText( i, 0, szTmp, dwColor );						// 任务等级

		tstring QuestName;
		pData->GetQuestPanelTaskName( vecQuest[i], QuestName );
		m_pLBQuestPanel->SetText( i, 1, QuestName.c_str(), dwColor );			// 任务名字

		// 任务完成状况
		const QuestQuery::QuestsMap& quests = pQuery->GetCurrentQuests();
		UINT16 questID = i + GetQuestStartID();
		QuestQuery::QuestsMap::const_iterator finder = quests.find(questID);
		if( finder != quests.end() )
		{
			if( pQuery->QuestCanComplete(questID) )
				m_pLBQuestPanel->SetText( i, 2, g_StrTable[_T("QuestPanelQuestState_Deliver")], 0xFFFFFF00 );	// 任务状态
			else
				m_pLBQuestPanel->SetText( i, 2, g_StrTable[_T("QuestPanelQuestState_Accept")], 0xFF80FF00 );	// 任务状态
			m_pBtnAcceptQuest->SetEnable(FALSE);
		}
		else
		{
			m_pLBQuestPanel->SetText( i, 2, g_StrTable[_T("QuestPanelQuestState_None")], 0xFFFF0000 );	// 任务状态
			m_pBtnAcceptQuest->SetEnable(FALSE);
		}
		nTotal++;
	}
	tagPoint ptSizeOldListBox = m_pLBQuestPanel->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pLBQuestPanel->GetRowHeight() * nTotal;
	m_pLBQuestPanel->SetResizable( TRUE );
	m_pLBQuestPanel->Resize( ptSizeListBox );
	m_pLBQuestPanel->SetResizable( FALSE );
	m_pLBQuestPanel->SetRefresh(TRUE);
	tagGUIEvent evt(m_pLBQuestPanel, EGUIE_ItemClick);
	if( m_nSelRow != GT_INVALID && m_nSelRow < (INT)vecQuest.size() )
		evt.dwParam1 = m_nSelRow;
	else
		evt.dwParam1 = 0;
	m_pLBQuestPanel->SendEvent(&evt);
	m_pWnd->SetRefresh(TRUE);
}

VOID QuestPanelFrame::UpdateQuestsDetail( UINT id )
{
	QuestData *pData = QuestMgr::Inst()->GetData();
	QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
	if( !P_VALID(pData) || !P_VALID(pQuery) )
		return;
	tstringstream stream;
	tstring questDesc;
	pData->GetQuestPanelTaskDesc( id, questDesc );
	QuestMgr::Inst()->ParseQuestView( questDesc, m_idLinks, m_posLinks );
	stream << g_StrTable[_T("Quest_DescOverview")] << _T("\\n    ") << questDesc << _T("\\n");

	TCHAR szQuestTrack[X_HUGE_NAME] = {0};
	UINT16 questID = GetQuestStartID() + m_nSelRow;
	
	const tagQuestProto* pQuest = pData->GetQuestProto(questID);
	const QuestQuery::QuestsMap& quests = pQuery->GetCurrentQuests();
	QuestQuery::QuestsMap::const_iterator finder = quests.find(questID);
	if( finder != quests.end() )
	{
		const tagQuestDynamicTarget *pDynamicTarget = pData->GetQuestDynamicTarget(questID);
		if( P_VALID(pDynamicTarget) ) //动态
		{
			UINT16 itemCount,creatureCount,itemSum,creatureSum;
			DWORD itemID,creatureID;

			switch( pDynamicTarget->eTargetType )
			{
			case EQTT_Kill:
				{
					// 怪物跟踪
					stream << g_StrTable[_T("Quest_DescCompleteConditions")] << _T("\\n");
					for( int i = 0; i < DYNAMIC_TARGET_COUNT; i++ )
					{
						if( pData->GetQuestRequiredCreatureCount( questID, i, creatureID, creatureSum ) )
						{
							const tagCreatureProto* creatureProto = CreatureData::Inst()->FindNpcAtt(creatureID);
							ASSERT( creatureProto != NULL );

							pQuery->GetQuestCreatureCount( questID, i, creatureCount );
							if( creatureCount > creatureSum )
							{
								creatureCount = creatureSum;
							}

							_stprintf( szQuestTrack, 
								_T("  <color=0xFFFFF7E0><link=%x,0xFFABABFF>%s<link=0xffffffff,0><color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"),
								creatureProto->dwTypeID, 
								creatureProto->szName, 
								creatureCount, 
								creatureSum );
							stream << _T("	") << g_StrTable[_T("Quest_TrackKillCreatureDesc")] << szQuestTrack;
						}
					}
				}
				break;
			case EQTT_Collect:
				{
					// 物品跟踪
					stream << g_StrTable[_T("Quest_DescCompleteConditions")] << _T("\\n");
					for( int i=0; i<DYNAMIC_TARGET_COUNT; i++ )
					{
						if( pData->GetQuestRequiredItemCount( questID, i, itemID, itemSum ) )
						{
							const tagItemDisplayInfo* itemProto = ItemProtoData::Inst()->FindItemDisplay(itemID);
							ASSERT( itemProto != NULL );

							pQuery->GetQuestItemCount( questID, i, itemCount );
							if( itemCount > itemSum )
							{
								itemCount = itemSum;
							}

							_stprintf( szQuestTrack, 
								_T("  <color=0xFFFFF7E0>%s<color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"), 
								itemProto->szName, 
								itemCount, 
								itemSum );
							stream << _T("	") << g_StrTable[_T("Quest_TrackGetItemDesc")] << szQuestTrack;
						}
					}
				}
				break;
			case EQTT_NPCTalk:
				{
					// 对话NPC跟踪
					stream << g_StrTable[_T("Quest_DescCompleteConditions")] << _T("\\n");
					for( int i=0; i<DYNAMIC_TARGET_COUNT; i++ )
					{
						if( pDynamicTarget->dwTargetID[i] == 0 )
							break;

						tstring strTalked;
						if( pData->NPCBeTalked( questID, i ) )
							strTalked = g_StrTable[_T("QuestTrack_NPCTalked")];
						else
							strTalked = g_StrTable[_T("QuestTrack_NPCNoTalk")];

						const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pDynamicTarget->dwTargetID[i] );
						ASSERT( P_VALID(creatureProto) );
						_stprintf( szQuestTrack,
							_T("  <color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0> <color=0xFFF6EB78>(%s)<color=0xFFFFF7E0>\\n"),
							creatureProto->dwTypeID,
							creatureProto->szName,
							strTalked.c_str() );
						stream << _T("	") << g_StrTable[_T("Quest_TrackNPCTalk")] << szQuestTrack;
					}
				}
				break;
			case EQTT_Invest:
				{
					// 调查地物追踪
					stream << g_StrTable[_T("Quest_DescCompleteConditions")] << _T("\\n");
					for( int i=0; i<DYNAMIC_TARGET_COUNT; i++ )
					{
						if( pDynamicTarget->dwTargetID[i] == 0 )
							break;

						tstring strInves;
						if( pQuery->ObjBeInves( questID, i ) )
							strInves = g_StrTable[_T("QuestTrack_TriggerUpdated")];
						else
							strInves = g_StrTable[_T("QuestTrack_TriggerNoUpdate")];

						const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pDynamicTarget->dwTargetID[i] );
						ASSERT( P_VALID(creatureProto) );
						_stprintf( szQuestTrack,
							_T("  <color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0> <color=0xFFF6EB78>(%s)<color=0xFFFFF7E0>\\n"),
							creatureProto->dwTypeID,
							creatureProto->szName,
							strInves.c_str() );
						stream << _T("	") << g_StrTable[_T("Quest_TrackCheckItemDesc")] << szQuestTrack;
					}
				}
				break;
			}
		}
	}
	else
	{
		const tagBoardQuestProto *pProto = pData->GetQuestPanelProto( id );
		if(P_VALID(pProto))
		{
			stream << g_StrTable[_T("Quest_DescCompleteConditions")] << _T("\\n");
			for( int i = 0; i < BOARD_QUEST_INVEST_NUM; i++ )
			{
				if( pProto->dwInvestItem[i] != GT_INVALID )
				{
					const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pProto->dwInvestItem[i] );
					if( P_VALID(creatureProto) )
					{
						_sntprintf( szQuestTrack, X_HUGE_NAME, g_StrTable[_T("Quest_Research")], creatureProto->szName );
						stream << szQuestTrack << _T("\\n");
					}
					
				}
			}
			for( int i = 0; i < BOARD_QUEST_MONSTER_NUM; i++ )
			{
				if( pProto->TargetMonster[i].byTargetMonsterNum != GT_INVALID && pProto->TargetMonster[i].byTargetMonsterNum != 0 )
				{
					if( pProto->TargetMonster[i].dwTargetMonsterID_A != GT_INVALID && pProto->TargetMonster[i].dwTargetMonsterID_A != 0 )
					{
						const tagCreatureProto* creatureProto = CreatureData::Inst()->FindNpcAtt(pProto->TargetMonster[i].dwTargetMonsterID_A);
						if( P_VALID(creatureProto) )
						{
							_sntprintf( szQuestTrack, X_HUGE_NAME, g_StrTable[_T("Quest_KillCreature")], creatureProto->szName, 0, pProto->TargetMonster[i].byTargetMonsterNum );
							stream << szQuestTrack << _T("\\n");
						}
					}
					else if( pProto->TargetMonster[i].dwTargetMonsterID_B != GT_INVALID && pProto->TargetMonster[i].dwTargetMonsterID_B != 0 )
					{
						const tagCreatureProto* creatureProto = CreatureData::Inst()->FindNpcAtt(pProto->TargetMonster[i].dwTargetMonsterID_B);
						if( P_VALID(creatureProto) )
						{
							_sntprintf( szQuestTrack, X_HUGE_NAME, g_StrTable[_T("Quest_KillCreature")], creatureProto->szName, 0, pProto->TargetMonster[i].byTargetMonsterNum );
							stream << szQuestTrack << _T("\\n");
						}
					}
				}
			}
			for( int i = 0; i < BOARD_QUEST_NPC_NUM; i++ )
			{
				if( pProto->dwTalkToNPC[i] != GT_INVALID && pProto->dwTalkToNPC[i] != 0 )
				{
					const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pProto->dwTalkToNPC[i] );
					if( P_VALID(creatureProto) )
					{
						_sntprintf( szQuestTrack, X_HUGE_NAME, g_StrTable[_T("Quest_NPCTalk")], creatureProto->szName );
						stream << szQuestTrack << _T("\\n");
					}
				}
			}
			for( int i = 0; i < BOARD_QUEST_GATHER_NUM; i++ )
			{
				if( pProto->GatherItem[i].dwID != GT_INVALID && pProto->GatherItem[i].dwID != 0 )
				{
					const tagItemDisplayInfo* itemProto = ItemProtoData::Inst()->FindItemDisplay(pProto->GatherItem[i].dwID);
					if( P_VALID(itemProto) )
					{
						_sntprintf( szQuestTrack, X_HUGE_NAME, g_StrTable[_T("Quest_Item")], itemProto->szName, 0, pProto->GatherItem[i].byNum );
						stream << szQuestTrack << _T("\\n");
					}
				}
			}
		}
	}
	m_pStcExQuestDeatail->SetText(_T(""));
	m_pStcExQuestDeatail->SetText(stream.str().c_str());
	tagPoint size = m_pStcExQuestDeatail->GetSize();
	if( m_pStcExQuestDeatail->GetTextTotalHeight() > size.y )
	{
		size.y = m_pStcExQuestDeatail->GetTextTotalHeight();
		m_pStcExQuestDeatail->SetResizable(TRUE);
		m_pStcExQuestDeatail->Resize( size );
		m_pStcExQuestDeatail->SetResizable(FALSE);
	}
}

VOID QuestPanelFrame::UpdateQuestReward( UINT16 id )
{
	QuestData *pData = QuestMgr::Inst()->GetData();
	if( !P_VALID(pData) )
		return;
	const tagBoardQuestProto *pProto = pData->GetQuestPanelProto( id );
	if(P_VALID(pProto))
	{
		// 经验金钱奖励
		m_pStcExpAwarad->SetText(_T(""));
		m_pStcGoldAward->SetText(_T(""));
		m_pStcSilverAward->SetText(_T(""));
		TCHAR szTemp[X_LONG_NAME] = {0};
		_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pProto->nRewardExp );
		m_pStcExpAwarad->SetText(szTemp);
		_sntprintf( szTemp, X_LONG_NAME, _T("%d"), MSilver2DBGold(pProto->nRewardMoney) );
		m_pStcGoldAward->SetText(szTemp);
		_sntprintf( szTemp, X_LONG_NAME, _T("%d"), MSilver2DBSilver(pProto->nRewardMoney) );
		m_pStcSilverAward->SetText(szTemp);
		// 物品奖励
		for( int i=0; i < BOARD_QUEST_REWARD_NUM; i++ )
		{
			if( pProto->RewardItem[i].dwID != GT_INVALID )
			{
				tstring strPic = ItemMgr::Inst()->GetItemIconPath( pProto->RewardItem[i].dwID );
				m_pPicItemAward[i]->SetPic( strPic );
				TCHAR szNum[5];
				_stprintf(szNum, _T("%4d"), pProto->RewardItem[i].byNum);
				m_pPicItemAward[i]->SetText( szNum );
				m_dwRewardItemID[i] = pProto->RewardItem[i].dwID;
				m_pPicItemAward[i]->SetInvisible(FALSE);
			}
			else
			{
				m_dwRewardItemID[i] = 0;
				m_pPicItemAward[i]->SetInvisible(TRUE);
			}
		}
	}
}

UINT16 QuestPanelFrame::GetQuestStartID()
{
	switch(m_eNPCType)
	{
	case EFNPCT_QuestPanelReward:
		return QUEST_PANEL_START_TASK_ID;
	case EFNPCT_QuestPanelYellowList:
		return YELLOW_LIST_START_TASK_ID;
	}
	return QUEST_PANEL_START_TASK_ID;
}