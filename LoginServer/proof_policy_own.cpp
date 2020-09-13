//-----------------------------------------------------------------------------
//!\file proof_policy_own.cpp
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief 自己公司的验证策略——中央激活
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

#include "../ServerDefine/base_define.h"
#include "../ServerDefine/msg_rt_errorcode.h"
#include "proof_policy_own_define.h"
#include "proof_policy.h"
#include "proof_policy_own.h"
#include "login_server.h"
#include "db.h"
#include <fstream>
//-------------------------------------------------------------------------
// 初始化
//-------------------------------------------------------------------------
BOOL ProofPolicyOwnDB::Init()
{
	TObjRef<VarContainer> pVar = "LoginServerConfig";
	if( !P_VALID(pVar) ) return FALSE;

	BOOL bRet = m_DB.Init(pVar->GetString(_T("ip database")), 
		pVar->GetString(_T("user database")),
		pVar->GetString(_T("psd database")), 
		pVar->GetString(_T("name database")),
		(INT)pVar->GetDword(_T("port database")) );

	return bRet;
}

//-------------------------------------------------------------------------
// 查询
//-------------------------------------------------------------------------
BOOL ProofPolicyOwnDB::QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pProofResult)
{
	ILOG->Write(_T("ProofPolicyOwnDB::QueryAccount \r\n"));

#ifdef DEBUG
	try
	{
#endif
	if( !P_VALID(szAccountName) || !P_VALID(pProofResult) ) 
	{
		ILOG->Write(_T("ProofPolicyOwnDB::QueryAccount1 \r\n"));
		return FALSE;
	}

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) 
	{
		ILOG->Write(_T("ProofPolicyOwnDB::QueryAccount2 \r\n"));
		return FALSE;
	}

	TObjRef<StreamTransport> pTrans;
	pTrans = "StreamTransport";

	char szIP[X_IP_LEN] = "";
	char szDateTime[X_DATATIME_LEN + 1]= "";
	// 申请连接
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetSelect("account", "id,forbid_mask,privilege,guard,login_status,worldname_crc,psd,mibao,ip,time, savetelnum");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// 释放连接
	m_DB.ReturnConnection(pCon);

	QueryResult* pResult = m_DB.Query(pStream);

	// 释放流
	m_DB.ReturnStream(pStream);

	// 查询结果为空
	if( !P_VALID(pResult) || pResult->GetRowCount() <= 0)
	{
		m_DB.FreeQueryResult( pResult );
		ILOG->Write(_T("ProofPolicyOwnDB::QueryAccount3 \r\n"));
		return FALSE;
	}
	// 设置结果
	pProofResult->dwAccountID		=	(*pResult)[0].GetDword();
	pProofResult->dwFrobidMask		=	(*pResult)[1].GetDword();
	pProofResult->byPrivilege		=	(*pResult)[2].GetByte();
	pProofResult->bGuard			=	(*pResult)[3].GetBool();
	pProofResult->nGuardAccuTime	=	0;
	pProofResult->eLoginStatus		=	(EPlayerLoginStatus)(*pResult)[4].GetInt();
	pProofResult->dwWorldNameCrc	=	(*pResult)[5].GetDword();
	memcpy(pProofResult->szAccountName,szAccountName,X_SHORT_NAME);

	// 记录密码
	strPsd = (*pResult)[6].GetString();

	// 记录密保
	CHAR szMibao[MIBAO_LEN] = {'\0'};
	BOOL bRet = (*pResult)[7].GetBlob(szMibao, MIBAO_LEN);
	if( '\0' == szMibao[0] )
	{
		pProofResult->bNeedMibao = FALSE;
	}
	else
	{
		pProofResult->bNeedMibao = TRUE;
	}

	//上次登录的时间和ip
	memcpy(szIP,(*pResult)[8].GetString(),(*pResult)[8].GetLen());
	memcpy(szDateTime,(*pResult)[9].GetString(),(*pResult)[9].GetLen());

	pProofResult->dwPreLoginIP = pTrans->StringIP2IP(szIP);
	DataTime2DwordTime(pProofResult->dwPreLoginTime,szDateTime,(*pResult)[9].GetLen());

	pProofResult->bSaveTelNum		=	(*pResult)[10].GetByte();

	// 归还结果集
	m_DB.FreeQueryResult(pResult);
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test17.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif

	ILOG->Write(_T("ProofPolicyOwnDB::QueryAccount4 \r\n"));
	return TRUE;

}

//-------------------------------------------------------------------------
// 插入玩家记录
//-------------------------------------------------------------------------
BOOL ProofPolicyOwnDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
{
	ILOG->Write(_T("ProofPolicyOwnDB::InsertAccout \r\n"));

#ifdef DEBUG
	try
	{
#endif
	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) 
	{
		ILOG->Write(_T("ProofPolicyOwnDB::InsertAccout1 \r\n"));
		return FALSE;
	}

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) 
	{
		ILOG->Write(_T("ProofPolicyOwnDB::InsertAccout2 \r\n"));
		return FALSE;
	}

	// 申请连接
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetInsert("account");
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("',");
	pStream->FillString("psd='").FillString(szPsd, pCon).FillString("',");
	pStream->FillString("mibao='").FillBlob(szMiBao, MIBAO_LEN, pCon).FillString("',");
	pStream->FillString("guard=") << bGuard;

	// 释放连接
	m_DB.ReturnConnection(pCon);

	// 查询
	BOOL bRet = m_DB.Execute(pStream);

	// 释放流
	m_DB.ReturnStream(pStream);

	ILOG->Write(_T("ProofPolicyOwnDB::InsertAccout3 \r\n"));
	return bRet;
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test18.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
}

//----------------------------------------------------------------------------
// 更新玩家记录
//----------------------------------------------------------------------------
BOOL ProofPolicyOwnDB::UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard/*, BYTE byForbidMask */)
{
	ILOG->Write(_T("ProofPolicyOwnDB::UpdateAccout \r\n"));

#ifdef DEBUG
	try
	{
#endif

	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) 
	{
		ILOG->Write(_T("ProofPolicyOwnDB::UpdateAccout1 \r\n"));
		return FALSE;
	}

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) 
	{
		ILOG->Write(_T("ProofPolicyOwnDB::UpdateAccout2 \r\n"));
		return FALSE;
	}

	// 申请连接
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetUpdate("account");
	pStream->FillString("psd='").FillString(szPsd, pCon).FillString("',");
// 	pStream->FillString("forbid_mask=")<< byForbidMask;
// 	pStream->FillString(",mibao='").FillBlob(szMiBao, MIBAO_LEN, pCon).FillString("',");
	pStream->FillString("mibao='").FillBlob(szMiBao, MIBAO_LEN, pCon).FillString("',");
	pStream->FillString("guard=") << bGuard;
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// 释放连接
	m_DB.ReturnConnection(pCon);

	// 查询
	BOOL bRet = m_DB.Execute(pStream);

	// 释放流
	m_DB.ReturnStream(pStream);

	ILOG->Write(_T("ProofPolicyOwnDB::UpdateAccout3 \r\n"));
	return bRet;
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test19.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif

}

//-------------------------------------------------------------------------
// 更新密码
//-------------------------------------------------------------------------
BOOL ProofPolicyOwnDB::UpdatePsd(LPCSTR szAccountName, LPCSTR szNewPsd)
{
	ILOG->Write(_T("ProofPolicyOwnDB::UpdatePsd \r\n"));

#ifdef DEBUG
	try
	{
#endif

	if( !P_VALID(szAccountName) || !P_VALID(szNewPsd) ) 
	{
		ILOG->Write(_T("ProofPolicyOwnDB::UpdatePsd1 \r\n"));
		return FALSE;
	}

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) 
	{
		ILOG->Write(_T("ProofPolicyOwnDB::UpdatePsd2 \r\n"));
		return FALSE;
	}

	// 申请连接
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetUpdate("account");
	pStream->FillString("psd='").FillString(szNewPsd, pCon).FillString("'");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// 释放连接
	m_DB.ReturnConnection(pCon);

	// 查询
	BOOL bRet = m_DB.Execute(pStream);

	// 释放流
	m_DB.ReturnStream(pStream);

	ILOG->Write(_T("ProofPolicyOwnDB::UpdatePsd3 \r\n"));
	return bRet;
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test20.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
}

//-------------------------------------------------------------------------
// 更新密保
//-------------------------------------------------------------------------
BOOL ProofPolicyOwnDB::UpdateMibao(LPCSTR szAccountName, LPCSTR szMibao)
{
	ILOG->Write(_T("ProofPolicyOwnDB::UpdateMibao \r\n"));

#ifdef DEBUG
	try
	{
#endif
	if( !P_VALID(szAccountName) ) 
	{
		ILOG->Write(_T("ProofPolicyOwnDB::UpdateMibao1 \r\n"));
		return FALSE;
	}

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) 
	{
		ILOG->Write(_T("ProofPolicyOwnDB::UpdateMibao2 \r\n"));
		return FALSE;
	}

	// 申请连接
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetUpdate("account");
	pStream->FillString("mibao='").FillBlob(szMibao, MIBAO_LEN, pCon).FillString("'");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// 释放连接
	m_DB.ReturnConnection(pCon);

	// 查询
	BOOL bRet = m_DB.Execute(pStream);

	// 释放流
	m_DB.ReturnStream(pStream);

	ILOG->Write(_T("ProofPolicyOwnDB::UpdateMibao3 \r\n"));
	return bRet;
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test21.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
}

//-------------------------------------------------------------------------
// 初始化
//-------------------------------------------------------------------------
BOOL ProofPolicyOwn::Init(PROOFCALLBACK fn)
{
	m_pUtil		=	"Util";
	m_pThread	=	"Thread";

	m_fnCallback = fn;

	// 读取配置文件
	if( !InitConfig() )
	{
		return FALSE;
	}

	// 初始化数据库
	if( !m_ProofDB.Init() )
	{
		return FALSE;
	}

	// 生成网络命令管理器
	CreateObj("NetCmdMgrProofPolicyOwn", "NetCmdMgr");
	m_pCmdMgr = "NetCmdMgrProofPolicyOwn";
	if( !P_VALID(m_pCmdMgr) ) return FALSE;

	// 初始化网络
	CreateObj("StreamTransportProofPolicyOwn", "StreamTransport");
	m_pTransport = "StreamTransportProofPolicyOwn";
	if( !P_VALID(m_pTransport) ) return FALSE;
	m_pTransport->Init();

	// 注册网络命令
	RegisterProofMsg();

	// 启动连接线程
	if(!m_pThread->CreateThread(_T("ProofPolicyOwnConnectServer"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOwn::ThreadConnectServer), 
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ProofPolicyOwnConnectServer")))
	{
		continue;
	}

	// 启动更新线程
	if(!m_pThread->CreateThread(_T("ProofPolicyOwnThreadUpdate"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOwn::ThreadUpdate),
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ProofPolicyOwnThreadUpdate")))
	{
		continue;
	}

	return TRUE;
}

//-------------------------------------------------------------------------
// 初始化配置
//-------------------------------------------------------------------------
BOOL ProofPolicyOwn::InitConfig()
{
	ILOG->Write(_T("ProofPolicyOwn::InitConfig Begin /r/n"));
#ifdef DEBUG
	try
	{
#endif
	// 加载配置文件
	CreateObj("ProofPolicyOwn", "VarContainer");
	TObjRef<VarContainer> pVar = "ProofPolicyOwn";

	if( !P_VALID(pVar) ) return FALSE;

	pVar->Load("VirtualFileSys", _T("server_config/login/center_proof.ini"));

	// 读取IP和端口
	TCHAR szIP[X_IP_LEN];
	_tcsncpy(szIP, pVar->GetString(_T("ip server")), MCalTCharArrayNum(szIP) - 1);
	memcpy(m_szIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szIP) - 1);

	m_nPort = pVar->GetInt(_T("port server"));

	m_toCentralEveryTime=0;
	m_toCentralEveryTime = pVar->GetInt(_T("toCentralEveryTime option")); 
	// 删除配置文件对象
	KillObj("ProofPolicyOwn");
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test22.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif	
	ILOG->Write(_T("ProofPolicyOwn::InitConfig End /r/n"));
	return TRUE;
}

//-------------------------------------------------------------------------
// 销毁
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::Destroy()
{

	// 结束线程
	InterlockedExchange((LPLONG)&m_bTerminateUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("ProofPolicyOwnThreadUpdate"), INFINITE);

	InterlockedExchange((LPLONG)&m_bTerminateConnect, TRUE);
	m_pThread->WaitForThreadDestroy( _T("ProofPolicyOwnConnectServer"), INFINITE);

	// 销毁网络
	m_pTransport->Destroy();
	KillObj("StreamTransportProofPolicyOwn");

	// 销毁网络消息管理器
	UnregisterProofMsg();
	KillObj("NetCmdMgrProofPolicyOwn");

	// 删除所有list中的数据
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}
}

//-------------------------------------------------------------------------
// 注册网络消息
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::RegisterProofMsg()
{
	m_pCmdMgr->Register("NS_LOGIN",				(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleLogin),				_T("login return") );
	m_pCmdMgr->Register("NS_USERLOGIN",			(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleUserLogin),			_T("accout login return") );
	m_pCmdMgr->Register("NS_USERUPDATEPWD",		(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleUserUpdatePwd),		_T("accout update password") );
	m_pCmdMgr->Register("NS_USERBINDMIBAO",		(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleUserBindMibao),		_T("accout bind mibao") );
	m_pCmdMgr->Register("NS_USERUNBINDMIBAO",	(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleUserUnbindMibao),	_T("accout unbind mibao") );

	m_pCmdMgr->Register("NS_BLOCKACCOUNT",		(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleBlockAccount),	_T("block accout ") );
	m_pCmdMgr->Register("NS_UNBLOCKACCOUNT",	(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleUnBlockAccount),	_T("unblock accout ") );
	m_pCmdMgr->Register("NS_USERUPDATECHENMI",	(NETMSGPROC)m_Trunk.sfp2(&ProofPolicyOwn::HandleSetChenMi),			_T("set chenmi") );

}

//-------------------------------------------------------------------------
// 清空网络消息
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::UnregisterProofMsg()
{
	m_pCmdMgr->Destroy();
}

//-------------------------------------------------------------------------
// 更新线程
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::ThreadUpdate()
{
	DWORD dwTime = 0;

#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		while( !m_bTerminateUpdate )
		{
			// 得到当前时间
			dwTime = timeGetTime();

			//ILOG->Write(_T("ProofPolicyOwn::ThreadUpdate \r\n"));

			// 更新验证列表
			UpdateProofList();

			//ILOG->Write(_T("ProofPolicyOwn::ThreadUpdate1 \r\n"));

			// 更新消息
			UpdateSession();

			//ILOG->Write(_T("ProofPolicyOwn::ThreadUpdate2 \r\n"));

			// 等待
			WaitForSingleObject(m_listProofData.GetEvent(), 50);

			//ILOG->Write(_T("ProofPolicyOwn::ThreadUpdate3 \r\n"));
		}
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test9.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif

	_endthreadex(0);
}

//-------------------------------------------------------------------------
// 更新验证列表
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::UpdateProofList()
{
#ifdef DEBUG
	try
	{
#endif

	// 从列表中取出验证数据
	tagProofResult result;
	string strPsd;

	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		// 重置返回码
		BOOL bNeedCenterProof = m_toCentralEveryTime == 1; //是否每次都需要去中央验证
		INT  nRet = E_Success;

		// 如果需要中心激活但现在没有连接
		if( bNeedCenterProof && !m_bConnected )
		{
			nRet = E_SelectWorld_Server_Maintenance;
			bNeedCenterProof = FALSE;
#ifdef TS_LOG
			ILOG->Write(_T("-----------------------UpdateProofList-------------------\r\n"));  
			ILOG->Write(_T("m_bConnected = %d/r/n"),m_bConnected);
#endif
		}

#ifdef TEST_MIX_SERVER
		TCHAR accoutName[32];
		memset(accoutName,0,sizeof(accoutName));
		MultiByteToWideChar(CP_ACP,NULL,pData->strAccoutName.c_str(),-1,accoutName,X_SHORT_NAME);

		TCHAR accountName1[X_STRING_LEN];
		memset(accountName1,0,sizeof(accountName1));
		MultiByteToWideChar(CP_ACP,NULL,pData->strGUID.c_str(),-1,accountName1,X_STRING_LEN);


		TCHAR accountName2[X_STRING_LEN];
		memset(accountName2,0,sizeof(accountName2));
		MultiByteToWideChar(CP_ACP,NULL, pData->strPsd.c_str(),-1,accountName2,X_STRING_LEN);

		IMSG(_T("-----------------------------------Send To CenterProof -----------------------------------\r\n"));
		IMSG(_T("szGUID: %s,szPsd: %s,AccountName: %s,bNeedCenterProof: %d,ClientID: %d ,ChannelID: %d\r\n"), accountName1,accountName2,accoutName,(INT)bNeedCenterProof,pData->dwClientID,(INT)pData->dwChannelID);
		ILOG->Write(_T("-----------------------------------Send To CenterProof -----------------------------------\r\n"));
		ILOG->Write(_T("szGUID: %s,szPsd: %s,AccountName: %s,bNeedCenterProof: %d,ClientID: %d ,ChannelID: %d\r\n"), accountName1,accountName2,accoutName,(INT)bNeedCenterProof,pData->dwClientID,(INT)pData->dwChannelID);   
#endif
		if( bNeedCenterProof )
		{
			// 发往中心激活
			tagNC_USERLOGIN send;
			memset(&send.szAccountName,0,sizeof(send.szAccountName));
			memset(&send.szGUID,0,sizeof(send.szGUID));
			memset(&send.szPsd,0,sizeof(send.szPsd));
            // 10013 代表的是金山快快的ID
			// ID规则是这样的： 1(合作类型)+合作ID(2位)+游戏ID(2位)
			if (  ECT_YYYuYinPingTai == pData->dwChannelID )
			{

				//ILOG->Write(_T("ProofPolicyOwn::UpdateProofList YYStart/r/n"));
				send.dwLoginID	=	g_login.GetSectionID();
				send.dwClientID	=	pData->dwClientID;
				strncpy(send.szAccountName, pData->strGUID.c_str(), X_USERNAME_LENEX);
				send.szAccountName[X_USERNAME_LENEX-1] = '\0';
				send.szPsd[0] = '\0';
				send.szGUID[0]= '\0';
				send.dwChannelID =  pData->dwChannelID;

#ifdef  YYLogin_LOG	
				TCHAR accoutName[1024];
				memset(accoutName,0,sizeof(accoutName));
				MultiByteToWideChar(CP_ACP,NULL,send.szAccountName,-1,accoutName,X_USERNAME_LENEX);
				accoutName[X_USERNAME_LENEX] = _T('\0');

				TCHAR accountName1[X_STRING_LEN+2];
				memset(accountName1,0,sizeof(accountName1));
				MultiByteToWideChar(CP_ACP,NULL,send.szGUID,-1,accountName1,X_STRING_LEN);
				accountName1[X_STRING_LEN] = _T('\0');


				TCHAR accountName2[X_STRING_LEN+2];
				memset(accountName2,0,sizeof(accountName2));
				MultiByteToWideChar(CP_ACP,NULL, send.szPsd,-1,accountName2,X_STRING_LEN);
				accountName2[X_STRING_LEN] = _T('\0');

				//ILOG->Write(_T("ProofPolicyOwn::UpdateProofList YYEnd/r/n"));
				ILOG->Write(_T("-----------------------------------Send To CenterProof -----------------------------------\r\n"));
				ILOG->Write(_T("szGUID: %s,szPsd: %s,AccountName: %s,bNeedCenterProof: %d,ClientID: %d ,ChannelID: %d\r\n"), accountName1,accountName2,accoutName,(INT)bNeedCenterProof,pData->dwClientID,(INT)pData->dwChannelID);
#endif


			}
			else if ( ECT_JinShanKuaiKuai == pData->dwChannelID)
			{
				send.dwLoginID	=	g_login.GetSectionID();
				send.dwClientID	=	pData->dwClientID;
				strncpy(send.szGUID, pData->strGUID.c_str(), X_STRING_LEN);
				strncpy(send.szAccountName, pData->strGUID.c_str(), X_STRING_LEN);
				memset(&send.szPsd,0,sizeof(send.szPsd));
				send.szPsd[0] = '\0';
				send.dwChannelID =  pData->dwChannelID;

			}
			else
			{
				send.dwLoginID	=	g_login.GetSectionID();
				send.dwClientID	=	pData->dwClientID;
				strncpy(send.szGUID, pData->strGUID.c_str(), X_STRING_LEN);
				strncpy(send.szAccountName, pData->strAccoutName.c_str(), X_STRING_LEN);
				strncpy(send.szPsd, pData->strPsd.c_str(), X_STRING_LEN);
				send.dwChannelID =  pData->dwChannelID;

			}

			

			
			m_pTransport->Send(&send, send.dwSize);
		}
		else
		{
			// 调用回调函数
			(*m_fnCallback)(nRet, &result);
		}
		
		SAFE_DEL(pData);
		pData = m_listProofData.PopFront();		// 取出下一个
	}

#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test23.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
}

//-------------------------------------------------------------------------
// 更新消息
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::UpdateSession()
{
#ifdef DEBUG
	try
	{
#endif

	if( NULL == m_pTransport )
		return;

	// 检测断连
	if(!m_pTransport->IsConnected() && !m_pThread->IsThreadActive(_T("ProofPolicyOwnConnectServer")))
	{
		InterlockedExchange((LONG*)&m_bTerminateConnect, TRUE);
		InterlockedExchange((LONG*)&m_bConnected, FALSE);
		m_pTransport->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("ProofPolicyOwnConnectServer"), INFINITE);

		InterlockedExchange((LONG*)&m_bTerminateConnect, FALSE);

		//===============

		KillObj("StreamTransportProofPolicyOwn");
		CreateObj("StreamTransportProofPolicyOwn", "StreamTransport");
		m_pTransport = "StreamTransportProofPolicyOwn";
		if( !P_VALID(m_pTransport) ) return ;
		m_pTransport->Init();

		//==============



		// 重新启动数据库服务器连接线程
		m_pThread->CreateThread(_T("ProofPolicyOwnConnectServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOwn::ThreadConnectServer), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("ProofPolicyOwnConnectServer")))
		{
			//m_pThread->CreateThread(_T("ProofPolicyOwnConnectServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyOwn::ThreadConnectServer), NULL);
			continue;
		}

		return;
	}

	// 处理来自中心激活服务器的消息
	DWORD	dwSize = 0;
	LPBYTE	pRecv = m_pTransport->Recv(dwSize);

	while( P_VALID(pRecv) )
	{
		// 处理消息
		m_pCmdMgr->HandleCmd((tagNetCmd*)pRecv, dwSize, GT_INVALID);
		m_pTransport->FreeRecved(pRecv);

		// 收取下一条消息
		pRecv = m_pTransport->Recv(dwSize);
	}
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test24.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
}

//-------------------------------------------------------------------------
// 连接线程
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::ThreadConnectServer()
{
#ifdef DEBUG
	try
	{
#endif

#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while( FALSE == m_bTerminateConnect )
	{
		if( !m_pTransport->IsConnected() )
		{
			if( !m_pTransport->IsTryingToConnect() )
			{
				ILOG->Write(_T("ProofPolicyOwn::ThreadConnectServer IsTryingToConnect1 \r\n"));
				m_pTransport->TryToConnect(m_szIP, m_nPort);
				ILOG->Write(_T("ProofPolicyOwn::ThreadConnectServer IsTryingToConnect2 \r\n"));
			}

			Sleep(100);
			continue;	// 重新检测连接
		}

		IMSG(_T("Contected to center server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szIP), m_nPort);

		// 发送初始消息
		tagNC_LOGIN	send;
		send.dwLoginID = g_login.GetSectionID();
		m_pTransport->Send(&send, send.dwSize);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);

#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test25.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
}

//-------------------------------------------------------------------------
// 处理登陆
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleLogin(tagNetCmd* pCmd, DWORD)
{
#ifdef DEBUG
	try
	{
#endif

	ILOG->Write(_T("HandleLogin \r\n"));
	tagNS_LOGIN* pRecv = (tagNS_LOGIN*)pCmd;

	if( 0 != pRecv->byResult )
	{
		ASSERT(0);
	}

	InterlockedExchange((LONG*)&m_bConnected, TRUE);	// 设置连接

	ILOG->Write(_T("HandleLogin end \r\n"));
	return 0;

#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test26.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
}

//-------------------------------------------------------------------------
// 处理玩家登陆
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleUserLogin(tagNetCmd* pCmd, DWORD)
{
#ifdef DEBUG
	try
	{
#endif

	ILOG->Write(_T("HandleUserLogin \r\n"));

	tagNS_USERLOGIN* pRecv = (tagNS_USERLOGIN*)pCmd;

	tagProofResult result;
	result.dwClientID = pRecv->dwClientID;

#ifdef  YYLogin_LOG	
	ILOG->Write(_T("-----------------------------------Recv from CenterProof -----------------------------------\r\n"));
	TCHAR accoutName[32];
	memset(accoutName,0,sizeof(accoutName));
	MultiByteToWideChar(CP_ACP,NULL,pRecv->szAccountName,-1,accoutName,X_SHORT_NAME);
	ILOG->Write(_T("AccountName: %s, ProofResult: %d, ClientID: %d /r/n"),accoutName,(INT)pRecv->byResult,pRecv->dwClientID);
#endif
	// 查看返回码
	INT nRet = E_Success;
#ifdef TEST_MIX_SERVER
	TCHAR accoutName[32];
	memset(accoutName,0,sizeof(accoutName));
	MultiByteToWideChar(CP_ACP,NULL,pRecv->szAccountName,-1,accoutName,X_SHORT_NAME);
	ILOG->Write(_T("-----------------------RecvCenterProof-------------------\r\n"));  
	ILOG->Write(_T("AccountName: %s, ProofResult: %d, ClientID: %d /r/n"),accoutName,(INT)pRecv->byResult,pRecv->dwClientID);
#endif

	// 是否操作数据库
	if( 2 == pRecv->byResult )	// 帐号不存在
	{
		nRet = E_ProofResult_Account_No_Match;
	}
	else if( 1 == pRecv->byResult )	// 密码错误
	{
		nRet = E_ProofResult_Account_No_Match;
	
		// 只尝试更新数据库，而不插入，因为如果玩家是第一次登陆，此时中央激活并没有将玩家账号放入激活列表，可能会导致划拨时没有该区记录
		m_ProofDB.UpdateAccout(pRecv->szAccountName, pRecv->szPsd, pRecv->szMibao, pRecv->byGuard);
	}
	else if( 0 == pRecv->byResult )	// 正确
	{
		// 尝试插入，如果插入失败则尝试更新
		BOOL bRet = m_ProofDB.InsertAccout(pRecv->szAccountName, pRecv->szPsd, pRecv->szMibao, pRecv->byGuard);
		if( !bRet )
		{
			bRet = m_ProofDB.UpdateAccout(pRecv->szAccountName, pRecv->szPsd, pRecv->szMibao, pRecv->byGuard/*,pRecv->byBlocktype*/);
		}

	}
	else
	{
		nRet = E_ProofResult_Account_No_Match;
	}

	// 是否再需要重新查询
	if( E_ProofResult_Account_No_Match != nRet )
	{
		string strPsd;
		BOOL bRet = m_ProofDB.QueryAccount(pRecv->szAccountName, strPsd, &result);
		if( bRet )
		{
			if( EPLS_OffLine != result.eLoginStatus )
			{
				nRet = E_ProofResult_Account_In_Use;
			}
			else if( pRecv->byState == 1 )
			{
				//表示上来就被封停了~
				sBeton.ForbidAccount(result.dwAccountID,pRecv->byBlocktype);
				//重新得到result
				m_ProofDB.QueryAccount(pRecv->szAccountName, strPsd, &result);
			}
			//如果封停掩码不为零 且 当前登录状态并未改变成E_ProofResult_Account_In_Use
			if( (result.dwFrobidMask != 0)  && (nRet != E_ProofResult_Account_In_Use) )
			{
				do{
					if(result.dwFrobidMask & EPLM_MemberCenter)
					{
						nRet = E_ProofResult_Forbid_MemberCenter;						
						break;
					}
					if(result.dwFrobidMask & EPLM_GMTool)
					{
						//这种情况最后调用回调函数时需要world_forbid表里找一下
						nRet = E_ProofResult_Forbid_GMTool;						
						break;
					}
					if(result.dwFrobidMask & EPLM_CellPhone)
					{
						nRet = E_ProofResult_Forbid_CellPhone;						
						break;
					}
					if(result.dwFrobidMask & EPLM_MiBao)
					{
						nRet = E_ProofResult_Forbid_MiBao;						
						break;
					}
					if(result.dwFrobidMask & EPLM_WaiGua)
					{
						nRet = E_ProofResult_Disabled;						
						break;
					}
				}while(0);
			}
		}
		else
		{
			nRet = E_ProofResult_Account_No_Match;
		}
	}

	// 调用回调函数
	(*m_fnCallback)(nRet, &result);


	ILOG->Write(_T("HandleUserLogin end \r\n"));
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test27.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
	return 0;
}

//-------------------------------------------------------------------------
// 处理玩家更新密码
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleUserUpdatePwd(tagNetCmd* pCmd, DWORD)
{
#ifdef DEBUG
	try
	{
#endif

	ILOG->Write(_T("HandleUserUpdatePwd \r\n"));

	tagNS_USERUPDATEPWD* pRecv = (tagNS_USERUPDATEPWD*)pCmd;

	// 尝试更新数据库
	BOOL bRet = m_ProofDB.UpdatePsd(pRecv->szAccountName, pRecv->szPsd);

	// 发送返回消息
	tagNC_USERUPDATEPWD send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);

	m_pTransport->Send(&send, send.dwSize);

	ILOG->Write(_T("HandleUserUpdatePwd end \r\n"));
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test28.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
	return 0;
}

//-------------------------------------------------------------------------
// 处理玩家绑定密保
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleUserBindMibao(tagNetCmd* pCmd, DWORD)
{
#ifdef DEBUG
	try
	{
#endif

	ILOG->Write(_T("HandleUserBindMibao \r\n"));

	tagNS_USERBINDMIBAO* pRecv = (tagNS_USERBINDMIBAO*)pCmd;

	// 尝试更新数据库
	BOOL bRet = m_ProofDB.UpdateMibao(pRecv->szAccountName, pRecv->szMibao);

	// 发送返回消息
	tagNC_USERBINDMIBAO send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);

	m_pTransport->Send(&send, send.dwSize);

	ILOG->Write(_T("HandleUserBindMibao end \r\n"));
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test29.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
	return 0;
}

//-------------------------------------------------------------------------
// 玩家取消绑定密保
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleUserUnbindMibao(tagNetCmd* pCmd, DWORD)
{
#ifdef DEBUG
	try
	{
#endif

	ILOG->Write(_T("HandleUserUnbindMibao \r\n"));

	tagNS_USERUNBINDMIBAO* pRecv = (tagNS_USERUNBINDMIBAO*)pCmd;

	// 尝试更新数据库
	BOOL bRet = m_ProofDB.UpdateMibao(pRecv->szAccountName, pRecv->szMibao);

	// 发送返回消息
	tagNC_USERUNBINDMIBAO send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);

	m_pTransport->Send(&send, send.dwSize);

	ILOG->Write(_T("HandleUserUnbindMibao end \r\n"));
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test30.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
	return 0;
}

//-------------------------------------------------------------------------
// 处理账号封停
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleBlockAccount(tagNetCmd* pCmd, DWORD)
{
#ifdef DEBUG
	try
	{
#endif

	ILOG->Write(_T("HandleBlockAccount \r\n"));

	tagNS_BLOCKACCOUNT* pRecv = (tagNS_BLOCKACCOUNT*)pCmd;

	// 从列表中取出验证数据
	tagProofResult result;
	string strPsd;
	BOOL bRet = m_ProofDB.QueryAccount(pRecv->szAccountName, strPsd, &result);

	if(bRet)
	{
		DWORD dwRet = sBeton.ForbidAccount(result.dwAccountID,pRecv->byBlockType);
		bRet = ((dwRet==E_RT_Success) ? 1 : 0);
	}

	tagNC_BLOCKACCOUNT send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);
	m_pTransport->Send(&send, send.dwSize);

	ILOG->Write(_T("HandleBlockAccount end \r\n"));
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test31.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif

	return 0;
}
//-------------------------------------------------------------------------
// 处理账号解封
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleUnBlockAccount(tagNetCmd* pCmd, DWORD)
{
#ifdef DEBUG
	try
	{
#endif

	ILOG->Write(_T("HandleUnBlockAccount \r\n"));

	tagNS_UNBLOCKACCOUNT* pRecv = (tagNS_UNBLOCKACCOUNT *)pCmd;

	// 从列表中取出验证数据
	tagProofResult result;
	string strPsd;
	BOOL bRet = m_ProofDB.QueryAccount(pRecv->szAccountName, strPsd, &result);

	if(bRet)
	{
		DWORD dwRet = sBeton.RemoveAccountForbid(result.dwAccountID,pRecv->byUnBlockType);
		bRet = ((dwRet==E_RT_Success) ? 1 : 0);
	}

	tagNC_UNBLOCKACCOUNT send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);
	m_pTransport->Send(&send, send.dwSize);

	ILOG->Write(_T("HandleUnBlockAccount end \r\n"));

#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test32.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
	return 0;
}

//-------------------------------------------------------------------------
// 设置防沉迷
//-------------------------------------------------------------------------
DWORD ProofPolicyOwn::HandleSetChenMi(tagNetCmd* pCmd, DWORD)
{
#ifdef DEBUG
	try
	{
#endif

	ILOG->Write(_T("HandleSetChenMi \r\n"));

	tagNS_USERUPDATECHENMI* pRecv = (tagNS_USERUPDATECHENMI*)pCmd;

	// 从列表中取出验证数据
	tagProofResult result;
	string strPsd;
	BOOL bRet = m_ProofDB.QueryAccount(pRecv->szAccountName, strPsd, &result);

	if(bRet)
	{
		DWORD dwRet = sBeton.SetAccountChenMi(result.dwAccountID,pRecv->byChenMi);
		bRet = ((dwRet==E_RT_Success) ? 1 : 0);
	}

	tagNC_USERUPDATECHENMI send;
	send.dwLoginID = g_login.GetSectionID();
	send.byResult = (bRet ? 0 : 1);
	memcpy(send.szGUID, pRecv->szGUID, X_STRING_LEN);
	memcpy(send.szAccountName, pRecv->szAccountName, X_STRING_LEN);
	m_pTransport->Send(&send, send.dwSize);

	ILOG->Write(_T("HandleSetChenMi end \r\n"));
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test33.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
	return 0;
}


//-------------------------------------------------------------------------
// 验证接口
//-------------------------------------------------------------------------
VOID ProofPolicyOwn::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUILD, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*混服运营商ID*/,LPCSTR szMac,LPCSTR szUserID/* = NULL*/,LPCSTR szAppKey/*  = NULL*/, LPCSTR szToken/* = NULL*/)
{
#ifdef DEBUG
	try
	{
#endif

	ILOG->Write(_T("ProofPolicyOwn::Proof \r\n"));

	if( !P_VALID(dwClientID) || !P_VALID(szAccoutName) ) 
	{
		ILOG->Write(_T("ProofPolicyOwn::Proof1 \r\n"));
		return;
	}

	// 生成一个新的验证信息
	tagPlayerProofData* pData = new tagPlayerProofData(dwClientID, szAccoutName, szPsd, szGUILD,dwChannelID,szMac);

	m_listProofData.PushBack(pData);
	ILOG->Write(_T("ProofPolicyOwn::Proof2 \r\n"));

#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test34.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
}

//-------------------------------------------------------------------------
// 验证服务器连接状态
//-------------------------------------------------------------------------
INT ProofPolicyOwn::GetProofServerStatus()
{
#ifdef DEBUG
	try
	{
#endif

	if (P_VALID(m_pTransport) && m_pTransport->IsConnected())
	{
		return EWS_Well;
	}

	return EWS_ProofError;

#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test35.txt",ios::out|ios::app/*|ios::trunc*/);
		if (wFileStream)
		{
			wFileStream.close();
		}

		//如果是调试，则抛出异常
		if( TObjRef<Util>()->IsDebuggerPresent() )
		{
			throw;
		}
		else
		{
			exit(1);
		}
	}
#endif
}