#pragma once
#include "..\WorldDefine\RoleDefine.h"
#include "..\WorldDefine\clergy_define.h"
#include "ActiveProgress.h"

/** \brief 凝聚Frame
*/

struct tagNS_CondensedChange;
struct tagNS_StopCondensed;
struct tagNS_BeginCondensed;
struct tagNS_Skill;

class CondensedFrame : GameFrame
{
public:
	CondensedFrame();
	~CondensedFrame();
	virtual BOOL Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID Show(bool bShow);
	VOID Clear();

private:
	DWORD OnNS_CondensedChange(tagNS_CondensedChange* pCmd, DWORD);
	DWORD OnNS_StopCondensed(tagNS_StopCondensed* pCmd, DWORD);
	DWORD OnNS_BeginCondensed(tagNS_BeginCondensed* pCmd, DWORD);

	DWORD OnOpenCondensedFrame(tagGameEvent* pEvent);
	DWORD OnNS_Skill(tagNS_Skill* pMsg, DWORD dwParam);

private:
	TSFPTrunk<CondensedFrame> m_Trunk;
	TObjRef<GUISystem>		   m_pGUI;
	TObjRef<GameFrameMgr>	   m_pFrameMgr;
	TObjRef<NetSession>        m_pNetSession;
	TObjRef<NetCmdMgr>         m_pNetCmdMgr;

	GUIWnd                  *m_pWnd;
	GUIWnd                  *m_pWndCaption;
	GUIStatic				*m_pStcGodHead;					//神格
	GUIStatic               *m_pStcBelief;					//信仰
	ActiveProgress          *m_pProCondensed;				//凝聚度
	GUIStatic				*m_pStcCondensed;				//凝聚度数值
	ActiveProgress          *m_pProBar;						//进度条
	GUIButton               *m_pBtnBegin;					//开始凝聚
	GUIButton               *m_pBtnClose;					//关闭
	GUIButton               *m_pBtnCloseWnd;				//关闭*
	GUIStatic				*m_pStcStaricon[MAX_GodHead];	//小星星图片

	BOOL					m_bIsConsdensed;				//是否正在凝聚

private:
	BOOL EventHandler(tagGUIEvent* pEvent);
};