#pragma once

#define BUF_CHATLOG_SIZE (1024*4)

class MsgCmd;
class ChatLogger
{
	ChatLogger();
	ChatLogger(const ChatLogger&);
	ChatLogger& operator=(const ChatLogger&);
	BOOL						m_bOn;
	TSFPTrunk<ChatLogger>		m_Trunk;
	MsgQueueTS*					m_pMsgQueue;
	TObjRef<Thread>				m_pThread;
	volatile BOOL						m_bTerminated;
	TCHAR						m_pBuf[BUF_CHATLOG_SIZE];
	TCHAR					    m_szDefaultName[1024];
	int							m_nIndex;
	TObjRef<Log>				m_Logger;
	DWORD						m_dwChatCmdID;
	DWORD						m_dwEquipCmdID;
	DWORD						m_dwItemCmdID;
	SOCKET                      sockClient; 
	SOCKADDR_IN                 addrSrv; 
	tagDWORDTime				m_InitTime;
protected:
	UINT ThreadWriteLog();
	void FlushToDisk(); //none thread safe
	LPTSTR Format(const MsgCmd& msgCmd);
	void CreateDir(const TCHAR* Path);
	LPTSTR CreateLogFileName();
public:	
	static ChatLogger& getInstance();
	void Log(const MsgCmd& msgCmd);
	BOOL Init();
	VOID Update();
	void Destroy();
	
};

#define TheChatLogger (ChatLogger::getInstance())