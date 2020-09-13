/////////////////////////////////////////////////////////////////////////////
// Program:     LoongGMTool
// Name:        LoongGMTool.h
// Created:     2009-02-04
/////////////////////////////////////////////////////////////////////////////

#ifndef LOONGGMTOOL_H_
#define LOONGGMTOOL_H_
#include "UserFrame.h"
#include "log.h"
class MyPageInfo;

extern tstring g_Tester;
extern CLog g_PMDLog;
extern CLog g_RNLog;
extern CLog g_CNLog;
extern CLog g_AFLog;
extern CLog g_MRLog;

//GMTool的页面枚举值
enum EGMToolPages
{
	EGMP_Start,
	EGMP_GMManger					= 0,//GM管理
	EGMP_AccountManager				= 1,//帐号管理
	EGMP_RoleInfo					= 2,//角色信息
	EGMP_Guild						= 3,//公会信息
	EGMP_Double						= 4,//开放双倍
	EGMP_AutoNotice					= 5,//自动循环跑马灯
	EGMP_RightNotice				= 6,//右下角公告
	EGMP_AutoChat					= 7,//聊天频道公告


	EGMP_End						= 8,
};
//程序入口
class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
};

//非模式对话框
class MyDialog : public wxDialog, public UserFrame
{
public:
	MyDialog(const wxString& title);

	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);
	//void OnTimer(wxTimerEvent& event);

	//--挂起2秒
	void Suspend();

	DWORD OnLoginEvent(tagUserEvent* pGameEvent);

private:
	//TextCtrl
	wxTextCtrl *m_textName;
	wxTextCtrl *m_textPassword;

	//Button
	wxButton   *m_btnOk;
	wxButton   *m_btnCancel;

	//wxTimer		m_timer;
	DWORD		m_dwConnectTime;
	BOOL		m_bConnect;

private:
	DECLARE_EVENT_TABLE()
};

#define MyFrame_ID 9527
//Frame
class MyFrame : public wxFrame, public UserFrame
{
public:
	MyFrame(wxWindow *parent, const wxString& title, INT nID);
	~MyFrame();
	
	void OnClose(wxCloseEvent& event);
	void OnSelectAll(wxCommandEvent& event);
	void OnSelectCheckListBox(wxCommandEvent& evet);

	//等到设置服务器列表
	wxCheckListBox* GetCheckListBox() { return m_lbox; }
	wxBookCtrlBase* GetBook() {return m_book;}

	void GetCheckListBoxChecked(std::vector<int>& vec);

	DWORD OnServerStatInfoEvent(tagUserEvent* pGameEvent);
	DWORD OnTimeLimited(tagUserEvent* pGameEvent);

private:
	//wxPanel		   *m_panel;
	wxScrolledWindow   *m_panel;
	wxBookCtrlBase *m_book;
	wxCheckListBox *m_lbox;
	wxCheckBox     *m_chkSelectAll;
	bool            m_bSelectAll;
	DECLARE_EVENT_TABLE()
};
#endif /* LOONGGMTOOL_H_ */