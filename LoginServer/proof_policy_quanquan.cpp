#include "StdAfx.h"
#include "proof_policy_quanquan.h"

#include "../ServerDefine/base_define.h"
#include "../WorldDefine/login.h"

#include "proof_policy.h"
#include "login_server.h"
#include "sdoa4server.h"
#include "loongworld.h"
#include "loongworld_mgr.h"
#include <fstream>

#define MD5_CHECK	"dacn2010sdo"
//-------------------------------------------------------------------------
// 初始化
//-------------------------------------------------------------------------
BOOL ProofPolicyQuanQuanDB::Init()
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
BOOL ProofPolicyQuanQuanDB::QueryAccount(LPCSTR szAccountName, string& strPsd, tagProofResult* pProofResult)
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
BOOL ProofPolicyQuanQuanDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
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
BOOL ProofPolicyQuanQuanDB::UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
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
/	迅雷验证策略
/
*/

//-------------------------------------------------------------------------
// 初始化
//-------------------------------------------------------------------------
BOOL ProofPolicyQuanQuan::Init(PROOFCALLBACK fn)
{
	m_pUtil		=	"Util";
	m_pThread	=	"Thread";

	m_fnCallback = fn;	

	// 初始化数据库
	if( !m_ProofDB.Init() )
	{
		return FALSE;
	}

	TObjRef<VarContainer> pVar = "LoginServerConfig";
	if( !P_VALID(pVar) ) return FALSE; 

	WORD wVersionRequested;  
	WSADATA wsaData;  
	int err;  
	wVersionRequested=MAKEWORD(2,2);  
	err=WSAStartup(wVersionRequested,&wsaData);  
	if(err!=0){  
		return FALSE;  
	}  
	if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wHighVersion)!=2)  
	{  
		WSACleanup();  
		return FALSE ;  
	}  

	m_hSockClient=socket(AF_INET,SOCK_DGRAM,0);   

	m_addrSrv.sin_addr.S_un.S_addr=inet_addr(m_pUtil->UnicodeToAnsi(pVar->GetString(_T("billingip quanquan"))));  
	m_addrSrv.sin_family=AF_INET;  
	m_addrSrv.sin_port=htons((u_short)pVar->GetDword(_T("billingport quanquan")));   

	m_hShengDaDll = LoadLibrary(_T("SDOA4ServerDLL.dll"));
	if( m_hShengDaDll == NULL )//if the handle is NULL, it is failed.
	{
		ASSERT(0);
		return FALSE;
	}

	m_pFuncCreate = (pFuncCreateUserInfoAuthen)GetProcAddress(m_hShengDaDll,"sdoaCreateUserInfoAuthen");

	if(!P_VALID(m_pFuncCreate)) 
	{
		ASSERT(0);
		return FALSE;
	}

	m_pAuth = m_pFuncCreate();

	if(!P_VALID(m_pAuth))
	{
		ASSERT(0);
		return FALSE;
	}

	HRESULT hr = m_pAuth->Initialize(GetConfigFile().c_str(), (UserInfoCallback)m_Trunk.sfp3(&ProofPolicyQuanQuan::OnUserInfoCallback));
	
	if(FAILED(hr))
	{
		ASSERT(0);
		m_pAuth->Release();
		m_pAuth = NULL;
		return FALSE;
	}

	// 启动更新线程
	if(!m_pThread->CreateThread(_T("ProofPolicyQuanQuanThreadUpdate"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyQuanQuan::ThreadUpdate),
		NULL))
	{
		ASSERT(0);
		m_pAuth->Release();
		m_pAuth = NULL;
		return FALSE;
	}

	while(!m_pThread->IsThreadActive(_T("ProofPolicyQuanQuanThreadUpdate")))
	{
		continue;
	}

	// 启动金牌账号线程
	if(!m_pThreadGolden->CreateThread(_T("ProofPolicyQuanQuanThreadGolden"),
		(vEngine::THREADPROC)m_Trunk.sfp0(&ProofPolicyQuanQuan::ThreadUpdateGoldenList),
		NULL))
	{
		ASSERT(0);
		InterlockedExchange((LPLONG)&m_bTerminateUpdate, TRUE);
		m_pThread->WaitForThreadDestroy( _T("ProofPolicyQuanQuanThreadUpdate"), INFINITE);
		m_pAuth->Release();
		m_pAuth = NULL;
		return FALSE;
	}

	while(!m_pThreadGolden->IsThreadActive(_T("ProofPolicyQuanQuanThreadGolden")))
	{
		continue;
	}

	return TRUE;
}

//-------------------------------------------------------------------------
// 销毁
//-------------------------------------------------------------------------
VOID ProofPolicyQuanQuan::Destroy()
{
	// 结束线程
	InterlockedExchange((LPLONG)&m_bTerminateUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("ProofPolicyQuanQuanThreadUpdate"), INFINITE);
	m_pThreadGolden->WaitForThreadDestroy(_T("ProofPolicyQuanQuanThreadGolden"), INFINITE);
	// 删除所有list中的数据
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}

	if(P_VALID(m_pAuth))m_pAuth->Release();
	FreeLibrary(m_hShengDaDll);

	closesocket(m_hSockClient);
}

//-------------------------------------------------------------------------
// 更新线程
//-------------------------------------------------------------------------
VOID ProofPolicyQuanQuan::ThreadUpdate()
{
#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		while( !m_bTerminateUpdate )
		{
			// 更新验证列表
			UpdateProofList();

			// 更新消息
			UpdateSession();

			// 休息
			Sleep(50);
		}
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test10.txt",ios::out|ios::app/*|ios::trunc*/);
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
VOID ProofPolicyQuanQuan::UpdateProofList()
{	
#ifdef _DEBUG
	EXCEPTION_PROTECT_START;
#endif
	tagPlayerProofData* pData = m_listProofData.PopFront();

	int iNum = 0;

	//一次处理50个
	while( P_VALID(pData) && iNum<50)
	{

		++iNum;
		HRESULT hr = m_pAuth->AsyncGetUserInfo((char*)pData->byAccountData, IP2String(pData->dwIP), pData->dwClientID);
		if(FAILED(hr))
		{
			tagPlayerProofResult* pRes = new tagPlayerProofResult;
			pRes->dwClientID = pData->dwClientID;
			pRes->eResult = E_ConnectServer;
			m_listProofResult.PushBack(pRes);
		}
		m_mapClientID2World.Add(pData->dwClientID, pData->dwWordNameCrc);
		SAFE_DEL(pData);
		pData = m_listProofData.PopFront();		// 取出下一个
	}

#ifdef _DEBUG
	EXCEPTION_PROTECT_END;
#endif


}

//-------------------------------------------------------------------------
// 更新消息
//-------------------------------------------------------------------------
VOID ProofPolicyQuanQuan::UpdateSession()
{	
	// 处理来自盛大服务器的消息	
	tagPlayerProofResult* pData = m_listProofResult.PopFront();
	tagProofResult result;
	INT nRet = E_Success;	
	string strPsd = "***";
	
	while(P_VALID(pData))
	{
		result.dwClientID = pData->dwClientID;
		if(pData->eResult != E_Success) // 出错处理
		{
			switch(pData->eResult)
			{
			case E_ConnectServer:
			case E_SocketError:
				nRet = E_ProofResult_AuthServer_Unreachable;
				break;
			case E_EmptyField:
				nRet = E_ProofResult_EmptyField;
				break;
			case E_WrongFormat:
				nRet = E_WrongFormat;
				break;
			case E_AlreadyAuthrized:
			case E_AuthFailed:
			case E_OutOfTime:
			case E_SysError:
				nRet = E_ProofResult_Account_No_Match;
				break;
			}			
		}
		else // 认证通过
		{
			result.bGuard = pData->bGuard;
			strcpy(result.szAccountName, pData->strAccountName.c_str());
			CHAR szMibao[MIBAO_LEN] = ""; //没有密保
			//插入数据库
			BOOL bRet = m_ProofDB.InsertAccout(pData->strAccountName.c_str(),strPsd.c_str(), szMibao, pData->bGuard);
			if( !bRet )
			{
				bRet = m_ProofDB.UpdateAccout(pData->strAccountName.c_str(),strPsd.c_str(), szMibao, pData->bGuard);
			}

			bRet = m_ProofDB.QueryAccount(pData->strAccountName.c_str(), strPsd, &result);

			if( bRet )
			{
				if( EPLS_OffLine != result.eLoginStatus )
				{
					nRet = E_ProofResult_Account_In_Use;
				}
				else if( result.dwFrobidMask != 0 )
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

		if(nRet == E_Success)
		{
			result.byReserved[0] = pData->byNeedShengDaInfo;
		}

		// 调用回调函数
		(*m_fnCallback)(nRet, &result);
		SAFE_DEL(pData);
		pData = m_listProofResult.PopFront();
	}
}

int ProofPolicyQuanQuan::OnUserInfoCallback(int nResult, unsigned long nUniqueId, ISDOAMsg *pMsg)
{
	tagPlayerProofResult* pRes = new tagPlayerProofResult;
	pRes->dwClientID = nUniqueId;
	switch(nResult)
	{
	case 0://(表示成功)
		pRes->eResult = E_Success;
		break;
	case -10332001://(内容或签名为空)
		pRes->eResult = E_EmptyField;
		break;
	case -10332002://(该签名已经验证过)
		pRes->eResult = E_AlreadyAuthrized;
		break;
	case -10332003://(签名验证失败)
		pRes->eResult = E_AuthFailed;
		break;
	case -10332004://(内容格式有误)
		pRes->eResult = E_WrongFormat;
		break;
	case -10332005://(超时)
		pRes->eResult = E_OutOfTime;
		break;
	case -10332006://(系统错误)
		pRes->eResult = E_SysError;
		break;
	case -10332007://(socket error)
		pRes->eResult = E_SocketError;
		break;
	default:
		pRes->eResult = E_SysError;
		break;
	}
	if(pRes->eResult == E_Success && pMsg)
	{
		try{
			//if(atoi(pMsg->GetValue("appId")) != 642) // 应用代号
			if(0)
			{
				pRes->eResult = E_SysError;
			}
			else
			{
				pRes->strAccountName = pMsg->GetValue("sndaID");
				pRes->bGuard = strcmp(pMsg->GetValue("adult"), "Y") != 0;
				pRes->byNeedShengDaInfo = strcmp(pMsg->GetValue("applingAdult"), "Y") == 0? 0:1;
				if(strcmp(pMsg->GetValue("goldenAccount"), "Y") == 0)
				{
					if(m_mapClientID2World.IsExist(nUniqueId))
					{
						tagGoldenMember* pGoldenMember = new tagGoldenMember;
						pGoldenMember->dwWorldNameCrc = m_mapClientID2World.Peek(nUniqueId);
						pGoldenMember->strAccountName = pRes->strAccountName;
						m_listGoldenMemberList.PushBack(pGoldenMember);
					}
				}				
			}
		}catch(...)
		{
			ofstream wFileStream;
			wFileStream.open("log\\LoginError_Test11.txt",ios::out|ios::app/*|ios::trunc*/);
			if (wFileStream)
			{
				wFileStream.close();
			}

			pRes->eResult = E_SysError;
		}
	}
	m_mapClientID2World.Erase(nUniqueId);
	m_listProofResult.PushBack(pRes);
	return 0;
}

//-------------------------------------------------------------------------
// 验证接口
//-------------------------------------------------------------------------
VOID ProofPolicyQuanQuan::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUILD, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*混服运营商ID*/,LPCSTR szMac,LPCSTR szUserID/* = NULL*/,LPCSTR szAppKey/*  = NULL*/, LPCSTR szToken/* = NULL*/)
{
	if( !P_VALID(dwClientID) || !P_VALID(szAccoutName) ) return;

	//处理用户名
	// 生成一个新的验证信息
	tagPlayerProofData* pData = new tagPlayerProofData(dwClientID, szGUILD, dwIP, dwWorldNameCrc,szMac);

	m_listProofData.PushBack(pData);
}

//-------------------------------------------------------------------------
// 验证服务器连接状态
//-------------------------------------------------------------------------
INT ProofPolicyQuanQuan::GetProofServerStatus()
{
	if (P_VALID(m_pAuth))
	{
		return EWS_Well;
	}

	return EWS_ProofError;
}

string ProofPolicyQuanQuan::GetConfigFile()const
{
	CHAR szTemp[MAX_PATH];
	GetModuleFileNameA(NULL, szTemp, MAX_PATH);

	// 截去路径
	for(int i=0,j=strlen(szTemp); i<2 && j>0;j--)
	{
		if(szTemp[j] == '\\' || szTemp[j] == '//')
			i++;
		if(i==2) szTemp[j] = 0;
	}
	string res = szTemp;
	res += "/server_config/login/quanquan_proof.ini";
	return res;
}

void ProofPolicyQuanQuan::ThreadUpdateGoldenList()
{

#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		DWORD dwSectionID = g_login.GetSectionID();
		while( !m_bTerminateUpdate )
		{
			int iNum = 0;	// 每秒钟最多处理40个
			tagGoldenMember* pData = m_listGoldenMemberList.PopFront();
			while(iNum<20 && P_VALID(pData))
			{
				LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(pData->dwWorldNameCrc);
				if(!P_VALID(pWorld) || EWS_Well != pWorld->GetStatus())
				{
					delete pData;
					pData = m_listGoldenMemberList.PopFront();
					continue;
				}
				if(SendGloden2Server(pData->strAccountName, dwSectionID, pWorld->GetWorldID()))
				{
					//// erase all pending data for the same user
					//std::list<tagGoldenMember*> tempList;
					//m_listGoldenMemberList.Export(tempList);
					//for(std::list<tagGoldenMember*>::iterator itor=tempList.begin(); itor!= tempList.end(); itor++)
					//{
					//	if((*itor)->strAccountName == pData->strAccountName)
					//	{
					//		m_listGoldenMemberList.Erase(*itor);
					//		delete *itor;
					//	}
					//}
				}
				delete pData;
				pData = m_listGoldenMemberList.PopFront();		
				iNum++;
			}
			::Sleep(500);
		}
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test12.txt",ios::out|ios::app/*|ios::trunc*/);
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

bool ProofPolicyQuanQuan::SendGloden2Server(string account, DWORD sectionID, DWORD worldID)
{
	stringstream ts;				
	ts << account << sectionID  << worldID << MD5_CHECK;
	BYTE digest[16];
	m_pUtil->MD5ForString( (char*)ts.str().c_str(), digest );
	string strPswMD5;
	m_pUtil->MD5ToString( strPswMD5, digest );
	ts.str("");
	ts << account << ";" << sectionID << ";" << worldID << ";" << strPswMD5;
	string msg = ts.str().c_str();
	return SOCKET_ERROR != sendto(m_hSockClient, msg.c_str(),msg.length()+1,0,(SOCKADDR*)&m_addrSrv,sizeof(SOCKADDR));
}