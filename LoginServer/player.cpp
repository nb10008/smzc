//-----------------------------------------------------------------------------
//!\file player.h
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief 玩家类
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"



#include "player.h"
#include "player_mgr.h"
#include "player_net_cmd_mgr.h"
#include "loongworld.h"
#include "loongworld_mgr.h"
#include "db.h"
#include "login_server.h"
#include "proof_policy_corn_define.h"
#include "proof_policy.h"
#include "proof_policy_corn.h"
#include "yauthLib.h"
#include "../WorldDefine/login.h"
#include "fstream"
          
//-------------------------------------------------------------------------------
// 静态变量声明
//-------------------------------------------------------------------------------
PlayerNetCmdMgr Player::m_playerNetMgr;

//-------------------------------------------------------------------------------
// 构造函数
//-------------------------------------------------------------------------------
Player::Player(DWORD dwClientID, DWORD dwCDIndex, DWORD dwIP, DWORD dwWorldID, CHAR *pszMac) : 
				m_Data(), m_bConnLost(FALSE), m_nVerifyCode(0), m_bProofEnd(FALSE),
				m_bProofing(FALSE), m_bNeedMibao(FALSE), m_dwKickTime(), m_bSelectWorldOk(FALSE), m_dwReserved(0),m_bIsGM(false)
{
	m_Data.dwClientID		=		dwClientID;
	m_Data.dwCDIndex		=		dwCDIndex;
	m_Data.dwAccountID		=		GT_INVALID;
	m_Data.dwFrobidMask		=		0;
	m_Data.bGuard			=		FALSE;
	m_Data.nAccOLSec		=		0;
	m_Data.dwIP				=		dwIP;
	m_Data.dwWorldNameCrc	=		dwWorldID;
	m_Data.byPrivilege		=		0;
	m_Data.dwChannelID      =       0;
	if(P_VALID(pszMac))
	{
		pszMac[MAX_MAC_LENGTH-1] = '\0';
		strncpy_s(m_Data.szMac, pszMac, MAX_MAC_LENGTH);	// MAC
	}
}

//-------------------------------------------------------------------------------
// 析构函数
//-------------------------------------------------------------------------------
Player::~Player()
{
   
}

//-------------------------------------------------------------------------------
// 验证返回
//-------------------------------------------------------------------------------
VOID Player::ProofReturn(tagProofResult* pResult)
{
	m_Data.dwAccountID		=	pResult->dwAccountID;
	m_Data.bGuard			=	pResult->bGuard;
	m_Data.nAccOLSec		=	pResult->nGuardAccuTime;
	m_Data.byPrivilege		=	pResult->byPrivilege;
	m_Data.dwFrobidMask		=	pResult->dwFrobidMask;
	m_Data.dwSecondPsdCrc   =   pResult->dwSeondPsdCrc;

	//如果是橘子验证 需要把真正的用户名给附上
	if(sPlayerMgr.GetProofPolicy() == EPP_Orange)
	{	
		memcpy(m_Data.szAccountName,pResult->szAccountName,X_SHORT_NAME); 
	}
    //非官方的国服
	if (  (sPlayerMgr.GetProofPolicy() == EPP_Own) && (GetChannelID() != 0) )
	{
        memcpy(m_Data.szAccountName,pResult->szAccountName,X_SHORT_NAME); 
	}

	if ( EPP_Korea == sPlayerMgr.GetProofPolicy() )
	{
		memcpy(m_Data.szAccountName,pResult->szAccountName,X_SHORT_NAME); 
		memcpy(m_Data.szKRUseName,pResult->szUserID,X_SHORT_NAME);
	}




	m_bNeedMibao			=	pResult->bNeedMibao;
	m_bSaveTelNum			=	pResult->bSaveTelNum;

	SetProofEnd();

	// 缓冲一下账号名
	sPlayerMgr.CacheAccountName(GetAccountID(), GetAccoutName());
	sPlayerMgr.CacheGuard(GetAccountID(), GetPlayerData().bGuard);
	sPlayerMgr.MapAccountName2AccountID(GetAccoutName(), GetAccountID());
}

//-------------------------------------------------------------------------------
// 注册消息处理函数
//-------------------------------------------------------------------------------
VOID Player::RegisterPlayerMsg()
{
	m_playerNetMgr.Register("NLC_SafeProof",	&HandleProof,			_T("NLC_SafeProof"));
	m_playerNetMgr.Register("NLC_MiBao",		&HandleMibao,			_T("NLC_MiBao"));
	m_playerNetMgr.Register("NLC_SetSecondPsd",	&HandleSetSecondPsd,			_T("NLC_SetSecondPsd"));
	m_playerNetMgr.Register("NLC_InputSecondPsd",&HandleInputSecondPsd,			_T("NLC_InputSecondPsd"));

}

//-------------------------------------------------------------------------------
// 注销消息处理函数
//-------------------------------------------------------------------------------
VOID Player::UnRegisterPlayerMsg()
{
	 m_playerNetMgr.UnRegisterAll();
}

//-------------------------------------------------------------------------------
// 处理玩家网络消息函数
//-------------------------------------------------------------------------------
INT Player::HandleMessage()
{
    // 检测玩家是否断开连接
    if( m_bConnLost )
        return CON_LOST;

    // 接收消息
	DWORD	dwSize		=	0;
	INT		nUnRecved	=	0;

	LPBYTE pMsg = RecvMsg(dwSize, nUnRecved);
	if( !P_VALID(pMsg) ) return 0;

	// 处理消息
    tagNetCmd* pCmd = (tagNetCmd*)pMsg;

    NETMSGHANDLER pHandler = m_playerNetMgr.GetHandler(pCmd, dwSize);

    if( NULL != pHandler )
        (this->*pHandler)(pCmd);
    
	// 回收消息
    ReturnMsg(pMsg);

	return 0;
}

//-------------------------------------------------------------------------------
// 发送消息
//-------------------------------------------------------------------------------
VOID Player::SendMessage(LPVOID pMsg, DWORD dwSize)
{
	if( !P_VALID(pMsg) || dwSize == 0 )
		return;

	// 连接已经中断，不再发送了
	if( m_bConnLost ) return;

	SendMsg((LPBYTE)pMsg, dwSize);
}

//-------------------------------------------------------------------------------
// 接收消息
//-------------------------------------------------------------------------------
LPBYTE Player::RecvMsg(DWORD& dwSize, INT& nUnRecved)
{
	return sPlayerMgr.RecvMsg(dwSize, nUnRecved, m_Data.dwCDIndex);
}

//-------------------------------------------------------------------------------
// 归还消息
//-------------------------------------------------------------------------------
VOID Player::ReturnMsg(LPBYTE pMsg)
{
	sPlayerMgr.ReturnMsg(pMsg);
}

//-------------------------------------------------------------------------------
// 发送消息
//-------------------------------------------------------------------------------
VOID Player::SendMsg(LPBYTE pMsg, DWORD dwSize)
{
	sPlayerMgr.SendMsg(m_Data.dwCDIndex, pMsg, dwSize);
}

VOID  Player::SendTokenID(DWORD dwCDIndex)
{
	DWORD dwConnID = (DWORD)dwCDIndex;

	string strTokenID ;
	sPlayerMgr.RequestTokenID(dwConnID,strTokenID);

	if ( strTokenID.empty())
	{
		ILOG->Write(_T("-----------------------TokenID is empty!-------------------\r\n"));  
		IMSG(_T("-----------------------TokenID is empty!-------------------\r\n"));
		return;
	}
	DWORD dwSize = sizeof(tagNLS_ProofResult) + strTokenID.length()*sizeof(BYTE);
	BYTE* pByte = new BYTE[dwSize];
	memset(pByte,0,dwSize);
	tagNLS_ProofResult* pSend = (tagNLS_ProofResult*)pByte;
	pSend->dwID        = pSend->Crc32("NLS_ProofResult");
	pSend->dwSize      = dwSize;
	pSend->dwErrorCode = E_YYYuYinPingTai_GetTokenID;
	pSend->dwReserved  = strTokenID.length();
	pSend->dwConnID    = dwConnID;
	memcpy(pSend->byData,strTokenID.c_str(),strTokenID.length()*sizeof(BYTE));

	SendMessage(pSend,dwSize);
	SAFE_DEL_ARRAY(pByte);

#ifdef  YYLogin_LOG	
	ILOG->Write(_T("登入2 dwConnID: %d,byData: %s \r\n"),(INT)dwConnID,strTokenID.c_str());
#endif
}



//-----------------------------------------------------------------------------
// 玩家认证消息
//-----------------------------------------------------------------------------
DWORD Player::HandleProof(tagNetCmd* pCmd)
{
	ILOG->Write(_T("HandleProof \r\n"));

    tagNLC_SafeProof* pRecv = (tagNLC_SafeProof*)pCmd;

	string strAccount = pRecv->szUserName;
	ofstream wFileStream;
	wFileStream.open("E:\\LoginAccountID.txt",ios::out|ios::app/*|ios::trunc*/);
	if (wFileStream)
	{
		struct tm *_ptm;
		time_t _time;
		time(&_time);
		_ptm = gmtime(&_time);
		if(P_VALID(_ptm))
		{
			int l = 0;
			int day = _ptm->tm_mday, hour = _ptm->tm_hour+8;
			if( hour >= 24 )
			{
				hour -= 24;
				day += 1;
			}
			//_stprintf(pStr,_T("[%d-%d-%d %d:%d:%d] "),_ptm->tm_year+1900,_ptm->tm_mon+1,day,hour,_ptm->tm_min,_ptm->tm_sec);
			wFileStream << "[时间:"<<_ptm->tm_year+1900 << "-" << _ptm->tm_mon+1 << "-" << day<< "-"  << hour<< "-"  << _ptm->tm_min<< "-"  << _ptm->tm_sec<< "-"  << "]账号：";
		}

		wFileStream << strAccount << endl;
		wFileStream.close();
	}

	//TCHAR tszUserName[X_SHORT_NAME] = {0};
	//MultiByteToWideChar(CP_ACP,NULL,pRecv->szUserName,-1,tszUserName,X_SHORT_NAME);
	//ILOG->Write(_T("------Proof begin-----,userid:%s.\r\n"),tszUserName);  

	// 防止字符串溢出
	pRecv->szUserName[X_SHORT_NAME-1] = '\0';
	pRecv->szPsd[MAX_MD5_ARRAY-1] = '\0';
	pRecv->szGUID[X_LONG_NAME-1] = '\0';
	*((char*)pCmd+pCmd->dwSize-1) = '\0'; // Jay 2010-4-7, set last byte of szGUID to 0 and make it valiable for various message length.

#ifdef  KOREA_LOG
	TCHAR accoutName[300];
	memset(accoutName,0,sizeof(accoutName));
	MultiByteToWideChar(CP_ACP,NULL,pRecv->szGUID,-1,accoutName,sizeof(pRecv->szGUID));
	SPE_LOG->Write(_T("-----------------------------------Handle client proof -----------------------------------\r\n"));
	SPE_LOG->Write(_T("parameter: %s\r\n"), accoutName);   
#endif


#ifdef  YYLogin_LOG	
	TCHAR accoutName1[300];
	memset(accoutName1,0,sizeof(accoutName1));
	MultiByteToWideChar(CP_ACP,NULL,pRecv->szUserName,-1,accoutName1,sizeof(pRecv->szUserName));
	ILOG->Write(_T("登入1 bIsGetTokenID: %d,dwChannelID: %d ,UserName: %s\r\n"),(INT)pRecv->bIsGetTokenID,(INT)pRecv->dwChannelID,accoutName1);
#endif
	// 进行初始认证
	INT nRet = E_Success;

	//ILOG->Write(_T("HandleProof sign1\r\n"));
	//上海Web登入客户端获取下载链接地址
	if ( pRecv->bIsGetURL && ELM_Web_LoginMode == pRecv->dwLoginMode)
	{
		if ( sPlayerMgr.GetWebLoginOn())
		{
			SendWebLoginInfoToClient();
		}
		else
		{

		}
		ILOG->Write(_T("HandleProof end1 \r\n"));
		return GT_INVALID;
	}
	//ILOG->Write(_T("HandleProof sign2\r\n"));

	if ( !pRecv->bIsGetURL && ELM_Web_LoginMode == pRecv->dwLoginMode)
	{
		if ( sPlayerMgr.GetWebLoginOn())
		{
			memset(&pRecv->szGUID,0,sizeof(pRecv->szGUID));
			SendWebProof(pRecv->szGUID,m_Data.dwClientID,pRecv->dwChannelID);
			pRecv->szGUID[X_LONG_NAME-1] = '\0';
		}
		else
		{
			ILOG->Write(_T("HandleProof end2 \r\n"));
			return GT_INVALID;
		}
	}

	//ILOG->Write(_T("HandleProof sign3\r\n"));
	//YY客户端请求验证令牌
	if ( pRecv->bIsGetTokenID && (ECT_YYYuYinPingTai == pRecv->dwChannelID))
	{
		if ( sPlayerMgr.GetYYMinSerOn())
		{
			SendTokenID(m_Data.dwClientID);
		}
		else
		{
			ILOG->Write(_T("-----------------------Warning!!! YY Mixed Server  closed,lllegal Client is logining-------------------\r\n"));  
			IMSG(_T("-----------------------Warning!!! YY Mixed Server  closed,lllegal Client is logining-------------------\r\n"));
		}
#ifdef TEST_MIX_SERVER
		ILOG->Write(_T("-----------------------------------Send TokenID -----------------------------------\r\n"));
		ILOG->Write(_T("ConnID: %d\r\n"), m_Data.dwClientID);

#endif

		ILOG->Write(_T("HandleProof end3 \r\n"));
		return  GT_INVALID;
	}

	//ILOG->Write(_T("HandleProof sign4\r\n"));
	if ( (!pRecv->bIsGetTokenID) && ECT_YYYuYinPingTai == pRecv->dwChannelID )
	{
		if ( sPlayerMgr.GetYYMinSerOn())
		{

			bool  bSuc = sPlayerMgr.VerifyToken(pRecv,pRecv->dwConnID);
			if ( !bSuc )
			{
				nRet = E_ProofResult_Account_No_Match;  
				ILOG->Write(_T("-----------------------Warning!!! YY Mixed Server ! Player's GUID verification Failed-------------------\r\n"));  
			}
#ifdef TEST_MIX_SERVER
			TCHAR accoutName[1024];
			memset(accoutName,0,sizeof(accoutName));
			MultiByteToWideChar(CP_ACP,NULL,pRecv->szGUID,-1,accoutName,1024);
			accoutName[1023] = '\0';
		
			ILOG->Write(_T("-----------------------------------验证请求 -----------------------------------\r\n"));
			ILOG->Write(_T("szGUID: %s,bSuc: %d ,ChannelID: %d,ConnID: %d\r\n"), accoutName,(INT)bSuc,(INT)pRecv->dwChannelID,(INT)pRecv->dwConnID);
			   
#endif
		}
		else
		{
			ILOG->Write(_T("-----------------------Warning!!! YY Mixed Server  closed,lllegal Client is logining-------------------\r\n"));  
			IMSG(_T("-----------------------Warning!!! YY Mixed Server  closed,lllegal Client is logining-------------------\r\n"));
		}

	}



	//ILOG->Write(_T("HandleProof sign5\r\n"));
	
    tagDecryptData  DecryptData;
	// 验证玩家要登录的游戏世界是否可用
	LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(pRecv->dwWorldNameCrc);
	if( !P_VALID(pWorld) )
	{
		nRet = E_SelectWorld_GameWorldName_Wrong;
	}

	// 查看玩家要登录的LoongWorld是否正常
	else if( pWorld->GetStatus() != EWS_Well )
	{
		nRet = E_SelectWorld_Server_Maintenance;
	}

	// 查看验证策略是否一致
	else if( pRecv->dwType != sPlayerMgr.GetProofPolicy() )
	{
	      nRet = E_ProofResult_Wrong_Type;
	}
	// 查看版本号是否一致
 	else if( pRecv->dwCurVersionID != sPlayerMgr.GetCurrVer() )
 	{
 		nRet = E_ProofResult_Wrong_Build_Number;
 	}

	// 如果正在验证，或者已经验证完毕
	else if( IsProofing() || IsProofEnd() )
	{
		nRet = E_ProofResult_Proofing;
	}
	// 检查用户名合法
	else
	{

		if ( EPP_Own == sPlayerMgr.GetProofPolicy())
  		{
 			m_Data.dwChannelID = pRecv->dwChannelID ;  //国服混服ID
 		}
 		else
 		{
             m_Data.dwChannelID = 0 ;
 		}

		string strUserName ;
	 	if( sPlayerMgr.GetProofPolicy() == 5 )
		{
			if ( !P_VALID(pRecv->szGUID))
			{
                nRet = E_ProofResult_Account_No_Match;    
			}
			else
			{

				
				ProofPolicyCorn* pProofPolicy =(ProofPolicyCorn*)sPlayerMgr.GetProofPolicyPtr();
				if ( P_VALID(pProofPolicy) )
				{
					BOOL bRet =pProofPolicy->DecryptDataToString(&DecryptData,pRecv->szGUID,pRecv->szPsd);
					if( bRet )
					{
                        strUserName = DecryptData.UserID.c_str();
					
					}
					else
					{
						nRet = E_ProofResult_Account_No_Match;  
					}
					
				}
			}
			
		}
		else if( EPP_Russia == sPlayerMgr.GetProofPolicy() )
		{
            strUserName = pRecv->szUserID;
		}
		//金山快快的话 就用GUID替换成用户名
		else if ( ECT_JinShanKuaiKuai == GetChannelID() || ECT_YYYuYinPingTai == GetChannelID() || ELM_Web_LoginMode == pRecv->dwLoginMode)
		{
			//ILOG->Write(_T("WebLoginSetGUID Start"));
			strUserName = pRecv->szGUID;
			//ILOG->Write(_T("WebLoginSetGUID End"));
		}
		else if ( EPP_Korea == sPlayerMgr.GetProofPolicy()  )
		{
			strUserName = pRecv->szGUID;
		}
		else
		{
			strUserName = pRecv->szUserName;
		}

  		
		
		

 		if( CheckName(strUserName) )
 		{
			nRet = E_ProofResult_Account_No_Match;
#ifdef TEST_MIX_SERVER
			TCHAR accoutName[1024];
			memset(accoutName,0,sizeof(accoutName));
			MultiByteToWideChar(CP_ACP,NULL,strUserName.c_str(),-1,accoutName,1024);

		
			ILOG->Write(_T("szGUID: %s ,ChannelID: %d,dwCornID: %d\r\n"), accoutName,(INT)pRecv->dwChannelID,m_Data.dwClientID);
			ILOG->Write(_T("Check UserName Failed!\r\n"));
#endif
#ifdef KOREA_LOG
			TCHAR accoutName1[1024];
			memset(accoutName1,0,sizeof(accoutName1));
			MultiByteToWideChar(CP_ACP,NULL,strUserName.c_str(),-1,accoutName1,1024);


			SPE_LOG->Write(_T("szGUID: %s ,ChannelID: %d,dwCornID: %d\r\n"), accoutName1,(INT)pRecv->dwChannelID,m_Data.dwClientID);
			SPE_LOG->Write(_T("Check UserName Failed!\r\n"));
#endif

		}
		else
 		{
			strUserName.copy(m_Data.szAccountName, X_SHORT_NAME);
#ifdef UNICODE
			MultiByteToWideChar(CP_ACP,NULL,m_Data.szAccountName,-1,m_cAccountNameT,X_SHORT_NAME);
#else
			strcpy(m_cAccountNameT,m_Data.szAccountName);
#endif
	    }
	}

	//ILOG->Write(_T("HandleProof sign6\r\n"));
	// 如果有错误，则返回客户端
	if( E_Success != nRet )
	{
		tagNLS_ProofResult send;
		send.dwErrorCode = nRet;
		SendMessage(&send, send.dwSize);
		#ifdef KOREA_LOG
		SPE_LOG->Write(_T("ProofResult: AcountID:%d,ErrorCode:%d!\r\n"),GetAccountID(),send.dwErrorCode);
        #endif

		//ILOG->Write(_T("ProofResult: AcountID:%d,ErrorCode:%d!\r\n"),GetAccountID(),send.dwErrorCode);

#ifdef  KOREA_LOG
		SPE_LOG->Write(_T("-----------------------------------ProofResult failed-----------------------------------\r\n"));
		SPE_LOG->Write(_T("ErrorCode: %u\r\n"), send.dwErrorCode);   
#endif
	}
	// 如果没有错误，则发送帐号验证
	else 
	{
#ifdef KOREA_LOG
		SPE_LOG->Write(_T("-----------------------------------Send data to ProofCenter----------------------------------- \r\n"));
#endif

		sPlayerMgr.Proof(GetClientID(), GetAccoutName(), pRecv->szPsd, pRecv->szGUID, m_Data.dwIP,pRecv->dwWorldNameCrc,pRecv->dwChannelID,pRecv->szMac,pRecv->szUserID,pRecv->szAppKey,pRecv->szToken);
		//ILOG->Write(_T("------sPlayerMgr.Proof-----.\r\n"));  
	}

	ILOG->Write(_T("HandleProof end4 \r\n"));
	return 0;
}

DWORD Player::HandleMibao(tagNetCmd* pCmd)
{
	ILOG->Write(_T("HandleMibao \r\n"));

	tagNLC_MiBao* pRecv = (tagNLC_MiBao*)pCmd;

	// 进行验证
	INT nRet = E_Success;

	if( !IsProofEnd() )
	{
		nRet = E_ProofResult_Mibao_Error;
	}
	else if( !CheckMibao(pRecv->dwMBCrc) )
	{
		nRet = E_ProofResult_Mibao_Error;
	}

	// 如果返回正确
	if( E_Success == nRet )
	{
		// 加入到游戏世界登入列表
		sPlayerMgr.RemovePlayerFromLogining(GetClientID());

		// 加入帐号
		sPlayerMgr.AddAccount(GetAccountID(), GetClientID());

		//将玩家加入到LoongWorld的玩家列表中
		sLoongWorldMgr.AddToWorld(this, GetLoginWorldNameCrc());

	}
	// 如果返回错误
	else
	{
		// 发送给客户端验证错误
		tagNLS_ProofResult send;
		send.dwErrorCode = nRet;
		SendMessage(&send, send.dwSize);
		//ILOG->Write(_T("ProofResult: AcountID:%d,ErrorCode:%d!\r\n"),GetAccountID(),send.dwErrorCode);
	}

	ILOG->Write(_T("HandleMibao end \r\n"));

	return 0;
}


DWORD	Player::HandleSetSecondPsd(tagNetCmd* pCmd)
{
	ILOG->Write(_T("HandleSetSecondPsd \r\n"));

	tagNLC_SetSecondPsd* pRecv = (tagNLC_SetSecondPsd*)pCmd;

	if ( !P_VALID(pRecv) )
	{
		ILOG->Write(_T("HandleSetSecondPsd end1 \r\n"));
		return GT_INVALID;
	}

	tagNLS_SetSecondPsd send;

	if( !IsProofEnd() )
	{
		send.dwEorrorCode  = E_KoreaProof_TwicePsd_SetError;
		SendMessage(&send,send.dwSize);
		ILOG->Write(_T("HandleSetSecondPsd end2 \r\n"));
		return GT_INVALID;
	}

	//如果是修改二次密码
	if ( pRecv->bIsChangeTwPsd )
	{
		BOOL bRet = CheckSecondPsd(pRecv->dwOldSecondPsdCrc);
		if ( !bRet )
		{
			
			send.dwEorrorCode  = E_KoreaProof_InputOldPsd_Error;
			SendMessage(&send,send.dwSize);
			ILOG->Write(_T("HandleSetSecondPsd end3 \r\n"));
			return GT_INVALID;
		}
		else
		{
			
			if ( '\0' == pRecv->czNewSecondPsd[0])
			{
				
				send.dwEorrorCode  = E_KoreaProof_InputNewPsd_Error;
				SendMessage(&send,send.dwSize);
				ILOG->Write(_T("HandleSetSecondPsd end4 \r\n"));
				return GT_INVALID;
			}
			pRecv->czNewSecondPsd[X_SHORT_NAME-1] = '\0';
			m_Data.dwSecondPsdCrc = IUTIL->Crc32(pRecv->czNewSecondPsd);
			bool bRet = sBeton.SetSecondPsd(pRecv->czNewSecondPsd,GetAccountID());

			send.dwEorrorCode = bRet ? E_Success : E_KoreaProof_TwicePsd_ChgError;
			SendMessage(&send,send.dwSize);
			ILOG->Write(_T("HandleSetSecondPsd end5 \r\n"));
			return true;
		}
	}
	else
	{
		pRecv->czNewSecondPsd[X_SHORT_NAME-1] = '\0';
		DWORD dwNewTwicePsdCrc = IUTIL->Crc32(pRecv->czNewSecondPsd);
		if ( (GT_INVALID == dwNewTwicePsdCrc) || 0 == dwNewTwicePsdCrc )
		{
			send.dwEorrorCode  = E_KoreaProof_TwicePsd_SetError;
			SendMessage(&send,send.dwSize);
			ILOG->Write(_T("HandleSetSecondPsd end6 \r\n"));
			return GT_INVALID;
		}
		m_Data.dwSecondPsdCrc = dwNewTwicePsdCrc ;
		bool bRet = sBeton.SetSecondPsd(pRecv->czNewSecondPsd,GetAccountID());
		send.dwEorrorCode = bRet ? E_Success : E_KoreaProof_TwicePsd_SetError;
		SendMessage(&send,send.dwSize);
		ILOG->Write(_T("HandleSetSecondPsd end7 \r\n"));
		return true;
	}
	ILOG->Write(_T("HandleSetSecondPsd end8 \r\n"));
	return 0;
}

DWORD	Player::HandleInputSecondPsd(tagNetCmd* pCmd)
{
	ILOG->Write(_T("HandleInputSecondPsd \r\n"));

	tagNLC_InputSecondPsd* pRecv = (tagNLC_InputSecondPsd*)pCmd;
	
	if ( !P_VALID(pRecv) )
	{
		ILOG->Write(_T("HandleInputSecondPsd end1 \r\n"));
		return GT_INVALID;
	}
	

	INT nRet = E_Success;

	if( !IsProofEnd() )
	{
		nRet = E_KoreaProof_TwicePsd_InputError;
	}

	bool bRet  = CheckSecondPsd(pRecv->dwTwicePsdCrc);
	if( !bRet )
	{
		nRet = E_KoreaProof_TwicePsd_InputError;
	}

	// 如果返回正确
	if( E_Success == nRet )
	{
		// 加入到游戏世界登入列表
		sPlayerMgr.RemovePlayerFromLogining(GetClientID());

		// 加入帐号
		sPlayerMgr.AddAccount(GetAccountID(), GetClientID());

		//将玩家加入到LoongWorld的玩家列表中
		sLoongWorldMgr.AddToWorld(this, GetLoginWorldNameCrc());

	}
	// 如果返回错误
	else
	{
		// 发送给客户端验证错误
		tagNLS_InputSecondPsd send;
		send.dwEorrorCode = nRet;
		SendMessage(&send, send.dwSize);
	}	

	ILOG->Write(_T("HandleInputSecondPsd end2 \r\n"));
	return 0;
}

//-------------------------------------------------------------------------------
// 替换掉姓名中的回车和换行,同时检查用户名是否为空
//-------------------------------------------------------------------------------
BOOL Player::CheckName(string& str)
{
	// 去掉回车
	INT nFind = 0;
	while( (nFind = str.find('\r', nFind)) != str.npos )
		str.replace(nFind, 1, "");

	// 去掉换行
	nFind = 0;
	while( (nFind = str.find('\n', nFind)) != str.npos )
		str.replace(nFind, 1, "");

	// 去掉tab
	nFind = 0;
	while( (nFind = str.find('\t', nFind)) != str.npos )
		str.replace(nFind, 1, "");

	// 去掉空格
	if((nFind = str.find(' ')) != str.npos)
	{
		str = str.substr(0,nFind);
	}

	// 检查用户名是否为空
	return str.empty();
}

//--------------------------------------------------------------------------------
// 生成验证用密保
//--------------------------------------------------------------------------------
BOOL Player::GenerateMibao(CHAR szMibao[MAX_MIBAO])
{
	// 从数据库里面查询该玩家的密报值
	CHAR szMibaoInDB[MIBAO_LEN] = {'\0'};
	if( !sBeton.GetPlayerMibao(GetAccountID(), szMibaoInDB) )
	{
		return FALSE;
	}

	CHAR szValue[MAX_MIBAO] = {'\0'};
	// 密保的格式：以逗号分隔的字符序列，8*10的矩阵
	// 例如：97,55,87,21,90,33,19,36,55,90,26,96,24,61,32,27,70,64,86,69,97,54,36,21,
	// 18,58,55,96,37,32,75,64,08,87,08,74,33,13,34,90,70,14,09,98,93,37,19,75,21,68,51,
	// 46,59,41,86,69,13,93,00,15,48,36,57,50,16,98,24,57,38,63,91,28,53,06,35,40,61,59,94,15,
	for(INT n = 0; n < 3; ++n)
	{
		INT nRand = IUTIL->Rand() % 80;		// 随机值

		INT nX = nRand / 10;	// x轴，从'A'到'H'
		INT nY = nRand % 10;	// y轴，从'0'到'9'

		// 向密保行列中写入行列号
		szMibao[n*2]	=	'A' + nX;
		szMibao[n*2+1]	=	'0' + nY;

		// 向值中写入密保卡值
		szValue[n*2]	=	szMibaoInDB[nRand*3];
		szValue[n*2+1]	=	szMibaoInDB[nRand*3+1];
	}

	// 将szValue算出CRC保存在起来
	m_Data.dwMibaoCrc = IUTIL->Crc32((LPBYTE)szValue, MAX_MIBAO);


	return TRUE;
}

//--------------------------------------------------------------------------------
// 检测密保
//--------------------------------------------------------------------------------
BOOL Player::CheckMibao(DWORD dwResultCrc)
{
	return m_Data.dwMibaoCrc == dwResultCrc;
}

//--------------------------------------------------------------------------------
// 检测二次密码
//--------------------------------------------------------------------------------
BOOL Player::CheckSecondPsd(DWORD dwResultCrc)
{
	if ( (GT_INVALID == dwResultCrc) || (0 == dwResultCrc) )
	{
		return FALSE;
	}

	return m_Data.dwSecondPsdCrc == dwResultCrc;
}

//--------------------------------------------------------------------------------
// 自适应类型版本
//--------------------------------------------------------------------------------
LPCTSTR	Player::GetAccoutNameT()
{
	return m_cAccountNameT;
}

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 获取下载地址
//--------------------------------------------------------------------------
VOID	Player::SendWebLoginInfoToClient()
{
	//ILOG->Write(_T("Player::SendWebLoginInfoToClient Start \r\n"));
	BYTE  pByte[1024*10] = {0};
	DWORD dwSize = 0;
	sPlayerMgr.GetWebLoginInfo(pByte,dwSize);
	if ( dwSize <= 0)
	{
		return;
	}
	DWORD dwMsgSize = sizeof(tagNLS_ProofResult) + dwSize;
	BYTE* p = new BYTE[dwMsgSize];
	memset(p,0,dwMsgSize);
	tagNLS_ProofResult* pSend = (tagNLS_ProofResult*)p;
	pSend->dwID        = pSend->Crc32("NLS_ProofResult");
	pSend->dwSize      = dwMsgSize;
	pSend->dwErrorCode = E_WebLoginMode_GetURL;
	pSend->dwURLNum    = sPlayerMgr.GetChannelNum();

	memcpy(pSend->byData,pByte,dwSize);

	SendMessage(pSend,dwMsgSize);
	SAFE_DEL_ARRAY(p);

	//ILOG->Write(_T("Player::SendWebLoginInfoToClient End \r\n"));
}

//--------------------------------------------------------------------------------
bool  Player::SendWebProof(LPVOID pWebLoginProof,DWORD dwTokenID,DWORD dwCorpID)
{
	//ILOG->Write(_T("Player::SendWebProof Start \r\n"));
	tagNLS_WebLoginProof msg;

	CHAR  czRechargeURL[X_LONG_NAME] = {0};
	CHAR  czLoginURL[X_LONG_NAME]	 = {0};
	DWORD dwRechargeURLSize			 =  0 ;
	DWORD dwLoginURLSize			 =  0 ;
	bool bRet =	sPlayerMgr.GetRechargeURL(czLoginURL,dwLoginURLSize,czRechargeURL,dwRechargeURLSize,dwCorpID);
	if ( !bRet )
	{
		return false;
	}
	DWORD dwMsgSize = sizeof(tagNLS_WebLoginProof) +  dwRechargeURLSize;
	if( dwMsgSize > 1024)
	{
		return false;
	}
	PBYTE pDataMsg = new BYTE[dwMsgSize];
	memset(pDataMsg,0,dwMsgSize);
	tagNLS_WebLoginProof* pSend = (tagNLS_WebLoginProof*)pDataMsg ;
	pSend->dwID                 = msg.dwID ;
	pSend->dwSize				= dwMsgSize;
	pSend->dwLoginID			= g_login.GetSectionID();
	pSend->dwTokenID			= dwTokenID;
	pSend->dwURLSize			= dwRechargeURLSize;


	stringstream strKey;
	strKey << dwTokenID;
	strKey << g_login.GetSectionID();
	strKey << dwCorpID;
	strKey << "mfkgh56y23utg&^%33455";

	//sign MD5( token + loginid + cropid +key)值,转大写
	CHAR szKey[X_LONG_NAME] ={0};
	memcpy(szKey, strKey.str().c_str(), strKey.str().length()*sizeof(CHAR));
	szKey[X_LONG_NAME-1] = '\0';
	strupr(szKey);
	BYTE digest[16];
	IUTIL->MD5ForString( szKey, digest );
	string strPswMD5;
	IUTIL->MD5ToString( strPswMD5, digest );

	memcpy(pSend->szSign,strPswMD5.c_str(),strPswMD5.length()*sizeof(CHAR));
	memcpy(pSend->szURL,czRechargeURL,dwRechargeURLSize);
	memcpy(pSend->szLoginURL,czLoginURL,dwLoginURLSize);
	SendMessage(pSend,pSend->dwSize);

	SAFE_DEL_ARRAY(pDataMsg);


	stringstream szGUID;
	szGUID << dwTokenID;
	szGUID << ";";
	szGUID << g_login.GetSectionID();
	szGUID << ";";
	szGUID << dwCorpID;
	szGUID << ";";

	char* pGUID = (char*)pWebLoginProof;
	DWORD dwGUIDSize = sizeof(CHAR)*szGUID.str().length();
	memcpy(pWebLoginProof,szGUID.str().c_str(),dwGUIDSize);
	pGUID[dwGUIDSize] = '\0';

	//ILOG->Write(_T("Player::SendWebProof End \r\n"));
	return true;
}
//--------------------------------------------------------------------------------