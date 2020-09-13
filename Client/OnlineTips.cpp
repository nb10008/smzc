#include "stdafx.h"
#include "OnlineTips.h"
#include "QuestMainFrame.h"
#include "CharacterFrame.h"
#include "TalentFrame.h"
#include "QuestMgr.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "CurrencyMgr.h"
#include "Container.h"
#include "ItemMgr.h"
#include "FlashPushButton.h"
#include "ToolTipFrame.h"
#include "NetSession.h"
#include "SocialData.h"
#include "SocialMgr.h"
#include "SocialListFrame.h"
#include "GuildFrame.h"
#include "MasterFrame.h"
#include "WorldFrame.h"
#include "GuildMgr.h"

OnlineTips::OnlineTips()
	: m_Trunk(this)
	, m_pWnd(NULL)
	, m_pBtnNotCompleteQuestsNum(NULL)
	, m_pBtnCanAssignAttNum(NULL)
	, m_pBtnCanAssignTalentNum(NULL)
	, m_pBtnBBYuanBaoNum(NULL)
	,m_pBtnTodayFocus(NULL)            // 今日热门活动
	,m_bFirstClick(true)
	,m_pStcFriendsOL(NULL)            //在线好友
	,m_iFriendsNumOL(0)
	,m_pStcGuildMemberOL(NULL)            //在线同帮
	,m_iGuildMemberNumOL(0)
	,m_pStcClassMateOL(NULL)            //在线师门
	,m_iClassMateNumOL(0)
	, m_pBtnClose(NULL)
	, m_bShow(false)
	, m_nLastAttNum(0)
	, m_nLastTalentNum(0)
	,m_BnFriendsOnline(NULL)
	,m_BnGuildOnline(NULL)
	,m_BnTeacherOnline(NULL)
{
	
}

OnlineTips::~OnlineTips()
{

}

BOOL OnlineTips::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), 
		m_Trunk.sfp1(&OnlineTips::EventHandler));

	m_pGameFrameMgr->RegisterEventHandle(_T("Refresh_Onlinetips"),  (FRAMEEVENTPROC)m_Trunk.sfp1(&OnlineTips::OnRefreshTips));
	m_pCmdMgr->Register("NS_SyncOnlineNum", (NETMSGPROC)m_Trunk.sfp2(&OnlineTips::NetRecvServerOLInfo), _T("NS_SyncOnlineNum") );

	ShowWnd(FALSE);
	return TRUE;
}

BOOL OnlineTips::Destroy()
{
	GameFrame::Destroy();
	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName().c_str(), 
		m_Trunk.sfp1(&OnlineTips::EventHandler));
	m_pGameFrameMgr->UnRegisterEventHandler(_T("Refresh_Onlinetips"),  
		(FRAMEEVENTPROC)m_Trunk.sfp1(&OnlineTips::OnRefreshTips));
	m_pCmdMgr->UnRegister("NS_SyncOnlineNum", (NETMSGPROC)m_Trunk.sfp2(&OnlineTips::NetRecvServerOLInfo));
	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	return TRUE;
}

BOOL OnlineTips::ReloadUI()
 {
	if (P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);

	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\onlinetips1.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "onlinetips_win\\onlinetips_backpic\\butt2", "ClassName", "FlashButton");
	m_pGUI->ChangeXml(&ele, "onlinetips_win\\onlinetips_backpic\\butt3", "ClassName", "FlashButton");
	m_pGUI->ChangeXml(&ele, "onlinetips_win\\onlinetips_backpic\\butt5", "ClassName", "FlashButton");
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	GUIPatch *pPatch = static_cast<GUIPatch*>(m_pWnd->GetChild(_T("onlinetips_backpic")));
	m_pBtnNotCompleteQuestsNum = static_cast<GUIButton*>(pPatch->GetChild(_T("butt1")));
	m_pBtnCanAssignAttNum = static_cast<FlashButton*>(pPatch->GetChild(_T("butt2")));
	m_pBtnCanAssignTalentNum = static_cast<FlashButton*>(pPatch->GetChild(_T("butt3")));
	m_pBtnBBYuanBaoNum = static_cast<GUIButton*>(pPatch->GetChild(_T("butt4")));
	m_pBtnTodayFocus = static_cast<FlashButton*>(pPatch->GetChild(_T("butt5")));
	m_pBtnTodayFocus->SetFlash(true);
	m_pBtnTodayFocus->SetText(g_StrTable[_T("OnlineTips_TodayFocus")]);
	m_pBtnTaskQuery = static_cast<FlashButton*>(pPatch->GetChild(_T("butt6")));
	m_pBtnTaskQuery->SetText(g_StrTable[_T("OnlineTips_TaskQuery")]);
	m_pBtnClose = static_cast<GUIButton*>(pPatch->GetChild(_T("closebutt")));
	m_pStcFriendsOL = static_cast<GUIStatic*>(pPatch->GetChild(_T("num1")));        //在线好友
	m_pStcGuildMemberOL= static_cast<GUIStatic*>(pPatch->GetChild(_T("num2")));        //在线同帮
	m_pStcClassMateOL  = static_cast<GUIStatic*>(pPatch->GetChild(_T("num3")));        //在线师门

	m_BnFriendsOnline = static_cast<GUIButton*>(pPatch->GetChild(_T("friendsol")));
	m_BnGuildOnline = static_cast<GUIButton*>(pPatch->GetChild(_T("partyol")));
	m_BnTeacherOnline = static_cast<GUIButton*>(pPatch->GetChild(_T("teachol")));

	return TRUE;
}

void OnlineTips::ShowWnd(BOOL bShow)
{
	m_pWnd->SetInvisible(!bShow);
	m_bShow = bShow;
}

DWORD OnlineTips::EventHandler(tagGUIEvent *pEvent)
{
	GUIWnd *pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnNotCompleteQuestsNum)
			{
				m_pMgr->SendEvent( &tagGameEvent( _T("Open_Task_UI"), this ) );
				m_pMgr->SendEvent(&tagGameEvent(_T("QuestMainFrameEvent_CloseNPC"), this));
				QuestMgr::Inst()->OpenNPCFrame( _T("QuestMain"), _T("QuestNPCQuestMain"), "\\desktop\\task\\taskwin", "npclist_quest" );
			}
			if (pWnd == m_pBtnCanAssignAttNum)
			{
				m_pBtnCanAssignAttNum->SetFlash(false);
				m_pMgr->SendEvent( &tagGameEvent( _T("Open_Role_UI"), this ) );
			}
			if (pWnd == m_pBtnCanAssignTalentNum)
			{
				m_pBtnCanAssignTalentNum->SetFlash(false);
				m_pMgr->SendEvent( &tagGameEvent( _T("Open_Dower_UI"), this ) );
			}
			if (pWnd == m_pBtnBBYuanBaoNum)
			{
				GameFrame* pFrame = m_pGameFrameMgr->GetFrame(_T("Throne"));
				if(!P_VALID(pFrame))
				{
					m_pGameFrameMgr->CreateFrame(m_strName.c_str(), _T("Throne"), _T("ThroneFrame"), 0);
				}
			}
			if (pWnd == m_pBtnTodayFocus)
			{ 
			   //今日热门
				if (m_bFirstClick)
				{
					m_bFirstClick = false;
					m_pBtnTodayFocus->SetFlash(false);
				}
				tstring		 szUrl;
				szUrl = TObjRef<NetSession>()->GetTodayFocusUrl();
				if(szUrl == _T("") )
				{
					MessageBox(NULL, _T("Can't find today Focus url!"), _T(""), MB_OK);
				}
				ShellExecute(NULL, _T("open"), szUrl.c_str(), NULL, NULL, SW_MAXIMIZE);
			}
			if (pWnd == m_pBtnClose)
			{
				ShowWnd(FALSE);
			}
			if (pWnd == m_BnFriendsOnline)
			{
				SocialListFrame *pSocialListFrame = (SocialListFrame*)m_pGameFrameMgr->GetFrame(_T("SocialList"));
				if(!P_VALID(pSocialListFrame))
				{
					m_pGameFrameMgr->SendEvent( &tagGameEvent( _T("Open_Sociality"), this ) );
				}
				else
				{
					pSocialListFrame->Show();
				}		
			}
			if (pWnd == m_BnGuildOnline)
			{
				LocalPlayer* pLP = RoleMgr::Inst()->GetLocalPlayer();
				if (!P_VALID(pLP))
					break;
				if (!pLP->IsInGuild())
					break;
					
				GuildFrame *pGuildFrame = (GuildFrame*)m_pGameFrameMgr->GetFrame(_T("Guild"));
				if(!P_VALID(pGuildFrame))
				{
					m_pGameFrameMgr->CreateFrame(_T("World"), _T("Guild"), _T("GuildFrame"), 0);
					m_pGameFrameMgr->SendEvent(&tagGameEvent(_T("Open_Faction"), this));
				}
				else
				{
					if (!pGuildFrame->IsShow())
					{
						m_pGameFrameMgr->SendEvent(&tagGameEvent(_T("ShowGuildFrame"), NULL));
					}
				}
					
			}
			if( pWnd == m_pBtnTaskQuery )                             // 活动任务查询
			{
				if( QuestMgr::Inst()->IsActiveFrameOpened() )
					QuestMgr::Inst()->CloseActiveFrame();
				else
					QuestMgr::Inst()->OpenActiveFrame();
				m_pGameFrameMgr->SendEvent(&tagGameEvent(_T("Refresh_QueryBtn_Flash"),NULL));
			}
			if (pWnd == m_BnTeacherOnline)
			{
				MasterFrame *pMasterFrame = (MasterFrame*)m_pGameFrameMgr->GetFrame(_T("Master"));
				if(!P_VALID(pMasterFrame))
				{
					MasterFrame *pMasterFrame = (MasterFrame*)(TObjRef<GameFrameMgr>()->CreateFrame(_T("World"), _T("Master"), _T("MasterFrame"), 0));
					//pMasterFrame->ShowFrame();
				}	
			}
		} 
		break;
	default : 
		break;
	}
	return 0;
}

DWORD OnlineTips::OnRefreshTips(tagGUIEvent *pEvent)
{
	RefreshNotCompleteQuestsNum();
	RefreshCanAssignAttNum();
	RefreshCanAssignTalentNum();
	RefreshBBYuanBaoNum();
	RefreshOLNum();
	return 0;
}

DWORD OnlineTips::NetRecvServerOLInfo(tagNS_SyncOnlineNum* pNetCmd, DWORD dwParam)

{
	ESyncOnlineNumType  recvOLType=pNetCmd->eSyncOnlineNumType;

	switch (recvOLType)
	{
	case ESOR_Online_Friend_Num:
		m_iFriendsNumOL = pNetCmd->nNum;
		break;
	case ESOR_Online_GuildMember_Num:
		m_iGuildMemberNumOL = pNetCmd->nNum;
		break;
	case ESOR_Online_ClassMate_Num:
		m_iClassMateNumOL = pNetCmd->nNum;
		break;
	}
	
	RefreshOLNum();
	
	return 0;
}

void OnlineTips::RefreshOLNum()
{
	
	TCHAR szTmp[X_SHORT_NAME] = {0};
	//计算并显示在线好友
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_iFriendsNumOL);
	m_pStcFriendsOL->SetText(szTmp);  
	//计算并显示在线同帮   
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_iGuildMemberNumOL);
	m_pStcGuildMemberOL->SetText(szTmp);
	//计算并显示在线师门   
	_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("%d"), m_iClassMateNumOL);
	m_pStcClassMateOL->SetText(szTmp);
	//m_pGameFrameMgr->SendEvent(&tagGameEvent( _T("Refresh_Onlinetips"), NULL ));
    m_pStcFriendsOL->SetRefresh(TRUE);
	m_pStcGuildMemberOL->SetRefresh(TRUE);
	m_pStcClassMateOL->SetRefresh(TRUE);
}
void OnlineTips::RefreshNotCompleteQuestsNum()
{
	
	QuestQuery *pQQ = QuestMgr::Inst()->GetQuery();
	ASSERT( P_VALID(pQQ) );
	const QuestQuery::QuestsMap &QuestMap =  pQQ->GetCurrentQuests();
	int nNum = QuestMap.size(); 

	for( QuestQuery::QuestsMap::const_iterator iter = QuestMap.begin(); iter!=QuestMap.end(); ++iter )
	{
		//if( pQQ->UpdateQuestState( iter->first ) )
		if (pQQ->QuestCanComplete(iter->first) || pQQ->QuestCanCompleteBySpecItem(iter->first) )
		{
			nNum--;
		}	
	}	
	TCHAR buffer[20];
	_sntprintf(buffer, 20, g_StrTable[_T("OnlineTips_NotCompleteQuestsNum")], nNum);
	m_pBtnNotCompleteQuestsNum->SetText(buffer);
}

void OnlineTips::RefreshCanAssignAttNum()
{
	int nNum = RoleMgr::Inst()->GetLocalPlayer()->GetAttribute(ERA_AttPoint);
	TCHAR buffer[20];
	_sntprintf(buffer, 20, g_StrTable[_T("OnlineTips_CanAssignAttNum")], nNum);
	m_pBtnCanAssignAttNum->SetText(buffer);
	if( 0 != nNum && nNum > m_nLastAttNum )
	{
		m_pBtnCanAssignAttNum->SetFlash(true);
		//m_pBtnCanAddAttNum->SetInvisible(FALSE);      //显示小按钮
		m_pMgr->SendEvent( &tagGameEvent(_T("ShowAddAtt_Event"),this));  //发送显示属性投点按钮信息 
	}
	else if( 0 == nNum )
	{
		m_pBtnCanAssignAttNum->SetFlash(false);
		//m_pBtnCanAddAttNum->SetInvisible(TRUE);       //隐藏小按钮
		m_pMgr->SendEvent( &tagGameEvent(_T("HideAddAtt_Event"),this));  //发送隐藏属性投点按钮信息
	}
	m_nLastAttNum = nNum;
}

void OnlineTips::RefreshCanAssignTalentNum()
{
	int nNum = RoleMgr::Inst()->GetLocalPlayer()->GetAttribute(ERA_TalentPoint);
	TCHAR buffer[20];
	_sntprintf(buffer, 20, g_StrTable[_T("OnlineTips_CanAssignTalentNum")], nNum);
	m_pBtnCanAssignTalentNum->SetText(buffer);
	if( 0 != nNum && nNum > m_nLastTalentNum )
	{
		m_pBtnCanAssignTalentNum->SetFlash(true);
		//m_pBtnCanAddTanlentNum->SetInvisible(FALSE);  //显示小按钮
		m_pMgr->SendEvent( &tagGameEvent(_T("ShowAddTelent_Event"),this));  //发送显示天资投点按钮信息
	}
	else if( 0 == nNum )
	{
		m_pBtnCanAssignTalentNum->SetFlash(false);
		//m_pBtnCanAddTanlentNum->SetInvisible(TRUE);  //隐藏小按钮
		m_pMgr->SendEvent( &tagGameEvent(_T("HideAddTelent_Event"),this));  //发送隐藏天资投点按钮信息
	}
	m_nLastTalentNum = nNum;
}

BOOL OnlineTips::EscCancel()
{
	if( m_pWnd->IsInvisible() )
		return FALSE;

	ShowWnd(FALSE);
	return TRUE;
}

void OnlineTips::RefreshBBYuanBaoNum()
{
	INT nYuanBao = CurrencyMgr::Inst()->GetBaibaoYuanbao();
	TCHAR szBuffer[20];
	if (nYuanBao > 99)
	{
		_sntprintf(szBuffer, 20, g_StrTable[_T("OnlineTips_BBYuanBaoNum2")]);
	}
	else
	{
		_sntprintf(szBuffer, 20, g_StrTable[_T("OnlineTips_BBYuanBaoNum1")], nYuanBao);
	}
	m_pBtnBBYuanBaoNum->SetText(szBuffer);
}