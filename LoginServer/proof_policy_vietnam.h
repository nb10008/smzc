//-----------------------------------------------------------------------------
//!\file proof_policy_vietnam.h
//!\author lamoair
//!
//!\date 2010-5-10
//! last 2010-5-10
//!
//!\brief 越南登入器 验证策略 
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.


#pragma once
#include "proof_policy.h"

#define X_OG_RECEIVE_DIVIDE_SIGN ';' //接收橘子消息的分隔符
#define X_OG_PROOF_SIGN "1"    //验证信息

//-----------------------------------------------------------------------------
// 该验证策略的数据库
//-----------------------------------------------------------------------------
class ProofPolicyVietnamDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyVietnamDB() {}
	~ProofPolicyVietnamDB() {}

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

class ProofPolicyVietnam : public ProofPolicy
{
public:
	ProofPolicyVietnam(void): m_Trunk(this), m_nLoginPort(0), m_bTerminateLoginUpdate(FALSE), m_bTerminateLoginConnect(FALSE){}
	virtual ~ProofPolicyVietnam(void){}

public:
	//-------------------------------------------------------------------------
	// 初始化和销毁
	//-------------------------------------------------------------------------
	BOOL	Init(PROOFCALLBACK fn);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// 验证接口
	//-------------------------------------------------------------------------
	VOID	Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID,DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*混服运营商ID*/,LPCSTR szMac,LPCSTR szUserID = NULL,LPCSTR szAppKey = NULL, LPCSTR szToken = NULL);

	//-------------------------------------------------------------------------
	// 验证服务器连接状态
	//-------------------------------------------------------------------------
	INT		GetProofServerStatus();

	//-------------------------------------------------------------------------
	// 玩家登出事件
	//-------------------------------------------------------------------------
	VOID	SigPlayerLogoutEvent(DWORD dwAccountID) {  ; }

private:

	//-----------------------------------------------------------------------------------------------
	// 得到各个数据项 单引号分隔
	//-----------------------------------------------------------------------------------------------
	VOID ParseVietnamProofData(const string &strFrameString,vector <string> &vecOrangeData,char cDivideSign);

	//-------------------------------------------------------------------------
	// 更新login服务器的线程
	//-------------------------------------------------------------------------
	VOID	ThreadLoginServerUpdate();

	//-------------------------------------------------------------------------
	// 连接对方login服务器线程
	//-------------------------------------------------------------------------
	VOID	ThreadConnectLoginServer();

	//-------------------------------------------------------------------------
	// 初始化配置
	//-------------------------------------------------------------------------
	BOOL	InitConfig();

	//-------------------------------------------------------------------------
	// 更新验证列表
	//-------------------------------------------------------------------------
	VOID	UpdateProofList();

	//-------------------------------------------------------------------------
	// 更新login服务器消息
	//-------------------------------------------------------------------------
	VOID	UpdateLoginSession();


	//-------------------------------------------------------------------------
	// 处理Orange验证信息
	//-------------------------------------------------------------------------
	VOID	HandleVietnamLogin(LPBYTE pRecv);


	//-----------------------------------------------------------------------------------------------
	// 去掉回车换行
	//-----------------------------------------------------------------------------------------------
	BOOL CheckString(string& str);

private:

	TSFPTrunk<ProofPolicyVietnam>	m_Trunk;
	TObjRef<Util>				m_pUtil;
	TObjRef<Thread>				m_pThread;
	//登入
	TObjRef<StreamTransport>	m_pTransLogin;
	//在ThreadUpdate工作 不需要考虑线程安全
	std::map<DWORD, string>		m_mapClientIDVsAccount;	// ClientID和玩家账号的对照表
	//-------------------------------------------------------------------------
	// 线程结束标志
	//-------------------------------------------------------------------------
	volatile BOOL				m_bTerminateLoginUpdate;
	volatile BOOL				m_bTerminateLoginConnect;

	//-------------------------------------------------------------------------
	// 连接参数
	//-------------------------------------------------------------------------
	CHAR						m_szLoginIP[X_IP_LEN];		// 连接越南认证服务器的IP
	INT							m_nLoginPort;				// 连接越南认证服务器的端口
	//-----------------------------------------------------------------------
	// 验证数据
	//-------------------------------------------------------------------------
	struct tagPlayerProofData
	{
		DWORD		dwClientID;
		string		strAccoutName;
		string		strPsd;
		BYTE		byAccountData[X_LONG_NAME];		//
		DWORD		dwIP;				//ip
		DWORD		dwWorldNameCrc;
		string		strMac;

		tagPlayerProofData(DWORD _dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID,DWORD _dwIP,DWORD _dwWorldNameCrc, LPCSTR szMac)
		{
			dwClientID		=	_dwClientID;
			strAccoutName	=	szAccoutName;
			strPsd			=	szPsd;
			memcpy(byAccountData,szGUID,X_LONG_NAME);
			dwIP			=	_dwIP;
			dwWorldNameCrc	=	_dwWorldNameCrc;
			strMac			=	szMac;
		}
	};

	TSafeList<tagPlayerProofData*>		m_listProofData;		// 验证用户数据
	TSafeList<tagPlayerProofData*>		m_listProofSolvedData;	// 处理完数据下层处理

	
	//--------------------------------------------------------------------------
	// 数据库
	//--------------------------------------------------------------------------
	ProofPolicyVietnamDB					m_ProofDB;				// 数据库接口
};
