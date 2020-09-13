//-----------------------------------------------------------------------------
//!\file proof_policy_corn.h
//!\author lamoair
//!
//!\date 2010-5-10
//! last 2010-5-10
//!
//!\brief 韩国登入器 验证策略 
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.

#pragma once

#include "proof_policy_corn_define.h"
//-----------------------------------------------------------------------------
// 该验证策略的数据库
//-----------------------------------------------------------------------------
class ProofPocilyCornDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPocilyCornDB() {}
	~ProofPocilyCornDB() {}

	//-------------------------------------------------------------------------
	// 初始化
	//-------------------------------------------------------------------------
	BOOL	Init();

	//-------------------------------------------------------------------------
	// 查询
	//-------------------------------------------------------------------------
	BOOL    QueryAccount(const string& name,string& seckey); 
	BOOL	QueryAccount(LPCSTR szAccountName,LPCSTR szPsd, tagProofResult* pResult);
	BOOL	InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);
	BOOL	UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);

private:
	DataBase		m_DB;
};

//-----------------------------------------------------------------------------
// 验证策略
//-----------------------------------------------------------------------------
class ProofPolicyCorn : public ProofPolicy
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyCorn() : m_Trunk(this),m_nTimeOut(0) {}
	virtual ~ProofPolicyCorn() { }

public:
	//-------------------------------------------------------------------------
	// 初始化和销毁
	//-------------------------------------------------------------------------
	BOOL	Init(PROOFCALLBACK fn);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// 验证接口
	//-------------------------------------------------------------------------
	VOID	Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID,LPCSTR szMac,LPCSTR szUserID = NULL,LPCSTR szAppKey = NULL, LPCSTR szToken = NULL);

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

	INT     GetTimeOut()   { return m_nTimeOut ;}
	//-------------------------------------------------------------------------
	//把加密数据转换到字符串
	//-------------------------------------------------------------------------
	BOOL    DecryptDataToString(OUT tagDecryptData* pDecryptData,IN const CHAR* pSrc,const CHAR* pPsd);
    
	//-------------------------------------------------------------------------
	//从加密数据解析出字符串
	//-------------------------------------------------------------------------
	BOOL    ParseStringFromDecryptData(OUT tagDecryptData* pDecryptData,IN const CHAR* pSrc);
private:
	TSFPTrunk<ProofPolicyCorn>		m_Trunk;
	TObjRef<Thread>					m_pThread;

	//-------------------------------------------------------------------------
	// 线程结束标志
	//-------------------------------------------------------------------------
	volatile BOOL					m_bTerminateUpdate;
    
	INT                             m_nTimeOut;
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
	ProofPocilyCornDB					m_ProofDB;				// 数据库接口
};
