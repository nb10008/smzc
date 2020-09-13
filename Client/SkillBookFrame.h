#pragma once
#include "SkillItem.h"
struct tagUpdateLPSkillEvent;

const INT MAX_SKILL_BOOK_NUM = 8;


class SkillBookFrame : public GameFrame
{
	enum ECurDisplaySkillType
	{
		ECDST_Active,			//主动技能
		ECDST_Passive,			//被动技能
		ECDST_Produce,			//生产技能
		ECDST_Guild,			//公会技能
		ECDST_GodArea,			//领域技能
		ECDST_OtherPassive		//其他生产技能
	};

public:
	SkillBookFrame();
	~SkillBookFrame();

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	
	// 处理技能界面打开
	VOID OnSkillBookOpen();

private:
	// 处理GUI消息
	DWORD EventHandler(tagGUIEvent* pEvent);

	// 处理游戏事件
	DWORD OnUpdateLPSkillEvent(tagUpdateLPSkillEvent* pGameEvent);
	DWORD OnWeaponSwap(tagGameEvent* pGameEvent);
	

	// 是否是当前页
	BOOL IsCurPage(INT nIndex){return nIndex >= m_nCurPage*MAX_SKILL_BOOK_NUM 
		&& nIndex < (m_nCurPage+1)*MAX_SKILL_BOOK_NUM;}
	
	// 设置当前页码
	VOID SetCurPage(INT nPage);
	
	// 处理翻页事件
	VOID OnPageUp(BOOL bUpPage);

	// 刷新当前页
	VOID ReflashCurPage();

	/** 当前显示的技能类型，最多多少页
	*/
	INT GetMaxPages();

	/** 校正当前页
	*/
	void CheckCurPage();

private:

	TSFPTrunk<SkillBookFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIWnd*						m_pWnd;
	GUIStatic*					m_pStcPage;
	GUIButton*					m_pBtnPageUp;
	GUIButton*					m_pBtnPageDown;

	GUIPushButton*              m_activesSkill;			//主动技能按钮
	GUIPushButton*              m_passiveSkill;			//被动技能按钮
	GUIPushButton*				m_produceSkill;			//生产技能
	GUIPushButton*				m_GuildSkill;			//生产技能
	GUIPushButton*				m_GuildGodArea;			//领域技能
	GUIStaticEx*                m_pProduceNotice;       //如果生产技能没学的话用于显示提示信息的

	SkillItem					m_SkillItem[MAX_SKILL_BOOK_NUM];

	TList<DWORD>				m_skillList;			//主动技能列表缓存
	//TList<DWORD>::TListIterator m_skillIter;			//主动技能列表迭代器

	TList<DWORD>                m_PassiveSkillList;		//被动技能列表缓存
	//TList<DWORD>::TListIterator m_PassiveSkillIter;		//被动技能列表迭代器

	TList<DWORD>				m_lstProduceSkill;		//生产技能列表
	//TList<DWORD>::TListIterator m_iterProduceSkill;		//生产技能列表迭代器

	TList<DWORD>				m_lstGuildSkill;			//公会技能

	TList<DWORD>				m_lstGodSkill;			//领域技能

	TList<DWORD>				m_lstOtherPassiveSkill;			//其它被动技能列表
	//TList<DWORD>::TListIterator m_iterlstOtherPassiveSkill;		//其它被动技能列表迭代器


	INT							m_nCurPage;
	//BOOL                        m_isActiveSkill;    //--值为TRUE表示显示主动技能，为FALSE表示显示被动技能，初始为TRUE显示主动技能
	
	ECurDisplaySkillType		m_eCurSkillType;
};
