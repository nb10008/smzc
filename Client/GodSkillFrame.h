#pragma once
#include "GodSkillDataMgr.h"


/*********************
//!brief:仙界修行技能界面
*********************/



class IconStatic;

struct tagNS_GetConsumptiveStrength;

const int SKILL_NUM_PER_PAGE = 12;

class GodSkillFrame : public GameFrame
{
public:
	GodSkillFrame();
	~GodSkillFrame();
	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

private:
	DWORD EventHandler(tagGUIEvent *pEvent);
	void OnSkillButtonClick(int nPos);
	void OnSkillLearnClick();
	void SetDefaultUI();
	void UpdateUI();
	DWORD OnEventRefresh(tagGameEvent *pEvent);
	DWORD OnNetGetConsumptiveStrength(tagNS_GetConsumptiveStrength *pNetCmd, DWORD);

private:
	TObjRef<GUISystem> m_pGUI;
	TSFPTrunk<GodSkillFrame> m_trunk;
	GUIWnd *m_pWnd;
	IconStatic *m_pIstSkillImg;		// 技能图片

	GUIStatic  *m_pStcSkillName;	// 技能名称
	GUIStatic  *m_pStcSkillLv;		// 技能等级
	GUIStatic  *m_pStcOwnGold;		// 当前拥有金
	GUIStatic  *m_pStcOwnSilver;	// 当前拥有银
	GUIStatic  *m_pStcCurStrength;	// 当前实力值
	GUIStatic  *m_pStcCurCount;		// 当前可修炼次数
	GUIProgress *m_pProgress;		// 修炼进度
	
	GUIButton  *m_pBtnPre;			// 向前翻页
	GUIButton  *m_pBtnNext;			// 向后翻页
	GUIStatic  *m_pStcCurPageNum;	// 当前页数
	
	GUIStatic  *m_pStcGold;			// 单次修炼所需金
	GUIStatic  *m_pStcSilver;		// 单次修炼所需银
	GUIStatic  *m_pStcStrength;		// 单次修炼所需实力
	GUIStatic  *m_pStcItem;			// 修炼所需道具

	GUIButton  *m_pBtnLearn;		// 修炼技能
	GUIButton  *m_pBtnClose;		// 关闭
	GUIButton  *m_pBtnX;			// X
	
	IconStatic *m_pIstSkillBacks[SKILL_NUM_PER_PAGE];
	IconStatic *m_pIstSkills[SKILL_NUM_PER_PAGE];	
	GUIStatic  *m_pStcSkillNames[SKILL_NUM_PER_PAGE];
	GUIStatic  *m_pStcSkillGrades[SKILL_NUM_PER_PAGE];

	int m_nCurItem;				// 当前选中的技能
	GodSkillDataMgr::EGodSkillLevel  m_eCurGodSkillLv;		// 仙界技能等级
	int m_nCurPageNum;				// 当前页数
	int m_nTotalPageNum;			// 总页数
	DWORD m_dwCurTime;
};
