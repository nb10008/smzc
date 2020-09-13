/* -------------------------------------------------------------------------
// 文件名		:	kksimpleloginhelper.cpp
// 创建人		:	chenghui
// 创建时间		:	2011-4-19 15:18:41
// 功能描述     :	
//
// $Id: $
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kksimpleloginhelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
#define KKGAME_INSTALL_REG	L"SOFTWARE\\KuaiKuai\\KKGame"
#define KKGAME_PATH_KEY		L"installdir"
#define KKGAME_EXE		L"\\kkgame.exe"
BOOL isKKGameInstalled(std::wstring& strInstallPath)
{
	BOOL bRet = FALSE;
	CRegKey regKey;
	LONG nRetCode =	regKey.Open(HKEY_LOCAL_MACHINE, KKGAME_INSTALL_REG, KEY_READ);
	if (ERROR_SUCCESS == nRetCode)
	{
		WCHAR wcsKKPath[MAX_PATH] = {0};
		ULONG ulRead = MAX_PATH;
		nRetCode = regKey.QueryStringValue(KKGAME_PATH_KEY, wcsKKPath, &ulRead);
		if (ERROR_SUCCESS == nRetCode)
		{
			std::wstring strTempPath;
			strTempPath = wcsKKPath;
			strTempPath += KKGAME_EXE;
			if (_waccess(strTempPath.c_str(), 0) == 0)
			{
				strInstallPath = wcsKKPath;
				bRet = TRUE;
			}
		}
	}

	return bRet;
}



// -------------------------------------------------------------------------
// $Log: $
