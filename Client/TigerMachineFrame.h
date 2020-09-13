#pragma once
#include "gameframe.h"

#define TAGER_NUM 3
class RollStatic;
class ChangeStatic;
struct tagNS_GetTigerResult;
struct tagNS_ItemAdd;
struct tagNS_NewItemAdd;

class TigerMachineFrame :
	public GameFrame
{
public:
	TigerMachineFrame(void);
	~TigerMachineFrame(void);

	BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	BOOL Destroy();
	BOOL ReloadUI();
	BOOL EscCancel();
	VOID Update();

	BOOL EventHandler(tagGUIEvent* pEvent);
	void GenerateRandomResult(int nResult);
	bool CheckItem(int nType);

	void RefreshProfitUI();
	void ShowFrame(BOOL b);
	VOID SetNpcID( DWORD dwID ) { m_dwNPCID = dwID; }
private:
	DWORD OnNS_GetTigerResult(tagNS_GetTigerResult* pNetCmd,DWORD);

	// 增加一个已知物品
	DWORD NetItemAdd(tagNS_ItemAdd* pNetCmd, DWORD);
	// 新增一个物品
	DWORD NetNewItemAdd(tagNS_NewItemAdd* pNetCmd, DWORD);
private:
	TSFPTrunk<TigerMachineFrame>			m_trunk;
	TObjRef<Util>							m_pUtil;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<GameFrameMgr>       m_pFrameMgr;
	TObjRef<GUISystem>				m_pGUI;

	GUIWnd*		m_pWnd;
	RollStatic*		m_pStTiger[TAGER_NUM];

	GUIPushButton*	m_pBnStart;

	GUIPushButton*	m_pBnX1;
	GUIPushButton*	m_pBnX5;
	GUIPushButton*	m_pBnX10;
	GUIPushButton*	m_pBnX50;

	GUIStatic*		m_pStCurrWin;
	GUIStatic*		m_pStTotal;
	ChangeStatic*	m_pIsLights;

	int	m_nType;
	bool						m_bStarted;

	GUIButton*	m_pBnClose;
	int				m_iStopNum[TAGER_NUM];

	bool m_bStopMark;
	DWORD	m_dwStopTime;
	int m_nCurNum;	// 显示计数用

	int	m_nCurWin;	// 当前赢得的游戏币

	DWORD										m_dwNPCID;				// NPC的ID
	DWORD										m_escapeTime;
	bool	m_bRolling;

	DWORD										m_dwLightCount;
};
