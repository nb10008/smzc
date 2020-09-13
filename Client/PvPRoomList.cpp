#include "stdafx.h"

#include "PvPRoomList.h"
#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "PvPMgr.h"
#include "PvPRoom.h"
#include "PvPEvent.h"

PvPRoomList::PvPRoomList(E_PvP_RoomType hallType, E_PvP_RoomLevel pvpLevel)
:m_eHallType(hallType), m_eLevel(pvpLevel)
{
	m_lastUpdate = 0;
	m_bInitialized = FALSE;
}

PvPRoomList::~PvPRoomList()
{
	std::list<PvPRoom*> roomList;
	m_RoomMap.ExportAllValue(roomList);
	m_RoomMap.Clear();
	for(std::list<PvPRoom*>::iterator itor = roomList.begin(); itor!= roomList.end(); itor++)
		delete *itor;
	roomList.clear();
}

void PvPRoomList::Update()
{
	if(m_lastUpdate != 0 && CalcTimeDiff(GetCurrentDWORDTime(), m_lastUpdate) < PvPMgr::Inst()->MinRefreshInterval())
		return;
	ForceUpdate();
}

void PvPRoomList::ForceUpdate()
{
	tagNC_PvPGetRoomList msg;
	msg.eRoomLevel = m_eLevel;
	m_pSession->Send(&msg);

	m_lastUpdate = GetCurrentDWORDTime();
}

void PvPRoomList::UpdateRoomList(DWORD nCount, INT16* pRoomPoss)
{
	std::list<INT16> roomIDs, newRoomIDs;
	m_RoomMap.ExportAllKey(roomIDs);
	for(DWORD i=0; i<nCount; i++)
		newRoomIDs.push_back(pRoomPoss[i]);
	roomIDs.sort(); newRoomIDs.sort();
	bool bChanged = false;
	std::list<INT16>::iterator i=roomIDs.begin(), j=newRoomIDs.begin();
	while(i!= roomIDs.end() && j != newRoomIDs.end())
	{
		if(*i < *j) // delete
		{
			delete m_RoomMap.Peek(*i);
			m_RoomMap.Erase(*i);
			bChanged = true;
			i++;			
		}else if(*i > *j) // add
		{
			m_RoomMap.Add(*j, new PvPRoom(m_eHallType, m_eLevel, *j));
			bChanged = true;
			j++;
		}
		else
		{
			i++;
			j++;
		}
	}

	while(i!= roomIDs.end()) // delete
	{
		delete m_RoomMap.Peek(*i);
		m_RoomMap.Erase(*i);
		bChanged = true;
		i++;			
	}
	while(j!= newRoomIDs.end()) // add
	{
		m_RoomMap.Add(*j, new PvPRoom(m_eHallType, m_eLevel, *j));
		bChanged = true;
		j++;			
	}

	m_lastUpdate = GetCurrentDWORDTime();
	m_bInitialized = TRUE;

	tagRoomListChange evt;
	evt.eHallType = m_eHallType;
	evt.eLevel = m_eLevel;
	m_pFrameMgr->SendEvent(&evt);
}

PvPRoom* PvPRoomList::AddRoom(INT16 roomID)
{
	PvPRoom* pRoom = NULL;
	if(!m_RoomMap.IsExist(roomID))
	{
		pRoom = new PvPRoom(m_eHallType, m_eLevel, roomID);
		m_RoomMap.Add(roomID, pRoom);
		tagRoomListChange evt;
		evt.eHallType = m_eHallType;
		evt.eLevel = m_eLevel;
		m_pFrameMgr->SendEvent(&evt);
	}
	else
	{
		pRoom = m_RoomMap.Peek(roomID);
	}
	return pRoom;
}

std::list<INT16> PvPRoomList::GetOrderedRoomList()const
{
	std::list<INT16> roomList;
	const_cast<TSafeMap<INT16, PvPRoom*>*>(&m_RoomMap)->ExportAllKey(roomList);
	roomList.sort();
	return roomList;
}

PvPRoom* PvPRoomList::GetRoom(INT16 pos)const
{
	return const_cast<TSafeMap<INT16, PvPRoom*>*>(&m_RoomMap)->Peek(pos);
}

void PvPRoomList::RemoveInvalid(const std::list<INT16>& roomIDs)
{	
	tagInvalidRoomEvent evt;
	for(std::list<INT16>::const_iterator itor= roomIDs.begin(); itor != roomIDs.end(); itor++)
	{
		PvPRoom* pRoom = m_RoomMap.Peek(*itor);
		if(P_VALID(pRoom))
		{
			m_RoomMap.Erase(*itor);		
			evt.nRoomIDs.push_back(*itor);
			delete pRoom;
		}
	}
	if(evt.nRoomIDs.size()>0)
	{
		evt.eHallType = m_eHallType;
		evt.eLevel = m_eLevel;
		m_pFrameMgr->SendEvent(&evt);
	}
}