#pragma once

enum E_PvP_RoomType;
enum E_PvP_RoomLevel;
struct tagRoomChange;

class PvPRoomCtrl
{
public:
	enum EPrepareState
	{
		E_Unprepared,
		E_Preparing,
		E_Prepared,
		E_Unprepring,
	};

	PvPRoomCtrl(GUIPatch* pTitle, GUIStatic* pName, GUIStatic* pLevel, GUIStatic* pWuxun, GUIButton* pBtnKick, GUIButton* pBtnPrepare, GUIButton* pBtnStart, GUIButton* pBtnLeave, GUIStatic* pFolderName, GUIComboBox* pFriendList);
	void SetRoom(E_PvP_RoomType	eHallType, E_PvP_RoomLevel eLevel, INT16 nRoomID);
	void ResetRoom();
	void OnUpdate(tagRoomChange* evt);
	E_PvP_RoomType GetHallType()const { return m_eHallType; }
	E_PvP_RoomLevel GetLevel()const { return m_eLevel; }
	INT16	GetRoomID()const { return m_nRoomID;}
	BOOL	IsMyRoom()const;
	void SetPrepare(EPrepareState prepareState);
	EPrepareState GetPrepareState()const { return m_ePreapareState;}
	void	OnFriendChange();
protected:
	void SetTitle(E_PvP_RoomType hallType, E_PvP_RoomLevel eLevel, INT16 roomID);
	void SetFolderName(E_PvP_RoomType hallType, E_PvP_RoomLevel eLevel, INT16 roomID, INT nCurrent);
	void SetName(const TCHAR* pName);
	void SetLimit(INT lowerLevel, INT upperLevel);
	void SetWuxun(INT wuXin);
	void UpdateUI();
	void RefreshInvitableFriends(PvPRoom* pRoom); // 刷新房间界面上可邀请的好友列表
private:
	E_PvP_RoomType				m_eHallType;
	E_PvP_RoomLevel				m_eLevel;
	INT16	                    m_nRoomID ;	//房间号
	EPrepareState				m_ePreapareState;
	GUIPatch* m_pTitle;
	GUIStatic* m_pName, *m_pLevel, *m_pWuxun, *m_pFolderName;
	GUIButton* m_pBtnKick, *m_pBtnPrepare, *m_pBtnStart, *m_pBtnLeave;
	GUIComboBox*							m_pCmb3v3FriendList; // 好友列表
};