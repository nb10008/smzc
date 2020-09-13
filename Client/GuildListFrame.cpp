#include "StdAfx.h"
#include "GuildListFrame.h"
#include "../WorldDefine/msg_guild.h"
#include "CombatSysUtil.h"
#include "ChatEvent.h"
#include "ChatFrame.h"
#include "GuildMgr.h"

GuildListFrame::GuildListFrame(void) : m_Trunk(this),
m_pWndApply( NULL ),
m_pBtnPageBack( NULL ),
m_pBtnPageFront( NULL ),
m_pBtnSearch( NULL ),
m_pBtnPurpose( NULL ),
m_pBtnApply( NULL ),
m_pBtnCancel( NULL ),
m_pBtnClose( NULL ),
m_pStcPageNum( NULL ),
m_pLstGuilds( NULL ),
m_pWndSearch( NULL ),
m_pWndInput( NULL ),
m_pBtnSearchOk( NULL ),
m_pBtnSearchCancel( NULL ),
m_pBtnSearchClose( NULL ),
m_pPbtnLeader( NULL ),
m_pPbtnGuild( NULL ),
m_nMaxPage(0),
m_nCurPage(0),
m_dwLookingPurposeGuildID(GT_INVALID)
{
	m_strSearchName.assign( _T("") );
}


GuildListFrame::~GuildListFrame(void)
{
}


BOOL GuildListFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register("NS_GetAllGuildSmallInfo", (NETMSGPROC)m_Trunk.sfp2(&GuildListFrame::OnNS_GetAllGuildSmallInfo), _T("NS_GetAllGuildSmallInfo") );
	m_pCmdMgr->Register("NS_GetGuildPurpose", (NETMSGPROC)m_Trunk.sfp2(&GuildListFrame::OnNS_GetGuildPurpose), _T("NS_GetGuildPurpose") );
	m_pCmdMgr->Register("NS_WantToJoinGuild", (NETMSGPROC)m_Trunk.sfp2(&GuildListFrame::OnNS_WantToJoinGuild), _T("NS_WantToJoinGuild") );
	m_pCmdMgr->Register("NS_SearchGuild", (NETMSGPROC)m_Trunk.sfp2(&GuildListFrame::OnNS_SearchGuild), _T("NS_SearchGuild") );
	//m_pCmdMgr->Register("NS_ContactGuildLeader", (NETMSGPROC)m_Trunk.sfp2(&GuildListFrame::OnNS_ContactGuildLeader), _T("NS_ContactGuildLeader") );

	m_pMgr->RegisterEventHandle( _T("MsgBoxEvent_ApplyToJoinGuild"), (FRAMEEVENTPROC)m_Trunk.sfp1( &GuildListFrame::OnApplyToJoinGuild ) );
	
	//界面初始化完成先，显示出来，然后发送信息获取帮派信息
	//tagNC_GetAllGuildSmallInfo cmd;
	//m_pSession->Send( &cmd );

	return TRUE;
}


BOOL GuildListFrame::Destroy()
{
	GameFrame::Destroy();

	if( P_VALID( m_pWndSearch ) )
		m_pGUI->DestroyWnd( m_pWndSearch );

	if( P_VALID(m_pWndTenet) )
		m_pGUI->DestroyWnd( m_pWndTenet );

	if( P_VALID( m_pWndApply ) )
		m_pGUI->DestroyWnd( m_pWndApply );
	
	m_pWndApply = NULL;
	m_pWndSearch = NULL;

	m_pCmdMgr->UnRegister("NS_GetAllGuildSmallInfo", (NETMSGPROC)m_Trunk.sfp2(&GuildListFrame::OnNS_GetAllGuildSmallInfo) );
	m_pCmdMgr->UnRegister("NS_GetGuildPurpose", (NETMSGPROC)m_Trunk.sfp2(&GuildListFrame::OnNS_GetGuildPurpose) );
	m_pCmdMgr->UnRegister("NS_WantToJoinGuild", (NETMSGPROC)m_Trunk.sfp2(&GuildListFrame::OnNS_WantToJoinGuild) );
	m_pCmdMgr->UnRegister("NS_SearchGuild", (NETMSGPROC)m_Trunk.sfp2(&GuildListFrame::OnNS_SearchGuild) );
	
	//m_pCmdMgr->UnRegister("NS_ContactGuildLeader", (NETMSGPROC)m_Trunk.sfp2(&GuildListFrame::OnNS_ContactGuildLeader) );

	m_pMgr->UnRegisterEventHandler( _T("MsgBoxEvent_ApplyToJoinGuild"), (FRAMEEVENTPROC)m_Trunk.sfp1( &GuildListFrame::OnApplyToJoinGuild ) );
	return TRUE;
}

BOOL GuildListFrame::EscCancel()
{
	m_pMgr->AddToDestroyList( this );
	return TRUE;
}

DWORD GuildListFrame::OnApplyToJoinGuild( tagMsgBoxEvent* pEvent )
{
	if( pEvent->eResult == MBF_OK )
	{
		if (GuildMgr::Inst()->IsInSpecState(EGDSS_Warfare))
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GuildWarStateLimit_New")]);
			return 0;
		}
		tagNC_WantToJoinGuild cmd;
		cmd.dwGuildID = TObjRef<MsgBox>("ApplyToJoinGuild_MsgBox")->GetDwordUserData();

		if( !GT_VALID( cmd.dwGuildID ) )
			return 0;

		m_pSession->Send( &cmd );
	}

	return 0;
}


BOOL GuildListFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\guild_list.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWndApply = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	m_pGUI->RegisterEventHandler( m_pWndApply->GetFullName().c_str(), m_Trunk.sfp1(&GuildListFrame::EventHandler) );

	m_pBtnPageBack	 = (GUIButton*)m_pWndApply->GetChild( _T("group_pic\\listback\\backbutt") );
	m_pBtnPageFront	 = (GUIButton*)m_pWndApply->GetChild( _T("group_pic\\listback\\nextbutt") );
	m_pStcPageNum	 = (GUIStatic*)m_pWndApply->GetChild( _T("group_pic\\listback\\num") );
	m_pBtnSearch	 = (GUIButton*)m_pWndApply->GetChild( _T("group_pic\\listback\\searchbutt") );
	m_pBtnPurpose	 = (GUIButton*)m_pWndApply->GetChild( _T("group_pic\\listback\\aimbutt") );
	m_pBtnApply		 = (GUIButton*)m_pWndApply->GetChild( _T("group_pic\\listback\\applicationbutt") );
	m_pBtnClose		 = (GUIButton*)m_pWndApply->GetChild( _T("group_pic\\closebutt") );
	m_pBtnCancel	 = (GUIButton*)m_pWndApply->GetChild( _T("group_pic\\listback\\cancelbutt") );
	m_pLstGuilds	 = (GUIListBox*)m_pWndApply->GetChild( _T("group_pic\\listback\\listback0\\member_list") );
	
	TCHAR szNum[10] = {0};
	m_pStcPageNum->SetText( _itot(m_nCurPage+1,szNum,10) );
	m_pLstGuilds->SetColNum( 1, 73 );
	m_pLstGuilds->SetColNum( 4, 73 );
	m_pLstGuilds->SetColWidth( 0, 119 );
	m_pLstGuilds->SetColWidth( 1, 67 );
	m_pLstGuilds->SetColWidth( 2, 67 );
	m_pLstGuilds->SetColWidth( 3, 130 );
	m_pWndApply->SetInvisible( TRUE );

	//搜索界面
	strPath = vEngine::g_strLocalPath + _T("\\ui\\guild_search.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWndSearch = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	m_pGUI->RegisterEventHandler( m_pWndSearch->GetFullName().c_str(), m_Trunk.sfp1(&GuildListFrame::EventHandler) );

	m_pWndInput			= (GUIEditBox*)m_pWndSearch->GetChild( _T("searchgroup_pic\\putinpic\\putin") );
	m_pBtnSearchOk		= (GUIButton*)m_pWndSearch->GetChild( _T("searchgroup_pic\\surebutt") );
	m_pBtnSearchCancel	= (GUIButton*)m_pWndSearch->GetChild( _T("searchgroup_pic\\canclebutt") );
	m_pBtnSearchClose	= (GUIButton*)m_pWndSearch->GetChild( _T("searchgroup_pic\\closebutt") );
	m_pPbtnGuild		= (GUIPushButton*)m_pWndSearch->GetChild( _T("searchgroup_pic\\groupback\\optbutt") );
	m_pPbtnLeader		= (GUIPushButton*)m_pWndSearch->GetChild( _T("searchgroup_pic\\groupback\\optbutt1") );
	m_pWndSearch->SetInvisible( TRUE );

	//帮派宗旨界面
	strPath = vEngine::g_strLocalPath + _T("\\ui\\msgbox2.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWndTenet = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	m_pGUI->RegisterEventHandler( m_pWndTenet->GetFullName().c_str(), m_Trunk.sfp1(&GuildListFrame::EventHandler) );

	m_pBtnContactLeader	= (GUIButton*)m_pWndTenet->GetChild( _T("back\\ok") );
	m_pBtnTenetCancel	= (GUIButton*)m_pWndTenet->GetChild( _T("back\\cancel") );
	m_pStcTenet			= (GUIStaticEx*)m_pWndTenet->GetChild( _T("back\\textback\\text") );
	
	m_pBtnContactLeader->SetText( g_StrTable[_T("GuildApply_ContactGuildLeader")] );
	m_pBtnContactLeader->SetRefresh( TRUE );
	m_pWndTenet->SetInvisible( TRUE );


	return TRUE;
}

//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL GuildListFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			//关闭按钮
			if( pWnd == m_pBtnClose )
			{
				m_pMgr->AddToDestroyList( this );
			}
			//取消按钮
			else if( pWnd == m_pBtnCancel )
			{
				m_pMgr->AddToDestroyList( this );
			}
			//向第一页的方向翻页
			else if( pWnd == m_pBtnPageBack )
			{
				//不能再翻页了，已经是第一页了，第一页索引为0
				if( m_nCurPage <= 0 )
					break;

				m_nCurPage--;
				RefreshGuildsDisplay();
			}
			//向最后一页的方向翻页
			else if( pWnd == m_pBtnPageFront )
			{
				//不能再翻页了，已经是最后一页了
				if( m_nCurPage >= (GetPages()-1) )
					break;

				m_nCurPage++;
				RefreshGuildsDisplay();
			}
			//申请入帮
			else if( pWnd == m_pBtnApply )
			{
				OpenApplyMsgBox();
			}
			//打开帮派搜索界面
			else if( pWnd == m_pBtnSearch )
			{
				m_pWndInput->SetText( _T("") );
				m_pWndSearch->SetInvisible( FALSE );
				m_pWndSearch->FlipToTop();
			}
			//搜索界面的关闭按钮
			else if( pWnd == m_pBtnSearchClose )
			{
				m_pWndSearch->SetInvisible( TRUE );
			}
			//搜索界面的取消按钮
			else if( pWnd == m_pBtnSearchCancel )
			{
				m_pWndSearch->SetInvisible( TRUE );
			}
			//搜索界面的确定按钮
			else if( pWnd == m_pBtnSearchOk )
			{
				INT nType  = GT_INVALID;
				if( EGUIBS_PushDown == m_pPbtnGuild->GetState() )
					nType = 0;
				else if( EGUIBS_PushDown == m_pPbtnLeader->GetState() )
					nType = 1;

				if( GT_VALID(nType) )
				{
					int nIndex = SearchGuild( nType, m_pWndInput->GetText() );
					if( !GT_VALID(nIndex) )
					{
						tagNC_SearchGuild cmd;
						cmd.dwSearchMode = nType;
						INT len = _tcslen(m_pWndInput->GetText());
						INT size = sizeof(cmd.szName)/sizeof(cmd.szName[0]);
						len = min(len,size-1);
						_tcsncpy( cmd.szName, m_pWndInput->GetText(),len );
						cmd.szName[X_SHORT_NAME-1]=0;
						m_pSession->Send( &cmd );
						break;
					}

					//得到目标帮派的页
					int nPage = nIndex / GUILD_APPLY_MAX_ITEMS_PER_PAGE;
					m_nCurPage = nPage;
					RefreshGuildsDisplay();
					m_pLstGuilds->SetCurSelectedRow( nIndex - m_nCurPage*GUILD_APPLY_MAX_ITEMS_PER_PAGE );
					SelectListBoxItem( m_pLstGuilds->GetCurSelectedRow() );
					m_pWndSearch->SetInvisible( TRUE );
				}
			}
			//帮派宗旨
			else if( pWnd == m_pBtnPurpose )
			{
				OpenGuildPurpose();
			}
			//帮派宗旨界面的取消按钮
			else if( pWnd == m_pBtnTenetCancel )
			{
				m_pWndTenet->SetInvisible( TRUE );
			}
			//帮派宗旨界面的联系帮主按钮
			else if( pWnd == m_pBtnContactLeader )
			{
				ContactGuildLeader();
			}
		}
		break;

	case EGUIE_ItemClick:
		{
			if( pWnd == m_pLstGuilds )
			{
				SelectListBoxItem( m_pLstGuilds->GetCurSelectedRow() );
			}
		}
		break;

	default:
		break;
	}

	return TRUE;
}


DWORD GuildListFrame::OnNS_WantToJoinGuild( tagNS_WantToJoinGuild* pMsg, DWORD dwParam )
{
	TCHAR szContent[X_LONG_NAME] = {0};
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			_stprintf( szContent, g_StrTable[_T("GuildApply_NeedWaitAnswer")] );
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szContent );
		}
		break;
	case E_Guild_Join_AlreadyIn:
		{
			_stprintf( szContent, g_StrTable[_T("GuildApply_AlredyInThisGuild")] );
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szContent );
		}
		break;
	case E_GuildApply_MemberTooMuch:
		{
			_stprintf( szContent, g_StrTable[_T("GuildApply_GuildMemberLimit")] );
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szContent );
		}
		break;
	case E_GuildApply_YouHaveAlreadyApplied:
		{
			_stprintf( szContent, g_StrTable[_T("GuildApply_HaveApplied")] );
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szContent );
			
			_stprintf( szContent, g_StrTable[_T("GuildApply_HaveApplied4Chat")] );
			ChatFrame* pFrame = (ChatFrame*)m_pMgr->GetFrame( _T("Chat") );
			if( P_VALID(pFrame) )
				pFrame->PushInfo( szContent, ESCC_Current );
		}
		break;
	case E_GuildApply_ApplyTooMuch:
		{
			_stprintf( szContent, g_StrTable[_T("GuildApply_ApplierTooMuch")] );
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szContent );
		}
		break;
	case E_GuildApply_NotExist:
		{
			_stprintf( szContent, g_StrTable[_T("GuildApply_GuildNotExist")] );
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szContent );
		}
		break;
	case E_GuildApply_YouAlredyHaveGuild:
		{
			_stprintf( szContent, g_StrTable[_T("GuildApply_AlredyHaveGuild")] );
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szContent );
		}
		break;
	case E_Guild_Can_Not_Join_Or_Create:
		{
			if( P_VALID(GetObj("RoleInJustLeaveGuildState")) )
				KillObj("RoleInJustLeaveGuildState");
			CreateObj( "RoleInJustLeaveGuildState", "MsgBoxEx" );

			TObjRef<MsgBoxEx>("RoleInJustLeaveGuildState")->Init( _T(""), g_StrTable[_T("Guild_Can_Not_Join_Or_Create")],NULL,MBF_OK,FALSE  );
		}
		break;
	case E_Guild_Can_Not_Be_Invited:
		{
			_stprintf( szContent, g_StrTable[_T("Guild_Can_Not_Be_Invited")] );
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szContent );
		}
		break;
	default:
		{
			_stprintf( szContent, g_StrTable[_T("GuildApply_ApplyFailed")] );
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szContent );
		}
		break;
	}
	return 0;
}

DWORD GuildListFrame::OnNS_SearchGuild( tagNS_SearchGuild* pMsg, DWORD dwParam )
{
	if( pMsg->dwErrorCode == E_Success )
	{
		if( pMsg->nGuildNum == 0 )
			return 0;

		tagGuildSmallInfo info;
		INT nGuildCount = pMsg->nGuildNum;
		for( INT i = 0; i< nGuildCount; i++ )
		{
			memcpy( &info, &(pMsg->guilds[i]), sizeof(tagGuildSmallInfo) );
			AddGuildInfo( info );
		}

		int nIndex = m_lstGuildInfo.size()-1;
		int nPage = nIndex/GUILD_APPLY_MAX_ITEMS_PER_PAGE;
		m_nCurPage = nPage;
		RefreshGuildsDisplay();
		m_pLstGuilds->SetCurSelectedRow( nIndex - m_nCurPage*GUILD_APPLY_MAX_ITEMS_PER_PAGE );
		SelectListBoxItem( m_pLstGuilds->GetCurSelectedRow() );
		
		if( !m_pWndSearch->IsInvisible() )
		{
			m_pWndSearch->SetInvisible( TRUE );
		}

		return 0;
	}

	CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_NotFindTheGuild")] );

	return 0;
}


DWORD GuildListFrame::OnNS_ContactGuildLeader( tagNS_ContactGuildLeader* pMsg, DWORD dwParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			INT nCurGuildDataIndex = GetGuildDataIndex();
			if( !GT_VALID(nCurGuildDataIndex) )
				break;

			GuildListIterator iter = FindGuildInfo( nCurGuildDataIndex );
			if( iter == m_lstGuildInfo.end() || !GT_VALID(iter->dwGuildID) )
				break;

			tagPrivateChatEvent evt( _T("tagPrivateChatEvent"), this );
			evt.dwRoleID = pMsg->dwGuildLeaderID;
			evt.strRoleName = pMsg->szLeaderName;
			m_pMgr->SendEvent( &evt );

			m_pWndTenet->SetInvisible( TRUE );
		}
		break;
	case E_GuildApply_NotExist:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_NotExist")] );
			break;
		}
		break;
	case E_GuildApply_LackOfLeader:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_LackOfLeader")] );
			break;
		}
		break;
	case E_GuildApply_LeaderOffline:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_LeaderOffline")] );
			break;
		}
		break;

	default:
		break;

	}

	return 0;
}

DWORD GuildListFrame::OnNS_GetGuildPurpose( tagNS_GetGuildPurpose* pMsg, DWORD dwParam )
{
	switch( pMsg->dwErrorCode )
	{
	case E_Success:
		{
			TCHAR szPurpose[X_LONG_NAME] = {0};
			_stprintf( szPurpose, g_StrTable[_T("GuildApply_Purpose")], pMsg->szGuildName );
			tstring strContent = szPurpose;
			strContent += _T("\\n");
			strContent += pMsg->szContent;
			//-----------------------清一下格式------------------------
			tstring strTenet = strContent.c_str();
			tstring::iterator new_end;
			new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0d);
			strTenet.erase(new_end, strTenet.end());
			new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0a);
			strTenet.erase(new_end, strTenet.end());
			//---------------------------------------------------------
			m_pStcTenet->SetText( strTenet.c_str() );
			m_pStcTenet->SetRefresh( TRUE );
			m_pWndTenet->SetInvisible( FALSE );
		}
		break;
	default:
		{
			m_pWndTenet->SetInvisible( TRUE );
		}
		break;
	}

	return 0;
}


DWORD GuildListFrame::OnNS_GetAllGuildSmallInfo( tagNS_GetAllGuildSmallInfo* pMsg, DWORD dwParam )
{
	if( !P_VALID(pMsg) )
		return 0;

	//获取帮派失败
	if( pMsg->dwErrorCode != E_Success )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_FailedToGetGuildInfo")] );
		return 0;
	}

	if( pMsg->nGuildNum == 0 )
		return 0;

	m_lstGuildInfo.clear();
	UpdateGuildList( pMsg );
	RefreshGuildsDisplay();
	SelectListBoxItem( m_pLstGuilds->GetCurSelectedRow() );
	return 0;
}


void GuildListFrame::UpdateGuildList( tagNS_GetAllGuildSmallInfo* pMsg )
{
	tagGuildSmallInfo info;
	INT nGuildCount = pMsg->nGuildNum;

	for( INT i = 0; i< nGuildCount; i++ )
	{
// 		info.byLevel = pMsg->guilds[i].byLevel;
// 		info.dwGuildID = pMsg->guilds[i].dwGuildID;
// 		info.dwLeaderID = pMsg->guilds[i].dwLeaderID;
// 		info.nGuildMembersNum = pMsg->guilds[i].nGuildMembersNum;
// 		_tcscpy( info.szGuildName,pMsg->guilds[i].szGuildName );
// 		_tcscpy( info.szLeaderName, pMsg->guilds[i].szLeaderName );
		//info = pMsg->guilds[i];
		memcpy( &info, &(pMsg->guilds[i]), sizeof(tagGuildSmallInfo) );
		AddGuildInfo( info );
	}
}


void GuildListFrame::AddGuildInfo( tagGuildSmallInfo& info )
{
	std::list<tagGuildSmallInfo>::iterator iter;
	for( iter = m_lstGuildInfo.begin(); iter != m_lstGuildInfo.end(); ++iter )
	{
		if( iter->dwGuildID == info.dwGuildID )
		{
			iter->dwLeaderID = info.dwLeaderID;
			iter->byLevel = info.byLevel;
			iter->nGuildMembersNum = info.nGuildMembersNum;
			_tcscpy( iter->szGuildName,info.szGuildName );
			_tcscpy( iter->szLeaderName, info.szLeaderName );	

			return;
		}
	}

	m_lstGuildInfo.push_back( info );
}


void GuildListFrame::RefreshGuildsDisplay()
{  
	//再次校正页数
	if( m_nCurPage < 0 )
		m_nCurPage = 0;
	
	if( m_nCurPage >= GetPages() )
		m_nCurPage = GetPages() - 1;

	//得到要显示的listbox索引范围
	INT nBeginNum = m_nCurPage * GUILD_APPLY_MAX_ITEMS_PER_PAGE;			//要显示的记录的启示索引
	TCHAR szNum[X_SHORT_NAME] = {0};

	//定位到当前要显示的页的位置
	GuildListIterator iter = m_lstGuildInfo.begin();
	for( int nRowIndex = 0; iter != m_lstGuildInfo.end(); ++iter,++nRowIndex )
	{
		//从nBeginNum索引开始显示
		if( nRowIndex < nBeginNum )
			continue;
		else
			break;
	}

	ClearListBox();
	for( int nRowIndex = 0; iter != m_lstGuildInfo.end(); ++iter,++nRowIndex )
	{
		//每页显示这么多记录
		if( nRowIndex >= GUILD_APPLY_MAX_ITEMS_PER_PAGE )
			break;

		INT nColNum = 0;

		//设置帮派的名字
		m_pLstGuilds->SetText( nRowIndex, nColNum, iter->szGuildName );
		nColNum++;

		//设置帮派的等级
		m_pLstGuilds->SetText( nRowIndex, nColNum, _itot( iter->byLevel, szNum, 10 ) );
		nColNum++;
		
		//设置帮派人数
		m_pLstGuilds->SetText( nRowIndex, nColNum, _itot( iter->nGuildMembersNum, szNum, 10 ) );
		nColNum++;

		//设置帮主的名字
		m_pLstGuilds->SetText( nRowIndex, nColNum, iter->szLeaderName );
	}
	
	m_pStcPageNum->SetText( _itot( m_nCurPage+1, szNum, 10 ) );
	m_pStcPageNum->SetRefresh( TRUE );
	m_pLstGuilds->SetRefresh( TRUE );
}



int GuildListFrame::SearchGuild( INT nType, tstring strName )
{
	GuildListIterator iter = m_lstGuildInfo.begin();
	for( int nIndex = 0; iter != m_lstGuildInfo.end(); ++iter, ++nIndex )
	{
		//按照帮派的名字搜索
		if( nType == 0 )
		{
			if( _tcscmp( iter->szGuildName, strName.c_str() ) == 0 )
				return nIndex;
		}
		//按照帮主的名字搜索
		else if( nType == 1 )
		{
			if( _tcscmp( iter->szLeaderName, strName.c_str() ) == 0 )
				return nIndex;
		}
	}
	
	return GT_INVALID;
}

int GuildListFrame::GetGuildDataIndex()
{
	if( !IsCurPageGood() )
		return GT_INVALID;

	int nGuildIndex = m_nCurPage * GUILD_APPLY_MAX_ITEMS_PER_PAGE + m_nCurSelectItem;
	if( nGuildIndex >= 0 && nGuildIndex < (int)m_lstGuildInfo.size() )
	{
		return nGuildIndex;
	}

	return GT_INVALID;
}

void GuildListFrame::SelectListBoxItem( INT nSelectItem )
{
	if( nSelectItem == m_nCurSelectItem )
		return;

	if( nSelectItem < 0 || nSelectItem >= GUILD_APPLY_MAX_ITEMS_PER_PAGE )
	{
		m_pStcTenet->SetInvisible( TRUE );
		m_nCurSelectItem = GT_INVALID;
		return;
	}

	m_nCurSelectItem = nSelectItem;
}

void GuildListFrame::OpenApplyMsgBox()
{
	INT nCurGuildDataIndex = GetGuildDataIndex();
	if( !GT_VALID(nCurGuildDataIndex) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_NeedSelectOneGuild")] );
		return;
	}

	GuildListIterator iter = FindGuildInfo( nCurGuildDataIndex );
	if( iter == m_lstGuildInfo.end() )
		return;
	
	DWORD dwGuildID = iter->dwGuildID;
	if( !GT_VALID( dwGuildID) )
	{
		return;
	}

	tstring strGuildName = iter->szGuildName;

	if( P_VALID( GetObj( "ApplyToJoinGuild_MsgBox" ) ) )
		KillObj( "ApplyToJoinGuild_MsgBox" );

	CreateObj( "ApplyToJoinGuild_MsgBox", "MsgBox" );
	TObjRef<MsgBox>("ApplyToJoinGuild_MsgBox")->SetDwordUserData( dwGuildID );

	TCHAR szContent[X_LONG_NAME] = {0};
	_stprintf( szContent, g_StrTable[_T("GuildApply_ApplyMsgBoxContent")], strGuildName.c_str() );
	TObjRef<MsgBox>("ApplyToJoinGuild_MsgBox")->Init( g_StrTable[_T("GuildApply_ApplyMsgBoxCaption")],szContent,_T("MsgBoxEvent_ApplyToJoinGuild"),(MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE );

}

void GuildListFrame::OpenGuildPurpose()
{
	INT nCurGuildDataIndex = GetGuildDataIndex();
	if( !GT_VALID(nCurGuildDataIndex) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_NeedSelectOneGuild")] );
		return;
	}

	GuildListIterator iter = FindGuildInfo( nCurGuildDataIndex );
	if( iter == m_lstGuildInfo.end() || !GT_VALID(iter->dwGuildID) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_NeedSelectOneGuild")] );
		return;
	}

	m_pStcTenet->SetText( g_StrTable[_T("GuildApply_WaitServerPurpose")] );
	m_pWndTenet->SetInvisible( FALSE );
	m_pWndTenet->FlipToTop();

	m_dwLookingPurposeGuildID = iter->dwGuildID;

	tagNC_GetGuildPurpose cmd;
	cmd.dwGuildID = iter->dwGuildID;
	m_pSession->Send( &cmd );
}

void GuildListFrame::ContactGuildLeader()
{
	/*INT nCurGuildDataIndex = GetGuildDataIndex();
	if( !GT_VALID(nCurGuildDataIndex) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_NeedSelectOneGuild")] );
		return;
	}

	GuildListIterator iter = FindGuildInfo( nCurGuildDataIndex );
	if( iter == m_lstGuildInfo.end() || GT_VALID(iter->dwGuildID) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_NeedSelectOneGuild")] );
		return;
	}

	tagNC_ContactGuildLeader cmd;
	cmd.dwGuildID = iter->dwGuildID;
	m_pSession->Send( &cmd );*/
	
	if( !GT_VALID(m_dwLookingPurposeGuildID) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_NeedSelectOneGuild")] );
		return;
	}

	GuildListIterator iter = FindGuildInfoByID( m_dwLookingPurposeGuildID );
	if( iter == m_lstGuildInfo.end() || !GT_VALID(iter->dwGuildID) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_NeedSelectOneGuild")] );
		return;
	}

	tagPrivateChatEvent evt( _T("tagPrivateChatEvent"), this );
	evt.dwRoleID = iter->dwLeaderID;
	evt.strRoleName = iter->szLeaderName;
	m_pMgr->SendEvent( &evt );

	m_pWndTenet->SetInvisible( TRUE );
}

void GuildListFrame::ClearListBox()
{
	if( !P_VALID(m_pLstGuilds) )
		return;
	
	for( int nRow = 0; nRow< GUILD_APPLY_MAX_ITEMS_PER_PAGE; nRow++ )
	{
		for( int nCol = 0; nCol <=3; nCol++ )
		{
			m_pLstGuilds->SetText( nRow, nCol, _T("") );
		}
	}
}

GuildListFrame::GuildListIterator GuildListFrame::FindGuildInfoByID( DWORD dwGuildID )
{
	GuildListIterator iter = m_lstGuildInfo.begin();
	for( ; iter != m_lstGuildInfo.end(); ++iter )
	{
		if( iter->dwGuildID == dwGuildID )
			return iter;
	}

	return m_lstGuildInfo.end();
}

GuildListFrame::GuildListIterator GuildListFrame::FindGuildInfo( int nGuildDataIndex )
{
	if( nGuildDataIndex < 0 || nGuildDataIndex >= (int)m_lstGuildInfo.size() )
		return m_lstGuildInfo.end();

	GuildListIterator iter = m_lstGuildInfo.begin();
	for( int i=0; iter != m_lstGuildInfo.end(); ++iter, ++i )
	{
		if( i == nGuildDataIndex )
			return iter;
	}

	return m_lstGuildInfo.end();
}