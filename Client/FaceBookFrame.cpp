#include "StdAfx.h"
#include "FaceBookFrame.h"
#include "SaveAccount.h"
#include "ClientConfigInCpk.h"

FaceBookFrame::FaceBookFrame(void):m_Trunk(this)
{
}

FaceBookFrame::~FaceBookFrame(void)
{
}

BOOL FaceBookFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL ret = GameFrame::Init(pMgr,pFather,dwParam);

	return ret;
}
BOOL FaceBookFrame::Destroy()
{
	BOOL ret = GameFrame::Destroy();

	if( P_VALID(m_pWnd) )
	{
		m_pGUI->DestroyWnd( m_pWnd );
		m_pWnd = NULL;
	}

	return ret;
}
BOOL FaceBookFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\facebook_connect.xml");
	if(!m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str()))
		return FALSE;

	// create the window
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&FaceBookFrame::EventHandler));

	m_pBnClose = (GUIButton*)m_pWnd->GetChild( _T("facebook_pic\\closebutton") );
	m_pBnFaceBook = (GUIButton*)m_pWnd->GetChild( _T("facebook_pic\\connectbutton") );	
	return TRUE;
}

BOOL FaceBookFrame::EscCancel()
{
	m_pFrameMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL FaceBookFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		if( pWnd == m_pBnClose )
		{
			m_pFrameMgr->AddToDestroyList(this);
		}
		if( pWnd == m_pBnFaceBook )
		{
			tstring FACE_BOOK;
			if (0 == ClientConfigInCpk::Inst()->GetFaceBookMode())
			{
				FACE_BOOK = GetURL_Taiwan();
			}
			else if (1 == ClientConfigInCpk::Inst()->GetFaceBookMode())
			{
				FACE_BOOK = GetURL_HongKong();
			}
			ShellExecute(NULL, _T("open"), FACE_BOOK.c_str(), NULL, NULL, SW_MAXIMIZE);
		}
		break;
	default:
		break;

	}
	return TRUE;
}

tstring	FaceBookFrame::GetURL_Taiwan()
{
	TCHAR szPath[1024];
	tstring FACE_BOOK = g_StrTable[_T("facebook_URL")];
 	tstring Key = _T("g4ai65vm3p");
 	tstring pid =SaveAccount::Inst()->GetAccount();
	DWORD dwPid = SaveAccount::Inst()->GetAccountID();
 	tstring severid = SaveAccount::Inst()->GetWorldID();
 	tstring datatime =_T("");
 	tagDWORDTime time = GetCurrentDWORDTime();
 	_stprintf(szPath,_T("20%02d%02d%02d%02d%02d%02d"),time.year,time.month,time.day,time.hour,time.min,time.sec);
 	datatime = szPath;
 	tstring sign = _T("");
	TCHAR szPidNum[32];
	_stprintf(szPidNum,_T("%d"),dwPid);
 	sign =szPidNum + severid +datatime + Key; 
 	string pass = m_pUtil->UnicodeToAnsi(sign.c_str());
 	//ÃÜÔ¿
 	BYTE digest[16];
 	m_pUtil->MD5ForString( (char*)pass.c_str(), digest );
 	string strPswMD5;
 	m_pUtil->MD5ToString( strPswMD5, digest );
	transform(strPswMD5.begin(), strPswMD5.end(), strPswMD5.begin(), toupper);
 	tstring Fsign=m_pUtil->AnsiToUnicode(strPswMD5.c_str());
 	//_stprintf(szPath,g_StrTable[_T("facebook_URL")],pid.c_str(),severid.c_str(),datatime.c_str(),Fsign.c_str());
	_stprintf(szPath,g_StrTable[_T("facebook_URL")],dwPid,_T("&"),severid.c_str(),_T("&"),datatime.c_str(),_T("&"),Fsign.c_str());
 	FACE_BOOK.clear();
 	FACE_BOOK = szPath;
	
	return FACE_BOOK;
}

tstring	FaceBookFrame::GetURL_HongKong()
{
	TCHAR szPath[1024];
	tstring FACE_BOOK = g_StrTable[_T("facebook_URL_HongKong")];
	tstring Key = _T("2zRxLjn~ex0kKH[8+LF6ugx)IPFwoW^i");
	tstring pid =SaveAccount::Inst()->GetAccount();
	DWORD dwPid = SaveAccount::Inst()->GetAccountID();
	tstring severid = SaveAccount::Inst()->GetWorldID();
	tstring datatime =_T("");
	tagDWORDTime time = GetCurrentDWORDTime();
	_stprintf(szPath,_T("20%02d%02d%02d%02d%02d%02d"),time.year,time.month,time.day,time.hour,time.min,time.sec);
	datatime = szPath;
	tstring sign = _T("");
	TCHAR szPidNum[32];
	_stprintf(szPidNum,_T("%d"),dwPid);
	sign =szPidNum + severid +datatime + Key; 
	string pass = m_pUtil->UnicodeToAnsi(sign.c_str());
	//ÃÜÔ¿
	BYTE digest[16];
	m_pUtil->MD5ForString( (char*)pass.c_str(), digest );
	string strPswMD5;
	m_pUtil->MD5ToString( strPswMD5, digest );
	transform(strPswMD5.begin(), strPswMD5.end(), strPswMD5.begin(), toupper);
	tstring Fsign=m_pUtil->AnsiToUnicode(strPswMD5.c_str());
	//_stprintf(szPath,g_StrTable[_T("facebook_URL")],pid.c_str(),severid.c_str(),datatime.c_str(),Fsign.c_str());
	_stprintf(szPath,g_StrTable[_T("facebook_URL_HongKong")],dwPid,_T("&"),severid.c_str(),_T("&"),datatime.c_str(),_T("&"),Fsign.c_str());
	FACE_BOOK.clear();
	FACE_BOOK = szPath;


	return FACE_BOOK;
}