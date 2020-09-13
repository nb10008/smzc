//-----------------------------------------------------------------------------
//!\file player_mgr.cpp
//!\author Aslan
//!
//!\date 2008-11-30
//! last 2008-11-30
//!
//!\brief 玩家管理器
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/login.h"

#include "player_mgr.h"
#include "player.h"
#include "loongworld_mgr.h"
#include "loongworld.h"
#include "db.h"
#include "proof_policy_mgr.h"
#include "proof_policy.h"
#include "login_server.h"
#include "yauthLib.h"
#include "kr_util.h"
#include "rijndael.h"
#include "event_mgr.h"
#include "proof_policy_cornEx.h"
Rijndael  g_KrUtil;
using namespace namepolicy;

//-------------------------------------------------------------------------------
// 构造函数
//-------------------------------------------------------------------------------
PlayerMgr::PlayerMgr() : m_Trunk(this), m_nPlayerNum(0), m_nPlayerLoging(0), m_nProofResultNum(0),
						m_dwClientIDGen(0), m_dwCurrVer(0), m_nPort(0), m_ePolicy(EPP_Null),m_pPolicy(NULL),m_bUseGMAccess(TRUE),
						m_bYYMixSerOn(false),m_hInstYYDLL(NULL),m_pYAuth_init(NULL),m_pYAuth_cleanUp(NULL),m_YAuth_genRequest(NULL),
						m_pYAuth_verifyToken(NULL),m_pYAuth_HexToString(NULL),m_pYAuth_GetAttribute(NULL),m_bWebLoginModeOn(false),m_dwChannelNum(0)
						
{
}

//-------------------------------------------------------------------------------
// 析构函数
//-------------------------------------------------------------------------------
PlayerMgr::~PlayerMgr()
{
	Destroy();
}

//-------------------------------------------------------------------------------
// 初始化函数
//-------------------------------------------------------------------------------
BOOL PlayerMgr::Init()
{
	// vEngine
	m_pUtil = "Util";

	// 读取配置文件
	TObjRef<VarContainer> pVar = "LoginServerConfig";

	//初始化
	if(!InitConfig())
	{
		return FALSE;
	}

	sBeton.LoadCacheAccountData(m_mapAccountData, m_mapAccountNameCrc2AccountID);

	// 读取验证方式
	m_ePolicy = (enum EProofPolicy)pVar->GetInt(_T("policy"), _T("proof"));
	if( m_ePolicy < EPP_Test || m_ePolicy >= EPP_End || 8 == m_ePolicy || 9 == m_ePolicy)
	{
		return FALSE;
	}

	m_bYYMixSerOn = (pVar->GetInt(_T("YY_On"), _T("mixedserver")) == 0) ? false : true;
     
	m_bWebLoginModeOn = (pVar->GetInt(_T("WebLogin_On"), _T("web_loginmode")) == 0) ? false : true;

	// 生成验证方式
	m_pPolicy = sProofPolicyMgr.CreateProofPolicy(m_ePolicy);
	if( !P_VALID(m_pPolicy) )
	{
		return FALSE;
	}
	if( !m_pPolicy->Init((PROOFCALLBACK)m_Trunk.sfp2(&PlayerMgr::ProofCallBack)) )
	{
		return FALSE;
	}
	g_KrUtil.init(Rijndael::CBC, Rijndael::Decrypt, key, Rijndael::Key32Bytes, iv);

	// 读取版本号
	LPCTSTR szVerNum = pVar->GetString(_T("version version"));
	m_dwCurrVer = m_pUtil->Crc32(szVerNum);

	// 读取端口号
	m_nPort = pVar->GetInt(_T("port player_session"));

	// 初始化网络
	m_pNetSession = "XServer";

	tagXServerConfig InitParam;
	InitParam.fnLogIn			=	(XLOGINCALLBACK)m_Trunk.sfp2(&PlayerMgr::LoginCallBack);
	InitParam.fnLogOut			=	(XLOGOUTCALLBACK)m_Trunk.sfp1(&PlayerMgr::LogoutCallBack);
	InitParam.bReusePort		=	true;
	InitParam.nPort				=	m_nPort;
	InitParam.nMaxServerLoad	=	20000;
	InitParam.nAcceptExNum		=	128;

	m_pNetSession->Init(InitParam);
	m_nPort = m_pNetSession->GetConfig()->nPort;

	// 注册网络消息
	Player::RegisterPlayerMsg();



	//YY的接口初始化
	if ( m_bYYMixSerOn )
	{
		m_hInstYYDLL = LoadLibrary(_T("yauth.conf\\yauthlib.dll"));
		if ( NULL == m_hInstYYDLL)
		{
			::MessageBox(0,_T("缺少混服YY语音平台动态库!"),_T("警告"),0);
			return FALSE;
		}
		m_pYAuth_init = (PtrInitType)GetProcAddress(m_hInstYYDLL,"YAuth_init");

		m_pYAuth_cleanUp = (PtrCleanUpType)GetProcAddress(m_hInstYYDLL,"YAuth_cleanUp");

		m_YAuth_genRequest = (PtrGenRequestTypeEx)GetProcAddress(m_hInstYYDLL,"YAuth_genRequest");

		m_pYAuth_verifyToken = (PtrVerifyTokenType)GetProcAddress(m_hInstYYDLL,"YAuth_verifyToken");

		m_pYAuth_HexToString = (PtrHexToStringType)GetProcAddress(m_hInstYYDLL,"YAuth_fromHexToStr");

		m_pYAuth_GetAttribute = (PtrGetAttributeType)GetProcAddress(m_hInstYYDLL,"YAuth_getAttribute");

		if ( NULL == m_pYAuth_init || NULL == m_pYAuth_cleanUp || NULL == m_YAuth_genRequest || 
			NULL == m_pYAuth_verifyToken || NULL == m_pYAuth_HexToString || NULL == m_pYAuth_HexToString)
		{
			::MessageBox(0,_T("获取YY动态库导出函数失败!"),_T("警告"),1);
			return FALSE;
		}

		if (  NULL != m_pYAuth_init )
		{
			DWORD dwEorror = m_pYAuth_init();
			if (  1 != dwEorror)
			{
				::MessageBox(0,_T("YY动态库初始化失败!"),_T("警告"),1); 
				return FALSE;
			}
		}
	}

	if ( m_bWebLoginModeOn )
	{
		InitWebNoteInfo();
	}

	return TRUE;
}

//-------------------------------------------------------------------------
VOID PlayerMgr::InitWebNoteInfo()
{
	// 读取配置文件
	TObjRef<VarContainer> pVar = "LoginServerConfig";
	m_dwChannelNum	 = pVar->GetDword(_T("Num"), _T("web_loginmode"));
	for (int i = 0; i < (INT)m_dwChannelNum; i++ )
	{
		TCHAR szURL[30] =  {0};
		_stprintf(szURL,_T("URL%d"),i+1);
		tstring tstrURL = pVar->GetString(szURL, _T("web_loginmode"));
		string  strURL = IUTIL->UnicodeToAnsi(tstrURL.c_str());

		memset(szURL,0,sizeof(szURL));
		_stprintf(szURL,_T("RechargeURL%d"),i+1);
		tstring tstrRechargeURL = pVar->GetString(szURL, _T("web_loginmode"));
		string  strRechargeURL = IUTIL->UnicodeToAnsi(tstrRechargeURL.c_str());

		memset(szURL,0,sizeof(szURL));
		_stprintf(szURL,_T("DescName%d"),i+1);
		tstring tstrDescName = pVar->GetString(szURL, _T("web_loginmode"));

		memset(szURL,0,sizeof(szURL));
		_stprintf(szURL,_T("LoginURL%d"),i+1);
		tstring tstrLoginURL = pVar->GetString(szURL, _T("web_loginmode"));
		string  strLoginURL = IUTIL->UnicodeToAnsi(tstrLoginURL.c_str());


		memset(szURL,0,sizeof(szURL));
		_stprintf(szURL,_T("CropID%d"),i+1);
		DWORD dwCropID = pVar->GetDword(szURL, _T("web_loginmode"));
		tagWebNoteInfo* p = new tagWebNoteInfo;
		if( !P_VALID(p) )
		{     
			::MessageBox(0,_T("tagWebNoteInfo 内存分配失败!"),_T("警告"),1); 
			return;
		}
		if ( tstrDescName.length() >= CROP_NAME_MAX_LEN)
		{
			memset(szURL,0,sizeof(szURL));
			_stprintf(szURL,_T("descname : %s 超过了%d 个字符，请重新配置!"),tstrDescName.c_str(),(CROP_NAME_MAX_LEN-1));
			::MessageBox(0,szURL,_T("警告"),1); 
			SAFE_DEL(p);
			return;    
		}
		p->dwCropID			= dwCropID;
		p->strURL			= strURL;
		p->strRechargeURL	= strRechargeURL;
		p->strLoginURL		= strLoginURL;
		p->tstrDescName		= tstrDescName;
		if ( m_MapURL.IsExist(p->dwCropID))
		{
			memset(szURL,0,sizeof(szURL));
			_stprintf(szURL,_T("dwCropID : %u 重复配置!"),p->dwCropID);
			::MessageBox(0,szURL,_T("警告"),1); 
			SAFE_DEL(p);
			continue;
		}
		m_MapURL.Add(p->dwCropID,p);		
	}

}
//-------------------------------------------------------------------------
VOID PlayerMgr::DestroyWebNoteInfo()
{
	tagWebNoteInfo* pNote = NULL;
	m_MapURL.ResetIterator();
	while( m_MapURL.PeekNext( pNote ) )
	{
		if( P_VALID(pNote) )
		{
			SAFE_DEL(pNote);
		}

	}
	m_MapURL.Clear();
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// 初始化配置
//-------------------------------------------------------------------------
BOOL PlayerMgr::InitConfig()
{
	TObjRef<VarContainer> pVar = "GMAccess";
	if( !P_VALID(pVar) ) return FALSE;

	std::set <tstring> setAccountName;
	
	m_bUseGMAccess = pVar->GetInt(_T("on switch"));//是否使用gm地址限制

	//读取account名
	INT iAccountNum = pVar->GetInt(_T("num account"));

	TCHAR szTemp[X_SHORT_NAME] = {_T('\0')};
	for(int iLoop = 1; iLoop<=iAccountNum; ++iLoop)
	{
		_stprintf(szTemp,_T("name%d"),iLoop);
		setAccountName.insert(pVar->GetString(szTemp,_T("account")));
	}
	CHAR cIP[X_IP_LEN]= {'\0'};
	std::set <tstring>::iterator itLoop = setAccountName.begin();
	for(;itLoop != setAccountName.end();++itLoop)
	{
		INT iIPNum = pVar->GetInt(_T("num"), (*itLoop).c_str());
		for(int iLoop = 1; iLoop<=iIPNum; ++iLoop)
		{
			_stprintf(szTemp,_T("ip%d"),iLoop);

			//转成ansi
			tstring strIP = pVar->GetString(szTemp,(*itLoop).c_str());
			WideCharToMultiByte(CP_OEMCP,NULL,strIP.c_str(),-1,cIP,X_IP_LEN,NULL,FALSE);

			//转成DWORD
			DWORD dwIP = inet_addr(cIP);
			m_mapGMvsIP.insert( make_pair((*itLoop).c_str(),dwIP) );

		}		
	}
	// 台湾 读取配置文件
	TObjRef<VarContainer> pVarTemp = "LoginServerConfig";
	if( !P_VALID(pVarTemp) ) return FALSE;

	m_bGMLogin = (pVarTemp->GetInt(_T("on"), _T("gm_login")) == 0) ? false : true;

	if ( m_bGMLogin )
	{
		INT dwChannelNum	 = pVarTemp->GetDword(_T("num"), _T("gm_login"));
		for (INT i = 0; i < dwChannelNum; i++ )
		{
			TCHAR szTemp[30] =  {_T('\0')};
			_stprintf(szTemp,_T("ip%d"),i+1);
			tstring tstrTemp = pVarTemp->GetString(szTemp, _T("gm_login"));
			string  strURL = IUTIL->UnicodeToAnsi(tstrTemp.c_str());

			DWORD dwIP = inet_addr(strURL.c_str());
			m_setGMIP.insert(dwIP);
		}		
	}

	return TRUE;
}
//-------------------------------------------------------------------------------
// 销毁函数
//-------------------------------------------------------------------------------
VOID PlayerMgr::Destroy()
{
	// 取消注册网络消息
	Player::UnRegisterPlayerMsg();

	// 注销网络
	m_pNetSession->Destroy();

    // 清空正在登录的玩家
    Player* pPlayer = NULL;
    m_mapLoginingPlayer.ResetIterator();
    while( m_mapLoginingPlayer.PeekNext(pPlayer) )
    {
        SAFE_DEL(pPlayer);
    }

	// 验证策略销毁
	m_pPolicy->Destroy();

	// 删除验证策略
	sProofPolicyMgr.DestroyProofPolicy(m_ePolicy, m_pPolicy);

	// 清除账号名缓冲
	CleanCachedAccountDatas();

	//YY的接口销毁
	if ( 0 != m_bYYMixSerOn )
	{
       if ( NULL != m_pYAuth_cleanUp )
       {
		   m_pYAuth_cleanUp();
       }
       
	   FreeLibrary(m_hInstYYDLL);
	}

	if ( m_bWebLoginModeOn )
	{
		DestroyWebNoteInfo();
	}

}

//-------------------------------------------------------------------------------
// 更新函数
//-------------------------------------------------------------------------------
VOID PlayerMgr::Update()
{
	// 更新玩家消息
    UpdateSession();

	// 更新验证结果
	UpdateProofResult();

	// 得到统计信息
	InterlockedExchange((LPLONG)&m_nPlayerNum,		m_mapAllPlayer.Size());
	InterlockedExchange((LPLONG)&m_nPlayerLoging,	m_mapLoginingPlayer.Size());
	InterlockedExchange((LPLONG)&m_nProofResultNum,	m_listProofResult.Size());
}

//-------------------------------------------------------------------------------
// 更新玩家消息
//-------------------------------------------------------------------------------
VOID PlayerMgr::UpdateSession()
{
	Player* pPlayer = NULL;
	m_mapLoginingPlayer.ResetIterator();
	while( m_mapLoginingPlayer.PeekNext(pPlayer) )
	{
		INT nRet = pPlayer->HandleMessage();

		if( CON_LOST == nRet )	// 掉线
		{
			m_mapLoginingPlayer.Erase(pPlayer->GetClientID());
			PlayerLogout(pPlayer);
		}
	}
}

//-------------------------------------------------------------------------------
// 更新验证返回
//-------------------------------------------------------------------------------
VOID PlayerMgr::UpdateProofResult()
{
	// 每个Tick最多处理20个人
	INT nNum = 20;

	// 取出验证结果
	tagProofResultFull* pResult = m_listProofResult.PopFront();

	while( P_VALID(pResult) )
	{
		// 找到玩家
		Player* pPlayer = m_mapLoginingPlayer.Peek(pResult->dwClientID);
		if( !P_VALID(pPlayer) )
		{
#ifdef  KOREA_LOG
			SPE_LOG->Write(_T("pPlayer not find.client id is:%d.\r\n"),pResult->dwClientID);
#endif
			ILOG->Write(_T("pPlayer not find.client id is:%d.\r\n"),pResult->dwClientID);
			SAFE_DEL(pResult);
			pResult = m_listProofResult.PopFront();
			continue;
		}

		if(m_ePolicy == EPP_QuanQuan)
		{
			pPlayer->SetReservedData(pResult->byReserved[0]);
		}

		if( E_Success != pResult->nRet )
		{
			// 如果返回帐号已经登陆
			if( E_ProofResult_Account_In_Use == pResult->nRet )
			{
				// 如果数据库是在线状态，则发往world服务器踢人
				if( EPLS_Online == pResult->eLoginStatus )
				{
					LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(pResult->dwWorldNameCrc);
					if( P_VALID(pWorld) )
					{
						tagNLW_KickPlayer send;
						send.dwAccountID = pResult->dwAccountID;
						pWorld->SendKickPlayerMsg(send);
					}
					else
					{
						sBeton.FixPlayerLoginStatus(pResult->dwAccountID, EPLS_OffLine);
						pResult->nRet = E_Success;	// 注意这个地方
					}
				}
				else if(EPLS_Unknown == pResult->eLoginStatus)	//如果检测到时未知也踢掉吧，有的时候玩家会被卡死在游戏~~
				{
					LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(pResult->dwWorldNameCrc);
					if( P_VALID(pWorld) )
					{
						tagNLW_KickPlayer send;
						send.dwAccountID = pResult->dwAccountID;
						pWorld->SendKickPlayerMsg(send);
						sBeton.FixPlayerLoginStatus(pResult->dwAccountID, EPLS_OffLine);
					}
					else
					{
						sBeton.FixPlayerLoginStatus(pResult->dwAccountID, EPLS_OffLine);
						pResult->nRet = E_Success;	// 注意这个地方
					}
				}
				// 如果这时候帐号处在登陆中
				else if( EPLS_Loging == pResult->eLoginStatus )
				{
					// 在本地查找是否该账号在login里面，如果有，不管了
					if( IsAccountExist(pResult->dwAccountID) )
					{

					}
					// 如果没有，则可能是因为有些情况导致不同步，那么就直接清空数据库，并设置成功
					else
					{
						sBeton.FixPlayerLoginStatus(pResult->dwAccountID, EPLS_OffLine);
						pResult->nRet = E_Success;	// 注意这个地方
					}
				}
			}
			//如果是GM工具封停 往world_forbid表里搜索一下
			else if( pResult->nRet == E_ProofResult_Forbid_GMTool )
			{
				BOOL bForbid = sBeton.IfWorldForbid(pResult->dwAccountID,pPlayer->GetLoginWorldNameCrc());

				// 这表示该账号可能是别的游戏世界被封停了
				if( bForbid == FALSE )
				{
					pResult->nRet = E_Success;	// 注意这个地方
				}
			}
			// 其它情况
			else
			{

			}
		}
		// 验证成功
		else
		{
			// 如果是权限为gm到m_mapGMvsIP表里找
			if( m_bUseGMAccess && (pResult->byPrivilege != 0) )
			{
				BOOL bFind = FALSE;
				typedef std::multimap<tstring, DWORD>::iterator itFinder;

				itFinder beg = m_mapGMvsIP.lower_bound(pPlayer->GetAccoutNameT()),
					end = m_mapGMvsIP.upper_bound(pPlayer->GetAccoutNameT());
				while( beg != end )
				{
					if(beg->first == pPlayer->GetAccoutNameT() 
						&& beg->second == pPlayer->GetPlayerData().dwIP )
					{
						bFind = TRUE;
						break;
					}
					++beg;
				}
				if( bFind == FALSE )
				{
					// 目前对应成这个
					pResult->nRet = E_ProofResult_Account_No_Match;	// 注意这个地方

					TObjRef<StreamTransport> pTrans;
					pTrans = "StreamTransport";
					char szIP[X_IP_LEN];
					strcpy(szIP,pTrans->IP2String(pPlayer->GetPlayerData().dwIP));

					TCHAR tszIP[X_IP_LEN];
#ifdef UNICODE
					MultiByteToWideChar(CP_ACP,NULL,szIP,-1,tszIP,X_IP_LEN);
#else
					strcpy(tszIP,szIP);
#endif
					ILOG->Write(_T("GM:%s非法登录服务器!!	IP:%s\r\n"),pPlayer->GetAccoutNameT(),tszIP);
#ifdef  KOREA_LOG
					SPE_LOG->Write(_T("GM:%s非法登录服务器!!	IP:%s\r\n"),pPlayer->GetAccoutNameT(),tszIP);
#endif
				}	
			}
		}

		// 如果对应客户端的ip被封了
		if( TRUE == sBeton.IfIPForbid(pPlayer->GetPlayerData().dwIP) || TRUE == sBeton.IfMacForbid(pPlayer->GetPlayerData().szMac) )
		{
			//目前对应成这个
			pResult->nRet = E_ProofResult_Account_No_Match;	// 注意这个地方
#ifdef  KOREA_LOG
			SPE_LOG->Write(_T("对应客户端的ip被封了"));
#endif
		}

		// 如果验证成功，此时再验证一下游戏世界
		if( E_Success == pResult->nRet )
		{
			// 找到玩家所要请求进入的游戏世界
			LoongWorld* pWorld = sLoongWorldMgr.GetLoongWorld(pPlayer->GetLoginWorldNameCrc());
			if( !P_VALID(pWorld) || EWS_Well != pWorld->GetStatus() )
			{
				pResult->nRet = E_SelectWorld_Server_Maintenance;
			}
		}

		if( sLoongWorldMgr.IsInPKPunishmentState(pResult->dwAccountID) )
			pResult->nRet = E_PrrofResult_PKRunawayPunishmentState;

		// 最后验证完毕
		if( E_Success == pResult->nRet )
		{
			//ILOG->Write(_T("Proof Result:%d!\r\n"),pResult->nRet);
			// 设置玩家验证完毕
			pPlayer->ProofReturn(pResult);

			//设置玩家上次登录ip和时间
			pPlayer->SetPreLoginIP(pResult->dwPreLoginIP);
			pPlayer->SetPreLoginTime(pResult->dwPreLoginTime);

			if ( GetGMLoginOn() )
			{
				std::set<DWORD>::iterator it = m_setGMIP.find(pPlayer->GetCurrIP());
				bool bIsGM = false;
				if ( it != m_setGMIP.end() )
				{
					bIsGM = true;
				}
				pPlayer->SetGM(bIsGM);
			}

			if( EPP_Korea != GetProofPolicy() )
			{	
				// 如果该玩家还需要密保
				if( pPlayer->IsNeedMibao() )
				{
					tagNLS_MiBao send;
					pPlayer->GenerateMibao(send.szMiBao);	// 生成密保
					pPlayer->SendMessage(&send, send.dwSize);
				}
				// 不需要密保，则加入到游戏世界列表中等待
				else
				{
					RemovePlayerFromLogining(pPlayer->GetClientID());

					// 加入帐号
					AddAccount(pPlayer->GetAccountID(), pPlayer->GetClientID());

					// 将玩家加入到LoongWorld的玩家列表中
					sLoongWorldMgr.AddToWorld(pPlayer, pPlayer->GetLoginWorldNameCrc());

					//ILOG->Write(_T("Proof Result AddToWorld"));
				}
			}
			else
			{
					tagNLS_SecondPsdProof send;
					send.bIsFirstLogin	= pResult->bIsFirstLogin;
					pPlayer->SendMessage(&send, send.dwSize);
			}
		}
		// 如果最终没有验证成功
		else
		{
			ILOG->Write(_T("Proof Result:%d!\r\n"),pResult->nRet);

			tagNLS_ProofResult send;
			// Jason 2010-4-26 v2.1.0
			//if( E_PlayerLogin_PKRunawayPunishmentState == pResult->nRet )
			//	send.dwErrorCode = E_PrrofResult_PKRunawayPunishmentState;
			//else
				send.dwErrorCode = pResult->nRet;
#ifdef  KOREA_LOG
				SPE_LOG->Write(_T("-----------------------------------PlayerMgr ProofResult failed-----------------------------------\r\n"));
				SPE_LOG->Write(_T("ErrorCode: %u\r\n"), send.dwErrorCode);   
#endif

			pPlayer->SendMessage(&send, send.dwSize);

			//ILOG->Write(_T("ProofResult: AcountID:%d,ErrorCode:%d!\r\n"),pPlayer->GetAccountID(),pResult->nRet);
		}

		SAFE_DEL(pResult);

		// 减一个人，如果减到0就退出
		if( --nNum <= 0 ) break;

		pResult = m_listProofResult.PopFront();
	}
}

//-------------------------------------------------------------------------------
// 登录回调函数
//-------------------------------------------------------------------------------
DWORD PlayerMgr::LoginCallBack(LPVOID p1, LPVOID p2)
{
    //查看第一条消息是否正确
    static DWORD dwMsgCrc = m_pUtil->Crc32("NLC_SafeProof");

	tagUnit* pUnit = (tagUnit*)p1;
	tagXLoginParam* pParam = (tagXLoginParam*)p2;

	tagNLC_SafeProof* pCmd = (tagNLC_SafeProof*)pUnit->pBuffer;

#ifdef  KOREA_LOG
	SPE_LOG->Write(_T("------------------------------------Login Call Back-----------------------------------\r\n"));
#endif

	// 检查消息的Crc是否相同
	if( pCmd->dwID != dwMsgCrc )
	{
#ifdef  KOREA_LOG
		SPE_LOG->Write(_T("-----------------------------------dwID != dwMsgCrc  -----------------------------------\r\n"));
#endif
		return GT_INVALID;
	}

	//加入验证key
	DWORD dwProofKey = 0;
	ProofKey(dwProofKey,pCmd);
	if(dwProofKey != pCmd->dwProofKey)
	{
#ifdef  KOREA_LOG
		SPE_LOG->Write(_T("-----------------------------------加入验证key  -----------------------------------\r\n"));
#endif
		return GT_INVALID; //验证key不对
	}

	// 锁住Update
	g_login.LockUpdate();

	// 生成一个新的客户端ID
	DWORD dwClientID = GenerateClientID();




    
	// 生成玩家
	
	Player* pPlayer ;
	LoongWorld* pWorld=sLoongWorldMgr.GetLoongWorld(pCmd->dwWorldNameCrc);
	if(!P_VALID(pWorld))
	{
#ifdef  KOREA_LOG
		SPE_LOG->Write(_T("-----------------------------------!P_VALID(pWorld)  -----------------------------------\r\n"));
#endif
		g_login.UnlockUpdate();
		return GT_INVALID;
	}
    DWORD dwWorldID=pWorld->GetParentServerCrc();
	pPlayer	= new Player(dwClientID, pParam->dwHandle, pParam->dwAddress, dwWorldID, pCmd->szMac); 


	//Player* pPlayer	= new Player(dwClientID, pParam->dwHandle, pParam->dwAddress, pCmd->dwWorldNameCrc);
	
	if( !P_VALID(pPlayer) )
	{
		g_login.UnlockUpdate();
		return GT_INVALID;
	}

	// 将玩家加入到列表中
	AddPlayerToAll(pPlayer);
	// 将玩家加入到登陆玩家列表中
	AddPlayerToLogining(pPlayer);

	//ILOG->Write(_T("AddPlayerToLogining.client id is:%d\r\n"),dwClientID);

	// 释放Update
	g_login.UnlockUpdate();

    return dwClientID;
}

//-------------------------------------------------------------------------------
// 登出回调函数
//-------------------------------------------------------------------------------
DWORD PlayerMgr::LogoutCallBack(LPVOID p)
{
	// 锁住Update
	g_login.LockUpdate();

	// 得到ID
    DWORD dwClientID = (DWORD)p;

    // 在所有玩家中查找玩家
    Player* pPlayer = m_mapAllPlayer.Peek(dwClientID);
    if( P_VALID(pPlayer) )
    {
        // 设置此玩家断开连接标志
		pPlayer->SetConnLost();
    }

	// 释放Update
	g_login.UnlockUpdate();


	if ( EPP_Korea == m_ePolicy)
	{
		ProofPolicyKorea* pKorea = (ProofPolicyKorea*)m_pPolicy;
		pKorea->AddEvent(dwClientID,EVT_PlayerLoginOut,0,NULL);
#ifdef  KOREA_LOG		
		SPE_LOG->Write(_T("-----------------------------------delete proof data -----------------------------------\r\n"));		
#endif
	}

	

    return 0;
}

//---------------------------------------------------------------------------------
// 玩家登出
//---------------------------------------------------------------------------------
VOID PlayerMgr::PlayerLogout(Player* pPlayer)
{
	BOOL bLogoutFromDB = TRUE;		// 是否设置数据库中该玩家为离线

	if( !pPlayer->IsProofEnd() || GT_INVALID == pPlayer->GetAccountID() )	// 还没有验证成功，数据库根本就没有操作过
	{
		bLogoutFromDB = FALSE;
	}
	else if( pPlayer->IsSelectWorldOk() )	// world已经给了回应，说明马上就会登入到world上
	{
		bLogoutFromDB = FALSE;
	}

	// 如果需要设置数据库
	if( bLogoutFromDB )
	{
		sBeton.PlayerLogout(pPlayer->GetAccountID());
	}

	// 如果帐号合法，则移除帐号
	if( GT_INVALID != pPlayer->GetAccountID() )
	{
		RemoveAccount(pPlayer->GetAccountID());
	}

	// 从列表中删除该玩家
	RemovePlayerFromAll(pPlayer->GetClientID());

	// 删除玩家
	SAFE_DEL(pPlayer);
}

//------------------------------------------------------------------------------------
// 发送验证
//------------------------------------------------------------------------------------
VOID PlayerMgr::Proof(DWORD dwClientID, LPCSTR szAccoutName, LPCSTR szPsd, LPCSTR szGUID, DWORD dwIP,DWORD dwWorldNameCrc,DWORD dwChannelID, const CHAR *pszMac, LPCSTR szUserID/* = NULL*/,LPCSTR szAppKey/*  = NULL*/, LPCSTR szToken/* = NULL*/)
{
	if ( EPP_Korea == GetProofPolicy() )
	{
		if ( !P_VALID(szGUID) )
		{
			return ;
		}
	}
	else
	{

		if( !P_VALID(szAccoutName) || !P_VALID(szPsd) || !P_VALID(szGUID) )
			return;
	}
	

	// 发送给相应的验证策略进行验证
	m_pPolicy->Proof(dwClientID, szAccoutName, szPsd, szGUID, dwIP,dwWorldNameCrc,dwChannelID,pszMac,szUserID,szAppKey,szToken);
}

//--------------------------------------------------------------------------------------
// 验证回调函数
//--------------------------------------------------------------------------------------
VOID PlayerMgr::ProofCallBack(INT nRet, tagProofResult* pResult)
{
	tagProofResultFull* pResultFull = new tagProofResultFull;
	memcpy(pResultFull, pResult, sizeof(tagProofResult));
	pResultFull->nRet = nRet;

	m_listProofResult.PushBack(pResultFull);
}

VOID PlayerMgr::CacheAccountName( DWORD dwAccountID, const CHAR* szAccountName)
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);		
	if (!P_VALID(pAccountData))
	{
		pAccountData = new tagAccountData;
		m_mapAccountData.Add(dwAccountID, pAccountData);
	}
	memcpy(pAccountData->szAccountName, szAccountName, X_SHORT_NAME);
}

VOID PlayerMgr::CacheIpAddres( DWORD dwAccountID, DWORD dwIp)
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);		
	if (!P_VALID(pAccountData))
	{
		pAccountData = new tagAccountData;
		m_mapAccountData.Add(dwAccountID, pAccountData);
	}
	pAccountData->dwIp = dwIp;
}

VOID PlayerMgr::CacheGuard( DWORD dwAccountID, BOOL bGuard)
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);		
	if (!P_VALID(pAccountData))
	{
		pAccountData = new tagAccountData;
		m_mapAccountData.Add(dwAccountID, pAccountData);
	}
	pAccountData->bGuard = bGuard;
}

VOID PlayerMgr::EraseCachedAccountData( DWORD dwAccountID )
{
	tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);
	if (P_VALID(pAccountData))
	{
		SAFE_DEL(pAccountData);
		m_mapAccountData.Erase(dwAccountID);
	}
}

VOID PlayerMgr::CleanCachedAccountDatas()
{
	std::list<DWORD> listIDs;
	m_mapAccountData.ExportAllKey(listIDs);
	std::list<DWORD>::iterator itr = listIDs.begin();
	while(itr != listIDs.end())
	{
		EraseCachedAccountData(*itr);
		++itr;
	}
}

VOID PlayerMgr::MapAccountName2AccountID( LPCSTR szAccountName, DWORD dwAccountID )
{
	DWORD dwNameCrc = m_pUtil->Crc32(szAccountName);
	if (!GT_VALID(m_mapAccountNameCrc2AccountID.Peek(dwNameCrc)))
	{
		m_mapAccountNameCrc2AccountID.Add(dwNameCrc, dwAccountID);
	}
}

DWORD PlayerMgr::GetAccountIDByAccountName( LPCSTR szAccountName )
{
	DWORD dwNameCrc = m_pUtil->Crc32(szAccountName);
	return m_mapAccountNameCrc2AccountID.Peek(dwNameCrc);
}

VOID PlayerMgr::CacheMac( DWORD dwAccountID, LPCSTR szMac)
{
    tagAccountData* pAccountData = m_mapAccountData.Peek(dwAccountID);		
    if (!P_VALID(pAccountData))
    {
        pAccountData = new tagAccountData;
        m_mapAccountData.Add(dwAccountID, pAccountData);
    }
    memcpy(pAccountData->szMac, szMac, MAX_MAC_LENGTH);
} 

LPCSTR    PlayerMgr::RequestTokenID(DWORD dwConnID,string& strTokenID)
{
	if ( NULL != m_YAuth_genRequest )
	{

		strTokenID = m_YAuth_genRequest(dwConnID);
		return strTokenID.c_str();
	}
	else
	{
		ILOG->Write(_T("-----------------------Request Failed! dwConnID: %u-------------------\r\n"),dwConnID);  
		IMSG(_T("-----------------------Request Failed! dwConnID: %u-------------------\r\n"),dwConnID);
		return NULL;
	}
}

static char* ConvertUtf8ToGBK(LPCSTR strUtf8); 
static char* ConvertUtf8ToGBK(LPCSTR strUtf8) 
{
	int len=MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, NULL,0);
	unsigned short * wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	char *szGBK=new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL,NULL);
	delete [] wszGBK; 
	return szGBK;
} 

bool   PlayerMgr::VerifyToken(LPVOID pLoginCmdInfo,DWORD dwCDIndex)
{
	DWORD dwConnID = (DWORD)dwCDIndex;

	if ( NULL != m_pYAuth_verifyToken)
	{
		tagNLC_SafeProof* pLoginCMD =  (tagNLC_SafeProof*)pLoginCmdInfo;
		if ( pLoginCMD->dwReserved <= 0 || pLoginCMD->dwReserved >= 1024)
		{
#ifdef  YYLogin_LOG	
			ILOG->Write(_T("登入waring:字符串超出正常范围 dwReserved: %d\r\n"),(INT)pLoginCMD->dwReserved);
#endif
			return false;
		}

		PBYTE pByte = new BYTE[pLoginCMD->dwReserved+3];
		if( !P_VALID(pByte))
		{
			return false;
		}
		memset(pByte,0,pLoginCMD->dwReserved+3);
		CHAR* pLoginInfo = (CHAR*)pByte;
		memcpy(pLoginInfo,pLoginCMD->byData,pLoginCMD->dwReserved);
		*((char*)pByte+pLoginCMD->dwReserved+2) = '\0';
		int nRet = m_pYAuth_verifyToken(pLoginInfo,dwConnID) ;

		bool bSuc = false;
		if ( 1 == nRet )
		{
			bSuc = true;
		}

		string strGUID;
		if ( bSuc )
		{
			//ILOG->Write(_T("PlayerMgr::VerifyToken Begin /r/n"));
			char szBuff[1024] = {0}; 
			char szBuff_2[1024] = {0}; 
			//联系方式，姓名，证件号，需要十六进制转换，UID不需要。
			// 取得YY的uid
			m_pYAuth_GetAttribute( pLoginInfo, "UserID", szBuff, sizeof(szBuff));
			szBuff[1023] = '\0';
			strGUID += szBuff;
			strGUID += ',';

			//ILOG->Write(_T("PlayerMgr::VerifyToken1 %s /r/n"), strGUID.c_str());
			memset(szBuff,0,sizeof(szBuff));
			memset(szBuff_2,0,sizeof(szBuff_2));

			// 取得姓名
			m_pYAuth_GetAttribute(pLoginInfo, "IDCardName", szBuff, sizeof(szBuff) );
			szBuff[1023]	= '\0';
			m_pYAuth_HexToString( szBuff, szBuff_2, sizeof(szBuff_2) ); 
			szBuff_2[1023]	= '\0';
			char * p = ConvertUtf8ToGBK(szBuff_2); 
			strGUID += p;
			strGUID +=  ',';
			delete [] p; 

			//ILOG->Write(_T("PlayerMgr::VerifyToken2 %s /r/n"), strGUID.c_str());
			memset(szBuff,0,sizeof(szBuff));
			memset(szBuff_2,0,sizeof(szBuff_2));

			// 取证件
			m_pYAuth_GetAttribute( pLoginInfo, "IDCardNumber", szBuff, sizeof(szBuff) );
			szBuff[1023]	= '\0';
			m_pYAuth_HexToString( szBuff, szBuff_2, sizeof(szBuff_2) );
			szBuff_2[1023]  = '\0';
			strGUID += szBuff_2;
			strGUID +=  ',';

			//ILOG->Write(_T("PlayerMgr::VerifyToken3 %s /r/n"), strGUID.c_str());
			memset(szBuff,0,sizeof(szBuff));
			memset(szBuff_2,0,sizeof(szBuff_2));

			// 联系方式
			m_pYAuth_GetAttribute( pLoginInfo, "ContactInfo", szBuff, sizeof(szBuff) );
			szBuff[1023]    = '\0';
			m_pYAuth_HexToString( szBuff, szBuff_2, sizeof(szBuff_2) );
			szBuff_2[1023]  = '\0';
			strGUID +=  szBuff_2;
			strGUID +=  ',';
			

			//ILOG->Write(_T("PlayerMgr::VerifyToken4 %s /r/n"), strGUID.c_str());
			if ( strGUID.length() > X_USERNAME_LENEX)
			{
				ILOG->Write(_T("-----------------------YY 账号信息超过50个字符-------------------\r\n"),dwConnID);  
				IMSG(_T("-----------------------YY 账号信息超过50个字符-------------------\r\n"),dwConnID);
				SAFE_DEL_ARRAY(pByte);
				return false;
			}
		
			memset(pLoginCMD->szGUID,0,sizeof(pLoginCMD->szGUID));
			memcpy(pLoginCMD->szGUID,strGUID.c_str(),strGUID.length());

			//ILOG->Write(_T("PlayerMgr::VerifyToken End /r/n"));
		}

#ifdef  YYLogin_LOG	
		TCHAR accoutName[1024];
		memset(accoutName,0,sizeof(accoutName));
		MultiByteToWideChar(CP_ACP,NULL,strGUID.c_str(),-1,accoutName,strGUID.length()*2);
		accoutName[strGUID.length()*2+1] = _T('\0');

		ILOG->Write(_T("-----------------------------------Get User information -----------------------------------\r\n"));
		ILOG->Write(_T("szGUID: %s,bSuc: %d,ClientID: %d \r\n"),accoutName,(INT)bSuc,dwCDIndex);
#endif
		SAFE_DEL_ARRAY(pByte);

		//ILOG->Write(_T("PlayerMgr::VerifyToken End1 /r/n"));
		return bSuc;    
	}
	else
	{
		ILOG->Write(_T("-----------------------Request Failed! dwConnID: %u-------------------\r\n"),dwConnID);  
		IMSG(_T("-----------------------Request Failed! dwConnID: %u-------------------\r\n"),dwConnID);
		return false;    
	}

}

bool	PlayerMgr::GetWebLoginInfo(LPVOID pWebLoginInfo,DWORD& dwMsgSize)
{
	//ILOG->Write(_T("PlayerMgr::GetWebLoginInfo Start"));
	tagWebNoteInfo* pWebNoteInfo = NULL;
	m_MapURL.ResetIterator();
	PBYTE pByte = (PBYTE)pWebLoginInfo;
	DWORD dwNum = 0;
	dwMsgSize = 0;
	while( m_MapURL.PeekNext(pWebNoteInfo) )
	{
		if ( P_VALID(pWebNoteInfo) )
		{
			if ( dwNum > m_dwChannelNum)
			{
				break;
			}
			DWORD dwSize      = 0 ;
			dwSize			  = sizeof(DWORD)*2 + CROP_NAME_MAX_LEN*sizeof(TCHAR) + pWebNoteInfo->strURL.length()*sizeof(CHAR) +1;
			(*(DWORD*)pByte)  = dwSize;
			pByte			  = pByte + sizeof(DWORD);
			tagWeb_URLInfo* p = (tagWeb_URLInfo*)pByte;
			p->dwCropID       =  pWebNoteInfo->dwCropID;
			DWORD dwLen = pWebNoteInfo->tstrDescName.length();
			memcpy(p->szDescName,pWebNoteInfo->tstrDescName.c_str(),dwLen*sizeof(TCHAR));
			p->szDescName[dwLen] = _T('\0');
			p->dwSize		  = pWebNoteInfo->strURL.length()*sizeof(CHAR) +1;
			memcpy(p->szURL,pWebNoteInfo->strURL.c_str(),p->dwSize-1);
			p->szURL[p->dwSize-1] = '\0';
			pByte			  = pByte + dwSize;
			dwMsgSize += (dwSize + sizeof(DWORD)); 
		}
	}
	//ILOG->Write(_T("PlayerMgr::GetWebLoginInfo End"));
	return true;

}

bool	PlayerMgr::GetRechargeURL(OUT char* pLoginURLBuff,DWORD& dwLoginURLSize,OUT char* pBuff,DWORD& dwURLSize,DWORD dwCropID)
{
	//ILOG->Write(_T("PlayerMgr::GetRechargeURL Start"));
	tagWebNoteInfo* pWebNoteInfo	 = NULL;
	if ( !m_MapURL.IsExist( dwCropID ))
	{
		return false;
	}
	pWebNoteInfo = m_MapURL.Peek(dwCropID);
	if (!P_VALID( pWebNoteInfo) )
	{
		return false;
	}
	dwURLSize						 = pWebNoteInfo->strRechargeURL.length() + 1;
	dwLoginURLSize					 = pWebNoteInfo->strLoginURL.length() + 1;
	memcpy(pBuff,pWebNoteInfo->strRechargeURL.c_str(),pWebNoteInfo->strRechargeURL.length());
	memcpy(pLoginURLBuff,pWebNoteInfo->strLoginURL.c_str(),pWebNoteInfo->strLoginURL.length());
	pBuff[dwURLSize-1]				 = '\0';
	pLoginURLBuff[dwLoginURLSize-1] = '\0';
	//ILOG->Write(_T("PlayerMgr::GetRechargeURL End"));
	return true;
}