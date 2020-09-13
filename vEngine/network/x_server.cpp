//-----------------------------------------------------------------------------
//!\file x_server.cpp
//!\author Lyp
//!
//!\date 2004-04-03
//! last 2009-06-15
//!
//!\brief 服务器端传输底层
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "x_server.h"

#include "..\debug\exception.h"
#include "..\system\sysinfo.h"
#include "..\filter\FilterMgr.h"
#include "..\debug\debug.h"
#include "..\console\console_gui.h"

#pragma intrinsic(memcpy)

namespace vEngine {
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
XServer::XServer():m_Trunk(this)
{
	m_hCompletionPort = 0;
	m_lpfnAcceptEx = 0;
	m_lpfnGetAcceptExSockAddr = 0;
	m_bTerminateWorkThread = FALSE;
	m_Pool = NULL;
	m_nPoolNum = 0;
	m_nAcceptExs = 0;
	m_n64SendSize = 0;
	m_n64RealSendSize = 0;
	m_dwFreeClientLogTime = GetCurrentDWORDTime();
	m_dwUnauthClientLogTime = GetCurrentDWORDTime();
	m_dwDestroyClientLogTime = GetCurrentDWORDTime();

	m_hEventAccepEx = ::CreateEvent(NULL, FALSE, TRUE, NULL);	// 自动激活
	m_hEventSend = ::CreateEvent(NULL, FALSE, TRUE, NULL);		// 自动激活
	m_hEventDestroy = ::CreateEvent(NULL, FALSE, TRUE, NULL);	// 自动激活
	m_pFilter = CreateObj("XServerFilter", "FilterMgr");
	m_pFilter->InitFilter(FMF_MiniLzo);
	m_pUtil = "Util";
}


//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
XServer::~XServer()
{
	KillObj("XServerFilter");
	CloseHandle(m_hEventAccepEx);
	CloseHandle(m_hEventSend);
	CloseHandle(m_hEventDestroy);
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL XServer::Init(tagXServerConfig& param)
{
	// 生成log
	m_pLog = "Log";
	m_pLog->Create();

	ASSERT(param.fnLogIn && param.fnLogOut);
	memcpy(&m_Config, &param, sizeof(m_Config)); // 内部保存一份配置
	
	m_Client = new tagXClient[m_Config.nMaxServerLoad];	// 建立client data
	if( !m_Client )
		return FALSE;

	ZeroMemory(m_Client, sizeof(tagXClient)*m_Config.nMaxServerLoad);
	for(INT n=0; n<m_Config.nMaxServerLoad; n++)
	{
		m_Client[n].pRecvQueue = new XQueue<tagUnit*>;
		m_Client[n].pSendQueue = new XQueue<tagRawUnit*>;
		m_listFreeClient.PushBack(&m_Client[n]);
	}

	m_sockListen = socket(AF_INET, SOCK_STREAM, 0);	// 监听socket

	// reuse造成多次绑定成功，可能不能找到正确可用的端口
	// 如果两台服务器开在同一台机器会使用同一个端口，造成混乱
	if( m_Config.bReusePort )
	{
		DWORD dwReUseAddr = TRUE;	
		setsockopt( m_sockListen, SOL_SOCKET, SO_REUSEADDR, 
			(char*)&dwReUseAddr, sizeof(dwReUseAddr) );
	}

	// disables the NAGLE algorithm for send coalescing
	if( !m_Config.bNagle )
	{
		BOOL bNoDelay = TRUE;
		setsockopt(m_sockListen, IPPROTO_TCP, TCP_NODELAY, (char*)&bNoDelay, sizeof(bNoDelay));
	}

	// Jason 设置接收和发送缓冲区大小为原来的双倍
	if( 1 )
	{
		INT nSize = sizeof(INT);
		INT nValue = 0;
		getsockopt(m_sockListen,SOL_SOCKET,SO_RCVBUF,(char *)&nValue,&nSize);
		m_pLog->Write(_T("rcvbuf default size:%u\r\n"),nValue);
		nValue *= 2;
		if( setsockopt(m_sockListen,SOL_SOCKET,SO_RCVBUF,(const char *)&nValue,nSize) == SOCKET_ERROR )
			m_pLog->Write(_T("resize rcvbuf error:%u\r\n"),WSAGetLastError());
		else
			m_pLog->Write(_T("rcvbuf expand size:%u\r\n"),nValue);
		getsockopt(m_sockListen,SOL_SOCKET,SO_SNDBUF,(char *)&nValue,&nSize);
		m_pLog->Write(_T("sndbuf default size:%u\r\n"),nValue);
		nValue *= 2;
		if( setsockopt(m_sockListen,SOL_SOCKET,SO_SNDBUF,(const char *)&nValue,nSize) == SOCKET_ERROR )
			m_pLog->Write(_T("resize sndbuf size error:%u\r\n"),WSAGetLastError());
		else
			m_pLog->Write(_T("sndbuf expand size:%u\r\n"),nValue);
	}

	sockaddr_in address;
    ZeroMemory(&address, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port        = htons(m_Config.nPort);

    while( SOCKET_ERROR == bind(m_sockListen, (sockaddr*)&address, sizeof(address)) )
	{
		if( WSAEADDRINUSE == WSAGetLastError() )
		{
			++m_Config.nPort;	// 依次尝试后面的端口
			address.sin_port = htons(m_Config.nPort);
		}
		else
		{
			closesocket(m_sockListen);
			ERR(_T("bind() failed"));
			return FALSE;
		}
	}

    m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    listen( m_sockListen, SOMAXCONN );
	CreateIoCompletionPort((HANDLE)m_sockListen, m_hCompletionPort, (DWORD)0, 0);

	// 获得AcceptEx\GetAcceptExSockAddrs的函数指针，直接调用更加快捷
	GUID Guid = WSAID_ACCEPTEX, Guid2 = WSAID_GETACCEPTEXSOCKADDRS;
	DWORD dwBytes = 0;
	WSAIoctl(m_sockListen, SIO_GET_EXTENSION_FUNCTION_POINTER, &Guid,
		sizeof(GUID), &m_lpfnAcceptEx, sizeof(m_lpfnAcceptEx), &dwBytes, 0, 0);
	WSAIoctl(m_sockListen, SIO_GET_EXTENSION_FUNCTION_POINTER, &Guid2,
		sizeof(GUID), &m_lpfnGetAcceptExSockAddr,sizeof(m_lpfnGetAcceptExSockAddr), &dwBytes, 0, 0);

	// 按配置文件指定的数目建立工作线程
	INT nWorkerThread = (TObjRef<SysInfo>()->GetProcessorNum() * m_Config.nPerCpuWorkerThread) 
		+ m_Config.nAdditionWorkerThread;
	
	if( FALSE == CreateWorkThread(nWorkerThread) )	// 创建工作线程以及内存池
		return FALSE;
	
	if( FALSE == CreateSetupAccepExThread() )	// SetupAccpetEx线程
		return FALSE;

	if( FALSE == CreateSendThread() )	// send线程
		return FALSE;
	
	if( FALSE == CreateUnauthHeartbeatThread() )	// unauth heartbeat线程
		return FALSE;

	if( FALSE == CreateDestroyThread() )	// destroy线程
		return FALSE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL XServer::Destroy()
{
	SAFE_CLOSE_HANDLE(m_hCompletionPort);

	// 等待工作线程结束
	InterlockedExchange((LONG*)&m_bTerminateWorkThread, TRUE);
	for(INT n=(INT)m_vecThreadHandle.size()-1; n>=0; n--)
	{
		if( m_vecThreadHandle[n] )
		{
			WaitForSingleObject(m_vecThreadHandle[n], INFINITE);
			SAFE_CLOSE_HANDLE(m_vecThreadHandle[n]);
		}
	}

	for(INT n=0; n<m_Config.nMaxServerLoad; n++)	// 设置 index
	{
		if( !m_Client[n].bShutDown )
		{
			closesocket(m_Client[n].sock);
			// 如果在这里出assert说明还有上层未处理的网络消息残留
			// 在recv queue和resend queue		
			m_Client[n].bShutDown = TRUE;
		}
		tagUnit* pUnit = NULL;
		while( pUnit = m_Client[n].pRecvQueue->Get() )
			pUnit->pPool->Free(pUnit);

		tagRawUnit* pSendUnit = NULL;
		while( pSendUnit = m_Client[n].pSendQueue->Get() )
			pSendUnit->pPool->Free(pSendUnit);
		SAFE_DEL(m_Client[n].pRecvQueue);
		SAFE_DEL(m_Client[n].pSendQueue);
	}

	SAFE_DEL_ARRAY(m_Pool);
	SAFE_DEL_ARRAY(m_Client);
	m_pLog->Close();
	return TRUE;
}


//-----------------------------------------------------------------------------
// 踢出客户端
//-----------------------------------------------------------------------------
VOID XServer::Kick(DWORD dwHandle)
{ 
	PreDestroyClient((tagXClient*)dwHandle, NULL);
}

//-----------------------------------------------------------------------------
// setup acceptEx thread
//-----------------------------------------------------------------------------
BOOL XServer::CreateSetupAccepExThread()
{
	// 创建setup acceptEx线程
	INT nReturn = _beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&vEngine::XServer::ThreadSetupAcceptEx), 0, 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("[s]create setup acceptex thread failed"));
		return FALSE;
	}

	// 激活event
	::SetEvent(m_hEventAccepEx);
	m_vecThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}


//-----------------------------------------------------------------------------
// thread setup acceptEx
//-----------------------------------------------------------------------------
UINT XServer::ThreadSetupAcceptEx(LPVOID)
{
	tagXClient* pClient = NULL;
	BOOL bResult = FALSE;
	DWORD dwBytes = 0;

	while( FALSE == m_bTerminateWorkThread )
	{
		if (CalcTimeDiff(GetCurrentDWORDTime(), m_dwFreeClientLogTime) > CLIENT_LIST_LOG_TIME)
		{
			m_pLog->Write(_T("x_serve::m_listFreeClient Size=%d\r\n"), m_listFreeClient.Size());
			m_dwFreeClientLogTime = GetCurrentDWORDTime();
		}
		while( m_nAcceptExs < m_Config.nAcceptExNum 
			&& GT_VALID(pClient = m_listFreeClient.PopFront()) )
		{
			pClient->bShutDown = FALSE;		// 是否已经被shutdown
			pClient->sock = socket(AF_INET, SOCK_STREAM, 0);
			pClient->dwClientID = GT_INVALID;
			pClient->dwConnectTime = GT_INVALID;
			pClient->lSendCast = 0;
			pClient->lRecvCast = 1;
			pClient->dwSendSize = 0;
			pClient->dwKickTimeOut = 0;
			pClient->dwDestoryTimeOut = 0;
			
			tagUnit* pUnit = (tagUnit*)m_Pool[0].Alloc(UNIT_SIZE(PARAM_SIZE));
			pUnit->pPool = &m_Pool[0];
			pUnit->ov.hEvent = NULL;
			pUnit->eState = EXS_Accept;
			pUnit->pNext = NULL;
			((tagXLoginParam*)(&pUnit->pBuffer[PARAM_INDEX]))->dwHandle = (DWORD)pClient;
			
			bResult = m_lpfnAcceptEx(m_sockListen, pClient->sock, pUnit->pBuffer, 0, 
				sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, &dwBytes, (OVERLAPPED*)pUnit);//m_lpfnAcceptEx
			if( !bResult && ERROR_IO_PENDING != WSAGetLastError() )
			{
				DestroyClient(pClient);
				pUnit->pPool->Free(pUnit);
				continue;
			}

			InterlockedIncrement((LONG*)&m_nAcceptExs);
		}

		WaitForSingleObject(m_hEventAccepEx, X_SLEEP_TIME);
	}

	shutdown(m_sockListen, SD_BOTH);
	closesocket(m_sockListen);
	return 0;
}


//-----------------------------------------------------------------------------
// create worker thread
//-----------------------------------------------------------------------------
BOOL XServer::CreateWorkThread(INT nThreadNum)
{
	m_Pool = new SafeMemPool[nThreadNum+1];
	m_nPoolNum = nThreadNum+1;

    for(INT n=1; n<=nThreadNum; n++)
    {
		m_Pool[n].SetMaxSize(m_Config.dwPoolSize);

		// 创建工作线程
        INT nReturn = _beginthreadex(NULL, 0, (THREADPROC)
			m_Trunk.sfp1(&vEngine::XServer::ThreadWorker), (LPVOID)n, 0, NULL);
		if( 0 == nReturn || -1 == nReturn )
		{
			ERR(_T("[s]create worker thread failed"));
			return FALSE;
		}
		m_vecThreadHandle.push_back((HANDLE)nReturn);
    }

	return TRUE;
}


//-----------------------------------------------------------------------------
// work thread
//-----------------------------------------------------------------------------
UINT XServer::ThreadWorker(INT	nThreadIndex)
{
	INT nResult = 0;
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;
	BOOL bDontLinger = TRUE;
	sockaddr_in *pRemoteAddr, *pLocalAddr;
	DWORD dwRemoteAddrSize = 0, dwLocalAddrSize = 0;
	
	WSAOVERLAPPED*	pOverlapped = NULL;
	tagXClient* pClient = NULL;
	tagUnit* pUnitGot = NULL;
	tagUnit* pUnit = NULL;

	WSABUF wbuf;
	DWORD dwBytesNeed = 0;
	DWORD dwBytesComp = 0;
        
	while( !m_bTerminateWorkThread )
	{
		// 得到完成状态
		nResult = GetQueuedCompletionStatus(m_hCompletionPort, &dwBytesComp, (DWORD*)&pClient, &pOverlapped, X_SLEEP_TIME);
		if( !pOverlapped )
			continue;

		pUnitGot = CONTAINING_RECORD(pOverlapped, tagUnit, ov);
		if( EXS_Accept == pUnitGot->eState )	// accept过程
		{
			pClient = (tagXClient*)(((tagXLoginParam*)(&pUnitGot->pBuffer[PARAM_INDEX]))->dwHandle);
			InterlockedDecrement((LONG*)&m_nAcceptExs);	// 再创建AcceptEx
			SetEvent(m_hEventAccepEx);
		}

		if( !nResult )	// 完成端口收到失败通知
		{
			DWORD dwErrorCode = GetLastError();
			if (ERROR_NETNAME_DELETED != dwErrorCode && ERROR_OPERATION_ABORTED != dwErrorCode)
				m_pLog->Write(_T("kick r iocp err:%u\r\n"),dwErrorCode);

			switch(dwErrorCode)
			{
			case ERROR_IO_PENDING:
			case ERROR_IO_INCOMPLETE:
				break;
			default:
				PreDestroyClient(pClient, pUnitGot);
				break;
			}
			continue;
		}

		switch( pUnitGot->eState )
		{
		case EXS_Accept:
			setsockopt(pClient->sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&m_sockListen, sizeof(m_sockListen));
			setsockopt(pClient->sock, SOL_SOCKET, SO_DONTLINGER, (char*)&bDontLinger, sizeof(bDontLinger));
			m_lpfnGetAcceptExSockAddr(pUnitGot->pBuffer, 0, sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, 
				(sockaddr**)&pLocalAddr, (LPINT)&dwLocalAddrSize, (sockaddr**)&pRemoteAddr, (LPINT)&dwRemoteAddrSize);
			((tagXLoginParam*)(&pUnitGot->pBuffer[PARAM_INDEX]))->dwAddress = pRemoteAddr->sin_addr.S_un.S_addr;
			((tagXLoginParam*)(&pUnitGot->pBuffer[PARAM_INDEX]))->dwPort = pRemoteAddr->sin_port;

			if( !CreateIoCompletionPort((HANDLE)pClient->sock, m_hCompletionPort, (ULONG_PTR)pClient, 0) )	// 绑定完成端口
			{
				PreDestroyClient(pClient, pUnitGot); 
				break;
			}

			pClient->dwConnectTime = timeGetTime();
			m_listUnauthClient.PushBack(pClient);	// 未验证客户

			// 引发一个recv length重叠IO请求
			pUnit = (tagUnit*)m_Pool[nThreadIndex].Alloc(UNIT_SIZE(m_Config.dwMaxRecvSize));
			pUnit->pPool = &m_Pool[nThreadIndex];
			pUnit->ov.hEvent = NULL;
			pUnit->eState = EXS_RecvLength;
			pUnit->dwBytesReady = 0;
			pUnit->dwSize = sizeof(DWORD);
			pUnit->pNext = pUnitGot;

			wbuf.buf = pUnit->pBuffer;
			wbuf.len = pUnit->dwSize;
			if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags, &pUnit->ov, NULL) )
			{
				if( WSA_IO_PENDING != WSAGetLastError() )
					PreDestroyClient(pClient, pUnit);
			}	
			break;

		case EXS_RecvLength:	// 读入包长度
			if( dwBytesComp == 0 )
			{
				//m_pLog->Write(_T("kick iocp read:%u\r\n"),0);
				PreDestroyClient(pClient, pUnitGot);
				break;
			}

			dwBytesNeed = pUnitGot->dwSize - pUnitGot->dwBytesReady;
			pUnitGot->dwBytesReady += dwBytesComp;
			if( dwBytesComp == dwBytesNeed )
			{
				pUnitGot->dwBytesReady = 0;
				pUnitGot->dwSize = (*(DWORD*)&pUnitGot->pBuffer[0]);
				XCS_FIX_SIZE(pUnitGot->dwSize);		//	解除扰码
				if( pUnitGot->dwSize > m_Config.dwMaxRecvSize )	// 客户端发出超长的包
				{
					//CHAR temp[256];
					//sprintf_s(temp, "Kick player, package size too large. Account[%u] PackageSize[%u]", pClient->dwClientID, pUnitGot->dwSize);
					//OutputDebugStringA(temp);
					m_pLog->Write(_T("kick r package size large: Account[%u] PackageSize[%u]\r\n"),pClient->dwClientID, pUnitGot->dwSize);
					PreDestroyClient(pClient, pUnitGot);
					break;
				}
				pUnitGot->ov.hEvent = NULL;
				pUnitGot->eState = EXS_Recv;
				wbuf.buf = pUnitGot->pBuffer;
				wbuf.len = pUnitGot->dwSize;
				if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags, &pUnitGot->ov, NULL) )
				{
					int nerrono = WSAGetLastError() ;
					if( WSA_IO_PENDING != nerrono)
					{
						m_pLog->Write(_T("File:%s, Line:%d, kick r socket error: [%u]\r\n"),__FILE__,__LINE__,nerrono);
						PreDestroyClient(pClient, pUnitGot);
					}
				}	
			}
			else
			{
				pUnitGot->ov.hEvent = NULL;
				wbuf.buf = pUnitGot->pBuffer + pUnitGot->dwBytesReady;
				wbuf.len = pUnitGot->dwSize - pUnitGot->dwBytesReady;
				if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags, &pUnitGot->ov, NULL) )
				{
					int nerrono = WSAGetLastError() ;
					if( WSA_IO_PENDING != nerrono)
					{
						m_pLog->Write(_T("File:%s, Line:%d, kick r socket error: [%u]\r\n"),__FILE__,__LINE__,nerrono);
						PreDestroyClient(pClient, pUnitGot);
					}
				}		
			}
			break;

		case EXS_Recv:	
			if( dwBytesComp == 0 )
			{
				//m_pLog->Write(_T("kick read 0\r\n"));
				PreDestroyClient(pClient, pUnitGot);
				break;
			}

			dwBytesNeed = pUnitGot->dwSize - pUnitGot->dwBytesReady;
			pUnitGot->dwBytesReady += dwBytesComp;
			if( dwBytesComp == dwBytesNeed )	// 实际接收完成
			{
				if( pUnitGot->pNext )	// 玩家的第一个包
				{
					if( TRUE == m_listUnauthClient.Erase(pClient) )
					{
						// 收到第一个包,用户数目增加,通知外部,调用回调函数
						tagXLoginParam* pParam = (tagXLoginParam*)(&pUnitGot->pNext->pBuffer[PARAM_INDEX]);
						pClient->dwClientID = (m_Config.fnLogIn)(pUnitGot, pParam);
						if( GT_INVALID == pClient->dwClientID )	// 未通过身份验证,踢出
						{
							m_pLog->Write(_T("kick r client id invalid: -1\r\n"));
							PreDestroyClient(pClient, pUnitGot);
							break;
						}

						pUnitGot->pNext->pPool->Free(pUnitGot->pNext);
						pUnitGot->pNext = NULL;
					}
					else	// 用户已经超过验证时间
					{
						PreDestroyClient(pClient, pUnitGot);
						break;
					}
				}

				pClient->pRecvQueue->Add(pUnitGot);	// 将收到的加入接收缓冲

				pUnit = (tagUnit*)m_Pool[nThreadIndex].Alloc(UNIT_SIZE(m_Config.dwMaxRecvSize));
				pUnit->pPool = &m_Pool[nThreadIndex];
				pUnit->ov.hEvent = NULL;
				pUnit->eState = EXS_RecvLength;
				pUnit->dwSize = sizeof(DWORD);
				pUnit->dwBytesReady = 0;
				pUnit->pNext = NULL;

				wbuf.buf = pUnit->pBuffer;
				wbuf.len = pUnit->dwSize;
				if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags, &pUnit->ov, NULL) )
				{
					int nerrono = WSAGetLastError() ;
					if( WSA_IO_PENDING != nerrono)
					{
						m_pLog->Write(_T("File:%s, Line:%d, kick r socket error: [%u]\r\n"),__FILE__,__LINE__,nerrono);
						PreDestroyClient(pClient, pUnit);
					}
				}				
			}
			else
			{
				pUnitGot->ov.hEvent = NULL;
				wbuf.buf = pUnitGot->pBuffer + pUnitGot->dwBytesReady;
				wbuf.len = pUnitGot->dwSize - pUnitGot->dwBytesReady;
				if( 0 != WSARecv(pClient->sock, &wbuf, 1, &dwBytes, &dwFlags, &pUnitGot->ov, NULL) )
				{
					int nerrono = WSAGetLastError() ;
					if( WSA_IO_PENDING != nerrono)
					{
						m_pLog->Write(_T("File:%s, Line:%d, kick r socket error: [%u]\r\n"),__FILE__,__LINE__,nerrono);
						PreDestroyClient(pClient, pUnitGot);
					}
				}
			}
			break;

		case EXS_Send:	// 发送正常完成,交还unit到内存池
			pClient->dwSendSize -= pUnitGot->dwSize;
			pUnitGot->pPool->Free(pUnitGot);
			InterlockedDecrement(&pClient->lSendCast);
			if( pClient->bShutdownFriendly )
				PreDestroyClient(pClient, NULL);
			break;

		default:
			{
				//	...
				IMSG(_T("heat beat map !!!\r\n"));
				__asm int 3;
			}	break;	// 状态异常
		}
	}
	
	_endthreadex(0);
	return 0;
}


//-----------------------------------------------------------------------------
// create send thread
//-----------------------------------------------------------------------------
BOOL XServer::CreateSendThread()
{
    INT nReturn = _beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&vEngine::XServer::ThreadSend), NULL, 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("create send thread failed"));
		return FALSE;
    }

	m_vecThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}


//-----------------------------------------------------------------------------
// send thread
//-----------------------------------------------------------------------------
UINT XServer::ThreadSend(LPVOID)
{
	CHAR*		pBuff = new CHAR[m_Config.dwPerTickSendSize];
	tagRawUnit*	pRaw = NULL;
	tagUnit*	pUnit = NULL;
	DWORD		dwBytes = 0;
	DWORD		dwTotal = 0;
	WSABUF		wbuf;
	INT			nFakeRandom = ( timeGetTime() % (m_nPoolNum-1) ) + 1;
	tagXClient* pClient = NULL;
	tagFilterParam param;
	param.bCompress = TRUE;
	param.bSuccess = FALSE;
	param.eFilter = FMF_MiniLzo;
	param.lpMemIn = pBuff;
	param.dwInBufferSize = m_Config.dwPerTickSendSize;

	while( !m_bTerminateWorkThread )
	{
		for(INT n=0; n<m_Config.nMaxServerLoad; ++n)	// 注意内部的break
		{
			pClient = &m_Client[n];
			if( 0 == pClient->pSendQueue->GetNum() )
				continue;

			if( pClient->bShutDown )
			{
				while( NULL != (pRaw = pClient->pSendQueue->Get()) )
				{
					//pRaw = pClient->pSendQueue->Get();
					InterlockedDecrement(&pClient->lSendCast);
					pRaw->pPool->Free(pRaw);
				}
				continue;
			}
			// 加入水位限制，当客户端有超过MAX_SENDCAST个包未回复服务器的时候，不再给客户端发送消息
			if (pClient->lSendCast >= MAX_SENDCAST)
			{
				if (CalcTimeDiff(GetCurrentDWORDTime(), pClient->dwKickTimeOut) > KICK_TIME_OUT)
				{
					m_pLog->Write(_T("x_server::Kick Time Out Client!!!ClientID=%u,SendCast=%d,RecvCast=%d,bShutDown=%d,SendQueueSize=%d,\r\n"),
						pClient->dwClientID,pClient->lSendCast, pClient->lRecvCast, pClient->bShutDown, pClient->pSendQueue->GetNum());
					PreDestroyClient(pClient, NULL);
				}
				continue;
			}
			else
			{
				pClient->dwKickTimeOut = GetCurrentDWORDTime();
			}
			InterlockedIncrement(&pClient->lSendCast);

			// 拼合
			dwTotal = 0;
			while( pClient->pSendQueue->GetNum() > 0 )
			{
				pRaw = pClient->pSendQueue->Get();
				if(pRaw->dwSize + dwTotal >= m_Config.dwPerTickSendSize)
				{
					pClient->pSendQueue->AddFront(pRaw);
					break;
				}
				if( pRaw->dwSize > 0 )
				{
					pClient->dwSendSize -= pRaw->dwSize;
					memcpy(pBuff + dwTotal, pRaw->pBuffer, pRaw->dwSize);
					dwTotal += pRaw->dwSize;
				}
				else if( 0 == pRaw->dwSize ) // Jason DisconnectFriendly 的要求，2010-10-9
				{
					::InterlockedExchange((LPLONG)&pClient->bShutdownFriendly ,TRUE);
				}
				pRaw->pPool->Free(pRaw);
			}

			if( dwTotal == 0 )	// 空包？
			{
				InterlockedDecrement(&pClient->lSendCast);
				if( pClient->bShutdownFriendly )
					PreDestroyClient(pClient, NULL);
				continue;
			}

			// 寻找可以进入锁定的内存池
			for( pUnit=0;!pUnit; ) 
			{
				if( ++nFakeRandom >= m_nPoolNum )
					nFakeRandom = 1;
				pUnit = (tagUnit*)m_Pool[nFakeRandom].TryAlloc(UNIT_SIZE(dwTotal)+LZO_ADD(dwTotal)+sizeof(DWORD)*2);
			} 

			// 压缩
			param.dwInDataSize = dwTotal;
			param.dwOutBufferSize = dwTotal + LZO_ADD(dwTotal);
			param.dwOutDataSize = 0;
			param.lpMemOut = pUnit->pBuffer + sizeof(DWORD) * 2;
			m_pFilter->Filter(&param);

			// 在包头填写大小和压缩前大小
			*(DWORD*)pUnit->pBuffer = param.dwOutDataSize;
			*(DWORD*)(pUnit->pBuffer + sizeof(DWORD)) = dwTotal;
			pUnit->dwSize = param.dwOutDataSize + sizeof(DWORD) * 2;
			pUnit->ov.hEvent = NULL;
			pUnit->dwBytesReady = 0;
			pUnit->pPool = &m_Pool[nFakeRandom];
			pUnit->eState = EXS_Send;
			pUnit->pNext = NULL;

			pClient->dwSendSize += pUnit->dwSize;

			wbuf.buf = pUnit->pBuffer;
			wbuf.len = pUnit->dwSize;
			if( WSASend(pClient->sock, &wbuf, 1, &dwBytes, 0, &pUnit->ov, NULL) )
			{
				int nerrono = WSAGetLastError() ;
				if( WSA_IO_PENDING != nerrono )
				{
					m_pLog->Write(_T("kick s socket error: [%u], send size %u,SendCast=%u,RecvCast=%u\r\n"),nerrono,pUnit->dwSize,pClient->lSendCast, pClient->lRecvCast);
					PreDestroyClient(pClient, pUnit);
					break;
				}
			}
			
			m_n64SendSize += dwTotal + sizeof(DWORD) * 2; 
			m_n64RealSendSize += param.dwOutDataSize + sizeof(DWORD);
		}
		WaitForSingleObject(m_hEventSend, X_SLEEP_TIME);
	}
	
	SAFE_DEL_ARRAY(pBuff);
	_endthreadex(0);
	return 0;
}


//-----------------------------------------------------------------------------
// 创建踢出未认证客户端线程
//-----------------------------------------------------------------------------
BOOL XServer::CreateUnauthHeartbeatThread()
{
	INT nReturn = _beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&vEngine::XServer::ThreadUnauthHeartbeat), (LPVOID)(m_Config.dwAuthWaitTime), 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("[s]create heart beat to unauth client thread failed\r\n"));
		return FALSE;
	}

	m_vecThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}


//-----------------------------------------------------------------------------
// 踢出未认证客户端线程
//-----------------------------------------------------------------------------
UINT XServer::ThreadUnauthHeartbeat(DWORD dwWaitForAuthTime)
{
	tagXClient* pClient = NULL;

	while( !m_bTerminateWorkThread )
	{
		if (CalcTimeDiff(GetCurrentDWORDTime(), m_dwUnauthClientLogTime) > CLIENT_LIST_LOG_TIME)
		{
			m_pLog->Write(_T("x_serve::m_dwUnauthClientLogTime Size=%d\r\n"), m_listUnauthClient.Size());
			m_dwUnauthClientLogTime = GetCurrentDWORDTime();
		}

		if( m_listUnauthClient.Size() > 0 )
		{
			DWORD dwTime = timeGetTime();
			m_listUnauthClient.Lock();
			m_listUnauthClient._ResetIterator();
			while( m_listUnauthClient._Peek(pClient) )
			{
				if( dwTime - pClient->dwConnectTime > dwWaitForAuthTime )
				{
					m_pLog->Write(_T("kick unauth client\r\n "));
					PreDestroyClient(pClient, NULL);
					m_listUnauthClient._EraseCurrent();
					break;
				}
				m_listUnauthClient._AddIterator();
			}
			m_listUnauthClient.Unlock();
		}
		
		Sleep(X_SLEEP_TIME);
	}

	_endthreadex(0);
	return 0;
}


//-----------------------------------------------------------------------------
// 回收资源
//-----------------------------------------------------------------------------
VOID XServer::PreDestroyClient(tagXClient* pClient, tagUnit* pUnit)
{
	if( FALSE == InterlockedCompareExchange((LPLONG)(&pClient->bShutDown), TRUE, FALSE) )
	{
		shutdown(pClient->sock, SD_BOTH);

		if( GT_VALID(pClient->dwClientID) )
			(m_Config.fnLogOut)(pClient->dwClientID);	// 回调用户登出函数
			
		m_listDestroyClient.PushBack(pClient);
		SetEvent(m_hEventDestroy);

		closesocket(pClient->sock);
	}

	if( pUnit )
	{
		if( EXS_Send == pUnit->eState )	// 处理引用计数
			InterlockedDecrement(&pClient->lSendCast);
		else
			InterlockedDecrement(&pClient->lRecvCast); 

		if( pUnit->pNext )	// 释放unit
			pUnit->pNext->pPool->Free(pUnit->pNext);
		pUnit->pPool->Free(pUnit);
	}
}


//-----------------------------------------------------------------------------
// 创建资源回收线程
//-----------------------------------------------------------------------------
BOOL XServer::CreateDestroyThread()
{
	INT nReturn = _beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&vEngine::XServer::ThreadDestroy), 0, 0, NULL);
	if( 0 == nReturn || -1 == nReturn )
	{
		ERR(_T("[s]create destroy client thread failed"));
		return FALSE;
	}

	m_vecThreadHandle.push_back((HANDLE)nReturn);
	return TRUE;
}


//-----------------------------------------------------------------------------
// 资源回收线程
//-----------------------------------------------------------------------------
UINT XServer::ThreadDestroy(LPVOID)
{
	tagXClient*	pClient = NULL;

	while( !m_bTerminateWorkThread )
	{
		if (CalcTimeDiff(GetCurrentDWORDTime(), m_dwDestroyClientLogTime) > CLIENT_LIST_LOG_TIME)
		{
			m_pLog->Write(_T("x_serve::m_dwDestroyClientLogTime Size=%d\r\n"), m_listDestroyClient.Size());
			m_dwDestroyClientLogTime = GetCurrentDWORDTime();
		}

		if( !GT_VALID(pClient = m_listDestroyClient.PopFront()) )
		{
			WaitForSingleObject(m_hEventDestroy, X_SLEEP_TIME);
			continue;
		}

		if (pClient->dwDestoryTimeOut == 0)
			pClient->dwDestoryTimeOut = GetCurrentDWORDTime();

		BOOL bDestoryTimeOut = (CalcTimeDiff(GetCurrentDWORDTime(), pClient->dwDestoryTimeOut) > DESTORY_LIST_TIME_OUT)? TRUE:FALSE;
		if (bDestoryTimeOut)
			m_pLog->Write(_T("x_serve::kick Destory List Timeout Client!! SendCast=%u,RecvCast=%u\r\n"), pClient->lSendCast, pClient->lRecvCast);
		if((0 == pClient->lSendCast && 0 == pClient->lRecvCast) ||	bDestoryTimeOut)
		{
			DestroyClient(pClient);	// 真正清除并且回收
		}
		else
		{
			WaitForSingleObject(m_hEventDestroy, X_SLEEP_TIME);
			m_listDestroyClient.PushBack(pClient);
		}
	}

	_endthreadex(0);
	return 0;
}


//-----------------------------------------------------------------------------
// 回收资源
//-----------------------------------------------------------------------------
VOID XServer::DestroyClient(tagXClient* pClient)
{
	m_listUnauthClient.Erase(pClient);

	// 将我们收到的所有包都丢弃
	tagUnit* pUnit = NULL;
	while( pUnit = pClient->pRecvQueue->Get() )
		pUnit->pPool->Free(pUnit);

	tagRawUnit* pSendUnit = NULL;
	while( pSendUnit = pClient->pSendQueue->Get() )
		pSendUnit->pPool->Free(pSendUnit);
	
	pClient->bShutDown = TRUE;
	pClient->lSendCast = 0;
	pClient->lRecvCast = 0;
	pClient->dwSendSize = 0;
	pClient->sock = INVALID_SOCKET;
	pClient->dwClientID = GT_INVALID;	// 客户端ID,上层确认第一个包以后才有效
	pClient->dwKickTimeOut = 0;
	pClient->dwDestoryTimeOut = 0;
		
	// 收回client data
	m_listFreeClient.PushBack(pClient);
	
}

VOID XServer::DisconnectFriendly(DWORD dwHandle)
{
	tagXClient * pClientAgent = (tagXClient*)dwHandle;
	if( P_VALID(pClientAgent) && !pClientAgent->bShutDown )
	{
		INT n = (dwHandle%(m_nPoolNum-1))+1;
		tagRawUnit* pSendUnit=0;
		for(;!pSendUnit;)
		{
			if( ++n >= m_nPoolNum )
				n = 1;
			pSendUnit = (tagRawUnit*)m_Pool[n].TryAlloc(sizeof(tagRawUnit));
		}
		pSendUnit->dwSize = 0;
		pSendUnit->pPool = &m_Pool[n];
		pClientAgent->pSendQueue->Add(pSendUnit);
	}
}


} // namespace vEngine {
