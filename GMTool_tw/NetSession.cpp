#include "stdafx.h"
#include "wx/wx.h"
#include "log.h"
#include "NetSession.h"
#include "..\ServerDefine\msg_gm_tool_c.h"
#include "..\ServerDefine\msg_rt_c.h"
#include "stream_transport.h"
#include "NetSessionMgr.h"
#include "UserFrameMgr.h"
#include "DataMgr.h"

// 全局变量
bool g_bSingleSection = false;
bool g_bGMserverCallback = false;	// 加入这个全局变量的目的是为了保证账号登陆的时候，先处理来自gmserver的NRTS_Challenge消息，之后对界面进行初始化

extern tstring g_Tester;
extern CLog g_PMDLog;
extern CLog g_RNLog;
extern CLog g_CNLog;
extern CLog g_AFLog;
extern CLog g_MRLog;

NetSession::NetSession( void )
{
	m_pStream = NULL;
}

NetSession::~NetSession( void )
{
	Destroy();
}

bool NetSession::Init()
{
	if( !P_VALID(m_pStream) )
		m_pStream = new StreamTransport;

	if( P_VALID(m_pStream) )
	{
		if( m_pStream->Init(FALSE)!=TRUE )		
			IMSG(_T("Init NetSession Fail"));
	}

	//注册网络消息
	this->RegiserHandleMessage();

	return true;
}

void NetSession::Destroy()
{
	//取消注册网络消息
	if( P_VALID(m_pStream) )
	{
		//网络析构
		m_pStream->Destroy();
		delete m_pStream;
		m_pStream = NULL;
	}
	m_net_command_mgr.UnRegisterAll();
}

void NetSession::RegiserHandleMessage()
{
	m_net_command_mgr.Register("NGMTS_GMCreate",			&NetSession::OnNGMTS_GMCreate);
	m_net_command_mgr.Register("NGMTS_GMModify",			&NetSession::OnNGMTS_GMModify);
	m_net_command_mgr.Register("NGMTS_GMDelete",			&NetSession::OnNGMTS_GMDelete);
	m_net_command_mgr.Register("NRTS_Challenge",			&NetSession::OnNRTS_Challenge);
	m_net_command_mgr.Register("NRTS_PrivilegeInfo",		&NetSession::OnNRTS_PrivilegeInfo);
	m_net_command_mgr.Register("NRTS_AllServerStatInfo",	&NetSession::OnNRTS_AllServerStatInfo);
	m_net_command_mgr.Register("NRTS_AllServerDynaInfo",	&NetSession::OnNRTS_AllServerDynaInfo);
	m_net_command_mgr.Register("NGMTS_RoleResume",			&NetSession::OnNGMTS_RoleResume);
	m_net_command_mgr.Register("NGMTS_ProModify",			&NetSession::OnNGMTS_ProModify);
	m_net_command_mgr.Register("NGMTS_ItemCreate",			&NetSession::OnNGMTS_ItemCreate);	
	m_net_command_mgr.Register("NGMTS_ItemDelete",			&NetSession::OnNGMTS_ItemDelete);	
	m_net_command_mgr.Register("NGMTS_TimeLimitedMsg",		&NetSession::OnNGMTS_TimeLimitedMsg);
	m_net_command_mgr.Register("NGMTS_AccountForbid",		&NetSession::OnNGMTS_AccountForbid);
	m_net_command_mgr.Register("NGMTS_Blacklist",			&NetSession::OnNGMTS_IPBlacklist);
	m_net_command_mgr.Register("NGMTS_RoleSpeak",			&NetSession::OnNGMTS_RoleSpeak);
	m_net_command_mgr.Register("NGMTS_ItemResume",			&NetSession::OnNGMTS_ItemResume);
	m_net_command_mgr.Register("NGMTS_CancelTimeLimitedMsg",&NetSession::OnNGMTS_CancelTimeLimitedMsg);
	m_net_command_mgr.Register("NGMTS_CancelDoubleMsg",		&NetSession::OnNGMTS_CancelDouble);
	m_net_command_mgr.Register("NGMTS_CancelRightNotice",	&NetSession::OnNGMTS_CancelRightNotice);
	m_net_command_mgr.Register("NGMTS_KickLockedRole",		&NetSession::OnNGMTS_KickLockedAccount);
	m_net_command_mgr.Register("NGMTS_IssueTitle",			&NetSession::OnNGMTS_TitleIssue);
	m_net_command_mgr.Register("NGMTS_DelTitle",			&NetSession::OnNGMTS_TitleDel);
	m_net_command_mgr.Register("NGMTS_CancelAutoNotice",	&NetSession::OnNGMTS_CancelAutoNotice);
	m_net_command_mgr.Register("NGMTS_MoveRole",			&NetSession::OnNGMTS_MoveRole);
	m_net_command_mgr.Register("NGMTS_CancelAutoChatNotice",&NetSession::OnNGMTS_CancelAutoChatNotice);
	m_net_command_mgr.Register("NGMTS_ClearAttPoint",		&NetSession::OnNGMTS_ClearAttPoint);
	m_net_command_mgr.Register("NGMTS_LoadLog",				&NetSession::OnNGMTS_LoadLog);
}

void NetSession::RecvMessage()
{
	//收取消息
	while(true)
	{
		DWORD msg_total_size = 0;
		LPBYTE recv = m_pStream->Recv(msg_total_size);
		if (!P_VALID(recv))
		{
			break;
		}
		LPBYTE next_msg = recv;
		DWORD msg_size = 0;
		while (next_msg  < recv + msg_total_size )
		{
			tagNetCmd *command = (tagNetCmd*)next_msg;
			msg_size = command->dwSize;
			m_net_command_mgr.HandleCommand(command, msg_size, this);
			next_msg += msg_size;
		}
		m_pStream->FreeRecved(recv);
	}
}

BOOL NetSession::IsConnected()
{
	if( P_VALID(m_pStream) )
		return m_pStream->IsConnected();

	return FALSE;
}

BOOL NetSession::IsTryingToConnect()
{
	if( P_VALID(m_pStream) )
		return m_pStream->IsTryingToConnect();

	return FALSE;
}

VOID NetSession::TryToConnect( LPCSTR szIP, INT nPort/*=0*/ )
{	
	if( P_VALID(m_pStream) )
		m_pStream->TryToConnect( szIP, nPort );
}

VOID NetSession::StopTryingConnect()
{
	if( P_VALID(m_pStream) )
		m_pStream->StopTryingConnect();
}

void NetSession::SendMsg( LPVOID msg, DWORD size )
{
	if (msg == NULL )
	{
		IMSG(_T("msg pointer is null..."));
			return;
	}
	if (size < 0)
	{
		IMSG(_T("msg size is less than 0..."));
			return;
	}
	if (size >= 0xFFFFFFFF)
	{
		IMSG(_T("msg size is greater than max..."));
			return;
	}

	m_pStream->Send(msg, size);
}

DWORD NetSession::OnNRTS_AllServerStatInfo( tagNetCmd* pMsg )
{
	tagNRTS_AllServerStatInfo* msg = (tagNRTS_AllServerStatInfo*)pMsg;
	int section_num = msg->nSectionNum;
	LPBYTE tmp = msg->byInfo;
	if (section_num > 0)
	{
		for (int i = 0; i < section_num; i++)
		{
			tagSectionInfo section_info;

			TCHAR  buff[X_SHORT_NAME] = {0};
			CHAR    ip[X_SHORT_NAME] = {0};
			//大区名
			memcpy(buff, tmp, X_SHORT_NAME*sizeof(TCHAR) );
			section_info.section_name = buff;
			tmp +=X_SHORT_NAME*sizeof(TCHAR);
			section_info.id = *(DWORD*)tmp;
			tmp += sizeof(DWORD);
			//section_info.id = Crc32(buff);
			//tmp +=X_SHORT_NAME*sizeof(TCHAR);

			//LoginIP
			memcpy(ip, tmp, X_SHORT_NAME*sizeof(CHAR) );
			section_info.login_ip = ip;
			tmp +=X_SHORT_NAME*sizeof(CHAR);

			//world数量
			DWORD num = *(DWORD*)tmp;
			section_info.num = (INT)num;
			tmp +=sizeof(DWORD);
			for( int j=0; j<(int)num; j++ )
			{
				//游戏世界名称
				tagGameWorldInfo info;
				memcpy(buff, tmp, X_SHORT_NAME*sizeof(TCHAR) );
				info.world_name = buff;
				tmp +=X_SHORT_NAME*sizeof(TCHAR);
				info.id = *(DWORD*)tmp;
				tmp += sizeof(DWORD);
				//info.id = Crc32(buff);
				//tmp +=X_SHORT_NAME*sizeof(TCHAR);

				//WorldIP
				memcpy(ip, tmp, X_SHORT_NAME*sizeof(CHAR) );
				info.world_ip = ip;
				tmp +=X_SHORT_NAME*sizeof(CHAR);

				memcpy(ip, tmp, X_SHORT_NAME*sizeof(CHAR) );
				info.db_ip = ip;
				tmp +=X_SHORT_NAME*sizeof(CHAR);
				section_info.game_world_infos.insert(std::pair<DWORD, tagGameWorldInfo>(info.id, info));

				tagWorld* world = new tagWorld;
				world->section_name = section_info.section_name;
				world->section_id = section_info.id;
				world->world_name = info.world_name;
				world->world_id = info.id;
				world->ip = info.world_ip;

				sDataMgrPtr->AddWorldDataList(world);
			}
			sDataMgrPtr->AddSectionDataList(section_info);
		}
	}

	tagServerStatInfoEvent evt(_T("tagServerStatInfoEvent"));
	sUserFrameMgrPgr->SendEvent(&evt);

	return 0;
}

DWORD NetSession::OnNRTS_AllServerDynaInfo( tagNetCmd* pMsg )
{
	tagNRTS_AllServerDynaInfo *recv = (tagNRTS_AllServerDynaInfo *)pMsg;
	int section_num = recv->nSectionNum;
	LPBYTE tmp = recv->byInfo;
	if (section_num > 0)
	{
		for (int i = 0; i < section_num; i++)
		{
			tagSectionInfo section_info;
			//大区ID
			section_info.id = *(DWORD*)tmp;
			tmp += sizeof(DWORD);
			//Login状态
			tagServerInfo server_info = *(tagServerInfo*)tmp;
			section_info.login_status =  server_info.eStatus;
			tmp += sizeof(tagServerInfo);
			//world 数量
			DWORD world_num = *(DWORD*)tmp;
			tmp += sizeof(DWORD);
			for (int i = 0; i < (int)world_num;  i++)
			{
				//GameWorld 动态信息
				tagGameWorldInfo info;

				//world ID
				DWORD world_id = *(DWORD*)tmp;
				info.id = world_id;
				tmp += sizeof(DWORD);

				//world info
				tagServerInfo world_info = *(tagServerInfo*)tmp;
				info.world_status = world_info.eStatus;
				info.nPlayerOnline = world_info.nPlayerOnline;
				info.nMaxPlayerNum = world_info.nMaxPlayerNum;

				tmp += sizeof(tagServerInfo);

				//DB info
				tagServerInfo db_info = *(tagServerInfo*)tmp;
				info.db_status = db_info.eStatus;
				tmp += sizeof(tagServerInfo);

				section_info.game_world_infos.insert(std::pair<DWORD, tagGameWorldInfo>(info.id, info));
			}

			sDataMgrPtr->UpdateWorldDataList(&section_info);
		}
	}

	return 0;
}

//DWORD NetSession::OnNGMTS_Notice( tagNetCmd* pMsg )
//{
//	tagNGMTS_RightNotice* pRecv = (tagNGMTS_RightNotice*)pMsg;
//
//
//	tagNotice evt( _T("tagYxjPage") );
//	evt.dwErrorCode = pRecv->dwErrorCode;
//	evt.dwWorldID = pRecv->dwWorldID;
//	sUserFrameMgrPgr->SendEvent(&evt);
//	return 0;
//}

//DWORD NetSession::OnNGMTC_CastRun( tagNetCmd* pMsg )
//{
//	tagNGMTC_CastRun* pRecv = (tagNGMTC_CastRun*)pMsg;
//	 
//
//	tagUserEvent evt( _T("tagPmdPage") );
//	sUserFrameMgrPgr->SendEvent(&evt);
//	return 0;
//}

//DWORD NetSession::OnNGMTS_AutoNotice( tagNetCmd* pMsg )
//{
//	tagNGMTS_AutoNotice* pRecv = (tagNGMTS_AutoNotice*)pMsg;
//
//
//	tagAutoNotice evt( _T("tagZdxhPage") );
//	evt.dwErrorCode = pRecv->dwErrorCode;
//	evt.dwWorldID = pRecv->dwWorldID;	
//	sUserFrameMgrPgr->SendEvent(&evt);
//	return 0;
//}

DWORD NetSession::OnNRTS_Challenge( tagNetCmd* pMsg )
{
	tagNRTS_Challenge* pRecv = (tagNRTS_Challenge*)pMsg;

	g_bSingleSection = pRecv->bSingleSection;

	g_bGMserverCallback = true;

	while (g_bGMserverCallback)
	{
		Sleep(200);
	}

	tagLoginEvent evt( _T("tagLogin") );
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);

	sNetSessionMgrPtr->SetPrivilege(pRecv->ePrivilege);

	return 0;
}

DWORD NetSession::OnNRTS_PrivilegeInfo( tagNetCmd* pMsg )
{
	tagNRTS_PrivilegeInfo* pRecv = (tagNRTS_PrivilegeInfo*)pMsg;
	INT nNum = pRecv->nNum;

	for( INT i=0; i<nNum; i++ )
	{ 
		DWORD dwMsgCrc = pRecv->dwInfo[i*2];
		
		DWORD dwPri = pRecv->dwInfo[i*2 + 1];
		std::pair<DWORD, DWORD> pairPri = std::make_pair(dwMsgCrc, dwPri);
		sNetSessionMgrPtr->InsertPrivilegeMap(pairPri);
	}
	return 0;
}

DWORD NetSession::OnNGMTS_GMCreate( tagNetCmd* pMsg )
{
	tagNGMTS_GMCreate* pRecv = (tagNGMTS_GMCreate*)pMsg;

	tagGMCreate evt( _T("tagGMCreate") );
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);

	return 0;
}

DWORD NetSession::OnNGMTS_GMModify( tagNetCmd* pMsg )
{
	tagNGMTS_GMModify* pRecv = (tagNGMTS_GMModify*)pMsg;

	tagGMModify evt( _T("tagGMModify") );
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);

	return 0;
}

DWORD NetSession::OnNGMTS_GMDelete( tagNetCmd* pMsg )
{
	tagNGMTS_GMDelete* pRecv = (tagNGMTS_GMDelete*)pMsg;

	tagGMDelete evt( _T("tagGMDelete") );
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

DWORD NetSession::OnNGMTS_RoleResume( tagNetCmd* pMsg )
{
	tagNGMTS_RoleResume* pRecv = (tagNGMTS_RoleResume*)pMsg;

	tagRoleResume evt(_T("tagRoleResume"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

DWORD NetSession::OnNGMTS_ProModify( tagNetCmd* pMsg )
{
	tagNGMTS_ProModify* pRecv = (tagNGMTS_ProModify*)pMsg;

	tagRoleProModify evt(_T("tagRoleProModify"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

//DWORD NetSession::OnNGMTS_Double( tagNetCmd* pMsg )
//{
//	tagNGMTS_Double* pRecv = (tagNGMTS_Double*)pMsg;
//
//	tagDouble evt(_T("tagDoublePage"));
//	evt.dwErrorCode = pRecv->dwErrorCode;
//	evt.dwWorldID = pRecv->dwWorldID;
//	sUserFrameMgrPgr->SendEvent(&evt);
//
//	return 0;
//}

DWORD NetSession::OnNGMTS_ItemCreate( tagNetCmd* pMsg )
{
	tagNGMTS_ItemCreate* pRecv = (tagNGMTS_ItemCreate*)pMsg;

	tagItemCreate evt(_T("tagItemCreate"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);

	return 0;
}

DWORD NetSession::OnNGMTS_ItemDelete( tagNetCmd* pMsg )
{
	tagNGMTS_ItemDelete* pRecv = (tagNGMTS_ItemDelete*)pMsg;
	
	tagItemDelete evt(_T("tagItemDelete"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

DWORD NetSession::OnNGMTS_ItemResume( tagNetCmd* pMsg )
{
	tagNGMTS_ItemResume* pRecv = (tagNGMTS_ItemResume*)pMsg;

	tagItemDelete evt(_T("tagItemResume"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

DWORD NetSession::OnNGMTS_TitleIssue( tagNetCmd* pMsg )
{
	tagNGMTS_IssueTitle* pRecv = (tagNGMTS_IssueTitle*)pMsg;

	tagTitleIssue evt(_T("tagTitleIssue"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

DWORD NetSession::OnNGMTS_TitleDel( tagNetCmd* pMsg )
{
	tagNGMTS_DelTitle* pRecv = (tagNGMTS_DelTitle*)pMsg;

	tagTitleDel evt(_T("tagTitleDel"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

DWORD NetSession::OnNGMTS_TimeLimitedMsg( tagNetCmd* pMsg )
{
	tagNGMTS_TimeLimitedMsg* pRecv = (tagNGMTS_TimeLimitedMsg*)pMsg;
	tagTimeLimited evt(_T("tagTimeLimited"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	evt.dwType = pRecv->eType;
	evt.dwWorldID = pRecv->dwWorldID;
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

DWORD NetSession::OnNGMTS_AccountForbid(tagNetCmd* pMsg)
{
	tagNGMTS_AccountForbid* pRecv = (tagNGMTS_AccountForbid*) pMsg;

	//当使用导入文件的形式批量封停的时候，不是最后一个确认消息不处理
	//if(!(pRecv->bLast)) return;

	tagAccountSuspend evt(_T("tagAccountSuspend"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	evt.bForbid = (BOOL)pRecv->nAction;
	_sntprintf( evt.szAccount, X_SHORT_NAME, pRecv->szAccount );
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

DWORD NetSession::OnNGMTS_IPBlacklist(tagNetCmd* pMsg)
{
	tagNGMTS_Blacklist* pRecv = (tagNGMTS_Blacklist*) pMsg;

	tagIPBlacklist evt(_T("tagIPBlacklist"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

DWORD NetSession::OnNGMTS_RoleSpeak(tagNetCmd* pMsg)
{
	tagNGMTS_RoleSpeak* pRecv = (tagNGMTS_RoleSpeak*) pMsg;

	tagRoleSpeak evt(_T("tagRoleSpeak"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

DWORD NetSession::OnNGMTS_KickLockedAccount(tagNetCmd* pMsg)
{
	tagNGMTS_KickLockedRole* pRecv = (tagNGMTS_KickLockedRole*) pMsg;

	/*tagKickAccount evt(_T("tagKickAccount"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);*/

	TCHAR szBuff[X_LONG_STRING] = {0};
	switch(pRecv->dwErrorCode)
	{
	case E_GMTool_Success:
		wxMessageBox(g_StrTable[_T("SuspendOK")]);
		break;
	case E_GMTool_AccountNotExist:
		wxMessageBox(g_StrTable[_T("AccountNoExist")]);
		break;
	case E_GMTool_Err:
		wxMessageBox(g_StrTable[_T("ErrorDone")]);
		break;
	case E_GMTool_LoginDBLost:
		wxMessageBox(g_StrTable[_T("LoginDBLost")]);
		break;
	default:
		break;
	}
	return 0;
}

DWORD NetSession::OnNGMTS_CancelTimeLimitedMsg(tagNetCmd* pMsg)
{
	wxMessageBox(g_StrTable[_T("CancelDone")]);
	return 0;
}

DWORD NetSession::OnNGMTS_CancelDouble(tagNetCmd* pMsg)
{
	tagNGMTS_CancelDoubleMsg* pRecv = (tagNGMTS_CancelDoubleMsg*) pMsg;
	if (pRecv->dwErrorCode == E_GMTool_Success)
	{
		wxMessageBox(g_StrTable[_T("CancelDoubleDone")]);
		//g_MRLog.Note(_T("!!!!! Success !!!!!\n"));
	}
	else
	{
		wxMessageBox(g_StrTable[_T("ErrorDone")]);
		//g_MRLog.Note(_T("!!!!! Error !!!!!\n"));
	}
	return 0;
}

DWORD NetSession::OnNGMTS_CancelRightNotice(tagNetCmd* pMsg)
{
	tagNGMTS_CancelRightNotice* pRecv = (tagNGMTS_CancelRightNotice*) pMsg;
	if (pRecv->dwErrorCode == E_GMTool_Success)
	{
		wxMessageBox(g_StrTable[_T("CancelRightNoticeDone")]);
		//g_RNLog.Note(_T("!!!!! Success !!!!!\n"));
	}
	else
	{
		wxMessageBox(g_StrTable[_T("ErrorDone")]);
		//g_RNLog.Note(_T("!!!!! Error !!!!!\n"));
	}
	return 0;
}

DWORD NetSession::OnNGMTS_CancelAutoNotice(tagNetCmd* pMsg)
{
	tagNGMTS_CancelAutoNotice* pRecv = (tagNGMTS_CancelAutoNotice*) pMsg;
	if (pRecv->dwErrorCode == E_GMTool_Success)
	{
		wxMessageBox(g_StrTable[_T("CancelZdxhDone")]);
		//g_PMDLog.Note(_T("!!!!! Success !!!!!\n"));
	}
	else
	{
		wxMessageBox(g_StrTable[_T("ErrorDone")]);
		//g_PMDLog.Note(_T("!!!!! Error !!!!!\n"));
	}
	return 0;
}

DWORD NetSession::OnNGMTS_MoveRole(tagNetCmd* pMsg)
{
	tagNGMTS_MoveRole* pRecv = (tagNGMTS_MoveRole*) pMsg;

	tagMoveRole evt(_T("tagMoveRole"));
	evt.dwErrorCode = pRecv->dwErrorCode;
	sUserFrameMgrPgr->SendEvent(&evt);
	return 0;
}

DWORD NetSession::OnNGMTS_CancelAutoChatNotice(tagNetCmd* pMsg)
{
	tagNGMTS_CancelAutoChatNotice* pRecv = (tagNGMTS_CancelAutoChatNotice*) pMsg;
	if (pRecv->dwErrorCode == E_GMTool_Success)
	{
		wxMessageBox(g_StrTable[_T("CancelChatNoticeDone")]);
		//g_CNLog.Note(_T("!!!!! Success !!!!!\n"));
	}
	else
	{
		wxMessageBox(g_StrTable[_T("ErrorDone")]);
		//g_CNLog.Note(_T("!!!!! Error !!!!!\n"));
	}
	return 0;
}

DWORD NetSession::OnNGMTS_ClearAttPoint(tagNetCmd* pMsg)
{
	//MGET_MSG(pRecv, pMsg, NGMTS_ClearAttPoint);
	tagNGMTS_ClearAttPoint* pRecv = (tagNGMTS_ClearAttPoint*) pMsg;
	if (pRecv->dwErrorCode == E_GMTool_RoleNotExist)
	{
		wxMessageBox(g_StrTable[_T("RoleNoExist")]);
	}
	else if (pRecv->dwErrorCode == E_GMTool_RoleNotOnline)
	{
		wxMessageBox(g_StrTable[_T("RoleNotOnline")]);
	}
	else
	{
		wxMessageBox(g_StrTable[_T("SuspendOK")]);
	}
	return 0;
}

DWORD NetSession::OnNGMTS_LoadLog(tagNetCmd* pMsg)
{
	tagNGMTS_LoadLog* pRecv = (tagNGMTS_LoadLog*) pMsg;

	if (EAT_DoubleEnd == pRecv->eType || EAT_DoubleStart == pRecv->eType)
	{
		for (int i=0; i<pRecv->nNum; ++i)
		{
			TCHAR szNewItem[10*X_HUGE_STRING] = {0};
			_sntprintf(szNewItem, 10*X_HUGE_STRING, _T("\r %s | %s | %s | %s \n"), pRecv->item[i].szGMName, pRecv->item[i].szTime, pRecv->item[i].szIP, pRecv->item[i].szDetails);
			g_MRLog.Note(szNewItem);
		}
	}

	if (EAT_AutoNoticeStart == pRecv->eType || EAT_AutoNoticeEnd == pRecv->eType)
	{
		for (int i=0; i<pRecv->nNum; ++i)
		{
			TCHAR szNewItem[10*X_HUGE_STRING] = {0};
			_sntprintf(szNewItem, 10*X_HUGE_STRING, _T("\r %s | %s | %s | %s \n"), pRecv->item[i].szGMName, pRecv->item[i].szTime, pRecv->item[i].szIP, pRecv->item[i].szDetails);
			g_PMDLog.Note(szNewItem);
		}
	}

	if (EAT_NoticeStart == pRecv->eType || EAT_NoticeEnd == pRecv->eType)
	{
		for (int i=0; i<pRecv->nNum; ++i)
		{
			TCHAR szNewItem[10*X_HUGE_STRING] = {0};
			_sntprintf(szNewItem, 10*X_HUGE_STRING, _T("\r %s | %s | %s | %s \n"), pRecv->item[i].szGMName, pRecv->item[i].szTime, pRecv->item[i].szIP, pRecv->item[i].szDetails);
			g_RNLog.Note(szNewItem);
		}
	}

	if (EAT_AutoChatStart == pRecv->eType || EAT_AutoChatEnd == pRecv->eType)
	{
		for (int i=0; i<pRecv->nNum; ++i)
		{
			TCHAR szNewItem[10*X_HUGE_STRING] = {0};
			_sntprintf(szNewItem, 10*X_HUGE_STRING, _T("\r %s | %s | %s | %s \n"), pRecv->item[i].szGMName, pRecv->item[i].szTime, pRecv->item[i].szIP, pRecv->item[i].szDetails);
			g_CNLog.Note(szNewItem);
		}
	}

	if (EAT_AccountBanned == pRecv->eType || EAT_UndoAccountBanned == pRecv->eType)
	{
		for (int i=0; i<pRecv->nNum; ++i)
		{
			TCHAR szNewItem[10*X_HUGE_STRING] = {0};
			_sntprintf(szNewItem, 10*X_HUGE_STRING, _T("\r %s | %s | %s | %s \n"), pRecv->item[i].szGMName, pRecv->item[i].szTime, pRecv->item[i].szIP, pRecv->item[i].szDetails);
			g_AFLog.Note(szNewItem);
		}
	}

	return E_GMTool_Success;
}