#pragma once
#include "gameframe.h"
#include "GuildMembers.h"
#include "..\WorldDefine\msg_GuildBattle.h"
#include "..\WorldDefine\msg_guild.h"
#include "..\WorldDefine\guild_define.h"


struct tagRoleGetNameEvent;
struct tagGetNameByNameID;
struct tagGuildPosAppointEvent;
struct tagGuildInfoReflashEvent;
struct tagGuildNameGetEvent;
struct tagGuildBase;
struct tagGuildMemberClient;
struct tagUseGuildSkillBookEvent;
struct tagRefreshRemnantStateEvent;
struct tagGetSomeGuildNameEvent;
struct tagNS_GetAllGuildSmallInfo;
struct tagNS_SearchGuild;
struct tagNS_DeclareGuildWar;
struct tagNS_GuildWarEnd;
struct tagNS_SyncWarStateGuild;
struct tagNS_GetGuildUpgradeInfo;
struct tagNS_UpdateFacilitiesInfo;

class IconStatic;
struct tagGuildSkill;
class Item;
class GlintButton;
struct tagSureAllianceEvent;
struct tagMoneyConfirmApply;
class ItemButton;
struct tagNS_ActiveGuildSkill;

#define GUILD_SKILL_ITEM_NUM_PERPAGE 12
#define GUILD_UPGRADE_NUM 4
#define GUILD_WAR_MAXALLY_NUM 5

class GuildFrame :
	public GameFrame
{
public:
	typedef vector<GUIButton*> GUIButtonList;
	
	struct tagMemberNameReq
	{
		INT		nIndex;
		DWORD	dwColor;

		tagMemberNameReq(INT nIdx, DWORD dwClr){nIndex = nIdx; dwColor = dwClr;}
	};

	struct tagTempItem
	{
		DWORD dwID;
		INT64 n64ID;
		tagTempItem():dwID(0),n64ID(0)
		{}

		VOID clear()
		{
			dwID = 0;
			n64ID = 0;
		}
	};

	typedef map<DWORD, tagMemberNameReq> NameReqList;  
	
	// 帮派界面状态
	enum EGuildFrameState
	{
		EGFS_MemberList,	//成员类表
		EGFS_Remnant,		//神迹状态（准备用来实现战场状态）
		EGFS_GuildSkill,	//工会技能
		EGFS_Construction,	//工会设施
		EGFS_War			//工会战争
	};

public:
	GuildFrame(void);
	~GuildFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual VOID Update();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	EGuildFrameState GetFrameState() const { return m_eGuildFrameState; }
	VOID ShowWnd(bool bShow){ m_bShow = bShow; m_pWnd->SetInvisible(!bShow);}
	bool IsShow() const { return m_bShow; }
private:

	typedef std::list<tagGuildSmallInfo>::iterator GuildListIterator;

	VOID EventHandler(tagGUIEvent* pEvent);

	// 刷新帮派
	VOID ReflashGuild();
	// 刷新列表
	VOID ReflashList();
	// 刷新列表IMPL
	VOID ReflashListImpl();
	// 刷新功能按钮
	VOID ReflashButton();
	// 刷新技能
	void RefreshSkills();
	// 刷新神迹状态
	void RefreshGodState();

	void UpdateSelectGodInfo(EGodMiracle eGod);

	// 获取离线时间显示
	tstring GetOfflineTime(bool bIsOnline, DWORD dwOfflineTime = GT_INVALID);
	
	// 发送网络消息
	VOID SendGuildAppoint(DWORD dwDstRoleID, INT nPos);
	VOID SendGuildLeave();
	VOID SendGuildKick(DWORD dwDstRoleID);
	VOID SendGuildTurnover(DWORD dwDstRoleID);
	VOID SendGuildDemiss();
	VOID SendGuildTenetChange(tstring strTenetInfo);
	
	// 处理游戏事件
	DWORD OnEventGetRoleName(tagRoleGetNameEvent* pGameEvent);
	DWORD OnEventGetNameByNameID(tagGetNameByNameID* pGameEvent);
	DWORD OnEventGetGuildName(tagGuildNameGetEvent* pGameEvent);
	DWORD OnEventGetGuildTenet(tagGameEvent* pGameEvent);
	DWORD OnEventGuildPosAppoint(tagGuildPosAppointEvent* pGameEvent);
	DWORD OnEventGuildInfoReflash(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventGuildListReflash(tagGameEvent* pGameEvent);
	DWORD OnEventGuildInfoReq(tagGuildInfoReflashEvent* pGameEvent);
	DWORD OnEventMsgBoxInputTenet(tagMsgInputBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxLeaveCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxTurnoverCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxKickCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventMsgBoxDemissCheck(tagMsgBoxEvent* pGameEvent);
	DWORD OnEventRefreshGuildSkills(tagGameEvent *pEvent);
	DWORD OnEventShowGuildFrame(tagGameEvent *pEvent);
	DWORD OnEventHideGuildFrame(tagGameEvent *pEvent);
	DWORD OnEventRefreshGuild(tagGameEvent *pEvent);
	DWORD OnEventUsGuildSkillBook(tagUseGuildSkillBookEvent *pGameEvent);
	DWORD OnEventRefreshBook(tagGameEvent *pGameEvent);
	DWORD OnEventCommerceWarningMsgBox(tagMsgBoxEvent *pEvent);
	DWORD OnEventRefreshClanRemnantState(tagGameEvent *pGameEvent);
	DWORD OnEventApply(tagMsgBoxEvent *pEvent);
	DWORD OnEventAddMoney(tagMsgBoxEvent *pEvent);
	DWORD OnEventShowRemnant(tagGameEvent *pEvent);
	DWORD OnEventKickGuildMember(tagGameEvent *pEvent);
	DWORD OnEventOpenGuildAllianceList(tagSureAllianceEvent *pEvent);
	DWORD OnEventMsgBoxWarCheck(tagMsgBoxEvent* pGameEvent);

	DWORD OnEventApplyAttackAlly(tagMsgBoxEvent *pEvent);
	DWORD OnEventApplyDefenceAlly(tagMsgBoxEvent *pEvent);

	DWORD OnEventMoneyConfirmApply(tagMoneyConfirmApply *pGameEvent);
	DWORD OnEventTurnOverHuiZhang(tagGameEvent *pGameEvent);


	DWORD OnNS_GuildApplierBill( tagNS_GuildApplierBill* pMsg, DWORD dwParam );

	DWORD OnNS_DeclareGuildWar(tagNS_DeclareGuildWar* pMsg, DWORD dwParam);
	DWORD OnNS_GuildWarEnd(tagNS_GuildWarEnd* pMsg, DWORD dwParam);
	DWORD OnNS_SyncWarStateGuild(tagNS_SyncWarStateGuild* pMsg, DWORD dwParam);

	DWORD OnNS_GetGuildUpgradeInfo( tagNS_GetGuildUpgradeInfo* pCmd, DWORD );
	DWORD OnNS_UpdateFacilitiesInfo( tagNS_UpdateFacilitiesInfo* pCmd, DWORD );
	DWORD OnNS_ActiveGuildSkill( tagNS_ActiveGuildSkill* pCmd, DWORD );

	// 处理按钮按下事件
	VOID OnClickedLookupInfo();
	VOID OnClickedLeaveGuild();
	VOID OnClickedTurnoverLeader();
	VOID OnClickedAppointMember();
	
	//VOID OnClickedModifyTenet();
	void OnClickManageGuild();
	VOID OnClickedKickMember();
	VOID OnClickedDemissPostion();

	VOID DestroyMsgBox();
	void SetSkillDefaultUI();
	void OnSkillButtonClick(int nButton);		// 选中某个技能button
	void OnClickPreOrNextButton(bool bOrder);	// true为后退，false为前进

	// 获取当前行的成员
	const tagGuildMemberClient* GetMember(INT nIndex);

	/**
		\brief 得到第 nRow 行正在显示的成员 在整个帮派表中的实际索引
		\param nRow 鼠标停留的行
		\return 实际的索引
	*/
	int GetMember2GuildTips( int nRow );

	//  [7/12/2010 zhangsu]

	/** 获得帮派信息
	*/
	DWORD OnNS_GetAllGuildSmallInfo( tagNS_GetAllGuildSmallInfo* pMsg, DWORD dwParam );

	/** 搜索帮派
	*/
	DWORD OnNS_SearchGuild( tagNS_SearchGuild* pMsg, DWORD dwParam );

	void UpdateGuildList( tagNS_GetAllGuildSmallInfo* pMsg );

	void AddGuildInfo( tagGuildSmallInfo& info );

	/** 刷新列表框里显示的帮派的信息
	*/
	void RefreshGuildsDisplay();

	/** 清空列表框
	*/
	void ClearListBox();

	/** 得到总页数
	*/
	int GetPages(){ return (m_lstGuildInfo.size() / GUILD_APPLY_MAX_ITEMS_PER_PAGE) + 1 ; }

	/** 处理选中帮派列表中的条目事件
	*/
	void SelectListBoxItem( INT nSelectItem );

	/** 搜索合适的帮派
	\param nType 0 为是按照帮派的名字搜索，1是按照帮主的名字搜索
	\return 如果找到合适的帮派返回其在listBox中的索引，否则返回GT_INVALID
	*/
	INT SearchGuild( INT type, tstring strName );

	VOID OnStartWar();
	VOID UpdataFacilitiesInfoUI( const EFacilitiesType eType );

	VOID HandInItems(const EFacilitiesType eType);

	// 刷新同盟列表
	VOID RefreshAlliance(EGodMiracle eType);

	VOID SureAlliance();

	VOID SelectListBoxAlly(INT nSelectItem);

	VOID RefreshAllianceDisplay();

	VOID RefreshGuildXuanZhan();
private:
	TSFPTrunk<GuildFrame>		m_Trunk;
	TObjRef<NetSession>			m_pSession;
	TObjRef<GUISystem>			m_pGUI;	
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	NameReqList					m_mapTemp;
	vector<INT>					m_vecIndex;			// 成员索引缓存
	GuildMembers::EListColType	m_eSortType;		// 排序方式


	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndMembers;
	GUIButton*					m_pBtnClose;
	GUIListBox*					m_pLstMembers;		// 成员列表
	GUIStatic*					m_pStcName;			// 帮派名称
	GUIStatic*					m_pStcLevel;		// 等级
	IconStatic*					m_pStcSymbol;		// 帮派标志
	GUIStatic*					m_pStcFounder;		// 创始人
	GUIStatic*					m_pStcLeader;		// 帮主
	GUIStatic*					m_pStcMemberNum;	// 成员数量	
	GUIStatic*					m_pStcReputation;	// 威望

	GUIButton*					m_pBtnZhongzhi;		//帮派宗旨
	

	GUIStatic*					m_pStcFund;			// 资金
	GUIStatic*					m_pStcMaterial;		// 资材
	GUIStatic*					m_pStcPeace;		// 安宁度
	GUIStatic*					m_pStcDailyCost;	// 每日维护费
	GUIEditBoxEx*				m_pEdtTenet;		// 帮派宗旨
	GUIPushButton*				m_pPbnMembers;		// 成员列表
	GUIPushButton*				m_pPbnSkills;		// 帮派技能
	GUIPushButton*				m_pPbnInfomation;	// 战争状态
	GUIPushButton*				m_pPbnWar;			// 工会宣战
	GUIPushButton*				m_pPbnConstruction;	// 工会设施
	
	
	GUIButtonList				m_pBtnListIndex;	// 成员列表表头
	GUIButton*					m_pBtnLookupInfo;	// 查看信息
	GUIButton*					m_pBtnLeaveGuild;	// 离开帮派
	GUIButton*					m_pBtnTurnLeader;	// 移交帮主
	GUIButton*					m_pBtnAppointMember;// 任命职位
	GlintButton*				m_pBtnManageGuild;	// 修改宗旨,修改为支持闪烁
	GUIButton*					m_pBtnKickMember;	// 开革成员
	GUIButton*					m_pBtnDemissPostion;// 辞去职务
	GUIPushButton*				m_pPbnOffline;		// 显示离线

	GUIPatch*					m_pGuildMemberPatch;
	GUIListBox*					m_pGuildMemberListBox;

	const tagGuildBase*			m_pGuild;			// 帮派信息

	DWORD						m_dwDstRoleID;		// 操作的目标ID

	GUIWnd *m_pWndGuild;			// 父窗体
	IconStatic *m_pBtnCurSkillIcon;	// 当前研究技能图标
	GUIStatic *m_pStcCurSkillName;	// 当前研究技能名称
	GUIStatic *m_pStcCurSkillLevel; // 当前研究技能等级
	GUIStatic *m_pStcFundCost;		// 消耗帮派资金
	GUIStatic *m_pStcMaterialCost;	// 消耗帮派资材
	GUIProgress * m_pProResearchProgress; //当前技能研究进度

	ItemButton *m_pBtnBookIcon;		// 放入典籍
	GUIStatic *m_pProgressIncrement; // 进度增量
	GUIButton *m_pLearnBook;		// 精研典籍

	GUIButton *m_pBtnBack;
	GUIButton *m_pBtnNext;

	GUIStatic *m_pStcLearnGoldCost;			// 学习所需金子
	GUIStatic *m_pStcLearnSilverCost;		// 学习所需银子
	GUIStatic *m_pStcLearnContributionNeed; // 学习所需帮派贡献
	GUIStatic *m_pStcLearnGuildFundCost;	// 学习所需帮派资金
	GUIStatic *m_pStcLearnGuildMaterialCost;// 学习所需帮派资材

	GUIButton *m_pBtnActiveResearch;		// 激活研究
	GUIButton *m_pBtnLearnSkill;			// 学习技能
	GUIButton *m_pBtnCancelResearch;		// 取消研究
	GUIButton *m_pBtnStartResearch;			// 开始研究
	

	GUIPatch *m_pSkillPatch;
	IconStatic *m_pIstState[5];				// 帮派状态
	
	IconStatic *m_pSkillItemBack[GUILD_SKILL_ITEM_NUM_PERPAGE];
	IconStatic *m_pSkillItem[GUILD_SKILL_ITEM_NUM_PERPAGE];
	GUIStatic *m_pSkillName[GUILD_SKILL_ITEM_NUM_PERPAGE];
	GUIStatic *m_pSkillLevel[GUILD_SKILL_ITEM_NUM_PERPAGE];
	
	GUIPatch *m_pRemnantStatePatch;			// 神迹状态
	
	GUIListBox *m_pLstClanState;			// 氏族神迹状态列表

	GUIStatic *m_pStcBattleDesc;			
	GUIStatic *m_pStcBattleTime;			
	
	GUIStatic *m_pStcAttackGuildName;		// 进攻帮派名称
	GUIStatic *m_pStcAttackGuildLv;			// 进攻帮派等级
	GUIStatic *m_pStcAttackGuildMember;		// 进攻帮派成员数
	//GUIStatic *m_pStcAttackMoney;			// 当前出价（F计划更改）

	GUIStatic *m_pStcDefenceGuildName;		// 防御帮派名称
	GUIStatic *m_pStcDefenceGuildLv;		// 防御帮派等级
	GUIStatic *m_pStcDefenceGuildMember;	// 防御帮派成员数
	//GUIStatic *m_pStcDefenceDay;			// 连续占领天数
	
	GUIEditBox *m_pEdtMoney;				// 竞价输入框
	GUIButton *m_pBtnApply;					// 报名
	GUIStatic *m_pStcNowBegMoney;			// 对该战场的当前出价

	GUIButton *m_pBtnBackMoney;				// 追加竞价
	GUIButton *m_pBtnApplyAttackAlly;		// 申请进攻者同盟
	GUIButton *m_pBtnApplyDefenceAlly;		// 申请防御者同盟
	GUIButton *confirmAllys;				// 确认同盟

	GUIStatic *m_pStcAttackAllies[GUILD_WAR_MAXALLY_NUM];			// 进攻同盟者
	GUIStatic *m_pStcDefenceAllies[GUILD_WAR_MAXALLY_NUM];			// 防御同盟者

	int m_nCurChooseRenmant;				// 当前选中神迹
	bool m_bNeedUpdate;

	std::vector<DWORD> m_vecClanStateID;	// 记录神迹ID
	int m_nClanSateFlag;

	int m_nCurPageNum;	// 当前翻页数
	DWORD m_dwCurChooseSkill; // 当前选中帮派技能ID;
	std::vector<tagGuildSkill*> m_vecCurPageItem;
	//  [7/12/2010 zhangsu] 帮派战争辅助数据
	tstring							m_strSearchName;		//正在搜索的名字
	std::list<tagGuildSmallInfo>	m_lstGuildInfo;			//所有帮派信息

	//std::set<DWORD>					m_setWarGuilds;			//处在战争中的公会
	
	EGuildFrameState m_eGuildFrameState;
	bool m_bShow;
	Item *m_pSkillBook;			// 保存典籍的指针

	DWORD m_dwTempRoleID; // 临时保存角色ID
	INT m_nTempPOS;     // 临时保存POSID
	BOOL m_bIsHaveGuildApply;  //是否有人申请入帮，TRUE为有FALSE为无

	//  [7/12/2010 zhangsu]
	GUIPatch*					m_pGuildConstruction;	//工会设施

	tagGuildFacilitiesInfo	        m_sFacilitiesInfo[MAX_GUILD_FACILITIES_TYPE];  // 设施升级信息
	GUIStatic *m_pStcConstrLevel[EFT_End];		//等级
	GUIStatic *m_pStcConstrGold[EFT_End];		//金
	GUIStatic *m_pStcConstrSilver[EFT_End];		//银
	GUIStatic *m_pStcConstrResource[EFT_End];	//资源
	GUIProgress * m_pProConstResearch[EFT_End];	//升级进度

	GUIButton *m_pBtnLevelUp[EFT_End];			//升级

	//工会战争

	GUIPatch*					m_pGuildWar;	//工会战争

	GUIListBox*					m_pLstGuilds;		// 成员列表
	GUIButton *m_pBtnPageBack;
	GUIButton *m_pBtnPageFront;

	GUIButton *m_pBtnSearch;			// 工会搜索
	GUIButton *m_pBtnPartyWarStart;			// 宣战
	GUIButton *m_pBtnPartyWarCancel;		// 取消

	GUIStatic *m_pStcWarTimes;	//公会宣战剩余次数

	GUIStatic *m_pStcPageNum;	
  	INT								m_nMaxPage;				//最大页
  	INT								m_nCurPage;				//当前页 
  	int								m_nCurSelectItem;		//正在申请加入的帮派在listBox中的条目索引

	//搜索界面
	GUIWnd*							m_pWndSearch;			//帮派搜索界面
	GUIEditBox*						m_pWndInput;			//输入要搜索的名字
	GUIButton*						m_pBtnSearchOk;			//确定
	GUIButton*						m_pBtnSearchCancel;		//取消
	GUIButton*						m_pBtnSearchClose;		//关闭
	GUIPushButton*					m_pPbtnLeader;			//按照帮主名字搜索
	GUIPushButton*					m_pPbtnGuild;			//按照帮派的名字搜索

	//帮派宗旨界面
	GUIWnd*							m_pWndtenet;			
	GUIEditBoxEx*					m_pEditBoxTenet;		//宗旨输入框
	GUIButton*						m_pBtnModifyTenet;			//修改宗旨
	GUIButton*						m_pBtnCancelTenet;			//取消按钮
	GUIButton*						m_pBtnCloseTenet;			//取消按钮

	//确认同盟界面
	GUIWnd*							m_pWndAlliance;
	GUIListBox*						m_pLstAlliance;				//同盟列表
	GUIButton*						m_pBtnAllianceOk;			//确定
	GUIButton*						m_pBtnAllianceCancel;		//取消
	GUIButton*						m_pBtnAllianceXclose;		//X号关闭键
	GUIStatic*						m_pStcNowAllies;			//当前同盟数量
	GUIStatic*						m_pStcRemainAllies;			//剩余可选同盟数量
	int								m_nCurAlly;
	DWORD							m_nCurAllyID;
	bool							needRefresh;
	EGodMiracle						m_eTypeEx;					// 刷新辅助参数

	// 激活公会技能界面
	GUIWnd*							m_pWndActiveSkill;
	GUIButton*						m_pBnActive;				// 激活按钮
	ItemButton*						m_pIbnActiveItem;			// 激活物品
	GUIButton*						m_pBnActiveClose;			// 关闭按钮
	tagTempItem						m_strActiveItem;			//激活使用物品的ID
	// 刷新标志位
	bool							m_bNeedRefreshAlliance;

	
};

