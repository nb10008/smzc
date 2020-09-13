#pragma once
class PvPRoom;
enum E_PvP_RoomType;
enum E_PvP_RoomLevel;
struct tagRoomChange;
class IconStatic;
class StaticEx;

class RoomListItemCtrl
{
public:
	RoomListItemCtrl(GUIPatch* pButton, GUIStatic* pPos, GUIStatic* pLevel, GUIStatic* pName, GUIStatic* pNum, IconStatic* pState, IconStatic* pImg, GUIStatic* pLimit, StaticEx* pBackImg);
	void SetRoom(E_PvP_RoomType	eHallType, E_PvP_RoomLevel eLevel, INT16 nRoomID);
	void ResetRoom();
	void OnUpdate(tagRoomChange* evt);
	E_PvP_RoomType GetHallType()const { return m_eHallType; }
	E_PvP_RoomLevel GetLevel()const { return m_eLevel; }
	INT16	GetRoomID()const { return m_nRoomID;}
protected:
	void SetPos(INT16 pos);
	void SetLevel(E_PvP_RoomLevel level);
	void SetName(TCHAR* name);
	void SetNum(DWORD count, DWORD limit);
	void SetLimit(INT lowerLevel, INT upperLevel);
	void SetState(E_PvP_RoomState state);
	void SetImg(DWORD wuXun);
private:
	E_PvP_RoomType				m_eHallType;
	E_PvP_RoomLevel				m_eLevel;
	INT16	                    m_nRoomID ;	//·¿¼äºÅ
	GUIPatch* m_pButton;
	GUIStatic *m_pPos, *m_pLevel, *m_pName, *m_pNum, *m_pLimit;
	IconStatic *m_pState, *m_pImg;
	StaticEx* m_pBackImg;
};