
#include "stdafx.h"
#include "proof_policy.h"
#include "proof_policy_corn.h"
#include "player_mgr.h"
#include "login_server.h"
#include "kr_util.h"
#include  "rijndael.h"
#include <fstream>
extern Rijndael  g_KrUtil;
//-----------------------------------------------------------------------------
// DB初始化
//-----------------------------------------------------------------------------
BOOL ProofPocilyCornDB::Init()
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

BOOL   ProofPocilyCornDB::QueryAccount(const string& name,string& seckey)
{
	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	TObjRef<StreamTransport> pTrans;
	pTrans = "StreamTransport";

	pStream->SetSelect("zinaccount", "secrandkey");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(name.c_str()).FillString("'");

	QueryResult* pResult = m_DB.Query(pStream);

	// 释放流
	m_DB.ReturnStream(pStream);

	// 查询结果为空
	if( !P_VALID(pResult)|| pResult->GetRowCount() <= 0 )
	{
		m_DB.FreeQueryResult( pResult );
		return FALSE;
	}
	
	// 加密数据
	seckey   =  (*pResult)[0].GetString();
	
	// 归还结果集
	m_DB.FreeQueryResult(pResult);

	return TRUE;
}

//-----------------------------------------------------------------------------
// DB查询玩家
//-----------------------------------------------------------------------------
BOOL ProofPocilyCornDB::QueryAccount(LPCSTR szAccountName,LPCSTR szPsd, tagProofResult* pProofResult)
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

	pStream->SetSelect("account", "id,forbid_mask,privilege,guard,login_status,worldname_crc,mibao,SecondPsd,ip,time");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("'");
	//pStream->FillString(" AND psd='").FillString(szPsd, pCon).FillString("'"); 
	// 释放连接
	m_DB.ReturnConnection(pCon);

	QueryResult* pResult = m_DB.Query(pStream);

	// 释放流
	m_DB.ReturnStream(pStream);

	// 查询结果为空
	if( !P_VALID(pResult) || pResult->GetRowCount() <= 0)
	{
		m_DB.FreeQueryResult( pResult );
		return FALSE;
	}
	// 设置结果
	pProofResult->dwAccountID		=	(*pResult)[0].GetDword();
	pProofResult->dwFrobidMask		=	(*pResult)[1].GetDword();
	pProofResult->byPrivilege		=	(*pResult)[2].GetByte();
	pProofResult->bGuard			=	0;
	pProofResult->nGuardAccuTime	=	0;
	pProofResult->eLoginStatus		=	(EPlayerLoginStatus)(*pResult)[4].GetInt();
	pProofResult->dwWorldNameCrc	=	(*pResult)[5].GetDword();

    pProofResult->bNeedMibao		=	FALSE;

	pProofResult->dwSeondPsdCrc     =	IUTIL->Crc32((*pResult)[7].GetString());
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
BOOL ProofPocilyCornDB::InsertAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
{
	if( !P_VALID(szAccountName) || !P_VALID(szPsd) ) return FALSE;

	MyStream* pStream = m_DB.GetStream();
	if( !P_VALID(pStream) ) return FALSE;

	// 申请连接
	Connection* pCon = m_DB.GetFreeConnection();
	char czSecondPsd[10]= {0};

	pStream->SetInsert("account");
	pStream->FillString("name='").FillString(szAccountName, pCon).FillString("',");
	pStream->FillString("psd='").FillString(szPsd, pCon).FillString("',");
	pStream->FillString("mibao='").FillBlob(szMiBao, MIBAO_LEN, pCon).FillString("',");
	pStream->FillString("SecondPsd='").FillString(czSecondPsd, pCon).FillString("',");
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
BOOL ProofPocilyCornDB::UpdateAccout(LPCSTR szAccountName, LPCSTR szPsd, LPCSTR szMiBao, BOOL bGuard)
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

//-----------------------------------------------------------------------------
// 初始化
//-----------------------------------------------------------------------------
BOOL ProofPolicyCorn::Init(PROOFCALLBACK fn)
{
	m_pThread = "Thread";

	SetProofCallback(fn);		// 设置验证回调函数
    
	// 读取配置文件
	TObjRef<VarContainer> pVar = "LoginServerConfig";
	m_nTimeOut  = (INT)pVar->GetInt(_T("timeout"), _T("prooftime"));
	// 初始化数据库
	if( FALSE == m_ProofDB.Init() )
	{
		return FALSE;
	}

	// 初始化线程
	m_bTerminateUpdate = FALSE;

	if( FALSE == m_pThread->CreateThread( _T("thread_update_proof_corn"), 
		(THREADPROC)m_Trunk.sfp1(&ProofPolicyCorn::ThreadUpdate), NULL) )
		return FALSE;

	return TRUE;
}

//------------------------------------------------------------------------------
// 销毁
//------------------------------------------------------------------------------
VOID ProofPolicyCorn::Destroy()
{
	// 结束线程
	InterlockedExchange((LPLONG)&m_bTerminateUpdate, TRUE);
	m_pThread->WaitForThreadDestroy( _T("thread_update_proof_corn"), INFINITE);

	// 删除所有list中的数据
	tagPlayerProofData* pData = m_listProofData.PopFront();

	while( P_VALID(pData) )
	{
		SAFE_DEL(pData);

		pData = m_listProofData.PopFront();
	}
}

//-------------------------------------------------------------------------
//把加密数据转换到字符串
//-------------------------------------------------------------------------
BOOL   ProofPolicyCorn::DecryptDataToString(OUT tagDecryptData* pDecryptData,IN const CHAR* pSrc,const CHAR* pPsd)
{
	  CHAR* pStrData = new CHAR[MAXSTR_SIZE];
	  memset(pStrData,0,sizeof(CHAR)*MAXSTR_SIZE);
	  if( !P_VALID(pStrData) || !P_VALID(pSrc) || !P_VALID(pDecryptData)|| !P_VALID(pPsd) )
	  {
		  delete [] pStrData;
		  pStrData = NULL;
          return FALSE;
	  }
      INT nLength = atoi(pPsd);
	  if ( nLength <= 0 || nLength >= MAXSTR_SIZE)
	  {
		  return FALSE;
	  }
      CHAR cipherBuff[MAXSTR_SIZE];
	  memset(cipherBuff,0,sizeof(cipherBuff));
	  INT nStrSize = strlen(pSrc)/2;
	  if ( nStrSize > MAXSTR_SIZE)
	  {
		  return  FALSE ;
	  }
	  HexStr2CharStr(pSrc, (byte*)cipherBuff, nStrSize);
	  g_KrUtil.padDecrypt((PBYTE)cipherBuff,nLength,(PBYTE)pStrData);
	  *(pStrData+nLength+1) = SECKEY_FLAG;
      if( FALSE == ParseStringFromDecryptData(pDecryptData,pStrData))
	  {
		  delete [] pStrData;
		  pStrData = NULL;
		  return FALSE ;
	  }
	  else
	  {
		  delete [] pStrData;
		  pStrData = NULL;
		  return TRUE;
	  }
}

//-------------------------------------------------------------------------
//从加密数据解析出字符串
//-------------------------------------------------------------------------
BOOL    ProofPolicyCorn::ParseStringFromDecryptData(OUT tagDecryptData* pDecryptData,IN const CHAR* pSrc)
{
	DWORD dwStrSize = strlen(pSrc);
	if ( dwStrSize > MAXSTR_SIZE)
	{
		return FALSE;
	}
	INT i = 0;
	INT nStrCount = 0;
	INT nFirstPos = 0,nLastPos = 0;
	CHAR szDescStr[MAXSTR_SIZE];
	memset(szDescStr,0,sizeof(szDescStr));
	string str[6];
	memset(str,0,sizeof(str));
	//找到有效的字符串开头
	while( pSrc[i] == SECKEY_FLAG || pSrc[i] == ' ')
	{
		if(i >= MAXSTR_SIZE)
		{
			return FALSE;
		}
		i++;
	}
	nFirstPos = i;
	while ( i < MAXSTR_SIZE )//不管传进的字符串末尾是否为/0 都结束
	{
		if( pSrc[i] == SECKEY_FLAG)
		{
			if (pSrc[i+1] == SECKEY_FLAG)  //连续两个标志符 说明字符串有问题
			{
				return FALSE;
			}
			nLastPos = i;
			memcpy(szDescStr,(pSrc+nFirstPos),(nLastPos - nFirstPos));//去掉标志符 22
			str[nStrCount] = szDescStr ;
			nStrCount++;
			memset(szDescStr,0,sizeof(szDescStr));
			nFirstPos = i + 1 ;
			if( nStrCount >= SECKEY_STRCOUNT )
			{
				break;
			}

		}	
		i++;

	}
	if ( nStrCount < SECKEY_STRCOUNT)  //如果读出的字符串小于6个说明 字符串有问题 放过
	{
		return FALSE;
	}

	pDecryptData->NationCode      = str[0].c_str();
	pDecryptData->PortalSiteCode  = str[1].c_str();
	pDecryptData->UserID          = str[2].c_str();
	pDecryptData->SecurityCode    = str[3].c_str();
	pDecryptData->UserAge         = str[4].c_str();
	pDecryptData->GameStartTime   = str[5].c_str();

	return TRUE;


}

//------------------------------------------------------------------------------
// 更新线程
//------------------------------------------------------------------------------
VOID ProofPolicyCorn::ThreadUpdate()
{
	DWORD dwTime = 0;
	// 重置返回码
	INT nRet = E_Success;

#ifdef DEBUG
	_set_se_translator(vEngine::SEH_Translation); 

	try
	{
#endif
		while( !m_bTerminateUpdate )
		{
			// 得到当前时间
			dwTime = timeGetTime();
			// 从列表中取出验证数据
			tagProofResult result;
			// 解密数据
            tagDecryptData DecryptData;
            string strPsd = "";   
			tagPlayerProofData* pData = m_listProofData.PopFront();
           
			while( P_VALID(pData) )
			{
				result.dwClientID = pData->dwClientID;

				string seckey   = "";
                //--------------------------------------------------
				//把传过来的字符密码解密
				//--------------------------------------------------
				//BOOL bRet = DecryptDataToString(&DecryptData,pData->strGUID.c_str(),pData->strPsd.c_str());
				// 				if ( !bRet)
				// 				{
				// 					nRet =  E_ProofResult_Account_No_Match ;
				// 					(*m_fnCallback)(nRet, &result);
				// 					SAFE_DEL(pData);
				// 					pData = m_listProofData.PopFront();
				// 					continue;  
				// 				}
				
				// 				INT nTime[6];
				// 				memset(&nTime,0,sizeof(nTime));
				// 				__int64  dwTime = _atoi64(DecryptData.GameStartTime.c_str());
				// 				for (INT i=0; i < 6; i++)
				// 				{
				// 					nTime[i] = dwTime % 100 ;
				// 					dwTime /= 100;
				// 				}
				// 				tagDWORDTime SrcTime(nTime[0],nTime[1],nTime[2],nTime[3],nTime[4],nTime[5]);
				// 				tagDWORDTime CurretTime = GetCurrentDWORDTime();
				// 				DWORD dwDisTime = CalcTimeDiff(CurretTime,SrcTime);
				// 				if( dwDisTime > m_nTimeOut   )
				// 				{
				// 					nRet =  E_ProofResult_TimeOut ;
				// 					(*m_fnCallback)(nRet, &result);
				// 					SAFE_DEL(pData);
				// 					pData = m_listProofData.PopFront();
				// 					continue; 
				// 				}

					//--------------------------------------------------
					//查询数据库数据来验证
					//--------------------------------------------------
			   BOOL  bRet = true;//m_ProofDB.QueryAccount(DecryptData.UserID,seckey);


				if ( !bRet )
				{
					nRet =  E_ProofResult_Account_No_Match ;
					(*m_fnCallback)(nRet, &result);
					SAFE_DEL(pData);
                    pData = m_listProofData.PopFront();
                    continue;  
				}
				else
				{
				 	if ( seckey != DecryptData.SecurityCode)
				    {
					  nRet =  E_ProofResult_Account_No_Match ;
					  (*m_fnCallback)(nRet, &result);
					  SAFE_DEL(pData);
					  pData = m_listProofData.PopFront();
					  continue;

				    }
					else
				    {
					    nRet = E_Success;
				    }

				 }
				result.bGuard = FALSE;
				strcpy(result.szAccountName, "test17"/*DecryptData.UserID.c_str()*/);
				CHAR szMibao[MIBAO_LEN] = ""; //没有密保
				//插入数据库
				bRet = m_ProofDB.InsertAccout(result.szAccountName,strPsd.c_str(), szMibao, result.bGuard);
				if( !bRet )
				{
					bRet = m_ProofDB.UpdateAccout(result.szAccountName,strPsd.c_str(), szMibao, result.bGuard);
					
				}


				bRet = m_ProofDB.QueryAccount(result.szAccountName,pData->strPsd.c_str(), &result);
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

				// 调用回调函数
				(*m_fnCallback)(nRet, &result);

				SAFE_DEL(pData);

				pData = m_listProofData.PopFront();		// 取出下一个
			}

			WaitForSingleObject(m_listProofData.GetEvent(), 500);
		}
#ifdef DEBUG
	}
	catch(vEngine::Exception)
	{
		ofstream wFileStream;
		wFileStream.open("log\\LoginError_Test3.txt",ios::out|ios::app/*|ios::trunc*/);
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

//---------------------------------------------------------------------------------
// 验证接口
//---------------------------------------------------------------------------------
VOID	ProofPolicyCorn::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID/*混服运营商ID*/,LPCSTR szMac,LPCSTR szUserID/* = NULL*/,LPCSTR szAppKey/*  = NULL*/, LPCSTR szToken/* = NULL*/)
{
	if( !P_VALID(dwClientID) || !P_VALID(szGUID) ) return;

	// 生成一个新的验证信息
	tagPlayerProofData* pData = new tagPlayerProofData(dwClientID, szAccoutName,szPsd,szGUID,szMac);

	m_listProofData.PushBack(pData);
}

//-------------------------------------------------------------------------
// 验证服务器连接状态
//-------------------------------------------------------------------------
INT ProofPolicyCorn::GetProofServerStatus()
{
	return EWS_Well;
}