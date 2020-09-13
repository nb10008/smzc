//-----------------------------------------------------------------------------
//!\file player_net_cmd_mgr.h
//!\author Aslan
//!
//!\date 2008-06-13
//! last 2008-06-13
//!
//!\brief 客户端命令管理器，主要解决NetCmdMgr无法针对多对象消息处理的缺陷
//!
//!	Copyright (c) 2004 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

class PlayerSession;

typedef DWORD (PlayerSession::*NETMSGHANDLER)(tagNetCmd*);

//-----------------------------------------------------------------------------
// 客户端命令管理器
//-----------------------------------------------------------------------------
class PlayerNetCmdMgr
{
public:
	PlayerNetCmdMgr();
	~PlayerNetCmdMgr();

	static VOID Init();
	VOID Destroy();

	VOID LogAllMsg();

	VOID LogNetAnalyze(INT nFrom);

	BOOL RegisterRecvProc(LPCSTR szCmd, NETMSGHANDLER fp, LPCTSTR szDesc, DWORD dwSize, DWORD dwTimeInterval = 0);
	BOOL RegisterSendProc(LPCSTR szCmd);
	VOID UnRegisterAll();

	NETMSGHANDLER GetHandler(tagNetCmd* pMsg, UINT32 nMsgSize, DWORD dwLastTime);
	VOID CountServerMsg(DWORD dwMsgID);

	BOOL HandleCmd(tagNetCmd* pMsg, DWORD nMsgSize, PlayerSession* pSession);

	// 取得消息执行次数
	UINT32 GetRecvCmdRunTimes(DWORD dwMsgID);

	VOID AddCmdRunTime(DWORD dwMsgID, LARGE_INTEGER nTime);

protected:
	typedef struct tagPlayerCmd
	{
		std::string				strCmd;			// 命令名
		tstring					strDesc;		// 描述
		DWORD					dwSize;			// 消息大小
		NETMSGHANDLER			handler;		// 函数指针
		DWORD					dwTimeInterval;	// 命令执行时间间隔 [7/4/2012 zhangzhihua]
		volatile UINT32			nTimes;			// 此命令的次数
		volatile UINT32			nTimesPer;		// 此命令的次数
		volatile UINT32			nTotalSize;		// 此命令的大小和
		volatile UINT32			nMaxTime;		// 此命令的单次执行时间最大值
		volatile LARGE_INTEGER	nTotalTime;		// 此命令时间段内的执行时间和
	} tagPlayerCmd;

	TObjRef<Util>				m_pUtil;
	TObjRef<Log>				m_pLog;

	TMap<DWORD, tagPlayerCmd*>	m_mapRecvProc;	// 接收消息的处理及统计
	TMap<DWORD,	tagPlayerCmd*>	m_mapSendProc;	// 发送消息的处理及统计

private:
	// 网络包记录相关 [6/11/2012 zhangzhihua]
	static BOOL					m_bNetCmdLog;	// 是否开启log
	static DWORD				m_dwNetCmdLogTime;// 日志记录时间间隔
	DWORD						m_dwNetLogTimes;// 日志记录次数
	DWORD						m_dwRecordTime;	// 记录时间
};