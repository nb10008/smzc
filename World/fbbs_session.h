/********************************************************************
	created:	2010/11/24
	created:	24:11:2010   15:26
	filename: 	e:\F-Project\src\World\fbbs_session.h
	file path:	e:\F-Project\src\World
	file base:	fbbs_session
	file ext:	h
	author:		Jason
	
	purpose:	facebook边界服务器(boundary server)会话层
*********************************************************************/
#ifndef fbbs_session_h__
#define fbbs_session_h__

class FBBSSession
{
public:
	FBBSSession();
	~FBBSSession();
public:
	BOOL Init();
	VOID Destroy();

	VOID Update();
	BOOL IsWell() { return (m_bInitOK && m_pTran->IsConnected()); }

	VOID Send(LPVOID pMsg,INT size);

	VOID NotifyFBBS(DWORD dwRoleID,INT nCmdID,INT nParam = 0);

private:
	TSFPTrunk<FBBSSession>	m_Trunk;
	TObjRef<Util>						m_pUtil;
	TObjRef<StreamTransport>			m_pTran;
	TObjRef<Thread>					m_pThread;
	// 连接参数
	CHAR									m_szIP[X_IP_LEN];	// 服务器ip
	DWORD									m_dwPort;			// 服务器port
	DWORD									m_dwOn;				// fbbs开关
	DWORD									m_dwWorldID;		// 服务器ID

	volatile BOOL				m_bTermConnect;			// 记录当前连接状态
	volatile BOOL				m_bInitOK;					// 是否初始化完成

	// 连接facebook边界服务器
	DWORD ThreadConnectFBBS();
};

extern FBBSSession g_FBBSSession;

#endif // fbbs_session_h__

