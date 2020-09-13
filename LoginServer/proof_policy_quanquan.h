#pragma once

#include "proof_policy.h"

//-----------------------------------------------------------------------------
// 该验证策略的数据库
//-----------------------------------------------------------------------------
class ProofPolicyQuanQuanDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyQuanQuanDB() {}
	~ProofPolicyQuanQuanDB() {}

	//-------------------------------------------------------------------------
	// 初始化
	//-------------------------------------------------------------------------
	BOOL	Init();

	//-------------------------------------------------------------------------
	// 查询
	//-------------------------------------------------------------------------
	BOOL	QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pResult);
	BOOL	InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);
	BOOL	UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);


private:
	DataBase		m_DB;
};

struct ISDOAUserInfoAuthen;
struct ISDOAMsg;
//-----------------------------------------------------------------------------
// 盛大圈圈验证策略
//-----------------------------------------------------------------------------
class ProofPolicyQuanQuan :
	public ProofPolicy
{
	typedef ISDOAUserInfoAuthen* (*pFuncCreateUserInfoAuthen)(void);
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyQuanQuan(void): m_Trunk(this), m_bTerminateUpdate(FALSE){}
	~ProofPolicyQuanQuan(void){}

public:
	//-------------------------------------------------------------------------
	// 初始化和销毁
	//-------------------------------------------------------------------------
	BOOL	Init(PROOFCALLBACK fn);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// 验证接口
	//-------------------------------------------------------------------------
	VOID	Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP, DWORD dwWorldNameCrc,DWORD dwChannelID/*混服运营商ID*/,LPCSTR szMac,LPCSTR szUserID = NULL,LPCSTR szAppKey = NULL, LPCSTR szToken = NULL);

	//-------------------------------------------------------------------------
	// 验证服务器连接状态
	//-------------------------------------------------------------------------
	INT		GetProofServerStatus();

	//-------------------------------------------------------------------------
	// 玩家登出事件
	//-------------------------------------------------------------------------
	VOID	SigPlayerLogoutEvent(DWORD dwAccountID){}

private:
	//-------------------------------------------------------------------------
	// 更新
	//-------------------------------------------------------------------------
	VOID	ThreadUpdate();

	//-------------------------------------------------------------------------
	// 连接线程
	//-------------------------------------------------------------------------
	VOID	ThreadConnectServer();

	//-------------------------------------------------------------------------
	// 更新验证列表
	//-------------------------------------------------------------------------
	VOID	UpdateProofList();

	//-------------------------------------------------------------------------
	// 更新消息
	//-------------------------------------------------------------------------
	VOID	UpdateSession();

	//-------------------------------------------------------------------------
	// 发送金牌账号信息
	//-------------------------------------------------------------------------
	VOID	ThreadUpdateGoldenList();

	//-------------------------------------------------------------------------
	// 发送金牌账号信息到服务器
	//-------------------------------------------------------------------------
	bool	SendGloden2Server(string account, DWORD sectionID, DWORD worldID); // UDP


	int OnUserInfoCallback(int nResult, unsigned long nUniqueId, ISDOAMsg *pMsg);

	char* IP2String(DWORD dwIP)const;
	string GetConfigFile()const;
private:
	HINSTANCE					m_hShengDaDll; //shengda dll
	pFuncCreateUserInfoAuthen	m_pFuncCreate;

	TSFPTrunk<ProofPolicyQuanQuan>	m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<Thread>				m_pThread;
	TObjRef<StreamTransport>	m_pTransport;
	TObjRef<Thread>				m_pThreadGolden;

	//-------------------------------------------------------------------------
	// 线程结束标志
	//-------------------------------------------------------------------------
	volatile BOOL				m_bTerminateUpdate;
	
	ISDOAUserInfoAuthen*		m_pAuth;

	//-------------------------------------------------------------------------
	// 验证数据
	//-------------------------------------------------------------------------
	struct tagPlayerProofData
	{
		DWORD		dwClientID;
		BYTE		byAccountData[1024];		//加密包	由盛大公司提供的登录器产生
		DWORD		dwIP;
		DWORD		dwWordNameCrc;
		string		strMac;

		tagPlayerProofData(DWORD _dwClientID, LPCSTR szGUID, DWORD IP, DWORD _dwWordNameCrc, LPCSTR szMac)
		{
			dwClientID		=	_dwClientID;
			dwIP			=	IP;
			strcpy_s((char*)byAccountData,1024, szGUID);
			byAccountData[1023] = 0;
			dwWordNameCrc = _dwWordNameCrc;
			strMac			=	szMac;
		}
	};

	enum ProofResult
	{
		E_Success,
		E_ConnectServer,
		E_EmptyField,
		E_AlreadyAuthrized,
		E_AuthFailed,
		E_WrongFormat,
		E_OutOfTime,
		E_SysError,
		E_SocketError,
	};

	struct tagPlayerProofResult
	{
		DWORD		dwClientID;
		string strAccountName;
		ProofResult eResult;
		bool bGuard;
		BYTE	byNeedShengDaInfo;
	};

	struct tagGoldenMember
	{
		string strAccountName;
		DWORD	dwWorldNameCrc;
	};

	TSafeList<tagPlayerProofData*>		m_listProofData;		// 验证用户数据
	TSafeList<tagPlayerProofResult*>	m_listProofResult;		// 验证结果数据
	TSafeList<tagGoldenMember*>	m_listGoldenMemberList;		// 金牌用户数据
	TSafeMap<DWORD, DWORD>		m_mapClientID2World;

	SOCKET                      m_hSockClient; 
	SOCKADDR_IN                 m_addrSrv; 

	//--------------------------------------------------------------------------
	// 数据库
	//--------------------------------------------------------------------------
	ProofPolicyQuanQuanDB					m_ProofDB;				// 数据库接口
};

inline char* ProofPolicyQuanQuan::IP2String(DWORD dwIP)const
{
	in_addr addr;
	addr.S_un.S_addr = dwIP;
	return inet_ntoa(addr);
}