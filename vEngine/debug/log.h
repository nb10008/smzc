//-----------------------------------------------------------------------------
// File: log
// Desc: Game Tool log 1.0
// Auth: Lyp
// Date: 2003/12/18
//-----------------------------------------------------------------------------
#pragma once
namespace vEngine {

class Thread;
//-----------------------------------------------------------------------------
// log
//-----------------------------------------------------------------------------
class VENGINE_API Log
{
public:

	BOOL Create(LPCTSTR szFileName=NULL);
	BOOL Write(LPCTSTR lpFormat, ...);
	VOID Close();
	Log();~Log();
	DWORD ThreadWriteLog(LPVOID);
private:
	TObjRef<DiscIOMgr>		m_pDiscIOMgr;	// 穿过了VFS系统
	DWORD					m_dwFileHandle;
	TCHAR					m_szDefaultName[1024];
	
	// Jason added 2010-10-23
	TSFPTrunk<Log>		m_Trunk;
	// 减少io次数
	std::vector<tstring>		m_vecLogs;
	CRITICAL_SECTION		m_cs;
	LONG					m_lTerminated;
	TCHAR					m_szBuf[10240];
	TObjRef<Thread>			m_pThread;

	VOID					DumpAllRemained(VOID);

	TCHAR					m_szLogThreadName[MAX_PATH];
	HANDLE	hThead;

	UINT					m_unFileSize;
};


//-----------------------------------------------------------------------------
// 专门提供给异常模块使用的log,外部应调用上面的log模块
//-----------------------------------------------------------------------------
class ExceptionLog
{
public:

	BOOL Create();
	BOOL Write(LPCSTR lpFormat, ...);
	VOID Close();
	LPCTSTR	GetDefaultFileName() { return m_szDefaultName; }

	ExceptionLog();~ExceptionLog();

private:
	TObjRef<DiscIOMgr>		m_pDiscIOMgr;	// 穿过了VFS系统
	DWORD					m_dwFileHandle;
	TCHAR					m_szDefaultName[1024];
};


} // namespace vEngine {
