#include "stdafx.h"
#include "SDOAIgw.h"
#include <strsafe.h>
#include <oleauto.h>
#include "ClientMain.h"
#include "OwnerImeMgr.h"
#include "GameSet.h"

SDOAIgw* SDOAIgw::m_pSDOAIGW = NULL;
BOOL CALLBACK OnLoginCallback(int ErrorCode, const LoginResult *LoginResult, int nUserData, int Reserved );

SDOAIgw* SDOAIgw::Inst()
{
	if( m_pSDOAIGW == NULL )
	{
		TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
		TCHAR szTemp[X_LONG_NAME*2];
		_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
		tstring strAppPath = szTemp;

		tstring strIniFileFullName = strAppPath + szIniFileName;
		::GetPrivateProfileString(_T("Launcher"), _T("LoginMode"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
		tstring policy = szTemp;
		INT nLoginMode = _ttoi(policy.c_str());
		if( nLoginMode == 4 )
		{
			tstring szPath = g_strLocalPath + _T("\\..\\..\\IGW\\WidgetTest\\OAITTray.exe");
			ShellExecute(NULL, _T("open"), szPath.c_str(), NULL, NULL, SW_HIDE);
		}
		m_pSDOAIGW = new SDOAIgw();
		static Cleaner cTemp;
	}
	return m_pSDOAIGW;
}

SDOAIgw::SDOAIgw()
{
	ZeroMemory( &m_lrRelsult, sizeof(m_lrRelsult) );
	m_bShowLogin = FALSE;
	m_bSDOA = FALSE;
	m_hInstIGWDll = 0;
	m_igwInitialize = NULL;
	m_igwGetModule = NULL;
	m_igwTerminal = NULL;
	m_pSDOADx9 = NULL;
	m_pSDOAApp = NULL;
}

SDOAIgw::~SDOAIgw()
{
	DestroySDOAIGW();
}

VOID SDOAIgw::DestroySDOAIGW()
{
	if (m_pSDOADx9)
	{
		m_pSDOADx9->Finalize();
		m_pSDOADx9->Release();
		m_pSDOADx9 = NULL;
	}

	if (m_pSDOAApp)
	{
		m_pSDOAApp->Release();
		m_pSDOAApp = NULL;
	}

	if (m_igwTerminal)
	{
		m_igwTerminal();
		m_igwTerminal = NULL;
	}

	if (m_hInstIGWDll)
	{
		FreeLibrary(m_hInstIGWDll);
		m_hInstIGWDll = 0;
	}

	m_igwInitialize = NULL;
}

VOID SDOAIgw::InitSDOAIGW(DWORD groupID, DWORD worldID)
{
	if(m_bSDOA)
		return;
	m_bSDOA = TRUE;
	WCHAR strExePath[MAX_PATH] = {0};
	WCHAR strExeName[MAX_PATH] = {0};
	WCHAR* strLastSlash = NULL;
	GetModuleFileNameW( NULL, strExePath, MAX_PATH );
	strExePath[MAX_PATH-1]=0;
	strLastSlash = wcsrchr( strExePath, TEXT('\\') );
	if( strLastSlash )
	{
		StringCchCopyW( strExeName, MAX_PATH, &strLastSlash[1] );
		*strLastSlash = 0;
		strLastSlash = wcsrchr( strExeName, TEXT('.') );
		if( strLastSlash )
			*strLastSlash = 0;
	}

	WCHAR strGameWidgetDll[MAX_PATH] = {0};
	StringCchPrintfW( strGameWidgetDll, MAX_PATH, L"%s\\..\\data\\IGW\\GameWidget.dll", strExePath );

	DestroySDOAIGW();

	if ( GetFileAttributesW(strGameWidgetDll) != 0xFFFFFFFF )
	{
		m_hInstIGWDll = LoadLibraryW(strGameWidgetDll);
		if (m_hInstIGWDll)
		{
			m_igwInitialize = (LPigwInitialize)GetProcAddress( m_hInstIGWDll, "igwInitialize" );
			m_igwGetModule = (LPigwGetModule)GetProcAddress( m_hInstIGWDll, "igwGetModule" );
			m_igwTerminal = (LPigwTerminal)GetProcAddress( m_hInstIGWDll, "igwTerminal" );
			if ( m_igwInitialize && m_igwGetModule && m_igwTerminal)
			{
				const int MAX_STRING_SIZE = 512;
				TCHAR szVer[MAX_STRING_SIZE] = {0};
				::GetPrivateProfileString(_T("version"), _T("latest_game"), NULL,  szVer, MAX_STRING_SIZE, _T("launcher\\patchinfo.txt"));

				// 注意AppInfo类型所有字段必须填
				AppInfo tmpAppInfo = {
					sizeof(AppInfo),       // 结构体大小，方便扩展
					642,                  // 接入应用ID，从开发者网站中申请
					L"龙",     // 应用名称
					szVer,            // 应用客户端当前版本号
					SDOA_RENDERTYPE_D3D9,  // 客户端支持的图形引擎类型，这里同时支持d3d9和d3d8
					1,                     // 可同时在一台机器上游戏的最大人数（通常为1，例如:KOF应该是2）
					groupID,                    // 游戏区ID，不可用时传入-1
					worldID                     // 游戏组ID，不可用时传入-1
				};

				if (m_igwInitialize(SDOA_SDK_VERSION, &tmpAppInfo) == SDOA_OK)
				{
					m_igwGetModule(__uuidof(ISDOADx9), (void**)&m_pSDOADx9);
					m_igwGetModule(__uuidof(ISDOAApp), (void**)&m_pSDOAApp);

					if ( (!m_pSDOADx9) || (!m_pSDOAApp))
						DestroySDOAIGW();

				}
				else
					DestroySDOAIGW();
			}
			else
				DestroySDOAIGW();
		}
	}

	if (m_pSDOADx9)
	{
		IDirect3DDevice9* myDevice= Cool3D::Device()->GetD3DDevice();
		D3DPRESENT_PARAMETERS myD3DPP= Cool3D::Device()->GetD3DPresentParameters();
		m_pSDOADx9->Initialize( myDevice, &myD3DPP, FALSE );
	}
	if(m_pSDOAApp)
	{
		RECT rect;
		ZeroMemory( &rect, sizeof(RECT) );
		HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));
		::GetClientRect(hWnd, &rect);
		LONG x = rect.right - 50;
		LONG y = rect.top + rect.bottom / 5;
		POINT pos = { x, y };
		m_pSDOAApp->SetTaskBarPosition(&pos);
	}
}

VOID SDOAIgw::Render()
{
	if (m_pSDOADx9)
		m_pSDOADx9->RenderEx();

	if(m_pSDOAApp && m_bShowLogin )
	{
		m_pSDOAApp->ShowLoginDialog( &OnLoginCallback, 1, 0);
		OwnerImeMgr::Inst()->DisableOwnerIme();
	}
}

BOOL SDOAIgw::OnMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult; 
	if(m_pSDOADx9) 
	{
		//switch(uMsg)
		//{
		//case WM_IME_SETCONTEXT:
		//case WM_INPUTLANGCHANGEREQUEST:
		//case WM_INPUTLANGCHANGE:
		//case WM_IME_STARTCOMPOSITION:
		//case WM_IME_ENDCOMPOSITION:
		//case WM_IME_NOTIFY:
		//case WM_IME_COMPOSITION:
		//case WM_CHAR:
		//	return FALSE;;
		//}
		if( m_pSDOADx9->OnWindowProc( hWnd, uMsg, wParam, lParam, &lResult ) == SDOA_OK )
			return TRUE;
	}
	return FALSE;
}

BOOL SDOAIgw::OnLogin(int nErrorCode, const LoginResult *pLoginResult, int nUserData, int nReserved) 
{

	if( SDOA_ERRORCODE_OK == nErrorCode )
	{
		memcpy( &m_lrRelsult, pLoginResult, sizeof(LoginResult) );
		m_bShowLogin = FALSE;
		if(GameSet::Inst()->Get(EGS_Windowed) == 0)
			OwnerImeMgr::Inst()->EnableOwnerIme();
		return TRUE;
	}

	if( SDOA_ERRORCODE_CANCEL == nErrorCode )
	{
		TObjRef<Client>()->ExitThreadLoad();
		PostQuitMessage(0);
	}
	
	if (SDOA_ERRORCODE_SHOWMESSAGE == nErrorCode)
	{
		if (nReserved != 0)
		{
			LoginMessage* pCurLoginMsg = (LoginMessage*)nReserved;
			if (pCurLoginMsg->dwSize >= sizeof(LoginMessage))
			{
				SysReAllocString(pCurLoginMsg->pbstrTitle, L"[测试游戏错误提示]");
			}
		}
	}
	return FALSE;
} 

VOID SDOAIgw::LogOut()
{
	if(m_pSDOAApp)
		m_pSDOAApp->Logout();
}

VOID SDOAIgw::LeaveLogIn()
{
	ZeroMemory( &m_lrRelsult, sizeof(m_lrRelsult) );
	m_bShowLogin = FALSE;
}

VOID SDOAIgw::ResetDevice()
{
	if(m_pSDOADx9)
	{
		D3DPRESENT_PARAMETERS myD3DPP= Cool3D::Device()->GetD3DPresentParameters();
		m_pSDOADx9->OnDeviceReset ( &myD3DPP ) ;
	}
}

VOID SDOAIgw::LostDevice()
{
	if(m_pSDOADx9)
		m_pSDOADx9->OnDeviceLost();
}

BOOL CALLBACK OnLoginCallback(int ErrorCode, const LoginResult *LoginResult, int nUserData, int Reserved)
{
	return SDOAIgw::Inst()->OnLogin(ErrorCode, LoginResult, nUserData, Reserved);
}

VOID SDOAIgw::SendRoleInfo( TCHAR *pName, int nSex )
{
	if( m_pSDOAApp )
	{
		int nSexe = ( nSex == 1 ) ? 0 : 1;
		RoleInfo tmpRoleInfo = { sizeof(RoleInfo), pName, nSexe };
		m_pSDOAApp->SetRoleInfo( &tmpRoleInfo );
	}
}

BOOL SDOAIgw::IsInIGW()
{ 
	if( m_pSDOAApp )
	{
		POINT pos;
		GetCursorPos(&pos);
		HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));
		ScreenToClient( hWnd, &pos );
		if( m_pSDOAApp->GetFocus() || m_pSDOAApp->HasUI(&pos) )
			return TRUE;
	}
		
	return FALSE;
}

BOOL SDOAIgw::IsGetFocus()
{ 
	if( m_pSDOAApp )
	{
		if( m_pSDOAApp->GetFocus() )
			return TRUE;
	}
	return FALSE;
}