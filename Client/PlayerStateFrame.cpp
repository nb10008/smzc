#include "stdafx.h"
#include "PlayerStateFrame.h"
#include "RoleHeadPicIcon.h"
#include "ActiveStatic.h"
#include "RoleMgr.h"
#include "Player.h"
#include "LocalPlayer.h"
#include "PlayerNameTab.h"
#include "RoleEvent.h"
#include "PlayerNameTab.h"
#include "CombatEvent.h"
#include "ProfessionMgr.h"
#include "BuffUIList.h"
#include "WorldPickerFrame.h"
#include "..\WorldDefine\msg_group.h"
#include "..\WorldDefine\msg_pk.h"
#include "TeamSys.h"
#include "TeamEvent.h"
#include "GameSet.h"
#include "ActiveProgress.h"
#include "ToolTipCreator.h"
#include "FlashPushButton.h"

const DWORD FLASH_TIME = 10*1000; 

PlayerStateFrame::PlayerStateFrame() :
m_Trunk( this ),
m_pWnd( NULL ),
m_pHead( NULL ),
m_pSt_PVP( NULL ),
m_pSt_Name( NULL ),
m_pSt_Level( NULL ),
m_pPro_Hp( NULL ),
m_pPro_Mp( NULL ),
m_pPro_Sta( NULL ),
m_pSt_Vocation( NULL ),
m_pBn_Menu( NULL ),
m_pSt_Captain( NULL ),
m_pWnd_Menu( NULL ),
m_pWnd_SecondMenu( NULL ),
m_pLb_Menu( NULL ),
m_pLb_SecondMenu( NULL ),
m_pBuffUIList( NULL ),
m_ePKMode( EMPKI_Protected ),
m_eRefuseApplyMode( EMRefuse_No ),
m_ePickMode( EMPickI_InTurn ),
m_pWnd_Violence( NULL ),
m_pMutex(NULL),
m_pBtnPeace(NULL),
m_pBtnRivalry(NULL),
m_pBtnKill(NULL),
m_dwViolenceTimeCount( 0 ),
m_dwViolenceStartCountDownTime( 0 ),
m_nLastSelMainMenu( GT_INVALID ),
m_nMenuPanelHeightExt( 0 ),
m_pPro_LoongYin( NULL ),
m_pSt_ViolenceTime(NULL),
m_pBtnExitTeam(NULL),
m_bFlag(false)
{
}

PlayerStateFrame::~PlayerStateFrame()
{
}

BOOL PlayerStateFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pMgr->RegisterEventHandle(_T("tagShowDeadMenuEvent"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnLPDeadEvent));
	m_pMgr->RegisterEventHandle( _T("tagLPPKState"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnLPPKState));
	m_pMgr->RegisterEventHandle( _T("tagRoleGetNameEvent"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnRoleGetNameEvent));
	m_pMgr->RegisterEventHandle( _T("tagRoleRenameEvent"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnRoleRenameEvent));
	m_pMgr->RegisterEventHandle( _T("tagUpdataAvatarEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnUpdataAvatarEvent));
	m_pMgr->RegisterEventHandle( _T("tagPKStateStartCountDownEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnPKStateStartCountDownEvent));
	m_pMgr->RegisterEventHandle( _T("tagRoleBuffChangeEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnRoleBuffChangeEvent));
	m_pMgr->RegisterEventHandle( _T("tagUpdateLPAttEvent"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnUpdateLPAttEvent));
	m_pMgr->RegisterEventHandle( _T("tagLPRoleStateChangeEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnLPRoleStateChangeEvent));
	m_pMgr->RegisterEventHandle( _T("tagUpdateLPLevel"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnUpdateLPLevel));
	m_pMgr->RegisterEventHandle( _T("tagTeamLeaderChangeEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnTeamLeaderChangeEvent));
	m_pMgr->RegisterEventHandle( _T("tagUpdataAvatarDisplaySetEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnUpdataAvatarDisplaySetEvent));
    m_pMgr->RegisterEventHandle( _T("LocalPlayerSetClass"),	            (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnLocalPlayerSetClass));
	m_pMgr->RegisterEventHandle( _T("AutoOpenPK"),						(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnEventAutoOpenPK));

	//TObjRef<NetCmdMgr>()->Register( "NS_RolePKStateChange",				(NETMSGPROC)m_Trunk.sfp2(&PlayerStateFrame::OnNetPKStateChange), _T("NS_RolePKStateChange") );
	TObjRef<NetCmdMgr>()->Register( "NS_RoleRevive",				(NETMSGPROC)m_Trunk.sfp2(&PlayerStateFrame::NetRecvNS_RoleRevive), _T("NS_RoleRevive") );
	TObjRef<NetCmdMgr>()->Register( "NS_RolePKMorality",				(NETMSGPROC)m_Trunk.sfp2(&PlayerStateFrame::NetNS_RolePKMorality), _T("NS_RolePKMorality") );
	

	// 快捷键
	m_pKeyMap->Register( 0, g_StrTable[_T("PK_Mode0")],			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&PlayerStateFrame::OnOpen_ViolenceOnlyHotKey), (DIK_X<<16)+DIK_LCONTROL, TRUE, FALSE, 0 );
	m_pKeyMap->Register( 0, g_StrTable[_T("PK_Mode1")],			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&PlayerStateFrame::OnOpen_ViolenceAllHotKey), (DIK_C<<16)+DIK_LCONTROL, TRUE, FALSE, 0);
	m_pKeyMap->Register( 0, g_StrTable[_T("PK_Mode2")],			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&PlayerStateFrame::OnOpen_ViolenceKillHotKey), (DIK_V<<16)+DIK_LCONTROL, TRUE, FALSE, 0);

	return TRUE;
}

BOOL PlayerStateFrame::Destroy()
{
	m_pMgr->UnRegisterEventHandler(_T("tagShowDeadMenuEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnLPDeadEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagLPPKState"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnLPPKState));
	m_pMgr->UnRegisterEventHandler( _T("tagRoleRenameEvent"),			 (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnRoleRenameEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagUpdataAvatarEvent"),			 (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnUpdataAvatarEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagPKStateStartCountDownEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnPKStateStartCountDownEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagRoleBuffChangeEvent"),		 (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnRoleBuffChangeEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagUpdateLPAttEvent"),			 (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnUpdateLPAttEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagLPRoleStateChangeEvent"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnLPRoleStateChangeEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagUpdateLPLevel"),				 (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnUpdateLPLevel));
	m_pMgr->UnRegisterEventHandler( _T("tagTeamLeaderChangeEvent"),		 (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnTeamLeaderChangeEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagUpdataAvatarDisplaySetEvent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnUpdataAvatarDisplaySetEvent));
    m_pMgr->UnRegisterEventHandler( _T("LocalPlayerSetClass"),           (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnLocalPlayerSetClass));
	m_pMgr->UnRegisterEventHandler( _T("tagRoleGetNameEvent"),			 (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnRoleGetNameEvent));
	m_pMgr->UnRegisterEventHandler( _T("AutoOpenPK"),					 (FRAMEEVENTPROC)m_Trunk.sfp1(&PlayerStateFrame::OnEventAutoOpenPK));

	//TObjRef<NetCmdMgr>()->UnRegister("NS_RolePKStateChange",			(NETMSGPROC)m_Trunk.sfp2(&PlayerStateFrame::OnNetPKStateChange));
	TObjRef<NetCmdMgr>()->UnRegister("NS_RoleRevive",					(NETMSGPROC)m_Trunk.sfp2(&PlayerStateFrame::NetRecvNS_RoleRevive));
	TObjRef<NetCmdMgr>()->UnRegister("NS_RolePKMorality",					(NETMSGPROC)m_Trunk.sfp2(&PlayerStateFrame::NetNS_RolePKMorality));

	m_pKeyMap->SetEnable( g_StrTable[_T("PK_Mode0")],	    FALSE );
	m_pKeyMap->SetEnable( g_StrTable[_T("PK_Mode1")],	    FALSE );
	m_pKeyMap->SetEnable( g_StrTable[_T("PK_Mode2")],	    FALSE );

	if( P_VALID(GetObj("MoralityWarning")) )
		KillObj("MoralityWarning");

	GameFrame::Destroy();

	SafeDestroyUI();

	return TRUE;
}

BOOL PlayerStateFrame::ReloadUI()
{
	SafeDestroyUI();

	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\main_ui.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &element, "characteratt\\att\\facepic", "ClassName", "RoleHeadPicIcon");
	m_pGUI->ChangeXml( &element, "characteratt\\att\\hp", "ClassName", "ActiveProgress");
	m_pGUI->ChangeXml( &element, "characteratt\\att\\mp", "ClassName", "ActiveProgress");

	for( INT i = 0; i < BUFFUI_NUM; i++ )
	{
		std::stringstream streamBuffStaticName;
	    streamBuffStaticName<<"characteratt\\buff"<<i+1;
		m_pGUI->ChangeXml( &element, streamBuffStaticName.str().c_str(), "ClassName", "ActiveStatic");
	}

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	if( !P_VALID( m_pWnd ) )
		return FALSE;

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&PlayerStateFrame::GUIEventHandler));

	m_pHead			= (RoleHeadPicIcon*)m_pWnd->GetChild( _T("att\\facepic") );
	m_pSt_PVP		= (GUIStatic*    )m_pWnd->GetChild( _T("att\\attack") );
	m_pSt_Name		= (GUIStatic*    )m_pWnd->GetChild( _T("att\\name") );
	m_pSt_Level		= (GUIStatic*    )m_pWnd->GetChild( _T("att\\grade") );
	m_pPro_Hp		= (ActiveProgress*)m_pWnd->GetChild( _T("att\\hp") );
	m_pPro_Mp		= (ActiveProgress*)m_pWnd->GetChild( _T("att\\mp") );
	m_pPro_Sta      = (GUIProgress*  )m_pWnd->GetChild( _T("att\\exp") );
	m_pPro_LoongYin	= (GUIProgress*  )m_pWnd->GetChild( _T("att\\progressbar4") );  
	m_pSt_Vocation  = (GUIStatic*    )m_pWnd->GetChild( _T("att\\vocation") );
	m_pBn_Menu		= (GUIPushButton*)m_pWnd->GetChild( _T("att\\teambutt") );
	m_pBn_Menu->SetEnable(FALSE);
	m_pSt_Captain	= (GUIStatic*    )m_pWnd->GetChild( _T("att\\teampic") );
	m_pBtnExitTeam  = (GUIButton*)m_pWnd->GetChild(_T("leavebutt"));

	m_pBuffUIList = new BuffUIList;
	for( INT i = 0; i < BUFFUI_NUM; i++ )
	{
		tstringstream streamBuffStaticName;
		tstringstream streamBuffProgressName;
		streamBuffStaticName<<_T("buff")<<i+1;
		streamBuffProgressName<<_T("pro")<<i+1;
		m_pBuffUIList->AddUI( (ActiveStatic*)m_pWnd->GetChild( streamBuffStaticName.str().c_str() ),
			                (GUIProgress*)m_pWnd->GetChild( streamBuffProgressName.str().c_str() ) );
	}


	// 创建菜单
	if( !CreateMenus() )
	{
		SafeDestroyUI();
		return FALSE;
	}

	// 创建PK模式窗口
	strPath = g_strLocalPath + _T("\\ui\\pkbutt.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml( &element, "pkwin\\choosemode\\upgrademode", "ClassName", "FlashPushButton");
	m_pWnd_Violence = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	if( !P_VALID( m_pWnd_Violence ) )
		return FALSE;
	m_pMutex = (GUIFrameMutex*)m_pWnd_Violence->GetChild( _T("choosemode") );
	m_pMutex->SetUnClickable(TRUE);
	m_pBtnPeace = (FlashPushButton*)m_pMutex->GetChild( _T("upgrademode") );
	m_pBtnRivalry = (GUIPushButton*)m_pMutex->GetChild( _T("confrontmode") );
	m_pBtnKill = (GUIPushButton*)m_pMutex->GetChild( _T("slaughtermode") );
	m_pSt_ViolenceTime = (GUIStatic*)m_pWnd_Violence->GetChild( _T("timeback") );
	m_pSt_ViolenceTime->SetInvisible(TRUE);
	m_pWnd->SetInvisible(FALSE);

	m_pGUI->RegisterEventHandler(m_pWnd_Menu->GetFullName().c_str(), m_Trunk.sfp1(&PlayerStateFrame::GUIEventHandler));
	m_pGUI->RegisterEventHandler(m_pWnd_SecondMenu->GetFullName().c_str(), m_Trunk.sfp1(&PlayerStateFrame::GUIEventHandler));
	m_pGUI->RegisterEventHandler(m_pWnd_Violence->GetFullName().c_str(), m_Trunk.sfp1(&PlayerStateFrame::GUIEventHandler));

	m_pSt_Captain->FlipToTop();
	m_pSt_Captain->SetTopMost( TRUE );
	m_pSt_PVP->SetInvisible( TRUE );
	m_pSt_Captain->SetInvisible( TRUE );

	m_pWnd_Menu->SetInvisible( TRUE );
	m_pWnd_SecondMenu->SetInvisible( TRUE );

	// 加载时设置玩家名字，以后就不再更新了
	tstring strName = PlayerNameTab::Inst()->FindNameByID( RoleMgr::Inst()->GetLocalPlayer()->GetID() );
	m_pSt_Name->SetText( strName.c_str() );

	return TRUE;
}

BOOL PlayerStateFrame::CreateMenus()
{
	tstring strPath = g_strLocalPath + _T("\\ui\\systemexpand.xml");
	XmlElement eleMenu;
	if( !m_pGUI->LoadXml( &eleMenu, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &eleMenu, "expand", "AlignControl", "\\desktop");
	m_pGUI->ChangeXml( &eleMenu, "expand", "AlignType", "NoAlign");
	{
		m_pGUI->ChangeXml( &eleMenu, "expand", "Name", "PlayerMenu");
		m_pGUI->ChangeXml( &eleMenu, "PlayerMenu\\listbutton1", "AlignControl", "\\desktop\\PlayerMenu");
		tagPoint ptWnd = m_pWnd->GetClient();
		stringstream streamX,streamY;
		streamX<<m_pWnd->GetClient().x + m_pWnd->GetSize().x;
		streamY<<m_pWnd->GetClient().y + 20;
		m_pGUI->ChangeXml( &eleMenu, "PlayerMenu", "LogicPos_x", streamX.str().c_str() );
		m_pGUI->ChangeXml( &eleMenu, "PlayerMenu", "LogicPos_y", streamY.str().c_str() );
		m_pWnd_Menu	= m_pGUI->CreateWnd( _T("\\desktop"), &eleMenu );
	}

	{
		m_pGUI->ChangeXml( &eleMenu, "PlayerMenu", "Name", "PlayerSecondMenu");
		m_pGUI->ChangeXml( &eleMenu, "PlayerSecondMenu\\listbutton1", "AlignControl", "\\desktop\\PlayerSecondMenu");
		tagPoint ptWnd = m_pWnd_Menu->GetClient();
		stringstream streamX,streamY;
		streamX<<m_pWnd_Menu->GetClient().x + m_pWnd_Menu->GetSize().x;
		streamY<<m_pWnd_Menu->GetClient().y;
		m_pGUI->ChangeXml( &eleMenu, "PlayerSecondMenu", "LogicPos_x", streamX.str().c_str() );
		m_pGUI->ChangeXml( &eleMenu, "PlayerSecondMenu", "LogicPos_y", streamY.str().c_str() );
		m_pWnd_SecondMenu	= m_pGUI->CreateWnd( _T("\\desktop"), &eleMenu );
	}

	m_pLb_Menu			= (GUIListBox*)m_pWnd_Menu->GetChild( _T("listbutton1") );
	m_pLb_Menu->SetColWidth(0, 116);
	m_pLb_SecondMenu	= (GUIListBox*)m_pWnd_SecondMenu->GetChild( _T("listbutton1") );
	m_pLb_SecondMenu->SetColWidth(0, 116);
	m_nMenuPanelHeightExt = m_pWnd_Menu->GetSize().y - m_pLb_Menu->GetSize().y;

	return TRUE;
}

VOID PlayerStateFrame::SafeDestroyUI()
{
	SAFE_DEL( m_pBuffUIList );

	if( P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd( m_pWnd );
		m_pWnd = NULL;
	}

	if( P_VALID( m_pWnd_Menu ) )
	{
		m_pGUI->DestroyWnd( m_pWnd_Menu );
		m_pWnd_Menu = NULL;
	}

	if( P_VALID( m_pWnd_SecondMenu ) )
	{
		m_pGUI->DestroyWnd( m_pWnd_SecondMenu );
		m_pWnd_SecondMenu = NULL;
	}

	if( P_VALID( m_pWnd_Violence ) )
	{
		m_pGUI->DestroyWnd( m_pWnd_Violence );
		m_pWnd_Violence = NULL;
	}
}

VOID PlayerStateFrame::Update()
{
	if( TeamSys::Inst()->IsInTeam() )
	{
		m_pBtnExitTeam->SetInvisible(FALSE);
		m_pBn_Menu->SetEnable(TRUE);
	}
	else
	{
		m_pBtnExitTeam->SetInvisible(TRUE);
		m_pBn_Menu->SetEnable(FALSE);
	}
	m_pBuffUIList->Updata();
	UpdataSecondMenuDisplay();
	if (RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_CSAttack) || RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_CSDefence) 
		||  RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_PvPAttack) || RoleMgr::Inst()->GetLocalPlayer()->IsInState(ES_PvPDefence))
		m_pWnd_Violence->SetInvisible(TRUE);
	else
		m_pWnd_Violence->SetInvisible(FALSE);

	// PK模式更新
	if( RoleMgr::Inst()->GetLocalPlayer()->GetRoleState( ERS_PK ) || 
		RoleMgr::Inst()->GetLocalPlayer()->GetRoleState( ERS_PKEX ) || 
		RoleMgr::Inst()->GetLocalPlayer()->GetRoleState( ERS_PeaceModel ) )
	{
		if( 0 != m_dwViolenceStartCountDownTime )
		{
			tstringstream stream;
			DWORD dwTimeLeft = m_dwViolenceTimeCount - ( timeGetTime() - m_dwViolenceStartCountDownTime );
			stream<<dwTimeLeft/1000+1;
			if( dwTimeLeft >= 0 && dwTimeLeft <= m_dwViolenceTimeCount )
			{
				m_pSt_ViolenceTime->SetText( stream.str().c_str() );
			}
			else
			{
				m_dwViolenceTimeCount = 0;
				m_dwViolenceStartCountDownTime = 0;
				m_pMutex->SetUnClickable(FALSE);
				m_pSt_ViolenceTime->SetInvisible(TRUE);
				if( m_bFlag )
				{
					m_bFlag = false;
					m_pBtnPeace->SetFlash(FALSE);
					m_pMutex->SetPush(m_pBtnPeace);
				}
			}
		}
	}

	// 显示/隐藏HP MP文字
	if( GameSet::Inst()->Get( EGS_ShowHPMPText ) )
	{
		if( m_pPro_Hp->GetText().empty() )
		{
			tagUpdateLPAttEvent evt( _T("tagUpdateLPAttEvent"), this );
			evt.eType = ERA_HP;
			OnUpdateLPAttEvent( &evt );
			evt.eType = ERA_MP;
			OnUpdateLPAttEvent( &evt );
			evt.eType = ERA_Endurance;
			OnUpdateLPAttEvent( &evt );
			evt.eType = ERA_Rage;
			OnUpdateLPAttEvent( &evt );
		}
	}
	else
	{
		if( !m_pPro_Hp->GetText().empty() )
		{
			m_pPro_Hp->SetText( _T("") );
			m_pPro_Mp->SetText( _T("") );
			m_pPro_Sta->SetText( _T("") );
			m_pPro_LoongYin->SetText( _T("") );
		}
	}

	// 防止滚动菜单
	if( 0 != m_pLb_Menu->GetCurrentTextRow() )
		m_pLb_Menu->SetCurrentTextRow( 0 );
	if( 0 != m_pLb_SecondMenu->GetCurrentTextRow() )
		m_pLb_SecondMenu->SetCurrentTextRow( 0 );

	GameFrame::Update();
}

VOID PlayerStateFrame::Render3D()
{
	if( P_VALID( m_pHead ) )
		m_pHead->Render3D();
}

DWORD PlayerStateFrame::OnRoleGetNameEvent(tagRoleGetNameEvent* pEvent)
{
	if( pEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		m_pSt_Name->SetText( pEvent->strRoleName.c_str() );
		m_pSt_Name->SetRefresh(true);
	}
	return 0;
}

DWORD PlayerStateFrame::OnRoleRenameEvent(tagRoleRenameEvent* pEvent)
{
	if( pEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		m_pSt_Name->SetText( pEvent->strRoleName.c_str() );
		m_pSt_Name->SetRefresh(true);
	}
	return 0;
}

DWORD PlayerStateFrame::OnUpdataAvatarEvent(tagUpdataAvatarEvent* pEvent)
{
	if( pEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		m_pHead->UpdataHeadPic( RoleMgr::Inst()->GetLocalPlayer() );
	}
	return 0;
}

DWORD PlayerStateFrame::OnPKStateStartCountDownEvent(tagPKStateStartCountDownEvent* pEvent)
{
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if( P_VALID(pLP) && !pLP->GetRoleState( ERS_PeaceModel ) )
	{
		m_dwViolenceTimeCount = pEvent->dwTimeCount;
		m_dwViolenceStartCountDownTime = timeGetTime();
		m_pSt_ViolenceTime->SetInvisible(FALSE);
		m_pMutex->SetUnClickable(TRUE);
	}
	return 0;
}

DWORD PlayerStateFrame::OnRoleBuffChangeEvent(tagRoleBuffChangeEvent* pEvent)
{
	Role* pRole = RoleMgr::Inst()->GetLocalPlayer();
	if( pRole->GetID() == pEvent->dwRoleID )
	{
		if(P_VALID(m_pBuffUIList))
			m_pBuffUIList->ClearBuffs();
		tagRoleBuff* pBuff = NULL;
		TList<tagRoleBuff*>& listBuff = pRole->GetBuffList();
		listBuff.ResetIterator();
		while( listBuff.PeekNext(pBuff) )
		{
			if( !P_VALID( pBuff ) )
				continue;
			if(P_VALID(m_pBuffUIList))
			{
				if( !m_pBuffUIList->AddBuff( pBuff ) )
				break;
			}
		}
	}
	return 0;
}

BOOL PlayerStateFrame::GUIEventHandler(tagGUIEvent* pEvent)
{
	const GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID( pWnd ) )
		return FALSE;
	switch( pEvent->eEvent )
	{
	case EGUIE_Deactive:
		if( !m_pWnd_Menu->IsInvisible() &&
			pEvent->dwParam1 != (DWORD)m_pWnd_Menu &&
			pEvent->dwParam1 != (DWORD)m_pLb_Menu &&
			pEvent->dwParam1 != (DWORD)m_pWnd_SecondMenu &&
			pEvent->dwParam1 != (DWORD)m_pLb_SecondMenu )
		{
			m_pWnd_Menu->SetInvisible( TRUE );
			m_pWnd_SecondMenu->SetInvisible( TRUE );
			m_pBn_Menu->SetState( EGUIBS_PopUp );
		}
		break;
	case EGUIE_Click:
		if( pWnd == m_pHead )
		{
			tagRolePickEvent evt( _T("tagRolePickEvent"), this );
			evt.eType = EWPT_LClick;
			evt.dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();
			m_pMgr->SendEvent( &evt );
		}
		else if( pWnd == m_pBtnExitTeam )
			TeamSys::Inst()->LeaveTeam();
		break;
	case EGUIE_PushButtonDown:
		if( pWnd == m_pBn_Menu )
		{
			if( 1 == pEvent->dwParam1 )                                                 // 菜单
			{
				UpdataMenuItem();
				m_pWnd_Menu->SetInvisible( FALSE );
				m_pWnd_SecondMenu->SetInvisible( TRUE );
				m_pLb_Menu->SetCurSelectedRow( GT_INVALID );
				m_nLastSelMainMenu = GT_INVALID;
			}
			else
			{
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pWnd_SecondMenu->SetInvisible( TRUE );
			}
		}
		else if( pWnd == m_pBtnPeace )
		{
			tagOpenPKEvent evt( _T("tagOpenPKEvent"), this );
			evt.bPK = (BOOL)GT_INVALID;
			m_pMgr->SendEvent( &evt );
			m_pBtnPeace->SetFlash(FALSE);
			m_bFlag = false;
		}
		else if( pWnd == m_pBtnRivalry )
		{
			tagOpenPKEvent evt( _T("tagOpenPKEvent"), this );
			evt.bPK = TRUE;
			m_pMgr->SendEvent( &evt );
			m_pBtnPeace->SetFlash(FALSE);
			m_bFlag = false;
		}
		else if( pWnd == m_pBtnKill )
		{
			tagOpenPKEvent evt( _T("tagOpenPKEvent"), this );
			evt.bPK = FALSE;
			m_pMgr->SendEvent( &evt );
			m_pBtnPeace->SetFlash(FALSE);
			m_bFlag = false;
		}
		break;
	case EGUIE_ItemClick:
		if( pWnd == m_pLb_Menu )
		{
			if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_ExitTeam] )
			{
				TeamSys::Inst()->LeaveTeam();
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pWnd_SecondMenu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
		}
		else if( pWnd == m_pLb_SecondMenu )
		{
			OnSecondMenuItemClick( pEvent->dwParam1 );
		}
		break;
	default:
		break;
	}

	if( P_VALID( m_pBuffUIList ) )
		m_pBuffUIList->GUIEventHandler( pEvent, pWnd, RoleMgr::Inst()->GetLocalPlayer()->GetID() );

	return FALSE;
}

VOID PlayerStateFrame::UpdataMenuItem()
{
	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if( !P_VALID( pLocalPlayer ) )
		return;

	for( int i = 0; i < EMI_Num; i++ )
	{
		m_nCurMenuIndex[i] = GT_INVALID;
	}
	INT nMenuIndex = 0;

	// PK模式
	//BOOL bProtected = pLocalPlayer->GetRoleState() & ERS_Safeguard;
	//if( bProtected )
	//	m_ePKMode = EMPKI_Protected;
	//else
	//	m_ePKMode = EMPKI_Freedom;
	//SetMenuFromStrTable( m_pLb_Menu, _T("LPMenu_PKMode_"), nMenuIndex, m_ePKMode );
	//m_nCurMenuIndex[EMI_PKMode] = nMenuIndex++;

	// 队伍操作
	if( TeamSys::Inst()->GetTeammateNum() > 0 )
	{
		// 队长操作
		if( TeamSys::Inst()->IsLeader( pLocalPlayer->GetID() ) )
		{
			// 驱除队员
			SetMenuFromStrTable( m_pLb_Menu, _T("LPMenu_"), nMenuIndex, EMI_RemoveTeamMate );
			m_nCurMenuIndex[EMI_RemoveTeamMate] = nMenuIndex++;

			// 队长移交
			SetMenuFromStrTable( m_pLb_Menu, _T("LPMenu_"), nMenuIndex, EMI_CaptainHandOut );
			m_nCurMenuIndex[EMI_CaptainHandOut] = nMenuIndex++;

			//设置拒绝申请
			const ERefuseApplyMode eRefuseMode = TeamSys::Inst()->GetRefuseApplyMode();
			if( eRefuseMode == ERAM_Accept )
				m_eRefuseApplyMode = EMRefuse_No;
			else if( eRefuseMode == ERAM_Refuse )
				m_eRefuseApplyMode = EMRefuse_Yes;

			SetMenuFromStrTable( m_pLb_Menu, _T("LPMenu_RefuseMode_"), nMenuIndex, m_eRefuseApplyMode );
			m_nCurMenuIndex[EMI_RefuseApply] = nMenuIndex++;
		}

		// 拾取模式
		const EPickMode ePickMode = TeamSys::Inst()->GetPickMode();
		if( ePickMode == EPUM_Order )
			m_ePickMode = EMPickI_InTurn;
		else if( ePickMode == EPUM_Free )
			m_ePickMode = EMPickI_Freedom;

		SetMenuFromStrTable( m_pLb_Menu, _T("LPMenu_PickMode_"), nMenuIndex, m_ePickMode );
		m_nCurMenuIndex[EMI_PickMode] = nMenuIndex++;

		// 离开队伍
		SetMenuFromStrTable( m_pLb_Menu, _T("LPMenu_"), nMenuIndex, EMI_ExitTeam );
		m_nCurMenuIndex[EMI_ExitTeam] = nMenuIndex++;
	}

	ForceResizeMenu( m_pLb_Menu, nMenuIndex );
	m_pLb_Menu->SetCurSelectedRow( GT_INVALID );
}

VOID PlayerStateFrame::ForceResizeMenu( GUIListBox* pListBox, const INT nNumRow )
{
	tagPoint ptSizeListBox = pListBox->GetSize();
	ptSizeListBox.y = pListBox->GetRowHeight() * nNumRow;

	GUIWnd* pFather = pListBox->GetFather();
	if( P_VALID( pFather ) )
	{
		pFather->SetResizable( TRUE );
		tagPoint ptSizeFather = pFather->GetSize();
		ptSizeFather.y = ptSizeListBox.y + m_nMenuPanelHeightExt;
		pFather->Resize( ptSizeFather );
		pFather->SetResizable( FALSE );
	}
	pListBox->SetResizable( TRUE );
	pListBox->Resize( ptSizeListBox );
	pListBox->SetResizable( FALSE );
}

VOID PlayerStateFrame::SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nLbIndex, const INT nStrIndex )
{
	tstringstream stream;
	stream<<szBaseKey<<nStrIndex;
	tstring strText = g_StrTable[ stream.str().c_str() ];
	pListBox->SetText( nLbIndex, 0, strText.c_str() );
}

VOID PlayerStateFrame::SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nNumRow, const INT nStartRow, const INT nEndRow, const BOOL bAutoResize )
{
	if( bAutoResize )
		ForceResizeMenu( pListBox, nNumRow );

	for( INT i = nStartRow; i <= nEndRow; i++ )
	{
		tstringstream stream;
		stream<<szBaseKey<<i;
		tstring strText = g_StrTable[ stream.str().c_str() ];
		pListBox->SetText( i, 0, strText.c_str() );
	}
}

VOID PlayerStateFrame::UpdataSecondMenuDisplay()
{
	if( !P_VALID( m_pWnd_Menu ) )
		return;

	if( m_pWnd_Menu->IsInvisible() )
		return;

	const INT nCurSelMainMenu = m_pLb_Menu->GetCurSelectedRow();
	if( GT_INVALID == nCurSelMainMenu )
	{
		m_pWnd_SecondMenu->SetInvisible( TRUE );
		return;
	}

	if( nCurSelMainMenu == m_nLastSelMainMenu )
		return;

	m_nLastSelMainMenu = nCurSelMainMenu;

	//if( nCurSelMainMenu == m_nCurMenuIndex[EMI_PKMode] )
	//{
	//	SetMenuFromStrTable( m_pLb_SecondMenu, _T("LPMenu_PKMode_"), 2, 0, 1, TRUE );
	//	m_pWnd_SecondMenu->SetInvisible( FALSE );
	//}
	if( nCurSelMainMenu == m_nCurMenuIndex[EMI_RemoveTeamMate] )
	{
		if( TeamSys::Inst()->IsLeader( RoleMgr::Inst()->GetLocalPlayerID() ) )
		{
			ReBuildTeammateList( m_pLb_SecondMenu );
			m_pWnd_SecondMenu->SetInvisible( FALSE );
		}
		else
			m_pWnd_SecondMenu->SetInvisible( TRUE );
	}
	else if( nCurSelMainMenu == m_nCurMenuIndex[EMI_CaptainHandOut] )
	{
		if( TeamSys::Inst()->IsLeader( RoleMgr::Inst()->GetLocalPlayerID() ) )
		{
			ReBuildTeammateList( m_pLb_SecondMenu );
			m_pWnd_SecondMenu->SetInvisible( FALSE );
		}
		else
			m_pWnd_SecondMenu->SetInvisible( TRUE );
	}
	else if( nCurSelMainMenu == m_nCurMenuIndex[EMI_PickMode] )
	{
		if( TeamSys::Inst()->IsLeader( RoleMgr::Inst()->GetLocalPlayerID() ) )
		{
			SetMenuFromStrTable( m_pLb_SecondMenu, _T("LPMenu_PickMode_"), 2, 0, 1, TRUE );
			m_pWnd_SecondMenu->SetInvisible( FALSE );
		}
		else
			m_pWnd_SecondMenu->SetInvisible( TRUE );
	}
	else if( nCurSelMainMenu == m_nCurMenuIndex[EMI_RefuseApply] )
	{
		if( TeamSys::Inst()->IsLeader( RoleMgr::Inst()->GetLocalPlayerID() ) )
		{
			SetMenuFromStrTable( m_pLb_SecondMenu, _T("LPMenu_RefuseMode_"), 2, 0, 1, TRUE );
			m_pWnd_SecondMenu->SetInvisible( FALSE );
		}
		else
			m_pWnd_SecondMenu->SetInvisible( TRUE );
	}
	else
		m_pWnd_SecondMenu->SetInvisible( TRUE );

	if( !m_pWnd_SecondMenu->IsInvisible() )
	{
		ResetSecondMenuPos( nCurSelMainMenu );
		m_pLb_SecondMenu->SetCurSelectedRow( GT_INVALID );
		m_pWnd_SecondMenu->SetRefresh();
	}
}

VOID PlayerStateFrame::ResetSecondMenuPos( const INT nCurMainMenuSel )
{
	const BOOL bMoveable = m_pWnd_SecondMenu->IsMovable();
	m_pWnd_SecondMenu->SetMovable( TRUE );
	tagPoint ptDestPos = tagPoint( m_pWnd_Menu->GetClient().x + m_pWnd_Menu->GetSize().x, m_pWnd_Menu->GetClient().y );
	ptDestPos.y += nCurMainMenuSel * m_pLb_Menu->GetRowHeight();
	tagPoint ptOldPos = m_pWnd_SecondMenu->GetClient();
	m_pWnd_SecondMenu->Move( ptDestPos - ptOldPos );
	m_pWnd_SecondMenu->SetMovable( bMoveable );
}

VOID PlayerStateFrame::ReBuildTeammateList( GUIListBox* pListBox )
{
	const vector<tagTeammateData>& vecTeammates = TeamSys::Inst()->GetTeammateList();
	ForceResizeMenu( m_pLb_SecondMenu, vecTeammates.size() );
	for( size_t i = 0; i < vecTeammates.size(); i++ )
	{
		pListBox->SetText( i, 0, vecTeammates[i].strName.c_str() );
		pListBox->SetUserData( i, vecTeammates[i].dwRoleID );
	}
}

VOID PlayerStateFrame::OnSecondMenuItemClick( const INT nSelItem )
{
	if( GT_INVALID == nSelItem )
		return;

	const INT nCurSelMainMenu = m_pLb_Menu->GetCurSelectedRow();

	if( nCurSelMainMenu == m_nCurMenuIndex[EMI_PKMode] )
	{
		tagOpenSafeguardEvent evt( _T("tagOpenSafeguardEvent"), this );
		evt.bOpen = ( nSelItem == EMPKI_Protected );
		m_pMgr->SendEvent( &evt );
	}
	else if( nCurSelMainMenu == m_nCurMenuIndex[EMI_RemoveTeamMate] )
	{
		TeamSys::Inst()->KickTeammate( m_pLb_SecondMenu->GetUserData( nSelItem ) );
	}
	else if( nCurSelMainMenu == m_nCurMenuIndex[EMI_CaptainHandOut] )
	{
		TeamSys::Inst()->HandOutLeader( m_pLb_SecondMenu->GetUserData( nSelItem ) );
	}
	else if( nCurSelMainMenu == m_nCurMenuIndex[EMI_PickMode] )
	{
		if( nSelItem == EMPickI_InTurn )
			TeamSys::Inst()->SetPickMode( EPUM_Order );
		else if( nSelItem == EMPickI_Freedom )
			TeamSys::Inst()->SetPickMode( EPUM_Free );
	}
	else if( nCurSelMainMenu == m_nCurMenuIndex[EMI_RefuseApply] )
	{
		if( nSelItem == EMRefuse_No )
		{
			TeamSys::Inst()->SetRefuseApplyMode( ERAM_Accept );
			m_eRefuseApplyMode = EMRefuse_No;
		}
		else if( nSelItem == EMRefuse_Yes )
		{
			TeamSys::Inst()->SetRefuseApplyMode( ERAM_Refuse );
			m_eRefuseApplyMode = EMRefuse_Yes;
		}
	}
	else
		return;

	m_pWnd_SecondMenu->SetInvisible( TRUE );
	m_pWnd_Menu->SetInvisible( TRUE );
	m_pBn_Menu->SetState( EGUIBS_PopUp );
}

DWORD PlayerStateFrame::OnUpdateLPLevel(tagUpdateLPLevel* pEvent)
{
	tstringstream stream;
	stream<<pEvent->nCurLevel;
	m_pSt_Level->SetText( stream.str().c_str() );
	m_pSt_Level->SetRefresh( TRUE );
	m_bFlag = false;
	if( FREE_PK_LEVEL <= pEvent->nCurLevel && pEvent->nCurLevel - pEvent->nInc < FREE_PK_LEVEL && pEvent->bLevelUp )
	{
// 		if( P_VALID(GetObj("FreePKTips")) )
// 			KillObj("FreePKTips");
// 		CreateObj("FreePKTips", "MsgBoxEx");
// 		TObjRef<MsgBoxEx>("FreePKTips")->Init( _T(""), 
// 			g_StrTable[_T("FreePKTips")], NULL, MsgBoxFlag(MBF_OK), TRUE, TRUE );
		m_pBtnPeace->SetEnable(true);
		m_pBtnPeace->SetFlash(true);
		m_pBtnRivalry->SetEnable(true);
		m_pBtnKill->SetEnable(true);
		m_pMutex->SetRefresh(true);
		m_bFlag = true;
		m_dwViolenceTimeCount = FLASH_TIME;
		m_dwViolenceStartCountDownTime = timeGetTime();
		return 0;
	}
	if( pEvent->nCurLevel >= FREE_PK_LEVEL )
	{
		m_pMutex->SetUnClickable(FALSE);
		if( RoleMgr::Inst()->GetLocalPlayer()->GetRoleState( ERS_PeaceModel ) )
			m_pMutex->SetPush(m_pBtnPeace);
		else if( RoleMgr::Inst()->GetLocalPlayer()->GetRoleState( ERS_PK ) )
			m_pMutex->SetPush(m_pBtnKill);
		else if( RoleMgr::Inst()->GetLocalPlayer()->GetRoleState( ERS_PKEX ) )
			m_pMutex->SetPush(m_pBtnRivalry);

		m_pBtnPeace->SetEnable(true);
		m_pBtnRivalry->SetEnable(true);
		m_pBtnKill->SetEnable(true);
		m_pMutex->SetRefresh(true);
	}
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return 0;
	return 0;
}

DWORD PlayerStateFrame::OnUpdateLPAttEvent(tagUpdateLPAttEvent* pEvent)
{
	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if( !P_VALID(pLocalPlayer) )
		return 0;

	switch( pEvent->eType )
	{
	case ERA_HP:
	case ERA_MaxHP:
		{
			//死亡状态不更新角色的红和蓝
			if( !P_VALID(pLocalPlayer) || pLocalPlayer->IsInState( ES_Dead ) )
				break;

			const int nMaxHp	= pLocalPlayer->GetAttribute( ERA_MaxHP );
			const int nHp		= pLocalPlayer->GetAttribute( ERA_HP );
			if( GT_INVALID != nMaxHp && GT_INVALID != nHp )
			{
				m_pPro_Hp->SetMaxValue( (FLOAT)nMaxHp );
				m_pPro_Hp->SetValue( (FLOAT)nHp, true );
				if( GameSet::Inst()->Get( EGS_ShowHPMPText ) )
				{
					tstringstream stream;
					stream<<nHp<<" / "<<nMaxHp;
					m_pPro_Hp->SetText( stream.str().c_str() );

					// 文字超出了控件，所以画布也需要刷新
					m_pPro_Mp->SetRefresh( TRUE );
				}
			}

			// 低于30%闪烁
			if( (float)nHp / nMaxHp < 0.3f )
				m_pPro_Hp->Glint( TRUE, 0x80 );
			else
				m_pPro_Hp->Glint( FALSE, 0x80 );
		}
		break;
	case ERA_MP:
	case ERA_MaxMP:
		{
			//死亡状态不更新角色的红和蓝
			if( !P_VALID(pLocalPlayer) || pLocalPlayer->IsInState( ES_Dead ) )
				break;

			const int nMaxMp	= pLocalPlayer->GetAttribute( ERA_MaxMP );
			const int nMp		= pLocalPlayer->GetAttribute( ERA_MP );
			if( GT_INVALID != nMaxMp && GT_INVALID != nMp )
			{
				m_pPro_Mp->SetMaxValue( (FLOAT)nMaxMp );
				m_pPro_Mp->SetValue( (FLOAT)nMp, true );
				if( GameSet::Inst()->Get( EGS_ShowHPMPText ) )
				{
					tstringstream stream;
					stream<<nMp<<" / "<<nMaxMp;
					m_pPro_Mp->SetText( stream.str().c_str() );

					// 文字超出了控件，所以画布也需要刷新
					m_pPro_Mp->SetRefresh( TRUE );
				}
			}

			// 低于30%闪烁
			if( (float)nMp / nMaxMp < 0.3f )
				m_pPro_Mp->Glint( TRUE, 0x80 );
			else
				m_pPro_Mp->Glint( FALSE, 0x80 );
		}
		break;
	case ERA_Endurance:
	case ERA_MaxEndurance:
		{
			const int nMaxSta	= pLocalPlayer->GetAttribute( ERA_MaxEndurance );
			const int nSta      = pLocalPlayer->GetAttribute( ERA_Endurance );
			if( GT_INVALID != nMaxSta && GT_INVALID != nSta )
			{
				m_pPro_Sta->SetMaxValue( (FLOAT)nMaxSta );
				m_pPro_Sta->SetValue( (FLOAT)nSta, true );
				if( GameSet::Inst()->Get( EGS_ShowHPMPText ) )
				{
					tstringstream stream;
					stream<<nSta<<" / "<<nMaxSta;
					m_pPro_Sta->SetText( stream.str().c_str() );
				}
			}
		}
		break;
	case ERA_Rage:	//龙隐值
		{
			const int nMaxLoongYin = 100;
			const int nLoongYin = pLocalPlayer->GetAttribute( ERA_Rage );
			if( nLoongYin!=GT_INVALID )
			{
				m_pPro_LoongYin->SetMaxValue( (FLOAT)nMaxLoongYin );
				m_pPro_LoongYin->SetValue( (FLOAT)nLoongYin, true );
				if( GameSet::Inst()->Get( EGS_ShowHPMPText ) )
				{
					tstringstream stream;
					stream<<nLoongYin<<" / "<<nMaxLoongYin;
					m_pPro_LoongYin->SetText( stream.str().c_str() );
				}
			}

		}
		break;
	}
	return 0;
}

DWORD PlayerStateFrame::OnLPRoleStateChangeEvent(tagLPRoleStateChangeEvent* pEvent)
{
	LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	bool bProtected = pLocalPlayer->GetRoleState( ERS_Safeguard );
	bool bInSafeArea = pLocalPlayer->GetRoleState( ERS_SafeArea );
	if( bProtected || ( bInSafeArea && !( pEvent->dwOldState & ERS_SafeArea ) ) )
	{
		m_pBtnPeace->SetEnable(false);
		m_pBtnRivalry->SetEnable(false);
		m_pBtnKill->SetEnable(false);
		m_pMutex->SetRefresh(true);
	}
	else if( !bInSafeArea && ( pEvent->dwOldState & ERS_SafeArea ) )
	{
		m_pBtnRivalry->SetEnable(true);
		m_pBtnKill->SetEnable(true);
		if( pLocalPlayer->GetPKState() == ERPKS_Peace || pLocalPlayer->GetPKState() == ERPKS_Warnning )
			m_pBtnPeace->SetEnable(true);
		if( RoleMgr::Inst()->GetLocalPlayer()->GetRoleState( ERS_PeaceModel ) )
			m_pMutex->SetPush(m_pBtnPeace);
		else if( RoleMgr::Inst()->GetLocalPlayer()->GetRoleState( ERS_PK ) )
			m_pMutex->SetPush(m_pBtnKill);
		else if( RoleMgr::Inst()->GetLocalPlayer()->GetRoleState( ERS_PKEX ) )
			m_pMutex->SetPush(m_pBtnRivalry);
		m_pMutex->SetRefresh(true);
	}
	return 0;
}

DWORD PlayerStateFrame::OnTeamLeaderChangeEvent(tagTeamLeaderChangeEvent* pEvent)
{
	// 队长图标
	if( RoleMgr::Inst()->GetLocalPlayerID() == pEvent->dwNewLeaderID )
		m_pSt_Captain->SetInvisible( FALSE );
	else
		m_pSt_Captain->SetInvisible( TRUE );

	return 0;
}

DWORD PlayerStateFrame::OnUpdataAvatarDisplaySetEvent(tagUpdataAvatarDisplaySetEvent* pEvent)
{
	if( pEvent->dwRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		m_pHead->UpdataDisplaySet( RoleMgr::Inst()->GetLocalPlayer()->GetDisplaySet() );
	}
	return 0;
}

DWORD PlayerStateFrame::OnOpen_ViolenceOnlyHotKey(DWORD dwID, BOOL bDown)
{
	if( bDown )
	{
		LocalPlayer *pPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pPlayer) || pPlayer->IsDead() )
			return 0;
		if (pPlayer->IsInState(ES_CSAttack) || pPlayer->IsInState(ES_CSDefence) 
			||  pPlayer->IsInState(ES_PvPAttack) || pPlayer->IsInState(ES_PvPDefence))
			return 0;
		if( pPlayer->GetRoleLevel() < FREE_PK_LEVEL || 
			pPlayer->GetRoleState( ERS_Safeguard ) || 
			pPlayer->GetRoleState( ERS_SafeArea ) || 
			pPlayer->GetPKState() == ERPKS_Wicked || 
			pPlayer->GetPKState() == ERPKS_Wanted )
			return 0;
		if( m_dwViolenceTimeCount == 0 )
			m_pBtnPeace->SetState(EGUIBS_PushDown);
	}
	return 0;
}

DWORD PlayerStateFrame::OnOpen_ViolenceAllHotKey(DWORD dwID, BOOL bDown)
{
	if( bDown )
	{
		LocalPlayer *pPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pPlayer) || pPlayer->IsDead() )
			return 0;
		if (pPlayer->IsInState(ES_CSAttack) || pPlayer->IsInState(ES_CSDefence) 
			||  pPlayer->IsInState(ES_PvPAttack) || pPlayer->IsInState(ES_PvPDefence))
			return 0;
		if( pPlayer->GetRoleLevel() < FREE_PK_LEVEL || 
			pPlayer->GetRoleState( ERS_Safeguard ) ||
			pPlayer->GetRoleState( ERS_SafeArea ) )
			return 0;
		if( m_dwViolenceTimeCount == 0 )
			m_pBtnRivalry->SetState(EGUIBS_PushDown);
	}
	return 0;
}

DWORD PlayerStateFrame::OnOpen_ViolenceKillHotKey(DWORD dwID, BOOL bDown)
{
	if( bDown )
	{
		LocalPlayer *pPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pPlayer) || pPlayer->IsDead() )
			return 0;
		if (pPlayer->IsInState(ES_CSAttack) || pPlayer->IsInState(ES_CSDefence) 
			||  pPlayer->IsInState(ES_PvPAttack) || pPlayer->IsInState(ES_PvPDefence))
			return 0;
		if( pPlayer->GetRoleLevel() < FREE_PK_LEVEL || 
			pPlayer->GetRoleState( ERS_Safeguard ) ||
			pPlayer->GetRoleState( ERS_SafeArea ) )
			return 0;
		if( m_dwViolenceTimeCount == 0 )
			m_pBtnKill->SetState(EGUIBS_PushDown);
	}
	return 0;
}

DWORD PlayerStateFrame::OnLocalPlayerSetClass( tagGameEvent* pEvent )
{
    RefreshClassText();

    return 0;
}

VOID PlayerStateFrame::RefreshClassText()
{
    LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
    if (P_VALID(pLp))
    {
        m_pSt_Vocation->SetText(ProfessionMgr::Inst().GetClassName(pLp->GetPlayerClass()));
        m_pSt_Vocation->SetRefresh(TRUE);
    }
}

DWORD PlayerStateFrame::OnEventAutoOpenPK(tagGameEvent *pEvent)
{
	m_pBtnKill->SetState(EGUIBS_PushDown,TRUE);
	return 0;
}

DWORD PlayerStateFrame::OnLPPKState(tagLPPKState* pEvent)
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp) || pLp->GetID() != pEvent->dwRoleID )
		return 0;
	if( pEvent->eState == ERPKS_Wicked || pEvent->eState == ERPKS_Wanted )
	{
		if( m_pBtnPeace->GetState() == EGUIBS_PushDown )
			m_pBtnRivalry->SetState(EGUIBS_PushDown,TRUE);
		m_pBtnPeace->SetEnable(false);
		if( pEvent->eState == ERPKS_Wicked && pEvent->eOldState == ERPKS_Warnning )
		{
// 			if( P_VALID(GetObj("MoralityWarning")) )
// 				KillObj("MoralityWarning");
// 			CreateObj("MoralityWarning", "MsgBoxEx");
// 
// 			TObjRef<MsgBoxEx>("MoralityWarning")->Init( _T(""), g_StrTable[_T("MoralityWarning")],NULL ,MBF_OK,FALSE);
		}
	}
	else if( pLp->GetRoleLevel() >= FREE_PK_LEVEL )
		m_pBtnPeace->SetEnable(true);
	m_pBtnPeace->SetRefresh(true);
	return 0;
}


DWORD PlayerStateFrame::NetRecvNS_RoleRevive(tagNS_RoleRevive* pMsg, DWORD dwParam)
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))
		return 0;
	if( pLp->GetID() == pMsg->dwRoleID && pMsg->dwErrorCode == E_Success )
	{
		m_dwViolenceTimeCount = 0;
		m_dwViolenceStartCountDownTime = 0;
		if( pLp->GetPKState() == ERPKS_Peace || pLp->GetPKState() == ERPKS_Warnning )
			m_pBtnPeace->SetEnable(true);
		m_pBtnRivalry->SetEnable(true);
		m_pBtnKill->SetEnable(true);
		m_pSt_ViolenceTime->SetInvisible(TRUE);
		m_pMutex->SetUnClickable(FALSE);
		m_pMutex->SetRefresh(true);
	}
	return 0;
}


DWORD PlayerStateFrame::OnLPDeadEvent(tagShowDeadMenuEvent* pEvent)
{
	m_pBtnPeace->SetEnable(false);
	m_pBtnRivalry->SetEnable(false);
	m_pBtnKill->SetEnable(false);
	m_pMutex->SetRefresh(true);
	return 0;
}

DWORD PlayerStateFrame::NetNS_RolePKMorality(tagNS_RolePKMorality* pMsg, DWORD dwParam)
{
	 	if( P_VALID(GetObj("MoralityWarning")) )
	 		KillObj("MoralityWarning");
	 	CreateObj("MoralityWarning", "MsgBoxEx");
		TCHAR szNum[X_HUGE_NAME];
		_stprintf(szNum,g_StrTable[_T("MoralityWarning")],pMsg->nTime,pMsg->nValue);
	 	TObjRef<MsgBoxEx>("MoralityWarning")->Init( _T(""), szNum,NULL ,MBF_OK,FALSE);
	return 0;
}