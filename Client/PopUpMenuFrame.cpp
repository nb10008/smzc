#include "StdAfx.h"
#include "PopUpMenuFrame.h"
#include "WorldPickerFrame.h"
#include "RoleMgr.h"
#include "Role.h"
#include "ChatEvent.h"
#include "TeamSys.h"
#include "SocialEvent.h"
#include "CombatSys.h"
#include "PetManager.h"
#include "Pet.h"
#include "TeamEvent.h"
#include "LocalPlayer.h"
#include "PlayerNameTab.h"
#include "RoleEvent.h"
#include "CombatEvent.h"
#include "GuildMgr.h"
#include "DispRoleInfoFrame.h"
#include "UIEvent.h"
#include "PlayerNameTab.h"
#include "CursorMgr.h"
#include "RoleMgr.h"
#include "MasterEvent.h"
#include "SpritePartnershipFrame.h"
#include "CombatSysUtil.h"
#include "ItemMgr.h"
#include "..\WorldDefine\msg_fashion.h"
#include "ClothespressData.h"
#include "FamilyEvent.h"

PopUpMenuFrame::PopUpMenuFrame(void) : 
m_Trunk(this),
m_eShowType(ESPPT_Null),
m_dwCurShowRoleID(GT_INVALID)
{

}

PopUpMenuFrame::~PopUpMenuFrame(void)
{
}

BOOL PopUpMenuFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pMgr->RegisterEventHandle( _T("tagPlayerPopupMenu"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PopUpMenuFrame::OnPlayerPopUpMenu) );
	m_pMgr->RegisterEventHandle( _T("tagPvPRoomMenuEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PopUpMenuFrame::OnPvPPopUpMenu) );
	return TRUE;
}

BOOL PopUpMenuFrame::Destroy()
{
 	m_pMgr->UnRegisterEventHandler( _T("tagPlayerPopupMenu"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PopUpMenuFrame::OnPlayerPopUpMenu) );
	m_pMgr->UnRegisterEventHandler( _T("tagPvPRoomMenuEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&PopUpMenuFrame::OnPvPPopUpMenu) );

	GameFrame::Destroy();

	if( P_VALID(m_pMenu) )
	{
		m_pMenu->Destroy();
		m_pMenu = NULL;
	}

	if( P_VALID(m_pWnd_SuitMenu) )
	{
		m_pWnd_SuitMenu->Destroy();
		m_pWnd_SuitMenu = NULL;
	}

	return TRUE;
}

BOOL PopUpMenuFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\systemexpand.xml");
	if( !m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pGUI->ChangeXml( &ele, "expand", "Name", "PopUpMenu" );
	m_pMenu	= m_pGUI->CreateWnd( _T("\\desktop"), &ele );
	m_pGUI->RegisterEventHandler( m_pMenu->GetFullName().c_str(), m_Trunk.sfp1(&PopUpMenuFrame::EventHandler) );
	m_pListBox = (GUIListBox*)m_pMenu->GetChild( _T("listbutton1") );
	m_pListBox->SetColWidth(0, m_pListBox->GetSize().y);
	m_pMenu->SetInvisible( TRUE );
	m_pMenu->SetTopMost(TRUE);

	m_nMenuPanelHeightExt = m_pMenu->GetSize().y - m_pListBox->GetSize().y;

	m_pGUI->ChangeXml( &ele, "PopUpMenu", "Name", "PopUpFashionMenu");
	m_pGUI->ChangeXml( &ele, "PopUpFashionMenu\\listbutton1", "AlignControl", "\\desktop\\PopUpFashionMenu");
	tagPoint ptWnd = m_pMenu->GetClient();
	stringstream streamX,streamY;
	streamX<<m_pMenu->GetClient().x + m_pMenu->GetSize().x;
	streamY<<m_pMenu->GetClient().y;
	m_pGUI->ChangeXml( &ele, "PopUpFashionMenu", "LogicPos_x", streamX.str().c_str() );
	m_pGUI->ChangeXml( &ele, "PopUpFashionMenu", "LogicPos_y", streamY.str().c_str() );
	m_pWnd_SuitMenu	= m_pGUI->CreateWnd( _T("\\desktop"), &ele );
	m_pLb_SuitMenu = (GUIListBox*)m_pWnd_SuitMenu->GetChild( _T("listbutton1") );
	m_pLb_SuitMenu->SetColWidth(0, m_pLb_SuitMenu->GetSize().y);
	m_pWnd_SuitMenu->SetInvisible(TRUE);
	m_pGUI->RegisterEventHandler(m_pWnd_SuitMenu->GetFullName().c_str(), m_Trunk.sfp1(&PopUpMenuFrame::EventHandler));
	
	return TRUE;
}


VOID PopUpMenuFrame::Update()
{
	if( m_eShowType == ESPPT_Null )
		return;
	
	tagPoint pt = CursorMgr::Inst()->GetCursorPos();
	if( m_pMenu->PointInWnd( pt ) )
		return;

	//玩家已经不在视野内
	if(m_eShowType == ESPPT_Player)
	{
		Role* pPlayer = RoleMgr::Inst()->FindRole( m_dwCurShowRoleID );
		if( !P_VALID(pPlayer) )
		{
			ResetMenu();
		}		
	}	

	if( NeedClose( pt, m_pMenu->GetView(), m_pMenu->GetSize(), 40 ) )
	{
		ResetMenu();
	}

	//UpdateFashionData();
	//// 防止滚动菜单
	//if( 0 != m_pLb_SuitMenu->GetCurrentTextRow() )
	//	m_pLb_SuitMenu->SetCurrentTextRow( 0 );
}

BOOL PopUpMenuFrame::EscCancel()
{
	if( !P_VALID( m_pMenu ) || m_pMenu->IsInvisible() )
		return FALSE;

	ResetMenu();

	return TRUE;
}

void PopUpMenuFrame::UpdateMenuItem()
{
	switch( m_eShowType )
	{
	case ESPPT_Null:
		{
			ResetMenu();
		}
		break;
	case ESPPT_Player:
		{
			UpateMenuPlayer();
		}
		break;
	case ESPPT_PvPRoomPos:
		{
			UpdateMenuPvP();
		}
		break;
	default:
		break;
	}
}

BOOL PopUpMenuFrame::EventHandler(tagGUIEvent* pEvent)
{
	const GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID( pWnd ) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Deactive:
		{
			ResetMenu();
		}
		break;
	case EGUIE_ItemClick:
		{
			if( pWnd == m_pListBox )
			{
				switch( m_eShowType )
				{	
				case ESPPT_Player:
					ProcessPlayerEvent( pEvent->dwParam1 );
					break;
				case ESPPT_PvPRoomPos:
					ProcessPvPEvent(pEvent->dwParam1 );
					break;
				default:
					break;
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
				ResetMenu();
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		if( NeedClose( pEvent->msg.pt, m_pMenu->GetView(), m_pMenu->GetSize(), 40 ) )
		{
			ResetMenu();
		}
		break;
	default:
		break;
	}

	return FALSE;
}

VOID PopUpMenuFrame::SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nLbIndex, const INT nStrIndex )
{
	tstringstream stream;
	stream<<szBaseKey<<nStrIndex;
	tstring strText = g_StrTable[ stream.str().c_str() ];
	pListBox->SetText( nLbIndex, 0, strText.c_str() );
}


VOID PopUpMenuFrame::SetMenuFromStrTable( GUIListBox* pListBox, const TCHAR* szBaseKey, const INT nNumRow, const INT nStartRow, const INT nEndRow, const BOOL bAutoResize )
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

VOID PopUpMenuFrame::ForceResizeMenu( GUIListBox* pListBox, const INT nNumRow )
{
	tagPoint p(0, 0);
	ForceMoveTo(p);
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

void PopUpMenuFrame::ProcessPlayerEvent( DWORD dwMenuItemIndex )
{
	if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_PrivateChat] )
	{
		tagPrivateChatEvent evt( _T("tagPrivateChatEvent"), this );
		evt.dwRoleID = m_dwCurShowRoleID;
		evt.strRoleName = m_strRoleName;
		m_pMgr->SendEvent( &evt );
		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_Friend] )
	{
		tagAddFriendEvent evt( _T("SocialEvent_AddFriend"), this );
		evt.roleID = m_dwCurShowRoleID;
		m_pMgr->SendEvent( &evt );
		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_Trade] )
	{
		tagFollowRoleEvent evt( _T("TargetEvent_ReqTrade"), this );
		evt.dwTargetRoleID = m_dwCurShowRoleID;
		m_pMgr->SendEvent( &evt );
		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_Follow] )
	{
		tagFollowRoleEvent evt( _T("tagFollowRoleEvent"), this );
		evt.dwTargetRoleID = m_dwCurShowRoleID;
		m_pMgr->SendEvent( &evt );
		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_TeamInvite] )
	{
		TeamSys::Inst()->InviteJoinTeam( m_dwCurShowRoleID );
		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_TeamApply] )
	{
		tagApplyJoinTeamEvent evt( _T("tagApplyJoinTeamEvent"), this );
		evt.dwRoleID = m_dwCurShowRoleID;
		m_pMgr->SendEvent( &evt );

		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_FactionInvite] )
	{
		GuildMgr::Inst()->SendGuildJoinReq( m_dwCurShowRoleID );
		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_PlayerInfo] )
	{
		tagDispPlayerState evt(_T("tagDispPlayerState"), this);
		evt.dwRoleID = m_dwCurShowRoleID;
		m_pMgr->SendEvent( &evt );
		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_PlayerInfo2] )
	{
		tagOpenRoleInfoEvent evt(_T("Open_RoleInfo"), this);
		evt.dwRoleID = m_dwCurShowRoleID;
		m_pMgr->SendEvent( &evt );
		ResetMenu();
	}
    else if (dwMenuItemIndex == m_MenuItemPlayer[EMIP_ApprenticeToMaster])
    {
        tagGetAquireMasterEvent e(_T("tagGetAquireMasterEvent"), this);
        e.roleID = m_dwCurShowRoleID;
        m_pMgr->SendEvent(&e);
        ResetMenu();
    }
    else if (dwMenuItemIndex == m_MenuItemPlayer[EMIP_ToHaveApprentice])
    {
        tagAquireApprenticeEvent e(_T("tagAquireApprenticeEvent"), this);
        e.roleID = m_dwCurShowRoleID;
        m_pMgr->SendEvent(&e);
        ResetMenu();
    }
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_AddToBlackList] )
	{
		tagAddBlackEvent evt( _T("SocialEvent_AddBlack"), this );
		evt.roleID = m_dwCurShowRoleID;
		m_pMgr->SendEvent( &evt );
		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_PetTrade] )
	{
		tagFollowRoleEvent evt( _T("TargetEvent_ReqPetTrade"), this );
		evt.dwTargetRoleID = m_dwCurShowRoleID;
		m_pMgr->SendEvent( &evt );
		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_MountInvite] )//发送邀请驾御消息
	{
		CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame( _T("CombatSys") );
		tagNC_MountInvite msg;
		msg.dwDstRoleID	= pCombatSys->GetCurTargetID();
		TObjRef<NetSession>()->Send( &msg );
		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_MiniChat] )//发送密语消息
	{
		CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame( _T("CombatSys") );
		tagOpenMiniChatEvent evt(_T("tagOpenMiniChatEvent"), this);
		evt.name = PlayerNameTab::Inst()->FindNameByID( m_dwCurShowRoleID);
		m_pMgr->SendEvent(&evt);
		ResetMenu();   
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_ApplySpritePart] )//妖精繁育申请
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
			cmd.dwRoleID = m_dwCurShowRoleID;
			TObjRef<NetSession>()->Send(&cmd);
			ResetMenu();
		}  
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMI_InviteFamily] )
	{
		tagFamilyInviteEvent evt(_T("FamilyEvent_Invite"), this);
		evt.dwRoleID	= m_dwCurShowRoleID;
		evt.strRoleName	= PlayerNameTab::Inst()->FindNameByID(m_dwCurShowRoleID);
		m_pMgr->SendEvent(&evt);
		ResetMenu();   
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMI_ApplyFamily] )
	{
		tagFamilyApplyEvent evt(_T("FamilyEvent_Apply"), this);
		evt.dwRoleID = m_dwCurShowRoleID;
		evt.strRoleName	= PlayerNameTab::Inst()->FindNameByID(m_dwCurShowRoleID);
		m_pMgr->SendEvent(&evt);
		ResetMenu();   
	}
}


void PopUpMenuFrame::ProcessPvPEvent( DWORD dwMenuItemIndex )
{
	if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_PrivateChat] )
	{
		tagPrivateChatEvent evt( _T("tagPrivateChatEvent"), this );
		evt.dwRoleID = m_dwCurShowRoleID;
		evt.strRoleName = m_strRoleName;
		m_pMgr->SendEvent( &evt );
		ResetMenu();
	}
	else if( dwMenuItemIndex == m_MenuItemPlayer[EMIP_Friend] )
	{
		tagAddFriendEvent evt( _T("SocialEvent_AddFriend"), this );
		evt.roleID = m_dwCurShowRoleID;
		m_pMgr->SendEvent( &evt );
		ResetMenu();
	}	
}

void PopUpMenuFrame::UpateMenuPlayer()
{
	for( int i = 0; i < EMIP_Num; i++ )
	{
		m_MenuItemPlayer[i] = GT_INVALID;
	}

	INT nMenuIndex = 0;
	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_PrivateChat );
	m_MenuItemPlayer[EMIP_PrivateChat] = nMenuIndex++;

	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_Friend );
	m_MenuItemPlayer[EMIP_Friend] = nMenuIndex++;

	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_Trade );
	m_MenuItemPlayer[EMIP_Trade] = nMenuIndex++;

	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_Follow );
	m_MenuItemPlayer[EMIP_Follow] = nMenuIndex++;

	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_ApplySpritePart );
	m_MenuItemPlayer[EMIP_ApplySpritePart] = nMenuIndex++;

	// 装扮好友
	//Role *pRole = RoleMgr::Inst()->FindRole(m_dwCurShowRoleID);
	//if(P_VALID(pRole))
	//{
	//	if (IS_PLAYER(pRole->GetID()))
	//	{
	//		Player* pPlayer = static_cast<Player*>(pRole);
	//		vector<DWORD> vecFashion;
	//		bool bMan =false;
	//		if(pPlayer->GetPlayerSex())
	//			bMan = true;
	//		ItemMgr::Inst()->GetAllSuitableFashion( bMan, vecFashion);
	//		if(!vecFashion.empty())
	//		{
	//			SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMI_BeautyFriends );
	//			m_MenuItemPlayer[EMI_BeautyFriends] = nMenuIndex++;
	//		}
	//	}
	//}

	//如果选中的不是本地玩家,选中的是远程玩家
	if( RoleMgr::Inst()->GetLocalPlayerID() != m_dwCurShowRoleID )
	{
		//本地玩家没有队伍 或者 是队长可以邀请其他玩家加入队伍
		if( TeamSys::Inst()->GetTeammateNum() == 0 || 
			( TeamSys::Inst()->IsLeader( RoleMgr::Inst()->GetLocalPlayerID() ) && !TeamSys::Inst()->IsTeamFull() && !TeamSys::Inst()->IsTeammate( m_dwCurShowRoleID ) ) )
		{
			SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_TeamInvite );
			m_MenuItemPlayer[EMIP_TeamInvite] = nMenuIndex++;
		}

		//本地玩家没有队伍，可以申请加入其他队伍
		if( !TeamSys::Inst()->IsInTeam() )
		{
			SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_TeamApply );
			m_MenuItemPlayer[EMIP_TeamApply] = nMenuIndex++;
		}
	}

	if( GuildMgr::Inst()->IsCanJionReq( m_dwCurShowRoleID ))// 如果可以邀请该玩家加入帮派，则显示帮派邀请
	{
		SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_FactionInvite );
		m_MenuItemPlayer[EMIP_FactionInvite] = nMenuIndex++;
	}

	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_PlayerInfo );
	m_MenuItemPlayer[EMIP_PlayerInfo] = nMenuIndex++;

// 	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_PlayerInfo2 );
// 	m_MenuItemPlayer[EMIP_PlayerInfo2] = nMenuIndex++;

	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_AddToBlackList );
	m_MenuItemPlayer[EMIP_AddToBlackList] = nMenuIndex++;

	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_PetTrade );
	m_MenuItemPlayer[EMIP_PetTrade] = nMenuIndex++;

    //SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_ApprenticeToMaster );
    //m_MenuItemPlayer[EMIP_ApprenticeToMaster] = nMenuIndex++;

    //SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_ToHaveApprentice );
    //m_MenuItemPlayer[EMIP_ToHaveApprentice] = nMenuIndex++;

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
			SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_MountInvite );
			m_MenuItemPlayer[EMIP_MountInvite] = nMenuIndex++;
		}
	}

 	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_MiniChat );   //密语,加师徒的话直接在后面添加就好了，stringtable我也改了
 	m_MenuItemPlayer[EMIP_MiniChat] = nMenuIndex++;

	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMI_InviteFamily );   //家族邀请
	m_MenuItemPlayer[EMI_InviteFamily] = nMenuIndex++;

	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMI_ApplyFamily );   //家族申请
	m_MenuItemPlayer[EMI_ApplyFamily] = nMenuIndex++;

	ForceResizeMenu( m_pListBox, nMenuIndex );
}


void PopUpMenuFrame::UpdateMenuPvP()
{
	for( int i = 0; i < EMIP_Num; i++ )
	{
		m_MenuItemPlayer[i] = GT_INVALID;
	}

	INT nMenuIndex = 0;
	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_PrivateChat );
	m_MenuItemPlayer[EMIP_PrivateChat] = nMenuIndex++;

	SetMenuFromStrTable( m_pListBox, _T("TargetMenu_"), nMenuIndex, EMIP_Friend );
	m_MenuItemPlayer[EMIP_Friend] = nMenuIndex++;	

	ForceResizeMenu( m_pListBox, nMenuIndex );
}

DWORD PopUpMenuFrame::OnPlayerPopUpMenu( tagPlayerPopupMenu* pEvent )
{
	if( (m_eShowType == ESPPT_Player &&m_dwCurShowRoleID == pEvent->dwRoleID) || !P_VALID(pEvent->dwRoleID) )
		return 0;
	
	//清空上一次的显示
	ResetMenu();

	m_eShowType = ESPPT_Player;
	m_dwCurShowRoleID = pEvent->dwRoleID;
	m_strRoleName = PlayerNameTab::Inst()->FindNameByID( m_dwCurShowRoleID );

	UpdateMenuItem();
	ForceMoveTo( pEvent->pt );

	m_pMenu->SetInvisible( FALSE );
	m_pMenu->FlipToTop();
	m_pMenu->SetRefresh( TRUE );	

	return 0;
}

DWORD PopUpMenuFrame::OnPvPPopUpMenu( tagPvPRoomMenuEvent* pEvent )
{
	//清空上一次的显示
	ResetMenu();

	m_eShowType = ESPPT_PvPRoomPos;
	m_dwCurShowRoleID = pEvent->dwRoleID;
	m_strRoleName = pEvent->szName;

	UpdateMenuItem();
	ForceMoveTo( pEvent->pt );

	m_pMenu->SetInvisible( FALSE );
	m_pMenu->FlipToTop();
	m_pMenu->SetRefresh( TRUE );	
	m_pMenu->FlipToTop();
	m_pMenu->SetRefresh( TRUE );

	return 0;
}

void PopUpMenuFrame::ForceMoveTo( tagPoint& pt )
{
	m_pListBox->SetCurrentTextRow(0);
	tagPoint pointOff( pt - (tagPoint&)(m_pMenu->GetView()) );
	m_pMenu->SetMovable(TRUE);
	m_pMenu->Move(pointOff);
	m_pMenu->SetMovable(FALSE);
}

void PopUpMenuFrame::ResetMenu()
{ 
	m_pMenu->SetInvisible( TRUE ); 
	m_pMenu->SetRefresh(TRUE); 
	m_eShowType = ESPPT_Null;
	m_dwCurShowRoleID = GT_INVALID;
	m_strRoleName.assign( _T("") );
	m_pWnd_SuitMenu->SetInvisible(TRUE);
}

BOOL PopUpMenuFrame::NeedClose( tagPoint pt, tagPoint ptView,tagPoint ptSize, LONG dist )
{
	if( pt.x < ptView.x )
	{
		if( (ptView.x - pt.x) >= dist )
			return TRUE;
	}
	else if( pt.x > (ptView.x + ptSize.x) )
	{
		if( pt.x - ptView.x - ptSize.x >= dist )
			return TRUE;
	}
	else if( pt.y < ptView.y )
	{
		if( ptView.y - pt.y >= dist )
			return TRUE;
	}
	else if( pt.y > (ptView.y+ptSize.y) )
	{
		if( pt.y - ptView.y - ptSize.y > dist )
			return TRUE;
	}

	return FALSE;
}


VOID PopUpMenuFrame::UpdateFashionData()
{
	if( !P_VALID( m_pMenu ) )
		return;

	if( m_pMenu->IsInvisible() )
		return;

	if(m_eShowType!= ESPPT_Player)
		return;

	const INT nCurSelMainMenu = m_pListBox->GetCurSelectedRow();
	if( GT_INVALID == nCurSelMainMenu )
	{
		m_pWnd_SuitMenu->SetInvisible( TRUE );
		return;
	}

	if( nCurSelMainMenu == m_MenuItemPlayer[EMI_BeautyFriends] )
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
		m_pWnd_SuitMenu->SetInvisible( TRUE );

	if( !m_pWnd_SuitMenu->IsInvisible() )
	{
		const BOOL bMoveable = m_pWnd_SuitMenu->IsMovable();
		m_pWnd_SuitMenu->SetMovable( TRUE );
		tagPoint ptDestPos = tagPoint( m_pMenu->GetClient().x + m_pMenu->GetSize().x, m_pMenu->GetClient().y );
		ptDestPos.y += nCurSelMainMenu * m_pListBox->GetRowHeight();
		tagPoint ptOldPos = m_pWnd_SuitMenu->GetClient();
		m_pWnd_SuitMenu->Move( ptDestPos - ptOldPos );
		m_pWnd_SuitMenu->SetMovable( bMoveable );

		m_pLb_SuitMenu->SetCurSelectedRow( GT_INVALID );
		m_pWnd_SuitMenu->SetRefresh();
	}
}