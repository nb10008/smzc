#include "stdafx.h"
#include "MiniMapStatic.h"
#include "MiniMapFrame.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "MapMgr.h"
#include "MapLogicData.h"
#include "MapEvent.h"
#include "ZoneMapFrame.h"
#include "QuestMgr.h"
#include "UIEvent.h"
#include "GameSet.h"
#include "OnlineTips.h"
#include "WorldFrame.h"
#include "SpecialSkillFrame.h"
#include "StrengthRankingFrame.h"
#include "GlobalHelpFrame.h"
#include "PetSkillAssistanceFrame.h"
#include "CityWarInfoFrame.h"
#include "CreatureData.h"
#include "CombatEvent.h"
#include "CombatActionChecker.h"
#include "CombatSysUtil.h"
#include "NetSession.h"
#include "FlashPushButton.h"
#include "ActivityMgr.h"
#include "EasyTeamFrame.h"
#include "..\WorldDefine\msg_spec_item.h"
#include "ItemEvent.h"
#include "FaceBookFrame.h"
#include "SaveAccount.h"
#include "PickupMgr.h"
#include "KeyCodeFrame.h"//add by xp
struct tagSpecItemEvent;

const INT g_nTileSize = 50;
static bool g_bfistLogin = TRUE;
MiniMapFrame::MiniMapFrame() :
m_Trunk( this ),
m_pWnd_MaxSize( NULL ),
m_pMSt_MiniMap( NULL ),
m_pSt_MapName( NULL ),
m_pSt_PosX( NULL ),
m_pSt_PosZ( NULL ),
m_pBn_MiniSize( NULL ),
m_pBn_Setting( NULL ),
m_pBn_AreaMap( NULL ),
m_pBn_StyleAction( NULL ),
m_pBn_TaskQuery( NULL ),
m_pBn_PosGoTo(NULL),
m_pBn_TaskTrace( NULL ),
m_pBn_ZoomIn( NULL ),
m_pBn_ZoomOut( NULL ),
m_pWnd_MiniSize( NULL ),
m_pBnSinaWeiBo(NULL),
m_pBnSinaWeiBoCard(NULL),
//m_pSt_MapName_MiniSize( NULL ),
//m_pSt_PosX_MiniSize( NULL ),
//m_pSt_PosZ_MiniSize( NULL ),
m_pBn_MaxSize( NULL ),
m_pWnd_Set( NULL ),
m_pWnd_PosQuery(NULL),
m_pBn_Pos_Ok_Set(NULL),
m_pBn_Pos_Cancel_Set(NULL),
m_pEd_PosX(NULL),
m_pEd_PosZ(NULL),
m_pBn_Close_Set( NULL ),
m_ptLocalPlayerPos( 0, 0 ),
m_pBn_VipStall(NULL),
m_pBn_OnlineTip(NULL),
m_pBtnQuestHelp(NULL),
m_pBtnStrengthRanking(NULL),
//m_pPBtn_ContinuteSkill(NULL),
m_pBnFaceBook(NULL)
{
	for( int i = 0; i < EMME_Num-1; i++ )
		m_pBn_EleShow[i] = NULL;
}

MiniMapFrame::~MiniMapFrame()
{
}



BOOL MiniMapFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pMgr->RegisterEventHandle( _T("OpenMap"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::OnOpenMap));
	m_pMgr->RegisterEventHandle( _T("tagLeaveMapAreaEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::OnLeaveMapArea));
	m_pMgr->RegisterEventHandle( _T("tagEnterMapAreaEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::OnEnterMapArea));
	m_pMgr->RegisterEventHandle( _T("tagSetFlagEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::OnSetFlagEvent));
	m_pMgr->RegisterEventHandle( _T("tagMouseMoveFinishEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::OnMouseMoveFinishEvent));

	m_pMgr->RegisterEventHandle(_T("Refresh_QueryBtn_Flash"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::RefreshQueryBtnFlash));

	m_pKeyMap->Register( 0, g_StrTable[_T("Hotkey_MiniMapZoomIn")],		(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&MiniMapFrame::OnZoomIn_HotKey), DIK_ADD, TRUE );
	m_pKeyMap->Register( 0, g_StrTable[_T("Hotkey_MiniMapZoomOut")],	(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&MiniMapFrame::OnZoomOut_HotKey), DIK_SUBTRACT, TRUE );

	//--注册网络消息处理函数
	m_pCmdMgr->Register("NS_CanGoToPos",		(NETMSGPROC)m_Trunk.sfp2(&MiniMapFrame::OnNS_CanGoToPos),			_T("NS_CanGoToPos"));

	return TRUE;
}

BOOL MiniMapFrame::Destroy()
{
	m_pMgr->UnRegisterEventHandler( _T("OpenMap"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::OnOpenMap));
	m_pMgr->UnRegisterEventHandler( _T("tagLeaveMapAreaEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::OnLeaveMapArea));
	m_pMgr->UnRegisterEventHandler( _T("tagEnterMapAreaEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::OnEnterMapArea));
	m_pMgr->UnRegisterEventHandler( _T("tagSetFlagEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::OnSetFlagEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagMouseMoveFinishEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::OnMouseMoveFinishEvent));
	m_pMgr->UnRegisterEventHandler( _T("Refresh_QueryBtn_Flash"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&MiniMapFrame::RefreshQueryBtnFlash));

	m_pKeyMap->SetEnable( g_StrTable[_T("Hotkey_MiniMapZoomIn")],	 FALSE );
	m_pKeyMap->SetEnable( g_StrTable[_T("Hotkey_MiniMapZoomOut")],   FALSE );

	//--注销网络消息处理函数
	m_pCmdMgr->UnRegister("NS_CanGoToPos",			(NETMSGPROC)m_Trunk.sfp2(&MiniMapFrame::OnNS_CanGoToPos));
	GameFrame::Destroy();

	SafeDestroyUI();

	return TRUE;
}


BOOL MiniMapFrame::ReloadUI()
{
	SafeDestroyUI();

	// 加载小地图界面
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\minimap.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &element, "minimap\\map", "ClassName", "MiniMapStatic");
	m_pGUI->ChangeXml(&element, "minimap\\map\\but4", "ClassName", "FlashButton");

	// 小地图中需要显示的图标
	std::vector<std::string> vecControlName( EMME_Num );
	std::vector<tstring> vecPicFilename( EMME_Num );
	vecControlName[EMME_LocalPlayer]		= "player";
	vecControlName[EMME_NatuRes]			= "collection";
	vecControlName[EMME_Nest]				= "nestmark";
	vecControlName[EMME_NormalMonster]		= "monster";
	vecControlName[EMME_Boss]				= "bossmark";
	vecControlName[EMME_NormalNPC]			= "npcmark";
	vecControlName[EMME_AcceptQuestNPC]		= "npcdoable";
	vecControlName[EMME_CompleteQuestNPC]   = "npcfinish";
    vecControlName[EMME_RepeatAcceptQuestNPC] = "rnpcdoable";
    vecControlName[EMME_RepeatCompleteQuestNPC] = "rnpcfinish";
	vecControlName[EMME_Teammate]			= "party";
	vecControlName[EMME_OtherPlayer]		= "other";
	vecControlName[EMME_ActiveMonster]		= "ActiveMonster";
	vecControlName[EMME_AttackSide]			= "AttackSide";
	vecControlName[EMME_DefenceSide]		= "DefenceSide";
	vecControlName[EMME_Foe]				= "enemy";
	tstring strDestMarkControlName;

	// Alpha图路径
	tstring strAlphaTexFilename;

	// 地图显示区域
	tagRect rcMap;

	// 读取贴图路径及地图显示区域
	XmlElement* pElement = element.FirstChildElement();
	while( pElement )
	{
		const std::string strName = pElement->Attribute("Name");
		if( strName == "maprect" )
		{
			pElement->Attribute( "LogicPos_x", (int*)&rcMap.left );
			pElement->Attribute( "LogicPos_y", (int*)&rcMap.top );
			pElement->Attribute( "Size_x", (int*)&rcMap.right );
			pElement->Attribute( "Size_y", (int*)&rcMap.bottom );
			rcMap.right += rcMap.left;
			rcMap.bottom += rcMap.top;

			XmlElement* pEleRes = pElement->FirstChildElement();
			while ( pEleRes )
			{
				const std::string strNameRes = pEleRes->Attribute("Name");
				for( UINT i = 0; i < vecControlName.size(); i++ )
				{
					if( strNameRes == vecControlName[i] )
					{
						vecPicFilename[i] = m_pUtil->Unicode8ToUnicode( pEleRes->Attribute( "PicFileName" ) );
						break;
					}
				}
				if( strNameRes == "mark" )
				{
					const char* szPicFileName = pEleRes->Attribute( "PicFileName" );
					if( NULL != szPicFileName )
						strDestMarkControlName = m_pUtil->Unicode8ToUnicode( szPicFileName );
				}
				pEleRes = pEleRes->NextSiblingElement();
			}
		}
		else if( strName == "alphatex" )
		{
			strAlphaTexFilename = m_pUtil->Unicode8ToUnicode( pElement->Attribute( "PicFileName" ) );
		}
		pElement = pElement->NextSiblingElement();
	}

	// 创建窗口
	m_pWnd_MaxSize = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd_MaxSize ) )
	{
		SafeDestroyUI();
		return FALSE;
	}

	// 销毁用来保存数据的控件
	m_pGUI->DestroyWnd( m_pWnd_MaxSize->GetChild( _T("maprect") ) );
	m_pGUI->DestroyWnd( m_pWnd_MaxSize->GetChild( _T("alphatex") ) );
	
	strPath = g_strLocalPath + _T("\\ui\\minmap.xml");
	m_pWnd_MiniSize = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );

	if( !P_VALID( m_pWnd_MiniSize )  )
	{
		SafeDestroyUI();
		return FALSE;
	}

	//加载手动输入坐标查询界面

	strPath = g_strLocalPath + _T("\\ui\\tasktrack_manual.xml");
	m_pWnd_PosQuery = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	if( !P_VALID( m_pWnd_PosQuery ) )
	{
		SafeDestroyUI();
		return FALSE;
	}

	// 加载小地图配置界面
	strPath = g_strLocalPath + _T("\\ui\\mm_set.xml");
	m_pWnd_Set = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	if( !P_VALID( m_pWnd_Set ) )
	{
		SafeDestroyUI();
		return FALSE;
	}

	m_pGUI->RegisterEventHandler( m_pWnd_MaxSize->GetFullName().c_str(),   m_Trunk.sfp1(&MiniMapFrame::OnWnd_MaxSize));
	m_pGUI->RegisterEventHandler( m_pWnd_MiniSize->GetFullName().c_str(),  m_Trunk.sfp1(&MiniMapFrame::OnWnd_MiniSize));
	m_pGUI->RegisterEventHandler( m_pWnd_Set->GetFullName().c_str(),       m_Trunk.sfp1(&MiniMapFrame::OnWnd_Set));
	m_pGUI->RegisterEventHandler( m_pWnd_PosQuery->GetFullName().c_str(),  m_Trunk.sfp1(&MiniMapFrame::OnWnd_PosQuery));

	m_pMSt_MiniMap          = (MiniMapStatic*)m_pWnd_MaxSize->GetChild( _T("map") );
	m_pSt_MapName           = (GUIStatic*)m_pMSt_MiniMap->GetChild( _T("mapname") );
	m_pSt_PosX              = (GUIStatic*)m_pMSt_MiniMap->GetChild( _T("xzone") );
	m_pSt_PosZ              = (GUIStatic*)m_pMSt_MiniMap->GetChild( _T("yzone") );
	m_pBn_MiniSize          = (GUIButton*)m_pMSt_MiniMap->GetChild( _T("mapbutt") );
	m_pBn_Setting           = (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but1") );
	m_pBn_AreaMap           = (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but2") );
	m_pBn_TaskQuery         = (FlashButton*)m_pMSt_MiniMap->GetChild( _T("but4") );
	m_pBn_TaskQuery->SetFlash(FALSE);
	m_pBn_PosGoTo			= (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but50") );
	m_pBn_TaskTrace         = (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but5") );
	m_pBn_ZoomIn            = (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but11") );
	m_pBn_ZoomOut           = (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but10") );
	m_pBn_VipStall			= (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but3") );
	m_pBn_OnlineTip			= (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but6") );
	m_pBn_StyleAction       = (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but12") );
	m_pBn_CityWarInfo		= (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but15"));
	m_pBtnStrengthRanking	= (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but7"));
	//m_pPBtn_ContinuteSkill	= (GUIPushButton*)m_pMSt_MiniMap->GetChild( _T("but13") );
	//m_pPBtn_ContinuteSkill->SetState( EGUIBS_PushDown, FALSE );
	m_pBtnQuestHelp			= (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but14") );
	m_pBtnKilling			= (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but17") );
	m_pBnPickStrategy		= (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but3") );
	m_pBtnShowHideDialyExp	= (GUIButton*)m_pMSt_MiniMap->GetChild(_T("but13"));
	m_pBtnPaidashi			= (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but16") );
	//m_pBnSinaWeiBo=(GUIButton*)m_pMSt_MiniMap->GetChild( _T("but18") );//add by xp 新浪微薄按钮
	m_pBnSinaWeiBoCard=(GUIButton*)m_pMSt_MiniMap->GetChild( _T("but19") );//add by xp 新浪微薄卡
	
	m_pBnFaceBook  = (GUIButton*)m_pMSt_MiniMap->GetChild( _T("facebookbutt") );

	m_pBnFaceBook->SetInvisible(true);
	int mode = SaveAccount::Inst()->GetLoginMode();
	if( mode == 7 ||	mode == 8)
		m_pBnFaceBook->SetInvisible(false);
    //m_pBt_PetSkillOnline    = (GUIButton*)m_pMSt_MiniMap->GetChild( _T("but16") );

	//m_pSt_MapName_MiniSize  = (GUIStatic*)m_pWnd_MiniSize->GetChild( _T("map_minipic\\mapname") );
	//m_pSt_PosX_MiniSize     = (GUIStatic*)m_pWnd_MiniSize->GetChild( _T("map_minipic\\xzone1") );
	//m_pSt_PosZ_MiniSize     = (GUIStatic*)m_pWnd_MiniSize->GetChild( _T("map_minipic\\yzone1") );
	m_pBn_MaxSize           = (GUIButton*)m_pWnd_MiniSize->GetChild( _T("showmapbutt") );

	m_pBn_EleShow[EMME_Nest]				= (GUIPushButton*)m_pWnd_Set->GetChild( _T("ms_pic\\opt3") );
	m_pBn_EleShow[EMME_NormalMonster]		= (GUIPushButton*)m_pWnd_Set->GetChild( _T("ms_pic\\opt1") );
	m_pBn_EleShow[EMME_Boss]				= (GUIPushButton*)m_pWnd_Set->GetChild( _T("ms_pic\\opt2") );
	m_pBn_EleShow[EMME_NormalNPC]			= (GUIPushButton*)m_pWnd_Set->GetChild( _T("ms_pic\\opt5") );
	m_pBn_EleShow[EMME_AcceptQuestNPC]		= (GUIPushButton*)m_pWnd_Set->GetChild( _T("ms_pic\\opt6") );
	m_pBn_EleShow[EMME_CompleteQuestNPC]	= (GUIPushButton*)m_pWnd_Set->GetChild( _T("ms_pic\\opt7") );
	m_pBn_EleShow[EMME_Teammate]			= (GUIPushButton*)m_pWnd_Set->GetChild( _T("ms_pic\\opt4") );
	m_pBn_EleShow[EMME_OtherPlayer]			= (GUIPushButton*)m_pWnd_Set->GetChild( _T("ms_pic\\opt8") );

	m_pBn_Ok_Set     = (GUIButton*)m_pWnd_Set->GetChild( _T("ms_pic\\surebutt") );
	m_pBn_Cancel_Set = (GUIButton*)m_pWnd_Set->GetChild( _T("ms_pic\\canclebutt") );
	m_pBn_Close_Set  = (GUIButton*)m_pWnd_Set->GetChild( _T("ms_pic\\closebutt") );

	m_pBn_Pos_Ok_Set =		(GUIButton*)m_pWnd_PosQuery->GetChild( _T("manual_backpic\\gobutt") );//putinpic\\putin
	m_pBn_Pos_Cancel_Set =	(GUIButton*)m_pWnd_PosQuery->GetChild( _T("manual_backpic\\closebutt") );
	m_pEd_PosX			 =	(GUIEditBox*)m_pWnd_PosQuery->GetChild( _T("manual_backpic\\putinpic\\putin") );
	m_pEd_PosZ			 =  (GUIEditBox*)m_pWnd_PosQuery->GetChild( _T("manual_backpic\\putinpic0\\putin") );

	// 初始化贴图及数据
	m_pMSt_MiniMap->SetMapRect( rcMap );

	if( !m_pMSt_MiniMap->SetAlphaTex( strAlphaTexFilename ) )
	{
		SafeDestroyUI();
		return FALSE;
	}

	for( UINT i = 0; i< vecControlName.size(); i++ )
	{
		if( !m_pMSt_MiniMap->SetElementImg( i, vecPicFilename[i] ) )
		{
			SafeDestroyUI();
			return FALSE;
		}
	}
	m_pMSt_MiniMap->SetDestMarkImg( strDestMarkControlName );

	m_pWnd_MaxSize->SetInvisible( FALSE );
	m_pWnd_MiniSize->SetInvisible( TRUE );
	m_pWnd_Set->SetInvisible( TRUE );
	m_pWnd_PosQuery->SetInvisible( TRUE );
	m_dwSpaceTime = 0;

	// 设置小地图配置
	for( int i = EGS_MM_Start; i <= EGS_MM_End; i++ )
	{
		m_pMSt_MiniMap->SetElementShow( EGStoEMME(i), GameSet::Inst()->Get(i) );
	}

	if(!TObjRef<NetSession>()->IsEnableCitySys()) m_pBn_CityWarInfo->SetInvisible(TRUE);
	return TRUE;
}

VOID MiniMapFrame::SafeDestroyUI()
{
	if( P_VALID( m_pWnd_Set ) )
	{
		m_pGUI->DestroyWnd(m_pWnd_Set);
		m_pWnd_Set = NULL;		
	}
	if( P_VALID( m_pWnd_PosQuery ) )
	{
		m_pGUI->DestroyWnd(m_pWnd_PosQuery);
		m_pWnd_PosQuery = NULL;		
	}
	if( P_VALID(m_pWnd_MaxSize) )
	{
		m_pGUI->DestroyWnd(m_pWnd_MaxSize);
		m_pWnd_MaxSize = NULL;
	}
	if( P_VALID(m_pWnd_MiniSize) )
	{
		m_pGUI->DestroyWnd(m_pWnd_MiniSize);
		m_pWnd_MiniSize = NULL;
	}
}

DWORD MiniMapFrame::RefreshQueryBtnFlash(tagGameEvent*)
{
	m_pBn_TaskQuery->SetFlash(FALSE);
	m_clickTaskQueTime.savehour = ServerTime::Inst()->GetCurrentHour();
	m_clickTaskQueTime.saveMin = ServerTime::Inst()->GetCurrentMinute();
	return 0;
}

DWORD MiniMapFrame::OnOpenMap(tagGameEvent*)
{
	ResetMapData();
	return 0;
}

DWORD MiniMapFrame::OnLeaveMapArea( tagLeaveMapAreaEvent* pEvent )
{
	ResetMapData();
	return 0;
}

DWORD MiniMapFrame::OnEnterMapArea( tagEnterMapAreaEvent* pEvent )
{
	ResetMapData();
	return 0;
}

DWORD MiniMapFrame::OnSetFlagEvent( tagSetFlagEvent* pEvent )
{
	if( !P_VALID(m_pMSt_MiniMap) )
		return 0;

	m_pMSt_MiniMap->ShowDestMark( MapMgr::Inst()->GetCurMapID(), Vector2( pEvent->x, pEvent->z ) );
	return 0;
}

DWORD MiniMapFrame::OnMouseMoveFinishEvent( tagMouseMoveFinishEvent* pEvent )
{
	m_pMSt_MiniMap->CloseDestMark();
	return 0;
}

//比较是否为活动时间
bool   MiniMapFrame::CompareTime(int SrcHour, int SrcMinute, int DestHour, int DestMinute)
{
	if (((SrcHour * 60 + SrcMinute) - (DestHour * 60 + DestMinute)) > 0)
		return true;
	return false;
}

//固定活动按钮 闪烁提醒玩家
VOID  MiniMapFrame::TaskQueryBtnFlash()
{
	const ActivityMgr::ActivityVector &activities = ActivityMgr::Inst()->GetActivityVector();
	
	bool tempIfFlash = FALSE;
	for ( UINT i= 0; i < activities.size(); ++i)
	{
		const tagActivityProtoData &activitydata = activities[i];
		EWeek curWeek = ServerTime::Inst()->GetCurrentWeekDay();
		INT   curDay  = ServerTime::Inst()->GetCurrentDay();
		INT   curMonth= ServerTime::Inst()->GetCurrentMonth();

		// 不是同一天	
		if (activitydata.week != curWeek%7 )
		{
			if(activitydata.mode == 3&&(curMonth>=activitydata.startmonth&&curMonth<=activitydata.endmonth))   //看看在不在月区间
			{
				if(curMonth==activitydata.endmonth&&curDay>activitydata.endday)          //在月区间中判断在不在天区间
					continue;
			}
			else 
				continue;
		}
		 
	    // 过滤已经结束的任务
		if (CompareTime(    activitydata.endHour, activitydata.endMinute, 
							ServerTime::Inst()->GetCurrentHour(), 
							ServerTime::Inst()->GetCurrentMinute()) == false)
	       continue;
		// 未开时
		else if (CompareTime(   activitydata.startHour, activitydata.startMinute, 
								ServerTime::Inst()->GetCurrentHour(), 
								ServerTime::Inst()->GetCurrentMinute()) == true)
		{
			continue;
		}
		// 进行中
		else
		{
			//开始与否
			bool bStarted = (!CompareTime( activitydata.startHour, activitydata.startMinute,
				                       m_clickTaskQueTime.savehour ,m_clickTaskQueTime.saveMin )) ;
		    //结束与否 
			bool bEnded   =	(!CompareTime( activitydata.endHour, activitydata.endMinute, 
										 m_clickTaskQueTime.savehour ,m_clickTaskQueTime.saveMin )) ;
			if ( bStarted== TRUE && bEnded == FALSE )
			{
				//如果在进行时间内已经点击查看过，也不用闪烁提示
				tempIfFlash = FALSE;
				break;
			} 
			else
			{  
				tempIfFlash = TRUE;
				break;
			}
		}
	}
	if (QuestMgr::Inst()->IsActiveFrameOpened())
	{
		m_pBn_TaskQuery->SetFlash(FALSE);
	}
	else
	{
		m_pBn_TaskQuery->SetFlash(tempIfFlash);
	}

}

VOID MiniMapFrame::Update()
{
	Vector3 vPos = RoleMgr::Inst()->GetLocalPlayer()->GetPos();
	tagPoint ptPos;
	ptPos.x = (LONG)vPos.x;
	ptPos.y = (LONG)vPos.z;
	if( m_ptLocalPlayerPos.x != ptPos.x )
	{
		tstringstream stream;
		stream<<_T("x ")<<ptPos.x/g_nTileSize;
		m_pSt_PosX->SetText( stream.str().c_str() );
		//m_pSt_PosX_MiniSize->SetText( stream.str().c_str() );
		m_pWnd_MiniSize->SetRefresh( TRUE );
		m_pWnd_MaxSize->SetRefresh( TRUE );
	}
	if( m_ptLocalPlayerPos.y != ptPos.y )
	{
		tstringstream stream;
		stream<<_T("z ")<<ptPos.y/g_nTileSize;
		m_pSt_PosZ->SetText( stream.str().c_str() );
		//m_pSt_PosZ_MiniSize->SetText( stream.str().c_str() );
		m_pWnd_MiniSize->SetRefresh( TRUE );
		m_pWnd_MaxSize->SetRefresh( TRUE );
	}

	INT startMin = ServerTime::Inst()->GetCurrentMinute();
	//由活动开始时间特点定的规则，可见activity_proto.xml
	if ( g_bfistLogin == TRUE )
	{
		//初次登陆需要检验
		g_bfistLogin = FALSE;
		TaskQueryBtnFlash();
	} 
	//到10和15倍数时间点，或者是整点过1分的时候检验是否btnFlash	
	else if ( startMin%10 == 0 || startMin == 1 || startMin%15 == 0 )
	{ 
		INT tempSavehour = ServerTime::Inst()->GetCurrentHour();
		INT tempSaveMin  = ServerTime::Inst()->GetCurrentMinute();
		//防止点击时间正好等于刷新检测时间后，接下来的一分钟内按钮持续闪烁
		if ( m_clickTaskQueTime.savehour != tempSavehour|| m_clickTaskQueTime.saveMin != tempSaveMin )
		{
			TaskQueryBtnFlash();
		} 	
	}

	m_dwSpaceTime += Kernel::Inst()->GetDeltaTimeDW();

	return GameFrame::Update();
}

//-----------------------------------------------------------------------------
//! 事件处理
//! 
//-----------------------------------------------------------------------------
BOOL MiniMapFrame::OnWnd_MaxSize(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBn_MiniSize )                                   // 最小化
		{
			m_pWnd_MaxSize->SetInvisible( TRUE );
			m_pWnd_MiniSize->SetInvisible( FALSE );
			m_pWnd_MiniSize->FlipToTop();

			// 传给最小化窗口一个WM_MOUSEMOVE事件,防止不移动鼠标点击最大化按钮(最小化窗口上的)没反应的现象
			m_pGUI->SetActive( m_pBn_MaxSize );
			tagGUIInputMsg msg = pEvent->msg;
			msg.dwType = WM_MOUSEMOVE;
			m_pGUI->OnWindowsMsg( msg.dwType, msg.dwParam1, msg.dwParam2 );
			m_pBn_MaxSize->OnInputMessage( &msg );
		}
		else if( pWnd == m_pBn_Setting )                               // 小地图配置
		{
			m_pWnd_Set->SetInvisible( !m_pWnd_Set->IsInvisible() );
			if( !m_pWnd_Set->IsInvisible() )
			{
				for( int i = 0; i < EMME_Num -1; i++ )
				{
					if( !P_VALID( m_pBn_EleShow[i] ) )
						continue;
					if( m_pMSt_MiniMap->GetElementShow( i ) )
						m_pBn_EleShow[i]->SetState( EGUIBS_PushDown );
					else
						m_pBn_EleShow[i]->SetState( EGUIBS_PopUp );
				}
				m_pWnd_Set->FlipToTop();
			}
		}
		else if( pWnd == m_pBn_PosGoTo )                             // 手动输入坐标查询
		{
			m_pWnd_PosQuery->SetInvisible( !m_pWnd_PosQuery->IsInvisible() );
			if( !m_pWnd_PosQuery->IsInvisible() )
			{
				m_pWnd_PosQuery->FlipToTop();
				m_pGUI->SetActive(m_pEd_PosX);
			}
		
		}
		else if( pWnd == m_pBnFaceBook )                             // facebook
		{
			FaceBookFrame*pFrame = (FaceBookFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("FaceBook") );
			if( !P_VALID(pFrame) )
				pFrame = (FaceBookFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("FaceBook"), _T("FaceBookFrame"), 0 );
		}
		else if( pWnd == m_pBnPickStrategy )                             // 拾取模式
		{
			PickupMgr::Inst()->OpenUI();
		}
		else if ( pWnd == m_pBtnKilling )							// 击杀计数
		{
			tagGameEvent evt(_T("OpenCloseKillingCounter"), NULL);
			m_pMgr->SendEvent(&evt);
		}
		else if( pWnd == m_pBn_AreaMap)                                // 区域地图
		{
			tagOpenCloseMapWndEvent evt;
			evt.bWorldMap = false;
			m_pMgr->SendEvent( &evt );
		}
		else if( pWnd == m_pBn_VipStall )                              // 世界地图
		{
			tagGameEvent evt(_T("OpenVipStall"), NULL);
			m_pMgr->SendEvent(&evt);
		}
		else if (pWnd == m_pBn_OnlineTip)
		{
			OnlineTips *pFrame = (OnlineTips*)(TObjRef<GameFrameMgr>()->GetFrame(_T("OnlineTips")));
			if (!P_VALID(pFrame))
			{
				OnlineTips *pFrame = (OnlineTips*)(TObjRef<GameFrameMgr>()->CreateFrame(GetWorldFrm()->GetName().c_str(), _T("OnlineTips"), _T("OnlineTipsFrame"), 0));
				pFrame->ShowWnd(TRUE);
			}
			else
			{
				if (pFrame->IsShow())
					pFrame->ShowWnd(FALSE);
				else
					pFrame->ShowWnd(TRUE);
			}
		}
		else if( pWnd == m_pBn_TaskQuery )                             // 活动任务查询
		{
			if( QuestMgr::Inst()->IsActiveFrameOpened() )
				QuestMgr::Inst()->CloseActiveFrame();
			else
				QuestMgr::Inst()->OpenActiveFrame();
			m_pGameFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_QueryBtn_Flash"),NULL));
		}
		else if( pWnd == m_pBn_TaskTrace )                             // 任务追踪
		{
			if( QuestMgr::Inst()->IsTrackFrameOpened() )
				QuestMgr::Inst()->CloseTrackFrame();
			else
				QuestMgr::Inst()->OpenTrackFrame();
		}
		else if( pWnd == m_pBn_ZoomIn )                                // 放大
		{
			OnZoomIn_HotKey( 0, TRUE );
		}
		else if( pWnd == m_pBn_ZoomOut )                               // 缩小
		{
			OnZoomOut_HotKey( 0, TRUE );
		}
		else if( pWnd == m_pBn_StyleAction )                           // 个性动作
		{
		//	m_pMgr->SendEvent( &tagGameEvent( _T("OpenClose_StyleAction"), this ) );
			EasyTeamFrame *pFrame = (EasyTeamFrame*)(TObjRef<GameFrameMgr>()->GetFrame(_T("EasyTeam")));
			if(P_VALID(pFrame))
				TObjRef<GameFrameMgr>()->AddToDestroyList(pFrame);
			else
				TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("EasyTeam"), _T("EasyTeamFrame"), 0 );
		}
		else if( pWnd == m_pBtnQuestHelp )								// 帮助
		{
			GlobalHelpFrame* pHelpFrame = (GlobalHelpFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("GlobalHelp"), _T("GlobalHelpFrame"), 0 );
			if( P_VALID(pHelpFrame) )
				pHelpFrame->ShowWnd( TRUE );
		}
		else if (pWnd == m_pBn_CityWarInfo)
		{
			CityWarInfoFrame *pCityWarInfoFrame = (CityWarInfoFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("CityWarInfo"));
			if (!P_VALID(pCityWarInfoFrame))
				TObjRef<GameFrameMgr>()->CreateFrame(_T("World"), _T("CityWarInfo"), _T("CityWarInfoFrame"), 0);
			else
				TObjRef<GameFrameMgr>()->AddToDestroyList(pCityWarInfoFrame);
		}
		else if(pWnd==m_pBnSinaWeiBoCard)//---------------------add by xp 新浪微博媒体卡
		{
			KeyCodeFrame *pKeyCodeFrame = (KeyCodeFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("KeyCode"));
			if (!P_VALID(pKeyCodeFrame))
			{
				pKeyCodeFrame = (KeyCodeFrame*) TObjRef<GameFrameMgr>()->CreateFrame(_T("World"), _T("KeyCode"), _T("KeyCodeFrame"),0);
			}
			if (P_VALID(pKeyCodeFrame))
			{
				pKeyCodeFrame->Show(TRUE);
				pKeyCodeFrame->IsCheckDistance=FALSE;//不用检测与NPC的距离
				pKeyCodeFrame->SetNPC(2000000196);//这里先写死为媒体卡领取人的ID
				//m_pKeyCodeFrame->SetNPC(pEvt->globalID);
				//if (EFNPCT_KeyCodeNPC == pCreatureProto->eFunctionType)
				//{
				//	m_pKeyCodeFrame->SetType(FALSE);
				//}
				//else //if (EFNPCT_NewKeyCodeNPC == pCreatureProto->eFunctionType)
				//{
				//	m_pKeyCodeFrame->SetType(TRUE);
				//}

			}
			//CommKeyCode *pCommKeyCodeFrame=(CommKeyCode*)TObjRef<GameFrameMgr>()->GetFrame(_T("CommKeyCode"));
			//if(!P_VALID(pCommKeyCodeFrame))
			//{
			//	pCommKeyCodeFrame=(CommKeyCode*)TObjRef<GameFrameMgr>()->CreateFrame(_T("World"), _T("CommKeyCode"), _T("CommKeyCode"), 0);
			//	if( P_VALID(pCommKeyCodeFrame) )
			//		pCommKeyCodeFrame->ShowWnd(TRUE);
			//	/*pCommKeyCodeFrame->m_pWndMain->SetInvisible(FALSE);
			//	pCommKeyCodeFrame->m_pGUI->SetActive(pCommKeyCodeFrame->m_pEboxInput);
			//	pCommKeyCodeFrame->m_pEboxInput->SetText(_T(""));*/
			//}
			//else
			//{
			//	/*pCommKeyCodeFrame->m_pWndMain->SetInvisible(TRUE);*/
			//	TObjRef<GameFrameMgr>()->AddToDestroyList(pCommKeyCodeFrame);
			//}
		}//---------------------------------------------------------------------------------add end 
		else if (pWnd == m_pBtnStrengthRanking)
		{
			StrengthRankingFrame *pFrame = (StrengthRankingFrame*)(TObjRef<GameFrameMgr>()->GetFrame(_T("StrengthRanking")));
			if (!P_VALID(pFrame))
			{
				StrengthRankingFrame *pFrame = (StrengthRankingFrame*)(TObjRef<GameFrameMgr>()->CreateFrame(GetWorldFrm()->GetName().c_str(), _T("StrengthRanking"), _T("StrengthRankingFrame"), 0));
			}
			else
			{
				TObjRef<GameFrameMgr>()->AddToDestroyList(pFrame);
			}
		}
		else if(pWnd == m_pBtnShowHideDialyExp)
		{
			TObjRef<GameFrameMgr>()->SendEvent(&tagGameEvent(_T("OpenDialyExpFrame"),NULL));
		}
		else if(pWnd == m_pBtnPaidashi)
		{
			//WinExec("data\\local\\zhCN\\paidashi\\bin\\pai.exe mini", SW_SHOW);
			ShellExecute(NULL, _T("open"), _T("reso\\paidashi\\bin\\pai.exe"), _T("mini"), NULL, SW_SHOW);
		}
		 // 灵兽内挂 //<!F计划中该功能废弃
  //      else if (pWnd == m_pBt_PetSkillOnline)
  //      {
  //          PetSkillAssistanceFrame* pFrame = (PetSkillAssistanceFrame*)(TObjRef<GameFrameMgr>()->GetFrame(_T("PetSkillAssistance")));
  //          if (P_VALID(pFrame))
  //          {
  //              pFrame->ShowWnd(!pFrame->IsOpen());
  //          }
  //      }
		//break;
	case EGUIE_PushButtonDown:
		{
// 			if( pWnd == m_pPBtn_ContinuteSkill )
// 			{
// 				SpecialSkillFrame* pFrame = GetSpecialSkillFrame();
// 				if( P_VALID(pFrame) )
// 				{
// 					//激活
// 					if( pEvent->dwParam1==1 )
// 						pFrame->SetActiveState(TRUE);
// 					else
// 						pFrame->SetActiveState(FALSE);
// 				}
// 			}
		}
		break;
	default:
		break;
	}// switch( pEvent->eEvent )

	return FALSE;
}


BOOL MiniMapFrame::OnWnd_MiniSize(tagGUIEvent* pEvent)
{
	const GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBn_MaxSize )                               //最大化
		{
			m_pWnd_MiniSize->SetInvisible( TRUE );
			m_pWnd_MaxSize->SetInvisible( FALSE );

			// 传给最大化窗口一个WM_MOUSEMOVE事件,防止不移动鼠标点击最小化按钮(最大化窗口上的)没反应的现象
			m_pGUI->SetActive( m_pBn_MiniSize );
			tagGUIInputMsg msg = pEvent->msg;
			msg.dwType = WM_MOUSEMOVE;
			m_pGUI->OnWindowsMsg( msg.dwType, msg.dwParam1, msg.dwParam2 );
			m_pBn_MiniSize->OnInputMessage( &msg );
		}
		break;
	default:
		break;
	}// switch( pEvent->eEvent )

	return FALSE;
}

BOOL MiniMapFrame::OnWnd_PosQuery(tagGUIEvent* pEvent)
{
	const GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBn_Pos_Ok_Set )                              // 确定
		{
			StartPathfinding();
		}
		else if( pWnd == m_pBn_Pos_Cancel_Set )                     // 取消
		{
			m_pWnd_PosQuery->SetInvisible( TRUE );
		}
		break;
	case EGUIE_EditBoxEnter:
		{
			if( pWnd == m_pEd_PosX )
			{
				StartPathfinding();
			}
			else if( pWnd == m_pEd_PosZ )
			{
				StartPathfinding();
			}
		}
		break;
	case EGUIE_KeyDown:
		if( pEvent->msg.dwParam1 == VK_TAB )
		{
			if( m_pGUI->GetActive() == m_pEd_PosX )
			{
				m_pGUI->SetActive(m_pEd_PosZ);
			}
			else if(m_pGUI->GetActive() == m_pEd_PosZ)
			{
				m_pGUI->SetActive(m_pEd_PosX);
			}
		}
		break;
	default:
		break;
	}// switch( pEvent->eEvent )

	return FALSE;


}
BOOL MiniMapFrame::OnWnd_Set(tagGUIEvent* pEvent)
{
	const GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBn_Ok_Set )                              // 确定
		{
			for( int i = 0; i < EMME_Num-1; i++ )
			{
				if( !P_VALID(m_pBn_EleShow[i]) )
					continue;
				m_pMSt_MiniMap->SetElementShow( i, m_pBn_EleShow[i]->GetState() );
			}

			// 保存
			for( int i = EGS_MM_Start; i <= EGS_MM_End; i++ )
			{
				const BOOL bShow = m_pMSt_MiniMap->GetElementShow( EGStoEMME(i) );
				GameSet::Inst()->Set( i, bShow );
			}
			GameSet::Inst()->SaveMiniMapSettings();

			m_pWnd_Set->SetInvisible( TRUE );
		}
		else if( pWnd == m_pBn_Cancel_Set )                     // 取消
		{
			m_pWnd_Set->SetInvisible( TRUE );
		}
		else if( pWnd == m_pBn_Close_Set )                      // 关闭
		{
			m_pWnd_Set->SetInvisible( TRUE );
		}
		break;
	default:
		break;
	}// switch( pEvent->eEvent )

	return FALSE;
}

VOID MiniMapFrame::ResetMapData()
{
	if( !P_VALID( m_pWnd_MaxSize ) )
		return;

	LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
	if( NULL == pLP )
		return;

	const DWORD dwMapID = MapMgr::Inst()->GetCurMapID();
	const tagMapLogicData* pLogicData = MapLogicData::Inst()->FindMapLogicData( dwMapID );
	ASSERT( P_VALID( pLogicData ) );

	MiniMapStatic::tagMapData data;
	const int nAreaIndex = MapMgr::Inst()->GetMapAreaIndex( pLP->GetPos(), pLP->GetRoleSize(), EMA_Display );
	BOOL bShowAreaMap = FALSE;
	if( -1 != nAreaIndex )
	{
		const vector<tagMapArea>& mapArea = MapMgr::Inst()->GetGameMap().GetMapRect();
		if( mapArea[nAreaIndex].dwMiniMapSize != 0 )
			bShowAreaMap = TRUE;
	}
	if( !bShowAreaMap )
	{
		//m_pSt_MapName_MiniSize->SetText( pLogicData->szShowName.c_str() );
		m_pSt_MapName->SetText( pLogicData->szShowName.c_str() );
		MiniMapStatic::tagMapData data;
		data.vWorldSize = pLogicData->vSize * g_nTileSize;
		m_pMSt_MiniMap->ResetMapData( data );

		TCHAR szMapFileName[MAX_PATH];
		_stprintf( szMapFileName, _T("%s\\map\\%s\\%s.tga"), g_strLocalPath.c_str(), pLogicData->szName.c_str(), pLogicData->szName.c_str() );
		m_pMSt_MiniMap->SetColorTex( tstring(szMapFileName) );
	}
	else
	{
		const vector<tagMapArea>& mapArea = MapMgr::Inst()->GetGameMap().GetMapRect();
		const tagMapArea& area = mapArea[nAreaIndex];
		AABBox boxArea = area.box;
		const Vector3 vCenter = area.box.GetCenter();
		const float fHalfSize = max( area.box.max.x - area.box.min.x, area.box.max.z - area.box.min.z ) * 0.5f;
		boxArea.min = Vector3( vCenter.x - fHalfSize, vCenter.y - fHalfSize, vCenter.z - fHalfSize );
		boxArea.max = Vector3( vCenter.x + fHalfSize, vCenter.y + fHalfSize, vCenter.z + fHalfSize );

		data.vWorldPos.x = boxArea.min.x;
		data.vWorldPos.y = boxArea.min.z;
		data.vWorldSize.x = boxArea.max.x - boxArea.min.x;
		data.vWorldSize.y = boxArea.max.z - boxArea.min.z;
		m_pMSt_MiniMap->ResetMapData( data );

		TCHAR szMapFileName[MAX_PATH];
		_stprintf( szMapFileName, _T("%s\\map\\%s\\%s_%x.tga"), g_strLocalPath.c_str(), pLogicData->szName.c_str(), pLogicData->szName.c_str(), area.dwObjID );
		m_pMSt_MiniMap->SetColorTex( tstring( szMapFileName ) );

		const tagMapShowAreaData* pShowAreaData = MapLogicData::Inst()->FindMapShowAreaData( dwMapID, area.dwObjID );
		if( P_VALID( pShowAreaData ) )
		{
			//m_pSt_MapName_MiniSize->SetText( pShowAreaData->szTitle.c_str() );
			m_pSt_MapName->SetText( pShowAreaData->szTitle.c_str() );
		}
	}
}


DWORD MiniMapFrame::OnZoomIn_HotKey( DWORD dwID, BOOL bDown )
{
	if( bDown )
	{
		m_pMSt_MiniMap->SetZoom( m_pMSt_MiniMap->GetZoom() - 0.1f );
		if( m_pMSt_MiniMap->GetZoom() < 0.8f )
			m_pMSt_MiniMap->SetZoom( 0.8f );
	}
	return 0;
}

DWORD MiniMapFrame::OnZoomOut_HotKey( DWORD dwID, BOOL bDown )
{
	if( bDown )
	{
		m_pMSt_MiniMap->SetZoom( m_pMSt_MiniMap->GetZoom() + 0.1f );
		if( m_pMSt_MiniMap->GetZoom() > 1.2f )
			m_pMSt_MiniMap->SetZoom( 1.2f );
	}
	return 0;
}

INT MiniMapFrame::EGStoEMME( const INT nEGS ) const
{
	ASSERT( nEGS >= EGS_MM_Start && nEGS <= EGS_MM_End );
	switch( nEGS )
	{
		case EGS_MM_ShowNatuRes:			return EMME_NatuRes;
		case EGS_MM_ShowNest:				return EMME_Nest;
		case EGS_MM_ShowNormalMonster:		return EMME_NormalMonster;
		case EGS_MM_ShowBoss:				return EMME_Boss;
		case EGS_MM_ShowNormalNPC:			return EMME_NormalNPC;
		case EGS_MM_ShowAcceptQuestNPC:		return EMME_AcceptQuestNPC;
		case EGS_MM_ShowCompleteQuestNPC:	return EMME_CompleteQuestNPC;
		case EGS_MM_ShowTeammate:			return EMME_Teammate;
		case EGS_MM_ShowOtherPlayer:		return EMME_OtherPlayer;
		default: ASSERT(0); return GT_INVALID;
	}
}

DWORD MiniMapFrame::OnNS_CanGoToPos( tagNS_CanGoToPos* pNetCmd,DWORD dwParam)
{
	if(pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowActionCheckMsg(pNetCmd->dwErrorCode);
		return false;
	}
	//不在同一地图中则拒绝寻路
	if (pNetCmd->mapID != MapMgr::Inst()->GetCurMapID())
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("InputMsg_InvalidMap")]);
		return false;
	}
	if(!CombatActionChecker::Inst()->IfCanMove(true))
		return false;

	LocalPlayer* pSelf = RoleMgr::Inst()->GetLocalPlayer();

	Vector2 ptDestMark(static_cast<FLOAT>(pNetCmd->fX*TILE_SCALE),static_cast<FLOAT>(pNetCmd->fZ*TILE_SCALE));
	m_pMSt_MiniMap->ShowDestMark( MapMgr::Inst()->GetCurMapID(), ptDestMark );

	tagMouseMoveEvent pEvent;
	pEvent.start = pSelf->GetPos();
	pEvent.end.x   = static_cast<FLOAT>(pNetCmd->fX*TILE_SCALE);	//格子坐标转换为世界坐标
	pEvent.end.z   = static_cast<FLOAT>(pNetCmd->fZ*TILE_SCALE);	//格子坐标转换为世界坐标
	pEvent.end.y   = MapMgr::Inst()->GetNavMap()->GetCollider()->SafeGetGroundHeight(pEvent.end.x,pEvent.end.z);
	pEvent.bUserCtrl=false;
	pEvent.validDist = 0;
	pSelf->OnGameEvent(&pEvent);

	

	return 0;
}

BOOL MiniMapFrame::EscCancel()
{
	if(m_pWnd_PosQuery->IsInvisible())
		return FALSE;
	m_pWnd_PosQuery->SetInvisible( TRUE );
	return TRUE;
}

VOID MiniMapFrame::StartPathfinding()
{

	if (m_dwSpaceTime < 100)
		return;

	m_pGUI->SetActive(m_pBn_Pos_Ok_Set);

	INT POS_X	= _tcstol(m_pEd_PosX->GetText(),    NULL, 10);
	INT POS_Z	= _tcstol(m_pEd_PosZ->GetText(),	NULL, 10);

	//如果都是0则不发送消息，让用户重新输入
	if (0 == POS_X && 0 == POS_Z)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("InputMsg_NoNumber")]);
		return;
	}
	if (0 == POS_X)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("InputMsg_NoXNumber")]);
		return;
	}
	if (0 == POS_Z)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("InputMsg_NoZNumber")]);
		return;
	}
	if (	POS_X > MAX_INPUT_MAPCOORINATE  
		||	POS_Z > MAX_INPUT_MAPCOORINATE)
	{
		m_pEd_PosX->SetText( _T("") );
		m_pEd_PosZ->SetText( _T("") );
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("InputMsg_InvalidNumber")]);
		return;
	}
	//发送网络消息，让服务器判断指定点是否可到达
	tagNC_CanGoToPos cmd;
	cmd.fX = POS_X;
	cmd.fZ = POS_Z;
	TObjRef<NetSession>()->Send(&cmd);


	m_pWnd_PosQuery->SetInvisible( TRUE );

	m_dwSpaceTime = 0;
}