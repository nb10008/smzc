#pragma once
#include "gameframe.h"
#include "..\WorldDefine\guild_define.h"

struct tagNS_GetAllGuildSmallInfo;
struct tagNS_GetGuildPurpose;
struct tagNS_WantToJoinGuild;
struct tagNS_ContactGuildLeader;
struct tagNS_SearchGuild;

class GuildListFrame : public GameFrame
{
public:
	GuildListFrame(void);
	~GuildListFrame(void);

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL ReloadUI();
	virtual BOOL Destroy();
	virtual BOOL EscCancel();

	void Show( bool bShow )
	{
		m_pWndApply->SetInvisible( !bShow );
		m_pWndApply->SetRefresh( TRUE );
	}

private:
	typedef std::list<tagGuildSmallInfo>::iterator GuildListIterator;

	BOOL EventHandler( tagGUIEvent* pEvent );

	/** 获得帮派信息
	*/
	DWORD OnNS_GetAllGuildSmallInfo( tagNS_GetAllGuildSmallInfo* pMsg, DWORD dwParam );

	/** 获得帮派宗旨 
	*/
	DWORD OnNS_GetGuildPurpose( tagNS_GetGuildPurpose* pMsg, DWORD dwParam );

	/** 申请加入帮派的回复
	*/
	DWORD OnNS_WantToJoinGuild( tagNS_WantToJoinGuild* pMsg, DWORD dwParam );

	/** 搜索帮派
	*/
	DWORD OnNS_SearchGuild( tagNS_SearchGuild* pMsg, DWORD dwParam );

	/** 联系帮主
	*/
	DWORD OnNS_ContactGuildLeader( tagNS_ContactGuildLeader* pMsg, DWORD dwParam );

	/** 加入帮派申请的处理函数
	*/
	DWORD OnApplyToJoinGuild( tagMsgBoxEvent* pEvent );
	
	/** 得到总页数
	*/
	int GetPages(){ return (m_lstGuildInfo.size() / GUILD_APPLY_MAX_ITEMS_PER_PAGE) + 1 ; }
	
	/** 判断当前页是否合法
	*/
	bool IsCurPageGood(){ return m_nCurPage >= 0 && m_nCurPage < GetPages(); }

	void UpdateGuildList( tagNS_GetAllGuildSmallInfo* pMsg );

	void AddGuildInfo( tagGuildSmallInfo& info );

	/** 刷新列表框里显示的帮派的信息
	*/
	void RefreshGuildsDisplay();

	/** 搜索合适的帮派
		\param nType 0 为是按照帮派的名字搜索，1是按照帮主的名字搜索
		\return 如果找到合适的帮派返回其在listBox中的索引，否则返回GT_INVALID
	*/
	INT SearchGuild( INT type, tstring strName );

	
	/** 根据帮派数据列表的索引值查找对应的数据
		\return 返回值 要判断是否等于 m_lstGuildInfo.end() 因为
				列表可能在此期间被改变
	*/
	GuildListFrame::GuildListIterator FindGuildInfo( int nGuildDataIndex );

	/** 根据帮派的ID查找帮派信息
	*/
	GuildListFrame::GuildListIterator FindGuildInfoByID( DWORD dwGuildID );

	/** 打开申请加入帮派对话框
	*/
	void OpenApplyMsgBox();

	/** 打开帮派宗旨框
	*/
	void OpenGuildPurpose();

	/** 联系帮主
	*/
	void ContactGuildLeader();
	
	/** 根据当前页，当前列表框中的选中项，计算帮派数据列表的索引
	*/
	int GetGuildDataIndex();

	/** 处理选中帮派列表中的条目事件
	*/
	void SelectListBoxItem( INT nSelectItem );

	/** 清空列表框
	*/
	void ClearListBox();

private:
	TSFPTrunk<GuildListFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;
	
	//帮派列表界面
	GUIWnd*							m_pWndApply;
	GUIButton*						m_pBtnPageBack;			//上一页
	GUIButton*						m_pBtnPageFront;		//下一页
	GUIButton*						m_pBtnSearch;			//帮派搜索
	GUIButton*						m_pBtnPurpose;			//帮派宗旨
	GUIButton*						m_pBtnApply;			//入帮申请
	GUIButton*						m_pBtnCancel;			//取消
	GUIButton*						m_pBtnClose;			//关闭按钮
	GUIStatic*						m_pStcPageNum;			//当前页/最大页显示
	GUIListBox*						m_pLstGuilds;			//帮派列表

	//搜索界面
	GUIWnd*							m_pWndSearch;			//帮派搜索界面
	GUIEditBox*						m_pWndInput;			//输入要搜索的名字
	GUIButton*						m_pBtnSearchOk;			//确定
	GUIButton*						m_pBtnSearchCancel;		//取消
	GUIButton*						m_pBtnSearchClose;		//关闭
	GUIPushButton*					m_pPbtnLeader;			//按照帮主名字搜索
	GUIPushButton*					m_pPbtnGuild;			//按照帮派的名字搜索

	//帮派宗旨界面
	GUIWnd*							m_pWndTenet;			//帮派宗旨界面
	GUIButton*						m_pBtnContactLeader;	//联系帮主
	GUIButton*						m_pBtnTenetCancel;		//取消
	GUIStaticEx*					m_pStcTenet;			//帮派宗旨内容

	std::list<tagGuildSmallInfo>	m_lstGuildInfo;			//所有帮派信息
	INT								m_nMaxPage;				//最大页
	INT								m_nCurPage;				//当前页

	tstring							m_strSearchName;		//正在搜索的名字
	int								m_nCurSelectItem;		//正在申请加入的帮派在listBox中的条目索引

	DWORD							m_dwLookingPurposeGuildID;	//正在查看此帮派的帮派宗旨
};
