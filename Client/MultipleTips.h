#pragma once

/*************
\物品掉率、经验等等多倍的提示界面
*************/
#include "..\WorldDefine\msg_gm_cmd.h"
class MultipleTips:
	public GameFrame
{
public:
	MultipleTips(void);
	~MultipleTips(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();

private:
	DWORD  NetRecvServerDoubleInfo(tagNS_DoubleDropProbabilityChange* pNetCmd, DWORD dwParam);

private:

	TSFPTrunk<MultipleTips>		m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;
	TObjRef<Util>				m_pUtil;

	GUIWnd						*m_pTurnWnd;
	GUIWnd						*m_pExpWnd;

	GUIStatic					*m_pStcServerTurn;
	GUIStatic					*m_pStcRestTurnTime;
	GUIStatic					*m_pStcServerExp;
	GUIStatic					*m_pStcRestExpTime;

	DWORD                     m_dwRestTime_Trun;
	DWORD						m_dwRestTime_Exp;
	DWORD						m_dwEscapeTime;			//毫秒
	//BOOL						m_bShow;
};
