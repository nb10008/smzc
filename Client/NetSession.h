//-----------------------------------------------------------------------------
//!\file NetSession.h
//!\author Lyp
//!
//!\date 2008-06-10
//! last 2008-06-12
//!
//!\brief net session
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "../WorldDefine/msg_serverinfo.h"
#include "../GameGuard/NPGameLib.h"
struct tagNS_KoreaCSApprove;
//-----------------------------------------------------------------------------
//!\brief net session
//! 控制发包频率在每次心跳一个，同时提供监控，可以查看有哪些消息累积
//! 可以方便的替换队列中的消息
//-----------------------------------------------------------------------------
class NetSession
{
public:
	VOID Init();
	VOID Connect(LPCTSTR szIP, INT nPort);
	VOID Disconnect();
	BOOL IsConnect();
	VOID ClearMsgQueue();

	VOID Update();

	VOID Send(tagNetCmd* pNetCmd);
	INT	GetSendQueueSize();
	INT	GetSendQueueActionSize();
	INT	GetSendQueueIdleSize();

	// 初始化服务器信息
	DWORD NetRecvServerInfo(tagNS_ServerInfo* pNetCmd, DWORD dwParam);
	DWORD OnNS_KoreaCSApprove(tagNS_KoreaCSApprove* pNetCmd, DWORD dwParam);


	// 得到远程IP
	LPCTSTR GetRemoteIPString() { return m_strRemoteIP.c_str(); }

	
	NetSession():m_Trunk(this){}
	~NetSession();

	BOOL IsEnableExpressPay()const { return m_bEnableExpressPay; }
	const TCHAR* GetExpressPayUrl() const { return m_szExpressPayUrl; }
	DWORD	GetLoginMode() const { return m_dwLoginMode; }
	BOOL	IsSpiritOn() const { return m_bSpiritOn; }
	const TCHAR* GetTodayFocusUrl() const { return m_szTodayFocusUrl; }
	BOOL	IsEnableCitySys() const { return m_bEnableCitySys; }
	const DWORD	GetAutoFightTimeSpace() const { return m_dwAutoFightTimeSpace; }
	BOOL	IsEnableGuildBattle() const { return m_bEnableGuildBattle; }
	BOOL	IsEnableShopingCart() const { return m_bsServerFunctionSwitch.at(ESFS_MallCart); }

	VOID StartWGChecking(bool bStart);// 开始外挂检测
	int	GetSkillTimes();

	VOID StartWGChecking_Strategy2(bool bStart);// 开始外挂检测 策略2
	int	GetSkillTimes_Strategy2();

protected:
	TSFPTrunk<NetSession>		m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<Console>			m_pConsole;
	TObjRef<XClient>			m_pStream;
	TObjRef<NetCmdMgr>			m_pCmdMgr;

	tstring						m_strRemoteIP;
	DWORD						m_dwLastSendTime;

	MsgQueue					m_SendQueueAction;	// 行为队列
	MsgQueue					m_SendQueue;		// 普通队列
	MsgQueue					m_SendQueueIdle;	// 次要消息队列

	TCHAR						m_szExpressPayUrl[256];
	BOOL						m_bEnableExpressPay;
	DWORD						m_dwLoginMode;
	BOOL						m_bSpiritOn;
	TCHAR						m_szTodayFocusUrl[256];
	BOOL						m_bEnableCitySys;
	DWORD						m_dwAutoFightTimeSpace;
	BOOL						m_bEnableGuildBattle;

	int							m_nSkillNumber;
	bool						m_bStartCheck;

	int							m_nSkillNumber_Strategy2;
	bool						m_bStartCheck_Strategy2;

	BitSetServerFunctionSwitch			m_bsServerFunctionSwitch;	//各种开关

	DWORD SendQueue(LPCTSTR szCommand);	// 显示sendQueue的信息


};

#ifdef ON_GAMEGUARD
extern CNPGameLib g_npgl;
#endif