#pragma once


class PvPRoomList;
enum E_PvP_RoomType;
class PvPHall
{
public:
	PvPHall(E_PvP_RoomType eType);
	~PvPHall();
	void Update();
	void UpdateHallInfo(DWORD nCount, DWORD* pdwPlayerIDs);
	const PvPRoomList* GetDefault()const { return m_pJuniorRoomList;}
	PvPRoomList* GetRoomList(E_PvP_RoomLevel level);
private:
	E_PvP_RoomType m_eType;
	PvPRoomList* m_pSeniorRoomList;
	PvPRoomList* m_pMidRoomList;
	PvPRoomList* m_pJuniorRoomList;
	TSafeList<DWORD> m_Players;
};

inline PvPRoomList* PvPHall::GetRoomList(E_PvP_RoomLevel level) 
{
	switch(level)
	{
	case E_PvP_Junior:
		return m_pJuniorRoomList;
		break;
	case E_PvP_Middle:
		return m_pMidRoomList;
		break;
	case E_PvP_Senior:
		return m_pSeniorRoomList;
		break;
	}
	ASSERT(0);
	return NULL;
}