#pragma once
#include "GameFrameMgr.h"
#include "GameFrame.h"
#include "ItemEvent.h"
#include "ItemFrame.h"
#include "..\WorldDefine\msg_activity.h"
#include "ActiveProgress.h"
#include "..\WorldDefine\msg_map.h"

class ActiveTianXuanFrame : public GameFrame
{
public:
	ActiveTianXuanFrame(void);
	~ActiveTianXuanFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual VOID OnEvent(tagGameEvent* pEvent);

	VOID RefreshUI();	//刷新分数人数
	tstring GetNextValue(LPCTSTR &pMsg, TCHAR token);

private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnNS_DotaMsg(tagNS_DotaMsg* pCmd, DWORD);
	DWORD OnNS_GotoNewMap(tagNS_GotoNewMap* pCmd, DWORD);

private:
	TSFPTrunk<ActiveTianXuanFrame>			m_Trunk;
	TObjRef<GUISystem>						m_pGUI;
	TObjRef<Util>							m_pUtil;
	TObjRef<GameFrameMgr>					m_pFrameMgr;
	TObjRef<NetSession>						m_pSession;
	TObjRef<NetCmdMgr>						m_pCmdMgr;

	GUIWnd*									m_pWndMain;
	GUIStatic*								m_pStcBluePoint;	//蓝方分数
	GUIStatic*								m_pStcRedPoint;		//红方分数
	GUIStatic*								m_pStcBlueNum;		//蓝方人数
	GUIStatic*								m_pStcRedNum;		//红方人数
	GUIStatic*								m_pStcIcon;			//中间那个东西
	ActiveProgress*							m_pProPersent;		//进度条

	DWORD									m_dwRedPoint;		//红分数
	DWORD									m_dwBluePoint;		//蓝分数
	INT										m_nRedNum;			//红人数
	INT										m_nBlueNum;			//蓝人数

};