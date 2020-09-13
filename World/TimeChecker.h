#pragma once
#include "mmsystem.h"
#include "world.h"

#ifdef _UNICODE
#define WIDEN2(x) L##x
#else
#define WIDEN2(x) x
#endif
#define WIDEN(x) WIDEN2(x)

//#define CHECK_TIME()	
//#define CHECK_TMIE_WITH_TIMEOUT(n)	
//#define CHECK_TIME_WITH_INFO(a)
//#define CHECK_TIME_MAP(m)

#define CHECK_TIME()				TimeChecker _check(WIDEN(__FILE__), __LINE__, WIDEN(__FUNCTION__));
#define CHECK_TMIE_WITH_TIMEOUT(n)	TimeChecker _check(WIDEN(__FILE__), __LINE__, WIDEN(__FUNCTION__), NULL, n);
#define CHECK_TIME_WITH_INFO(a)		TimeChecker _check(WIDEN(__FILE__), __LINE__, WIDEN(__FUNCTION__), a);
#define CHECK_TIME_MAP(m)			TimeChecker _check(WIDEN(__FILE__), __LINE__, m);
// Jason 2010/8/20
#define CHECK_TIME_WITH_ALLPARAM( info, timeout )	TimeChecker _check(WIDEN(__FILE__), __LINE__, WIDEN(__FUNCTION__), (info), (timeout) );
#define CHECK_SPEC_BEGIN( info,timeout )	{ CHECK_TIME_WITH_ALLPARAM(info,timeout)
#define CHECK_SPEC_END }
#define CHECK_TIME_WITH_ALLPARAM_EX( info, timeout, screenshow )	TimeChecker _check(WIDEN(__FILE__), __LINE__, WIDEN(__FUNCTION__), (info), (timeout) );_check.ConsoleShow(screenshow);
#define CHECK_SPEC_BEGIN_EX( info,timeout, screenshow )	{ CHECK_TIME_WITH_ALLPARAM_EX(info,timeout,screenshow)

class TimeChecker
{
	class a2w
	{
		wchar_t* buffer;
	public:
		explicit a2w(const char* str):buffer(0)
		{
			if(str) {
				size_t nu = strlen(str);
				size_t n = ::MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),NULL,0);
				buffer = new wchar_t[n+1];
				::MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),buffer,int(n));
				buffer[n] = 0;
			}
		}
		~a2w() {  delete[] buffer;  }

		operator const wchar_t*() { return buffer; }

	}; 

	const TCHAR* m_pFile;
	int m_nLine;
	const TCHAR* m_pFuncName;
	const CHAR* m_pInfo;
	DWORD m_dwStartTime; 
	DWORD m_dTimeOut;
	BOOL	m_bConsoleShow;
	static TObjRef<Log> s_logger;
	static void Init();
public:
	TimeChecker(const TCHAR * pFile, int line, const TCHAR* pFunc, const CHAR* pInfo = NULL, DWORD dTimeOut = TICK_TIME)
	{
		m_pFile  = pFile;
		m_nLine = line;
		m_pFuncName = pFunc;
		m_pInfo = pInfo;
		m_dwStartTime =timeGetTime();
		m_dTimeOut = dTimeOut;
		m_bConsoleShow = FALSE;
	}

	VOID ConsoleShow(BOOL b){m_bConsoleShow = b;}
	BOOL ConsoleShow(VOID){return m_bConsoleShow;}

	~TimeChecker()
	{
		if( !P_VALID(s_logger) )
			return;
		DWORD usedTime = timeGetTime() - m_dwStartTime;
		if( usedTime >= m_dTimeOut )
		{
			Init();
			a2w a(m_pInfo?m_pInfo: "NULL"); 
			s_logger->Write(_T("TimeCheck too much time used. File: %s Line: %d Function: %s TimeUsed: %u, Info:%s\r\n"), m_pFile, m_nLine, m_pFuncName, usedTime, (const wchar_t*)a);
			if( m_bConsoleShow )
				IMSG(_T("Timeout Opt.in File: %s Line: %d Function: %s TimeUsed: %u, Info:%s\r\n"), m_pFile, m_nLine, m_pFuncName, usedTime, (const wchar_t*)a);
		}
	}
};

// 纯为调试使用，请勿在对外版本中使用 [6/29/2012 zhangzhihua]

#define DEBUG_TIME_CHECK				DebugTimeCheck _DebugCheck;
#define DEBUG_TIME_RECORD				_DebugCheck.CheckExecTime(WIDEN(__FUNCTION__), __LINE__);

class DebugTimeCheck
{
public:
	DebugTimeCheck()
	{
		QueryPerformanceFrequency(&m_nFreq);
		QueryPerformanceCounter(&m_nExecStart);
	}
	~DebugTimeCheck()
	{
	}

	VOID			CheckExecTime(const TCHAR* pFunc, int line)
	{
		LARGE_INTEGER	nUsedTime;
		QueryPerformanceCounter(&nUsedTime);
		nUsedTime.QuadPart = nUsedTime.QuadPart - m_nExecStart.QuadPart;

		ILOGNETANALYZE->Write(_T("DebugTimeCheck=Function: %s: %d	%f\r\n"), pFunc, line, (float)nUsedTime.QuadPart / (float)m_nFreq.QuadPart);
		QueryPerformanceCounter(&m_nExecStart);
	}

private:
	LARGE_INTEGER	m_nFreq;
	LARGE_INTEGER	m_nExecStart;
};