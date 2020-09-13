#include "StdAfx.h"
#include "proof_policy_vietnam.h"
#include "login_server.h"
#include "loongworld.h"
#include "../ServerDefine/base_define.h"
#include <fstream>

//-------------------------------------------------------------------------
// 初始化
//-------------------------------------------------------------------------
BOOL ProofPolicyVietnamDB::Init()
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
BOOL ProofPolicyVietnamDB::QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pProofResult)
{
	if( !P_VALID(szAccountName) || !P_VALID(pProofResult) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	TObjRef<StreamTransport> pTrans;
	pTrans = "StreamTransport";

	char szIP[X_IP_LEN] = "";
	char szDateTime[X_DATATIME_LEN + 1]= "";
	// 申请连接
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetSelect("account", "id,forbid_mask,privilege,guard,login_status,worldname_crc,psd,mibao,ip,time");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");

	// 释放连接
	m_DB.ReturnConnection(pCon);

	QueryResult* pResult = m_DB.Query(pStream);

	// 释放流
	m_DB.ReturnStream(pStream);

	// 查询结果为空
	if( !P_VALID(pResult)|| pResult->GetRowCount() <= 0 )
	{
		m_DB.FreeQueryResult( pResult );
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

	//用户名
	strncpy_s(pProofResult->szAccountName,szAccountName,X_SHORT_NAME);

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

	// 归还结果集
	m_DB.FreeQueryResult(pResult);

	return TRUE;
}

//-------------------------------------------------------------------------
// 插入玩家记录
//-------------------------------------------------------------------------
BOOL ProofPolicyVietnamDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
{
	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

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

	return bRet;
}

//----------------------------------------------------------------------------
// 更新玩家记录
//----------------------------------------------------------------------------
BOOL ProofPolicyVietnamDB::UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
{
	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 申请连接
	Connection* pCon = m_DB.GetFreeConnection();

	pStream->SetUpdate("account");
	pStream->FillString("psd='").FillString(szPsd, pCon).FillString("',");
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

	return bRet;
}


/*
/	越南验证策略
/
*/
//-------------------------------------------------------------------------
// 初始化
//-------------------------------------------------------------------------
BOOL ProofPolicyVietnam::Init(PROOFCALLBACK fn)
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

	// 初始化网络

	//登入
	CreateObj("StreamTransPPVietnamLogin", "StreamTransport");
	m_pTransLogin = "StreamTransPPVietnamLogin";
	if( !P_VALID(m_pTransLogin) ) return FALSE;


	m_pTransLogin->Init(TRUE,ESTM_Stream); //
	// 启动连接线程
	if(!m_pThread->CreateThread(_T("PPVietnamConnectLoginServer"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyVietnam::ThreadConnectLoginServer), 
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("PPVietnamConnectLoginServer")))
	{
		continue;
	}

	// 启动更新线程
	if(!m_pThread->CreateThread(_T("PPVietnamLoginThreadUpdate"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyVietnam::ThreadLoginServerUpdate),
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("PPVietnamLoginThreadUpdate")))
	{
		continue;
	}

	return TRUE;
}


//-------------------------------------------------------------------------
// 销毁
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::Destroy()
{

	// 结束线程
	InterlockedExchange((LPLONG)&m_bTerminateLoginUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("PPVietnamLoginThreadUpdate"), INFINITE);

	InterlockedExchange((LPLONG)&m_bTerminateLoginConnect, TRUE);
	m_pThread->WaitForThreadDestroy( _T("PPVietnamConnectLoginServer"), INFINITE);

	// 销毁网络
	m_pTransLogin->Destroy();
	KillObj("StreamTransPPVietnamLogin");

	

	// 删除所有list中的数据
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}

    pData = m_listProofSolvedData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofSolvedData.PopFront();
	}

	// 删除配置文件对象
	KillObj("ProofPolicyVietnam");
}
//-------------------------------------------------------------------------
// 连接对方login服务器线程
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::ThreadConnectLoginServer()
{
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif

	while( FALSE == m_bTerminateLoginConnect )
	{
		if( !m_pTransLogin->IsConnected() )
		{
			if( !m_pTransLogin->IsTryingToConnect() )
			{
				m_pTransLogin->TryToConnect(m_szLoginIP, m_nLoginPort);
			}

			Sleep(100);
			continue;	// 重新检测连接
		}

		IMSG(_T("Contected to center login server at %s: %d\r\n"), m_pUtil->Unicode8ToUnicode(m_szLoginIP), m_nLoginPort);

		break;
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif
	_endthreadex(0);
}

//-------------------------------------------------------------------------
// 更新login线程
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::ThreadLoginServerUpdate()
{
	DWORD dwTime = 0;

#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		while( !m_bTerminateLoginUpdate )
		{
			// 得到当前时间
			dwTime = timeGetTime();

			// 更新验证列表
			UpdateProofList();

			// 更新消息
			UpdateLoginSession();

			// 休息
			Sleep(50);
		}
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test14.txt",ios::out|ios::app/*|ios::trunc*/);
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
// 初始化配置
//-------------------------------------------------------------------------
BOOL ProofPolicyVietnam::InitConfig()
{
	// 加载配置文件
	CreateObj("ProofPolicyVietnam", "VarContainer");
	TObjRef<VarContainer> pVar = "ProofPolicyVietnam";

	if( !P_VALID(pVar) ) return FALSE;
	pVar->Load("VirtualFileSys", _T("server_config/login/vietnam_proof.ini"));
	//登录的
	TCHAR szIP[X_IP_LEN] = {'\0'};
	_tcsncpy(szIP, pVar->GetString(_T("ip login_server")), MCalTCharArrayNum(szIP) - 1);
	memcpy(m_szLoginIP, m_pUtil->UnicodeToUnicode8(szIP), sizeof(m_szLoginIP) - 1);

	m_nLoginPort = pVar->GetInt(_T("port login_server"));

	return TRUE;
}

//-------------------------------------------------------------------------
// 更新验证列表
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::UpdateProofList()
{
	//
	if(!m_pTransLogin->IsConnected())
		return;

#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif


	// 从列表中取出验证数据
	tagProofResult result;

	tagPlayerProofData* pData = m_listProofData.PopFront();
	int iNum = 0;

	//一次处理50个
	while( P_VALID(pData) && iNum<50)
	{

		++iNum;

		//////////////////////////////////////////////////////////////////////////
		//以下数据都走ansi

		TObjRef<StreamTransport> pTrans;
		pTrans = "StreamTransport";

		//拼验证数据
		stringstream strLoginFrame;
        strLoginFrame<<pData->strAccoutName.c_str();
        strLoginFrame<<X_OG_RECEIVE_DIVIDE_SIGN; 
		strLoginFrame<<pData->strPsd.c_str();
		strLoginFrame<<"\r\n";

		//发送数据
		m_pTransLogin->Send((void *)strLoginFrame.str().c_str(),strLoginFrame.str().length());


		m_listProofSolvedData.PushBack(pData);
		pData = m_listProofData.PopFront();		// 取出下一个
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif


}

//-------------------------------------------------------------------------
// 更新Login消息
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::UpdateLoginSession()
{
	if( NULL == m_pTransLogin )
		return;

	// 检测断连
	if(!m_pTransLogin->IsConnected() && !m_pThread->IsThreadActive(_T("PPVietnamConnectLoginServer")))
	{
		InterlockedExchange((LONG*)&m_bTerminateLoginConnect, TRUE);
		m_pTransLogin->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("PPVietnamConnectLoginServer"), INFINITE);

		// 重新启动验证服务器连接线程
		InterlockedExchange((LONG*)&m_bTerminateLoginConnect, FALSE);
		m_pThread->CreateThread(_T("PPVietnamConnectLoginServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyVietnam::ThreadConnectLoginServer), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("PPVietnamConnectLoginServer")))
		{
			m_pThread->CreateThread(_T("PPVietnamConnectLoginServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyVietnam::ThreadConnectLoginServer), NULL);
			continue;
		}

		return;
	}

	// 处理来自越南服务器的消息
	DWORD	dwSize = 0;
	LPBYTE	pRecv = m_pTransLogin->Recv(dwSize);

	while( P_VALID(pRecv) )
	{	
		//处理越南验证返回信息
		HandleVietnamLogin(pRecv);

		//越南验证的返回信息
		m_pTransLogin->FreeRecved(pRecv);

		pRecv = m_pTransLogin->Recv(dwSize);
	}

}

//-------------------------------------------------------------------------
// 处理越南验证信息
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::HandleVietnamLogin(LPBYTE pRecv)
{
	


	tagPlayerProofData* pData = m_listProofSolvedData.PopFront();
	
	tagProofResult result;
	if ( P_VALID( pData ) )
	{
		result.dwClientID     = pData->dwClientID;
		result.dwWorldNameCrc = pData->dwWorldNameCrc;
		result.dwPreLoginIP   = pData->dwIP ;
		SAFE_DEL(pData);
	}
	string strAccountName;
	//不存密码
	string strPsd = "***";
	// 查看返回码
	INT nRet = E_Success;	

	vector <string> vecVietnamData;
	//直接转
	string strVietnamRecv = (CHAR*)pRecv;
	//解析数据
	ParseVietnamProofData(strVietnamRecv,vecVietnamData,X_OG_RECEIVE_DIVIDE_SIGN);
	//返回码数据项错误
	/*
if (vecVietnamData.size() < 4)
	{
		return;
	}
*/

	//////////////////////////////////////////////////////////////////////////
	if(0 != memcmp(vecVietnamData[1].c_str(),X_OG_PROOF_SIGN,vecVietnamData[1].size()))
	{
	     nRet = E_ProofResult_Account_No_Match;		
	}
	else 
	{
		do 
		{
			strAccountName = vecVietnamData[0];

			//防沉迷
			BOOL bGuard = FALSE;
			CHAR szMibao[MIBAO_LEN] = ""; //没有密保
			//插入数据库
			BOOL bRet = m_ProofDB.InsertAccout(strAccountName.c_str(),strPsd.c_str(), szMibao, bGuard);
			if( !bRet )
			{
				bRet = m_ProofDB.UpdateAccout(strAccountName.c_str(),strPsd.c_str(), szMibao, bGuard);
			}

		} while (0);
	}
	if( E_ProofResult_Account_No_Match != nRet && E_ProofResult_Forbid_MemberCenter != nRet )
	{

		BOOL bRet = m_ProofDB.QueryAccount(strAccountName.c_str(), strPsd, &result);

		if( bRet )
		{
			if( EPLS_OffLine != result.eLoginStatus )
			{
				nRet = E_ProofResult_Account_In_Use;
			}
			else if( result.dwFrobidMask != 0 )
			{
				do{
					if(result.dwFrobidMask & EPLM_GMTool)
					{
						//这种情况最后调用回调函数时需要world_forbid表里找一下
						nRet = E_ProofResult_Forbid_GMTool;						
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
}
//-----------------------------------------------------------------------------------------------
// 去掉回车换行
//-----------------------------------------------------------------------------------------------
BOOL ProofPolicyVietnam::CheckString(string& str)
{
	// 以回车换行结束
	INT nFind = 0;
	while( (nFind = str.find('\r', nFind)) != str.npos )
		str = str.substr(0,nFind);

	// 
	nFind = 0;
	while( (nFind = str.find('\n', nFind)) != str.npos )
		str = str.substr(0,nFind);

	// 检查用户名是否为空
	return str.empty();
}
//-----------------------------------------------------------------------------------------------
// 得到各个数据项 单引号分隔
//-----------------------------------------------------------------------------------------------
VOID ProofPolicyVietnam::ParseVietnamProofData(const string &strFrameString,vector <string> &vecOrangeData,char cDivideSign)
{
	int iprepos = 0;
	int icurpos = 0;
	for(;;)
	{
		string strTemp;

		icurpos = strFrameString.find(cDivideSign,iprepos);
		//最后一个
		if(icurpos == -1)
		{
			strTemp = strFrameString.substr(iprepos,strFrameString.length() - iprepos);

			//检查最后一个字符串
			CheckString(strTemp);

			vecOrangeData.push_back(strTemp);
			break;
		}
		strTemp = strFrameString.substr(iprepos,icurpos-iprepos);

		vecOrangeData.push_back(strTemp);
		iprepos = icurpos + 1;
	}
}

//-------------------------------------------------------------------------
// 验证接口
//-------------------------------------------------------------------------
VOID ProofPolicyVietnam::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUILD, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*混服运营商ID*/,LPCSTR szMac,LPCSTR szUserID/* = NULL*/,LPCSTR szAppKey/*  = NULL*/, LPCSTR szToken/* = NULL*/)
{
	if( !P_VALID(dwClientID) || !P_VALID(szAccoutName) ) return;

	// 生成一个新的验证信息
	tagPlayerProofData* pData = new tagPlayerProofData(dwClientID, szAccoutName, szPsd, szGUILD,dwIP,dwWorldNameCrc,szMac);

	m_listProofData.PushBack(pData);

}

//-------------------------------------------------------------------------
// 验证服务器连接状态
//-------------------------------------------------------------------------
INT ProofPolicyVietnam::GetProofServerStatus()
{
	if (P_VALID(m_pTransLogin) && m_pTransLogin->IsConnected())
	{
		return EWS_Well;
	}

	return EWS_ProofError;
}
