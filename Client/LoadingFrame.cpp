//-----------------------------------------------------------------------------
//!\file LoadingFrame.cpp
//!\author Lyp
//!
//!\date 2008-02-17
//! last 2008-04-15
//!
//!\brief loading frame
//!
//!	Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "LoadingFrame.h"

#include "MapMgr.h"
#include "SeleRoleMgr.h"
#include "ClientMain.h"
#include "CircleProgress.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "CombatSysUtil.h"
#include "GUIEffectMgr.h"
//-----------------------------------------------------------------------------
//! construction
//-----------------------------------------------------------------------------
LoadingFrame::LoadingFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndText = NULL;
	m_pProgress = NULL;
	m_dwCurrent = 0;
	m_dwQuitTime = GT_INVALID;
	m_eLoadingType = ELT_World;
	m_bLoginDownloadRes = TRUE;
}


//-----------------------------------------------------------------------------
//! destruction
//-----------------------------------------------------------------------------
LoadingFrame::~LoadingFrame()
{
	
}


//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL LoadingFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
    int nNum = 4;
    tstring strPathLoading = _T("data\\ui\\loading\\num.xml");
    list<tstring> numList;
    TObjRef<VarContainer> VarNumTable = CreateObj("NumTable", "VarContainer");
    if (VarNumTable->Load("VFS_System", strPathLoading.c_str(), "id", &numList))
    {
        for (list<tstring>::iterator it = numList.begin();
            it != numList.end(); ++it)
        {
            nNum = VarNumTable->GetInt(_T("value"), (*it).c_str(), 0);
        }
    }

    KillObj("NumTable");

	TCHAR szBackFileName[255];
	_stprintf( szBackFileName, _T("data\\ui\\loading\\L_loading-diban%02d.jpg"), m_pUtil->RandomInRange(1,nNum) );
	m_strBackPicFileName = szBackFileName;
	if( dwParam )
		m_strBackPicFileName.assign((LPCTSTR)dwParam);

	tstring strPath = g_strLocalPath + _T("\\tip\\loading_tips.xml");
	if (P_VALID(GetObj("LoadingTips")))
		KillObj("LoadingTips");

	m_pTips = CreateObj("LoadingTips", "VarContainer");
	if( !m_pTips )
		return FALSE;

	if( FALSE == m_pTips->Load("VFS_System", strPath.c_str()) )
		return FALSE;


	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	// 屏蔽F10功能
	m_pGUI->GetDesktop()->SetInvisible(false);	// 显示界面
	m_pInput->SetAllEnable(FALSE);

	m_pFrameMgr->SendEvent( &tagGameEvent( _T("tagEnterLoadingFrame"), this ) );

	CombatSysUtil::Inst()->HideErrorCodeMsg(true);

	m_pMgr->RegisterEventHandle( _T("Finish_All_Download"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&LoadingFrame::OnFinishAllDownload));

	m_bLoginDownloadRes = TRUE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL LoadingFrame::Destroy()
{
	GameFrame::Destroy();

	m_pWndText = NULL;
	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);
	if (P_VALID(GetObj("LoadingTips")))
		KillObj("LoadingTips");

	m_pInput->SetAllEnable(TRUE);

	m_pFrameMgr->SendEvent( &tagGameEvent( _T("tagExitLoadingFrame"), this ) );

	LocalPlayer* plp =  RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(plp))
	{
		plp->KeepMoving();
	}

	CombatSysUtil::Inst()->HideErrorCodeMsg(false);

	m_pMgr->UnRegisterEventHandler( _T("Finish_All_Download"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&LoadingFrame::OnFinishAllDownload));

	return TRUE;
}



//-----------------------------------------------------------------------------
//! 事件处理
//! 
//-----------------------------------------------------------------------------
BOOL LoadingFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_KeyDown:
		break;

	}

	return FALSE;
}



//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL LoadingFrame::ReloadUI()
{
	XmlElement element;

	tstring strPath = g_strLocalPath + _T("\\ui\\loading.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());

	m_pGUI->ChangeXml(&element, "panel", "PicFileName", 
		m_pUtil->UnicodeToUnicode8(m_strBackPicFileName.c_str()));

	m_pGUI->ChangeXml(&element, "panel\\progress", "ClassName", "CircleProgress" );

	// 缩放背景到整个游戏桌面
	GUIWnd* pDesktop = m_pGUI->GetDesktop();
	std::stringstream streamWidth, streamHeight;
	streamWidth << pDesktop->GetSize().x;
	streamHeight << pDesktop->GetSize().y;
	
	m_pGUI->ChangeXml(&element, "panel", "Size_x", streamWidth.str().c_str());
	m_pGUI->ChangeXml(&element, "panel", "Size_y", streamHeight.str().c_str());
/*
	// 将滚动条以及文字移动到合适位置
	std::stringstream streamY;
	streamY << (pDesktop->GetSize().y - 50);
	m_pGUI->ChangeXml(&element, "panel\\progress", "LogicPos_y", streamY.str().c_str());
*/
	// 创建
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&LoadingFrame::EventHandler));

	m_pWndText = (GUIStatic*)m_pWnd->GetChild(_T("text"));
	//m_pProgress = (GUIProgress*)m_pWnd->GetChild(_T("progress"));
	m_pProgress = (CircleProgress*)m_pWnd->GetChild(_T("progress"));

	tstring strTmp = _T("data\\ui\\loading\\L_Chilun.bmp");
	tagRect rc(0,0,90,90);
	tagRect rcDest(80,115,150,185);
	m_pProgress->AddNewPic(strTmp,rcDest,rc,1,0.3f);
	strTmp = _T("data\\ui\\loading\\L_Chilun.bmp");
	rcDest.Set(145,115,215,185);
	m_pProgress->AddNewPic(strTmp,rcDest,rc,0,0.3f);
	strTmp = _T("data\\ui\\loading\\L_shen.bmp");
	rcDest.Set(80,115,150,185);
	m_pProgress->AddNewPic(strTmp,rcDest,rc,1,0);
	strTmp = _T("data\\ui\\loading\\L_mo.bmp");
	rcDest.Set(145,115,215,185);
	m_pProgress->AddNewPic(strTmp,rcDest,rc,1,0);

	m_pWndText->Align();

	// 设置tips内容
	INT nMaxTips = m_pTips->GetVarNum() / 3;
	if( nMaxTips > 0 )
	{
		INT nTips = (rand() % nMaxTips) + 1;

		tstringstream stream;
		stream << nTips;
		LPCTSTR szText = m_pTips->GetString(stream.str().c_str());

		if( szText )
			m_pWndText->SetText(szText);
	}

	m_pProgress->SetSpeed(100000);
	m_pProgress->SetMaxValue(100);
	m_pProgress->SetValue(0);

	GUIEffectMgr::Inst()->SetLoadingFlag(TRUE);

	return TRUE;
}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID LoadingFrame::Update()
{
	GameFrame::Update();

	Sleep(50);	// 给读取线程更多CPU

	UINT uiFinished=0, uiReq=0;	
	ResMgr::Inst()->GetLoadCount(uiFinished, uiReq);
	if(m_eLoadingType == ELT_SelectRole)
	{
		int nLoaded = 0, nTotal = 0;
		TObjRef<Client>()->GetLoadCount(Client::E_Level_BeforeSelectRole, nLoaded, nTotal);
		nLoaded = min(nLoaded, nTotal);
		uiFinished += nLoaded*10;
		uiReq += nTotal*10;
	}
	if( uiFinished == uiReq )
	{
		if( m_eLoadingType == ELT_World )
		{
			SceneGraph* pSG = MapMgr::Inst()->GetSceneGraph();
			if( pSG )
				pSG->SyncInit();
		}
		else
		{
			SceneGraph* pSG = SeleRoleMgr::Inst()->GetHugeSG();
			if( P_VALID(pSG) )
				pSG->SyncInit();
		}

		ResMgr::Inst()->GetLoadCount(uiFinished, uiReq);
		if( GT_INVALID == m_dwQuitTime && uiFinished == uiReq )	// 仍然相等。。就退出
			m_dwQuitTime = timeGetTime();
	}

	if( GT_VALID(m_dwQuitTime) && timeGetTime() - m_dwQuitTime > 200 )
	{
		if (m_bLoginDownloadRes || timeGetTime() - m_dwQuitTime > 1000 )
		{
			GUIEffectMgr::Inst()->SetLoadingFlag(FALSE);
			m_pFrameMgr->AddToDestroyList(this);
			m_pMgr->SendEvent( &tagGameEvent( _T("Finish_Close_Loading"), NULL ) );
			// 发送打开地图加载完毕游戏事件
			tagGameEvent evt( _T("LoadMapIsOK"), NULL );
			m_pFrameMgr->SendEvent(&evt);
		}
	}

	if(0 == uiReq) return;
	DWORD dwCurrent = uiFinished * 100 / uiReq;
	if( dwCurrent > m_dwCurrent )
		m_dwCurrent = dwCurrent;

	m_pProgress->SetValue((FLOAT)m_dwCurrent);
	m_pProgress->SetRefresh();

}

BOOL LoadingFrame::EscCancel()
{
	return TRUE;
}

void LoadingFrame::SetLoadingType( ELoadingType type/*=ELT_World */)
{
	m_eLoadingType = type; 
	//如果为登陆loading，判断是否收到登陆界面加载资源完成的消息
	if ( ELT_Login == m_eLoadingType )
		m_bLoginDownloadRes = FALSE;
	else
		m_bLoginDownloadRes = TRUE;
}


DWORD LoadingFrame::OnFinishAllDownload(tagGameEvent* )
{
	if (P_VALID(m_pWnd))
		m_bLoginDownloadRes = TRUE;
	return 0;
}