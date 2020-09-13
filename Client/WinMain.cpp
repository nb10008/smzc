//-----------------------------------------------------------------------------
//!\file winmain.cpp
//!\author Lyp
//!
//!\date 2008-04-20
//! last 2008-04-22
//!
//!\brief 客户端入口函数
//!
//!	Copyright (c) 2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "ClientMain.h"
#include "Render2D.h"

#include "AudioSys.h"
VOID InitDump();

#ifdef ON_X_TRAP
DWORD WINAPI XTrapCallbackAliveThreadFunc(LPVOID lpParam);
#endif
#ifdef ON_GAMEGUARD
CNPGameLib g_npgl(_T("VanOnlineKR"));
#endif
//-----------------------------------------------------------------------------
// 入口
//-----------------------------------------------------------------------------
INT APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR lpCmdLine, INT)
{
	// XTrap
#ifdef ON_X_TRAP
	TCHAR szIniFileName[512] = _T("config\\config.ini");
	const int MAX_STRING_SIZE = 512;
	TCHAR szTemp[512*2];

	// 获取EXE路径
	GetModuleFileName(NULL, szTemp, sizeof(szTemp)/sizeof(TCHAR));
	TCHAR *find=::_tcsrchr(szTemp,_T('\\'));
	if(find!=NULL)
	{
		*find=0;
		find=::_tcsrchr(szTemp,_T('\\'));
		if(find!=NULL)
		{
			*(find+1)=0;
			_tcslwr_s(szTemp,sizeof(szTemp)/sizeof(TCHAR));
		}
	}

	tstring m_strAppPath = szTemp;

	tstring m_strIniFileFullName = m_strAppPath + szIniFileName;

	::GetPrivateProfileString(_T("Config"), _T("cfgname"), NULL,  szTemp, 512*2, m_strIniFileFullName.c_str());
	char buf[1024]={0};
	WideCharToMultiByte(CP_ACP, 0, szTemp, wcslen(szTemp), buf, 1024, NULL, NULL);
	XTrap_C_Start((LPCSTR)buf,NULL);
#endif

	//韩国GameGuard
#ifdef ON_GAMEGUARD
	DWORD dwResult = g_npgl.Init();
	if (dwResult != NPGAMEMON_SUCCESS)
	{
		// 每个错误代码的处理方法请参考后面的例子
		TCHAR msg[256];
		LPCTSTR lpszMsg;
		// 相关信息的显示请参考“6 主要错误代码”
		switch (dwResult)
		{
		case NPGAMEMON_ERROR_EXIST:
			lpszMsg = _T("GameGuard 正在运行，请重启后重新执行游戏。");
			break;
		case NPGAMEMON_ERROR_GAME_EXIST:
			lpszMsg = _T("游戏重复执行，或GameGuard已经运行，结束游戏后请重试。");
			break;
		case NPGAMEMON_ERROR_INIT:
			lpszMsg = _T("GameGuard初始化错误。请重启后执行游戏，或关闭有冲突的程序。");
			break;
		case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
		case NPGAMEMON_ERROR_NFOUND_GG:
		case NPGAMEMON_ERROR_AUTH_INI:
		case NPGAMEMON_ERROR_NFOUND_INI:
			lpszMsg = _T("GameGuard 文件不存在或已损坏，请重新安装GameGuard 文件。");
			break;
		case NPGAMEMON_ERROR_CRYPTOAPI:
			lpszMsg = _T("Windows系统文件有可能被损坏，请重新安装Internet Explorer(IE)。");
			break;
		case NPGAMEMON_ERROR_EXECUTE:
			lpszMsg = _T("GameGuard执行失败。请重新安装GameGuard文件。");
			break;
		case NPGAMEMON_ERROR_ILLEGAL_PRG:
			lpszMsg = _T("检测到非法程序，请关闭与游戏无关的程序后执行游戏。");
			break;
		case NPGMUP_ERROR_ABORT:
			lpszMsg = _T("GameGuard更新被中断，请检查网络状况或个人防火墙设置。");
			break;
		case NPGMUP_ERROR_CONNECT:
			lpszMsg = _T("连接GameGuard更新服务器失败。请检查网络设置或重新连接。");
			break;
// 		case HOOK_TIMEOUT:
// 			lpszMsg = "病毒或间谍软件导致Hook失败，请下载最新的杀毒软件扫描您的电脑。";
// 			break;
		case NPGAMEMON_ERROR_GAMEGUARD:
			lpszMsg = _T("旧的版本的GameGuard初始化错误。请重新安装GameGuard文件后再执行游戏。");
			break;
		case NPGMUP_ERROR_PARAM:
			lpszMsg = _T("ini 文件不存在或被更改。请重新安装GameGuard文件。");
			break;
		case NPGMUP_ERROR_INIT:
			lpszMsg = _T("npgmup.des初始化错误。请删除GameGuard文件夹后重新执行游戏。");
			break;
		case NPGMUP_ERROR_DOWNCFG:
			lpszMsg = _T("连接GameGuard更新服务器失败。请稍后重试或检查个人防火墙的设置。");
			break;
		case NPGMUP_ERROR_AUTH:
			lpszMsg = _T("GameGuard更新未完成。请暂时禁用杀毒软件并重新执行游戏，或检查您电脑的设置。");
			break;
		case NPGAMEMON_ERROR_NPSCAN:
			lpszMsg = _T("因内存空间不足或已感染病毒，病毒和间谍程序扫描模块加载失败。");
			break;
// 		case NPGAMEMON_ERROR_COLLSION：
// 			IPszMsg="您电脑中存在其他软件和GameGuard存在冲突，请关闭这些软件后再执行游戏";
// 			break;
		default:
			// 显示相关信息.
			lpszMsg = _T(" GameGuard运行发生错误，请发送GameGuard文件夹下的gameservice@3zsafe.com");
			break;
		}
		wsprintf(msg, _T("GameGuard:%lu"), dwResult);
		MessageBox(NULL, _T("Error"), msg, MB_OK);
		return 0;
	}
#endif
	

	// 为了让Launcher程序识别客户端是否在运行，创建一个互斥量
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("sm_client"));

	// 禁止屏幕保护及电源管理
	::SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 0, NULL, 0);

	vEngine::InitNetwork();
	vEngine::InitEngine();
	_set_se_translator( vEngine::SEH_Translation );
	InitDump();

	// 向vEngine注册客户端自己实现的模块
	vEngine::Register(TWrap<Client>(),			"Client");
	vEngine::Register(TWrap<GUIRender>(),		"GUIRender");
	vEngine::Register(TWrap<GameFrameMgr>(),	"GameFrameMgr");
	vEngine::Register(TWrap<GameScriptMgr>(),	"GameScriptMgr");
	vEngine::Register(TWrap<NetSession>(),		"NetSession");

	vEngine::Register(TWrap<GameInputMap>(),	"GameInputMap");
	vEngine::Register(TWrap<MsgBox>(),			"MsgBox");
	vEngine::Register(TWrap<MsgBoxEx>(),		"MsgBoxEx");
	vEngine::Register(TWrap<MsgInputBox>(),		"MsgInputBox");
	vEngine::Register(TWrap<MsgMoneyInputBox>(),"MsgMoneyInputBox");
	vEngine::Register(TWrap<MsgOptionBox>(),    "MsgOptionBox");

	// 音乐音效系统
	vEngine::Register(TWrap<AudioSys>(),		"AudioSys");

	//try
	//{
		TObjRef<Client> pClient;
		if( pClient->Init(hInst) )
		{
			#ifdef ON_X_TRAP
			DWORD dwThreadId;
			::CreateThread(NULL,0,XTrapCallbackAliveThreadFunc,NULL,0,&dwThreadId);
			#endif

			#ifdef ON_GAMEGUARD
			HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));
			g_npgl.SetHwnd(hWnd);
			#endif
			pClient->Mainloop();
			pClient->Destroy();
		}
	//} 
	//catch(vEngine::Exception)
	//{
	//	throw;	// 方便调试器调试
	//}

#ifdef ON_X_TRAP
	XTrap_C_Terminate();
#endif

	vEngine::DestroyEngine();
	vEngine::DestroyNetwork();

	::CloseHandle(hMutex);
	exit(0);
	return 0;
}


//-----------------------------------------------------------------------------
// 得到Dump FTP配置
//-----------------------------------------------------------------------------
VOID InitDump()
{
	TObjRef<VarContainer> pCfg = CreateObj("DumpConfig", "VarContainer");

	if( pCfg->Load(NULL, _T("config\\DumpCfg.xml")) )
	{
		const int MAX_STRING_SIZE = 512;
		TCHAR szVer[MAX_STRING_SIZE] = {0};
		::GetPrivateProfileString(_T("version"), _T("latest_game"), NULL,  szVer, MAX_STRING_SIZE, _T("launcher\\patchinfo.txt"));

		vEngine::InitDump(pCfg->GetString(_T("ftp")), 
			pCfg->GetString(_T("user")), pCfg->GetString(_T("password")), szVer);
	}

	KillObj("DumpConfig");
}

#ifdef ON_X_TRAP
DWORD WINAPI XTrapCallbackAliveThreadFunc(LPVOID lpParam)
{
	DWORD dwPeriod = 20000;
	while (TRUE)
	{
		Sleep(dwPeriod);
		XTrap_C_CallbackAlive(dwPeriod);
	}
	return 0;
}

#endif 

