#pragma once
#include "TalentTree.h"
#include "..\WorldDefine\RoleDefine.h"
#include "..\WorldDefine\talent_define.h"

struct tagUpdateLPSkillEvent;
struct tagUpdateLPLevel;
struct tagUpdateLPAttEvent;
struct tagUpdateLPTalentPoint;
struct tagRoleClassChange;
class GlintButton;

/** class	TalentTreeFrame
	brief	天赋树界面
*/
class TalentTreeFrame :
	public GameFrame
{
public:
	TalentTreeFrame();
	~TalentTreeFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();

	// 处理天资打开
	VOID OnTalentTreeOpen();

private:
	// 处理GUI消息
	DWORD EventHandler(tagGUIEvent* pEvent);
	
	// 处理游戏事件
	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pGameEvent);
	DWORD OnUpdateLPLevel(tagUpdateLPLevel* pGameEvent);
	DWORD OnUpdateLPAttEvent(tagUpdateLPAttEvent* pGameEvent);
	DWORD OnUpdateLPTalentPoint(tagUpdateLPTalentPoint* pGameEvent);
	DWORD OnEventClearTalent(tagGameEvent* pGameEvent);
	DWORD OnMsgClearTalent(tagMsgBoxEvent* pGameEvent);
	DWORD OnUpdateLPClassEvent(tagRoleClassChange* pGameEvent);

	// 设置当前可投点
	VOID SetBidden(INT nValue);
	
	// 设置当前已投点
	VOID SetAvail(INT nValue);

	/** 投过点的天资系，按钮的文本的颜色是绿色的
	*/
	void RefreshTextColor();

private:

	TSFPTrunk<TalentTreeFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>		m_pSession;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndLearnBack;
	GUIPushButton*				m_pPbnTalent[X_MAX_TALENT_PER_ROLE];
	GUIStatic*					m_pStcAvail;
	GUIStatic*					m_pStcBidden;
	GUIButton*					m_pBtnCleanTalent;
	
	//天资描述界面，没对任何天资投过点的时候显示整体描述，否则某系天资没投过点的时候具体描述此系天资
	//GUIPushButton*				m_pBtnSimpleMode;		//精简的显示模式

	TalentTree					m_TalentTree;

	INT							m_nCurBidden;
	ETalentType					m_eCurTalent;
	INT							m_nCurAvail;
	INT							m_nCurTalentNum;

};
