#pragma once
#include "..\WorldDefine\clergy_define.h"

struct tagNS_GetGodRelation;

const DWORD dwNameColor = 0xfffffea9;		//名字颜色
const DWORD dwNameShade = 0xff321500;		//名字描边
const DWORD dwDefaultColor = 0xff8ea28e;	//空缺时的颜色
const DWORD dwDefaultShade = 0xff112a11;	//空缺时描边

class GodRelationFrame : GameFrame
{
public:
	GodRelationFrame();
	~GodRelationFrame();
	virtual BOOL Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID Hide();
	VOID Show();

	VOID ClearName();		//所有控件设置成空缺中
	VOID ShowClergyTips(GUIWnd* pWnd, int nClergyID);	//显示神职tips

private:
	TSFPTrunk<GodRelationFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pNetSession;
	TObjRef<NetCmdMgr>				m_pNetCmdMgr;

	GUIWnd							*m_pWnd;
	GUIWnd							*m_pWndCaption;
	GUIStaticEx						*m_pStcMap[ECGT_Num][MAX_GOD_NUM];
	GUIButton						*m_pBtnClose;
	GUIStatic						*m_pStcGodTips[MAX_GOD_Relation_NUM];	//用于显示tips的那26个控件..

	DWORD							m_escapeTime;


private:
	BOOL EventHandler(tagGUIEvent* pEvent);

	DWORD OnNS_GetGodRelation(tagNS_GetGodRelation* pCmd, DWORD);

	DWORD OnOpenGodRelation(tagGameEvent* pEvent);
};