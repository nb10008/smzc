#include "Stdafx.h"
#include "SendHttpPost.h"
#include <windows.h>
#include "Wininet.h"
#include <fstream>

#pragma comment(lib,"Wininet.lib")

SendHttpPost::SendHttpPost()
{

}
SendHttpPost::~SendHttpPost()
{

}

bool SendHttpPost::HttpRequest(LPCWSTR lpHostName,short sPort,const char * lpUrl,char * lpMethod,char * lpPostData,int nPostDataLen)
{
	HINTERNET hInternet,hConnect,hRequest;
	BOOL bRet;
	fd_set fdwrite;
	timeval block_time;	// 设置阻塞时间
	block_time.tv_sec = 0;
	block_time.tv_usec = GT_NET_BLOCK_TIME;


	hInternet = InternetOpen(_T("User-Agent"),INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	if(!hInternet)
	{
		MessageBox(NULL, _T("Err1"), _T("Err1"), MB_OK);
		goto Ret0;
	}
	MessageBox(NULL, _T("Open"), _T("Success"), MB_OK);

	hConnect = InternetConnect(hInternet,(LPCWSTR)lpHostName,sPort,NULL,_T("HTTP/1.1"),INTERNET_SERVICE_HTTP,0,0);
	if(!hConnect)
	{
		if( WSAEWOULDBLOCK == WSAGetLastError() )
		{
			for(INT n=0; n<64; n++) // 不停重试,外部可以通过调用StopTryingConnect停止重试
			{
				FD_ZERO(&fdwrite);
				FD_SET((SOCKET)hInternet, &fdwrite);

				block_time.tv_sec = 0;
				block_time.tv_usec = GT_NET_BLOCK_TIME;
				if( 1 == select(0, NULL, &fdwrite, NULL, &block_time) )
				{
					MessageBox(NULL, _T("Connect"), _T("Success"), MB_OK);
					goto __connected;
				}

				Sleep(100);
			}
		}

		goto Ret0;
	}

__connected:
	hRequest = HttpOpenRequest(hConnect,(LPCWSTR)lpMethod,(LPCWSTR)lpUrl,_T("HTTP/1.1"),NULL,NULL,INTERNET_FLAG_RELOAD,0);
	if(!hRequest)
	{
		MessageBox(NULL, _T("Err3"), _T("Err3"), MB_OK);
		goto Ret0;
	}

	MessageBox(NULL, _T("OpenRequest"), _T("Success"), MB_OK);

	bRet = HttpSendRequest(hRequest,NULL,0,lpPostData,nPostDataLen);
	TCHAR szRet[128] = {0};
	_stprintf(szRet, _T("%d"), (INT)bRet);

	MessageBox(NULL, _T("SendResult"), szRet, MB_OK);

	MessageBox(NULL, _T("BeginRead"), _T("BeginRead"), MB_OK);
	DWORD dwByteRead = 0;
	CHAR* pszRespones;
	if( InternetReadFile( hRequest, pszRespones, 2048, &dwByteRead ) && dwByteRead > 0 )
	{
		pszRespones[dwByteRead] = '\0';
	}
	MessageBox(NULL, TObjRef<Util>()->Unicode8ToUnicode(pszRespones), _T("Err4"), MB_OK);
	
Ret0:

	if(hRequest)
		InternetCloseHandle(hRequest);

	if(hConnect)
		InternetCloseHandle(hConnect);

	if(hInternet)
		InternetCloseHandle(hInternet);


	int nRet = GetLastError();
	TCHAR szText[128] = {0};
	_stprintf(szText, _T("%d"), nRet);
	MessageBox(NULL, szText, _T("Err5"), MB_OK);

	ofstream wFileStream;
	wFileStream.open("c:\\yunlianlog.txt",ios::out|ios::app/*|ios::trunc*/);
	if (wFileStream)
	{
	 	wFileStream <<  lpHostName <<endl;
	 	wFileStream.close();
	 }

	return bRet;
}