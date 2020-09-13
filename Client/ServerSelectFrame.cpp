#include "stdafx.h"
#include "ServerSelectFrame.h"
#include "shellapi.h"
#include "ClientMain.h"
#include "SerActiveData.h"
#include "ServerTime.h"
#include "HealthyControlFrame.h"
#include "SaveAccount.h"
#include "SDOAIgw.h"
#include "IconStatic.h"
#include "LoadingFrame.h"
#include "SaveAccount.h"
#include "CameraSetData.h"
#include "ClientConfigInCpk.h"
#include "YY\DemoInterface.h"
#include "IconStatic.h"
#include "ToolTipFrame.h"

const tstring strServerHot = _T("data\\ui\\common\\SM_server_hot.bmp");
const tstring strServerNew = _T("data\\ui\\common\\SM_server_new.bmp");

const TCHAR* szFileName = _T("launcher\\serverinfo_back.xml");
const char* szBackName = "\\data\\ui\\Login\\l_server-diban-b.bmp";
static INT  tempBOROUGH_MAX  = BOROUGH_MAX;
static INT  tempGAMEWORLD_MAX = GAMEWORLD_MAX;
static bool bIfInitLoginMode = FALSE; //判断是否做了InitLoginMode（）。

extern int YYPIPCallBack( int nType, WPARAM wParam );

void CallbackShowMessage( const char* szText )
{
	if( szText == NULL ) return;
	WCHAR wzMsgText[256]; 
	memset( wzMsgText, 0, sizeof(wzMsgText) );
	MultiByteToWideChar( CP_ACP, 0, szText, lstrlenA(szText), wzMsgText, sizeof(wzMsgText) ); 
}

void CallbackShowState( int nCurSize, int nMaxSize, int nSpeed )
{
	WCHAR wzStateText[256]; 
	memset( wzStateText, 0, sizeof(wzStateText) );
	wsprintf( wzStateText, L"Cur=%d, Max=%d, Speed=%dk//s", nCurSize, nMaxSize, nSpeed/1024 );
}

void CallbackResult( int bUpdateRet )
{
	if( bUpdateRet == TRUE )
	{
		TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
		TCHAR szTemp[X_LONG_NAME*2];
		_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
		tstring strAppPath = szTemp;

		tstring strIniFileFullName = strAppPath + szIniFileName;

		::GetPrivateProfileString(_T("Launcher"), _T("CropID"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
		tstring strCropID = szTemp;
		if(_tcscmp(strCropID.c_str(), _T("")) == 0 && !SaveAccount::Inst()->GetYunLianOn())
		{
			g_yyDemo.InitFactory();
			if(g_yyDemo.HaveInited())
			{
				g_yyDemo.m_pfnLoadInGame("KZ_SMZX|YY3D9|HOOK9ALL");
				g_yyDemo.m_pfnCheckClient("..\\YY\\yy.zip",NULL);
				g_yyDemo.m_pfnSetCallBack( YYPIPCallBack );
			}
		}
	}
	else
	{

	}
}


// 判断是否安装的360
BOOL Get360SafeboxPath(TCHAR* szValue, DWORD dwSize)
{
	BOOL bRet = FALSE;
	HKEY hKeyOpen = NULL;
	LONG lres = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\360safebox.exe"), 0, KEY_READ | KEY_QUERY_VALUE, &hKeyOpen);

	if (ERROR_SUCCESS == lres) {
		if (NULL != hKeyOpen) {
			DWORD dwType = REG_SZ;
			BYTE* pBuf = new BYTE[dwSize];
			LONG lres = RegQueryValueEx(hKeyOpen, _T(""), NULL, &dwType, pBuf, &dwSize);
			if (ERROR_SUCCESS == lres) {
				if (pBuf) {
					_tcsncpy(szValue, (LPCTSTR)pBuf, dwSize);
					delete[] pBuf;
					bRet = TRUE;
				}
			}
		}
	}
	lres = RegCloseKey(hKeyOpen);
	return bRet;
}

ServerSelectFrame::ServerSelectFrame():m_Trunk(this)
{
	m_pWnd					= NULL;
	m_pWndCaption			= NULL;
	m_pOK					= NULL;
	m_pRegister				= NULL;
	m_pAbundance			= NULL;
	m_pExit					= NULL;
	m_pLastLogin			= NULL;
	m_pLastServer			= NULL;
	m_bIfKoreaServer       = FALSE; //判断是否是韩国服务器
	memset(m_pRecommend, 0x0, sizeof(m_pRecommend));
	memset(m_pRecIconHot, 0x0, sizeof(m_pRecIconHot));
	memset(m_pRecIconNew, 0x0, sizeof(m_pRecIconNew));
	memset(m_pBorough, 0x0, sizeof(m_pBorough));
	memset(m_pGameWorld, 0x0, sizeof(m_pGameWorld));
	memset(m_pWorldIconHot, 0x0, sizeof(m_pWorldIconHot));
	memset(m_pWorldIconNew, 0x0, sizeof(m_pWorldIconNew));
	//m_pToday				= NULL;
	//m_pTomorrow				= NULL;
	m_nBorough				= GT_INVALID;
	m_nGameWorld			= GT_INVALID;
	m_nRecoWorld			= GT_INVALID;
	m_pWnd_Back				= NULL;
	m_pStc_Pic				= NULL;
	for(int i=0; i<(int)m_vBrough.size(); i++)
	{
		m_vBrough[i].vGameWorld.clear();
	}
	m_vBrough.clear();
	m_nLoginMode = 0;
	m_bOnlyGameWord = false;
	m_pFreeStyleWnd = NULL;
	m_nLauncherValue = 0;
	m_nDispValue = 0;
	m_dwKoreaPicShowTime = 0;
	m_bKoreaEnterGame = FALSE;
}
ServerSelectFrame::~ServerSelectFrame()
{

}

BOOL ServerSelectFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;
		
	CameraSetData::Inst()->LoadFromFile();

	SaveAccount::Inst()->GoBack(false);
	m_pVarContainer = CreateObj("Borough", "VarContainer");
	m_pVarIPSaver	= CreateObj("ip_login", "VarContainer");
	m_pVarIPSaver->SetSaveSupport(TRUE);
	InitLoginMode();
	SerActiveData::Inst()->LoadFromFile();
	LoadConfigFile();
	LoadLastLogin();
	CalDate();

	//--------------------------------加载云联开关文件---------------------------------
	TCHAR szYunLianFileName[X_LONG_NAME] = _T("launcher\\YunLian.ini");
	TCHAR szTempTemp[X_LONG_NAME*2];
	_tcscpy( szTempTemp, Kernel::Inst()->GetWorkPath() );
	tstring strYunLianPath = szTempTemp;

	tstring stryunLianFileFullName = strYunLianPath + szYunLianFileName;

	::GetPrivateProfileString(_T("YunLian"), _T("On"), NULL,  szTempTemp, X_LONG_NAME*2, stryunLianFileFullName.c_str());
	tstring strYunLianOn = szTempTemp;

	if(_tcscmp(strYunLianOn.c_str(), _T("1")) == 0)
	{
		SaveAccount::Inst()->SetYunLianOn(TRUE);
	}
	else 
	{
		SaveAccount::Inst()->SetYunLianOn(FALSE);
	}
	//--------------------------------------------------------------------------------
	
	//非大陆登录模式直接显示游戏世界
	if (m_nLoginMode!=1)
	{
		m_nBorough = 0;
		ShowWorldPushButton();
	}

	// 360
	if (ClientConfigInCpk::Inst()->Is360Open())
	{
		tstring strPath = Kernel::Inst()->GetWorkPath();
		strPath += _T("config\\protect.dat");
		FILE *fp= _tfopen( strPath.c_str(), _T("rb") );
		if( fp != NULL )
		{
			DWORD dw360ID = GT_INVALID;
			fread(&dw360ID, sizeof(DWORD), 1, fp);
			if(dw360ID != 1)
			{
				TCHAR szValue[MAX_PATH] = {0};
				BOOL bSuccess = Get360SafeboxPath(szValue, MAX_PATH);
				if (!bSuccess)
				{
					m_p360Wnd->SetInvisible(FALSE);
				}
			}
		}
		else
		{
			TCHAR szValue[MAX_PATH] = {0};
			BOOL bSuccess = Get360SafeboxPath(szValue, MAX_PATH);
			if (!bSuccess)
			{
				m_p360Wnd->SetInvisible(FALSE);
			}
		}
	}

	// 更新YY
	if(1 == m_nLoginMode)
	{
		TCHAR szIniFileName[512] = L"\\YY\\pipFactory.dll";
		TCHAR szIniFileName2[512] = L"..\\YY\\yy.zip";
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
		if( hDll == NULL ) return TRUE;

		typedef int (* PFN_UPDATAYYVOICE)( const char*, void*, void*, void* ) ;
		PFN_UPDATAYYVOICE pfnUpdate = (PFN_UPDATAYYVOICE)GetProcAddress( hDll, "YYPIP_StartUpdate" );
		if( pfnUpdate == NULL ) return TRUE;

		CHAR*	m_pCharPool;
		m_pCharPool = new CHAR[1024*64];
		WideCharToMultiByte(CP_ACP, 0, m_strIniFileFullName2.c_str(), -1, m_pCharPool, 1024*64, NULL, NULL);
		pfnUpdate(m_pCharPool, CallbackShowMessage, CallbackShowState, CallbackResult );

		SAFE_DEL_ARRAY(m_pCharPool);
	}

	
	 
	return TRUE;
}

BOOL ServerSelectFrame::Destroy()
{
	GameFrame::Destroy();
	if( P_VALID(m_pWnd) )
		m_pGUI->DestroyWnd(m_pWnd);
	if( P_VALID(m_pWnd_Back) )
		m_pGUI->DestroyWnd(m_pWnd_Back);
	if( P_VALID(m_pFreeStyleWnd) )
		m_pGUI->DestroyWnd(m_pFreeStyleWnd);


	KillObj("Borough");
	KillObj("ip_login");

	
	
	return TRUE;
}

BOOL ServerSelectFrame::ReloadUI()
{

	//登录模式判断
	InitLoginMode();

	// 背景
	tstring strPath1 = g_strLocalPath + _T("\\ui\\optseverback.xml");
	XmlElement ele;
	m_pGUI->LoadXml(&ele, "VFS_System", strPath1.c_str());
	stringstream clientX, clientY;
	GUIWnd* pDesktop = m_pGUI->GetDesktop();
	clientX << (pDesktop->GetSize().x);
	clientY << (pDesktop->GetSize().y);
	m_pGUI->ChangeXml(&ele, "os_win", "Size_x", clientX.str().c_str());
	m_pGUI->ChangeXml(&ele, "os_win", "Size_y", clientY.str().c_str());
	m_pGUI->ChangeXml(&ele, "os_win", "LogicPos_x", "0");
	m_pGUI->ChangeXml(&ele, "os_win", "LogicPos_y", "0");
	m_pGUI->ChangeXml(&ele, "os_win\\logopic", "Size_x", clientX.str().c_str());
	m_pGUI->ChangeXml(&ele, "os_win\\logopic", "Size_y", clientY.str().c_str());
	m_pGUI->ChangeXml(&ele, "os_win\\logopic", "LogicPos_x", "0");
	m_pGUI->ChangeXml(&ele, "os_win\\logopic", "LogicPos_y", "0");
	m_pGUI->ChangeXml(&ele, "os_win\\Koreabanpic", "Size_x", clientX.str().c_str());
	m_pGUI->ChangeXml(&ele, "os_win\\Koreabanpic", "Size_y", clientY.str().c_str());
	m_pGUI->ChangeXml(&ele, "os_win\\Koreabanpic", "LogicPos_x", "0");
	m_pGUI->ChangeXml(&ele, "os_win\\Koreabanpic", "LogicPos_y", "0");
	m_pWnd_Back = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pWnd_Back->SetUnClickable(TRUE);

	// 创建：大陆模式才有大区选择，xml中控件位置有不同
	tstring strPath;
	if (m_nLoginMode==1)
	{
		strPath = g_strLocalPath + _T("\\ui\\optsever.xml");
	}
	else
	{
		strPath = g_strLocalPath + _T("\\ui\\optsever_f.xml");
	}

	XmlElement ele1;
	m_pGUI->LoadXml(&ele1, "VFS_System", strPath.c_str());
	CHAR szTemp[X_LONG_NAME] = {0};
	
	for(int i=0; i<RECOMMEND_GAMEWORLD; i++)
	{
		_snprintf(szTemp, X_LONG_NAME, "os_win\\os_pic\\fra1\\Recommended%d\\pic0", i);
		m_pGUI->ChangeXml(&ele1, szTemp, "ClassName", "IconStatic");
		_snprintf(szTemp, X_LONG_NAME, "os_win\\os_pic\\fra1\\Recommended%d\\pic1", i);
		m_pGUI->ChangeXml(&ele1, szTemp, "ClassName", "IconStatic");
	}

	for(int i=0; i<GAMEWORLD_MAX; i++)
	{
		_snprintf(szTemp, X_LONG_NAME, "os_win\\os_pic\\fram3\\burough%d\\pic0", i);
		m_pGUI->ChangeXml(&ele1, szTemp, "ClassName", "IconStatic");
		_snprintf(szTemp, X_LONG_NAME, "os_win\\os_pic\\fram3\\burough%d\\pic1", i);
		m_pGUI->ChangeXml(&ele1, szTemp, "ClassName", "IconStatic");
	}

	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back\\exp", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back\\gold", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back\\present", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back\\four", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back\\five", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back1\\exp", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back1\\gold", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back1\\present", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back1\\four", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back1\\five", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back2\\exp", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back2\\gold", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back2\\present", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back2\\four", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back2\\five", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back3\\exp" , "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back3\\gold" , "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back3\\present" , "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back3\\four" , "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\back3\\five" , "ClassName", "IconStatic");

	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\abundance", "ClassName", "CGUIButton");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\register", "ClassName", "CGUIButton");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\login", "ClassName", "CGUIButton");
	m_pGUI->ChangeXml(&ele1, "os_win\\os_pic\\exit", "ClassName", "CGUIButton");

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele1);
	m_pWnd->FlipToTop();

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&ServerSelectFrame::EventHandler));
	
	m_pWndCaption	= m_pWnd->GetChild(_T("os_pic"));
	m_pOK			= (GUIButton*)m_pWndCaption->GetChild(_T("login"));
	m_pOK->SetText(g_StrTable[_T("SelectServer1")]);
	m_pRegister		= (GUIButton*)m_pWndCaption->GetChild(_T("register"));
	m_pRegister->SetText(g_StrTable[_T("SelectServer2")]);
	m_pAbundance	= (GUIButton*)m_pWndCaption->GetChild(_T("abundance"));
	m_pAbundance->SetText(g_StrTable[_T("SelectServer3")]);
	m_pExit			= (GUIButton*)m_pWndCaption->GetChild(_T("exit"));
	m_pExit->SetText(g_StrTable[_T("SelectServer4")]);
	//m_pToday		= (GUIListBox*)m_pWndCaption->GetChild(_T("tasklist1"));
	//m_pTomorrow		= (GUIListBox*)m_pWndCaption->GetChild(_T("tasklist2"));
	m_pLastLogin = (GUIPushButton*)m_pWndCaption->GetChild(_T("burough"));
	m_pLastServer = (GUIStatic*)m_pWndCaption->GetChild(_T("lastburough"));

	m_pStcTip[0] = (IconStatic*)m_pWndCaption->GetChild(_T("back\\exp"));
	m_pStcTip[1] = (IconStatic*)m_pWndCaption->GetChild(_T("back\\gold"));
	m_pStcTip[2] = (IconStatic*)m_pWndCaption->GetChild(_T("back\\present"));
	m_pStcTip[3] = (IconStatic*)m_pWndCaption->GetChild(_T("back\\four"));
	m_pStcTip[4] = (IconStatic*)m_pWndCaption->GetChild(_T("back\\five"));
	m_pStcTip[5] = (IconStatic*)m_pWndCaption->GetChild(_T("back1\\exp"));
	m_pStcTip[6] = (IconStatic*)m_pWndCaption->GetChild(_T("back1\\gold"));
	m_pStcTip[7] = (IconStatic*)m_pWndCaption->GetChild(_T("back1\\present"));
	m_pStcTip[8] = (IconStatic*)m_pWndCaption->GetChild(_T("back1\\four"));
	m_pStcTip[9] = (IconStatic*)m_pWndCaption->GetChild(_T("back1\\five"));
	m_pStcTip[10] = (IconStatic*)m_pWndCaption->GetChild(_T("back2\\exp"));
	m_pStcTip[11] = (IconStatic*)m_pWndCaption->GetChild(_T("back2\\gold"));
	m_pStcTip[12] = (IconStatic*)m_pWndCaption->GetChild(_T("back2\\present"));
	m_pStcTip[13] = (IconStatic*)m_pWndCaption->GetChild(_T("back2\\four"));
	m_pStcTip[14] = (IconStatic*)m_pWndCaption->GetChild(_T("back2\\five"));
	m_pStcTip[15] = (IconStatic*)m_pWndCaption->GetChild(_T("back3\\exp"));
	m_pStcTip[16] = (IconStatic*)m_pWndCaption->GetChild(_T("back3\\gold"));
	m_pStcTip[17] = (IconStatic*)m_pWndCaption->GetChild(_T("back3\\present"));
	m_pStcTip[18] = (IconStatic*)m_pWndCaption->GetChild(_T("back3\\four"));
	m_pStcTip[19] = (IconStatic*)m_pWndCaption->GetChild(_T("back3\\five"));

	m_pBtnHttp = (GUIButton*)m_pWndCaption->GetChild(_T("np_butt"));
	if(m_nLoginMode==1)
	{
		for(int i = 0; i < 20; i++)
		{
			m_pStcTip[i]->SetInvisible(TRUE);
		}
		m_pBtnHttp->SetInvisible(TRUE);
	}

	
	for(int i=0; i<RECOMMEND_GAMEWORLD; i++)
	{
		tstringstream name;
		name << _T("fra1\\Recommended") << i;
		m_pRecommend[i]	= (GUIPushButton*)m_pWndCaption->GetChild(name.str().c_str());
		if( P_VALID(m_pRecommend[i]) )
			m_pRecommend[i]->SetInvisible(TRUE);
		m_pRecIconHot[i] = (IconStatic*)m_pRecommend[i]->GetChild(_T("pic0"));
		m_pRecIconHot[i]->SetInvisible(TRUE);
		m_pRecIconNew[i] = (IconStatic*)m_pRecommend[i]->GetChild(_T("pic1"));
		m_pRecIconNew[i]->SetInvisible(TRUE);
	}

	for(int i=0; i<BOROUGH_MAX; i++) 
		//一般服务器UI中比韩服多出来的控件已经在XML中设为Invisible了
	{
		tstringstream name;
		name << _T("fra2\\burough") << i;
		m_pBorough[i]	= (GUIPushButton*)m_pWndCaption->GetChild(name.str().c_str());
		if( P_VALID(m_pBorough[i]) )
			m_pBorough[i]->SetInvisible(TRUE);
	}

	for(int i=0; i<GAMEWORLD_MAX; i++)
		//一般服务器UI中比韩服多出来的控件已经在XML中设为Invisible了
	{
		tstringstream name;
		name << _T("fram3\\burough") << i;
		m_pGameWorld[i]	= (GUIPushButton*)m_pWndCaption->GetChild(name.str().c_str());
		if( P_VALID(m_pGameWorld[i]) )
			m_pGameWorld[i]->SetInvisible(TRUE);

		m_pWorldIconHot[i] = (IconStatic*)m_pGameWorld[i]->GetChild(_T("pic0"));
		m_pWorldIconHot[i]->SetInvisible(TRUE);
		m_pWorldIconNew[i] = (IconStatic*)m_pGameWorld[i]->GetChild(_T("pic1"));
		m_pWorldIconNew[i]->SetInvisible(TRUE);
	}
	/*if (m_bIfKoreaServer == FALSE)
	{
		m_pToday->SetColWidth( 0, 70 );
		m_pToday->SetColWidth( 1, 80 );
		m_pToday->SetColWidth( 2, 100 );
		m_pTomorrow->SetColWidth( 0, 70 );
		m_pTomorrow->SetColWidth( 1, 80 );
		m_pTomorrow->SetColWidth( 2, 100 );
	} 
	else
	{
		m_pToday->SetColWidth( 0, 125 );
		m_pToday->SetColWidth( 1, 125 );
		m_pToday->SetColWidth( 2, 150 );
		m_pTomorrow->SetColWidth( 0, 125 );
		m_pTomorrow->SetColWidth( 1, 125 );
		m_pTomorrow->SetColWidth( 2, 150 );
	}*/
	
	//和谐游戏界面。。。
	tstring strPath2 = g_strLocalPath + _T("\\ui\\fight_sure.xml");
	XmlElement ele2;
	m_pGUI->LoadXml(&ele2, "VFS_System", strPath2.c_str());
	m_pFreeStyleWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele2);
	m_pPBFreeStyleChoose = (GUIPushButton*)m_pFreeStyleWnd->GetChild(_T("back\\optbutt"));
	m_pPBFreeStyleChoose->SetState(EGUIBS_PushDown,FALSE);
	m_pBtnChoose = (GUIButton*)m_pFreeStyleWnd->GetChild(_T("back\\button"));
	m_pBtnChoose->SetText(g_StrTable[_T("Server_Select_Confirm")]);
	m_pFreeStyleWnd->SetInvisible(TRUE);
	m_pGUI->RegisterEventHandler( m_pFreeStyleWnd->GetFullName().c_str(),   m_Trunk.sfp1(&ServerSelectFrame::OnFreeStyleWnd));
	GetSpecialValue();

	tstring strPath3 = g_strLocalPath + _T("\\ui\\360.xml");
	XmlElement ele3;
	m_pGUI->LoadXml(&ele3, "VFS_System", strPath3.c_str());
	m_p360Wnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele3);
	m_pBn360Again = (GUIPushButton*)m_p360Wnd->GetChild(_T("back\\optbutt"));
	m_pBn360Again->SetState(EGUIBS_PushDown,FALSE);
	m_pBn360On = (GUIButton*)m_p360Wnd->GetChild(_T("back\\sure"));
	m_pBn360Off = (GUIButton*)m_p360Wnd->GetChild(_T("back\\cancle"));
	m_p360Wnd->SetInvisible(TRUE);
	m_pGUI->RegisterEventHandler( m_p360Wnd->GetFullName().c_str(),   m_Trunk.sfp1(&ServerSelectFrame::OnFreeStyleWnd));


	if(m_nLoginMode == 11) //韩国模式
	{
		m_pStcKoreaPic = (GUIStatic*)m_pWnd_Back->GetChild(_T("Koreabanpic"));
	}
	return TRUE;
}

VOID ServerSelectFrame::Update()
{
	// 激活自身窗口，避免刚启动又不小心被用户切到后台
	HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));
	::ShowWindow(hWnd, SW_SHOWNORMAL);

	if ( m_nLoginMode == 1 )
	{
		if (P_VALID(m_pPBFreeStyleChoose)&&P_VALID(m_pBtnChoose))
		{
			if ( m_pPBFreeStyleChoose->GetState() == EGUIBS_PushDown ) //勾选
			{
				m_pBtnChoose->SetText(g_StrTable[_T("Server_Select_Confirm")]);
			} 
			else
			{
				m_pBtnChoose->SetText(g_StrTable[_T("Server_Select_Quit")]);
			} 
		}
	}
	else if( m_nLoginMode == 11 )
	{
		if(Kernel::Inst()->GetAccumTimeDW() - m_dwKoreaPicShowTime >= 4000 && m_bKoreaEnterGame )		//玉米棒子毛病多，又改成4秒了
		{
			m_pStcKoreaPic->SetInvisible(TRUE);
			m_pWnd->SetInvisible(FALSE);
			ConfirmLogin();
		}
	}
}

BOOL ServerSelectFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;
	switch(pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pOK )
			{
				if ( m_nLoginMode == 1 )
				{
					m_pFreeStyleWnd->SetInvisible(FALSE);
					m_pGUI->SetActive(m_pFreeStyleWnd);
				}
				else if( m_nLoginMode == 11 )
				{
					m_bKoreaEnterGame = TRUE;
					m_dwKoreaPicShowTime = Kernel::Inst()->GetAccumTimeDW();

					m_pStcKoreaPic->SetInvisible(FALSE);
					m_pStcKoreaPic->SetTopMost(TRUE);
					m_pWnd->SetInvisible(TRUE);
				}
				else
				{
					ConfirmLogin();
				}
			}
			if( pWnd == m_pRegister )
				AccountRegister();

			if( pWnd == m_pAbundance )
				AccountAbundance();

			if(	pWnd == m_pExit )
				ExitGame();

			if( pWnd == m_pBtnHttp)
			{
				ShellExecute(NULL, _T("open"), szButtHppt, _T(""), NULL, SW_MAXIMIZE);
			}
		}
		break;	
	case EGUIE_PushButtonDown:
		{
			//如果是模拟发的Down事件,则不处理
			if(pEvent->dwParam1!=1)
				return false;

			if (m_bIfKoreaServer == FALSE)
			{
				if(_tcsnccmp(pWnd->GetName().c_str(), _T("Recommended"), 11) == 0)
				{
					for(int i=0; i<RECOMMEND_GAMEWORLD; i++)
					{	
						if( pWnd == m_pRecommend[i] )
						{
							m_nRecoWorld = i;
							SetWorldPushButtonState();
							m_pLastLogin->SetState(EGUIBS_PopUp, FALSE);
							break;
						}
					}
				}
			}
			
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("burough"), 7) == 0)
			{
				for(int i=0; i<tempBOROUGH_MAX; i++)
				{
					if( pWnd == m_pBorough[i] )
					{
						// 如果两次点的是同一个按钮,不进行处理
						if( !m_bOnlyGameWord )
						{
							if (m_nBorough != i)
							{
								m_nBorough = i;
								// 切换大区,游戏世界默认设置为无
								SetWorldPushButtonState();
								ShowWorldPushButton();
								m_pLastLogin->SetState(EGUIBS_PopUp, FALSE);
							}
						}
						else
						{
							m_nGameWorld = GetWolrd(i);
							m_nBorough = GetBorough(i);
							SetRecommedPushButtonState();
							m_pLastLogin->SetState(EGUIBS_PopUp, FALSE);
						}
					}
				}

				for(int i=0; i<tempGAMEWORLD_MAX; i++)
				{
					if( pWnd == m_pGameWorld[i] )
					{
						m_nGameWorld = i;
						SetRecommedPushButtonState();
						m_pLastLogin->SetState(EGUIBS_PopUp, FALSE);
					}
				}
			}

			if( pWnd == m_pLastLogin )
			{
				SetWorldPushButtonState();
				SetRecommedPushButtonState();
				for(int i=0; i<tempBOROUGH_MAX; i++)
				{					
					if( P_VALID(m_pBorough[i]) )
						m_pBorough[i]->SetState(EGUIBS_PopUp, FALSE);				 
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for(int i = 0; i <  20; i++)
			{
				if(pWnd == m_pStcTip[i])
				{
					ShowTip(pWnd, m_vTip[i].szTips);
				}
			}
		}
		break;
	case EGUIE_Drag:
		{
			for(int i = 0; i < 20; i++)
			{
				if(pWnd == m_pStcTip[i])
				{
					TCHAR szTemp[128] = {0};
					_tcscpy(szTemp, m_vTip[i].szHttp);
					if(_tcscmp(szTemp, _T("")) != 0)
					{
						ShellExecute(NULL, _T("open"), szTemp, _T(""), NULL, SW_MAXIMIZE);
					}
					
				}
			}
		}

	}
	return FALSE;
}

BOOL ServerSelectFrame::OnFreeStyleWnd(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;
	switch(pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnChoose )
			{
				if ( m_pPBFreeStyleChoose->GetState() == EGUIBS_PushDown ) //勾选
				{
					 ConfirmLogin();
				} 
				else
				{
					  m_pFreeStyleWnd->SetInvisible(TRUE);
					  m_pGUI->SetActive(m_pWnd);
				} 
			}

			if (pWnd == m_pBn360On)
			{
				m_p360Wnd->SetInvisible(TRUE);
				if ( EGUIBS_PushDown ==  m_pBn360Again->GetState())
				{
					tstring strPath = Kernel::Inst()->GetWorkPath();
					strPath += _T("config\\protect.dat");

					FILE* fp = NULL;
					fp=_tfopen( strPath.c_str(), _T("wb") );

					if( fp != NULL)
					{
						DWORD dw360ID = 1;
						fwrite(&dw360ID, sizeof(DWORD), 1, fp);
						fclose(fp);
					}


					TCHAR szIniFileName[512] = L"\\reso\\360Inst_shengmzx.exe";
					const int MAX_STRING_SIZE = 512;
					TCHAR szTemp[512*2];

					// 获取EXE路径
					GetModuleFileName(NULL, szTemp, sizeof(szTemp)/sizeof(TCHAR));
					std::wstring ss = szTemp;
					int pos = ss.rfind(L"\\");
					ss.erase(ss.begin()+pos,ss.end());
					std::wstring sDir = ss;
					pos = ss.rfind(L"\\");
					ss.erase(ss.begin()+pos,ss.end());
					std::wstring m_strIniFileFullName = ss + szIniFileName;

					STARTUPINFO si;
					ZeroMemory(&si, sizeof(si));
					si.cb = sizeof(si);
					PROCESS_INFORMATION pi;
					::CreateProcess(m_strIniFileFullName.c_str(), NULL, 0, 0, TRUE, 0, 0, sDir.c_str(), &si, &pi);

				}
			}
			else if (pWnd == m_pBn360Off)
			{
				m_p360Wnd->SetInvisible(TRUE);
				if ( EGUIBS_PushDown ==  m_pBn360Again->GetState())
				{
					tstring strPath = Kernel::Inst()->GetWorkPath();
					strPath += _T("config\\protect.dat");

					FILE* fp = NULL;
					fp=_tfopen( strPath.c_str(), _T("wb") );

					if( fp != NULL)
					{
						DWORD dw360ID = 1;
						fwrite(&dw360ID, sizeof(DWORD), 1, fp);
						fclose(fp);
					}
				}
			}
		}
		break;
	}
	return FALSE;
}
void ServerSelectFrame::CalDate()
{
	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
}

// 得到服务器返回的游戏世界状态颜色
DWORD ServerSelectFrame::GetWorldStatusColor(const INT nWorldStatus)
{
	INT nStatus;
	if(nWorldStatus>0 && nWorldStatus<=30)
	{
		nStatus = 0;			//优良
	}
	else if(nWorldStatus>30 && nWorldStatus<=50)
	{
		nStatus = 1;			//良好
	}
	else if(nWorldStatus>50 && nWorldStatus<=70)
	{
		nStatus = 2;			//热闹
	}
	else if(nWorldStatus>70 && nWorldStatus<=100)
	{
		nStatus = 3;			//火爆
	}
	else
		nStatus = 4;			//维护

	DWORD dwStatus;
	switch(nStatus)
	{	
	case 0:
		dwStatus =  0xFF13CF00;
		break;
	case 1:
		dwStatus =  0xFFFFDE26;
		break;
	case 2:
		dwStatus =  0xFFFF7800;
		break;
	case 3:
		dwStatus =  0xFFFF0000;
		break;
	case 4:
		dwStatus =  0xFFACACAC;
		break;
	default:
		dwStatus =  0xFFACACAC;
		break;
	}
	return dwStatus;
}

void ServerSelectFrame::GetWorldIDByWorldName(tstring strWorldName,tstring& strIp, tstring& strPort, tstring& strWorldID)
{
	TObjRef<StreamTransport>	Stream;
	list<tstring> FieldList;
	m_pVarContainer->Clear();
	m_pVarContainer->Load(NULL, szFileName, "id", &FieldList);
	list<tstring>::iterator iter = FieldList.begin();



	tstring _strWorldName;

	for(; iter!=FieldList.end(); iter++)
	{
		
		_strWorldName= m_pVarContainer->GetString(_T("name"), (*iter).c_str(), _T(""));		//读出游戏世界名
		strIp = m_pVarContainer->GetString(_T("ip"), (*iter).c_str(), _T("") );					
		strPort = m_pVarContainer->GetString(_T("port"), (*iter).c_str(), _T("")  );				//游戏世界Port
		strWorldID = m_pVarContainer->GetString(_T("worldid"), (*iter).c_str(), _T("")  );				//读出游戏世界ID

		if (_strWorldName.compare(strWorldName)==0){


           return;

		}


	}

}


void ServerSelectFrame::ConfirmLogin()
{
	//选择了推荐的游戏世界
	if(m_nRecoWorld!=GT_INVALID)
	{
		TObjRef<StreamTransport>	Stream;
		tstringstream ip;
		ip << _T("ip last_login_server");
		CHAR* szIP = Stream->IP2String(m_vRecommend[m_nRecoWorld].dwWorldIP);
		tstring strIP = m_pUtil->Unicode8ToUnicode(szIP);
		m_pVarIPSaver->Set(strIP.c_str(), ip.str().c_str());
		tstringstream port;
		port << _T("port last_login_server");
		m_pVarIPSaver->Set(m_vRecommend[m_nRecoWorld].dwWorldPort, port.str().c_str());
		for( vector<tagBorough>::iterator it=m_vBrough.begin(); it!=m_vBrough.end(); ++it )
		{
			for( vector<tagGameWorld>::iterator it1=it->vGameWorld.begin(); it1!=it->vGameWorld.end(); ++it1 )
			{
				if( _tcscmp(m_vRecommend[m_nRecoWorld].szWorldName, it1->szWorldName)==0 )
				{
					tstringstream serName;
					serName << _T("server_name last_login_server");
					m_pVarIPSaver->Set(it->szBoroughName, serName.str().c_str());
					tstringstream serID;
					serID << _T("server_id last_login_server");
					m_pVarIPSaver->Set(it->dwBoroughID, serID.str().c_str());
					tstringstream worldid;
					worldid << _T("world_id last_login_server");
					m_pVarIPSaver->Set(it1->dwWorldID, worldid.str().c_str());
					break;
				}
			}
		}
		
		tstringstream name;
		name << _T("gamename last_login_server");
		m_pVarIPSaver->Set(m_vRecommend[m_nRecoWorld].szWorldName, name.str().c_str());
		m_pVarIPSaver->Save(_T("config\\lastlogin.xml"));
	}
	//选择了大区下的普通游戏世界
	else if(m_nBorough!=GT_INVALID && m_nGameWorld!=GT_INVALID)
	{
		TObjRef<StreamTransport>	Stream;
		tstringstream ip;
		ip << _T("ip last_login_server");
		CHAR* szIP = Stream->IP2String(m_vBrough[m_nBorough].vGameWorld[m_nGameWorld].dwWorldIP);
		tstring strIP = m_pUtil->Unicode8ToUnicode(szIP);
		m_pVarIPSaver->Set(strIP.c_str(), ip.str().c_str());
		tstringstream port;
		port << _T("port last_login_server");
		m_pVarIPSaver->Set(m_vBrough[m_nBorough].vGameWorld[m_nGameWorld].dwWorldPort, port.str().c_str());
		tstringstream serName;
		serName << _T("server_name last_login_server");
		m_pVarIPSaver->Set(m_vBrough[m_nBorough].szBoroughName, serName.str().c_str());
		tstringstream serID;
		serID << _T("server_id last_login_server");
		m_pVarIPSaver->Set(m_vBrough[m_nBorough].dwBoroughID, serID.str().c_str());
		tstringstream worldid;
		worldid << _T("world_id last_login_server");
		m_pVarIPSaver->Set(m_vBrough[m_nBorough].vGameWorld[m_nGameWorld].dwWorldID, worldid.str().c_str());
		tstringstream name;
		name << _T("gamename last_login_server");
		m_pVarIPSaver->Set(m_vBrough[m_nBorough].vGameWorld[m_nGameWorld].szWorldName, name.str().c_str());
		m_pVarIPSaver->Save(_T("config\\lastlogin.xml"));
	}

	

	tstring strBoroghID;
	tstring strWorldName;
	tstring  strIp;
	tstring  strPort;
	tstring  strWorldID;

	tstring _server_name;
	tstring _ip;
	tstring _port;
	tstring _server_id;


	TObjRef<VarContainer> serInfo = CreateObj("lastlogin", "VarContainer");
	const TCHAR* szFileName = _T("config\\lastlogin.xml");
	if( serInfo->Load(NULL, szFileName) ) 
	{
		strBoroghID = serInfo->GetString(_T("world_id"), _T("last_login_server"), _T(""));
        strWorldName = serInfo->GetString(_T("gamename"), _T("last_login_server"), _T(""));
		_server_name = serInfo->GetString(_T("server_name"), _T("last_login_server"), _T(""));
		_ip = serInfo->GetString(_T("ip"), _T("last_login_server"), _T(""));
		_port=serInfo->GetString(_T("port"), _T("last_login_server"), _T(""));
        _server_id =serInfo->GetString(_T("server_id"), _T("last_login_server"), _T(""));

        GetWorldIDByWorldName(strWorldName, strIp,  strPort, strWorldID);
		if(strWorldID.compare(strBoroghID)!=0 || strIp.compare(_ip)!=0 || strPort.compare(_port)!=0 ){
			SaveAccount::Inst()->SetWorldID(strWorldID.c_str());

			TObjRef<StreamTransport>	Stream;
			tstringstream ip;
			ip << _T("ip last_login_server");
			m_pVarIPSaver->Set(strIp.c_str(), ip.str().c_str());
			tstringstream port;
			port << _T("port last_login_server");
			m_pVarIPSaver->Set(strPort.c_str(), port.str().c_str());
			tstringstream serName;
			serName << _T("server_name last_login_server");
			m_pVarIPSaver->Set(_server_name.c_str(), serName.str().c_str());
			tstringstream serID;
			serID << _T("server_id last_login_server");
			m_pVarIPSaver->Set(_server_id.c_str(), serID.str().c_str());
			tstringstream worldid;
			worldid << _T("world_id last_login_server");
			m_pVarIPSaver->Set(strWorldID.c_str(), worldid.str().c_str());
			tstringstream name;  
			name << _T("gamename last_login_server");
			m_pVarIPSaver->Set(strWorldName.c_str(), name.str().c_str());
			m_pVarIPSaver->Save(_T("config\\lastlogin.xml"));


		}else{
			SaveAccount::Inst()->SetWorldID(strBoroghID.c_str());
		}
		

       //<item name="last_login_server" gamename="ALEX-WORLD" server_name="ALEX" ip="127.0.0.1" port="4200" server_id="900500" world_id="900500" />




 

	}
	KillObj("lastlogin");

    //防沉迷提示
    HealthyControlFrame* pHCFrame;
    //防止重复建立frame
    pHCFrame = (HealthyControlFrame*)m_pFrameMgr->GetFrame(_T("HealthyControl"));
    if (P_VALID(pHCFrame))
        m_pFrameMgr->DestroyFrame(pHCFrame);
    //创建
    pHCFrame = (HealthyControlFrame*)m_pFrameMgr->CreateFrame(_T("Root"), _T("HealthyControl"), _T("HealthyControlFrame"), 0);
    if (P_VALID(pHCFrame))
    {
        // 先关闭窗体，等收到服务器发来的通知再显示窗体
        pHCFrame->ShowWnd(FALSE);
    }
    if (!pHCFrame)
        IMSG(_T("Create HealthyControlFrame failed"));

    m_pFrameMgr->CreateFrame(_T("Root"), _T("Login"), _T("LoginFrame"), 0);
	LoadingFrame* pFrame = (LoadingFrame*)m_pFrameMgr->CreateFrame(_T("Root"), _T("Loading"), _T("LoadingFrame"), 0);
	if(P_VALID(pFrame))
		pFrame->SetLoadingType(LoadingFrame::ELT_Login);

	m_pFrameMgr->AddToDestroyList(this);
}

void ServerSelectFrame::AccountRegister()
{
	TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
	TCHAR szTemp[X_LONG_NAME*2];

	_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
	tstring strAppPath = szTemp;

	tstring strIniFileFullName = strAppPath + szIniFileName;
	::GetPrivateProfileString(_T("Launcher"), _T("UserRegURL"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
	tstring url = szTemp;
	if( url == _T("") )
	{
		MessageBox(NULL, _T("Don't find user register url!"), _T(""), MB_OK);
		return;
	}else if (url==_T("http://passport.gamall.net/reg.jsp"))
	{
		url=_T("http://passport.kongzhong.com/reg.jsp");
		::WritePrivateProfileString(_T("Launcher"), _T("UserRegURL"),url.c_str(),strIniFileFullName.c_str());
	}

	//打开注册网页
	ShellExecute(NULL, _T("open"), url.c_str(), NULL, NULL, SW_MAXIMIZE);
}

void ServerSelectFrame::AccountAbundance()
{
	TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
	TCHAR szTemp[X_LONG_NAME*2];

	_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
	tstring strAppPath = szTemp;

	tstring strIniFileFullName = strAppPath + szIniFileName;
	::GetPrivateProfileString(_T("Launcher"), _T("AccountActURL"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
	tstring url = szTemp;
	if( url == _T("") )
	{
		MessageBox(NULL, _T("Don't find account active url!"), _T(""), MB_OK);
		return;
	}else if (url==_T("http://passport.gamall.net/act.jsp")) 
	{
		url=_T("http://pay.kongzhong.com");
		::WritePrivateProfileString(_T("Launcher"), _T("AccountActURL"),url.c_str(),strIniFileFullName.c_str());
	}

	//打开充值网页
	ShellExecute(NULL, _T("open"), url.c_str(), NULL, NULL, SW_MAXIMIZE);
}

void ServerSelectFrame::ExitGame()
{
	if( SDOAIgw::Inst()->GetSDOAFlag() )
		SDOAIgw::Inst()->LogOut();
	m_pClient->ExitThreadLoad();
	PostQuitMessage(0);
}

bool CompareGameWorldRank(const tagGameWorld& world1, const tagGameWorld& world2)
{
	return world1.dwWorldRank < world2.dwWorldRank;
}

bool CompareBoroughRank(const tagBorough& borough1, const tagBorough& borough2)
{
	return borough1.dwBoroughRank < borough2.dwBoroughRank;
}

bool CompareRecommendRank(const tagGameWorld& world1, const tagGameWorld& world2)
{
	return world1.dwRecoRank < world2.dwRecoRank;
}






void ServerSelectFrame::LoadConfigFile()
{
	TObjRef<StreamTransport>	Stream;
	list<tstring> FieldList;
	m_pVarContainer->Load(NULL, szFileName, "id", &FieldList);
	list<tstring>::iterator iter = FieldList.begin();


	tstring strIP;
	string szIP;
	tstring strBourugh;
	DWORD dwBouroghID;
	for(; iter!=FieldList.end(); iter++)
	{
		INT nValue = m_pVarContainer->GetInt(_T("disp"), (*iter).c_str(), 0 );
		if( m_nLauncherValue != nValue )
			continue;
		tagGameWorld gameWorld;
		_tcscpy( gameWorld.szWorldName, m_pVarContainer->GetString(_T("name"), (*iter).c_str(), _T("")) );		//读出游戏世界名
		gameWorld.dwWorldRank = m_pVarContainer->GetDword(_T("worldrank"), (*iter).c_str(), GT_INVALID );			//读出游戏世界的编号
		gameWorld.nStatus = (INT)m_pVarContainer->GetDword(_T("state"), (*iter).c_str(), GT_INVALID );				//读出游戏世界的状态
		gameWorld.dwStatus = GetWorldStatusColor(gameWorld.nStatus);									//计算出游戏世界状态的颜色
		strIP = m_pVarContainer->GetString(_T("ip"), (*iter).c_str(), _T("") );					
		szIP = m_pUtil->UnicodeToUnicode8(strIP.c_str());
		gameWorld.dwWorldIP = Stream->StringIP2IP( (CHAR*)szIP.c_str() );								//游戏世界IP
		gameWorld.dwWorldPort = m_pVarContainer->GetDword(_T("port"), (*iter).c_str(), GT_INVALID );				//游戏世界Port
		gameWorld.byRecommend = (BYTE)m_pVarContainer->GetDword(_T("recommend"), (*iter).c_str(), GT_INVALID );		//读出游戏世界是否为推荐	
		gameWorld.byGreenFlag = (BYTE)m_pVarContainer->GetDword(_T("greenflag"), (*iter).c_str(), GT_INVALID );		//读出游戏世界的绿色区标识
		gameWorld.dwRecoRank = m_pVarContainer->GetDword(_T("recorank"), (*iter).c_str(), GT_INVALID );			//读出游戏世界推荐编号
		gameWorld.dwWorldID = m_pVarContainer->GetDword(_T("worldid"), (*iter).c_str(), GT_INVALID );				//读出游戏世界ID
		
// 		gameWorld.dwTips[0] = m_pVarContainer->GetDword(_T("tip1"), (*iter).c_str(), GT_INVALID);
// 		gameWorld.dwTips[1] = m_pVarContainer->GetDword(_T("tip2"), (*iter).c_str(), GT_INVALID);
// 		gameWorld.dwTips[2] = m_pVarContainer->GetDword(_T("tip3"), (*iter).c_str(), GT_INVALID);
// 		gameWorld.dwTips[3] = m_pVarContainer->GetDword(_T("tip4"), (*iter).c_str(), GT_INVALID);
// 		gameWorld.dwTips[4] = m_pVarContainer->GetDword(_T("tip5"), (*iter).c_str(), GT_INVALID);


		_tcscpy(gameWorld.szIcon[0], m_pVarContainer->GetString(_T("icon1"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szIcon[1], m_pVarContainer->GetString(_T("icon2"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szIcon[2], m_pVarContainer->GetString(_T("icon3"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szIcon[3], m_pVarContainer->GetString(_T("icon4"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szIcon[4], m_pVarContainer->GetString(_T("icon5"), (*iter).c_str(), _T("")));

		_tcscpy(gameWorld.szTips[0], m_pVarContainer->GetString(_T("tips1"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szTips[1], m_pVarContainer->GetString(_T("tips2"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szTips[2], m_pVarContainer->GetString(_T("tips3"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szTips[3], m_pVarContainer->GetString(_T("tips4"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szTips[4], m_pVarContainer->GetString(_T("tips5"), (*iter).c_str(), _T("")));

		_tcscpy(gameWorld.szHttp[0], m_pVarContainer->GetString(_T("http1"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szHttp[1], m_pVarContainer->GetString(_T("http2"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szHttp[2], m_pVarContainer->GetString(_T("http3"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szHttp[3], m_pVarContainer->GetString(_T("http4"), (*iter).c_str(), _T("")));
		_tcscpy(gameWorld.szHttp[4], m_pVarContainer->GetString(_T("http5"), (*iter).c_str(), _T("")));

		strBourugh = m_pVarContainer->GetString(_T("borogh"), (*iter).c_str(), _T("")); 
		_tcscpy( gameWorld.szBoroughName, strBourugh.c_str() );
		_tcscpy( gameWorld.szRecommendName, m_pVarContainer->GetString(_T("recommendname"), (*iter).c_str(), _T("")) );
		gameWorld.eState= (EServerState)m_pVarContainer->GetDword(_T("number_state"), (*iter).c_str(), 0 );					

		if(gameWorld.byRecommend>0)	//推荐服
		{
			m_vRecommend.push_back(gameWorld);
			for(int i = 0; i < 5; i++)
			{
				tagServerState temp;
				_tcscpy(temp.szIcon, gameWorld.szIcon[i]);
				_tcscpy(temp.szTips, gameWorld.szTips[i]);
				_tcscpy(temp.szHttp, gameWorld.szHttp[i]);
				m_vTip.push_back(temp);
			}
			TCHAR szTemp[128] = {0};
			_tcscpy(szTemp, m_pVarContainer->GetString(_T("httpbutt"), (*iter).c_str(), _T("")));
			
			if(_tcscmp(szTemp, _T("")) != 0)
				_tcscpy(szButtHppt, szTemp);

			_tcscpy(szTemp, m_pVarContainer->GetString(_T("buttText"), (*iter).c_str(), _T("")));
			if(_tcscmp(szTemp, _T("")) != 0)
			{
				m_pBtnHttp->SetText(szTemp);
				m_pBtnHttp->SetInvisible(FALSE);
			}
		}

		dwBouroghID = m_pVarContainer->GetDword(_T("boroughid"), (*iter).c_str(), GT_INVALID); 
		DWORD dwBoroughRank = m_pVarContainer->GetDword(_T("boroughrank"), (*iter).c_str(), GT_INVALID );			//读出大区的编号
		
		//缓存
		bool bHave = false;
		int nBorough = 0;
		for(; nBorough<(int)m_vBrough.size(); ++nBorough)
		{
			if( _tcscmp(m_vBrough[nBorough].szBoroughName, strBourugh.c_str())==0 )
			{
				m_vBrough[nBorough].vGameWorld.push_back(gameWorld);
				bHave = true;
				break;
			}
				
		}
		if( !bHave )
		{
			tagBorough borogh;
			_tcscpy( borogh.szBoroughName, strBourugh.c_str() );
			borogh.dwBoroughID = dwBouroghID;
			borogh.dwBoroughRank = dwBoroughRank;
			m_vBrough.push_back(borogh);
			m_vBrough[nBorough].vGameWorld.push_back(gameWorld);
		}	
	}

	vector<tagBorough>::iterator bIter = m_vBrough.begin();
	for(; bIter!=m_vBrough.end(); bIter++)
	{
		bIter->dwWorldNum = (DWORD)bIter->vGameWorld.size();
		sort( bIter->vGameWorld.begin(), bIter->vGameWorld.end(), CompareGameWorldRank);
	}
	sort(m_vBrough.begin(), m_vBrough.end(), CompareBoroughRank);

	if ( m_bIfKoreaServer == FALSE )
	{
		sort(m_vRecommend.begin(), m_vRecommend.end(), CompareRecommendRank);
		for(int i=0; i<(int)m_vRecommend.size(); i++)
		{
			if(i>=RECOMMEND_GAMEWORLD)
				break;
			if( P_VALID(m_pRecommend[i]) )
			{
				m_pRecommend[i]->SetInvisible(FALSE);
				tstring strWorldName = m_vRecommend[i].szRecommendName;
				if(strWorldName.empty())
				{
					tstringstream strName;
					tstring strTemp = m_vRecommend[i].szBoroughName;
					INT nPos = strTemp.find_first_of(_T('（'));
					if( nPos != strTemp.npos )
						strTemp = strTemp.substr(0, nPos);
					//非大陆模式不显示大区名
					if (m_nLoginMode==1)
					{
						strName << _T("[") << strTemp << _T("] - ");
					}
					strName<< m_vRecommend[i].szWorldName;
					
					m_pRecommend[i]->SetText(strName.str().c_str());
				}
				else
					m_pRecommend[i]->SetText(m_vRecommend[i].szRecommendName);
				m_pRecommend[i]->SetTextColor(m_vRecommend[i].dwStatus);
				m_pRecommend[i]->SetPushTextColor(m_vRecommend[i].dwStatus);
				m_pRecommend[i]->SetMoveTextColor(m_vRecommend[i].dwStatus);

				switch(m_vRecommend[i].eState)
				{
				case ESS_All:
					{
						m_pRecIconHot[i]->SetInvisible(FALSE);
						m_pRecIconHot[i]->SetPic(strServerHot);
						m_pRecIconNew[i]->SetInvisible(FALSE);
						m_pRecIconNew[i]->SetPic(strServerNew);
					}
					break;
				case ESS_Hot:
					{
						m_pRecIconHot[i]->SetInvisible(FALSE);
						m_pRecIconHot[i]->SetPic(strServerHot);
						m_pRecIconNew[i]->SetInvisible(TRUE);
					}
					break;
				case ESS_New:
					{
						m_pRecIconHot[i]->SetInvisible(FALSE);
						m_pRecIconHot[i]->SetPic(strServerNew);
						m_pRecIconNew[i]->SetInvisible(TRUE);
					}
					break;
				default:
					{
						m_pRecIconHot[i]->SetInvisible(TRUE);
						m_pRecIconNew[i]->SetInvisible(TRUE);
					}
					break;
				}
			}
		}
		if(m_nLoginMode==1)
		{
			TCHAR szTemp[128] = {0};
			for(int i = 0; i < (int)m_vTip.size(); i++)
			{
				if(_tcscmp(m_vTip[i].szIcon, _T("")) != 0)
				{
					_stprintf(szTemp, _T("SeverSelectIcon%d"), i);
					m_pStcTip[i]->SetPic(m_vTip[i].szIcon);
					m_pStcTip[i]->SetTips(m_vTip[i].szTips);
					m_pStcTip[i]->SetInvisible(FALSE);
				}
			}
		}
		
	}

	if( !m_bOnlyGameWord )
	{
		for(int i=0; i<(int)m_vBrough.size(); i++)
		{
			if(i>=tempBOROUGH_MAX)
				break;
			if( P_VALID(m_pBorough[i]) )
			{
				m_pBorough[i]->SetInvisible(FALSE);
				m_pBorough[i]->SetText(m_vBrough[i].szBoroughName);
			}
		}
	}
	else
	{
		for(int i=0; i<BOROUGH_MAX; i++)
		{
			if( P_VALID(m_pBorough[i]) )
				m_pBorough[i]->SetInvisible(TRUE);
		}
		int nWorld = 0;
		for( int i = 0; i < (int)m_vBrough.size(); i++ )
		{
			if(i>=tempBOROUGH_MAX)
				break;
			for(int j=0; j<(int)m_vBrough[i].vGameWorld.size(); j++)
			{

				if( P_VALID(m_pBorough[nWorld]) )
				{
					m_pBorough[nWorld]->SetText(m_vBrough[i].vGameWorld[j].szWorldName);
					m_pBorough[nWorld]->SetInvisible(FALSE);
					m_pBorough[nWorld]->SetTextColor(m_vBrough[i].vGameWorld[j].dwStatus);
					m_pBorough[nWorld]->SetPushTextColor(m_vBrough[i].vGameWorld[j].dwStatus);
					m_pBorough[nWorld]->SetMoveTextColor(m_vBrough[i].vGameWorld[j].dwStatus);
					nWorld++;
				}
			}
		}
	}

	//非大陆模式没有大区选择
	if (m_nLoginMode!=1)
	{
		for (int i=0; i<BOROUGH_MAX; ++i)
		{
			m_pBorough[i]->SetInvisible(TRUE);
		}
	}

	ShowServerActive();
}

//读取上次登录过的游戏世界
void ServerSelectFrame::LoadLastLogin()
{
	const TCHAR* szFileName = _T("config\\lastlogin.xml");
	if(m_pVarIPSaver->Load(NULL, szFileName))
	{
		m_strLastWorldName	= m_pVarIPSaver->GetString(_T("gamename"), _T("last_login_server"), _T(""));
		m_strLastIP			= m_pVarIPSaver->GetString(_T("ip"), _T("last_login_server"), _T(""));	
		m_dwLastPort		= m_pVarIPSaver->GetDword(_T("port"), _T("last_login_server"), 0);	
		m_strLastServer		= m_pVarIPSaver->GetString(_T("server_name"), _T("last_login_server"), _T(""));	
		if(m_strLastIP.empty())
		{
			remove("config\\lastlogin.xml");
			if( P_VALID(m_pLastLogin) )
				m_pLastLogin->SetInvisible(TRUE);
			if( P_VALID(m_pLastServer) )
				m_pLastServer->SetInvisible(TRUE);

			return;
		}
		if( P_VALID(m_pLastLogin) )
		{
			m_pLastLogin->SetText(m_strLastWorldName.c_str());
			m_pLastLogin->SetInvisible(FALSE);
			m_pLastLogin->SetState(EGUIBS_PushDown, FALSE);
		}
		if( P_VALID(m_pLastServer) )
		{
			m_pLastServer->SetText(m_strLastServer.c_str());
			//非大陆模式不显示大区
			if (m_nLoginMode==1)
			{
				//m_pLastServer->SetInvisible(FALSE);  //这个不显示了~
			}
			else
			{
				m_pLastServer->SetInvisible(TRUE);
			}
		}
	}
	else
	{
		if( P_VALID(m_pLastLogin) )
			m_pLastLogin->SetInvisible(TRUE);
		if( P_VALID(m_pLastServer) )
			m_pLastServer->SetInvisible(TRUE);
	}
}

void ServerSelectFrame::SetRecommedPushButtonState()
{
	if ( m_bIfKoreaServer == TRUE)
		return ;	
	for(int i=0; i<(int)m_vRecommend.size(); i++)
	{
		if( P_VALID(m_pRecommend[i]) )
			m_pRecommend[i]->SetState(EGUIBS_PopUp, FALSE);
	}
	m_nRecoWorld = GT_INVALID;
}

void ServerSelectFrame::ShowWorldPushButton()
{
	if(m_nBorough==GT_INVALID)
		return;
	for(int i=0; i<GAMEWORLD_MAX; i++)
	{
		if( P_VALID(m_pGameWorld[i]) )
			m_pGameWorld[i]->SetInvisible(TRUE);
		if( P_VALID(m_pWorldIconHot[i]) )
			m_pWorldIconHot[i]->SetInvisible(TRUE);
		if( P_VALID(m_pWorldIconNew[i]) )
			m_pWorldIconNew[i]->SetInvisible(TRUE);
	}
	for(int i=0; i<(int)m_vBrough[m_nBorough].vGameWorld.size(); i++)
	{
		if( i< tempGAMEWORLD_MAX &&P_VALID(m_pGameWorld[i]) )
		{
			m_pGameWorld[i]->SetText(m_vBrough[m_nBorough].vGameWorld[i].szWorldName);
			m_pGameWorld[i]->SetInvisible(FALSE);
			m_pGameWorld[i]->SetTextColor(m_vBrough[m_nBorough].vGameWorld[i].dwStatus);
			m_pGameWorld[i]->SetPushTextColor(m_vBrough[m_nBorough].vGameWorld[i].dwStatus);
			m_pGameWorld[i]->SetMoveTextColor(m_vBrough[m_nBorough].vGameWorld[i].dwStatus);
			switch(m_vBrough[m_nBorough].vGameWorld[i].eState)
			{
			case ESS_All:
				{
					m_pWorldIconHot[i]->SetInvisible(FALSE);
					m_pWorldIconHot[i]->SetPic(strServerHot);
					m_pWorldIconNew[i]->SetInvisible(FALSE);
					m_pWorldIconNew[i]->SetPic(strServerNew);
				}
				break;
			case ESS_Hot:
				{
					m_pWorldIconHot[i]->SetInvisible(FALSE);
					m_pWorldIconHot[i]->SetPic(strServerHot);
					m_pWorldIconNew[i]->SetInvisible(TRUE);
				}
				break;
			case ESS_New:
				{
					m_pWorldIconHot[i]->SetInvisible(FALSE);
					m_pWorldIconHot[i]->SetPic(strServerNew);
					m_pWorldIconNew[i]->SetInvisible(TRUE);
				}
				break;
			default:
				{
					m_pWorldIconHot[i]->SetInvisible(TRUE);
					m_pWorldIconNew[i]->SetInvisible(TRUE);
				}
				break;
			}
		}
	}
}

//设置游戏世界按钮状态
void ServerSelectFrame::SetWorldPushButtonState()
{
	if(m_nBorough==GT_INVALID)
		return;
	for(int i=0; i<(int)m_vBrough[m_nBorough].vGameWorld.size(); i++)
	{
		if( i< tempGAMEWORLD_MAX && P_VALID(m_pGameWorld[i]) )
			m_pGameWorld[i]->SetState(EGUIBS_PopUp, FALSE);
	}
	m_nGameWorld = GT_INVALID;
}

void ServerSelectFrame::ShowServerActive()
{
	//<!注释原因：F计划中没有相应功能

	//vector<tagSerActiveData> actmap;
	//actmap = SerActiveData::Inst()->GetServerActiveVecData();
	//if( actmap.size() <= 0)
	//	return;

	////星期日:0
	//BYTE byToday = ServerTime::Inst()->GetCurrentWeekDay();
	//BYTE byTomorrow = 0;
	//if( byToday == 6 )
	//	byTomorrow = 0;
	//else
	//	byTomorrow = byToday+1;

	////获得当前月和日
	//BYTE byCurMonth = ServerTime::Inst()->GetCurrentMonth();
	//BYTE byCurDay   = ServerTime::Inst()->GetCurrentDay();
	//
	//TCHAR szBuff[X_SHORT_NAME] = {0};
	//INT nRow = 0;
	//INT nRow1 = 0;
	//INT nCol = 0;
	//DWORD dwColor = 0xFFFFF2A4;
	//for( vector<tagSerActiveData>::iterator it=actmap.begin(); it!=actmap.end(); ++it )
	//{
	//	nCol = 0;
	//	const tagSerActiveData& data  = (*it);
	//	
	//	//日过是节日活动，即按照月和日开启的
	//	if( Is_FastivalActivity(data.eStartMode) )
	//	{
	//		if( byCurMonth == (BYTE)data.nMonth && byCurDay == (BYTE)data.nDay )
	//		{
	//			TCHAR szMonthDay[X_SHORT_NAME] = {0};
	//			_stprintf( szMonthDay, g_StrTable[_T("Activity_FestivalMonthDay")], data.nMonth, data.nDay );
	//			
	//			m_pToday->SetText( nRow, nCol++, data.strActiveName.c_str(), data.dwColor );
	//			m_pToday->SetText( nRow, nCol++, szMonthDay, data.dwColor );
	//			m_pToday->SetText( nRow, nCol++, data.strContent.c_str(), data.dwColor );
	//			++nRow;
	//		}
	//		else if( byCurMonth == (BYTE)data.nMonth && (byCurDay+1) == data.nDay )
	//		{
	//			TCHAR szMonthDay[X_SHORT_NAME] = {0};
	//			_stprintf( szMonthDay, g_StrTable[_T("Activity_FestivalMonthDay")], data.nMonth, data.nDay );
	//			m_pTomorrow->SetText( nRow1, nCol++, data.strActiveName.c_str(), data.dwColor );
	//			m_pTomorrow->SetText( nRow1, nCol++, szMonthDay, data.dwColor );
	//			m_pTomorrow->SetText( nRow1, nCol++, data.strContent.c_str(), data.dwColor );
	//			++nRow1;
	//		}
	//		continue;
	//	}

	//	//今天是星期日
	//	if( byToday==0 )
	//	{
	//		if( data.byDate[MAX_WEEK-1] != 0 ) 
	//		{
	//			m_pToday->SetText( nRow, nCol++, data.strActiveName.c_str(), data.dwColor );
	//			m_pToday->SetText( nRow, nCol++, data.strTime.c_str(), data.dwColor );
	//			m_pToday->SetText( nRow, nCol++, data.strContent.c_str(), data.dwColor );
	//			++nRow;
	//		}
	//	}
	//	else
	//	{
	//		if( data.byDate[byToday-1] != 0 )
	//		{
	//			m_pToday->SetText( nRow, nCol++, data.strActiveName.c_str(), data.dwColor );
	//			m_pToday->SetText( nRow, nCol++, data.strTime.c_str(), data.dwColor );
	//			m_pToday->SetText( nRow, nCol++, data.strContent.c_str(), data.dwColor );
	//			++nRow;
	//		}
	//	}

	//	nCol = 0;
	//	//明天是星期日
	//	if( byTomorrow==0 )
	//	{
	//		if( data.byDate[MAX_WEEK-1] != 0 ) 
	//		{
	//			m_pTomorrow->SetText( nRow1, nCol++, data.strActiveName.c_str(), data.dwColor );
	//			m_pTomorrow->SetText( nRow1, nCol++, data.strTime.c_str(), data.dwColor );
	//			m_pTomorrow->SetText( nRow1, nCol++, data.strContent.c_str(), data.dwColor );
	//			++nRow1;
	//		}
	//	}
	//	else
	//	{
	//		if( data.byDate[byTomorrow-1] != 0 )
	//		{
	//			m_pTomorrow->SetText( nRow1, nCol++, data.strActiveName.c_str(), data.dwColor );
	//			m_pTomorrow->SetText( nRow1, nCol++, data.strTime.c_str(), data.dwColor );
	//			m_pTomorrow->SetText( nRow1, nCol++, data.strContent.c_str(), data.dwColor );
	//			++nRow1;
	//		}
	//	}
	//}

	//m_pToday->SetCurSelectedRow(GT_INVALID);
	//m_pTomorrow->SetCurSelectedRow(GT_INVALID);
}

void ServerSelectFrame::InitLoginMode()
{
	bIfInitLoginMode = TRUE;
	TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
	TCHAR szTemp[X_LONG_NAME*2];

	_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
	tstring strAppPath = szTemp;

	tstring strIniFileFullName = strAppPath + szIniFileName;
	::GetPrivateProfileString(_T("Launcher"), _T("LoginMode"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
	tstring policy = szTemp;
	if( policy == _T("") )
	{
		MessageBox(NULL, _T("Don't find LoginMode !"), _T(""), MB_OK);
		return;
	}
	m_nLoginMode = _ttoi(policy.c_str());
	switch(m_nLoginMode)
	{
	case 1:
	case 2:
		m_bOnlyGameWord = false;
		break;
	case 0:
	case 3:
	case 4:
		m_bOnlyGameWord = true;
		break;
	case 5:  //5表示是韩服，不显示推荐游戏   
		m_bIfKoreaServer  = TRUE;
		tempBOROUGH_MAX   = BOROUGH_MAX_KOREA;
		tempGAMEWORLD_MAX = GAMEWORLD_MAX_KOREA;
		break;
	default:
		m_bOnlyGameWord = false;
		break;
	}
}

INT ServerSelectFrame::GetBorough( INT nGameWorld )
{
	int nTotal = 0;
	for( int i = 0; i < (int)m_vBrough.size(); i++ )
	{
		nTotal += m_vBrough[i].dwWorldNum;
		if( nGameWorld < nTotal )
		{
			return i;
			break;
		}	
	}
	return GT_INVALID;
}

INT ServerSelectFrame::GetWolrd( INT nGameWorld )
{
	int nTotal = 0;
	for( int i = 0; i < (int)m_vBrough.size(); i++ )
	{
		for(int j=0; j<(int)m_vBrough[i].vGameWorld.size(); j++)
		{
			if( nTotal == nGameWorld )
				return j;
			else
				nTotal++;
		}
	}
	return GT_INVALID;
}

VOID ServerSelectFrame::GetSpecialValue()
{
	string strCommandLine = m_pUtil->UnicodeToAnsi( GetCommandLine() );
	if( strCommandLine.empty() || strCommandLine.length() > 1024 )
		return;
	const char *pCommandLine = strstr( strCommandLine.c_str(), ".exe" );
	if( pCommandLine == NULL )
	{
		return;
	}
	if(10 != m_nLoginMode && 11 != m_nLoginMode)
		m_nLauncherValue = atoi(&pCommandLine[6]);
	else
		m_nLauncherValue =0;

	TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
	TCHAR szTemp[X_LONG_NAME*2];

	_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
	tstring strAppPath = szTemp;

	tstring strIniFileFullName = strAppPath + szIniFileName;
	::GetPrivateProfileString(_T("Config"), _T("Distribution"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
	tstring policy = szTemp;
	if( policy == _T("") )
		policy = _T("0");

	m_nDispValue = _ttoi(policy.c_str());
}
