#pragma once

class SendHttpPost
{
public:
	SendHttpPost();
	~SendHttpPost();

	bool HttpRequest(LPCWSTR lpHostName,short sPort,const char * lpUrl,char * lpMethod,char * lpPostData,int nPostDataLen);
	//bool SendRequset( TCHAR* ptszHostName,const char*ptszRequest, CHAR *pszRespones );
};