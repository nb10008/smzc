/***************************************************************************
 * Copyright (C) 2011 by Soldier Of Fortune Online
 * Email : duccati@dragonflygame.com
 *
 * Name : IPGDragonfly.h
 * Description : IPG for DragonFly
 * Create : 	2011-07-25   	Ver 1.0
 * Modified: 	
 ****************************************************************************/
#ifndef IPGDragonfly_H
#define IPGDragonfly_H

#include "stdafx.h"
//#include <pthread.h>
//#include <string.h>
//#include "Map.h"
//#include "UserProtocol.h"
//#include "ServerProtocol.h"
//#include "UserManager.h"
//#include "InformationFile.h"
//#include "Packet.h"
class World;
////////////////////////////////////////////////////////////////////////////
//
//	PreDefined Variant Definition
//
////////////////////////////////////////////////////////////////////////////
#define PKT_PG_LOG_DATA				0x00001000	//PG pack.type
#define USERID_LEN					40
#define USERNO_LEN					40
#define PKT_PG_AUTH_DATA			100
#define PKT_PG_PAY_DATA				110
#define PKT_PG_AUTHPAY_DATA			120
#define PKT_PG_CANCEL_DATA			200

//socket 관련 상수
#define MAX_WAIT_MILISEC			5000
#define MAX_WAIT_SECOND				5
//#define NO_ERROR					0

#define FALSE						0
#define TRUE						1

////////////////////////////////////////////////////////////////////////////
//
//	ErrorCode Definition
//
////////////////////////////////////////////////////////////////////////////
#define INVALID_IP_ADDRESS			999
#define RECEIVE_PACKET_ERROR		211
#define INVALID_PACKET_TYPE			212
#define INVALID_CONTENTS_ID			213
#define INVALID_ECPACKET_TYPE		214

#define PEERSOCKCLOSED				20000

#define LOGIN_ERROR_SOCKET_INIT		110
#define LOGIN_ERROR_SOCKET_CONN		111
#define LOGIN_ERROR_SEND_PACKET		120
#define LOGIN_ERROR_RECV_PACKET		121
#define LOGIN_ERROR_WRONG_PACKET	123
#define LOGIN_ERROR_UNKNOWN			199

#define H_INPUT						"100"
#define H_INPUT_CANCEL				"101"
#define H_OUTPUT					"200"
#define H_OUTPUT_CANCEL				"201"

#define IPG_INOUT_PORT				23100 
#define IPG_GETCASH_PORT			23200
#define IPG_BUFFER_SIZE				16384 

#define IPG_TIME_OUT				10000 // IPG 응답 대기시간 10초  

////////////////////////////////////////////////////////////////////////////
//
//	Packet Definition
//
////////////////////////////////////////////////////////////////////////////

struct PACKET_INOUT
{
	char	pack_type[4];			// 입금:100, 입금취소 :101, 출금:200, 출금취소:201 [in] 
	char	UserNo[USERNO_LEN];		//이용자 번호					[in]
	char	UserId[USERID_LEN];		//이용자 id					[in]
	char	CompanyCd[4+1];			//사이트 코드					[in]
	char	NickName[30];			//이용자 별명					[in]
	char	UserIp[15+1];			//이용자 ip					[in]
	char	OrgBxaid[20+1];			//취소시 원본 주문번호			[in]
	char	GoodsNo[6];				//상품 번호					[in]
	char	EventNo[6];				//이벤트 번호					[in]
	char	Amt[12];				//이벤트, 상품 금액			[in]
	char	AgencyNo[20+1];			//가맹점 주문 번호				[in]
	char	Etc1[100];				//여유변수1					[in]
	char	Etc2[100];				//여유변수2					[in]
	char	Etc3[100];				//여유변수3					[in]
	char	Etc4[100];				//여유변수4					[in]
	char	UserName[50];			//이름						[in]
	char	SocialNo[13+1];			//주민번호 - null허용('-'없이)	[in]
	char	Email[50];				//이메일 - null허용			[in]
	char	ZipCd[6+1];				//우편번호 - null허용('-'없이)	[in]
	char	TelNo[20];				//전화번호(휴대폰) - null허용	[in]
	char	Sex[2];					//성별						[in]
	char	BirthdayDt[9];			//생일 - null허용				[in]
	char	NationCd[3];			//ISO 국가코드 - null허용		[in] 
	char	SponsorCd[20];			//스폰서 코드 - null허용		[in]
	
	char	UserGrade[2+1];			//이용자 등급					[out]
	char	Bxaid[20+1];			//BillFlex IPG 주문번호		[out]
	char	RetCode[4+1];			//BillFlex IPG 결과값	 		[out]
	
	PACKET_INOUT() { memset(this, 0x00, sizeof(PACKET_INOUT)); strcpy(pack_type, "200"); strcpy(CompanyCd, "DFLY"); }  
};

struct INOUT_RET
{
	char UserGrade[2+1];	
	int Bxaid;
	int RetCode;
	
	INOUT_RET() { memset( this, 0x00, sizeof(INOUT_RET) ); } 
};

struct PACKET_GETCASH
{
	char	UserNo[USERNO_LEN];		//이용자 번호					[in]
	char	UserId[USERID_LEN];		//이용자 id					[in]
	char	CompanyCd[4+1];			//사이트 코드					[in]
	char	NickName[30];			//이용자 별명					[in]
	
	char	CashShop[12];			//Shopping-Mall Cash		[out]
	char	CashContents[12];		//Contents Cash				[out]
	char	CashBonus[12];			//Bonus Cash				[out]
	char	CashEtc[12];			//Mileage Point				[out]
	char	MileageShop[12];		//Shopping-Mall Mileage		[out]
	char	MileageContents[12];	//Contents Mileage			[out]
	char	RetCode[4+1];			//BillFlex IPG 결과 code. 	[out]
	
	PACKET_GETCASH() { memset( this, 0x00, sizeof(PACKET_GETCASH) ); strcpy( CompanyCd, "DFLY" ); } 
};

class IPGDragonfly
{	
private:
	bool 		m_bInitialize;					// 초기화 작동여부
	bool	 	m_bRunning;
			
	char		m_IPGServerIP[20];			// IPG 서버 IP 
	int			m_GetCashPort;					// GetCash Port 
	int			m_IOCashPort;					// IOCash Port 

	static IPGDragonfly* m_SelfInstance;

	IPGDragonfly();
	~IPGDragonfly();
		
public:
	static IPGDragonfly* GetInstance();
	static void ReleaseInstance();

	bool InitIPGDragonfly( char *pszIP, int nGetCashPort, int nIoCashPort);
	
	bool IsRunning() { return m_bRunning;}
	bool IsInitialize() { return m_bInitialize; }
	
	// Network Function 
	BOOL ConnectToServer( SOCKET* sock, char *IPAddr, int Port );
	BOOL Send( SOCKET sock, char* Buffer, int Length );
	BOOL Receive( SOCKET sock, char* Buffer, int Length );
	BOOL CloseMySocket( SOCKET sock );
	
	// GetCash 	
	bool GetDFCashReq( const char* usn, const char* accid, const char* nick, int& cash );	
	// OutCash	
	bool OutDFCashReq( INOUT_RET& ret, const char* usn, const char* accid, char* nick, char* ip, int shopid, int cash, int sex, char* gift_accid, int nNum );
	
};

#endif //~IPGDragonfly_H
