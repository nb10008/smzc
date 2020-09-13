#include "StdAfx.h"
#include "ItemTrackPlayerFrame.h"
#include "ChatFrame.h"
#include "CombatSysUtil.h"
#include "MapLogicData.h"
#include "WorldFrame.h"
#include "ItemMgr.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "Container.h"
#include "..\WorldDefine\msg_spec_item.h"

ItemTrackPlayerFrame::ItemTrackPlayerFrame(void): m_Trunk(this)
{
}

ItemTrackPlayerFrame::~ItemTrackPlayerFrame(void)
{
}

BOOL ItemTrackPlayerFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init( pMgr, pFather, dwParam );

	m_pCmdMgr->Register("NS_TrackPlayerPos",		(NETMSGPROC)m_Trunk.sfp2(&ItemTrackPlayerFrame::OnNS_TrackPlayerPos),	_T("NS_TrackPlayerPos") );
	m_pCmdMgr->Register("NS_Move2Player4Track",		(NETMSGPROC)m_Trunk.sfp2(&ItemTrackPlayerFrame::OnNS_TrackPlayerTransfer), _T("NS_Move2Player4Track") );
	m_pCmdMgr->Register("NS_GetPositionAfterTrack",	(NETMSGPROC)m_Trunk.sfp2(&ItemTrackPlayerFrame::OnNS_GetPositionAfterTrack), _T("NS_GetPositionAfterTrack") );

	m_pMgr->RegisterEventHandle(_T("MsgBox_PlayerTrackMove2Player"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ItemTrackPlayerFrame::OnEvent_PlayerTrackMove2Player) );
	m_pMgr->RegisterEventHandle(_T("PlayerTrack_MsgBoxEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemTrackPlayerFrame::OnTryToTrackPlayer) );
	
	m_TrackItemSpecVal = 0;
	m_step = EStep_Null;
	m_n64ItemID = GT_INVALID;
	memset( m_szPlayerName,0, sizeof(m_szPlayerName) );
	m_nTellPositionAfterTransfer = 0;
	m_dwTargetRoleID = 0;
	return bRet;
}

BOOL ItemTrackPlayerFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	
	m_pCmdMgr->UnRegister("NS_TrackPlayerPos",			(NETMSGPROC)m_Trunk.sfp2(&ItemTrackPlayerFrame::OnNS_TrackPlayerPos));
	m_pCmdMgr->UnRegister("NS_Move2Player4Track",		(NETMSGPROC)m_Trunk.sfp2(&ItemTrackPlayerFrame::OnNS_TrackPlayerTransfer) );
	m_pCmdMgr->UnRegister("NS_GetPositionAfterTrack",		(NETMSGPROC)m_Trunk.sfp2(&ItemTrackPlayerFrame::OnNS_GetPositionAfterTrack) );

	m_pMgr->UnRegisterEventHandler(_T("PlayerTrack_MsgBoxEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ItemTrackPlayerFrame::OnTryToTrackPlayer) );
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_PlayerTrackMove2Player"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ItemTrackPlayerFrame::OnEvent_PlayerTrackMove2Player) );
	return bRet;
}

VOID ItemTrackPlayerFrame::Update()
{
	switch( m_step )
	{
	case EStep_Null:
	case EStep_Wait:
		break;
	case EStep_ShowInputMsgBox:
		{
			if( P_VALID( GetObj( "PlayerTrack_NameMsgBox" ) ) )
				KillObj( "PlayerTrack_NameMsgBox" );

			CreateObj("PlayerTrack_NameMsgBox", "MsgInputBox");
			TObjRef<MsgInputBox>("PlayerTrack_NameMsgBox")->Init(_T(""), g_StrTable[_T("PlayerTrack_MsgBoxText")], _T("PlayerTrack_MsgBoxEvent"), GT_INVALID, MBIT_Name, NULL, TRUE );
			
			//等待输入要追踪的玩家的名字
			m_step = EStep_Wait;
		}
		break;
	case EStep_Loading:
		{
			WorldFrame* pWorldFrame = (WorldFrame*)m_pMgr->GetFrame( _T("World") );
			if( P_VALID(pWorldFrame) )
			{
				if( pWorldFrame->GetCurWorldSate() == WorldFrame::EWState_GotoNewMap )
				{
					//让更多的CPU时间加载地图
					::Sleep( 100 );
				}
				else
				{
					m_dwTimer = Kernel::Inst()->GetAccumTimeDW();
					m_step = EStep_RTSHowPlayerInfo;
				}
			}
		}
		break;
	case EStep_RTSHowPlayerInfo:
		{
			DWORD dwCurTime = Kernel::Inst()->GetAccumTimeDW();
			if( dwCurTime-m_dwTimer > 10000 && m_nTellPositionAfterTransfer >=0 && m_nTellPositionAfterTransfer < 2 )
			{
				tagNC_GetPositionAfterTrack cmd;
				cmd.n64Item = m_n64ItemID;
				cmd.dwRoleID = m_dwTargetRoleID;
				m_pSession->Send( &cmd );
				m_dwTimer = Kernel::Inst()->GetAccumTimeDW();
			}
			else if( m_nTellPositionAfterTransfer <0 || m_nTellPositionAfterTransfer >=2 )
			{
				m_step = EStep_End;
			}
		}
		break;
	case EStep_End:
		{
			m_pMgr->AddToDestroyList( this );
		}
		break;
	}
}

DWORD ItemTrackPlayerFrame::OnTryToTrackPlayer( tagMsgInputBoxEvent* pGameEvent )
{
	if( MBF_OK == pGameEvent->eResult )
	{
		tstring strName = pGameEvent->strInputName;
		if( strName.empty() )
			return 0;

		Item* pItem = ItemMgr::Inst()->GetPocketItemByID( m_n64ItemID );
		if( !P_VALID(pItem) || !P_VALID(pItem->GetProto()) )
			return 0;
		if( pItem->GetProto()->nSpecFuncVal1 == 1 )
		{

			if(!CheckLPState())
				return 0;
			m_pItem = pItem;

			tagNC_Move2Player4Track cmd;
			cmd.n64ItemTrack = m_n64ItemID;
			_tcsncpy( cmd.szPlayerName, strName.c_str(),sizeof(cmd.szPlayerName)/sizeof(TCHAR) );
			m_pSession->Send( &cmd );
		}
		else
		{
			tagNC_TrackPlayerPos _evt;
			_evt.n64Item = m_n64ItemID;
			_tcsncpy( _evt.szPlayerName, strName.c_str(), sizeof(_evt.szPlayerName)/sizeof(TCHAR) );
			m_pSession->Send( &_evt );
		}
	}
	else
	{
		m_step = EStep_End;
	}

	return 0;
}

DWORD ItemTrackPlayerFrame::OnNS_TrackPlayerPos( tagNS_TrackPlayerPos *pNetCmd, DWORD )
{
	if( !P_VALID(pNetCmd) )
	{
		m_step = EStep_End;
		return 0;
	}
	
	switch( pNetCmd->dwErrorCode )
	{
	case E_Success:
		{
			BOOL bMapNormal = FALSE;
			MapName mapName = GetMapName( pNetCmd->dwMapID, pNetCmd->vPosition, &bMapNormal );

			//检查当前是否要进行传送提示
			Item* pItem = NULL;
			bool bNeedTransfer = CheckTransfer( pItem );

			//通用描述
			TCHAR szPosDesc[X_LONG_NAME] = {0};
			_stprintf( szPosDesc, g_StrTable[_T("PlayerTrack_PlayerPosition")], pNetCmd->szPlayerName,mapName.first.c_str(),mapName.second.c_str(),(INT)(pNetCmd->vPosition.x/50), (INT)(pNetCmd->vPosition.z/50) );

			//聊天当前频道提示
			ChatFrame* pChatFrame = (ChatFrame*)m_pMgr->GetFrame( _T("Chat") );
			if( P_VALID(pChatFrame) )
			{
				pChatFrame->PushInfo( szPosDesc, ESCC_Current );
			}

			//如果玩家处于不可传送区域，提示之
			if( !bMapNormal )
			{
				TCHAR szDesc[X_LONG_NAME] = {0};
				_stprintf( szDesc, g_StrTable[_T("PlayerTrack_IllegalArea")], pNetCmd->szPlayerName );
				
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szDesc );
				if( P_VALID(pChatFrame) )
				{
					pChatFrame->PushInfo( szDesc, ESCC_Current );
				}
				break;
			}

			//可以传送
			if( bNeedTransfer )
			{
				//屏显提示
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szPosDesc );

				//询问是否传送
				if( P_VALID(pItem) )
				{
					TCHAR szTransferDesc[X_LONG_NAME] = {0};
					_stprintf( szTransferDesc, g_StrTable[_T("PlayerTrack_DoYouWantToTransfer")], pNetCmd->szPlayerName );

					if( P_VALID(GetObj("PlayerTrack_Move2PlayerMsgBox")) )
						KillObj( "PlayerTrack_Move2PlayerMsgBox" );

					CreateObj( "PlayerTrack_Move2PlayerMsgBox", "MsgBox" );
					TObjRef<MsgBox>("PlayerTrack_Move2PlayerMsgBox")->SetN64UserData( pItem->GetItemId() );
					TObjRef<MsgBox>("PlayerTrack_Move2PlayerMsgBox")->SetStringUserData( pNetCmd->szPlayerName );
					TObjRef<MsgBox>( "PlayerTrack_Move2PlayerMsgBox" )->Init( _T(""), szTransferDesc, _T("MsgBox_PlayerTrackMove2Player"), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE );
				}
				else
				{
					if( P_VALID(GetObj("PlayerTrack_MsgBox")) )
						KillObj( "PlayerTrack_MsgBox" );

					CreateObj( "PlayerTrack_MsgBox", "MsgBoxEx" );
					TObjRef<MsgBoxEx>( "PlayerTrack_MsgBox" )->Init( _T(""), szPosDesc, NULL, MBF_OK );
				}
			}
			else
			{
				if( P_VALID(GetObj("PlayerTrack_MsgBox")) )
					KillObj( "PlayerTrack_MsgBox" );

				CreateObj( "PlayerTrack_MsgBox", "MsgBoxEx" );
				TObjRef<MsgBoxEx>( "PlayerTrack_MsgBox" )->Init( _T(""), szPosDesc, NULL, MBF_OK );
			}
		}
		break;
	case E_ItemTrack_HaveNotItem:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_HaveNotItem")] );
		}
		break;
	case E_ItemTrack_ItemUseTimeLimit:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_UseTimeLimit")] );
		}
		break;
	case E_ItemTrack_PlayerOffLine:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_PlayerOffLine2")] );
		}
		break;
	case E_ItemTrack_PlayerNotExists:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_PlayerNotExist")] );
		}
		break;
	case E_ItemTrack_BuffLimit:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("PlayerTrack_BuffLimit")]);
		}
		break;
	default:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_TrackFailed")] );
		}
		break;
	}

	return 0;
}

DWORD ItemTrackPlayerFrame::OnNS_TrackPlayerTransfer( tagNS_Move2Player4Track *pNetCmd, DWORD )
{
	if( !P_VALID(pNetCmd) )
		return 0;

	tstring strPlayerName = pNetCmd->szPlayerName;
	switch( pNetCmd->dwErrorCode )
	{
	case E_Success:
		{
			GotoNewMap( pNetCmd->dwMapID, pNetCmd->pos, pNetCmd->faceTo );
			m_dwTargetRoleID = pNetCmd->dwPlayerID;
			m_step = EStep_Loading;
		}
		break;
	case E_ItemTrack_TargetPrisonAreaState:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_TargetPrisonArea")] );
			
			//聊天当前频道提示
			ChatFrame* pChatFrame = (ChatFrame*)m_pMgr->GetFrame( _T("Chat") );
			if( P_VALID(pChatFrame) )
			{
				pChatFrame->PushInfo( g_StrTable[_T("PlayerTrack_TargetPrisonArea")], ESCC_Current );
			}
		}
		break;
	case E_ItemTrack_XiulianChang:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_XiulianChang")] );

			//聊天当前频道提示
			ChatFrame* pChatFrame = (ChatFrame*)m_pMgr->GetFrame( _T("Chat") );
			if( P_VALID(pChatFrame) )
			{
				pChatFrame->PushInfo( g_StrTable[_T("PlayerTrack_XiulianChang")], ESCC_Current );
			}
		}
		break;
	case E_ItemTrack_Instance:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_Instance")] );
			
			//聊天当前频道提示
			ChatFrame* pChatFrame = (ChatFrame*)m_pMgr->GetFrame( _T("Chat") );
			if( P_VALID(pChatFrame) )
			{
				pChatFrame->PushInfo( g_StrTable[_T("PlayerTrack_Instance")], ESCC_Current );
			}
		}
		break;
	case E_ItemTrack_JinCity:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_JinCity")] );
			
			//聊天当前频道提示
			ChatFrame* pChatFrame = (ChatFrame*)m_pMgr->GetFrame( _T("Chat") );
			if( P_VALID(pChatFrame) )
			{
				pChatFrame->PushInfo( g_StrTable[_T("PlayerTrack_JinCity")], ESCC_Current );
			}
		}
		break;
	case E_ItemTrack_ShenJi:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_ShenJi")] );

			//聊天当前频道提示
			ChatFrame* pChatFrame = (ChatFrame*)m_pMgr->GetFrame( _T("Chat") );
			if( P_VALID(pChatFrame) )
			{
				pChatFrame->PushInfo( g_StrTable[_T("PlayerTrack_ShenJi")], ESCC_Current );
			}
		}
		break;
	case E_ItemTrack_HaveNotItem:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_HaveNotItem")] );
		}
		break;
	case E_ItemTrack_ItemUseTimeLimit:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_UseTimeLimit")] );
		}
		break;
	case E_ItemTrack_PlayerOffLine:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_PlayerOffLine2")] );
		}
		break;
	case E_ItemTrack_PlayerNotExists:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_PlayerNotExist")] );
		}
		break;
	default:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_TransferFailed")] );
		}
		break;
	}

	return 0;
}

DWORD ItemTrackPlayerFrame::OnNS_GetPositionAfterTrack( tagNS_GetPositionAfterTrack *pNetCmd, DWORD )
{
	if( !P_VALID(pNetCmd ) )
		return 0;

	m_nTellPositionAfterTransfer++;
	switch( pNetCmd->dwErrorCode )
	{
	case E_Success:
		{
			BOOL bMapNormal = FALSE;
			MapName mapName = GetMapName( pNetCmd->dwMapID, pNetCmd->pos, &bMapNormal );

			//通用描述
			TCHAR szPosDesc[X_LONG_NAME] = {0};
			_stprintf( szPosDesc, g_StrTable[_T("PlayerTrack_PlayerPosition")], pNetCmd->szPlayerName,mapName.first.c_str(),mapName.second.c_str(),(INT)(pNetCmd->pos.x/50), (INT)(pNetCmd->pos.z/50) );

			//聊天频道通知
			ChatFrame* pChatFrame = (ChatFrame*)m_pMgr->GetFrame( _T("Chat") );
			if( P_VALID(pChatFrame) )
			{
				pChatFrame->PushInfo( szPosDesc, ESCC_Current );
			}

			//屏幕显示通知
			CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szPosDesc );
		}
		break;
	default:
		break;
	}

	return 0;
}


DWORD ItemTrackPlayerFrame::OnEvent_PlayerTrackMove2Player( tagMsgBoxEvent* pGameEvent )
{
	if ( MBF_OK == pGameEvent->eResult )
	{
		if( !P_VALID(GetObj("PlayerTrack_Move2PlayerMsgBox")) )
			return 0;

		tstring strPlayerName = TObjRef<MsgBox>("PlayerTrack_Move2PlayerMsgBox")->GetStringUserData();
		if( strPlayerName.empty() )
			return 0;

		INT64 n64ItemID = TObjRef<MsgBox>("PlayerTrack_Move2PlayerMsgBox")->GetN64UserData();
		Item* pItem = ItemMgr::Inst()->GetPocketItemByID( n64ItemID );
		if( !P_VALID(pItem) )
			return 0;

		if(!CheckLPState())
			return 0;
		m_pItem = pItem;
		m_n64ItemID = n64ItemID;

		tagNC_Move2Player4Track cmd;
		cmd.n64ItemTrack = n64ItemID;
		_tcsncpy( cmd.szPlayerName, strPlayerName.c_str(),sizeof(cmd.szPlayerName)/sizeof(TCHAR) );
		m_pSession->Send( &cmd );

		
	}

	return 0;
}

bool ItemTrackPlayerFrame::CheckTransfer( Item* & pTransferItem )
{
	std::list<Item*> lstTrack;
	ItemMgr::Inst()->GetPocket()->GetSpecFuncMap( EISF_PlayerTrack, lstTrack );

	Item* pItem = NULL;
	const tagItemProto* pItemProto = NULL;

	//追踪的时候使用的是什么样的道具，
	for( std::list<Item*>::iterator iter = lstTrack.begin(); iter != lstTrack.end(); ++iter )
	{
		pItemProto = (*iter)->GetProto();
		if( !P_VALID(pItemProto) )
			continue;

		if( pItemProto->nSpecFuncVal1 == m_TrackItemSpecVal )
		{
			pItem = *iter;
			break;
		}
	}

	//使用的是只能追踪的道具 并且现在还用这种道具，不进行传送询问
	if( m_TrackItemSpecVal == 0 && P_VALID(pItem) )
		return false;

	pItem = NULL;
	pItemProto = NULL;
	
	//既然没有之前使用的道具了，检查一下有没有带传送功能的追踪道具
	for( std::list<Item*>::iterator iter = lstTrack.begin(); iter != lstTrack.end(); ++iter )
	{
		pItemProto = (*iter)->GetProto();
		if( !P_VALID(pItemProto) )
			continue;

		if( pItemProto->nSpecFuncVal1 == 1 )
		{
			pItem = *iter;
			break;
		}
	}

	//有带传送功能的追踪道具
	if( P_VALID(pItem) )
	{
		pTransferItem = pItem;
		return true;
	}

	return false;
}

ItemTrackPlayerFrame::MapName ItemTrackPlayerFrame::GetMapName( DWORD dwMapId, Vector3 pos, LPBOOL pNormal )
{	
	//得到大地图的名字
	tstring strMapName = _T("");
	const tagMapLogicData* pMapLogicData = MapLogicData::Inst()->FindMapLogicData( dwMapId );
	if( P_VALID(pMapLogicData) )
		strMapName = pMapLogicData->szShowName;

	//得到地图区域的名字
	tstring strAreaName = _T("");

	/*AABBox box;
	Vector3 vHalfSize = RoleMgr::Inst()->GetLocalPlayer()->GetRoleSize() * 0.5f;
	box.min = pos - vHalfSize;
	box.max = pos + vHalfSize;
	box.min.y += vHalfSize.y;
	box.max.y += vHalfSize.y;

	const std::vector<tagMapShowAreaData>& vecMapArea = pMapLogicData->vecShowAreaData;
	std::vector<tagMapShowAreaData>::const_iterator iter;
	for( iter = vecMapArea.begin(); iter != vecMapArea.end(); ++iter )
	{
		if( (*iter).box.Contain( box ) )
		{
			strAreaName = (*iter).szTitle;
			_tcsncpy( strAreaName, (*iter).szTitle.c_str(), sizeof(strAreaName)/sizeof(TCHAR) );
			break;
		}
	}*/
	
	if( P_VALID(pMapLogicData) && pMapLogicData->eMapType == EMT_Normal )
		*pNormal = TRUE;
	else
		*pNormal = FALSE;

	//以前是副本地图不能传送，但是有些地图不是副本也要求不能传送
	//暂时这么写吧，着急出版本
	//if( dwMapId == 3017298127			//m02
	//	|| dwMapId == 3017298383		//m03
	//	|| dwMapId == 3017299663		//m04
	//	|| dwMapId == 3017299919		//m05
	//	|| dwMapId == 3017299151		//m06
	//	|| dwMapId == 1146339967		//m06_1
	//	|| dwMapId == 3017299407		//m07
	//	|| dwMapId == 2983744207		//m20
	//  )
	//{
	//	*pNormal = TRUE;
	//}
	//else
	//{
	//	*pNormal = FALSE;
	//}

	return MapName( strMapName, strAreaName );
}

void ItemTrackPlayerFrame::GotoNewMap( DWORD dwMapID, Vector3 vecPos,Vector3 vecFaceto )
{
	WorldFrame* pWorldFrame = (WorldFrame*)m_pMgr->GetFrame( _T("World") );
	if( !P_VALID(pWorldFrame) )
		return;

	pWorldFrame->GoToMap( dwMapID, vecPos, vecFaceto );
}


bool ItemTrackPlayerFrame::CheckLPState()
{
	LocalPlayer* pLPlayer = RoleMgr::Inst()->GetLocalPlayer();
	if( !P_VALID(pLPlayer) )
		return false;

	int nState = GT_INVALID;

	if( pLPlayer->IsInState(ES_Dead) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_DeadState")] );
		return false;
	}

	if( pLPlayer->IsInState(ES_Dizzy) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_DizzyState")] );
		return false;
	}

	if( pLPlayer->IsInState(ES_Tie) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_TieState")] );
		return false;
	}

	if( pLPlayer->IsInState(ES_Spor) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_SporState")] );
		return false;
	}

	if( pLPlayer->GetRoleState(ERS_PK) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_PKState")] );
		return false;
	}

	if( pLPlayer->GetRoleState(ERS_Commerce) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_CommerceState")] );
		return false;
	}

	if( pLPlayer->GetRoleState(ERS_PrisonArea) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_PrisonState")] );
		return false;
	}

	if( pLPlayer->GetRoleState(ERS_Stall) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_StallState")] );
		return false;
	}

	if( P_VALID(pLPlayer->GetMount()) )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("PlayerTrack_MountState")] );
		return false;
	}

	return true;
}