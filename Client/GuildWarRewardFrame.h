#pragma once
#include "gameframe.h"

class IconStatic;
struct tagNS_GuildOccupyedNum;
struct tagNS_TakeGuildWarReword;

class GuildWarRewardFrame :
	public GameFrame
{
public:
	GuildWarRewardFrame(void);
	~GuildWarRewardFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();
	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }
private:
	BOOL EventHandler(tagGUIEvent* pEvent);
	DWORD OnNS_GuildOccupyedNum(tagNS_GuildOccupyedNum *pNetCmd, DWORD);
	DWORD OnNS_TakeGuildWarReword(tagNS_TakeGuildWarReword *pNetCmd, DWORD);

private:
	TSFPTrunk<GuildWarRewardFrame>			m_Trunk;
	TObjRef<GUISystem>								m_pGUI;
	TObjRef<Util>											m_pUtil;
	TObjRef<GameFrameMgr>						m_pFrameMgr;
	TObjRef<NetSession>								m_pSession;
	TObjRef<NetCmdMgr>								m_pCmdMgr;

	GUIWnd*                     m_pWnd;

	IconStatic*					m_pIsItemReword;
	IconStatic*					m_pIsBuffReword;
	IconStatic*					m_pIsAlreadyTake;

	GUIButton*					m_pBnGetReword;
	GUIButton*					m_pBnClose;

	GUIStaticEx*				m_pSeItemName;
	GUIStaticEx*				m_pSeBuffName;

	GUIStatic*					m_pStGuildName;
	GUIStatic*					m_pStGuildPos;
	GUIStatic*					m_pStOccupyNum;

	DWORD						m_dwNPCID;				// NPCµÄID
	DWORD						m_escapeTime;

	DWORD						m_dwItemID;
	BYTE							m_byItemQuity;	// Æ·¼¶
	DWORD						m_dwBuffID;
};
