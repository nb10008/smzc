#include "stdafx.h"
#include "../WorldDefine/athletics_define.h"
#include "PvPRoomList.h"
#include "PvPHall.h"

PvPHall::PvPHall(E_PvP_RoomType eType)
{
	m_eType = eType;
	m_pSeniorRoomList = new PvPRoomList(m_eType, E_PvP_Senior);
	m_pJuniorRoomList = new PvPRoomList(m_eType, E_PvP_Junior);
	m_pMidRoomList = new PvPRoomList(m_eType, E_PvP_Middle);
}

PvPHall::~PvPHall()
{
	delete m_pSeniorRoomList;
	delete m_pJuniorRoomList;
	delete m_pMidRoomList;
}

void PvPHall::Update()
{

}

void PvPHall::UpdateHallInfo(DWORD nCount, DWORD* pdwPlayerIDs)
{

}
