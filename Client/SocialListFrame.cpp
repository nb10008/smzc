/********************************************************************
	created:	2008/12/10
	created:	10:12:2008   11:32
	filename: 	d:\loong_client\Client\SocialListFrame.cpp
	file path:	d:\loong_client\Client
	file base:	SocialListFrame
	file ext:	cpp
	author:		leili
	
	purpose:	好友列表界面实现
*********************************************************************/

#include "stdafx.h"
#include "SocialListFrame.h"
#include "ChatEvent.h"
#include "PlayerNameTab.h"
#include "ItemMgr.h"
#include "Container.h"
#include "ItemProtoData.h"
#include "TeamEvent.h"
#include "DispRoleInfoFrame.h"
#include "ToolTipFrame.h"
#include "MasterFrame.h"
#include "ChatEvent.h"
#include "MasterEvent.h"
#include "GuildMgr.h"
#include "EasyBuyFrame.h"

const float HIDECHILDTIME = 1.0f;

SocialListFrame::SocialListFrame( void )
: m_Trunk(this)
, m_pWnd(0)
, m_pListFriends(0)
, m_pPshFriend1(0)
, m_pPshFriend2(0)
, m_pPshFriend3(0)
, m_pPshFriend4(0)
, m_pPshBlack(0)
, m_pBtnAddFriend(0)
, m_pBtnAddBlack(0)
, m_pBtnDelete(0)
, m_pBtnClose(0)
, m_pPshSwitchOnline(0)
, m_curListTab(SocialData::ELT_None)
, m_showOutline(false)
, m_curRow(0)
, m_pWndChild1(0)
, m_pListMenu1(0)
, m_pWndChildGroup(0)
, m_pListMenuGroup(0)
, m_clickRow(-1)
, m_pLevelCaption(0)
, m_pWndChildGift(0)
, m_pListMenuGift(0)
, m_bPreHideChild(false)
, m_startHideChildTime(0.0f)
, m_pPshAutoReply(0)
, m_eAddType(EAT_None)
, m_autoReply(false)
, m_bIsInitUI(true)
, m_pBtnCheckMaster(NULL)
{
	m_uiState = EUIS_OutOfChild1 | EUIS_OutOfChild2;
}

SocialListFrame::~SocialListFrame( void )
{

}

BOOL SocialListFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	m_showOutline = (dwParam & 0x11110000) > 0;
	m_autoReply = (dwParam & 0x00001111) > 0;

	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	pMgr->RegisterEventHandle(_T("Social_AddFriend"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnAddFriend));
	pMgr->RegisterEventHandle(_T("Social_AddBlack"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnAddBlack));
	pMgr->RegisterEventHandle(_T("Social_DeleteFriend"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnDeleteFriend));
	pMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnGetRoleName));
	pMgr->RegisterEventHandle(_T("SocialEvent_SetReply"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnSetReply));
	pMgr->RegisterEventHandle(_T("tagRoleGetIDEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnGetRoleID));
	pMgr->RegisterEventHandle(_T("Event_RoleNotExist"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnGetRoleIDError));

	return TRUE;
}

BOOL SocialListFrame::Destroy()
{
	LPVOID pObj = GetObj("SocialSetReplyBox");
	if( P_VALID(pObj) )
		KillObj("SocialSetReplyBox");

	if( P_VALID(m_pWndChildGift) )
	{
		m_pGUI->AddToDestroyList(m_pWndChildGift);
		m_pWndChildGift = NULL;
	}

	if( P_VALID(m_pWndChildGroup) )
	{
		m_pGUI->AddToDestroyList(m_pWndChildGroup);
		m_pWndChildGroup = NULL;
	}

	if( P_VALID(m_pWndChild1) )
	{
		m_pGUI->AddToDestroyList(m_pWndChild1);
		m_pWndChild1 = NULL;
	}

	if( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	m_pMgr->UnRegisterEventHandler(_T("Social_AddFriend"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnAddFriend));
	m_pMgr->UnRegisterEventHandler(_T("Social_AddBlack"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnAddBlack));
	m_pMgr->UnRegisterEventHandler(_T("Social_DeleteFriend"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnDeleteFriend));
	m_pMgr->UnRegisterEventHandler(_T("tagRoleGetNameEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnGetRoleName));
	m_pMgr->UnRegisterEventHandler(_T("SocialEvent_SetReply"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnSetReply));
	m_pMgr->UnRegisterEventHandler(_T("tagRoleGetIDEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnGetRoleID));
	m_pMgr->UnRegisterEventHandler(_T("Event_RoleNotExist"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&SocialListFrame::OnGetRoleIDError));

	return GameFrame::Destroy();
}

VOID SocialListFrame::Update()
{
	GameFrame::Update();

	if( m_bPreHideChild )
	{
		if( Kernel::Inst()->GetAccumTime() - m_startHideChildTime > HIDECHILDTIME )
		{
			m_pWndChildGift->SetInvisible(TRUE);
			m_pWndChildGroup->SetInvisible( TRUE );
			m_pWndChild1->SetInvisible( TRUE );

			m_bPreHideChild = false;
		}
	}

	// 防止滚动菜单
	if( 0 != m_pListMenu1->GetCurrentTextRow() )
		m_pListMenu1->SetCurrentTextRow( 0 );
	if( 0 != m_pListMenuGroup->GetCurrentTextRow() )
		m_pListMenuGroup->SetCurrentTextRow( 0 );
	if( 0 != m_pListMenuGift->GetCurrentTextRow() )
		m_pListMenuGift->SetCurrentTextRow( 0 );
}

BOOL SocialListFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\friends.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	m_pListFriends		= (GUIListBox*)		m_pWnd->GetChild( _T("fripic\\listback\\frilist") );
	m_pPshFriend1		= (GUIPushButton*)	m_pWnd->GetChild( _T("fripic\\fraback\\team1") );
	m_pPshFriend2		= (GUIPushButton*)	m_pWnd->GetChild( _T("fripic\\fraback\\team2") );
	m_pPshFriend3		= (GUIPushButton*)	m_pWnd->GetChild( _T("fripic\\fraback\\team3") );
	m_pPshFriend4		= (GUIPushButton*)	m_pWnd->GetChild( _T("fripic\\fraback\\team4") );
	m_pPshBlack			= (GUIPushButton*)	m_pWnd->GetChild( _T("fripic\\fraback\\shield") );
	m_pPshFoe          = (GUIPushButton*)	m_pWnd->GetChild( _T("fripic\\fraback\\enemy") );
	m_pBtnAddFriend		= (GUIButton*)		m_pWnd->GetChild( _T("fripic\\addfriend") );
	m_pBtnAddBlack		= (GUIButton*)		m_pWnd->GetChild( _T("fripic\\shield") );
	m_pBtnDelete		= (GUIButton*)		m_pWnd->GetChild( _T("fripic\\delfriend") );
	m_pPshSwitchOnline	= (GUIPushButton*)	m_pWnd->GetChild( _T("fripic\\new0") );
	m_pBtnClose			= (GUIButton*)		m_pWnd->GetChild( _T("fripic\\closebutt") );
	m_pLevelCaption		= (GUIStatic*)		m_pWnd->GetChild( _T("fripic\\listback\\word0") );
	m_pPshAutoReply		= (GUIPushButton*)	m_pWnd->GetChild( _T("fripic\\optbutt2") );
	m_pBtnCheckMaster	= (GUIButton*)		m_pWnd->GetChild( _T("fripic\\settlebutt") );
	//m_pBtnCheckMaster->SetInvisible(true);

	m_pListFriends->SetColNum( 8, 20 );
	m_pListFriends->SetColWidth( 1, 105 );	// 名字
	m_pListFriends->SetColWidth( 2, 106 );	// 好友度1
	m_pListFriends->SetColWidth( 3, 1 );	// 好友度2
	m_pListFriends->SetColWidth( 4, 1 );	// 好友度3
	m_pListFriends->SetColWidth( 5, 1 );	// 好友度4
	m_pListFriends->SetColWidth( 6, 1 );	// 好友度5
	m_pListFriends->SetColWidth( 7, 30 );	// 等级

	if( m_showOutline )
		m_pPshSwitchOnline->SetState( EGUIBS_PushDown, FALSE );
	else
		m_pPshSwitchOnline->SetState( EGUIBS_PopUp, FALSE );
	if( m_autoReply )
		m_pPshAutoReply->SetState( EGUIBS_PushDown, FALSE );
	else
		m_pPshAutoReply->SetState( EGUIBS_PopUp, FALSE );
	m_pPshFriend1->SetState( EGUIBS_PushDown, FALSE );

	// 注册事件处理函数
	
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&SocialListFrame::EventHandler));
	EasyBuyFrame* pFrame = (EasyBuyFrame*)m_pMgr->GetFrame(_T("SocialEasyBuyFrame"));
	if( P_VALID(pFrame))
	{
		pFrame->Align();
	}

	m_pWnd->SetInvisible( FALSE );

	strPath = g_strLocalPath + _T("\\ui\\systemexpand.xml");
	XmlElement eleMenu;
	if( !m_pGUI->LoadXml( &eleMenu, "VFS_System", strPath.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml( &eleMenu, "expand", "AlignControl", "\\desktop");

	// 一级子菜单
	m_pGUI->ChangeXml( &eleMenu, "expand", "Name", "SocialMenuList");
	m_pGUI->ChangeXml( &eleMenu, "SocialMenuList\\listbutton1", "AlignControl", "\\desktop\\SocialMenuList");
	m_pWndChild1 = m_pGUI->CreateWnd( _T("\\desktop"), &eleMenu );
	m_pListMenu1 = (GUIListBox*)m_pWndChild1->GetChild( _T("listbutton1") );
	m_pWndChild1->SetInvisible( TRUE );
	m_pGUI->RegisterEventHandler(m_pWndChild1->GetFullName().c_str(), m_Trunk.sfp1(&SocialListFrame::EventHandlerChild1));
	tagPoint ptInside = m_pListMenu1->GetSize();
	GUIWnd* pFather = m_pListMenu1->GetFather();
	tagPoint ptOutSize = pFather->GetSize();
	m_menuEdge = ptOutSize - ptInside;

	// 分组 二级子菜单
	XmlElement eleMenu1;
	if( !m_pGUI->LoadXml( &eleMenu1, "VFS_System", strPath.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml( &eleMenu1, "expand", "AlignControl", "\\desktop");
	m_pGUI->ChangeXml( &eleMenu1, "SocialMenu1", "Name", "SocialMenuGroup");
	m_pGUI->ChangeXml( &eleMenu1, "SocialMenuGroup\\listbutton1", "AlignControl", "\\desktop\\SocialMenuGroup");
	m_pWndChildGroup = m_pGUI->CreateWnd( _T("\\desktop"), &eleMenu1 );
	m_pListMenuGroup = (GUIListBox*)m_pWndChildGroup->GetChild( _T("listbutton1") );
	m_pListMenuGroup->SetText( 0, 0, g_StrTable[ _T("Social_Menu_Group1") ] );
	m_pListMenuGroup->SetText( 1, 0, g_StrTable[ _T("Social_Menu_Group2") ] );
	m_pListMenuGroup->SetText( 2, 0, g_StrTable[ _T("Social_Menu_Group3") ] );
	m_pListMenuGroup->SetText( 3, 0, g_StrTable[ _T("Social_Menu_Group4") ] );
	m_pListMenuGroup->SetText( 4, 0, g_StrTable[ _T("Social_Menu_BlackList") ] );
	m_pWndChildGroup->SetInvisible( TRUE );
	ResizeMenu( m_pListMenuGroup, 5, 54 );
	m_pGUI->RegisterEventHandler(m_pWndChildGroup->GetFullName().c_str(), m_Trunk.sfp1(&SocialListFrame::EventHandlerChildGroup));

	// 礼物 二级子菜单
	XmlElement eleMenu2;
	if( !m_pGUI->LoadXml( &eleMenu2, "VFS_System", strPath.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml( &eleMenu2, "expand", "AlignControl", "\\desktop");
	m_pGUI->ChangeXml( &eleMenu2, "SocialMenuGroup", "Name", "SocialMenuGift");
	m_pGUI->ChangeXml( &eleMenu2, "SocialMenuGift\\listbutton1", "AlignControl", "\\desktop\\SocialMenuGift");
	m_pWndChildGift = m_pGUI->CreateWnd( _T("\\desktop"), &eleMenu2 );
	m_pListMenuGift = (GUIListBox*)m_pWndChildGift->GetChild( _T("listbutton1") );
	m_pWndChildGift->SetInvisible( TRUE );
	m_pGUI->RegisterEventHandler(m_pWndChildGift->GetFullName().c_str(), m_Trunk.sfp1(&SocialListFrame::EventHandlerChildGift));

	return TRUE;
}

BOOL SocialListFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnClose )//关闭窗口
			{
				m_pMgr->SendEvent( &tagGameEvent( _T("Close_Sociality"), this ) );
				m_pMgr->SendEvent( &tagGameEvent( _T("Close_Sociality_UI"), this ) );
			}
			else if( pWnd == m_pBtnCheckMaster )	// 查看师门
			{
				MasterFrame *pFrame = (MasterFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Master")); 
				if( !P_VALID(pFrame) )
				{
					pFrame = (MasterFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("Master"), _T("MasterFrame"), 0 );
				}
			}
			else if( pWnd == m_pBtnAddFriend )//增加好友
			{
				KillObjects();
				CreateObj("Social_AddFriend", "MsgInputBox");
				TObjRef<MsgInputBox>("Social_AddFriend")->Init( g_StrTable[_T("Social_AddFriendCaption")], g_StrTable[_T("Social_AddFriendText")], _T("Social_AddFriend"), GT_INVALID, MBIT_Name, _T(""), TRUE);
			}
			else if( pWnd == m_pBtnAddBlack )//增加黑名单
			{
				KillObjects();
				CreateObj("Social_AddBlack", "MsgInputBox");
				TObjRef<MsgInputBox>("Social_AddBlack")->Init( g_StrTable[_T("Social_AddBlackCaption")], g_StrTable[_T("Social_AddBlackText")], _T("Social_AddBlack"), GT_INVALID, MBIT_Name, _T(""), TRUE);
			}
			else if( pWnd == m_pBtnDelete )//删除好友或黑名单
			{
// 				if( m_clickRow<0
// 					|| (INT)m_pListFriends->GetText(m_clickRow,1)==GT_INVALID )//选中行没有名字，则不进行删除操作
// 					return TRUE;

				if( m_curListTab == SocialData::ELT_BlackList )//删除黑名单
				{
					SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
					SocialData *pData = pMgr->GetData();
					ASSERT( P_VALID(pData) );

					DWORD roleID = pData->GetBlackListID( m_clickRow );
					if( P_VALID(roleID) )
						pMgr->SendDeleteBlack( roleID );
				}
				else if( m_curListTab == SocialData::ELT_FoeList )//删除仇敌名单
				{
					SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
					SocialData *pData = pMgr->GetData();
					ASSERT( P_VALID(pData) );

					DWORD roleID = pData->GetFoeListID( m_clickRow );
					if( P_VALID(roleID) )
						pMgr->SendDeleteFoe( roleID );
				}
				else//删除好友
				{
					TCHAR szText[128]={0};
					_stprintf( szText, g_StrTable[_T("Social_DeleteFriendText")], m_pListFriends->GetText(m_clickRow,1) );
					KillObjects();
					CreateObj("Social_DeleteFriend", "MsgBox");
					TObjRef<MsgBox>("Social_DeleteFriend")->Init( g_StrTable[_T("Social_DeleteFriendCaption")],
						szText,
						_T("Social_DeleteFriend"),
						(MsgBoxFlag)(MBF_OK|MBF_Cancel),
						TRUE);
				}
			}
		}
		break;
	case EGUIE_Drag:
		{
			EasyBuyFrame* pFrame = (EasyBuyFrame*)m_pMgr->GetFrame(_T("SocialEasyBuyFrame"));
			if( P_VALID(pFrame))
			{
				pFrame->Align();
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if( pWnd == m_pPshFriend1 && m_curListTab != SocialData::ELT_Friend1 )//显示好友列表1
			{
				m_curListTab = SocialData::ELT_Friend1;
				m_pBtnAddFriend->SetEnable(true);
				m_pBtnAddBlack->SetEnable(false);
				ShowFriendList( SocialData::ELT_Friend1, m_showOutline );
				break;
			}

			if( pWnd == m_pPshFriend2 && m_curListTab != SocialData::ELT_Friend2 )//显示好友列表2
			{
				m_curListTab = SocialData::ELT_Friend2;
				m_pBtnAddFriend->SetEnable(true);
				m_pBtnAddBlack->SetEnable(false);
				ShowFriendList( SocialData::ELT_Friend2, m_showOutline );
				break;
			}

			if( pWnd == m_pPshFriend3 && m_curListTab != SocialData::ELT_Friend3 )//显示好友列表3
			{
				m_curListTab = SocialData::ELT_Friend3;
				m_pBtnAddFriend->SetEnable(true);
				m_pBtnAddBlack->SetEnable(false);
				ShowFriendList( SocialData::ELT_Friend3, m_showOutline );
				break;
			}

			if( pWnd == m_pPshFriend4 && m_curListTab != SocialData::ELT_Friend4 )//显示好友列表4
			{
				m_curListTab = SocialData::ELT_Friend4;
				m_pBtnAddFriend->SetEnable(true);
				m_pBtnAddBlack->SetEnable(false);
				ShowFriendList( SocialData::ELT_Friend4, m_showOutline );
				break;
			}

			if( pWnd == m_pPshBlack && m_curListTab != SocialData::ELT_BlackList )//显示黑名单
			{
				m_curListTab = SocialData::ELT_BlackList;
				m_pBtnAddFriend->SetEnable(false);
				m_pBtnAddBlack->SetEnable(true);
				ShowBlackList( m_showOutline );
				break;
			}

			if( pWnd == m_pPshFoe && m_curListTab != SocialData::ELT_FoeList ) //显示仇敌
			{
				m_curListTab = SocialData::ELT_FoeList;
				m_pBtnAddFriend->SetEnable(false);
				m_pBtnAddBlack->SetEnable(false);
				ShowFoeList(  );
				break;
			}

			if( pWnd == m_pPshSwitchOnline )//显示/隐藏离线好友
			{
				if( pEvent->dwParam1 == EGUIBS_PushDown )
				{
					m_showOutline = true;
				}
				else
				{
					m_showOutline = false;
				}

				SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
				pMgr->SetShowOutline(m_showOutline);

				RefreshList( m_curListTab );
				break;
			}

			if( pWnd == m_pPshAutoReply )//设置自动回复
			{
				if( m_bIsInitUI )
				{
					m_bIsInitUI = false;
					return TRUE;
				}

				if( pEvent->dwParam1 == EGUIBS_PushDown )
				{
					LPVOID pObj = GetObj("SocialSetReplyBox");
					if( P_VALID(pObj) )
						KillObj("SocialSetReplyBox");

					CreateObj("SocialSetReplyBox", "MsgInputBox");

					TObjRef<MsgInputBox>("SocialSetReplyBox")->Init( 
						g_StrTable[_T("Social_SetReplyCaption")],
						g_StrTable[_T("Social_SetReplyText")],
						_T("SocialEvent_SetReply"),
						GT_INVALID,
						MBIT_Text,
						NULL,
						TRUE);

					SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
					const TCHAR *szReply = pMgr->GetReply();
					if( _tcslen(szReply) < 1 )
					{
						szReply = g_StrTable[_T("Social_DefaultReply")];
						TObjRef<MsgInputBox>("SocialSetReplyBox")->SetContent( szReply, TRUE );
					}
					else
					{
						TObjRef<MsgInputBox>("SocialSetReplyBox")->SetContent( szReply, FALSE );
					}

					pMgr->SetAutoReply(true);
				}
				else
				{
					SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
					pMgr->SetAutoReply(false);
				}

				break;
			}
		}
		break;
	case EGUIE_ItemDblClick:                     //双击好友列表中的一项
		{
			if( pWnd == m_pListFriends)
			{
				if( m_curRow > pEvent->dwParam1 )
				{
					if( m_curListTab != SocialData::ELT_BlackList 
					/* && m_curListTab != SocialData::ELT_FoeList*/ )  //双击左键且不为黑名单
					{
						//发送一条消息通知密语更新窗口并打开
						if ( m_pListFriends->GetText(pEvent->dwParam1,1) != (LPCTSTR)GT_INVALID )
						{
							tagOpenMiniChatEvent evt(_T("tagOpenMiniChatEvent"), this);
							evt.name = m_pListFriends->GetText(pEvent->dwParam1,1);
							if ( !evt.name.empty() )
								m_pMgr->SendEvent(&evt);

							tagPrivateChatEvent evtPri( _T("tagPrivateChatEvent"), this );
							evtPri.dwRoleID = GT_INVALID;
							evtPri.strRoleName = m_pListFriends->GetText(pEvent->dwParam1,1);
							if ( !evtPri.strRoleName.empty() )
								m_pMgr->SendEvent( &evtPri );

						}
					}
				}
			}
		}
		break;
	case EGUIE_ItemClick:
		{
			if( pWnd == m_pListFriends )
			{
				if( m_curRow > pEvent->dwParam1 )
				{
					m_clickRow = pEvent->dwParam1;//记录当前点击的row

					if( pEvent->msg.dwType == WM_RBUTTONUP 
						&& m_curListTab != SocialData::ELT_BlackList //鼠标右键点击 并且 非黑名单 
						&& m_curListTab != SocialData::ELT_FoeList ) //非仇敌名单
					{
						UpdateMenu();

						tagPoint ptLT = m_pWnd->GetClient();
						ptLT = ptLT + m_pListFriends->GetFather()->GetClient();
						ptLT = ptLT + m_pListFriends->GetClient();
						if( pEvent->msg.pt.x > ptLT.x+20
							&& pEvent->msg.pt.x < ptLT.x+135 )
						{
							MoveMenu( m_pListMenu1, pEvent->msg.pt );
							m_pWndChild1->SetInvisible( FALSE );
							m_pWndChild1->FlipToTop();
							m_pGUI->SetActive(m_pListMenu1);
						}
					}
				}
			}
		}
		break;
	//case EGUIE_MoveIntoCtrl:
	//	{
	//		if( pWnd == m_pListFriends )
	//		{
	//			int row = pEvent->dwParam1;
	//			int col = pEvent->dwParam2;
	//			SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
	//			SocialData *pData = pMgr->GetData();

	//			//第二列是友好度
	//			if( col == 2 && P_VALID(pData) )
	//			{
	//				ShowTip( pWnd, NULL );

	//				const tagFriendInfo* pFriendInfo = GetFriendInfo2Tips( m_curListTab, row );
	//				if( !P_VALID(pFriendInfo) )
	//					break;

	//				TCHAR szTips[150] = {0};
	//				memset( szTips, 0, 150 );
	//				_stprintf( szTips, g_StrTable[_T("FriendValue2Social_Tips")], pFriendInfo->dwFriVal );
	//				ShowTip( pEvent->msg.pt + tagPoint(16,16), szTips );
	//			}
	//			else
	//			{
	//				ShowTip( pWnd, NULL );
	//			}
	//		}
	//	}
	//	break;
	default:
		break;
	}

	return FALSE;
}

BOOL SocialListFrame::EventHandlerChild1( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_ItemClick:
		{
			if( pWnd == m_pListMenu1 )
			{
				if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_PlayerInfo] )//玩家资料
				{
					m_pWndChildGift->SetInvisible(TRUE);
					m_pWndChildGroup->SetInvisible( TRUE );
					m_pWndChild1->SetInvisible( TRUE );

					// 名帖
					SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
					if (P_VALID(pMgr))
					{
						SocialData *pData = pMgr->GetData();
						if (P_VALID(pData))
						{
							const tagFriendInfo* info = pData->GetFriendPtrByIndex( m_curListTab, m_clickRow );
							if (P_VALID(info))
							{
								tagOpenRoleInfoEvent e(_T("Open_RoleInfo"), this);
								e.dwRoleID = info->dwFriendID;
								m_pMgr->SendEvent(&e);
							}				
						}
					}

				}
				else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_Chat] )//私聊
				{
					m_pWndChildGift->SetInvisible(TRUE);
					m_pWndChildGroup->SetInvisible( TRUE );
					m_pWndChild1->SetInvisible( TRUE );

					SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
					if (P_VALID(pMgr))
					{
						SocialData *pData = pMgr->GetData();
						if (P_VALID(pData))
						{
							const tagFriendInfo* info = pData->GetFriendPtrByIndex( m_curListTab, m_clickRow );
							if (P_VALID(info))
							{
								tagPrivateChatEvent evt( _T("tagPrivateChatEvent"), this );
								evt.dwRoleID = info->dwFriendID;

								if ( m_pListFriends->GetText(m_clickRow,1) != (LPCTSTR)GT_INVALID )
								{
									evt.strRoleName = m_pListFriends->GetText(m_clickRow,1);
									if ( !evt.strRoleName.empty() )
									{
										m_pMgr->SendEvent(&evt);
									}
								}
							}
						}
					}
					
				}
				else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_InviteGroup] )//邀请组队
				{
					m_pWndChildGift->SetInvisible(TRUE);
					m_pWndChildGroup->SetInvisible( TRUE );
					m_pWndChild1->SetInvisible( TRUE );

					SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
					if (P_VALID(pMgr))
					{
						SocialData *pData = pMgr->GetData();
						if (P_VALID(pData))
						{
							const tagFriendInfo* info = pData->GetFriendPtrByIndex( m_curListTab, m_clickRow );
							if (P_VALID(info))
							{
								tagInviteJoinTeamEvent e(_T("tagInviteJoinTeamEvent"), this);
								e.dwRoleID = info->dwFriendID;
								m_pMgr->SendEvent(&e);
							}
						}
					}	
				}
				else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_ApplyGroup] )//申请组队
				{
					m_pWndChildGift->SetInvisible(TRUE);
					m_pWndChildGroup->SetInvisible( TRUE );
					m_pWndChild1->SetInvisible( TRUE );

					SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
					if (P_VALID(pMgr))
					{
						SocialData *pData = pMgr->GetData();
						if (P_VALID(pData))
						{
							const tagFriendInfo* info = pData->GetFriendPtrByIndex( m_curListTab, m_clickRow );
							if (P_VALID(info))
							{
								tagApplyJoinTeamEvent evt( _T("tagApplyJoinTeamEvent"),this );
								evt.dwRoleID = info->dwFriendID;
								m_pMgr->SendEvent( &evt );
							}
						}
					}
				}
				else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_GiveGift] )//赠送礼物
				{
					if( m_gifts.Size()>0 )//背包里有礼物的弹出礼物子菜单
					{
						m_gifts.ResetIterator();
						DWORD giftID;
						INT i=0;
                        int textSize = 5;
						while( m_gifts.PeekNext(giftID) )
						{
							const tagItemDisplayInfo *pInfo = ItemProtoData::Inst()->FindItemDisplay( giftID );
							if ( P_VALID(pInfo) )
							{
								if (_tcslen(pInfo->szName) > textSize)
									textSize = _tcslen(pInfo->szName);
								m_pListMenuGift->SetText( i++, 0, pInfo->szName );
							}
						}
                      ResizeMenu( m_pListMenuGift, m_gifts.Size(), textSize * 12 );

						tagPoint ptAnchor;
						ptAnchor.x = m_pWndChild1->GetClient().x + m_pWndChild1->GetSize().x - 3;
						ptAnchor.y = m_pWndChild1->GetClient().y + m_pListMenu1->GetClient().y + m_nCurMenuIndex[EMI_GiveGift]*m_pListMenu1->GetRowHeight();
						MoveMenu( m_pListMenuGift, ptAnchor );

						m_pWndChildGroup->SetInvisible( TRUE );
						m_pWndChildGift->SetInvisible( FALSE );
						m_pWndChildGift->FlipToTop();
					}
					else//没有则在聊天窗口提示
					{
						m_pWndChildGift->SetInvisible( TRUE );
						m_pWndChildGroup->SetInvisible( TRUE );
						m_pWndChild1->SetInvisible( TRUE );

						tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
						chatInfo.strInfo = g_StrTable[_T("Social_NoGift")];
						m_pMgr->SendEvent(&chatInfo);
					}
				}
				else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_MoveTo] )//移动至
				{
					tagPoint ptAnchor;
					ptAnchor.x = m_pWndChild1->GetClient().x + m_pWndChild1->GetSize().x - 3;
					ptAnchor.y = m_pWndChild1->GetClient().y + m_pListMenu1->GetClient().y + m_nCurMenuIndex[EMI_MoveTo]*m_pListMenu1->GetRowHeight();
					MoveMenu( m_pListMenuGroup, ptAnchor );

					m_pWndChildGift->SetInvisible( TRUE );
					m_pWndChildGroup->SetInvisible( FALSE );
					m_pWndChildGroup->FlipToTop();
				}
				else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_CopyName] )//复制名称到剪贴板
				{
					m_pWndChildGift->SetInvisible( TRUE );
					m_pWndChildGroup->SetInvisible( TRUE );
					m_pWndChild1->SetInvisible( TRUE );

					LPCTSTR szName = m_pListFriends->GetText( m_clickRow, 1 );

					if( !OpenClipboard(NULL) )
						break;

					EmptyClipboard(); 

					INT nLen = _tcslen(szName);
					HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (nLen+1) * sizeof(TCHAR)); 
					if( NULL == hglbCopy ) 
					{ 
						CloseClipboard();
						break;
					}

					// Lock the handle and copy the text to the buffer. 
					LPTSTR lptstrCopy = (LPTSTR)GlobalLock( hglbCopy ); 
					memcpy(lptstrCopy, szName, nLen * sizeof(TCHAR)); 
					lptstrCopy[nLen] = (TCHAR)0;    // null character 
					GlobalUnlock(hglbCopy); 

					// Place the handle on the clipboard. 
					SetClipboardData(CF_UNICODETEXT, hglbCopy); 
					CloseClipboard();
				}
                else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_ApprenticeToMaster] )
                {
                    SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
					if (P_VALID(pMgr))
					{
						SocialData *pData = pMgr->GetData();
						if (P_VALID(pData))
						{
							tagGetAquireMasterEvent e(_T("tagGetAquireMasterEvent"), this);
							const tagFriendInfo* info = pData->GetFriendPtrByIndex( m_curListTab, m_clickRow );
							if (P_VALID(info))
							{
								e.roleID = info->dwFriendID;
								m_pMgr->SendEvent(&e);
							}
						}
					}
                }
                else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_ToHaveApprentice] )
                {
                    SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
					if (P_VALID(pMgr))
					{
						SocialData *pData = pMgr->GetData();
						if (P_VALID(pData))
						{
							tagAquireApprenticeEvent e(_T("tagAquireApprenticeEvent"), this);
							const tagFriendInfo* info = pData->GetFriendPtrByIndex( m_curListTab, m_clickRow );
							if (P_VALID(info))
							{
								e.roleID = info->dwFriendID;
								m_pMgr->SendEvent(&e);
							}
						}
					}
                }
				else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_InviteAddGuild] )
				{
					m_pWndChildGift->SetInvisible(TRUE);
					m_pWndChildGroup->SetInvisible( TRUE );
					m_pWndChild1->SetInvisible( TRUE );
					SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
					if (P_VALID(pMgr))
					{
						SocialData *pData = pMgr->GetData();
						if (P_VALID(pData))
						{
							const tagFriendInfo* info = pData->GetFriendPtrByIndex( m_curListTab, m_clickRow );
							if (P_VALID(info))
							{
								GuildMgr::Inst()->SendGuildJoinReq( info->dwFriendID );
							}
						}
					}
				}
			}
		}
		break;

	case EGUIE_MoveOutOfCtrl:
		{
			if( pWnd == m_pListMenu1 )
			{
				if( m_uiState & EUIS_OutOfChild1 )
				{
					m_uiState |= EUIS_OutOfChild1;
					if( m_uiState & EUIS_OutOfChild2 )
					{
						// 开始计时
						m_bPreHideChild = true;
						m_startHideChildTime = Kernel::Inst()->GetAccumTime();
					}
				}
			}
			//GUIWnd *pActive = m_pGUI->GetMousePoint();
			//if( pWnd == m_pWndChild1 
			//	&& pActive != m_pWndChildGroup 
			//	&& pActive != m_pListMenuGroup 
			//	&& pActive != m_pWndChildGift 
			//	&& pActive != m_pListMenuGift )
			//{
			//	m_pWndChildGift->SetInvisible( TRUE );
			//	m_pWndChildGroup->SetInvisible( TRUE );
			//	m_pWndChild1->SetInvisible( TRUE );
			//}
		}
		break;

	case EGUIE_MoveIntoCtrl:
		{
			if( pWnd == m_pListMenu1 )
			{
				m_uiState ^= EUIS_OutOfChild1;
				// 停止计时
				m_bPreHideChild = false;
			}
		}
		break;

	case EGUIE_Deactive:
		{
			if( pWnd == m_pListMenu1 )
			{
				m_uiState |= EUIS_OutOfChild1;
				// 停止计时
				m_bPreHideChild = false;
				// 隐藏界面
				m_pWndChildGift->SetInvisible(TRUE);
				m_pWndChildGroup->SetInvisible( TRUE );
				m_pWndChild1->SetInvisible( TRUE );
			}
		}
		break;

	default:
		break;
	}

	return TRUE;
}

BOOL SocialListFrame::EventHandlerChildGift( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_ItemClick:
		{
			if( pWnd == m_pListMenuGift )
			{
				SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
				SocialData *pData = pMgr->GetData();
				ASSERT( P_VALID(pData) );
				const tagFriendInfo* info = pData->GetFriendPtrByIndex( m_curListTab, m_clickRow );

				// 赠送第pEvent->dwParam1个礼物
				m_gifts.ResetIterator();
				DWORD i = 0;
				DWORD giftID;
				while( i++ <= pEvent->dwParam1 )
				{
					m_gifts.PeekNext( giftID );
				}

				ItemContainer *pContainer = ItemMgr::Inst()->GetPocket();

				pMgr->SendGiveGiftTo( info->dwFriendID, pContainer->GetItemSerialID(giftID) );
				
				m_pWndChildGift->SetInvisible( TRUE );
				m_pWndChild1->SetInvisible( TRUE );
			}
		}

	case EGUIE_MoveOutOfCtrl:
		{
			m_pWndChildGift->SetInvisible( TRUE );
			m_uiState |= EUIS_OutOfChild2;
			//GUIWnd *pActive = m_pGUI->GetMousePoint();
			//if( pActive != m_pWndChild1 
			//	&& pActive != m_pListMenu1
			//	&& pActive != m_pWndChildGift )
			//{
			//	m_pWndChild1->SetInvisible( TRUE );
			//}
			//else
			//	m_pGUI->SetActive(m_pWndChild1);
		}
		break;

	case EGUIE_MoveIntoCtrl:
		m_uiState ^= EUIS_OutOfChild2;
		break;
	}

	return TRUE;
}

BOOL SocialListFrame::EventHandlerChildGroup( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch( pEvent->eEvent )
	{
	case EGUIE_ItemClick:
		{
			if( pWnd == m_pListMenuGroup )
			{
				switch( pEvent->dwParam1 )
				{
				case 0:	//移动至好友列表1
					{
						if( m_curListTab == SocialData::ELT_Friend1 )
							return TRUE;

						m_pWndChildGroup->SetInvisible( TRUE );
						m_pWndChild1->SetInvisible( TRUE );

						MoveToList( SocialData::ELT_Friend1, m_clickRow );
					}
					break;

				case 1:	//移动至好友列表2
					{
						if( m_curListTab == SocialData::ELT_Friend2 )
							return TRUE;

						m_pWndChildGroup->SetInvisible( TRUE );
						m_pWndChild1->SetInvisible( TRUE );

						MoveToList( SocialData::ELT_Friend2, m_clickRow );
					}
					break;

				case 2:	//移动至好友列表3
					{
						if( m_curListTab == SocialData::ELT_Friend3 )
							return TRUE;

						m_pWndChildGroup->SetInvisible( TRUE );
						m_pWndChild1->SetInvisible( TRUE );

						MoveToList( SocialData::ELT_Friend3, m_clickRow );
					}
					break;

				case 3:	//移动至好友列表4
					{
						if( m_curListTab == SocialData::ELT_Friend4 )
							return TRUE;

						m_pWndChildGroup->SetInvisible( TRUE );
						m_pWndChild1->SetInvisible( TRUE );

						MoveToList( SocialData::ELT_Friend4, m_clickRow );
					}
					break;

				case 4:	//移动至黑名单
					{
						if( m_curListTab == SocialData::ELT_BlackList )
							return TRUE;

						m_pWndChildGroup->SetInvisible( TRUE );
						m_pWndChild1->SetInvisible( TRUE );

						MoveToList( SocialData::ELT_BlackList, m_clickRow );
					}
					break;

				default:
					break;
				}
			}
		}
		break;

	case EGUIE_MoveOutOfCtrl:
		{
			m_pWndChildGroup->SetInvisible( TRUE );
			m_uiState |= EUIS_OutOfChild2;
			//GUIWnd *pActive = m_pGUI->GetMousePoint();
			//if( pActive != m_pWndChild1 
			//	&& pActive != m_pListMenu1
			//	&& pActive != m_pWndChildGroup )
			//{
			//	m_pWndChild1->SetInvisible( TRUE );
			//}
			//else
			//	m_pGUI->SetActive(m_pWndChild1);
		}
		break;

	case EGUIE_MoveIntoCtrl:
		m_uiState ^= EUIS_OutOfChild2;
		break;

	default:
		break;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// 显示对话窗口
//-----------------------------------------------------------------------------
VOID SocialListFrame::Show( void )
{
	if( P_VALID(m_pWnd) ) 
	{
		m_pWnd->SetInvisible( FALSE );
		m_pWnd->FlipToTop();
	}
}

//-----------------------------------------------------------------------------
// 隐藏对话窗口
//-----------------------------------------------------------------------------
VOID SocialListFrame::Hide( void )
{
	if( P_VALID(m_pWndChildGift) )
		m_pWndChildGift->SetInvisible( TRUE );

	if( P_VALID(m_pWndChildGroup) )
		m_pWndChildGroup->SetInvisible( TRUE );

	if( P_VALID(m_pWndChild1) )
		m_pWndChild1->SetInvisible( TRUE );

	if( P_VALID(m_pWnd) ) 
		m_pWnd->SetInvisible(TRUE);
}

//-----------------------------------------------------------------------------
// 显示好友列表
//-----------------------------------------------------------------------------
VOID SocialListFrame::ShowFriendList( SocialData::EListType listType, bool bShowOutline )
{
	m_pListFriends->Clear();
	m_delayNameRows.clear();
	m_curRow = 0;

	m_pLevelCaption->SetInvisible( FALSE );

	SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
	SocialData *pData = pMgr->GetData();
	ASSERT( P_VALID(pData) );

	const SocialData::FriendsArray *friends;
	pData->SortFriendList( listType );
	if( pData->GetFriendList( listType, &friends ) )
	{
		vEngine::tagRect rcType(0,0,20,20);
		SocialData::FriendsArray::const_iterator iteri;
		for( iteri=friends->begin(); iteri!=friends->end(); ++iteri )
		{
			const tagFriendInfo& friendInfo = *iteri;

			if( !bShowOutline && !friendInfo.bOnline )//不显示离线好友或者黑名单
				break;			

			// todo 根据关系放置相应图片
			if( 0 == friendInfo.nVIPLevel)
			{
				m_pListFriends->SetItemPic( m_curRow, 0, _T("data\\ui\\friend\\L_friends-ral-haoy.bmp"), rcType);
			}
			else if (1 == friendInfo.nVIPLevel)	// 银钻
			{
				m_pListFriends->SetItemPic( m_curRow, 0, _T("data\\ui\\friend\\L_friend_silvervip.bmp"), rcType);
			}
			else if (2 == friendInfo.nVIPLevel)	// 金钻
			{
				m_pListFriends->SetItemPic( m_curRow, 0, _T("data\\ui\\friend\\L_friend_goldvip.bmp"), rcType);
			}
			else if (3 == friendInfo.nVIPLevel)	// 白金
			{
				m_pListFriends->SetItemPic( m_curRow, 0, _T("data\\ui\\friend\\L_friend_supervip.bmp"), rcType);
			}
			

			TCHAR szLevel[5]={0};
			_stprintf(szLevel, _T("%-3d"), friendInfo.nLevel);

			SetFriendValPic(m_curRow, friendInfo.dwFriVal);

			DWORD dwColor = GT_INVALID;//在线名单字体颜色为白色
			if( !friendInfo.bOnline )//离线名单字体颜色为灰色
			{
				dwColor = 0xFF808080;
			}
			LPCTSTR szRoleName = PlayerNameTab::Inst()->FindNameByID( friendInfo.dwFriendID );
			if( _tcslen(szRoleName) > 0 )
				m_pListFriends->SetText( m_curRow, 1, szRoleName, dwColor );
			else
				m_delayNameRows.push_back(make_pair(friendInfo.dwFriendID,m_curRow));

			if (((SocialMgr*)this->GetFather())->GetBloodBrotherTeacherID() == friendInfo.dwFriendID )
			{
				INT day = ((SocialMgr*)this->GetFather())->GetBloodBrotherRemainDays();
				TCHAR szDays[X_SHORT_NAME]={0};
				_stprintf(szDays, g_StrTable[_T("bloodbrotherdays")], day);
				m_pListFriends->SetText( m_curRow, 2, szDays, 0xFF00FF00 );
			}
			m_pListFriends->SetText( m_curRow, 7, szLevel, dwColor );

			m_curRow++;
		}
	}

	tagPoint ptSizeOldListBox = m_pListFriends->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pListFriends->GetRowHeight() * m_curRow;

	m_pListFriends->SetResizable( TRUE );
	m_pListFriends->Resize( ptSizeListBox );
	m_pListFriends->SetResizable( FALSE );

	m_pListFriends->SetRefresh(TRUE);
}

//-----------------------------------------------------------------------------
// 显示黑名单
//-----------------------------------------------------------------------------
VOID SocialListFrame::ShowBlackList( bool bShowOutline )
{
	m_pListFriends->Clear();
	m_delayNameRows.clear();
	m_curRow = 0;

	m_pLevelCaption->SetInvisible( TRUE );

	SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
	SocialData *pData = pMgr->GetData();
	ASSERT( P_VALID(pData) );

	pData->SortBlackList();
	const SocialData::BlackArray& blackArray = pData->GetBlackList();
	SocialData::BlackArray::const_iterator iter;
	for( iter=blackArray.begin(); iter!=blackArray.end(); ++iter )
	{
		LPCTSTR szRoleName = PlayerNameTab::Inst()->FindNameByID( *iter );
		if( _tcslen(szRoleName) > 0 )
			m_pListFriends->SetText( m_curRow, 1, szRoleName );
		else
			m_delayNameRows.push_back( make_pair(*iter,m_curRow) );

		m_curRow++;
	}

	tagPoint ptSizeOldListBox = m_pListFriends->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pListFriends->GetRowHeight() * m_curRow;

	m_pListFriends->SetResizable( TRUE );
	m_pListFriends->Resize( ptSizeListBox );
	m_pListFriends->SetResizable( FALSE );

	m_pListFriends->SetRefresh(TRUE);
}

//-----------------------------------------------------------------------------
// 显示仇敌列表
//-----------------------------------------------------------------------------
VOID SocialListFrame::ShowFoeList()
{
	m_pListFriends->Clear();
	m_delayNameRows.clear();
	m_curRow = 0;

	m_pLevelCaption->SetInvisible( FALSE );

	SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
	SocialData *pData = pMgr->GetData();
	ASSERT( P_VALID(pData) );

	//pData->SortFoeList();
	const SocialData::FoeArray& foeArray = pData->GetFoeList();
	SocialData::FoeArray::const_iterator iter;
	for( iter=foeArray.begin(); iter!=foeArray.end(); ++iter )
	{
		const tagEnemyInfo& enemyInfo = *iter;
		LPCTSTR szRoleName = PlayerNameTab::Inst()->FindNameByID( enemyInfo.dwEnemyID );
		if( _tcslen(szRoleName) > 0 )
		{
			m_pListFriends->SetText( m_curRow, 1, szRoleName );
			tstringstream tss;
			tss<<enemyInfo.nLevel;
			m_pListFriends->SetText( m_curRow,  7, tss.str().c_str() );
			m_curRow++;
		}
		else
			m_delayNameRows.push_back( make_pair(enemyInfo.dwEnemyID,m_curRow) );
	}

	tagPoint ptSizeOldListBox = m_pListFriends->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pListFriends->GetRowHeight() * m_curRow;

	m_pListFriends->SetResizable( TRUE );
	m_pListFriends->Resize( ptSizeListBox );
	m_pListFriends->SetResizable( FALSE );

	m_pListFriends->SetRefresh(TRUE);
}

//-----------------------------------------------------------------------------
// 重新设置菜单尺寸
//-----------------------------------------------------------------------------
VOID SocialListFrame::ResizeMenu( GUIListBox* pListBox, int row, int width )
{	
	tagPoint p(0,0);
	MoveMenu(pListBox, p);
	tagPoint ptSizeListBox = pListBox->GetSize();
	ptSizeListBox.x = width;
	ptSizeListBox.y = pListBox->GetRowHeight() * row;

	GUIWnd* pFather = pListBox->GetFather();
	if( !P_VALID( pFather ) )
		return;

	pFather->SetResizable( TRUE );
	tagPoint ptSizeFather = pFather->GetSize();
	ptSizeFather = ptSizeListBox + m_menuEdge;
	pFather->Resize( ptSizeFather );
	pFather->SetResizable( FALSE );

	pListBox->SetResizable( TRUE );
    pListBox->SetColWidth(0, width);
	pListBox->Resize( ptSizeListBox );
	pListBox->SetResizable( FALSE );
}

//-----------------------------------------------------------------------------
// 移动菜单到pt点
//-----------------------------------------------------------------------------
VOID SocialListFrame::MoveMenu( GUIListBox* pListBox, const tagPoint& ptDest )
{
	GUIWnd* pFather = pListBox->GetFather();
	if( !P_VALID( pFather ) )
		return;

	const tagPoint& oldPt = pFather->GetClient();
	tagPoint movePt = ptDest;
	pFather->SetMovable( TRUE );
	pFather->Move( movePt - oldPt );
	pFather->SetMovable( FALSE );
}

//-----------------------------------------------------------------------------
// 移动某行好友到某个列表
//-----------------------------------------------------------------------------
VOID SocialListFrame::MoveToList( SocialData::EListType listType, int row )
{
	if( m_curListTab == SocialData::ELT_BlackList )//从黑名单中移除
	{
		tagSocialInfoEvent chatInfo(_T("tagSocialInfoEvent"), NULL);
		chatInfo.strInfo = g_StrTable[_T("Social_BlackNotBeFriend")];
		m_pMgr->SendEvent(&chatInfo);
	}
	else//从好友列表移除
	{
		SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
		SocialData *pData = pMgr->GetData();
		ASSERT( P_VALID(pData) );

		const tagFriendInfo* pFriendInfo = pData->GetFriendPtrByIndex( m_curListTab, row );
		if( P_VALID(pFriendInfo) )
		{
			if( listType == SocialData::ELT_BlackList )//移入黑名单
				pMgr->AddBlack( pFriendInfo->dwFriendID );
			else//移入别的好友列表
				pMgr->SendMoveFriend( pFriendInfo->dwFriendID, listType );
		}
	}
}

//-----------------------------------------------------------------------------
// 刷新某个列表，只刷新当前显示的列表
//-----------------------------------------------------------------------------
VOID SocialListFrame::RefreshList( SocialData::EListType listType )
{
	if( listType != m_curListTab )
		return;

	if( listType == SocialData::ELT_BlackList )
	{
		ShowBlackList( m_showOutline );
	}
	else if ( listType == SocialData::ELT_FoeList )
	{
		ShowFoeList();
	}
	else
	{
		ShowFriendList( listType, m_showOutline );
	}
}
//-----------------------------------------------------------------------------
// 刷新当前显示的列表
//-----------------------------------------------------------------------------
VOID SocialListFrame::RefreshList( void )
{
	RefreshList(m_curListTab);
}

//-----------------------------------------------------------------------------
// 刷新是否显示右击的功能菜单
//-----------------------------------------------------------------------------
VOID SocialListFrame::ShowFunList(BOOL bShow)
{
	if( P_VALID(m_pWndChild1) )
	    m_pWndChild1->SetInvisible(!bShow);
}

//-----------------------------------------------------------------------------
// 接受添加好友事件
//-----------------------------------------------------------------------------
DWORD SocialListFrame::OnAddFriend( tagMsgInputBoxEvent* pEvent )
{
	if(MBF_OK == pEvent->eResult)
	{
		DWORD roleID = PlayerNameTab::Inst()->FindIDByName( pEvent->strInputName.c_str() );
		if( P_VALID(roleID) )//输入的角色是否存在
		{
			SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
			pMgr->AddFriend( roleID );
		}
		else
			m_eAddType = EAT_Friend;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 接受屏蔽玩家事件
//-----------------------------------------------------------------------------
DWORD SocialListFrame::OnAddBlack( tagMsgInputBoxEvent* pEvent )
{
	if(MBF_OK == pEvent->eResult)
	{
		DWORD roleID = PlayerNameTab::Inst()->FindIDByName( pEvent->strInputName.c_str() );
		if( P_VALID(roleID) )//输入的角色是否存在
		{
			SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
			pMgr->AddBlack( roleID );
		}
		else
			m_eAddType = EAT_Black;
	}

	return 0;
}

DWORD SocialListFrame::OnGetRoleID( tagRoleGetIDEvent* pEvent )
{
	if( m_eAddType == EAT_None )
		return 0;

	SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
	if( m_eAddType == EAT_Friend )
		pMgr->AddFriend( pEvent->dwRoleID );
	else if ( m_eAddType == EAT_Black )
		pMgr->AddBlack( pEvent->dwRoleID );

	m_eAddType = EAT_None;

	return 0;
}

DWORD SocialListFrame::OnGetRoleIDError( tagGameEvent* pEvent )
{
	m_eAddType = EAT_None;

	return 0;
}

//-----------------------------------------------------------------------------
// 设置自动回复事件
//-----------------------------------------------------------------------------
DWORD SocialListFrame::OnSetReply( tagMsgInputBoxEvent* pEvent )
{
	if(MBF_OK == pEvent->eResult)
	{
		SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
		pMgr->SetReply( pEvent->strInputName.c_str() );
	}

	return 0;
}

//-----------------------------------------------------------------------------
// 接受删除好友事件
//-----------------------------------------------------------------------------
DWORD SocialListFrame::OnDeleteFriend( tagMsgBoxEvent* pEvent )
{
	if(MBF_OK == pEvent->eResult)
	{
		// todo 特殊关系不能被删除，夫妻、师徒、情侣

		SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
		SocialData *pData = pMgr->GetData();
		ASSERT( P_VALID(pData) );
		const tagFriendInfo* pFriendInfo = pData->GetFriendPtrByIndex( m_curListTab, m_clickRow );
		if ( P_VALID(pFriendInfo))
		{
			pMgr->SendDeleteFriend( pFriendInfo->dwFriendID );
		}
	}

	return 0;
}

DWORD SocialListFrame::OnGetRoleName( tagRoleGetNameEvent* pEvent )
{
	std::list<DelayNameRow>::iterator iter;
	for( iter=m_delayNameRows.begin(); iter!=m_delayNameRows.end(); ++iter )
	{
		DelayNameRow& nameRow = *iter;

		if( nameRow.first == pEvent->dwRoleID )//有ID相同的
		{
			if( pEvent->bResult )
			{
				if ( m_curListTab != SocialData::ELT_FoeList )
				{
					m_pListFriends->SetText( nameRow.second, 1, pEvent->strRoleName.c_str());
				}
			}
			else
				m_pListFriends->SetText( nameRow.second, 1, pEvent->strRoleName.c_str(), 0xFF808080);

			m_delayNameRows.erase(iter);
			RefreshList();
			break;
		}
	}

	return 0;
}

VOID SocialListFrame::SetFriendValPic( DWORD row,DWORD friendVal )
{
	vEngine::tagRect rcVal(0,0,18,18);

	const TCHAR szHeartPic[]= _T("data\\ui\\friend\\L_friends-qinmi-heart.bmp");
	const TCHAR szSunPic[]	= _T("data\\ui\\friend\\L_friends-qinmi-sun.bmp");
	const TCHAR szMoonPic[] = _T("data\\ui\\friend\\L_friends-qinmi-moon.bmp");
	const TCHAR szStarPic[] = _T("data\\ui\\friend\\L_friends-qinmi-star.bmp");

	if( friendVal >= 6020 )
	{
		m_pListFriends->SetItemPic( row, 2, szHeartPic, rcVal );
	}
	else if( friendVal >= 5270 )
	{
		m_pListFriends->SetItemPic( row, 2, szSunPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szSunPic, rcVal );
		m_pListFriends->SetItemPic( row, 4, szSunPic, rcVal );
		m_pListFriends->SetItemPic( row, 5, szSunPic, rcVal );
		m_pListFriends->SetItemPic( row, 6, szSunPic, rcVal );
	}
	else if( friendVal >= 4570 )
	{
		m_pListFriends->SetItemPic( row, 2, szSunPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szSunPic, rcVal );
		m_pListFriends->SetItemPic( row, 4, szSunPic, rcVal );
		m_pListFriends->SetItemPic( row, 5, szSunPic, rcVal );
	}
	else if( friendVal >= 3920 )
	{
		m_pListFriends->SetItemPic( row, 2, szSunPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szSunPic, rcVal );
		m_pListFriends->SetItemPic( row, 4, szSunPic, rcVal );
	}
	else if( friendVal >= 3320 )
	{
		m_pListFriends->SetItemPic( row, 2, szSunPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szSunPic, rcVal );
	}
	else if( friendVal >= 2770 )
	{
		m_pListFriends->SetItemPic( row, 2, szSunPic, rcVal );
	}
	else if( friendVal >= 2270 )
	{
		m_pListFriends->SetItemPic( row, 2, szMoonPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szMoonPic, rcVal );
		m_pListFriends->SetItemPic( row, 4, szMoonPic, rcVal );
		m_pListFriends->SetItemPic( row, 5, szMoonPic, rcVal );
		m_pListFriends->SetItemPic( row, 6, szMoonPic, rcVal );
	}
	else if( friendVal >= 1820 )
	{
		m_pListFriends->SetItemPic( row, 2, szMoonPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szMoonPic, rcVal );
		m_pListFriends->SetItemPic( row, 4, szMoonPic, rcVal );
		m_pListFriends->SetItemPic( row, 5, szMoonPic, rcVal );
	}
	else if( friendVal >= 1420 )
	{
		m_pListFriends->SetItemPic( row, 2, szMoonPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szMoonPic, rcVal );
		m_pListFriends->SetItemPic( row, 4, szMoonPic, rcVal );
	}
	else if( friendVal >= 1070 )
	{
		m_pListFriends->SetItemPic( row, 2, szMoonPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szMoonPic, rcVal );
	}
	else if( friendVal >= 770 )
	{
		m_pListFriends->SetItemPic( row, 2, szMoonPic, rcVal );
	}
	else if( friendVal >= 520 )
	{
		m_pListFriends->SetItemPic( row, 2, szStarPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szStarPic, rcVal );
		m_pListFriends->SetItemPic( row, 4, szStarPic, rcVal );
		m_pListFriends->SetItemPic( row, 5, szStarPic, rcVal );
		m_pListFriends->SetItemPic( row, 6, szStarPic, rcVal );
	}
	else if( friendVal >= 320 )
	{
		m_pListFriends->SetItemPic( row, 2, szStarPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szStarPic, rcVal );
		m_pListFriends->SetItemPic( row, 4, szStarPic, rcVal );
		m_pListFriends->SetItemPic( row, 5, szStarPic, rcVal );
	}
	else if( friendVal >= 170 )
	{
		m_pListFriends->SetItemPic( row, 2, szStarPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szStarPic, rcVal );
		m_pListFriends->SetItemPic( row, 4, szStarPic, rcVal );
	}
	else if( friendVal >= 70 )
	{
		m_pListFriends->SetItemPic( row, 2, szStarPic, rcVal );
		m_pListFriends->SetItemPic( row, 3, szStarPic, rcVal );
	}
	else if( friendVal >= 20 )
	{
		m_pListFriends->SetItemPic( row, 2, szStarPic, rcVal );
	}
}

BOOL SocialListFrame::EscCancel()
{
	if( P_VALID(m_pWnd) && !m_pWnd->IsInvisible() )
	{
		m_pMgr->SendEvent( &tagGameEvent( _T("Close_Sociality"), this ) );
		m_pMgr->SendEvent( &tagGameEvent( _T("Close_Sociality_UI"), this ) );
		return TRUE;
	}

	return GameFrame::EscCancel();
}

VOID SocialListFrame::UpdateMenu( void )
{
	for( int i = 0; i < EMI_Num; i++ )
		m_nCurMenuIndex[i] = GT_INVALID;
	INT nMenuIndex = 0;

	SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
	SocialData *pData = pMgr->GetData();
	ASSERT( P_VALID(pData) );
	const tagFriendInfo* info = pData->GetFriendPtrByIndex( m_curListTab, m_clickRow );

	if( info->bOnline )
	{
		m_nCurMenuIndex[EMI_Chat] = nMenuIndex;
		m_pListMenu1->SetText( nMenuIndex++, 0, g_StrTable[ _T("Social_Menu_Talk") ] );
	}

	if( info->bOnline )
	{
		ItemContainer *pContainer = ItemMgr::Inst()->GetPocket();
		pContainer->GetSpecFuncItemTypeList(EISF_FriendGift, m_gifts);
		DWORD dwColor = GT_INVALID;//在线名单字体颜色为白色
		if( m_gifts.Size()==0 )//背包里没有礼物灰色
			dwColor = 0xFF808080;

		m_nCurMenuIndex[EMI_GiveGift] = nMenuIndex;
		m_pListMenu1->SetText( nMenuIndex++, 0, g_StrTable[_T("Social_Menu_GiveGift")], dwColor );
	}

// 	m_nCurMenuIndex[EMI_PlayerInfo] = nMenuIndex;
// 	m_pListMenu1->SetText( nMenuIndex++, 0, g_StrTable[_T("Social_Menu_Info")]);

	if( info->bOnline )
	{
		m_nCurMenuIndex[EMI_InviteGroup] = nMenuIndex;
		m_pListMenu1->SetText( nMenuIndex++, 0, g_StrTable[_T("Social_Menu_InviteGroup")] );

		m_nCurMenuIndex[EMI_ApplyGroup] = nMenuIndex;
		m_pListMenu1->SetText( nMenuIndex++, 0, g_StrTable[_T("Social_Menu_ApplyGroup")] );
	}

	m_nCurMenuIndex[EMI_MoveTo] = nMenuIndex;
	m_pListMenu1->SetText( nMenuIndex++, 0, g_StrTable[_T("Social_Menu_MoveTo")] );

	m_nCurMenuIndex[EMI_CopyName] = nMenuIndex;
	m_pListMenu1->SetText( nMenuIndex++, 0, g_StrTable[_T("Social_Menu_CopyName")] );

    //m_nCurMenuIndex[EMI_ApprenticeToMaster] = nMenuIndex;
    //m_pListMenu1->SetText( nMenuIndex++, 0, g_StrTable[_T("TargetMenu_13")] );

    //m_nCurMenuIndex[EMI_ToHaveApprentice] = nMenuIndex;
    //m_pListMenu1->SetText( nMenuIndex++, 0, g_StrTable[_T("TargetMenu_14")] );

	if( info->bOnline )
	{
		m_nCurMenuIndex[EMI_InviteAddGuild] = nMenuIndex;
		m_pListMenu1->SetText( nMenuIndex++, 0, g_StrTable[_T("TargetMenu_5")] );
	}

	ResizeMenu( m_pListMenu1, nMenuIndex, 72 );
}

const tagFriendInfo* SocialListFrame::GetFriendInfo2Tips( SocialData::EListType eListType, int row )
{
	SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
	SocialData *pData = pMgr->GetData();
	if( !P_VALID(pData) )
		return NULL;

	const SocialData::FriendsArray *friends;
	if( !pData->GetFriendList( eListType, &friends ) )
		return NULL;

	int nRowTmp = 0;
	SocialData::FriendsArray::const_iterator iter;
	for( iter = friends->begin(); iter != friends->end(); ++iter )
	{
		const tagFriendInfo& friendInfo = *iter;
		if( !m_showOutline && !friendInfo.bOnline )			//不显示离线好友或者黑名单
			continue;

		if( nRowTmp == row )
			return &friendInfo;

		nRowTmp++;
	}

	return NULL;
}

VOID  SocialListFrame::KillObjects()
{
	if(P_VALID(GetObj("Social_DeleteFriend")))
		KillObj("Social_DeleteFriend");
	if(P_VALID(GetObj("Social_AddFriend")))
		KillObj("Social_AddFriend");
	if(P_VALID(GetObj("Social_AddBlack")))
		KillObj("Social_AddBlack");

	if( P_VALID(GetObj("IsAddFriendMsgBox")) )
		KillObj("IsAddFriendMsgBox");
	if( P_VALID(GetObj("GiftChoiceBox")) )
		KillObj("GiftChoiceBox");
	if( P_VALID(GetObj("MountInviteBox")) )
		KillObj("MountInviteBox");
	if( P_VALID(GetObj("Social_MoveToBlack")) )
		KillObj("Social_MoveToBlack");

}