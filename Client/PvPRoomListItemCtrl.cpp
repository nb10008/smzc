#include "stdafx.h"

#include "../WorldDefine/athletics_define.h"
#include "PvPRoomListItemCtrl.h"
#include "PvPEvent.h"
#include "PvPRoom.h"
#include "PvPMgr.h"
#include "IconStatic.h"
#include "RoleMgr.h"

RoomListItemCtrl::RoomListItemCtrl(GUIPatch* pButton, GUIStatic* pPos, GUIStatic* pLevel, GUIStatic* pName, GUIStatic* pNum, IconStatic* pState, IconStatic* pImg, GUIStatic* pLimit, StaticEx* pBackImg)
{
	m_pButton = pButton;	
	m_pPos=	 pPos;
	m_pLevel=	 pLevel;
	m_pName=	 pName;
	m_pNum=	 pNum;
	m_pState=	 pState;
	m_pImg=	 pImg;
	m_pLimit=	 pLimit;
	m_pBackImg = pBackImg;
	m_eHallType = E_PvP_RoomTypeNull;
	m_eLevel = E_PvP_RoomLevelNull;
	m_nRoomID = -1;	
}

void RoomListItemCtrl::SetRoom(E_PvP_RoomType eHallType, E_PvP_RoomLevel eLevel, INT16 nRoomID)
{
	ResetRoom();
	m_eHallType = eHallType;
	m_eLevel = eLevel;
	m_nRoomID = nRoomID;

	PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_eHallType, m_eLevel, m_nRoomID);
	if(P_VALID(pRoom))
	{
		if( pRoom->IsInitialized())
		{
			SetPos(pRoom->GetRoomID());
			SetLevel(pRoom->GetRoomLevel());
			SetName(m_eLevel==E_PvP_Junior && pRoom->GetManagerID()!=RoleMgr::Inst()->GetLocalPlayerID()?_T("***"):pRoom->GetName());
			SetNum(pRoom->GetCount(), pRoom->GetNumLimit());
			SetState(pRoom->GetRoomState());
			SetImg(pRoom->GetWuXunLimit());
			SetLimit(pRoom->GetLevelLowerBound(), pRoom->GetLevelUpperBound());
		}
		else
		{
			SetPos(pRoom->GetRoomID());
			m_pName->SetText(g_StrTable[_T("PvP_UI_NotInitialized")]);
			m_pName->SetRefresh();
			//pRoom->Update();
		}
	}
}

void RoomListItemCtrl::ResetRoom()
{
	m_eHallType = E_PvP_RoomTypeNull;
	m_eLevel = E_PvP_RoomLevelNull;
	m_nRoomID = -1;
	m_pButton->SetText(_T(""));
	m_pButton->SetRefresh();
	m_pPos->SetText(_T(""));
	m_pPos->SetRefresh();
	m_pLevel->SetText(_T(""));
	m_pLevel->SetRefresh();
	m_pName->SetText(_T(""));
	m_pName->SetRefresh();
	m_pNum->SetText(_T(""));
	m_pNum->SetRefresh();
	m_pState->SetText(_T(""));
	m_pState->SetNoPic();
	m_pState->SetRefresh();
	m_pImg->SetText(_T(""));
	m_pImg->SetNoPic();
	m_pImg->SetRefresh();
	m_pLimit->SetText(_T(""));
	m_pLimit->SetRefresh();
}

void RoomListItemCtrl::OnUpdate(tagRoomChange* evt)
{
	if(evt->eHallType == m_eHallType && evt->eLevel==m_eLevel && evt->nRoomID==m_nRoomID)
	{
		PvPRoom* pRoom = PvPMgr::Inst()->GetRoom(m_eHallType, m_eLevel, m_nRoomID);
		if(P_VALID(pRoom) )
		{
			SetPos(pRoom->GetRoomID());
			SetLevel(pRoom->GetRoomLevel());
			SetName(m_eLevel==E_PvP_Junior && pRoom->GetManagerID()!=RoleMgr::Inst()->GetLocalPlayerID()?_T("***"):pRoom->GetName());
			SetNum(pRoom->GetCount(), pRoom->GetNumLimit());
			SetState(pRoom->GetRoomState());
			SetImg(pRoom->GetWuXunLimit());
			SetLimit(pRoom->GetLevelLowerBound(), pRoom->GetLevelUpperBound());
		}
	}
}

void RoomListItemCtrl::SetPos(INT16 pos)
{
	tstringstream ts;
	ts << pos;
	m_pPos->SetText(ts.str().c_str());
	m_pPos->SetRefresh();
}

void RoomListItemCtrl::SetLevel(E_PvP_RoomLevel level)
{
	tstring levelString;
	
	switch(level)
	{
	case E_PvP_Junior:	//初级
		levelString = g_StrTable[_T("PvP_UI_Junior")];
		break;
	case E_PvP_Middle:	//中级
		levelString = g_StrTable[_T("PvP_UI_Mid")];
		break;
	case E_PvP_Senior:	//高级
		levelString = g_StrTable[_T("PvP_UI_Senior")];
		break;
	}
	m_pLevel->SetText(levelString.c_str());
	m_pLevel->SetRefresh();
}

void RoomListItemCtrl::SetName(TCHAR* name)
{
	tstringstream ts;
	ts << name << g_StrTable[_T("PvP_UI_ManagerName")];
	m_pName->SetText(ts.str().c_str());
	m_pName->SetRefresh();
}

void RoomListItemCtrl::SetNum(DWORD count, DWORD limit)
{
	tstringstream ts;
	ts << count << g_StrTable[_T("PvP_UI_NumSplit")] << limit;
	m_pNum->SetText(ts.str().c_str());
	m_pNum->SetRefresh();
}

void RoomListItemCtrl::SetLimit(INT lowerLevel, INT upperLevel)
{
	tstringstream ts;
	ts << lowerLevel << _T("-") << upperLevel << g_StrTable[_T("PvP_UI_Level")];
	m_pLimit->SetText(ts.str().c_str());
	m_pLimit->SetRefresh();
}

void RoomListItemCtrl::SetState(E_PvP_RoomState state)
{
	tstringstream ts;
	switch(state)
	{
	case E_PvP_Waiting:
		ts << _T("data\\ui\\compet\\ready.bmp");
		break;
	case E_PvP_Fighting:
		ts << _T("data\\ui\\compet\\battle.bmp");
		break;
	}
	m_pState->SetPic(ts.str(), tagRect(0, 0, 64, 16));
	m_pState->SetRefresh();
}

void RoomListItemCtrl::SetImg(DWORD wuXun)
{
	tstringstream ts;
	ts << _T("data\\ui\\compet\\sword") << wuXun << _T(".bmp");
	m_pImg->SetPic(ts.str(), tagRect(0, 0, 20, 20));
	m_pImg->SetRefresh();
}
