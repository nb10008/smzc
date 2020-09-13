#pragma once



class CLoongOutputCtrl
{
public:
	CLoongOutputCtrl(void);
	~CLoongOutputCtrl(void);
public:
	VOID Init(LPVOID pOutputWnd);

	VOID Output(LPCTSTR szStr, BOOL bOnlyLog = FALSE); //¥Ú”°–≈œ¢
private:
	TObjRef<Util> m_pUtil;

	LPVOID m_pOutputWnd;

	CRITICAL_SECTION m_csOutput;

};
extern CLoongOutputCtrl g_clsOutput;

