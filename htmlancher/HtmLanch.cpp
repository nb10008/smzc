// HtmLanch.cpp : Implementation of CHtmLanch
#include "stdafx.h"
#include "HtmLanch.h"


// CHtmLanch
static HRESULT sLanch(BSTR sAppName, BSTR sParam)
{
	if( ::ShellExecute(NULL,_T("open"),COLE2T(sAppName),COLE2T(sParam),NULL,SW_SHOW) > (HINSTANCE)32 )
		return S_OK;
	DWORD error = ::GetLastError();
	LPVOID lpMsgBuf;
	lpMsgBuf = NULL;
	TCHAR szBuf[1024];
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM 
		| FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf, 0, NULL);
	_stprintf_s(szBuf,_countof(szBuf), _T("errorcode:%lu info:%s"), error, (CONST PTCHAR)lpMsgBuf);
	LocalFree( lpMsgBuf );
	::MessageBox(NULL, szBuf, _T(" Message:"), MB_ICONWARNING | MB_OK | MB_DEFBUTTON1);
	return S_FALSE;
}

STDMETHODIMP CHtmLanch::Lanch(BSTR sApp, BSTR sParam)
{
	return sLanch(sApp,sParam);
}
