#include "StdAfx.h"
#include "MultChatMgr.h"
#include "MultChatFrame.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "SocialData.h"
#include "SocialMgr.h"
#include "PlayerNameTab.h"
#include "CombatSysUtil.h"

MultChatMgr g_multChatMgr;
MultChatMgr* MultChatMgr::Inst()
{
	return &g_multChatMgr;
}

static INT  nDefaultGroupNameNum = 1;
MultChatMgr::MultChatMgr(void)
:m_trunk(this)
,m_dwCurChatGroupID(GT_INVALID)
,m_dwCurChatMemberID(GT_INVALID)
,m_bIsNeedRefresh(TRUE)
,m_iPlayerCreateGroupNum(0)
{
}

MultChatMgr::~MultChatMgr(void)
{
}

void MultChatMgr::Init()
{

	m_pNetCmdMgr = TObjRef<NetCmdMgr>();
	m_pSession = TObjRef<NetSession>();
	m_pFrameMgr = TObjRef<GameFrameMgr>();
	m_pUtil	= TObjRef<Util>();
	//m_mapGroupChatData的处理
	//--注册游戏事件
	m_pFrameMgr->RegisterEventHandle(_T("Open_MultChatFrame"),	  (FRAMEEVENTPROC)m_trunk.sfp1(&MultChatMgr::OnOpen_MultChatFrame));
	m_pFrameMgr->RegisterEventHandle(_T("Close_MultChatFrame"),	  (FRAMEEVENTPROC)m_trunk.sfp1(&MultChatMgr::OnClose_MultChatFrame));
	m_pFrameMgr->RegisterEventHandle(_T("Invite_To_Join"),(FRAMEEVENTPROC)m_trunk.sfp1(&MultChatMgr::AddNewMember));

	m_pNetCmdMgr->Register("NS_CreateGroupChat", (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecCreateNewGroupMsg)), _T("NS_CreateGroupChat"));
	m_pNetCmdMgr->Register("NS_DeleteGroupChat", (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecDeleteGurGroupMsg)), _T("NS_DeleteGroupChat"));
	m_pNetCmdMgr->Register("NS_LoginOutGroupChat", (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecQuitCurGroupMsg)), _T("NS_LoginOutGroupChat"));
	m_pNetCmdMgr->Register("NS_InviteRoleJoinToGroupChat", (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecInviteMemberMsg)), _T("NS_InviteRoleJoinToGroupChat"));
	m_pNetCmdMgr->Register("NS_AddRoleToGroupChat", (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecNewMemberJoinMsg)), _T("NS_AddRoleToGroupChat"));
	m_pNetCmdMgr->Register("NS_RoleChat", (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecNewMessage)), _T("NS_RoleChat"));
	m_pNetCmdMgr->Register("NS_RoleJoinToGroupChat", (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecAddNewMemberMsg)), _T("NS_RoleJoinToGroupChat"));
	m_pNetCmdMgr->Register("NS_InviteRoleToGroupChat", (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecInviteRoleToGroupChat)), _T("NS_InviteRoleToGroupChat"));
	m_pNetCmdMgr->Register("NS_KickRoleFromGroupChat", (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecKickMemberMsg)), _T("NS_KickRoleFromGroupChat"));

}

void MultChatMgr::Destroy()
{

	//--注销游戏事件
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_MultChatFrame"),	  (FRAMEEVENTPROC)m_trunk.sfp1(&MultChatMgr::OnOpen_MultChatFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("Close_MultChatFrame"),	  (FRAMEEVENTPROC)m_trunk.sfp1(&MultChatMgr::OnClose_MultChatFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("Invite_To_Join"),(FRAMEEVENTPROC)m_trunk.sfp1(&MultChatMgr::AddNewMember));
	
	m_pNetCmdMgr->UnRegister("NS_CreateGroupChat",   (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecCreateNewGroupMsg)));
	m_pNetCmdMgr->UnRegister("NS_DeleteGroupChat",   (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecDeleteGurGroupMsg)));
	m_pNetCmdMgr->UnRegister("NS_LoginOutGroupChat",   (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecQuitCurGroupMsg)));
	m_pNetCmdMgr->UnRegister("NS_InviteRoleJoinToGroupChat",   (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecInviteMemberMsg)));
	m_pNetCmdMgr->UnRegister("NS_AddRoleToGroupChat",   (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecNewMemberJoinMsg)));
	m_pNetCmdMgr->UnRegister("NS_RoleChat",   (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecNewMessage)));
	m_pNetCmdMgr->UnRegister("NS_RoleJoinToGroupChat",   (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecAddNewMemberMsg)));
	m_pNetCmdMgr->UnRegister("NS_InviteRoleToGroupChat",   (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecInviteRoleToGroupChat)));
	m_pNetCmdMgr->UnRegister("NS_KickRoleFromGroupChat",   (NETMSGPROC)(m_trunk.sfp2(&MultChatMgr::NetRecKickMemberMsg)));

	KillObjs();

	std::map<DWORD, tagLocalChatGroup*>::iterator it = m_mapGroupChatData.begin();
	while( it != m_mapGroupChatData.end() )
	{
		std::list<tagGroupChatMember*>::iterator iter = it->second->GroupChatMember.begin();
		while ( iter != it->second->GroupChatMember.end() )
		{
			SAFE_DELETE(*iter);
			iter++;
		}
		it->second->GroupChatMember.clear();
		SAFE_DELETE(it->second);
		it = m_mapGroupChatData.erase(it);
	}
	m_mapGroupChatData.clear();

	m_vecGroupIndex.clear();
	m_vecMemberIndex.clear();
	m_vecFriendsIDList.clear();
	m_vecFriendsArray.clear();
	m_vecGroupClint.clear();
	m_dwCurChatGroupID = GT_INVALID;
	m_dwCurChatMemberID = GT_INVALID;
	m_bIsNeedRefresh = TRUE;
	m_iPlayerCreateGroupNum = 0;
}

BOOL   MultChatMgr::RefreshGroupAndMemberList()
{
	
	m_vecGroupIndex.clear();
	std::map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.begin();
	for ( ; iter != m_mapGroupChatData.end(); iter++ )
	{
		m_vecGroupIndex.push_back(iter->first);
		if ( m_dwCurChatGroupID != GT_INVALID )
		{
			if ( iter->first == m_dwCurChatGroupID )
			{
				m_vecMemberIndex.clear();
				std::list<tagGroupChatMember*>::iterator iterList = iter->second->GroupChatMember.begin();
				while ( iterList != iter->second->GroupChatMember.end() )
				{
					m_vecMemberIndex.push_back((*iterList)->dwRoleId);
					iterList++;
				}
			}
		}
	}
	return TRUE;
}

VOID	MultChatMgr::SetCurPickGroupID(INT index)
{
	if (!m_vecGroupIndex.empty())
	{
		m_dwCurChatGroupID = m_vecGroupIndex[index];
	}
}

DWORD	MultChatMgr::GetCurPickGroupID() const
{
	if (!m_vecGroupIndex.empty())
	{
		 return  m_dwCurChatGroupID;   
	}
	else
	{
		return GT_INVALID;
	}
}

DWORD	MultChatMgr::GetCurPickGroupMasterID() const
{
	std::map<DWORD,tagLocalChatGroup*> mapGroupChatData = MultChatMgr::Inst()->GetGroupChatData();
	std::map<DWORD,tagLocalChatGroup*>::iterator iter = mapGroupChatData.begin();
	if (mapGroupChatData.empty())
	{
		return GT_INVALID;
	}
	iter = mapGroupChatData.find( m_dwCurChatGroupID );
	if ( iter != mapGroupChatData.end() )
	{
		DWORD dwMasterID = iter->second->dwGroupChatMasterId;
	    return dwMasterID;
	}
	return 0;
}

VOID	MultChatMgr::SetCurPickMemberID(INT index)
{
	if (!m_vecMemberIndex.empty())
	{
		m_dwCurChatMemberID = m_vecMemberIndex[index];
	}
}

DWORD	MultChatMgr::GetCurPickMemberID() const
{
	if (!m_vecMemberIndex.empty())
	{
		return  m_dwCurChatMemberID;
	}
	else
	{
		return GT_INVALID;
	}
}
INT		MultChatMgr::GetMemberNum( DWORD id) const
{
	std::map<DWORD,tagLocalChatGroup*>::const_iterator iterGroup = m_mapGroupChatData.find(id);
	if ( iterGroup == m_mapGroupChatData.end() )
	{
		return GROUP_MAX_MEMBER_NUM;
	}
	return iterGroup->second->dwMemberNum;
}
//--------------------------向服务器发送消息的按键响应函数--------------------------
//创建新的群聊组
VOID   MultChatMgr::CreateNewGroup(const tstring& tstr)
{
	tstring strTemp = tstr;
	if(strTemp.empty())
	{
		tstringstream ssTempName;
		ssTempName<<g_StrTable[_T("GroupChat_Default_Group_Name")]<<nDefaultGroupNameNum;
		strTemp = ssTempName.str();
		++nDefaultGroupNameNum;
	}
	if (m_iPlayerCreateGroupNum>=5)
	{
		if(P_VALID(GetObj("Chat_Group_Num_Too_Much")))
			KillObj("Chat_Group_Num_Too_Much");

		CreateObj("Chat_Group_Num_Too_Much", "MsgBoxEx");
		TObjRef<MsgBoxEx>("Chat_Group_Num_Too_Much")->Init( 
			_T(""), g_StrTable[_T("Chat_Group_Num_Too_Much")],       
			_T("Chat_Group_Num_Too_Much") );
		return;
	}
	tagNC_CreateGroupChat pMsg;
	_tcsncpy(pMsg.szGroupChatName, strTemp.c_str(), MAX_GROUPCHAT_LEN);  
	m_pSession->Send(&pMsg);
}
//删除一个聊天组
VOID   MultChatMgr::DeleteGurGroup()
{
	if(m_dwCurChatGroupID == GT_INVALID)
		return;
	tagNC_DeleteGroupChat msg ;
	msg.dwGroupChatId = m_dwCurChatGroupID;
	m_pSession->Send(&msg);
}
//退出聊天群
VOID   MultChatMgr::QuitCurGroup()
{
	if(m_dwCurChatGroupID == GT_INVALID)
		return;
	tagNC_LoginOutGroupChat msg;
	msg.dwGroupChatId = m_dwCurChatGroupID;
	m_pSession->Send(&msg);
}
//剔除某个玩家
VOID   MultChatMgr::KickMember(INT index)
{
	SetCurPickMemberID(index);
	if(m_dwCurChatGroupID == GT_INVALID || m_dwCurChatMemberID == GT_INVALID)
		return;
	if ( m_dwCurChatMemberID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		return;
	}
	DWORD  dwCurMasterID = GetCurPickGroupMasterID();
	DWORD  dwPlayerID    = RoleMgr::Inst()->GetLocalPlayerID();
	if ( dwPlayerID != dwCurMasterID )
	{
		if(P_VALID(GetObj("GRC_NotGroupChatMasterCanNotKick")))
			KillObj("GRC_NotGroupChatMasterCanNotKick");

		CreateObj("GRC_NotGroupChatMasterCanNotKick", "MsgBoxEx");
		TObjRef<MsgBoxEx>("GRC_NotGroupChatMasterCanNotKick")->Init( 
			_T(""),  g_StrTable[_T("GRC_NotGroupChatMasterCanNotKick")],
			_T("GRC_NotGroupChatMasterCanNotKick") );
		return;
	}
	tagNC_KickRoleFromGroupChat msg;
	msg.dwGroupChatId = m_dwCurChatGroupID;
	msg.dwRoleId     = m_dwCurChatMemberID;
	m_pSession->Send(&msg);
}

//邀请者发送邀请
VOID   MultChatMgr::InviteMember(const DWORD dwInviteRoleID)
{
	if(dwInviteRoleID == GT_INVALID)
		return;
	tagNC_InviteRoleToGroupChat msg;
	msg.dwDestRoleID  = dwInviteRoleID;
	msg.dwGroupChatId = m_dwCurChatGroupID;
	m_pSession->Send(&msg);
}
//被邀请者发送是否接受邀请
VOID   MultChatMgr::AddNewMember(tagMsgBoxEvent* pGameEvent)
{
	/*const TCHAR*  pMsgNameTstr = pGameEvent->pSender->GetName().c_str();
	const CHAR*	  pMsgNameStr  = m_pUtil->UnicodeToAnsi( pMsgNameTstr );*/
	MsgBoxFlag eResault = TObjRef<MsgBox>("GroupChat_Join_Event_Msgbox")->GetResult();
	BOOL bAccepted = FALSE;
	if ( eResault == 1 )
	{
		bAccepted = TRUE;
	} 
	DWORD dwSendInviteMsgGroupID = TObjRef<MsgBox>("GroupChat_Join_Event_Msgbox")->GetDwordUserData();
	DWORD dwSendInviteMsgGroupMasterID = TObjRef<MsgBox>("GroupChat_Join_Event_Msgbox")->GetDwordUserData2();

	//向服务器发消息
	tagNC_AddRoleToGroupChat msg; 
	msg.bIsAccepted = bAccepted;
	msg.dwGroupChatId = dwSendInviteMsgGroupID;
	msg.dwInviteRoleId = dwSendInviteMsgGroupMasterID;
	m_pSession->Send(&msg);
}
//发送消息
VOID   MultChatMgr::SendMessage(const tstring& tstr,INT len)
{
	tstring sendMsg = tstr; 
	tstring::iterator new_end;
	new_end = remove(sendMsg.begin(), sendMsg.end(), (TCHAR)0x0d);
	sendMsg.erase(new_end, sendMsg.end());
	new_end = remove(sendMsg.begin(), sendMsg.end(), (TCHAR)0x0a);
	sendMsg.erase(new_end, sendMsg.end());

	if( len > 0 && !sendMsg.empty())
	{
		if( m_dwCurChatGroupID != GT_INVALID )
		{
			TCHAR mess[1024] = {0};
			tagNC_RoleChat* pMsg = (tagNC_RoleChat*)mess;
			pMsg->dwID			= m_pUtil->Crc32("NC_RoleChat");
			pMsg->dwGroupChatID = m_dwCurChatGroupID;		
			pMsg->dwDestRoleID	= m_dwCurChatGroupID;						 
			pMsg->byChannel		= ESCC_Group;			 
			_tcsncpy(pMsg->szMsg, sendMsg.c_str(),MAX_CHAT_LEN);//说话内容
			pMsg->dwSize		= sizeof(tagNC_RoleChat) +(_tcslen(pMsg->szMsg))*sizeof(TCHAR);
			m_pSession->Send(pMsg);
		}
	}
}

//--------------------------接收服务器发送消息的响应函数---------------------------
//创建新的群聊组
DWORD MultChatMgr::NetRecCreateNewGroupMsg( tagNS_CreateGroupChat* pNetCmd, DWORD )
{
	DWORD dwError = pNetCmd->dwErrorCode;
	if ( DealWithErrorCode(dwError) == FALSE)
	{
		return 0;
	}
	
	tagLocalChatGroup  *LocalChatGroup = new tagLocalChatGroup;
	LocalChatGroup->dwGroupChatId = pNetCmd->dwGroupChatId;
	LocalChatGroup->dwGroupChatMasterId = RoleMgr::Inst()->GetLocalPlayerID();
	_tcsncpy(LocalChatGroup->szGroupChatName, pNetCmd->szGroupChatName, MAX_GROUPCHAT_LEN); 
	LocalChatGroup->dwMemberNum = 0;
	tagGroupChatMember *groupMember = new tagGroupChatMember;
	groupMember->dwRoleId = RoleMgr::Inst()->GetLocalPlayerID();
	_tcsncpy(groupMember->szRoleName, RoleMgr::Inst()->GetLocalPlayer()->GetRoleName().c_str(), X_SHORT_NAME); 
	LocalChatGroup->GroupChatMember.push_back(groupMember);
	_tcsncpy(LocalChatGroup->szChatContent,_T(""), MAX_GROUPCHAT_PAGE_SHOW); 

	m_mapGroupChatData[LocalChatGroup->dwGroupChatId] = LocalChatGroup;
	
	m_dwCurChatGroupID = pNetCmd->dwGroupChatId;
	m_dwCurChatMemberID = RoleMgr::Inst()->GetLocalPlayerID();
	m_bIsNeedRefresh = RefreshGroupAndMemberList();
	m_iPlayerCreateGroupNum++;
	
	return 1;
	
}
//删除一个聊天组
DWORD MultChatMgr::NetRecDeleteGurGroupMsg( tagNS_DeleteGroupChat* pNetCmd, DWORD )
{
	DWORD dwError = pNetCmd->dwError;
	if ( DealWithErrorCode(dwError) == FALSE)
	{
		return 0;
	}
	if (m_mapGroupChatData.empty())
	{
		return GT_INVALID;
	}
	DWORD dwId = pNetCmd->dwGroupChatId;
	std::map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.find( dwId );
	const TCHAR* ptempTchar = iter->second->szGroupChatName;
	DWORD dwMasterID = iter->second->dwGroupChatMasterId;
	TCHAR temp[128];
	wsprintf(temp, g_StrTable[_T("DeleteChatGroupBroadcast_String")],ptempTchar );
	std::list<tagGroupChatMember*>::iterator it = iter->second->GroupChatMember.begin();
	if ( iter != m_mapGroupChatData.end() )
	{
		while ( it != iter->second->GroupChatMember.end() )
		{

			tagGroupChatMember* pMemberIter=(*it);
			if( P_VALID(pMemberIter) )
			{
				SAFE_DELETE(pMemberIter);
				it = iter->second->GroupChatMember.erase(it);
				continue;
			}
			it++;
		}
		iter->second->GroupChatMember.clear();
		m_mapGroupChatData.erase( iter );
	}

	CombatSysUtil::Inst()->ShowScreenCenterMsgEx(temp);

	if (m_mapGroupChatData.empty())
	{
		m_dwCurChatGroupID = GT_INVALID;
	}
	else
	{
		std::map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.begin();
		m_dwCurChatGroupID = iter->first;
	}
	m_dwCurChatMemberID = GT_INVALID;
	m_bIsNeedRefresh = RefreshGroupAndMemberList();
	if ( RoleMgr::Inst()->GetLocalPlayerID() == dwMasterID )
	{
		m_iPlayerCreateGroupNum--;
	}
	return 1;
}
//退出聊天群
DWORD MultChatMgr::NetRecQuitCurGroupMsg( tagNS_LoginOutGroupChat* pNetCmd, DWORD )
{
	DWORD dwError = pNetCmd->dwError;
	if ( DealWithErrorCode(dwError) == FALSE)
	{
		return 0;
	}
	if (m_mapGroupChatData.empty())
	{
		return GT_INVALID;
	}
	DWORD dwQuitRoleID = pNetCmd->dwRoleId;
	DWORD dwGroupID = pNetCmd->dwGroupChatId;
	//DWORD dwGroupMasterID = pNetCmd->dwGroupChatMasterId;
	std::map<DWORD,tagLocalChatGroup*>::iterator iterGroup = m_mapGroupChatData.find( dwGroupID );
	if ( iterGroup == m_mapGroupChatData.end() )
	{
		return GT_INVALID;
	}
	//移交群主权力
	//iterGroup->second->dwGroupChatMasterId = dwGroupMasterID;
	
	//若为本地玩家退出则删掉群
	if ( dwQuitRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		if ( dwQuitRoleID == iterGroup->second->dwGroupChatMasterId && dwQuitRoleID != GT_INVALID )
		{
			//本地玩家退出自己创建的群，已创建数目自减
			m_iPlayerCreateGroupNum--;
		}

		std::list<tagGroupChatMember*>::iterator it = iterGroup->second->GroupChatMember.begin();
		while ( it != iterGroup->second->GroupChatMember.end() )
		{
			tagGroupChatMember* pMemberIter=(*it);
			if( P_VALID(pMemberIter) )
			{
				SAFE_DELETE(pMemberIter);
				it = iterGroup->second->GroupChatMember.erase(it);
				continue;
			}
			it++;
		}

		iterGroup->second->GroupChatMember.clear();
		m_mapGroupChatData.erase( iterGroup );

		if (m_mapGroupChatData.empty())
		{
			m_dwCurChatGroupID = GT_INVALID;
		}
		else
		{
			std::map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.begin();
			m_dwCurChatGroupID = iter->first;
		}
		m_dwCurChatMemberID = GT_INVALID;
		
	}
	//若为其他玩家退出则清除其名字
	else
	{
		(iterGroup->second->dwMemberNum)--;
		std::list<tagGroupChatMember*>& member = iterGroup->second->GroupChatMember;
		std::list<tagGroupChatMember*>::iterator  iterMember = member.begin();
		while( iterMember != member.end() )
		{
			if ( (*iterMember)->dwRoleId == dwQuitRoleID )
			{
				const TCHAR* ptempTchar = MemberID2Name( dwQuitRoleID );
				TCHAR temp[128];
				wsprintf(temp, g_StrTable[_T("GroupChat_Role_Quit")],ptempTchar );

				tstring tempcolor;
				tempcolor.append(_T("<color=(186,76,210)>"));
				tempcolor.append(temp);
				tempcolor.append(_T("<color=(255,255,255)>"));
				_stprintf( temp, _T("%s"),tempcolor.c_str() );

				INT iOldLength  = _tcslen( iterGroup->second->szChatContent );
				INT iNewLength  = _tcslen( temp );
				tstringstream  ssContent;
				ssContent<<iterGroup->second->szChatContent     //算iOldLength
					<<temp							   //算iNewLength
					<<_T("\n");                      //算+1

				TCHAR szContentNewBuff[MAX_GROUPCHAT_PAGE_SHOW*2]  = {0};
				_tcsncpy(szContentNewBuff,ssContent.str().c_str(), MAX_GROUPCHAT_PAGE_SHOW*2); 
				//超过最大长度，要删掉前面的内容腾出空间
				if ( iOldLength + iNewLength + 1 >= MAX_GROUPCHAT_PAGE_SHOW-1 )
				{
					//计算开始保留的字串位置
					INT iLenBegin = iOldLength + iNewLength + 1 - (MAX_GROUPCHAT_PAGE_SHOW-1);
					//找到保留串的第一个换行处
					while ( iLenBegin < MAX_GROUPCHAT_PAGE_SHOW-1 )
					{
						if ( szContentNewBuff[iLenBegin++] == '\n' )
							break;
					}
					//从第一个换行之后开始复制
					TCHAR*	pTcharBegin =  &szContentNewBuff[iLenBegin];
					_tcsncpy( iterGroup->second->szChatContent,pTcharBegin, MAX_GROUPCHAT_PAGE_SHOW-1 ); 
					iterGroup->second->szChatContent[MAX_GROUPCHAT_PAGE_SHOW-1] = '\0';
				} 
				else
				{
					_tcsncpy( iterGroup->second->szChatContent,szContentNewBuff, MAX_GROUPCHAT_PAGE_SHOW-1 ); 
					iterGroup->second->szChatContent[MAX_GROUPCHAT_PAGE_SHOW-1] = '\0';
				}

				SAFE_DELETE(*iterMember);
				member.erase(iterMember);
				break;
			}
			iterMember++;
		}
		
		if ( dwQuitRoleID == m_dwCurChatMemberID)
		{
			m_dwCurChatMemberID = GT_INVALID;
		}
	}
	m_bIsNeedRefresh = RefreshGroupAndMemberList();
	return 1;
}
//剔除某个玩家
DWORD MultChatMgr::NetRecKickMemberMsg( tagNS_KickRoleFromGroupChat* pNetCmd, DWORD )
{
	DWORD dwError = pNetCmd->dwError;
	if ( DealWithErrorCode(dwError) == FALSE)
	{
		return 0;
	}
	if (m_mapGroupChatData.empty())
	{
		return GT_INVALID;
	}
	DWORD dwKickRoleID = pNetCmd->dwRoleId;
	DWORD dwGroupID = pNetCmd->dwGroupChatId;
	std::map<DWORD,tagLocalChatGroup*>::iterator iterGroup = m_mapGroupChatData.find( dwGroupID );
	//是本地玩家被踢出，删掉群
	if ( dwKickRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		tstringstream tss;
		tss<<g_StrTable[_T("ChatGroup_KickFromGroup")]<<iterGroup->second->szGroupChatName;
		CombatSysUtil::Inst()->ShowScreenCenterMsg(tss.str().c_str());

		std::list<tagGroupChatMember*>::iterator it = iterGroup->second->GroupChatMember.begin();
		while ( it != iterGroup->second->GroupChatMember.end() )
		{

			tagGroupChatMember* pMemberIter=(*it);
			if( P_VALID(pMemberIter) )
			{
				SAFE_DELETE(pMemberIter);
				it = iterGroup->second->GroupChatMember.erase(it);
				continue;
			}
			it++;
		}
		iterGroup->second->GroupChatMember.clear();
		m_mapGroupChatData.erase( iterGroup );

		if (m_mapGroupChatData.empty())
		{
			m_dwCurChatGroupID = GT_INVALID;
		}
		else
		{
			std::map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.begin();
			m_dwCurChatGroupID = iter->first;
		}
		m_dwCurChatMemberID = GT_INVALID;
	} 
	//非本地玩家被踢出则清除其名字
	else
	{
		(iterGroup->second->dwMemberNum)--;
		std::list<tagGroupChatMember*>& member = iterGroup->second->GroupChatMember;
		std::list<tagGroupChatMember*>::iterator  iterMember = member.begin();
		while( iterMember != member.end() )
		{
			if ( (*iterMember)->dwRoleId == dwKickRoleID )
			{
				const TCHAR* ptempTchar = MemberID2Name( dwKickRoleID );
				TCHAR temp[128];
 				wsprintf(temp, g_StrTable[_T("GroupChat_Role_Quit")],ptempTchar );

				tstring tempcolor;
				tempcolor.append(_T("<color=(186,76,210)>"));
				tempcolor.append(temp);
				tempcolor.append(_T("<color=(255,255,255)>"));
				_stprintf( temp, _T("%s"),tempcolor.c_str() );
				 
				INT iOldLength  = _tcslen( iterGroup->second->szChatContent );
				INT iNewLength  = _tcslen( temp );
				tstringstream  ssContent;
				ssContent<<iterGroup->second->szChatContent     //算iOldLength
					<<temp							   //算iNewLength
					<<_T("\n");                      //算+1

				TCHAR szContentNewBuff[MAX_GROUPCHAT_PAGE_SHOW*2]  = {0};
				_tcsncpy(szContentNewBuff,ssContent.str().c_str(), MAX_GROUPCHAT_PAGE_SHOW*2); 
				//超过最大长度，要删掉前面的内容腾出空间
				if ( iOldLength + iNewLength + 1 >= MAX_GROUPCHAT_PAGE_SHOW-1 )
				{
					//计算开始保留的字串位置
					INT iLenBegin = iOldLength + iNewLength + 1 - (MAX_GROUPCHAT_PAGE_SHOW-1);
					//找到保留串的第一个换行处
					while ( iLenBegin < MAX_GROUPCHAT_PAGE_SHOW-1 )
					{
						if ( szContentNewBuff[iLenBegin++] == '\n' )
							break;
					}
					//从第一个换行之后开始复制
					TCHAR*	pTcharBegin =  &szContentNewBuff[iLenBegin];
					_tcsncpy( iterGroup->second->szChatContent,pTcharBegin, MAX_GROUPCHAT_PAGE_SHOW-1 ); 
					iterGroup->second->szChatContent[MAX_GROUPCHAT_PAGE_SHOW-1] = '\0';
				} 
				else
				{
					_tcsncpy( iterGroup->second->szChatContent,szContentNewBuff, MAX_GROUPCHAT_PAGE_SHOW-1 ); 
					iterGroup->second->szChatContent[MAX_GROUPCHAT_PAGE_SHOW-1] = '\0';
				}

				SAFE_DELETE(*iterMember);
				member.erase(iterMember);
				break;
			}
			iterMember++;
		}	 
		m_dwCurChatMemberID = GT_INVALID;
	}
	m_bIsNeedRefresh = RefreshGroupAndMemberList();
	return 1;
}
//被邀请者收到信息
DWORD MultChatMgr::NetRecInviteMemberMsg( tagNS_InviteRoleJoinToGroupChat* pNetCmd, DWORD )
{

	if(P_VALID(GetObj("GroupChat_Join_Event_Msgbox")))
		KillObj("GroupChat_Join_Event_Msgbox");
	CreateObj("GroupChat_Join_Event_Msgbox", "MsgBox"); //弹出确定按钮

	TCHAR temp[128];
	wsprintf(temp, g_StrTable[_T("Invite_To_Join")], pNetCmd->szGroupChatName );
	TObjRef<MsgBox>("GroupChat_Join_Event_Msgbox")->Init(_T(""), 
			         temp, _T("Invite_To_Join"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);
    TObjRef<MsgBox>("GroupChat_Join_Event_Msgbox")->SetDwordUserData(pNetCmd->dwGroupChatId);
	TObjRef<MsgBox>("GroupChat_Join_Event_Msgbox")->SetDwordUserData2(pNetCmd->dwInviteRoleId);
	TObjRef<MsgBox>("GroupChat_Join_Event_Msgbox")->SetStringUserData(_T("GroupChat_Join_Event_Msgbox"));
	m_bIsNeedRefresh = RefreshGroupAndMemberList();
	return 1;
}
//列表中加入新成员（被邀请者）
DWORD MultChatMgr::NetRecNewMemberJoinMsg( tagNS_AddRoleToGroupChat* pNetCmd, DWORD )
{
	DWORD dwError = pNetCmd->dwError;
	if ( DealWithErrorCode(dwError) == FALSE)
	{
		return 0;
	}

	std::map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.find( pNetCmd->dwGroupChatId );
	//说明本地原来就是群成员，删除原来该群的消息，再更新该群信息
	//这样做主要是为被邀请者
	if ( iter != m_mapGroupChatData.end() )
	{
		std::list<tagGroupChatMember*>::iterator it = iter->second->GroupChatMember.begin();
		while ( it != iter->second->GroupChatMember.end() )
		{
			SAFE_DELETE(*it);
			it++;
		}
		iter->second->GroupChatMember.clear();
		SAFE_DELETE( iter->second );
		m_mapGroupChatData.erase( iter );
	}

	tagLocalChatGroup  *LocalChatGroup = new tagLocalChatGroup;
	LocalChatGroup->dwGroupChatId = pNetCmd->dwGroupChatId;
	LocalChatGroup->dwGroupChatMasterId = pNetCmd->dwGroupChatMasterId;
	_tcsncpy(LocalChatGroup->szGroupChatName, pNetCmd->szGroupChatName, MAX_GROUPCHAT_LEN); 
	LocalChatGroup->dwMemberNum = pNetCmd->dwMemberNum;
	for (INT i=0; i< (INT)LocalChatGroup->dwMemberNum; i++)
	{
		tagGroupChatMember *groupMember = new tagGroupChatMember;
		groupMember->dwRoleId = pNetCmd->GroupChatMember[i].dwRoleId;
		tstringstream tss;
		tss<<pNetCmd->GroupChatMember[i].szRoleName;
		_tcsncpy(groupMember->szRoleName, tss.str().c_str(), X_SHORT_NAME); 
		LocalChatGroup->GroupChatMember.push_back(groupMember);
	}
	m_mapGroupChatData[LocalChatGroup->dwGroupChatId] = LocalChatGroup;
	if ( m_dwCurChatGroupID == GT_INVALID )
	{
		m_dwCurChatGroupID = pNetCmd->dwGroupChatId;
	}
	if ( m_dwCurChatMemberID == GT_INVALID )
	{
		m_dwCurChatMemberID = pNetCmd->dwGroupChatMasterId;
	}
	m_bIsNeedRefresh = RefreshGroupAndMemberList();
	return 1;
}

//群成员接收到加入新成员
DWORD MultChatMgr::NetRecAddNewMemberMsg( tagNS_RoleJoinToGroupChat* pNetCmd, DWORD )
{
	DWORD dwGroupID = pNetCmd->dwGroupChatId;
	DWORD dwJoinRoleID = pNetCmd->dwRoleID;
	const TCHAR* ptempTchar = pNetCmd->szName;
	if ( pNetCmd->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		return 0;
	}
	std::map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.find( dwGroupID );
	if ( iter != m_mapGroupChatData.end() )
	{
		tagGroupChatMember *groupMember = new tagGroupChatMember;
		groupMember->dwRoleId = pNetCmd->dwRoleID;
		_tcsncpy(groupMember->szRoleName, pNetCmd->szName, X_SHORT_NAME); 
		iter->second->GroupChatMember.push_back(groupMember);
		iter->second->dwMemberNum++;
		
		TCHAR temp[128];
		wsprintf(temp, g_StrTable[_T("GroupChat_Role_Join_In")],ptempTchar );
		
		tstring tempcolor;
		tempcolor.append(_T("<color=(186,76,210)>"));
		tempcolor.append(temp);
		tempcolor.append(_T("<color=(255,255,255)>"));
		_stprintf( temp, _T("%s"),tempcolor.c_str() );

		std::list<tagGroupChatMember*> tempGroupChatMember = iter->second->GroupChatMember;
		std::list<tagGroupChatMember*>::iterator iterMember = tempGroupChatMember.begin() ;
		for ( ; iterMember != tempGroupChatMember.end(); iterMember++)
		{
			if ( (*iterMember)->dwRoleId == dwJoinRoleID )
			{
				INT iOldLength  = _tcslen( iter->second->szChatContent );
				INT iNewLength  = _tcslen( temp );

				tstringstream  ssContent;
				ssContent<<iter->second->szChatContent     //算iOldLength
						 <<temp							   //算iNewLength
						 <<_T("\n");                      //算+1

				TCHAR szContentNewBuff[MAX_GROUPCHAT_PAGE_SHOW*2]  = {0};
				_tcsncpy(szContentNewBuff,ssContent.str().c_str(), MAX_GROUPCHAT_PAGE_SHOW*2); 
				//超过最大长度，要删掉前面的内容腾出空间
				if ( iOldLength + iNewLength + 1 >= MAX_GROUPCHAT_PAGE_SHOW-1 )
				{
					//计算开始保留的字串位置
					INT iLenBegin = iOldLength + iNewLength + 1 - (MAX_GROUPCHAT_PAGE_SHOW-1);
					//找到保留串的第一个换行处
					while ( iLenBegin < MAX_GROUPCHAT_PAGE_SHOW-1 )
					{
						if ( szContentNewBuff[iLenBegin++] == '\n' )
							break;
					}
					//从第一个换行之后开始复制
					TCHAR*	pTcharBegin =  &szContentNewBuff[iLenBegin];
					_tcsncpy( iter->second->szChatContent,pTcharBegin, MAX_GROUPCHAT_PAGE_SHOW-1 ); 
					iter->second->szChatContent[MAX_GROUPCHAT_PAGE_SHOW-1] = '\0';
				} 
				else
				{
					_tcsncpy( iter->second->szChatContent,szContentNewBuff, MAX_GROUPCHAT_PAGE_SHOW-1 ); 
					iter->second->szChatContent[MAX_GROUPCHAT_PAGE_SHOW-1] = '\0';
				}
			}

		}

		if ( m_dwCurChatGroupID == GT_INVALID )
		{
			m_dwCurChatGroupID = pNetCmd->dwGroupChatId;
		}
		if ( m_dwCurChatMemberID == GT_INVALID )
		{
			m_dwCurChatMemberID = pNetCmd->dwRoleID;
		}
		m_bIsNeedRefresh = RefreshGroupAndMemberList();
	}
	return 1;
}

//被邀请加入与否给邀请者的反馈
DWORD MultChatMgr::NetRecInviteRoleToGroupChat( tagNS_InviteRoleToGroupChat* pNetCmd, DWORD )
{
	DWORD dwError = pNetCmd->dwError;
	TCHAR szName[X_SHORT_NAME];
	_sntprintf(szName, X_SHORT_NAME, pNetCmd->szName );
	if ( DealWithErrorCode(dwError) == FALSE)
	{
		if ( GRC_RefuseJoinGroupChat == dwError)
		{
			tstring tsName = szName;
			tstringstream tss; 
			if ( !tsName.empty() )
				tss<<pNetCmd->szName;
			tss<<g_StrTable[_T("GRC_RefuseJoinGroupChat")];
			CombatSysUtil::Inst()->ShowScreenCenterMsg( tss.str().c_str() );
		}
		return 0;
	}

	return 1;
}

//接收服务器发送的聊天消息
DWORD MultChatMgr::NetRecNewMessage( tagNS_RoleChat* pNetCmd, DWORD dwValue)
{
	ESendChatChannel  ChatChannelflag = (ESendChatChannel)pNetCmd->byChannel;
	if ( ChatChannelflag != ESCC_Group )
	{
		return -1;
	}
	DWORD dwError = pNetCmd->dwErrorCode;
	if ( DealWithErrorCode(dwError) == FALSE)
	{
		return 0;
	}
	DWORD dwGroupID = pNetCmd->dwDestRoleID;
	DWORD dwChatMemberID = pNetCmd->dwSrcRoleID;
	//黑名单判断
	SocialMgr* pSocialMgr = (SocialMgr*)m_pFrameMgr->GetFrame(_T("SocialMgr"));
	if( P_VALID(pSocialMgr) )
	{
		SocialData* pData = pSocialMgr->GetData();
		if( P_VALID(pData) && pData->HasBlack( dwChatMemberID ) )
		{
			return 0;
		}

	}
	map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.find(dwGroupID);
	if ( iter == m_mapGroupChatData.end())
	{
		return -1;
	}

	std::list<tagGroupChatMember*> tempGroupChatMember = iter->second->GroupChatMember;
	std::list<tagGroupChatMember*>::iterator iterMember = tempGroupChatMember.begin() ;
	for ( ; iterMember != tempGroupChatMember.end(); iterMember++)
	{
		if ( (*iterMember)->dwRoleId == dwChatMemberID )
		{
			TCHAR* pMemberName = (*iterMember)->szRoleName;
			INT iOldLength  = _tcslen( iter->second->szChatContent );
			INT iNewLength  = _tcslen( pNetCmd->szMsg );
			INT iNameLength = _tcslen( pMemberName );

			tstringstream  ssContent;
			ssContent<<iter->second->szChatContent     //算iOldLength
				<<pMemberName                    //算iNameLength
				<<_T("：")							//算+1
				<<pNetCmd->szMsg                 //算iNewLength
				<<_T("\n");                      //算+1

			TCHAR szContentNewBuff[MAX_GROUPCHAT_PAGE_SHOW*2]  = {0};
			_tcsncpy(szContentNewBuff,ssContent.str().c_str(), MAX_GROUPCHAT_PAGE_SHOW*2); 

			//超过最大长度，要删掉前面的内容腾出空间
			if ( iOldLength + iNewLength + iNameLength + 2 >= MAX_GROUPCHAT_PAGE_SHOW-1 )
			{
				//计算开始保留的字串位置
				INT iLenBegin = iOldLength + iNewLength + iNameLength + 2 - (MAX_GROUPCHAT_PAGE_SHOW-1);
				//找到保留串的第一个换行处
				while ( iLenBegin < MAX_GROUPCHAT_PAGE_SHOW-1 )
				{
					if ( szContentNewBuff[iLenBegin++] == '\n' )
						break;
				}
				//从第一个换行之后开始复制
				TCHAR*	pTcharBegin =  &szContentNewBuff[iLenBegin];
				_tcsncpy( iter->second->szChatContent,pTcharBegin, MAX_GROUPCHAT_PAGE_SHOW-1 ); 
				iter->second->szChatContent[MAX_GROUPCHAT_PAGE_SHOW-1] = '\0';
			} 
			else
			{
				_tcsncpy( iter->second->szChatContent,szContentNewBuff, MAX_GROUPCHAT_PAGE_SHOW-1 ); 
				iter->second->szChatContent[MAX_GROUPCHAT_PAGE_SHOW-1] = '\0';
			}
		}
	
	}
	if ( dwGroupID != m_dwCurChatGroupID )
	{
		AddGlintGroup(dwGroupID);
	}
	m_bIsNeedRefresh = RefreshGroupAndMemberList();
	m_pFrameMgr->SendEvent(&tagGameEvent(_T("GroupChatButtonGlin"), NULL));

	return 1;
}

//---------------------------------------------------------------------------------
//错误码处理函数
BOOL  MultChatMgr::DealWithErrorCode(DWORD dwError)
{
	switch (dwError)
	{
	case GRC_AlreadyInGroupChat:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GRC_AlreadyInGroupChat")]);
		}
		break;
	case GRC_AddRoleToGroupChatError:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GRC_AddRoleToGroupChatError")]);
		}
		break;
	case GRC_LoginOutGroupChatError:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GRC_LoginOutGroupChatError")]);
		}
		break;
	case GRC_GroupChatNameInvalid:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GRC_GroupChatNameInvalid")]);
		}
		break;
	case GRC_RoleNotOnline:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GRC_RoleNotOnline")]);
			
		}
		break;
	case GRC_RefuseJoinGroupChat:
		{
			//do nothing and deal with this error later.
		}
		break;
	case GRC_GroupChatNotExist:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GRC_GroupChatNotExist")]);
			
		}
		break;
	case GRC_NotGroupChatMaster:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GRC_NotGroupChatMaster")]);
			
		}
		break;
	case GRC_AddRoleNotExist:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GRC_AddRoleNotExist")]);
			
		}
		break;
	case GRC_LoginOutGroupRoleNotExist:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GRC_LoginOutGroupRoleNotExist")]);
			
		}
		break;
	case GRC_GroupChatRoleOverMaxNum:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GRC_GroupChatRoleOverMaxNum")]);
			
		}
		break;
	default:
		return TRUE;
	}
	return FALSE;
}



std::vector<DWORD>& MultChatMgr::GetFriendsIDList()
{
	UpdateFriendsList();
	return m_vecFriendsIDList;
}
std::vector<LPCTSTR>& MultChatMgr::GetFriendsNameList()
{
	UpdateFriendsList();
   return m_vecFriendsArray;
}

VOID MultChatMgr::UpdateFriendsList()
{
	m_vecFriendsArray.clear();
	m_vecFriendsIDList.clear();
	SocialMgr* pSocialMgr = (SocialMgr*)m_pFrameMgr->GetFrame(_T("SocialMgr"));
	if( P_VALID(pSocialMgr) )
	{
		SocialData* pData = pSocialMgr->GetData();
		if( P_VALID(pData))
		{
			const SocialData::FriendsArray *friends;
			SocialData::EListType listType;
			for ( INT FriendType = 1; FriendType <= 4 ; FriendType++)
			{
				switch( FriendType )
				{
				case 1:
					listType = SocialData::ELT_Friend1;
					break;

				case 2:
					listType = SocialData::ELT_Friend2;
					break;

				case 3:
					listType = SocialData::ELT_Friend3;
					break;

				case 4:
					listType = SocialData::ELT_Friend4;
					break;

				default:
					break;
				}

				if( pData->GetFriendList( listType, &friends ) )
				{
					SocialData::FriendsArray::const_iterator iteri;
					for( iteri=friends->begin(); iteri!=friends->end(); ++iteri )
					{
						const tagFriendInfo& friendInfo = *iteri;

						if( !friendInfo.bOnline )//不显示离线好友或者黑名单
							continue;

						LPCTSTR szRoleName = PlayerNameTab::Inst()->FindNameByID( friendInfo.dwFriendID );
						m_vecFriendsIDList.push_back(friendInfo.dwFriendID);
						m_vecFriendsArray.push_back(szRoleName);
					}
				}
			}		
		}
	}
}

DWORD MultChatMgr::OnOpen_MultChatFrame(tagGameEvent* pEvent)
{
	if (!P_VALID(m_pFrameMgr->GetFrame(_T("MultChat"))))
	{
		m_pFrameMgr->CreateFrame(_T("World"), _T("MultChat"), _T("MultChatFrame"), 0);
		MultChatFrame* pMultChatFrame = (MultChatFrame*) m_pFrameMgr->GetFrame(_T("MultChat"));

	}
	else
	{
		MultChatFrame* pMultChatFrame = (MultChatFrame*) m_pFrameMgr->GetFrame(_T("MultChat"));
		pMultChatFrame->ShowWnd(TRUE);
	}
	
	return 0;
}

DWORD MultChatMgr::OnClose_MultChatFrame(tagGameEvent* pEvent)
{
	 
	if( P_VALID(m_pFrameMgr->GetFrame(_T("MultChat"))))
	{
		MultChatFrame* pMultChatFrame = (MultChatFrame*) m_pFrameMgr->GetFrame(_T("MultChat"));
		pMultChatFrame->ShowWnd(FALSE);
	}
	return 0;
}

VOID MultChatMgr::KillObjs()
{
	if(P_VALID(GetObj("GRC_NotGroupChatMasterCanNotKick")))
		KillObj("GRC_NotGroupChatMasterCanNotKick");//
	
	if(P_VALID(GetObj("GroupChat_Join_Event_Msgbox")))
		KillObj("GroupChat_Join_Event_Msgbox");//


	if(P_VALID(GetObj("Chat_Group_Num_Too_Much")))
		KillObj("Chat_Group_Num_Too_Much");//
}

//成员ID转名字
const TCHAR*  MultChatMgr::MemberID2Name(DWORD menberID)
{
	std::map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.begin();
	for ( ; iter != m_mapGroupChatData.end(); iter++ )
	{
		std::list<tagGroupChatMember*>::iterator iterList = iter->second->GroupChatMember.begin();
		while ( iterList != iter->second->GroupChatMember.end() )
		{
			if ( (*iterList)->dwRoleId == menberID )
			{
				return &((*iterList)->szRoleName[0]);
			}
			iterList++;
		}
	}
	return NULL;
}
//群ID转名字
const TCHAR*  MultChatMgr::GroupID2Name(DWORD groupID)
{
	std::map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.begin();
	for ( ; iter != m_mapGroupChatData.end(); iter++ )
	{
		 if ( groupID == iter->second->dwGroupChatId)
		 {
			 return &(iter->second->szGroupChatName[0]);
		 }
	}
	return NULL;
}

//返回当前群的聊天内容
const TCHAR*   MultChatMgr::GetCurGroupChatContent()
{
	std::map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.find(m_dwCurChatGroupID);
	
	if ( iter != m_mapGroupChatData.end() )
	{
		return &(iter->second->szChatContent[0]);
	}
	return NULL;
}

//获得群列表
std::vector<DWORD>&   MultChatMgr::GetGroupIDList()
{
	RefreshGroupAndMemberList();
	return 	m_vecGroupIndex;		 			
}

//获得成员列表
std::vector<DWORD>&   MultChatMgr::GetMemberIDList()
{
	RefreshGroupAndMemberList();
	return m_vecMemberIndex;		 
}

DWORD MultChatMgr::GetGrouptMasterID(DWORD dwGroupID)
{
	std::map<DWORD,tagLocalChatGroup*>::iterator iter = m_mapGroupChatData.find(dwGroupID);
	if ( iter != m_mapGroupChatData.end() )
	{
		DWORD dwMasterID = iter->second->dwGroupChatMasterId;
		return dwMasterID;
	}
	return 0;
}

//添加闪烁群
VOID  MultChatMgr::AddGlintGroup(DWORD id)
{
	std::vector<DWORD>::iterator iter = m_vecGroupClint.begin();
	while ( iter != m_vecGroupClint.end() )
	{
		if ( (*iter) == id )
		{
			return;
		}
		iter++;
	}
	m_vecGroupClint.push_back(id);
}

//减少闪烁群
VOID  MultChatMgr::DelGlintGroup(DWORD id)
{
	std::vector<DWORD>::iterator iter = m_vecGroupClint.begin();
	while ( iter != m_vecGroupClint.end() )
	{
		if ( (*iter) == id )
		{
			m_vecGroupClint.erase(iter);
			return;
		}
		iter++;
	}
}

//该ID的群是否需要闪烁
BOOL MultChatMgr::IfNeedGlint(DWORD id)
{
	if (m_vecGroupClint.empty())
	{
		return FALSE;
	}
	for ( int i = 0; i<(INT)m_vecGroupClint.size(); i++)
	{
		if ( id == m_vecGroupClint[i] )
		{
			return TRUE;
		}
	}
	return FALSE;
}