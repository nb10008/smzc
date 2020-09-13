// AutoLoginMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoLoginMFC.h"
#include "AutoLoginMFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAutoLoginMFCDlg dialog




CAutoLoginMFCDlg::CAutoLoginMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoLoginMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoLoginMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_pButt);
}

BEGIN_MESSAGE_MAP(CAutoLoginMFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CAutoLoginMFCDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAutoLoginMFCDlg message handlers

BOOL CAutoLoginMFCDlg::OnInitDialog()
{
	m_bisOpen = false;
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAutoLoginMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutoLoginMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutoLoginMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAutoLoginMFCDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	m_bisOpen = ! m_bisOpen;

	if(m_bisOpen)
	{
		SetTimer(1, 2000, NULL);
		m_pButt.SetWindowText(_T("停止监控"));
	}
	else
	{
		KillTimer(1);
		m_pButt.SetWindowText(_T("开始监控"));
	}
}

#include "tlhelp32.h"

int CAutoLoginMFCDlg::GetProcessConut(const TCHAR* szExeName)
{
	TCHAR sztarget[128];
	lstrcpy(sztarget, szExeName);
	CharLowerBuff(sztarget, 128);

	int count = 0;
	PROCESSENTRY32 my;
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if( ((int)handle) != -1 )
	{
		my.dwSize = sizeof(my);
		if(Process32First(handle, &my))
		{
			do 
			{
				CharLowerBuff(my.szExeFile, 128);
				if(lstrcmp(sztarget, my.szExeFile) == 0)
				{
					count++;
				}
			} 
			while (Process32Next(handle, &my));
		}
		CloseHandle(handle);

		return count;
	}
	return 0;
}

void CAutoLoginMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);


	const TCHAR* szProcessName = _T("LoginServer.exe");
	int nResult = GetProcessConut(szProcessName);
	if(nResult == 0)
	{
		//需要重启程序
		//WinExec("LoginServer.exe", SW_SHOWMAXIMIZED);
		WinExec("cmd.exe /k LoginServer.exe Debugger=d:\\autodump.bat", SW_HIDE);
		//ShellExecute(NULL, _T("open"), _T("LoginServer.exe"), _T("Debugger=d:\\autodump.bat"), NULL, SW_SHOW);
	}
}
