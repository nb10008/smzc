#pragma once
#include "gameframe.h"


/*********************
//!brief:飞升技能修炼界面
*********************/

class IconStatic;
struct tagNS_RoleExpChange;
const int SOAR_SKILL_NUM_PER_PAGE = 12;

class SoarSkillFrame :
	public GameFrame
{
public:
	SoarSkillFrame(void);
	~SoarSkillFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	void OnSkillButtonClick(int nPos);
	void OnSkillLearnClick();
	void SetDefaultUI();
	void UpdateUI();
	DWORD OnEventRefresh(tagGameEvent *pEvent);
	DWORD OnNS_RoleExpChange(tagNS_RoleExpChange* pNetCmd, DWORD);
public:
	VOID SetInvisable();

private:
	TObjRef<GUISystem> m_pGUI;
	TSFPTrunk<SoarSkillFrame> m_trunk;
	TObjRef<NetCmdMgr> m_pNetCmdMgr;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	GUIWnd *m_pWnd;
	IconStatic *m_pIstSkillImg;		// 技能图片

	GUIStatic  *m_pStcSkillName;	// 技能名称
	GUIStatic  *m_pStcSkillLv;		// 技能等级
	GUIStatic  *m_pStcOwnGold;		// 当前拥有金
	GUIStatic  *m_pStcOwnSilver;	// 当前拥有银
	GUIStatic  *m_pStcCurStrength;	// 当前经验值
	GUIStatic  *m_pStcCurCount;		// 当前可修炼次数
	GUIProgress *m_pProgress;		// 修炼进度

	GUIButton  *m_pBtnPre;			// 向前翻页
	GUIButton  *m_pBtnNext;			// 向后翻页
	GUIStatic  *m_pStcCurPageNum;	// 当前页数

	GUIStatic  *m_pStcGold;			// 单次修炼所需金
	GUIStatic  *m_pStcSilver;		// 单次修炼所需银
	GUIStatic  *m_pStcStrength;		// 单次修炼所需实力
	GUIStatic  *m_pStcItem;			// 修炼所需道具
	GUIStatic  *m_pStcTips;			// 小提示

	GUIButton  *m_pBtnLearn;		// 修炼技能
	GUIButton  *m_pBtnClose;		// 关闭
	GUIButton  *m_pBtnX;			// X

	IconStatic *m_pIstSkillBacks[SOAR_SKILL_NUM_PER_PAGE];
	IconStatic *m_pIstSkills[SOAR_SKILL_NUM_PER_PAGE];	
	GUIStatic  *m_pStcSkillNames[SOAR_SKILL_NUM_PER_PAGE];
	GUIStatic  *m_pStcSkillGrades[SOAR_SKILL_NUM_PER_PAGE];

	GUIStatic  *m_pStcNowExpText;		
	GUIStatic  *m_pStcNeedExpText;			
	int m_nCurItem;				// 当前选中的技能
	int m_nCurPageNum;				// 当前页数
	int m_nTotalPageNum;			// 总页数
	DWORD m_dwCurTime;

};
