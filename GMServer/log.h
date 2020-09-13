//-----------------------------------------------------------------------------
//!\file	log.h
//!\brief	记Log的类
//!
//!\date	2010-2-5
//! last	2010-2-5
//!\author	zhangrong
//!
//-----------------------------------------------------------------------------
#include <stdio.h>

class CLog
{
	FILE*		m_pLogFile;
	TCHAR		m_tszFilePath[X_LONG_STRING];

public:
	CLog()
	{
		m_pLogFile = NULL;

		// 获得当前可执行文件运行的路径
		ZeroMemory(m_tszFilePath, X_LONG_STRING);
		GetModuleFileName(NULL, m_tszFilePath, X_LONG_STRING);
		int nCount = _tcslen(m_tszFilePath);
		while( m_tszFilePath[nCount-1] != '\\' )
		{
			nCount--;
		}
		m_tszFilePath[nCount] = '\0';

		// 生成文件名
		tagDWORDTime now = GetCurrentDWORDTime();
		TCHAR szFileName[50] = {0};
		_sntprintf( szFileName, 50, _T("gmserver_log_20%d%d%d.txt"), now.year, now.month, now.day);

		// 组装成带有完整路径的文件名
		_tcscat_s(m_tszFilePath, szFileName);

		// 创建文件
		m_pLogFile = _tfopen(m_tszFilePath, _T("w+"));
		if (!P_VALID(m_pLogFile))
		{
			_tprintf(_T("Log Writer Init Failed, No Log Message Will be Saved.\n"));
		}

		fclose(m_pLogFile);
	}

	~CLog()
	{
		if (P_VALID(m_pLogFile))
		{
			fclose(m_pLogFile);
		}
	}

	VOID Write(LPCTSTR szContent)
	{
		m_pLogFile = _tfopen(m_tszFilePath, _T("at+"));
		
		if (P_VALID(m_pLogFile))
		{
			//BYTE byLen = _tcslen(szContent);
			tagDWORDTime now = GetCurrentDWORDTime();
			TCHAR szLogContent[X_LONG_STRING] = {0};
			_sntprintf(szLogContent, X_LONG_STRING, _T("\r\n[%d:%d]: %s"), now.hour, now.min, szContent);
			fputws(szLogContent, m_pLogFile);
		}

		fclose(m_pLogFile);
	}
};

extern CLog g_Log;