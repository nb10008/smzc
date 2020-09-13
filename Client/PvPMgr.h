#pragma once

class PvPHall;

enum E_PvP_RoomType;
enum E_PvP_RoomLevel;
class PvPRoom;
class PvPRoomList;

struct tagWuXunLevel
{
	DWORD dwLevel;
	DWORD dwWuXunLowerBound;
	DWORD dwWuJiMax;
};

class PvPMgr
{
	PvPMgr();
	~PvPMgr();
	PvPMgr(const PvPMgr&);
	const PvPMgr& operator = (const PvPMgr&);
public:
	static PvPMgr* Inst();
	PvPRoomList* GetRoomList(E_PvP_RoomType hallType, E_PvP_RoomLevel level);
	PvPRoom* GetRoom(E_PvP_RoomType hallType, E_PvP_RoomLevel level, INT16 roomID);
	DWORD	MinRefreshInterval()const { return 1;} // in seconds
	BOOL	CanEnterHall(E_PvP_RoomLevel eType)const;
	INT		Wuxun2Level(DWORD wuXun) const ;
	tagWuXunLevel GetWuXunLevelPro(DWORD dwLevel) { if(m_wuXunMap.find(dwLevel)!= m_wuXunMap.end()) return m_wuXunMap[dwLevel]; return tagWuXunLevel();}
	void	SetRoomToRoleMap(E_PvP_RoomType hallType, E_PvP_RoomLevel level, INT16 roomID);
	tstring GetRoleName(DWORD roleID)const;
	DWORD	Init();
private:
	std::map<E_PvP_RoomType, PvPHall*> m_hallMap;	// not changed after init
	std::map<DWORD, tstring> m_roleNameMap;	// for display result;
	std::map<DWORD, tagWuXunLevel> m_wuXunMap; // level to properties;
};

inline tstring PvPMgr::GetRoleName(DWORD roleID)const
{
	std::map<DWORD, tstring>::const_iterator itor = m_roleNameMap.find(roleID);
	if(itor != m_roleNameMap.end())
		return itor->second;
	return _T("");
}