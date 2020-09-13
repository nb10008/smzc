#include "StdAfx.h"
#include "wx/wx.h"
#include "Loong_OutputCtrl.h"
#include "ServerMergeToolApp.h"

CLoongOutputCtrl g_clsOutput;

CLoongOutputCtrl::CLoongOutputCtrl(void):m_pOutputWnd(NULL)
{
	InitializeCriticalSection(&m_csOutput);	
}

CLoongOutputCtrl::~CLoongOutputCtrl(void)
{
	DeleteCriticalSection(&m_csOutput);	
}
VOID CLoongOutputCtrl::Init(LPVOID pOutputWnd)
{
	ASSERT(pOutputWnd);
	m_pOutputWnd = pOutputWnd;

	m_pUtil	= "Util";
}

VOID CLoongOutputCtrl::Output(LPCTSTR szStr, BOOL bOnlyLog)
{
	if(!P_VALID(m_pOutputWnd))
		return;

	EnterCriticalSection(&m_csOutput);

	//得到当前时间
// 	CHAR szTime[X_DATATIME_LEN + 1] = {0};
// 	DwordTime2DataTime(szTime, sizeof(szTime), GetCurrentDWORDTime());
 
// 	TCHAR szTTime[X_DATATIME_LEN + 1] = {0};
// 	_tcscpy(szTTime, m_pUtil->AnsiToUnicode(szTime));

// 	tstring strOutput = szTTime;
// 	strOutput += _T(" ");
// 	strOutput += szStr;

	tstring strOutput = szStr;

	if(!bOnlyLog)
	{
		wxTextPos pos = ((wxTextCtrl *)m_pOutputWnd)->GetLastPosition();
		((wxTextCtrl *)m_pOutputWnd)->Replace(pos,-1,strOutput.c_str());
	}

	//写日志
	ILOG->Write(strOutput.c_str());

	LeaveCriticalSection(&m_csOutput);
}