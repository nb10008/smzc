//-----------------------------------------------------------------------------
//!\file NetSession.cpp
//!\author Lyp
//!
//!\date 2008-06-10
//! last 2008-06-12
//!
//!\brief net session
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "NetSession.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include<fstream>
#include "../WorldDefine/role_att.h"
#include "ClientMain.h"
//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
NetSession::~NetSession()
{
	m_pCmdMgr->UnRegister("NS_ServerInfo", (NETMSGPROC)m_Trunk.sfp2(&NetSession::NetRecvServerInfo) );
	m_pCmdMgr->UnRegister("NS_KoreaCSApprove", (NETMSGPROC)m_Trunk.sfp2(&NetSession::OnNS_KoreaCSApprove));
	m_pCmdMgr->Destroy();

	stringstream stream;
	stream << "XClient_" << this;

	KillObj(stream.str().c_str());
}


//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
VOID NetSession::Init()
{
	stringstream stream;
	stream << "XClient_" << this;
	m_pStream = CreateObj(stream.str().c_str(), "XClient");

	m_pStream->Init(FALSE);	// 关闭Nagle算法
	m_dwLastSendTime = timeGetTime();

	m_pCmdMgr->Init();
	m_pConsole->Register(_T("sendqueue"), m_Trunk.sfp1(&NetSession::SendQueue), _T("send queue list"), 1);
	m_pCmdMgr->Register("NS_ServerInfo", (NETMSGPROC)m_Trunk.sfp2(&NetSession::NetRecvServerInfo), _T("NS_ServerInfo") );
	m_pCmdMgr->Register("NS_KoreaCSApprove", (NETMSGPROC)m_Trunk.sfp2(&NetSession::OnNS_KoreaCSApprove), _T("NS_KoreaCSApprove"));

	m_dwLoginMode = 1;
	m_bEnableExpressPay = TRUE;
	_tcscpy(m_szExpressPayUrl, _T("http://open.kongzhong.com/hfapi/pay.jsp"));
	_tcscpy(m_szTodayFocusUrl, _T(""));
	m_bEnableCitySys = FALSE;

	m_bStartCheck = false;
}



//-----------------------------------------------------------------------------
// Connect
//-----------------------------------------------------------------------------
VOID NetSession::Connect(LPCTSTR szIP, INT nPort)
{
	m_strRemoteIP = szIP;
	m_pStream->TryToConnect(m_pUtil->UnicodeToUnicode8(szIP), nPort);
}


//-----------------------------------------------------------------------------
// Disconnect
//-----------------------------------------------------------------------------
VOID NetSession::Disconnect()
{
	m_pStream->Disconnect();
}


//-----------------------------------------------------------------------------
// IsConnect
//-----------------------------------------------------------------------------
BOOL NetSession::IsConnect()
{
	return m_pStream->IsConnected();
}



//-----------------------------------------------------------------------------
// Update
//-----------------------------------------------------------------------------
VOID NetSession::Update()
{
	static DWORD dwID_Skill = m_pUtil->Crc32("NC_Skill");	// 使用技能

	// 不停收网络消息
	for(;;)
	{
		DWORD dwRecvSize = 0;
		LPBYTE pRecv = m_pStream->Recv(dwRecvSize);
		if( !P_VALID(pRecv) )
			break;

		// 从大包中解出小包依次处理
		LPBYTE pRecvPointer = pRecv;
		DWORD dwMsgSize = 0;
		while( pRecvPointer < pRecv + dwRecvSize )
		{
			tagNetCmd* pRecvCmd = (tagNetCmd*)pRecvPointer;
			dwMsgSize = pRecvCmd->dwSize;
			m_pCmdMgr->HandleCmd(pRecvCmd, dwMsgSize, 0);
			pRecvPointer += dwMsgSize;
		}

		m_pStream->FreeRecved(pRecv);
	}

	
	if( m_SendQueueAction.GetMsgNum() > 0 )	
	{
		// 如果是使用技能消息，瞬间发
		m_SendQueueAction.ResetIterator();

		DWORD dwMsgSize = 0;
		LPBYTE pMsg = m_SendQueueAction.PeekMsg(dwMsgSize);
		if( ((tagNetCmd*)pMsg)->dwID == dwID_Skill )
		{
			m_pStream->Send(pMsg, dwMsgSize);
			m_SendQueueAction.RemoveMsg();
		}
	}


	// 每次心跳发送一条
	if( timeGetTime() - m_dwLastSendTime > TICK_TIME + 20 )
	{
		// 优先发送行为队列中的消息
		DWORD dwMsgSize = 0;
		LPBYTE pMsg = m_SendQueueAction.GetMsg(dwMsgSize);
		if( pMsg )
		{
			m_pStream->Send(pMsg, dwMsgSize);
			m_SendQueueAction.FreeMsg(pMsg);
		}
		else
		{
			pMsg = m_SendQueue.GetMsg(dwMsgSize);
			if( pMsg )
			{
				m_pStream->Send(pMsg, dwMsgSize);
				m_SendQueue.FreeMsg(pMsg);
			}
			else
			{
				pMsg = m_SendQueueIdle.GetMsg(dwMsgSize);
				if( pMsg )
				{
					m_pStream->Send(pMsg, dwMsgSize);
					m_SendQueueIdle.FreeMsg(pMsg);
				}
			}
		}

		m_dwLastSendTime = timeGetTime();
	}

	// 监视网络状态
	m_pConsole->Watch(_T("SendQueueAction"), m_SendQueueAction.GetMsgNum());
	m_pConsole->Watch(_T("SendQueue"), m_SendQueue.GetMsgNum());
	m_pConsole->Watch(_T("SendQueueIdle"), m_SendQueueIdle.GetMsgNum());
	m_pConsole->Watch(_T("NetSendSpeed"), m_pStream->GetBytesFlowOut());
	m_pConsole->Watch(_T("NetRecvSpeed"), m_pStream->GetBytesFlowIn());
	m_pConsole->Watch(_T("NetSendPack"), m_pStream->GetTotalSendPackNum());
	m_pConsole->Watch(_T("NetRecvPack"), m_pStream->GetTotalRecvPackNum());

}


//-----------------------------------------------------------------------------
// Send
//-----------------------------------------------------------------------------
VOID NetSession::Send(tagNetCmd* pNetCmd)
{
	static DWORD dwID_MouseMove	= m_pUtil->Crc32("NC_MouseWalk");		// 鼠标行走
	static DWORD dwID_KeyboardMove = m_pUtil->Crc32("NC_KeyboardWalk");	// 键盘行走
	static DWORD dwID_StopKeyboardWalk = m_pUtil->Crc32("NC_StopWalk");	// 停止键盘行走
	static DWORD dwID_Jump = m_pUtil->Crc32("NC_Jump");					// 跳跃
	static DWORD dwID_Drop = m_pUtil->Crc32("NC_Drop");					// 掉落
	static DWORD dwID_VDrop = m_pUtil->Crc32("NC_VDrop");				// 垂直掉落
	static DWORD dwID_Slide = m_pUtil->Crc32("NC_Slide");				// 滑落
	static DWORD dwID_Skill = m_pUtil->Crc32("NC_Skill");				// 使用技能
	static DWORD dwID_UseItem = m_pUtil->Crc32("NC_UseItem");			// 使用物品
	static DWORD dwID_SwapWeapon = m_pUtil->Crc32("NC_SwapWeapon");// 切换武器
	static DWORD dwID_PosCanGoTo = m_pUtil->Crc32("NC_CanGoToPos");// 输入坐标导航
	static DWORD dwID_PetSkillUse = m_pUtil->Crc32("NC_PetSkillUse");// 宠物使用技能
	

	static DWORD dwID_GetRemoteRoleState = m_pUtil->Crc32("NC_GetRemoteRoleState");	// 得到远程玩家信息
	static DWORD dwID_RoleGetSomeName = m_pUtil->Crc32("NC_RoleGetSomeName");	// 得到远程玩家名字

	static DWORD dwID_AutoHookOn = m_pUtil->Crc32("NC_AutoHookOn");	// 开启自动挂机
	static DWORD dwID_Stall = m_pUtil->Crc32("NC_StallGetTitle");	// 摆摊
	
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(plp) && plp->IsHookState())
	{
		pNetCmd->dwExID = static_cast<BYTE>(dwID_AutoHookOn);
	}
	// 检测外挂先放弃netsession检测环节
	/*
	if (m_bStartCheck)
	{
		if (pNetCmd->dwID == dwID_Skill)
			m_nSkillNumber++;
	}

	if (m_bStartCheck_Strategy2)
	{
		if (pNetCmd->dwID == dwID_Skill)
			m_nSkillNumber_Strategy2++;
	}
	*/
	// 将指令分类
	if( pNetCmd->dwID == dwID_MouseMove
		|| pNetCmd->dwID == dwID_KeyboardMove
		|| pNetCmd->dwID == dwID_StopKeyboardWalk
		|| pNetCmd->dwID == dwID_Jump
		|| pNetCmd->dwID == dwID_Drop
		|| pNetCmd->dwID == dwID_VDrop
		|| pNetCmd->dwID == dwID_Slide
		|| pNetCmd->dwID == dwID_Skill
		|| pNetCmd->dwID == dwID_UseItem
		|| pNetCmd->dwID == dwID_SwapWeapon
		|| pNetCmd->dwID == dwID_PosCanGoTo
		|| pNetCmd->dwID == dwID_GetRemoteRoleState
		|| pNetCmd->dwID == dwID_Stall)
	{
		// 有些新指令可以取代队列中的老指令
		if( pNetCmd->dwID == dwID_MouseMove
			|| pNetCmd->dwID == dwID_KeyboardMove
			|| pNetCmd->dwID == dwID_StopKeyboardWalk
			|| pNetCmd->dwID == dwID_Slide
			|| pNetCmd->dwID == dwID_SwapWeapon)
		{
			DWORD dwMsgSize=0;
			tagNetCmd* pSendCmd=NULL;
			m_SendQueueAction.ResetIterator();
			while(pSendCmd=(tagNetCmd*)m_SendQueueAction.PeekMsg(dwMsgSize))
			{
				if( pSendCmd->dwID==pNetCmd->dwID )
					m_SendQueueAction.RemoveMsg();
				else
					m_SendQueueAction.IncIterator();
			}
		}

		// 发送
		m_SendQueueAction.AddMsg(pNetCmd, pNetCmd->dwSize);
	}
	else
	{
		if( pNetCmd->dwID == dwID_GetRemoteRoleState 
			|| pNetCmd->dwID == dwID_RoleGetSomeName )
			m_SendQueueIdle.AddMsg(pNetCmd, pNetCmd->dwSize);
		else
		{
			// 宠物使用技能只能保存最新的一条（删除掉队列中所有的宠物技能消息）
			if(pNetCmd->dwID == dwID_PetSkillUse)
			{
				tagNetCmd* pSendCmd = NULL;
				DWORD dwMsgSize = 0;
				m_SendQueue.ResetIterator();
				while( pSendCmd = (tagNetCmd*)m_SendQueue.PeekMsg(dwMsgSize) )
				{
					if( pSendCmd->dwID  == dwID_PetSkillUse )	// 清除掉所有的宠物技能消息
						m_SendQueue.RemoveMsg();
					else
						m_SendQueue.IncIterator();
				}
				m_SendQueue.ResetIterator();
			}
			m_SendQueue.AddMsg(pNetCmd, pNetCmd->dwSize);
		}
	}
}


INT NetSession::GetSendQueueSize()
{
	return m_SendQueue.GetMsgNum();
}

INT NetSession::GetSendQueueActionSize()
{
	return m_SendQueueAction.GetMsgNum();
}

INT NetSession::GetSendQueueIdleSize()
{
	return m_SendQueueIdle.GetMsgNum();
}

VOID NetSession::ClearMsgQueue()
{
	static DWORD dwID = m_pUtil->Crc32("NC_GameGuarder");	// 反外挂消息

	m_SendQueueAction.Clear();
	m_SendQueueIdle.Clear();
	
	tagNetCmd* pSendCmd = NULL;
	DWORD dwMsgSize = 0;
	m_SendQueue.ResetIterator();
	while( pSendCmd = (tagNetCmd*)m_SendQueue.PeekMsg(dwMsgSize) )
	{
		if( pSendCmd->dwID != dwID )	// 清除掉反外挂以外所有的消息
			m_SendQueue.RemoveMsg();
		else
			m_SendQueue.IncIterator();
	}
}


// 显示sendQueue的信息
DWORD NetSession::SendQueue(LPCTSTR szCommand)
{
	DWORD dwID = _tstoi(szCommand);
	tagNetCmd* pSendCmd = NULL;
	DWORD dwMsgSize = 0;

	switch(dwID)
	{
	case 1:
		m_SendQueue.ResetIterator();
		while( pSendCmd = (tagNetCmd*)m_SendQueue.PeekMsg(dwMsgSize) )
		{
			IMSG(_T("SendQueue: 0x%X \r\n"), pSendCmd->dwID);
			m_SendQueue.IncIterator();
		}
		break;
	case 2:
		m_SendQueueIdle.ResetIterator();
		while( pSendCmd = (tagNetCmd*)m_SendQueueIdle.PeekMsg(dwMsgSize) )
		{
			IMSG(_T("SendQueueIdle: 0x%X \r\n"), pSendCmd->dwID);
			m_SendQueueIdle.IncIterator();
		}
		break;
	default:
		m_SendQueueAction.ResetIterator();
		while( pSendCmd = (tagNetCmd*)m_SendQueueAction.PeekMsg(dwMsgSize) )
		{
			IMSG(_T("SendQueueAction: 0x%X \r\n"), pSendCmd->dwID);
			m_SendQueueAction.IncIterator();
		}
		break;
	}
	return 0;
}


DWORD NetSession::NetRecvServerInfo(tagNS_ServerInfo* pNetCmd, DWORD dwParam)
{	
	m_dwLoginMode = pNetCmd->dwLoginMode;
	m_bEnableExpressPay = pNetCmd->bEnableExpressPay;
	if(m_bEnableExpressPay)
		memcpy(m_szExpressPayUrl, pNetCmd->szExpressPayUrl, sizeof(m_szExpressPayUrl));
	else
		memset(m_szExpressPayUrl, 0, sizeof(m_szExpressPayUrl));
	m_bSpiritOn = pNetCmd->bEnableSpiritSys;
	memcpy(m_szTodayFocusUrl, pNetCmd->szTodayHotActivityUrl, sizeof(m_szTodayFocusUrl));
	m_bEnableCitySys = pNetCmd->bEnableCitySys;
	m_dwAutoFightTimeSpace = pNetCmd->dwAutoFightTimeInterval;
	m_bEnableGuildBattle = pNetCmd->bGuildWarOpen;

	m_bsServerFunctionSwitch = pNetCmd->dwServerFunctionSwitch;
	return 0;
}

int	NetSession::GetSkillTimes()
{
	return m_nSkillNumber;
}

VOID NetSession::StartWGChecking(bool bStart)
{
	m_bStartCheck = bStart;
	m_nSkillNumber = 0;
}


int	NetSession::GetSkillTimes_Strategy2()
{
	return m_nSkillNumber_Strategy2;
}

VOID NetSession::StartWGChecking_Strategy2(bool bStart)
{	
	m_bStartCheck_Strategy2 = bStart;
	m_nSkillNumber_Strategy2 = 0;
}

DWORD NetSession::OnNS_KoreaCSApprove(tagNS_KoreaCSApprove* pNetCmd, DWORD dwParam)
{
#ifdef ON_GAMEGUARD
 	tagNS_KoreaCSApprove* pCmd = (tagNS_KoreaCSApprove*)pNetCmd;
 	g_npgl.Auth3((PBYTE)pCmd->byData, pCmd->dwSize - sizeof(tagNS_KoreaCSApprove), 0);
#endif
 	return 0;
}

