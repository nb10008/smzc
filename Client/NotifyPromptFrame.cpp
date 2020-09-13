#include "StdAfx.h"
#include "NotifyPromptFrame.h"
#include "LocalPlayer.h"
#include "MapMgr.h"
#include "UIEvent.h"
#include "RoleMgr.h"
#include "CombatEvent.h"


NotifyPromptFrame::NotifyPromptFrame(void):
m_Trunk(this),
m_pWnd(NULL),
m_pOk(NULL),
m_pStcContent(NULL),
m_pScrollBar(NULL)
{
}

NotifyPromptFrame::~NotifyPromptFrame(void)
{
}

BOOL NotifyPromptFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init( pMgr, pFather, dwParam ) )
		return FALSE;

	return TRUE;
}

BOOL NotifyPromptFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	if( P_VALID(m_pWnd) )
		m_pGUI->DestroyWnd( m_pWnd );

	return bRet;
}

BOOL NotifyPromptFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\common15.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd ) )
		return FALSE;

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&NotifyPromptFrame::EventHandler) );

	m_pOk				= (GUIButton*)m_pWnd->GetChild( _T("comm_pic\\surebutt") );
	m_pStcContent		= (GUIStaticEx*)m_pWnd->GetChild( _T("comm_pic\\back\\win\\word") );
	m_pScrollBar		= (GUIScrollBar*)m_pWnd->GetChild( _T("comm_pic\\back\\win\\scroll") );

	m_pWnd->SetInvisible( TRUE );
	return TRUE;
}


BOOL NotifyPromptFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return TRUE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pOk )
			{
				m_pMgr->AddToDestroyList(this);
			}
		}
		break;
	case EGUIE_StaticExLinkClick:
		{
			if( m_vecIdLinks.size() <=0 && m_vecPosLinks.size() <= 0 )
				return TRUE;
			Navigate( pEvent );
		}
		break;
	default:
		break;
	}

	return TRUE;
}

VOID NotifyPromptFrame::Navigate( tagGUIEvent* pEvent )
{
	if( !P_VALID(pEvent) )
		return;

	LPCTSTR szLink = (LPCTSTR)( pEvent->dwParam2 );
	Vector3 pos;
	DWORD dwNPCTypeID = GT_INVALID;
	tstring mapName;
	bool isMapEx = false;//是否跨场景

	//根据ID寻路
	if( (INT)pEvent->dwParam1 < 1000 )	
	{
		INT index = pEvent->dwParam1 - 1;
		if( index > (INT)m_vecIdLinks.size()-1 )
			return;

		tagIDLink& link = m_vecIdLinks[index];
		mapName = link.mapName;
		IMSG( _T("path map: %s typeid: %d\n"), link.mapName.c_str(), link.typeID );

		//不在本地图，进行跨场景寻路
		if( link.mapName != MapMgr::Inst()->GetCurMapName() )
		{
			QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
			ASSERT( P_VALID(pQuery) );
			dwNPCTypeID = link.typeID;
			isMapEx = true;
			if( QuestMgr::Inst()->MoveToMap(link.mapName.c_str(),pos) != TRUE )
				return;
		}
		else
		{
			//触发器或者其他没有TypeID的东西
			if( link.typeID == 1 )
			{
				QuestData *pData = QuestMgr::Inst()->GetData();
				ASSERT( P_VALID(pData) );
				if( !pData->GetTriggerPosition( szLink, pos ) )
					return;
			}
			else
			{
				QuestQuery *pQuery = QuestMgr::Inst()->GetQuery();
				ASSERT( P_VALID(pQuery) );
				dwNPCTypeID = link.typeID;
				if( !pQuery->GetNPCPosition( link.typeID, pos ) )
					return;
			}
		}
	}
	else
	{
		INT index = pEvent->dwParam1-1000;
		if( index > (INT)m_vecPosLinks.size()-1 )
			return;

		tagPosLink& link = m_vecPosLinks[index];

		//不在本地图，进行跨场景寻路
		if( link.mapName != MapMgr::Inst()->GetCurMapName() )
		{
			if( QuestMgr::Inst()->MoveToMap( link.mapName.c_str(),pos ) != TRUE )
				return;
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

	IMSG( _T("target pos: x-%f,y-%f,z-%f\n"),pos.x,pos.y,pos.z );
	m_pMgr->SendEvent( &event );
}

VOID NotifyPromptFrame::Show( const tstring& strContent )
{
	if( strContent.empty() )
		return;

	m_vecIdLinks.clear();
	m_vecPosLinks.clear();
	m_pStcContent->SetText( _T("") );

	//将描述中的有关NPC,地物，等的导航信息提取并改变现实颜色
	tstring str = strContent;
	QuestMgr::Inst()->ParseQuestView( str, m_vecIdLinks,m_vecPosLinks );
	m_pStcContent->SetText( str.c_str() );

	// 滑动条拖动设置
	tagPoint size = m_pStcContent->GetSize();
	if( m_pStcContent->GetTextTotalHeight() > size.y )
	{
		size.y = m_pStcContent->GetTextTotalHeight();
		m_pStcContent->SetResizable(TRUE);
		m_pStcContent->Resize( size );
		m_pStcContent->SetResizable(FALSE);
	}

	m_pStcContent->SetRefresh(TRUE);

	m_pWnd->SetInvisible( FALSE );
}
