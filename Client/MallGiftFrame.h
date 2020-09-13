#pragma once

enum MallGiftFlag
{
	MGF_NULL	= 0,
	MGF_OK		= 1,	// 确定
	MGF_Cancel	= 2,	// 取消
};

struct tagMsgMallGiftEvent : public tagGameEvent
{
	tagMsgMallGiftEvent(LPCTSTR szEventName, GameFrame* pSenderFrame):tagGameEvent(szEventName, pSenderFrame){}
	MallGiftFlag	eRet;
	tstring			strName;			//玩家姓名
	INT16			n16Num;				//赠送物品个数
	tstring			strWord;			//赠言
	DWORD			dwTime;
};

struct tagRefreshFriendsList : public tagGameEvent
{
	tagRefreshFriendsList(LPCTSTR szEventName, GameFrame* pSenderFrame):tagGameEvent(szEventName, pSenderFrame){}
};

class MallGiftFrame : public GameFrame
{
public:
	MallGiftFrame(void);
	~MallGiftFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

	VOID UpdateNameList();
	void SetBuyNum();
	void SetMaxBuyNum(INT nMaxNum);
	void ShowGiftFrame(BOOL bShow);
private:
	// 处理GUI消息
	DWORD EventHandler(tagGUIEvent* pEvent);
	VOID ClearUIData();
	BOOL UpdateListData();
	BOOL UpdateListFrame();
	DWORD OnRefreshFriendsList(tagRefreshFriendsList* pEvent);
	DWORD OnClose_UIEvent(tagGameEvent* pEvent);
	VOID  DeleteRETURNS(tstring& strText);
private:

	TSFPTrunk<MallGiftFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<GameFrameMgr>		m_pFrameMgr;

	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndCaption;		
	GUIButton*					m_pBtnOK;
	GUIButton*					m_pBtnCancel;
	GUIEditBox*					m_pEdtName;					//玩家姓名
	GUIListBox*					m_pLBNameList;	
	//GUIComboBox*				m_pCbNameBox;				//玩家姓名列表组合框
	GUIEditBox*					m_pEdtNum;					//个数
	GUIEditBox*					m_pEditWord;				//赠言
	//DWORD						m_curRow;
	std::vector<LPCTSTR>		m_vecFriendsArray;			//玩家名字列表
};