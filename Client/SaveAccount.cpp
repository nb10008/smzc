#include "stdafx.h"
#include "SaveAccount.h"
#include "..\WorldDefine\login.h"

SaveAccount::SaveAccount( void )
{
	m_nMusic		= 0;
	m_bGoBack = false;
	m_nCurLoginMode = 0;
	m_dwCropid =0;
	m_yyGUID.clear();
	m_jskkPm.clear();
	m_bIsYYlogin = false;
	m_bIsOpenPassPort = FALSE;
	m_bIsNeedSetTelNum = TRUE;	//默认设置了
}

SaveAccount::~SaveAccount( void )
{

}

SaveAccount g_SaveAccount;
SaveAccount* SaveAccount::Inst()
{
	return &g_SaveAccount;
}

void SaveAccount::SetAccount( const TCHAR* szAccount )
{
	m_account = szAccount;
}

DWORD SaveAccount::GetCropID(  bool bCommandLine /*= TRUE */)
{
	if (bCommandLine)
	{
		// JSKK
		char  cmdLine[1024] = {0};
		if(HaveCommandLine(cmdLine))
			return m_dwCropid;
	}	
	return m_dwCropid;
}

DWORD SaveAccount::GetYYCropID()
{
	if(m_bIsYYlogin)
		return ECT_YYYuYinPingTai;
	else
		return 0;
}
bool SaveAccount::HaveCommandLine(char* cmd)
{
	string strCMDLine = TObjRef<Util>()->UnicodeToAnsi( GetCommandLine() );
	if( strCMDLine.empty() || strCMDLine.length() > 1024 )
		return false;

	const char *pCommandLine = strstr( strCMDLine.c_str(), ".exe" );
	if( pCommandLine == NULL )
	{
		return false;
	}
	else
	{
		strcpy( cmd, pCommandLine );
		if( !strlen( cmd))
			return false;

		char *pPart = strtok( cmd, "-" );
		pPart = strtok( NULL, " " );
		if(NULL == pPart)
			return false;

		if (strcmp( pPart, "fromkk" ) == 0)
		{
			pPart = strtok( NULL, " " );
			if (strcmp( pPart, "-channel" ) == 0)
			{
				pPart = strtok( NULL, " " );
				m_jskkPm = pPart;
			}
			return true;
		}
	}

	return false;
}

// open passport相关 
VOID  SaveAccount::SetChargeUrl(string  str)
{
	m_ChargeUrl = str;
}

const std::vector<DWORD>&  SaveAccount::GetCropIDInfo( )
{
	m_vecCropIDInfo.clear();

	TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\weblogin.ini");
	TCHAR szTemp[X_LONG_NAME*2];

	_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
	tstring strAppPath = szTemp;

	tstring strIniFileFullName = strAppPath + szIniFileName;
	::GetPrivateProfileString(_T("WebLogin"), _T("CropIDNum"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());

	if( szTemp != _T("") ||  szTemp != _T("-1")  ||  szTemp != _T("0")  )
	{
		INT nNum = 0;
		tstringstream tss;
		tss<<szTemp;
		tss>>nNum;
		for ( INT i=0; i<nNum; ++i )
		{
			tstringstream tssIDname;
			tssIDname<<_T("CropID")<<i+1;
			TCHAR szID [X_LONG_NAME*2];
			::GetPrivateProfileString(_T("WebLogin"), tssIDname.str().c_str() , NULL,  szID , X_LONG_NAME*2, strIniFileFullName.c_str());
			DWORD dwID = 0;
			tstringstream tssID ;
			tssID<<szID;
			tssID>>dwID;
			m_vecCropIDInfo.push_back(dwID);
		}
	}

	return m_vecCropIDInfo;

}