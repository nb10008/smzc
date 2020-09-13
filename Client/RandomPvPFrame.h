#pragma once
#include "gameframe.h"

#define MAX_RANPVP_NUM 6
class IconStatic;
enum EClassType;
struct tagNS_PvPRanRoomInfo;
struct tagNS_PvPRanRoomInfoSingleChange;
struct tagNS_PvPJoinRoom;
struct tagNS_PvPLeaveRoom;

struct tagPvPRanSaveInfo
{
	INT			nPlayerLevel;			//远程玩家等级
	EClassType	ePlayerProfession;		//远程玩家职业
	DWORD		dwPlayerID;				//玩家ID
	tstring		strName;				//玩家姓名
};

struct tagPvPPlayerInfo
{
	GUIStatic* pStcBack;	//背景

	GUIStatic* pStcName;		//名字
	GUIStatic* pStcLevel;		//等级
	GUIStatic* pStcProfession;	//职业
	GUIStatic* pStcJi;			//"级"字
	IconStatic* pStcProfessionPic;		//职业图片

};

class RandomPvPFrame :
	public GameFrame
{
public:
	RandomPvPFrame(void);
	~RandomPvPFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

	VOID ClearInfo(tagPvPPlayerInfo &strPvPinfo);
	VOID ClearSaveInfo(tagPvPRanSaveInfo &strPvPinfo);
	VOID RefreshPlayer(INT pos,INT level,EClassType eType);
	VOID ClearAll();

	tstring& GetPvPmemberName(DWORD dwPlayerID);
	EClassType GetPvPmemberClass(DWORD dwPlayerID);

	VOID HideWnd(){m_pWnd->SetInvisible(true); m_pWndMin->SetInvisible(true);}

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	DWORD OnNS_PvPRanRoomInfo(tagNS_PvPRanRoomInfo* pNetCmd,DWORD dwParam);
	DWORD OnNS_PvPRanRoomInfoSingleChange(tagNS_PvPRanRoomInfoSingleChange* pNetCmd, DWORD dwParam);
	DWORD OnNS_PvPJoinRoom(tagNS_PvPJoinRoom* pNetCmd,DWORD dwParam);
	DWORD OnNS_PvPLeaveRoom(tagNS_PvPLeaveRoom* pNetCmd,DWORD dwParam);
	

private:
	// tools
	TSFPTrunk<RandomPvPFrame>		m_Trunk;
	TObjRef<GUISystem>          m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	GUIWnd*                     m_pWnd;

	GUIWnd*                     m_pWndMin;
	GUIButton*					m_pBnMin;			// 最小化

	GUIButton*					m_pBnHide;			// 隐藏
	GUIButton*					m_pBnExit;			// 退出
	GUIButton*					m_pBnChangeRoom;			// 更换房间

	GUIStatic*					m_pStcName;			// 姓名
	GUIStatic*					m_pStcLevel;		// 等级
	GUIStatic*					m_pStcProfession;	// 职业
	GUIStatic*					m_pStcGlory;		// 荣誉
	GUIStatic*					m_pStcRankLevel;	// 排名
	GUIStatic*					m_pStcSucessRate;	// 胜率
	GUIStatic*					m_pStcPvPPoint;		// 斗技点数
	tagPvPPlayerInfo			m_strPvPPlayerInfo[MAX_RANPVP_NUM];
	tagPvPRanSaveInfo			m_strPvPSaveInfo[MAX_RANPVP_NUM];
	tstring						m_strEmpty;
};
