/********************************************************************
	created:	2008/04/30
	created:	30:4:2008   12:01
	filename: 	d:\loong_client\Client\QuestMainFrame.cpp
	file path:	d:\loong_client\Client
	file base:	QuestMainFrame
	file ext:	cpp
	author:		leili
	
	purpose:	任务主界面实现，处理任务主窗口的界面逻辑
*********************************************************************/
#include "StdAfx.h"
#include "QuestMainFrame.h"
#include "QuestNPCFrame.h"
#include "ItemMgr.h"
#include "GameClientDefine.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "ItemProtoData.h"
#include "CreatureData.h"
#include "RoleMgr.h"
#include "CombatEvent.h"
#include "LocalPlayer.h"
#include "MapMgr.h"
#include "UIEvent.h"
#include "..\WorldDefine\currency_define.h"
#include "EasyBuyFrame.h"
#include "CombatSysUtil.h"
#include "Container.h"
#include "..\WorldDefine\quest_board.h"
#include "..\WorldDefine\msg_quest_board.h"

namespace
{
	tstring& replace_all(tstring& str,const tstring& old_value,const tstring& new_value)
	{  
		while( true )
		{
			tstring::size_type pos(0);
			if( (pos=str.find(old_value)) != tstring::npos )
				str.replace(pos,old_value.length(),new_value);
			else
				break;
		}  
		return str;
	}
}

//-----------------------------------------------------------------------------
// 构造函数
//-----------------------------------------------------------------------------
QuestMainFrame::QuestMainFrame(void)
: m_Trunk(this)
, m_pWnd(0)
, m_pTreeTaskList(0)
, m_pBtnQuestNpc(0)
, m_pBtnActiveQuest(0)
, m_pBtnQuestTrack(0)
, m_pBtnDelQuest(0)
, m_pBtnCloseWnd1(0)
, m_pBtnCloseWnd2(0)
, m_pStcCondition(0)
, m_pStcBonusExp(0)
, m_pStcBonusMoney1(0)
, m_pStcBonusMoney2(0)
, m_pSrlCondition(0)
, m_showNPC(false)
, m_showTrack(true)
, m_pStcBack(NULL)
, m_pStcRewardBack(NULL)
, m_pStcMoneyReward(NULL)
, m_pStcFixReward(NULL)
, m_pStcOptionReward(NULL)
{
	memset( m_BonusGoodsID, 0, sizeof(DWORD)*QUEST_REW_ITEM );
	memset( m_BonusSelsID, 0, sizeof(DWORD)*QUEST_REW_ITEM );
}

//-----------------------------------------------------------------------------
// 析构函数
//-----------------------------------------------------------------------------
QuestMainFrame::~QuestMainFrame(void)
{
	
}

//-----------------------------------------------------------------------------
// 初始化
//-----------------------------------------------------------------------------
BOOL QuestMainFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pStcCondition->SetText( g_StrTable[_T("Quest_None")] );

	// 注册游戏事件
	pMgr->RegisterEventHandle( _T("QuestMainFrameEvent_CloseTrack"),(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMainFrame::OnEvent_CloseTrack));
	pMgr->RegisterEventHandle( _T("QuestMainFrameEvent_CloseNPC"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMainFrame::OnEvent_CloseNPC));
	pMgr->RegisterEventHandle( _T("QuestMainFrameEvent_OpenNPC"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMainFrame::OnEvent_OpenNPC));
	pMgr->RegisterEventHandle( _T("QuestMainFrameEvent_CancelQuest"),(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMainFrame::OnEvent_CancelQuest));

	return TRUE;
}

//-----------------------------------------------------------------------------
// 销毁
//-----------------------------------------------------------------------------
BOOL QuestMainFrame::Destroy()
{
	// 注销游戏事件
	m_pMgr->UnRegisterEventHandler( _T("QuestMainFrameEvent_CloseTrack"),(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMainFrame::OnEvent_CloseTrack));
	m_pMgr->UnRegisterEventHandler( _T("QuestMainFrameEvent_CloseNPC"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMainFrame::OnEvent_CloseNPC));
	m_pMgr->UnRegisterEventHandler( _T("QuestMainFrameEvent_OpenNPC"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMainFrame::OnEvent_OpenNPC));
	m_pMgr->UnRegisterEventHandler( _T("QuestMainFrameEvent_CancelQuest"),(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMainFrame::OnEvent_CancelQuest));
	if( P_VALID(GetObj("QuestMain_CancelQuest")) )
		KillObj("QuestMain_CancelQuest");

	if( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	return GameFrame::Destroy();
}

//-----------------------------------------------------------------------------
// 重新读入窗口界面
//-----------------------------------------------------------------------------
BOOL QuestMainFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\tasklist.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &element, "task\\taskwin\\tasklistpic\\tasklist", "ClassName", "AppliedTree");
	for( int i = 0; i < QUEST_REW_ITEM; i++ )
	{
		CHAR szTemp[X_LONG_NAME] = {0};
		sprintf( szTemp, "task\\taskwin\\conwinpic\\contentwnd\\content\\rewards\\fixedreward\\itemback%d\\itempic", i );
		m_pGUI->ChangeXml( &element, szTemp, "ClassName", "IconStatic" );
		sprintf( szTemp, "task\\taskwin\\conwinpic\\contentwnd\\content\\rewards\\choose\\itemback%d\\itempic", i );
		m_pGUI->ChangeXml( &element, szTemp, "ClassName", "IconStatic" );
	}

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	if( !P_VALID( m_pWnd ) )
		return FALSE;

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&QuestMainFrame::EventHandler));

	m_pTreeTaskList		= (AppliedTree*)	m_pWnd->GetChild(_T("taskwin\\tasklistpic\\tasklist"));
	m_pBtnQuestNpc		= (GUIButton*)		m_pWnd->GetChild(_T("taskwin\\npclistbutt"));
	m_pBtnActiveQuest	= (GUIButton*)		m_pWnd->GetChild(_T("taskwin\\dtaskbutt"));
	m_pBtnQuestTrack	= (GUIButton*)		m_pWnd->GetChild(_T("taskwin\\trackbutt"));
	m_pBtnDelQuest		= (GUIButton*)		m_pWnd->GetChild(_T("taskwin\\delbutt"));
	m_pBtnCloseWnd1		= (GUIButton*)		m_pWnd->GetChild(_T("taskwin\\close"));
	m_pBtnCloseWnd2		= (GUIButton*)		m_pWnd->GetChild(_T("taskwin\\closebutt"));
	m_pStcCondition		= (GUIStaticEx*)	m_pWnd->GetChild(_T("taskwin\\conwinpic\\contentwnd\\content\\taskcontent"));											
	m_pStcBonusExp		= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\conwinpic\\contentwnd\\content\\rewards\\rewardback\\expnum"));
	m_pStcBonusMoney1	= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\conwinpic\\contentwnd\\content\\rewards\\rewardback\\goldnum"));
	m_pStcBonusMoney2	= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\conwinpic\\contentwnd\\content\\rewards\\rewardback\\silvernum"));
	m_pSrlCondition		= (GUIScrollBar*)	m_pWnd->GetChild(_T("taskwin\\conwinpic\\contentwnd\\scr2"));
	m_pStcBack			= (GUIStaticEx*)	m_pWnd->GetChild(_T("taskwin\\conwinpic\\contentwnd\\content"));
	m_ptOriSize = m_pStcBack->GetSize();
	m_pStcRewardBack	= (GUIStaticEx*)	m_pWnd->GetChild(_T("taskwin\\conwinpic\\contentwnd\\content\\rewards"));
	m_ptOriReWSize = m_pStcRewardBack->GetSize();
	m_pStcMoneyReward	= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\conwinpic\\contentwnd\\content\\rewards\\rewardback"));
	m_pStcFixReward	= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\conwinpic\\contentwnd\\content\\rewards\\fixedreward"));
	m_pStcOptionReward	= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\conwinpic\\contentwnd\\content\\rewards\\choose"));
	m_pStcExQuestNum	= (GUIStaticEx*) m_pWnd->GetChild(_T("taskwin\\tasklistpic\\titlepic\\num"));
	m_pStcMoneyReward->SetInvisible(TRUE);
	m_pStcFixReward->SetInvisible(TRUE);
	m_pStcOptionReward->SetInvisible(TRUE);

	//GUIStatic *pStatic	= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\taskpic\\encpic\\eword5"));
	//m_pStcBonusSpecT.push_back( pStatic );
	//pStatic				= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\taskpic\\encpic\\eword6"));
	//m_pStcBonusSpecT.push_back( pStatic );
	//pStatic				= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\taskpic\\encpic\\eword2"));
	//m_pStcBonusSpecT.push_back( pStatic );
	//pStatic				= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\taskpic\\encpic\\eword4"));
	//m_pStcBonusSpecT.push_back( pStatic );
	//pStatic				= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\taskpic\\encpic\\necnum4"));
	//m_pStcBonusSpecV.push_back( pStatic );
	//pStatic				= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\taskpic\\encpic\\encnum5"));
	//m_pStcBonusSpecV.push_back( pStatic );
	//pStatic				= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\taskpic\\encpic\\encnum2"));
	//m_pStcBonusSpecV.push_back( pStatic );
	//pStatic				= (GUIStatic*)		m_pWnd->GetChild(_T("taskwin\\taskpic\\encpic\\encnum3"));
	//m_pStcBonusSpecV.push_back( pStatic );

	for( int i = 0; i < 5 ; i++ )
	{
		TCHAR szTmp[255];

		_stprintf(szTmp, _T("taskwin\\conwinpic\\contentwnd\\content\\rewards\\fixedreward\\itemback%d\\itempic"), i );
		IconStatic *pBonusGood = (IconStatic*)m_pWnd->GetChild( szTmp );
		m_stcBonusGoods.push_back( pBonusGood );

		_stprintf(szTmp, _T("taskwin\\conwinpic\\contentwnd\\content\\rewards\\fixedreward\\itemback%d"), i );
		GUIStatic *pBonusGoodBack = (GUIStatic*)m_pWnd->GetChild( szTmp );
		m_stcBonusGoodsBack.push_back( pBonusGoodBack );
		pBonusGoodBack->SetInvisible(TRUE);

		_stprintf(szTmp, _T("taskwin\\conwinpic\\contentwnd\\content\\rewards\\choose\\itemback%d\\itempic"), i );
		IconStatic *pBonusSel = (IconStatic*)m_pWnd->GetChild( szTmp );
		m_stcBonusSels.push_back( pBonusSel );

		_stprintf(szTmp, _T("taskwin\\conwinpic\\contentwnd\\content\\rewards\\choose\\itemback%d"), i );
		GUIStatic *pBonusSelBack = (GUIStatic*)m_pWnd->GetChild( szTmp );
		m_stcBonusSelsBack.push_back( pBonusSelBack );
		pBonusSelBack->SetInvisible(TRUE);
	}

	m_pWnd->SetInvisible( TRUE );
	m_showNPC = true;
	return TRUE;
}

//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL QuestMainFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_Active:
		{
			ShowTip(pWnd, NULL);
			Show();
		}
		break;

	case EGUIE_Drag:
		{
			QuestNPCFrame *pFrame = QuestMgr::Inst()->GetNPCFrame(_T("QuestNPCQuestMain"));
			if( P_VALID(pFrame) )
			{
				pFrame->Align();
			}
			EasyBuyFrame *pQuestEasyBuyFrame = (EasyBuyFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestEasyBuyFrame"));
			if( P_VALID(pQuestEasyBuyFrame) )
			{
				pQuestEasyBuyFrame->Align();
			}
		}
		break;

	case EGUIE_Click:
		{
			if( pWnd == m_pBtnQuestNpc )			// 显示/隐藏可接任务NPC窗口
			{
				m_showNPC = !m_showNPC;
				if( m_showNPC )
					QuestMgr::Inst()->OpenNPCFrame( m_strName.c_str(), _T("QuestNPCQuestMain"), "\\desktop\\task\\taskwin", "npclist_quest" );
				else
					QuestMgr::Inst()->CloseNPCFrame( _T("QuestNPCQuestMain") );
			}
			else if( pWnd == m_pBtnActiveQuest )	// 显示/隐藏活动任务窗口
			{
				QuestMgr::Inst()->OpenActiveFrame();
			}
			else if( pWnd == m_pBtnQuestTrack )		// 显示/隐藏任务追踪窗口
			{
				m_showTrack = !m_showTrack;
				if( m_showTrack )
					QuestMgr::Inst()->OpenTrackFrame();
				else
					QuestMgr::Inst()->CloseTrackFrame();
			}
			else if( pWnd == m_pBtnDelQuest )		// 删除任务
			{
				QuestData *pData = QuestMgr::Inst()->GetData();
				ASSERT( P_VALID(pData) );

				DWORD treeID = m_pTreeTaskList->GetCurrentSelItem();
				if( treeID != GT_INVALID )
				{
					tstring questName;
					pData->GetQuestName(m_tree2quest[treeID], questName);
					TCHAR szText[128]={0};
					_stprintf( szText, g_StrTable[_T("QuestMain_CancelQuest")], questName.c_str() );
					if( P_VALID(GetObj("QuestMain_CancelQuest")) )
						KillObj("QuestMain_CancelQuest");
					CreateObj( "QuestMain_CancelQuest", "MsgBox" );
					TObjRef<MsgBox>("QuestMain_CancelQuest")->Init( _T(""),
						szText,
						_T("QuestMainFrameEvent_CancelQuest"),
						(MsgBoxFlag)(MBF_OK|MBF_Cancel),
						TRUE);
				}
				else
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestIsEmpty")]);
			}
			else if( pWnd == m_pBtnCloseWnd1 || pWnd == m_pBtnCloseWnd2 )// 关闭主窗口
			{
				ShowTip(pWnd, NULL);
				m_pMgr->SendEvent( &tagGameEvent( _T("Close_Task_UI"), this ) );
			}
		}
		break;

	case EGUIE_StaticExLinkClick:
		{
			// [<link=linksIndex,0xffffffff>玄天龙城<link=0xffffffff,0>]
			const TCHAR *szLink = (const TCHAR*)(pEvent->dwParam2);
			Vector3 pos;
			bool isMapEx = false;//是否跨场景
			DWORD dwNPCTypeID = GT_INVALID;
			QuestData *pData = QuestMgr::Inst()->GetData();
			ASSERT( P_VALID(pData) );
			tstring mapName;

			if( (INT)pEvent->dwParam1 < 1000 )//根据ID寻路
			{
				INT index = pEvent->dwParam1-1;
				if( index > (INT)m_idLinks.size()-1 )
					break;

				tagIDLink& link = m_idLinks[index];
				mapName = link.mapName;
				IMSG( _T("path map: %s typeid: %d\n"), link.mapName.c_str(), link.typeID );

				if( link.mapName != MapMgr::Inst()->GetCurMapName() )//不在本地图，进行跨场景寻路
				{
					QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
					ASSERT( P_VALID(pQuery) );
					dwNPCTypeID = link.typeID;
					isMapEx = true;
					if( QuestMgr::Inst()->MoveToMap(link.mapName.c_str(),pos) != TRUE )
						break;
				}
				else
				{
					if( link.typeID == 1 )//触发器或者其他没有TypeID的东西
					{
						if( !pData->GetTriggerPosition( szLink, pos ) )
							break;
					}
					else
					{
						QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
						ASSERT( P_VALID(pQuery) );
						dwNPCTypeID = link.typeID;
						if( !pQuery->GetNPCPosition( link.typeID, pos ) )
							break;
					}
				}
			}
			else//根据坐标寻路
			{
				INT index = pEvent->dwParam1-1000;
				if( index > (INT)m_posLinks.size()-1 )
					break;

				tagPosLink& link = m_posLinks[index];

				if( link.mapName != MapMgr::Inst()->GetCurMapName() )//不在本地图，进行跨场景寻路
				{
					if( QuestMgr::Inst()->MoveToMap(link.mapName.c_str(),pos) != TRUE )
						break;
				}
				else
				{
					pos = link.pos;
				}
			}

			LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();

			tagMouseMoveEvent event( _T("tagMouseMoveEvent"), NULL );
			event.start = pLP->GetPos();
			event.end = pos;
			event.validDist = 100.0f;
			event.dwNpcTypeID = dwNPCTypeID;
			event.bNotThisMap = isMapEx;
			event.strMapName = mapName;
			

			IMSG(_T("target pos: x-%f,y-%f,z-%f\n"),pos.x,pos.y,pos.z);

			m_pMgr->SendEvent( &event );
		}
		break;

	case EGUIE_ItemClick:
		{
			if( pWnd == m_pTreeTaskList )//选择某个任务
			{
				TreeQuestMap::iterator iter = m_tree2quest.find(pEvent->dwParam1);
				if( iter != m_tree2quest.end() )
				{
					if( m_pInput->IsKeyDown(DIK_RCONTROL) || m_pInput->IsKeyDown(DIK_LCONTROL) )//跟踪选中的任务
					{
						if( QuestMgr::Inst()->HandleTrackQuest( iter->second ) )//任务可以进行跟踪
						{
							QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
							ASSERT( P_VALID(pQuery) );

							// 完成的不显示跟踪标志
							if( !pQuery->QuestCanComplete( iter->second ) || !pQuery->QuestCanCompleteBySpecItem(iter->second) )
							{
								tagGUITreeItem *pItem = m_pTreeTaskList->GetItem( iter->first );

								QuestData *pData = QuestMgr::Inst()->GetData();
								ASSERT( P_VALID(pData) );

								TCHAR szText[20];
								tstring questName;
								INT questLvl = 0;
								if( iter->second >= QUEST_PANEL_START_TASK_ID && iter->second <= QUEST_PANEL_END_TASK_ID )
								{
									UINT16 nIndex = 0;
									BYTE byType = 0;
									if(iter->second>=YELLOW_LIST_START_TASK_ID)
									{
										nIndex = iter->second - YELLOW_LIST_START_TASK_ID;
										byType = EQBT_YellowList;
									}
									else
									{
										nIndex = iter->second - QUEST_PANEL_START_TASK_ID;
										byType = EQBT_Reward;
									}
									const vector<UINT16> &vecQuest = QuestMgr::Inst()->GetQuestPanelTask(byType);
									const tagBoardQuestProto *pProto = pData->GetQuestPanelProto(vecQuest[nIndex]);
									pData->GetQuestPanelTaskName( vecQuest[nIndex], questName );
									if(P_VALID(pProto))
										questLvl = (INT)pProto->byLevel;
								}
								else
								{
									pData->GetQuestName( iter->second, questName );
									questLvl = pData->GetQuestLevel( iter->second );
								}
								if( questLvl > 0 )
								{
									_stprintf(szText, _T("[%d]"), questLvl);
									questName = szText + questName;
								}

								if( pQuery->QuestIsTracked(iter->second) )
								{
									questName = questName + g_StrTable[_T("Quest_TrackFlag")];
								}

								pItem->strShow = questName;
							}
						}
					}
					//else // 显示任务信息
					{
						SetQuestDesc(iter->second);

						QuestData *pData = QuestMgr::Inst()->GetData();
						ASSERT( P_VALID(pData) );
						if( pData->QuestCanDelete(iter->second) )
							m_pBtnDelQuest->SetEnable(true);
						else
							m_pBtnDelQuest->SetEnable(false);
						m_pBtnDelQuest->SetRefresh();
						m_pBtnQuestTrack->SetEnable(true);
						m_pBtnQuestTrack->SetRefresh();
					}
				}
				else
				{
					m_pBtnDelQuest->SetEnable(false);
					m_pBtnDelQuest->SetRefresh();
					m_pBtnQuestTrack->SetEnable(false);
					m_pBtnQuestTrack->SetRefresh();
				}
			}
		}
		break;

	case EGUIE_MoveIntoCtrl:
		{
			for( int i=0; i<QUEST_REW_ITEM; i++ )
			{
				if( pWnd == (GUIWnd*)m_stcBonusGoods[i] )
				{
					if( m_BonusGoodsID[i] != 0 )
					{
						TreeQuestMap::iterator iter = m_tree2quest.find(m_pTreeTaskList->GetCurrentSelItem());
						if( iter != m_tree2quest.end() )
						{
							QuestData *pData = QuestMgr::Inst()->GetData();
							ASSERT( P_VALID(pData) );
							INT16 quality;
							if( iter->second >= QUEST_PANEL_START_TASK_ID && iter->second <= QUEST_PANEL_END_TASK_ID )
							{
								const tagBoardQuestProto *pProto = pData->GetQuestPanelProto(iter->second);
								if(P_VALID(pProto))
								{
									quality = (INT16)pProto->RewardItem[i].byQuality;
								}
							}
							else
								pData->GetQuestItemQuality(iter->second,i,quality);
							ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(m_BonusGoodsID[i],(BYTE)quality).c_str() );
						}
						break;
					}
				}

				if( pWnd == (GUIWnd*)m_stcBonusSels[i] )
				{
					if( m_BonusSelsID[i] != 0 )
					{
						TreeQuestMap::iterator iter = m_tree2quest.find(m_pTreeTaskList->GetCurrentSelItem());
						if( iter != m_tree2quest.end() )
						{
							QuestData *pData = QuestMgr::Inst()->GetData();
							ASSERT( P_VALID(pData) );
							INT16 quality;
							pData->GetQuestChoiceItemQuality(iter->second,i,quality);
							ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(m_BonusSelsID[i],(BYTE)quality).c_str() );
						}
					}
				}
			}
		}
		break;

	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;

	default:
		break;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// 删除任务
//-----------------------------------------------------------------------------
VOID QuestMainFrame::DeleteQuest( UINT16 questID )
{
	ASSERT( P_VALID(m_pWnd) );

	TreeQuestMap::iterator iter;
	for( iter = m_tree2quest.begin(); iter != m_tree2quest.end(); ++iter )
	{
		if( iter->second == questID )	// 删除此任务的Item
		{
			m_pTreeTaskList->RemoveItem( iter->first );
			m_tree2quest.erase(iter);
			break;
		}
	}

	if( m_tree2quest.empty() )	// 当前任务全部删除完毕后，需要在任务描述框中显示“没有正在进行的任务”
	{
		m_pStcCondition->SetText( g_StrTable[_T("Quest_None")] );
		m_pStcBonusExp->SetText(_T(""));
		m_pStcBonusMoney1->SetText(_T(""));
		m_pStcBonusMoney2->SetText(_T(""));

		//for( size_t i = 0; i < m_pStcBonusSpecT.size(); i++ )
		//	m_pStcBonusSpecT[i]->SetText(_T(""));
		//for( size_t i = 0; i < m_pStcBonusSpecV.size(); i++ )
		//	m_pStcBonusSpecV[i]->SetText(_T(""));
		for( UINT i=0; i<m_stcBonusGoods.size(); i++ )
		{
			m_stcBonusGoodsBack[i]->SetInvisible(TRUE);
		}
		for( UINT i=0; i<m_stcBonusSels.size(); i++ )
		{
			m_stcBonusSelsBack[i]->SetInvisible(TRUE);
		}
		m_pBtnDelQuest->SetEnable(true);
		m_pStcMoneyReward->SetInvisible(TRUE);
		m_pStcFixReward->SetInvisible(TRUE);
		m_pStcOptionReward->SetInvisible(TRUE);
	}
	else
	{
		m_pTreeTaskList->SetCurrentSelItem(m_pTreeTaskList->GetCurrentSelItem(), TRUE);
	}
	ShowQuestNum();
}

//-----------------------------------------------------------------------------
// 加入任务
//-----------------------------------------------------------------------------
VOID QuestMainFrame::AddQuest( UINT16 questID, bool sel )
{
	if( !P_VALID(m_pWnd) )
		return;

	if( m_tree2quest.empty() )	// 加入第一个任务时，需要做些额外处理，插入根节点
	{
		m_pTreeTaskList->InsterItem(0, _T("Root"), _T("根节点"));
	}

	QuestData *pData = QuestMgr::Inst()->GetData();
	ASSERT( P_VALID(pData) );

	TCHAR szText[20];
	tstring questName;
	INT questLvl = 0;
	if( questID >= QUEST_PANEL_START_TASK_ID && questID <= QUEST_PANEL_END_TASK_ID )
	{
		UINT16 nIndex = 0;
		BYTE byType = 0;
		if(questID>=YELLOW_LIST_START_TASK_ID)
		{
			nIndex = questID - YELLOW_LIST_START_TASK_ID;
			byType = EQBT_YellowList;
		}
		else
		{
			nIndex = questID - QUEST_PANEL_START_TASK_ID;
			byType = EQBT_Reward;
		}
		const vector<UINT16> &vecQuest = QuestMgr::Inst()->GetQuestPanelTask(byType);
		const tagBoardQuestProto *pProto = pData->GetQuestPanelProto(vecQuest[nIndex]);
		pData->GetQuestPanelTaskName( vecQuest[nIndex], questName );
		if(P_VALID(pProto))
			questLvl = (INT)pProto->byLevel;
	}
	else
	{
		pData->GetQuestName(questID,questName);
		questLvl = pData->GetQuestLevel(questID);
	}
	if( questLvl > 0 )
	{
		_stprintf(szText, _T("[%d]"), questLvl);
		questName = szText + questName;
	}

	DWORD parentID = GT_INVALID;
	DWORD treeID = GT_INVALID;
	INT16 questType = 0;
	tstring strTypeName = _T("");
	pData->GetQuestType(questID, questType);
	pData->GetQuestTypeName(questType, strTypeName);

	TCHAR szTmp[X_SHORT_NAME];
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), questType);
	parentID = m_pTreeTaskList->InsterItem(_T("\\Root"), szTmp, strTypeName.c_str(), TRUE);
	_stprintf( szText, _T("%d_%d"), questID, questLvl );
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("\\Root\\%d"), questType);
	treeID = m_pTreeTaskList->InsterItem(szTmp, szText, questName.c_str(), TRUE );

	// 加入任务失败
	if( !P_VALID(treeID) )
		return;

	// 设置颜色
	DWORD color = 0xFFFFF7E0;
	pData->GetQuestTypeColor(questID, color);
	tagGUITreeItem *pItem = NULL;
	if( P_VALID( parentID ) )
	{
		pItem = m_pTreeTaskList->GetItem( parentID );
		pItem->dwColor = color;
	}
	pItem = m_pTreeTaskList->GetItem( treeID );
	if( questID >= QUEST_PANEL_START_TASK_ID && questID <= QUEST_PANEL_END_TASK_ID )
	{
		color = QuestMgr::Inst()->GetQuestPanelTaskColor(questID);
		pItem->dwColor = color;
	}
	else
	{
		color = pData->GetQuestLevelColor( questID, RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel() );
		pItem->dwColor = color;
	}

	QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
	ASSERT( P_VALID(pQuery) );
	if( pQuery->QuestCanComplete(questID) || pQuery->QuestCanCompleteBySpecItem(questID) )
	{
		pItem->strShow += g_StrTable[_T("Quest_CompleteFlag")];
	}
	else if( pQuery->QuestCanTrack(questID) )
	{
		pItem->strShow += g_StrTable[_T("Quest_TrackFlag")];
	}

	m_tree2quest[treeID] = questID;
	if( sel )
	{
		m_pTreeTaskList->SetCurrentSelItem(treeID, TRUE);
		tagGUIEvent event(m_pSrlCondition, EGUIE_Scroll);
		event.dwParam1 = (DWORD)0;
		event.dwParam2 = (DWORD)0;
		event.dwParam3 = (DWORD)0;
		m_pSrlCondition->SendEvent(&event);
		m_pSrlCondition->SetValue(0);
	}
	ShowQuestNum();
}

//-----------------------------------------------------------------------------
// 根据玩家等级重新设置任务颜色
//-----------------------------------------------------------------------------
VOID QuestMainFrame::ChangeColor( void )
{
	_ChangeColor( m_pTreeTaskList->GetRoot() );

	m_pTreeTaskList->SetRefresh(TRUE);
}

VOID QuestMainFrame::_ChangeColor( tagGUITreeItem* pItem )
{
	if( pItem != NULL )
	{
		if( pItem->listChildren.empty() )//任务节点，开始变色
		{
			tstring strQuestID = pItem->strText;
			tstring::size_type pos(0);
			if( (pos=strQuestID.find(_T("_"))) != tstring::npos )
			{
				strQuestID.replace(pos,strQuestID.length()-pos,_T(""));
				UINT16 questID = (UINT16)_tstoi(strQuestID.c_str());

				QuestData *pData = QuestMgr::Inst()->GetData();
				ASSERT( P_VALID(pData) );

				if( questID >= QUEST_PANEL_START_TASK_ID && questID <= QUEST_PANEL_END_TASK_ID )
				{

				}
				else
					pItem->dwColor = pData->GetQuestLevelColor( questID, RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel() );
			}
		}
		else//继续迭代
		{
			std::list<tagGUITreeItem*>::iterator iter;
			for( iter=pItem->listChildren.begin(); iter!=pItem->listChildren.end(); ++iter )
			{
				_ChangeColor( *iter );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 显示窗口
//-----------------------------------------------------------------------------
VOID QuestMainFrame::Show( bool bReset )
{
	if( P_VALID(m_pWnd) ) 
	{
		if( bReset )
		{
			tagGUIEvent event(m_pSrlCondition, EGUIE_Scroll);
			event.dwParam1 = (DWORD)0;
			event.dwParam2 = (DWORD)0;
			event.dwParam3 = (DWORD)0;
			m_pSrlCondition->SendEvent(&event);
			m_pSrlCondition->SetValue(0);
		}

		m_pWnd->SetInvisible(FALSE); 
		m_pWnd->FlipToTop();

		QuestNPCFrame *pFrame = QuestMgr::Inst()->GetNPCFrame(_T("QuestNPCQuestMain"));
		if( P_VALID(pFrame) )
			pFrame->Show();
		else if( m_showNPC )
			QuestMgr::Inst()->OpenNPCFrame( m_strName.c_str(), _T("QuestNPCQuestMain"), "\\desktop\\task\\taskwin", "npclist_quest" );
	}
}

//-----------------------------------------------------------------------------
// 隐藏窗口
//-----------------------------------------------------------------------------
VOID QuestMainFrame::Hide( void )
{
	if( P_VALID(m_pWnd) ) 
		m_pWnd->SetInvisible(TRUE);

	QuestMgr::Inst()->CloseNPCFrame( _T("QuestNPCQuestMain"), true );
}

//-----------------------------------------------------------------------------
// 窗口是否可见
//-----------------------------------------------------------------------------
BOOL QuestMainFrame::IsVisible( void ) const
{
	if( P_VALID(m_pWnd) )
	{
		return !m_pWnd->IsInvisible();
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// 显示某个任务的描述信息
//-----------------------------------------------------------------------------
VOID QuestMainFrame::SetQuestDesc( UINT16 questID )
{
	m_pStcMoneyReward->SetInvisible(TRUE);
	m_pStcFixReward->SetInvisible(TRUE);
	m_pStcOptionReward->SetInvisible(TRUE);
	m_pStcBack->SetResizable(TRUE);
	m_pStcBack->Resize( m_ptOriSize );
	m_pStcBack->SetResizable(FALSE);
	m_ptOriReWSize.y = 0;
	m_pStcRewardBack->SetResizable(TRUE);
	m_pStcRewardBack->Resize(m_ptOriReWSize);
	m_pStcRewardBack->SetResizable(FALSE);
	tstring desc;
	QuestData *pData = QuestMgr::Inst()->GetData();
	ASSERT( P_VALID(pData) && _T("任务系统获得QuestData错误") );

	const tagQuestProto* pQuestProto = pData->GetQuestProto( questID );
	ASSERT( P_VALID(pQuestProto) );

	tstringstream stream;
	tstring strView;
	//stream << g_StrTable[_T("Quest_DescAcceptConditions")]
	//	<< _T("\\n    ") << pQuestProto->conditions;

	// 解析over_view
	if( questID >= QUEST_PANEL_START_TASK_ID && questID <= QUEST_PANEL_END_TASK_ID )
	{
		UINT16 nIndex = 0;
		BYTE byType = 0;
		if(questID>=YELLOW_LIST_START_TASK_ID)
		{
			nIndex = questID - YELLOW_LIST_START_TASK_ID;
			byType = EQBT_YellowList;
		}
		else
		{
			nIndex = questID - QUEST_PANEL_START_TASK_ID;
			byType = EQBT_Reward;
		}
		const vector<UINT16> &vecQuest = QuestMgr::Inst()->GetQuestPanelTask(byType);
		pData->GetQuestPanelTaskDesc( vecQuest[nIndex], strView );
	}
	else
		strView = pQuestProto->over_view;
	QuestMgr::Inst()->ParseQuestView(strView,m_idLinks,m_posLinks);
	
	stream << g_StrTable[_T("Quest_DescOverview")] << _T("\\n    ") << strView;

	

	if( pData->QuestRequireItemOrCreature(questID)||pData->QuestRequireLevel(questID))
		stream << _T("\\n \\n")	<< g_StrTable[_T("Quest_DescCompleteConditions")] << _T("\\n");

	QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
	ASSERT( P_VALID(pQuery) && _T("QuestMainFrame获得QuestQuery错误") );

	const tagQuestDynamicTarget *pDynamicTarget = pData->GetQuestDynamicTarget(questID);
	if( P_VALID(pDynamicTarget) )//动态
	{
		UINT16 itemCount,creatureCount,itemSum,creatureSum;
		DWORD itemID,creatureID;

		switch( pDynamicTarget->eTargetType )
		{
		case EQTT_Kill:
			{
				// 怪物信息
				for( int i=0; i<DYNAMIC_TARGET_COUNT; i++ )
				{
					if( pDynamicTarget->dwTargetID[i] == 0 )
						continue;
					if( pData->GetQuestRequiredCreatureCount( questID , i, creatureID, creatureSum ) )
					{
						const tagCreatureProto* creatureProto = CreatureData::Inst()->FindNpcAtt(creatureID);
						if( P_VALID(creatureProto) )
						{
							TCHAR szCreatureID[15];
							_ui64tot(creatureID,szCreatureID,16);

							pQuery->GetQuestCreatureCount( questID , i, creatureCount );
							if( creatureCount > creatureSum )
							{
								creatureCount = creatureSum;
							}

							stream << _T("    ") << g_StrTable[_T("Quest_TrackKillCreatureDesc")] << _T(" ")
								<< creatureProto->szName << _T(" ") << creatureCount << _T("/") << creatureSum << _T("\\n");
						}
					}
				}
			}
			break;
		case EQTT_Collect:
			{
				// 物品信息
				for( int i=0; i<DYNAMIC_TARGET_COUNT; i++ )
				{
					if( pData->GetQuestRequiredItemCount( questID , i, itemID, itemSum ) )
					{
						const tagItemDisplayInfo* itemProto = ItemProtoData::Inst()->FindItemDisplay(itemID);
						if( P_VALID(itemProto) )
						{
							TCHAR szItemID[15];
							_ui64tot(itemID,szItemID,16);

							pQuery->GetQuestItemCount( questID , i, itemCount );
							if( itemCount > itemSum )
							{
								itemCount = itemSum;
							}

							stream << _T("    ") << g_StrTable[_T("Quest_TrackGetItemDesc")] << _T(" ") 
								<< itemProto->szName << _T(" ") << itemCount << _T("/") << itemSum << _T("\\n");
						}
					}
				}
			}
			break;
		case EQTT_NPCTalk:
			{
				// 对话NPC跟踪
				for( int i=0; i<3; i++ )
				{
					if( pDynamicTarget->dwTargetID[i] == 0 )
						break;

					tstring strTalked;
					if( pData->NPCBeTalked( questID, i ) )
						strTalked = g_StrTable[_T("QuestTrack_NPCTalked")];
					else
						strTalked = g_StrTable[_T("QuestTrack_NPCNoTalk")];

					const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( pDynamicTarget->dwTargetID[i] );
					if( P_VALID(creatureProto) )
					{
						TCHAR szQuestTrack[X_LONG_NAME] = {0};
						_stprintf( szQuestTrack,
							_T("  <color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0> <color=0xFFF6EB78>(%s)<color=0xFFFFF7E0>\\n"),
							creatureProto->dwTypeID,
							creatureProto->szName,
							strTalked.c_str() );
						stream << szQuestTrack;
					}
				}
			}
			break;
		case EQTT_Invest:
			{
				// 调查地物追踪
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
					if( P_VALID(creatureProto) )
					{
						TCHAR szQuestTrack[X_LONG_NAME] = {0};
						_stprintf( szQuestTrack,
							_T("  <color=0xFFABABFF><link=%x,0xFFABABFF>%s<link=0xffffffff,0> <color=0xFFF6EB78>(%s)<color=0xFFFFF7E0>\\n"),
							creatureProto->dwTypeID,
							creatureProto->szName,
							strInves.c_str() );
						stream << szQuestTrack;
					}
				}
			}
			break;
		}

		TCHAR szTemp[X_LONG_NAME] = {0};
		_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pDynamicTarget->nRewExp);
		m_pStcBonusExp->SetText(szTemp);
		_sntprintf( szTemp, X_LONG_NAME, _T("%d"), MSilver2DBGold(pDynamicTarget->nRewMoney));
		m_pStcBonusMoney1->SetText(szTemp);
		_sntprintf( szTemp, X_LONG_NAME, _T("%d"), MSilver2DBSilver(pDynamicTarget->nRewMoney));
		m_pStcBonusMoney2->SetText(szTemp);

		if( pDynamicTarget->nRewExp != 0 || pDynamicTarget->nRewMoney != 0 )
			InsertWnd(m_pStcMoneyReward);
		
		// 物品奖励
		for( int i=0; i<QUEST_REW_ITEM-1; i++ )
		{
			if( pDynamicTarget->dwRewItemID[i] != 0 )
			{
				tstring strPic = ItemMgr::Inst()->GetItemIconPath( pDynamicTarget->dwRewItemID[i] );
				m_stcBonusGoods[i]->SetPic( strPic );

				TCHAR szNum[5];
				_stprintf(szNum, _T("%4d"), pDynamicTarget->dwRewItemNum[i]);
				m_stcBonusGoods[i]->SetText( szNum );
				m_BonusGoodsID[i] = pDynamicTarget->dwRewItemID[i];

				m_stcBonusGoodsBack[i]->SetInvisible(FALSE);
			}
			else
			{
				m_BonusGoodsID[i] = 0;
				m_stcBonusGoodsBack[i]->SetInvisible(TRUE);
			}
		}
		if( pDynamicTarget->dwRewItemID[0] != 0 )
			InsertWnd(m_pStcFixReward);

		// 可选物品奖励
		for( int i=0; i<QUEST_REW_ITEM-1; i++ )
		{
			if( pDynamicTarget->dwOptionRewItemID[i] != 0 )
			{
				tstring strPic = ItemMgr::Inst()->GetItemIconPath( pDynamicTarget->dwOptionRewItemID[i] );
				m_stcBonusSels[i]->SetPic( strPic );

				TCHAR szNum[5];
				_stprintf(szNum, _T("%4d"), pDynamicTarget->dwOptionRewItemNum[i]);
				m_stcBonusSels[i]->SetText( szNum );
				m_BonusSelsID[i] = pDynamicTarget->dwOptionRewItemID[i];

				m_stcBonusSelsBack[i]->SetInvisible(FALSE);
			}
			else
			{
				m_BonusSelsID[i] = 0;
				m_stcBonusSelsBack[i]->SetInvisible(TRUE);
			}
		}
		if( pDynamicTarget->dwOptionRewItemID[0] != 0 )
			InsertWnd(m_pStcOptionReward);
	}
	else
	{
		// 等级信息
		const tagQuestProto* pQuest = pData->GetQuestProto(questID);
		ASSERT( pQuest != NULL );
		TCHAR szQuestTrack[512];
		if( pQuest->complete_req_level != 0 )
		{
			_stprintf( szQuestTrack, 
				_T("  <color=0xFFFFF7E0>%s<color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"), 
				g_StrTable[_T("SelectRole_Lvl")], 
				RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel(), 
				pQuest->complete_req_level );
			stream<< _T(" ")<< szQuestTrack<< _T("\\n");
		}
		
		UINT16 itemCount,creatureCount,itemSum,creatureSum;
		DWORD itemID,creatureID;

		// 物品信息
		for( int i=0; i<QUEST_ITEMS_COUNT; i++ )
		{
			if( pData->GetQuestRequiredItemCount( questID , i, itemID, itemSum ) )
			{
				const tagItemDisplayInfo* itemProto = ItemProtoData::Inst()->FindItemDisplay(itemID);
				ASSERT( itemProto != NULL );

				TCHAR szItemID[15];
				_ui64tot(itemID,szItemID,16);

				pQuery->GetQuestItemCount( questID , i, itemCount );
				if( itemCount > itemSum )
				{
					itemCount = itemSum;
				}

				stream << _T("    ") << g_StrTable[_T("Quest_TrackGetItemDesc")] << _T(" ") 
					<< itemProto->szName << _T(" ") << itemCount << _T("/") << itemSum << _T("\\n");
			}
		}

		// 怪物信息
		for( int i=0; i<QUEST_CREATURES_COUNT; i++ )
		{
			if( pData->GetQuestRequiredCreatureCount( questID , i, creatureID, creatureSum ) )
			{
				const tagCreatureProto* creatureProto = CreatureData::Inst()->FindNpcAtt(creatureID);
				ASSERT( creatureProto != NULL );

				TCHAR szCreatureID[15];
				_ui64tot(creatureID,szCreatureID,16);

				pQuery->GetQuestCreatureCount( questID , i, creatureCount );
				if( creatureCount > creatureSum )
				{
					creatureCount = creatureSum;
				}

				stream  << _T("    ") << g_StrTable[_T("Quest_TrackKillCreatureDesc")] << _T(" ")
					<< creatureProto->szName << _T(" ") << creatureCount << _T("/") << creatureSum << _T("\\n");
			}
		}

		// 特殊目标任务
// 		if( pQuest->spec_condition )
// 			stream << _T("  ") << pQuest->conditions << _T("\\n");

		// 传道
		if( pQuest->chuandao_count > 0 )
		{
			UINT16 nCount = 0;
			if(pQuery->GetQuestKillTimes(questID, nCount))
			{
				if(nCount > pQuest->chuandao_count)
					nCount = pQuest->chuandao_count;
				_stprintf( szQuestTrack, 
					_T("  <color=0xFFFFF7E0>%s<color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"), 
					g_StrTable[_T("QuestKillChuanDaoMonster")], nCount, pQuest->chuandao_count );
			}

			stream << szQuestTrack;
		}

		// 任务无阻令
		const QuestData::tagQuestCompleteConditionBySpecItem *pItem = pData->GetQuestItemConditionSpecItem(questID);
		if (P_VALID(pItem))
		{
			ItemContainer* pItemContainer = ItemMgr::Inst()->GetPocket();
			if (P_VALID(pItemContainer))
			{
				int nNum = pItemContainer->GetItemQuantity(pItem->itemid);
				if (nNum >= pItem->itemnum)
					nNum = pItem->itemnum;

				_stprintf( szQuestTrack, 
					_T("  <color=0xFFFFF7E0>%s<color=0xFFF6EB78>(%d/%d)<color=0xFFFFF7E0>\\n"), 
					g_StrTable[_T("QuickDoneQuest")], 
					nNum, 
					pItem->itemnum );
				stream << _T("    ") << szQuestTrack;
			}
		}

		// 奖励
		TCHAR szRewXP[10];
		_ultot(pQuestProto->rew_xp, szRewXP, 10);
		m_pStcBonusExp->SetText( szRewXP );

		TCHAR szRewGold[5],szRewSilver[5];
		_ultot( MSilver2DBGold(pQuestProto->rew_money), szRewGold, 10 );
		_ultot( MSilver2DBSilver(pQuestProto->rew_money), szRewSilver, 10 );
		m_pStcBonusMoney1->SetText( szRewGold );
		m_pStcBonusMoney2->SetText( szRewSilver );

		if( pQuestProto->rew_xp != 0 || pQuestProto->rew_money != 0 )
			InsertWnd(m_pStcMoneyReward);
	}

	if( questID >= QUEST_PANEL_START_TASK_ID && questID <= QUEST_PANEL_END_TASK_ID )
	{
		UINT16 nIndex = 0;
		BYTE byType = 0;
		if(questID>=YELLOW_LIST_START_TASK_ID)
		{
			nIndex = questID - YELLOW_LIST_START_TASK_ID;
			byType = EQBT_YellowList;
		}
		else
		{
			nIndex = questID - QUEST_PANEL_START_TASK_ID;
			byType = EQBT_Reward;
		}
		const vector<UINT16> &vecQuest = QuestMgr::Inst()->GetQuestPanelTask(byType);
		const tagBoardQuestProto *pProto = pData->GetQuestPanelProto(vecQuest[nIndex]);
		if(P_VALID(pProto))
		{
			// 经验金钱奖励
			m_pStcBonusExp->SetText(_T(""));
			m_pStcBonusMoney1->SetText(_T(""));
			m_pStcBonusMoney2->SetText(_T(""));
			TCHAR szTemp[X_LONG_NAME] = {0};
			_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pProto->nRewardExp );
			m_pStcBonusExp->SetText(szTemp);
			_sntprintf( szTemp, X_LONG_NAME, _T("%d"), MSilver2DBGold(pProto->nRewardMoney) );
			m_pStcBonusMoney1->SetText(szTemp);
			_sntprintf( szTemp, X_LONG_NAME, _T("%d"), MSilver2DBSilver(pProto->nRewardMoney) );
			m_pStcBonusMoney2->SetText(szTemp);

			if( pProto->nRewardExp != 0 || pProto->nRewardMoney != 0 )
				InsertWnd(m_pStcMoneyReward);

			// 物品奖励
			for( int i=0; i < BOARD_QUEST_REWARD_NUM; i++ )
			{
				if( pProto->RewardItem[i].dwID != GT_INVALID )
				{
					tstring strPic = ItemMgr::Inst()->GetItemIconPath( pProto->RewardItem[i].dwID );
					m_stcBonusGoods[i]->SetPic( strPic );
					TCHAR szNum[5];
					_stprintf(szNum, _T("%4d"), pProto->RewardItem[i].byNum);
					m_stcBonusGoods[i]->SetText( szNum );
					m_BonusGoodsID[i] = pProto->RewardItem[i].dwID;
					m_stcBonusGoodsBack[i]->SetInvisible(FALSE);
				}
				else
				{
					m_BonusGoodsID[i] = 0;
					m_stcBonusGoodsBack[i]->SetInvisible(TRUE);
				}
			}

			if( pProto->RewardItem[0].dwID != GT_INVALID )
				InsertWnd(m_pStcFixReward);
		}
	}
	else
	{
		// 物品奖励
		for( int i=0; i<QUEST_REW_ITEM; i++ )
		{
			if( pQuestProto->rew_item[i] != 0 )
			{
				tstring strPic = ItemMgr::Inst()->GetItemIconPath( pQuestProto->rew_item[i] );
				m_stcBonusGoods[i]->SetPic( strPic );

				TCHAR szNum[5];
				_stprintf(szNum, _T("%4d"), pQuestProto->rew_item_num[i]);
				m_stcBonusGoods[i]->SetText( szNum );
				m_BonusGoodsID[i] = pQuestProto->rew_item[i];

				m_stcBonusGoodsBack[i]->SetInvisible(FALSE);
			}
			else
			{
				m_BonusGoodsID[i] = 0;
				m_stcBonusGoodsBack[i]->SetInvisible(TRUE);
			}
		}

		if( pQuestProto->rew_item[0] != 0 )
			InsertWnd(m_pStcFixReward);

		INT nValue = static_cast<INT>(RoleMgr::Inst()->GetLocalPlayer()->GetPlayerClass());
		for( int i=0; i<QUEST_CLASS_REW_COUNT; i++ )
		{
			if( pQuestProto->rew_class_item[i].nClass & ( 1 << (nValue - 1) ) )
			{
				for( int j = 0; j < CLASS_REWARD_ITEM_NUM; j++ )
				{
					if( pQuestProto->rew_class_item[i].Item[j].dwItemID != 0 )
					{
						tstring strPic = ItemMgr::Inst()->GetItemIconPath( pQuestProto->rew_class_item[i].Item[j].dwItemID );
						m_stcBonusGoods[j]->SetPic( strPic );

						TCHAR szNum[5];
						_stprintf(szNum, _T("%4d"), pQuestProto->rew_class_item[i].Item[j].byNum);
						m_stcBonusGoods[j]->SetText( szNum );
						m_BonusGoodsID[j] = pQuestProto->rew_class_item[i].Item[j].dwItemID;
						m_stcBonusGoodsBack[j]->SetInvisible(FALSE);
					}
					else
					{
						m_BonusGoodsID[j] = 0;
						m_stcBonusGoodsBack[j]->SetInvisible(TRUE);
					}
				}
				m_BonusGoodsID[4] = 0;
				m_stcBonusGoodsBack[4]->SetInvisible(TRUE);
				InsertWnd(m_pStcFixReward);
				break;
			}
		}
		
		// 可选物品奖励
		for( int i=0; i<QUEST_REW_ITEM; i++ )
		{
			if( pQuestProto->rew_choice_item[i] != 0 )
			{
				tstring strPic = ItemMgr::Inst()->GetItemIconPath( pQuestProto->rew_choice_item[i] );
				m_stcBonusSels[i]->SetPic( strPic );

				TCHAR szNum[5];
				_stprintf(szNum, _T("%4d"), pQuestProto->rew_choice_item_num[i]);
				m_stcBonusSels[i]->SetText( szNum );
				m_BonusSelsID[i] = pQuestProto->rew_choice_item[i];

				m_stcBonusSelsBack[i]->SetInvisible(FALSE);
			}
			else
			{
				m_BonusSelsID[i] = 0;
				m_stcBonusSelsBack[i]->SetInvisible(TRUE);
			}
		}
		if( pQuestProto->rew_choice_item[0] != 0 )
			InsertWnd(m_pStcOptionReward);
	}
	m_pStcCondition->SetText( stream.str().c_str() );

	tagGUIEvent event(m_pSrlCondition, EGUIE_Scroll);
	event.dwParam1 = (DWORD)0;
	event.dwParam2 = (DWORD)0;
	event.dwParam3 = (DWORD)0;
	m_pSrlCondition->SendEvent(&event);
	m_pSrlCondition->SetValue(0);

	tagPoint size = m_pStcCondition->GetSize();
	size.y = m_pStcCondition->GetTextTotalHeight();
	m_pStcCondition->SetResizable(TRUE);
	m_pStcCondition->Resize( size );
	m_pStcCondition->SetResizable(FALSE);
	tagPoint ptSrcPt = m_pStcCondition->GetClient();
	ptSrcPt.y += m_pStcCondition->GetSize().y;
	size.y = ptSrcPt.y + m_pStcRewardBack->GetSize().y;
	m_pStcBack->SetResizable(TRUE);
	m_pStcBack->Resize( size );
	m_pStcBack->SetResizable(FALSE);

	m_pStcRewardBack->SetMovable(TRUE);
	m_pStcRewardBack->Move(ptSrcPt);
	m_pStcRewardBack->SetMovable(FALSE);

	// 声望、贡献、属性、技能、buff
	//QuestMgr::Inst()->SetSpeRew(pQuestProto,m_pStcBonusSpecT,m_pStcBonusSpecV);
}

VOID QuestMainFrame::SetQuestCompleteFlag( UINT16 questID )
{
	TreeQuestMap::const_iterator iter;
	for( iter = m_tree2quest.begin(); iter != m_tree2quest.end(); ++iter )
	{
		if( iter->second == questID )
		{
			tagGUITreeItem *pItem = m_pTreeTaskList->GetItem( iter->first );

			QuestData *pData = QuestMgr::Inst()->GetData();
			ASSERT( P_VALID(pData) );

			tstring questName;
			INT questLvl = 0;
			TCHAR szLvl[20];
			if( questID >= QUEST_PANEL_START_TASK_ID && questID <= QUEST_PANEL_END_TASK_ID )
			{
				UINT16 nIndex = 0;
				BYTE byType = 0;
				if(questID>=YELLOW_LIST_START_TASK_ID)
				{
					nIndex = questID - YELLOW_LIST_START_TASK_ID;
					byType = EQBT_YellowList;
				}
				else
				{
					nIndex = questID - QUEST_PANEL_START_TASK_ID;
					byType = EQBT_Reward;
				}
				const vector<UINT16> &vecQuest = QuestMgr::Inst()->GetQuestPanelTask(byType);
				const tagBoardQuestProto *pProto = pData->GetQuestPanelProto(vecQuest[nIndex]);
				pData->GetQuestPanelTaskName( vecQuest[nIndex], questName );
				if(P_VALID(pProto))
					questLvl = (INT)pProto->byLevel;
			}
			else
			{
				pData->GetQuestName( iter->second, questName );
				questLvl = pData->GetQuestLevel(questID);
			}
			if( questLvl > 0 )
			{
				_stprintf(szLvl, _T("[%d]"), questLvl);
				questName = szLvl + questName;
			}
			pItem->strShow = questName + g_StrTable[_T("Quest_CompleteFlag")];
		}
	}
}

VOID QuestMainFrame::SetQuestTrackFlag( UINT16 questID )
{
	TreeQuestMap::const_iterator iter;
	for( iter = m_tree2quest.begin(); iter != m_tree2quest.end(); ++iter )
	{
		if( iter->second == questID )
		{
			tagGUITreeItem *pItem = m_pTreeTaskList->GetItem( iter->first );

			QuestData *pData = QuestMgr::Inst()->GetData();
			ASSERT( P_VALID(pData) );

			tstring questName;
			INT questLvl = 0;
			TCHAR szLvl[20];
			if( questID >= QUEST_PANEL_START_TASK_ID && questID <= QUEST_PANEL_END_TASK_ID )
			{
				UINT16 nIndex = 0;
				BYTE byType = 0;
				if(questID>=YELLOW_LIST_START_TASK_ID)
				{
					nIndex = questID - YELLOW_LIST_START_TASK_ID;
					byType = EQBT_YellowList;
				}
				else
				{
					nIndex = questID - QUEST_PANEL_START_TASK_ID;
					byType = EQBT_Reward;
				}
				const vector<UINT16> &vecQuest = QuestMgr::Inst()->GetQuestPanelTask(byType);
				const tagBoardQuestProto *pProto = pData->GetQuestPanelProto(vecQuest[nIndex]);
				pData->GetQuestPanelTaskName( vecQuest[nIndex], questName );
				if(P_VALID(pProto))
					questLvl = (INT)pProto->byLevel;
			}
			else
			{
				pData->GetQuestName( iter->second, questName );
				questLvl = pData->GetQuestLevel(questID);
			}
			if( questLvl > 0 )
			{
				_stprintf(szLvl, _T("[%d]"), questLvl);
				questName = szLvl + questName;
			}
			pItem->strShow = questName + g_StrTable[_T("Quest_TrackFlag")];
		}
	}
}

VOID QuestMainFrame::UpdateQuestDesc( UINT16 questID )
{
	if( questID == m_tree2quest[ m_pTreeTaskList->GetCurrentSelItem() ] )
	{
		SetQuestDesc( questID );
	}
}

DWORD QuestMainFrame::OnEvent_CloseTrack( tagGameEvent* pEvent )
{
	m_showTrack = false;

	return 0;
}

DWORD QuestMainFrame::OnEvent_CloseNPC( tagGameEvent* pEvent )
{
	m_showNPC = false;

	return 0;
}

DWORD QuestMainFrame::OnEvent_OpenNPC(tagGameEvent* pEvent)
{
	m_showNPC = true;
	return 0;
}


DWORD QuestMainFrame::OnEvent_CancelQuest( tagMsgBoxEvent* pEvent )
{
	if(MBF_OK == pEvent->eResult)
	{
		DWORD treeID = m_pTreeTaskList->GetCurrentSelItem();
		QuestMgr::Inst()->SendDeleteQuest(m_tree2quest[treeID]);
	}

	return 0;
}

BOOL QuestMainFrame::EscCancel()
{
	if( P_VALID(m_pWnd) && !m_pWnd->IsInvisible() )
	{
		ShowTip(m_pWnd, NULL);
		m_pMgr->SendEvent( &tagGameEvent( _T("Close_Task_UI"), this ) );

		return TRUE;
	}

	return GameFrame::EscCancel();
}

VOID QuestMainFrame::InitFocus( void )
{
	if( !P_VALID(m_pWnd) )
		return;

	if( !m_tree2quest.empty() )
	{
		DWORD dwCrc32 = m_pUtil->Crc32(_T("\\Root"));
		tagGUITreeItem *pItem = m_pTreeTaskList->GetItem(dwCrc32);
		while( !pItem->listChildren.empty() )
		{
			pItem = *(pItem->listChildren.begin());
		}

		m_pTreeTaskList->SetCurrentSelItem( m_pUtil->Crc32( pItem->strName.c_str() ), TRUE );
		tagGUIEvent event(m_pSrlCondition, EGUIE_Scroll);
		event.dwParam1 = (DWORD)0;
		event.dwParam2 = (DWORD)0;
		event.dwParam3 = (DWORD)0;
		m_pSrlCondition->SendEvent(&event);
		m_pSrlCondition->SetValue(0);
	}
}

VOID QuestMainFrame::UpdateQuickDoneQuest()
{
	QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
	ASSERT( P_VALID(pQuery) );
	const QuestQuery::QuestsMap& quests = pQuery->GetCurrentQuests();
	for( QuestQuery::QuestsMap::const_iterator iter = quests.begin(); iter!=quests.end(); ++iter )
	{
		UpdateQuestDesc(iter->first);

		if( pQuery->UpdateQuestState( iter->first ) )
			SetQuestCompleteFlag( iter->first );
		else
			SetQuestTrackFlag(iter->first);
	}
}

VOID QuestMainFrame::InsertWnd( GUIWnd *pWnd )
{
	GUIWnd *pFather = pWnd->GetFather();
	if( !P_VALID(pFather) )
		return;
	tagPoint ptSize = pFather->GetSize();
	ptSize.y += pWnd->GetSize().y;
	pFather->SetResizable(TRUE);
	pFather->Resize( ptSize );
	pFather->SetResizable(FALSE);

	m_ptPos.x = 0;
	pWnd->SetMovable(TRUE);
	pWnd->Move(m_ptPos);
	pWnd->SetMovable(FALSE);
	pWnd->SetInvisible(FALSE);
	m_ptPos.y += pWnd->GetSize().y + pWnd->GetClient().y;
}

VOID QuestMainFrame::ShowQuestNum()
{
	int nNum = m_tree2quest.size();
	TCHAR szText[128] = {0};
	if(nNum == QUEST_MAX_COUNT)
		_stprintf(szText, _T("<color=0xffff0000>%d/%d"), nNum, QUEST_MAX_COUNT);
	else
		_stprintf(szText, _T("<color=0xffffff00>%d/%d"), nNum, QUEST_MAX_COUNT);
	m_pStcExQuestNum->SetText(szText);
	m_pWnd->SetRefresh(TRUE);
}