#include "stdafx.h"
#include "PvPRoomPosCtrl.h"
#include "../WorldDefine/athletics_define.h"
#include "PvPEvent.h"
#include "PvPRoom.h"
#include "PvPMgr.h"
#include "IconStatic.h"
#include "Role.h"
#include "RoleMgr.h"

PvPRoomPosCtrl::PvPRoomPosCtrl(IconStatic* pPos, IconStatic* pState, GUIStatic* pLevel, GUIStatic* pName, BYTE pos)
{
	m_pPos = pPos;
	m_pState= pState;
	m_pLevel = pLevel;
	m_pName = pName;
	m_bNeedUpdate = FALSE;
	m_ucPos = pos;
	m_dwRoleID = 0;
	m_dwLastUpdate = 0;
	ResetPos();
}

void PvPRoomPosCtrl::SetRoom(E_PvP_RoomType	eHallType, E_PvP_RoomLevel eLevel, INT16 nRoomID)
{
	ResetPos();
	m_eHallType = eHallType;
	m_eLevel = eLevel;
	m_nRoomID = nRoomID;	
	UpdateUI();
}

void PvPRoomPosCtrl::ResetPos()
{
	if(P_VALID(m_pPos))
	{
		m_pPos->SetNoPic();
		m_pPos->SetRefresh();
	}
	if(P_VALID(m_pState))
	{
		m_pState->SetNoPic();
		m_pState->SetRefresh();
	}
	if(P_VALID(m_pLevel))
	{
		m_pLevel->SetText(_T(""));
		m_pLevel->SetRefresh();
	}
	if(P_VALID(m_pName))
	{
		m_pName->SetText(g_StrTable[_T("PvP_UI_PosIsEmpty")]);
		m_pName->SetRefresh();
	}
	m_dwRoleID = 0;
}

void PvPRoomPosCtrl::UpdateUI()
{
	PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_eHallType, m_eLevel, m_nRoomID);
	if(P_VALID(pRoom) )
	{
		PvPRoom::PRoleInfo pRoleInfo = pRoom->GetRoleInfoAtPos(m_ucPos);
		if(P_VALID(pRoleInfo) && pRoleInfo->dwRoleID >0)	// 位置有人
		{
			m_dwRoleID = pRoleInfo->dwRoleID;
			Role* pRole = RoleMgr::Inst()->FindRole(pRoleInfo->dwRoleID, true);
			if(P_VALID(pRole) && pRole->IsAttributeInited())
			{
				SetLevel(pRole->GetRoleLevel());
				SetName(m_eLevel==E_PvP_Junior && m_dwRoleID!=RoleMgr::Inst()->GetLocalPlayerID()?_T("***"):pRole->GetRoleName().c_str());
				//SetWuXun(PvPMgr::Inst()->Wuxun2Level(pRole->GetAttribute(ERA_WuXun))); // wuxun is not a remote attribute
			}
			else
			{
				SetName(m_eLevel==E_PvP_Junior && m_dwRoleID!=RoleMgr::Inst()->GetLocalPlayerID()?_T("***"):pRoleInfo->szName);
				SetLevel(pRoleInfo->dwLevel);				
			}
			SetWuXun(PvPMgr::Inst()->Wuxun2Level(pRoleInfo->dwWuXun));
			SetState(pRoleInfo->eState);
			return;	// 设置用户信息
		}
		else
		{
			m_dwRoleID = 0;
		}
	}
	ResetPos();	// 位置为空
}

void PvPRoomPosCtrl::OnUpdate(tagRoomChange* evt)
{
	if(evt->eHallType == m_eHallType && evt->eLevel==m_eLevel && evt->nRoomID==m_nRoomID)
	{
		UpdateUI();
	}
}

void PvPRoomPosCtrl::SetPos(BYTE pos)
{
	/*tstringstream ts;
	ts << _T("data\\ui\\pvp\\position") << pos << _T(".tga");
	if(P_VALID(m_pPos)) 
	{
		m_pPos->SetPic(ts.str());
		m_pPos->SetRefresh();
	}*/
}

void PvPRoomPosCtrl::SetWuXun(DWORD wuXun)
{
	tstringstream ts;
	ts << _T("data\\ui\\compet\\sword") << wuXun << _T(".bmp");
	m_pPos->SetPic(ts.str(), tagRect(0, 0, 20, 20));
	m_pPos->SetRefresh();
}

void PvPRoomPosCtrl::SetState(E_PvP_RoleState state)
{
	tstring ts;
	switch(state)
	{
	case E_PvP_Preparing:       //准备  
		ts = _T("data\\ui\\compet\\ready.bmp");
		break;
	case E_PvP_Ready:           //就绪
		ts = _T("data\\ui\\compet\\complete.bmp");
		break;
	}
	if(P_VALID(m_pState))
	{
		m_pState->SetPic(ts, tagRect(0, 0, 64, 16));
		m_pState->SetRefresh();
	}
}

void PvPRoomPosCtrl::SetName(const TCHAR* name)
{
	tstringstream ts;
	ts << name;
	if(P_VALID(m_pName))
	{
		m_pName->SetText(ts.str().c_str());
		m_pName->SetRefresh();
	}
}

void PvPRoomPosCtrl::SetLevel(DWORD level)
{
	tstringstream ts;
	ts << level << g_StrTable[_T("PvP_UI_Level")];
	if(P_VALID(m_pLevel))
	{
		m_pLevel->SetText(ts.str().c_str());
		m_pLevel->SetRefresh();
	}
}