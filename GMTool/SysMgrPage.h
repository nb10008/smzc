/////////////////////////////////////////////////////////////////////////////
// Name:       SystemMgr.h
// Created:     2011-03-25
// Info:		系统管理页面
/////////////////////////////////////////////////////////////////////////////

#ifndef SYSTEMMGRPAGE_H_
#define SYSTEMMGRPAGE_H_

class MyListCtrl;
class SysMgrPage : public wxPanel, public UserFrame
{
public:
	SysMgrPage(wxBookCtrlBase *book, const wxString& label);
	const wxString& GetLabel(){return m_label;}
	void SetMyFrame(MyFrame* frame){m_pMyFrame = frame; }

	//按键事件,添加/删除广告过滤词
	void OnAddFilterWords(wxCommandEvent& event);
	void OnDelFilterWords(wxCommandEvent& event);

	DWORD OnSysMgrPageEvent(tagUserEvent* pGameEvent);

protected:
private:
	wxString		m_label;
	MyFrame*		m_pMyFrame;
	wxTextCtrl*     m_Text_Add_Filter;
	wxTextCtrl*     m_Text_Del_Filter;


	wxButton*		m_Btn_Add_Ok;
	wxButton*		m_Btn_Del_Ok;

	DECLARE_EVENT_TABLE()
};


#endif