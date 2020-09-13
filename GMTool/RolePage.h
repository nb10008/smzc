/////////////////////////////////////////////////////////////////////////////
// Name:        RolePage.h
// Created:     2009-05-20
// Info:		角色维护页面
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "UserFrame.h"

class MyListCtrl;

class RolePage : public wxPanel, public UserFrame
{
public:
	RolePage(wxBookCtrlBase *book, const wxString& label);
	const wxString& GetLabel() { return m_label; }

	void SetMyFrame(MyFrame* frame) { m_pMyFrame = frame; }

	//--按键事件
	void OnRoleResumeOperation(wxCommandEvent& event);
	void OnRoleProModifyOperation(wxCommandEvent& event);
	void OnRoleItemProOperation(wxCommandEvent& event);
	void OnRoleItemDelOperation(wxCommandEvent& event);
	void OnRoleItemResOperation(wxCommandEvent& event);
	void OnRoleClearBaibaoOperation(wxCommandEvent& event);
	void OnRoleTitleIssue(wxCommandEvent& event);
	void OnRoleTitleDel(wxCommandEvent& event);
	void OnRoleGetMove(wxCommandEvent& event);
	void OnClearAttPoint(wxCommandEvent& event);
	void OnRoleSpeakOff(wxCommandEvent& event);
	void OnRoleSpeakOn(wxCommandEvent& event);
	void OnRoleAddPKLog(wxCommandEvent& event);

	DWORD OnRolePageEvent(tagUserEvent* pGameEvent);
	DWORD OnRoleSpeakEvent(tagUserEvent* pGameEvent);

private:
	wxString				m_label;
	wxTextCtrl*				m_text_delrole;
	wxTextCtrl*				m_text_rolename;
	wxButton*				m_btn_okresume;

	wxTextCtrl*				m_text_prorole;
	wxTextCtrl*				m_text_level;
	wxTextCtrl*				m_text_exp;
	wxTextCtrl*				m_text_skillnum;
	wxTextCtrl*				m_text_money;
	wxTextCtrl*				m_text_yuanbao;
	wxTextCtrl*				m_text_attpoint;
	wxButton*				m_btn_okmodipro;
	//wxButton*				m_btn_attpoint_set;
	wxButton*				m_btn_clearattpoint;

	wxTextCtrl*				m_text_itemrole;
	wxComboBox*				m_combox_item;
	wxTextCtrl*				m_text_num;
	wxTextCtrl*				m_text_quality;
	wxButton*				m_btn_okitem;

	wxTextCtrl*				m_text_itemownername;
	//wxTextCtrl*				m_text_itemdelid;
	wxTextCtrl*				m_text_Item64IDStr;
	wxTextCtrl*				m_text_baibaoownername;
	wxButton*				m_btn_okitemdel;
	wxButton*				m_btn_clearbaibao;

	//wxTextCtrl*				m_text_resumeitemid;
	wxTextCtrl*				m_text_resumeitemidstr;
	wxTextCtrl*				m_text_targetrolename;
	wxButton*				m_btn_okitemresume;

	wxTextCtrl*				m_text_title_rolename;
	wxTextCtrl*				m_text_title_titleid;
	wxButton*				m_btn_oktitle;
	wxButton*				m_btn_deltitle;

	wxTextCtrl*				m_text_moverole_rolename;
	wxTextCtrl*				m_text_moverole_map;
	wxTextCtrl*				m_text_moverole_x;
	wxTextCtrl*				m_text_moverole_z;
	wxButton*				m_btn_moverole_ok;

	wxTextCtrl*				m_text_RoleName;
	wxButton*				m_add_SpeakOff;
	wxButton*				m_del_SpeakOff;

	wxTextCtrl*				m_text_PKlogRoleID;
	wxTextCtrl*				m_text_PKlogRoleName;
	wxButton*				m_btn_AddPKLog;

	MyFrame*				m_pMyFrame;	

	DECLARE_EVENT_TABLE()
};