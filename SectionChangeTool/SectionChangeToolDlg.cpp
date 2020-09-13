// SectionChangeToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SectionChangeTool.h"
//#include "SectionChangeToolDlg.h"
#include "tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSectionChangeToolDlg* g_pDlg;

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


// CSectionChangeToolDlg dialog




CSectionChangeToolDlg::CSectionChangeToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSectionChangeToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pTool = &g_Tool;
}

void CSectionChangeToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ProcessListBox);
	DDX_Control(pDX, IDC_EDIT1, m_XmlPathEdit);
}

BEGIN_MESSAGE_MAP(CSectionChangeToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT1, &CSectionChangeToolDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CSectionChangeToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSectionChangeToolDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CSectionChangeToolDlg message handlers

BOOL CSectionChangeToolDlg::OnInitDialog()
{
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
	vEngine::InitNetwork();
	vEngine::InitEngine();
	
	m_pUtil	= "Util";

	m_szLineTemp = m_pUtil->Unicode8ToUnicode("Please select role information file!");//, szLineTemp);
	m_ProcessListBox.AddString(m_szLineTemp);

	g_Tool.Init();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSectionChangeToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSectionChangeToolDlg::OnPaint()
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
HCURSOR CSectionChangeToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSectionChangeToolDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CSectionChangeToolDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if ( m_XmlPath.IsEmpty() )
	{
		m_szLineTemp = m_pUtil->Unicode8ToUnicode("Please select role information file!");
		m_ProcessListBox.AddString(m_szLineTemp);
	}
	else{

		TCHAR szTmp[X_LONG_STRING] = {0};
		_tcscpy(szTmp, m_XmlPath);

		m_pTool->Control( EO_XMLSelected, 1, szTmp );

		if ( m_pTool->Control( EO_Merge, 1, NULL ) )
		{
			m_szLineTemp = m_pUtil->Unicode8ToUnicode("Merge Done!");

			m_ProcessListBox.ResetContent();
			m_ProcessListBox.AddString(m_szLineTemp);
		}
		else
		{
			m_szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to merge!");
			m_ProcessListBox.AddString(m_szLineTemp);
		}
	}	
}

void CSectionChangeToolDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, 
		_T("角色属性文件（*.xml）|*.xml||"));


	if ( IDOK == fileDlg.DoModal() )
	{
		CString filename = fileDlg.GetPathName();
		m_XmlPath = filename;

		m_XmlPathEdit.SetWindowText( m_XmlPath );

		m_ProcessListBox.ResetContent();

		m_szLineTemp = m_pUtil->Unicode8ToUnicode("Ready!");
		m_ProcessListBox.AddString(m_szLineTemp);
	}
}
