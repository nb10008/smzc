#pragma once

class IconStatic;
enum E_PvP_RoomType;
enum E_PvP_RoomLevel;
enum E_PvP_RoleState;
struct tagRoomChange;

class PvPRoomPosCtrl
{
public:
	PvPRoomPosCtrl(IconStatic* pPos, IconStatic* pState, GUIStatic* pLevel, GUIStatic* pName, BYTE pos);
	void SetRoom(E_PvP_RoomType	eHallType, E_PvP_RoomLevel eLevel, INT16 nRoomID);
	void ResetPos();
	void OnUpdate(tagRoomChange* evt);
	E_PvP_RoomType GetHallType()const { return m_eHallType; }
	E_PvP_RoomLevel GetLevel()const { return m_eLevel; }
	INT16	GetRoomID()const { return m_nRoomID;}
	BYTE	GetPos()const { return m_ucPos;}
	DWORD	GetRoleID()const { return m_dwRoleID;}
protected:
	void UpdateUI();
	void SetPos(BYTE pos);
	void SetWuXun(DWORD wuXun);
	void SetState(E_PvP_RoleState state);
	void SetName(const TCHAR* name);
	void SetLevel(DWORD level);
private:
	E_PvP_RoomType				m_eHallType;
	E_PvP_RoomLevel				m_eLevel;
	INT16	                    m_nRoomID ;	//·¿¼äºÅ
	BYTE						m_ucPos;
	BOOL						m_bNeedUpdate;
	DWORD						m_dwRoleID;
	IconStatic *m_pPos, *m_pState;
	GUIStatic *m_pLevel, *m_pName;
	DWORD						m_dwLastUpdate;
};