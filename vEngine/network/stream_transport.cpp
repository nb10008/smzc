//-----------------------------------------------------------------------------
//!\file stream_transport.cpp
//!\author Lyp
//!
//!\date 2003-03-17
//! last 2008-04-17
//!
//!\brief 广域网客户端通讯底层
/*
  in http://tangentsoft.net/wskfaq/newbie.html#howclose 

	The proper sequence for closing a TCP connection is:
	1.Finish sending data. 
	2.Call shutdown() with the how parameter set to 1. 
	3.Loop on recv() until it returns 0. 
	4.Call closesocket(). 

  in MSDN:
	To assure that all data is sent and received on a connected socket 
	before it is closed, an application should use shutdown to close connection 
	before calling closesocket. For example, to initiate a graceful disconnect:
	1.Call WSAAsyncSelect to register for FD_CLOSE notification. 
	2.Call shutdown with how=SD_SEND. 
	3.When FD_CLOSE received, call recv until zero returned, or SOCKET_ERROR. 
	4.Call closesocket. 
*/
//-----------------------------------------------------------------------------
#include "..\stdafx.h"
#include "stream_transport.h"
#include "..\console\console_gui.h"

// 使用其它部件
#include "..\frame\msg_queue_ts.h"
#include "..\memory\mem_pool.h"
#include   <openssl/crypto.h> 
#include   <openssl/x509.h> 
#include   <openssl/pem.h> 
#include   <openssl/ssl.h> 
#include   <openssl/err.h> 
#include   <openssl/bio.h> 

#pragma  comment(lib, "ssleay32.lib")
#pragma  comment(lib, "libeay32.lib")

#define ONE_SECOND			1000
#define TEMP_BUFF			512*1024


namespace vEngine {
//-----------------------------------------------------------------------------
// construction / destruction
//-----------------------------------------------------------------------------
StreamTransport::StreamTransport():m_Trunk(this)
{
	m_bBigEndian = FALSE;	// 默认Little Endian
	Clear();
	m_pMemPool = new MemPool(TEMP_BUFF);
}

StreamTransport::~StreamTransport()
{
	this->Destroy();
	SAFE_DEL(m_pMemPool);
}


//-----------------------------------------------------------------------------
// clear
//-----------------------------------------------------------------------------
VOID StreamTransport::Clear()
{
	m_pSendMsgQueue = NULL;
	m_pRecvMsgQueue = NULL;

	m_strIP.clear();
	m_nPort = 0;
	m_Sock = 0;
	m_bNagle = TRUE;

	m_hConnect = 0;
	m_hSend = 0;
	m_hReceive = 0;

	::InterlockedExchange((LONG*)&m_bTerminateConnect, FALSE);
	::InterlockedExchange((LONG*)&m_bTerminateSend, FALSE);
	::InterlockedExchange((LONG*)&m_bTerminateReceive, FALSE);
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);

	m_Trunk.sfp_clear();
}



//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL StreamTransport::Init(BOOL bNagle, EStreamTransportMode eMode,
						   BOOL bSSL, BOOL bSSLVerify, CHAR* pCertName, CHAR* pKeyName, CHAR* pCACertName, CHAR* pKeyPasswd)
{
	this->Clear();

	m_bNagle = bNagle;
	m_eTransportMode = eMode;
	
	// 申请发送接受缓冲
	if( ESTM_Package == m_eTransportMode )
		m_pSendMsgQueue = new MsgQueueTS(TRUE, TRUE);
	else
		m_pSendMsgQueue = new MsgQueueTS(TRUE, FALSE);

	m_pRecvMsgQueue = new MsgQueueTS(FALSE, FALSE);
	if( NULL == m_pSendMsgQueue || NULL == m_pRecvMsgQueue )
	{
		SAFE_DEL(m_pSendMsgQueue);
		SAFE_DEL(m_pRecvMsgQueue);
		return FALSE;
	}

	m_bSSL = bSSL;

	if (TRUE == m_bSSL)
	{
		constructor();

		m_pCtx = SSL_CTX_new(SSLv3_client_method());
		m_pBIO = BIO_new_ssl_connect(m_pCtx);
		if (m_pBIO == NULL)
		{
			SSL_CTX_free(m_pCtx);
			return FALSE;
		}

		BIO_get_ssl(m_pBIO, &m_pSSL);
		SSL_set_mode(m_pSSL, SSL_MODE_AUTO_RETRY);
	}
	else
	{
		//创建socket
		m_Sock = socket(AF_INET, SOCK_STREAM, 0);
		if( INVALID_SOCKET == m_Sock )
		{
			IMSG(_T("could not create socket[%u]\r\n"),WSAGetLastError());
			SAFE_DEL(m_pSendMsgQueue);
			SAFE_DEL(m_pRecvMsgQueue);
			return FALSE;
		}

		// 使用非阻塞方式,linger行为
		DWORD dwCmd = 1;
		INT nResult = ioctlsocket( m_Sock, FIONBIO, &dwCmd );
		if( SOCKET_ERROR == nResult )
		{
			IMSG(_T("set nonblocking mode failed in init client\r\n"));
			closesocket( m_Sock );
			m_Sock = 0;
			SAFE_DEL(m_pSendMsgQueue);
			SAFE_DEL(m_pRecvMsgQueue);
			return FALSE;
		}

		// disables the Nagle algorithm for send coalescing
		if( !m_bNagle )
		{
			BOOL bNoDelay = TRUE;
			setsockopt(m_Sock, IPPROTO_TCP, TCP_NODELAY, (char*)&bNoDelay, sizeof(bNoDelay));
		}

		// set addr reusabel
		DWORD dwReUseAddr = TRUE;
		setsockopt( m_Sock, SOL_SOCKET, SO_REUSEADDR, 
			(char*)&dwReUseAddr, sizeof(dwReUseAddr) );
	}



	// hard shut down
	linger lin;
	lin.l_linger = 0;
	lin.l_onoff = 1;
	if (FALSE == m_bSSL)
	{
		setsockopt( m_Sock, SOL_SOCKET, SO_LINGER, (char*)&lin, sizeof(lin) );
	}

	// 每秒流量统计
	m_dwLastStatOutTime = timeGetTime();
	m_dwLastStatInTime = timeGetTime();
	m_dwByteOut = 0;
	m_dwByteIn = 0;
	m_dwByteOutCounter = 0;
	m_dwByteInCounter = 0;

	m_dwSendPackNum = 0;
	m_dwRecvPackNum = 0;

	return TRUE;
}


//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
void StreamTransport::Destroy()
{
	// 设置结束线程标志
	::InterlockedExchange((LONG*)&m_bTerminateConnect, TRUE);
	::InterlockedExchange((LONG*)&m_bTerminateSend, TRUE);
	::InterlockedExchange((LONG*)&m_bTerminateReceive, TRUE);

	// 等待线程结束,一直等下去
	GracefulCloseThread(m_hConnect, INFINITE);
	GracefulCloseThread(m_hSend, INFINITE);
	GracefulCloseThread(m_hReceive, INFINITE);

	// 完美关闭socket
	if( TRUE == m_bConnected )
	{
		GracefulCloseSocket(m_Sock, 20);	// 重试 20 次
	}
	else
	{
		// m_bConnected尚未为真之前极短时间内可能已经连上
		// 所以先shutdown一下，并无副作用
		if( m_Sock )
		{
			shutdown(m_Sock, SD_BOTH);// SD_SEND原来是这个，不过，既然都连不上了，而且下面用的是closesocket，就把他们都关了吧，之前有垃圾数据，也没有办法了
			closesocket(m_Sock);
			m_Sock = 0;
		}
	}

	// 关闭缓冲
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
	SAFE_DEL( m_pSendMsgQueue );
	SAFE_DEL( m_pRecvMsgQueue );
}



//-----------------------------------------------------------------------------
// 完美关闭socket
//-----------------------------------------------------------------------------
BOOL StreamTransport::GracefulCloseSocket(SOCKET& socket, INT nRetryTimes)
{
	if( 0 == socket )
		return TRUE;

	shutdown(socket, SD_SEND);
	
	INT nRetry = 0;
	fd_set fdread;
	timeval block_time;	// 设置阻塞时间
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;	// 150 ms 
	
	while( nRetry++ < nRetryTimes )
	{
		FD_ZERO(&fdread);
		FD_SET(socket, &fdread);
		if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
		{
			CHAR chTemp;
			INT nResult;
			if (TRUE == m_bSSL)
			{
				nResult = SSL_read(m_pSSL, &chTemp, 1);
			}
			else
			{
				nResult = recv(socket, &chTemp, 1, 0);
			}
			if( 0 == nResult )	// receive FIN, ok
			{
				if (TRUE == m_bSSL)
				{
					BIO_free_all(m_pBIO);
					SSL_CTX_free(m_pCtx);
				}
				else
				{
					closesocket( socket );
				}
				socket = 0;
				return TRUE;
			}
			
			if( SOCKET_ERROR == nResult )	// network down
				break;
		}
	}
	
	if (TRUE == m_bSSL)
	{
		BIO_free_all(m_pBIO);
		SSL_CTX_free(m_pCtx);
	}
	else
	{
		closesocket( socket );
	}
	socket = 0;
	return FALSE;
}


//-----------------------------------------------------------------------------
// 完美结束thread
//-----------------------------------------------------------------------------
BOOL StreamTransport::GracefulCloseThread(HANDLE& thread, INT nRetryTime)
{
	if( 0 == thread )
		return TRUE;

	DWORD dwResult = WaitForSingleObject(thread, nRetryTime);
	SAFE_CLOSE_HANDLE(thread);
	thread = 0;

	if( WAIT_TIMEOUT == dwResult )
		return FALSE;
	else
		return TRUE;
}

void StreamTransport::constructor()
{
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();
}



//-----------------------------------------------------------------------------
// 发送网络信息
//-----------------------------------------------------------------------------
BOOL StreamTransport::Send(LPVOID pMsg, DWORD dwMsgSize)
{
	ASSERT(NULL != pMsg);
	ASSERT(dwMsgSize > 0 && dwMsgSize < 0xFFFFFF);	// 数据过分长
	// 将发送信息添加到发送信息缓冲
	if( FALSE == m_bConnected )
	{
		IMSG(_T("connection to streamserver lost, send failed.\r\n"));
		return FALSE;
	}

	//初始化信息统计
	m_dwSendPackNum++;
	return m_pSendMsgQueue->AddMsg((LPBYTE)pMsg, dwMsgSize);
}



//-----------------------------------------------------------------------------
//! 接受网络信息,返回FALSE说明没有消息
//! 外部需要负责释放返回的内存指针
//-----------------------------------------------------------------------------
LPBYTE StreamTransport::Recv(DWORD& dwMsgSize)
{
	if( !P_VALID(m_pRecvMsgQueue) )
		return NULL;

	// 从接受缓冲接收消息
	return m_pRecvMsgQueue->GetMsg(dwMsgSize);
}


//-----------------------------------------------------------------------------
// 对应于Recv(),因为内存是内部分配的，所以应该交还内部来释放
//-----------------------------------------------------------------------------
VOID StreamTransport::FreeRecved(LPBYTE pMsg)
{
	if( m_pRecvMsgQueue )
		m_pRecvMsgQueue->FreeMsg(pMsg);
}


//-----------------------------------------------------------------------------
// 设置包模式下SIZE头的字节序
//-----------------------------------------------------------------------------
BOOL StreamTransport::SetPackageSizeEndian(BOOL bBigEndian)
{
	if( ESTM_Package != m_eTransportMode )
		return FALSE;

	m_bBigEndian = bBigEndian;
	return TRUE;
}



//-----------------------------------------------------------------------------
// 尝试连接指定地址
//-----------------------------------------------------------------------------
VOID StreamTransport::TryToConnect(LPCSTR szIP, INT nPort)
{
	this->Disconnect();
	
	m_strIP = szIP;
	m_nPort = nPort;

	//IMSG(_T("connecting.\r\n"));
	// 启动线程 connect
	if (TRUE == m_bSSL)
	{
		m_hConnect = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
			(&vEngine::StreamTransport::ThreadSSLConnect), NULL, 0, NULL);
	}
	else
	{
		m_hConnect = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
			(&vEngine::StreamTransport::ThreadConnect), NULL, 0, NULL);
	}

	INT i = 0;
	do 
	{
		if( ::WaitForSingleObject( m_hConnect, 110 ) == WAIT_OBJECT_0 )
			break;
	} while (++i < 10);
	//while(::WaitForSingleObject( m_hConnect, 0 ) != WAIT_OBJECT_0)
	//	continue;
}


//-----------------------------------------------------------------------------
//! 返回是否正在尝试连接
//! 假如所有设置正确，但是远程主机并没有启动服务器，那么就会一直尝试连接
//! 但是如果网络发生错误，就有可能停止尝试连接
//-----------------------------------------------------------------------------
BOOL StreamTransport::IsTryingToConnect()
{
	if( !m_hConnect )
		return FALSE;

	DWORD dwConnectExitCode = STILL_ACTIVE;
	GetExitCodeThread(m_hConnect, &dwConnectExitCode);
	if( STILL_ACTIVE == dwConnectExitCode )
		return TRUE;
	else
		return FALSE;
}


//-----------------------------------------------------------------------------
// 放弃尝试连接
//-----------------------------------------------------------------------------
VOID StreamTransport::StopTryingConnect()
{
	// 如果我们已经连接上了，那么也会断开
	this->Destroy();
	this->Init(m_bNagle, m_eTransportMode);
}

//-----------------------------------------------------------------------------
// 正常断开连接
//-----------------------------------------------------------------------------
VOID StreamTransport::Disconnect()
{
	this->StopTryingConnect();
}


//-----------------------------------------------------------------------------
// 得到未发送包的数量，可以以此来判断网络负荷
//-----------------------------------------------------------------------------
INT	StreamTransport::GetUnsendPackageNum()
{
	if( !m_bConnected )
		return -1;

	return m_pSendMsgQueue->GetMsgNum();
}


//-----------------------------------------------------------------------------
// 得到已接受包的数量，可以以此来判断网络负荷
//-----------------------------------------------------------------------------
INT	StreamTransport::GetReceivedPackageNum()
{
	if( !m_bConnected )
		return -1;

	return m_pRecvMsgQueue->GetMsgNum();
}

UINT StreamTransport::ThreadSSLConnect(LPVOID)
{
	char szBuf[512];
	memset(szBuf, 0, sizeof(szBuf));
	sprintf(szBuf, "%s:%d", m_strIP.c_str(), m_nPort);
	BIO_set_conn_hostname(m_pBIO, szBuf);
	if (BIO_do_connect(m_pBIO) != 1)
	{
		goto __connected_terminated;
	}

	if (BIO_do_handshake(m_pBIO) != 1)
	{
		goto __connected_terminated;
	}

	::InterlockedExchange((LONG*)&m_bConnected, TRUE);
	::Sleep(0);
	_endthreadex(0);
	return 0;

__connected_terminated:
	// 连接失败
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
	::Sleep(0);
	_endthreadex(0);
	return 0;
}

//-----------------------------------------------------------------------------
// thread connect
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadConnect(LPVOID)
{
	INT nResult = 0;
	sockaddr_in address;
	ZeroMemory(&address, sizeof(address));
	address.sin_addr.s_addr = inet_addr(m_strIP.c_str());
	address.sin_family = AF_INET;
	address.sin_port = htons(m_nPort);

	// 检查IP是否合法
	if( INADDR_NONE == address.sin_addr.s_addr )
	{
		IMSG(_T("[c] invalid IP\r\n"));
		goto __connected_terminated;
	}
	
	fd_set fdwrite;
	timeval block_time;	// 设置阻塞时间
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;
	
	// 连接到服务器
	// For connection-oriented, nonblocking sockets, it is often not possible 
	// to complete the connection immediately. In such a case, "connect"
	// returns the error WSAEWOULDBLOCK. However, the operation proceeds.
	nResult = connect(m_Sock, (sockaddr*)&(address), sizeof(address));
	if( SOCKET_ERROR == nResult )
	{	
		if( WSAEWOULDBLOCK == WSAGetLastError() )
		{
			for(INT n=0; n<64; n++) // 不停重试,外部可以通过调用StopTryingConnect停止重试
			{
				FD_ZERO(&fdwrite);
				FD_SET(m_Sock, &fdwrite);

				block_time.tv_sec = 0;
				block_time.tv_usec = GT_NET_BLOCK_TIME;
				if( 1 == select(0, NULL, &fdwrite, NULL, &block_time) )
					goto __connected;

				if( m_bTerminateConnect )
					goto __connected_terminated;

				Sleep(100);
			}
		}

		// 连接尝试失败
		goto __connected_terminated;
	}
	
__connected:

#ifdef KOREA_LOG
	IMSG(_T("Contected one Server %d. IP: %s\r\n"), m_nPort,m_strIP.c_str());
#endif
	//初始化信息统计
	m_dwSendPackNum = 0;
	m_dwRecvPackNum = 0; 

	// 启动线程 send
	m_hSend = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
		(&vEngine::StreamTransport::ThreadSend), NULL, 0, NULL);

	// 启动线程 receive
	switch( m_eTransportMode )
	{
	case ESTM_Package:
		m_hReceive = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
			(&vEngine::StreamTransport::ThreadReceive), NULL, 0, NULL);
		break;
	case ESTM_Stream:
	case ESTM_Text:
		if (TRUE == m_bSSL)
		{
			m_hReceive = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
				(&vEngine::StreamTransport::ThreadSSLStreamReceive), NULL, 0, NULL);
		}
		else
		{
			m_hReceive = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)m_Trunk.sfp1
				(&vEngine::StreamTransport::ThreadStreamReceive), NULL, 0, NULL);
		}
		break;
	}

	// 连接成功
	::InterlockedExchange((LONG*)&m_bConnected, TRUE);
	::Sleep(0);
	_endthreadex(0);
	return 0;

__connected_terminated:
	// 连接失败
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
	::Sleep(0);
	_endthreadex(0);
	// 防止超时引起的连接问题，并清除socket资源
	//shutdown(m_Sock,SD_BOTH);
	//closesocket(m_Sock);
	//m_Sock = 0;
	return 0;
}



//-----------------------------------------------------------------------------
// thread send
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadSend(LPVOID)
{
	fd_set fdwrite;
	timeval block_time;	// 设置发送阻塞时间
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;	// 150ms

	LPBYTE	pMsg = NULL, pTempPtr = NULL;
	DWORD	dwMsgSize = 0;
	INT		nReturn = 0;

	while( FALSE == m_bTerminateSend )
	{
		for(;;)	// 注意内部的break
		{
			// 流量统计
			DWORD dwTime = timeGetTime();
			if( dwTime - m_dwLastStatOutTime >= ONE_SECOND ) 
			{
				m_dwLastStatOutTime = dwTime;
				m_dwByteOut = m_dwByteOutCounter;
				m_dwByteOutCounter = 0;
			}

			pTempPtr = pMsg = m_pSendMsgQueue->GetMsg(dwMsgSize);
			if( NULL == pMsg )
				break;	// 发完为止

			if( m_bTerminateSend )
			{
				m_pSendMsgQueue->FreeMsg(pMsg);
				goto __thread_send_end;
			}

			if( m_bBigEndian )	// 处理字节序
				*(u_long*)pMsg = htonl(*(u_long*)pMsg);

			// send 可能不能一次将数据发完
			DWORD	dwSendTimeout = timeGetTime();
			while( dwMsgSize > 0 && FALSE == m_bTerminateSend )
			{
				if (TRUE == m_bSSL)
				{
					nReturn = BIO_write(m_pBIO, (char*)pTempPtr, dwMsgSize);
					if (nReturn <= 0)
					{
						goto __thread_send_end;
					}
				}
				else
				{
					nReturn = send(m_Sock, (char*)pTempPtr, dwMsgSize, 0);
				}
				if( SOCKET_ERROR == nReturn )
				{
					switch(WSAGetLastError())
					{
					case WSAEWOULDBLOCK:
					case WSAENOBUFS:
						// no buffer space is available within the transport 
						// system to hold the data to be transmitted
						// 等待数据可以发送的时机
						FD_ZERO(&fdwrite);
						FD_SET(m_Sock, &fdwrite);
						select(0, NULL, &fdwrite, NULL, &block_time);
						// 如果卡在这里超过60s，干脆断开连接
						if( timeGetTime() - dwSendTimeout > ONE_SECOND * 60 )
						{
							shutdown(m_Sock, SD_SEND);
							m_pSendMsgQueue->FreeMsg(pMsg);
							IMSG(_T("transport timeout ,send thread dead.\r\n"));
							goto __thread_send_end;
						}
						break;

					default:	// 故障
						shutdown(m_Sock, SD_SEND);
						m_pSendMsgQueue->FreeMsg(pMsg);
						IMSG(_T("transport client send thread dead.\r\n"));
						goto __thread_send_end;
					}
				}
				else
				{
					pTempPtr += nReturn;
					dwMsgSize -= nReturn;
					m_dwByteOutCounter += nReturn;	// 流量统计
				}
			}

			m_pSendMsgQueue->FreeMsg(pMsg);
			pTempPtr = pMsg = NULL;
		}

		//if( m_bConnected == FALSE ) // 如果此时就直接goto end吧
		//	goto __thread_send_end;

		::WaitForSingleObject(m_pSendMsgQueue->GetEvent(), 120);
	}


__thread_send_end:
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}



//-----------------------------------------------------------------------------
// receive thread function
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadReceive(LPVOID)
{
	fd_set fdread;
	timeval block_time;	// 设置阻塞时间
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;

	INT nResult = 0;
	DWORD dwMsgSize = 0;
	DWORD dwTempSize = 0;
	CHAR* pTempPtr = NULL;

	// 循环获得数据
	while( FALSE == m_bTerminateReceive )
	{
		// 先接受DWORD型的信息长度信息
		dwTempSize = sizeof(DWORD);
		pTempPtr = (char*)&dwMsgSize;

		// 流量统计
		DWORD dwTime = timeGetTime();
		if( dwTime - m_dwLastStatInTime >= ONE_SECOND )
		{
			m_dwLastStatInTime = dwTime;
			m_dwByteIn = m_dwByteInCounter;
			m_dwByteInCounter = 0;
		}

		while( dwTempSize > 0 )
		{
			// 先取得数据的大小
			FD_ZERO(&fdread);
			FD_SET(m_Sock, &fdread);
			if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
			{
				if (TRUE == m_bSSL)
				{
					nResult = BIO_read(m_pBIO, pTempPtr, dwTempSize);
				}
				else
				{
					nResult = recv(m_Sock, pTempPtr, dwTempSize, 0);
				}
				if( 0 == nResult )	// FIN
					goto __thread_receive_end;

				if( SOCKET_ERROR == nResult )
				{
					// 网络故障
					//shutdown(m_Sock, SD_SEND);
					goto __thread_receive_end;
				}
				else
				{
					dwTempSize -= nResult;
					pTempPtr += nResult;

					// 流量统计
					m_dwByteInCounter += nResult;
				}
			}
			
			if( m_bTerminateReceive )
				goto __thread_receive_end;
		}

		if( m_bBigEndian )	// 处理字节序
			dwMsgSize = ntohl(dwMsgSize);

		// ASSERT(dwMsgSize < 40960);	// 异常的数据
		if( SOCKET_ERROR != nResult )
		{
			char* pTemp = (char*)m_pMemPool->Alloc(dwMsgSize);
			if(!P_VALID(pTemp)) goto __thread_receive_end;
			dwTempSize = dwMsgSize;
			pTempPtr = pTemp;
			while( dwTempSize > 0 )
			{
				FD_ZERO(&fdread);
				FD_SET(m_Sock, &fdread);
				if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
				{
					if (TRUE == m_bSSL)
					{
						nResult = BIO_read(m_pBIO, pTempPtr, dwTempSize);
					}
					else
					{
						nResult = recv(m_Sock, pTempPtr, dwTempSize, 0);
					}
					if( 0 == nResult )	// FIN
					{
						// IMSG(_T("client get a FIN\r\n"));
						m_pMemPool->Free(pTemp);
						goto __thread_receive_end;
					}

					if( nResult == SOCKET_ERROR )
					{
						// 网络故障
						//shutdown(m_Sock, SD_SEND);
						m_pMemPool->Free(pTemp);
						goto __thread_receive_end;
					}
					else
					{
						dwTempSize -= nResult;
						pTempPtr += nResult;

						// 流量统计
						m_dwByteInCounter += nResult;
					}
				}

				if( m_bTerminateReceive )
				{
					m_pMemPool->Free(pTemp);
					goto __thread_receive_end;
				}
			}
			
			//初始化信息统计
			m_dwRecvPackNum++; 
			m_pRecvMsgQueue->AddMsg(pTemp, dwMsgSize);
			m_pMemPool->Free(pTemp);
		}
	}

__thread_receive_end:
	GracefulCloseSocket(m_Sock, 2);
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}

UINT StreamTransport::ThreadSSLStreamReceive(LPVOID)
{
	CONST INT RECV_BUFF_SIZE = 2048;	// 2KB recv buff
	INT nResult = 0;
	LPBYTE pBuffer = (LPBYTE)m_pMemPool->Alloc(RECV_BUFF_SIZE);

	// 循环获得数据
	while( FALSE == m_bTerminateReceive )
	{
		DWORD dwTime = timeGetTime();
		if( dwTime - m_dwLastStatInTime >= ONE_SECOND )	// 流量统计
		{
			m_dwLastStatInTime = dwTime;
			m_dwByteIn = m_dwByteInCounter;
			m_dwByteInCounter = 0;
		}

		if( ESTM_Stream == m_eTransportMode )
		{
			nResult = BIO_read(m_pBIO, (CHAR*)pBuffer, RECV_BUFF_SIZE);
			if( 0 <= nResult )	// FIN
				goto __thread_stream_receive_end;

			m_dwByteInCounter += nResult;	// 流量统计
			m_dwRecvPackNum++;	// 流量统计
			m_pRecvMsgQueue->AddMsg(pBuffer, nResult);	// 保存数据
		}


	}

__thread_stream_receive_end:
	m_pMemPool->Free(pBuffer);
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}


//-----------------------------------------------------------------------------
// stream receive thread function
//-----------------------------------------------------------------------------
UINT StreamTransport::ThreadStreamReceive(LPVOID)
{
	fd_set fdread;
	timeval block_time;	// 设置阻塞时间
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;

	CONST INT RECV_BUFF_SIZE = 2048;	// 2KB recv buff
	INT nResult = 0;
	LPBYTE pBuffer = (LPBYTE)m_pMemPool->Alloc(RECV_BUFF_SIZE);

	// 循环获得数据
	while( FALSE == m_bTerminateReceive )
	{
		DWORD dwTime = timeGetTime();
		if( dwTime - m_dwLastStatInTime >= ONE_SECOND )	// 流量统计
		{
			m_dwLastStatInTime = dwTime;
			m_dwByteIn = m_dwByteInCounter;
			m_dwByteInCounter = 0;
		}

		if( ESTM_Stream == m_eTransportMode )
		{
			FD_ZERO(&fdread);
			FD_SET(m_Sock, &fdread);
			if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
			{
				if (TRUE == m_bSSL)
				{
					nResult = BIO_read(m_pBIO, (CHAR*)pBuffer, RECV_BUFF_SIZE);
				}
				else
				{
					nResult = recv(m_Sock, (CHAR*)pBuffer, RECV_BUFF_SIZE, 0);
				}
				if( 0 == nResult )	// FIN
					goto __thread_stream_receive_end;

				if( nResult == SOCKET_ERROR )	// 网络故障
				{
					shutdown(m_Sock, SD_SEND);
					goto __thread_stream_receive_end;
				}
				else
				{
					m_dwByteInCounter += nResult;	// 流量统计
					m_dwRecvPackNum++;	// 流量统计
					m_pRecvMsgQueue->AddMsg(pBuffer, nResult);	// 保存数据
				}
			}
		}
		else if( ESTM_Text == m_eTransportMode )	// 文本模式按字节收
		{
			CHAR* pTempPtr = (CHAR*)pBuffer;
			DWORD dwMsgSize = 0;
			while( FALSE == m_bTerminateReceive )
			{
				FD_ZERO(&fdread);
				FD_SET(m_Sock, &fdread);
				if( 1 == select(0, &fdread, NULL, NULL, &block_time) )
				{
					if (TRUE == m_bSSL)
					{
						nResult = BIO_read(m_pBIO, pTempPtr, 1);
					}
					else
					{
						nResult = recv(m_Sock, pTempPtr, 1, 0);	// 收1个字节
					}
					if( 0 == nResult )	// FIN
						goto __thread_stream_receive_end;

					if( nResult == SOCKET_ERROR )	// 网络故障
					{
						shutdown(m_Sock, SD_SEND);
						goto __thread_stream_receive_end;
					}
					else
					{
						dwMsgSize += nResult;
						if( *pTempPtr == '\n' || dwMsgSize >= RECV_BUFF_SIZE )
						{
							//初始化信息统计
							m_dwRecvPackNum++; 
							m_pRecvMsgQueue->AddMsg(pBuffer, dwMsgSize);

							if (  *pTempPtr == '\n'   &&  *(pTempPtr-1) == '\r' && dwMsgSize == 2 )
								m_eTransportMode = ESTM_Stream;

							break;
						}

						// 流量统计
						m_dwByteInCounter += nResult;
						pTempPtr += nResult;
					}
				}
				else
				{
					if( dwMsgSize == 0 )
						break;
				}
			}
		}


	}

__thread_stream_receive_end:
	m_pMemPool->Free(pBuffer);
	::InterlockedExchange((LONG*)&m_bConnected, FALSE);
	_endthreadex(0);
	return 0;
}


//-----------------------------------------------------------------------------
// 文本模式和流模式之间切换，适合HTTP传输
//-----------------------------------------------------------------------------
BOOL StreamTransport::SwitchTransportMode(EStreamTransportMode eMode)
{
	if( ESTM_Package == m_eTransportMode || ESTM_Package == eMode )
		return FALSE;

	m_eTransportMode = eMode;
	return TRUE;

}


//-----------------------------------------------------------------------------
// 得到本地主机名
//-----------------------------------------------------------------------------
BOOL StreamTransport::GetHostName(std::string& strName)
{
	CHAR szHostName[X_LONG_STRING];
	INT nResult = gethostname(szHostName, sizeof(szHostName)-10);
	if( 0 == nResult )
	{
		strName = szHostName;
		return TRUE;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// 得到指定主机的IP
//-----------------------------------------------------------------------------
BOOL StreamTransport::GetIPByHostName(DWORD& dwIP, LPCSTR lpHostName)
{
	ASSERT(lpHostName);
	hostent* p = gethostbyname(lpHostName);
	if( p )
	{
		INT n=0;
		while((DWORD*)p->h_addr_list[n])
		{
			dwIP = *((DWORD*)p->h_addr_list[n]);
			n++;
		}
		return TRUE;
	}
	else
		return FALSE;
}


//-----------------------------------------------------------------------------
// 得到本机局域网地址
//-----------------------------------------------------------------------------
BOOL StreamTransport::GetLocalLANIP(DWORD& dwIP)
{
    SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
    if( sd == SOCKET_ERROR )
	{
        IMSG(_T("failed to get a socket.\r\n"));
		return FALSE;
    }

    INTERFACE_INFO InterfaceList[128];
    DWORD dwBytesReturned;
    if( SOCKET_ERROR == WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, 
		&InterfaceList,	sizeof(InterfaceList), &dwBytesReturned, 0, 0) ) 
	{
        IMSG(_T("failed calling WSAIoctl\r\n"));
		closesocket(sd);
		return FALSE;
    }

	closesocket(sd);

	INT nNumInterfaces = dwBytesReturned / sizeof(INTERFACE_INFO);
    for( INT i = 0; i < nNumInterfaces; ++i) 
	{
		u_long nFlags = InterfaceList[i].iiFlags;
		if( !(nFlags & IFF_UP) )	// 必须可用
			continue;

		if( nFlags & IFF_LOOPBACK ) // 不能是 loopback
			continue;

        sockaddr_in *pAddress;
        pAddress = (sockaddr_in *) & (InterfaceList[i].iiAddress);
        dwIP = pAddress->sin_addr.S_un.S_addr;

		if( pAddress->sin_addr.S_un.S_un_b.s_b1 != 51 
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 52 && pAddress->sin_addr.S_un.S_un_b.s_b2 >= 53 && pAddress->sin_addr.S_un.S_un_b.s_b2 <= 54)
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 55 && pAddress->sin_addr.S_un.S_un_b.s_b2 == 56) )
			return TRUE;
    }

	return FALSE;
}


//-----------------------------------------------------------------------------
// 得到本机广域网地址
//-----------------------------------------------------------------------------
BOOL StreamTransport::GetLocalWANIP(DWORD& dwIP)
{
    SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
    if( sd == SOCKET_ERROR )
	{
        IMSG(_T("failed to get a socket.\r\n"));
		return FALSE;
    }

    INTERFACE_INFO InterfaceList[128];
    DWORD dwBytesReturned;
    if( SOCKET_ERROR == WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, 
		&InterfaceList,	sizeof(InterfaceList), &dwBytesReturned, 0, 0) ) 
	{
        IMSG(_T("failed calling WSAIoctl\r\n"));
		closesocket(sd);
		return FALSE;
    }

	closesocket(sd);

	INT nNumInterfaces = dwBytesReturned / sizeof(INTERFACE_INFO);
    for( INT i = 0; i < nNumInterfaces; ++i) 
	{
		u_long nFlags = InterfaceList[i].iiFlags;
		if( !(nFlags & IFF_UP) )	// 必须可用
			continue;

		if( nFlags & IFF_LOOPBACK ) // 不能是 loopback
			continue;

        sockaddr_in *pAddress;
        pAddress = (sockaddr_in *) & (InterfaceList[i].iiAddress);
        dwIP = pAddress->sin_addr.S_un.S_addr;

   		if( !( pAddress->sin_addr.S_un.S_un_b.s_b1 != 51 
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 52 && pAddress->sin_addr.S_un.S_un_b.s_b2 >= 53 && pAddress->sin_addr.S_un.S_un_b.s_b2 <= 54)
			|| (pAddress->sin_addr.S_un.S_un_b.s_b1 == 55 && pAddress->sin_addr.S_un.S_un_b.s_b2 == 56) ))
			return TRUE;
    }

	return FALSE;
}



//-----------------------------------------------------------------------------
// 将真正IP地址转化为IP地址字符串
//-----------------------------------------------------------------------------
CHAR* StreamTransport::IP2String(DWORD dwIP)
{
	in_addr addr;
	addr.S_un.S_addr = dwIP;
	return inet_ntoa(addr);
}

//-----------------------------------------------------------------------------
// 将IP地址字符串转化为真正IP地址
//-----------------------------------------------------------------------------
DWORD StreamTransport::StringIP2IP(CHAR* szIP)
{
	return inet_addr(szIP);
}

BOOL StreamTransport::IsConnected()
{
	return m_bConnected;
}

} // namespace vEngine {