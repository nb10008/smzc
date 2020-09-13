/////////////////////////////////////////////////////////////////////////////
// Name:        GuildPage.h
// Created:     2011-01-11
// Info:		公会维护页面
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "UserFrame.h"

class MyListCtrl;

class GuildPage : public wxPanel, public UserFrame
{
public:
	GuildPage(wxBookCtrlBase *book, const wxString& label);
	const wxString& GetLabel() { return m_label; }

	void SetMyFrame(MyFrame* frame) { m_pMyFrame = frame; }

	//--按键事件
	void OnGuildChgMemLvl(wxCommandEvent& event);
	void OnGuildChgMoney(wxCommandEvent& event);
	void OnGuildChgState(wxCommandEvent& event);
	void OnGuildGodMiracleModify(wxCommandEvent& event);
	DWORD OnGuildPageEvent(tagUserEvent* pGameEvent);

	//DWORD OnGuildPageEvent(tagUserEvent* pGameEvent);

private:
	wxString				m_label;

	wxTextCtrl*				m_text_guildname1;
	wxTextCtrl*				m_text_guildname2;
	wxTextCtrl*				m_text_guildname3;
	wxTextCtrl*				m_text_guildname4;

	wxTextCtrl*				m_text_guildrolename;
	wxTextCtrl*				m_text_guildrolelvl;
	wxTextCtrl*				m_text_guildmoney;
	wxTextCtrl*				m_text_guildstate;

	wxButton*				m_btn_ok1;
	wxButton*				m_btn_ok2;
	wxButton*				m_btn_ok3;
	wxButton*				m_btn_ok4;

	wxComboBox*				m_combox_type;

	MyFrame*				m_pMyFrame;	

	DECLARE_EVENT_TABLE()
};