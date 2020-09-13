/********************************************************************
	created:	2008/04/28
	created:	28:4:2008   18:04
	filename: 	d:\loong_client\Client\QuestMgr.cpp
	file path:	d:\loong_client\Client
	file base:	QuestMgr
	file ext:	cpp
	author:		leili
	
	purpose:	任务管理器实现
*********************************************************************/
#include "StdAfx.h"
#include "QuestMgr.h"
#include "QuestMainFrame.h"
#include "QuestTalkFrame.h"
#include "QuestTrackFrame.h"
#include "QuestNPCFrame.h"
#include "NPCTalkFrame.h"
#include "QuestActiveFrame.h"
#include "QuestFilterFrame.h"
#include "CombatSysUtil.h"
#include "MapMgr.h"
#include "CreatureData.h"
#include "ItemProtoData.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "Container.h"
#include "ItemMgr.h"
#include "MapLogicData.h"
#include "SkillProtoData.h"
#include "EffectMgr.h"
#include "QuestHelpFrame.h"
#include "AudioSys.h"
#include "CombatEvent.h"
#include "NPC.h"
#include "ItemEvent.h"
#include "..\WorldDefine\msg_quest_board.h"
#include "QuestPanelFrame.h"
#include "SkillMgr.h"
#include "..\WorldDefine\skill_define.h"
#include "GuildMgr.h"

#define CANCELTRACKQUESTTIME 900.0f	//15*60

QuestMgr::QuestMgr( void )
: m_Trunk(this)
, m_pData(0)
, m_pQuery(0)
, m_pScript(0)
, m_curTrigger(GT_INVALID)
, m_pActiveFrame(0)
, m_pTrackFrame(0)
, m_bLPAttIsOK(false)
, m_lastUpdatetime(0.0f)
, m_pQuestHelpFrame(NULL)
{
	m_curTalkNPC.globalID	= GT_INVALID;
	m_curTalkNPC.typeID		= GT_INVALID;
}

QuestMgr::~QuestMgr( void )
{
	SAFE_DEL(m_pData);
	SAFE_DEL(m_pQuery);
	SAFE_DEL(m_pScript);
}

QuestMgr g_questMgr;
QuestMgr* QuestMgr::Inst( void )
{
	return &g_questMgr;
}

VOID QuestMgr::Destroy( void )
{
	m_pData->Reset();
	m_pQuery->Reset();

	m_NPCFrameList.clear();
	m_pActiveFrame = NULL;
	m_pTrackFrame = NULL;
	m_pQuestHelpFrame = NULL;
	m_bLPAttIsOK = false;

	// 注销网络消息
	TObjRef<NetCmdMgr>()->UnRegister("NS_OpenQuestBoard",					(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::OnNetGetQuest) );
	TObjRef<NetCmdMgr>()->UnRegister("NS_GetRoleInitState_IncompleteQuest", (NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_IncompleteQuest));
	TObjRef<NetCmdMgr>()->UnRegister("NS_GetRoleInitState_CompleteQuest",	(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_GetCompleteQuest));
	TObjRef<NetCmdMgr>()->UnRegister("NS_AcceptQuest",						(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_AcceptQuest));
	TObjRef<NetCmdMgr>()->UnRegister("NS_CompleteQuest",					(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_CompleteQuest));
	TObjRef<NetCmdMgr>()->UnRegister("NS_DeleteQuest",						(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_DeleteQuest));
	TObjRef<NetCmdMgr>()->UnRegister("NS_QuestUpdateNPCTalk",				(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_UpdateQuestNPCTalk));
	TObjRef<NetCmdMgr>()->UnRegister("NS_QuestUpdateTrigger",				(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_UpdateQuestTrigger));
	TObjRef<NetCmdMgr>()->UnRegister("NS_QuestUpdateKillCreature",			(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_UpdateKillCreature));
	TObjRef<NetCmdMgr>()->UnRegister("NS_QuestUpdateItem",					(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_UpdateItem));
	TObjRef<NetCmdMgr>()->UnRegister("NS_QuestUpdateInveset",				(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_UpdateInves));
	TObjRef<NetCmdMgr>()->UnRegister("NS_AddQuest",							(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_AddQuest));
	TObjRef<NetCmdMgr>()->UnRegister("NS_BindRebornMap",					(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_BindRebornMap));
	TObjRef<NetCmdMgr>()->UnRegister("NS_SyncWalk",							(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_SyncWalk));
	TObjRef<NetCmdMgr>()->UnRegister("NS_MsgBlob",							(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_MsgBlob));
	TObjRef<NetCmdMgr>()->UnRegister("NS_QuestUpdateFriends",				(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_UpdateQuestFriends));
	TObjRef<NetCmdMgr>()->UnRegister("NS_QuestUpdateTeammates",				(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_UpdateQuestTeammates));
	TObjRef<NetCmdMgr>()->UnRegister("NS_QuestUpdateLevel",					(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_UpdateQuestLevel) );
	TObjRef<NetCmdMgr>()->UnRegister("NS_RoleSpecQuestCompleteState",		(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_UpdateSpecialQuest) );
	TObjRef<NetCmdMgr>()->UnRegister("NS_ChuanDaoQuestUpdateCount",			(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_ChuanDaoQuestUpdateCount) );

	// 注销GM网络消息
	TObjRef<NetCmdMgr>()->UnRegister("NS_GMQuestState",						(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_GMQuestState));

	TObjRef<NetCmdMgr>()->UnRegister("NS_Skill",						(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::OnNS_Skill));

	// 注销游戏事件
	//TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("CombatMsg_ItemAdd"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_GetSpeItem));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("tagStartNPCTalkEvent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_NPCTalk));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("Open_Task"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_OpenTask));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("Close_Task"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_CloseTask));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("SetRebornEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnSetRebornEvent));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("OpenMap"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LoadNewMap));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("tagInitLPAttEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPAttIsOK));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("tagUpdateLPLevel"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPLevelChanged));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("tagUpdateLPSkillEvent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPSkillChanged));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("ItemEvent_UpdateItem"),(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPItemChanged));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("Bag_Silver"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPMoneyChanged));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("ReputeChange"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPReputeChanged));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("Event_RoleEnterTrigger"),(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_RoleEnterTrigger));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("QuestEvent_UseQuest"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_UseQuest));
	TObjRef<GameFrameMgr>()->UnRegisterEventHandler( _T("tagMouseMoveFinishEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnMouseMoveFinishEvent));
}

VOID QuestMgr::Update( void )
{
	//float curTime = Kernel::Inst()->GetAccumTime();
	//if( curTime-m_lastUpdatetime > 60.0f )//每隔60秒更新任务追踪列表
	//{
	//	CleanUpTrackQuest(curTime);
	//	m_lastUpdatetime = curTime;
	//}
}

//-----------------------------------------------------------------------------
// 取消跟踪15分钟内还没有更新的任务
//-----------------------------------------------------------------------------
VOID QuestMgr::CleanUpTrackQuest( float curTime )
{
	bool bClean = false;
	QuestQuery::QuestsMap::iterator i = m_pQuery->m_currentQuests.begin();
	for( ; i!=m_pQuery->m_currentQuests.end(); ++i )
	{
		QuestQuery::tagQuestTrack& quest = i->second;
		if( quest.isTracked )
		{
			if( curTime-quest.lastUpdateTime > CANCELTRACKQUESTTIME )
			{
				quest.isTracked = false;
				bClean = true;
			}
		}
	}

	if( bClean && P_VALID(m_pTrackFrame) && m_pTrackFrame->IsVisible() )
	{
		m_pTrackFrame->UpdateQuestTrack();
	}
}

BOOL QuestMgr::LoadFromFile()
{
	m_pData = new QuestData();
	m_pQuery = new QuestQuery();
	m_pScript = new QuestScript();

	DWORD  dwTime = timeGetTime();
	CreateObj("QuestContainer", "VarContainer");
	m_pVar = "QuestContainer";

	tstring strPath = _T("data\\system\\quest");
	tstring strFile = _T("data\\system\\quest\\dynamicnpc.xml");
	list<tstring> listField;

	// 加载动态可接任务NPC
	m_pVar->Load("VFS_System", strFile.c_str(), "npc", &listField);
	m_pQuery->BuildDynamicQuestNPCs(listField);

	listField.clear();
	strFile = _T("data\\system\\quest\\quest.xml");
	m_pVar->Load("VFS_System", strFile.c_str(), "name", &listField);

	KillObj("QuestContainer");

	IMSG(_T("--TIME1=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	if( !m_pData->LoadQuestData( strPath.c_str(), &listField ) )
		return FALSE;
	IMSG(_T("--TIME2=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	strPath = vEngine::g_strLocalPath + _T("\\quest");
	if( !m_pData->LoadQuestLocalData( strPath.c_str(), &listField ) )
		return FALSE;
	IMSG(_T("--TIME3=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	// 加载触发器信息
	strPath = vEngine::g_strLocalPath + _T("\\quest\\trigger.xml");
	if( !m_pData->LoadTriggerData(strPath.c_str()) )
		return FALSE;
	IMSG(_T("--TIME4=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	strPath = _T("data\\system\\quest\\accept_quest_npc.data");
	if( !m_pData->LoadNPCAcceptQuestFile( strPath.c_str() ) )
		return FALSE;
	IMSG(_T("--TIME5=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	strPath = _T("data\\system\\quest\\complete_quest_npc.data");
	if( !m_pData->LoadNPCCompleteQuestFile( strPath.c_str() ) )
		return FALSE;
	IMSG(_T("--TIME6=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	strPath = _T("data\\system\\quest\\quest_npc.data");
	if( !m_pData->LoadNPCUpdateQuestFile( strPath.c_str() ) )
		return FALSE;
	IMSG(_T("--TIME7=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	// 加载寻路点地图对应文件，用于寻路
	strPath = _T("data\\system\\quest\\pathfinding.csv");
	if( !m_pData->LoadPathfinding( strPath.c_str() ) )
		return FALSE;
	IMSG(_T("--TIME8=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	// 加载任务大类别文件
	strPath = vEngine::g_strLocalPath + _T("\\quest\\RCategory.xml");
	if (! m_pData->LoadQuestCategory(strPath.c_str()))
		return FALSE;
	IMSG(_T("--TIME9=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	// 加载任务类型文件
	strPath = vEngine::g_strLocalPath + _T("\\quest\\RType.xml");
	if( !m_pData->LoadQuestType( strPath.c_str() ) )
		return FALSE;
	IMSG(_T("--TIME10=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	// 加载任过滤默认配置
	strPath = vEngine::g_strLocalPath + _T("\\quest\\RDefaultCategory.xml");
	if (!m_pData->LoadQuestDefaultFilterConfig(strPath.c_str()))
		return FALSE;
	IMSG(_T("--TIME11=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	// 加载可以使用特殊道具完成的任务配置文件
	strPath = _T("data\\system\\quest\\complete_fuzhu.xml");
	if (!m_pData->LoadQuestCompleteConditionBySpecItem(strPath.c_str()))
		return FALSE;
	IMSG(_T("--TIME12=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	// 加载新手任务
	strPath = _T("data\\system\\quest\\NewGuysQuest.xml");
	if (!m_pData->LoadNewGuysQuest(strPath.c_str()))
		return FALSE;
	IMSG(_T("--TIME12=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	// 加载任务板任务
	strPath = _T("data\\system\\attdata\\Questdata.xml");
	if (!m_pData->LoadQuestPanelFile(strPath.c_str()))
		return FALSE;
	IMSG(_T("--TIME12=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	// 加载任务板任务描述
	strPath = _T("data\\local\\zhCN\\attdata\\QuestTEXT.xml");
	if (!m_pData->LoadQuestPanelTaskDesc(strPath.c_str()))
		return FALSE;
	IMSG(_T("--TIME12=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();

	// 加载物品任务怪物表
	strPath = _T("data\\local\\zhCN\\quest\\PathfindingITEM.xml");
	if (!m_pData->LoadItemMonsterFile(strPath.c_str()))
		return FALSE;
	IMSG(_T("--TIME12=%d\r\n"), timeGetTime()-dwTime);
	dwTime = timeGetTime();
	
	return TRUE;
}

BOOL QuestMgr::Init( void )
{
	m_pUtil = TObjRef<Util>();
	m_pGSMgr = TObjRef<GameScriptMgr>();

	// 注册网络消息
	TObjRef<NetCmdMgr>()->Register("NS_OpenQuestBoard",						(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::OnNetGetQuest),						_T("NS_OpenQuestBoard"));
	TObjRef<NetCmdMgr>()->Register("NS_GetRoleInitState_IncompleteQuest",	(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_IncompleteQuest),	_T("没有完成的任务"));
	TObjRef<NetCmdMgr>()->Register("NS_GetRoleInitState_CompleteQuest",		(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_GetCompleteQuest),	_T("已经完成的任务"));
	TObjRef<NetCmdMgr>()->Register("NS_AcceptQuest",						(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_AcceptQuest),		_T("接受任务"));
	TObjRef<NetCmdMgr>()->Register("NS_CompleteQuest",						(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_CompleteQuest),		_T("完成任务"));
	TObjRef<NetCmdMgr>()->Register("NS_DeleteQuest",						(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_DeleteQuest),		_T("删除任务"));
	TObjRef<NetCmdMgr>()->Register("NS_QuestUpdateNPCTalk",					(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_UpdateQuestNPCTalk),	_T("NPCTalk更新任务"));
	TObjRef<NetCmdMgr>()->Register("NS_QuestUpdateTrigger",					(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_UpdateQuestTrigger),	_T("Trigger更新任务"));
	TObjRef<NetCmdMgr>()->Register("NS_QuestUpdateKillCreature",			(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_UpdateKillCreature),	_T("击杀怪物"));
	TObjRef<NetCmdMgr>()->Register("NS_QuestUpdateItem",					(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_UpdateItem),			_T("获得物品"));
	TObjRef<NetCmdMgr>()->Register("NS_QuestUpdateInveset",					(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_UpdateInves),		_T("调查"));
	TObjRef<NetCmdMgr>()->Register("NS_AddQuest",							(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_AddQuest),			_T("任务物品初始化"));
	TObjRef<NetCmdMgr>()->Register("NS_BindRebornMap",						(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_BindRebornMap),	_T("NS_BindRebornMap"));
	TObjRef<NetCmdMgr>()->Register("NS_SyncWalk",							(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_SyncWalk),			_T("NS_SyncWalk"));
	TObjRef<NetCmdMgr>()->Register("NS_MsgBlob",							(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_MsgBlob),			_T("NS_MsgBlob"));
	TObjRef<NetCmdMgr>()->Register("NS_QuestUpdateFriends",					(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_UpdateQuestFriends), _T("NS_QuestUpdateFriends") );
	TObjRef<NetCmdMgr>()->Register("NS_QuestUpdateTeammates",				(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_UpdateQuestTeammates), _T("NS_QuestUpdateTeammates") );
	TObjRef<NetCmdMgr>()->Register("NS_QuestUpdateLevel",					(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_UpdateQuestLevel), _T("NS_QuestUpdateLevel") );
	TObjRef<NetCmdMgr>()->Register("NS_RoleSpecQuestCompleteState",			(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_UpdateSpecialQuest),	_T("NS_RoleSpecQuestCompleteState") );
	TObjRef<NetCmdMgr>()->Register("NS_ChuanDaoQuestUpdateCount",			(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_ChuanDaoQuestUpdateCount), _T("NS_ChuanDaoQuestUpdateCount") );
	TObjRef<NetCmdMgr>()->Register("NS_Skill",						(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::OnNS_Skill),				 _T("NS_Skill"));


	// 注册GM网络消息
	TObjRef<NetCmdMgr>()->Register("NS_GMQuestState",						(NETMSGPROC)m_Trunk.sfp2(&QuestMgr::NetRecvMessage_NS_GMQuestState),	_T("GM命令"));

	// 注册游戏事件
	//TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("CombatMsg_ItemAdd"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_GetSpeItem));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("tagStartNPCTalkEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_NPCTalk));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("Open_Task"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_OpenTask));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("Close_Task"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_CloseTask));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("SetRebornEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnSetRebornEvent));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("OpenMap"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LoadNewMap));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("tagInitLPAttEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPAttIsOK));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("tagUpdateLPLevel"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPLevelChanged));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("tagUpdateLPSkillEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPSkillChanged));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("ItemEvent_UpdateItem"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPItemChanged));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("Bag_Silver"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPMoneyChanged));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("ReputeChange"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_LPReputeChanged));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("Event_RoleEnterTrigger"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_RoleEnterTrigger));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("QuestEvent_UseQuest"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnEvent_UseQuest));
	TObjRef<GameFrameMgr>()->RegisterEventHandle( _T("tagMouseMoveFinishEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&QuestMgr::OnMouseMoveFinishEvent));

	m_lastUpdatetime = Kernel::Inst()->GetAccumTime();
	
	// 任务过滤默认打开
	GetData()->SetFilterByPlayerLevel(FALSE);
	//GetData()->SetCurFilter(QuestData::EF_CustomFilter1);
	
	TObjRef<GameFrameMgr>()->CreateFrame(_T("World"), _T("QuestFilter"), _T("QuestFilterFrame"), 0);

	return TRUE;
}

// 接受任务时是否需要帮助

VOID QuestMgr::HandleAcceptQuestHelp( UINT16 questID )
{
	if( !P_VALID(m_pQuestHelpFrame) )
	{
		m_pQuestHelpFrame = (QuestHelpFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("QuestHelp"), _T("QuestHelpFrame"), 0 );
		ASSERT( P_VALID(m_pQuestHelpFrame) );
		m_pQuestHelpFrame->HideFrame();
	}
	if( m_pQuestHelpFrame->GetShowFrameFlag() )
	{
		const vector<tagQuestHelpInfo> vecTemp = m_pQuestHelpFrame->GetQuestHelpInfo();
		for( size_t i = 0; i < vecTemp.size(); i++ )
		{
			if( questID == vecTemp[i].dwQuestId )
			{
				if( vecTemp[i].dwStartPicPage != GT_INVALID )
				{
					m_pQuestHelpFrame->SetPicName( vecTemp[i].szStartPicName );
					m_pQuestHelpFrame->SetCurPage( vecTemp[i].dwStartPicPage );
					m_pQuestHelpFrame->ShowFrame();
				}
				else
					CloseQuestHelpFrame();
				break;
			}
		}
	}
	else
	{
		CloseQuestHelpFrame();
	}
	
}

// 完成任务时是否需要帮助

VOID QuestMgr::HandleCompleteQuestHelp( UINT16 questID )
{
	if( !P_VALID(m_pQuestHelpFrame) )
	{
		m_pQuestHelpFrame = (QuestHelpFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("QuestHelp"), _T("QuestHelpFrame"), 0 );
		ASSERT( P_VALID(m_pQuestHelpFrame) );
		m_pQuestHelpFrame->HideFrame();
	}
	if( m_pQuestHelpFrame->GetShowFrameFlag() )
	{
		const vector<tagQuestHelpInfo> vecTemp = m_pQuestHelpFrame->GetQuestHelpInfo();
		for( size_t i = 0; i < vecTemp.size(); i++ )
		{
			if( questID == vecTemp[i].dwQuestId )
			{
				if( vecTemp[i].dwFinishPicPage != GT_INVALID )
				{
					m_pQuestHelpFrame->SetPicName( vecTemp[i].szFinishPicName );
					m_pQuestHelpFrame->SetCurPage( vecTemp[i].dwFinishPicPage );
					m_pQuestHelpFrame->ShowFrame();
				}
				else
					CloseQuestHelpFrame();
				break;
			}
		}
	}
	else
	{
		CloseQuestHelpFrame();
	}
}

//-----------------------------------------------------------------------------
// 接受某任务
//-----------------------------------------------------------------------------
VOID QuestMgr::AcceptQuest( UINT16 questID )
{
	HandleAcceptQuestHelp(questID);
	QuestQuery::tagQuestTrack track;
	m_pQuery->m_currentQuests.insert( QuestQuery::Quest_Pair(questID,track) );

	INT16 questType;
	m_pData->GetQuestType( questID, questType );
	QuestMainFrame *pMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestMain"));
	pMainFrame->AddQuest(questID,true);

	m_pQuery->AddInvesObject(questID);
	m_pQuery->AddTriggerEffect(questID);

	UpdateQuest();

	// 释放任务对话窗口
	QuestTalkFrame *pQuestTalkFrame = (QuestTalkFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTalk"));
	if( P_VALID(pQuestTalkFrame) )
		TObjRef<GameFrameMgr>()->AddToDestroyList(pQuestTalkFrame);
}

VOID QuestMgr::QuickComQuest(UINT16 questID)
{
	if( m_pQuery->QuestCanCompleteBySpecItem(questID) )
	{
		LocalPlayer *pPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if(P_VALID(pPlayer))
		{
			const QuestData::tagQuestCompleteConditionBySpecItem *pItem = m_pData->GetQuestItemConditionSpecItem(questID);
			if( P_VALID(pItem) )
			{
				INT nLevel = pPlayer->GetRoleLevel();
				int nNum = 0;
				if( nLevel <= 30 )
					nNum = 1;
				else if( nLevel <= 45 )
					nNum = 2;
				else if( nLevel <= 55 )
					nNum = 3;
				else if( nLevel <= 70 )
					nNum = 4;
				else if( nLevel <= 80 )
					nNum = 5;
				else if( nLevel <= 90 )
					nNum = 6;
				else
					nNum = 7;
				TCHAR szTemp[X_LONG_NAME] = {0};
				_sntprintf( szTemp, X_SHORT_NAME, _T("Grade_%d"), nNum);
				TCHAR szFinal[X_HUGE_NAME] = {0};
				_sntprintf( szFinal, X_HUGE_NAME, g_StrTable[_T("Quest_CancompleteBySpecialItem")], pItem->itemnum, g_StrTable[szTemp] );
				CombatSysUtil::Inst()->ShowScreenCenterMsg( szFinal );
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 完成某任务
//-----------------------------------------------------------------------------
VOID QuestMgr::CompleteQuest( UINT16 questID )
{
	m_pData->m_questDynamicTarget.erase(questID);
	HandleCompleteQuestHelp(questID);
	QuestQuery::DoneQuestsMap::iterator questFinder = m_pQuery->m_doneQuests.find( questID );
	if( questFinder == m_pQuery->m_doneQuests.end() )
	{
		m_pQuery->m_doneQuests.insert( std::pair<UINT16,INT>(questID,1) );
	}
	else
	{
		++questFinder->second;
	}
	m_pQuery->m_doneTimes[questID] = ServerTime::Inst()->CalCurrentServerDwordTime();

	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find(questID);
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		m_pQuery->DeleteInvesObject(questID);
		m_pQuery->DeleteTriggerEffect(questID);

		QuestQuery::tagQuestTrack track = finder->second;
		m_pQuery->m_currentQuests.erase(finder);
		if( track.isTracked )// 更新任务追踪窗口
		{
			QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
			if( P_VALID(pFrame) )
			{
				pFrame->UpdateQuestTrack();
			}
		}
	}

	// 更新任务主界面
	QuestMainFrame *pFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestMain"));
	pFrame->DeleteQuest(questID);

	UpdateQuest();

	// 更新任务NPC界面
	QuestNPCFrame *pQuestNPCMainFrame = (QuestNPCFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestNPCQuestMain"));
	if( P_VALID(pQuestNPCMainFrame) )
	{
		pQuestNPCMainFrame->UpdateNPCList();
	}

	// 释放任务对话窗口
	QuestTalkFrame *pQuestTalkFrame = (QuestTalkFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTalk"));
	if( P_VALID(pQuestTalkFrame) )
		TObjRef<GameFrameMgr>()->AddToDestroyList(pQuestTalkFrame);
}

//-----------------------------------------------------------------------------
// 删除某任务
//-----------------------------------------------------------------------------
VOID QuestMgr::DeleteQuest( UINT16 questID )
{
	// 清除被删除任务状况
	QuestData::QuestStateMap::iterator iterState = m_pData->m_questStates.find( questID );
	QuestData::tagQuestState& state = iterState->second;
	memset(&state,0,sizeof(QuestData::tagQuestState));

	// 删除动态数据
	m_pData->m_questDynamicTarget.erase(questID);

	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find(questID);
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		m_pQuery->DeleteInvesObject(questID);
		m_pQuery->DeleteTriggerEffect(questID);

		QuestQuery::tagQuestTrack track = finder->second;
		m_pQuery->m_currentQuests.erase(finder);
		if( track.isTracked )// 更新任务追踪窗口
		{
			QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
			if( P_VALID(pFrame) )
			{
				pFrame->UpdateQuestTrack();
			}
		}

		// 更新任务主界面
		QuestMainFrame *pFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestMain"));
		pFrame->DeleteQuest(questID);

		UpdateQuest();

		// 更新任务NPC界面
		QuestNPCFrame *pQuestNPCMainFrame = (QuestNPCFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestNPCQuestMain"));
		if( P_VALID(pQuestNPCMainFrame) )
		{
			pQuestNPCMainFrame->UpdateNPCList();
		}
	}
}

//-----------------------------------------------------------------------------
// 发送接收任务消息
//-----------------------------------------------------------------------------
VOID QuestMgr::SendAcceptQuest( UINT16 questID )
{
	// 判断背包是否有空位放置任务物品
	INT blank = 0;
	ItemContainer* pItemContainer = ItemMgr::Inst()->GetQuest();
	pItemContainer->IsFull(blank);
	if( blank < m_pData->GetQuestSrcItem(questID) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("QuestAcceptFailed_BagFull")] );
		return;
	}

	tagNC_NPCAcceptQuest msg;
	msg.dwNPCID		= m_curTalkNPC.globalID;
	msg.u16QuestID	= questID;
	TObjRef<NetSession>()->Send(&msg);
}

//-----------------------------------------------------------------------------
// 发送接收任务消息
//-----------------------------------------------------------------------------
VOID QuestMgr::SendAcceptTriggerQuest( UINT16 questID )
{
	tagNC_TriggerAcceptQuest msg;
	msg.u16QuestID	= questID;
	msg.dwTriggerID	= m_curTrigger;
	TObjRef<NetSession>()->Send(&msg);
}

//-----------------------------------------------------------------------------
// 发送完成任务消息
//-----------------------------------------------------------------------------
VOID QuestMgr::SendCompleteQuest( UINT16 questID,int index, EQuestComplteType eType )
{
	// 判断背包是否有空位放置任务奖励物品
	INT blank = 0;
	ItemContainer* pItemContainer = ItemMgr::Inst()->GetPocket();
	pItemContainer->IsFull(blank);
	if( blank < m_pData->GetQuestRewItemNum(questID) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("QuestCompleteFailed_BagFull")] );
		return;
	}

	tagNC_CompleteQuest msg;
	const tagQuestProto *pQuest = m_pData->GetQuestProto(questID);
	if(!P_VALID(pQuest))
		return;
	if( pQuest->complete_quest_npc[0] == 0 )
		msg.dwNPCID	= GT_INVALID;
	else
		msg.dwNPCID	= m_curTalkNPC.globalID;
	msg.u16QuestID	= questID;
	msg.nRewChoicesItemIndex = index;
	msg.eType = eType;
	TObjRef<NetSession>()->Send(&msg);
}

//-----------------------------------------------------------------------------
// 发送删除任务消息
//-----------------------------------------------------------------------------
VOID QuestMgr::SendDeleteQuest( UINT16 questID )
{
	tagNC_DeleteQuest msg;
	msg.u16QuestID	= questID;
	TObjRef<NetSession>()->Send(&msg);
}

//-----------------------------------------------------------------------------
// 发送更新任务消息
//-----------------------------------------------------------------------------
VOID QuestMgr::SendUpdateQuest( UINT16 questID )
{
	tagNC_UpdateQuestNPCTalk msg;
	msg.dwNPCID		= m_curTalkNPC.globalID;
	msg.u16QuestID	= questID;
	TObjRef<NetSession>()->Send(&msg);
}

//-----------------------------------------------------------------------------
// 与NPC进行对话
//-----------------------------------------------------------------------------
VOID QuestMgr::OnNPCTalk( DWORD globalID, DWORD typeID )
{
	if( GT_INVALID != globalID && GT_INVALID != typeID )
	{
		// 关闭所有的对话窗口
		CloseAllQuestWnd();

		m_curTalkNPC.globalID	= globalID;
		m_curTalkNPC.typeID		= typeID;
		m_pScript->OnTalk( typeID );
	}
}

//-----------------------------------------------------------------------------
// 关闭所有的对话窗口
//-----------------------------------------------------------------------------
VOID QuestMgr::CloseAllQuestWnd( void )
{
	TObjRef<GameFrameMgr>()->SendEvent( &tagGameEvent( _T("Close_Task_UI"), NULL ) );

	// 关闭NPC对话窗口
	NPCTalkFrame *pNPCTalkFrame = (NPCTalkFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("NPCTalk"));
	if( P_VALID(pNPCTalkFrame) )
	{
		pNPCTalkFrame->Hide();
	}

	// 关闭任务对话窗口
	//QuestTalkFrame *pQuestTalkFrame = (QuestTalkFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTalk"));
	//if( P_VALID(pQuestTalkFrame) )
	//	TObjRef<GameFrameMgr>()->AddToDestroyList(pQuestTalkFrame);
	
	//关闭商店
	tagGameEvent event1(_T("Close_Shop"), NULL);
	TObjRef<GameFrameMgr>()->SendEvent(&event1);
	//关闭仓库
// 	tagGameEvent event2(_T("Close_Ware"), NULL);
// 	TObjRef<GameFrameMgr>()->SendEvent(&event2);
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_GetRoleInitState_IncompleteQuest
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_IncompleteQuest( tagNS_GetRoleInitState_IncompleteQuest* pMsg, DWORD pParam )
{
	m_pQuery->m_currentQuests.clear();
	QuestMainFrame *pMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestMain"));
	ASSERT( P_VALID(pMainFrame) );

	BYTE *pContent = (BYTE*)&pMsg->incompleteQuest[0];

	for(INT i=0; i<pMsg->nNum; i++)
	{
		// 任务ID
		UINT16 questID = *(UINT16 *)pContent;
		pContent += sizeof(UINT16);

		const tagQuestProto* pQuestProto = m_pData->GetQuestProto(questID);
		if( !P_VALID(pQuestProto) )
		{
			IMSG(_T("Invalid Quest ID:%d"),questID);
			return 0;
		}

		// 接受任务时间
		pContent += sizeof(DWORD);

		// 杀怪数量
		QuestQuery::tagQuestTrack track;
		for( int creaturei=0; creaturei<QUEST_CREATURES_COUNT; creaturei++ )
		{
			track.creatureCount[creaturei] = *(INT16*)pContent;
			pContent += sizeof(INT16);
		}

		// 物品数量
		for( int itemi=0; itemi<QUEST_ITEMS_COUNT; itemi++ )
		{
			track.itemCount[itemi] = *(INT16*)pContent;
			pContent += sizeof(INT16);
		}

		// 特殊任务状态
		track.bSpecialDone = *(BOOL*)pContent;
		pContent += sizeof(BOOL);

		// 传道杀怪次数
		track.nKillTimes = *(INT16*)pContent;
		pContent += sizeof(INT16);

		m_pQuery->m_currentQuests.insert( QuestQuery::Quest_Pair(questID,track) );

		// 动态目标
		if( pQuestProto->target_mode == 1 )//动态
		{
			m_pData->m_questDynamicTarget.erase(questID);
			m_pData->m_questDynamicTarget.insert( make_pair( questID, *(tagQuestDynamicTarget*)pContent ) );

			tagQuestDynamicTarget *pTarget = (tagQuestDynamicTarget*)pContent;
			if( pTarget->dwQuestTipsID > 0)
			{
				LPCTSTR pQuestDesc = TObjRef<GameScriptMgr>()->GetQuestDesc(pTarget->dwQuestTipsID);
				if(P_VALID(pQuestDesc)) 	m_pData->SetQuestDesc( questID,pQuestDesc );
			}

			pContent += sizeof(tagQuestDynamicTarget);
		}

		m_pQuery->AddInvesObject(questID);
		m_pQuery->UpdateInvesObject(questID);
		m_pQuery->UpdateTriggerEffect(questID);
	}

	tagNC_GetRoleInitState msg;
	msg.eType = ERIT_Att;
	TObjRef<NetSession>()->Send(&msg);

	// 发送游戏事件让onlinetips刷新
	TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("Refresh_Onlinetips"), NULL ) );

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_GetRoleInitState_CompleteQuest
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_GetCompleteQuest( tagNS_GetRoleInitState_CompleteQuest* pMsg, DWORD pParam )
{
	for(INT i = 0; i< pMsg->nNum; i++)
	{
		m_pQuery->m_doneQuests.insert(std::pair<UINT16,INT>(pMsg->completeQuest[i].u16QuestID,pMsg->completeQuest[i].nTimes));
		m_pQuery->m_doneTimes[pMsg->completeQuest[i].u16QuestID] = pMsg->completeQuest[i].dwStartTime;
	}

	tagNC_GetRoleInitState msg;
	msg.eType = ERIT_IncompleteQuest;
	TObjRef<NetSession>()->Send(&msg);

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_AcceptQuest
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_AcceptQuest( tagNS_AcceptQuest* pMsg, DWORD pParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			tstring QuestName;
			m_pData->GetQuestName( pMsg->u16QuestID, QuestName );
			CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("Quest_AcceptSucceed")], QuestName.c_str() );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_accept"));
			OnNPCTalk( m_curTalkNPC.globalID, m_curTalkNPC.typeID );
			NPCTalkFrame *pFrame = (NPCTalkFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("NPCTalk") );
			if( P_VALID(pFrame) )
			{
				INT nNum = pFrame->GetTaskNum();
				if( nNum <= 0 )
				{
					pFrame->Hide();
					TObjRef<GameFrameMgr>()->AddToDestroyList(pFrame);
				}
			}
			if(pMsg->u16QuestID >= QUEST_PANEL_START_TASK_ID && 
				pMsg->u16QuestID <= QUEST_PANEL_END_TASK_ID )
			{
				QuestPanelFrame *pFrame = (QuestPanelFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestPanel"));
				if( P_VALID(pFrame) )
				{
					pFrame->UpdateQuestList();
				}
			}
		}
		break;
	case E_QUEST_NOT_EXIST:		//任务不存在
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestFailed_QuestNotExist")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_QUEST_NPC_NOT_EXIST:		//任务NPC不存在
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestFailed_NPCNotExist")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_QUEST_NPC_TOO_FAR:		//任务NPC与玩家距离太远
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestFailed_TooFar")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_LOW_LEVEL:		//没有足够的等级
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_LowLevel")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_LOW_REP:		//没有足够的声望
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_LowRep")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_ALREADY_DONE:		//已经完成了
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_AlreadyDone")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_ALREADY_TAKE:		//已经接取了
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_AlreadyTake")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_MISSING_ITEMS:		//没有任务所需要的物品
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_MissItem")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_MISSING_SKILLS:		//没有任务所需要的技能
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_MissSkill")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_MISSING_Att:		//没有任务所需要的人物属性
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_MissAtt")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_WRONG_SEX:       //任务所需要的性别
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_WrongSex")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_WRONG_CLASS:       //任务所需要的职业
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_WrongClass")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_NOT_ENOUGH_MONEY:       //没有任务所需要的金钱
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_NotEnoughMoney")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_QUEST_NUM_FULL:		//任务满了，不能再接
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_QuestFull")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_WRONG_MAP: 		//地图不对
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_PREV:      	//任务前序不对
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_Prev")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_WRONG_TIME:       //任务时间不对
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_WrongTime")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_WRONG_TIMES:       //任务接取次数不对
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_WrongTimes")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_MISS_ITEM_OF_QUEST:       //没有任务物品
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_MissQuestItem")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_WRONG_TRIGGER:      //任务触发器不对
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_WrongTrigger")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_BAG_FULL:		//背包没有地方，放置任务物品
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_BagFull")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_Tomorrow:		//每天只能完成一次
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanTakeQuest_Tomorrow")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_Week:		//只能在星期×完成
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_WrongTime")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_Class:		//职业不符合
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_WrongClass")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanAcceptQuest_TenTimesEachDay:		// 天之行会任务榜每天只能接取10次
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_SexLimit:		//性别不符合
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("QuestAcceptFailed_WrongSex")]);
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_FAILED_SCRIPT:
		{
		
		}
		break;
// 	default: //应策划要求，不显示默认提示了 chg-by-lib
// 		{
// 			CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("QuestFailed_None")] );
// 		}
// 		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_CompleteQuest
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_CompleteQuest( tagNS_CompleteQuest* pMsg, DWORD pParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			tstring QuestName;
			m_pData->GetQuestName( pMsg->u16QuestID, QuestName );
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("Quest_CompleteSucceed")], QuestName.c_str() );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_complete"));
			EffectMgr::Inst()->PlayRoleEffect( RoleMgr::Inst()->GetLocalPlayerID(), _T("txxy02") );
			CompleteQuest(pMsg->u16QuestID);
			OnNPCTalk( m_curTalkNPC.globalID, m_curTalkNPC.typeID );
			NPCTalkFrame *pFrame = (NPCTalkFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("NPCTalk") );
			if( P_VALID(pFrame) )
			{
				INT nNum = pFrame->GetTaskNum();
				if( nNum <= 0  || RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID() ) )
					TObjRef<GameFrameMgr>()->AddToDestroyList(pFrame);
			}
		}
		break;
	case E_QUEST_NPC_TOO_FAR:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("QuestFailed_TooFar")] );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_QUEST_NOT_EXIST:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("QuestFailed_QuestNotExist")] );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanCompleteQuest_FAILED_MISSING_Creature:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("QuestCompleteFailed_MissCreature")] );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanCompleteQuest_FAILED_MISSING_NPC:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("QuestCompleteFailed_MissNPC")] );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanCompleteQuest_FAILED_MISSING_ITEMS:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("QuestCompleteFailed_MissItem")] );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanCompleteQuest_FAILEDENOUGH_MONEY:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("QuestCompleteFailed_NotEnoughMoney")] );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanCompleteQuest_BAG_FULL:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("QuestCompleteFailed_BagFull")] );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanAcceptQuest_TenTimesEachDay:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("QuestCompleteFailed_TenTimesEachDay")] );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	case E_CanTakeQuest_SexLimit:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("QuestCompleteFailed_TenTimesEachDay")] );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	default:
		{
			IMSG(_T("quest failed: %d"),pMsg->dwErrorCode);
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("QuestFailed_None")] );
			TObjRef<AudioSys>()->Play2DSound(_T("quest_failed"));
		}
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_DeleteQuest
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_DeleteQuest( tagNS_DeleteQuest* pMsg, DWORD pParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			DeleteQuest( pMsg->u16QuestID );
			tstring QuestName;
			m_pData->GetQuestName( pMsg->u16QuestID, QuestName );
			CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("Quest_DeleteSucceed")], QuestName.c_str() );
			QuestPanelFrame *pFrame = (QuestPanelFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestPanel"));
			if( P_VALID(pFrame) && 
				pMsg->u16QuestID >= QUEST_PANEL_START_TASK_ID && 
				pMsg->u16QuestID <= QUEST_PANEL_END_TASK_ID )
			{
				pFrame->UpdateQuestList();
			}
		}
		break;

	default:
		{
			tstring QuestName;
			m_pData->GetQuestName( pMsg->u16QuestID, QuestName );
			CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("Quest_DeleteFailed")], QuestName.c_str() );
		}
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_UpdateQuestNPCTalk
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_UpdateQuestNPCTalk( tagNS_QuestUpdateNPCTalk* pMsg, DWORD pParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			QuestData::QuestStateMap::iterator find = m_pData->m_questStates.find( pMsg->u16QuestID );
			if( find != m_pData->m_questStates.end() )
			{
				QuestData::tagQuestState& state = find->second;
				state.beTalked[pMsg->nIndex] = TRUE;

				// 重建更新NPC列表
				m_pQuery->BuildUpdateQuestNPCs();

				// 如果任务完成，更新任务名称为[完成]
				if( m_pQuery->UpdateQuestState( pMsg->u16QuestID ) )
				{
					QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
					ASSERT( P_VALID(pQuestMainFrame) );
					
					pQuestMainFrame->SetQuestCompleteFlag( pMsg->u16QuestID );
				}

				// 任务追踪
				QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find(pMsg->u16QuestID);
				if( finder != m_pQuery->m_currentQuests.end() )
				{
					QuestQuery::tagQuestTrack& track = finder->second;
					track.isTracked = true;
					track.lastUpdateTime = Kernel::Inst()->GetAccumTime();

					QuestTrackFrame *pQuestTrackFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
					if( P_VALID(pQuestTrackFrame) )
					{
						pQuestTrackFrame->UpdateQuestTrack();
					}
				}

				// 释放NPCTalk窗口
				NPCTalkFrame *pNpcTalkFrame = (NPCTalkFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("NPCTalk"));
				if( P_VALID(pNpcTalkFrame) )
				{
					TObjRef<GameFrameMgr>()->AddToDestroyList(pNpcTalkFrame);
				}

				// 屏显信息 任务名称 与NPC名称对话 完成！
				tstring QuestName;
				m_pData->GetQuestName( pMsg->u16QuestID, QuestName );
				DWORD npcID = m_pData->GetCompleteReqNPC( pMsg->u16QuestID, pMsg->nIndex );
				const tagCreatureProto *creatureProto = CreatureData::Inst()->FindNpcAtt( npcID );

				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("Quest_UpdateNPCTalk")], QuestName.c_str(), creatureProto->szName );

				QuestPanelFrame *pFrame = (QuestPanelFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestPanel"));
				if( P_VALID(pFrame) && 
					pMsg->u16QuestID >= QUEST_PANEL_START_TASK_ID && 
					pMsg->u16QuestID <= QUEST_PANEL_END_TASK_ID )
				{
					pFrame->UpdateQuestList();
				}
			}
		}
		break;

	case E_CanUpdateQuest_Fail:
		break;

	default:
		// 未知消息错误
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_UpdateQuestTrigger
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_UpdateQuestTrigger( tagNS_QuestUpdateTrigger* pMsg, DWORD pParam )
{
	QuestData::QuestStateMap::iterator find = m_pData->m_questStates.find( pMsg->u16QuestID );
	if( find != m_pData->m_questStates.end() )
	{
		QuestData::tagQuestState& state = find->second;
		state.beTriggered[pMsg->nIndex] = TRUE;

		m_pQuery->UpdateTriggerEffect(pMsg->u16QuestID);

		// 如果任务完成，更新任务名称为[完成]
		if( m_pQuery->UpdateQuestState( pMsg->u16QuestID ) )
		{
			QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
			ASSERT( P_VALID(pQuestMainFrame) );

			pQuestMainFrame->SetQuestCompleteFlag( pMsg->u16QuestID );
		}
	}

	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find( pMsg->u16QuestID );
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		QuestQuery::tagQuestTrack& track = finder->second;
		track.isTracked = true;
		track.lastUpdateTime = Kernel::Inst()->GetAccumTime();
		QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
		if( P_VALID(pFrame) )
		{
			pFrame->UpdateQuestTrack();
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_QuestUpdateKillCreature
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_UpdateKillCreature( tagNS_QuestUpdateKillCreature* pMsg, DWORD pParam )
{
	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find( pMsg->u16QuestID );
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		QuestQuery::tagQuestTrack& track = finder->second;
		if( m_pQuery->SetQuestCreatureCount( pMsg->u16QuestID, pMsg->nCreatureIndex, pMsg->n16NewKillNum ) )
		{
			track.isTracked = true;
			track.lastUpdateTime = Kernel::Inst()->GetAccumTime();
			QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
			if( P_VALID(pFrame) )
			{
				pFrame->UpdateQuestTrack();
			}

			// 屏显，显示击杀怪物，任务名称 击杀 怪物名称 1/5
			tstring questName;
			DWORD id=0;
			UINT16 count=0;
			m_pData->GetQuestName( pMsg->u16QuestID, questName );
			m_pData->GetQuestRequiredCreatureCount( pMsg->u16QuestID,pMsg->nCreatureIndex,id,count );
			const tagCreatureProto* creatureProto = CreatureData::Inst()->FindNpcAtt(id);
			if( P_VALID(creatureProto) )
			{
				if( pMsg->n16NewKillNum > count )
				{
					pMsg->n16NewKillNum = count;
				}

				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("Quest_UpdateKillCreature")], 
					questName.c_str(), creatureProto->szName, pMsg->n16NewKillNum, count );
			}
		}

		QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
		ASSERT( P_VALID(pQuestMainFrame) );
		pQuestMainFrame->UpdateQuestDesc(pMsg->u16QuestID);

		// 如果任务完成，更新任务名称为[完成]
		if( m_pQuery->UpdateQuestState( pMsg->u16QuestID ) )
		{
			pQuestMainFrame->SetQuestCompleteFlag( pMsg->u16QuestID );
		}

		QuestPanelFrame *pFrame = (QuestPanelFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestPanel"));
		if( P_VALID(pFrame) && 
			pMsg->u16QuestID >= QUEST_PANEL_START_TASK_ID && 
			pMsg->u16QuestID <= QUEST_PANEL_END_TASK_ID )
		{
			pFrame->UpdateQuestList();
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_QuestUpdateItem
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_UpdateItem( tagNS_QuestUpdateItem* pMsg, DWORD pParam )
{
	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find( pMsg->u16QuestID );
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		QuestQuery::tagQuestTrack& track = finder->second;
		if( m_pQuery->SetQuestItemCount( pMsg->u16QuestID, pMsg->nItemIndex, pMsg->n16NewItemNum ) )
		{
			track.isTracked = true;
			track.lastUpdateTime = Kernel::Inst()->GetAccumTime();
			QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
			if( P_VALID(pFrame) )
			{
				pFrame->UpdateQuestTrack();
			}

			// 屏显，显示获得物品，任务名称 获得 物品名称 1/5
			tstring questName;
			DWORD id=0;
			UINT16 count=0;
			m_pData->GetQuestName( pMsg->u16QuestID, questName );
			m_pData->GetQuestRequiredItemCount( pMsg->u16QuestID,pMsg->nItemIndex,id,count );
			const tagItemDisplayInfo* itemProto = ItemProtoData::Inst()->FindItemDisplay(id);
			if( P_VALID(itemProto) )
			{
				if( pMsg->n16NewItemNum > count )
				{
					pMsg->n16NewItemNum = count;
				}

				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("Quest_UpdateItem")], 
					questName.c_str(), itemProto->szName, pMsg->n16NewItemNum, count );
			}

			m_pQuery->UpdateInvesObject(pMsg->u16QuestID);
		}

		QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
		ASSERT( P_VALID(pQuestMainFrame) );
		pQuestMainFrame->UpdateQuestDesc(pMsg->u16QuestID);

		// 如果任务完成，更新任务名称为[完成]
		if( m_pQuery->UpdateQuestState( pMsg->u16QuestID ) )
		{
			pQuestMainFrame->SetQuestCompleteFlag( pMsg->u16QuestID );
		}

		QuestPanelFrame *pFrame = (QuestPanelFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestPanel"));
		if( P_VALID(pFrame) && 
			pMsg->u16QuestID >= QUEST_PANEL_START_TASK_ID && 
			pMsg->u16QuestID <= QUEST_PANEL_END_TASK_ID )
		{
			pFrame->UpdateQuestList();
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_QuestUpdateInveset
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_UpdateInves( tagNS_QuestUpdateInveset* pMsg, DWORD pParam )
{
	const tagQuestDynamicTarget *pDynamicTarget = m_pData->GetQuestDynamicTarget(pMsg->u16QuestID);
	if( P_VALID(pDynamicTarget) && pDynamicTarget->eTargetType == EQTT_Invest )//动态
	{
		QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find( pMsg->u16QuestID );
		if( finder != m_pQuery->m_currentQuests.end() )
		{
			QuestQuery::tagQuestTrack& track = finder->second;
			if( m_pQuery->SetQuestInvesObj( pMsg->u16QuestID, pMsg->nIndex ) )
			{
				track.isTracked = true;
				track.lastUpdateTime = Kernel::Inst()->GetAccumTime();
				QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
				if( P_VALID(pFrame) )
				{
					pFrame->UpdateQuestTrack();
				}

				// 屏显，显示调查地物，任务名称 调查 物品名称 完毕
				//CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("Quest_UpdateItem")], 
				//	questName.c_str(), itemProto->szName, pMsg->n16NewItemNum, count );

				m_pQuery->DeleteInvesObject(pMsg->u16QuestID,pMsg->nIndex);
			}

			QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
			ASSERT( P_VALID(pQuestMainFrame) );
			pQuestMainFrame->UpdateQuestDesc(pMsg->u16QuestID);

			// 如果任务完成，更新任务名称为[完成]
			if( m_pQuery->UpdateQuestState( pMsg->u16QuestID ) )
			{
				pQuestMainFrame->SetQuestCompleteFlag( pMsg->u16QuestID );
			}

			QuestPanelFrame *pFrame = (QuestPanelFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestPanel"));
			if( P_VALID(pFrame) && 
				pMsg->u16QuestID >= QUEST_PANEL_START_TASK_ID && 
				pMsg->u16QuestID <= QUEST_PANEL_END_TASK_ID )
			{
				pFrame->UpdateQuestList();
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_AddQuest
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_AddQuest( tagNS_AddQuest* pMsg, DWORD pParam )
{
	// 动态目标
	const tagQuestProto* pQuestProto = m_pData->GetQuestProto( pMsg->u16QuestID );
	if( !P_VALID(pQuestProto) )
	{
		IMSG(_T("Invalid Quest ID:%d"),pMsg->u16QuestID);
		return 0;
	}

	if( pQuestProto->target_mode == 1 )//动态
	{
		m_pData->m_questDynamicTarget.erase(pMsg->u16QuestID);
		m_pData->m_questDynamicTarget.insert( make_pair( pMsg->u16QuestID, pMsg->DynamicTarget ) );
	}

	QuestData::QuestStateMap::iterator iterState = m_pData->m_questStates.find( pMsg->u16QuestID );
	QuestData::tagQuestState& state = iterState->second;
	memset(&state,0,sizeof(QuestData::tagQuestState));

	AcceptQuest(pMsg->u16QuestID);
	QuickComQuest(pMsg->u16QuestID);

	QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );

	// 是否立即显示任务界面
	if( P_VALID(pQuestMainFrame) && m_pData->ShowQuestAtOnce(pMsg->u16QuestID) )
	{
		TObjRef<GameFrameMgr>()->SendEvent( &tagGameEvent( _T("Open_Task_UI"), NULL ) );
	}

	if( TrackQuest(pMsg->u16QuestID,false) )
	{
		if( P_VALID(pQuestMainFrame) && !m_pQuery->QuestCanCompleteBySpecItem(pMsg->u16QuestID) )
		{
			if(m_pQuery->QuestCanComplete(pMsg->u16QuestID))
				pQuestMainFrame->SetQuestCompleteFlag( pMsg->u16QuestID );
			else
				pQuestMainFrame->SetQuestTrackFlag( pMsg->u16QuestID );
		}
	}

	tagNS_QuestUpdateItem itemMsg;
	itemMsg.u16QuestID = pMsg->u16QuestID;
	for( int i=0; i<QUEST_ITEMS_COUNT; i++ )
	{
		if( pMsg->n16ItemCount[i] == 0 )
			continue;

		itemMsg.nItemIndex = i;
		itemMsg.n16NewItemNum = pMsg->n16ItemCount[i];
		NetRecvMessage_UpdateItem(&itemMsg,NULL);
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_SyncWalk，用于显示任务NPC与玩家的距离
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_SyncWalk( tagNS_SyncWalk* pMsg, DWORD pParam )
{
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if( pMsg->dwRoleID == pLP->GetID() )
	{
		NPCFrameList::iterator iter;
		for( iter=m_NPCFrameList.begin(); iter!=m_NPCFrameList.end(); ++iter )
		{
			QuestNPCFrame *pFrame = *iter;
			if( pFrame->IsVisible() )
				pFrame->UpdateNPCList();
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_BindRebornMap
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_NS_BindRebornMap(tagNS_BindRebornMap* pMsg, DWORD pParam)
{
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
        {
		    CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("BindRebornMap_Success")]);
            LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
            if (P_VALID(pLp))
                pLp->SetRebornPoint(pMsg->dwBornMapID, pMsg->vBornPos);
        }
		break;
	case E_BindRebornMap_NPCInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("BindRebornMap_NPCInvalid")]);
		break;
	case E_BindRebornMap_MapInvalid:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("BindRebornMap_MapInvalid")]);
		break;
	case E_BindRebornMap_Already:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("BindRebornMap_Already")]);
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_MsgQuestBlob
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_NS_MsgBlob( tagNS_MsgBlob* pMsg, DWORD pParam )
{
	DWORD dwSize = pMsg->dwSize - sizeof(tagNS_MsgBlob) + sizeof(BYTE);
	m_pGSMgr->SetData(pMsg->byData,dwSize);

	m_pScript->OnParse();

	return 0;
}

DWORD QuestMgr::NetRecvMessage_NS_GMQuestState( tagNS_GMQuestState* pMsg, DWORD pParam )
{
	if( pMsg->bDone )//完成任务
	{
		QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find( pMsg->u16QuestID );
		if( finder != m_pQuery->m_currentQuests.end() )
		{
			CompleteQuest( pMsg->u16QuestID );
		}
	}
	else//设置任务为未完成
	{
		AcceptQuest( pMsg->u16QuestID );

		QuestQuery::DoneQuestsMap::iterator questFinder = m_pQuery->m_doneQuests.find( pMsg->u16QuestID );
		if( questFinder != m_pQuery->m_doneQuests.end() )
		{
			questFinder->second--;
			if( questFinder->second == 0 )
				m_pQuery->m_doneQuests.erase( questFinder );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理游戏事件tagStartNPCTalkEvent
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_NPCTalk( tagStartNPCTalkEvent* pEvent )
{
	OnNPCTalk( pEvent->globalID, pEvent->typeID );

	return 0;
}

//-----------------------------------------------------------------------------
// 处理游戏事件Open_Task
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_OpenTask( tagGameEvent* pEvent )
{
	QuestMainFrame *pMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestMain"));
	ASSERT( P_VALID(pMainFrame) );
	pMainFrame->Show(true);

	return 0;
}

//-----------------------------------------------------------------------------
// 处理游戏事件Close_Task
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_CloseTask( tagGameEvent* pEvent )
{
	QuestMainFrame *pMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestMain"));
	ASSERT( P_VALID(pMainFrame) );
	pMainFrame->Hide();

	return 0;
}

//-----------------------------------------------------------------------------
// 处理复活点MsgBox
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnSetRebornEvent(tagMsgBoxEvent* pEvent)
{
	if(pEvent->strEventName == _T("SetRebornEvent"))
	{
		if(pEvent->eResult == MBF_OK && TObjRef<NetSession>()->IsConnect())
		{
			tagNC_BindRebornMap msg;
			msg.dwNPCID = this->GetTalkNPCGlobalID();
			TObjRef<NetSession>()->Send(&msg);
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 任务追踪
//-----------------------------------------------------------------------------
BOOL QuestMgr::TrackQuest( UINT16 questID, bool bShowTrack )
{
	// 判断该任务是否可以进行跟踪
	if( !m_pQuery->QuestCanTrack( questID ) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("Quest_TrackDisable")] );
		return FALSE;
	}

	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find(questID);
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		QuestQuery::tagQuestTrack& track = finder->second;
		track.isTracked = !track.isTracked;
		track.lastUpdateTime = Kernel::Inst()->GetAccumTime();

		if( !P_VALID(m_pTrackFrame) && bShowTrack )
		{
			m_pTrackFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("QuestTrack"), _T("QuestTrackFrame"), 0 );
			ASSERT( P_VALID(m_pTrackFrame) );
		}

		if( P_VALID(m_pTrackFrame) )
		{
			if( track.isTracked
				&& !m_pTrackFrame->CanTrackQuest(finder->first) )// 增加任务跟踪
			{
				track.isTracked = false;
				CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("Quest_TrackOverflow")] );
			}
			else// 更新任务跟踪
			{
				m_pTrackFrame->UpdateQuestTrack();
			}
			m_pTrackFrame->Show();
		}

		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// 加载新地图，需要加载脚本
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_LoadNewMap( tagGameEvent* pEvent )
{
	m_pData->LoadNPCScriptFile( MapMgr::Inst()->GetCurMapName().c_str() );

	m_pQuery->BuildMapQuestNPCs();
	m_pQuery->UpdateTriggerEffect();

	if( m_bLPAttIsOK )
	{
		UpdateQuest();
	}
	
	//刷新下QUESTNPCFRAME
	QuestNPCFrame *pFrame = (QuestNPCFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestNPCQuestMain"));
	if (P_VALID(pFrame) && pFrame->IsVisible())
	{
		pFrame->UpdateNPCList();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 本地人物初始化完毕，需要建立各种任务列表
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_LPAttIsOK( tagGameEvent* pEvent )
{
	if( !m_bLPAttIsOK )
	{
		UpdateQuest();

		m_bLPAttIsOK = true;

		QuestMainFrame *pMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestMain"));
		ASSERT( P_VALID(pMainFrame) );

		if( !P_VALID(m_pTrackFrame) )
		{
			m_pTrackFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("QuestTrack"), _T("QuestTrackFrame"), 0 );
			ASSERT( P_VALID(m_pTrackFrame) );
		}

		//加入任务，因为颜色是根据等级生成的
		QuestQuery::QuestsMap::const_iterator iter;
		for( iter=m_pQuery->m_currentQuests.begin(); iter!=m_pQuery->m_currentQuests.end(); ++iter )
		{
			pMainFrame->AddQuest( iter->first );
			HandleTrackQuest(iter->first);
		}

		//发送点击事件，选中第一个任务
		pMainFrame->InitFocus();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 本地人物等级变化，需要重建各种任务列表
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_LPLevelChanged( tagGameEvent* pEvent )
{
	if( m_bLPAttIsOK )
	{
		UpdateQuest();

		QuestMainFrame *pMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestMain"));
		ASSERT( P_VALID(pMainFrame) );

		//任务变色
		pMainFrame->ChangeColor();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 本地人物技能变化，需要重建各种任务列表
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_LPSkillChanged( tagGameEvent* pEvent )
{
	if( m_bLPAttIsOK )
	{
		UpdateQuest();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 本地人物物品变化，需要重建各种任务列表
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_LPItemChanged( tagUpdateLPItemEvent* pEvent )
{
	if( m_bLPAttIsOK )
	{
		UpdateQuest();
		const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(pEvent->dwTypeID);
		if( P_VALID(pItem) )
		{
			if( pEvent->dwTypeID == QUICK_COMPLETE_QUEST_ITEM )
			{
				QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
				if( P_VALID(pQuestMainFrame) )
					pQuestMainFrame->UpdateQuickDoneQuest();
				QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
				if( P_VALID(pFrame) )
				{
					pFrame->UpdateQuestTrack();
				}
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 本地人物金钱变化，需要重建各种任务列表
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_LPMoneyChanged( tagGameEvent* pEvent )
{
	if( m_bLPAttIsOK )
	{
		UpdateQuest();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 本地人物声望变化，需要重建各种任务列表
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_LPReputeChanged( tagGameEvent* pEvent )
{
	if( m_bLPAttIsOK )
	{
		UpdateQuest();
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 触发器触发任务
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_RoleEnterTrigger( tagEnterTriggerEvent* pEvent )
{
	GameMap& gm = MapMgr::Inst()->GetGameMap();
	const vector<tagMapTrigger>& mapTriggers = gm.GetTriggers();
	for( int i=0; i<(int)mapTriggers.size(); i++ )
	{
		const tagMapTrigger& trigger = mapTriggers[i];
		if( trigger.dwObjID == pEvent->triggerID )
		{
			// 记录当前触发脚本的触发器ID
			m_curTrigger = trigger.dwObjID;

			//// 清空当前NPC记录
			//ClearTalkNPC();

			// 执行触发器中的脚本
			tstring strPath = vEngine::g_strLocalPath + _T("\\") + trigger.szScriptName;
			m_pScript->OnTrigger( strPath.c_str(), trigger.dwQuestSerial );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 使用任务物品触发任务
//-----------------------------------------------------------------------------
DWORD QuestMgr::OnEvent_UseQuest( tagUseQuestEvent* pEvent )
{
	// 清空当前NPC记录
	ClearTalkNPC();

	// 执行物品脚本
	tstringstream stream;
	stream << vEngine::g_strLocalPath << _T("\\script\\item\\") << pEvent->itemID << _T(".lua");
	m_pScript->OnTrigger( stream.str().c_str(), pEvent->questID );

	return 0;
}

VOID QuestMgr::RegisterFrames( GameFrameMgr* pMgr )
{
	if( P_VALID( pMgr ) )
	{
		pMgr->Register(TWrap<QuestMainFrame>(),		_T("QuestMainFrame"));
		pMgr->Register(TWrap<QuestTalkFrame>(),		_T("QuestTalkFrame"));
		pMgr->Register(TWrap<NPCTalkFrame>(),		_T("NPCTalkFrame"));
		pMgr->Register(TWrap<QuestActiveFrame>(),	_T("QuestActiveFrame"));
		pMgr->Register(TWrap<QuestNPCFrame>(),		_T("QuestNPCFrame"));
		pMgr->Register(TWrap<QuestTrackFrame>(),	_T("QuestTrackFrame"));
		pMgr->Register(TWrap<QuestFilterFrame>(),	_T("QuestFilterFrame"));
		pMgr->Register(TWrap<QuestHelpFrame>(),	_T("QuestHelpFrame"));

	}
}

VOID QuestMgr::ClearTalkNPC( void )
{
	m_curTalkNPC.globalID = GT_INVALID;
	m_curTalkNPC.typeID = GT_INVALID;
}

VOID QuestMgr::OpenActiveFrame( void )
{
	if( !P_VALID(m_pActiveFrame) )
	{
		m_pActiveFrame = (QuestActiveFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("QuestActive"), _T("QuestActiveFrame"), 0 );
	}

	m_pActiveFrame->Show();
}

VOID QuestMgr::CloseActiveFrame( void )
{
	if( P_VALID(m_pActiveFrame) )
	{
		m_pActiveFrame->Hide();
		TObjRef<GameFrameMgr>()->AddToDestroyList(m_pActiveFrame);
		m_pActiveFrame = NULL;
	}
}

BOOL QuestMgr::IsActiveFrameOpened(void)
{
	return P_VALID(m_pActiveFrame);
}

VOID QuestMgr::OpenNPCFrame( LPCTSTR szFather, LPCTSTR szFrame, LPCSTR szAlign, LPCSTR szControlName/* = NULL*/ )
{
	NPCFrameList::iterator iter;
	for( iter=m_NPCFrameList.begin(); iter!=m_NPCFrameList.end(); ++iter )
	{
		QuestNPCFrame *pFrame = *iter;
		if( _tcscmp(pFrame->GetName().c_str(), szFrame) == 0 )
		{
			pFrame->Show();
			return;
		}
	}

	QuestNPCFrame *pFrame = (QuestNPCFrame*)TObjRef<GameFrameMgr>()->GetFrame( szFrame );
	if( !P_VALID(pFrame) )
	{
		pFrame = (QuestNPCFrame*)TObjRef<GameFrameMgr>()->CreateFrame( szFather, szFrame, _T("QuestNPCFrame"), 0 );
	}
	pFrame->ReloadUI(szAlign, "OuterLeftTop", szControlName);
	pFrame->Show();

	m_NPCFrameList.push_back( pFrame );
}

QuestNPCFrame* QuestMgr::GetNPCFrame( LPCTSTR szFrame )
{
	NPCFrameList::iterator iter;
	for( iter=m_NPCFrameList.begin(); iter!=m_NPCFrameList.end(); ++iter )
	{
		QuestNPCFrame *pFrame = *iter;
		if( _tcscmp(pFrame->GetName().c_str(), szFrame) == 0 )
		{
			return pFrame;
		}
	}

	return NULL;
}

VOID QuestMgr::CloseNPCFrame( LPCTSTR szFrame, bool bHide )
{
	NPCFrameList::iterator iter;
	for( iter=m_NPCFrameList.begin(); iter!=m_NPCFrameList.end(); ++iter )
	{
		QuestNPCFrame *pFrame = *iter;
		if( _tcscmp(pFrame->GetName().c_str(), szFrame) == 0 )
		{
			pFrame->Hide();

			// 如果父窗口是QuestMain，发送通知消息
			if( !bHide && pFrame->GetFather() == (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestMain")) )
			{
				tagGameEvent e(_T("QuestMainFrameEvent_CloseNPC"), NULL);
				TObjRef<GameFrameMgr>()->SendEvent(&e);
			}

			TObjRef<GameFrameMgr>()->AddToDestroyList(pFrame);
			m_NPCFrameList.erase( iter );

			return;
		}
	}
}

VOID QuestMgr::OpenTrackFrame( void )
{
	if( !P_VALID(m_pTrackFrame) )
	{
		m_pTrackFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("QuestTrack"), _T("QuestTrackFrame"), 0 );
		ASSERT( P_VALID(m_pTrackFrame) );
	}

	m_pTrackFrame->UpdateQuestTrack();
	m_pTrackFrame->Show();
}

VOID QuestMgr::CloseTrackFrame( void )
{
	if( P_VALID(m_pTrackFrame) )
	{
		m_pTrackFrame->Hide();
		TObjRef<GameFrameMgr>()->AddToDestroyList(m_pTrackFrame);
		m_pTrackFrame = NULL;
	}
}

VOID QuestMgr::OpenQuestHelpFrame( void )
{
	if( !P_VALID(m_pQuestHelpFrame) )
	{
		m_pQuestHelpFrame = (QuestHelpFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("QuestHelp"), _T("QuestHelpFrame"), 0 );
		ASSERT( P_VALID(m_pTrackFrame) );
	}
	m_pQuestHelpFrame->ShowFrame();
}

VOID QuestMgr::CloseQuestHelpFrame( void )
{
	if( P_VALID(m_pQuestHelpFrame) )
	{
		m_pQuestHelpFrame->HideFrame();
		TObjRef<GameFrameMgr>()->AddToDestroyList(m_pQuestHelpFrame);
		m_pQuestHelpFrame = NULL;
	}
}


BOOL QuestMgr::IsTrackFrameOpened(void)
{
	return P_VALID(m_pTrackFrame);
}

BOOL QuestMgr::IsQuestHelpFrameOpened(void)
{
	return P_VALID(m_pQuestHelpFrame);
}

BOOL QuestMgr::MoveToMap( LPCTSTR szDstMap,Vector3& pos )
{
	NavMap* pNav = MapMgr::Inst()->GetNavMap();
	PlayerNavMap* pPlayerNav = pNav->GetPlayerNavMap();
	if( !pPlayerNav->FindPathByMap( MapMgr::Inst()->GetCurMapName().c_str(), szDstMap, pos ) )
		return FALSE;

	tstring showMapName = MapLogicData::Inst()->FindMapShowName( m_pUtil->Crc32(szDstMap) );
	CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("Quest_GotoMap")], showMapName.c_str() );

	return TRUE;
}

VOID QuestMgr::UpdateQuest( void )
{
	m_pQuery->BuildAcceptQuestNPCs();
	m_pQuery->BuildCompleteQuestNPCs();
	m_pQuery->BuildUpdateQuestNPCs();

	// 发送游戏事件让onlinetips刷新
	TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent( _T("Refresh_Onlinetips"), NULL ));
}

//-----------------------------------------------------------------------------
// 解析任务描述信息
//-----------------------------------------------------------------------------
tstring& QuestMgr::ParseQuestView( tstring& strView,vector<tagIDLink>& idLinks,vector<tagPosLink>& posLinks )
{
	DWORD	curTextColor = 0xffffffff;
	UINT	nIndex = 0;
	int		idLinkIndex = 1;
	int		posLinkIndex = 1000;
	TCHAR	newValue[128]={0};
	idLinks.clear();
	posLinks.clear();
	while( nIndex < strView.size() )
	{
		if( strView[nIndex] == _T('<') )
		{
			tstring::size_type flagEndPos = strView.find_first_of(_T('>'), nIndex);
			if( flagEndPos == tstring::npos || flagEndPos - nIndex <= 1 )
				break;	// 语法错误

			tstring str = strView.substr( nIndex+1, flagEndPos-nIndex-1 );

			if( str.length() == 1 )
			{
				bool isValid = true;
				switch( str[0] )
				{
				case _T('N'):
					curTextColor = 0xFFABABFF;
					break;
				case _T('M'):
					curTextColor = 0xFF00DE2A;
					break;
				case _T('P'):
					curTextColor = 0xFFFFE289;
					break;
				case _T('D'):
					curTextColor = 0xFF00DDBB;
					break;
				case _T('I'):
					curTextColor = 0xFF31A0FF;
					break;
				case _T('A'):
					curTextColor = 0xFFFFBC00;
					break;
				case _T('S'):
					curTextColor = 0xFF31A0FF;
					break;
				case _T('T'):
					curTextColor = 0xFFFFBC00;
					break;
				case _T('C'):
					curTextColor = 0xFF00DDFE;
					break;
				default:
					curTextColor = 0xffffffff;
					isValid = false;
				}

				if( isValid )
				{
					_stprintf( newValue, _T("<color=0x%x>"), curTextColor );
					strView.replace( nIndex, flagEndPos-nIndex+1, newValue );

					nIndex += _tcslen(newValue);
				}
				else
					nIndex = flagEndPos+1;
			}
			else if( str.length() == 2 )
			{
				int state = 0;
				switch( str[1] )
				{
				case _T('N'):
				case _T('M'):
				case _T('P'):
				case _T('D'):
				case _T('I'):
				case _T('A'):
				case _T('S'):
				case _T('T'):
				case _T('C'):
					state = 1;
					curTextColor = 0xffffffff;
					break;
				case _T('L'):
				case _T('Z'):
					state = 2;
					break;
				}

				if( state == 1 )
				{
					_stprintf( newValue, _T("<color=0x%x>"), curTextColor );
					strView.replace( nIndex, flagEndPos-nIndex+1, newValue );

					nIndex += _tcslen(newValue);
				}
				else if( state == 2 )
				{
					_stprintf( newValue, _T("%s"), _T("<link=0xffffffff,0>") );
					strView.replace( nIndex, flagEndPos-nIndex+1, newValue );

					nIndex += _tcslen(newValue);
				}
				else
					nIndex = flagEndPos+1;
			}
			else
			{
				if( str[0] == _T('L') )
				{
					_stprintf( newValue,
						_T("<link=%x,0x%x>"),
						idLinkIndex++,
						curTextColor
						);
					tagIDLink tagLink;

					tstring::size_type flagStartPos = str.find_first_of( _T("="), 0 );
					tstring::size_type mapPos = str.find( _T(","), flagStartPos+1 );
					tagLink.mapName = str.substr(flagStartPos+1, mapPos-flagStartPos-1);

					tstring strID = str.substr( mapPos+1, str.length()-mapPos-1 );
					tagLink.typeID = _tcstoul( strID.c_str(), NULL, 10 );

					strView.replace( nIndex, flagEndPos-nIndex+1, newValue );

					nIndex += _tcslen(newValue);

					idLinks.push_back( tagLink );
				}
				else if( str[0] == _T('Z') )
				{
					_stprintf( newValue,
						_T("<link=%x,0x%x>"),
						posLinkIndex++,
						curTextColor
						);
					tagPosLink tagLink;

					tstring::size_type flagStartPos = str.find_first_of( _T("="), 0 );
					tstring::size_type mapPos = str.find( _T(","), flagStartPos+1 );
					tagLink.mapName = str.substr( flagStartPos+1, mapPos-flagStartPos-1 );

					tstring::size_type xPos = str.find( _T(","), mapPos+1 );
					tstring strPos = str.substr( mapPos+1, xPos-mapPos-1 );
					tagLink.pos.x = (float)_tstof(strPos.c_str());

					tstring::size_type yPos = str.find( _T(","), xPos+1 );
					strPos = str.substr( xPos+1, yPos-xPos-1 );
					tagLink.pos.y = (float)_tstof(strPos.c_str());

					strPos = str.substr( yPos+1, str.length()-yPos-1 );
					tagLink.pos.z = (float)_tstof(strPos.c_str());

					strView.replace( nIndex, flagEndPos-nIndex+1, newValue );

					nIndex += _tcslen(newValue);

					posLinks.push_back( tagLink );
				}
				else if(str[0] == _T('G'))	//工会名字
				{
					// 取出GuildID
					tstring strTest = str.substr(2, 12);
					DWORD dwGuildID = _tcstoul(strTest.c_str(), NULL, 10);
					tstring strGuildName = GuildMgr::Inst()->GetGuildName(dwGuildID);
					tstring strLastText = strView.substr(nIndex+14);
					tstring strBeginText = strView.substr(0, nIndex);
					if (!strGuildName.empty())
					{
// 						strView.replace(nIndex, nIndex+strGuildName.length(), strGuildName.c_str());
// 						nIndex += strGuildName.length();
// 						strView += strLastText.c_str();

						strView = strBeginText.c_str();
						strView += strGuildName.c_str();
						strView += strLastText.c_str();
						nIndex += strGuildName.length();
					}
					else
					{
						strView = strBeginText.c_str();
						strView += strLastText.c_str();
					}
				}
				else
					nIndex = flagEndPos+1;
			}
		}
		else
			nIndex++;
	}

	return strView;
}

VOID QuestMgr::SetSpeRew( const tagQuestProto* pQuestProto, vector<GUIStatic*>& bonusSpecT, vector<GUIStatic*>& bonusSpecV )
{
	vector<tagSpeRew> speRews;

	// 声望
	for( INT i=0; i<QUEST_REPUTATIONS_COUNT; i++ )
	{
		if( pQuestProto->rew_rep[i] != 0 )
		{
			tagSpeRew rew;
			rew.rew = ESRT_REP;
			rew.type = pQuestProto->rew_rep[i];
			rew.value = pQuestProto->rew_rep_val[i];

			speRews.push_back( rew );
		}
		else
			break;
	}

	// 贡献
	for( INT i=0; i<QUEST_CONTRIBUTION_COUNT; i++ )
	{
		if( pQuestProto->rew_contribution[i] != 0 )
		{
			tagSpeRew rew;
			rew.rew = ESRT_CONTRIBUTION;
			rew.type = pQuestProto->rew_contribution[i];
			rew.value = pQuestProto->rew_contribution_val[i];

			speRews.push_back( rew );
		}
		else
			break;
	}

	// 属性
	for( INT i=0; i<QUEST_ATTS_COUNT; i++ )
	{
		if( pQuestProto->rew_att[i] != 0 )
		{
			tagSpeRew rew;
			rew.rew = ESRT_ATT;
			rew.type = pQuestProto->rew_att[i];
			rew.value = pQuestProto->rew_att_val[i];

			speRews.push_back( rew );
		}
		else
			break;
	}

	// 技能
	for( INT i=0; i<QUEST_SKILLS_COUNT; i++ )
	{
		if( pQuestProto->rew_skill[i] != 0 )
		{
			tagSpeRew rew;
			rew.rew = ESRT_SKILL;
			rew.type = pQuestProto->rew_skill[i];
			rew.value = pQuestProto->rew_skill_val[i];

			speRews.push_back( rew );
		}
		else
			break;
	}

	// buff
	for( INT i=0; i<QUEST_REW_BUFF; i++ )
	{
		if( pQuestProto->rew_buff[i] != 0 )
		{
			tagSpeRew rew;
			rew.rew = ESRT_BUFF;
			rew.type = pQuestProto->rew_buff[i];

			speRews.push_back( rew );
		}
		else
			break;
	}

	TCHAR szValue[20];
	for( size_t i=0; i<bonusSpecT.size(); i++ )
	{
		GUIStatic *pStaticT = bonusSpecT[i];
		GUIStatic *pStaticV = bonusSpecV[i];

		if( i<speRews.size() )
		{
			const tagSpeRew& rew = speRews[i];
			switch( rew.rew )
			{
			case ESRT_REP:
				pStaticT->SetText( g_StrTable[_T("QuestMain_RewRepName")] );
				pStaticV->SetTextColor( 0xFFBBAD37 );
				TCHAR szRepName[40];
				_stprintf( szRepName, _T("QuestMain_RepName%d"), rew.type );
				_stprintf( szValue, _T("%s +%d"), g_StrTable[szRepName], rew.value );
				pStaticV->SetText( szValue );
				break;

			case ESRT_CONTRIBUTION:
				pStaticT->SetText( g_StrTable[_T("QuestMain_RewConName")] );
				pStaticV->SetTextColor( 0xFFFF9000 );
				TCHAR szConName[40];
				_stprintf( szConName, _T("QuestMain_ConName%d"), rew.type );
				_stprintf( szValue, _T("%s +%d"), g_StrTable[szConName], rew.value );
				pStaticV->SetText( szValue );
				break;

			case ESRT_ATT:
				pStaticT->SetText( g_StrTable[_T("QuestMain_RewAttName")] );
				pStaticV->SetTextColor( 0xFFC6FF00 );
				TCHAR szAttName[40];
				_stprintf( szAttName, _T("QuestMain_AttName%d"), rew.type );
				_stprintf( szValue, _T("%s +%d"), g_StrTable[szAttName], rew.value );				
				pStaticV->SetText( szValue );
				break;

			case ESRT_SKILL:
				{
					const tagSkillProtoClient* pSkill = SkillProtoData::Inst()->FindSkillProto(rew.type);
					if( P_VALID(pSkill) )
					{
						pStaticT->SetText( g_StrTable[_T("QuestMain_RewSkillName")] );
						pStaticV->SetTextColor( 0xFF23DDFF );
						_stprintf( szValue, _T("%s +%d"), pSkill->szName, rew.value );
						pStaticV->SetText( szValue );
					}
				}
				break;

			case ESRT_BUFF:
				{
					const tagBuffProtoClient* pBuff = SkillProtoData::Inst()->FindBuffProto(rew.type);
					if( P_VALID(pBuff) )
					{
						pStaticT->SetText( g_StrTable[_T("QuestMain_RewBuffName")] );
						pStaticV->SetTextColor( 0xFFFFE25E );
						pStaticV->SetText( pBuff->szName );
					}
				}
				break;
			}
		}
		else
		{
			pStaticT->SetText(_T(""));
			pStaticV->SetText(_T(""));
		}
	}
}

DWORD QuestMgr::GetTalkNPCGlobalID(void) const
{
	return m_curTalkNPC.globalID;
}

DWORD QuestMgr::GetTalkNPCTypeID(void) const
{
	return m_curTalkNPC.typeID;
}


//-----------------------------------------------------------------------------
// 进游戏就初始化任务追踪
//-----------------------------------------------------------------------------
BOOL QuestMgr::HandleTrackQuest( UINT16 questID )
{
	// 判断该任务是否可以进行跟踪
	if( !m_pQuery->QuestCanTrack( questID ) )
	{
		return FALSE;
	}

	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find(questID);
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		QuestQuery::tagQuestTrack& track = finder->second;
		track.isTracked = !track.isTracked;
		track.lastUpdateTime = Kernel::Inst()->GetAccumTime();

		if( P_VALID(m_pTrackFrame) )
		{
			m_pTrackFrame->UpdateQuestTrack();
		}

		return TRUE;
	}

	return FALSE;
}

void QuestMgr::OpenWalkWare()
{
	m_pScript->OnTalk4WalkWare();
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_QuestUpdateFriends
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_NS_UpdateQuestFriends( tagNS_QuestUpdateFriends *pMsg, DWORD )
{
	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find( pMsg->u16QuestID );
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		QuestQuery::tagQuestTrack& track = finder->second;
		if( pMsg->nFriendsNum >= 0 )
		{
			track.isTracked = true;
			track.lastUpdateTime = Kernel::Inst()->GetAccumTime();
			QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
			if( P_VALID(pFrame) )
			{
				pFrame->UpdateQuestTrack();
			}
			tstringstream strinfo;
			strinfo << g_StrTable[_T("FriendNum")];
			strinfo << pMsg->nFriendsNum;
			CombatSysUtil::Inst()->ShowScreenCenterMsg(strinfo.str().c_str());
			m_pQuery->UpdateInvesObject(pMsg->u16QuestID);
		}

		QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
		ASSERT( P_VALID(pQuestMainFrame) );
		pQuestMainFrame->UpdateQuestDesc(pMsg->u16QuestID);

		// 如果任务完成，更新任务名称为[完成]
		if( m_pQuery->UpdateQuestState( pMsg->u16QuestID ) )
		{
			pQuestMainFrame->SetQuestCompleteFlag( pMsg->u16QuestID );
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 处理网络消息NS_QuestUpdateTeammates
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_NS_UpdateQuestTeammates( tagNS_QuestUpdateTeammates *pMsg, DWORD )
{
	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find( pMsg->u16QuestID );
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		QuestQuery::tagQuestTrack& track = finder->second;
		if( pMsg->byTeammatesNum >= 0 )
		{
			track.isTracked = true;
			track.lastUpdateTime = Kernel::Inst()->GetAccumTime();
			QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
			if( P_VALID(pFrame) )
			{
				pFrame->UpdateQuestTrack();
			}
			tstringstream strinfo;
			strinfo << g_StrTable[_T("TeammateNum")];
			strinfo << pMsg->byTeammatesNum;
			CombatSysUtil::Inst()->ShowScreenCenterMsg(strinfo.str().c_str());
			m_pQuery->UpdateInvesObject(pMsg->u16QuestID);
		}

		QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
		ASSERT( P_VALID(pQuestMainFrame) );
		pQuestMainFrame->UpdateQuestDesc(pMsg->u16QuestID);

		// 如果任务完成，更新任务名称为[完成]
		if( m_pQuery->UpdateQuestState( pMsg->u16QuestID ) )
		{
			pQuestMainFrame->SetQuestCompleteFlag( pMsg->u16QuestID );
		}
		else
		{
			pQuestMainFrame->SetQuestTrackFlag( pMsg->u16QuestID );
		}
		QuestMgr::Inst()->GetQuery()->BuildCompleteQuestNPCs();
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 处理网络消息NS_QuestUpdateLevel
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_NS_UpdateQuestLevel( tagNS_QuestUpdateLevel *pMsg, DWORD )
{
	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find( pMsg->u16QuestID );
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		QuestQuery::tagQuestTrack& track = finder->second;
		if( pMsg->nLevel > 0 )
		{
			track.isTracked = true;
			track.lastUpdateTime = Kernel::Inst()->GetAccumTime();
			QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
			if( P_VALID(pFrame) )
			{
				pFrame->UpdateQuestTrack();
			}

			m_pQuery->UpdateInvesObject(pMsg->u16QuestID);
		}

		QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
		ASSERT( P_VALID(pQuestMainFrame) );
		pQuestMainFrame->UpdateQuestDesc(pMsg->u16QuestID);

		// 如果任务完成，更新任务名称为[完成]
		if( m_pQuery->UpdateQuestState( pMsg->u16QuestID ) )
		{
			pQuestMainFrame->SetQuestCompleteFlag( pMsg->u16QuestID );
		}
	}

	return 0;
}

// 取得新手任务的状态 0：暂未接取； 1：进行中； 2：已完成
int QuestMgr::GetQuestState( DWORD questid )
{
	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find( (UINT16)questid );
	if( finder != m_pQuery->m_currentQuests.end() )
		return 1;
	else
	{
		QuestQuery::DoneQuestsMap::iterator questFinder = m_pQuery->m_doneQuests.find( (UINT16)questid );
		if( questFinder != m_pQuery->m_doneQuests.end() )
			return 2;
		else
			return 0;
	}
}

DWORD QuestMgr::OnMouseMoveFinishEvent(tagMouseMoveFinishEvent* pEvent)
{
	if( pEvent->bSuccessed)
	{
		const map<DWORD,Role*>& roleMap = RoleMgr::Inst()->GetRoleMap();
		DWORD dwGlobalID = GT_INVALID;
		map<DWORD, Role*>::const_iterator iter = roleMap.begin();
		for( iter; iter != roleMap.end(); iter++ )
		{
			Role *pRole = iter->second;
			if( pRole->IS_KIND_OF(NPC) )
			{
				NPC *pNPC = (NPC*)pRole;
				if( pNPC->GetTypeID() == pEvent->dwNpcTypeID )
					dwGlobalID = iter->first;
			}
			else
				continue;
		}
		if( dwGlobalID != GT_INVALID )
			OnNPCTalk( dwGlobalID, pEvent->dwNpcTypeID );
	}
	return 0;
}

//DWORD QuestMgr::OnEvent_GetSpeItem( tagCombatMsgForItemChange* pEvent )
//{
//	const tagItemProto	*pItem = ItemProtoData::Inst()->FindItemProto(pEvent->dwTypeID);
//	if( P_VALID(pItem) )
//	{
//		if( pEvent->n16Num >= 1 && pEvent->dwTypeID == QUICK_COMPLETE_QUEST_ITEM )
//		{
//			QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
//			if( P_VALID(pQuestMainFrame) )
//				pQuestMainFrame->UpdateQuickDoneQuest();
//			QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
//			if( P_VALID(pFrame) )
//			{
//				pFrame->UpdateQuestTrack();
//			}
//		}
//	}
//	return 0;
//}

// 任务板任务
DWORD QuestMgr::OnNetGetQuest( tagNS_OpenQuestBoard *pNetCmd, DWORD )
{
	if( pNetCmd->dwErrorCode == E_Success )
	{
		m_mapQuestPannel[pNetCmd->eQuestBoardType].clear();
		for( int i = 0; i < QUEST_BOARD_MAX_CAPACITY; i++ )
			m_mapQuestPannel[pNetCmd->eQuestBoardType].push_back(pNetCmd->u16Quest[i]);
		QuestPanelFrame *pFrame = (QuestPanelFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestPanel"));
		if(P_VALID(pFrame))
			pFrame->UpdateQuestList();
	}
	return 0;
}


//-----------------------------------------------------------------------------
// 处理网络消息NS_RoleSpecQuestCompleteState
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_NS_UpdateSpecialQuest( tagNS_RoleSpecQuestCompleteState *pMsg, DWORD )
{
	if( RoleMgr::Inst()->GetLocalPlayerID() != pMsg->dwRoleID )
		return 0;

	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find( pMsg->u16QuestID );
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		QuestQuery::tagQuestTrack& track = finder->second;
		if( m_pQuery->SetSpecialQuestFlag( pMsg->u16QuestID, pMsg->byState ) )
		{
			track.isTracked = true;
			track.lastUpdateTime = Kernel::Inst()->GetAccumTime();
			QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
			if( P_VALID(pFrame) )
			{
				pFrame->UpdateQuestTrack();
			}
		}

		QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
		ASSERT( P_VALID(pQuestMainFrame) );
		pQuestMainFrame->UpdateQuestDesc(pMsg->u16QuestID);

		// 如果任务完成，更新任务名称为[完成]
		if( m_pQuery->UpdateQuestState( pMsg->u16QuestID ) )
		{
			pQuestMainFrame->SetQuestCompleteFlag( pMsg->u16QuestID );
		}
	}

	return 0;
}

DWORD QuestMgr::GetQuestPanelTaskColor( UINT16 questID )
{
	size_t i = 0;
	vector<UINT16> vector;
	if(questID>=YELLOW_LIST_START_TASK_ID)
	{
		i = questID - YELLOW_LIST_START_TASK_ID;
		vector = m_mapQuestPannel[EQBT_YellowList];
	}
	else
	{
		i = questID - QUEST_PANEL_START_TASK_ID;
		vector = m_mapQuestPannel[EQBT_Reward];
	}
	DWORD dwColor = GT_INVALID;
	QuestData *pData = QuestMgr::Inst()->GetData();
	if( i >= 0 && i < vector.size() )
	{
		const tagBoardQuestProto *pProto = pData->GetQuestPanelProto(vector[i]);
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
	}
	return dwColor;
}


//-----------------------------------------------------------------------------
// 处理网络消息NS_RoleSpecQuestCompleteState
//-----------------------------------------------------------------------------
DWORD QuestMgr::NetRecvMessage_NS_ChuanDaoQuestUpdateCount( tagNS_ChuanDaoQuestUpdateCount *pMsg, DWORD )
{
	if( RoleMgr::Inst()->GetLocalPlayerID() != pMsg->dwRoleID )
		return 0;

	QuestQuery::QuestsMap::iterator finder = m_pQuery->m_currentQuests.find( pMsg->u16QuestID );
	if( finder != m_pQuery->m_currentQuests.end() )
	{
		QuestQuery::tagQuestTrack& track = finder->second;
		if( m_pQuery->SetQuestKillTimes( pMsg->u16QuestID, (INT16)pMsg->nNewNum ) )
		{
			track.isTracked = true;
			track.lastUpdateTime = Kernel::Inst()->GetAccumTime();
			QuestTrackFrame *pFrame = (QuestTrackFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestTrack"));
			if( P_VALID(pFrame) )
			{
				pFrame->UpdateQuestTrack();
			}

			// 屏显，显示击杀怪物，任务名称 击杀 怪物名称 1/5
			QuestData *pData = QuestMgr::Inst()->GetData();
			if(!P_VALID(pData))
				return 0;
			const tagQuestProto* pQuest = pData->GetQuestProto(pMsg->u16QuestID);
			if(!P_VALID(pQuest))
				return 0;
			UINT16 count = pQuest->chuandao_count;
			tstring questName;
			m_pData->GetQuestName( pMsg->u16QuestID, questName );
			if( pMsg->nNewNum > count )
			{
				pMsg->nNewNum = count;
			}
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("Quest_ChuanDaoKillCreature")], 
				questName.c_str(), pMsg->nNewNum, count );
		}

		QuestMainFrame *pQuestMainFrame = (QuestMainFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("QuestMain") );
		ASSERT( P_VALID(pQuestMainFrame) );
		pQuestMainFrame->UpdateQuestDesc(pMsg->u16QuestID);

		// 如果任务完成，更新任务名称为[完成]
		if( m_pQuery->UpdateQuestState( pMsg->u16QuestID ) )
		{
			pQuestMainFrame->SetQuestCompleteFlag( pMsg->u16QuestID );
		}
	}

	return 0;
}

DWORD QuestMgr::OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam)
{
	if(pMsg->dwErrorCode!=E_Success)
		return 0; 

	const tagSkillData* pData = SkillMgr::Inst()->GetSkillData(pMsg->dwSkillID/100);
	if(P_VALID(pData))
	{
		if(ESSF_FreeOpenFace == pData->pProto->eSpecFunc && ESSOF_AWARD	 == pData->pProto->nSpecFuncVal1)
		{
			const tagCreatureProto* pCreatureProto = CreatureData::Inst()->FindNpcAtt(pData->pProto->nSpecFuncVal2);
			if(P_VALID(pCreatureProto))
			{
				QuestPanelFrame *pFrame = (QuestPanelFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("QuestPanel"));
				if(P_VALID(pFrame))
					TObjRef<GameFrameMgr>()->DestroyFrame(pFrame);

				pFrame = (QuestPanelFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("QuestPanel"), _T("QuestPanelFrame"), 0 );
				if( P_VALID(pFrame) )
				{
					OnNPCTalk(0,pCreatureProto->dwTypeID);
					//2000000030
					pFrame->SetNPCID(0, pCreatureProto->eFunctionType,pCreatureProto->dwTypeID);
					tagNC_OpenQuestBoard m;
					m.dwNPCID = 0;
					m.dwNPCTypeID = pCreatureProto->dwTypeID;
					TObjRef<NetSession>()->Send(&m);
				}
			}
		}
	}

	return 0;
}