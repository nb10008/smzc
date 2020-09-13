#include "StdAfx.h"
#include "GuildApplyManageFrame.h"
#include "GuildMgr.h"
#include "ChatEvent.h"
#include "CombatSysUtil.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "..\WorldDefine\msg_guild.h"

GuildApplyManageFrame::GuildApplyManageFrame() : m_Trunk(this),
m_pWnd( NULL ),
m_pPatchPlayers( NULL ),		
m_pBtnClose( NULL ),			
m_pPbnGuildTenet( NULL ),
m_pPbnGuildApply( NULL ),
m_pBtnChat( NULL ),
m_pBtnAccept( NULL ),		
m_pBtnRefuse( NULL ),	
m_pLstPlayers( NULL ),
m_pBtnPageBack( NULL ),
m_pBtnPageFront( NULL ),
m_pStcPageNum( NULL ),
m_pPatchTenet( NULL ),	
m_pEditBoxTenet( NULL ),
m_pBtnModify( NULL ),
m_pBtnCancel( NULL ),
m_nCurPage(0),
m_nCurSelectItem( GT_INVALID )
{
}

GuildApplyManageFrame::~GuildApplyManageFrame()
{
}


BOOL GuildApplyManageFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register( "NS_GuildApplierBill", (NETMSGPROC)m_Trunk.sfp2( &GuildApplyManageFrame::OnNS_GuildApplierBill), _T("NS_GuildApplierBill") );
	
	const tagGuildBase* pGuildInfo = GuildMgr::Inst()->GetGuildInfo();
	if( P_VALID(pGuildInfo) && pGuildInfo->dwLeaderRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		tagNC_GetGuildApplierBill cmd;
		m_pSession->Send( &cmd );
	}

	return TRUE;
}

BOOL GuildApplyManageFrame::EscCancel()
{
	m_pMgr->AddToDestroyList( this );
	return TRUE;
}


BOOL GuildApplyManageFrame::Destroy()
{
	GameFrame::Destroy();

	if( P_VALID( m_pWnd ) )
		m_pGUI->DestroyWnd( m_pWnd );
	m_pWnd = NULL;

	m_pCmdMgr->UnRegister("NS_GuildApplierBill", (NETMSGPROC)m_Trunk.sfp2(&GuildApplyManageFrame::OnNS_GuildApplierBill) );

	return TRUE;
}


BOOL GuildApplyManageFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\guild_deal.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&GuildApplyManageFrame::EventHandler) );
	
	m_pBtnClose		 = (GUIButton*)m_pWnd->GetChild( _T("groupmanage_pic\\closebutt") );
	m_pPbnGuildTenet = (GUIPushButton*)m_pWnd->GetChild( _T("groupmanage_pic\\fraback\\opbutt1") );
	m_pPbnGuildApply = (GUIPushButton*)m_pWnd->GetChild( _T("groupmanage_pic\\fraback\\opbutt2") );
	
	m_pPatchPlayers	= (GUIPatch*)m_pWnd->GetChild( _T("groupmanage_pic\\listback") );
	m_pBtnPageBack	= (GUIButton*)m_pWnd->GetChild( _T("groupmanage_pic\\listback\\backbutt") );
	m_pBtnPageFront	= (GUIButton*)m_pWnd->GetChild( _T("groupmanage_pic\\listback\\nextbutt") );	
	m_pStcPageNum	= (GUIStatic*)m_pWnd->GetChild( _T("groupmanage_pic\\listback\\new") );
	m_pLstPlayers	= (GUIListBox*)m_pWnd->GetChild( _T("groupmanage_pic\\listback\\listback0\\member_list") );
	m_pBtnChat		= (GUIButton*)m_pWnd->GetChild( _T("groupmanage_pic\\listback\\saybutt") );
	m_pBtnAccept	= (GUIButton*)m_pWnd->GetChild( _T("groupmanage_pic\\listback\\agreebutt") );
	m_pBtnRefuse	= (GUIButton*)m_pWnd->GetChild( _T("groupmanage_pic\\listback\\refusebutt") );

	m_pPatchTenet	= (GUIPatch*)m_pWnd->GetChild( _T("groupmanage_pic\\listback1") );
	m_pEditBoxTenet	= (GUIEditBoxEx*)m_pWnd->GetChild( _T("groupmanage_pic\\listback1\\pic\\editbox") );
	m_pBtnModify	= (GUIButton*)m_pWnd->GetChild( _T("groupmanage_pic\\listback1\\alterbutt") );
	m_pBtnCancel	= (GUIButton*)m_pWnd->GetChild( _T("groupmanage_pic\\listback1\\cancelbutt") );
	
	TCHAR szNum[10] = {0};
	m_pStcPageNum->SetText( _itot(m_nCurPage+1,szNum,10) );
	m_pLstPlayers->SetColNum( 5, 73 );
// 	m_pLstPlayers->SetColNum( 0, 120 );
// 	m_pLstPlayers->SetColNum( 1, 52 );
// 	m_pLstPlayers->SetColNum( 2, 52 );
	m_pLstPlayers->SetColWidth(0,120);
	m_pLstPlayers->SetColWidth(1,52);
	m_pLstPlayers->SetColWidth(2,52);
	m_pLstPlayers->SetColWidth(3,73);
	m_pLstPlayers->SetColWidth(4,73);

	tstring strTenet = GuildMgr::Inst()->GetGuildTenet();
	m_pEditBoxTenet->SetText( strTenet.c_str() );
	m_pEditBoxTenet->SetRefresh( TRUE );
	
	const tagGuildBase* pGuildInfo = GuildMgr::Inst()->GetGuildInfo();
	if( P_VALID(pGuildInfo) && pGuildInfo->dwLeaderRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
	{
		m_pPbnGuildApply->SetState( EGUIBS_PushDown );
	}
	else
	{
		//m_pPbnGuildTenet->SetState( EGUIBS_PushDown );
		m_pPbnGuildApply->SetEnable( FALSE );
	}

	m_pWnd->SetInvisible( TRUE );

	return TRUE;
}

//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL GuildApplyManageFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return 0;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			//关闭按钮
			if( pWnd == m_pBtnClose )
			{
				m_pMgr->AddToDestroyList(this);
			}
			//取消按钮
			else if( pWnd == m_pBtnCancel )
			{
				m_pPbnGuildApply->SetState( EGUIBS_PushDown );
			}
			//向第一页的方向翻页
			else if( pWnd == m_pBtnPageBack )
			{
				//不能再翻页了，已经是第一页了，第一页索引为0
				if( m_nCurPage <= 0 )
					break;

				m_nCurPage--;
				RefreshDisplay();
			}
			//向最后一页的方向翻页
			else if( pWnd == m_pBtnPageFront )
			{
				//不能再翻页了，已经是最后一页了
				if( m_nCurPage >= (GetPages()-1) )
					break;

				m_nCurPage++;
				RefreshDisplay();
			}
			//设为私聊对象
			else if( pWnd == m_pBtnChat )
			{
				OnChat();
			}
			//同意
			else if( pWnd == m_pBtnAccept )
			{
				OnAcceptApply();
				const tagGuildBase* pGuildInfo = GuildMgr::Inst()->GetGuildInfo();
				if( P_VALID(pGuildInfo) && pGuildInfo->dwLeaderRoleID == RoleMgr::Inst()->GetLocalPlayerID() )
				{
					tagNC_GetGuildApplierBill cmd;
					m_pSession->Send( &cmd );
				}
			}
			//拒绝
			else if( pWnd == m_pBtnRefuse )
			{
				OnRefuseApply();
			}
			//修改宗旨
			else if( pWnd == m_pBtnModify )
			{
				tstring strTenet = m_pEditBoxTenet->GetText( TRUE );
				//------------------------------------------------------
				tstring::iterator new_end;
				new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0d);
				strTenet.erase(new_end, strTenet.end());
				new_end = remove(strTenet.begin(), strTenet.end(), (TCHAR)0x0a);
				strTenet.erase(new_end, strTenet.end());
				//------------------------------------------------------
				if( strTenet.size() > 200 )
				{
					TCHAR szWarning[100] = {0};
					_stprintf( szWarning, g_StrTable[_T("GuildApply_TenetTooLong")], 200 );
					CombatSysUtil::Inst()->ShowScreenCenterMsgEx( szWarning );
					break;
				}
				
				INT nLen = strTenet.size();
				INT nMax = (nLen + 1) * sizeof(TCHAR) + sizeof(tagNC_GuildTenetChange) - 1;
				BYTE* pMsg = new BYTE[nMax];
				tagNC_GuildTenetChange* pCmd = (tagNC_GuildTenetChange*)pMsg;
				pCmd->dwID = pCmd->Crc32("NC_GuildTenetChange");
				memcpy( pCmd->szTenet, strTenet.c_str(), (nLen + 1) * sizeof(TCHAR) );
				pCmd->szTenet[nLen] = _T('\0');
				pCmd->dwSize = nMax;
				m_pSession->Send( pCmd );
				SAFE_DEL_ARRAY( pMsg );
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if( pWnd == m_pPbnGuildTenet )
			{
				m_pPatchTenet->SetInvisible( FALSE );
				m_pPatchPlayers->SetInvisible( TRUE );
			}
			else if( pWnd == m_pPbnGuildApply )
			{
				m_pPatchTenet->SetInvisible( TRUE );
				m_pPatchPlayers->SetInvisible( FALSE );
			}
		}
		break;
	case EGUIE_ItemClick:
		{
			if( pWnd == m_pLstPlayers )
			{
				SelectListBoxItem( m_pLstPlayers->GetCurSelectedRow() );
			}
		}
		break;

	default:
		break;
	}

	return TRUE;
}

DWORD GuildApplyManageFrame::OnNS_GuildApplierBill( tagNS_GuildApplierBill* pMsg, DWORD dwParam )
{
	//获取帮派失败
	if( pMsg->dwErrorCode != E_Success )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsgEx( g_StrTable[_T("GuildApply_FailedToGetApplierBill")] );
		return 0;
	}

	if( pMsg->n16BillNum == 0 )
		return 0;

	m_lstPlayers.clear();
	UpdateGuildList( pMsg );
	RefreshDisplay();
	SelectListBoxItem( m_pLstPlayers->GetCurSelectedRow() );
	return 0;
}

void GuildApplyManageFrame::UpdateGuildList( tagNS_GuildApplierBill* pMsg )
{
	tagGuildApplierBillInfo info;
	INT nGuildCount = pMsg->n16BillNum;

	for( INT i = 0; i< nGuildCount; i++ )
	{
		info = pMsg->applierBill[i];
		m_lstPlayers.push_back( info );
	}
}


void GuildApplyManageFrame::ShowWnd( BOOL bShow )
{
	m_pWnd->SetInvisible( !bShow );
	m_pWnd->SetTopMost(TRUE);
}

void GuildApplyManageFrame::RefreshDisplay()
{  
	//再次校正页数
	if( m_nCurPage < 0 )
		m_nCurPage = 0;

	if( m_nCurPage >= GetPages() )
		m_nCurPage = GetPages() - 1;

	//得到要显示的listbox索引范围
	INT nBeginNum = m_nCurPage * GUILD_APPLY_MAX_ITEMS_PER_PAGE;			//要显示的记录的启示索引
	TCHAR szNum[X_SHORT_NAME] = {0};

	ClearListBox();
	PlayerListIterator iter = m_lstPlayers.begin();
	for( int nRowIndex = 0; iter != m_lstPlayers.end(); ++iter,++nRowIndex )
	{
		//从nBeginNum索引开始显示
		if( nRowIndex < nBeginNum )
			continue;

		//每页显示这么多记录
		if( nRowIndex >= (m_nCurPage+1)*GUILD_APPLY_MAX_ITEMS_PER_PAGE )
			break;

		INT nColNum = 0;

		//设置玩家的名字
		m_pLstPlayers->SetText( nRowIndex, nColNum, iter->strRoleName );
		nColNum++;

		//设置性别
		if( iter->bySex == ES_Man )
			m_pLstPlayers->SetText( nRowIndex, nColNum, g_StrTable[_T("GuildApply_SexMan")] );
		else
			m_pLstPlayers->SetText( nRowIndex, nColNum, g_StrTable[_T("GuildApply_SexWoman")] );
		nColNum++;

		//设置帮派的等级
		m_pLstPlayers->SetText( nRowIndex, nColNum, _itot( iter->byLevel, szNum, 10 ) );
		nColNum++;

		//设置职业
		tstring strClass = GetClassName( (EClassType)iter->byVaction );
		m_pLstPlayers->SetText( nRowIndex, nColNum, strClass.c_str() );
		nColNum++;

		//设置离线时间
		TCHAR szTime[X_SHORT_NAME] = {0};
		tstring strText = GetOnlineText( iter->dwOfflineTime );
		m_pLstPlayers->SetText( nRowIndex, nColNum, strText.c_str() );
	}

	m_pStcPageNum->SetText( _itot( m_nCurPage+1, szNum, 10 ) );
	m_pStcPageNum->SetRefresh( TRUE );
	m_pLstPlayers->SetRefresh( TRUE );
}

tstring GuildApplyManageFrame::GetClassName( EClassType eClassType )
{
	if(eClassType >= EV_Base && eClassType < EV_End)
	{
		tstringstream ts;
		ts << _T("ClassName_") << eClassType;
		return g_StrTable[ts.str().c_str()];
	}

	return _T("");
}

void GuildApplyManageFrame::SelectListBoxItem( INT nSelectItem )
{
	if( nSelectItem == m_nCurSelectItem )
		return;

	if( nSelectItem < 0 || nSelectItem >= GUILD_APPLY_MAX_ITEMS_PER_PAGE )
	{
		m_nCurSelectItem = GT_INVALID;
		return;
	}

	m_nCurSelectItem = nSelectItem;
}

int GuildApplyManageFrame::GetPlayerDataIndex()
{
	if( !IsCurPageGood() )
		return GT_INVALID;

	int nPlayerIndex = m_nCurPage * GUILD_APPLY_MAX_ITEMS_PER_PAGE + m_nCurSelectItem;
	if( nPlayerIndex >= 0 && nPlayerIndex < (int)m_lstPlayers.size() )
	{
		return nPlayerIndex;
	}

	return GT_INVALID;
}

GuildApplyManageFrame::PlayerListIterator GuildApplyManageFrame::FindGuildInfo( int nPlayerDataIndex )
{
	if( nPlayerDataIndex < 0 || nPlayerDataIndex >= (int)m_lstPlayers.size() )
		return m_lstPlayers.end();

	PlayerListIterator iter = m_lstPlayers.begin();
	for( int i=0; iter != m_lstPlayers.end(); ++iter, ++i )
	{
		if( i == nPlayerDataIndex )
			return iter;
	}

	return m_lstPlayers.end();
}

void GuildApplyManageFrame::OnAcceptApply()
{
	//得到对于 m_lstPlayers 的索引
	INT nCurPlayerIndex = GetPlayerDataIndex();
	if( !GT_VALID(nCurPlayerIndex) )
		return;

	//从 m_lstPlayers 中得到数据
	PlayerListIterator iter = FindGuildInfo( nCurPlayerIndex );
	if( iter == m_lstPlayers.end() )
		return;

	//tagNC_AcceptToApply cmd;
	tagNC_ApproveToJoinGuild cmd;
	cmd.dwErrorCode = 0;
	cmd.dwApplierID = iter->dwRoleID;
	m_pSession->Send( &cmd );

	DeletePlayer( iter->dwRoleID );
	RefreshDisplay();
}

void GuildApplyManageFrame::OnRefuseApply()
{
	//得到对于 m_lstPlayers 的索引
	INT nCurPlayerIndex = GetPlayerDataIndex();
	if( !GT_VALID(nCurPlayerIndex) )
		return;

	//从 m_lstPlayers 中得到数据
	PlayerListIterator iter = FindGuildInfo( nCurPlayerIndex );
	if( iter == m_lstPlayers.end() )
		return;

	//tagNC_RefuseToApply cmd;
	tagNC_ApproveToJoinGuild cmd;
	cmd.dwErrorCode = 1;
	cmd.dwApplierID = iter->dwRoleID;
	m_pSession->Send( &cmd );

	DeletePlayer( iter->dwRoleID );
	RefreshDisplay();
}

void GuildApplyManageFrame::OnChat()
{
	//得到对于 m_lstPlayers 的索引
	INT nCurPlayerIndex = GetPlayerDataIndex();
	if( !GT_VALID(nCurPlayerIndex) )
		return;

	//从 m_lstPlayers 中得到数据
	PlayerListIterator iter = FindGuildInfo( nCurPlayerIndex );
	if( iter == m_lstPlayers.end() )
		return;

	tagPrivateChatEvent evt( _T("tagPrivateChatEvent"), this );
	evt.dwRoleID = iter->dwRoleID;
	evt.strRoleName = iter->strRoleName;
	m_pMgr->SendEvent( &evt );
}

void GuildApplyManageFrame::ClearListBox()
{
	if( !P_VALID(m_pLstPlayers) )
		return;

	for( int nRow = 0; nRow< GUILD_APPLY_MAX_ITEMS_PER_PAGE; nRow++ )
	{
		for( int nCol = 0; nCol <=4; nCol++ )
		{
			m_pLstPlayers->SetText( nRow, nCol, _T("") );
		}
	}
}

tstring GuildApplyManageFrame::GetOnlineText( DWORD dwTime )
{
	DWORD dwSecondPerDay = 24*3600;
	TCHAR szText[X_SHORT_NAME] = {0};

	DWORD dwDays = 0, dwHours = 0;
	dwDays = dwTime / dwSecondPerDay;
	
	if( dwTime == 0 )
	{
		_stprintf( szText, g_StrTable[_T("GuildList_Online")] );
	}
	//间隔长超过一天
	else if( dwDays > 0 )
	{
		_stprintf( szText, g_StrTable[_T("GuildList_Offline_Day")], dwDays );
	}
	//一天之内
	else
	{
		dwHours = dwTime/3600;
		_stprintf( szText, g_StrTable[_T("GuildList_Offline_Hour")], dwHours );
	}

	return szText;
}

void GuildApplyManageFrame::DeletePlayer( DWORD dwRoleID )
{
	PlayerListIterator iter = m_lstPlayers.begin();
	for( ; iter !=m_lstPlayers.end(); ++iter )
	{
		if( iter->dwRoleID == dwRoleID )
		{
			m_lstPlayers.erase( iter );
			break;
		}
	}
}