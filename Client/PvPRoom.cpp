#include "stdafx.h"
#include "../WorldDefine/msg_common_errorcode.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "PvPRoom.h"
#include "PvPEvent.h"
#include "PvPRoomList.h"
#include "PvPMgr.h"

PvPRoom::PvPRoom(E_PvP_RoomType hallType, E_PvP_RoomLevel level, INT16 roomPos)
:m_eHallType(hallType), m_eLevel(level), m_nRoomID(roomPos)
{
	m_bInitialized = FALSE;
	m_eRoomState = E_PvP_RoomStateNull;			//房间状态
	m_dwNumLimit = 0;
	m_iLevelUpperBound = 0;
	m_iLevelLowerBound = 0;
	m_dwWuXunLimit = 0;
	m_dwManagerID = 0;
	m_dwCount = 0;
	m_lastUpdate = 0;
	memset(m_szManagerName, 0, sizeof(m_szManagerName));
}

PvPRoom::~PvPRoom()
{
	std::list<PRoleInfo> roleInfoList;
	m_RoleMap.ExportAllValue(roleInfoList);
	m_RoleMap.Clear();
	for(std::list<PRoleInfo>::iterator itor=roleInfoList.begin(); itor!=roleInfoList.end(); itor++)
		delete *itor;
	roleInfoList.clear();
}

void PvPRoom::RefreshUpdateTime()
{
	m_lastUpdate = GetCurrentDWORDTime();
}

BOOL PvPRoom::NeedUpdate() { return ((DWORD)m_lastUpdate == 0 || CalcTimeDiff(GetCurrentDWORDTime(), m_lastUpdate) > PvPMgr::Inst()->MinRefreshInterval()); }

void PvPRoom::Update()
{
	if(!NeedUpdate())
		return;
	ForceUpdate();
}

void PvPRoom::ForceUpdate()
{	
	m_lastUpdate = GetCurrentDWORDTime();
	tagNC_PvPGetRoomInfo msg;
	msg.eRoomLevel = (E_PvP_RoomLevel)m_eLevel;
	msg.dwCount = 1;
	msg.pData[0] = m_nRoomID;
	m_pSession->Send(&msg);
}

void PvPRoom::Update(const tagPvPSimRoom& roomInfo)
{
	if(roomInfo.eLevel != m_eLevel && roomInfo.nRoomID != m_nRoomID) return;
	bool bChanged = false;
	if(m_eRoomState != roomInfo.eRoomState)
	{
		m_eRoomState = roomInfo.eRoomState;		
		bChanged = true;
	}
	if(m_dwNumLimit != roomInfo.ucTotalPersonNum)
	{
		m_dwNumLimit = roomInfo.ucTotalPersonNum;
		bChanged = true;
	}
	if(m_iLevelUpperBound != roomInfo.iLevelUpperLimit)
	{
		m_iLevelUpperBound = roomInfo.iLevelUpperLimit;
		bChanged = true;
	}
	if(m_iLevelLowerBound != roomInfo.iLevelLowerLimit)
	{
		m_iLevelLowerBound = roomInfo.iLevelLowerLimit;
		bChanged = true;
	}
	if(m_dwWuXunLimit != roomInfo.dwWuXunLimit)
	{
		m_dwWuXunLimit = roomInfo.dwWuXunLimit;
		bChanged = true;
	}
	if(m_dwManagerID != roomInfo.dwManagerID)
	{
		m_dwManagerID = roomInfo.dwManagerID;
		bChanged = true;
	}
	if(m_dwCount != roomInfo.ucCurrentPersonNum)
	{
		m_dwCount = roomInfo.ucCurrentPersonNum;
		bChanged = true;
	}

	if( wcscmp(m_szManagerName, roomInfo.szManagerName) !=0)
	{
		memcpy(m_szManagerName, roomInfo.szManagerName, sizeof(m_szManagerName));
		bChanged = true;
	}

	m_lastUpdate = GetCurrentDWORDTime();
	m_bInitialized = TRUE;

	if(bChanged)
	{
		tagRoomChange evt;
		evt.eHallType = m_eHallType;
		evt.eLevel = m_eLevel;
		evt.nRoomID = m_nRoomID;
		m_pFrameMgr->SendEvent(&evt);
	}	
}

void PvPRoom::Update(const tagPvPRoom& roomInfo)
{	
	if(roomInfo.eLevel != m_eLevel && roomInfo.nRoomID != m_nRoomID) return;
	bool bChanged = false;
	if(m_eRoomState != roomInfo.eRoomState)
	{
		m_eRoomState = roomInfo.eRoomState;		
		bChanged = true;
	}
	if(m_dwNumLimit != roomInfo.ucTotalPersonNum)
	{
		m_dwNumLimit = roomInfo.ucTotalPersonNum;
		bChanged = true;
	}
	if(m_iLevelUpperBound != roomInfo.iLevelUpperLimit)
	{
		m_iLevelUpperBound = roomInfo.iLevelUpperLimit;
		bChanged = true;
	}
	if(m_iLevelLowerBound != roomInfo.iLevelLowerLimit)
	{
		m_iLevelLowerBound = roomInfo.iLevelLowerLimit;
		bChanged = true;
	}
	if(m_dwWuXunLimit != roomInfo.dwWuXunLimit)
	{
		m_dwWuXunLimit = roomInfo.dwWuXunLimit;
		bChanged = true;
	}
	if(m_dwManagerID != roomInfo.dwManagerID)
	{
		m_dwManagerID = roomInfo.dwManagerID;
		bChanged = true;
	}
	if(m_dwCount != roomInfo.ucCurrentPersonNum)
	{
		m_dwCount = roomInfo.ucCurrentPersonNum;
		bChanged = true;
	}

	if( wcscmp(m_szManagerName, roomInfo.szManagerName) !=0)
	{
		memcpy(m_szManagerName, roomInfo.szManagerName, sizeof(m_szManagerName));
		bChanged = true;
	}
	std::map<BYTE, INT>	roleMap;
	for(INT i=0; i<(INT)m_dwCount; i++)
	{
		roleMap[roomInfo.pData[i].ucPos] = i;
	}

	for(int i=1; i<=(int)m_dwNumLimit; i++) // 座位号从1开始
	{
		PRoleInfo pRolInfo = m_RoleMap.Peek(i);
		bool bExist = roleMap.find(i) != roleMap.end();
		if(!bExist) 
		{
			if(!P_VALID(pRolInfo)) continue;
			else
			{
				m_RoleMap.Erase(i);
				delete pRolInfo;
				bChanged = true;
			}	
		}
		else
		{
			ASSERT(bExist);
			const tagPvPRoleInfo& roleInfo = roomInfo.pData[roleMap[i]];
			if(!P_VALID(pRolInfo))
			{
				pRolInfo = new RoleInfo;
				pRolInfo->dwRoleID = roleInfo.dwRoleID;
				pRolInfo->eState = roleInfo.eState;
				pRolInfo->ucPos = roleInfo.ucPos;
				pRolInfo->dwWuXun = roleInfo.dwWuXun;
				memcpy(pRolInfo->szName, roleInfo.szName, sizeof(pRolInfo->szName));
				pRolInfo->dwLevel = roleInfo.dwRoleLevel;
				m_RoleMap.Add(i, pRolInfo);
				bChanged = true;
			}
			else
			{
				if(pRolInfo->dwRoleID != roleInfo.dwRoleID)
				{
					pRolInfo->dwRoleID = roleInfo.dwRoleID;
					bChanged = true;
				}
				if(pRolInfo->eState != roleInfo.eState)
				{
					pRolInfo->eState = roleInfo.eState;
					bChanged = true;
				}
				if(pRolInfo->ucPos != roleInfo.ucPos)
				{
					pRolInfo->ucPos = roleInfo.ucPos;
					bChanged = true;
				}
				if(pRolInfo->dwLevel != roleInfo.dwRoleLevel)
				{
					pRolInfo->dwLevel = roleInfo.dwRoleLevel;
					bChanged = true;
				}
				if(pRolInfo->dwWuXun != roleInfo.dwWuXun)
				{
					pRolInfo->dwWuXun = roleInfo.dwWuXun;
					bChanged = true;
				}
				if(wcscmp(pRolInfo->szName, roleInfo.szName) != 0)
				{
					memcpy(pRolInfo->szName, roleInfo.szName, sizeof(pRolInfo->szName));
					bChanged = true;
				}
			}
		}
	}
	m_lastUpdate = GetCurrentDWORDTime();
	m_bInitialized = TRUE;

	if(bChanged)
	{
		tagRoomChange evt;
		evt.eHallType = m_eHallType;
		evt.eLevel = m_eLevel;
		evt.nRoomID = m_nRoomID;
		m_pFrameMgr->SendEvent(&evt);
	}	
}