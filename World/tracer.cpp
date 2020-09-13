#include "stdafx.h"

#include "tracer.h"
#include "OutputDebugStringFack.h"

Tracer& Tracer::getInst()
{
	static Tracer _inst;
	return _inst;
}

Tracer::Tracer(): m_bOn(FALSE)
{
}

void Tracer::Trace(const CHAR* pFromat,...)
{
	if(m_bOn)
	{
		char* buffer;
		va_list vl;
		int len;

		va_start(vl, pFromat);
		len = _vscprintf(pFromat, vl) + 3;
		buffer = (char*)malloc(len * sizeof(char));
		vsprintf_s(buffer, len,  pFromat, vl);
		va_end(vl);
		buffer[len-3] = buffer [len-2] = buffer [len-1] = 0;
		strcat_s(buffer, len, "\r\n");
		OutputDebugStringA(buffer);
		free(buffer);
	}
}

void Tracer::Trace(const WCHAR* pFromat,...)
{
	if(m_bOn)
	{
		wchar_t* buffer;
		va_list vl;
		int len;

		va_start(vl, pFromat);
		len = _vscwprintf(pFromat, vl) + 3;
		buffer = (wchar_t*)malloc(len * sizeof(wchar_t));
		vswprintf_s(buffer, len, pFromat, vl);
		va_end(vl);
		buffer[len-3] = buffer [len-2] = buffer [len-1] = 0;
		wcscat_s(buffer, len, L"\r\n");
		OutputDebugStringW(buffer);
		free(buffer);
	}
}

DWORD WINAPI TurnOnSysTrace(LPCTSTR szOn)
{
	Tracer::getInst().TurnOn((BOOL)_ttoi(szOn));
	return 0;
}

void RegisterSysTracerCmd()
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("trace"), TurnOnSysTrace, _T("turn on/off trace"), 1);
}