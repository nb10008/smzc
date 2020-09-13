#pragma once

#include "SDOADx9.h"
#include "SDOA4Client.h"

class SDOAIgw
{
public:
	~SDOAIgw();
	VOID InitSDOAIGW(DWORD groupID, DWORD worldID);
	VOID DestroySDOAIGW();
	VOID Render();
	BOOL OnMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	VOID LogOut();
	VOID ResetDevice();
	VOID LostDevice();
	BOOL OnLogin( int nErrorCode, const LoginResult *pLoginResult, int nUserData, int nReserved );
	BOOL GetSDOAFlag() { return m_bSDOA; }
	VOID LeaveLogIn();
	VOID SendRoleInfo( TCHAR *pName, int nSex );
	LoginResult GetSDOALoginResult() { return m_lrRelsult; }
	static SDOAIgw* Inst();

	BOOL IsInIGW();
	BOOL IsGetFocus();

	VOID SetShowLoginFlag(BOOL bFlag) 
	{ 
		m_bShowLogin = bFlag;
		if( m_bShowLogin )
			ZeroMemory( &m_lrRelsult, sizeof(LoginResult) );
	}

	VOID SetScreenStatus( int nValue )
	{
		if( m_pSDOAApp )
			m_pSDOAApp->SetScreenStatus(nValue);
	}

private:
	SDOAIgw();

private:
	HINSTANCE			m_hInstIGWDll;
	LPigwInitialize		m_igwInitialize;
	LPigwGetModule		m_igwGetModule;
	LPigwTerminal		m_igwTerminal;
	ISDOADx9			*m_pSDOADx9;
	ISDOAApp			*m_pSDOAApp;
	BOOL				m_bSDOA;
	BOOL				m_bShowLogin;
	LoginResult			m_lrRelsult;
	static SDOAIgw		*m_pSDOAIGW;

	class Cleaner
	{
	public:
		Cleaner()
		{

		}
		~Cleaner()
		{
			if( SDOAIgw::Inst() )
				delete SDOAIgw::Inst();
		}
	};
};