//-----------------------------------------------------------------------------
//!\file game_net_web.cpp
//!\author liutingting
//!
//!\date 2011-4-29
//! last 2011-4-29
//!
//!\brief http 协议控制类，用于Russia交互
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "game_net_web.h"
#include "login_server.h"
#include <Wininet.h>
#include <comutil.h>
#include <Msxml2.h>
#include <atlConv.h>
// 2d77: msxml.dll is not existed under Vista and later OS -->
//#import "msxml.dll"
//using namespace MSXML
// Must use "msxml6.dll" under Win7, and the namespace is MSXML2
#import "msxml6.dll"
using namespace MSXML2;
// Also create a namespace alias so that the code compiles...
namespace MSXML = MSXML2;
// <--

#define		WEB_SERVICE_ID		300003010000000000
#define		STATUS_FREE			_T("free")
#define		STATUS_PREMIUM		_T("premium")
#define		STATUS_TRIAL		_T("trial")
#define		STATUS_BLOCKED		_T("blocked")
#define		STATUS_FROZEN		_T("frozen")
//-------------------------------------------------------------------------------
// 初始化函数
BOOL GameNetWeb::Init()
{
	//CoInitialize(NULL);
	return TRUE;
}

//-------------------------------------------------------------------------------
// 销毁函数
VOID GameNetWeb::Destroy()
{
	//CoUninitialize();
}
//-------------------------------------------------------------------------------
BOOL GameNetWeb::SendRequset( TCHAR *ptszRequest, CHAR *pszRespones )
{
	if( !P_VALID(ptszRequest) || !P_VALID(pszRespones) )
		return FALSE;

	HINTERNET	hSession;
	HINTERNET	hConnect;
	HINTERNET	hRequest;

	//打开会话
	hSession = InternetOpen(_T("GameNetWeb"),INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
	if(NULL == hSession)
	{
		ILOG->Write(_T("SendRequset:1.\r\n"));
		return FALSE;
	}

	//建立连接
	hConnect = InternetConnect(hSession, _T("api.gamenet.ru"), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if(NULL == hConnect)
	{
		ILOG->Write(_T("SendRequset:2.\r\n"));
		InternetCloseHandle(hSession);
		return FALSE;
	}

	//初始化下载请求
	hRequest = HttpOpenRequest( hConnect, 
		_T("GET"), 
		ptszRequest,
		_T("HTTP/1.1"), 
		NULL, 
		0, 
		INTERNET_FLAG_SECURE, 
		0 );


	if(NULL == hRequest)
	{
		ILOG->Write(_T("SendRequset:3.\r\n"));
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return FALSE;
	}

	//发送下载请求
	if( !HttpSendRequest( hRequest, NULL, 0,NULL, 0 ) )
	{
		int nRet = GetLastError();
	
		ILOG->Write(_T("SendRequset:4.\r\n"));
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return FALSE;
	}

	DWORD dwByteRead = 0;

	if( InternetReadFile( hRequest, pszRespones, WEB_REQUEST_MAX, &dwByteRead ) && dwByteRead > 0 )
	{
		pszRespones[dwByteRead] = '\0';
	}

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hSession);

	return TRUE;
}
//-------------------------------------------------------------------------------
DWORD GameNetWeb::GetServiceToken( LPCSTR pszToken )
{
	if( !P_VALID(pszToken) )
		return E_ProofResult_GameNet_CanNotConnect_WEB;

	TCHAR tszTemp[WEB_REQUEST_MAX] = {0};
	TCHAR tszToken[1024] = {0};

	//wsprintf( tszTemp,_T("/restapi?method=auth.getServiceToken&ServiceID=%d&Token=%s"), WEB_SERVICE_ID, pszToken ); 
	MultiByteToWideChar(CP_ACP,NULL,pszToken,-1,tszToken,1024);

	wsprintf( tszTemp,_T("/restapi?method=auth.getServiceToken&serviceId=300003010000000000&authType=UserId&authId=%s"), tszToken ); 

	ILOG->Write(_T("GetServiceToken:%s.\r\n"),tszTemp);

	CHAR szBuffer[WEB_REQUEST_MAX] = {0};
	if( !SendRequset( tszTemp, szBuffer ) )
		return E_ProofResult_GameNet_CanNotConnect_WEB;

	TCHAR tszBuffer[WEB_REQUEST_MAX] = {0};
	MultiByteToWideChar(CP_ACP,NULL,szBuffer,-1,tszBuffer,WEB_REQUEST_MAX);
	ILOG->Write(_T("GetServiceToken return buff:%s.\r\n"),tszBuffer);

	//解析得到的xml
	HRESULT hr = 0;
	VARIANT_BOOL vResult;
	BSTR bstrText;
	const TCHAR	*pszValue = NULL;
	MSXML::IXMLDOMDocumentPtr	docPtr;
	MSXML::IXMLDOMElementPtr	elementDataPtr;
	MSXML::IXMLDOMElementPtr	elementItemPtr;
	MSXML::IXMLDOMNodeListPtr	nodeListItemsPtr;
	MSXML::IXMLDOMNodePtr		nodeItemPtr;
	VARIANT value;

	//CHAR	szLogin[256] = {0};
	//CHAR	szToken[X_TOKEN] = {0};
	//CHAR	szPassword[256] = {0};

	CoInitialize(NULL);
	USES_CONVERSION;

	hr = docPtr.CreateInstance("MSXML.domdocument");
	if(FAILED(hr))
	{
		CoUninitialize();
		ILOG->Write(_T("GetServiceToken:1.\r\n"));
		return E_ProofResult_GameNet_XML;
	}

	vResult = docPtr->loadXML(_bstr_t(szBuffer));
	if( vResult != VARIANT_TRUE)
	{
		docPtr.Release();
		CoUninitialize();
		ILOG->Write(_T("GetServiceToken:2.\r\n"));
		return E_ProofResult_GameNet_XML;
	}

	//根节点
	elementDataPtr = docPtr->documentElement;
	if(NULL == elementDataPtr)
	{
		docPtr.Release();
		CoUninitialize();
		ILOG->Write(_T("GetServiceToken:3.\r\n"));
		return E_ProofResult_GameNet_XML;
	}

	nodeListItemsPtr = elementDataPtr->GetchildNodes();
	if(NULL == nodeListItemsPtr)
	{
		elementDataPtr.Release();
		docPtr.Release();
		CoUninitialize();
		ILOG->Write(_T("GetServiceToken:4.\r\n"));
		return E_ProofResult_GameNet_XML;
	}

	nodeItemPtr = nodeListItemsPtr->nextNode();

	if(nodeItemPtr != NULL)
	{
		nodeItemPtr->get_nodeName(&bstrText);

		pszValue = OLE2T(bstrText);

		if(0 == _tcscmp(pszValue,_T("token")))
		{
			ILOG->Write(_T("GetServiceToken:7.\r\n"));
			nodeItemPtr->get_nodeTypedValue(&value);

			const TCHAR *ptszToken1 = NULL;
			ptszToken1 = OLE2T(value.bstrVal);
			ILOG->Write(_T("GetServiceToken:Token1 %s.\r\n"),ptszToken1);

			if( !P_VALID(ptszToken1) )
			{
				nodeItemPtr.Release();
				nodeListItemsPtr.Release();
				elementDataPtr.Release();
				docPtr.Release();
				CoUninitialize();
				return E_ProofResult_GameNet_XML;
			}

			TCHAR tszToken2[X_TOKEN] = {0};
			MultiByteToWideChar(CP_ACP,NULL,pszToken,-1,tszToken2,X_TOKEN);
			tszToken2[40] = _T('\0');
			ILOG->Write(_T("GetServiceToken :Token2 %s.\r\n"),tszToken2);

			if( 0 == _tcscmp( ptszToken1, tszToken2 ) )
			{
				nodeItemPtr.Release();
				nodeListItemsPtr.Release();
				elementDataPtr.Release();
				docPtr.Release();
				CoUninitialize();
				return E_Success;
			}
			else
			{
				nodeItemPtr.Release();
				nodeListItemsPtr.Release();
				elementDataPtr.Release();
				docPtr.Release();
				CoUninitialize();
				ILOG->Write(_T("GetServiceToken:5.\r\n"));
				return E_ProofResult_GameNet_Error_Token;
			}
		}
	}

	nodeItemPtr.Release();
	nodeListItemsPtr.Release();
	elementDataPtr.Release();
	docPtr.Release();;
	CoUninitialize();
	ILOG->Write(_T("GetServiceToken:6.\r\n"));
	return E_ProofResult_GameNet_XML;
}
//-------------------------------------------------------------------------------
DWORD GameNetWeb::CheckSubscribeStatus( LPCSTR pszToken )
{
	if( !P_VALID(pszToken) )
		return E_ProofResult_GameNet_CanNotConnect_WEB;

	TCHAR tszTemp[WEB_REQUEST_MAX] = {0};
	TCHAR tszToken[1024] = {0};
	MultiByteToWideChar(CP_ACP,NULL,pszToken,-1,tszToken,1024);
	tszToken[40] = _T('\0');
	wsprintf(tszTemp,_T("restapi?method=user.checkSubscribeStatus&serviceId=300003010000000000&authType=token&authId=%s"), tszToken ); 
	ILOG->Write(_T("CheckSubscribeStatus:%s.\r\n"),tszTemp);

	CHAR szBuffer[WEB_REQUEST_MAX] = {0};
	if( !SendRequset( tszTemp, szBuffer ) )
		return E_ProofResult_GameNet_CanNotConnect_WEB;

	TCHAR tszBuffer[WEB_REQUEST_MAX] = {0};
	MultiByteToWideChar(CP_ACP,NULL,szBuffer,-1,tszBuffer,WEB_REQUEST_MAX);
	ILOG->Write(_T("CheckSubscribeStatus return buff:%s.\r\n"),tszBuffer);

	//解析得到的xml文件
	HRESULT hr = 0;
	VARIANT_BOOL vResult;
	BSTR bstrText;
	const TCHAR	*pszValue = NULL;
	MSXML::IXMLDOMDocumentPtr	docPtr;
	MSXML::IXMLDOMElementPtr	elementDataPtr, elementItemPtr;
	MSXML::IXMLDOMNodeListPtr	nodeListItemsPtr;
	MSXML::IXMLDOMNodePtr		nodeItemPtr;
	VARIANT value;

	
	//CHAR	szToken[X_TOKEN] = {0};
	//CHAR	szStatus[256] = {0};

	CoInitialize(NULL);
	USES_CONVERSION;

	hr = docPtr.CreateInstance("MSXML.domdocument");
	if(FAILED(hr))
	{
		CoUninitialize();
		ILOG->Write(_T("CheckSubscribeStatus:1.\r\n"));
		return E_ProofResult_GameNet_XML;
	}

	vResult = docPtr->loadXML(_bstr_t(szBuffer));
	if( vResult != VARIANT_TRUE)
	{
		docPtr.Release();
		CoUninitialize();
		ILOG->Write(_T("CheckSubscribeStatus:2.\r\n"));
		return E_ProofResult_GameNet_XML;
	}

	//根节点
	elementDataPtr = docPtr->documentElement;
	if(NULL == elementDataPtr)
	{
		docPtr.Release();
		CoUninitialize();
		ILOG->Write(_T("CheckSubscribeStatus:3.\r\n"));
		return E_ProofResult_GameNet_XML;
	}

	nodeListItemsPtr = elementDataPtr->GetchildNodes();
	if(NULL == nodeListItemsPtr)
	{
		elementDataPtr.Release();
		docPtr.Release();
		CoUninitialize();
		ILOG->Write(_T("CheckSubscribeStatus:4.\r\n"));
		return E_ProofResult_GameNet_XML;
	}

	nodeItemPtr = nodeListItemsPtr->nextNode();

	if(nodeItemPtr != NULL)
	{
		nodeItemPtr->get_nodeName(&bstrText);

		pszValue = OLE2T(bstrText);

		if(0 == _tcscmp(pszValue,_T("status")))
		{
			//nodeItemPtr->get_nodeValue(&value);
			//DWORD dwState = value.intVal;
			//if( 8 == dwState & 8 )
			//	return E_ProofResult_GameNet_Block;
			//else if( 16 == dwState & 16 )
			//	return E_ProofResult_GameNet_Frozen;

			nodeItemPtr->get_nodeTypedValue(&value);
			TCHAR *ptszStatus = OLE2T(value.bstrVal);
			if( !P_VALID(ptszStatus) )
			{
				nodeItemPtr.Release();
				nodeListItemsPtr.Release();
				elementDataPtr.Release();
				docPtr.Release();
				CoUninitialize();
				return E_ProofResult_GameNet_XML;
			}

			if( 0 == _tcscmp(ptszStatus,STATUS_BLOCKED) )
			{
				nodeItemPtr.Release();
				nodeListItemsPtr.Release();
				elementDataPtr.Release();
				docPtr.Release();
				CoUninitialize();
				ILOG->Write(_T("CheckSubscribeStatus:5.\r\n"));
				return E_ProofResult_GameNet_Block;
			}
			if( 0 == _tcscmp(ptszStatus,STATUS_FROZEN) )
			{
				nodeItemPtr.Release();
				nodeListItemsPtr.Release();
				elementDataPtr.Release();
				docPtr.Release();
				CoUninitialize();
				ILOG->Write(_T("CheckSubscribeStatus:6.\r\n"));
				return E_ProofResult_GameNet_Frozen;
			}

			nodeItemPtr.Release();
			nodeListItemsPtr.Release();
			elementDataPtr.Release();
			docPtr.Release();
			CoUninitialize();
			return E_Success;
		}
	}

	nodeItemPtr.Release();
	nodeListItemsPtr.Release();
	elementDataPtr.Release();
	docPtr.Release();;
	CoUninitialize();
	ILOG->Write(_T("CheckSubscribeStatus:7.\r\n"));
	return E_ProofResult_GameNet_XML;
}
//-------------------------------------------------------------------------------


