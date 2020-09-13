//-----------------------------------------------------------------------------
//!\file proof_policy_russia.h
//!\author liutingting
//!
//!\date 2011-5-10
//! last 2011-5-10
//!
//!\brief 俄罗斯验证策略
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#include "proof_policy.h"
#include "game_net_web.h"

//-----------------------------------------------------------------------------
// 该验证策略的数据库
//-----------------------------------------------------------------------------
class ProofPolicyRussiaDB
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyRussiaDB() {}
	~ProofPolicyRussiaDB() {}

	//-------------------------------------------------------------------------
	// 初始化
	//-------------------------------------------------------------------------
	BOOL	Init();

	//-------------------------------------------------------------------------
	// 查询
	//-------------------------------------------------------------------------
	BOOL	QueryAccount(LPCSTR szAccountName, tagProofResult* pResult);
	BOOL	InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);
	BOOL	UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard);
	//BOOL	UpdatePsd(LPCSTR szAccountName, LPCSTR szNewPsd);
	//BOOL	UpdateMibao(LPCSTR szAccountName, LPCSTR szMibao);

private:
	DataBase		m_DB;
};

//-----------------------------------------------------------------------------
// 验证策略
//-----------------------------------------------------------------------------
class ProofPolicyRussia : public ProofPolicy
{
public:
	//-------------------------------------------------------------------------
	// CONSTRUCT
	//-------------------------------------------------------------------------
	ProofPolicyRussia() : m_Trunk(this), m_bTerminateUpdate(FALSE){}
	~ProofPolicyRussia() {}

public:
	//-------------------------------------------------------------------------
	// 初始化和销毁
	//-------------------------------------------------------------------------
	BOOL	Init(PROOFCALLBACK fn);
	VOID	Destroy();

	//-------------------------------------------------------------------------
	// 验证接口
	//-------------------------------------------------------------------------
	VOID	Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*混服运营商ID*/,LPCSTR szMac, LPCSTR szUserID = NULL,LPCSTR szAppKey = NULL, LPCSTR szToken = NULL);

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
	// 初始化配置
	//-------------------------------------------------------------------------
	BOOL	InitConfig();

private:
	TSFPTrunk<ProofPolicyRussia>	m_Trunk;
	TObjRef<Thread>					m_pThread;
	TObjRef<Util>					m_pUtil;


	//-------------------------------------------------------------------------
	// 验证数据
	//-------------------------------------------------------------------------
	struct tagPlayerProofData
	{
		DWORD		dwClientID;
		CHAR		szUserID[X_SHORT_NAME];
		CHAR		szAppKey[48];
		CHAR		szToken[48]; 

		tagPlayerProofData( DWORD dwClientIDIn, LPCSTR szInUserID,LPCSTR szInAppKey, LPCSTR szInToken )
		{
			dwClientID = dwClientIDIn;

			if(P_VALID(szInUserID))
				lstrcpynA( szUserID,szInUserID,X_SHORT_NAME);

			if(P_VALID(szInAppKey))
				lstrcpynA( szAppKey, szInAppKey,48);

			if(P_VALID(szInToken))
				lstrcpynA( szToken, szInToken, 48);
		}
	};

	TSafeMutexList<tagPlayerProofData*>	m_listProofData;		// 验证用户数据

	//-------------------------------------------------------------------------
	// 线程结束标志
	//-------------------------------------------------------------------------
	volatile BOOL				m_bTerminateUpdate;

	//--------------------------------------------------------------------------
	// 数据库
	//--------------------------------------------------------------------------
	ProofPolicyRussiaDB					m_ProofDB;				// 数据库接口
	GameNetWeb							m_GameNetWeb;			// http交互用
	CHAR								m_szUrl[256];			// 连接GameNetWeb的地址
};