#include "StdAfx.h"
#include "proof_policy_cornEx.h"
#include "login_server.h"
#include "loongworld.h"
#include "../ServerDefine/base_define.h"
#include "proof_policy_coreEx_define.h"
#include <fstream>
using namespace namepolicy;

//-------------------------------------------------------------------------
// 初始化
//-------------------------------------------------------------------------
BOOL ProofPolicyKoreaDB::Init()
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
BOOL ProofPolicyKoreaDB::QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pProofResult)
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
BOOL ProofPolicyKoreaDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
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
BOOL ProofPolicyKoreaDB::UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
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
BOOL ProofPolicyKorea::Init(PROOFCALLBACK fn)
{
	m_pUtil		=	"Util";
	m_pThread	=	"Thread";
	memset(m_szLoginIP,0,sizeof(m_szLoginIP));

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
	CreateObj("StreamTransPPKoreaLogin", "StreamTransport");
	m_pTransLogin = "StreamTransPPKoreaLogin";
	if( !P_VALID(m_pTransLogin) ) return FALSE;


	m_pTransLogin->Init(FALSE,ESTM_Stream); //
	// 启动连接线程
	if(!m_pThread->CreateThread(_T("PPKoreaConnectLoginServer"), 
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyKorea::ThreadConnectLoginServer), 
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("PPKoreaConnectLoginServer")))
	{
		continue;
	}

	// 启动更新线程
	if(!m_pThread->CreateThread(_T("PPKoreaLoginThreadUpdate"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyKorea::ThreadLoginServerUpdate),
		NULL))
	{
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("PPKoreaLoginThreadUpdate")))
	{
		continue;
	}

	RegisterEventFunc(EVT_PlayerLoginOut,		&ProofPolicyKorea::PlayerLoginOut);
	return TRUE;
}


//-------------------------------------------------------------------------
// 销毁
//-------------------------------------------------------------------------
VOID ProofPolicyKorea::Destroy()
{

	// 结束线程
	InterlockedExchange((LPLONG)&m_bTerminateLoginUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("PPKoreaLoginThreadUpdate"), INFINITE);

	InterlockedExchange((LPLONG)&m_bTerminateLoginConnect, TRUE);
	m_pThread->WaitForThreadDestroy( _T("PPKoreaConnectLoginServer"), INFINITE);

	// 销毁网络
	m_pTransLogin->Destroy();
	KillObj("StreamTransPPKoreaLogin");

	IMSG(_T("Destroy loginServer KoreaProof!!!\r\n"));



	// 删除所有list中的数据
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}

	std::list<tagPlayerProofData*> listData;
	m_mapProofSolvedData.ExportAllValue(listData);
	if ( listData.size() > 0)
	{
		pData = listData.front();
		listData.pop_front();
	}
	

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);
		pData = NULL;

		if ( listData.size() > 0)
		{
			pData = listData.front();
			listData.pop_front();
		}	
	}

	// 删除配置文件对象
	KillObj("ProofPolicyKorea");
}
//-------------------------------------------------------------------------
// 连接对方login服务器线程
//-------------------------------------------------------------------------
VOID ProofPolicyKorea::ThreadConnectLoginServer()
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
VOID ProofPolicyKorea::ThreadLoginServerUpdate()
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

			EventMgr<ProofPolicyKorea>::Update();

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
		wFileStream.open("log\\LoginError_Test4.txt",ios::out|ios::app/*|ios::trunc*/);
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
BOOL ProofPolicyKorea::InitConfig()
{
	// 加载配置文件
	CreateObj("ProofPolicyKorea", "VarContainer");
	TObjRef<VarContainer> pVar = "ProofPolicyKorea";

	if( !P_VALID(pVar) ) return FALSE;
	pVar->Load("VirtualFileSys", _T("server_config/login/korea_proof.ini"));
	//登录的
	TCHAR szIP[X_IP_LEN] = {'\0'};
	//_tcsncpy(szIP, pVar->GetString(_T("ip login_server")), MCalTCharArrayNum(szIP) - 1);
	wstring tstrIP = pVar->GetString(_T("ip login_server"));
	string  strIP  = m_pUtil->UnicodeToUnicode8(tstrIP.c_str());
	memcpy(m_szLoginIP, strIP.c_str(), strIP.length());
	m_szLoginIP[strIP.length()] = '\0';

	m_nLoginPort = pVar->GetInt(_T("port login_server"));

	return TRUE;
}

//-------------------------------------------------------------------------
// 更新验证列表
//-------------------------------------------------------------------------
VOID ProofPolicyKorea::UpdateProofList()
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
	if( P_VALID(pData) && iNum < 1)
	{

		++iNum;

		//////////////////////////////////////////////////////////////////////////

		//以下数据都走ansi

		TObjRef<StreamTransport> pTrans;
		pTrans = "StreamTransport";
         
		CHAR AccoutData[300] = {0};
		memcpy(AccoutData,pData->byAccountData,sizeof(pData->byAccountData));
		string strGUID = AccoutData;
		std::vector<string> vetStr;
		ParseKoreaProofData(strGUID,vetStr,X_KR_DIVIDE_SIGN);

		if (vetStr.size() < 6 )
		{
			ILOG->Write(_T("-----------------------------------Parse parameter string failed! -----------------------------------\r\n")); 
			TCHAR accoutName[256];
			memset(accoutName,0,sizeof(accoutName));
			MultiByteToWideChar(CP_ACP,NULL,strGUID.c_str(),-1,accoutName,strGUID.length());
			ILOG->Write(_T("parameter: %s\r\n"), accoutName);  
			SAFE_DEL(pData);
			return;

		}

		pData->strAccoutName.clear();
		pData->strAccoutName = vetStr[0];
		pData->strKRUseName  = vetStr[1];
	
		DWORD dwSize						 = sizeof(tagNHC_KoreaUserInfoProof) + vetStr[3].length();
		DWORD dwStrSize						 = vetStr[3].length() + 1 + sizeof(UINT)*2;
		DWORD dwUID							 = (DWORD)_atoi64(vetStr[0].c_str());
		DWORD dwGameIDX						 = (DWORD)atoi(vetStr[5].c_str());

		BYTE* p								 = new BYTE[dwSize];
		memset(p,0,dwSize);
		tagNHC_KoreaUserInfoProof* pUserInfo = (tagNHC_KoreaUserInfoProof*)p;
		pUserInfo->czChar1					 = 'D';
		pUserInfo->czChar2					 = 'F';
		pUserInfo->nPacketSize				 = dwStrSize;
		pUserInfo->nPacketProctolSize		 = 9000;
		pUserInfo->nUsn						 = (UINT)dwUID;
		memcpy(pUserInfo->czHashValue,vetStr[3].c_str(),vetStr[3].length());
		pUserInfo->czHashValue[vetStr[3].length()] = '\0';
		PBYTE pByte							 = (PBYTE)&pUserInfo->czHashValue[vetStr[3].length()+1];
		UINT* pGameIDX						 = (UINT*)pByte;
		*pGameIDX							 = (UINT)dwGameIDX;

		

		
			
#ifdef  KOREA_LOG
		TCHAR accoutName[256];
		memset(accoutName,0,sizeof(accoutName));
		MultiByteToWideChar(CP_ACP,NULL,vetStr[3].c_str(),-1,accoutName,vetStr[3].length());
		SPE_LOG->Write(_T("-----------------------------------Send To CenterProof -----------------------------------\r\n"));
		SPE_LOG->Write(_T("char1: %c,char2: %c,nPacketSize: %d,nPacketProctolSize: %d,nUsn: %d,czHashValue: %s,GameIDX: %d\r\n"),pUserInfo->czChar1,
			pUserInfo->czChar2,pUserInfo->nPacketSize,pUserInfo->nPacketProctolSize,pUserInfo->nUsn,accoutName,*pGameIDX);   

#endif

		
		BOOL bIsExist = m_mapProofSolvedData.IsExist(pData->dwClientID);
		
		if ( bIsExist )
		{
			SAFE_DEL(pData);
#ifdef  KOREA_LOG		
		SPE_LOG->Write(_T("-----------------------------------Insert ClientID Failed -----------------------------------\r\n"));		
#endif
		
		}
		else
		{
			if ( m_mapClientID.IsExist( pUserInfo->nUsn ))
			{
				DWORD dwClientID = m_mapClientID.Peek(pUserInfo->nUsn);
				tagPlayerProofData* pProofData = m_mapProofSolvedData.Peek(dwClientID);
				if ( P_VALID(pProofData) )
				{
					SAFE_DEL(pProofData);
					m_mapProofSolvedData.Erase(dwClientID);
				}
				m_mapClientID.Erase(pUserInfo->nUsn);


			}
			m_mapClientID.Add(pUserInfo->nUsn,pData->dwClientID);
			m_mapProofSolvedData.Add(pData->dwClientID,pData);
			//发送数据
			m_pTransLogin->Send((void *)p,dwSize);
			
		}
		
		
		SAFE_DEL_ARRAY(p);

		//pData = m_listProofData.PopFront();		// 取出下一个
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif


}

//-------------------------------------------------------------------------
// 更新Login消息
//-------------------------------------------------------------------------
VOID ProofPolicyKorea::UpdateLoginSession()
{
	if( NULL == m_pTransLogin )
		return;

	// 检测断连
	if(!m_pTransLogin->IsConnected() && !m_pThread->IsThreadActive(_T("PPKoreaConnectLoginServer")))
	{
		InterlockedExchange((LONG*)&m_bTerminateLoginConnect, TRUE);
		m_pTransLogin->Disconnect();

		m_pThread->WaitForThreadDestroy(_T("PPKoreaConnectLoginServer"), INFINITE);

		// 重新启动验证服务器连接线程
		InterlockedExchange((LONG*)&m_bTerminateLoginConnect, FALSE);
		m_pThread->CreateThread(_T("PPKoreaConnectLoginServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyKorea::ThreadConnectLoginServer), NULL);

		while(FALSE == m_pThread->IsThreadActive(_T("PPKoreaConnectLoginServer")))
		{
			m_pThread->CreateThread(_T("PPKoreaConnectLoginServer"), (vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyKorea::ThreadConnectLoginServer), NULL);
			continue;
		}

		return;
	}

	// 处理来自越南服务器的消息
	DWORD	dwSize = 0;
	LPBYTE	pRecv = m_pTransLogin->Recv(dwSize);

	while(  P_VALID(pRecv) && dwSize > 0)
	{	
		//处理越南验证返回信息
		HandleKoreaLogin(pRecv);

		//越南验证的返回信息
		m_pTransLogin->FreeRecved(pRecv);

		pRecv = m_pTransLogin->Recv(dwSize);
	}

}

//-------------------------------------------------------------------------
// 处理越南验证信息
//-------------------------------------------------------------------------
VOID ProofPolicyKorea::HandleKoreaLogin(LPBYTE pRecv)
{
	tagPlayerProofData* pData = NULL;

	string strAccountName;
	//不存密码
	string strPsd = "";// "***";
	// 查看返回码
	INT nRet = E_Success;	

	vector <string> vecKoreaData;
	tagProofResult result;

   
	//直接转
	tagNHS_KoreaUserInfoProof* pStrKoreaRecv = (tagNHS_KoreaUserInfoProof*)pRecv;
	if ( pStrKoreaRecv->nPacketSize < 0 || pStrKoreaRecv->nPacketSize  >= 200)
	{
		ILOG->Write(_T("-----------------------------------接收数据异常-----------------------------------\r\n"));
		return;
	}


#ifdef  KOREA_LOG
	SPE_LOG->Write(_T("-----------------------------------Recv To CenterProof -----------------------------------\r\n"));
	SPE_LOG->Write(_T("char1: %d,char2: %d,protocol: %d,DataSize: %d\r\n"), (INT)pStrKoreaRecv->czChar1,(int)pStrKoreaRecv->czChar2,
		(int)pStrKoreaRecv->nPacketProctolSize,(int)pStrKoreaRecv->nPacketSize);

#endif

	if ( pStrKoreaRecv->nPacketProctolSize != 9001 )
	{
		return ;
	}
#ifdef  KOREA_LOG
	SPE_LOG->Write(_T("-----------------------------------Recv To CenterProof -----------------------------------\r\n"));
	SPE_LOG->Write(_T("char1: %d,char2: %d,protocol: %d,DataSize: %d,nUsn: %d, nResultValL %d\r\n"), (INT)pStrKoreaRecv->czChar1,(int)pStrKoreaRecv->czChar2,
	(int)pStrKoreaRecv->nPacketProctolSize,(int)pStrKoreaRecv->nPacketSize,pStrKoreaRecv->nUsn,pStrKoreaRecv->nResultVal);
#endif

	//返回码数据项错误
	

	DWORD dwKeyID = (DWORD)pStrKoreaRecv->nUsn;
	DWORD dwClientid = m_mapClientID.Peek(dwKeyID);
	pData = m_mapProofSolvedData.Peek(dwClientid);

	//////////////////////////////////////////////////////////////////////////
	if( !P_VALID(pData) )
	{
		nRet = E_ProofResult_Account_No_Match;
		#ifdef  KOREA_LOG
		SPE_LOG->Write(_T("-----------------------------------pData is NULL -----------------------------------\r\n"));
		#endif
	}
	else if(1 != pStrKoreaRecv->nResultVal)
	{
		nRet = E_ProofResult_Account_No_Match;		
		SAFE_DEL(pData);
		m_mapProofSolvedData.Erase(dwClientid);
		m_mapClientID.Erase(dwKeyID);
#ifdef  KOREA_LOG
		SPE_LOG->Write(_T("-----------------------------------nResultVal !=  1 -----------------------------------\r\n"));
#endif
	}
	else 
	{
		result.dwClientID     = pData->dwClientID;
		result.dwWorldNameCrc = pData->dwWorldNameCrc;
		result.dwPreLoginIP   = pData->dwIP ;
		strAccountName		  = pData->strAccoutName;
		memcpy(result.szAccountName,strAccountName.c_str(),strAccountName.length());
		result.szAccountName[strAccountName.length()] = '\0';
		memcpy(result.szUserID,pData->strKRUseName.c_str(),pData->strKRUseName.length());
		result.szUserID[pData->strKRUseName.length()] = '\0';

		SAFE_DEL(pData);
		m_mapProofSolvedData.Erase(dwClientid);
		m_mapClientID.Erase(dwKeyID);
		do 
		{
			//防沉迷
			BOOL bGuard = FALSE;
			CHAR szMibao[MIBAO_LEN] = ""; //没有密保
			//插入数据库
			BOOL bRet = m_ProofDB.InsertAccout(strAccountName.c_str(),strPsd.c_str(), szMibao, bGuard);
			if( !bRet )
			{
#ifdef  KOREA_LOG
				SPE_LOG->Write(_T("-----------------------------------Insert Accout Failed-----------------------------------\r\n"));
#endif
				bRet = m_ProofDB.UpdateAccout(strAccountName.c_str(),strPsd.c_str(), szMibao, bGuard);
			}

		} while (0);

	}
	if( E_ProofResult_Account_No_Match != nRet && E_ProofResult_Forbid_MemberCenter != nRet )
	{

		BOOL bRet = m_ProofDB.QueryAccount(strAccountName.c_str(), strPsd.c_str(), &result);
		if ( GT_INVALID == result.dwSeondPsdCrc || 0 == result.dwSeondPsdCrc)
		{
			result.bIsFirstLogin = true;
		}
		else
		{
			result.bIsFirstLogin = false;
		}

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
#ifdef  KOREA_LOG
			SPE_LOG->Write(_T("-----------------------------------Query Account Failed-----------------------------------\r\n"));
#endif
		}
	}
	#ifdef  KOREA_LOG
	SPE_LOG->Write(_T("-----------------------------------Proof data-----------------------------------\r\n"));
	SPE_LOG->Write(_T("nRet: %d\r\n"), nRet);
	#endif
	// 调用回调函数
	(*m_fnCallback)(nRet, &result);
}
//-----------------------------------------------------------------------------------------------
// 去掉回车换行
//-----------------------------------------------------------------------------------------------
BOOL ProofPolicyKorea::CheckString(string& str)
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
VOID ProofPolicyKorea::ParseKoreaProofData(const string &strFrameString,vector <string> &vecOrangeData,char cDivideSign)
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
VOID ProofPolicyKorea::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUILD, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*混服运营商ID*/,LPCSTR szMac,LPCSTR szUserID/* = NULL*/,LPCSTR szAppKey/*  = NULL*/, LPCSTR szToken/* = NULL*/)
{
	if( !P_VALID(dwClientID) || !P_VALID(szGUILD) ) return;

	// 生成一个新的验证信息
	tagPlayerProofData* pData = new tagPlayerProofData(dwClientID, szAccoutName, szPsd, szGUILD,dwIP,dwWorldNameCrc,szMac);

	m_listProofData.PushBack(pData);

}

//-------------------------------------------------------------------------
// 验证服务器连接状态
//-------------------------------------------------------------------------
INT ProofPolicyKorea::GetProofServerStatus()
{
	if (P_VALID(m_pTransLogin) && m_pTransLogin->IsConnected())
	{
		return EWS_Well;
	}

	return EWS_ProofError;
}

//-------------------------------------------------------------------------
// 玩家登出
//-------------------------------------------------------------------------
VOID   ProofPolicyKorea::PlayerLoginOut(DWORD dwSenderID, LPVOID pEventMessage)
{
		tagPlayerProofData* pData = m_mapProofSolvedData.Peek(dwSenderID);
		if ( P_VALID(pData) )
		{
			SAFE_DEL(pData);
			m_mapProofSolvedData.Erase(dwSenderID);
		}		
}