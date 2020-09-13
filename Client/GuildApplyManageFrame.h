#pragma once
#include "gameframe.h"
#include "..\WorldDefine\guild_define.h"
#include "..\WorldDefine\msg_guild.h"

class GuildApplyManageFrame : public GameFrame
{
public:
	GuildApplyManageFrame();
	~GuildApplyManageFrame();

	virtual BOOL Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam );
	virtual BOOL ReloadUI();
	virtual BOOL Destroy();
	virtual BOOL EscCancel();

	void AddPlayerInfo( tagGuildApplyInfo& info );
	
	void ShowWnd( BOOL bShow );

private:
	typedef std::list<tagGuildApplierBillInfo>::iterator PlayerListIterator;

	DWORD OnNS_GuildApplierBill( tagNS_GuildApplierBill* pMsg, DWORD dwParam );

	BOOL EventHandler( tagGUIEvent* pEvent );
	
	tstring GetClassName( EClassType eClassType );

	int GetPages(){ return (m_lstPlayers.size() / GUILD_APPLY_MAX_ITEMS_PER_PAGE) + 1 ; }

	/** 判断当前页是否合法
	*/
	bool IsCurPageGood(){ return m_nCurPage >= 0 && m_nCurPage < GetPages(); }

	/** 根据当前页，当前列表框中的选中项，计算帮派数据列表的索引
	*/
	int GetPlayerDataIndex();

	void UpdateGuildList( tagNS_GuildApplierBill* pMsg );

	void RefreshDisplay();

	/** 处理选中帮派列表中的条目事件
	*/
	void SelectListBoxItem( INT nSelectItem );

	GuildApplyManageFrame::PlayerListIterator FindGuildInfo( int nPlayerDataIndex );

	void OnChat();

	void OnRefuseApply();

	void OnAcceptApply();

	void AddPlayerInfo( std::list<tagGuildApplyInfo >& lstInfo );
	
	/** 清空列表框
	*/
	void ClearListBox();

	void DeletePlayer( DWORD dwRoleID );

	tstring GetOnlineText( DWORD dwTime );

private:
	TSFPTrunk<GuildApplyManageFrame> m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetCmdMgr>				m_pCmdMgr;
	TObjRef<NetSession>				m_pSession;

	GUIWnd*							m_pWnd;
	GUIPatch*						m_pPatchPlayers;		//玩家列表页
	GUIPushButton*					m_pPbnGuildTenet;		//帮派宗旨
	GUIPushButton*					m_pPbnGuildApply;		//申请入帮
	GUIButton*						m_pBtnClose;			//关闭按钮
	GUIButton*						m_pBtnChat;				//设为私聊对象
	GUIButton*						m_pBtnAccept;			//同意
	GUIButton*						m_pBtnRefuse;			//拒绝
	GUIListBox*						m_pLstPlayers;			//玩家列表

	GUIButton*						m_pBtnPageBack;			//上一页
	GUIButton*						m_pBtnPageFront;		//下一页
	GUIStatic*						m_pStcPageNum;			//当前页/最大页显示
	
	//宗旨界面
	GUIPatch*						m_pPatchTenet;			//帮派宗旨
	GUIEditBoxEx*					m_pEditBoxTenet;		//宗旨输入框
	GUIButton*						m_pBtnModify;			//修改宗旨
	GUIButton*						m_pBtnCancel;			//取消按钮

	std::list<tagGuildApplierBillInfo>	m_lstPlayers;		//申请加入帮派的玩家信息列表
	int								m_nCurPage;
	int								m_nCurSelectItem;		//玩家列表中被选中条目的索引
};
