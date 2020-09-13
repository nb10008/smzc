#pragma once

enum E_PvP_RoomType;
enum E_PvP_RoomLevel;
class PvPRoom;
#include <list>
class PvPRoomList
{
public:
	PvPRoomList(E_PvP_RoomType hallType, E_PvP_RoomLevel pvpLevel);
	~PvPRoomList();
	void Update();
	void ForceUpdate();
	void UpdateRoomList(DWORD nCount, INT16* pRoomPoss);
	void RemoveInvalid(const std::list<INT16>& roomID);
	std::list<INT16>  GetOrderedRoomList()const;
	PvPRoom* GetRoom(INT16 pos)const;
	E_PvP_RoomLevel GetPvPLevel()const {return m_eLevel;}
	BOOL IsInitialized()const { return m_bInitialized; }
	PvPRoom* AddRoom(INT16 roomID);
private:
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;
	tagDWORDTime				m_lastUpdate;
	BOOL						m_bInitialized;

	E_PvP_RoomLevel m_eLevel;
	E_PvP_RoomType m_eHallType;
	TSafeMap<INT16, PvPRoom*> m_RoomMap;
};