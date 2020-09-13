#include "StdAfx.h"
#include "MasterFrame.h"
#include "DispRoleInfoFrame.h"
#include "ChatEvent.h"
#include "TeamEvent.h"
#include "SocialEvent.h"
#include "CurrencyMgr.h"
#include "PlayerNameTab.h"
#include "ServerTime.h"
#include "RoleMgr.h"
#include "CombatSysUtil.h"
#include "LocalPlayer.h"
#include "MasterMgr.h"
#include "Item.h"
#include "ItemMgr.h"
#include "Container.h"
#include "../WorldDefine/master_apprentice.h"

MasterFrame::MasterFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndBack = NULL;
	m_pMain = NULL;
	m_pTeaAndStu = NULL;
	m_pBtnMaster = NULL;
	m_pBtnStu = NULL;
	m_pStcName = NULL;
	m_pStcLevel = NULL;
	m_pStcNumOfQingWu = NULL;
	m_pStcNumOfQinWu = NULL;
	m_pBtnName = NULL;
	m_pBtnLevel = NULL;
	m_pBtnTask = NULL;
	m_pBtnLastLogin = NULL;
	m_pList = NULL;
	m_pStcInfo = NULL;
	m_pBtnAssign = NULL;
	m_pBtnLeave = NULL;
	m_pBtnClose = NULL;
	m_pBtnQuit = NULL;
	m_pBtnRule = NULL;

	m_pAssignWnd = NULL;
	m_pStcStuName = NULL;
	m_pStcJingWu = NULL;
	m_pEdtAssignJingWu = NULL;
	m_pStcTotalJingWu = NULL;
	m_pBtnOK = NULL;
	m_pBtnCancle = NULL;

	m_pUseWnd = NULL;
	m_pStcUseInfo = NULL;
	m_pBtnUseOK = NULL;
	m_pBtnUseCancle = NULL;

	m_pMenuWnd = NULL;
	m_pMenuList = NULL;

	m_dwTotalRow = 0;
	m_dwClickRow = GT_INVALID;
	m_dwRoleID = GT_INVALID;

	m_pRuleWnd = NULL;
	m_pBtnRuleJingWu = NULL;
	m_pBtnRuleMaster = NULL;
	m_pBtnRuleMasterTask = NULL;
	m_pBtnUseCancle = NULL;
}

MasterFrame::~MasterFrame()
{

}

BOOL MasterFrame::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if( !GameFrame::Init( pMgr, pFather, dwParam ) )
		return false;

	m_pNetCmdMgr->Register("NS_MatesInfo", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNetGetMasterInfo)), _T("NS_MatesInfo"));
	m_pNetCmdMgr->Register("NS_ApprenticeInfo", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNetGetStudentInfo)), _T("NS_ApprenticeInfo"));
	m_pNetCmdMgr->Register("NS_UseJingWuPoint", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNetUseJingWuPoint)), _T("NS_UseJingWuPoint"));
	m_pNetCmdMgr->Register("NS_GiveJingWuPoint", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNetAssignJingWuPoint)), _T("NS_GiveJingWuPoint"));
	m_pNetCmdMgr->Register( "NS_BreakOutApprenticeConfirm", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNS_BreakOutApprenticeConfirm)), _T("NS_BreakOutApprenticeConfirm"));
	m_pNetCmdMgr->Register( "NS_BreakOutApprentice", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNS_BreakOutApprentice)), _T("NS_BreakOutApprentice") );
	m_pNetCmdMgr->Register( "NS_BreakOutMasterConfirm", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNS_BreakOutMasterConfirm)), _T("NS_BreakOutMasterConfirm"));
	m_pNetCmdMgr->Register( "NS_BreakOutMaster", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNS_BreakOutMaster)), _T("NS_BreakOutMaster") );
	m_pNetCmdMgr->Register( "NS_ForceBreakOutMaster", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNS_ForceBreakOutMaster)), _T("NS_ForceBreakOutMaster") );
	m_pFrameMgr->RegisterEventHandle( _T("OnLeaveMaster"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&MasterFrame::OnLeaveMaster)) );
	m_pFrameMgr->RegisterEventHandle( _T("OnForceLeaveMaster"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&MasterFrame::OnForceLeaveMaster)) );
	m_pFrameMgr->RegisterEventHandle( _T("OnLeaveApprentice"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&MasterFrame::OnLeaveApprentice)) );
	return true;
}

BOOL MasterFrame::Destroy()
{
	m_pNetCmdMgr->UnRegister("NS_MatesInfo", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNetGetMasterInfo)));
	m_pNetCmdMgr->UnRegister("NS_ApprenticeInfo", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNetGetStudentInfo)));
	m_pNetCmdMgr->UnRegister("NS_UseJingWuPoint", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNetUseJingWuPoint)));
	m_pNetCmdMgr->UnRegister("NS_GiveJingWuPoint", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNetAssignJingWuPoint)));
	m_pNetCmdMgr->UnRegister( "NS_BreakOutApprenticeConfirm", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNS_BreakOutApprenticeConfirm)));
	m_pNetCmdMgr->UnRegister( "NS_BreakOutApprentice", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNS_BreakOutApprentice)) );
	m_pNetCmdMgr->UnRegister( "NS_BreakOutMasterConfirm", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNS_BreakOutMasterConfirm)));
	m_pNetCmdMgr->UnRegister( "NS_BreakOutMaster", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNS_BreakOutMaster)) );
	m_pNetCmdMgr->UnRegister( "NS_ForceBreakOutMaster", (NETMSGPROC)(m_Trunk.sfp2(&MasterFrame::OnNS_ForceBreakOutMaster)) );
	m_pFrameMgr->UnRegisterEventHandler( _T("OnLeaveMaster"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&MasterFrame::OnLeaveMaster)) );
	m_pFrameMgr->UnRegisterEventHandler( _T("OnForceLeaveMaster"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&MasterFrame::OnForceLeaveMaster)) );
	m_pFrameMgr->UnRegisterEventHandler( _T("OnLeaveApprentice"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&MasterFrame::OnLeaveApprentice)) );

	GameFrame::Destroy();
	if( P_VALID(m_pWnd) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;
	}
	if( P_VALID(m_pAssignWnd) )
	{
		m_pGUI->DestroyWnd(m_pAssignWnd);
		m_pAssignWnd = NULL;
	}
	if( P_VALID(m_pUseWnd) )
	{
		m_pGUI->SetModal(NULL);
		m_pGUI->DestroyWnd(m_pUseWnd);
		m_pUseWnd = NULL;
	}
	if( P_VALID(m_pMenuWnd) )
	{
		m_pGUI->DestroyWnd(m_pMenuWnd);
		m_pMenuWnd = NULL;
	}
	if( P_VALID(m_pRuleWnd) )
	{
		m_pGUI->DestroyWnd(m_pRuleWnd);
		m_pRuleWnd = NULL;
	}
	return true;
}

BOOL MasterFrame::EscCancel()
{
	if(m_pWnd->IsInvisible() )
		return FALSE;
	m_pFrameMgr->AddToDestroyList(this);
	return TRUE;
}

BOOL MasterFrame::ReloadUI()
{
	tstring strPath = g_strLocalPath + _T("\\ui\\teach.xml");

	// 师门界面
	m_pWnd = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	m_pWndBack = (GUIPatch*)m_pWnd->GetChild( _T("teach_pic") );
	m_pMain = (GUIPatch*)m_pWndBack->GetChild(_T("listback"));

	m_pTeaAndStu = (GUIFrameMutex*)m_pWndBack->GetChild(_T("fraback"));
	m_pBtnMaster = (GUIPushButton*)m_pTeaAndStu->GetChild(_T("opbutt1"));
	m_pBtnStu = (GUIPushButton*)m_pTeaAndStu->GetChild(_T("opbutt2"));

	m_pStcName = (GUIStatic*)m_pMain->GetChild(_T("listback\\word3"));
	m_pStcLevel = (GUIStatic*)m_pMain->GetChild(_T("listback\\word5"));
	m_pStcNumOfQingWu = (GUIStatic*)m_pMain->GetChild(_T("listback\\word4"));
	m_pStcNumOfQinWu = (GUIStatic*)m_pMain->GetChild(_T("listback\\word7"));

	m_pBtnName = (GUIButton*)m_pMain->GetChild(_T("listback0\\namebutt"));
	m_pBtnLevel = (GUIButton*)m_pMain->GetChild(_T("listback0\\levelbutt"));
	m_pBtnTask = (GUIButton*)m_pMain->GetChild(_T("listback0\\taskbutt"));
	m_pBtnLastLogin = (GUIButton*)m_pMain->GetChild(_T("listback0\\leavebutt"));
	m_pList = (GUIListBox*)m_pMain->GetChild(_T("listback0\\member_list"));

	m_pStcInfo = (GUIStatic*)m_pWndBack->GetChild(_T("word0"));
	m_pStcInfo->SetText(g_StrTable[_T("MasterWarning")]);
	m_pBtnAssign = (GUIButton*)m_pWndBack->GetChild(_T("assignbutt"));
	m_pBtnAssign->SetEnable(false);
	m_pBtnLeave = (GUIButton*)m_pWndBack->GetChild(_T("rushbutt"));
	m_pBtnLeave->SetEnable(false);
	m_pBtnRule = (GUIButton*)m_pWndBack->GetChild(_T("rushbutt0"));
	m_pBtnClose = (GUIButton*)m_pWndBack->GetChild(_T("shutbutt"));
	m_pBtnQuit = (GUIButton*)m_pWndBack->GetChild(_T("closebutt"));
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&MasterFrame::EventHandler) );
	m_pBtnMaster->SetState(EGUIBS_PushDown, FALSE);

	m_pList->SetColNum( 5, 50 );
	m_pList->SetColWidth( 0, 18 );	// 出师图标
	m_pList->SetColWidth( 1, 110 );	// 名字
	m_pList->SetColWidth( 2, 65 );	// 等级
	m_pList->SetColWidth( 3, 90 );	// 师徒任务
	m_pList->SetColWidth( 4, 90 );	// 最后上线时间

	// 授业点数使用界面
	strPath = g_strLocalPath + _T("\\ui\\msgbox.xml");
	m_pUseWnd = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	m_pStcUseInfo = (GUIStaticEx*)m_pUseWnd->GetChild(_T("back\\textback\\text"));
	m_pBtnUseOK = (GUIButton*)m_pUseWnd->GetChild(_T("back\\ok"));
	m_pBtnUseCancle = (GUIButton*)m_pUseWnd->GetChild(_T("back\\cancel"));
	m_pUseWnd->SetInvisible(true);
	m_pGUI->RegisterEventHandler( m_pUseWnd->GetFullName().c_str(), m_Trunk.sfp1(&MasterFrame::EventHandler) );

	// 授业点数分配界面
	strPath = g_strLocalPath + _T("\\ui\\teach_quality.xml");
	m_pAssignWnd = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	m_pStcStuName = (GUIStatic*)m_pAssignWnd->GetChild(_T("comm_pic\\word1"));
	m_pStcJingWu = (GUIStatic*)m_pAssignWnd->GetChild(_T("comm_pic\\num"));
	m_pEdtAssignJingWu = (GUIEditBox*)m_pAssignWnd->GetChild(_T("comm_pic\\word3\\inputpic1\\num1"));
	m_pStcTotalJingWu = (GUIStatic*)m_pAssignWnd->GetChild(_T("comm_pic\\word3\\new"));
	m_pBtnOK = (GUIButton*)m_pAssignWnd->GetChild(_T("comm_pic\\surebutt"));
	m_pBtnCancle = (GUIButton*)m_pAssignWnd->GetChild(_T("comm_pic\\canclebutt"));
	m_pBtnQuitA = (GUIButton*)m_pAssignWnd->GetChild(_T("comm_pic\\closebutt"));
	m_pAssignWnd->SetInvisible(true);
	m_pGUI->RegisterEventHandler( m_pAssignWnd->GetFullName().c_str(), m_Trunk.sfp1(&MasterFrame::EventHandler) );

	// 便捷界面
	strPath = g_strLocalPath + _T("\\ui\\systemexpand.xml");
	XmlElement eleMenu;
	if( !m_pGUI->LoadXml( &eleMenu, "VFS_System", strPath.c_str() ) )
		return FALSE;
	m_pGUI->ChangeXml( &eleMenu, "expand", "AlignControl", "\\desktop");
	m_pGUI->ChangeXml( &eleMenu, "expand", "Name", "MasterMenu1");
	m_pGUI->ChangeXml( &eleMenu, "MasterMenu1\\listbutton1", "AlignControl", "\\desktop\\MasterMenu1");
	m_pMenuWnd = m_pGUI->CreateWnd( _T("\\desktop"), &eleMenu );
	m_pMenuList = (GUIListBox*)m_pMenuWnd->GetChild( _T("listbutton1") );
	m_pMenuWnd->SetInvisible( TRUE );
	m_pGUI->RegisterEventHandler( m_pMenuWnd->GetFullName().c_str(), m_Trunk.sfp1(&MasterFrame::EventHandler) );

	// 规则说明界面
	strPath = g_strLocalPath + _T("\\ui\\teach_rules.xml");
	m_pRuleWnd = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str() );
	m_pBtnRuleMaster = (GUIButton*)m_pRuleWnd->GetChild(_T("teach_rules_pic\\teachbutt"));
	m_pBtnRuleMasterTask = (GUIButton*)m_pRuleWnd->GetChild(_T("teach_rules_pic\\taskbutt"));
	m_pBtnRuleJingWu = (GUIButton*)m_pRuleWnd->GetChild(_T("teach_rules_pic\\numberbutt"));
	m_pBtnRuleQuit = (GUIButton*)m_pRuleWnd->GetChild(_T("teach_rules_pic\\closebutt"));
	m_pRuleWnd->SetInvisible(true);
	m_pGUI->RegisterEventHandler( m_pRuleWnd->GetFullName().c_str(), m_Trunk.sfp1(&MasterFrame::EventHandler) );

	return true;
}

DWORD MasterFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnClose || pWnd == m_pBtnQuit )		// 关闭师门界面
				m_pFrameMgr->AddToDestroyList(this);
			else if( pWnd == m_pBtnAssign && m_pBtnStu->GetState() == EGUIBS_PushDown )		// 打开分配授业点数界面
			{
				if( CanAssignJingWuPoint() )
					ShowAssignWnd();
			}
			else if( pWnd == m_pBtnOK )									// 确定分配授业点数
			{
				m_pAssignWnd->SetInvisible(true);
				tagNC_GiveJingWuPoint cmd;
				cmd.dwRoleID = m_dwRoleID;
				cmd.nPoint = _tcstol(m_pEdtAssignJingWu->GetText(), NULL, 10);
				m_pSession->Send(&cmd);
			}
			else if( pWnd == m_pBtnQuitA || pWnd == m_pBtnCancle )		// 退出分配授业点数界面
				m_pAssignWnd->SetInvisible(true);
			else if( pWnd == m_pBtnAssign && m_pBtnMaster->GetState() == EGUIBS_PushDown )	// 打开使用授业点数界面
				ShowUseWnd();
			else if( pWnd == m_pBtnUseOK )								// 确定使用授业点数
			{
				if( CanUseJingWuPoint() )
				{
					tagNC_UseJingWuPoint cmd;
					m_pSession->Send(&cmd);
				}
			}
			else if( pWnd == m_pBtnUseCancle )							// 退出使用授业点数界面
			{
				m_pGUI->SetModal(NULL);
				m_pUseWnd->SetInvisible(true);
			}
			else if( pWnd == m_pBtnLeave && m_pBtnStu->GetState() == EGUIBS_PushDown )	// 逐出师门
			{
				if( m_dwRoleID != GT_INVALID )
				{
					tagNC_BreakOutApprentice cmd;
					cmd.dwRoleID = m_dwRoleID;
					m_pSession->Send(&cmd);
				}
				else
				{
					if( P_VALID(GetObj("CanNotBreakOutApprentice")) )
						KillObj("CanNotBreakOutApprentice");
					CreateObj( "CanNotBreakOutApprentice", "MsgBoxEx" );
					TObjRef<MsgBoxEx>("CanNotBreakOutApprentice")->Init( _T(""), g_StrTable[_T("SelectOneApprentice")] );
				}
			}
			else if( pWnd == m_pBtnLeave && m_pBtnMaster->GetState() == EGUIBS_PushDown ) // 脱离师门
			{
				tagNC_BreakOutMaster cmd;
				cmd.dwRoleID = RoleMgr::Inst()->GetLocalPlayer()->GetMasterID();
				m_pSession->Send(&cmd);
			}
			else if( pWnd == m_pBtnRule )
			{
				m_pRuleWnd->SetInvisible(false);
				m_pRuleWnd->FlipToTop();
			}
			else if( pWnd == m_pBtnRuleMaster )
			{
				if( P_VALID(GetObj("RuleMaster")) )
					KillObj("RuleMaster");
				CreateObj( "RuleMaster", "MsgBox" );
				TObjRef<MsgBox>("RuleMaster")->Init( g_StrTable[_T("RuleMasterCaption")], g_StrTable[_T("RuleMaster")] );
			}
			else if( pWnd == m_pBtnRuleMasterTask )
			{
				if( P_VALID(GetObj("RuleMasterTask")) )
					KillObj("RuleMasterTask");
				CreateObj( "RuleMasterTask", "MsgBox" );
				TObjRef<MsgBox>("RuleMasterTask")->Init( g_StrTable[_T("RuleMasterTaskCaption")], g_StrTable[_T("RuleMasterTask")] );
			}
			else if( pWnd == m_pBtnRuleJingWu )
			{
				if( P_VALID(GetObj("RuleJingWu")) )
					KillObj("RuleJingWu");
				CreateObj( "RuleJingWu", "MsgBox" );
				TObjRef<MsgBox>("RuleJingWu")->Init( g_StrTable[_T("RuleJingWuCaption")], g_StrTable[_T("RuleJingWu")] );
			}
			else if( pWnd == m_pBtnRuleQuit )
				m_pRuleWnd->SetInvisible(true);
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if( pWnd == m_pBtnMaster )			// 师门界面
			{
				tagNC_MatesInfo cmd;
				m_pSession->Send(&cmd);
				ShowFrame();
			}
			else if( pWnd == m_pBtnStu )		// 弟子界面
			{
				tagNC_ApprenticeInfo cmd;
				m_pSession->Send(&cmd);
				ShowFrame();
			}
		}
		break;
	case EGUIE_ItemClick:
		{
			if( pWnd == m_pList )		// 师门或弟子界面
			{
				if( m_dwTotalRow > pEvent->dwParam1 )
				{
					m_dwClickRow = pEvent->dwParam1;
					if( m_pBtnStu->GetState() == EGUIBS_PushDown )
					{
						if( m_dwClickRow != GT_INVALID && m_dwClickRow < m_vecMyStudent.size() )
						{
							m_dwRoleID = m_vecMyStudent[m_dwClickRow].dwRoleID;
						}
						else
							m_dwClickRow = GT_INVALID;
					}
						
					else if( m_pBtnMaster->GetState() == EGUIBS_PushDown )
					{
						if( m_dwClickRow != GT_INVALID && m_dwClickRow < m_vecMyGuild.size() )
						{
							m_dwRoleID = m_vecMyGuild[m_dwClickRow].dwRoleID; 
						}
						else
							m_dwClickRow = GT_INVALID;
					}
						
					if( pEvent->msg.dwType == WM_RBUTTONUP )
					{
						UpdateMenu();
						tagPoint ptLT = m_pWnd->GetClient();
						ptLT = ptLT + m_pList->GetFather()->GetClient();
						ptLT = ptLT + m_pList->GetClient();
						if( pEvent->msg.pt.x > ptLT.x+20
							&& pEvent->msg.pt.x < ptLT.x+135 )
						{
							MoveMenu( m_pMenuList, pEvent->msg.pt );
							m_pMenuWnd->SetInvisible( FALSE );
							m_pMenuWnd->FlipToTop();
							m_pGUI->SetActive(m_pMenuList);
						}
					}
				}
			}
			else if( pWnd == m_pMenuList )
			{
				if( pEvent->dwParam1 == EMI_PlayerInfo ) // 玩家资料
				{
					m_pMenuWnd->SetInvisible( TRUE );
					tagOpenRoleInfoEvent e(_T("Open_RoleInfo"), this);
					e.dwRoleID = m_dwRoleID;
					m_pMgr->SendEvent(&e);
				}
				else if( pEvent->dwParam1 == EMI_Chat ) // 设为私聊
				{
					m_pMenuWnd->SetInvisible( TRUE );

					tagPrivateChatEvent evt( _T("tagPrivateChatEvent"), this );
					evt.dwRoleID = m_dwRoleID;
					evt.strRoleName = m_pList->GetText( m_dwClickRow, 1 );
					m_pMgr->SendEvent( &evt );
				}
				else if( pEvent->dwParam1 == EMI_InviteGroup ) // 邀请组队
				{
					m_pMenuWnd->SetInvisible( TRUE );
					tagInviteJoinTeamEvent e(_T("tagInviteJoinTeamEvent"), this);
					e.dwRoleID = m_dwRoleID;
					m_pMgr->SendEvent(&e);
				}
				else if( pEvent->dwParam1 == EMI_ApplyGroup ) // 申请组队
				{
					m_pMenuWnd->SetInvisible( TRUE );
					tagApplyJoinTeamEvent evt( _T("tagApplyJoinTeamEvent"),this );
					evt.dwRoleID = m_dwRoleID;
					m_pMgr->SendEvent( &evt );
				}
				else if( pEvent->dwParam1 == EMI_AddFriend ) // 加为好友
				{
					m_pMenuWnd->SetInvisible( TRUE );
					tagAddFriendEvent event(_T("SocialEvent_AddFriend"), this);
					event.roleID = m_dwRoleID;
					m_pMgr->SendEvent(&event);
				}
			}
		}
		break;
	case EGUIE_Deactive:
		{
			if( pWnd == m_pMenuList )
			{
				m_pMenuWnd->SetInvisible( TRUE );
			}
		}
		break;
	default:
		break;
	}

	return FALSE;
}

void MasterFrame::ShowFrame()
{
	if( P_VALID(m_pWnd) )
	{
		m_pWnd->SetInvisible(false);
		m_pStcName->SetText(_T(""));
		m_pStcName->SetRefresh();
		m_pStcLevel->SetText(_T(""));
		m_pStcLevel->SetRefresh();
		m_pStcNumOfQingWu->SetText(_T(""));
		m_pStcNumOfQingWu->SetRefresh();
		m_pStcNumOfQinWu->SetText(_T(""));
		m_pStcNumOfQinWu->SetRefresh();
		m_dwClickRow = GT_INVALID;
		m_dwRoleID = GT_INVALID;
		m_pList->Clear();
		m_dwTotalRow = 0;
		tagPoint ptSizeOldListBox = m_pList->GetSize();
		tagPoint ptSizeListBox = ptSizeOldListBox;
		ptSizeListBox.y = m_pList->GetRowHeight() * m_dwTotalRow;
		m_pList->SetResizable( TRUE );
		m_pList->Resize( ptSizeListBox );
		m_pList->SetResizable( FALSE );
		m_pList->SetRefresh(TRUE);
		m_vecMyGuild.clear();
		m_vecMyStudent.clear();
		if( m_pBtnMaster->GetState() == EGUIBS_PushDown )
			ShowMaster();
		else if( m_pBtnStu->GetState() == EGUIBS_PushDown )
			ShowStudent();
	}
}

void MasterFrame::ShowMaster()
{
	m_pStcInfo->SetText(g_StrTable[_T("MasterWarning")]);
	m_pBtnLeave->SetText(g_StrTable[_T("LeaveMaster")]);
	TCHAR szTips[X_LONG_NAME] = {0};
	if( RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel() >= GOOD_STU_CAN_LEAVE_LEVEL )
		_sntprintf( szTips, X_LONG_NAME, g_StrTable[_T("UseJingWu")], 0 );
	else
		_sntprintf( szTips, X_LONG_NAME, g_StrTable[_T("UseJingWu")], MasterMgr::inst()->GetMyJingWu() );
	m_pBtnAssign->SetText(szTips);
	if( m_vecMyGuild.empty() )
	{
		m_pBtnAssign->SetEnable(false);
		m_pBtnLeave->SetEnable(false);
	}
	else
	{
		if( MasterMgr::inst()->GetMyJingWu() != 0 && RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel() < GOOD_STU_CAN_LEAVE_LEVEL )
			m_pBtnAssign->SetEnable(true);
		m_pBtnLeave->SetEnable(true);
	}
}

void MasterFrame::ShowStudent()
{
	m_pStcInfo->SetText(g_StrTable[_T("TachStu")]);
	m_pBtnLeave->SetText(g_StrTable[_T("BanishFromMaster")]);
	m_pBtnAssign->SetText(g_StrTable[_T("AssignJingWu")]);
	if( m_vecMyStudent.empty() )
	{
		m_pBtnAssign->SetEnable(false);
		m_pBtnLeave->SetEnable(false);
	}
	else
	{
		if( MasterMgr::inst()->GetMyJingWu() != 0 )
			m_pBtnAssign->SetEnable(true);
		m_pBtnLeave->SetEnable(true);
	}
}

VOID MasterFrame::MoveMenu( GUIListBox* pListBox, const tagPoint& ptDest )
{
	GUIWnd* pFather = pListBox->GetFather();
	if( !P_VALID( pFather ) )
		return;

	const tagPoint& oldPt = pFather->GetClient();
	tagPoint movePt = ptDest;
	pFather->SetMovable( TRUE );
	pFather->Move( movePt - oldPt );
	pFather->SetMovable( FALSE );
}

VOID MasterFrame::UpdateMenu()
{
	int nRow = 0;
	m_pMenuList->SetText( nRow, 0, g_StrTable[_T("Social_Menu_Info")] );
	nRow++;
	m_pMenuList->SetText( nRow, 0, g_StrTable[_T("Social_Menu_Talk")] );
	nRow++;
	m_pMenuList->SetText( nRow, 0, g_StrTable[_T("Chat_Menu_1")] );
	nRow++;
	m_pMenuList->SetText( nRow, 0, g_StrTable[_T("Chat_Menu_0")] );
	nRow++;
	m_pMenuList->SetText( nRow, 0, g_StrTable[_T("Chat_Menu_4")] );
	nRow++;
	ResizeMenu( m_pMenuList, nRow, 72 );
}

VOID MasterFrame::ResizeMenu( GUIListBox* pListBox, int row, int width )
{
	tagPoint ptSizeOldListBox = pListBox->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.x = width;
	ptSizeListBox.y = pListBox->GetRowHeight() * row;

	GUIWnd* pFather = pListBox->GetFather();
	if( !P_VALID( pFather ) )
		return;

	pFather->SetResizable( TRUE );
	tagPoint ptSizeFather = pFather->GetSize();
	ptSizeFather.x += ptSizeListBox.x - ptSizeOldListBox.x;
	ptSizeFather.y += ptSizeListBox.y - ptSizeOldListBox.y;
	pFather->Resize( ptSizeFather );
	pFather->SetResizable( FALSE );

	pListBox->SetResizable( TRUE );
	pListBox->Resize( ptSizeListBox );
	pListBox->SetResizable( FALSE );
}

// 查看我的师门界面
DWORD MasterFrame::OnNetGetMasterInfo(tagNS_MatesInfo *pNetCmd, DWORD)
{
	if(!P_VALID(m_pWnd))
		return 0;
	// 自己的授业点数
	MasterMgr::inst()->SetJingWuPoint( pNetCmd->nJingWuPoint );
	// 师尊
	if( pNetCmd->sMasterInfo.dwRoleID != GT_INVALID )
	{
		tstring strName = PlayerNameTab::Inst()->FindNameByID( pNetCmd->sMasterInfo.dwRoleID );
		m_pStcName->SetText( strName.c_str() );	// 名字
		tstringstream level;
		level << pNetCmd->sMasterInfo.nLevel;
		m_pStcLevel->SetText( level.str().c_str() );	// 等级
		tstringstream Point;
		Point << pNetCmd->sMasterInfo.dwJingWuPoint;
		m_pStcNumOfQingWu->SetText( Point.str().c_str() );	// 师傅授业点数
		tstringstream QinWuPoint;
		QinWuPoint << pNetCmd->sMasterInfo.dwQinWuPoint;	// 师傅师德点数
		m_pStcNumOfQinWu->SetText( QinWuPoint.str().c_str() );
	}
	RoleMgr::Inst()->GetLocalPlayer()->SetMasterID(pNetCmd->sMasterInfo.dwRoleID);
	
	// 我的师门
	m_pList->Clear();
	m_vecMyGuild.clear();
	m_dwTotalRow = 0;
	for( int i = 0; i < pNetCmd->nNum; i++ )
	{
		m_vecMyGuild.push_back(pNetCmd->sMatesInfo[i]);
		if( pNetCmd->sMatesInfo[i].nLevel >= GOOD_STU_CAN_LEAVE_LEVEL )
		{
			tagRect rcRect(0, 0, 22, 22);
			m_pList->SetItemPic( m_dwTotalRow, 0, _T("data\\ui\\teach\\l-baishi.dds"), rcRect); // 出师标志
		}
		
		DWORD dwColor = GT_INVALID;
		tstring strName = PlayerNameTab::Inst()->FindNameByID( pNetCmd->sMatesInfo[i].dwRoleID );
		m_pList->SetText( m_dwTotalRow, 1, strName.c_str(), dwColor );	// 名字
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("Role_Level")], pNetCmd->sMatesInfo[i].nLevel);
		m_pList->SetText( m_dwTotalRow, 2, szTmp, dwColor );			// 等级
		// 上次做师徒任务时间
		m_pList->SetText( m_dwTotalRow, 3, _T("----"), dwColor );
		// 上次离线时间
		m_pList->SetText( m_dwTotalRow, 4, GetOfflineTime( pNetCmd->sMatesInfo[i].bIsOnLine, false, dwColor, pNetCmd->sMatesInfo[i].lastOnLineTime).c_str(), dwColor );
		m_dwTotalRow++;
	}
	tagPoint ptSizeOldListBox = m_pList->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pList->GetRowHeight() * m_dwTotalRow;
	m_pList->SetResizable( TRUE );
	m_pList->Resize( ptSizeListBox );
	m_pList->SetResizable( FALSE );
	m_pList->SetRefresh(TRUE);
	m_pList->SetCurSelectedRow(GT_INVALID);
	ShowMaster();
	return 0;
}

// 查看我的徒弟界面
DWORD MasterFrame::OnNetGetStudentInfo(tagNS_ApprenticeInfo *pNetCmd, DWORD)
{
	if(!P_VALID(m_pWnd))
		return 0;
	// 师尊
	if( pNetCmd->sMasterInfo.dwRoleID != GT_INVALID )
	{
		tstring strName = PlayerNameTab::Inst()->FindNameByID( pNetCmd->sMasterInfo.dwRoleID );
		m_pStcName->SetText( strName.c_str() ); // 名字
		tstringstream level;
		level << pNetCmd->sMasterInfo.nLevel;
		m_pStcLevel->SetText( level.str().c_str() ); // 等级
		tstringstream Point;
		Point << pNetCmd->sMasterInfo.dwJingWuPoint;
		m_pStcNumOfQingWu->SetText( Point.str().c_str() );	// 师傅授业点数
		tstringstream QinWuPoint;
		QinWuPoint << pNetCmd->sMasterInfo.dwQinWuPoint;
		m_pStcNumOfQinWu->SetText( QinWuPoint.str().c_str() );	//	师傅师德点数
		MasterMgr::inst()->SetJingWuPoint( pNetCmd->sMasterInfo.dwJingWuPoint );
		MasterMgr::inst()->SetQinWuPoint( pNetCmd->sMasterInfo.dwQinWuPoint );
	}

	// 我的徒弟
	m_pList->Clear();
	m_vecMyStudent.clear();
	m_dwTotalRow = 0;
	for( int i = 0; i < pNetCmd->nNum; i++ )
	{
		m_vecMyStudent.push_back(pNetCmd->sApprenticeInfo[i]);
		if( pNetCmd->sApprenticeInfo[i].nLevel >= GOOD_STU_CAN_LEAVE_LEVEL )
		{
			tagRect rcRect(0, 0, 22, 22);
			m_pList->SetItemPic( m_dwTotalRow, 0, _T("data\\ui\\teach\\l-baishi.dds"), rcRect); // 出师标志
		}

		DWORD dwColor = GT_INVALID;
		tstring strName = PlayerNameTab::Inst()->FindNameByID( pNetCmd->sApprenticeInfo[i].dwRoleID );
		m_pList->SetText( m_dwTotalRow, 1, strName.c_str(), dwColor ); // 名字
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("Role_Level")], pNetCmd->sApprenticeInfo[i].nLevel);
		m_pList->SetText( m_dwTotalRow, 2, szTmp, dwColor ); // 等级
		// 上次做师徒任务时间
		m_pList->SetText( m_dwTotalRow, 3, _T("----"), dwColor );
		// 上次离线时间
		m_pList->SetText( m_dwTotalRow, 4, GetOfflineTime( pNetCmd->sApprenticeInfo[i].bIsOnLine, false, dwColor, pNetCmd->sApprenticeInfo[i].lastOnLineTime).c_str(), dwColor );
		m_dwTotalRow++;
	}
	tagPoint ptSizeOldListBox = m_pList->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pList->GetRowHeight() * m_dwTotalRow;
	m_pList->SetResizable( TRUE );
	m_pList->Resize( ptSizeListBox );
	m_pList->SetResizable( FALSE );
	m_pList->SetRefresh(TRUE);
	m_pList->SetCurSelectedRow(GT_INVALID);
	ShowStudent();
	return 0;
}

// 获取离线时间显示
tstring MasterFrame::GetOfflineTime( bool bIsOnline, bool bMasterTask, DWORD &dwColor, DWORD dwOfflineTime /*= GT_INVALID*/ )
{
	if( bIsOnline && !bMasterTask )			// 在线
	{
		return g_StrTable[_T("GuildList_Online")];
	}

	// 获取当前时间和离线时间
	tagDWORDTime srcTime = (tagDWORDTime)dwOfflineTime;
	tagDWORDTime dstTime = ServerTime::Inst()->CalCurrentServerDwordTime();
	// 计算过了多少秒
	DWORD dwSecond = CalcTimeDiff(dstTime, srcTime);

	// 超过7天未作师徒任务
	if( bMasterTask && dwSecond > 7*24*60*60 )
		dwColor = 0xFFFF0000;
	// 三天未上线
	else if( !bMasterTask && dwSecond > 24*60*60 )
		dwColor = 0xFFFF0000;

	if( dwSecond > 365*24*60*60 )			// 大于1年
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Year")], dwSecond/(365*24*60*60));
		return szTmp;
	}
	else if( dwSecond > 30*24*60*60 )		// 大于1个月
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Month")], dwSecond/(30*24*60*60));
		return szTmp;
	}
	else if( dwSecond > 24*60*60 )			// 大于1天
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Day")], dwSecond/(24*60*60));
		return szTmp;
	}
	else if( dwSecond > 60*60 )				// 大于1小时
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("GuildList_Offline_Hour")], dwSecond/(60*60));
		return szTmp;
	}
	else									// 小于一小时
	{
		return g_StrTable[_T("GuildList_Offline_Min")];
	}
}

// 显示分配授业点数界面
void MasterFrame::ShowAssignWnd()
{
	tstring strName = PlayerNameTab::Inst()->FindNameByID(m_dwRoleID);
	m_pStcStuName->SetText(strName.c_str());
	tstringstream strNeedJingWu;
	strNeedJingWu << MasterMgr::inst()->NeedJingWuCurLevel(m_vecMyStudent[m_dwClickRow].nLevel);
	m_pStcJingWu->SetText(strNeedJingWu.str().c_str());
	tstringstream strTotalJingWu;
	strTotalJingWu << MasterMgr::inst()->GetMyJingWu();
	m_pStcTotalJingWu->SetText( strTotalJingWu.str().c_str() );
	m_pEdtAssignJingWu->SetMaxValue( MasterMgr::inst()->GetMyJingWu() );
	m_pAssignWnd->SetInvisible(false);
	m_pAssignWnd->FlipToTop();
}

// 使用授业点数界面
void MasterFrame::ShowUseWnd()
{
	tstringstream info;
	TCHAR szTemp[X_LONG_NAME];
	INT nLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
	_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("Current_Level")], nLevel );
	info << szTemp << _T("\\n");
	_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("LevelUp_Point")], MasterMgr::inst()->NeedJingWuCurLevel(nLevel), MasterMgr::inst()->GetMyJingWu() );
	info << szTemp << _T("\\n");
	INT nNum = 0;
	ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
	nNum = pContainer->GetItemQuantity(APPRENTICE_LEVEL_UP_ITEM);
	_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("LevelUp_YuanBao")], MasterMgr::inst()->NeedItemNumCurLevel(nLevel), nNum );
	info << szTemp << _T("\\n");
	m_pStcUseInfo->SetText(info.str().c_str());

	m_pGUI->SetModal(m_pUseWnd);
	m_pUseWnd->SetInvisible(false);
	m_pUseWnd->FlipToTop();
}

bool MasterFrame::CanAssignJingWuPoint()
{
	// 未选中弟子，或者弟子已出师
	if( m_dwClickRow == GT_INVALID || m_vecMyStudent[m_dwClickRow].nLevel >= GOOD_STU_CAN_LEAVE_LEVEL )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SelectRightStu")]);
		return false;
	}
	// 弟子不在线
	else if( !m_vecMyStudent[m_dwClickRow].bIsOnLine )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("StuOffline")]);
		return false;
	}
	// 授业点数为零
	else if( 0 == MasterMgr::inst()->GetMyJingWu() )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NoJingWuPoint")]);
		return false;
	}
	return true;
}

bool MasterFrame::CanUseJingWuPoint()
{
	int nLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
	INT nNum = 0;
	ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
	nNum = pContainer->GetItemQuantity(APPRENTICE_LEVEL_UP_ITEM);
	if( MasterMgr::inst()->NeedJingWuCurLevel(nLevel) > MasterMgr::inst()->GetMyJingWu() )
	{
		if( P_VALID(GetObj("NoEnoughJingWu")) )
			KillObj("NoEnoughJingWu");
		CreateObj( "NoEnoughJingWu", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("NoEnoughJingWu")->Init( _T(""), g_StrTable[_T("NoEnoughJingWu")] );
		return false;
	}
	else if( MasterMgr::inst()->NeedItemNumCurLevel(nLevel) > (DWORD)nNum )
	{
		if( P_VALID(GetObj("NoEnoughItems")) )
			KillObj("NoEnoughItems");
		CreateObj( "NoEnoughItems", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("NoEnoughItems")->Init( _T(""), g_StrTable[_T("NoEnoughItems")] );
		return false;
	}
	return true;
}

DWORD MasterFrame::OnNetUseJingWuPoint( tagNS_UseJingWuPoint *pNetCmd, DWORD )
{
	if( pNetCmd->dwErrorCode == E_Success )
	{
		MasterMgr::inst()->SetJingWuPoint(pNetCmd->nCurJingWuPoint);
		TCHAR szTips[X_LONG_NAME] = {0};
		_sntprintf( szTips, X_LONG_NAME, g_StrTable[_T("UseJingWu")], MasterMgr::inst()->GetMyJingWu() );
		m_pBtnAssign->SetText(szTips);
		ShowUseWnd();
	}
	else
	{
		MasterMgr::inst()->ShowErrMsg(pNetCmd->dwErrorCode);
		//CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("NoEnoughJingWuOrMoney")]);
	}
	return 0;
}

DWORD MasterFrame::OnNetAssignJingWuPoint( tagNS_GiveJingWuPoint *pNetCmd, DWORD )
{
	if( pNetCmd->dwErrorCode == E_Success )
	{
		tagNC_ApprenticeInfo cmd;
		m_pSession->Send(&cmd);
		CombatSysUtil::Inst()->ShowScreenCenterMsg( g_StrTable[_T("AssignJingWu_Success")] );
	}
	else
		MasterMgr::inst()->ShowErrMsg( pNetCmd->dwErrorCode );
	return 0;
}


DWORD MasterFrame::OnNS_BreakOutApprentice( tagNS_BreakOutApprentice *pNetCmd, DWORD )
{
	if( pNetCmd->dwErrorCode == E_Success )
	{
		if( m_dwRoleID != GT_INVALID )
		{
			tstring strName = PlayerNameTab::Inst()->FindNameByID(m_dwRoleID);
			if( P_VALID(GetObj("LeaveApprentice")) )
				KillObj("LeaveApprentice");
			CreateObj( "LeaveApprentice", "MsgBox" );
			TCHAR szTips[X_LONG_NAME];
			_sntprintf( szTips, X_LONG_NAME, g_StrTable[_T("OnLeaveApprentice")], strName.c_str() );
			TObjRef<MsgBox>("LeaveApprentice")->Init( _T(""), szTips, _T("OnLeaveApprentice") );
		}
	}
	else
	{
		if( P_VALID(GetObj("CanNotBreakOutApprentice")) )
			KillObj("CanNotBreakOutApprentice");
		CreateObj( "CanNotBreakOutApprentice", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("CanNotBreakOutApprentice")->Init( _T(""), g_StrTable[_T("CanNotBreakOutApprentice")] );
	}
	return 0;
}

DWORD MasterFrame::OnNS_BreakOutApprenticeConfirm( tagNS_BreakOutApprenticeConfirm *pNetCmd, DWORD )
{
	if( pNetCmd->dwRoleID != GT_INVALID )
	{
		LocalPlayer *pPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( pPlayer->GetID() == pNetCmd->dwRoleID )
		{
			pPlayer->SetMasterID(GT_INVALID);
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("IBreakOutMaster")]);
		}
		else
		{
			tstring strName = PlayerNameTab::Inst()->FindNameByID( pNetCmd->dwRoleID );
			TCHAR szTips[X_LONG_NAME];
			_sntprintf( szTips, X_LONG_NAME, g_StrTable[_T("BreakOutMaster")], strName.c_str() );
			CombatSysUtil::Inst()->ShowScreenCenterMsg(szTips);
		}
		
		tagNC_ApprenticeInfo cmd;
		m_pSession->Send(&cmd);
		ShowFrame();
	}
	return 0;
}

DWORD MasterFrame::OnLeaveApprentice( tagMsgBoxEvent *pGameEvent )
{
	if( pGameEvent->eResult == MBF_OK )
	{
		tagNC_BreakOutApprenticeConfirm cmd;
		cmd.dwRoleID = m_dwRoleID;
		m_pSession->Send(&cmd);
	}
	return 0;
}

DWORD MasterFrame::OnNS_BreakOutMaster( tagNS_BreakOutMaster *pNetCmd, DWORD )
{
	if( pNetCmd->dwErrorCode == E_Success )
	{
		if( P_VALID(GetObj("LeaveMaster")) )
			KillObj("LeaveMaster");
		CreateObj( "LeaveMaster", "MsgBox" );
		TObjRef<MsgBox>("LeaveMaster")->Init( _T(""), g_StrTable[_T("OnLeaveMaster")], _T("OnLeaveMaster") );
	}
	else
	{
		if( P_VALID(GetObj("CanNotBreakOutMaster")) )
			KillObj("CanNotBreakOutMaster");
		CreateObj( "CanNotBreakOutMaster", "MsgBox" );
		TObjRef<MsgBox>("CanNotBreakOutMaster")->Init( _T(""), g_StrTable[_T("CanNotBreakOutMaster")], _T("OnForceLeaveMaster") );
	}
	return 0;
}

// 脱离师门
DWORD MasterFrame::OnNS_ForceBreakOutMaster( tagNS_ForceBreakOutMaster *pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode == E_Success)
	{
		if( P_VALID(GetObj("ForceBreakOutMaster")) )
			KillObj("ForceBreakOutMaster");
		CreateObj( "ForceBreakOutMaster", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("ForceBreakOutMaster")->Init( _T(""), g_StrTable[_T("ForceBreakOutMaster")]);
	}
	else if (pNetCmd->dwErrorCode == E_MA_Already_Force_BreakOut)
	{
		TCHAR szTemp[X_LONG_NAME] = {0};
		_sntprintf(szTemp, sizeof(szTemp)/sizeof(TCHAR), g_StrTable[_T("AlreadyForceBreakOutMaster")], pNetCmd->dwRemainTime);

		if( P_VALID(GetObj("ForceBreakOutMaster")) )
			KillObj("ForceBreakOutMaster");
		CreateObj( "ForceBreakOutMaster", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("ForceBreakOutMaster")->Init( _T(""), szTemp);
	}
	return 0;
}

DWORD MasterFrame::OnNS_BreakOutMasterConfirm( tagNS_BreakOutMasterConfirm *pNetCmd, DWORD )
{
	if( pNetCmd->dwRoleID != GT_INVALID )
	{
		TCHAR szTips[X_LONG_NAME];
		LocalPlayer *pPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( pPlayer->GetID() == pNetCmd->dwRoleID )
		{
			pPlayer->SetMasterID(GT_INVALID);
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("IBreakOutMaster")]);
		}
		else
		{
			tstring strName = PlayerNameTab::Inst()->FindNameByID( pNetCmd->dwRoleID );
			_sntprintf( szTips, X_LONG_NAME, g_StrTable[_T("BreakOutMaster")], strName.c_str() );
			CombatSysUtil::Inst()->ShowScreenCenterMsg(szTips);
		}

		tagNC_MatesInfo cmd;
		m_pSession->Send(&cmd);
		ShowFrame();

		m_pBtnAssign->SetEnable(false);
		m_pBtnLeave->SetEnable(false);
		_sntprintf( szTips, X_LONG_NAME, g_StrTable[_T("UseJingWu")], 0 );
		m_pBtnAssign->SetText(szTips);
	}
	return 0;
}

DWORD MasterFrame::OnLeaveMaster( tagMsgBoxEvent *pGameEvent )
{
	if( pGameEvent->eResult == MBF_OK )
	{
		tagNC_BreakOutMasterConfirm cmd;
		cmd.dwRoleID = RoleMgr::Inst()->GetLocalPlayer()->GetMasterID();
		m_pSession->Send(&cmd);
	}
	return 0;
}

DWORD MasterFrame::OnForceLeaveMaster( tagMsgBoxEvent *pGameEvent )
{
	if( pGameEvent->eResult == MBF_OK )
	{
		tagNC_ForceBreakOutMaster cmd;
		cmd.dwRoleID = RoleMgr::Inst()->GetLocalPlayer()->GetMasterID();
		m_pSession->Send(&cmd);
	}
	return 0;
}