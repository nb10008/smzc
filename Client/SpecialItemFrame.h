#pragma once
#include "gameframe.h"

struct tagSpecItemEvent;
struct tagNS_SpecScriptItem;
struct tagNC_AddPetLiveTime;

/** \class SpecialItemFrame

	\brief ：特殊物品的操作使用相关

	\remarks 处理特殊物品的操作 ，主要是为了方便给以后没有办法归类的
	有特殊物品找一个合适的地方来植入程序。
*/
class SpecialItemFrame :
	public GameFrame
{
public:
	SpecialItemFrame(void);
	~SpecialItemFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual BOOL EscCancel();

private:
	//事件相应
	//三八妇女节活动相关事件
	BOOL  OnWnd_WomenFestival( tagGUIEvent* pEvent );
	BOOL  On_WomenFestivalShow( tagSpecItemEvent* pEvent );
	BOOL  OnEvent_OpenSpecFrame(tagGameEvent *pEvent);
	BOOL  OnEvent_CloseSpecFrame(tagGameEvent *pEvent);

	//网络消息事件
	//三八妇女节活动相关事件
	DWORD NS_SpecScriptItem(tagNS_SpecScriptItem* pNetCmd, DWORD dwParam);

	VOID SafeDestroyUI();
private:

	TSFPTrunk<SpecialItemFrame>		m_Trunk;
	TObjRef<GUISystem>				m_pGUI;
	TObjRef<Util>					m_pUtil;
	TObjRef<GameFrameMgr>			m_pFrameMgr;
	TObjRef<NetSession>				m_pSession;
	TObjRef<NetCmdMgr>				m_pCmdMgr;

	//38妇女节窗口
	GUIWnd*							m_pWnd_WomenFestval;	//主窗口
	GUIButton*                      m_pBn_Ok_Set;                 //确定
	GUIButton*                      m_pBn_Cancel_Set;             //取消
	GUIButton*                      m_pBn_Close_Set;              //关闭
	GUIEditBox*						m_pEb_FriendName;			//输入框
	GUIStatic*						m_pStc_Title;				//标题框
	GUIComboBox*					m_pCb_Friends_Box;				//好友列表组合框
	DWORD							m_curRow;
	std::vector<LPCTSTR>			m_vecFriendsArray;
	std::vector<tstring>			m_vecPetArray;
	INT64				            m_n64NUM;						//暂时存储64位物品ID号所用

	//map<tstring ,DWORD>				m_mapName2pet;
	BOOL							m_bIsPetFood;
};
