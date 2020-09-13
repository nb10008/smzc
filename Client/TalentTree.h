#pragma once

#include "..\WorldDefine\skill_define.h"

class TalentButton;
struct tagSkillData;
class FlashStatic;
/** \struct	tagTalentLeaf
	\brief	记录天资树叶节点相关内容
*/
struct tagTalentLeaf
{
	DWORD				dwTalentLeaf;				// 天资ID
	ESkillUseType		eUseType;					// 技能使用方式
	ETalentType			eTalentType;				// 技能天资类型
	ESkillLevelUpType	eLevelUpType;				// 技能升级类型
	BOOL				bActive;					// 是否可投点
	INT					nCurLevel;					// 当前等级
	INT					nLearnLevel;				// 习得的等级
	INT					nMaxLevel;					// 最大可投等级
	tagPoint			Pos;						// 所在位置
	DWORD				dwChildLeaf;				// 附属技能ID
	DWORD				dwParentLeaf;				// 连线的先驱ID
	TalentButton*		pBtnTalentLeaf;				// 技能按钮
	FlashStatic*		pStcMaxPoint;				// 最大可投点数显示
	FlashStatic*		pStcBidden;					// 已投点数显示
	GUIStatic*			pStcTalentBranch;			// 包含树枝
	TCHAR				szIconPath[X_SHORT_NAME];	// 所用图标
	
	DWORD				dwIconCtrlId;				// 控制图标显示，如果此ID标识的技能没学，在精简模式下会显示图标为问号

	tagTalentLeaf(){ZeroMemory(this, sizeof(tagTalentLeaf)); dwTalentLeaf = dwChildLeaf = NULL; }
};

/** \class	 TalentTree
	\brief	 天资树
	\remarks 存储天资树各个节点及相应关系
*/
class TalentTree
{
public:
	TalentTree(void);
	~TalentTree(void);
	
	/** \创建天资树
	*/
	VOID Create(GUIWnd* pWnd, GUISystem* pGUI);
	/** \销毁天资树
	*/
	VOID Destroy();
	
	/** \获取叶节点技能ID
	*/
	DWORD GetCurLeafID(DWORD dwWndID){return m_mapIndex.Peek(dwWndID);}
	/** \获取叶节点数据结构
	*/
	tagTalentLeaf* GetCurLeaf(DWORD dwWndID);

	/** \设置当前页
	*/
	VOID SetCurPage(INT nPage);

	/** \隐藏当前页
	*/
	VOID HideCurPage();

	/** \更新天资树
	*/
	VOID UpdateTalentLeaf(DWORD dwID, BOOL bAdd = FALSE);
	/** \玩家升级对天资树的影响
	*/
	VOID OnRoleLevelUp(INT nLevel, INT nInc);
	/** \职业变化对天资树的影响
	*/
	VOID OnRoleClassChange(EClassType eCur, EClassType ePast);
	/** \天资点变化对天资树的影响
	*/
	VOID OnTalentPointChange(ETalentType eTalent, INT nCurPoint);
	
	/** \天资升级对天资树的影响
	*/
	VOID OnTalentLevelUp(DWORD dwPreTalentID);
	/** \没有天资点
	*/
	VOID OnNoTalentPoint();
	/** \有天资点可投
	*/
	VOID OnHaveTalentPoint();
	/** \已投天资类型达到饱和
	*/
	VOID OnNoTalentTypeCanChoose();
	/** \清空天资
	*/
	VOID OnClearAllTalent();
	
	/** 设置是否是精简显示模式
	*/
	VOID SetSimpleDisplayMode( BOOL bDisplayMode )
	{ 
		if( bDisplayMode != m_bSimpleDisplayMode )
		{
			m_bSimpleDisplayMode = bDisplayMode; 
			Refresh4DisplayMode();
		}
	}

private:
	/** \获取天资树配置文件
	*/
	VOID LoadTalentConfig(LPCTSTR szFile);
	/** \获取天资树叶节点
	*/
	VOID LoadTalentLeaf(XmlElement* pElement);
	
	/** \创建一个天资树背景
	*/
	VOID CreateWnd(ETalentType eType, GUIWnd* pFather, GUISystem* pGUI);
	/** \创建一个叶节点
	*/
	VOID CreateLeaf(tagTalentLeaf* pLeaf, GUIWnd* pFather, GUISystem* pGUI);
	/** \创建一个叶节点的树枝
	*/
	VOID CreateBranch(tagTalentLeaf* pLeaf, tagPoint ChildPos, GUIWnd* pFather, GUISystem* pGUI);
	
	/** \设置已投点数
	*/
	VOID SetBidden(tagTalentLeaf* pLeaf);

	/** 是否可投点
	*/
	BOOL IfSkillCanLevelUp(tagTalentLeaf* pLeaf);
	

	/** \获取当前叶节点的坐标
	*/
	VOID GetCurLeafPos(SIZE& size, tagPoint pos);
	/** \获取当前叶节点树枝的大小
	*/
	VOID GetCurBranchSize(SIZE& size, tagPoint pos, tagPoint child);

	/** \释放TMap的内容
	*/
	template<typename K, typename V>
	VOID FreeMap(TMap<K, V*> &mapList);

	/** \增加开启影响表
	*/
	VOID AddTalentActiveEffect(DWORD dwTypeID);
	/** \增加一个单元项
	*/
	template<typename A, typename B>
	VOID AddNode(TMap<A, TList<B>*> &mapList, A key, B value);

	/** \刷新技能开启状态
	*/
	BOOL IsTalentLeafActive(tagTalentLeaf* pLeaf);
	/** \刷新可投点天资技能列表
	*/
	VOID ReflashActiveTalentList(tagTalentLeaf* pLeaf);

	/** \获取下一级的TypeID
	*/
	DWORD GetNextLevelTalentTypeID(tagTalentLeaf* pLeaf, DWORD dwSkillType);
	
	/** \更新天资Tips
	*/
	VOID ReflashTalentTips(tagTalentLeaf* pLeaf);

	/** 根据显示模式刷新所有天资的技能的图标
	*/
	VOID Refresh4DisplayMode();

	/** 学会一个技能后，刷新依赖于这个技能的技能图标
		\param 已经学会的技能
	*/
	VOID RefreshTalentIcon( DWORD dwLearnedSkillTypeID );

private:
	TObjRef<Util>	m_pUtil;

	TMap<DWORD, tagTalentLeaf*>		m_mapTalent;						//--天资树数据存储容器数组，其中键值为天资树叶节点天资ID
	TMap<DWORD, tagTalentLeaf*>		m_mapBoxFirst2Talent;				//--盒子内第一个技能对应天资叶子节点的映射
	TMap<DWORD, DWORD>				m_mapIndex;							//--天资树索引，通过对应控件ID找到对应的技能ID
	GUIWnd*							m_pWndTalent[ETT_End];				//--各种天资树的背景控件
	TList<DWORD>					m_listActiveTalent[ETT_End];		//--可投点天资技能列表
	TMap<INT, TList<DWORD>*>		m_mapTalentPointEffect[ETT_End];	//--天资点数对技能的开启影响
	TMap<DWORD, TList<DWORD>*>		m_mapTalentSkillEffect;				//--技能等级对后续技能的开启影响
	TMap<INT, TList<DWORD>*>		m_mapRoleLevelEffect;				//--玩家等级对技能的开启影响
	TMap<EClassType, TList<DWORD>*>	m_mapRoleClassEffect;				//--玩家职业对技能的开启影响
	INT								m_nCurPage;							//当前页

	BOOL							m_bSimpleDisplayMode;				//是否是精简的显示模式，如果是有些图标要显示成问号并且无tips
	
};


