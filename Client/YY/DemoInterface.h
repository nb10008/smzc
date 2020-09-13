#pragma once
#include <Windows.h>


typedef int (*PFN_INITYYINTERFACE)();	                //初始化接口
typedef FARPROC (*PFN_GETYYINTERFACE)( const char * );  //获得接口函数


typedef int (*PFN_RUNSERVICE)( const char* );
typedef int (*PFN_LOADINGAME)( const char* );
typedef int (*PFN_FREEINGAME)();


typedef int (*PFN_MOUSEINPUT)( UINT );
typedef int (*PFN_SETMAINWND)( HWND );
typedef int (*PFN_CREATEUI)( void* );
typedef int (*PFN_DESTORYUI)();
typedef int (*PFN_RENDERGUI)();
typedef int (*PFN_GAMEWNDMSG)( UINT, WPARAM, LPARAM );


typedef int (*PFN_JOINCHANNEL)( int );
typedef int (*PFN_SETTEAMADMIN)( int );
typedef int (*PFN_SETUSERNAME)( const char* );
typedef int (*PFN_JOINTEAM)( const char*, int );
typedef int (*PFN_SETTEAMDEVICE)( int, const char* );
typedef int (*PFN_SETTEAMVOICE)( int, int );
typedef int (*PFN_LOCKTEAMVOICE)( int , int );


typedef int (*PFN_GETPIPSHOW)();
typedef int (*PFN_SETPIPSHOW)( int );
typedef int (*PFN_SETMSGSHOW)( int );
typedef int (*PFN_SETMOUSESHOW)( int );
typedef int (*PFN_LOCKWINDOW)( int );
typedef int (*PFN_MOVEWINDOW)( int, int );


typedef int (*PFN_LOGINSUCCEED)();
typedef int (*PFN_LOGINFAILURE)( const char* );
typedef int (*PFN_SHOWOPENID)( int, int, int );
typedef int (*PFN_SETLOGINKEY)( const char* );
typedef int (*PFN_SETCALLBACK)( void* );
typedef int (*PFN_CHECKCLIENT)( const char*,void* );


class CDemoInterface
{
private:
    HMODULE m_hFactory;
    PFN_GETYYINTERFACE  m_pfnGetInterface;
    PFN_INITYYINTERFACE m_pfnInitInterface;
	BOOL m_bInited;

public:
    CDemoInterface(void);
    ~CDemoInterface(void);

    BOOL InitFactory();
	BOOL HaveInited(){return m_bInited;}

    /////////////////////////////////////////////////////////////////////////
    //all game must be call this func to show yy window
    PFN_RUNSERVICE m_pfnRunService;
    PFN_LOADINGAME m_pfnLoadInGame;
    PFN_FREEINGAME m_pfnFreeGame;

    /////////////////////////////////////////////////////////////////////////
    //sometimes call this func to show yy window
    PFN_MOUSEINPUT m_pfnMouseInput;
    PFN_SETMAINWND m_pfnSetMainWnd;
    PFN_CREATEUI   m_pfnCreateUI;
    PFN_DESTORYUI  m_pfnDestoryUI;
    PFN_RENDERGUI  m_pfnRenderGUI;
    PFN_GAMEWNDMSG m_pfnGameWndMsg;

    /////////////////////////////////////////////////////////////////////////
    //game used yy voice channel
    PFN_JOINCHANNEL     m_pfnJoinChannel;    
    PFN_SETTEAMADMIN    m_pfnSetTeamAdmin;
    PFN_SETUSERNAME     m_pfnSetUserName;
    PFN_JOINTEAM        m_pfnJoinTeam;
    PFN_SETTEAMDEVICE   m_pfnSetTeamDevice;
    PFN_SETTEAMVOICE    m_pfnSetTeamVoice;
    PFN_LOCKTEAMVOICE   m_pfnLockTeamVoice;

    /////////////////////////////////////////////////////////////////////////
    //game to channel yy voice window
    PFN_GETPIPSHOW      m_pfnGetPipShow;
    PFN_SETPIPSHOW      m_pfnSetPipShow;
    PFN_SETMSGSHOW      m_pfnSetMsgShow;
    PFN_SETMOUSESHOW    m_pfnMouseShow;
    PFN_LOCKWINDOW      m_pfnLockWnd;
    PFN_MOVEWINDOW      m_pfnMoveWnd;

    /////////////////////////////////////////////////////////////////////////
    //game to used open id func 
    PFN_SHOWOPENID      m_pfnShowOpid;
    PFN_LOGINSUCCEED    m_pfnLoginSucceed;
    PFN_LOGINFAILURE    m_pfnLoginFailure;
	PFN_LOGINFAILURE    m_pfnLoginCancel;
    PFN_SETLOGINKEY     m_pfnSetLoginKey;
    PFN_SETCALLBACK     m_pfnSetCallBack;
	PFN_CHECKCLIENT     m_pfnCheckClient;
};

extern CDemoInterface g_yyDemo;


