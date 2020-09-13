//-----------------------------------------------------------------------------
//!\file LoginFrame.cpp
//!\author Lyp xtian
//!
//!\date 2008-02-17
//! last 2009-5-31
//!
//!\brief login
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "LoginFrame.h"
#include "..\WorldDefine\login.h"
#include ".\SeleRoleMgr.h"
#include "shellapi.h"

#include "AudioSys.h"
#include "ClientMain.h"
#include "HackShield.h"
#include "ServerTime.h"
#include "GameSet.h"
#include "../WorldDefine/msg_fatigue_guarder.h"
#include <iphlpapi.h>
#include "LoadingFrame.h"
#include "SaveAccount.h"
#include "HealthyControlFrame.h"
#include "SDOAIgw.h"
#include "MapMgr.h"
#include "SelectRoleFrame.h"
#include "../WorldDefine/login.h"
#include "CameraSetData.h"
#include "HttpPicEx.h"
#include "ToolTipFrame.h"
#include "..\vEngine\network\HttpDownload.h"
#include "jskk/kksimpleloginhelper.h"
#include "jskk/signverify.h"
#include "jskk/kksimplelogin.h"
#include "YY\DemoInterface.h"
#include "SendHttpPost.h"

#include <fstream>

#include <NtDDNdis.h>
#include <winioctl.h>


typedef INT (WINAPI *FnKKGetActiveUserToken)( WCHAR* strTokenBuffer, ULONG nBufferLength );




#pragma comment(lib, "IPHlpAPI.lib")
bool g_bCommandLine = false;
static BOOL bSendOnceFlag = FALSE;
bool	 g_bLoadYY= false;				// 是否加初始化过YY

//open passport
static BOOL g_bUnrealInit = FALSE;			//假登陆初始化
static BOOL g_bUnrealSend = FALSE;		//假登陆消息（只发1次）
static BOOL g_bSendDoneMsg = FALSE;	//图片全下载完发消息（只发1次） 

void ShowYYLog(string sLogInfo)
{
	return;
	// 等待YY上线稳定后再完全删除日志
	ofstream wFileStream;
	wFileStream.open("e:\\smzx_log.txt",ios::out|ios::app/*|ios::trunc*/);
	if (wFileStream)
	{
		wFileStream <<  sLogInfo <<endl;
		wFileStream.close();
	}
}

// YY回调函数
int YYPIPCallBack( int nType, WPARAM wParam )
{
	stringstream ss;
	ss << nType << "---" << wParam;
	ShowYYLog(ss.str());
	// 游戏客户端通知验证服务器
	if( 2 ==nType )
	{
		LoginFrame* pFrame = (LoginFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Login"));
		if (P_VALID(pFrame))
		{
			SaveAccount::Inst()->SetYYLoginMode(true);
			ShowYYLog("YY混服登陆开始：一共六个步骤 ");
			ShowYYLog("步骤1：客户端和YY通讯成功，准备连接服务器 ");
			pFrame->SetYY_GetTokenIDorNot(true);
			pFrame->Connect();
			
		}
	}

	else if( 3 ==nType )
	{
		char szMess[X_HUGE_NAME*2];
		sprintf(szMess, "%s",wParam);
		string ss = szMess;
		SaveAccount::Inst()->SetYY_GUID(ss);
		LoginFrame* pFrame = (LoginFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Login"));
		if (P_VALID(pFrame))
		{
			ShowYYLog("步骤4：拿到YY返回的GUID ");
			ShowYYLog(ss);
			pFrame->SetYY_GetTokenIDorNot(false);
			pFrame->Connect();
			
		}
	}

// 	else if(6 == nType)
// 	{
// 		//YY错误调用failure，其他的错误调用cancel
// 		g_yyDemo.m_pfnLoginFailure("login fail");
// 	}

	return nType;
}


// 获得金山快快token
INT GetToken(WCHAR* wcsToken, ULONG nBuffer)
{
	INT nRet = KK_SIMPLELOGIN_FAILED;
	// 快快是否安装
	std::wstring strKKGamePath;
	BOOL bInstall = isKKGameInstalled(strKKGamePath);
	if (bInstall)
	{
		// 获取kkslgn.dll路径
		std::wstring strSimpleLoginPath = strKKGamePath;
		strSimpleLoginPath += L"\\kkslgn.dll";

		// 检查数字签名，以校验kkslgn.dll的有效性
		if (_waccess(strSimpleLoginPath.c_str(), 0) == 0
			&& VerifySignFile(strSimpleLoginPath.c_str()) == NO_ERROR)
		{
			// 通过导出函数获取token
			HMODULE hModule = LoadLibrary(strSimpleLoginPath.c_str());
			if (hModule)
			{
				FnKKGetActiveUserToken fun = (FnKKGetActiveUserToken)GetProcAddress(hModule, "kkGetActiveUserToken");
				if (fun)
				{
					nRet = fun(wcsToken, nBuffer);
				}

				FreeLibrary(hModule);
			}
		}
	}
	return nRet;
}


//获得本机mac地址
void __fastcall GetMAC(string& strMAC)   
{
	IP_ADAPTER_INFO AdapterInfo[10];
	PIP_ADAPTER_INFO pAdapt;   
	DWORD   AdapterInfoSize;  
	AdapterInfoSize = sizeof(AdapterInfo);
	if(GetAdaptersInfo(AdapterInfo,&AdapterInfoSize)!=NO_ERROR)
		return;
	pAdapt = AdapterInfo;
	std::string str("");   
	while(pAdapt) 
	{ 	
		if(pAdapt->Description)
		{
			CHAR pstrBuf[512] = {0};
			strcpy(pstrBuf, "\\\\.\\");
			strcat(pstrBuf, pAdapt->AdapterName);
			HANDLE hDev = CreateFileA(pstrBuf, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
			if( hDev != INVALID_HANDLE_VALUE )
			{
				int inBuf;
				BYTE outBuf[256] = {0};
				DWORD bytesReturned;
				inBuf = OID_802_3_PERMANENT_ADDRESS;
				if(DeviceIoControl(hDev, IOCTL_NDIS_QUERY_GLOBAL_STATS, (LPVOID)&inBuf, 4, outBuf, 256, &bytesReturned, NULL) )
				{
					for(int i=0; i<6; i++)   
					{      
						char szBuff[128] = {0};
						INT64 n64Value = INT64(outBuf[i]);
						sprintf(szBuff, "%02X", n64Value);
						str = str + szBuff;

					}
				}
				CloseHandle(hDev);
				break;
			}
			else
				pAdapt = pAdapt->Next;
		}
		else
			pAdapt = pAdapt->Next;
	}
	strMAC = str;
}

//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
LoginFrame::LoginFrame():m_Trunk(this)
{
	m_pWnd			= NULL;
	m_pWndKey		= NULL;
	//m_pWndCaption	= NULL;
	m_pEditAccount	= NULL;
	m_pEditPassword = NULL;
	m_pStcAccountText	= NULL;
	m_pStcPsswordText	= NULL;
	m_pButtonLogin	= NULL;
	m_pButtonQuit	= NULL;
	m_pBtnReturn	= NULL;

	m_pWndWorldList	= NULL;		
	m_pBtnOKWorld	= NULL;
	m_pBtnCancelWorld = NULL;
	m_pWorldList	= NULL;
	m_pPBtn_Tab		= NULL;
	m_bTab			= TRUE;

	m_eState		= ELS_WaitInput;
	m_dwStateTime	= timeGetTime();
	m_fPingTime		= 0.0f;
	//m_pAniCtrl		= NULL;
	//m_pTime			= 0.0f;
	//m_nPlayState	= Loading;
	m_bEnter		= FALSE;
	m_dwSentJoinGame = 0;
	m_bSelectedWorld = FALSE;
	m_bGuard		= FALSE;

	m_pWndQueue		= NULL;
	m_pStcTime		= NULL;
	m_pStcPosition	= NULL;
	m_pWndCancelQueue = NULL;
	m_pBtnCancelQueue = NULL;
	m_pStcTips		= NULL;
	m_eMB			= EMBP_NULL;	
	m_bMiBao		= FALSE;	
	m_pWndSecurity	= NULL;	
	m_pStcSecuHint	= NULL;	
	m_pStcSecuNum	= NULL;	
	m_pWndSeInputBack=NULL;	

	for(int i=0; i<10; i++)	
	{		
		m_pBtnSecuInput[0] = NULL;	
	}	
	m_pStcInputHint	= NULL;
	m_pBtnSecuOK	= NULL;	
	m_pBtnSecuCancel= NULL;	
	m_nPolicy		= GT_INVALID;	
	m_pPBtnSaveAccount	= NULL;
	m_pPBtnCamera		= NULL;
	m_bSave			= false;
	m_bSavePassword = false;
	m_hDll			= NULL;
	m_pStcSavePass	= NULL;
	//m_pStcUser		= NULL;
	m_eHint			= EXLHint_User;
	m_bLoginType	= false;
	ZeroMemory( m_szEncrypt, 100 );
	ZeroMemory( m_szUserName, 20 );
	m_bSendSDOAInfo = TRUE;
	m_nBase = 10;

	// 	m_LookAt = Vector3(10000, 10000, 10000);
	// 	m_LookFrom = Vector3(4500, 9000, 4500);
	const tagCameraData* pData = CameraSetData::Inst()->GetData();
	m_LookAt = Vector3(pData->dwLoginCameraLookAt_x, pData->dwLoginCameraLookAt_y, pData->dwLoginCameraLookAt_z);
	m_LookFrom = Vector3(pData->dwLoginCameraPos_x, pData->dwLoginCameraPos_y, pData->dwLoginCameraPos_z);
	m_bIsShake = pData->bIsShake;

	m_bLeft = false;
	m_bRight = false;
	m_fMAXTime = 0.0f;
	m_nAlpha = 0;
	m_dwColor = 0xFFFFFFFF;
	m_pTex = NULL;
	m_bRender = false;
	m_dwCurPos = 0;
	m_fParam = 1.0f;
	m_bAskTokenID = false;
	m_bShowID = false;
	m_dwConnID = 0;
	m_strKoreaLoginNumber = "";	
	m_strKoreaSetPassWordOne = "";
	m_strKoreaSetPassWordTwo = "";
	m_strKoreaChangePassWordOne = "";
	m_strKoreaChangePassWordTwo = "";
	m_strKoreaChangePassWordOld = "";
	m_n8KoreaTryTimes = 0;

	//
	m_pWndKoreaPassWord = NULL;	//韩国二次密码设置
	m_pBtnKoreaPassWordClose = NULL;//关闭
	m_pBtnKoreaChangeWorld = NULL;	//韩国修改二次密码
	m_pBtnKoreaLoginOK = NULL;		//韩国二次密码点击确定按钮，可能是
	m_pBtnKoreaLoginCancel = NULL;	//韩国二次密码取消按钮
	for(int i = 0; i < 10; i++)
	{
		m_pBtnKoreaWordKey[i] = NULL;
	}
	m_pBtnKoreaClear = NULL;		//清零按钮
	m_pBtnKoreaBack = NULL;		//回退按钮

	m_pWndFirstSetPassWord = NULL;	//第一次设置密码面板
	m_pStcKoreaFTip = NULL;		//那个提示信息框
	m_pStcFirstPassWord = NULL;	//第一次输入密码
	m_pStcSecondPassWord = NULL;	//第二次密码确认
	m_pStcFirstPassWordPic = NULL;	//选择第一次后显示的那个框
	m_pStcSecondPassWordPic = NULL;//选择第二次后显示的那个框

	m_pWndKoreaLogin = NULL;		//韩国二次密码登陆面板
	m_pStcKoreaLoginTip = NULL;	//登陆提示
	m_pStcLoginPassWord = NULL;	//登陆密码

	m_pWndKoreaChangePassWord = NULL;	//韩国修改二次密码
	m_pStcChangeWordTip = NULL;		//修改密码提示
	m_pStcOldPassWord = NULL;			//旧密码
	m_pStcNewPassWord = NULL;			//新密码
	m_pStcNewPassWordConfirm = NULL;	//新密码确认
	m_pStcOldPassWordPic = NULL;		//旧密码底图
	m_pStcNewPassWordPic = NULL;		//新密码底图
	m_pStcNewPassWordConfirmPic = NULL;//新密码确认底图

	for (int i=0;i<5;++i)
	{
		m_pStcUrlPic[i] = NULL;
		m_pPBUrl[i] = NULL;
	}
	m_pBtnPrev			= NULL;
	m_pBtnNext		= NULL;
	m_pBtnNormal	= NULL;
	m_nCurUrlPage	= 0;
	m_nCurChoose	= GT_INVALID;
	m_UpdateUrlTime = 0;
	m_dwDownLoadTime = 0;
	m_bOpenPassport = TRUE;
	m_bSendUrlLoginMsg = FALSE;
	m_bOpenWaitFlag = FALSE;
	m_dwCropID = 0;
	m_byOpenPassPotIndex = GT_INVALID;

	m_sendSave = NULL;
	m_bIsNeedSendAgain = TRUE;

}


//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
LoginFrame::~LoginFrame()
{
	
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL LoginFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	SeleRoleMgr::Inst()->Init();
	stringstream stream;
	stream << "XClient_Login" << this;
	m_pStream = CreateObj(stream.str().c_str(), "XClient");

	m_pStream->Init(FALSE);
	m_pVarContainer = CreateObj("login_ip_list", "VarContainer");
	m_bSelectedWorld = m_pVarContainer->Load(NULL, _T("config\\lastlogin.xml"));
	if(!m_bSelectedWorld )
		m_pStcHint->SetText(g_StrTable[_T("SelectWorld_NoServerSelect")]);

	switch( m_nPolicy )
	{
	case 1:
		{
			if (SaveAccount::Inst()->GetCropID())
			{
				bSendOnceFlag = FALSE;
				//HideLoginFrame( TRUE );
			}
			else if (OPEN_PASSPORT)
			{	// 大陆版本weblogin
				Connect(FALSE);
				OpenPassportLog("---------------------log---------------------");
			}
		}
		break;
	case 4: //盛大特殊处理
		{
			DWORD dwWorldID = m_pVarContainer->GetDword(_T("world_id"), _T("last_login_server"));
			DWORD dwServerID = m_pVarContainer->GetDword(_T("server_id"), _T("last_login_server"));
			SDOAIgw::Inst()->InitSDOAIGW(dwServerID, dwWorldID);
		}
		break;
	case 5:
			bSendOnceFlag = FALSE;
		break;
	case 7: // OMG mode
	case 9: // DK mode 豆客登陆模式
	case 10: // 俄罗斯登陆模式
		{
			bSendOnceFlag = FALSE;
			HideLoginFrame( TRUE );
		}
		break;
	case 8:
		m_b8sendLogin = false;
		break;
	
	}
	m_pFrameMgr->RegisterEventHandle(_T("Finish_Close_Loading"),(FRAMEEVENTPROC)m_Trunk.sfp1(&LoginFrame::OnFinishCloseLoading));
	m_pCmdMgr->Register("NS_JoinGame", (NETMSGPROC)m_Trunk.sfp2(&LoginFrame::NetRecvJoinGame), _T("NS_JoinGame"));
	m_pFrameMgr->RegisterEventHandle(_T("tagExitLoadingFrame"), (FRAMEEVENTPROC)m_Trunk.sfp1(&LoginFrame::OnExitLoadingFrame));
	m_pFrameMgr->RegisterEventHandle(_T("Msgbox_KoreaLoginErr3Times"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&LoginFrame::OnMsgbox_KoreaLoginErr3Times));
	// 播放登陆音乐
	SaveAccount::Inst()->SetCurMusic(0);
	TObjRef<AudioSys>()->StopMusic();
	TCHAR szBuff[X_SHORT_NAME] = {0};
	_sntprintf( szBuff, X_SHORT_NAME, _T("music_login%d"), 0 );
	TObjRef<AudioSys>()->PlayMusic(szBuff, FALSE, SOUND_LOUD);

	LoadMap();

    // 如果有防沉迷提示，则关掉
    HealthyControlFrame* pFrame = (HealthyControlFrame*)m_pFrameMgr->GetFrame(_T("HealthyControl"));
    if (P_VALID(pFrame))
        pFrame->ShowWnd(FALSE);

	/*
	if (  1 == SaveAccount::Inst()->GetLoginMode())
	{
	// 初始化YY
		g_yyDemo.InitFactory();
		if(g_yyDemo.HaveInited())
		{
			g_yyDemo.m_pfnLoadInGame("KZ_SMZX|YY3D9|HOOK9ALL");
			g_yyDemo.m_pfnCheckClient("..\\YY\\yy.zip",NULL);
			g_yyDemo.m_pfnSetCallBack( YYPIPCallBack );
			//m_bShowID = true;
		}
	}
	*/

	m_nCurUrlPage	= 0;
	m_nCurChoose	= GT_INVALID;
	m_bOpenPassport = TRUE;
	g_bUnrealInit = FALSE;
	g_bUnrealSend = FALSE;
	g_bSendDoneMsg = FALSE;
	m_bSendUrlLoginMsg = FALSE;
	m_bOpenWaitFlag = FALSE;

	m_bShowID = true;
	//JudegPolicy();
	return TRUE;
}

const float CamYaw=_DegToRad(0.0f);
const float CamPitch=_DegToRad(15.0f);//     _DegToRad(-6.5f);
const float CamRoll=0.0f;
const float MAXCamDist = 80000.0f;

VOID LoginFrame::LoadMap()
{
	SeleRoleMgr::Inst()->OpenMap(_T("denglu3"), SeleRoleMgr::Inst()->GetCamera(), SeleRoleMgr::EPlayer_Login);
	//SeleRoleMgr::Inst()->AttachSceneNodeToSG(SeleRoleMgr::Inst()->GetHugeSG());
	//SeleRoleMgr::Inst()->SetCamera(m_LookAt, CamYaw, CamPitch, CamRoll, MAXCamDist);
	SeleRoleMgr::Inst()->SetCamera(m_LookFrom,m_LookAt);
	//IFS* pFS=(IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_Map");
	////--加载场景、node 
	//m_pSG				= new NullSceneGraph;
	//m_EntityNode		= new EntityNode;	
	//m_EntityNode->LoadRes(_T("data\\model\\building\\denglu\\denglu.FAK"));
	//m_pSG->AttachDynamicNode(m_EntityNode);
	//m_pAniCtrl = (VertexAnimationCtrl*)m_EntityNode->GetAniCtrl();		
	//m_pResRenderable = m_EntityNode->GetResObj();

	////--设置设像机的位置
	//ASSERT(m_pSG!=NULL);
	//if( m_nPolicy == 3 )
	//	m_Camera.SetPerspective((FLOAT)Device()->GetClientWidth()/*1024*/, /*800*/(FLOAT)Device()->GetClientHeight(), 
	//	_DegToRad(30.0f), 10.0f, 20000.0f);
	//else
	//	m_Camera.SetPerspective((FLOAT)Device()->GetClientWidth()/*1024*/, /*800*/(FLOAT)Device()->GetClientHeight(), 
	//	_DegToRad(35.0f), 10.0f, 20000.0f);
	//m_Camera.Set(Lookat, CamYaw, CamPitch, CamRoll, MAXCamDist);

	//if( P_VALID(m_pSG) )
	//{
	//	m_pSG->BindCamera(&m_Camera);
	//}
	//
	////--设置人物的位置为原点
	//Transform trans;
	//trans.Translate(Vector3(0,0,0));
	////trans.Translate(Vector3(0,-50,0));
	//m_EntityNode->SetWorldMat(trans.GetMatrix());	

	//tagLight light;
	//light = (tagLight&)m_pSG->GetSunLight();
	//light.diffuse = Color4f(1.0f, 1.5f, 1.5f, 1.5f);
	//m_pSG->SetSunLight(light);
}

VOID LoginFrame::Render3D()
{
	float delTime = Kernel::Inst()->GetDeltaTime();	

	SeleRoleMgr::Inst()->UpdateSG(delTime);

	if( P_VALID(m_pTex) && m_bRender )
	{
		IDraw2D::Inst()->BeginDraw(IDraw2D::EBlend_Alpha, true);
		IDraw2D::Inst()->Draw( &m_rectDest, &m_rectSrc, m_pTex, m_dwColor, ETFilter_POINT, 0, IDraw2D::EAWT_Write, 0.1f );
		IDraw2D::Inst()->EndDraw();	
	}

	//if(!P_VALID(m_pResRenderable))
	//	return;
	//if(!m_pResRenderable->IsCreated())
	//	return;
	//if( !P_VALID(m_pAniCtrl) )
	//	return;

	//if( P_VALID(m_pSG) && m_nPlayState!=Loading )
	//	m_pSG->Render(delTime);

	//if( m_nPlayState == Loading )
	//{
	//	m_nPlayState = Preface;
	//	JudegPolicy();
	//}
	//
	//if( m_nPlayState == Preface )
	//{
	//	m_pAniCtrl->OpenTrack(_T("sta"), false);
	//	m_pTime = Kernel::Inst()->GetAccumTime();
	//	m_nPlayState = Loop;
	//	return;
	//}

	//if( m_nPlayState == Loop )
	//{
	//	if( m_pAniCtrl->GetTrackPosition(_T("sta")) > 1.4f )
	//	{
	//		if(!m_bLoginType && m_nPolicy != 4 )
	//			m_pWnd->SetInvisible(FALSE);
	//		//JudegPolicy();
	//	}
	//}

	//if( m_nPlayState==Loop && m_pAniCtrl->IsTrackEnd(_T("sta")) )
	//{
	//	m_pAniCtrl->OpenTrack(_T("mid"), true);
	//	if( !m_bLoginType && m_nPolicy != 4  )
	//		m_pWnd->SetInvisible(FALSE);
	//	m_nPlayState = Over;
	//	if(m_bLoginType)
	//		LoginGame();
	//	return;
	//}	
	//
	//if( m_nPlayState==Over && m_bEnter )
	//{
	//	int nLoaded, nTotal;
	//	m_pClient->GetLoadCount(Client::E_Level_BeforeConnect, nLoaded, nTotal);
	//	if(nTotal!=0 && nLoaded < nTotal)
	//	{
	//		::Sleep(20); // 给资源转载更多时间
	//	}
	//	else
	//	{
	//		m_pAniCtrl->OpenTrack(_T("end"), false);
	//		m_pTime = Kernel::Inst()->GetAccumTime();
	//			m_pWnd->SetInvisible(TRUE);
	//		m_nPlayState = Enter;
	//		return;
	//	}
	//}

	//if( m_nPlayState==Enter && m_pAniCtrl->IsTrackEnd(_T("end")) )
	//{	
	//	if( m_nPolicy == 4 )
	//	{
	//		SDOAIgw::Inst()->LeaveLogIn();
	//		SDOAIgw::Inst()->SetScreenStatus(SDOA_SCREEN_MINI);
	//	}
	//	SeleRoleMgr::Inst()->CloseMap();
	//	GameFrame* pFrame = m_pFrameMgr->CreateFrame(_T("Root"), _T("SelectRole"), _T("SelectRoleFrame"), 0);
	//	m_pFrameMgr->CreateFrame( _T("Root"), _T("OldPlayerComeBackFrame"), _T("OldPlayerComeBackFrame"), 0 );

	//	SeleRoleMgr::Inst()->Init();
	//	LoadingFrame* pFrame1 = (LoadingFrame*)m_pFrameMgr->CreateFrame(_T("Root"), _T("Loading"), _T("LoadingFrame"), 0);
	//	if( P_VALID(pFrame1) )
	//		pFrame1->SetLoadingType(LoadingFrame::ELT_SelectRole);

	//	m_pFrameMgr->AddToDestroyList(this);
	//}
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL LoginFrame::Destroy()
{
	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);
	if( P_VALID(m_pWndKey) )
		m_pGUI->AddToDestroyList(m_pWndKey);
	if(	P_VALID(m_pWndWorldList) )
		m_pGUI->AddToDestroyList(m_pWndWorldList);
	if( P_VALID(m_pWndSecurity) )
		m_pGUI->AddToDestroyList(m_pWndSecurity);
	if( P_VALID(m_pWndSeInputBack) )
		m_pGUI->AddToDestroyList(m_pWndSeInputBack);
	if( P_VALID(m_pWndQueue) )
		m_pGUI->AddToDestroyList(m_pWndQueue);
	if( P_VALID(m_pWndCancelQueue) )
		m_pGUI->AddToDestroyList(m_pWndCancelQueue);
	if( P_VALID(m_pWndKoreaPassWord) )
		m_pGUI->AddToDestroyList(m_pWndKoreaPassWord);

	//m_pSG->DetachDynamicNode(m_EntityNode);

	//if( P_VALID(m_EntityNode) )
	//	SAFE_DELETE(m_EntityNode);

	//if( P_VALID(m_pSG) )
	//	SAFE_DELETE(m_pSG); 

	m_pFrameMgr->UnRegisterEventHandler(_T("Finish_Close_Loading"),(FRAMEEVENTPROC)m_Trunk.sfp1(&LoginFrame::OnFinishCloseLoading));
	m_pCmdMgr->UnRegister("NS_JoinGame", (NETMSGPROC)m_Trunk.sfp2(&LoginFrame::NetRecvJoinGame));
	m_pFrameMgr->UnRegisterEventHandler(_T("tagExitLoadingFrame"), (FRAMEEVENTPROC)m_Trunk.sfp1(&LoginFrame::OnExitLoadingFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("Msgbox_KoreaLoginErr3Times"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&LoginFrame::OnMsgbox_KoreaLoginErr3Times));


	KillObj("login_ip_list");

	stringstream stream;
	stream << "XClient_Login" << this;

	KillObj(stream.str().c_str());

	if( P_VALID(m_hDll) )
	{
		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
	if( m_bLoginType )
		g_bCommandLine = true;

	if(P_VALID(m_pTex))
		IDraw2D::Inst()->ReleaseTexture(m_pTex);

	m_bShowID = false;

	g_bUnrealInit = FALSE;
	g_bUnrealSend = FALSE;
	m_bOpenPassport = TRUE;
	g_bSendDoneMsg = FALSE;
	m_bSendUrlLoginMsg = FALSE;
	m_bOpenWaitFlag = FALSE;
	for ( int i =0; i< int(m_vecHttpPic.size()); ++i )
	{	
		HttpPicEx* pHttpPic = m_vecHttpPic[i];
		if (P_VALID(pHttpPic))
		{
			m_pGUI->AddToDestroyList(pHttpPic);
			pHttpPic = NULL;
		}
	}
	m_vecHttpPic.clear();

	for ( int j =0; j< int(m_OpenPassportInfo.size()); ++j )
	{	
		tagURLInfo  info = m_OpenPassportInfo[j];
		if ( info.dwID != GT_INVALID || info.pHttp != NULL )
		{
			(m_OpenPassportInfo[j].pHttp)->Terminate();
			SAFE_DEL(m_OpenPassportInfo[j].pHttp);
		}
	}
	m_OpenPassportInfo.clear();

	if (1 == SaveAccount::Inst()->GetLoginMode()&& g_yyDemo.HaveInited())
	{
		if (ECT_YYYuYinPingTai ==SaveAccount::Inst()->GetYYCropID() )	
		{
			g_yyDemo.m_pfnShowOpid(0,1000,10);
		}
	}

	if(P_VALID(m_sendSave))
		SAFE_DELETE(m_sendSave);

	return GameFrame::Destroy();
}



//-----------------------------------------------------------------------------
//! 事件处理
//! 
//-----------------------------------------------------------------------------
DWORD LoginFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Drag:	//移动
		{
			m_pWndKey->Align();
			m_pWndKey->SetRefresh();
		}
	case EGUIE_EditBoxEnter:
		if( pWnd == m_pEditAccount )
		{
			tstring strAccount = m_pEditAccount->GetText();
			if( !strAccount.empty() )
				m_pGUI->SetActive(m_pEditPassword);
		}

		if( pWnd == m_pEditPassword 
			&& m_pButtonLogin->IsEnable() )
		{
			m_bOpenPassport = FALSE;
			m_bSendUrlLoginMsg = FALSE;
			m_bOpenWaitFlag = FALSE;
			SaveAccount::Inst()->SetYYLoginMode(false);
			tstring strPassword  = m_pEditPassword->GetText();
			switch( m_nPolicy )
			{
			case 0:
			case 8:
				{
					//m_pClient->WaitThreadLoad();
					Connect();
					return TRUE;
				}
				break;
			case 1:
				{
					if( !strPassword.empty() )
					{
						//m_pClient->WaitThreadLoad();
						Connect();
						return TRUE;
					}
				}
				break;
			case 6:
				{
					if( !strPassword.empty() )
					{
						m_pClient->WaitThreadLoad();
						Connect();
						return TRUE;
					}
				}
				break;
			}
			
		}
		break;

	case EGUIE_KeyDown:
		if( pEvent->msg.dwParam1 == VK_TAB )
		{
			if( m_pGUI->GetActive() == m_pEditAccount )
			{
				m_pGUI->SetActive(m_pEditPassword);
				m_pPBtn->SetState(EGUIBS_PopUp, FALSE);
				m_pWndKey->SetInvisible(m_pPBtn->GetState()!=EGUIBS_PushDown);
			}
			else if(m_pGUI->GetActive() == m_pEditPassword)
			{
				m_pGUI->SetActive(m_pEditAccount);
				m_pPBtn->SetState(EGUIBS_PopUp, FALSE);
				m_pWndKey->SetInvisible(m_pPBtn->GetState()!=EGUIBS_PushDown);
			}
		}
		break;

	case EGUIE_Click:
		if( pWnd == m_pButtonQuit )
		{
			m_pClient->ExitThreadLoad();
			PostQuitMessage(0);
		}
		else if( pWnd == m_pBtnReturn )
		{
			if ( m_nPolicy !=2 )		 
				SeleRoleMgr::Inst()->CloseMap();
			else if ( !m_bCloseMapOnce ) // m_nPolicy ==2
				SeleRoleMgr::Inst()->CloseMap();
			SeleRoleMgr::Inst()->Destroy();
			GameFrame* pFrame = m_pFrameMgr->CreateFrame(_T("Root"), _T("ServerSelect"), _T("ServerSelectFrame"), 0);
			m_pFrameMgr->AddToDestroyList(this);
		}
		else if( pWnd == m_pButtonLogin )//登录
		{
			if(m_dwCropID != 0)	//新三方登陆
			{
				int nCurChoose = m_byOpenPassPotIndex;
				m_bSendUrlLoginMsg = TRUE;
				m_nCurChoose = nCurChoose;
				//hide ui
				UrlModeHideWedget(TRUE);
				m_bOpenPassport = TRUE;
				m_bOpenWaitFlag = TRUE;
				CutOffLogin();
			}
			else	//正常模式
			{
				m_bOpenPassport = FALSE;
				m_bSendUrlLoginMsg = FALSE;
				m_bOpenWaitFlag = FALSE;
				SaveAccount::Inst()->SetYYLoginMode(false);
				tstring strPassword  = m_pEditPassword->GetText();
				switch( m_nPolicy )
				{
				case 0:
				case 8:
					{
						//m_pClient->WaitThreadLoad();
						Connect();
						return TRUE;
					}
					break;
				case 1:
					{
						if (SaveAccount::Inst()->GetCropID())
						{
							bSendOnceFlag = FALSE;
							return TRUE;
						}
						else if( !strPassword.empty() )
						{
							//m_pClient->WaitThreadLoad();
							Connect();
							return TRUE;
						}
					}
					break;
				case 3:
				case 6:
					{
						if( !strPassword.empty() )
						{
							//m_pClient->WaitThreadLoad();
							Connect();
							return TRUE;
						}
					}
					break;
				case 5:
					{	 
						bSendOnceFlag = FALSE;
						//Connect();
						return TRUE;
					}
					break;
				case 7:
				case 9:
				case 10: // 俄罗斯登陆模式
					{	 
						bSendOnceFlag = FALSE;
						return TRUE;
					}
					break;

				}
			}
		
		}
		else if( pWnd == m_pBtnCancelQueue )		//取消排队不用向服务器发消息
		{
			m_pWndQueue->SetInvisible(TRUE);
			m_pWndCancelQueue->SetInvisible(TRUE);
			m_pStcPosition->SetText(_T(""));
			m_pStcTime->SetText(_T(""));
			if( m_nPolicy == 4 )
				SDOAIgw::Inst()->SetShowLoginFlag(TRUE);
			else if( !m_bLoginType )
			{
				if ( !m_pWndCancelQueue->IsInvisible() 
					&& !m_pWndQueue->IsInvisible() )
				{
					m_pWnd->SetInvisible(TRUE);
				}
				else if(m_nPolicy != 11)
				{
					m_pWnd->SetInvisible(FALSE);
				}
			}
			m_pWndKey->SetInvisible(m_pWndKey->IsInvisible());
			if ( m_nPolicy != 7 && m_nPolicy != 9 && m_nPolicy != 10)
			{
				m_pStcHint->SetText(g_StrTable[_T("JoinGame_InputNameOrSN")]);
			}
			else if(m_nPolicy != 11)
			{
				m_pWnd->SetInvisible(FALSE);
				HideLoginFrame(TRUE);
				m_pButtonQuit->SetInvisible(FALSE);
				m_pButtonLogin->SetInvisible(FALSE); 
			}
			CutOffLogin();
			if( !m_bLoginType )
				m_pButtonLogin->SetEnable(true);
			switch( m_nPolicy )
			{
			case 0:
			case 1:
			case 3:
			case 5:
			case 6:
				break;
			case 2:				//迅雷
				{
					JudegPolicy();
				}
				break;
			}

			if ( m_nPolicy !=2 )		 
				SeleRoleMgr::Inst()->CloseMap();
			else if ( !m_bCloseMapOnce ) // m_nPolicy ==2
				SeleRoleMgr::Inst()->CloseMap();
			SeleRoleMgr::Inst()->Destroy();
			GameFrame* pFrame = m_pFrameMgr->CreateFrame(_T("Root"), _T("ServerSelect"), _T("ServerSelectFrame"), 0);
			m_pFrameMgr->AddToDestroyList(this);
		}
		else if ( pWnd == m_pBtnPrev  && OPEN_PASSPORT  )
		{
			if (m_nCurUrlPage>0)
			{
				--m_nCurUrlPage;
				for ( int k=0; k<g_nUrlNum; ++k )
					m_pPBUrl[k]->SetState( EGUIBS_PopUp, FALSE, FALSE );
			}
			RefreshUrlPic();
		}
		else if ( pWnd == m_pBtnNext  && OPEN_PASSPORT  )
		{
			if ( (m_nCurUrlPage+1)*g_nUrlNum< int(m_OpenPassportInfo.size()) )
			{
				++m_nCurUrlPage;
				for ( int k=0; k<g_nUrlNum; ++k )
					m_pPBUrl[k]->SetState( EGUIBS_PopUp, FALSE, FALSE );
			}
			RefreshUrlPic();
		}
		else if ( pWnd == m_pBtnNormal  && OPEN_PASSPORT  )
		{
			//show ui
 			this->SetUIEnable(TRUE);
 			UrlModeHideWedget(FALSE);
 			m_bOpenPassport = FALSE;
 			m_bOpenWaitFlag = FALSE;
			m_pStcWebLoginTips->SetInvisible(TRUE);
			m_pStcSavePass->SetInvisible(FALSE);
 			m_nCurChoose = GT_INVALID;
 			for ( int k=0; k<g_nUrlNum; ++k )
				m_pPBUrl[k]->SetState( EGUIBS_PopUp, FALSE, FALSE );
			m_dwCropID = 0;
 			RefreshUrlPic();
			CutOffLogin();
		}
		else
		{
			if( tstring::npos != pWnd->GetName().find(_T("butt")) )
			{
				tstring str = m_pLastActive->GetText();
				str += ((GUIButton*)pWnd)->GetText();
				m_pLastActive->SetText(str.c_str());
				m_pGUI->SetActive(m_pLastActive);
				m_pLastActive->Deselect();
			}
		}
		break;

	case EGUIE_Active:
		if( pWnd == m_pEditAccount )
			m_pLastActive = m_pEditAccount;
		if( pWnd == m_pEditPassword )
			m_pLastActive = m_pEditPassword;
		break;

	case EGUIE_PushButtonDown:
		{
			if( pEvent->dwParam1==1 || pEvent->dwParam1==2 )
			{
				m_pWndKey->Align();
				m_pWndKey->SetInvisible(m_pPBtn->GetState()!=EGUIBS_PushDown);
				if( pEvent->dwParam1 == 1 )
					m_pWndKey->FlipToTop();
			}
			if( pWnd == m_pPBtnSaveAccount )
			{
				if( pEvent->dwParam1 == 1 )
					m_bSave = true;
				else if( pEvent->dwParam1 == 2 )
					m_bSave = false;
			}
			if( pWnd == m_pPBtnSavePassword )
			{
				if( pEvent->dwParam1 == 1 )
					m_bSavePassword = true;
				else if( pEvent->dwParam1 == 2 )
					m_bSavePassword = false;
			}
			if( pWnd == m_pPBtnCamera )
			{
				if( pEvent->dwParam1 == 1 )
				{
					GameSet::Inst()->Set( EGS_2DCamera, true );
					GameSet::Inst()->SaveCameraSet();
				}
				else if( pEvent->dwParam1 == 2 )
				{
					GameSet::Inst()->Set( EGS_2DCamera, false );
					GameSet::Inst()->SaveCameraSet();
				}
			}

			if( pWnd == m_pPBtn_Tab )
			{
				LetterTab();				
			}
			//按下才处理 
			if ( (_tcsnccmp(pWnd->GetName().c_str(), _T("butt"), 4) == 0) && OPEN_PASSPORT )
			{
				for ( int j=0; j<g_nUrlNum; ++j )
				{
					if ( pWnd ==(GUIWnd*)m_pPBUrl[j] )
					{
						if( pEvent->dwParam1 == 1 )
						{
							for ( int k=0; k<g_nUrlNum; ++k )
								if ( j != k )
									m_pPBUrl[k]->SetState( EGUIBS_PopUp, FALSE, FALSE );

							int nCurChoose = m_nCurUrlPage*g_nUrlNum + j;
							//if ( nCurChoose != m_nCurChoose )
							//{
							m_bSendUrlLoginMsg = TRUE;
							m_nCurChoose = nCurChoose;
							//hide ui
							UrlModeHideWedget(TRUE);
							m_bOpenPassport = TRUE;
							m_bOpenWaitFlag = TRUE;
							CutOffLogin();
							m_pBtnNormal->SetInvisible(FALSE);
							m_pPBtn->SetInvisible(TRUE);
							//}					
							break;
						}
					}
				}
			}

		}
		break;
	case EGUIE_ItemClick:
		{
			ASSERT(m_pWorldList);
			ASSERT(m_pBtnOKWorld);
			int curSel = m_pWorldList->GetCurSelectedRow();
			if( GT_INVALID!=(int)m_pWorldList->GetText(curSel,0) ) //选择项是否为空
				m_pBtnOKWorld->SetEnable(true);
			else
				m_pBtnOKWorld->SetEnable(false);
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if ( (_tcsnccmp(pWnd->GetName().c_str(), _T("butt"), 4) == 0) && OPEN_PASSPORT )
			{
				for ( int j=0; j<g_nUrlNum; ++j )
				{
					if ( pWnd ==(GUIWnd*)m_pPBUrl[j] )
					{
						int nCurChoose = m_nCurUrlPage*g_nUrlNum + j;
						if ( nCurChoose < m_OpenPassportInfo.size() )
						{
							tagURLInfo info = m_OpenPassportInfo[nCurChoose];
							ShowTip( pWnd, info.szDesc.c_str() );
						}
						break;
					}
				}
			}
			else if(pWnd == m_pButtonLogin)
			{
				if(m_dwCropID != 0 )//&& m_OpenPassportInfo.size() > m_byOpenPassPotIndex)
				{
					//tagURLInfo info = m_OpenPassportInfo[m_byOpenPassPotIndex];
					ShowTip(pWnd, g_StrTable[_T("OpenPassTips")]);
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	}

	return FALSE;
}

DWORD LoginFrame::EventHandlerForSecurity(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnSecuOK )
			{
				m_strInputMiBao += m_pStcInputHint->GetText();
				MBPhaseHint();
			}
			else if( pWnd == m_pBtnSecuCancel )
			{
				RandSetKeyMB();
				m_pStcInputHint->SetText(_T(""));
			}
			else
			{
				if( _tcsncmp(pWnd->GetName().c_str(), _T("butt"), 4) == 0 ) 
				{
					for( int i=0; i<10; i++ )
					{
						if( pWnd == m_pBtnSecuInput[i] )
						{
							tstring strTemp = m_pStcInputHint->GetText();
							if( _tcslen(strTemp.c_str())<2 ) 
							{
								strTemp += m_pBtnSecuInput[i]->GetText();
								m_pStcInputHint->SetText(strTemp.c_str());
								break;
							}
						}
					}
				}
			}
		}
		break;
	}
	return FALSE;
}

//韩国Login界面事件
DWORD LoginFrame::EventKoreaLoginEvent(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return 0;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pBtnKoreaLoginOK)	//点击确定按钮
			{
				if(!m_pWndFirstSetPassWord->IsInvisible())	//第一次进入游戏设置密码
				{
					BOOL bRet = TRUE;
					if(m_strKoreaSetPassWordOne.empty() || m_strKoreaSetPassWordTwo.empty() )
					{
						bRet = FALSE;
						m_pStcKoreaFTip->SetText(g_StrTable[_T("ItemMsgError_PsdNull")]);
					}
					else if(strcmp(m_strKoreaSetPassWordOne.c_str(), m_strKoreaSetPassWordTwo.c_str()) != 0)
					{
						bRet = FALSE;
						m_pStcKoreaFTip->SetText(g_StrTable[_T("ItemMsgError_DoublePsdDiff")]);
					}
					else if(m_strKoreaSetPassWordOne.length() < 4 || m_strKoreaSetPassWordOne.length() > 8 )
					{
						bRet = FALSE;
						m_pStcKoreaFTip->SetText(g_StrTable[_T("ItemMsgError_sizeError")]);
					}
					m_pStcKoreaFTip->SetRefresh(TRUE);

					if(bRet)
					{
						tagNLC_SetSecondPsd cmd;
						cmd.bIsChangeTwPsd = FALSE;	//标识不是更改密码，是第一次设置密码
						strcpy(cmd.czNewSecondPsd, m_strKoreaSetPassWordOne.c_str());
						m_pStream->Send(&cmd, cmd.dwSize);
					}
					else	//输入有误
					{
						m_pStcFirstPassWord->SetText(_T(""));
						m_pStcSecondPassWord->SetText(_T(""));
					}
				}
				else if(!m_pWndKoreaLogin->IsInvisible())	//进入游戏
				{
					tagNLC_InputSecondPsd cmd;
					cmd.dwTwicePsdCrc = m_pUtil->Crc32(m_strKoreaLoginNumber.c_str());
					m_pStream->Send(&cmd, cmd.dwSize);
				}
				else if(!m_pWndKoreaChangePassWord->IsInvisible())	//修改密码
				{
					BOOL bRet = TRUE;
					if(m_strKoreaChangePassWordOne.empty() || m_strKoreaChangePassWordTwo.empty() )
					{
						bRet = FALSE;
						m_pStcChangeWordTip->SetText(g_StrTable[_T("ItemMsgError_PsdNull")]);
					}
					else if(strcmp(m_strKoreaChangePassWordOne.c_str(), m_strKoreaChangePassWordTwo.c_str()) != 0)
					{
						bRet = FALSE;
						m_pStcChangeWordTip->SetText(g_StrTable[_T("ItemMsgError_DoublePsdDiff")]);
					}
					else if(m_strKoreaChangePassWordOne.length() < 4|| m_strKoreaChangePassWordOne.length() > 8 )
					{
						bRet = FALSE;
						m_pStcChangeWordTip->SetText(g_StrTable[_T("ItemMsgError_sizeError")]);
					}
					else if(strcmp(m_strKoreaChangePassWordOne.c_str(), m_strKoreaChangePassWordOld.c_str()) == 0 )
					{
						bRet = FALSE;
						m_pStcChangeWordTip->SetText(g_StrTable[_T("ItemMsgError_SameToOld")]);
					}
					m_pStcChangeWordTip->SetRefresh(TRUE);
					
					if(bRet)
					{
						tagNLC_SetSecondPsd cmd;
						cmd.bIsChangeTwPsd = TRUE;	//标识修改密码
						cmd.dwOldSecondPsdCrc = m_pUtil->Crc32(m_strKoreaChangePassWordOld.c_str());
						strcpy(cmd.czNewSecondPsd, m_strKoreaChangePassWordOne.c_str());
						m_pStream->Send(&cmd, cmd.dwSize);
					}
					else
					{
						m_pStcNewPassWord->SetText(_T(""));
						m_pStcNewPassWordConfirm->SetText(_T(""));
						m_pStcOldPassWord->SetText(_T(""));
					}		
				}
				KoreaClearPassWordInfo();
			}
			else if(pWnd == m_pBtnKoreaChangeWorld)
			{
				KoreaChangeUI(EKCU_ChangePassWord);
			}
			else if(pWnd == m_pBtnKoreaLoginCancel)
			{
				if(!m_pWndKoreaLogin->IsInvisible())	//登陆返回，退回到选择服务器
				{
					SeleRoleMgr::Inst()->CloseMap();
					SeleRoleMgr::Inst()->Destroy();
					GameFrame* pFrame = m_pFrameMgr->CreateFrame(_T("Root"), _T("ServerSelect"), _T("ServerSelectFrame"), 0);
					m_pFrameMgr->AddToDestroyList(this);
				}
				else if(!m_pWndKoreaChangePassWord->IsInvisible())	//修改密码返回，退回至登陆
				{
					KoreaChangeUI(EKCU_Login);
				}
				else if(!m_pWndFirstSetPassWord->IsInvisible())	//设置密码返回，退回至选择服务器
				{
					SeleRoleMgr::Inst()->CloseMap();
					SeleRoleMgr::Inst()->Destroy();
					GameFrame* pFrame = m_pFrameMgr->CreateFrame(_T("Root"), _T("ServerSelect"), _T("ServerSelectFrame"), 0);
					m_pFrameMgr->AddToDestroyList(this);
				}
			}
			else if(pWnd == m_pBtnKoreaBack)
			{
				if(!m_pWndKoreaLogin->IsInvisible())	//登陆
				{
					if(m_strKoreaLoginNumber.begin() != m_strKoreaLoginNumber.end())
					{
						tstring strTemp = m_pStcLoginPassWord->GetText();
						strTemp.erase(strTemp.begin());	
						m_pStcLoginPassWord->SetText(strTemp.c_str());
						m_strKoreaLoginNumber.erase(m_strKoreaLoginNumber.end()-1);
						m_pWndKoreaPassWord->SetRefresh(TRUE);
					}
	
				}
				else if(!m_pWndKoreaChangePassWord->IsInvisible())			//修改
				{
					if(m_eKoreaInputType == EKIT_ChangePassWordOld)			//当前选中旧密码
					{
						if(m_strKoreaChangePassWordOld.begin() != m_strKoreaChangePassWordOld.end())
						{
							tstring strTemp = m_pStcOldPassWord->GetText();
							strTemp.erase(strTemp.begin());	
							m_pStcOldPassWord->SetText(strTemp.c_str());
							m_strKoreaChangePassWordOld.erase(m_strKoreaChangePassWordOld.end()-1);
							m_pWndKoreaPassWord->SetRefresh(TRUE);
						}
					}
					else if(m_eKoreaInputType == EKIT_ChangePassWordOne)	//当前选中新密码
					{
						if(m_strKoreaChangePassWordOne.begin() != m_strKoreaChangePassWordOne.end())
						{
							tstring strTemp = m_pStcNewPassWord->GetText();
							strTemp.erase(strTemp.begin());	
							m_pStcNewPassWord->SetText(strTemp.c_str());
							m_strKoreaChangePassWordOne.erase(m_strKoreaChangePassWordOne.end()-1);
							m_pWndKoreaPassWord->SetRefresh(TRUE);
						}
					}
					else if(m_eKoreaInputType == EKIT_ChangePassWordTwo)	//当前选中新密码第二次输入
					{
						if(m_strKoreaChangePassWordTwo.begin() != m_strKoreaChangePassWordTwo.end())
						{
							tstring strTemp = m_pStcNewPassWordConfirm->GetText();
							strTemp.erase(strTemp.begin());	
							m_pStcNewPassWordConfirm->SetText(strTemp.c_str());
							m_strKoreaChangePassWordTwo.erase(m_strKoreaChangePassWordTwo.end()-1);
							m_pWndKoreaPassWord->SetRefresh(TRUE);
						}
					}
				}
				else if(!m_pWndFirstSetPassWord->IsInvisible())				//第一次登陆
				{
					if(m_eKoreaInputType == EKIT_SetPassWordOne)			//修改密码
					{
						if(m_strKoreaSetPassWordOne.begin() != m_strKoreaSetPassWordOne.end())
						{
							tstring strTemp = m_pStcFirstPassWord->GetText();
							strTemp.erase(strTemp.begin());	
							m_pStcFirstPassWord->SetText(strTemp.c_str());
							m_strKoreaSetPassWordOne.erase(m_strKoreaSetPassWordOne.end()-1);
							m_pWndKoreaPassWord->SetRefresh(TRUE);
						}
					}
					else if(m_eKoreaInputType == EKIT_SetPassWordTwo)		//修改密码确认
					{
						if(m_strKoreaSetPassWordTwo.begin() != m_strKoreaSetPassWordTwo.end())
						{
							tstring strTemp = m_pStcSecondPassWord->GetText();
							strTemp.erase(strTemp.begin());	
							m_pStcSecondPassWord->SetText(strTemp.c_str());
							m_strKoreaSetPassWordTwo.erase(m_strKoreaSetPassWordTwo.end()-1);
							m_pWndKoreaPassWord->SetRefresh(TRUE);
						}
					}
				}
			}
			else if(pWnd == m_pBtnKoreaClear)
			{
				if(!m_pWndKoreaLogin->IsInvisible())	//登陆
				{	
					m_pStcLoginPassWord->SetText(_T(""));
					m_pWndKoreaPassWord->SetRefresh(TRUE);
					m_strKoreaLoginNumber = "";
				}
				else if(!m_pWndKoreaChangePassWord->IsInvisible())			//修改
				{
					if(m_eKoreaInputType == EKIT_ChangePassWordOld)			//当前选中旧密码
					{
						m_pStcOldPassWord->SetText(_T(""));
						m_pWndKoreaPassWord->SetRefresh(TRUE);
						m_strKoreaChangePassWordOne = "";
					}
					else if(m_eKoreaInputType == EKIT_ChangePassWordOne)	//当前选中新密码
					{
						m_pStcNewPassWord->SetText(_T(""));
						m_pWndKoreaPassWord->SetRefresh(TRUE);
						m_strKoreaChangePassWordTwo = "";
					}
					else if(m_eKoreaInputType == EKIT_ChangePassWordTwo)	//当前选中新密码第二次输入
					{
						m_pStcNewPassWordConfirm->SetText(_T(""));
						m_pWndKoreaPassWord->SetRefresh(TRUE);
						m_strKoreaChangePassWordOld = "";
					}
				}
				else if(!m_pWndFirstSetPassWord->IsInvisible())				//第一次登陆
				{
					if(m_eKoreaInputType == EKIT_SetPassWordOne)			//修改密码
					{
						m_pStcFirstPassWord->SetText(_T(""));
						m_pWndKoreaPassWord->SetRefresh(TRUE);
						m_strKoreaSetPassWordOne = "";
					}
					else if(m_eKoreaInputType == EKIT_SetPassWordTwo)		//修改密码确认
					{
						m_pStcSecondPassWord->SetText(_T(""));
						m_pWndKoreaPassWord->SetRefresh(TRUE);
						m_strKoreaSetPassWordTwo = "";
					}
				}
			}
			else	//判断是否点击数字键
			{
				for(int i = 0; i < 10; i++)
				{
					if(pWnd == m_pBtnKoreaWordKey[i])
					{
						if(!m_pWndKoreaLogin->IsInvisible())	//登陆
						{
							tstring strText;
							if(m_strKoreaLoginNumber.length() >= 8)
								return 0;
							m_strKoreaLoginNumber += m_pUtil->UnicodeToUnicode8(((GUIButton*)pWnd)->GetText().c_str());
							strText = m_pStcLoginPassWord->GetText();
							strText.append(_T("*"));
							m_pStcLoginPassWord->SetText(strText.c_str());
						}
						else if(!m_pWndKoreaChangePassWord->IsInvisible())			//修改
						{
							DWORD dwNum = 0;
							tstring strText;
							dwNum = _ttoi(((GUIButton*)pWnd)->GetText().c_str());
							if(m_eKoreaInputType == EKIT_ChangePassWordOld)			//当前选中旧密码
							{
								m_strKoreaChangePassWordOld += m_pUtil->UnicodeToUnicode8(((GUIButton*)pWnd)->GetText().c_str());
								strText = m_pStcOldPassWord->GetText();
								strText.append(_T("*"));
								m_pStcOldPassWord->SetText(strText.c_str());
							}
							else if(m_eKoreaInputType == EKIT_ChangePassWordOne)	//当前选中新密码
							{
								m_strKoreaChangePassWordOne += m_pUtil->UnicodeToUnicode8(((GUIButton*)pWnd)->GetText().c_str());
								strText = m_pStcNewPassWord->GetText();
								strText.append(_T("*"));
								m_pStcNewPassWord->SetText(strText.c_str());
							}
							else if(m_eKoreaInputType == EKIT_ChangePassWordTwo)	//当前选中新密码第二次输入
							{
								m_strKoreaChangePassWordTwo += m_pUtil->UnicodeToUnicode8(((GUIButton*)pWnd)->GetText().c_str());
								strText = m_pStcNewPassWordConfirm->GetText();
								strText.append(_T("*"));
								m_pStcNewPassWordConfirm->SetText(strText.c_str());
							}
						}
						else if(!m_pWndFirstSetPassWord->IsInvisible())				//第一次登陆
						{
							tstring strText;
							if(m_eKoreaInputType == EKIT_SetPassWordOne)			//修改密码
							{
								m_strKoreaSetPassWordOne += m_pUtil->UnicodeToUnicode8(((GUIButton*)pWnd)->GetText().c_str());
								strText = m_pStcFirstPassWord->GetText();
								strText.append(_T("*"));
								m_pStcFirstPassWord->SetText(strText.c_str());
							}
							else if(m_eKoreaInputType == EKIT_SetPassWordTwo)		//修改密码确认
							{
								m_strKoreaSetPassWordTwo += m_pUtil->UnicodeToUnicode8(((GUIButton*)pWnd)->GetText().c_str());
								strText = m_pStcSecondPassWord->GetText();
								strText.append(_T("*"));
								m_pStcSecondPassWord->SetText(strText.c_str());
							}
						}
					}
				}
			}
		}
		break;
	case EGUIE_DragStart:
		{
			if(pWnd == m_pStcFirstPassWord)				//初始设置里的第一个密码
			{
				KoreaRefrashInput(EKIT_SetPassWordOne);
			}
			else if(pWnd == m_pStcSecondPassWord)		//初始设置里的第二个密码
			{
				KoreaRefrashInput(EKIT_SetPassWordTwo);
			}
			else if(pWnd == m_pStcOldPassWord)			//旧密码
			{
				KoreaRefrashInput(EKIT_ChangePassWordOld);
			}
			else if(pWnd == m_pStcNewPassWord)			//新密码
			{
				KoreaRefrashInput(EKIT_ChangePassWordOne);
			}
			else if(pWnd == m_pStcNewPassWordConfirm)	//新密码确认
			{
				KoreaRefrashInput(EKIT_ChangePassWordTwo);
			}
		}
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ReloadUI
//-----------------------------------------------------------------------------
BOOL LoginFrame::ReloadUI()
{
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
		return FALSE;
	}
	::GetPrivateProfileString(_T("Launcher"), _T("CropID"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
	tstring strCropID = szTemp;
	if(_tcscmp(strCropID.c_str(), _T("")) != 0)
		m_dwCropID = _ttoi(strCropID.c_str());
	else
		m_dwCropID = 0;

	INT nPolicy = _ttoi(policy.c_str());
	m_nPolicy = nPolicy;
	SaveAccount::Inst()->SetLoginMode(m_nPolicy);


	// 创建
	tstring strPath = g_strLocalPath + _T("\\ui\\login.xml");
	tstring strPath2 = g_strLocalPath + _T("\\ui\\queue.xml");
	tstring strPath3 = g_strLocalPath + _T("\\ui\\queuebutt.xml");
	tstring strPath4 = g_strLocalPath + _T("\\ui\\security.xml");
	tstring strPath5 = g_strLocalPath + _T("\\ui\\security_putin.xml");
	XmlElement element;
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());

	//调整位置
	GUIWnd* pDesktop = m_pGUI->GetDesktop();
	std::stringstream streamX, streamY;
	streamX << (pDesktop->GetSize().x/2-110);
	m_pGUI->ChangeXml(&element, "loginwin", "LogicPos_x", streamX.str().c_str());
	if( m_nPolicy == 4 )
	{
		m_pGUI->ChangeXml(&element, "loginwin\\loginpic", "PicColor", "0xFFFFFF");
		streamY << (pDesktop->GetSize().y/2+100);
	}
	else
	{
		streamY << (pDesktop->GetSize().y/2-50);		
	}
	m_pGUI->ChangeXml(&element, "loginwin", "LogicPos_y", streamY.str().c_str());
	switch( nPolicy )
	{
	case 2:			//迅雷特殊处理
		{
			m_pGUI->ChangeXml(&element, "loginwin\\loginpic\\name", "ReadOnly", "yes");
			m_pGUI->ChangeXml(&element, "loginwin\\loginpic\\name", "Password", "yes");
			m_pGUI->ChangeXml(&element, "loginwin\\loginpic\\code1", "ReadOnly", "yes");
			m_pGUI->ChangeXml(&element, "loginwin\\kbbutt", "Invisible", "yes");	
			m_pGUI->ChangeXml(&element, "loginwin\\kbbutt", "Invisible", "yes");	
		}
		break;
	}

	for (int i=0; i<5; ++i)
	{
		char cbuff[X_LONG_NAME];
		_snprintf(cbuff, sizeof(cbuff)/sizeof(char),"loginwin\\webarea\\icon%d",i );
		m_pGUI->ChangeXml(&element, cbuff, "ClassName", "HttpPicEx");
	}
	m_pGUI->ChangeXml(&element, "loginwin\\surebutt\\logo", "ClassName", "HttpPicEx");

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	
	tstring strPath1 = g_strLocalPath + _T("\\ui\\login_key.xml");
	m_pWndKey = m_pGUI->CreateFromFile("VFS_System", strPath1.c_str());
	m_pPBtn_Tab = (GUIPushButton*)m_pWndKey->GetChild(_T("kbpic\\shiftbutt"));

	// 排队
	m_pWndQueue = m_pGUI->CreateFromFile("VFS_System", strPath2.c_str());
	m_pWndQueue->SetInvisible(TRUE);
	m_pWndCancelQueue	= (GUIStatic*)m_pGUI->CreateFromFile("VFS_System", strPath3.c_str());
	m_pBtnCancelQueue	= (GUIButton*)m_pWndCancelQueue->GetChild(_T("backbutt"));
	m_pStcTips			= (GUIStatic*)m_pWndCancelQueue->GetChild(_T("content"));
	m_pStcTips->SetInvisible(TRUE);
	m_pWndCancelQueue->SetInvisible(TRUE);
	//密保
	std::stringstream streamX1, streamY1;
	streamX1 << (pDesktop->GetSize().x/2-130);
	streamY1 << (pDesktop->GetSize().y/2);
	m_pGUI->LoadXml(&element, "VFS_System", strPath4.c_str());
	m_pGUI->ChangeXml(&element, "security_showback", "LogicPos_x", streamX1.str().c_str());
	m_pGUI->ChangeXml(&element, "security_showback", "LogicPos_y", streamY1.str().c_str());
	m_pGUI->ChangeXml(&element, "security_showback\\num", "ClassName", "CGUIEditBox");
	m_pWndSecurity = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pWndSecurity->SetInvisible(TRUE);
	std::stringstream streamX2, streamY2;
	streamX2 << (pDesktop->GetSize().x/2+200);
	streamY2 << (pDesktop->GetSize().y/2);
	m_pGUI->LoadXml(&element, "VFS_System", strPath5.c_str());
	m_pGUI->ChangeXml(&element, "security_inputback", "LogicPos_x", streamX2.str().c_str());
	m_pGUI->ChangeXml(&element, "security_inputback", "LogicPos_y", streamY2.str().c_str());
	m_pWndSeInputBack = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pWndSeInputBack->SetInvisible(TRUE);

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&LoginFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndKey->GetFullName().c_str(), m_Trunk.sfp1(&LoginFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndCancelQueue->GetFullName().c_str(), m_Trunk.sfp1(&LoginFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndSecurity->GetFullName().c_str(), m_Trunk.sfp1(&LoginFrame::EventHandlerForSecurity));
	m_pGUI->RegisterEventHandler(m_pWndSeInputBack->GetFullName().c_str(), m_Trunk.sfp1(&LoginFrame::EventHandlerForSecurity));

	//m_pWndCaption	= (GUIStatic*)m_pWnd->GetChild(_T("loginpic"));
//	m_pWndPswHint	= (GUIStatic*)m_pWndCaption->GetChild(_T("word2"));
	m_pStcAccountBack = (GUIStatic*)m_pWnd->GetChild(_T("loginpic\\namepic"));
	m_pStcPsswordBack = (GUIStatic*)m_pWnd->GetChild(_T("loginpic\\codepic"));
	m_pEditAccount	= (GUIEditBox*)m_pWnd->GetChild(_T("loginpic\\namepic\\putin"));
	m_pEditPassword = (GUIEditBox*)m_pWnd->GetChild(_T("loginpic\\codepic\\putin"));
	m_pStcAccountText = (GUIStatic*)m_pWnd->GetChild(_T("loginpic\\name"));
	m_pStcPsswordText = (GUIStatic*)m_pWnd->GetChild(_T("loginpic\\code"));
	m_pButtonLogin	= (GUIButton*)m_pWnd->GetChild(_T("surebutt"));
	m_pButtonQuit	= (GUIButton*)m_pWnd->GetChild(_T("canclebutt"));
	m_pBtnReturn	= (GUIButton*)m_pWnd->GetChild(_T("backbutt"));
	m_pStcSavePass	= (GUIStatic*)m_pWnd->GetChild(_T("word3"));
	m_pStcWebLoginTips = (GUIStatic*)m_pWnd->GetChild(_T("word"));
	if(m_dwCropID != 0)
	{
		m_pStcWebLoginTips->SetInvisible(FALSE);
		m_pStcSavePass->SetInvisible(TRUE);
	}
	else
		m_pStcWebLoginTips->SetInvisible(TRUE);
	//m_pStcUser		= (GUIStatic*)m_pWndCaption->GetChild(_T("word1"));
	switch( nPolicy )
	{
	case 2:			//迅雷特殊处理
		m_pButtonQuit->SetEnable(false);
		break;
	}
	m_pStcHint		= (GUIStatic*)m_pWnd->GetChild(_T("new"));
	m_pPBtn			= (GUIPushButton*)m_pWnd->GetChild(_T("kbbutt"));
	m_pPBtnSaveAccount = (GUIPushButton*)m_pWnd->GetChild(_T("word3\\choosebutt"));
	m_pPBtnCamera	   = (GUIPushButton*)m_pWnd->GetChild(_T("word4\\choosebutt"));
	m_pPBtnSavePassword = (GUIPushButton*)m_pWnd->GetChild(_T("word2\\choosebutt"));
	m_pStcSavePasswordBack = (GUIStatic*)m_pWnd->GetChild(_T("word2"));

	m_pStcSavePasswordBack->SetInvisible(true);
	switch( nPolicy )
	{
	case 8:			//迅雷特殊处理
		m_pStcSavePasswordBack->SetInvisible(false);
		break;
	}

	for ( int k=0 ; k<g_nUrlNum; ++k )
	{
		TCHAR buffer1[X_LONG_NAME];
		_sntprintf(buffer1, sizeof(buffer1)/sizeof(TCHAR), _T("webarea\\icon%d"), k );
		m_pStcUrlPic[k]	= (HttpPicEx*)m_pWnd->GetChild( buffer1 );	
		m_pStcUrlPic[k]->SetInvisible(TRUE);
		m_pPBUrl[k]	= (GUIPushButton*)m_pStcUrlPic[k]->GetChild( _T("butt") );	
		m_pStcOpenPassName[k] = (GUIStatic*)m_pStcUrlPic[k]->GetChild( _T("name") );
	}
	m_pBtnPrev		= (GUIButton*)m_pWnd->GetChild( _T("webarea\\last") );	
	m_pBtnPrev->SetInvisible(TRUE);
	m_pBtnNext	= (GUIButton*)m_pWnd->GetChild( _T("webarea\\next") );	
	m_pBtnNext->SetInvisible(TRUE);
	m_pBtnNormal= (GUIButton*)m_pWnd->GetChild( _T("routinebutt") );	
	//m_pBtnNormal->SetInvisible(TRUE);
	m_pOpenPassportPath = (GUIPatch*)m_pWnd->GetChild(_T("webarea"));
	m_pStcLoginPic = (GUIStatic*)m_pWnd->GetChild(_T("loginpic"));
	if (OPEN_PASSPORT) //显示url图片 
	{
		for ( int i=0 ; i<g_nUrlNum; ++i )
			m_pStcUrlPic[i]->SetDefaultPic(_T("data\\ui\\Common\\l_icon.dds"));
		m_nCurUrlPage	= 0;
		m_nCurChoose	=	GT_INVALID;
		//m_pHttpPicCurr = (HttpPicEx*)m_pWnd->GetChild(_T("surebutt\\logo"));
	}
	else
	{
		for ( int i=0 ; i<g_nUrlNum; ++i )
			m_pStcUrlPic[i]->SetInvisible(TRUE);
	}

	if( GameSet::Inst()->Get( EGS_2DCamera ) )
		m_pPBtnCamera->SetState(EGUIBS_PushDown, false);
	else
		m_pPBtnCamera->SetState(EGUIBS_PopUp, false);

	m_pStcTime		= (GUIStatic*)m_pWndQueue->GetChild(_T("time"));
	m_pStcPosition	= (GUIStatic*)m_pWndQueue->GetChild(_T("queueing"));

	m_pStcSecuHint	= (GUIStatic*)m_pWndSecurity->GetChild(_T("word1"));
	m_pStcSecuNum	= (GUIEditBox*)m_pWndSecurity->GetChild(_T("num"));
	m_pStcInputHint = (GUIStatic*)m_pWndSeInputBack->GetChild(_T("input_show"));
	m_pBtnSecuOK	= (GUIButton*)m_pWndSeInputBack->GetChild(_T("surebutt"));
	m_pBtnSecuCancel= (GUIButton*)m_pWndSeInputBack->GetChild(_T("canclebutt"));
	TCHAR szBuff[X_LONG_NAME] = {0};
	for( int i=0; i<10; i++ )
	{
		_stprintf( szBuff, _T("butt%d"), i+1 );
		m_pBtnSecuInput[i] = (GUIButton*)m_pWndSeInputBack->GetChild(szBuff);
	}

	m_pWndKey->SetInvisible(TRUE);
	this->LoadLastLoginInfo();
	this->SetKeyButtonPos();
	if( nPolicy == 4 )
		HideLoginFrame(TRUE);

	m_pLastActive = m_pEditAccount;
	m_pGUI->SetActive(m_pEditAccount);

	tstring strFlag = _T("data\\ui\\login\\login_black.dds");
	if(P_VALID(m_pTex))
		IDraw2D::Inst()->ReleaseTexture(m_pTex);
	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	m_pTex = IDraw2D::Inst()->NewTexture(strFlag.c_str(), pIFS);

	if (!P_VALID(m_pTex))
		return TRUE;

	IDeviceTex *pRealTex = m_pTex->GetDeviceTex();
	if (!P_VALID(pRealTex))
		return TRUE;
	m_rectSrc.top = m_rectSrc.left = 0;
	m_rectSrc.bottom = m_rectSrc.top + pRealTex->Height();
	m_rectSrc.right = m_rectSrc.left + pRealTex->Width();
	m_rectDest.left = 0;
	m_rectDest.top = 0;
	m_rectDest.right = pDesktop->GetSize().x;
	m_rectDest.bottom = pDesktop->GetSize().y;

	if( m_nPolicy == 11 )	//如果是韩国登陆加载二次密码确认控件
	{	
		m_pWnd->SetInvisible(TRUE);	//隐藏国服登陆窗口
		TCHAR szText[128] = {0};
		tstring strPathKoreaPassWord = g_strLocalPath + _T("\\ui\\set.xml");	
		m_pGUI->LoadXml(&element, "VFS_System", strPathKoreaPassWord.c_str());
		//m_pGUI->ChangeXml(&element, "security_showback", "LogicPos_x", streamX1.str().c_str());
		m_pWndKoreaPassWord = m_pGUI->CreateWnd(_T("\\desktop"), &element);

		// 注册事件处理函数
		m_pGUI->RegisterEventHandler(m_pWndKoreaPassWord->GetFullName().c_str(), m_Trunk.sfp1(&LoginFrame::EventHandler));
		m_pBtnKoreaPassWordClose	= (GUIButton*)m_pWndKoreaPassWord->GetChild(_T("set\\closebutton"));					//关闭
		m_pBtnKoreaChangeWorld		= (GUIButton*)m_pWndKoreaPassWord->GetChild(_T("set\\button2"));						//韩国修改二次密码
		m_pBtnKoreaLoginOK			= (GUIButton*)m_pWndKoreaPassWord->GetChild(_T("set\\button3"));						//韩国二次密码点击确定按钮，可能是登陆、确认等
		m_pBtnKoreaLoginCancel		= (GUIButton*)m_pWndKoreaPassWord->GetChild(_T("set\\button1"));						//韩国二次密码取消按钮
		for(int i = 0; i < 10; i++)
		{
			_stprintf(szText, _T("set\\back1\\back\\num\\num%d"), i+1);
			m_pBtnKoreaWordKey[i] = (GUIButton*)m_pWndKoreaPassWord->GetChild(szText);			//数字0～9
		}
		m_pBtnKoreaClear			= (GUIButton*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\back\\num\\num11"));		//清零按钮
		m_pBtnKoreaBack				= (GUIButton*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\back\\num\\num12"));		//回退按钮
		//设置密码面板相关控件
		m_pWndFirstSetPassWord		= (GUIWnd*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k1"));							//第一次设置密码面板
		m_pStcKoreaFTip				= (GUIStatic*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k1\\word2"));				//那个提示信息框
		m_pStcFirstPassWord			= (GUIStatic*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k1\\pic1\\word"));			//第一次输入密码
		m_pStcSecondPassWord		= (GUIStatic*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k1\\pic2\\word"));			//第二次密码确认
		m_pStcFirstPassWordPic		= (GUIStatic*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k1\\pic1\\pic"));			//选择第一次后显示的那个框
		m_pStcSecondPassWordPic		= (GUIStatic*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k1\\pic2\\pic"));			//选择第二次后显示的那个框
		m_pStcFirstPassWordPic->SetInvisible(TRUE);
		m_pStcSecondPassWordPic->SetInvisible(TRUE);
		//登陆面板
		m_pWndKoreaLogin			= (GUIWnd*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k2"));							//韩国二次密码登陆面板
		m_pStcKoreaLoginTip			= (GUIStatic*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k2\\word2"));				//登陆提示
		m_pStcLoginPassWord			= (GUIStatic*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k2\\pic2\\word"));			//登陆密码
		//修改密码面板
		m_pWndKoreaChangePassWord	= (GUIWnd*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k3"));							//韩国修改二次密码
		m_pStcChangeWordTip			= (GUIStatic*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k3\\word2"));				//修改密码提示
		m_pStcOldPassWord			= (GUIEditBox*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k3\\pic1\\word"));			//旧密码
		m_pStcNewPassWord			= (GUIEditBox*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k3\\pic2\\word"));			//新密码
		m_pStcNewPassWordConfirm	= (GUIEditBox*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k3\\pic3\\word"));			//新密码确认
		m_pStcOldPassWordPic		= (GUIStatic*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k3\\pic1\\pic"));			//旧密码底图
		m_pStcNewPassWordPic		= (GUIStatic*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k3\\pic2\\pic"));			//新密码底图
		m_pStcNewPassWordConfirmPic = (GUIStatic*)m_pWndKoreaPassWord->GetChild(_T("set\\back1\\k3\\pic3\\pic"));			//新密码确认底图
		m_pStcOldPassWordPic->SetInvisible(TRUE);		
		m_pStcNewPassWordPic->SetInvisible(TRUE);		
		m_pStcNewPassWordConfirmPic->SetInvisible(TRUE);

		m_pWndKoreaPassWord->SetInvisible(TRUE);
		m_pWndKoreaPassWord->FlipToTop();

		m_pGUI->RegisterEventHandler(m_pWndKoreaPassWord->GetFullName().c_str(), m_Trunk.sfp1(&LoginFrame::EventKoreaLoginEvent));

		bSendOnceFlag = FALSE;
	}

	if(SaveAccount::Inst()->GetYunLianOn())
	{
		CloseOtherLogin();
	}

	return TRUE;
}



//-----------------------------------------------------------------------------
// 随机交换键盘按钮位置
//-----------------------------------------------------------------------------
VOID LoginFrame::SetKeyButtonPos()
{
	tstring strName;
	GUIWnd* pPanel = m_pWndKey->GetChild(_T("kbpic"));
    if (!P_VALID(pPanel))
        return;

	for(INT n=0, m=0; n<36; n++)
	{
		m = (m_pUtil->Rand() % 36);
		while( m == n ) 
			m = (m_pUtil->Rand() % 36);

		tstringstream stream1, stream2;
		stream1 << _T("butt") << n;
		stream2 << _T("butt") << m;
		GUIButton* pButton1 = (GUIButton*)pPanel->GetChild(stream1.str().c_str());
		GUIButton* pButton2 = (GUIButton*)pPanel->GetChild(stream2.str().c_str());

        if (P_VALID(pButton1) && P_VALID(pButton2))
        {
            tstring strTemp = pButton1->GetText();
            pButton1->SetText(pButton2->GetText().c_str());
            pButton2->SetText(strTemp.c_str());
        }
	}
}

void LoginFrame::RandSetKeyMB()
{
	tstring strName;
	for(INT n=0, m=0; n<10; n++)
	{
		m = (m_pUtil->Rand() % 10);
		while( m == n ) 
			m = (m_pUtil->Rand() % 10);

		tstringstream stream1, stream2;
		stream1 << _T("butt") << n+1;
		stream2 << _T("butt") << m+1;
		GUIButton* pButton1 = (GUIButton*)m_pWndSeInputBack->GetChild(stream1.str().c_str());
		GUIButton* pButton2 = (GUIButton*)m_pWndSeInputBack->GetChild(stream2.str().c_str());

		tstring strTemp = pButton1->GetText();
		pButton1->SetText(pButton2->GetText().c_str());
		pButton2->SetText(strTemp.c_str());
	}
}

void LoginFrame::SendLoginMsg()
{
	//发送nlc_proof
	TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
	TCHAR szTemp[X_LONG_NAME*2];

	_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
	tstring strAppPath = szTemp;

	tstring strIniFileFullName = strAppPath + szIniFileName;
	::GetPrivateProfileString(_T("Launcher"), _T("LocalGame"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
	tstring version = szTemp;


	tstring strAccount = m_pEditAccount->GetText();
	tstring strPassword = m_pEditPassword->GetText();
	this->FilterString(strAccount);
	this->FilterString(strPassword);
	string strAct = m_pUtil->UnicodeToAnsi(strAccount.c_str());

	tagNLC_SafeProof send;

	if(m_nPolicy == 8||m_nPolicy == 9)
		send.dwType = 0;	
	else
		send.dwType = m_nPolicy;	

	tstring szWorldName = m_pVarContainer->GetString(_T("gamename"), _T("last_login_server"));
	send.dwWorldNameCrc = m_pUtil->Crc32(szWorldName.c_str());
	send.dwCurVersionID = m_pUtil->Crc32(version.c_str());
	send.dwLoginMode = ELM_NULL;
	send.bIsGetURL = false;

	switch(m_nPolicy)
	{
	case 0:					//内部使用
	case 8:
		{
			string strMAC;
			GetMAC(strMAC);
			strncpy( send.szMac, strMAC.c_str(), MAX_MAC_LENGTH );
			strncpy( send.szUserName, strAct.c_str(), X_SHORT_NAME );
			if( !strPassword.empty() )
			{
				string pass = m_pUtil->UnicodeToAnsi(strPassword.c_str());
				//密钥
				pass += "xvDvgqZq";
				BYTE digest[16];
				m_pUtil->MD5ForString( (char*)pass.c_str(), digest );
				string strPswMD5;
				m_pUtil->MD5ToString( strPswMD5, digest );
				char szBuff[X_LONG_NAME] = {0};
				strncpy( szBuff, strPswMD5.c_str(), MAX_MD5_ARRAY );
				strupr( szBuff );
				strncpy( send.szPsd, szBuff, MAX_MD5_ARRAY );
			}
		}
		break;
	case 1:					//中心激活
		{
			strncpy( send.szUserName, strAct.c_str(), X_SHORT_NAME );
			string pass = m_pUtil->UnicodeToAnsi(strPassword.c_str());
			//密钥
			pass += "xvDvgqZq";
			BYTE digest[16];
			m_pUtil->MD5ForString( (char*)pass.c_str(), digest );
			string strPswMD5;
			m_pUtil->MD5ToString( strPswMD5, digest );
			char szBuff1[X_LONG_NAME] = {0};
			strncpy( szBuff1, strPswMD5.c_str(), MAX_MD5_ARRAY );
			strupr( szBuff1 );
			strncpy( send.szPsd, szBuff1, MAX_MD5_ARRAY );

			string strMAC = "0026C60EB630";
			//GetMAC(strMAC);
			strncpy( send.szMac, strMAC.c_str(), MAX_MAC_LENGTH );
			//strncpy( send.szMac, strMAC.c_str(), MAX_MAC_LENGTH );

			// 金山快快
			if(ECT_JinShanKuaiKuai == SaveAccount::Inst()->GetCropID())
			{
				WCHAR wcsToken[X_LONG_NAME] = {0};
				INT nRet = GetToken(wcsToken, X_LONG_NAME);
				string sTempName = m_pUtil->UnicodeToAnsi(wcsToken);
				if (KK_SIMPLELOGIN_SUCCESS == nRet)
				{
					strncpy( send.szGUID, sTempName.c_str(), X_LONG_NAME );
				}
			}
			else if (ECT_YYYuYinPingTai ==SaveAccount::Inst()->GetYYCropID() )	// YY
			{
				if(m_bAskTokenID)
				{
					send.bIsGetTokenID = true;
					ShowYYLog("步骤2：向服务器所要登陆tokenID ");
				}
				else
				{

					string sTempName = SaveAccount::Inst()->GetYY_GUID();

 					INT size = (sTempName.length()) * sizeof(CHAR) + sizeof(tagNLC_SafeProof);
 					BYTE* pMsg = new BYTE[size];
 					tagNLC_SafeProof* pCmd = (tagNLC_SafeProof*)pMsg;
 					pCmd->dwID = pCmd->Crc32("NLC_SafeProof");
 
 					pCmd->dwReserved = sTempName.length();
 					ZeroMemory(pCmd->byData, pCmd->dwReserved);
 					memcpy(pCmd->byData, sTempName.c_str(), (pCmd->dwReserved)*sizeof(CHAR));
 					pCmd->dwSize = size;
 
 					pCmd->dwType = m_nPolicy;
 					//pCmd->dwDistributionID = (DWORD)nDistribution;
					pCmd->dwLoginMode = ELM_NULL;
					pCmd->bIsGetURL = false;
 					tstring szWorldName = m_pVarContainer->GetString(_T("gamename"), _T("last_login_server"));
 					pCmd->dwWorldNameCrc = m_pUtil->Crc32(szWorldName.c_str());
 					pCmd->dwCurVersionID = m_pUtil->Crc32(version.c_str());
 
 					pCmd->bIsGetTokenID = false;
 					pCmd->dwChannelID = ECT_YYYuYinPingTai;
 
 					DWORD dwKeyResult;
 					ProofKey(dwKeyResult, pCmd);
 					pCmd->dwProofKey     = dwKeyResult;
 					pCmd->dwConnID = m_dwConnID;

					ShowYYLog("向服务器发送的绑定ID ");
					stringstream ss;
					ss << pCmd->dwConnID;
					ShowYYLog(ss.str());
 					m_pStream->Send(pCmd,size);
 					SAFE_DEL_ARRAY(pMsg);
 					SetState(ELS_WaitProofResult);
 					ShowYYLog("步骤5：向服务器发送GUID ");
 					ShowYYLog("向服务器发送消息----》》》》》 ");
					return;

				}
			}
			else
			{
				//MAC地址+C盘的序列号
				
				DWORD Volue ;
				//API获得某个特定驱动器的序列号,这个程序中是获得C盘的序列号
				::GetVolumeInformation(_T("c:\\"), NULL, 12, &Volue, NULL, NULL, NULL, 10);
				char szBuff[64];
				std::string strTemp(_itoa(Volue, szBuff, 16));
				strMAC += strTemp;
				BYTE digest1[16];
				m_pUtil->MD5ForString( (char*)strMAC.c_str(), digest1 );
				string strGUIDMD5;
				m_pUtil->MD5ToString( strGUIDMD5, digest1 );
				strncpy( send.szGUID, strGUIDMD5.c_str(), MAX_MD5_ARRAY );
			}

		}
		break;
	case 2:					//迅雷
		{
			memcpy( send.szUserName, m_szUserName, sizeof(m_szUserName) );
			memcpy( send.szGUID, m_szEncrypt, sizeof(m_szEncrypt) );
		}
		break;
	case 3://台湾
		{
			char szLength[64];
			string strTemp( _itoa(strAct.length(), szLength, 10) );
			strncpy( send.szUserName, strTemp.c_str(), X_SHORT_NAME );
			if( m_bLoginType )
			{
				int nLength = strAct.length();
				strncpy( send.szGUID, strAct.c_str(), nLength );
				if( !strPassword.empty() && m_szPass != NULL )
				{
					char szBuff[64] = {0};
					m_pUtil->Encrypt( m_szPass, szBuff, 64, 0 );
					memcpy( send.szGUID + nLength, szBuff, 64 );
					//strncpy( send.szGUID + nLength, szBuff, 64 );
				}
			}
			else
			{
				int nLength = strAct.length();
				strncpy( send.szGUID, strAct.c_str(), nLength );
				string pass = m_pUtil->UnicodeToAnsi(strPassword.c_str());
				char szPass[64];
				strcpy( szPass, pass.c_str() );
				if( szPass )
				{
					char szBuff[64] = {0};
					m_pUtil->Encrypt( szPass, szBuff, 64, 0 );
					memcpy( send.szGUID + nLength, szBuff, 64 );
					//strncpy( send.szGUID + nLength, szBuff, 64 );
				}
			}
		}
		break;
	case 4:
		{
			LoginResult SDOALoginResult = SDOAIgw::Inst()->GetSDOALoginResult();
			memcpy( send.szUserName, SDOALoginResult.szSndaid, strlen(SDOALoginResult.szSndaid) + 1 );

			int nAdult = atoi(SDOALoginResult.szIdentityState);
			if( nAdult == 0 )
				m_pStcHint->SetText(g_StrTable[_T("NotAdult")]);

			INT nLength = strlen(SDOALoginResult.szSessionId) + 1;
			DWORD dwSize = sizeof(tagNLC_SafeProof);// - sizeof(send.szGUID) * sizeof(CHAR) + nLength;
			BYTE *pTemp = new BYTE[dwSize];
			ZeroMemory( pTemp, dwSize );
			tagNLC_SafeProof *pSafeProofData = (tagNLC_SafeProof*)pTemp;
			memcpy( pTemp, &send, sizeof(tagNLC_SafeProof) - X_LONG_NAME * sizeof(CHAR) -sizeof(DWORD) -sizeof(BYTE));
			memcpy( pSafeProofData->szGUID, SDOALoginResult.szSessionId, strlen(SDOALoginResult.szSessionId) + 1 );
			pSafeProofData->dwSize = dwSize;
			DWORD dwKeyResult;
			ProofKey(dwKeyResult, pSafeProofData);
			pSafeProofData->dwProofKey = dwKeyResult;
			pSafeProofData->bIsGetURL = false;
			pSafeProofData->dwLoginMode = ELM_NULL;
			m_pStream->Send( pSafeProofData, dwSize );
			SAFE_DELETEA(pTemp);
			SetState(ELS_WaitProofResult);
			return;
		}
		break;
	case 5:
		{
			char *charInit = "";
			strcpy( send.szUserName, charInit);

			string sTempEncryptCode = m_pUtil->UnicodeToAnsi(m_szEncryptCode.c_str());
			strncpy( send.szGUID, sTempEncryptCode.c_str(),X_LONG_NAME);

			string sTempPssWord = m_pUtil->UnicodeToAnsi(m_szPssword.c_str());
			strncpy( send.szPsd, sTempPssWord.c_str(),MAX_MD5_ARRAY);
		}
		break;
	case 6:					//越南模式
		{
			strncpy( send.szUserName, strAct.c_str(), X_SHORT_NAME );
			string pass = m_pUtil->UnicodeToAnsi(strPassword.c_str());
			//密钥
			BYTE digest[16];
			m_pUtil->MD5ForString( (char*)pass.c_str(), digest );
			string strPswMD5;
			m_pUtil->MD5ToString( strPswMD5, digest );
			char szBuff1[X_LONG_NAME] = {0};
			strncpy( szBuff1, strPswMD5.c_str(), MAX_MD5_ARRAY );
			strupr( szBuff1 );
			strncpy( send.szPsd, szBuff1, MAX_MD5_ARRAY );
		}
		break;
	case 7: // OMG mode
		{
			string strMAC;
			GetMAC(strMAC);
			strncpy( send.szMac, strMAC.c_str(), MAX_MAC_LENGTH );

			LoadOMGLoginTWInfo();
			char *charInit = "";
			strcpy( send.szGUID, charInit);

			string sTempName = m_pUtil->UnicodeToAnsi(m_szOMGUserName.c_str());
			strncpy( send.szUserName, sTempName.c_str(),X_SHORT_NAME);

			string sTempPssWord = m_pUtil->UnicodeToAnsi(m_szOMGPassWord.c_str());
			strncpy( send.szPsd, sTempPssWord.c_str(),MAX_MD5_ARRAY);
		}
		break;
	case 9: // DK mode
		{
			LoadDKLoginInfo();
			char *charInit = "";
			strcpy( send.szGUID, charInit);

			string sTempName = m_pUtil->UnicodeToAnsi(m_szOMGUserName.c_str());
			strncpy( send.szUserName, sTempName.c_str(),X_SHORT_NAME);

			if( !m_szOMGPassWord.empty() )
			{
				string pass = m_pUtil->UnicodeToAnsi(m_szOMGPassWord.c_str());
				//密钥
				pass += "xvDvgqZq";
				BYTE digest[16];
				m_pUtil->MD5ForString( (char*)pass.c_str(), digest );
				string strPswMD5;
				m_pUtil->MD5ToString( strPswMD5, digest );
				char szBuff[X_LONG_NAME] = {0};
				strncpy( szBuff, strPswMD5.c_str(), MAX_MD5_ARRAY );
				strupr( szBuff );
				strncpy( send.szPsd, szBuff, MAX_MD5_ARRAY );
			}

		}
		break;
	case 10: // 俄罗斯
		{
			string t_userID;
			string t_appKey;
			string t_tokenID;
			string strCMDLine = TObjRef<Util>()->UnicodeToAnsi( GetCommandLine() );
			if( !strCMDLine.empty() && strCMDLine.length() <= 1024 )
			{
				char  cmd[1024] = {0};
				const char *pCommandLine = strstr( strCMDLine.c_str(), ".exe" );
				if( pCommandLine != NULL )
				{
					strcpy( cmd, pCommandLine );
					if( strlen( cmd))
					{
						char *pPart = strtok( cmd, " " );
						pPart = strtok( NULL, " " );
						if(NULL != pPart)
						{
							t_userID = pPart;
							strncpy( send.szUserID, pPart,X_SHORT_NAME);
						}

						pPart = strtok( NULL, " " );
						if(NULL != pPart)
						{
							t_appKey = pPart;
							strncpy( send.szAppKey, pPart,X_APPKEY);
						}

						pPart = strtok( NULL, " " );
						if(NULL != pPart)
						{
							t_tokenID = pPart;
							strncpy( send.szToken, pPart,X_TOKEN);
						}
					}
				}
			}
		}
		break;
	case 11:	//韩国Web登陆
		{
			string strMAC;
			GetMAC(strMAC);
			strncpy( send.szMac, strMAC.c_str(), MAX_MAC_LENGTH );

			LoadKoreaWebInfo();
			string sTempName = m_pUtil->UnicodeToAnsi(m_szKoreaWebUID.c_str());
			sTempName.append("\0");
			strncpy(send.szGUID, sTempName.c_str(), X_LONG_NAME);
// 			char *charInit = "";
// 			strcpy( send.szGUID, charInit);
// 
// 			string sTempName = m_pUtil->UnicodeToAnsi(m_szOMGUserName.c_str());
// 			strncpy( send.szUserName, sTempName.c_str(),X_SHORT_NAME);
// 
// 			string sTempPssWord = m_pUtil->UnicodeToAnsi(m_szOMGPassWord.c_str());
// 			strncpy( send.szPsd, sTempPssWord.c_str(),MAX_MD5_ARRAY);
		}
		break;
	
	}
	DWORD dwCropID = SaveAccount::Inst()->GetCropID();
	if (SaveAccount::Inst()->GetYYCropID())
		dwCropID = SaveAccount::Inst()->GetYYCropID();
	send.dwChannelID = dwCropID;
    DWORD dwKeyResult;
    ProofKey(dwKeyResult, (&send));
    send.dwProofKey     = dwKeyResult;

	//log
	if(m_nPolicy == 11)
	{
// 		ofstream wFileStream;
// 		stringstream strTempTream;
// 		strTempTream << send.dwProofKey <<"+" <<send.dwType<<"+"<<send.dwCurVersionID <<"+"<<send.dwWorldNameCrc<<"+"<<send.szGUID<<endl;
// 		wFileStream.open("c:\\sm_KoreaWebLogin1.txt",ios::out|ios::app/*|ios::trunc*/);
// 		if (wFileStream)
// 		{
// 			wFileStream <<  strTempTream.str().c_str();
// 			wFileStream.close();
// 		}

		//保存上一次的发送信息，错误后再发送一次
		if(P_VALID(m_sendSave))
			SAFE_DELETEA(m_sendSave);

		DWORD dwSize = sizeof(tagNLC_SafeProof);
		BYTE *pTemp = new BYTE[dwSize];
		ZeroMemory( pTemp, dwSize );
		m_sendSave = (tagNLC_SafeProof*)pTemp;
		memcpy( pTemp, &send, sizeof(tagNLC_SafeProof));

	}

	
	m_pStream->Send(&send, sizeof(send));
	SaveAccount::Inst()->SetOpenPassport(FALSE);
	SetState(ELS_WaitProofResult);
	m_bOpenWaitFlag = FALSE;

}


//-----------------------------------------------------------------------------
// login
//-----------------------------------------------------------------------------
VOID LoginFrame::Connect(bool bUnrealLogin/* = TRUE*/)
{
	if ( !bUnrealLogin ) //init 假登陆用 
	{
		if ( !g_bUnrealSend &&  m_pStream->IsConnected() )  
		{
			SendUnrealLoginInfo();    
			g_bUnrealSend = TRUE;
		}

		if( m_pStream->IsConnected() )
			return;

		if( m_pStream->IsTryingToConnect() )
			return;	// 仍然在尝试连接

		if ( g_bUnrealInit )
			return;

		tstringstream stream;
		stream << _T("last_login_server");
		tstring strIP = m_pVarContainer->GetString(_T("ip"), stream.str().c_str());
		INT nPort = m_pVarContainer->GetDword(_T("port"), stream.str().c_str());

		// 尝试连接
		m_pStream->TryToConnect(m_pUtil->UnicodeToUnicode8(strIP.c_str()), nPort);
		g_bUnrealInit = TRUE;

		return;
	}

	this->SaveLastLoginInfo();
 	if (m_nPolicy == 5)
 	{
		HideLoginFrame(TRUE);
 	}
	else if ( m_nPolicy == 7 || m_nPolicy == 9 || m_nPolicy == 10 )
	{
		HideLoginFrame(TRUE);
	}
 	else
 	{
		this->SetUIEnable(FALSE);
 	}
	
	if( m_pStream->IsConnected() )
	{
		if ( OPEN_PASSPORT && m_bSendUrlLoginMsg && m_nPolicy==1 )
			this->SendChooseUrlMsg(m_nCurChoose);
		else
			this->SendLoginMsg();
		return;
	}

	if( m_pStream->IsTryingToConnect() )
		return;	// 仍然在尝试连接

	if( m_eState == ELS_Connect )	// 连接失败
	{
		SetState(ELS_WaitInput);
		this->SetUIEnable(TRUE);
		m_pStcHint->SetText(g_StrTable[_T("ConnectFail")]);
		switch( m_nPolicy )
		{
		case 2:		//迅雷
			{
				m_eHint = EXLHint_ConnectFail;
				JudegPolicy();
			}
			break;
		case 4:
			{
				SDOAIgw::Inst()->SetShowLoginFlag(TRUE);
			}
			break;
		case 5:
			{
				HideLoginFrame(TRUE);
				m_pButtonQuit ->SetInvisible(FALSE); 
				m_pButtonLogin->SetInvisible(FALSE); 
			}
			break;
		case 7: // OMG mode
		case 9:	//dk 
		case 10:	
			{
				HideLoginFrame(TRUE);
				m_pButtonQuit ->SetInvisible(FALSE); 
				m_pButtonLogin->SetInvisible(FALSE); 
			}
			break;
		case 11:
			{
				SetKoreaTimaOutError();
			}
		}

		if (m_nPolicy == 1 &&  SaveAccount::Inst()->GetCropID())
		{
			HideLoginFrame(TRUE);
			m_pButtonQuit ->SetInvisible(FALSE); 
			m_pButtonLogin->SetInvisible(FALSE); 
		}
		return;
	}

	if( !m_bSelectedWorld )
		return;

	tstringstream stream;
	stream << _T("last_login_server");
	tstring strIP = m_pVarContainer->GetString(_T("ip"), stream.str().c_str());
	INT nPort = m_pVarContainer->GetDword(_T("port"), stream.str().c_str());
	
	// 尝试连接
	m_dwCurPos = 0;
	m_pStream->TryToConnect(m_pUtil->UnicodeToUnicode8(strIP.c_str()), nPort);
	if ( (m_nPolicy == 7||m_nPolicy==9 ||m_nPolicy==10 || ( m_nPolicy == 1 && SaveAccount::Inst()->GetCropID()) )&& !m_pWndCancelQueue->IsInvisible() && !m_pWndQueue->IsInvisible() )
	{
		m_pStcHint->SetInvisible(TRUE);
	}
	else
	{
		m_pStcHint->SetText(g_StrTable[_T("WaitAuth")]);
	}
	SetState(ELS_Connect);
}

//-----------------------------------------------------------------------------
// WaitResponse
//-----------------------------------------------------------------------------
VOID LoginFrame::WaitProofResult()
{
	DWORD dwLimit = 30000;
	if ( OPEN_PASSPORT && m_bOpenWaitFlag&& m_nPolicy==1  )
		dwLimit = 180000;
	else if(m_nPolicy == 11)
		dwLimit = 60000;	//韩国要改成2分钟....
	//超时
	if(timeGetTime() - m_dwStateTime > dwLimit)
	{
		stringstream ts;
		ts<<"等待验证信息超过时间限制"<<dwLimit<<"毫秒.";
		OpenPassportLog(ts.str());

		m_pStcHint->SetText(g_StrTable[_T("ConnectFail")]);
		this->SetUIEnable(TRUE);
		m_eMB = EMBP_NULL;
		CutOffLogin();
		switch( m_nPolicy )
		{
		case 2:		//迅雷
			{
				m_eHint = EXLHint_ConnectFail;
				JudegPolicy();
			}
			break;
		case 4:
			SDOAIgw::Inst()->SetShowLoginFlag(TRUE);
			break;
		case 5:
			{
				HideLoginFrame(TRUE);
				m_pButtonQuit->SetInvisible(FALSE);
				m_pButtonLogin->SetInvisible(FALSE); 
			}
			break;
		case 7: // OMG mode
		case 9:	//dk mode
		case 10:
			{
				m_pStcHint->SetInvisible(FALSE);
				HideLoginFrame(TRUE);
				m_pButtonQuit->SetInvisible(FALSE);
				m_pButtonLogin->SetInvisible(FALSE); 
			}
			break;
		case 11:
			{
				//韩国登陆，超时一次后再尝试一次
				if(m_bIsNeedSendAgain)
				{
					m_pStream->Send(m_sendSave, sizeof(*m_sendSave));
					m_bIsNeedSendAgain = !m_bIsNeedSendAgain;
					SetKoreaTimaOutError();
				}
				else
				{
					SetKoreaTimaOutError();
				}
			}
		}
		return;
	}

	LPBYTE pMsg = NULL;
	DWORD dwMsgSize = 0;

	pMsg = m_pStream->Recv(dwMsgSize);
	if( P_VALID(pMsg) )
	{
		ASSERT(m_dwCurPos + dwMsgSize < sizeof(m_pMsgBuff));
		memcpy(m_pMsgBuff+m_dwCurPos, pMsg, dwMsgSize);
		m_pStream->FreeRecved(pMsg);
		m_dwCurPos += dwMsgSize;
	}
	pMsg = m_pMsgBuff;
	if(m_dwCurPos >= sizeof(tagNetCmd) && ((tagNetCmd*)pMsg)->dwSize <= m_dwCurPos)
	{
		DWORD dwSize = ((tagNetCmd*)pMsg)->dwSize;
		tagNLS_MiBao* pMiBao = (tagNLS_MiBao*)pMsg;
		if( pMiBao->dwID == m_pUtil->Crc32("NLS_MiBao") )
		{
			m_strMiBao = pMiBao->szMiBao;
			this->SetMBWndShow(TRUE);
			this->SetUIEnable(FALSE);
			m_pWnd->SetInvisible(TRUE);
			this->MBPhaseHint();
			SetState(ELS_MiBao);
			m_strInputMiBao = _T("");
			m_dwCurPos -= dwSize;
			for(DWORD i=0; i<m_dwCurPos; i++)
				m_pMsgBuff[i] = m_pMsgBuff[i+dwSize];		
			return;
		}

		tagNLS_ProofResult* pRecv = (tagNLS_ProofResult*)pMsg;
		if( pRecv->dwID == m_pUtil->Crc32("NLS_ProofResult") )
		{
			SaveAccount::Inst()->SetLoginID(pRecv->dwLoginSessionID);
			//保存玩家是否填写
			SaveAccount::Inst()->SetIsNeedSetTelNum(pRecv->bNeedTelNum);
			if (E_YYYuYinPingTai_GetTokenID == pRecv->dwErrorCode && 1 == SaveAccount::Inst()->GetLoginMode() && g_yyDemo.HaveInited())
			{
				char szToken[X_HUGE_NAME*2]={0};
				memcpy(szToken,pRecv->byData,sizeof(BYTE)*pRecv->dwReserved);
				g_yyDemo.m_pfnSetLoginKey(szToken);
				m_bAskTokenID = false;
				m_dwConnID = pRecv->dwConnID;
				ShowYYLog("收到YY绑定ID ");
				stringstream ss;
				ss << pRecv->dwConnID;
				ShowYYLog(ss.str());
				ShowYYLog("步骤3：服务器通信成功，拿到tokenID，通信YY获取GUID ");
				m_dwCurPos -= dwSize;
				for(int i=0; i<m_dwCurPos; i++)
					m_pMsgBuff[i] = m_pMsgBuff[i+dwSize];	
				SetState(ELS_WaitInput);	

				return;
			}

			if( pRecv->dwErrorCode != E_Success )
			{

				if (ECT_YYYuYinPingTai ==SaveAccount::Inst()->GetYYCropID() && g_yyDemo.HaveInited() )	
				{
					if(pRecv->dwErrorCode == E_ProofResult_Account_Freezed ||
						pRecv->dwErrorCode == E_ProofResult_Disabled ||
						pRecv->dwErrorCode == E_ProofResult_Forbid_MemberCenter||
						pRecv->dwErrorCode == E_ProofResult_Forbid_GMTool||
						pRecv->dwErrorCode == E_ProofResult_Forbid_CellPhone||
						pRecv->dwErrorCode == E_ProofResult_Forbid_MiBao||
						pRecv->dwErrorCode == E_ProofResult_GameNet_Block)
					{
						g_yyDemo.m_pfnLoginCancel("Account Error");//("账号未激活或已停封或锁定");
					}
					else if(pRecv->dwErrorCode == E_ProofResult_Account_In_Use)
					{
						g_yyDemo.m_pfnLoginCancel("OnLine");//("已在线");
					}
					else if(pRecv->dwErrorCode == E_ProofResult_Account_No_Match)
					{
						g_yyDemo.m_pfnLoginCancel("PassWord Error");//("用户名或密码错误");
					}
					else if(pRecv->dwErrorCode == E_SelectWorld_Server_Full||
						pRecv->dwErrorCode == E_SelectWorld_Server_Suspended||
						pRecv->dwErrorCode == E_SelectWorld_Server_Maintenance||
						pRecv->dwErrorCode == E_SelectWorld_Failed)
					{
						g_yyDemo.m_pfnLoginCancel("Server Error");//("服务器异常或维护");
					}
					else 
					{
						g_yyDemo.m_pfnLoginCancel("Other");//("其他异常");
					}
				}

				ShowUserPwdInfo(pRecv->dwErrorCode);
				if( m_nPolicy == 4 )
					SDOAIgw::Inst()->SetShowLoginFlag(TRUE);
				else if( m_bLoginType && m_nPolicy != 5 )
				{
					HideLoginFrame(false);
					m_bLoginType = !m_bLoginType;
				}
				else if ( m_nPolicy == 5 )
				{
					HideLoginFrame(TRUE);
					m_pButtonQuit->SetInvisible(FALSE);
					m_pButtonLogin->SetInvisible(FALSE); 
				}	
				else if ( m_nPolicy == 7||m_nPolicy == 9 || (m_nPolicy == 1 &&  SaveAccount::Inst()->GetCropID() ) )
				{
					if (pRecv->dwErrorCode != E_ProofResult_Queue)
					{
						m_pStcHint->SetInvisible(FALSE);
						HideLoginFrame(TRUE);
						m_pButtonQuit->SetInvisible(FALSE);
						m_pButtonLogin->SetInvisible(FALSE); 
					}
					else
					{
						m_pWnd->SetInvisible(TRUE);
					}
				}	
				m_dwCurPos -= dwSize;
				for(DWORD i=0; i<m_dwCurPos; i++)
					m_pMsgBuff[i] = m_pMsgBuff[i+dwSize];	
				return;
			}
			else 
			{
				if(m_nPolicy == 11 && P_VALID(m_pWndKoreaPassWord) )
				{
					//m_pWndKoreaPassWord->SetInvisible(TRUE);
					//GetRandomNumber();
					m_pStcKoreaLoginTip->SetText(g_StrTable[_T("EnterGame")]);
					m_pStcKoreaLoginTip->SetRefresh(TRUE);
				}
			}


			if (1 == SaveAccount::Inst()->GetLoginMode()&& g_yyDemo.HaveInited())
			{
				if (ECT_YYYuYinPingTai ==SaveAccount::Inst()->GetYYCropID() )	
				{
					ShowYYLog("步骤6：服务器验证通过 success ");
					g_yyDemo.m_pfnLoginSucceed();

					tstring strAccount = m_pUtil->AnsiToUnicode(pRecv->szUserName);
					SaveAccount::Inst()->SetAccount(strAccount.c_str());
				}
				g_yyDemo.m_pfnShowOpid(0,1000,10);
			}
			//不是openpassport还是用之前的
			if ( OPEN_PASSPORT && TRUE ==SaveAccount::Inst()->IsOpenPassport() && m_nPolicy==1 ) 
			{
				tstring strAccount = m_pUtil->AnsiToUnicode(pRecv->szUserName);
				SaveAccount::Inst()->SetAccount(strAccount.c_str());
				string szPath = m_pUtil->UnicodeToAnsi(strAccount.c_str());
				OpenPassportLog(szPath.c_str());
			}
			m_dwZoneIP		= pRecv->dwIP;
			m_dwZonePort	= pRecv->dwPort;
			m_dwAccountID	= pRecv->dwAccountID;
			m_dwVerifyCode	= pRecv->dwVerifyCode;
			SaveAccount::Inst()->SetAccountID(m_dwAccountID);

			if (pRecv->dwErrorCode == 0)
				m_bGuard = pRecv->bGuard;
			CHAR* szIP = m_pStream->IP2String(m_dwZoneIP);
			m_pSession->Connect(m_pUtil->Unicode8ToUnicode(szIP), m_dwZonePort);		//与zone服务器建立连接
			SetState(ELS_ConnectZoneServer);	
			if( m_nPolicy == 4 )
			{
				if( pRecv->bGuard && pRecv->dwReserved == 1 )
				{
					if( P_VALID(GetObj("NeedProof")) )
						KillObj("NeedProof");
					CreateObj( "NeedProof", "MsgBoxEx" );
					TObjRef<MsgBoxEx>("NeedProof")->Init( _T(""), g_StrTable[_T("NeedProof")] );
				}
				else if( pRecv->bGuard )
				{
					if( P_VALID(GetObj("NotAdult")) )
						KillObj("NotAdult");
					CreateObj( "NotAdult", "MsgBoxEx" );
					TObjRef<MsgBoxEx>("NotAdult")->Init( _T(""), g_StrTable[_T("NotAdult")] );
				}
			}
			m_dwCurPos -= dwSize;
			for(DWORD i=0; i<m_dwCurPos; i++)
				m_pMsgBuff[i] = m_pMsgBuff[i+dwSize];		
			return;
		}
		tagNLS_SecondPsdProof* pKorea = (tagNLS_SecondPsdProof*)pMsg;
		if( pKorea->dwID == m_pUtil->Crc32("NLS_SecondPsdProof") )
		{
			m_pWndKoreaPassWord->SetInvisible(FALSE);
			if(pKorea->bIsFirstLogin)	//第一次登陆，设置初始密码
			{
				KoreaChangeUI(EKCU_SetPassWord);
			}
			else	//显示登录面板
			{
				KoreaChangeUI(EKCU_Login);
			}
		}
		tagNLS_SetSecondPsd* pKoreaSetPassWord = (tagNLS_SetSecondPsd*)pMsg;
		if( pKoreaSetPassWord->dwID == m_pUtil->Crc32("NLS_SetSecondPsd") )	//二次密码设置
		{
			if(pKoreaSetPassWord->dwEorrorCode != E_Success)
			{
				switch(pKoreaSetPassWord->dwEorrorCode)
				{
				case E_KoreaProof_InputOldPsd_Error:	// 旧密码输入有误
					m_pStcChangeWordTip->SetText(g_StrTable[_T("KoreaProof_InputOldPsd_Error")]);
					break;
				case E_KoreaProof_InputNewPsd_Error:	// 新密码输入有误
					m_pStcChangeWordTip->SetText(g_StrTable[_T("KoreaProof_InputNewPsd_Error")]);
					break;
				case E_KoreaProof_TwicePsd_ChgError:	// 密码修改有误
					m_pStcChangeWordTip->SetText(g_StrTable[_T("KoreaProof_TwicePsd_ChgError")]);
					break;
				case E_KoreaProof_TwicePsd_SetError:	// 密码设置有误
					m_pStcChangeWordTip->SetText(g_StrTable[_T("KoreaProof_TwicePsd_SetError")]);
					break;
				case E_KoreaProof_TwicePsd_InputError:  // 输入密码有误
					m_pStcChangeWordTip->SetText(g_StrTable[_T("KoreaProof_TwicePsd_InputError")]);
					break;
				}
				m_pStcChangeWordTip->SetRefresh(TRUE);
			}
			else
			{
				KoreaChangeUI(EKCU_Login);
				m_pStcKoreaLoginTip->SetText(g_StrTable[_T("KoreaProof_SetPsdSuccess")]);
				m_pStcKoreaLoginTip->SetRefresh(TRUE);
			}
		}
		tagNLS_InputSecondPsd* pKoreaLogin = (tagNLS_InputSecondPsd*)pMsg;
		if( pKoreaLogin->dwID == m_pUtil->Crc32("NLS_InputSecondPsd") )	//二次密码设置
		{
			if(pKoreaLogin->dwEorrorCode != E_Success)
			{
				m_n8KoreaTryTimes++;
				if(m_n8KoreaTryTimes >= 3)
				{
					if(P_VALID(GetObj("KoreaLoginErr3Times")))
						KillObj("KoreaLoginErr3Times");

					CreateObj("KoreaLoginErr3Times", "MsgBox");
					TObjRef<MsgBox>("KoreaLoginErr3Times")->Init(_T(""), g_StrTable[_T("KoreaLoginErr3Times")], _T("Msgbox_KoreaLoginErr3Times"), MBF_OK, TRUE);
					m_pWndKoreaPassWord->SetInvisible(TRUE);	//隐藏登陆框
					return;
				}

				switch(pKoreaLogin->dwEorrorCode)
				{
				case E_KoreaProof_InputOldPsd_Error:	// 旧密码输入有误
					m_pStcKoreaLoginTip->SetText(g_StrTable[_T("KoreaProof_InputOldPsd_Error")]);
					break;
				case E_KoreaProof_InputNewPsd_Error:	// 新密码输入有误
					m_pStcKoreaLoginTip->SetText(g_StrTable[_T("KoreaProof_InputNewPsd_Error")]);
					break;
				case E_KoreaProof_TwicePsd_ChgError:	// 密码修改有误
					m_pStcKoreaLoginTip->SetText(g_StrTable[_T("KoreaProof_TwicePsd_ChgError")]);
					break;
				case E_KoreaProof_TwicePsd_SetError:	// 密码设置有误
					m_pStcKoreaLoginTip->SetText(g_StrTable[_T("KoreaProof_TwicePsd_SetError")]);
					break;
				case E_KoreaProof_TwicePsd_InputError:  // 输入密码有误
					m_pStcKoreaLoginTip->SetText(g_StrTable[_T("KoreaProof_TwicePsd_InputError")]);
					break;
				}
				m_pStcKoreaLoginTip->SetRefresh(TRUE);
			}
			GetRandomNumber();
		}

		m_dwCurPos -= dwSize;
		for(DWORD i=0; i<m_dwCurPos; i++)
			m_pMsgBuff[i] = m_pMsgBuff[i+dwSize];		
	}
}

// 显示用户名\密码错误信息
VOID LoginFrame::ShowUserPwdInfo(DWORD nInfo)
{
	switch(nInfo)
	{
	case E_ProofResult_Account_No_Match:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Account_No_Match")]);
		m_eHint = EXLHint_Account_No_Match;
		break;
	case E_ProofResult_Account_In_Use:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Account_In_Use")]);
		m_eHint = EXLHint_Account_In_Use;
		break;
	case E_ProofResult_Account_Freezed:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Account_Freezed")]);
		m_eHint = EXLHint_Account_Freezed;
		break;
	case E_ProofResult_Wrong_Type:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Wrong_Type")]);
		m_eHint = EXLHint_Wrong_Type;
		break;
	case E_ProofResult_Mibao_Error:
		{
			this->SetMBWndShow(FALSE);
			this->SetUIEnable(TRUE);
			m_pStcHint->SetText(g_StrTable[_T("ProofResult_Account_No_Match")]);
			m_eHint = EXLHint_Account_No_Match;
			m_eMB = EMBP_NULL;
		}
		break;
	case E_ProofResult_Queue:
		{
			m_pWndQueue->SetInvisible(FALSE);
			m_pWndCancelQueue->SetInvisible(FALSE);
			m_pWnd->SetInvisible(TRUE);
			SetState(ELS_Queue);
			if ( m_nPolicy == 7 || m_nPolicy == 9|| m_nPolicy == 10 || ( m_nPolicy == 1 && SaveAccount::Inst()->GetCropID()))
			{
				m_pStcHint->SetInvisible(TRUE);
			}
			return;
		}
		break;
	case E_ProofResult_Wrong_Build_Number:
		{
			m_pStcHint->SetText(g_StrTable[_T("ProofResult_Wrong_Build_Number")]);
			WIN32_FIND_DATA wfd;
			HANDLE hFile;
			tstring szFindFilter = Kernel::Inst()->GetWorkPath();
			szFindFilter += _T("launcher.exe");
			hFile=FindFirstFile(szFindFilter.c_str(),&wfd);
			if(hFile!=INVALID_HANDLE_VALUE)
			{
				ShellExecute(NULL, _T("open"), szFindFilter.c_str(), NULL, NULL, SW_SHOW);
				SetState(ELS_Launcher);
				return;
			}
			else 
			{
				m_pStcHint->SetText(g_StrTable[_T("ProofResult_NoFindLauncher")]);
				m_eHint = EXLHint_NoFindLauncher;
			}
		}
		break;
	case E_ProofResult_Disabled:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Disabled")]);
		m_eHint = EXLHint_Disabled;
		break;
	case E_SelectWorld_Server_Full:
		m_pStcHint->SetText(g_StrTable[_T("SelectWorld_Server_Full")]);
		m_eHint = EXLHint_Wrong_Type;
		break;
	case E_SelectWorld_Server_Suspended:
		m_pStcHint->SetText(g_StrTable[_T("SelectWorld_Server_Suspended")]);
		m_eHint = EXLHint_Server_Suspended;
		break;
	case E_SelectWorld_Server_Maintenance:
		m_pStcHint->SetText(g_StrTable[_T("SelectWorld_Server_Maintenance")]);
		m_eHint = EXLHint_Server_Maintenance;
		break;
	case E_SelectWorld_Failed:
		m_pStcHint->SetText(g_StrTable[_T("SelectWorld_Failed")]);
		m_eHint = EXLHint_Failed;
		break;
	case E_SelectWorld_Short_Time:
		m_pStcHint->SetText(g_StrTable[_T("SelectWorld_Short_Time")]);
		m_eHint = EXLHint_Short_Time;
		break;
	case E_SelectWorld_No_Select:
		m_pStcHint->SetText(g_StrTable[_T("SelectWorld_No_Select")]);
		m_eHint = EXLHint_No_Select;
		break;
	case E_SelectWorld_GameWorldName_Wrong:
		m_pStcHint->SetText(g_StrTable[_T("SelectWorld_GameWorldName_Wrong")]);
		m_eHint = EXLHint_GameWorldName_Wrong;
		break;
	case E_SystemError:
		m_pStcHint->SetText(g_StrTable[_T("SystemError")]);
		m_eHint = EXLHint_SystemError;
		break;
	case E_ProofResult_Forbid_MemberCenter:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Forbid_MemberCenter")]);
		m_eHint = EXLHint_ProofResult_Forbid_MemberCenter;
		break;
	case E_ProofResult_Forbid_GMTool:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Forbid_GMTool")]);
		m_eHint = EXLHint_ProofResult_Forbid_GMTool;
		break;
	case E_ProofResult_Forbid_CellPhone:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Forbid_CellPhone")]);
		m_eHint = EXLHint_ProofResult_Forbid_CellPhone;
		break;
	case E_ProofResult_Forbid_MiBao:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Forbid_MiBao")]);
		m_eHint = EXLHint_ProofResult_Forbid_MiBao;
		break;
	case E_ProofResult_Forbid_Wrong_OTP:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Forbid_Wrong_OTP")]);
		m_eHint = EXLHint_SystemError;
		break;
	case E_ProofResult_Forbid_Wrong_Event_OTP:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Forbid_Wrong_Event_OTP")]);
		m_eHint = EXLHint_SystemError;
		break;
	case E_ProofResult_Orange_VipBar_Limited:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_Orange_VipBar_Limited")]);
		m_eHint = EXLHint_SystemError;
		break;
	case E_ProofResult_AuthServer_Unreachable:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_AuthServer_Unreachable")]);
		m_eHint = EXLHint_SystemError;
		break;
	case E_ProofResult_EmptyField:
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_EmptyField")]);
		m_eHint = EXLHint_SystemError;
		break;
	case E_PrrofResult_WrongFormat:
		m_pStcHint->SetText(g_StrTable[_T("PrrofResult_WrongFormat")]);
		m_eHint = EXLHint_SystemError;
		break;
	// Jason 2010-4-26 v2.1.0
	case E_PrrofResult_PKRunawayPunishmentState:
		m_pStcHint->SetText(g_StrTable[_T("PrrofResult_PKRunawayPunishmentState")]);
		m_eHint = EXLHint_SystemError;
		break;
	case E_ProofResult_TimeOut: //服务器认证时间过长（韩国）
		m_pStcHint->SetText(g_StrTable[_T("ProofResult_TimeOut")]);
		break;
	default:
		m_pStcHint->SetText(g_StrTable[_T("SystemError")]);
		m_eHint = EXLHint_SystemError;
		break;
	}
	this->SetUIEnable(TRUE);
	CutOffLogin();
	switch(m_nPolicy)
	{
	case 1:
		{
			if( SaveAccount::Inst()->GetCropID())
			{
				HideLoginFrame(TRUE);
				m_pButtonQuit->SetInvisible(FALSE);
				m_pButtonLogin->SetInvisible(FALSE);
			}
		}
	case 2:  //迅雷平台登录时,重新调迅雷界面
		{
			JudegPolicy();
		}
		break;
	case 5:  //韩国服务器登陆时，界面功能调整
		{
			HideLoginFrame(TRUE);
			m_pButtonQuit->SetInvisible(FALSE);
			m_pButtonLogin->SetInvisible(FALSE);
		}
		break;
	case 7:  // OMG 
	case 9:	//dk mode
	case 10:
		{
			HideLoginFrame(TRUE);
			m_pButtonQuit->SetInvisible(FALSE);
			m_pButtonLogin->SetInvisible(FALSE);
		}
		break;
	}
}

VOID LoginFrame::WaitQueue()
{
	LPBYTE pMsg = NULL;
	DWORD dwMsgSize = 0;

	pMsg = m_pStream->Recv(dwMsgSize);
	if( P_VALID(pMsg) )
	{
		ASSERT(m_dwCurPos + dwMsgSize < sizeof(m_pMsgBuff));
		memcpy(m_pMsgBuff+m_dwCurPos, pMsg, dwMsgSize);
		m_pStream->FreeRecved(pMsg);
		m_dwCurPos += dwMsgSize;
	}
	pMsg = m_pMsgBuff;
	if(m_dwCurPos >= sizeof(tagNetCmd) && ((tagNetCmd*)pMsg)->dwSize <= m_dwCurPos)
	{
		DWORD dwSize = ((tagNetCmd*)pMsg)->dwSize;
		tagNLS_Queue* pRecv = (tagNLS_Queue*)pMsg;
		if( pRecv->dwID == m_pUtil->Crc32("NLS_Queue") )
		{
			TCHAR szBuff[X_LONG_NAME] = {0};
			_stprintf(szBuff, g_StrTable[_T("Login_Queue0")], pRecv->dwPosition);
			m_pStcPosition->SetText(szBuff);

			DWORD dwTime = pRecv->dwTime;
			if( dwTime!=GT_INVALID && dwTime==QUEUE_TIME )
				_stprintf(szBuff, g_StrTable[_T("Login_Queue1")], QUEUE_TIME/60);
			else if( dwTime!=GT_INVALID && dwTime>60 )
				_stprintf(szBuff, g_StrTable[_T("Login_Queue2")], dwTime/60+1);
			else if( dwTime!=GT_INVALID && dwTime<=60 && dwTime>0)
				_stprintf(szBuff, g_StrTable[_T("Login_Queue3")], dwTime);
			else 
				_stprintf(szBuff, g_StrTable[_T("Login_Queue4")]);
			m_pStcTime->SetText(szBuff);

			if( pRecv->dwPosition==0 )
			{
				SetState(ELS_WaitProofResult);
				m_pWndQueue->SetInvisible(TRUE);
				m_pWndCancelQueue->SetInvisible(TRUE);
				m_pStcHint->SetInvisible(FALSE);
				if( !m_bLoginType  && m_nPolicy != 4 )
				{
					if ( !m_pWndCancelQueue->IsInvisible() 
						&& !m_pWndQueue->IsInvisible() )
					{
						m_pWnd->SetInvisible(TRUE);
					}
					else if(m_nPolicy != 11)
					{
						m_pWnd->SetInvisible(FALSE);
					}
				}
				m_pWndKey->SetInvisible(m_pWndKey->IsInvisible());
				/*m_pStcPosition->SetText(_T(""));
				m_pStcTime->SetText(_T(""));*/
			}

			m_dwCurPos -= dwSize;
			for(DWORD i=0; i<m_dwCurPos; i++)
				m_pMsgBuff[i] = m_pMsgBuff[i+dwSize];	
			return;
		}
		//显示换服务器tips广播
		tagNLS_Broadcast* pBroadcast = (tagNLS_Broadcast*)pMsg;
		if( pBroadcast->dwID == m_pUtil->Crc32("NLS_Broadcast") )
		{
			if (P_VALID(m_pWndQueue)&&P_VALID(m_pWndCancelQueue))
			{
				tstringstream tsTips;
				tsTips<<pBroadcast->szBMsg;
				m_pStcTips->SetInvisible(FALSE);
				m_pStcTips->SetText(tsTips.str().c_str());
			}
			m_dwCurPos -= dwSize;
			for(DWORD i=0; i<m_dwCurPos; i++)
				m_pMsgBuff[i] = m_pMsgBuff[i+dwSize];	
			return;
		}
		m_dwCurPos -= dwSize;
		for(DWORD i=0; i<m_dwCurPos; i++)
			m_pMsgBuff[i] = m_pMsgBuff[i+dwSize];	
	}
}

// 是否连接上区域服务器
VOID LoginFrame::TryIsConnectZoneSer()
{
	//-----------------------------------
	ofstream wFileStream;
	stringstream strTempTream;
	strTempTream << "-----------Start--------------"<<endl;
	strTempTream << "Try to Connect Server" << endl;
	wFileStream.open("log\\client.txt",ios::out|ios::app/*|ios::trunc*/);
	if (wFileStream)
	{
		wFileStream <<  strTempTream.str().c_str();
	}

	//-----------------------------------
	if(m_pSession->IsConnect())
	{
		m_pStream->Disconnect();						//断开与login服务器的连接
		tagNC_JoinGame joinGameMsg;
		string strMAC;
		GetMAC(strMAC);
		joinGameMsg.dwAccountID = m_dwAccountID;
		joinGameMsg.dwVerifyCode = m_dwVerifyCode;
		strncpy( joinGameMsg.szMac, strMAC.c_str(), MAX_MAC_LENGTH );
		m_pSession->Send(&joinGameMsg);
		m_dwSentJoinGame = timeGetTime();
		SetState(ELS_SentJoinGame);

		//
		stringstream strTream;
		strTream << "Send JoinGame To Server" << endl;
		if (wFileStream)
		{
			wFileStream <<  strTream.str().c_str();
		}
	}
	else if(timeGetTime() - m_dwStateTime > 10000)
	{
		//
		stringstream strTream1;
		strTream1 << "Join Game Time Out" << endl;
		if (wFileStream)
		{
			wFileStream <<  strTream1.str().c_str();
		}



		//如果超时
		CutOffLogin();
		m_pStcHint->SetText(g_StrTable[_T("ConnectFail")]);
		this->SetUIEnable(TRUE);
		if( m_nPolicy == 4 )
			SDOAIgw::Inst()->SetShowLoginFlag(TRUE);
		if( m_nPolicy == 5 )	
		{
			HideLoginFrame(TRUE);
			m_pButtonQuit->SetInvisible(FALSE);
			m_pButtonLogin->SetInvisible(FALSE); 
		}
		if( m_nPolicy == 7||m_nPolicy == 9||m_nPolicy == 10|| ( m_nPolicy == 1 && SaveAccount::Inst()->GetCropID()) )	
		{
			m_pStcHint->SetInvisible(FALSE);
			HideLoginFrame(TRUE);
			m_pButtonQuit->SetInvisible(FALSE);
			m_pButtonLogin->SetInvisible(FALSE); 
		}
		if(m_nPolicy == 11)
		{
			SetKoreaTimaOutError();
		}
	}

	if (wFileStream)
	{
		stringstream strTream2;
		strTream2 << "-----------End------------" << endl;
		wFileStream <<  strTream2.str().c_str();
		
		wFileStream.close();	//关闭文件
	}
	
}

//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------

const float MAXYAW=_DegToRad(5.0f);
const float MINYAW=_DegToRad(-5.0f);
const float MAXZ = 100000.0f;
const float ZSTEP = 100.0f;
const float YSTEP = 150.0f;
const float MAXY = 90000.0f;
const float PITCHSTEP = 0.0017f;

VOID LoginFrame::Update()
{
	OpenPassportUpdate();

	switch(m_eState)
	{
	case ELS_WaitInput:
		{
			if( m_nPolicy == 4 )
			{
				LoginResult lrLoginResult = SDOAIgw::Inst()->GetSDOALoginResult();
				if( lrLoginResult.cbSize != 0 && m_bSendSDOAInfo )
				{
					m_bSendSDOAInfo = FALSE;
					m_pClient->WaitThreadLoad();
					Connect();
				}
				else if( lrLoginResult.cbSize == 0 )
					m_bSendSDOAInfo = TRUE;
			}
			if( m_nPolicy == 5 && bSendOnceFlag == FALSE )
			{
				 bSendOnceFlag = TRUE;
				 Connect();
			}
			if( m_nPolicy == 7 && bSendOnceFlag == FALSE )
			{
				bSendOnceFlag = TRUE;
				Connect();
			}
			if (m_nPolicy == 8 && m_b8sendLogin == FALSE && m_bAutoLogin)
			{
				if (!SaveAccount::Inst()->IsGoBack())
				{
					Connect();
					m_b8sendLogin = TRUE;
				}
			}
			if( m_nPolicy == 9 && bSendOnceFlag == FALSE )
			{
				bSendOnceFlag = TRUE;
				Connect();
			}
			if( m_nPolicy == 10 && bSendOnceFlag == FALSE )
			{
				bSendOnceFlag = TRUE;
				Connect();
			}
			if( m_nPolicy == 11 && bSendOnceFlag == FALSE)
			{
				bSendOnceFlag = TRUE;
				Connect();
			}
			// 混服运营
			if( m_nPolicy == 1 &&  SaveAccount::Inst()->GetCropID()&& bSendOnceFlag == FALSE)	
			{
				bSendOnceFlag = TRUE;
				Connect();
			}
			else if ( OPEN_PASSPORT && TRUE == m_bSendUrlLoginMsg && m_nPolicy==1 )
			{
				Connect();
			}
		}
		break;
	case ELS_Connect:
		this->Connect();
		break;
	case ELS_WaitProofResult:
		this->WaitProofResult();
		break;
	case ELS_Queue:
		this->WaitQueue();
		break;
	case ELS_ConnectZoneServer:
		this->TryIsConnectZoneSer();
		break;
	case ELS_SentJoinGame:							//在发送join后,如果与zone服务器断开,则让玩家重新连接
		{
			if(!m_pSession->IsConnect())
				CutOffZone();
			if(timeGetTime() - m_dwSentJoinGame >= 10000)
				CutOffZone();
		}
		break;
	case ELS_JoinGame:
		break;
	case ELS_Launcher:
		{
			if(timeGetTime() - m_dwStateTime >= 1000)
			{
				m_pClient->ExitThreadLoad();
				PostQuitMessage(0);
			}
		}
		break;
	}

	if(!TObjRef<AudioSys>()->MusicIsPlaying())  
	{
		INT nMusic = SaveAccount::Inst()->GetCurMusic() ? 0 : 1;
		SaveAccount::Inst()->SetCurMusic(nMusic);
		TObjRef<AudioSys>()->StopMusic();
		TCHAR szBuff[X_SHORT_NAME] = {0};
		_sntprintf( szBuff, X_SHORT_NAME, _T("music_login%d"), nMusic );
		TObjRef<AudioSys>()->PlayMusic(szBuff, FALSE, SOUND_LOUD);
	}

	float deltaTime = Kernel::Inst()->GetDeltaTime();
	//控制摄像机的变化
 	if( m_bEnter )
 	{
 		CameraEuler* camera = SeleRoleMgr::Inst()->GetCamera();
//  		if( m_LookAt.z < MAXZ )
//  		{
//  			float fYaw= camera->GetYaw();
//  			if( fYaw > 0.0f )
//  			{
//  				m_nBase = -3;
//  				m_bLeft = true;
//  			}
//  			if( fYaw < 0.0f )
//  			{
//  				m_nBase = 3;
//  				m_bRight = true;
//  			}
//  			fYaw += _DegToRad(0.50f * deltaTime)*m_nBase;
//  			if( m_bLeft && m_bRight )
//  				fYaw = 0.0f;
//  			m_LookAt.z += ZSTEP;
//  			SeleRoleMgr::Inst()->SetCamera(m_LookAt, fYaw, CamPitch, CamRoll, MAXCamDist);
//  		}
//  		else
//  		{
//  			m_fMAXTime += deltaTime;
//  			m_fParam += 0.03f;
//  			float fPitch= camera->GetPitch();
//  			fPitch += PITCHSTEP*m_fParam;
//  			if( fPitch >= _DegToRad(75.0f) )
//  			{
//  				fPitch = _DegToRad(75.0f);
//  				m_LookAt.y += YSTEP*m_fParam*2.0f;
//  			}
//  			else
//  				m_LookAt.y += YSTEP*m_fParam *1.2f;
//  			SeleRoleMgr::Inst()->SetCamera(m_LookAt, CamYaw, fPitch, CamRoll, MAXCamDist);
 			m_bRender = true;
 			if( m_nAlpha >= 0 && m_nAlpha <= 0xFF )
 			{
 				m_nAlpha += (INT)ceil(0xFF / ( (MAXY-55000) / YSTEP)) ;
 				if( m_nAlpha > 0xFF || m_fMAXTime > 6.0f )
 				{
 					m_nAlpha = 0xFF;
 					m_pFrameMgr->AddToDestroyList(this);
 					if( m_nPolicy == 4 )
 					{
 						SDOAIgw::Inst()->LeaveLogIn();
 						SDOAIgw::Inst()->SetScreenStatus(SDOA_SCREEN_MINI);
 					}
 					SeleRoleMgr::Inst()->CloseMap();
					if ( m_nPolicy==2 )
						m_bCloseMapOnce = TRUE;
 					SelectRoleFrame* selRoleFrame = (SelectRoleFrame*)m_pFrameMgr->CreateFrame(_T("Root"), _T("SelectRole"), _T("SelectRoleFrame"), 0);
 					//m_pFrameMgr->CreateFrame( _T("Root"), _T("OldPlayerComeBackFrame"), _T("OldPlayerComeBackFrame"), 0 );
 					LoadingFrame* pFrame1 = (LoadingFrame*)m_pFrameMgr->CreateFrame(_T("Root"), _T("Loading"), _T("LoadingFrame"), 0);
 					if( P_VALID(pFrame1) )
 						pFrame1->SetLoadingType(LoadingFrame::ELT_SelectRole);
 				}
 			}
 			m_dwColor = ( m_nAlpha << 24 ) | ( m_dwColor & 0x00FFFFFF );
 		//}
 	}
 	else
 	{
//  		CameraEuler* camera = SeleRoleMgr::Inst()->GetCamera();
//  		float fYaw= camera->GetYaw();
//  		if(fYaw >= MAXYAW)
//  		{
//  			m_nBase = -1;
//  			fYaw=MAXYAW;
//  		}
//  		if(fYaw <= MINYAW)
//  		{
//  			m_nBase = 1;
//  			fYaw=MINYAW;
//  		}
//  		camera->SetYaw(fYaw + _DegToRad(0.50f * deltaTime)*m_nBase );
//  		camera->UpdateViewMatrix();
		if(m_bIsShake)
		{
			if(m_LookAt.x <= 19800)
			{
				m_nBase = 5;
				m_LookAt.x = 19800;
			}
			if(m_LookAt.x >= 21000)
			{
				m_nBase = -5;
				m_LookAt.x = 21000;
			}
			m_LookAt.x += m_nBase;
			SeleRoleMgr::Inst()->SetCamera(m_LookFrom, m_LookAt);
		}

 	}
}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID LoginFrame::SetState(ELoginState eState)
{
	if( m_eState == eState )
		return;

	m_eState = eState;
	m_dwStateTime = timeGetTime();

}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID LoginFrame::SetUIEnable(BOOL bVale)
{
	if(bVale == FALSE)
	{
		m_pButtonLogin->SetEnable(false);
		m_pWndKey->SetInvisible(TRUE);
		m_pPBtn->SetState(EGUIBS_PopUp, FALSE);
		m_pPBtn->SetEnable(false);
	}
	else
	{
		m_pWndKey->SetInvisible(m_pPBtn->GetState()!=EGUIBS_PushDown);
		if( !m_bLoginType )
			m_pButtonLogin->SetEnable(true);
		m_pPBtn->SetEnable(true);
	}
}


//-----------------------------------------------------------------------------
// 处理JoinGame消息
//-----------------------------------------------------------------------------
DWORD LoginFrame::NetRecvJoinGame(tagNS_JoinGame* pMsg, DWORD pPrama)
{
	if( pMsg->dwID != m_pUtil->Crc32("NS_JoinGame") )
		return 0;
	switch(pMsg->dwErrorCode)
	{
	case E_Success:
		{
			m_bEnter = TRUE;				//进入选人界面前,等待动画播放完毕
			m_pWnd->SetInvisible(TRUE);
			if(m_nPolicy == 11)
			{
				m_pWndKoreaPassWord->SetInvisible(TRUE);
			}
			CameraEuler* camera = SeleRoleMgr::Inst()->GetCamera();
			TObjRef<AudioSys>()->Play2DSound(_T("login"));
			//初始化服务器时间同步
			ServerTime::Inst()->Init();
			//初始化反外挂
			HackShield::Inst()->Init();
			HackShield::Inst()->StartApexClient();
			SetState(ELS_JoinGame);

			//保存密码
			SaveLastPassword();

			tstring szWorldName = m_pVarContainer->GetString(_T("gamename"), _T("last_login_server"));
			SaveAccount::Inst()->SetWorldName(szWorldName.c_str());
			//杀掉第三方登陆程序
			HWND hwnd = FindWindow(NULL, g_StrTable[_T("Opidlogin")]);
			if(hwnd != NULL)
			{
				DWORD dwProcld = 0;
				if(GetWindowThreadProcessId(hwnd, &dwProcld))
				{
					HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcld);
					if(hProc != NULL)
					{
						TerminateProcess(hProc, 0);
						CloseHandle(hProc);
					}
				}
			}

			//云联合作，登陆后发送http请求
			SendYunLianHttpPost();
		}
		break;
	case E_JoinGame_AccountOnline:
		m_pStcHint->SetText(g_StrTable[_T("JoinGame_AccountOnline")]);
		break;
	case E_JoinGame_VerifyCodeError:
		m_pStcHint->SetText(g_StrTable[_T("JoinGame_VerifyCodeError")]);
		break;
	}
	if(pMsg->dwErrorCode!=E_Success)
		CutOffZone();
	return 0;
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VOID LoginFrame::CutOffLogin()
{
	m_pStream->Disconnect();
	SetState(ELS_WaitInput);
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VOID LoginFrame::CutOffZone()
{
	SetState(ELS_WaitInput);
	m_pStcHint->SetText( g_StrTable[_T("DisconnectServer")] );
	if( !m_bLoginType)
		m_pButtonLogin->SetEnable(true);
	m_pSession->Disconnect();
}

//-----------------------------------------------------------------------------
// 从注册表读取登陆信息
//-----------------------------------------------------------------------------
VOID LoginFrame::LoadRegistLoginInfo()
{
	/*TCHAR szTemp[X_HUGE_NAME] = {0};
	m_pUtil->ReadReg(_T("Software\ZinOnline\ZinOn_Force"),_T("Param1"),szTemp);
    m_szAccName = szTemp;
	m_pUtil->ReadReg(_T("Software\ZinOnline\ZinOn_Force"),_T("Param2"),szTemp);
	m_szPssword = szTemp;*/
}

//-----------------------------------------------------------------------------
// 从注册表读取当前用户名和密码
//-----------------------------------------------------------------------------
VOID LoginFrame::LoadLastLoginInfo()
{
	TCHAR szTmp[512] = {0};
	m_pUtil->ReadReg(_T("SOFTWARE\\KONG\\SM"), _T("default_save_account"), szTmp);


	if( _tcscmp( szTmp, _T("EGUIBS_PushDown") ) == 0 )
	{
		// 填写默认的设置
		m_pUtil->ReadReg(_T("SOFTWARE\\KONG\\SM"), _T("default_account_name"), szTmp);
		tstring strAccount = szTmp;
		this->FilterString(strAccount);
		m_pEditAccount->SetText(strAccount.c_str());
		m_pPBtnSaveAccount->SetState(EGUIBS_PushDown, false);
	}

	m_bAutoLogin = false;
	if(8 == m_nPolicy)
	{
		m_pUtil->ReadReg(_T("SOFTWARE\\KONG\\SM"), _T("default_save_password"), szTmp);
		if( _tcscmp( szTmp, _T("EGUIBS_PushDown") ) == 0 )
		{
			// 填写默认的设置
			m_pUtil->ReadReg(_T("SOFTWARE\\KONG\\SM"), _T("default_account_password"), szTmp);
			tstring strPassword = szTmp;
			this->FilterString(strPassword);
			strPassword = OutPassWord(strPassword);
			m_bAutoLogin = true;
			m_pEditPassword->SetText(strPassword.c_str());
			m_pPBtnSavePassword->SetState(EGUIBS_PushDown, false);
		}
	}
	//程序专用
#ifdef _DEBUG
	m_pUtil->ReadReg(_T("SOFTWARE\\KONG\\SM"), _T("default_account_name"), szTmp);
	tstring strAccount = szTmp;
	this->FilterString(strAccount);
	m_pEditAccount->SetText(strAccount.c_str());

	m_pUtil->ReadReg(_T("SOFTWARE\\KONG\\SM"), _T("default_password"), szTmp);
	tstring strPassword = szTmp;
	this->FilterString(strPassword);
	m_pEditPassword->SetText(strPassword.c_str());
#endif
}



//-----------------------------------------------------------------------------
// 记录当前用户名和密码到注册表
//-----------------------------------------------------------------------------
VOID LoginFrame::SaveLastLoginInfo()
{
	tstring strAccount = m_pEditAccount->GetText();
	tstring strPassword = m_pEditPassword->GetText();
	strPassword = InPassWord(strPassword);

	this->FilterString(strAccount);
	this->FilterString(strPassword);

	SaveAccount::Inst()->SetAccount(strAccount.c_str());

	if( m_bSave )
	{
		m_pUtil->WriteReg(_T("SOFTWARE\\KONG\\SM"), _T("default_account_name"), strAccount.c_str());
		m_pUtil->WriteReg(_T("SOFTWARE\\KONG\\SM"), _T("default_save_account"), _T("EGUIBS_PushDown"));
	}
	else
	{
		m_pUtil->WriteReg(_T("SOFTWARE\\KONG\\SM"), _T("default_save_account"), _T("EGUIBS_PopUp"));
	}

	//程序专用
#ifdef _DEBUG
	m_pUtil->WriteReg(_T("SOFTWARE\\KONG\\SM"), _T("default_account_name"), strAccount.c_str());
	m_pUtil->WriteReg(_T("SOFTWARE\\KONG\\SM"), _T("default_password"), strPassword.c_str());
#endif

	// 重新存储客户端位置，以便手动更新包等工具使用
	tstring strDir = TObjRef<VarContainer>()->GetString(_T("DIRECTORY"));
	m_pUtil->WriteReg(_T("SOFTWARE\\KONG\\SM"), _T("InstallDir"), strDir.c_str());
}


//-----------------------------------------------------------------------------
// 过滤用户名和密码中的回车换行等无意义字符
//-----------------------------------------------------------------------------
VOID LoginFrame::FilterString(tstring& str)
{
	// 去掉回车
	INT nFind = 0;
	while( (nFind = str.find(_T('\r'), nFind)) != str.npos )
		str.replace(nFind, 1, _T(""));

	// 去掉换行
	nFind = 0;
	while( (nFind = str.find(_T('\n'), nFind)) != str.npos )
		str.replace(nFind, 1, _T(""));

}

void LoginFrame::MBPhaseHint()
{
	switch( m_eMB )
	{
	case EMBP_NULL:
		{
			m_eMB = EMBP_First;
			string strHint(m_strMiBao, m_eMB, 2);
			tstring strHint1 = m_pUtil->AnsiToUnicode(strHint.c_str());
			m_pStcSecuNum->SetText(strHint1.c_str());
			m_pStcInputHint->SetText(_T(""));
			this->RandSetKeyMB();
		}
		break;
	case EMBP_First:
		{
			m_eMB = EMBP_Second;
			string strHint(m_strMiBao, m_eMB, 2);
			tstring strHint1 = m_pUtil->AnsiToUnicode(strHint.c_str());
			m_pStcSecuNum->SetText(strHint1.c_str());
			m_pStcInputHint->SetText(_T(""));
			this->RandSetKeyMB();
		}
		break;
	case EMBP_Second:
		{
			m_eMB = EMBP_Third;
			string strHint(m_strMiBao, m_eMB, 2);
			tstring strHint1 = m_pUtil->AnsiToUnicode(strHint.c_str());
			m_pStcSecuNum->SetText(strHint1.c_str());
			m_pStcInputHint->SetText(_T(""));
			this->RandSetKeyMB();
		}		
		break;
	case EMBP_Third:
		{
			// 发送用户名、密码、密保
			this->SetMBWndShow(FALSE);
			this->SetUIEnable(TRUE);
			m_pButtonLogin->SetEnable(false);
			if( !m_bLoginType && m_nPolicy != 4 )
			{
				if ( !m_pWndCancelQueue->IsInvisible() 
					&& !m_pWndQueue->IsInvisible() )
				{
					m_pWnd->SetInvisible(TRUE);
				}
				else if(m_nPolicy != 11)
				{
					m_pWnd->SetInvisible(FALSE);
				}
			}
			string strCRC = m_pUtil->UnicodeToAnsi(m_strInputMiBao.c_str());
			tagNLC_MiBao msg;
			msg.dwMBCrc = m_pUtil->Crc32(strCRC.c_str());
			m_pStream->Send(&msg, msg.dwSize);
			SetState(ELS_WaitProofResult);
		}
		break;
	case EMBP_End:
		break;
	}
}

void LoginFrame::SetMBWndShow(BOOL bVal)
{
	m_pWndSecurity->SetInvisible(!bVal);
	m_pWndSeInputBack->SetInvisible(!bVal);
}

typedef int(*lpFun) (char*, int);
int g_iRtn = 0;
int XunLeiCallBack( char *szData, int iRet )
{
	g_iRtn = iRet;
	if( iRet == 1 )
	{
		//登录器认证已经成功，szData为100字节的字符串
		TObjRef<Client>()->WaitThreadLoad();
		LoginFrame* pFrame = (LoginFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Login"));
		
		char szEncrypt[100] = {0};
		char szUserName[20] = {0};

		memcpy( szEncrypt, szData, 100 );
		memcpy(szUserName, &szData[100], 20);
		pFrame->SetEncrypt( szEncrypt, szUserName );
		pFrame->Connect();
		return 2;
	}
	else if( iRet == 0 )
	{
		//用户进行退出操作，iRet为0，szData无效
		TObjRef<Client>()->ExitThreadLoad();
		PostQuitMessage(0);
	}

	return 2;
}

void LoginFrame::JudegPolicy()
{
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

	m_nPolicy = _ttoi(policy.c_str());
	switch( m_nPolicy )
	{
	case 0:
	case 1:
	case 6:
	case 8:
		break;
	case 2:
		{
			
			typedef void(*xlFun)(int, DWORD, TCHAR*, lpFun);
			HWND hWnd = (HWND)TObjRef<VarContainer>()->GetDword(_T("HWND"));

			m_hDll = LoadLibrary(_T("XlLogin.dll"));
			if( !P_VALID(m_hDll) )
			{
				MessageBox(NULL, _T("Load XlLogin.dll failed!"), _T(""), MB_OK);
				return;
			}

			ShowWindow(hWnd,SW_HIDE);
			xlFun LoginFun = (xlFun)GetProcAddress(m_hDll, "LoginWnd");
			if( !P_VALID(LoginFun) )
			{
				MessageBox(NULL, _T("Can't find Login()"), NULL, MB_OK);
				FreeLibrary(m_hDll);
				m_hDll = NULL;
				return;
			}
			DWORD dwWorldID = m_pVarContainer->GetDword(_T("world_id"), _T("last_login_server"));

			TCHAR szBuff[X_SHORT_NAME] = {0};
			_sntprintf( szBuff, X_SHORT_NAME, GetXunLeiHint(m_eHint) );

			DWORD dwWindow = GameSet::Inst()->Get(EGS_Windowed);
			
			INT nMode = GetWindowMode(dwWindow);

			LoginFun(nMode, dwWorldID, szBuff, (lpFun)XunLeiCallBack);
	
			if(m_hDll != NULL)
			{
				FreeLibrary(m_hDll);
				m_hDll = NULL;
			}

			ShowWindow(hWnd,SW_NORMAL);
			if( g_iRtn == 2 )
			{
				m_pStcHint->SetText(g_StrTable[_T("ProofResult_XlFlashError")]);
				ShellExecuteXunLei();
			}
			else if( g_iRtn!=1 && g_iRtn!=0 )
			{
				m_pStcHint->SetText(g_StrTable[_T("ProofResult_XlOtherError")]);
				ShellExecuteXunLei();
			}
		}
		break;
	case 3:
		{
			//m_Camera.SetLookAt(Vector3(0,0,-1450), Vector3(0,40,0));
			m_bLoginType = GetCmdLineUserAndPassword();
			if(m_bLoginType)
				m_bLoginType = g_bCommandLine ? false : true;
			if(m_bLoginType)
				HideLoginFrame(m_bLoginType);
		}
		break;
	case 4:
		{
			SDOAIgw::Inst()->SetShowLoginFlag(TRUE);
		}
		break;
	case 5:
		{   
			//LoadRegistLoginInfo();
			if(LoadCMDLoginInfo())
			{
				HideLoginFrame(TRUE);
			}
		}
		break;
	case 7: //OMG
		{   
			if(LoadOMGLoginTWInfo())
			{
				HideLoginFrame(TRUE);
				if ( !m_pWndCancelQueue->IsInvisible() 
					&& !m_pWndQueue->IsInvisible() )
				{
					m_pStcHint->SetInvisible(TRUE);
				}
			}
		}
		break;
	case 9:	//dk mode
		{   
			if(LoadDKLoginInfo())
			{
				HideLoginFrame(TRUE);
				if ( !m_pWndCancelQueue->IsInvisible() 
					&& !m_pWndQueue->IsInvisible() )
				{
					m_pStcHint->SetInvisible(TRUE);
				}
			}
		}
	case 10:	
		{   
			HideLoginFrame(TRUE);
			if ( !m_pWndCancelQueue->IsInvisible() 
				&& !m_pWndQueue->IsInvisible() )
			{
				m_pStcHint->SetInvisible(TRUE);
			}
		}
		break;
	}
}

LPCTSTR LoginFrame::GetXunLeiHint( EXLHint eType )
{
	switch(eType)
	{
	case EXLHint_User:
		return g_StrTable[_T("JoinGame_InputNameOrSN_xl")];
		break;
	case EXLHint_UserError:
		return g_StrTable[_T("ProofResult_Account_No_Match_xl")];
		break;
	case EXLHint_SendMsg:
		return g_StrTable[_T("WaitAuth")];
		break;
	case EXLHint_ConnectFail:
		return g_StrTable[_T("ConnectFail")];
		break;
	case EXLHint_Account_No_Match:
		return g_StrTable[_T("ProofResult_Account_No_Match_xl")];
		break;
	case EXLHint_Account_In_Use:
		return g_StrTable[_T("ProofResult_Account_In_Use_xl")];
		break;
	case EXLHint_Account_Freezed:
		return g_StrTable[_T("ProofResult_Account_Freezed_xl")];
		break;
	case EXLHint_Wrong_Build_Number:
		return g_StrTable[_T("ProofResult_Wrong_Build_Number")];
		break;
	case EXLHint_Disabled:
		return g_StrTable[_T("ProofResult_Disabled_xl")];
		break;
	case EXLHint_NoFindLauncher:
		return g_StrTable[_T("ProofResult_NoFindLauncher")];
		break;
	case EXLHint_Wrong_Type:
		return g_StrTable[_T("ProofResult_Wrong_Type")];
		break;
	case EXLHint_Server_Full:
		return g_StrTable[_T("SelectWorld_Server_Full")];
		break;
	case EXLHint_Server_Suspended:
		return g_StrTable[_T("SelectWorld_Server_Suspended")];
		break;
	case EXLHint_Server_Maintenance:
		return g_StrTable[_T("SelectWorld_Server_Maintenance")];
		break;
	case EXLHint_Failed:
		return g_StrTable[_T("SelectWorld_Failed")];
		break;
	case EXLHint_Short_Time:
		return g_StrTable[_T("SelectWorld_Short_Time")];
		break;
	case EXLHint_No_Select:
		return g_StrTable[_T("SelectWorld_No_Select")];
		break;
	case EXLHint_GameWorldName_Wrong:
		return g_StrTable[_T("SelectWorld_GameWorldName_Wrong")];
		break;;
	case EXLHint_ProofResult_Forbid_MemberCenter:
		return g_StrTable[_T("ProofResult_Forbid_MemberCenter_xl")];
		break;
	case EXLHint_ProofResult_Forbid_GMTool:
		return g_StrTable[_T("ProofResult_Forbid_GMTool_xl")];
		break;
	case EXLHint_ProofResult_Forbid_CellPhone:
		return g_StrTable[_T("ProofResult_Forbid_CellPhone_xl")];
		break;
	case EXLHint_ProofResult_Forbid_MiBao:
		return g_StrTable[_T("ProofResult_Forbid_MiBao_xl")];
		break;
	default:
		return _T("");
		break;
	}
	return _T("");
}

void LoginFrame::SetUserNameUI( const char* szUserName )
{
	tstring strUserName = m_pUtil->AnsiToUnicode(szUserName);
	this->FilterString(strUserName);
	m_pEditAccount->SetText(strUserName.c_str());
	m_pEditPassword->SetText(_T("123"));
}

INT LoginFrame::GetWindowMode( DWORD dwIndex )
{
	switch(dwIndex)
	{
	case 0:	// 全屏
		return 2;
		break;
	case 1:	// 最佳窗口
		return 1;
		break;
	case 2: // 标准窗口
		return 0;
		break;
	case 3:	// 假全屏
		return 2;
		break;
	default:

		break;
	}

	return 0;
}

void LoginFrame::SetEncrypt( const char* szEncry , const char* szUserName )
{
	memcpy( m_szEncrypt, szEncry, 100 );
	memcpy( m_szUserName, szUserName, 20 );
	SetUserNameUI( m_szUserName );
}

void LoginFrame::ShellExecuteXunLei()
{
	TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
	TCHAR szTemp[X_LONG_NAME*2];

	_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
	tstring strAppPath = szTemp;

	tstring strIniFileFullName = strAppPath + szIniFileName;
	::GetPrivateProfileString(_T("Launcher"), _T("XunLeiFAQURL"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
	tstring url = szTemp;
	if( url == _T("") )
	{
		MessageBox(NULL, _T("Don't find FAQ url!"), _T(""), MB_OK);
		return;
	}

	//打开注册网页
	ShellExecute(NULL, _T("open"), url.c_str(), NULL, NULL, SW_MAXIMIZE);
}

void LoginFrame::LetterTab()
{
	if( m_bTab )
	{
		m_bTab = FALSE;
		TCHAR szLetter;
		tstring strName;
		GUIWnd* pPanel = m_pWndKey->GetChild(_T("kbpic"));
		for(INT i=0; i<36; ++i)
		{
			tstringstream stream;
			stream << _T("butt") << i;
			GUIButton* pButton1 = (GUIButton*)pPanel->GetChild(stream.str().c_str());
			if (P_VALID(pButton1))
            {
			    tstring strTemp = pButton1->GetText();
			    szLetter = strTemp.at(0);
			    if( szLetter>='A' && szLetter<='Z' )
			    {
				    szLetter+=32;
				    strTemp = szLetter;
				    pButton1->SetText(strTemp.c_str());
			    }
            }
		}
	}
	else
	{
		m_bTab = TRUE;
		TCHAR szLetter;
		tstring strName;
		GUIWnd* pPanel = m_pWndKey->GetChild(_T("kbpic"));
		for(INT i=0; i<36; ++i)
		{
			tstringstream stream;
			stream << _T("butt") << i;
			GUIButton* pButton1 = (GUIButton*)pPanel->GetChild(stream.str().c_str());
            if (P_VALID(pButton1))
            {
			    tstring strTemp = pButton1->GetText();
			    szLetter = strTemp.at(0);
			    if( szLetter>='a' && szLetter<='z' )
			    {
				    szLetter-=32;
				    strTemp = szLetter;
				    pButton1->SetText(strTemp.c_str());
			    }
            }
		}
	}
	m_pWndKey->SetRefresh(TRUE);
}

bool LoginFrame::GetCmdLineUserAndPassword()
{
	string strCommandLine = m_pUtil->UnicodeToAnsi( GetCommandLine() );
	if( strCommandLine.empty() || strCommandLine.length() > 1024 )
		return false;
	const char *pCommandLine = strstr( strCommandLine.c_str(), ".exe" );
	if( pCommandLine == NULL )
	{
		return false;
	}
	if( !( ( pCommandLine[6] == 'T' || pCommandLine[6] == 't' ) && ( pCommandLine[7] == 'W' || pCommandLine[7] == 'w' ) ) )
	{
		return false;
	}

	char commandLine[1024] = {0};
	char userAndPas[2][64] = {0};
	strcpy( commandLine, pCommandLine + 9 );
	int nNum = 0;
	char *pPart = strtok( commandLine, " " );
	while( pPart && nNum < 2 )
	{
		strcpy( userAndPas[nNum], pPart );
		nNum++;
		pPart = strtok( NULL, " " );
	}

	if( nNum != 2 )
		return false;

	if( P_VALID(m_pEditAccount) )
		m_pEditAccount->SetText( m_pUtil->AnsiToUnicode( userAndPas[0] ) );

	if( P_VALID(m_pEditPassword) )
	{
		m_pEditPassword->SetText(_T("NathanielYu"));
		strcpy( m_szPass, userAndPas[1] );
		//m_pEditPassword->SetText( m_pUtil->AnsiToUnicode( userAndPas[1] ) );
	}
	return true;
}

BOOL LoginFrame::LoadCMDLoginInfo()
{
	string strCMDLine = m_pUtil->UnicodeToAnsi( GetCommandLine() );
	if( strCMDLine.empty() || strCMDLine.length() > 1024 )
		return false;
	const char *pCommandLine = strstr( strCMDLine.c_str(), ".exe" );
	if( pCommandLine == NULL )
	{
		return false;
	}

	char  cmdLine[1024] = {0};
	char  encryptStr[256] = {0};
	char  longthStr[256] = {0};

	strcpy( cmdLine, pCommandLine + 6 );
	int nNum = 0;
	char *pPart = strtok( cmdLine, " " );
	while( pPart && nNum < 2 )
	{
		if ( nNum == 0 )
		{
			strcpy( longthStr, pPart );
			m_szPssword = m_pUtil->AnsiToUnicode(longthStr);
		}
		else if ( nNum == 1 )
		{
			strcpy( encryptStr, pPart );
			m_szEncryptCode =  m_pUtil->AnsiToUnicode(encryptStr);
		}
		nNum++;
		pPart = strtok( NULL, " " );
	}
	m_pStcHint->SetText(g_StrTable[_T("WaitAuth")]);
	return true;
}


BOOL LoginFrame::LoadOMGLoginTWInfo()
{
	string strCMDLine = m_pUtil->UnicodeToAnsi( GetCommandLine() );
	if( strCMDLine.empty() || strCMDLine.length() > 1024 )
		return false;
	const char *pCommandLine = strstr( strCMDLine.c_str(), ".exe" );
	if( pCommandLine == NULL )
	{
		return false;
	}
	if( !( ( pCommandLine[6] == 'T' || pCommandLine[6] == 't' ) && ( pCommandLine[7] == 'W' || pCommandLine[7] == 'w' ) ) )
	{
		return false;
	}

	char  cmdLine[1024] = {0};
	char  encryptStr[256] = {0};
	char  longthStr[256] = {0};

	strcpy( cmdLine, pCommandLine + 9 );
	int nNum = 0;
	char *pPart = strtok( cmdLine, " " );
	while( pPart && nNum < 2 )
	{
		if ( nNum == 0 )
		{
			strcpy( encryptStr, pPart );
			m_szOMGUserName =  m_pUtil->AnsiToUnicode(encryptStr);	
		}
		else if ( nNum == 1 )
		{
			strcpy( longthStr, pPart );
			m_szOMGPassWord = m_pUtil->AnsiToUnicode(longthStr);
		}
		nNum++;
		pPart = strtok( NULL, " " );
	}
	m_pStcHint->SetText(g_StrTable[_T("WaitAuth")]);
	return true;
}


BOOL LoginFrame::LoadDKLoginInfo()
{
	string strCMDLine = m_pUtil->UnicodeToAnsi( GetCommandLine() );
	if( strCMDLine.empty() || strCMDLine.length() > 1024 )
		return false;
	const char *pCommandLine = strstr( strCMDLine.c_str(), ".exe" );
	if( pCommandLine == NULL )
	{
		return false;
	}
	if( !( ( pCommandLine[6] == 'D' || pCommandLine[6] == 'd' ) && ( pCommandLine[7] == 'K' || pCommandLine[7] == 'k' ) ) )
	{
		return false;
	}

	char  cmdLine[1024] = {0};
	char  encryptStr[256] = {0};
	char  longthStr[256] = {0};

	strcpy( cmdLine, pCommandLine + 9 );
	int nNum = 0;
	char *pPart = strtok( cmdLine, " " );
	while( pPart && nNum < 2 )
	{
		if ( nNum == 0 )
		{
			strcpy( encryptStr, pPart );
			m_szOMGUserName =  m_pUtil->AnsiToUnicode(encryptStr);	
		}
		else if ( nNum == 1 )
		{
			strcpy( longthStr, pPart );
			m_szOMGPassWord = m_pUtil->AnsiToUnicode(longthStr);
		}
		nNum++;
		pPart = strtok( NULL, " " );
	}
	m_pStcHint->SetText(g_StrTable[_T("WaitAuth")]);
	return true;
}

BOOL LoginFrame::LoadKoreaWebInfo()
{
	string strCMDLine = m_pUtil->UnicodeToAnsi( GetCommandLine() );
	if( strCMDLine.empty() || strCMDLine.length() > 1024 )
		return false;
	const char *pCommandLine = strstr( strCMDLine.c_str(), ".exe" );
	if( pCommandLine == NULL )
	{
		return false;
	}

	//log
// 	ofstream wFileStream;
// 	wFileStream.open("c:\\sm_KoreaWebLogin.txt",ios::out|ios::app/*|ios::trunc*/);
// 	if (wFileStream)
// 	{
// 		wFileStream <<  strCMDLine <<endl;
// 		wFileStream.close();
// 	}

// 	if( !( ( pCommandLine[6] == 'T' || pCommandLine[6] == 't' ) && ( pCommandLine[7] == 'W' || pCommandLine[7] == 'w' ) ) )
// 	{
// 		return false;
// 	}

	CHAR  cmdLine[1024] = {0};
// 	CHAR  encryptStr[256] = {0};
// 	CHAR  longthStr[256] = {0};
	CHAR  Uid[1024] = {0};

 	strcpy( cmdLine, pCommandLine+5);
// 	int nNum = 0;
 	CHAR *pPart = strtok( cmdLine, " " );
// 	while(pPart && nNum < 3)
// 	{
// 		nNum++;
// 		pPart = strtok(NULL, "-");
// 	}
	strcpy(Uid, pPart);
	m_szKoreaWebUID = m_pUtil->AnsiToUnicode(Uid);


// 	while( pPart && nNum < 2 )
// 	{
// 		if ( nNum == 0 )
// 		{
// 			strcpy( encryptStr, pPart );
// 			m_szOMGUserName =  m_pUtil->AnsiToUnicode(encryptStr);	
// 		}
// 		else if ( nNum == 1 )
// 		{
// 			strcpy( longthStr, pPart );
// 			m_szOMGPassWord = m_pUtil->AnsiToUnicode(longthStr);
// 		}
// 		nNum++;
// 		pPart = strtok( NULL, " " );
// 	}
// 	m_pStcHint->SetText(g_StrTable[_T("WaitAuth")]);
	return true;
}

void LoginFrame::HideLoginFrame( bool bHide )
{
	if ( m_nPolicy == 5 )
	{
		//m_pWndCaption->SetInvisible(bHide);
		m_pEditAccount->SetInvisible(bHide);
		m_pEditPassword->SetInvisible(bHide);		 
		m_pStcSavePass->SetInvisible(bHide);
		m_pPBtn->SetInvisible(bHide);
		m_pPBtn->SetEnable(FALSE);
		m_pButtonLogin->SetInvisible(bHide);
		m_pButtonQuit->SetInvisible(bHide);
	} 
	else if ( m_nPolicy == 7 || m_nPolicy == 9|| m_nPolicy == 10|| ( 1 ==m_nPolicy  && SaveAccount::Inst()->GetCropID()))
	{
		m_pEditAccount->SetInvisible(TRUE);
		m_pEditPassword->SetInvisible(TRUE);		 
		m_pStcSavePass->SetInvisible(TRUE);
		m_pPBtn->SetInvisible(TRUE);
		m_pPBtn->SetEnable(FALSE);
		m_pButtonLogin->SetInvisible(TRUE);
		m_pButtonQuit->SetInvisible(TRUE);
		m_pStcAccountText->SetInvisible(TRUE);
		m_pStcPsswordText->SetInvisible(TRUE);
		m_pStcAccountBack->SetInvisible(TRUE);
		m_pStcPsswordBack->SetInvisible(TRUE);
	}
	else
	{
		if( bHide && P_VALID(m_pWnd) )
		{
			m_pEditAccount->SetInvisible(bHide);
			m_pEditPassword->SetInvisible(bHide);
			//m_pWndPswHint->SetInvisible(bHide);
			m_pStcSavePass->SetInvisible(bHide);
			m_pPBtn->SetInvisible(bHide);
			//m_pStcUser->SetInvisible(bHide);
			m_pButtonLogin->SetEnable(!bHide);
			m_pStcHint->SetText(_T(""));
			if( m_nPolicy == 4 )
			{
				m_pButtonLogin->SetInvisible(bHide);
				m_pButtonQuit->SetInvisible(bHide);
			}
		}
		else if( !bHide && P_VALID(m_pWnd) )
		{
			m_pEditAccount->SetInvisible(bHide);
			m_pEditPassword->SetInvisible(bHide);
			//m_pWndPswHint->SetInvisible(bHide);
			m_pStcSavePass->SetInvisible(bHide);
			m_pPBtn->SetInvisible(bHide);
			//m_pStcUser->SetInvisible(bHide);
		}
	}
	
}

void LoginFrame::LoginGame()
{
	if( 3 == m_nPolicy )
	{
		//m_pClient->WaitThreadLoad();
		Connect();
	}
}

// password 加密
tstring& LoginFrame::InPassWord(tstring& ss)
{
	if(ss.empty()) return ss;

	tstring Incode = _T("ymzmgz");	// sm 开发小组名单 ^_^
	ss += Incode;
	tstring temp1 = ss.substr(0,ss.size()/2);
	tstring temp2 = ss.substr(ss.size()/2,ss.size()-ss.size()/2) ;
	
	tstring temp3 = temp1.substr(0,temp1.size()/2);
	tstring temp4 = temp1.substr(temp1.size()/2,temp1.size()-temp1.size()/2);

	tstring temp5 = temp2.substr(0,temp2.size()/2);
	tstring temp6 = temp2.substr(temp2.size()/2,temp2.size()-temp2.size()/2);

	ss = temp3 + temp6 +temp5 + temp4;

	tstring old =			_T("abcedfghijklmnopqrstuvwxyz1234567890");
	tstring EmCode =	_T("hiwxy91abcmz67dfge4opqrjkl23n50stuv8");
	for (int i=0; i < ss.size();i++)
	{
		int pos = old.find(ss[i]);
		if(pos >=0)
		{
			TCHAR temp = EmCode.at(pos);
			tstring te;
			te.push_back(temp);
			ss.replace(i,1,te);
		}
	}
	return ss;
}
// password	解密
tstring& LoginFrame::OutPassWord(tstring& ss)
{
	if(ss.empty()) return ss;

	tstring temp1 = ss.substr(0,ss.size()-ss.size()/2);
	tstring temp2 = ss.substr(ss.size()-ss.size()/2,ss.size()/2) ;

	tstring temp3 = temp1.substr(0,temp1.size()/2);
	tstring temp4 = temp1.substr(temp1.size()/2,temp1.size()-temp1.size()/2);

	tstring temp5 = temp2.substr(0,temp2.size()/2);
	tstring temp6 = temp2.substr(temp2.size()/2,temp2.size()-temp2.size()/2);

	ss = temp3 + temp6 +temp5 + temp4;
	ss = ss.substr(0,ss.size()-6);

	tstring EmCode	=	_T("abcedfghijklmnopqrstuvwxyz1234567890");
	tstring old =	_T("hiwxy91abcmz67dfge4opqrjkl23n50stuv8");
	for (int i=0; i < ss.size();i++)
	{
		int pos = old.find(ss[i]);
		if(pos >=0)
		{
			TCHAR temp = EmCode.at(pos);
			tstring te;
			te.push_back(temp);
			ss.replace(i,1,te);
		}
	}

	return ss;
}

VOID LoginFrame::SaveLastPassword()
{
	tstring strPassword = m_pEditPassword->GetText();
	strPassword = InPassWord(strPassword);

	this->FilterString(strPassword);

	if(8 == m_nPolicy)
	{
		if( m_bSavePassword )
		{
			m_pUtil->WriteReg(_T("SOFTWARE\\KONG\\SM"), _T("default_account_password"), strPassword.c_str());
			m_pUtil->WriteReg(_T("SOFTWARE\\KONG\\SM"), _T("default_save_password"), _T("EGUIBS_PushDown"));
		}
		else
		{
			m_pUtil->WriteReg(_T("SOFTWARE\\KONG\\SM"), _T("default_save_password"), _T("EGUIBS_PopUp"));
		}
	}
}


DWORD LoginFrame::OnExitLoadingFrame(tagGameEvent* pGameEvent)
{

	if (1 != SaveAccount::Inst()->GetLoginMode() || !g_yyDemo.HaveInited())
		return 0;

	if(m_bShowID)
		g_yyDemo.m_pfnShowOpid(1,1000,10);

	m_bShowID = false;
	return 0;
}

VOID LoginFrame::GetRandomNumber()
{
	map<int, int> tempMap;
	map<int, int>::iterator it;
	int Num = 0;
	int i = 0;
	TCHAR szText[10];
	while( i < 10)
	{
		Num = m_pUtil->Rand()%10;
		it = tempMap.find(Num);
		if(it != tempMap.end())
			continue;
		tempMap.insert(make_pair(Num, i++));
	}
	for( i = 0, it = tempMap.begin(); it != tempMap.end() && i < 10; ++it, i++)
	{
		Num = (*it).second;
		_stprintf(szText, _T("%d"), Num);
		m_pBtnKoreaWordKey[i]->SetText(szText);
	}
}

VOID LoginFrame::KoreaRefrashInput(KoreaInputType eNextState)
{
	//清除上一个状态
	m_pStcFirstPassWordPic->SetInvisible(TRUE);	
	m_pStcSecondPassWordPic->SetInvisible(TRUE);
	m_pStcOldPassWordPic->SetInvisible(TRUE);		
	m_pStcNewPassWordPic->SetInvisible(TRUE);		
	m_pStcNewPassWordConfirmPic->SetInvisible(TRUE);

	//刷新下一个状态
	switch(eNextState)
	{
	case EKIT_SetPassWordOne:			//设置密码第一个
		{
			m_pStcFirstPassWordPic->SetInvisible(FALSE);
		}
		break;
	case EKIT_SetPassWordTwo:			//设置密码第二个
		{
			m_pStcSecondPassWordPic->SetInvisible(FALSE);
		}
		break;
	case EKIT_ChangePassWordOld:		//修改密码，旧密码
		{
			m_pStcOldPassWordPic->SetInvisible(FALSE);
		}
		break;
	case EKIT_ChangePassWordOne:		//修改密码，第一次输入
		{
			m_pStcNewPassWordPic->SetInvisible(FALSE);
		}
		break;
	case EKIT_ChangePassWordTwo:		//修改密码，第二次输入
		{
			m_pStcNewPassWordConfirmPic->SetInvisible(FALSE);
		}
		break;
	}
	m_eKoreaInputType = eNextState;
}

VOID LoginFrame::KoreaClearPassWordInfo()
{
	m_strKoreaLoginNumber = "";	
	m_strKoreaSetPassWordOne = "";
	m_strKoreaSetPassWordTwo = "";
	m_strKoreaChangePassWordOne = "";
	m_strKoreaChangePassWordTwo = "";
	m_strKoreaChangePassWordOld = "";
	m_pStcFirstPassWord->SetText(_T(""));
	m_pStcSecondPassWord->SetText(_T(""));
	m_pStcLoginPassWord->SetText(_T(""));
	m_pStcOldPassWord->SetText(_T(""));
	m_pStcNewPassWord->SetText(_T(""));
	m_pStcNewPassWordConfirm->SetText(_T(""));
	m_pWndKoreaPassWord->SetRefresh(TRUE);
}

VOID LoginFrame::KoreaChangeUI(EKoreaChangeUI eType)
{
	switch(eType)
	{
	case EKCU_SetPassWord:
		{
			m_pWndFirstSetPassWord->SetInvisible(FALSE);
			m_pWndKoreaLogin->SetInvisible(TRUE);
			m_pWndKoreaChangePassWord->SetInvisible(TRUE);
			m_pBtnKoreaLoginOK->SetText(g_StrTable[_T("KoreaOKText")]);
			m_pBtnKoreaChangeWorld->SetInvisible(TRUE);
			GetRandomNumber();
			KoreaClearPassWordInfo();
		}
		break;
	case EKCU_Login:	//显示登录界面
		{
			m_pWndFirstSetPassWord->SetInvisible(TRUE);
			m_pWndKoreaLogin->SetInvisible(FALSE);
			m_pWndKoreaChangePassWord->SetInvisible(TRUE);
			m_pBtnKoreaChangeWorld->SetInvisible(FALSE);
			m_pBtnKoreaLoginOK->SetText(g_StrTable[_T("KoreaLoginText")]);
			GetRandomNumber();
			KoreaClearPassWordInfo();
		}
		break;
	case EKCU_ChangePassWord:
		{
			m_pWndFirstSetPassWord->SetInvisible(TRUE);
			m_pWndKoreaLogin->SetInvisible(TRUE);
			m_pWndKoreaChangePassWord->SetInvisible(FALSE);
			m_pBtnKoreaChangeWorld->SetInvisible(TRUE);
			m_pBtnKoreaLoginOK->SetText(g_StrTable[_T("KoreaOKText")]);
			GetRandomNumber();
			KoreaClearPassWordInfo();
		}
		break;
	}
}

DWORD LoginFrame::OnMsgbox_KoreaLoginErr3Times(tagMsgBoxEvent* pGameEvent)
{
	if(pGameEvent->eResult == MBF_OK)
	{
		//输入错误三次后退出游戏
		m_pClient->ExitThreadLoad();
		PostQuitMessage(0);
	}
	return 0;
}

VOID LoginFrame::RefreshUrlPic()
{
	if ( OPEN_PASSPORT && m_nPolicy==1 )
	{
		for ( int i=0 ; i<g_nUrlNum ; ++i )
		{
			m_pStcUrlPic[i]->SetInvisible(TRUE);
			m_pStcOpenPassName[i]->SetText(_T(""));
		}
		int nMax = 0;
		int nStart =  m_nCurUrlPage*g_nUrlNum;
		int nEnd  =  (m_nCurUrlPage+1)*g_nUrlNum;
		std::vector<DWORD> vecCropInfo = SaveAccount::Inst()->GetCropIDInfo();
		if(m_dwCropID != 0)	//三方登陆
		{
			int i = 0;
			for(m_itOpenPassPortInfo = m_OpenPassportInfo.begin(), i = 0; m_itOpenPassPortInfo != m_OpenPassportInfo.end(); ++m_itOpenPassPortInfo, i++)
			{
				if((*m_itOpenPassPortInfo).dwID == m_dwCropID)
				{
					m_byOpenPassPotIndex = i;
				}
			}
			TCHAR szText[128] = {0};
			_stprintf(szText, _T("%s%s"), m_OpenPassportInfo[m_byOpenPassPotIndex].szDesc.c_str(), g_StrTable[_T("LoginText")]);
			HideNormalLoginUI(TRUE);
			m_pButtonLogin->SetText(szText);
			//m_pHttpPicCurr->SetInvisible(FALSE);
			m_pOpenPassportPath->SetInvisible(TRUE);
			m_pBtnNormal->SetInvisible(FALSE);
		}
		else
		{
			m_pButtonLogin->SetText(g_StrTable[_T("LoginText")]);
			HideNormalLoginUI(FALSE);
			//m_pHttpPicCurr->SetInvisible(TRUE);
			if(!SaveAccount::Inst()->GetYunLianOn())
				m_pOpenPassportPath->SetInvisible(FALSE);
			m_pBtnNormal->SetInvisible(TRUE);
		}


		if (vecCropInfo.empty())
		{
			nMax = int (m_OpenPassportInfo.size());
			for ( int i=nStart ; (i<nMax)&&(i<nEnd); ++i )
			{
				int index = i%g_nUrlNum;
				tagURLInfo info = m_OpenPassportInfo[i];
				tstring str = m_pUtil->AnsiToUnicode(info.strUrl.c_str());
				if (str.empty())
					continue;
				m_pStcUrlPic[index]->SetInvisible(FALSE);

				std::vector<tstring> vecFileNames;
				m_pUtil->StringToToken(vecFileNames, str.c_str(), _T('/'));
				if( !vecFileNames.empty() )
				{
					int nEndIndex = int(vecFileNames.size());
					tstring  tsName = vecFileNames[nEndIndex-1].c_str();
					tstringstream tssPath;
					tssPath<<_T("temp\\")<<tsName;
					m_pStcUrlPic[index]->SetDefaultPic( tssPath.str().c_str() );
					m_pStcUrlPic[index]->SetRefresh(TRUE);
				}
				m_pStcOpenPassName[index]->SetText(info.szDesc.c_str());
			}

// 			//加载下面那个
// 			if(!m_OpenPassportInfo.empty() && m_byOpenPassPotIndex < m_OpenPassportInfo.size() && m_dwCropID != 0)
// 			{
// 				tagURLInfo info = m_OpenPassportInfo[m_byOpenPassPotIndex];
// 				tstring str = m_pUtil->AnsiToUnicode(info.strUrl.c_str());
// 				if (!str.empty())
// 				{
// 					//m_pHttpPicCurr->SetInvisible(FALSE);
// 
// 					std::vector<tstring> vecFileNames;
// 					m_pUtil->StringToToken(vecFileNames, str.c_str(), _T('/'));
// 					if( !vecFileNames.empty() )
// 					{
// 						int nEndIndex = int(vecFileNames.size());
// 						tstring  tsName = vecFileNames[nEndIndex-1].c_str();
// 						tstringstream tssPath;
// 						tssPath<<_T("temp\\")<<tsName;
// 						m_pHttpPicCurr->SetDefaultPic( tssPath.str().c_str() );
// 						m_pHttpPicCurr->SetRefresh(TRUE);
// 					}
// 				}
// 			}
			
			

		} // if (vecCropInfo.empty())
		else 
		{
			//m_pHttpPicCurr->SetInvisible(TRUE);
			nMax = int (vecCropInfo.size());
			if(nMax == 0)
			{
				m_pOpenPassportPath->SetInvisible(TRUE);
			}
			else
			{
				m_pOpenPassportPath->SetInvisible(FALSE);
			}
			for ( int i=nStart ; (i<nMax)&&(i<nEnd); ++i )
			{
				DWORD dwID = vecCropInfo[i];
				int index = i%g_nUrlNum;
				for ( int j=0; j< int(m_OpenPassportInfo.size()); ++j )
				{
					tagURLInfo info = m_OpenPassportInfo[j];
					if ( info.dwID == dwID )
					{
						tstring str = m_pUtil->AnsiToUnicode(info.strUrl.c_str());
						if (str.empty())
							continue;
						m_pStcUrlPic[index]->SetInvisible(FALSE);
						std::vector<tstring> vecFileNames;
						m_pUtil->StringToToken(vecFileNames, str.c_str(), _T('/'));
						if( !vecFileNames.empty() )
						{
							int nEndIndex = int(vecFileNames.size());
							tstring  tsName = vecFileNames[nEndIndex-1].c_str();
							tstringstream tssPath;
							tssPath<<_T("temp\\")<<tsName;
							m_pStcUrlPic[index]->SetDefaultPic( tssPath.str().c_str() );
							m_pStcUrlPic[index]->SetRefresh(TRUE);
						}
					}
				}
			}
		}
		if (nMax>g_nUrlNum)
		{
			m_pBtnPrev->SetInvisible(FALSE);
			m_pBtnNext->SetInvisible(FALSE);
		}

	}
	
}

VOID LoginFrame::OpenPassportUpdate()
{
	if ( OPEN_PASSPORT && m_bOpenPassport && m_nPolicy==1 && SaveAccount::Inst()->IsYYLoginMode()==FALSE )
	{
		Connect(FALSE);
		//Data
		LPBYTE pMsg = NULL;
		DWORD dwMsgSize = 0;
		pMsg = m_pStream->Recv(dwMsgSize);
		if( P_VALID(pMsg) )
		{
			DWORD dwCropID = GT_INVALID;
			if ( m_nCurChoose>=0 && m_nCurChoose<int(m_OpenPassportInfo.size()) )
			{
				tagURLInfo info = m_OpenPassportInfo[m_nCurChoose];
				dwCropID = info.dwID;
			}
			tagNLS_WebLoginProof* pRecv = (tagNLS_WebLoginProof*)pMsg;
			if( pRecv->dwID == m_pUtil->Crc32("NLS_WebLoginProof") && dwCropID != GT_INVALID )
			{
				stringstream ts;
				ts<<pRecv->szLoginURL;
				ts<<"token="<<DWORD(pRecv->dwTokenID);
				ts<<"&loginid="<<DWORD(pRecv->dwLoginID);
				ts<<"&cropid="<<dwCropID;
				ts<<"&sign="<<pRecv->szSign;

				stringstream ss;
				ss<<"客户端打开url地址为："<< ts.str();
				OpenPassportLog(ss.str());

				tstring szUrl = m_pUtil->AnsiToUnicode( ts.str().c_str() );
				//ShellExecute(NULL, _T("open"), szUrl.c_str(), NULL, NULL, SW_MAXIMIZE);
				ShellExecute(NULL, _T("open"), _T("client\\OpenIDLogin.exe"), szUrl.c_str(), NULL, SW_SHOW);
				CHAR	pUrl[X_LONG_NAME];
				int nSize = pRecv->dwURLSize;
				strncpy( pUrl, pRecv->szURL , nSize ); 
				pUrl[pRecv->dwURLSize-1] = '\0';
				string str( pUrl );
				SaveAccount::Inst()->SetChargeUrl(str);
				SaveAccount::Inst()->SetCropID(dwCropID);
				m_pStream->FreeRecved(pMsg);
				//接收完若不重新点申请url键钮，则不进入该函数，交给之前的消息接受流
				m_bOpenPassport = FALSE; 
				
			}

			tagNLS_ProofResult* pRecv1 = (tagNLS_ProofResult*)pMsg;
			if( pRecv1->dwID == m_pUtil->Crc32("NLS_ProofResult") )
			{
				if( pRecv1->dwErrorCode == E_WebLoginMode_GetURL )
				{
					/*解析假登陆 
					不定长储存格式是: 节点大小(DWORD) + 节点内容(tagWeb_URLInfo) */
					for ( int i=0, pos = 0; i<int(pRecv1->dwURLNum); ++i )
					{

						DWORD dwStrutSize =  *( (DWORD*)(pRecv1->byData+pos) );
						tagWeb_URLInfo* pInfo = (tagWeb_URLInfo*)(pRecv1->byData +pos +sizeof(DWORD) );
						if ( int(pInfo->dwSize) > X_LONG_NAME )
							continue;
						CHAR	pUrl[X_LONG_NAME];
						int nSize = pInfo->dwSize;
						strncpy( pUrl, pInfo->szURL , nSize ); 
						pUrl[pInfo->dwSize-1] = '\0';
						string str( pUrl );
						tagURLInfo saveInfo;
						saveInfo.dwID = pInfo->dwCropID;
						saveInfo.strUrl = str;
						saveInfo.szDesc = pInfo->szDescName;
						m_OpenPassportInfo.push_back(saveInfo);

						pos += dwStrutSize + sizeof(dwStrutSize);

						stringstream ss;
						ss<<"客户端已收到第"<<i+1<<"个图片下载地址:"<<str.c_str();
						OpenPassportLog(ss.str());

					}		
					m_pStream->FreeRecved(pMsg);
				} 
			}
		}   // P_VALID(pMsg) 

		DownloadFunction();

		//下载检测， 向loading界面发消息
		m_UpdateUrlTime += Kernel::Inst()->GetDeltaTimeDW();
		if( m_UpdateUrlTime > 100 ) 
		{
			m_UpdateUrlTime = 0;

			if ( !g_bSendDoneMsg && IsFinishAllDownload() ) 
			{
				m_pMgr->SendEvent( &tagGameEvent( _T("Finish_All_Download"), this ) );
				g_bSendDoneMsg = TRUE;
				RefreshUrlPic();
			}
		}
		//重建下载
		m_dwDownLoadTime += Kernel::Inst()->GetDeltaTimeDW();
		if( m_dwDownLoadTime > 60000 ) 
		{
			m_dwDownLoadTime = 0;
			if ( !IsFinishAllDownload() ) 
				RedownloadFunction();

		}
	}//  if ( OPEN_PASSPORT && m_bOpenPassport )

}

VOID LoginFrame::SendUnrealLoginInfo()
{
	if (OPEN_PASSPORT&& m_nPolicy==1)
	{
		INT size = sizeof(tagNLC_SafeProof);
		BYTE* pMsg = new BYTE[size];
		ZeroMemory( pMsg, size );
		tagNLC_SafeProof* pCmd = (tagNLC_SafeProof*)pMsg;
		pCmd->dwID = pCmd->Crc32("NLC_SafeProof");

		pCmd->dwReserved = GT_INVALID;
		pCmd->dwSize = size;

		pCmd->dwType = m_nPolicy;

		tstring szWorldName = m_pVarContainer->GetString(_T("gamename"), _T("last_login_server"));
		pCmd->dwWorldNameCrc = m_pUtil->Crc32(szWorldName.c_str());

		TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
		TCHAR szTemp[X_LONG_NAME*2];
		_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
		tstring strAppPath = szTemp;
		tstring strIniFileFullName = strAppPath + szIniFileName;
		::GetPrivateProfileString(_T("Launcher"), _T("LocalGame"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
		tstring version = szTemp;
		pCmd->dwCurVersionID =  m_pUtil->Crc32(version.c_str());

		pCmd->dwLoginMode = ELM_Web_LoginMode;
		pCmd->bIsGetTokenID = false;
		pCmd->dwChannelID = GT_INVALID;
		pCmd->bIsGetURL	= true;
		DWORD dwKeyResult;
		ProofKey(dwKeyResult, pCmd);
		pCmd->dwProofKey     = dwKeyResult;
		pCmd->dwConnID = GT_INVALID;
		m_pStream->Send(pCmd,size);

		SAFE_DEL_ARRAY(pMsg);

		OpenPassportLog("客户端向服务器发送假登陆消息获取图片下载地址。");

	}
}

VOID LoginFrame::SendChooseUrlMsg(INT index)
{
	if (OPEN_PASSPORT&& m_nPolicy==1)
	{
		int nMax = int (m_OpenPassportInfo.size());
		if ( index< nMax && index != GT_INVALID )
		{
			tagURLInfo info = m_OpenPassportInfo[index];

			INT size = sizeof(tagNLC_SafeProof);
			BYTE* pMsg = new BYTE[size];
			ZeroMemory( pMsg, size );
			tagNLC_SafeProof* pCmd = (tagNLC_SafeProof*)pMsg;
			pCmd->dwID = pCmd->Crc32("NLC_SafeProof");
			pCmd->dwReserved = GT_INVALID;
			pCmd->dwSize = size;
			pCmd->dwType = m_nPolicy;

			tstring szWorldName = m_pVarContainer->GetString(_T("gamename"), _T("last_login_server"));
			pCmd->dwWorldNameCrc = m_pUtil->Crc32(szWorldName.c_str());

			TCHAR szIniFileName[X_LONG_NAME] = _T("launcher\\launcher.ini");
			TCHAR szTemp[X_LONG_NAME*2];
			_tcscpy( szTemp, Kernel::Inst()->GetWorkPath() );
			tstring strAppPath = szTemp;
			tstring strIniFileFullName = strAppPath + szIniFileName;
			::GetPrivateProfileString(_T("Launcher"), _T("LocalGame"), NULL,  szTemp, X_LONG_NAME*2, strIniFileFullName.c_str());
			tstring version = szTemp;
			pCmd->dwCurVersionID =  m_pUtil->Crc32(version.c_str());

			pCmd->dwLoginMode = ELM_Web_LoginMode;
			pCmd->bIsGetTokenID = false;
			pCmd->dwChannelID = info.dwID;
			pCmd->bIsGetURL	= false;
			DWORD dwKeyResult;
			ProofKey(dwKeyResult, pCmd);
			pCmd->dwProofKey     = dwKeyResult;
			pCmd->dwConnID = GT_INVALID;
			m_pStream->Send(pCmd,size);

			SAFE_DEL_ARRAY(pMsg);
			SetState(ELS_WaitProofResult);
			m_bSendUrlLoginMsg = FALSE; //表明传输完毕
			OpenPassportLog("客户端向login发送登陆消息，以及打开open passport相关网页");
			SaveAccount::Inst()->SetOpenPassport(TRUE);
			m_bOpenWaitFlag = TRUE;
		}
		else
		{
			OpenPassportLog("所选open passport相关网页链接不存在" );
		}
	}

}

VOID   LoginFrame::UrlModeHideWedget( BOOL bHide )
{
	if (OPEN_PASSPORT&& m_nPolicy==1)
	{
		m_pEditAccount->SetInvisible(bHide);
		m_pEditPassword->SetInvisible(bHide);		 
		m_pStcSavePass->SetInvisible(bHide);
		//m_pPBtn->SetInvisible(bHide);
		//m_pButtonLogin->SetInvisible(bHide);
		//m_pButtonQuit->SetInvisible(bHide);
		//m_pBtnReturn->SetInvisible(bHide);
		//m_pBtnNormal->SetInvisible( !bHide );
		m_pStcAccountBack->SetInvisible(bHide);
		m_pStcPsswordBack->SetInvisible(bHide);
		m_pStcAccountText->SetInvisible(bHide);
		m_pStcPsswordText->SetInvisible(bHide);
		m_pStcLoginPic->SetInvisible(bHide);
	}
}

VOID LoginFrame::OpenPassportLog(string sLogInfo)
{
	if (OPEN_PASSPORT_LOG)
	{
		ofstream wFileStream;
		tstring strPath = _T("f:\\OpenPassportLogMsg.txt");
		string szPath = m_pUtil->UnicodeToAnsi(strPath.c_str());
		wFileStream.open(szPath.c_str(),ios::out|ios::app);
		if (wFileStream)
		{
			wFileStream <<  sLogInfo <<endl;
			wFileStream.close();
		}
	}
}

VOID LoginFrame::DownloadFunction()
{
	if (OPEN_PASSPORT&& m_nPolicy==1)
	{
		int nMax = int (m_OpenPassportInfo.size());
		// 新建下载
		for ( int i=0 ; i<nMax; ++i )
		{
			tagURLInfo info = m_OpenPassportInfo[i];
			tstring strUrl = m_pUtil->AnsiToUnicode(info.strUrl.c_str());
			if ( strUrl.empty() || P_VALID(info.pHttp) )
				continue;
			NewHttpDownload( strUrl, i );
		}
	}
	
}

VOID LoginFrame::RedownloadFunction()
{
	if (OPEN_PASSPORT&& m_nPolicy==1)
	{
		int nMax = int (m_OpenPassportInfo.size());
		// 新建下载
		for ( int i=0 ; i<nMax; ++i )
		{
			tagURLInfo info = m_OpenPassportInfo[i];
			tstring strUrl = m_pUtil->AnsiToUnicode(info.strUrl.c_str());
			if (strUrl.empty() )
				continue;

			if ( NULL == info.pHttp || FALSE == (info.pHttp)->GetResult() )
				NewHttpDownload( strUrl, i );
		}
	}
	
}

BOOL LoginFrame::NewHttpDownload( tstring tsPath, INT nIndex)
{
	// 设置新的url
	int nMax = int (m_OpenPassportInfo.size());
	if (tsPath.empty() || nIndex>=nMax)
		return FALSE;
	string szUrl = m_pUtil->UnicodeToAnsi(tsPath.c_str());
	HttpDownload* tmpHttp = m_OpenPassportInfo[nIndex].pHttp;
	// 新建下载
	if ( NULL == tmpHttp )
	{
		HttpDownload* pHttpDL = new HttpDownload();
		pHttpDL->Download(tsPath.c_str());
		m_OpenPassportInfo[nIndex].pHttp = pHttpDL;
		if (!szUrl.empty())
		{
			stringstream ss;
			ss<<"客户端启动线程下载图片:"<<szUrl;
			OpenPassportLog( ss.str().c_str() );
		}
		m_dwDownLoadTime = 0;
	}		
	// 重建下载
	else
	{
		(m_OpenPassportInfo[nIndex].pHttp)->Terminate();
		SAFE_DEL(m_OpenPassportInfo[nIndex].pHttp);
		HttpDownload* pHttpDL = new HttpDownload();
		pHttpDL->Download(tsPath.c_str());
		m_OpenPassportInfo[nIndex].pHttp = pHttpDL;
		if (!szUrl.empty())
		{
			stringstream ss;
			ss<<"客户端重建线程下载图片:"<<szUrl;
			OpenPassportLog( ss.str().c_str() );
		}
		m_dwDownLoadTime = 0;
	}

	return TRUE;
}

BOOL LoginFrame::NewHttpDownloadPic( tstring tsPath, INT nIndex)
{
	TCHAR szWndName[X_LONG_NAME]={0};
	_stprintf(szWndName, _T("%s%d"), _T("\\desktop\\httpdownloadpic"), nIndex );

	HttpPicEx *pHttpEx = (HttpPicEx*)TObjRef<GUISystem>()->GetWnd(szWndName);
	if( P_VALID(pHttpEx) )
		return TRUE;

	char szName[X_LONG_NAME]= {0};
	sprintf(szName, "%s%d", "\\desktop\\httpdownloadpic", nIndex);
	XmlElement ele;
	ele.SetAttribute("Name", szName);
	ele.SetAttribute("ClassName", "HttpPicEx");
	ele.SetAttribute("Size_x", 32);
	ele.SetAttribute("Size_y", 32);
	ele.SetAttribute("AlignOffset_x", 800);
	ele.SetAttribute("AlignOffset_y", 600);
	ele.SetAttribute("Font", TObjRef<Util>()->UnicodeToUnicode8(g_StrTable[_T("Font_MSYH")]));
	ele.SetAttribute("FontWidth", 0);
	ele.SetAttribute("FontHeight", 18);
	ele.SetAttribute("TextRect_left", 19);
	ele.SetAttribute("TextRect_top", 19);		
	ele.SetAttribute("TextAlign", "LeftTop");
	ele.SetAttribute("AlphaWrite", EGUIAW_NoWrite);
	ele.SetAttribute("TextColor", 0xFFFFFFFF);
	ele.SetAttribute("ShadowColor", 0xFF290404);
	ele.SetAttribute("PicColor", 0xEEFFFFFF);
	ele.SetAttribute("Resizable", "yes");
	ele.SetAttribute("CanMoveOutOfFather", "yes");
	ele.SetAttribute("ReadOnly", "yes");
	ele.SetAttribute("AlphaWrite", EGUIAW_Write);
	ele.SetAttribute("Movable", "yes");
	ele.SetAttribute("TopMost", "yes");
	pHttpEx = (HttpPicEx*)m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	pHttpEx->SetInvisible(TRUE);
	if( !P_VALID(pHttpEx) )
		return FALSE;

	m_vecHttpPic.push_back(pHttpEx);
	pHttpEx->SetPicURL(tsPath.c_str());
	return TRUE;
}

BOOL LoginFrame::IsFinishAllDownload( )
{
	if (OPEN_PASSPORT&& m_nPolicy==1)
	{
		if ( m_OpenPassportInfo.empty())
			return FALSE;	
		for ( int i =0; i< int(m_OpenPassportInfo.size()); ++i )
		{	
			HttpDownload*  pHttp = m_OpenPassportInfo[i].pHttp;
			if ( !P_VALID(pHttp) ||  FALSE == pHttp->GetResult() )
				return FALSE;	
		}
	}
	
	return TRUE;
}

DWORD  LoginFrame::OnFinishCloseLoading(tagGameEvent* pGameEvent)
{
	if (P_VALID(m_pWnd))
		RefreshUrlPic();
	return 0;
}

VOID LoginFrame::HideNormalLoginUI(BOOL bIsHide)	//显示隐藏常规登陆相关选项
{
	m_pStcLoginPic->SetInvisible(bIsHide);
	m_pPBtn->SetInvisible(bIsHide);
}

VOID LoginFrame::SetKoreaTimaOutError()
{
	if(!m_pWndKoreaLogin->IsInvisible())
	{
		m_pStcKoreaLoginTip->SetText(g_StrTable[_T("KoreaLogin_TimeOut")]);
		m_pStcKoreaLoginTip->SetRefresh(TRUE);
	}
	else if(!m_pWndFirstSetPassWord->IsInvisible())
	{
		m_pStcKoreaFTip->SetText(g_StrTable[_T("KoreaLogin_TimeOut")]);
		m_pStcKoreaFTip->SetRefresh(TRUE);
	}
	else if(!m_pWndKoreaChangePassWord->IsInvisible())
	{
		m_pStcChangeWordTip->SetText(g_StrTable[_T("KoreaLogin_TimeOut")]);
		m_pStcChangeWordTip->SetRefresh(TRUE);
	}
}

void LoginFrame::SendYunLianHttpPost()
{
	if(SaveAccount::Inst()->GetYunLianOn())
	{
		SendHttpPost sendHttpPost;
		stringstream strHttp;
		strHttp<< "/index.php?directive=Login|" << m_pUtil->UnicodeToAnsi(m_pEditAccount->GetText()) << "|";
		string strUserMacMD5;
		strUserMacMD5 += m_pUtil->UnicodeToAnsi(m_pEditAccount->GetText());
		strUserMacMD5 += "kzlieling";
		BYTE digest[16];
		m_pUtil->MD5ForString( (char*)strUserMacMD5.c_str(), digest );
		string strPswMD5;
		m_pUtil->MD5ToString( strPswMD5, digest );
		strHttp << strPswMD5;
		//HttpRequest("translate.google.com",80,"/translate_t?langpair=en|zh-CN","POST","hl=zh-CN&ie=UTF-8&text=this is me&langpair=en|zh-CN",strlen("hl=zh-CN&ie=UTF-8&text=this is me&langpair=en|zh-CN"));
		LPCWSTR FinalStr;
		FinalStr = m_pUtil->AnsiToUnicode(strHttp.str().c_str());
		ShellExecute(NULL, _T("open"), _T("client\\YunLianSend.exe"), FinalStr, NULL, SW_SHOW);
		//sendHttpPost.HttpRequest(_T("game.yxyun.com"), 80, strHttp.str().c_str(), "POST", "hl=zh-CN&ie=UTF-8&text=this is me&langpair=en|zh-CN",strlen("hl=zh-CN&ie=UTF-8&text=this is me&langpair=en|zh-CN"));
	}
	


// 	ofstream wFileStream;
// 	wFileStream.open("c:\\yunlianlog.txt",ios::out|ios::app/*|ios::trunc*/);
// 	if (wFileStream)
// 	{
// 		wFileStream <<  strHttp.str().c_str() <<endl;
// 		wFileStream.close();
// 	}
}

void LoginFrame::CloseOtherLogin()
{
	m_pOpenPassportPath->SetInvisible(TRUE);

}