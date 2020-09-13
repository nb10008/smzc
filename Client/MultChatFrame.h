#pragma once
#include "gameframe.h"

//-----------------------------------------------------------------------------
//!class MultChatFrame.h
//!brief 群聊界面  
//-----------------------------------------------------------------------------
class RichEditBoxForChatInput;
class RichEditBoxForChatShow;
class GlintListBox;
enum E_Click_Btn_Type
{
	Click_Btn_NULL = 0,
	Click_Btn_Create,
	Click_Btn_Delete,
	Click_Btn_Quit,
	Click_Btn_Kick,
	Click_Btn_AddMember
};
class MultChatFrame :
	public GameFrame
{
public:
	MultChatFrame(void);
	~MultChatFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();
	virtual VOID Update();

	VOID   ShowWnd(BOOL b);
	BOOL   FrameIsInvisible();
	   
private:
	//m_pWnd响应
	DWORD	EventHandler(tagGUIEvent* pEvent);
	//右键点击成员列表的菜单响应
	DWORD	EventHandlerMenberMenu(tagGUIEvent* pEvent);
	//添加新成员m_pWndAddNew响应
	DWORD	OnWnd_AddNew(tagGUIEvent* pEvent);
	//创建群聊msgbox响应
	DWORD	OnInputStringMsgbox(tagMsgInputBoxEvent* pGameEvent);
	//删除、退出的msgbox响应
	DWORD   OnMsgBoxClickEvent(tagMsgBoxEvent* pGameEvent);
	VOID	KillObject();
	VOID    RefreshGroupList();
	VOID    RefreshMemberList();
	VOID    RefreshChatBoard();
    VOID    RefreshGlint();
	VOID    RefreshTitle();
	VOID    DeleteRETURNS(tstring& strText);
	VOID    RefreshAddMenberButton();

private:
	TObjRef<GUISystem>			m_pGUI;
	TSFPTrunk<MultChatFrame>	m_trunk;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;


	GUIWnd*						m_pWnd;
	GUIButton*                m_BtnNewChatGroup;		//创建聊天群
	GUIButton*			       m_BtnDelChatGroup;		//删除聊天群
	GUIButton*                m_BtnQuitChatGroup;		//退出聊天群
	GUIButton*					m_BtnAddMember;			//添加新成员
	GUIButton*					m_BtnSendMessage;		//发送消息
	GUIButton*					m_BtnExitFrame;			//退出群聊界面
	GlintListBox*             m_LBGroupNameList;		//群聊组名称列表
	GUIListBox*               m_LBMemberNameList;		//群聊群成员列表
	GUIStaticEx*              m_StcShowGroupIn;		//显示在那个群
	//RichEditBoxForChatShow*	   m_REBChatBlank;			//群聊天窗口
	GUIRichEditBox*			   m_REBChatBlank;			//群聊天窗口	
	RichEditBoxForChatInput*   m_REBLocalMessage;		//本地信息发送窗口

	INT							m_clickGroupRow;				// 当前选中聊天组的Row
	INT							m_clickMemberRow;				//当前选中成员的ROW			
	E_Click_Btn_Type			m_eClickBtnType;


	//----------------------------添加新成员子窗口-------------------------------
	GUIWnd*						 m_pWndAddNew;
	GUIButton*                 m_pBn_Ok_Set;                 //确定
	GUIButton*                 m_pBn_Cancel_Set;             //取消
	GUIButton*                 m_pBn_Close_Set;              //关闭
	GUIEditBox*					 m_pEb_FriendName;			//输入框
	GUIStatic*					 m_pStc_Title;				//标题框
	GUIComboBox*				 m_pCb_Friends_Box;				//好友列表组合框
	DWORD						m_curRow;					//好友列表计行		
	//----------------------------右键点击弹出的子菜单----------------------------
	GUIWnd						*m_pWndMenberMenu;			// 一级子菜单
	GUIListBox					*m_pListMenberMenu;			// 一级列表

};
