// launcherDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "launcher.h"
#include "launcherDlg.h"
#include "Resource.h"


#include "httpdown.h"
#include "process.h"

#ifdef ON_X_TRAP
#pragma comment(lib,"urlmon")
#pragma comment(lib,"wininet")
#include "urlmon.h"

#pragma comment(lib,"..\\XTrap\\20100306_0010_XTrap4Launcher_mt.lib")
#include "20100306_0010_Xtrap_L_Interface.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CLauncherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_BN_CLICKED(IDC_Update,	OnUpdate)
	ON_BN_CLICKED(IDC_Start,	OnStart)
	ON_BN_CLICKED(IDC_Setup,	OnSetup)
	ON_BN_CLICKED(IDC_Info,		OnInfo)
	ON_BN_CLICKED(IDC_Quit,		OnQuit)

	ON_BN_DOUBLECLICKED(IDC_Update,	OnUpdate)
	ON_BN_DOUBLECLICKED(IDC_Start,	OnStart)
	ON_BN_DOUBLECLICKED(IDC_Setup,	OnSetup)
	ON_BN_DOUBLECLICKED(IDC_Info,	OnInfo)
	ON_BN_DOUBLECLICKED(IDC_Quit,	OnQuit)


	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//  YY

HWND                tempWnd;

void ShowYYLog(LPCTSTR szText)
{
	TCHAR	m_szShowInfo[1024*32];
	_tcscpy(m_szShowInfo, szText);
	PostMessage(tempWnd, WM_USER_INFO, (WPARAM)m_szShowInfo, 0);
	PostMessage(tempWnd, WM_USER_LOG, (WPARAM)m_szShowInfo, 0);
}

void CallbackShowMessage( const char* szText )
{
	if( szText == NULL ) return;
	WCHAR wzMsgText[256]; 
	memset( wzMsgText, 0, sizeof(wzMsgText) );
	MultiByteToWideChar( CP_ACP, 0, szText, lstrlenA(szText), wzMsgText, sizeof(wzMsgText) ); 
	ShowYYLog(wzMsgText);
}

void CallbackShowState( int nCurSize, int nMaxSize, int nSpeed )
{
	WCHAR wzStateText[256]; 
	memset( wzStateText, 0, sizeof(wzStateText) );
	wsprintf( wzStateText, L"Cur=%d, Max=%d, Speed=%dk//s", nCurSize, nMaxSize, nSpeed/1024 );
	ShowYYLog(wzStateText);
}

void CallbackResult( int bUpdateRet )
{
	if( bUpdateRet == TRUE )
	{
		ShowYYLog(L"YYPIP:Update Succeed");
	}else
	{
		ShowYYLog(L"YYPIP:UpdateFailure");
	}
}

//-----------------------------------------------------------------------------
// CLauncherDlg 对话框
//-----------------------------------------------------------------------------
CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLauncherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pProcess = new Process;
}

CLauncherDlg::~CLauncherDlg()
{
	SAFE_DEL(m_pProcess);
	m_BackGround.DeleteObject();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


//-----------------------------------------------------------------------------
// CLauncherDlg 消息处理程序
//-----------------------------------------------------------------------------
BOOL CLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow(GetDesktopWindow());
	

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	RECT rect;		  
// 	rect.left = 53;
// 	rect.top = 160;
// 	rect.right = rect.left + 696;
// 	rect.bottom = rect.top + 395;			
	rect.left = 111 +3;
	rect.top = 323 +13;
	rect.right = rect.left +695;
	rect.bottom =  rect.top +230;			

	
	m_Web.Create(NULL, NULL, WS_VISIBLE,  rect, this, -1);
	m_Web.Navigate2(m_pProcess->GetURL(),NULL,NULL,NULL,NULL);
	m_Web.ShowWindow(FALSE);
	// 网页中加入<body scroll="no" >可以去掉滚动条
	// <BODY oncontextmenu="return false"> 可以去掉右键菜单
	//m_Web.Navigate2(_T("http://www.wokchina.com/ad/notes.htm"),NULL,NULL,NULL,NULL);
	
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, _T("launcher\\background.bmp"), IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	m_BackGround.Attach(bitmap);
	HRGN hRgn = BitmapToRegion(bitmap, RGB(255,0,255), 0);
	if( hRgn )
	{
		SetWindowRgn( hRgn , TRUE);
		::DeleteObject((HGDIOBJ)hRgn);
	}

	CreateControls();
	this->SetWindowText(g_StrTable[_T("AppName")]);

	// 开始更新
	if( !m_pProcess->Work(m_hWnd) )
	{
		_exit(0);	// 强行退出
		return FALSE;
	}
/*
	// 更新YY
	tempWnd = m_hWnd;

	TCHAR szIniFileName[512] = L"\\client\\YY\\pipFactory.dll";
	TCHAR szIniFileName2[512] = L"\\YY\\yy3.8.rar";
	const int MAX_STRING_SIZE = 512;
	TCHAR szTemp[512*2];

	// 获取EXE路径
	GetModuleFileName(NULL, szTemp, sizeof(szTemp)/sizeof(TCHAR));
	std::wstring ss = szTemp;
	int pos = ss.rfind(L"\\");
	ss.erase(ss.begin()+pos,ss.end());
	std::wstring m_strIniFileFullName = ss + szIniFileName;
	std::wstring m_strIniFileFullName2 = ss + szIniFileName2;

	HMODULE hDll = LoadLibrary( m_strIniFileFullName.c_str() );
	if( hDll == NULL ) return FALSE;

	typedef int (* PFN_UPDATAYYVOICE)( const char*, void*, void*, void* ) ;
	PFN_UPDATAYYVOICE pfnUpdate = (PFN_UPDATAYYVOICE)GetProcAddress( hDll, "YYPIP_StartUpdate" );
	if( pfnUpdate == NULL ) return FALSE;

	CHAR*	m_pCharPool;
	m_pCharPool = new CHAR[1024*64];
	WideCharToMultiByte(CP_ACP, 0, m_strIniFileFullName2.c_str(), -1, m_pCharPool, 1024*64, NULL, NULL);
	pfnUpdate(m_pCharPool, CallbackShowMessage, CallbackShowState, CallbackResult );

	SAFE_DEL_ARRAY(m_pCharPool);
*/
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//-----------------------------------------------------------------------------
// 如果向对话框添加最小化按钮，则需要下面的代码
// 来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
// 这将由框架自动完成。
//-----------------------------------------------------------------------------
void CLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
BOOL CLauncherDlg::OnEraseBkgnd(CDC* pDC) 
{
	CDC* pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(pDC);

	//保存旧对象
	CBitmap * pOldBitmap;
	pOldBitmap = pMemDC->SelectObject(&m_BackGround);

	pDC->BitBlt(0, 0, 1260, 758, pMemDC, 0, 0, SRCCOPY);
	delete pMemDC;

	return TRUE;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CLauncherDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ptMouse = point;  //保存鼠标左键按下是初始的位置，以后用来计算窗口位置。
	::SetCapture(m_hWnd);
	CWnd::OnLButtonDown(nFlags, point);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CLauncherDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CLauncherDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags&MK_LBUTTON)  //如果鼠标移动时左键是按下的。
	{
		CPoint ipoint;
		::GetCursorPos(&ipoint); //得到当前光标位置。
		SetWindowPos(NULL, ipoint.x - m_ptMouse.x, ipoint.y-m_ptMouse.y, 
			0, 0, SWP_NOSIZE | SWP_NOACTIVATE);//设置窗口新位置。
	}

	CWnd::OnMouseMove(nFlags, point);


}


//-----------------------------------------------------------------------------
// 当用户拖动最小化窗口时系统调用此函数取得光标显示。
//-----------------------------------------------------------------------------
HCURSOR CLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#define BUTTON_WIDTH	75
#define BUTTON_HEIGHT	22
#define BUTTON_INTER	100
//-----------------------------------------------------------------------------
// 创建各种控件
//-----------------------------------------------------------------------------
VOID CLauncherDlg::CreateControls()
{
	m_ButtonUpdate.SetNormalBitmap(_T("launcher\\button.bmp"));
	m_ButtonUpdate.SetSelBitmap(_T("launcher\\button_down.bmp"));
	m_ButtonUpdate.SetDisableBitmap(_T("launcher\\button_disable.bmp"));
	m_ButtonUpdate.SetHoverBitmap(_T("launcher\\button_light.bmp"));

	m_ButtonStart.SetNormalBitmap(_T("launcher\\button.bmp"));
	m_ButtonStart.SetSelBitmap(_T("launcher\\button_down.bmp"));
	m_ButtonStart.SetDisableBitmap(_T("launcher\\button_disable.bmp"));
	m_ButtonStart.SetHoverBitmap(_T("launcher\\button_light.bmp"));

	m_ButtonSetup.SetNormalBitmap(_T("launcher\\button.bmp"));
	m_ButtonSetup.SetSelBitmap(_T("launcher\\button_down.bmp"));
	m_ButtonSetup.SetDisableBitmap(_T("launcher\\button_disable.bmp"));
	m_ButtonSetup.SetHoverBitmap(_T("launcher\\button_light.bmp"));

	m_ButtonInfo.SetNormalBitmap(_T("launcher\\button.bmp"));
	m_ButtonInfo.SetSelBitmap(_T("launcher\\button_down.bmp"));
	m_ButtonInfo.SetDisableBitmap(_T("launcher\\button_disable.bmp"));
	m_ButtonInfo.SetHoverBitmap(_T("launcher\\button_light.bmp"));

	m_ButtonQuit.SetNormalBitmap(_T("launcher\\button.bmp"));
	m_ButtonQuit.SetSelBitmap(_T("launcher\\button_down.bmp"));
	m_ButtonQuit.SetDisableBitmap(_T("launcher\\button_disable.bmp"));
	m_ButtonQuit.SetHoverBitmap(_T("launcher\\button_light.bmp"));

	m_ProgressDownload.SetForceBmp(_T("launcher\\progress.bmp"));
	m_ProgressDownload.SetBackBmp(_T("launcher\\progress_back.bmp"));

	m_ButtonState.SetDisableBitmap(_T("launcher\\black.bmp"));
	m_ButtonState.SetTextLeft(TRUE);
	m_ButtonProgress.SetDisableBitmap(_T("launcher\\black.bmp"));
	
	m_ButtonGameVer.SetDisableBitmap(_T("launcher\\black.bmp"));		// 游戏版本
	m_ButtonGameVer.SetTextLeft(TRUE);
	m_ButtonLauncherVer.SetDisableBitmap(_T("launcher\\black.bmp"));	// launcher版本
	m_ButtonLauncherVer.SetTextLeft(TRUE);
	m_ButtonLatestGameVer.SetDisableBitmap(_T("launcher\\black.bmp"));		// 最新游戏版本
	m_ButtonLatestGameVer.SetTextLeft(TRUE);
	m_ButtonLatestLauncherVer.SetDisableBitmap(_T("launcher\\black.bmp"));	// 最新launcher版本
	m_ButtonLatestLauncherVer.SetTextLeft(TRUE);

	RECT rect;
// 	rect.left = 205;
// 	rect.top = 530+120;
	rect.left = 303 +3;
	rect.top = 632 +14;
	rect.right = rect.left + BUTTON_WIDTH;
	rect.bottom = rect.top + BUTTON_HEIGHT;
	m_ButtonStart.Create(g_StrTable[_T("StartGame")], WS_VISIBLE | WS_CHILD, rect, this, IDC_Start);
	m_ButtonStart.EnableWindow(FALSE);
	m_ButtonStart.ShowWindow(FALSE);
	
	rect.left += BUTTON_INTER;
	rect.right+= BUTTON_INTER;
	m_ButtonUpdate.Create(g_StrTable[_T("ManualUpdate")], WS_VISIBLE | WS_CHILD, rect, this, IDC_Update);
	m_ButtonUpdate.ShowWindow(FALSE);

	rect.left += BUTTON_INTER;
	rect.right+= BUTTON_INTER;
	m_ButtonSetup.Create(g_StrTable[_T("GameSetup")], WS_VISIBLE | WS_CHILD, rect, this, IDC_Setup);
	m_ButtonSetup.ShowWindow(FALSE);

	rect.left += BUTTON_INTER;
	rect.right+= BUTTON_INTER;
	m_ButtonInfo.Create(g_StrTable[_T("UpdateInfo")], WS_VISIBLE | WS_CHILD, rect, this, IDC_Info);
	m_ButtonInfo.ShowWindow(FALSE);

	rect.left += BUTTON_INTER;
	rect.right+= BUTTON_INTER;
	m_ButtonQuit.Create(g_StrTable[_T("Quit")], WS_VISIBLE | WS_CHILD, rect, this, IDC_Quit);
	m_ButtonQuit.ShowWindow(FALSE);


	//更新进度条
// 	rect.left = 40+20+5;
// 	rect.right = rect.left + 663;
// 	rect.top = 500+115;
// 	rect.bottom = rect.top + 11;
	rect.left = 30 + 3;
	rect.right = rect.left + 437;
	rect.top = 260 + 14;
	rect.bottom = rect.top + 18;
	m_ProgressDownload.Create(NULL, WS_VISIBLE, rect, this, -1);
	m_ProgressDownload.SetRange(0, 100);
	m_ProgressDownload.SetPos(0);

	// 文字说明
// 	rect.left = 44+20+2;
// 	rect.right = rect.left + 661;
// 	rect.top = 478+115;
// 	rect.bottom = rect.top + 16;
	rect.left = 110 +3;
	rect.right = rect.left + 586;
	rect.top = 582 +14;
	rect.bottom = rect.top + 16;
	m_ButtonState.Create(_T(""), WS_VISIBLE | WS_CHILD, rect, this, 1);	// 状态
	m_ButtonState.EnableWindow(FALSE);

// 	rect.left = 500;
// 	rect.right = rect.left + 200;
// 	rect.top = 478+115;
// 	rect.bottom = rect.top + 16;
	rect.left += 276;
	rect.right = rect.left + 100;
	m_ButtonProgress.Create(_T(""), WS_VISIBLE | WS_CHILD, rect, this, 1);	// 状态
	m_ButtonProgress.EnableWindow(FALSE);

// 	rect.left = 46+20;
// 	rect.right = rect.left + 180;
// 	rect.top = 452+115;
// 	rect.bottom = rect.top + 16;
	rect.left = 110 +3;
	rect.right = rect.left + 154;
	rect.top = 562 +14;
	rect.bottom = rect.top + 16;
	m_ButtonGameVer.Create(_T(""), WS_VISIBLE | WS_CHILD, rect, this, 1);	// 游戏版本
	m_ButtonGameVer.EnableWindow(FALSE);

// 	rect.left += 180;
// 	rect.right = rect.left + 150;
	rect.left += 154;
	rect.right = rect.left + 124;
	m_ButtonLatestGameVer.Create(_T(""), WS_VISIBLE | WS_CHILD, rect, this, 1);	// 最新游戏版本
	m_ButtonLatestGameVer.EnableWindow(FALSE);

// 	rect.left += 150;
// 	rect.right = rect.left + 180;
	rect.left += 124;
	rect.right = rect.left + 154;
	m_ButtonLauncherVer.Create(_T(""), WS_VISIBLE | WS_CHILD, rect, this, 1);	// launcher版本
	m_ButtonLauncherVer.EnableWindow(FALSE);


// 	rect.left += 180;
// 	rect.right = rect.left + 150;
	rect.left += 154;
	rect.right = rect.left + 154;
	m_ButtonLatestLauncherVer.Create(_T(""), WS_VISIBLE | WS_CHILD, rect, this, 1);	// 最新launcher版本
	m_ButtonLatestLauncherVer.EnableWindow(FALSE);

	m_DlgDownInfo.Create(IDD_DOWNLOADINFO, this);
	m_DlgConfig.Create(IDD_CONFIG, this);
	m_DlgConfig.Load(m_pProcess->GetCfgFileFullName());
}







//-----------------------------------------------------------------------------
// BitmapToRegion : Create a region from the "non-transparent" pixels of a bitmap
// Author :   Jean-Edouard Lachand-Robert (http://www.geocities.com/Paris/LeftBank/1160/resume.htm), June 1998.
//
// hBmp :    Source bitmap
// cTransparentColor : Color base for the "transparent" pixels (default is black)
// cTolerance :  Color tolerance for the "transparent" pixels.
//
// A pixel is assumed to be transparent if the value of each of its 3 components (blue, green and red) is 
// greater or equal to the corresponding value in cTransparentColor and is lower or equal to the 
// corresponding value in cTransparentColor + cTolerance.
//-----------------------------------------------------------------------------
#define ALLOC_UNIT 100
HRGN CLauncherDlg::BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
{
	HRGN hRgn = NULL;

	// Create a memory DC inside which we will scan the bitmap content
	HDC hMemDC = CreateCompatibleDC(NULL);
	if(!hMemDC)
		return NULL;

	// Get bitmap size
	BITMAP bm;
	GetObject(hBmp, sizeof(bm), &bm);

	// Create a 32 bits depth bitmap and select it into the memory DC 
	BITMAPINFOHEADER RGB32BITSBITMAPINFO = { 
		sizeof(BITMAPINFOHEADER), // biSize 
		bm.bmWidth,     // biWidth; 
		bm.bmHeight,    // biHeight; 
		1,       // biPlanes; 
		32,       // biBitCount 
		BI_RGB,      // biCompression; 
		0,       // biSizeImage; 
		0,       // biXPelsPerMeter; 
		0,       // biYPelsPerMeter; 
		0,       // biClrUsed; 
		0       // biClrImportant; 
	};
	VOID * pbits32; 
	HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
	if (hbm32)
	{
		HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

		// Create a DC just to copy the bitmap into the memory DC
		HDC hDC = CreateCompatibleDC(hMemDC);
		if (hDC)
		{
			// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
			BITMAP bm32;
			GetObject(hbm32, sizeof(bm32), &bm32);
			while (bm32.bmWidthBytes % 4)
				bm32.bmWidthBytes++;

			// Copy the bitmap into the memory DC
			HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
			BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

			// For better performances, we will use the ExtCreateRegion() function to create the
			// region. This function take a RGNDATA structure on entry. We will add rectangles by
			// amount of ALLOC_UNIT number in this structure.

			DWORD maxRects = ALLOC_UNIT;
			HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
			RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
			pData->rdh.dwSize = sizeof(RGNDATAHEADER);
			pData->rdh.iType = RDH_RECTANGLES;
			pData->rdh.nCount = pData->rdh.nRgnSize = 0;
			SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

			// Keep on hand highest and lowest values for the "transparent" pixels
			BYTE lr = GetRValue(cTransparentColor);
			BYTE lg = GetGValue(cTransparentColor);
			BYTE lb = GetBValue(cTransparentColor);
			BYTE hr = min(0xff, lr + GetRValue(cTolerance));
			BYTE hg = min(0xff, lg + GetGValue(cTolerance));
			BYTE hb = min(0xff, lb + GetBValue(cTolerance));

			// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
			BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;

			for (int y = 0; y < bm.bmHeight; y++)
			{
				// Scan each bitmap pixel from left to right
				for (int x = 0; x < bm.bmWidth; x++)
				{
					// Search for a continuous range of "non transparent pixels"
					int x0 = x;
					LONG *p = (LONG *)p32 + x;
					while (x < bm.bmWidth)
					{
						BYTE b = GetRValue(*p);
						if (b >= lr && b <= hr)
						{
							b = GetGValue(*p);
							if (b >= lg && b <= hg)
							{
								b = GetBValue(*p);
								if (b >= lb && b <= hb)
									// This pixel is "transparent"
									break;
							}
						}
						p++;
						x++;
					}

					if (x > x0)
					{
						// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
						if (pData->rdh.nCount >= maxRects)
						{
							GlobalUnlock(hData);
							maxRects += ALLOC_UNIT;
							hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
							pData = (RGNDATA *)GlobalLock(hData);
						}
						RECT *pr = (RECT *)&pData->Buffer;
						SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
						if (x0 < pData->rdh.rcBound.left)
							pData->rdh.rcBound.left = x0;
						if (y < pData->rdh.rcBound.top)
							pData->rdh.rcBound.top = y;
						if (x > pData->rdh.rcBound.right)
							pData->rdh.rcBound.right = x;
						if (y+1 > pData->rdh.rcBound.bottom)
							pData->rdh.rcBound.bottom = y+1;
						pData->rdh.nCount++;

						// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
						// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
						if (pData->rdh.nCount == 2000)
						{
							HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
							if (hRgn)
							{
								CombineRgn(hRgn, hRgn, h, RGN_OR);
								DeleteObject(h);
							}
							else
								hRgn = h;
							pData->rdh.nCount = 0;
							SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
						}
					}
				}

				// Go to next row (remember, the bitmap is inverted vertically)
				p32 -= bm32.bmWidthBytes;
			}

			// Create or extend the region with the remaining rectangles
			HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
			if (hRgn)
			{
				CombineRgn(hRgn, hRgn, h, RGN_OR);
				DeleteObject(h);
			}
			else
				hRgn = h;

			// Clean up
			GlobalFree(hData);
			SelectObject(hDC, holdBmp);
			DeleteDC(hDC);
		}

		DeleteObject(SelectObject(hMemDC, holdBmp));
	}

	DeleteDC(hMemDC);



	return hRgn;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CLauncherDlg::OnUpdate()
{
	// 打开官方手动补丁下载网页
	CString strURL = m_pProcess->GetManualURL();
	ShellExecute(NULL, _T("open"), _T("IEXPLORE"), strURL, NULL, SW_MAXIMIZE);

	//退出本更新程序
	m_pProcess->Terminate();
	m_pProcess->WaitForEnd(INFINITE);
	DestroyWindow();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CLauncherDlg::OnStart()
{
	EnterGame();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CLauncherDlg::OnSetup()
{
	BOOL bShow = m_DlgConfig.ShowWindow(SW_HIDE);
	if( !bShow )
		m_DlgConfig.ShowWindow(SW_SHOW);

}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CLauncherDlg::OnInfo()
{
	BOOL bShow = m_DlgDownInfo.ShowWindow(SW_HIDE);
	if( !bShow )
		m_DlgDownInfo.ShowWindow(SW_SHOW);

}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CLauncherDlg::OnQuit()
{
	m_pProcess->Terminate();
	m_pProcess->WaitForEnd(INFINITE);
	DestroyWindow();
}




//-----------------------------------------------------------------------------------------------
//消息处理
//-----------------------------------------------------------------------------------------------
BOOL CLauncherDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}


//-----------------------------------------------------------------------------------------------
// 消息处理
//-----------------------------------------------------------------------------------------------
LRESULT CLauncherDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_USER_INFO:
		{
			CString str;
			str.Format(_T("%s"), wParam);
			m_ButtonState.SetWindowText(str);
			m_ButtonProgress.SetWindowText(_T(""));
		}
		break;

	case WM_USER_LOG:
		{
			CString str;
			str.Format(_T("%s"), wParam);
			m_DlgDownInfo.m_ctrlEdit.SetSel(-1,-1);
			m_DlgDownInfo.m_ctrlEdit.ReplaceSel(str);
		}
		break;

	case WM_USER_PROGRESS_RANGE:
		m_ProgressDownload.SetRange(0, (INT)wParam);
		break;

	case WM_USER_PROGRESS_POS:
		m_ProgressDownload.SetPos((INT)wParam);
		break;

	case WM_USER_DOWNLOAD_SPEED:
		{
			CString str;
			str.Format(g_StrTable[_T("DownloadProgress")], (FLOAT)wParam/1024, (FLOAT)lParam/1024);
			m_ButtonProgress.SetWindowText(str);
		}
		break;

	case WM_USER_CAN_UPDATE:
		m_ButtonUpdate.EnableWindow(TRUE);
		break;

	case WM_USER_CAN_RUN_GAME:
		//m_ButtonStart.EnableWindow(TRUE);
		EnterGame();
		break;

	case WM_USER_GAME_VER:
		{
			CString str;
			str.Format(g_StrTable[_T("GameVersion")], wParam);
			m_ButtonGameVer.SetWindowText(str);
		}
		break;

	case WM_USER_LAUNCHER_VER:
		{
			CString str;
			str.Format(g_StrTable[_T("LauncherVersion")], wParam);
			m_ButtonLauncherVer.SetWindowText(str);
		}
		break;

	case WM_USER_LATEST_GAME_VER:
		{
			CString str;
			str.Format(g_StrTable[_T("LatestGameVersion")], wParam);
			m_ButtonLatestGameVer.SetWindowText(str);
		}
		break;

	case WM_USER_LATEST_LAUNCHER_VER:
		{
			CString str;
			str.Format(g_StrTable[_T("LatestLauncherVersion")], wParam);
			m_ButtonLatestLauncherVer.SetWindowText(str);
		}
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


//-----------------------------------------------------------------------------------------------
// 进入游戏
//-----------------------------------------------------------------------------------------------
VOID CLauncherDlg::EnterGame()
{
	CString strName = m_pProcess->GetGameFullName();
	CString strPath = m_pProcess->GetAppPath();
#ifdef ON_X_TRAP
	TCHAR szIniFileName[512] = _T("config\\config.ini");
	const int MAX_STRING_SIZE = 512;
	TCHAR szTemp[512*2];

	// 获取EXE路径
	GetModuleFileName(NULL, szTemp, sizeof(szTemp)/sizeof(TCHAR));
	TCHAR *pFind = ::_tcsrchr(szTemp, _T('\\'));
	if( pFind )
		*(pFind+1) = 0;
	_tcslwr_s(szTemp, sizeof(szTemp)/sizeof(TCHAR));
	tstring m_strAppPath = szTemp;

	tstring m_strIniFileFullName = m_strAppPath + szIniFileName;

	::GetPrivateProfileString(_T("Config"), _T("cfgname"), NULL,  szTemp, 512*2, m_strIniFileFullName.c_str());
	char buf[1024]={0};
	WideCharToMultiByte(CP_ACP, 0, szTemp, wcslen(szTemp), buf, 1024, NULL, NULL);
	//CHAR szXTrapArg[] = "660970B42839D2D9C5246D9844CFE86289B2F349E4DBAEA0EEDB3052EF1033ED0A15908290059B721DE54D9A4A98B89395F265292EB6296A835858306C5DA82A0F7D04245246E414A86D1910D45D7043924B3DF031D6A1CD4482DDF8889C7EC155";
	XTrap_L_Patch(buf,"client\\",60);
#endif

	ShellExecute(NULL, _T("open"), strName, AfxGetApp()->m_lpCmdLine, strPath, SW_SHOWNORMAL);

	//退出本更新程序
	m_pProcess->Terminate();
	m_pProcess->WaitForEnd(INFINITE);
	DestroyWindow();
}