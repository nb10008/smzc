/***************************************************************************
 * Copyright (C) 2011 by Soldier Of Fortune Online
 * Email : duccati@dragonflygame.com
 *
 * Name : IPGDragonfly.cpp
 * Description : IPG for DragonFly
 * Create : 	2011-07-25   	Ver 1.0
 * Modified: 	
 ****************************************************************************/
//#include "Main.h"
#include "StdAfx.h"
#include "IPGDragonfly.h"

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <string.h>
//#include <unistd.h>
//#include <stdio.h>
#include "world.h"
#include "world_session.h"

#define USE_NONBLOCK_CONNECT  


IPGDragonfly* IPGDragonfly::m_SelfInstance = NULL;

IPGDragonfly* IPGDragonfly::GetInstance()
{
	if(m_SelfInstance == NULL)
	{
		m_SelfInstance = new IPGDragonfly();
	}

	return m_SelfInstance;
}

void IPGDragonfly::ReleaseInstance()
{
	if(m_SelfInstance != NULL) 
	{ 
		delete m_SelfInstance; 
		m_SelfInstance = NULL; 
	}
}

IPGDragonfly::IPGDragonfly()
{
	m_bInitialize = false;
	
	memset( m_IPGServerIP, 0x00, sizeof(m_IPGServerIP) );
	m_GetCashPort = 0;
	m_IOCashPort = 0;
}

IPGDragonfly::~IPGDragonfly()
{
}

bool IPGDragonfly::InitIPGDragonfly( char *pszIP, int nGetCashPort, int nIoCashPort)
{
	//memset( m_IPGServerIP, 0x00, sizeof(m_IPGServerIP) );
	strcpy( m_IPGServerIP, pszIP );
	m_GetCashPort = nGetCashPort;
	m_IOCashPort = nIoCashPort;


	m_bInitialize = true;
	
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Network Function 

BOOL IPGDragonfly::ConnectToServer( SOCKET* sock, char *ipaddr, int port )
{
	if( (*sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{		
		//DebugMsg( "IPGDragonfly::ConnectToServer..Cannot Create socket" );
		g_world.WriteKroeaLog(_T("IPGDragonfly::ConnectToServer..Cannot Create socket. \r\n"));
		return FALSE;
	}

	struct sockaddr_in	m_ServerAddr;	
	m_ServerAddr.sin_addr.s_addr = inet_addr( ipaddr );
	m_ServerAddr.sin_port = htons( port );
	m_ServerAddr.sin_family = AF_INET;
	 
#ifdef USE_NONBLOCK_CONNECT
	// 플레그 백업 

	//int saveflags  = fcntl( *sock, F_GETFL, 0 ); 	
	//
	//// non-block 세팅		
	//fcntl(*sock, F_SETFL, saveflags|O_NONBLOCK);		

	ULONG argp = 1;
	if ( ioctlsocket(*sock,FIONBIO,&argp) < 0 ) 
	{
		//DebugMsg
		g_world.WriteKroeaLog(_T("IPGDragonfly::ioctlsocket error \r\n"));
	}

	// connect 
	if( connect(*sock, (struct sockaddr*)&m_ServerAddr, sizeof(struct sockaddr_in)) < 0 )
	{
		INT iErr = WSAGetLastError() ;
		//if( errno != EINPROGRESS )
		if(WSAEWOULDBLOCK != iErr)
		{
			//DebugMsg( "IPGDragonfly::ConnectToServer..Cannot connect Server(%s:%d), connect..errno(%d)", ipaddr, port, errno );
			g_world.WriteKroeaLog(_T("IPGDragonfly::ConnectToServer err. \r\n"));
			return FALSE;
		}
		else
		{
			g_world.WriteKroeaLog(_T("IPGDragonfly::ConnectToServer ok. \r\n"));
		}
	}
	else
	{
		g_world.WriteKroeaLog(_T("IPGDragonfly::ConnectToServer ok1. \r\n"));
	}
	
	
	// 플레그 복귀  
	//fcntl(*sock, F_SETFL, saveflags);					
	
	// connect 체크 
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0L;
	
	fd_set conset;
	FD_ZERO( &conset );
	FD_SET( *sock, &conset );
	
	int err = select( *sock+1, NULL, &conset, NULL, &timeout );
	if( err < 0 || err == 0 )
	{
		//DebugMsg( "IPGDragonfly::ConnectToServer..Cannot connect Server(%s:%d), err < 0 || err == 0..errno(%d)", ipaddr, port, errno );

		TCHAR tszaAddr[16] = {0};
		wsprintf( tszaAddr, IUTIL->AnsiToUnicode(ipaddr) );

		TCHAR tszTemp[128] = {0};
		wsprintf( tszTemp, _T("IPGDragonfly::ConnectToServer..Cannot connect Server(%s:%d), err < 0 || err == 0.errno(%d). \r\n"),tszaAddr,port,err);
		g_world.WriteKroeaLog(tszTemp);
		return FALSE;	
	}
	else
	{
		g_world.WriteKroeaLog(_T("IPGDragonfly::ConnectToServer ok2. \r\n"));
	}
	
	//int ret = 0;
	//socklen_t optlen = sizeof(int);
	//if( getsockopt(*sock, SOL_SOCKET, SO_ERROR, &ret, &optlen) < 0 )
	//{
	//	//DebugMsg( "IPGDragonfly::ConnectToServer..Cannot connect Server(%s:%d), getsockopt..errno(%d)", ipaddr, port, errno );
	//	return FALSE;
	//}

	int ret = 0;
	int optlen = sizeof(int);
	if( getsockopt( *sock , SOL_SOCKET , SO_ERROR , (CHAR*)&ret , &optlen ) < 0 )
	{
		//DebugMsg
		g_world.WriteKroeaLog(_T("IPGDragonfly::getsockopt err < 0. \r\n"));
		return FALSE;
	}
	
	if( ret != 0 )
	{
		//DebugMsg( "IPGDragonfly::ConnectToServer..Cannot connect Server(%s:%d), ret != 0..errno(%d)", ipaddr, port, errno );
		g_world.WriteKroeaLog(_T("IPGDragonfly::getsockopt ret != 0. \r\n"));
		return FALSE;		
	}
#else 
	if( connect(*sock, (struct sockaddr*)&m_ServerAddr, sizeof(struct sockaddr_in)) < 0 )
	{
		//DebugMsg( "IPGDragonfly::ConnectToServer..Cannot connect Server(%s:%d)", ipaddr, port );
		return FALSE;
	}	
#endif //~USE_NONBLOCK_CONNECT
	
	return TRUE;	
}

BOOL IPGDragonfly::Send( SOCKET sock, char* buffer, int length )
{
	//if( write(sock, buffer, length) != length )
	//{
	//	//DebugMsg( "IPGDragonfly::Send..Socket(%d) Write(%d) error(%d)", sock, length, errno );
	//	return FALSE;
	//}


	if( length != send(sock,(const CHAR *)buffer,length,0) )
	{
		g_world.WriteKroeaLog(_T("IPGDragonfly::Send err. \r\n"));
		return FALSE;
	}

	
	return TRUE;
}

BOOL IPGDragonfly::Receive( SOCKET sock, char* buffer, int length )
{
	DWORD r_cnt = 0;
	int	r_value;

	fd_set	readfds;
	int	nfounds;
	struct timeval tm;

	memset( buffer, 0x00, length );

	while( (int)r_cnt < length ) 
	{
		tm.tv_sec = MAX_WAIT_SECOND;
		tm.tv_usec = 0;

		FD_ZERO( &readfds );
		FD_SET( sock, &readfds );

		nfounds = select( sock + 1, &readfds, NULL, NULL, &tm );
		
		if( nfounds <= 0 ) 
		{
			//DebugMsg( "IPGDragonfly::Receive..Socket(%d) Read Timeout", sock );
			g_world.WriteKroeaLog(_T("IPGDragonfly::Receive  Read Timeout. \r\n"));
			return FALSE;
		}

		if( FD_ISSET(sock, &readfds) )
		{ 
			//r_value = read( sock, buffer+r_cnt, length-r_cnt );
			r_value = recv(sock,(CHAR*)(buffer+r_cnt),length-r_cnt,0);

			if ( r_value <= 0 ) 
			{
				//DebugMsg( "IPGDragonfly::Receive..Socket(%d) Read Error(%d)", sock, errno );
				g_world.WriteKroeaLog(_T("IPGDragonfly::Receive Read Error. \r\n"));
				return FALSE;
			}
			
			r_cnt += r_value;
		}
	}
	
	return TRUE;	
}

BOOL IPGDragonfly::CloseMySocket( SOCKET sock )
{
	return closesocket(sock);
}
	
////////////////////////////////////////////////////////////////////////////////////////////////	
// GetCash 

bool IPGDragonfly::GetDFCashReq( const char* usn, const char* accid, const char* nick, int& cash )
{
	g_world.WriteKroeaLog(_T("begin GetDFCashReq. \r\n"));

	if( NULL == usn || !accid || !nick ) 
	{
		g_world.WriteKroeaLog(_T("GetDFCashReq err1. \r\n"));
		return false; 
	}
		
	PACKET_GETCASH packet;
	//snprintf( packet.UserNo, sizeof(packet.UserNo), "%d", usn );
	//snprintf( packet.UserId, sizeof(packet.UserId), "%s", accid );
	//snprintf( packet.NickName, sizeof(packet.NickName), "%s", nick );

	sprintf( packet.UserNo, "%s", usn );
	sprintf( packet.UserId, "%s", accid );
	sprintf( packet.NickName, "%s", nick );
	
	SOCKET sockGetCash;
	if( !ConnectToServer(&sockGetCash, m_IPGServerIP, m_GetCashPort) )
	{
		TCHAR tszIPGServerIP[64] = {0};
		IUTIL->Unicode8ToUnicode(m_IPGServerIP,tszIPGServerIP);

		TCHAR tszTemp[64] = {0};
		wsprintf(tszTemp,_T(">>> GetDFCash - Connection(%s:%d) Failed!!. \r\n"),tszIPGServerIP, m_GetCashPort);
		g_world.WriteKroeaLog(tszTemp);
		CloseMySocket(sockGetCash);
		return false;
	}
	else
	{
		TCHAR tszTemp[256] = {0};
		TCHAR tszUserNo[USERNO_LEN] = {0};
		TCHAR tszUserId[USERID_LEN] = {0};
		TCHAR tszNickName[30] = {0};

		//IUTIL->Unicode8ToUnicode(packet.UserNo,tszUserNo);
		//g_world.WriteKroeaLog(tszUserNo);
		//IUTIL->Unicode8ToUnicode(packet.UserId,tszUserId);
		//g_world.WriteKroeaLog(tszUserId);
		//IUTIL->Unicode8ToUnicode(packet.NickName,tszNickName);
		//wsprintf(tszTemp,_T("GetDFCash packet date. packet.UserNo:%s, packet.UserId:%s, packet.NickName:%s. \r\n"),tszUserNo,tszUserId,tszNickName);
		//g_world.WriteKroeaLog(tszTemp);

		wsprintf( tszUserNo, IUTIL->AnsiToUnicode(packet.UserNo) );
		wsprintf( tszUserId, IUTIL->AnsiToUnicode(packet.UserId) );
		wsprintf( tszNickName, IUTIL->AnsiToUnicode(packet.NickName) );
		wsprintf(tszTemp,_T("GetDFCash packet date. UserNo:%s, UserId:%s, NickName:%s. \r\n"),tszUserNo,tszUserId,tszNickName);
		g_world.WriteKroeaLog(tszTemp);


		if( !Send(sockGetCash, (char *)&packet, sizeof(PACKET_GETCASH)))
		{	
			// 인증요청 패킷 송신 실패시
			//DebugMsg( ">>> GetDFCash - Send Packet Error(%d), ErrorMsg:%s", errno, strerror(errno) );

			g_world.WriteKroeaLog(_T(">>> GetDFCash - Send Packet Error. \r\n"));
			CloseMySocket(sockGetCash);
			return false;
		}
		else
		{ 
			if( !Receive( sockGetCash, (char *)&packet, sizeof(PACKET_GETCASH) ) )
			{	
				// 응답 패킷 수신 실패시
				//DebugMsg( ">>> GetDFCash - Receive Packet Error(%d), ErrorMsg:%s", errno, strerror(errno) );
				
				g_world.WriteKroeaLog(_T(">>> GetDFCash - Receive Packet Error. \r\n"));
				CloseMySocket(sockGetCash);
				return false;
			} 
			else
			{	
				// 응답 패킷 수신 성공시			
				int CashShop = atoi(packet.CashShop);
				int CashContents = atoi(packet.CashContents);
				int CashBonus = atoi(packet.CashBonus);
				int CashEtc = atoi(packet.CashEtc);
				int MileageShop = atoi(packet.MileageShop);
				int MileageContents = atoi(packet.MileageContents);
				int Amount = CashShop + CashContents + CashBonus + CashEtc + MileageShop + MileageContents;
				packet.RetCode[4] = '\0';
						
				TCHAR tszUserNo[USERNO_LEN] = {0};
				TCHAR tszUserId[USERID_LEN] = {0};
				TCHAR tszRetCode[5] = {0};
				//IUTIL->Unicode8ToUnicode(accid,tszUserId);
				//IUTIL->Unicode8ToUnicode(usn,tszUserNo);
				//IUTIL->Unicode8ToUnicode(packet.RetCode,tszRetCode);


				wsprintf( tszUserNo, IUTIL->AnsiToUnicode(usn) );
				wsprintf( tszUserId, IUTIL->AnsiToUnicode(accid) );
				wsprintf( tszRetCode, IUTIL->AnsiToUnicode(packet.RetCode) );

				if( !strncmp( packet.RetCode, "0000", 4 ) )
				{
					//DebugMsg( "GetDFCash User(%s:%d), cash(%d:%d:%d:%d:%d:%d), sum:%d, ret:%s", \
							accid, usn, CashShop, CashContents, CashBonus, CashEtc, MileageShop, MileageContents, Amount, packet.RetCode ); 
					cash = Amount;

					TCHAR tszTemp[256] = {0};
					wsprintf(tszTemp,_T("GetDFCash User(%s:%s), cash(%d:%d:%d:%d:%d:%d), sum:%d, ret:%s. \r\n"),tszUserId, tszUserNo, CashShop, CashContents, CashBonus, CashEtc, MileageShop, MileageContents, Amount,tszRetCode);
					g_world.WriteKroeaLog(tszTemp);

					wsprintf(tszTemp,_T("GetDFCash cash:%d. \r\n"),cash);
					g_world.WriteKroeaLog(tszTemp); 
				}
				else
				{
					//DebugMsg( ">>> GetDFCash - User(%s:%d) FAILED(%s)", accid, usn, packet.RetCode ); 
					ILOG->Write(_T(">>> GetDFCash - User(%s:%s) FAILED(%s). \r\n"),tszUserId, tszUserNo, tszRetCode);

					TCHAR tszTemp[64] = {0};
					wsprintf(tszTemp,_T(">>> GetDFCash - User(%s:%s) FAILED(%s). \r\n"),tszUserId, tszUserNo, tszRetCode);
					g_world.WriteKroeaLog(tszTemp);
				}
				
				CloseMySocket(sockGetCash);
			}
		}
	}

	g_world.WriteKroeaLog(_T("GetDFCashReq end . \r\n"));

	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// OutCash 

bool IPGDragonfly::OutDFCashReq( INOUT_RET& ret, const char* usn, const char* accid, char* nick, char* ip, int shopid, int cash, int sex, char* gift_accid, int nNum)
{
	g_world.WriteKroeaLog(_T("begin OutDFCash. \r\n"));

	if( NULL == usn || !accid || !nick || !ip ) 
	{
		g_world.WriteKroeaLog(_T(">>> OutDFCash err1. \r\n"));
		return false; 
	}
		
	PACKET_INOUT packet;
	strncpy( packet.pack_type, H_OUTPUT, sizeof(packet.pack_type)-1 );
	g_world.WriteKroeaLog(_T(">>> OutDFCash H_OUTPUT. \r\n"));

	//snprintf( packet.UserNo, sizeof(packet.UserNo), "%d", usn );
	//snprintf( packet.UserId, sizeof(packet.UserId), "%s", accid  );
	//snprintf( packet.NickName, sizeof(packet.NickName), "%s", nick );
	//snprintf( packet.UserIp, sizeof(packet.UserIp), "%s", ip );	
	//snprintf( packet.GoodsNo, sizeof(packet.GoodsNo)+sizeof(packet.EventNo)-1, "%d", shopid );		
	//snprintf( packet.Amt, sizeof(packet.Amt), "%d", cash );		
	//snprintf( packet.Sex, sizeof(packet.Sex), "%d", sex );
	//snprintf( packet.Etc2, sizeof(packet.Etc2), "%s", gift_accid ); // 보통 여기에 선물 받을 사람의 계정명을  쓴다고 한다 

	sprintf( packet.UserNo, "%s", usn );
	sprintf( packet.UserId, "%s", accid  );
	sprintf( packet.NickName, "%s", nick );
	sprintf( packet.UserIp, "%s", ip );	
	sprintf( packet.GoodsNo, "%d", shopid );		
	sprintf( packet.Amt, "%d", cash );		
	sprintf( packet.Sex, "%d", sex );
	//sprintf( packet.Etc2, "%s", gift_accid ); // 보통 여기에 선물 받을 사람의 계정명을  쓴다고 한다 
	sprintf( packet.Etc2, "%d", nNum );
	
	SOCKET sockOutCash;
	if( !ConnectToServer(&sockOutCash, m_IPGServerIP, m_IOCashPort) )
	{
		TCHAR tszIPGServerIP[64] = {0};
		IUTIL->Unicode8ToUnicode(m_IPGServerIP,tszIPGServerIP);

		//DebugMsg( ">>> OutDFCash - Connection(%s:%d) Failed!!", m_IPGServerIP, m_IOCashPort );

		TCHAR tszTemp[64] = {0};
		wsprintf(tszTemp,_T(">>> OutDFCash - Connection(%s:%d) Failed!! \r\n"),tszIPGServerIP, m_IOCashPort);

		g_world.WriteKroeaLog(tszTemp);
		CloseMySocket(sockOutCash);
		return false;
	}
	else
	{ 
		TCHAR tszUserNo[USERNO_LEN] = {0};
		TCHAR tszUserId[USERID_LEN] = {0};
		TCHAR tszName[30] = {0};
		TCHAR tszGoodsNo[6] = {0};
		TCHAR tszAmt[12] = {0};

		wsprintf( tszUserId, IUTIL->AnsiToUnicode(packet.UserId) );
		wsprintf( tszUserNo, IUTIL->AnsiToUnicode(packet.UserNo) );
		wsprintf( tszName, IUTIL->AnsiToUnicode(packet.NickName) );
		wsprintf( tszGoodsNo, IUTIL->AnsiToUnicode(packet.GoodsNo) );
		wsprintf( tszAmt, IUTIL->AnsiToUnicode(packet.Amt) );

		TCHAR tszTemp[256] = {0};
		wsprintf(tszTemp,_T("OutDFCash send packer. User(%s:%s:%s), GoodsNo:%s, Amt:%s . \r\n"),tszUserId, tszUserNo, tszName, tszGoodsNo, tszAmt);
		g_world.WriteKroeaLog(tszTemp);


		if( !Send(sockOutCash, (char *)&packet, sizeof(PACKET_INOUT)))
		{	
			// 요청 패킷 송신 실패시
			//DebugMsg( ">>> OutDFCash - Send Packet Error(%d), ErrorMsg:%s", errno, strerror(errno) );

			g_world.WriteKroeaLog(_T(">>> OutDFCash - Send Packet Error \r\n"));
			CloseMySocket(sockOutCash);
			return false;
		}
		else
		{ 
			if( !Receive( sockOutCash, (char *)&packet, sizeof(PACKET_INOUT) ) )
			{	
				// 응답 패킷 수신 실패시
				//DebugMsg( ">>> OutDFCash - Receive Packet Error(%d), ErrorMsg:%s", errno, strerror(errno) );
				g_world.WriteKroeaLog(_T(">>> OutDFCash - Receive Packet Error \r\n"));
				CloseMySocket(sockOutCash);
				return false;
			} 
			else
			{	
				// 응답 패킷 수신 성공시	
				packet.RetCode[4] = '\0';

				TCHAR tszUserNo[USERNO_LEN] = {0};
				TCHAR tszUserId[USERID_LEN] = {0};
				TCHAR tszRetCode[5] = {0};
				TCHAR tszIP[16] = {0};
				TCHAR tszBxaid[21] = {0};
				//IUTIL->Unicode8ToUnicode(accid,tszUserId);
				//IUTIL->Unicode8ToUnicode(usn,tszUserNo);
				//IUTIL->Unicode8ToUnicode(packet.RetCode,tszRetCode);
				//IUTIL->Unicode8ToUnicode(ip,tszIP);
				//IUTIL->Unicode8ToUnicode(packet.Bxaid,tszBxaid);

				wsprintf( tszUserId, IUTIL->AnsiToUnicode(accid) );
				wsprintf( tszUserNo, IUTIL->AnsiToUnicode(usn) );
				wsprintf( tszRetCode, IUTIL->AnsiToUnicode(packet.RetCode) );
				wsprintf( tszIP, IUTIL->AnsiToUnicode(ip) );
				wsprintf( tszBxaid, IUTIL->AnsiToUnicode(packet.Bxaid) );
					
				if( !strncmp( packet.RetCode, "0000", 4 ) )
				{
					//DebugMsg( "OutDFCash User(%s:%d), ip:%s, shopid:%d, chash:%d, ret:%s, bxaid:%s", \
							accid, usn, ip, shopid, cash, packet.RetCode, packet.Bxaid ); 

					ret.RetCode = 0;
					memcpy( ret.UserGrade, packet.UserGrade, sizeof(ret.UserGrade) );
					ret.Bxaid = atoi(packet.Bxaid);

					TCHAR tszTemp[128] = {0};
					wsprintf(tszTemp,_T("OutDFCash User(%s:%s), ip:%s, shopid:%d, chash:%d, ret:%s, bxaid:%s. \r\n"),tszUserId, tszUserNo, tszIP, shopid, cash, tszRetCode, tszBxaid);
					g_world.WriteKroeaLog(tszTemp);
					return true;
				}
				else
				{
					//DebugMsg( ">>> OutDFCash - User(%s:%d) FAILED(%s)", accid, usn, packet.RetCode ); 
					TCHAR tszTemp[128] = {0};
					wsprintf(tszTemp,_T(">>> OutDFCash - User(%s:%s) FAILED(%s). \r\n"),tszUserId, tszUserNo, tszRetCode);
					g_world.WriteKroeaLog(tszTemp);

					CloseMySocket(sockOutCash);
					return false;				
				}
				
				CloseMySocket(sockOutCash);
			}
		}
	}
	
	g_world.WriteKroeaLog(_T("end OutDFCash. \r\n"));
	return false;
}

