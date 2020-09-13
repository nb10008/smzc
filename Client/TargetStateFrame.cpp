#include "stdafx.h"
#include "TargetStateFrame.h"
#include "ActiveStatic.h"
#include "RoleHeadPicIcon.h"
#include "WorldPickerFrame.h"
#include "RoleMgr.h"
#include "Role.h"
#include "Player.h"
#include "NPC.h"
#include "MapMgr.h"
#include "LocalPlayer.h"
#include "PlayerNameTab.h"
#include "RoleEvent.h"
#include "CreatureData.h"
#include "CombatEvent.h"
#include "ProfessionMgr.h"
#include "BuffUIList.h"
#include "EffectMgr.h"
#include "ChatEvent.h"
#include "TeamSys.h"
#include "SocialEvent.h"
#include "GameSet.h"
#include "GuildMgr.h"
#include "CombatSys.h"
#include "PetManager.h"
#include "Pet.h"
#include "ItemMgr.h"
#include "TeamEvent.h"
#include "DispRoleInfoFrame.h"
#include "MasterEvent.h"
#include "CombatActionChecker.h"
#include "CombatSysUtil.h"
#include "SpritePartnershipFrame.h"
#include "ClothespressData.h"
#include "..\WorldDefine\msg_fashion.h"
#include "FamilyEvent.h"

static DWORD  dwCreateId = GT_INVALID;
static DWORD  dwTaggedID = GT_INVALID;
static DWORD  dwFormerShowRoleID = GT_INVALID;   
const DWORD MIN_ACQUIRE_TARGETsTARGET_TIME = 3000;
TargetStateFrame::TargetStateFrame() :
m_Trunk( this ),
m_pWnd( NULL ),
m_pHead( NULL ),
m_pSt_PVP( NULL ),
m_pSt_Name( NULL ),
m_pSt_Level( NULL ),
m_pPro_Hp( NULL ),
m_pPro_Mp( NULL ),
m_pSt_AppendInfo( NULL ),
m_pBn_Menu( NULL ),
m_pWnd_Menu( NULL ),
m_pLb_Menu( NULL ),
m_pBuffUIList( NULL ),
m_dwCurShowRoleID( GT_INVALID ),
m_dwSelectEffectID( GT_INVALID ),
m_nMenuPanelHeightExt( 0 ),
m_nLastLevel( GT_INVALID ),
m_nLastMaxHp( GT_INVALID ),
m_nLastHp( GT_INVALID ),
m_nLastMaxMp( GT_INVALID ),
m_nLastMp( GT_INVALID ),
m_pSt_AI(NULL),
m_pSt_Belong(NULL),
m_bLastNameGray( false ),
m_bIfRevServerInfo(FALSE),
m_dwCurTargetID(GT_INVALID),
m_dwCurTargetRemainTime(0),
m_pWnd_SuitMenu(NULL),
m_bNeedToSyncNoTarget(true)
{

}

TargetStateFrame::~TargetStateFrame()
{
}

BOOL TargetStateFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pMgr->RegisterEventHandle( _T("tagRolePickEvent"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnRolePickEvent));
	m_pMgr->RegisterEventHandle( _T("tagRoleRenameEvent"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnRoleGetNameEvent));
	m_pMgr->RegisterEventHandle( _T("tagUpdataAvatarEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnUpdataAvatarEvent));
	m_pMgr->RegisterEventHandle( _T("tagRoleBuffChangeEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnRoleBuffChangeEvent));
	m_pMgr->RegisterEventHandle( _T("tagUpdataAvatarDisplaySetEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnUpdataAvatarDisplaySetEvent));
    m_pMgr->RegisterEventHandle( _T("RemotePlayerSetClass"),	        (FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnChangePlayerClass));
	m_pCmdMgr->Register("NS_CreatureTaggedOwnerChange",(NETMSGPROC)m_Trunk.sfp2(&TargetStateFrame::CreatureTaggedOwnerChange), _T("NS_CreatureTaggedOwnerChange"));
	m_pCmdMgr->Register("NS_TargetOfTarget",(NETMSGPROC)m_Trunk.sfp2(&TargetStateFrame::OnNS_TargetOfTarget), _T("NS_TargetOfTarget"));

	return TRUE;
}

BOOL TargetStateFrame::Destroy()
{
	m_pMgr->UnRegisterEventHandler( _T("tagRolePickEvent"),				 (FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnRolePickEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagRoleRenameEvent"),			 (FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnRoleGetNameEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagUpdataAvatarEvent"),			 (FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnUpdataAvatarEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagRoleBuffChangeEvent"),		 (FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnRoleBuffChangeEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagUpdataAvatarDisplaySetEvent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnUpdataAvatarDisplaySetEvent));
    m_pMgr->UnRegisterEventHandler( _T("RemotePlayerSetClass"),	         (FRAMEEVENTPROC)m_Trunk.sfp1(&TargetStateFrame::OnChangePlayerClass));
	m_pCmdMgr->UnRegister("NS_CreatureTaggedOwnerChange",(NETMSGPROC)m_Trunk.sfp2(&TargetStateFrame::CreatureTaggedOwnerChange));
	m_pCmdMgr->UnRegister("NS_TargetOfTarget",(NETMSGPROC)m_Trunk.sfp2(&TargetStateFrame::OnNS_TargetOfTarget));

	GameFrame::Destroy();

	m_vecOwnNPCList.clear();
	SafeDestroyUI();

	return TRUE;
}


BOOL TargetStateFrame::ReloadUI()
{
	SafeDestroyUI();

	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\target.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	// 修改类名以便创建自定义控件
	m_pGUI->ChangeXml( &element, "targetwin\\att\\facepic", "ClassName", "RoleHeadPicIcon");
	m_pGUI->ChangeXml( &element, "targetwin\\t_of_target\\pic\\facepic", "ClassName", "RoleHeadPicIcon");

	for( INT i = 0; i < BUFFUI_NUM; i++ )
	{
		std::stringstream streamBuffStaticName;
	    streamBuffStaticName<<"targetwin\\buff"<<i+1;
		m_pGUI->ChangeXml( &element, streamBuffStaticName.str().c_str(), "ClassName", "ActiveStatic");
	}

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	if( !P_VALID( m_pWnd ) )
		return FALSE;

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&TargetStateFrame::GUIEventHandler));

	m_pHead				= (RoleHeadPicIcon*)m_pWnd->GetChild( _T("att\\facepic") );
	m_pSt_PVP			= (GUIStatic*    )m_pWnd->GetChild( _T("att\\attack") );
	m_pSt_Name			= (GUIStatic*)m_pWnd->GetChild( _T("att\\name") );
	m_pSt_Level			= (GUIStatic*)m_pWnd->GetChild( _T("att\\grade") );
	m_pPro_Hp			= (GUIProgress*)m_pWnd->GetChild( _T("att\\hp") );
	m_pPro_Mp			= (GUIProgress*)m_pWnd->GetChild( _T("att\\mp") );
	m_pSt_AppendInfo    = (GUIStatic*)m_pWnd->GetChild( _T("att\\ai") );
	m_pBn_Menu			= (GUIPushButton*)m_pWnd->GetChild( _T("att\\teambutt") );
	m_pSt_AI			= (GUIStatic*)m_pWnd->GetChild(_T("att\\ai"));
	m_pSt_Belong		= (GUIStatic*)m_pWnd->GetChild(_T("att\\facepic\\belong"));

	m_pSt_TofTBackAll= (GUIStatic*)m_pWnd->GetChild( _T("t_of_target") );
	m_pSt_TofTBack= (GUIStatic*)m_pWnd->GetChild( _T("t_of_target\\pic") );
	m_pSt_TofTHeadPic= (RoleHeadPicIcon*)m_pWnd->GetChild( _T("t_of_target\\pic\\facepic") );
	m_pSt_TofTName= (GUIStatic*)m_pWnd->GetChild( _T("t_of_target\\name") );

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
	strPath = g_strLocalPath + _T("\\ui\\systemexpand.xml");
	XmlElement eleMenu;
	if( !m_pGUI->LoadXml( &eleMenu, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &eleMenu, "expand", "Name", "TargetMenu");
	tagPoint ptWnd = m_pWnd->GetClient();
	stringstream streamX,streamY;
	streamX<<m_pWnd->GetClient().x + m_pWnd->GetSize().x;
	streamY<<m_pWnd->GetClient().y + 20;
	m_pGUI->ChangeXml( &eleMenu, "TargetMenu", "AlignControl", "\\desktop");
	m_pGUI->ChangeXml( &eleMenu, "TargetMenu", "AlignType", "NoAlign");
	m_pGUI->ChangeXml( &eleMenu, "TargetMenu\\listbutton1", "AlignControl", "\\desktop\\TargetMenu");
	m_pGUI->ChangeXml( &eleMenu, "TargetMenu", "LogicPos_x", streamX.str().c_str() );
	m_pGUI->ChangeXml( &eleMenu, "TargetMenu", "LogicPos_y", streamY.str().c_str() );

	m_pWnd_Menu	= m_pGUI->CreateWnd( _T("\\desktop"), &eleMenu );
	m_pGUI->RegisterEventHandler(m_pWnd_Menu->GetFullName().c_str(), m_Trunk.sfp1(&TargetStateFrame::GUIEventHandler));

	m_pLb_Menu = (GUIListBox*)m_pWnd_Menu->GetChild( _T("listbutton1") );
	m_nMenuPanelHeightExt = m_pWnd_Menu->GetSize().y - m_pLb_Menu->GetSize().y;
	m_pLb_Menu->SetColWidth(0, m_pLb_Menu->GetSize().y);

	m_pGUI->ChangeXml( &eleMenu, "TargetMenu", "Name", "FashionMenu");
	m_pGUI->ChangeXml( &eleMenu, "FashionMenu\\listbutton1", "AlignControl", "\\desktop\\FashionMenu");
	ptWnd = m_pWnd_Menu->GetClient();
	stringstream streamXx,streamYy;
	streamXx<<m_pWnd_Menu->GetClient().x + m_pWnd_Menu->GetSize().x;
	streamYy<<m_pWnd_Menu->GetClient().y;
	m_pGUI->ChangeXml( &eleMenu, "FashionMenu", "LogicPos_x", streamXx.str().c_str() );
	m_pGUI->ChangeXml( &eleMenu, "FashionMenu", "LogicPos_y", streamYy.str().c_str() );
	m_pWnd_SuitMenu	= m_pGUI->CreateWnd( _T("\\desktop"), &eleMenu );
	m_pLb_SuitMenu = (GUIListBox*)m_pWnd_SuitMenu->GetChild( _T("listbutton1") );

	tagPoint ptSizeListBox = m_pLb_SuitMenu->GetSize();
	ptSizeListBox.y = m_pLb_SuitMenu->GetRowHeight() * 0;
	ptSizeListBox.x = 200;

	GUIWnd* pFather = m_pLb_SuitMenu->GetFather();
	if( P_VALID( pFather ) )
	{
		pFather->SetResizable( TRUE );
		tagPoint ptSizeFather = pFather->GetSize();
		ptSizeFather.y = ptSizeListBox.y + m_nMenuPanelHeightExt;
		ptSizeFather.x = 200;
		pFather->Resize( ptSizeFather );
		pFather->SetResizable( FALSE );
	}
	m_pLb_SuitMenu->SetResizable( TRUE );
	m_pLb_SuitMenu->Resize( ptSizeListBox );
	m_pLb_SuitMenu->SetResizable( FALSE );

	m_pLb_SuitMenu->SetColWidth(0, m_pLb_SuitMenu->GetSize().x);
	m_pWnd_SuitMenu->SetInvisible(TRUE);
	m_pGUI->RegisterEventHandler(m_pWnd_SuitMenu->GetFullName().c_str(), m_Trunk.sfp1(&TargetStateFrame::GUIEventHandler));

	m_pWnd_Menu->SetInvisible( TRUE );
	m_pSt_PVP->SetInvisible( TRUE );
	m_pWnd->SetInvisible( TRUE );
	
	m_pHead->FlipToTop();
	m_pHead->SetTopMost( TRUE );
	m_pHead->SetUnClickable(true);

	m_pSt_TofTHeadPic->FlipToTop();
	m_pSt_TofTHeadPic->SetTopMost(TRUE);

	m_nLastLevel = GT_INVALID;
	m_nLastMaxHp = GT_INVALID;
	m_nLastHp	 = GT_INVALID;
	m_nLastMaxMp = GT_INVALID;
	m_nLastMp	 = GT_INVALID;

	return TRUE;
}

VOID TargetStateFrame::SafeDestroyUI()
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

	if( P_VALID( m_pWnd_SuitMenu ) )
	{
		m_pGUI->DestroyWnd( m_pWnd_SuitMenu );
		m_pWnd_SuitMenu = NULL;
	}
}

VOID TargetStateFrame::Update()
{
	if( GT_INVALID == m_dwCurShowRoleID )
	{
		if (m_bNeedToSyncNoTarget)
		{
			// 告诉服务器我现在没有看任何人
			Role* plp = RoleMgr::Inst()->GetLocalPlayer();
			if (P_VALID(plp))
			{
				tagNC_TargetOfTarget cmd;
				cmd.dwPlayerID = plp->GetID();
				cmd.dwTargetID = GT_INVALID;
				TObjRef<NetSession>()->Send(&cmd);
			}
		}
		m_bNeedToSyncNoTarget = false;
		return;
	}
		

	Role* pRole = RoleMgr::Inst()->FindRole( m_dwCurShowRoleID );
	if( P_VALID( pRole ) && !( pRole->IS_KIND_OF( NPC ) && pRole->IsDead() ))
	{
 		m_bNeedToSyncNoTarget = true;
 		UpdataRoleData( pRole );
 		dwFormerShowRoleID = m_dwCurShowRoleID;
 
// 		m_dwCurTargetRemainTime += Kernel::Inst()->GetDeltaTimeDW();
// 		if(m_dwCurTargetRemainTime > MIN_ACQUIRE_TARGETsTARGET_TIME )
// 		{
// 			m_dwCurTargetRemainTime = 0;
// 
// 			Role* plp = RoleMgr::Inst()->GetLocalPlayer();
// 			if (P_VALID(plp))
// 			{
// 				tagNC_TargetOfTarget cmd;
// 				cmd.dwPlayerID = plp->GetID();
// 				cmd.dwTargetID = m_dwCurShowRoleID;
// 				TObjRef<NetSession>()->Send(&cmd);
// 			}
// 		}
	}
	else
	{
		////NPC死亡，从列表中删除对应的NPC ID
		//std::list<DWORD>::iterator  vecOwnNPCListIter = m_vecOwnNPCList.begin();
		//for ( ; vecOwnNPCListIter != m_vecOwnNPCList.end(); vecOwnNPCListIter++)
		//{
		//	if ( m_vecOwnNPCList.empty() )
		//	{
		//		break;
		//	}
		//	else if ( (*vecOwnNPCListIter) == dwFormerShowRoleID )
		//	{
		//		m_vecOwnNPCList.erase(vecOwnNPCListIter);
		//		break;
		//	}
		//}
		// 
		// 角色出了视野或死亡NPC，取消选中
		tagRolePickEvent evt( _T("tagRolePickEvent"), this );
		evt.eType = EWPT_LClick;
		evt.dwRoleID = GT_INVALID;
		m_pMgr->SendEvent( &evt );	
	}

	UpdateFashionData();
	// 防止滚动菜单
	if( 0 != m_pLb_Menu->GetCurrentTextRow() )
		m_pLb_Menu->SetCurrentTextRow( 0 );
	if( 0 != m_pLb_SuitMenu->GetCurrentTextRow() )
		m_pLb_SuitMenu->SetCurrentTextRow( 0 );

	GameFrame::Update();
}

VOID TargetStateFrame::Render3D()
{
	if( P_VALID( m_pHead ) )
		m_pHead->Render3D();

	if( P_VALID( m_pSt_TofTHeadPic ) )
		m_pSt_TofTHeadPic->Render3D();
}

BOOL TargetStateFrame::EscCancel()
{
	if( !P_VALID( m_pWnd ) || m_pWnd->IsInvisible() )
		return FALSE;

	tagRolePickEvent evt( _T("tagRolePickEvent"), this );
	evt.eType = EWPT_LClick;
	evt.dwRoleID = GT_INVALID;
	m_pMgr->SendEvent( &evt );

	return TRUE;
}

DWORD TargetStateFrame::OnRolePickEvent(tagRolePickEvent* pEvent)
{
	if( GT_INVALID != pEvent->dwRoleID )
		OnOpen( pEvent->dwRoleID );
	else
		OnClose();

	return 0;
}

DWORD TargetStateFrame::OnRoleGetNameEvent(tagRoleRenameEvent* pEvent)
{
	if( GT_INVALID != pEvent->dwRoleID && m_dwCurShowRoleID == pEvent->dwRoleID )
			m_pSt_Name->SetText( pEvent->strRoleName.c_str() );

	if( GT_INVALID != m_dwCurTargetID && m_dwCurTargetID == pEvent->dwRoleID )
		m_pSt_TofTName->SetText( pEvent->strRoleName.c_str() );

	return 0;
}

DWORD TargetStateFrame::OnUpdataAvatarEvent(tagUpdataAvatarEvent* pEvent)
{
	if( GT_INVALID != m_dwCurShowRoleID && pEvent->dwRoleID == m_dwCurShowRoleID )
	{
		Role* pRole = RoleMgr::Inst()->FindRole( pEvent->dwRoleID );
		if( P_VALID( pRole ) && pRole->IsAttributeInited() )
			m_pHead->UpdataHeadPic( pRole );
	}

	if( GT_INVALID != m_dwCurTargetID && pEvent->dwRoleID == m_dwCurTargetID )
	{
		Role* pRole = RoleMgr::Inst()->FindRole( pEvent->dwRoleID );
		if( P_VALID( pRole ) && pRole->IsAttributeInited() )
			m_pSt_TofTHeadPic->UpdataHeadPic( pRole );
	}
	return 0;
}

DWORD TargetStateFrame::OnRoleBuffChangeEvent(tagRoleBuffChangeEvent* pEvent)
{
	if( GT_INVALID != m_dwCurShowRoleID && pEvent->dwRoleID == m_dwCurShowRoleID )
	{
		Role* pRole = RoleMgr::Inst()->FindRole( m_dwCurShowRoleID );
		if( P_VALID( pRole ) )
			RebuildBuffs( pRole );
	}
	return 0;
}

VOID TargetStateFrame::RebuildBuffs( Role* pRole )
{
	ASSERT( P_VALID( pRole ) );

	m_pBuffUIList->ClearBuffs();
	tagRoleBuff* pBuff = NULL;
	TList<tagRoleBuff*>& listBuff = pRole->GetBuffList();
	listBuff.ResetIterator();
	while( listBuff.PeekNext(pBuff) )
	{
		if( !P_VALID( pBuff ) )
			continue;
		if( !m_pBuffUIList->AddBuff( pBuff ) )
			break;
	}
}

BOOL TargetStateFrame::GUIEventHandler(tagGUIEvent* pEvent)
{
	const GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID( pWnd ) )
		return FALSE;
	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pSt_TofTHeadPic || pWnd == m_pSt_TofTBack)
			{
				if (m_dwCurTargetID != GT_INVALID && m_dwCurTargetID != 0)
				{
					if(!RoleMgr::Inst()->AutoPickRole(m_dwCurTargetID))
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TargetOfTarget_CantSwitch")]);
				}
			}
		}
	case EGUIE_Deactive:
		if( !m_pWnd_Menu->IsInvisible() &&
			pEvent->dwParam1 != (DWORD)m_pWnd_Menu &&
			pEvent->dwParam1 != (DWORD)m_pLb_Menu &&
			pEvent->dwParam1 != (DWORD)m_pWnd_SuitMenu &&
			pEvent->dwParam1 != (DWORD)m_pLb_SuitMenu )
		{
			m_pWnd_Menu->SetInvisible( TRUE );
			m_pWnd_SuitMenu->SetInvisible( TRUE );
			m_pBn_Menu->SetState( EGUIBS_PopUp );
		}
		break;
	case EGUIE_PushButtonDown:
		if( pWnd == m_pBn_Menu )
		{
			if( 1 == pEvent->dwParam1 )
			{
				m_pWnd_Menu->SetInvisible( FALSE );
				UpdataMenuItem();
			}
			else
			{
				m_pWnd_Menu->SetInvisible( TRUE );
			}
		}
		break;
	case EGUIE_ItemClick:
		if( pWnd == m_pLb_Menu )
		{
			if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_PrivateChat] )
			{
				tagPrivateChatEvent evt( _T("tagPrivateChatEvent"), this );
				evt.dwRoleID = m_dwCurShowRoleID;
				evt.strRoleName = m_pSt_Name->GetText();
				m_pMgr->SendEvent( &evt );
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_Friend] )
			{
				tagAddFriendEvent evt( _T("SocialEvent_AddFriend"), this );
				evt.roleID = m_dwCurShowRoleID;
				m_pMgr->SendEvent( &evt );
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_Trade] )
			{
				tagFollowRoleEvent evt( _T("TargetEvent_ReqTrade"), this );
				evt.dwTargetRoleID = m_dwCurShowRoleID;
				m_pMgr->SendEvent( &evt );
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_Follow] )
			{
				tagFollowRoleEvent evt( _T("tagFollowRoleEvent"), this );
				evt.dwTargetRoleID = m_dwCurShowRoleID;
				m_pMgr->SendEvent( &evt );
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_TeamInvite] )
			{
				TeamSys::Inst()->InviteJoinTeam( m_dwCurShowRoleID );
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_TeamApply] )
			{
				//TeamSys::Inst()->ApplyJoinTeam( m_dwCurShowRoleID );
				tagApplyJoinTeamEvent evt( _T("tagApplyJoinTeamEvent"), this );
				evt.dwRoleID = m_dwCurShowRoleID;
				m_pMgr->SendEvent( &evt );

				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_FactionInvite] )
			{
				GuildMgr::Inst()->SendGuildJoinReq( m_dwCurShowRoleID );
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_PlayerInfo] )
			{
                tagDispPlayerState e(_T("tagDispPlayerState"), this);
                e.dwRoleID = m_dwCurShowRoleID;
                m_pMgr->SendEvent(&e);
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_PlayerInfo2] )
			{
				tagOpenRoleInfoEvent e(_T("Open_RoleInfo"), this);
				e.dwRoleID = m_dwCurShowRoleID;
				m_pMgr->SendEvent(&e);
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
            else if (pEvent->dwParam1 == m_nCurMenuIndex[EMI_ApprenticeToMaster])
            {
                tagGetAquireMasterEvent e(_T("tagGetAquireMasterEvent"), this);
                e.roleID = m_dwCurShowRoleID;
                m_pMgr->SendEvent(&e);

                m_pWnd_Menu->SetInvisible( TRUE );
                m_pBn_Menu->SetState( EGUIBS_PopUp );
            }
            else if (pEvent->dwParam1 == m_nCurMenuIndex[EMI_ToHaveApprentice])
            {
                tagAquireApprenticeEvent e(_T("tagAquireApprenticeEvent"), this);
                e.roleID = m_dwCurShowRoleID;
                m_pMgr->SendEvent(&e);

                m_pWnd_Menu->SetInvisible( TRUE );
                m_pBn_Menu->SetState( EGUIBS_PopUp );
            }
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_AddToBlackList] )
			{
				tagAddBlackEvent evt( _T("SocialEvent_AddBlack"), this );
				evt.roleID = m_dwCurShowRoleID;
				m_pMgr->SendEvent( &evt );
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
            else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_PetTrade] )
            {
				tagFollowRoleEvent evt( _T("TargetEvent_ReqPetTrade"), this );
				evt.dwTargetRoleID = m_dwCurShowRoleID;
				m_pMgr->SendEvent( &evt );
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
            }
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMT_MountInvite] )//发送邀请驾御消息
			{
				CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame(_T("CombatSys"));
				tagNC_MountInvite msg;
				msg.dwDstRoleID	= pCombatSys->GetCurTargetID();
				TObjRef<NetSession>()->Send(&msg);
				m_pWnd_Menu->SetInvisible( TRUE );
				m_pBn_Menu->SetState( EGUIBS_PopUp );
			}
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_MiniChat])
			{
				CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame( _T("CombatSys") );
				tagOpenMiniChatEvent evt(_T("tagOpenMiniChatEvent"), this);
				DWORD temp = pCombatSys->GetCurTargetID();
				evt.name = PlayerNameTab::Inst()->FindNameByID(pCombatSys->GetCurTargetID());
				m_pMgr->SendEvent(&evt);
			}
			else if( pEvent->dwParam1 == m_nCurMenuIndex[EMI_ApplySpritePart])
			{
				SpritePartnershipFrame *pFrame = (SpritePartnershipFrame*)TObjRef<GameFrameMgr>()->GetFrame( _T("SpritePartnership") );
				if( P_VALID(pFrame) )
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoMating_OnceOnePlayer")]);
				}
				else
				{
					CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame( _T("CombatSys") );
					tagNC_ApplyFairyBirth cmd;
					cmd.dwRoleID = pCombatSys->GetCurTargetID();
					TObjRef<NetSession>()->Send(&cmd);
					m_pWnd_Menu->SetInvisible( TRUE );
					m_pBn_Menu->SetState( EGUIBS_PopUp );
				}
			}
			else if( pEvent->dwParam1  == m_nCurMenuIndex[EMI_InviteFamily] )
			{
				tagFamilyInviteEvent evt(_T("FamilyEvent_Invite"), this);
				evt.dwRoleID	= m_dwCurShowRoleID;
				evt.strRoleName	= PlayerNameTab::Inst()->FindNameByID(m_dwCurShowRoleID);
				m_pMgr->SendEvent(&evt);
			}
		}
		else if( pWnd == m_pLb_SuitMenu )
		{
			const INT nRow = m_pLb_SuitMenu->GetCurSelectedRow();
			if(nRow < 0)
				break;
			DWORD dwSuitID = m_pLb_SuitMenu->GetUserData( nRow );
			tagNC_DressUpAnotherRole cmd;
			cmd.dwTargetRoleID = m_dwCurShowRoleID;
			cmd.dwWardrobeLayerID = dwSuitID;
			TObjRef<NetSession>()->Send(&cmd);
			m_pWnd_SuitMenu->SetInvisible( TRUE );
			m_pWnd_Menu->SetInvisible( TRUE );
			m_pBn_Menu->SetState( EGUIBS_PopUp );
		}
		break;
	default:
		break;
	}

	m_pBuffUIList->GUIEventHandler( pEvent, pWnd, m_dwCurShowRoleID );
	return FALSE;
}


VOID TargetStateFrame::UpdataMenuItem()
{
	for( int i = 0; i < EMI_Num; i++ )
	{
		m_nCurMenuIndex[i] = GT_INVALID;
	}
	INT nMenuIndex = 0;
	SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_PrivateChat );
	m_nCurMenuIndex[EMI_PrivateChat] = nMenuIndex++;

	SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_Friend );
	m_nCurMenuIndex[EMI_Friend] = nMenuIndex++;

	SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_Trade );
	m_nCurMenuIndex[EMI_Trade] = nMenuIndex++;

	SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_Follow );
	m_nCurMenuIndex[EMI_Follow] = nMenuIndex++;

 	SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_ApplySpritePart );
 	m_nCurMenuIndex[EMI_ApplySpritePart] = nMenuIndex++;

	// 装扮好友
	Role *pRole = RoleMgr::Inst()->FindRole(m_dwCurShowRoleID);
	if(P_VALID(pRole))
	{
		if (IS_PLAYER(pRole->GetID()))
		{
			Player* pPlayer = static_cast<Player*>(pRole);
			vector<DWORD> vecFashion;
			bool bMan =false;
			if(pPlayer->GetPlayerSex())
				bMan = true;
			ItemMgr::Inst()->GetAllSuitableFashion( bMan, vecFashion);
			if(!vecFashion.empty())
			{
				SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_BeautyFriends );
				m_nCurMenuIndex[EMI_BeautyFriends] = nMenuIndex++;
			}
		}
	}

	//如果选中的不是本地玩家,选中的是远程玩家
	if( RoleMgr::Inst()->GetLocalPlayerID() != m_dwCurShowRoleID )
	{
		//本地玩家没有队伍 或者 是队长可以邀请其他玩家加入队伍
		if( TeamSys::Inst()->GetTeammateNum() == 0 || 
			( TeamSys::Inst()->IsLeader( RoleMgr::Inst()->GetLocalPlayerID() ) && !TeamSys::Inst()->IsTeamFull() && !TeamSys::Inst()->IsTeammate( m_dwCurShowRoleID ) ) )
		{
			SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_TeamInvite );
			m_nCurMenuIndex[EMI_TeamInvite] = nMenuIndex++;
		}

		//本地玩家没有队伍，可以申请加入其他队伍
		if( !TeamSys::Inst()->IsInTeam() )
		{
			SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_TeamApply );
			m_nCurMenuIndex[EMI_TeamApply] = nMenuIndex++;
		}
	}
	
	if( GuildMgr::Inst()->IsCanJionReq( m_dwCurShowRoleID ))// 如果可以邀请该玩家加入帮派，则显示帮派邀请
	{
		SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_FactionInvite );
		m_nCurMenuIndex[EMI_FactionInvite] = nMenuIndex++;
	}

	SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_PlayerInfo );
	m_nCurMenuIndex[EMI_PlayerInfo] = nMenuIndex++;

	SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_PlayerInfo2 );
	m_nCurMenuIndex[EMI_PlayerInfo2] = nMenuIndex++;

	SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_AddToBlackList );
	m_nCurMenuIndex[EMI_AddToBlackList] = nMenuIndex++;

    SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_PetTrade );
	m_nCurMenuIndex[EMI_PetTrade] = nMenuIndex++;

    //SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_ApprenticeToMaster );
    //m_nCurMenuIndex[EMI_ApprenticeToMaster] = nMenuIndex++;

    //SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_ToHaveApprentice );
    //m_nCurMenuIndex[EMI_ToHaveApprentice] = nMenuIndex++;

	// 正在驾御灵兽，当前驾御人数少于可驾御人数，被邀请的对象在自己周围10格之内
	Pet *pPet = RoleMgr::Inst()->GetLocalPlayer()->GetMount();
	Role *pTarget = NULL;
	CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame(_T("CombatSys"));
	map<DWORD,Role*>::const_iterator finder = RoleMgr::Inst()->GetRoleMap().find(pCombatSys->GetCurTargetID());
	if( finder != RoleMgr::Inst()->GetRoleMap().end() )
		pTarget = finder->second;
	if( P_VALID(pPet) 
		&& (PetManager::Inst()->GetPetCurrMountNum(pPet->GetID()) < PetManager::Inst()->GetPetMaxMountNum(pPet->GetID()))
		&& P_VALID(pTarget) && pTarget->IS_STATIC_CLASS(Player) )
	{
		if( Vec3DistSq( RoleMgr::Inst()->GetLocalPlayer()->GetPos(), pTarget->GetPos() ) < 250000 )
		{
			SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMT_MountInvite );
			m_nCurMenuIndex[EMT_MountInvite] = nMenuIndex++;
		}
	}

	SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_MiniChat );  //密语聊天
	m_nCurMenuIndex[EMI_MiniChat] = nMenuIndex++;

	LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
	if ( P_VALID(pLp) && pLp->IsInFamily() )
	{
		SetMenuFromStrTable( m_pLb_Menu, _T("TargetMenu_"), nMenuIndex, EMI_InviteFamily );   //家族邀请
		m_nCurMenuIndex[EMI_InviteFamily] = nMenuIndex++;
	}

	ForceResizeMenu( m_pLb_Menu, nMenuIndex );
}

VOID TargetStateFrame::OnOpen( const DWORD dwRoleID )
{
	if( dwRoleID == m_dwCurShowRoleID )
		return;

	OnClose();

	Role* pRole = RoleMgr::Inst()->FindRole( dwRoleID );
	if( !P_VALID( pRole ) || ( !pRole->IsAttributeInited() ) )
		return;
	if( pRole->IS_KIND_OF( NPC ) )
	{
		NPC* pNpc = (NPC*)pRole;
		if( !pNpc->IsMonster() && !pNpc->IsNPC() )
			return;
	}
	m_pWnd->SetInvisible( FALSE );
	m_pHead->UpdataHeadPic( pRole );

	UpdataRoleData( pRole );
	
	m_dwCurShowRoleID = dwRoleID;
	if( IS_PLAYER( m_dwCurShowRoleID ) )
	{
		m_pSt_AppendInfo->SetInvisible( FALSE );

		m_pSt_PVP->SetInvisible( TRUE );
		if( RoleMgr::Inst()->GetLocalPlayer()->GetID() == m_dwCurShowRoleID )
			m_pBn_Menu->SetEnable( false );
		else
			m_pBn_Menu->SetEnable( true );
		if (CombatActionChecker::Inst()->IfCanAttack(dwRoleID))
			m_dwSelectEffectID = EffectMgr::Inst()->PlayRoleEffect( dwRoleID, _T("Txqt01_2"), NULL, EffectMgr::EEST_XZ );
		else
			m_dwSelectEffectID = EffectMgr::Inst()->PlayRoleEffect( dwRoleID, _T("Txqt01_1"), NULL, EffectMgr::EEST_XZ );
	}
	else
	{
		m_pSt_AppendInfo->SetInvisible( TRUE );
		m_pSt_PVP->SetInvisible( TRUE );	
		m_pBn_Menu->SetEnable( false );
		if( pRole->IS_KIND_OF( NPC ) )
		{
			NPC* pNpc = (NPC*)pRole;
			if( pNpc->IsMonster() )
				m_dwSelectEffectID = EffectMgr::Inst()->PlayRoleEffect( dwRoleID, _T("Txqt01_2"), NULL, EffectMgr::EEST_XZ );
			else
				m_dwSelectEffectID = EffectMgr::Inst()->PlayRoleEffect( dwRoleID, _T("Txqt01_1"), NULL, EffectMgr::EEST_XZ );
		}
	}
	// 设置职业
	SetPlayerClassName(m_dwCurShowRoleID);
	RebuildBuffs( pRole );

	// ---目标的目标
	m_pSt_TofTBackAll->SetInvisible(true);
	LocalPlayer *plp = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(plp))
	{
		tagNC_TargetOfTarget cmd;
		cmd.dwPlayerID = plp->GetID();
		cmd.dwTargetID = dwRoleID;
		TObjRef<NetSession>()->Send(&cmd);
		m_dwCurTargetRemainTime = 0;
	}
}

VOID TargetStateFrame::OnClose()
{
	m_dwCurShowRoleID = GT_INVALID;
	m_dwCurTargetID = GT_INVALID;
	m_pSt_TofTName->SetText(_T(""));
	m_pSt_TofTBackAll->SetInvisible(true);
	m_pWnd->SetInvisible( TRUE );
	m_pWnd_Menu->SetInvisible( TRUE );
	EffectMgr::Inst()->StopEffect( m_dwSelectEffectID );
	m_dwSelectEffectID = GT_INVALID;

	// 恢复名字颜色
	m_pSt_Name->SetTextColor( 0xFFFFF7E0 );
	m_bLastNameGray = false;
}

VOID TargetStateFrame::UpdataPlayerData( Player* pPlayer )
{
	// PVP图标
	m_pSt_PVP->SetInvisible( !pPlayer->GetRoleState( ERS_PVP ) );
	m_pSt_Belong->SetInvisible(TRUE);
}

VOID TargetStateFrame::UpdataNPCData( NPC* pNpc )
{
	const bool bGray = !pNpc->IsBelongToLP();
	if( bGray != m_bLastNameGray )
	{
		m_pSt_Name->SetTextColor( bGray ? 0xFFC0C0C0 : 0xFFFFF7E0 );
		m_pSt_Name->SetRefresh( TRUE );
		m_bLastNameGray = bGray;
	}
	
	const tagCreatureProto *pProto = CreatureData::Inst()->FindNpcAtt(pNpc->GetTypeID());
	if (P_VALID(pProto) )
	{
		if (pProto->eType == ECT_Monster)
		{
			switch(pProto->eTaggedType)
			{
			case ECTT_Hit:
				m_pSt_Belong->SetText(g_StrTable[_T("Monster_Hit")]);
				break;
			case ECTT_Kill:
				m_pSt_Belong->SetText(g_StrTable[_T("Monster_Kill")]);
				break;
			case ECTT_All:
				m_pSt_Belong->SetText(g_StrTable[_T("Monster_All")]);
				break;
			case ECIT_Enmity:
				m_pSt_Belong->SetText(g_StrTable[_T("Monster_Enmity")]);
				break;
			default:
				m_pSt_Belong->SetText(g_StrTable[_T("Monster_Hit")]);
				break;
			}
			m_pSt_Belong->SetInvisible(false);
		}
		else
		{
			m_pSt_Belong->SetInvisible(true);
			m_pSt_Belong->SetText(_T(""));
		}

		switch (pProto->eLite)
		{
		case ER_Normal:
			{
				m_pSt_AI->SetTextColor(0xFFFFF2A4);
				m_pSt_AI->SetText(g_StrTable[_T("ERank1")]);
			} break;
		case ER_Elite:   // 精英
			{
				m_pSt_AI->SetTextColor(0xFFFFFF00);
				m_pSt_AI->SetText(g_StrTable[_T("ERank2")]);
			} break;
		case ER_Boss:    // 头目
			{
				m_pSt_AI->SetTextColor(0xFFFF8040);
				m_pSt_AI->SetText(g_StrTable[_T("ERank3")]);
			} break;
		case ER_BigBoss: // 首领
			{
				m_pSt_AI->SetTextColor(0xFF1FDFE9);
				m_pSt_AI->SetText(g_StrTable[_T("ERank4")]);
			} break;
		case ER_Rare:	 // 稀有
			{
				m_pSt_AI->SetTextColor(0xFFFCACFF);
				m_pSt_AI->SetText(g_StrTable[_T("ERank5")]);
			} break;
		default: break;
		}
	}

		

	m_pSt_AI->SetRefresh();
	m_pSt_Belong->SetRefresh();
	//m_pSt_Belong->SetInvisible(FALSE);
}

VOID TargetStateFrame::UpdataRoleData( Role* pRole )
{
	ASSERT( P_VALID( pRole ) );

	// 显示/隐藏 HP MP文字
	if( GameSet::Inst()->Get( EGS_ShowHPMPText ) )
	{
		if( m_pPro_Hp->GetText().empty() )
		{
			m_nLastMaxHp = GT_INVALID;
			m_nLastHp	 = GT_INVALID;
			m_nLastMaxMp = GT_INVALID;
			m_nLastMp	 = GT_INVALID;
		}
	}
	else
	{
		if( !m_pPro_Hp->GetText().empty() )
		{
			m_pPro_Hp->SetText( _T("") );
			m_pPro_Mp->SetText( _T("") );

			// 文字超出了控件，所以画布也需要刷新
			m_pPro_Mp->SetRefresh( TRUE );
		}
	}

	DWORD targetID =0;
	if( pRole->IS_KIND_OF(Player) )
	{
		UpdataPlayerData( (Player*)pRole );
	}
	else if( pRole->IS_KIND_OF(NPC) )
	{
		UpdataNPCData( (NPC*)pRole );
	}


	if(m_dwCurTargetID != GT_INVALID && !m_pSt_TofTBackAll->IsInvisible() && m_pSt_TofTName->GetText() == _T(""))
		UpdataTargetName();
	// 角色名字
	DWORD dwRoleState =	RoleMgr::Inst()->GetLocalPlayer()->GetRoleState();
	if( pRole->GetID() != m_dwCurShowRoleID || m_bIfRevServerInfo == TRUE )
	{
		if( pRole->GetID() == m_dwCurShowRoleID )
		{
			m_bIfRevServerInfo = FALSE;
		}

		BOOL bIsInList = FALSE;
		std::list<DWORD>::iterator  vecOwnNPCListIter = m_vecOwnNPCList.begin();
		for ( ; vecOwnNPCListIter != m_vecOwnNPCList.end(); vecOwnNPCListIter++)
		{
			if ( pRole->GetID() == (*vecOwnNPCListIter) )
			{
				bIsInList = TRUE;
				break;
			}
		}

		TCHAR szBuff[X_SHORT_NAME] = {0};
		//如果是NPC（怪物）并且在战斗改变头像显示图标和字体
		if( pRole->IS_KIND_OF(NPC) && bIsInList == TRUE)
		{		
			NPC* pNPC = (NPC*)pRole;
			//属于本地玩家(包括怪物仇恨列表清0)
			if ( pNPC->IsBelongToLP()) 
			{
				//战斗状态
				if ( dwTaggedID != GT_INVALID  )
				{
					m_pHead->SetColorGray( FALSE );
					m_pSt_Name->SetTextColor( 0xFFFFF7E0 );
					tstring tempTStr = g_StrTable[_T("BelongTo")];
					tstring tempTStrBuff = pRole->GetRoleName().c_str();
					tempTStrBuff = tempTStrBuff+tempTStr;
					_stprintf( szBuff, tempTStrBuff.c_str() );
				} 
				//脱战状态
				else
				{
					m_pHead->SetColorGray( FALSE );
					m_pSt_Name->SetTextColor( 0xFFFFF7E0 );
					_stprintf(szBuff, pRole->GetRoleName().c_str());
				}
			}
			//不属于本地玩家
			else
			{
				m_pHead->SetColorGray( TRUE );
				m_pSt_Name->SetTextColor( 0xFFC0C0C0 );
				tstring tempTStr = g_StrTable[_T("NotBelongTo")];
				tstring tempTStrBuff = pRole->GetRoleName().c_str();
				tempTStrBuff = tempTStrBuff+tempTStr;
				_stprintf( szBuff, tempTStrBuff.c_str() );
			}
		}
		//选取的不是NPC或者NPC尚未战斗
		else
		{
			m_pHead->SetColorGray( FALSE );
			m_pSt_Name->SetTextColor( 0xFFFFF7E0 );
			_stprintf(szBuff, pRole->GetRoleName().c_str());
		}
		m_pSt_Name->SetText( szBuff );
		m_pSt_Name->SetRefresh( TRUE );
	}

	const int nLevel    = pRole->GetRoleLevel();
	if( nLevel != m_nLastLevel )
	{
		tstringstream stream;
		stream<<nLevel;
		m_pSt_Level->SetText( stream.str().c_str() );
		m_pSt_Level->SetRefresh( TRUE );
		m_nLastLevel = nLevel;
	}

	const int nMaxHp	= pRole->GetAttribute( ERA_MaxHP );
	const int nHp		= pRole->GetAttribute( ERA_HP );
	const int nMaxMp	= pRole->GetAttribute( ERA_MaxMP );
	const int nMp		= pRole->GetAttribute( ERA_MP );
	if( nMaxHp != m_nLastMaxHp || nHp != m_nLastHp )
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
		m_nLastMaxHp = nMaxHp;
		m_nLastHp	 = nHp;
	}

	if( nMaxMp != m_nLastMaxMp || nMp != m_nLastMp )
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
		m_nLastMaxMp = nMaxMp;
		m_nLastMp    = nMp;
	}
	m_pBuffUIList->Updata();
}

VOID TargetStateFrame::ForceResizeMenu( GUIListBox* pListBox, const INT nNumRow )
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

VOID TargetStateFrame::SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nLbIndex, const INT nStrIndex )
{
	tstringstream stream;
	stream<<szBaseKey<<nStrIndex;
	tstring strText = g_StrTable[ stream.str().c_str() ];
	pListBox->SetText( nLbIndex, 0, strText.c_str() );
}


VOID TargetStateFrame::SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nNumRow, const INT nStartRow, const INT nEndRow, const BOOL bAutoResize )
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

DWORD TargetStateFrame::OnUpdataAvatarDisplaySetEvent(tagUpdataAvatarDisplaySetEvent* pEvent)
{
	if( pEvent->dwRoleID == m_dwCurShowRoleID )
	{
		Role* pRole = RoleMgr::Inst()->FindRole( pEvent->dwRoleID );
		if( P_VALID( pRole ) && pRole->IS_KIND_OF(Player) )
		{
			Player* pPlayer = (Player*)pRole;
			m_pHead->UpdataDisplaySet( pPlayer->GetDisplaySet() );
		}
	}

	if( pEvent->dwRoleID == m_dwCurTargetID )
	{
		Role* pRole = RoleMgr::Inst()->FindRole( pEvent->dwRoleID );
		if( P_VALID( pRole ) && pRole->IS_KIND_OF(Player) )
		{
			Player* pPlayer = (Player*)pRole;
			m_pSt_TofTHeadPic->UpdataDisplaySet( pPlayer->GetDisplaySet() );
		}
	}
	return 0;
}

VOID TargetStateFrame::SetPlayerClassName( DWORD dwRoleID )
{
    Role* pRole = RoleMgr::Inst()->FindRole(dwRoleID);
	if( pRole )
	{
		if (pRole->IS_KIND_OF(Player))
		{
			Player* pPlayer = static_cast<Player*>(pRole);
			m_pSt_AppendInfo->SetText(ProfessionMgr::Inst().GetClassName(pPlayer->GetPlayerClass()));
		}
		else if( pRole->IS_KIND_OF(NPC) )
		{
			NPC *pNpc = static_cast<NPC*>(pRole);
			if( pNpc->GetProto()->szMonsterAI != NULL )
			{
				m_pSt_AppendInfo->SetInvisible( FALSE );
				m_pSt_AppendInfo->SetText( pNpc->GetProto()->szMonsterAI );
				m_pSt_AppendInfo->SetRefresh(TRUE);
			}
		}
	}
}

DWORD TargetStateFrame::OnChangePlayerClass( tagGameEvent* pEvent )
{
    if (m_pWnd->IsInvisible())
        return 0;

    SetPlayerClassName(m_dwCurShowRoleID);

    return 0;
}

DWORD TargetStateFrame::CreatureTaggedOwnerChange(tagNS_CreatureTaggedOwnerChange* pNetCmd, DWORD)
{
	dwCreateId = pNetCmd->dwCreatureID;
	dwTaggedID = pNetCmd->dwTaggedOwner;
	m_bIfRevServerInfo = TRUE;


	DWORD dwRoleID =	RoleMgr::Inst()->GetLocalPlayer()->GetID();

	std::list<DWORD>::iterator  vecOwnNPCListIter1 = m_vecOwnNPCList.begin();
	for ( ; vecOwnNPCListIter1 != m_vecOwnNPCList.end(); vecOwnNPCListIter1++)
	{
		if ( (*vecOwnNPCListIter1) == dwCreateId )
		{
			break;
		}
	}
	if ( vecOwnNPCListIter1 == m_vecOwnNPCList.end() || m_vecOwnNPCList.empty())
	{
		m_vecOwnNPCList.push_back(dwCreateId);
	}
	
	return 0;

}

VOID TargetStateFrame::UpdataTargetofTarget_NPC(DWORD roleID)
{
	if (roleID == 0 || roleID == GT_INVALID)
		return;

	Role* pRole = RoleMgr::Inst()->FindRole( roleID );
	if( P_VALID( pRole ) && !( pRole->IS_KIND_OF( NPC ) && pRole->IsDead() ))
	{	
		m_pSt_TofTName->SetText(_T(""));
		m_dwCurTargetID = roleID;
		m_pSt_TofTHeadPic->UpdataHeadPic( pRole );
		m_pSt_TofTBack->SetRefresh(true);
	}
	else
	{
		m_dwCurTargetID = GT_INVALID;
	}
}

VOID TargetStateFrame::UpdataTargetName()
{
	if (m_dwCurTargetID == GT_INVALID || m_dwCurTargetID == 0)	return;

	Role* pRole = RoleMgr::Inst()->FindRole( m_dwCurTargetID );
	if( P_VALID( pRole ) && !( pRole->IS_KIND_OF( NPC ) && pRole->IsDead() ))
	{
		tstring name = pRole->GetRoleName();
		m_pSt_TofTName->SetText(name.c_str());
	}

}

DWORD TargetStateFrame::OnNS_TargetOfTarget(tagNS_TargetOfTarget* pNetCmd,DWORD)
{
	if(		pNetCmd->dwPlayerID == GT_INVALID || pNetCmd->dwTargetID == GT_INVALID 
		||	pNetCmd->dwPlayerID == 0 || pNetCmd->dwTargetID == 0 )
	{
		m_pSt_TofTBackAll->SetInvisible(true);
		return 0;
	}
		
	// 是当前目标的目标
	if (pNetCmd->dwPlayerID == m_dwCurShowRoleID  && ( pNetCmd->dwTargetID != m_dwCurTargetID ))
	{
		UpdataTargetofTarget_NPC(pNetCmd->dwTargetID);
		UpdataTargetName();
		if(m_dwCurTargetID != GT_INVALID)
			m_pSt_TofTBackAll->SetInvisible(false);
	}
	
	return 0;
}

VOID TargetStateFrame::UpdateFashionData()
{
	if( !P_VALID( m_pWnd_Menu ) )
		return;

	if( m_pWnd_Menu->IsInvisible() )
		return;

	const INT nCurSelMainMenu = m_pLb_Menu->GetCurSelectedRow();
	if( GT_INVALID == nCurSelMainMenu )
	{
		m_pWnd_SuitMenu->SetInvisible( TRUE );
		return;
	}

	if( nCurSelMainMenu == m_nCurMenuIndex[EMI_BeautyFriends] )
	{
		Role *pRole = RoleMgr::Inst()->FindRole(m_dwCurShowRoleID);
		if(P_VALID(pRole))
		{
			if (IS_PLAYER(pRole->GetID()))
			{
				Player* pPlayer = static_cast<Player*>(pRole);
				vector<DWORD> vecFashion;
				bool bMan =false;
				if(pPlayer->GetPlayerSex())
					bMan = true;
				ItemMgr::Inst()->GetAllSuitableFashion( bMan, vecFashion);
				if(vecFashion.empty())
					m_pWnd_SuitMenu->SetInvisible(TRUE);
				ForceResizeMenu( m_pLb_SuitMenu, vecFashion.size() );
				for( size_t i = 0; i < vecFashion.size(); i++ )
				{
					tstring strName = ClothespressData::Inst()->GetSuitName(vecFashion[i]);
					m_pLb_SuitMenu->SetText( i, 0, strName.c_str() );
					m_pLb_SuitMenu->SetUserData( i, vecFashion[i] );
				}
				m_pWnd_SuitMenu->SetInvisible( FALSE );
			}
		}
	}
	else
		m_pWnd_SuitMenu->SetInvisible(TRUE);

	if( !m_pWnd_SuitMenu->IsInvisible() )
	{
		const BOOL bMoveable = m_pWnd_SuitMenu->IsMovable();
		m_pWnd_SuitMenu->SetMovable( TRUE );
		tagPoint ptDestPos = tagPoint( m_pWnd_Menu->GetClient().x + m_pWnd_Menu->GetSize().x, m_pWnd_Menu->GetClient().y );
		ptDestPos.y += nCurSelMainMenu * m_pLb_Menu->GetRowHeight();
		tagPoint ptOldPos = m_pWnd_SuitMenu->GetClient();
		m_pWnd_SuitMenu->Move( ptDestPos - ptOldPos );
		m_pWnd_SuitMenu->SetMovable( bMoveable );

		//m_pLb_SuitMenu->SetCurSelectedRow( GT_INVALID );
		m_pWnd_SuitMenu->SetRefresh();
	}
}