#include "stdafx.h"
#include "MultChatFrame.h"
#include "MultChatMgr.h"
#include "FilterData.h"
#include "MsgBox.h"
#include "ChatEvent.h"
#include "TeamEvent.h"
#include "RoleMgr.h"
#include "SocialMgr.h"
#include "SocialEvent.h"
#include "RichEditBoxForChatInput.h"
#include "RichEditBoxForChatShow.h"
#include "GlintListBox.h"
#include "PlayerNameTab.h"
#include "FilterData.h"

MultChatFrame::MultChatFrame(void)
:m_trunk(this)
,m_pWnd(NULL)
,m_pWndAddNew(NULL)
,m_pWndMenberMenu(NULL)
,m_curRow(NULL)
,m_clickGroupRow(-1)
,m_clickMemberRow(-1)
,m_eClickBtnType(Click_Btn_NULL)
{
}

MultChatFrame::~MultChatFrame(void)
{
}

BOOL MultChatFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
	{
		return FALSE;
	}

	m_pGameFrameMgr->RegisterEventHandle(_T("InputString_MultChatFrame"),(FRAMEEVENTPROC)m_trunk.sfp1(&MultChatFrame::OnInputStringMsgbox));
	m_pGameFrameMgr->RegisterEventHandle(_T("DelChatGroup_MultChatFrame"),(FRAMEEVENTPROC)m_trunk.sfp1(&MultChatFrame::OnMsgBoxClickEvent));
	m_pGameFrameMgr->RegisterEventHandle(_T("QuitChatGroup_MultChatFrame"),(FRAMEEVENTPROC)m_trunk.sfp1(&MultChatFrame::OnMsgBoxClickEvent));

	m_pGUI->RegisterEventHandler( m_pWndAddNew->GetFullName().c_str(),   m_trunk.sfp1(&MultChatFrame::OnWnd_AddNew));
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_trunk.sfp1(&MultChatFrame::EventHandler)); 
	m_pGUI->RegisterEventHandler(m_pWndMenberMenu->GetFullName().c_str(), m_trunk.sfp1(&MultChatFrame::EventHandlerMenberMenu));
	return TRUE;
}

BOOL MultChatFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;

	KillObject();

	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	if( P_VALID(m_pWndAddNew) )
	{
		m_pGUI->AddToDestroyList( m_pWndAddNew );
		m_pWndAddNew = NULL;
	}
	if( P_VALID(m_pWndMenberMenu) )
	{
		m_pGUI->AddToDestroyList( m_pWndMenberMenu );
		m_pWndMenberMenu = NULL;
	}
	m_pGameFrameMgr->UnRegisterEventHandler(_T("InputString_MultChatFrame"),(FRAMEEVENTPROC)m_trunk.sfp1(&MultChatFrame::OnInputStringMsgbox));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("DelChatGroup_MultChatFrame"),(FRAMEEVENTPROC)m_trunk.sfp1(&MultChatFrame::OnMsgBoxClickEvent));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("QuitChatGroup_MultChatFrame"),(FRAMEEVENTPROC)m_trunk.sfp1(&MultChatFrame::OnMsgBoxClickEvent));
	
	return TRUE;
}

BOOL MultChatFrame::EscCancel()
{
	//m_pGameFrameMgr->AddToDestroyList(this);
	
	if( m_pWnd->IsInvisible() )
		return FALSE;

	KillObject();
	m_pWnd->SetInvisible( TRUE );

	return TRUE;
}

void MultChatFrame::Update()
{
	GameFrame::Update();
	
	if ( MultChatMgr::Inst()->IsNeedRefresh() )
	{
		RefreshGroupList();
		RefreshGlint();
		RefreshTitle();
		RefreshMemberList();
		RefreshChatBoard();
		RefreshAddMenberButton();
	}
	
}

BOOL MultChatFrame::ReloadUI()
{

	if (P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\groupchat.xml");
	XmlElement ele;
	if (!m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str()))
		return FALSE;

	//修改输入框的类
	m_pGUI->ChangeXml(&ele, "groupchat_win\\groupchat_pic\\textback0\\editback\\inpupict\\editbox", "ClassName", "RichEditBoxForChatInput");
	//修改显示框的类
	m_pGUI->ChangeXml(&ele, "groupchat_win\\groupchat_pic\\textback\\text", "ClassName", "RichEditBoxForChatShow");				
	m_pGUI->ChangeXml(&ele, "groupchat_win\\groupchat_pic\\grouplistpic\\list", "ClassName", "GlintListBox");	
	
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_BtnNewChatGroup  = (GUIButton*)m_pWnd->GetChild(_T("groupchat_pic\\createbutton"));
	m_BtnDelChatGroup  = (GUIButton*)m_pWnd->GetChild(_T("groupchat_pic\\deletebutton"));
	m_BtnQuitChatGroup = (GUIButton*)m_pWnd->GetChild(_T("groupchat_pic\\quitbutton"));
	m_BtnAddMember	   = (GUIButton*)m_pWnd->GetChild(_T("groupchat_pic\\addmember"));
	m_BtnAddMember->SetEnable(FALSE);
	m_BtnExitFrame	   = (GUIButton*)m_pWnd->GetChild(_T("groupchat_pic\\closebutt"));
	m_BtnSendMessage   = (GUIButton*)m_pWnd->GetChild(_T("groupchat_pic\\textback0\\editback\\send"));
	m_LBGroupNameList  = (GlintListBox*)m_pWnd->GetChild(_T("groupchat_pic\\grouplistpic\\list"));   
	m_LBGroupNameList->SetColWidth(0,131);
	m_LBMemberNameList = (GUIListBox*)m_pWnd->GetChild(_T("groupchat_pic\\memberlistpic\\list"));  
	m_LBMemberNameList->SetColWidth(0,131);
	m_StcShowGroupIn   = (GUIStaticEx*)m_pWnd->GetChild(_T("groupchat_pic\\textback\\sl_title"));
	m_REBChatBlank	   = (RichEditBoxForChatShow*)m_pWnd->GetChild(_T("groupchat_pic\\textback\\text"));
	m_REBLocalMessage  = (RichEditBoxForChatInput*)m_pWnd->GetChild(_T("groupchat_pic\\textback0\\editback\\inpupict\\editbox"));
	m_REBChatBlank->ClsText();
	m_REBLocalMessage->ClsText();	
	m_pWnd->SetInvisible( FALSE );

	//--------------------------添加新成员UI-------------------------------------------
	if (P_VALID(m_pWndAddNew))
		m_pGUI->AddToDestroyList(m_pWndAddNew);
	tstring strPath2 = vEngine::g_strLocalPath + _T("\\ui\\task_female.xml");
	XmlElement ele2;
	if (!m_pGUI->LoadXml(&ele2, "VFS_System", strPath2.c_str()))
		return FALSE;
	m_pWndAddNew = m_pGUI->CreateWnd(_T("\\desktop"), &ele2);

	m_pBn_Ok_Set		= (GUIButton*)m_pWndAddNew->GetChild( _T("task_female_pic\\closebutt0") );
	m_pBn_Cancel_Set	= (GUIButton*)m_pWndAddNew->GetChild( _T("task_female_pic\\closebutt") );
	m_pBn_Close_Set		= (GUIButton*)m_pWndAddNew->GetChild( _T("task_female_pic\\closebutt00") );
	m_pEb_FriendName	= (GUIEditBox*)m_pWndAddNew->GetChild( _T("task_female_pic\\inputpic\\input") );
	m_pCb_Friends_Box	= (GUIComboBox*)m_pWndAddNew->GetChild( _T("task_female_pic\\comboback") );
	m_pStc_Title		= (GUIStatic*)m_pWndAddNew->GetChild( _T("task_female_pic\\word1"));

	m_pCb_Friends_Box->GetListBox()->SetColWidth(0, m_pCb_Friends_Box->GetSize().x);
	m_pWndAddNew->SetInvisible( TRUE );

	//--------------------------右键点击弹出的子菜单-------------------------------------------
	if (P_VALID(m_pWndMenberMenu))
		m_pGUI->AddToDestroyList(m_pWndMenberMenu);
	strPath = g_strLocalPath + _T("\\ui\\systemexpand.xml");
	XmlElement eleMenu;
	if( !m_pGUI->LoadXml( &eleMenu, "VFS_System", strPath.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml( &eleMenu, "expand", "AlignControl", "\\desktop");
	// 一级子菜单
	m_pGUI->ChangeXml( &eleMenu, "expand", "Name", "SocialMenuGrouptChat");
	m_pGUI->ChangeXml( &eleMenu, "SocialMenuGrouptChat\\listbutton1", "AlignControl", "\\desktop\\SocialMenuGrouptChat");

	m_pGUI->ChangeXml( &eleMenu, "SocialMenuGrouptChat", "Size_y", "110");
	m_pGUI->ChangeXml( &eleMenu, "SocialMenuGrouptChat\\listbutton1", "Size_y", "80");

	m_pWndMenberMenu = m_pGUI->CreateWnd( _T("\\desktop"), &eleMenu );
	m_pListMenberMenu = (GUIListBox*)m_pWndMenberMenu->GetChild( _T("listbutton1") );
	m_pWndMenberMenu->SetInvisible( TRUE );

	return TRUE;
}

VOID MultChatFrame::ShowWnd( BOOL bVal )
{ 
	if(P_VALID(m_pWnd)) 
	{
		m_pWnd->FlipToTop();
		m_pWnd->SetInvisible(!bVal);
	}
}

BOOL MultChatFrame::FrameIsInvisible()
{
	if(P_VALID(m_pWnd)) 
	{
		return m_pWnd->IsInvisible();
	}
	return FALSE;
}

DWORD MultChatFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			//退出群聊界面
			if( pWnd == m_BtnExitFrame )
			{
				 KillObject();
				 m_pWnd->SetInvisible( TRUE );
			}
			//创建聊天群
			else if( pWnd == m_BtnNewChatGroup )
			{
				m_eClickBtnType = Click_Btn_Create;

				KillObject();
				CreateObj("NewChatGroup", "MsgInputBox");
				TObjRef<MsgInputBox>("NewChatGroup")->Init( 
						_T(""), g_StrTable[_T("NewChatGroup_String")],       
						_T("InputString_MultChatFrame"), 
						GT_INVALID,  MBIT_Name, NULL, FALSE );			
			}
			//删除聊天群
			else if( pWnd == m_BtnDelChatGroup )
			{
				m_eClickBtnType = Click_Btn_Delete;

				KillObject();
				CreateObj("DelChatGroup", "MsgBox");
				TObjRef<MsgBox>("DelChatGroup")->Init( 
					_T(""), g_StrTable[_T("DelChatGroup_String")],       
					_T("DelChatGroup_MultChatFrame"),(MsgBoxFlag)(MBF_OK|MBF_Cancel), FALSE );
				
			}
			//退出聊天群
			else if( pWnd == m_BtnQuitChatGroup )
			{
				m_eClickBtnType = Click_Btn_Quit;

				KillObject();
				CreateObj("QuitChatGroup", "MsgBox");
				TObjRef<MsgBox>("QuitChatGroup")->Init( 
					_T(""), g_StrTable[_T("QuitChatGroup_String")],       
					_T("QuitChatGroup_MultChatFrame"),(MsgBoxFlag)(MBF_OK|MBF_Cancel), FALSE );
			}
			//添加新成员
			else if( pWnd == m_BtnAddMember )
			{
				DWORD dwCurPickGroupID = MultChatMgr::Inst()->GetCurPickGroupID();
				INT nMenber = MultChatMgr::Inst()->GetMemberNum(dwCurPickGroupID);
				if ( nMenber < GROUP_MAX_MEMBER_NUM )
				{
					KillObject();
					m_pWndAddNew->SetInvisible(FALSE);
					m_pGUI->SetActive(m_pWndAddNew);
				}
				else
				{
					KillObject();
					CreateObj("ChatGroupMenberOverbrim", "MsgBoxEx");
					TObjRef<MsgBox>("ChatGroupMenberOverbrim")->Init( 
						_T(""), g_StrTable[_T("ChatGroupMenberOverbrim")],       
						_T("ChatGroupMenberOverbrim") );
				}
			}
			//发送消息
			else if( pWnd == m_BtnSendMessage )
			{
				if ( MultChatMgr::Inst()->GetCurPickGroupID() != GT_INVALID )
				{
					INT len = 0;
					tstring sendMsg = m_REBLocalMessage->GetText(len);
					FilterData::Inst()->WordsFilteror(sendMsg);    //屏蔽字
					DeleteRETURNS(sendMsg);
					len = _tcslen(sendMsg.c_str());
					if ( len > MAX_CHAT_LEN )
					{
						KillObject();
						CreateObj("GroupChat_Msg_Is_Too_Long", "MsgBoxEx");
						TObjRef<MsgBoxEx>("GroupChat_Msg_Is_Too_Long")->Init( 
							_T(""), g_StrTable[_T("GroupChat_Msg_Is_Too_Long")],       
							_T("GroupChat_Msg_Is_Too_Long") );
					}
					else
					{
						MultChatMgr::Inst()->SendMessage( sendMsg, len );
						m_REBLocalMessage->ClsText();
					}
				}
			}
		}
		break;
	case EGUIE_EditBoxEnter:
		{
			if( pWnd == m_REBLocalMessage )
			{
				if ( MultChatMgr::Inst()->GetCurPickGroupID() != GT_INVALID )
				{
					INT len = 0;
					tstring sendMsg = m_REBLocalMessage->GetText(len);
					FilterData::Inst()->WordsFilteror(sendMsg);    //屏蔽字
					DeleteRETURNS(sendMsg);
					len = _tcslen(sendMsg.c_str());
					if ( len > MAX_CHAT_LEN )
					{
						KillObject();
						CreateObj("GroupChat_Msg_Is_Too_Long", "MsgBoxEx");
						TObjRef<MsgBoxEx>("GroupChat_Msg_Is_Too_Long")->Init( 
							_T(""), g_StrTable[_T("GroupChat_Msg_Is_Too_Long")],       
							_T("GroupChat_Msg_Is_Too_Long") );
					}
					else
					{
						MultChatMgr::Inst()->SendMessage( sendMsg, len );
						m_REBLocalMessage->ClsText();
					}
				}
			}
		}
		break;
	case EGUIE_ItemDblClick:                     //双击好友列表中的一项
		{
			//if( pWnd == m_pListFriends)
			//{
			//	if( m_curRow > pEvent->dwParam1 )
			//	{
			//		if( m_curListTab != SocialData::ELT_BlackList )  //双击左键且不为黑名单
			//		{
			//			//发送一条消息通知密语更新窗口并打开
			//			tagOpenMiniChatEvent evt(_T("tagOpenMiniChatEvent"), this);
			//			evt.name = m_pListFriends->GetText(pEvent->dwParam1,1);
			//			m_pMgr->SendEvent(&evt);
			//		}
			//	}
			//}
		}
		break;
	case EGUIE_ItemClick:
		{
			if( pWnd == m_LBGroupNameList )
			{
				DWORD curRow = MultChatMgr::Inst()->GetGroupIDList().size();
				if( curRow > pEvent->dwParam1
				 && pEvent->msg.dwType == WM_LBUTTONUP)
				{
					m_clickGroupRow = (INT)pEvent->dwParam1;//记录当前点击的row   
					m_LBGroupNameList->SetCurSelectedRow(m_clickGroupRow);
					MultChatMgr::Inst()->SetCurPickGroupID(m_clickGroupRow);
					DWORD dwPickGroupID = MultChatMgr::Inst()->GetCurPickGroupID();
					MultChatMgr::Inst()->DelGlintGroup(dwPickGroupID);
					RefreshTitle();
					RefreshGlint();
					RefreshMemberList();
					RefreshChatBoard();
					RefreshAddMenberButton();
					m_LBGroupNameList->SetRefresh(TRUE);
				}
			}
			else if( pWnd == m_LBMemberNameList )
			{
				DWORD curRow = MultChatMgr::Inst()->GetMemberIDList().size();
				if( curRow > pEvent->dwParam1
				&& (pEvent->msg.dwType == WM_LBUTTONUP||pEvent->msg.dwType == WM_RBUTTONUP) )
				{
					m_clickMemberRow = (INT)pEvent->dwParam1;//记录当前点击的row
					m_LBMemberNameList->SetCurSelectedRow(m_clickMemberRow);
					MultChatMgr::Inst()->SetCurPickMemberID(m_clickMemberRow);
					m_pWndMenberMenu->SetInvisible( TRUE );
				}
				DWORD  dwCurPickMemberID = MultChatMgr::Inst()->GetCurPickMemberID();
				DWORD  dwLocalRoleID = RoleMgr::Inst()->GetLocalPlayerID();
				if( pEvent->msg.dwType == WM_RBUTTONUP 
				 && P_VALID( m_LBMemberNameList->GetText( m_clickMemberRow,0 ) ) 
				 && dwCurPickMemberID != dwLocalRoleID )//鼠标右键点击、有内容且非自己
				{
					tagPoint ptLT = m_pWnd->GetClient();
					ptLT = ptLT + m_LBMemberNameList->GetFather()->GetClient();
					ptLT = ptLT + m_LBMemberNameList->GetClient();
					if( pEvent->msg.pt.x > ptLT.x+20
					 && pEvent->msg.pt.x < ptLT.x+135 )
					{
						//设置菜单内容
						m_pListMenberMenu->SetText( 0, 0, g_StrTable[_T("Menu_String_Pravite_Chat")] );
						m_pListMenberMenu->SetText( 1, 0, g_StrTable[_T("Menu_String_Add_Friend")] );
						m_pListMenberMenu->SetText( 2, 0, g_StrTable[_T("Menu_String_Invite_Join")] );
						m_pListMenberMenu->SetText( 3, 0, g_StrTable[_T("Menu_String_Kick_Out")] );

						//移动菜单到pt点
						GUIWnd* pFather = m_pListMenberMenu->GetFather();
						if( !P_VALID( pFather ) )
							return 0;
						const tagPoint& oldPt = pFather->GetClient();
						tagPoint movePt = pEvent->msg.pt;
						pFather->SetMovable( TRUE );
						pFather->Move( movePt - oldPt );
						pFather->SetMovable( FALSE );

						m_pWndMenberMenu->SetInvisible( FALSE );
						m_pWndMenberMenu->FlipToTop();
						m_pGUI->SetActive(m_pWndMenberMenu);
						m_pGUI->SetActive(m_pListMenberMenu);
					}
				}
			}
		}
		break;	
	default :
		break;
	}
	return 0;
}

DWORD MultChatFrame::EventHandlerMenberMenu(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_ItemClick:
		{
			if( pEvent->dwParam1 == 0 )//私聊
			{
				SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
				SocialData *pData = pMgr->GetData();
				ASSERT( P_VALID(pData) );
				//const tagFriendInfo* info = pData->GetFriendPtrByIndex( m_curListTab, m_clickRow );
				tagPrivateChatEvent evt( _T("tagPrivateChatEvent"), this );
				evt.dwRoleID = MultChatMgr::Inst()->GetCurPickMemberID();
				evt.strRoleName = MultChatMgr::Inst()->MemberID2Name( evt.dwRoleID );
				if ( evt.dwRoleID != GT_INVALID )
				{
					m_pMgr->SendEvent( &evt );
				}
				if (P_VALID(m_pWndMenberMenu))
				{
					m_pWndMenberMenu->SetInvisible(TRUE);
				}
			}
			else if( pEvent->dwParam1 == 1 )//加为好友
			{
				tagAddFriendEvent evt( _T("SocialEvent_AddFriend"), this );
				evt.roleID = MultChatMgr::Inst()->GetCurPickMemberID();
				if ( evt.roleID != GT_INVALID )
				{
					m_pMgr->SendEvent( &evt );
				}
				if (P_VALID(m_pWndMenberMenu))
				{
					m_pWndMenberMenu->SetInvisible(TRUE);
				}
			}
			else if( pEvent->dwParam1 == 2 )//邀请组队
			{
				SocialMgr *pMgr = static_cast<SocialMgr *>(m_pFather);
				SocialData *pData = pMgr->GetData();
				ASSERT( P_VALID(pData) );
				tagInviteJoinTeamEvent e(_T("tagInviteJoinTeamEvent"), this);
				e.dwRoleID = MultChatMgr::Inst()->GetCurPickMemberID();
				if ( e.dwRoleID != GT_INVALID )
				{
					m_pMgr->SendEvent(&e);
				}
				if (P_VALID(m_pWndMenberMenu))
				{
					m_pWndMenberMenu->SetInvisible(TRUE);
				}
			}
			else if( pEvent->dwParam1 == 3 )//踢出群聊
			{
				MultChatMgr::Inst()->KickMember(m_clickMemberRow);
				m_eClickBtnType = Click_Btn_Kick;
				if (P_VALID(m_pWndMenberMenu))
				{
					m_pWndMenberMenu->SetInvisible(TRUE);
				}
			}
		}
		break;	
	case EGUIE_MoveOutOfCtrl:
		{
			if (P_VALID(m_pWndMenberMenu))
			{
				m_pWndMenberMenu->SetInvisible(TRUE);
			}
		}
		break;
	default :
		break;
	}
	return 0;
}
DWORD MultChatFrame::OnWnd_AddNew(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;
	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBn_Ok_Set )
		{
			DWORD dwFriendID = GT_INVALID;

			LPCTSTR name = m_pEb_FriendName->GetText();
			if( _tcslen(name) <= 0 )
			{
				return 0;
			}
			DWORD roleID = PlayerNameTab::Inst()->FindIDByName(name);
			if( roleID != GT_INVALID && roleID != RoleMgr::Inst()->GetLocalPlayerID() )
			{
				MultChatMgr::Inst()->InviteMember(roleID);
			}
			m_pEb_FriendName->SetText(_T(""));
			m_curRow = 0;
			m_pWndAddNew->SetInvisible( TRUE );
			m_pGUI->SetActive(m_pWnd);
		}
		else if( pWnd == m_pBn_Cancel_Set )
		{
			m_pWndAddNew->SetInvisible( TRUE );
			m_pGUI->SetActive(m_pWnd);
		}
		else if( pWnd == m_pBn_Close_Set )
		{
			m_pWndAddNew->SetInvisible( TRUE );
			m_pGUI->SetActive(m_pWnd);
		}
		else if( pWnd == m_pCb_Friends_Box->GetButton() )
		{
			m_pGUI->SetActive( m_pCb_Friends_Box->GetListBox() );
			m_curRow = 0;
			m_pCb_Friends_Box->GetListBox()->Clear();

			std::vector<LPCTSTR> vecFriendsList = MultChatMgr::Inst()->GetFriendsNameList();
			std::vector<LPCTSTR>::iterator  iterFriends = vecFriendsList.begin();
			for ( ; iterFriends != vecFriendsList.end(); iterFriends++ )
			{
				LPCTSTR szRoleName = (*iterFriends);
				DWORD dwColor = GT_INVALID;		//在线名单字体颜色为白色
				if( _tcslen(szRoleName) > 0 )
					m_pCb_Friends_Box->GetListBox()->SetText( m_curRow, 0, szRoleName, dwColor );
				m_curRow++;
			}

			tagPoint ptSizeListBox = m_pCb_Friends_Box->GetListBox()->GetSize();
			ptSizeListBox.y = m_pCb_Friends_Box->GetListBox()->GetRowHeight() * m_curRow;
			m_pCb_Friends_Box->GetListBox()->SetResizable( TRUE );
			m_pCb_Friends_Box->GetListBox()->Resize( ptSizeListBox );
			m_pCb_Friends_Box->GetListBox()->SetResizable( FALSE );
			m_pCb_Friends_Box->GetListBox()->SetRefresh(TRUE);
		}
		break;
	case EGUIE_ItemClick:
		if( pWnd == m_pCb_Friends_Box->GetListBox() )
		{
			std::vector<LPCTSTR> vecFriendsList = MultChatMgr::Inst()->GetFriendsNameList();
			if (vecFriendsList.empty())
				break;
			if (vecFriendsList.size() >= pEvent->dwParam1)
			{
				LPCTSTR RoleName = vecFriendsList[pEvent->dwParam1];
				m_pEb_FriendName->SetText( RoleName );
			}	
			m_pGUI->SetActive(m_pWnd);
		}	
		break;
	default:
		break;
	}
	
	return TRUE;

}

DWORD MultChatFrame::OnInputStringMsgbox(tagMsgInputBoxEvent* pGameEvent)
{
	if( MBF_OK == pGameEvent->eResult 
		&& MBIT_Name == pGameEvent->eInputType
		&& m_eClickBtnType == Click_Btn_Create )
	{
		tstring tstrInput = pGameEvent->strInputName;
		if ( _tcslen( tstrInput.c_str() ) <= MAX_GROUPCHAT_LEN - 1 )
		{
			MultChatMgr::Inst()->CreateNewGroup(tstrInput);
		}
		else
		{
			if(P_VALID(GetObj("GroupChat_Msg_Is_Too_Long")))
				KillObj("GroupChat_Msg_Is_Too_Long");
			CreateObj("Input_GroupName_Is_Too_Long", "MsgBoxEx");
			TObjRef<MsgBoxEx>("Input_GroupName_Is_Too_Long")->Init( 
				_T(""), g_StrTable[_T("Input_GroupName_Is_Too_Long")],       
				_T("Input_GroupName_Is_Too_Long") );
		}
	}
	m_eClickBtnType = Click_Btn_NULL;
	return 0;
}

DWORD   MultChatFrame::OnMsgBoxClickEvent(tagMsgBoxEvent* pGameEvent)
{
	if ( m_eClickBtnType == Click_Btn_NULL )
	{
		return 0;
	}
	else if( MBF_OK == pGameEvent->eResult && m_eClickBtnType == Click_Btn_Delete )
	{
		MultChatMgr::Inst()->DeleteGurGroup();
		DWORD dwPickGroupID = MultChatMgr::Inst()->GetCurPickGroupID();
		MultChatMgr::Inst()->DelGlintGroup(dwPickGroupID);
	}
	else if( MBF_OK == pGameEvent->eResult && m_eClickBtnType == Click_Btn_Quit )
	{
		MultChatMgr::Inst()->QuitCurGroup();
		DWORD dwPickGroupID = MultChatMgr::Inst()->GetCurPickGroupID();
		MultChatMgr::Inst()->DelGlintGroup(dwPickGroupID);
	}
	m_eClickBtnType = Click_Btn_NULL;
	return 0;
}

VOID  MultChatFrame::KillObject()
{
	if (P_VALID(m_pWndAddNew))
	{
		m_pWndAddNew->SetInvisible(TRUE);
	}
	if (P_VALID(m_pWndMenberMenu))
	{
		m_pWndMenberMenu->SetInvisible(TRUE);
	}
	if(P_VALID(GetObj("NewChatGroup")))
		KillObj("NewChatGroup");
	if(P_VALID(GetObj("DelChatGroup")))
		KillObj("DelChatGroup");
	if(P_VALID(GetObj("QuitChatGroup")))
		KillObj("QuitChatGroup");
	if(P_VALID(GetObj("No_Right_To_Delete")))
		KillObj("No_Right_To_Delete");
	if(P_VALID(GetObj("Input_GroupName_Is_Too_Long")))
		KillObj("Input_GroupName_Is_Too_Long");
	if(P_VALID(GetObj("GroupChat_Msg_Is_Too_Long")))
		KillObj("GroupChat_Msg_Is_Too_Long");
	if(P_VALID(GetObj("ChatGroupMenberOverbrim")))
		KillObj("ChatGroupMenberOverbrim");

}

VOID   MultChatFrame::RefreshTitle()
{
	DWORD dwCurGroupID = MultChatMgr::Inst()->GetCurPickGroupID();
	const TCHAR*  szGroupName = MultChatMgr::Inst()->GroupID2Name(dwCurGroupID);
	TCHAR temp[128];
	wsprintf(temp, g_StrTable[_T("Current_Chat_Group_Name")], szGroupName );
	m_StcShowGroupIn->SetText(temp);
	m_StcShowGroupIn->SetRefresh(TRUE);
	m_BtnAddMember->SetRefresh(TRUE);
}

VOID   MultChatFrame::RefreshGlint()
{
	INT curRow = 0;
	std::vector<DWORD> GroupIDList = MultChatMgr::Inst()->GetGroupIDList();
	curRow = (INT)GroupIDList.size();
	std::vector<DWORD>& vecGroup = MultChatMgr::Inst()->GetGroupIDList();
	for (INT i=0; i<curRow; i++)
	{
		BOOL b = MultChatMgr::Inst()->IfNeedGlint(vecGroup[i]);
		if ( b == TRUE )
		{
			m_LBGroupNameList->AddClintRow( i );
		}
		else
		{
			m_LBGroupNameList->StopClintRow( i );
		}
	}
	if ( Click_Btn_Create == m_eClickBtnType )
	{
		m_LBGroupNameList->SetCurSelectedRow(curRow-1);
	}
	else if ( Click_Btn_Delete == m_eClickBtnType 
		|| Click_Btn_Quit == m_eClickBtnType )
	{
		m_LBGroupNameList->SetCurSelectedRow(0);
	}
	m_LBGroupNameList->SetRefresh(TRUE);
}

VOID  MultChatFrame::RefreshGroupList()
{
	INT curRow = 0;
	m_LBGroupNameList->Clear();
	std::vector<DWORD> GroupIDList = MultChatMgr::Inst()->GetGroupIDList();
	if (!GroupIDList.empty())
	{
		m_BtnDelChatGroup->SetEnable(TRUE);
		m_BtnQuitChatGroup->SetEnable(TRUE);
		m_BtnSendMessage->SetEnable(TRUE);
		std::vector<DWORD>::const_iterator cIter = GroupIDList.begin();
		for ( ; cIter != GroupIDList.end(); cIter++ )
		{
			LPCTSTR szGroupName = MultChatMgr::Inst()->GroupID2Name(*cIter);
			if ( szGroupName == NULL)
			{
				continue;
			}
			if ( _tcslen(szGroupName) > 0 )
			{
				DWORD dwGroupID = (*cIter);
				DWORD dwGroupMaster = MultChatMgr::Inst()->GetGrouptMasterID(dwGroupID);
				DWORD dwLocalPlayer = RoleMgr::Inst()->GetLocalPlayerID();
				if ( dwGroupMaster == dwLocalPlayer )
				{
					m_LBGroupNameList->SetTextColor(0xFFFF0000);
				} 
				else
				{
					m_LBGroupNameList->SetTextColor(0xFFFFFFFF);
				}
				m_LBGroupNameList->SetText( curRow, 0, szGroupName );
			}
			curRow++;
		}
	}
	else
	{
		m_BtnDelChatGroup->SetEnable(FALSE);
		m_BtnQuitChatGroup->SetEnable(FALSE);
		m_BtnSendMessage->SetEnable(FALSE);
	}

	tagPoint ptSizeOldListBox = m_LBGroupNameList->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_LBGroupNameList->GetRowHeight() * curRow;
	m_LBGroupNameList->SetResizable( TRUE );
	m_LBGroupNameList->Resize( ptSizeListBox );
	m_LBGroupNameList->SetResizable( FALSE );
	m_LBGroupNameList->SetRefresh(TRUE);

}

VOID  MultChatFrame::RefreshMemberList()
{
	DWORD curRow = 0;
	m_LBMemberNameList->Clear();
	std::vector<DWORD> MemberIDList = MultChatMgr::Inst()->GetMemberIDList();
	if (!MemberIDList.empty())
	{
		std::vector<DWORD>::const_iterator cIter = MemberIDList.begin();
		for ( ; cIter != MemberIDList.end(); cIter++ )
		{
			LPCTSTR szMemberName = MultChatMgr::Inst()->MemberID2Name(*cIter);
			if ( szMemberName == NULL )
			{
				continue;
			}
			if ( _tcslen(szMemberName) > 0 )
			{
				tstringstream tss;
				if ( (*cIter)== MultChatMgr::Inst()->GetCurPickGroupMasterID() )
				{
					m_LBMemberNameList->SetTextColor(0xFFFF0000);
					tss<<g_StrTable[_T("Chat_Group_Master")]<<szMemberName;
				} 
				else
				{
					m_LBMemberNameList->SetTextColor(0xFFFFFFFF);
					tss<<szMemberName;
				}
				m_LBMemberNameList->SetText( curRow, 0, tss.str().c_str() );
			}
			curRow++;
		}
	}
	
	tagPoint ptSizeOldListBox = m_LBMemberNameList->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_LBMemberNameList->GetRowHeight() * curRow;
	m_LBMemberNameList->SetResizable( TRUE );
	m_LBMemberNameList->Resize( ptSizeListBox );
	m_LBMemberNameList->SetResizable( FALSE );
	if ( Click_Btn_Kick == m_eClickBtnType )
	{
		m_LBMemberNameList->SetCurSelectedRow(0);
		m_eClickBtnType = Click_Btn_NULL;
	}
	else if ( Click_Btn_AddMember == m_eClickBtnType )
	{
		m_LBMemberNameList->SetCurSelectedRow(curRow);
		m_eClickBtnType = Click_Btn_NULL;
	}
	m_LBMemberNameList->SetRefresh(TRUE);

	//若选中成员退出则需要隐藏该listbox
	DWORD dwCurPickMemberID = MultChatMgr::Inst()->GetCurPickMemberID(); 
	if ( dwCurPickMemberID == GT_INVALID )
	{
		if (P_VALID(m_pWndMenberMenu))
		{
			m_pWndMenberMenu->SetInvisible(TRUE);
		}
	}
}

VOID  MultChatFrame::RefreshChatBoard()
{
	m_REBChatBlank->ClsText();
	LPCTSTR szText = MultChatMgr::Inst()->GetCurGroupChatContent();
	m_REBChatBlank->AddText( szText , FALSE, GT_INVALID, GT_INVALID, TRUE, FALSE);   
}

VOID    MultChatFrame::RefreshAddMenberButton()
{
	DWORD dwGroupMasterID = MultChatMgr::Inst()->GetCurPickGroupMasterID() ;
	DWORD dwLocalPlayerID = RoleMgr::Inst()->GetLocalPlayerID();
	std::vector<DWORD> GroupIDList = MultChatMgr::Inst()->GetGroupIDList();
	if ( dwGroupMasterID== dwLocalPlayerID && !GroupIDList.empty())
	{
		m_BtnAddMember->SetEnable(TRUE);
	}
	else
	{
		m_BtnAddMember->SetEnable(FALSE);
	}
	m_BtnAddMember->SetRefresh(TRUE);
}

//删除richEditBox为了进行排版在文字（strText）里自动加上的回车键
VOID MultChatFrame::DeleteRETURNS(tstring& strText)
{
	tstring::iterator new_end;
	new_end = remove(strText.begin(), strText.end(), (TCHAR)0x0d);
	strText.erase(new_end, strText.end());
	new_end = remove(strText.begin(), strText.end(), (TCHAR)0x0a);
	strText.erase(new_end, strText.end());
}