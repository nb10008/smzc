#include "stdafx.h"

#include "../WorldDefine/athletics_define.h"
#include "PvPRoom.h"
#include "PvPMgr.h"
#include "PvPRoomCtrl.h"
#include "Role.h"
#include "RoleMgr.h"
#include "PvPEvent.h"
#include "CombatSysUtil.h"
#include "SocialData.h"
#include "SocialMgr.h"
#include "PlayerNameTab.h"

PvPRoomCtrl::PvPRoomCtrl(GUIPatch* pTitle, GUIStatic* pName, GUIStatic* pLevel, GUIStatic* pWuxun, GUIButton* pBtnKick, GUIButton* pBtnPrepare, GUIButton* pBtnStart, GUIButton* pBtnLeave, GUIStatic* pFolderName, GUIComboBox* pFriendList)
{
	 m_pTitle = pTitle;
	 m_pName = pName;
	 m_pLevel = pLevel;
	 m_pWuxun = pWuxun;
	 m_pBtnKick = pBtnKick;
	 m_pBtnPrepare = pBtnPrepare;
	 m_pBtnStart = pBtnStart;
	 m_pFolderName = pFolderName;
	 m_pCmb3v3FriendList = pFriendList;
	 m_ePreapareState = E_Unprepared;
	 m_pBtnLeave = pBtnLeave;
}

void PvPRoomCtrl::SetRoom(E_PvP_RoomType eHallType, E_PvP_RoomLevel eLevel, INT16 nRoomID)
{
	ResetRoom();
	m_eHallType = eHallType;
	m_eLevel = eLevel;
	m_nRoomID = nRoomID;

	UpdateUI();
}

void PvPRoomCtrl::ResetRoom()
{
	 m_pTitle->SetText(_T(""));
	 m_pTitle->SetRefresh();
	 m_pName->SetText(_T(""));
	 m_pName->SetRefresh();
	 m_pLevel->SetText(_T(""));
	 m_pLevel->SetRefresh();
	 m_pWuxun->SetText(_T(""));
	 m_pWuxun->SetRefresh();
	 m_pBtnKick->SetEnable(FALSE);
	 m_pBtnKick->SetRefresh();
	 m_pFolderName->SetText(_T(""));
	 m_pFolderName->SetRefresh();
	 m_pBtnPrepare->SetText(g_StrTable[_T("PvP_UI_Prepare")]);
	 m_pBtnPrepare->SetEnable(FALSE);
	 m_pBtnPrepare->SetRefresh();
	 m_pBtnStart->SetEnable(FALSE);
	 m_pBtnStart->SetRefresh();	 
	 m_ePreapareState = E_Unprepared;
}

BOOL PvPRoomCtrl::IsMyRoom()const
{
	if(m_nRoomID>0)
	{
		PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_eHallType, m_eLevel, m_nRoomID);
		if(P_VALID(pRoom))
		{
			return RoleMgr::Inst()->GetLocalPlayerID() == pRoom->GetManagerID();
		}		
	}
	return FALSE;
}

void PvPRoomCtrl::OnUpdate(tagRoomChange* evt)
{
	if(evt->eHallType == m_eHallType && evt->eLevel==m_eLevel && evt->nRoomID==m_nRoomID)
	{
		PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_eHallType, m_eLevel, m_nRoomID);
		if(P_VALID(pRoom))
		{
			PvPRoom::PRoleInfo pRoleInfo = pRoom->GetRoleInfo(RoleMgr::Inst()->GetLocalPlayerID());
			if(P_VALID(pRoleInfo) && pRoleInfo->dwRoleID >0)
				m_ePreapareState = pRoleInfo->eState == E_PvP_Ready? E_Prepared: E_Unprepared;
		}
		UpdateUI();
	}
}

void PvPRoomCtrl::OnFriendChange()
{
	PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_eHallType, m_eLevel, m_nRoomID);
	if(P_VALID(pRoom) && pRoom->IsInitialized())
	{
		RefreshInvitableFriends(pRoom);
	}
}

void PvPRoomCtrl::UpdateUI()
{
	PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_eHallType, m_eLevel, m_nRoomID);
	if(P_VALID(pRoom))
	{
		SetTitle(m_eHallType, m_eLevel, m_nRoomID);
		if( pRoom->IsInitialized())
		{
			SetName(m_eLevel==E_PvP_Junior && !IsMyRoom()?_T("***"):pRoom->GetName());
			SetLimit(pRoom->GetLevelLowerBound(), pRoom->GetLevelUpperBound());
			SetWuxun(pRoom->GetWuXunLimit());		
			SetFolderName(m_eHallType, m_eLevel, m_nRoomID, pRoom->GetCount());
			RefreshInvitableFriends(pRoom);
			if(IsMyRoom())
			{
				m_pBtnKick->SetEnable(TRUE);
				m_pBtnKick->SetRefresh();
				m_pBtnStart->SetEnable(TRUE);
				m_pBtnStart->SetRefresh();
				m_pBtnPrepare->SetText(g_StrTable[_T("PvP_UI_Prepare")]);
				m_pBtnPrepare->SetEnable(FALSE);
				m_pBtnPrepare->SetRefresh();
				m_pBtnLeave->SetEnable(TRUE);
			}
			else
			{
				m_pBtnKick->SetEnable(FALSE);
				m_pBtnKick->SetRefresh();
				m_pBtnStart->SetEnable(FALSE);
				m_pBtnStart->SetRefresh();
				switch(m_ePreapareState)
				{
				case E_Unprepared:
					m_pBtnPrepare->SetText(g_StrTable[_T("PvP_UI_Prepare")]);
					m_pBtnPrepare->SetEnable(TRUE);
					m_pBtnLeave->SetEnable(TRUE);
					break;
				case E_Prepared:
					m_pBtnPrepare->SetText(g_StrTable[_T("PvP_UI_Unprepare")]);
					m_pBtnPrepare->SetEnable(TRUE);
					m_pBtnLeave->SetEnable(FALSE);
					break;
				case E_Preparing:
					m_pBtnPrepare->SetText(g_StrTable[_T("PvP_UI_Prepare")]);
					m_pBtnPrepare->SetEnable(FALSE);
					m_pBtnLeave->SetEnable(FALSE);
					break;
				case E_Unprepring:
					m_pBtnPrepare->SetText(g_StrTable[_T("PvP_UI_Unprepare")]);
					m_pBtnPrepare->SetEnable(FALSE);
					m_pBtnLeave->SetEnable(FALSE);
					break;
				}
				m_pBtnPrepare->SetRefresh();
			}
		}
		else
		{
			m_pName->SetText(g_StrTable[_T("PvP_UI_NotInitialized")]);
			m_pName->SetRefresh();
			pRoom->Update();
		}
	}
	else
	{
		ResetRoom();
	}
}

void PvPRoomCtrl::SetPrepare(EPrepareState prepareState)
{
	m_ePreapareState = prepareState;
	if(!IsMyRoom())
	{
		switch(m_ePreapareState)
		{
		case E_Unprepared:
			m_pBtnPrepare->SetText(g_StrTable[_T("PvP_UI_Prepare")]);
			m_pBtnPrepare->SetEnable(TRUE);
			break;
		case E_Prepared:
			m_pBtnPrepare->SetText(g_StrTable[_T("PvP_UI_Unprepare")]);
			m_pBtnPrepare->SetEnable(TRUE);
			break;
		case E_Preparing:
			m_pBtnPrepare->SetText(g_StrTable[_T("PvP_UI_Prepare")]);
			m_pBtnPrepare->SetEnable(FALSE);
			break;
		case E_Unprepring:
			m_pBtnPrepare->SetText(g_StrTable[_T("PvP_UI_Unprepare")]);
			m_pBtnPrepare->SetEnable(FALSE);
			break;
		}
		m_pBtnPrepare->SetRefresh();
	}
}

void PvPRoomCtrl::SetTitle(E_PvP_RoomType hallType, E_PvP_RoomLevel eLevel, INT16 roomID)
{
	tstringstream ts;
	switch(eLevel)
	{
	case E_PvP_Junior:
		ts << g_StrTable[_T("PvP_UI_Junior")];
		break;
	case E_PvP_Middle:
		ts << g_StrTable[_T("PvP_UI_Mid")];
		break;
	case E_PvP_Senior:
		ts << g_StrTable[_T("PvP_UI_Senior")];
		break;
	}
	switch(hallType)
	{
	case E_PvP_Room3v3:
		ts << _T("3v3") << g_StrTable[_T("PvP_UI_Room")];
		break;
	case E_PvP_Room6v6:
		ts << _T("6v6") << g_StrTable[_T("PvP_UI_Room")];
		break;
	}
	ts << _T(" ") << roomID;
	m_pTitle->SetText(ts.str().c_str());
}

void PvPRoomCtrl::SetName(const TCHAR* pName)
{
	tstringstream ts;
	ts << pName << g_StrTable[_T("PvP_UI_ManagerName")];
	m_pName->SetText(ts.str().c_str());
	m_pName->SetRefresh();
}

void PvPRoomCtrl::SetLimit(INT lowerLevel, INT upperLevel)
{
	tstringstream ts;
	ts << lowerLevel << _T("-") << upperLevel << g_StrTable[_T("PvP_UI_Level")];
	m_pLevel->SetText(ts.str().c_str());
	m_pLevel->SetRefresh();
}

void PvPRoomCtrl::SetWuxun(INT wuXun)
{
	tstringstream ts;
	ts << _T("PvP_UI_WuXin") << wuXun;
	m_pWuxun->SetText(g_StrTable[ts.str().c_str()]);
	m_pWuxun->SetRefresh();
}

void PvPRoomCtrl::SetFolderName(E_PvP_RoomType hallType, E_PvP_RoomLevel eLevel, INT16 roomID, INT nCurrent)
{
	INT nTotal = 0;
	tstringstream ts;
	switch(eLevel)
	{
	case E_PvP_Junior:
		ts << g_StrTable[_T("PvP_UI_Junior")];
		break;
	case E_PvP_Middle:
		ts << g_StrTable[_T("PvP_UI_Mid")];
		break;
	case E_PvP_Senior:
		ts << g_StrTable[_T("PvP_UI_Senior")];
		break;
	}
	switch(hallType)
	{
	case E_PvP_Room3v3:
		ts << "3v3";
		nTotal = 6;
		break;
	case E_PvP_Room6v6:
		ts << "6v6";
		nTotal = 12;
		break;
	}
	ts << g_StrTable[_T("PvP_UI_Room")] << roomID << _T(":") << nCurrent << _T("/") << nTotal;
	m_pFolderName->SetText(ts.str().c_str());
	m_pFolderName->SetRefresh(TRUE);
}

// 刷新房间界面上可邀请的好友列表
void PvPRoomCtrl::RefreshInvitableFriends(PvPRoom* pRoom)
{
	GUIListBox* pList = m_pCmb3v3FriendList->GetListBox();
	pList->Clear();
	INT nRow = 0;
	SocialMgr* pSocialMgr = (SocialMgr*)TObjRef<GameFrameMgr>()->GetFrame(_T("SocialMgr"));
	if( P_VALID(pSocialMgr) )
	{
		SocialData* pData = pSocialMgr->GetData();
		for(int t=SocialData::ELT_Friend1; t<= SocialData::ELT_Friend4; t++)
		{
			const tagFriendInfo* pInfo = pData->GetFriendPtrByIndex( SocialData::EListType(t), 0 );
			for( int i = 0; P_VALID(pInfo); pInfo = pData->GetFriendPtrByIndex(SocialData::EListType(t), ++i))
			{
				if(pInfo->bOnline && pInfo->nLevel>=pRoom->GetLevelLowerBound() && pInfo->nLevel<=pRoom->GetLevelUpperBound()) // 在线且等级合适
				{
					Role* pRole = (Role*)RoleMgr::Inst()->FindRole(pInfo->dwFriendID);
					if(P_VALID(pRole) && pRole->IsAttributeInited())
					{
						pList->SetText(nRow++, 0, pRole->GetRoleName().c_str());
					}
					else
					{
						LPCTSTR szFriendName = PlayerNameTab::Inst()->FindNameByID(pInfo->dwFriendID);
						if(P_VALID(szFriendName) && _tcslen(szFriendName) > 0)
						{
							pList->SetText(nRow++, 0, szFriendName);
						}
					}
				}
			}
		}
	}
	tagPoint ptSizeListBox = pList->GetSize();
	ptSizeListBox.y = pList->GetRowHeight() * 2;
	pList->SetResizable(TRUE);
	pList->Resize(ptSizeListBox);
	pList->SetResizable(FALSE);
}