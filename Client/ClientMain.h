//-----------------------------------------------------------------------------
//!\file ClientMain.h
//!\author Lyp
//!
//!\date 2008-03-27
//! last 2008-03-27
//!
//!\brief 龙客户端
//!
//!	Copyright (c) 2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

struct tagNC_AnswerKoreaCSApprove;
//-----------------------------------------------------------------------------
// Client
//-----------------------------------------------------------------------------
class Client
{
public:
	enum LoadRequiementsLevel
	{
		E_Level_BeforeConnect,
		E_Level_BeforeSelectRole,
		E_Level_BeforeEnterWorld,
	};
	BOOL Init(HINSTANCE hInst);
	VOID Mainloop();
	VOID Destroy();

	VOID WaitThreadLoad();
	VOID ExitThreadLoad();
	BOOL IsLoadFinished()const;
	void GetLoadCount(LoadRequiementsLevel level, int& nLoaded, int& nTotal)const;

	Client():m_Trunk(this){}

private:
	TSFPTrunk<Client>			m_Trunk;
	TObjRef<InputDX8>			m_pInput;
	TObjRef<GameInputMap>		m_pInputMap;
	TObjRef<Util>				m_pUtil;
	TObjRef<WindowWrap>			m_pWindow;
	TObjRef<LuaManager>			m_pLuaMgr;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<GUIEditor>			m_pGUIEditor;
	TObjRef<Console>			m_pConsole;
	TObjRef<GUIRender>			m_pGUIRender;
	TObjRef<GUIEditor>			m_pEditor;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<GameScriptMgr>		m_pGameScriptMgr;
	TObjRef<NetSession>			m_pSession;
	TObjRef<AudioSys>			m_pAudioSys;


	BOOL InitRenderSystem(HINSTANCE hInst);

	VOID RegisterControl();
	VOID RegisterFrame();

	// GUIWnd显示开关
	DWORD SwitchGUIWnd(DWORD dwID, BOOL bDown);
	// 截图
	DWORD ScreenShot(DWORD dwID, BOOL bDown);

	// GM命令处理
	DWORD GMCommand(LPCTSTR szCommand);

	// 测试 
	DWORD Test();
	
	// 将3D引擎资源池状态写入日志文件
	DWORD LogResPool(LPCTSTR szCommand);

	// 显示本地角色所有属性
	DWORD DisplayLocalRoleAtt(LPCTSTR szCommand);

	//界面特效测试小工具 add by xp
	DWORD UiCameraTool(LPCTSTR szCommand);

	// 关闭/开起头顶tips
	DWORD HeadTipsCommand(LPCTSTR szCommand);

	// 装载服务器命令log
	DWORD NetCmdViewer(LPCTSTR szCommand, LPCTSTR szStart, LPCTSTR szEnd);

    // 删除一个文件夹下所有文件
    BOOL DeleteDirectory(LPCTSTR szDirName);
	
	HANDLE	m_hThreadLoad;	// 读取线程的HANDLE
	VOLATILE BOOL m_bTerminateThreadLoad; // 是否结束读取线程
	// 读取资源文件的线程
	UINT ThreadLoad(LPVOID);

	INT m_nLoadStep;
	BOOL m_bSDOA;			// 盛大模式
	INT m_nLogin;			// 登陆模式
	VOID InitSDOAMode();	
	VOID InitJinShanKuaiKuai();	// 金山快快
	VOID LoadJSKKLibary();			// 动态加载金山快快dll

	// 获取服务器信息
	DWORD GetServiceInfo(LPCTSTR szCommand);

	// Russia
	HINSTANCE			m_hInstIGWDll;
};

