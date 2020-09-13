//-----------------------------------------------------------------------------
//!\file proof_policy_corn.h
//!\author lamoair
//!
//!\date 2010-5-10
//! last 2010-5-10
//!
//!\brief 台湾OMG登入器 验证策略 
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.

#pragma once

//-----------------------------------------------------------------------------
// 该验证策略的数据库
//-----------------------------------------------------------------------------
class ProofPocilyOmgDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPocilyOmgDB() {}
	~ProofPocilyOmgDB() {}

	//-------------------------------------------------------------------------
	// 初始化
	//-------------------------------------------------------------------------
	BOOL	Init();

	//-------------------------------------------------------------------------
	// 查询
	//-------------------------------------------------------------------------
	BOOL	QueryAccount(LPCSTR szAccountName,LPCSTR szPsd, tagProofResult* pResult);

private:
	DataBase		m_DB;
};

//-----------------------------------------------------------------------------
// 验证策略
//-----------------------------------------------------------------------------
class ProofPolicyOmg : public ProofPolicy
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyOmg() : m_Trunk(this) {}
	virtual ~ProofPolicyOmg() { }

public:
	//-------------------------------------------------------------------------
	// 初始化和销毁
	//-------------------------------------------------------------------------
	BOOL	Init(PROOFCALLBACK fn);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// 验证接口
	//-------------------------------------------------------------------------
	VOID	Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*混服运营商ID*/,LPCSTR szMac,LPCSTR szUserID = NULL,LPCSTR szAppKey = NULL, LPCSTR szToken = NULL);

	//-------------------------------------------------------------------------
	// 验证服务器连接状态
	//-------------------------------------------------------------------------
	INT		GetProofServerStatus();

	//-------------------------------------------------------------------------
	// 更新
	//-------------------------------------------------------------------------
	VOID	ThreadUpdate();

	//-------------------------------------------------------------------------
	// 玩家登出事件
	//-------------------------------------------------------------------------
	VOID	SigPlayerLogoutEvent(DWORD dwAccountID){}
    
private:
	TSFPTrunk<ProofPolicyOmg>		m_Trunk;
	TObjRef<Thread>					m_pThread;

	//-------------------------------------------------------------------------
	// 线程结束标志
	//-------------------------------------------------------------------------
	volatile BOOL					m_bTerminateUpdate;
    
	//-------------------------------------------------------------------------
	// 验证数据
	//-------------------------------------------------------------------------
	struct tagPlayerProofData
	{
		DWORD		dwClientID;
		string		strAccoutName;
		string		strPsd; //
		string      strGUID;
		string      strMac;


		tagPlayerProofData(DWORD _dwClientID, LPCSTR szAccoutName,LPCSTR szPsd,LPCSTR szGUID,LPCSTR szMac)
		{
			dwClientID		=	_dwClientID;
			strAccoutName	=	szAccoutName;
			strPsd	=	szPsd;
			strGUID = szGUID;
			strMac = szMac;
		}
	};

	TSafeMutexList<tagPlayerProofData*>	m_listProofData;		// 验证用户数据

	//--------------------------------------------------------------------------
	// 数据库
	//--------------------------------------------------------------------------
	ProofPocilyOmgDB					m_ProofDB;				// 数据库接口
};